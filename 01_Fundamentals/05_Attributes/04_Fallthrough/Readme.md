# `[[fallthrough]]`: falling into the next case is intended

> **Intent:** Tell the compiler and the reader that a `case` deliberately continues into the next one, so `-Wimplicit-fallthrough` stays quiet.

**Since:** C++17

## Examples in `main.cpp`

| # | What | Result |
|---|---|---|
| 1 | Cascading `switch`: Admin gets manage + write + read | `[[fallthrough]];` at the end of each case that continues |
| 2 | Several `case` labels in a row (`'a'`, `'e'`, `'i'`, ...) | no attribute needed, this is not a fall-through |
| 3 | The same switch without the attribute | `-Wimplicit-fallthrough` warns |

```cpp
case Level::Admin:
    result += "manage ";
    [[fallthrough]];          // a statement of its own, directly before the next label
case Level::User:
    result += "write ";
    [[fallthrough]];
case Level::Guest:
    result += "read";
    break;
```

## What the compiler says

Compiling the commented-out version (marked `//!`) without the attribute (GCC 15.2.0, `-Wall -Wextra`; `-Wimplicit-fallthrough` is part of `-Wextra`):

```text
warning: this statement may fall through [-Wimplicit-fallthrough=]
warning: this statement may fall through [-Wimplicit-fallthrough=]
```

## Notes

- The attribute is its own statement (`[[fallthrough]];`) and must be the last statement before the next `case` or `default` label.
- A forgotten `break` is one of the oldest C++ bugs. The warning catches it, and the attribute shows that a missing `break` is not a mistake.
- Empty labels in a row are never reported, so do not add the attribute there.
- With only `-Wall` this warning is off in GCC; use `-Wextra` or `-Wimplicit-fallthrough`.

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`): GCC 14+ or a recent Clang/MSVC.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o fallthrough
./fallthrough
```

or with CMake:

```bash
cmake -S . -B build
cmake --build build
./build/fallthrough
```

## Output (GCC 15.2.0)

```text
WARNING: ld.so: invalid GLIBC_TUNABLES value `default' for option `glibc.malloc.mmap_threshold': ignored.
WARNING: ld.so: invalid GLIBC_TUNABLES value `default' for option `glibc.malloc.trim_threshold': ignored.
=== EXAMPLE 1: CASCADING SWITCH ===
Guest: read
User:  write read
Admin: manage write read

=== EXAMPLE 2: GROUPED LABELS ===
isVowel('e') = true
isVowel('x') = false

=== EXAMPLE 3: WITHOUT THE ATTRIBUTE ===
see Readme.md: -Wimplicit-fallthrough warns, the code would still run the same
```

## Tested

- Builds without warnings with GCC 15.2.0, `-std=c++23 -Wall -Wextra`; also with `-O2 -DNDEBUG`. Runs clean under AddressSanitizer and UBSan.
- The warnings shown above were produced by compiling a copy of `main.cpp` with the `//!` lines enabled.
- Not tested with Clang or MSVC.

## Reference

- Attribute `[[fallthrough]]`, C++17.
