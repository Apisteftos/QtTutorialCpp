# VectorSTL — std::vector in C++23

`std::vector` is a **dynamic array** — it grows and shrinks automatically.
It is the most commonly used STL container and should be your default choice.

---

## Header

```cpp
#include <vector>
```

---

## Declaration and initialization

```cpp
std::vector<int> empty;                        // empty
std::vector<int> sized(5);                     // 5 elements, all 0
std::vector<int> filled(5, 42);                // 5 elements, all 42
std::vector<int> list = {10, 20, 30, 40, 50}; // initializer list
std::vector<int> copy = list;                  // copy constructor
std::vector<int> moved = std::move(list);      // move — list is left empty in practice (valid but unspecified)
```

---

## Adding elements

```cpp
v.push_back(10);      // add to end (copies or moves an existing object)
v.emplace_back(10);   // add to end (constructs the element in place from the arguments)
v.insert(v.begin() + 2, 99);           // insert at index 2
v.insert(v.begin(), {1, 2, 3});        // insert multiple
```

`emplace_back` skips the temporary: `push_back(T(1))` costs one constructor
call plus one move, `emplace_back(1)` costs one constructor call (tested).
For `int` and other trivial types there is no difference.
Also note that `emplace_back` on an aggregate such as `Player` (see below)
only compiles with **C++20 or newer** (parenthesized aggregate initialization);
with `-std=c++17` it is a compile error (tested).

---

## Removing elements

```cpp
v.pop_back();                   // remove last element
v.erase(v.begin() + 2);         // remove element at index 2
v.erase(v.begin(), v.begin()+3);// remove range [0, 3)
v.clear();                      // remove all elements
```

---

## Element access

```cpp
v[2]        // no bounds check — undefined behavior if out of range
v.at(2)     // bounds checked — throws std::out_of_range
v.front()   // first element
v.back()    // last element
v.data()    // raw pointer to underlying array
```

---

## Size and capacity

```cpp
v.size()       // number of elements
v.capacity()   // allocated space (>= size)
v.empty()      // true if size == 0
v.max_size()   // theoretical maximum

v.reserve(100)      // pre-allocate — avoids reallocations
v.shrink_to_fit()   // release excess capacity
v.resize(10)        // resize to 10 (new elements are value-initialized: 0 for int)
v.resize(10, -1)    // resize to 10 (new elements = -1)
```

### Why reserve matters

```cpp
// Without reserve — multiple reallocations as vector grows:
std::vector<int> v;
for (int i = 0; i < 1000; ++i) v.push_back(i);   // 11 reallocations (libstdc++ doubles: 1,2,4,...,1024)

// With reserve — single allocation:
std::vector<int> v;
v.reserve(1000);
for (int i = 0; i < 1000; ++i) v.push_back(i);   // 0 reallocations
```

---

## Iterators

```cpp
for (const auto& x : v) { }            // range-based (most common)
for (auto it = v.begin(); it != v.end(); ++it) { }   // iterator
for (auto it = v.rbegin(); it != v.rend(); ++it) { } // reverse
for (auto it = v.cbegin(); it != v.cend(); ++it) { } // const
```

---

## Searching and sorting

```cpp
#include <algorithm>
#include <numeric>

std::sort(v.begin(), v.end());                          // ascending
std::sort(v.begin(), v.end(), std::greater<int>());     // descending
std::sort(v.begin(), v.end(), [](int a, int b) { return a > b; }); // custom

std::find(v.begin(), v.end(), 7)          // linear search — returns iterator
std::binary_search(v.begin(), v.end(), 7) // O(log n) — vector must be sorted
std::lower_bound(v.begin(), v.end(), 5)   // first position >= 5
std::upper_bound(v.begin(), v.end(), 5)   // first position > 5

std::min_element(v.begin(), v.end())      // iterator to minimum
std::max_element(v.begin(), v.end())      // iterator to maximum
std::accumulate(v.begin(), v.end(), 0)    // sum — the init type decides the result type!
```

---

## Erase-remove idiom

```cpp
// Remove all even numbers
v.erase(
    std::remove_if(v.begin(), v.end(), [](int x) { return x % 2 == 0; }),
    v.end()
);
```

`std::remove_if` moves the elements you keep to the front and returns an iterator
to the new logical end. The elements behind that iterator are in a valid but
**unspecified** state (with `{1..10}` and "remove evens" GCC leaves `1 3 5 7 9 | 6 7 8 9 10`,
tested). `erase` then removes them physically.

Since C++20 there is a one-liner that does the same:

```cpp
std::erase_if(v, [](int x) { return x % 2 == 0; });   // returns the number of removed elements
```

---

## Vector of objects

```cpp
struct Player { std::string name; int score; };   // aggregate

std::vector<Player> players;
players.reserve(10);   // pre-allocate before adding objects

players.emplace_back("Kostas", 1500);   // constructs in place
players.emplace_back("Andre",  800);

// Sort by score
std::sort(players.begin(), players.end(),
          [](const Player& a, const Player& b) { return a.score > b.score; });

// Find by name
auto it = std::find_if(players.begin(), players.end(),
                       [](const Player& p) { return p.name == "Kostas"; });
```

---

## 2D vector

```cpp
int rows = 3, cols = 4;
std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols, 0));

matrix[1][2] = 42;
matrix.push_back({1, 2, 3, 4});   // add a row
```

---

## C++20/23 ranges

```cpp
#include <algorithm>

std::ranges::sort(v);                          // cleaner — no begin/end
std::ranges::find(v, 5);
std::ranges::count(v, 1);
std::ranges::contains(v, 5);                   // C++23: bool, no iterator compare

// Projection — sort objects by a field
std::ranges::sort(players, {}, &Player::score);  // sort by score field
```

---

## Complexity

| Operation | Complexity |
|-----------|-----------|
| `push_back` / `emplace_back` | O(1) amortized |
| `insert` at middle | O(n) |
| `erase` at middle | O(n) |
| `operator[]` / `at()` | O(1) |
| `find` (unsorted) | O(n) |
| `binary_search` (sorted) | O(log n) |
| `sort` | O(n log n) |

---

## When to use vector

✅ Default container for sequences
✅ When you need random access by index
✅ When you need cache-friendly iteration
✅ When size changes at the end (push_back)

❌ Frequent insertions/deletions in the middle → use `std::list`
❌ Fixed size known at compile time → use `std::array`
❌ FIFO queue → use `std::deque` or `std::queue`

---

## Notes moved from main.cpp / Corrections

- `main.cpp` now uses `std::println`/`std::print` and `std::` everywhere (no `using namespace std;`).
- Example 10 was titled "C++23 Features" but showed C++20 ranges; it is now "Ranges (C++20) and C++23 additions" and also shows `std::ranges::contains` (C++23).
- Loop variables no longer shadow vectors, and one-line `for (...) ...; ...;` loops got braces/own lines (`-Wmisleading-indentation`).
- Corrected README claims:
  - "~10 reallocations" for 1000 `push_back`s is **11** on libstdc++ (implementation-specific).
  - "always prefer `emplace_back`" was too broad: it only saves a move for types with a non-trivial constructor/move, and for aggregates it needs C++20.
  - After `std::move(v)` the source is "valid but unspecified"; it is empty in all mainstream implementations.
  - The tail after `remove_if` is unspecified, not "the removed elements".
- Gotcha: `std::accumulate(d.begin(), d.end(), 0)` on `std::vector<double>{1.5, 2.5}` returns `3` (int init truncates every step); use `0.0` to get `4` (tested).
- Gotcha: pointers/iterators into a vector are invalidated by `reserve`/`push_back` reallocation; AddressSanitizer reports heap-use-after-free (tested). `clear()` keeps the capacity.

## What was tested

GCC 13.3, `-std=c++23 -Wall -Wextra -Wshadow`, AddressSanitizer + UBSan: all of `main.cpp` (output identical to the original apart from `true`/`false` instead of `1`/`0`, the retitled Example 10 and the new `contains` lines), the capacity sequence 0 → 10 → 20 → 11, copy/move counts of `push_back` vs `emplace_back`, the 11 reallocations, `accumulate` truncation, `remove_if` tail, `erase_if`, aggregate `emplace_back` in C++17 vs C++20, iterator invalidation.

## Not tested

`append_range` / `std::ranges::to` (C++23, need GCC 14+), complexity table and the "when to use" advice (standard guarantees / general guidance), exact capacity growth on MSVC/libc++ (differs: MSVC grows by 1.5x).
