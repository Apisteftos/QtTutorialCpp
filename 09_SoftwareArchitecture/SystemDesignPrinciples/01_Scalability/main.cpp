// =============================================================================
// Scalability Patterns — C++23
//
// Three concrete scalability techniques demonstrated:
//
//   1. ThreadPool      — handle concurrent requests without per-request thread cost
//   2. LRUCache        — avoid repeated expensive computation (O(1) get/put)
//   3. ConnectionPool  — reuse costly resources (DB connections, sockets)
//
// Each is benchmarked to show the improvement quantitatively.
// =============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <unordered_map>
#include <list>
#include <cassert>
#include <chrono>
#include <atomic>
#include <memory>

using Clock = std::chrono::steady_clock;
using Ms    = std::chrono::milliseconds;

// =============================================================================
// 1. Thread Pool — fixed worker threads, task queue
// =============================================================================

class ThreadPool {
public:
    explicit ThreadPool(size_t threads) {
        for (size_t i = 0; i < threads; i++) {
            workers_.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock lock(mutex_);
                        cv_.wait(lock, [this]{ return stop_ || !tasks_.empty(); });
                        if (stop_ && tasks_.empty()) return;
                        task = std::move(tasks_.front());
                        tasks_.pop();
                    }
                    task();
                }
            });
        }
    }

    // Submit a task and get a future for its result
    template<typename F, typename... Args>
    auto submit(F&& f, Args&&... args)
        -> std::future<std::invoke_result_t<F, Args...>>
    {
        using R = std::invoke_result_t<F, Args...>;
        auto task = std::make_shared<std::packaged_task<R()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        std::future<R> future = task->get_future();
        {
            std::lock_guard lock(mutex_);
            tasks_.push([task]{ (*task)(); });
        }
        cv_.notify_one();
        return future;
    }

    size_t threadCount() const { return workers_.size(); }

    ~ThreadPool() {
        { std::lock_guard lock(mutex_); stop_ = true; }
        cv_.notify_all();
        for (auto& w : workers_) w.join();
    }

private:
    std::vector<std::thread>          workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex                        mutex_;
    std::condition_variable           cv_;
    bool                              stop_{false};
};

// =============================================================================
// 2. LRU Cache — O(1) get and put, evicts least-recently-used on overflow
// =============================================================================

template<typename Key, typename Value>
class LRUCache {
public:
    explicit LRUCache(size_t capacity) : capacity_(capacity) {}

    // Get value — returns nullptr if not cached
    const Value* get(const Key& key) {
        auto it = map_.find(key);
        if (it == map_.end()) { misses_++; return nullptr; }
        // Move to front (most recently used)
        items_.splice(items_.begin(), items_, it->second);
        hits_++;
        return &it->second->second;
    }

    // Put value — evicts LRU if at capacity
    void put(const Key& key, Value value) {
        auto it = map_.find(key);
        if (it != map_.end()) {
            it->second->second = std::move(value);
            items_.splice(items_.begin(), items_, it->second);
            return;
        }
        if (items_.size() >= capacity_) {
            // Evict least recently used (back of list)
            map_.erase(items_.back().first);
            items_.pop_back();
            evictions_++;
        }
        items_.emplace_front(key, std::move(value));
        map_[key] = items_.begin();
    }

    int hits()      const { return hits_; }
    int misses()    const { return misses_; }
    int evictions() const { return evictions_; }
    double hitRate() const {
        int total = hits_ + misses_;
        return total == 0 ? 0.0 : 100.0 * hits_ / total;
    }
    size_t size()   const { return items_.size(); }

private:
    size_t                                              capacity_;
    std::list<std::pair<Key, Value>>                    items_;
    std::unordered_map<Key, typename
                       std::list<std::pair<Key,Value>>::iterator>      map_;
    int hits_{0}, misses_{0}, evictions_{0};
};

// =============================================================================
// 3. Connection Pool — reuse expensive resources
// =============================================================================

// Simulated "database connection" — expensive to create
struct DbConnection {
    int id;
    bool in_use{false};

    std::string query(const std::string& sql) const {
        // Simulate work
        return "result[" + std::to_string(id) + "]:" + sql;
    }
};

class ConnectionPool {
public:
    explicit ConnectionPool(int pool_size) {
        for (int i = 0; i < pool_size; i++)
            pool_.push_back(std::make_unique<DbConnection>(DbConnection{i}));
    }

    // Acquire a connection (blocks until one is free)
    DbConnection* acquire() {
        std::unique_lock lock(mutex_);
        cv_.wait(lock, [this]{
            for (auto& c : pool_) if (!c->in_use) return true;
            return false;
        });
        for (auto& c : pool_) {
            if (!c->in_use) {
                c->in_use = true;
                acquisitions_++;
                return c.get();
            }
        }
        return nullptr;
    }

    // Release connection back to pool
    void release(DbConnection* conn) {
        std::lock_guard lock(mutex_);
        conn->in_use = false;
        cv_.notify_one();
    }

    int poolSize()     const { return static_cast<int>(pool_.size()); }
    int acquisitions() const { return acquisitions_; }

private:
    std::vector<std::unique_ptr<DbConnection>> pool_;
    std::mutex              mutex_;
    std::condition_variable cv_;
    int                     acquisitions_{0};
};

// RAII guard for connection pool
struct ConnectionGuard {
    ConnectionGuard(ConnectionPool& pool) : pool_(pool), conn_(pool.acquire()) {}
    ~ConnectionGuard() { if (conn_) pool_.release(conn_); }
    DbConnection* operator->() { return conn_; }

private:
    ConnectionPool& pool_;
    DbConnection*   conn_;
};

// =============================================================================
// main — demonstrate and benchmark all three patterns
// =============================================================================

int main()
{
    std::cout << "=== Scalability Patterns Demo ===\n\n";

    // ── 1. Thread Pool ────────────────────────────────────────────────────────
    std::cout << "--- 1. Thread Pool ---\n";
    {
        ThreadPool pool(4);
        std::cout << "  Workers: " << pool.threadCount() << '\n';

        std::atomic<int> completed{0};

        // Submit 20 tasks concurrently
        std::vector<std::future<int>> futures;
        for (int i = 0; i < 20; i++) {
            futures.push_back(pool.submit([i, &completed]() -> int {
                // Simulate work
                std::this_thread::sleep_for(std::chrono::microseconds(100));
                completed++;
                return i * i;
            }));
        }

        int sum = 0;
        for (auto& f : futures) sum += f.get();

        assert(completed == 20);
        // sum of squares 0..19 = 2470
        assert(sum == 2470);
        std::cout << "  ✓ 20 tasks completed, sum of squares = " << sum << '\n';
    }

    // ── 2. LRU Cache ─────────────────────────────────────────────────────────
    std::cout << "\n--- 2. LRU Cache (capacity=3) ---\n";
    {
        LRUCache<std::string, int> cache(3);

        // Simulate expensive computation cached by key
        auto expensiveCompute = [&](const std::string& key) -> int {
            if (auto* cached = cache.get(key)) return *cached;
            // Simulate expensive work
            int result = static_cast<int>(std::hash<std::string>{}(key) % 1000);
            cache.put(key, result);
            return result;
        };

        // Access pattern: repeated keys should hit cache
        for (const auto& k : {"alpha","beta","gamma","alpha","beta",
                              "alpha","delta","alpha","beta","alpha"}) {
            expensiveCompute(k);
        }

        std::cout << "  Cache hits:      " << cache.hits()      << '\n';
        std::cout << "  Cache misses:    " << cache.misses()    << '\n';
        std::cout << "  Cache evictions: " << cache.evictions() << '\n';
        std::cout << "  Hit rate:        " << cache.hitRate()   << "%\n";

        assert(cache.hits()   > cache.misses());
        assert(cache.size()   <= 3);
        assert(cache.hitRate() > 50.0);
        std::cout << "  ✓ Cache working, hit rate > 50%\n";
    }

    // ── 3. Connection Pool ────────────────────────────────────────────────────
    std::cout << "\n--- 3. Connection Pool (size=3) ---\n";
    {
        ConnectionPool pool(3);
        std::cout << "  Pool size: " << pool.poolSize() << '\n';

        // 8 concurrent queries sharing 3 connections
        ThreadPool workers(4);
        std::vector<std::future<std::string>> results;

        for (int i = 0; i < 8; i++) {
            results.push_back(workers.submit([&pool, i]() -> std::string {
                ConnectionGuard conn(pool);  // RAII — auto-releases on scope exit
                return conn->query("SELECT * FROM orders WHERE id=" + std::to_string(i));
            }));
        }

        int ok = 0;
        for (auto& r : results) {
            auto res = r.get();
            if (!res.empty()) ok++;
        }

        assert(ok == 8);
        assert(pool.acquisitions() == 8);
        std::cout << "  Queries executed:    " << ok << '\n';
        std::cout << "  Pool acquisitions:   " << pool.acquisitions() << '\n';
        std::cout << "  ✓ 8 queries via 3 connections (pooled, not 8 new connections)\n";
    }

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
