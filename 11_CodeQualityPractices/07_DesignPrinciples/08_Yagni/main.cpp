#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <functional>
using namespace std;

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
    string host;
    int port;
    string username;
    string password;
    bool useTLS;
    bool useSSL;
    int timeout;
    int retries;
    int retryDelay;
    bool enableLogging;
    string logLevel;
    string logFormat;
    string logDestination;
    bool enableMetrics;
    string metricsEndpoint;
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

    void sendEmail(const string& to, const string& subject, const string& body) {
        cout << "Sending with " << config.host << ":" << config.port << endl;
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
    string host;

public:
    EmailService(const string& emailHost) : host(emailHost) {}

    void sendEmail(const string& to, const string& subject, const string& body) {
        cout << "Sending to: " << to << endl;
        cout << "Subject: " << subject << endl;
        cout << "Via: " << host << endl;
        // ✅ Simple, meets current requirements
        // Add complexity only when needed!
    }
};
}

void example1_configuration() {
    cout << "\n╔═══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 1: CONFIGURATION OVERKILL               ║\n";
    cout << "╚═══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (Over-configured):\n";
    cout << "```cpp\n";
    cout << "class EmailConfig {\n";
    cout << "    string host, username, password;\n";
    cout << "    int port, timeout, retries, retryDelay;\n";
    cout << "    bool useTLS, useSSL, enableLogging, enableMetrics;\n";
    cout << "    string logLevel, logFormat, logDestination;\n";
    cout << "    // ... 20 more options for \"might need\"\n";
    cout << "};\n";
    cout << "// 🚨 90% of options never used!\n";
    cout << "```\n";

    cout << "\n🚨 PROBLEMS:\n";
    cout << "   • 20+ configuration options added speculatively\n";
    cout << "   • Most options never used in practice\n";
    cout << "   • Complex initialization for simple task\n";
    cout << "   • Maintaining dead code\n";

    cout << "\n✅ GOOD CODE (Current needs only):\n";
    cout << "```cpp\n";
    cout << "class EmailService {\n";
    cout << "private:\n";
    cout << "    string host;\n";
    cout << "public:\n";
    cout << "    EmailService(string host) : host(host) {}\n";
    cout << "    void sendEmail(string to, string subject, string body);\n";
    cout << "};\n";
    cout << "// ✅ Only what's needed NOW!\n";
    cout << "```\n";

    cout << "\n✨ DEMONSTRATION:\n";

    good_example1::EmailService service("smtp.example.com");
    service.sendEmail("user@example.com", "Test", "Hello World");

    cout << "\n💡 LESSON:\n";
    cout << "   Don't add configuration options for hypothetical needs.\n";
    cout << "   Add them when requirements actually appear!\n";
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
    virtual void query(const string& sql) = 0;
    virtual void transaction(function<void()> work) = 0;
};

class MySQLDatabase : public DatabaseInterface {
public:
    void connect() override { cout << "MySQL: Connected\n"; }
    void disconnect() override { cout << "MySQL: Disconnected\n"; }
    void query(const string& sql) override { cout << "MySQL: " << sql << endl; }
    void transaction(function<void()> work) override { work(); }
};

class PostgreSQLDatabase : public DatabaseInterface {
public:
    void connect() override { cout << "PostgreSQL: Connected\n"; }
    void disconnect() override { cout << "PostgreSQL: Disconnected\n"; }
    void query(const string& sql) override { cout << "PostgreSQL: " << sql << endl; }
    void transaction(function<void()> work) override { work(); }
};

class MongoDatabase : public DatabaseInterface {
public:
    void connect() override { cout << "MongoDB: Connected\n"; }
    void disconnect() override { cout << "MongoDB: Disconnected\n"; }
    void query(const string& sql) override { cout << "MongoDB: " << sql << endl; }
    void transaction(function<void()> work) override { work(); }
};

class DatabaseFactory {
public:
    static shared_ptr<DatabaseInterface> create(const string& type) {
        if (type == "mysql") return make_shared<MySQLDatabase>();
        if (type == "postgresql") return make_shared<PostgreSQLDatabase>();
        if (type == "mongodb") return make_shared<MongoDatabase>();
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
        cout << "MySQL: Connected\n";
    }

    void query(const string& sql) {
        cout << "MySQL: " << sql << endl;
    }
};

// When we ACTUALLY need multiple databases:
// 1. Requirements appear
// 2. THEN add abstraction
// 3. Refactor existing code
// Until then: Keep it simple!
}

void example2_premature_abstraction() {
    cout << "\n╔═══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 2: PREMATURE ABSTRACTION                ║\n";
    cout << "╚═══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (Speculative support):\n";
    cout << "   • DatabaseInterface abstract class\n";
    cout << "   • MySQLDatabase implementation\n";
    cout << "   • PostgreSQLDatabase implementation\n";
    cout << "   • MongoDatabase implementation\n";
    cout << "   • DatabaseFactory pattern\n";
    cout << "   • 5 classes, 100+ lines of code\n";
    cout << "   🚨 Reality: Only MySQL is used!\n";

    cout << "\n✅ GOOD CODE (Current need):\n";
    cout << "   • One Database class\n";
    cout << "   • 10 lines of code\n";
    cout << "   • Works perfectly for MySQL\n";
    cout << "   • Will refactor WHEN other databases needed\n";

    cout << "\n💡 LESSON:\n";
    cout << "   \"We might switch databases\" is speculation.\n";
    cout << "   Build abstractions when requirements are REAL,\n";
    cout << "   not when they're hypothetical!\n";

    cout << "\n📊 REALITY CHECK:\n";
    cout << "   • 90% of \"might switch\" never happens\n";
    cout << "   • When it does, refactoring is straightforward\n";
    cout << "   • Cost of maintaining dead code > cost of refactoring\n";
}

// ============================================
// EXAMPLE 3: FEATURE BLOAT
// ============================================

namespace bad_example3 {
// ❌ BAD: User class with "might need" features

class User {
private:
    string username;
    string password;
    string email;

    // 🚨 "Might need" features:
    string phoneNumber;           // "Users might want to add phone"
    string address;                // "Might need for shipping"
    string profilePicture;         // "Social features someday"
    vector<string> friends;        // "Friend system planned"
    vector<string> blockedUsers;   // "Might need blocking"
    int reputationScore;           // "Gamification ideas"
    vector<string> badges;         // "Achievement system"
    string preferredLanguage;      // "Internationalization"
    string timezone;               // "Timezone support"
    bool isPremium;                // "Premium tier planned"
    string subscriptionLevel;      // "Multiple tiers maybe"
    // ... 15 more speculative features

public:
    User(const string& user, const string& pass, const string& mail)
        : username(user), password(pass), email(mail),
        reputationScore(0), isPremium(false) {}

    // Methods for all the speculative features...
    void addFriend(const string& friendName) { friends.push_back(friendName); }
    void blockUser(const string& userName) { blockedUsers.push_back(userName); }
    void awardBadge(const string& badge) { badges.push_back(badge); }
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
    string username;
    string password;
    string email;

public:
    User(const string& user, const string& pass, const string& mail)
        : username(user), password(pass), email(mail) {}

    string getUsername() const { return username; }
    string getEmail() const { return email; }
    bool verifyPassword(const string& pass) const {
        return password == pass;
    }
};

// When friend system is ACTUALLY needed:
// 1. Add friends vector
// 2. Add friend methods
// Until then: Don't carry the weight!
}

void example3_feature_bloat() {
    cout << "\n╔═══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 3: FEATURE BLOAT                        ║\n";
    cout << "╚═══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (Speculative features):\n";
    cout << "   Current requirement: username, password, email\n";
    cout << "   \n";
    cout << "   Added \"just in case\":\n";
    cout << "   • phoneNumber (\"users might want this\")\n";
    cout << "   • address (\"for shipping someday\")\n";
    cout << "   • profilePicture (\"social features planned\")\n";
    cout << "   • friends vector (\"friend system ideas\")\n";
    cout << "   • blockedUsers (\"might need blocking\")\n";
    cout << "   • reputationScore (\"gamification\")\n";
    cout << "   • badges (\"achievement system\")\n";
    cout << "   • 15 more speculative features...\n";
    cout << "   \n";
    cout << "   🚨 Result: 25 fields, 3 used!\n";

    cout << "\n✅ GOOD CODE (Actual requirements):\n";
    cout << "   • username\n";
    cout << "   • password\n";
    cout << "   • email\n";
    cout << "   That's it!\n";

    cout << "\n✨ DEMONSTRATION:\n";

    good_example3::User user("john_doe", "secret123", "john@example.com");
    cout << "   Created user: " << user.getUsername() << endl;
    cout << "   Email: " << user.getEmail() << endl;
    cout << "   Password valid: " << (user.verifyPassword("secret123") ? "Yes" : "No") << endl;

    cout << "\n💡 LESSON:\n";
    cout << "   Don't add fields for hypothetical features.\n";
    cout << "   Requirements: \"We need username/password/email\"\n";
    cout << "   YAGNI: Build exactly that!\n";
    cout << "   \n";
    cout << "   When friend system is ACTUALLY requested:\n";
    cout << "   → Add it THEN, not before!\n";
}

// ============================================
// EXAMPLE 4: EXTENSIBILITY OVERKILL
// ============================================

namespace bad_example4 {
// ❌ BAD: Plugin system for simple calculator

class CalculatorPlugin {
public:
    virtual ~CalculatorPlugin() = default;
    virtual string getName() const = 0;
    virtual double execute(double a, double b) = 0;
};

class PluginManager {
private:
    map<string, shared_ptr<CalculatorPlugin>> plugins;

public:
    void registerPlugin(shared_ptr<CalculatorPlugin> plugin) {
        plugins[plugin->getName()] = plugin;
    }

    double executePlugin(const string& name, double a, double b) {
        return plugins[name]->execute(a, b);
    }

    vector<string> listPlugins() {
        vector<string> names;
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

    double calculate(const string& operation, double a, double b) {
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
    cout << "\n╔═══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 4: EXTENSIBILITY OVERKILL               ║\n";
    cout << "╚═══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (Over-engineered):\n";
    cout << "   Built for simple calculator:\n";
    cout << "   • Plugin system\n";
    cout << "   • Plugin manager\n";
    cout << "   • Plugin interface\n";
    cout << "   • Plugin registration\n";
    cout << "   • Plugin discovery\n";
    cout << "   • Configuration system\n";
    cout << "   \n";
    cout << "   🚨 All for: add, subtract, multiply, divide!\n";

    cout << "\n✅ GOOD CODE (Current needs):\n";
    cout << "   • Four simple methods\n";
    cout << "   • 15 lines of code\n";
    cout << "   • Works perfectly\n";

    cout << "\n✨ DEMONSTRATION:\n";

    good_example4::Calculator calc;
    cout << "   10 + 5 = " << calc.add(10, 5) << endl;
    cout << "   10 - 5 = " << calc.subtract(10, 5) << endl;
    cout << "   10 * 5 = " << calc.multiply(10, 5) << endl;
    cout << "   10 / 5 = " << calc.divide(10, 5) << endl;

    cout << "\n💡 LESSON:\n";
    cout << "   \"Users might want plugins\" is speculation.\n";
    cout << "   Build plugin system when:\n";
    cout << "   • User requests it\n";
    cout << "   • You have 3+ concrete examples\n";
    cout << "   • Not before!\n";
}

// ============================================
// EXAMPLE 5: PREMATURE OPTIMIZATION
// ============================================

namespace bad_example5 {
// ❌ BAD: Optimizing before measuring

class DataCache {
private:
    map<string, string> primaryCache;
    map<string, string> secondaryCache;
    vector<string> lruQueue;
    map<string, int> accessCount;
    map<string, long> lastAccessTime;
    bool useCompression;
    bool useEncryption;
    int maxCacheSize;

public:
    DataCache() : useCompression(true), useEncryption(true), maxCacheSize(1000) {}

    string get(const string& key) {
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

    void put(const string& key, const string& value) {
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
    map<string, string> cache;

public:
    string get(const string& key) {
        return cache[key];
    }

    void put(const string& key, const string& value) {
        cache[key] = value;
    }
};

// When performance becomes an issue:
// 1. MEASURE the bottleneck
// 2. THEN optimize
// Don't optimize before measuring!
}

void example5_premature_optimization() {
    cout << "\n╔═══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 5: PREMATURE OPTIMIZATION               ║\n";
    cout << "╚═══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (Premature optimization):\n";
    cout << "   • Two-level cache system\n";
    cout << "   • LRU eviction algorithm\n";
    cout << "   • Access count tracking\n";
    cout << "   • Compression layer\n";
    cout << "   • Encryption layer\n";
    cout << "   • 150+ lines of code\n";
    cout << "   \n";
    cout << "   🚨 No performance measurements!\n";
    cout << "   🚨 \"Might need to be fast\" speculation\n";

    cout << "\n✅ GOOD CODE (Simple first):\n";
    cout << "   • map<string, string>\n";
    cout << "   • 10 lines of code\n";
    cout << "   • Works perfectly\n";
    cout << "   • Optimize AFTER measuring!\n";

    cout << "\n💡 FAMOUS QUOTE:\n";
    cout << "   \"Premature optimization is the root of all evil.\"\n";
    cout << "   — Donald Knuth\n";

    cout << "\n📊 PROCESS:\n";
    cout << "   1. Build simple version\n";
    cout << "   2. MEASURE performance\n";
    cout << "   3. Find ACTUAL bottlenecks\n";
    cout << "   4. THEN optimize\n";
    cout << "   \n";
    cout << "   Don't optimize hypothetical problems!\n";
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
    virtual bool processPayment(double amount, const string& currency) = 0;
    virtual string getAPIVersion() const { return "1.0"; }
};

// ✅ JUSTIFIED: Database schema (migrations are costly)
struct UserTable {
    // Some forward thinking in schema is okay
    // Adding columns later requires migrations
    int id;
    string username;
    string email;
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
    cout << "\n╔═══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 6: WHEN TO BUILD FOR FUTURE             ║\n";
    cout << "╚═══════════════════════════════════════════════════╝\n";

    cout << "\n✅ YAGNI EXCEPTIONS (Plan ahead when):\n";

    cout << "\n1. PUBLIC APIs:\n";
    cout << "   • External users depend on interface\n";
    cout << "   • Breaking changes are expensive\n";
    cout << "   • Versioning makes sense\n";
    cout << "   • Some forward thinking justified\n";

    cout << "\n2. DATABASE SCHEMAS:\n";
    cout << "   • Migrations are costly\n";
    cout << "   • Common fields often needed\n";
    cout << "   • created_at, updated_at usually good\n";
    cout << "   • But don't add every possible field!\n";

    cout << "\n3. FILE FORMATS:\n";
    cout << "   • Hard to change once deployed\n";
    cout << "   • Version number useful\n";
    cout << "   • Some extensibility makes sense\n";

    cout << "\n❌ YAGNI APPLIES TO (Don't plan ahead):\n";

    cout << "\n1. INTERNAL CODE:\n";
    cout << "   • Easy to refactor\n";
    cout << "   • No external dependencies\n";
    cout << "   • Build exactly what's needed\n";

    cout << "\n2. FEATURES:\n";
    cout << "   • Don't build unused features\n";
    cout << "   • Wait for real requirements\n";
    cout << "   • User needs drive development\n";

    cout << "\n3. ABSTRACTIONS:\n";
    cout << "   • Wait for pattern to emerge\n";
    cout << "   • Needs 3+ concrete examples\n";
    cout << "   • Refactor when pattern is clear\n";

    cout << "\n⚖️ THE BALANCE:\n";
    cout << "   Public API -----> Plan ahead (hard to change)\n";
    cout << "   Internal code --> YAGNI (easy to change)\n";
}

// ============================================
// EXAMPLE 7: COST OF SPECULATION
// ============================================

void example7_cost_of_speculation() {
    cout << "\n╔═══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 7: COST OF SPECULATION                  ║\n";
    cout << "╚═══════════════════════════════════════════════════╝\n";

    cout << "\n💰 COSTS OF BUILDING \"JUST IN CASE\":\n";

    cout << "\n1. DEVELOPMENT TIME:\n";
    cout << "   • Time spent on unused features\n";
    cout << "   • Could build actual requirements instead\n";
    cout << "   • Opportunity cost is real\n";

    cout << "\n2. MAINTENANCE BURDEN:\n";
    cout << "   • Dead code needs maintenance\n";
    cout << "   • Tests for unused features\n";
    cout << "   • Documentation for unused features\n";
    cout << "   • Confuses new developers\n";

    cout << "\n3. INCREASED COMPLEXITY:\n";
    cout << "   • Harder to understand\n";
    cout << "   • More places for bugs\n";
    cout << "   • Slower to make changes\n";

    cout << "\n4. WRONG PREDICTIONS:\n";
    cout << "   • 90% of \"might need\" never happens\n";
    cout << "   • When it does, requirements differ\n";
    cout << "   • Built wrong solution to wrong problem\n";

    cout << "\n5. DELAYED VALUE:\n";
    cout << "   • Actual features delayed\n";
    cout << "   • Users waiting for real needs\n";
    cout << "   • Wasted effort on speculation\n";

    cout << "\n📊 STATISTICS:\n";
    cout << "   • 64% of features rarely/never used (Standish Group)\n";
    cout << "   • 45% of features never used at all\n";
    cout << "   • Cost to maintain > cost to build\n";

    cout << "\n✅ BENEFITS OF YAGNI:\n";

    cout << "\n1. FASTER DELIVERY:\n";
    cout << "   • Build only what's needed\n";
    cout << "   • Ship features faster\n";
    cout << "   • Real user value sooner\n";

    cout << "\n2. LOWER COSTS:\n";
    cout << "   • Less code to write\n";
    cout << "   • Less code to maintain\n";
    cout << "   • Less code to test\n";

    cout << "\n3. BETTER DECISIONS:\n";
    cout << "   • Real requirements guide design\n";
    cout << "   • Actual usage informs features\n";
    cout << "   • Data-driven development\n";

    cout << "\n4. EASIER MAINTENANCE:\n";
    cout << "   • Smaller codebase\n";
    cout << "   • Less complexity\n";
    cout << "   • Faster changes\n";
}

// ============================================
// EXAMPLE 8: HOW TO PRACTICE YAGNI
// ============================================

void example8_how_to_practice() {
    cout << "\n╔═══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 8: HOW TO PRACTICE YAGNI                ║\n";
    cout << "╚═══════════════════════════════════════════════════╝\n";

    cout << "\n✅ STRATEGIES FOR YAGNI:\n";

    cout << "\n1. THE FUNDAMENTAL QUESTION:\n";
    cout << "   Before writing ANY code, ask:\n";
    cout << "   \"Do I need this RIGHT NOW?\"\n";
    cout << "   \n";
    cout << "   If NO → Don't build it!\n";

    cout << "\n2. CHALLENGE ASSUMPTIONS:\n";
    cout << "   ❌ \"We might need this someday\"\n";
    cout << "   ✅ \"Is this needed for current requirements?\"\n";
    cout << "   \n";
    cout << "   ❌ \"What if requirements change?\"\n";
    cout << "   ✅ \"What are the requirements NOW?\"\n";
    cout << "   \n";
    cout << "   ❌ \"Let's make it flexible\"\n";
    cout << "   ✅ \"Let's make it work\"\n";

    cout << "\n3. BUILD MINIMUM VIABLE SOLUTION:\n";
    cout << "   • Simplest thing that could work\n";
    cout << "   • Meets current requirements exactly\n";
    cout << "   • No more, no less\n";

    cout << "\n4. WAIT FOR PATTERNS:\n";
    cout << "   First time:  Write it\n";
    cout << "   Second time: Duplicate it (yes, really!)\n";
    cout << "   Third time:  Abstract it\n";
    cout << "   \n";
    cout << "   Rule of Three: Need 3 examples before abstracting\n";

    cout << "\n5. REFACTOR WHEN NEEDED:\n";
    cout << "   • Start simple\n";
    cout << "   • When requirements appear, refactor\n";
    cout << "   • Refactoring is cheaper than maintaining dead code\n";

    cout << "\n6. TRUST THE PROCESS:\n";
    cout << "   • Requirements will emerge\n";
    cout << "   • Users will tell you what they need\n";
    cout << "   • Don't guess, listen\n";

    cout << "\n📋 YAGNI CHECKLIST:\n";
    cout << "   □ Is this required for current functionality?\n";
    cout << "   □ Has someone actually requested this?\n";
    cout << "   □ Will this be used in the next release?\n";
    cout << "   □ Do I have concrete examples of the need?\n";
    cout << "   □ Is the cost of adding later high?\n";
    cout << "   \n";
    cout << "   If all NO → Don't build it!\n";

    cout << "\n🚫 RED FLAGS:\n";
    cout << "   • \"Just in case\"\n";
    cout << "   • \"Might need someday\"\n";
    cout << "   • \"What if...\"\n";
    cout << "   • \"Let's make it configurable\"\n";
    cout << "   • \"For future flexibility\"\n";
    cout << "   • \"We might want to...\"\n";

    cout << "\n✅ GREEN FLAGS:\n";
    cout << "   • \"The requirement says...\"\n";
    cout << "   • \"Users asked for...\"\n";
    cout << "   • \"This release needs...\"\n";
    cout << "   • \"We're using this now for...\"\n";
}

// ============================================
// EXAMPLE 9: YAGNI vs OTHER PRINCIPLES
// ============================================

void example9_with_other_principles() {
    cout << "\n╔═══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 9: YAGNI & OTHER PRINCIPLES             ║\n";
    cout << "╚═══════════════════════════════════════════════════╝\n";

    cout << "\n🔗 YAGNI + KISS:\n";
    cout << "   YAGNI:  Don't build it until needed\n";
    cout << "   KISS:   When you build it, keep it simple\n";
    cout << "   Result: Simple solutions to actual problems\n";

    cout << "\n🔗 YAGNI + DRY:\n";
    cout << "   YAGNI:  Wait for pattern to emerge\n";
    cout << "   DRY:    Then eliminate duplication\n";
    cout << "   Result: Abstract when pattern is clear (Rule of 3)\n";

    cout << "\n🔗 YAGNI + SOLID:\n";
    cout << "   YAGNI:  Build for current needs\n";
    cout << "   SOLID:  Design it well\n";
    cout << "   Result: Well-designed code for actual requirements\n";

    cout << "\n🔗 YAGNI + TDD:\n";
    cout << "   YAGNI:  Only what's needed\n";
    cout << "   TDD:    Test-driven development\n";
    cout << "   Result: Tests define \"needed\" - if no test, not needed!\n";

    cout << "\n⚠️ COMMON MISCONCEPTIONS:\n";

    cout << "\n❌ \"YAGNI means no planning\"\n";
    cout << "   ✅ YAGNI means plan for KNOWN requirements\n";
    cout << "   ✅ Don't plan for UNKNOWN hypotheticals\n";

    cout << "\n❌ \"YAGNI means poor design\"\n";
    cout << "   ✅ YAGNI means design for current needs\n";
    cout << "   ✅ Good design for actual requirements\n";

    cout << "\n❌ \"YAGNI means no refactoring\"\n";
    cout << "   ✅ YAGNI encourages refactoring when needed\n";
    cout << "   ✅ Refactor when patterns emerge\n";

    cout << "\n❌ \"YAGNI means technical debt\"\n";
    cout << "   ✅ Speculation creates technical debt\n";
    cout << "   ✅ YAGNI reduces debt by avoiding dead code\n";
}

// ============================================
// EXAMPLE 10: SUMMARY & BENEFITS
// ============================================

void example10_summary() {
    cout << "\n╔═══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 10: SUMMARY & BENEFITS                  ║\n";
    cout << "╚═══════════════════════════════════════════════════╝\n";

    cout << "\n✅ BENEFITS OF YAGNI:\n";

    cout << "\n1. SAVE TIME:\n";
    cout << "   • Focus on actual requirements\n";
    cout << "   • Ship features faster\n";
    cout << "   • Less wasted effort\n";

    cout << "\n2. REDUCE COMPLEXITY:\n";
    cout << "   • Smaller codebase\n";
    cout << "   • Fewer abstractions\n";
    cout << "   • Easier to understand\n";

    cout << "\n3. LOWER MAINTENANCE:\n";
    cout << "   • Less code to maintain\n";
    cout << "   • Less code to test\n";
    cout << "   • Less code to document\n";

    cout << "\n4. BETTER DESIGNS:\n";
    cout << "   • Real requirements guide design\n";
    cout << "   • Actual usage informs decisions\n";
    cout << "   • Avoid over-engineering\n";

    cout << "\n5. FLEXIBILITY:\n";
    cout << "   • Easier to change small codebase\n";
    cout << "   • Less coupling\n";
    cout << "   • Refactor when needed\n";

    cout << "\n💡 KEY INSIGHTS:\n";

    cout << "\n\"You Aren't Gonna Need It\"\n";
    cout << "— Ron Jeffries\n";

    cout << "\n\"Do the simplest thing that could possibly work.\"\n";
    cout << "— Ward Cunningham\n";

    cout << "\n\"Premature optimization is the root of all evil.\"\n";
    cout << "— Donald Knuth\n";

    cout << "\n\"The best code is no code at all.\"\n";
    cout << "— Jeff Atwood\n";

    cout << "\n🎯 REMEMBER:\n";
    cout << "   • Build for TODAY's requirements\n";
    cout << "   • Not TOMORROW's speculation\n";
    cout << "   • Real needs > Imagined needs\n";
    cout << "   • Refactor when requirements appear\n";
    cout << "   • Trust the process\n";

    cout << "\n📊 THE YAGNI PRINCIPLE IN ACTION:\n";

    cout << "\n   Without YAGNI:\n";
    cout << "   ════════════════════════════════════\n";
    cout << "   Requirements:     5 features\n";
    cout << "   Built:           20 features (\"just in case\")\n";
    cout << "   Actually used:    3 features\n";
    cout << "   Wasted effort:   17 features (85%!)\n";
    cout << "   Maintenance:     20 features forever\n";

    cout << "\n   With YAGNI:\n";
    cout << "   ════════════════════════════════════\n";
    cout << "   Requirements:     5 features\n";
    cout << "   Built:            5 features\n";
    cout << "   Actually used:    5 features\n";
    cout << "   Wasted effort:    0 features\n";
    cout << "   Maintenance:      5 features\n";

    cout << "\n⚖️ THE YAGNI DECISION TREE:\n";
    cout << "\n   Do I need this RIGHT NOW?\n";
    cout << "   ├─ YES → Build it\n";
    cout << "   └─ NO  → Don't build it\n";
    cout << "       │\n";
    cout << "       └─ But what if I need it later?\n";
    cout << "           ├─ If needed: Add it THEN\n";
    cout << "           └─ If not needed: Saved time!\n";

    cout << "\n🎓 FINAL WISDOM:\n";
    cout << "   The best code is the code you don't have to write.\n";
    cout << "   The best feature is the feature you don't have to maintain.\n";
    cout << "   The best abstraction is the abstraction you don't need.\n";
    cout << "   \n";
    cout << "   Build what you need, when you need it.\n";
    cout << "   YAGNI!\n";
}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║      YAGNI PRINCIPLE (You Aren't Gonna Need It)       ║\n";
    cout << "║                                                       ║\n";
    cout << "║  \"Always implement things when you actually need      ║\n";
    cout << "║   them, never when you just foresee that you need     ║\n";
    cout << "║   them.\"                                              ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n";

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

    cout << "\n\n";
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║            🎉 YAGNI PRINCIPLE COMPLETE! 🎉             ║\n";
    cout << "║                                                       ║\n";
    cout << "║  Key Takeaway: Build only what you need RIGHT NOW.    ║\n";
    cout << "║  Don't add functionality for hypothetical future      ║\n";
    cout << "║  needs. When requirements appear, THEN add the        ║\n";
    cout << "║  feature. Trust the process!                          ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n\n";

    return 0;
}
