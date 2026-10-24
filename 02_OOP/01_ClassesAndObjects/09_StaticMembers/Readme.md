# 11_StaticMembers - Class-Level Members

## 📚 Overview

This folder introduces **Static Members** - members that belong to the class itself rather than individual objects. Static members are shared by all instances of a class and can even be accessed without creating any objects.

**Static Members** = Class-level members shared by all objects

This is a powerful feature for managing class-wide data, implementing design patterns, and creating utility classes.

---

## 🎯 What You'll Learn

- What static members are and why they exist
- Static data members (variables)
- Static member functions
- Difference between static and instance members
- Initializing static members
- Static function restrictions
- Common patterns (Singleton, Factory, Utility classes)
- Object counting and tracking
- Shared configuration
- Best practices and common mistakes

---

## 📖 Concept Explanation

### What are Static Members?

**Static members** belong to the **class**, not to individual objects.

**Key Characteristics:**
- **Shared by all objects** - only one copy exists
- **Class-level** - not object-level
- **Exist even if no objects created**
- **Accessible without an object**

### Real-World Analogy
```
Think of a school:

INSTANCE MEMBERS (each student has their own):
  - Name
  - Grade
  - Student ID
  
STATIC MEMBERS (shared by all students):
  - School name
  - School address
  - Total number of students
  - School policy

Just like the school name is the same for ALL students,
static members are the same for ALL objects of a class.
```

### Visual Representation
```
class MyClass {
    int instanceVar;     // Each object has own copy
    static int staticVar; // All objects share ONE copy
};

Memory Layout:
┌──────────────┐
│ obj1         │
│ instanceVar  │ = 10
└──────────────┘

┌──────────────┐
│ obj2         │
│ instanceVar  │ = 20
└──────────────┘

┌──────────────┐
│ obj3         │
│ instanceVar  │ = 30
└──────────────┘

┌──────────────┐
│ staticVar    │ = 100  ◄─── ONE copy for ALL objects
└──────────────┘
```

---

## 💻 Basic Syntax

### Static Data Members
```cpp
class MyClass {
private:
    int instanceVar;        // Each object has own
    static int staticVar;   // Shared by all objects
    
public:
    MyClass(int val) : instanceVar(val) {
        staticVar++;  // Increment shared counter
    }
    
    static int getStaticVar() {
        return staticVar;
    }
};

// CRITICAL: Initialize static member outside class
int MyClass::staticVar = 0;

// Usage
MyClass::staticVar = 10;        // Via class name
MyClass obj;
obj.staticVar = 20;             // Via object (also works)
int x = MyClass::getStaticVar(); // Via class name
```

### Static Member Functions
```cpp
class MyClass {
private:
    int instanceVar;
    static int staticVar;
    
public:
    // Non-static function - has 'this'
    void instanceFunction() {
        instanceVar = 10;  // ✅ Can access instance members
        staticVar = 20;    // ✅ Can access static members
    }
    
    // Static function - NO 'this'
    static void staticFunction() {
        // instanceVar = 10;  // ❌ Error! No 'this' pointer
        staticVar = 20;       // ✅ Can access static members
    }
};

// Usage - no object needed!
MyClass::staticFunction();
```

---

## 🔍 Static vs Instance Members

### Comparison Table
```
┌─────────────────────────┬─────────────────────────┐
│ INSTANCE MEMBERS        │ STATIC MEMBERS          │
├─────────────────────────┼─────────────────────────┤
│ Each object has own     │ One copy for class      │
│ Requires object         │ No object needed        │
│ Has 'this' pointer      │ No 'this' pointer       │
│ Can access all members  │ Only static members (*)  │
│ Object-level            │ Class-level             │
│ Created with object     │ Created at program start│
│ Memory per object       │ Memory once per program │
└─────────────────────────┴─────────────────────────┘
```
(*) directly. A static function can still reach instance members of an object it is given as a parameter (see the workaround below).

### Code Comparison
```cpp
class Demo {
    int instanceVar;        // Instance member
    static int staticVar;   // Static member
    
public:
    // INSTANCE FUNCTION
    void instanceFunc() {
        instanceVar = 10;   // ✅ Can access instance
        staticVar = 20;     // ✅ Can access static
        this->instanceVar;  // ✅ Has 'this' pointer
    }
    
    // STATIC FUNCTION
    static void staticFunc() {
        // instanceVar = 10;   // ❌ Error! No 'this'
        staticVar = 20;        // ✅ Can access static only
        // this->instanceVar;  // ❌ Error! No 'this'
    }
};

// USAGE
Demo obj;
obj.instanceFunc();    // Needs object
Demo::staticFunc();    // No object needed!
```

---

## 📋 Examples in This Folder

### Example 1: Basic Static Members
`Counter` counts live objects with a static counter. `displayCount()` is called on the class before any object exists, and also through an object (`c1.displayCount()`).

### Example 2: Static vs Instance Members
`Student`: name and grade belong to each object, school name and student total are shared. The school name is set before any student exists.

### Example 3: Object Counting
`Tracker` counts active, created and destroyed objects, including copies (it has a copy constructor that counts).

### Example 4: Shared Configuration
`DatabaseConnection` with a shared server address, port and connection limit. The fourth connection is refused. It now remembers whether it was established, so the destructor does not close a connection that never existed (see Mistake 7).

### Example 5: Static Const Members
`MathConstants`: `constexpr` integers in-class, `static const double` defined outside (the old rule), and functions using them.

### Example 6: Static Function Restrictions
A static function can use static members, call other static functions, and work on an object passed as a parameter (`processObject`). It cannot use instance members directly.

### Example 7: Factory Pattern
Private constructor plus static `createBasic()`, `createPremium()`, `createCustom()`. A static `nextId` hands out IDs.

### Example 8: Singleton Pattern
`Logger` with a static instance pointer, a private constructor, deleted copy operations and an explicit `cleanup()`.

### Example 9: Utility Class
`StringUtils`: all static, constructor deleted.

### Example 10: ID Generator
`Employee` with auto-incrementing IDs, a static list of recycled IDs and statistics.

---

## 🧾 Notes moved from main.cpp

### Static function: what it can and cannot do
- ✅ use static data members, call other static functions, work with objects/parameters it is given, be called without an object
- ❌ use instance members directly, call non-static functions directly, use `this`, be `const`, be `virtual`

### Student example (Example 2), in one picture
```
Each Student object has its own:      name, grade       (s1: Alice/95, s2: Bob/87, s3: Charlie/92)
ONE copy for the whole class:         schoolName ("MIT"), totalStudents (3)
```

---

## 🎓 Key Concepts

### Static Data Members

**Declaration:**
```cpp
class MyClass {
    static int count;  // Declaration only
};
```

**Definition (REQUIRED):**
```cpp
int MyClass::count = 0;  // Must initialize outside class
```

**Access:**
```cpp
// Via class name (preferred)
MyClass::count = 10;

// Via object (also works)
MyClass obj;
obj.count = 20;

// Both refer to same variable!
```

### Static Const Members

**Integral types** (int, char, bool) can be initialized in-class, but prefer `constexpr`:
```cpp
class MyClass {
    static constexpr int MAX_SIZE = 100;     // ✅ best: in-class, nothing else needed
    static constexpr double PI = 3.14159265; // ✅ constexpr also works for double
    static const char DELIMITER = ',';       // ok, but see the warning below
};
```

⚠️ **Why `constexpr`?** The in-class line of a plain `static const int` is only a *declaration with a value*, not a definition. As soon as the member is bound to a reference (**odr-used**), the linker needs a real definition. `std::println` and `std::format` take their arguments by reference, so
```cpp
struct A { static const int MAX = 1000; };
std::format("{}", A::MAX);    // ❌ linker: undefined reference to `A::MAX'   (tested, GCC -O0)
```
fails at `-O0` (an optimizer may hide the problem, so it can work with `-O2`: another reason to avoid relying on it). Fixes: `static constexpr int MAX = 1000;` (since C++17 `constexpr` static members are implicitly `inline`, so no outside definition is needed), or add `const int A::MAX;` in one `.cpp` file. Example 5 uses `constexpr` for the integers.

**Non-integral types before C++11 / the old rule** must be initialized outside:
```cpp
class MyClass {
    static const double PI;      // Declaration
    static const string NAME;    // Declaration
};

// Must initialize outside (in exactly one .cpp file)
const double MyClass::PI = 3.14159;
const string MyClass::NAME = "MyClass";
```
(`static constexpr double` can be written in-class, and so can a `static constexpr std::string_view NAME = "MyClass";`; both tested. A `std::string` member stays outside the class as above.)

### Static Member Functions
```cpp
class MyClass {
private:
    int instanceVar;
    static int staticVar;
    
public:
    // Non-static function - has 'this'
    void instanceFunction() {
        instanceVar = 10;  // ✅ Can access instance members
        staticVar = 20;    // ✅ Can access static members
    }
    
    // Static function - NO 'this'
    static void staticFunction() {
        // instanceVar = 10;  // ❌ Error! No 'this' pointer
        staticVar = 20;       // ✅ Can access static members
    }
};

// Usage - no object needed!
MyClass::staticFunction();
```

---

## 🔍 Static vs Instance Members

### Comparison Table
```
┌─────────────────────────┬─────────────────────────┐
│ INSTANCE MEMBERS        │ STATIC MEMBERS          │
├─────────────────────────┼─────────────────────────┤
│ Each object has own     │ One copy for class      │
│ Requires object         │ No object needed        │
│ Has 'this' pointer      │ No 'this' pointer       │
│ Can access all members  │ Only static members (*)  │
│ Object-level            │ Class-level             │
│ Created with object     │ Created at program start│
│ Memory per object       │ Memory once per program │
└─────────────────────────┴─────────────────────────┘
```
(*) directly. A static function can still reach instance members of an object it is given as a parameter (see the workaround below).

### Code Comparison
```cpp
class Demo {
    int instanceVar;        // Instance member
    static int staticVar;   // Static member
    
public:
    // INSTANCE FUNCTION
    void instanceFunc() {
        instanceVar = 10;   // ✅ Can access instance
        staticVar = 20;     // ✅ Can access static
        this->instanceVar;  // ✅ Has 'this' pointer
    }
    
    // STATIC FUNCTION
    static void staticFunc() {
        // instanceVar = 10;   // ❌ Error! No 'this'
        staticVar = 20;        // ✅ Can access static only
        // this->instanceVar;  // ❌ Error! No 'this'
    }
};

// USAGE
Demo obj;
obj.instanceFunc();    // Needs object
Demo::staticFunc();    // No object needed!
```

---

## 📋 Examples in This Folder

### Example 1: Basic Static Members
Understanding static data and functions.

### Example 2: Static vs Instance Members
Clear comparison of the two types.

### Example 3: Object Counting
Tracking how many objects exist.

### Example 4: Shared Configuration
Configuration shared by all objects.

### Example 5: Static Const Members
Constants shared across the class.

### Example 6: Static Function Restrictions
What static functions can and cannot do.

### Example 7: Factory Pattern
Using static methods for object creation.

### Example 8: Singleton Pattern
Ensuring only one instance exists.

### Example 9: Utility Class
Pure static functionality.

### Example 10: ID Generator
Real-world ID management with recycling.

---

## 🎓 Key Concepts

### Static Data Members

**Declaration:**
```cpp
class MyClass {
    static int count;  // Declaration only
};
```

**Definition (REQUIRED):**
```cpp
int MyClass::count = 0;  // Must initialize outside class
```

**Access:**
```cpp
// Via class name (preferred)
MyClass::count = 10;

// Via object (also works)
MyClass obj;
obj.count = 20;

// Both refer to same variable!
```

### Static Const Members

**Integral types** (int, char, bool) can be initialized in-class:
```cpp
class MyClass {
    static const int MAX_SIZE = 100;  // ✅ OK in class
    static const char DELIMITER = ','; // ✅ OK in class
};
```

**Non-integral types** must be initialized outside:
```cpp
class MyClass {
    static const double PI;      // Declaration
    static const string NAME;    // Declaration
};

// Must initialize outside
const double MyClass::PI = 3.14159;
const string MyClass::NAME = "MyClass";
```

### Static Member Functions

**Characteristics:**
- No `this` pointer
- Cannot access instance members directly (only through an object passed in)
- Can be called without an object
- Cannot be const (no `this` to be const): `static void f() const` → *error: static member function cannot have cv-qualifier* (tested)
- Cannot be virtual: *error: member 'f' cannot be declared both 'virtual' and 'static'* (tested)

**Example:**
```cpp
class MyClass {
    int instanceVar;
    static int staticVar;
    
public:
    static void staticFunc() {
        // What CAN do:
        staticVar = 10;              // ✅ Access static members
        helperStaticFunc();          // ✅ Call other static functions
        
        // What CANNOT do:
        // instanceVar = 10;         // ❌ No 'this' pointer
        // this->instanceVar = 10;   // ❌ No 'this'
        // nonStaticFunc();          // ❌ Can't call non-static
    }
    
    static void helperStaticFunc() { }
    void nonStaticFunc() { }
};
```

**Workaround for accessing instance members:**
```cpp
class MyClass {
    int instanceVar;
    static int staticVar;
    
public:
    // Pass object as parameter
    static void process(MyClass& obj) {
        obj.instanceVar = 10;  // ✅ Can access via parameter
        staticVar = 20;        // ✅ Can access static
    }
};
```

---

## 💡 Common Patterns

### 1. Object Counting
```cpp
class Counter {
    static int count;
public:
    Counter() { count++; }
    ~Counter() { count--; }
    static int getCount() { return count; }
};

int Counter::count = 0;

// Usage
Counter c1, c2, c3;
cout << Counter::getCount();  // 3
```

### 2. Singleton Pattern

**Ensures only ONE instance exists:**
```cpp
class Singleton {
private:
    static Singleton* instance;
    
    // Private constructor
    Singleton() { }
    
    // Delete copy
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    
public:
    static Singleton* getInstance() {
        if (instance == nullptr) {
            instance = new Singleton();
        }
        return instance;
    }
};

Singleton* Singleton::instance = nullptr;

// Usage
Singleton* s1 = Singleton::getInstance();
Singleton* s2 = Singleton::getInstance();
// s1 == s2 (same instance!)
```

⚠️ This classic version has two weaknesses: the `instance == nullptr` check is not thread-safe (two threads can both create an instance), and the instance is never deleted unless you add a `cleanup()` as Example 8 does. The usual modern form is a function-local static (a "Meyers singleton"):
```cpp
static Singleton& getInstance() {
    static Singleton instance;   // created on first call; C++11 guarantees this initialization is thread-safe
    return instance;             // destroyed automatically at program end
}
```
*(The thread-safe initialization is guaranteed by the C++11 standard; I did not write a race test for it.)* Singletons are also global state, so they make code harder to test; use them sparingly.

### 3. Factory Pattern

**Controlled object creation:**
```cpp
class Product {
private:
    static int nextId;
    int id;
    
    // Private constructor
    Product() : id(nextId++) { }
    
public:
    // Static factory methods
    static Product createBasic() {
        return Product();
    }
    
    static Product createPremium() {
        Product p;
        // Configure as premium
        return p;
    }
    
    int getId() const { return id; }
};

int Product::nextId = 1;

// Usage - can only create via factory
Product p1 = Product::createBasic();
Product p2 = Product::createPremium();
```

### 4. Utility Class

**Pure static functionality** (in C++ a `namespace` with free functions does the same job more simply; the class form is mostly seen in code ported from Java or C#):
```cpp
class MathUtils {
private:
    MathUtils() = delete;  // Prevent instantiation
    
public:
    static int max(int a, int b) {
        return (a > b) ? a : b;
    }
    
    static double square(double x) {
        return x * x;
    }
    
    static bool isPrime(int n) {
        if (n < 2) return false;
        for (int i = 2; i * i <= n; i++) {
            if (n % i == 0) return false;
        }
        return true;
    }
};

// Usage - no object needed
int result = MathUtils::max(5, 10);
bool prime = MathUtils::isPrime(17);
```

### 5. Shared Configuration
```cpp
class Connection {
    static string serverAddress;
    static int serverPort;
    string connectionId;
    
public:
    Connection(string id) : connectionId(id) {
        // All connections use same server config
        connect(serverAddress, serverPort);
    }
    
    static void setServer(string addr, int port) {
        serverAddress = addr;
        serverPort = port;
    }
};

string Connection::serverAddress = "localhost";
int Connection::serverPort = 8080;

// Set config once, affects all connections
Connection::setServer("db.example.com", 5432);
Connection c1("App1");  // Uses new config
Connection c2("App2");  // Uses new config
```

---

## 💡 Initialization Rules

### Basic Rule

**A non-inline static data member needs exactly one definition outside the class:**
```cpp
// In header (.h):
class MyClass {
    static int count;
    static double rate;
};

// In source (.cpp):
int MyClass::count = 0;        // REQUIRED
double MyClass::rate = 1.5;    // REQUIRED
```
Put the definition in **one** `.cpp` file, not in the header: a header definition included from two files gives a "multiple definition" linker error.

### Modern Alternative (C++17): `static inline`

```cpp
class MyClass {
    static inline int count = 0;           // ✅ defined right here, header-safe
    static inline std::vector<int> ids;    // ✅ works for any type
};
```
Tested: this compiles and links with no definition outside the class. The examples in this folder keep the classic outside definition because that is what you will see in most code.

### Initialization Order Warning
Static data members of *different* `.cpp` files are initialized in an unspecified order. If one static object's initialization uses another from a different file (a `std::vector` or `std::string` static used during start-up of another static), it may be used before it exists (the "static initialization order fiasco"). The usual fix is a function-local static (see Singleton below). *Not tested here: a well-known rule, e.g. Effective C++ Item 4.*

### Exception: Static Const Integral

**Can be initialized in-class:**
```cpp
class MyClass {
    static const int MAX = 100;        // ✅ OK
    static const char DELIMITER = ','; // ✅ OK
    static const bool FLAG = true;     // ✅ OK
    
    // But not these:
    static const double PI;            // ❌ (old rule) must init outside, or use constexpr double
    static const string NAME;          // ❌ Must init outside
};

const double MyClass::PI = 3.14159;
const string MyClass::NAME = "MyClass";
```

### Common Initialization Patterns
```cpp
class MyClass {
    static int count;
    static vector<string> names;
    static MyClass* instance;
};

// Simple initialization
int MyClass::count = 0;

// Container initialization
vector<string> MyClass::names = {"Alice", "Bob", "Charlie"};

// Pointer initialization
MyClass* MyClass::instance = nullptr;
```

---

## 💡 Common Mistakes & Solutions

### ❌ Mistake 1: Forgetting to Initialize
```cpp
class Bad {
    static int count;  // Declared
};
// Missing: int Bad::count = 0;

int main() {
    Bad::count = 10;  // ❌ Linker error: undefined reference to `Bad::count'  (tested)
}
```

**Fix:**
```cpp
class Good {
    static int count;
};
int Good::count = 0;  // ✅ Initialize outside

int main() {
    Good::count = 10;  // ✅ Works!
}
```

### ❌ Mistake 2: Initializing in Constructor
```cpp
class Bad {
    static int count;
public:
    Bad() {
        count = 0;  // ❌ Resets for EVERY object!
    }
};
```

**Fix:**
```cpp
class Good {
    static int count;
public:
    Good() {
        count++;  // ✅ Increment shared counter
    }
};
int Good::count = 0;  // ✅ Initialize once outside
```

### ❌ Mistake 3: Accessing Instance Members from Static
```cpp
class Bad {
    int value;
    static int count;
public:
    static void func() {
        value = 10;  // ❌ Error! No 'this' pointer
    }
};
```

**Fix:**
```cpp
class Good {
    int value;
    static int count;
public:
    static void func(Good& obj) {
        obj.value = 10;  // ✅ Pass object as parameter
        count = 20;      // ✅ Can access static
    }
};
```

### ❌ Mistake 4: Repeating 'static' in Definition
```cpp
class MyClass {
    static void func();
};

// ❌ Wrong (error: cannot declare member function to have static linkage, tested):
static void MyClass::func() { }

// ✅ Correct:
void MyClass::func() { }
```

### ❌ Mistake 5: Trying to Make Static Function Const
```cpp
class Bad {
    static int count;
public:
    static void func() const {  // ❌ Error: static member function cannot have cv-qualifier (tested)
        // Static functions can't be const
        // (no 'this' pointer to be const)
    }
};
```

### ❌ Mistake 6: Counting Objects But Forgetting Copies
```cpp
struct Counter {
    static int n;
    Counter()  { n++; }
    ~Counter() { n--; }
};
int Counter::n = 0;

{ Counter a; Counter b = a; }   // b is made by the implicit COPY constructor: n is not incremented
// While both are alive n == 1 instead of 2, and after the scope n == -1   (tested)
```
**Fix:** write a copy constructor that also does `n++` (Example 3's `Tracker` does exactly this).

### ❌ Mistake 7: Counting a Failed Construction Twice
A constructor that "fails" with an early `return` still creates an object, and the destructor still runs. If the destructor does `count--` but the constructor never did `count++`, the count is wrong. Example 4 had this bug (the refused `App-4` connection was "closed", lowering the count to -1 at the end). It now remembers whether the connection was established (`bool connected`). In real code, throw an exception from the constructor instead, then no object exists.

---

## 🎯 Practice Exercises

### Exercise 1: Student Registration System
Create a `Student` class with:
- Static counter for total students
- Static method to get next student ID
- Auto-increment IDs
- Display total students

### Exercise 2: Logger Singleton
Implement a `Logger` class:
- Singleton pattern
- Log to file
- Only one instance allowed
- Static getInstance() method

### Exercise 3: Math Utility Class
Create a `MathUtils` class with:
- All static methods
- max, min, average, gcd, lcm
- No instances allowed
- Test all functions

### Exercise 4: Connection Pool
Create a `Connection` class with:
- Static max connections limit
- Static current connection count
- Prevent exceeding limit
- Track all connections

---

## 🚀 How to Run

### Compile:
```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o static_members
```
`main.cpp` uses `std::println`, so it needs GCC 14 or newer (or another compiler with `<print>`). The addresses in the output are different on every run.

### Run:
```bash
./static_members
```

### Expected Output:
The program demonstrates:
1. Basic static members
2. Static vs instance comparison
3. Object counting
4. Shared configuration
5. Static const members
6. Static function restrictions
7. Factory pattern
8. Singleton pattern
9. Utility class
10. ID generator with recycling

---

## 📊 Visual Concepts

### Memory Layout
```
Three objects created:
obj1, obj2, obj3

┌─────────────────────────────────────┐
│ INSTANCE MEMBERS                    │
│ (Each object has own copy)          │
├─────────────────────────────────────┤
│ obj1: instanceVar = 10              │
│ obj2: instanceVar = 20              │
│ obj3: instanceVar = 30              │
└─────────────────────────────────────┘

┌─────────────────────────────────────┐
│ STATIC MEMBERS                      │
│ (One copy shared by all)            │
├─────────────────────────────────────┤
│ MyClass::staticVar = 100            │
│ ↑                                   │
│ All three objects share this!       │
└─────────────────────────────────────┘
```

### Access Methods
```
INSTANCE MEMBERS:
─────────────────
obj.instanceVar        ✅ Via object only
MyClass::instanceVar   ❌ Cannot via class

STATIC MEMBERS:
───────────────
MyClass::staticVar     ✅ Via class (preferred)
obj.staticVar          ✅ Via object (also works)

Both refer to same variable!
```

---

## 📚 Related Topics

### Prerequisites:
- **04_MemberFunctions** - Member functions
- **10_ThisPointer** - Understanding 'this'

### Coming Next:
- **12_FriendFunctions** - Friend functions
- **13_Inheritance** - Static in inheritance
- **16_Polymorphism** - Static vs virtual

### Related Concepts:
- **Singleton Pattern** - Uses static extensively
- **Factory Pattern** - Static factory methods
- **Global Variables** - Static members are better alternative

---

## 🎓 Summary

### What We Learned:

✅ **Static members** belong to the class, not objects
✅ **Shared by all** - only one copy exists
✅ **Static data** must be initialized outside class
✅ **Static functions** have no `this` pointer
✅ **Can access** without creating objects
✅ **Used for** counting, configuration, patterns
✅ **Essential** for Singleton, Factory patterns
✅ **Perfect for** utility classes
✅ **Class-level** not object-level
✅ **Memory efficient** - one copy for all

### The Complete Pattern:
```cpp
class MyClass {
private:
    // Instance members (each object has own)
    int instanceVar;
    
    // Static members (shared by all)
    static int staticVar;
    
public:
    // Constructor
    MyClass(int val) : instanceVar(val) {
        staticVar++;  // Access static member
    }
    
    // Instance function (has 'this')
    void instanceFunc() {
        instanceVar = 10;  // ✅ Can access instance
        staticVar = 20;    // ✅ Can access static
    }
    
    // Static function (no 'this')
    static void staticFunc() {
        // instanceVar = 10;  // ❌ Error!
        staticVar = 20;       // ✅ Can access static
    }
    
    // Static getter
    static int getStaticVar() {
        return staticVar;
    }
};

// Initialize static member (REQUIRED!)
int MyClass::staticVar = 0;

// Usage
MyClass::staticFunc();          // No object needed
int x = MyClass::getStaticVar(); // No object needed
MyClass obj(5);
obj.instanceFunc();             // Needs object
```

---

## ✨ Quick Reference Card
```cpp
// DECLARATION
class MyClass {
    static int count;              // Static data
    static void func();            // Static function
    static const int MAX = 100;    // Static const (integral)
};

// DEFINITION (outside class - REQUIRED)
int MyClass::count = 0;

void MyClass::func() {
    // Implementation
    // NO 'static' keyword here!
}

// ACCESS
MyClass::count = 10;      // Via class name
MyClass::func();          // Via class name

MyClass obj;
obj.count = 20;           // Via object (also works)
obj.func();               // Via object (also works)

// RESTRICTIONS in static functions
static void staticFunc() {
    // ✅ CAN access static members
    // ✅ CAN call other static functions
    // ❌ CANNOT access instance members
    // ❌ CANNOT call non-static functions
    // ❌ CANNOT use 'this'
}
```

---

## 🔍 Best Practices

### ✅ DO:

- **Use static** for class-wide data
- **Initialize** static members outside class
- **Access via class name** for clarity (`MyClass::member`)
- **Use static** for utility functions
- **Use static const** for shared constants
- **Document** thread-safety for mutable static
- **Use static** for object counting
- **Implement** Singleton via static
- **Use static** for factory methods

### ❌ DON'T:

- **Forget to initialize** static members
- **Initialize in constructor** (resets each time!)
- **Access instance members** from static functions
- **Repeat 'static'** in definition
- **Make static function const** (won't compile)
- **Overuse static** (breaks encapsulation)
- **Assume thread-safe** without synchronization
- **Use when instance member** would be better

---

## 🌟 Real-World Applications

### Configuration Management
```cpp
class Config {
    static string appName;
    static int version;
    static bool debugMode;
public:
    static void load(string file);
};
```

### Object Pools
```cpp
class ConnectionPool {
    static int maxConnections;
    static int activeConnections;
public:
    static Connection* acquire();
    static void release(Connection* conn);
};
```

### Logging Systems
```cpp
class Logger {
    static Logger* instance;
    static ofstream logFile;
public:
    static Logger* getInstance();
    static void log(string message);
};
```

### Caching
```cpp
class Cache {
    static map<string, Data> cache;
public:
    static Data get(string key);
    static void put(string key, Data value);
};
```

---

## 📖 Further Reading

- [cppreference.com - static members](https://en.cppreference.com/cpp/language/static)
- "Effective C++" (3rd ed.) by Scott Meyers - Item 4: Make sure that objects are initialized before they're used (static initialization order and function-local statics)
- "Design Patterns" - Singleton and Factory patterns
- [C++ Core Guidelines - Static Members](https://isocpp.github.io/CppCoreGuidelines/)

---

## 🎯 Key Takeaways

1. Static members **belong to the class**, not objects
2. **Only one copy** exists (shared by all)
3. Static data **must be initialized** outside class
4. Static functions have **no 'this' pointer**
5. Can **access without object**: `ClassName::member`
6. Static functions can **only access static members**
7. Perfect for **Singleton and Factory patterns**
8. Essential for **utility classes**
9. Used for **object counting and tracking**
10. **Class-level, not object-level**

---

**Previous Topic:** [10_ThisPointer](../10_ThisPointer/) - The this Pointer

**Next Topic:** [12_FriendFunctions](../12_FriendFunctions/) - Friend Functions (or continue with other topics)

---

*Part of the C++ Classes and Objects series*
*Difficulty: Intermediate*
*Prerequisites: 04_MemberFunctions, 10_ThisPointer*
*Essential for Design Patterns*