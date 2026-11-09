# std::print and std::println

`std::print` writes formatted text; `std::println` does the same and adds a line break. Both use the `std::format` syntax.

## Features

| Feature | Header | Standard |
|---|---|---|
| `std::print`, `std::println` | `<print>` | C++23 |
| `std::print(FILE*, ...)` / `std::println(stderr, ...)` | `<print>` | C++23 |

## What the example shows

- Example 1: Basic values
- Example 2: Floating point specs (precision, width, alignment, zero padding, sign, argument index)
- Example 3: print without line break, and output to stderr / a `FILE*`

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/01_printExample
```

## Output (GCC 15.2.0)

```text
=== Example 1: Basic values ===
The value x is: 1
The value y is: 3.6
The value f is: F
The boolean value is: true
The string value is: FDFA

=== Example 2: Floating point specs ===
1233564.500
   1233564.500
1233564.500   |
000001233564.500
+1233564.500
1233564.500 1233564.5

=== Example 3: print, stderr and FILE* ===
no line break here, so this continues the line
this line goes to stdout via a FILE*
```

The stderr line is written to a different stream, so its position in a terminal can differ from the source order.

## Notes

- A blank line is `std::println("")`; no `\n` is needed in the format string.
- The old example included `<iostream>` without using it; it was removed.

## Tested

- Built with CMake and with `g++ -std=c++23 -Wall -Wextra` using GCC 15.2.0: no warnings. Run with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Same output with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `std::print`, `std::println` | `<print>` | C++23 |
