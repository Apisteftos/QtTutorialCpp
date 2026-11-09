# views::join_with, cartesian_product, repeat

Join ranges with a separator, build every combination of several ranges, repeat a value.

## Features

| Feature | Header | Standard |
|---|---|---|
| `views::join_with` | `<ranges>` | C++23 |
| `views::cartesian_product` | `<ranges>` | C++23 |
| `views::repeat` | `<ranges>` | C++23 |

## What the example shows

- Example 1: join_with
- Example 2: cartesian_product
- Example 3: repeat

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/04_join_with_cartesian_repeat
```

Or without CMake:

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 04_join_with_cartesian_repeat
./04_join_with_cartesian_repeat
```

## Output (GCC 15.2.0)

```text
=== Example 1: join_with ===
red, green, blue
rows joined with 0: [1, 2, 0, 3, 0, 4, 5, 6]

=== Example 2: cartesian_product ===
S-black S-white S-grey M-black M-white M-grey 

=== Example 3: repeat ===
--------------------
[0, 0, 0, 0, 0]
```

## Notes

- `views::repeat(value)` is unbounded; `views::repeat(value, n)` repeats `n` times.

## Tested

- Built with CMake (Debug) and with `g++ -std=c++23 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `views::join_with` | `<ranges>` | C++23 |
| `views::cartesian_product` | `<ranges>` | C++23 |
| `views::repeat` | `<ranges>` | C++23 |
