# constexpr vector, string and algorithms

Containers and algorithms inside constant expressions.

## Features

| Feature | Header | Standard |
|---|---|---|
| `constexpr std::vector, std::string` | `<vector>, <string>` | C++20 |
| `constexpr algorithms (sort, accumulate)` | `<algorithm>, <numeric>` | C++20 |

## What the example shows

- Example 1: vector in a constant expression
- Example 2: string
- Example 3: algorithms

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/02_constexpr_containers
```

Or without CMake:

```bash
g++ -std=c++20 -Wall -Wextra main.cpp -o 02_constexpr_containers
./02_constexpr_containers
```

## Output (GCC 15.2.0)

```text
=== Example 1: constexpr vector ===
sum of squares 1..10 computed at compile time: 385

=== Example 2: constexpr string ===
words: 4

=== Example 3: constexpr sort ===
2 3 5 7 11 
```

## Notes

- `std::print` / `std::println` need C++23, so this C++20 example prints with `std::cout`.
- The memory must be released inside the constant expression. A `constexpr` variable cannot hold a `std::vector`.

## Tested

- Built with CMake and with `g++ -std=c++20 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `constexpr std::vector, std::string` | `<vector>, <string>` | C++20 |
| `constexpr algorithms (sort, accumulate)` | `<algorithm>, <numeric>` | C++20 |
