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
public:
    A() { cout << "1. A\n"; }
};

class B {
public:
    B() { cout << "2. B\n"; }
};

class C {
public:
    C() { cout << "3. C\n"; }
};

class D : public A, public B, public C {
public:
    D() { cout << "4. D\n"; }
};

// Output:
// 1. A
// 2. B
// 3. C
// 4. D
```
✅ tested. (An earlier version of this snippet had no `public:`, so all the constructors were private and it did not compile: `'A::A()' is private within this context`.) The order follows the order in the *base-specifier list* (`: public A, public B, public C`), not the order in a mem-initializer list.

**Destruction order: exactly REVERSE (the derived destructor first, then the bases from right to left)**

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

**Solution 2: Declare the function in the derived class**
```cpp
class Derived : public ClassA, public ClassB {
public:
    void display() {  // Hides both bases' display(), so the name is unambiguous
        ClassA::display();  // Choose which to call
        // or ClassB::display();
        // or both!
    }
};
```
This **hides** the two base functions; it is not overriding, because they are not `virtual`. ✅ tested: writing `void display() override` here does not compile (`marked 'override', but does not override`) - the earlier version of this snippet had `override`. Example 2 also prints a compile-time check (`requires { t.commonFunc(); }`): it is `true` for `ClassA` alone and `false` for the class derived from both.

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
        int phoneId  = Phone::id;    // = 1
        int cameraId = Camera::id;   // = 2
    }
};
```

**Problems:**
- ❌ SmartPhone contains **TWO Device objects**
- ❌ Ambiguous access to Device members
- ❌ Wasted memory
- ❌ Inconsistent state (two different ids!)
- ❌ Confusing behavior

✅ tested: `sm.powerOn()` is ambiguous, `Device* p = &sm;` is `'Device' is an ambiguous base of 'SmartPhone'`; in Example 3 `is_convertible_v<SmartPhone*, Device*>` is `false` and `sizeof(SmartPhone)` is 96 with GCC on x86-64 (two `Device` objects) versus 64 for the virtual version in Example 4.

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
3. Most derived class calls base constructor (✅ Example 4 prints just one `VDevice constructor: iPhone (ID: 12345)`; the default arguments in `VPhone()` / `VCamera()` are never used for it)
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

**Why this is (mostly) safe:**
- ✅ No duplicated or inconsistent state (interfaces hold no data)
- ✅ Same-named pure virtual functions in two interfaces are implemented by one override ✅ tested (`P::run` and `Q::run` are both implemented by a single `PQ::run`)
- ✅ Clear contracts
- ✅ Flexible polymorphism
- ✅ Common in modern C++ design (Core Guidelines [C.135](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rh-mi-interface))
- ⚠️ A *diamond of interfaces* still exists if two interfaces derive from a common interface: ✅ tested `struct IA : I {}; struct IB : I {}; struct C : IA, IB {};` makes `I* p = &c;` the error `'I' is an ambiguous base of 'C'`. Deriving the shared interface virtually (`struct IA : virtual I {}`) fixes it ✅ tested.

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
│ Fixed at compile time   │ Swappable if held by    │
│                         │ pointer/interface       │
│ Harder to mock          │ Easier to mock/inject   │
│ Less flexible           │ More flexible           │
└─────────────────────────┴─────────────────────────┘
```

Table rows are rules of thumb. Composition *by value* is also fixed at compile time; swapping an implementation at run time needs a pointer, reference or `std::function` to an interface.

**Gang of Four Principle (from memory, not re-checked):**
> "Favor object composition over class inheritance."

---

## 💡 When to Use Multiple Inheritance

### ✅ GOOD Use Cases

#### 1. Multiple Pure Interfaces
```cpp
class Widget : public IDrawable, public ISerializable, public IClickable {
    // Implement multiple interfaces
    // No duplicated state (no data)
    // Generally safe multiple inheritance
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

**Careful:** `Loggable<Timestamped<MyClass>>` is a *chain of single inheritance* (each mixin derives from the class passed to it), so it is multilevel inheritance, not multiple inheritance. Real multiple-inheritance mixins are ordinary bases: `class Widget : public Loggable, public Timestamped`. In Example 6 the mixins need forwarding constructors (`template<typename... Args> explicit Timestamped(Args&&... args) : Base(std::forward<Args>(args)...)`) to pass the arguments down the chain.

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
- You need to pass the object where *either* base is expected (substitutability for both); if not, composition is simpler

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
g++ -std=c++23 -Wall -Wextra main.cpp -o multiple_inheritance
```

`main.cpp` uses `std::println` (`<print>`, GCC 14+ or a recent Clang with `-std=c++23`) and one C++20 `requires` expression.

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

✅ Safe - no shared state
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
✅ **Constructor order** = left to right (the base-specifier order)
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
public:
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
public:
    D() : A(), B(), C() { }
};

// AMBIGUITY RESOLUTION
Base1::func();  // Explicit scope

// VIRTUAL INHERITANCE (diamond solution)
class A : virtual public Base { };
class B : virtual public Base { };
class C : public A, public B {
public:
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
- **Give interfaces a virtual destructor** (needed for deletion through a base pointer, [C.35](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rc-dtor-virtual))
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
- **Ignore ambiguity** (it is a hard compile error, not a warning: qualify the call or declare the function in the derived class)
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

- [cppreference.com - Derived classes](https://en.cppreference.com/cpp/language/derived_class) (covers multiple and virtual inheritance)
- "Effective C++" (3rd ed.) by Scott Meyers - Item 40, "Use multiple inheritance judiciously"
- "Design Patterns" by Gamma, Helm, Johnson, Vlissides - the chapter 1 discussion of class vs object composition
- C++ Core Guidelines: [C.135 multiple distinct interfaces](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rh-mi-interface), [C.136 union of implementation attributes](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rh-mi-implementation), [C.137 virtual bases](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rh-vbase)

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
10. **Favor composition over inheritance** - GoF

---

**Previous Topic:** [15_AbstractClasses](../15_AbstractClasses/) - Abstract Classes

**Next Topic:** [17_Templates](../17_Templates/) - Generic Programming (coming next)

---

*Part of the C++ Classes and Objects series*
*Difficulty: Advanced*
*Prerequisites: 13_Inheritance, 14_VirtualFunctions*
*Use with Caution - Powerful but Complex*

---

## 🧾 Notes moved from main.cpp

Per-example points from the removed `✅`/`⚠️`/`💡` blocks (not repeated where the sections above already say them):

- **Example 1:** `Car : public Engine, public Wheels` combines both; constructors run left to right, destructors right to left. New in the program: it prints where the base subobjects sit (`Engine` at offset 0, `Wheels` at offset 4 with GCC on x86-64). Converting a `Car*` to `Wheels*` therefore **changes the address** - one more difference from single inheritance.
- **Example 2:** ambiguity occurs when several bases have a member with the same name; the compiler does not guess, so qualify the call (`obj.ClassA::commonFunc()`) or declare the function in the derived class (which hides both).
- **Example 3:** two `Device` objects, ambiguous access, wasted memory and inconsistent state (the two IDs 1 and 2).
- **Example 4:** one shared `VDevice`, no ambiguity, consistent state, the most derived class constructs the virtual base.
- **Example 5:** only pure virtual functions → safe and common. (The old note "no diamond problem (no data)" was too strong, see the corrected list above.)
- **Example 6:** small reusable features that are composed flexibly and avoid an explosion of hierarchy classes - but note it is a chain of single inheritance.
- **Example 7:** regular: bases left to right, then derived. With virtual bases: virtual bases first, then the others. Destruction is always the exact reverse. The destruction labels were wrong (`3. 4. 5. 6.` and `2. 4. 5. 6.`); they now read `1. 2. 3. 4.` in the order they actually run, and the "Destruction order (reverse):" line is printed before the destructors, not after them.
- **Example 8:** use multiple inheritance for multiple *interfaces* (pure virtual), no implementation conflicts, real IS-A for all bases; avoid it for implementation reuse, when composition works, when a diamond is likely.
- **Example 9:** interfaces enable flexible role combinations (`TechLead` is an `IManager` and an `IDeveloper`) and interface-based polymorphism.
- **Example 10:** composition: loose coupling, flexibility, easier testing, swappable implementations, no diamond. The Gang of Four wording is "Favor object composition over class inheritance" (the printed quote used a different wording).

Other lecture content that was only in `main.cpp` (syntax, constructor-order lists, ambiguity, diamond, virtual inheritance, interfaces, "when to use", composition, best practices, common mistakes, quick reference) is already in the sections above. Two details were only in the lecture:

- **Common mistakes:** (1) forgetting `virtual` on the two middle bases of a diamond; (2) calling an ambiguous function without qualification; (3) forgetting that the *most derived* class must initialize the virtual base (✅ tested: if `SmartPhone` omits `Device(n)` and `Device` has no default constructor it does not compile); (4) using multiple inheritance where composition is enough.
- **With virtual bases:** 1. virtual base constructors, 2. non-virtual bases left to right, 3. derived constructor.

**Code changes in `main.cpp`:**
  - Example 2: the comment "Can override to resolve ambiguity" was wrong (hiding, not overriding); a compile-time check demo replaces the lecture text.
  - Example 1/3/4: real demos (subobject offsets, `is_convertible_v`, `sizeof`; sizes are for GCC on x86-64).
  - Example 6: `std::forward` was called unqualified (Clang `-Wunqualified-std-cast-call`); `<utility>` added.
  - Example 7: destruction labels fixed, label line moved before the destructors.
  - One-argument constructors made `explicit` (see 23_ExplicitKeyword); `using namespace std;` removed.
  - ✅ tested with `-Wall -Wextra`, AddressSanitizer and UndefinedBehaviorSanitizer (and Clang `-Wall -Wextra`); output identical to the original apart from the changes listed.

---

## ⚠️ Open points

- Numbering: this README calls itself `16_MultipleInheritance`, but your other folders already use `16_Polymorphism` and `17_VirtualDestructor`. "Coming Next" (17_Templates, 18_STL, 19_DesignPatterns) and "Next Topic" were left as they are because I don't know the real next folder; the "Previous Topic" (15_AbstractClasses) and the prerequisites (13_Inheritance, 14_VirtualFunctions, 15_AbstractClasses) match your folders.
- Not opened / not tested: the cppreference link (taken from a search result) and the Core Guidelines anchors (taken from the guidelines text); "Effective C++ Item 40" and the Gang of Four wording are from memory. The paper "Multiple Inheritance Considered Useful" was removed because I could not verify it.
