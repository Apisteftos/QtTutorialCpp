# Semaphores

Counting and binary semaphores.

## Features

| Feature | Header | Standard |
|---|---|---|
| `std::counting_semaphore, std::binary_semaphore` | `<semaphore>` | C++20 |

## What the example shows

- Example 1: counting_semaphore limits parallel access
- Example 2: binary_semaphore as signal
- Example 3: try_acquire

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/03_semaphore
```

Or without CMake:

```bash
g++ -std=c++20 -Wall -Wextra -pthread main.cpp -o 03_semaphore
./03_semaphore
```

## Output (GCC 15.2.0)

```text
=== Example 1: at most 2 threads inside ===
maximum number of threads inside at the same time: 2

=== Example 2: binary_semaphore as signal ===
consumer got 42

=== Example 3: try_acquire ===
first try_acquire:  true
second try_acquire: false
```

## Notes

- `std::print` / `std::println` need C++23, so this C++20 example prints with `std::cout`.
- The example proves the limit by measuring the maximum number of threads inside the section (always 2).

## Tested

- Built with CMake and with `g++ -std=c++20 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `std::counting_semaphore, std::binary_semaphore` | `<semaphore>` | C++20 |
