# ArraySTL — std::array in C++23

`std::array` is a **fixed-size array** whose size is known at compile time.
It is a zero-overhead wrapper around a C array with full STL interface.

---

## Header

```cpp
#include <array>
```

---

## Declaration

```cpp
std::array<int, 5> a1{};                  // zero-initialized
std::array<int, 5> a2 = {1, 2, 3, 4, 5}; // initializer list
std::array<int, 5> a3 = {10, 20};         // rest are zero

// C++20: create from C array (copies the elements)
int cArr[] = {1, 2, 3};
auto a4 = std::to_array(cArr);            // std::array<int, 3>
```

Size is a **compile-time constant** — you cannot change it. `std::array<int, n>` with a non-constant `n` does not compile (tested).

---

## Element access

```cpp
a[2]        // no bounds check
a.at(2)     // bounds checked — throws std::out_of_range
a.front()   // first element
a.back()    // last element
a.data()    // raw pointer to underlying C array
```

---

## Size

```cpp
a.size()       // always the template parameter N
a.max_size()   // same as size()
a.empty()      // true only if N == 0
```

Size is a **compile-time constant**:
```cpp
constexpr std::size_t N = a.size();   // known at compile time
```

---

## constexpr — compile-time computation

```cpp
constexpr std::array<int, 5> primes = {2, 3, 5, 7, 11};
constexpr int first = primes[0];      // computed at compile time
constexpr int count = primes.size();  // compile-time constant
```

Use for lookup tables and sine/cosine tables.

---

## Algorithms

All `std::algorithm` functions work with `std::array`:

```cpp
std::sort(a.begin(), a.end());
std::find(a.begin(), a.end(), 5);
std::fill(a.begin(), a.end(), 0);
std::copy(src.begin(), src.end(), dst.begin());
std::accumulate(a.begin(), a.end(), 0);
std::reverse(a.begin(), a.end());
```

---

## Multi-dimensional

```cpp
std::array<std::array<int, 3>, 3> matrix = {{
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
}};

int val = matrix[1][2];   // 6
```

Both braces layers matter here: `{{ {1,2,3}, {4,5,6} }}` and the fully spelled
`{{ {{1,2,3}}, {{4,5,6}} }}` compile, but a single outer brace
`= { {1,2,3}, {4,5,6} }` is a compile error ("too many initializers", tested).
Reason: `std::array` contains an inner C array member, so the outer `{}` is the
`std::array` and the second `{}` is that member; the rows `{1,2,3}` rely on brace elision.

---

## Comparison operators

`std::array` supports `==`, `!=`, `<`, `<=`, `>`, `>=` — lexicographic:

```cpp
std::array<int,3> a = {1,2,3};
std::array<int,3> b = {1,2,4};
a == b   // false
a <  b   // true (3 < 4)
(a <=> b) < 0   // C++20: three-way comparison also works
```

---

## swap

```cpp
std::array<int,5> a = {1,2,3,4,5};
std::array<int,5> b = {9,8,7,6,5};
a.swap(b);   // O(N) — swaps element by element (unlike std::vector::swap, which swaps pointers in O(1))
```

---

## std::array vs C array vs std::vector

| | C array `int[N]` | `std::array<int,N>` | `std::vector<int>` |
|--|-----------------|---------------------|-------------------|
| Size | Compile-time | Compile-time | Runtime |
| Size tracking | ❌ lost when passed | ✅ always known | ✅ always known |
| Resizable | ❌ | ❌ | ✅ |
| Stack allocated | ✅ | ✅ | ❌ (heap) |
| STL algorithms | ✅ via `std::begin/end` (but only where the array is still an array) | ✅ | ✅ |
| Comparison `==` | ⚠️ compiles but compares addresses (warning `-Warray-compare`) | ✅ compares elements | ✅ compares elements |
| constexpr variable | ✅ | ✅ | ❌ (usable inside a `constexpr` function since C++20, but not as a `constexpr` object) |
| Overhead | Zero | Zero (same `sizeof` as the C array; `array<T,0>` is 1 byte on GCC) | Small (pointer triple + heap) |

---

## When to use std::array

✅ Size known at compile time and never changes
✅ Stack allocation preferred (small, fixed data)
✅ constexpr / compile-time tables
✅ Replacing C arrays — all the same speed, none of the risks
✅ Fixed-size buffers and small lookup tables

❌ Size not known at compile time → use `std::vector`
❌ Size changes at runtime → use `std::vector`
❌ Large array → the elements live inside the object, so on the stack: a 16 MB `std::array<char, …>` crashed with a segmentation fault on Linux with the default 8 MB stack (tested); prefer `std::vector` (the limit is platform-specific; Windows defaults to 1 MB)

---

## Notes moved from main.cpp / Corrections

- `main.cpp` now uses `std::println`/`std::print` and `std::` everywhere; `<iostream>` is gone and the includes the code really needs were added.
- Removed the unused variable `std::array<int, 3> strings` (it was an `int` array with a misleading name).
- The "C array: STL algorithms ❌" claim in the table was wrong: `std::sort(std::begin(c), std::end(c))` and `std::ranges::sort(c)` work on a real C array (tested). What a C array loses is its size once it decays to a pointer.
- "C array `==` ❌" is only half true: it compiles and compares addresses, which is almost never what you want (`-Warray-compare`, deprecated in C++20).
- `std::vector` is not "❌ constexpr" in C++20 any more in every sense, see the table.
- The brace explanation for nested arrays was imprecise, see "Multi-dimensional".
- The Declaration snippet redeclared `a` three times; the variables now have distinct names.
- "Large array > few KB" was a rule of thumb without a number; the 16 MB stack overflow is the tested part, the "few KB" threshold is not.
- Example 8 uses `std::size_t` loop counters, Example 6 `auto` for the `count_if` result, and the Example 7 lambda/loop names no longer shadow other variables (`-Wshadow` clean).

## What was tested

GCC 13.3, `-std=c++23 -Wall -Wextra -Wshadow`, ASan + UBSan: all of `main.cpp` (output identical to the original except `true`/`false` instead of `1`/`0` and one extra blank line at the very top from the banner helper); `sizeof` of `std::array`; `std::array<int,n>` with non-constant `n` rejected; C array `==` behaviour in C++17/20/23; algorithms on C arrays; constexpr `std::vector` inside a `constexpr` function; the brace forms for nested arrays; `<=>`; the 16 MB stack crash.

## Not tested

Behaviour on MSVC/Clang (e.g. `sizeof(std::array<T,0>)`, stack limits), the O(N) swap complexity and "zero overhead" in optimized machine code (standard guarantees / not inspected).
