# shared_mutex, scoped_lock, interference size

Reader/writer locking, deadlock-free locking of several mutexes, cache line size.

## Features

| Feature | Header | Standard |
|---|---|---|
| `std::shared_mutex, std::shared_lock` | `<shared_mutex>` | C++17 |
| `std::scoped_lock` | `<mutex>` | C++17 |
| `std::hardware_destructive_interference_size` | `<new>` | C++17 |

## What the example shows

- Example 1: shared_mutex
- Example 2: scoped_lock with two mutexes
- Example 3: avoiding false sharing

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/01_shared_mutex_scoped_lock
```

Or without CMake:

```bash
g++ -std=c++17 -Wall -Wextra -pthread main.cpp -o 01_shared_mutex_scoped_lock
./01_shared_mutex_scoped_lock
```

## Output (GCC 15.2.0)

```text
=== Example 1: shared_mutex ===
4 readers read the value, sum = 168

=== Example 2: scoped_lock ===
balances: 100 and 100 (total 200)

=== Example 3: interference size ===
destructive interference size: 64 bytes
sizeof(PaddedCounter): 64
```

## Notes

- `std::print` / `std::println` need C++23, so this C++17 example prints with `std::cout`.
- `scoped_lock` locks both mutexes with a deadlock-avoidance algorithm, so the two threads that lock in opposite order cannot deadlock.
- The interference size is checked with `__cpp_lib_hardware_interference_size`.

## Tested

- Built with CMake and with `g++ -std=c++17 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `std::shared_mutex, std::shared_lock` | `<shared_mutex>` | C++17 |
| `std::scoped_lock` | `<mutex>` | C++17 |
| `std::hardware_destructive_interference_size` | `<new>` | C++17 |
