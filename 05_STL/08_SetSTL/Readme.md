# SetSTL — std::set in C++23

`std::set` stores **unique, sorted** elements backed by a red-black tree.
All elements are always in ascending order. Duplicates are silently ignored.

- all elements are **unique**, always **sorted** (ascending by default)
- backed by a balanced binary search tree (red-black tree in libstdc++)
- `insert` / `erase` / `find`: O(log n)
- no random access (no `operator[]`)
- elements are **const**: `*s.begin() = 5` does not compile (tested), because changing a key would break the order

---

## Header

```cpp
#include <set>
```

---

## Declaration

```cpp
std::set<int> s;
std::set<int> s2 = {5, 3, 8, 1, 9, 3, 5}; // duplicates removed → {1,3,5,8,9}
std::set<int, std::greater<int>> desc;      // descending order
```

---

## insert, erase

```cpp
auto [it, ok] = s.insert(10);   // ok=true if inserted, false if duplicate
s.insert({20, 30, 40});          // insert multiple
s.emplace(50);                   // construct in place

s.erase(10);                     // erase by value — O(log n), returns the number of removed elements (0 or 1)
s.erase(it);                     // erase by iterator — O(1) amortized, returns the next iterator
s.erase(first, last);            // erase range [first, last)
s.clear();
```

Insert/erase do **not** invalidate iterators to other elements (tested).

### Changing an element

Elements are const, so to change a key you remove and re-insert it. `extract` (C++17) does this without
allocating a new node:

```cpp
auto nh = s.extract(it);     // node handle, element is removed from the set
nh.value() = 20;             // modify the key
s.insert(std::move(nh));     // put it back at the right position (tested)
```

---

## find, contains

```cpp
auto it = s.find(30);
if (it != s.end()) use(*it);     // found

// Strings: find("literal") builds a temporary std::string.
// With a transparent comparator no temporary is created:
std::set<std::string, std::less<>> names;
names.find("alice");             // compares const char* directly (C++14, compiles — tested)

s.count(30)      // 0 or 1 (set = unique)
s.contains(30)   // C++20 — cleaner than count()
```

---

## lower_bound / upper_bound

```cpp
auto lb = s.lower_bound(25);  // first element >= 25 (end() if none — don't dereference blindly)
auto ub = s.upper_bound(25);  // first element > 25

// Elements in range [20, 50):
for (auto it = s.lower_bound(20); it != s.lower_bound(50); ++it)
    use(*it);
```

---

## Custom comparator

```cpp
// Descending
std::set<int, std::greater<int>> desc = {5, 1, 3};  // {5, 3, 1}

// By string length, then alphabetically
auto cmp = [](const std::string& a, const std::string& b) {
    if (a.size() != b.size()) return a.size() < b.size();
    return a < b;                  // tie-break: REQUIRED, see below
};
std::set<std::string, decltype(cmp)> byLen(cmp);
```

**Pitfall:** the comparator defines when two elements are "the same". With only `a.size() < b.size()`
(the version this README used before) `"banana"` and `"cherry"` are equivalent, so the second one is
silently dropped: inserting `banana, cherry, apple, fig, kiwi, date` kept **4 of 6** elements
(`fig kiwi apple banana`, tested). Always add a tie-break unless you really want equivalent elements merged.
The comparator must be a strict weak ordering (use `<`, never `<=`).

---

## std::multiset — allows duplicates

```cpp
#include <set>
std::multiset<int> ms = {3, 1, 3, 5, 3};  // {1, 3, 3, 3, 5}

ms.count(3)     // 3 — counts all occurrences
ms.erase(3)     // removes ALL 3s
ms.erase(ms.find(3))  // removes only ONE 3 (find must not return end())
ms.equal_range(3)     // [first, last) of all 3s
```

---

## std::unordered_set — O(1) average

```cpp
#include <unordered_set>
std::unordered_set<int> us = {3, 1, 4, 1, 5};

us.insert(42);
us.contains(42);   // O(1) average
us.erase(3);       // O(1) average
```

| | `std::set` | `std::unordered_set` |
|--|-----------|---------------------|
| Order | Sorted | None |
| insert/find/erase | O(log n) | O(1) avg |
| Memory | One tree node per element (3 pointers + color + value) | One node per element + bucket array — **not necessarily more** (see below) |
| Sorted iteration | ✅ | ❌ |
| `lower_bound` | ✅ | ❌ |
| Worst case | O(log n) | O(n) (many hash collisions) |
| Key requirement | `operator<` / comparator | `std::hash` + `operator==` |

Measured on GCC 13.3 with 1,000,000 shuffled `int`s (one noisy run, `-O2`):

- Memory (bytes *requested* from `operator new`, not counting allocator overhead): `std::set` 40 B/element;
  `std::unordered_set` 24.5 B/element with `reserve(n)` (38.8 B/element when it grows by rehashing). The old "more memory" claim
  for the hash table is **not true for small keys** like `int`; both are one heap allocation per element.
- Speed: 3,000,000 `contains` calls on 1,000,000 elements: `std::set` ≈ 3150 ms, `std::unordered_set` ≈ 56 ms
  (random keys, cache misses dominate the tree). For small sets the difference is far smaller.
---

## Remove duplicates from vector

```cpp
std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6, 5};
std::set<int> unique(v.begin(), v.end());
// unique = {1, 2, 3, 4, 5, 6, 9}
```

---

## Set operations

```cpp
// Intersection
for (int x : setA) if (setB.contains(x)) intersection.insert(x);

// Union
for (int x : setA) unionSet.insert(x);
for (int x : setB) unionSet.insert(x);

// Difference (A \ B)
for (int x : setA) if (!setB.contains(x)) diff.insert(x);
```

The standard algorithms do the same on any two **sorted** ranges (`#include <algorithm>`, `<iterator>`):

```cpp
std::set<int> out;
std::set_intersection(setA.begin(), setA.end(), setB.begin(), setB.end(),
                      std::inserter(out, out.end()));   // {3,4,5}
std::set_union(...);        // {1..7}
std::set_difference(...);   // {1,2}
```

They are linear in the size of the inputs (tested with `vector` and `set` outputs).

---

## Complexity

| Operation | `std::set` | `std::unordered_set` |
|-----------|-----------|---------------------|
| insert | O(log n) | O(1) avg |
| erase | O(log n) | O(1) avg |
| find | O(log n) | O(1) avg |
| contains | O(log n) | O(1) avg |
| iteration | O(n) sorted | O(n) unsorted |

---

## When to use set

✅ Need unique elements automatically
✅ Need sorted iteration
✅ Need range queries (`lower_bound`, `upper_bound`)
✅ Need to remove duplicates from a collection
✅ Set intersection/union/difference

❌ Need fast O(1) lookup and don't care about order → `unordered_set`
❌ Need random access by index → `vector`
❌ Need duplicates in sorted order → `multiset`

---

## Notes moved from main.cpp / Corrections

- `main.cpp` now uses `std::println`/`std::print` and `std::` everywhere; `<iostream>` is gone, `<functional>` (for `std::greater`) and `<iterator>` were added.
- Removed the lecture-only **"What is a Set"** example (its text is now at the top of this README) and the closing "set vs unordered_set" printout (now the table). The remaining examples are renumbered 1–9 and the "Topics covered" list was updated.
- **Bug in the README comparator:** it compared only `size()` and so silently dropped strings of equal length (4 of 6 kept). `main.cpp` always had the alphabetical tie-break; the README now has it, too, and explains why.
- **Wrong claim fixed:** "unordered_set uses more memory" — for `int` keys on libstdc++ it requested *less* than `std::set` (measurements above).
- The unused `empty` and `copy` sets from Example 1 are now printed.
- Several one-line `for (...) cout ...; cout << '\n';` loops (`-Wmisleading-indentation`) became a small `printAll` helper.

## What was tested

GCC 13.3, `-std=c++23 -Wall -Wextra -Wshadow`, ASan + UBSan: all of `main.cpp` (output identical to the original apart from `true`/`false`, the removed lecture text, the renumbering, the two newly printed sets and the line break before "(order not guaranteed)"); the comparator pitfall; iterator stability; `extract`/modify/insert; transparent `find`; `set_intersection`/`set_union`/`set_difference` into a vector and into a set; `multiset::equal_range`; const elements and missing `operator[]` rejected; the memory and speed measurements above.

## Not tested

The red-black tree claim (a libstdc++/libc++ implementation detail, not guaranteed by the standard), exact complexity of `set_*` algorithms (standard guarantee), results on other standard libraries (node sizes, hash quality), the worst-case O(n) of `unordered_set`.
