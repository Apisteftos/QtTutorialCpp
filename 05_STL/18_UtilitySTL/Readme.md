# MoveSemantics — std::move in C++23

`std::move` doesn't move anything by itself. It's a **cast** — it tells the
compiler "treat this lvalue as an rvalue," which makes it *eligible* to be
moved from instead of copied. The actual moving happens in whatever
function receives it (a move constructor, move assignment operator, or a
container's internal logic).

---

## Header

```cpp
#include <utility>      // std::move, std::forward, std::swap, std::exchange, std::pair, std::cmp_*, std::in_place
```

> **Careful:** `std::move` for a *single object* lives in `<utility>`. A
> different, unrelated `std::move` — the 3-argument **range algorithm**
> that copies/moves a whole range into another container — lives in
> `<algorithm>`. Almost every time someone says "`std::move`" they mean
> the `<utility>` version below.

---

## What it actually is

```cpp
template<typename T>
constexpr std::remove_reference_t<T>&& move(T&& t) noexcept {
    return static_cast<std::remove_reference_t<T>&&>(t);
}
```

It's `static_cast<T&&>(t)` with a friendlier name. `std::move` promises
nothing on its own — it only **unlocks** move semantics for whatever code
runs next. If nothing on the receiving end has a move constructor/move
assignment operator, `std::move` silently falls back to an ordinary copy.

---

## The problem it solves

```cpp
std::vector<std::string> makeNames() {
    std::vector<std::string> names = {"Anna", "Kostas", "Maria"};
    return names;
}

std::vector<std::string> a = makeNames();     // no copy needed — RVO/move
std::vector<std::string> b = a;               // COPY — a is untouched
std::vector<std::string> c = std::move(a);    // MOVE — a is now empty/unspecified
```

- `b = a` — every string inside `a` is deep-copied into `b`. `a` still
  holds valid data afterward.
- `c = std::move(a)` — `a`'s internal buffer/pointers are **transferred**
  to `c` directly. No deep copy. `a` is left in a valid but unspecified
  (typically empty) state — you must not rely on its contents afterward.

---

## Example 1 — moving a large object

```cpp
#include <algorithm>    // std::copy
#include <cstddef>
#include <iostream>
#include <utility>

class Buffer {
public:
    explicit Buffer(std::size_t size) : m_size(size), m_data(new int[size]) {
        std::cout << "Constructed Buffer(" << m_size << ")\n";
    }

    // Copy constructor — expensive, allocates + copies
    Buffer(const Buffer& other) : m_size(other.m_size), m_data(new int[other.m_size]) {
        std::copy(other.m_data, other.m_data + m_size, m_data);
        std::cout << "COPY constructed (" << m_size << " ints copied)\n";
    }

    // Move constructor — cheap, steals the pointer
    Buffer(Buffer&& other) noexcept
        : m_size(other.m_size), m_data(other.m_data) {
        other.m_data = nullptr;   // other no longer owns the memory
        other.m_size = 0;
        std::cout << "MOVE constructed (pointer stolen, no copy)\n";
    }

    ~Buffer() { delete[] m_data; }

private:
    std::size_t m_size;
    int* m_data;
};

int main() {
    Buffer a(1'000'000);          // Constructed Buffer(1000000)

    Buffer b = a;                 // COPY constructed — 1,000,000 ints copied
    Buffer c = std::move(a);      // MOVE constructed — just a pointer swap

    // a is now in a valid-but-empty state — do not use a's data after this
}
```

**Output:**
```
Constructed Buffer(1000000)
COPY constructed (1000000 ints copied)
MOVE constructed (pointer stolen, no copy)
```

`std::move(a)` is what tells the compiler to prefer `Buffer(Buffer&&)`
over `Buffer(const Buffer&)` — without it, `Buffer c = a;` would always
pick the copy constructor, even if a move constructor exists.

`main.cpp` also has the copy and move **assignment** operators (the snippet above omits them for space) and
shows `d = b;` → `COPY assigned` and `d = std::move(c);` → `MOVE assigned`. A class with a user-declared move
constructor and no assignment operators would not even allow `d = b;` (see the Rule of 5 section).

---

## Example 2 — moving into a container

```cpp
std::vector<std::string> names;
names.reserve(3);

std::string s = "a very long string that would be expensive to copy...";

names.push_back(s);              // COPY — s is still valid and unchanged after
names.push_back(std::move(s));   // MOVE — s is now unspecified/likely empty
// libstdc++ (tested): the moved-from std::string is empty, for short (SSO) and long strings alike.
// The standard only says "valid but unspecified" — don't rely on it.

// std::cout << s;   // legal, but s's content should not be relied upon here
```

`push_back` is overloaded for both `const T&` (copy) and `T&&` (move).
`std::move(s)` selects the move overload.

---

## Example 3 — moving a return value out of a function

```cpp
std::vector<int> buildLargeVector() {
    std::vector<int> result(1'000'000, 42);
    return result;   // usually elided/moved automatically (RVO / NRVO)
}

std::vector<int> data = buildLargeVector();   // no manual std::move needed here
```

**Do NOT do this:**
```cpp
std::vector<int> buildLargeVector() {
    std::vector<int> result(1'000'000, 42);
    return std::move(result);   // ❌ usually PESSIMIZES — blocks Return Value
                                  //    Optimization (RVO), which the compiler
                                  //    would otherwise apply for free
}
```

Returning a local variable by value already triggers RVO/NRVO or an
implicit move in modern C++ — wrapping it in `std::move` can actually
prevent the compiler's more efficient RVO path. **Never call `std::move`
on a local variable in a `return` statement.**

Counted with a type that logs its copies and moves (GCC 13, `-std=c++23`):

| Code | copies | moves |
|------|--------|-------|
| `Cnt c; return c;` (NRVO) | 0 | 0 |
| `Cnt c; return std::move(c);` | 0 | **1** |
| `return Cnt{};` (prvalue) | 0 | 0 |
| `Cnt t = std::move(makeCnt());` | 0 | **1** |

GCC warns about both bad forms with `-Wall`: `moving a local object in a return statement prevents copy elision [-Wpessimizing-move]`.
Elision of a returned prvalue is **guaranteed** since C++17 (it works even if copy and move are deleted — tested); NRVO of a named local
is not guaranteed, but if it isn't applied the compiler still treats the `return` as a move.
`std::move` in a return statement is only right when you return something that is *not* a plain local variable —
e.g. a data member (`return std::move(m_buffer);`).

---

## Example 4 — moving unique_ptr (ownership transfer)

```cpp
#include <memory>

std::unique_ptr<Buffer> makeBuffer() {
    return std::make_unique<Buffer>(1000);
}

std::unique_ptr<Buffer> owner1 = makeBuffer();
std::unique_ptr<Buffer> owner2 = std::move(owner1);   // ownership transferred

// owner1 is now nullptr — this is REQUIRED, not just "unspecified"
if (!owner1) {
    std::cout << "owner1 no longer owns anything\n";
}
```

`std::unique_ptr` **cannot be copied at all** — its copy constructor is
deleted. `std::move` is the *only* way to transfer a `unique_ptr` between
variables, into a container, or into a function parameter.

---

## When to use std::move

✅ Transferring ownership of a resource you no longer need (heap buffers,
file handles, `unique_ptr`)
✅ Passing a large object into a container/function when you don't need
the original afterward (`push_back(std::move(x))`)
✅ Implementing your own move constructor / move assignment operator
✅ Swapping/reordering elements without deep copies

❌ On a local variable in a `return` statement — blocks RVO, use plain
`return result;`
❌ On a `const` object — `std::move` on a `const T` silently falls back
to a copy, since a `const T&&` can't bind to the `T&&` parameter of the move
constructor (it binds to `const T&`, the copy constructor; tested: `copies=1 moves=0`).
If the type has *no* usable copy constructor, you get a compile error instead
❌ On an object you still need afterward — the moved-from object is left
in a valid-but-unspecified state; reading its contents is a logic bug,
even though it's not undefined behavior
❌ On the return value of a function that already returns a temporary
(`std::move(makeString())` — it is already an rvalue; the cast is redundant **and** it
turns a guaranteed-elided construction into an extra move: tested `moves=1`, and GCC warns
`moving a temporary object prevents copy elision`)

---

## The Rule of 5 — where move constructors come from

```cpp
class Resource {
public:
    Resource(const Resource&);              // copy constructor
    Resource& operator=(const Resource&);   // copy assignment
    Resource(Resource&&) noexcept;          // move constructor
    Resource& operator=(Resource&&) noexcept; // move assignment
    ~Resource();                             // destructor
};
```

If a class manages a resource (raw pointer, file handle, socket) and
defines **any one** of these five, it should almost always define **all
five** explicitly (or `= default`/`= delete` them deliberately) — this is
the **Rule of 5**. Better still: own resources through members such as `std::vector` /
`std::unique_ptr` and declare none of the five (**Rule of 0**).

What the compiler does for you:

- Declaring a **move constructor or move assignment** makes the implicit copy operations *deleted*.
- Declaring a **copy constructor, copy assignment or destructor** suppresses the implicit move operations, so
  `std::move(x)` silently **copies**. Tested: a class with only a user-written copy constructor, and a class with only a
  user-written destructor, both reported `copies=1` for `T b = std::move(a);`. `std::move` is what lets calling code actually invoke
the move constructor/move assignment operator instead of falling back to
the (usually more expensive) copy versions.

| Operation | Signature | Triggered by |
|---|---|---|
| Copy constructor | `T(const T&)` | `T b = a;` (lvalue) |
| Move constructor | `T(T&&)` | `T b = std::move(a);` (and e.g. `v.push_back(makeT())`). `T b = makeT();` needs **no** constructor at all in C++17 and later — guaranteed elision |
| Copy assignment | `T& operator=(const T&)` | `b = a;` (lvalue) |
| Move assignment | `T& operator=(T&&)` | `b = std::move(a);` |

---

## Common mistakes

| Mistake | Why it's a problem |
|---|---|
| `return std::move(localVar);` | Blocks RVO — almost always *slower*, not faster |
| Using a variable after `std::move`-ing it | Its state is valid but unspecified — undefined *content*, not undefined *behavior* |
| `std::move` on a `const` object | Silently degrades to a copy — `const T&&` won't bind to `T&&` move constructor parameters (compile error if there is no copy constructor either) |
| Forgetting `noexcept` on move constructors | `std::vector` and other containers won't use your move constructor during reallocation unless it's `noexcept` — they'll fall back to copying to preserve the strong exception guarantee (tested: 8 × `emplace_back` → 7 moves with a `noexcept` move constructor, 7 copies without). A *move-only* type is moved regardless, because there is no copy to fall back to |
| Moving from an object you still read afterward | Logic bug — the moved-from object's contents are unreliable |

---

## Quick reference

```cpp
#include <utility>

T a = ...;

T b = a;                 // COPY — a is unchanged
T c = std::move(a);      // MOVE — a is left valid-but-unspecified

container.push_back(a);              // copies a
container.push_back(std::move(a));   // moves a

std::unique_ptr<T> p2 = std::move(p1);  // REQUIRED — unique_ptr can't be copied

// Inside a move constructor:
MyClass(MyClass&& other) noexcept
    : m_data(other.m_data) {
    other.m_data = nullptr;   // leave source in a safe, destructible state
}
```

---

## When to use — summary

✅ Ownership transfer (`unique_ptr`, resource-owning classes)
✅ Avoiding deep copies of large objects you no longer need
✅ Writing move constructors/move assignment operators
✅ Populating containers from temporaries or objects you're done with

❌ `return`-ing a local variable (let RVO handle it)
❌ On `const` objects
❌ When you still need the original object's value afterward

---

## The rest of `<utility>`

`std::move` is the headline, but `<utility>` is a small grab-bag header with
several other tools worth knowing. They show up constantly in generic code.

### `std::pair`

A simple two-element bundle. The building block behind `std::map`'s
key/value entries.

```cpp
#include <utility>

std::pair<std::string, int> p{"Kostas", 30};

std::cout << p.first << " is " << p.second << "\n";

auto [name, age] = p;   // structured bindings (C++17) — COPIES p; use `auto& [n, a] = p;` to refer to it

auto p2 = std::make_pair(1, 2.5);   // deduces std::pair<int, double>
```

---

### `std::swap`

Exchanges the contents of two objects, using move construction/assignment
under the hood instead of a manual copy-based swap.

```cpp
int x = 1, y = 2;
std::swap(x, y);   // x == 2, y == 1

std::vector<int> v1{1, 2, 3}, v2{4, 5};
std::swap(v1, v2);   // cheap — just swaps internal pointers, no element copies
```

The standard containers have a member `.swap()` and **overloads of `std::swap` in namespace `std`**, which overload
resolution prefers over the generic template — that is why `std::swap(v1, v2)` is cheap. For *your own* type with its own
`swap` in its namespace, `std::swap(a, b)` does **not** call it (tested: it used the generic move-based swap). To pick up a custom swap
use the two-step idiom, or `std::ranges::swap`:

```cpp
using std::swap;   // fallback
swap(a, b);        // ADL finds ns::swap(T&, T&) if it exists, otherwise std::swap
```

---

### `std::forward` — perfect forwarding

Used inside **forwarding references** (`T&&` in a template) to pass an
argument along *exactly as it was received* — as an lvalue if it came in as
an lvalue, as an rvalue if it came in as an rvalue. Unlike `std::move`,
`std::forward` is conditional: it does not unconditionally cast to an
rvalue.

```cpp
void inner(const std::string&) { std::cout << "lvalue overload\n"; }   // must be declared BEFORE wrapper
void inner(std::string&&)      { std::cout << "rvalue overload\n"; }

template<typename T>
void wrapper(T&& arg) {
    inner(std::forward<T>(arg));   // preserves arg's original value category
}

std::string s = "hi";
wrapper(s);              // T deduced as std::string&  -> "lvalue overload"
wrapper(std::string{});  // T deduced as std::string   -> "rvalue overload"
```

(In the original order, with `wrapper` first, this does not compile: `'inner' was not declared in this scope, and no
declarations were found by argument-dependent lookup` — `inner` is in the global namespace, not in `std`.)

**Rule of thumb:** use `std::move` on things you own and are done with; use
`std::forward` only on a `T&&` template parameter, to relay the caller's
original value category onward.

---

### `std::exchange`

Replaces a value and returns the *old* value in one step. Extremely handy
inside hand-written move constructors/move assignment operators.

```cpp
int a = 10;
int old = std::exchange(a, 20);   // old == 10, a == 20
```

```cpp
// Rewriting the Buffer move constructor with std::exchange:
Buffer(Buffer&& other) noexcept
    : m_size(std::exchange(other.m_size, 0)),
      m_data(std::exchange(other.m_data, nullptr)) {
    std::cout << "MOVE constructed (via std::exchange)\n";
}
```

This is a common, idiomatic alternative to writing `other.m_data = nullptr;`
as a separate statement — it reads as "grab the old value, and leave `0`/
`nullptr` behind" in a single expression.

---

### `std::move_if_noexcept`

Used internally by the standard library (and available for your own code)
to decide, at compile time, whether it is *safe* to move an object or
whether it should be copied instead — based on whether the type's move
constructor is `noexcept`.

```cpp
template<typename T>
T maybeMove(T& obj) {
    return std::move_if_noexcept(obj);
    // returns T&&       if T's move constructor is noexcept, or if T cannot be copied
    // returns const T&  (forcing a copy) if the move may throw AND T is copyable
}
```

Checked with `decltype`: `noexcept` move → `T&&`; throwing move + copyable → `const T&`; throwing move + move-only → `T&&`.

This is exactly why the **"forgetting `noexcept`" mistake** listed earlier
matters: containers like `std::vector` use this same logic during
reallocation to preserve the strong exception guarantee.

---

### C++20 safe integer comparisons — `std::cmp_*`

Comparing a signed and an unsigned integer with `<`, `>`, etc. is a classic
footgun — the signed value gets silently converted to unsigned first, which
can produce nonsensical results for negative numbers. `std::cmp_equal`,
`std::cmp_less`, `std::cmp_greater`, `std::cmp_less_equal`,
`std::cmp_greater_equal`, and `std::cmp_not_equal` compare the *mathematical*
values correctly regardless of signedness.

```cpp
int a = -1;
unsigned b = 0;

a < b;                    // ❌ false! -1 converts to a huge unsigned value,
                           //    so the comparison silently comes out wrong
std::cmp_less(a, b);      // ✅ true — compares mathematically: -1 IS less than 0
```

GCC warns about the first line with `-Wall`/`-Wextra` (`-Wsign-compare`). The `cmp_*` functions only accept integer
types: `std::cmp_less(true, 1)` is rejected (`static assertion failed`) — `bool` and character types are not allowed.

---

### `std::in_place` tags

Marker types (`std::in_place`, `std::in_place_type`, `std::in_place_index`)
that tell `std::optional`, `std::variant`, and `std::any` to construct their
contained object **directly in place**, rather than constructing a
temporary and then moving/copying it in.

```cpp
#include <optional>

// Without in_place: Buffer(1000) is constructed, then MOVED into the optional
std::optional<Buffer> o1 = Buffer(1000);

// With in_place: Buffer is constructed directly inside the optional's storage
std::optional<Buffer> o2(std::in_place, 1000);   // no move/copy at all
```

Counted (tested): `optional<T> o = T();` → 1 move; `optional<T> o(std::in_place);` → 0 copies, 0 moves.
`main.cpp` Example 9 prints both: `Constructed Buffer(100)` only for the `in_place` one, and
`Constructed Buffer(50)` followed by `MOVE constructed` for the temporary.

---

### Quick reference — the rest of `<utility>`

| Utility | Purpose |
|---|---|
| `std::pair` | Bundle two values together (`.first` / `.second`) |
| `std::swap` | Exchange two objects' contents via moves, not copies |
| `std::forward<T>` | Preserve a forwarding reference's original value category |
| `std::exchange` | Replace a value, return the old one — great in move ctors |
| `std::move_if_noexcept` | Move only if the move constructor is `noexcept`, else copy |
| `std::cmp_equal` / `cmp_less` / ... (C++20) | Compare signed vs. unsigned safely |
| `std::in_place*` tags | Construct directly inside `optional`/`variant`/`any` — no extra move |

---

## Notes moved from main.cpp

- `main.cpp` uses `std::println` and `std::` everywhere (no `using namespace std;`, no `std::cout`); `size_t` became `std::size_t`
  and the missing `<algorithm>` (for `std::copy`) and `<cstddef>` includes were added.
- All nine examples contain real code, so none were removed. The top title comment and the `=== Example n ===` headings are kept
  and every example has a `// === EXAMPLE n: ... ===` comment.
- The `std::boolalpha` call is gone because `std::println` prints `bool` as `true`/`false` anyway.
- The explanatory remarks stay in the code as short comments; the longer reasoning (when not to `std::move`, Rule of 5,
  `noexcept`, `in_place`) lives in this README.

## Changes to the program

- `Buffer` now has copy and move **assignment** operators (it had a copy constructor, a move constructor and a destructor,
  but this README itself says to define all five). Example 1 gained two lines using them (`COPY assigned`, `MOVE assigned`).
- Example 9 gained a contrast line, `std::optional<Buffer> o1 = Buffer(50);`, which shows the extra `MOVE constructed`.
- Example 8 still triggers `-Wsign-compare` on purpose (that is the point of the demo).
- Everything else prints exactly what the original printed (`diff` shows only the added lines).

## Corrections to the original README

- The `std::forward` snippet did not compile (`inner` declared after the template) — reordered.
- "`std::swap` calls the ADL `swap` overload when one exists" is wrong: the standard containers have overloads *in `std`*; for your own
  type you need `using std::swap; swap(a, b);`.
- `T b = makeT();` is not a "move constructor" case in C++17 and later; it is guaranteed copy elision.
- `std::move(makeString())` is not "a no-op": it adds a move and blocks elision.
- "`const` → copy" is only half the story: with no copy constructor it does not compile.
- User-declared copy constructor / destructor also suppress the implicit move operations — added.
- Quick reference had "unchanged, unchanged" (duplicated word).
- The `Buffer` snippet used `size_t` and `std::copy` without the includes (`<cstddef>`, `<algorithm>`).

## What was tested

GCC 13.3, `-std=c++23 -Wall -Wextra -Wshadow -fsanitize=address,undefined` (plus leak detection), with a small test-only
`<print>` shim (GCC 13 has no `<print>`): clean build apart from the intentional `-Wsign-compare`, no sanitizer or leak reports; output compared with
the original by `diff`. A separate program with copy/move counters checked: NRVO, `return std::move(local)`, prvalue elision (also with deleted copy
and move), `std::move` of a prvalue, `std::move` of a `const`, a class with only a copy constructor or only a destructor, `std::string` moved-from
state, vector growth with/without `noexcept` and for a move-only type, `move_if_noexcept` result types, `std::swap` vs ADL swap,
`optional` with/without `in_place`, structured binding copy vs reference, moved-from `unique_ptr`, the `std::forward` compile error,
`cmp_less(true, 1)`, and the `-Wpessimizing-move` warnings.

## Not tested

- Behavior on other compilers / standard libraries (the moved-from `std::string` state, NRVO decisions).
- Timing of copy vs move for the 1 000 000-int `Buffer` (no benchmark was run; the README claims are about behavior, not speed).
- Real `<print>` from GCC 14+.
