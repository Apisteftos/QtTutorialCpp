# Singleton — `03_EagerSingleton`

> **Intent:** Ensures that a class has exactly one instance and provides a global access point to it.

## What this example contains

Sections in `main.cpp`:

- TYPE 1: EAGER INITIALIZATION (CLASSIC)
- TYPE 2: LAZY INITIALIZATION (NOT THREAD-SAFE)
- TYPE 3: THREAD-SAFE SINGLETON (WITH MUTEX)
- DEMONSTRATION FUNCTIONS
- MAIN

Types defined in `main.cpp`:

| Type | Derives from |
|---|---|
| `EagerSingleton` | — |
| `LazySingleton` | — |
| `ThreadSafeSingleton` | — |

## The pattern in short

- A private constructor and deleted copy operations.
- A static accessor that creates or returns the one instance.

**Use it when:** Exactly one instance is genuinely required (for example a hardware resource).

**Watch out:** It is global state: it hides dependencies and makes unit testing harder. Initialisation strategy matters for thread safety.

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`), so you need GCC 14+ or a recent Clang/MSVC with a standard library that ships `<print>`.

```bash
g++ -std=c++23 -Wall -Wextra -pthread main.cpp -o 03_EagerSingleton
./03_EagerSingleton
```

The included `CMakeLists.txt` sets C++23 but also asks CMake to find Qt Core. `main.cpp` itself does not use Qt, so the plain `g++` command above is enough.

## What was tested

Checked with GCC 13.3 using a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra` without warnings.
- Runs to completion (exit code 0) and prints 56 lines.
- Clean under AddressSanitizer + UBSan (no errors, no leaks reported).
- Output differs between two runs (thread ids, timing or random values are printed), so exact output cannot be compared.
- The output of the `std::cout` → `std::println` conversion matched the original. Afterwards the lecture text that `main()` used to print was moved into *Notes moved from main.cpp* below. I compared the output before and after that move: the only difference is that the moved lines are no longer printed (values that change on every run, such as addresses, thread ids or timings, were ignored)..

## Not tested

- Behaviour on Windows/macOS or with MSVC/Apple Clang.
- The CMake build (Qt is not installed in the sandbox).
- Thread timing: results with different core counts or schedulers may differ.

## Changes made to `main.cpp`

- `std::cout` replaced by `std::println` / `std::print`; no `"\n"` at line ends, `std::println("")` for blank lines.
- `using namespace std;` removed; standard names are written with `std::`.
- Types that only had a stream `operator<<` get a `std::formatter` (or the address is cast to `const void*`) because `std::println` cannot use `operator<<`.

## Notes moved from main.cpp

This text used to be printed by `main()`. It is lecture material, so it lives here and the program only prints the demo output.

### ⭐ RECOMMENDED: Use Meyers' Singleton! (closing summary)
