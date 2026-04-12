# CassertSTL — Assertions in C++23

Assertions verify that conditions that *should always be true* actually are.
They document programmer intent and catch bugs early in development.

---

## Headers

```cpp
#include <cassert>       // assert() macro
#include <type_traits>   // for static_assert with type traits
```

---

## assert() — runtime assertion

```cpp
assert(condition);
assert(condition && "descriptive message");
```

- **Debug build**: if `condition` is false → prints file/line/condition → calls `std::abort()`
- **Release build**: completely removed — zero overhead

```cpp
int x = 42;
assert(x > 0);              // passes silently
assert(x > 0 && "x must be positive");  // with message

// assert(x < 0);  // fails → "Assertion failed: x < 0, file main.cpp, line 10"
```

---

## NDEBUG — disabling assertions

```cpp
#define NDEBUG   // before #include <cassert> — disables all assert()
```

In CMake, Release build sets NDEBUG automatically:
```cmake
set(CMAKE_BUILD_TYPE Release)   // adds -DNDEBUG compiler flag
```

Check in code:
```cpp
#ifdef NDEBUG
    // release mode — assertions disabled
#else
    // debug mode — assertions enabled
#endif
```

---

## static_assert — compile-time assertion

Checked at **compile time** — fails to compile if false. Zero runtime cost.

```cpp
static_assert(sizeof(int) >= 4, "int must be at least 4 bytes");
static_assert(sizeof(void*) == 8, "requires 64-bit platform");

constexpr int N = 64;
static_assert(N > 0 && N <= 256, "N must be in [1, 256]");
```

No message required in C++17+:
```cpp
static_assert(sizeof(MyStruct) == 16);  // message optional
```

---

## static_assert with type traits

```cpp
template<typename T>
T add(T a, T b) {
    static_assert(std::is_arithmetic_v<T>, "T must be numeric");
    static_assert(!std::is_same_v<T, bool>, "bool not allowed");
    return a + b;
}

add(1, 2);       // OK
add(1.5, 2.5);   // OK
// add("a","b"); // COMPILE ERROR: not arithmetic
```

---

## Class invariant pattern

```cpp
class Queue {
    std::vector<int> data;
    size_t maxSize;

    void checkInvariant() const {
        assert(data.size() <= maxSize && "Queue: size exceeded max!");
    }
public:
    void push(int val) {
        assert(!full() && "Queue: push on full queue!");
        data.push_back(val);
        checkInvariant();
    }
};
```

---

## Precondition / postcondition pattern

```cpp
double sqrt(double x) {
    assert(x >= 0.0 && "sqrt: x must be non-negative");  // precondition

    double result = compute(x);

    assert(result >= 0.0 && "sqrt: result must be non-negative");  // postcondition
    return result;
}

int binarySearch(const std::vector<int>& v, int target) {
    assert(std::is_sorted(v.begin(), v.end()) && "must be sorted!");
    // ...
}
```

---

## static_assert with constexpr

```cpp
constexpr int factorial(int n) {
    return (n <= 1) ? 1 : n * factorial(n-1);
}

static_assert(factorial(5) == 120);  // computed at compile time!

// Protocol size guarantees
struct Header { uint8_t a; uint16_t b; uint32_t c; };
static_assert(sizeof(Header) == 7, "Header must be exactly 7 bytes");
// Note: may fail due to padding — use __attribute__((packed))
```

---

## C++23 [[assume]] attribute

```cpp
[[assume(x > 0)]];   // tells optimizer: assume x > 0 — no runtime check!
```

Unlike `assert`, `[[assume]]` **never checks** — it's purely an optimizer hint.
If the assumption is wrong, behavior is undefined.

```
assert(x > 0)     → DEBUG: checks. RELEASE: gone.
[[assume(x > 0)]] → never checks, optimizer hint only.
```

---

## assert vs exception vs if

| | `assert` | `throw` | `if` check |
|--|---------|---------|-----------|
| Runtime cost | 0 in release | Stack unwind | Always present |
| Disabled in release | ✅ | ❌ | ❌ |
| Recoverable | ❌ (aborts) | ✅ | ✅ |
| Use for | Programming bugs | User/IO errors | Expected failures |

---

## When to use assert

✅ Programming errors that should never happen
✅ Function preconditions (caller's responsibility)
✅ Function postconditions (callee's guarantee)
✅ Class invariants
✅ Template type constraints (use `static_assert`)
✅ Protocol field size guarantees (use `static_assert`)

❌ Errors a user can cause (invalid input) → use exceptions
❌ I/O failures → use exceptions or error codes
❌ Conditions that can happen in release → use `if` + proper error handling
❌ Inside destructors → destructors should never throw or abort
