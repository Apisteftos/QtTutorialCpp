# 02_Constructor - Initializing Objects Automatically

## 📚 Overview

A **constructor** is a special member function that is called **automatically** when an object is created. Its job is to put the object into a valid starting state.

Without constructors, every object would start with garbage values (for built-in types like `int`) and you would have to remember to set every member by hand. With a constructor, the object is ready to use as soon as it exists.

---

## 🎯 What You'll Learn

- What a constructor is and why we need it
- Default constructor (no parameters)
- Parameterized constructor
- Constructor overloading
- Default arguments in constructors
- Member initializer list (and why it is preferred)
- Initialization order of members
- Delegating constructors (C++11)
- Validation inside a constructor
- Compiler-generated constructors
- The `explicit` keyword (preview)

---

## 📖 Concept Explanation

### What is a Constructor?

A constructor:
- has the **same name as the class**
- has **no return type** (not even `void`)
- is **called automatically** when an object is created
- is used to **initialize** the object

```cpp
class MyClass {
public:
    int data;

    // Constructor
    MyClass() {              // Same name as class
        data = 0;            // No return type!
    }
};

MyClass obj;  // Constructor automatically called!
```

### Purpose

- set initial values for member variables
- allocate resources if needed
- validate input
- make sure the object starts in a valid state

### Real-World Analogy

```
Class:       Car (blueprint)
Constructor: The factory assembly line

You don't get a car without wheels or an engine.
The assembly line (constructor) builds the car
completely, before you ever get to drive it.
```

---

## 💻 Types of Constructors

### 1. Default Constructor
No parameters. Called with `MyClass obj;`
```cpp
MyClass() { }
```

### 2. Parameterized Constructor
Takes parameters. Called with `MyClass obj(42);`
```cpp
MyClass(int x) { }
```

### 3. Copy Constructor
Creates a copy of an existing object. Detailed in `07_CopyConstructor`.
```cpp
MyClass(const MyClass& other) { }
```

---

## 🔍 Examples in This Folder

### Example 1: Class Without Constructor (the problem)
`StudentWithoutConstructor` has no constructor, so its `int` members contain garbage until you set them by hand.

**Problems:**
- easy to forget initialization
- repetitive code for each object
- error-prone

### Example 2: Default Constructor
`StudentWithConstructor()` sets `name = "Unknown"`, `age = 0`, `id = 0`.

**Benefits:**
- automatic initialization
- no garbage values
- safe to use immediately

### Example 3: Parameterized Constructor
```cpp
Car car1("Toyota", "Camry", 2024, 28000);
```
The object is created and initialized in one step.

### Example 4: Constructor Overloading
`BankAccount` has three constructors: default, `(holder, number)` and `(holder, number, balance)`. The compiler picks the one that matches the arguments.
```cpp
class MyClass {
public:
    MyClass() { }              // Constructor 1
    MyClass(int x) { }         // Constructor 2
    MyClass(int x, int y) { }  // Constructor 3
};

MyClass obj1;           // Calls constructor 1
MyClass obj2(10);       // Calls constructor 2
MyClass obj3(10, 20);   // Calls constructor 3
```

### Example 5: Default Arguments
```cpp
Rectangle(double w = 1.0, double h = 1.0) { ... }

Rectangle r1;          // w=1.0, h=1.0
Rectangle r2(5.0);     // w=5.0, h=1.0
Rectangle r3(4.0, 6.0);
```
One constructor handles all cases.

> ⚠️ Don't mix default arguments with overloading. It can cause ambiguity: `MyClass()` and `MyClass(int x = 0)` both match `MyClass obj;`.

### Example 6: Member Initializer List
```cpp
Point(int xVal, int yVal) : x(xVal), y(yVal) { }
```
`Point` has `const` members. They can **only** be set in the initializer list, because a `const` member can't be assigned in the body.

**Initializer list benefits:**
- can initialize `const` members
- can initialize reference members
- more efficient (direct initialization)
- required for members without a default constructor (like `Point center;` inside `Circle`)

### Example 7: Initialization Order
```cpp
class InitializationOrder {
    int first;
    int second;
    int third;
    InitializationOrder(int a, int b, int c) : third(c), first(a), second(b) { }
};
```
Members are **always initialized in declaration order** (`first`, `second`, `third`), **not** in the order of the initializer list. GCC and Clang warn about this with `-Wreorder` (part of `-Wall`).

Always write the initializer list in declaration order.

### Example 8: Delegating Constructors (C++11)
One constructor calls another, so the initialization logic exists only once:
```cpp
Employee(string n, int i, double s, string d) : name(n), id(i), salary(s), department(d) { }
Employee(string n)         : Employee(n, 0, 0.0, "Unassigned") { }
Employee(string n, int i)  : Employee(n, i, 0.0, "Unassigned") { }
```

**Benefits:**
- reduces code duplication
- centralizes initialization logic

The delegated (main) constructor runs **first**, then the body of the delegating one.

### Example 9: Real-World - Game Character
`GameCharacter(name, level)` calculates `maxHealth`, `attackPower` and `defense` from the level. The caller never sets them by hand.

### Example 10: Validation in Constructor
`Temperature(double)` clamps values below absolute zero (-273.15 °C), so no `Temperature` object can ever hold an impossible value.

---

## 🎓 Key Concepts

### Initialization vs Assignment

Two ways to set values in a constructor:

**1. Assignment (in the constructor body)**
```cpp
MyClass(const string& s) {
    name = s;          // Assignment
}
```
- the member is default-constructed first
- then it is assigned a new value
- less efficient

**2. Initialization (initializer list)**
```cpp
MyClass(const string& s) : name(s) { }   // Initialization
```
- the member is constructed directly with the value
- more efficient
- preferred way!

### When Constructors Are Called

```cpp
// 1. Creating an object
MyClass obj;                    // Constructor called
MyClass obj(10);                // Constructor called

// 2. Dynamic allocation
MyClass* ptr = new MyClass();   // Constructor called

// 3. Temporary objects
function(MyClass(10));          // Constructor called

// 4. Array of objects
MyClass arr[5];                 // Default constructor called 5 times
```

### Common Uses of Constructors

```cpp
// 1. Initialize member variables
MyClass(int x) : data(x) { }

// 2. Allocate resources
MyClass() { buffer = new int[100]; }

// 3. Validate input
MyClass(int age) {
    if (age < 0) age = 0;
    this->age = age;
}

// 4. Calculate derived values
Circle(double r) : radius(r) {
    area = 3.14159 * r * r;
}

// 5. Open files / connections
Database(string conn) {
    connection = connect(conn);
}
```

### Compiler-Generated Constructors

**If you don't define any constructor:**
- the compiler generates a default constructor
- it is like `MyClass() { }` (does nothing for built-in types)

**If you define ANY constructor:**
- the compiler does **not** generate the default constructor
- you must write it yourself if you need it (or ask for it with `= default`)

```cpp
class MyClass {
public:
    MyClass(int x) { }      // Custom constructor
    MyClass() = default;    // Ask the compiler for the default one
};

class OnlyCustom {
public:
    OnlyCustom(int x) { }
};

OnlyCustom a;      // ❌ Error! No default constructor
OnlyCustom b(10);  // ✅ OK
```

### `explicit` Keyword (Preview)

Prevents implicit conversions:
```cpp
class MyClass {
public:
    explicit MyClass(int x) { }
};

MyClass obj1(10);    // ✅ OK
MyClass obj2 = 10;   // ❌ Error! Can't implicitly convert
```
Use `explicit` for single-parameter constructors to prevent unexpected conversions.

### Constructor vs Destructor

| Constructor | Destructor |
|-------------|------------|
| Called when the object is created | Called when the object is destroyed |
| Initializes the object | Cleans up resources |
| Can have parameters | No parameters |
| Can be overloaded | Cannot be overloaded |
| This lesson | Covered in `03_Destructor` |

---

## ⚠️ Common Mistakes

### ❌ Mistake 1: Forgetting to initialize
```cpp
MyClass() {      // data not initialized!
}
```
**Fix:** initialize every member, preferably in the initializer list.

### ❌ Mistake 2: Wrong initialization order
```cpp
MyClass(int a, int b) : y(b), x(a) { }
// But x is declared before y - confusing, and the compiler warns!
```
**Fix:** list initializers in declaration order.

### ❌ Mistake 3: Assignment instead of initialization
```cpp
MyClass(const string& s) {
    name = s;     // Assignment (less efficient)
}
```
**Fix:** `MyClass(const string& s) : name(s) { }`

### ❌ Mistake 4: Most vexing parse
```cpp
MyClass obj();   // Not a default constructor call!
                 // It's a function declaration!
```
**Fix:** `MyClass obj;` or `MyClass obj{};`

---

## 🎯 Practice Exercises

### Exercise 1: Book Class
Create a `Book` class with a parameterized constructor `(title, author, pages)` and a default constructor that sets `"Untitled"`, `"Unknown"`, `0`.

### Exercise 2: Overloading
Create a `Rectangle` with three constructors: default (1×1), square `(side)` and `(width, height)`. Then rewrite it with default arguments and compare.

### Exercise 3: Initializer List
Create a class with a `const int id` and a `string& owner` reference member. Make it compile (hint: initializer list).

### Exercise 4: Initialization Order
Write a class where the initializer list order differs from the declaration order. Compile with `-Wall` and read the warning.

### Exercise 5: Delegating Constructors
Create a `Server(host, port, useTls)` class with two delegating constructors `Server(host)` and `Server(host, port)`.

### Exercise 6: Validation
Create a `Percentage` class that clamps its value to the range 0–100 in the constructor.

---

## 🚀 How to Run

### Compile:
```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o constructor
```
(`std::println` needs C++23 and a recent compiler, e.g. GCC 14+. `-Wall` shows the `-Wreorder` warning from Example 7 on purpose.)

### Run:
```bash
./constructor
```

### Expected Output:
The program demonstrates, in order:
1. A class without constructor (manual initialization)
2. Default constructor
3. Parameterized constructor (three cars)
4. Constructor overloading (three bank accounts)
5. Default arguments (three rectangles)
6. Member initializer list (`Point`, `Circle`)
7. Initialization order
8. Delegating constructors (three employees)
9. Game characters with calculated stats
10. Validation (temperature below absolute zero)

---

## 📚 Related Topics

### Previous:
**01_BasicClass** - classes, objects, members, the dot operator

### Coming Next:
**03_Destructor**
- Automatic cleanup
- Resource management

**05_Encapsulation**
- Private members
- Getters and setters

**07_CopyConstructor**
- Copying objects in detail

---

## 🎓 Summary

### What We Learned:

✅ **Constructor** = special function that initializes an object
✅ **Same name as the class, no return type**
✅ **Automatically called** when the object is created
✅ **Overloading** = several constructors with different parameters
✅ **Initializer list** = preferred, required for `const` / reference members
✅ **Members are initialized in declaration order**
✅ **Delegating constructors** reduce duplication
✅ **Validation** keeps objects always in a valid state

### Quick Reference Card
```cpp
// Default Constructor
class MyClass {
    MyClass() { x = 0; }
};
MyClass obj;           // Calls default constructor

// Parameterized Constructor
class MyClass {
    MyClass(int a) : x(a) { }
};
MyClass obj(42);       // Calls parameterized constructor

// Constructor Overloading
class MyClass {
    MyClass() { }
    MyClass(int a) { }
    MyClass(int a, int b) { }
};

// Delegating Constructors
class MyClass {
    MyClass(int x, int y) : x(x), y(y) { }
    MyClass() : MyClass(0, 0) { }
    MyClass(int x) : MyClass(x, 0) { }
};
```

---

## 🔍 Best Practices

### ✅ DO:
- always initialize all member variables
- use initializer lists (more efficient)
- validate input parameters
- keep constructors simple
- use delegating constructors to reduce duplication
- make objects usable right after construction

### ❌ DON'T:
- return values (constructors have no return type)
- call virtual functions in a constructor (the derived part doesn't exist yet)
- throw exceptions unless construction really cannot succeed
- do complex operations
- leave members uninitialized

---

## 📖 Further Reading

- [cppreference.com - Constructors and member initializer lists](https://en.cppreference.com/w/cpp/language/constructor)
- [cppreference.com - Delegating constructor](https://en.cppreference.com/w/cpp/language/initializer_list#Delegating_constructor)
- [cppreference.com - explicit specifier](https://en.cppreference.com/w/cpp/language/explicit)

---

## 🎯 Key Takeaways

1. Constructor = special function to initialize objects
2. Same name as the class, no return type
3. Automatically called when the object is created
4. Can be overloaded (multiple versions)
5. Use the initializer list (more efficient)
6. Always initialize all member variables
7. Validate input in the constructor
8. Keep constructors simple and focused

---

**Previous Topic:** [01_BasicClass](../01_BasicClass/) - Introduction to Classes

**Next Topic:** [03_Destructor](../03_Destructor/) - Automatic cleanup

---

*Part of the C++ Classes and Objects series*
*Difficulty: Beginner*
*Prerequisites: 01_BasicClass*
