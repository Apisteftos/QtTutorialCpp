# tuple-like conversions, constexpr unique_ptr and bitset

Construct `pair` / `tuple` from other tuple-like objects, and use `unique_ptr` and `bitset` in constant expressions.

## Features

| Feature | Header | Standard |
|---|---|---|
| `pair / tuple constructors from tuple-like` | `<utility>, <tuple>` | C++23 |
| `constexpr std::unique_ptr` | `<memory>` | C++23 |
| `constexpr std::bitset` | `<bitset>` | C++23 |

## What the example shows

- Example 1: pair and tuple from tuple-like types
- Example 2: constexpr unique_ptr and bitset

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/07_tuple_like_and_constexpr
```

Or without CMake:

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 07_tuple_like_and_constexpr
./07_tuple_like_and_constexpr
```

## Output (GCC 15.2.0)

```text
=== Example 1: tuple-like conversions ===
pair from array: (3, 4)
pair from tuple: (7, "seven")
tuple from pair: (1, 2)

=== Example 2: constexpr unique_ptr and bitset ===
compile time: unique_ptr gave 42, bitset count 6
```

## Notes

- The allocation of the `unique_ptr` is created and released inside the constant expression; it must not escape it.

## Tested

- Built with CMake (Debug) and with `g++ -std=c++23 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `pair / tuple constructors from tuple-like` | `<utility>, <tuple>` | C++23 |
| `constexpr std::unique_ptr` | `<memory>` | C++23 |
| `constexpr std::bitset` | `<bitset>` | C++23 |
