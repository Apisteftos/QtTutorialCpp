# Coroutines

`co_yield` with a hand-written generator type.

## Features

| Feature | Header | Standard |
|---|---|---|
| co_yield, co_return, co_await | (language) | C++20 |
| `std::coroutine_handle, suspend_always, suspend_never` | `<coroutine>` | C++20 |

## What the example shows

- Example 1: finite generator
- Example 2: infinite generator

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/01_generator_coroutine
```

Or without CMake:

```bash
g++ -std=c++20 -Wall -Wextra main.cpp -o 01_generator_coroutine
./01_generator_coroutine
```

## Output (GCC 15.2.0)

```text
=== Example 1: countTo(5) ===
1 2 3 4 5 

=== Example 2: fibonacci ===
0 1 1 2 3 5 8 13 21 34 
```

## Notes

- `std::print` / `std::println` need C++23, so this C++20 example prints with `std::cout`.
- C++20 has the language support but no ready-made generator type; `std::generator` is C++23 (see the C++23 examples).
- The `Generator` class is deliberately minimal: not copyable, destroys the coroutine in its destructor.

## Tested

- Built with CMake and with `g++ -std=c++20 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| co_yield, co_return, co_await | (language) | C++20 |
| `std::coroutine_handle, suspend_always, suspend_never` | `<coroutine>` | C++20 |
