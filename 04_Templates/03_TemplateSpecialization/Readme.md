# TemplateSpecialization - Custom Behavior for Specific Types

## 📚 Overview

This folder introduces **Template Specialization** - the ability to provide custom implementations of templates for specific types. Specialization allows you to optimize, customize, or completely change template behavior for particular types while maintaining a generic implementation for everything else. This is essential for type-specific optimizations and is used extensively in the STL.

**Template Specialization** = Custom implementation for specific types

This is **ESSENTIAL** for advanced template programming and STL mastery.

---

## 🎯 What You'll Learn

- What template specialization is and why it exists
- Full (explicit) specialization
- Partial specialization (class templates only)
- Specialization priority rules
- Function vs class specialization
- Pointer and array specializations
- Type traits patterns
- Optimization techniques
- The vector<bool> pattern
- When to specialize vs alternatives
- Best practices and pitfalls

---

## 📖 Concept Explanation

### What is Template Specialization?

**Template specialization** allows you to provide a custom implementation of a template for specific types, overriding the generic version. This enables type-specific behavior, optimizations, and different data structures.

```cpp
// Generic template (works for all types)
template<typename T>
class Box {
    T data;
public:
    Box(T d) : data(d) { }
    T get() const { return data; }
};

// Specialized for bool (custom implementation)
template<>
class Box<bool> {
    char data;  // Use char instead of bool
public:
    Box(bool d) : data(d ? 1 : 0) { }
    bool get() const { return data != 0; }
};

// Usage
Box<int> intBox(42);     // Uses generic template
Box<bool> boolBox(true); // Uses specialization
```

**Types of Specialization:**
1. **Full (Explicit) Specialization** - Complete custom implementation for specific type
2. **Partial Specialization** - Custom implementation for patterns (e.g., pointers)

### How Specialization Works

```
TEMPLATE SELECTION:

1. Compiler sees usage
   Box<int> b1;
   Box<bool> b2;
   Box<int*> b3;
   
2. Checks specializations
   ┌─────────────────┐
   │ Full > Partial  │
   │ > Primary       │
   └─────────────────┘
   
3. Selects most specific
   Box<int>  → Primary template
   Box<bool> → Full specialization (if exists)
   Box<int*> → Partial specialization for T* (if exists)
   
4. Generates code
   Each gets appropriate implementation
```

### Real-World Analogy

```
Think of specialization like customer service:

GENERIC TEMPLATE (Standard Service):
  - Same process for all customers
  - Works fine for most cases
  - Default handling

SPECIALIZATION (VIP Service):
  - Special handling for VIP customers
  - Different process, better service
  - Customized experience
  - Still a customer, but different treatment

Regular customer → Generic template
VIP customer → Specialized template
```

---

## 💻 Basic Syntax

### Full Specialization (Function)

```cpp
// Generic template
template<typename T>
bool isEqual(T a, T b) {
    return a == b;
}

// Full specialization for double
template<>
bool isEqual<double>(double a, double b) {
    const double epsilon = 0.0001;
    return abs(a - b) < epsilon;  // Epsilon comparison
}

// Usage
isEqual(5, 5);         // Generic
isEqual(3.14, 3.14);   // Specialized (epsilon)
```

### Full Specialization (Class)

```cpp
// Generic template
template<typename T>
class Container {
    T data;
public:
    Container(T d) : data(d) { }
};

// Full specialization for const char*
template<>
class Container<const char*> {
    char* data;  // Different data structure!
public:
    Container(const char* d) {
        data = new char[strlen(d) + 1];
        strcpy(data, d);
    }
    ~Container() { delete[] data; }
};
```

### Partial Specialization (Class Only!)

```cpp
// Primary template
template<typename T>
class Container { };

// Partial specialization for pointers
template<typename T>
class Container<T*> {  // T is still generic
    // Special handling for any pointer type
};

// Partial specialization for const pointers
template<typename T>
class Container<const T*> {
    // Special handling for const pointers
};

// Usage
Container<int> c1;       // Primary
Container<int*> c2;      // Partial (T*)
Container<const int*> c3; // Partial (const T*)
```

**Note:** Functions do NOT support partial specialization!

---

## 🔍 Key Concepts

### 1. Full Specialization

Complete custom implementation for a specific type:

```cpp
template<typename T>
class Storage {
    T value;
public:
    Storage(T v) : value(v) { }
    T get() const { return value; }
};

// Completely different implementation for bool
template<>
class Storage<bool> {
    char value;  // Different data type
public:
    Storage(bool v) : value(v ? 1 : 0) { }
    bool get() const { return value != 0; }
    // Can have different methods too
};
```

**Characteristics:**
- `template<>` syntax
- Must specify exact type
- Complete class/function redefinition
- Can be completely different

### 2. Partial Specialization (Classes Only)

Specialize for patterns, not specific types:

```cpp
// Primary
template<typename T1, typename T2>
class Pair { };

// Partial: both types same
template<typename T>
class Pair<T, T> {  // Pattern: same type
    // T is still generic
};

// Partial: second is pointer
template<typename T1, typename T2>
class Pair<T1, T2*> {  // Pattern: pointer
    // T1 and T2 still generic
};

// Usage
Pair<int, double> p1;  // Primary
Pair<int, int> p2;     // Partial (both same)
Pair<int, double*> p3; // Partial (pointer)
```

**Common Patterns:**
- Pointers: `T*`
- Const pointers: `const T*`
- References: `T&`
- Arrays: `T[]` or `T[N]`
- Multiple params matching: `<T, T>`

### 3. Specialization Priority

When multiple specializations match, compiler picks most specific:

```cpp
template<typename T1, typename T2>
class Pair { };                    // 1. Primary (least specific)

template<typename T>
class Pair<T, T> { };              // 2. Partial

template<typename T>
class Pair<int, T> { };            // 3. Partial (more specific)

template<>
class Pair<int, int> { };          // 4. Full (most specific)

// Selection
Pair<double, string> p1;  → Primary
Pair<double, double> p2;  → Partial (T, T)
Pair<int, double> p3;     → Partial (int, T)
Pair<int, int> p4;        → Full (most specific!)
```

**Priority Order:**
1. Full specialization (most specific)
2. Partial specialization
3. Primary template (least specific)

### 4. Pointer Specializations

Very common pattern:

```cpp
// Primary
template<typename T>
class SmartPtr {
public:
    void info() { cout << "Regular type\n"; }
};

// Partial for pointers
template<typename T>
class SmartPtr<T*> {
public:
    void info() { cout << "Pointer type\n"; }
    T& operator*() { /* dereference */ }
    T* operator->() { /* arrow */ }
};

SmartPtr<int> s1;   // Regular
SmartPtr<int*> s2;  // Pointer specialization
```

### 5. Type Traits Pattern

Foundation of `<type_traits>` library:

```cpp
// Primary: not a pointer
template<typename T>
struct IsPointer {
    static const bool value = false;
};

// Partial: is a pointer
template<typename T>
struct IsPointer<T*> {
    static const bool value = true;
};

// Usage
IsPointer<int>::value;   // false
IsPointer<int*>::value;  // true
IsPointer<int**>::value; // true (int** matches T*)
```

---

## 📋 Examples in This Folder

### Example 1: Basic Function Specialization
Epsilon comparison for doubles.

### Example 2: Basic Class Specialization
Memory management for strings.

### Example 3: Partial - Pointers
Special handling for pointer types.

### Example 4: Partial - Arrays
Detecting and handling arrays.

### Example 5: Multiple Parameters
Various specialization patterns.

### Example 6: Type Traits
Compile-time type information.

### Example 7: Optimization
Performance improvements for specific types.

### Example 8: Vector<bool> Pattern
Space optimization with bit packing.

### Example 9: Member Function Specialization
Specializing individual members.

### Example 10: Alternatives
When not to use specialization.

---

## 🎓 When to Use Specialization

### ✅ Use Specialization For:

**Different data structures:**
```cpp
template<typename T>
class Vector { T* data; };

template<>
class Vector<bool> {
    unsigned char* data;  // Bit array
};
```

**Performance optimizations:**
```cpp
template<typename T>
void copy(T* dest, T* src, size_t n) {
    for (size_t i = 0; i < n; ++i)
        dest[i] = src[i];
}

template<>
void copy<char>(char* dest, char* src, size_t n) {
    memcpy(dest, src, n);  // Much faster!
}
```

**Type-specific algorithms:**
```cpp
template<typename T>
bool compare(T a, T b) { return a == b; }

template<>
bool compare<double>(double a, double b) {
    return abs(a - b) < 0.0001;  // Epsilon
}
```

**Type traits:**
```cpp
template<typename T>
struct IsPointer { static const bool value = false; };

template<typename T>
struct IsPointer<T*> { static const bool value = true; };
```

### ❌ Don't Use Specialization When:

**Simple behavior differences** - Use overloading:
```cpp
// ❌ Bad: Specialize
template<typename T>
void print(T x) { cout << x; }

template<>
void print<int>(int x) { cout << "Int: " << x; }

// ✅ Better: Overload
template<typename T>
void print(T x) { cout << x; }

void print(int x) { cout << "Int: " << x; }
```

**Can use if constexpr (C++17):**
```cpp
template<typename T>
void process(T value) {
    if constexpr (is_pointer_v<T>) {
        // Handle pointers
    } else {
        // Handle regular types
    }
}
```

**Can use concepts (C++20):**
```cpp
template<Integral T>
void process(T value) { /* ... */ }

template<FloatingPoint T>
void process(T value) { /* ... */ }
```

---

## ⚠️ Common Mistakes

### ❌ Mistake 1: Partial Specialization of Functions

**Bad:**
```cpp
template<typename T>
void func(T value) { }

// ❌ Error: Functions don't support partial specialization
template<typename T>
void func(T* value) { }
```

**Fix:** Use overloading instead
```cpp
template<typename T>
void func(T value) { }

// ✅ Overload
template<typename T>
void func(T* value) { }
```

### ❌ Mistake 2: Inconsistent Interface

**Bad:**
```cpp
template<typename T>
class Container {
public:
    void add(T item) { }
    T get(int index) { }
};

template<>
class Container<bool> {
public:
    void push(bool item) { }  // ❌ Different name!
    // ❌ Missing get()!
};
```

**Fix:** Keep interface consistent
```cpp
template<>
class Container<bool> {
public:
    void add(bool item) { }  // ✅ Same name
    bool get(int index) { }  // ✅ Same interface
};
```

### ❌ Mistake 3: Wrong Specialization Syntax

**Bad:**
```cpp
template<typename T>
class Box { };

// ❌ Error: Missing template<>
class Box<int> { };
```

**Fix:**
```cpp
template<>  // ✅ Need this!
class Box<int> { };
```

### ❌ Mistake 4: Forgetting to Test Specializations

**Problem:** Specializations may have bugs!

**Fix:** Test all specializations thoroughly
```cpp
// Test generic
Box<int> b1(42);
assert(b1.get() == 42);

// Test specialization
Box<bool> b2(true);
assert(b2.get() == true);
```

---

## 🎯 Practice Exercises

### Exercise 1: String Specialization
Create a container with specialization for const char*.

### Exercise 2: Type Traits
Implement IsArray trait with partial specialization.

### Exercise 3: Optimization
Specialize swap for large arrays.

### Exercise 4: Pointer Handler
Create a smart pointer with pointer specialization.

### Exercise 5: Vector<bool>
Implement basic bit-packed bool vector.

---

## 🚀 How to Run

### Compile:
```bash
g++ -std=c++17 template_specialization.cpp -o template_specialization
```

### Run:
```bash
./template_specialization
```

### Expected Output:
The program demonstrates:
1. Basic function specialization
2. Basic class specialization
3. Partial specialization for pointers
4. Partial specialization for arrays
5. Multiple parameter specializations
6. Type traits patterns
7. Optimization examples
8. Vector<bool> pattern
9. Member function specialization
10. When to use alternatives

---

## 📊 Visual Concepts

### Specialization Selection

```
COMPILER DECISION:

User code: Box<int*> b;
              │
              ▼
┌─────────────────────────────┐
│ Check for specializations   │
└─────────────────────────────┘
              │
       ┌──────┴──────┬─────────┐
       │             │         │
       ▼             ▼         ▼
┌──────────┐  ┌───────────┐ ┌─────────┐
│   Full   │  │  Partial  │ │ Primary │
│ Box<int*>│  │  Box<T*>  │ │ Box<T>  │
└──────────┘  └───────────┘ └─────────┘
              │             │
              │ Most        │ Least
              │ Specific    │ Specific
              │             │
              ▼             ▼
          Use this!    Fallback

Priority: Full > Partial > Primary
```

### Partial vs Full

```
FULL SPECIALIZATION:
template<typename T>
class Box { };
         │
         ▼
template<>      ← Specific type
class Box<int> { };

PARTIAL SPECIALIZATION:
template<typename T>
class Box { };
         │
         ▼
template<typename T>  ← Still generic
class Box<T*> { };    ← Pattern
```

---

## 📚 Related Topics

### Prerequisites:
- **32_FunctionTemplates** - Function templates
- **33_ClassTemplates** - Class templates
- **Type System** - Understanding types

### Coming Next:
- **35_VariadicTemplates** - Variable arguments
- **36_SFINAE** - Template metaprogramming
- **37_Concepts** - C++20 constraints

### Related Concepts:
- **Type Traits** - Using specialization
- **STL** - Uses specialization extensively
- **Optimization** - Type-specific

---

## 🎓 Summary

### What We Learned:

✅ **Specialization** provides custom implementations
✅ **Full specialization** for specific types
✅ **Partial specialization** for patterns (classes only)
✅ **Priority**: Full > Partial > Primary
✅ **Functions**: No partial, use overloading
✅ **Classes**: Both full and partial supported
✅ **Common patterns**: pointers, arrays, type traits
✅ **vector<bool>** is famous example
✅ **Optimization** through specialization
✅ **Alternatives**: overloading, if constexpr, concepts

### The Complete Pattern:

```cpp
// ============================================
// TEMPLATE SPECIALIZATION COMPLETE EXAMPLE
// ============================================

// PRIMARY TEMPLATE
template<typename T>
class Container {
    T* data;
    size_t size;
    
public:
    Container(size_t n) : size(n) {
        data = new T[n];
    }
    
    ~Container() {
        delete[] data;
    }
    
    T& operator[](size_t index) {
        return data[index];
    }
};

// FULL SPECIALIZATION for bool
template<>
class Container<bool> {
    unsigned char* data;  // Bit array
    size_t size;
    
public:
    // Proxy class for bit access
    class BitRef {
        unsigned char& byte;
        unsigned char mask;
    public:
        BitRef(unsigned char& b, unsigned char m) 
            : byte(b), mask(m) {}
        
        operator bool() const {
            return (byte & mask) != 0;
        }
        
        BitRef& operator=(bool value) {
            if (value)
                byte |= mask;
            else
                byte &= ~mask;
            return *this;
        }
    };
    
    Container(size_t n) : size(n) {
        data = new unsigned char[(n + 7) / 8]();
    }
    
    ~Container() {
        delete[] data;
    }
    
    BitRef operator[](size_t index) {
        return BitRef(data[index / 8], 1 << (index % 8));
    }
};

// PARTIAL SPECIALIZATION for pointers
template<typename T>
class Container<T*> {
    T** data;
    size_t size;
    
public:
    Container(size_t n) : size(n) {
        data = new T*[n];
        for (size_t i = 0; i < n; ++i)
            data[i] = nullptr;
    }
    
    ~Container() {
        for (size_t i = 0; i < n; ++i)
            delete data[i];
        delete[] data;
    }
    
    T*& operator[](size_t index) {
        return data[index];
    }
};

// Usage
Container<int> ints(100);      // Primary template
Container<bool> bools(100);    // Full specialization (bit-packed)
Container<int*> ptrs(100);     // Partial specialization (manages pointers)
```

---

## ✨ Quick Reference Card

```cpp
// === FULL SPECIALIZATION (Function) ===
template<typename T>
void func(T x) { }  // Generic

template<>
void func<int>(int x) { }  // Specialized

// === FULL SPECIALIZATION (Class) ===
template<typename T>
class Box { };  // Generic

template<>
class Box<bool> { };  // Specialized

// === PARTIAL SPECIALIZATION (Pointer) ===
template<typename T>
class Box<T*> { };  // For any pointer

// === PARTIAL SPECIALIZATION (Array) ===
template<typename T, size_t N>
class Box<T[N]> { };  // For arrays

// === MULTIPLE PARAMS ===
template<typename T1, typename T2>
class Pair { };  // Primary

template<typename T>
class Pair<T, T> { };  // Both same

template<typename T1, typename T2>
class Pair<T1, T2*> { };  // Second is pointer

// === TYPE TRAITS ===
template<typename T>
struct IsPointer {
    static const bool value = false;
};

template<typename T>
struct IsPointer<T*> {
    static const bool value = true;
};

// === USAGE ===
Box<int> b1;      // Primary
Box<bool> b2;     // Full specialization
Box<int*> b3;     // Partial specialization
```

---

## 🔍 Best Practices

### ✅ DO:

- **Use for different** data structures
- **Document why** specializing
- **Keep interface consistent** across specializations
- **Test all specializations**
- **Consider alternatives** first
- **Use for type traits**
- **Optimize when beneficial**
- **Prefer overloading** for functions
- **Use partial specialization** for patterns
- **Keep in same file**

### ❌ DON'T:

- **Over-specialize** - keep it simple
- **Partial specialize functions** - not supported!
- **Forget to test** all versions
- **Create inconsistent** interfaces
- **Specialize unnecessarily**
- **Ignore alternatives** (if constexpr, concepts)
- **Put in different** namespaces
- **Assume it's faster** - profile!
- **Forget copy/move** semantics
- **Mix specialization** with overloading

---

## 🌟 Real-World Applications

### std::vector<bool>
```cpp
// Space optimization
template<typename T>
class vector { T* data; };

template<>
class vector<bool> {
    unsigned char* data;  // Bit packing
};
```

### Type Traits Library
```cpp
template<typename T>
struct is_pointer : false_type { };

template<typename T>
struct is_pointer<T*> : true_type { };
```

### String Optimizations
```cpp
template<typename T>
class BasicString { };

template<>
class BasicString<char> {
    // SSO, COW, optimizations
};
```

### Smart Pointers
```cpp
template<typename T>
class UniquePtr { };

template<typename T>
class UniquePtr<T[]> {
    // Array delete
};
```

---

## 📖 Further Reading

- [cppreference.com - Template specialization](https://en.cppreference.com/w/cpp/language/template_specialization)
- "C++ Templates: The Complete Guide" - Chapter on Specialization
- "Effective C++" by Scott Meyers - Template items
- [Why Not Specialize Functions](https://www.gotw.ca/publications/mill17.htm)

---

## 🎯 Key Takeaways

1. **Specialization** = custom implementation for types
2. **Full** = specific type (Box<int>)
3. **Partial** = pattern (Box<T*>)
4. **Priority**: Full > Partial > Primary
5. **Functions**: No partial, prefer overloading
6. **Classes**: Support both full and partial
7. **Common patterns**: pointers, arrays, traits
8. **vector<bool>** is classic example
9. **Test all specializations**
10. **Consider alternatives** first

---

## 🚨 THE GOLDEN RULES

```
⚠️  RULE 1: Prefer overloading for functions
   Functions don't support partial!
   
   void func(int x) { }  // Overload, not specialization

⚠️  RULE 2: Keep interface consistent
   Specializations should match!
   
   Same public methods across all versions

⚠️  RULE 3: Most specific wins
   Priority matters!
   
   Full > Partial > Primary

⚠️  RULE 4: Test thoroughly
   Each specialization is different code!
   
   Test generic, full, and partial versions

⚠️  RULE 5: Document the reason
   Why specialize?
   
   // Specialized for space optimization
   // Specialized for performance
   
   USE SPECIALIZATION FOR TYPE-SPECIFIC NEEDS!
```

---

**Previous Topic:** [33_ClassTemplates](../33_ClassTemplates/) - Template Classes

**Next Topic:** [35_VariadicTemplates](../35_VariadicTemplates/) - Variable Arguments

---

*Part of the Templates and Generic Programming series*
*Difficulty: Advanced*
*Prerequisites: Function and class templates*
*CRITICAL for Advanced Template Programming*