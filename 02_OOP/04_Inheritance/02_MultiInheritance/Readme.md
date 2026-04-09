# 16_MultipleInheritance - Multiple Base Classes

## 📚 Overview

This folder introduces **Multiple Inheritance** - a feature where a class can inherit from TWO OR MORE base classes simultaneously. While powerful, multiple inheritance comes with complexity, ambiguity issues, and the infamous diamond problem that must be carefully managed.

**Multiple Inheritance** = A class inheriting from 2+ base classes

This is a controversial but sometimes useful C++ feature that requires careful design and understanding.

---

## 🎯 What You'll Learn

- What multiple inheritance is and how it works
- Basic multiple inheritance syntax
- Ambiguity problems and solutions
- The diamond problem (and why it's a problem)
- Virtual inheritance (the diamond solution)
- Constructor and destructor calling order
- Multiple interface inheritance (recommended pattern)
- Mixin pattern
- When to use (and avoid) multiple inheritance
- Composition as an alternative
- Real-world examples and best practices

---

## 📖 Concept Explanation

### What is Multiple Inheritance?

**Multiple inheritance** allows a class to inherit from more than one base class, combining their features and functionality.

**Syntax:**
```cpp
class Derived : public Base1, public Base2, public Base3 {
    // Inherits from all three bases
};
```

**Key Characteristics:**
- ✅ Inherit from multiple parents
- ✅ Combine functionality from multiple sources
- ✅ Implement multiple interfaces
- ⚠️ Can cause ambiguity
- ⚠️ Can cause diamond problem
- ⚠️ Increases complexity

### Real-World Analogy
```
Think of a Swiss Army Knife:

Base Classes:
  - Knife (cutting functionality)
  - Screwdriver (screwing functionality)
  - Scissors (cutting differently)
  - Can opener (opening functionality)

Swiss Army Knife = All of the above combined!
  - IS-A Knife
  - IS-A Screwdriver
  - IS-A Scissors
  - IS-A Can opener

Multiple inheritance lets one object be many things.
But what if Knife and Scissors both have "cut()"?
→ Ambiguity problem!
```

### Visual Representation
```
SINGLE INHERITANCE:
    ┌─────┐
    │  A  │
    └──┬──┘
       │
    ┌──▼──┐
    │  B  │
    └─────┘

MULTIPLE INHERITANCE:
    ┌─────┐   ┌─────┐   ┌─────┐
    │  A  │   │  B  │   │  C  │
    └──┬──┘   └──┬──┘   └──┬──┘
       └─────────┼─────────┘
              ┌──▼──┐
              │  D  │
              └─────┘
    D inherits from A, B, and C

DIAMOND PROBLEM:
         ┌─────┐
         │Base │
         └──┬──┘
       ┌────┴────┐
    ┌──▼──┐   ┌──▼──┐
    │  A  │   │  B  │
    └──┬──┘   └──┬──┘
       └─────┬────┘
          ┌──▼──┐
          │  C  │  ← Has TWO Base copies!
          └─────┘
```

---

## 💻 Basic Syntax

### Simple Multiple Inheritance
```cpp
class Base1 {
public:
    void func1() {
        cout << "Base1::func1()\n";
    }
};

class Base2 {
public:
    void func2() {
        cout << "Base2::func2()\n";
    }
};

// Multiple inheritance
class Derived : public Base1, public Base2 {
public:
    Derived() : Base1(), Base2() {  // Call both constructors
        cout << "Derived constructor\n";
    }
    
    void useBoth() {
        func1();  // From Base1
        func2();  // From Base2
    }
};

// Usage
Derived obj;
obj.func1();   // ✅ From Base1
obj.func2();   // ✅ From Base2
obj.useBoth(); // ✅ Uses both
```

### Constructor Order

**Construction order: LEFT to RIGHT, then derived**
```cpp
class A {
    A() { cout << "1. A\n"; }
};

class B {
    B() { cout << "2. B\n"; }
};

class C {
    C() { cout << "3. C\n"; }
};

class D : public A, public B, public C {
    D() { cout << "4. D\n"; }
};

// Output:
// 1. A
// 2. B
// 3. C
// 4. D
```

**Destruction order: REVERSE (right to left, then derived)**

---

## 🔍 The Problems with Multiple Inheritance

### Problem 1: Ambiguity

**When multiple bases have the same function name:**
```cpp
class ClassA {
public:
    void display() { cout << "A\n"; }
};

class ClassB {
public:
    void display() { cout << "B\n"; }  // Same name!
};

class Derived : public ClassA, public ClassB {
public:
    void use() {
        // display();  // ❌ ERROR! Ambiguous!
        // Which display() - A's or B's?
    }
};
```

**Solution 1: Scope Resolution**
```cpp
void use() {
    ClassA::display();  // ✅ Explicitly specify
    ClassB::display();  // ✅ Explicitly specify
}
```

**Solution 2: Override in Derived**
```cpp
class Derived : public ClassA, public ClassB {
public:
    void display() override {  // Resolve ambiguity
        ClassA::display();  // Choose which to call
        // or ClassB::display();
        // or both!
    }
};
```

---

### Problem 2: The Diamond Problem

**The most famous problem in multiple inheritance:**
```
         Device
         /    \
      Phone  Camera
         \    /
       SmartPhone

SmartPhone inherits Device TWICE!
- Once through Phone
- Once through Camera
```

**Code Example:**
```cpp
class Device {
protected:
    string name;
    int id;
public:
    Device(string n, int i) : name(n), id(i) { }
    void powerOn() { cout << name << " on\n"; }
};

class Phone : public Device {
public:
    Phone(string n, int i) : Device(n, i) { }
    void call() { }
};

class Camera : public Device {
public:
    Camera(string n, int i) : Device(n, i) { }
    void takePicture() { }
};

class SmartPhone : public Phone, public Camera {
public:
    SmartPhone(string n)
        : Phone(n, 1), Camera(n, 2) { }
    
    void use() {
        // powerOn();  // ❌ Ambiguous! Which Device?
        Phone::powerOn();   // Must specify
        Camera::powerOn();
        
        // Two different ids!
        Phone::id;    // = 1
        Camera::id;   // = 2
    }
};
```

**Problems:**
- ❌ SmartPhone contains **TWO Device objects**
- ❌ Ambiguous access to Device members
- ❌ Wasted memory
- ❌ Inconsistent state (two different ids!)
- ❌ Confusing behavior

---

### Solution: Virtual Inheritance

**Virtual inheritance creates a SINGLE shared base:**
```cpp
class Device {
protected:
    string name;
    int id;
public:
    Device(string n = "Unknown", int i = 0) 
        : name(n), id(i) { }
    void powerOn() { cout << name << " on\n"; }
};

// VIRTUAL inheritance
class Phone : virtual public Device {
public:
    Phone(string n = "Unknown", int i = 0) 
        : Device(n, i) { }
    void call() { }
};

// VIRTUAL inheritance
class Camera : virtual public Device {
public:
    Camera(string n = "Unknown", int i = 0) 
        : Device(n, i) { }
    void takePicture() { }
};

class SmartPhone : public Phone, public Camera {
public:
    // MUST call Device constructor explicitly!
    SmartPhone(string n, int i)
        : Device(n, i), Phone(), Camera() { }
    
    void use() {
        powerOn();  // ✅ No ambiguity!
        // Only ONE Device now
        cout << id << "\n";  // ✅ Only one id
    }
};
```

**How Virtual Inheritance Works:**
```
WITHOUT virtual:                WITH virtual:
     Device                         Device (shared)
     /    \                         /    \
  Phone  Camera                  Phone  Camera
     \    /                         \    /
   SmartPhone                     SmartPhone
   (TWO Device copies)            (ONE Device copy)
```

**Key Points:**
1. Use `virtual` keyword: `class Phone : virtual public Device`
2. Only ONE shared base object
3. Most derived class calls base constructor
4. Solves diamond problem
5. Slight performance overhead (one extra indirection)

---

## 📋 Examples in This Folder

### Example 1: Basic Multiple Inheritance
Simple multiple inheritance with Engine and Wheels.

### Example 2: Ambiguity Problem
Same function names in multiple bases.

### Example 3: Diamond Problem
The classic diamond inheritance issue.

### Example 4: Virtual Inheritance
Solving the diamond problem.

### Example 5: Multiple Interface Implementation
Implementing multiple pure interfaces (recommended).

### Example 6: Mixin Pattern
Template-based mixins for reusable functionality.

### Example 7: Constructor/Destructor Order
Understanding the calling order.

### Example 8: When to Use Multiple Inheritance
Good and bad use cases.

### Example 9: Employee System
Real-world example with multiple roles.

### Example 10: Composition Alternative
Why composition is often better.

---

## 🎓 Key Concepts

### Constructor and Destructor Order

#### Regular Multiple Inheritance

**Construction: LEFT to RIGHT, then derived**
```cpp
class D : public A, public B, public C { };

Order:
1. A constructor
2. B constructor
3. C constructor
4. D constructor
```

**Destruction: REVERSE**
```cpp
Order:
1. D destructor
2. C destructor
3. B destructor
4. A destructor
```

#### Virtual Inheritance

**Virtual bases constructed FIRST, before non-virtual bases:**
```cpp
class Base { };
class A : virtual public Base { };
class B : virtual public Base { };
class C : public A, public B { };

Order:
1. Base constructor (virtual base first!)
2. A constructor
3. B constructor
4. C constructor
```

**Key Rule:** Most derived class is responsible for calling virtual base constructor!

---

### Multiple Interfaces (Best Practice)

**Interfaces = Pure virtual functions only**

This is the **RECOMMENDED** use of multiple inheritance:
```cpp
// Pure interfaces - no data, no implementation
class IDrawable {
public:
    virtual void draw() const = 0;
    virtual ~IDrawable() = default;
};

class ISerializable {
public:
    virtual string serialize() const = 0;
    virtual ~ISerializable() = default;
};

class IClickable {
public:
    virtual void onClick() = 0;
    virtual ~IClickable() = default;
};

// Safe multiple inheritance - all interfaces
class Button : public IDrawable, public ISerializable, public IClickable {
public:
    // Implement all interfaces
    void draw() const override { }
    string serialize() const override { return ""; }
    void onClick() override { }
};
```

**Why this is safe:**
- ✅ No diamond problem (no shared data)
- ✅ No ambiguity (pure virtual only)
- ✅ Clear contracts
- ✅ Flexible polymorphism
- ✅ Common in modern C++ design

**Naming Convention:**
- Prefix interfaces with 'I': `IDrawable`, `ISerializable`
- Makes intent clear

---

### Composition vs Multiple Inheritance

**Multiple Inheritance Approach:**
```cpp
class Logger {
public:
    void log(string msg) { }
};

class Validator {
public:
    bool validate() { return true; }
};

// Multiple inheritance
class Processor : public Logger, public Validator {
    // IS-A Logger AND IS-A Validator
};
```

**Composition Approach (BETTER!):**
```cpp
class Processor {
private:
    Logger logger;        // HAS-A Logger
    Validator validator;  // HAS-A Validator
    
public:
    void process() {
        if (validator.validate()) {
            logger.log("Processing");
        }
    }
};
```

**Why Composition is Better:**
```
┌─────────────────────────┬─────────────────────────┐
│ Multiple Inheritance    │ Composition             │
├─────────────────────────┼─────────────────────────┤
│ Tight coupling          │ Loose coupling          │
│ IS-A relationship       │ HAS-A relationship      │
│ Complex hierarchies     │ Flat structure          │
│ Diamond problem         │ No diamond problem      │
│ Compile-time binding    │ Runtime flexibility     │
│ Hard to test            │ Easy to test            │
│ Less flexible           │ More flexible           │
└─────────────────────────┴─────────────────────────┘
```

**Gang of Four Principle:**
> "Prefer composition over inheritance"

---

## 💡 When to Use Multiple Inheritance

### ✅ GOOD Use Cases

#### 1. Multiple Pure Interfaces
```cpp
class Widget : public IDrawable, public ISerializable, public IClickable {
    // Implement multiple interfaces
    // NO diamond problem (no data)
    // SAFE multiple inheritance
};
```

**Use when:**
- All bases are pure interfaces
- No implementation inheritance
- Need multiple contracts

#### 2. Mixin Pattern
```cpp
template<typename Base>
class Loggable : public Base {
    // Add logging to any class
};

template<typename Base>
class Timestamped : public Base {
    // Add timestamp to any class
};

// Compose features
Loggable<Timestamped<MyClass>> obj;
```

**Use when:**
- Small, reusable features
- Orthogonal concerns
- Template-based composition

#### 3. True Multiple IS-A (Rare!)
```cpp
// Duck IS-A Flyer AND IS-A Swimmer
class Duck : public Flyer, public Swimmer {
    // Genuinely both
};
```

**Use when:**
- Truly multiple IS-A relationships
- No conflicts between bases
- Not achievable with composition

---

### ❌ BAD Use Cases

#### 1. Multiple Implementation Inheritance
```cpp
// ❌ BAD - Both have implementation
class Processor : public Logger, public Validator {
    // Complex, tightly coupled
};

// ✅ GOOD - Use composition
class Processor {
    Logger logger;
    Validator validator;
};
```

#### 2. Diamond Inheritance
```cpp
// ❌ BAD - Diamond without virtual
class SmartPhone : public Phone, public Camera {
    // TWO Device copies!
};

// ✅ GOOD - Use virtual OR composition
class Phone : virtual public Device { };
class Camera : virtual public Device { };
```

#### 3. Just for Code Reuse
```cpp
// ❌ BAD - Using inheritance just to reuse code
class MyClass : public UtilityA, public UtilityB {
    // Not IS-A relationships!
};

// ✅ GOOD - Use composition
class MyClass {
    UtilityA utilA;
    UtilityB utilB;
};
```

---

## 🎯 Practice Exercises

### Exercise 1: Media Player
Create `AudioPlayer` and `VideoPlayer` base classes.
Create `MediaPlayer` with multiple inheritance.
Then refactor to use composition instead.

### Exercise 2: Employee Roles
Create interfaces: `IManager`, `IDeveloper`, `IDesigner`.
Create concrete classes implementing multiple interfaces.
Test polymorphic behavior.

### Exercise 3: Diamond Resolution
Create a diamond hierarchy without virtual inheritance.
Observe the problems. Fix with virtual inheritance.

### Exercise 4: Mixin Composition
Create timestamp and logging mixins.
Apply them to different classes using templates.

---

## 🚀 How to Run

### Compile:
```bash
g++ -std=c++17 multiple_inheritance.cpp -o multiple_inheritance
```

### Run:
```bash
./multiple_inheritance
```

### Expected Output:
The program demonstrates:
1. Basic multiple inheritance
2. Ambiguity problems
3. Diamond problem
4. Virtual inheritance solution
5. Multiple interfaces
6. Mixin pattern
7. Constructor/destructor order
8. When to use/avoid
9. Employee system example
10. Composition alternative

---

## 📊 Visual Concepts

### Constructor Order
```
class D : public A, public B, public C { };

CONSTRUCTION (left → right → derived):
    A → B → C → D

DESTRUCTION (reverse):
    D → C → B → A
```

### Diamond Problem Visualization
```
WITHOUT virtual inheritance:

        Device(id=1)
        /          \
     Phone       Camera
        \          /
         SmartPhone
    Contains: Device(id=1) via Phone
              Device(id=2) via Camera
    Total: TWO Device objects! ❌

WITH virtual inheritance:

        Device(id=1)  ← Shared
        /          \
     Phone       Camera
        \          /
         SmartPhone
    Contains: ONE Device(id=1)
    Total: ONE Device object! ✅
```

### Interface-Based Design
```
    IDrawable   ISerializable   IClickable
         \           |           /
          \          |          /
           \         |         /
            \        |        /
             ───────────────
                  Button
    (implements all three interfaces)

✅ Safe - no diamond problem
✅ Clear - multiple contracts
✅ Flexible - polymorphic behavior
```

---

## 📚 Related Topics

### Prerequisites:
- **13_Inheritance** - Single inheritance
- **14_VirtualFunctions** - Virtual functions
- **15_AbstractClasses** - Abstract classes

### Coming Next:
- **17_Templates** - Generic programming
- **18_STL** - Standard Template Library
- **19_DesignPatterns** - Advanced patterns

### Related Concepts:
- **Composition** - HAS-A relationships
- **Interfaces** - Pure virtual functions
- **Mixins** - Template-based composition

---

## 🎓 Summary

### What We Learned:

✅ **Multiple inheritance** = inherit from 2+ bases
✅ **Ambiguity** when bases have same names
✅ **Diamond problem** creates duplicate bases
✅ **Virtual inheritance** solves diamond problem
✅ **Constructor order** = left to right
✅ **Virtual bases** constructed first
✅ **Multiple interfaces** = recommended pattern
✅ **Composition** often better than MI
✅ **Use sparingly** and carefully
✅ **Document thoroughly** when using

### The Complete Pattern:
```cpp
// RECOMMENDED: Multiple pure interfaces
class IInterface1 {
public:
    virtual void method1() = 0;
    virtual ~IInterface1() = default;
};

class IInterface2 {
public:
    virtual void method2() = 0;
    virtual ~IInterface2() = default;
};

class Concrete : public IInterface1, public IInterface2 {
public:
    void method1() override { }
    void method2() override { }
};

// IF diamond problem exists: Use virtual
class Base { };
class A : virtual public Base { };
class B : virtual public Base { };
class C : public A, public B {
    C() : Base(), A(), B() { }  // Call Base explicitly
};

// PREFERRED: Composition
class BetterDesign {
    Interface1Impl impl1;  // HAS-A
    Interface2Impl impl2;  // HAS-A
};
```

---

## ✨ Quick Reference Card
```cpp
// BASIC MULTIPLE INHERITANCE
class D : public A, public B, public C {
    D() : A(), B(), C() { }
};

// AMBIGUITY RESOLUTION
Base1::func();  // Explicit scope

// VIRTUAL INHERITANCE (diamond solution)
class A : virtual public Base { };
class B : virtual public Base { };
class C : public A, public B {
    C() : Base(), A(), B() { }  // Call Base!
};

// MULTIPLE INTERFACES (RECOMMENDED)
class Widget : public IDrawable, public ISerializable {
    // Implement all pure virtuals
};

// COMPOSITION (BETTER!)
class MyClass {
    Base1 member1;  // HAS-A
    Base2 member2;  // HAS-A
};
```

---

## 🔍 Best Practices

### ✅ DO:

- **Use for multiple pure interfaces** (most common)
- **Use virtual inheritance** for diamond
- **Make destructors virtual** (always!)
- **Document MI relationships** clearly
- **Consider composition first**
- **Keep hierarchies shallow**
- **Test thoroughly** for ambiguity
- **Use scope resolution** when needed
- **Prefix interfaces** with 'I'

### ❌ DON'T:

- **Use for implementation reuse** (use composition)
- **Create diamond** without virtual
- **Make deep hierarchies**
- **Ignore ambiguity warnings**
- **Use MI** just because you can
- **Forget to call** virtual base constructor
- **Mix implementation** inheritance heavily
- **Overuse MI** (prefer composition)

---

## 🌟 Real-World Applications

### GUI Frameworks
```cpp
class Button : public IWidget, public IDrawable, public IClickable {
    // Multiple interface implementation
};
```

### Plugin Systems
```cpp
class Plugin : public ILoadable, public IExecutable, public IConfigurable {
    // Multiple capabilities
};
```

### Network Protocols
```cpp
class Connection : public ITCPHandler, public IUDPHandler {
    // Multiple protocol support
};
```

### Role-Based Systems
```cpp
class TechLead : public IManager, public IDeveloper {
    // Multiple roles
};
```

---

## 📖 Further Reading

- [cppreference.com - Multiple Inheritance](https://en.cppreference.com/w/cpp/language/derived_class)
- "Effective C++" by Scott Meyers - Item 40
- "Multiple Inheritance Considered Useful" (papers)
- "Design Patterns" by GoF - Composition
- [C++ Core Guidelines - MI](https://isocpp.github.io/CppCoreGuidelines/)

---

## 🎯 Key Takeaways

1. **Multiple inheritance** = inheriting from 2+ bases
2. **Ambiguity** when same names in bases
3. **Diamond problem** = duplicate base objects
4. **Virtual inheritance** = shared single base
5. **Constructor order** = left to right
6. **Virtual bases first** in construction
7. **Multiple interfaces** = recommended pattern
8. **Composition often better** than MI
9. **Use sparingly** and carefully
10. **"Prefer composition over inheritance"** - GoF

---

**Previous Topic:** [15_AbstractClasses](../15_AbstractClasses/) - Abstract Classes

**Next Topic:** [17_Templates](../17_Templates/) - Generic Programming (coming next)

---

*Part of the C++ Classes and Objects series*
*Difficulty: Advanced*
*Prerequisites: 13_Inheritance, 14_VirtualFunctions*
*Use with Caution - Powerful but Complex*