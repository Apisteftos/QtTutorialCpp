# std::mdspan

A non-owning multidimensional view over a flat array, with `m[i, j]` syntax.

## Features

| Feature | Header | Standard |
|---|---|---|
| `std::mdspan, extents, dextents` | `<mdspan>` | C++23 |
| `layout_right, layout_left, layout_stride` | `<mdspan>` | C++23 |
| multidimensional operator[] | (language) | C++23 |

## What the example shows

- Example 1: 2D view with multi-index operator[]
- Example 2: layouts
- Example 3: dynamic and fixed extents mixed, 3 dimensions
- Example 4: layout_stride, a column of the matrix

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/02_mdspan
```

Or without CMake:

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 02_mdspan
./02_mdspan
```

## Output (GCC 15.2.0)

```text
std::mdspan is not available in this standard library
```

### Output with a GCC 16 snapshot (pre-release)

```text
=== Example 1: 3 x 4 matrix ===
  0  1  2  3
  4  5  6  7
  8  9 10 11
rank 2, size 12
storage[6] is now 99

=== Example 2: layout_right and layout_left ===
row major:    element [1, 2] = 99
column major: element [1, 2] = 7

=== Example 3: 3D view ===
sum of all elements: 41.5

=== Example 4: layout_stride ===
1 5 9 
```

## Notes

- **`<mdspan>` is not in libstdc++ 15** (it arrives with GCC 16). On GCC 15 the example compiles and prints one message; the full output below was produced with a GCC 16 snapshot.
- `layout_right` is row major (default), `layout_left` is column major.

## Tested

- Built with CMake (Debug) and with `g++ -std=c++23 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Also built and run with a GCC 16 pre-release snapshot (16-20260322); that compiler is not a release.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `std::mdspan, extents, dextents` | `<mdspan>` | C++23 |
| `layout_right, layout_left, layout_stride` | `<mdspan>` | C++23 |
| multidimensional operator[] | (language) | C++23 |
