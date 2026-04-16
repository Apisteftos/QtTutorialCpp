# Design Principles - Complete Guide

## 📚 Overview

This repository contains comprehensive examples and documentation for essential **Software Design Principles** that every developer should master. These principles form the foundation of clean, maintainable, and scalable software.

> **"Good design is obvious. Great design is transparent."**  
> — Joe Sparano

---

## 🎯 What Are Design Principles?

Design principles are **guidelines** that help us:
- ✅ Write maintainable code
- ✅ Create flexible architectures
- ✅ Reduce technical debt
- ✅ Build scalable systems
- ✅ Improve code quality

They are **not** strict rules, but rather proven best practices that guide better design decisions.

---

## 📖 Principles Covered

### 🏛️ SOLID Principles (The Foundation)

1. **[Single Responsibility Principle (SRP)](#1-single-responsibility-principle-srp)**
2. **[Open/Closed Principle (OCP)](#2-openclosed-principle-ocp)**
3. **[Liskov Substitution Principle (LSP)](#3-liskov-substitution-principle-lsp)**
4. **[Interface Segregation Principle (ISP)](#4-interface-segregation-principle-isp)**
5. **[Dependency Inversion Principle (DIP)](#5-dependency-inversion-principle-dip)**

### 🎨 Additional Core Principles

6. **[DRY - Don't Repeat Yourself](#6-dry---dont-repeat-yourself)**
7. **[KISS - Keep It Simple, Stupid](#7-kiss---keep-it-simple-stupid)**
8. **[High Cohesion](#8-high-cohesion)**

---

## 1. Single Responsibility Principle (SRP)

> **"A class should have one, and only one, reason to change."**

### What It Means
- Each class/module does **one thing** well
- One reason to change
- Focused responsibility

### Example
```cpp
// ❌ BAD: Multiple responsibilities
class User {
    void saveToDatabase();      // Database responsibility
    void sendEmail();           // Email responsibility
    void generateReport();      // Reporting responsibility
};

// ✅ GOOD: Single responsibility
class User { /* user data only */ };
class UserRepository { void save(User&); };
class EmailService { void send(User&); };
class ReportGenerator { void generate(User&); };
```

### Benefits
- ✅ Easier to understand
- ✅ Easier to modify
- ✅ Easier to test
- ✅ More reusable

📁 **[View detailed examples →](01_SingleResponsibility/)**

---

## 2. Open/Closed Principle (OCP)

> **"Software entities should be open for extension, but closed for modification."**

### What It Means
- Add new features **without changing** existing code
- Use **abstraction** and **polymorphism**
- Extend behavior through new classes

### Example
```cpp
// ❌ BAD: Modify existing code for new shapes
class AreaCalculator {
    double calculate(Shape& shape) {
        if (shape.type == "circle") { /* ... */ }
        else if (shape.type == "rectangle") { /* ... */ }
        // Need to modify for new shapes!
    }
};

// ✅ GOOD: Extend through inheritance
class Shape {
    virtual double getArea() = 0;
};

class Circle : public Shape {
    double getArea() override { /* ... */ }
};

class Rectangle : public Shape {
    double getArea() override { /* ... */ }
};
// Add new shapes without modifying existing code!
```

### Benefits
- ✅ Stable codebase
- ✅ Easy to extend
- ✅ Reduced bugs
- ✅ Better maintainability

📁 **[View detailed examples →](02_OpenClosed/)**

---

## 3. Liskov Substitution Principle (LSP)

> **"Objects of a superclass should be replaceable with objects of a subclass without breaking the application."**

### What It Means
- Subclass must be **substitutable** for parent
- No surprising behavior changes
- Honor parent's contract

### Example
```cpp
// ❌ BAD: Square violates Rectangle's contract
class Rectangle {
    virtual void setWidth(int w) { width = w; }
    virtual void setHeight(int h) { height = h; }
};

class Square : public Rectangle {
    void setWidth(int w) override {
        width = w;
        height = w;  // Violates expectation!
    }
};

// ✅ GOOD: Separate hierarchies
class Shape { virtual double getArea() = 0; };
class Rectangle : public Shape { /* ... */ };
class Square : public Shape { /* ... */ };
```

### Benefits
- ✅ Reliable polymorphism
- ✅ Predictable behavior
- ✅ Type safety
- ✅ Correct inheritance

📁 **[View detailed examples →](03_LiskovSubstitution/)**

---

## 4. Interface Segregation Principle (ISP)

> **"No client should be forced to depend on methods it does not use."**

### What It Means
- Many **small, focused** interfaces
- No fat interfaces
- Clients depend only on what they need

### Example
```cpp
// ❌ BAD: Fat interface
class IMachine {
    virtual void print() = 0;
    virtual void scan() = 0;
    virtual void fax() = 0;
    virtual void staple() = 0;
};

// SimplePrinter forced to implement all!

// ✅ GOOD: Segregated interfaces
class IPrinter { virtual void print() = 0; };
class IScanner { virtual void scan() = 0; };
class IFax { virtual void fax() = 0; };
class IStapler { virtual void staple() = 0; };

class SimplePrinter : public IPrinter { /* only print */ };
class MultiFunctionPrinter : public IPrinter, IScanner, IFax { };
```

### Benefits
- ✅ No forced implementations
- ✅ Better flexibility
- ✅ Reduced coupling
- ✅ Cleaner code

📁 **[View detailed examples →](04_InterfaceSegregation/)**

---

## 5. Dependency Inversion Principle (DIP)

> **"Depend upon abstractions, not concretions."**  
> **"High-level modules should not depend on low-level modules. Both should depend on abstractions."**

### What It Means
- Depend on **interfaces**, not concrete classes
- High-level and low-level both depend on abstractions
- Invert the dependency direction

### Example
```cpp
// ❌ BAD: Depends on concrete class
class UserRepository {
    MySQLDatabase database;  // Tight coupling!
};

// ✅ GOOD: Depends on abstraction
class IDatabase {
    virtual void query() = 0;
};

class MySQLDatabase : public IDatabase { /* ... */ };
class PostgreSQLDatabase : public IDatabase { /* ... */ };

class UserRepository {
    IDatabase* database;  // Depends on interface!
public:
    UserRepository(IDatabase* db) : database(db) {}
};
```

### Benefits
- ✅ Flexible architecture
- ✅ Easy to swap implementations
- ✅ Testable with mocks
- ✅ Reduced coupling

📁 **[View detailed examples →](05_DependencyInversionPrinciple/)**

---

## 6. DRY - Don't Repeat Yourself

> **"Every piece of knowledge must have a single, unambiguous, authoritative representation within a system."**

### What It Means
- **Don't duplicate** code, logic, or knowledge
- **Single source of truth** for each piece of information
- Extract common patterns

### Example
```cpp
// ❌ BAD: Duplicated logic
void processOrder1() {
    double tax = amount * 0.1;  // Tax rate duplicated
    // ...
}

void processOrder2() {
    double tax = amount * 0.1;  // Tax rate duplicated
    // ...
}

// ✅ GOOD: Single source of truth
const double TAX_RATE = 0.10;

double calculateTax(double amount) {
    return amount * TAX_RATE;  // Defined once
}
```

### Benefits
- ✅ Easier maintenance (change once)
- ✅ Fewer bugs (fix once)
- ✅ Better readability
- ✅ Consistent behavior

📁 **[View detailed examples →](06_Dry/)**

---

## 7. KISS - Keep It Simple, Stupid

> **"Simplicity should be a key goal in design, and unnecessary complexity should be avoided."**

### What It Means
- Favor **simple solutions** over complex ones
- Avoid **over-engineering**
- Clarity over cleverness

### Example
```cpp
// ❌ BAD: Over-complicated
class Operation { virtual double execute(double, double) = 0; };
class AddOperation : public Operation { /* ... */ };
class OperationFactory { /* ... */ };
class Calculator { OperationFactory factory; };
// 6 classes for simple addition!

// ✅ GOOD: Simple and clear
double add(double a, double b) { return a + b; }
double subtract(double a, double b) { return a - b; }

double calculate(const string& op, double a, double b) {
    if (op == "add") return add(a, b);
    if (op == "subtract") return subtract(a, b);
    return 0;
}
```

### Benefits
- ✅ Easier to understand
- ✅ Fewer bugs
- ✅ Faster development
- ✅ Better maintainability

📁 **[View detailed examples →](07_Kiss/)**

---

## 8. High Cohesion

> **"A class should have high cohesion - all its methods and data should be closely related and work toward a single purpose."**

### What It Means
- **Group related functionality** together
- Strong relationship between class members
- Each class has single, well-defined purpose

### Example
```cpp
// ❌ BAD: Low cohesion (God class)
class UserManager {
    // User data
    // Database operations
    // Email operations
    // Logging operations
    // Validation operations
    // Everything mixed together!
};

// ✅ GOOD: High cohesion (focused classes)
class User { /* user data only */ };
class UserRepository { /* database only */ };
class EmailService { /* email only */ };
class Validator { /* validation only */ };
class Logger { /* logging only */ };
```

### Benefits
- ✅ Easier to understand
- ✅ Easier to maintain
- ✅ More reusable
- ✅ Easier to test

📁 **[View detailed examples →](11_HighCohesion/)**

---

## 🔗 How Principles Work Together

### SOLID as a Complete System

```
┌─────────────────────────────────────────────┐
│         SOLID PRINCIPLES                    │
├─────────────────────────────────────────────┤
│                                             │
│  SRP → One responsibility per class         │
│   ↓                                         │
│  OCP → Extend without modifying             │
│   ↓                                         │
│  LSP → Subclasses truly substitutable       │
│   ↓                                         │
│  ISP → Small, focused interfaces            │
│   ↓                                         │
│  DIP → Depend on abstractions               │
│                                             │
│  Together = Well-designed system            │
└─────────────────────────────────────────────┘
```

### Principle Relationships

```
High Cohesion + Low Coupling = Good Design
     ↓              ↓
   Related      Independent
  together       modules

DRY + KISS = Simple, non-repetitive code
 ↓      ↓
No      No
duplication  complexity

SOLID + DRY + KISS = Professional codebase
```

### Common Patterns

**Building a Feature:**
1. **SRP**: One class per responsibility
2. **OCP**: Design for extension
3. **DIP**: Depend on interfaces
4. **DRY**: Extract common code
5. **KISS**: Keep it simple
6. **High Cohesion**: Group related items

---

## 🎯 Quick Reference Cheat Sheet

| Principle | Key Question | Red Flag |
|-----------|--------------|----------|
| **SRP** | Does this class have one reason to change? | "Manager", "Handler" in name |
| **OCP** | Can I add features without modifying? | Lots of if/else for types |
| **LSP** | Can I substitute child for parent? | Type checking, empty methods |
| **ISP** | Do clients depend only on what they use? | Fat interfaces, forced implementations |
| **DIP** | Do I depend on abstractions? | `new ConcreteClass()` everywhere |
| **DRY** | Is this knowledge in one place? | Copy-pasted code |
| **KISS** | Is this the simplest solution? | Hard to explain |
| **High Cohesion** | Do all methods work toward same goal? | Utility classes, unrelated methods |

---

## 📊 Design Principles Checklist

### Before Writing Code

- [ ] **SRP**: Does each class have single responsibility?
- [ ] **OCP**: Am I designing for extension?
- [ ] **DIP**: Am I depending on abstractions?
- [ ] **DRY**: Am I avoiding duplication?
- [ ] **KISS**: Am I keeping it simple?
- [ ] **High Cohesion**: Are related items together?

### During Code Review

- [ ] **SRP**: One reason to change per class?
- [ ] **OCP**: Can add features without modifying?
- [ ] **LSP**: Subclasses truly substitutable?
- [ ] **ISP**: No forced implementations?
- [ ] **DIP**: Dependencies injected?
- [ ] **DRY**: No duplicated logic?
- [ ] **KISS**: Simple and clear?
- [ ] **High Cohesion**: Methods use class data?

### Refactoring Triggers

- 🚨 **SRP**: Class doing multiple things → Split it
- 🚨 **OCP**: Modifying for new features → Add abstraction
- 🚨 **LSP**: Type checking subclasses → Fix hierarchy
- 🚨 **ISP**: Empty implementations → Split interface
- 🚨 **DIP**: Hard to test → Inject dependencies
- 🚨 **DRY**: Copy-paste code → Extract function
- 🚨 **KISS**: Hard to explain → Simplify
- 🚨 **High Cohesion**: Unrelated methods → Extract class

---

## 🎓 Learning Path

### Beginner Path

1. **Start with KISS** → Simple code first
2. **Learn DRY** → Don't repeat yourself
3. **Master SRP** → One responsibility per class
4. **Understand High Cohesion** → Group related items

### Intermediate Path

5. **Learn OCP** → Design for extension
6. **Study LSP** → Proper inheritance
7. **Apply ISP** → Small interfaces

### Advanced Path

8. **Master DIP** → Dependency injection
9. **Combine All Principles** → Holistic design
10. **Recognize Patterns** → When to apply what

---

## 💡 Common Mistakes to Avoid

### Over-Engineering

```cpp
// ❌ Don't do this for simple addition:
class AdditionStrategy : public OperationStrategy { };
class AdditionFactory { };
class AdditionVisitor { };
class AdditionBuilder { };

// ✅ Do this:
int add(int a, int b) { return a + b; }
```

**Lesson**: Apply principles appropriately, not blindly.

### Under-Engineering

```cpp
// ❌ Don't do this for complex system:
class God {
    // Everything in one class
    // 10,000 lines of code
};

// ✅ Do this:
class UserService { };
class OrderService { };
class PaymentService { };
class NotificationService { };
```

**Lesson**: Break down complexity with proper design.

### Premature Abstraction

```cpp
// ❌ Don't abstract on first occurrence:
int calculate() { return 5; }

// First time: Just write it
// Second time: Acceptable duplication
// Third time: Then abstract (Rule of Three)
```

**Lesson**: Wait for patterns to emerge.

---

## 🔧 Practical Application

### Example: Building a User Management System

**Step 1: Identify Responsibilities (SRP)**
```
User data → User class
Database → UserRepository
Email → EmailService
Validation → UserValidator
```

**Step 2: Design Interfaces (ISP, OCP)**
```cpp
class IRepository { virtual void save() = 0; };
class IEmailSender { virtual void send() = 0; };
class IValidator { virtual bool validate() = 0; };
```

**Step 3: Inject Dependencies (DIP)**
```cpp
class UserService {
    IRepository* repo;
    IEmailSender* emailer;
public:
    UserService(IRepository* r, IEmailSender* e)
        : repo(r), emailer(e) {}
};
```

**Step 4: Keep It Simple (KISS, DRY)**
```cpp
// Simple validation
bool isValidEmail(string email) {
    return email.find("@") != string::npos;
}

// Constants instead of magic numbers
const int MIN_PASSWORD_LENGTH = 8;
```

**Step 5: Group Related Items (High Cohesion)**
```cpp
class User {
    string name;
    string email;
    // All user-related data and methods together
};
```

---

## 📖 Further Reading

### Books

- **"Clean Code" by Robert C. Martin** - Code quality fundamentals
- **"Clean Architecture" by Robert C. Martin** - SOLID principles
- **"Design Patterns" by Gang of Four** - Classic patterns
- **"The Pragmatic Programmer" by Hunt & Thomas** - DRY and more
- **"Code Complete" by Steve McConnell** - Comprehensive guide

### Online Resources

- **Refactoring Guru** - Design patterns and principles
- **Martin Fowler's Blog** - Software design insights
- **Uncle Bob's Blog** - SOLID principles from the source

---

## 🎯 Summary

### The Golden Rules

1. **SRP**: One class, one job
2. **OCP**: Open for extension, closed for modification
3. **LSP**: Child can substitute parent
4. **ISP**: Small, focused interfaces
5. **DIP**: Depend on abstractions
6. **DRY**: Don't repeat yourself
7. **KISS**: Keep it simple
8. **High Cohesion**: Related items together

### The Ultimate Goal

```
Good Design = 
    SOLID principles +
    DRY (no duplication) +
    KISS (no complexity) +
    High Cohesion (related together)
```

### Remember

> **"First, make it work. Then, make it right. Then, make it fast."**  
> — Kent Beck

Start with working code, apply principles to make it maintainable, then optimize if needed.

---

## 🚀 Getting Started

### 1. Explore Examples

Each principle has:
- ✅ Comprehensive C++ examples
- ✅ Detailed README documentation
- ✅ Bad vs Good code comparisons
- ✅ Real-world scenarios

### 2. Practice

- Read the examples
- Run the code
- Modify and experiment
- Apply to your projects

### 3. Master Gradually

- Don't try to apply everything at once
- Start with one principle
- Practice until natural
- Add another principle

---

## 📁 Repository Structure

```
DesignPrinciples/
├── 01_SingleResponsibility/
├── 02_OpenClosed/
├── 03_LiskovSubstitution/
├── 04_InterfaceSegregation/
├── 05_DependencyInversionPrinciple/
├── 06_Dry/
├── 07_Kiss/
├── 11_HighCohesion/
└── Readme.md (this file)
```

Each folder contains:
- `*.cpp` - Complete working examples
- `Readme.md` - Detailed documentation

---

## 💭 Final Thoughts

Design principles are **guidelines**, not laws. Use judgment:

- ✅ Apply when they improve code
- ✅ Balance simplicity with flexibility
- ✅ Consider your context
- ⚠️ Don't over-engineer
- ⚠️ Don't under-engineer

The goal is **maintainable, understandable, flexible code**.

---

## 🎉 Congratulations!

You now have access to comprehensive examples of all essential design principles. These principles will help you write better software throughout your career.

> **"The only way to go fast is to go well."**  
> — Robert C. Martin

**Happy coding!** 🚀

---

## 📞 Contributing

Feel free to:
- Add more examples
- Improve documentation
- Share your insights
- Report issues

---

## 📜 License

This educational resource is provided for learning purposes.

---

**Last Updated**: 2024  
**Author**: Learning Journey through Design Principles  
**Version**: 1.0 - Complete Edition