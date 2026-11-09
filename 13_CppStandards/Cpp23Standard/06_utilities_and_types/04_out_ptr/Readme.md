# out_ptr and inout_ptr

Use smart pointers with C style `T**` output parameters.

## Features

| Feature | Header | Standard |
|---|---|---|
| `std::out_ptr` | `<memory>` | C++23 |
| `std::inout_ptr` | `<memory>` | C++23 |

## What the example shows

- Example 1: out_ptr
- Example 2: inout_ptr
- Example 3: out_ptr with a shared_ptr needs a deleter

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/04_out_ptr
```

Or without CMake:

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 04_out_ptr
./04_out_ptr
```

## Output (GCC 15.2.0)

```text
=== Example 1: out_ptr ===
created handle 7

=== Example 2: inout_ptr ===
handle is now 8

=== Example 3: shared_ptr ===
shared handle 7
leaving main
destroyHandle(7)
destroyHandle(8)
```

## Notes

- `inout_ptr` releases the old pointer before the call, because the C function frees it.
- The deleter of the `unique_ptr` runs at the end of `main` for the last handle (the output shows `destroyHandle(...)` lines).

## Tested

- Built with CMake (Debug) and with `g++ -std=c++23 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `std::out_ptr` | `<memory>` | C++23 |
| `std::inout_ptr` | `<memory>` | C++23 |
