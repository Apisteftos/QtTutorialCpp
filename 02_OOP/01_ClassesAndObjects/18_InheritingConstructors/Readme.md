# 25_InheritingConstructors - Eliminating Forwarding Boilerplate

## 📚 Overview

This folder introduces **Inheriting Constructors** - a powerful C++11 feature that allows derived classes to automatically inherit base class constructors. This eliminates the need for repetitive constructor forwarding, reduces boilerplate code, and makes inheritance hierarchies cleaner and more maintainable.

**Inheriting Constructors** = `using Base::Base;` - inherit all base constructors

This is **ESSENTIAL** for writing clean, maintainable inheritance hierarchies.

---

## 🎯 What You'll Learn

- What inheriting constructors are and why they exist
- How to use the `using Base::Base;` syntax
- What gets inherited (and what doesn't)
- Working with default member initializers
- Overriding specific inherited constructors
- Multiple inheritance scenarios
- Common patterns and use cases
- Best practices and pitfalls
- Real-world examples

---

## 📖 Concept Explanation

### What are Inheriting Constructors?

**Inheriting constructors** allow a derived class to automatically use the constructors of its base class with a simple `using` declaration. This is a C++11 feature that eliminates tedious constructor forwarding.

```cpp
class Base {
public:
    Base(int x) { }
    Base(int x, int y) { }
    Base(int x, int y, int z) { }
};

// ✅ With inheriting constructors (C++11+)
class Derived : public Base {
public:
    using Base::Base;  // One line - inherit all!
};

// All these work automatically:
Derived d1(10);
Derived d2(10, 20);
Derived d3(10, 20, 30);
```

### The Problem Without Inheriting Constructors

**Before C++11**, you had to manually forward every constructor:

```cpp
class Base {
public:
    Base(int x) { }
    Base(int x, int y) { }
    Base(int x, int y, int z) { }
};

// ❌ Pre-C++11: Manual forwarding (tedious!)
class Derived : public Base {
public:
    Derived(int x) : Base(x) { }
    Derived(int x, int y) : Base(x, y) { }
    Derived(int x, int y, int z) : Base(x, y, z) { }
};
```

**Problems:**
- Lots of boilerplate code
- Easy to forget a constructor
- Error-prone parameter forwarding
- Hard to maintain
- Doesn't scale well

### The Solution With Inheriting Constructors

```cpp
class Derived : public Base {
public:
    using Base::Base;  // ✅ One line - done!
};
```

**Benefits:**
- No boilerplate code!
- All constructors automatically available
- Easy to maintain
- Scales perfectly
- Less error-prone

### Real-World Analogy

```
Think of a franchise business:

WITHOUT INHERITING CONSTRUCTORS:
  Franchise: "Here's how to make our products"
  You: "Let me write down every recipe..."
       "Burger with fries... (copy recipe)"
       "Burger with drink... (copy recipe)"
       "Burger with fries and drink... (copy recipe)"
  ⚠️  Tedious! Error-prone!

WITH INHERITING CONSTRUCTORS:
  Franchise: "Here's how to make our products"
  You: "I'll use your recipes!" (using Franchise::recipes)
  ✅ Automatic! All recipes available!

INHERITING = Reuse all base recipes automatically
```

---

## 💻 Basic Syntax

### Simple Inheritance

```cpp
class Base {
public:
    Base(int x) { }
    Base(string s) { }
};

class Derived : public Base {
public:
    using Base::Base;  // Inherit all Base constructors
};

// Usage:
Derived d1(42);        // Uses Base(int)
Derived d2("hello");   // Uses Base(string)
```

### With Default Member Initializers

```cpp
class Base {
public:
    Base(int x) { }
};

class Derived : public Base {
private:
    string label = "Default";  // ✅ Default initializer
    int count = 0;
    
public:
    using Base::Base;  // Members initialized automatically!
};
```

### Overriding Specific Constructors

```cpp
class Derived : public Base {
public:
    using Base::Base;  // Inherit most constructors
    
    // Override specific constructor for custom behavior
    Derived(int x) : Base(x) {
        // Custom initialization logic
    }
};
```

---

## 📝 Common Use Cases

### 1. Exception Hierarchies

**Perfect use case for inheriting constructors:**

```cpp
class BaseException {
protected:
    string message;
    int errorCode;
    
public:
    BaseException(const string& msg, int code)
        : message(msg), errorCode(code) { }
    
    BaseException(const string& msg)
        : BaseException(msg, 0) { }
};

// ✅ Each exception type inherits constructors
class NetworkException : public BaseException {
public:
    using BaseException::BaseException;  // One line!
};

class DatabaseException : public BaseException {
public:
    using BaseException::BaseException;  // One line!
};

class FileException : public BaseException {
public:
    using BaseException::BaseException;  // One line!
};

// Usage:
throw NetworkException("Connection timeout", 404);
throw DatabaseException("Query failed");
throw FileException("File not found", 2);
```

### 2. Simple Wrapper Classes

```cpp
class Container {
public:
    Container(size_t size) { }
    Container(initializer_list<int> init) { }
};

// Simple wrapper adds functionality without storage
class NamedContainer : public Container {
private:
    string name = "Unnamed";  // Default initializer
    
public:
    using Container::Container;  // ✅ Inherit all constructors
    
    void setName(const string& n) { name = n; }
    string getName() const { return name; }
};

// Usage:
NamedContainer c1(10);           // Uses Container(size_t)
NamedContainer c2{1, 2, 3, 4};   // Uses Container(initializer_list)
c1.setName("Numbers");
```

### 3. Type-Safe ID Wrappers

```cpp
template<typename Tag>
class ID {
protected:
    int value;
    
public:
    explicit ID(int v) : value(v) { }
    int getValue() const { return value; }
};

struct UserTag {};
struct OrderTag {};

class UserID : public ID<UserTag> {
public:
    using ID::ID;  // ✅ Inherit constructor
};

class OrderID : public ID<OrderTag> {
public:
    using ID::ID;  // ✅ Inherit constructor
};

// Type-safe - can't mix UserID and OrderID
void processUser(UserID uid);
void processOrder(OrderID oid);

UserID user(123);
OrderID order(456);
processUser(user);   // ✅ OK
// processUser(order); // ❌ Error - type mismatch!
```

### 4. Configuration Classes

```cpp
class BaseConfig {
protected:
    string filename;
    string mode;
    
public:
    BaseConfig(const string& file, const string& m)
        : filename(file), mode(m) { }
};

class ProductionConfig : public BaseConfig {
public:
    using BaseConfig::BaseConfig;  // ✅ Inherit
    
    ProductionConfig()
        : BaseConfig("prod.cfg", "production") { }
};

class DevelopmentConfig : public BaseConfig {
public:
    using BaseConfig::BaseConfig;  // ✅ Inherit
    
    DevelopmentConfig()
        : BaseConfig("dev.cfg", "development") { }
};
```

### 5. Policy-Based Design

```cpp
template<typename Policy>
class Algorithm : public Policy {
public:
    using Policy::Policy;  // ✅ Inherit policy constructors
    
    void execute() {
        Policy::doWork();
    }
};

class FastPolicy {
public:
    FastPolicy(int level) { }
protected:
    void doWork() { /* fast implementation */ }
};

class SlowPolicy {
public:
    SlowPolicy(int level) { }
protected:
    void doWork() { /* slow but accurate */ }
};

Algorithm<FastPolicy> fast(5);
Algorithm<SlowPolicy> slow(10);
```

---

## 📋 Examples in This Folder

### Example 1: The Problem
Shows tedious manual constructor forwarding.
- Every base constructor needs a hand-written forwarding constructor; easy to forget one; change the base, update all

### Example 2: The Solution
Demonstrates inheriting constructors eliminating boilerplate.
- One line, `using BaseGood::BaseGood;`, makes all base constructors available; the derived `category` member keeps its default initializer

### Example 3: Wrapper Class
Simple wrapper with inherited constructors.
- Perfect for simple wrappers: minimal code, all base functionality available
- New in the demo: `NamedContainer c3{5, 10};` creates **two** elements (5 and 10), because braces prefer the `initializer_list` constructor

### Example 4: Additional Members
Using default member initializers with inherited constructors.
- Inherited constructors apply the derived default member initializers (`radius = 1.0`); you can add Circle-specific constructors

### Example 5: Overriding
Selectively overriding specific constructors.
- Inherit most constructors, replace one with the same signature for custom behavior

### Example 6: Multiple Inheritance
Inheriting from multiple base classes.
- Was misleading: the own `MultiDerived(int)` / `MultiDerived(const string&)` hid the inherited ones, so nothing was inherited. Now the two inherited constructors are really used, each leaving the other base default-constructed

### Example 7: What Isn't Inherited
Default, copy, move constructors not inherited.
- The compiler generates the derived default / copy / move constructors, which call the base versions

### Example 8: Private Constructors
Only public/protected constructors inherited.
- Access is that of the base constructor. Was mislabelled: the old demo used the (never inherited) default constructor. Now it uses a public constructor with a parameter and a `Leaf` class that uses the protected one

### Example 9: Exception Classes
Real-world exception hierarchy.
- Minimal code per exception type, all base constructors available, consistent interface; new: thrown as the derived type and caught by base reference

### Example 10: When Not to Use
Cases where explicit constructors are better.
- Was only printed text; now two real demos: an inheriting constructor cannot validate (`UncheckedAccount(-50)` keeps -50), and the initialization order (base first, then derived members)

---

## 🎓 Key Concepts

### What Gets Inherited?

**✅ INHERITED:**
- All other base constructors, with any parameter types and any number of parameters
- Template constructors ✅ tested
- `explicit` stays `explicit`: with `explicit Base(int)`, `Derived d = 1;` is an error ✅ tested
- `= delete`d base constructors are inherited as deleted ✅ tested

**Access:** the inherited constructor has the **access of the base constructor**; the place of the `using` declaration does not matter (a `using` in a `private:` section still leaves public constructors public ✅ tested).
- public → usable everywhere
- protected → not usable from outside (`error: 'B::B(int, bool)' is protected within this context`), but a further derived class can use it in its initializer list (Example 8, `Leaf`) ✅ tested
- private → unusable (`is private within this context`) ✅ tested

**❌ NOT INHERITED:**
- **Default constructor** - compiler generates one for derived (only if derived declares no constructor of its own)
- **Copy constructor** - compiler generates one for derived
- **Move constructor** - compiler generates one for derived

Copy/move *assignment* operators are not constructors; `using Base::Base;` does not touch them (the derived class gets its own implicit ones).

### Critical: Default Member Initializers

An inherited constructor only knows about the base class, so the derived members are **default-initialized**: class types call their default constructor (a `string` is empty), but scalars such as `int` stay **indeterminate**. GCC warns `'d.Ds::id' is used uninitialized` when you read one. ✅ tested (the rule is from the standard; no sanitizer for uninitialized reads was available here). Give scalar members default initializers:

```cpp
// ❌ BAD - Uninitialized members!
class Bad : public Base {
    string name;   // empty string (default-constructed) - fine
    int id;        // ⚠️  Indeterminate!
public:
    using Base::Base;
};

// ✅ GOOD - Use default initializers
class Good : public Base {
    string name = "Unknown";  // ✅ Initialized
    int id = 0;               // ✅ Initialized
public:
    using Base::Base;
};
```

**Order of initialization:**
- The base class is constructed **first** (the inherited `Base(...)`)
- Then the derived members, in declaration order, using their default member initializers
- Then the body of a derived constructor, if any

✅ tested: `OrderBase(int)` is printed before `Noisy member: default member initializer` (Example 10).

### Rules and Restrictions

**RULE 1: Only public/protected constructors inherited**
```cpp
class Base {
private:
    Base(int);         // ❌ Not inherited
protected:
    Base(double);      // ✅ Inherited (stays protected)
public:
    Base(string);      // ✅ Inherited (stays public)
};
```

**RULE 2: Can override specific constructors**
```cpp
class Derived : public Base {
public:
    using Base::Base;           // Inherit all
    Derived(int x) : Base(x) {  // Override one
        // Custom logic
    }
};
```

A derived constructor with the **same signature hides** the inherited one (Example 5: `Derived(int, bool)` replaces the inherited `Base(int, bool)`). ✅ tested

**RULE 3: Works with multiple inheritance**
```cpp
class Derived : public Base1, public Base2 {
public:
    using Base1::Base1;  // Inherit from Base1
    using Base2::Base2;  // Inherit from Base2
};
```

Caveats (all ✅ tested, Example 6):
- An inherited constructor initializes only **its** base; the other base is default-constructed. If that base has no default constructor, the inherited constructor is deleted (`use of deleted function 'M1::M1(int) [inherited from B1]'`).
- If both bases have a constructor with the same signature, calling it is ambiguous (`call of overloaded 'M2(int)' is ambiguous`).
- To initialize both bases, write your own constructor.

---

## ⚠️ Common Mistakes

### ❌ Mistake 1: Uninitialized Members

**Bad:**
```cpp
class Bad : public Base {
    string name;  // fine: empty string
    int id;       // ⚠️  Indeterminate when using inherited constructors!
public:
    using Base::Base;
};
```

**Fix:**
```cpp
class Good : public Base {
    string name = "Default";  // ✅ Use default initializer
    int id = 0;
public:
    using Base::Base;
};
```

### ❌ Mistake 2: Expecting Validation

**Bad:**
```cpp
class Bad : public Base {
public:
    using Base::Base;  // ⚠️  Can't add validation!
};
```

**Why it's bad:** You can't intercept parameters to validate them.

**Fix:**
```cpp
class Good : public Base {
public:
    Good(int x) : Base(x) {
        if (x < 0) throw invalid_argument("x must be >= 0");
    }
};
```

### ❌ Mistake 3: Forgetting Special Members Aren't Inherited

**Remember:**
- Default constructor is NOT inherited
- Copy constructor is NOT inherited
- Move constructor is NOT inherited
- These are compiler-generated for derived class
- If the derived class declares **any** constructor of its own, it gets no implicit default constructor, and the base's default constructor is not inherited either: `Derived d;` fails with `no matching function for call to 'D5::D5()'` ✅ tested

### ❌ Mistake 4: Complex Initialization Without Default Initializers

**Bad:**
```cpp
class NoDefault { public: NoDefault(int) {} };

class Bad : public Base {
    NoDefault member;  // ⚠️  No default constructor and no initializer!
public:
    using Base::Base;
};

// Bad b(1);  // ❌ error: use of deleted function 'Bad::Bad(int) [inherited from Base]'
```
✅ tested

**Fix:** Give the member a default member initializer (`NoDefault member{0};`) or write explicit constructors instead.

---

## 🎯 Practice Exercises

### Exercise 1: Exception Hierarchy
Create a hierarchy of exception classes using inheriting constructors.

### Exercise 2: Wrapper Class
Implement a wrapper around std::vector with additional functionality.

### Exercise 3: Type-Safe IDs
Create UserID, ProductID, OrderID classes that can't be mixed.

### Exercise 4: Find the Bug
Given code with inheriting constructors, identify initialization issues.

### Exercise 5: Policy Pattern
Implement policy-based design with inheriting constructors.

---

## 🚀 How to Run

### Compile:
```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o inheriting_constructors
```

(`<print>` / `std::println` needs GCC 14 or newer.)

### Run:
```bash
./inheriting_constructors
```

### Expected Output:
The program demonstrates:
1. The problem with manual forwarding
2. The solution with inheriting constructors
3. Simple wrapper classes
4. Working with additional members
5. Overriding specific constructors
6. Multiple inheritance
7. What isn't inherited
8. Private constructor handling
9. Real-world exception hierarchy
10. When not to use inheriting constructors

---

## 📊 Visual Concepts

### Inheriting Constructors Flow

```
WITHOUT INHERITING CONSTRUCTORS
┌──────────────────────────────────┐
│ Base:                            │
│ ├─ Base(int)                     │
│ ├─ Base(int, int)                │
│ └─ Base(int, int, int)           │
└──────────────────────────────────┘
         │
         │ Manual forwarding required
         ↓
┌──────────────────────────────────┐
│ Derived:                         │
│ ├─ Derived(int) : Base(int) { }  │
│ ├─ Derived(int,int) : Base...    │
│ └─ Derived(int,int,int) : Base...│
└──────────────────────────────────┘
    ⚠️  Boilerplate code!

WITH INHERITING CONSTRUCTORS
┌──────────────────────────────────┐
│ Base:                            │
│ ├─ Base(int)                     │
│ ├─ Base(int, int)                │
│ └─ Base(int, int, int)           │
└──────────────────────────────────┘
         │
         │ using Base::Base;
         ↓
┌──────────────────────────────────┐
│ Derived:                         │
│ ✅ All Base constructors          │
│    automatically available!      │
└──────────────────────────────────┘
    ✅ One line!
```

### What Gets Inherited

```
BASE CLASS CONSTRUCTORS
┌─────────────────────────────────────┐
│ private:                            │
│   Base(int)          ❌ Not inherited│
├─────────────────────────────────────┤
│ protected:                          │
│   Base(double)       ✅ Inherited    │
│                      (stays protected│
├─────────────────────────────────────┤
│ public:                             │
│   Base(string)       ✅ Inherited    │
│   Base(int, int)     ✅ Inherited    │
│                                     │
│   Base()             ❌ Special       │
│   Base(const Base&)  ❌ Special       │
│   Base(Base&&)       ❌ Special       │
└─────────────────────────────────────┘

SPECIAL MEMBERS = Compiler-generated for derived class
```

### Member Initialization Order

```
WITH INHERITING CONSTRUCTORS

Derived d(42);  // Using inherited Base(int)

Execution Order:
┌─────────────────────────────────────┐
│ 1. Base(int) constructor executes   │
│    Base members initialized         │
├─────────────────────────────────────┤
│ 2. Derived member default           │
│    initializers execute             │
│    string name = "Default";         │
│    int id = 0;                      │
├─────────────────────────────────────┤
│ 3. Derived constructor body         │
│    (if it has one)                  │
└─────────────────────────────────────┘

KEY: The base class is constructed FIRST, then the derived members!
```

---

## 📚 Related Topics

### Prerequisites:
- **Inheritance basics** - base and derived classes
- **02_Constructor** - Constructor concepts
- **24_DelegatingConstructors** - Constructor delegation

### Coming Next:
- **26_CopyConstructor** - Deep copy concepts

Already covered in other folders: **08_MoveSemantics**, **14_VirtualFunctions**.

### Related Concepts:
- **using Declaration** - Name introduction
- **Constructor Delegation** - Calling other constructors
- **CRTP** - Curiously Recurring Template Pattern

---

## 🎓 Summary

### What We Learned:

✅ **Inheriting constructors** with `using Base::Base;`  
✅ **Eliminates boilerplate** forwarding code  
✅ **One line** inherits all base constructors  
✅ **Default/copy/move** NOT inherited  
✅ **Default member initializers** for derived scalars  
✅ **Can override** specific constructors  
✅ **Perfect for** wrappers and exceptions  
✅ **Available since C++11**  
✅ **Reduces maintenance** burden  
✅ **Makes code** cleaner and safer

### The Complete Pattern:

```cpp
// Exception hierarchy with inheriting constructors
class BaseException {
protected:
    string message;
    int code;
    
public:
    BaseException(const string& msg, int c)
        : message(msg), code(c) { }
    
    BaseException(const string& msg)
        : BaseException(msg, 0) { }
    
    virtual void display() const {
        cout << "Error [" << code << "]: " << message << "\n";
    }
    
    virtual ~BaseException() = default;
};

// ✅ Each derived exception uses inheriting constructors
class NetworkException : public BaseException {
public:
    using BaseException::BaseException;
    
    void display() const override {
        cout << "Network Error [" << code << "]: " << message << "\n";
    }
};

class DatabaseException : public BaseException {
public:
    using BaseException::BaseException;
    
    void display() const override {
        cout << "Database Error [" << code << "]: " << message << "\n";
    }
};

// Usage - clean and simple!
throw NetworkException("Timeout", 408);
throw DatabaseException("Connection failed", 500);
```

---

## ✨ Quick Reference Card

```cpp
// === BASIC SYNTAX ===
class Derived : public Base {
public:
    using Base::Base;  // Inherit all base constructors
};

// === WITH DEFAULT MEMBERS ===
class Derived : public Base {
    string name = "Default";  // Default initializer
    int count = 0;              // Required for scalars
public:
    using Base::Base;
};

// === OVERRIDE SPECIFIC ===
class Derived : public Base {
public:
    using Base::Base;           // Inherit most
    Derived(int x) : Base(x) {  // Override one
        // Custom logic
    }
};

// === MULTIPLE INHERITANCE ===
class Derived : public Base1, public Base2 {
public:
    using Base1::Base1;
    using Base2::Base2;
};

// === WHAT'S INHERITED ===
✅ Public constructors → public in derived
✅ Protected constructors → protected in derived
❌ Private constructors → not inherited
❌ Default constructor → compiler-generated
❌ Copy/move constructors → compiler-generated
```

---

## 📝 Best Practices

### ✅ DO:

- **Use for simple wrappers** without state
- **Use for exception hierarchies**
- **Use default member initializers** for derived members
- **Override specific constructors** when needed
- **Document** that constructors are inherited
- **Test** all inherited constructors
- **Use for policy-based** designs
- **Keep it simple** - perfect for minimal derived classes
- **Combine with virtual functions** when appropriate
- **Use for type-safe wrappers**

### ❌ DON'T:

- **Don't use with uninitialized members**
- **Don't expect** parameter validation
- **Don't use** when complex initialization needed
- **Don't forget** default member initializers
- **Don't use** just to save typing if logic needed
- **Don't use** with resource management differences
- **Don't forget** special members aren't inherited
- **Don't use** when constructors need different behavior
- **Don't overuse** - write explicit when clearer
- **Don't mix** with complex initialization patterns

---

## 🌟 Real-World Applications

### Exception Hierarchies
```cpp
class AppException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;   // what() comes for free
};

class IOError : public AppException {
public:
    using AppException::AppException;
};

class NetworkError : public AppException {
public:
    using AppException::AppException;
};
```

### Type-Safe Handles
```cpp
template<typename T>
class Handle {
    T* ptr;
public:
    explicit Handle(T* p) : ptr(p) { }
};

class FileHandle : public Handle<FILE> {
public:
    using Handle::Handle;
};

class SocketHandle : public Handle<int> {
public:
    using Handle::Handle;
};
```

### Configuration Presets
```cpp
class Config {
public:
    Config(const string& file, const string& env);
};

class DevConfig : public Config {
public:
    DevConfig() : Config("dev.cfg", "development") { }
    using Config::Config;  // Also allow custom
};

class ProdConfig : public Config {
public:
    ProdConfig() : Config("prod.cfg", "production") { }
    using Config::Config;  // Also allow custom
};
```

---

## 📖 Further Reading

- [cppreference.com - Inheriting constructors](https://en.cppreference.com/cpp/language/using_declaration)
- [C++ Core Guidelines - C.52: Use inheriting constructors to import constructors into a derived class that does not need further explicit initialization](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rc-inheriting)
- [MSDN - Inheriting Constructors](https://learn.microsoft.com/en-us/cpp/cpp/constructors-cpp)

---

## 🎯 Key Takeaways

1. **`using Base::Base;`** inherits all base constructors
2. **Eliminates forwarding** boilerplate completely
3. **Only public/protected** constructors inherited
4. **Default/copy/move** NOT inherited (special members)
5. **Use default initializers** for derived scalar members
6. **Can override** specific constructors when needed
7. **Perfect for** wrappers and exception hierarchies
8. **Available since C++11**
9. **Reduces maintenance** significantly
10. **Best for classes** without complex initialization

---

## 🚨 THE GOLDEN RULES

```
⚠️  RULE 1: Use default member initializers
   Derived scalar members need initializers!
   
   ❌ int count;                // Indeterminate!
   ✅ int count = 0;            // Initialized

⚠️  RULE 2: Perfect for simple wrappers
   Best when derived adds minimal/no state
   
   Exception hierarchies ✅
   Simple wrappers ✅
   Complex initialization ❌

⚠️  RULE 3: Override when needed
   Can selectively override specific constructors
   
   using Base::Base;           // Inherit most
   Derived(int x) : Base(x) {} // Override one

⚠️  RULE 4: Special members not inherited
   Default, copy, move are compiler-generated
   
   Inherited:     Base(int), Base(string), etc.
   NOT inherited: Base(), Base(const Base&), etc.

⚠️  RULE 5: Can't validate parameters
   If you need validation, write explicit constructors
   
   Inheriting = automatic forwarding
   No validation possible!

   USE WISELY - ELIMINATE BOILERPLATE!
```

---

**Previous Topic:** [24_DelegatingConstructors](../24_DelegatingConstructors/) - Delegating Constructors  
**Next Topic:** [26_CopyConstructor](../26_CopyConstructor/) - Copy Constructor

---

*Part of the C++ Classes and Objects series*  
*Difficulty: Intermediate*  
*Prerequisites: Inheritance, constructors*  
*ESSENTIAL for Clean Inheritance Hierarchies*
---

## 🧾 Notes moved from main.cpp

- **Comparison with alternatives:**
  - *Manual forwarding (pre-C++11):* full control, can customize; but boilerplate and a maintenance burden.
  - *Inheriting constructors (C++11+):* concise, automatic, maintainable; less control, derived scalars need default initializers.
  - *Hybrid:* `using Base::Base;` plus own constructors for the special cases; best of both, slightly more complex.
- **Perfect for:** exception hierarchies, simple wrapper classes, tag/marker classes, derived classes without state, policy-based designs, type-safe wrappers.
- **Not suitable for:** classes whose extra members need non-trivial initialization, when parameters must be validated, when constructors need different behavior, or when the class manages resources differently.
- **Decision tree:**
  - Does the derived class add new data members? No → use inheriting constructors.
  - Can the members use default initializers? Yes → use inheriting constructors.
  - Need custom initialization logic or parameter validation? Yes → write explicit constructors. No → use inheriting constructors.
- **Corrected comparison table:**

| Aspect | Manual | Inheriting |
|---|---|---|
| Code amount | High | Minimal |
| Maintenance | Harder | Easier |
| Forgetting a constructor | Possible | Not possible |
| Custom logic | Easy | Write an own constructor |
| Validation | Easy | Write an own constructor |
| Flexibility | Full | Good |

  The old table said "Error-prone: Yes / No" and "Scalability: Poor / Excellent"; I toned these down because they are opinions, not measured facts.
- **Code changes in main.cpp:**
  - Example 6: the demo did not demonstrate inheriting at all (see above). `Base1` and `Base2` got default constructors, which the inherited constructors need.
  - Example 8: see above. Example 9 got the throw/catch part. Example 10 is real code now.
  - Example 7: unused parameter names removed (`-Wunused-parameter`); added `<initializer_list>` and `<utility>`.
  - Values print with `{:g}`, so they look the same as with `cout`.
  - ✅ tested with `-Wall -Wextra`, AddressSanitizer and UndefinedBehaviorSanitizer.
