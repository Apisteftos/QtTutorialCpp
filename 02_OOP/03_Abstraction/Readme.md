# 15_AbstractClasses - Interface-Based Design

## 📚 Overview

This folder introduces **Abstract Classes** - classes that cannot be instantiated and contain at least one pure virtual function. Abstract classes are the foundation of interface-based design in C++, enabling polymorphism, defining contracts, and implementing design patterns.

**Abstract Class** = Class with at least one pure virtual function (= 0)

This is essential for writing extensible, maintainable, and loosely-coupled C++ applications.

---

## 🎯 What You'll Learn

- What abstract classes are and why they're essential
- Pure virtual functions (= 0)
- Difference between abstract and concrete classes
- Pure interfaces vs abstract base classes
- When to use abstract classes
- Design patterns using abstract classes
- Template Method, Strategy, Observer, Factory patterns
- Real-world interface design
- Best practices and common pitfalls

---

## 📖 Concept Explanation

### What are Abstract Classes?

**Abstract classes** are classes that cannot be instantiated directly. They contain at least one pure virtual function and serve as base classes that define interfaces or contracts for derived classes.

**Key Characteristics:**
- ❌ **Cannot be instantiated** directly
- ✅ **Can have pointers/references** to abstract classes
- ✅ **Define interface contracts** for derived classes
- ✅ **Force implementation** in concrete classes
- ✅ **Enable polymorphism** through common interface

### Real-World Analogy
```
Think of a "Vehicle" specification document:

Vehicle (Abstract Class)
  - MUST have: start(), stop(), refuel()
  - These are requirements (pure virtual functions)
  - You can't build a "generic vehicle"
  - But you CAN build:
    * Car (implements all requirements)
    * Motorcycle (implements all requirements)
    * Truck (implements all requirements)

The specification defines WHAT must be done,
Concrete classes define HOW it's done.
```

### Visual Representation
```
ABSTRACT CLASS (Cannot instantiate):
┌─────────────────────────┐
│       Shape             │  ❌ Shape obj;
│  ─────────────────────  │     (Error!)
│  + area() = 0           │  Pure virtual
│  + perimeter() = 0      │  Pure virtual
│  + draw()               │  Regular virtual
└─────────────────────────┘
           ▲
           │ implements
    ┌──────┴──────┐
    │             │
┌───▼──────┐  ┌──▼─────┐
│ Circle   │  │ Square │  CONCRETE CLASSES
│ ──────── │  │ ────── │  ✅ Can instantiate
│ +area()  │  │+area() │  Must implement
│ +perim() │  │+perim()│  pure virtuals
└──────────┘  └────────┘
```

---

## 💻 Basic Syntax

### Simple Abstract Class
```cpp
// Abstract class - has pure virtual function
class Shape {
public:
    // Pure virtual function (= 0)
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    
    // Regular virtual function (default implementation)
    virtual void draw() const {
        cout << "Drawing shape\n";
    }
    
    // Virtual destructor (ALWAYS!)
    virtual ~Shape() = default;
};

// Cannot instantiate abstract class
// Shape shape;  // ❌ Error!

// Concrete class - implements all pure virtuals
class Circle : public Shape {
    double radius;
    
public:
    Circle(double r) : radius(r) { }
    
    // MUST implement pure virtual functions
    double area() const override {
        return 3.14159 * radius * radius;
    }
    
    double perimeter() const override {
        return 2 * 3.14159 * radius;
    }
};

// Now can instantiate
Circle circle(5.0);  // ✅ OK!

// Can use abstract class pointers
Shape* ptr = &circle;  // ✅ OK!
ptr->area();           // ✅ OK! (polymorphism)
```

### Pure Virtual Function

**Syntax:**
```cpp
virtual ReturnType functionName(parameters) = 0;
```

**The `= 0` means:**
- This is a pure virtual function
- No implementation provided (usually)
- Makes the class abstract
- Derived classes MUST implement

---

## 🔍 Abstract vs Concrete Classes

### Comparison
```
┌──────────────────────────┬──────────────────────────┐
│ ABSTRACT CLASS           │ CONCRETE CLASS           │
├──────────────────────────┼──────────────────────────┤
│ Has ≥1 pure virtual      │ No pure virtual          │
│ Cannot instantiate       │ Can instantiate          │
│ Defines interface        │ Provides implementation  │
│ Used as base class       │ Used to create objects   │
│ Pointers/references only │ Can create actual objects│
│ Forces derived to impl   │ Complete implementation  │
└──────────────────────────┴──────────────────────────┘
```

### Code Example
```cpp
// ABSTRACT CLASS
class Animal {
public:
    virtual void makeSound() = 0;  // Pure virtual
    virtual ~Animal() = default;
};

// Animal animal;  // ❌ Error! Cannot instantiate

// CONCRETE CLASS
class Dog : public Animal {
public:
    void makeSound() override {  // Implemented
        cout << "Woof!\n";
    }
};

Dog dog;  // ✅ OK! Can instantiate
```

---

## 📋 Examples in This Folder

### Example 1: Basic Abstract Class
Understanding abstract classes fundamentals.

### Example 2: Pure Interface
Interface with only pure virtual functions.

### Example 3: Abstract Class with Implementation
Mix of pure virtual and regular methods.

### Example 4: Partially Abstract Class
Implementing some but not all pure virtuals.

### Example 5: Abstract Class as Interface
Database interface example.

### Example 6: Template Method Pattern
Algorithm skeleton with abstract steps.

### Example 7: Strategy Pattern
Interchangeable algorithms.

### Example 8: Observer Pattern
Event notification system.

### Example 9: Factory Method Pattern
Object creation through abstract factory.

### Example 10: Payment Gateway
Real-world payment processing system.

---

## 🎓 Key Concepts

### Pure Virtual Functions

**What is Pure Virtual?**

A pure virtual function is declared with `= 0` and has no implementation (usually).
```cpp
class Interface {
public:
    // Pure virtual - no implementation
    virtual void method1() = 0;
    virtual int method2(int x) = 0;
    virtual ~Interface() = default;
};
```

**Characteristics:**
- ✅ Makes class abstract
- ✅ MUST be implemented by derived classes
- ✅ Defines interface contract
- ✅ No function body (usually)
- ✅ Can optionally have implementation (rare)

**Example with Optional Implementation:**
```cpp
class Base {
public:
    // Pure virtual with implementation (rare pattern)
    virtual void func() = 0;
    virtual ~Base() = default;
};

// Provide implementation (optional)
void Base::func() {
    cout << "Default implementation\n";
}

class Derived : public Base {
public:
    void func() override {
        Base::func();  // Can call base implementation
        cout << "Derived implementation\n";
    }
};
```

---

### What Abstract Classes Can Have

**✅ Abstract classes CAN have:**
```cpp
class AbstractClass {
protected:
    int data;  // ✅ Data members
    
public:
    // ✅ Constructor (called by derived)
    AbstractClass(int d) : data(d) { }
    
    // ✅ Pure virtual functions
    virtual void pureVirtual() = 0;
    
    // ✅ Regular virtual functions
    virtual void virtualFunc() {
        cout << "Default implementation\n";
    }
    
    // ✅ Non-virtual functions
    void regularFunc() {
        cout << "Regular function\n";
    }
    
    // ✅ Virtual destructor (IMPORTANT!)
    virtual ~AbstractClass() = default;
};
```

**❌ Abstract classes CANNOT:**
- Be instantiated directly
- Create objects (only pointers/references)
```cpp
// AbstractClass obj;     // ❌ Error!
AbstractClass* ptr;       // ✅ OK!
AbstractClass& ref;       // ✅ OK!
```

---

### Pure Interface vs Abstract Base Class

#### Pure Interface

**Only pure virtual functions, no implementation:**
```cpp
// Pure interface - naming convention: prefix with 'I'
class IDrawable {
public:
    virtual void draw() const = 0;
    virtual void resize(double factor) = 0;
    virtual ~IDrawable() = default;
    
    // NO data members
    // NO implementation
};
```

**Characteristics:**
- ✅ Only pure virtual functions
- ✅ No data members
- ✅ No implementation
- ✅ Pure contract definition
- ✅ Safe for multiple inheritance

**When to use:**
- Defining pure behavior contract
- Multiple inheritance scenarios
- Plugin interfaces
- Dependency inversion

#### Abstract Base Class

**Mix of pure virtual, virtual, and regular functions:**
```cpp
class AbstractBase {
protected:
    string name;  // Data member
    
public:
    AbstractBase(string n) : name(n) { }
    
    // Pure virtual - must implement
    virtual void pureMethod() = 0;
    
    // Virtual - can override
    virtual void virtualMethod() {
        cout << "Default implementation\n";
    }
    
    // Regular - cannot override
    void regularMethod() {
        cout << "Regular method\n";
    }
    
    virtual ~AbstractBase() = default;
};
```

**Characteristics:**
- ✅ Mix of pure virtual and regular methods
- ✅ Can have data members
- ✅ Can provide default implementations
- ✅ Shared functionality + contract

**When to use:**
- Sharing common implementation
- Template Method pattern
- Base class with required + optional behavior

---

### Partially Abstract Classes

**A class remains abstract until ALL pure virtual functions are implemented:**
```cpp
// Abstract - has pure virtual
class Level1 {
public:
    virtual void func1() = 0;
    virtual void func2() = 0;
    virtual ~Level1() = default;
};

// Still abstract - only implements func1
class Level2 : public Level1 {
public:
    void func1() override {
        cout << "func1 implemented\n";
    }
    // func2() still pure virtual - still abstract!
};

// Concrete - implements all pure virtuals
class Level3 : public Level2 {
public:
    void func2() override {
        cout << "func2 implemented\n";
    }
};

// Level1 l1;  // ❌ Error! Abstract
// Level2 l2;  // ❌ Error! Still abstract
Level3 l3;     // ✅ OK! Concrete
```

**Benefits:**
- Share partial implementation
- Reduce code duplication
- Progressive refinement
- Layered abstraction

---

## 💡 Design Patterns with Abstract Classes

### 1. Template Method Pattern

**Define algorithm skeleton, let derived classes implement steps:**
```cpp
class Algorithm {
public:
    // Template method - defines structure
    void execute() {
        step1();
        step2();  // Abstract - must implement
        step3();
    }
    
    virtual ~Algorithm() = default;
    
protected:
    void step1() { cout << "Step 1\n"; }
    virtual void step2() = 0;  // Must implement
    void step3() { cout << "Step 3\n"; }
};

class ConcreteAlgorithm : public Algorithm {
protected:
    void step2() override {
        cout << "Custom Step 2\n";
    }
};

ConcreteAlgorithm algo;
algo.execute();  // Runs full algorithm with custom step2
```

**Use when:**
- Algorithm structure is fixed
- Some steps vary by implementation
- Want to control overall flow

### 2. Strategy Pattern

**Define family of interchangeable algorithms:**
```cpp
// Strategy interface
class Strategy {
public:
    virtual void execute() = 0;
    virtual ~Strategy() = default;
};

class StrategyA : public Strategy {
    void execute() override { cout << "Strategy A\n"; }
};

class StrategyB : public Strategy {
    void execute() override { cout << "Strategy B\n"; }
};

class Context {
    Strategy* strategy;
public:
    void setStrategy(Strategy* s) { strategy = s; }
    void doWork() { strategy->execute(); }
};

// Can switch strategies at runtime
Context ctx;
StrategyA stratA;
ctx.setStrategy(&stratA);
ctx.doWork();  // Uses Strategy A
```

**Use when:**
- Multiple algorithms for same task
- Want to switch at runtime
- Avoid conditionals

### 3. Observer Pattern

**One-to-many dependency notification:**
```cpp
class Observer {
public:
    virtual void update(const string& message) = 0;
    virtual ~Observer() = default;
};

class Subject {
    vector<Observer*> observers;
public:
    void attach(Observer* obs) { observers.push_back(obs); }
    void notify(const string& msg) {
        for (auto obs : observers) {
            obs->update(msg);
        }
    }
};

class ConcreteObserver : public Observer {
    void update(const string& msg) override {
        cout << "Received: " << msg << "\n";
    }
};
```

**Use when:**
- One object changes, many need notification
- Event handling systems
- MVC pattern

### 4. Factory Method Pattern

**Define interface for object creation:**
```cpp
// Product interface
class Product {
public:
    virtual void use() = 0;
    virtual ~Product() = default;
};

// Creator (abstract factory)
class Creator {
public:
    void doSomething() {
        Product* p = createProduct();
        p->use();
        delete p;
    }
    
    virtual ~Creator() = default;
    
protected:
    virtual Product* createProduct() = 0;  // Factory method
};

class ConcreteProduct : public Product {
    void use() override { cout << "Using product\n"; }
};

class ConcreteCreator : public Creator {
protected:
    Product* createProduct() override {
        return new ConcreteProduct();
    }
};
```

**Use when:**
- Class can't anticipate type to create
- Subclasses specify objects to create
- Delegate instantiation

---

## 🎯 Practice Exercises

### Exercise 1: File System
Create abstract `FileSystem` with:
- Pure virtual: `read()`, `write()`, `delete()`
- Concrete: `LocalFileSystem`, `CloudFileSystem`
- Test polymorphic file operations

### Exercise 2: Sorting Algorithms
Create `SortStrategy` interface:
- Pure virtual: `sort(vector<int>&)`
- Implement: BubbleSort, QuickSort, MergeSort
- Use Strategy pattern

### Exercise 3: Notification System
Create `Notification` abstract class:
- Pure virtual: `send(message)`
- Concrete: EmailNotification, SMSNotification, PushNotification
- Implement Observer pattern

### Exercise 4: Shape Calculator
Create abstract `Shape`:
- Pure virtual: `area()`, `perimeter()`, `volume()`
- Concrete: Sphere, Cube, Cylinder
- Calculate total area/volume of collection

---

## 🚀 How to Run

### Compile:
```bash
g++ -std=c++17 abstract_classes.cpp -o abstract_classes
```

### Run:
```bash
./abstract_classes
```

### Expected Output:
The program demonstrates:
1. Basic abstract class
2. Pure interface
3. Abstract with implementation
4. Partially abstract classes
5. Interface example
6. Template Method pattern
7. Strategy pattern
8. Observer pattern
9. Factory Method pattern
10. Payment gateway example

---

## 📊 Visual Concepts

### Abstract Class Hierarchy
```
        ┌──────────────┐
        │   Animal     │  (Abstract)
        │  ──────────  │
        │ +makeSound()=0  Pure virtual
        │ +move()=0       Pure virtual
        │ +sleep()        Virtual
        └──────────────┘
               ▲
               │ implements
        ┌──────┴──────┐
        │             │
    ┌───▼────┐    ┌──▼────┐
    │  Dog   │    │  Cat  │  (Concrete)
    │ ────── │    │ ────  │
    │+sound()│    │+sound()│  Must implement
    │+move() │    │+move() │  all pure virtuals
    └────────┘    └───────┘
```

### Instantiation Rules
```
ABSTRACT CLASS:
┌────────────────────┐
│  Abstract          │
│  ───────────────   │
│  + pureFunc() = 0  │
└────────────────────┘
         │
         │ ❌ Abstract obj;
         │    (Cannot instantiate)
         │
         │ ✅ Abstract* ptr;
         │    (Can have pointer)
         │
         ▼
┌────────────────────┐
│  Concrete          │
│  ───────────────   │
│  + pureFunc()      │  Implemented
└────────────────────┘
         │
         │ ✅ Concrete obj;
         ▼    (Can instantiate)
```

### Template Method Pattern
```
┌─────────────────────────┐
│     AbstractClass       │
├─────────────────────────┤
│ templateMethod() {      │  ← Defines structure
│     step1();            │     (non-virtual)
│     step2();  ←abstract │
│     step3();            │
│ }                       │
└─────────────────────────┘
            ▲
            │ implements step2()
┌───────────▼─────────────┐
│    ConcreteClass        │
├─────────────────────────┤
│ step2() {               │  ← Provides implementation
│     // Custom impl      │
│ }                       │
└─────────────────────────┘
```

---

## 📚 Related Topics

### Prerequisites:
- **14_VirtualFunctions** - Virtual functions
- **13_Inheritance** - Inheritance basics
- **09_OperatorOverloading** - Operators

### Coming Next:
- **16_Polymorphism** - Advanced polymorphism
- **17_Templates** - Generic programming
- **18_DesignPatterns** - More patterns

### Related Concepts:
- **Interface Design** - Pure interfaces
- **Design Patterns** - GoF patterns
- **SOLID Principles** - OOP principles

---

## 🎓 Summary

### What We Learned:

✅ **Abstract classes** cannot be instantiated
✅ **Pure virtual** functions declared with `= 0`
✅ **Derived classes** must implement pure virtuals
✅ **Can have pointers/references** to abstract classes
✅ **Mix of abstract and concrete** methods allowed
✅ **Pure interfaces** for contract definition
✅ **Design patterns** use abstract classes heavily
✅ **Virtual destructor** always required
✅ **Polymorphism** foundation
✅ **Interface-based design** principles

### The Complete Pattern:
```cpp
// Abstract base class
class AbstractBase {
protected:
    int data;  // Can have data
    
public:
    // Constructor (called by derived)
    AbstractBase(int d) : data(d) { }
    
    // Pure virtual - must implement
    virtual void pureVirtual() = 0;
    
    // Regular virtual - can override
    virtual void regularVirtual() {
        cout << "Default implementation\n";
    }
    
    // Non-virtual - cannot override
    void nonVirtual() {
        cout << "Fixed implementation\n";
    }
    
    // Virtual destructor (CRITICAL!)
    virtual ~AbstractBase() = default;
};

// Concrete derived class
class Concrete : public AbstractBase {
public:
    Concrete(int d) : AbstractBase(d) { }
    
    // MUST implement pure virtual
    void pureVirtual() override {
        cout << "Concrete implementation\n";
    }
    
    // CAN override regular virtual
    void regularVirtual() override {
        cout << "Overridden implementation\n";
    }
};

// Usage
// AbstractBase obj(10);  // ❌ Error!
Concrete obj(10);         // ✅ OK!
AbstractBase* ptr = &obj; // ✅ OK!
ptr->pureVirtual();       // ✅ Polymorphism
```

---

## ✨ Quick Reference Card
```cpp
// ABSTRACT CLASS
class Abstract {
public:
    // Pure virtual (= 0)
    virtual void pureFunc() = 0;
    
    // Regular virtual
    virtual void virtualFunc() {
        // Default implementation
    }
    
    // Non-virtual
    void regularFunc() { }
    
    // Virtual destructor
    virtual ~Abstract() = default;
};

// CONCRETE CLASS
class Concrete : public Abstract {
public:
    // MUST implement pure virtual
    void pureFunc() override {
        cout << "Implementation\n";
    }
};

// USAGE
// Abstract a;          // ❌ Error!
Concrete c;             // ✅ OK!
Abstract* ptr = &c;     // ✅ OK!
ptr->pureFunc();        // ✅ Polymorphism

// PURE INTERFACE
class IInterface {
public:
    virtual void method1() = 0;
    virtual void method2() = 0;
    virtual ~IInterface() = default;
    // No data, no implementation
};
```

---

## 🔍 Best Practices

### ✅ DO:

- **Make destructor virtual** (ALWAYS!)
- **Use pure virtual** for required behavior
- **Provide default implementations** when sensible
- **Keep interfaces small** and focused (ISP)
- **Document contracts** clearly
- **Use `override` keyword** in derived classes
- **Prefix interfaces** with 'I' (IDrawable, IPrintable)
- **Think interface** vs implementation
- **Use abstract classes** for design patterns
- **Enable dependency inversion**

### ❌ DON'T:

- **Forget virtual destructor** (memory leaks!)
- **Make interfaces too large** (violates ISP)
- **Try to instantiate** abstract classes
- **Forget to implement** pure virtuals
- **Add unnecessary** pure virtuals
- **Mix too many concerns** in one abstract class
- **Use for everything** (overkill)
- **Ignore error messages** about abstract classes

---

## 🌟 Real-World Applications

### Plugin Systems
```cpp
class IPlugin {
    virtual void initialize() = 0;
    virtual void execute() = 0;
    virtual void shutdown() = 0;
};
```

### Database Abstraction
```cpp
class IDatabase {
    virtual bool connect() = 0;
    virtual bool query() = 0;
    virtual void disconnect() = 0;
};
```

### GUI Frameworks
```cpp
class IWidget {
    virtual void render() = 0;
    virtual void handleEvent() = 0;
};
```

### Game Engines
```cpp
class IGameObject {
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void handleCollision() = 0;
};
```

---

## 📖 Further Reading

- [cppreference.com - Abstract Classes](https://en.cppreference.com/w/cpp/language/abstract_class)
- "Design Patterns" by GoF - Gang of Four
- "Clean Architecture" by Robert C. Martin
- "Head First Design Patterns" by Freeman
- [C++ Core Guidelines - Inheritance](https://isocpp.github.io/CppCoreGuidelines/)

---

## 🎯 Key Takeaways

1. Abstract classes **cannot be instantiated**
2. **Pure virtual** functions declared with `= 0`
3. Derived classes **must implement** pure virtuals
4. Can have **pointers/references** to abstract classes
5. **Mix of methods** (pure virtual, virtual, regular)
6. **Virtual destructor** is critical
7. **Pure interfaces** vs abstract base classes
8. **Design patterns** foundation
9. Enable **polymorphism** and **flexibility**
10. Foundation for **interface-based design**

---

**Previous Topic:** [14_VirtualFunctions](../14_VirtualFunctions/) - Virtual Functions

**Next Topic:** [16_Polymorphism](../16_Polymorphism/) - Advanced Polymorphism (coming next)

---

*Part of the C++ Classes and Objects series*
*Difficulty: Advanced*
*Prerequisites: 14_VirtualFunctions, 13_Inheritance*
*Foundation for Design Patterns*