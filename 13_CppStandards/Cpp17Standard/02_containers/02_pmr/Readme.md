# Polymorphic memory resources

Containers whose memory comes from a resource chosen at run time (for example a stack buffer).

## Features

| Feature | Header | Standard |
|---|---|---|
| `std::pmr::memory_resource, monotonic_buffer_resource` | `<memory_resource>` | C++17 |
| `std::pmr::vector, std::pmr::string` | `<vector>, <string>` | C++17 |

## What the example shows

- Example 1: a stack buffer as memory source
- Example 2: when the buffer is too small, the upstream resource is used
- Example 3: pmr strings propagate their resource

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/02_pmr
```

Or without CMake:

```bash
g++ -std=c++17 -Wall -Wextra main.cpp -o 02_pmr
./02_pmr
```

## Output (GCC 15.2.0)

```text
=== Example 1: monotonic_buffer_resource on the stack ===
20 ints stored, heap allocations: 0

=== Example 2: buffer too small ===
10000 ints stored, heap allocations: 1

=== Example 3: pmr::string in a pmr::vector ===
a string that is long enough to need heap memory
string uses the vector's resource: true
```

## Notes

- `std::print` / `std::println` need C++23, so this C++17 example prints with `std::cout`.
- A `monotonic_buffer_resource` never frees single allocations; all memory is released when the resource is destroyed.
- When the buffer is used up, the upstream resource is asked for more memory (the example counts these requests).

## Tested

- Built with CMake and with `g++ -std=c++17 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `std::pmr::memory_resource, monotonic_buffer_resource` | `<memory_resource>` | C++17 |
| `std::pmr::vector, std::pmr::string` | `<vector>, <string>` | C++17 |
