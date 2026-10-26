# Variadic Templates (C++11/17)

## Overview

**Variadic templates** allow functions and classes to accept any number of template arguments. They're a type-safe alternative to C-style `va_args` and are fundamental to modern C++ generic programming.

**File:** `main.cpp`  
**Difficulty:** 🟡 Intermediate to 🔴 Advanced  
**C++ Version:** C++11 (Fold expressions C++17)  
**Compile:** `g++ -std=c++23 -Wall -Wextra main.cpp -o variadic` (`std::println` needs C++23 and GCC 14+ / recent Clang / MSVC; the templates themselves need only C++11, fold expressions C++17)

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
    (std::cout << ... << args);  // Fold expression
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
    std::println("{}", sizeof...(Args));  // Count at compile-time
}

count(1, 2, 3, 4, 5);  // Prints: 5
```

#### 4. Pack Expansion

```cpp
func(args...)           // Expands to: func(arg1, arg2, arg3, ...)
func(f(args)...)        // Expands to: func(f(arg1), f(arg2), f(arg3), ...)
func((args + 1)...)     // Expands to: func(arg1+1, arg2+1, arg3+1, ...)  (parentheses needed!)
```

#### 5. Fold Expressions (C++17)

```cpp
(args + ...)           // Unary right fold:  arg1 + (arg2 + (arg3 + arg4))
(... + args)           // Unary left fold:   ((arg1 + arg2) + arg3) + arg4
(args + ... + 0)       // Binary right fold: arg1 + (arg2 + (arg3 + (arg4 + 0)))
(0 + ... + args)       // Binary left fold:  (((0 + arg1) + arg2) + arg3) + arg4
```

---

## Processing Patterns

### Pattern 1: Recursion (Classic C++11)

```cpp
// Base case: no arguments
void printArgs() {
    std::println("");
}

// Recursive case: peel off first argument
template<typename T, typename... Args>
void printArgs(T first, Args... rest) {
    std::print("{} ", first);
    printArgs(rest...);  // Recursive call with remaining args
}

printArgs(1, 2, 3, 4, 5);  // Output: 1 2 3 4 5
```

**How it works:**
```
printArgs(1, 2, 3)
→ print 1; printArgs(2, 3)
  → print 2; printArgs(3)
    → print 3; printArgs()  // Base case
```

### Pattern 2: Fold Expressions (Modern C++17)

```cpp
// Much simpler with fold expressions!
// Binary left fold over <<: ((std::cout << a1) << a2) << a3 ...
template<typename... Args>
void printAll(Args... args) {
    (std::cout << ... << args) << "\n";
}

// The same with std::print: a unary right fold over the comma operator
template<typename... Args>
void printAll2(Args... args) {
    (std::print("{}", args), ...);
    std::println("");
}

// Sum example
template<typename... Args>
auto sum(Args... args) {
    return (args + ...);
}

std::println("{}", sum(1, 2, 3, 4, 5));  // 15
```

**Fold expression types:**
```cpp
(args op ...)           // Unary right: a op (b op (c op d))
(... op args)           // Unary left:  ((a op b) op c) op d
(args op ... op init)   // Binary right: a op (b op (c op init))
(init op ... op args)   // Binary left:  ((init op a) op b) op c
```

**Rules for fold expressions:**
- The whole fold must be in parentheses, and the operand next to the `...` must be
  a *cast-expression*: `(std::cout << args, ...)` is an error
  (`binary expression in operand of fold-expression`); write
  `((std::cout << args), ...)` or `(std::cout << ... << args)`.
- A unary fold over an **empty** pack is only allowed for `&&` (true), `||` (false)
  and `,` (`void()`). `sum()` with `(args + ...)` does not compile
  (`fold of empty expansion over operator+`). Use a binary fold with an init value,
  `(args + ... + 0)`, if zero arguments must work.

### Pattern 3: Comma Trick (Legacy)

```cpp
template<typename... Args>
void printAll(Args... args) {
    int dummy[] = {0, (std::print("{} ", args), 0)...};
    (void)dummy;  // Suppress warning
}
```

A braced list is evaluated left to right, which is what makes this trick work.
The leading `0` is needed: with an empty pack `int dummy[] = {}` is a zero-size
array and GCC rejects it (`zero-size array 'dummy'`).

### Pattern 4: Initializer List

```cpp
template<typename... Args>
void process(Args... args) {
    // {func(args)...};   // ❌ not valid as a statement
    (void)std::initializer_list<int>{(func(args), 0)...};   // ✅ Execute for all arguments
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
printArgs(1, 2, 3, 4, 5);
printArgs("Hello", "World");
printArgs(42, "answer", 3.14, 'X');  // Mixed types!
```

### Example 3: sizeof... Operator

Count arguments at compile-time:
```cpp
template<typename... Args>
std::size_t count_args(Args...) {
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
    return (args && ...);  // Logical AND (true for an empty pack)
}
```

`main.cpp` also shows a binary `<<` fold into a `std::ostringstream`, and the comma fold
`(std::print("{}", args), ...)` for printing.

### Example 5: Variadic Class Templates

```cpp
template<typename... Types>
class MyTuple { /* ... */ };

MyTuple<int, double, std::string> t;  // Tuple with 3 types
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

### Example 7: Custom printf

Replacement for unsafe C printf:
```cpp
my_printf("Number: %, String: %, Float: %\n", 42, "test", 3.14);
```

Each argument keeps its real type (no `%d` / `%s` mix-ups), and an argument that cannot
be formatted does not compile. The **number** of arguments is only checked at
runtime: too few or too many arguments throws `std::runtime_error`.

### Example 8: Variadic Min/Max

```cpp
std::println("{}", minimum(5, 2, 9, 1, 7));  // 1
std::println("{}", maximum(5, 2, 9, 1, 7));  // 9
```

The result type is the type of the **first** argument, so mixing types can truncate:
`minimum(5, 2.5)` returns `2` (an `int`), not `2.5`. Convert explicitly or use
`std::common_type_t<Args...>` when the types differ.

### Example 9: Variadic emplace

Construct objects directly in container:
```cpp
container.emplace_back(10, 20, "Point1");  // Constructed in place, no temporary Point
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
    (std::print("{}", args), ...);
    std::println("");
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
std::unique_ptr<T> makeUnique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
```

### 4. Tuple-like Containers

```cpp
template<typename... Types>
class Tuple { /* ... */ };

Tuple<int, std::string, double> t{42, "hello", 3.14};   // sketch: this Tuple is not implemented here
```

### 5. Event Handlers

```cpp
template<typename... Args>
void emit(const std::string& event, Args... args) {
    // Notify all listeners with any arguments
}
```

### 6. Generic Wrappers

```cpp
template<typename Func, typename... Args>
auto measure_time(Func f, Args&&... args) {
    auto start = std::chrono::high_resolution_clock::now();
    f(std::forward<Args>(args)...);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
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
    constexpr std::size_t count = sizeof...(Args);  // ✅ Compile-time count
}
```

4. **Document requirements** for types

```cpp
// All types must be formattable
template<typename... Args>
void printAll(Args... args) {
    (std::print("{}", args), ...);
}
```

5. **Test with 0, 1, and many arguments**

```cpp
func();              // 0 arguments
func(42);            // 1 argument
func(1, 2, 3, 4, 5); // Many arguments
```

Careful with unary folds: `sum()` over `(args + ...)` does not compile for zero arguments.

### ❌ DON'T

1. **Forget base case** in recursion

```cpp
// ❌ Missing base case!
template<typename T, typename... Args>
void printArgs(T first, Args... rest) {
    std::print("{} ", first);
    printArgs(rest...);  // ❌ Last call is printArgs() - no matching function!
}

// ✅ Add base case (declared BEFORE the template)
void printArgs() { }  // Base case
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
    (std::cout << ... << args);
}
```

---

## Common Mistakes

### Mistake 1: Wrong Pack Expansion

```cpp
// ❌ Wrong - the pack is not expanded inside the call
template<typename... Args>
void wrapper(Args... args) {
    func(args)...;  // ❌ Error: expected ';' before '...' token
}

// ✅ Correct - expands arguments in single call
template<typename... Args>
void wrapper(Args... args) {
    func(args...);  // ✅ Correct!
}
```

### Mistake 2: Forgetting Base Case

```cpp
// ❌ No base case - does not compile at all: the last recursive call
//    is print() with no arguments, and nothing matches
template<typename T, typename... Args>
void print(T first, Args... rest) {
    std::print("{}", first);
    print(rest...);
}

// ✅ Add base case (declared before the template)
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

### Mistake 4: Unparenthesized Operand in a Fold

```cpp
// ❌ Does not compile: operand of a fold must not be a bare binary expression
template<typename... Args>
void print(Args... args) {
    (std::cout << args, ...);  // ❌ error: binary expression in operand of fold-expression
}

// ✅ Parenthesize the operand (comma fold) ...
template<typename... Args>
void print(Args... args) {
    ((std::cout << args), ...);
}

// ✅ ... or use a binary left fold over <<
template<typename... Args>
void print(Args... args) {
    (std::cout << ... << args);
}
```

---

## Performance

### No Runtime Cost for the Mechanism

Variadic templates are a **compile-time feature**: the compiler generates one
function per distinct list of argument types, and there is no loop, no `va_list`,
no type information at runtime.

```cpp
template<typename... Args>
auto sum(Args... args) {
    return (args + ...);
}

sum(1, 2, 3, 4, 5);
```

With optimization (`-O2`) GCC folds `sum(1, 2, 3, 4, 5)` to the constant 15, so there is
no call left in the generated code. Without optimization, the function is still an
ordinary call.

**Trade-off:** every distinct combination of argument types creates another
instantiation, so heavy use increases compile time and binary size.

### Perfect Forwarding Performance

```cpp
// ❌ Copies every argument by value
template<typename... Args>
void wrapper(Args... args) {
    func(args...);
}

// ✅ No copies: references are forwarded
template<typename... Args>
void wrapper(Args&&... args) {
    func(std::forward<Args>(args)...);
}
```

How much the by-value version costs depends entirely on the argument types. As a
rough illustration (GCC 13, `-O2`, one run, not a rigorous benchmark): passing a
1 MiB `std::string` by value cost about 70 µs per call because of the copy, while
the forwarding version was not measurable. For `int` arguments there is no difference.

> The "10-100x faster", "~1000ns vs ~10ns" and benchmark table that were in the
> earlier version of this file had no measurements behind them and were removed.

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
   void process(const std::vector<int>& values) { /* ... */ }
   ```

3. **Runtime collection of arguments**
   ```cpp
   // ❌ Can't build pack at runtime
   std::vector<int> values = getUserInput();
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
^=  &=  |=  <<=  >>=  =  ==  !=  <  >  <=  >=  &&  ||  ,  .*  ->*
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
namespace std {
    template<typename... Types>
    class tuple { /* variadic base */ };   // simplified
}

std::tuple<int, std::string, double> t{42, "hello", 3.14};
```

### Concepts (C++20)

```cpp
template<typename... Args>
requires (std::integral<Args> && ...)
auto sum(Args... args) {
    return (args + ...);
}
```

Since C++20 a pack can also be captured by a lambda: `[...captured = std::move(args)] { ... }`.

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
      └─3  │     │
           └─6   │
                 └─10
```

### Recursion Pattern

```
printArgs(1, 2, 3)
    │
    ├─> print 1
    └─> printArgs(2, 3)
            │
            ├─> print 2
            └─> printArgs(3)
                    │
                    ├─> print 3
                    └─> printArgs()  // Base case
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

### Compile

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o variadic
./variadic
```

### Enable Optimizations

```bash
g++ -std=c++23 -O3 -Wall -Wextra main.cpp -o variadic
```

### Check Assembly

```bash
g++ -std=c++23 -O3 -S main.cpp
# Check generated assembly for inlining
```

---

## Common Errors

### Error 1: No Matching Function

```
error: no matching function for call to 'printArgs()'

Solution: Add base case (before the template):
void printArgs() { }
```

### Error 2: Pack Expansion

```
error: expected ';' before '...' token
error: parameter packs not expanded with '...'

Solution: Correct expansion:
func(args)...  → func(args...)
```

### Error 3: sizeof Without ...

```
error: parameter packs not expanded with '...'

Solution: Use sizeof...:
sizeof(Args) → sizeof...(Args)
```

### Error 4: Fold Over an Empty Pack

```
error: fold of empty expansion over operator+

Solution: Binary fold with an init value:
(args + ...)  → (args + ... + 0)
```

### Error 5: Fold Operand Not Parenthesized

```
error: binary expression in operand of fold-expression

Solution:
(std::cout << args, ...)    → ((std::cout << args), ...)
```

### Error 6: Zero-Size Array in the Comma Trick

```
error: zero-size array 'dummy'

Solution: Add a leading element:
int dummy[] = {(f(args), 0)...};  → int dummy[] = {0, (f(args), 0)...};
```

---

## Type Requirements

`Args` can be mixed types, one repeated type, or empty. The operation applied to the pack must be valid for **every** type in it:

```cpp
(args + ...)             // every type must support operator+
std::print("{}", args)   // every type must be formattable
```

---

## C++ Evolution

| Standard | What it added for variadic code |
|----------|---------------------------------|
| C++11 | Variadic templates, parameter packs, `sizeof...` |
| C++14 | Return type deduction (`auto sum(...)`), generic lambdas (`[](auto... values)`) |
| C++17 | Fold expressions, class template argument deduction |
| C++20 | Concepts on packs (`requires (std::integral<Args> && ...)`), packs in lambda captures |
| C++23 | `std::print` / `std::println`, themselves variadic templates |

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
   Without it the final call print() has no match: compile error!

⚠️  RULE 2: Use fold expressions (C++17)
   Much simpler than recursion!

⚠️  RULE 3: Perfect forward when forwarding
   std::forward preserves value categories!

⚠️  RULE 4: Use sizeof...(Args) not sizeof(Args)
   Count elements, not size!

⚠️  RULE 5: Test with 0, 1, many arguments
   Edge cases matter! (sum() with (args + ...) does not compile)
   
   VARIADIC TEMPLATES = TYPE-SAFE FLEXIBILITY!
```

---

**Previous Topic:** [34_TemplateSpecialization](../34_TemplateSpecialization/) - Template Specialization

**Next Topic:** [36_SFINAE](../36_SFINAE/) - SFINAE

---

*Part of the Templates and Generic Programming series*  
*Difficulty: Intermediate to Advanced*  
*Prerequisites: Function Templates, Template Basics*  
*Essential for Modern C++ Generic Programming*

---

## Notes moved from main.cpp

The title/description comment at the top of `main.cpp` ("WHAT ARE VARIADIC TEMPLATES?") stays. What changed:

- `std::cout` → `std::println` / `std::print`; `#include <print>` added, `<iostream>`, `<tuple>` and `<type_traits>` removed (unused). `using namespace std;` is gone and every standard name is written with `std::`.
- Each example has a `// === EXAMPLE n: … ===` title comment. The `=== Example n ===` lines the program prints stay.
- **Printed lecture text removed from the program** (it is covered by the sections above):
  - the "❌ Problems", "✅ Advantages" and "✅ …" bullet lists at the end of the examples → *What Are Variadic Templates?*, *Key Concepts*, *Best Practices*;
  - the whole **`printComprehensiveInfo()`** block (what/syntax, processing patterns, use cases, common mistakes, best practices, type requirements, performance, related concepts, C++ evolution) and the **KEY TAKEAWAYS** at the end of `main()` → *Syntax and Key Concepts*, *Processing Patterns*, *Common Use Cases*, *Best Practices*, *Common Mistakes*, *Performance*, *Related Concepts*, *Type Requirements* and *C++ Evolution* (both new), *Summary*.
- Behavior and code changes:
  - `#include <cstdarg>` was written **inside** `namespace UnsafeVersion`; that is ill-formed and only compiled because the header had already been included by `<iostream>`. It is now at the top of the file.
  - `print` (Example 2) was renamed `printArgs`, so it can never be confused with `std::print` (an unqualified call with a `std::string` argument would find `std::print` through ADL).
  - Example 3: the `print_with_count()` comma trick now has a leading `0` in the array (an empty pack gave a zero-size array), and a call with no arguments is shown. The comment "Alternative: use fold expression" above `count_args` was wrong (it uses `sizeof...`) and was removed.
  - Example 4 now shows all three fold flavors: `<<` fold into a `std::ostringstream` (`concat`), comma fold for printing, and `all_true()` on an empty pack (`true`). `std::println` has no stream to fold `<<` into, which is why the printing helpers use the comma fold.
  - Example 7 is called **Custom printf** instead of "Type-safe printf", and demonstrates the runtime exceptions for a wrong argument count (the old text claimed "type-safe at compile-time", which was only true for the types).
  - Example 8: `min_fold` / `max_fold` were never called, and the lambda trick `auto result = (values, ...)` warns (`left operand of comma operator has no effect`) and **silently truncates** mixed types (`min_fold(1.5, 2)` returned `1`). They now take the first value as the start value and are called.
  - `bool` prints as `true` / `false` instead of `1` / `0`.

### Corrections to the original files

- **Mistake 4 was wrong.** `(cout << args, ...)` is not a "comma operator you don't want"; it does not compile (`binary expression in operand of fold-expression`). Fixed: `((std::cout << args), ...)` works.
- **`func(args + 1...)`** does not compile (`1...` is lexed as a number with two dots). It is `func((args + 1)...)`.
- **Pattern 4 (`{func(args)...};`)** is not valid as a statement; replaced with `std::initializer_list<int>{(func(args), 0)...}`.
- **"Infinite recursion" without a base case** is wrong. The last recursive call is `print()` with no arguments, and the program does not compile: `no matching function for call to 'print()'`. (Mistake 2 said "won't compile with 0 arguments"; it does not compile with any arguments.)
- **Error messages:** `sizeof(Args)` gives `parameter packs not expanded with '...'`, and `func(args)...;` gives `expected ';' before '...' token`; the quoted messages were not what GCC prints.
- **Folds over an empty pack:** `sum()` with `(args + ...)` does not compile; only `&&`, `||`, `,` are allowed. Added to the rules and to the error list.
- **Fold comments** showed `(arg1 + (arg2 + (arg3 + ...)))` and a garbled left-fold diagram; fixed.
- **The `=` operator** was missing from the list of foldable operators.
- **Performance section:** the table ("10-100x"), "~1000ns vs ~10ns" and "100x faster" had no measurements behind them. Removed, replaced by a statement of what is true plus one clearly labelled quick measurement.
- **Examples with mixed types:** `minimum(5, 2.5)` returns `2` (type of the first argument); added a warning.
- **Compile command:** `-std=c++17 VariadicTemplates.cpp` → `-std=c++23 main.cpp`.
- **Footer links:** Previous/Next topic pointed to 32 and 34; the series order is 34_TemplateSpecialization → 35_VariadicTemplates → 36_SFINAE, so they now do.
- `make_unique` was renamed `makeUnique` in the factory example (a user-defined `make_unique` collides with `std::make_unique` via ADL when `std::` types are passed).

### What was tested (GCC 13.3, `-Wall -Wextra`, ASan/UBSan)

- `main.cpp` builds without warnings and runs; output matches the old program except for the intended differences above.
- `func(args + 1...)` → error; `func((args + 1)...)` → compiles.
- `func(args)...;` and `{func(args)...};` → errors; the comma-trick forms `{(f(args), 0)...}`, `{0, (f(args), 0)...}` and `std::initializer_list<int>{…}` compile and run, evaluating left to right; with an empty pack the first form fails with `zero-size array`.
- Recursion without a base case → `no matching function for call to 'p()'`, for any call.
- `sizeof(Args)` → `parameter packs not expanded with '...'`.
- `(std::cout << args, ...)` → error; `((std::cout << args), ...)` and `(std::cout << ... << args)` both print `1a2.5`.
- Empty folds: `(args + ...)` → `fold of empty expansion over operator+`; `&&` and `||` compile.
- Forwarding: `func(args...)` calls the lvalue overload even for `bad(1)`; `func(std::forward<Args>(args)...)` calls the rvalue overload for `1` and the lvalue overload for a variable.
- `makeUnique`, `measure_time` (with `<chrono>`), the recursive `sum`, `(args = ...)` fold, `requires (std::integral<Args> && ...)`, a C++20 pack init-capture `[...a = std::move(args)]`, and `std::apply` with a fold lambda all compile and run.
- `min_fold(1.5, 2)` with the old lambda trick returned `1`; `minimum(5, 2.5)` returns `2`.
- `sum(1, 2, 3, 4, 5)` at `-O2` leaves no call in the generated assembly.
- Quick timing (one run, `-O2`): copying a 1 MiB `std::string` per call ≈ 70 µs; forwarding by reference was not measurable. A va_args sum of 5 ints cost about 8 ns per call, while the template version was optimized away entirely. These are micro-benchmarks and not representative of real code.

Not tested: Clang / MSVC behavior, the undefined behavior of mismatched `va_arg` types (documented, not run), and the claim that variadic templates increase compile time and binary size (generally true, not measured here).

