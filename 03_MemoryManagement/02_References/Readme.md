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
| Can be null | ✅ | ❌ — must refer to a valid object (a "null reference" is undefined behavior) |
| Can be reseated | ✅ | ❌ — fixed at initialization |
| Needs dereference | ✅ `*p` | ❌ — use directly |
| Must initialize | ❌ | ✅ — always |
| Arithmetic | ✅ | ❌ |
| Array / container element type | ✅ | ❌ (`std::vector<int&>` does not compile ✅ tested; use `std::reference_wrapper`) |
| Can dangle | ✅ | ✅ — a reference to a destroyed object is just as bad |

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
int& ref2 = x;   // OK
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

Three alternative versions of the same function (in `main.cpp` they have different names, `levelUpByValue` / `levelUpByRef` / `printPlayer`, because `levelUp(Player)` and `levelUp(Player&)` together make the call `levelUp(kostas)` ambiguous ✅ tested):

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
`main.cpp` now proves it: `LargeData` counts its copies (`LargeData::copyCount`), which goes from 0 to 1 after the by-value call and stays at 1 after the by-const-reference call.

Rule of thumb for function parameters:

| Type | Recommended parameter style |
|------|----------------------------|
| `int`, `float`, `bool`, `char` | by value |
| `std::string` (read only) | `const std::string&` |
| `std::vector` (read only) | `const std::vector<T>&` |
| Any struct/class (read only) | `const T&` |
| Any struct/class (modify) | `T&` |
| `std::string_view`, `std::span` | by value (they are small views) |
| Smart pointers | `std::unique_ptr` by value only to *transfer ownership* (it cannot be copied, you pass it with `std::move`); `std::shared_ptr` by value only to share ownership (copying costs a reference-count update); otherwise pass `T&` / `T*` to the pointee (Core Guidelines [F.7](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rf-smart)) |

This is Core Guidelines [F.16](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rf-in): pass cheaply copied types by value and others by `const&`.

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
`std::swap` (declared in `<utility>`) works the same way ✅ tested: it compiles with only `<utility>` included.

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
✅ GCC warns about it by default: `reference to local variable 'local' returned [-Wreturn-local-addr]`.

Only return references to:
- Parameters passed by reference
- Members of objects that outlive the function
- Static variables
- Elements of containers passed in - but such a reference becomes invalid when the container reallocates (e.g. `push_back` on a `std::vector`) or the element is erased; do not keep it across such calls

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
(For a small type like `int`, `for (int s : scores)` is just as good; the const reference matters for strings, structs and other expensive-to-copy types.)

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
                   // cannot bind non-const lvalue reference of type 'int&' to an rvalue of type 'int' ✅ tested
```

### rvalue reference (`&&`)
Binds to a **temporary** (something without a persistent address):
```cpp
int&& rref  = 10;      // OK — 10 is a temporary
int&& rref2 = x * 2;   // OK — expression result is a temporary

int&& rref3 = rref;    // ERROR — see below
```

**A named rvalue reference is itself an lvalue.** `rref` has a name, so it cannot be bound to another `int&&`: ✅ tested, `cannot bind rvalue reference of type 'int&&' to lvalue of type 'int'`. To pass it on as an rvalue you must cast it, which is exactly what `std::move(rref)` does.

### const lvalue reference — special rule
Can bind to **both** lvalues and rvalues:
```cpp
const int& r1 = 42;   // OK — extends lifetime of the temporary
const int& r2 = x;    // OK — normal lvalue binding
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
instead of copying them. `std::move` (a cast to an rvalue reference) and `std::unique_ptr` are built on this.
Full move semantics are covered in the 08_MoveSemantics example.

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


---

## 🚀 How to run

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o references
./references
```
`main.cpp` uses `std::println` (`<print>`), so it needs GCC 14+ or a recent Clang with `-std=c++23`.

---

## 🧾 Notes on main.cpp

No lecture text to move. Changes:

- `std::cout` → `std::println` / `std::print`; the `YES`/`NO` and message strings are unchanged.
- Example 4: `LargeData` now counts its copies, so the program shows `0 → 1` copies after the by-value call and no further copy for the const reference (a user-written copy constructor and a defaulted copy assignment were added for this).
- Example 8: the by-value loop variable is `[[maybe_unused]]` (Clang warns it is set but not used - that is the point of the demo).
- Comment on `getFirstName`: precondition that `names` is not empty.
- ✅ tested with `-Wall -Wextra`, AddressSanitizer and UndefinedBehaviorSanitizer (and Clang); output identical to the original except the two added "copies so far" lines.

---

## ⚠️ Open points

- The example numbers in the README already matched `main.cpp` (1-9).
- This folder has no number or Previous/Next links.
- `Player` uses the names "Kostas", "Andre", "Maria" (and "Konstantinos" in Example 7) - say if you want neutral names.
- The Core Guidelines anchors (`#rf-smart`, `#rf-in`) come from the guidelines text; I did not open the pages. The `vector` reallocation warning is standard behavior, not run here.
