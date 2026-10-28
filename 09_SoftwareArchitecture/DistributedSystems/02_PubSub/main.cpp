// =============================================================================
// Publish-Subscribe Pattern — C++23
//
// Topic-based broker with wildcard matching ("weather.*" matches all subtopics).
//
// Broker   — stores subscriptions, routes messages by topic
// Publisher — emits messages on topics (WeatherStation, TrafficSensor)
// Subscriber — registers handlers for topics (MobileApp, Dashboard, AlertService)
//
// Production equivalent: MQTT broker, RabbitMQ topic exchange, Kafka
// =============================================================================

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <cassert>
#include <algorithm>
#include <sstream>
#include <print>

// =============================================================================
// Message
// =============================================================================

struct Message {
    std::string topic;
    std::string payload;
    int         timestamp{0};  // simplified counter
};

// =============================================================================
// Broker — topic routing with wildcard support
// =============================================================================

using Handler = std::function<void(const Message&)>;

class Broker {
public:
    // Subscribe to exact topic or wildcard ending in ".*"
    // e.g. "weather.update" or "weather.*" or "#" (all topics)
    void subscribe(const std::string& topicPattern,
                   const std::string& subscriberName,
                   Handler handler) {
        subscriptions_.push_back({topicPattern, subscriberName, std::move(handler)});
        std::println("  [Broker] {} subscribed to \"{}\"", subscriberName, topicPattern);
    }

    void unsubscribe(const std::string& subscriberName) {
        subscriptions_.erase(
            std::remove_if(subscriptions_.begin(), subscriptions_.end(),
                           [&](const Sub& s){ return s.name == subscriberName; }),
            subscriptions_.end());
    }

    // Publish a message — routes to all matching subscribers
    void publish(const std::string& topic, const std::string& payload) {
        msg_count_++;
        Message msg{topic, payload, msg_count_};
        std::println("");
        std::println("  [Broker] publish [{}] {}", topic, payload);

        int dispatched = 0;
        for (auto& sub : subscriptions_) {
            if (matches(sub.pattern, topic)) {
                sub.handler(msg);
                dispatched++;
            }
        }
        if (dispatched == 0)
            std::println("  [Broker] no subscribers for \"{}\"", topic);

        dispatch_counts_[topic] += dispatched;
    }

    int totalMessages()              const { return msg_count_; }
    int dispatchesFor(const std::string& t) const {
        auto it = dispatch_counts_.find(t);
        return it != dispatch_counts_.end() ? it->second : 0;
    }
    int subscriberCount()            const { return static_cast<int>(subscriptions_.size()); }

private:
    // Topic pattern matching:
    //   "#"         → matches everything
    //   "a.*"       → matches "a.x", "a.y" but not "b.x"
    //   "a.update"  → exact match only
    bool matches(const std::string& pattern, const std::string& topic) const {
        if (pattern == "#") return true;
        if (pattern == topic) return true;
        // Wildcard: pattern ends with ".*"
        if (pattern.size() >= 2 &&
            pattern.substr(pattern.size() - 2) == ".*") {
            std::string prefix = pattern.substr(0, pattern.size() - 2);
            return topic.substr(0, prefix.size()) == prefix;
        }
        return false;
    }

    struct Sub {
        std::string pattern;
        std::string name;
        Handler     handler;
    };

    std::vector<Sub>                         subscriptions_;
    std::unordered_map<std::string, int>     dispatch_counts_;
    int                                      msg_count_{0};
};

// =============================================================================
// Subscribers — each tracks what it received
// =============================================================================

class MobileApp {
public:
    explicit MobileApp(Broker& broker) {
        broker.subscribe("weather.*", "MobileApp",
                         [this](const Message& m){ onWeather(m); });
    }

    int weatherUpdates() const { return weather_count_; }

private:
    void onWeather(const Message& m) {
        weather_count_++;
        std::println("    [MobileApp]   📱 weather received: {}", m.payload);
    }
    int weather_count_{0};
};

class Dashboard {
public:
    explicit Dashboard(Broker& broker) {
        broker.subscribe("weather.*", "Dashboard",
                         [this](const Message& m){ onMessage(m, "🌤 weather"); });
        broker.subscribe("traffic.*", "Dashboard",
                         [this](const Message& m){ onMessage(m, "🚦 traffic"); });
    }

    int totalReceived() const { return total_; }

private:
    void onMessage(const Message& m, const std::string& label) {
        total_++;
        std::println("    [Dashboard]   {}: {}", label, m.payload);
    }
    int total_{0};
};

class AlertService {
public:
    explicit AlertService(Broker& broker) {
        // Only subscribes to alert subtopics — not all weather/traffic
        broker.subscribe("weather.alert", "AlertService",
                         [this](const Message& m){ onAlert("WEATHER", m); });
        broker.subscribe("traffic.jam",   "AlertService",
                         [this](const Message& m){ onAlert("TRAFFIC", m); });
    }

    int alertCount() const { return alert_count_; }

private:
    void onAlert(const std::string& type, const Message& m) {
        alert_count_++;
        std::println("    [AlertService] 🚨 ALERT [{}]: {}", type, m.payload);
    }
    int alert_count_{0};
};

class AuditLogger {
public:
    explicit AuditLogger(Broker& broker) {
        broker.subscribe("#", "AuditLogger",   // wildcard — all topics
                         [this](const Message& m){ onAny(m); });
    }

    int logCount() const { return log_count_; }

private:
    void onAny(const Message& m) {
        log_count_++;
        std::println("    [AuditLogger]  📋 t={} [{}] {}", m.timestamp, m.topic, m.payload);
    }
    int log_count_{0};
};

// =============================================================================
// Publishers
// =============================================================================

class WeatherStation {
public:
    explicit WeatherStation(Broker& broker) : broker_(broker) {}

    void reportUpdate(const std::string& data) {
        broker_.publish("weather.update", data);
    }
    void reportAlert(const std::string& data) {
        broker_.publish("weather.alert", data);
    }

private:
    Broker& broker_;
};

class TrafficSensor {
public:
    explicit TrafficSensor(Broker& broker) : broker_(broker) {}

    void reportJam(const std::string& location) {
        broker_.publish("traffic.jam", "jam at " + location);
    }
    void reportClear(const std::string& location) {
        broker_.publish("traffic.clear", "clear at " + location);
    }

private:
    Broker& broker_;
};

// =============================================================================
// main
// =============================================================================

int main()
{
    std::println("=== Publish-Subscribe Pattern Demo ===");
    std::println("");

    // ── Setup ─────────────────────────────────────────────────────────────────
    std::println("--- Subscriptions ---");
    Broker broker;

    // Subscribers self-register
    MobileApp    mobile(broker);
    Dashboard    dashboard(broker);
    AlertService alerts(broker);
    AuditLogger  audit(broker);

    assert(broker.subscriberCount() == 6);  // mobile(1) + dashboard(2) + alerts(2) + audit(1)
    std::println("  {} subscriptions registered", broker.subscriberCount());

    // ── Publish events ────────────────────────────────────────────────────────
    std::println("");
    std::println("--- Publishing events ---");

    WeatherStation weather(broker);
    TrafficSensor  traffic(broker);

    weather.reportUpdate("temp=22C humidity=65%");
    // → MobileApp, Dashboard, AuditLogger (weather.* matches)
    // → AlertService does NOT receive (not weather.alert)

    weather.reportAlert("storm warning: wind 120km/h");
    // → MobileApp, Dashboard, AlertService, AuditLogger

    traffic.reportJam("Autobahn A2 km 247");
    // → Dashboard, AlertService, AuditLogger

    traffic.reportClear("Autobahn A2 km 247");
    // → Dashboard, AuditLogger (AlertService does NOT subscribe to traffic.clear)

    broker.publish("system.heartbeat", "uptime=3600s");
    // → AuditLogger only (wildcard #) — no other subscriber

    // ── Assertions ────────────────────────────────────────────────────────────
    std::println("");
    std::println("=== Summary ===");
    std::println("Total messages published : {}", broker.totalMessages());
    std::println("MobileApp weather updates: {}", mobile.weatherUpdates());
    std::println("Dashboard total received : {}", dashboard.totalReceived());
    std::println("AlertService alerts fired: {}", alerts.alertCount());
    std::println("AuditLogger log entries  : {}", audit.logCount());

    assert(broker.totalMessages()    == 5);
    assert(mobile.weatherUpdates()   == 2);   // weather.update + weather.alert
    assert(dashboard.totalReceived() == 4);   // 2 weather + 2 traffic
    assert(alerts.alertCount()       == 2);   // weather.alert + traffic.jam
    assert(audit.logCount()          == 5);   // all messages (wildcard #)

    std::println("");
    std::println("All assertions passed.");
    return 0;
}
