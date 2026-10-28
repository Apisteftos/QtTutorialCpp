// caching_proxy_example.cpp
// Caching Proxy Pattern - Database Query Optimization

#include <string>
#include <memory>
#include <unordered_map>
#include <chrono>
#include <thread>
#include <ctime>
#include <iomanip>
#include <vector>
#include <print>

// ===== SUBJECT INTERFACE =====
class IDatabase {
public:
    virtual ~IDatabase() = default;
    virtual std::string query(const std::string& sql) = 0;
    virtual void clearCache() = 0;
};

// ===== REAL SUBJECT - Actual Database =====
class RealDatabase : public IDatabase {
private:
    std::string dbName;

    // Simulate expensive database operation
    std::string executeQuery(const std::string& sql) {
        std::println("  💾 Executing query on database...");

        // Simulate network latency and processing time
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        // Generate mock result based on query
        if (sql.find("SELECT * FROM users") != std::string::npos) {
            return "User Data: [id:1, name:John], [id:2, name:Jane], [id:3, name:Bob]";
        } else if (sql.find("SELECT * FROM products") != std::string::npos) {
            return "Product Data: [id:101, name:Laptop, price:999], [id:102, name:Mouse, price:29]";
        } else if (sql.find("COUNT") != std::string::npos) {
            return "Count: 42";
        }
        return "Query Result: [sample data]";
    }

public:
    explicit RealDatabase(const std::string& name) : dbName(name) {
        std::println("🔌 Connected to database: {}", dbName);
    }

    std::string query(const std::string& sql) override {
        std::println("  🔍 Database processing: {}", sql);
        return executeQuery(sql);
    }

    void clearCache() override {
        // Real database doesn't have cache
    }
};

// ===== CACHE ENTRY =====
struct CacheEntry {
    std::string result;
    std::chrono::steady_clock::time_point timestamp;
    int hitCount = 0;
};

// ===== CACHING PROXY - Adds Caching Layer =====
class CachingDatabaseProxy : public IDatabase {
private:
    std::unique_ptr<RealDatabase> database;
    std::unordered_map<std::string, CacheEntry> cache;
    int cacheHits = 0;
    int cacheMisses = 0;
    std::chrono::seconds cacheTimeout;
    size_t maxCacheSize;

    bool isCacheValid(const CacheEntry& entry) const {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
            now - entry.timestamp);
        return elapsed < cacheTimeout;
    }

    void evictOldestEntry() {
        if (cache.empty()) return;

        // Find oldest entry (simple LRU)
        auto oldest = cache.begin();
        for (auto it = cache.begin(); it != cache.end(); ++it) {
            if (it->second.timestamp < oldest->second.timestamp) {
                oldest = it;
            }
        }

        std::println("  🗑️  Evicting old cache entry for: {}", oldest->first);
        cache.erase(oldest);
    }

public:
    CachingDatabaseProxy(const std::string& dbName,
                         int timeoutSeconds = 5,
                         size_t maxSize = 10)
        : database(std::make_unique<RealDatabase>(dbName)),
        cacheTimeout(timeoutSeconds),
        maxCacheSize(maxSize) {
        std::println("📦 Caching proxy initialized (timeout: {}s, max size: {})", timeoutSeconds, maxSize);
    }

    std::string query(const std::string& sql) override {
        std::println("");
        std::println("🔄 Proxy received query: {}", sql);

        // Check cache first
        auto it = cache.find(sql);
        if (it != cache.end() && isCacheValid(it->second)) {
            // Cache hit!
            cacheHits++;
            it->second.hitCount++;

            std::println("  ✅ CACHE HIT! (hit #{} for this query)", it->second.hitCount);
            std::println("  ⚡ Returning cached result immediately");

            return it->second.result;
        }

        // Cache miss - need to query database
        cacheMisses++;
        std::println("  ❌ CACHE MISS - querying database...");

        auto start = std::chrono::high_resolution_clock::now();
        std::string result = database->query(sql);
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::println("  ⏱️  Query took: {}ms", duration.count());

        // Store in cache
        if (cache.size() >= maxCacheSize) {
            evictOldestEntry();
        }

        CacheEntry entry;
        entry.result = result;
        entry.timestamp = std::chrono::steady_clock::now();
        entry.hitCount = 0;
        cache[sql] = entry;

        std::println("  💾 Result cached for future use");

        return result;
    }

    void clearCache() override {
        cache.clear();
        cacheHits = 0;
        cacheMisses = 0;
        std::println("🧹 Cache cleared!");
    }

    void showStatistics() const {
        std::println("");
        std::println("📊 === Cache Statistics ===");
        std::println("  Cache Hits: {}", cacheHits);
        std::println("  Cache Misses: {}", cacheMisses);

        if (cacheHits + cacheMisses > 0) {
            double hitRate = (double)cacheHits / (cacheHits + cacheMisses) * 100;
            std::println("  Hit Rate: {:.1f}%", hitRate);
        }

        std::println("  Current Cache Size: {}/{}", cache.size(), maxCacheSize);

        if (!cache.empty()) {
            std::println("  Cached Queries:");
            for (const auto& [query, entry] : cache) {
                std::println("    - {}... (hits: {})", query.substr(0, 30), entry.hitCount);
            }
        }
    }
};

// ===== APPLICATION - Client Code =====
class Application {
private:
    std::unique_ptr<IDatabase> database;

public:
    explicit Application(std::unique_ptr<IDatabase> db)
        : database(std::move(db)) {}

    void runQueries() {
        // Execute various queries
        std::vector<std::string> queries = {
            "SELECT * FROM users WHERE active = 1",
            "SELECT * FROM products WHERE price < 100",
            "SELECT COUNT(*) FROM orders",
            "SELECT * FROM users WHERE active = 1",  // Duplicate
            "SELECT * FROM users WHERE active = 1",  // Duplicate
            "SELECT * FROM products WHERE price < 100",  // Duplicate
        };

        for (const auto& sql : queries) {
            auto start = std::chrono::high_resolution_clock::now();

            std::string result = database->query(sql);

            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

            std::println("  📋 Result: {}...", result.substr(0, 50));
            std::println("  ⏰ Total time: {}ms", duration.count());
        }
    }

    void demonstrateCacheExpiry() {
        std::println("");
        std::println("===== Cache Expiry Test =====");

        std::string sql = "SELECT * FROM customers";

        std::println("First query:");
        database->query(sql);

        std::println("");
        std::println("Immediate second query (should hit cache):");
        database->query(sql);

        std::println("");
        std::println("Waiting for cache to expire (6 seconds)...");
        std::this_thread::sleep_for(std::chrono::seconds(6));

        std::println("Query after expiry (should miss cache):");
        database->query(sql);
    }
};

// ===== DEMONSTRATION =====
int main() {
    std::println("===== Caching Proxy Pattern Demo =====");
    std::println("Database Query Optimization with Caching");
    std::println("");

    // Create database with caching proxy (5 second timeout, max 10 entries)
    auto proxy = std::make_unique<CachingDatabaseProxy>("MainDB", 5, 10);
    CachingDatabaseProxy* proxyPtr = proxy.get();  // Keep pointer for statistics

    // Create application
    Application app(std::move(proxy));

    // Run queries
    std::println("");
    std::println("===== Running Database Queries =====");
    app.runQueries();

    // Show statistics
    proxyPtr->showStatistics();

    // Demonstrate cache expiry
    app.demonstrateCacheExpiry();

    // Final statistics
    proxyPtr->showStatistics();

    return 0;
}
