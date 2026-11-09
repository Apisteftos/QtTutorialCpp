# views::zip, zip_transform, enumerate

Walk several ranges together, combine them with a function, or add an index to every element.

## Features

| Feature | Header | Standard |
|---|---|---|
| `views::zip` | `<ranges>` | C++23 |
| `views::zip_transform` | `<ranges>` | C++23 |
| `views::enumerate` | `<ranges>` | C++23 |

## What the example shows

- Example 1: zip
- Example 2: zip_transform
- Example 3: enumerate

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/02_zip_enumerate
```

Or without CMake:

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 02_zip_enumerate
./02_zip_enumerate
```

## Output (GCC 15.2.0)

```text
=== Example 1: zip ===
Oslo    120 visitors, price 9.5
Lima    340 visitors, price 7
Kyoto   275 visitors, price 12.25
after the change: [130, 350, 285] [19, 14, 24.5]

=== Example 2: zip_transform ===
revenue 2470
revenue 4900
revenue 6982.5

=== Example 3: enumerate ===
0: Oslo
1: Lima
2: Kyoto
Oslo -> 1
Lima -> 2
```

## Notes

- `zip` yields tuples of references, so changing the structured bindings changes the original containers.
- `zip` stops at the shortest input range.

## Tested

- Built with CMake (Debug) and with `g++ -std=c++23 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `views::zip` | `<ranges>` | C++23 |
| `views::zip_transform` | `<ranges>` | C++23 |
| `views::enumerate` | `<ranges>` | C++23 |
