# 14_VirtualFunctions - Runtime Polymorphism

## 📚 Overview

This folder introduces **Virtual Functions** - the mechanism that enables runtime polymorphism in C++. Virtual functions allow derived classes to provide their own implementations of base class methods, with the correct version being called at runtime based on the actual object type.

**Virtual Functions** = Functions that can be overridden in derived classes for polymorphic behavior

This is THE cornerstone of object-oriented programming that makes polymorphism possible.

---

## 🎯 What You'll Learn

- What virtual functions are and why they're essential
- Static binding vs dynamic binding
- Virtual destructors (critical!)
- `override` keyword (C++11)
- `final` keyword (C++11)
- Pure virtual functions and abstract classes
- Object slicing problem
- vtable mechanism (how it works under the hood)
- Real-world polymorphic design
- Best practices and common pitfalls

---

## 📖 Concept Explanation

### What are Virtual Functions?

**Virtual functions** are member functions that can be overridden in derived classes, with the correct version being called at runtime based on the actual object type, not the pointer/reference type.

**The Problem Without Virtual:**
```cpp
class Animal {
public:
    void makeSound() { cout << "Generic sound\n"; }
};

class Dog : public Animal {
public:
    void makeSound() { cout << "Woof!\n"; }
};

Animal* ptr = new Dog();
ptr->makeSound();  // Prints "Generic sound" ❌
                   // We wanted "Woof!"
```

**The Solution With Virtual:**
```cpp
class Animal {
public:
    virtual void makeSound() { cout << "Generic sound\n"; }
};

class Dog : public Animal {
public:
    void makeSound() override { cout << "Woof!\n"; }
};

Animal* ptr = new Dog();
ptr->makeSound();  // Prints "Woof!" ✅
                   // Correct polymorphic behavior!
```

### Real-World Analogy
```
Think of a remote control:

Remote Control (Base Class)
  - Has button: "Play"
  - Button behavior: virtual void play()

Different Devices (Derived Classes)
  - DVD Player: play() → plays DVD
  - Music Player: play() → plays music
  - TV: play() → shows channel

When you press "Play" on the remote:
  • Remote doesn't know which device
  • Device itself determines behavior
  • Same button, different actions
  • That's polymorphism!
```

### Visual Representation
```
WITHOUT virtual (Static Binding):
┌──────────────┐
│ Animal* ptr  │ ────┐
└──────────────┘     │ Compiler sees pointer type
                     │ Calls Animal::makeSound()
                     ▼
                ┌─────────┐
                │   Dog   │ (actual object ignored)
                │  Woof!  │
                └─────────┘

WITH virtual (Dynamic Binding):
┌──────────────┐
│ Animal* ptr  │ ────┐
└──────────────┘     │ Runtime checks actual object
                     │ Follows vptr to vtable
                     ▼
                ┌─────────┐
                │   Dog   │ ✅ Calls Dog::makeSound()
                │  Woof!  │
                └─────────┘
```

---

## 💻 Basic Syntax

### Simple Virtual Function
```cpp
// Base class
class Base {
public:
    // Virtual function (can be overridden)
    virtual void display() {
        cout << "Base display\n";
    }
    
    // Virtual destructor (ALWAYS do this!)
    virtual ~Base() { }
};

// Derived class
class Derived : public Base {
public:
    // Override virtual function
    void display() override {
        cout << "Derived display\n";
    }
};

// Usage
Base* ptr = new Derived();
ptr->display();  // Calls Derived::display() ✅
delete ptr;      // Calls both destructors ✅
```

### The Three Keywords

1. **`virtual`** - in base class (makes function overridable)
2. **`override`** - in derived class (explicit override, C++11)
3. **`final`** - prevents further overriding (C++11)
```cpp
class Base {
    virtual void func1();           // Can override
    virtual void func2() final;     // Cannot override further
};

class Derived : public Base {
    void func1() override;          // ✅ Overrides
    // void func2() override;       // ❌ Error! func2 is final
};
```

---

## 🔍 Static vs Dynamic Binding

### Static Binding (Without Virtual)

**Resolved at compile-time based on pointer/reference type.**
```cpp
class Animal {
public:
    void makeSound() { cout << "Animal sound\n"; }
};

class Dog : public Animal {
public:
    void makeSound() { cout << "Woof!\n"; }
};

Dog dog;
Animal* ptr = &dog;

ptr->makeSound();  // "Animal sound" (static binding)
dog.makeSound();   // "Woof!" (direct call)
```

**Characteristics:**
- ⚡ Fast (direct call)
- 📊 Compile-time decision
- ❌ No polymorphism
- 🔒 Based on pointer type

### Dynamic Binding (With Virtual)

**Resolved at runtime based on actual object type.**
```cpp
class Animal {
public:
    virtual void makeSound() { cout << "Animal sound\n"; }
    virtual ~Animal() = default;
};

class Dog : public Animal {
public:
    void makeSound() override { cout << "Woof!\n"; }
};

Dog dog;
Animal* ptr = &dog;

ptr->makeSound();  // "Woof!" (dynamic binding) ✅
```

**Characteristics:**
- 🐌 Slightly slower (vtable lookup)
- ⏱️ Runtime decision
- ✅ Enables polymorphism
- 🎯 Based on actual object type

### Comparison Table
```
┌──────────────────┬──────────────────┬──────────────────┐
│                  │ Static Binding   │ Dynamic Binding  │
├──────────────────┼──────────────────┼──────────────────┤
│ Keyword          │ (none)           │ virtual          │
│ When resolved    │ Compile-time     │ Runtime          │
│ Based on         │ Pointer type     │ Object type      │
│ Speed            │ Fast             │ Slightly slower  │
│ Polymorphism     │ No               │ Yes              │
│ Overhead         │ None             │ vptr + vtable    │
│ Use case         │ Non-polymorphic  │ Polymorphic      │
└──────────────────┴──────────────────┴──────────────────┘
```

---

## 📋 Examples in This Folder

### Example 1: Without Virtual (Static Binding)
Shows the problem when virtual is not used.

### Example 2: With Virtual (Dynamic Binding)
Demonstrates polymorphic behavior.

### Example 3: Virtual Destructor
Critical example showing memory leak prevention.

### Example 4: Override Keyword
Using C++11 override for safety.

### Example 5: Final Keyword
Preventing further overriding.

### Example 6: Pure Virtual Functions
Creating abstract classes and interfaces.

### Example 7: Object Slicing
The problem with passing by value.

### Example 8: vtable Mechanism
How virtual functions work internally.

### Example 9: Payment System
Real-world polymorphic design.

### Example 10: Covariant Return Types
Advanced override feature.

---

## 🎓 Key Concepts

### Virtual Destructor (CRITICAL!)

**Problem Without Virtual Destructor:**
```cpp
class Base {
public:
    ~Base() {  // ❌ NOT virtual
        cout << "Base destructor\n";
    }
};

class Derived : public Base {
    int* data;
public:
    Derived() { data = new int[100]; }
    
    ~Derived() {  // Never called!
        delete[] data;
        cout << "Derived destructor\n";
    }
};

Base* ptr = new Derived();
delete ptr;  // Only calls Base destructor!
             // Derived destructor NEVER called!
             // MEMORY LEAK! ⚠️
```

**Output:**
```
Base destructor
(Derived destructor NOT called - memory leak!)
```

**Solution With Virtual Destructor:**
```cpp
class Base {
public:
    virtual ~Base() {  // ✅ Virtual
        cout << "Base destructor\n";
    }
};

class Derived : public Base {
    int* data;
public:
    Derived() { data = new int[100]; }
    
    ~Derived() {
        delete[] data;
        cout << "Derived destructor\n";
    }
};

Base* ptr = new Derived();
delete ptr;  // Calls both destructors! ✅
```

**Output:**
```
Derived destructor
Base destructor
(Memory properly freed!)
```

**⚠️ CRITICAL RULE:**
```
If a class has ANY virtual function,
the destructor MUST be virtual!

Always: virtual ~ClassName() { }
```

---

### Override Keyword (C++11)

**Problem Without Override:**
```cpp
class Base {
public:
    virtual void display() { }
    virtual void show(int x) { }
    virtual void print() const { }
};

class Derived : public Base {
public:
    // Typo - doesn't override, creates new function!
    void dysplay() { }  // ❌ Compiles but wrong!
    
    // Missing parameter - doesn't override!
    void show() { }  // ❌ Compiles but wrong!
    
    // Missing const - doesn't override!
    void print() { }  // ❌ Compiles but wrong!
};

// These compile but don't do what you expect!
```

**Solution With Override:**
```cpp
class Derived : public Base {
public:
    // ❌ Compiler error - catches typo!
    void dysplay() override { }
    
    // ❌ Compiler error - catches signature mismatch!
    void show() override { }
    
    // ❌ Compiler error - catches missing const!
    void print() override { }
    
    // ✅ Correct - compiles successfully
    void display() override { }
    void show(int x) override { }
    void print() const override { }
};
```

**Benefits of Override:**
- ✅ Catches typos in function names
- ✅ Catches signature mismatches
- ✅ Catches const/non-const mismatches
- ✅ Makes intent explicit
- ✅ Compile-time safety
- ✅ Self-documenting code

**⚠️ BEST PRACTICE:**
```
ALWAYS use 'override' when overriding virtual functions!
```

---

### Final Keyword (C++11)

**Final Function:**
```cpp
class Base {
public:
    // This function cannot be overridden further
    virtual void func() final {
        cout << "Base::func() - FINAL\n";
    }
};

class Derived : public Base {
    // ❌ Compiler error!
    void func() override { }
};
```

**Final Class:**
```cpp
// This class cannot be inherited from
class FinalClass final {
public:
    void method() { }
};

// ❌ Compiler error!
class CannotDerive : public FinalClass { };
```

**Uses of Final:**
- 🛡️ Prevent further overriding
- 🛡️ Prevent inheritance
- ⚡ Optimization (compiler knows no override)
- 📝 Design intent (this is the final implementation)

---

### Pure Virtual Functions

**Syntax:**
```cpp
class Abstract {
public:
    // Pure virtual function (= 0)
    virtual void func() = 0;
    
    // Pure virtual with implementation (rare)
    virtual void func2() = 0;
    
    virtual ~Abstract() = default;
};

// Cannot instantiate abstract class
// Abstract obj;  // ❌ Error!

// But can have pointers/references
Abstract* ptr;  // ✅ OK!
```

**Pure Virtual Function = Abstract Method**
- Declared with `= 0`
- No implementation (usually)
- Makes class abstract
- Derived classes MUST implement

**Concrete Class (Must Implement):**
```cpp
class Concrete : public Abstract {
public:
    // MUST implement pure virtual functions
    void func() override {
        cout << "Concrete implementation\n";
    }
    
    void func2() override {
        cout << "Concrete implementation 2\n";
    }
};

// Now can instantiate
Concrete obj;  // ✅ OK!
```

---

### Abstract Classes

**A class is abstract if:**
1. Has at least one pure virtual function, OR
2. Inherits pure virtual without implementing it

**Abstract Class Rules:**
```cpp
class AbstractBase {
protected:
    int data;  // ✅ Can have data members
    
public:
    // ✅ Can have constructor
    AbstractBase(int d) : data(d) { }
    
    // ✅ Can have regular functions
    void normalFunc() { }
    
    // ✅ Can have virtual functions
    virtual void virtualFunc() { }
    
    // Pure virtual function
    virtual void pureVirtualFunc() = 0;
    
    // ✅ Should have virtual destructor
    virtual ~AbstractBase() = default;
};

// ❌ Cannot instantiate
// AbstractBase obj;

// ✅ Can have pointers/references
AbstractBase* ptr;
AbstractBase& ref;

// ✅ Can have constructors (called by derived)
// ✅ Can have implementation for pure virtual (rare)
```

**What Abstract Classes Can Have:**
- ✅ Pure virtual functions
- ✅ Virtual functions
- ✅ Regular functions
- ✅ Data members
- ✅ Constructors
- ✅ Destructors (should be virtual)

**What They Cannot Do:**
- ❌ Cannot be instantiated directly
- ❌ Cannot be used by value (only pointers/references)

---

### Object Slicing

**The Problem:**
```cpp
class Base {
public:
    virtual void display() { cout << "Base\n"; }
    virtual ~Base() = default;
};

class Derived : public Base {
    int extraData;
public:
    void display() override { cout << "Derived\n"; }
};

Derived derived;

// ❌ Object slicing - derived part lost!
Base sliced = derived;
sliced.display();  // Prints "Base" (not "Derived")

// ✅ Correct - use pointer
Base* ptr = &derived;
ptr->display();  // Prints "Derived"

// ✅ Correct - use reference
Base& ref = derived;
ref.display();  // Prints "Derived"
```

**What Happens:**
```
Derived object:
┌──────────────┐
│ Base part    │
├──────────────┤
│ Derived part │  ← This gets sliced off!
│ extraData    │
└──────────────┘

After slicing (Base sliced = derived):
┌──────────────┐
│ Base part    │  ← Only this remains
└──────────────┘
```

**Problems with Slicing:**
- ❌ Derived data lost
- ❌ Virtual functions don't work
- ❌ Polymorphism lost
- ❌ Unexpected behavior

**Solution:**
```
⚠️ ALWAYS use pointers or references
   for polymorphic behavior!

Base* ptr = &derived;    ✅
Base& ref = derived;     ✅
Base sliced = derived;   ❌
```

---

### vtable Mechanism

**How Virtual Functions Work:**

Every class with virtual functions has:
1. **vtable (virtual table)** - stores function pointers
2. Every object has **vptr (virtual pointer)** - points to class vtable

**Visual:**
```
class Base {
    virtual void func1() { }
    virtual void func2() { }
};

class Derived : public Base {
    void func1() override { }
    // func2 not overridden
};

Base vtable:              Derived vtable:
┌─────────────────┐      ┌─────────────────────┐
│ func1() → Base  │      │ func1() → Derived   │
│ func2() → Base  │      │ func2() → Base      │
│ ~Base()         │      │ ~Derived()          │
└─────────────────┘      └─────────────────────┘

Base object:              Derived object:
┌─────────────────┐      ┌─────────────────────┐
│ vptr ─────────► │      │ vptr ─────────────► │
│ (data members)  │      │ (data members)      │
└─────────────────┘      └─────────────────────┘
     │                         │
     └────► Base vtable        └────► Derived vtable
```

**Function Call Process:**
```cpp
Base* ptr = new Derived();
ptr->func1();  // How is this resolved?
```

**Steps:**
1. Dereference `ptr` to get object
2. Follow `vptr` to vtable
3. Lookup `func1()` in vtable
4. Call the function pointer
5. Executes `Derived::func1()`

**Cost:**
- **Space:** One vptr per object (~8 bytes on 64-bit)
- **Time:** One extra indirection per virtual call
- **Usually negligible in practice**

---

## 💡 Common Patterns

### Interface Pattern (Pure Virtual)
```cpp
// Interface - only pure virtual functions
class IDrawable {
public:
    virtual void draw() const = 0;
    virtual ~IDrawable() = default;
};

class Circle : public IDrawable {
public:
    void draw() const override {
        cout << "Drawing circle\n";
    }
};

class Rectangle : public IDrawable {
public:
    void draw() const override {
        cout << "Drawing rectangle\n";
    }
};

// Polymorphic usage
void render(const IDrawable& shape) {
    shape.draw();
}
```

### Template Method Pattern
```cpp
class Algorithm {
public:
    // Template method (non-virtual)
    void execute() {
        step1();
        step2();  // Hook - can override
        step3();
    }
    
    virtual ~Algorithm() = default;

protected:
    void step1() { cout << "Step 1\n"; }
    virtual void step2() { }  // Hook
    void step3() { cout << "Step 3\n"; }
};

class ConcreteAlgorithm : public Algorithm {
protected:
    void step2() override {
        cout << "Custom Step 2\n";
    }
};
```

### Strategy Pattern
```cpp
class Strategy {
public:
    virtual void execute() = 0;
    virtual ~Strategy() = default;
};

class ConcreteStrategyA : public Strategy {
public:
    void execute() override {
        cout << "Strategy A\n";
    }
};

class ConcreteStrategyB : public Strategy {
public:
    void execute() override {
        cout << "Strategy B\n";
    }
};

class Context {
    Strategy* strategy;
public:
    void setStrategy(Strategy* s) { strategy = s; }
    void doWork() { strategy->execute(); }
};
```

---

## 🎯 Practice Exercises

### Exercise 1: Shape Hierarchy
Create abstract `Shape` class with:
- Pure virtual: `area()`, `perimeter()`
- Concrete classes: Circle, Rectangle, Triangle
- Test polymorphic collection

### Exercise 2: Animal Sounds
Create `Animal` hierarchy:
- Virtual: `makeSound()`, `move()`
- Derived: Dog, Cat, Bird
- Each with unique behaviors

### Exercise 3: Document System
Create abstract `Document`:
- Pure virtual: `open()`, `save()`, `print()`
- Concrete: PDFDocument, WordDocument
- Test polymorphic document management

### Exercise 4: Game Entities
Create `GameObject` base:
- Virtual: `update()`, `render()`, `handleInput()`
- Derived: Player, Enemy, Collectible
- Implement game loop

---

## 🚀 How to Run

### Compile:
```bash
g++ -std=c++17 virtual_functions.cpp -o virtual_functions
```

### Run:
```bash
./virtual_functions
```

### Expected Output:
The program demonstrates:
1. Static vs dynamic binding
2. Virtual functions in action
3. Virtual destructor importance
4. Override keyword usage
5. Final keyword
6. Pure virtual functions
7. Object slicing problem
8. vtable mechanism
9. Payment system example
10. Covariant return types

---

## 📊 Visual Concepts

### Static vs Dynamic Binding
```
STATIC BINDING (Compile-time):
    Code                    Compiled
Base* ptr = &derived;  →  Call Base::func()
ptr->func();              (Direct address)

DYNAMIC BINDING (Runtime):
    Code                    Runtime
Base* ptr = &derived;  →  Follow vptr
ptr->func();              → Lookup vtable
                          → Call Derived::func()
```

### Virtual Function Call
```
1. Code:
   ptr->virtualFunc();

2. Compiled to:
   (*ptr->vptr[index])();

3. At runtime:
   ┌─────┐
   │ ptr │──┐
   └─────┘  │
            ▼
   ┌─────────────┐
   │   Object    │
   │ vptr ───────┼──┐
   └─────────────┘  │
                    ▼
            ┌──────────────┐
            │    vtable    │
            │ [0] func1()  │
            │ [1] func2()  │◄── index
            │ [2] ...      │
            └──────────────┘
                    │
                    ▼
            ┌──────────────┐
            │  Function    │
            │    Code      │
            └──────────────┘
```

### Abstract Class Hierarchy
```
        ┌─────────────┐
        │   Shape     │ (Abstract)
        │  ────────   │
        │ + area()=0  │ Pure virtual
        │ + draw()    │ Virtual
        └─────────────┘
               ▲
               │ implements
        ┌──────┴──────┐
        │             │
┌───────▼───────┐ ┌──▼─────────┐
│    Circle     │ │  Rectangle │ (Concrete)
│  ──────────   │ │  ────────  │
│ + area()      │ │ + area()   │ Must implement
│ + draw()      │ │ + draw()   │
└───────────────┘ └────────────┘
```

---

## 📚 Related Topics

### Prerequisites:
- **13_Inheritance** - Basic inheritance
- **05_Encapsulation** - Access control
- **07_CopyConstructor** - Constructors

### Coming Next:
- **15_Polymorphism** - Advanced polymorphism
- **16_AbstractClasses** - Interface design
- **17_OperatorOverloading** - With virtual

### Related Concepts:
- **Dynamic Binding** - Runtime resolution
- **vtable/vptr** - Implementation mechanism
- **RTTI** - Runtime type information

---

## 🎓 Summary

### What We Learned:

✅ **Virtual functions** enable runtime polymorphism
✅ **Dynamic binding** resolves at runtime
✅ **Virtual destructor** prevents memory leaks
✅ **Override** keyword provides compile-time safety
✅ **Final** keyword prevents further overriding
✅ **Pure virtual** (= 0) creates abstract classes
✅ **Abstract classes** cannot be instantiated
✅ **Object slicing** loses polymorphic behavior
✅ **vtable/vptr** mechanism enables virtual calls
✅ **Always use pointers/references** for polymorphism

### The Complete Pattern:
```cpp
// Abstract base class
class Base {
public:
    // Virtual function
    virtual void regularVirtual() {
        cout << "Base implementation\n";
    }
    
    // Pure virtual function
    virtual void mustImplement() = 0;
    
    // Virtual destructor (CRITICAL!)
    virtual ~Base() = default;
};

// Concrete derived class
class Derived : public Base {
public:
    // Override with override keyword
    void regularVirtual() override {
        cout << "Derived implementation\n";
    }
    
    // Must implement pure virtual
    void mustImplement() override {
        cout << "Derived implementation\n";
    }
};

// Polymorphic usage
Base* ptr = new Derived();
ptr->regularVirtual();   // Calls Derived version
ptr->mustImplement();    // Calls Derived version
delete ptr;              // Calls both destructors
```

---

## ✨ Quick Reference Card
```cpp
// VIRTUAL FUNCTION
class Base {
public:
    virtual void func() {  // Can override
        cout << "Base\n";
    }
    virtual ~Base() { }    // ALWAYS virtual!
};

// OVERRIDE
class Derived : public Base {
public:
    void func() override {  // Override safely
        cout << "Derived\n";
    }
};

// PURE VIRTUAL (Abstract)
class Abstract {
public:
    virtual void pure() = 0;  // Must implement
    virtual ~Abstract() = default;
};

// FINAL
class Final {
    virtual void func() final { }  // Cannot override
};

class FinalClass final { };  // Cannot inherit

// USAGE
Base* ptr = new Derived();
ptr->func();  // Calls Derived::func()
delete ptr;   // Calls both destructors
```

---

## 🔍 Best Practices

### ✅ DO:

- **Make destructors virtual** in polymorphic classes (ALWAYS!)
- **Use `override` keyword** in derived classes (ALWAYS!)
- **Use `final`** when you want to prevent overriding
- **Use pure virtual** for interface contracts
- **Use pointers/references** for polymorphism
- **Document virtual functions** clearly
- **Keep virtual function signature** exactly the same
- **Consider performance** (vtable overhead)

### ❌ DON'T:

- **Forget virtual destructor** (memory leaks!)
- **Omit `override` keyword** (error-prone)
- **Call virtual functions** in constructors/destructors
- **Pass by value** for polymorphic objects (slicing!)
- **Make everything virtual** (unnecessary overhead)
- **Use virtual** for small, frequently-called functions
- **Mix up signatures** (use override to catch)
- **Forget that derived must implement** pure virtual

---

## 🌟 Real-World Applications

### GUI Systems
```cpp
class Widget {
    virtual void render() = 0;
    virtual void handleEvent() = 0;
};
class Button : public Widget { };
class TextBox : public Widget { };
```

### Game Engines
```cpp
class GameObject {
    virtual void update() = 0;
    virtual void render() = 0;
};
class Player : public GameObject { };
class Enemy : public GameObject { };
```

### Plugin Systems
```cpp
class Plugin {
    virtual void initialize() = 0;
    virtual void execute() = 0;
};
class AudioPlugin : public Plugin { };
class VideoPlugin : public Plugin { };
```

### Document Processing
```cpp
class Document {
    virtual void open() = 0;
    virtual void save() = 0;
};
class PDFDocument : public Document { };
class WordDocument : public Document { };
```

---

## 📖 Further Reading

- [cppreference.com - Virtual Functions](https://en.cppreference.com/w/cpp/language/virtual)
- "Effective C++" by Scott Meyers - Items 7, 34-40
- "More Effective C++" by Scott Meyers - Item 24
- [C++ Core Guidelines - Virtual Functions](https://isocpp.github.io/CppCoreGuidelines/)

---

## 🎯 Key Takeaways

1. Virtual functions enable **runtime polymorphism**
2. Use **`virtual`** keyword in base class
3. Use **`override`** keyword in derived class
4. **ALWAYS** make destructor virtual
5. Pure virtual **(= 0)** creates abstract class
6. Abstract classes **cannot be instantiated**
7. Use **pointers/references** for polymorphism
8. **Object slicing** loses derived part
9. **vtable/vptr** mechanism has small overhead
10. **`final`** prevents further overriding

---

**Previous Topic:** [13_Inheritance](../13_Inheritance/) - Inheritance Basics

**Next Topic:** [15_Polymorphism](../15_Polymorphism/) - Advanced Polymorphism (coming next)

---

*Part of the C++ Classes and Objects series*
*Difficulty: Intermediate-Advanced*
*Prerequisites: 13_Inheritance*
*Foundation for True OOP*