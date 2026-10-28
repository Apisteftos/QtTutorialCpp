#include <vector>
#include <string>
#include <memory>
#include <map>
#include <functional>
#include <print>

// ============================================
// YAGNI PRINCIPLE (You Aren't Gonna Need It)
// ============================================

/*
YAGNI PRINCIPLE:
    "You Aren't Gonna Need It"
    "Always implement things when you actually need them,
     never when you just foresee that you need them."
    — Ron Jeffries (Extreme Programming)

WHAT IT MEANS:
    • Don't add functionality until it's needed
    • Don't write code for hypothetical future use
    • Don't build features "just in case"
    • Focus on current requirements only
    • Implement when needed, not when anticipated

WHY IT MATTERS:
    ✅ Saves development time
    ✅ Reduces code complexity
    ✅ Less code to maintain and test
    ✅ Prevents wasted effort on unused features
    ✅ Keeps codebase lean and focused

KEY CONCEPTS:
    • Future-proofing = Often unnecessary complexity
    • Speculative development = Wasted effort
    • Requirements change = Today's "might need" is tomorrow's dead code
    • Cost of change = Lower when codebase is smaller
    • Premature optimization = Root of all evil

VIOLATION SIGNS:
    🚨 "We might need this someday"
    🚨 "What if the requirements change?"
    🚨 "Let's make it configurable just in case"
    🚨 "I'll add hooks for future features"
    🚨 "This will be useful eventually"

CORE QUESTION:
    "Do I need this RIGHT NOW for current requirements?"
    If NO → Don't build it!
*/

// ============================================
// EXAMPLE 1: CONFIGURATION OVERKILL
// ============================================

namespace bad_example1 {
// ❌ BAD: Over-configurable for simple needs

class EmailConfig {
public:
    std::string host;
    int port;
    std::string username;
    std::string password;
    bool useTLS;
    bool useSSL;
    int timeout;
    int retries;
    int retryDelay;
    bool enableLogging;
    std::string logLevel;
    std::string logFormat;
    std::string logDestination;
    bool enableMetrics;
    std::string metricsEndpoint;
    bool enableCache;
    int cacheSize;
    int cacheTTL;
    // ... 20 more "might need" options

    EmailConfig() :
        port(25), timeout(30), retries(3), retryDelay(5),
        useTLS(false), useSSL(false), enableLogging(false),
        logLevel("INFO"), logFormat("json"), logDestination("stdout"),
        enableMetrics(false), enableCache(false),
        cacheSize(100), cacheTTL(3600) {}
};

class EmailService {
private:
    EmailConfig config;

public:
    EmailService(const EmailConfig& cfg) : config(cfg) {}

    void sendEmail(const std::string& to, const std::string& subject, const std::string& body) {
        std::println("Sending with {}:{}", config.host, config.port);
        // 🚨 Problem: 90% of config options never used!
        // 🚨 All this complexity for "might need someday"
    }
};

// PROBLEMS:
// • 20+ configuration options
// • Most never used
// • Complex initialization
// • Hard to understand what's actually needed
// • Maintains unused code
}

namespace good_example1 {
// ✅ GOOD: Only what's needed NOW

class EmailService {
private:
    std::string host;

public:
    EmailService(const std::string& emailHost) : host(emailHost) {}

    void sendEmail(const std::string& to, const std::string& subject, const std::string& body) {
        std::println("Sending to: {}", to);
        std::println("Subject: {}", subject);
        std::println("Via: {}", host);
        // ✅ Simple, meets current requirements
        // Add complexity only when needed!
    }
};
}

void example1_configuration() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 1: CONFIGURATION OVERKILL               ║");
    std::println("╚═══════════════════════════════════════════════════╝");

    good_example1::EmailService service("smtp.example.com");
    service.sendEmail("user@example.com", "Test", "Hello World");

}

// ============================================
// EXAMPLE 2: PREMATURE ABSTRACTION
// ============================================

namespace bad_example2 {
// ❌ BAD: Building for multiple databases "just in case"

class DatabaseInterface {
public:
    virtual ~DatabaseInterface() = default;
    virtual void connect() = 0;
    virtual void disconnect() = 0;
    virtual void query(const std::string& sql) = 0;
    virtual void transaction(std::function<void()> work) = 0;
};

class MySQLDatabase : public DatabaseInterface {
public:
    void connect() override { std::println("MySQL: Connected"); }
    void disconnect() override { std::println("MySQL: Disconnected"); }
    void query(const std::string& sql) override { std::println("MySQL: {}", sql); }
    void transaction(std::function<void()> work) override { work(); }
};

class PostgreSQLDatabase : public DatabaseInterface {
public:
    void connect() override { std::println("PostgreSQL: Connected"); }
    void disconnect() override { std::println("PostgreSQL: Disconnected"); }
    void query(const std::string& sql) override { std::println("PostgreSQL: {}", sql); }
    void transaction(std::function<void()> work) override { work(); }
};

class MongoDatabase : public DatabaseInterface {
public:
    void connect() override { std::println("MongoDB: Connected"); }
    void disconnect() override { std::println("MongoDB: Disconnected"); }
    void query(const std::string& sql) override { std::println("MongoDB: {}", sql); }
    void transaction(std::function<void()> work) override { work(); }
};

class DatabaseFactory {
public:
    static std::shared_ptr<DatabaseInterface> create(const std::string& type) {
        if (type == "mysql") return std::make_shared<MySQLDatabase>();
        if (type == "postgresql") return std::make_shared<PostgreSQLDatabase>();
        if (type == "mongodb") return std::make_shared<MongoDatabase>();
        return nullptr;
    }
};

// 🚨 PROBLEM: Built support for 3 databases
// 🚨 Reality: Only using MySQL
// 🚨 "We might switch databases someday" → Never happens
}

namespace good_example2 {
// ✅ GOOD: One database, the one we actually use

class Database {
public:
    void connect() {
        std::println("MySQL: Connected");
    }

    void query(const std::string& sql) {
        std::println("MySQL: {}", sql);
    }
};

// When we ACTUALLY need multiple databases:
// 1. Requirements appear
// 2. THEN add abstraction
// 3. Refactor existing code
// Until then: Keep it simple!
}

void example2_premature_abstraction() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 2: PREMATURE ABSTRACTION                ║");
    std::println("╚═══════════════════════════════════════════════════╝");

}

// ============================================
// EXAMPLE 3: FEATURE BLOAT
// ============================================

namespace bad_example3 {
// ❌ BAD: User class with "might need" features

class User {
private:
    std::string username;
    std::string password;
    std::string email;

    // 🚨 "Might need" features:
    std::string phoneNumber;           // "Users might want to add phone"
    std::string address;                // "Might need for shipping"
    std::string profilePicture;         // "Social features someday"
    std::vector<std::string> friends;        // "Friend system planned"
    std::vector<std::string> blockedUsers;   // "Might need blocking"
    int reputationScore;           // "Gamification ideas"
    std::vector<std::string> badges;         // "Achievement system"
    std::string preferredLanguage;      // "Internationalization"
    std::string timezone;               // "Timezone support"
    bool isPremium;                // "Premium tier planned"
    std::string subscriptionLevel;      // "Multiple tiers maybe"
    // ... 15 more speculative features

public:
    User(const std::string& user, const std::string& pass, const std::string& mail)
        : username(user), password(pass), email(mail),
        reputationScore(0), isPremium(false) {}

    // Methods for all the speculative features...
    void addFriend(const std::string& friendName) { friends.push_back(friendName); }
    void blockUser(const std::string& userName) { blockedUsers.push_back(userName); }
    void awardBadge(const std::string& badge) { badges.push_back(badge); }
    // ... 20 more methods never used
};

// PROBLEMS:
// • 20+ fields for hypothetical features
// • Current requirement: Just username, password, email
// • All other fields: Dead weight
// • Complex constructor initialization
// • Methods never called
}

namespace good_example3 {
// ✅ GOOD: Only current requirements

class User {
private:
    std::string username;
    std::string password;
    std::string email;

public:
    User(const std::string& user, const std::string& pass, const std::string& mail)
        : username(user), password(pass), email(mail) {}

    std::string getUsername() const { return username; }
    std::string getEmail() const { return email; }
    bool verifyPassword(const std::string& pass) const {
        return password == pass;
    }
};

// When friend system is ACTUALLY needed:
// 1. Add friends vector
// 2. Add friend methods
// Until then: Don't carry the weight!
}

void example3_feature_bloat() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 3: FEATURE BLOAT                        ║");
    std::println("╚═══════════════════════════════════════════════════╝");

    std::println("   ");
    std::println("   ");

    good_example3::User user("john_doe", "secret123", "john@example.com");
    std::println("   Created user: {}", user.getUsername());
    std::println("   Email: {}", user.getEmail());
    std::println("   Password valid: {}", (user.verifyPassword("secret123") ? "Yes" : "No"));

    std::println("   ");
}

// ============================================
// EXAMPLE 4: EXTENSIBILITY OVERKILL
// ============================================

namespace bad_example4 {
// ❌ BAD: Plugin system for simple calculator

class CalculatorPlugin {
public:
    virtual ~CalculatorPlugin() = default;
    virtual std::string getName() const = 0;
    virtual double execute(double a, double b) = 0;
};

class PluginManager {
private:
    std::map<std::string, std::shared_ptr<CalculatorPlugin>> plugins;

public:
    void registerPlugin(std::shared_ptr<CalculatorPlugin> plugin) {
        plugins[plugin->getName()] = plugin;
    }

    double executePlugin(const std::string& name, double a, double b) {
        return plugins[name]->execute(a, b);
    }

    std::vector<std::string> listPlugins() {
        std::vector<std::string> names;
        for (const auto& pair : plugins) {
            names.push_back(pair.first);
        }
        return names;
    }
};

class Calculator {
private:
    PluginManager pluginManager;

public:
    void loadPlugins() {
        // Plugin loading system
        // Configuration files
        // Dynamic loading
        // All for basic math!
    }

    double calculate(const std::string& operation, double a, double b) {
        return pluginManager.executePlugin(operation, a, b);
    }
};

// 🚨 PROBLEM: Built entire plugin architecture
// 🚨 Reality: Just need add/subtract/multiply/divide
// 🚨 "Users might want custom operations" → Never requested
}

namespace good_example4 {
// ✅ GOOD: Simple calculator for current needs

class Calculator {
public:
    double add(double a, double b) { return a + b; }
    double subtract(double a, double b) { return a - b; }
    double multiply(double a, double b) { return a * b; }
    double divide(double a, double b) {
        return b != 0 ? a / b : 0;
    }
};

// If custom operations needed later:
// → Add plugin system THEN
// Until then: Keep it simple!
}

void example4_extensibility() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 4: EXTENSIBILITY OVERKILL               ║");
    std::println("╚═══════════════════════════════════════════════════╝");

    std::println("   ");

    good_example4::Calculator calc;
    std::println("   10 + 5 = {}", calc.add(10, 5));
    std::println("   10 - 5 = {}", calc.subtract(10, 5));
    std::println("   10 * 5 = {}", calc.multiply(10, 5));
    std::println("   10 / 5 = {}", calc.divide(10, 5));

}

// ============================================
// EXAMPLE 5: PREMATURE OPTIMIZATION
// ============================================

namespace bad_example5 {
// ❌ BAD: Optimizing before measuring

class DataCache {
private:
    std::map<std::string, std::string> primaryCache;
    std::map<std::string, std::string> secondaryCache;
    std::vector<std::string> lruQueue;
    std::map<std::string, int> accessCount;
    std::map<std::string, long> lastAccessTime;
    bool useCompression;
    bool useEncryption;
    int maxCacheSize;

public:
    DataCache() : useCompression(true), useEncryption(true), maxCacheSize(1000) {}

    std::string get(const std::string& key) {
        // Check primary cache
        // Check secondary cache
        // Update LRU
        // Update access count
        // Update access time
        // Decompress if needed
        // Decrypt if needed
        // All for hypothetical performance!
        return primaryCache[key];
    }

    void put(const std::string& key, const std::string& value) {
        // Compress data
        // Encrypt data
        // Manage two-level cache
        // Update LRU
        // Evict if necessary
        // Complex for no proven need!
        primaryCache[key] = value;
    }
};

// 🚨 PROBLEM: Complex caching for unproven performance need
// 🚨 Reality: Simple map would work fine
// 🚨 "Might need performance" → Premature optimization
}

namespace good_example5 {
// ✅ GOOD: Simple solution, optimize later if needed

class DataCache {
private:
    std::map<std::string, std::string> cache;

public:
    std::string get(const std::string& key) {
        return cache[key];
    }

    void put(const std::string& key, const std::string& value) {
        cache[key] = value;
    }
};

// When performance becomes an issue:
// 1. MEASURE the bottleneck
// 2. THEN optimize
// Don't optimize before measuring!
}

void example5_premature_optimization() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 5: PREMATURE OPTIMIZATION               ║");
    std::println("╚═══════════════════════════════════════════════════╝");

    std::println("   ");

    std::println("   ");
}

// ============================================
// EXAMPLE 6: WHEN TO BUILD FOR FUTURE
// ============================================

namespace example6 {
// Sometimes you DO need to plan ahead!

// ✅ JUSTIFIED: Public API (hard to change later)
class PaymentAPI {
public:
    // Version in API is justified
    // Breaking changes are expensive
    virtual bool processPayment(double amount, const std::string& currency) = 0;
    virtual std::string getAPIVersion() const { return "1.0"; }
};

// ✅ JUSTIFIED: Database schema (migrations are costly)
struct UserTable {
    // Some forward thinking in schema is okay
    // Adding columns later requires migrations
    int id;
    std::string username;
    std::string email;
    long createdAt;  // Good to have timestamp
    long updatedAt;  // Good to have timestamp
};

// ❌ NOT JUSTIFIED: Internal helper class
class InternalHelper {
    // Don't over-engineer internal code
    // Easy to change later
    // Just build what's needed now
};
}

void example6_when_justified() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 6: WHEN TO BUILD FOR FUTURE             ║");
    std::println("╚═══════════════════════════════════════════════════╝");

}

// ============================================
// EXAMPLE 7: COST OF SPECULATION
// ============================================

void example7_cost_of_speculation() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 7: COST OF SPECULATION                  ║");
    std::println("╚═══════════════════════════════════════════════════╝");

}

// ============================================
// EXAMPLE 8: HOW TO PRACTICE YAGNI
// ============================================

void example8_how_to_practice() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 8: HOW TO PRACTICE YAGNI                ║");
    std::println("╚═══════════════════════════════════════════════════╝");

    std::println("   ");

    std::println("   ");
    std::println("   ");

    std::println("   ");

    std::println("   ");

}

// ============================================
// EXAMPLE 9: YAGNI vs OTHER PRINCIPLES
// ============================================

void example9_with_other_principles() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 9: YAGNI & OTHER PRINCIPLES             ║");
    std::println("╚═══════════════════════════════════════════════════╝");

}

// ============================================
// EXAMPLE 10: SUMMARY & BENEFITS
// ============================================

void example10_summary() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 10: SUMMARY & BENEFITS                  ║");
    std::println("╚═══════════════════════════════════════════════════╝");

    std::println("   ════════════════════════════════════");

    std::println("   ════════════════════════════════════");

    std::println("   ");
}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    std::println("╔═══════════════════════════════════════════════════════╗");
    std::println("║                                                       ║");
    std::println("║      YAGNI PRINCIPLE (You Aren't Gonna Need It)       ║");
    std::println("║                                                       ║");
    std::println("║  \"Always implement things when you actually need     ║");
    std::println("║   them, never when you just foresee that you need     ║");
    std::println("║   them.\"                                             ║");
    std::println("║                                                       ║");
    std::println("╚═══════════════════════════════════════════════════════╝");

    example1_configuration();
    example2_premature_abstraction();
    example3_feature_bloat();
    example4_extensibility();
    example5_premature_optimization();
    example6_when_justified();
    example7_cost_of_speculation();
    example8_how_to_practice();
    example9_with_other_principles();
    example10_summary();

    return 0;
}
