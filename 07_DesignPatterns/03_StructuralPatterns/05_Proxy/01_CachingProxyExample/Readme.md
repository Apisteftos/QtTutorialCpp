# Proxy — `01_CachingProxyExample`

> **Intent:** Provides a stand-in object that controls access to another object (caching, protection, lazy creation, remoting).

## What this example contains

Types defined in `main.cpp`:

| Type | Derives from |
|---|---|
| `IDatabase` | — |
| `RealDatabase` | `IDatabase` |
| `CacheEntry` | — |
| `CachingDatabaseProxy` | `IDatabase` |
| `Application` | — |

## The pattern in short

- A subject interface.
- A real subject.
- A proxy with the same interface that decides when and how to forward calls.

**Use it when:** You need to control access to an object, delay its creation, or add caching/permissions transparently.

**Watch out:** Adds indirection and can hide expensive operations behind a cheap-looking call.

## Sample output (first lines)

```text
===== Caching Proxy Pattern Demo =====
Database Query Optimization with Caching

🔌 Connected to database: MainDB
📦 Caching proxy initialized (timeout: 5s, max size: 10)

===== Running Database Queries =====

🔄 Proxy received query: SELECT * FROM users WHERE active = 1
  ❌ CACHE MISS - querying database...
  🔍 Database processing: SELECT * FROM users WHERE active = 1
  💾 Executing query on database...
  ⏱️  Query took: 1000ms
  💾 Result cached for future use
```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`), so you need GCC 14+ or a recent Clang/MSVC with a standard library that ships `<print>`.

```bash
g++ -std=c++23 -Wall -Wextra -pthread main.cpp -o 01_CachingProxyExample
./01_CachingProxyExample
```

The included `CMakeLists.txt` sets C++23 but also asks CMake to find Qt Core. `main.cpp` itself does not use Qt, so the plain `g++` command above is enough.

## What was tested

Checked with GCC 13.3 using a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra` without warnings.
- Runs to completion (exit code 0) and prints 106 lines.
- Clean under AddressSanitizer + UBSan (no errors, no leaks reported).
- Output is identical between two consecutive runs.
- The output of the `std::cout` → `std::println` conversion matched the original. Afterwards the lecture text that `main()` used to print was moved into *Notes moved from main.cpp* below. I compared the output before and after that move: the only difference is that the moved lines are no longer printed (values that change on every run, such as addresses, thread ids or timings, were ignored).

## Not tested

- Behaviour on Windows/macOS or with MSVC/Apple Clang.
- The CMake build (Qt is not installed in the sandbox).
- Thread timing: results with different core counts or schedulers may differ.

## Changes made to `main.cpp`

- `std::cout` replaced by `std::println` / `std::print`; no `"\n"` at line ends, `std::println("")` for blank lines.
- `using namespace std;` removed; standard names are written with `std::`.

## Notes moved from main.cpp

This text used to be printed by `main()`. It is lecture material, so it lives here and the program only prints the demo output.

### Closing summary

===== Performance Summary =====

- ✓ First queries: ~1000ms (database hit)
- ✓ Cached queries: ~0ms (cache hit)
- ✓ Performance improvement: >99% for cached queries
- ✓ Cache automatically expires old entries
- ✓ LRU eviction when cache is full
