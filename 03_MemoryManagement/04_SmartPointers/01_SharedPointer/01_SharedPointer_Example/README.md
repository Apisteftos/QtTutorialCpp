# 06 — Smart Pointers in C++23

Smart pointers are wrapper classes that manage the lifetime of heap-allocated objects.
They replace raw `new`/`delete` and make memory management automatic and exception-safe.

All smart pointers live in `#include <memory>`.

---

## The three smart pointers

### `std::unique_ptr<T>` — exclusive ownership
```cpp
auto p = std::make_unique<MyClass>();
```
- **One owner only** — cannot be copied, only moved
- Object destroyed when the `unique_ptr` goes out of scope
- Zero overhead compared to raw pointer
- **Use this by default**

### `std::shared_ptr<T>` — shared ownership
```cpp
auto p = std::make_shared<MyClass>();
```
- **Multiple owners** — reference counted
- Object destroyed when the **last** `shared_ptr` to it is destroyed
- Small overhead: atomic reference count increment/decrement
- Use when multiple parts of the code genuinely share ownership

### `std::weak_ptr<T>` — non-owning observer
```cpp
std::weak_ptr<MyClass> w = sharedPtr;
```
- **No ownership** — does not affect reference count
- Object may be destroyed while `weak_ptr` still exists
- Must call `.lock()` to access — returns `nullptr` if object is gone
- Use to break circular references between `shared_ptr` objects

---

## Quick decision guide

```
Do you need heap allocation?
│
├── NO  → use a stack variable (no pointer needed)
│
└── YES → Who owns the object?
          │
          ├── ONE owner → std::unique_ptr
          │
          ├── MULTIPLE owners → std::shared_ptr
          │
          └── NO ownership (observe only) → std::weak_ptr
```

---

## Folder structure

```
06_SmartPointers/
├── README.md                  ← you are here
├── SharedPointer/
│   ├── main.cpp               — Document/User shared ownership example
│   └── README.md              — shared_ptr reference counting, use_count()
├── UniquePointer/
│   ├── main.cpp               — Weapon/FileHandle exclusive ownership example
│   └── README.md              — move semantics, RAII, reset(), release()
└── WeakPointer/
    ├── main.cpp               — BadNode/GoodNode circular reference + Observer pattern
    └── README.md              — lock(), expired(), breaking cycles
```

---

## Comparison table

| | `unique_ptr` | `shared_ptr` | `weak_ptr` |
|--|-------------|-------------|-----------|
| Header | `<memory>` | `<memory>` | `<memory>` |
| Ownership | Exclusive | Shared | None |
| Reference count | No | Yes | No |
| Copyable | ❌ | ✅ | ✅ |
| Moveable | ✅ | ✅ | ✅ |
| Dereference | `*p`, `p->` | `*p`, `p->` | Must `.lock()` first |
| Null check | `if (p)` | `if (p)` | `.expired()` or `.lock()` |
| Create with | `make_unique` | `make_shared` | from `shared_ptr` |
| Runtime cost | Zero | Atomic refcount | None extra |
| Destroys when | Scope ends | Last owner gone | Never (non-owning) |

---

## Always use make_unique / make_shared

```cpp
// PREFERRED — exception safe, single allocation
auto p1 = std::make_unique<MyClass>(args);
auto p2 = std::make_shared<MyClass>(args);

// AVOID — two expressions, potential leak between them
std::unique_ptr<MyClass> p1(new MyClass(args));
std::shared_ptr<MyClass> p2(new MyClass(args));
```

`make_shared` also performs a **single allocation** for both the object and the
reference count block — `shared_ptr(new T)` requires two separate allocations.

---

## Smart pointers and Qt

Qt has its own memory management for `QObject`-derived classes (parent/child ownership).
For non-`QObject` types, smart pointers are the right choice:

| Situation | Recommendation |
|-----------|---------------|
| `QObject` with parent | Qt parent/child — no smart pointer needed |
| `QObject` without parent | `std::unique_ptr<QObject>` or `QScopedPointer` |
| Shared non-QObject | `std::shared_ptr<T>` |
| Observing QObject lifetime | `QPointer<T>` (Qt's equivalent of `weak_ptr`) |

---

## The Rule of Zero

If you use smart pointers and standard containers (`std::vector`, `std::string` etc.)
as class members, you never need to write a custom destructor, copy constructor,
or assignment operator — the compiler generates correct ones automatically.

```cpp
// Rule of Zero — no custom destructor/copy/move needed
class Player {
    std::string              name;      // manages its own memory
    std::unique_ptr<Weapon>  weapon;    // unique ownership
    std::vector<std::string> inventory; // manages its own memory
};
```
