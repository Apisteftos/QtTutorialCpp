# adjacent, pairwise, chunk, slide, chunk_by, stride

Different ways to look at neighbouring elements.

## Features

| Feature | Header | Standard |
|---|---|---|
| `views::adjacent<N>, views::pairwise` | `<ranges>` | C++23 |
| `views::adjacent_transform<N>, views::pairwise_transform` | `<ranges>` | C++23 |
| `views::chunk` | `<ranges>` | C++23 |
| `views::slide` | `<ranges>` | C++23 |
| `views::chunk_by` | `<ranges>` | C++23 |
| `views::stride` | `<ranges>` | C++23 |

## What the example shows

- Example 1: adjacent / pairwise
- Example 2: chunk and slide
- Example 3: chunk_by
- Example 4: stride

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/03_adjacent_chunk_slide_stride
```

Or without CMake:

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 03_adjacent_chunk_slide_stride
./03_adjacent_chunk_slide_stride
```

## Output (GCC 15.2.0)

```text
data: [12, 14, 13, 17, 20, 19, 22, 25]

=== Example 1: adjacent, pairwise, adjacent_transform ===
(12 -> 14) (14 -> 13) (13 -> 17) (17 -> 20) (20 -> 19) (19 -> 22) (22 -> 25) 
[12 14 13] [14 13 17] [13 17 20] [17 20 19] [20 19 22] [19 22 25] 
changes: [2, -1, 4, 3, -1, 3, 3]

=== Example 2: chunk and slide ===
chunk: [12, 14, 13]
chunk: [17, 20, 19]
chunk: [22, 25]
window: [12, 14, 13]
window: [14, 13, 17]
window: [13, 17, 20]
window: [17, 20, 19]
window: [20, 19, 22]
window: [19, 22, 25]

=== Example 3: chunk_by ===
rising run: [12, 14]
rising run: [13, 17, 20]
rising run: [19, 22, 25]

=== Example 4: stride ===
every 3rd element: [12, 17, 22]
```

## Notes

- `chunk(n)` makes separate groups (the last may be shorter); `slide(n)` makes overlapping windows.
- `chunk_by(pred)` starts a new group when `pred(previous, next)` is false.

## Tested

- Built with CMake (Debug) and with `g++ -std=c++23 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `views::adjacent<N>, views::pairwise` | `<ranges>` | C++23 |
| `views::adjacent_transform<N>, views::pairwise_transform` | `<ranges>` | C++23 |
| `views::chunk` | `<ranges>` | C++23 |
| `views::slide` | `<ranges>` | C++23 |
| `views::chunk_by` | `<ranges>` | C++23 |
| `views::stride` | `<ranges>` | C++23 |
