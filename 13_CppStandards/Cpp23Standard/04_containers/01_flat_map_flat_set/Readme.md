# std::flat_map and std::flat_set

Sorted containers stored in contiguous memory. Compact and fast to iterate; inserting in the middle is slower than in `std::map`.

## Features

| Feature | Header | Standard |
|---|---|---|
| `std::flat_map` | `<flat_map>` | C++23 |
| `std::flat_set` | `<flat_set>` | C++23 |
| `std::sorted_unique` | `<flat_set>` | C++23 |

## What the example shows

- Example 1: flat_map
- Example 2: the data is stored in two sorted vectors
- Example 3: flat_set
- Example 4: replacing the underlying container

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/01_flat_map_flat_set
```

Or without CMake:

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 01_flat_map_flat_set
./01_flat_map_flat_set
```

## Output (GCC 15.2.0)

```text
=== Example 1: flat_map ===
apple   10
banana  7
cherry  25
pear    4
as a map: {"apple": 10, "banana": 7, "cherry": 25, "pear": 4}
apple count: 10
contains plum: false

=== Example 2: keys and values are separate containers ===
keys:   ["apple", "banana", "cherry", "pear"]
values: [10, 7, 25, 4]

=== Example 3: flat_set ===
set: {1, 2, 3, 4, 5}
size 5, contains 4: true
after erase(4): {1, 2, 3, 5}

=== Example 4: sorted_unique build ===
{10, 20, 30}
```

## Notes

- `flat_map` keeps keys and values in two separate sorted vectors (`keys()`, `values()`).
- Use `std::sorted_unique` when the input is already sorted and has no duplicates, to skip sorting.

## Tested

- Built with CMake (Debug) and with `g++ -std=c++23 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `std::flat_map` | `<flat_map>` | C++23 |
| `std::flat_set` | `<flat_set>` | C++23 |
| `std::sorted_unique` | `<flat_set>` | C++23 |
