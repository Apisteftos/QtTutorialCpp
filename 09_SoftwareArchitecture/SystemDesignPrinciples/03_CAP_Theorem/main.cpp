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

#include <string>
#include <unordered_map>
#include <vector>
#include <optional>
#include <cassert>
#include <stdexcept>
#include <print>

// =============================================================================
// Network Partition simulation
// =============================================================================

class Network {
public:
    bool partitioned() const { return partitioned_; }
    void partition()         { partitioned_ = true;  std::println("  ⚡ NETWORK PARTITION"); }
    void heal()              { partitioned_ = false; std::println("  ✅ PARTITION HEALED"); }

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
        std::println("  [CP:{}] write({}={} v{})", id_, key, value, version_[key]);
    }

    // During partition: returns error rather than potentially stale data
    std::optional<std::string> read(const std::string& key) const {
        if (net_.partitioned() && !is_primary_) {
            std::println("  [CP:{}] read({}) → ERROR: partition — refusing stale read", id_, key);
            return std::nullopt;  // CP choice: error over stale data
        }
        auto it = store_.find(key);
        if (it == store_.end()) return std::nullopt;
        std::println("  [CP:{}] read({}) → {} (v{})", id_, key, it->second, version_.at(key));
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
        std::println("  [CP:{}] synced from {}", id_, primary.id_);
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
        std::println("  [AP:{}] write({}={} v{})", id_, key, value, version_[key]);
    }

    // During partition: returns last known value (may be stale) — never errors
    std::optional<std::string> read(const std::string& key) const {
        auto it = store_.find(key);
        if (it == store_.end()) return std::nullopt;

        bool stale = net_.partitioned() && !is_primary_;
        std::println("  [AP:{}] read({}) → {} (v{}){}",
            id_, key, it->second, version_.at(key), (stale ? " ⚠️  possibly stale" : ""));
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
        std::println("  [AP:{}] merged from {} (eventual consistency)", id_, other.id_);
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
    std::println("=== CAP Theorem Demo ===");
    std::println("");

    Network net;

    // ── CP System Demo ────────────────────────────────────────────────────────
    std::println("=== CP System: Bank Account ===");
    {
        CPNode primary("Primary", net);
        CPNode replica("Replica", net);
        primary.setPrimary(true);

        // Normal operation
        std::println("");
        std::println("-- Normal operation --");
        primary.write("balance", "1000");
        replica.syncFrom(primary);

        auto r1 = replica.read("balance");
        assert(r1.has_value() && *r1 == "1000");

        // Partition — primary gets new write, replica refuses reads
        std::println("");
        std::println("-- During partition --");
        net.partition();

        primary.write("balance", "800");  // write on primary during partition

        // Replica REFUSES to read — CP choice: error over stale data
        auto r2 = replica.read("balance");
        assert(!r2.has_value());  // returns nullopt — consistent behaviour
        std::println("  CP: replica refused stale read ✓");

        // Heal + sync
        std::println("");
        std::println("-- After partition heals --");
        net.heal();
        replica.syncFrom(primary);

        auto r3 = replica.read("balance");
        assert(r3.has_value() && *r3 == "800");
        std::println("  CP: both nodes agree on balance=800 ✓");
    }

    // ── AP System Demo ────────────────────────────────────────────────────────
    std::println("");
    std::println("=== AP System: Social Media Like Count ===");
    {
        APNode node1("Node1", net);
        APNode node2("Node2", net);
        node1.setPrimary(true);

        // Normal operation
        std::println("");
        std::println("-- Normal operation --");
        node1.write("post_42_likes", "1500");
        node2.mergeFrom(node1);

        auto r1 = node2.read("post_42_likes");
        assert(r1.has_value() && *r1 == "1500");

        // Partition — node1 gets updates, node2 serves stale but stays available
        std::println("");
        std::println("-- During partition --");
        net.partition();

        node1.write("post_42_likes", "1523");  // new likes come in

        // Node2 STILL RESPONDS with stale data — AP choice: availability over consistency
        auto r2 = node2.read("post_42_likes");
        assert(r2.has_value() && *r2 == "1500");  // stale, but not an error
        std::println("  AP: node2 served stale value (1500 instead of 1523) but stayed available ✓");

        // Heal + merge (eventual consistency)
        std::println("");
        std::println("-- After partition heals (eventual consistency) --");
        net.heal();
        node2.mergeFrom(node1);

        auto r3 = node2.read("post_42_likes");
        assert(r3.has_value() && *r3 == "1523");  // now converged
        std::println("  AP: both nodes converged to 1523 ✓");
    }
    return 0;
}
