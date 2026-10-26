# MallocVsNew — C vs C++ Memory Allocation

This project compares C-style (`malloc`/`free`) and C++-style (`new`/`delete`)
heap memory allocation, and explains when and why each is used.

> **Status marks**
> ✅ *tested* = compiled and run with GCC 13.3 (`-Wall -Wextra`, AddressSanitizer, UBSan, LeakSanitizer).
> ⚠️ *not tested* = from the language rules or common practice, not run here.
> "Undefined behavior" means the standard promises nothing. The behavior I observed is listed so you can see what it looks like, **not** as a guarantee.

## 📚 Table of Contents
- [The core difference](#the-core-difference)
- [Example 1 — malloc: raw memory only](#example-1--malloc-raw-memory-only)
- [Example 2 — new: allocation + construction](#example-2--new-allocation--construction)
- [Example 3 — calloc: zeroed memory](#example-3--calloc-zeroed-memory)
- [Example 4 — Arrays](#example-4--arrays)
- [Example 5 — realloc: resize memory](#example-5--realloc-resize-memory)
- [Never mix malloc/free with new/delete](#never-mix-mallocfree-with-newdelete)
- [Example 6 — Placement new](#example-6--placement-new)
- [Example 7 — Handling allocation failure](#example-7--handling-allocation-failure)
- [Full comparison table](#full-comparison-table)
- [When to use which](#when-to-use-which)
- [Quick rules](#quick-rules)
- [Compilation](#compilation)
- [Notes moved from main.cpp](#notes-moved-from-maincpp)

---

## The core difference

| | `malloc` / `free` | `new` / `delete` |
|--|------------------|-----------------|
| Language | C (also valid C++) | C++ |
| Header | `<cstdlib>` | built-in; placement new and `std::nothrow` need `<new>` ✅ |
| Returns | `void*` — must cast | Typed pointer — no cast |
| Calls constructor | ❌ | ✅ |
| Calls destructor | ❌ | ✅ |
| On failure | Returns `nullptr` | Throws `std::bad_alloc` |
| Resize | `realloc()` | No equivalent — manual |
| Array variant | `malloc(n * sizeof(T))` | `new T[n]` / `delete[] ptr` |

✅ Placement new without `#include <new>` does not compile (`no matching function for call to 'operator new(sizetype, unsigned char [4])'`). `main.cpp` includes `<new>` explicitly. (In the original file it only worked because `<iostream>` pulls `<new>` in.)

---

## Example 1 — malloc: raw memory only

```cpp
Sensor* s = static_cast<Sensor*>(malloc(sizeof(Sensor)));
// s points to raw bytes — Sensor constructor was NOT called
// s->read();   // UNDEFINED BEHAVIOR — object does not exist yet

free(s);        // releases memory — Sensor destructor NOT called
```

`malloc` only allocates bytes. It has no knowledge of types, constructors,
or destructors. The memory it returns is **uninitialized** (indeterminate contents, not guaranteed zero).
✅ `sizeof(Sensor)` is 40 bytes on this machine (a `std::string` plus a `double`); the program prints it.

What `main.cpp` shows: no `[Sensor] Constructor` and no `[Sensor] Destructor` line appears, because neither was called.

---

## Example 2 — new: allocation + construction

```cpp
Sensor* s = new Sensor("TempSensor", 36.6);
// 1. allocates sizeof(Sensor) bytes
// 2. calls Sensor constructor with the arguments
// s is fully constructed and safe to use

s->read();   // safe

delete s;
// 1. calls Sensor destructor
// 2. releases memory
```

`new` is a **two-step operation** — allocate + construct.
`delete` is also two steps — destruct + deallocate.

---

## Example 3 — calloc: zeroed memory

```cpp
int* arr = static_cast<int*>(calloc(5, sizeof(int)));
// allocates 5 * sizeof(int) bytes
// ALL bytes set to zero — arr[0..4] == 0

free(arr);
```

`calloc(count, size)` is equivalent to `malloc(count * size)` followed by `memset(ptr, 0, ...)`.
Useful when you need guaranteed zero-initialization of a raw buffer.
(Implementations also guard against `count * size` overflowing, which a hand-written `malloc(count * size)` does not. ⚠️ not tested.)

There is no function with that name in C++, but `new` can zero-initialize:

```cpp
int* a = new int[5]{};   // all zeros
int* b = new int[5]();   // all zeros
delete[] a;
delete[] b;
```

✅ Tested: `new int[5]{}`, `new int[5]()`, `std::make_unique<int[]>(5)` and `std::vector<int>(5)` all give `0 0 0 0 0`. (`new int[5]` without `{}` or `()` leaves the values uninitialized.)

---

## Example 4 — Arrays

```cpp
// C style — NO constructors, NO destructors
Sensor* cArr = static_cast<Sensor*>(malloc(3 * sizeof(Sensor)));
// objects do not exist — do not use them
free(cArr);   // no destructors

// C++ style — constructors AND destructors
Sensor* cppArr = new Sensor[3];   // default constructor called 3 times
delete[] cppArr;                  // destructor called 3 times, then memory freed
```

### CRITICAL: `delete` vs `delete[]`

```cpp
int* arr = new int[5];
delete  arr;    // WRONG — undefined behavior
delete[] arr;   // CORRECT
```

The old text here said that `delete arr` "only frees the first element". That is not a correct description: the behavior is simply **undefined**. What I observed (✅ tested):

- AddressSanitizer: `alloc-dealloc-mismatch (operator new [] vs operator delete)`;
- for `int`, a build without sanitizers shows nothing at all;
- for a class with a destructor (`delete p` on `new S[3]`), the program aborted with `munmap_chunk(): invalid pointer`, and AddressSanitizer reported `attempting free on address which was not malloc()-ed`. (`new[]` typically stores bookkeeping in front of the elements, which plain `delete` does not expect. ⚠️ implementation detail.)

Always pair:
```
new     →  delete
new[]   →  delete[]
malloc  →  free
```

---

## Example 5 — realloc: resize memory

`realloc` resizes a `malloc`-allocated block, preserving existing data:

```cpp
int* arr = static_cast<int*>(malloc(3 * sizeof(int)));
arr[0]=10; arr[1]=20; arr[2]=30;

// IMPORTANT: assign to temp first — realloc may return nullptr on failure
int* temp = static_cast<int*>(realloc(arr, 6 * sizeof(int)));
if (temp != nullptr) {
    arr = temp;   // safe to reassign now
    arr[3]=40; arr[4]=50; arr[5]=60;
} else {
    // arr still valid — realloc failed, original untouched
}
free(arr);
```

### Why use `temp` and not assign directly?

```cpp
arr = static_cast<int*>(realloc(arr, 6 * sizeof(int)));
// If realloc returns nullptr: arr is now nullptr AND the original memory is leaked!
```

✅ Tested: an impossible `realloc` size assigned directly back to the pointer is reported by LeakSanitizer as `Direct leak of 12 byte(s)`.

Always assign the `realloc` result to a **temporary**, check it, then reassign.

> `realloc` copies **bytes**. Only use it for plain data such as `int` or `char`. For a class like `Sensor` (which owns a `std::string`) a byte copy is undefined behavior. ⚠️ language rule, not run here.

### C++ equivalent of realloc

There is no `realloc` for `new[]`. You must do it manually:

```cpp
int* oldArr = new int[3]{10, 20, 30};
int* newArr = new int[6];
std::copy(oldArr, oldArr + 3, newArr);   // copy old data (needs <algorithm>)
delete[] oldArr;                          // free old
oldArr = newArr;                          // reassign
```

This is why `std::vector` exists — it handles growth automatically and correctly.

---

## Never mix malloc/free with new/delete

This has no code in `main.cpp` (it is undefined behavior and the program would be a bad thing to run), so it is described here. All three are **undefined behavior**:

```cpp
Sensor* p = new Sensor("Test", 1.0);
free(p);          // WRONG — destructor never called

Sensor* q = static_cast<Sensor*>(malloc(sizeof(Sensor)));
delete q;         // WRONG — destructor runs on an object that was never constructed

int* arr = new int[5];
delete arr;       // WRONG — must use delete[]
```

✅ What I observed, with a small class that holds a `std::string`:

| Case | AddressSanitizer | Without sanitizer |
|------|------------------|-------------------|
| `new` → `free` | `alloc-dealloc-mismatch (operator new vs free)` | no output of the destructor; no visible failure |
| `malloc` → `delete` | `SEGV` (destructor runs on garbage) | the destructor ran; no visible crash this time |
| `new int[5]` → `delete` | `alloc-dealloc-mismatch (operator new [] vs operator delete)` | no visible failure |
| `new int[5]` → `free` | `alloc-dealloc-mismatch (operator new [] vs free)` | — |
| `new S[3]` → `delete` | `attempting free on address which was not malloc()-ed` | `munmap_chunk(): invalid pointer` |

So "may appear to work in debug builds but crash on other platforms" is true in spirit: three of the four plain runs showed nothing wrong. The old text's claims "resource leak" and "→ crash" were too specific; the only guaranteed statement is that the behavior is undefined. In the `new` → `free` case the leak happens when the object owns something: here the `std::string` had a short name that fits inside the object, so there was nothing to leak.

---

## Example 6 — Placement new

Placement new constructs an object **in memory you already own**:

```cpp
void* buffer = malloc(sizeof(Sensor));

// construct in existing memory — no new allocation
Sensor* s = new (buffer) Sensor("Placed", 42.0);   // needs #include <new>
s->read();   // fully constructed, safe to use

// MUST call destructor manually — do NOT use delete
s->~Sensor();

free(buffer);   // free the underlying memory separately
```

Memory from `malloc` is suitably aligned for any ordinary type. For over-aligned types use `std::aligned_alloc` or an `alignas` buffer. ⚠️ not tested.

### When is placement new useful?
- **Memory pools** — pre-allocate a large block, construct objects in it
- **Embedded systems** — control exactly where objects live in memory
- **Shared memory** — construct objects in shared memory segments
- **Serialization / network buffers** — construct objects directly in a pre-allocated buffer

(Standard containers such as `std::vector` do exactly this internally.)

---

## Example 7 — Handling allocation failure

### Normal `new` — throws on failure
```cpp
try {
    int* p = new int[999999999999LL];   // throws std::bad_alloc
} catch (const std::bad_alloc& e) {
    std::println("Failed: {}", e.what());
}
```

### `new (std::nothrow)` — returns nullptr on failure
```cpp
#include <new>

int* p = new (std::nothrow) int[999999999999LL];
if (p == nullptr) {
    std::println("Allocation failed");
} else {
    delete[] p;
}
```

✅ Tested on Linux x86-64 (request of about 4 TB): `new` threw `std::bad_alloc`, `new (std::nothrow)` returned `nullptr`, and `malloc` returned `nullptr`. Whether such a request fails depends on the system (overcommit settings, address-space limits), so a smaller request may succeed.

⚠️ **Under AddressSanitizer** the same line does not throw: ASan aborts with `requested allocation size ... exceeds maximum supported size`. This is why the `new int[999999999999LL]` line stays commented out in `main.cpp`: the demo also runs with sanitizers. The `catch` block in the demo is therefore never entered; it only shows where the handler goes.

Use `std::nothrow` in environments where exceptions are disabled (for example builds with `-fno-exceptions`). ⚠️ not tested here. (The old text also named "some Qt configurations"; I could not check that, so it is not repeated.)

---

## Full comparison table

| Operation | C (malloc family) | C++ (new family) | Modern C++ |
|-----------|------------------|-----------------|------------|
| Single object | `malloc(sizeof(T))` | `new T(args)` | `std::make_unique<T>(args)` |
| Array | `malloc(n*sizeof(T))` | `new T[n]` | `std::vector<T>(n)` |
| Zeroed array | `calloc(n, sizeof(T))` | `new T[n]{}` ✅ | `std::vector<T>(n, 0)` ✅ |
| Resize | `realloc(ptr, newSize)` | manual copy | `vector.resize(n)` |
| Free single | `free(ptr)` | `delete ptr` | automatic |
| Free array | `free(ptr)` | `delete[] ptr` | automatic |
| Constructor | ❌ | ✅ | ✅ |
| Destructor | ❌ | ✅ | ✅ |
| Type safe | ❌ | ✅ | ✅ |
| Exception on fail | ❌ (returns nullptr) | ✅ (throws) | ✅ |

`std::vector<int>(n)` already zero-initializes; the `, 0` is only for clarity ✅.

---

## When to use which

### Use `new`/`delete` when:
- You need a single heap object with a known type and cannot use a smart pointer
- Implementing data structures (linked lists, trees) by hand

### Use `malloc`/`free` when:
- Interfacing with C libraries that expect `malloc`-allocated memory (they will call `free` on it)
- Allocating raw byte buffers (serialization, network frames)
- Storage for placement new
- The type has no constructor/destructor (plain old data)

### Use smart pointers / containers (preferred in modern C++)
```cpp
auto p   = std::make_unique<Sensor>("s", 1.0);   // single object
auto arr = std::vector<Sensor>(10);               // array
```
These combine the correctness of `new`/`delete` with automatic lifetime management —
no manual `delete` needed, and the memory is released even if an exception is thrown.

---

## Quick rules

1. In C++, **prefer `new` over `malloc`** for objects with constructors (and prefer smart pointers and containers over both)
2. **Always pair** `malloc↔free`, `new↔delete`, `new[]↔delete[]`
3. **Never mix** — no `free` on `new`, no `delete` on `malloc`
4. Always assign `realloc` result to a **temp variable** before checking
5. With placement new — **call destructor manually**, do NOT use `delete`
6. In modern C++ — **prefer smart pointers and containers** over raw `new`/`delete`

---

## Compilation

`main.cpp` uses `std::println` (`<print>`), so it needs **C++23 and GCC 14+** (or another standard library that ships `<print>`):

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o malloc_vs_new
./malloc_vs_new
```

To see the undefined-behavior cases from this file yourself, build a copy with `-fsanitize=address,undefined`.

---

## Notes moved from main.cpp

`main.cpp` has no top description block, so nothing was removed there. What changed:

- `std::cout` → `std::println` / `std::print`; `#include <print>` added and `<iostream>` removed. `<algorithm>` was added (`std::copy` only compiled through a transitive include), and the unused `<cstring>` was removed.
- The `// ====` comment banners over each example were replaced with `// === EXAMPLE n: … ===` title comments.
- **Old Example 6 ("Never mix malloc/free with new/delete") only printed lecture text and code snippets** → [Never mix malloc/free with new/delete](#never-mix-mallocfree-with-newdelete). Because of that, the examples in `main.cpp` are now numbered 1–7: old Example 7 (placement new) is **6** and old Example 8 (`std::nothrow`) is **7**. The headings in this file use the same numbers.
- The program output of the other examples is unchanged (✅ compared line by line against the original run).
- The explanation of why `huge = new int[...]` stays commented out was added as a code comment (see [Example 7](#example-7--handling-allocation-failure)).

### Corrections to the original README

1. "`delete arr` only frees the first element" → undefined behavior (see [Example 4](#example-4--arrays)).
2. "`free(p)` on a `new` object → resource leak" and "`delete p` on `malloc` memory → crash" → both undefined; observed results are in the table above.
3. The comparison table said `delete[] T`; the correct syntax is `delete[] ptr`.
4. Placement new and `std::nothrow` need `#include <new>` (the README listed `new` as "built-in").
5. `std::copy` needs `<algorithm>`.
6. "Memory pools / embedded / shared memory / **MCX/protocol work**": the last item was replaced by neutral "serialization / network buffers".
7. Added the warning that `realloc` is only for plain data.

### Tested

✅ `g++ -std=c++23 -Wall -Wextra` with AddressSanitizer + UBSan: no warnings, no errors; LeakSanitizer: no leaks.
✅ New output identical to the old output (old Example 6 removed, Examples 7/8 renumbered).
✅ Zero-initialization forms, `realloc` leak, `bad_alloc` / `nothrow` / `malloc` failure on a huge request, the five mixed-allocation cases, placement new without `<new>`.
⚠️ Not tested: `-fno-exceptions` builds, over-aligned types, `std::println` with GCC 14 itself (the sandbox has GCC 13; a small stand-in header with the same behavior was used).

---
