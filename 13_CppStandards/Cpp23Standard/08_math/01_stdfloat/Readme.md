# <stdfloat>

Fixed-width floating point types.

## Features

| Feature | Header | Standard |
|---|---|---|
| `float16_t, bfloat16_t, float32_t, float64_t, float128_t` | `<stdfloat>` | C++23 |

## What the example shows

- Example 1: which types exist here?
- Example 2: same value, different precision

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/01_stdfloat
```

Or without CMake:

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 01_stdfloat
./01_stdfloat
```

## Output (GCC 15.2.0)

```text
=== Example 1: availability ===
float16_t:  yes
bfloat16_t: yes
float32_t:  yes
float64_t:  yes
float128_t: yes

=== Example 2: precision ===
float32_t: 0.10000000149011611938
float64_t: 0.10000000000000000555
sizes: 4 and 8 bytes
float16_t: 0.0999755859, size 2 bytes
```

## Notes

- Every type is optional. The example checks `__STDCPP_FLOAT16_T__` and the other macros first. With GCC 15.2.0 on x86-64 all five were available.
- `std::float32_t` / `std::float64_t` are distinct types from `float` / `double` even when they have the same format.

## Tested

- Built with CMake (Debug) and with `g++ -std=c++23 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `float16_t, bfloat16_t, float32_t, float64_t, float128_t` | `<stdfloat>` | C++23 |
