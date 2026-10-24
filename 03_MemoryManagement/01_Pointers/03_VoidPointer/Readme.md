# Void Pointers in C++23

A `void*` is a pointer that can hold the address of **any type**,
but has **no type information** attached to it.

```cpp
void* vptr;        // can point to anything

int    i = 42;
double d = 3.14;

vptr = &i;         // OK
vptr = &d;         // OK — same pointer, different type
```

(Any *object* pointer converts implicitly to `void*`; `main.cpp` Example 1 prints the addresses.)

---

## The fundamental rule (main.cpp Example 2)

**You cannot dereference a `void*` without casting it first.**
The compiler has no idea what type of data is at that address.

```cpp
void* vp = &i;

*vp;                          // ERROR — cannot dereference void*
*(static_cast<int*>(vp));     // OK    — cast first, then dereference
```

Always use `static_cast<T*>` in C++ (never C-style cast `(int*)vp`).
The type you cast back to must be the type that was stored: `static_cast<double*>` on a pointer to an `int` is undefined behavior (see *void\* limitations*).

✅ tested: `int* p = vp;` does not compile in C++ (`invalid conversion from 'void*' to 'int*'`), and a pointer to a **const** object needs `const void*` (`void* v = &constInt;` is the error `invalid conversion from 'const void*' to 'void*'`).

---

## Example 3 — Generic function parameter (main.cpp Example 3)

In C (and in C++ before templates) `void*` is the usual way to write type-generic functions:

```cpp
void printValue(void* ptr, char type) {
    switch (type) {
        case 'i': std::cout << *(static_cast<int*>(ptr));    break;
        case 'd': std::cout << *(static_cast<double*>(ptr)); break;
    }
}

printValue(&myInt,    'i');
printValue(&myDouble, 'd');
```

This is how C's `qsort`, `bsearch`, and many OS APIs work.
In modern C++ you would use **templates** instead.

---

## Example 4 — Generic swap with memcpy (main.cpp Example 4)

```cpp
void genericSwap(void* a, void* b, size_t size) {
    unsigned char temp[256];
    if (size > sizeof(temp)) return;   // otherwise the memcpy below overflows temp
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
}

int x = 10, y = 20;
genericSwap(&x, &y, sizeof(int));    // x=20, y=10

double da = 1.11, db = 9.99;
genericSwap(&da, &db, sizeof(double));
```

Works for any **trivially copyable** type (`int`, `double`, plain structs) because `memcpy` copies raw bytes — it doesn't care about type.

Limits:
- The 256-byte buffer: the original had no size check, so `size > 256` overflowed `temp`. `main.cpp` now prints a message and returns.
- It is **not** valid for types such as `std::string` or `std::vector` (they contain pointers into themselves or own heap memory). ✅ tested: swapping two short `std::string`s with this function and letting them be destroyed gives AddressSanitizer's `attempting free on address which was not malloc()-ed`. Use `std::swap` for such types.

---

## Example 5 — Memory functions (C interop) (main.cpp Example 5)

`malloc`, `calloc`, `realloc` all return `void*` — you must cast:

```cpp
void* rawMem = malloc(sizeof(int) * 4);
int*  arr    = static_cast<int*>(rawMem);   // cast required in C++
// ... use arr ...
free(rawMem);   // use free() with malloc(), not delete
```
(`malloc` / `free` are declared in `<cstdlib>`, `memcpy` in `<cstring>`.) `malloc` does not run constructors, so it is only suitable for trivial types.

`memcpy` and `memset` take `void*` parameters:

```cpp
int src[3] = {1, 2, 3};
int dst[3];
memcpy(dst, src, sizeof(src));   // copies raw bytes
```

---

## Example 6 — void* limitations (main.cpp Example 6)

```cpp
void* vp = &i;

vp++;          // ERROR in ISO C++ — no arithmetic, size unknown
vp + 1;        // ERROR in ISO C++ — same reason
*vp;           // ERROR — no dereference without cast
```
✅ tested with GCC 13: `*vp;` is the error `'void*' is not a pointer-to-object type`. `vp++` and `vp + 1` are **accepted by GCC as an extension** (it treats `sizeof(void)` as 1) and only produce a `-Wpointer-arith` warning; `-pedantic-errors` makes them errors. So "ERROR" holds for standard C++, not for every compiler flag set.

**The wrong cast:** the old `main.cpp` cast a pointer to an `int` to `double*` and read it - 8 bytes from a 4-byte object. ✅ AddressSanitizer reports `stack-buffer-overflow` for it (the original program aborted there before printing anything when redirected). It now casts to `float*` (same size, still undefined behavior because of aliasing) and prints `std::bit_cast<float>(myInt)` next to it, which is the well-defined way to reinterpret the bytes.

| Operation | `int*` | `void*` |
|-----------|--------|---------|
| Dereference `*p` | ✅ | ❌ must cast first |
| Arithmetic `p + 1` | ✅ advances by sizeof(int) | ❌ |
| Implicit conversion **to** `void*` | ✅ (object pointers; `const` needs `const void*`) | — |
| Implicit conversion **from** `void*` to `T*` | — | ❌ needs a cast in C++ (allowed in C) |
| Type safety | ✅ | ❌ |

---

## void* vs modern C++ alternatives

| Use case | Old way (void*) | Modern C++ |
|----------|----------------|------------|
| Generic function | `void* + type tag` | `template<typename T>` |
| Any value storage | `void*` | `std::any` |
| Optional value | `void*` or `nullptr` | `std::optional<T>` |
| Type-safe variant | `void* + enum` | `std::variant<T1,T2>` |
| Raw memory | `malloc/free` | `std::vector`, `std::unique_ptr` |

---

## When void* is still used in C++23

Despite its limitations, `void*` is still common in:

- **C library interop** — `pthread_create`, `dlsym`, OS callbacks
- **Qt's low-level APIs** — e.g. `QVariant::constData()` returns a `const void*`, and the meta-object calls pass `void**` argument arrays (from memory of the Qt docs, not run here)
- **Memory operations** — `memcpy`, `memset`, `memmove`
- **Plugin/callback systems** — function pointers with user data
- **Serialization** — writing raw bytes to buffers (network packets, file formats)

```cpp
// Callback with user data (common C pattern; MyClass is any class of yours)
void myCallback(void* userData) {
    MyClass* obj = static_cast<MyClass*>(userData);   // must be the type that was passed in
    obj->doSomething();
}

// The caller:
MyClass object;
registerCallback(myCallback, &object);   // registerCallback: some C API taking (void(*)(void*), void*)
```
`void*` can hold any *object* pointer; a function pointer is not guaranteed to fit in one (`dlsym` relies on a platform guarantee).

---

## Summary

| | |
|--|--|
| What it is | A pointer with no type information |
| Can point to | Any object type |
| Can dereference | ❌ — must cast first |
| Can do arithmetic | ❌ |
| Type safe | ❌ |
| Still used for | C interop, memory ops, callbacks |
| Modern alternative | Templates, `std::any`, `std::variant` |


---

## 🚀 How to run

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o void_pointers
./void_pointers
```
`main.cpp` uses `std::println` (`<print>`), so it needs GCC 14+ or a recent Clang with `-std=c++23`.

---

## 🧾 Notes on main.cpp

No lecture text to move. Changes:

- `std::cout` → `std::println` / `std::print`. `void*` is printed directly with `{}`.
- Example 6: the `double*` wrong cast read past a 4-byte `int` (AddressSanitizer: stack-buffer-overflow); now a `float*` cast plus `std::bit_cast`.
- `genericSwap`: size check for the 256-byte buffer, a comment about trivially copyable types, `std::size_t` / `std::memcpy`.
- Example 1: the unused `float f` (GCC warning) is now used (`vptr = &f`). `malloc`/`free` are `std::malloc`/`std::free` with `<cstdlib>`.
- A comment now says that `voidPtr++` is only an error in ISO C++ (GCC accepts it as an extension).
- ✅ tested with `-Wall -Wextra`, AddressSanitizer and UndefinedBehaviorSanitizer (and Clang).

---

## ⚠️ Open points

- The README's examples 1–3 did not match `main.cpp` (1–6); the headings now use the `main.cpp` numbers.
- This folder has no number or Previous/Next links.
- A serialization bullet referred to a protocol from a previous job; it now says "network packets, file formats". The Qt statements are from memory.
