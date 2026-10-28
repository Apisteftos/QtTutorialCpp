# Type Traits and Type Manipulation — `01_template_traits`

**Difficulty: 🟡 Intermediate**

## Key concepts

- Standard library type traits
- Custom type traits
- Type transformations
- Compile-time type queries
- Practical applications

## Sections in `main.cpp`

- PART 1: Standard Library Type Traits
- PART 2: Custom Type Traits
- PART 3: Type Transformations
- PART 4: Conditional Types
- PART 5: Enable If Pattern
- PART 6: Decay Type
- PART 7: Common Type
- PART 8: Underlying Type (for enums)
- PART 9: Practical Application - Type-Safe ID
- PART 10: Practical Application - Generic Serialization
- MAIN

## Sample output (first lines)

```text

╔═══════════════════════════════════════════════════════╗
║           TYPE TRAITS AND TYPE MANIPULATION         ║
╚═══════════════════════════════════════════════════════╝

=== Standard Library Type Traits ===

Analyzing type: i
  Is integral: true
  Is floating point: false
  Is pointer: false
  Is reference: false
  Is const: false
  Is class: false
```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`): GCC 14+ or a recent Clang/MSVC with `<print>` in the standard library.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 01_template_traits
./01_template_traits
```

The included `CMakeLists.txt` also asks CMake to find Qt Core; `main.cpp` does not use Qt, so the plain `g++` command is enough.

## What was tested

Checked with GCC 13.3 and a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra` without warnings.
- Runs to completion (exit code 0) and prints 172 lines.
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

- ✅ Use type traits to query type properties
- ✅ Transform types at compile-time
- ✅ Enable/disable code based on types
- ✅ Create type-safe abstractions
- ✅ Build generic, reusable code

- 💡 Typical uses of type traits:
  - Type-safe interfaces
  - Generic algorithm selection
  - Compile-time optimizations
  - Zero-cost abstractions
