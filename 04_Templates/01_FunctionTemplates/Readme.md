# FunctionTemplates - Generic Programming Foundation

## 📚 Overview

This folder introduces **Function Templates** - the foundation of generic programming in C++. Templates allow you to write code once that works with any type, providing type-safe generics with no runtime cost for the abstraction itself. They're the backbone of the Standard Template Library (STL) and essential for modern C++ development.

**Function Templates** = Write once, use with any type

This is **ESSENTIAL** for generic programming and understanding the STL.

> **Status marks**
> ✅ *tested* = compiled and run with GCC 13.3 (`-std=c++23`, `-Wall -Wextra`, AddressSanitizer, UBSan).
> ⚠️ *not tested* = from the language rules or common practice, not run here.

---

## 🎯 What You'll Learn

- What function templates are and why they exist
- Template syntax and type parameters
- Type deduction (implicit and explicit)
- Multiple template parameters
- Template specialization (and why overloads are often better)
- Overloading with templates
- Non-type template parameters
- Return type deduction
- Template constraints (SFINAE, `if constexpr`, concepts)
- Real-world usage patterns
- Best practices and common pitfalls

---

## 📖 Concept Explanation

### What are Function Templates?

**Function templates** are blueprints for creating functions that work with any type. Instead of writing separate functions for int, double, string, etc., you write one template that the compiler uses to generate type-specific functions.

```cpp
// Without templates - need separate functions
int maxInt(int a, int b) { return (a > b) ? a : b; }
double maxDouble(double a, double b) { return (a > b) ? a : b; }
std::string maxString(std::string a, std::string b) { return (a > b) ? a : b; }
// Duplicated code! 💥

// With templates - one function for all types
template<typename T>
T maximum(T a, T b) {
    return (a > b) ? a : b;
}
// Works with any type that has operator>! ✅
```

**Key Properties:**
- Template is a blueprint, not actual code
- Compiler generates specific functions (instantiations)
- Type-safe (compile-time checking)
- No runtime overhead from the abstraction itself (the cost is paid at compile time, and in code size if you instantiate many types)
- One template → many functions
- `class` and `typename` mean the same in `template<class T>` / `template<typename T>`

### How Templates Work

```
COMPILATION PROCESS:

1. Template Definition
   ┌─────────────────────────┐
   │ template<typename T>    │
   │ T add(T a, T b) {      │
   │     return a + b;       │
   │ }                       │
   └─────────────────────────┘
         Blueprint only

2. Template Usage
   ┌─────────────────────────┐
   │ int r1 = add(5, 10);   │
   │ double r2 = add(3.1, 2.7);│
   └─────────────────────────┘
         Trigger instantiation

3. Compiler Generates
   ┌─────────────────────────┐
   │ int add(int a, int b) { │
   │     return a + b;       │
   │ }                       │
   │                         │
   │ double add(double a, double b) { │
   │     return a + b;       │
   │ }                       │
   └─────────────────────────┘
         Actual functions

4. Result
   One template → Multiple functions
   Generated at compile time
   Each type gets its own function
```

✅ Tested: calling `maximum` with `int`, `double` and `char` (at `-O0`) leaves three separate functions in the binary: `maximum<int>`, `maximum<double>`, `maximum<char>` (visible with `nm -C`). With optimization the compiler may inline them instead. The old text said "each type gets an *optimized* version"; what is true is that each type gets its *own* version, which is then optimized like any other function.

### Real-World Analogy

```
Think of templates like a cookie cutter:

WITHOUT TEMPLATES (Cookie Cutters):
  - One cutter for chocolate cookies
  - One cutter for sugar cookies
  - One cutter for ginger cookies
  - Different cutter for each type!

WITH TEMPLATES (Universal Cutter):
  - One cutter shape (template)
  - Works with any dough (type)
  - Same shape, different flavors
  - Make cookies for any occasion!

The template is the shape,
the type is the dough,
the result is delicious cookies! 🍪
```

---

## 💻 Basic Syntax

### Simple Function Template

```cpp
// Template declaration
template<typename T>
T maximum(T a, T b) {
    return (a > b) ? a : b;
}

// Usage - type deduction
int m1 = maximum(10, 20);           // T = int
double m2 = maximum(3.14, 2.71);    // T = double
char m3 = maximum('a', 'z');        // T = char
```

### Multiple Type Parameters

```cpp
template<typename T1, typename T2>
void printPair(T1 first, T2 second) {
    std::println("{}, {}", first, second);
}

// Different types
printPair(42, 3.14);        // T1=int, T2=double
printPair("Age", 25);       // T1=const char*, T2=int
printPair(true, "Success"); // T1=bool, T2=const char*
```

### Explicit Type Specification

```cpp
// Implicit type deduction
maximum(10, 20);            // T deduced as int

// Explicit type specification
maximum<int>(10, 20);       // T explicitly int
maximum<double>(5, 10);     // T explicitly double (converts args)
```

✅ `maximum<double>(5, 10)` returns `10` as a `double`.

---

## 🔍 Key Concepts

### 1. Type Deduction

The compiler automatically determines the type parameter based on arguments:

```cpp
template<typename T>
void show(T value) {
    std::println("{}", value);
}

show(42);          // T = int
show(3.14);        // T = double
show("Hello");     // T = const char*
show(std::string("Hi")); // T = std::string
```

**Rules:**
- Every use of `T` must deduce to the same type, otherwise deduction fails
- Implicit conversions between argument types are not tried to make them match
- Deduction itself still applies a few adjustments: arrays and functions decay to pointers, and top-level `const` is dropped for by-value parameters (this is why `"Hello"` becomes `const char*`)
- Can specify explicitly when needed

**Example of deduction rules:**
```cpp
template<typename T>
T add(T a, T b) { return a + b; }

add(5, 10);       // ✅ OK - both int
add(5, 10.5);     // ❌ Error - int and double don't match
add<double>(5, 10.5);  // ✅ OK - explicit type
```

✅ Tested: `add(5, 10.5)` → `no matching function for call to 'add(int, double)'`; `add<double>(5, 10.5)` compiles.

`typeid(T).name()` (used in Examples 3 and 4) is implementation-defined. GCC prints mangled names (`i`, `d`, `PKc`, ...); MSVC prints readable ones. ✅ GCC output: `i`, `d`, `PKc` for `int`, `double`, `const char*`.

#### ⚠️ Pitfall: string literals deduce `const char*`, not `string`

```cpp
template<typename T>
T maximum(T a, T b) { return (a > b) ? a : b; }

maximum("apple", "banana");   // T = const char*  →  compares POINTERS, not text!
maximum(std::string("apple"), std::string("banana"));   // ✅ compares text
```

✅ Tested: `maximum("apple", "banana")` and `maximum("zebra", "ant")` compile without any warning and return whichever literal has the higher address, which is not guaranteed to be alphabetical (they happened to return `apple` and `zebra`, i.e. the *wrong* answer for the first). The old lecture text in `main.cpp` claimed `string m3 = maximum("a", "b"); // T = string`, which is incorrect: `T` is `const char*`. This is why `main.cpp` Example 2 passes `string("apple")`.

With `const T&` parameters the situation is different: literals of different lengths have different array types (`const char[6]` vs `const char[7]`), so deduction fails with `no matching function for call to 'maxRef(const char [6], const char [7])'` ✅.

### 2. Template Specialization

Sometimes you need custom behavior for specific types:

```cpp
// Generic template
template<typename T>
T add(T a, T b) {
    return a + b;
}

// Real specialization for bool: same signature as the template with T = bool
template<>
bool add<bool>(bool a, bool b) {
    return a || b;
}

// Plain overload (NOT a specialization) for string literals
std::string add(const char* a, const char* b) {
    return std::string(a) + std::string(b);
}

// Usage
add(10, 20);            // generic - returns 30
add(true, false);       // specialization - logical OR
add("Hello", " World"); // the overload - concatenates
```

**Syntax:**
```cpp
template<>
ReturnType FunctionName<SpecificType>(parameters) {
    // Custom implementation
}
```

**Correction.** The old README (and a comment in `main.cpp`) wrote the string case as a specialization:

```cpp
template<>
std::string add<const char*>(const char* a, const char* b) { ... }   // ❌ does not compile
```

✅ GCC: `template-id 'add<const char*>' for 'std::string add(const char*, const char*)' does not match any template declaration`. A specialization must have exactly the signature the template would have for that `T`: for `T = const char*` the return type is `const char*`, not `string`. A specialization with the right signature (`const char* add<const char*>(const char*, const char*)`) does compile ✅, but for string literals the **plain overload** is what you want, and `main.cpp` Example 5 uses exactly that (the comment there now says "overload, not specialization").

⚠️ General advice (not tested here): prefer overloads to function template specializations. Specializations do not take part in overload resolution; only the primary template does, which can lead to surprising choices.

### 3. Overloading with Templates

You can have both template and regular functions with the same name:

```cpp
// Template version
template<typename T>
void display(T value) {
    std::println("Template: {}", value);
}

// Regular version
void display(int value) {
    std::println("Regular: {}", value);
}

display(42);      // Calls regular (same match quality, non-template wins)
display(3.14);    // Calls template (no double overload)
display("Hi");    // Calls template
```

**Overload resolution, in the right order:**
1. Candidates are the regular functions **and** the templates (after deduction).
2. The one with the **better conversion sequences** wins. A template that matches *exactly* beats a regular function that needs a conversion.
3. Only if that is a tie, a **non-template** beats a template.
4. If both are templates, the more specialized one wins. (⚠️ not tested here)

The old list ("1 exact non-template, 2 template exact, 3 template with conversion, 4 non-template with conversion") was wrong at the end: a non-template that needs a conversion *loses* to a template that is an exact match. ✅ Tested with `template<T> display(T)` and `display(int)`:

| Call | Chosen | Why |
|------|--------|-----|
| `display(42)` | regular int | tie → non-template wins |
| `display('x')` | **template** (`T = char`) | exact match beats the promotion `char → int` |
| `display(3.14)` | template | `double → int` would be a conversion |
| `display(42L)` | **template** (`T = long`) | exact match beats the conversion `long → int` |
| `display<>(42)` | template | `<>` forces the template |

So "regular functions preferred" is only a tie-breaker, not a rule that beats everything. `main.cpp` Example 6 (which has both `display(int)` and `display(double)`) shows `display('x')` going to the template.

### 4. Non-Type Template Parameters

Templates can have compile-time constant parameters:

```cpp
template<typename T, int Size>
class Array {
    T data[Size];  // Size is compile-time constant
public:
    int size() const { return Size; }
};

Array<int, 5> arr1;     // int array, size 5
Array<double, 10> arr2; // double array, size 10
```

**Allowed Non-Type Parameters:**
- Integral types (int, char, bool)
- Enums
- Pointers
- References
- Must be compile-time constants!
- Since C++20 also floating-point values and "structural" class types; since C++17 `template<auto N>` deduces the type of the value ✅ (`template<double D>` and `template<auto N>` compile with `-std=c++20`; with `-std=c++17` GCC says `'double' is not a valid type for a template non-type parameter`).

The value is part of the type: `Array<int, 5>` and `Array<int, 6>` are different types. Typical uses: array sizes, matrix dimensions, buffer sizes, compile-time options.

`printArray(const T (&arr)[Size])` (Example 7) deduces both `T` and `Size` from an array argument, because taking the array **by reference** stops it from decaying to a pointer.

### 5. Return Type Deduction

**C++11 - Trailing Return Type:**
```cpp
template<typename T1, typename T2>
auto add(T1 a, T2 b) -> decltype(a + b) {
    return a + b;
}
```

**C++14 - Full Auto:**
```cpp
template<typename T1, typename T2>
auto add(T1 a, T2 b) {
    return a + b;  // Type deduced from return
}
```

`decltype(a + b)` is "the type of the expression `a + b`": `int + int` → `int`, `int + double` → `double`. ✅ Example 8 prints `Results: 30, 12.5, 4` and `Results: 50, 10`.

### 6. Constraining a template (C++11 → C++20)

```cpp
// SFINAE (works since C++11)
template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value, T>::type
safeAdd(T a, T b) { return a + b; }

// Concepts (C++20) - same idea, readable
template<typename T> requires is_arithmetic_v<T>
T safeAdd(T a, T b) { return a + b; }
```

✅ Both reject `safeAdd("a", "b")` with `no matching function for call to 'safeAdd(const char [2], const char [2])'`.

**`if` versus `if constexpr`.** `checkType` in Example 9 uses a plain `if (is_integral<T>::value)`. A plain `if` still **compiles every branch for every `T`**, so the template only works if all branches are valid for every type. ✅ Tested with a non-streamable struct: `if (is_integral<T>::value) cout << v;` fails with `no match for 'operator<<'`, while `if constexpr (is_integral_v<T>) cout << v;` compiles because the false branch is discarded. Use `if constexpr` (C++17) when branches only make sense for some types.

---

## 📋 Examples in This Folder

`main.cpp` has 10 examples:

1. **The Problem Without Templates** - separate function per type
2. **Basic Function Template** - one template for all types
3. **Type Deduction** - implicit vs explicit
4. **Multiple Template Parameters** - different types in one template
5. **Template Specialization** - the `bool` specialization and the `const char*` overload
6. **Overloading with Templates** - templates and regular functions together
7. **Non-Type Template Parameters** - compile-time constants
8. **Return Type Deduction** - `auto` and `decltype`
9. **Template Constraints** - SFINAE and type checking
10. **Real-World Examples** - swap, print container, find, sum

---

## 🎓 When to Use Templates

### ✅ Use Templates When:

**Generic algorithms:**
```cpp
template<typename T>
void sort(T* std::array, int size) {
    // Works with any comparable type
}
```

**Type-independent operations:**
```cpp
template<typename T>
void mySwap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}
```

**Container classes:**
```cpp
template<typename T>
class Vector {
    T* data;
    // Generic storage
};
```

**Code reuse:**
```cpp
template<typename T>
T minimum(T a, T b) { return (a < b) ? a : b; }
// One function for all types
```

### ❌ Don't Use Templates When:

- Type-specific behavior needed
- Adds unnecessary complexity
- Simple function used once
- Debugging is more important than generality
- Compile times are critical

---

## ⚠️ Common Mistakes

### ❌ Mistake 1: Type Mismatch in Deduction

**Bad:**
```cpp
template<typename T>
T add(T a, T b) { return a + b; }

add(5, 10.5);  // ❌ Error: int and double don't match
```

**Problem:** every use of `T` must deduce to the same type

**Fix:**
```cpp
add<double>(5, 10.5);  // ✅ Explicit type
// OR
template<typename T1, typename T2>
auto add(T1 a, T2 b) -> decltype(a + b) {
    return a + b;
}
```

### ❌ Mistake 2: Forgetting Type Requirements

**Bad:**
```cpp
template<typename T>
T maximum(T a, T b) {
    return (a > b) ? a : b;  // Assumes operator>
}

struct NoCompare { int x; };
maximum(NoCompare{1}, NoCompare{2});  // ❌ Error!
```

**Problem:** T must support `operator>` (the old comment said `operator<`, but the code uses `>`)

✅ GCC: `no match for 'operator>' (operand types are 'NoCompare' and 'NoCompare')`, reported inside the template, which is why concepts give better messages.

**Fix:** Document requirements or use concepts (C++20)

### ❌ Mistake 3: Template in .cpp File

**Bad:**
```cpp
// header.h
template<typename T>
T maximum(T a, T b);

// implementation.cpp
template<typename T>
T maximum(T a, T b) {
    return (a > b) ? a : b;
}
// ❌ Linker error!
```

**Problem:** Templates must be in header or explicitly instantiated

✅ Tested: `main.cpp` calling `maximum(3, 7)` fails at link time with `undefined reference to 'int maximum<int>(int, int)'`, because `implementation.cpp` never instantiated it.

**Fix:** Keep the template definition in the header, **or** explicitly instantiate the types you need in the `.cpp`:
```cpp
template int maximum<int>(int, int);
template double maximum<double>(double, double);
```
✅ Tested: with these two lines the program links and prints `7`. Only those two types will work.

### ❌ Mistake 4: Unnecessary Copies

**Bad:**
```cpp
template<typename T>
void process(T value) {  // Copies large objects!
    // Use value
}
```

**Fix:**
```cpp
template<typename T>
void process(const T& value) {  // ✅ Reference
    // Use value
}
```

(For small types like `int` a by-value parameter is just as good. And remember the side effect from above: with `const T&`, array arguments no longer decay, so `process("a", "bc")`-style calls with two parameters of the same `T` fail.)

### ❌ Mistake 5: Comparing string literals

See [the pitfall above](#️-pitfall-string-literals-deduce-const-char-not-string) ✅.

### ❌ Mistake 6: Reusing standard names together with `using namespace std`

```cpp
template<typename T, typename... A>
std::unique_ptr<T> std::make_unique(A&&... a) { ... }   // your own version

auto p = std::make_unique<int>(5);   // ❌ ambiguous with std::make_unique
```
✅ GCC: `call of overloaded 'make_unique<int>(int)' is ambiguous`.

The same happened to the original `main.cpp`: it defined its own `accumulate`, which compiled only because `<numeric>` was not included. ✅ Adding `#include <numeric>` makes `accumulate(numbers.begin(), numbers.end(), 0)` fail with `call of overloaded 'accumulate(...)' is ambiguous` (the vector iterators belong to `std`, so `std::accumulate` is found through argument-dependent lookup). The function is now called `sumRange`. For the same reason the example swap is called `mySwap`, not `swap`.

---

## 🎯 Practice Exercises

### Exercise 1: Generic Swap
Implement a template swap function that works with any type.

### Exercise 2: Min of Three
Create a template that finds minimum of three values.

### Exercise 3: Generic Print
Write a template that prints any container.

### Exercise 4: Array Sum
Template function that sums array of any numeric type.

### Exercise 5: Type-Safe Compare
Create a comparison template with proper specializations.

---

## 🚀 How to Run

### Compile:
```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o function_templates
```

`main.cpp` prints with `std::println` (`<print>`), so it needs **C++23 and GCC 14+** (or another standard library that ships `<print>`). The templates themselves need C++11 at minimum; the trailing `decltype` form is C++11, `auto` return deduction is C++14, `if constexpr` is C++17, concepts are C++20.

### Run:
```bash
./function_templates
```

### Expected Output:
The program demonstrates:
1. Problem without templates
2. Basic template usage
3. Type deduction
4. Multiple parameters
5. Specialization
6. Overloading
7. Non-type parameters
8. Return type deduction
9. Constraints (SFINAE)
10. Real-world patterns

**Output differs from the old `cout` version in two ways** (both come from `std::println`, not from the templates):
- The `"  maxInt called"` style messages now appear **before** the line that uses the result. Old output: `Max int:   maxInt called` followed by `20` on the next line (the function printed in the middle of the `cout` chain). New output: `  maxInt called`, then `Max int: 20`. This is because `println` evaluates its arguments first.
- `bool` values print as `true` / `false` instead of `1` / `0` (`add(true, false)` → `true`; `printPair(true, "Success")` → `Pair: (true, Success)`).

---

## 📊 Visual Concepts

### Template Instantiation

```
TEMPLATE:
┌──────────────────────┐
│ template<typename T> │
│ T max(T a, T b) {   │
│   return (a>b)?a:b; │
│ }                    │
└──────────────────────┘
         │
         │ Compiler sees usage
         │
    ┌────┴────┬────────┬────────┐
    │         │        │        │
    ▼         ▼        ▼        ▼
┌───────┐ ┌────────┐ ┌──────┐ ┌───────┐
│ max   │ │  max   │ │ max  │ │  max  │
│ int   │ │ double │ │ char │ │string │
└───────┘ └────────┘ └──────┘ └───────┘

One template → Multiple functions
Generated at compile time
Each type gets its own function
```

### Type Deduction

```
FUNCTION CALL:
maximum(10, 20)
    │
    ├─ Argument 1: 10 → type int
    └─ Argument 2: 20 → type int
         │
         ▼
    T = int (deduced)
         │
         ▼
Instantiate: int maximum(int a, int b)
```

### Overload Resolution

```
display(42)
    │
    ├─ Candidates: display(int) AND display<int>(int)
    │  → both exact matches (a tie)
    │  → tie-breaker: non-template wins
    │  → void display(int) ✅ USE THIS
    │
display('x')
    │
    ├─ display(int):      needs promotion char → int
    ├─ display<char>(char): exact match ✅
    │  → better conversion sequence wins
    │  → template ✅ USE THIS
    │
display(3.14)
    │
    └─ no display(double) → template with T = double ✅
```

---

## 📚 Related Topics

### Prerequisites:
- **Functions** - Basic function concepts
- **Types** - Understanding C++ types
- **References** - Pass by reference

### Coming Next:
- **33_ClassTemplates** - Template classes
- **34_TemplateSpecialization** - Advanced specialization
- **35_VariadicTemplates** - Variable arguments

### Related Concepts:
- **Generic Programming** - Paradigm
- **STL** - Uses templates extensively
- **Type Traits** - Template metaprogramming

---

## 🎓 Summary

### What We Learned:

✅ **Templates** are blueprints for functions
✅ **Write once**, use with any type (that supports the operations used)
✅ **Type deduction** is automatic, and must agree for every use of `T`
✅ **Compiler generates** a specific function per type
✅ **Specialization** for custom behavior (often an overload is simpler)
✅ **Overloading** with regular functions (better match first, non-template as tie-breaker)
✅ **Non-type parameters** for constants
✅ **Return type deduction** with auto/decltype
✅ **No runtime overhead** from the abstraction - the work happens at compile time
✅ **Foundation of STL** and generic programming

### The Complete Pattern:

```cpp
// ============================================
// FUNCTION TEMPLATE PATTERNS
// ============================================

// 1. BASIC TEMPLATE
template<typename T>
T maximum(T a, T b) {
    return (a > b) ? a : b;
}

// 2. MULTIPLE PARAMETERS
template<typename T1, typename T2>
void printPair(T1 first, T2 second) {
    std::println("{}, {}", first, second);
}

// 3. CONST REFERENCES (efficient)
template<typename T>
void process(const T& value) {
    // No copy for large objects
}

// 4. RETURN TYPE DEDUCTION
template<typename T1, typename T2>
auto add(T1 a, T2 b) -> decltype(a + b) {
    return a + b;
}

// 5. SPECIALIZATION (same signature as the template for that T)
template<typename T>
T func(T param) {
    // Generic implementation
    return param;
}

template<>
std::string func<std::string>(std::string param) {
    // Custom for string
    return param + "!";
}

// 6. NON-TYPE PARAMETER
template<typename T, int Size>
void printArray(T (&arr)[Size]) {
    for (int i = 0; i < Size; ++i) {
        std::print("{} ", arr[i]);
    }
}

// 7. OVERLOADING
template<typename T>
void display(T value) { }  // Template

void display(int value) { }  // Regular (wins ties, e.g. for int)

// 8. GENERIC ALGORITHMS
template<typename Container>
void printAll(const Container& c) {
    for (const auto& item : c) {
        std::print("{} ", item);
    }
}
```

---

## ✨ Quick Reference Card

```cpp
// === BASIC SYNTAX ===
template<typename T>
T func(T param) { return param; }

// === USAGE ===
func(42);           // T = int
func<int>(42);      // Explicit

// === MULTIPLE TYPES ===
template<typename T1, typename T2>
void func(T1 a, T2 b) { }

// === NON-TYPE PARAMETER ===
template<typename T, int N>
void func(T (&arr)[N]) { }

// === SPECIALIZATION ===
template<>
void func<int>(int x) { }

// === RETURN TYPE DEDUCTION ===
template<typename T1, typename T2>
auto func(T1 a, T2 b) -> decltype(a+b) {
    return a + b;
}

// === CONST REFERENCE (efficient) ===
template<typename T>
void func(const T& param) { }

// === CONSTRAINT (C++20) ===
template<typename T> requires is_arithmetic_v<T>
T func(T a, T b) { return a + b; }

// === TYPE REQUIREMENTS ===
// T must support:
// - Copy constructor
// - operator> (for comparisons)
// - operator+ (for addition)
// Document these!
```

---

## 🔍 Best Practices

### ✅ DO:

- **Use meaningful names** (T, U, V or Value, Key)
- **Prefer const references** for large parameters (small types can be passed by value)
- **Document type requirements** (or express them with concepts)
- **Keep templates simple**
- **Test with multiple types**
- **Put templates in headers** (or explicitly instantiate)
- **Use auto for return types** when appropriate
- **Prefer overloads over specializations** when you can
- **Use `if constexpr`** when branches depend on the type
- **Consider concepts** (C++20)
- **Provide good error messages**
- **Don't reuse standard names** (`swap`, `accumulate`, `make_unique`) together with `using namespace std`

### ❌ DON'T:

- **Over-complicate templates**
- **Forget type requirements**
- **Ignore compiler errors**
- **Use templates unnecessarily**
- **Forget const-correctness**
- **Make too general**
- **Put in .cpp files** (usually)
- **Assume all types work**
- **Create circular dependencies**
- **Ignore compilation time**

---

## 🌟 Real-World Applications

### STL Algorithms
```cpp
template<typename Iter, typename Func>
void for_each(Iter begin, Iter end, Func f) {
    for (auto it = begin; it != end; ++it) {
        f(*it);
    }
}
```

### Generic Swap
```cpp
template<typename T>
void mySwap(T& a, T& b) {
    T temp = std::move(a);
    a = std::move(b);
    b = std::move(temp);
}
```
(`std::swap` already does this; the example is called `mySwap` to avoid a name clash.)

### Container Operations
```cpp
template<typename Container>
typename Container::value_type sum(const Container& c) {
    typename Container::value_type result{};
    for (const auto& item : c) {
        result += item;
    }
    return result;
}
```

### Generic Factories
```cpp
template<typename T, typename... Args>
std::unique_ptr<T> makeUnique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
```
(`std::make_unique` already exists, see Mistake 6; the `Args&&...` part is a variadic template, covered in 35_VariadicTemplates.)

---

## 📖 Further Reading

- [cppreference.com - Function templates](https://en.cppreference.com/w/cpp/language/function_template)
- "C++ Templates: The Complete Guide" by Vandevoorde & Josuttis
- "Effective C++" by Scott Meyers - Items on templates
- [Template Tutorial](https://isocpp.org/wiki/faq/templates)

---

## 🎯 Key Takeaways

1. **Templates** are blueprints, not code
2. **Write once**, use with any type
3. **Type deduction** happens automatically
4. **Compiler generates** specific functions
5. **No runtime overhead** from the abstraction - it is a compile-time feature
6. **Type requirements** must be met
7. **Specialization** for custom behavior (overloads are often simpler)
8. **Keep in headers** (usually)
9. **Foundation of STL** algorithms
10. **Essential for** modern C++

---

## 🚨 THE GOLDEN RULES

```
⚠️  RULE 1: Template is a blueprint
   Not actual code until used!

   Compiler generates functions on demand
   One template → many functions

⚠️  RULE 2: Type must support operations
   Document requirements!

   maximum(a,b) needs operator>
   sort needs operator<

⚠️  RULE 3: Prefer const references
   For large types!

   template<typename T>
   void func(const T& param)

⚠️  RULE 4: Keep in headers
   Or explicitly instantiate!

   Templates need full definition visible

⚠️  RULE 5: Test with multiple types
   Don't assume it works!

   Test int, double, string, custom types

⚠️  RULE 6: Better match first
   In overload resolution a non-template only wins ties.
   Exact match beats a conversion, template or not.

   USE TEMPLATES FOR CODE REUSE AND TYPE SAFETY!
```

---

**Previous Topic:** [31_InitializerLists](../31_InitializerLists/) - Uniform Initialization

**Next Topic:** [33_ClassTemplates](../33_ClassTemplates/) - Template Classes

---

## Notes moved from main.cpp

The title/description comment at the top of `main.cpp` ("WHAT ARE FUNCTION TEMPLATES?") stays. What changed:

- `std::cout` → `std::println` / `std::print`; `#include <print>` added, `<iostream>` removed. `<typeinfo>` added (`typeid` was used without it), the unused `<algorithm>` removed.
- Each example has a `// === EXAMPLE n: … ===` title comment.
- **Printed lecture text removed from the program** (it is covered by the sections above):
  - the "❌ PROBLEMS", "✅ TEMPLATE SOLUTION" and "💡 …" bullet lists at the end of Examples 1–10 → *When to use templates*, *Key Concepts* (type deduction, specialization, overloading, non-type parameters, return type deduction, constraints);
  - the whole **COMPREHENSIVE EXPLANATION** block printed by `main()` (What are function templates, Why templates, How templates work, Type deduction, Multiple parameters, Specialization, Overload resolution, Non-type parameters, Best practices, Common patterns, Quick reference, Key takeaways, Golden rules) → *Concept Explanation*, *Basic Syntax*, *Key Concepts*, *Best Practices*, *Quick Reference Card*, *Key Takeaways*, *THE GOLDEN RULES*.
- Example numbering is unchanged (all 10 examples run).

### Corrections to the original files

1. **Invalid specialization** `template<> string add<const char*>(...)` (README) and the wrong comment "Specialization for const char*" (`main.cpp`): that function is an **overload**. Fixed in both.
2. **Overload priority list** was wrong (a non-template with a conversion does not beat an exact-match template). Rewritten, with a test table.
3. **`maximum("a", "b")` is not `T = string`** (lecture text in `main.cpp`): it is `const char*` and compares pointers.
4. **`accumulate` renamed `sumRange`** (ambiguous with `std::accumulate` as soon as `<numeric>` is included).
5. **`typeid` without `<typeinfo>`**: include added; the printed names are implementation-defined.
6. **"Type deduction: types must match exactly / no conversions"** was too absolute: arrays and functions decay, top-level `const` is dropped.
7. **"Each type gets optimized version" / "Zero runtime overhead"** reworded: each type gets its own function; the cost moves to compile time and code size.
8. **Comment in Mistake 2** said `operator<` while the code uses `>`.
9. **`if` vs `if constexpr`:** `checkType` uses a plain `if`; noted in the constraints section.
10. **`swap` and `make_unique`** examples clashed with `std` names under `using namespace std`: renamed (`mySwap`, `makeUnique`).
11. **Compile command** `-std=c++14` → `-std=c++23` because of `<print>`.
12. **Output order and bool formatting changed** with `println` (see *Expected Output*).

### Tested

✅ `g++ -std=c++23 -Wall -Wextra` with AddressSanitizer + UBSan: no warnings, no errors.
✅ Output compared line by line with the original run: identical except the reordered function messages, `true`/`false` instead of `1`/`0`, three changed sub-headings (Examples 5 and 10) and the removed lecture lines. All numeric values match (for example `7.2`, `6`, `12.5`).
✅ Claims marked ✅ above: deduction error for `add(5, 10.5)`, invalid specialization, valid specialization, pointer comparison with string literals, `const T&` with literals of different length, missing `operator>`, template in a `.cpp` (link error and explicit-instantiation fix), overload table (`42`, `'x'`, `3.14`, `42L`, `<>`), C++20 floating-point and `auto` non-type parameters (and the C++17 error), `if` versus `if constexpr`, SFINAE versus concepts error messages, `make_unique` and `accumulate` ambiguity, separate instantiations in the binary.
⚠️ Not tested: partial ordering between two templates, "prefer overloads over specializations", compile-time and code-size cost, and `std::println` with GCC 14 itself (the sandbox has GCC 13; a small stand-in header with the same behavior was used).

---

*Part of the Templates and Generic Programming series*
*Difficulty: Intermediate*
*Prerequisites: Functions, types, references*
*CRITICAL for Generic Programming*
