# std::source_location and <bit>

Where am I, and bit manipulation.

## Features

| Feature | Header | Standard |
|---|---|---|
| `std::source_location` | `<source_location>` | C++20 |
| `std::bit_cast, popcount, countl_zero, countr_zero, rotl, has_single_bit, bit_ceil, bit_floor, bit_width, endian` | `<bit>` | C++20 |

## What the example shows

- Example 1: source_location
- Example 2: bit operations
- Example 3: bit_cast and endian

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/02_source_location_and_bit
```

Or without CMake:

```bash
g++ -std=c++20 -Wall -Wextra main.cpp -o 02_source_location_and_bit
./02_source_location_and_bit
```

## Output (GCC 15.2.0)

```text
=== Example 1: source_location ===
[int main() line 26] from main
[void worker() line 20] working
file name ends with main.cpp: 1

=== Example 2: bit operations ===
popcount(0b10110) = 3
countl_zero       = 27
countr_zero       = 1
has_single_bit(64)= 1, (65)= 0
bit_ceil(100)     = 128
bit_floor(100)    = 64
bit_width(255)    = 8
rotl(0x80000001, 1) = 0x3

=== Example 3: bit_cast, endian ===
bits of 1.0f: 0x3f800000
little endian machine: 1
```

## Notes

- `std::print` / `std::println` need C++23, so this C++20 example prints with `std::cout`.
- `source_location::current()` as a default argument gives the position of the **caller**.

## Tested

- Built with CMake and with `g++ -std=c++20 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `std::source_location` | `<source_location>` | C++20 |
| `std::bit_cast, popcount, countl_zero, countr_zero, rotl, has_single_bit, bit_ceil, bit_floor, bit_width, endian` | `<bit>` | C++20 |
