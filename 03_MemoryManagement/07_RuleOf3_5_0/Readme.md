# 30_RuleOf3_5_0 - The Ultimate Resource Management Guide

## 📚 Overview

This folder contains the **most important design rules** in C++: the **Rule of Three**, **Rule of Five**, and **Rule of Zero**. These rules dictate when and how to define special member functions for classes that manage resources.

Understanding these rules is **critical** for professional C++ development and for preventing memory leaks, crashes, and undefined behavior.

> **Status marks**
> ✅ *tested* = compiled and run with GCC 13.3 (`-Wall -Wextra`, AddressSanitizer, UBSan, LeakSanitizer).
> ⚠️ *not tested* = from the language rules or common practice, not run here.
> Timings are from one sandbox machine and will differ on yours.

## 📋 Table of Contents
- [What You'll Learn](#-what-youll-learn)
- [The Problem](#the-problem)
- [Rule of Three](#-rule-of-three-c98)
- [Rule of Five](#-rule-of-five-c11)
- [Rule of Zero](#-rule-of-zero-modern-c---best)
- [Decision Tree](#-decision-tree-which-rule-to-use)
- [Move Semantics Deep Dive](#-move-semantics-deep-dive)
- [Self-Assignment](#self-assignment-why-the-check-matters)
- [= default and = delete](#-default-and--delete)
- [Common Mistakes](#️-common-mistakes)
- [Best Practices](#-best-practices)
- [Performance Comparison](#-performance-comparison)
- [Examples in the Code](#-examples-in-the-code)
- [Quick Reference](#-quick-reference)
- [Compilation & Execution](#-compilation--execution)
- [Summary](#-summary)
- [Notes moved from main.cpp](#notes-moved-from-maincpp)

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

**The Issue:** The compiler-generated copy constructor and copy assignment operator do **shallow copies** - they copy the pointer value, not the data it points to. This causes:
- **Double deletion** (undefined behavior): ✅ AddressSanitizer reports `attempting double-free`; a plain build aborts with `free(): double free detected in tcache 2`.
- **Memory leaks** with the generated copy *assignment*: `b = a;` overwrites `b`'s pointer, so `b`'s old memory is lost. ✅ LeakSanitizer reports `Direct leak of 4 byte(s)`.
- Dangling pointers: after one object is destroyed the other one points to freed memory.

`main.cpp` contains this class as Example 1 (`BadResource`), but the call is commented out in `main()` because the program would crash. To see it, uncomment `demonstrateProblem();` and build with `-fsanitize=address`.

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

⚠️ This copy assignment deletes the old data *before* the new allocation. If `new` throws, `data` is left dangling. The exception-safe alternative is to allocate first and delete afterwards, or to use copy-and-swap. Not tested here; the demo class does the same as above for simplicity.

### Key Points

✅ **Deep copy** - Allocate new memory and copy the data
✅ **Self-assignment check** - `if (this != &other)`
✅ **All three or none** - Don't define just one or two!

`main.cpp` (Example 2) shows the copy constructor, copy assignment and the detected self-assignment `obj1 = obj1;` with printed messages.

---

## 🚀 Rule of Five (C++11)

### The Rule

**If your class needs ONE of these, it needs ALL FIVE:**

1. **Destructor** - `~MyClass()`
2. **Copy Constructor** - `MyClass(const MyClass&)`
3. **Copy Assignment Operator** - `MyClass& operator=(const MyClass&)`
4. **Move Constructor** - `MyClass(MyClass&&) noexcept` ⭐ NEW!
5. **Move Assignment Operator** - `MyClass& operator=(MyClass&&) noexcept` ⭐ NEW!

### Why do declaring some of them matter?

✅ Tested: a **user-declared destructor suppresses the implicit move operations**. A class with a destructor and a member whose copy and move constructors print messages printed `Tracer copy` for `WithDtor d = std::move(c);`, but `Tracer move` for the same class without the destructor. So `std::move` on such a class silently copies. Defining the destructor (and the copy operations) therefore means: also decide about the moves, either implement them or `= default` them.

### Why Move Semantics?

**Problem:** Copying large objects is EXPENSIVE!

```cpp
std::vector<int> v(1000000);      // 1 million elements
std::vector<int> v2 = v;          // COPIES 1 million elements! 😱
                            // 4,000,000 bytes (1M ints)
```

**Solution:** MOVE instead of copy!

```cpp
std::vector<int> v(1000000);      // 1 million elements
std::vector<int> v2 = std::move(v); // Just takes over the pointers! 🚀
                              // 24 bytes copied (3 pointers in libstdc++)
```

✅ `sizeof(vector<int>)` is 24 bytes (three pointers), not 16 as the old text said. Measured speed: see [Performance Comparison](#-performance-comparison).

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
✅ **Mark noexcept** - see the test below
✅ **Leave source valid** - Set pointer to nullptr
✅ **Much faster** - O(1) vs O(n)

### Why `noexcept` matters ✅

When `std::vector` grows it must move its elements to the new buffer. To keep its strong exception guarantee it uses the move constructor **only if it is `noexcept`** (or if the type cannot be copied at all). Tested with 100 `push_back`s:

| Element type | Copies | Moves |
|--------------|--------|-------|
| move constructor `noexcept` | 0 | 227 (100 for the temporaries + 127 during growth) |
| move constructor **not** `noexcept` | **127** | 100 (only the temporaries) |
| copy deleted, move not `noexcept` | - | 227 (no choice: it has to move) |

So the old sentence "STL won't use moves without noexcept" is only true when a copy is available: the vector then silently copies, which is exactly the lost performance.

`main.cpp` Example 3 runs the five operations of `RuleOfFive` and shows that the moved-from `Obj1` prints `data = nullptr`.

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
✅ **CORRECT by default** - the compiler generates what is right for the members
✅ **HARDER to make mistakes** - no manual memory management
✅ **EXCEPTION SAFE** - automatic cleanup

(The old text also promised "FASTER - compiler can optimize better" and "OPTIMAL implementations". That is not something I measured. The generated operations are simply memberwise, which is usually what a hand-written version would do.)

### Implementation

```cpp
// BAD: Manual memory management (Rule of Five)
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

// GOOD: Use RAII types (Rule of Zero)
class GoodClass {
    std::vector<int> data;        // RAII!
    std::vector<char> buffer;     // RAII!
    std::string name;             // RAII!
    std::unique_ptr<int> ptr;     // RAII!
public:
    GoodClass(int size, std::string n)
        : data(size), buffer(size), name(n), ptr(std::make_unique<int>(42)) {}

    // NO SPECIAL MEMBERS NEEDED! 🎉
    // What the compiler generates:
    // ~GoodClass()                           ✅ generated
    // GoodClass(const GoodClass&)            ❌ DELETED (unique_ptr member cannot be copied)
    // GoodClass& operator=(const GoodClass&) ❌ DELETED
    // GoodClass(GoodClass&&)                 ✅ generated
    // GoodClass& operator=(GoodClass&&)      ✅ generated
};
```

**Correction.** The old code comment said that the compiler generates "ALL FIVE correctly" including the copy operations (`= default`). That is wrong for this class: because of the `unique_ptr` member the copy constructor and copy assignment are **deleted**, and only the moves work. ✅ Tested with `is_copy_constructible_v`. `main.cpp` Example 4 states this correctly (the copy line is commented out and the move works). If you want a copyable class, hold the object by value or use `shared_ptr`.

### What the members decide ✅

The Rule-of-Zero class gets the strictest combination of its members:

| Type | Copy | Move |
|------|------|------|
| `vector`, `string`, `shared_ptr` | ✅ | ✅ |
| `unique_ptr`, `std::thread`, `ifstream` | ❌ | ✅ |
| `std::mutex` | ❌ | ❌ |
| class with a `unique_ptr` member (`AutoResource`) | ❌ | ✅ |
| class with a `std::mutex` member | ❌ | ❌ |

A class that contains a `mutex` can be neither copied nor moved unless you write those operations yourself (and then decide what to do with the mutex).

### RAII Types to Use

| Resource Type | Use This |
|--------------|----------|
| Dynamic single object | `unique_ptr<T>` |
| Dynamic array | `vector<T>` |
| Shared ownership | `shared_ptr<T>` |
| String data | `string` |
| File handle | `fstream`, `ifstream`, `ofstream` |
| Thread | `std::thread` (must be joined or detached before destruction) |
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

### Examples of each

```cpp
// Rule of Zero
class MyClass {
    std::vector<int> data;       // RAII!
    std::unique_ptr<Foo> ptr;    // RAII!
    std::string name;            // RAII!
};                          // No special members needed!

// Rule of Five
class MyClass {
    int* data;  // Raw pointer
public:
    ~MyClass();                              // 1
    MyClass(const MyClass&);                 // 2
    MyClass& operator=(const MyClass&);      // 3
    MyClass(MyClass&&) noexcept;             // 4
    MyClass& operator=(MyClass&&) noexcept;  // 5
};

// Rule of Three
class MyClass {
    int* data;  // Raw pointer (C++98)
public:
    ~MyClass();                              // 1
    MyClass(const MyClass&);                 // 2
    MyClass& operator=(const MyClass&);      // 3
};
```

---

## 🚀 Move Semantics Deep Dive

### What is std::move?

**Common misconception:** `std::move` moves the object.
**Reality:** `std::move` doesn't move anything! It just **casts** an lvalue to an rvalue reference, so that the move constructor / move assignment is chosen.

```cpp
MyClass obj1(42);

// obj1 is an lvalue (has a name)
MyClass obj2 = obj1;           // Copy constructor

// std::move(obj1) casts obj1 to rvalue
MyClass obj3 = std::move(obj1); // Move constructor
```

### After std::move

```cpp
MyClass obj1(42);
MyClass obj2 = std::move(obj1);

// obj1 is now in a "valid but unspecified state"
// ✅ Can be destroyed safely
// ✅ Can be assigned to
// ⚠️ Anything else depends on the type - don't rely on its value

obj1 = MyClass(100);  // ✅ OK: Assignment
```

**Correction:** the old text called any use of a moved-from object "UNDEFINED BEHAVIOR". The standard says the object is in a *valid but unspecified* state. Calling functions without preconditions is allowed; what is unspecified is the *value*. ✅ For the standard library types in libstdc++ a moved-from `vector` and a moved-from long `string` both had size 0, and assigning to the moved-from `vector` worked, but that is an implementation detail. The real undefined behavior comes from your own class when it breaks its own invariants: in `RuleOfFive` the moved-from object has `data == nullptr`, so `*data` would be UB (the `display()` method checks for `nullptr` for this reason).

### When to Use std::move

✅ **When you're done with an object and want to transfer it:**
```cpp
std::vector<int> v(1000000);
MyClass obj(std::move(v));  // Transfer ownership
// Don't rely on v anymore!
```

✅ **In containers:**
```cpp
std::vector<MyClass> vec;
MyClass obj(42);
vec.push_back(std::move(obj));  // Move instead of copy
```

❌ **DON'T use on return of a local (let the compiler elide):**
```cpp
// BAD: Prevents copy elision
MyClass func() {
    MyClass obj;
    return std::move(obj);  // ❌ DON'T!
}

// GOOD: Let compiler optimize
MyClass func() {
    MyClass obj;
    return obj;  // ✅ elided, or moved automatically if not elided
}
```

✅ Tested with a class that prints its constructors: `return t;` printed only `ctor` (no copy, no move); `return std::move(t);` printed `ctor` then `move`, and GCC warned `moving a local object in a return statement prevents copy elision [-Wpessimizing-move]`. Even when the compiler cannot elide (a function that returns one of two locals), `return x;` still *moves* automatically instead of copying ✅. The old README listed `return std::move(local)` both as a good use "sometimes" and as a bad one; it is only the bad one.

("RVO" for a *temporary* is guaranteed since C++17; for a *named* local it is allowed but not guaranteed. ⚠️ language rule.)

---

## Self-Assignment: why the check matters

```cpp
// ❌ BAD: no self-assignment check
S& operator=(const S& other) {
    delete data;                    // Delete our data
    data = new int(*other.data);    // Copy from other - but if this == &other, it is already deleted
    return *this;
}

// ✅ GOOD
S& operator=(const S& other) {
    if (this == &other) return *this;   // Safe!
    delete data;
    data = new int(*other.data);
    return *this;
}
```

✅ Tested: `a = a;` with the bad version is reported by AddressSanitizer as `heap-use-after-free`.

Self-assignment can happen:
- directly: `obj = obj;`
- through pointers or references: `*ptr1 = *ptr2;` when both are the same
- through containers: `arr[i] = arr[j];` when `i == j`

(Copy-and-swap ⚠️ avoids the need for the check and is also exception safe.)

---

## 🧰 = default and = delete

- `= default` asks the compiler to generate the normal version.
- `= delete` forbids the operation (using it is a compile error).

```cpp
class NonCopyable {
    std::unique_ptr<int> data;
public:
    NonCopyable(int value) : data(std::make_unique<int>(value)) {}

    // Delete copy operations
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;

    // Default move operations
    NonCopyable(NonCopyable&&) = default;
    NonCopyable& operator=(NonCopyable&&) = default;
};
```

Common patterns:
1. **Non-copyable:** delete copy, default/custom move
2. **Non-moveable:** delete move, default/custom copy: but see the warning below
3. **Neither:** delete both (singletons)
4. **Both:** default both

⚠️ **Gotcha (✅ tested).** A *deleted* move operation is still considered by overload resolution. If a class has a copy constructor and `X(X&&) = delete;`, then `X c = std::move(a);` is a **compile error** (`use of deleted function 'X::X(X&&)'`), it does **not** fall back to the copy. Without the deleted move, the same line compiles and copies. So pattern 2 makes the class unusable with rvalues; to say "copy only", simply don't declare a move at all.

---

## ⚠️ Common Mistakes

### 1. Forgetting Self-Assignment Check

See [Self-Assignment](#self-assignment-why-the-check-matters) ✅ (`heap-use-after-free`).

### 2. Missing noexcept on Move Operations

```cpp
// ❌ BAD: No noexcept
MyClass(MyClass&& other) { ... }

// ✅ GOOD: With noexcept
MyClass(MyClass&& other) noexcept { ... }
```

**Why it matters:** ✅ `std::vector` growth copies elements whose move is not `noexcept` (127 copies in the test above) whenever a copy constructor exists.

### 3. Not Leaving Moved-From Object in Valid State

```cpp
// ❌ BAD: Dangling pointer in source
MyClass(MyClass&& other) : data(other.data) {
    // other.data still points to the data!
    // When other is destroyed: the data is deleted twice! 💥
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

### 5. Relying on the Value of a Moved-From Object

```cpp
MyClass obj1(42);
MyClass obj2 = std::move(obj1);
obj1.doSomething();  // ❌ depends entirely on what your move left behind
```
Not necessarily undefined behavior (see the correction above), but you cannot rely on the value. Only destroy it, or assign to it.

### 6. Unnecessary std::move on Return

See [When to Use std::move](#when-to-use-stdmove) ✅ (`-Wpessimizing-move`).

### 7. Not using RAII types

```cpp
int* data = new int[size];                    // ❌
std::vector<int> data(size);                       // ✅
std::unique_ptr<int[]> data(new int[size]);        // ✅ (or make_unique<int[]>(size))
```

---

## ✅ Best Practices

### 1. Prefer Rule of Zero

```cpp
class MyClass {
    std::vector<int> data;       // RAII
    std::unique_ptr<Foo> ptr;    // RAII
    std::string name;            // RAII
    // No special members needed!
};
```

### 2. If You Must Manage Resources

**Use Rule of Five (C++11+):** see the five declarations above.

### 3. Always Include

✅ Self-assignment check in copy assignment
✅ `noexcept` on move operations
✅ Leave moved-from in valid state
✅ Delete copy if only move makes sense
✅ Make destructors `virtual` in base classes that are deleted through a base pointer

### 4. Templates for move operations

```cpp
// Move constructor
MyClass(MyClass&& other) noexcept
    : data(other.data), size(other.size) {
    other.data = nullptr;  // Nullify source
    other.size = 0;
}

// Move assignment (delete[] if data came from new[])
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
void func(const std::vector<int>& v);  // No copy!

// ✅ Return by value (let compiler optimize)
std::vector<int> func() {
    std::vector<int> v(1000);
    return v;  // elided or moved
}

// ✅ emplace_back constructs in place
vec.emplace_back(args...);          // construct in place
vec.push_back(MyClass(args...));    // construct a temporary + move it

// ✅ Reserve capacity for containers
vec.reserve(1000);  // Avoid reallocations
```

✅ Tested `emplace_back`: `push_back(P(1))` printed `ctor` + `move`; `emplace_back(2)` printed only `ctor`. The saving is one move per element, so it matters most for types with an expensive move.

---

## 📊 Performance Comparison

### Copy vs Move

```cpp
class HeavyResource {
    std::vector<int> data;  // 1 million elements
public:
    // Copy constructor - EXPENSIVE!
    HeavyResource(const HeavyResource& other) : data(other.data) {}
    // copies all 1 million elements (4,000,000 bytes)

    // Move constructor - CHEAP!
    HeavyResource(HeavyResource&& other) noexcept
        : data(std::move(other.data)) {}
    // takes over the vector's three pointers (24 bytes)
};
```

### Measured results ✅

Copy-constructing versus move-constructing a `std::vector<int>` (g++ 13.3, `-O2`, averaged over many repetitions on the sandbox machine; the vector is not touched afterwards, apart from one read to keep the copy from being optimized away):

| Elements | Copy | Move | Ratio (copy / move) |
|----------|------|------|---------------------|
| 1,000 | ≈ 0.1 µs | ≈ 0.03 µs | ≈ 3x |
| 100,000 | ≈ 12 µs | ≈ 0.07 µs | ≈ 170x |
| 1,000,000 | ≈ 0.31–0.32 ms | ≈ 0.08 µs | ≈ 3,600–4,100x (two runs) |

What this shows: the move time stays roughly constant (O(1)), the copy time grows with the size (O(n)). The exact ratio depends on the machine, the allocator and the cache, so treat the numbers as an order of magnitude.

**The old README's figures were wrong.** It claimed "250,000x", "100,000x" and "1,000,000x" in different places, a copy time of "~100 ms" for 1 million `int`s, and a benchmark table with strings. None of these were measured, they contradicted each other, and 100 ms is about 300 times more than what I measured (≈ 0.3 ms). Short strings are not worth comparing: small-string optimization copies them without allocating anything. The tables have been replaced by the measurements above.

---

## 🎓 Examples in the Code

`main.cpp` has **6 examples** (Example 1 is not called by default):

1. **The Problem** - shows the double deletion crash (call is commented out)
2. **Rule of Three** - deep copy, copy assignment, self-assignment
3. **Rule of Five** - copy and move operations, moved-from state
4. **Rule of Zero** - `AutoResource` with RAII members
5. **= default & = delete** - a non-copyable, movable class
6. **Copy vs Move** - copy and move of a 1-million-element vector (`HeavyResource`)

The other topics (self-assignment, why move, `std::move`, decision tree, common mistakes, best practices) are in this README.

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
    std::vector<int> data;       // RAII!
    std::unique_ptr<Foo> ptr;    // RAII!
    std::string name;            // RAII!
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

`main.cpp` uses `std::println` (`<print>`), so it needs **C++23 and GCC 14+** (or another standard library that ships `<print>`):

```bash
# Compile
g++ -std=c++23 -Wall -Wextra main.cpp -o RuleOf3_5_0

# Run
./RuleOf3_5_0
```

(The old command used `-std=c++17`; the rules themselves are older, only the printing needs C++23.)

**Expected Output:** the banner, then Examples 2–6: construction and destruction messages, the copy / move messages, `data = nullptr` for the moved-from object, and the two lines `Copy` / `Move` for the 1-million-element object.

---

## 🎓 Summary

### The Golden Rules

1. **PREFER RULE OF ZERO** - Use RAII types (vector, unique_ptr, string)
2. **If managing resources** - Use Rule of Five (C++11) or Rule of Three (C++98)
3. **If you define ONE** - Define ALL (three or five)
4. **Move operations** - Always mark `noexcept`
5. **Moved-from objects** - Leave in valid state
6. **Self-assignment** - Always check in copy assignment

### Performance (✅ measured, see above)

| Operation | Complexity | 1,000,000 `int`s |
|-----------|------------|------------------|
| Copy | O(n) | ≈ 0.3 ms |
| Move | O(1) | ≈ 0.08 µs |

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

---

## Notes moved from main.cpp

The title/description comment block at the top of `main.cpp` ("WHAT IS THE RULE OF THREE/FIVE/ZERO?") stays. What changed:

- `std::cout` → `std::println`; `#include <print>` added, `<iostream>` removed.
- Each example has a `// === EXAMPLE n: … ===` title comment.
- **Examples that only printed lecture text were removed from the program** (everything is in this README):
  - old Example 3 *Self-assignment* (and the unused `SelfAssignmentDemo` class) → [Self-Assignment](#self-assignment-why-the-check-matters);
  - old Example 4 *Why move semantics?* (and the unused `ExpensiveToCopy` class and `createLargeObject()`) → [Rule of Five](#-rule-of-five-c11);
  - old Example 6 *std::move explained* → [Move Semantics Deep Dive](#-move-semantics-deep-dive);
  - old Example 8 *When to use each rule* → [Decision Tree](#-decision-tree-which-rule-to-use);
  - old Example 11 *Common mistakes* → [Common Mistakes](#️-common-mistakes);
  - old Example 12 *Best practices* → [Best Practices](#-best-practices);
  - the final **KEY TAKEAWAYS** block printed by `main()` → [Summary](#-summary).
- The examples that remain are renumbered: old 2 → **2**, old 5 → **3**, old 7 → **4**, old 9 → **5**, old 10 → **6**. (Example 1 keeps its number, its call stays commented out, as in the original.)
- **Printed lecture lines removed from the remaining examples:** the "RULE OF THREE / FIVE / ZERO" explanations, "Benefits" list, the printed `NonCopyable` code and "Common patterns" list (→ [= default and = delete](#-default-and--delete)), the "Performance difference" block, and the closing "✅ …" statements.
- The comment block at the end of `main.cpp` (COMPILATION / RUN / TOPICS COVERED / OUTPUT) → [Compilation & Execution](#-compilation--execution) and the table of contents.

### Corrections to the original files

1. **`AutoResource` comment:** the code claimed the compiler generates all five special members, including copy. With a `unique_ptr` member the copy operations are **deleted**. Fixed in `main.cpp` and in the README's `GoodClass`.
2. **`vector<int>` is 24 bytes, not 16** ("2 pointers").
3. **Benchmark numbers** (250,000x, 100,000x, 1,000,000x, "~100 ms", string rows) were unmeasured and contradicted each other: replaced by measurements.
4. **Moved-from objects:** "UNDEFINED BEHAVIOR" → valid but unspecified state.
5. **`return std::move(local)`** was listed as both good and bad: it is bad (`-Wpessimizing-move`).
6. **"STL won't use moves without noexcept"** → precisely: `std::vector` falls back to *copying* if a copy exists.
7. **"Memory leaks"** from shallow copy: only copy *assignment* leaks (the old pointer is lost); copy construction causes the double delete.
8. **`= delete` on move** is not the same as "no move": it breaks `X c = std::move(a);` instead of falling back to copy.
9. "Compiler generates OPTIMAL implementations" / "FASTER" for Rule of Zero removed (not measured).
10. Compile command `-std=c++17` → `-std=c++23` because of `<print>`.

### Tested

✅ `g++ -std=c++23 -Wall -Wextra` with AddressSanitizer + UBSan: no warnings, no errors; LeakSanitizer: no leaks.
✅ Output of Examples 2, 3, 4, 5 and 6 is identical to the original; only the removed lecture lines differ.
✅ Example 1 with the call enabled: AddressSanitizer `attempting double-free`; a plain build aborts with `double free detected`.
✅ Bad self-assignment (`heap-use-after-free`), leak from shallow copy assignment, `noexcept` and vector growth (copies 0 versus 127), `return std::move` (extra move + `-Wpessimizing-move`), implicit move suppressed by a user destructor, copy/move traits of `AutoResource`, `unique_ptr`, `thread`, `ifstream`, `mutex`, deleted-move gotcha, moved-from `vector` and `string`, `emplace_back` versus `push_back`, `sizeof(vector<int>) == 24`, copy and move timings.
⚠️ Not tested: copy-and-swap, exception safety of the copy assignment, behavior on compilers other than GCC, `std::println` with GCC 14 itself (the sandbox has GCC 13; a small stand-in header with the same behavior was used).
