# Null Pointers in C++23

A null pointer is a pointer that **does not point to any valid memory location**.
It is used to indicate "no object", "not found", or "uninitialized".

---

## nullptr — the modern way (main.cpp Example 1)

```cpp
int* p = nullptr;   // C++11 and later — always prefer this
int* p = 0;         // old C style — avoid
int* p = NULL;      // C macro — avoid in C++
```

`NULL` is a macro that expands to an integer-like null constant (GCC uses the built-in `__null`), not a pointer.

---

## nullptr vs NULL vs 0 (main.cpp Example 2)

`nullptr` is a keyword of type `std::nullptr_t` (declared in `<cstddef>`) — a dedicated type, not an integer.
This makes it **unambiguous** in overload resolution:

```cpp
void process(int x)  { ... }
void process(int* p) { ... }

process(0);        // calls process(int)   — 0 is an int
process(nullptr);  // calls process(int*)  — always the pointer version
process(NULL);     // ❌ error: call of overloaded 'process(NULL)' is ambiguous
```
✅ tested with GCC 13 and Clang: `process(NULL)` is rejected as ambiguous, `process(0)` picks the `int` overload. An earlier version of `main.cpp` used two lambdas with different names (`process`, `processPtr`) instead of overloads, so the commented-out "ambiguity" calls it described were not real overload problems; the program now uses real overloads.

---

## Example 3 — Always check before dereferencing

Dereferencing a null pointer is **undefined behavior** — usually a crash (segmentation fault) on desktop systems, but the standard does not guarantee one:

```cpp
int* p = nullptr;
*p = 5;           // undefined behavior — typically a segmentation fault
```

Always guard with a null check:

```cpp
if (p != nullptr) {
    *p = 5;       // safe
}

// Shorter equivalent — pointer in boolean context
if (p) {
    *p = 5;       // safe
}
```

---

## Example 4 — nullptr as "not found"

Returning `nullptr` from a function is a standard C++ pattern for "not found":

```cpp
Player* findPlayer(Player* players, int count, const std::string& name)
{
    for (int i = 0; i < count; ++i)
        if (players[i].name == name)
            return &players[i];
    return nullptr;   // not found
}

Player* result = findPlayer(players, 3, "Kostas");
if (result != nullptr) {
    std::cout << result->name;   // safe — we know it's valid
}
```

This pattern is common throughout Qt's API — `QObject::findChild()`,
`QLayout::itemAt()`, and many others return `nullptr` on failure (from memory of the Qt docs, not run here).

> In modern C++ prefer `std::optional<T>` over returning `nullptr`
> when the function result has value semantics. Use pointer return
> when you genuinely return a pointer into existing storage.

---

## Example 5 — Dangling pointer vs null pointer

| | Null pointer | Dangling pointer |
|--|-------------|-----------------|
| Value | `nullptr` (0) | Some old/freed address |
| Safe to check | ✅ `if (p)` works | ❌ `if (p)` is still true, so it tells you nothing |
| Safe to dereference | ❌ (undefined behavior) | ❌ (undefined behavior, may even appear to work) |
| How to create | `p = nullptr` | `delete p;` without `p = nullptr`, or a pointer to a destroyed local |

Always set a pointer to `nullptr` after `delete`:

```cpp
int* p = new int(10);
delete p;        // memory freed — but p still holds the old address!
p = nullptr;     // now safe — if (p) correctly returns false
delete p;        // deleting a null pointer is allowed and does nothing ✅ (Example 5 prints this)
```

Limits of this habit: it only protects *that variable*. Other copies of the address (`int* q = p;` made before the `delete`) still dangle. Where possible avoid raw `new`/`delete` altogether and use RAII types such as `std::unique_ptr` (C++ Core Guidelines [R.11](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rr-newdelete)).

---

## Example 6 — std::nullptr_t

`nullptr` has its own type: `std::nullptr_t`

```cpp
std::nullptr_t n = nullptr;
int*    pi = n;    // OK — implicitly converts to any pointer type
double* pd = n;    // OK
```

Useful for writing overloads that specifically handle `nullptr`:

```cpp
void handle(int* p)           { std::cout << "got pointer\n"; }
void handle(std::nullptr_t)   { std::cout << "got nullptr\n"; }

handle(nullptr);   // calls the nullptr_t overload — unambiguous
```
✅ tested (exact match with `nullptr_t` beats the conversion to `int*`).

Two more things from `main.cpp`:

- `bool b = nullptr;` does not compile (`converting to 'bool' from 'std::nullptr_t' requires direct-initialization`), `bool b(nullptr);` does and gives `false` ✅ tested.
- **Why the program prints pointers through `static_cast<const void*>`:** with `std::cout`, a `char*` is printed as a C string, so a *null* `char*` is undefined behavior (✅ tested with libstdc++: the stream gets `badbit` and later output silently disappears). `std::println` cannot format `int*` / `double*` directly at all, so every pointer is cast to `const void*`, which prints as `0x0` (the old `std::cout` version printed `0`).

---

## nullptr in Qt

Qt follows the same pattern extensively:

```cpp
QObject* obj = parent->findChild<QObject*>("myWidget");
if (obj != nullptr) {
    // safe to use
}

QLayoutItem* item = layout->itemAt(0);
if (item) {
    // safe to use
}
```

---

## Summary

| Do | Don't |
|----|-------|
| `int* p = nullptr;` | `int* p;` (uninitialized) |
| Check `if (p)` before `*p` | Dereference without checking |
| `p = nullptr` after `delete p` | Leave dangling pointer |
| Use `nullptr` keyword | Use `0` or `NULL` |
| Use `std::optional` for value semantics | Return `nullptr` for value types |


---

## 🚀 How to run

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o nullptr_demo
./nullptr_demo
```
`main.cpp` uses `std::println` (`<print>`), so it needs GCC 14+ or a recent Clang with `-std=c++23`.

---

## 🧾 Notes on main.cpp

This program had no lecture text to move; the short explanatory messages stay in the output. Changes:

- All `std::cout` calls converted to `std::println`. Pointers are printed via `static_cast<const void*>` (output `0x0` instead of `0`); `true`/`false` come from formatting the `bool` directly.
- Example 2: the two differently named lambdas (`process`, `processPtr`) did not demonstrate overload resolution. They are now real overloads `process(int)` / `process(int*)` above `main`, with the `process(NULL)` ambiguity as a commented-out line, and a printed check `is_same_v<decltype(nullptr), std::nullptr_t>`.
- Example 5: added `delete` on the null pointer (a no-op) with a message.
- "CRASH" comments reworded to "undefined behavior (typically a crash)". `<cstddef>` and `<type_traits>` added.
- ✅ tested with `-Wall -Wextra`, AddressSanitizer and UndefinedBehaviorSanitizer (and Clang); the output differs from the original only in the pointer format (`0x0`) and the lines added above.

---

## ⚠️ Open points

- The README's example numbers (1–4) did not match `main.cpp` (1–6); the headings now use the `main.cpp` numbers.
- This folder has no number or Previous/Next links in the README.
- `Player` uses the names "Kostas", "Andre", "Maria" in `main.cpp` (and "Kostas" in the README) - fine if intended; say if you want neutral names.
- The Core Guidelines anchor `#rr-newdelete` comes from the guidelines text; the Qt statements are from memory.
