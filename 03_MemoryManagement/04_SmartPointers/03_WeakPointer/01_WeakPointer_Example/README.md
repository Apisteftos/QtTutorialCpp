# WeakPointer — std::weak_ptr in C++23

`std::weak_ptr` is a **non-owning** pointer to an object managed by `shared_ptr`.
It does not affect the reference count and does not keep the object alive.

---

## Why weak_ptr exists

`shared_ptr` uses reference counting — the object lives as long as at least one
`shared_ptr` points to it. But what happens when two objects hold `shared_ptr`
references to each other?

```
Parent  ──shared_ptr──▶  Child
  ▲                         │
  └──────shared_ptr──────────┘
```

Both reference counts stay at 1 forever — **neither object is ever destroyed**.
This is a **circular reference** — a memory leak that `shared_ptr` alone cannot solve.

`weak_ptr` breaks the cycle by observing without owning:

```
Parent  ──shared_ptr──▶  Child
  ▲                         │
  └───────weak_ptr───────────┘   ← does not prevent destruction
```

---

## Example 1 — Circular reference (the problem)

```cpp
class BadNode {
public:
    std::shared_ptr<BadNode> parent;   // creates cycle!
    std::vector<std::shared_ptr<BadNode>> children;
};

{
    auto parent = std::make_shared<BadNode>("Parent");
    auto child  = std::make_shared<BadNode>("Child");

    parent->children.push_back(child);
    child->parent = parent;   // circular reference

}   // MEMORY LEAK — neither destructor is called!
```

When the scope ends:
- `parent` local variable destroyed → ref count: 1 (child still holds it)
- `child`  local variable destroyed → ref count: 1 (parent still holds it)
- Neither reaches 0 → neither is destroyed

---

## Example 2 — Circular reference (the solution)

```cpp
class GoodNode {
public:
    std::weak_ptr<GoodNode> parent;   // non-owning — breaks the cycle
    std::vector<std::shared_ptr<GoodNode>> children;
};

{
    auto parent = std::make_shared<GoodNode>("Parent");
    auto child  = std::make_shared<GoodNode>("Child");

    parent->children.push_back(child);
    child->parent = parent;   // weak_ptr — does NOT increment ref count

}   // parent ref count → 0 → destroyed ✅
    // child  ref count → 0 → destroyed ✅
```

---

## Accessing a weak_ptr — lock()

You **cannot dereference** a `weak_ptr` directly. You must convert it to a
`shared_ptr` first using `.lock()`:

```cpp
std::weak_ptr<GoodNode> weakParent = child->parent;

if (auto p = weakParent.lock()) {   // returns shared_ptr if still alive
    std::cout << p->name;           // safe — p keeps object alive during use
} else {
    std::cout << "parent is gone";  // object was already destroyed
}
```

`.lock()` is **atomic** — it either returns a valid `shared_ptr` or `nullptr`.
There is no race condition between checking and using.

---

## Checking if expired

```cpp
std::weak_ptr<GoodNode> w = node;

w.expired()      // true if object has been destroyed
w.use_count()    // number of shared_ptr owners (0 if destroyed)
w.lock()         // returns shared_ptr or nullptr
```

```cpp
// Inner scope
{
    auto node = std::make_shared<GoodNode>("Temp");
    weakRef = node;
    std::cout << weakRef.expired();    // 0 (false) — alive
}
// node destroyed here

std::cout << weakRef.expired();        // 1 (true) — destroyed
if (auto p = weakRef.lock()) { ... }  // p is nullptr — block skipped
```

---

## Example 3 — Observer pattern

`weak_ptr` is the standard tool for the **Observer pattern** — observers should
not prevent the subject from being destroyed:

```cpp
class Subject {
    std::vector<std::weak_ptr<Observer>> observers;  // non-owning

    void notify() {
        for (auto it = observers.begin(); it != observers.end(); ) {
            if (auto obs = it->lock()) {   // still alive?
                obs->update(data);
                ++it;
            } else {
                it = observers.erase(it);  // remove dead reference
            }
        }
    }
};
```

When an `Observer` is destroyed, the `Subject` automatically skips it on the
next notification. No manual unregistration needed.

This pattern is used throughout Qt — `QPointer<T>` is Qt's equivalent of `weak_ptr`
for `QObject`-derived classes.

---

## weak_ptr vs shared_ptr vs unique_ptr

| | unique_ptr | shared_ptr | weak_ptr |
|--|-----------|-----------|---------|
| Ownership | Exclusive | Shared | None |
| Ref count | No | Yes | No (observes only) |
| Keeps object alive | Yes | Yes | No |
| Can dereference | Yes | Yes | No — must lock() first |
| Copy | ❌ | ✅ | ✅ |
| Move | ✅ | ✅ | ✅ |
| Use for | Single owner | Multiple owners | Breaking cycles, observers |

---

## When to use weak_ptr

✅ Parent-child relationships where child holds reference back to parent
✅ Observer / event listener patterns
✅ Caches — hold reference without preventing cleanup
✅ Any situation where you want to observe but not own

❌ Do not use as a primary access mechanism — always `lock()` before use
❌ Do not use where you need guaranteed object lifetime — use `shared_ptr`

---

## Summary

```
shared_ptr  →  owns the object, keeps it alive
weak_ptr    →  watches the object, does NOT keep it alive
               must lock() to access — may return nullptr if object is gone
```

The rule is simple: if two objects need to reference each other,
one side uses `shared_ptr` (the owner) and the other uses `weak_ptr` (the observer).
