# 02 – Variables in C++23

This project demonstrates variable declaration, initialization, scoping rules,
`const`/`constexpr`, and `static` variables in modern C++.

---

## Example 1 — Declaration & Initialization

### Basic declaration
```cpp
int age = 25;           // declaration + assignment
int uninitialzed;       // declaration only — AVOID, value is undefined
```

### Uniform initialization (C++11) — preferred modern style
```cpp
int    score{100};
double temperature{36.6};
```
Uniform initialization with `{}` is safer than `=` because it **prevents narrowing conversions**:
```cpp
int x = 3.7;    // compiles — silently truncates to 3
int x{3.7};     // ERROR — narrowing conversion not allowed
```

### `auto` — type deduction
```cpp
auto i = 42;            // int
auto d = 3.14;          // double
auto s = std::string("hello");  // std::string
```
The compiler deduces the type from the right-hand side. Useful for long type names,
iterators, and lambda return types. Use it when the type is obvious from context.

---

## Example 2 — `const` vs `constexpr`

| | `const` | `constexpr` |
|--|---------|-------------|
| When evaluated | Runtime | Compile time |
| Can be function | ❌ | ✅ (C++11) |
| Value known at compile time | Not necessarily | Always |
| Use for | Runtime constants | Mathematical constants, array sizes, template params |

```cpp
const int maxScore = 100;               // runtime constant
constexpr double PI = 3.14159;          // compile-time constant
constexpr double circleArea(double r) { // compile-time function
    return PI * r * r;
}
constexpr double area = circleArea(5.0); // computed before program runs
```

### Rules
- `const` variables **cannot be reassigned** after initialization
- `constexpr` variables must be initialized with a **compile-time expression**
- Prefer `constexpr` over `#define` for constants — it is type-safe and scoped

---

## Example 3 — Global vs Local Scope

### Global variables
```cpp
int globalCounter = 0;  // declared outside all functions
```
- Accessible from **any function** in the file
- Lifetime = entire program duration
- Use sparingly — global mutable state makes code hard to reason about

### Local variables
```cpp
void foo() {
    int localVar = 10;  // lives only inside foo()
}
// localVar is destroyed here
```

### Block scope
```cpp
{
    int blockVar = 42;  // lives only inside these braces
}
// blockVar is destroyed here — accessing it is a compile error
```

### Shadowing
A local variable can have the same name as a global, hiding (shadowing) it:
```cpp
int counter = 0;        // global

int main() {
    int counter = 999;  // local — shadows the global
    std::cout << counter;    // prints 999 (local)
    std::cout << ::counter;  // prints 0 — :: forces global scope
}
```
The `::` operator (scope resolution) accesses the global when shadowed.

---

## Example 4 — Static Variables

### Static local variable
```cpp
void countCalls() {
    static int callCount = 0;  // initialized ONCE, survives function exit
    ++callCount;
    std::cout << callCount << '\n';
}

countCalls();  // prints 1
countCalls();  // prints 2
countCalls();  // prints 3
```

A `static` local variable behaves like a global in terms of lifetime,
but is **scoped to the function** — nobody outside can access it directly.

### Comparison: static vs normal in a loop
```cpp
for (int i = 0; i < 3; ++i) {
    static int s = 0;   // initialized once — persists across iterations
    int n = 0;          // re-initialized every iteration
    ++s; ++n;
    // i=0: s=1, n=1
    // i=1: s=2, n=1
    // i=2: s=3, n=1
}
```

### When to use `static` local variables
- Counters that track how many times a function was called
- Lazy initialization (initialize expensive resource only once)
- Caching a computed value across calls

---

## Scope summary

| Variable type | Where declared | Lifetime | Accessible from |
|---------------|---------------|----------|----------------|
| Global | Outside all functions | Entire program | Anywhere in file |
| Local | Inside a function | Until function returns | That function only |
| Block | Inside `{}` braces | Until closing `}` | That block only |
| Static local | Inside a function | Entire program | That function only |
| Global `const` | Outside all functions | Entire program | Anywhere in file |
| `constexpr` | Anywhere | Compile time / entire program | Depends on where declared |

---

## Good practices

- Always initialize variables at declaration — never leave them uninitialized
- Use `constexpr` instead of `#define` for constants
- Prefer uniform initialization `{}` over `=` for new code
- Keep variable scope as narrow as possible — declare where first used
- Use `static` locals sparingly and only when persistence across calls is the intent
- Avoid global mutable variables — prefer passing values as function parameters
