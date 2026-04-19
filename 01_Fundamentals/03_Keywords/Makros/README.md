# Macros — C++ Preprocessor and Modern Alternatives

> *"Macros are evil. Use them only when there's no other option."*
> — C++ Core Guidelines

---

## What are Macros?

Macros are handled by the **C++ preprocessor** — a text substitution step
that runs **before** the compiler even sees your code. The preprocessor
replaces every occurrence of a macro name with its defined text.

```
Source code  →  Preprocessor  →  Compiler  →  Linker  →  Executable
                (macros expanded)
```

---

## Two files in this folder

| File | What it covers |
|------|---------------|
| `macros_example.cpp` | Old-style `#define` macros — all types with pitfalls |
| `macros_modern_cpp23.cpp` | Modern C++23 replacements for every macro type |

---

## Types of macros

### 1. Object-like macros — constant replacement

```cpp
// Old
#define PI         3.14159265358979
#define MAX_BUFFER 1024
#define APP_NAME   "MCXApp"

// Modern C++23 — always prefer this
constexpr double      PI         = 3.14159265358979;
constexpr int         MAX_BUFFER = 1024;
constexpr std::string_view APP_NAME = "MCXApp";
```

### 2. Function-like macros — code replacement

```cpp
// Old — dangerous! No type safety, double evaluation risk
#define SQUARE(x)        ((x) * (x))
#define MAX(a, b)        ((a) > (b) ? (a) : (b))
#define CLAMP(v, lo, hi) ((v) < (lo) ? (lo) : ((v) > (hi) ? (hi) : (v)))

// Modern C++23 — safe, typed, debuggable
template<typename T>
[[nodiscard]] constexpr T square(T x) noexcept { return x * x; }

template<typename T>
[[nodiscard]] constexpr T clamp(T v, T lo, T hi) noexcept {
    return v < lo ? lo : (v > hi ? hi : v);
}
```

### 3. Conditional compilation

```cpp
// Old — preprocessor only
#define DEBUG_MODE
#ifdef DEBUG_MODE
    std::cout << "debug\n";
#endif

// Modern — if constexpr inside functions
constexpr bool DEBUG_BUILD = true;

template<bool Debug = DEBUG_BUILD>
void logDebug(std::string_view msg) {
    if constexpr (Debug) {
        std::println("[DBG] {}", msg);
    }
    // When false: branch removed entirely by compiler
}
```

### 4. Include guards

```cpp
// Old — error-prone, name collisions possible
#ifndef MYHEADER_H
#define MYHEADER_H
// content
#endif

// Modern — single line, always prefer
#pragma once
```

### 5. Stringification and token concatenation

```cpp
// # turns argument into string literal
#define STRINGIFY(x)  #x
STRINGIFY(hello)  →  "hello"
STRINGIFY(42)     →  "42"

// ## joins two tokens
#define CONCAT(a, b)  a##b
int CONCAT(my, Var) = 42;   // creates: int myVar = 42;
```

### 6. Variadic macros

```cpp
// Takes variable number of arguments
#define LOG(fmt, ...)  printf("[LOG] " fmt "\n", ##__VA_ARGS__)

LOG("started");
LOG("session %s on port %d", "SES-001", 5060);
```

### 7. Predefined macros

```cpp
__FILE__   // current source filename
__LINE__   // current line number
__DATE__   // compilation date "Apr 19 2026"
__TIME__   // compilation time "12:34:56"
__func__   // current function name (C++11)

// Modern replacement — std::source_location (C++20)
void log(std::string_view msg,
         const std::source_location& loc = std::source_location::current())
{
    std::println("[{}:{}] {}", loc.file_name(), loc.line(), msg);
}
// Automatically captures file, line, function — no macro needed
```

---

## Full comparison table

```mermaid
flowchart TD
    subgraph Old ["❌ Old — #define macros"]
        O1["#define PI 3.14"]
        O2["#define SQUARE(x) ((x)*(x))"]
        O3["#ifdef DEBUG"]
        O4["#ifndef HEADER_H / #define HEADER_H"]
        O5["__FILE__ / __LINE__"]
        O6["#define PI 3.14159"]
        O7["enable_if SFINAE macro"]
    end

    subgraph New ["✅ Modern C++23"]
        N1["constexpr double PI"]
        N2["template T square(T x)"]
        N3["if constexpr (Debug)"]
        N4["#pragma once"]
        N5["std::source_location"]
        N6["std::numbers::pi"]
        N7["concept Numeric"]
    end

    O1 --> N1
    O2 --> N2
    O3 --> N3
    O4 --> N4
    O5 --> N5
    O6 --> N6
    O7 --> N7
```

---

## Why #define is dangerous

### Problem 1 — No parentheses = wrong result

```cpp
#define DOUBLE(x)  x * 2

DOUBLE(3 + 4)
// expands to: 3 + 4 * 2 = 11   NOT 14!
// Fix: #define DOUBLE(x) ((x) * 2)
```

### Problem 2 — Double evaluation

```cpp
#define MAX(a, b)  ((a) > (b) ? (a) : (b))

int i = 5, j = 3;
MAX(i++, j++)
// expands to: ((i++) > (j++) ? (i++) : (j++))
// i is incremented TWICE — undefined behavior!

// Template function: argument evaluated once — safe
```

### Problem 3 — No type safety

```cpp
#define PI 3.14
int x = PI;       // silently truncates to 3 — no warning
std::string s = PI;  // might compile depending on context

// constexpr double PI:
int x = PI;       // compiler warning or error
```

### Problem 4 — No scope

```cpp
#define MAX 100   // pollutes entire translation unit
// Clashes with: std::max, class MAX, variable max, anything named MAX
```

### Problem 5 — Invisible to debugger

```cpp
#define SQUARE(x) ((x) * (x))
// Debugger shows: ((5) * (5))
// Not: "calling SQUARE with x=5"
```

---

## std::numbers — built-in constants (C++20)

```cpp
#include <numbers>

std::numbers::pi      // 3.14159265358979...
std::numbers::e       // 2.71828182845905...
std::numbers::sqrt2   // 1.41421356237310...
std::numbers::phi     // 1.61803398874989... (golden ratio)
std::numbers::ln2     // 0.69314718055994...

// Available for any floating point type
std::numbers::pi_v<float>        // float precision
std::numbers::pi_v<double>       // double precision
std::numbers::pi_v<long double>  // long double precision
```

---

## consteval vs constexpr

```cpp
// constexpr — CAN be evaluated at compile time
constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n-1);
}
constexpr int a = factorial(6);   // compile time ✅
int b = factorial(someRuntimeValue);  // runtime — also allowed

// consteval — MUST be evaluated at compile time
consteval int factorial(int n) { ... }
constexpr int a = factorial(6);   // compile time ✅
int b = factorial(someRuntimeValue);  // ERROR — not compile time!
```

Use `consteval` when you want to **guarantee** something is always computed
at compile time — no accidental runtime calls.

---

## When #define is still acceptable

Even in modern C++23, some macros remain:

```cpp
// 1. Platform detection — no alternative
#if defined(__linux__)
    // Linux-specific code
#elif defined(_WIN32)
    // Windows-specific code
#endif

// 2. Include guards — #pragma once not in ISO standard
//    (but supported everywhere — prefer #pragma once)
#ifndef MYHEADER_H
#define MYHEADER_H
#endif

// 3. C interoperability — C doesn't have constexpr
#define C_COMPATIBLE_CONSTANT 42

// 4. Disabling warnings in specific sections
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated"
// ... use deprecated API ...
#pragma GCC diagnostic pop
```

---

## Compile commands

```bash
# Old macros example
g++ -std=c++23 -O2 macros_example.cpp -o macros_old && ./macros_old

# Modern C++23 example
g++ -std=c++23 -O2 macros_modern_cpp23.cpp -lbenchmark -lpthread -o macros_modern && ./macros_modern

# With GCC 14 (for <print>, <source_location>, std::numbers)
/usr/local/gcc-14.1.0/bin/g++-14.1.0 -std=c++23 -O2 macros_modern_cpp23.cpp -lstdc++exp -o macros_modern
```

---

## Summary

| | `#define` | Modern C++23 |
|--|-----------|-------------|
| Type safety | ❌ None | ✅ Fully typed |
| Scope | ❌ Global | ✅ Namespace/function |
| Debugger | ❌ Invisible | ✅ Named symbol |
| Overloadable | ❌ No | ✅ Templates |
| Side effects | ❌ Double eval | ✅ Single eval |
| Compile-time | ✅ Always | ✅ constexpr/consteval |
| Still needed | Platform detect, C interop | Everything else |
