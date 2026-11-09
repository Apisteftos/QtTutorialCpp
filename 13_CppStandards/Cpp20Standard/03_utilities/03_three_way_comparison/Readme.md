# Three-way comparison (<=>)

One defaulted operator gives all comparison operators.

## Features

| Feature | Header | Standard |
|---|---|---|
| `operator<=>, defaulted comparison` | `<compare>` | C++20 |
| `strong_ordering, weak_ordering, partial_ordering` | `<compare>` | C++20 |

## What the example shows

- Example 1: defaulted <=>
- Example 2: usable as key
- Example 3: ordering categories

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/03_three_way_comparison
```

Or without CMake:

```bash
g++ -std=c++20 -Wall -Wextra main.cpp -o 03_three_way_comparison
./03_three_way_comparison
```

## Output (GCC 15.2.0)

```text
=== Example 1: defaulted <=> ===
a < b:  true
a == b: false
a >= b: false
a <=> b is less

=== Example 2: ordered container ===
1.9.9 1.10.0 2.0.0 

=== Example 3: weak ordering ===
"Hello" == "hELLO" (ignoring case): true
(x <=> y) is equivalent: true
1.0 <=> NaN is unordered: true
```

## Notes

- `std::print` / `std::println` need C++23, so this C++20 example prints with `std::cout`.
- `= default` compares the members in declaration order.
- `double` gives `partial_ordering` because NaN is unordered.

## Tested

- Built with CMake and with `g++ -std=c++20 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `operator<=>, defaulted comparison` | `<compare>` | C++20 |
| `strong_ordering, weak_ordering, partial_ordering` | `<compare>` | C++20 |
