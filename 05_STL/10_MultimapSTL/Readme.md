# MultimapSTL — std::multimap in C++23

`std::multimap` is like `std::map` but allows **duplicate keys**.
Multiple values can be associated with the same key, all stored in sorted key order.

- key-value pairs like `std::map`, **duplicate keys allowed**, always **sorted by key**
- **no `operator[]` and no `at()`** (several values per key would be ambiguous) — both are compile errors (tested)
- `insert` / `find` / `erase`: O(log n)

---

## Header

```cpp
#include <map>   // same header as std::map
```

---

## Key difference from map

```cpp
std::map<std::string, int> m;
m["Alice"] = 95;
m["Alice"] = 88;   // overwrites! Alice = 88

std::multimap<std::string, int> mm;
mm.insert({"Alice", 95});
mm.insert({"Alice", 88});   // both stored! Alice → 95, Alice → 88
```

---

## Declaration

```cpp
std::multimap<std::string, int> empty;
std::multimap<std::string, int> mm = {
    {"Alice", 95}, {"Alice", 88}, {"Bob", 82}
};
```

---

## Insert — always succeeds

```cpp
mm.insert({"key", value});    // always inserts — no uniqueness check
mm.emplace("key", value);     // construct in place

// Entries with EQUAL keys keep their insertion order (a plain insert goes behind the existing
// equal keys — tested). Careful: with a position HINT this is not guaranteed; in libstdc++
// mm.insert(mm.begin(), {"k", 100}) put the new entry in FRONT of the existing "k" entries (tested).

// NO operator[] — would be ambiguous with multiple values!
// mm["Alice"]   // ERROR — does not compile
```

---

## Accessing all values for a key

```cpp
// equal_range — returns [first, last) for the key
auto [lo, hi] = mm.equal_range("Alice");
for (auto it = lo; it != hi; ++it)
    std::println("{}", it->second);

// lower_bound + upper_bound (equivalent, tested)
auto lo2 = mm.lower_bound("Alice");
auto hi2 = mm.upper_bound("Alice");

// All keys in an interval [150, 350]:
auto from = log.lower_bound(150);   // first key >= 150
auto to   = log.upper_bound(350);   // first key >  350
```

---

## find, count, contains

```cpp
auto it = mm.find("Alice");   // iterator to FIRST "Alice"
mm.count("Alice")             // number of entries with key "Alice" — O(log n + k), k = number of matches
mm.contains("Alice")          // C++20 — true if any entry with key
```

---

## Erase

```cpp
mm.erase("Alice");         // removes ALL entries with key "Alice"
mm.erase(mm.find("Alice")); // removes only FIRST "Alice" (by iterator)
```

`mm.erase(mm.find(key))` is **undefined behavior if the key is missing** (`find` returns `end()`; with
`-D_GLIBCXX_ASSERTIONS` it aborts, tested) — check the iterator first.

More ways to erase (all tested):

```cpp
auto [lo, hi] = mm.equal_range("Alice");
mm.erase(lo, hi);                                  // same as erase("Alice")
std::erase_if(mm, [](auto& kv) { return kv.second < 50; });   // C++20
for (auto it = mm.begin(); it != mm.end(); )       // erase while looping
    it = (it->second % 2 == 0) ? mm.erase(it) : std::next(it);
```

---

## Group by key pattern

```cpp
std::multimap<std::string, std::string> departments = {
    {"Eng", "Kostas"}, {"Eng", "Andre"}, {"HR", "Anna"}
};

std::string last;
for (const auto& [dept, name] : departments) {
    if (dept != last) { std::println("{}:", dept); last = dept; }
    std::println("  - {}", name);
}
```

---

## Average per key

```cpp
auto [lo, hi] = mm.equal_range("Math");
int sum = 0, count = 0;
for (auto it = lo; it != hi; ++it) { sum += it->second; ++count; }
double avg = count > 0 ? static_cast<double>(sum) / count : 0.0;   // count == 0 if the key is missing
```

---

## multimap vs map vs vector<pair>

| | `map` | `multimap` | `vector<pair>` |
|--|-------|-----------|---------------|
| Duplicate keys | ❌ | ✅ | ✅ |
| Sorted | ✅ | ✅ | ❌ (manual) |
| `operator[]` | ✅ | ❌ | ❌ |
| find by key | O(log n) | O(log n) | O(n) |
| All values for key | N/A | `equal_range` | manual loop |
| Insertion order of equal keys | N/A | kept | kept (if stable sort) |

`std::unordered_multimap` also allows duplicate keys with O(1) average lookup but no ordering. Equal keys are
neighbours there, but **not in insertion order**: inserting `x:0,1,2,3` came back as `0 3 2 1` (libstdc++, tested).
Use it when speed matters and sorted order does not.
---

## When to use multimap

✅ Multiple values per key in sorted key order
✅ Event logs grouped by timestamp
✅ Students grouped by subject/class
✅ Employees grouped by department
✅ Index structures (one key maps to many records)
✅ Range queries on keys (`lower_bound` / `upper_bound`), as in the timeline example

❌ One value per key → use `map`
❌ Don't need sorted keys → use `unordered_multimap`
❌ Just need to group and sort once → `vector<pair>` + `std::stable_sort` (keeps insertion order among equal keys)
❌ You need all values for a key as one container → `map<Key, vector<Value>>`

---

## Notes moved from main.cpp / Corrections

- `main.cpp` now uses `std::println`/`std::print` and `std::` everywhere; `<iostream>` is gone. A small `printMap` helper replaced the `for (...) cout ...; cout << '\n';` one-liners.
- Removed the two lecture-only examples **"What is a Multimap"** (now the intro of this README) and **"multimap vs map vs vector<pair>"** (now the table and the notes above). Examples are renumbered 1–8 and the "Topics covered" list was corrected: it listed an "Iterators" example that did not exist and shifted all numbers.
- The event-log example used message names from a specific protocol domain; it is now neutral (`FILE_OPENED`, `READ_STARTED`, …). The `log` variable in the bound example was renamed `timeline` (it hid the C library function `log`).
- Example "lower_bound, upper_bound": the label said "events in (150, 350]" but the range with `lower_bound(150)` / `upper_bound(350)` is `[150, 350]` (closed on both ends); label and comments fixed.
- `(double)sum/cnt` became `static_cast<double>`; `sum`/`cnt` are declared on separate lines.
- README: the `lo`/`hi` variables were declared twice in one snippet (renamed), the average snippet now guards against `count == 0`, and the `std::cout` snippets use `std::println`.

## What was tested

GCC 13.3, `-std=c++23 -Wall -Wextra -Wshadow`, ASan + UBSan: all of `main.cpp` (output identical to the original apart from `true`/`false`, the removed lecture text, the neutral event names and the corrected interval label); insertion order of equal keys including the hinted `insert`; `equal_range` equals `lower_bound`/`upper_bound`; `count`; `erase(lo, hi)`, `erase_if`, erase-in-loop, `extract`; `operator[]` and `at()` rejected; `erase(find(missing))` aborts with `-D_GLIBCXX_ASSERTIONS`; `unordered_multimap` order of equal keys.

## Not tested

Behaviour of hinted `insert` on other standard libraries (the standard only says "as close as possible to the position just prior to hint"), the red-black-tree implementation detail, `vector<pair>` columns of the table (general knowledge, not run).
