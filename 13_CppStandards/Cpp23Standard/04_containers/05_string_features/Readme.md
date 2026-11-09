# string::contains, resize_and_overwrite, deleted nullptr constructor

Small additions to `std::string` and `std::string_view`.

## Features

| Feature | Header | Standard |
|---|---|---|
| `basic_string::contains, basic_string_view::contains` | `<string>, <string_view>` | C++23 |
| `basic_string::resize_and_overwrite` | `<string>` | C++23 |
| `deleted basic_string(nullptr_t)` | `<string>` | C++23 |

## What the example shows

- Example 1: contains
- Example 2: resize_and_overwrite
- Example 3: nullptr is no longer accepted

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/05_string_features
```

Or without CMake:

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 05_string_features
./05_string_features
```

## Output (GCC 15.2.0)

```text
=== Example 1: contains ===
string contains "local": true
string contains 'z':     false
view contains "bin":     true

=== Example 2: resize_and_overwrite ===
size 16: value=255 hex=ff

=== Example 3: deleted nullptr constructors ===
empty string: '', empty: true
```

## Notes

- `resize_and_overwrite(n, op)` gives `op` a raw buffer of `n` characters; `op` returns the final size. Useful with C functions such as `snprintf`.
- `std::string s = nullptr;` is a compile error from C++23 on; it is shown only as a comment.

## Tested

- Built with CMake (Debug) and with `g++ -std=c++23 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `basic_string::contains, basic_string_view::contains` | `<string>, <string_view>` | C++23 |
| `basic_string::resize_and_overwrite` | `<string>` | C++23 |
| `deleted basic_string(nullptr_t)` | `<string>` | C++23 |
