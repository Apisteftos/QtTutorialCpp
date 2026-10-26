# MapSTL — std::map in C++23

`std::map` stores **unique key-value pairs**, always **sorted by key**.
Backed by a red-black tree (libstdc++/libc++; the standard only requires the O(log n) bounds) —
`insert` / `find` / `erase` are O(log n).

- stores **key-value pairs**, keys are **unique**
- always **sorted by key** (ascending by default)
- elements are `std::pair<const Key, Value>`: the value can be changed through an iterator, the **key cannot**
  (`it->first = 5` does not compile, tested)

---

## Header

```cpp
#include <map>
```

---

## Declaration

```cpp
std::map<std::string, int> m;
std::map<std::string, int> scores = {{"Alice", 95}, {"Bob", 82}};
std::map<int, std::string, std::greater<int>> desc;  // descending keys
```

---

## Inserting elements

```cpp
// operator[] — creates with default value if key missing!
m["key"] = 42;
m["new_key"];   // WARNING: creates entry with value 0

// insert — safe, does NOT overwrite existing key
auto [it, ok] = m.insert({"key", 99});  // ok=false if key exists

// insert_or_assign — always writes (C++17)
m.insert_or_assign("key", 99);

// try_emplace — only inserts if key not present (C++17)
m.try_emplace("key", 99);   // no-op if key exists
m.try_emplace("new", 42);   // inserts if key missing
```

**`try_emplace` vs `emplace`:** if the key already exists, `try_emplace` leaves its arguments untouched, while
`emplace` may already have consumed them. Tested with a `std::map<std::string, std::unique_ptr<int>>`:
`m.emplace("a", std::move(p))` on an existing key left `p` **empty** (the new object was destroyed),
`m.try_emplace("a", std::move(q))` left `q` **still owning** its object. Prefer `try_emplace` for move-only or expensive values.

---

## Accessing elements

```cpp
m["key"]        // creates if missing (use carefully!); returns a reference to the value
                // needs a default-constructible value type and a non-const map (both tested: compile errors otherwise)
m.at("key")     // throws std::out_of_range if missing — safe read
m.find("key")   // returns iterator or end()
m.contains("k") // C++20 — true/false
m.count("key")  // 0 or 1 for map
```

---

## Erase

```cpp
m.erase("key");              // by key — O(log n)
m.erase(it);                 // by iterator — O(1) amortized
m.erase(first, last);        // range erase [first, last)
m.clear();
std::erase_if(m, pred);      // C++20: erase all pairs matching a predicate
```

Erasing while looping: `erase(it)` returns the next iterator — use it, never `++it` on an erased one:

```cpp
for (auto it = m.begin(); it != m.end(); ) {
    if (it->second == 0) it = m.erase(it);   // tested
    else                 ++it;
}
```

Insert/erase do not invalidate iterators to other elements (tested). To change a **key**, use `extract`:
`auto nh = m.extract(key); nh.key() = newKey; m.insert(std::move(nh));` (C++17, tested).

---

## Iteration — always sorted by key

```cpp
for (const auto& [key, value] : m)
    std::println("{} = {}", key, value);

for (auto it = m.rbegin(); it != m.rend(); ++it)
    std::println("{} = {}", it->first, it->second);
```

---

## lower_bound / upper_bound

```cpp
auto lb = m.lower_bound(key);  // first key >= key
auto ub = m.upper_bound(key);  // first key > key

// Iterate range [20, 50)
for (auto it = m.lower_bound(20); it != m.lower_bound(50); ++it)
    use(it->first, it->second);
```

---

## operator[] pitfall

```cpp
// DANGER: accidentally creates entries
void printScoreBad(std::map<std::string,int>& m, const std::string& name) {
    std::println("{}", m[name]);   // creates name=0 if not found! (size grows, tested)
}

// SAFE: use find (or at, which throws for a missing key)
void printScore(const std::map<std::string,int>& m, const std::string& name) {
    auto it = m.find(name);
    if (it != m.end()) std::println("{}", it->second);
}
```

---

## Custom key comparator

```cpp
// Case-insensitive keys
auto lower = [](std::string s) {
    for (auto& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));  // <cctype>
    return s;
};
auto ci = [&lower](const std::string& a, const std::string& b) { return lower(a) < lower(b); };

std::map<std::string, int, decltype(ci)> ciMap(ci);
ciMap["Alice"] = 1;
ciMap["ALICE"] = 10;   // same key as "Alice": overwrites the value, the stored key stays "Alice"
```

The comparator defines when two keys are "the same", and it must be a strict weak ordering.
`std::tolower` must receive an `unsigned char` value; a plain negative `char` is undefined behavior.
For descending order use `std::map<int, std::string, std::greater<int>>` (tested).

---

## std::unordered_map — O(1) average

```cpp
#include <unordered_map>
std::unordered_map<std::string, int> um;

um["key"] = 42;
um.insert({"key2", 43});
um.contains("key");   // O(1) average
um.erase("key");
```

| | `std::map` | `std::unordered_map` |
|--|-----------|---------------------|
| Order | Sorted by key | None |
| insert/find | O(log n) | O(1) avg |
| Sorted iteration | ✅ | ❌ |
| `lower_bound` | ✅ | ❌ |
| Worst case | O(log n) | O(n) (many hash collisions) |
| Key requirement | `operator<` / comparator | `std::hash` + `operator==` |

Measured on GCC 13.3 with 1,000,000 shuffled `int` → `int` pairs (one noisy run, `-O2`):

- Memory (bytes *requested* from `operator new`, one allocation per element for both):
  `std::map` 40 B/element, `std::unordered_map` 24.5 B/element with `reserve(n)`.
  So the old claim "unordered_map uses more memory" is **not true for small keys/values** on libstdc++.
- Speed: 3,000,000 lookups: `std::map` ≈ 3075 ms, `std::unordered_map` ≈ 42 ms (random keys, tree traversal causes cache misses).
  For small maps the difference is much smaller.

---

## Word frequency pattern

```cpp
std::map<std::string, int> freq;
for (const std::string& word : words)
    freq[word]++;   // operator[] creates with 0, then increments

for (const auto& [word, count] : freq)
    std::println("{}: {}", word, count);
```

---

## Safe config access with default

```cpp
auto get = [&](const std::string& key, const std::string& def) {
    auto it = config.find(key);
    return (it != config.end()) ? it->second : def;
};

std::string host = get("server.host", "localhost");
```

---

## When to use map

✅ Key-value store with sorted keys
✅ Need range queries on keys (`lower_bound`)
✅ Word counting, frequency tables
✅ Config/settings storage
✅ Phone books, dictionaries

❌ Don't need sorted keys and want speed → `unordered_map`
❌ Need duplicate keys → `multimap`
❌ Simple sequence → `vector`

---

## Notes moved from main.cpp / Corrections

- `main.cpp` now uses `std::println`/`std::print` and `std::` everywhere; `<iostream>` is gone, `<cctype>` and `<stdexcept>` were added. A small `printMap` helper replaced the `for (...) cout ...; cout << '\n';` one-liners.
- Removed the lecture-only **"What is a Map"** example (now at the top of this README); examples renumbered 1–9, "Topics covered" updated (it listed a "phone book, config store" example 10; the phone book is Example 3, the config store Example 9).
- The config example used keys and values from a specific protocol domain; it now uses neutral keys (`db.name`, `db.timeout`, port 8080, default protocol `HTTP`).
- Custom comparator example: `std::tolower` got a plain `char` (undefined for negative values) and `<cctype>` was missing; the lambda parameter `lb` shadowed the earlier iterator `lb` (`-Wshadow`). It now lowercases via a helper with `unsigned char`, and an extra line `ciMap["ALICE"] = 10` shows that equivalent keys overwrite.
- Removed an unused structured binding (`it1`/`ok1`) in Example 2.
- **Wrong claim fixed:** "unordered_map uses more memory" (measurement above).
- README snippets used `std::cout`; they now use `std::println`. The `printScore` pitfall sketch had two functions with the same signature; the bad one is now `printScoreBad`.

## What was tested

GCC 13.3, `-std=c++23 -Wall -Wextra -Wshadow`, ASan + UBSan: all of `main.cpp` (output identical to the original apart from `true`/`false`, the removed lecture text, the renumbering, the neutral config values and the `ALICE` overwrite); `emplace` vs `try_emplace` on an existing key with `unique_ptr`; `it = m.erase(it)` loop, `std::erase_if`, `extract` with a key change, iterator stability; `operator[]` on a const map / non-default-constructible value / writing to `->first` rejected; descending map; memory and lookup timing above.

## Not tested

The red-black-tree detail (libstdc++/libc++ implementation), the O(n) worst case of `unordered_map`, whether `emplace` consumes its arguments on other standard libraries (the standard allows it; libstdc++ did), behaviour with non-ASCII text in the case-insensitive example.
