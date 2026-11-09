# std::unreachable and std::stacktrace

`std::unreachable()` marks a code path that can never run. `std::stacktrace` captures the call chain at run time.

## Features

| Feature | Header | Standard |
|---|---|---|
| `std::unreachable` | `<utility>` | C++23 |
| `std::stacktrace, stacktrace_entry` | `<stacktrace>` | C++23 |

## What the example shows

- Example 1: std::unreachable
- Example 2: std::stacktrace

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/02_unreachable_and_stacktrace
```

Or without CMake:

```bash
g++ -std=c++23 -Wall -Wextra -lstdc++exp main.cpp -o 02_unreachable_and_stacktrace
./02_unreachable_and_stacktrace
```

## Output (GCC 15.2.0)

```text
=== Example 1: unreachable ===
turn right from north: east
turn right from west:  north

=== Example 2: stacktrace ===
captured at least 4 frames (level3, level2, level1, main): true
default constructed stacktrace is empty: true
```

## Notes

- **GCC 14 and 15 need `-lstdc++exp` when linking** (the CMakeLists.txt does this). Without it the linker reports an undefined reference to `std::__stacktrace_impl::_S_current`. This was checked on GCC 15.2.0.
- Calling `std::unreachable()` on a path that is really reached is undefined behaviour. The example only keeps it after a `switch` that handles every enumerator.

## Tested

- Built with CMake (Debug) and with `g++ -std=c++23 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `std::unreachable` | `<utility>` | C++23 |
| `std::stacktrace, stacktrace_entry` | `<stacktrace>` | C++23 |
