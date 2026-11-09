# import std and import std.compat

Use the whole standard library through a module instead of many `#include` lines.

## Features

| Feature | Header | Standard |
|---|---|---|
| import std; | (module) | C++23 |
| import std.compat; | (module) | C++23 |

## What the example shows

- Example 1: use the standard library without includes
- Example 2: ranges and expected come with it

## Build and run

```bash
cmake -G Ninja -B build -DCMAKE_CXX_COMPILER=g++-15
cmake --build build
./build/01_import_std
```

## Output (GCC 15.2.0)

```text
=== Example 1: import std ===
sorted: [1, 3, 5, 8]
Ann is 31
Ben is 27

=== Example 2: everything is available ===
expected holds: 42
evens: [2, 4, 6, 8]
```

## Notes

- Needs **CMake 3.30 or newer (tested with 4.2.3), the Ninja generator and GCC 15** (`/usr/include/c++/15/bits/std.cc` must exist).
- In CMake 4.2 the feature is still experimental: `CMAKE_EXPERIMENTAL_CXX_IMPORT_STD` has to be set to the documented gate value **before** `project()`. The CMakeLists.txt does this.
- With GCC 15.2.0 the pipe operator on a view (`range | views::filter(...)`) failed inside this module build with "use of operator| before deduction of auto". The example calls the adaptor as a function instead. Normal `#include` builds are not affected.
- `std.compat` also exports the global C names (`::printf`, `::size_t`). Only `import std;` is used in the example.

## Tested

- Built with CMake (Debug) and with `g++ -std=c++23 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Built with CMake 4.2.3 + Ninja 1.13.2 and GCC 15.2.0: no warnings.
- Not tested: `import std` with GCC 16.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| import std; | (module) | C++23 |
| import std.compat; | (module) | C++23 |
