# 17_VirtualDestructor - Preventing Memory Leaks in Polymorphism

## 📚 Overview

This folder introduces **Virtual Destructors** - one of the most critical concepts in C++ for preventing memory leaks and ensuring proper cleanup in inheritance hierarchies. A virtual destructor ensures that when you delete a derived class object through a base class pointer, both the derived and base destructors are called.

**Virtual Destructor** = Destructor declared with `virtual` keyword to ensure proper cleanup

This is **ESSENTIAL** knowledge for writing correct, leak-free C++ code with polymorphism.

---

## 🎯 What You'll Learn

- What virtual destructors are and why they're critical
- The problem: memory leaks without virtual destructors
- The solution: how virtual destructors work
- Destruction order in inheritance hierarchies
- Default virtual destructors (C++11)
- Pure virtual destructors
- Protected destructors pattern
- Virtual destructors with smart pointers
- When to use (and when not to use)
- Real-world memory leak scenarios
- Best practices and common pitfalls

---

## 📖 Concept Explanation

### What are Virtual Destructors?

**Virtual destructors** are destructors declared with the `virtual` keyword. They ensure that when deleting a derived class object through a base class pointer, both the derived class destructor and the base class destructor are called in the correct order.

**The Golden Rule:**
> If a class has ANY virtual function, its destructor MUST be virtual!

### The Problem Without Virtual Destructor
```cpp
class Base {
public:
    ~Base() { }  // ❌ NOT virtual
    virtual void func() { }
};

class Derived : public Base {
    int* data;
public:
    Derived() { data = new int[100]; }
    ~Derived() { delete[] data; }  // Never called!
};

Base* ptr = new Derived();
delete ptr;  // ⚠️ MEMORY LEAK!
             // Only ~Base() called
             // ~Derived() NOT called
             // Memory NOT freed!
```

**What Happens:**
1. ❌ Only `Base` destructor is called
2. ❌ `Derived` destructor is NOT called
3. ❌ Memory allocated in `Derived` is NOT freed
4. ❌ **MEMORY LEAK!** 💥
5. ❌ Undefined behavior

### The Solution With Virtual Destructor
```cpp
class Base {
public:
    virtual ~Base() { }  // ✅ Virtual!
    virtual void func() { }
};

class Derived : public Base {
    int* data;
public:
    Derived() { data = new int[100]; }
    ~Derived() { delete[] data; }
};

Base* ptr = new Derived();
delete ptr;  // ✅ Correct cleanup!
             // 1. ~Derived() called first
             // 2. ~Base() called second
             // Memory properly freed!
```

**What Happens:**
1. ✅ `Derived` destructor is called first
2. ✅ `Base` destructor is called second
3. ✅ Memory allocated in `Derived` is freed
4. ✅ **NO MEMORY LEAK!** ✅
5. ✅ Defined behavior

### Real-World Analogy
```
Think of cleaning up after a party:

WITHOUT virtual destructor:
  - You hire a cleaning service (Base)
  - Party adds extra decorations (Derived)
  - You tell cleaning service "clean the base setup"
  - They clean basic setup only
  - Decorations left behind! (memory leak)

WITH virtual destructor:
  - You hire a cleaning service (Base)
  - Party adds extra decorations (Derived)
  - You tell cleaning service "clean everything"
  - They clean decorations first (Derived)
  - Then clean basic setup (Base)
  - Everything cleaned! (no leak)
```

### Visual Representation
```
WITHOUT virtual destructor:

Base* ptr = new Derived();
delete ptr;

   ┌─────────────┐
   │   Derived   │
   │  ─────────  │
   │  data[100]  │ ← NOT freed! 💥
   └─────────────┘
   ┌─────────────┐
   │    Base     │
   │  ─────────  │ ← Only this cleaned ❌
   └─────────────┘

WITH virtual destructor:

Base* ptr = new Derived();
delete ptr;

   ┌─────────────┐
   │   Derived   │
   │  ─────────  │
   │  data[100]  │ ← Freed! ✅
   └─────────────┘
        ↓
   ┌─────────────┐
   │    Base     │
   │  ─────────  │ ← Then this ✅
   └─────────────┘
```

---

## 💻 Basic Syntax

### Simple Virtual Destructor
```cpp
class Base {
public:
    // Virtual destructor
    virtual ~Base() {
        cout << "Base destructor\n";
    }
    
    virtual void func() { }
};

class Derived : public Base {
    int* data;
public:
    Derived() {
        data = new int[100];
    }
    
    // Implicitly virtual (base is virtual)
    ~Derived() {
        delete[] data;
        cout << "Derived destructor\n";
    }
};

// Usage
Base* ptr = new Derived();
delete ptr;  // Calls both destructors ✅
```

### Default Virtual Destructor (C++11)
```cpp
class Base {
public:
    // Compiler-generated virtual destructor
    virtual ~Base() = default;
    
    virtual void func() { }
};
```

### Pure Virtual Destructor (Rare)
```cpp
class Abstract {
public:
    // Pure virtual destructor
    virtual ~Abstract() = 0;
};

// MUST provide implementation!
Abstract::~Abstract() {
    cout << "Abstract destructor\n";
}
```

---

## 🔍 The Critical Problem

### Memory Leak Scenario

**Setup:**
```cpp
class Resource {
    int* data;
public:
    Resource() {
        data = new int[1000];
        cout << "Allocated 1000 ints\n";
    }
    
    ~Resource() {
        delete[] data;
        cout << "Freed 1000 ints\n";
    }
};

class Base {
    Resource* resource;
public:
    Base() {
        resource = new Resource();
    }
    
    ~Base() {  // ❌ NOT virtual
        delete resource;
        cout << "Base destructor\n";
    }
    
    virtual void use() { }
};

class Derived : public Base {
    Resource* derivedResource;
public:
    Derived() {
        derivedResource = new Resource();
    }
    
    ~Derived() {
        delete derivedResource;
        cout << "Derived destructor\n";
    }
};
```

**The Leak:**
```cpp
Base* ptr = new Derived();
delete ptr;  // 💥 MEMORY LEAK!

// Output:
// Allocated 1000 ints (base)
// Allocated 1000 ints (derived)
// Freed 1000 ints (base)
// Base destructor
// ❌ Derived destructor NOT called!
// ❌ Derived resource NOT freed!
// ❌ 1000 ints leaked!
```

**The Fix:**
```cpp
class Base {
public:
    virtual ~Base() {  // ✅ Virtual!
        delete resource;
        cout << "Base destructor\n";
    }
    
    // ... rest of class
};

Base* ptr = new Derived();
delete ptr;  // ✅ No leak!

// Output:
// Allocated 1000 ints (base)
// Allocated 1000 ints (derived)
// Freed 1000 ints (derived) ✅
// Derived destructor
// Freed 1000 ints (base) ✅
// Base destructor
```

---

## 📋 Examples in This Folder

### Example 1: Without Virtual Destructor (The Problem)
Demonstrates memory leak and undefined behavior.

### Example 2: With Virtual Destructor (The Solution)
Shows proper cleanup with virtual destructor.

### Example 3: Comparison Side-by-Side
Direct comparison of both approaches.

### Example 4: Destruction Order
Understanding the calling sequence.

### Example 5: Default Virtual Destructor
Using `= default` syntax (C++11).

### Example 6: Pure Virtual Destructor
Abstract class with pure virtual destructor.

### Example 7: Protected Destructor
Preventing polymorphic deletion pattern.

### Example 8: Memory Leak Demonstration
Real memory leak tracking and fixing.

### Example 9: Smart Pointers
Virtual destructors with modern C++.

### Example 10: Polymorphic Collection
Real-world usage with shape hierarchy.

---

## 🎓 Key Concepts

### Destruction Order

**With Virtual Destructor:**
```cpp
class GrandParent {
    virtual ~GrandParent() { cout << "3. GrandParent\n"; }
};

class Parent : public GrandParent {
    ~Parent() { cout << "2. Parent\n"; }
};

class Child : public Parent {
    ~Child() { cout << "1. Child\n"; }
};

GrandParent* ptr = new Child();
delete ptr;

// Output:
// 1. Child destructor       ← Most derived first
// 2. Parent destructor
// 3. GrandParent destructor ← Base last
```

**Key Points:**
- ✅ Destruction is **REVERSE** of construction
- ✅ Most derived class destroyed first
- ✅ Base class destroyed last
- ✅ Ensures proper cleanup order

---

### When Virtual Destructor is REQUIRED

#### ✅ ALWAYS Required When:

1. **Class has ANY virtual function:**
```cpp
class Base {
    virtual void func() { }  // Has virtual function
    virtual ~Base() { }      // MUST be virtual!
};
```

2. **Class is meant to be a base class:**
```cpp
class Base {
    virtual ~Base() = default;  // Will be inherited
};
```

3. **Objects deleted via base pointer:**
```cpp
Base* ptr = new Derived();
delete ptr;  // Needs virtual destructor!
```

4. **Polymorphic usage:**
```cpp
vector<Base*> collection;
// When clearing, needs virtual destructor
```

#### ❌ NOT Required When:

1. **Final class (no derivation):**
```cpp
class FinalClass final {
    ~FinalClass() { }  // Non-virtual OK
};
```

2. **No virtual functions:**
```cpp
class Simple {
    int data;
    ~Simple() { }  // Non-virtual OK
};
```

3. **Never deleted polymorphically:**
```cpp
Derived obj;  // Stack object
// No virtual destructor needed
```

4. **Protected destructor (prevents polymorphic deletion):**
```cpp
class Base {
protected:
    ~Base() { }  // Can't delete via Base*
};
```

---

### Special Cases

#### 1. Default Virtual Destructor
```cpp
class Base {
public:
    virtual ~Base() = default;  // Compiler-generated
    virtual void func() { }
};
```

**Benefits:**
- ✅ Clean syntax
- ✅ Compiler-optimized
- ✅ Still virtual
- ✅ Best practice when no cleanup needed

#### 2. Pure Virtual Destructor
```cpp
class Abstract {
public:
    virtual ~Abstract() = 0;  // Pure virtual
};

// MUST provide implementation!
Abstract::~Abstract() {
    cout << "Abstract destructor\n";
}

class Concrete : public Abstract {
    ~Concrete() {
        cout << "Concrete destructor\n";
    }
};
```

**Characteristics:**
- Makes class abstract
- MUST provide implementation (unlike other pure virtuals)
- Still called during destruction
- Rare pattern (use pure virtual function instead)

#### 3. Protected Non-Virtual Destructor
```cpp
class Base {
protected:
    ~Base() { }  // Protected, non-virtual
    
public:
    void doSomething() { }
};

class Derived : public Base {
public:
    ~Derived() { }
};

// Base* ptr = new Derived();
// delete ptr;  // ❌ Error! Destructor protected

Derived* ptr = new Derived();
delete ptr;  // ✅ OK
```

**Use Case:**
- Prevent polymorphic deletion
- Force proper usage
- No virtual overhead needed
- Can't accidentally delete via base pointer

---

### Virtual Destructors with Smart Pointers

**Smart pointers STILL need virtual destructor:**
```cpp
class Base {
public:
    virtual ~Base() = default;  // Still needed!
    virtual void func() { }
};

class Derived : public Base {
    int* data;
public:
    Derived() { data = new int[100]; }
    ~Derived() { delete[] data; }
};

// unique_ptr
unique_ptr<Base> ptr1 = make_unique<Derived>();
// Automatic cleanup when ptr1 goes out of scope
// Virtual destructor ensures proper cleanup

// shared_ptr
shared_ptr<Base> ptr2 = make_shared<Derived>();
// Automatic cleanup when reference count reaches 0
// Virtual destructor ensures proper cleanup
```

**Benefits:**
- ✅ Automatic memory management
- ✅ Exception safe
- ✅ No explicit delete needed
- ✅ Still need virtual destructor for proper cleanup
- ✅ Modern C++ best practice

---

## 💡 Common Mistakes

### ❌ Mistake 1: Forgetting Virtual
```cpp
class Base {
    ~Base() { }  // ❌ Not virtual!
    virtual void func() { }
};

// Fix:
class Base {
    virtual ~Base() { }  // ✅ Virtual
    virtual void func() { }
};
```

### ❌ Mistake 2: Calling Virtual Functions in Destructor
```cpp
class Base {
    virtual ~Base() {
        virtualFunc();  // ❌ Dangerous!
    }
    
    virtual void virtualFunc() { }
};

class Derived : public Base {
    void virtualFunc() override {
        // Uses derived data
    }
};

// Problem: When ~Base() runs, derived part already destroyed!
```

**Why it's bad:**
- Derived part already destroyed when base destructor runs
- Virtual function calls base version (not derived)
- May access destroyed data
- Undefined behavior

### ❌ Mistake 3: Not Providing Pure Virtual Implementation
```cpp
class Abstract {
    virtual ~Abstract() = 0;
};

// ❌ Missing implementation!

// Fix:
Abstract::~Abstract() { }  // ✅ MUST provide
```

### ❌ Mistake 4: Assuming Smart Pointers Don't Need Virtual
```cpp
class Base {
    ~Base() { }  // ❌ Not virtual
    virtual void func() { }
};

unique_ptr<Base> ptr = make_unique<Derived>();
// Still leaks! Smart pointer doesn't fix this!

// Fix:
class Base {
    virtual ~Base() = default;  // ✅ Virtual
};
```

---

## 🎯 Practice Exercises

### Exercise 1: Find the Leak
Given a class hierarchy without virtual destructor, identify the memory leak and fix it.

### Exercise 2: Proper Cleanup
Create a resource management class hierarchy with proper virtual destructors. Verify no leaks with tracking.

### Exercise 3: Smart Pointer Usage
Refactor raw pointer code to use smart pointers with proper virtual destructors.

### Exercise 4: Destruction Order
Create a multi-level hierarchy and trace destruction order with and without virtual destructors.

---

## 🚀 How to Run

### Compile:
```bash
g++ -std=c++17 virtual_destructor.cpp -o virtual_destructor
```

### Run:
```bash
./virtual_destructor
```

### Expected Output:
The program demonstrates:
1. Memory leak without virtual destructor
2. Proper cleanup with virtual destructor
3. Side-by-side comparison
4. Destruction order
5. Default virtual destructor
6. Pure virtual destructor
7. Protected destructor pattern
8. Memory leak tracking
9. Smart pointers usage
10. Polymorphic collection

---

## 📊 Visual Concepts

### Without Virtual Destructor
```
delete ptr;  (Base* pointing to Derived)

WRONG: Static binding
    ┌─────────────┐
    │   Derived   │
    │  ─────────  │
    │  *data      │ ← NOT freed! 💥
    └─────────────┘
         │
         │ Only this called ❌
         ▼
    ┌─────────────┐
    │    Base     │
    │  ─────────  │
    │  ~Base()    │ ← Called
    └─────────────┘
```

### With Virtual Destructor
```
delete ptr;  (Base* pointing to Derived)

CORRECT: Dynamic binding
    ┌─────────────┐
    │   Derived   │
    │  ─────────  │
    │  *data      │ ← Freed! ✅
    │  ~Derived() │ ← Called first (1)
    └─────────────┘
         │
         ▼
    ┌─────────────┐
    │    Base     │
    │  ─────────  │
    │  ~Base()    │ ← Called second (2)
    └─────────────┘
```

### Destruction Chain
```
Construction:            Destruction:
                        (reverse order)

GrandParent ──┐         ┌── ~Child
              │         │
Parent ───────┤         ├── ~Parent
              │         │
Child ────────┘         └── ~GrandParent
```

---

## 📚 Related Topics

### Prerequisites:
- **14_VirtualFunctions** - Virtual functions
- **13_Inheritance** - Inheritance basics
- **07_CopyConstructor** - Constructors/destructors

### Coming Next:
- **18_RTTI** - Runtime type information
- **19_Templates** - Generic programming
- **20_SmartPointers** - Memory management

### Related Concepts:
- **Memory Management** - Resource cleanup
- **RAII** - Resource acquisition
- **Smart Pointers** - Automatic memory

---

## 🎓 Summary

### What We Learned:

✅ **Virtual destructors** ensure proper cleanup
✅ **Critical** for polymorphic base classes
✅ **Prevents memory leaks** and undefined behavior
✅ **Destruction order** = derived → base (reverse)
✅ **Use `= default`** when no cleanup needed
✅ **Smart pointers** still need virtual destructor
✅ **If ANY virtual** → destructor MUST be virtual
✅ **Protected non-virtual** prevents polymorphic deletion
✅ **Pure virtual** destructor needs implementation
✅ **Always test** polymorphic deletion!

### The Complete Pattern:
```cpp
// Polymorphic base class
class Base {
public:
    // Virtual destructor (CRITICAL!)
    virtual ~Base() = default;
    
    // Virtual function
    virtual void func() { }
};

// Derived class
class Derived : public Base {
    int* data;
    
public:
    Derived() {
        data = new int[100];
    }
    
    // Implicitly virtual (base is virtual)
    ~Derived() {
        delete[] data;
    }
    
    void func() override { }
};

// Modern C++ usage
unique_ptr<Base> ptr = make_unique<Derived>();
// Automatic cleanup with proper destruction order
```

---

## ✨ Quick Reference Card
```cpp
// BASIC VIRTUAL DESTRUCTOR
class Base {
public:
    virtual ~Base() { }  // Manual implementation
    virtual void func() { }
};

// DEFAULT VIRTUAL DESTRUCTOR (Best practice)
class Base {
public:
    virtual ~Base() = default;  // Compiler-generated
    virtual void func() { }
};

// PURE VIRTUAL DESTRUCTOR
class Abstract {
public:
    virtual ~Abstract() = 0;
};
Abstract::~Abstract() { }  // MUST implement

// PROTECTED NON-VIRTUAL
class Base {
protected:
    ~Base() { }  // Prevents polymorphic deletion
public:
    virtual void func() { }
};

// WITH SMART POINTERS
unique_ptr<Base> ptr = make_unique<Derived>();
// Still needs virtual destructor!
```

---

## 🔍 Best Practices

### ✅ DO:

- **Make destructor virtual** in polymorphic classes (ALWAYS!)
- **Use `= default`** when no cleanup needed
- **Use smart pointers** (unique_ptr, shared_ptr)
- **Test polymorphic deletion** thoroughly
- **Check for memory leaks** with tools (valgrind, sanitizers)
- **Document inheritance** relationships
- **Follow RAII** principles
- **Make virtual early** (easier than adding later)

### ❌ DON'T:

- **Forget virtual** when class has virtual functions
- **Call virtual functions** in destructors
- **Assume non-virtual** is OK
- **Ignore memory warnings**
- **Mix ownership** models (raw + smart pointers)
- **Delete polymorphic** objects without virtual
- **Skip testing** destruction
- **Assume smart pointers** fix non-virtual destructor

---

## 🌟 Real-World Applications

### GUI Frameworks
```cpp
class Widget {
    virtual ~Widget() = default;
    virtual void render() = 0;
};
```

### Game Engines
```cpp
class GameObject {
    virtual ~GameObject() = default;
    virtual void update() = 0;
};
```

### Plugin Systems
```cpp
class Plugin {
    virtual ~Plugin() = default;
    virtual void execute() = 0;
};
```

### Resource Management
```cpp
class Resource {
    virtual ~Resource() = default;
    virtual void release() = 0;
};
```

---

## 📖 Further Reading

- [cppreference.com - Destructors](https://en.cppreference.com/w/cpp/language/destructor)
- "Effective C++" by Scott Meyers - Item 7
- "More Effective C++" by Scott Meyers - Item 14
- [C++ Core Guidelines - Destructors](https://isocpp.github.io/CppCoreGuidelines/)

---

## 🎯 Key Takeaways

1. Virtual destructor **ensures proper cleanup**
2. **CRITICAL** for polymorphic base classes
3. Prevents **memory leaks** and undefined behavior
4. Destruction order: **derived → base** (reverse)
5. Use **`= default`** when no cleanup needed
6. Smart pointers **still need** virtual destructor
7. **If ANY virtual function** → destructor MUST be virtual
8. Protected non-virtual **prevents polymorphic deletion**
9. Pure virtual destructor **needs implementation**
10. **ALWAYS test** polymorphic deletion thoroughly!

---

## 🚨 THE GOLDEN RULE
```
⚠️  If a class has ANY virtual function,
   its destructor MUST be virtual!

   class Base {
       virtual ~Base() = default;
       virtual void func() { }
   };

   This prevents memory leaks and ensures
   proper cleanup in inheritance hierarchies.

   ALWAYS. NO EXCEPTIONS.
```

---

**Previous Topic:** [16_MultipleInheritance](../16_MultipleInheritance/) - Multiple Inheritance

**Next Topic:** [18_RTTI](../18_RTTI/) - Runtime Type Information (or other advanced topics)

---

*Part of the C++ Classes and Objects series*
*Difficulty: Intermediate*
*Prerequisites: 14_VirtualFunctions, 13_Inheritance*
*CRITICAL for Preventing Memory Leaks*