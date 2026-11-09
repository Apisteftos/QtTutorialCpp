# reduce, transform_reduce, scans

Numeric algorithms that may run in any order (and in parallel).

## Features

| Feature | Header | Standard |
|---|---|---|
| `std::reduce, std::transform_reduce` | `<numeric>` | C++17 |
| `std::inclusive_scan, exclusive_scan, transform_inclusive_scan` | `<numeric>` | C++17 |

## What the example shows

- Example 1: reduce and transform_reduce
- Example 2: scans

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/02_reduce_and_scans
```

Or without CMake:

```bash
g++ -std=c++17 -Wall -Wextra main.cpp -o 02_reduce_and_scans
./02_reduce_and_scans
```

## Output (GCC 15.2.0)

```text
=== Example 1: reduce, transform_reduce ===
reduce:           15
product:          120
sum of squares:   55
dot product:      35

=== Example 2: inclusive_scan, exclusive_scan ===
inclusive_scan: 1 3 6 10 15
exclusive_scan: 0 1 3 6 10
transform_inclusive_scan: 10 30 60 100 150
```

## Notes

- `std::print` / `std::println` need C++23, so this C++17 example prints with `std::cout`.
- Unlike `std::accumulate`, `reduce` may combine elements in any order, so the operation should be associative. Floating point sums can differ slightly.

## Tested

- Built with CMake and with `g++ -std=c++17 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `std::reduce, std::transform_reduce` | `<numeric>` | C++17 |
| `std::inclusive_scan, exclusive_scan, transform_inclusive_scan` | `<numeric>` | C++17 |
