# std::expected

`std::expected<T, E>` holds either a value or an error. The error is part of the function signature, so no exception is needed for expected failures.

## Features

| Feature | Header | Standard |
|---|---|---|
| `std::expected, std::unexpected, std::unexpect` | `<expected>` | C++23 |
| `std::bad_expected_access` | `<expected>` | C++23 |
| `expected::and_then / transform / or_else` | `<expected>` | C++23 |

## What the example shows

- Example 1: Value or error
- Example 2: Chaining with and_then / transform / or_else
- Example 3: std::unexpect and bad_expected_access

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/01_expected
```

Or without CMake:

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 01_expected
./01_expected
```

## Output (GCC 15.2.0)

```text
=== Example 1: Value or error ===
'8080' -> value 8080
'' -> error: empty input
'12ab' -> error: not a number
'99999999999' -> error: out of range
value_or: -1

=== Example 2: Chaining ===
'443' -> next port 444
'70000' -> next port 8080
'http' -> error: not a number

=== Example 3: unexpect and bad_expected_access ===
caught bad_expected_access: disk not ready
```

## Notes

- `and_then` runs a function that itself returns an `expected` (only on success). `transform` changes the value. `or_else` runs only on error and can recover.
- `value()` on an error throws `std::bad_expected_access<E>`; `value_or(x)` returns a default instead.

## Tested

- Built with CMake (Debug) and with `g++ -std=c++23 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `std::expected, std::unexpected, std::unexpect` | `<expected>` | C++23 |
| `std::bad_expected_access` | `<expected>` | C++23 |
| `expected::and_then / transform / or_else` | `<expected>` | C++23 |
