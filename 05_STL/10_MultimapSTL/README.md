# MultimapSTL — std::multimap in C++23

`std::multimap` is like `std::map` but allows **duplicate keys**.
Multiple values can be associated with the same key, all stored in sorted key order.

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
std::multimap<std::string, int> mm;
std::multimap<std::string, int> mm = {
    {"Alice", 95}, {"Alice", 88}, {"Bob", 82}
};
```

---

## Insert — always succeeds

```cpp
mm.insert({"key", value});    // always inserts — no uniqueness check
mm.emplace("key", value);     // construct in place

// NO operator[] — would be ambiguous with multiple values!
// mm["Alice"]   // ERROR — does not compile
```

---

## Accessing all values for a key

```cpp
// equal_range — returns [first, last) for the key
auto [lo, hi] = mm.equal_range("Alice");
for (auto it = lo; it != hi; ++it)
    std::cout << it->second << '\n';

// lower_bound + upper_bound (equivalent)
auto lo = mm.lower_bound("Alice");
auto hi = mm.upper_bound("Alice");
```

---

## find, count, contains

```cpp
auto it = mm.find("Alice");   // iterator to FIRST "Alice"
mm.count("Alice")             // number of entries with key "Alice"
mm.contains("Alice")          // C++20 — true if any entry with key
```

---

## Erase

```cpp
mm.erase("Alice");         // removes ALL entries with key "Alice"
mm.erase(mm.find("Alice")); // removes only FIRST "Alice" (by iterator)
```

---

## Group by key pattern

```cpp
std::multimap<std::string, std::string> departments = {
    {"Eng", "Kostas"}, {"Eng", "Andre"}, {"HR", "Anna"}
};

std::string last;
for (const auto& [dept, name] : departments) {
    if (dept != last) { std::cout << dept << ":\n"; last = dept; }
    std::cout << "  - " << name << '\n';
}
```

---

## Average per key

```cpp
auto [lo, hi] = mm.equal_range("Math");
int sum = 0, count = 0;
for (auto it = lo; it != hi; ++it) { sum += it->second; ++count; }
double avg = (double)sum / count;
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

---

## When to use multimap

✅ Multiple values per key in sorted key order
✅ Event logs grouped by timestamp
✅ Students grouped by subject/class
✅ Employees grouped by department
✅ Index structures (one key maps to many records)

❌ One value per key → use `map`
❌ Don't need sorted keys → use `unordered_multimap`
❌ Just need to group and sort once → `vector<pair>` + `sort`
