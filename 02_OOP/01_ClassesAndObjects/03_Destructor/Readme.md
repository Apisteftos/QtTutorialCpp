# 03_Destructor - Automatic Resource Cleanup

## 📚 Overview

This folder introduces **Destructors** - special member functions that automatically clean up resources when objects are destroyed. Destructors are the counterpart to constructors and are essential for proper resource management in C++.

A **destructor** is called automatically when an object is destroyed, ensuring that resources (memory, files, connections) are properly released.

---

## 🎯 What You'll Learn

- What a destructor is and why we need it
- When destructors are called
- Destruction order (reverse of construction)
- Resource cleanup (memory, files, connections)
- Virtual destructors for inheritance
- RAII pattern (Resource Acquisition Is Initialization)
- Exception safety with destructors
- Best practices and common mistakes

---

## 📖 Concept Explanation

### What is a Destructor?

A **destructor** is a special member function that:
- Has the **same name** as the class with **`~` prefix**
- Has **NO return type** (not even `void`)
- Has **NO parameters** (cannot be overloaded)
- Is **automatically called** when an object is destroyed
- Is used to **clean up resources**

### The Problem Without Destructors
```cpp
class DynamicArray {
    int* data;
public:
    DynamicArray(size_t size) {
        data = new int[size];  // Allocate memory
    }
    // No destructor!
};

void problem() {
    DynamicArray arr(1000);
}  // Memory leaked! 💔 data never freed
```

### The Solution With Destructors
```cpp
class DynamicArray {
    int* data;
public:
    DynamicArray(size_t size) {
        data = new int[size];
    }
    
    ~DynamicArray() {
        delete[] data;  // Free memory automatically!
    }
};

void solution() {
    DynamicArray arr(1000);
}  // Memory automatically freed! ✅
```

---

## 💻 Destructor Syntax

### Basic Structure
```cpp
class ClassName {
public:
    // Destructor
    ~ClassName() {
        // Cleanup code
    }
};
```

### Key Characteristics

1. **`~` prefix** - Tilde before class name
2. **No return type** - Not even `void`
3. **No parameters** - Cannot be overloaded (only one destructor)
4. **Automatically called** - When object is destroyed
5. **Should be `noexcept`** - Never throw exceptions (implicit)

---

## 🔍 When Destructors Are Called

### Automatic Invocation

Destructors are called automatically in these situations:

#### 1. **Scope Ends**
```cpp
{
    MyClass obj;
    // Use obj...
}  // ← Destructor called here!
```

#### 2. **Function Returns**
```cpp
void function() {
    MyClass obj;
    // Use obj...
    return;  // Destructor called before return
}
```

#### 3. **`delete` Called**
```cpp
MyClass* ptr = new MyClass();
delete ptr;  // ← Destructor called here!
```

#### 4. **Program Exits**
```cpp
MyClass global;

int main() {
    // ...
}  // global's destructor called on program exit
```

#### 5. **Exception Thrown**
```cpp
try {
    MyClass obj;
    throw exception();
}  // ← Destructor called before exception propagates!
catch (...) { }
```

---

## 📋 Examples in This Folder

### Example 1: Basic Destructor
Simplest destructor showing when it's called.
```cpp
class SimpleClass {
    SimpleClass() { cout << "Created\n"; }
    ~SimpleClass() { cout << "Destroyed\n"; }
};
```

### Example 2: When Destructors Are Called
Demonstrates all scenarios where destructors are invoked.

### Example 3: Memory Cleanup
Managing dynamic memory allocation/deallocation.
```cpp
~DynamicArray() {
    delete[] data;  // Free allocated memory
}
```

### Example 4: File Handle Cleanup
Automatically closing files.
```cpp
~FileManager() {
    if (file) {
        fclose(file);  // Close file automatically
    }
}
```

### Example 5: Destructor Order
Objects destroyed in **reverse order** of construction.
```cpp
MyClass obj1;  // Created 1st
MyClass obj2;  // Created 2nd
MyClass obj3;  // Created 3rd
// Destroyed: obj3, obj2, obj1 (reverse!)
```

### Example 6: Composition (Nested Objects)
Member destructors called automatically.
```cpp
class Car {
    Engine engine;   // Destroyed 2nd
    Wheels wheels;   // Destroyed 1st
    ~Car() { }       // Destroyed 3rd
};
```

### Example 7: Exception Safety
Destructors ensure cleanup even when exceptions thrown.

### Example 8: Array of Objects
All array elements automatically destroyed.

### Example 9: Heap vs Stack Destruction
Comparing automatic (stack) vs manual (heap) cleanup.

### Example 10: Virtual Destructor
**Critical** for polymorphism and inheritance.
```cpp
class Base {
    virtual ~Base() { }  // ← Must be virtual!
};
```

### Example 11: Database Connection
Real-world RAII example with connections.

### Example 12: RAII Pattern
Timer that automatically reports elapsed time.

---

## 🎓 Key Concepts

### Destruction Order

**Rule:** Objects are destroyed in **REVERSE order** of construction.
```cpp
void example() {
    MyClass obj1;  // Constructed 1st
    MyClass obj2;  // Constructed 2nd
    MyClass obj3;  // Constructed 3rd
}
// Destruction order: obj3 → obj2 → obj1
```

**Why?** Later objects may depend on earlier ones.

### Member Destruction Order

For classes with member objects:
```cpp
class Car {
    Engine engine;   // Declared 1st
    Wheels wheels;   // Declared 2nd
public:
    ~Car() {
        cout << "Car destructor\n";
    }
};
```

**Destruction sequence:**
1. `Car` destructor body executes
2. `wheels` destructor called (reverse order)
3. `engine` destructor called

### Virtual Destructors

**Critical Rule:** Always make destructors `virtual` in base classes!
```cpp
class Base {
public:
    virtual ~Base() { }  // ← MUST be virtual!
};

class Derived : public Base {
    int* data;
public:
    ~Derived() {
        delete[] data;  // Must be called!
    }
};

// Usage:
Base* ptr = new Derived();
delete ptr;  // Without virtual: MEMORY LEAK!
             // With virtual: Both destructors called ✅
```

**Without `virtual`:**
- Only `Base` destructor called
- `Derived` destructor NOT called
- Memory leaked! 💔

**With `virtual`:**
- `Derived` destructor called first
- Then `Base` destructor called
- Proper cleanup! ✅

---

## 🔧 RAII Pattern

### What is RAII?

**RAII** = **R**esource **A**cquisition **I**s **I**nitialization

**Pattern:**
- **Constructor** acquires resource
- **Destructor** releases resource
- Automatic, exception-safe cleanup

### Example:
```cpp
class FileHandle {
    FILE* file;
public:
    // Constructor: ACQUIRE resource
    FileHandle(const string& name) {
        file = fopen(name.c_str(), "r");
    }
    
    // Destructor: RELEASE resource
    ~FileHandle() {
        if (file) {
            fclose(file);  // Automatic cleanup!
        }
    }
};

// Usage:
{
    FileHandle file("data.txt");
    // Use file...
}  // File automatically closed!
```

### RAII Benefits:

✅ **Cannot forget cleanup** - Automatic
✅ **Exception-safe** - Cleanup even if exception thrown
✅ **Simple to use** - Just create object
✅ **No manual management** - Compiler handles it

### Common RAII Types:
```cpp
unique_ptr<T>      // Memory management
shared_ptr<T>      // Shared ownership
ifstream/ofstream  // File streams
lock_guard<mutex>  // Mutex locks
vector<T>          // Dynamic arrays
string             // String management
```

---

## 🔑 Important Points

### 1. Stack vs Heap Objects

**Stack Objects (Automatic):**
```cpp
MyClass obj;  // Stack
// Destructor called automatically when scope ends
✅ Automatic cleanup
✅ Cannot forget
✅ Exception-safe
✅ Preferred way
```

**Heap Objects (Manual):**
```cpp
MyClass* ptr = new MyClass();  // Heap
delete ptr;  // MUST manually delete!
❌ Can forget to delete
❌ Causes memory leaks
❌ Not exception-safe
⚠️  Use smart pointers instead!
```

### 2. Exception Safety

Destructors are called even when exceptions are thrown:
```cpp
try {
    MyClass obj;
    throw exception();  // Exception!
}  // obj's destructor STILL called! ✅
catch (...) { }
```

**This guarantees:**
- Resources always cleaned up
- No leaks even with exceptions
- Code is exception-safe by default

### 3. Never Throw from Destructor

**❌ NEVER DO THIS:**
```cpp
~MyClass() {
    throw exception();  // DON'T!
}
```

**Why?**
- Can cause program termination
- If destructor called during exception handling → `std::terminate()`
- Destructors are implicitly `noexcept`

**✅ DO THIS:**
```cpp
~MyClass() noexcept {
    try {
        // Cleanup code
    } catch (...) {
        // Log error, but don't rethrow!
    }
}
```

### 4. Virtual Destructors for Inheritance

**Rule:** If a class is intended to be a base class, make destructor `virtual`.
```cpp
class Base {
public:
    virtual ~Base() { }  // ✅ ALWAYS virtual in base classes!
};
```

**When to use `virtual`:**
- ✅ Base classes that will be inherited
- ✅ Classes with virtual functions
- ✅ Polymorphic classes

**When NOT needed:**
- ❌ Final classes (not inherited from)
- ❌ Classes never used polymorphically

---

## 💡 Common Mistakes & Solutions

### ❌ Mistake 1: Forgetting to Free Memory
```cpp
class Bad {
    int* data;
public:
    Bad() { data = new int[100]; }
    ~Bad() {
        // ❌ Forgot: delete[] data;
    }  // Memory leak!
};
```

**Fix:**
```cpp
class Good {
    int* data;
public:
    Good() { data = new int[100]; }
    ~Good() {
        delete[] data;  // ✅ Free memory
    }
};
```

### ❌ Mistake 2: Non-Virtual Destructor in Base Class
```cpp
class Base {
    ~Base() { }  // ❌ Not virtual!
};

class Derived : public Base {
    int* data;
    ~Derived() { delete[] data; }
};

Base* ptr = new Derived();
delete ptr;  // 💥 Only Base destructor called! Memory leak!
```

**Fix:**
```cpp
class Base {
    virtual ~Base() { }  // ✅ Virtual!
};
```

### ❌ Mistake 3: Throwing Exception from Destructor
```cpp
~MyClass() {
    throw exception();  // ❌ NEVER throw!
}
```

**Fix:**
```cpp
~MyClass() noexcept {
    try {
        // Cleanup
    } catch (...) {
        // Handle but don't rethrow
    }
}
```

### ❌ Mistake 4: Forgetting to Delete Heap Object
```cpp
void leak() {
    MyClass* ptr = new MyClass();
    // ❌ Forgot: delete ptr;
}  // Memory leak!
```

**Fix:**
```cpp
void safe() {
    auto ptr = make_unique<MyClass>();  // ✅ Smart pointer
}  // Automatically deleted!
```

### ❌ Mistake 5: Double Delete
```cpp
MyClass* ptr = new MyClass();
delete ptr;
delete ptr;  // ❌ Double delete! Crash!
```

**Fix:**
```cpp
MyClass* ptr = new MyClass();
delete ptr;
ptr = nullptr;  // ✅ Set to null after delete
```

---

## 🎯 Practice Exercises

### Exercise 1: String Buffer
Create a `StringBuffer` class that:
- Allocates char array in constructor
- Frees it in destructor
- Test with multiple objects

### Exercise 2: Log File
Create a `Logger` class that:
- Opens log file in constructor
- Writes messages with `log()` method
- Closes file in destructor

### Exercise 3: Scope Timer
Create a `ScopeTimer` class that:
- Records start time in constructor
- Prints elapsed time in destructor
- Use to time code blocks

### Exercise 4: Connection Pool
Create a `Connection` class that:
- Connects in constructor
- Disconnects in destructor
- Test with exceptions

---

## 📊 Visual Flow
```
Object Created
      │
      ▼
┌─────────────────┐
│  Constructor    │
│  Acquires       │
│  Resources      │
└────────┬────────┘
         │
         ▼
   Object Used
         │
         ▼
┌─────────────────┐
│  Scope Ends /   │
│  delete called  │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│  Destructor     │  ← AUTOMATIC!
│  Releases       │
│  Resources      │
└─────────────────┘
```

---

## 🚀 How to Run

### Compile:
```bash
g++ -std=c++17 destructor.cpp -o destructor
```

### Run:
```bash
./destructor
```

### Expected Output:
The program demonstrates:
1. Basic destructor behavior
2. When destructors are called
3. Memory cleanup
4. File handle cleanup
5. Destruction order
6. Composition and members
7. Exception safety
8. Array destruction
9. Heap vs stack
10. Virtual destructors
11. Database connections
12. RAII pattern

---

## 🔗 Comparison Tables

### Constructor vs Destructor

| Feature | Constructor | Destructor |
|---------|------------|------------|
| **Name** | `ClassName` | `~ClassName` |
| **Prefix** | None | `~` (tilde) |
| **Return Type** | None | None |
| **Parameters** | Yes (any) | No (none) |
| **Overloading** | Yes (multiple) | No (only one) |
| **Called When** | Object created | Object destroyed |
| **Purpose** | Initialize | Cleanup |
| **Can Throw** | Yes | No (dangerous!) |

### Stack vs Heap Destruction

| Aspect | Stack Object | Heap Object |
|--------|-------------|-------------|
| **Creation** | `MyClass obj;` | `new MyClass()` |
| **Destruction** | Automatic | Manual (`delete`) |
| **When** | Scope ends | `delete` called |
| **Can Forget** | No | Yes (memory leak) |
| **Exception-Safe** | Yes | No (without smart pointers) |
| **Recommended** | ✅ Preferred | ⚠️ Use smart pointers |

---

## 📚 Related Topics

### Prerequisites:
- **01_BasicClass** - Understanding classes
- **02_Constructor** - Object initialization

### Coming Next:
- **07_CopyConstructor** - Copying objects safely
- **08_MoveSemantics** - Efficient object transfer
- **13_Inheritance** - Base and derived classes
- **14_VirtualFunctions** - Polymorphism

### Related Concepts:
- **RAII Pattern** - Resource management idiom
- **Smart Pointers** - Automatic memory management
- **Rule of Three/Five** - Special member functions

---

## 🎓 Summary

### What We Learned:

✅ **Destructor** = Automatic cleanup when object destroyed
✅ **`~ClassName()`** syntax with tilde prefix
✅ **No parameters** and **no return type**
✅ **Automatically called** when object dies
✅ **Reverse order** of construction
✅ **Virtual** for base classes
✅ **Never throw** exceptions from destructor
✅ **RAII pattern** for resource management

### Destructor Syntax:
```cpp
class MyClass {
public:
    ~MyClass() {           // ~ prefix
        // Cleanup code    // No return type
    }                      // No parameters
};
```

### Why Use Destructors?

1. **Automatic Cleanup** - No manual resource management
2. **Exception-Safe** - Cleanup even with exceptions
3. **RAII Pattern** - Acquire in constructor, release in destructor
4. **No Leaks** - Resources always freed
5. **Reliable** - Can't forget cleanup
6. **Simple** - Just works automatically

---

## ✨ Quick Reference Card
```cpp
// BASIC DESTRUCTOR
class MyClass {
    int* data;
public:
    MyClass() {
        data = new int[100];
    }
    
    ~MyClass() {              // Destructor
        delete[] data;        // Free memory
    }
};

// VIRTUAL DESTRUCTOR (for inheritance)
class Base {
public:
    virtual ~Base() { }       // ← Must be virtual!
};

class Derived : public Base {
    int* data;
public:
    ~Derived() {
        delete[] data;
    }
};

// USAGE
void automatic() {
    MyClass obj;              // Stack
}  // Destructor called automatically

void manual() {
    MyClass* ptr = new MyClass();  // Heap
    delete ptr;               // Must manually delete
}

// MODERN WAY (smart pointers)
void modern() {
    auto ptr = make_unique<MyClass>();
}  // Automatically deleted!
```

---

## 🔍 Best Practices

### ✅ DO:

- **Free all resources** - Memory, files, connections, locks
- **Make noexcept** - Destructors should never throw (implicit)
- **Make virtual in base classes** - For proper polymorphic deletion
- **Keep simple** - Don't do complex operations
- **Use RAII** - Constructor acquires, destructor releases
- **Prefer stack objects** - Automatic cleanup
- **Use smart pointers** - For heap objects

### ❌ DON'T:

- **Throw exceptions** - Can cause program termination
- **Call virtual functions** - Object partially destroyed
- **Access other objects** - They might be destroyed
- **Do complex operations** - Keep cleanup simple
- **Forget to free resources** - Causes leaks
- **Make non-virtual in base** - Causes leaks in derived
- **Double delete** - Causes crashes

---

## 🌟 The Power of RAII

RAII makes C++ unique among programming languages:
```cpp
// Before RAII (C style):
void oldWay() {
    FILE* file = fopen("data.txt", "r");
    // ... use file ...
    if (error) {
        fclose(file);  // Must remember!
        return;
    }
    // ... more code ...
    fclose(file);  // Must remember again!
}

// With RAII (C++ style):
void modernWay() {
    ifstream file("data.txt");
    // ... use file ...
    if (error) {
        return;  // File automatically closed!
    }
    // ... more code ...
}  // File automatically closed!
```

**This is C++'s superpower!** 🦸

---

## 📖 Further Reading

- [cppreference.com - Destructors](https://en.cppreference.com/w/cpp/language/destructor)
- [C++ Core Guidelines - RAII](https://isocpp.github.io/CppCoreGuidelines/)
- "Effective C++" by Scott Meyers - Item 7: Virtual destructors

---

## 🎯 Key Takeaways

1. Destructors **automatically clean up** resources
2. Called when object is **destroyed** (scope, delete, exception)
3. **Reverse order** of construction
4. **Virtual in base classes** for polymorphism
5. **Never throw exceptions** from destructor
6. **RAII pattern** = Constructor acquires, destructor releases
7. **Stack objects** are automatic and safe
8. **Smart pointers** for heap objects
9. Destructors make C++ **exception-safe** by default
10. The foundation of **modern C++**

---

**Previous Topic:** [02_Constructor](../02_Constructor/) - Object Initialization

**Next Topic:** [04_MemberFunctions](../04_MemberFunctions/) - Class Methods

---

*Part of the C++ Classes and Objects series*
*Difficulty: Beginner to Intermediate*
*Prerequisites: 01_BasicClass, 02_Constructor*