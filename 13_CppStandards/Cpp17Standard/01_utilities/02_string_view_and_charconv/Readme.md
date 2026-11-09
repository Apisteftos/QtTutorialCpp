# std::string_view, from_chars, to_chars

Non-owning string references and fast, locale-independent number conversion.

## Features

| Feature | Header | Standard |
|---|---|---|
| `std::string_view` | `<string_view>` | C++17 |
| `std::from_chars, std::to_chars` | `<charconv>` | C++17 |

## What the example shows

- Example 1: string_view
- Example 2: from_chars
- Example 3: to_chars

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/02_string_view_and_charconv
```

Or without CMake:

```bash
g++ -std=c++17 -Wall -Wextra main.cpp -o 02_string_view_and_charconv
./02_string_view_and_charconv
```

## Output (GCC 15.2.0)

```text
=== Example 1: string_view ===
spaces: 3, 2
first word: the
trimmed view: quick brown

=== Example 2: from_chars ===
'123' -> 123
'12x' -> 12 (stopped early)
'' -> not a number
'99999999999' -> out of range

=== Example 3: to_chars ===
255 in hex: ff
double: 3.14159
```

## Notes

- `std::print` / `std::println` need C++23, so this C++17 example prints with `std::cout`.
- A `string_view` does not own its characters. Never keep one after the string it points to is gone.
- `from_chars` does not throw; it returns an error code and the position where parsing stopped.

## Tested

- Built with CMake and with `g++ -std=c++17 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `std::string_view` | `<string_view>` | C++17 |
| `std::from_chars, std::to_chars` | `<charconv>` | C++17 |
