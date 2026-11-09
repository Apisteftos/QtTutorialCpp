# ranges::fold_* and ranges::iota

Reduce a range to one value, and fill a range with increasing values.

## Features

| Feature | Header | Standard |
|---|---|---|
| `ranges::fold_left, fold_right` | `<algorithm>` | C++23 |
| `ranges::fold_left_first, fold_right_last` | `<algorithm>` | C++23 |
| `ranges::fold_left_with_iter, fold_left_first_with_iter` | `<algorithm>` | C++23 |
| `ranges::iota` | `<numeric>` | C++23 |

## What the example shows

- Example 1: fold_left / fold_right
- Example 2: the "first" variants (no start value)
- Example 3: the with_iter variants
- Example 4: ranges::iota

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/06_fold_and_iota
```

Or without CMake:

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 06_fold_and_iota
./06_fold_and_iota
```

## Output (GCC 15.2.0)

```text
=== Example 1: fold_left and fold_right ===
sum:     15
left:    ((((( 1) 2) 3) 4) 5)
right:   (1 (2 (3 (4 (5 )))))

=== Example 2: fold_left_first and fold_right_last ===
maximum: 5
empty range has a value: false
fold_right_last with minus: 3

=== Example 3: fold_left_with_iter ===
total 115 and iterator at end: true

=== Example 4: ranges::iota ===
filled: [10, 11, 12, 13, 14, 15], next value would be 16
```

## Notes

- The `_first` / `_last` variants use the first (or last) element as the start value and return `std::optional`, empty for an empty range.
- The example uses `#include <functional>` for `std::plus` and `std::minus`.

## Tested

- Built with CMake (Debug) and with `g++ -std=c++23 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `ranges::fold_left, fold_right` | `<algorithm>` | C++23 |
| `ranges::fold_left_first, fold_right_last` | `<algorithm>` | C++23 |
| `ranges::fold_left_with_iter, fold_left_first_with_iter` | `<algorithm>` | C++23 |
| `ranges::iota` | `<numeric>` | C++23 |
