// =============================================================================
// PTT Simplified — MCX/MCPTT Group Call State Machine — C++23
//
// A faithful architectural simulation of an MCPTT client state machine.
// Standards basis: ETSI TS 124 380, 3GPP TS 23.280
//
// Components:
//   McpttClient      — the state machine (Idle→Joining→Active→Transmitting etc.)
//   FloorController  — arbitrates who has the floor (speaks)
//   MediaPlane       — simulates RTP TX/RX audio stream
//   GroupServer      — simulates MCX core network (call setup)
//   EventLog         — records all state transitions and events
//
// Patterns: State Machine + Event-Driven + Layered + Observer
// =============================================================================

#include <iostream>
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
    Joining,
    Active,
    FloorRequesting,
    Transmitting,
    Receiving
};

std::string to_string(ClientState s) {
    switch (s) {
    case ClientState::Idle:            return "Idle";
    case ClientState::Joining:         return "Joining";
    case ClientState::Active:          return "Active";
    case ClientState::FloorRequesting: return "FloorRequesting";
    case ClientState::Transmitting:    return "Transmitting";
    case ClientState::Receiving:       return "Receiving";
    }
    return "?";
}

// =============================================================================
// Floor control signals (simplified MCCP messages)
// =============================================================================

enum class FloorSignal {
    FloorGranted,
    FloorDenied,
    FloorTaken,     // another user is transmitting
    FloorIdle,      // floor released by other user
    FloorRevoked    // server revokes our floor
};

std::string to_string(FloorSignal s) {
    switch (s) {
    case FloorSignal::FloorGranted: return "FLOOR_GRANTED";
    case FloorSignal::FloorDenied:  return "FLOOR_DENIED";
    case FloorSignal::FloorTaken:   return "FLOOR_TAKEN";
    case FloorSignal::FloorIdle:    return "FLOOR_IDLE";
    case FloorSignal::FloorRevoked: return "FLOOR_REVOKED";
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
        std::cout << "  [" << component << "] " << event << '\n';
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
// Media Plane — simulates RTP audio
// =============================================================================

class MediaPlane {
public:
    explicit MediaPlane(EventLog& log) : log_(log) {}

    void startTx(const std::string& callId, const std::string& userId) {
        transmitting_ = true;
        tx_packets_ = 0;
        log_.record("MediaPlane", "RTP TX started — call=" + callId + " user=" + userId);
    }

    void sendPacket() {
        if (transmitting_) {
            tx_packets_++;
        }
    }

    void stopTx() {
        transmitting_ = false;
        log_.record("MediaPlane",
                    "RTP TX stopped — " + std::to_string(tx_packets_) + " packets sent");
    }

    void startRx(const std::string& fromUser) {
        receiving_ = true;
        rx_packets_ = 0;
        log_.record("MediaPlane", "RTP RX started — speaker=" + fromUser);
    }

    void receivePacket() {
        if (receiving_) rx_packets_++;
    }

    void stopRx() {
        receiving_ = false;
        log_.record("MediaPlane",
                    "RTP RX stopped — " + std::to_string(rx_packets_) + " packets received");
    }

    bool isTransmitting() const { return transmitting_; }
    bool isReceiving()    const { return receiving_; }
    int  txPackets()      const { return tx_packets_; }
    int  rxPackets()      const { return rx_packets_; }

private:
    EventLog& log_;
    bool transmitting_{false};
    bool receiving_{false};
    int  tx_packets_{0};
    int  rx_packets_{0};
};

// =============================================================================
// Floor Controller — arbitrates PTT floor
// =============================================================================

class McpttClient;  // forward declaration

class FloorController {
public:
    explicit FloorController(EventLog& log) : log_(log) {}

    void registerClient(const std::string& userId, McpttClient* client) {
        clients_[userId] = client;
    }

    // Called when a client requests the floor
    FloorSignal requestFloor(const std::string& requesterId);

    // Called when a client releases the floor
    void releaseFloor(const std::string& userId);

    const std::string& currentHolder() const { return current_holder_; }
    bool               floorIdle()     const { return current_holder_.empty(); }

private:
    void notifyFloorTaken(const std::string& speakerId);
    void notifyFloorIdle();

    EventLog&     log_;
    std::string   current_holder_;
    std::queue<std::string>                           wait_queue_;
    std::unordered_map<std::string, McpttClient*>     clients_;
};

// =============================================================================
// MCPTT Client — the state machine
// =============================================================================

class McpttClient {
public:
    McpttClient(const std::string& userId,
                FloorController& floor,
                MediaPlane& media,
                EventLog& log)
        : userId_(userId), floor_(floor), media_(media), log_(log)
    {
        floor_.registerClient(userId_, this);
    }

    // ── User actions ──────────────────────────────────────────────────────────

    void joinGroup(const std::string& groupId) {
        assertState(ClientState::Idle, "joinGroup");
        groupId_ = groupId;
        transition(ClientState::Joining);
        log_.record(userId_, "→ GROUP_CALL_REQUEST group=" + groupId);

        // Simulate server accepting immediately (synchronous for demo)
        onCallEstablished();
    }

    void pressPtt() {
        // In real MCX a user can request the floor while receiving (interrupt)
        if (state_ != ClientState::Active && state_ != ClientState::Receiving)
            throw std::runtime_error(userId_ + "::pressPtt called in wrong state: "
                                     + to_string(state_) + " (expected Active or Receiving)");
        if (state_ == ClientState::Receiving) {
            media_.stopRx();   // stop receiving before requesting floor
        }
        transition(ClientState::FloorRequesting);
        log_.record(userId_, "→ FLOOR_REQUEST");

        auto signal = floor_.requestFloor(userId_);
        onFloorSignal(signal);
    }

    void releasePtt() {
        assertState(ClientState::Transmitting, "releasePtt");
        log_.record(userId_, "→ FLOOR_RELEASE");
        media_.stopTx();
        transition(ClientState::Active);   // move to Active FIRST
        floor_.releaseFloor(userId_);      // then release — may trigger FloorTaken
        // NOTE: if another queued user gets the floor, onFloorSignal(FloorTaken)
        // will override Active→Receiving. That is correct behaviour.
    }

    void leaveGroup() {
        if (state_ == ClientState::Transmitting) {
            media_.stopTx();
            floor_.releaseFloor(userId_);
        }
        if (state_ == ClientState::Receiving) {
            media_.stopRx();
        }
        transition(ClientState::Idle);
        log_.record(userId_, "→ GROUP_CALL_RELEASE group=" + groupId_);
        groupId_ = "";
    }

    // ── Network/floor events (called by FloorController) ─────────────────────

    void onFloorSignal(FloorSignal signal) {
        log_.record(userId_, "← " + to_string(signal));

        switch (signal) {
        case FloorSignal::FloorGranted:
            // Accept from FloorRequesting (normal) OR Active (queued grant)
            if (state_ == ClientState::FloorRequesting ||
                state_ == ClientState::Active) {
                transition(ClientState::Transmitting);
                media_.startTx(groupId_, userId_);
                // Simulate sending 3 voice packets
                media_.sendPacket();
                media_.sendPacket();
                media_.sendPacket();
            }
            break;

        case FloorSignal::FloorDenied:
            if (state_ == ClientState::FloorRequesting) {
                log_.record(userId_, "floor busy — waiting in queue");
                transition(ClientState::Active);
            }
            break;

        case FloorSignal::FloorTaken:
            if (state_ == ClientState::Active) {
                transition(ClientState::Receiving);
                media_.startRx(floor_.currentHolder());
                // Simulate receiving 5 packets from speaker
                media_.receivePacket();
                media_.receivePacket();
                media_.receivePacket();
                media_.receivePacket();
                media_.receivePacket();
            }
            break;

        case FloorSignal::FloorIdle:
            if (state_ == ClientState::Receiving) {
                media_.stopRx();
                transition(ClientState::Active);
            }
            break;

        case FloorSignal::FloorRevoked:
            if (state_ == ClientState::Transmitting) {
                media_.stopTx();
                transition(ClientState::Active);
            }
            break;
        }
    }

    // ── Accessors ─────────────────────────────────────────────────────────────

    ClientState        state()  const { return state_; }
    const std::string& userId() const { return userId_; }
    int                stateChangeCount() const { return state_changes_; }

private:
    void onCallEstablished() {
        transition(ClientState::Active);
        log_.record(userId_, "← 200 OK — call active group=" + groupId_);
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
    std::string     groupId_;
    ClientState     state_{ClientState::Idle};
    int             state_changes_{0};
    FloorController& floor_;
    MediaPlane&      media_;
    EventLog&        log_;
};

// =============================================================================
// FloorController method bodies (need McpttClient complete)
// =============================================================================

FloorSignal FloorController::requestFloor(const std::string& requesterId) {
    log_.record("FloorCtrl", "FLOOR_REQUEST from " + requesterId);

    if (current_holder_.empty()) {
        current_holder_ = requesterId;
        log_.record("FloorCtrl", "FLOOR_GRANTED to " + requesterId);

        // Notify all other clients that floor is taken
        notifyFloorTaken(requesterId);
        return FloorSignal::FloorGranted;
    } else {
        wait_queue_.push(requesterId);
        log_.record("FloorCtrl", "FLOOR_DENIED — floor held by " + current_holder_
                                     + " (queue=" + std::to_string(wait_queue_.size()) + ")");
        return FloorSignal::FloorDenied;
    }
}

void FloorController::releaseFloor(const std::string& userId) {
    if (current_holder_ != userId) return;

    log_.record("FloorCtrl", "FLOOR_RELEASE from " + userId);
    current_holder_ = "";

    if (!wait_queue_.empty()) {
        // Grant to next in queue
        std::string next = wait_queue_.front(); wait_queue_.pop();
        current_holder_ = next;
        log_.record("FloorCtrl", "FLOOR_GRANTED (queued) to " + next);
        if (clients_.count(next))
            clients_[next]->onFloorSignal(FloorSignal::FloorGranted);
        notifyFloorTaken(next);
    } else {
        notifyFloorIdle();
    }
}

void FloorController::notifyFloorTaken(const std::string& speakerId) {
    for (auto& [id, client] : clients_) {
        if (id != speakerId)
            client->onFloorSignal(FloorSignal::FloorTaken);
    }
}

void FloorController::notifyFloorIdle() {
    log_.record("FloorCtrl", "FLOOR_IDLE — broadcast to all");
    for (auto& [id, client] : clients_)
        client->onFloorSignal(FloorSignal::FloorIdle);
}

// =============================================================================
// main
// =============================================================================

int main()
{
    std::cout << "=== MCX PTT State Machine Demo ===\n\n";

    EventLog        log;
    FloorController floor(log);
    MediaPlane      media(log);

    // Two clients in the same group
    McpttClient alice("Alice", floor, media, log);
    McpttClient bob  ("Bob",   floor, media, log);

    // ── Scenario 1: Normal PTT session ───────────────────────────────────────
    std::cout << "\n--- Scenario 1: Alice joins and presses PTT ---\n";
    alice.joinGroup("ALPHA-1");
    bob.joinGroup("ALPHA-1");

    assert(alice.state() == ClientState::Active);
    assert(bob.state()   == ClientState::Active);

    alice.pressPtt();  // Alice requests floor → granted
    assert(alice.state() == ClientState::Transmitting);
    assert(bob.state()   == ClientState::Receiving);

    alice.releasePtt();  // Alice releases → floor idle
    assert(alice.state() == ClientState::Active);
    assert(bob.state()   == ClientState::Active);
    std::cout << "✓ Scenario 1 passed\n";

    // ── Scenario 2: Floor contention — Bob queued ────────────────────────────
    std::cout << "\n--- Scenario 2: Alice transmits, Bob tries PTT (gets queued) ---\n";
    alice.pressPtt();
    assert(alice.state() == ClientState::Transmitting);

    bob.pressPtt();    // Bob requests while Alice has floor → DENIED, queued
    assert(bob.state() == ClientState::Active);   // back to active, waiting in queue

    alice.releasePtt();  // Alice releases → Bob's queued request is auto-granted
    assert(alice.state() == ClientState::Receiving);  // Alice now receives Bob
    assert(bob.state()   == ClientState::Transmitting);

    bob.releasePtt();
    assert(bob.state()   == ClientState::Active);
    assert(alice.state() == ClientState::Active);
    std::cout << "✓ Scenario 2 passed\n";

    // ── Scenario 3: Leave group ───────────────────────────────────────────────
    std::cout << "\n--- Scenario 3: Both leave ---\n";
    alice.leaveGroup();
    bob.leaveGroup();
    assert(alice.state() == ClientState::Idle);
    assert(bob.state()   == ClientState::Idle);
    std::cout << "✓ Scenario 3 passed\n";

    // ── Scenario 4: Error — PTT without joining ───────────────────────────────
    std::cout << "\n--- Scenario 4: Error handling ---\n";
    bool caught = false;
    try { alice.pressPtt(); }
    catch (const std::runtime_error& e) {
        caught = true;
        std::cout << "  ✓ caught: " << e.what() << '\n';
    }
    assert(caught);

    // ── Summary ───────────────────────────────────────────────────────────────
    std::cout << "\n=== Summary ===\n";
    std::cout << "Alice state changes: " << alice.stateChangeCount() << '\n';
    std::cout << "Bob   state changes: " << bob.stateChangeCount()   << '\n';
    std::cout << "TX packets sent    : " << media.txPackets()         << '\n';
    std::cout << "RX packets received: " << media.rxPackets()         << '\n';
    std::cout << "Floor requests     : " << log.count("FLOOR_REQUEST") << '\n';
    std::cout << "FLOOR_GRANTED      : " << log.count("FLOOR_GRANTED") << '\n';

    assert(media.txPackets()          == 3);   // 3 packets in 1 completed TX session
    assert(media.rxPackets()          == 5);   // 5 packets in 1 completed RX session
    assert(log.count("FLOOR_REQUEST") == 6);   // 4 user requests + 2 from queued grants
    assert(log.count("FLOOR_GRANTED") == 6);

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
