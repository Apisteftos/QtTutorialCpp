# New type traits

Type traits added in C++23.

## Features

| Feature | Header | Standard |
|---|---|---|
| `std::is_scoped_enum` | `<type_traits>` | C++23 |
| `std::is_implicit_lifetime` | `<type_traits>` | C++23 |
| `std::reference_constructs_from_temporary` | `<type_traits>` | C++23 |
| `std::reference_converts_from_temporary` | `<type_traits>` | C++23 |

## What the example shows

- Example 1: is_scoped_enum
- Example 2: is_implicit_lifetime
- Example 3: reference binding to temporaries

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/06_type_traits
```

Or without CMake:

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 06_type_traits
./06_type_traits
```

## Output (GCC 15.2.0)

```text
=== Example 1: is_scoped_enum ===
Plain:  false
Scoped: true
int:    false

=== Example 2: is_implicit_lifetime ===
is_implicit_lifetime: not available in this standard library

=== Example 3: reference_constructs_from_temporary ===
const std::string& from const char*:        true
const std::string& from const std::string&: false
const int& from long:                       true
const int& from int&:                       false
converts: const int& from long:             true
converts: const int& from int&:             false
```

### Output with a GCC 16 snapshot (pre-release)

```text
=== Example 1: is_scoped_enum ===
Plain:  false
Scoped: true
int:    false

=== Example 2: is_implicit_lifetime ===
int:            true
Aggregate:      true
WithDestructor: false
std::string:    false

=== Example 3: reference_constructs_from_temporary ===
const std::string& from const char*:        true
const std::string& from const std::string&: false
const int& from long:                       true
const int& from int&:                       false
converts: const int& from long:             true
converts: const int& from int&:             false
```

## Notes

- **`is_implicit_lifetime` is not in libstdc++ 15.** The example checks `__cpp_lib_is_implicit_lifetime`. The GCC 16 snapshot prints the real values.

## Tested

- Built with CMake (Debug) and with `g++ -std=c++23 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Also built and run with a GCC 16 pre-release snapshot (16-20260322); that compiler is not a release.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `std::is_scoped_enum` | `<type_traits>` | C++23 |
| `std::is_implicit_lifetime` | `<type_traits>` | C++23 |
| `std::reference_constructs_from_temporary` | `<type_traits>` | C++23 |
| `std::reference_converts_from_temporary` | `<type_traits>` | C++23 |
