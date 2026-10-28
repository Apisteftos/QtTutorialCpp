// =============================================================================
// Event-Driven Architecture — C++23
//
// Example: Smart-panel Event Bus — a wall panel with a button, a network link and a display.
//
// EventBus  — central dispatcher, routes events to registered handlers
// Publishers — panel button, network monitor (emit events, know nothing of subscribers)
// Subscribers — SoundModule, DisplayModule, Logger (react to events independently)
//
// Key property: adding a new subscriber requires ZERO changes to publishers.
// This is the Open/Closed Principle at architectural level.
// =============================================================================

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <chrono>
#include <cassert>
#include <print>

// =============================================================================
// Event types and Event struct
// =============================================================================

enum class EventType {
    BUTTON_PRESSED,
    BUTTON_RELEASED,
    LINK_DOWN,
    LINK_UP,
    MESSAGE_RECEIVED,
    MESSAGE_READ
};

std::string to_string(EventType t) {
    switch (t) {
    case EventType::BUTTON_PRESSED:    return "BUTTON_PRESSED";
    case EventType::BUTTON_RELEASED:   return "BUTTON_RELEASED";
    case EventType::LINK_DOWN:      return "LINK_DOWN";
    case EventType::LINK_UP:        return "LINK_UP";
    case EventType::MESSAGE_RECEIVED:  return "MESSAGE_RECEIVED";
    case EventType::MESSAGE_READ:     return "MESSAGE_READ";
    default:                        return "UNKNOWN";
    }
}

struct Event {
    EventType   type;
    std::string payload;    // JSON-like string for demo; real systems often use a schema-based binary format
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

class SoundModule {
public:
    explicit SoundModule(EventBus& bus) {
        // Subscribe to relevant events
        bus.subscribe(EventType::BUTTON_PRESSED,
                      [this](const Event& e){ onButtonPressed(e); });
        bus.subscribe(EventType::BUTTON_RELEASED,
                      [this](const Event& e){ onButtonReleased(e); });
        bus.subscribe(EventType::LINK_DOWN,
                      [this](const Event& e){ onLinkDown(e); });
    }

    int chimeCount() const { return chime_count_; }

private:
    void onButtonPressed(const Event& e) {
        chime_count_++;
        std::println("  [SoundModule]   🔔  CHIME START — speaker active — {}", e.payload);
    }
    void onButtonReleased(const Event&) {
        std::println("  [SoundModule]   🔇  CHIME STOP  — speaker idle");
    }
    void onLinkDown(const Event&) {
        std::println("  [SoundModule]   ⚠️  Link down — muting output");
    }

    int chime_count_{0};
};

class DisplayModule {
public:
    explicit DisplayModule(EventBus& bus) {
        bus.subscribe(EventType::BUTTON_PRESSED,
                      [this](const Event& e){ onButtonPressed(e); });
        bus.subscribe(EventType::BUTTON_RELEASED,
                      [this](const Event&){ onButtonReleased(); });
        bus.subscribe(EventType::MESSAGE_RECEIVED,
                      [this](const Event& e){ onMessageReceived(e); });
        bus.subscribe(EventType::MESSAGE_READ,
                      [this](const Event&){ onMessageRead(); });
        bus.subscribe(EventType::LINK_DOWN,
                      [this](const Event&){ onLinkDown(); });
        bus.subscribe(EventType::LINK_UP,
                      [this](const Event&){ onLinkUp(); });
    }

private:
    void onButtonPressed(const Event& e) {
        std::println("  [Display]       🟢 BUTTON DOWN — {}", e.payload);
    }
    void onButtonReleased() {
        std::println("  [Display]       ⚪ BUTTON UP");
    }
    void onMessageReceived(const Event& e) {
        std::println("  [Display]       ✉️  NEW MESSAGE — {}", e.payload);
    }
    void onMessageRead() {
        std::println("  [Display]       📭 MESSAGE READ");
    }
    void onLinkDown() {
        std::println("  [Display]       🔴 LINK DOWN");
    }
    void onLinkUp() {
        std::println("  [Display]       🟢 LINK UP");
    }
};

class Logger {
public:
    explicit Logger(EventBus& bus) {
        // Logger subscribes to ALL event types
        for (auto type : {
                 EventType::BUTTON_PRESSED, EventType::BUTTON_RELEASED,
                 EventType::LINK_DOWN,   EventType::LINK_UP,
                 EventType::MESSAGE_RECEIVED, EventType::MESSAGE_READ
             }) {
            bus.subscribe(type, [this](const Event& e){ log(e); });
        }
    }

    int logCount() const { return log_count_; }

private:
    void log(const Event& e) {
        log_count_++;
        std::println("  [Logger]        📋 t={} {} | {}", e.timestamp, to_string(e.type), e.payload);
    }
    int log_count_{0};
};

// =============================================================================
// Publishers — emit events, know nothing about subscribers
// =============================================================================

class PanelButton {
public:
    explicit PanelButton(EventBus& bus) : bus_(bus) {}

    void press(const std::string& label, int ts) {
        bus_.emit({EventType::BUTTON_PRESSED,
                   "button=" + label + " user=KA1", ts});
    }
    void release(int ts) {
        bus_.emit({EventType::BUTTON_RELEASED, "", ts});
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
    std::println("=== Event-Driven Architecture — Smart Panel Demo ===");
    std::println("");

    // 1. Create the bus
    EventBus bus;

    // 2. Create subscribers (they self-register to the bus)
    SoundModule   sound(bus);
    DisplayModule display(bus);
    Logger        logger(bus);

    // 3. Create publishers
    PanelButton      button(bus);
    NetworkMonitor net(bus);

    // 4. Simulate button, message and link events
    int t = 0;

    std::println("--- Event: Button Press ---");
    button.press("front-door", ++t);

    std::println("");
    std::println("--- Event: Button Release ---");
    button.release(++t);

    std::println("");
    std::println("--- Event: Message Received ---");
    bus.emit({EventType::MESSAGE_RECEIVED, "sender=neighbour", ++t});

    std::println("");
    std::println("--- Event: Link Down ---");
    net.reportLinkDown("eth0", ++t);

    std::println("");
    std::println("--- Event: Link Up ---");
    net.reportLinkUp("eth0", ++t);

    std::println("");
    std::println("--- Event: Message Read ---");
    bus.emit({EventType::MESSAGE_READ, "", ++t});

    // Verify
    std::println("");
    std::println("=== Summary ===");
    std::println("Chime activations: {}", sound.chimeCount());
    std::println("Log entries     : {}", logger.logCount());

    assert(sound.chimeCount()  == 1);
    assert(logger.logCount() == 6);
    std::println("All assertions passed.");

    return 0;
}
