# Returning Multiple Values — C++23

C++ functions can only return one value directly.
This file covers all modern approaches to return multiple values.

---

## Approach 1 — Output parameters via references

```cpp
void getValues(int& a, char& b, double& c) {
    a = 10; b = 'A'; c = 3.14;
}

int i; char ch; double d;
getValues(i, ch, d);
```

✅ Simple, no extra types
❌ Caller must pre-declare variables, verbose, intent unclear

---

## Approach 2 — std::pair (exactly two values)

```cpp
std::pair<int, double> getValues() {
    return {10, 3.14};   // or std::make_pair(10, 3.14)
}

auto result = getValues();
result.first    // 10
result.second   // 3.14

// C++17 structured binding — cleaner
auto [intVal, doubleVal] = getValues();
```

✅ Simple for two values
❌ `.first` / `.second` not descriptive — use struct for clarity

---

## Approach 3 — std::tuple (three or more values)

```cpp
std::tuple<int, char, double> getValues() {
    return {10, 'A', 3.14};
}

auto result = getValues();
std::get<0>(result)   // 10
std::get<1>(result)   // 'A'
std::get<2>(result)   // 3.14
```

✅ Works for any number of values
❌ `get<0>`, `get<1>` — not readable, error-prone by index

---

## Approach 4 — std::tie (C++11 unpacking)

```cpp
std::tuple<int,int> mathOps(int a, int b) { return {a+b, a*b}; }

int sum, product;
std::tie(sum, product) = mathOps(23, 44);

// Skip a value with std::ignore
std::tie(sum, std::ignore) = mathOps(10, 5);
```

✅ Unpacks into existing variables
❌ Must declare variables beforehand — C++17 binding is cleaner

---

## Approach 5 — Structured bindings C++17 (preferred)

```cpp
auto [sum, product] = mathOps(23, 44);   // declare + unpack in one line

// Works on pair, tuple, and struct
auto [x, y, z] = getPoint3D();
```

✅ Cleanest syntax — descriptive names, no index numbers
✅ Works on `std::pair`, `std::tuple`, and any struct
❌ Requires C++17

---

## Approach 6 — Named struct (best for complex returns)

```cpp
struct Point3D { double x, y, z; };

Point3D getOrigin() { return {1.0, 2.5, -0.5}; }

Point3D p = getOrigin();
p.x   // 1.0 — named field, self-documenting
p.y   // 2.5
```

✅ Self-documenting named fields
✅ Extensible — add methods, constructors
✅ Best practice for complex return types

---

## Approach 7 — Struct with constructor and methods

```cpp
struct Stats {
    int count; double average; int min; int max;
    Stats(int c, double avg, int mn, int mx)
        : count(c), average(avg), min(mn), max(mx) {}
    void print() const { ... }
};

Stats s = computeStats(data, size);
s.print();
s.average;   // named field
```

✅ Most readable, extensible, can add methods
✅ Best choice for complex multi-value returns

---

## Comparison table

| Method | Syntax | Readability | When to use |
|--------|--------|-------------|-------------|
| Output refs | `fn(int& a)` | Low | C legacy |
| `std::pair` | `pair<int,double>` | Medium | Exactly 2 values |
| `std::tuple` | `tuple<int,char>` | Low | Quick, 3+ values |
| `std::tie` | `tie(a,b) = fn()` | Medium | C++11, existing vars |
| Structured binding | `auto [a,b] = fn()` | High | **Modern default** |
| Struct | `struct Result` | Highest | **Best practice** |

---

## Recommendation

```
2 values  → std::pair  + auto [a, b]
3+ values → struct     + auto [a, b, c]
Complex   → struct with named fields and methods
```
