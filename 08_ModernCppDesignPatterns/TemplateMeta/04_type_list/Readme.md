# Type Lists and Compile-Time Type Manipulation — `04_type_list`

**Difficulty: 🔴 Advanced (Expert Level)**

## Key concepts

- Type lists (compile-time type collections)
- Recursive type manipulation
- Type list operations (append, find, transform)
- Parameter packs
- Variadic templates

## Sections in `main.cpp`

- COMMON TYPE LIST DEFINITION (used across namespaces)
- PART 1: Basic Type List
- PART 2: Modern Type List with Variadic Templates
- PART 3: Type List Operations - Append
- PART 4: Type List Search - Contains
- PART 5: Type List Transformations
- PART 6: Type List Filter
- PART 7: Practical Application - Variant Helper
- PART 8: Type List Algorithms - All/Any
- MAIN

## Sample output (first lines)

```text

╔═══════════════════════════════════════════════════════╗
║            TYPE LISTS AND TYPE MANIPULATION         ║
╚═══════════════════════════════════════════════════════╝

=== Basic Type List ===

Type list created: int, double, char
  Head: i
  Tail head: d

✅ Type list = compile-time data structure!

=== Modern Type List ===
```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`): GCC 14+ or a recent Clang/MSVC with `<print>` in the standard library.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 04_type_list
./04_type_list
```

The included `CMakeLists.txt` also asks CMake to find Qt Core; `main.cpp` does not use Qt, so the plain `g++` command is enough.

## What was tested

Checked with GCC 13.3 and a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra` without warnings.
- Runs to completion (exit code 0) and prints 111 lines.
- Clean under AddressSanitizer + UBSan (no errors, no leaks reported).
- Output identical between two runs.
- Output differs from the original `std::cout` version only where `std::println` formats differently (see "Differences in output").

## Differences in output

`std::cout` printed `bool` values as `1`/`0`; `std::println` prints `true`/`false`. That is the only difference to the original output.

## Not tested

- GCC 14 / the real `<print>`; MSVC; Apple Clang; Windows/macOS.
- The CMake build.

## Changes made to `main.cpp`

- `std::cout` replaced by `std::println` / `std::print`; no `"\n"` at line ends, `std::println("")` for blank lines.
- `using namespace std;` removed; standard names are written with `std::`.

## Notes moved from main.cpp

This text used to be printed by `main()`. It is lecture material, so it lives here and the program only prints the demo output.

### Notes

- ✅ Type lists = compile-time type collections
- ✅ Operations: push, pop, search, filter, transform
- ✅ Algorithms: all, any, contains, index
- ✅ All computed at compile-time!
- ✅ Zero runtime cost

- 💡 Practical uses:
  - Type-safe variants
  - Generic visitors
  - Compile-time polymorphism
  - Meta-data structures

- 💡 Typical uses of type lists:
  - Generic algorithm selection
  - Type-safe interfaces
  - Compile-time dispatch
  - Zero-cost abstractions

🚀 This is EXPERT-level template metaprogramming!
