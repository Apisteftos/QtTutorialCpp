# SFINAE - Substitution Failure Is Not An Error

## Overview

**SFINAE** (Substitution Failure Is Not An Error) is an advanced C++ template technique that enables compile-time function selection based on type properties. When template parameter substitution fails, the compiler removes that overload from consideration rather than producing an error.

**File:** `SFINAE.cpp`  
**Difficulty:** 🔴 Advanced  
**C++ Version:** C++11 (enable_if), C++17 (if constexpr), C++20 (concepts)  
**Compile:** `g++ -std=c++17 -Wall -Wextra SFINAE.cpp -o sfinae`

---

## Table of Contents

1. [What Is SFINAE?](#what-is-sfinae)
2. [The Problem It Solves](#the-problem-it-solves)
3. [Basic Syntax](#basic-syntax)
4. [Examples Overview](#examples-overview)
5. [Type Traits](#type-traits)
6. [Custom Type Traits](#custom-type-traits)
7. [Common Patterns](#common-patterns)
8. [Modern Alternatives](#modern-alternatives)
9. [Best Practices](#best-practices)
10. [Common Mistakes](#common-mistakes)
11. [Performance](#performance)
12. [When to Use](#when-to-use)

---

## What Is SFINAE?

### The Name

**SFINAE** = **S**ubstitution **F**ailure **I**s **N**ot **A**n **E**rror

### The Concept

When the compiler tries to instantiate a template:

1. **Substitutes** template parameters with actual types
2. If substitution **fails** → Removes that overload (no error)
3. Tries **next candidate**
4. **Errors** only if no candidates work

### Simple Analogy

Think of SFINAE like a restaurant menu:

```
You: "I'd like the fish, please."
Waiter: "Sorry, we're out of fish. Would you like chicken instead?"
    (Failure to provide fish is NOT an error - just try the next option!)
```

In C++:
```cpp
template<typename T>
void func(T value) {
    value.foo();  // Try this first
}

template<typename T>
void func(T value) {
    // Fallback if first doesn't work
}
```

---

## The Problem It Solves

### Problem Without SFINAE

```cpp
// ❌ This template tries to work with ALL types
template<typename T>
void print_size(const T& container) {
    cout << container.size();  // What if T has no size()?
}

print_size(vector<int>{1,2,3});  // ✅ OK - vector has size()
print_size(42);                   // ❌ ERROR! int has no size()
```

**Result:** Compilation error even though we just wanted it to skip that overload!

### Solution With SFINAE

```cpp
// ✅ Only works with types that have size()
template<typename T>
std::enable_if_t<has_size<T>::value, void>
print_size(const T& container) {
    cout << container.size();
}

// ✅ Fallback for types without size()
template<typename T>
std::enable_if_t<!has_size<T>::value, void>
print_size(const T& value) {
    cout << "No size() available";
}

print_size(vector<int>{1,2,3});  // ✅ Calls first version
print_size(42);                   // ✅ Calls second version (no error!)
```

---

## Basic Syntax

### enable_if

```cpp
// C++11/14
template<typename T>
typename std::enable_if<condition, ReturnType>::type
func(T value) { /* ... */ }

// C++14+ (cleaner with _t helper)
template<typename T>
std::enable_if_t<condition, ReturnType>
func(T value) { /* ... */ }
```

### Template Parameter SFINAE

```cpp
template<typename T,
         typename = std::enable_if_t<condition>>
void func(T value) {
    // Only instantiated when condition is true
}
```

### Return Type SFINAE

```cpp
template<typename T>
std::enable_if_t<condition, ReturnType>
func(T value) {
    // Different return types based on condition
}
```

### Multiple Constraints

```cpp
template<typename T>
std::enable_if_t<
    condition1 && 
    condition2 && 
    !condition3,
    void>
func(T value) { /* ... */ }
```

---

## Examples Overview

### Example 1: The Problem

Shows why we need SFINAE - templates that fail for certain types.

### Example 2: Basic enable_if

Simple type checking with standard type traits:
```cpp
template<typename T,
         typename = std::enable_if_t<std::is_integral_v<T>>>
void print_integral(T value) {
    cout << "Integral: " << value << "\n";
}

print_integral(42);    // ✅ Works
print_integral(3.14);  // ❌ Won't compile (not integral)
```

### Example 3: Return Type SFINAE

Different return types based on input type:
```cpp
// Returns int for numbers
template<typename T>
std::enable_if_t<std::is_arithmetic_v<T>, int>
process(T value) { return value * 2; }

// Returns string for strings
template<typename T>
std::enable_if_t<std::is_same_v<T, string>, string>
process(const T& value) { return value + " processed"; }
```

### Example 4: Detecting Member Functions

Check if a type has specific methods:
```cpp
// Detect if T has size() method
template<typename T, typename = void>
struct has_size : std::false_type {};

template<typename T>
struct has_size<T, std::void_t<decltype(std::declval<T>().size())>>
    : std::true_type {};
```

### Example 5: Detecting Operators

Check if a type supports specific operators:
```cpp
// Detect operator<
template<typename T, typename = void>
struct has_less_than : std::false_type {};

template<typename T>
struct has_less_than<T, std::void_t<
    decltype(std::declval<T>() < std::declval<T>())>>
    : std::true_type {};
```

### Example 6: Multiple Constraints

Combine multiple conditions:
```cpp
template<typename T>
std::enable_if_t<
    std::is_arithmetic_v<T> && 
    !std::is_same_v<T, bool>,
    void>
print_number(T value) { /* ... */ }
```

### Example 7: Function Overloading

Multiple overloads for different type categories:
```cpp
// For pointers
template<typename T>
std::enable_if_t<std::is_pointer_v<T>, void>
print_value(T ptr) { /* ... */ }

// For values
template<typename T>
std::enable_if_t<!std::is_pointer_v<T>, void>
print_value(T value) { /* ... */ }
```

### Example 8: Tag Dispatching

Cleaner alternative to SFINAE:
```cpp
template<typename T>
void func(T value) {
    func_impl(value, tag_type<T>{});  // Dispatch to correct version
}
```

### Example 9: if constexpr (C++17)

Modern, simpler alternative:
```cpp
template<typename T>
void print_info(T value) {
    if constexpr (std::is_integral_v<T>) {
        // Code for integral types
    } else if constexpr (std::is_floating_point_v<T>) {
        // Code for floating types
    }
}
```

### Example 10: Generic Serializer

Real-world example - serializer that adapts to type capabilities:
```cpp
// Use custom serialize() if available
template<typename T>
std::enable_if_t<has_serialize_v<T>, void>
serialize(const T& obj) {
    obj.serialize();
}

// Fallback for arithmetic types
template<typename T>
std::enable_if_t<!has_serialize_v<T> && std::is_arithmetic_v<T>, void>
serialize(T value) {
    cout << value;
}
```

---

## Type Traits

### Standard Type Traits (C++11+)

#### Primary Type Categories

```cpp
std::is_void_v<T>              // void
std::is_null_pointer_v<T>      // nullptr_t
std::is_integral_v<T>          // int, char, bool, long, etc.
std::is_floating_point_v<T>    // float, double, long double
std::is_array_v<T>             // T[N] or T[]
std::is_enum_v<T>              // enum or enum class
std::is_union_v<T>             // union
std::is_class_v<T>             // class or struct
std::is_function_v<T>          // function types
std::is_pointer_v<T>           // T*
std::is_reference_v<T>         // T& or T&&
std::is_member_pointer_v<T>    // T::*
```

#### Composite Type Categories

```cpp
std::is_arithmetic_v<T>        // integral or floating
std::is_fundamental_v<T>       // arithmetic or void or nullptr
std::is_object_v<T>            // not function, reference, or void
std::is_scalar_v<T>            // arithmetic, enum, pointer, etc.
std::is_compound_v<T>          // not fundamental
```

#### Type Properties

```cpp
std::is_const_v<T>             // const qualified
std::is_volatile_v<T>          // volatile qualified
std::is_signed_v<T>            // signed arithmetic
std::is_unsigned_v<T>          // unsigned arithmetic
```

#### Type Relationships

```cpp
std::is_same_v<T, U>           // T and U are same type
std::is_base_of_v<Base, Der>   // Base is base of Derived
std::is_convertible_v<From, To> // From convertible to To
```

---

## Custom Type Traits

### Detecting Member Functions

#### Basic Pattern

```cpp
// Default: assume false
template<typename T, typename = void>
struct has_foo : std::false_type {};

// Specialization: true if foo() exists
template<typename T>
struct has_foo<T, std::void_t<decltype(std::declval<T>().foo())>>
    : std::true_type {};

// Helper variable template
template<typename T>
inline constexpr bool has_foo_v = has_foo<T>::value;
```

#### How It Works

```
1. Compiler tries first template (false_type)
2. Then tries specialization with void_t
3. If T has foo():
   → decltype(std::declval<T>().foo()) works
   → void_t makes it void
   → Specialization selected (true_type)
4. If T has no foo():
   → decltype fails
   → SFINAE removes specialization
   → First template selected (false_type)
```

### Detecting Member Types

```cpp
template<typename T, typename = void>
struct has_value_type : std::false_type {};

template<typename T>
struct has_value_type<T, std::void_t<typename T::value_type>>
    : std::true_type {};
```

### Detecting Operators

```cpp
template<typename T, typename = void>
struct has_plus : std::false_type {};

template<typename T>
struct has_plus<T, std::void_t<
    decltype(std::declval<T>() + std::declval<T>())>>
    : std::true_type {};
```

### Detecting with Arguments

```cpp
template<typename T, typename = void>
struct is_printable : std::false_type {};

template<typename T>
struct is_printable<T, std::void_t<
    decltype(std::cout << std::declval<T>())>>
    : std::true_type {};
```

---

## Common Patterns

### Pattern 1: Enable/Disable Overloads

```cpp
// Enable for condition
template<typename T>
std::enable_if_t<condition, void>
func(T value) { /* ... */ }

// Disable for condition (enable for !condition)
template<typename T>
std::enable_if_t<!condition, void>
func(T value) { /* ... */ }
```

### Pattern 2: Different Return Types

```cpp
// Return int for numbers
template<typename T>
std::enable_if_t<std::is_arithmetic_v<T>, int>
process(T value) { return value * 2; }

// Return string for strings
template<typename T>
std::enable_if_t<std::is_same_v<T, string>, string>
process(const string& s) { return s + "!"; }
```

### Pattern 3: Multiple Constraints

```cpp
template<typename T>
std::enable_if_t<
    condition1 &&
    condition2 &&
    !condition3,
    ReturnType>
func(T value) { /* ... */ }
```

### Pattern 4: Tag Dispatching (Alternative)

```cpp
// Dispatch based on type
template<typename T>
void func(T value) {
    func_impl(value, typename type_category<T>::type{});
}

// Implementations
void func_impl(int value, integral_tag) { /* ... */ }
void func_impl(double value, floating_tag) { /* ... */ }
void func_impl(const string& s, string_tag) { /* ... */ }
```

---

## Modern Alternatives

### C++17: if constexpr

Much cleaner than SFINAE for many cases:

```cpp
// ❌ Old SFINAE way - multiple overloads
template<typename T>
std::enable_if_t<std::is_integral_v<T>, void>
print(T value) { cout << "Integral\n"; }

template<typename T>
std::enable_if_t<std::is_floating_point_v<T>, void>
print(T value) { cout << "Floating\n"; }

// ✅ Modern if constexpr - single function
template<typename T>
void print(T value) {
    if constexpr (std::is_integral_v<T>) {
        cout << "Integral\n";
    } else if constexpr (std::is_floating_point_v<T>) {
        cout << "Floating\n";
    }
}
```

**Advantages:**
- Single function (easier to read)
- No overload resolution complexity
- Better error messages
- Easier to maintain

### C++20: Concepts

Even cleaner and more explicit:

```cpp
// Define concept
template<typename T>
concept Integral = std::is_integral_v<T>;

template<typename T>
concept Floating = std::is_floating_point_v<T>;

// Use concepts
void print(Integral auto value) {
    cout << "Integral\n";
}

void print(Floating auto value) {
    cout << "Floating\n";
}

// Or with requires
template<typename T>
requires Integral<T>
void process(T value) { /* ... */ }
```

**Advantages:**
- Self-documenting
- Clear error messages
- No SFINAE tricks needed
- Standard way to constrain templates

### Comparison

| Feature | SFINAE | if constexpr | Concepts |
|---------|--------|--------------|----------|
| C++ Version | C++11+ | C++17+ | C++20+ |
| Readability | ❌ Complex | ✅ Good | ✅✅ Excellent |
| Error Messages | ❌ Cryptic | ✅ Better | ✅✅ Clear |
| Overload Resolution | Manual | Automatic | Clean |
| Learning Curve | Steep | Moderate | Easy |
| Compile Time | Slower | Faster | Fastest |

**Recommendation:** Use C++20 concepts when available!

---

## Best Practices

### ✅ DO

1. **Prefer modern alternatives**

```cpp
// C++20: Use concepts (best)
template<Arithmetic T>
void func(T value) { }

// C++17: Use if constexpr (good)
template<typename T>
void func(T value) {
    if constexpr (std::is_arithmetic_v<T>) { }
}

// C++11/14: Use SFINAE (when necessary)
template<typename T>
std::enable_if_t<std::is_arithmetic_v<T>, void>
func(T value) { }
```

2. **Create helper traits**

```cpp
// ✅ Reusable trait
template<typename T>
inline constexpr bool is_container_v = /* ... */;

// Use it
template<typename T>
std::enable_if_t<is_container_v<T>, void>
process(const T& container) { }
```

3. **Document requirements**

```cpp
/// @brief Process numeric types
/// @tparam T Must be arithmetic type
template<typename T>
std::enable_if_t<std::is_arithmetic_v<T>, void>
process(T value) { }
```

4. **Use mutually exclusive conditions**

```cpp
// ✅ Good - conditions are mutually exclusive
enable_if_t<is_integral_v<T>, void> func(T);
enable_if_t<is_floating_point_v<T>, void> func(T);

// ❌ Bad - both could be true (ambiguous)
enable_if_t<is_arithmetic_v<T>, void> func(T);
enable_if_t<is_integral_v<T>, void> func(T);  // int satisfies both!
```

5. **Keep conditions simple**

```cpp
// ✅ Simple, readable
enable_if_t<is_arithmetic_v<T>, void>

// ❌ Too complex
enable_if_t<
    (is_integral_v<T> || is_floating_point_v<T>) &&
    !is_same_v<T, bool> &&
    !is_const_v<T> &&
    sizeof(T) >= 4,
    void>
```

### ❌ DON'T

1. **Forget typename (C++11/14)**

```cpp
// ❌ Missing typename
enable_if<condition, T>::type func();

// ✅ With typename
typename enable_if<condition, T>::type func();

// ✅✅ Better - use _t helper (C++14+)
enable_if_t<condition, T> func();
```

2. **Create ambiguous overloads**

```cpp
// ❌ Ambiguous if T is integral
template<typename T>
enable_if_t<is_arithmetic_v<T>, void> func(T);

template<typename T>
enable_if_t<is_integral_v<T>, void> func(T);  // Overlap!

// ✅ Mutually exclusive
template<typename T>
enable_if_t<is_integral_v<T>, void> func(T);

template<typename T>
enable_if_t<is_floating_point_v<T>, void> func(T);
```

3. **Overuse SFINAE**

```cpp
// ❌ SFINAE overkill for simple case
template<typename T>
enable_if_t<is_same_v<T, int>, void>
print_int(T value) { cout << value; }

// ✅ Just use overloading
void print_int(int value) { cout << value; }
```

4. **Make it too complex**

```cpp
// ❌ Too complex - hard to understand
template<typename T>
enable_if_t<
    has_begin_v<T> &&
    has_end_v<T> &&
    has_value_type_v<T> &&
    is_same_v<typename T::value_type, int>,
    void>
process(const T& container);

// ✅ Create a concept/trait
template<typename T>
concept IntContainer = /* conditions above */;

void process(IntContainer auto container);
```

---

## Common Mistakes

### Mistake 1: Missing typename

```cpp
// ❌ C++11/14 - won't compile
template<typename T>
enable_if<condition, T>::type func(T value);

// ✅ Add typename
template<typename T>
typename enable_if<condition, T>::type func(T value);

// ✅✅ Or use _t helper (C++14+)
template<typename T>
enable_if_t<condition, T> func(T value);
```

### Mistake 2: Ambiguous Overloads

```cpp
// ❌ Both match for int
template<typename T>
enable_if_t<is_arithmetic_v<T>, void> func(T);  // Matches int

template<typename T>
enable_if_t<is_integral_v<T>, void> func(T);    // Also matches int!

// Result: Ambiguous call for int

// ✅ Use mutually exclusive conditions
template<typename T>
enable_if_t<is_integral_v<T>, void> func(T);

template<typename T>
enable_if_t<is_floating_point_v<T>, void> func(T);
```

### Mistake 3: Wrong std::declval Usage

```cpp
// ❌ Missing ()
decltype(std::declval<T>.foo())  // Won't compile

// ✅ Correct
decltype(std::declval<T>().foo())
```

### Mistake 4: Forgetting _v Suffix (C++17+)

```cpp
// ❌ Old style (verbose)
enable_if_t<is_integral<T>::value, void>

// ✅ Modern style (C++17+)
enable_if_t<is_integral_v<T>, void>
```

### Mistake 5: Using SFINAE for Simple Cases

```cpp
// ❌ SFINAE overkill
template<typename T>
enable_if_t<is_same_v<T, int> || is_same_v<T, long>, void>
func(T value) { }

// ✅ Just use overloading
void func(int value) { }
void func(long value) { }
```

---

## Performance

### Compile-Time

**Zero runtime overhead:**
- All SFINAE resolved during compilation
- No runtime type checks
- Overload selection at compile-time
- Can inline completely

**Compilation cost:**
```
Simple SFINAE:   +5-10% compile time
Complex SFINAE:  +20-50% compile time
Many overloads:  +50-100% compile time

if constexpr:    +2-5% (faster than SFINAE)
Concepts:        +1-3% (fastest)
```

### Binary Size

```cpp
// Each template instantiation creates code
func<int>(42);       // Instance 1
func<double>(3.14);  // Instance 2
func<char>('X');     // Instance 3

// Result: Larger binary size
// But: Compiler can deduplicate
```

### Benchmarks

| Operation | Runtime Check | SFINAE | Difference |
|-----------|---------------|--------|------------|
| Type selection | ~10ns | 0ns | ∞ faster |
| Function call | ~5ns | 0ns (inlined) | No overhead |
| Code size | Smaller | Larger | Trade-off |
| Compile time | Faster | Slower | -20-50% |

**Key insight:** SFINAE has **zero runtime cost** but **higher compile-time cost**.

---

## When to Use

### ✅ Use SFINAE When:

1. **Need compile-time type selection (C++11/14)**
   ```cpp
   template<typename T>
   enable_if_t<is_arithmetic_v<T>, void>
   process(T value) { }
   ```

2. **Detecting type capabilities**
   ```cpp
   template<typename T>
   enable_if_t<has_serialize_v<T>, void>
   serialize(const T& obj) { obj.serialize(); }
   ```

3. **Writing generic library code**
   ```cpp
   // Work with any container type
   template<typename Container>
   enable_if_t<is_container_v<Container>, void>
   process(const Container& c) { }
   ```

4. **Supporting multiple type categories**
   ```cpp
   // Different code paths for different types
   enable_if_t<is_pointer_v<T>, void> func(T);
   enable_if_t<is_reference_v<T>, void> func(T);
   enable_if_t<is_value_v<T>, void> func(T);
   ```

### ❌ Don't Use SFINAE When:

1. **C++17 available - use if constexpr**
   ```cpp
   // ❌ SFINAE (old way)
   template<typename T>
   enable_if_t<is_integral_v<T>, void> func(T);
   
   template<typename T>
   enable_if_t<is_floating_point_v<T>, void> func(T);
   
   // ✅ if constexpr (modern)
   template<typename T>
   void func(T value) {
       if constexpr (is_integral_v<T>) { }
       else if constexpr (is_floating_point_v<T>) { }
   }
   ```

2. **C++20 available - use concepts**
   ```cpp
   // ❌ SFINAE
   template<typename T>
   enable_if_t<is_integral_v<T>, void> func(T);
   
   // ✅ Concepts
   void func(Integral auto value) { }
   ```

3. **Simple overloading works**
   ```cpp
   // ❌ SFINAE overkill
   template<typename T>
   enable_if_t<is_same_v<T, int>, void> func(T);
   
   // ✅ Just overload
   void func(int value) { }
   ```

4. **Runtime polymorphism is better**
   ```cpp
   // ❌ Complex SFINAE for runtime behavior
   // ✅ Use virtual functions or std::variant
   ```

---

## Visual Diagrams

### How SFINAE Works

```
Step 1: Template Instantiation Attempt
   template<typename T>
   enable_if_t<is_integral_v<T>, void> func(T);
   
   Call: func(42)
   
Step 2: Substitution
   T = int
   is_integral_v<int> = true
   enable_if_t<true, void> = void
   Result: void func(int)  ✅ Success!

Step 3: Same with double
   Call: func(3.14)
   T = double
   is_integral_v<double> = false
   enable_if_t<false, void> = [SFINAE! Remove overload]
   
   Compiler: "Try next overload..."
   (No error - substitution failure is not an error!)
```

### Overload Resolution

```
Available overloads:
┌─────────────────────────────────────┐
│ func(T) requires is_integral       │
│ func(T) requires is_floating_point │
│ func(T) requires is_pointer        │
└─────────────────────────────────────┘

Call: func(42)  (int)
     │
     ├──> Try is_integral? ✅ Match! (selected)
     ├──> Try is_floating_point? ❌ SFINAE (removed)
     └──> Try is_pointer? ❌ SFINAE (removed)
     
Result: Calls first overload

Call: func(3.14)  (double)
     │
     ├──> Try is_integral? ❌ SFINAE (removed)
     ├──> Try is_floating_point? ✅ Match! (selected)
     └──> Try is_pointer? ❌ SFINAE (removed)
     
Result: Calls second overload
```

### Type Trait Detection

```
Question: Does type T have foo() method?

template<typename T, typename = void>
struct has_foo : std::false_type {};
              │
              └──> Default: Assume NO

template<typename T>
struct has_foo<T, std::void_t<decltype(std::declval<T>().foo())>>
    : std::true_type {};
           │
           └──> Specialization: YES if foo() exists

For type WITH foo():
   1. Try specialization
   2. decltype(obj.foo()) works ✅
   3. void_t makes it void
   4. Specialization selected → true_type
   
For type WITHOUT foo():
   1. Try specialization
   2. decltype(obj.foo()) FAILS ❌
   3. SFINAE removes specialization
   4. Default selected → false_type
```

---

## Quick Reference

### Basic Template

```cpp
template<typename T,
         typename = std::enable_if_t<condition>>
void func(T value) { }
```

### Return Type SFINAE

```cpp
template<typename T>
std::enable_if_t<condition, ReturnType>
func(T value) { }
```

### Helper Trait

```cpp
template<typename T, typename = void>
struct has_foo : std::false_type {};

template<typename T>
struct has_foo<T, std::void_t<decltype(std::declval<T>().foo())>>
    : std::true_type {};

template<typename T>
inline constexpr bool has_foo_v = has_foo<T>::value;
```

### Common Type Traits

```cpp
is_integral_v<T>         // int, long, char
is_floating_point_v<T>   // float, double
is_arithmetic_v<T>       // integral or floating
is_pointer_v<T>          // T*
is_same_v<T, U>          // T == U
```

---

## Summary

### Key Takeaways

1. **SFINAE** = Substitution Failure Is Not An Error
2. **Failed substitution** removes overload (doesn't error)
3. **enable_if** controls overload availability
4. **Type traits** detect type properties
5. **Custom traits** detect members/operators
6. **Zero runtime overhead** (all compile-time)
7. **C++17: if constexpr** is simpler
8. **C++20: Concepts** are clearest
9. **Essential** for generic library code
10. **Use modern alternatives** when possible

### The Golden Rules

```
⚠️  RULE 1: Prefer modern alternatives
   C++20: Use concepts
   C++17: Use if constexpr
   C++11/14: Use SFINAE

⚠️  RULE 2: Keep conditions simple
   Complex SFINAE = hard to maintain!

⚠️  RULE 3: Use mutually exclusive conditions
   Avoid ambiguous overloads!

⚠️  RULE 4: Create helper traits
   Reuse detection logic!

⚠️  RULE 5: Document requirements
   Comment what types work!
   
   SFINAE = COMPILE-TIME TYPE SELECTION!
```

---

**Previous Topic:** [35_VariadicTemplates](../35_VariadicTemplates/) - Variadic Templates

**Next Topic:** [37_Concepts](../37_Concepts/) - C++20 Concepts (if continuing series)

---

*Part of the Templates and Generic Programming series*  
*Difficulty: Advanced*  
*Prerequisites: Templates, Type Traits*  
*Essential for Advanced Generic Programming*