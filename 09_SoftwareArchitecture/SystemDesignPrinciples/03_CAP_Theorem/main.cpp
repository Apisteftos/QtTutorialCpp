// =============================================================================
// CAP Theorem — C++23
//
// Simulates two distributed node models under a network partition:
//
//   CP Node — prioritises Consistency over Availability
//             During partition: refuses reads that may be stale → returns error
//
//   AP Node — prioritises Availability over Consistency
//             During partition: serves last known value → may be stale
//
// After partition heals: both models sync and converge (eventual consistency).
// =============================================================================

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <optional>
#include <cassert>
#include <stdexcept>

// =============================================================================
// Network Partition simulation
// =============================================================================

class Network {
public:
    bool partitioned() const { return partitioned_; }
    void partition()         { partitioned_ = true;  std::cout << "  ⚡ NETWORK PARTITION\n"; }
    void heal()              { partitioned_ = false; std::cout << "  ✅ PARTITION HEALED\n"; }

private:
    bool partitioned_{false};
};

// =============================================================================
// CP Node — Consistent + Partition Tolerant
// Refuses to serve potentially stale data during a partition.
// =============================================================================

class CPNode {
public:
    CPNode(const std::string& id, Network& net) : id_(id), net_(net) {}

    void write(const std::string& key, const std::string& value) {
        store_[key]   = value;
        version_[key] = ++global_version_;
        std::cout << "  [CP:" << id_ << "] write(" << key << "=" << value
                  << " v" << version_[key] << ")\n";
    }

    // During partition: returns error rather than potentially stale data
    std::optional<std::string> read(const std::string& key) const {
        if (net_.partitioned() && !is_primary_) {
            std::cout << "  [CP:" << id_ << "] read(" << key
                      << ") → ERROR: partition — refusing stale read\n";
            return std::nullopt;  // CP choice: error over stale data
        }
        auto it = store_.find(key);
        if (it == store_.end()) return std::nullopt;
        std::cout << "  [CP:" << id_ << "] read(" << key
                  << ") → " << it->second << " (v" << version_.at(key) << ")\n";
        return it->second;
    }

    // Sync from primary after partition heals
    void syncFrom(const CPNode& primary) {
        for (auto& [k, v] : primary.store_) {
            if (primary.version_.at(k) > version_[k]) {
                store_[k]   = v;
                version_[k] = primary.version_.at(k);
            }
        }
        std::cout << "  [CP:" << id_ << "] synced from " << primary.id_ << '\n';
    }

    void setPrimary(bool p) { is_primary_ = p; }
    const std::string& id() const { return id_; }

private:
    std::string id_;
    Network&    net_;
    bool        is_primary_{false};
    std::unordered_map<std::string, std::string> store_;
    std::unordered_map<std::string, int>         version_;
    inline static int global_version_{0};
};

// =============================================================================
// AP Node — Available + Partition Tolerant
// Always responds, but may serve stale data during partition.
// =============================================================================

class APNode {
public:
    APNode(const std::string& id, Network& net) : id_(id), net_(net) {}

    void write(const std::string& key, const std::string& value) {
        store_[key]   = value;
        version_[key] = ++global_version_;
        std::cout << "  [AP:" << id_ << "] write(" << key << "=" << value
                  << " v" << version_[key] << ")\n";
    }

    // During partition: returns last known value (may be stale) — never errors
    std::optional<std::string> read(const std::string& key) const {
        auto it = store_.find(key);
        if (it == store_.end()) return std::nullopt;

        bool stale = net_.partitioned() && !is_primary_;
        std::cout << "  [AP:" << id_ << "] read(" << key << ") → " << it->second
                  << " (v" << version_.at(key) << ")"
                  << (stale ? " ⚠️  possibly stale" : "") << "\n";
        return it->second;
    }

    // Merge writes after partition heals (last-write-wins by version)
    void mergeFrom(const APNode& other) {
        for (auto& [k, v] : other.store_) {
            auto it = version_.find(k);
            if (it == version_.end() || other.version_.at(k) > it->second) {
                store_[k]   = v;
                version_[k] = other.version_.at(k);
            }
        }
        std::cout << "  [AP:" << id_ << "] merged from " << other.id_ << " (eventual consistency)\n";
    }

    void setPrimary(bool p) { is_primary_ = p; }
    const std::string& id() const { return id_; }
    std::optional<std::string> localRead(const std::string& k) const {
        auto it = store_.find(k);
        return it != store_.end() ? std::optional<std::string>{it->second} : std::nullopt;
    }

private:
    std::string id_;
    Network&    net_;
    bool        is_primary_{false};
    std::unordered_map<std::string, std::string> store_;
    std::unordered_map<std::string, int>         version_;
    inline static int global_version_{0};
};

// =============================================================================
// main
// =============================================================================

int main()
{
    std::cout << "=== CAP Theorem Demo ===\n\n";

    Network net;

    // ── CP System Demo ────────────────────────────────────────────────────────
    std::cout << "=== CP System: Bank Account ===\n";
    {
        CPNode primary("Primary", net);
        CPNode replica("Replica", net);
        primary.setPrimary(true);

        // Normal operation
        std::cout << "\n-- Normal operation --\n";
        primary.write("balance", "1000");
        replica.syncFrom(primary);

        auto r1 = replica.read("balance");
        assert(r1.has_value() && *r1 == "1000");

        // Partition — primary gets new write, replica refuses reads
        std::cout << "\n-- During partition --\n";
        net.partition();

        primary.write("balance", "800");  // write on primary during partition

        // Replica REFUSES to read — CP choice: error over stale data
        auto r2 = replica.read("balance");
        assert(!r2.has_value());  // returns nullopt — consistent behaviour
        std::cout << "  CP: replica refused stale read ✓\n";

        // Heal + sync
        std::cout << "\n-- After partition heals --\n";
        net.heal();
        replica.syncFrom(primary);

        auto r3 = replica.read("balance");
        assert(r3.has_value() && *r3 == "800");
        std::cout << "  CP: both nodes agree on balance=800 ✓\n";
    }

    // ── AP System Demo ────────────────────────────────────────────────────────
    std::cout << "\n=== AP System: Social Media Like Count ===\n";
    {
        APNode node1("Node1", net);
        APNode node2("Node2", net);
        node1.setPrimary(true);

        // Normal operation
        std::cout << "\n-- Normal operation --\n";
        node1.write("post_42_likes", "1500");
        node2.mergeFrom(node1);

        auto r1 = node2.read("post_42_likes");
        assert(r1.has_value() && *r1 == "1500");

        // Partition — node1 gets updates, node2 serves stale but stays available
        std::cout << "\n-- During partition --\n";
        net.partition();

        node1.write("post_42_likes", "1523");  // new likes come in

        // Node2 STILL RESPONDS with stale data — AP choice: availability over consistency
        auto r2 = node2.read("post_42_likes");
        assert(r2.has_value() && *r2 == "1500");  // stale, but not an error
        std::cout << "  AP: node2 served stale value (1500 instead of 1523) but stayed available ✓\n";

        // Heal + merge (eventual consistency)
        std::cout << "\n-- After partition heals (eventual consistency) --\n";
        net.heal();
        node2.mergeFrom(node1);

        auto r3 = node2.read("post_42_likes");
        assert(r3.has_value() && *r3 == "1523");  // now converged
        std::cout << "  AP: both nodes converged to 1523 ✓\n";
    }

    // ── Summary ───────────────────────────────────────────────────────────────
    std::cout << "\n=== Summary ===\n";
    std::cout << "  CP: during partition → replica returned ERROR (no stale reads)\n";
    std::cout << "  AP: during partition → node served STALE DATA (always available)\n";
    std::cout << "  Both: after heal → data converged (eventual consistency)\n";
    std::cout << "\nAll assertions passed.\n";
    return 0;
}
