# CassertSTL — Assertions in C++23

Assertions verify that conditions that *should always be true* actually are.
They document programmer intent and catch bugs early in development.

---

## Headers

```cpp
#include <cassert>       // assert() macro
#include <type_traits>   // for static_assert with type traits
#include <cstdint>       // fixed-width types used in the layout examples
#include <cstddef>       // std::size_t
#include <vector>        // class invariant example
#include <algorithm>     // std::is_sorted
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

// assert(x < 0);  // fails → prints (glibc/GCC) and aborts:
//   a.out: main.cpp:10: int main(): Assertion `x < 0' failed.
//   (the exact text depends on the C library; MSVC prints "Assertion failed: x < 0, file ..., line 10")
```

Exit status after the abort is 134 (SIGABRT) on Linux.

---

## NDEBUG — disabling assertions

```cpp
#define NDEBUG   // before #include <cassert> — disables all assert()
```

`<cassert>` is special: it is re-read on every `#include`, so the **last** definition of `NDEBUG`
before an include decides for the asserts that follow. Normally set it on the compiler command line (`-DNDEBUG`)
instead of in source.

In CMake the build type decides (flags as printed by CMake 3.28 with GCC):

| Build type | Flags | assert active? |
|------------|-------|----------------|
| Debug | `-g` | yes |
| Release | `-O3 -DNDEBUG` | no |
| RelWithDebInfo | `-O2 -g -DNDEBUG` | **no** |
| MinSizeRel | `-Os -DNDEBUG` | no |

```cmake
set(CMAKE_BUILD_TYPE Release)   # adds -DNDEBUG   (CMake comments use #, not //)
```

**Never put side effects in an assert.** `assert(++i > 0)` increments `i` in a normal build and
does nothing with `NDEBUG` (tested: `i=1` vs `i=0`).

**Commas inside the condition break the macro** because `assert` is a macro:
`assert(std::pair<int,int>{1,2}.first == 1)` fails to compile ("macro "assert" passed 3 arguments");
wrap the whole condition in extra parentheses: `assert((std::pair<int,int>{1,2}.first == 1))`.

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
    std::size_t maxSize;

    bool full() const { return data.size() >= maxSize; }

    void checkInvariant() const {
        assert(data.size() <= maxSize && "Queue: size exceeded max!");
    }
public:
    explicit Queue(std::size_t max) : maxSize(max) {}

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
double mySqrt(double x) {
    assert(x >= 0.0 && "mySqrt: x must be non-negative");  // precondition

    double result = std::sqrt(x);   // needs <cmath>

    assert(result >= 0.0 && "mySqrt: result must be non-negative");  // postcondition
    return result;
}

int binarySearch(const std::vector<int>& v, int target) {
    assert(std::is_sorted(v.begin(), v.end()) && "must be sorted!");   // needs <algorithm>
    // ...
}
```

The `is_sorted` check is O(n) and turns an O(log n) search into O(n) in Debug builds —
fine for debugging, but be aware of the cost (it vanishes with `NDEBUG`).

---

## static_assert with constexpr

```cpp
constexpr int factorial(int n) {
    return (n <= 1) ? 1 : n * factorial(n-1);
}

static_assert(factorial(5) == 120);  // computed at compile time!

// Binary layout size guarantees
struct Header { std::uint8_t a; std::uint16_t b; std::uint32_t c; };
static_assert(sizeof(Header) == 7, "Header must be exactly 7 bytes");
// FAILS on GCC/x86-64: sizeof(Header) is 8 because of padding after `a`.
// Fix: add __attribute__((packed)) (GCC/Clang) — then sizeof is 7.
```

---

## C++23 [[assume]] attribute

```cpp
[[assume(x > 0)]];   // tells optimizer: assume x > 0 — no runtime check!
```

Unlike `assert`, `[[assume]]` **never checks** — it's purely an optimizer hint.
If the assumption is wrong, behavior is undefined. The expression is not evaluated, so it must
not have side effects you rely on. Compiler support: GCC 13 accepts it (tested); Clang 18
ignores it with a warning (`unknown attribute 'assume' ignored`).

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
✅ Binary layout / field size guarantees (use `static_assert`)

❌ Errors a user can cause (invalid input) → use exceptions
❌ I/O failures → use exceptions or error codes
❌ Conditions that can happen in release → use `if` + proper error handling
❌ Anything that can legitimately happen at runtime → handle it, do not assert it
(An `assert` for a real bug inside a destructor works — tested, it aborts — but a destructor must not *throw*: it is implicitly `noexcept`, so a throw calls `std::terminate`.)

---

## Notes moved from main.cpp

- `main.cpp` uses `std::println` and `std::` everywhere (no `using namespace std;`); `std::cerr` became `std::println(stderr, ...)`.
- **Example 9 ("assert vs exception vs if") was only printed text**, so it was removed from `main.cpp` and the
  old Example 10 is now Example 9. Its content is the table and checklist above.
- Moved here from the printed output: *assert in DEBUG fires, prints and aborts; in RELEASE it is removed (zero overhead)*,
  the CMake / `NDEBUG` instructions (see the build-type table), and the note that the overflow/underflow demos
  (`buf.push` on a full buffer, `buf.pop` on an empty one) are commented out so the program doesn't abort.
- `assert vs [[assume]]`: `assert(x > 0)` checks and aborts in Debug and disappears in Release; `[[assume(x > 0)]]` never checks and only
  tells the optimizer; use it only when you are certain the condition holds.
- Example names were made neutral: port `8080` and a `RecordHeader` struct.
- Output differences vs the original are only from `println`: `full: true` (was `1`), and doubles print their shortest exact text
  (`safeSqrt(2.0) = 1.414213562373095`, `1.1 + 2.2 = 3.3000000000000003`) instead of 6 digits.

## Corrections to main.cpp

- `std::is_sorted` was used without `#include <algorithm>` (it only compiled where another header pulled it in; GCC 13 failed). Added.
- `uint32_t` / `int32_t` are now `std::uint32_t` / `std::int32_t`.
- The postcondition comment said `result * result ≈ x` but the code only checked `result >= 0`. An
  `assert(std::abs(result * result - x) <= 1e-9 * (1.0 + x))` was added. It holds for the demo values and also for
  `1e-300`, `1e-20`, `0.5`, `1e10` — but it **fails for `1e300`**, which is the postcondition doing its job: Newton's method
  started at `x` halves the guess each step and 100 iterations are not enough there.
- Example 4's comment ("cleaner messages with [[assume]]") was wrong; `[[assume]]` has nothing to do with messages.
  It now says the hand-written `verify` stays active in release builds.
- `int x` is only used inside `assert`, so a `-DNDEBUG` build warned `unused variable 'x'`; it is now `[[maybe_unused]]`.

## Corrections to the original README

- The failure message `Assertion failed: x < 0, file main.cpp, line 10` is the MSVC style; glibc prints
  ``main.cpp:10: int main(): Assertion `x < 0' failed.``
- The CMake snippet used `//` as a comment, which is a CMake parse error (use `#`).
- `RelWithDebInfo` and `MinSizeRel` also define `NDEBUG`, not just `Release`.
- `static_assert(sizeof(Header) == 7)` with `uint8_t, uint16_t, uint32_t` *does* fail (size 8), the note said "may".
- Class-invariant and precondition snippets did not compile (`full()`, constructor, `compute`, `size_t`, missing headers) — completed.
- "❌ Inside destructors" was misleading: an `assert` there is fine; throwing is the problem.

## What was tested

GCC 13.3 (`-std=c++23`), Clang 18 for `[[assume]]`, CMake 3.28 for the build-type flags. Every snippet in
this README was compiled; the assert message, abort status 134, `NDEBUG` side-effect behavior, the
comma-in-macro error, the template `static_assert` errors (`T must be numeric`, `bool not allowed`),
the `sizeof(Header)` failure, and the destructor behavior were run.

Also: each commented-out failing demo in `main.cpp` (push on a full buffer, pop on an empty one, `safeSqrt(-1.0)`, unsorted
`binarySearch`, `BoundedBuffer(0)`) was enabled in a test copy and aborted with status 134; the `-DNDEBUG -O2 -Wall -Wextra` build is warning-free.

## Not tested

- MSVC output text; other C libraries' assert messages.
- Behavior of `[[assume]]` on optimization (only that it compiles and runs on GCC 13).
- Real `<print>` from GCC 14+ / libc++ (a small test-only stand-in header was used with GCC 13.3).
