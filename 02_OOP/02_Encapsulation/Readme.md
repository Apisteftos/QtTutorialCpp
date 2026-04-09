# 05_Encapsulation - Data Hiding and Protection

## 📚 Overview

This folder introduces **Encapsulation** - one of the four fundamental pillars of Object-Oriented Programming (OOP). Encapsulation is the practice of bundling data and methods together while hiding internal details and providing controlled access through a public interface.

**Encapsulation** = **Data Hiding** + **Controlled Access**

---

## 🎯 What You'll Learn

- What encapsulation is and why it's important
- Access specifiers: `private`, `protected`, `public`
- Data hiding and information hiding
- Getters and setters (accessors/mutators)
- Validation and data integrity
- Benefits of encapsulation
- Best practices and common mistakes
- Real-world encapsulation examples

---

## 📖 Concept Explanation

### What is Encapsulation?

**Encapsulation** is the bundling of data (member variables) and methods (member functions) that operate on that data within a single unit (class), while restricting direct access to some components.

### Two Key Aspects:

1. **Data Hiding** - Make internal data private
2. **Controlled Access** - Provide public interface (getters/setters)

### Real-World Analogy:
```
Think of a TV Remote:
├── Hidden internals (encapsulated):
│   ├── Circuit board
│   ├── Battery connections
│   └── IR transmitter
└── Public interface (accessible):
    ├── Power button
    ├── Volume buttons
    └── Channel buttons

You don't need to know HOW it works internally.
You just use the buttons (public interface).
```

---

## 💻 Basic Example

### ❌ Without Encapsulation (Bad!)
```cpp
class BankAccount {
public:
    double balance;  // ❌ Anyone can modify!
};

BankAccount account;
account.balance = -5000.00;  // ❌ Negative balance!
account.balance = 999999999; // ❌ No validation!
```

**Problems:**
- No validation
- Data integrity compromised
- No control over modifications
- Business rules violated

### ✅ With Encapsulation (Good!)
```cpp
class BankAccount {
private:
    double balance;  // ✅ Hidden - can't access directly
    
public:
    double getBalance() const {
        return balance;
    }
    
    bool deposit(double amount) {
        if (amount > 0) {  // ✅ Validation!
            balance += amount;
            return true;
        }
        return false;
    }
    
    bool withdraw(double amount) {
        if (amount > 0 && amount <= balance) {  // ✅ Validation!
            balance -= amount;
            return true;
        }
        return false;
    }
};

BankAccount account;
// account.balance = -5000;  // ❌ Error! balance is private
account.deposit(1000);       // ✅ Controlled access
account.withdraw(500);       // ✅ Validated
```

**Benefits:**
- ✅ Validation enforced
- ✅ Data integrity maintained
- ✅ Business rules respected
- ✅ Controlled modifications

---

## 🔑 Access Specifiers

C++ provides three access specifiers to control visibility:

### 1. `private`

**Accessible only within the class**
```cpp
class MyClass {
private:
    int data;           // Only accessible inside MyClass
    void helper() { }   // Only accessible inside MyClass
};
```

**Use for:**
- Internal data
- Implementation details
- Helper methods

### 2. `protected`

**Accessible in class and derived classes**
```cpp
class Base {
protected:
    int data;  // Accessible in Base and derived classes
};

class Derived : public Base {
    void func() {
        data = 10;  // ✅ Can access protected member
    }
};
```

**Use for:**
- Members that derived classes need
- Covered more in inheritance topic

### 3. `public`

**Accessible from anywhere**
```cpp
class MyClass {
public:
    void doSomething() { }  // Anyone can call
};
```

**Use for:**
- Constructors
- Public interface (getters, setters, methods)
- Functions users need to call

### Default Access:
```cpp
class MyClass {
    int x;  // private by default in class
};

struct MyStruct {
    int x;  // public by default in struct
};
```

---

## 📋 Examples in This Folder

### Example 1: Without Encapsulation (The Problem)
Shows what goes wrong when data is public - no validation, chaos!

### Example 2: With Encapsulation (The Solution)
Proper data hiding with controlled access through methods.

### Example 3: Access Specifiers
Demonstrates `private`, `protected`, and `public` in action.

### Example 4: Data Validation
Using setters to validate data before accepting it.
```cpp
void setAge(int a) {
    if (a >= 0 && a < 150) {  // ✅ Validate!
        age = a;
    } else {
        cout << "Invalid age!\n";
    }
}
```

### Example 5: Information Hiding
Hiding internal representation from users.

### Example 6: Email Validation
Real-world example with email format validation.

### Example 7: Password Security
Never expose passwords - only allow verification.

### Example 8: Calculated Properties
Hiding calculation logic and caching.

### Example 9: Immutable Class
Objects that cannot be modified after creation.

### Example 10: Shopping Cart
Complete real-world encapsulation with multiple features.

---

## 🎓 Key Concepts

### Getters and Setters

**Getters (Accessors)** - Read member values:
```cpp
class Student {
private:
    int age;
    
public:
    // Getter - always const!
    int getAge() const {
        return age;
    }
};
```

**Setters (Mutators)** - Modify members with validation:
```cpp
class Student {
private:
    int age;
    
public:
    // Setter - validate before setting
    void setAge(int a) {
        if (a >= 0 && a < 150) {
            age = a;
        } else {
            cout << "Invalid age!\n";
        }
    }
};
```

### Why Getters/Setters?

1. **Validation** - Check values before setting
2. **Encapsulation** - Hide internal representation
3. **Flexibility** - Change implementation later
4. **Control** - Read-only, write-only, or both
5. **Debugging** - Add logging, breakpoints
6. **Computed values** - Calculate on demand

### Information Hiding

Hide implementation details from users:
```cpp
class Stack {
private:
    vector<int> data;  // Users don't know we use vector
    
public:
    void push(int value) { data.push_back(value); }
    int pop() { /* ... */ }
    int top() const { /* ... */ }
};

// Later, can change to array without breaking user code!
```

**Benefits:**
- Can change internal representation
- Users don't need to update their code
- Implementation is flexible

### Data Validation

Always validate in setters:
```cpp
class Rectangle {
private:
    double width;
    
public:
    void setWidth(double w) {
        if (w > 0) {           // ✅ Validate
            width = w;
        } else {
            cout << "Width must be positive!\n";
            width = 1.0;       // Safe default
        }
    }
};
```

---

## 🔍 Benefits of Encapsulation

### 1. **Data Protection**
```cpp
class BankAccount {
private:
    double balance;  // Protected from direct access
};

// account.balance = -1000;  ❌ Can't do this!
```

### 2. **Validation**
```cpp
void setAge(int a) {
    if (a >= 0 && a < 150) {  // Check before setting
        age = a;
    }
}
```

### 3. **Flexibility to Change**
```cpp
// Version 1: Store as Celsius
class Temperature {
private:
    double celsius;  // Internal representation
public:
    double getFahrenheit() const {
        return celsius * 9/5 + 32;  // Calculate
    }
};

// Version 2: Can change to store as Fahrenheit
// Users' code doesn't break!
```

### 4. **Ease of Use**
```cpp
// Complex internals hidden
// Simple interface exposed
circle.setRadius(5.0);
double area = circle.getArea();
```

### 5. **Maintainability**
```cpp
// All access goes through setters
// Easy to add logging, debugging
void setBalance(double b) {
    logChange(balance, b);  // Add logging easily
    balance = b;
}
```

---

## 💡 Common Mistakes & Solutions

### ❌ Mistake 1: Public Data Members
```cpp
class Bad {
public:
    int age;  // ❌ Anyone can set to any value!
};

Bad b;
b.age = -100;  // ❌ No validation!
```

**Fix:**
```cpp
class Good {
private:
    int age;  // ✅ Private
    
public:
    void setAge(int a) {
        if (a >= 0 && a < 150) {
            age = a;
        }
    }
    
    int getAge() const {
        return age;
    }
};
```

### ❌ Mistake 2: Returning Non-Const Reference to Private Data
```cpp
class Bad {
private:
    vector<int> data;
    
public:
    vector<int>& getData() {  // ❌ Exposes internals!
        return data;
    }
};

Bad b;
b.getData().clear();  // ❌ Can modify private data!
```

**Fix:**
```cpp
class Good {
private:
    vector<int> data;
    
public:
    // Return const reference or copy
    const vector<int>& getData() const {  // ✅ Can't modify
        return data;
    }
};
```

### ❌ Mistake 3: No Validation in Setters
```cpp
class Bad {
private:
    int age;
    
public:
    void setAge(int a) {
        age = a;  // ❌ No checking!
    }
};
```

**Fix:**
```cpp
class Good {
private:
    int age;
    
public:
    void setAge(int a) {
        if (a >= 0 && a < 150) {  // ✅ Validate!
            age = a;
        } else {
            cout << "Invalid age!\n";
        }
    }
};
```

### ❌ Mistake 4: Non-Const Getters
```cpp
class Bad {
private:
    int value;
    
public:
    int getValue() {  // ❌ Missing const!
        return value;
    }
};

const Bad b;
// b.getValue();  // ❌ Error! Can't call non-const on const object
```

**Fix:**
```cpp
class Good {
private:
    int value;
    
public:
    int getValue() const {  // ✅ Const getter
        return value;
    }
};
```

### ❌ Mistake 5: Over-Encapsulation
```cpp
class Bad {
private:
    int x, y;  // Coordinates
    
public:
    // Too many getters/setters for simple data
    int getX() const { return x; }
    void setX(int val) { x = val; }
    int getY() const { return y; }
    void setY(int val) { y = val; }
};
```

**Fix:** For simple data structures, consider struct:
```cpp
struct Point {  // Simple data container
    int x, y;   // Public is OK here
};
```

---

## 🎯 Practice Exercises

### Exercise 1: Temperature Class
Create a `Temperature` class:
- Private: Store in Celsius
- Public: Getters for Celsius, Fahrenheit, Kelvin
- Public: Setters for all three (convert to Celsius internally)
- Validation: Temperature >= absolute zero (-273.15°C)

### Exercise 2: Date Class
Create a `Date` class:
- Private: day, month, year
- Validation: Check for valid dates
- Handle leap years
- Setters reject invalid dates

### Exercise 3: User Account
Create a `UserAccount` class:
- Private: username, password (hashed), email
- Validate email format
- Validate password strength (min 8 chars, uppercase, lowercase, digit)
- No getter for password (only verify method)

### Exercise 4: Inventory Item
Create an `InventoryItem` class:
- Private: name, quantity, price
- Validation: quantity >= 0, price > 0
- Methods: restock(), sell(), getTotal()

---

## 📊 Visual Concept
```
┌─────────────────────────────────────────┐
│          Class (Capsule)                │
│                                         │
│  ┌───────────────────────────────┐     │
│  │   PRIVATE (Hidden)            │     │
│  │   ─────────────────           │     │
│  │   • Data members              │     │
│  │   • Helper methods            │     │
│  │   • Implementation details    │     │
│  │                               │     │
│  │   ❌ Not accessible from      │     │
│  │      outside                  │     │
│  └───────────────────────────────┘     │
│                                         │
│  ┌───────────────────────────────┐     │
│  │   PUBLIC (Interface)          │     │
│  │   ─────────────────           │     │
│  │   • Constructors              │     │
│  │   • Getters (const)           │     │
│  │   • Setters (validation)      │     │
│  │   • Public methods            │     │
│  │                               │     │
│  │   ✅ Accessible from outside  │     │
│  └───────────────────────────────┘     │
│                                         │
└─────────────────────────────────────────┘
           │
           │ User only sees public interface
           ▼
    obj.setValue(10);  ✅
    obj.getValue();    ✅
    obj.data = 10;     ❌ Error!
```

---

## 🚀 How to Run

### Compile:
```bash
g++ -std=c++17 encapsulation.cpp -o encapsulation
```

### Run:
```bash
./encapsulation
```

### Expected Output:
The program demonstrates:
1. Problems without encapsulation
2. Benefits with encapsulation
3. Access specifiers in action
4. Data validation
5. Information hiding
6. Email validation
7. Password security
8. Calculated properties
9. Immutable objects
10. Shopping cart system

---

## 🔗 Design Principles

### Encapsulation Principles:

1. **Hide data** - Make members private
2. **Expose interface** - Public methods only
3. **Validate input** - Check in setters
4. **Const correctness** - Make getters const
5. **Minimal interface** - Only expose what's needed

### Typical Class Structure:
```cpp
class WellEncapsulated {
private:
    // 1. Private data
    int data;
    
    // 2. Private helper methods
    bool validate(int value) const {
        return value > 0;
    }
    
public:
    // 3. Constructor
    WellEncapsulated(int value) {
        setData(value);
    }
    
    // 4. Public getters (const)
    int getData() const {
        return data;
    }
    
    // 5. Public setters (with validation)
    void setData(int value) {
        if (validate(value)) {
            data = value;
        }
    }
    
    // 6. Public interface methods
    void doSomething() {
        // ...
    }
};
```

---

## 📚 Related Topics

### Prerequisites:
- **01_BasicClass** - Understanding classes
- **02_Constructor** - Object initialization
- **04_MemberFunctions** - Methods

### Coming Next:
- **06_GettersSetters** - Deep dive into accessors/mutators (if separate)
- **13_Inheritance** - Protected access in derived classes
- **15_AbstractClass** - Interface design

### Related Concepts:
- **Information Hiding** - Hide implementation details
- **Data Abstraction** - Show only essential features
- **Interface vs Implementation** - Separation of concerns

---

## 🎓 Summary

### What We Learned:

✅ **Encapsulation** = Data hiding + Controlled access
✅ **Access specifiers**: `private`, `protected`, `public`
✅ **Data hiding** - Make members private
✅ **Getters** - Const methods to read data
✅ **Setters** - Methods with validation to modify data
✅ **Information hiding** - Hide implementation details
✅ **Validation** - Enforce business rules
✅ **Flexibility** - Change implementation without breaking code

### The Encapsulation Pattern:
```cpp
class MyClass {
private:
    // Hidden internals
    Type member;
    
    bool validate(Type value) const {
        // Validation logic
    }
    
public:
    // Public interface
    Type getMember() const {     // Getter
        return member;
    }
    
    void setMember(Type value) { // Setter
        if (validate(value)) {
            member = value;
        }
    }
};
```

### Why Encapsulation?

1. **Protection** - Data can't be corrupted
2. **Validation** - Enforce business rules
3. **Flexibility** - Change implementation freely
4. **Maintenance** - Easier to modify and debug
5. **Security** - Hide sensitive data
6. **Modularity** - Clear separation of concerns

---

## ✨ Quick Reference Card
```cpp
// BASIC ENCAPSULATION
class Encapsulated {
private:
    int data;  // ✅ Hidden
    
public:
    // Getter (const)
    int getData() const {
        return data;
    }
    
    // Setter (with validation)
    void setData(int value) {
        if (value > 0) {
            data = value;
        }
    }
};

// ACCESS SPECIFIERS
class AccessDemo {
private:
    int privateVar;     // Only in this class
    
protected:
    int protectedVar;   // This class + derived classes
    
public:
    int publicVar;      // Anywhere
};

// IMMUTABLE CLASS
class Immutable {
private:
    const int value;  // Can't change
    
public:
    Immutable(int v) : value(v) { }
    int getValue() const { return value; }
    // No setter - immutable!
};

// VALIDATION PATTERN
void setAge(int a) {
    if (a >= 0 && a < 150) {  // Validate
        age = a;
    } else {
        cout << "Invalid!\n";  // Reject
    }
}
```

---

## 🔍 Best Practices

### ✅ DO:

- **Make data private** - Always (except POD structs)
- **Provide getters** - For read access (make const)
- **Provide setters** - For write access (with validation)
- **Validate input** - Check before setting
- **Hide helpers** - Private helper methods
- **Use const** - On all read-only methods
- **Minimal interface** - Only expose what's needed
- **Document interface** - What users can call

### ❌ DON'T:

- **Make data public** - Breaks encapsulation
- **Return non-const refs** - Exposes internals
- **Skip validation** - Allows invalid data
- **Forget const** - On getters
- **Over-expose** - Keep interface minimal
- **Break abstraction** - Don't leak implementation details

---

## 🌟 The Four Pillars of OOP
```
1. ✅ ENCAPSULATION (This topic)
   │
   ├─ Data Hiding
   ├─ Controlled Access
   └─ Information Hiding

2. ABSTRACTION
   │
   ├─ Hide Complexity
   └─ Show Essential Features

3. INHERITANCE
   │
   ├─ Code Reuse
   └─ "Is-a" Relationship

4. POLYMORPHISM
   │
   ├─ Many Forms
   └─ Virtual Functions
```

Encapsulation is the **foundation** - you need it before the others!

---

## 📖 Further Reading

- [cppreference.com - Access specifiers](https://en.cppreference.com/w/cpp/language/access)
- [C++ Core Guidelines - Encapsulation](https://isocpp.github.io/CppCoreGuidelines/)
- "Effective C++" by Scott Meyers - Item 22: Declare data members private

---

## 🎯 Key Takeaways

1. Encapsulation = **Bundling data with methods** + **Hiding internals**
2. Always make **data private**
3. Provide **public interface** (getters/setters)
4. **Validate** in setters
5. Make **getters const**
6. **Hide implementation** details
7. Benefits: **Security, flexibility, maintainability**
8. One of the **four pillars of OOP**
9. Foundation for **good class design**
10. Enables **change without breaking user code**

---

**Previous Topic:** [04_MemberFunctions](../04_MemberFunctions/) - Class Methods

**Next Topic:** [06_GettersSetters](../06_GettersSetters/) - Deep Dive (or continue with other topics)

---

*Part of the C++ Classes and Objects series*
*Difficulty: Beginner to Intermediate*
*Prerequisites: 01_BasicClass, 02_Constructor, 04_MemberFunctions*
*One of the Four Pillars of OOP*