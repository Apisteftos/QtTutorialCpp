# latch and barrier

One-shot and reusable synchronization points.

## Features

| Feature | Header | Standard |
|---|---|---|
| `std::latch` | `<latch>` | C++20 |
| `std::barrier` | `<barrier>` | C++20 |

## What the example shows

- Example 1: latch
- Example 2: barrier with a completion step

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/02_latch_and_barrier
```

Or without CMake:

```bash
g++ -std=c++20 -Wall -Wextra -pthread main.cpp -o 02_latch_and_barrier
./02_latch_and_barrier
```

## Output (GCC 15.2.0)

```text
=== Example 1: latch ===
all 4 workers have started

=== Example 2: barrier ===
phases completed: 2, total work items: 6
```

## Notes

- `std::print` / `std::println` need C++23, so this C++20 example prints with `std::cout`.
- A `latch` is used once; a `barrier` is reused for every phase and can run a completion function.

## Tested

- Built with CMake and with `g++ -std=c++20 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `std::latch` | `<latch>` | C++20 |
| `std::barrier` | `<barrier>` | C++20 |
