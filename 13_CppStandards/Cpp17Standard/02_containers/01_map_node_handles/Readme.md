# map/set node handles, try_emplace, insert_or_assign

Move elements between associative containers without copying, change keys in place.

## Features

| Feature | Header | Standard |
|---|---|---|
| `extract, node handles, merge` | `<map>, <set>` | C++17 |
| `try_emplace` | `<map>` | C++17 |
| `insert_or_assign` | `<map>` | C++17 |

## What the example shows

- Example 1: try_emplace and insert_or_assign
- Example 2: extract: change a key without copying the element
- Example 3: merge
- Example 4: moving a node between containers

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/01_map_node_handles
```

Or without CMake:

```bash
g++ -std=c++17 -Wall -Wextra main.cpp -o 01_map_node_handles
./01_map_node_handles
```

## Output (GCC 15.2.0)

```text
=== Example 1: try_emplace, insert_or_assign ===
second try_emplace inserted: false, value one
map: 1=ONE 2=two

=== Example 2: extract and change a key ===
after key change: 1=ONE 20=two

=== Example 3: merge ===
merged: 1=ONE 5=five 20=two
left in other: 1=dup

=== Example 4: set node ===
a has 1 element, b has 1
```

## Notes

- `std::print` / `std::println` need C++23, so this C++17 example prints with `std::cout`.
- `try_emplace` does not construct (or move from) the arguments if the key already exists.
- `merge` moves only the nodes whose keys are new; duplicates stay in the source container.

## Tested

- Built with CMake and with `g++ -std=c++17 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `extract, node handles, merge` | `<map>, <set>` | C++17 |
| `try_emplace` | `<map>` | C++17 |
| `insert_or_assign` | `<map>` | C++17 |
