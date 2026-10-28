// =============================================================================
// Shared Printer Arbiter — Client State Machine + Exclusive-Access Arbitration — C++23
//
// A small architectural simulation: several clients share ONE printer. A client
// must ask the arbiter for exclusive access before it may send pages, and the
// arbiter queues requests that arrive while the printer is busy.
//
// Components:
//   PrinterClient   — the state machine (Idle→Connecting→Connected→Requesting→Printing etc.)
//   PrinterArbiter  — decides who has exclusive access (grant / deny + FIFO queue)
//   DataChannel     — simulates the page stream to the printer and status updates back
//   EventLog        — records all state transitions and events
//
// Patterns: State Machine + Event-Driven + Layered + Observer
// =============================================================================

#include <print>
#include <string>
#include <vector>
#include <queue>
#include <functional>
#include <cassert>
#include <stdexcept>
#include <optional>
#include <unordered_map>

// =============================================================================
// Client state machine states
// =============================================================================

enum class ClientState {
    Idle,
    Connecting,
    Connected,
    Requesting,
    Printing,
    Observing
};

std::string to_string(ClientState s) {
    switch (s) {
    case ClientState::Idle:            return "Idle";
    case ClientState::Connecting:         return "Connecting";
    case ClientState::Connected:          return "Connected";
    case ClientState::Requesting: return "Requesting";
    case ClientState::Printing:    return "Printing";
    case ClientState::Observing:       return "Observing";
    }
    return "?";
}

// =============================================================================
// Access control signals (simplified protocol messages)
// =============================================================================

enum class AccessSignal {
    AccessGranted,
    AccessDenied,
    DeviceBusy,     // another user is printing
    DeviceFree,      // printer released by other user
    AccessRevoked    // arbiter revokes our access
};

std::string to_string(AccessSignal s) {
    switch (s) {
    case AccessSignal::AccessGranted: return "ACCESS_GRANTED";
    case AccessSignal::AccessDenied:  return "ACCESS_DENIED";
    case AccessSignal::DeviceBusy:   return "DEVICE_BUSY";
    case AccessSignal::DeviceFree:    return "DEVICE_FREE";
    case AccessSignal::AccessRevoked: return "ACCESS_REVOKED";
    }
    return "?";
}

// =============================================================================
// Event log — records all transitions for audit/testing
// =============================================================================

struct LogEntry {
    std::string component;
    std::string event;
};

class EventLog {
public:
    void record(const std::string& component, const std::string& event) {
        entries_.push_back({component, event});
        std::println("  [{}] {}", component, event);
    }

    bool contains(const std::string& event) const {
        for (const auto& e : entries_)
            if (e.event.find(event) != std::string::npos) return true;
        return false;
    }

    int count(const std::string& keyword) const {
        int n = 0;
        for (const auto& e : entries_)
            if (e.event.find(keyword) != std::string::npos) n++;
        return n;
    }

    const std::vector<LogEntry>& entries() const { return entries_; }

private:
    std::vector<LogEntry> entries_;
};

// =============================================================================
// Data Channel — simulates the page stream
// =============================================================================

class DataChannel {
public:
    explicit DataChannel(EventLog& log) : log_(log) {}

    void beginSend(const std::string& printerId, const std::string& userId) {
        transmitting_ = true;
        pages_sent_ = 0;
        log_.record("DataChannel", "send started — printer=" + printerId + " user=" + userId);
    }

    void sendPage() {
        if (transmitting_) {
            pages_sent_++;
        }
    }

    void endSend() {
        transmitting_ = false;
        log_.record("DataChannel",
                    "send finished — " + std::to_string(pages_sent_) + " pages sent");
    }

    void beginReceive(const std::string& fromUser) {
        receiving_ = true;
        updates_received_ = 0;
        log_.record("DataChannel", "status feed started — printing user=" + fromUser);
    }

    void receiveUpdate() {
        if (receiving_) updates_received_++;
    }

    void endReceive() {
        receiving_ = false;
        log_.record("DataChannel",
                    "status feed stopped — " + std::to_string(updates_received_) + " updates received");
    }

    bool isPrinting() const { return transmitting_; }
    bool isObserving()    const { return receiving_; }
    int  pagesSent()      const { return pages_sent_; }
    int  updatesReceived()      const { return updates_received_; }

private:
    EventLog& log_;
    bool transmitting_{false};
    bool receiving_{false};
    int  pages_sent_{0};
    int  updates_received_{0};
};

// =============================================================================
// Printer Arbiter — arbitrates exclusive printer access
// =============================================================================

class PrinterClient;  // forward declaration

class PrinterArbiter {
public:
    explicit PrinterArbiter(EventLog& log) : log_(log) {}

    void registerClient(const std::string& userId, PrinterClient* client) {
        clients_[userId] = client;
    }

    // Called when a client requests access
    AccessSignal requestAccess(const std::string& requesterId);

    // Called when a client releases access
    void releaseAccess(const std::string& userId);

    const std::string& currentHolder() const { return current_holder_; }
    bool               deviceIdle()     const { return current_holder_.empty(); }

private:
    void notifyDeviceBusy(const std::string& activeUserId);
    void notifyDeviceFree();

    EventLog&     log_;
    std::string   current_holder_;
    std::queue<std::string>                           wait_queue_;
    std::unordered_map<std::string, PrinterClient*>     clients_;
};

// =============================================================================
// Printer Client — the state machine
// =============================================================================

class PrinterClient {
public:
    PrinterClient(const std::string& userId,
                PrinterArbiter& arbiter,
                DataChannel& media,
                EventLog& log)
        : userId_(userId), arbiter_(arbiter), media_(media), log_(log)
    {
        arbiter_.registerClient(userId_, this);
    }

    // ── User actions ──────────────────────────────────────────────────────────

    void connect(const std::string& printerId) {
        assertState(ClientState::Idle, "connect");
        printerId_ = printerId;
        transition(ClientState::Connecting);
        log_.record(userId_, "→ CONNECT_REQUEST printer=" + printerId);

        // Simulate server accepting immediately (synchronous for demo)
        onConnected();
    }

    void startJob() {
        // A client that is only observing may still ask for access
        if (state_ != ClientState::Connected && state_ != ClientState::Observing)
            throw std::runtime_error(userId_ + "::startJob called in wrong state: "
                                     + to_string(state_) + " (expected Connected or Observing)");
        if (state_ == ClientState::Observing) {
            media_.endReceive();   // stop the status feed before requesting access
        }
        transition(ClientState::Requesting);
        log_.record(userId_, "→ ACCESS_REQUEST");

        auto signal = arbiter_.requestAccess(userId_);
        onAccessSignal(signal);
    }

    void finishJob() {
        assertState(ClientState::Printing, "finishJob");
        log_.record(userId_, "→ ACCESS_RELEASE");
        media_.endSend();
        transition(ClientState::Connected);   // move to Connected FIRST
        arbiter_.releaseAccess(userId_);      // then release — may trigger DeviceBusy
        // NOTE: if another queued user gets access, onAccessSignal(DeviceBusy)
        // will override Active→Observing. That is correct behaviour.
    }

    void disconnect() {
        if (state_ == ClientState::Printing) {
            media_.endSend();
            arbiter_.releaseAccess(userId_);
        }
        if (state_ == ClientState::Observing) {
            media_.endReceive();
        }
        transition(ClientState::Idle);
        log_.record(userId_, "→ DISCONNECT printer=" + printerId_);
        printerId_ = "";
    }

    // ── Network/arbiter events (called by PrinterArbiter) ─────────────────────

    void onAccessSignal(AccessSignal signal) {
        log_.record(userId_, "← " + to_string(signal));

        switch (signal) {
        case AccessSignal::AccessGranted:
            // Accept from Requesting (normal) OR Connected (queued grant)
            if (state_ == ClientState::Requesting ||
                state_ == ClientState::Connected) {
                transition(ClientState::Printing);
                media_.beginSend(printerId_, userId_);
                // Simulate sending 3 pages
                media_.sendPage();
                media_.sendPage();
                media_.sendPage();
            }
            break;

        case AccessSignal::AccessDenied:
            if (state_ == ClientState::Requesting) {
                log_.record(userId_, "printer busy — waiting in queue");
                transition(ClientState::Connected);
            }
            break;

        case AccessSignal::DeviceBusy:
            if (state_ == ClientState::Connected) {
                transition(ClientState::Observing);
                media_.beginReceive(arbiter_.currentHolder());
                // Simulate receiving 5 status updates
                media_.receiveUpdate();
                media_.receiveUpdate();
                media_.receiveUpdate();
                media_.receiveUpdate();
                media_.receiveUpdate();
            }
            break;

        case AccessSignal::DeviceFree:
            if (state_ == ClientState::Observing) {
                media_.endReceive();
                transition(ClientState::Connected);
            }
            break;

        case AccessSignal::AccessRevoked:
            if (state_ == ClientState::Printing) {
                media_.endSend();
                transition(ClientState::Connected);
            }
            break;
        }
    }

    // ── Accessors ─────────────────────────────────────────────────────────────

    ClientState        state()  const { return state_; }
    const std::string& userId() const { return userId_; }
    int                stateChangeCount() const { return state_changes_; }

private:
    void onConnected() {
        transition(ClientState::Connected);
        log_.record(userId_, "← connected printer=" + printerId_);
    }

    void transition(ClientState next) {
        log_.record(userId_,
                    "STATE: " + to_string(state_) + " → " + to_string(next));
        state_ = next;
        state_changes_++;
    }

    void assertState(ClientState expected, const std::string& action) const {
        if (state_ != expected)
            throw std::runtime_error(
                userId_ + "::" + action + " called in wrong state: " +
                to_string(state_) + " (expected " + to_string(expected) + ")");
    }

    std::string     userId_;
    std::string     printerId_;
    ClientState     state_{ClientState::Idle};
    int             state_changes_{0};
    PrinterArbiter& arbiter_;
    DataChannel&      media_;
    EventLog&        log_;
};

// =============================================================================
// PrinterArbiter method bodies (need PrinterClient complete)
// =============================================================================

AccessSignal PrinterArbiter::requestAccess(const std::string& requesterId) {
    log_.record("Arbiter", "ACCESS_REQUEST from " + requesterId);

    if (current_holder_.empty()) {
        current_holder_ = requesterId;
        log_.record("Arbiter", "ACCESS_GRANTED to " + requesterId);

        // Notify all other clients that the printer is busy
        notifyDeviceBusy(requesterId);
        return AccessSignal::AccessGranted;
    } else {
        wait_queue_.push(requesterId);
        log_.record("Arbiter", "ACCESS_DENIED — printer held by " + current_holder_
                                     + " (queue=" + std::to_string(wait_queue_.size()) + ")");
        return AccessSignal::AccessDenied;
    }
}

void PrinterArbiter::releaseAccess(const std::string& userId) {
    if (current_holder_ != userId) return;

    log_.record("Arbiter", "ACCESS_RELEASE from " + userId);
    current_holder_ = "";

    if (!wait_queue_.empty()) {
        // Grant to next in queue
        std::string next = wait_queue_.front(); wait_queue_.pop();
        current_holder_ = next;
        log_.record("Arbiter", "ACCESS_GRANTED (queued) to " + next);
        if (clients_.count(next))
            clients_[next]->onAccessSignal(AccessSignal::AccessGranted);
        notifyDeviceBusy(next);
    } else {
        notifyDeviceFree();
    }
}

void PrinterArbiter::notifyDeviceBusy(const std::string& activeUserId) {
    for (auto& [id, client] : clients_) {
        if (id != activeUserId)
            client->onAccessSignal(AccessSignal::DeviceBusy);
    }
}

void PrinterArbiter::notifyDeviceFree() {
    log_.record("Arbiter", "DEVICE_FREE — broadcast to all");
    for (auto& [id, client] : clients_)
        client->onAccessSignal(AccessSignal::DeviceFree);
}

// =============================================================================
// main
// =============================================================================

int main()
{
    std::println("=== Shared Printer Arbiter Demo ===");
    std::println("");

    EventLog        log;
    PrinterArbiter arbiter(log);
    DataChannel      media(log);

    // Two clients using the same printer
    PrinterClient alice("Alice", arbiter, media, log);
    PrinterClient bob  ("Bob",   arbiter, media, log);

    // ── Scenario 1: Normal job ───────────────────────────────────────
    std::println("");
    std::println("--- Scenario 1: Alice connects and starts a job ---");
    alice.connect("printer-1");
    bob.connect("printer-1");

    assert(alice.state() == ClientState::Connected);
    assert(bob.state()   == ClientState::Connected);

    alice.startJob();  // Alice requests access → granted
    assert(alice.state() == ClientState::Printing);
    assert(bob.state()   == ClientState::Observing);

    alice.finishJob();  // Alice finishes → printer free
    assert(alice.state() == ClientState::Connected);
    assert(bob.state()   == ClientState::Connected);
    std::println("✓ Scenario 1 passed");

    // ── Scenario 2: Contention — Bob queued ────────────────────────────
    std::println("");
    std::println("--- Scenario 2: Alice prints, Bob asks for access (gets queued) ---");
    alice.startJob();
    assert(alice.state() == ClientState::Printing);

    bob.startJob();    // Bob asks while Alice has access → DENIED, queued
    assert(bob.state() == ClientState::Connected);   // back to Connected, waiting in queue

    alice.finishJob();  // Alice finishes → Bob's queued request is auto-granted
    assert(alice.state() == ClientState::Observing);  // Alice now observes Bob
    assert(bob.state()   == ClientState::Printing);

    bob.finishJob();
    assert(bob.state()   == ClientState::Connected);
    assert(alice.state() == ClientState::Connected);
    std::println("✓ Scenario 2 passed");

    // ── Scenario 3: Disconnect ───────────────────────────────────────────────
    std::println("");
    std::println("--- Scenario 3: Both disconnect ---");
    alice.disconnect();
    bob.disconnect();
    assert(alice.state() == ClientState::Idle);
    assert(bob.state()   == ClientState::Idle);
    std::println("✓ Scenario 3 passed");

    // ── Scenario 4: Error — job without connecting ───────────────────────────────
    std::println("");
    std::println("--- Scenario 4: Error handling ---");
    bool caught = false;
    try { alice.startJob(); }
    catch (const std::runtime_error& e) {
        caught = true;
        std::println("  ✓ caught: {}", e.what());
    }
    assert(caught);

    // ── Summary ───────────────────────────────────────────────────────────────
    std::println("");
    std::println("=== Summary ===");
    std::println("Alice state changes: {}", alice.stateChangeCount());
    std::println("Bob   state changes: {}", bob.stateChangeCount());
    std::println("Pages sent         : {}", media.pagesSent());
    std::println("Status updates     : {}", media.updatesReceived());
    std::println("Access requests    : {}", log.count("ACCESS_REQUEST"));
    std::println("ACCESS_GRANTED     : {}", log.count("ACCESS_GRANTED"));

    assert(media.pagesSent()          == 3);   // 3 pages in 1 completed send
    assert(media.updatesReceived()          == 5);   // 5 updates in 1 completed status feed
    assert(log.count("ACCESS_REQUEST") == 6);   // 4 user requests + 2 from queued grants
    assert(log.count("ACCESS_GRANTED") == 6);

    std::println("");
    std::println("All assertions passed.");
    return 0;
}
