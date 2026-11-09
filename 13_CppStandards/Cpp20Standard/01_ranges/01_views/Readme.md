# Range views

Lazy pipelines with `std::views`.

## Features

| Feature | Header | Standard |
|---|---|---|
| `views::filter, transform, take, drop, take_while, reverse` | `<ranges>` | C++20 |
| `views::iota` | `<ranges>` | C++20 |
| `views::split, keys, values` | `<ranges>` | C++20 |

## What the example shows

- Example 1: filter, transform, take, drop
- Example 2: iota and lazy evaluation
- Example 3: split, keys, values, elements
- Example 4: a view sees later changes

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/01_views
```

Or without CMake:

```bash
g++ -std=c++20 -Wall -Wextra main.cpp -o 01_views
./01_views
```

## Output (GCC 15.2.0)

```text
=== Example 1: filter, transform, take, drop ===
even squares: 4 16 36 64 100
first three: 1 2 3
without first seven: 8 9 10
while < 4: 1 2 3
reversed: 10 9 8

=== Example 2: iota (an infinite range is fine) ===
squares: 1 4 9 16 25

=== Example 3: split, keys, values ===
[red][green][blue]
keys: apple pear
values: 3 5

=== Example 4: views are lazy ===
doubled: 2 4 6 8
```

## Notes

- `std::print` / `std::println` need C++23, so this C++20 example prints with `std::cout`.
- A view is lazy: nothing is computed until it is iterated, so a view over an infinite `iota` is fine when combined with `take`.
- `ranges::to` (to collect a view into a container) is C++23.

## Tested

- Built with CMake and with `g++ -std=c++20 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `views::filter, transform, take, drop, take_while, reverse` | `<ranges>` | C++20 |
| `views::iota` | `<ranges>` | C++20 |
| `views::split, keys, values` | `<ranges>` | C++20 |
