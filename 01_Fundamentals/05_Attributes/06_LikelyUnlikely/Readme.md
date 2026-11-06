# `[[likely]]` and `[[unlikely]]`: which branch is usual

> **Intent:** Give the compiler a hint which branch or `case` is taken most of the time, so it can lay out the machine code for the common path.

**Since:** C++20

## Examples in `main.cpp`

| # | What | Hint |
|---|---|---|
| 1 | `if` / `else` in `parseDigit` | digit is `[[likely]]`, anything else `[[unlikely]]` |
| 2 | `switch` in `cost` | `Data` is `[[likely]]`, `Reset` is `[[unlikely]]` |
| 3 | A loop that skips invalid values | the `v < 0` branch is `[[unlikely]]` |

```cpp
if (c >= '0' && c <= '9') [[likely]] {
    return c - '0';
} else [[unlikely]] {
    return -1;
}

switch (p) {
case Packet::Data:  [[likely]]   return 1;
case Packet::Reset: [[unlikely]] return 100;
}
```

## Notes

- The attributes **never change the result**. They only influence how the compiler arranges code (branch layout, inlining). The output of the program is the same without them.
- They produce no warnings.
- Placement: after the condition for `if` / `else`, or in front of the statement after a `case` label.
- Do not guess. A wrong hint makes the program slower. Use them for clear cases (error paths, rare conditions) and measure with a profiler or `perf`.
- Before C++20 the same idea was `__builtin_expect(cond, 1)` (GCC, Clang).
- Modern CPUs predict most branches well, so the gain is often small. Do not expect big changes.

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`): GCC 14+ or a recent Clang/MSVC.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o likely
./likely
```

or with CMake:

```bash
cmake -S . -B build
cmake --build build
./build/likely
```

## Output (GCC 15.2.0)

```text
WARNING: ld.so: invalid GLIBC_TUNABLES value `default' for option `glibc.malloc.mmap_threshold': ignored.
WARNING: ld.so: invalid GLIBC_TUNABLES value `default' for option `glibc.malloc.trim_threshold': ignored.
=== EXAMPLE 1: IF / ELSE ===
parseDigit('7') = 7
parseDigit('x') = -1

=== EXAMPLE 2: SWITCH ===
cost(Data)  = 1
cost(Ack)   = 2
cost(Reset) = 100

=== EXAMPLE 3: ERROR PATH IN A LOOP ===
sumValid = 12
```

## Tested

- Builds without warnings with GCC 15.2.0, `-std=c++23 -Wall -Wextra`; also with `-O2 -DNDEBUG`. Runs clean under AddressSanitizer and UBSan.
- Not tested with Clang or MSVC.

## Reference

- Attributes `[[likely]]` and `[[unlikely]]`, C++20.
