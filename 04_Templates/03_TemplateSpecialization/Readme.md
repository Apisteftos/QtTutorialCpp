# TemplateSpecialization - Custom Behavior for Specific Types

## 📚 Overview

This folder introduces **Template Specialization** - the ability to provide custom implementations of templates for specific types. Specialization allows you to optimize, customize, or completely change template behavior for particular types while maintaining a generic implementation for everything else. This is essential for type-specific optimizations and is used extensively in the STL.

**Template Specialization** = Custom implementation for specific types

This is **ESSENTIAL** for advanced template programming and STL mastery.

---

## 🎯 What You'll Learn

- What template specialization is and why it exists
- Full (explicit) specialization
- Partial specialization (class and variable templates, not functions)
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
    return std::abs(a - b) < epsilon;  // Epsilon comparison
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
        data = new char[std::strlen(d) + 1];
        std::strcpy(data, d);
    }
    ~Container() { delete[] data; }

    // Owns raw memory: copying would double-delete, so disable it
    Container(const Container&) = delete;
    Container& operator=(const Container&) = delete;
};
```

### Partial Specialization (Classes, Not Functions)

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

**Note:** Functions do NOT support partial specialization! Use overloading instead (see *Common Mistakes*).

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

### 2. Partial Specialization (Classes and Variables)

Specialize for patterns, not specific types. This works for class templates and
variable templates, but **not** for function templates and not for alias templates:

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

When several specializations match, the compiler picks the **most specialized** one:

```cpp
template<typename T1, typename T2>
class Pair { };                    // 1. Primary (least specific)

template<typename T>
class Pair<T, T> { };              // 2. Partial: both types the same

template<typename T2>
class Pair<int, T2> { };           // 3. Partial: first type is int

template<>
class Pair<int, int> { };          // 4. Full (exact match)

// Selection
Pair<double, std::string> p1;  // Primary
Pair<double, double> p2;       // Partial <T, T>
Pair<int, double> p3;          // Partial <int, T2>
Pair<int, int> p4;             // Full (exact match)
```

**Priority Order:**
1. Full specialization (exact match)
2. The most specialized matching partial specialization
3. Primary template

**Ambiguity:** `<T, T>` and `<int, T2>` are *not* ordered against each other.
`Pair<int, int>` matches both, so without the full specialization
`Pair<int, int>` the compiler reports
`ambiguous template instantiation for 'struct Pair<int, int>'`.
The same happens for `Pair<int, int*>` with `<int, T2>` and `<T1, T2*>`.
Resolve it with a more specific specialization, as the full one above does.

### 4. Pointer Specializations

Very common pattern:

```cpp
// Primary
template<typename T>
class SmartPtr {
public:
    void info() { std::println("Regular type"); }
};

// Partial for pointers
template<typename T>
class SmartPtr<T*> {
public:
    void info() { std::println("Pointer type"); }
    T& operator*();    // dereference (declaration only in this sketch)
    T* operator->();   // arrow
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
    static constexpr bool value = false;
};

// Partial: is a pointer
template<typename T>
struct IsPointer<T*> {
    static constexpr bool value = true;
};

// Usage
IsPointer<int>::value;   // false
IsPointer<int*>::value;  // true
IsPointer<int**>::value; // true (int** matches T*)
```

Use `static constexpr bool` (implicitly inline since C++17). A plain
`static const bool value = false;` works in a `constant expression`, but passing
it to something that takes a reference (such as `std::println`) odr-uses it
and gives a linker error (`undefined reference to 'IsPointer<int>::value'`).

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
void copy(T* dest, T* src, std::size_t n) {
    for (std::size_t i = 0; i < n; ++i)
        dest[i] = src[i];
}

template<>
void copy<char>(char* dest, char* src, std::size_t n) {
    std::memcpy(dest, src, n);  // Much faster!
}
```

**Type-specific algorithms:**
```cpp
template<typename T>
bool compare(T a, T b) { return a == b; }

template<>
bool compare<double>(double a, double b) {
    return std::abs(a - b) < 0.0001;  // Epsilon
}
```

**Type traits:**
```cpp
template<typename T>
struct IsPointer { static constexpr bool value = false; };

template<typename T>
struct IsPointer<T*> { static constexpr bool value = true; };
```

### ❌ Don't Use Specialization When:

**Simple behavior differences** - Use overloading:
```cpp
// ❌ Bad: Specialize
template<typename T>
void show(T x) { std::print("{}", x); }

template<>
void show<int>(int x) { std::print("Int: {}", x); }

// ✅ Better: Overload
template<typename T>
void show(T x) { std::print("{}", x); }

void show(int x) { std::print("Int: {}", x); }
```

**Why overloading is safer:** explicit specializations of a function template do
**not** take part in overload resolution. Only the primary templates (and plain
functions) compete; a specialization is used afterwards, only if its primary
template wins. This surprises people:

```cpp
template<typename T> void f(T)  { std::println("1: primary f(T)"); }
template<>           void f<int*>(int*) { std::println("2: specialization of #1"); }
template<typename T> void f(T*) { std::println("3: overload f(T*)"); }

int* p = nullptr;
f(p);   // prints "3: overload f(T*)", the specialization (2) is never chosen
```

A plain overload is a real candidate, so it behaves the way it looks.

```cpp
template<typename T> void process(T)  { std::println("template"); }
void process(int)    { std::println("overload int"); }
void process(double) { std::println("overload double"); }

process(1);      // overload int
process(2.5);    // overload double
process("x");    // template  (const char* -> T)
process('c');    // template  (char is an exact match for T, no conversion)
process<int>(1); // template  (explicit <int> forces the template)
```

**Can use if constexpr (C++17):**
```cpp
template<typename T>
void process(T value) {
    if constexpr (std::is_pointer_v<T>) {
        // Handle pointers
    } else {
        // Handle regular types
    }
}
```

**Can use concepts (C++20):**
```cpp
template<std::integral T>
void process(T value) { /* ... */ }

template<std::floating_point T>
void process(T value) { /* ... */ }
```

---

## ⚠️ Common Mistakes

### ❌ Mistake 1: Partial Specialization of Functions

**Bad:**
```cpp
template<typename T>
void func(T value) { }

// ❌ Error: partial specialization syntax on a function
template<typename T>
void func<T*>(T* value) { }
```

GCC: `non-class, non-variable partial specialization 'func<T*>' is not allowed`.

**Fix:** Use overloading instead (leave out the `<T*>` after the name)
```cpp
template<typename T>
void func(T value) { }

// ✅ A new overload, not a specialization
template<typename T>
void func(T* value) { }
```

### ❌ Mistake 2: Inconsistent Interface

This compiles, but generic code that uses `add()` / `get()` breaks as soon as it
is instantiated with `bool`.

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

GCC: `an explicit specialization must be preceded by 'template <>'`.

### ❌ Mistake 4: Forgetting to Test Specializations

**Problem:** Specializations may have bugs!

**Fix:** Test all specializations thoroughly
```cpp
#include <cassert>

// Test generic
Box<int> b1(42);
assert(b1.get() == 42);

// Test specialization
Box<bool> b2(true);
assert(b2.get() == true);
```

### ❌ Mistake 5: Function Specialization in a Header Without `inline`

An explicit (full) specialization of a function template is an ordinary function,
not a template. Defined in a header that is included by two `.cpp` files it is
defined twice:

```cpp
// utils.h
template<typename T> bool isEqual(T a, T b) { return a == b; }

template<>
bool isEqual<double>(double a, double b) { ... }   // ❌ multiple definition at link time
```

GCC/ld: `multiple definition of 'bool isEqual<double>(double, double)'`.

**Fix:**
```cpp
template<>
inline bool isEqual<double>(double a, double b) { ... }   // ✅
```

(Class template specializations do not have this problem; only their
non-inline member function definitions outside the class do.)

### ❌ Mistake 6: Specializing From the Wrong Namespace

```cpp
namespace lib   { template<typename T> struct S { }; }
namespace other { template<> struct lib::S<int> { }; }   // ❌ error
```

GCC: `declaration of 'struct lib::S<int>' in namespace 'other' which does not enclose 'lib'`.
A specialization must be declared in the namespace of the template or in an
enclosing namespace (that is why `std::hash<MyType>` is specialized by
reopening `namespace std`).

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
g++ -std=c++23 main.cpp -o template_specialization
```

### Run:
```bash
./template_specialization
```

### Expected Output:
`std::println` needs C++23 and a standard library that ships `<print>` (GCC 14+, recent Clang/MSVC).

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
    std::size_t size;
    
public:
    Container(std::size_t n) : size(n) {
        data = new T[n];
    }
    
    ~Container() {
        delete[] data;
    }
    
    T& operator[](std::size_t index) {
        return data[index];
    }
};

// FULL SPECIALIZATION for bool
template<>
class Container<bool> {
    unsigned char* data;  // Bit array
    std::size_t size;
    
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
    
    Container(std::size_t n) : size(n) {
        data = new unsigned char[(n + 7) / 8]();
    }
    
    ~Container() {
        delete[] data;
    }
    
    BitRef operator[](std::size_t index) {
        return BitRef(data[index / 8], 1 << (index % 8));
    }
};

// PARTIAL SPECIALIZATION for pointers
template<typename T>
class Container<T*> {
    T** data;
    std::size_t size;
    
public:
    Container(std::size_t n) : size(n) {
        data = new T*[n];
        for (std::size_t i = 0; i < n; ++i)
            data[i] = nullptr;
    }
    
    ~Container() {
        for (std::size_t i = 0; i < size; ++i)
            delete data[i];   // owns the pointed-to objects
        delete[] data;
    }
    
    T*& operator[](std::size_t index) {
        return data[index];
    }
};

// Usage
Container<int> ints(100);      // Primary template
Container<bool> bools(100);    // Full specialization (bit-packed)
Container<int*> ptrs(100);     // Partial specialization (manages pointers)
```

These classes own raw memory, so a real version also needs a copy constructor and
copy assignment (or `= delete` them), see the Rule of 3/5/0. They are omitted to keep the pattern short.

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
template<typename T, std::size_t N>
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
    static constexpr bool value = false;
};

template<typename T>
struct IsPointer<T*> {
    static constexpr bool value = true;
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
// Space optimization (simplified sketch of what the library does)
namespace std {
    template<typename T>
    class vector { T* data; };

    template<>
    class vector<bool> {
        unsigned char* data;  // Bit packing
    };
}
```

### Type Traits Library
```cpp
template<typename T>
struct is_pointer : std::false_type { };

template<typename T>
struct is_pointer<T*> : std::true_type { };
```

### Character Traits
```cpp
template<typename CharT>
struct char_traits { /* generic, element-by-element loops */ };

template<>
struct char_traits<char> {
    // libstdc++: uses std::memcpy / memcmp / std::strlen for char
};
```
(`std::basic_string<char>` itself is not specialized; the specialization lives in `std::char_traits<char>`.)

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

---

## Notes moved from main.cpp

The title/description comment at the top of `main.cpp` ("WHAT IS TEMPLATE SPECIALIZATION?") stays. What changed:

- `std::cout` → `std::println` / `std::print`; `#include <print>` added, `<iostream>`, `<vector>` and `<type_traits>` removed (unused), `<cmath>` / `<cstddef>` added. `using namespace std;` is gone and every standard name is written with `std::`.
- Each example has a `// === EXAMPLE n: … ===` title comment.
- **Printed lecture text removed from the program** (it is covered by the sections above):
  - the "💡 …" bullet lists at the end of Examples 1–9 → *Concept Explanation*, *Key Concepts*;
  - the whole **COMPREHENSIVE EXPLANATION** block printed by `main()` (What is template specialization, Full / Partial specialization, Priority, Common patterns, When to specialize, Alternatives, Best practices, Quick reference, Key takeaways, Golden rules) → *Basic Syntax*, *Key Concepts*, *When to Use Specialization*, *Best Practices*, *Quick Reference Card*, *Key Takeaways*, *THE GOLDEN RULES*.
- **Old Example 10 ("Alternatives") was removed from `main.cpp`**: it only printed lecture text and its `process()` overloads were never called. It now lives in *When to Use Specialization* (overloading vs. specialization, `if constexpr`, concepts), with the output of the `process()` calls checked.
- Output differences from the old program (all intended):
  - `std::println` prints `bool` as `true` / `false` instead of `1` / `0`.
  - The result is computed before it is printed (`bool r = isEqual(...)`), so the "Generic comparison" trace comes **before** the `5 == 5: true` line. With `cout << "5 == 5: " << isEqual(5, 5)` the text was printed first and then interleaved with the function's own output.
  - `IsPointer::value` is now `static constexpr bool` (see corrections).
  - `boolVec[i]` is converted with `static_cast<bool>`, because `std::println` cannot format the `BitReference` proxy directly.
- `Storage<const char*>` now has a deleted copy assignment (it already had a copy constructor, so assigning would have shallow-copied the pointer and double-freed). The misleading "checks overflow" in `Calculator<int>::multiply` was removed; the code never checked anything.

### Corrections to the original files

- **Mistake 1 was wrong.** `template<typename T> void func(T* value) { }` is a perfectly valid *overload*, not an error. The real error is the partial-specialization syntax `void func<T*>(T* value)`: `non-class, non-variable partial specialization … is not allowed`. Rewritten.
- **Priority example:** `Pair<int, T>` was described as "more specific" than `Pair<T, T>`. They are not ordered against each other; without the full `Pair<int, int>` specialization, `Pair<int, int>` is **ambiguous**. Fixed and explained.
- **"Complete Pattern" did not compile**: the pointer specialization's destructor used `n`, which does not exist there (`size` is the member). Fixed, plus a note about copy operations.
- **`String Optimizations` example was wrong**: `std::basic_string<char>` is not specialized (SSO is an implementation detail of the primary template). The real specialization is `std::char_traits<char>`. Replaced.
- **`static const bool value`** in the type traits: fine in constant expressions, but passing it to `std::println` (takes a reference) gives a linker error `undefined reference to 'IsPointer<int>::value'`. Changed to `static constexpr bool` everywhere.
- **Partial specialization "classes only"**: variable templates support it too (`template<typename T> constexpr bool is_ptr<T*> = true;`); alias templates and function templates do not.
- **Concepts snippet** used non-existent `Integral` / `FloatingPoint`; now `std::integral` / `std::floating_point`.
- **Compile command:** `-std=c++17 template_specialization.cpp` → `-std=c++23 main.cpp`.
- **Added:** why function specializations are ignored by overload resolution, ambiguity between partial specializations, `inline` for function specializations in headers (Mistake 5), specializing from the wrong namespace (Mistake 6).
- Raw `→` arrows inside code blocks were not valid C++; they are comments now. `print` was renamed `show` in the overload example so it cannot collide with `std::print`.

### What was tested (GCC 13.3, `-Wall -Wextra`, ASan/UBSan)

- `main.cpp` builds without warnings and runs; the output matches the old program except for the intended differences above.
- Overload `func(T*)` compiles; `void func<T*>(T*)` is rejected with the quoted message.
- `class Box<int> { };` without `template<>` → `an explicit specialization must be preceded by 'template <>'`.
- `Pair<int,int>` with `<T,T>` + `<int,T2>` and no full specialization → `ambiguous template instantiation`; same for `Pair<int,int*>` with `<int,T2>` + `<T1,T2*>`.
- `static const bool` + `std::println` → linker error; `static constexpr bool` → fine.
- Specializing `lib::S<int>` from `namespace other` → error `…in namespace 'other' which does not enclose 'lib'`; from an enclosing namespace → accepted.
- Function overload vs. specialization example: `f(p)` calls the overload (`3`), never the specialization. `process(1)` → overload int, `process(2.5)` → overload double, `process("x")` / `process('c')` / `process<int>(1)` → template.
- A function-template specialization defined in a header included by two `.cpp` files → `multiple definition`; adding `inline` links.
- Variable-template partial specialization compiles; alias-template partial specialization is rejected.
- `std::integral` / `std::floating_point` constrained overloads, `if constexpr (std::is_pointer_v<T>)`, tag dispatch with `std::true_type` / `std::false_type` compile and select the expected branch.
- The "Complete Pattern" `Container` (primary, `bool` bit-packed, `T*`) compiles and runs clean under ASan/UBSan after the fix.
- `std::char_traits<char>` is a full specialization in libstdc++ (`struct char_traits<char>`).

Not tested: Clang / MSVC behavior, SSO / COW details of `std::string`, and the performance claims ("much faster" `memcpy`, 8x memory savings beyond the byte counts printed by the program).

