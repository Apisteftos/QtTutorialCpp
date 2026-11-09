# byteswap and start_lifetime_as

Reverse bytes of an integer, and tell the compiler that an object lives in a byte buffer.

## Features

| Feature | Header | Standard |
|---|---|---|
| `std::byteswap` | `<bit>` | C++23 |
| `std::start_lifetime_as` | `<memory>` | C++23 |

## What the example shows

- Example 1: byteswap
- Example 2: start_lifetime_as

## Build and run

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/03_byteswap_and_lifetime
```

Or without CMake:

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 03_byteswap_and_lifetime
./03_byteswap_and_lifetime
```

## Output (GCC 15.2.0)

```text
=== Example 1: byteswap ===
0x11223344 -> 0x44332211
16 bit: 0xaabb -> 0xbbaa
host value: 256

=== Example 2: start_lifetime_as ===
(start_lifetime_as not available in this library, used memcpy) magic 0xcafebabe, version 3
```

### Output with a GCC 16 snapshot (pre-release)

```text
=== Example 1: byteswap ===
0x11223344 -> 0x44332211
16 bit: 0xaabb -> 0xbbaa
host value: 256

=== Example 2: start_lifetime_as ===
magic 0xcafebabe, version 3, flags 7
```

## Notes

- **`start_lifetime_as` is not in libstdc++ 15.** The example checks `__cpp_lib_start_lifetime_as` and falls back to `memcpy`. The GCC 16 snapshot takes the real path.

## Tested

- Built with CMake (Debug) and with `g++ -std=c++23 -Wall -Wextra` using GCC 15.2.0 on Ubuntu 26.04: no warnings.
- Run once with AddressSanitizer and UndefinedBehaviorSanitizer: no findings.
- Also built and run with a GCC 16 pre-release snapshot (16-20260322); that compiler is not a release.
- Not tested: Clang/libc++, MSVC, other operating systems.

## Reference

| Feature | Header | Standard |
|---|---|---|
| `std::byteswap` | `<bit>` | C++23 |
| `std::start_lifetime_as` | `<memory>` | C++23 |
