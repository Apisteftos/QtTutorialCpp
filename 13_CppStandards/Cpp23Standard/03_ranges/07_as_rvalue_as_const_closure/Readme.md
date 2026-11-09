# as_rvalue, as_const, constant ranges, range_adaptor_closure

Move or read-only views, constant iterators, and your own pipe-able adaptor.

## Features

| Feature | Header | Standard |
|---|---|---|
| `views::as_rvalue` | `<ranges>` | C++23 |
| `views::as_const` | `<ranges>` | C++23 |
| `ranges::constant_range, cbegin` | `<ranges>, <iterator>` | C++23 |
| `ranges::range_adaptor_closure` | `<ranges>` | C++23 |

## What the example shows

- Example 1: as_rvalue
- Example 2: as_const
- Example 3: range_adaptor_closure

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/07_as_rvalue_as_const_closure
```

Or without CMake:

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 07_as_rvalue_as_const_closure
./07_as_rvalue_as_const_closure
```

## Output (GCC 15.2.0)

```text
=== Example 1: as_rvalue ===
target: ["alpha", "beta", "gamma"]
source elements are empty after the move: true

=== Example 2: as_const ===
elements are read only: true
first element through a const iterator: 1

=== Example 3: custom adaptor with range_adaptor_closure ===
[20, 40, 60, 80]
called like a function: [2, 4, 6, 8]
```

## Notes

- The concept is `std::ranges::constant_range` (not `std::constant_range`).
- A class derived from `range_adaptor_closure` can be used on the right of `|`.

## Tested

- Built with CMake (Debug) and with `g++ -std=c++23 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `views::as_rvalue` | `<ranges>` | C++23 |
| `views::as_const` | `<ranges>` | C++23 |
| `ranges::constant_range, cbegin` | `<ranges>, <iterator>` | C++23 |
| `ranges::range_adaptor_closure` | `<ranges>` | C++23 |
