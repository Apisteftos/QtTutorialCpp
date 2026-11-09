# Special math functions, 3-argument hypot

Mathematical special functions in `<cmath>`.

## Features

| Feature | Header | Standard |
|---|---|---|
| `std::hypot(x, y, z)` | `<cmath>` | C++17 |
| `riemann_zeta, beta, expint, legendre, ellint_1, cyl_bessel_j, assoc_laguerre` | `<cmath>` | C++17 |

## What the example shows

- Example 1: hypot with three arguments
- Example 2: special functions

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/01_special_functions_and_hypot
```

Or without CMake:

```bash
g++ -std=c++17 -Wall -Wextra main.cpp -o 01_special_functions_and_hypot
./01_special_functions_and_hypot
```

## Output (GCC 15.2.0)

```text
=== Example 1: hypot ===
hypot(3, 4)    = 5.000000
hypot(1, 2, 2) = 3.000000   (length of a 3D vector)

=== Example 2: special functions ===
riemann_zeta(2) = 1.644934   (pi^2 / 6 = 1.644934)
beta(2, 3)      = 0.083333
expint(1)       = 1.895118
erf(1) (C++11)  = 0.842701
legendre(2, 0.5)= -0.125000
ellint_1(0, pi/2)= 1.570796   (= pi/2)
cyl_bessel_j(0, 1) = 0.765198
assoc_laguerre(2, 1, 0.5) = 1.625000
```

## Notes

- `std::print` / `std::println` need C++23, so this C++17 example prints with `std::cout`.
- The example uses `M_PI`, which is a common extension (not standard before C++20); in C++20 use `std::numbers::pi`.

## Tested

- Built with CMake and with `g++ -std=c++17 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `std::hypot(x, y, z)` | `<cmath>` | C++17 |
| `riemann_zeta, beta, expint, legendre, ellint_1, cyl_bessel_j, assoc_laguerre` | `<cmath>` | C++17 |
