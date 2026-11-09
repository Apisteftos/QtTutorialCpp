# Parallel algorithms

Pass an execution policy as the first argument of an algorithm.

## Features

| Feature | Header | Standard |
|---|---|---|
| `std::execution::seq, par, par_unseq` | `<execution>` | C++17 |
| `for_each, sort, transform, reduce with a policy` | `<algorithm>, <numeric>` | C++17 |

## What the example shows

- Example 1: for_each with a policy
- Example 2: sort and transform
- Example 3: reduce

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/01_parallel_algorithms
```

Or without CMake:

```bash
g++ -std=c++17 -Wall -Wextra -ltbb main.cpp -o 01_parallel_algorithms
./01_parallel_algorithms
```

## Output (GCC 15.2.0)

```text
=== Example 1: for_each(par) ===
sum = 5000050000

=== Example 2: sort(par), transform(par_unseq) ===
0 1 2 3 4 5 6 7 8 9 
doubled.back() = 200000

=== Example 3: reduce(par) ===
total = 5000050000
```

## Notes

- `std::print` / `std::println` need C++23, so this C++17 example prints with `std::cout`.
- With GCC the parallel backend is Intel TBB. Install it (for example `libtbb-dev`) and link with `-ltbb`; the CMakeLists.txt does this when TBB is found.
- With the TBB headers installed but without `-ltbb` the link fails. This was checked on GCC 15.2.0. If TBB is not installed, GCC uses a sequential backend and the program still runs.
- The callable runs on several threads: shared data needs protection (the example uses `std::atomic`).

## Tested

- Built with CMake and with `g++ -std=c++17 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `std::execution::seq, par, par_unseq` | `<execution>` | C++17 |
| `for_each, sort, transform, reduce with a policy` | `<algorithm>, <numeric>` | C++17 |
