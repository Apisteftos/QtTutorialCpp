# 27_ConversionOperators - Type Casting and Conversions

## 📚 Overview

This folder introduces **Conversion Operators** - special member functions that allow objects to be converted to other types. Conversion operators enable both implicit and explicit type conversions, with C++11 adding the critical `explicit` keyword for safer conversions. Proper use of conversion operators is essential for creating classes that integrate naturally with C++'s type system while maintaining type safety.

**Conversion Operators** = `operator Type() const` - convert objects to other types

This is **ESSENTIAL** for type safety and natural type conversions.

---

## 🎯 What You'll Learn

- What conversion operators are and why they exist
- Implicit vs explicit conversions
- The critical `explicit operator bool()`
- Contextual conversions (C++11)
- Common patterns (smart pointers, numeric wrappers)
- Multiple conversion operators
- Conversion ambiguity and resolution
- When to use (and when not to use)
- Best practices and safety guidelines
- Real-world examples from the standard library

---

## 📖 Concept Explanation

### What are Conversion Operators?

**Conversion operators** are special member functions that define how objects of a class can be converted to other types. They enable your custom types to work naturally with C++'s type system.

```cpp
class Fraction {
private:
    int numerator, denominator;
    
public:
    Fraction(int n, int d) : numerator(n), denominator(d) { }
    
    // ✅ Conversion operator to double
    operator double() const {
        return static_cast<double>(numerator) / denominator;
    }
};

// Usage
Fraction f(3, 4);
double d = f;  // ⚠️ Implicit conversion!
```

**Key features:**
- No return type specified (it's implicit in the name)
- Usually `const` (doesn't modify the object)
- Can be implicit or `explicit`
- Called automatically or with cast

### The Problem: Implicit Conversions

**Without `explicit`, conversions can happen unexpectedly:**

```cpp
class Integer {
    int value;
public:
    Integer(int v) : value(v) { }
    
    // ❌ Non-explicit conversions - dangerous!
    operator int() const { return value; }
    operator bool() const { return value != 0; }
};

Integer num(42);

// ⚠️ Unexpected conversions:
int x = num;          // Converts to int
int y = num + 10;     // Converts to int, adds 10
bool b = num;         // Converts to bool
int z = num * 2;      // Converts to int, multiplies!
```

**Problems:**
- Too many automatic conversions
- Unexpected behavior
- Can lead to subtle bugs
- Hard to track where conversions occur
- Bypasses type safety

### The Solution: explicit Conversions

**C++11 added `explicit` for conversion operators:**

```cpp
class Integer {
    int value;
public:
    Integer(int v) : value(v) { }
    
    // ✅ Explicit conversions - safe!
    explicit operator int() const { return value; }
    explicit operator bool() const { return value != 0; }
};

Integer num(42);

// ❌ Implicit conversions prevented:
// int x = num;       // Error!
// int y = num + 10;  // Error!

// ✅ Must be explicit:
int x = static_cast<int>(num);  // OK
if (num) { }  // OK - contextual conversion
```

**Benefits:**
- Prevents accidental conversions
- More intentional code
- Better type safety
- Still works in boolean contexts

### Real-World Analogy

```
Think of currency exchange:

IMPLICIT CONVERSION (Dangerous):
  You: "I have 100"
  System: "Is that dollars? Euros? Yen?"
  System: *Assumes dollars and converts*
  ⚠️  Might be wrong assumption!

EXPLICIT CONVERSION (Safe):
  You: "I have 100 USD"
  System: "You want to convert to EUR?"
  You: "Yes, explicitly convert 100 USD to EUR"
  System: *Performs requested conversion*
  ✅ Clear, intentional, no surprises

CONTEXTUAL CONVERSION (Smart):
  You: "Do I have money?"
  System: *Checks if amount > 0*
  System: "Yes" or "No"
  ✅ Natural question, clear meaning
```

---

## 💻 Basic Syntax

### Simple Conversion Operator

```cpp
class MyClass {
    int value;
public:
    // ⚠️ Implicit conversion
    operator int() const {
        return value;
    }
};

MyClass obj;
int x = obj;  // Automatic conversion
```

### Explicit Conversion Operator

```cpp
class MyClass {
    int value;
public:
    // ✅ Explicit conversion
    explicit operator int() const {
        return value;
    }
};

MyClass obj;
// int x = obj;  // ❌ Error!
int x = static_cast<int>(obj);  // ✅ Must use cast
```

### operator bool - Special Case

```cpp
class SmartPointer {
    int* ptr;
public:
    // ✅ ALWAYS explicit for bool!
    explicit operator bool() const {
        return ptr != nullptr;
    }
};

SmartPointer p;

// ✅ Works in boolean contexts:
if (p) { }
while (p) { }
for (; p;) { }

// ❌ Prevented elsewhere:
// int x = p;  // Error!
// p + 5;      // Error!
```

---

## 📝 Common Use Cases

### 1. Smart Pointer Validity Check

**The most common and important use case:**

```cpp
template<typename T>
class SmartPtr {
private:
    T* ptr;
    
public:
    SmartPtr(T* p = nullptr) : ptr(p) { }
    
    ~SmartPtr() {
        delete ptr;
    }
    
    // ✅ Explicit bool for validity checking
    explicit operator bool() const {
        return ptr != nullptr;
    }
    
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
};

// Usage - natural and safe
SmartPtr<int> p(new int(42));

if (p) {  // ✅ Check if valid
    cout << *p << "\n";
}

if (!p) {  // ✅ Check if null
    cout << "Pointer is null\n";
}
```

This is exactly how `std::unique_ptr`, `std::shared_ptr`, and `std::weak_ptr` work!

### 2. Numeric Wrapper Types

**Strong types with explicit conversion:**

```cpp
class Temperature {
private:
    double celsius;
    
public:
    explicit Temperature(double c) : celsius(c) { }
    
    // ✅ Explicit conversion to Fahrenheit
    explicit operator double() const {
        return (celsius * 9.0 / 5.0) + 32.0;
    }
    
    double getCelsius() const { return celsius; }
};

// Usage
Temperature temp(100.0);  // 100°C

// ❌ Prevented:
// double f = temp;  // Error!

// ✅ Must be explicit:
double f = static_cast<double>(temp);  // 212°F
```

### 3. String Representation

**Convert to string for display:**

```cpp
class Person {
private:
    string firstName;
    string lastName;
    int age;
    
public:
    Person(const string& first, const string& last, int a)
        : firstName(first), lastName(last), age(a) { }
    
    // ✅ Explicit string conversion
    explicit operator string() const {
        return firstName + " " + lastName + 
               " (age " + to_string(age) + ")";
    }
};

// Usage
Person p("Alice", "Smith", 30);

// Must be explicit:
string str = static_cast<string>(p);
cout << str << "\n";  // "Alice Smith (age 30)"
```

### 4. File/Stream State Checking

**Check if file/stream is valid:**

```cpp
class File {
private:
    string filename;
    bool isOpen;
    
public:
    File(const string& name) : filename(name), isOpen(false) { }
    
    void open() { isOpen = true; }
    void close() { isOpen = false; }
    
    // ✅ Explicit bool for state checking
    explicit operator bool() const {
        return isOpen;
    }
};

// Usage - natural error checking
File file("data.txt");
file.open();

if (file) {  // ✅ Check if opened successfully
    // Process file
}

while (file) {  // ✅ Read until file becomes invalid
    // Read data
}
```

### 5. Optional/Maybe Types

**Similar to `std::optional`:**

```cpp
template<typename T>
class Optional {
private:
    bool hasValue;
    T value;
    
public:
    Optional() : hasValue(false) { }
    Optional(const T& v) : hasValue(true), value(v) { }
    
    // ✅ Explicit bool to check if has value
    explicit operator bool() const {
        return hasValue;
    }
    
    const T& getValue() const {
        if (!hasValue) throw runtime_error("No value");
        return value;
    }
};

// Usage
Optional<int> opt(42);

if (opt) {  // ✅ Check if has value
    cout << opt.getValue() << "\n";
}
```

---

## 📋 Examples in This Folder

### Example 1: Basic Conversion
Simple conversion operator to double.

### Example 2: The Problem
Dangerous implicit conversions causing issues.

### Example 3: Explicit Conversions
Safe explicit conversions with static_cast.

### Example 4: Smart Pointer Bool
operator bool for pointer validity.

### Example 5: Multiple Conversions
Multiple conversion operators in one class.

### Example 6: String Conversion
Converting objects to string representation.

### Example 7: Numeric Conversions
Unit conversions (Celsius to Fahrenheit).

### Example 8: Contextual Conversions
Using explicit bool in boolean contexts.

### Example 9: Conversion Ambiguity
Multiple conversions causing ambiguity.

### Example 10: Best Practices
Guidelines for safe conversion operators.

---

## 🎓 Key Concepts

### Contextual Conversions

**`explicit operator bool()` has special rules - it works in boolean contexts:**

```cpp
class MyClass {
public:
    explicit operator bool() const { return true; }
};

MyClass obj;

// ✅ These work (contextual conversions):
if (obj) { }
while (obj) { }
for (; obj;) { }
obj ? a : b
!obj
obj && other
obj || other

// ❌ These don't work:
// bool b = obj;  // Error
// int x = obj;   // Error
// obj + 5;       // Error
```

This is called **contextual conversion** and is critical for modern C++!

### Implicit vs Explicit Summary

```cpp
// IMPLICIT (avoid for most types)
class ImplicitClass {
public:
    operator int() const { return 42; }
};

ImplicitClass obj;
int x = obj;           // ✅ Automatic
int y = obj + 10;      // ✅ Automatic
processInt(obj);       // ✅ Automatic

// EXPLICIT (recommended default)
class ExplicitClass {
public:
    explicit operator int() const { return 42; }
};

ExplicitClass obj;
// int x = obj;        // ❌ Error
int x = static_cast<int>(obj);  // ✅ Must cast
```

### Multiple Conversion Operators

```cpp
class Number {
    double value;
public:
    // ✅ Can have multiple
    explicit operator int() const {
        return static_cast<int>(value);
    }
    
    explicit operator double() const {
        return value;
    }
    
    explicit operator bool() const {
        return value != 0;
    }
};

// Each conversion is independent
Number n(3.14);
int i = static_cast<int>(n);       // Calls operator int()
double d = static_cast<double>(n); // Calls operator double()
if (n) { }                         // Calls operator bool()
```

### Const Correctness

**Conversion operators should almost always be `const`:**

```cpp
// ✅ CORRECT - const
class MyClass {
public:
    operator int() const {  // const!
        return value;
    }
};

// ❌ WRONG - not const
class MyClass {
public:
    operator int() {  // Not const
        return value;
    }
};
```

**Why const:**
- Conversion doesn't modify the object
- Works with const objects
- Allows conversion in const contexts
- Better const correctness

---

## ⚠️ Common Mistakes

### ❌ Mistake 1: Non-Explicit operator bool

**Bad:**
```cpp
class Bad {
public:
    operator bool() const {  // ❌ Not explicit!
        return true;
    }
};

Bad obj;
int x = obj;       // ⚠️ Converts to int!
obj + 5;           // ⚠️ Arithmetic!
```

**Fix:**
```cpp
class Good {
public:
    explicit operator bool() const {  // ✅ Explicit!
        return true;
    }
};

Good obj;
// int x = obj;    // ❌ Error - good!
if (obj) { }       // ✅ OK in boolean context
```

### ❌ Mistake 2: Multiple Implicit Conversions

**Bad:**
```cpp
class Bad {
public:
    operator int() const { return 42; }     // Implicit
    operator double() const { return 3.14; } // Implicit
};

Bad b;
auto x = b + 5;  // ⚠️ Ambiguous! int or double?
```

**Fix:**
```cpp
class Good {
public:
    explicit operator int() const { return 42; }
    explicit operator double() const { return 3.14; }
};

Good g;
auto x = static_cast<int>(g) + 5;  // ✅ Clear!
```

### ❌ Mistake 3: Forgetting const

**Bad:**
```cpp
class Bad {
public:
    operator int() {  // ❌ Not const!
        return value;
    }
};

const Bad obj;
// int x = obj;  // ❌ Error! Can't call non-const
```

**Fix:**
```cpp
class Good {
public:
    operator int() const {  // ✅ const!
        return value;
    }
};
```

### ❌ Mistake 4: Surprising Conversions

**Bad:**
```cpp
class UserId {
    int id;
public:
    operator int() const { return id; }  // ❌ Implicit!
};

void processUser(UserId uid);
void processProduct(ProductId pid);

UserId user(123);
processProduct(user);  // ⚠️ Might compile if ProductId(int)!
```

**Fix:**
```cpp
class UserId {
    int id;
public:
    explicit operator int() const { return id; }  // ✅ Explicit!
};
```

---

## 🎯 Practice Exercises

### Exercise 1: Smart Pointer
Implement a smart pointer with explicit operator bool.

### Exercise 2: Optional Type
Create an Optional<T> class with bool conversion.

### Exercise 3: Unit Conversions
Implement Meters, Feet classes with proper conversions.

### Exercise 4: Find the Bug
Given code with implicit conversions, identify problems.

### Exercise 5: Refactor
Convert implicit conversions to explicit.

---

## 🚀 How to Run

### Compile:
```bash
g++ -std=c++17 conversion_operators.cpp -o conversion_operators
```

### Run:
```bash
./conversion_operators
```

### Expected Output:
The program demonstrates:
1. Basic conversion operators
2. Problems with implicit conversions
3. Explicit conversions solution
4. Smart pointer bool conversion
5. Multiple conversion operators
6. String conversions
7. Numeric/unit conversions
8. Contextual bool conversions
9. Conversion ambiguity
10. Best practices summary

---

## 📊 Visual Concepts

### Implicit vs Explicit Flow

```
IMPLICIT CONVERSION (Automatic)
┌────────────────────────────────┐
│ MyClass obj;                   │
│                                │
│ int x = obj;  ────→  Automatic │
│                      conversion│
│                                │
│ ⚠️  Happens without asking!     │
└────────────────────────────────┘

EXPLICIT CONVERSION (Manual)
┌────────────────────────────────┐
│ MyClass obj;                   │
│                                │
│ int x = obj;  ────→  ❌ Error!  │
│                                │
│ int x = static_cast<int>(obj); │
│         ─────→ ✅ Must request! │
│                                │
│ ✅ Intentional and clear        │
└────────────────────────────────┘
```

### Contextual Conversion

```
EXPLICIT OPERATOR BOOL

┌──────────────────────────────────────┐
│ class MyClass {                      │
│   explicit operator bool() const;    │
│ };                                   │
│                                      │
│ MyClass obj;                         │
│                                      │
│ WORKS (Contextual):                  │
│ ├─ if (obj) { }          ✅          │
│ ├─ while (obj) { }       ✅          │
│ ├─ !obj                  ✅          │
│ └─ obj && other          ✅          │
│                                      │
│ DOESN'T WORK (Non-contextual):       │
│ ├─ bool b = obj;         ❌          │
│ ├─ int x = obj;          ❌          │
│ ├─ obj + 5;              ❌          │
│ └─ return obj;           ❌          │
│                                      │
│ MUST CAST:                           │
│ └─ bool b = static_cast<bool>(obj);✅│
└──────────────────────────────────────┘
```

### Decision Tree

```
SHOULD YOU ADD A CONVERSION OPERATOR?

Need conversion?
  NO  → Don't add it!
  YES → Continue...

Is it operator bool()?
  YES → ALWAYS explicit ✅
  NO  → Continue...

Is conversion safe and obvious?
  NO  → Make explicit ✅
  YES → Continue...

Could it cause surprises?
  YES → Make explicit ✅
  NO  → Still consider explicit ✅

DEFAULT: Make it explicit!
Implicit conversions should be extremely rare.
```

---

## 📚 Related Topics

### Prerequisites:
- **09_Constructors** - Constructor conversions
- **23_ExplicitKeyword** - Explicit constructors
- **08_AccessModifiers** - Const correctness

### Coming Next:
- **28_OperatorOverloading** - Other operators
- **29_FriendFunctions** - Friend declarations
- **30_TypeCasting** - Cast operators

### Related Concepts:
- **Type Safety** - Strong typing
- **RAII** - Resource management
- **Smart Pointers** - std::unique_ptr, std::shared_ptr

---

## 🎓 Summary

### What We Learned:

✅ **Conversion operators** enable type conversions  
✅ **Syntax** `operator Type() const`  
✅ **explicit prevents** implicit conversions  
✅ **operator bool** should ALWAYS be explicit  
✅ **Contextual conversions** work with explicit  
✅ **Use static_cast** for explicit conversions  
✅ **Make const** (doesn't modify object)  
✅ **Avoid implicit** conversions (usually)  
✅ **Multiple operators** possible but careful  
✅ **Safety over** convenience!

### The Complete Pattern:

```cpp
// Modern C++ smart pointer pattern
template<typename T>
class SmartPointer {
private:
    T* ptr;
    
public:
    explicit SmartPointer(T* p = nullptr) : ptr(p) { }
    
    ~SmartPointer() {
        delete ptr;
    }
    
    // ✅ Explicit bool for validity
    explicit operator bool() const {
        return ptr != nullptr;
    }
    
    // Prevent copying
    SmartPointer(const SmartPointer&) = delete;
    SmartPointer& operator=(const SmartPointer&) = delete;
    
    // Allow moving
    SmartPointer(SmartPointer&& other) noexcept 
        : ptr(other.ptr) {
        other.ptr = nullptr;
    }
    
    SmartPointer& operator=(SmartPointer&& other) noexcept {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }
    
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
    T* get() const { return ptr; }
};

// Usage - safe and natural
SmartPointer<int> p(new int(42));

if (p) {  // ✅ Check validity
    cout << *p << "\n";
}

// ❌ Prevented:
// int x = p;  // Error!
// p + 5;      // Error!
```

---

## ✨ Quick Reference Card

```cpp
// === BASIC CONVERSION ===
class MyClass {
public:
    operator int() const { return 42; }  // Implicit
};

// === EXPLICIT CONVERSION ===
class MyClass {
public:
    explicit operator int() const { return 42; }  // Explicit
};

// === OPERATOR BOOL (ALWAYS EXPLICIT!) ===
class MyClass {
public:
    explicit operator bool() const { return true; }
};

// === USAGE ===
MyClass obj;

// Implicit:
int x = obj;  // Automatic

// Explicit:
int x = static_cast<int>(obj);  // Must cast

// Contextual bool:
if (obj) { }  // Works with explicit!

// === MULTIPLE CONVERSIONS ===
class MyClass {
public:
    explicit operator int() const;
    explicit operator double() const;
    explicit operator bool() const;
};
```

---

## 📝 Best Practices

### ✅ DO:

- **Make conversions explicit** by default
- **operator bool ALWAYS explicit**
- **Make conversion operators const**
- **Have clear semantic meaning**
- **Document conversion behavior**
- **Use static_cast** for explicit conversions
- **Test conversion scenarios**
- **Consider safety over convenience**
- **Keep conversions simple**
- **Follow standard library patterns**

### ❌ DON'T:

- **Don't make everything implicit**
- **Don't create surprising conversions**
- **Don't forget const**
- **Don't create ambiguous conversions**
- **Don't use for unrelated types**
- **Don't have lossy implicit conversions**
- **Don't bypass type safety**
- **Don't use to make bad designs work**
- **Don't forget about contextual conversions**
- **Don't overuse conversion operators**

---

## 🌟 Real-World Applications

### Standard Library Smart Pointers
```cpp
unique_ptr<int> p(new int(42));
if (p) {  // explicit operator bool()
    // Use pointer
}
```

### Optional Values (C++17)
```cpp
optional<int> opt = getValue();
if (opt) {  // explicit operator bool()
    int value = *opt;
}
```

### Stream State Checking
```cpp
ifstream file("data.txt");
if (file) {  // explicit operator bool()
    // File opened successfully
}
```

### Strong Type Wrappers
```cpp
class UserId {
    int id;
public:
    explicit operator int() const { return id; }
};
```

---

## 📖 Further Reading

- [cppreference.com - User-defined conversion](https://en.cppreference.com/w/cpp/language/cast_operator)
- "Effective C++" by Scott Meyers - Item 15
- "Effective Modern C++" by Scott Meyers - Item 18
- [C++ Core Guidelines - C.164](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rc-explicit)

---

## 🎯 Key Takeaways

1. **Conversion operators** enable type conversions
2. **Syntax** `operator Type() const`
3. **explicit** prevents implicit conversions
4. **operator bool** should ALWAYS be explicit
5. **Contextual conversions** work in boolean contexts
6. **Make operators const**
7. **Prefer explicit** over implicit
8. **Avoid ambiguous** conversions
9. **Safety over** convenience
10. **Follow standard** library patterns

---

## 🚨 THE GOLDEN RULES

```
⚠️  RULE 1: operator bool ALWAYS explicit
   Never use implicit operator bool!
   
   ❌ operator bool() const;           // Dangerous
   ✅ explicit operator bool() const;  // Safe

⚠️  RULE 2: Prefer explicit
   Make conversions explicit by default
   
   Default: explicit operator Type() const;
   Rare: operator Type() const;  // Think hard!

⚠️  RULE 3: Make it const
   Conversions don't modify the object
   
   ✅ operator int() const;   // Correct
   ❌ operator int();          // Wrong

⚠️  RULE 4: Safety over convenience
   Type safety is more important than easy syntax
   
   Better to require explicit cast than cause bugs

⚠️  RULE 5: Follow the standard library
   std::unique_ptr, std::optional use explicit
   
   If standard library uses explicit, you should too!

   BE EXPLICIT - BE SAFE!
```

---

**Previous Topic:** [26_NestedClasses](../26_NestedClasses/) - Nested Classes  
**Next Topic:** [28_OperatorOverloading](../28_OperatorOverloading/) - Operator Overloading

---

*Part of the C++ Classes and Objects series*  
*Difficulty: Intermediate to Advanced*  
*Prerequisites: Constructors, const correctness, type casting*  
*CRITICAL for Type Safety*