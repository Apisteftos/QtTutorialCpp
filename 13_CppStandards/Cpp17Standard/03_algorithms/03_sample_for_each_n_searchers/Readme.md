# sample, for_each_n, searchers

More algorithms in C++17.

## Features

| Feature | Header | Standard |
|---|---|---|
| `std::sample` | `<algorithm>` | C++17 |
| `std::for_each_n` | `<algorithm>` | C++17 |
| `std::boyer_moore_searcher, default_searcher` | `<functional>` | C++17 |

## What the example shows

- Example 1: sample
- Example 2: for_each_n
- Example 3: searchers

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/03_sample_for_each_n_searchers
```

Or without CMake:

```bash
g++ -std=c++17 -Wall -Wextra main.cpp -o 03_sample_for_each_n_searchers
./03_sample_for_each_n_searchers
```

## Output (GCC 15.2.0)

```text
=== Example 1: sample ===
picked 5 values, all within 1..100: 1
order is kept (sorted): 1

=== Example 2: for_each_n ===
10 20 30 4 5 

=== Example 3: boyer_moore_searcher ===
found at index 35
default_searcher agrees: 1
```

## Notes

- `std::print` / `std::println` need C++23, so this C++17 example prints with `std::cout`.
- The generator has a fixed seed, so the example is reproducible; the picked values are not printed because they depend on the standard library.

## Tested

- Built with CMake and with `g++ -std=c++17 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `std::sample` | `<algorithm>` | C++17 |
| `std::for_each_n` | `<algorithm>` | C++17 |
| `std::boyer_moore_searcher, default_searcher` | `<functional>` | C++17 |
