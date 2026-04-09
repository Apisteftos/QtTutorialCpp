# 04_MemberFunctions - Class Methods and Behaviors

## 📚 Overview

This folder introduces **Member Functions** (also called **methods**) - functions defined inside a class that operate on object data and define object behaviors. Member functions are the "verbs" of your objects - they define what objects can do.

A **member function** operates on the data of a specific object and can access all members (both public and private) of that object.

---

## 🎯 What You'll Learn

- What member functions are and how they work
- Regular member functions
- Const member functions (read-only)
- Static member functions (class-level)
- Function overloading
- The `this` pointer
- Getters and setters (accessors/mutators)
- Default arguments
- Method chaining
- Best practices and common patterns

---

## 📖 Concept Explanation

### What is a Member Function?

A **member function** is:
- A function defined inside a class
- Operates on object's data (member variables)
- Can access all members (public and private)
- Called using an object: `obj.function()`
- Defines what objects can do (their behavior)

### Simple Analogy
```
Class = Car blueprint
├── Data (member variables): speed, fuel, color
└── Functions (member functions): accelerate(), brake(), refuel()

Car myCar;
myCar.accelerate();  // Call member function
```

---

## 💻 Basic Syntax

### Defining Member Functions
```cpp
class Rectangle {
private:
    double width;
    double height;
    
public:
    // Member function to calculate area
    double area() {
        return width * height;  // Access member variables
    }
    
    // Member function to display info
    void display() {
        cout << "Rectangle: " << width << "×" << height << "\n";
    }
};

// Usage:
Rectangle rect(5.0, 3.0);
double a = rect.area();      // Call member function
rect.display();              // Call member function
```

---

## 🔍 Types of Member Functions

### 1. Regular Member Functions

Can access and modify all members of the object.
```cpp
class Counter {
private:
    int count;
    
public:
    void increment() {
        count++;  // Can modify members
    }
    
    void reset() {
        count = 0;
    }
};
```

### 2. Const Member Functions

Promise **not to modify** the object. Should be used for all read-only operations.
```cpp
class BankAccount {
private:
    double balance;
    
public:
    // Const function - doesn't modify object
    double getBalance() const {
        // balance = 0;  ❌ Error! Can't modify in const function
        return balance;  // ✅ Can read
    }
    
    // Non-const function - can modify
    void deposit(double amount) {
        balance += amount;  // ✅ Can modify
    }
};

// Usage:
const BankAccount account("Alice", 1000);
double bal = account.getBalance();  // ✅ Can call const function
// account.deposit(100);            // ❌ Error! Can't call non-const on const object
```

**When to use `const`:**
- ✅ All getters
- ✅ Any function that doesn't modify the object
- ✅ Query functions (isEmpty(), size(), etc.)

**Benefits:**
- Prevents accidental modification
- Can be called on const objects
- Documents intent (read-only)
- Compiler enforces correctness

### 3. Static Member Functions

Belong to the **class**, not to individual objects.
```cpp
class Counter {
private:
    static int totalCount;  // Shared by all objects
    
public:
    Counter() {
        totalCount++;
    }
    
    // Static function - belongs to class
    static int getTotalCount() {
        // Can only access static members
        return totalCount;
    }
};

int Counter::totalCount = 0;  // Initialize static member

// Usage:
int count = Counter::getTotalCount();  // Call on class, not object
```

**Characteristics:**
- No `this` pointer
- Can only access static members
- Called on class: `ClassName::function()`
- Used for class-level operations

---

## 📋 Examples in This Folder

### Example 1: Basic Member Functions
Simple functions operating on rectangle data.
```cpp
rect.area();       // Calculate area
rect.perimeter();  // Calculate perimeter
rect.isSquare();   // Check if square
```

### Example 2: Const Member Functions
Read-only functions that don't modify the object.
```cpp
double bal = account.getBalance() const;  // ✅ Read-only
account.deposit(100);                     // Modifies object
```

### Example 3: Definition Styles
Three ways to define member functions:
1. Inside class (inline)
2. Outside class (ClassName::functionName)
3. Explicit inline keyword

### Example 4: Function Overloading
Multiple functions with same name, different parameters.
```cpp
calc.add(5, 3);          // Calls int version
calc.add(5.5, 3.2);      // Calls double version
calc.add(1, 2, 3);       // Calls 3-parameter version
```

### Example 5: Static Member Functions
Class-level functions shared by all objects.
```cpp
Counter::getCount();     // Call on class
Counter::resetCount();   // Call on class
```

### Example 6: This Pointer
Pointer to current object for disambiguation and chaining.
```cpp
obj.setValue(10).setName("Test");  // Method chaining
```

### Example 7: Getters and Setters
Controlled access to private members with validation.
```cpp
student.setAge(21);      // Setter with validation
int age = student.getAge();  // Getter (const)
```

### Example 8: Default Arguments
Functions with optional parameters.
```cpp
printer.print("Hello");              // Uses defaults
printer.print("Report", 3);          // Overrides copies
printer.print("Flyer", 100, true);   // Overrides all
```

### Example 9: Bank Account (Real-World)
Complete banking system with transactions and history.

### Example 10: Game Character (Real-World)
Character with health, attacks, and interactions.

---

## 🎓 Key Concepts

### The `this` Pointer

Every non-static member function has a hidden parameter called `this` - a pointer to the current object.
```cpp
class MyClass {
    int value;
    
public:
    void setValue(int value) {
        this->value = value;  // 'this->value' = member
                              // 'value' = parameter
    }
    
    MyClass& setValueChain(int value) {
        this->value = value;
        return *this;  // Return current object
    }
};

// Method chaining:
obj.setValueChain(10).setValueChain(20).setValueChain(30);
```

**Uses of `this`:**
1. **Resolve name conflicts** - distinguish parameter from member
2. **Return current object** - `return *this;`
3. **Method chaining** - return `*this` for fluent interface
4. **Compare with other objects**
5. **Get pointer to current object**

**Type:** `ClassName* const this`

### Getters and Setters

**Getters (Accessors)** - Read member values:
```cpp
int getAge() const {  // Always const!
    return age;
}
```

**Setters (Mutators)** - Modify members with validation:
```cpp
void setAge(int a) {
    if (a > 0 && a < 150) {  // Validation!
        age = a;
    } else {
        cout << "Invalid age\n";
    }
}
```

**Benefits:**
- ✅ **Encapsulation** - Hide internal representation
- ✅ **Validation** - Check values before setting
- ✅ **Flexibility** - Change implementation without breaking interface
- ✅ **Control** - Read-only, write-only, or both
- ✅ **Debugging** - Add logging or breakpoints

### Function Overloading

Multiple functions with the **same name** but **different parameter lists**.
```cpp
class Calculator {
public:
    int add(int a, int b) {
        return a + b;
    }
    
    int add(int a, int b, int c) {
        return a + b + c;
    }
    
    double add(double a, double b) {
        return a + b;
    }
};

Calculator calc;
calc.add(5, 3);        // Calls int version
calc.add(5, 3, 2);     // Calls 3-parameter version
calc.add(5.5, 3.2);    // Calls double version
```

**Requirements:**
- Same function name
- Different parameter lists (number or types)
- Return type alone is NOT enough

**Compiler chooses** the right version based on arguments provided.

---

## 🔑 Important Points

### 1. Const Correctness

**Always make read-only functions `const`!**
```cpp
class Student {
    string name;
    
public:
    // ✅ GOOD - const getter
    string getName() const {
        return name;
    }
    
    // ❌ BAD - missing const
    string getNameBad() {
        return name;
    }
};

const Student s("Alice");
s.getName();     // ✅ Works
// s.getNameBad();  // ❌ Error! Can't call non-const on const object
```

### 2. Access to Members

Member functions can access **all** members (public and private):
```cpp
class MyClass {
private:
    int privateData;
    
public:
    void memberFunction() {
        privateData = 10;  // ✅ Can access private members
    }
};
```

### 3. Member Function Definition Styles

**Style 1: Inside class (inline)**
```cpp
class MyClass {
    void func() {  // Defined inside
        // ...
    }
};
```

**Style 2: Outside class**
```cpp
class MyClass {
    void func();  // Declaration
};

void MyClass::func() {  // Definition outside
    // ...
}
```

**When to use:**
- **Inside:** Short, simple functions (getters, setters)
- **Outside:** Long functions, keeps header clean

### 4. Default Arguments
```cpp
class Printer {
public:
    void print(string msg, int copies = 1, bool color = false) {
        // ...
    }
};

Printer p;
p.print("Hello");              // copies=1, color=false
p.print("Report", 3);          // color=false
p.print("Flyer", 100, true);   // All specified
```

**Rules:**
- Default arguments must be at the end
- Can skip trailing arguments
- Specified in declaration, not definition

---

## 💡 Common Mistakes & Solutions

### ❌ Mistake 1: Forgetting `const` on Getters
```cpp
class Bad {
    int value;
public:
    int getValue() {  // ❌ Missing const
        return value;
    }
};
```

**Fix:**
```cpp
class Good {
    int value;
public:
    int getValue() const {  // ✅ Const getter
        return value;
    }
};
```

### ❌ Mistake 2: Modifying in Const Function
```cpp
class Bad {
    int count;
public:
    void display() const {
        count++;  // ❌ Error! Can't modify in const function
    }
};
```

**Fix:**
```cpp
class Good {
    int count;
public:
    void display() const {
        cout << count;  // ✅ Only read, don't modify
    }
    
    void increment() {  // Non-const for modification
        count++;
    }
};
```

### ❌ Mistake 3: No Validation in Setters
```cpp
class Bad {
    int age;
public:
    void setAge(int a) {
        age = a;  // ❌ No validation! Could be negative
    }
};
```

**Fix:**
```cpp
class Good {
    int age;
public:
    void setAge(int a) {
        if (a > 0 && a < 150) {  // ✅ Validate!
            age = a;
        }
    }
};
```

### ❌ Mistake 4: Direct Member Access from Outside
```cpp
class Bad {
public:
    int age;  // ❌ Public data
};

Bad b;
b.age = -50;  // ❌ No validation!
```

**Fix:**
```cpp
class Good {
private:
    int age;  // ✅ Private data
    
public:
    void setAge(int a) {  // ✅ Controlled access
        if (a > 0 && a < 150) {
            age = a;
        }
    }
    
    int getAge() const {
        return age;
    }
};
```

### ❌ Mistake 5: Accessing Non-Static Members from Static Function
```cpp
class Bad {
    int value;
    
public:
    static void func() {
        value = 10;  // ❌ Error! Static can't access non-static
    }
};
```

**Fix:**
```cpp
class Good {
    static int value;  // Make member static too
    
public:
    static void func() {
        value = 10;  // ✅ Can access static members
    }
};

int Good::value = 0;  // Initialize static member
```

---

## 🎯 Practice Exercises

### Exercise 1: Circle Class
Create a `Circle` class with:
- Private member: radius
- Functions: area(), circumference(), display()
- Setter with validation (radius > 0)
- Const getters

### Exercise 2: String Manipulator
Create a `StringManipulator` class with overloaded functions:
- `reverse(string)` - reverse a string
- `reverse(string, int)` - reverse first n characters
- `reverse(vector<string>)` - reverse each string in vector

### Exercise 3: Counter with Static
Create a `Counter` class with:
- Static member for total count
- Instance ID for each object
- Static function to get total count
- Static function to reset count

### Exercise 4: Temperature Converter
Create a `Temperature` class with:
- Private: value in Celsius
- Setters: setCelsius(), setFahrenheit(), setKelvin()
- Getters: getCelsius(), getFahrenheit(), getKelvin()
- All conversions done automatically

---

## 📊 Visual Concept
```
┌─────────────────────────────────┐
│         Class                   │
│                                 │
│  Member Variables (Data)        │
│    - int value                  │
│    - string name                │
│                                 │
│  Member Functions (Behavior)    │
│    + setValue(int)              │
│    + getValue() const           │
│    + display() const            │
│    + reset()                    │
└─────────────────────────────────┘
           │
           │ Create object
           ▼
┌─────────────────────────────────┐
│         Object                  │
│                                 │
│  value = 42                     │
│  name = "Example"               │
│                                 │
│  obj.setValue(100) ──────────► Call function
│  obj.getValue()    ──────────► Call function
│  obj.display()     ──────────► Call function
└─────────────────────────────────┘
```

---

## 🚀 How to Run

### Compile:
```bash
g++ -std=c++17 member_functions.cpp -o member_functions
```

### Run:
```bash
./member_functions
```

### Expected Output:
The program demonstrates:
1. Basic member functions
2. Const member functions
3. Definition styles
4. Function overloading
5. Static member functions
6. The `this` pointer
7. Getters and setters
8. Default arguments
9. Real-world bank account
10. Real-world game character

---

## 🔗 Comparison Tables

### Regular vs Const Member Functions

| Feature | Regular Function | Const Function |
|---------|-----------------|----------------|
| **Modify Members** | Yes | No |
| **Syntax** | `void func()` | `void func() const` |
| **Called on const objects** | No | Yes |
| **Purpose** | Commands/Mutations | Queries/Accessors |
| **Examples** | setters, commands | getters, queries |

### Static vs Non-Static Functions

| Feature | Non-Static | Static |
|---------|-----------|--------|
| **Belongs to** | Object | Class |
| **Called on** | Object: `obj.func()` | Class: `Class::func()` |
| **`this` pointer** | Yes | No |
| **Access non-static** | Yes | No |
| **Access static** | Yes | Yes |

---

## 📚 Related Topics

### Prerequisites:
- **01_BasicClass** - Understanding classes
- **02_Constructor** - Object initialization
- **03_Destructor** - Cleanup

### Coming Next:
- **05_Encapsulation** - Private vs public access
- **06_GettersSetters** - Deep dive into accessors/mutators
- **10_ThisPointer** - More about `this`
- **11_StaticMembers** - Static variables and functions

---

## 🎓 Summary

### What We Learned:

✅ **Member functions** define object behavior
✅ **Const functions** promise not to modify object
✅ **Static functions** belong to class, not objects
✅ **`this` pointer** points to current object
✅ **Overloading** provides multiple versions
✅ **Getters/setters** provide controlled access
✅ **Default arguments** make functions flexible
✅ **Method chaining** uses `return *this`

### Member Function Patterns:
```cpp
// Getter pattern (always const)
Type getMember() const { return member; }

// Setter pattern (with validation)
void setMember(Type value) {
    if (valid(value)) {
        member = value;
    }
}

// Method chaining
MyClass& doSomething() {
    // ...
    return *this;
}

// Static function
static Type getClassLevel() {
    return staticMember;
}
```

### Why Use Member Functions?

1. **Encapsulation** - Control access to data
2. **Validation** - Check values before setting
3. **Behavior** - Define what objects can do
4. **Interface** - Clear API for users
5. **Flexibility** - Change implementation later
6. **Maintainability** - Centralize logic

---

## ✨ Quick Reference Card
```cpp
// REGULAR MEMBER FUNCTION
class MyClass {
    int value;
public:
    void setValue(int v) {
        value = v;
    }
};

// CONST MEMBER FUNCTION
class MyClass {
    int value;
public:
    int getValue() const {  // ← const keyword
        return value;
    }
};

// STATIC MEMBER FUNCTION
class MyClass {
    static int count;
public:
    static int getCount() {
        return count;
    }
};
int MyClass::count = 0;

// Call: MyClass::getCount();

// THIS POINTER
class MyClass {
    int value;
public:
    void setValue(int value) {
        this->value = value;  // Resolve conflict
    }
    
    MyClass& chain() {
        return *this;  // Method chaining
    }
};

// OVERLOADING
class MyClass {
public:
    void func(int x) { }
    void func(double x) { }
    void func(int x, int y) { }
};

// GETTER/SETTER
class MyClass {
    int age;
public:
    int getAge() const { return age; }
    void setAge(int a) {
        if (a > 0) age = a;
    }
};
```

---

## 🔍 Best Practices

### ✅ DO:

- **Make getters `const`** - Always!
- **Validate in setters** - Check before setting
- **Use meaningful names** - `calculateArea()` not `calc()`
- **Keep functions short** - One responsibility
- **Use `const` everywhere** - Const correctness
- **Return `*this` for chaining** - Fluent interface
- **Document complex functions** - Help future you
- **Prefer member functions** - Over global functions

### ❌ DON'T:

- **Forget `const` on getters** - Common mistake
- **Make long functions** - Split into smaller ones
- **Access data directly** - Use getters/setters
- **Modify in const functions** - Compiler will catch
- **Return references to locals** - Undefined behavior
- **Mix static/non-static access** - Won't compile
- **Overload on return type** - Not allowed
- **Validate after setting** - Validate before!

---

## 🌟 Design Principles

### Single Responsibility Principle

Each function should do **one thing well**.
```cpp
// ❌ BAD - doing too much
void processAndDisplayStudent() {
    // Calculate GPA
    // Validate data
    // Update records
    // Display to screen
    // Log to file
}

// ✅ GOOD - separate concerns
void calculateGPA();
bool validate();
void updateRecords();
void display() const;
void logToFile() const;
```

### Encapsulation

Use functions to **control access** to data.
```cpp
class BankAccount {
private:
    double balance;  // Hidden!
    
public:
    // Controlled interface
    double getBalance() const;
    bool deposit(double amount);   // Validation
    bool withdraw(double amount);  // Validation
};

// Can't do: account.balance = -1000;  ❌
// Must use: account.withdraw(1000);   ✅
```

---

## 📖 Further Reading

- [cppreference.com - Member functions](https://en.cppreference.com/w/cpp/language/member_functions)
- [C++ Core Guidelines - Functions](https://isocpp.github.io/CppCoreGuidelines/)
- "Effective C++" by Scott Meyers - Item 3: Use const whenever possible

---

## 🎯 Key Takeaways

1. Member functions define **what objects can do**
2. Always make **getters `const`**
3. **Validate input** in setters
4. Use **`this`** for disambiguation and chaining
5. **Static functions** are class-level operations
6. **Overloading** provides intuitive interfaces
7. **Getters/setters** enable encapsulation
8. Keep functions **short and focused**
9. **Const correctness** prevents bugs
10. Member functions are the **heart of OOP**

---

**Previous Topic:** [03_Destructor](../03_Destructor/) - Automatic Cleanup

**Next Topic:** [05_Encapsulation](../05_Encapsulation/) - Data Hiding

---

*Part of the C++ Classes and Objects series*
*Difficulty: Beginner to Intermediate*
*Prerequisites: 01_BasicClass, 02_Constructor, 03_Destructor*