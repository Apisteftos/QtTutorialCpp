# optional monadic operations and visit on derived variants

Chain optional calls without nested `if`s, and call `std::visit` on a class that inherits from `std::variant`.

## Features

| Feature | Header | Standard |
|---|---|---|
| `optional::and_then / transform / or_else` | `<optional>` | C++23 |
| `std::visit for classes derived from std::variant` | `<variant>` | C++23 |

## What the example shows

- Example 1: and_then / transform / or_else on optional
- Example 2: visit with a class derived from variant

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/05_optional_monadic_and_variant_visit
```

Or without CMake:

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 05_optional_monadic_and_variant_visit
./05_optional_monadic_and_variant_visit
```

## Output (GCC 15.2.0)

```text
=== Example 1: optional monadic operations ===
Mia: adult!
Leo: minor!
Zoe: unknown

=== Example 2: visit on a derived variant ===
holds 3
holds 2.5
holds text
```

## Notes

- The derived class needs `using variant::variant;` to inherit the constructors.

## Tested

- Built with CMake (Debug) and with `g++ -std=c++23 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `optional::and_then / transform / or_else` | `<optional>` | C++23 |
| `std::visit for classes derived from std::variant` | `<variant>` | C++23 |
