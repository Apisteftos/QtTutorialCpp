# MallocVsNew — C vs C++ Memory Allocation

This project compares C-style (`malloc`/`free`) and C++-style (`new`/`delete`)
heap memory allocation, and explains when and why each is used.

---

## The core difference

| | `malloc` / `free` | `new` / `delete` |
|--|------------------|-----------------|
| Language | C (also valid C++) | C++ |
| Header | `<cstdlib>` | built-in |
| Returns | `void*` — must cast | Typed pointer — no cast |
| Calls constructor | ❌ | ✅ |
| Calls destructor | ❌ | ✅ |
| On failure | Returns `nullptr` | Throws `std::bad_alloc` |
| Resize | `realloc()` | No equivalent — manual |
| Array variant | `malloc(n * sizeof(T))` | `new T[n]` / `delete[] T` |

---

## Example 1 — malloc: raw memory only

```cpp
Sensor* s = static_cast<Sensor*>(malloc(sizeof(Sensor)));
// s points to raw bytes — Sensor constructor was NOT called
// s->read();   // UNDEFINED BEHAVIOR — object does not exist yet

free(s);        // releases memory — Sensor destructor NOT called
```

`malloc` only allocates bytes. It has no knowledge of types, constructors,
or destructors. The memory it returns is **uninitialized** (random bytes).

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

There is no direct C++ equivalent — `new int[5]{}` zero-initializes with uniform init:
```cpp
int* arr = new int[5]{};   // all zeros, C++ style
delete[] arr;
```

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
delete  arr;    // WRONG — undefined behavior, only frees first element
delete[] arr;   // CORRECT — frees the entire array
```

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

Always assign `realloc` result to a **temporary**, check it, then reassign.

### C++ equivalent of realloc

There is no `realloc` for `new[]`. You must do it manually:

```cpp
int* oldArr = new int[3]{10, 20, 30};
int* newArr = new int[6];
std::copy(oldArr, oldArr + 3, newArr);   // copy old data
delete[] oldArr;                          // free old
oldArr = newArr;                          // reassign
```

This is why `std::vector` exists — it handles growth automatically and correctly.

---

## Example 6 — Never mix malloc/free with new/delete

```cpp
Sensor* p = new Sensor("Test", 1.0);
free(p);          // WRONG — destructor never called → resource leak

Sensor* p = static_cast<Sensor*>(malloc(sizeof(Sensor)));
delete p;         // WRONG — destructor called on unconstructed object → crash

int* arr = new int[5];
delete arr;       // WRONG — must use delete[]
```

These are **undefined behavior** — they may appear to work in debug builds
but crash or corrupt memory in release builds or on different platforms.

---

## Example 7 — Placement new

Placement new constructs an object **in memory you already own**:

```cpp
void* buffer = malloc(sizeof(Sensor));

// construct in existing memory — no new allocation
Sensor* s = new (buffer) Sensor("Placed", 42.0);
s->read();   // fully constructed, safe to use

// MUST call destructor manually — do NOT use delete
s->~Sensor();

free(buffer);   // free the underlying memory separately
```

### When is placement new useful?
- **Memory pools** — pre-allocate a large block, construct objects in it
- **Embedded systems** — control exactly where objects live in memory
- **Shared memory** — construct objects in shared memory segments
- **MCX/protocol work** — construct message objects directly in network buffers

---

## Example 8 — Handling allocation failure

### Normal `new` — throws on failure
```cpp
try {
    int* p = new int[999999999999LL];   // throws std::bad_alloc
} catch (const std::bad_alloc& e) {
    std::cout << "Failed: " << e.what();
}
```

### `new (std::nothrow)` — returns nullptr on failure
```cpp
#include <new>

int* p = new (std::nothrow) int[999999999999LL];
if (p == nullptr) {
    std::cout << "Allocation failed\n";
} else {
    delete[] p;
}
```

Use `std::nothrow` in environments where exceptions are disabled
(embedded systems, some Qt configurations with `-fno-exceptions`).

---

## Full comparison table

| Operation | C (malloc family) | C++ (new family) | Modern C++ |
|-----------|------------------|-----------------|------------|
| Single object | `malloc(sizeof(T))` | `new T(args)` | `std::make_unique<T>(args)` |
| Array | `malloc(n*sizeof(T))` | `new T[n]` | `std::vector<T>(n)` |
| Zeroed array | `calloc(n, sizeof(T))` | `new T[n]{}` | `std::vector<T>(n, 0)` |
| Resize | `realloc(ptr, newSize)` | manual copy | `vector.resize(n)` |
| Free single | `free(ptr)` | `delete ptr` | automatic |
| Free array | `free(ptr)` | `delete[] ptr` | automatic |
| Constructor | ❌ | ✅ | ✅ |
| Destructor | ❌ | ✅ | ✅ |
| Type safe | ❌ | ✅ | ✅ |
| Exception on fail | ❌ (returns nullptr) | ✅ (throws) | ✅ |

---

## When to use which

### Use `new`/`delete` when:
- You need a single heap object with a known type
- Implementing data structures (linked lists, trees)
- C API interop requires manual lifecycle control

### Use `malloc`/`free` when:
- Interfacing with C libraries that expect `malloc`-allocated memory
- Allocating raw byte buffers (serialization, network frames)
- Placement new scenarios
- The type has no constructor/destructor (plain old data)

### Use smart pointers / containers (preferred in modern C++)
```cpp
auto p   = std::make_unique<Sensor>("s", 1.0);   // single object
auto arr = std::vector<Sensor>(10);               // array
```
These combine the correctness of `new`/`delete` with automatic lifetime management —
no manual `delete` ever needed, exception safe by default.

---

## Quick rules

1. In C++, **always prefer `new` over `malloc`** for objects with constructors
2. **Always pair** `malloc↔free`, `new↔delete`, `new[]↔delete[]`
3. **Never mix** — no `free` on `new`, no `delete` on `malloc`
4. Always assign `realloc` result to a **temp variable** before checking
5. With placement new — **call destructor manually**, do NOT use `delete`
6. In modern C++ — **prefer smart pointers and containers** over raw `new`/`delete`
