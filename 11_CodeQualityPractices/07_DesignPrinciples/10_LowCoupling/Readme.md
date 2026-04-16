# Low Coupling Principle

## 📚 Overview

The **Low Coupling** principle states:

> **"Minimize dependencies between modules"**  
> **"Each module should know as little as possible about others"**  
> — Based on principles by Larry Constantine & Edward Yourdon

### What It Means

- **Coupling** = Degree of interdependence between modules
- **Low coupling** = Modules are independent
- **High coupling** = Modules are tightly connected
- **Goal**: Modules can change independently without affecting each other

### Why It Matters

✅ **Easier to change** - Changes don't ripple through the system  
✅ **Easier to test** - Fewer dependencies to mock  
✅ **Better reusability** - Modules work independently  
✅ **Easier to understand** - Less to think about at once  
✅ **Parallel development** - Teams work independently  
✅ **Better maintainability** - Bugs isolated to modules  

---

## 🎯 Understanding Coupling

### The Coupling Spectrum

```
TIGHT COUPLING ←――――――――――――――――――――→ LOOSE COUPLING
    (Bad)                                  (Good)

• Hard to change              • Easy to change
• Hard to test                • Easy to test
• Low reusability             • High reusability
• High maintenance cost       • Low maintenance cost
```

### Types of Coupling (Worst to Best)

1. **Content Coupling** (Worst) - Module modifies another's internal data
2. **Common Coupling** - Modules share global data
3. **Control Coupling** - One module controls another's flow
4. **Stamp Coupling** - Modules share complex data structures
5. **Data Coupling** - Modules share simple data
6. **Message Coupling** (Best) - Modules communicate via messages/interfaces

---

## 🚨 Violation Signs

How to spot high coupling:

- 🚨 Changes in one class require changes in many others
- 🚨 Hard to test because of many dependencies
- 🚨 Can't reuse class without dragging along others
- 🚨 Concrete classes used everywhere (not abstractions)
- 🚨 Global variables shared between modules
- 🚨 Long dependency chains: `obj.getX().getY().getZ()`
- 🚨 Can't swap implementations
- 🚨 Circular dependencies

---

## 📖 Classic Example 1: Tight vs Loose Coupling

### ❌ **Bad: Tightly Coupled to Concrete Class**

```cpp
class MySQLDatabase {
public:
    void connect() {
        cout << "MySQL: Connected" << endl;
    }
    
    void query(const string& sql) {
        cout << "MySQL: Executing " << sql << endl;
    }
};

class UserService {
private:
    MySQLDatabase database;  // 🚨 Tightly coupled to MySQL!
    
public:
    void saveUser(const string& username) {
        database.connect();
        database.query("INSERT INTO users VALUES ('" + username + "')");
    }
};
```

**Problems:**
- UserService **MUST** use MySQL (can't switch databases)
- Can't test UserService without real MySQL database
- If MySQLDatabase class changes, UserService must change
- Can't reuse UserService with different database
- High coupling = High maintenance cost

### ✅ **Good: Loosely Coupled Through Interface**

```cpp
// Interface (abstraction)
class Database {
public:
    virtual ~Database() = default;
    virtual void connect() = 0;
    virtual void query(const string& sql) = 0;
};

// Concrete implementations
class MySQLDatabase : public Database {
public:
    void connect() override {
        cout << "MySQL: Connected" << endl;
    }
    
    void query(const string& sql) override {
        cout << "MySQL: Executing " << sql << endl;
    }
};

class PostgreSQLDatabase : public Database {
public:
    void connect() override {
        cout << "PostgreSQL: Connected" << endl;
    }
    
    void query(const string& sql) override {
        cout << "PostgreSQL: Executing " << sql << endl;
    }
};

// Service depends on interface, not concrete class
class UserService {
private:
    Database& database;  // ✅ Depends on abstraction!
    
public:
    UserService(Database& db) : database(db) {}
    
    void saveUser(const string& username) {
        database.connect();
        database.query("INSERT INTO users VALUES ('" + username + "')");
    }
};

// Usage:
MySQLDatabase mysql;
UserService service1(mysql);  // Use MySQL

PostgreSQLDatabase postgres;
UserService service2(postgres);  // Use PostgreSQL
```

**Benefits:**
- Can switch databases easily (MySQL, PostgreSQL, etc.)
- Can test with mock database
- UserService doesn't know about concrete database
- Changes in database don't affect service
- High reusability
- Low coupling through abstraction

---

## 📖 Classic Example 2: Dependency Injection

### ❌ **Bad: Creating Dependencies Internally**

```cpp
class EmailService {
public:
    void sendEmail(const string& to, const string& message) {
        cout << "Email sent" << endl;
    }
};

class Logger {
public:
    void log(const string& message) {
        cout << "[LOG] " << message << endl;
    }
};

class OrderService {
private:
    EmailService emailService;  // 🚨 Created internally!
    Logger logger;               // 🚨 Created internally!
    
public:
    void placeOrder(int orderId, const string& customerEmail) {
        logger.log("Placing order");
        
        // Order processing logic
        
        emailService.sendEmail(customerEmail, "Order confirmed");
        logger.log("Order placed");
    }
};
```

**Problems:**
- OrderService creates its own dependencies
- Can't test with mock EmailService or Logger
- Can't use different implementations
- Hard to change EmailService or Logger
- OrderService controls everything (tight coupling)

### ✅ **Good: Dependencies Injected from Outside**

```cpp
// Interfaces
class EmailService {
public:
    virtual ~EmailService() = default;
    virtual void sendEmail(const string& to, const string& message) = 0;
};

class Logger {
public:
    virtual ~Logger() = default;
    virtual void log(const string& message) = 0;
};

// Implementations
class SMTPEmailService : public EmailService {
public:
    void sendEmail(const string& to, const string& message) override {
        cout << "[SMTP] Email sent" << endl;
    }
};

class ConsoleLogger : public Logger {
public:
    void log(const string& message) override {
        cout << "[LOG] " << message << endl;
    }
};

class MockEmailService : public EmailService {
public:
    void sendEmail(const string& to, const string& message) override {
        cout << "[MOCK] Would send email" << endl;
    }
};

// Service with injected dependencies
class OrderService {
private:
    EmailService& emailService;  // ✅ Injected!
    Logger& logger;               // ✅ Injected!
    
public:
    OrderService(EmailService& email, Logger& log)
        : emailService(email), logger(log) {}
    
    void placeOrder(int orderId, const string& customerEmail) {
        logger.log("Placing order");
        emailService.sendEmail(customerEmail, "Order confirmed");
        logger.log("Order placed");
    }
};

// Production usage:
SMTPEmailService emailService;
ConsoleLogger logger;
OrderService service(emailService, logger);

// Testing usage:
MockEmailService mockEmail;
ConsoleLogger testLogger;
OrderService testService(mockEmail, testLogger);
```

**Benefits:**
- Can inject different implementations
- Easy to test with mocks
- OrderService doesn't control dependencies
- Loose coupling through interfaces
- Flexible and maintainable

---

## 📖 Classic Example 3: Global State Coupling

### ❌ **Bad: Coupled Through Global State**

```cpp
// Global variable! 🚨
int globalCounter = 0;

class OrderProcessor {
public:
    void processOrder() {
        globalCounter++;  // 🚨 Depends on global state!
        cout << "Order processed. Total: " << globalCounter << endl;
    }
};

class ReportGenerator {
public:
    void generateReport() {
        // 🚨 Also depends on global state!
        cout << "Total orders: " << globalCounter << endl;
    }
};
```

**Problems:**
- Hidden dependency on global state
- Can't test independently (shared state)
- Race conditions in multi-threaded code
- Hard to reason about program state
- Multiple modules coupled through global variable

### ✅ **Good: Explicit Dependencies, No Global State**

```cpp
class OrderCounter {
private:
    int count = 0;
    
public:
    void increment() {
        count++;
    }
    
    int getCount() const {
        return count;
    }
};

class OrderProcessor {
private:
    OrderCounter& counter;  // ✅ Explicit dependency!
    
public:
    OrderProcessor(OrderCounter& c) : counter(c) {}
    
    void processOrder() {
        counter.increment();
        cout << "Order processed. Total: " << counter.getCount() << endl;
    }
};

class ReportGenerator {
private:
    OrderCounter& counter;  // ✅ Explicit dependency!
    
public:
    ReportGenerator(OrderCounter& c) : counter(c) {}
    
    void generateReport() {
        cout << "Total orders: " << counter.getCount() << endl;
    }
};

// Usage:
OrderCounter counter;
OrderProcessor processor(counter);
ReportGenerator reporter(counter);
```

**Benefits:**
- Explicit dependencies (no hidden coupling)
- Can test with different counter instances
- Thread-safe (no shared global state)
- Easy to reason about
- Proper encapsulation

---

## 📖 Law of Demeter (Principle of Least Knowledge)

### The Rule

> **"Only talk to your immediate friends"**

A method should only call methods of:
1. Itself
2. Its parameters
3. Objects it creates
4. Its direct fields

### ❌ **Bad: Violating Law of Demeter**

```cpp
class Address {
public:
    string country;
};

class Customer {
public:
    Address address;
};

class Order {
public:
    Customer customer;
};

class ShippingService {
public:
    void shipOrder(Order& order) {
        // 🚨 Reaching through multiple objects!
        string country = order.customer.address.country;
        
        if (country == "USA") {
            cout << "Shipping to USA" << endl;
        }
    }
};
```

**Problems:**
- ShippingService knows internal structure of Order, Customer, AND Address
- Coupled to three classes
- Changes in any class affect ShippingService
- Violates "only talk to immediate friends"

### ✅ **Good: Following Law of Demeter**

```cpp
class Address {
private:
    string country;
    
public:
    Address(const string& c) : country(c) {}
    bool isDomestic() const { return country == "USA"; }
};

class Customer {
private:
    Address address;
    
public:
    Customer(const Address& addr) : address(addr) {}
    bool hasDomesticAddress() const {
        return address.isDomestic();
    }
};

class Order {
private:
    Customer customer;
    
public:
    Order(const Customer& cust) : customer(cust) {}
    bool isDomesticOrder() const {
        return customer.hasDomesticAddress();
    }
};

class ShippingService {
public:
    void shipOrder(const Order& order) {
        // ✅ Only talks to immediate friend (Order)
        if (order.isDomesticOrder()) {
            cout << "Shipping to USA" << endl;
        }
    }
};
```

**Benefits:**
- ShippingService only coupled to Order
- Changes in Customer/Address don't affect ShippingService
- Proper encapsulation
- Lower coupling

**Remember:**
```
❌ Don't: object.getX().getY().getZ()
✅ Do:    object.doSomething()
```

---

## 📖 Event-Driven Loose Coupling

### ❌ **Bad: Direct Dependencies**

```cpp
class OrderService {
private:
    InventorySystem inventory;       // 🚨 Direct dependency!
    ShippingSystem shipping;          // 🚨 Direct dependency!
    NotificationSystem notifications;  // 🚨 Direct dependency!
    
public:
    void placeOrder(int orderId) {
        // OrderService must know about all systems!
        inventory.updateInventory();
        shipping.scheduleShipment();
        notifications.sendNotification();
    }
};
```

**Problems:**
- OrderService coupled to 3 systems
- Adding new system requires changing OrderService
- Can't test OrderService independently
- High coupling, low flexibility

### ✅ **Good: Event-Driven Architecture**

```cpp
// Event structure
struct OrderPlacedEvent {
    int orderId;
    int productId;
    int quantity;
};

// Observer interface
class OrderEventListener {
public:
    virtual ~OrderEventListener() = default;
    virtual void onOrderPlaced(const OrderPlacedEvent& event) = 0;
};

// Concrete listeners
class InventorySystem : public OrderEventListener {
public:
    void onOrderPlaced(const OrderPlacedEvent& event) override {
        cout << "[INVENTORY] Updating..." << endl;
    }
};

class ShippingSystem : public OrderEventListener {
public:
    void onOrderPlaced(const OrderPlacedEvent& event) override {
        cout << "[SHIPPING] Scheduling..." << endl;
    }
};

class NotificationSystem : public OrderEventListener {
public:
    void onOrderPlaced(const OrderPlacedEvent& event) override {
        cout << "[NOTIFICATION] Sending..." << endl;
    }
};

// Service publishes events
class OrderService {
private:
    vector<OrderEventListener*> listeners;
    
public:
    void addListener(OrderEventListener* listener) {
        listeners.push_back(listener);
    }
    
    void placeOrder(int orderId, int productId, int quantity) {
        cout << "Order placed" << endl;
        
        // ✅ Notify listeners without knowing who they are!
        OrderPlacedEvent event{orderId, productId, quantity};
        for (auto listener : listeners) {
            listener->onOrderPlaced(event);
        }
    }
};

// Usage:
OrderService service;
InventorySystem inventory;
ShippingSystem shipping;
NotificationSystem notifications;

service.addListener(&inventory);
service.addListener(&shipping);
service.addListener(&notifications);

service.placeOrder(123, 456, 2);
```

**Benefits:**
- OrderService doesn't know about specific systems
- Can add new listeners without changing OrderService
- Can remove listeners easily
- Very loose coupling
- High flexibility

---

## 🏛️ Dependency Inversion Principle

### The Principle

1. **High-level modules should not depend on low-level modules**  
   Both should depend on abstractions

2. **Abstractions should not depend on details**  
   Details should depend on abstractions

### ❌ **Bad: High-Level Depends on Low-Level**

```cpp
class MySQLDatabase {
public:
    void saveData(const string& data) {
        cout << "MySQL: Saving" << endl;
    }
};

// High-level business logic
class ReportGenerator {
private:
    MySQLDatabase database;  // 🚨 Depends on concrete class!
    
public:
    void generateReport() {
        // Generate report
        database.saveData("report data");
    }
};
```

**Problems:**
- High-level module (ReportGenerator) depends on low-level (MySQL)
- Can't switch databases
- Violates Dependency Inversion Principle

### ✅ **Good: Both Depend on Abstraction**

```cpp
// Abstraction
class DatabaseInterface {
public:
    virtual ~DatabaseInterface() = default;
    virtual void saveData(const string& data) = 0;
};

// Low-level modules implement abstraction
class MySQLDatabase : public DatabaseInterface {
public:
    void saveData(const string& data) override {
        cout << "MySQL: Saving" << endl;
    }
};

class MongoDatabase : public DatabaseInterface {
public:
    void saveData(const string& data) override {
        cout << "MongoDB: Saving" << endl;
    }
};

// High-level module depends on abstraction
class ReportGenerator {
private:
    DatabaseInterface& database;  // ✅ Depends on abstraction!
    
public:
    ReportGenerator(DatabaseInterface& db) : database(db) {}
    
    void generateReport() {
        database.saveData("report data");
    }
};

// Usage:
MySQLDatabase mysql;
ReportGenerator reporter1(mysql);

MongoDatabase mongo;
ReportGenerator reporter2(mongo);
```

**Benefits:**
- High-level logic independent of low-level details
- Can switch implementations easily
- Testable with mocks
- Low coupling
- Follows SOLID principles

---

## 📊 Measuring Coupling

### Coupling Metrics

**1. Afferent Coupling (Ca)**
- Number of classes that depend ON this class
- High Ca = Many classes depend on you (stable)

**2. Efferent Coupling (Ce)**
- Number of classes this class depends on
- High Ce = You depend on many (unstable)

**3. Instability (I = Ce / (Ca + Ce))**
- Range: 0 to 1
- I = 0: Stable (many depend on you, you depend on few)
- I = 1: Unstable (few depend on you, you depend on many)

### Example

```
Class A:
• 5 classes depend on A (Ca = 5)
• A depends on 2 classes (Ce = 2)
• Instability: I = 2 / (5 + 2) = 0.28 (Stable) ✅

Class B:
• 1 class depends on B (Ca = 1)
• B depends on 8 classes (Ce = 8)
• Instability: I = 8 / (1 + 8) = 0.89 (Unstable) 🚨
```

### Goals

✅ Keep coupling low (minimize dependencies)  
✅ Stable classes should be abstract  
✅ Unstable classes should be concrete  
✅ Depend on stable abstractions  

### Warning Signs

🚨 Class depends on > 7 other classes  
🚨 Class is depended on by > 10 classes  
🚨 Circular dependencies  
🚨 Deep inheritance hierarchies (> 5 levels)  

---

## 📋 Interface Segregation Principle

### The Principle

> **"Clients should not be forced to depend on interfaces they don't use"**

Many small, specific interfaces > One large, general interface

### ❌ **Bad: Fat Interface**

```cpp
class Worker {
public:
    virtual void work() = 0;
    virtual void eat() = 0;
    virtual void sleep() = 0;
    virtual void takeMeetings() = 0;
    virtual void writeCode() = 0;
    virtual void managePeople() = 0;
};

// Robot forced to implement methods it doesn't need!
class Robot : public Worker {
public:
    void work() override {
        cout << "Robot working" << endl;
    }
    
    void eat() override {
        // 🚨 Robots don't eat!
        throw runtime_error("Robots don't eat");
    }
    
    void sleep() override {
        // 🚨 Robots don't sleep!
        throw runtime_error("Robots don't sleep");
    }
    
    // ... more unnecessary methods
};
```

**Problems:**
- Robot coupled to methods it doesn't need
- Fat interface
- Violates Interface Segregation Principle

### ✅ **Good: Segregated Interfaces**

```cpp
// Small, focused interfaces
class Workable {
public:
    virtual void work() = 0;
    virtual ~Workable() = default;
};

class Eatable {
public:
    virtual void eat() = 0;
    virtual ~Eatable() = default;
};

class Sleepable {
public:
    virtual void sleep() = 0;
    virtual ~Sleepable() = default;
};

// Robot only implements what it needs
class Robot : public Workable {
public:
    void work() override {
        cout << "Robot working" << endl;
    }
};

// Human implements multiple interfaces
class Human : public Workable, public Eatable, public Sleepable {
public:
    void work() override {
        cout << "Human working" << endl;
    }
    
    void eat() override {
        cout << "Human eating" << endl;
    }
    
    void sleep() override {
        cout << "Human sleeping" << endl;
    }
};
```

**Benefits:**
- Clients only depend on interfaces they use
- Robot not coupled to eat/sleep methods
- Changes to one interface don't affect others
- Lower coupling
- Follows Interface Segregation Principle

---

## ✅ Strategies for Low Coupling

### 1. Program to Interfaces, Not Implementations

```cpp
❌ MySQLDatabase database;          // Concrete
✅ Database& database;               // Interface
```

### 2. Use Dependency Injection

```cpp
❌ class Service {
       EmailService email;  // Created internally
   }

✅ class Service {
       EmailService& email;  // Injected
       Service(EmailService& e) : email(e) {}
   }
```

### 3. Follow Law of Demeter

```cpp
❌ order.getCustomer().getAddress().getCountry()
✅ order.getShippingCountry()
```

### 4. Use Events/Observers

```cpp
❌ Direct method calls between modules
✅ Publish events, listeners subscribe
```

### 5. Avoid Global State

```cpp
❌ Global variables
✅ Explicit dependencies
```

### 6. Apply Interface Segregation

```cpp
❌ One big interface
✅ Many small, focused interfaces
```

### 7. Depend on Stable Abstractions

```cpp
❌ Depend on volatile concrete classes
✅ Depend on stable interfaces
```

---

## 📊 Benefits vs Costs

### Benefits of Low Coupling

✅ **Easier to Change**
- Changes localized
- Less ripple effect
- Lower risk

✅ **Easier to Test**
- Test in isolation
- Mock dependencies easily
- Faster tests

✅ **Better Reusability**
- Modules work independently
- Can reuse in different contexts
- Don't drag dependencies

✅ **Easier to Understand**
- Less to think about
- Clear interfaces
- Simpler mental model

✅ **Parallel Development**
- Teams work independently
- Less coordination
- Faster delivery

✅ **Better Maintainability**
- Bugs isolated
- Easier to fix
- Lower long-term costs

### When Coupling Is Acceptable

Sometimes coupling is necessary:
- Within a module/package (internal cohesion)
- For performance-critical code
- When abstraction adds no value
- Stable dependencies (standard library)

**Key**: Minimize coupling between modules, maximize cohesion within modules

---

## 📋 Checklist

Before writing/reviewing code, ask:

### Design Questions

- [ ] Do I depend on interfaces, not concrete classes?
- [ ] Are dependencies injected from outside?
- [ ] Can I test this module independently?
- [ ] Do I follow Law of Demeter (only talk to friends)?
- [ ] Are my interfaces focused and small?
- [ ] Do I avoid global state?
- [ ] Can this module be reused elsewhere?
- [ ] Are abstractions stable?

### Warning Signs

- [ ] Changes in one class require changes in many others?
- [ ] Hard to test without complex setup?
- [ ] Can't reuse without dragging dependencies?
- [ ] Code like: `obj.getX().getY().getZ()`?
- [ ] Many concrete class dependencies?
- [ ] Circular dependencies?
- [ ] Global variables everywhere?

### Coupling Metrics

- [ ] Class depends on < 7 other classes?
- [ ] Class depended on by < 10 classes?
- [ ] Instability metric reasonable?
- [ ] No circular dependencies?

---

## 💡 Key Insights

### The Core Principle

```
╔════════════════════════════════════════╗
║                                        ║
║  LOW COUPLING                          ║
║                                        ║
║  Minimize dependencies between modules ║
║  Depend on abstractions                ║
║  Keep modules independent              ║
║                                        ║
╚════════════════════════════════════════╝
```

### Remember

1. **Depend on abstractions**, not concretions
2. **Inject dependencies**, don't create them
3. **Follow Law of Demeter** - only talk to friends
4. **Segregate interfaces** - many small > one large
5. **Avoid global state** - explicit is better than implicit
6. **Use events** when appropriate
7. **Measure and monitor** coupling metrics

### Signs of Good Design

✅ Can explain what module does without referencing others  
✅ Can test module independently  
✅ Can change module without affecting others  
✅ Can reuse module in different contexts  
✅ Changes localized to one module  

### Signs of Poor Design

🚨 Changes ripple through system  
🚨 Can't test without mocking 10 classes  
🚨 Can't reuse without dragging dependencies  
🚨 Understanding one module requires understanding many  
🚨 Circular dependencies  

---

## 🎯 Summary

### What We've Learned

1. **Coupling** is the degree of interdependence between modules
2. **Low coupling** means modules are independent
3. **High coupling** makes systems rigid and hard to maintain
4. **Key techniques**: Interfaces, dependency injection, Law of Demeter, events

### The Golden Rule

```
"Good design is loosely coupled and highly cohesive"

Low Coupling  = Modules are independent
High Cohesion = Related things stay together

Together, they create maintainable software!
```

### Final Wisdom

> "The secret to building large apps is never build large apps.  
> Break your applications into small pieces.  
> Then, assemble those testable, bite-sized pieces  
> into your big application."  
> — Justin Meyer

**Low coupling is the key to building systems that can grow and evolve.**

Minimize dependencies between modules.  
Depend on abstractions.  
Keep modules independent.  
Your future self will thank you.

---

## 📚 Further Reading

- **"Design Patterns" by Gang of Four** - Patterns for loose coupling
- **"Clean Architecture" by Robert C. Martin** - Dependency management
- **"Dependency Injection" by Mark Seemann** - DI in depth
- **"Working Effectively with Legacy Code" by Michael Feathers** - Breaking dependencies

---

**Master Low Coupling, and you'll write code that's flexible, testable, and maintainable. 🚀**