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
std::array<int, 5> a{};                  // zero-initialized
std::array<int, 5> a = {1, 2, 3, 4, 5}; // initializer list
std::array<int, 5> a = {10, 20};         // rest are zero

// C++20: create from C array
int cArr[] = {1, 2, 3};
auto a = std::to_array(cArr);
```

Size is a **compile-time constant** — you cannot change it.

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

Use for lookup tables, sine/cosine tables, fixed protocol field sizes.

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

Note the double `{{ }}` — outer for the `std::array`, inner for the nested arrays.

---

## Comparison operators

`std::array` supports `==`, `!=`, `<`, `<=`, `>`, `>=` — lexicographic:

```cpp
std::array<int,3> a = {1,2,3};
std::array<int,3> b = {1,2,4};
a == b   // false
a <  b   // true (3 < 4)
```

---

## swap

```cpp
std::array<int,5> a = {1,2,3,4,5};
std::array<int,5> b = {9,8,7,6,5};
a.swap(b);   // O(N) — swaps element by element
```

---

## std::array vs C array vs std::vector

| | C array `int[N]` | `std::array<int,N>` | `std::vector<int>` |
|--|-----------------|---------------------|-------------------|
| Size | Compile-time | Compile-time | Runtime |
| Size tracking | ❌ lost when passed | ✅ always known | ✅ always known |
| Resizable | ❌ | ❌ | ✅ |
| Stack allocated | ✅ | ✅ | ❌ (heap) |
| STL algorithms | ❌ (needs pointer+size) | ✅ | ✅ |
| Comparison `==` | ❌ | ✅ | ✅ |
| constexpr | ✅ | ✅ | ❌ |
| Overhead | Zero | Zero | Small |

---

## When to use std::array

✅ Size known at compile time and never changes
✅ Stack allocation preferred (small, fixed data)
✅ constexpr / compile-time tables
✅ Replacing C arrays — all the same speed, none of the risks
✅ Fixed-size protocol fields, hardware register maps

❌ Size not known at compile time → use `std::vector`
❌ Size changes at runtime → use `std::vector`
❌ Large array (> few KB) → prefer heap via `std::vector`
