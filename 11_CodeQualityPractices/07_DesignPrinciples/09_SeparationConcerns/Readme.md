# Separation of Concerns (SoC)

## 📚 Overview

The **Separation of Concerns (SoC)** principle states:

> **"Separate different concerns into distinct sections"**  
> **"Each module should have a single, well-defined responsibility"**  
> — Edsger W. Dijkstra

### What It Means

- **Break complex problems** into smaller, focused pieces
- **Each piece handles ONE concern** or responsibility
- **Different concerns** shouldn't be mixed together
- **Related functionality** grouped together
- **Unrelated functionality** kept separate

### Why It Matters

✅ **Easier to understand** - Focus on one thing at a time  
✅ **Easier to maintain** - Changes are isolated  
✅ **Easier to test** - Test each concern independently  
✅ **Better reusability** - Concerns can be reused  
✅ **Parallel development** - Different people, different concerns  
✅ **Greater flexibility** - Swap implementations easily  

---

## 🎯 What is a "Concern"?

A **concern** is a specific aspect, feature, or responsibility of your system.

### Common Concerns

```
┌────────────────────────────────────────┐
│  TECHNICAL CONCERNS                    │
├────────────────────────────────────────┤
│  • Data persistence (database, files)  │
│  • User interface (display, input)     │
│  • Networking (HTTP, APIs)             │
│  • Caching                             │
│  • Logging                             │
│  • Error handling                      │
│  • Configuration                       │
│  • Security/Authentication             │
└────────────────────────────────────────┘

┌────────────────────────────────────────┐
│  BUSINESS CONCERNS                     │
├────────────────────────────────────────┤
│  • Business rules                      │
│  • Calculations                        │
│  • Validation                          │
│  • Workflows                           │
│  • Domain logic                        │
└────────────────────────────────────────┘
```

### The Goal

```
Each module → ONE concern → Well defined → Easy to understand
```

---

## 🚨 Violation Signs

How to spot poor separation of concerns:

- 🚨 Classes doing too many different things
- 🚨 Mixed business logic with UI code
- 🚨 Database queries in presentation layer
- 🚨 Hard to test because everything is tangled
- 🚨 Changes in one area break unrelated areas
- 🚨 Can't reuse code because it's too coupled
- 🚨 "God classes" that do everything
- 🚨 Need to understand entire system to change one thing

---

## 📖 Classic Example 1: Mixed UI and Business Logic

### ❌ **Bad: Everything Tangled Together**

```cpp
class UserManager {
public:
    void createUser() {
        // 🚨 UI CONCERN: Getting input
        cout << "=== Create User ===" << endl;
        cout << "Enter username: ";
        string username;
        cin >> username;
        
        cout << "Enter email: ";
        string email;
        cin >> email;
        
        cout << "Enter password: ";
        string password;
        cin >> password;
        
        // 🚨 VALIDATION CONCERN: Checking data
        if (username.empty()) {
            cout << "Error: Username required!" << endl;
            return;
        }
        
        if (email.find("@") == string::npos) {
            cout << "Error: Invalid email!" << endl;
            return;
        }
        
        if (password.length() < 8) {
            cout << "Error: Password too short!" << endl;
            return;
        }
        
        // 🚨 DATABASE CONCERN: Saving data
        cout << "Saving to database..." << endl;
        // Database code here
        
        // 🚨 PRESENTATION CONCERN: Displaying result
        cout << "User created successfully!" << endl;
        cout << "Username: " << username << endl;
        cout << "Email: " << email << endl;
    }
};
```

**Problems:**
- UI, validation, business logic, database all mixed
- Can't test business logic without UI
- Can't reuse validation elsewhere
- Can't change UI without touching business logic
- Hard to maintain and understand
- One giant method doing everything

### ✅ **Good: Concerns Properly Separated**

```cpp
// CONCERN 1: Data Model (pure data)
struct User {
    string username;
    string email;
    string password;
    
    User(const string& u, const string& e, const string& p)
        : username(u), email(e), password(p) {}
};

// CONCERN 2: Validation (business rules)
class UserValidator {
public:
    static bool validateUsername(const string& username) {
        return !username.empty() && username.length() >= 3;
    }
    
    static bool validateEmail(const string& email) {
        return email.find("@") != string::npos && 
               email.find(".") != string::npos;
    }
    
    static bool validatePassword(const string& password) {
        return password.length() >= 8;
    }
    
    static bool validateUser(const User& user) {
        return validateUsername(user.username) &&
               validateEmail(user.email) &&
               validatePassword(user.password);
    }
};

// CONCERN 3: Business Logic
class UserService {
public:
    User createUser(const string& username, const string& email, 
                   const string& password) {
        User user(username, email, password);
        
        if (!UserValidator::validateUser(user)) {
            throw runtime_error("Invalid user data");
        }
        
        return user;
    }
};

// CONCERN 4: Data Persistence
class UserRepository {
public:
    void save(const User& user) {
        // Database operations only
        cout << "[DB] Saving user: " << user.username << endl;
    }
};

// CONCERN 5: User Interface
class UserUI {
private:
    UserService service;
    UserRepository repository;
    
public:
    void createUserInterface() {
        cout << "\n=== Create User ===" << endl;
        
        string username, email, password;
        
        cout << "Enter username: ";
        cin >> username;
        
        cout << "Enter email: ";
        cin >> email;
        
        cout << "Enter password: ";
        cin >> password;
        
        try {
            User user = service.createUser(username, email, password);
            repository.save(user);
            
            cout << "\n✅ User created successfully!" << endl;
            cout << "Username: " << user.username << endl;
            cout << "Email: " << user.email << endl;
            
        } catch (const exception& e) {
            cout << "\n❌ Error: " << e.what() << endl;
        }
    }
};
```

**Benefits:**
- Each class has ONE clear responsibility
- Can test each concern independently
- Can reuse validation, business logic, etc.
- Changes isolated to relevant concern
- Much easier to understand and maintain
- Can swap implementations (e.g., different database)

---

## 📖 Classic Example 2: Data Access in Business Logic

### ❌ **Bad: Mixed Concerns**

```cpp
class OrderProcessor {
public:
    void processOrder(int orderId) {
        // 🚨 DATABASE CONCERN mixed in
        cout << "SELECT * FROM orders WHERE id = " << orderId << endl;
        // Fetching from database...
        
        double total = 100.0;
        
        // BUSINESS LOGIC
        if (total > 1000) {
            total *= 0.9;  // 10% discount
            cout << "Discount applied!" << endl;
        }
        
        // 🚨 DATABASE CONCERN again
        cout << "UPDATE orders SET total = " << total 
             << " WHERE id = " << orderId << endl;
        
        // 🚨 EMAIL CONCERN mixed in
        cout << "Sending email confirmation..." << endl;
        cout << "To: customer@example.com" << endl;
        
        // PROBLEMS:
        // • Can't test business logic without database
        // • Can't reuse discount logic
        // • Can't change database without changing business logic
        // • Three different concerns tangled together
    }
};
```

### ✅ **Good: Concerns Separated**

```cpp
// CONCERN 1: Data Model
struct Order {
    int id;
    double total;
    string customerEmail;
};

// CONCERN 2: Data Access
class OrderRepository {
public:
    Order findById(int orderId) {
        cout << "[DB] Fetching order " << orderId << endl;
        return Order{orderId, 100.0, "customer@example.com"};
    }
    
    void update(const Order& order) {
        cout << "[DB] Updating order " << order.id << endl;
    }
};

// CONCERN 3: Business Logic
class OrderService {
public:
    void applyDiscount(Order& order) {
        // Pure business logic
        if (order.total > 1000) {
            order.total *= 0.9;  // 10% discount
            cout << "[BUSINESS] Discount applied" << endl;
        }
    }
};

// CONCERN 4: Notification
class EmailService {
public:
    void sendOrderConfirmation(const Order& order) {
        // Email concerns only
        cout << "[EMAIL] Sending to: " << order.customerEmail << endl;
    }
};

// CONCERN 5: Orchestration (coordinates concerns)
class OrderProcessor {
private:
    OrderRepository repository;
    OrderService service;
    EmailService emailService;
    
public:
    void processOrder(int orderId) {
        // Clean orchestration
        Order order = repository.findById(orderId);
        service.applyDiscount(order);
        repository.update(order);
        emailService.sendOrderConfirmation(order);
    }
};
```

**Benefits:**
- Can test business logic with mock repository
- Can reuse OrderService for other operations
- Can change database without touching business logic
- Can change email service independently
- Each concern clear and focused

---

## 📖 Classic Example 3: The "God Class" Anti-Pattern

### ❌ **Bad: God Class Doing Everything**

```cpp
class Application {
private:
    map<string, string> users;
    map<int, double> inventory;
    vector<string> logs;
    
public:
    // User management
    void registerUser(string username, string password) { /* ... */ }
    bool loginUser(string username, string password) { /* ... */ }
    
    // Inventory management
    void addProduct(int id, double price) { /* ... */ }
    double getProductPrice(int id) { /* ... */ }
    
    // Order processing
    void processOrder(int productId, string username) { /* ... */ }
    
    // Reporting
    void generateReport() { /* ... */ }
    
    // Logging
    void printLogs() { /* ... */ }
    
    // Configuration
    void loadConfig() { /* ... */ }
    void saveConfig() { /* ... */ }
    
    // Email
    void sendEmail(string to, string message) { /* ... */ }
    
    // File operations
    void saveToFile() { /* ... */ }
    void loadFromFile() { /* ... */ }
    
    // ... 50+ more methods
};

// 🚨 PROBLEMS:
// • One class doing EVERYTHING
// • 1000+ lines of code
// • Hard to understand
// • Impossible to test properly
// • Hard to maintain
// • Changes affect everything
// • Multiple people can't work on it simultaneously
```

### ✅ **Good: Separated into Focused Classes**

```cpp
// Each class handles ONE concern

class UserManager {
private:
    map<string, string> users;
public:
    void registerUser(string username, string password);
    bool authenticate(string username, string password);
    int getUserCount() const;
};

class InventoryManager {
private:
    map<int, double> inventory;
public:
    void addProduct(int id, double price);
    double getPrice(int id) const;
    int getProductCount() const;
};

class OrderProcessor {
private:
    InventoryManager& inventory;
public:
    OrderProcessor(InventoryManager& inv) : inventory(inv) {}
    bool processOrder(int productId, string username);
};

class ReportGenerator {
private:
    UserManager& userManager;
    InventoryManager& inventoryManager;
public:
    ReportGenerator(UserManager& um, InventoryManager& im);
    void generate();
};

class Logger {
private:
    vector<string> logs;
public:
    void log(string message);
    void printAll() const;
};
```

**Benefits:**
- Each class focused on one concern
- 100-200 lines per class instead of 1000+
- Easy to understand each component
- Easy to test independently
- Multiple developers can work in parallel
- Changes isolated to relevant class

---

## 🏗️ Layered Architecture

A classic pattern for separation of concerns:

```
┌─────────────────────────────────────────┐
│  LAYER 4: Presentation (UI/Controllers) │
│  • Display data                         │
│  • Handle user input                    │
│  • Format output                        │
└─────────────────────────────────────────┘
                  ↓ uses
┌─────────────────────────────────────────┐
│  LAYER 3: Business Logic (Services)     │
│  • Business rules                       │
│  • Calculations                         │
│  • Workflows                            │
│  • Orchestration                        │
└─────────────────────────────────────────┘
                  ↓ uses
┌─────────────────────────────────────────┐
│  LAYER 2: Data Access (Repositories)    │
│  • Database queries                     │
│  • File I/O                             │
│  • External APIs                        │
└─────────────────────────────────────────┘
                  ↓ uses
┌─────────────────────────────────────────┐
│  LAYER 1: Domain (Models/Entities)      │
│  • Data structures                      │
│  • Core domain objects                  │
└─────────────────────────────────────────┘
```

### Rules

1. **Each layer only depends on layers below**
2. **No skipping layers** (e.g., Presentation can't directly access Data)
3. **Clear boundaries** between layers
4. **Each layer has specific responsibility**

### Example

```cpp
// LAYER 1: Domain
namespace domain {
    struct Product {
        int id;
        string name;
        double price;
        int stock;
    };
}

// LAYER 2: Data Access
namespace data {
    class ProductRepository {
    public:
        Product findById(int id);
        void save(const Product& product);
        vector<Product> findAll();
    };
}

// LAYER 3: Business Logic
namespace business {
    class ProductService {
    private:
        ProductRepository repository;
    public:
        Product getProduct(int id);
        bool purchaseProduct(int productId, int quantity);
        double calculateTotalValue();
    };
}

// LAYER 4: Presentation
namespace presentation {
    class ProductController {
    private:
        ProductService service;
    public:
        void displayProduct(int id);
        void handlePurchase(int productId, int quantity);
    };
}
```

---

## 🔍 How to Identify Concerns

### 1. The "Reason to Change" Test

Ask: **"Why would this module need to change?"**

```cpp
class UserManager {
    void saveUser();      // Changes if database changes
    void validateUser();  // Changes if validation rules change
    void displayUser();   // Changes if UI changes
    void emailUser();     // Changes if email format changes
};
```

If multiple reasons → Multiple concerns → **Separate them!**

### 2. Verb Analysis

Different verbs often indicate different concerns:

```
• Validate    → Validation concern
• Calculate   → Business logic concern
• Save/Load   → Data access concern
• Display     → Presentation concern
• Send/Notify → Communication concern
• Log         → Logging concern
```

### 3. Technical vs Business Concerns

**Technical Concerns** (how):
- Database access
- File I/O
- Network communication
- Caching
- Logging

**Business Concerns** (what):
- Business rules
- Calculations
- Workflows
- Domain logic

**Keep them separate!**

### 4. Abstraction Level Mixing

Watch for mixed abstraction levels:

```cpp
❌ class ReportGenerator {
       void connectToDatabase();  // Low-level
       void generateReport();     // High-level
   }
```

Different levels = Different concerns

---

## 🎯 Best Practices

### 1. Single Responsibility Principle (SRP)

```
Each class should have ONE reason to change
= Each class should handle ONE concern
```

### 2. High Cohesion

```
Keep related things together
All methods in a class should relate to its single concern
```

### 3. Low Coupling

```
Minimize dependencies between concerns
Changes in one concern shouldn't require changes in others
```

### 4. Clear Boundaries

```
Define interfaces between concerns
Use dependency injection
Program to interfaces, not implementations
```

### 5. Consistent Patterns

```
Use established patterns:
• Layered Architecture
• MVC (Model-View-Controller)
• Repository Pattern
• Service Layer Pattern
```

### 6. Avoid God Classes

```
No class should do everything
Break large classes into focused smaller ones
If class has 20+ methods, probably doing too much
```

---

## 🔄 Cross-Cutting Concerns

Some concerns apply across multiple modules:

### Common Cross-Cutting Concerns

```
• Logging
• Security/Authentication
• Caching
• Error handling
• Transactions
• Monitoring/Metrics
• Audit trails
```

### Solution: Aspect-Oriented Approach

```cpp
// Separate aspect classes
class SecurityAspect {
public:
    static bool checkPermission(string user, string action) {
        cout << "[SECURITY] Checking permission" << endl;
        return true;
    }
};

class CacheAspect {
private:
    static map<string, string> cache;
public:
    static bool hasCache(string key);
    static string getCache(string key);
    static void putCache(string key, string value);
};

class LoggingAspect {
public:
    static void logEntry(string method) {
        cout << "[TRACE] Entering " << method << endl;
    }
    
    static void logExit(string method) {
        cout << "[TRACE] Exiting " << method << endl;
    }
};

// Business service using aspects
class DataService {
public:
    string getData(string user, string key) {
        LoggingAspect::logEntry("getData");
        
        if (!SecurityAspect::checkPermission(user, "read")) {
            return "Access denied";
        }
        
        if (CacheAspect::hasCache(key)) {
            return CacheAspect::getCache(key);
        }
        
        // Business logic
        string data = fetchDataFromSource(key);
        CacheAspect::putCache(key, data);
        
        LoggingAspect::logExit("getData");
        return data;
    }
};
```

**Benefits:**
- Cross-cutting logic applied consistently
- Business logic stays clean
- Easy to modify cross-cutting behavior
- Reusable across system

---

## ✅ Benefits

### 1. Maintainability

```
✅ Changes isolated to one concern
✅ Don't break unrelated functionality
✅ Easy to find where to make changes
✅ Smaller, focused modules
```

### 2. Testability

```
✅ Test each concern independently
✅ Mock dependencies easily
✅ Smaller, focused tests
✅ Better test coverage
```

### 3. Reusability

```
✅ Concerns can be reused
✅ Validation logic reusable
✅ Data access reusable
✅ Business logic reusable
```

### 4. Understandability

```
✅ Each module has clear purpose
✅ Easier to learn codebase
✅ Less cognitive load
✅ New developers onboard faster
```

### 5. Parallel Development

```
✅ Different people work on different concerns
✅ Less merge conflicts
✅ Faster development
✅ Better team collaboration
```

### 6. Flexibility

```
✅ Swap implementations easily
✅ Change database without affecting business logic
✅ Change UI without affecting data access
✅ Adapt to new requirements faster
```

---

## 📊 Before vs After

### Without Separation of Concerns

```
┌─────────────────────────────────────┐
│  ONE GOD CLASS (1000+ lines)        │
├─────────────────────────────────────┤
│  • UI code                          │
│  • Business logic                   │
│  • Database code                    │
│  • Validation                       │
│  • Logging                          │
│  • Email                            │
│  • Everything tangled together      │
└─────────────────────────────────────┘

Problems:
• Hard to understand
• Hard to test
• Hard to maintain
• Changes break everything
• Can't reuse anything
• Single point of failure
```

### With Separation of Concerns

```
┌────────────────┐  ┌────────────────┐
│  UserUI        │  │  ProductUI     │
│  (100 lines)   │  │  (100 lines)   │
└────────────────┘  └────────────────┘
        ↓                   ↓
┌────────────────┐  ┌────────────────┐
│  UserService   │  │  ProductService│
│  (150 lines)   │  │  (150 lines)   │
└────────────────┘  └────────────────┘
        ↓                   ↓
┌────────────────┐  ┌────────────────┐
│  UserRepo      │  │  ProductRepo   │
│  (100 lines)   │  │  (100 lines)   │
└────────────────┘  └────────────────┘

Benefits:
• Easy to understand each part
• Easy to test individually
• Easy to maintain
• Changes isolated
• High reusability
• Parallel development possible
```

---

## 🎓 Common Patterns

### 1. MVC (Model-View-Controller)

```
┌───────────┐
│   View    │ ← Presentation concern
└───────────┘
      ↕
┌───────────┐
│Controller │ ← Coordination concern
└───────────┘
      ↕
┌───────────┐
│   Model   │ ← Data + Business logic concern
└───────────┘
```

### 2. Repository Pattern

```
Business Logic
      ↓
Repository Interface (abstraction)
      ↓
Repository Implementation (data access concern)
```

### 3. Service Layer

```
Presentation Layer
      ↓
Service Layer (business logic concern)
      ↓
Data Access Layer
```

### 4. Dependency Injection

```cpp
class OrderProcessor {
private:
    IOrderRepository& repository;     // Injected
    IEmailService& emailService;      // Injected
    ILogger& logger;                  // Injected
    
public:
    OrderProcessor(IOrderRepository& repo,
                   IEmailService& email,
                   ILogger& log)
        : repository(repo), emailService(email), logger(log) {}
    
    // Business logic here
    // Dependencies injected, not created internally
};
```

---

## 🚨 Common Mistakes

### 1. Mixing UI and Business Logic

```cpp
❌ void processOrder() {
       cout << "Enter order ID: ";  // UI
       cin >> orderId;
       
       if (total > 100) {            // Business logic
           total *= 0.9;
       }
       
       cout << "Total: " << total;   // UI
   }
```

### 2. Database Code in Presentation

```cpp
❌ void displayUsers() {
       // Presentation layer doing database work
       cout << "SELECT * FROM users";
       // Fetch and display
   }
```

### 3. Business Logic in Repository

```cpp
❌ class UserRepository {
       void save(User user) {
           // Business logic in repository!
           if (user.age < 18) {
               throw "Too young";
           }
           // Save to database
       }
   }
```

### 4. God Classes

```cpp
❌ class Application {
       void manageUsers();
       void manageProducts();
       void manageOrders();
       void manageReports();
       void manageConfig();
       // ... 50 more methods
   }
```

---

## 📋 Checklist

Before writing/reviewing code, ask:

### Design Questions

- [ ] Can I explain this class's purpose in one sentence?
- [ ] Does this class have a single, clear responsibility?
- [ ] Are different concerns mixed together?
- [ ] Can this be broken into smaller, focused pieces?
- [ ] Is business logic separate from UI?
- [ ] Is data access separate from business logic?

### Testing Questions

- [ ] Can I test this concern independently?
- [ ] Do I need complex setup to test?
- [ ] Can I mock dependencies easily?
- [ ] Are tests focused on one concern?

### Maintenance Questions

- [ ] If requirement X changes, which classes need modification?
- [ ] Will changing this concern affect others?
- [ ] Can I reuse this elsewhere?
- [ ] Is it clear where to make changes?

### Red Flags

- [ ] Class has methods with very different purposes?
- [ ] Need to understand entire system to change one thing?
- [ ] Changes in one area break unrelated areas?
- [ ] Hard to name the class (doing too many things)?

---

## 💡 Key Insights

### The Core Principle

```
╔════════════════════════════════════════╗
║                                        ║
║  Complexity managed through            ║
║  SEPARATION                            ║
║                                        ║
║  Each module → ONE concern            ║
║  Different concerns → SEPARATE         ║
║                                        ║
╚════════════════════════════════════════╝
```

### Remember

1. **Identify concerns** in your system
2. **Separate** them into distinct modules
3. **Keep** concerns independent
4. **Define** clear interfaces between concerns
5. **Maintain** high cohesion, low coupling

### Signs of Good Separation

✅ Can explain each module in one sentence  
✅ Can test each concern independently  
✅ Can change one concern without affecting others  
✅ Can reuse concerns in different contexts  
✅ New developers understand quickly  

### Signs of Poor Separation

🚨 Classes doing many different things  
🚨 Hard to explain what class does  
🚨 Can't test without complex setup  
🚨 Changes ripple across unrelated code  
🚨 God classes with 1000+ lines  

---

## 🎯 Summary

### What We've Learned

1. **Concerns** are distinct responsibilities/features
2. **Separate** different concerns into different modules
3. **Each module** should handle ONE concern well
4. **Benefits**: Maintainable, testable, reusable, flexible code

### The Golden Rule

```
When in doubt, ask:
"Does this class/module have ONE clear purpose?"

If NO → Separate the concerns!
```

### Final Wisdom

> "The art of programming is the art of organizing complexity,  
> of mastering multitude and avoiding its bastard chaos."  
> — Edsger W. Dijkstra

**Separation of Concerns is the key to managing complexity.**

Break complex problems into simple, focused pieces.  
Each piece does ONE thing well.  
Together, they create maintainable, professional software.

---

## 📚 Further Reading

- **"Clean Architecture" by Robert C. Martin** - Architectural separation
- **"Domain-Driven Design" by Eric Evans** - Domain vs technical concerns
- **"Patterns of Enterprise Application Architecture" by Martin Fowler** - Layered architecture patterns
- **"Design Patterns" by Gang of Four** - Separation through patterns

---

**Master Separation of Concerns, and you'll write code that's maintainable, testable, and professional. 🚀**

## Notes moved from main.cpp

This text used to be printed by `main()`. It is lecture material, so it lives here and the program only prints the demo output.

### 🎉 SEPARATION OF CONCERNS COMPLETE! 🎉 (closing summary)

Key Takeaway: Break complex problems into focused  
pieces. Each piece should handle ONE concern well.  
Keep concerns separate and independent!

### Text printed by the example functions

This text used to be printed between the demo steps. It is lecture material, so it lives here.

### From `example1_mixed_concerns()`

- ❌ BAD CODE (Everything mixed):

```cpp

```cpp
class UserManager {
    void createUser() {
        // UI code
        cout << "Enter username: ";
        cin >> username;
        // Validation code
        if (username.empty()) { ... }
        // Database code
        saveToDatabase(...);
        // Logging code
        cout << "User created";
    }
};
// 🚨 Everything tangled together!
```

```

**🚨 PROBLEMS:**

  - Can't test business logic without UI
  - Can't reuse validation elsewhere
  - Can't change UI without touching business logic
  - Hard to maintain and understand
  - Different concerns all mixed together

- ✅ GOOD CODE (Separated concerns):

```cpp

```cpp
// Concern 1: Data
struct User { string username, email, password; };

// Concern 2: Validation
class UserValidator { bool validate(...); };

// Concern 3: Business logic
class UserService { User createUser(...); };

// Concern 4: Database
class UserRepository { void save(User); };

// Concern 5: UI
class UserUI { void createUserInterface(); };
```

```

**✨ BENEFITS:**

  - Each class has ONE clear responsibility
  - Can test each concern independently
  - Can reuse validation, business logic, etc.
  - Changes isolated to relevant concern
  - Much easier to understand and maintain

### From `example1_mixed_concerns()`

Sending email confirmation...  
To: customer@example.com  
Subject: Order confirmed

### From `example2_data_access()`

- ❌ BAD CODE (Mixed concerns):

Single class doing:

  - Database queries
  - Business logic (discount calculation)
  - Email sending

🚨 Can't test discount logic without database!

- ✅ GOOD CODE (Separated concerns):
  - OrderRepository: Database access
  - OrderService: Business logic
  - EmailService: Notifications
  - OrderProcessor: Orchestrates the flow

**✨ DEMONSTRATION:**

### From `example2_data_access()`

**✨ BENEFITS:**

  - Can test business logic with mock repository
  - Can reuse OrderService for other operations
  - Can change database without touching business logic
  - Can change email service independently
  - Each concern clear and focused

### From `example3_reporting()`

- ❌ BAD CODE:

One method doing:

  - Fetching data from database
  - Calculating totals and averages
  - Formatting output
  - Writing to file

🚨 Impossible to reuse or test separately!

- ✅ GOOD CODE:
  - SalesRepository: Data access
  - SalesCalculator: Business logic
  - ReportFormatter: Presentation (Console, HTML, JSON)
  - ReportWriter: File I/O
  - SalesReportGenerator: Orchestration

**✨ DEMONSTRATION:**

### From `example3_reporting()`

**✨ BENEFITS:**

  - Can test calculations without database
  - Can easily add new formats (XML, CSV)
  - Can reuse calculator for other reports
  - Can change data source without affecting calculations
  - Each component has single, clear purpose

### From `example3_reporting()`

[LOG] Validating payment method  
[LOG] Contacting payment gateway  
[LOG] Payment successful

### From `example4_logging()`

- ❌ BAD CODE:
  - cout << "[LOG]" scattered everywhere
  - Hard to disable logging
  - Hard to change output destination
  - Business logic obscured

- ✅ GOOD CODE:
  - Logger interface for logging concern
  - Multiple implementations (Console, File, Null)
  - Business logic clean and focused
  - Easy to swap logging strategies

**✨ DEMONSTRATION:**

### From `example4_logging()`

**✨ BENEFITS:**

  - Can disable logging easily
  - Can log to file, console, or network
  - Business logic stays clean
  - Can test without logging noise

### From `example4_logging()`

Product Details  
╠════════════════════════════╣

### From `example4_logging()`

Inventory Value  
╠════════════════════════════╣

### From `example5_layered_architecture()`

- ✅ LAYERED ARCHITECTURE:

Layer 4: Presentation (UI/Controllers)  
↓ uses  
Layer 3: Business Logic (Services)  
↓ uses  
Layer 2: Data Access (Repositories)  
↓ uses  
Layer 1: Domain (Models/Entities)

**🔒 RULES:**

  - Each layer only depends on layers below
  - No skipping layers
  - Clear separation of concerns

**✨ DEMONSTRATION:**

### From `example5_layered_architecture()`

**✨ BENEFITS:**

  - Clear organization
  - Easy to test each layer
  - Can replace layers (e.g., swap database)
  - Changes isolated to relevant layer
  - Standard pattern, easy to understand

### From `example6_god_class()`

- ❌ BAD CODE (God Class):

```cpp

```cpp
class Application {
    // User management
    void registerUser();
    void loginUser();
    // Inventory
    void addProduct();
    // Orders
    void processOrder();
    // Reporting
    void generateReport();
    // Logging, Email, Files...
    // ... 50+ methods
};
// 🚨 One class doing EVERYTHING!
```

```

**🚨 PROBLEMS:**

  - 1000+ lines in one class
  - Hard to understand what it does
  - Impossible to test properly
  - Multiple people can't work on it
  - Changes affect everything
  - Violates Single Responsibility Principle

- ✅ GOOD CODE (Separated classes):
  - UserManager: User operations only
  - InventoryManager: Inventory operations only
  - OrderProcessor: Order processing only
  - ReportGenerator: Reporting only
  - Logger: Logging only

**✨ DEMONSTRATION:**

### From `example6_god_class()`

**✨ BENEFITS:**

  - Each class focused on one concern
  - Easy to understand each component
  - Easy to test independently
  - Multiple people can work in parallel
  - Changes isolated to relevant class

### From `example7_cross_cutting()`

**📚 CROSS-CUTTING CONCERNS:**

Concerns that span multiple modules:

  - Logging
  - Security/Authentication
  - Caching
  - Error handling
  - Transactions
  - Monitoring

- ✅ SOLUTION: Aspect-Oriented approach
  - Separate aspect classes
  - Applied consistently across system
  - Don't duplicate in every method

**✨ DEMONSTRATION:**

### From `example7_cross_cutting()`

**✨ BENEFITS:**

  - Logging consistent everywhere
  - Security applied uniformly
  - Caching reusable
  - Easy to modify cross-cutting logic
  - Business logic stays clean

### From `example8_identifying_concerns()`

**🔍 HOW TO IDENTIFY CONCERNS:**

1. LOOK FOR DIFFERENT RESPONSIBILITIES:
- ❌ Bad: class UserManager {

```cpp
    void saveUser();      // Database
    void validateUser();  // Validation
    void displayUser();   // UI
    void emailUser();     // Communication
}
```

- ✅ Good: Separate classes for each concern

2. APPLY THE 'REASON TO CHANGE' TEST:

Ask: Why would this class need to change?

  - Change database? → Data access concern
  - Change validation rules? → Validation concern
  - Change UI? → Presentation concern
  - Change email format? → Communication concern

If multiple reasons → Multiple concerns → Separate!

3. IDENTIFY TECHNICAL VS BUSINESS CONCERNS:

Technical concerns:

  - Database access
  - File I/O
  - Network communication
  - Caching
  - Logging

Business concerns:

  - Business rules
  - Calculations
  - Workflows
  - Domain logic

Keep them separate!

4. USE VERB ANALYSIS:

Different verbs often indicate different concerns:

  - Validate → Validation concern
  - Calculate → Business logic concern
  - Save/Load → Data access concern
  - Display/Show → Presentation concern
  - Send/Notify → Communication concern

5. WATCH FOR MIXED ABSTRACTIONS:
- ❌ class ReportGenerator {

```cpp
        void connectToDatabase();  // Low-level
        void generateReport();     // High-level
    }
Different levels of abstraction = Different concerns
```

### From `example9_benefits()`

- ✅ BENEFITS OF SEPARATION OF CONCERNS:

1. MAINTAINABILITY:
  - Changes isolated to one concern
  - Don't break unrelated functionality
  - Easy to find where to make changes

2. TESTABILITY:
  - Test each concern independently
  - Mock dependencies easily
  - Smaller, focused tests

3. REUSABILITY:
  - Concerns can be reused
  - Validation logic reusable
  - Data access reusable

4. UNDERSTANDABILITY:
  - Each module has clear purpose
  - Easier to learn codebase
  - Less cognitive load

5. PARALLEL DEVELOPMENT:
  - Different people work on different concerns
  - Less merge conflicts
  - Faster development

6. FLEXIBILITY:
  - Swap implementations easily
  - Change database without affecting business logic
  - Change UI without affecting data access

**🎯 BEST PRACTICES:**

1. SINGLE RESPONSIBILITY PRINCIPLE:

Each class should have one reason to change

2. HIGH COHESION:

Keep related things together

3. LOW COUPLING:

Minimize dependencies between concerns

4. CLEAR BOUNDARIES:

Define interfaces between concerns

5. CONSISTENT PATTERNS:

Use layered architecture or similar patterns

6. AVOID GOD CLASSES:

No class should do everything

- 💡 COMMON PATTERNS:
  - MVC (Model-View-Controller)
  - Layered Architecture
  - Repository Pattern
  - Service Layer Pattern
  - Dependency Injection

### From `example10_summary()`

🎯 SEPARATION OF CONCERNS: KEY POINTS

1. WHAT IS A CONCERN?

A distinct feature or behavior of the system  
Examples:

  - Data storage
  - Business rules
  - User interface
  - Validation
  - Communication

2. WHY SEPARATE?
  - Easier to understand
  - Easier to maintain
  - Easier to test
  - Easier to reuse
  - Easier to modify

3. HOW TO SEPARATE?
  - Identify different concerns
  - Create separate classes/modules
  - Define clear interfaces
  - Keep concerns independent

4. SIGNS OF GOOD SEPARATION:
- ✅ Each class has one clear purpose
- ✅ Can explain what class does in one sentence
- ✅ Can test each concern independently
- ✅ Can change one concern without affecting others
- ✅ Can reuse concerns in different contexts

5. SIGNS OF POOR SEPARATION:

🚨 Classes doing many different things  
🚨 Hard to explain what class does  
🚨 Can't test without complex setup  
🚨 Changes ripple across unrelated code  
🚨 Code duplication across concerns

**📊 BEFORE vs AFTER:**

WITHOUT Separation of Concerns:

  - 1 God class, 1000+ lines
  - Everything tangled together
  - Hard to understand
  - Hard to test
  - Changes break everything
  - Can't reuse anything

WITH Separation of Concerns:

  - 10 focused classes, 100 lines each
  - Each handles one concern
  - Easy to understand
  - Easy to test
  - Changes isolated
  - High reusability

**💭 REMEMBER:**

"Separation of Concerns is the art of dividing  
software into distinct sections, each addressing  
a separate concern."

Each module should:

  - Do ONE thing well
  - Have a clear responsibility
  - Be independent of other concerns

**🎓 FINAL WISDOM:**

Good separation of concerns leads to:

  - Maintainable code
  - Testable code
  - Reusable code
  - Flexible code
  - Professional code

## What was tested

- Compiled with GCC 13.3 (`-std=c++23`) using a stand-in `<print>` header, because `std::println` needs GCC 14 and it was not available here. The source files were compiled directly, not through CMake.
- Run, and the output matches the original `std::cout` version except that the lecture text moved into *Notes moved from main.cpp* is no longer printed (and the remaining example titles were renumbered where an example was removed).
- Run once with AddressSanitizer + UndefinedBehaviorSanitizer: no findings.

## Not tested

- The real `<print>` header (GCC 14+, recent Clang or MSVC) and platforms other than Linux x86-64.
