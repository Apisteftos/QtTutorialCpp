# C++ Custom Deleters - Complete Guide

Custom deleters give `unique_ptr` and `shared_ptr` specialized cleanup logic for resources that are **not** released with a plain `delete`: C API handles, `malloc` memory, arrays, borrowed pointers, or anything that needs logging or bookkeeping on release.

> **How to read the status marks in this file**
> ✅ *tested* = compiled and run with GCC 13.3 (`-std=c++23 -Wall -Wextra`, AddressSanitizer, UBSan).
> ⚠️ *not tested* = explained from the language rules or common practice, not run here (platform APIs like Windows/sqlite, speed).

## 📚 Table of Contents
- [What are custom deleters?](#what-are-custom-deleters)
- [The problem: default deleters](#the-problem-default-deleters)
- [Three deleter styles](#three-deleter-styles)
- [unique_ptr vs shared_ptr deleters](#unique_ptr-vs-shared_ptr-deleters)
- [C API resource management](#c-api-resource-management)
- [Array deleters](#array-deleters)
- [No-op deleters](#no-op-deleters)
- [Stateful deleters](#stateful-deleters)
- [Deleter type deduction](#deleter-type-deduction)
- [Size and performance](#size-and-performance)
- [Common use cases](#common-use-cases)
- [Common mistakes](#common-mistakes)
- [Best practices](#best-practices)
- [Key takeaways](#key-takeaways)
- [Compilation](#compilation)
- [Notes moved from main.cpp](#notes-moved-from-maincpp)

---

## What are custom deleters?

```cpp
std::unique_ptr<T, DeleterType> up(resource, deleter);   // deleter is part of the TYPE
std::shared_ptr<T>              sp(resource, deleter);   // deleter lives in the control block
```

A deleter is any callable that takes the pointer: a lambda, a function pointer, a functor, `std::function`.

| | `unique_ptr<T, D>` | `shared_ptr<T>` |
|---|---|---|
| Where the deleter lives | inside the `unique_ptr` object | in the control block |
| Part of the type? | **yes** | **no** (type-erased) |
| Assign pointers with different deleters | ❌ compile error ✅ | ✅ compiles ✅ |
| Extra size for a stateless deleter | none ✅ | none (always 16 bytes) ✅ |

✅ = tested (see [Tested](#tested)).

---

## The problem: default deleters

The default deleter calls `delete` (or `delete[]` for `unique_ptr<T[]>`). That is wrong when the resource is released some other way.

| # | Code | What goes wrong | Tested? |
|---|------|-----------------|---------|
| 1 | `FILE* f = fopen(...); unique_ptr<FILE> p(f);` | compiles, but destructor runs `delete f` instead of `fclose(f)` (undefined behavior) | ✅ compiles with GCC; the UB itself is by the language rules |
| 2 | `int* a = new int[10]; unique_ptr<int> p(a);` | `delete` instead of `delete[]` | ✅ AddressSanitizer: `alloc-dealloc-mismatch (operator new [] vs operator delete)` |
| 3 | `malloc` + `unique_ptr<int>` | `delete` on malloc memory | ✅ AddressSanitizer: `alloc-dealloc-mismatch (malloc vs operator delete)` |
| 3b | `unique_ptr<void> p(malloc(100));` | **does not compile** (`static assertion failed: can't delete pointer to incomplete type`), because you cannot `delete` a `void*` | ✅ |
| 4 | Need logging / metrics / notification on release | plain `delete` does none of that | by definition |
| 5 | Non-owning observation of a global or stack object | smart pointer would `delete` something it does not own | ✅ see Mistake 3 |

> The original `main.cpp` text said that `unique_ptr<void>` "calls delete not free()". That is not what happens: it does not compile. The fix is the same (give it a deleter, see Example 1/5/7 in `main.cpp`).

---

## Three deleter styles

### Lambda (Example 1)

```cpp
auto fileDeleter = [](FILE* fp) { fclose(fp); };
std::unique_ptr<FILE, decltype(fileDeleter)> file(f, fileDeleter);
```
✅ inline, can capture context, type-safe  ·  ⚠️ the type has to be spelled with `decltype`.

### Function pointer (Example 2)

```cpp
void fileFree(FILE* f) { if (f) fclose(f); }
std::unique_ptr<FILE, decltype(&fileFree)> file(f, fileFree);
```
✅ reusable and named, easy to unit test  ·  the pointer is stored inside the `unique_ptr`, so it is **16 bytes**, not 8 (✅ tested).

### Functor (Example 3)

```cpp
struct FileDeleter { void operator()(FILE* f) const { if (f) fclose(f); } };
std::unique_ptr<FILE, FileDeleter> file(f);          // no deleter argument needed
```
✅ stateless functors add no size (8 bytes, ✅ tested), can be a template (`ArrayDeleter` works for any `T`), and can carry state (`LoggingDeleter` holds a `std::string`, making the `unique_ptr` 40 bytes, ✅ tested).

---

## unique_ptr vs shared_ptr deleters

```cpp
// unique_ptr: different deleter = different type
std::unique_ptr<FILE, FileDeleter>   a(f1);
std::unique_ptr<FILE, MallocDeleter> b(f2);
// a = move(b);                    // ❌ no match for operator=   (✅ tested)

// shared_ptr: same type whatever the deleter
std::shared_ptr<FILE> c(f1, FileDeleter{});
std::shared_ptr<FILE> d(f2, MallocDeleter{});
c = d;                             // ✅ compiles                (✅ tested)
```

| | unique_ptr | shared_ptr |
|---|---|---|
| Different deleters → same type | no | yes |
| Cost | none for stateless deleters (✅ sizes tested) | control block holds the deleter; the call is indirect (⚠️ speed not measured) |
| Use when | performance-critical, single ownership | flexibility, shared ownership |

Also: `make_shared` cannot take a deleter (✅ compile error). Use the `shared_ptr<T>(ptr, deleter)` constructor or `allocate_shared`.

---

## C API resource management

```cpp
using FilePtr = std::unique_ptr<FILE, int (*)(FILE*)>;
FilePtr f(fopen("data.txt", "r"), fclose);
```

| Resource | Acquire / release |
|----------|-------------------|
| `FILE*` | `fopen` / `fclose` |
| `void*` | `malloc` / `free` |
| socket `int` | `socket` / `close` |
| Windows `HANDLE` | `CreateFile` / `CloseHandle` |
| `sqlite3*` | `sqlite3_open` / `sqlite3_close` |

Only the first two are compiled and run in `main.cpp` (✅). Socket, `HANDLE` and `sqlite3` are ⚠️ not tested here (platform libraries not available).

**`decltype(&fclose)` vs `int (*)(FILE*)`:** both are the same type, but GCC 13 prints
`warning: ignoring attributes on template argument 'int (*)(FILE*)' [-Wignored-attributes]` for `decltype(&fclose)` (✅ tested). `main.cpp` therefore spells the type as `int (*)(FILE*)` to compile warning-free. Also ⚠️ since C++20 the standard does not guarantee that you may take the address of a standard library function; it works on the common compilers, but wrapping `fclose` in your own function or lambda is the portable choice.

**Order of destruction:** two local `unique_ptr`s are destroyed in reverse order of declaration (language rule; the program's output does not show it because the `fclose` deleter prints nothing).

---

## Array deleters

Arrays need `delete[]`. In order of preference:

1. `vector<T>` (best)
2. `unique_ptr<T[]>` (calls `delete[]`)
3. `unique_ptr<T, custom_deleter>`
4. **Never** `unique_ptr<T>` for an array (✅ AddressSanitizer reports the mismatch)

---

## No-op deleters

For observing something you do not own (global, stack object, borrowed pointer):

```cpp
int value = 42;
std::shared_ptr<int> observer(&value, [](int*) {});      // does nothing on release
std::unique_ptr<int, NoOpDeleter> p(&globalResource);
```

⚠️ The object must outlive every observer, otherwise the observers dangle. A raw pointer or reference already says "I do not own this" more clearly; a no-op deleter is for APIs that force a smart pointer on you.

Without the no-op deleter, `shared_ptr<int> p(&value);` on a stack variable fails at runtime: ✅ AddressSanitizer `attempting free on address which was not malloc()-ed`.

---

## Stateful deleters

A deleter with data members (name, counter, pool reference) works, but for `unique_ptr` the data lives inside every `unique_ptr`.

✅ Measured sizes (GCC 13, 64-bit): see the table in [Size and performance](#size-and-performance).

Use cases: metrics, logging with context, returning to a resource pool, debug tracking.

---

## Deleter type deduction

A lambda's type has no name. Options for `unique_ptr`:

| Option | Code | Size of the `unique_ptr` (✅ tested) |
|--------|------|-----------------------|
| `decltype` | `unique_ptr<int, decltype(del)> p(new int, del);` | 8 (stateless lambda) |
| function pointer via unary `+` | `auto del = +[](int* p){ delete p; };` then `decltype(del)` | 16 |
| `std::function` | `unique_ptr<int, function<void(int*)>>` | 40 (libstdc++) |
| `shared_ptr` | `shared_ptr<int> p(new int, [](int* q){ delete q; });` | 16, no type to write |

Recommendation: `decltype` or a function pointer for `unique_ptr`; just pass the lambda to `shared_ptr`. `std::function` is the largest option (40 bytes, ✅); its call-speed cost is ⚠️ not measured.

---

## Size and performance

✅ Measured with GCC 13.3, 64-bit (Example 10 prints these):

| Type | Bytes |
|------|-------|
| `int*` | 8 |
| `unique_ptr<int>` (default deleter) | 8 |
| `unique_ptr<int, EmptyDeleter>` (stateless functor) | 8 |
| `unique_ptr<int, stateless lambda>` | 8 |
| `unique_ptr<int, void (*)(int*)>` (function pointer) | **16** |
| `unique_ptr<int, StatefulDeleter>` (one `int`) | 16 |
| `unique_ptr<int, capturing lambda>` (one `int` captured) | 16 |
| `unique_ptr<int, function<void(int*)>>` | 40 |
| `unique_ptr<int, LoggingDeleter>` (holds a `std::string`) | 40 |
| `unique_ptr<int, MetricsDeleter>` (`string` + pointer) | 48 |
| `shared_ptr<int>` | 16 (two pointers: object + control block; same for any deleter) |

The empty-base optimization is why stateless deleters cost nothing.

**Corrected claim:** the original code printed `sizeof(unique_ptr<int, decltype(&fpDeleter)>)` and said "Still pointer-sized!". It is **16 bytes**, not 8: a function pointer is data stored in the `unique_ptr`. (`&fpDeleter` was also the address of a lambda *object*, not a function pointer, so that line did not show what it claimed.) It now uses a real function pointer type.

**Speed:** the old text claimed that a function-pointer deleter "cannot inline" and is "slightly slower", and that `shared_ptr` deleters are "slower". ⚠️ None of that was measured. The call goes through a stored pointer (or the control block), so it *can* cost an indirect call, but a compiler can sometimes see through it, and the deleter is usually dwarfed by what it frees. Benchmark before deciding.

**Null pointers:** the deleter is **not** called for a null `unique_ptr`, but it **is** called for a `shared_ptr` that was constructed with a null pointer and a deleter. ✅ tested (0 calls versus 1 call). So `if (f)` inside a deleter is a must for shared_ptr, and harmless for unique_ptr.

---

## Common use cases

Illustrative patterns (⚠️ not compiled here unless marked):

```cpp
// File handles                                     ✅ (Example 5)
std::unique_ptr<FILE, int (*)(FILE*)> file(fopen("data.txt", "r"), fclose);

// malloc memory                                    ✅ (Example 1, 5)
auto freeDeleter = [](void* p) { free(p); };
std::unique_ptr<void, decltype(freeDeleter)> mem(malloc(1024), freeDeleter);

// Arrays                                           ✅
std::unique_ptr<int[]> arr(new int[100]);   // better: vector<int> arr(100);

// Database connection                              ⚠️ needs sqlite3
auto closeDb = [](sqlite3* db) { sqlite3_close(db); };
std::unique_ptr<sqlite3, decltype(closeDb)> db(handle, closeDb);

// Socket                                           ⚠️ needs POSIX; see note below
auto closeSock = [](int* s) { close(*s); delete s; };
std::unique_ptr<int, decltype(closeSock)> sock(new int(fd), closeSock);

// Windows HANDLE                                   ⚠️ Windows only; see note below
// Resource pool                                    ⚠️ capture makes the unique_ptr 16 bytes (✅ size)
auto back = [&pool](Resource* r) { pool.release(r); };
std::unique_ptr<Resource, decltype(back)> res(pool.acquire(), back);

// Logging                                          ✅ same idea as Example 1 / 3
auto logged = [](Widget* w) { LOG("Deleting widget"); delete w; };
```

Notes on the platform snippets: the socket version heap-allocates an `int` just to have a pointer to manage. A deleter with a `pointer` typedef avoids that. On Windows `HANDLE` is already a pointer (`void*`), so `unique_ptr<void, decltype(&CloseHandle)>` is more direct than allocating a `HANDLE*`; note that failed `CreateFile` returns `INVALID_HANDLE_VALUE`, not null. Both ⚠️ not tested.

---

## Common mistakes

| # | Mistake | Result | Tested? |
|---|---------|--------|---------|
| 1 | `unique_ptr<int> p(new int[10]);` | `delete` instead of `delete[]`; use `unique_ptr<int[]>` or `vector` | ✅ ASan |
| 2 | `auto d = [](int* p){ delete p; }; unique_ptr<int> p(new int, d);` | **compile error** (`no matching function`); the type must be `unique_ptr<int, decltype(d)>` | ✅ |
| 3 | `int v = 42; shared_ptr<int> p(&v);` | runtime error freeing a stack address; use a no-op deleter | ✅ ASan |
| 4 | `unique_ptr<FILE> p(f);` | `delete` instead of `fclose`; use `unique_ptr<FILE, int (*)(FILE*)> p(f, fclose)` | ✅ compiles, UB by rules |
| 5 | A very large stateful deleter in a `unique_ptr` | every `unique_ptr` carries it: a 1024-byte deleter gives a **1032-byte** `unique_ptr` | ✅ |
| 6 | Capturing `this` in a deleter that outlives the object | see below | ✅ |
| 7 | `p1 = move(p2)` with two different lambda deleters | **compile error** (`no match for operator=`) because each lambda has its own type | ✅ |

**Mistake 6, corrected.** The old text said "`this` might be gone". More precisely: a member `unique_ptr<Resource, function<void(Resource*)>>` with a `[this]`-capturing deleter is fine while the `Manager` stays where it is (note that the deleter then runs while `Manager` is already being destroyed, so members declared *after* the pointer are already gone). The real danger is moving the owner: the moved `unique_ptr` carries the lambda with the **old** `this`. ✅ tested: `Manager b = std::move(*a); delete a;` then destroying `b` gives AddressSanitizer `heap-use-after-free` inside the lambda. Prefer a deleter that does not depend on the owner's address, or make the owner non-movable.

Fixes: use the same deleter type (a named functor or one `using` alias), or use `shared_ptr` when you need to mix deleters.

---

## Best practices

1. **Prefer stateless deleters** with `unique_ptr`: no size cost (✅ 8 bytes), no state to manage.
2. **Use a type alias**: `using FilePtr = unique_ptr<FILE, int (*)(FILE*)>;`. Everything that takes or returns the pointer then uses the same type (this also avoids Mistake 7).
3. **`shared_ptr` for flexibility** (type-erased deleter, copyable); **`unique_ptr` for exclusive ownership.**
4. **Prefer standard containers** over custom deleters when they fit: `vector<int> arr(100);` instead of `unique_ptr<int[]>`.
5. **Factory functions** that return the smart pointer keep creation and deleter together:
   ```cpp
   std::unique_ptr<FILE, int (*)(FILE*)> makeFile(const std::string& name) {
       return {fopen(name.c_str(), "r"), fclose};
   }
   ```
6. **No-op for observation only** and only when the object clearly outlives the pointer. The old snippet `shared_ptr<T> ptr(stack_obj, [](T*){})` was missing the `&`: it must be `ptr(&stack_obj, ...)`.
7. **Document ownership** in the type (`unique_ptr<FILE, ...>` = "I close this file").
8. **Check for null in the deleter** when it may be given null (always for `shared_ptr` with a deleter, see [Size and performance](#size-and-performance)); `fclose(nullptr)` is undefined behavior.
9. **Test every deletion path**: normal exit, exception, early return. Example 11 shows the exception path and the destructor-based rollback.
10. A deleter must not throw (it runs in a destructor). ⚠️ stated by the standard's requirements, not tested here.

---

## Key takeaways

1. **Custom deleters** = cleanup beyond `delete` / `delete[]`: C APIs, arrays, logging, non-owning pointers.
2. **unique_ptr**: the deleter is part of the type. Stateless = no extra size; function pointer or capturing = 16 bytes; `std::function` = 40 bytes.
3. **shared_ptr**: the deleter is type-erased in the control block; pointers with different deleters have the same type and can be assigned.
4. **Three styles**: lambda (inline), function pointer (reusable), functor (stateful or templated).
5. **Type deduction**: `decltype(deleter)` for `unique_ptr`; pass the lambda directly to `shared_ptr`.
6. **Patterns**: `FILE*` → `unique_ptr<FILE, int (*)(FILE*)>`; `malloc` → lambda with `free`; arrays → `unique_ptr<T[]>` or `vector<T>`; observation → no-op deleter.
7. **RAII for any resource**: files, locks, sockets, handles, connections, transactions (Example 11).

---

## Compilation

`main.cpp` uses `std::println` (`<print>`), so it needs **C++23 and GCC 14+** (or a standard library that ships `<print>`):

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o custom_deleters
./custom_deleters
```

The original comment block said `-std=c++17`; that was true for the `cout` version. The deleter techniques themselves (everything except `println`) work in C++17 as well, since lambdas as deleters, `unique_ptr<T[]>` and `+[]` are available there.

The examples write small files to `/tmp` (`test_deleter.txt`, `test_func.txt`, `test_functor.txt`, `test_capi.txt`, `input.txt`, `output.txt`, `database.txt`, `database2.txt`). On Windows, change these paths.

### Expected output (start)

```
╔══════════════════════════════════════════════════════════╗
║                                                          ║
║           CUSTOM DELETERS - COMPLETE GUIDE               ║
║                                                          ║
╚══════════════════════════════════════════════════════════╝

╔════════════════════════════════════╗
║  EXAMPLE 1: LAMBDA DELETERS        ║
╚════════════════════════════════════╝
...
```

Addresses printed in Examples 1, 2, 3, 5 and 8 change from run to run.

---

## Notes moved from main.cpp

The title/description comment at the top of `main.cpp` ("WHAT ARE CUSTOM DELETERS?") stays. What changed:

- `std::cout` → `std::println`; `#include <print>` added, `<iostream>` removed, `<stdexcept>` added (the code used `runtime_error` without it).
- Examples are now numbered **1–11** (four lecture-only examples were removed from the program):
  - old Example 1 (default deleter problems), old 12 (common use cases), old 13 (common mistakes) and old 14 (best practices) only printed code snippets as text → [The problem](#the-problem-default-deleters), [Common use cases](#common-use-cases), [Common mistakes](#common-mistakes), [Best practices](#best-practices).
  - old 2 → **1**, 3 → **2**, 4 → **3**, 5 → **4**, 6 → **5**, 7 → **6**, 8 → **7**, 9 → **8**, 10 → **9**, 11 → **10**, 15 → **11**.
- **Printed lecture text removed from the examples** (all of it now lives in this file):
  - the "Benefits" / "WHEN TO USE" / "RECOMMENDATIONS" / "WARNING" / "Common C API Patterns" / "Stateful Deleter Use Cases" lists → the sections above;
  - printed code of the wrong versus right usage in old Examples 5, 7 and 10 → [unique_ptr vs shared_ptr](#unique_ptr-vs-shared_ptr-deleters), [Array deleters](#array-deleters), [Deleter type deduction](#deleter-type-deduction);
  - the old Example 11 text (size and speed bullets) → [Size and performance](#size-and-performance); Example 10 now only prints the measured `sizeof` values;
  - the "Example 1:", "Example 2:" sub-headings inside each example lost their numbers so they cannot be confused with the example numbers.
- The final **KEY TAKEAWAYS** block printed by `main()` → [Key takeaways](#key-takeaways).
- The comment block at the end of `main.cpp` (COMPILATION / RUN / TOPICS COVERED / OUTPUT) → [Compilation](#compilation) and the table of contents.
- `FileWrapper::FilePtr` and `DatabaseHandle::DBPtr` use `int (*)(FILE*)` instead of `decltype(&fclose)` (see [C API](#c-api-resource-management)).

### Bugs found while testing

1. **Example 15 (now 11) never showed a successful transaction.** It opened `/tmp/database.txt` with `"r+"`, which fails when the file does not exist, so the program printed `❌ Error: Failed to open database` in both runs, and the "committed" and "rolled back" paths were never reached. Fixed by opening with `"w+"` (creates the file). Output now shows: opened → started → executed → committed, and for the failing run: executed → `Rolling back transaction` (from the destructor, *before* the catch block prints the error) → error.
2. **"Still pointer-sized" was wrong** for a function-pointer deleter (16 bytes, see above), and the line measured `decltype(&lambdaObject)` instead of a function pointer type.
3. **`unique_ptr<void>` is not "calls delete"**: it does not compile (see [The problem](#the-problem-default-deleters)).
4. **Mistake 6 (`this` capture)** blamed the wrong thing; the verified failure is moving the owner (see [Common mistakes](#common-mistakes)).
5. **Best practice 7** was missing `&` (`stack_obj` → `&stack_obj`).
6. The printed claim "Both files closed (reverse order)" was removed from the program output because the program does not show the order; the rule is described above instead.
7. `<stdexcept>` was missing (`runtime_error` compiled only through other headers).

### Tested

✅ `g++ -std=c++23 -Wall -Wextra` with AddressSanitizer + UBSan: no warnings, no errors, no leaks (LeakSanitizer clean). The only change to the original demo output is Example 11 (see bug 1) and the sizes in Example 10, which also lists extra types. Lines with addresses were compared after masking the address.
✅ Compile-error claims: `unique_ptr<void>`, `unique_ptr<int>` with a lambda deleter argument, assignment between `unique_ptr`s with different deleters, `make_shared` with a deleter.
✅ `shared_ptr` assignment with different deleters compiles; `shared_ptr` of a stack address aborts under ASan; `new[]`/`malloc` released by `delete` are flagged by ASan.
✅ All `sizeof` values in the table (GCC 13.3, x86-64; `std::function` and `std::string` sizes are library-specific).
✅ Null handling: `unique_ptr` does not call the deleter for null, `shared_ptr` with a deleter does.
✅ `[this]` capture + moved owner → heap-use-after-free.
⚠️ Not tested: sqlite3, sockets, Windows `HANDLE`, resource-pool snippets, any speed claim, `std::println` output with GCC 14 itself (the sandbox has GCC 13; a small stand-in header with the same `println` behavior was used).

---
