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