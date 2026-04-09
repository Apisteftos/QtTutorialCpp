# References in C++23

A reference is an **alias** for an existing variable — another name for the same
memory location. Unlike a pointer, a reference cannot be null, cannot be reseated
(made to refer to something else), and does not need to be dereferenced.

---

## Declaration

```cpp
int  original = 42;
int& ref      = original;   // ref is an alias for original
```

| | Pointer | Reference |
|--|---------|-----------|
| Syntax | `int* p = &x` | `int& r = x` |
| Can be null | ✅ | ❌ — always refers to something |
| Can be reseated | ✅ | ❌ — fixed at initialization |
| Needs dereference | ✅ `*p` | ❌ — use directly |
| Must initialize | ❌ | ✅ — always |
| Arithmetic | ✅ | ❌ |

---

## Example 1 — Basic reference

```cpp
int original = 42;
int& ref     = original;   // same memory location

ref = 100;
std::cout << original;   // 100 — modified through ref

original = 999;
std::cout << ref;        // 999 — ref sees the change
```

`&original == &ref` — they are **the same address**.

---

## Example 2 — Reference rules

### Cannot be left uninitialized
```cpp
int& ref;        // ERROR — must initialize at declaration
int& ref = x;   // OK
```

### Cannot be reseated
```cpp
int a = 10, b = 20;
int& ref = a;   // ref refers to a

ref = b;        // does NOT make ref point to b
                // ASSIGNS b's value (20) to a
                // ref still refers to a
```

This is the most common beginner mistake with references.
After `ref = b`, both `a` and `ref` equal 20, but `ref` still refers to `a`.

---

## Example 3 — Pass by value vs reference

### Pass by value — receives a copy
```cpp
void levelUp(Player player) {   // copy made here
    player.level++;             // modifies copy only
}

levelUp(kostas);
// kostas.level is UNCHANGED
```

### Pass by reference — receives the original
```cpp
void levelUp(Player& player) {  // no copy — same object
    player.level++;             // modifies the original
}

levelUp(kostas);
// kostas.level IS changed
```

### Pass by const reference — read-only, no copy
```cpp
void print(const Player& player) {  // no copy, no modification
    std::cout << player.name;
    // player.level++;   // ERROR — const
}
```

**Use const reference for any parameter you only read and don't need to copy.**
This is especially important for strings, vectors, structs, and classes.

---

## Example 4 — Performance: const reference vs value

```cpp
struct LargeData { std::string data[100]; };

void byValue(LargeData ld)         { ... }   // copies 100 strings — EXPENSIVE
void byConstRef(const LargeData& ld) { ... } // zero copy — CHEAP
```

Rule of thumb for function parameters:

| Type | Recommended parameter style |
|------|----------------------------|
| `int`, `float`, `bool`, `char` | by value |
| `std::string` (read only) | `const std::string&` |
| `std::vector` (read only) | `const std::vector<T>&` |
| Any struct/class (read only) | `const T&` |
| Any struct/class (modify) | `T&` |
| Smart pointers | by value (they are cheap to copy) |

---

## Example 5 — Reference to struct member

```cpp
Player andre{"Andre", 800, 8};
int& scoreRef = andre.score;   // direct alias to the member

scoreRef += 500;   // same as andre.score += 500
```

Useful when you access the same member many times in a function —
avoids repeated `andre.score` lookups and makes intent clear.

---

## Example 6 — Swap with references

The classic reference example:

```cpp
void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

swap(x, y);   // x and y are modified directly
```

Without references this is impossible without pointers.
`std::swap` in `<algorithm>` works the same way.

---

## Example 7 — Return by reference

A function can return a reference to an existing object:

```cpp
std::string& getFirst(std::vector<std::string>& v) {
    return v[0];   // reference to element inside vector
}

getFirst(names) = "Konstantinos";   // modifies names[0] directly
```

### NEVER return a reference to a local variable
```cpp
int& badFunction() {
    int local = 42;
    return local;   // UNDEFINED BEHAVIOR — local is destroyed when function returns
}
```

Only return references to:
- Parameters passed by reference
- Members of objects that outlive the function
- Static variables
- Elements of containers passed in

---

## Example 8 — Range-based for loop

```cpp
std::vector<int> scores = {10, 20, 30};

for (int s : scores)    // copy — original unchanged
    s *= 2;

for (int& s : scores)   // reference — modifies original
    s *= 2;

for (const int& s : scores)   // const reference — read only, no copy
    std::cout << s;
```

**Always use `const auto&` in range-based for when you only read:**
```cpp
for (const auto& player : players)   // works for any type, zero copy
    printPlayer(player);
```

---

## Example 9 — lvalue vs rvalue references (C++11)

### lvalue reference (`&`)
Binds to a **named variable** (something with an address):
```cpp
int x = 5;
int& lref = x;     // OK — x is an lvalue
int& bad  = 5;     // ERROR — 5 is an rvalue (no address)
```

### rvalue reference (`&&`)
Binds to a **temporary** (something without a persistent address):
```cpp
int&& rref = 10;       // OK — 10 is a temporary
int&& rref = x * 2;    // OK — expression result is a temporary
```

### const lvalue reference — special rule
Can bind to **both** lvalues and rvalues:
```cpp
const int& r = 42;   // OK — extends lifetime of the temporary
const int& r = x;    // OK — normal lvalue binding
```

This is why function parameters of type `const T&` accept both named variables
and temporaries:
```cpp
void print(const std::string& s) { ... }

print(name);          // lvalue — OK
print("hello");       // rvalue (temporary string) — also OK
```

### Why rvalue references matter
Rvalue references enable **move semantics** — transferring ownership of resources
instead of copying them. This is the foundation of `std::move` and `std::unique_ptr`.
Full move semantics are covered in the MoveSemantics example.

---

## Summary

```
int& ref         → lvalue reference   — alias, modifiable
const int& ref   → const reference    — alias, read-only, accepts temporaries
int&& ref        → rvalue reference   — binds to temporaries, enables move semantics
```

| Use case | Type |
|----------|------|
| Alias a variable | `T&` |
| Read-only function parameter | `const T&` |
| Modifying function parameter | `T&` |
| Avoid copy in range-based for | `const auto&` |
| Modify in range-based for | `auto&` |
| Move semantics | `T&&` |
