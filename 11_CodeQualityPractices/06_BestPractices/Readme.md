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