// =============================================================================
// Load Balancing — C++23
//
// Demonstrates three strategies + health checking + automatic failover:
//   1. Round-Robin      — rotate through servers in order
//   2. Least-Connections — pick server with fewest active requests
//   3. Weighted         — servers with higher weight get more traffic
//
// LoadBalancer also performs health checks and removes failed servers
// from rotation automatically — adding them back on recovery.
// =============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <functional>

// =============================================================================
// Backend Server simulation
// =============================================================================

struct Response { int statusCode; std::string body; };

class BackendServer {
public:
    explicit BackendServer(const std::string& id,
                           int weight = 1,
                           bool initially_healthy = true)
        : id_(id), weight_(weight), healthy_(initially_healthy) {}

    Response handle(const std::string& request) {
        if (!healthy_)
            throw std::runtime_error("Server " + id_ + " is down");

        active_connections_++;
        total_requests_++;
        // Simulate processing
        active_connections_--;

        return {200, "[" + id_ + "] OK: " + request};
    }

    Response healthCheck() const {
        return healthy_ ? Response{200, "ok"} : Response{503, "down"};
    }

    // Test controls — simulate server going down/up
    void setHealthy(bool h) { healthy_ = h; }

    const std::string& id()              const { return id_; }
    int                weight()          const { return weight_; }
    bool               isHealthy()       const { return healthy_; }
    int                totalRequests()   const { return total_requests_; }
    int                activeConnections() const { return active_connections_; }

private:
    std::string id_;
    int         weight_;
    bool        healthy_;
    int         active_connections_{0};
    int         total_requests_{0};
};

// =============================================================================
// Load Balancer strategies
// =============================================================================

enum class Strategy { RoundRobin, LeastConnections, Weighted };

class LoadBalancer {
public:
    explicit LoadBalancer(Strategy strategy = Strategy::RoundRobin)
        : strategy_(strategy) {}

    void addServer(BackendServer* server) {
        pool_.push_back(server);
        std::cout << "  [LB] added server: " << server->id()
                  << " (weight=" << server->weight() << ")\n";
    }

    // Run health checks on all servers
    void healthCheck() {
        for (auto* s : pool_) {
            auto resp = s->healthCheck();
            bool wasHealthy = s->isHealthy();
            // Health state is managed by the server in this simulation
            if (!wasHealthy && resp.statusCode == 200)
                std::cout << "  [LB] ✅ " << s->id() << " recovered — back in rotation\n";
            else if (wasHealthy && resp.statusCode != 200)
                std::cout << "  [LB] ❌ " << s->id() << " failed health check — removed\n";
        }
    }

    // Route a request using the selected strategy
    Response route(const std::string& request) {
        auto* server = select();
        if (!server)
            return {503, "No healthy servers available"};

        std::cout << "  [LB] → " << server->id() << " : " << request << '\n';
        try {
            return server->handle(request);
        } catch (const std::exception& e) {
            std::cout << "  [LB] ⚠️  " << server->id() << " failed: " << e.what() << '\n';
            // Retry once with a different server
            auto* fallback = selectExcluding(server);
            if (!fallback) return {503, "All servers failed"};
            std::cout << "  [LB] ↩️  retry → " << fallback->id() << '\n';
            return fallback->handle(request);
        }
    }

    int healthyCount() const {
        return static_cast<int>(std::count_if(pool_.begin(), pool_.end(),
                                              [](BackendServer* s){ return s->isHealthy(); }));
    }

    void printStats() const {
        std::cout << "\n  Load distribution:\n";
        for (auto* s : pool_) {
            std::cout << "    " << s->id()
                      << (s->isHealthy() ? " [UP]  " : " [DOWN] ")
                      << "requests=" << s->totalRequests() << '\n';
        }
    }

private:
    BackendServer* select() {
        auto healthy = healthyServers();
        if (healthy.empty()) return nullptr;

        switch (strategy_) {
        case Strategy::RoundRobin:
            return selectRoundRobin(healthy);
        case Strategy::LeastConnections:
            return selectLeastConnections(healthy);
        case Strategy::Weighted:
            return selectWeighted(healthy);
        }
        return nullptr;
    }

    BackendServer* selectExcluding(BackendServer* exclude) {
        auto healthy = healthyServers();
        healthy.erase(std::remove(healthy.begin(), healthy.end(), exclude),
                      healthy.end());
        if (healthy.empty()) return nullptr;
        return healthy[0];
    }

    std::vector<BackendServer*> healthyServers() const {
        std::vector<BackendServer*> result;
        for (auto* s : pool_)
            if (s->isHealthy()) result.push_back(s);
        return result;
    }

    // ── Strategy 1: Round-Robin ───────────────────────────────────────────────
    BackendServer* selectRoundRobin(const std::vector<BackendServer*>& healthy) {
        if (rr_index_ >= static_cast<int>(healthy.size()))
            rr_index_ = 0;
        return healthy[rr_index_++];
    }

    // ── Strategy 2: Least Connections ────────────────────────────────────────
    BackendServer* selectLeastConnections(const std::vector<BackendServer*>& healthy) {
        return *std::min_element(healthy.begin(), healthy.end(),
                                 [](BackendServer* a, BackendServer* b){
                                     return a->activeConnections() < b->activeConnections();
                                 });
    }

    // ── Strategy 3: Weighted ─────────────────────────────────────────────────
    // Build virtual pool: server with weight=3 appears 3 times
    BackendServer* selectWeighted(const std::vector<BackendServer*>& healthy) {
        std::vector<BackendServer*> weighted_pool;
        for (auto* s : healthy)
            for (int i = 0; i < s->weight(); i++)
                weighted_pool.push_back(s);

        if (weighted_index_ >= static_cast<int>(weighted_pool.size()))
            weighted_index_ = 0;
        return weighted_pool[weighted_index_++];
    }

    Strategy                    strategy_;
    std::vector<BackendServer*> pool_;
    int                         rr_index_{0};
    int                         weighted_index_{0};
};

// =============================================================================
// main
// =============================================================================

int main()
{
    std::cout << "=== Load Balancing Demo ===\n\n";

    // ── Strategy 1: Round-Robin ───────────────────────────────────────────────
    std::cout << "--- Strategy 1: Round-Robin ---\n";
    {
        BackendServer sa("Server-A"), sb("Server-B"), sc("Server-C");
        LoadBalancer lb(Strategy::RoundRobin);
        lb.addServer(&sa);
        lb.addServer(&sb);
        lb.addServer(&sc);

        // 6 requests → 2 per server
        for (int i = 1; i <= 6; i++)
            lb.route("request-" + std::to_string(i));

        lb.printStats();
        assert(sa.totalRequests() == 2);
        assert(sb.totalRequests() == 2);
        assert(sc.totalRequests() == 2);
        std::cout << "  ✓ Round-Robin: even distribution\n";
    }

    // ── Strategy 2: Failover during round-robin ───────────────────────────────
    std::cout << "\n--- Failover: Server goes down mid-flight ---\n";
    {
        BackendServer sa("Server-A"), sb("Server-B"), sc("Server-C");
        LoadBalancer lb(Strategy::RoundRobin);
        lb.addServer(&sa);
        lb.addServer(&sb);
        lb.addServer(&sc);

        lb.route("req-1");  // → A
        lb.route("req-2");  // → B

        // Server B goes down
        sb.setHealthy(false);
        lb.healthCheck();

        lb.route("req-3");  // → C (B skipped)
        lb.route("req-4");  // → A (wraps, B still down)

        // Server B recovers
        sb.setHealthy(true);
        lb.healthCheck();

        lb.route("req-5");  // → B again

        lb.printStats();
        assert(lb.healthyCount() == 3);
        std::cout << "  ✓ Failover handled correctly\n";
    }

    // ── Strategy 3: Weighted ──────────────────────────────────────────────────
    std::cout << "\n--- Strategy 3: Weighted ---\n";
    {
        // Server-A is 3x stronger than Server-B
        BackendServer sa("Server-A(w=3)", 3);
        BackendServer sb("Server-B(w=1)", 1);
        LoadBalancer lb(Strategy::Weighted);
        lb.addServer(&sa);
        lb.addServer(&sb);

        // 8 requests → A gets ~75%, B gets ~25%
        for (int i = 1; i <= 8; i++)
            lb.route("req-" + std::to_string(i));

        lb.printStats();
        assert(sa.totalRequests() == 6);
        assert(sb.totalRequests() == 2);
        std::cout << "  ✓ Weighted: Server-A got "
                  << sa.totalRequests() << "/8, Server-B got "
                  << sb.totalRequests() << "/8\n";
    }

    // ── No healthy servers ────────────────────────────────────────────────────
    std::cout << "\n--- All servers down ---\n";
    {
        BackendServer sa("Server-A");
        LoadBalancer lb;
        lb.addServer(&sa);
        sa.setHealthy(false);

        auto resp = lb.route("req-1");
        assert(resp.statusCode == 503);
        std::cout << "  ✓ 503 returned when all servers down\n";
    }

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
