# YAGNI Principle (You Aren't Gonna Need It)

## 📚 Overview

The **YAGNI Principle** states:

> **"You Aren't Gonna Need It"**  
> **"Always implement things when you actually need them, never when you just foresee that you need them."**  
> — Ron Jeffries (Extreme Programming)

### What It Means

- **Don't add functionality** until it's needed
- **Don't write code** for hypothetical future use
- **Don't build features** "just in case"
- **Focus on current requirements** only
- **Implement when needed**, not when anticipated

### Why It Matters

✅ **Saves development time** - Focus on actual needs  
✅ **Reduces complexity** - Smaller, simpler codebase  
✅ **Less maintenance burden** - Fewer features to maintain  
✅ **Prevents wasted effort** - No unused features  
✅ **Enables flexibility** - Easier to change smaller codebases  

---

## 🚨 Violation Signs

How to spot YAGNI violations:

- 🚨 "We might need this someday"
- 🚨 "What if the requirements change?"
- 🚨 "Let's make it configurable just in case"
- 🚨 "I'll add hooks for future features"
- 🚨 "This will be useful eventually"
- 🚨 Building plugin systems for simple tasks
- 🚨 Over-configurable classes with unused options
- 🚨 Abstractions before pattern emerges

---

## 🎯 The Core Question

Before writing ANY code, ask yourself:

```
┌─────────────────────────────────────┐
│  Do I need this RIGHT NOW?          │
│                                     │
│  ├─ YES → Build it                 │
│  └─ NO  → Don't build it           │
└─────────────────────────────────────┘
```

If the answer is **NO**, then **DON'T BUILD IT!**

---

## 📖 Classic Example 1: Configuration Overkill

### ❌ **Bad: Over-Configured Class**

```cpp
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
    // ... 15 more "might need" options
    
    EmailConfig() :
        port(25), timeout(30), retries(3), retryDelay(5),
        useTLS(false), useSSL(false), enableLogging(false),
        // ... initializing 20+ options
    {}
};

class EmailService {
private:
    EmailConfig config;
public:
    EmailService(const EmailConfig& cfg) : config(cfg) {}
    
    void sendEmail(const string& to, const string& subject, const string& body) {
        // Using config options...
        // 🚨 Problem: 90% of options never used!
    }
};
```

**Problems:**
- 20+ configuration options added speculatively
- Most options never used in practice
- Complex initialization for simple task
- Maintaining dead code

**Reasoning behind violations:**
- "Users might want SSL someday"
- "We might need logging"
- "Metrics could be useful"
- "Let's make it flexible"

### ✅ **Good: Only Current Needs**

```cpp
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
    }
};

// Usage:
EmailService service("smtp.example.com");
service.sendEmail("user@example.com", "Test", "Hello!");
```

**Solution:**
- Only what's needed NOW
- Simple and clear
- Easy to understand
- Add features WHEN requested

**When to add features:**
```
User request: "We need SSL support"
→ THEN add SSL configuration

User request: "We need logging"
→ THEN add logging option

NOT before!
```

---

## 📖 Classic Example 2: Premature Abstraction

### ❌ **Bad: Multi-Database Support "Just in Case"**

```cpp
// Abstract interface
class DatabaseInterface {
public:
    virtual ~DatabaseInterface() = default;
    virtual void connect() = 0;
    virtual void disconnect() = 0;
    virtual void query(const string& sql) = 0;
    virtual void transaction(function<void()> work) = 0;
};

// MySQL implementation
class MySQLDatabase : public DatabaseInterface {
public:
    void connect() override { cout << "MySQL: Connected\n"; }
    void disconnect() override { cout << "MySQL: Disconnected\n"; }
    void query(const string& sql) override { /* ... */ }
    void transaction(function<void()> work) override { /* ... */ }
};

// PostgreSQL implementation
class PostgreSQLDatabase : public DatabaseInterface {
public:
    void connect() override { cout << "PostgreSQL: Connected\n"; }
    void disconnect() override { cout << "PostgreSQL: Disconnected\n"; }
    void query(const string& sql) override { /* ... */ }
    void transaction(function<void()> work) override { /* ... */ }
};

// MongoDB implementation
class MongoDatabase : public DatabaseInterface {
public:
    void connect() override { cout << "MongoDB: Connected\n"; }
    void disconnect() override { cout << "MongoDB: Disconnected\n"; }
    void query(const string& sql) override { /* ... */ }
    void transaction(function<void()> work) override { /* ... */ }
};

// Factory pattern
class DatabaseFactory {
public:
    static shared_ptr<DatabaseInterface> create(const string& type) {
        if (type == "mysql") return make_shared<MySQLDatabase>();
        if (type == "postgresql") return make_shared<PostgreSQLDatabase>();
        if (type == "mongodb") return make_shared<MongoDatabase>();
        return nullptr;
    }
};

// 🚨 PROBLEM:
// - Built support for 3 databases
// - Reality: Only using MySQL
// - "We might switch databases someday" → Never happens
// - 5 classes, 150+ lines of code
```

### ✅ **Good: One Database (The One We Use)**

```cpp
class Database {
public:
    void connect() {
        cout << "MySQL: Connected\n";
    }
    
    void query(const string& sql) {
        cout << "MySQL: " << sql << endl;
    }
};

// Usage:
Database db;
db.connect();
db.query("SELECT * FROM users");

// ✅ Simple, works perfectly
// ✅ If we ACTUALLY need other databases later:
//    1. Requirements appear
//    2. THEN add abstraction
//    3. Refactor existing code
```

**Statistics:**
- 90% of "might switch databases" never happens
- When it does happen, refactoring is straightforward
- Cost of maintaining dead code > cost of refactoring

---

## 📖 Classic Example 3: Feature Bloat

### ❌ **Bad: Speculative Features**

```cpp
class User {
private:
    // Current requirements:
    string username;
    string password;
    string email;
    
    // 🚨 Speculative features added "just in case":
    string phoneNumber;           // "Users might want phone"
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
    string bio;                    // "Profile pages someday"
    string website;                // "Users might have sites"
    vector<string> interests;      // "Matching algorithm"
    // ... 10 more speculative fields
    
public:
    User(const string& user, const string& pass, const string& mail) 
        : username(user), password(pass), email(mail),
          reputationScore(0), isPremium(false) {
        // Initialize all speculative fields...
    }
    
    // Methods for all speculative features...
    void addFriend(const string& friendName) { /* ... */ }
    void blockUser(const string& userName) { /* ... */ }
    void awardBadge(const string& badge) { /* ... */ }
    void setTimezone(const string& tz) { /* ... */ }
    // ... 20 more methods never called
};

// 🚨 RESULT: 25 fields, only 3 used!
```

### ✅ **Good: Actual Requirements Only**

```cpp
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

// Usage:
User user("john_doe", "secret123", "john@example.com");
cout << user.getUsername() << endl;  // john_doe
cout << user.getEmail() << endl;     // john@example.com

// ✅ Simple, clear, meets requirements
// ✅ When friend system is ACTUALLY requested:
//    → Add friends vector THEN
//    → Not before!
```

**Lesson:**
```
Requirements say: "We need username, password, email"
YAGNI says: Build exactly that!

When request comes: "We need a friend system"
YAGNI says: Add it THEN, with real requirements!
```

---

## 📖 Classic Example 4: Extensibility Overkill

### ❌ **Bad: Plugin System for Simple Calculator**

```cpp
// Plugin interface
class CalculatorPlugin {
public:
    virtual ~CalculatorPlugin() = default;
    virtual string getName() const = 0;
    virtual double execute(double a, double b) = 0;
};

// Plugin manager
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
        // Return all available plugins...
    }
    
    void loadPluginsFromConfig() {
        // Load plugins from configuration file...
    }
};

// Calculator with plugin system
class Calculator {
private:
    PluginManager pluginManager;
    
public:
    void loadPlugins() {
        // Plugin discovery
        // Configuration parsing
        // Dynamic loading
        // All for basic math!
    }
    
    double calculate(const string& operation, double a, double b) {
        return pluginManager.executePlugin(operation, a, b);
    }
};

// 🚨 PROBLEM:
// - Built entire plugin architecture
// - Reality: Just need +, -, *, /
// - "Users might want custom operations" → Never requested
// - 200+ lines for basic calculator
```

### ✅ **Good: Simple Calculator**

```cpp
class Calculator {
public:
    double add(double a, double b) { return a + b; }
    double subtract(double a, double b) { return a - b; }
    double multiply(double a, double b) { return a * b; }
    double divide(double a, double b) { 
        return b != 0 ? a / b : 0; 
    }
};

// Usage:
Calculator calc;
cout << "10 + 5 = " << calc.add(10, 5) << endl;      // 15
cout << "10 - 5 = " << calc.subtract(10, 5) << endl; // 5
cout << "10 * 5 = " << calc.multiply(10, 5) << endl; // 50
cout << "10 / 5 = " << calc.divide(10, 5) << endl;   // 2

// ✅ 15 lines of code
// ✅ Clear and simple
// ✅ Meets all requirements

// If custom operations needed later:
// → Add plugin system THEN
// → With real examples of plugins
// → Not before!
```

**Decision Tree:**
```
Need plugin system?
├─ Have 3+ concrete plugin examples? → YES: Consider plugin system
└─ Hypothetical "might want plugins"? → NO: Don't build it
```

---

## 📖 Classic Example 5: Premature Optimization

### ❌ **Bad: Optimizing Before Measuring**

```cpp
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
    DataCache() : 
        useCompression(true), 
        useEncryption(true), 
        maxCacheSize(1000) {}
    
    string get(const string& key) {
        // Check primary cache
        // Check secondary cache
        // Update LRU queue
        // Update access count
        // Update last access time
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
        // Evict least recently used
        // Complex eviction algorithm
        // All without measuring actual need!
        primaryCache[key] = value;
    }
};

// 🚨 PROBLEM:
// - Two-level cache without proof it's needed
// - Compression without measuring data size
// - Encryption without security requirements
// - Complex LRU without measuring access patterns
// - 150+ lines for hypothetical performance
```

### ✅ **Good: Simple First, Optimize Later**

```cpp
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

// Usage:
DataCache cache;
cache.put("user:123", "John Doe");
cout << cache.get("user:123") << endl;  // John Doe

// ✅ 10 lines of code
// ✅ Works perfectly
// ✅ When performance becomes an issue:
//    1. MEASURE the bottleneck
//    2. Profile the code
//    3. THEN optimize
//    → Not before!
```

**The Right Process:**

```
Step 1: Build simple version
        ↓
Step 2: Deploy and measure
        ↓
Step 3: Is there a performance problem?
        ├─ NO → Done! Keep it simple
        └─ YES → Profile to find bottleneck
                 ↓
Step 4: Optimize the ACTUAL bottleneck
        ↓
Step 5: Measure improvement
```

**Famous Quote:**

> "Premature optimization is the root of all evil."  
> — Donald Knuth

**Translation:**
- Don't optimize before measuring
- Don't add complexity for hypothetical performance
- Optimize REAL bottlenecks, not imagined ones

---

## ⚖️ When YAGNI Doesn't Apply

YAGNI has exceptions! Sometimes planning ahead IS justified:

### ✅ **Plan Ahead For:**

**1. Public APIs**
```cpp
class PaymentAPI {
public:
    // Version number is justified
    // External users depend on this
    // Breaking changes are expensive
    virtual string getAPIVersion() const { return "1.0"; }
    virtual bool processPayment(double amount, const string& currency) = 0;
};
```

**Why:** External dependencies make changes costly

**2. Database Schemas**
```cpp
struct UserTable {
    int id;
    string username;
    string email;
    long createdAt;  // Good to have
    long updatedAt;  // Good to have
    
    // These timestamps are justified:
    // - Commonly needed
    // - Hard to add later (requires migration)
    // - Low cost to include
};
```

**Why:** Schema migrations are expensive

**3. File Formats**
```cpp
struct FileHeader {
    int version;     // Good to have
    int magic;       // Good to have
    long timestamp;  // Good to have
    // Some forward-thinking justified
};
```

**Why:** File format changes affect existing files

### ❌ **YAGNI DOES Apply To:**

**1. Internal Code**
```cpp
// Don't over-engineer internal helpers
class InternalHelper {
    // Just build what's needed
    // Easy to refactor later
};
```

**2. Features**
```cpp
// Don't build unused features
// Wait for user requests
```

**3. Abstractions**
```cpp
// Don't abstract prematurely
// Wait for pattern to emerge
// Rule of Three: Need 3 examples
```

### The Balance

```
Public API ────────────────> Plan ahead (hard to change)
Internal code ─────────────> YAGNI (easy to change)

Database schema ───────────> Some planning (migrations costly)
Feature set ───────────────> YAGNI (easy to add features)

File format ───────────────> Some versioning (compatibility)
Business logic ────────────> YAGNI (refactor when needed)
```

---

## 💰 Cost of Speculation

### What Happens When You Ignore YAGNI:

**1. Development Time Wasted**
```
Time spent building speculative features
+ Time that could have built real features
= Opportunity cost
```

**2. Maintenance Burden**
```
Dead code still needs:
• Testing
• Documentation
• Updates when dependencies change
• Understanding by new developers
= Ongoing cost forever
```

**3. Increased Complexity**
```
More code = More complexity
More complexity = More bugs
More bugs = More debugging time
= Slower development
```

**4. Wrong Solutions**
```
90% of "might need" never materializes
When it does, requirements differ
Result: Built wrong solution to wrong problem
```

**5. Delayed Real Features**
```
Time on speculation
= Less time on actual needs
= Delayed user value
```

### Statistics from Industry Studies

📊 **Standish Group Research:**
- **64%** of features are rarely or never used
- **45%** of features are NEVER used at all
- **19%** of features used rarely
- Only **36%** of features used regularly

💰 **Cost Breakdown:**
```
100 features built "just in case"
├─ 45 features: Never used (0% value, 100% cost)
├─ 19 features: Rarely used (10% value, 100% cost)
└─ 36 features: Actually used (100% value, 100% cost)

Total value delivered: 46% of development effort
Waste: 54% of development effort
```

---

## ✅ How to Practice YAGNI

### The Fundamental Question

Before writing ANY code:

```
┌────────────────────────────────────────────┐
│                                            │
│  "Do I need this RIGHT NOW?"               │
│                                            │
│  ├─ YES → Build it                        │
│  └─ NO  → Don't build it                  │
│      │                                     │
│      └─ "But what if I need it later?"    │
│          ├─ Then: Add it THEN             │
│          └─ If not needed: Time saved!    │
│                                            │
└────────────────────────────────────────────┘
```

### Challenge These Phrases

| ❌ Speculation | ✅ Reality |
|---|---|
| "We might need this someday" | "Is this needed for current requirements?" |
| "What if requirements change?" | "What are the requirements NOW?" |
| "Let's make it flexible" | "Let's make it work" |
| "For future extensibility" | "For current functionality" |
| "Just in case" | "Just in time" |

### Build Minimum Viable Solution

```
Requirements: User login system

❌ Speculation approach:
- Username/password login
- OAuth (Google, Facebook, Twitter, GitHub)
- Two-factor authentication
- Biometric login
- Single sign-on
- Remember me feature
- Session management
- Login attempt tracking
- IP-based restrictions
- → 2 months of development
- → Most features unused

✅ YAGNI approach:
- Username/password login
- → 3 days of development
- → Meets current needs
- → Add OAuth when requested
- → Add 2FA when security audit requires it
```

### The Rule of Three

Don't abstract until you have **3 concrete examples**:

```
First occurrence:  Write it
                   ↓
Second occurrence: Duplicate it (yes, really!)
                   ↓
Third occurrence:  NOW abstract it
                   (pattern is clear with 3 examples)
```

**Example:**

```cpp
// First time: Send email notification
void notifyUserByEmail(string email, string message) {
    // Send email
}

// Second time: Send SMS notification
void notifyUserBySMS(string phone, string message) {
    // Send SMS
    // 👀 Looks similar, but DON'T abstract yet!
}

// Third time: Send push notification
void notifyUserByPush(string deviceId, string message) {
    // Send push
    // ✅ NOW we have 3 examples
    // ✅ Pattern is clear
    // ✅ NOW create abstraction
}

// After third occurrence:
class NotificationService {
    virtual void send(string destination, string message) = 0;
};

class EmailNotification : public NotificationService { /* ... */ };
class SMSNotification : public NotificationService { /* ... */ };
class PushNotification : public NotificationService { /* ... */ };
```

### Trust the Refactoring Process

```
Phase 1: Build simple solution
         ↓
Phase 2: Ship and observe usage
         ↓
Phase 3: Real requirements emerge
         ↓
Phase 4: Refactor to accommodate
         ↓
Phase 5: Repeat
```

**Key Insight:** Refactoring with real requirements is **easier and cheaper** than maintaining speculative code!

---

## 📋 YAGNI Checklist

Before adding any feature or abstraction, ask:

### Required YES answers:

- [ ] Is this required for **current** functionality?
- [ ] Has someone **actually** requested this?
- [ ] Will this be used in the **next release**?
- [ ] Do I have **concrete examples** (3+) of the need?
- [ ] Would removing this break **existing** features?

### Red flags (any YES = don't build):

- [ ] Is this "just in case"?
- [ ] Is this "for future flexibility"?
- [ ] Is this "we might need someday"?
- [ ] Is this speculation about requirements?
- [ ] Am I guessing what users want?

### Decision Matrix:

```
All required questions YES + All red flags NO = Build it
Otherwise = Don't build it (YAGNI!)
```

---

## 🔗 YAGNI with Other Principles

### YAGNI + KISS

```
YAGNI: Don't build it until needed
  +
KISS:  When you build it, keep it simple
  =
Simple solutions to actual problems
```

### YAGNI + DRY

```
YAGNI: Wait for pattern to emerge (Rule of 3)
  +
DRY:   Then eliminate duplication
  =
Abstract when pattern is clear
```

**Example:**

```cpp
// First time: Don't abstract (YAGNI)
void processUserData() { /* code */ }

// Second time: Duplicate (YAGNI says wait)
void processOrderData() { /* similar code */ }

// Third time: NOW abstract (DRY)
template<typename T>
void processData(T& data) { /* abstracted code */ }
```

### YAGNI + SOLID

```
YAGNI: Build for current needs only
  +
SOLID: Design it well
  =
Well-designed code for actual requirements
```

### YAGNI + TDD

```
YAGNI: Only build what's needed
  +
TDD:   Tests define what's needed
  =
If no test requires it, don't build it!
```

---

## 🎯 Quick Reference

### Red Flags (Speculation Smells)

🚨 **Phrases that indicate YAGNI violation:**
- "Just in case"
- "Might need someday"
- "What if..."
- "Let's make it configurable"
- "For future flexibility"
- "We might want to..."
- "Eventually we'll need..."
- "This could be useful for..."

### Green Flags (Current Needs)

✅ **Phrases that indicate following YAGNI:**
- "The requirement says..."
- "Users requested..."
- "This release needs..."
- "We're using this now for..."
- "The test requires..."
- "Current functionality depends on..."

### The YAGNI Test

```
Feature proposed → Ask: "Needed NOW?"
                   ├─ YES → Build it
                   └─ NO  → Record as idea, don't build

Time passes → Requirement appears
              ├─ Was in ideas → Now build it
              └─ Wasn't needed → Saved time!
```

---

## 💡 Common Misconceptions

### ❌ Misconception 1: "YAGNI means no planning"

**Reality:** YAGNI means plan for **known** requirements, not **unknown** speculation.

```
✅ Good planning:
"User story says: Login with email/password"
→ Plan: Build email/password login

❌ Speculation:
"Users MIGHT want OAuth someday"
→ Don't plan: Wait for actual request
```

### ❌ Misconception 2: "YAGNI means poor design"

**Reality:** YAGNI means good design for **current** needs.

```
✅ YAGNI-friendly design:
- Clean code
- Well-named functions
- Proper separation of concerns
- For ACTUAL requirements

❌ Not YAGNI:
- Over-engineered
- Abstractions for hypothetical needs
- Plugin systems for simple tasks
```

### ❌ Misconception 3: "YAGNI means technical debt"

**Reality:** **Speculation** creates technical debt, not YAGNI.

```
Technical Debt Sources:

❌ Speculative code:
- Features never used
- Must maintain forever
- Increases complexity
= Real technical debt

✅ YAGNI approach:
- Only needed features
- Smaller codebase
- Easy to change
= Less technical debt
```

### ❌ Misconception 4: "YAGNI prevents refactoring"

**Reality:** YAGNI **encourages** refactoring when needed.

```
YAGNI Process:
1. Build simple solution
2. Requirements emerge
3. Refactor to accommodate ← Encouraged!
4. Pattern becomes clear
5. Abstract when needed ← Encouraged!
```

---

## 📊 Real-World Impact

### Case Study: Without YAGNI

```
Project: E-commerce platform
Timeline: 12 months

Features built:
├─ Core shopping cart (3 months) ✅ Used daily
├─ Payment processing (2 months) ✅ Used daily
├─ Wishlist system (1 month) ✅ Used weekly
├─ Advanced recommendation engine (3 months) ❌ Never launched
├─ Social sharing features (2 months) ❌ Used by <1% users
└─ Loyalty points program (1 month) ❌ Abandoned before launch

Result:
- 12 months spent
- 6 months (50%) on unused features
- 6 months delayed time-to-market
- Maintenance burden for unused code
```

### Case Study: With YAGNI

```
Project: E-commerce platform
Timeline: 6 months to MVP

Features built:
├─ Core shopping cart (3 months) ✅ Used daily
├─ Payment processing (2 months) ✅ Used daily
└─ Wishlist system (1 month) ✅ Used weekly

MVP launched in 6 months

After launch (based on real data):
├─ Month 7-8: Added order tracking (users requested)
├─ Month 9-10: Added reviews (users requested)
└─ Social sharing: Never requested, never built

Result:
- 6 months to market (6 months earlier!)
- All features actually used
- No wasted development
- Features guided by real user needs
```

---

## 🎓 Best Practices

### 1. Start with Minimum

```cpp
// ✅ Start here:
class Product {
    string name;
    double price;
};

// ❌ Don't start here:
class Product {
    string name;
    double price;
    string description;
    vector<string> images;
    vector<Review> reviews;
    vector<Tag> tags;
    Category category;
    Brand brand;
    Dimensions dimensions;
    Weight weight;
    // ... 20 more fields
};
```

### 2. Let Usage Guide Design

```
Week 1: Build basic Product class
        ↓
Week 2: Users need descriptions
        → Add description field
        ↓
Week 3: Users need images
        → Add images vector
        ↓
Week 4: Users need reviews
        → Add reviews vector

✅ Each addition driven by REAL need
```

### 3. Embrace Refactoring

```cpp
// Version 1: Simple
void sendNotification(string email, string message) {
    // Send email
}

// Version 2: Real need for SMS emerged
void sendEmailNotification(string email, string message) { /* ... */ }
void sendSMSNotification(string phone, string message) { /* ... */ }

// Version 3: Real need for push emerged (3rd example!)
class NotificationService {
    virtual void send(string destination, string message) = 0;
};
// Now abstraction is justified!
```

### 4. Delete Speculative Code

```cpp
// During code review, found this:
class User {
    // ... actual fields
    string futureField1;  // "Might need for phase 2"
    string futureField2;  // "Planning ahead"
    int futureField3;     // "Just in case"
};

// ✅ Delete them!
// ✅ Add when ACTUALLY needed
// ✅ Don't carry dead weight
```

---

## 🎯 Summary

### Core Principle

```
╔══════════════