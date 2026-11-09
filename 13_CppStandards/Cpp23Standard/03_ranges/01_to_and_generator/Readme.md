# ranges::to, std::generator, ranges::elements_of

Turn a range into a container, produce lazy sequences with a coroutine, and yield all elements of another generator.

## Features

| Feature | Header | Standard |
|---|---|---|
| `std::ranges::to` | `<ranges>` | C++23 |
| `std::generator` | `<generator>` | C++23 |
| `std::ranges::elements_of` | `<ranges>` | C++23 |

## What the example shows

- Example 1: ranges::to
- Example 2: generator
- Example 3: elements_of with a recursive generator

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/01_to_and_generator
```

Or without CMake:

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 01_to_and_generator
./01_to_and_generator
```

## Output (GCC 15.2.0)

```text
=== Example 1: ranges::to ===
vector: [1, 4, 9, 16, 25]
set:    {1, 4, 9, 16, 25}
list:   [1, 4, 9, 16, 25]
reversed: rotareneg

=== Example 2: generator ===
first ten Fibonacci numbers: [0, 1, 1, 2, 3, 5, 8, 13, 21, 34]

=== Example 3: elements_of ===
depth-first walk: [1, 2, 4, 3]
```

## Notes

- `ranges::to<std::vector>()` deduces the element type; `ranges::to<std::list<int>>()` names it.
- The Fibonacci generator is infinite. It works because `take(10)` only asks for ten values.

## Tested

- Built with CMake (Debug) and with `g++ -std=c++23 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `std::ranges::to` | `<ranges>` | C++23 |
| `std::generator` | `<generator>` | C++23 |
| `std::ranges::elements_of` | `<ranges>` | C++23 |
