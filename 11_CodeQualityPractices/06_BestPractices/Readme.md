# C++ Best Practices

## 📚 Overview

The **C++ Best Practices** are:

> **"Follow proven patterns and idioms that make C++ code safer, more efficient, and maintainable."**  
> — Modern C++ Guidelines

### What It Means

- **Use language features correctly** - Leverage C++ properly
- **Prevent common bugs** - Memory leaks, crashes, undefined behavior
- **Write safe code** - Exception-safe, resource-safe
- **Follow idioms** - RAII, Rule of Five, const correctness
- **Leverage type system** - Let compiler help you

### Why It Matters

✅ **Prevents memory leaks** - No forgotten cleanup  
✅ **Eliminates crashes** - No dangling pointers, double-deletes  
✅ **Exception-safe** - Resources cleaned up automatically  
✅ **Better performance** - Avoid unnecessary copies  
✅ **Easier maintenance** - Clear ownership and intent  
✅ **Compiler enforced** - Catch errors at compile time  

---

## 🚨 Violation Signs

How to spot violations of best practices:

- 🚨 Memory leaks detected
- 🚨 Crashes from double-delete
- 🚨 Dangling pointer dereferences
- 🚨 Resources not released (files, sockets, locks)
- 🚨 Uninitialized variable bugs
- 🚨 Null pointer dereferences
- 🚨 Raw `new`/`delete` everywhere
- 🚨 Pass large objects by value

---

## 💎 The Eight Essential Practices

```
C++ Best Practices:
1. RAII - Resource Acquisition Is Initialization
2. Rule of Zero/Three/Five
3. Const Correctness
4. Initialize All Variables
5. Prefer References Over Pointers
6. Use Smart Pointers
7. Avoid Raw new/delete
8. Pass by Const Reference
```

---

## 🏗️ Practice 1: RAII (Resource Acquisition Is Initialization)

### The Principle

> **"Resource lifetime is tied to object lifetime."**  
> Resources are acquired in constructor, released in destructor.

### ❌ **Bad: Manual Resource Management**

```cpp
void processFile() {
    FILE* file = fopen("data.txt", "r");
    if (!file) {
        return;  // OK here
    }
    
    char buffer[100];
    if (!fgets(buffer, 100, file)) {
        return;  // 🚨 LEAK! Forgot to close file
    }
    
    try {
        if (error) {
            throw runtime_error("Error");  // 🚨 LEAK!
        }
    } catch (...) {
        throw;  // 🚨 Forgot to close file!
    }
    
    fclose(file);  // Only reached in happy path
}
```

### ✅ **Good: RAII**

```cpp
class FileHandle {
private:
    FILE* file;
    
public:
    FileHandle(const string& filename, const char* mode) {
        file = fopen(filename.c_str(), mode);
        if (!file) {
            throw runtime_error("Can't open file");
        }
    }
    
    ~FileHandle() {
        if (file) {
            fclose(file);  // ALWAYS called!
        }
    }
    
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;
    
    FILE* get() { return file; }
};

void processFile() {
    FileHandle file("data.txt", "r");
    
    // Process file...
    // Exception thrown? File ALWAYS closed!
    
}  // Destructor ALWAYS called
```

### 🎯 RAII Guidelines

- **Acquire resource in constructor**
- **Release resource in destructor**
- **Delete copy operations** (or implement correctly)
- **Lifetime tied to scope** - automatic cleanup
- **Works with stack unwinding** - exception-safe

---

## 🔢 Practice 2: Rule of Zero/Three/Five

### The Rules

**Rule of Zero (Preferred):**
> Use standard containers and let the compiler handle everything.

**Rule of Five (C++11+):**
> If you define one of: Destructor, Copy Constructor, Copy Assignment, Move Constructor, Move Assignment  
> You must define all five.

### ❌ **Bad: Violates Rule**

```cpp
class Buffer {
private:
    int* data;
    size_t size;
    
public:
    Buffer(size_t s) : size(s) {
        data = new int[size];
    }
    
    ~Buffer() {
        delete[] data;
    }
    
    // 🚨 NO COPY CONSTRUCTOR!
    // 🚨 NO COPY ASSIGNMENT!
};

Buffer buf1(10);
Buffer buf2 = buf1;  // 🚨 Shallow copy! CRASH!
```

### ✅ **Good: Rule of Zero**

```cpp
class Buffer {
private:
    std::vector<int> data;  // ✅ Perfect!
    
public:
    Buffer(size_t size) : data(size) {}
    
    // ✅ Compiler generates everything perfectly
};
```

### ✅ **Good: Rule of Five**

```cpp
class Buffer {
private:
    int* data;
    size_t size;
    
public:
    // Constructor
    Buffer(size_t s) : size(s), data(new int[s]) {}
    
    // Destructor
    ~Buffer() { delete[] data; }
    
    // Copy Constructor
    Buffer(const Buffer& other) : size(other.size) {
        data = new int[size];
        std::copy(other.data, other.data + size, data);
    }
    
    // Copy Assignment
    Buffer& operator=(const Buffer& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = new int[size];
            std::copy(other.data, other.data + size, data);
        }
        return *this;
    }
    
    // Move Constructor
    Buffer(Buffer&& other) noexcept 
        : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
    }
    
    // Move Assignment
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
        }
        return *this;
    }
};
```

---

## 🔒 Practice 3: Const Correctness

### The Principle

> **"Mark as const anything that doesn't change."**  
> Let the compiler enforce immutability.

### ❌ **Bad: No Const**

```cpp
class Person {
public:
    string getName() { return name; }  // 🚨 Not const!
    void print() { }                   // 🚨 Not const!
};

void func(Person& p) {  // 🚨 Can't accept const!
    p.print();
}
```

### ✅ **Good: Const Correct**

```cpp
class Person {
public:
    string getName() const { return name; }  // ✅
    void print() const { }                   // ✅
    void setName(const string& n) { }        // ✅
};

void func(const Person& p) {  // ✅ Can accept const
    p.print();  // Works!
}
```

---

## 🎯 Practice 4: Initialize All Variables

### The Principle

> **"Always initialize variables at declaration."**  
> Prevent undefined behavior.

### ❌ **Bad: Uninitialized**

```cpp
void process() {
    int count;      // 🚨 Garbage!
    double total;   // 🚨 Random!
    bool flag;      // 🚨 Could be anything!
    
    if (flag) {     // 🚨 Undefined behavior!
        count++;
    }
}
```

### ✅ **Good: Always Initialize**

```cpp
void process() {
    int count = 0;        // ✅
    double total = 0.0;   // ✅
    bool flag = false;    // ✅
    
    // Or use uniform initialization:
    int count{0};
    double total{0.0};
    bool flag{false};
}

class User {
private:
    string name;
    int age = 0;              // ✅ Default initializer
    bool active = true;       // ✅
    
public:
    User(const string& n) : name(n) {}
};
```

---

## 🔗 Practice 5: Prefer References Over Pointers

### The Principle

> **"Use references when value must exist."**  
> Use pointers only when optional.

### ❌ **Bad: Unnecessary Pointers**

```cpp
void processUser(User* user) {  // 🚨 Can be null!
    if (user) {                 // 🚨 Need check
        user->process();
    }
}

User* findUser(int id) {  // 🚨 Can return null
    return nullptr;       // Caller must check!
}
```

### ✅ **Good: Use References**

```cpp
void processUser(const User& user) {  // ✅ Can't be null!
    user.process();  // No null check needed
}

// For optional values, use std::optional
std::optional<User> findUser(int id) {
    if (id < 0) {
        return std::nullopt;  // Clear: no value
    }
    return User();
}
```

### When to Use Each

| Use References | Use Pointers |
|----------------|--------------|
| Value must exist | Value is optional |
| No ownership transfer | Ownership transfer |
| No reassignment | Need to reassign |
| Function parameters | Polymorphism |

---

## 🧠 Practice 6: Use Smart Pointers

### The Principle

> **"Never use raw pointers for ownership."**  
> Let smart pointers manage memory.

### ❌ **Bad: Raw Pointers**

```cpp
void createUser() {
    User* user = new User();  // 🚨 Who deletes?
    
    if (error) {
        return;  // 🚨 MEMORY LEAK!
    }
    
    delete user;
}
```

### ✅ **Good: Smart Pointers**

```cpp
void createUser() {
    std::unique_ptr<User> user = std::make_unique<User>();
    
    if (error) {
        return;  // ✅ Automatically deleted!
    }
    
}  // ✅ Automatically deleted
```

### Smart Pointer Types

**`unique_ptr`** - Exclusive Ownership
```cpp
std::unique_ptr<Widget> widget = std::make_unique<Widget>();
// Can't be copied, only moved
// Zero overhead
// Use by default
```

**`shared_ptr`** - Shared Ownership
```cpp
std::shared_ptr<Widget> widget1 = std::make_shared<Widget>();
std::shared_ptr<Widget> widget2 = widget1;  // Share ownership
// Reference counted
// Deleted when last reference goes away
```

**`weak_ptr`** - Non-Owning Reference
```cpp
std::weak_ptr<Widget> weakWidget = sharedWidget;
// Doesn't increase reference count
// Prevents circular references
// Must check if still valid before use

if (auto widget = weakWidget.lock()) {
    // Use widget
}
```

---

## 🚫 Practice 7: Avoid Raw new/delete

### The Principle

> **"Almost never use new/delete directly."**  
> Use containers and smart pointers.

### ❌ **Bad: Manual new/delete**

```cpp
void processData() {
    int* array = new int[100];
    
    for (int i = 0; i < 100; i++) {
        if (array[i] < 0) {
            return;  // 🚨 MEMORY LEAK!
        }
    }
    
    delete[] array;
}
```

### ✅ **Good: Use Containers**

```cpp
void processData() {
    std::vector<int> array(100);  // ✅ Automatic!
    
    for (int i = 0; i < 100; i++) {
        if (array[i] < 0) {
            return;  // ✅ No leak!
        }
    }
}  // ✅ Automatic cleanup
```

### Alternatives to new/delete

```cpp
std::vector<T>          // Dynamic array
std::string             // Text
std::array<T, N>        // Fixed-size array
std::unique_ptr<T>      // Single ownership
std::shared_ptr<T>      // Shared ownership
```

---

## 📦 Practice 8: Pass by Const Reference

### The Principle

> **"Pass large objects by const reference."**  
> Pass small types by value.

### ❌ **Bad: Pass by Value**

```cpp
void processVector(std::vector<int> v) {  // 🚨 COPIES entire vector!
    for (int x : v) {
        cout << x << " ";
    }
}

std::vector<int> data(10000);
processVector(data);  // 🚨 10000 ints copied!
```

### ✅ **Good: Pass by Const Reference**

```cpp
void processVector(const std::vector<int>& v) {  // ✅ No copy!
    for (int x : v) {
        cout << x << " ";
    }
}

std::vector<int> data(10000);
processVector(data);  // ✅ Just passes reference!
```

### Parameter Passing Guidelines

| Type | When | Example |
|------|------|---------|
| **By Value** | Small types (≤8 bytes) | `void func(int x)` |
| **By Const Reference** | Large types | `void func(const vector<int>& v)` |
| **By Non-Const Reference** | Modify parameter | `void func(vector<int>& v)` |
| **By Pointer** | Optional value | `void func(Widget* w)` |

---

## 🎯 Quick Reference

### Best Practices Summary

| Practice | Key Point | Default Choice |
|----------|-----------|----------------|
| **RAII** | Resource lifetime = object lifetime | Always |
| **Rule of Zero** | Use standard containers | Preferred |
| **Const** | Mark non-modifying as const | Always |
| **Initialize** | Initialize all variables | Always |
| **References** | Prefer over pointers | When must exist |
| **Smart Pointers** | Never own with raw pointers | Always |
| **No new/delete** | Use containers/smart pointers | Always |
| **Const Reference** | Pass large objects efficiently | For reading |

---

## 💡 Key Insights

### The Modern C++ Way

```cpp
// Old C++ (Pre-C++11):
Widget* w = new Widget();
try {
    w->doSomething();
} catch (...) {
    delete w;
    throw;
}
delete w;

// Modern C++ (C++11+):
auto w = std::make_unique<Widget>();
w->doSomething();
// Automatic cleanup, exception-safe!
```

---

## 💭 Inspiring Quotes

> "C++ is designed to allow you to express ideas, but if you don't use the right idioms, it will bite you."  
> — Bjarne Stroustrup

> "RAII is the most important idiom in C++."  
> — Herb Sutter

> "Make your destructors do the dirty work."  
> — Scott Meyers

---

## 📚 Recommended Reading

- **"Effective Modern C++" by Scott Meyers** - Essential practices
- **"C++ Core Guidelines" by Bjarne Stroustrup & Herb Sutter** - Official guidelines
- **"A Tour of C++" by Bjarne Stroustrup** - Modern C++ overview

---

## 🎓 Key Takeaways

1. **RAII is fundamental** - Tie resource lifetime to object lifetime
2. **Prefer Rule of Zero** - Let containers do the work
3. **Const correctness matters** - Let compiler enforce
4. **Always initialize** - No undefined behavior
5. **References over pointers** - When value must exist
6. **Smart pointers for ownership** - Never use raw new/delete
7. **Containers over arrays** - Automatic management
8. **Pass by const reference** - Avoid unnecessary copies

---

## 📝 Final Wisdom

> **"Modern C++ provides all the tools you need to write safe, efficient code. Use them!"**

Following these best practices:
- Eliminates entire classes of bugs
- Makes code exception-safe by default
- Improves performance
- Makes code more maintainable
- Leverages compiler to catch errors

**The return on investment is enormous!**

## Notes moved from main.cpp

This text used to be printed by `main()`. It is lecture material, so it lives here and the program only prints the demo output.

### 🎉 C++ BEST PRACTICES COMPLETE! 🎉 (closing summary)

Key Takeaway: Modern C++ provides powerful tools  
to write safe, efficient code. Use them!

## Corrections

- The original did not compile (undeclared `User`, invalid `if (/* some condition */)`). The missing declarations were added and the condition became a `bool someCondition = false;`.

### Text printed by the example functions

This text used to be printed between the demo steps. It is lecture material, so it lives here.

### From `example1_raii()`

- ❌ BAD CODE (Manual Resource Management):

```cpp
FILE* file = fopen("data.txt", "r");
if (!file) return;
// Do work...
if (error) {
    return;  // 🚨 LEAK! Forgot to close file
}
fclose(file);  // Only in happy path!
```

- ✅ GOOD CODE (RAII):

```cpp
class FileHandle {
    FILE* file;
```

public:

```cpp
    FileHandle(const string& name) {
        file = fopen(name.c_str(), "r");
    }
    ~FileHandle() {
        if (file) fclose(file);  // ALWAYS called!
    }
};
```

**✨ DEMONSTRATION:**

Database connection example:

### From `example1_raii()`

- 💡 RAII BENEFITS:
- ✅ Automatic cleanup - no leaks
- ✅ Exception-safe - destructor always runs
- ✅ No manual cleanup code needed
- ✅ Impossible to forget to release
- ✅ Scope-based lifetime management

**🎯 RAII PATTERN:**

  - Constructor acquires resource
  - Destructor releases resource
  - Lifetime tied to object scope
  - Works with stack unwinding

### From `example2_rule_of_five()`

**📜 THE RULES:**

RULE OF ZERO:

  - Use standard containers (vector, string, etc.)
  - Let compiler generate all special members
  - Preferred approach!

RULE OF THREE (C++98):  
If you define ONE of:

  - Destructor
  - Copy constructor
  - Copy assignment

You must define ALL THREE!

RULE OF FIVE (C++11+):  
If you define ONE of:

  - Destructor
  - Copy constructor
  - Copy assignment
  - Move constructor
  - Move assignment

You must define ALL FIVE!

- ❌ BAD CODE (Violates Rule of Three):

```cpp
class Buffer {
    int* data;
```

public:

```cpp
    ~Buffer() { delete[] data; }
    // 🚨 Missing copy constructor!
    // 🚨 Missing copy assignment!
    // Result: Double delete → CRASH!
};
```

- ✅ GOOD CODE (Rule of Zero):

```cpp
class Buffer {
    vector<int> data;  // ✅ Perfect!
```

public:

```cpp
    Buffer(size_t size) : data(size) {}
    // Compiler generates everything else!
};
```

**✨ DEMONSTRATION (Rule of Five):**

### From `example2_rule_of_five()`

- 💡 RECOMMENDATION:

🥇 First choice: Rule of Zero (use std containers)  
🥈 Only if needed: Rule of Five (full implementation)

- ⚠️  Never: Partial implementation!

### From `example3_const_correctness()`

- ❌ BAD CODE (No Const):

```cpp
class Person {
```

public:

```cpp
    string getName() { return name; }  // 🚨 Not const!
    void print() { /* ... */ }         // 🚨 Not const!
};
void func(Person& p) {  // 🚨 Can't accept const!
    p.print();
}
```

- ✅ GOOD CODE (Const Correct):

```cpp
class Person {
```

public:

```cpp
    string getName() const { return name; }  // ✅
    void print() const { /* ... */ }         // ✅
    void setName(const string& n) { }        // ✅
};
void func(const Person& p) {  // ✅ Can accept const
    p.print();  // Works!
}
```

**✨ DEMONSTRATION:**

### From `example3_const_correctness()`

- 💡 CONST CORRECTNESS RULES:
- ✅ Mark member functions const if they don't modify
- ✅ Pass by const reference when not modifying
- ✅ Use const for variables that don't change
- ✅ Provide both const and non-const overloads

**🎯 BENEFITS:**

  - Compiler enforces immutability
  - Self-documenting intent
  - Enables optimizations
  - Prevents accidental modifications

### From `example4_initialize_variables()`

- ❌ BAD CODE (Uninitialized):

```cpp
int count;      // 🚨 Contains garbage!
bool flag;      // 🚨 Random value!
double total;   // 🚨 Undefined!
if (flag) {     // 🚨 Undefined behavior!
    count++;    // 🚨 Adding to garbage!
}
```

- ✅ GOOD CODE (Always Initialize):

```cpp
int count = 0;        // ✅ Defined value
bool flag = false;    // ✅ Defined value
double total = 0.0;   // ✅ Defined value
// Or use uniform initialization:
int count{0};
bool flag{false};
double total{0.0};
```

**✨ DEMONSTRATION:**

### From `example4_initialize_variables()`

- 💡 INITIALIZATION METHODS:

1. Direct initialization:

```cpp
int x = 5;
```

2. Uniform initialization (C++11):

```cpp
int x{5};  // Preferred - prevents narrowing
```

3. Member initializer list:

```cpp
MyClass(int x) : member(x) { }
```

4. Default member initializers (C++11):

```cpp
class MyClass {
    int member = 0;  // Default value
};
```

**🎯 BENEFITS:**

- ✅ No undefined behavior
- ✅ No garbage values
- ✅ Predictable program behavior
- ✅ Easier debugging
- ✅ Catches errors at compile time

### From `example5_references_vs_pointers()`

- ❌ BAD CODE (Unnecessary Pointers):

```cpp
void func(User* user) {  // 🚨 Can be null!
    if (user) {          // 🚨 Need null check
        user->process();
    }
}
User* u = findUser(123);  // 🚨 Might be null
if (u) {                  // 🚨 Always checking
    func(u);
}
```

- ✅ GOOD CODE (Use References):

```cpp
void func(const User& user) {  // ✅ Can't be null!
    user.process();            // ✅ No check needed
}
User& u = getUser(123);  // ✅ Must exist
func(u);                 // ✅ No null checks
// For optional values:
optional<User> maybeUser = findUser(123);
if (maybeUser) { /* use *maybeUser */ }
```

**✨ DEMONSTRATION:**

### From `example5_references_vs_pointers()`

**📊 WHEN TO USE EACH:**

USE REFERENCES when:

- ✅ Value must exist (not optional)
- ✅ No ownership transfer
- ✅ No reassignment needed
- ✅ Passing to/from functions

USE POINTERS when:

- ⚠️  Value is optional (can be null)
- ⚠️  Need to reassign (point to different object)
- ⚠️  Polymorphism with ownership (use unique_ptr)
- ⚠️  Shared ownership (use shared_ptr)

**🎯 BENEFITS OF REFERENCES:**

- ✅ No null pointer dereferences
- ✅ Cleaner syntax (. instead of ->)
- ✅ No null checks needed
- ✅ Clear intent: value must exist

### From `example6_smart_pointers()`

- ❌ BAD CODE (Raw Pointers):

```cpp
User* user = new User();  // 🚨 Who deletes?
// Do work...
if (error) {
    return;  // 🚨 MEMORY LEAK!
}
delete user;  // Must remember!
```

- ✅ GOOD CODE (Smart Pointers):

```cpp
unique_ptr<User> user = make_unique<User>();
// Do work...
if (error) {
    return;  // ✅ Automatically deleted!
}
// ✅ Automatically deleted at end of scope
```

**✨ DEMONSTRATION:**

unique_ptr (exclusive ownership):

### From `example6_smart_pointers()`

- ✅ Deleted when last reference goes away

**📊 SMART POINTER TYPES:**

unique_ptr:

  - Exclusive ownership
  - Can't be copied, only moved
  - Zero overhead
  - Use by default

shared_ptr:

  - Shared ownership
  - Reference counted
  - Slight overhead
  - Use when multiple owners needed

weak_ptr:

  - Non-owning reference
  - Doesn't increase ref count
  - Prevents cycles
  - Must check if still valid

**🎯 BENEFITS:**

- ✅ No memory leaks
- ✅ Exception-safe
- ✅ Clear ownership semantics
- ✅ RAII automatic cleanup

### From `example7_avoid_raw_new_delete()`

- ❌ BAD CODE (Manual new/delete):

```cpp
int* array = new int[100];  // 🚨 Manual!
// Process...
if (error) {
    return;  // 🚨 LEAK! Forgot delete[]
}
delete[] array;  // Must remember!
```

- ✅ GOOD CODE (Use Containers):

```cpp
vector<int> array(100);  // ✅ Automatic!
// Process...
if (error) {
    return;  // ✅ No leak!
}
// ✅ Automatic cleanup
```

**✨ DEMONSTRATION:**

Using vector (preferred):

### From `example7_avoid_raw_new_delete()`

- ✅ Memory automatically freed

**🎯 ALTERNATIVES TO new/delete:**

1. Use standard containers:

```cpp
vector<T>     - Dynamic array
```

string        - Text

```cpp
array<T, N>   - Fixed-size array
map<K, V>     - Key-value pairs
```

2. Use smart pointers:

```cpp
unique_ptr<T> - Single ownership
shared_ptr<T> - Shared ownership
make_unique<T>() - Preferred creation
make_shared<T>() - Preferred creation
```

3. Use stack allocation:

```cpp
int array[100];  - Fixed size
MyClass obj;     - Local object
```

- 💡 BENEFITS:
- ✅ No memory leaks
- ✅ Exception-safe
- ✅ Less code to write
- ✅ No manual cleanup needed
- ✅ Rule of Zero achieved

### From `example8_pass_by_const_ref()`

- ❌ BAD CODE (Pass by Value):

```cpp
void processVector(vector<int> v) {  // 🚨 COPIES!
    // Entire vector copied unnecessarily
    for (int x : v) { /* ... */ }
}
vector<int> data(10000);  // Large vector
processVector(data);      // 🚨 10000 ints copied!
```

- ✅ GOOD CODE (Pass by Const Reference):

```cpp
void processVector(const vector<int>& v) {  // ✅ No copy!
    // Just a reference, no copying
    for (int x : v) { /* ... */ }
}
vector<int> data(10000);
processVector(data);  // ✅ Just passes reference!
```

**✨ DEMONSTRATION:**

### From `example8_pass_by_const_ref()`

**📊 PARAMETER PASSING GUIDELINES:**

Pass by VALUE:

  - Built-in types (int, double, char, bool)
  - Small types (sizeof <= 2*pointer)
  - When you need a copy anyway

Pass by CONST REFERENCE:

  - Large objects (vector, string, custom classes)
  - When reading only (not modifying)
  - Default for user-defined types

Pass by NON-CONST REFERENCE:

  - When function needs to modify argument
  - Output parameters

Pass by POINTER:

  - When value is optional (can be nullptr)
  - When need to reassign

Pass by RVALUE REFERENCE (&&):

  - For perfect forwarding
  - When taking ownership

**🎯 BENEFITS:**

- ✅ No unnecessary copies
- ✅ Better performance
- ✅ Const prevents accidental modification
- ✅ Clear intent (reading vs modifying)

## What was tested

- Compiled with GCC 13.3 (`-std=c++23`) using a stand-in `<print>` header, because `std::println` needs GCC 14 and it was not available here. The source files were compiled directly, not through CMake.
- The original does not compile, so there is no original output to compare against. The corrected version builds and runs.
- Run once with AddressSanitizer + UndefinedBehaviorSanitizer: no findings.

## Not tested

- The real `<print>` header (GCC 14+, recent Clang or MSVC) and platforms other than Linux x86-64.
