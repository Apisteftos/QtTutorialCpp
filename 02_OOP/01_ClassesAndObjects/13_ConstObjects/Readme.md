# 20_ConstObjects - Immutability and Safety in C++

## 📚 Overview

This folder introduces **Const Objects** - objects that cannot be modified after initialization. Const objects are fundamental to writing safe, thread-safe, and maintainable C++ code. They work hand-in-hand with const member functions to enforce immutability at compile-time.

**Const Object** = Object declared with `const` that cannot be modified

This is **ESSENTIAL** knowledge for const correctness and professional C++ development.

---

## 🎯 What You'll Learn

- What const objects are and why they're important
- How to create and initialize const objects
- Restrictions and capabilities of const objects
- Const references (the most common usage!)
- Different types of const pointers
- Const objects in containers
- Const objects and constructors
- Thread safety with const objects
- Immutability patterns
- Best practices and common patterns

---

## 📖 Concept Explanation

### What are Const Objects?

**Const objects** are objects declared with the `const` keyword that cannot be modified after initialization. They can only call const member functions and provide compile-time guarantees of immutability.

```cpp
const Point p(10, 20);  // Const object
p.getX();   // ✅ OK if getX() is const
p.setX(30); // ❌ ERROR - can't modify const object
```

**The Golden Rule:**
> Make objects const when they shouldn't change!

### Why Are They Critical?

**1. Prevent Accidental Modification**
```cpp
const Config settings("app.conf");
// settings.modify();  // ❌ Compiler prevents bugs!
```

**2. Express Intent**
```cpp
const double PI = 3.14159;  // "This never changes" (for compile-time constants prefer constexpr)
```

**3. Thread Safety**
```cpp
const SharedData data(42);
// Multiple threads can read it at the same time:
// nothing can write to it, so there is no data race
```
**Careful:** this holds for an object that is *defined* `const` and has no `mutable` members. A `const&` to a **non-const** object gives no such guarantee, because another thread can still change the original. Tested with ThreadSanitizer: two threads reading a really const object → no report; one thread calling `set()` on the original while another reads it through a `const S&` → *data race* reported.

**4. Enable Const References**
```cpp
void process(const MyClass& obj) {
    // Can only call const functions
    // Most common usage pattern!
}
```

### Real-World Analogy

```
Think of a museum exhibit:

REGULAR OBJECT = Interactive Display
  - You can touch it
  - You can press buttons
  - You can change settings
  - Full interaction allowed

CONST OBJECT = Glass Case Display
  - You can look at it (read)
  - You can study it (query)
  - You CANNOT touch it (no modification)
  - You CANNOT change it (immutable)
  - Protected from accidental damage
```

### Visual Representation

```
REGULAR OBJECT              CONST OBJECT
┌──────────────────┐        ┌──────────────────┐
│  MyClass obj     │        │ const MyClass o  │
│  ──────────────  │        │  ──────────────  │
│                  │        │                  │
│  Read ✅         │        │  Read ✅         │
│  Write ✅        │        │  Write ❌        │
│  Modify ✅       │        │  Modify ❌       │
│  Call any func ✅│        │  Only const ✅   │
└──────────────────┘        └──────────────────┘
```

---

## 💻 Basic Syntax

### Creating Const Objects

```cpp
class Point {
private:
    int x, y;
    
public:
    Point(int x, int y) : x(x), y(y) {}
    
    // Const functions
    int getX() const { return x; }
    int getY() const { return y; }
    void display() const {
        cout << "(" << x << ", " << y << ")\n";
    }
    
    // Non-const functions
    void setX(int x) { this->x = x; }
    void move(int dx, int dy) {
        x += dx;
        y += dy;
    }
};

// Various ways to create const objects
const Point p1(10, 20);           // Constructor
const Point p2 = Point(30, 40);   // Copy initialization
const Point p3{50, 60};           // Uniform initialization (C++11)
```

### Using Const Objects

```cpp
const Point p(10, 20);

// ✅ Can call const functions
int x = p.getX();
int y = p.getY();
p.display();

// ❌ Cannot call non-const functions
// p.setX(30);      // Compilation error!
// p.move(5, 5);    // Compilation error!
```

### Const References (Most Important!)

```cpp
// ✅ BEST PRACTICE: Pass by const reference
void printPoint(const Point& p) {
    p.display();  // OK - const function
    // p.setX(10);  // Error - non-const function
}

// Usage
Point p(10, 20);
printPoint(p);                    // Regular object
printPoint(Point(30, 40));        // Temporary object
```

---

## 🔍 The Most Common Usage: Const References

### Why Const References are Everywhere

**The Problem:**
```cpp
// ❌ Pass by value - expensive copy!
void process(string s) {
    cout << s << endl;
}

string bigString = "lots of text...";
process(bigString);  // Copies entire string!
```

**The Solution:**
```cpp
// ✅ Pass by const reference - no copy!
void process(const string& s) {
    cout << s << endl;
}

string bigString = "lots of text...";
process(bigString);  // No copy, can't modify
```

**Benefits:**
- ✅ No copying (efficient)
- ✅ Can't modify (safe)
- ✅ Works with temporaries
- ✅ Standard C++ pattern

**Golden Rule:**
> Use `const Type&` for ALL function parameters unless you need to modify or it's cheap to copy (like int, char, bool)

**Things worth knowing (all tested):**
- A non-const reference cannot take a temporary: *cannot bind non-const lvalue reference of type 'T&' to an rvalue of type 'T'*. `const T&` can.
- `const T& r = T("x");` keeps the temporary alive until `r` goes out of scope (the destructor runs at the end of the block, not at the end of the statement).
- `const&` is a read-only **view**, not immutability: `int x = 1; const int& r = x; x = 7;` makes `r` read `7`.
- If the function needs its own copy anyway, taking the parameter by value (and moving) is the usual alternative (from memory, Core Guidelines F.16/F.18).

---

## 📋 Examples in This Folder

### Example 1: Basic Const Objects
Introduction to const objects and their restrictions.
- A normal `Point` can be read and changed; a `const Point` can only be read through const functions

### Example 2: Const Object Initialization
Different ways to initialize const objects.
- `()`, `= Point(...)` and `{}` all work; with C++17 the `=` form builds the object in place without a copy
- A const object can't be left uninitialized and can't be modified afterwards

### Example 3: Const References
The most common usage pattern in C++.
- `printBook(const Book&)` accepts a variable and a temporary; the by-value and non-const-reference versions are there for comparison only

### Example 4: Const Objects in Containers
Working with const vectors and collections.
- A `const vector<Color>`: no `push_back`, elements are const; loop with `const Color&`

### Example 5: Const Pointers
Four variations of const with pointers.
- Pointer to const, const pointer, const pointer to const, and what each allows

### Example 6: Const Objects and Constructors
How constructors work with const objects.
- A `const` data member (`maxConnections`) must be set in the initializer list; this is different from a const object

### Example 7: Const Objects and Lifetime
Understanding scope and destruction.
- Constructors and destructors run as for any object; the temporary `Timer("TempTimer")` is destroyed at the end of the statement

### Example 8: Const Objects in Functions
Parameters, returns, and local objects.
- `const Rectangle&` parameter, a `const` local object, and a function that returns by value (not `const Rectangle`, see Notes)

### Example 9: Const Objects and Thread Safety
How const enables safe concurrent access.
- Two `std::thread`s read one `const SharedData`; each stores what it read. (This demo is new and replaces the printed explanation.)

### Example 10: Real-World Example
Immutable Date class pattern.
- `nextDay()` returns a NEW `ImmutableDate`; the original stays unchanged

---

## 🎓 Key Concepts

### Const Object Rules

**MUST:**
- ✅ Be initialized when declared
- ✅ Only call const member functions
- ✅ Have all operations read-only

**CANNOT:**
- ❌ Be modified after initialization
- ❌ Call non-const member functions
- ❌ Have members changed
- ❌ Be default-initialized, unless the class has a user-provided default constructor (or default member initializers)

```cpp
const Point p(10, 20);  // ✅ Must initialize
const Point p;          // ❌ Error - Point has no default constructor
p.getX();              // ✅ OK - const function
p.setX(30);            // ❌ Error - non-const function
```

### Const References vs Pass by Value

```cpp
class LargeObject {
    int data[1000];
    // ...
};

// ❌ BAD: Expensive copy
void processBad(LargeObject obj) {
    // Copies 1000 integers!
}

// ✅ GOOD: No copy, can't modify
void processGood(const LargeObject& obj) {
    // No copy, efficient and safe!
}

// Also OK: Non-const if you need to modify
void processModify(LargeObject& obj) {
    // Can modify, but can't accept temporaries
}
```

### Const Pointers - Four Variations

```cpp
int value = 42;
int other = 100;

// 1. Pointer to const (can't modify data)
const int* ptr1 = &value;
*ptr1 = 50;     // ❌ Error
ptr1 = &other;  // ✅ OK

// 2. Const pointer (can't change pointer)
int* const ptr2 = &value;
*ptr2 = 50;     // ✅ OK
ptr2 = &other;  // ❌ Error

// 3. Const pointer to const (both const)
const int* const ptr3 = &value;
*ptr3 = 50;     // ❌ Error
ptr3 = &other;  // ❌ Error

// 4. Reference to const
const int& ref = value;
ref = 50;       // ❌ Error
```

**Memory Trick:** Read right to left!
- `const int* ptr` → ptr is a pointer to const int
- `int* const ptr` → ptr is a const pointer to int

### Const Members vs Const Objects

**Const Member Variable:**
```cpp
class Config {
    const int maxSize;  // This member is const
    int currentSize;    // This member can change
    
public:
    Config(int m) : maxSize(m), currentSize(0) {}
    void increment() { currentSize++; }  // OK
};

Config c(100);  // Non-const object
c.increment();  // ✅ OK - object not const
```

**Const Object:**
```cpp
const Config c(100);  // Entire object is const
// c.increment();     // ❌ Error - object is const
// Can only call const functions
```

### Immutability Pattern

Create truly immutable objects:

```cpp
class ImmutablePoint {
    const int x, y;  // All members const!
    
public:
    ImmutablePoint(int x, int y) : x(x), y(y) {}
    
    // All functions naturally const
    int getX() const { return x; }
    int getY() const { return y; }
    
    // Operations return NEW objects
    ImmutablePoint move(int dx, int dy) const {
        return ImmutablePoint(x + dx, y + dy);
    }
};

// Usage
const ImmutablePoint p1(10, 20);
const ImmutablePoint p2 = p1.move(5, 5);
// p1 unchanged, p2 is new object!
```

**Benefits:**
- Impossible to modify (all members const)
- Thread-safe by design
- Functional programming style
- No side effects

**Caveat (tested):** a class with `const` data members cannot be assigned.
```cpp
struct D { const int y; D(int v) : y(v) {} };
D a(1), b(2);
a = b;                          // ❌ use of deleted function 'D& D::operator=(const D&)'
std::vector<D> v;               // push_back works, but v.erase(...) fails:
                                // use of deleted function 'D& D::operator=(D&&)'
```
Moving also degrades to copying for const members. The C++ Core Guidelines (C.12) advise against const data members for this reason (from memory, not checked here). A simpler way to get an immutable class: keep the members private and non-const, offer no setters, and make the operations const functions that return new objects. Then `const ImmutableDate d(...)` gives you the constness.

---

## ⚠️ Common Mistakes

### ❌ Mistake 1: Forgetting to Initialize

**Bad:**
```cpp
const Point p;  // ❌ Error
```
GCC (tested): *no matching function for call to 'P::P()'* when the class has constructors but no default one; *uninitialized 'const x'* for a class with plain `int` members and no constructor. `const X x;` compiles if the class has a user-provided default constructor or every member has a default initializer.

**Fix:**
```cpp
const Point p(0, 0);  // ✅ Initialized
```

### ❌ Mistake 2: Missing Const on Member Functions

**Bad:**
```cpp
class Point {
public:
    int getX() { return x; }  // ❌ Not const
};

const Point p(10, 20);
// p.getX();  // ❌ Error - can't call non-const function
```

**Fix:**
```cpp
int getX() const { return x; }  // ✅ Added const
```

### ❌ Mistake 3: Passing by Value Instead of Const Ref

**Bad:**
```cpp
void process(string s) {  // ❌ Copies the string
    cout << s << endl;
}
```

**Fix:**
```cpp
void process(const string& s) {  // ✅ No copy
    cout << s << endl;
}
```

### ❌ Mistake 4: Using Const Cast

**Bad:**
```cpp
const Point p(10, 20);
Point& p2 = const_cast<Point&>(p);  // ❌ Dangerous!
p2.setX(30);  // Undefined behavior! p was DEFINED const
```
(`const_cast` on an object that was defined non-const is legal; see 19_ConstMemberFunctions. The undefined behaviour is about modifying an object that is itself const.)

**Fix:**
```cpp
// Don't cast away const!
// If you need to modify, don't make it const
Point p(10, 20);
p.setX(30);  // ✅ OK
```

### ❌ Mistake 5: Wrong Pointer Const

**Bad:**
```cpp
const int* ptr;  // Pointer to const
ptr = &other;    // ✅ Can change pointer
*ptr = 42;       // ❌ But wanted to modify data!
```

**Fix:**
```cpp
int* const ptr = &value;  // Const pointer
*ptr = 42;                // ✅ Can modify data
```

---

## 🎯 Practice Exercises

### Exercise 1: Const Objects
Create const objects of various types and verify you can only call const functions.

### Exercise 2: Const References
Write functions using const references and test with both regular and temporary objects.

### Exercise 3: Const Pointers
Practice all four variations of const with pointers.

### Exercise 4: Immutable Class
Design a class that's truly immutable (all members const).

### Exercise 5: Find the Bugs
Given code with const errors, identify and fix them.

---

## 🚀 How to Run

### Compile:
```bash
g++ -std=c++23 -Wall -Wextra -pthread main.cpp -o const_objects
```
(`main.cpp` uses `std::println` from `<print>`, so it needs GCC 14+ or another compiler with C++23 `<print>`. `-pthread` is for Example 9; newer glibc versions don't need it.)

### Run:
```bash
./const_objects
```

### Expected Output:
The program demonstrates:
1. Basic const objects and restrictions
2. Initialization methods
3. Const references (most common!)
4. Const objects in containers
5. Four types of const pointers
6. Const with constructors
7. Lifetime and scope
8. Function usage patterns
9. Thread safety benefits
10. Immutable Date class

---

## 📊 Visual Concepts

### Const Object Capabilities

```
WHAT YOU CAN DO              WHAT YOU CANNOT DO
┌──────────────────┐        ┌──────────────────┐
│ const Point p;   │        │ const Point p;   │
│                  │        │                  │
│ p.getX()     ✅  │        │ p.setX(10)   ❌  │
│ p.getY()     ✅  │        │ p.move(5,5)  ❌  │
│ p.display()  ✅  │        │ p.member = x ❌  │
│                  │        │                  │
│ (const functions)│        │ (non-const)      │
└──────────────────┘        └──────────────────┘
```

### Const Pointer Variations

```
1. Pointer to const        2. Const pointer
   const Type* ptr            Type* const ptr
   ┌──────┐   ┌──────┐       ┌──────┐   ┌──────┐
   │ ptr  │──>│ data │       │ ptr  │──>│ data │
   └──────┘   └──────┘       └──────┘   └──────┘
   Can change  Can't         Can't       Can
   pointer     modify data   change ptr  modify

3. Const ptr to const      4. Reference to const
   const Type* const ptr      const Type& ref
   ┌──────┐   ┌──────┐       ┌──────┐
   │ ptr  │──>│ data │       │ ref  │ = data
   └──────┘   └──────┘       └──────┘
   Can't       Can't         Can't modify
   change ptr  modify data   (can't rebind anyway)
```

### Pass by Value vs Const Reference

```
PASS BY VALUE                    PASS BY CONST REF
┌──────────────┐                ┌──────────────┐
│  Original    │                │  Original    │
│  Object      │                │  Object      │
└──────┬───────┘                └──────┬───────┘
       │ Copy                          │ Reference
       ↓                               ↓
┌──────────────┐                ┌──────────────┐
│  Function    │                │  Function    │
│  Parameter   │                │  Parameter   │
│  (Copy)      │                │  (Same obj)  │
└──────────────┘                └──────────────┘
  Slow, uses memory              Fast, no copy!
```

---

## 📚 Related Topics

### Prerequisites:
- **19_ConstMemberFunctions** - Const functions
- **06_GettersSetters** - Accessor methods
- **01_BasicClass** - Basic class concepts

### Coming Next:
- **21_MutableKeyword** - Mutable members
- **22_DefaultDelete** - Special functions
- **ExplicitKeyword** - Preventing conversions

### Related Concepts:
- **Const Correctness** - Design principle
- **Thread Safety** - Concurrent access
- **RAII** - Resource management

---

## 🎓 Summary

### What We Learned:

✅ **Const objects** cannot be modified after initialization
✅ **Must be initialized** when declared
✅ **Can only call** const member functions
✅ **Const references** are the most common usage
✅ **No copying** with const& parameters
✅ **Safe to read** from several threads (for a really const object without `mutable` members)
✅ **Four types** of const pointers
✅ **Compile-time safety** (no runtime cost)
✅ **Express intent** clearly
✅ **Not a speed feature**: const is about correctness

### The Complete Pattern:

```cpp
// Class with const functions
class Point {
private:
    int x, y;
    
public:
    Point(int x, int y) : x(x), y(y) {}
    
    // Const functions for const objects
    int getX() const { return x; }
    int getY() const { return y; }
    void display() const {
        cout << "(" << x << ", " << y << ")\n";
    }
    
    // Non-const modifiers
    void setX(int x) { this->x = x; }
};

// Creating const objects
const Point p1(10, 20);
p1.display();  // ✅ OK

// Const references (MOST COMMON!)
void process(const Point& p) {
    p.display();
}

process(p1);                // Regular object
process(Point(30, 40));     // Temporary

// Const pointers
Point q(1, 2);                 // a NON-const object
const Point* ptr1 = &p1;       // Pointer to const (may point to a const object)
Point* const ptr2 = &q;        // Const pointer (needs a non-const object)
const Point* const ptr3 = &p1; // Both const (must be initialised)
// Point* ptr4 = &p1;          // ❌ invalid conversion from 'const Point*' to 'Point*' (tested)

// Const in containers
const vector<Point> points = {
    Point(1, 2),
    Point(3, 4)
};

for (const Point& p : points) {
    p.display();
}
```

---

## ✨ Quick Reference Card

```cpp
// === CREATING CONST OBJECTS ===
const Type obj(args);        // Constructor
const Type obj = value;      // Copy initialization
const Type obj{args};        // Uniform (C++11)

// === USING CONST OBJECTS ===
const Point p(10, 20);
p.getX();      // ✅ const function
p.setX(30);    // ❌ non-const function

// === CONST REFERENCES (MOST IMPORTANT!) ===
void func(const Type& obj) {
    // Can read, can't modify
    // No copy, efficient
    // Most common pattern!
}

// === CONST POINTERS ===
const Type* ptr;           // pointer to const
Type* const ptr;           // const pointer
const Type* const ptr;     // both const
const Type& ref;           // const reference

// === CONST IN CONTAINERS ===
const vector<Type> vec = {...};
for (const auto& item : vec) {
    // item is const reference
}

// === IMMUTABLE PATTERN ===
class Immutable {
    const int x, y;  // All const
public:
    Immutable(int x, int y) : x(x), y(y) {}
    int getX() const { return x; }
    
    Immutable move(int dx) const {
        return Immutable(x + dx, y);
    }
};
```

---

## 🔍 Best Practices

### ✅ DO:

- **Use const& for parameters** (most important!)
- **Make objects const** when they won't change
- **Initialize const objects** immediately
- **Mark all getters const**
- **Use const for configuration** data
- **const for lookup tables**
- **const for thread-safe** shared data
- **Prefer const** over non-const
- **Test with const objects**
- **const by default** mindset

### ❌ DON'T:

- **Pass by value** when const& works
- **Forget to initialize** const objects
- **Cast away const** (const_cast)
- **Make everything const** without reason
- **Forget const** on member functions
- **Use mutable** to bypass const
- **Create unnecessary** copies
- **Mix const/non-const** inconsistently
- **Ignore const errors**
- **Use non-const** when const works

---

## 🌟 Real-World Applications

### Configuration Data
```cpp
const Config appConfig("settings.ini");
// Thread-safe, can't be modified accidentally
```

### Lookup Tables
```cpp
const vector<string> MONTHS = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};
```

### Immutable Objects
```cpp
const ImmutableDate date(2024, 1, 1);
// Functional programming style
const ImmutableDate tomorrow = date.nextDay();
```

### Function Parameters
```cpp
void processImage(const Image& img) {
    // Efficient, safe, most common pattern
}
```

---

## 📖 Further Reading

- [cppreference.com - cv (const and volatile) type qualifiers](https://en.cppreference.com/cpp/language/cv)
- "Effective C++" by Scott Meyers - Item 3
- [C++ Core Guidelines - Con.1 to Con.5](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#con-constants-and-immutability)
- [Const Correctness - Wikipedia](https://en.wikipedia.org/wiki/Const_(computer_programming))

---

## 🎯 Key Takeaways

1. **Const objects** cannot be modified after initialization
2. **Must initialize** when declared
3. **Only call const** member functions
4. **Const references** are THE most common usage
5. **No copying** with const& (efficient)
6. **Safe to read** from several threads (really const object, nothing `mutable`)
7. **Four types** of const pointers
8. **Compile-time** enforcement (no cost)
9. **Immutability** pattern for functional style
10. **Always prefer** const when possible!

---

## 🚨 THE GOLDEN RULES

```
⚠️  RULE 1: Use const& for function parameters
   Unless you need to modify or it's cheap!
   
   void process(const MyClass& obj) { }  // ✅ BEST
   void process(MyClass obj) { }         // ❌ Usually bad

⚠️  RULE 2: Const objects must be initialized
   Can't exist uninitialized!
   
   const Point p(10, 20);  // ✅ Initialized
   const Point p;          // ❌ ERROR (no default constructor)

⚠️  RULE 3: Make objects const when they won't change
   Express intent, prevent bugs!
   
   const Config settings("app.conf");  // ✅ Won't change
   Config settings("app.conf");        // ❌ Might change?

⚠️  RULE 4: All READ-ONLY member functions should be const
   For const objects to be useful!
   
   class Point {
       int getX() const { return x; }  // ✅ Usable
       int getY() { return y; }        // ❌ Not usable
   };

⚠️  RULE 5: Read pointer const right-to-left
   Avoid confusion!
   
   const int* ptr  → pointer to const int
   int* const ptr  → const pointer to int

   CONST BY DEFAULT, MUTABLE BY CHOICE!
```

---

**Previous Topic:** [19_ConstMemberFunctions](../19_ConstMemberFunctions/) - Const Member Functions

**Next Topic:** [21_MutableKeyword](../21_MutableKeyword/) - Mutable Keyword

---

*Part of the C++ Classes and Objects series*
*Difficulty: Intermediate*
*Prerequisites: Const member functions*
*CRITICAL for Const Correctness*

---

## 🧾 Notes moved from main.cpp

The long lecture output at the end of `main()` (the "COMPREHENSIVE EXPLANATION") and the printed texts inside the examples repeated this README, so they are gone from `main.cpp`. What they said that is not already above:

- **"Better optimization / compiler can optimize better / avoid defensive copies"** was dropped. A `const&` or `const*` says nothing about whether the object changes through another path (see the `const int& r` test above). Extra treatment is possible only for objects *defined* const (for example placing them in read-only memory, from memory). Const is about correctness.
- **"Return: const Type (rare, prevents modification)"** was wrong advice for class types. Tested with a small class that counts copies and moves, pushed into a `vector`: `const R make()` gave **1 copy + 1 move**, plain `R make()` gave **0 copies + 1 move**, because a const temporary cannot be moved from. For built-in types GCC warns *type qualifiers ignored on function return type* (`-Wignored-qualifiers`, tested). So `createSquare` in `main.cpp` now returns `Rectangle` by value. The *caller* can still write `const Rectangle square = createSquare(10);`, which is the useful part.
- **"Have const constructor (implicitly)"** (old lecture text) made no sense: constructors are never const. A const object is constructed by an ordinary constructor, and it becomes const once the constructor has finished.
- **"Const members vs const objects"**: a const *member* makes one field unchangeable (and the class non-assignable, see the caveat above); a const *object* makes the whole object read-only, so only const member functions can be called.
- **Example 7 comment** said "Temporary const object". `Timer("TempTimer")` is a temporary of a non-const type; calling the const `tick()` on it is just allowed.
- **Where the old text said** "Use const& for ALL function parameters": the usual wording is "for parameters that are expensive to copy and only read". Cheap types (`int`, `double`, pointers) go by value.
- **Code changes in `main.cpp`:**
  - Example 9 now starts two threads that read one `const SharedData`. It is clean under ThreadSanitizer. A writer plus a `const&` reader on a non-const object is not (see above).
  - `ImmutableDate::daysInMonth()` returns `0` for an invalid month instead of reading outside its array.
  - The unused `<memory>` include is removed and `<thread>` is added.
  - `createSquare` no longer returns `const Rectangle`.
- **Output formatting:** `GPA`, distances and areas are printed with `{:g}` to match the original `cout` output; the date uses `{:02}`.
