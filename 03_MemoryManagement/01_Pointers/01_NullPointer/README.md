# Null Pointers in C++23

A null pointer is a pointer that **does not point to any valid memory location**.
It is used to indicate "no object", "not found", or "uninitialized".

---

## nullptr — the modern way

```cpp
int* p = nullptr;   // C++11 and later — always prefer this
int* p = 0;         // old C style — avoid
int* p = NULL;      // C macro — avoid in C++
```

`nullptr` is a keyword of type `std::nullptr_t` — a dedicated type, not an integer.
This makes it **unambiguous** in overload resolution:

```cpp
void process(int x)  { ... }
void process(int* p) { ... }

process(0);        // calls process(int)   — ambiguous intent
process(nullptr);  // calls process(int*)  — always correct
```

---

## Example 1 — Always check before dereferencing

Dereferencing a null pointer is **undefined behavior** — it will crash:

```cpp
int* p = nullptr;
*p = 5;           // CRASH — segmentation fault
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

## Example 2 — nullptr as "not found"

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
`QLayout::itemAt()`, and many others return `nullptr` on failure.

> In modern C++ prefer `std::optional<T>` over returning `nullptr`
> when the function result has value semantics. Use pointer return
> when you genuinely return a pointer into existing storage.

---

## Example 3 — Dangling pointer vs null pointer

| | Null pointer | Dangling pointer |
|--|-------------|-----------------|
| Value | `nullptr` (0) | Some old/freed address |
| Safe to check | ✅ `if (p)` works | ❌ check passes but dereference crashes |
| Safe to dereference | ❌ | ❌ |
| How to create | `p = nullptr` | `delete p;` without `p = nullptr` |

Always set a pointer to `nullptr` after `delete`:

```cpp
int* p = new int(10);
delete p;        // memory freed — but p still holds the old address!
p = nullptr;     // now safe — if (p) correctly returns false
```

---

## Example 4 — std::nullptr_t

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
