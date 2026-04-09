# 13_Inheritance - Code Reuse Through Hierarchies

## 📚 Overview

This folder introduces **Inheritance** - one of the fundamental pillars of Object-Oriented Programming. Inheritance allows you to create new classes from existing classes, promoting code reuse, establishing hierarchical relationships, and enabling polymorphism.

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
- **Polymorphism** - Foundation for dynamic behavior

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
│  + name: string │
│  + age: int     │
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
│  + breed │  │  + color │
│  + bark()│  │  + meow()│
└──────────┘  └──────────┘
```

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

### Example 4: Function Overriding
Derived class providing new implementation.

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

**Use 99% of the time - represents IS-A relationship.**

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

**Rarely used - breaks IS-A relationship.**

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

**Better to use composition (HAS-A) instead.**

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
    // This creates temporary object, doesn't initialize base!
};
```

**Key Rule:** Base constructor is called FIRST, before derived constructor body executes.

---

## 💡 Function Overriding

### Basic Overriding
```cpp
class Shape {
public:
    void draw() {
        cout << "Drawing generic shape\n";
    }
};

class Circle : public Shape {
public:
    // Override base class function
    void draw() {
        cout << "Drawing circle\n";
    }
};

Shape s;
Circle c;

s.draw();  // "Drawing generic shape"
c.draw();  // "Drawing circle"
```

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

**Note:** Without `virtual` keyword, this is NOT polymorphism (yet)! See polymorphism chapter for that.

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
2. Most derived class calls base constructor
3. Only one copy of base class exists
4. Resolves diamond problem

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
    
    // Virtual destructor (important!)
    virtual ~Base() { }
    
    // Public interface
    void publicMethod() { }
    
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
    
    // Override methods
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
g++ -std=c++17 inheritance.cpp -o inheritance
```

### Run:
```bash
./inheritance
```

### Expected Output:
The program demonstrates:
1. Basic inheritance
2. Access specifiers
3. Constructor/destructor order
4. Function overriding
5. Protected members
6. Multiple inheritance
7. Diamond problem
8. Virtual inheritance
9. Employee hierarchy
10. IS-A vs HAS-A

---

## 📊 Visual Concepts

### Inheritance Hierarchy
```
┌────────────────────────┐
│       Vehicle          │  Base Class
│  ─────────────────     │
│  + brand: string       │
│  + year: int           │
│  + start()             │
└────────────────────────┘
           ▲
           │ inherits
    ┌──────┴──────┬──────────┐
    │             │          │
┌───▼────┐   ┌───▼────┐  ┌──▼─────┐
│  Car   │   │  Bike  │  │ Truck  │  Derived
│ ────── │   │ ────── │  │ ────── │  Classes
│ +doors │   │ +type  │  │ +cargo │
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
- **14_Polymorphism** - Dynamic binding
- **15_VirtualFunctions** - Virtual dispatch
- **16_AbstractClasses** - Pure virtual functions

### Related Concepts:
- **Composition** - HAS-A relationships
- **Polymorphism** - Runtime behavior
- **Templates** - Generic programming

---

## 🎓 Summary

### What We Learned:

✅ **Inheritance** creates IS-A relationships
✅ **Derived class** inherits base class members
✅ **Public inheritance** most common (99%)
✅ **Protected** members accessible to derived
✅ **Construction** order: base → derived
✅ **Destruction** order: derived → base
✅ **Multiple inheritance** possible but complex
✅ **Diamond problem** solved with virtual
✅ **Function overriding** provides specialization
✅ **Prefer composition** over inheritance

### The Complete Pattern:
```cpp
// Base class
class Base {
protected:
    int sharedData;
    
public:
    Base(int data) : sharedData(data) { }
    virtual ~Base() { }  // Virtual destructor!
    
    void baseMethod() {
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
    void baseMethod() {
        Base::baseMethod();  // Call base version
        cout << "Derived method\n";
    }
};

// Usage
Derived obj(5, 10);
obj.baseMethod();    // Calls Derived version
obj.derivedMethod(); // Derived's own method
```

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
    void method() { }
};

class Derived : public Base {
public:
    Derived(int d) : Base(d) { }
    void derivedMethod() {
        data = 10;  // Access protected
        method();   // Call inherited
    }
};

// MULTIPLE INHERITANCE
class Multi : public Base1, public Base2 {
public:
    Multi() : Base1(), Base2() { }
};

// VIRTUAL INHERITANCE (diamond solution)
class B : virtual public A { };
class C : virtual public A { };
class D : public B, public C { };

// CALLING BASE METHODS
void Derived::method() {
    Base::method();  // Call base version
    // Add derived behavior
}
```

---

## 🔍 Best Practices

### ✅ DO:

- **Use public inheritance** for IS-A relationships
- **Make base destructors virtual** (always!)
- **Call base constructors** explicitly
- **Use protected** for shared data
- **Keep hierarchies shallow** (2-3 levels max)
- **Override with same signature**
- **Document relationships** clearly
- **Prefer composition** when in doubt
- **Test IS-A relationship** ("Dog IS-A Animal?")

### ❌ DON'T:

- **Overuse inheritance** (composition often better)
- **Use multiple inheritance** without good reason
- **Forget base constructors** (will get default)
- **Make everything public** to inherit it
- **Create deep hierarchies** (hard to maintain)
- **Use private/protected** inheritance (use composition)
- **Ignore diamond problem** in multiple inheritance
- **Inherit for code reuse** alone (use composition)

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

- [cppreference.com - Derived Classes](https://en.cppreference.com/w/cpp/language/derived_class)
- "Effective C++" by Scott Meyers - Items 32-40
- "C++ Primer" by Lippman - Chapter on Inheritance
- [C++ Core Guidelines - Inheritance](https://isocpp.github.io/CppCoreGuidelines/)

---

## 🎯 Key Takeaways

1. Inheritance creates **IS-A relationships**
2. **Derived class inherits** base members
3. Use **public inheritance** (99% of time)
4. **Protected** accessible to derived classes
5. **Construction**: base before derived
6. **Destruction**: derived before base
7. **Always** make base destructor virtual
8. **Diamond problem** solved with virtual inheritance
9. **Prefer composition** over inheritance
10. **Keep hierarchies simple** and shallow

---

**Previous Topic:** [12_FriendFunctions](../12_FriendFunctions/) - Friend Functions

**Next Topic:** [14_Polymorphism](../14_Polymorphism/) - Dynamic Binding (coming next)

---

*Part of the C++ Classes and Objects series*
*Difficulty: Intermediate*
*Prerequisites: 05_Encapsulation, 07_CopyConstructor*
*Foundation for Polymorphism*