# contains, starts_with, find_last, shift

Searching algorithms added to `std::ranges`, plus `shift_left` / `shift_right`.

## Features

| Feature | Header | Standard |
|---|---|---|
| `ranges::contains, ranges::contains_subrange` | `<algorithm>` | C++23 |
| `ranges::starts_with, ranges::ends_with` | `<algorithm>` | C++23 |
| `ranges::find_last, find_last_if, find_last_if_not` | `<algorithm>` | C++23 |
| `std::shift_left, std::shift_right` | `<algorithm>` | C++20 |

## What the example shows

- Example 1: contains, contains_subrange, starts_with, ends_with
- Example 2: find_last family
- Example 3: shift_left / shift_right

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/05_search_algorithms
```

Or without CMake:

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 05_search_algorithms
./05_search_algorithms
```

## Output (GCC 15.2.0)

```text
data: [3, 8, 15, 4, 8, 23, 42]

=== Example 1: contains and friends ===
contains 15:            true
contains 99:            false
contains_subrange:      true
starts_with / ends_with: not available in this standard library

=== Example 2: find_last, find_last_if, find_last_if_not ===
find_last 8, found at index 4: [8, 23, 42]
last even number:        42
last number below 8:     4
find_last 99 is empty:   true

=== Example 3: shift_left and shift_right ===
after shift_left(2):  first three = [3, 4, 5]
after shift_right(2): last three  = [1, 2, 3]
```

### Output with a GCC 16 snapshot (pre-release)

```text
data: [3, 8, 15, 4, 8, 23, 42]

=== Example 1: contains and friends ===
contains 15:            true
contains 99:            false
contains_subrange:      true
starts_with {3, 8}:    true
ends_with {23, 42}:    true

=== Example 2: find_last, find_last_if, find_last_if_not ===
find_last 8, found at index 4: [8, 23, 42]
last even number:        42
last number below 8:     4
find_last 99 is empty:   true

=== Example 3: shift_left and shift_right ===
after shift_left(2):  first three = [3, 4, 5]
after shift_right(2): last three  = [1, 2, 3]
```

## Notes

- `ranges::starts_with` / `ends_with` are **not in libstdc++ 15**. The example detects this with `__cpp_lib_ranges_starts_ends_with` and prints a message instead. They work with the GCC 16 snapshot.
- `find_last*` return a subrange from the last match to the end of the input. An empty subrange means "not found".
- `shift_left` / `shift_right` are C++20; they are listed on the cheat sheet next to the C++23 additions.

## Tested

- Built with CMake (Debug) and with `g++ -std=c++23 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Also built and run with a GCC 16 pre-release snapshot (16-20260322); that compiler is not a release.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `ranges::contains, ranges::contains_subrange` | `<algorithm>` | C++23 |
| `ranges::starts_with, ranges::ends_with` | `<algorithm>` | C++23 |
| `ranges::find_last, find_last_if, find_last_if_not` | `<algorithm>` | C++23 |
| `std::shift_left, std::shift_right` | `<algorithm>` | C++20 |
