# MoveSemantics — std::move in C++23

`std::move` doesn't move anything by itself. It's a **cast** — it tells the
compiler "treat this lvalue as an rvalue," which makes it *eligible* to be
moved from instead of copied. The actual moving happens in whatever
function receives it (a move constructor, move assignment operator, or a
container's internal logic).

---

## Header

```cpp
#include <utility>
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
#include <iostream>
#include <string>
#include <utility>

class Buffer {
public:
    explicit Buffer(size_t size) : m_size(size), m_data(new int[size]) {
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
    size_t m_size;
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

---

## Example 2 — moving into a container

```cpp
std::vector<std::string> names;
names.reserve(3);

std::string s = "a very long string that would be expensive to copy...";

names.push_back(s);              // COPY — s is still valid and unchanged after
names.push_back(std::move(s));   // MOVE — s is now unspecified/likely empty

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
to a copy, since a `const T&&` still can't bind to a non-const move
constructor
❌ On an object you still need afterward — the moved-from object is left
in a valid-but-unspecified state; reading its contents is a logic bug,
even though it's not undefined behavior
❌ On the return value of a function that already returns a temporary
(`std::move(makeString())` — that's already an rvalue, `std::move` is a
no-op here)

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
the **Rule of 5**. `std::move` is what lets calling code actually invoke
the move constructor/move assignment operator instead of falling back to
the (usually more expensive) copy versions.

| Operation | Signature | Triggered by |
|---|---|---|
| Copy constructor | `T(const T&)` | `T b = a;` (lvalue) |
| Move constructor | `T(T&&)` | `T b = std::move(a);` or `T b = makeT();` |
| Copy assignment | `T& operator=(const T&)` | `b = a;` (lvalue) |
| Move assignment | `T& operator=(T&&)` | `b = std::move(a);` |

---

## Common mistakes

| Mistake | Why it's a problem |
|---|---|
| `return std::move(localVar);` | Blocks RVO — almost always *slower*, not faster |
| Using a variable after `std::move`-ing it | Its state is valid but unspecified — undefined *content*, not undefined *behavior* |
| `std::move` on a `const` object | Silently degrades to a copy — `const T&&` won't bind to `T&&` move constructor parameters |
| Forgetting `noexcept` on move constructors | `std::vector` and other containers won't use your move constructor during reallocation unless it's `noexcept` — they'll fall back to copying to preserve the strong exception guarantee |
| Moving from an object you still read afterward | Logic bug — the moved-from object's contents are unreliable |

---

## Quick reference

```cpp
#include <utility>

T a = ...;

T b = a;                 // COPY — a is unchanged, unchanged
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

auto [name, age] = p;   // structured bindings (C++17)

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

Most standard containers/types also provide a member `.swap()` and a
matching free `swap()` found via ADL — `std::swap` calls that overload when
one exists, which is why it's efficient even for big objects.

---

### `std::forward` — perfect forwarding

Used inside **forwarding references** (`T&&` in a template) to pass an
argument along *exactly as it was received* — as an lvalue if it came in as
an lvalue, as an rvalue if it came in as an rvalue. Unlike `std::move`,
`std::forward` is conditional: it does not unconditionally cast to an
rvalue.

```cpp
template<typename T>
void wrapper(T&& arg) {
    inner(std::forward<T>(arg));   // preserves arg's original value category
}

void inner(const std::string&) { std::cout << "lvalue overload\n"; }
void inner(std::string&&)      { std::cout << "rvalue overload\n"; }

std::string s = "hi";
wrapper(s);              // T deduced as std::string&  -> "lvalue overload"
wrapper(std::string{});  // T deduced as std::string   -> "rvalue overload"
```

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
    // returns T&& if T's move constructor is noexcept
    // returns const T& (forcing a copy) otherwise
}
```

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
