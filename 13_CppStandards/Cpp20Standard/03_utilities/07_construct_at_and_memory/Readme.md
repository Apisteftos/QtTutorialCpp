# construct_at, destroy_at, make_unique_for_overwrite, to_address, assume_aligned

Memory utilities in `<memory>`.

## Features

| Feature | Header | Standard |
|---|---|---|
| `std::construct_at, std::destroy_at` | `<memory>` | C++20 |
| `std::make_unique_for_overwrite` | `<memory>` | C++20 |
| `std::to_address` | `<memory>` | C++20 |
| `std::assume_aligned` | `<memory>` | C++20 |

## What the example shows

- Example 1: construct_at, destroy_at
- Example 2: make_unique_for_overwrite
- Example 3: to_address, assume_aligned

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/07_construct_at_and_memory
```

Or without CMake:

```bash
g++ -std=c++20 -Wall -Wextra main.cpp -o 07_construct_at_and_memory
./07_construct_at_and_memory
```

## Output (GCC 15.2.0)

```text
=== Example 1: construct_at, destroy_at ===
  construct 7
id = 7
  destroy 7
constexpr use compiled: 42

=== Example 2: make_unique_for_overwrite ===
buffer: 0 1 4 9

=== Example 3: to_address, assume_aligned ===
to_address: hello
aligned[0] = 1.5
```

## Notes

- `std::print` / `std::println` need C++23, so this C++20 example prints with `std::cout`.
- `construct_at` can be used in constant expressions; placement `new` cannot.
- `make_unique_for_overwrite` leaves the elements uninitialized, so read them only after writing.

## Tested

- Built with CMake and with `g++ -std=c++20 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `std::construct_at, std::destroy_at` | `<memory>` | C++20 |
| `std::make_unique_for_overwrite` | `<memory>` | C++20 |
| `std::to_address` | `<memory>` | C++20 |
| `std::assume_aligned` | `<memory>` | C++20 |
