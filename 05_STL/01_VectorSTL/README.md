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
std::vector<int> moved = std::move(list);      // move — list becomes empty
```

---

## Adding elements

```cpp
v.push_back(10);      // add to end (copies)
v.emplace_back(10);   // add to end (constructs in place — preferred for objects)
v.insert(v.begin() + 2, 99);           // insert at index 2
v.insert(v.begin(), {1, 2, 3});        // insert multiple
```

**Always prefer `emplace_back` over `push_back` for non-trivial types** —
it avoids an unnecessary copy or move.

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
v.resize(10)        // resize to 10 (new elements = 0)
v.resize(10, -1)    // resize to 10 (new elements = -1)
```

### Why reserve matters

```cpp
// Without reserve — multiple reallocations as vector grows:
std::vector<int> v;
for (int i = 0; i < 1000; ++i) v.push_back(i);   // ~10 reallocations

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
std::accumulate(v.begin(), v.end(), 0)    // sum
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

`std::remove_if` moves matching elements to the end and returns an iterator
to the new logical end. `erase` then removes them physically.

---

## Vector of objects

```cpp
struct Player { std::string name; int score; };

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
