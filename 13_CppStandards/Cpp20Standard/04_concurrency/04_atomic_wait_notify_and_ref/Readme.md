# atomic wait/notify, atomic_ref, atomic<shared_ptr>

New atomic operations.

## Features

| Feature | Header | Standard |
|---|---|---|
| `atomic::wait, notify_one, notify_all` | `<atomic>` | C++20 |
| `std::atomic_ref` | `<atomic>` | C++20 |
| `std::atomic<std::shared_ptr<T>>` | `<memory>, <atomic>` | C++20 |
| `atomic_flag::test` | `<atomic>` | C++20 |

## What the example shows

- Example 1: wait and notify
- Example 2: atomic_ref
- Example 3: atomic shared_ptr
- Example 4: atomic_flag can be tested

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/04_atomic_wait_notify_and_ref
```

Or without CMake:

```bash
g++ -std=c++20 -Wall -Wextra -pthread main.cpp -o 04_atomic_wait_notify_and_ref
./04_atomic_wait_notify_and_ref
```

## Output (GCC 15.2.0)

```text
=== Example 1: atomic wait / notify ===
waiter woke up, state = 1

=== Example 2: atomic_ref ===
counter = 40000

=== Example 3: atomic<shared_ptr> ===
old value 5, new value 6

=== Example 4: atomic_flag::test ===
initial: 0
after test_and_set: 1
```

## Notes

- `std::print` / `std::println` need C++23, so this C++20 example prints with `std::cout`.
- `atomic_ref` lets a normal object be accessed atomically for a while; no other thread may access it non-atomically in that time.

## Tested

- Built with CMake and with `g++ -std=c++20 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `atomic::wait, notify_one, notify_all` | `<atomic>` | C++20 |
| `std::atomic_ref` | `<atomic>` | C++20 |
| `std::atomic<std::shared_ptr<T>>` | `<memory>, <atomic>` | C++20 |
| `atomic_flag::test` | `<atomic>` | C++20 |
