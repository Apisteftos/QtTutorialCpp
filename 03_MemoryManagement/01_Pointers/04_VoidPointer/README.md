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

---

## The fundamental rule

**You cannot dereference a `void*` without casting it first.**
The compiler has no idea what type of data is at that address.

```cpp
void* vp = &i;

*vp;                          // ERROR — cannot dereference void*
*(static_cast<int*>(vp));     // OK    — cast first, then dereference
```

Always use `static_cast<T*>` in C++ (never C-style cast `(int*)vp`).

---

## Example 1 — Generic function parameter

Before templates existed, `void*` was the only way to write type-generic functions:

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

## Example 2 — Generic swap with memcpy

```cpp
void genericSwap(void* a, void* b, size_t size) {
    unsigned char temp[256];
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
}

int x = 10, y = 20;
genericSwap(&x, &y, sizeof(int));    // x=20, y=10

double da = 1.11, db = 9.99;
genericSwap(&da, &db, sizeof(double));
```

Works for any type because `memcpy` copies raw bytes — it doesn't care about type.

---

## Example 3 — Memory functions (C interop)

`malloc`, `calloc`, `realloc` all return `void*` — you must cast:

```cpp
void* rawMem = malloc(sizeof(int) * 4);
int*  arr    = static_cast<int*>(rawMem);   // cast required in C++
// ... use arr ...
free(rawMem);   // use free() with malloc(), not delete
```

`memcpy` and `memset` also use `void*` internally:

```cpp
int src[3] = {1, 2, 3};
int dst[3];
memcpy(dst, src, sizeof(src));   // copies raw bytes
```

---

## void* limitations

```cpp
void* vp = &i;

vp++;          // ERROR — no arithmetic, size unknown
vp + 1;        // ERROR — same reason
*vp;           // ERROR — no dereference without cast
```

| Operation | `int*` | `void*` |
|-----------|--------|---------|
| Dereference `*p` | ✅ | ❌ must cast first |
| Arithmetic `p + 1` | ✅ advances by sizeof(int) | ❌ |
| Assign to any pointer | ❌ needs cast | ✅ |
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
- **Qt internals** — `QVariant` storage, `QObject::setProperty`
- **Memory operations** — `memcpy`, `memset`, `memmove`
- **Plugin/callback systems** — function pointers with user data
- **Serialization** — writing raw bytes to buffers (MCX protocol frames)

```cpp
// Qt example — QVariant internally uses void*-like storage
QVariant v = 42;
int i = v.toInt();

// Callback with user data (common C pattern)
void myCallback(void* userData) {
    MyClass* obj = static_cast<MyClass*>(userData);
    obj->doSomething();
}
```

---

## Summary

| | |
|--|--|
| What it is | A pointer with no type information |
| Can point to | Any type |
| Can dereference | ❌ — must cast first |
| Can do arithmetic | ❌ |
| Type safe | ❌ |
| Still used for | C interop, memory ops, callbacks |
| Modern alternative | Templates, `std::any`, `std::variant` |
