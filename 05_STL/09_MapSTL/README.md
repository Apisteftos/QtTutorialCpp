# MapSTL — std::map in C++23

`std::map` stores **unique key-value pairs**, always **sorted by key**.
Backed by a red-black tree — all operations are O(log n).

---

## Header

```cpp
#include <map>
```

---

## Declaration

```cpp
std::map<std::string, int> m;
std::map<std::string, int> m = {{"Alice", 95}, {"Bob", 82}};
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

---

## Accessing elements

```cpp
m["key"]        // creates if missing (use carefully!)
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
m.erase(first, last);        // range erase
m.clear();
```

---

## Iteration — always sorted by key

```cpp
for (const auto& [key, value] : m)
    std::cout << key << " = " << value << '\n';

for (auto it = m.rbegin(); it != m.rend(); ++it)
    std::cout << it->first << " = " << it->second << '\n';
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
void printScore(std::map<std::string,int>& m, const std::string& name) {
    std::cout << m[name];   // creates name=0 if not found!
}

// SAFE: use find or at
void printScore(std::map<std::string,int>& m, const std::string& name) {
    auto it = m.find(name);
    if (it != m.end()) std::cout << it->second;
}
```

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
| Memory | Less | More |

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
