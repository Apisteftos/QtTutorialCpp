# Variadic Templates (C++11/17)

## Overview

**Variadic templates** allow functions and classes to accept any number of template arguments. They're a type-safe alternative to C-style `va_args` and are fundamental to modern C++ generic programming.

**File:** `VariadicTemplates.cpp`  
**Difficulty:** 🟡 Intermediate to 🔴 Advanced  
**C++ Version:** C++11 (Fold expressions C++17)  
**Compile:** `g++ -std=c++17 -Wall -Wextra VariadicTemplates.cpp -o VariadicTemplates`

---

## Table of Contents

1. [What Are Variadic Templates?](#what-are-variadic-templates)
2. [Syntax and Key Concepts](#syntax-and-key-concepts)
3. [Processing Patterns](#processing-patterns)
4. [Examples Overview](#examples-overview)
5. [Common Use Cases](#common-use-cases)
6. [Best Practices](#best-practices)
7. [Common Mistakes](#common-mistakes)
8. [Performance](#performance)
9. [When to Use](#when-to-use)
10. [Related Concepts](#related-concepts)

---

## What Are Variadic Templates?

### The Problem

C-style variadic functions (`printf`, `va_args`) are:
- ❌ **Not type-safe** - can cause undefined behavior
- ❌ **Runtime errors** - problems only appear when running
- ❌ **Manual type handling** - must specify types explicitly
- ❌ **No compile-time checking** - wrong types compile

```cpp
// C-style - UNSAFE!
void print(const char* format, ...) {
    va_list args;
    va_start(args, format);
    int x = va_arg(args, int);        // Must specify type!
    const char* s = va_arg(args, const char*);  // Easy to mess up!
    va_end(args);
}

print("%d %s", 42, "hello");  // Works
print("%s %d", 42, "hello");  // ❌ UNDEFINED BEHAVIOR!
```

### The Solution

Variadic templates provide:
- ✅ **Type-safe** - compile-time type checking
- ✅ **Any number of arguments** - from 0 to unlimited
- ✅ **Mixed types** - different types in single call
- ✅ **Zero overhead** - all resolved at compile-time

```cpp
// C++ variadic template - SAFE!
template<typename... Args>
void print(Args... args) {
    (cout << ... << args);  // Fold expression
}

print(42, "hello", 3.14, 'X');  // ✅ Type-safe!
// Different types, all checked at compile-time
```

---

## Syntax and Key Concepts

### Basic Syntax

```cpp
template<typename... Args>      // 1. Parameter pack declaration
void func(Args... args) {       // 2. Function parameters
    sizeof...(Args);            // 3. Count types
    sizeof...(args);            // 4. Count arguments
    (args + ...);               // 5. Fold expression (C++17)
    func2(args...);             // 6. Pack expansion
}
```

### Key Components

#### 1. Type Parameter Pack

```cpp
template<typename... Args>  // Args = parameter pack
                           // ... = ellipsis (pack declarator)
                           // Can have 0 or more types
```

#### 2. Function Parameter Pack

```cpp
void func(Args... args)  // args = function parameters
                        // Expands to: T1 arg1, T2 arg2, T3 arg3, ...
```

#### 3. sizeof... Operator

```cpp
sizeof...(Args)  // Number of types (compile-time constant)
sizeof...(args)  // Number of arguments

// Example:
template<typename... Args>
void count(Args... args) {
    cout << sizeof...(Args) << "\n";  // Count at compile-time
}

count(1, 2, 3, 4, 5);  // Prints: 5
```

#### 4. Pack Expansion

```cpp
func(args...)           // Expands to: func(arg1, arg2, arg3, ...)
func(f(args)...)        // Expands to: func(f(arg1), f(arg2), f(arg3), ...)
func(args + 1...)       // Expands to: func(arg1+1, arg2+1, arg3+1, ...)
```

#### 5. Fold Expressions (C++17)

```cpp
(args + ...)           // Unary right fold: (arg1 + (arg2 + (arg3 + ...)))
(... + args)           // Unary left fold:  (((... + arg1) + arg2) + arg3)
(args + ... + 0)       // Binary right fold: (arg1 + (arg2 + (arg3 + 0)))
(0 + ... + args)       // Binary left fold:  (((0 + arg1) + arg2) + arg3)
```

---

## Processing Patterns

### Pattern 1: Recursion (Classic C++11)

```cpp
// Base case: no arguments
void print() {
    cout << "\n";
}

// Recursive case: peel off first argument
template<typename T, typename... Args>
void print(T first, Args... rest) {
    cout << first << " ";
    print(rest...);  // Recursive call with remaining args
}

print(1, 2, 3, 4, 5);  // Output: 1 2 3 4 5
```

**How it works:**
```
print(1, 2, 3)
→ cout << 1; print(2, 3)
  → cout << 2; print(3)
    → cout << 3; print()  // Base case
```

### Pattern 2: Fold Expressions (Modern C++17)

```cpp
// Much simpler with fold expressions!
template<typename... Args>
void print(Args... args) {
    (cout << ... << args) << "\n";
}

// Sum example
template<typename... Args>
auto sum(Args... args) {
    return (args + ...);
}

cout << sum(1, 2, 3, 4, 5);  // 15
```

**Fold expression types:**
```cpp
(args op ...)           // Unary right: a op (b op (c op d))
(... op args)           // Unary left:  ((a op b) op c) op d
(args op ... op init)   // Binary right: a op (b op (c op init))
(init op ... op args)   // Binary left:  ((init op a) op b) op c
```

### Pattern 3: Comma Trick (Legacy)

```cpp
template<typename... Args>
void print(Args... args) {
    int dummy[] = {(cout << args << " ", 0)...};
    (void)dummy;  // Suppress warning
}
```

### Pattern 4: Initializer List

```cpp
template<typename... Args>
void process(Args... args) {
    {func(args)...};  // Execute for all arguments
}
```

---

## Examples Overview

### Example 1: The Problem (Unsafe va_args)

Shows the dangers of C-style variadic functions:
- Not type-safe
- Manual type specification required
- Undefined behavior on mistakes

### Example 2: Basic Variadic Template

Simple variadic print function using recursion:
```cpp
print(1, 2, 3, 4, 5);
print("Hello", "World");
print(42, "answer", 3.14, 'X');  // Mixed types!
```

### Example 3: sizeof... Operator

Count arguments at compile-time:
```cpp
template<typename... Args>
size_t count_args(Args...) {
    return sizeof...(Args);
}

count_args(1, 2, 3, 4, 5);  // Returns: 5
```

### Example 4: Fold Expressions (C++17)

Modern, clean syntax for operations:
```cpp
template<typename... Args>
auto sum(Args... args) {
    return (args + ...);  // Sum all arguments
}

template<typename... Args>
bool all_true(Args... args) {
    return (args && ...);  // Logical AND
}
```

### Example 5: Variadic Class Templates

```cpp
template<typename... Types>
class MyTuple { /* ... */ };

MyTuple<int, double, string> t;  // Tuple with 3 types
```

### Example 6: Perfect Forwarding

Factory functions that preserve value categories:
```cpp
template<typename T, typename... Args>
T* create(Args&&... args) {
    return new T(std::forward<Args>(args)...);
}

auto widget = create<Widget>(10, 3.14, "name");
```

### Example 7: Type-safe printf

Replacement for unsafe C printf:
```cpp
my_printf("Number: %, String: %, Float: %\n", 42, "test", 3.14);
// Type-safe, compile-time checked!
```

### Example 8: Variadic Min/Max

```cpp
cout << minimum(5, 2, 9, 1, 7);  // 1
cout << maximum(5, 2, 9, 1, 7);  // 9
```

### Example 9: Variadic emplace

Construct objects directly in container:
```cpp
container.emplace_back(10, 20, "Point1");  // No temporary!
```

### Example 10: Real-world Logger

```cpp
debug("Starting app...");
info("User: ", username, ", ID: ", id);
warning("Low memory: ", mb, " MB");
error("Connection failed: ", server, ":", port);
```

---

## Common Use Cases

### 1. Generic Print/Logging Functions

```cpp
template<typename... Args>
void log(Args... args) {
    (cout << ... << args) << "\n";
}
```

### 2. Mathematical Operations

```cpp
template<typename... Args>
auto sum(Args... args) {
    return (args + ...);
}

template<typename... Args>
auto product(Args... args) {
    return (args * ...);
}
```

### 3. Factory Functions

```cpp
template<typename T, typename... Args>
unique_ptr<T> make_unique(Args&&... args) {
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
}
```

### 4. Tuple-like Containers

```cpp
template<typename... Types>
class Tuple { /* ... */ };

Tuple<int, string, double> t{42, "hello", 3.14};
```

### 5. Event Handlers

```cpp
template<typename... Args>
void emit(const string& event, Args... args) {
    // Notify all listeners with any arguments
}
```

### 6. Generic Wrappers

```cpp
template<typename Func, typename... Args>
auto measure_time(Func f, Args&&... args) {
    auto start = chrono::high_resolution_clock::now();
    f(std::forward<Args>(args)...);
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::milliseconds>(end - start);
}
```

---

## Best Practices

### ✅ DO

1. **Prefer fold expressions (C++17)** over recursion

```cpp
// ✅ Good - fold expression
template<typename... Args>
auto sum(Args... args) {
    return (args + ...);
}

// ❌ Verbose - recursion
template<typename T>
T sum(T value) { return value; }

template<typename T, typename... Args>
T sum(T first, Args... rest) {
    return first + sum(rest...);
}
```

2. **Use perfect forwarding** when forwarding to other functions

```cpp
template<typename... Args>
void wrapper(Args&&... args) {
    func(std::forward<Args>(args)...);  // ✅ Preserve value categories
}
```

3. **Use sizeof...** to get pack size

```cpp
template<typename... Args>
void func(Args... args) {
    constexpr size_t count = sizeof...(Args);  // ✅ Compile-time count
}
```

4. **Document requirements** for types

```cpp
// All types must be printable
template<typename... Args>
void print(Args... args) {
    (cout << ... << args);
}
```

5. **Test with 0, 1, and many arguments**

```cpp
func();              // 0 arguments
func(42);            // 1 argument
func(1, 2, 3, 4, 5); // Many arguments
```

### ❌ DON'T

1. **Forget base case** in recursion

```cpp
// ❌ Missing base case!
template<typename T, typename... Args>
void print(T first, Args... rest) {
    cout << first << " ";
    print(rest...);  // ❌ Infinite recursion if no base case!
}

// ✅ Add base case
void print() { }  // Base case
```

2. **Use wrong pack expansion syntax**

```cpp
func(args)...   // ❌ Wrong!
func(args...)   // ✅ Correct!
```

3. **Forget std::forward** in perfect forwarding

```cpp
template<typename... Args>
void wrapper(Args&&... args) {
    func(args...);  // ❌ Loses rvalue-ness!
    func(std::forward<Args>(args)...);  // ✅ Correct!
}
```

4. **Use sizeof(Args)** instead of sizeof...(Args)

```cpp
sizeof(Args)     // ❌ Won't compile!
sizeof...(Args)  // ✅ Correct!
```

5. **Make it too complex**

```cpp
// ❌ Too complex
template<typename... Args>
void func(Args... args) {
    // 50 lines of complex template metaprogramming...
}

// ✅ Keep it simple
template<typename... Args>
void func(Args... args) {
    (cout << ... << args);
}
```

---

## Common Mistakes

### Mistake 1: Wrong Pack Expansion

```cpp
// ❌ Wrong - tries to call func multiple times
template<typename... Args>
void wrapper(Args... args) {
    func(args)...;  // ❌ Error!
}

// ✅ Correct - expands arguments in single call
template<typename... Args>
void wrapper(Args... args) {
    func(args...);  // ✅ Correct!
}
```

### Mistake 2: Forgetting Base Case

```cpp
// ❌ No base case - won't compile with 0 arguments
template<typename T, typename... Args>
void print(T first, Args... rest) {
    cout << first;
    print(rest...);
}

// ✅ Add base case
void print() { }  // Handles empty case
```

### Mistake 3: Not Using std::forward

```cpp
// ❌ Loses rvalue-ness
template<typename... Args>
void wrapper(Args&&... args) {
    func(args...);  // ❌ Always lvalue!
}

// ✅ Preserve value category
template<typename... Args>
void wrapper(Args&&... args) {
    func(std::forward<Args>(args)...);  // ✅ Correct!
}
```

### Mistake 4: Mixing Fold with Comma

```cpp
// ❌ Wrong fold expression
template<typename... Args>
void print(Args... args) {
    (cout << args, ...);  // ❌ Comma operator, not what you want!
}

// ✅ Correct fold expression
template<typename... Args>
void print(Args... args) {
    (cout << ... << args);  // ✅ Uses <<
}
```

---

## Performance

### Zero Runtime Overhead

Variadic templates are a **compile-time feature**:

```cpp
template<typename... Args>
auto sum(Args... args) {
    return (args + ...);
}

sum(1, 2, 3, 4, 5);
```

**After compilation:**
```cpp
// Equivalent to:
1 + 2 + 3 + 4 + 5  // Direct calculation, no function call!
```

### Benchmarks

| Operation | va_args | Variadic Template | Speedup |
|-----------|---------|-------------------|---------|
| Type checking | Runtime | Compile-time | N/A |
| Safety | ❌ Unsafe | ✅ Safe | ∞ |
| Overhead | Function call | Inlined | 10-100x |
| Code size | Small | Larger binary | Trade-off |

### Perfect Forwarding Performance

```cpp
// ❌ Bad - creates temporary copies
template<typename... Args>
void wrapper(Args... args) {
    func(args...);
}

// ✅ Good - no copies, perfect forward
template<typename... Args>
void wrapper(Args&&... args) {
    func(std::forward<Args>(args)...);
}
```

**Performance difference:**
- **Copy version:** ~1000ns (copies large objects)
- **Forward version:** ~10ns (moves/references only)
- **Speedup:** 100x faster!

---

## When to Use

### ✅ Use Variadic Templates When:

1. **Need variable number of arguments**
   - Logger: `log("User:", name, "ID:", id)`
   - Print: `print(1, 2, 3, "hello", 3.14)`

2. **Type-safe alternatives to va_args**
   - Replace C-style printf
   - Event handlers with any arguments

3. **Factory functions**
   - `make_unique<T>(args...)`
   - `emplace_back(args...)`

4. **Generic wrappers**
   - Timing functions
   - Logging wrappers
   - Transaction wrappers

5. **Tuple-like containers**
   - `tuple<int, string, double>`
   - Custom variadic containers

### ❌ Don't Use When:

1. **Fixed number of arguments**
   ```cpp
   // ❌ Overkill for 2 arguments
   template<typename... Args>
   void swap(Args... args) { /* ... */ }
   
   // ✅ Just use two parameters
   template<typename T>
   void swap(T& a, T& b) { /* ... */ }
   ```

2. **All same type - use container instead**
   ```cpp
   // ❌ Variadic for same types
   template<typename... Args>
   void process(Args... args) { /* ... */ }
   
   // ✅ Use vector
   void process(const vector<int>& values) { /* ... */ }
   ```

3. **Runtime collection of arguments**
   ```cpp
   // ❌ Can't build pack at runtime
   vector<int> values = getUserInput();
   // Can't call: func(values...)  // Won't work!
   
   // ✅ Use runtime approach
   for (auto v : values) func(v);
   ```

---

## Related Concepts

### Parameter Packs

```cpp
template<typename... Types>    // Type parameter pack
template<int... Values>        // Non-type parameter pack
template<template<typename> class... Templates>  // Template parameter pack
```

### Pack Expansion

```cpp
func(args...)              // Expand in function call
{args...}                  // Expand in initializer list
sizeof...(args)            // Count elements
return_type<Args...>       // Expand in template arguments
```

### Fold Expressions (C++17)

```cpp
(args op ...)              // Right fold
(... op args)              // Left fold  
(args op ... op init)      // Right fold with init
(init op ... op args)      // Left fold with init
```

**Supported operators:**
```
+  -  *  /  %  ^  &  |  <<  >>  +=  -=  *=  /=  %=
^=  &=  |=  <<=  >>=  ==  !=  <  >  <=  >=  &&  ||  ,  .*  ->*
```

### Perfect Forwarding

```cpp
template<typename... Args>
void wrapper(Args&&... args) {
    func(std::forward<Args>(args)...);
}
```

### std::tuple

```cpp
template<typename... Types>
class tuple { /* variadic base */ };

tuple<int, string, double> t{42, "hello", 3.14};
```

### Concepts (C++20)

```cpp
template<typename... Args>
requires (std::integral<Args> && ...)
auto sum(Args... args) {
    return (args + ...);
}
```

---

## Visual Diagrams

### How Pack Expansion Works

```
Template:  template<typename... Args>
           void func(Args... args)

Call:      func(1, "hello", 3.14)

Step 1:    Args = {int, const char*, double}
           args = {1, "hello", 3.14}

Step 2:    func(Args... args) expands to:
           func(int arg1, const char* arg2, double arg3)

Step 3:    Compiler generates actual function:
           void func_int_cstr_double(int arg1, const char* arg2, double arg3)
```

### Fold Expression Evaluation

```
Expression: (args + ...)
Call:       sum(1, 2, 3, 4)

Unary Right Fold:
    (1 + (2 + (3 + 4)))
     │   │   │   │
     │   │   │   └─ 4
     │   │   └───── 7
     │   └───────── 9
     └───────────── 10

Unary Left Fold: (... + args)
    (((1 + 2) + 3) + 4)
       │   │   │   │
       └─ 3│   │   │
           └─ 6│   │
               └─ 10│
                   └─ 10
```

### Recursion Pattern

```
print(1, 2, 3)
    │
    ├─> cout << 1
    └─> print(2, 3)
            │
            ├─> cout << 2
            └─> print(3)
                    │
                    ├─> cout << 3
                    └─> print()  // Base case
```

---

## Quick Reference

### Basic Template

```cpp
template<typename... Args>
void func(Args... args) {
    // Work with args...
}
```

### Count Arguments

```cpp
sizeof...(Args)   // Number of types
sizeof...(args)   // Number of arguments
```

### Recursion Pattern

```cpp
void func() { }  // Base case

template<typename T, typename... Args>
void func(T first, Args... rest) {
    // Process first
    func(rest...);  // Recurse
}
```

### Fold Expression (C++17)

```cpp
(args op ...)     // Right fold
(... op args)     // Left fold
```

### Perfect Forwarding

```cpp
template<typename... Args>
void wrapper(Args&&... args) {
    func(std::forward<Args>(args)...);
}
```

### Pack Expansion

```cpp
func(args...)              // Function call
{args...}                  // Initializer list
Type<Args...>              // Template argument
func(f(args)...)           // Transform each
```

---

## Compilation Tips

### Compile with C++17

```bash
g++ -std=c++17 -Wall -Wextra VariadicTemplates.cpp -o variadic
./variadic
```

### Enable Optimizations

```bash
g++ -std=c++17 -O3 -Wall -Wextra VariadicTemplates.cpp -o variadic
```

### Check Assembly

```bash
g++ -std=c++17 -O3 -S VariadicTemplates.cpp
# Check generated assembly for inlining
```

---

## Common Errors

### Error 1: No Matching Function

```
error: no matching function for call to 'print()'

Solution: Add base case:
void print() { }
```

### Error 2: Pack Expansion

```
error: expected primary-expression before '...'

Solution: Correct expansion:
func(args)...  → func(args...)
```

### Error 3: sizeof Without ...

```
error: 'sizeof' applied to non-array type 'Args'

Solution: Use sizeof...:
sizeof(Args) → sizeof...(Args)
```

---

## Summary

### Key Takeaways

1. **Variadic templates** = type-safe variable arguments
2. **Zero runtime overhead** - everything at compile-time
3. **Fold expressions** (C++17) simplify code dramatically
4. **Perfect forwarding** preserves value categories
5. **sizeof...** counts arguments at compile-time
6. **Works with any types** - mixed or homogeneous
7. **Foundation of STL** - tuple, apply, make_unique
8. **Replaces unsafe va_args** - compile-time safety
9. **Essential for modern C++** - generic programming
10. **Combine with SFINAE/concepts** for constraints

### The Golden Rules

```
⚠️  RULE 1: Always have base case in recursion
   Infinite recursion without it!

⚠️  RULE 2: Use fold expressions (C++17)
   Much simpler than recursion!

⚠️  RULE 3: Perfect forward when forwarding
   std::forward preserves value categories!

⚠️  RULE 4: Use sizeof...(Args) not sizeof(Args)
   Count elements, not size!

⚠️  RULE 5: Test with 0, 1, many arguments
   Edge cases matter!
   
   VARIADIC TEMPLATES = TYPE-SAFE FLEXIBILITY!
```

---

**Previous Topic:** [32_FunctionTemplates](../32_FunctionTemplates/) - Function Templates

**Next Topic:** [34_TemplateSpecialization](../34_TemplateSpecialization/) - Template Specialization

---

*Part of the Templates and Generic Programming series*  
*Difficulty: Intermediate to Advanced*  
*Prerequisites: Function Templates, Template Basics*  
*Essential for Modern C++ Generic Programming*