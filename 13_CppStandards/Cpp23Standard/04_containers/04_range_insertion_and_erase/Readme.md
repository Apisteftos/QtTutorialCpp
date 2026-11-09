# from_range, insert_range, append_range, assign_range, prepend_range, erase_if

Container members that accept a whole range, and erase helpers.

## Features

| Feature | Header | Standard |
|---|---|---|
| `std::from_range constructors` | `container headers` | C++23 |
| `insert_range, append_range, assign_range, prepend_range` | `container headers` | C++23 |
| `std::erase_if for associative containers` | `container headers` | C++20 |
| `heterogeneous erase for set/map` | `<set>, <map>` | C++23 |

## What the example shows

- Example 1: constructing from a range
- Example 2: insert_range, append_range, assign_range
- Example 3: prepend_range
- Example 4: erase and erase_if for associative containers

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/04_range_insertion_and_erase
```

Or without CMake:

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 04_range_insertion_and_erase
./04_range_insertion_and_erase
```

## Output (GCC 15.2.0)

```text
=== Example 1: std::from_range ===
vector: [1, 4, 9, 16, 25]
set:    {1, 4, 9, 16, 25}

=== Example 2: insert_range, append_range, assign_range ===
numbers: [1, 7, 8, 2, 3, 10, 11, 12]
assigned:  [0, 1, 2, 3]
string: hello world

=== Example 3: prepend_range ===
list: [1, 2, 3, 4, 5]
deque: [1, 2, 3, 4, 5]
forward_list: [1, 2, 3, 4, 5]

=== Example 4: erase_if ===
removed 2 items: {1: "one", 3: "three"}
names: {"anna", "carl"}
```

## Notes

- `prepend_range` exists for `list`, `forward_list` and `deque`; `vector` has `append_range`.
- Heterogeneous `erase` needs `__cpp_lib_associative_heterogeneous_erasure`. If it is missing the example erases with a temporary `std::string` instead. Both paths were compiled (the output is identical).

## Tested

- Built with CMake (Debug) and with `g++ -std=c++23 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `std::from_range constructors` | `container headers` | C++23 |
| `insert_range, append_range, assign_range, prepend_range` | `container headers` | C++23 |
| `std::erase_if for associative containers` | `container headers` | C++20 |
| `heterogeneous erase for set/map` | `<set>, <map>` | C++23 |
