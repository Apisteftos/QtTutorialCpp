# std::optional, std::variant, std::any

A missing value, a type-safe union, and a container for any type.

## Features

| Feature | Header | Standard |
|---|---|---|
| `std::optional` | `<optional>` | C++17 |
| `std::variant, std::visit, std::holds_alternative, std::get_if` | `<variant>` | C++17 |
| `std::any, std::any_cast` | `<any>` | C++17 |

## What the example shows

- Example 1: optional
- Example 2: variant and visit
- Example 3: any

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/01_optional_variant_any
```

Or without CMake:

```bash
g++ -std=c++17 -Wall -Wextra main.cpp -o 01_optional_variant_any
./01_optional_variant_any
```

## Output (GCC 15.2.0)

```text
=== Example 1: optional ===
42 -> 42
-5 -> no value (value_or: 0)
abc -> no value (value_or: 0)

=== Example 2: variant and visit ===
int 7
double 2.5
string text
holds int: 1, index 0
get_if string: now a string

=== Example 3: any ===
int inside: 42
string inside: hello
wrong type -> bad_any_cast
has value: 1
```

## Notes

- `std::print` / `std::println` need C++23, so this C++17 example prints with `std::cout`.
- `Overloaded` is a small helper (a class that inherits from several lambdas) so that `std::visit` can take one lambda per alternative.
- `std::any_cast<T>` throws `std::bad_any_cast` when the stored type is different.

## Tested

- Built with CMake and with `g++ -std=c++17 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `std::optional` | `<optional>` | C++17 |
| `std::variant, std::visit, std::holds_alternative, std::get_if` | `<variant>` | C++17 |
| `std::any, std::any_cast` | `<any>` | C++17 |
