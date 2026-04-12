# SetSTL — std::set in C++23

`std::set` stores **unique, sorted** elements backed by a red-black tree.
All elements are always in ascending order. Duplicates are silently ignored.

---

## Header

```cpp
#include <set>
```

---

## Declaration

```cpp
std::set<int> s;
std::set<int> s = {5, 3, 8, 1, 9, 3, 5};  // duplicates removed → {1,3,5,8,9}
std::set<int, std::greater<int>> desc;      // descending order
```

---

## insert, erase

```cpp
auto [it, ok] = s.insert(10);   // ok=true if inserted, false if duplicate
s.insert({20, 30, 40});          // insert multiple
s.emplace(50);                   // construct in place

s.erase(10);                     // erase by value — O(log n)
s.erase(it);                     // erase by iterator — O(1) amortized
s.erase(first, last);            // erase range
s.clear();
```

---

## find, contains

```cpp
auto it = s.find(30);
if (it != s.end()) use(*it);     // found

s.count(30)      // 0 or 1 (set = unique)
s.contains(30)   // C++20 — cleaner than count()
```

---

## lower_bound / upper_bound

```cpp
auto lb = s.lower_bound(25);  // first element >= 25
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

// By string length
auto cmp = [](const std::string& a, const std::string& b) {
    return a.size() < b.size();
};
std::set<std::string, decltype(cmp)> byLen(cmp);
```

---

## std::multiset — allows duplicates

```cpp
#include <set>
std::multiset<int> ms = {3, 1, 3, 5, 3};  // {1, 3, 3, 3, 5}

ms.count(3)     // 3 — counts all occurrences
ms.erase(3)     // removes ALL 3s
ms.erase(ms.find(3))  // removes only ONE 3
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
| Memory | Less | More (hash table) |
| Sorted iteration | ✅ | ❌ |
| `lower_bound` | ✅ | ❌ |

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
