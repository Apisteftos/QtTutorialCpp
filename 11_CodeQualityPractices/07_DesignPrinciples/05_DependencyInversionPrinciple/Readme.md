# Dependency Inversion Principle (DIP)

## 📚 Overview

The **Dependency Inversion Principle** states:

> **"High-level modules should not depend on low-level modules. Both should depend on abstractions."**  
> **"Abstractions should not depend on details. Details should depend on abstractions."**  
> — Robert C. Martin

### What It Means

- **Depend on interfaces**, not concrete classes
- **High-level policy** should not depend on low-level details
- **Both layers** depend on abstract interfaces
- **Invert the dependency** direction upward

### Why It Matters

✅ **Flexible architecture** - Easy to swap implementations  
✅ **Testable code** - Can inject mock objects  
✅ **Maintainable** - Changes localized to implementations  
✅ **Reduced coupling** - Layers independent  
✅ **Reusable components** - High-level code reusable  

---

## 🚨 Violation Signs

How to spot DIP violations:

- 🚨 `new` keyword everywhere in high-level code
- 🚨 Concrete class types in method parameters
- 🚨 High-level modules importing low-level modules
- 🚨 Difficult to test (can't mock dependencies)
- 🚨 Changes ripple through entire system
- 🚨 `if/else` chains for different implementations

---

## 🔄 The Inversion

### Traditional Dependency (❌ Bad)

```
High-Level Module
       ↓ depends on
Low-Level Module
```

**Problem:** High-level code depends directly on low-level details

### Inverted Dependency (✅ Good)

```
High-Level Module
       ↓ depends on
   Abstraction (Interface)
       ↑ implements
Low-Level Module
```

**Solution:** Both layers depend on abstraction layer

---

## 📖 Classic Example 1: Database Connection

### ❌ **Bad: High-Level Depends on Concrete Implementation**

```cpp
// Low-level module (implementation detail)
class MySQLDatabase {
public:
    void connect() { /* MySQL connection */ }
    void query(const string& sql) { /* MySQL query */ }
    void disconnect() { /* MySQL disconnect */ }
};

// 🚨 VIOLATION: High-level depends on concrete MySQLDatabase
class UserRepository {
private:
    MySQLDatabase database;  // 🚨 Tight coupling!
    
public:
    void getUser(int id) {
        database.connect();
        database.query("SELECT * FROM users WHERE id = " + to_string(id));
        database.disconnect();
    }
};
```

**Problems:**
- UserRepository tightly coupled to MySQL
- Can't switch to PostgreSQL without changing UserRepository
- Can't test UserRepository without real database
- High-level business logic depends on low-level implementation details
- Violates Open/Closed Principle

### ✅ **Good: Both Depend on Abstraction**

```cpp
// Abstraction layer (interface)
class IDatabase {
public:
    virtual ~IDatabase() = default;
    virtual void connect() = 0;
    virtual void query(const string& sql) = 0;
    virtual void disconnect() = 0;
};

// Low-level implementations
class MySQLDatabase : public IDatabase {
public:
    void connect() override { /* MySQL connection */ }
    void query(const string& sql) override { /* MySQL query */ }
    void disconnect() override { /* MySQL disconnect */ }
};

class PostgreSQLDatabase : public IDatabase {
public:
    void connect() override { /* PostgreSQL connection */ }
    void query(const string& sql) override { /* PostgreSQL query */ }
    void disconnect() override { /* PostgreSQL disconnect */ }
};

class MongoDatabase : public IDatabase {
public:
    void connect() override { /* Mongo connection */ }
    void query(const string& sql) override { /* Mongo query */ }
    void disconnect() override { /* Mongo disconnect */ }
};

// High-level module depends on abstraction
class UserRepository {
private:
    IDatabase* database;  // ✅ Depends on interface!
    
public:
    // Dependency Injection via constructor
    UserRepository(IDatabase* db) : database(db) {}
    
    void getUser(int id) {
        database->connect();
        database->query("SELECT * FROM users WHERE id = " + to_string(id));
        database->disconnect();
    }
};

// Usage:
MySQLDatabase mysql;
PostgreSQLDatabase postgres;
MongoDatabase mongo;

UserRepository repo1(&mysql);      // Use MySQL
UserRepository repo2(&postgres);   // Use PostgreSQL
UserRepository repo3(&mongo);      // Use MongoDB
```

**Solution:**
- UserRepository depends on `IDatabase` interface, not concrete class
- Can easily switch database implementations
- Easy to test with mock database
- High-level and low-level both depend on abstraction
- Follows Open/Closed Principle

---

## 📧 Classic Example 2: Notification System

### ❌ **Bad: Depends on Concrete Notification Classes**

```cpp
class EmailService {
public:
    void sendEmail(const string& msg) { /* send email */ }
};

class SMSService {
public:
    void sendSMS(const string& msg) { /* send SMS */ }
};

// 🚨 VIOLATION: Depends on concrete implementations
class NotificationService {
private:
    EmailService emailService;  // 🚨 Tight coupling!
    SMSService smsService;      // 🚨 Tight coupling!
    
public:
    void notify(const string& message, bool useEmail) {
        if (useEmail) {
            emailService.sendEmail(message);
        } else {
            smsService.sendSMS(message);
        }
    }
};

// To add push notifications:
// 1. Add PushService member variable
// 2. Modify notify() method
// 3. Recompile and test everything
```

### ✅ **Good: Depends on Abstraction**

```cpp
// Abstraction
class INotifier {
public:
    virtual ~INotifier() = default;
    virtual void send(const string& message) = 0;
};

// Implementations
class EmailNotifier : public INotifier {
public:
    void send(const string& message) override {
        cout << "📧 Email: " << message << endl;
    }
};

class SMSNotifier : public INotifier {
public:
    void send(const string& message) override {
        cout << "📱 SMS: " << message << endl;
    }
};

class PushNotifier : public INotifier {
public:
    void send(const string& message) override {
        cout << "🔔 Push: " << message << endl;
    }
};

// High-level depends on abstraction
class NotificationService {
private:
    vector<INotifier*> notifiers;  // ✅ Depends on interface!
    
public:
    void addNotifier(INotifier* notifier) {
        notifiers.push_back(notifier);
    }
    
    void notify(const string& message) {
        for (auto notifier : notifiers) {
            notifier->send(message);
        }
    }
};

// Usage:
EmailNotifier email;
SMSNotifier sms;
PushNotifier push;

NotificationService service;
service.addNotifier(&email);
service.addNotifier(&sms);
service.addNotifier(&push);
service.notify("Hello!");  // Sends via all three!
```

**Solution:**
- NotificationService never changes when adding new notifiers
- Can add SlackNotifier, WebhookNotifier, etc. without modifying existing code
- Open/Closed Principle achieved

---

## 💳 Classic Example 3: Payment Processing

### ❌ **Bad: Concrete Payment Gateways**

```cpp
class PayPalGateway {
public:
    void processPayment(double amount) { /* PayPal */ }
};

class StripeGateway {
public:
    void processPayment(double amount) { /* Stripe */ }
};

// 🚨 VIOLATION
class PaymentProcessor {
private:
    PayPalGateway paypal;  // 🚨
    StripeGateway stripe;  // 🚨
    
public:
    void process(double amount, const string& gateway) {
        if (gateway == "paypal") {
            paypal.processPayment(amount);
        } else if (gateway == "stripe") {
            stripe.processPayment(amount);
        }
        // Need to modify for each new gateway!
    }
};
```

### ✅ **Good: Abstracted Payment Gateway**

```cpp
class IPaymentGateway {
public:
    virtual ~IPaymentGateway() = default;
    virtual void processPayment(double amount) = 0;
};

class PayPalGateway : public IPaymentGateway {
    void processPayment(double amount) override { /* PayPal */ }
};

class StripeGateway : public IPaymentGateway {
    void processPayment(double amount) override { /* Stripe */ }
};

class SquareGateway : public IPaymentGateway {
    void processPayment(double amount) override { /* Square */ }
};

class PaymentProcessor {
private:
    IPaymentGateway* gateway;  // ✅
    
public:
    PaymentProcessor(IPaymentGateway* gw) : gateway(gw) {}
    
    void process(double amount) {
        gateway->processPayment(amount);
    }
    
    void setGateway(IPaymentGateway* gw) {
        gateway = gw;
    }
};

// Usage:
PayPalGateway paypal;
StripeGateway stripe;

PaymentProcessor processor(&paypal);
processor.process(100.00);

processor.setGateway(&stripe);  // Runtime switching!
processor.process(250.00);
```

---

## 📝 Classic Example 4: Logging System

### ❌ **Bad: Hardcoded Logger**

```cpp
class FileLogger {
public:
    void log(const string& message) { /* log to file */ }
};

// 🚨 Business logic depends on concrete logger
class OrderService {
private:
    FileLogger logger;  // 🚨 Tight coupling!
    
public:
    void placeOrder(const string& product) {
        logger.log("Order placed: " + product);
        // ... order logic
    }
};
```

### ✅ **Good: Abstracted Logger**

```cpp
class ILogger {
public:
    virtual ~ILogger() = default;
    virtual void log(const string& message) = 0;
};

class FileLogger : public ILogger {
    void log(const string& message) override { /* file */ }
};

class ConsoleLogger : public ILogger {
    void log(const string& message) override { /* console */ }
};

class DatabaseLogger : public ILogger {
    void log(const string& message) override { /* database */ }
};

class CloudLogger : public ILogger {
    void log(const string& message) override { /* cloud */ }
};

class OrderService {
private:
    ILogger* logger;  // ✅ Depends on interface!
    
public:
    OrderService(ILogger* log) : logger(log) {}
    
    void placeOrder(const string& product) {
        logger->log("Order placed: " + product);
        // ... order logic
    }
};

// Usage:
FileLogger fileLog;
ConsoleLogger consoleLog;
CloudLogger cloudLog;

OrderService service1(&fileLog);     // Log to file
OrderService service2(&consoleLog);  // Log to console
OrderService service3(&cloudLog);    // Log to cloud
```

---

## 💉 Dependency Injection Patterns

### 1. **Constructor Injection** (Recommended)

```cpp
class UserService {
private:
    IDatabase* database;
    
public:
    // Dependency injected via constructor
    UserService(IDatabase* db) : database(db) {}
    
    void saveUser(const string& name) {
        database->save(name);
    }
};

// Usage:
MySQLDatabase mysql;
UserService service(&mysql);  // Inject at construction
```

**Pros:**
- ✅ Dependencies required at construction
- ✅ Immutable after creation
- ✅ Clear requirements
- ✅ Compiler-enforced

**Cons:**
- ⚠️ Can't change dependencies after construction

### 2. **Setter Injection**

```cpp
class UserService {
private:
    IDatabase* database = nullptr;
    
public:
    // Dependency injected via setter
    void setDatabase(IDatabase* db) {
        database = db;
    }
    
    void saveUser(const string& name) {
        if (database) {
            database->save(name);
        }
    }
};

// Usage:
UserService service;
MySQLDatabase mysql;
service.setDatabase(&mysql);  // Inject after construction
```

**Pros:**
- ✅ Optional dependencies
- ✅ Can change after construction
- ✅ More flexible

**Cons:**
- ⚠️ Less clear requirements
- ⚠️ Null checks needed

### 3. **Method Injection**

```cpp
class UserService {
public:
    // Dependency injected per method call
    void saveUser(const string& name, IDatabase* database) {
        database->save(name);
    }
};

// Usage:
UserService service;
MySQLDatabase mysql;
PostgreSQLDatabase postgres;

service.saveUser("Alice", &mysql);      // Use MySQL
service.saveUser("Bob", &postgres);     // Use PostgreSQL
```

**Pros:**
- ✅ Different dependency per call
- ✅ No state in class

**Cons:**
- ⚠️ Can be verbose
- ⚠️ Less encapsulation

---

## ✅ How to Follow DIP

### 1. **Identify High-Level vs Low-Level**

```cpp
// High-Level (Business Logic, Policy)
// - What the system should do
// - Business rules
// - Use cases

class OrderService { };      // High-level
class PaymentProcessor { };  // High-level
class UserManager { };       // High-level

// Low-Level (Implementation Details)
// - How to do it
// - I/O operations
// - External systems

class MySQLDatabase { };     // Low-level
class FileSystem { };        // Low-level
class NetworkClient { };     // Low-level
```

### 2. **Create Abstractions for Dependencies**

```cpp
// Instead of:
class OrderService {
    MySQLDatabase db;  // ❌ Concrete dependency
};

// Do this:
class IDatabase { virtual void save() = 0; };

class OrderService {
    IDatabase* db;     // ✅ Abstract dependency
};
```

### 3. **Inject Dependencies from Outside**

```cpp
// ❌ DON'T create dependencies internally
class OrderService {
    IDatabase* db;
public:
    OrderService() {
        db = new MySQLDatabase();  // ❌ Creating concrete class!
    }
};

// ✅ DO inject from outside
class OrderService {
    IDatabase* db;
public:
    OrderService(IDatabase* database) : db(database) {
        // ✅ Dependency provided externally
    }
};
```

### 4. **Follow the Dependency Rule**

```
┌─────────────────────┐
│   High-Level        │
│   (Business Logic)  │
└──────────┬──────────┘
           │ depends on
           ↓
┌──────────────────────┐
│    Abstraction       │
│    (Interface)       │
└──────────┬───────────┘
           ↑ implements
           │
┌──────────┴──────────┐
│   Low-Level         │
│   (Implementation)  │
└─────────────────────┘
```

**Rule:** Source code dependencies must point inward (toward abstractions)

### 5. **Avoid Anti-Patterns**

```cpp
// ❌ DON'T DO THIS:

// Creating concrete objects in high-level code
MySQLDatabase* db = new MySQLDatabase();  // ❌

// Concrete types in method parameters
void process(MySQLDatabase* db) { }       // ❌

// if/else chains for implementations
if (type == "mysql") { /* ... */ }
else if (type == "postgres") { /* ... */ } // ❌

// Importing low-level modules in high-level
#include "MySQLDatabase.h"                 // ❌


// ✅ DO THIS:

// Depend on interfaces
IDatabase* db;                             // ✅

// Interface types in parameters
void process(IDatabase* db) { }            // ✅

// Polymorphism instead of conditionals
db->query();                               // ✅

// Import only interfaces
#include "IDatabase.h"                     // ✅
```

---

## 🎯 DIP Checklist

Before writing code, ask:

- [ ] Are high-level modules depending on abstractions?
- [ ] Are low-level modules implementing abstractions?
- [ ] Are dependencies injected from outside?
- [ ] Is there `new` keyword in high-level code?
- [ ] Can implementations be easily swapped?
- [ ] Can the code be tested with mocks?
- [ ] Are there concrete class types in method parameters?
- [ ] Do changes in low-level affect high-level?

---

## ⚠️ Common Violations

### 1. Creating Dependencies Internally

```cpp
// ❌ BAD
class OrderService {
    IDatabase* db;
public:
    OrderService() {
        db = new MySQLDatabase();  // ❌ Creating concrete!
    }
};

// ✅ GOOD
class OrderService {
    IDatabase* db;
public:
    OrderService(IDatabase* database) : db(database) {
        // ✅ Injected from outside
    }
};
```

### 2. Concrete Types in Parameters

```cpp
// ❌ BAD
void processOrder(MySQLDatabase* db, StripeGateway* payment) {
    // ❌ Depends on concrete types
}

// ✅ GOOD
void processOrder(IDatabase* db, IPaymentGateway* payment) {
    // ✅ Depends on abstractions
}
```

### 3. Type Checking for Implementation

```cpp
// ❌ BAD
void process(IDatabase* db) {
    if (dynamic_cast<MySQLDatabase*>(db)) {
        // Special MySQL handling
    } else if (dynamic_cast<PostgreSQLDatabase*>(db)) {
        // Special PostgreSQL handling
    }
}

// ✅ GOOD
void process(IDatabase* db) {
    db->query();  // Polymorphic call
}
```

---

## ✅ Benefits

### 1. Flexibility

```cpp
// Easy to swap implementations
MySQLDatabase mysql;
PostgreSQLDatabase postgres;

OrderService service1(&mysql);      // Use MySQL today
OrderService service2(&postgres);   // Use PostgreSQL tomorrow
```

### 2. Testability

```cpp
// Mock implementation for testing
class MockDatabase : public IDatabase {
    void query(const string& sql) override {
        // Track calls for testing
    }
};

MockDatabase mock;
OrderService service(&mock);  // Test without real database!
```

### 3. Maintainability

```cpp
// Changes localized to implementations
// OrderService never changes when:
// - Adding new database
// - Changing database implementation
// - Fixing database bugs
```

### 4. Reusability

```cpp
// OrderService can be reused with any database
OrderService withMySQL(&mysql);
OrderService withPostgres(&postgres);
OrderService withMongo(&mongo);
```

### 5. Parallel Development

```cpp
// Team A: Works on interface
class IDatabase { virtual void query() = 0; };

// Team B: Implements high-level (using interface)
class OrderService {
    IDatabase* db;  // Uses interface
};

// Team C: Implements low-level (interface already defined)
class MySQLDatabase : public IDatabase {
    void query() override { /* implement */ }
};
```

---

## 💡 Key Insights

### Stable Dependencies

**Depend on stable abstractions, not volatile implementations:**

```
Stable:   Interfaces, abstract classes
          → Don't change often
          → Safe to depend on

Volatile: Concrete implementations
          → Change frequently
          → Dangerous to depend on
```

### Ownership of Interfaces

**High-level module owns the interface:**

```cpp
// High-level package
class IDatabase { };           // Interface owned by high-level
class OrderService {           // High-level business logic
    IDatabase* db;
};

// Low-level package
class MySQLDatabase : public IDatabase {  // Implements interface
    // Low-level implementation
};
```

**Why?** The interface should be designed for the high-level module's needs, not the low-level module's capabilities.

### Inversion of Control (IoC)

**Control flow vs Dependency flow:**

```
Control Flow:
    Main → High-Level → Low-Level
    (Runtime execution order)

Dependency Flow (DIP):
    Main → Interface ← Low-Level
           ↑
       High-Level
    (Compile-time dependencies point inward)
```

---

## 🎯 Quick Reference

### Red Flags (DIP Violations)

- ❌ `new ConcreteClass()` in high-level code
- ❌ Concrete types in method parameters
- ❌ High-level importing low-level modules
- ❌ Hard to test (can't inject mocks)
- ❌ if/else chains for implementations
- ❌ Changes ripple through system

### Green Flags (DIP Compliance)

- ✅ Dependencies are interfaces/abstractions
- ✅ Dependencies injected from outside
- ✅ Easy to swap implementations
- ✅ Testable with mock objects
- ✅ High-level stable when low-level changes
- ✅ Clear separation of concerns

---

## 🔄 DIP and Other Principles

### DIP + SRP (Single Responsibility)

```cpp
// Each class has one reason to change
class OrderService { /* business logic only */ };
class MySQLDatabase { /* database only */ };
```

### DIP + OCP (Open/Closed)

```cpp
// Open for extension (new implementations)
// Closed for modification (high-level unchanged)
class OrderService { IDatabase* db; };  // Never changes
class NewDatabase : public IDatabase { };  // Add new
```

### DIP + LSP (Liskov Substitution)

```cpp
// All implementations must be substitutable
void process(IDatabase* db) {
    db->query();  // Works for all IDatabase implementations
}
```

### DIP + ISP (Interface Segregation)

```cpp
// Interfaces are small and focused
class IReadable { virtual string read() = 0; };
class IWritable { virtual void write() = 0; };
```

---

## 💭 Quotes

> "High-level modules should not depend on low-level modules. Both should depend on abstractions."  
> — Robert C. Martin

> "Abstractions should not depend on details. Details should depend on abstractions."  
> — Robert C. Martin

> "The most flexible systems are those in which source code dependencies refer only to abstractions, not to concretions."  
> — Clean Architecture

> "Depend upon abstractions, not concretions."  
> — Agile Software Development

---

## 🚀 Next Steps

After mastering DIP, explore:
- **Dependency Injection Frameworks** - Automated DI
- **Inversion of Control (IoC) Containers** - Managing dependencies
- **Clean Architecture** - Layered architecture with DIP
- **Hexagonal Architecture** - Ports and adapters pattern
- **SOLID Principles Review** - How all 5 work together

---

## 📚 Resources

- **"Clean Architecture" by Robert C. Martin** - Architecture with DIP
- **"Agile Software Development" by Robert C. Martin** - Original SOLID
- **"Dependency Injection in .NET" by Mark Seemann** - DI patterns
- **"Design Patterns" by Gang of Four** - Strategy, Factory patterns

---

## 🎓 Key Takeaways

1. **Invert dependencies** - High-level doesn't depend on low-level
2. **Depend on abstractions** - Interfaces, not concrete classes
3. **Inject dependencies** - Provide from outside, don't create inside
4. **Stable vs Volatile** - Depend on stable abstractions
5. **Ownership** - High-level owns interfaces
6. **Testability** - Can inject mock objects
7. **Flexibility** - Easy to swap implementations
8. **Maintainability** - Changes localized

---

## 📝 Final Wisdom

> "When you follow the Dependency Inversion Principle, your code becomes flexible and testable.  
> High-level policy no longer depends on low-level details.  
> Both depend on abstractions, creating a stable architecture."

The Dependency Inversion Principle is the culmination of SOLID. When in doubt, ask: **"Am I depending on an abstraction or a concrete implementation?"**

---

## 🔍 Real-World Analogy

Think of **Electrical Outlets**:

**Without DIP** (Direct wiring):
- Each appliance wired directly to power source
- Want new appliance? Rewire entire house!
- Different voltage? Rebuild everything!
- Can't test appliance without dangerous electrical work

**With DIP** (Standardized outlets):
- Appliances depend on outlet interface (abstraction)
- Power source implements outlet interface
- Plug and play any appliance!
- Easy to test with test fixtures
- Change power source without affecting appliances

**DIP says: Use standard interfaces (outlets), not direct wiring!**

---

## 🎯 The Complete SOLID Journey

```
SRP: One reason to change
OCP: Open for extension, closed for modification
LSP: Subclasses must be substitutable
ISP: Many small interfaces, not one large
DIP: Depend on abstractions, not concretions

Together, they create:
✅ Flexible architecture
✅ Maintainable code
✅ Testable systems
✅ Reusable components
✅ Robust applications
```

**Congratulations on completing all SOLID principles!** 🎉