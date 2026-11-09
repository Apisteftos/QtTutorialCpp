# Formatting ranges, std::range_formatter, std::format_kind, std::formattable

## Features

| Feature | Header | Standard |
|---|---|---|
| Formatters for ranges, `std::pair`, `std::tuple` | `<format>` | C++23 |
| `std::format_kind`, `std::range_format` | `<format>` | C++23 |
| `std::range_formatter` | `<format>` | C++23 |
| `std::formattable` | `<format>` | C++23 |

## What the example shows

- Example 1: built-in formatters and the range spec `{[range-spec]:[element-spec]}`
  (`n` no brackets, `m` map style, `s` string, `::.2f` element spec)
- Example 2: `format_kind` specialization so a user-defined range prints like a set
- Example 3: a `std::formatter<Vec3>` built on `std::range_formatter`
- Example 4: the `std::formattable` concept to check or constrain a template

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/02_formatter
```

## Output (GCC 15.2.0)

```text
=== Example 1: built-in formatters ===
[1, 2, 3]
{1, 2, 3}
{1: "one", 2: "two"}
["red", "green"]
(7, "seven")
(1, "hi", 2.5)
1, 2, 3
[0001, 0002, 0003]
[1.00, 2.50, 3.14]
1.0, 2.5, 3.1
7, "seven"
1, "hi", 2.5
[(1, "a"), (2, "b")]
{1: "a", 2: "b"}
['h', 'e', 'y']
hey

=== Example 2: std::format_kind ===
{"cpp", "qt", "cmake"}

=== Example 3: std::range_formatter ===
<1; 2.5; 3.14159>
<1.00; 2.50; 3.14>
1.0; 2.5; 3.1

=== Example 4: std::formattable ===
value = 42
value = [1, 2, 3]
value = (1, "x")
```

## Notes

- The range and tuple formatters need GCC 15 or newer; GCC 14 has `<print>` but not these formatters.
- In `std::formatter<Vec3>` the `format` function has an explicit return type (`typename FormatContext::iterator`) instead of `auto`. With `auto` the `std::formattable` check instantiates the body, and libstdc++ 15's `range_formatter::format` rejects that probing context.
- The example uses neutral data (colour names, tag names). The old version contained personal first names.

## Tested

- Built with CMake and with `g++ -std=c++23 -Wall -Wextra` using GCC 15.2.0: no warnings. Run with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Same output with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++ (the old file had notes about libc++ 18 behaviour; they were removed because they could not be verified here), MSVC.

## Reference

| Feature | Header | Standard |
|---|---|---|
| Range / pair / tuple formatters | `<format>` | C++23 |
| `std::format_kind`, `std::range_formatter`, `std::formattable` | `<format>` | C++23 |
