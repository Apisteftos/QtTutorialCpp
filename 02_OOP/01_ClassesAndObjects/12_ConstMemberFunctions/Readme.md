# 19_ConstMemberFunctions - Const Correctness in C++

## 📚 Overview

This folder introduces **Const Member Functions** - one of the most important concepts in C++ for writing safe, correct, and maintainable code. Const member functions promise not to modify object state and are essential for working with const objects, enabling const correctness throughout your codebase.

**Const Member Function** = Function that promises NOT to modify the object's state

This is **ESSENTIAL** knowledge for professional C++ development and const correctness.

---

## 🎯 What You'll Learn

- What const member functions are and why they're critical
- How to declare and use const member functions
- The difference between const and non-const functions
- Why const objects need const functions
- Const overloading (two versions of same function)
- The mutable keyword and when to use it
- Const with pointers and references
- Best practices for const correctness
- Common mistakes and how to avoid them
- Real-world applications

---

## 📖 Concept Explanation

### What are Const Member Functions?

**Const member functions** are member functions that promise not to modify the object's state. They are declared with the `const` keyword after the parameter list.

```cpp
class Person {
private:
    string name;
    int age;
    
public:
    // Const member function - read only
    string getName() const {
        return name;  // Can read
    }
    
    // Non-const member function - can modify
    void setName(const string& n) {
        name = n;  // Can modify
    }
};
```

**The Golden Rule:**
> If a member function doesn't modify object state, mark it const!

### Why Are They Critical?

**1. Enable Const Objects**
```cpp
const Person p("Alice", 30);
p.getName();  // ✅ Works - getName() is const
p.setName("Bob");  // ❌ Error - setName() is not const
```

Without const functions, const objects would be nearly useless!

**2. Required for Const References**
```cpp
void display(const Person& p) {
    cout << p.getName();  // ✅ Only works if getName() is const
}
```

**3. Communicate Intent**
- Makes it clear which functions read vs modify
- Self-documenting code
- Better API design

**4. Compiler Safety**
- Prevents accidental modifications
- Catches bugs at compile-time
- No runtime cost

### Real-World Analogy

```
Think of a library:

NON-CONST FUNCTIONS = Librarian
  - Can check out books (read)
  - Can add new books (write)
  - Can rearrange shelves (modify)
  - Full access

CONST FUNCTIONS = Visitor
  - Can read book titles (read only)
  - Can browse shelves (query)
  - Cannot take books (no modify)
  - Cannot rearrange (no changes)
  - View-only access

CONST OBJECT = Read-only library
  - Only visitors allowed
  - No librarians (no modifications)
  - Pure reading/browsing
```

### Visual Representation

```
REGULAR OBJECT:
┌──────────────────┐
│  Person object   │
│  ──────────────  │
│  Can call:       │
│  ✅ const funcs  │
│  ✅ non-const    │
└──────────────────┘

CONST OBJECT:
┌──────────────────┐
│ const Person obj │
│  ──────────────  │
│  Can call:       │
│  ✅ const funcs  │
│  ❌ non-const    │
└──────────────────┘
```

---

## 💻 Basic Syntax

### Simple Const Function

```cpp
class BankAccount {
private:
    double balance;
    
public:
    // Const - read only
    double getBalance() const {
        return balance;
    }
    
    // Non-const - modifies
    void deposit(double amount) {
        balance += amount;
    }
};
```

### Const Correctness Pattern

```cpp
class Rectangle {
private:
    double width, height;
    
public:
    // Query functions - CONST
    double getWidth() const { return width; }
    double getHeight() const { return height; }
    double area() const { return width * height; }
    bool isSquare() const { return width == height; }
    
    // Modifier functions - NOT CONST
    void setWidth(double w) { width = w; }
    void setHeight(double h) { height = h; }
    void scale(double factor) { width *= factor; height *= factor; }
};
```

### Usage with Const Objects

```cpp
Rectangle rect(10, 5);

// Regular object - can call anything
rect.getWidth();     // ✅ OK
rect.setWidth(20);   // ✅ OK

const Rectangle constRect(10, 5);

// Const object - only const functions
constRect.getWidth();     // ✅ OK - const function
constRect.area();         // ✅ OK - const function
constRect.setWidth(20);   // ❌ ERROR - not const!
```

---

## 🔍 The Problem Without Const

### The Issue

```cpp
class Point {
private:
    int x, y;
    
public:
    Point(int x, int y) : x(x), y(y) {}
    
    // ❌ Forgot const!
    int getX() { return x; }
    int getY() { return y; }
    void display() {
        cout << "(" << x << ", " << y << ")";
    }
};

// Function accepts const reference
void printPoint(const Point& p) {
    // ❌ All these fail!
    // p.getX();     // Error: not const
    // p.getY();     // Error: not const
    // p.display();  // Error: not const
}
```

**Problem:**
- Can't use with const objects
- Can't pass to functions with const references
- Poor API design

### The Solution

```cpp
class Point {
private:
    int x, y;
    
public:
    Point(int x, int y) : x(x), y(y) {}
    
    // ✅ Added const!
    int getX() const { return x; }
    int getY() const { return y; }
    void display() const {
        cout << "(" << x << ", " << y << ")";
    }
};

void printPoint(const Point& p) {
    // ✅ All work now!
    p.getX();
    p.getY();
    p.display();
}
```

---

## 📋 Examples in This Folder

### Example 1: Basic Const Functions
Introduction to const member functions.
- `display()` and `getBalance()` work on a normal and on a `const` account; `deposit()` would not compile on the const one

### Example 2: Why Const Matters
Demonstrates the problem without const.
- `PointBad` (no const) cannot be read through a `const&`; the function prints a placeholder instead
- `PointGood` can

### Example 3: Const Correctness in Action
Full example with proper const usage.
- `analyzeRectangle(const Rectangle&)` can call only the const getters; `scale()` changes the original

### Example 4: What You Can't Do
Understanding const function restrictions.
- Legal: read members, call other const functions, use locals
- Illegal (commented out in the class): modify members, call non-const functions

### Example 5: Const Overloading
Two versions of same function.
- The printed `[Called const version]` / `[Called non-const version]` shows which one the compiler picked

### Example 6: Mutable Keyword
Modifying in const functions.
- A `const` calculator still counts cache hits, because `cacheHits`, `cached` and `cachedResult` are `mutable`

### Example 7: Const and Pointers/References
Different const placements.
- Pointer to const, const pointer, const pointer to const, const reference: what each one allows

### Example 8: Real-World Example
Complete Date class.
- Getters, `isLeapYear()`, `isValid()`, `daysInMonth()` and `display()` are const; setters and `nextDay()` are not

### Example 9: Const in Containers
Working with collections.
- `Classroom` has a const and a non-const `getStudent()`; `displayAll()` works through a `const Classroom&`

### Example 10: Common Mistakes
Pitfalls and how to avoid them.
- The class holds the correct versions; the three mistakes are in comments inside it (see Common Mistakes below)

---

## 🎓 Key Concepts

### Rules for Const Functions

**What You CAN Do:**
```cpp
void constFunc() const {
    // ✅ Read member variables
    int x = memberVar;
    
    // ✅ Return member values
    return memberVar;
    
    // ✅ Call other const functions
    int y = otherConstFunc();
    
    // ✅ Create/modify local variables
    int temp = x * 2;
    
    // ✅ Modify mutable members
    mutableCounter++;
}
```

**What You CANNOT Do:**
```cpp
void constFunc() const {
    // ❌ Modify member variables
    // memberVar = 10;
    
    // ❌ Call non-const functions
    // nonConstFunc();
    
    // ❌ Assign to members
    // memberVar += 5;
}
```

**Const is shallow** (tested): a const member function protects the object's *own* members, not what they point to or refer to.
```cpp
struct B {
    int* p;
    int& r;
    void f() const {
        *p = 5;   // ✅ compiles: p itself is unchanged, only its target is modified
        r = 6;    // ✅ compiles: same for the referenced object
    }
};
```
Also, a const member function may call anything on *other* objects. The restriction is about `this`. And `const_cast<X*>(this)->v = 99;` inside a const member function compiles and works on a non-const object (tested, prints 99). It is undefined behaviour if the object was *defined* const (from memory, not run here).

### Const Overloading

You can have **two versions** of the same function:

```cpp
class Container {
private:
    string data;
    
public:
    // Const version - returns const reference
    const string& getData() const {
        return data;  // Read-only access
    }
    
    // Non-const version - returns non-const reference
    string& getData() {
        return data;  // Modifiable access
    }
};

// Usage
Container c;
c.getData() = "New";  // Calls non-const version, can modify

const Container cc;
string s = cc.getData();  // Calls const version, read-only
```

**When to use:**
- Element access (like `operator[]`)
- Getting references to internal data
- Iterator access (`begin()`, `end()`)
- When you want read-only and modifiable versions

**Which version is called** (tested):
- const object → const version; if only a non-const version exists: *error: passing 'const OnlyNon' as 'this' argument discards qualifiers*
- non-const object → non-const version when both exist; if only the const version exists, it is used
- A const getter cannot return a non-const reference to a member: *error: binding reference of type 'int&' to 'const int' discards qualifiers*

**Don't duplicate a long body** (Effective C++ Item 3 trick, tested): implement the const version, and let the non-const one call it:
```cpp
const std::string& get() const { return d; }
std::string& get() {
    return const_cast<std::string&>(static_cast<const C&>(*this).get());
}
```

### Mutable Keyword

**Mutable** allows modification in const functions:

```cpp
class Cache {
private:
    int value;
    mutable bool cached;      // Can modify in const!
    mutable int cachedResult;
    
public:
    int compute() const {
        if (cached) {
            return cachedResult;
        }
        
        // Expensive computation
        int result = value * value;
        
        // ✅ OK to modify mutable members
        cachedResult = result;
        cached = true;
        
        return result;
    }
};
```

**When to use mutable:**
- ✅ Caching mechanisms
- ✅ Performance counters
- ✅ Debug information
- ✅ Thread synchronization (mutexes)
- ✅ Lazy initialization

**When NOT to use:**
- ❌ Business logic state
- ❌ Core object data
- ❌ As a workaround for missing const
- ❌ To avoid proper design

**Thread safety:** the standard library expects const member functions to be safe to call from several threads at the same time (from memory, the "const means thread-safe" convention since C++11, not checked here). A `mutable` cache or counter changed inside a const function breaks that. Protect it with a `mutable std::mutex` or make it `std::atomic`. The `cacheHits++` in Example 6 is fine in a single thread but would be a data race in two. More on `mutable` in the `21_MutableKeyword` folder.

### Const with Pointers

**Four variations:**

```cpp
Type a, b;
Type value;

// 1. Pointer to const
const Type* ptr1 = &a;
ptr1 = &b;        // ✅ Can change pointer
// *ptr1 = value; // ❌ Can't modify data

// 2. Const pointer
Type* const ptr2 = &a;  // must be initialised (see below)
// ptr2 = &b;     // ❌ Can't change pointer
*ptr2 = value;    // ✅ Can modify data

// 3. Const pointer to const
const Type* const ptr3 = &a;
// ptr3 = &b;     // ❌ Can't change pointer
// *ptr3 = value; // ❌ Can't modify data

// 4. Reference to const
const Type& ref = a;
// ref = value;   // ❌ Can't modify
```

A `const` pointer or reference **must be initialised where it is declared**: `Type* const p;` gives *error: uninitialized 'const p'* (tested).

**Memory trick:** Read right to left:
```cpp
const int* ptr     → ptr is a pointer to const int
int* const ptr     → ptr is a const pointer to int
const int* const   → ptr is a const pointer to const int
```

---

## ⚠️ Common Mistakes

### ❌ Mistake 1: Forgetting Const on Getters

**Bad:**
```cpp
class Person {
    string name;
public:
    string getName() {  // ❌ Should be const!
        return name;
    }
};

const Person p("Alice");
// p.getName();  // ❌ Error!
```

**Fix:**
```cpp
string getName() const {  // ✅ Added const
    return name;
}
```

### ❌ Mistake 2: Wrong Const Placement

**Bad:**
```cpp
void const display() { }  // ❌ NOT a const member function!
const void display() { }  // ❌ Same thing
```

**What really happens (tested):** both lines **compile**. They declare an ordinary non-const function whose return type is `const void`. GCC only warns with `-Wextra`: *type qualifiers ignored on function return type*. The mistake stays hidden until you call it on a const object: *error: passing 'const A' as 'this' argument discards qualifiers*.

**Fix:**
```cpp
void display() const { }  // ✅ Correct - after parameters
```

**Order of the trailing parts:** `const` comes first, then `noexcept`, then `override`/`final`: `void f() const noexcept override` compiles, `void f() noexcept const` gives *error: expected ';' at end of member declaration* (tested). For a pure virtual: `virtual void f() const = 0;`.

### ❌ Mistake 3: Modifying in Const Function

**Bad:**
```cpp
void increment() const {
    count++;  // ❌ Compilation error!
}
```
GCC: *error: increment of member 'Y::count' in read-only object* (tested).

**Fix:**
```cpp
// Option 1: Remove const
void increment() {
    count++;
}

// Option 2: Make count mutable
mutable int count;
void increment() const {
    count++;  // ✅ OK now
}
```

### ❌ Mistake 4: Calling Non-Const from Const

**Bad:**
```cpp
void display() const {
    modify();  // ❌ Error if modify() not const
}

void modify() {  // Not const
    value++;
}
```
GCC: *error: passing 'const Y' as 'this' argument discards qualifiers* (tested).

**Fix:**
```cpp
// Option 1: Make modify() const (if it doesn't modify)
void modify() const { }

// Option 2: Remove const from display()
void display() { }
```

### ❌ Mistake 5: Not Testing with Const Objects

**Bad:**
```cpp
// Never tested with const!
class MyClass {
    int getValue() { return value; }  // Oops, not const
};
```

**Fix:**
```cpp
// Always test:
const MyClass obj(42);
obj.getValue();  // Will catch missing const!

// Careful: 'const MyClass obj;' (no arguments) only compiles if MyClass has a user-provided
// default constructor or every member has a default initializer. A class with plain
// 'int value;' members and no constructor gives: error: uninitialized 'const obj' (tested)
```

---

## 🎯 Practice Exercises

### Exercise 1: Add Const
Take a class without const functions and add const where appropriate.

### Exercise 2: Const Overloading
Implement a class with const-overloaded member functions.

### Exercise 3: Find the Bug
Given code with const errors, identify and fix them.

### Exercise 4: Mutable Usage
Create a class with appropriate use of mutable (e.g., caching).

### Exercise 5: Const Correctness
Design a class hierarchy with proper const correctness throughout.

---

## 🚀 How to Run

### Compile:
```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o const_member_functions
```
(`main.cpp` uses `std::println` from `<print>`, so it needs GCC 14+ or another compiler with C++23 `<print>`.)

### Run:
```bash
./const_member_functions
```

### Expected Output:
The program demonstrates:
1. Basic const functions
2. Why const matters
3. Const correctness in action
4. Const function restrictions
5. Const overloading
6. Mutable keyword usage
7. Const with pointers/references
8. Real-world Date class example
9. Const in containers
10. Common mistakes

---

## 📊 Visual Concepts

### Const Function Behavior

```
NON-CONST OBJECT              CONST OBJECT
┌──────────────────┐          ┌──────────────────┐
│  MyClass obj     │          │ const MyClass o  │
│  ──────────────  │          │  ──────────────  │
│                  │          │                  │
│  getValue() ✅   │          │  getValue() ✅   │
│  const function  │          │  const function  │
│                  │          │                  │
│  setValue() ✅   │          │  setValue() ❌   │
│  non-const func  │          │  COMPILATION     │
│                  │          │  ERROR!          │
└──────────────────┘          └──────────────────┘
```

### Const Propagation

```
Function Parameter              Requires
┌─────────────────────┐        ┌─────────────────┐
│ void f(const T& x)  │───────>│ const functions │
│                     │        │ in class T      │
└─────────────────────┘        └─────────────────┘
        │                              │
        │                              │
        v                              v
┌─────────────────────┐        ┌─────────────────┐
│ Can only call       │        │ Which can only  │
│ const functions     │        │ call other      │
│ on x                │        │ const functions │
└─────────────────────┘        └─────────────────┘
```

### Const Overloading Selection

```
Container c;                    const Container cc;
    │                                  │
    │ Call getData()                   │ Call getData()
    │                                  │
    v                                  v
┌──────────────────┐          ┌───────────────────┐
│ Non-const        │          │ Const version     │
│ string& getData()│          │ const string&     │
│                  │          │ getData() const   │
│ Returns          │          │ Returns           │
│ modifiable ref   │          │ read-only ref     │
└──────────────────┘          └───────────────────┘
```

---

## 📚 Related Topics

### Prerequisites:
- **01_BasicClass** - Basic class concepts
- **04_MemberFunctions** - Member function basics
- **06_GettersSetters** - Accessor methods

### Coming Next:
- **20_ConstObjects** - Working with const objects
- **21_MutableKeyword** - Advanced mutable usage
- **22_DefaultDelete** - Controlling special functions

### Related Concepts:
- **Const Correctness** - Design principle
- **RAII** - Resource management
- **Move Semantics** - Optimization

---

## 🎓 Summary

### What We Learned:

✅ **Const functions** promise not to modify state
✅ **Required** for const objects to call functions
✅ **Syntax**: `return_type func() const { }`
✅ **Can read** members but not modify
✅ **Can call** only other const functions on `this`
✅ **Const overloading** provides flexibility
✅ **Mutable** allows modification of specific members
✅ **Critical** for const correctness
✅ **Compile-time safety** at no runtime cost (it is a correctness feature, not a speed feature)
✅ **Mark all read-only** functions const!

### The Complete Pattern:

```cpp
class MyClass {
private:
    int value;
    string name;
    mutable int accessCount;
    
public:
    // Constructor
    MyClass(int v, const string& n) 
        : value(v), name(n), accessCount(0) {}
    
    // Const getters - read only
    int getValue() const { 
        accessCount++;  // mutable OK
        return value; 
    }
    
    string getName() const { 
        return name; 
    }
    
    // Const query functions
    bool isPositive() const {
        return value > 0;
    }
    
    void display() const {
        cout << name << ": " << value << "\n";
    }
    
    // Non-const setters - modify
    void setValue(int v) {
        value = v;
    }
    
    void setName(const string& n) {
        name = n;
    }
    
    // Const overloading
    const string& data() const {
        return name;
    }
    
    string& data() {
        return name;
    }
};

// Usage
MyClass obj(42, "Test");
obj.getValue();      // ✅ const function
obj.setValue(100);   // ✅ non-const function

const MyClass constObj(42, "Test");
constObj.getValue();      // ✅ const function
// constObj.setValue(100); // ❌ ERROR
```

---

## ✨ Quick Reference Card

```cpp
// === CONST MEMBER FUNCTIONS ===

class Example {
private:
    int value = 0;           // default initializers, so 'const Example constObj;' compiles
    mutable int counter = 0;
    
public:
    // CONST FUNCTION - read only
    int getValue() const {
        counter++;      // ✅ mutable OK
        return value;   // ✅ read OK
        // value = 10;  // ❌ can't modify
    }
    
    // NON-CONST FUNCTION - can modify
    void setValue(int v) {
        value = v;      // ✅ modify OK
    }
    
    // CONST OVERLOAD - read only
    const int& get() const {
        return value;
    }
    
    // NON-CONST OVERLOAD - modifiable
    int& get() {
        return value;
    }
};

// === USAGE ===

Example obj;
obj.getValue();         // ✅ OK
obj.setValue(10);       // ✅ OK
obj.get() = 20;         // ✅ OK - non-const version

const Example constObj;
constObj.getValue();    // ✅ OK
// constObj.setValue(10); // ❌ ERROR
int x = constObj.get(); // ✅ OK - const version

// === CONST POINTERS ===

const Type* ptr;        // pointer to const
Type* const ptr;        // const pointer
const Type* const ptr;  // const pointer to const
const Type& ref;        // const reference
```

---

## 🔍 Best Practices

### ✅ DO:

- **Mark all read-only functions const**
- **Use const references** for parameters
- **Test with const objects** regularly
- **Const-overload** when returning references
- **Use mutable** for caching/counters
- **const after parameters**: `func() const`
- **Make getters const** by default
- **Query functions const** (isEmpty, isValid)
- **Display functions const**
- **Comparison operators const**

### ❌ DON'T:

- **Forget const** on getters
- **Mark modifiers const** (setters, increment)
- **Wrong placement**: `const func()` ❌
- **Abuse mutable** for business logic
- **Use const_cast** unless necessary
- **Ignore const errors** from compiler
- **Skip testing** with const objects
- **Mix const and non-const** inconsistently
- **Modify state** in const functions
- **Call non-const** from const functions

---

## 🌟 Real-World Applications

### APIs and Libraries
```cpp
class Vector3D {
public:
    double length() const;  // Query - const
    Vector3D normalized() const;  // Calculation - const
    void normalize();  // Modification - not const
};
```

### Game Development
```cpp
class Player {
public:
    Vector getPosition() const;  // Read - const
    int getHealth() const;  // Read - const
    void takeDamage(int amount);  // Modify - not const
};
```

### GUI Frameworks
```cpp
class Widget {
public:
    bool isVisible() const;  // Query - const
    Size getSize() const;  // Read - const
    void setSize(const Size& s);  // Modify - not const
};
```

### Data Structures
```cpp
class BinaryTree {
public:
    int size() const;  // Query - const
    bool contains(int value) const;  // Search - const
    void insert(int value);  // Modify - not const
};
```

---

## 📖 Further Reading

- [cppreference.com - Member functions (const- and volatile-qualified)](https://en.cppreference.com/cpp/language/member_functions)
- "Effective C++" by Scott Meyers - Item 3
- "C++ Coding Standards" by Herb Sutter - Item 15
- [C++ Core Guidelines - Con.1 to Con.5](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#con-constants-and-immutability) (Con.2: by default, make member functions `const`)

---

## 🎯 Key Takeaways

1. **Const functions** promise not to modify object state
2. **Essential** for const objects to use methods
3. Declared with **const after** parameter list
4. Can **read** members but not **modify** (except mutable)
5. Can only **call other const** functions on `this` (calls on other objects are unrestricted)
6. **Const overloading** = two versions (const & non-const)
7. **Mutable** allows modification in const functions
8. **Test with const objects** to verify correctness
9. **Mark all read-only** functions const
10. **Fundamental** to const correctness in C++

---

## 🚨 THE GOLDEN RULES

```
⚠️  RULE 1: Mark ALL read-only functions const
   If it doesn't modify → make it const!
   
   class MyClass {
       int getValue() const { return value; }  // ✅
       int getBad() { return value; }          // ❌
   };

⚠️  RULE 2: const goes AFTER parameters
   Not before, not anywhere else!
   
   void func() const { }    // ✅ CORRECT
   void const func() { }    // ❌ compiles, but is NOT a const member function
   const void func() { }    // ❌ compiles, but is NOT a const member function

⚠️  RULE 3: Test with const objects
   Always verify your API works!
   
   const MyClass obj(42);
   obj.getValue();  // Must work!

⚠️  RULE 4: Use const references
   Enable const objects, avoid copies!
   
   void process(const MyClass& obj) {
       obj.getValue();  // Requires const function
   }

⚠️  RULE 5: Mutable for implementation ONLY
   Not for business logic!
   
   mutable int cacheHits;   // ✅ OK
   mutable double balance;  // ❌ BAD

   CONST CORRECTNESS = SAFER, BETTER C++ CODE!
```

---

**Previous Topic:** [18_HeaderSourceSeparation](../18_HeaderSourceSeparation/) - Header/Source Separation

**Next Topic:** [20_ConstObjects](../20_ConstObjects/) - Working with Const Objects

---

*Part of the C++ Classes and Objects series*
*Difficulty: Intermediate*
*Prerequisites: Basic class knowledge*
*CRITICAL for Const Correctness*

---

## 🧾 Notes moved from main.cpp

The long lecture output at the end of `main()` (the "COMPREHENSIVE EXPLANATION") and the printed texts inside the examples repeated this README, so they are gone from `main.cpp`. What they said that is not already above:

- **"Enables compiler optimizations / can avoid copies and locks"** was dropped. A const member function does not let the compiler assume the object stays unchanged: `const_cast` and `mutable` are legal, and a const function may modify what a pointer member points to (all tested above). The benefit is correctness and a clear API. Only objects *defined* `const` get extra treatment, for example read-only memory (from memory).
- **"Const parameters need const functions"** (the "const correctness chain"): `void display(const Person& p)` can call only `p`'s const members, so the getters of `Person` must be const. That is the reason const spreads through a code base. See the diagram "Const Propagation" above.
- **When a function should be const.** Always: getters, queries (`isEmpty`), calculations (`area`), display, comparisons, conversions. Never: setters, modifiers (`increment`, `clear`), initialisation, resource acquire/release.
- **Example 10** now runs the correct `MistakeExamples` class on a normal and on a const object. The mistake texts are in the class as comments and in "Common Mistakes" above, since a compile error cannot be shown at runtime.
- **Code fixes in `main.cpp`:**
  - `CachedCalculator::cachedResult` was never initialised in the constructor (now `0`).
  - `Date::daysInMonth()` read `days[month - 1]` even for an invalid month (for example after `setMonth(13)`); it now returns `0` for that case.
  - The unused `<memory>` include is removed.
  - The unused parameter of `printPointBad` is commented out, so `-Wall -Wextra` is clean.
- **Money output** now has two decimals (`$1000.00`) instead of `$1000`.
- **Floating-point note:** `Rectangle::isSquare()` compares two `double` values with `==`. That is fine for this demo (`10.0` against `10.0`), but in real code compare with a tolerance.
