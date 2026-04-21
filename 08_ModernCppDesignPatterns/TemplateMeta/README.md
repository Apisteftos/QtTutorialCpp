# Template Metaprogramming in C++

## 📚 Introduction

Template metaprogramming (TMP) is a technique where templates are used to perform computations at **compile-time** rather than runtime. It's one of the most powerful features of C++ and is heavily used in high-performance libraries and by companies like think-cell.

## 🎯 What is Template Metaprogramming?

Template metaprogramming allows you to:
- **Compute values at compile-time** - Zero runtime cost
- **Generate code automatically** - DRY (Don't Repeat Yourself)
- **Type manipulation** - Work with types as data
- **Conditional compilation** - Different code based on types
- **Static assertions** - Catch errors at compile-time

## 🔥 Why Use Template Metaprogramming?

### Advantages:
✅ **Zero Runtime Cost** - All computation happens at compile-time
✅ **Type Safety** - Catch errors during compilation
✅ **Code Generation** - Compiler writes repetitive code for you
✅ **Optimization** - Enables compiler optimizations
✅ **Generic Programming** - Write code that works with any type
✅ **Static Polymorphism** - Polymorphism without virtual functions

### Use Cases:
- High-performance libraries (Eigen, Boost)
- Embedded systems (no runtime overhead)
- Type-safe interfaces
- Compile-time validation
- Metafunctions and type traits
- Expression templates

## 📖 Core Concepts

### 1. **Templates as Functions**
Templates can be thought of as functions that operate on types:

```cpp
// Regular function (runtime)
int add(int a, int b) {
    return a + b;
}

// Template "function" (compile-time)
template<int A, int B>
struct Add {
    static constexpr int value = A + B;
};

// Usage
constexpr int result = Add<3, 5>::value;  // Computed at compile-time!
```

### 2. **Type as Data**
Types can be passed around and manipulated:

```cpp
template<typename T>
struct AddPointer {
    using type = T*;
};

// int -> int*
using IntPtr = AddPointer<int>::type;
```

### 3. **Template Specialization**
Different implementations for different types:

```cpp
// General template
template<typename T>
struct IsPointer {
    static constexpr bool value = false;
};

// Specialization for pointers
template<typename T>
struct IsPointer<T*> {
    static constexpr bool value = true;
};
```

### 4. **Recursion**
Templates can be recursive:

```cpp
// Compile-time factorial
template<int N>
struct Factorial {
    static constexpr int value = N * Factorial<N-1>::value;
};

// Base case (specialization)
template<>
struct Factorial<0> {
    static constexpr int value = 1;
};

constexpr int f5 = Factorial<5>::value;  // 120, computed at compile-time!
```

## 📂 Files in This Directory

### 1. `type_list.cpp`
**Difficulty**: 🔴 Advanced

Implementing a compile-time list of types:
- Type list data structure
- Append, prepend operations
- Finding types in lists
- Type list transformations
- Practical applications

**Learn**: How to work with types as data

---

### 2. `compile_time_computation.cpp`
**Difficulty**: 🔴 Advanced

Computing values at compile-time:
- Factorial, Fibonacci
- Prime number checking
- Power calculations
- GCD, LCM
- Array size computation
- Compile-time string hashing

**Learn**: Compile-time computation techniques

---

### 3. `SFINAE_examples.cpp`
**Difficulty**: 🔴 Advanced

**SFINAE**: Substitution Failure Is Not An Error

- Function overload resolution
- Enable/disable templates based on conditions
- Type traits implementation
- Detecting member functions
- Concept-like constraints (pre-C++20)

**Learn**: Advanced template selection techniques

---

### 4. `template_traits.cpp`
**Difficulty**: 🟡 Intermediate

Type traits and type manipulation:
- `std::is_same`, `std::is_pointer`, etc.
- Custom type traits
- Type transformations
- Conditional types
- Practical applications

**Learn**: Using and creating type traits

## 🎓 Learning Path

### Beginner Level:
1. Start with basic template syntax (if not familiar)
2. Understand template specialization
3. Read `compile_time_computation.cpp` (simpler examples first)

### Intermediate Level:
4. Study `template_traits.cpp`
5. Learn `std::enable_if` and basic SFINAE
6. Practice with simple metafunctions

### Advanced Level:
7. Master `SFINAE_examples.cpp`
8. Implement `type_list.cpp` structures
9. Build your own metaprogramming utilities

### Expert Level (think-cell level):
10. Combine TMP with ranges and concepts
11. Build expression template libraries
12. Create domain-specific embedded languages (DSELs)

## 💡 Key Techniques

### constexpr (C++11+)
Modern way to do compile-time computation:

```cpp
constexpr int factorial(int n) {
    return (n <= 1) ? 1 : n * factorial(n - 1);
}

constexpr int f5 = factorial(5);  // Compile-time!
```

### if constexpr (C++17)
Compile-time conditional:

```cpp
template<typename T>
auto process(T value) {
    if constexpr (std::is_integral_v<T>) {
        return value * 2;
    } else {
        return value;
    }
}
```

### Concepts (C++20)
Modern way to constrain templates:

```cpp
template<typename T>
concept Numeric = std::is_arithmetic_v<T>;

template<Numeric T>
T add(T a, T b) {
    return a + b;
}
```

### Variable Templates (C++14)
```cpp
template<typename T>
constexpr bool is_pointer_v = std::is_pointer<T>::value;

// Usage
bool x = is_pointer_v<int*>;  // true
```

## 🔧 Tools and Techniques

### 1. **Type Traits Library**
Standard library provides many type traits:
```cpp
#include <type_traits>

std::is_integral<int>::value          // true
std::is_same<int, int>::value         // true
std::is_pointer<int*>::value          // true
std::remove_const<const int>::type    // int
```

### 2. **Template Aliases (C++11)**
```cpp
template<typename T>
using Ptr = T*;

Ptr<int> x;  // int*
```

### 3. **Parameter Packs (C++11)**
```cpp
template<typename... Args>
void print(Args... args) {
    (std::cout << ... << args);  // C++17 fold expression
}
```

### 4. **decltype and declval**
```cpp
template<typename T>
auto process(T value) -> decltype(value.func()) {
    return value.func();
}
```

## ⚠️ Common Pitfalls

### 1. **Compilation Time**
Heavy template metaprogramming can increase compilation time:
```cpp
// ❌ Too much recursion
template<int N>
struct Heavy {
    static constexpr int value = Heavy<N-1>::value + Heavy<N-2>::value;
};

// ✅ Use constexpr functions instead (C++11+)
constexpr int better(int n) {
    return (n <= 1) ? n : better(n-1) + better(n-2);
}
```

### 2. **Error Messages**
Template errors can be cryptic:
```cpp
// Error: 50 lines of template instantiation stack
template<typename T>
void func(T value) {
    value.nonexistent_method();  // If T doesn't have this method
}
```

**Solution**: Use concepts (C++20) or static_assert for better errors

### 3. **Debugging**
Template code is harder to debug:
- Use `static_assert` to check conditions
- Print types with `type_name<T>()` tricks
- Use compiler explorer (godbolt.org)

### 4. **Overuse**
Not everything needs TMP:
```cpp
// ❌ Overkill
template<int N>
struct IsEven {
    static constexpr bool value = (N % 2) == 0;
};

// ✅ Just use a function
constexpr bool isEven(int n) {
    return (n % 2) == 0;
}
```

## 🏢 Real-World Applications

### think-cell's Use of TMP:
> "We constantly introduce and improve our own libraries that go far beyond what the standard library has to offer."

think-cell uses TMP for:
- Custom range library
- Type-safe interfaces
- Compile-time optimizations
- Zero-cost abstractions
- Generic algorithms

### Other Examples:
- **Eigen**: Linear algebra with expression templates
- **Boost**: Extensive metaprogramming utilities
- **Meta programming libraries**: Brigand, Metal, Kvasir
- **Compile-time parsers**: TMP-based JSON parsers
- **Type-safe units**: Physical units checked at compile-time

## 📊 TMP vs Runtime

| Aspect | Template Metaprogramming | Runtime Code |
|--------|-------------------------|--------------|
| **When** | Compile-time | Runtime |
| **Speed** | Zero runtime cost | Some cost |
| **Flexibility** | Less (fixed at compile) | More (dynamic) |
| **Type Safety** | Maximum | Less |
| **Debugging** | Harder | Easier |
| **Errors** | Compile-time | Runtime |
| **Code Size** | Can increase | Smaller |

## 🎯 When to Use TMP

### ✅ USE when:
- Performance is critical (zero-cost abstractions)
- Type safety is important
- You need compile-time validation
- Creating generic libraries
- Implementing type traits
- Building DSELs

### ❌ DON'T USE when:
- Simple runtime logic suffices
- Compilation time is a concern
- Code readability is priority
- Debugging is frequent
- Team is unfamiliar with TMP

## 📚 Recommended Resources

### Books:
1. **"C++ Templates: The Complete Guide"** by Vandevoorde, Josuttis, Gregor
2. **"Modern C++ Design"** by Andrei Alexandrescu
3. **"C++ Template Metaprogramming"** by Abrahams, Gurtovoy

### Online:
- CppReference - Template documentation
- Compiler Explorer (godbolt.org) - See assembly output
- think-cell blog - Real-world TMP usage

### Conference Talks:
- "C++ Template Metaprogramming" - Various CppCon talks
- "Compile-time programming" - Meeting C++ talks

## 🚀 Practice Projects

Apply TMP to:
1. **Compile-time unit system** (meters, seconds, etc.)
2. **Type-safe state machine**
3. **Compile-time string processing**
4. **Expression template calculator**
5. **Type list utilities library**
6. **Compile-time reflection** (limited)

## 🎓 TMP Mastery Checklist

**Beginner:**
- [ ] Understand template syntax
- [ ] Use template specialization
- [ ] Write simple constexpr functions
- [ ] Use standard type traits

**Intermediate:**
- [ ] Implement custom type traits
- [ ] Use SFINAE basics
- [ ] Work with parameter packs
- [ ] Use if constexpr (C++17)

**Advanced:**
- [ ] Master SFINAE patterns
- [ ] Implement type lists
- [ ] Create template metafunctions
- [ ] Use concepts (C++20)

**Expert (think-cell level):**
- [ ] Build complex metaprogramming libraries
- [ ] Design DSELs with templates
- [ ] Combine TMP with modern C++ features
- [ ] Optimize template compilation times

## 💡 think-cell Philosophy

> "We're not afraid of advanced template metaprogramming or macros when they avoid code duplication or lead to cleaner, more readable code."

think-cell's approach:
- Use TMP when it improves code quality
- Prefer modern alternatives when available
- Balance compile-time vs runtime complexity
- Focus on zero-cost abstractions

## 🔜 Next Steps

After mastering template metaprogramming:
- Move to `ThinkCellPhilosophy/` for real-world patterns
- Study `ZeroCostAbstractions/` for performance analysis
- Combine TMP with ranges and functional programming
- Build production-ready generic libraries

---

**Remember**: Template metaprogramming is powerful but should be used judiciously. Modern C++ (C++20+) provides alternatives like concepts and constexpr that are often clearer and easier to use!

**Ready to dive in?** Start with `compile_time_computation.cpp` for the basics! 🚀