# UniquePointer — std::unique_ptr in C++23

`std::unique_ptr` provides **exclusive ownership** of a heap-allocated object.
Only one `unique_ptr` can own the object at any time.
When the owner goes out of scope, the object is **automatically deleted**.

---

## Declaration

```cpp
#include <memory>

std::unique_ptr<Weapon> p = std::make_unique<Weapon>("Sword", 50);
```

Always prefer `std::make_unique<T>()` over `new`:
```cpp
// Preferred — exception safe, single allocation
auto p = std::make_unique<Weapon>("Sword", 50);

// Avoid — two separate expressions, exception between them leaks
std::unique_ptr<Weapon> p(new Weapon("Sword", 50));
```

---

## Example 1 — Automatic cleanup (RAII)

```cpp
{
    auto sword = std::make_unique<Weapon>("Sword", 50);
    sword->attack();
}   // destructor called automatically — no delete needed
```

No memory leak possible — the destructor always runs when the scope ends,
even if an exception is thrown.

---

## Example 2 — Cannot copy, only move

`unique_ptr` enforces exclusive ownership by **deleting the copy constructor**:

```cpp
auto bow = std::make_unique<Weapon>("Bow", 30);

auto bow2 = bow;              // ERROR — copy is deleted
auto bow2 = std::move(bow);   // OK — ownership transferred
// bow is now nullptr
```

After `std::move`, the original pointer is `nullptr`. Accessing it is undefined behavior.

```cpp
if (bow) bow->attack();   // safe — always check after move
```

---

## Example 3 — Transferring ownership

Functions can return `unique_ptr` to transfer ownership to the caller:

```cpp
std::unique_ptr<Weapon> createWeapon(const std::string& name, int damage) {
    return std::make_unique<Weapon>(name, damage);  // caller takes ownership
}

auto axe = createWeapon("Battle Axe", 80);   // ownership here now
auto stored = std::move(axe);                 // transfer again
```

This is the correct pattern for factory functions in modern C++.

---

## Example 4 — Borrowing without ownership

When a function needs to **use** an object but not **own** it:

```cpp
// Borrow via raw pointer — use .get()
void inspect(const Weapon* w) { ... }
inspect(spear.get());   // spear still owns the weapon

// Borrow via reference — preferred when non-null guaranteed
void use(const Weapon& w) { ... }
use(*spear);            // dereference to get reference
```

**Rule:** pass `unique_ptr` only when transferring ownership. Otherwise pass raw pointer or reference.

---

## Example 5 — Arrays

```cpp
auto scores = std::make_unique<int[]>(5);   // allocates int[5]
scores[0] = 100;
scores[1] = 200;
// deleted[] automatically — no need to call delete[]
```

Prefer `std::vector<int>` over `unique_ptr<int[]>` in most cases —
`vector` gives you size tracking and iterators for free.

---

## Example 6 — RAII pattern

`unique_ptr` is the standard tool for **RAII** (Resource Acquisition Is Initialization):

```cpp
{
    auto file = std::make_unique<FileHandle>("report.txt");
    file->write("data");
    // even if an exception is thrown here:
}   // file is always closed — destructor guaranteed to run
```

Works for any resource: files, sockets, database connections, GPU buffers, mutexes.

---

## Example 7 — reset() and release()

```cpp
auto p = std::make_unique<Weapon>("Dagger", 20);

p.reset();                        // destroys object, p becomes nullptr
p.reset(new Weapon("Magic", 45)); // destroys old, takes ownership of new

Weapon* raw = p.release();        // p gives up ownership — p becomes nullptr
// YOU are now responsible for deleting raw
delete raw;
```

| Method | What it does |
|--------|-------------|
| `reset()` | Destroys object, sets to nullptr |
| `reset(ptr)` | Destroys old object, takes ownership of `ptr` |
| `release()` | Gives up ownership without deleting — returns raw pointer |
| `get()` | Returns raw pointer — ownership stays with `unique_ptr` |

---

## Example 8 — vector of unique_ptr

```cpp
std::vector<std::unique_ptr<Weapon>> arsenal;
arsenal.push_back(std::make_unique<Weapon>("Crossbow", 40));
arsenal.push_back(std::make_unique<Weapon>("Hammer",   70));

for (const auto& w : arsenal)
    w->attack();
// all weapons destroyed when vector goes out of scope
```

Use `emplace_back` instead of `push_back` to construct in place:
```cpp
arsenal.emplace_back(std::make_unique<Weapon>("Katana", 65));
```

---

## unique_ptr vs raw pointer

| | Raw pointer | unique_ptr |
|--|------------|------------|
| Automatic cleanup | ❌ | ✅ |
| Copy | ✅ | ❌ |
| Move | ✅ | ✅ |
| Overhead | None | None (zero-cost abstraction) |
| Null check | Manual | `if (p)` |
| Array support | `delete[]` manual | `unique_ptr<T[]>` |

`unique_ptr` has **zero runtime overhead** compared to a raw pointer —
the compiler optimizes it away completely.

---

## When to use unique_ptr

✅ Single owner of a heap object
✅ Factory functions returning objects
✅ RAII resource management (files, sockets, handles)
✅ Polymorphism — `unique_ptr<Base>` storing derived objects
✅ Class members that own heap objects

❌ Shared ownership → use `shared_ptr`
❌ Non-owning observation → use raw pointer or reference
❌ Stack-allocated objects → no pointer needed
