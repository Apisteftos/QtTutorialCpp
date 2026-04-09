# Copy Constructors in C++23

This project demonstrates why copy constructors exist, when they are called,
and the critical difference between shallow and deep copy.

---

## What is a Copy Constructor?

A copy constructor creates a **new object as a copy of an existing object**.

```cpp
ClassName(const ClassName& other);
```

It is called automatically in three situations:
1. **Initialization** — `MyClass b(a);` or `MyClass b = a;`
2. **Pass by value** — when an object is passed to a function by value
3. **Return by value** — when a function returns an object by value

---

## Example 1 — Shallow Copy: The Problem

When a class contains a **raw pointer**, the compiler-generated copy constructor
copies the **pointer address**, not the data it points to.

```cpp
ShallowBox box1(3);
ShallowBox box2 = box1;   // copies the pointer — both point to SAME memory!

// When both go out of scope:
// box1 destructor → delete[] data   ✅ first delete is fine
// box2 destructor → delete[] data   💥 double delete — undefined behavior!
```

```
box1.data ──┐
             ▼
            [ 0, 10, 20 ]
             ▲
box2.data ──┘
```

This causes **undefined behavior** — usually a crash or memory corruption.

---

## Example 2 — Deep Copy: The Solution

A custom copy constructor allocates **new memory** and copies the **values**:

```cpp
DeepBox(const DeepBox& other) : size(other.size), name(other.name) {
    data = new int[size];              // NEW allocation
    for (int i = 0; i < size; ++i)
        data[i] = other.data[i];       // copy VALUES
}
```

```
original.data ──▶ [ 0, 10, 20, 30 ]
copy.data     ──▶ [ 0, 10, 20, 30 ]   ← separate memory
```

Now modifying `copy.data` does **not** affect `original.data`.

---

## Example 3 — Copy Constructor vs Copy Assignment

These are two different operations and easy to confuse:

```cpp
Player p1("Kostas", 1500, 10);

Player p2(p1);       // copy constructor — p2 does not exist yet
Player p3 = p1;      // copy constructor — this is INITIALIZATION, not assignment!

Player p4("Andre", 800, 5);
p4 = p1;             // copy ASSIGNMENT operator — p4 already exists
```

| | Copy Constructor | Copy Assignment |
|--|-----------------|----------------|
| When | Object is being **created** | Object **already exists** |
| Signature | `T(const T& other)` | `T& operator=(const T& other)` |
| Called by `T b = a` | ✅ Yes (initialization) | ❌ No |
| Called by `b = a` | ❌ No | ✅ Yes |

### Copy assignment operator
Always include a **self-assignment guard**:
```cpp
Player& operator=(const Player& other) {
    if (this == &other) return *this;   // guard: p = p would be a bug without this
    name  = other.name;
    score = other.score;
    return *this;   // return *this to allow chaining: p1 = p2 = p3
}
```

---

## Example 4 — Implicit copy constructor calls

The copy constructor is called **silently** by the compiler in many situations:

```cpp
void printPlayer(Player p) { ... }   // p is a COPY of the argument
printPlayer(hero);                   // copy constructor called here

Player makePlayer() {
    Player p("temp", 0, 1);
    return p;                        // copy constructor may be called here
                                     // (or RVO/NRVO eliminates it — see below)
}
```

---

## The Rule of Three / Five / Zero

If your class needs a **custom destructor** (e.g. it owns raw memory),
you almost certainly also need:

### Rule of Three (C++03)
1. Destructor
2. Copy constructor
3. Copy assignment operator

### Rule of Five (C++11) — adds move semantics
4. Move constructor
5. Move assignment operator

### Rule of Zero (modern C++) — preferred
Design your class so it owns **no raw resources** directly.
Use `std::string`, `std::vector`, `std::unique_ptr` etc. — they manage their own memory
and their copy/move constructors work correctly automatically.

```cpp
// Rule of Zero — no custom copy/move/destructor needed
class Player {
    std::string name;   // manages its own memory
    int score;
    int level;
};
```

---

## When does the compiler generate a copy constructor automatically?

The compiler generates a default copy constructor **unless** you define any of:
- A custom destructor
- A custom copy constructor
- A custom copy assignment operator
- A move constructor
- A move assignment operator

If you define a destructor (e.g. `delete[] data`) — **always write the copy constructor too**.

---

## Copy Elision / RVO (Return Value Optimization)

Since C++17, the compiler is **required** to eliminate unnecessary copies in certain cases:

```cpp
DeepBox makeBox() {
    return DeepBox(4, "temp");  // C++17: copy constructor NOT called — object
}                               // constructed directly in the caller's memory
```

This is called **NRVO (Named Return Value Optimization)** or **RVO**.
You may notice the copy constructor is not called when you expect it — this is why.

---

## Summary

| Situation | What happens |
|-----------|-------------|
| Class has no raw pointers | Compiler-generated copy is fine |
| Class owns raw memory (`new`) | Must write custom copy constructor |
| `T b(a)` or `T b = a` | Copy constructor called |
| `b = a` (b already exists) | Copy assignment operator called |
| Pass by value to function | Copy constructor called |
| Return by value (C++17+) | Usually elided (RVO) |