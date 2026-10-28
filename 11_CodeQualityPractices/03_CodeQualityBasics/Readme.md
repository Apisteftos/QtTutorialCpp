# Code Quality Basics - Complete Guide

## 📚 Overview

**Code quality** is the measure of how well code is written in terms of **maintainability**, **readability**, **reliability**, and **efficiency**. Quality code is easy to understand, modify, test, and extend.

### What Makes Quality Code?

Quality code has these characteristics:

✅ **Readable** - Clear and self-documenting  
✅ **Maintainable** - Easy to modify and extend  
✅ **Reliable** - Works correctly, handles errors  
✅ **Efficient** - Performs well without premature optimization  
✅ **Simple** - No unnecessary complexity  
✅ **Testable** - Can be tested easily  

---

## 💰 Why Code Quality Matters

### The Cost of Poor Quality

| Impact | Poor Quality | Quality Code |
|--------|-------------|--------------|
| **Maintenance** | 3-10x more expensive | Easy and cheap |
| **Bug Rate** | High defect density | Low defect density |
| **Modification Time** | Days/weeks | Hours/days |
| **Team Morale** | Frustration, burnout | Satisfaction, pride |
| **Technical Debt** | Grows exponentially | Manageable |

### Real-World Impact

```
Poor Quality Code:
- 80% of time spent understanding code
- Bugs take days to fix
- Features take weeks to add
- Team velocity slows over time
- New developers struggle for months

Quality Code:
- 80% of time spent adding value
- Bugs are rare and easy to fix
- Features added quickly
- Team velocity stays constant
- New developers productive in days
```

---

## 🚨 Code Smells

**Code smells** are warning signs that indicate potential problems in your code. They're not bugs, but symptoms of deeper issues.

### Categories of Code Smells

#### 1. **Bloaters** (Too Big, Too Much)

**Long Methods/Functions**
```cpp
// ❌ BAD: 100+ line function
void processEverything() {
    // Validation (20 lines)
    // Parsing (30 lines)
    // Calculation (20 lines)
    // Database (30 lines)
}

// ✅ GOOD: Small, focused functions
void processData() {
    validate();
    parse();
    calculate();
    save();
}
```

**Large Classes (God Objects)**
```cpp
// ❌ BAD: Class does everything
class Application {
    void manageUsers();
    void handleDatabase();
    void sendEmails();
    void processPayments();
    // ... 50+ methods
};

// ✅ GOOD: Focused classes
class UserManager { /* user operations */ };
class EmailService { /* email operations */ };
class PaymentProcessor { /* payment operations */ };
```

**Long Parameter Lists**
```cpp
// ❌ BAD: Too many parameters
void createUser(string name, string email, int age, 
                string address, string phone, string city,
                string zip, string country);

// ✅ GOOD: Use a struct/class
struct UserData {
    string name, email, address, phone;
    int age;
    // ... more fields
};
void createUser(const UserData& data);
```

#### 2. **Dispensables** (Can Be Removed)

**Dead Code**
```cpp
// ❌ BAD: Unused code
void oldFunction() {  // Nobody calls this anymore
    // ...
}

// Commented out code
// void doSomething() {
//     // Old implementation
// }
```

**Duplicate Code**
```cpp
// ❌ BAD: Repeated logic
void processStudent() {
    if (name.empty()) return;
    if (id <= 0) return;
    // process...
}

void processTeacher() {
    if (name.empty()) return;  // Same validation!
    if (id <= 0) return;        // Duplicated!
    // process...
}

// ✅ GOOD: Extract common logic
bool validatePerson(string name, int id) {
    return !name.empty() && id > 0;
}
```

**Speculative Generality**
```cpp
// ❌ BAD: Over-engineering for future "maybe" needs
class AbstractFactoryProviderManager {
    virtual AbstractFactory* getFactory() = 0;
    virtual void registerFactory(AbstractFactory*) = 0;
    // ... complex hierarchy nobody uses
};

// ✅ GOOD: Solve actual problems, not hypothetical ones
```

#### 3. **Change Preventers**

**Shotgun Surgery** - One change requires modifying many files
```cpp
// ❌ BAD: Tax rate scattered everywhere
// File1.cpp: amount * 0.19
// File2.cpp: price * 0.19
// File3.cpp: total * 0.19
// Change tax rate = edit 20 files!

// ✅ GOOD: Single source of truth
const double TAX_RATE = 0.19;
// Change once, works everywhere
```

**Divergent Change** - One class changed for many different reasons
```cpp
// ❌ BAD: Class has multiple reasons to change
class User {
    // Changes when database schema changes
    void saveToDB();
    
    // Changes when email template changes
    void sendEmail();
    
    // Changes when business rules change
    void validateData();
};

// ✅ GOOD: Each class has one reason to change
class User { /* just data */ };
class UserRepository { /* database */ };
class EmailService { /* email */ };
class UserValidator { /* validation */ };
```

#### 4. **Couplers** (Too Connected)

**High Coupling**
```cpp
// ❌ BAD: Everything depends on everything
class A {
    B b;
    C c;
    void doWork() { b.method(); c.method(); }
};

class B {
    A a;
    C c;
    void doWork() { a.method(); c.method(); }
};

// Changes cascade everywhere!

// ✅ GOOD: Low coupling through interfaces
class Component {
    IService& service;  // Depends on interface, not concrete class
    void doWork() { service.process(); }
};
```

**Global Variables**
```cpp
// ❌ BAD: Global mutable state
int globalCounter = 0;  // Anyone can modify this!

void function1() { globalCounter++; }
void function2() { globalCounter += 5; }
// Who changed it? When? Why?

// ✅ GOOD: Explicit dependencies
class Counter {
private:
    int count = 0;
public:
    void increment() { count++; }
    int getCount() const { return count; }
};
```

#### 5. **Object-Orientation Abusers**

**Refused Bequest** - Subclass doesn't use parent's methods
```cpp
// ❌ BAD: Bird can't fly, but inherits fly()
class Bird {
public:
    virtual void fly() { /* fly implementation */ }
};

class Penguin : public Bird {
public:
    void fly() override {
        throw runtime_error("Penguins can't fly!");
    }
};

// ✅ GOOD: Proper hierarchy
class Bird { /* common bird stuff */ };
class FlyingBird : public Bird {
    virtual void fly() = 0;
};
class FlightlessBird : public Bird { };
class Sparrow : public FlyingBird { };
class Penguin : public FlightlessBird { };
```

---

## 📊 Code Quality Metrics

### 1. Cyclomatic Complexity

**Definition:** Number of independent paths through code

**Formula:** Decision points + 1

```cpp
void simple() {          // Complexity: 1
    doSomething();
}

void withIf() {          // Complexity: 2
    if (condition) {     // +1 decision point
        doSomething();
    }
}

void complex() {         // Complexity: 5
    if (a) {            // +1
        doX();
    } else if (b) {     // +1
        doY();
    }
    
    for (int i = 0; i < n; i++) {  // +1
        doZ();
    }
    
    while (condition) {  // +1
        doW();
    }
}
```

**Guidelines:**
- ✅ **Good:** < 10
- ⚠️ **Warning:** 10-20
- ❌ **Bad:** > 20

**Why it matters:** High complexity = hard to understand, test, and maintain

### 2. Lines of Code (LOC)

**Guidelines:**
- **Functions:** < 50 lines (ideally 10-20)
- **Classes:** < 500 lines
- **Files:** < 1000 lines

**Why it matters:** Longer = harder to understand and maintain

### 3. Depth of Inheritance

**Guidelines:**
- ✅ **Good:** ≤ 3 levels
- ❌ **Bad:** > 5 levels

```cpp
// ❌ BAD: Too deep
class A { };
class B : public A { };
class C : public B { };
class D : public C { };
class E : public D { };  // Too deep!

// ✅ GOOD: Shallow hierarchy
class Base { };
class Derived : public Base { };
class Specialized : public Derived { };  // Max 3 levels
```

### 4. Coupling & Cohesion

**Coupling** - How much classes depend on each other
- **Goal:** Low coupling (independent modules)
- **Measure:** Count of dependencies

**Cohesion** - How related are methods in a class
- **Goal:** High cohesion (methods work together)
- **High cohesion:** Methods use same data, work toward same goal
- **Low cohesion:** Unrelated methods in same class

### 5. Code Coverage

**Definition:** % of code executed by tests

**Guidelines:**
- ✅ **Good:** > 80%
- ⭐ **Excellent:** > 90%
- ⚠️ **Warning:** < 70%

**Important:** 100% coverage doesn't guarantee quality! Test quality matters more than quantity.

### 6. Technical Debt Ratio

**Formula:** (Remediation Cost) / (Development Cost) × 100

**Guidelines:**
- ✅ **Good:** < 5%
- ⚠️ **Warning:** 5-10%
- ❌ **Bad:** > 10%

### 7. Code Duplication

**Definition:** % of code that is duplicated

**Guidelines:**
- ✅ **Good:** < 3%
- ⚠️ **Warning:** 3-5%
- ❌ **Bad:** > 5%

---

## 🎯 Common Anti-Patterns

### 1. Magic Numbers

```cpp
// ❌ BAD
if (status == 3) {  // What is 3?
    amount *= 0.9;  // What is 0.9?
}

// ✅ GOOD
const int STATUS_ACTIVE = 3;
const double DISCOUNT_RATE = 0.1;

if (status == STATUS_ACTIVE) {
    amount *= (1.0 - DISCOUNT_RATE);
}
```

### 2. Deep Nesting (Arrow Anti-Pattern)

```cpp
// ❌ BAD
if (a) {
    if (b) {
        if (c) {
            if (d) {
                // code drifts right
            }
        }
    }
}

// ✅ GOOD (Guard clauses)
if (!a) return;
if (!b) return;
if (!c) return;
if (!d) return;
// code stays flat
```

### 3. Comments as Crutch

```cpp
// ❌ BAD: Comments explain WHAT
// Calculate total
double calc(int x, int y) {  // x is quantity, y is price
    return x * y * 1.19;  // multiply by tax rate
}

// ✅ GOOD: Self-documenting
double calculateTotalWithTax(int quantity, int pricePerUnit) {
    const double TAX_RATE = 1.19;  // 19% VAT
    return quantity * pricePerUnit * TAX_RATE;
}
```

### 4. Premature Optimization

```cpp
// ❌ BAD: Overly optimized, cryptic
int calc(int* a, int n) {
    int r = 0;
    for (int i = 0; i < n; ++i)
        r += (a[i] << 1) - (a[i] >> 1);  // What??
    return r;
}

// ✅ GOOD: Clear and simple
int calculateSum(const vector<int>& numbers) {
    int sum = 0;
    for (int number : numbers) {
        sum += static_cast<int>(number * 1.5);
    }
    return sum;
}
// Compiler optimizes well. Profile before optimizing!
```

### 5. God Objects

```cpp
// ❌ BAD: One class does everything
class Application {
    // 50+ methods doing unrelated things
};

// ✅ GOOD: Focused, single-responsibility classes
class UserManager { /* user operations */ };
class DatabaseService { /* database operations */ };
class EmailService { /* email operations */ };
```

---

## ✅ Code Quality Checklist

### Before Committing Code

**Readability**
- [ ] Clear, descriptive names (no `x`, `tmp`, `data`)
- [ ] Functions < 50 lines (ideally 10-20)
- [ ] Max 3 levels of nesting
- [ ] Self-documenting code
- [ ] Consistent formatting
- [ ] Comments explain WHY, not WHAT

**Design**
- [ ] Single Responsibility Principle
- [ ] No god objects (< 500 LOC per class)
- [ ] Low coupling between modules
- [ ] High cohesion within modules
- [ ] Clear interfaces
- [ ] Proper abstraction levels

**Reliability**
- [ ] All errors handled
- [ ] Input validation
- [ ] No memory leaks (use RAII)
- [ ] Thread-safe (if multithreaded)
- [ ] Proper resource cleanup
- [ ] Exception-safe code

**Best Practices**
- [ ] Const correctness
- [ ] Smart pointers (no raw `new`/`delete`)
- [ ] Modern C++ features (`auto`, range-for)
- [ ] `nullptr` (not `NULL`)
- [ ] `enum class` (not plain `enum`)
- [ ] Pass by `const` reference for large objects

**Cleanliness**
- [ ] No magic numbers (use named constants)
- [ ] No code duplication (DRY)
- [ ] No dead code
- [ ] No commented-out code
- [ ] No global variables
- [ ] No deep nesting (use guard clauses)

**Testability**
- [ ] Functions are testable
- [ ] Dependencies injected (not hardcoded)
- [ ] Side effects minimized
- [ ] Pure functions where possible
- [ ] Mock-friendly design

---

## 💡 Golden Rules of Code Quality

### 1. **Code is Read 10x More Than Written**

Write for the reader, not the writer. Make it easy to understand.

### 2. **Simple is Better Than Clever**

```cpp
// Clever (bad)
int x = (a & 1) ? b : c;

// Simple (good)
int result = isOdd(number) ? valueIfOdd : valueIfEven;
```

### 3. **Explicit is Better Than Implicit**

Make dependencies and behaviors obvious.

### 4. **Fail Fast, Fail Clearly**

```cpp
// ✅ GOOD
if (input.empty()) {
    throw invalid_argument("Input cannot be empty");
}
```

### 5. **Don't Repeat Yourself (DRY)**

Every piece of knowledge should have a single representation.

### 6. **You Aren't Gonna Need It (YAGNI)**

Don't add functionality until you actually need it.

### 7. **Keep It Simple, Stupid (KISS)**

The simplest solution is usually the best.

---

## 🔧 Tools for Code Quality

### Static Analysis

**clang-tidy**
```bash
clang-tidy file.cpp -- -std=c++17
```
Finds bugs, style issues, performance problems

**cppcheck**
```bash
cppcheck --enable=all file.cpp
```
Detects undefined behavior, memory leaks

### Code Formatting

**clang-format**
```bash
clang-format -i file.cpp
```
Automatically formats code consistently

### Dynamic Analysis

**AddressSanitizer**
```bash
g++ -fsanitize=address file.cpp
```
Detects memory errors at runtime

**UndefinedBehaviorSanitizer**
```bash
g++ -fsanitize=undefined file.cpp
```
Catches undefined behavior

### Code Coverage

**gcov / lcov**
```bash
g++ --coverage file.cpp
./a.out
gcov file.cpp
```
Shows which lines are tested

---

## 📚 Code Review Guidelines

### What to Look For

**Critical Issues** (Must Fix)
- Memory leaks, undefined behavior
- Security vulnerabilities
- Logic errors, incorrect algorithms
- Crashes, deadlocks

**Important Issues** (Should Fix)
- Code smells (god objects, long methods)
- Missing error handling
- Poor naming
- High complexity

**Minor Issues** (Nice to Fix)
- Style inconsistencies
- Missing comments on complex logic
- Minor performance improvements
- TODOs

### How to Give Feedback

**❌ BAD:**
"This code is terrible"
"Why didn't you use X?"

**✅ GOOD:**
"Consider extracting this into a separate function for clarity"
"This could be simplified by using guard clauses"
"Have you considered the case where X is null?"

---

## 🎓 Learning Path

### Beginner
1. Learn to identify code smells
2. Practice writing clear, readable code
3. Use meaningful names
4. Keep functions small

### Intermediate
1. Apply SOLID principles
2. Master error handling
3. Learn design patterns
4. Understand coupling/cohesion

### Advanced
1. Design scalable architectures
2. Optimize performance intelligently
3. Lead code reviews
4. Establish team standards

---

## 🚀 Quick Reference

### Bad Code Signs

🚨 Functions > 50 lines  
🚨 Nesting > 3 levels  
🚨 Classes > 500 lines  
🚨 Magic numbers  
🚨 Global variables  
🚨 God objects  
🚨 Code duplication  
🚨 Poor naming  

### Quality Code Signs

✅ Clear intent  
✅ Single responsibility  
✅ No duplication  
✅ Proper error handling  
✅ Named constants  
✅ Small, focused functions  
✅ Low coupling  
✅ High cohesion  

---

## 📖 Recommended Reading

- **"Clean Code" by Robert C. Martin** - Essential reading
- **"Code Complete" by Steve McConnell** - Comprehensive guide
- **"Refactoring" by Martin Fowler** - Improving code design
- **"The Pragmatic Programmer" by Hunt & Thomas** - Best practices

---

## 🎯 Key Takeaways

1. **Quality matters more than speed** - You'll save time later
2. **Code smells warn you** - Learn to recognize them
3. **Simple beats clever** - Clear code wins
4. **Measure quality** - Use metrics to improve
5. **Review code** - Learn from others, teach others
6. **Refactor regularly** - Don't let debt accumulate
7. **Use tools** - Automate quality checks

---

## 💭 Remember

> "Any fool can write code that a computer can understand.  
> Good programmers write code that humans can understand."  
> — Martin Fowler

> "The ratio of time spent reading versus writing is well over 10 to 1.  
> We are constantly reading old code as part of the effort to write new code.  
> Making it easy to read makes it easier to write."  
> — Robert C. Martin

---

## 🎉 Next Steps

After mastering code quality basics, explore:
- **Code Style & Formatting** - Consistent, professional style
- **Design Principles** - SOLID, DRY, KISS in depth
- **Best Practices** - C++ specific best practices
- **Refactoring** - Improving existing code
- **Testing** - Writing quality tests

## Notes moved from main.cpp

This text used to be printed by `main()`. It is lecture material, so it lives here and the program only prints the demo output.

### 🎉 CODE QUALITY BASICS COMPLETE! 🎉 (closing summary)

You've learned what makes quality code and how  
to identify and fix common code smells!

Next: Dive deeper into specific quality topics!

### Text printed by the example functions

This text used to be printed between the demo steps. It is lecture material, so it lives here.

### From `example1_magic_numbers()`

- ❌ BAD CODE:

```cpp

```cpp
void processOrder(double amount) {
    if (amount > 1000) {
        amount *= 0.9;  // What is 0.9?
    }
    double tax = amount * 0.19;  // What is 0.19?
    double shipping = amount > 50 ? 0 : 5.99;  // What are these?
}
```

```

**🚨 PROBLEMS:**

  - 0.9, 0.19, 50, 5.99 - what do they mean?
  - Hard to maintain (what if tax rate changes?)
  - Can't understand without context
  - Easy to make typos (0.19 vs 0.91)

- ✅ GOOD CODE:

```cpp

```cpp
const double BULK_ORDER_THRESHOLD = 1000.0;
const double BULK_DISCOUNT_RATE = 0.1;  // 10% off
const double TAX_RATE = 0.19;           // 19% VAT
const double FREE_SHIPPING_MIN = 50.0;
const double STANDARD_SHIPPING = 5.99;

void processOrder(double amount) {
    if (amount > BULK_ORDER_THRESHOLD) {
        amount *= (1.0 - BULK_DISCOUNT_RATE);
    }
    double tax = amount * TAX_RATE;
    double shipping = amount > FREE_SHIPPING_MIN ? 0 : STANDARD_SHIPPING;
}
```

```

**✨ IMPROVEMENTS:**

- ✅ Self-documenting - clear meaning
- ✅ Easy to maintain - change in one place
- ✅ No confusion about values
- ✅ Less prone to typos

- 💡 KEY PRINCIPLE:

"Every magic number should be a named constant!"

### From `example2_long_functions()`

- ❌ BAD CODE (100+ lines in one function):

```cpp

```cpp
void processUserData() {
    // Validate input (20 lines)
    if (name.empty()) { /* error */ }
    if (email.empty()) { /* error */ }
    if (!isValidEmail(email)) { /* error */ }
    // ... more validation

    // Parse data (30 lines)
    string firstName = extractFirstName(name);
    string lastName = extractLastName(name);
    // ... more parsing

    // Calculate values (20 lines)
    double discount = calculateDiscount(...);
    double tax = calculateTax(...);
    // ... more calculations

    // Save to database (30 lines)
    connection.open();
    // ... database operations
    connection.close();
}
```

```

**🚨 PROBLEMS:**

  - Hard to understand - too much at once
  - Difficult to test - tests everything
  - Hard to reuse - can't use parts separately
  - High complexity - many responsibilities
  - Difficult to debug - where is the problem?

- ✅ GOOD CODE (Multiple small functions):

```cpp

```cpp
bool validateUserInput(const User& user) {
    return !user.name.empty() &&
           !user.email.empty() &&
           isValidEmail(user.email);
}

UserData parseUserData(const User& user) {
    UserData data;
    data.firstName = extractFirstName(user.name);
    data.lastName = extractLastName(user.name);
    return data;
}

OrderSummary calculateOrderDetails(const Order& order) {
    OrderSummary summary;
    summary.discount = calculateDiscount(order);
    summary.tax = calculateTax(order);
    return summary;
}

void saveToDatabase(const UserData& data) {
    DatabaseConnection conn;
    conn.save(data);
}

void processUserData(const User& user) {
    if (!validateUserInput(user)) return;
    auto userData = parseUserData(user);
    auto orderDetails = calculateOrderDetails(user.order);
    saveToDatabase(userData);
}
```

```

**✨ IMPROVEMENTS:**

- ✅ Each function has ONE clear purpose
- ✅ Easy to understand - clear names
- ✅ Easy to test - test each part separately
- ✅ Reusable - use validation elsewhere
- ✅ Low complexity - simple logic

- 💡 KEY PRINCIPLE:

"Functions should be small (10-20 lines) and do ONE thing!"

### From `example3_unclear_naming()`

- ❌ BAD CODE:

```cpp

```cpp
int d;           // What is d?
string s;        // What is s?
double calc(int x, int y) {  // Calculate what?
    int tmp = x * y;         // Temporary what?
    return tmp * 1.19;
}

void proc(vector<int>& v) {  // Process what?
    for (auto& i : v) {      // i is what?
        i *= 2;
    }
}

class Data {                 // What data?
    int val1, val2;          // What values?
    void do_it();            // Do what?
};
```

```

**🚨 PROBLEMS:**

  - d, s, x, y - meaningless single letters
  - tmp - temporary what?
  - calc(), proc() - vague verbs
  - Data, val1 - generic names
  - Need comments to understand

- ✅ GOOD CODE:

```cpp

```cpp
int daysUntilExpiry;
string customerEmail;

double calculatePriceWithTax(int quantity, int pricePerUnit) {
    int subtotal = quantity * pricePerUnit;
    const double TAX_RATE = 1.19;  // 19% VAT
    return subtotal * TAX_RATE;
}

void doubleAllValues(vector<int>& numbers) {
    for (auto& number : numbers) {
        number *= 2;
    }
}

class CustomerAccount {
    int accountBalance;
    int creditLimit;
    void processPayment();
};
```

```

**✨ IMPROVEMENTS:**

- ✅ Self-explanatory names
- ✅ Clear purpose and intent
- ✅ No comments needed
- ✅ Easy to understand
- ✅ Professional code

- 💡 KEY PRINCIPLE:

"Names should reveal intent - code should read like prose!"

### From `example4_god_object()`

- ❌ BAD CODE (God Object):

```cpp

```cpp
class Application {
```

**public:**

```cpp
    // User management
    void createUser();
    void deleteUser();
    void updateUser();
    // Database operations
    void connectDatabase();
    void executeQuery();
    void closeDatabase();
    // Email handling
    void sendEmail();
    void validateEmail();
    // Payment processing
    void processPayment();
    void refundPayment();
    // Logging
    void logError();
    void logInfo();
    // Configuration
    void loadConfig();
    void saveConfig();
    // ... 50+ more methods!
};
```

```

**🚨 PROBLEMS:**

  - One class doing EVERYTHING
  - 1000+ lines of code
  - Hard to test - tests everything
  - Hard to modify - affects everything
  - Multiple responsibilities
  - Impossible to reuse parts

- ✅ GOOD CODE (Separated Responsibilities):

```cpp

```cpp
class UserManager {
```

**public:**

```cpp
    void createUser(const User& user);
    void deleteUser(int userId);
    void updateUser(const User& user);
};

class DatabaseConnection {
```

**public:**

```cpp
    void connect(const string& connectionString);
    QueryResult execute(const string& query);
    void close();
};

class EmailService {
```

**public:**

```cpp
    void send(const Email& email);
    bool validate(const string& emailAddress);
};

class PaymentProcessor {
```

**public:**

```cpp
    bool process(const Payment& payment);
    bool refund(int transactionId);
};

class Logger {
```

**public:**

```cpp
    void error(const string& message);
    void info(const string& message);
};

class ConfigurationManager {
```

**public:**

```cpp
    void load(const string& path);
    void save(const string& path);
};
```

```

**✨ IMPROVEMENTS:**

- ✅ Each class has ONE responsibility
- ✅ Easy to test - test each separately
- ✅ Easy to modify - changes are isolated
- ✅ Reusable - use EmailService anywhere
- ✅ Clear purpose - obvious what each does

- 💡 KEY PRINCIPLE (Single Responsibility):

"A class should have only ONE reason to change!"

### From `example5_deep_nesting()`

- ❌ BAD CODE (Arrow Anti-Pattern):

```cpp

```cpp
void processOrder(Order order) {
    if (order.isValid()) {
        if (order.hasItems()) {
            if (order.customer.hasCredit()) {
                if (order.shippingAddress.isValid()) {
                    if (inventory.hasStock(order)) {
                        if (payment.process(order)) {
                            shipOrder(order);
                            sendConfirmation(order);
                        } else {
                            handlePaymentError();
                        }
                    } else {
                        notifyOutOfStock();
                    }
                } else {
                    requestValidAddress();
                }
            } else {
                rejectLowCredit();
            }
        } else {
            notifyEmptyOrder();
        }
    } else {
        rejectInvalidOrder();
    }
}
```

```

**🚨 PROBLEMS:**

  - 6 levels of nesting - hard to follow
  - 'Arrow' shape - code drifts to the right
  - Hard to read and understand
  - High cyclomatic complexity
  - Difficult to test all paths

- ✅ GOOD CODE (Guard Clauses):

```cpp

```cpp
void processOrder(Order order) {
    // Guard clauses - fail fast
    if (!order.isValid()) {
        rejectInvalidOrder();
        return;
    }
    if (!order.hasItems()) {
        notifyEmptyOrder();
        return;
    }
    if (!order.customer.hasCredit()) {
        rejectLowCredit();
        return;
    }
    if (!order.shippingAddress.isValid()) {
        requestValidAddress();
        return;
    }
    if (!inventory.hasStock(order)) {
        notifyOutOfStock();
        return;
    }
    if (!payment.process(order)) {
        handlePaymentError();
        return;
    }
    // Happy path - all validations passed
    shipOrder(order);
    sendConfirmation(order);
}
```

```

**✨ IMPROVEMENTS:**

- ✅ Flat structure - no deep nesting
- ✅ Easy to read - linear flow
- ✅ Clear error handling
- ✅ Happy path at the end
- ✅ Low complexity

- 💡 KEY PRINCIPLE:

"Fail fast with guard clauses - avoid the arrow!"

### From `example6_code_duplication()`

- ❌ BAD CODE (Repeated Logic):

```cpp

```cpp
double calculateCircleArea(double radius) {
    return 3.14159 * radius * radius;
}

double calculateCircleCircumference(double radius) {
    return 2 * 3.14159 * radius;  // PI duplicated!
}

double calculateSphereVolume(double radius) {
    return (4.0/3.0) * 3.14159 * radius * radius * radius;  // PI again!
}

void processStudentGrade(Student s) {
    if (s.name.empty()) {
        cout << "Error: Name is required" << endl;
        return;
    }
    if (s.id <= 0) {
        cout << "Error: Invalid ID" << endl;
        return;
    }
    // Process student...
}

void processTeacherData(Teacher t) {
    if (t.name.empty()) {  // Same validation!
        cout << "Error: Name is required" << endl;
        return;
    }
    if (t.id <= 0) {  // Same validation!
        cout << "Error: Invalid ID" << endl;
        return;
    }
    // Process teacher...
}
```

```

**🚨 PROBLEMS:**

  - PI value repeated 3 times (what if it's wrong?)
  - Validation logic duplicated
  - Bug fixes needed in multiple places
  - Maintenance nightmare
  - Inconsistency risk

- ✅ GOOD CODE (Extract Common Logic):

```cpp

```cpp
// Single definition of PI
const double PI = 3.14159265358979323846;

double calculateCircleArea(double radius) {
    return PI * radius * radius;
}

double calculateCircleCircumference(double radius) {
    return 2 * PI * radius;
}

double calculateSphereVolume(double radius) {
    return (4.0/3.0) * PI * radius * radius * radius;
}

// Base class with common validation
class Person {
```

**protected:**

```cpp
    bool validateBasicInfo() const {
        if (name.empty()) {
            cout << "Error: Name is required" << endl;
            return false;
        }
        if (id <= 0) {
            cout << "Error: Invalid ID" << endl;
            return false;
        }
        return true;
    }
    string name;
    int id;
};

class Student : public Person {
    void process() {
        if (!validateBasicInfo()) return;
        // Process student...
    }
};

class Teacher : public Person {
    void process() {
        if (!validateBasicInfo()) return;
        // Process teacher...
    }
};
```

```

**✨ IMPROVEMENTS:**

- ✅ Single source of truth (PI defined once)
- ✅ Validation logic in ONE place
- ✅ Bug fix needed only once
- ✅ Consistent behavior
- ✅ Easy to maintain

- 💡 KEY PRINCIPLE (DRY):

"Don't Repeat Yourself - every piece of knowledge  
should have a single representation!"

### From `example7_poor_error_handling()`

- ❌ BAD CODE (Ignoring Errors):

```cpp

```cpp
void readFile(const string& filename) {
    ifstream file(filename);
    // No check if file opened!
    string line;
    getline(file, line);  // Might fail!
    processLine(line);
}

int divide(int a, int b) {
    return a / b;  // Division by zero!
}

void allocateMemory() {
    int* ptr = new int[1000000000];  // Might fail!
    // Use ptr...
    delete[] ptr;
}
```

```

**🚨 PROBLEMS:**

  - No file opening check - crash!
  - Division by zero - undefined behavior!
  - No allocation failure check
  - Silent failures
  - Unpredictable behavior

- ✅ GOOD CODE (Proper Error Handling):

```cpp

```cpp
void readFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Cannot open file: " + filename);
    }
    string line;
    if (!getline(file, line)) {
        throw runtime_error("Failed to read from file");
    }
    processLine(line);
}

int divide(int a, int b) {
    if (b == 0) {
        throw invalid_argument("Division by zero");
    }
    return a / b;
}

void allocateMemory() {
    // Use smart pointer - automatic cleanup
    try {
        auto ptr = make_unique<int[]>(1000000000);
        // Use ptr...
    } catch (const bad_alloc& e) {
        cerr << "Memory allocation failed: " << e.what() << endl;
        throw;
    }
}
```

```

**✨ IMPROVEMENTS:**

- ✅ Check all operations that can fail
- ✅ Throw exceptions with clear messages
- ✅ Use RAII (smart pointers) for safety
- ✅ Handle errors gracefully
- ✅ Predictable behavior

- 💡 KEY PRINCIPLE:

"Always check operations that can fail -  
handle errors gracefully!"

### From `example8_comments_as_crutch()`

- ❌ BAD CODE (Needs Comments):

```cpp

```cpp
// Calculate total
double calc(vector<int>& v, bool f) {
    double t = 0;  // total
    for (auto i : v) {  // loop through items
        if (f) {  // if flag is true
            t += i * 1.19;  // add with tax
        } else {
            t += i;  // add without tax
        }
    }
    return t;
}

// Check user
bool chk(User u) {
    // Check if name is empty
    if (u.n.empty()) return false;
    // Check if age is valid
    if (u.a < 18) return false;
    // Check if email is valid
    if (u.e.find('@') == string::npos) return false;
    return true;
}
```

```

**🚨 PROBLEMS:**

  - Comments explain WHAT, not WHY
  - Comments compensate for bad names
  - Code is not self-documenting
  - Comments can become outdated
  - Extra maintenance burden

- ✅ GOOD CODE (Self-Documenting):

```cpp

```cpp
const double TAX_RATE = 1.19;  // 19% VAT

double calculateTotal(const vector<int>& prices, bool includeTax) {
    double total = 0.0;
    for (int price : prices) {
        if (includeTax) {
            total += price * TAX_RATE;
        } else {
            total += price;
        }
    }
    return total;
}

const int MINIMUM_AGE = 18;

bool isValidUser(const User& user) {
    bool hasName = !user.name.empty();
    bool isAdult = user.age >= MINIMUM_AGE;
    bool hasValidEmail = user.email.find('@') != string::npos;
    return hasName && isAdult && hasValidEmail;
}
```

```

**✨ IMPROVEMENTS:**

- ✅ Clear function names - no comment needed
- ✅ Descriptive variable names
- ✅ Self-explanatory logic
- ✅ Comments only for WHY, not WHAT
- ✅ Code tells the story

- 💡 KEY PRINCIPLE:

"Good code is self-documenting - comments should  
explain WHY, not WHAT!"

### From `example9_premature_optimization()`

- ❌ BAD CODE (Premature Optimization):

```cpp

```cpp
// Trying to be 'fast' but sacrificing readability
int calc(int* a, int n) {
    int r = 0;
    for (int i = 0; i < n; ++i) {
        r += (a[i] << 1) - (a[i] >> 1);  // What does this do??
    }
    return r;
}

// Manual loop unrolling for 'speed'
void proc(vector<int>& v) {
    int n = v.size();
    int i = 0;
    // Process 4 at a time
    for (; i + 4 <= n; i += 4) {
        v[i] *= 2;
        v[i+1] *= 2;
        v[i+2] *= 2;
        v[i+3] *= 2;
    }
    // Handle remainder
    for (; i < n; ++i) {
        v[i] *= 2;
    }
}
```

```

**🚨 PROBLEMS:**

  - Bit shifting makes code cryptic
  - Manual loop unrolling is complex
  - Hard to maintain
  - Compiler can optimize better!
  - Premature - did you measure?

- ✅ GOOD CODE (Clear and Simple):

```cpp

```cpp
int calculateSum(const vector<int>& numbers) {
    int sum = 0;
    for (int number : numbers) {
        // 2*x - x/2 = 1.5*x
        sum += static_cast<int>(number * 1.5);
    }
    return sum;
}

void doubleAllValues(vector<int>& numbers) {
    // Simple and clear - compiler will optimize
    for (int& number : numbers) {
        number *= 2;
    }
    // Or use STL algorithm
    // transform(numbers.begin(), numbers.end(), numbers.begin(),
    //          [](int n) { return n * 2; });
}
```

```

**✨ IMPROVEMENTS:**

- ✅ Clear, readable code
- ✅ Easy to understand intent
- ✅ Easy to maintain
- ✅ Compiler optimizes well
- ✅ Profile before optimizing!

- 💡 KEY PRINCIPLE (Donald Knuth):

"Premature optimization is the root of all evil.  
Write clear code first, optimize bottlenecks later!"

### From `example10_global_state()`

- ❌ BAD CODE (Global Mutable State):

```cpp

```cpp
// Global variables (bad!)
int userCount = 0;
string currentUser = "";
bool isLoggedIn = false;
vector<string> errors;

void loginUser(string name) {
    currentUser = name;  // Modifies global
    isLoggedIn = true;   // Modifies global
    userCount++;         // Modifies global
}

void processData() {
    if (isLoggedIn) {  // Depends on global
        // Process for currentUser...
    } else {
        errors.push_back("Not logged in");  // Modifies global
    }
}

void showErrors() {
    for (const auto& err : errors) {  // Depends on global
        cout << err << endl;
    }
}
```

```

**🚨 PROBLEMS:**

  - Hidden dependencies - hard to track
  - Hard to test - state persists
  - Threading issues - race conditions
  - Tight coupling - everything connected
  - Side effects everywhere

- ✅ GOOD CODE (Encapsulated State):

```cpp

```cpp
class UserSession {
```

**private:**

```cpp
string currentUser;
bool isLoggedIn;
vector<string> errors;
```

**public:**

```cpp
    UserSession() : isLoggedIn(false) {}
    void login(const string& name) {
        currentUser = name;
        isLoggedIn = true;
    }
    bool isActive() const { return isLoggedIn; }
    string getUser() const { return currentUser; }
    void addError(const string& error) {
        errors.push_back(error);
    }
    vector<string> getErrors() const { return errors; }
};

class DataProcessor {
```

**public:**

```cpp
    void process(UserSession& session) {  // Explicit dependency
        if (session.isActive()) {
            // Process for session.getUser()...
        } else {
            session.addError("Not logged in");
        }
    }
};
```

```

**✨ IMPROVEMENTS:**

- ✅ Encapsulated state - no globals
- ✅ Explicit dependencies - clear what's needed
- ✅ Easy to test - create session, test
- ✅ Thread-safe - each session separate
- ✅ Low coupling - classes independent

- 💡 KEY PRINCIPLE:

"Avoid global mutable state - pass dependencies explicitly!"

### From `example11_quality_metrics()`

**📊 KEY QUALITY METRICS:**

1. CYCLOMATIC COMPLEXITY:
  - Measures number of independent paths
  - Formula: E - N + 2P (edges - nodes + 2*connected components)
  - Or: Count decision points + 1
  - ✅ Good: < 10
  - ⚠️  Warning: 10-20
  - ❌ Bad: > 20

Example:

```cpp
void simple() { }                    // Complexity: 1
void withIf() { if(...) {...} }      // Complexity: 2
void complex() {
    if(...) {...}                    // +1
    else if(...) {...}               // +1
    for(...) {...}                   // +1
    while(...) {...}                 // +1
}                                    // Complexity: 5
```

2. LINES OF CODE (LOC):
  - Functions: ✅ < 50 lines (ideally 10-20)
  - Classes: ✅ < 500 lines
  - Files: ✅ < 1000 lines
  - If longer → split into smaller pieces

3. DEPTH OF INHERITANCE:
  - ✅ Good: ≤ 3 levels
  - ❌ Bad: > 5 levels
  - Deep hierarchies are hard to understand

4. COUPLING:
  - Afferent Coupling (Ca): Who depends on this?
  - Efferent Coupling (Ce): What does this depend on?
  - ✅ Goal: Low coupling (independent modules)

5. COHESION:
  - How related are methods in a class?
  - ✅ High cohesion: Methods work together
  - ❌ Low cohesion: Unrelated methods

6. CODE COVERAGE:
  - ✅ Good: > 80%
  - ⭐ Excellent: > 90%
  - But: 100% doesn't guarantee quality!

7. TECHNICAL DEBT RATIO:
  - Remediation cost / Development cost
  - ✅ Good: < 5%
  - ⚠️  Warning: 5-10%
  - ❌ Bad: > 10%

8. CODE DUPLICATION:
  - ✅ Good: < 3%
  - ⚠️  Warning: 3-5%
  - ❌ Bad: > 5%

- 💡 REMEMBER:

"Metrics are indicators, not goals!  
Use them to identify problems, not as targets."

### From `example12_quality_checklist()`

- ✅ CODE QUALITY CHECKLIST:

**📝 READABILITY:**

□ Clear, descriptive names (no x, tmp, data)  
□ Functions < 50 lines (ideally 10-20)  
□ Max 3 levels of nesting  
□ Self-documenting code  
□ Consistent formatting  
□ Comments explain WHY, not WHAT

**🏗️  DESIGN:**

□ Single Responsibility Principle  
□ No god objects (< 500 LOC)  
□ Low coupling between modules  
□ High cohesion within modules  
□ Clear interfaces  
□ Proper abstraction levels

**🔒 RELIABILITY:**

□ All errors handled  
□ Input validation  
□ No memory leaks (use RAII)  
□ Thread-safe (if multithreaded)  
□ Proper resource cleanup  
□ Exception-safe code

**🎯 BEST PRACTICES:**

□ Const correctness  
□ Smart pointers (no raw new/delete)  
□ Modern C++ features (auto, range-for)  
□ nullptr (not NULL)  
□ enum class (not plain enum)  
□ Pass by const reference for large objects

**🧹 CLEANLINESS:**

□ No magic numbers (use named constants)  
□ No code duplication (DRY)  
□ No dead code  
□ No commented-out code  
□ No global variables  
□ No deep nesting (use guard clauses)

**🧪 TESTABILITY:**

□ Functions are testable  
□ Dependencies injected (not hardcoded)  
□ Side effects minimized  
□ Pure functions where possible  
□ Mock-friendly design

**⚡ PERFORMANCE:**

□ No premature optimization  
□ Move semantics where appropriate  
□ Pass by const reference (large objects)  
□ Reserve vector capacity if known  
□ Avoid unnecessary copies

**📚 DOCUMENTATION:**

□ Public API documented  
□ Complex algorithms explained  
□ Design decisions documented  
□ README for project/module  
□ Examples provided

**🔧 TOOLS:**

□ Use linter (clang-tidy)  
□ Use formatter (clang-format)  
□ Static analysis  
□ Code coverage  
□ Sanitizers (Address, UB)

- 💡 GOLDEN RULES:
1. Code is read 10x more than written
2. Simple is better than clever
3. Explicit is better than implicit
4. Fail fast, fail clearly
5. Don't repeat yourself (DRY)
6. You aren't gonna need it (YAGNI)
7. Keep it simple, stupid (KISS)

## What was tested

- Compiled with GCC 13.3 (`-std=c++23`) using a stand-in `<print>` header, because `std::println` needs GCC 14 and it was not available here. The source files were compiled directly, not through CMake.
- Run, and the output matches the original `std::cout` version except that the lecture text moved into *Notes moved from main.cpp* is no longer printed (and the remaining example titles were renumbered where an example was removed).
- Run once with AddressSanitizer + UndefinedBehaviorSanitizer: no findings.

## Not tested

- The real `<print>` header (GCC 14+, recent Clang or MSVC) and platforms other than Linux x86-64.
