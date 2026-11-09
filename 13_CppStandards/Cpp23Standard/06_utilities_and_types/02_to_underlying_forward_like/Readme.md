# to_underlying, forward_like, deducing this

Enum conversion and perfect forwarding of members.

## Features

| Feature | Header | Standard |
|---|---|---|
| `std::to_underlying` | `<utility>` | C++23 |
| `std::forward_like` | `<utility>` | C++23 |
| explicit object parameter (`this Self&&`) | (language) | C++23 |

## What the example shows

- Example 1: to_underlying
- Example 2: forward_like
- Example 3: deducing this for a recursive lambda

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/02_to_underlying_forward_like
```

Or without CMake:

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 02_to_underlying_forward_like
./02_to_underlying_forward_like
```

## Output (GCC 15.2.0)

```text
=== Example 1: to_underlying ===
High is 10 and the type is unsigned char: true

=== Example 2: forward_like with deducing this ===
lvalue object -> lvalue member, const object -> const member, rvalue object -> rvalue member
moved out: payload

=== Example 3: recursive lambda ===
10! = 3628800
```

## Notes

- `forward_like<Self>(member)` gives the member the same const-ness and value category as `Self`.
- The recursive lambda uses `this auto&& self`.

## Tested

- Built with CMake (Debug) and with `g++ -std=c++23 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `std::to_underlying` | `<utility>` | C++23 |
| `std::forward_like` | `<utility>` | C++23 |
| explicit object parameter (`this Self&&`) | (language) | C++23 |
