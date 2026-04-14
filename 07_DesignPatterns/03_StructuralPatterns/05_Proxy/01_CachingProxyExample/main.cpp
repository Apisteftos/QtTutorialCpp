// caching_proxy_example.cpp
// Caching Proxy Pattern - Database Query Optimization

#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>
#include <chrono>
#include <thread>
#include <ctime>
#include <iomanip>
#include <vector>

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
        std::cout << "  💾 Executing query on database..." << std::endl;

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
        std::cout << "🔌 Connected to database: " << dbName << std::endl;
    }

    std::string query(const std::string& sql) override {
        std::cout << "  🔍 Database processing: " << sql << std::endl;
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

        std::cout << "  🗑️  Evicting old cache entry for: " << oldest->first << std::endl;
        cache.erase(oldest);
    }

public:
    CachingDatabaseProxy(const std::string& dbName,
                         int timeoutSeconds = 5,
                         size_t maxSize = 10)
        : database(std::make_unique<RealDatabase>(dbName)),
        cacheTimeout(timeoutSeconds),
        maxCacheSize(maxSize) {
        std::cout << "📦 Caching proxy initialized (timeout: " << timeoutSeconds
                  << "s, max size: " << maxSize << ")" << std::endl;
    }

    std::string query(const std::string& sql) override {
        std::cout << "\n🔄 Proxy received query: " << sql << std::endl;

        // Check cache first
        auto it = cache.find(sql);
        if (it != cache.end() && isCacheValid(it->second)) {
            // Cache hit!
            cacheHits++;
            it->second.hitCount++;

            std::cout << "  ✅ CACHE HIT! (hit #" << it->second.hitCount
                      << " for this query)" << std::endl;
            std::cout << "  ⚡ Returning cached result immediately" << std::endl;

            return it->second.result;
        }

        // Cache miss - need to query database
        cacheMisses++;
        std::cout << "  ❌ CACHE MISS - querying database..." << std::endl;

        auto start = std::chrono::high_resolution_clock::now();
        std::string result = database->query(sql);
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "  ⏱️  Query took: " << duration.count() << "ms" << std::endl;

        // Store in cache
        if (cache.size() >= maxCacheSize) {
            evictOldestEntry();
        }

        CacheEntry entry;
        entry.result = result;
        entry.timestamp = std::chrono::steady_clock::now();
        entry.hitCount = 0;
        cache[sql] = entry;

        std::cout << "  💾 Result cached for future use" << std::endl;

        return result;
    }

    void clearCache() override {
        cache.clear();
        cacheHits = 0;
        cacheMisses = 0;
        std::cout << "🧹 Cache cleared!" << std::endl;
    }

    void showStatistics() const {
        std::cout << "\n📊 === Cache Statistics ===" << std::endl;
        std::cout << "  Cache Hits: " << cacheHits << std::endl;
        std::cout << "  Cache Misses: " << cacheMisses << std::endl;

        if (cacheHits + cacheMisses > 0) {
            double hitRate = (double)cacheHits / (cacheHits + cacheMisses) * 100;
            std::cout << "  Hit Rate: " << std::fixed << std::setprecision(1)
                      << hitRate << "%" << std::endl;
        }

        std::cout << "  Current Cache Size: " << cache.size() << "/"
                  << maxCacheSize << std::endl;

        if (!cache.empty()) {
            std::cout << "  Cached Queries:" << std::endl;
            for (const auto& [query, entry] : cache) {
                std::cout << "    - " << query.substr(0, 30) << "... (hits: "
                          << entry.hitCount << ")" << std::endl;
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

            std::cout << "  📋 Result: " << result.substr(0, 50) << "..." << std::endl;
            std::cout << "  ⏰ Total time: " << duration.count() << "ms" << std::endl;
        }
    }

    void demonstrateCacheExpiry() {
        std::cout << "\n===== Cache Expiry Test =====" << std::endl;

        std::string sql = "SELECT * FROM customers";

        std::cout << "First query:" << std::endl;
        database->query(sql);

        std::cout << "\nImmediate second query (should hit cache):" << std::endl;
        database->query(sql);

        std::cout << "\nWaiting for cache to expire (6 seconds)..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(6));

        std::cout << "Query after expiry (should miss cache):" << std::endl;
        database->query(sql);
    }
};

// ===== DEMONSTRATION =====
int main() {
    std::cout << "===== Caching Proxy Pattern Demo =====" << std::endl;
    std::cout << "Database Query Optimization with Caching\n" << std::endl;

    // Create database with caching proxy (5 second timeout, max 10 entries)
    auto proxy = std::make_unique<CachingDatabaseProxy>("MainDB", 5, 10);
    CachingDatabaseProxy* proxyPtr = proxy.get();  // Keep pointer for statistics

    // Create application
    Application app(std::move(proxy));

    // Run queries
    std::cout << "\n===== Running Database Queries =====" << std::endl;
    app.runQueries();

    // Show statistics
    proxyPtr->showStatistics();

    // Demonstrate cache expiry
    app.demonstrateCacheExpiry();

    // Final statistics
    proxyPtr->showStatistics();

    std::cout << "\n===== Performance Summary =====" << std::endl;
    std::cout << "✓ First queries: ~1000ms (database hit)" << std::endl;
    std::cout << "✓ Cached queries: ~0ms (cache hit)" << std::endl;
    std::cout << "✓ Performance improvement: >99% for cached queries" << std::endl;
    std::cout << "✓ Cache automatically expires old entries" << std::endl;
    std::cout << "✓ LRU eviction when cache is full" << std::endl;

    return 0;
}
