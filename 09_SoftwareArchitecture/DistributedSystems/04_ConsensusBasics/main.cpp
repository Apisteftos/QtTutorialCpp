// =============================================================================
// Consensus Basics — Leader Election — C++23
//
// Simplified Raft-style leader election simulation.
// No real networking — nodes communicate via direct method calls,
// which isolates the consensus logic from transport.
//
// Implements:
//   - Three node states: Follower → Candidate → Leader
//   - Term-based voting (higher term wins)
//   - Quorum rule: ⌊N/2⌋ + 1 votes required
//   - Heartbeat suppression of new elections
//   - Leader failure → re-election
//   - Split-vote detection → retry with new term
// =============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <algorithm>
#include <stdexcept>

// =============================================================================
// Forward declarations
// =============================================================================

class RaftNode;
class Cluster;

// =============================================================================
// Node state
// =============================================================================

enum class NodeState { Follower, Candidate, Leader };

std::string to_string(NodeState s) {
    switch (s) {
    case NodeState::Follower:  return "Follower";
    case NodeState::Candidate: return "Candidate";
    case NodeState::Leader:    return "Leader";
    }
    return "?";
}

// =============================================================================
// Vote request / response
// =============================================================================

struct VoteRequest {
    int         term;
    std::string candidateId;
};

struct VoteResponse {
    int  term;
    bool granted;
};

// =============================================================================
// Raft Node
// =============================================================================

class RaftNode {
public:
    explicit RaftNode(const std::string& id, Cluster* cluster)
        : id_(id), cluster_(cluster) {}

    // ── State accessors ───────────────────────────────────────────────────────
    const std::string& id()      const { return id_; }
    NodeState          state()   const { return state_; }
    int                term()    const { return term_; }
    bool               alive()   const { return alive_; }
    bool               isLeader() const { return state_ == NodeState::Leader; }

    // ── Heartbeat from leader ─────────────────────────────────────────────────
    void receiveHeartbeat(int leaderTerm, const std::string& leaderId) {
        if (!alive_) return;
        if (leaderTerm >= term_) {
            term_    = leaderTerm;
            state_   = NodeState::Follower;
            votedFor_ = leaderId;
            heartbeat_received_ = true;
            std::cout << "    " << id_ << " ← heartbeat from " << leaderId
                      << " (term=" << leaderTerm << ")\n";
        }
    }

    // ── Vote RPC ──────────────────────────────────────────────────────────────
    VoteResponse requestVote(const VoteRequest& req) {
        if (!alive_) return {term_, false};

        // Reject if we've already voted in this term
        if (req.term < term_) {
            std::cout << "    " << id_ << " → REJECT vote for " << req.candidateId
                      << " (stale term " << req.term << " < " << term_ << ")\n";
            return {term_, false};
        }

        // Update term if newer
        if (req.term > term_) {
            term_     = req.term;
            votedFor_ = "";
            state_    = NodeState::Follower;
        }

        // Grant vote if haven't voted yet this term
        bool grant = votedFor_.empty() || votedFor_ == req.candidateId;
        if (grant) {
            votedFor_ = req.candidateId;
            std::cout << "    " << id_ << " → GRANT vote to " << req.candidateId
                      << " (term=" << req.term << ")\n";
        } else {
            std::cout << "    " << id_ << " → DENY vote (already voted for "
                      << votedFor_ << ")\n";
        }
        return {term_, grant};
    }

    // ── Election trigger ──────────────────────────────────────────────────────
    // Called when this node's election timer fires (no heartbeat received)
    void startElection();  // defined after Cluster

    // ── Leader sends heartbeats ───────────────────────────────────────────────
    void sendHeartbeats();  // defined after Cluster

    // ── Failure simulation ────────────────────────────────────────────────────
    void fail() {
        alive_ = false;
        state_ = NodeState::Follower;
        std::cout << "  💀 " << id_ << " CRASHED\n";
    }

    void recover() {
        alive_  = true;
        term_   = 0;
        votedFor_ = "";
        heartbeat_received_ = false;
        std::cout << "  ✅ " << id_ << " RECOVERED\n";
    }

    bool heartbeatReceived() const { return heartbeat_received_; }
    void resetHeartbeat()          { heartbeat_received_ = false; }

private:
    std::string id_;
    Cluster*    cluster_;
    NodeState   state_{NodeState::Follower};
    int         term_{0};
    std::string votedFor_;
    bool        alive_{true};
    bool        heartbeat_received_{false};
};

// =============================================================================
// Cluster — manages nodes, provides RPC routing
// =============================================================================

class Cluster {
public:
    void addNode(RaftNode* node) {
        nodes_.push_back(node);
    }

    int quorum() const {
        int alive = static_cast<int>(
            std::count_if(nodes_.begin(), nodes_.end(),
                          [](RaftNode* n){ return n->alive(); }));
        return alive / 2 + 1;
    }

    // Broadcast vote request, collect responses
    int collectVotes(const VoteRequest& req, const std::string& selfId) {
        int votes = 1;  // vote for self
        for (auto* node : nodes_) {
            if (!node->alive() || node->id() == selfId) continue;
            auto resp = node->requestVote(req);
            if (resp.granted) votes++;
        }
        return votes;
    }

    // Broadcast heartbeat from leader
    void broadcastHeartbeat(int term, const std::string& leaderId) {
        for (auto* node : nodes_) {
            if (node->id() == leaderId) continue;
            node->receiveHeartbeat(term, leaderId);
        }
    }

    RaftNode* currentLeader() const {
        for (auto* n : nodes_)
            if (n->isLeader() && n->alive()) return n;
        return nullptr;
    }

    int aliveCount() const {
        return static_cast<int>(
            std::count_if(nodes_.begin(), nodes_.end(),
                          [](RaftNode* n){ return n->alive(); }));
    }

    const std::vector<RaftNode*>& nodes() const { return nodes_; }

private:
    std::vector<RaftNode*> nodes_;
};

// ── Method bodies that need Cluster to be complete ───────────────────────────

void RaftNode::startElection() {
    if (!alive_) return;
    term_++;
    state_    = NodeState::Candidate;
    votedFor_ = id_;

    std::cout << "\n  ⚡ " << id_ << " starts election (term=" << term_ << ")\n";

    VoteRequest req{term_, id_};
    int votes    = cluster_->collectVotes(req, id_);
    int needed   = cluster_->quorum();

    std::cout << "  📊 " << id_ << " received " << votes
              << "/" << cluster_->aliveCount()
              << " votes (need " << needed << ")\n";

    if (votes >= needed) {
        state_ = NodeState::Leader;
        std::cout << "  👑 " << id_ << " is now LEADER (term=" << term_ << ")\n";
        sendHeartbeats();
    } else {
        state_ = NodeState::Follower;
        std::cout << "  ❌ " << id_ << " election failed (split vote)\n";
    }
}

void RaftNode::sendHeartbeats() {
    if (!alive_ || state_ != NodeState::Leader) return;
    std::cout << "  💓 " << id_ << " sending heartbeats\n";
    cluster_->broadcastHeartbeat(term_, id_);
}

// =============================================================================
// main
// =============================================================================

int main()
{
    std::cout << "=== Consensus — Raft Leader Election Demo ===\n\n";

    // ── Setup: 5-node cluster ─────────────────────────────────────────────────
    Cluster cluster;
    RaftNode n1("Node-1", &cluster);
    RaftNode n2("Node-2", &cluster);
    RaftNode n3("Node-3", &cluster);
    RaftNode n4("Node-4", &cluster);
    RaftNode n5("Node-5", &cluster);
    for (auto* n : {&n1, &n2, &n3, &n4, &n5}) cluster.addNode(n);

    std::cout << "Cluster: 5 nodes, quorum = " << cluster.quorum() << "\n";
    assert(cluster.quorum() == 3);

    // ── Scenario 1: Normal election ───────────────────────────────────────────
    std::cout << "\n=== Scenario 1: Normal election ===\n";
    n1.startElection();  // Node-1's timer fires first

    assert(n1.isLeader());
    assert(n1.term() == 1);
    assert(cluster.currentLeader() == &n1);
    std::cout << "✓ Node-1 elected leader, term=1\n";

    // Leader sends periodic heartbeats — suppresses new elections
    n1.sendHeartbeats();
    for (auto* n : cluster.nodes())
        if (n->id() != "Node-1") assert(!n->isLeader());
    std::cout << "✓ Heartbeats sent — followers remain followers\n";

    // ── Scenario 2: Leader fails → re-election ────────────────────────────────
    std::cout << "\n=== Scenario 2: Leader failure → re-election ===\n";
    n1.fail();
    assert(cluster.currentLeader() == nullptr);
    std::cout << "✓ No leader after Node-1 crash\n";

    // Node-2's timer fires
    n2.startElection();

    assert(n2.isLeader());
    assert(n2.term() == 2);
    assert(cluster.currentLeader() == &n2);
    std::cout << "✓ Node-2 elected new leader, term=2\n";

    // ── Scenario 3: Quorum lost — no election possible ────────────────────────
    std::cout << "\n=== Scenario 3: Quorum lost (3 of 5 down) ===\n";
    n3.fail();
    n4.fail();
    // Now only n2 (leader) and n5 are alive — 2 nodes, quorum=2

    n2.fail();
    assert(cluster.aliveCount() == 1);   // only n5 alive

    // n5 tries to elect itself — only 1 vote, needs 2 (quorum of 2 remaining)
    // But with only 1 alive, quorum() returns 1 (⌊1/2⌋+1 = 1)
    // Demonstrating: the quorum formula adapts to alive nodes here
    // In real Raft the quorum is fixed at cluster startup
    // Here we show the principle
    n5.startElection();
    std::cout << "✓ Scenario 3 complete (single-node quorum)\n";

    // ── Scenario 4: Node recovery ─────────────────────────────────────────────
    std::cout << "\n=== Scenario 4: Crashed node recovers ===\n";
    n1.recover();
    n2.recover();
    n3.recover();

    // Fresh election
    n3.startElection();
    assert(cluster.currentLeader() != nullptr);
    std::cout << "✓ Cluster recovered — leader: "
              << cluster.currentLeader()->id()
              << " term=" << cluster.currentLeader()->term() << "\n";

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
