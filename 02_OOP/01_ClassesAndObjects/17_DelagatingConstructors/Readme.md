# 23_ExplicitKeyword - Preventing Implicit Conversions

## 📚 Overview

This folder introduces the **explicit** keyword - a keyword that prevents implicit type conversions. (Explicit *constructors* exist since C++98; explicit *conversion operators* came with C++11.) By making constructors and conversion operators explicit, you gain better type safety, catch bugs at compile time, and make your code's intent crystal clear.

**explicit** = Prevents implicit conversions, requires explicit construction

This is **ESSENTIAL** for writing safe, maintainable C++ code.

---

## 🎯 What You'll Learn

- What the explicit keyword is and why it exists
- The dangers of implicit conversions
- How to prevent implicit conversions with explicit
- Explicit constructors vs conversion operators
- When to use explicit (and when not to)
- Contextual conversions with explicit bool
- Standard library usage patterns
- Best practices and common mistakes
- Real-world examples

---

## 📖 Concept Explanation

### What is the explicit Keyword?

The **explicit** keyword prevents the compiler from using a constructor or conversion operator for implicit type conversions. It forces users to explicitly construct objects or perform conversions.

```cpp
class Array {
public:
    // Without explicit - DANGEROUS!
    Array(size_t size);
    
    // With explicit - SAFE!
    explicit Array(size_t size);
};

// Without explicit:
Array a = 10;        // ⚠️ Implicit conversion!
process(10);         // ⚠️ Creates temporary Array(10)!

// With explicit:
// Array a = 10;     // ❌ Compilation error!
Array a(10);         // ✅ Must be explicit
process(Array(10));  // ✅ Clear intent
```

### The Problem: Implicit Conversions

C++ allows implicit conversions through:

1. **Single-argument constructors**
2. **Conversion operators**

This can lead to unexpected behavior:

```cpp
class String {
public:
    String(const char* s);  // Non-explicit constructor
};

void process(String s) {
    // ...
}

// These all work (maybe unexpectedly):
String s1 = "hello";     // Implicit conversion
process("world");        // Creates temporary String!
```

**The dangers:**
- Unexpected temporary objects created
- Possible extra temporary objects
- Silent conversions (0.5 → 0 → Array(0))
- Hard-to-find bugs
- Unclear code intent

### The Solution: explicit Keyword

```cpp
class String {
public:
    explicit String(const char* s);  // Explicit!
};

// Now these are compilation errors:
// String s1 = "hello";   // ❌ Error!
// process("world");      // ❌ Error!

// Must be explicit:
String s1("hello");            // ✅ OK
String s2 = String("world");   // ✅ OK
process(String("test"));       // ✅ OK
```

### What explicit Does NOT Do

`explicit` only blocks the **implicit** use of the constructor. Inside an explicit call, the normal argument conversions still apply: ✅ tested

```cpp
class Array { public: explicit Array(size_t size); };

Array a(0.5);   // compiles, silently truncates 0.5 -> 0 (no warning with -Wall -Wextra)
Array b{0.5};   // ❌ narrowing conversion error: braces catch it
```

- It does not stop `UserId(1.5)` or `Dollars(...)` from converting their *argument*; use braces (`UserId{...}`) or a deleted overload (see 22_DefaultDelete) for that.
- An explicit constructor cannot be used in copy-list-initialization either: `Array a = {10};` and `process({10});` are errors.
- Direct-initialization with an explicit conversion operator works: `bool b(p);` and `bool b{p};` compile for `explicit operator bool`.

### Real-World Analogy

```
Think of valet parking:

WITHOUT EXPLICIT (Implicit Conversions):
  Customer: "Here's my key"
  Valet: "I'll assume you want:
          - Park your car
          - Wash it
          - Fill the tank
          - Detail the interior"
  ⚠️  Did too much! Unexpected costs!

WITH EXPLICIT:
  Customer: "Park my car" (explicit request)
  Valet: "Sure, parking only"
  ✅ Clear intent, no surprises

EXPLICIT = Must explicitly state your intent
NO IMPLICIT = No automatic assumptions
```

---

## 💻 Basic Syntax

### Explicit Constructor

```cpp
class MyClass {
public:
    // Explicit single-argument constructor
    explicit MyClass(int value);
    
    // Explicit with default parameter (acts single-arg)
    explicit MyClass(int x, int y = 0);
};

// Usage:
MyClass obj1(10);              // ✅ Direct initialization
// MyClass obj2 = 10;          // ❌ Error - explicit prevents this
MyClass obj3 = MyClass(10);    // ✅ Explicit construction
```

### Explicit Conversion Operator

```cpp
class SmartPointer {
public:
    // Explicit bool conversion
    explicit operator bool() const {
        return ptr != nullptr;
    }
    
private:
    int* ptr;
};

// Usage:
SmartPointer p;

if (p) { }                     // ✅ Contextual conversion OK
// bool b = p;                 // ❌ Error - explicit prevents this
bool b = static_cast<bool>(p); // ✅ Explicit cast OK
```

---

## 📝 Common Use Cases

### 1. Container Size Constructor

```cpp
class Vector {
public:
    explicit Vector(size_t size);  // Size constructor
};

// Prevents:
// Vector v = 10;           // ❌ Error!
// processVector(10);       // ❌ Error!

// Requires:
Vector v(10);               // ✅ Clear
processVector(Vector(10));  // ✅ Explicit
```

**Why explicit?**
- Prevents accidental array creation
- Avoids silent conversions (0.5 → 0)
- Makes resource allocation explicit
- Catches logic errors at compile time

### 2. Smart Pointers

```cpp
template<typename T>
class UniquePtr {
public:
    explicit UniquePtr(T* ptr);
    explicit operator bool() const;
};

// Prevents:
// UniquePtr<int> p = new int(42);  // ❌ Error!
// int x = p;                       // ❌ Error!

// Requires:
UniquePtr<int> p(new int(42));     // ✅ Clear
if (p) { /* use */ }               // ✅ Contextual bool
```

### 3. Strong Type Wrappers

```cpp
class UserId {
    int id;
public:
    explicit UserId(int i) : id(i) {}
};

class ProductId {
    int id;
public:
    explicit ProductId(int i) : id(i) {}
};

void processUser(UserId uid);
void processProduct(ProductId pid);

// Type safety:
// processUser(123);           // ❌ Error - can't mix!
processUser(UserId(123));      // ✅ Type-safe
// processUser(ProductId(456)); // ❌ Error - wrong type!
```

### 4. Unit Types

```cpp
class Meters {
    double value;
public:
    explicit Meters(double v) : value(v) {}
};

class Feet {
    double value;
public:
    explicit Feet(double v) : value(v) {}
};

void setDistance(Meters m);

// Prevents mixing units:
// setDistance(10.0);        // ❌ Error!
// setDistance(Feet(10.0));  // ❌ Error - wrong unit!
setDistance(Meters(10.0));   // ✅ Clear unit
```

### 5. Money Types

```cpp
class Dollars {
    double amount;
public:
    explicit Dollars(double amt) : amount(amt) {}
};

class Cents {
    int amount;
public:
    explicit Cents(int amt) : amount(amt) {}
};

void pay(Dollars d);

// Type safety:
// pay(50.0);        // ❌ Error!
// pay(Cents(5000)); // ❌ Error - wrong type!
pay(Dollars(50.0)); // ✅ Clear
```

---

## 📋 Examples in This Folder

### Example 1: The Problem
Shows dangerous implicit conversions without explicit.
- A non-explicit `StringBad(const char*)` lets `StringBad s = "World";` and `processBad("Implicit")` compile
- Unexpected temporaries, hidden bugs, less type safety

### Example 2: The Solution
Demonstrates how explicit prevents issues.
- Prevents accidental conversions, forces explicit construction, catches bugs at compile time
- The lines that would not compile are printed as comments

### Example 3: Array Danger
Shows why array constructors should be explicit.
- `processArrayBad(10)` and `processArrayBad(0.5)` silently create arrays (0.5 → 0)
- New in the demo: `ArrayGood(0.5)` still truncates, because explicit only blocks the implicit call

### Example 4: Conversion Operators
Explicit operator bool() for safe conversions.
- Works in boolean contexts (`if`, `while`, ...), blocks `int x = ptr;` and `bool b = ptr;`; same idea as `std::unique_ptr`
- Both classes now delete copying (they own the raw pointer), and `SmartPointerGood`'s own constructor is explicit too

### Example 5: Numeric Types
Money types with explicit constructors.
- Prevents mixing dollars and cents, avoids accidental conversions, clear intent in calculations
- Amounts are printed with two decimals (`$100.50`)

### Example 6: When NOT to Use Explicit
Mathematical types where implicit is natural.
- Allow implicit for natural, lossless conversions that improve readability (`ComplexNumber c = 5.0;`, like `std::string`)
- `Point(double)` meaning (v, v) is debatable; `ComplexNumber` is the better example

### Example 7: File Handle
Real-world RAII example with explicit.
- File operations should be intentional; prevents accidental handle creation from a string; makes resource management clear

### Example 8: Multiple Constructors
When explicit is needed vs not needed.
- Single-argument: usually explicit; multi-argument: usually not. But `Buffer b4 = {20, 'Y'};` is an implicit conversion via braces and would fail if that constructor were explicit

### Example 9: Standard Library
How std::vector, unique_ptr use explicit.
- Now a real demo: `vector<int> v(10)`, `unique_ptr<int> p(new int(42))` with `if (p)`, and `string s = "Hello"` (not explicit by design)

### Example 10: Best Practices
Summary and guidelines.
- Was only lecture text; now a demo with strong types (`UserId` / `OrderId`) that cannot be mixed up

---

## 🎓 Key Concepts

### When to Use explicit

**✅ ALWAYS use explicit for:**

1. **Single-argument constructors (default rule)**
```cpp
explicit MyClass(int);
explicit MyClass(const string&);
```

2. **Constructors with default parameters**
```cpp
explicit MyClass(int x, int y = 0);  // Acts single-arg!
```

3. **Conversion operators**
```cpp
explicit operator bool() const;
explicit operator int() const;
```

4. **Resource-managing classes**
```cpp
explicit Array(size_t size);
explicit FileHandle(const string& name);
```

5. **Type wrappers / Strong types**
```cpp
explicit UserId(int);
explicit Meters(double);
```

### When NOT to Use explicit

**❌ NEVER use explicit for:**

1. **Copy constructors**
```cpp
MyClass(const MyClass&);  // Never explicit!
```

2. **Move constructors**
```cpp
MyClass(MyClass&&);  // Never explicit!
```

3. **Multi-argument constructors (usually)**
```cpp
Point(double x, double y);  // Usually not explicit
// Careful: braces still convert implicitly with a non-explicit multi-arg constructor:
//   void draw(Point p);
//   draw({1.0, 2.0});        // implicit conversion
//   Point p = {1.0, 2.0};    // implicit conversion
//   return {1.0, 2.0};       // implicit conversion
// Making it explicit forbids all three. ✅ tested
```

**⚠️ RARELY use explicit for:**

4. **Mathematical types**
```cpp
Complex(double real);  // Real → Complex is natural
Point(double value);   // value → Point(value, value)  (debatable: a scalar is not obviously a point)
```

5. **String-like types**
```cpp
string(const char*);  // Designed for implicit use
```

### Contextual Conversions

**Explicit bool** has special rules - it works in boolean contexts:

```cpp
class SmartPtr {
public:
    explicit operator bool() const;
};

SmartPtr p;

// ✅ These work (contextual conversion):
if (p) { }
while (p) { }
for (; p; ) { }
p ? a : b;
!p;
p && other;

// ❌ These don't work:
// bool b = p;
// int x = p;
// p + 1;
// p == true;
// std::cout << p;
```

Direct-initialization is allowed too: `bool b(p);` and `bool b{p};` compile. ✅ tested

This is exactly how `std::unique_ptr` and `std::shared_ptr` work!

### The Rule of Thumb

```
┌─────────────────────────────────────┐
│  DECISION FLOWCHART                 │
├─────────────────────────────────────┤
│                                     │
│  Copy/Move constructor?             │
│  └─ YES → Never explicit            │
│  └─ NO → Continue...                │
│                                     │
│  Single-argument constructor?       │
│  └─ NO → Usually not explicit       │
│  └─ YES → Continue...               │
│                                     │
│  Implicit conversion natural?       │
│  └─ YES → Don't use explicit        │
│  └─ NO → Continue...                │
│                                     │
│  Manages resources?                 │
│  └─ YES → USE EXPLICIT!             │
│  └─ NO → Continue...                │
│                                     │
│  Could cause bugs?                  │
│  └─ YES → USE EXPLICIT!             │
│  └─ NO → Continue...                │
│                                     │
│  When in doubt?                     │
│  └─ USE EXPLICIT!                   │
│                                     │
└─────────────────────────────────────┘
```

---

## ⚠️ Common Mistakes

### ❌ Mistake 1: Forgetting explicit

**Bad:**
```cpp
class Array {
public:
    Array(size_t size);  // ❌ Should be explicit!
};

// Allows unexpected conversions:
Array a = 10;         // Creates Array(10)
processArray(20);     // Creates temporary Array(20)
processArray(0.5);    // Converts 0.5 → 0 → Array(0)!
```

**Fix:**
```cpp
class Array {
public:
    explicit Array(size_t size);  // ✅ Safe!
};
```

### ❌ Mistake 2: explicit on Copy Constructor

**Bad:**
```cpp
class Bad {
public:
    explicit Bad(const Bad&);  // ❌ WRONG!
};

Bad original;                // (assuming a default constructor exists)
// Bad copy = original;      // ❌ Error: copy-initialization needs a non-explicit copy constructor
// void take(Bad b); take(original);  // ❌ Error as well
Bad copy(original);          // ✅ direct-initialization still works
```

**Why it's bad:** Copy constructors should never be explicit - it breaks normal copy semantics (copy-initialization and passing by value). ✅ tested

**Fix:**
```cpp
class Good {
public:
    Good(const Good&);  // ✅ Not explicit
};
```

### ❌ Mistake 3: Non-explicit bool Operator

**Bad:**
```cpp
class Pointer {
public:
    operator bool() const;  // ❌ Should be explicit!
};

Pointer p;
int x = p;        // ⚠️ Converts bool → int!
p + 5;            // ⚠️ Converts to int, adds 5!
std::cout << p;   // ⚠️ Compiles and prints 1
```
✅ tested

**Fix:**
```cpp
class Pointer {
public:
    explicit operator bool() const;  // ✅ Safe!
};
```

### ❌ Mistake 4: explicit on Natural Conversions

**Bad:**
```cpp
class Complex {
public:
    explicit Complex(double real);  // ⚠️ Usually not explicit
};

// Prevents natural usage:
// Complex c = 3.14;      // ❌ Error
// Complex result = c + 2.0;  // ❌ Error
```

**Why it's bad:** Real numbers → complex numbers is a natural, expected conversion.

**Fix:**
```cpp
class Complex {
public:
    Complex(double real);  // ✅ Allow implicit for math types
};
```

---

## 🎯 Practice Exercises

### Exercise 1: Identify Issues
Given a class without explicit, identify potential bugs.

### Exercise 2: Add explicit
Refactor a codebase to add explicit where appropriate.

### Exercise 3: Strong Types
Create UserId and ProductId types that can't be mixed.

### Exercise 4: Unit System
Implement Meters, Feet, Kilometers with type safety.

### Exercise 5: Smart Pointer
Implement a basic smart pointer with explicit conversions.

---

## 🚀 How to Run

### Compile:
```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o explicit_keyword
```

(`<print>` / `std::println` needs GCC 14 or newer.)

### Run:
```bash
./explicit_keyword
```

### Expected Output:
The program demonstrates:
1. The problem with implicit conversions
2. The solution with explicit keyword
3. Array danger scenarios
4. Explicit conversion operators
5. Numeric type safety
6. When NOT to use explicit
7. Real-world file handle example
8. Multiple constructor scenarios
9. Standard library patterns
10. Best practices summary

---

## 📊 Visual Concepts

### Implicit vs Explicit Conversion

```
WITHOUT EXPLICIT (Implicit Conversion)
┌──────────────────────────────────────┐
│ void process(Array arr);             │
│                                      │
│ process(10);  ────→ Array(10)       │
│               Automatic!             │
│                                      │
│ Array a = 10; ────→ Array(10)       │
│               Implicit!              │
│                                      │
│ ⚠️  Surprising behavior!             │
│ ⚠️  Hidden object creation!          │
│ ⚠️  Potential bugs!                  │
└──────────────────────────────────────┘

WITH EXPLICIT (No Implicit Conversion)
┌──────────────────────────────────────┐
│ void process(Array arr);             │
│                                      │
│ process(10);  ────→ ❌ ERROR!        │
│               Must be explicit!      │
│                                      │
│ process(Array(10)); ────→ ✅ OK!     │
│                     Clear intent!    │
│                                      │
│ ✅ No surprises!                     │
│ ✅ Compile-time safety!              │
│ ✅ Clear code!                       │
└──────────────────────────────────────┘
```

### Contextual bool Conversion

```
EXPLICIT OPERATOR BOOL
┌────────────────────────────────────┐
│ class SmartPtr {                   │
│   explicit operator bool() const;  │
│ };                                 │
│                                    │
│ SmartPtr p;                        │
│                                    │
│ ALLOWED (Contextual):              │
│ ├─ if (p) { }          ✅          │
│ ├─ while (p) { }       ✅          │
│ ├─ for (; p;) { }      ✅          │
│ ├─ !p                  ✅          │
│ └─ p && other          ✅          │
│                                    │
│ PREVENTED (Non-contextual):        │
│ ├─ bool b = p;         ❌          │
│ ├─ int x = p;          ❌          │
│ ├─ p + 1;              ❌          │
│ └─ return p;           ❌          │
│                                    │
│ EXPLICIT CAST WORKS:               │
│ └─ bool b = static_cast<bool>(p);✅│
└────────────────────────────────────┘
```

### When to Use explicit

```
CONSTRUCTOR TYPE MATRIX
┌──────────────────────┬─────────┬────────────┐
│ Type                 │explicit?│ Why?       │
├──────────────────────┼─────────┼────────────┤
│ Copy constructor     │ NEVER   │ Semantics  │
│ Move constructor     │ NEVER   │ Semantics  │
│ Single argument      │ USUALLY │ Safety     │
│ Multiple arguments   │ RARELY  │ {a, b} only│
│ Mathematical types   │ RARELY  │ Natural    │
│ Resource managers    │ ALWAYS  │ Safety     │
│ String-like types    │ VARIES  │ Design     │
│ Strong type wrappers │ ALWAYS  │ Type safety│
│ Unit types           │ ALWAYS  │ Prevent mix│
└──────────────────────┴─────────┴────────────┘
```

---

## 📚 Related Topics

### Prerequisites:
- **06_GettersSetters** - Member functions
- **02_Constructor** - Constructor basics
- **22_DefaultDelete** - Special member functions

### Coming Next:
- **24_DelegatingConstructors** - Constructor delegation
- **25_CopyConstructor** - Deep copy concepts
- **26_MoveSemantics** - Move operations

### Related Concepts:
- **Type Safety** - Strong typing
- **SFINAE** - Template metaprogramming
- **Concepts (C++20)** - Type constraints

---

## 🎓 Summary

### What We Learned:

✅ **explicit** prevents implicit conversions  
✅ **Use for** single-argument constructors (default)  
✅ **Use for** conversion operators (usually)  
✅ **Never use** for copy/move constructors  
✅ **Contextual conversions** work with explicit bool  
✅ **Type safety** improved dramatically  
✅ **Bugs caught** at compile time  
✅ **Clear intent** in code  
✅ **Standard library** uses it extensively  
✅ **When in doubt**, use explicit!

### The Complete Pattern:

```cpp
// Resource-managing class with explicit
class FileHandle {
private:
    FILE* file;
    string filename;
    
public:
    // ✅ Explicit constructor
    explicit FileHandle(const string& name) 
        : file(fopen(name.c_str(), "r")), filename(name) {}   // same order as the members
    
    // ✅ Explicit bool conversion
    explicit operator bool() const {
        return file != nullptr;
    }
    
    // Copy operations deleted (move-only)
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;
    
    // Move operations
    FileHandle(FileHandle&& other) noexcept 
        : file(other.file), filename(move(other.filename)) {
        other.file = nullptr;
    }
    
    FileHandle& operator=(FileHandle&& other) noexcept {
        if (this != &other) {
            if (file) fclose(file);
            file = other.file;
            filename = move(other.filename);
            other.file = nullptr;
        }
        return *this;
    }
    
    ~FileHandle() {
        if (file) fclose(file);
    }
};

// Usage:
// FileHandle f = "data.txt";  // ❌ Error - explicit!
FileHandle f("data.txt");      // ✅ Clear intent

if (f) {  // ✅ Contextual bool conversion
    // File is open
}

// bool b = f;  // ❌ Error - explicit!
bool b = static_cast<bool>(f);  // ✅ Explicit cast
```

---

## ✨ Quick Reference Card

```cpp
// === EXPLICIT CONSTRUCTOR ===
class MyClass {
public:
    explicit MyClass(int x);         // Single argument
    explicit MyClass(int x, int y = 0); // Default params
};

MyClass obj1(10);                    // ✅ Direct
// MyClass obj2 = 10;                // ❌ Error
MyClass obj3 = MyClass(10);          // ✅ Explicit

// === EXPLICIT CONVERSION OPERATOR ===
class SmartPtr {
public:
    explicit operator bool() const;  // Explicit bool
};

SmartPtr p;
if (p) { }                           // ✅ Contextual
// bool b = p;                       // ❌ Error
bool b = static_cast<bool>(p);       // ✅ Cast

// === WHEN TO USE ===
// Always explicit:
explicit Vector(size_t size);        // Containers
explicit UserId(int id);             // Strong types
explicit Meters(double value);       // Unit types
explicit operator bool() const;      // Bool conversion

// Never explicit:
MyClass(const MyClass&);             // Copy constructor
MyClass(MyClass&&);                  // Move constructor

// Usually not explicit:
Point(double x, double y);           // Multi-arg
Complex(double real);                // Math types
```

---

## 📝 Best Practices

### ✅ DO:

- **Make single-argument constructors explicit** by default
- **Use explicit for conversion operators**
- **Follow standard library conventions**
- **Think about implicit conversion implications**
- **Document when implicit is intended**
- **Use explicit for resource classes**
- **Use explicit for strong types**
- **When in doubt, use explicit**
- **Remove explicit only if needed**
- **Test with both explicit and implicit usage**

### ❌ DON'T:

- **Never make copy/move constructors explicit**
- **Don't forget explicit on single-arg constructors**
- **Don't use explicit on mathematical types** (usually)
- **Don't make multi-arg constructors explicit** (usually)
- **Don't forget contextual bool conversions**
- **Don't use for types designed for implicit use**
- **Don't overuse on natural conversions**
- **Don't forget default parameters** act single-arg
- **Don't ignore compiler warnings**
- **Don't assume implicit is always bad**

---

## 🌟 Real-World Applications

### Type-Safe Identifiers
```cpp
class UserId {
    int id;
public:
    explicit UserId(int i) : id(i) {}
};

class OrderId {
    int id;
public:
    explicit OrderId(int i) : id(i) {}
};

// Can't mix!
void processUser(UserId uid);
void processOrder(OrderId oid);
```

### Unit System
```cpp
class Meters {
    double value;
public:
    explicit Meters(double v) : value(v) {}
};

class Feet {
    double value;
public:
    explicit Feet(double v) : value(v) {}
};

// Type-safe units
void setDistance(Meters m);
```

### Resource Management
```cpp
class DatabaseConnection {
public:
    explicit DatabaseConnection(const string& connStr);
    explicit operator bool() const;
};

// Clear resource handling
DatabaseConnection db("localhost");
if (db) {
    // Connection successful
}
```

---

## 📖 Further Reading

- [cppreference.com - explicit specifier](https://en.cppreference.com/cpp/language/explicit)
- "Effective C++" by Scott Meyers - Items 15 and 24 (implicit conversions; item numbers from memory, not checked)
- "Effective Modern C++" by Scott Meyers - Item 7 (brace initialization)
- [C++ Core Guidelines - C.46](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rc-explicit)
- [Microsoft - explicit (C++)](https://learn.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2012/h1y7x448(v=vs.110)) (archived Visual Studio docs)

---

## 🎯 Key Takeaways

1. **explicit** prevents implicit conversions
2. **Use for single-argument constructors** (default rule)
3. **Use for conversion operators** (usually)
4. **Never use for copy/move** constructors
5. **Contextual conversions** work with explicit bool
6. **Catches bugs** at compile time
7. **Makes intent clear** in code
8. **Standard library** uses it (vector, unique_ptr)
9. **When in doubt**, use explicit!
10. **Easier to remove** than find bugs

---

## 🚨 THE GOLDEN RULES

```
⚠️  RULE 1: Single-argument constructors
   Make them explicit by default!
   
   explicit MyClass(int);     // ✅ Default
   MyClass(int);              // ⚠️  Only if intended

⚠️  RULE 2: Never copy/move constructors
   These must NEVER be explicit!
   
   MyClass(const MyClass&);   // ✅ Not explicit
   explicit MyClass(const MyClass&); // ❌ WRONG!

⚠️  RULE 3: Conversion operators
   Almost always explicit!
   
   explicit operator bool();  // ✅ Standard practice
   operator bool();           // ⚠️  Dangerous

⚠️  RULE 4: When in doubt
   Use explicit! It's the safer default.
   
   Can always remove later if needed
   Much harder to find bugs from implicit conversions

⚠️  RULE 5: Follow the standard library
   vector, unique_ptr, thread all use explicit
   
   If standard library uses it, you probably should too!

   MAKE IT EXPLICIT - MAKE IT SAFE!
```

---

**Previous Topic:** [22_DefaultDelete](../22_DefaultDelete/) - Default and Delete  
**Next Topic:** [24_DelegatingConstructors](../24_DelegatingConstructors/) - Delegating Constructors

---

*Part of the C++ Classes and Objects series*  
*Difficulty: Intermediate*  
*Prerequisites: Constructors, type conversions*  
*CRITICAL for Type Safety*
---

## 🧾 Notes moved from main.cpp

- **Implicit conversions in C++** come from two places: single-argument constructors (`String(const char*)` → `String s = "hello";`) and conversion operators (`operator int() const` → `int x = i;`).
- **Constructors with default parameters** act like single-argument constructors: `explicit MyClass(int x, int y = 0);` is needed to stop `MyClass m = 5;`. ✅ tested
- **Conditionally explicit (C++20):** `explicit(bool)` makes a constructor explicit only when a condition holds, e.g. `explicit(!std::is_convertible_v<T, int>) Opt(T);`. ✅ tested
- **Standard library:**
  - `std::vector`: the size constructor is explicit (`vector<int> v = 10;` fails). ✅ tested
  - `std::unique_ptr`: `unique_ptr(T*)` is explicit (`unique_ptr<int> p = new int(42);` fails). ✅ tested
  - `std::thread`: the constructor is explicit (`std::thread t = f;` fails). ✅ tested
  - `std::string`: `string(const char*)` is intentionally implicit.
- **Core Guidelines C.46:** "By default, declare single-argument constructors explicit." Rationale: prevents subtle bugs, follows the principle of least surprise, easier to remove `explicit` than to find bugs from implicit conversions, matches the standard library.
- **Summary table** (corrected):

| Constructor type | explicit? | Reason |
|---|---|---|
| Single argument | Usually | Safety |
| Copy / move constructor | Never | Semantics |
| Multiple arguments | Rarely | Only `{a, b}` conversions are blocked by explicit |
| Default parameters (acts like single-arg) | Usually | Acts like single-arg |
| Mathematical types | Rarely | Natural |
| Resource managers | Always | Safety |
| Conversion operators | Usually | Safety |
| Conversion to bool | Always | Safety |

- **Code changes in main.cpp:**
  - `SmartPointerBad` / `SmartPointerGood`: copying deleted (the classes delete the raw pointer, so a copy would double-delete); `SmartPointerGood`'s constructor is explicit (otherwise `SmartPointerGood g = new int(1);` compiled).
  - Example 3: added `ArrayGood(0.5)` to show that explicit does not stop truncation inside an explicit call (the old lecture claimed it avoids silent 0.5 → 0 conversions).
  - Example 9 and 10 were pure lecture/printed text; now real demos (`unique_ptr` is actually used, strong types `UserId` / `OrderId`). `<cmath>` was unused and is replaced by `<memory>`.
  - Money amounts print with two decimals; `bool b` / `b3` are printed (they triggered unused-variable warnings); unused parameter names removed.
  - The `StringBad` comment said "string -> StringBad", but the constructor takes `const char*`.
  - ✅ tested with `-Wall -Wextra`, AddressSanitizer and UndefinedBehaviorSanitizer.
