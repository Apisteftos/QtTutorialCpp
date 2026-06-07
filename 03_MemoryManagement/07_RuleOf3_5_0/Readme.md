# 30_RuleOf3_5_0 - The Ultimate Resource Management Guide

## 📚 Overview

This folder contains the **most important design rules** in C++: the **Rule of Three**, **Rule of Five**, and **Rule of Zero**. These rules dictate when and how to define special member functions for classes that manage resources.

Understanding these rules is **absolutely critical** for professional C++ development and preventing memory leaks, crashes, and undefined behavior.

---

## 🎯 What You'll Learn

- The problem: shallow copy and double deletion
- Rule of Three (C++98) - define all or none
- Rule of Five (C++11) - adding move semantics
- Rule of Zero (Modern C++) - the best approach
- When to use each rule
- Move semantics and std::move
- Self-assignment protection
- noexcept importance
- RAII (Resource Acquisition Is Initialization)
- Common mistakes and best practices
- Performance: Copy vs Move

---

## 📖 The Three Rules Explained

### The Problem

```cpp
class BadClass {
    int* data;
public:
    BadClass(int value) {
        data = new int(value);
    }
    ~BadClass() {
        delete data;
    }
    // Using compiler-generated copy! ❌
};

BadClass obj1(42);
BadClass obj2 = obj1;  // Shallow copy! Both share same pointer!
// When obj1 and obj2 are destroyed: DOUBLE DELETE! 💥 CRASH!
```

**The Issue:** Compiler-generated copy constructor and copy assignment operator do **shallow copies** - they copy the pointer value, not the data it points to. This causes:
- Double deletion (crash)
- Memory leaks
- Undefined behavior

---

## 🔥 Rule of Three (C++98)

### The Rule

**If your class needs ONE of these, it needs ALL THREE:**

1. **Destructor** - `~MyClass()`
2. **Copy Constructor** - `MyClass(const MyClass&)`
3. **Copy Assignment Operator** - `MyClass& operator=(const MyClass&)`

### Why?

If you need a custom destructor (to free resources), the default copy operations will do shallow copies, causing the problems shown above.

### Implementation

```cpp
class GoodClass {
    int* data;
public:
    // Constructor
    GoodClass(int value) {
        data = new int(value);
    }
    
    // 1. Destructor
    ~GoodClass() {
        delete data;
    }
    
    // 2. Copy Constructor (DEEP COPY)
    GoodClass(const GoodClass& other) {
        data = new int(*other.data);  // Allocate new memory!
    }
    
    // 3. Copy Assignment Operator (DEEP COPY)
    GoodClass& operator=(const GoodClass& other) {
        if (this != &other) {          // Self-assignment check!
            delete data;               // Clean up old
            data = new int(*other.data); // Deep copy new
        }
        return *this;
    }
};
```

### Key Points

✅ **Deep copy** - Allocate new memory and copy the data
✅ **Self-assignment check** - `if (this != &other)`
✅ **All three or none** - Don't define just one or two!

---

## 🚀 Rule of Five (C++11)

### The Rule

**If your class needs ONE of these, it needs ALL FIVE:**

1. **Destructor** - `~MyClass()`
2. **Copy Constructor** - `MyClass(const MyClass&)`
3. **Copy Assignment Operator** - `MyClass& operator=(const MyClass&)`
4. **Move Constructor** - `MyClass(MyClass&&) noexcept` ⭐ NEW!
5. **Move Assignment Operator** - `MyClass& operator=(MyClass&&) noexcept` ⭐ NEW!

### Why Move Semantics?

**Problem:** Copying large objects is EXPENSIVE!

```cpp
vector<int> v(1000000);      // 1 million elements
vector<int> v2 = v;          // COPIES 1 million elements! 😱
                            // ~4MB of data transfer
```

**Solution:** MOVE instead of copy!

```cpp
vector<int> v(1000000);      // 1 million elements
vector<int> v2 = std::move(v); // Just swaps pointers! 🚀
                              // ~16 bytes (2 pointers)
                              // 250,000x faster!
```

### Implementation

```cpp
class ModernClass {
    int* data;
    int size;
public:
    // Constructor
    ModernClass(int s) : size(s) {
        data = new int[size];
    }
    
    // 1. Destructor
    ~ModernClass() {
        delete[] data;
    }
    
    // 2. Copy Constructor (DEEP COPY)
    ModernClass(const ModernClass& other) : size(other.size) {
        data = new int[size];
        std::copy(other.data, other.data + size, data);
    }
    
    // 3. Copy Assignment (DEEP COPY)
    ModernClass& operator=(const ModernClass& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = new int[size];
            std::copy(other.data, other.data + size, data);
        }
        return *this;
    }
    
    // 4. Move Constructor (STEAL RESOURCES!)
    ModernClass(ModernClass&& other) noexcept
        : data(other.data), size(other.size) {
        other.data = nullptr;  // Leave source in valid state
        other.size = 0;
    }
    
    // 5. Move Assignment (STEAL RESOURCES!)
    ModernClass& operator=(ModernClass&& other) noexcept {
        if (this != &other) {
            delete[] data;          // Clean up our resource
            data = other.data;      // Steal other's resource
            size = other.size;
            other.data = nullptr;   // Leave source valid
            other.size = 0;
        }
        return *this;
    }
};
```

### Key Points

✅ **Move = Transfer ownership** (steal the pointer)
✅ **Mark noexcept** - STL won't use moves without it!
✅ **Leave source valid** - Set pointer to nullptr
✅ **Much faster** - O(1) vs O(n)

---

## 🎯 Rule of Zero (Modern C++ - BEST!)

### The Rule

**DON'T define ANY special member functions!**

Use RAII (Resource Acquisition Is Initialization) types instead:
- `vector`, `string`, `array` (containers)
- `unique_ptr`, `shared_ptr` (smart pointers)
- `fstream`, `thread`, `mutex` (system resources)

### Why?

✅ **LESS CODE** to write and maintain
✅ **CORRECT by default** - compiler generates optimal versions
✅ **HARDER to make mistakes** - no manual memory management
✅ **EXCEPTION SAFE** - automatic cleanup
✅ **FASTER** - compiler can optimize better

### Implementation

```cpp
// BAD: Manual memory management (Rule of Five - 50+ lines)
class BadClass {
    int* data;           // Raw pointer
    char* buffer;        // Another raw pointer
    int size;
public:
    // Need to define ALL FIVE special members! 😫
    ~BadClass() { delete[] data; delete[] buffer; }
    BadClass(const BadClass&) { /* deep copy */ }
    BadClass& operator=(const BadClass&) { /* deep copy */ }
    BadClass(BadClass&&) noexcept { /* move */ }
    BadClass& operator=(BadClass&&) noexcept { /* move */ }
};

// GOOD: Use RAII types (Rule of Zero - 5 lines!)
class GoodClass {
    vector<int> data;        // RAII!
    vector<char> buffer;     // RAII!
    string name;             // RAII!
    unique_ptr<int> ptr;     // RAII!
public:
    GoodClass(int size, string n) 
        : data(size), buffer(size), name(n), ptr(make_unique<int>(42)) {}
    
    // NO SPECIAL MEMBERS NEEDED! 🎉
    // Compiler generates ALL FIVE correctly:
    // ~GoodClass()                          = default;
    // GoodClass(const GoodClass&)           = default;
    // GoodClass& operator=(const GoodClass&) = default;
    // GoodClass(GoodClass&&)                = default;
    // GoodClass& operator=(GoodClass&&)     = default;
};
```

### RAII Types to Use

| Resource Type | Use This |
|--------------|----------|
| Dynamic single object | `unique_ptr<T>` |
| Dynamic array | `vector<T>` |
| Shared ownership | `shared_ptr<T>` |
| String data | `string` |
| File handle | `fstream`, `ifstream`, `ofstream` |
| Thread | `std::thread` |
| Lock | `lock_guard`, `unique_lock` |
| Network socket | Custom RAII wrapper |

---

## 🔀 Decision Tree: Which Rule to Use?

```
START
  |
  ├─> Can you use RAII types (vector, unique_ptr, string)?
  |    YES → Use RULE OF ZERO ✅ (BEST!)
  |    NO  → Continue
  |
  ├─> Do you manage raw resources (new/delete, fopen/fclose)?
  |    NO  → Use RULE OF ZERO ✅
  |    YES → Continue
  |
  └─> Do you need move semantics? (C++11+ and performance matters)
       YES → Use RULE OF FIVE ✅
       NO  → Use RULE OF THREE ✅
```

### Preference Order

1. **RULE OF ZERO** (1st choice - use RAII types)
2. **RULE OF FIVE** (2nd choice - if C++11+ and manual resources)
3. **RULE OF THREE** (3rd choice - if C++98 and manual resources)

---

## 🚀 Move Semantics Deep Dive

### What is std::move?

**Common misconception:** `std::move` moves the object.
**Reality:** `std::move` doesn't move anything! It just **casts** an lvalue to an rvalue reference.

```cpp
MyClass obj1(42);

// obj1 is an lvalue (has a name)
MyClass obj2 = obj1;           // Copy constructor (lvalue → lvalue)

// std::move(obj1) casts obj1 to rvalue
MyClass obj3 = std::move(obj1); // Move constructor (rvalue → lvalue)
```

### After std::move

```cpp
MyClass obj1(42);
MyClass obj2 = std::move(obj1);

// obj1 is now in a "valid but unspecified state"
// ✅ Can be destroyed safely
// ✅ Can be assigned to
// ❌ DON'T use it for anything else!

obj1 = MyClass(100);  // ✅ OK: Assignment
obj1.doSomething();   // ❌ UNDEFINED BEHAVIOR!
```

### When to Use std::move

✅ **When you're done with an object and want to transfer it:**
```cpp
vector<int> v(1000000);
MyClass obj(std::move(v));  // Transfer ownership
// Don't use v anymore!
```

✅ **Returning by value (sometimes):**
```cpp
MyClass createObject() {
    MyClass local;
    return std::move(local);  // Usually unnecessary - RVO is better!
}
```

✅ **In containers:**
```cpp
vector<MyClass> vec;
MyClass obj(42);
vec.push_back(std::move(obj));  // Move instead of copy
```

❌ **DON'T use on return (let RVO work):**
```cpp
// BAD: Prevents Return Value Optimization!
MyClass func() {
    MyClass obj;
    return std::move(obj);  // ❌ DON'T!
}

// GOOD: Let compiler optimize
MyClass func() {
    MyClass obj;
    return obj;  // ✅ Compiler will move automatically!
}
```

---

## ⚠️ Common Mistakes

### 1. Forgetting Self-Assignment Check

```cpp
// ❌ BAD: No self-assignment check
MyClass& operator=(const MyClass& other) {
    delete data;                    // Delete our data
    data = new int(*other.data);    // Copy from other
    // If this == &other, we already deleted it! 💥
    return *this;
}

// ✅ GOOD: With self-assignment check
MyClass& operator=(const MyClass& other) {
    if (this == &other) return *this;  // Check first!
    delete data;
    data = new int(*other.data);
    return *this;
}
```

### 2. Missing noexcept on Move Operations

```cpp
// ❌ BAD: No noexcept
MyClass(MyClass&& other) { ... }

// ✅ GOOD: With noexcept
MyClass(MyClass&& other) noexcept { ... }
```

**Why it matters:** STL containers (vector, etc.) won't use your move operations without `noexcept`! They'll fall back to copying, losing all performance benefits!

### 3. Not Leaving Moved-From Object in Valid State

```cpp
// ❌ BAD: Dangling pointer in source
MyClass(MyClass&& other) : data(other.data) {
    // other.data still points to the data!
    // When other is destroyed: delete already-moved data! 💥
}

// ✅ GOOD: Leave source in valid state
MyClass(MyClass&& other) : data(other.data) {
    other.data = nullptr;  // Now safe to destroy!
}
```

### 4. Defining Some But Not All

```cpp
// ❌ BAD: Only defines destructor
class BadClass {
    int* data;
public:
    ~BadClass() { delete data; }
    // Missing copy constructor and copy assignment!
    // Compiler generates shallow copies! 💥
};

// ✅ GOOD: Define all three (Rule of Three)
// Or better: Use Rule of Zero!
```

### 5. Using Moved-From Objects

```cpp
// ❌ BAD: Using moved-from object
MyClass obj1(42);
MyClass obj2 = std::move(obj1);
obj1.doSomething();  // 💥 UNDEFINED BEHAVIOR!

// ✅ GOOD: Don't use after move
MyClass obj1(42);
MyClass obj2 = std::move(obj1);
// Don't touch obj1 anymore (except assign/destroy)
```

### 6. Unnecessary std::move on Return

```cpp
// ❌ BAD: Prevents RVO (Return Value Optimization)
MyClass func() {
    MyClass obj;
    return std::move(obj);  // Compiler can't optimize!
}

// ✅ GOOD: Let compiler optimize
MyClass func() {
    MyClass obj;
    return obj;  // RVO: No move, no copy!
}
```

---

## ✅ Best Practices

### 1. Prefer Rule of Zero

**Always try this first:**
```cpp
class MyClass {
    vector<int> data;       // RAII
    unique_ptr<Foo> ptr;    // RAII
    string name;            // RAII
    // No special members needed!
};
```

### 2. If You Must Manage Resources

**Use Rule of Five (C++11+):**
```cpp
class MyClass {
    int* data;
public:
    ~MyClass();
    MyClass(const MyClass&);
    MyClass& operator=(const MyClass&);
    MyClass(MyClass&&) noexcept;
    MyClass& operator=(MyClass&&) noexcept;
};
```

### 3. Always Include

✅ Self-assignment check in copy assignment
✅ noexcept on move operations
✅ Leave moved-from in valid state
✅ Delete copy if only move makes sense

### 4. For Move Operations

```cpp
// Template for move constructor
MyClass(MyClass&& other) noexcept
    : data(other.data), size(other.size) {
    other.data = nullptr;  // Nullify source
    other.size = 0;
}

// Template for move assignment
MyClass& operator=(MyClass&& other) noexcept {
    if (this != &other) {
        delete data;           // Clean up our resource
        data = other.data;     // Steal other's resource
        size = other.size;
        other.data = nullptr;  // Nullify source
        other.size = 0;
    }
    return *this;
}
```

### 5. Performance Tips

```cpp
// ✅ Pass large objects by const&
void func(const vector<int>& v);  // No copy!

// ✅ Return by value (let compiler optimize)
vector<int> func() {
    vector<int> v(1000);
    return v;  // RVO or move
}

// ✅ Use emplace_back instead of push_back
vec.emplace_back(args...);  // Construct in-place
vec.push_back(MyClass(args...));  // Construct + move

// ✅ Reserve capacity for containers
vec.reserve(1000);  // Avoid reallocations
```

---

## 📊 Performance Comparison

### Copy vs Move

```cpp
class HeavyResource {
    vector<int> data;  // 1 million elements
public:
    // Copy constructor - EXPENSIVE!
    HeavyResource(const HeavyResource& other)
        : data(other.data) {
        // Copies 1 million elements
        // ~4MB data transfer
        // Time: ~100ms
    }
    
    // Move constructor - CHEAP!
    HeavyResource(HeavyResource&& other) noexcept
        : data(std::move(other.data)) {
        // Just swaps pointers
        // ~16 bytes (2 pointers)
        // Time: ~0.001ms
        // 100,000x FASTER! 🚀
    }
};
```

### Benchmark Results

| Operation | Elements | Copy Time | Move Time | Speedup |
|-----------|----------|-----------|-----------|---------|
| vector<int> | 1,000 | 0.01 ms | 0.00001 ms | 1,000x |
| vector<int> | 100,000 | 1 ms | 0.00001 ms | 100,000x |
| vector<int> | 1,000,000 | 10 ms | 0.00001 ms | 1,000,000x |
| string | 100 chars | 0.0001 ms | 0.00001 ms | 10x |
| string | 10,000 chars | 0.01 ms | 0.00001 ms | 1,000x |

---

## 🎓 Examples in the Code

The `RuleOf3_5_0.cpp` file contains **12 comprehensive examples**:

1. **The Problem** - Shows double deletion crash
2. **Rule of Three** - Solution with deep copy
3. **Self-Assignment** - Why the check matters
4. **Move Semantics Need** - Why moves are important
5. **Rule of Five** - Complete with move operations
6. **std::move Explained** - What it really does
7. **Rule of Zero** - The best approach with RAII
8. **When to Use** - Decision tree
9. **Default & Delete** - `= default` and `= delete`
10. **Performance** - Copy vs Move benchmark
11. **Common Mistakes** - What to avoid
12. **Best Practices** - How to do it right

---

## 🔗 Related Topics

### Prerequisites
- **02_Constructor** - Basic constructors
- **03_Destructor** - Resource cleanup
- **07_CopyConstructor** - Understanding copying

### Builds On
- **08_CopyAssignment** - Assignment operator
- **10_RvalueReferences** - && syntax
- **11_MoveSemantics** - Move operations

### Leads To
- **12_SmartPointers** - unique_ptr, shared_ptr (use moves!)
- **14_Templates** - Generic programming
- **15_PerfectForwarding** - std::forward
- **RAII** - Resource management pattern

---

## 🎯 Quick Reference

### Rule of Three (C++98)
```cpp
class MyClass {
    ~MyClass();                         // 1
    MyClass(const MyClass&);            // 2
    MyClass& operator=(const MyClass&); // 3
};
```

### Rule of Five (C++11)
```cpp
class MyClass {
    ~MyClass();                                // 1
    MyClass(const MyClass&);                   // 2
    MyClass& operator=(const MyClass&);        // 3
    MyClass(MyClass&&) noexcept;               // 4
    MyClass& operator=(MyClass&&) noexcept;    // 5
};
```

### Rule of Zero (Modern)
```cpp
class MyClass {
    vector<int> data;       // RAII!
    unique_ptr<Foo> ptr;    // RAII!
    string name;            // RAII!
    // Nothing else needed!
};
```

### Key Syntax

| Concept | Syntax |
|---------|--------|
| Copy constructor | `MyClass(const MyClass& other)` |
| Copy assignment | `MyClass& operator=(const MyClass& other)` |
| Move constructor | `MyClass(MyClass&& other) noexcept` |
| Move assignment | `MyClass& operator=(MyClass&&) noexcept` |
| Self-check | `if (this == &other) return *this;` |
| Cast to rvalue | `std::move(obj)` |
| Delete operation | `MyClass(const MyClass&) = delete;` |
| Default operation | `MyClass(const MyClass&) = default;` |

---

## 📝 Compilation & Execution

```bash
# Compile
g++ -std=c++17 -Wall -Wextra RuleOf3_5_0.cpp -o RuleOf3_5_0

# Run
./RuleOf3_5_0
```

**Expected Output:**
- Demonstrations of all three rules
- Copy vs Move comparisons
- Performance benchmarks
- Common mistakes examples
- Best practices guide

---

## 🎓 Summary

### The Golden Rules

1. **PREFER RULE OF ZERO** - Use RAII types (vector, unique_ptr, string)
2. **If managing resources** - Use Rule of Five (C++11) or Rule of Three (C++98)
3. **If you define ONE** - Define ALL (three or five)
4. **Move operations** - Always mark `noexcept`
5. **Moved-from objects** - Leave in valid state
6. **Self-assignment** - Always check in copy assignment

### Performance

| Operation | Complexity | Typical Time |
|-----------|------------|--------------|
| Copy | O(n) | ~100ms for 1M elements |
| Move | O(1) | ~0.001ms |
| **Speedup** | - | **100,000x faster!** 🚀 |

### When to Use

- **Always:** Try Rule of Zero first
- **Raw resources:** Use Rule of Five (C++11+) or Three (C++98)
- **Performance matters:** Implement move operations
- **Prevent copy:** Use `= delete` on copy operations

---

**Next Topics:**
- Smart Pointers (unique_ptr, shared_ptr)
- Perfect Forwarding (std::forward)
- Templates and Generic Programming

**Happy Coding!** 🚀