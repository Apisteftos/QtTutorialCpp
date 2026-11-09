# std::format

Type-safe formatting with a Python-like syntax.

## Features

| Feature | Header | Standard |
|---|---|---|
| `std::format, format_to, format_to_n, formatted_size` | `<format>` | C++20 |
| `std::formatter<T> specialization` | `<format>` | C++20 |

## What the example shows

- Example 1: placeholders and specs
- Example 2: width from an argument
- Example 3: format_to, format_to_n, formatted_size
- Example 4: custom type

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/01_std_format
```

Or without CMake:

```bash
g++ -std=c++20 -Wall -Wextra main.cpp -o 01_std_format
./01_std_format
```

## Output (GCC 15.2.0)

```text
=== Example 1: format specs ===
Ann is 31 years old
first before second
[   right] [left    ] [  mid   ]
[****x****] [0003.142] [+5]
hex 0xff, binary 0b101, octal 10
99.50% done, 1.234568e+04
braces: {}, bool: true

=== Example 2: dynamic width ===
[    42]
[      3.14]

=== Example 3: output iterators ===
format_to: 1-2
format_to_n cut: a long  (would need 11 characters)
formatted_size: 10

=== Example 4: custom formatter ===
point: (3, 4)
```

## Notes

- `std::print` / `std::println` need C++23, so this C++20 example prints with `std::cout`.
- `std::print` / `std::println` are **C++23**. In C++20 the string is built with `std::format` and written with `std::cout`.

## Tested

- Built with CMake and with `g++ -std=c++20 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `std::format, format_to, format_to_n, formatted_size` | `<format>` | C++20 |
| `std::formatter<T> specialization` | `<format>` | C++20 |
