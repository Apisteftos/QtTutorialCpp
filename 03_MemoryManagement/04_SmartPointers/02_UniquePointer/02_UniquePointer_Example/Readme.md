# unique_ptr - Smart Pointer with Unique Ownership

## Overview

**unique_ptr** is a smart pointer that owns and manages another object through a pointer and deletes that object when the unique_ptr goes out of scope. It provides automatic memory management with zero overhead compared to raw pointers.

**File:** `main.cpp`  
**Difficulty:** 🟡 Intermediate  
**C++ Version:** `unique_ptr` is C++11 (`make_unique` in C++14); `main.cpp` itself uses `std::println`, so it is built with C++23  
**Compile:** `g++ -std=c++23 -Wall -Wextra main.cpp -o unique_ptr`

---

## Table of Contents

1. [What Is unique_ptr?](#what-is-unique_ptr)
2. [The Problem It Solves](#the-problem-it-solves)
3. [Basic Syntax](#basic-syntax)
4. [Examples Overview](#examples-overview)
5. [Ownership and Moving](#ownership-and-moving)
6. [Creating unique_ptr](#creating-unique_ptr)
7. [Common Operations](#common-operations)
8. [Best Practices](#best-practices)
9. [Common Mistakes](#common-mistakes)
10. [Performance](#performance)
11. [When to Use](#when-to-use)
12. [Related Concepts](#related-concepts)

---

## What Is unique_ptr?

### Core Concept

unique_ptr is a **smart pointer** that:
- **Owns** a dynamically allocated object exclusively
- **Automatically deletes** the object when destroyed
- **Cannot be copied** (unique ownership)
- **Can be moved** (transfer ownership)
- Has **zero overhead** vs raw pointers

### Key Properties

```cpp
std::unique_ptr<T> ptr = std::make_unique<T>(args);

✅ Automatic cleanup     - Destructor calls delete
✅ Unique ownership      - Only one owner
✅ Move semantics        - Transfer with std::move
✅ Exception safe        - RAII pattern
✅ Zero overhead         - Same cost as raw pointer
❌ Cannot std::copy           - Copy constructor deleted
```

---

## The Problem It Solves

### Problem: Manual Memory Management

```cpp
// ❌ Raw pointer problems
Widget* ptr = new Widget();

// Problem 1: Forget to delete
if (condition) {
    return;  // ❌ Memory leak!
}

// Problem 2: Exception safety
process();  // If throws, leak!
delete ptr;  // Never reached

// Problem 3: Double deletion
delete ptr;
delete ptr;  // ❌ Undefined behavior!

// Problem 4: Who deletes?
Widget* shared = ptr;  // Who owns it?
```

### Solution: unique_ptr

```cpp
// ✅ unique_ptr solution
auto ptr = std::make_unique<Widget>();

// ✅ Automatic cleanup on:
// - Normal scope exit
// - Early return
// - Exception thrown
// - Any code path!

// ✅ No double deletion (move-only)
// ✅ Clear ownership (unique owner)
```

---

## Basic Syntax

### Creation

```cpp
// ✅ Preferred way (C++14+)
auto ptr = std::make_unique<T>(args);

// ✅ OK (C++11)
std::unique_ptr<T> ptr(new T(args));

// ✅ For arrays
auto arr = std::make_unique<T[]>(size);
```

### Access

```cpp
ptr->member()      // Access member
*ptr               // Dereference
ptr.get()          // Get raw pointer (rarely needed)
ptr.operator->()   // Same as ->
ptr.operator*()    // Same as *
```

### Ownership Transfer

```cpp
auto p2 = std::move(ptr);  // Transfer ownership
ptr.reset(new T());        // Delete old, take new
T* raw = ptr.release();    // Give up ownership (returns raw)
```

### Check Validity

```cpp
if (ptr) { }       // Check if not nullptr
if (!ptr) { }      // Check if nullptr
ptr == nullptr     // Compare
ptr != nullptr     // Compare
```

---

## Examples Overview

### Example 1: Raw Pointer Problems

Demonstrates all the problems with manual memory management:
- Memory leaks on early return
- Memory leaks on exceptions
- Double deletion bugs
- Complex ownership semantics

### Example 2: Basic unique_ptr

Simple usage showing automatic cleanup:
```cpp
{
    auto ptr = std::make_unique<Widget>(42);
    ptr->doWork();
}  // ✅ Automatically deleted here
```

### Example 3: make_unique (C++14)

Why prefer make_unique over `new`:
```cpp
// ✅ Preferred — no naked new, type written once
auto p = std::make_unique<T>(args);

// ❌ Old way — still works
std::unique_ptr<T> p(new T(args));
```

**About exception safety:** before C++17, `func(unique_ptr<T>(new T), other_func());` could leak, because the
compiler was allowed to run `new T`, then `other_func()`, and only then construct the `unique_ptr`.
Since C++17 the two arguments can no longer be interleaved that way, so this call is safe too
(C++17 evaluation-order rule P0145 — *not tested here*). `make_unique` is still preferred for the other reasons below.

### Example 4: Moving unique_ptr

Transferring ownership:
```cpp
auto ptr1 = std::make_unique<Widget>();
auto ptr2 = std::move(ptr1);  // Transfer ownership

// ptr1 is now nullptr
// ptr2 owns the Widget
```

### Example 5: reset() and release()

```cpp
ptr.reset();              // Delete and set to nullptr
ptr.reset(new T());       // Delete old, take new

T* raw = ptr.release();   // Give up ownership
delete raw;               // Must manually delete!
```

### Example 6: Custom Deleters

For resources needing special cleanup:
```cpp
struct FileDeleter {
    void operator()(FILE* f) const {
        if (f) fclose(f);
    }
};

std::unique_ptr<FILE, FileDeleter> file(fopen("file.txt", "r"));
// Automatically calls fclose on destruction
```

### Example 7: Arrays with unique_ptr

```cpp
// ✅ For arrays
auto arr = std::make_unique<int[]>(5);
arr[0] = 10;  // Supports []

// ⚠️ But prefer vector!
std::vector<int> v(5);  // Better choice
```

### Example 8: Returning from Functions

Transfer ownership to caller:
```cpp
std::unique_ptr<Widget> create() {
    return std::make_unique<Widget>();
    // Ownership transferred to caller
}

auto widget = create();  // Now owns it
```

### Example 9: Polymorphism

Perfect for polymorphic collections:
```cpp
std::vector<std::unique_ptr<Animal>> animals;
animals.push_back(std::make_unique<Dog>());
animals.push_back(std::make_unique<Cat>());

for (auto& animal : animals) {
    animal->makeSound();  // Polymorphic call
}
// All automatically deleted
```

### Example 10: RAII Resource Manager

Complete resource management system:
```cpp
class Application {
    std::unique_ptr<Database> db;
    std::unique_ptr<Logger> logger;
    
public:
    Application() {
        db = std::make_unique<Database>();
        logger = std::make_unique<Logger>();
    }
    // Destructor automatically cleans up everything
};
```

---

## Ownership and Moving

### Unique Ownership

```cpp
auto ptr = std::make_unique<Widget>();

// ❌ Cannot copy
auto ptr2 = ptr;  // Won't compile!

// ✅ Can move
auto ptr2 = std::move(ptr);  // OK!

// After move:
// ptr is nullptr
// ptr2 owns the Widget
```

### Visual Representation

```
Before move:
ptr1  ───────> [Widget]
ptr2  ───────> nullptr

After: auto ptr2 = std::move(ptr1);

ptr1  ───────> nullptr
ptr2  ───────> [Widget]
```

### Passing to Functions

```cpp
// Takes ownership (by value)
void take_ownership(std::unique_ptr<T> ptr) {
    // ptr deleted at end of function
}

// Borrow (by reference) - doesn't take ownership
void borrow(const std::unique_ptr<T>& ptr) {
    // ptr still owned by caller
}

// Better: Just use raw pointer for borrowing
void borrow(T* ptr) {
    // Clearer intent
}

// Usage
auto ptr = std::make_unique<T>();

take_ownership(std::move(ptr));  // Transfer
// ptr is now nullptr

borrow(ptr);  // Still owns it
```

---

## Creating unique_ptr

### make_unique (Preferred)

```cpp
// ✅ Best way (C++14+)
auto ptr = std::make_unique<Widget>(arg1, arg2);

// Advantages:
// 1. No need to repeat the type
// 2. Cannot accidentally use a raw pointer / naked new
// 3. Consistent with make_shared
// 4. (Pre-C++17 only) Exception safe inside function-call arguments
```

### Constructor (C++11)

```cpp
// ✅ OK (C++11)
std::unique_ptr<Widget> ptr(new Widget(arg1, arg2));

// ⚠️ Before C++17 this could leak (since C++17 it cannot):
// func(unique_ptr<T>(new T), other_func());
// If other_func() throws, potential leak
```

### Arrays

```cpp
// For arrays - use make_unique<T[]>
auto arr = std::make_unique<int[]>(10);

// ⚠️ But prefer vector!
std::vector<int> v(10);  // Better
```

### From Raw Pointer

```cpp
Widget* raw = new Widget();
std::unique_ptr<Widget> ptr(raw);  // Takes ownership

// ⚠️ Dangerous - don't do:
std::unique_ptr<Widget> p1(raw);
std::unique_ptr<Widget> p2(raw);  // ❌ Double ownership!
```

---

## Common Operations

### Accessing the Object

```cpp
auto ptr = std::make_unique<Widget>();

ptr->member()       // Call member function
(*ptr).member()     // Same as above
*ptr                // Dereference
ptr[i]              // For arrays (unique_ptr<T[]>)
```

### Getting Raw Pointer

```cpp
Widget* raw = ptr.get();

// Use cases:
// 1. Passing to legacy C API
// 2. Observing (not owning)
// 3. Debugging

// ⚠️ Don't:
// - Delete the raw pointer
// - Store it long-term
// - Create another unique_ptr from it
```

### Replacing the Object

```cpp
// Delete current and take nullptr
ptr.reset();

// Delete current and take new
ptr.reset(new Widget());

// Or use assignment
ptr = std::make_unique<Widget>();  // Easier!
```

### Releasing Ownership

```cpp
// Give up ownership without deleting
Widget* raw = ptr.release();

// Now YOU must delete
delete raw;

// ptr is now nullptr
```

### Swapping

```cpp
std::unique_ptr<T> p1 = std::make_unique<T>();
std::unique_ptr<T> p2 = std::make_unique<T>();

p1.swap(p2);  // Swap ownership
// or
std::swap(p1, p2);  // Same thing
```

---

## Best Practices

### ✅ DO

1. **Use make_unique by default**

```cpp
// ✅ Preferred
auto ptr = std::make_unique<Widget>(args);

// ❌ Avoid
std::unique_ptr<Widget> ptr(new Widget(args));
```

2. **Return by value**

```cpp
// ✅ Good - ownership transferred to caller
std::unique_ptr<T> create() {
    return std::make_unique<T>();
}
```

3. **Move when transferring ownership**

```cpp
// ✅ Explicit transfer
take_ownership(std::move(ptr));
```

4. **Use for polymorphism**

```cpp
// ✅ Perfect for polymorphic collections
std::vector<std::unique_ptr<Base>> items;
items.push_back(std::make_unique<Derived>());
```

5. **Check before dereferencing**

```cpp
// ✅ Safe
if (ptr) {
    ptr->doSomething();
}
```

6. **Pass raw pointer for observation**

```cpp
// ✅ Clear: observer, not owner
void process(Widget* widget);

process(ptr.get());
```

7. **Use for RAII resources**

```cpp
// ✅ Automatic cleanup
class Manager {
    std::unique_ptr<Resource> resource;
};
```

### ❌ DON'T

1. **Don't try to copy**

```cpp
// ❌ Won't compile
auto ptr2 = ptr1;

// ✅ Move instead
auto ptr2 = std::move(ptr1);
```

2. **Don't use for arrays when vector works**

```cpp
// ❌ Overkill
std::unique_ptr<int[]> arr = std::make_unique<int[]>(10);

// ✅ Better
std::vector<int> arr(10);
```

3. **Don't store raw pointers from get()**

```cpp
// ❌ Dangerous
Widget* raw = ptr.get();
// ptr deleted → raw is dangling!

// ✅ Keep unique_ptr alive
```

4. **Don't create multiple unique_ptrs from same raw pointer**

```cpp
Widget* raw = new Widget();
std::unique_ptr<Widget> p1(raw);
std::unique_ptr<Widget> p2(raw);  // ❌ Double delete!
```

5. **Don't manually delete**

```cpp
// ❌ Never
delete ptr.get();

// ✅ Let unique_ptr handle it
ptr.reset();  // or just let it go out of scope
```

6. **Don't use release() unless necessary**

```cpp
// ❌ Usually wrong
Widget* raw = ptr.release();
delete raw;

// ✅ Just let it be destroyed
// (ptr goes out of scope)
```

---

## Common Mistakes

### Mistake 1: Trying to Copy

```cpp
// ❌ Won't compile
std::unique_ptr<T> p1 = std::make_unique<T>();
std::unique_ptr<T> p2 = p1;  // Error!

// ✅ Move instead
std::unique_ptr<T> p2 = std::move(p1);
```

### Mistake 2: Wrong Array Syntax

```cpp
// ❌ Wrong - uses delete instead of delete[]
std::unique_ptr<int> arr(new int[10]);

// ✅ Correct - uses delete[]
std::unique_ptr<int[]> arr = std::make_unique<int[]>(10);

// ✅✅ Best - use vector
std::vector<int> arr(10);
```

### Mistake 3: Double Ownership

```cpp
// ❌ Both will try to delete same object
Widget* raw = new Widget();
std::unique_ptr<Widget> p1(raw);
std::unique_ptr<Widget> p2(raw);  // BOOM!

// ✅ Only one unique_ptr per object
auto p1 = std::make_unique<Widget>();
// No p2!
```

### Mistake 4: Using After Move

```cpp
auto ptr1 = std::make_unique<Widget>();
auto ptr2 = std::move(ptr1);

// ❌ ptr1 is nullptr now!
ptr1->doWork();  // Undefined behavior (typically a crash)

// ✅ Check first
if (ptr1) {
    ptr1->doWork();
}
```

### Mistake 5: Returning Reference

```cpp
// ❌ Don't return reference
std::unique_ptr<T>& create() {
    std::unique_ptr<T> ptr = std::make_unique<T>();
    return ptr;  // Dangling reference!
}

// ✅ Return by value
std::unique_ptr<T> create() {
    return std::make_unique<T>();  // Ownership transferred
}
```

### Mistake 6: Storing Raw Pointer Long-term

```cpp
// ❌ Dangerous
Widget* raw = ptr.get();
// ... later ...
ptr.reset();  // Deleted!
raw->doWork();  // Crash! Dangling pointer

// ✅ Keep unique_ptr alive
```

---

## Performance

### Size

```cpp
sizeof(std::unique_ptr<T>)  ==  sizeof(T*)    // 8 bytes on x86-64 ✅ tested

// Same size as raw pointer — with the DEFAULT deleter.
// With a function-pointer deleter or a capturing lambda it is 16 bytes ✅ tested
```

### Speed

No timing numbers are given here: the old table and benchmark figures were not measured, so they were removed.
To measure on your machine, benchmark with `-O2` and look at the generated assembly yourself.

With the default deleter, `unique_ptr` is a thin wrapper: no reference count, no extra allocation,
and `operator->` / `operator*` are inlined.

### Compiler Optimization

```cpp
// Code:
void process() {
    auto ptr = std::make_unique<Widget>(42);
    ptr->work();
}

// Roughly equivalent to:
void process() {
    Widget* ptr = new Widget(42);
    ptr->work();
    delete ptr;
}
```
✅ tested with `g++ -O2 -S`: the two functions are **not** byte-for-byte identical. The `unique_ptr` version
also contains cleanup code that deletes the object if `work()` throws, which the raw-pointer version
does not have (it would leak). So "same performance" is right for the normal path, but "identical assembly" is not.

### Key Insights

- **Zero abstraction overhead**
- **Same size as a raw pointer** (default deleter)
- **Normal-path cost is the same as manual new/delete**
- **A deleter that holds state makes the `unique_ptr` bigger** (16 bytes for a function-pointer or capturing-lambda deleter — ✅ tested)

---

## When to Use

### ✅ Use unique_ptr When:

1. **Single ownership needed**
   ```cpp
   class Manager {
       std::unique_ptr<Resource> resource;  // Manager owns it
   };
   ```

2. **Factory functions**
   ```cpp
   std::unique_ptr<Widget> createWidget() {
       return std::make_unique<Widget>();
   }
   ```

3. **Polymorphic collections**
   ```cpp
   std::vector<std::unique_ptr<Base>> items;
   items.push_back(std::make_unique<Derived>());
   ```

4. **RAII resources**
   ```cpp
   std::unique_ptr<FILE, FileDeleter> file(fopen(...));
   ```

5. **Preventing copies**
   ```cpp
   class NonCopyable {
       std::unique_ptr<Impl> pImpl;  // Can't copy
   };
   ```

6. **Optional ownership**
   ```cpp
   std::unique_ptr<T> maybeCreate(bool create) {
       return create ? std::make_unique<T>() : nullptr;
   }
   ```

### ❌ Don't Use When:

1. **Shared ownership needed**
   ```cpp
   // ❌ unique_ptr - can't share
   // ✅ shared_ptr - multiple owners
   std::shared_ptr<T> ptr = std::make_shared<T>();
   ```

2. **Stack allocation works**
   ```cpp
   // ❌ Unnecessary heap allocation
   auto ptr = std::make_unique<Widget>();
   
   // ✅ Stack is faster
   Widget widget;
   ```

3. **Need to observe only**
   ```cpp
   // ❌ unique_ptr when not owning
   void observe(std::unique_ptr<T>& ptr);
   
   // ✅ Raw pointer for observation
   void observe(T* ptr);
   ```

4. **Standard containers work**
   ```cpp
   // ❌ unique_ptr<T[]>
   auto arr = std::make_unique<int[]>(10);
   
   // ✅ vector is better
   std::vector<int> arr(10);
   ```

---

## Related Concepts

### RAII (Resource Acquisition Is Initialization)

```cpp
class FileHandler {
    std::unique_ptr<FILE, FileDeleter> file;
    
public:
    FileHandler(const char* name) 
        : file(fopen(name, "r"), FileDeleter{}) {
        if (!file) throw std::runtime_error("Failed to open");
    }
    
    // Destructor automatically closes file
    // Exception safe!
};
```

### Move Semantics

```cpp
std::unique_ptr<T> ptr1 = std::make_unique<T>();

// Move constructor
std::unique_ptr<T> ptr2(std::move(ptr1));

// Move assignment
std::unique_ptr<T> ptr3;
ptr3 = std::move(ptr2);
```

### Custom Deleters

```cpp
// Function pointer deleter
void cleanup(Resource* r) { /* ... */ }
std::unique_ptr<Resource, decltype(&cleanup)> ptr(new Resource, &cleanup);

// Lambda deleter
auto deleter = [](Resource* r) { /* ... */ };
std::unique_ptr<Resource, decltype(deleter)> ptr(new Resource, deleter);

// Functor deleter
struct Deleter {
    void operator()(Resource* r) const { /* ... */ }
};
std::unique_ptr<Resource, Deleter> ptr(new Resource, Deleter{});
```

### Polymorphism

```cpp
class Base {
public:
    virtual ~Base() = default;  // ⚠️ Must be virtual!
    virtual void work() = 0;
};

class Derived : public Base {
    void work() override { /* ... */ }
};

std::unique_ptr<Base> ptr = std::make_unique<Derived>();
ptr->work();  // Calls Derived::work()
// Properly deletes Derived object
```

### Pimpl Idiom

```cpp
// widget.h
class Widget {
public:
    Widget();
    ~Widget();
    void doWork();
    
private:
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};

// widget.cpp
struct Widget::Impl {
    // Implementation details
};

Widget::Widget() : pImpl(std::make_unique<Impl>()) {}
Widget::~Widget() = default;  // unique_ptr handles it
```

---

## Visual Diagrams

### Ownership Transfer

```
Initial state:
┌──────┐
│ ptr1 │───────> [Widget object]
└──────┘
┌──────┐
│ ptr2 │───────> nullptr
└──────┘

After: auto ptr2 = std::move(ptr1);
┌──────┐
│ ptr1 │───────> nullptr
└──────┘
┌──────┐
│ ptr2 │───────> [Widget object]
└──────┘
```

### Scope and Lifetime

```cpp
void function() {
    auto ptr = std::make_unique<Widget>();  // Created
    
    ptr->doWork();  // Used
    
    if (condition) {
        return;  // ← Widget deleted here
    }
    
    // More work...
    
}  // ← Or Widget deleted here
```

### Polymorphism

```
┌────────────────────────┐
│ vector<unique_ptr<     │
│        Animal>>        │
└────────────────────────┘
         │
         ├──> unique_ptr───> [Dog object]
         │
         ├──> unique_ptr───> [Cat object]
         │
         └──> unique_ptr───> [Bird object]

All properly deleted when vector destroyed
```

---

## Quick Reference

### Creation

```cpp
auto ptr = std::make_unique<T>(args);        // Preferred
std::unique_ptr<T> ptr(new T(args));         // OK
auto arr = std::make_unique<T[]>(size);      // Arrays
```

### Access

```cpp
ptr->member()     // Access member
*ptr              // Dereference
ptr.get()         // Raw pointer
ptr[i]            // For arrays
```

### Ownership

```cpp
auto p2 = std::move(ptr);    // Transfer
ptr.reset()                  // Delete
ptr.reset(new T())           // Replace
T* raw = ptr.release()       // Give up
```

### Check

```cpp
if (ptr) { }      // Not null
if (!ptr) { }     // Is null
```

### Custom Deleter

```cpp
std::unique_ptr<T, Deleter> ptr(new T, deleter);
```

---

## Compilation

### Basic

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o unique_ptr
./unique_ptr
```
`main.cpp` uses `std::println` (`<print>`), so it needs GCC 14+ or a recent Clang with `-std=c++23`.
Example 6 creates a file `test.txt` in the current directory.

### With Optimizations

```bash
g++ -std=c++23 -O3 -Wall -Wextra main.cpp -o unique_ptr
```

### Check Assembly

```bash
g++ -std=c++23 -O2 -S main.cpp
# Compare unique_ptr code with raw-pointer code yourself.
# Not identical: the unique_ptr version also contains exception-cleanup code.
```

---

## Summary

### Key Takeaways

1. **unique_ptr** = automatic memory management
2. **Unique ownership** - one owner at a time
3. **Use make_unique** - cleaner, no naked `new`
4. **Move with std::move** - can't copy
5. **Zero overhead** - same as raw pointers
6. **Exception safe** - RAII pattern
7. **Perfect for polymorphism** - virtual destructor
8. **Return by value** - ownership transfer
9. **Never raw new/delete** - use unique_ptr instead
10. **Default choice** - for all heap allocations

### The Golden Rules

```
⚠️  RULE 1: Use make_unique
   auto ptr = make_unique<T>(args);

⚠️  RULE 2: Move, don't copy
   auto p2 = std::move(p1);

⚠️  RULE 3: Check before use
   if (ptr) ptr->work();

⚠️  RULE 4: Prefer vector over unique_ptr<T[]>
   vector<T> v(size);

⚠️  RULE 5: Return by value
   unique_ptr<T> create() { return make_unique<T>(); }
   
   unique_ptr = AUTOMATIC MEMORY MANAGEMENT!
```

---

## 🧾 Notes moved from main.cpp

The title and description comment at the top of `main.cpp` stays. What changed:

- `std::cout` → `std::println` / `std::print`; `rand()` / `runtime_error` etc. now have their includes (`<cstdlib>`, `<stdexcept>`, `<cstdio>`).
- **Printed lecture text removed from the program** — everything in it is covered by the sections above:
  - Example 1: the list of raw-pointer problems → "The Problem It Solves" and Examples Overview, Example 1.
  - Example 2: the "Advantages" list → "What Is unique_ptr?" / Key Properties.
  - Example 3: the "Why prefer make_unique?" list and the exception-safety example → Example 3 and "Creating unique_ptr" (corrected for C++17, see above).
  - Examples 4, 8, 10: the closing "✅ …" lines → Ownership and Moving, Example 8, Best Practices.
  - Example 5: the reset()/release() comparison → Example 5 and "Common Operations".
  - Example 7: the "Important" list and the "Don't do this" block → Common Mistakes, Mistake 2.
  - `printComprehensiveInfo()` (quick reference, special cases, patterns, mistakes, best practices, performance, "C++ evolution") and the "KEY TAKEAWAYS" block → the Quick Reference, Best Practices, Common Mistakes, Performance and Summary sections of this Readme.
- The "C++17: Better deduction" line from the old "C++ evolution" block was dropped (not accurate enough to keep). From the same block: C++11 introduced `unique_ptr`, C++14 added `make_unique`, C++20 added `make_unique_for_overwrite`.
- Example 1 now prints one extra line, `(no 'destroyed' line above — the Widget leaked)`, because the leak is otherwise invisible once the list of problems is gone.
- Example 6: the heading said "lambda" for the second block, but it uses a function pointer; it now says "function pointer".

### Corrections to comments in main.cpp

- Example 8: "RVO/NRVO applies - no copy, no move!" was too strong. Returning a prvalue (`return make_unique<...>()`) is guaranteed elision; returning a named local (`return widget;`) usually gets NRVO, but if not, it is **moved** — never copied.
- Example 9: "destroyed in reverse order" was wrong for a `vector`: elements are destroyed in forward order (✅ tested: Dog, Cat, Bird; each prints `~Derived` then `~Animal`).
- Example 10: "logger destroyed after db" was wrong: members are destroyed in reverse declaration order, so `logger` is destroyed first, then `db` (✅ tested: `[LOG] Closing app.log` is printed before any DB line). `db` prints nothing on destruction here because `shutdown()` already closed it.

### Tested

✅ `-Wall -Wextra`, AddressSanitizer and UndefinedBehaviorSanitizer: no warnings, no errors. Output identical to the original except the removed lecture text, the two changed/added lines above, and pointer addresses.
Example 1 leaks one `Widget` on purpose, so LeakSanitizer (`detect_leaks=1`) reports it at exit.
`unique_ptr<int> arr(new int[10])` is reported by AddressSanitizer as `alloc-dealloc-mismatch` ✅ tested (Mistake 2).
`return create ? make_unique<T>() : nullptr;` compiles ✅ tested (the "Optional ownership" example).

---

**Previous Topic:** [36_SFINAE](../36_SFINAE/) - SFINAE Technique

**Next Topic:** [38_shared_ptr](../38_shared_ptr/) - Shared Ownership

---

*Part of the Smart Pointers and Memory Management series*  
*Difficulty: Intermediate*  
*Prerequisites: Pointers, RAII, Move Semantics*  
*Essential for Modern C++ Memory Management*