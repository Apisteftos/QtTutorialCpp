# jthread and stop_token

A thread that joins automatically and can be asked to stop.

## Features

| Feature | Header | Standard |
|---|---|---|
| `std::jthread` | `<thread>` | C++20 |
| `std::stop_token, stop_source, stop_callback` | `<stop_token>` | C++20 |

## What the example shows

- Example 1: automatic join
- Example 2: stop_token
- Example 3: stop_callback

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/01_jthread_stop_token
```

Or without CMake:

```bash
g++ -std=c++20 -Wall -Wextra -pthread main.cpp -o 01_jthread_stop_token
./01_jthread_stop_token
```

## Output (GCC 15.2.0)

```text
=== Example 1: jthread joins in the destructor ===
worker ran
worker finished before this line

=== Example 2: cooperative cancellation ===
stop requested, leaving the loop
loop ran at least once: 1

=== Example 3: stop_source and stop_callback ===
stop possible: 1
callback: stop was requested
stop_requested: 1
```

## Notes

- `std::print` / `std::println` need C++23, so this C++20 example prints with `std::cout`.
- The exact number of loop iterations depends on timing, so the example prints only whether the loop ran.

## Tested

- Built with CMake and with `g++ -std=c++20 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `std::jthread` | `<thread>` | C++20 |
| `std::stop_token, stop_source, stop_callback` | `<stop_token>` | C++20 |
