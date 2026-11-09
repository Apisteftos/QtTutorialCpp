# std::spanstream

Streams that read from or write into a buffer you own, without allocation.

## Features

| Feature | Header | Standard |
|---|---|---|
| `std::ispanstream, std::ospanstream, std::spanstream` | `<spanstream>` | C++23 |

## What the example shows

- Example 1: ospanstream writes into a fixed buffer
- Example 2: ispanstream reads from existing memory
- Example 3: output that does not fit

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/03_spanstream
```

Or without CMake:

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 03_spanstream
./03_spanstream
```

## Output (GCC 15.2.0)

```text
=== Example 1: ospanstream ===
wrote 8 characters: id=42 ok

=== Example 2: ispanstream ===
read 10 20 30, sum 60

=== Example 3: buffer too small ===
stream is good: false
```

## Notes

- `ospanstream::span()` returns the part of the buffer that was written.
- When the buffer is too small, the stream goes into a failed state; nothing is reallocated.

## Tested

- Built with CMake (Debug) and with `g++ -std=c++23 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Output was identical with a GCC 16 pre-release snapshot.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `std::ispanstream, std::ospanstream, std::spanstream` | `<spanstream>` | C++23 |
