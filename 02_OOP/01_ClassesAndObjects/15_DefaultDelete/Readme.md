# 22_DefaultDelete - Special Member Functions Control

## 📚 Overview

This folder introduces the **= default** and **= delete** keywords - powerful C++11 features that give you explicit control over special member functions. These keywords allow you to explicitly request compiler-generated implementations or prevent function usage entirely, enabling better type safety, and clearer design intent.

**= default** = Request compiler-generated implementation  
**= delete** = Explicitly prevent function usage

These are **ESSENTIAL** for modern C++ resource management and API design.

---

## 🎯 What You'll Learn

- What = default and = delete are and why they exist
- The six special member functions
- Rule of Zero, Rule of Three, and Rule of Five
- Creating non-copyable and move-only types
- Preventing implicit type conversions
- Implementing the Singleton pattern
- Trivial types and their benefits
- Best practices for resource management
- Real-world design patterns

---

## 📖 Concept Explanation

### What are = default and = delete?

**= default:**
- Explicitly requests the compiler to generate the default implementation
- Used for special member functions
- Makes your intent clear
- Can restore functions that were suppressed
- Keeps types trivial where possible (see *Trivial Types*)

**= delete:**
- Explicitly deletes a function
- Prevents the function from being called
- Works for any function (not just special members)
- Produces clear compile-time errors
- Replaces the old "private + undefined" idiom

```cpp
class Example {
public:
    Example() = default;              // Request default constructor
    Example(const Example&) = delete; // Prevent copying
    
    void setValue(int);               // OK
    void setValue(double) = delete;   // Prevent implicit conversion
};
```

### The Six Special Member Functions

C++ can automatically generate up to six special member functions:

```cpp
class MyClass {
public:
    // 1. Default Constructor
    MyClass();
    
    // 2. Destructor
    ~MyClass();
    
    // 3. Copy Constructor
    MyClass(const MyClass&);
    
    // 4. Copy Assignment Operator
    MyClass& operator=(const MyClass&);
    
    // 5. Move Constructor (C++11)
    MyClass(MyClass&&);
    
    // 6. Move Assignment Operator (C++11)
    MyClass& operator=(MyClass&&);
};
```

### Rule of Zero

**The best rule: If you can avoid defining special member functions, DO SO!**

```cpp
// ✅ GOOD: Rule of Zero
class RuleOfZero {
private:
    string name;           // RAII type
    vector<int> data;      // RAII type
    unique_ptr<int> ptr;   // RAII type
    
public:
    // No special members needed!
    // Compiler generates everything correctly
};
```

**Why it works:**
- `string`, `vector`, and `unique_ptr` manage their own resources
- No raw pointers or manual memory management
- Compiler-generated functions do the right thing
- Less code, fewer bugs

### Rule of Three (Pre-C++11)

If you define one of these three, define all three:
1. Destructor
2. Copy constructor
3. Copy assignment operator

```cpp
class RuleOfThree {
    int* data;
public:
    ~RuleOfThree() { delete data; }
    RuleOfThree(const RuleOfThree& other) {
        data = new int(*other.data);
    }
    RuleOfThree& operator=(const RuleOfThree& other) {
        if (this != &other) {
            delete data;
            data = new int(*other.data);
        }
        return *this;
    }
};
```

### Rule of Five (C++11 and later)

If you define one of these five, consider all five:
1. Destructor
2. Copy constructor
3. Copy assignment operator
4. Move constructor
5. Move assignment operator

```cpp
class RuleOfFive {
    int* data;
public:
    ~RuleOfFive() { delete data; }
    
    RuleOfFive(const RuleOfFive& other) {
        data = new int(*other.data);
    }
    
    RuleOfFive& operator=(const RuleOfFive& other) {
        if (this != &other) {
            delete data;
            data = new int(*other.data);
        }
        return *this;
    }
    
    RuleOfFive(RuleOfFive&& other) noexcept 
        : data(other.data) {
        other.data = nullptr;
    }
    
    RuleOfFive& operator=(RuleOfFive&& other) noexcept {
        if (this != &other) {
            delete data;
            data = other.data;
            other.data = nullptr;
        }
        return *this;
    }
};
```

**Or use = default / = delete:**

```cpp
class RuleOfFiveEasy {
private:
    unique_ptr<int> data;  // Use RAII
public:
    ~RuleOfFiveEasy() = default;
    RuleOfFiveEasy(const RuleOfFiveEasy&) = delete;   // a unique_ptr member cannot be copied
    RuleOfFiveEasy& operator=(const RuleOfFiveEasy&) = delete;
    RuleOfFiveEasy(RuleOfFiveEasy&&) = default;
    RuleOfFiveEasy& operator=(RuleOfFiveEasy&&) = default;
};
```

> `= default` on a copy operation does not make an uncopyable member copyable: with a `unique_ptr` member the defaulted copy is silently defined as deleted, and you only get an error when you use it (GCC accepts the declaration without a warning; clang warns with `-Wdefaulted-function-deleted`). Writing `= delete` says what is really happening. ✅ tested

### Real-World Analogy

```
Think of car manufacturing:

= DEFAULT = Use Standard Parts
  - "Use the standard engine"
  - Manufacturer knows best
  - Optimized and tested
  - Consistent quality
  - Less work for you

= DELETE = Remove Features
  - "This model has no sunroof"
  - Explicitly prevent option
  - Clear to buyers
  - No accidental requests
  - Design constraint enforced

RULE OF ZERO = Buy Off-the-Shelf
  - Everything standard
  - No custom parts needed
  - Reliable and efficient
  - Less maintenance

RULE OF FIVE = Custom Build
  - If you customize engine,
    also customize transmission,
    brakes, suspension, etc.
  - All parts must work together
```

---

## 💻 Basic Syntax

### Using = default

```cpp
class MyClass {
public:
    // Request default constructor
    MyClass() = default;
    
    // Request default destructor
    ~MyClass() = default;
    
    // Request default copy operations
    MyClass(const MyClass&) = default;
    MyClass& operator=(const MyClass&) = default;
    
    // Request default move operations
    MyClass(MyClass&&) = default;
    MyClass& operator=(MyClass&&) = default;
};
```

### Using = delete

```cpp
class MyClass {
public:
    // Delete copy operations (non-copyable)
    MyClass(const MyClass&) = delete;
    MyClass& operator=(const MyClass&) = delete;
    
    // Delete specific overload
    void setValue(int);           // OK
    void setValue(double) = delete;  // Prevent double
};
```

### Restoring Default Constructor

```cpp
class MyClass {
public:
    MyClass(int);         // Custom constructor
    MyClass() = default;  // Restore default constructor
};
```

---

## 📝 Common Use Cases

### 1. Non-Copyable Class

```cpp
class NonCopyable {
public:
    NonCopyable() = default;
    
    // Delete copy operations
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};

NonCopyable obj1;
// NonCopyable obj2 = obj1;  // ❌ Error!
```

**Use cases:**
- Unique resources (file handles, network connections)
- Singleton pattern
- Manager classes
- Hardware interfaces

### 2. Move-Only Class

```cpp
class MoveOnly {
public:
    MoveOnly() = default;
    
    // Delete copy operations
    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;
    
    // Allow move operations
    MoveOnly(MoveOnly&&) = default;
    MoveOnly& operator=(MoveOnly&&) = default;
};

MoveOnly obj1;
// MoveOnly obj2 = obj1;       // ❌ Error - can't copy
MoveOnly obj3 = move(obj1);    // ✅ OK - can move
```

**Similar to:**
- `unique_ptr`
- `thread`
- `ifstream`
- `ofstream`

### 3. Singleton Pattern

```cpp
class Singleton {
private:
    Singleton() = default;
    
public:
    // Delete all copy and move operations
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;
    
    // Meyers singleton: created on first call, destroyed at program exit.
    // Since C++11 the initialization of a function-local static is thread-safe.
    static Singleton& getInstance() {
        static Singleton instance;
        return instance;
    }
};
```

> The old version (`static Singleton* instance` + `new`) is never deleted, so its destructor never runs, and its lazy `if (!instance)` check is a data race when two threads call it at once (TSan reported it; the function-local static version was clean). ✅ tested

### 4. Preventing Implicit Conversions

```cpp
class SafeInteger {
public:
    void setValue(int value);           // OK
    void setValue(double) = delete;     // Prevent double
    void setValue(void*) = delete;      // Prevent pointer
};

SafeInteger num;
num.setValue(42);      // ✅ OK
// num.setValue(3.14);    // ❌ Error - double deleted
// num.setValue(nullptr); // ❌ Error - pointer deleted
// num.setValue(5L);      // ❌ Error - but only as 'ambiguous' (long -> int or double?)
```

Deleted overloads take part in overload resolution, so `long` is equally good for `int` and `double` and the call is ambiguous (still an error, just a vaguer message). To block *every* other type, delete a template:

```cpp
void setValue(int value);
template <class T> void setValue(T) = delete;   // blocks long, double, char, nullptr_t, ...
```
✅ tested

### 5. Interface Base Class

```cpp
class Interface {
public:
    virtual ~Interface() = default;  // Virtual destructor
    virtual void doWork() = 0;       // Pure virtual
    
protected:
    Interface() = default;  // Protected constructor
    Interface(const Interface&) = default;
    Interface& operator=(const Interface&) = default;
};
```

---

## 📋 Examples in This Folder

### Example 1: Default Constructor
Shows explicit vs implicit default constructors.
- Makes intent explicit and documents the code
- Restores the default constructor after another constructor was declared
- Members get default member initializers (`int value = 0;`); without them a defaulted constructor leaves an `int` indeterminate

### Example 2: Deleting Copy Operations
Non-copyable class with deleted copy operations.
- Prevents accidental copies; unique ownership (like `unique_ptr`); clear compiler errors
- The hand-written move operations must nullify the source

### Example 3: Singleton Pattern
Implementing singleton with deleted operations.
- Only one instance, no copy or move, one global access point, controlled instantiation
- Implemented as a Meyers singleton (function-local static)

### Example 4: Rule of Five - All Defaults
Using = default for all five operations.
- Rule of Zero: use RAII types (`string`, `vector`), no raw pointers, let the compiler generate everything
- Spelling all five as `= default` is explicit, but it is the Rule of Five with defaults, not the Rule of Zero

### Example 5: Rule of Five - Custom
Manual implementation of all five operations.
- If you manage a resource, define all five (or use `= default` / `= delete`)
- Destructor cleans up, copy makes a deep copy, move transfers ownership

### Example 6: Deleting Specific Overloads
Type safety by deleting specific function overloads.
- Prevents implicit conversions; errors at compile time instead of runtime checks
- The constructor `SafeInteger(double)` is deleted as well, otherwise `SafeInteger(3.14)` would silently truncate

### Example 7: Move-Only Type
Creating a move-only resource class.
- Unique ownership, no accidental copies, efficient transfers, clear semantics
- `createResource()` returns by value: since C++17 the object is built in place (guaranteed elision), no move happens

### Example 8: Default on Declaration
Separating declaration and definition.
- Declare in the header, define `= default` in the source file
- Separates interface from implementation; useful for the pimpl idiom

### Example 9: When NOT to Use
Anti-patterns and common mistakes.
- Don't use `= default` when the function would be generated anyway (noise), and don't over-document
- The demo uses a `Probe` member that prints when copied or moved to show that `~X() = default` plus defaulted copy operations turns a move into a copy

### Example 10: Real-World - File Handle
RAII file handle with proper resource management.
- Unique resource ownership, no accidental copies, safe transfer of ownership; similar to `unique_ptr`
- The demo creates `test.txt` first, so the open / move / close path really runs, and removes it at the end

---

## 🎓 Key Concepts

### When to Use = default

**✅ USE = DEFAULT:**

1. **Restoring suppressed functions** (declaring any constructor suppresses the implicit default constructor):
```cpp
class A {
    A(int);           // Custom constructor
    A() = default;    // Restore default
};
```

2. **Making intent explicit:**
```cpp
class B {
    ~B() = default;   // Explicitly defaulted
};
```

3. **Out-of-line definition:**
```cpp
class C {
    C();  // Declare
};
C::C() = default;  // Define outside
```

4. **Rule of Five (with RAII types):**
```cpp
class D {
    string name;           // copyable AND movable member
public:
    ~D() = default;
    D(const D&) = default;
    D& operator=(const D&) = default;
    D(D&&) = default;
    D& operator=(D&&) = default;
};
// With a unique_ptr member, the defaulted copy would be implicitly deleted - write = delete instead.
```

### When to Use = delete

**✅ USE = DELETE:**

1. **Prevent copying:**
```cpp
ClassName(const ClassName&) = delete;
ClassName& operator=(const ClassName&) = delete;
```

2. **Prevent specific conversions:**
```cpp
void func(int);           // OK
void func(double) = delete;  // Prevent
```

3. **Singleton/unique objects:**
```cpp
// Delete all copy and move operations
```

4. **Type safety:**
```cpp
void process(const string&);  // OK
void process(char*) = delete; // Force string
```

### Trivial Types

A type is **trivial** if:
- Has trivial default constructor (= default or none)
- Has trivial copy/move operations
- Has trivial destructor
- No virtual functions
- All base classes and members are trivial

**Why it matters:**
- Can be used in unions (without custom special members)
- Can be created without running a constructor (e.g. in raw storage)
- POD-like behavior

`memcpy` safety is about **trivially copyable** (trivial copy/move/destructor), not about a trivial default constructor. ✅ tested: both `Trivial` and `NonTrivial` below are trivially copyable.

```cpp
// Trivial type
class Trivial {
    int x, y;
public:
    Trivial() = default;  // Trivial!
};

// Non-trivial type
class NonTrivial {
    int x, y;
public:
    NonTrivial() : x(0), y(0) {}  // Not trivial
};

class OutOfLine {
    int x, y;
public:
    OutOfLine();
};
OutOfLine::OutOfLine() = default;  // Also NOT trivial: defaulting outside the class makes it user-provided
```

✅ tested (`is_trivially_default_constructible`): in-class `= default` → true; manual `: x(0)` → false; out-of-line `= default` → false.

Also note: since C++20 a class with a user-*declared* constructor (even `= default`) is no longer an aggregate, so `Agg a{1};` compiles in C++17 but not in C++20. ✅ tested

### Compiler-Generated vs Manual

**Compiler-Generated (= default):**
- Produces trivial types when possible
- Leaves members uninitialized (`int x;` stays indeterminate) - the manual `: x(0)` version does not
- Less code to maintain
- Consistent behavior

**Manual Implementation:**
- Full control over initialization
- Can add custom logic
- Required for complex resource management
- More verbose

**Rule of Thumb:** Use = default unless you need custom behavior. Better: give members default member initializers (`int x = 0;`) so even a defaulted constructor starts from a known value.

---

## ⚠️ Common Mistakes

### ❌ Mistake 1: Unnecessary = default

**Bad:**
```cpp
class Bad {
public:
    Bad() = default;  // ❌ Would be generated anyway!
    ~Bad() = default;
    Bad(const Bad&) = default;
    Bad& operator=(const Bad&) = default;
};
```

**Why it's bad:** Over-documenting, adds noise - and worse, declaring the destructor or the copy operations (even as `= default`) **suppresses the implicit move operations**, so every "move" silently becomes a copy. With a member that prints when copied/moved: the plain class printed "Probe moved", the class with `~X() = default` + defaulted copy printed "Probe copied". ✅ tested (see Example 9)

**Fix:**
```cpp
class Good {
    // Nothing - compiler generates all
};
```

**When = default IS needed:**
```cpp
class Needed {
    Needed(int);      // Custom constructor
    Needed() = default;  // ✅ Needed to restore default
};
```

### ❌ Mistake 2: Partial Rule of Five

**Bad:**
```cpp
class Bad {
    int* data;
public:
    ~Bad() { delete data; }
    // ❌ Missing copy/move operations!
    // Compiler-generated ones are wrong!
};
```

**Why it's bad:** Double-delete, memory leaks, undefined behavior

**Fix:**
```cpp
class Good {
    int* data;
public:
    ~Good() { delete data; }
    
    // Define all five
    Good(const Good&);
    Good& operator=(const Good&);
    Good(Good&&) noexcept;
    Good& operator=(Good&&) noexcept;
};

// Or better - use unique_ptr!
class Better {
    unique_ptr<int> data;
    // Nothing needed - Rule of Zero!
};
```

### ❌ Mistake 3: Deleting Destructor

**Bad:**
```cpp
class Bad {
public:
    ~Bad() = delete;  // ❌ Can't destroy objects!
};

// Bad obj;              // ❌ Error: use of deleted function 'Bad::~Bad()'
// Bad* p = new Bad;     // ✅ compiles - creating is fine...
// delete p;             // ❌ Error - ...destroying is not
```

**Why it's bad:** Such an object can never be destroyed (this is a compile error, not a leak). ✅ tested

**Fix:** Don't delete destructors. The rare legitimate use is to forbid objects that can be destroyed normally, e.g. to prevent stack objects (not tested here).

### ❌ Mistake 4: Forgetting noexcept on Move

**Bad:**
```cpp
class Bad {
public:
    Bad(Bad&&) = default;  // Missing noexcept
};
```

**Correction:** this is *not* a problem for `= default`. A defaulted move constructor is automatically `noexcept` when all members' moves are (`is_nothrow_move_constructible` was true with and without the keyword). ✅ tested

It matters for **user-written** moves: if you write `Bad(Bad&&) { ... }` without `noexcept`, `vector` reallocation copies the elements instead of moving them. ✅ tested

```cpp
class Good {
public:
    Good(Good&&) noexcept { /* hand-written move */ }
    Good& operator=(Good&&) = default;   // already noexcept if the members' moves are
};
```
(`noexcept = default` is allowed and harmless; GCC 13 even accepts it when a member's move may throw.)

### ❌ Mistake 5: Using Private Instead of Delete

**Old Way (C++03):**
```cpp
class Old {
private:
    Old(const Old&);  // Declared but not defined
    // ❌ Not clear intent
    // Outside the class: compile error ('is private within this context')
    // Only inside members/friends does it become a link error ('undefined reference')
};
```

**Modern Way (C++11+):**
```cpp
class Modern {
public:
    Modern(const Modern&) = delete;
    // ✅ Clear compile error
    // ✅ Explicit intent
};
```

---

## 🎯 Practice Exercises

### Exercise 1: Create Non-Copyable Class
Implement a `Logger` class that can't be copied but can be moved.

### Exercise 2: Implement Rule of Five
Create a class managing a dynamic array with all five operations.

### Exercise 3: Type-Safe Function
Create a function that accepts `int` but prevents implicit conversion from `double`.

### Exercise 4: Find the Bugs
Given code with improper special member functions, identify and fix issues.

### Exercise 5: Refactor to Rule of Zero
Take a class with manual resource management and refactor to use RAII types.

---

## 🚀 How to Run

### Compile:
```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o default_delete
```

(`<print>` / `std::println` needs GCC 14 or newer.)

### Run:
```bash
./default_delete
```

### Expected Output:
The program demonstrates:
1. Default constructor usage
2. Deleting copy operations
3. Singleton pattern
4. Rule of Five with defaults
5. Rule of Five custom implementation
6. Deleting specific overloads
7. Move-only types
8. Out-of-line defaults
9. When NOT to use
10. Real-world file handle example

---

## 📊 Visual Concepts

### Special Member Function Generation

```
CLASS DEFINITION                  COMPILER GENERATES
┌─────────────────────┐          ┌──────────────────────┐
│ class Simple {      │          │ Default Constructor  │
│   int x;            │   ────>  │ Destructor          │
│ };                  │          │ Copy Constructor    │
│                     │          │ Copy Assignment     │
│ (No special members)│          │ Move Constructor    │
└─────────────────────┘          │ Move Assignment     │
                                 └──────────────────────┘

CLASS WITH CUSTOM                COMPILER SUPPRESSES
┌─────────────────────┐          ┌──────────────────────┐
│ class Custom {      │          │ ❌ Default Constructor│
│   Custom(int) {}    │   ────>  │ ✅ Destructor        │
│ };                  │          │ ✅ Copy Constructor  │
│                     │          │ ✅ Copy Assignment   │
│ (Custom constructor)│          │ ✅ Move Constructor  │
└─────────────────────┘          │ ✅ Move Assignment   │
                                 └──────────────────────┘

RESTORE WITH = default
┌─────────────────────┐          ┌──────────────────────┐
│ class Restored {    │          │ ✅ Default Constructor│
│   Restored(int) {}  │   ────>  │ ✅ Destructor        │
│   Restored()=default│          │ ✅ Copy Constructor  │
│ };                  │          │ ✅ Copy Assignment   │
└─────────────────────┘          └──────────────────────┘
```

### Rule of Zero vs Rule of Five

```
RULE OF ZERO                    RULE OF FIVE
┌────────────────────┐          ┌────────────────────┐
│ class RuleOfZero { │          │ class RuleOfFive { │
│   string name;     │          │   int* data;       │
│   vector<int> data;│          │                    │
│   unique_ptr<X> p; │          │   ~RuleOfFive();   │
│ };                 │          │   Copy ctor        │
│                    │          │   Copy assign      │
│ No special members!│          │   Move ctor        │
│ Compiler does all  │          │   Move assign      │
└────────────────────┘          └────────────────────┘
        ✅                               ⚠️
   Preferred way!              Only when necessary
```

### Move-Only Type Pattern

```
MOVE-ONLY TYPE
┌──────────────────────────────────┐
│ class MoveOnly {                 │
│ public:                          │
│   // ❌ Delete copy              │
│   MoveOnly(const MoveOnly&)      │
│     = delete;                    │
│   MoveOnly& operator=            │
│     (const MoveOnly&) = delete;  │
│                                  │
│   // ✅ Allow move               │
│   MoveOnly(MoveOnly&&)           │
│     = default;                   │
│   MoveOnly& operator=            │
│     (MoveOnly&&) = default;      │
│ };                               │
└──────────────────────────────────┘

USAGE:
MoveOnly obj1;
MoveOnly obj2 = obj1;        // ❌ Error!
MoveOnly obj3 = move(obj1);  // ✅ OK!
```

---

## 📚 Related Topics

### Prerequisites:
- **06_GettersSetters** - Member functions
- **19_ConstMemberFunctions** - Const correctness
- **21_MutableKeyword** - Logical constness

### Coming Next:
- **23_ExplicitKeyword** - Preventing implicit conversions
- **24_DelegatingConstructors** - Constructor delegation
- **25_CopyConstructor** - Deep dive into copying

### Related Concepts:
- **RAII** - Resource Acquisition Is Initialization
- **Move Semantics** - Efficient resource transfer
- **Smart Pointers** - Automatic memory management

---

## 🎓 Summary

### What We Learned:

✅ **= default** requests compiler-generated implementation  
✅ **= delete** prevents function usage  
✅ **Rule of Zero** - prefer RAII types  
✅ **Rule of Five** - if you define one, consider all five  
✅ **Non-copyable** types with deleted copy operations  
✅ **Move-only** types for unique ownership  
✅ **Type safety** by deleting specific overloads  
✅ **Trivial types** stay memcpy-friendly  
✅ **Explicit intent** makes code clearer  
✅ **Better errors** than old-style private + undefined

### The Complete Pattern:

```cpp
// RULE OF ZERO (Preferred)
class RuleOfZero {
private:
    string name;
    vector<int> data;
    unique_ptr<Resource> resource;
public:
    // Nothing needed - compiler does everything!
};

// MOVE-ONLY TYPE
class MoveOnly {
public:
    MoveOnly() = default;
    
    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;
    
    MoveOnly(MoveOnly&&) = default;
    MoveOnly& operator=(MoveOnly&&) = default;
};

// RULE OF FIVE (When necessary)
class RuleOfFive {
private:
    int* data;
    size_t size;
public:
    ~RuleOfFive() { delete[] data; }
    
    RuleOfFive(const RuleOfFive& other)
        : data(new int[other.size]), size(other.size) {   // same order as the declarations
        memcpy(data, other.data, size * sizeof(int));
    }
    
    RuleOfFive& operator=(const RuleOfFive& other) {
        if (this != &other) {
            int* newData = new int[other.size];   // allocate first, so a throw leaves *this intact
            memcpy(newData, other.data, other.size * sizeof(int));
            delete[] data;
            data = newData;
            size = other.size;
        }
        return *this;
    }
    
    RuleOfFive(RuleOfFive&& other) noexcept
        : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
    }
    
    RuleOfFive& operator=(RuleOfFive&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
        }
        return *this;
    }
};
```

---

## ✨ Quick Reference Card

```cpp
// === REQUESTING DEFAULT ===
class A {
public:
    A() = default;                    // Default constructor
    ~A() = default;                   // Destructor
    A(const A&) = default;            // Copy constructor
    A& operator=(const A&) = default; // Copy assignment
    A(A&&) = default;                 // Move constructor
    A& operator=(A&&) = default;      // Move assignment
};

// === DELETING FUNCTIONS ===
class B {
public:
    B(const B&) = delete;             // Delete copy
    B& operator=(const B&) = delete;  // Delete copy assign
    
    void func(int);                   // OK
    void func(double) = delete;       // Delete overload
};

// === NON-COPYABLE ===
class NonCopyable {
public:
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};

// === MOVE-ONLY ===
class MoveOnly {
public:
    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;
    MoveOnly(MoveOnly&&) = default;
    MoveOnly& operator=(MoveOnly&&) = default;
};

// === RESTORING DEFAULT ===
class Restore {
public:
    Restore(int);          // Custom constructor
    Restore() = default;   // Restore default
};
```

---

## 📝 Best Practices

### ✅ DO:

- **Prefer Rule of Zero** - use RAII types
- **Use unique_ptr** instead of raw pointers
- **Use = default** when restoring suppressed functions
- **Use = delete** to prevent operations
- **Define all five** if you define one
- **Make hand-written move operations** noexcept
- **Use = delete** for type safety
- **Keep it simple** - avoid manual resource management
- **Document intent** - explicit is better
- **Test thoroughly** - verify copy/move behavior

### ❌ DON'T:

- **Don't overuse** = default unnecessarily
- **Don't forget** the Rule of Five
- **Don't delete** destructors (except special cases)
- **Don't mix** RAII and manual management
- **Don't forget** noexcept on hand-written move operations (defaulted ones get it automatically)
- **Don't use** private + undefined (old style)
- **Don't manage** raw resources without need
- **Don't define** special members when not needed
- **Don't forget** to handle self-assignment
- **Don't make** everything non-copyable

---

## 🌟 Real-World Applications

### Unique Resource Ownership
```cpp
class DatabaseConnection {
    unique_ptr<Handle> handle;   // the reason this class is move-only
public:
    DatabaseConnection(const DatabaseConnection&) = delete;
    DatabaseConnection& operator=(const DatabaseConnection&) = delete;
    
    DatabaseConnection(DatabaseConnection&&) = default;
    DatabaseConnection& operator=(DatabaseConnection&&) = default;
};
```

### Configuration Singleton
```cpp
class Config {
private:
    Config() = default;
public:
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;
    
    static Config& instance() {
        static Config cfg;
        return cfg;
    }
};
```

### Type-Safe API
```cpp
class SafeAPI {
public:
    void process(const string& data);     // OK
    void process(char*) = delete;         // Force string
    void process(const char*) = delete;   // Force string
};
// api.process(string("x"));  // ✅ OK
// api.process("x");          // ❌ Error - a string literal is a const char[], so it hits the deleted overload too
```
✅ tested

---

## 📖 Further Reading

- [cppreference.com - default specifier](https://en.cppreference.com/cpp/language/function)
- [cppreference.com - delete specifier](https://en.cppreference.com/cpp/language/function)
- "Effective Modern C++" by Scott Meyers - Items 11, 17
- [C++ Core Guidelines - C.20 (Rule of Zero)](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rc-zero)
- [C++ Core Guidelines - C.21 (Rule of Five)](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rc-five)
- [Tutorial: When to Write Which Special Member (foonathan)](https://foonathan.net/2019/02/special-member-functions/)
- [cppreference.com - Rule of three/five/zero](https://en.cppreference.com/cpp/language/rule_of_three)

---

## 🎯 Key Takeaways

1. **= default** requests compiler implementation
2. **= delete** prevents function usage
3. **Rule of Zero** is preferred - use RAII
4. **Rule of Five** - if you define one, define all
5. **Non-copyable** - delete copy operations
6. **Move-only** - delete copy, default move
7. **Type safety** - delete unwanted overloads
8. **Explicit intent** - clearer than implicit
9. **Better errors** - compile-time vs link-time
10. **Modern C++** - always prefer over old idioms

---

## 🚨 THE GOLDEN RULES

```
⚠️  RULE 1: Prefer Rule of Zero
   Use string, vector, unique_ptr instead of raw pointers
   
   ✅ string name;              // Good
   ❌ char* name;               // Bad

⚠️  RULE 2: If you define one, define all five
   Destructor + copy + move operations
   
   class A {
       ~A();                    // If you define this...
       A(const A&);             // Define these too!
       A& operator=(const A&);
       A(A&&) noexcept;
       A& operator=(A&&) noexcept;
   };

⚠️  RULE 3: Use = delete for non-copyable
   Clear intent, better errors
   
   A(const A&) = delete;        // ✅ Clear
   private: A(const A&);        // ❌ Old style

⚠️  RULE 4: Hand-written move operations should be noexcept
   vector reallocation copies otherwise
   
   A(A&&) noexcept { ... }      // ✅ Good
   A(A&&) { ... }               // ⚠️  Missing noexcept
   A(A&&) = default;            // ✅ Already noexcept if the members' moves are

⚠️  RULE 5: Don't overthink it
   If you don't need special handling, don't define anything!
   
   FOLLOW THE RULE OF ZERO WHEN POSSIBLE!
```

---

**Previous Topic:** [21_MutableKeyword](../21_MutableKeyword/) - Mutable Keyword  
**Next Topic:** [23_ExplicitKeyword](../23_ExplicitKeyword/) - Explicit Keyword

---

*Part of the C++ Classes and Objects series*  
*Difficulty: Intermediate to Advanced*  
*Prerequisites: Constructors, destructors, pointers*  
*ESSENTIAL for Resource Management*
---

## 🧾 Notes moved from main.cpp

- **Default vs manual implementation:** `class Default { int x, y; Default() = default; }` leaves `x` and `y` uninitialized; `Manual() : x(0), y(0) {}` initializes them. So the difference is not only speed (see *Compiler-Generated vs Manual*).
- **Deleting inherited functions:** `Derived` can delete an overload (`void func(double) = delete;`) so `d.func(1.5)` is a compile error. But through a `Base&`, `b.func(1.5)` still compiles and silently converts to `int`; the deleted overload only exists in `Derived`. ✅ tested
- **Decision tree:**
  - Managing resources? No → Rule of Zero (declare nothing). Yes → can `unique_ptr` / `string` / `vector` do it? Yes → Rule of Zero. No → Rule of Five.
  - Prevent copying? Delete the copy operations; allow move → `= default`, otherwise `= delete` the move operations too.
  - Custom constructor defined and you also want a default constructor? Add `ClassName() = default;`.
- **Rule of Five (from main.cpp):** the destructor, copy constructor, copy assignment, move constructor and move assignment together; the `Interface` base class pattern (`virtual ~Interface() = default; protected: Interface() = default;`) is covered in *Common Use Cases #5*.
- **Mistake 2 (partial Rule of Five):** `~Good();` plus `Good(const Good&) = delete;` and defaulted moves is a valid fix only when the members really can be moved; with a raw pointer you must write them by hand.
- **Code changes in main.cpp:**
  - `SimpleClass` / `ExplicitDefault`: `int value = 0;` (the old code read an uninitialized `int`, which is undefined behavior; the original printed `Value: 1`, now `Value: 0`). The comment `// SimpleClass() : value(0), name() {}` was wrong for a defaulted constructor.
  - `Singleton`: raw `new`, never deleted → Meyers singleton; "Singleton destroyed" now prints at exit.
  - `RuleOfFiveCustom`, `MoveOnlyResource`: initializer lists reordered to match the declaration order (`-Wreorder`); `memcpy` on a possibly-null pointer → `std::copy`; copy assignment allocates before deleting.
  - `SafeInteger`: added `SafeInteger(double) = delete;`.
  - Example 9 had only lecture text, now a real demo; Example 10 now creates and removes `test.txt` (the original always printed "Failed to open"); `#include <cstdio>` added.
  - ✅ tested with `-Wall -Wextra`, AddressSanitizer and UndefinedBehaviorSanitizer; the uninitialized-`int` claim rests on the standard plus the original output (no MemorySanitizer available).

