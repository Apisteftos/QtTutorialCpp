# std::span

A non-owning view of contiguous memory.

## Features

| Feature | Header | Standard |
|---|---|---|
| `std::span, first, last, subspan, as_bytes` | `<span>` | C++20 |

## What the example shows

- Example 1: one function for every contiguous container
- Example 2: sub-spans
- Example 3: fixed extent and bytes

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/01_span
```

Or without CMake:

```bash
g++ -std=c++20 -Wall -Wextra main.cpp -o 01_span
./01_span
```

## Output (GCC 15.2.0)

```text
=== Example 1: span as parameter ===
vector: 21, array: 60, C array: 28

=== Example 2: first, last, subspan ===
first(2): 3, last(2): 11, subspan(1, 3): 9
1 2 0 0 5 6 

=== Example 3: static extent, as_bytes ===
size known at compile time: 3
bytes: 12, size_bytes(): 12
```

## Notes

- `std::print` / `std::println` need C++23, so this C++20 example prints with `std::cout`.
- A span does not own the elements. It becomes invalid when the vector reallocates or is destroyed.

## Tested

- Built with CMake and with `g++ -std=c++20 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `std::span, first, last, subspan, as_bytes` | `<span>` | C++20 |
