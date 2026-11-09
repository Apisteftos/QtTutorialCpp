# Concepts and requires

Named constraints on template arguments.

## Features

| Feature | Header | Standard |
|---|---|---|
| concept, requires expression, requires clause | (language) | C++20 |
| `std::integral, floating_point, same_as, convertible_to, derived_from, invocable` | `<concepts>` | C++20 |

## What the example shows

- Example 1: overloading by concept
- Example 2: own concepts
- Example 3: requires clause

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/03_concepts
```

Or without CMake:

```bash
g++ -std=c++20 -Wall -Wextra main.cpp -o 03_concepts
./03_concepts
```

## Output (GCC 15.2.0)

```text
=== Example 1: integral vs floating_point ===
half(9)   = 4
half(9.0) = 4.5

=== Example 2: own concepts ===
value: 42
value: text
size: 3
size: 4
Printable<int>: 1, Printable<Plain>: 0
HasSize<std::vector<int>>: 1, HasSize<int>: 0

=== Example 3: requires clause ===
doubleIt(21) = 42
static_asserts with std::derived_from and std::invocable passed
```

## Notes

- `std::print` / `std::println` need C++23, so this C++20 example prints with `std::cout`.
- The commented-out lines show calls that would fail to compile with a short constraint error.

## Tested

- Built with CMake and with `g++ -std=c++20 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| concept, requires expression, requires clause | (language) | C++20 |
| `std::integral, floating_point, same_as, convertible_to, derived_from, invocable` | `<concepts>` | C++20 |
