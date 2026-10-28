# Singleton — `06_MultitonSingleton`

> **Intent:** Ensures that a class has exactly one instance and provides a global access point to it.

## What this example contains

Types defined in `main.cpp`:

| Type | Derives from |
|---|---|
| `Multiton` | — |
| `Configuration` | `Multiton<Configuration, std::string>` |

## The pattern in short

- A private constructor and deleted copy operations.
- A static accessor that creates or returns the one instance.

**Use it when:** Exactly one instance is genuinely required (for example a hardware resource).

**Watch out:** It is global state: it hides dependencies and makes unit testing harder. Initialisation strategy matters for thread safety.

## Sample output (first lines)

```text
Created a new instance
Created a new instance
Database config: Database connection string: localhost:5432
Network config: Network settings: TCP/IP
Are dbConfig and dbConfigAgain the same instance? Yes
Number of instances: 2
After releasing database, instances: 1
After releasing all, instances: 0
Instance destroyed
Instance destroyed
```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`), so you need GCC 14+ or a recent Clang/MSVC with a standard library that ships `<print>`.

```bash
g++ -std=c++23 -Wall -Wextra -pthread main.cpp -o 06_MultitonSingleton
./06_MultitonSingleton
```

The included `CMakeLists.txt` sets C++23 but also asks CMake to find Qt Core. `main.cpp` itself does not use Qt, so the plain `g++` command above is enough.

## What was tested

Checked with GCC 13.3 using a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra` without warnings.
- Runs to completion (exit code 0) and prints 10 lines.
- Clean under AddressSanitizer + UBSan (no errors, no leaks reported).
- Output is identical between two consecutive runs.
- Output of the converted program is byte-identical to the original `std::cout` version.

## Not tested

- Behaviour on Windows/macOS or with MSVC/Apple Clang.
- The CMake build (Qt is not installed in the sandbox).
- Thread timing: results with different core counts or schedulers may differ.

## Changes made to `main.cpp`

- `std::cout` replaced by `std::println` / `std::print`; no `"\n"` at line ends, `std::println("")` for blank lines.
- `using namespace std;` removed; standard names are written with `std::`.

