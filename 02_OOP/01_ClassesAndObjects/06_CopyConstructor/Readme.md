# Copy Constructors in C++23

This project demonstrates why copy constructors exist, when they are called,
and the critical difference between shallow and deep copy.

---

## What is a Copy Constructor?

A copy constructor creates a **new object as a copy of an existing object**.

```cpp
ClassName(const ClassName& other);
```

It is called automatically in three main situations:
1. **Initialization** — `MyClass b(a);` or `MyClass b = a;`
2. **Pass by value** — when an object is passed to a function by value
3. **Return by value** — when a function returns an object by value (often elided, see below)

(Copying an element into a container, e.g. `push_back(obj)`, also uses it.)

---

## Example 1 — Shallow Copy: The Problem

When a class contains a **raw pointer**, the compiler-generated copy constructor
copies the **pointer address**, not the data it points to.

```cpp
ShallowBox box1(3);
ShallowBox box2 = box1;   // copies the pointer — both point to SAME memory!

// When both go out of scope (if the destructor calls delete[] data):
// box1 destructor → delete[] data   ✅ first delete is fine
// box2 destructor → delete[] data   💥 double delete — undefined behavior!
```

> **In `main.cpp` the `delete[]` is commented out**, so the demo does not crash. The
> memory is simply never freed (a leak; AddressSanitizer reports 12 bytes lost).
> Uncomment it in `~ShallowBox` to see the real double delete.

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
DeepBox(const DeepBox& other) : size(other.size), name(other.name + "_copy") {
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

`main.cpp` appends `"_copy"` to the name in the copy constructor, only so that the
output shows which object is the copy (`original` → `original_copy`).

> ⚠️ `DeepBox` has a copy constructor but **no copy assignment operator**. `a = b;`
> would use the compiler-generated one, copy the pointer, and both destructors would
> `delete[]` it (AddressSanitizer: `attempting double-free`, tested). That is the Rule
> of Three below. The demo never assigns a `DeepBox`, so it does not show up.

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
    name  = other.name;       // (main.cpp appends "_assigned" so the output shows it)
    score = other.score;
    level = other.level;
    return *this;   // return *this to allow chaining: p1 = p2 = p3
}
```
For `Player` (only `std::string` and `int`) the guard is just a cheap shortcut. It becomes
essential when the operator first frees memory and then copies from `other`: with
`p = p` you would free the data you are about to copy.

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

The compiler generates a copy constructor (member-wise copy) **unless**:
- you declare your own copy constructor, or
- you declare a **move constructor or a move assignment operator**. Then the implicit copy
  constructor is defined as **deleted** (tested: `use of deleted function 'MV::MV(const MV&)'`).

Declaring only a **destructor** or a **copy assignment operator** does *not* stop it. The
compiler still generates the copy constructor (deprecated since C++11, and GCC warns about
it with `-Wextra` in the copy-assignment case). That is exactly the trap of the Rule of
Three: a class with `delete[] data` in its destructor still gets a pointer-copying copy
constructor unless you write one.

So if you define a destructor that frees memory, **always write the copy constructor
(and copy assignment) too**.

---

## Copy Elision / RVO (Return Value Optimization)

Since C++17 the compiler is **required** to skip the copy when you return (or initialize from)
a **temporary** (a prvalue):

```cpp
DeepBox makeBox() {
    return DeepBox(4, "temp");  // guaranteed (C++17): no copy, no move.
}                               // The object is constructed directly in the caller's memory.
```

This is called **RVO** and it works even if the class has no accessible copy or move
constructor.

Returning a **named local variable** is different (**NRVO**):

```cpp
DeepBox makeBox2() {
    DeepBox b(4, "named");
    return b;                   // NRVO: the compiler MAY skip the copy, but is not required to
}
```

Tested with a class that prints from its copy constructor: with GCC the single-candidate case
above prints no copy, but `-fno-elide-constructors` brings the copy back, while the temporary
case stays copy-free even with that flag. If a function has two different named locals it may
return (`if (x) return a; return c;`), GCC did not elide and called the copy constructor.
So never rely on NRVO to avoid a copy.

---

## Summary

| Situation | What happens |
|-----------|-------------|
| Class has no raw pointers | Compiler-generated copy is fine |
| Class owns raw memory (`new`) | Must write custom copy constructor |
| `T b(a)` or `T b = a` | Copy constructor called |
| `b = a` (b already exists) | Copy assignment operator called |
| Pass by value to function | Copy constructor called |
| Return a temporary, `return T(...)` (C++17+) | Copy always elided (guaranteed) |
| Return a named local, `return x;` | Usually elided (NRVO), not guaranteed |
| Class declares a move constructor/assignment | Implicit copy constructor is deleted |
| Class has a destructor that frees memory | Also write copy constructor and copy assignment (Rule of Three) |

---

## How to Run

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o copy_constructor
./copy_constructor
```
`main.cpp` uses `std::println`, so it needs GCC 14 or newer (or another compiler with `<print>`).
The pointer addresses in the output are different on every run.

In Example 4 (`printPlayer(hero)`) the parameter is a copy, so you see `[Player] Copy constructor
called for 'Hero_copy'` before the call and its destructor message right after it.
