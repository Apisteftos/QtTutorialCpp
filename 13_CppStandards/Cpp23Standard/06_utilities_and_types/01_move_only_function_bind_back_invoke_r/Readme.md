# move_only_function, bind_back, invoke_r

Function utilities.

## Features

| Feature | Header | Standard |
|---|---|---|
| `std::move_only_function` | `<functional>` | C++23 |
| `std::bind_back` | `<functional>` | C++23 |
| `std::invoke_r` | `<functional>` | C++23 |

## What the example shows

- Example 1: move_only_function
- Example 2: bind_back
- Example 3: invoke_r

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/01_move_only_function_bind_back_invoke_r
```

Or without CMake:

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 01_move_only_function_bind_back_invoke_r
./01_move_only_function_bind_back_invoke_r
```

## Output (GCC 15.2.0)

```text
=== Example 1: move_only_function ===
task() = 42
first job
second job

=== Example 2: bind_back ===
minusTen(25) = 15
hello!

=== Example 3: invoke_r ===
invoke_r<double>: 5
invoke_r<void> ran without using the result
```

## Notes

- `std::function` requires a copyable callable; `move_only_function` does not, so a lambda that owns a `unique_ptr` works.
- `bind_back` fixes the last arguments; `std::bind_front` (C++20) fixes the first.

## Tested

- Built with CMake (Debug) and with `g++ -std=c++23 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `std::move_only_function` | `<functional>` | C++23 |
| `std::bind_back` | `<functional>` | C++23 |
| `std::invoke_r` | `<functional>` | C++23 |
