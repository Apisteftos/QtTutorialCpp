// =============================================================================
// Event-Driven Architecture — C++23
//
// Example: PTT (Push-to-Talk) Event Bus — inspired by MCX/MCPTT systems.
//
// EventBus  — central dispatcher, routes events to registered handlers
// Publishers — PTT button, network monitor (emit events, know nothing of subscribers)
// Subscribers — AudioEngine, DisplayModule, Logger (react to events independently)
//
// Key property: adding a new subscriber requires ZERO changes to publishers.
// This is the Open/Closed Principle at architectural level.
// =============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <chrono>
#include <cassert>

// =============================================================================
// Event types and Event struct
// =============================================================================

enum class EventType {
    PTT_PRESSED,
    PTT_RELEASED,
    LINK_DOWN,
    LINK_UP,
    CALL_INCOMING,
    CALL_ENDED
};

std::string to_string(EventType t) {
    switch (t) {
    case EventType::PTT_PRESSED:    return "PTT_PRESSED";
    case EventType::PTT_RELEASED:   return "PTT_RELEASED";
    case EventType::LINK_DOWN:      return "LINK_DOWN";
    case EventType::LINK_UP:        return "LINK_UP";
    case EventType::CALL_INCOMING:  return "CALL_INCOMING";
    case EventType::CALL_ENDED:     return "CALL_ENDED";
    default:                        return "UNKNOWN";
    }
}

struct Event {
    EventType   type;
    std::string payload;    // JSON-like string for demo; real MCX uses ASN.1
    int         timestamp;  // simplified: just a counter
};

// =============================================================================
// Event Bus — the core of the architecture
// =============================================================================

using Handler = std::function<void(const Event&)>;

class EventBus {
public:
    // Subscribe: register a handler for an event type
    void subscribe(EventType type, Handler handler) {
        handlers_[type].push_back(std::move(handler));
    }

    // Emit: publish an event — bus routes it to all subscribers
    void emit(const Event& event) {
        auto it = handlers_.find(event.type);
        if (it == handlers_.end()) return;   // no subscribers for this type
        for (const auto& handler : it->second)
            handler(event);
    }

    int subscriberCount(EventType type) const {
        auto it = handlers_.find(type);
        return it == handlers_.end() ? 0 : static_cast<int>(it->second.size());
    }

private:
    std::unordered_map<EventType, std::vector<Handler>> handlers_;
};

// =============================================================================
// Subscribers — completely independent, know nothing about each other
// =============================================================================

class AudioEngine {
public:
    explicit AudioEngine(EventBus& bus) {
        // Subscribe to relevant events
        bus.subscribe(EventType::PTT_PRESSED,
                      [this](const Event& e){ onPttPressed(e); });
        bus.subscribe(EventType::PTT_RELEASED,
                      [this](const Event& e){ onPttReleased(e); });
        bus.subscribe(EventType::LINK_DOWN,
                      [this](const Event& e){ onLinkDown(e); });
    }

    int txCount() const { return tx_count_; }

private:
    void onPttPressed(const Event& e) {
        tx_count_++;
        std::cout << "  [AudioEngine]   🎙️  TX START — codec active — "
                  << e.payload << '\n';
    }
    void onPttReleased(const Event&) {
        std::cout << "  [AudioEngine]   🔇  TX STOP  — codec idle\n";
    }
    void onLinkDown(const Event&) {
        std::cout << "  [AudioEngine]   ⚠️  Link down — muting output\n";
    }

    int tx_count_{0};
};

class DisplayModule {
public:
    explicit DisplayModule(EventBus& bus) {
        bus.subscribe(EventType::PTT_PRESSED,
                      [this](const Event& e){ onPttPressed(e); });
        bus.subscribe(EventType::PTT_RELEASED,
                      [this](const Event&){ onPttReleased(); });
        bus.subscribe(EventType::CALL_INCOMING,
                      [this](const Event& e){ onCallIncoming(e); });
        bus.subscribe(EventType::CALL_ENDED,
                      [this](const Event&){ onCallEnded(); });
        bus.subscribe(EventType::LINK_DOWN,
                      [this](const Event&){ onLinkDown(); });
        bus.subscribe(EventType::LINK_UP,
                      [this](const Event&){ onLinkUp(); });
    }

private:
    void onPttPressed(const Event& e) {
        std::cout << "  [Display]       🟢 PTT ACTIVE — " << e.payload << '\n';
    }
    void onPttReleased() {
        std::cout << "  [Display]       ⚪ PTT IDLE\n";
    }
    void onCallIncoming(const Event& e) {
        std::cout << "  [Display]       📞 INCOMING CALL — " << e.payload << '\n';
    }
    void onCallEnded() {
        std::cout << "  [Display]       📵 CALL ENDED\n";
    }
    void onLinkDown() {
        std::cout << "  [Display]       🔴 LINK DOWN\n";
    }
    void onLinkUp() {
        std::cout << "  [Display]       🟢 LINK UP\n";
    }
};

class Logger {
public:
    explicit Logger(EventBus& bus) {
        // Logger subscribes to ALL event types
        for (auto type : {
                 EventType::PTT_PRESSED, EventType::PTT_RELEASED,
                 EventType::LINK_DOWN,   EventType::LINK_UP,
                 EventType::CALL_INCOMING, EventType::CALL_ENDED
             }) {
            bus.subscribe(type, [this](const Event& e){ log(e); });
        }
    }

    int logCount() const { return log_count_; }

private:
    void log(const Event& e) {
        log_count_++;
        std::cout << "  [Logger]        📋 t=" << e.timestamp
                  << " " << to_string(e.type)
                  << " | " << e.payload << '\n';
    }
    int log_count_{0};
};

// =============================================================================
// Publishers — emit events, know nothing about subscribers
// =============================================================================

class PttButton {
public:
    explicit PttButton(EventBus& bus) : bus_(bus) {}

    void press(const std::string& group, int ts) {
        bus_.emit({EventType::PTT_PRESSED,
                   "group=" + group + " user=KA1", ts});
    }
    void release(int ts) {
        bus_.emit({EventType::PTT_RELEASED, "", ts});
    }

private:
    EventBus& bus_;
};

class NetworkMonitor {
public:
    explicit NetworkMonitor(EventBus& bus) : bus_(bus) {}

    void reportLinkDown(const std::string& iface, int ts) {
        bus_.emit({EventType::LINK_DOWN, "iface=" + iface, ts});
    }
    void reportLinkUp(const std::string& iface, int ts) {
        bus_.emit({EventType::LINK_UP, "iface=" + iface, ts});
    }

private:
    EventBus& bus_;
};

// =============================================================================
// main
// =============================================================================

int main()
{
    std::cout << "=== Event-Driven Architecture — MCX PTT Demo ===\n\n";

    // 1. Create the bus
    EventBus bus;

    // 2. Create subscribers (they self-register to the bus)
    AudioEngine   audio(bus);
    DisplayModule display(bus);
    Logger        logger(bus);

    // 3. Create publishers
    PttButton      ptt(bus);
    NetworkMonitor net(bus);

    // 4. Simulate a PTT session
    int t = 0;

    std::cout << "--- Event: PTT Press ---\n";
    ptt.press("ALPHA-1", ++t);

    std::cout << "\n--- Event: PTT Release ---\n";
    ptt.release(++t);

    std::cout << "\n--- Event: Incoming Call ---\n";
    bus.emit({EventType::CALL_INCOMING, "caller=BASE-COMMAND", ++t});

    std::cout << "\n--- Event: Link Down ---\n";
    net.reportLinkDown("eth0", ++t);

    std::cout << "\n--- Event: Link Up ---\n";
    net.reportLinkUp("eth0", ++t);

    std::cout << "\n--- Event: Call Ended ---\n";
    bus.emit({EventType::CALL_ENDED, "", ++t});

    // Verify
    std::cout << "\n=== Summary ===\n";
    std::cout << "TX activations : " << audio.txCount()  << "\n";
    std::cout << "Log entries    : " << logger.logCount() << "\n";

    assert(audio.txCount()  == 1);
    assert(logger.logCount() == 6);
    std::cout << "All assertions passed.\n";

    return 0;
}
