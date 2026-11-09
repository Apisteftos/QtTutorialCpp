# Designated initializers, template lambdas, using enum, consteval, constinit

C++20 language features.

## Features

| Feature | Header | Standard |
|---|---|---|
| designated initializers | (language) | C++20 |
| template lambdas, pack capture | (language) | C++20 |
| using enum | (language) | C++20 |
| consteval, constinit | (language) | C++20 |
| [[likely]], [[unlikely]] | (language) | C++20 |
| `<version> feature test macros` | `<version>` | C++20 |

## What the example shows

- Example 1: designated initializers
- Example 2: template lambdas
- Example 3: using enum, consteval, constinit
- Example 4: attributes and init in range-for

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/01_designated_init_and_lambdas
```

Or without CMake:

```bash
g++ -std=c++20 -Wall -Wextra main.cpp -o 01_designated_init_and_lambdas
./01_designated_init_and_lambdas
```

## Output (GCC 15.2.0)

```text
=== Example 1: designated initializers ===
120x24 fullscreen: true

=== Example 2: template lambda ===
4 a
pack capture sum: 6

=== Example 3: using enum, consteval, constinit ===
green
consteval square(7) = 49, constinit variable = 16
after increment: 17

=== Example 4: [[likely]], for with initializer ===
1 2 (even, rare) 3 
__cpp_lib_ranges = 202110
```

## Notes

- `std::print` / `std::println` need C++23, so this C++20 example prints with `std::cout`.
- Designated initializers must follow the declaration order of the members.
- `constinit` guarantees compile-time initialization, but the variable stays mutable (unlike `constexpr`).

## Tested

- Built with CMake and with `g++ -std=c++20 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| designated initializers | (language) | C++20 |
| template lambdas, pack capture | (language) | C++20 |
| using enum | (language) | C++20 |
| consteval, constinit | (language) | C++20 |
| [[likely]], [[unlikely]] | (language) | C++20 |
| `<version> feature test macros` | `<version>` | C++20 |
