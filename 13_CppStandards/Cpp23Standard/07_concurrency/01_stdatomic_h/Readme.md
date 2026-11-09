# <stdatomic.h>

C compatible atomics: `_Atomic(T)` and `atomic_*` functions usable in both C and C++ headers.

## Features

| Feature | Header | Standard |
|---|---|---|
| `<stdatomic.h>, _Atomic(T)` | `<stdatomic.h>` | C++23 |

## What the example shows

- Example 1: _Atomic(T) and atomic_fetch_add
- Example 2: exchange and compare_exchange

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/01_stdatomic_h
```

Or without CMake:

```bash
g++ -std=c++23 -Wall -Wextra -pthread main.cpp -o 01_stdatomic_h
./01_stdatomic_h
```

## Output (GCC 15.2.0)

```text
=== Example 1: counter shared by several threads ===
counter = 40000

=== Example 2: exchange and compare_exchange ===
exchange: old 40000, new 5
compare_exchange(5 -> 9): true, value 9
```

## Notes

- In pure C++ code `std::atomic<T>` is the normal choice; `<stdatomic.h>` is for code shared with C.
- `std::jthread`, `std::latch`, `std::barrier`, `std::stop_token` and `std::counting_semaphore` are C++20 and belong to the multithreading area.

## Tested

- Built with CMake (Debug) and with `g++ -std=c++23 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `<stdatomic.h>, _Atomic(T)` | `<stdatomic.h>` | C++23 |
