# C++17 language features

Language features that C++17 code uses together with the library.

## Features

| Feature | Header | Standard |
|---|---|---|
| structured bindings | (language) | C++17 |
| if / switch with initializer | (language) | C++17 |
| if constexpr | (language) | C++17 |
| fold expressions | (language) | C++17 |
| class template argument deduction (CTAD) | (language) | C++17 |
| inline variables, nested namespaces | (language) | C++17 |
| [[nodiscard]], [[maybe_unused]], [[fallthrough]] | (language) | C++17 |

## What the example shows

- Example 1: structured bindings
- Example 2: if / switch with initializer
- Example 3: if constexpr and fold expressions
- Example 4: CTAD and inline variable

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/05_language_features
```

Or without CMake:

```bash
g++ -std=c++17 -Wall -Wextra main.cpp -o 05_language_features
./05_language_features
```

## Output (GCC 15.2.0)

```text
=== Example 1: structured bindings ===
7 seven
Ann is 31
Ben is 27

=== Example 2: if with initializer ===
found Ben
nine
default also runs

=== Example 3: if constexpr, fold ===
integer 5, floating point 2.500000, something else
sumAll = 10
1 two 3.5 

=== Example 4: CTAD ===
1 3 x
pi = 3.14159
```

## Notes

- `std::print` / `std::println` need C++23, so this C++17 example prints with `std::cout`.
- `describe("text")` takes the last `else` branch of `if constexpr`; the other branches are not even compiled for that type.

## Tested

- Built with CMake and with `g++ -std=c++17 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| structured bindings | (language) | C++17 |
| if / switch with initializer | (language) | C++17 |
| if constexpr | (language) | C++17 |
| fold expressions | (language) | C++17 |
| class template argument deduction (CTAD) | (language) | C++17 |
| inline variables, nested namespaces | (language) | C++17 |
| [[nodiscard]], [[maybe_unused]], [[fallthrough]] | (language) | C++17 |
