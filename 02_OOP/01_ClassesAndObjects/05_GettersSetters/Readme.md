# 06_GettersSetters - Accessors and Mutators

## 📚 Overview

This folder provides a deep dive into **Getters and Setters** (also called **Accessors** and **Mutators**) - the fundamental methods for controlled access to private class members. These are the primary tools for implementing encapsulation in C++.

**Getters** provide **read** access to private data.
**Setters** provide **write** access with **validation**.

---

## 🎯 What You'll Learn

- What getters and setters are
- Why we use them instead of public data
- Different getter patterns (value, reference, pointer)
- Different setter patterns (validation, exceptions, bool return)
- Read-only and write-only properties
- Computed properties
- Dependent properties
- Validation strategies
- Real-world examples
- Best practices

---

## 📖 Concept Explanation

### What are Getters and Setters?

**Getter (Accessor):**
- A function that **returns** the value of a private member
- Should **always be `const`**
- Provides **read access** to data
- Naming convention: `get` + PropertyName (e.g., `getName()`)

**Setter (Mutator):**
- A function that **modifies** the value of a private member
- Should **validate** input before setting
- Provides **controlled write access**
- Naming convention: `set` + PropertyName (e.g., `setName()`)

### Why Not Just Make Data Public?
```cpp
// ❌ BAD - Public data
class Bad {
public:
    int age;  // Anyone can set to any value!
};

Bad b;
b.age = -100;  // ❌ No validation!
b.age = 999;   // ❌ No control!

// ✅ GOOD - Private with getters/setters
class Good {
private:
    int age;  // Protected
    
public:
    int getAge() const {
        return age;
    }
    
    void setAge(int a) {
        if (a >= 0 && a <= 150) {  // ✅ Validation!
            age = a;
        }
    }
};
```

---

## 💻 Basic Pattern

### The Standard Template
```cpp
class MyClass {
private:
    Type memberVariable;
    
public:
    // GETTER - const, returns value
    Type getMemberVariable() const {
        return memberVariable;
    }
    
    // SETTER - validates, then sets
    void setMemberVariable(Type value) {
        if (isValid(value)) {  // Validation
            memberVariable = value;
        }
    }
};
```

### Usage
```cpp
MyClass obj;
obj.setMemberVariable(value);          // Set
Type val = obj.getMemberVariable();    // Get
```

---

## 🔍 Getter Patterns

### Pattern 1: Return by Value (Simple Types)

For small types (int, double, bool, char, pointers):
```cpp
class Person {
private:
    int age;
    
public:
    int getAge() const {  // Return by value
        return age;
    }
};
```

**When to use:** Small, cheap-to-copy types

### Pattern 2: Return by Const Reference (Large Objects)

For large objects (string, vector, custom classes):
```cpp
class Person {
private:
    string name;
    vector<int> data;
    
public:
    const string& getName() const {  // Return by const reference
        return name;
    }
    
    const vector<int>& getData() const {
        return data;
    }
};
```

**Benefits:**
- ✅ No copy (efficient)
- ✅ `const` prevents modification
- ✅ Safe to use

**When to use:** Large objects, avoid copying

### Pattern 3: Return Pointer (Nullable Values)

When the value might not exist:
```cpp
class Optional {
private:
    int* data;  // Might be nullptr
    
public:
    const int* getData() const {
        return data;  // Can return nullptr
    }
};

// Usage:
const int* ptr = obj.getData();
if (ptr != nullptr) {
    // Use *ptr
}
```

**When to use:** Optional values, nullable data

### Pattern 4: Computed Properties

Calculate value on demand, don't store:
```cpp
class Rectangle {
private:
    double width, height;
    
public:
    double getArea() const {
        return width * height;  // Computed!
    }
    
    double getPerimeter() const {
        return 2 * (width + height);  // Computed!
    }
};
```

**Benefits:**
- ✅ Always consistent with base data
- ✅ No storage needed
- ✅ Can't get out of sync

**When to use:** Derived/calculated values

### Pattern 5: Lazy Evaluation with Caching

Calculate once, cache result:
```cpp
class Expensive {
private:
    mutable double cachedValue;
    mutable bool cacheValid;
    
public:
    double getValue() const {
        if (!cacheValid) {
            cachedValue = expensiveCalculation();
            cacheValid = true;
        }
        return cachedValue;
    }
    
    void invalidateCache() {
        cacheValid = false;
    }
};
```

**When to use:** Expensive computations

---

## 🔧 Setter Patterns

### Pattern 1: Basic Validation

Simplest form - validate and set or ignore:
```cpp
void setAge(int a) {
    if (a >= 0 && a <= 150) {
        age = a;
    } else {
        cout << "Invalid age\n";
    }
}
```

### Pattern 2: Throw Exception

For strict validation:
```cpp
void setAge(int a) {
    if (a < 0 || a > 150) {
        throw invalid_argument("Age must be 0-150");
    }
    age = a;
}
```

**When to use:** Critical validation, user must handle error

### Pattern 3: Return Bool (Success/Failure)

Let caller know if it worked:
```cpp
bool setAge(int a) {
    if (a >= 0 && a <= 150) {
        age = a;
        return true;  // Success
    }
    return false;  // Failure
}

// Usage:
if (!obj.setAge(200)) {
    cout << "Failed to set age\n";
}
```

**When to use:** Caller needs to know success/failure

### Pattern 4: Clamp to Valid Range

Force value into valid range:
```cpp
void setVolume(int v) {
    if (v < 0) {
        volume = 0;
        cout << "Clamped to 0\n";
    } else if (v > 100) {
        volume = 100;
        cout << "Clamped to 100\n";
    } else {
        volume = v;
    }
}
```

**When to use:** User-facing controls (volume, brightness)

### Pattern 5: Normalize/Sanitize Input

Clean up input before storing:
```cpp
void setName(string n) {
    // Trim whitespace
    n = trim(n);
    
    // Capitalize first letter
    if (!n.empty()) {
        n[0] = toupper(n[0]);
    }
    
    name = n;
}
```

**When to use:** User input that needs formatting

### Pattern 6: Method Chaining (Fluent Interface)

Return `*this` for chaining:
```cpp
MyClass& setName(const string& n) {
    name = n;
    return *this;
}

MyClass& setAge(int a) {
    age = a;
    return *this;
}

// Usage:
obj.setName("Alice")
   .setAge(25)
   .setSalary(50000);
```

**When to use:** Builder pattern, fluent APIs

### Pattern 7: Batch Setter

Set multiple values at once:
```cpp
void setPersonInfo(const string& n, int a, const string& e) {
    setName(n);
    setAge(a);
    setEmail(e);
}
```

**When to use:** Related properties often set together

---

## 📋 Examples in This Folder

### Example 1: Basic Getters and Setters
The fundamental pattern with validation.

### Example 2: Different Getter Patterns
Five different ways to return values.

### Example 3: Different Setter Patterns
Seven different validation and setting strategies.

### Example 4: Read-Only and Write-Only Properties
Controlling access levels (no setter, no getter).

### Example 5: Validation Strategies
Email, phone, zip code validation.

### Example 6: Computed Properties
Values calculated from other data (area, perimeter).

### Example 7: Dependent Properties
Multiple views of same data (Celsius/Fahrenheit/Kelvin).

### Example 8: Range-Based Properties
Percentage class with clamping (0-100).

### Example 9: Real-World User Profile
Complete profile with username, email, age validation.

### Example 10: Real-World Product Inventory
Stock management with price, quantity validation.

---

## 🎓 Key Concepts

### Property Types

#### 1. Stored Properties
Directly backed by member variable:
```cpp
class Person {
private:
    string name;  // Stored
    
public:
    string getName() const { return name; }
    void setName(const string& n) { name = n; }
};
```

#### 2. Computed Properties
Calculated on demand:
```cpp
class Rectangle {
private:
    double width, height;
    
public:
    double getArea() const {
        return width * height;  // Computed!
    }
    // No setter - computed from width/height
};
```

#### 3. Dependent Properties
Multiple representations of same data:
```cpp
class Temperature {
private:
    double celsius;  // Only this is stored
    
public:
    double getCelsius() const { return celsius; }
    void setCelsius(double c) { celsius = c; }
    
    double getFahrenheit() const {
        return celsius * 9/5 + 32;  // Computed
    }
    
    void setFahrenheit(double f) {
        celsius = (f - 32) * 5/9;  // Updates celsius
    }
};
```

### Access Control

#### Read-Only (Getter Only)
```cpp
class Person {
private:
    int id;  // Assigned at creation, never changes
    
public:
    int getId() const { return id; }
    // No setter - read-only!
};
```

**Use cases:** ID, creation date, calculated values

#### Write-Only (Setter Only)
```cpp
class Account {
private:
    string password;  // Never expose!
    
public:
    void setPassword(const string& p) {
        password = hash(p);
    }
    // No getter!
    
    bool verifyPassword(const string& p) {
        return hash(p) == password;
    }
};
```

**Use cases:** Passwords, secure tokens, secrets

#### Full Access (Both)
```cpp
class Person {
private:
    string name;
    
public:
    string getName() const { return name; }
    void setName(const string& n) { name = n; }
};
```

**Use cases:** Most normal properties

---

## 🔑 Benefits of Getters/Setters

### 1. Validation
```cpp
void setAge(int a) {
    if (a >= 0 && a <= 150) {  // ✅ Check before setting
        age = a;
    }
}
```

### 2. Encapsulation
```cpp
class Temperature {
private:
    double celsius;  // Users don't know internal representation
    
public:
    double getFahrenheit() const {
        return celsius * 9/5 + 32;  // Convert on the fly
    }
};
```

### 3. Flexibility to Change
```cpp
// Version 1: Store as string
class Person {
    string name;
public:
    string getName() const { return name; }
};

// Version 2: Store as first/last (users' code doesn't break!)
class Person {
    string firstName, lastName;
public:
    string getName() const {
        return firstName + " " + lastName;
    }
};
```

### 4. Side Effects/Notifications
```cpp
void setBalance(double b) {
    double oldBalance = balance;
    balance = b;
    
    notifyObservers();  // Notify listeners
    logChange(oldBalance, balance);  // Log change
    updateUI();  // Update display
}
```

### 5. Debugging
```cpp
void setAge(int a) {
    cout << "Setting age to " << a << endl;  // Debug output
    age = a;
    // Can set breakpoint here easily
}
```

### 6. Computed Values
```cpp
double getTotal() const {
    return price * quantity * (1 - discount);  // Calculate
}
```

---

## 💡 Common Mistakes & Solutions

### ❌ Mistake 1: Non-Const Getter
```cpp
class Bad {
    int value;
public:
    int getValue() {  // ❌ Missing const!
        return value;
    }
};

const Bad b;
// b.getValue();  // ❌ Error!
```

**Fix:**
```cpp
int getValue() const {  // ✅ Const
    return value;
}
```

### ❌ Mistake 2: No Validation in Setter
```cpp
class Bad {
    int age;
public:
    void setAge(int a) {
        age = a;  // ❌ No checking!
    }
};

Bad b;
b.setAge(-100);  // ❌ Invalid!
```

**Fix:**
```cpp
void setAge(int a) {
    if (a >= 0 && a <= 150) {  // ✅ Validate
        age = a;
    }
}
```

### ❌ Mistake 3: Returning Non-Const Reference
```cpp
class Bad {
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
const vector<int>& getData() const {  // ✅ Const reference
    return data;
}
```

### ❌ Mistake 4: Setter with Confusing Side Effects
```cpp
class Bad {
    int x, y;
public:
    void setX(int val) {
        x = val;
        y = 0;  // ❌ Confusing side effect!
    }
};
```

**Fix:**
```cpp
void setX(int val) {
    x = val;  // ✅ Only set x
}
```

### ❌ Mistake 5: Complex Logic in Getter
```cpp
class Bad {
public:
    string getName() const {
        // ❌ Doing too much in getter!
        updateCache();
        validateData();
        logAccess();
        return processName(name);
    }
};
```

**Fix:** Keep getters simple, move complex logic elsewhere

---

## 🎯 Practice Exercises

### Exercise 1: Person Class
Create a `Person` class with:
- Properties: firstName, lastName, age, email
- Getters for all properties
- Setters with validation:
  - Names: not empty
  - Age: 0-150
  - Email: valid format
- Computed property: fullName (firstName + lastName)

### Exercise 2: BankAccount Class
Create a `BankAccount` class with:
- Properties: accountNumber (read-only), balance, interestRate
- Methods: deposit(), withdraw() with validation
- Computed: interest (balance * interestRate)
- Read-only accountNumber set at construction

### Exercise 3: Date Class
Create a `Date` class with:
- Properties: day, month, year
- Validation: valid dates (handle leap years)
- Computed: dayOfWeek, isLeapYear, daysInMonth
- Setters reject invalid dates

### Exercise 4: Color Class
Create a `Color` class with:
- Internal: Store as RGB (0-255 each)
- Getters/setters for: RGB, HSL, Hex
- All stay in sync (dependent properties)
- Validation: RGB 0-255, HSL 0-360/0-100/0-100

---

## 🚀 How to Run

### Compile:
```bash
g++ -std=c++17 getter_setter.cpp -o getter_setter
```

### Run:
```bash
./getter_setter
```

### Expected Output:
The program demonstrates:
1. Basic getters and setters
2. Different getter patterns
3. Different setter patterns
4. Access control (read-only, write-only)
5. Validation strategies
6. Computed properties
7. Dependent properties
8. Range-based properties
9. Real-world user profile
10. Real-world product inventory

---

## 📊 Visual Concept
```
┌──────────────────────────────────────┐
│            Class                     │
│                                      │
│  PRIVATE:                            │
│  ┌────────────────┐                 │
│  │ int age;       │ ◄────┐          │
│  └────────────────┘      │          │
│         ▲                │          │
│         │                │          │
│  PUBLIC:                 │          │
│  ┌─────────────────────┐ │          │
│  │ int getAge() const  │─┘          │
│  │ {                   │            │
│  │   return age;       │ ✅ Read    │
│  │ }                   │            │
│  └─────────────────────┘            │
│                                      │
│  ┌─────────────────────┐            │
│  │ void setAge(int a)  │            │
│  │ {                   │            │
│  │   if (a >= 0)       │ ✅ Validate│
│  │     age = a;        │──┐         │
│  │ }                   │  │         │
│  └─────────────────────┘  │         │
│                            ▼         │
│                      ┌────────────┐  │
│                      │   age      │  │
│                      └────────────┘  │
└──────────────────────────────────────┘

User can only access through controlled interface
```

---

## 🔗 Design Patterns

### The Standard Pattern
```cpp
class WellDesigned {
private:
    Type data;  // 1. Private data
    
    // 2. Private validation
    bool isValid(Type value) const {
        return /* check */;
    }
    
public:
    // 3. Public getter (const)
    Type getData() const {
        return data;
    }
    
    // 4. Public setter (with validation)
    void setData(Type value) {
        if (isValid(value)) {
            data = value;
        }
    }
};
```

### Naming Conventions
```cpp
// Standard naming
int getAge() const;
void setAge(int a);

// Alternative (less common)
int age() const;      // Getter without "get"
void age(int a);      // Setter

// Boolean getters
bool isValid() const;    // is + Adjective
bool hasData() const;    // has + Noun
bool canProcess() const; // can + Verb
```

---

## 📚 Related Topics

### Prerequisites:
- **01_BasicClass** - Understanding classes
- **04_MemberFunctions** - Member functions
- **05_Encapsulation** - Data hiding concept

### Coming Next:
- **07_CopyConstructor** - Copying objects
- **08_MoveSemantics** - Moving objects efficiently
- **09_OperatorOverloading** - Custom operators

---

## 🎓 Summary

### What We Learned:

✅ **Getters** = Read access (always const)
✅ **Setters** = Write access (with validation)
✅ **Validation** = Prevent invalid states
✅ **Encapsulation** = Hide internal representation
✅ **Flexibility** = Can change implementation
✅ **Computed properties** = Calculate on demand
✅ **Dependent properties** = Keep values in sync
✅ **Access control** = Read-only, write-only, full access
✅ **Patterns** = Different ways to get/set values
✅ **Best practices** = Always validate, always const

### The Pattern:
```cpp
class MyClass {
private:
    Type data;
    
public:
    // Getter
    Type getData() const {
        return data;
    }
    
    // Setter
    void setData(Type value) {
        if (isValid(value)) {
            data = value;
        }
    }
};
```

### Why Use Them?

1. **Validation** - Enforce business rules
2. **Encapsulation** - Hide implementation
3. **Flexibility** - Change internals freely
4. **Control** - Manage access levels
5. **Debugging** - Track modifications
6. **Computation** - Calculate values on demand

---

## ✨ Quick Reference Card
```cpp
// BASIC PATTERN
class MyClass {
    int value;
public:
    int getValue() const { return value; }
    void setValue(int v) { value = v; }
};

// WITH VALIDATION
void setAge(int a) {
    if (a >= 0 && a <= 150) {
        age = a;
    }
}

// RETURN BY CONST REFERENCE
const string& getName() const {
    return name;
}

// COMPUTED PROPERTY
double getArea() const {
    return width * height;
}

// METHOD CHAINING
MyClass& setValue(int v) {
    value = v;
    return *this;
}

// READ-ONLY
int getId() const { return id; }
// No setter

// WRITE-ONLY
void setPassword(string p) { password = hash(p); }
// No getter
```

---

## 🔍 Best Practices

### ✅ DO:

- **Always make getters `const`**
- **Always validate in setters**
- **Return const reference for large objects**
- **Use meaningful names** (getValue, setValue)
- **Keep getters simple** (just return)
- **Document validation rules**
- **Consider computed properties**
- **Provide read-only for immutable data**
- **Test edge cases** in validation

### ❌ DON'T:

- **Forget `const` on getters**
- **Skip validation in setters**
- **Return non-const reference to private data**
- **Make getters/setters too complex**
- **Expose internal representation**
- **Allow invalid states**
- **Have surprising side effects**
- **Make everything public "because it's easier"**

---

## 🌟 Real-World Applications

### Web Forms
```cpp
class UserForm {
    string email;
    string password;
public:
    void setEmail(string e) {
        if (isValidEmail(e)) email = e;
    }
    void setPassword(string p) {
        if (isStrongPassword(p)) password = hash(p);
    }
};
```

### Game Characters
```cpp
class Character {
    int health, maxHealth;
public:
    int getHealth() const { return health; }
    void setHealth(int h) {
        health = clamp(h, 0, maxHealth);
    }
    double getHealthPercent() const {
        return (double)health / maxHealth * 100;
    }
};
```

### Financial Systems
```cpp
class Account {
    double balance;
public:
    double getBalance() const { return balance; }
    bool deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            return true;
        }
        return false;
    }
};
```

---

## 📖 Further Reading

- [C++ Core Guidelines - Getters and Setters](https://isocpp.github.io/CppCoreGuidelines/)
- "Effective C++" by Scott Meyers - Item 20: Prefer pass-by-reference-to-const
- "Clean Code" by Robert C. Martin - Chapter on Objects and Data Structures

---

## 🎯 Key Takeaways

1. Getters provide **controlled read access** (always `const`)
2. Setters provide **controlled write access** (with validation)
3. **Validation** prevents invalid states
4. **Encapsulation** hides internal representation
5. **Flexibility** to change implementation later
6. **Computed properties** calculate on demand
7. **Dependent properties** keep values synchronized
8. **Access levels**: read-only, write-only, full access
9. **Always validate** in setters
10. Foundation of **proper encapsulation**

---

**Previous Topic:** [05_Encapsulation](../05_Encapsulation/) - Data Hiding

**Next Topic:** [07_CopyConstructor](../07_CopyConstructor/) - Copying Objects

---

*Part of the C++ Classes and Objects series*
*Difficulty: Beginner to Intermediate*
*Prerequisites: 01_BasicClass, 04_MemberFunctions, 05_Encapsulation*
*Core Encapsulation Pattern*