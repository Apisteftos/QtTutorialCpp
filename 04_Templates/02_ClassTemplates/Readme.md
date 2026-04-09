# ClassTemplates - Generic Classes and Containers

## 📚 Overview

This folder introduces **Class Templates** - the foundation of generic container classes in C++. Class templates allow you to create reusable classes that work with any type, providing type-safe data structures with zero runtime overhead. They're the backbone of the Standard Template Library (STL) containers like vector, list, and map.

**Class Templates** = Generic classes for any type

This is **ESSENTIAL** for understanding STL and creating reusable containers.

---

## 🎯 What You'll Learn

- What class templates are and why they exist
- Template class syntax and instantiation
- Member function definitions (inside and outside class)
- Multiple template parameters
- Default template parameters
- Non-type template parameters
- Full and partial specialization
- Static members in templates
- Friend functions with templates
- Real-world container implementation
- Best practices and common pitfalls

---

## 📖 Concept Explanation

### What are Class Templates?

**Class templates** are blueprints for creating generic classes that work with any type. Instead of writing separate container classes for int, double, string, etc., you write one template that the compiler uses to generate type-specific classes.

```cpp
// One template for all types
template<typename T>
class Box {
private:
    T value;
public:
    Box(T v) : value(v) { }
    T getValue() const { return value; }
};

// Use with any type!
Box<int> intBox(42);           // Box for int
Box<double> doubleBox(3.14);   // Box for double
Box<string> stringBox("Hi");   // Box for string
```

**Key Properties:**
- Template is a blueprint, not actual code
- Each instantiation creates separate class
- Type-safe containers
- Compile-time generation
- Zero runtime overhead
- Foundation of STL

### How Class Templates Work

```
COMPILATION PROCESS:

1. Template Definition
   ┌──────────────────────┐
   │ template<typename T> │
   │ class Box {         │
   │     T value;         │
   │ public:             │
   │     Box(T v) {...}   │
   │ };                   │
   └──────────────────────┘
         Blueprint

2. Instantiation
   ┌──────────────────────┐
   │ Box<int> b1(42);    │
   │ Box<string> b2("Hi");│
   └──────────────────────┘
         Trigger

3. Compiler Generates
   ┌──────────────────────┐
   │ class Box_int {     │
   │     int value;       │
   │     ...              │
   │ };                   │
   │                      │
   │ class Box_string {  │
   │     string value;    │
   │     ...              │
   │ };                   │
   └──────────────────────┘
         Actual classes

4. Result
   One template → Multiple classes
   Generated at compile time
   Each type is separate class
   Independent static members
```

### Real-World Analogy

```
Think of class templates like a mold:

WITHOUT TEMPLATES (Separate Molds):
  - One mold for plastic bottles
  - One mold for glass bottles
  - One mold for metal bottles
  - Different mold for each material!

WITH TEMPLATES (Universal Mold):
  - One mold design (template)
  - Works with any material (type)
  - Same shape, different materials
  - Plastic, glass, metal, ceramic...

The template is the mold design,
the type is the material,
the result is a specific bottle! 🏺
```

---

## 💻 Basic Syntax

### Simple Class Template

```cpp
template<typename T>
class Box {
private:
    T value;
    
public:
    Box(T v) : value(v) { }
    
    void setValue(T v) { value = v; }
    T getValue() const { return value; }
};

// Instantiation
Box<int> intBox(42);
Box<string> strBox("Hello");
```

### Member Functions Outside Class

```cpp
template<typename T>
class Box {
private:
    T value;
public:
    Box(T v);           // Declaration
    T getValue() const; // Declaration
};

// Definitions outside class
template<typename T>
Box<T>::Box(T v) : value(v) { }

template<typename T>
T Box<T>::getValue() const {
    return value;
}
```

**Important:** Need `template<typename T>` prefix and `ClassName<T>::` syntax

### Instantiation

```cpp
// Explicit instantiation
Box<int> b1(42);        // Creates Box class for int
Box<double> b2(3.14);   // Creates Box class for double

// Each type is separate class
Box<int> != Box<double>
```

---

## 🔍 Key Concepts

### 1. Multiple Template Parameters

Different types for different purposes:

```cpp
template<typename K, typename V>
class KeyValue {
private:
    K key;
    V value;
    
public:
    KeyValue(K k, V v) : key(k), value(v) { }
    
    K getKey() const { return key; }
    V getValue() const { return value; }
};

// Usage
KeyValue<int, string> kv1(1, "One");
KeyValue<string, double> kv2("Pi", 3.14);
```

### 2. Default Template Parameters

Provide sensible defaults:

```cpp
template<typename T, typename Container = vector<T>>
class Stack {
private:
    Container data;
    
public:
    void push(const T& value) { data.push_back(value); }
    void pop() { data.pop_back(); }
};

// Use default
Stack<int> s1;  // Uses vector<int>

// Override default
Stack<int, deque<int>> s2;  // Uses deque<int>
```

### 3. Non-Type Template Parameters

Compile-time constants as parameters:

```cpp
template<typename T, int Size>
class Array {
private:
    T data[Size];  // Fixed size at compile time
    
public:
    int size() const { return Size; }
    
    T& operator[](int index) { return data[index]; }
};

// Usage
Array<int, 5> arr1;     // int[5]
Array<double, 10> arr2; // double[10]

// Different types!
Array<int, 5> != Array<int, 10>
```

**Allowed non-type parameters:**
- Integral types (int, char, bool)
- Enums
- Pointers
- References
- Must be compile-time constants

### 4. Full Specialization

Complete custom implementation for specific type:

```cpp
// Generic template
template<typename T>
class Printer {
public:
    void print(const T& value) {
        cout << "Generic: " << value << "\n";
    }
};

// Full specialization for bool
template<>
class Printer<bool> {
public:
    void print(const bool& value) {
        cout << "Bool: " << (value ? "true" : "false") << "\n";
    }
};

// Usage
Printer<int> p1;
p1.print(42);  // Uses generic

Printer<bool> p2;
p2.print(true);  // Uses specialization
```

### 5. Partial Specialization

Specialize based on pattern (class templates only):

```cpp
// Primary template
template<typename T1, typename T2>
class Pair {
public:
    void describe() { cout << "Two different types\n"; }
};

// Partial specialization: both types same
template<typename T>
class Pair<T, T> {
public:
    void describe() { cout << "Both types same\n"; }
};

// Partial specialization: second is pointer
template<typename T1, typename T2>
class Pair<T1, T2*> {
public:
    void describe() { cout << "Second is pointer\n"; }
};

// Usage
Pair<int, double> p1;  // Primary template
Pair<int, int> p2;     // Both same specialization
Pair<int, double*> p3; // Pointer specialization
```

### 6. Static Members

Each instantiation has own static members:

```cpp
template<typename T>
class Counter {
private:
    static int count;  // Static member
    
public:
    Counter() { count++; }
    ~Counter() { count--; }
    static int getCount() { return count; }
};

// Definition (required!)
template<typename T>
int Counter<T>::count = 0;

// Usage
Counter<int> c1;    // count for Counter<int>
Counter<int> c2;    // Same count
Counter<string> s1; // Separate count for Counter<string>

cout << Counter<int>::getCount();    // 2
cout << Counter<string>::getCount(); // 1
```

**Important:** Each instantiation has its own static member!

---

## 📋 Examples in This Folder

### Example 1: Basic Class Template
Simple generic container.

### Example 2: Member Functions Outside
Defining members outside class.

### Example 3: Multiple Template Parameters
Different types in one template.

### Example 4: Default Template Parameters
Sensible defaults for convenience.

### Example 5: Non-Type Template Parameters
Compile-time constants.

### Example 6: Template Specialization
Custom implementation for types.

### Example 7: Partial Specialization
Pattern-based specialization.

### Example 8: Static Members
Per-instantiation static data.

### Example 9: Friend Functions
Template friend functions.

### Example 10: Real-World Container
Complete dynamic array implementation.

---

## 🎓 When to Use Class Templates

### ✅ Use Class Templates When:

**Generic containers:**
```cpp
template<typename T>
class Vector {
    T* data;
    // Works with any type
};
```

**Type-independent data structures:**
```cpp
template<typename T>
class LinkedList {
    struct Node { T data; Node* next; };
    // Generic linked list
};
```

**RAII wrappers:**
```cpp
template<typename T>
class UniquePtr {
    T* ptr;
    // Smart pointer for any type
};
```

**Algorithm + data:**
```cpp
template<typename T>
class SortedArray {
    // Container with built-in sorting
};
```

### ❌ Don't Use When:

- Type-specific logic needed
- Runtime polymorphism sufficient
- Simple wrapper around one type
- Adds unnecessary complexity

---

## ⚠️ Common Mistakes

### ❌ Mistake 1: Definition in .cpp File

**Bad:**
```cpp
// box.h
template<typename T>
class Box {
    T value;
public:
    void set(T v);
};

// box.cpp
template<typename T>
void Box<T>::set(T v) {  // ❌ Linker error!
    value = v;
}
```

**Problem:** Template definitions must be visible at instantiation

**Fix:** Keep everything in header
```cpp
// box.h
template<typename T>
class Box {
    T value;
public:
    void set(T v) { value = v; }  // ✅ Inline
};

// OR
template<typename T>
void Box<T>::set(T v) { value = v; }  // ✅ In header
```

### ❌ Mistake 2: Forgetting Template Prefix

**Bad:**
```cpp
template<typename T>
class Box {
public:
    T getValue() const;
};

T Box<T>::getValue() const {  // ❌ Error!
    return value;
}
```

**Fix:**
```cpp
template<typename T>  // ✅ Need this!
T Box<T>::getValue() const {
    return value;
}
```

### ❌ Mistake 3: Wrong Syntax for Nested Types

**Bad:**
```cpp
template<typename T>
class Container {
public:
    class Iterator { };
};

Container<int>::Iterator it;  // ❌ May not work
```

**Fix:**
```cpp
typename Container<int>::Iterator it;  // ✅ Use typename
```

### ❌ Mistake 4: Ignoring Copy/Move Semantics

**Bad:**
```cpp
template<typename T>
class Container {
    T* data;
public:
    Container(T* d) : data(d) { }
    // ❌ No copy constructor, destructor, etc.
};
```

**Fix:** Follow Rule of 3/5/0
```cpp
template<typename T>
class Container {
    T* data;
public:
    ~Container() { delete[] data; }
    Container(const Container& other);  // Copy ctor
    Container& operator=(const Container& other);
    // etc.
};
```

---

## 🎯 Practice Exercises

### Exercise 1: Generic Stack
Implement a template stack with push/pop/top.

### Exercise 2: Template Pair
Create a pair class with two different types.

### Exercise 3: Fixed Array
Template array with compile-time size.

### Exercise 4: Smart Pointer
Implement a basic unique_ptr template.

### Exercise 5: Specialized Container
Create a container with specialization for bool.

---

## 🚀 How to Run

### Compile:
```bash
g++ -std=c++17 class_templates.cpp -o class_templates
```

### Run:
```bash
./class_templates
```

### Expected Output:
The program demonstrates:
1. Basic class template
2. Member functions outside class
3. Multiple template parameters
4. Default parameters
5. Non-type parameters
6. Full specialization
7. Partial specialization
8. Static members
9. Friend functions
10. Real-world dynamic array

---

## 📊 Visual Concepts

### Template Instantiation

```
TEMPLATE:
┌───────────────────┐
│ template<T>       │
│ class Box {       │
│     T value;      │
│ };                │
└───────────────────┘
         │
         │ Usage
         │
    ┌────┴────┬──────────┬──────────┐
    │         │          │          │
    ▼         ▼          ▼          ▼
┌────────┐┌────────┐┌──────────┐┌─────────┐
│Box<int>││Box<dbl>││Box<str>  ││Box<char>│
│        ││        ││          ││         │
│int val ││dbl val ││string val││char val │
└────────┘└────────┘└──────────┘└─────────┘

One template → Many classes
Each type = Separate class
Independent static members
```

### Member Function Syntax

```
INSIDE CLASS:
┌────────────────────────┐
│ template<typename T>   │
│ class Box {           │
│     T get() const {    │
│         return value;  │ ← Inline
│     }                  │
│ };                     │
└────────────────────────┘

OUTSIDE CLASS:
┌────────────────────────┐
│ template<typename T>   │ ← Prefix needed!
│ T Box<T>::get() const {│ ← Box<T>:: syntax
│     return value;      │
│ }                      │
└────────────────────────┘
```

### Static Members

```
template<typename T>
class Counter {
    static int count;
};

Counter<int> i1;  ┐
Counter<int> i2;  ├─→ count = 2 (shared)
                  ┘
Counter<double> d1; ┐
Counter<double> d2; ├─→ count = 2 (separate!)
                    ┘

Each instantiation = Own static member
```

---

## 📚 Related Topics

### Prerequisites:
- **32_FunctionTemplates** - Function templates
- **Classes** - Class basics
- **Constructors** - Object creation

### Coming Next:
- **34_TemplateSpecialization** - Advanced specialization
- **35_VariadicTemplates** - Variable arguments
- **36_SFINAE** - Template metaprogramming

### Related Concepts:
- **STL Containers** - Built on templates
- **Smart Pointers** - Template-based RAII
- **Generic Programming** - Paradigm

---

## 🎓 Summary

### What We Learned:

✅ **Class templates** are generic class blueprints
✅ **Each instantiation** creates separate class
✅ **Keep definitions** in headers
✅ **Multiple parameters** for flexibility
✅ **Default parameters** for convenience
✅ **Non-type parameters** for compile-time values
✅ **Specialization** for custom behavior
✅ **Static members** per instantiation
✅ **Zero overhead** - compile-time only
✅ **Foundation of STL** containers

### The Complete Pattern:

```cpp
// ============================================
// CLASS TEMPLATE COMPLETE EXAMPLE
// ============================================

template<typename T>
class Container {
private:
    T* data;
    size_t capacity;
    size_t length;
    
public:
    // Constructor
    Container() : data(nullptr), capacity(0), length(0) { }
    
    // Destructor
    ~Container() { delete[] data; }
    
    // Copy constructor
    Container(const Container& other) {
        capacity = other.capacity;
        length = other.length;
        data = new T[capacity];
        for (size_t i = 0; i < length; ++i) {
            data[i] = other.data[i];
        }
    }
    
    // Copy assignment
    Container& operator=(const Container& other) {
        if (this != &other) {
            delete[] data;
            capacity = other.capacity;
            length = other.length;
            data = new T[capacity];
            for (size_t i = 0; i < length; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }
    
    // Move constructor
    Container(Container&& other) noexcept
        : data(other.data), capacity(other.capacity), 
          length(other.length) {
        other.data = nullptr;
        other.capacity = 0;
        other.length = 0;
    }
    
    // Move assignment
    Container& operator=(Container&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            capacity = other.capacity;
            length = other.length;
            other.data = nullptr;
            other.capacity = 0;
            other.length = 0;
        }
        return *this;
    }
    
    // Add element
    void push_back(const T& value) {
        if (length >= capacity) {
            resize();
        }
        data[length++] = value;
    }
    
    // Access element
    T& operator[](size_t index) {
        return data[index];
    }
    
    const T& operator[](size_t index) const {
        return data[index];
    }
    
    // Size
    size_t size() const { return length; }
    
private:
    void resize() {
        capacity = capacity == 0 ? 1 : capacity * 2;
        T* newData = new T[capacity];
        for (size_t i = 0; i < length; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
    }
};

// Usage
Container<int> ints;
ints.push_back(42);

Container<string> strings;
strings.push_back("Hello");
```

---

## ✨ Quick Reference Card

```cpp
// === BASIC SYNTAX ===
template<typename T>
class Box {
    T value;
public:
    Box(T v) : value(v) { }
    T get() const { return value; }
};

// === MEMBER OUTSIDE ===
template<typename T>
T Box<T>::get() const {
    return value;
}

// === MULTIPLE PARAMS ===
template<typename K, typename V>
class Map { };

// === DEFAULT PARAM ===
template<typename T, typename C = vector<T>>
class Stack { };

// === NON-TYPE PARAM ===
template<typename T, int Size>
class Array { };

// === SPECIALIZATION ===
template<>
class Box<bool> { };

// === PARTIAL SPECIALIZATION ===
template<typename T>
class Pair<T, T> { };

// === STATIC MEMBER ===
template<typename T>
class Counter {
    static int count;
};

template<typename T>
int Counter<T>::count = 0;

// === INSTANTIATION ===
Box<int> b1(42);
Box<string> b2("Hi");
```

---

## 🔍 Best Practices

### ✅ DO:

- **Keep in headers** - full definition visible
- **Follow Rule of 3/5/0** - copy/move semantics
- **Use const references** - avoid copies
- **Document requirements** - what T needs
- **Provide defaults** - sensible parameters
- **Test multiple types** - int, double, string, custom
- **Use type traits** - enable_if, is_integral
- **Consider concepts** - C++20 constraints
- **Handle exceptions** - strong guarantee
- **Optimize for common** cases

### ❌ DON'T:

- **Put in .cpp files** - causes linker errors
- **Forget template prefix** - on external definitions
- **Ignore copy semantics** - memory leaks
- **Make assumptions** - about type T
- **Over-complicate** - keep it simple
- **Forget const-correctness** - const member functions
- **Create circular** dependencies
- **Ignore warnings** - template errors cryptic
- **Skip testing** - with various types
- **Assume performance** - profile first

---

## 🌟 Real-World Applications

### STL Containers
```cpp
template<typename T>
class vector {
    T* data;
    size_t size, capacity;
    // Dynamic array
};
```

### Smart Pointers
```cpp
template<typename T>
class unique_ptr {
    T* ptr;
    // RAII wrapper
};
```

### Optional Values
```cpp
template<typename T>
class optional {
    bool has_value;
    T value;
    // May or may not have value
};
```

### Thread-Safe Queue
```cpp
template<typename T>
class ThreadSafeQueue {
    queue<T> data;
    mutable mutex mtx;
    // Thread-safe operations
};
```

---

## 📖 Further Reading

- [cppreference.com - Class templates](https://en.cppreference.com/w/cpp/language/class_template)
- "C++ Templates: The Complete Guide" by Vandevoorde & Josuttis
- "Effective C++" by Scott Meyers - Template items
- [Template Tutorial](https://isocpp.org/wiki/faq/templates)

---

## 🎯 Key Takeaways

1. **Class templates** = generic classes
2. **Each type** creates separate class
3. **Keep definitions** in headers
4. **Instantiation** at compile time
5. **Multiple parameters** allowed
6. **Specialization** for custom behavior
7. **Static members** per type
8. **Follow Rule of 3/5/0**
9. **Foundation of STL** containers
10. **Zero runtime overhead**

---

## 🚨 THE GOLDEN RULES

```
⚠️  RULE 1: Keep definitions in headers
   Compiler needs full definition!
   
   All template code in .h files

⚠️  RULE 2: Each type = separate class
   Box<int> is different from Box<double>
   
   Separate classes, separate statics

⚠️  RULE 3: Member function syntax
   Outside class needs template prefix!
   
   template<typename T>
   T Box<T>::get() const { }

⚠️  RULE 4: Follow Rule of 3/5/0
   Handle copy/move correctly!
   
   Define destructor, copy, move

⚠️  RULE 5: Document type requirements
   What must T support?
   
   T needs operator<, copy ctor, etc.
   
   USE CLASS TEMPLATES FOR REUSABLE CONTAINERS!
```

---

**Previous Topic:** [32_FunctionTemplates](../32_FunctionTemplates/) - Function Templates

**Next Topic:** [34_TemplateSpecialization](../34_TemplateSpecialization/) - Advanced Specialization

---

*Part of the Templates and Generic Programming series*
*Difficulty: Intermediate-Advanced*
*Prerequisites: Function templates, classes*
*CRITICAL for Understanding STL*