# Structs in C++

## What is a struct?

A `struct` is a user-defined type that groups related data together into a single unit. It originated in C as a way to bundle variables (of possibly different types) under one name, with no behavior attached — just data.

```cpp
struct Point2D {
    int x;
    int y;
};
```

In **C**, a struct can *only* hold data. No constructors, no methods, no access control — it's purely a data container.

In **C++**, `struct` was extended to support everything a `class` can: constructors, destructors, methods, inheritance, templates, virtual functions, access specifiers — the full feature set. This is the source of a lot of confusion, which the next section clears up.

---

## struct vs class — the only real difference

In C++, `struct` and `class` are **functionally identical** except for one thing: **default access level**.

| | `struct` | `class` |
|---|---|---|
| Default member access | `public` | `private` |
| Default inheritance access | `public` | `private` |
| Can have constructors/destructors | ✅ | ✅ |
| Can have methods | ✅ | ✅ |
| Can inherit | ✅ | ✅ |
| Can be templated | ✅ | ✅ |
| Can have virtual functions | ✅ | ✅ |
| Can have private/protected members | ✅ (must specify) | ✅ (default) |

```cpp
struct S { int value; };        // value is public by default

class C { int value; };         // value is private by default
```

That's it. The compiler does not treat them differently beyond this. Everything else you associate with "structs are simple, classes are complex" is **convention**, not a language rule.

---

## Why the convention exists anyway

Since the language doesn't force a distinction, the reason to pick one over the other is about **communicating intent** to other developers (and your future self):

- **`struct`** → "This is just data. No hidden logic, no invariants, safe to read/write freely."
- **`class`** → "This manages its own state. There may be rules being enforced — don't poke at internals."

Seeing the keyword alone tells you how to treat the type, before you've read a single line of its implementation.

---

## Technical benefits of keeping a struct simple (POD / aggregate)

If a struct has **no user-defined constructors, no private/protected members, and no virtual functions**, it qualifies as an **aggregate**, and if it also has trivial special member functions, it's a **POD type** (Plain Old Data). This unlocks:

### 1. Aggregate initialization
```cpp
Point2D p{10, 20};   // no constructor needed
```

### 2. Designated initializers (C++20)
```cpp
Config cfg{.width = 1920, .height = 1080, .fullscreen = true};
```

### 3. Structured bindings (C++17)
```cpp
auto [x, y] = p;
```

### 4. Predictable memory layout
A POD struct can be safely `memcpy`'d, written to shared memory, or sent over a network/socket, because its layout is guaranteed and stable. This matters directly for:
- Shared memory IPC (e.g. a `SharedData` struct mapped between two processes)
- Wire/protocol formats (packet headers, MCX-style framing)
- Serialization without a library

As soon as you add a constructor, private members, or virtual functions, the type stops being an aggregate/POD, and you lose these guarantees — even if it's still technically a `struct`.

---

## When to use `struct`

- Plain data grouping, no behavior (`Point2D`, `Config`, `SharedData`)
- Small value types passed around freely (2D/3D vectors, coordinates)
- Return values that bundle multiple results (instead of `std::tuple`)
- Data that needs a stable, predictable memory layout (shared memory, protocol headers)
- Internal implementation details where public access is intentional and simple

## When to use `class`

- The type needs to enforce invariants (e.g. a `BankAccount` that must never have negative balance)
- Internal state should be hidden and only modified through controlled methods
- The type has real behavior beyond holding values
- You want to draw a clear line for maintainers: "this is more than data"

---

## Padding and alignment (why member order matters)

Struct members can have compiler-inserted padding to satisfy alignment requirements. Ordering members from **largest to smallest** minimizes wasted space:

```cpp
struct Unpacked {
    char  a;   // 1 byte
    int   b;   // 4 bytes -> 3 bytes padding inserted before this
    char  c;   // 1 byte  -> 3 bytes padding inserted after this
};             // sizeof = 12

struct Reordered {
    int   b;   // 4 bytes
    char  a;   // 1 byte
    char  c;   // 1 byte -> 2 bytes padding at the end
};             // sizeof = 8
```

This matters for large arrays of structs (cache efficiency) and for structs that map directly onto binary data.

---

## Summary

- Structurally, `struct` and `class` in C++ are the same thing with a different default access level.
- The distinction that matters is **convention**: struct = data, class = behavior + encapsulation.
- Keeping a struct as a plain aggregate/POD gives you real technical benefits (init syntax, structured bindings, safe memory layout) — but only if you don't add constructors, private members, or virtuals to it.
- Pick the keyword that tells the truth about what the type is for.

---

*Location in this repo: `01_Fundamentals/09_Structs/` — placed in Fundamentals rather than OOP because structs are taught here as C-style data containers first; their OOP-capable features (constructors, methods, inheritance) are covered as a secondary note, not the main focus. See `main.cpp` in this folder for runnable examples of every concept above.*
