# <numbers>, midpoint, lerp, integer comparison

Math constants and small numeric helpers.

## Features

| Feature | Header | Standard |
|---|---|---|
| `std::numbers::pi, e, sqrt2, ln2` | `<numbers>` | C++20 |
| `std::midpoint, std::lerp` | `<numeric>, <cmath>` | C++20 |
| `std::cmp_less ..., std::in_range` | `<utility>` | C++20 |
| `std::ssize` | `<iterator>` | C++20 |

## What the example shows

- Example 1: mathematical constants
- Example 2: midpoint and lerp
- Example 3: safe integer comparison
- Example 4: ssize

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/04_numbers_midpoint_lerp_cmp
```

Or without CMake:

```bash
g++ -std=c++20 -Wall -Wextra main.cpp -o 04_numbers_midpoint_lerp_cmp
./04_numbers_midpoint_lerp_cmp
```

## Output (GCC 15.2.0)

```text
=== Example 1: <numbers> ===
pi      = 3.1415927
e       = 2.7182818
sqrt(2) = 1.4142136
ln(2)   = 0.69314718
pi (float) = 3.1415927

=== Example 2: midpoint, lerp ===
midpoint(max, max-2) = 2147483646   (no overflow)
midpoint(1, 4) = 2 (rounds toward the first)
lerp(10, 20, 0.25) = 12.5
lerp(10, 20, 1.5)  = 25  (extrapolates)

=== Example 3: cmp_less, in_range ===
-1 < 1u with the operator:     true (after a cast)
plain comparison -1 < 1u:      false (wrong: -1 became a huge unsigned)
std::cmp_less(-1, 1u):         true
in_range<std::uint8_t>(300):   false
in_range<std::uint8_t>(200):   true

=== Example 4: ssize ===
3 2 1 
```

## Notes

- `std::print` / `std::println` need C++23, so this C++20 example prints with `std::cout`.
- `cmp_less(-1, 1u)` compares by value, so it is `true`; the plain comparison converts -1 to a huge unsigned number.

## Tested

- Built with CMake and with `g++ -std=c++20 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `std::numbers::pi, e, sqrt2, ln2` | `<numbers>` | C++20 |
| `std::midpoint, std::lerp` | `<numeric>, <cmath>` | C++20 |
| `std::cmp_less ..., std::in_range` | `<utility>` | C++20 |
| `std::ssize` | `<iterator>` | C++20 |
