# Range algorithms, projections, subrange, dangling

Algorithms in `std::ranges` take a whole container and an optional projection.

## Features

| Feature | Header | Standard |
|---|---|---|
| `std::ranges::sort, find, max_element, copy` | `<algorithm>` | C++20 |
| projections | <algorithm> | C++20 |
| `std::ranges::subrange, std::ranges::dangling` | `<ranges>` | C++20 |

## What the example shows

- Example 1: algorithms take the whole container
- Example 2: projections
- Example 3: subrange and dangling
- Example 4: results with structured information

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/02_range_algorithms_and_projections
```

Or without CMake:

```bash
g++ -std=c++20 -Wall -Wextra main.cpp -o 02_range_algorithms_and_projections
./02_range_algorithms_and_projections
```

## Output (GCC 15.2.0)

```text
=== Example 1: no begin()/end() pairs ===
1 2 5 7 9 
contains 9: 1

=== Example 2: projections ===
Mia (27)  Zoe (31)  Adam (45)  
Zoe  Mia  Adam  
oldest: Adam

=== Example 3: subrange, dangling ===
2 5 7 
result of an algorithm on a temporary is dangling: 1

=== Example 4: ranges::copy returns in and out ===
copied 5 elements, input fully consumed: 1
```

## Notes

- `std::print` / `std::println` need C++23, so this C++20 example prints with `std::cout`.
- `std::ranges::sort(people, {}, &Person::age)`: the empty `{}` is the default comparison, `&Person::age` is the projection.
- When an algorithm is called with a temporary container, it returns `std::ranges::dangling` instead of an iterator that would dangle.

## Tested

- Built with CMake and with `g++ -std=c++20 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `std::ranges::sort, find, max_element, copy` | `<algorithm>` | C++20 |
| projections | <algorithm> | C++20 |
| `std::ranges::subrange, std::ranges::dangling` | `<ranges>` | C++20 |
