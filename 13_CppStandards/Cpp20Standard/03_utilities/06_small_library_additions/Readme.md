# to_array, contains, erase, starts_with, remove_cvref, make_shared<T[]>

Small additions that remove boilerplate.

## Features

| Feature | Header | Standard |
|---|---|---|
| `std::to_array` | `<array>` | C++20 |
| `map::contains, set::contains` | `<map>, <set>` | C++20 |
| `std::erase, std::erase_if` | `<vector> ...` | C++20 |
| `string::starts_with, ends_with` | `<string>` | C++20 |
| `std::remove_cvref` | `<type_traits>` | C++20 |
| `std::make_shared<T[]>` | `<memory>` | C++20 |

## What the example shows

- Example 1: to_array, contains
- Example 2: erase / erase_if
- Example 3: string prefix / suffix
- Example 4: remove_cvref, make_shared for arrays

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/06_small_library_additions
```

Or without CMake:

```bash
g++ -std=c++20 -Wall -Wextra main.cpp -o 06_small_library_additions
./06_small_library_additions
```

## Output (GCC 15.2.0)

```text
=== Example 1: to_array, contains ===
array size 3, char array size 4
map contains Ann: true, set contains 5: false

=== Example 2: erase, erase_if ===
removed 3 twos, left 3
removed 2 values greater than 2, left 1

=== Example 3: starts_with, ends_with ===
starts with report: true, ends with .csv: true, starts with 'x': false

=== Example 4: remove_cvref, make_shared<T[]> ===
remove_cvref_t<const std::string&> is std::string
shared array: 0 7
```

## Notes

- `std::print` / `std::println` need C++23, so this C++20 example prints with `std::cout`.

## Tested

- Built with CMake and with `g++ -std=c++20 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `std::to_array` | `<array>` | C++20 |
| `map::contains, set::contains` | `<map>, <set>` | C++20 |
| `std::erase, std::erase_if` | `<vector> ...` | C++20 |
| `string::starts_with, ends_with` | `<string>` | C++20 |
| `std::remove_cvref` | `<type_traits>` | C++20 |
| `std::make_shared<T[]>` | `<memory>` | C++20 |
