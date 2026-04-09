# 22_DefaultDelete - Special Member Functions Control

## 📚 Overview

This folder introduces the **= default** and **= delete** keywords - powerful C++11 features that give you explicit control over special member functions. These keywords allow you to explicitly request compiler-generated implementations or prevent function usage entirely, enabling better const-correctness, type safety, and clearer design intent.

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
- May produce better optimized code

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
    RuleOfFiveEasy(const RuleOfFiveEasy&) = default;
    RuleOfFiveEasy& operator=(const RuleOfFiveEasy&) = default;
    RuleOfFiveEasy(RuleOfFiveEasy&&) = default;
    RuleOfFiveEasy& operator=(RuleOfFiveEasy&&) = default;
};
```

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
    static Singleton* instance;
    Singleton() = default;
    
public:
    // Delete all copy and move operations
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;
    
    static Singleton* getInstance() {
        if (!instance) {
            instance = new Singleton();
        }
        return instance;
    }
};
```

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
```

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

### Example 2: Deleting Copy Operations
Non-copyable class with deleted copy operations.

### Example 3: Singleton Pattern
Implementing singleton with deleted operations.

### Example 4: Rule of Five - All Defaults
Using = default for all five operations.

### Example 5: Rule of Five - Custom
Manual implementation of all five operations.

### Example 6: Deleting Specific Overloads
Type safety by deleting specific function overloads.

### Example 7: Move-Only Type
Creating a move-only resource class.

### Example 8: Default on Declaration
Separating declaration and definition.

### Example 9: When NOT to Use
Anti-patterns and common mistakes.

### Example 10: Real-World - File Handle
RAII file handle with proper resource management.

---

## 🎓 Key Concepts

### When to Use = default

**✅ USE = DEFAULT:**

1. **Restoring deleted functions:**
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
    unique_ptr<int> data;
public:
    ~D() = default;
    D(const D&) = default;
    D& operator=(const D&) = default;
    D(D&&) = default;
    D& operator=(D&&) = default;
};
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
- Can use `memcpy` safely
- Can use in unions
- Better performance
- POD-like behavior

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
```

### Compiler-Generated vs Manual

**Compiler-Generated (= default):**
- May be faster (compiler optimizations)
- Produces trivial types when possible
- Less code to maintain
- Consistent behavior

**Manual Implementation:**
- Full control over initialization
- Can add custom logic
- Required for complex resource management
- More verbose

**Rule of Thumb:** Use = default unless you need custom behavior.

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

**Why it's bad:** Over-documenting, adds noise

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

// Bad obj;  // ❌ Can't destroy when goes out of scope!
```

**Why it's bad:** Objects can't be destroyed, memory leaks

**Fix:** Don't delete destructors (except in very special cases)

### ❌ Mistake 4: Forgetting noexcept on Move

**Bad:**
```cpp
class Bad {
public:
    Bad(Bad&&) = default;  // Missing noexcept
};
```

**Why it's bad:** 
- Move operations should be noexcept
- Containers won't use move operations
- Performance impact

**Fix:**
```cpp
class Good {
public:
    Good(Good&&) noexcept = default;
    Good& operator=(Good&&) noexcept = default;
};
```

### ❌ Mistake 5: Using Private Instead of Delete

**Old Way (C++03):**
```cpp
class Old {
private:
    Old(const Old&);  // Declared but not defined
    // ❌ Link error, not compile error
    // ❌ Not clear intent
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
g++ -std=c++17 default_delete.cpp -o default_delete
```

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
✅ **Trivial types** for better performance  
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
    
    MoveOnly(MoveOnly&&) noexcept = default;
    MoveOnly& operator=(MoveOnly&&) noexcept = default;
};

// RULE OF FIVE (When necessary)
class RuleOfFive {
private:
    int* data;
    size_t size;
public:
    ~RuleOfFive() { delete[] data; }
    
    RuleOfFive(const RuleOfFive& other)
        : size(other.size), data(new int[size]) {
        memcpy(data, other.data, size * sizeof(int));
    }
    
    RuleOfFive& operator=(const RuleOfFive& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = new int[size];
            memcpy(data, other.data, size * sizeof(int));
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
    MoveOnly(MoveOnly&&) noexcept = default;
    MoveOnly& operator=(MoveOnly&&) noexcept = default;
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
- **Make move operations** noexcept
- **Use = delete** for type safety
- **Keep it simple** - avoid manual resource management
- **Document intent** - explicit is better
- **Test thoroughly** - verify copy/move behavior

### ❌ DON'T:

- **Don't overuse** = default unnecessarily
- **Don't forget** the Rule of Five
- **Don't delete** destructors (except special cases)
- **Don't mix** RAII and manual management
- **Don't forget** noexcept on move operations
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
```

---

## 📖 Further Reading

- [cppreference.com - default specifier](https://en.cppreference.com/w/cpp/language/function#Function_definition)
- [cppreference.com - delete specifier](https://en.cppreference.com/w/cpp/language/function#Deleted_functions)
- "Effective Modern C++" by Scott Meyers - Items 11, 17
- [C++ Core Guidelines - C.21](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rc-zero)
- [Rule of Zero on FLorian's Blog](https://foonathan.net/2019/02/special-member-functions/)

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

⚠️  RULE 4: Move operations should be noexcept
   Containers won't use them otherwise
   
   A(A&&) noexcept = default;   // ✅ Good
   A(A&&) = default;            // ⚠️  Missing noexcept

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