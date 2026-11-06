# `[[no_unique_address]]`: an empty member takes no space

> **Intent:** Allow an empty data member (no state) to share the address of another member, so it adds nothing to the size of the class.

**Since:** C++20

## Examples in `main.cpp`

| # | What | Size on x86-64 |
|---|---|---|
| 1 | `struct Plain { int* data; Empty tag; }` | 16 bytes (1 byte for `tag`, 7 bytes padding) |
| 2 | `struct Compact { int* data; [[no_unique_address]] Empty tag; }` | 8 bytes |
| 3 | `SortedPair<Compare>` with a stateless comparator | 8 bytes, the same as two `int` |
| 4 | Two empty members of the **same** type vs of different types | 2 bytes vs 1 byte |

## Why it exists

Every object has its own address, so even an empty `struct` has size 1. If you store a stateless helper (comparator, allocator, deleter) as a member, it makes the class larger by one byte plus padding. Before C++20 the trick was to inherit from the helper (the "empty base optimization"). `[[no_unique_address]]` does the same for plain members, which is simpler and works with `final` classes too.

## Notes

- It is a permission, not a promise: the compiler **may** overlap the member, and it is allowed to ignore the attribute.
- Two members of the **same** type cannot share an address, because two different objects of one type must be distinguishable by address (example 4: size 2).
- MSVC ignores the attribute and needs `[[msvc::no_unique_address]]` instead.
- The attribute is only useful when a type can be empty. Do not add it to members with data.
- The standard library uses this kind of storage for comparators, allocators and deleters inside containers and smart pointers.

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`): GCC 14+ or a recent Clang/MSVC.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o no_unique_address
./no_unique_address
```

or with CMake:

```bash
cmake -S . -B build
cmake --build build
./build/no_unique_address
```

## Output (GCC 15.2.0)

```text
WARNING: ld.so: invalid GLIBC_TUNABLES value `default' for option `glibc.malloc.mmap_threshold': ignored.
WARNING: ld.so: invalid GLIBC_TUNABLES value `default' for option `glibc.malloc.trim_threshold': ignored.
=== EXAMPLES 1 AND 2: SIZES ===
sizeof(Empty)   = 1
sizeof(Plain)   = 16
sizeof(Compact) = 8

=== EXAMPLE 3: STATELESS POLICY ===
sorted: 4 9
sizeof(SortedPair<Less>) = 8 (two ints = 8)

=== EXAMPLE 4: SAME TYPE vs DIFFERENT TYPES ===
sizeof(SameType)       = 2
sizeof(DifferentTypes) = 1
```

## Tested

- Builds without warnings with GCC 15.2.0, `-std=c++23 -Wall -Wextra`; also with `-O2 -DNDEBUG`. Runs clean under AddressSanitizer and UBSan.
- Not tested with Clang or MSVC.

## Reference

- Attribute `[[no_unique_address]]`, C++20. Sizes measured with `sizeof` on GCC 15.2.0, x86-64.
