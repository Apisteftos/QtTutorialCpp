# constexpr <cmath> functions

Basic math functions usable in constant expressions.

## Features

| Feature | Header | Standard |
|---|---|---|
| `floor, ceil, round, trunc, fabs, fmod, copysign, fmin, fmax` | `<cmath>` | C++23 |
| `abs (int, double)` | `<cstdlib>, <cmath>` | C++23 |

## What the example shows

- Example 1: rounding at compile time
- Example 2: sign and remainder
- Example 3: integer helpers
- Example 4: a compile-time table

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/02_constexpr_cmath
```

Or without CMake:

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 02_constexpr_cmath
./02_constexpr_cmath
```

## Output (GCC 15.2.0)

```text
=== Example 1: rounding ===
floor 7, ceil 8, round 8, trunc(-x) -7

=== Example 2: fabs, fmod, copysign, fmin, fmax ===
fabs(-2.5)=2.5 fmod(10,3)=1 copysign(3,-1)=-3

=== Example 3: abs for integers ===
abs(-42) = 42, abs(-3.5) = 3.5

=== Example 4: table built at compile time ===
[0, 1, 3, 4, 6]
```

## Notes

- `sqrt`, `sin`, `exp`, `pow` and the like become constexpr only in C++26.
- `std::abs(long)` was **not** constexpr with GCC 15.2.0 / the GCC 16 snapshot, and neither was `std::div`; the example only asserts what compiled.

## Tested

- Built with CMake (Debug) and with `g++ -std=c++23 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `floor, ceil, round, trunc, fabs, fmod, copysign, fmin, fmax` | `<cmath>` | C++23 |
| `abs (int, double)` | `<cstdlib>, <cmath>` | C++23 |
