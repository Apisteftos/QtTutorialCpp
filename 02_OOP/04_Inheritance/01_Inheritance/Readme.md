# 13_Inheritance - Code Reuse Through Hierarchies

## 📚 Overview

This folder introduces **Inheritance** - one of the fundamental pillars of Object-Oriented Programming. Inheritance allows you to create new classes from existing classes, promoting code reuse and establishing hierarchical relationships. Together with `virtual` functions it is also the foundation for polymorphism.

**Inheritance** = Creating new classes from existing ones (IS-A relationship)

This is a crucial concept for building maintainable, organized, and scalable C++ applications.

---

## 🎯 What You'll Learn

- What inheritance is and why it's important
- Basic inheritance syntax
- Types of inheritance (single, multiple, multilevel, hierarchical, hybrid)
- Access specifiers in inheritance (public, protected, private)
- Constructor and destructor calling order
- Protected access modifier
- Function overriding
- Multiple inheritance
- Diamond problem and virtual inheritance
- IS-A vs HAS-A relationships
- Real-world examples and best practices

---

## 📖 Concept Explanation

### What is Inheritance?

**Inheritance** is a mechanism where a new class (derived class) is created from an existing class (base class), inheriting its members and behaviors.

**Key Benefits:**
- **Code reuse** - Don't repeat yourself
- **Hierarchical classification** - Natural organization
- **Extensibility** - Add new features easily
- **Polymorphism** - inheritance plus `virtual` functions makes dynamic behavior possible (inheritance alone does not)

### Real-World Analogy
```
Think of biological classification:

Animal (Base Class)
  - Has: name, age
  - Can: eat(), sleep()
  
Dog (Derived Class) IS-A Animal
  - Inherits: name, age, eat(), sleep()
  - Adds: breed
  - Adds: bark()
  
Cat (Derived Class) IS-A Animal
  - Inherits: name, age, eat(), sleep()
  - Adds: color
  - Adds: meow()

Both Dog and Cat inherit common properties from Animal,
but each adds its own specialized features.
```

### Visual Representation
```
┌─────────────────┐
│     Animal      │  (Base Class)
│  ─────────────  │
│  # name: string │
│  # age: int     │
│  + eat()        │
│  + sleep()      │
└─────────────────┘
         ▲
         │ inherits
    ┌────┴────┐
    │         │
┌───▼──────┐  ┌───▼──────┐
│   Dog    │  │   Cat    │  (Derived Classes)
│  ──────  │  │  ──────  │
│  - breed │  │  - color │
│  + bark()│  │  + meow()│
└──────────┘  └──────────┘
```
`+` public, `#` protected, `-` private (as in `main.cpp`: `name` and `age` are protected).

---

## 💻 Basic Syntax

### Simple Inheritance
```cpp
// Base class
class Animal {
protected:  // Accessible to derived classes
    string name;
    int age;
    
public:
    Animal(string n, int a) : name(n), age(a) { }
    
    void eat() {
        cout << name << " is eating\n";
    }
    
    void sleep() {
        cout << name << " is sleeping\n";
    }
};

// Derived class
class Dog : public Animal {  // public inheritance
private:
    string breed;
    
public:
    // Must call base constructor
    Dog(string n, int a, string b) : Animal(n, a), breed(b) { }
    
    // Dog-specific method
    void bark() {
        cout << name << " says: Woof!\n";
    }
};

// Usage
Dog dog("Buddy", 3, "Golden Retriever");
dog.eat();    // Inherited from Animal
dog.sleep();  // Inherited from Animal
dog.bark();   // Dog's own method
```

### The Three Parts

1. **Base class definition** - What can be inherited
2. **Inheritance declaration** - `class Derived : public Base`
3. **Derived class definition** - Add new features

---

## 🔍 Types of Inheritance

### 1. Single Inheritance

**One base class → One derived class**
```cpp
class A { };
class B : public A { };

     A
     ↓
     B
```

**Most common and simplest form.**

### 2. Multiple Inheritance

**Multiple base classes → One derived class**
```cpp
class A { };
class B { };
class C : public A, public B { };

   A   B
    \ /
     C
```

**Use carefully - can lead to complexity.**

### 3. Multilevel Inheritance

**Chain of inheritance**
```cpp
class A { };
class B : public A { };
class C : public B { };

     A
     ↓
     B
     ↓
     C
```

**Creates hierarchy of specialization.**

### 4. Hierarchical Inheritance

**One base class → Multiple derived classes**
```cpp
class A { };
class B : public A { };
class C : public A { };

     A
    / \
   B   C
```

**Common base functionality for different types.**

### 5. Hybrid Inheritance

**Combination of above types**
```cpp
class A { };
class B : public A { };
class C : public A { };
class D : public B, public C { };

       A
      / \
     B   C
      \ /
       D
```

**Can cause diamond problem (see below).**

---

## 📋 Examples in This Folder

### Example 1: Basic Inheritance
Understanding fundamental inheritance concepts.

### Example 2: Access Specifiers in Inheritance
How public/protected/private inheritance works.

### Example 3: Constructor and Destructor Calls
Order of construction and destruction.

### Example 4: Function Hiding
Derived class providing a new implementation of a non-virtual function (hiding, not overriding).

### Example 5: Protected Access Modifier
When and why to use protected members.

### Example 6: Multiple Inheritance
Inheriting from multiple base classes.

### Example 7: Diamond Problem
The ambiguity issue with multiple inheritance.

### Example 8: Virtual Inheritance
Solving the diamond problem.

### Example 9: Employee Hierarchy
Real-world organizational structure.

### Example 10: IS-A vs HAS-A
Inheritance vs composition decision.

---

## 🎓 Key Concepts

### Access Specifiers in Inheritance

#### Public Inheritance (Most Common)
```cpp
class Base {
public:    int pub;
protected: int prot;
private:   int priv;
};

class Derived : public Base {
    // pub    → public    (✅ accessible, stays public)
    // prot   → protected (✅ accessible, stays protected)
    // priv   → N/A       (❌ not accessible)
};

// Outside code
Derived d;
d.pub = 10;   // ✅ Still public
// d.prot = 10;  // ❌ Protected
// d.priv = 10;  // ❌ Private
```

**The default choice - represents an IS-A relationship** (opinion: in practice almost all inheritance in real code is public).

#### Protected Inheritance (Rarely Used)
```cpp
class Derived : protected Base {
    // pub    → protected (✅ accessible, now protected)
    // prot   → protected (✅ accessible, stays protected)
    // priv   → N/A       (❌ not accessible)
};

// Outside code
Derived d;
// d.pub = 10;  // ❌ Now protected!
```

**Rarely used.** Outside code (and anything that is not a member or friend of `Derived`) cannot convert `Derived*` to `Base*`, so there is no substitutability - it is not an IS-A relationship. ✅ tested: `is_convertible_v<PublicDerived*, Base*>` is `true`, for `ProtectedDerived*` and `PrivateDerived*` it is `false` (Example 2 prints this); `Base* b = &protectedDerived;` outside the class is the error `'Base' is an inaccessible base`.

#### Private Inheritance (Use Composition Instead)
```cpp
class Derived : private Base {
    // pub    → private   (✅ accessible, now private)
    // prot   → private   (✅ accessible, now private)
    // priv   → N/A       (❌ not accessible)
};

// Outside code
Derived d;
// d.pub = 10;  // ❌ Now private!
```

**Usually composition (HAS-A) is the better tool** - private inheritance is mainly useful to reuse a base's implementation or override its virtual functions without exposing it.

**`class` vs `struct` default:** `class D : Base` inherits privately, `struct D : Base` inherits publicly. ✅ tested: `is_convertible_v<D*, Base*>` is `false` for the `class`, `true` for the `struct`; `cd.x` on the `class` version is the error `'int Base::x' is inaccessible`. Always write the access specifier.

### Summary Table
```
┌─────────────────┬────────────┬─────────────┬─────────────┐
│ Base Access     │ public     │ protected   │ private     │
│                 │ inheritance│ inheritance │ inheritance │
├─────────────────┼────────────┼─────────────┼─────────────┤
│ public          │ public     │ protected   │ private     │
│ protected       │ protected  │ protected   │ private     │
│ private         │ N/A        │ N/A         │ N/A         │
└─────────────────┴────────────┴─────────────┴─────────────┘
```

---

## 💡 Protected Access Modifier

### What is Protected?

**Protected members:**
- ❌ Not accessible to outside code
- ✅ Accessible to derived classes
- ✅ Balance between private and public

### When to Use Protected
```cpp
class BankAccount {
private:
    string accountNumber;  // Only BankAccount can access
    
protected:
    double balance;        // Derived classes can access
    
public:
    BankAccount(string acc, double bal) 
        : accountNumber(acc), balance(bal) { }
    
    double getBalance() const {
        return balance;
    }
};

class SavingsAccount : public BankAccount {
public:
    SavingsAccount(string acc, double bal, double rate)
        : BankAccount(acc, bal), interestRate(rate) { }
    
    void addInterest() {
        // Can access protected balance ✅
        balance += balance * interestRate;
    }
    
private:
    double interestRate;
};

// Outside code
SavingsAccount savings("SA-123", 1000, 0.05);
// savings.balance = 5000;  // ❌ Error! Protected
savings.addInterest();      // ✅ OK! Public method
```

**Use protected for:**
- Data that derived classes need to access/modify
- Implementation details shared in hierarchy
- Extension points for derived classes

**Avoid protected for:**
- Everything (breaks encapsulation)
- Public interface (use public methods)
- Data members in general: the C++ Core Guidelines advise against protected *data* ([C.133](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rh-protected)) because it spreads the class invariant over the whole hierarchy. The examples here use it to show the mechanism; protected *functions* are the safer extension point.

---

## 💡 Constructor and Destructor Order

### Construction Order (Base → Derived)
```cpp
class GrandParent {
public:
    GrandParent() { cout << "1. GrandParent\n"; }
};

class Parent : public GrandParent {
public:
    Parent() { cout << "2. Parent\n"; }
};

class Child : public Parent {
public:
    Child() { cout << "3. Child\n"; }
};

Child c;
// Output:
// 1. GrandParent
// 2. Parent
// 3. Child
```

### Destruction Order (Derived → Base)
```cpp
class GrandParent {
public:
    ~GrandParent() { cout << "3. GrandParent\n"; }
};

class Parent : public GrandParent {
public:
    ~Parent() { cout << "2. Parent\n"; }
};

class Child : public Parent {
public:
    ~Child() { cout << "1. Child\n"; }
};

Child c;
// When c is destroyed:
// 1. Child
// 2. Parent
// 3. GrandParent
```

### Calling Base Constructor
```cpp
class Base {
    int data;
public:
    Base(int d) : data(d) { }
};

class Derived : public Base {
    int derivedData;
public:
    // MUST call base constructor explicitly
    Derived(int d, int dd) : Base(d), derivedData(dd) { }
    
    // ❌ Wrong: Derived(int d, int dd) { Base(d); }
    // Does NOT compile here: Base has no default constructor, so the base
    // subobject cannot be built before the body runs.
    // (And even if Base had one, "Base(d);" in the body would only create
    // a separate object / declare a variable named d, never initialize the base.)
};
```
✅ tested: `Derived(int d, int dd) { Base(d); }` fails with `no matching function for call to 'Base::Base()'`, and so does a constructor that simply omits `Base(d)` from the initializer list.

**Key Rule:** Base constructor is called FIRST, before derived constructor body executes.

---

## 💡 Function Hiding vs. Overriding

### Hiding (no `virtual`)
```cpp
class Shape {
public:
    void draw() {
        cout << "Drawing generic shape\n";
    }
};

class Circle : public Shape {
public:
    // Hides Shape::draw (not virtual -> this is hiding, not overriding)
    void draw() {
        cout << "Drawing circle\n";
    }
};

Shape s;
Circle c;

s.draw();  // "Drawing generic shape"
c.draw();  // "Drawing circle"

Shape* p = &c;
p->draw(); // "Drawing generic shape" - the static type decides!
```
✅ tested; Example 4 in `main.cpp` shows the last call.

### Overriding (`virtual` + `override`)
```cpp
class Shape {
public:
    virtual ~Shape() = default;
    virtual void draw() { cout << "Drawing generic shape\n"; }
};

class Circle : public Shape {
public:
    void draw() override { cout << "Drawing circle\n"; }
};

Shape* p = new Circle;
p->draw();  // "Drawing circle" - chosen at run time
delete p;
```
`override` makes the compiler check that a base function really is overridden. ✅ tested: `override` on a function that is not virtual in the base is the error `marked 'override', but does not override`.

### Calling Base Version
```cpp
class Derived : public Base {
public:
    void display() {
        Base::display();  // Call base version
        cout << "Additional info\n";
    }
};
```

**Note:** Without the `virtual` keyword the derived function only hides the base one and there is no polymorphism. See 14_VirtualFunctions for that.

---

## 💡 Multiple Inheritance

### Syntax
```cpp
class Flyer {
public:
    void fly() { cout << "Flying\n"; }
};

class Swimmer {
public:
    void swim() { cout << "Swimming\n"; }
};

// Duck can both fly and swim
class Duck : public Flyer, public Swimmer {
public:
    void quack() { cout << "Quack!\n"; }
};

// Usage
Duck duck;
duck.fly();   // From Flyer
duck.swim();  // From Swimmer
duck.quack(); // Duck's own
```

### Pros and Cons

**✅ Advantages:**
- Combine functionality from multiple sources
- Model complex relationships
- Code reuse from multiple parents

**❌ Disadvantages:**
- Increased complexity
- Diamond problem
- Ambiguity issues
- Harder to maintain

**General Rule:** Use sparingly, prefer composition!

---

## 💡 Diamond Problem

### The Problem
```cpp
class Device {
    string name;
public:
    Device(string n) : name(n) { }
    void powerOn() { cout << name << " on\n"; }
};

class Phone : public Device {
public:
    Phone() : Device("Phone") { }
};

class Camera : public Device {
public:
    Camera() : Device("Camera") { }
};

// SmartPhone inherits Device TWICE!
class SmartPhone : public Phone, public Camera {
public:
    SmartPhone() : Phone(), Camera() { }
    
    void use() {
        // powerOn();  // ❌ Error! Ambiguous!
        // Which Device? Phone's or Camera's?
        
        Phone::powerOn();   // Must specify
        Camera::powerOn();  // Or this one
    }
};
```
✅ tested: `sm.powerOn()` gives `request for member 'powerOn' is ambiguous`, and `Device* p = &sm;` gives `'Device' is an ambiguous base of 'SmartPhone'`. Example 7 prints the same fact with `is_convertible_v<SmartPhone*, Device*>` (`false`) and `sizeof(SmartPhone)` (64 with GCC on x86-64: two `Device` objects).

**Problem:** SmartPhone has **TWO copies** of Device!
```
       Device
       /    \
    Phone  Camera
       \    /
     SmartPhone

SmartPhone contains:
- Device (via Phone)
- Device (via Camera)  ← Two copies!
```

### The Solution: Virtual Inheritance
```cpp
class Device {
    string name;
public:
    Device(string n) : name(n) { }
    void powerOn() { cout << name << " on\n"; }
};

// VIRTUAL inheritance
class Phone : virtual public Device {
public:
    Phone(string n) : Device(n) { }
};

// VIRTUAL inheritance
class Camera : virtual public Device {
public:
    Camera(string n) : Device(n) { }
};

class SmartPhone : public Phone, public Camera {
public:
    // Must call Device constructor directly
    SmartPhone(string n) 
        : Device(n), Phone(n), Camera(n) { }
    
    void use() {
        powerOn();  // ✅ No ambiguity!
    }
};
```

**Solution:** Only **ONE copy** of Device!
```
       Device
       /    \
    Phone  Camera
       \    /
     SmartPhone

SmartPhone contains:
- ONE Device (shared)  ✅
```

### Virtual Inheritance Rules

1. Use `virtual` keyword: `class B : virtual public A`
2. The most derived class initializes the virtual base (`SmartPhone` above); the `Device(n)` calls in `Phone`/`Camera` are ignored when a `SmartPhone` is built
3. Only one copy of base class exists
4. Resolves diamond problem

✅ tested: with `Device("S")` in `SmartPhone` only one `Device` is constructed (`Dv(S)`, not `Dv(S-pv)` or `Dv(S-cv)`). If `SmartPhone` omits `Device(n)` and `Device` has no default constructor, it does not compile (`no matching function for call to 'Device::Device()'`).

---

## 💡 IS-A vs HAS-A Relationships

### IS-A (Inheritance)

**Use inheritance when derived class IS-A type of base class.**
```cpp
class Animal {
    string name;
public:
    void eat() { }
};

// Dog IS-A Animal ✅
class Dog : public Animal {
    string breed;
public:
    void bark() { }
};
```

**Examples:**
- Dog IS-A Animal ✅
- Car IS-A Vehicle ✅
- Circle IS-A Shape ✅
- Student IS-A Person ✅

**Test:** Can you say "X IS-A Y" naturally? Use inheritance.

### HAS-A (Composition)

**Use composition when one class HAS-A member of another class.**
```cpp
class Engine {
    int horsepower;
public:
    void start() { }
};

// Car HAS-A Engine ✅
class Car {
    Engine engine;  // Composition
    string brand;
public:
    void drive() {
        engine.start();
    }
};
```

**Examples:**
- Car HAS-A Engine ✅
- Book HAS-A Author ✅
- House HAS-A Kitchen ✅
- Computer HAS-A CPU ✅

**Test:** Can you say "X HAS-A Y"? Use composition.

### When to Use Which?
```
┌──────────────────────┬─────────────────────────┐
│ Use Inheritance      │ Use Composition         │
├──────────────────────┼─────────────────────────┤
│ IS-A relationship    │ HAS-A relationship      │
│ Substitutability     │ Delegation              │
│ Shared behavior      │ Separate concerns       │
│ Specialization       │ Flexibility             │
│ Polymorphism needed  │ Avoid coupling          │
└──────────────────────┴─────────────────────────┘
```

**General Principle:** "Prefer composition over inheritance"

**Why?**
- More flexible
- Easier to change
- Looser coupling
- Easier to test

**Use inheritance when:**
- True IS-A relationship
- Polymorphism needed
- Shared behavior across hierarchy

---

## 💡 Common Patterns

### Base Class Pattern
```cpp
class Base {
protected:
    // Data derived classes need
    
public:
    // Constructor
    Base() { }
    
    // Virtual destructor: needed when objects are deleted through a Base*
    // (if the class is not meant for that, make it protected and non-virtual - C.35)
    virtual ~Base() { }
    
    // Public interface (virtual, so Derived can override it)
    virtual void publicMethod() { }
    
protected:
    // Protected methods for derived classes
    void protectedHelper() { }
};
```

### Derived Class Pattern
```cpp
class Derived : public Base {
private:
    // Derived-specific data
    
public:
    // Constructor (calls base constructor)
    Derived() : Base() { }
    
    // Override methods (publicMethod must be virtual in Base)
    void publicMethod() override {
        Base::publicMethod();  // Call base if needed
        // Add derived behavior
    }
};
```

---

## 🎯 Practice Exercises

### Exercise 1: Vehicle Hierarchy
Create a `Vehicle` base class with:
- Common properties: brand, year, speed
- Derived classes: Car, Motorcycle, Truck
- Each with specific properties and methods

### Exercise 2: Shape Hierarchy
Create a `Shape` base class with:
- Protected: color
- Public: area(), perimeter()
- Derived: Circle, Rectangle, Triangle
- Test all calculations

### Exercise 3: Employee System
Create an `Employee` hierarchy:
- Base: Employee (name, id, salary)
- Derived: Manager, Developer, Designer
- Each with specific attributes
- Calculate total payroll

### Exercise 4: Account System
Create a `BankAccount` base class:
- Protected: balance
- Derived: SavingsAccount, CheckingAccount
- Each with specific rules
- Test deposits and withdrawals

---

## 🚀 How to Run

### Compile:
```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o inheritance
```

`main.cpp` uses `std::println` (`<print>`), so it needs GCC 14+ or a recent Clang with `-std=c++23`.

### Run:
```bash
./inheritance
```

### Expected Output:
The program demonstrates:
1. Basic inheritance
2. Access specifiers
3. Constructor/destructor order
4. Function hiding (non-virtual `draw`)
5. Protected members
6. Multiple inheritance
7. Diamond problem
8. Virtual inheritance
9. Employee hierarchy (virtual `displayInfo` with `override`)
10. IS-A vs HAS-A

---

## 📊 Visual Concepts

### Inheritance Hierarchy
```
┌────────────────────────┐
│       Vehicle          │  Base Class
│  ─────────────────     │
│  # brand: string       │
│  # year: int           │
│  + start()             │
└────────────────────────┘
           ▲
           │ inherits
    ┌──────┴──────┬──────────┐
    │             │          │
┌───▼────┐   ┌───▼────┐  ┌──▼─────┐
│  Car   │   │  Bike  │  │ Truck  │  Derived
│ ────── │   │ ────── │  │ ────── │  Classes
│ -doors │   │ -type  │  │ -cargo │
│ +drive()│   │ +pedal()│  │+haul() │
└────────┘   └────────┘  └────────┘
```

### Access Control
```
class Base {
┌─────────────────────┐
│ private:            │ ◄── Only Base
│   int secret;       │
├─────────────────────┤
│ protected:          │ ◄── Base + Derived
│   int shared;       │
├─────────────────────┤
│ public:             │ ◄── Everyone
│   int data;         │
└─────────────────────┘
};

class Derived : public Base {
    Can access: shared ✅, data ✅
    Cannot:     secret ❌
};
```

### Diamond Problem
```
WITHOUT virtual inheritance:

       Device
       /    \
    Phone  Camera
       \    /
     SmartPhone
     
Contains TWO Device copies ❌


WITH virtual inheritance:

       Device (shared)
       /    \
    Phone  Camera
       \    /
     SmartPhone
     
Contains ONE Device copy ✅
```

---

## 📚 Related Topics

### Prerequisites:
- **04_MemberFunctions** - Member functions
- **05_Encapsulation** - Access control
- **07_CopyConstructor** - Constructors

### Coming Next:
- **14_VirtualFunctions** - Virtual dispatch
- **15_AbstractClasses** - Pure virtual functions
- **16_Polymorphism** - Dynamic binding

### Related Concepts:
- **Composition** - HAS-A relationships
- **Polymorphism** - Runtime behavior
- **Templates** - Generic programming

---

## 🎓 Summary

### What We Learned:

✅ **Inheritance** creates IS-A relationships
✅ **Derived class** inherits base class members
✅ **Public inheritance** is the common case
✅ **Protected** members accessible to derived
✅ **Construction** order: base → derived
✅ **Destruction** order: derived → base
✅ **Multiple inheritance** possible but complex
✅ **Diamond problem** solved with virtual
✅ **Overriding** (with `virtual`) provides specialization; without it a derived function only hides the base one
✅ **Prefer composition** over inheritance

### The Complete Pattern:
```cpp
// Base class
class Base {
protected:
    int sharedData;
    
public:
    Base(int data) : sharedData(data) { }
    virtual ~Base() { }  // Virtual destructor (for deletion through Base*)
    
    virtual void baseMethod() {
        cout << "Base method\n";
    }
};

// Derived class
class Derived : public Base {
private:
    int derivedData;
    
public:
    // Call base constructor
    Derived(int base, int derived) 
        : Base(base), derivedData(derived) { }
    
    // Can access protected members
    void derivedMethod() {
        sharedData = 10;  // ✅ Accessible
        baseMethod();     // ✅ Inherited
    }
    
    // Override base method
    void baseMethod() override {
        Base::baseMethod();  // Call base version
        cout << "Derived method\n";
    }
};

// Usage
Derived obj(5, 10);
obj.baseMethod();    // Calls Derived version (also through a Base&)
obj.derivedMethod(); // Derived's own method
```
✅ tested (compiles; `Base& r = obj; r.baseMethod();` prints `Base method` then `Derived method`). Earlier versions of this snippet and of the two patterns above did not compile or only hid the base function, because `baseMethod` / `publicMethod` were not virtual.

---

## ✨ Quick Reference Card
```cpp
// BASIC INHERITANCE
class Base {
protected:
    int data;
public:
    Base(int d) : data(d) { }
    virtual ~Base() { }
    virtual void method() { }
};

class Derived : public Base {
public:
    Derived(int d) : Base(d) { }
    void derivedMethod() {
        data = 10;  // Access protected
        method();   // Call inherited
    }
    void method() override;
};

// MULTIPLE INHERITANCE (Base1, Base2 are any two classes)
class Multi : public Base1, public Base2 {
public:
    Multi() : Base1(), Base2() { }
};

// VIRTUAL INHERITANCE (diamond solution)
class B : virtual public A { };
class C : virtual public A { };
class D : public B, public C { };

// CALLING BASE METHODS (method is virtual in Base)
void Derived::method() {
    Base::method();  // Call base version
    // Add derived behavior
}
```

---

## 🔍 Best Practices

### ✅ DO:

- **Use public inheritance** for IS-A relationships
- **Make a base destructor virtual** if objects may be deleted through a `Base*` (or protected and non-virtual if not - [C.35](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rc-dtor-virtual))
- **Call base constructors** explicitly
- **Use protected** for shared functionality (protected *data* is discouraged, C.133)
- **Keep hierarchies shallow** (rule of thumb, not a language limit)
- **Override with the same signature and write `override`**
- **Document relationships** clearly
- **Prefer composition** when in doubt
- **Test IS-A relationship** ("Dog IS-A Animal?")

### ❌ DON'T:

- **Overuse inheritance** (composition often better)
- **Use multiple inheritance** without good reason
- **Forget base constructors** (the base *default* constructor is called implicitly; if there is none, it does not compile)
- **Make everything public** to inherit it
- **Create deep hierarchies** (hard to maintain)
- **Use private/protected** inheritance (use composition)
- **Ignore diamond problem** in multiple inheritance
- **Inherit for code reuse** alone (use composition; Core Guidelines [C.120](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rh-domain): use hierarchies for concepts with inherent hierarchical structure)

---

## 🌟 Real-World Applications

### UI Component Hierarchy
```cpp
class Widget {
    // Base for all UI components
};
class Button : public Widget { };
class TextBox : public Widget { };
```

### Vehicle Management
```cpp
class Vehicle { };
class Car : public Vehicle { };
class Truck : public Vehicle { };
```

### Employee Management
```cpp
class Employee { };
class Manager : public Employee { };
class Developer : public Employee { };
```

### Game Entities
```cpp
class GameObject { };
class Player : public GameObject { };
class Enemy : public GameObject { };
```

---

## 📖 Further Reading

- [cppreference.com - Derived classes](https://en.cppreference.com/cpp/language/derived_class)
- "Effective C++" (3rd ed.) by Scott Meyers - Items 32-40 (public vs private inheritance, multiple inheritance)
- "C++ Primer" (5th ed.) by Lippman et al. - Chapter 15 (Object-Oriented Programming) and the section on virtual inheritance
- [C++ Core Guidelines - C.hier: Class hierarchies (OOP)](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#ss-hier)

---

## 🎯 Key Takeaways

1. Inheritance creates **IS-A relationships**
2. **Derived class inherits** base members
3. Use **public inheritance** for IS-A
4. **Protected** accessible to derived classes
5. **Construction**: base before derived
6. **Destruction**: derived before base
7. Make the base destructor **virtual** if you delete through a `Base*`
8. **Diamond problem** solved with virtual inheritance
9. **Prefer composition** over inheritance
10. **Keep hierarchies simple** and shallow

---

**Previous Topic:** [12_FriendFunctions](../12_FriendFunctions/) - Friend Functions

**Next Topic:** [14_VirtualFunctions](../14_VirtualFunctions/) - Virtual Functions

---

*Part of the C++ Classes and Objects series*
*Difficulty: Intermediate*
*Prerequisites: 05_Encapsulation, 07_CopyConstructor*
*Foundation for Polymorphism*

---

## 🧾 Notes moved from main.cpp

Per-example points from the removed `💡` blocks (not repeated where the sections above already say them):

- **Example 1:** benefits are code reuse (`eat`, `sleep`), hierarchical organization and specialized behavior (`bark`). Destruction runs in reverse order of construction (`~Dog`, then `~Animal`), visible after the `--- Destruction order ---` line.
- **Example 2:** protected members stay accessible inside all three derived classes; what changes is what *outside* code can do with the object (see the table and the `is_convertible_v` output).
- **Example 4:** `draw()` is not virtual, so `Circle::draw` and `Rectangle::draw` **hide** `Shape::draw`. Called through a `Shape*` the base version runs. The example was titled "Function Overriding" before; it is now "Function Hiding".
- **Example 5:** protected = accessible to derived classes, not to outside code, "between private and public".
- **Example 6:** `Duck` IS-A `Flyer` and IS-A `Swimmer`. Bases are constructed in the order they are *listed in the class head* (`Flyer`, then `Swimmer`), not in the order of the member-initializer list.
- **Example 7/8:** two `Device` subobjects (ambiguous) vs one shared `VDevice` constructed by the most derived class.
- **Example 9:** `displayInfo` is now `virtual` in `Employee` and `override` in `Manager`/`Developer`, so the loop over `Employee*` prints the full derived output. Before, the derived versions only hid the base one, although the base class had a virtual destructor "for polymorphism".
- **Example 10:** IS-A: Dog/Animal, Student/Person → inheritance. HAS-A: Car/Engine, Book/Author → composition. ✅ `Car*` converts to `Vehicle*`, `Motorcycle*` does not convert to `Engine*`.

Other lecture content that was only in `main.cpp`:

- **What is inherited?** Member functions, static members (✅ tested: `SD::count` and `sd.count` are the same static `B0::count`), and public/protected members according to the access table. Constructors, destructors and assignment operators are not inherited as such: each class has its own (the derived class gets implicitly generated ones that call the base's). ✅ tested: a defaulted `Derived` copy constructor calls `Base`'s copy constructor (`Base copy` is printed). Inheriting constructors explicitly is the topic of 25_InheritingConstructors. Private base members exist in the object but are not accessible.
- **Order with member objects:** base class constructor → member objects (in declaration order) → derived constructor body; destruction in exactly the reverse order. ✅ tested with `A` (member `Tag`) and `B : A` (member `Tag`): `Tag A.member`, `A()`, `Tag B.member`, `B()`, then `~B()`, `~Tag`, `~A()`, `~Tag`.
- **Common mistakes (corrected):**
  1. *Not calling the base constructor.* Only a problem if the base has no default constructor - then it is a compile error (tested above); otherwise the default constructor runs silently. The old note "Base() called implicitly" shown as an error was misleading.
  2. *Accessing private base members:* error; give the base a protected/public interface.
  3. *Non-virtual base destructor:* only a bug if an object is deleted through a `Base*` (undefined behavior, see 17_VirtualDestructor and 28_ObjectSlicing); for a class never deleted that way it is fine.
- **Best-practice summary** (public for IS-A, call base constructors, shallow hierarchies, composition when in doubt) is already in the *Best Practices* section above.

**Code changes in `main.cpp`:**
  - Example 4: titled "Function Hiding", comments corrected (non-virtual `draw` hides) and a `Shape*` demo added.
  - Example 9: `displayInfo` virtual + `override`, demo through `Employee*`.
  - Examples 2, 7, 8, 10: real `is_convertible_v` / `sizeof` demos instead of lecture text (sizes are for GCC on x86-64).
  - One-argument constructors made `explicit` (see 23_ExplicitKeyword); `using namespace std;` and the unused `<vector>` removed.
  - ✅ tested with `-Wall -Wextra`, AddressSanitizer and UndefinedBehaviorSanitizer; output identical to the original apart from the changes listed.

---

## ⚠️ Open points

- Numbering: this README's "Prerequisites" mention `07_CopyConstructor` and the footer says "Prerequisites: 05_Encapsulation, 07_CopyConstructor"; check that name against your actual folder. "Coming Next" was changed to 14_VirtualFunctions, 15_AbstractClasses, 16_Polymorphism to match the other folders (it listed 14_Polymorphism, 15_VirtualFunctions, 16_AbstractClasses).
- Not tested / not opened: the cppreference and Core Guidelines links (the cppreference URL comes from a search result; the Core Guidelines anchors are taken from the guidelines text), "Effective C++ Items 32-40" and the "C++ Primer" chapter number (from memory). The "2-3 levels" and "almost all inheritance is public" statements are rules of thumb.
