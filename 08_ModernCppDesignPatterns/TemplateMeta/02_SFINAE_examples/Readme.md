# SFINAE: Substitution Failure Is Not An Error — `02_SFINAE_examples`

**Difficulty: 🔴 Advanced**

## Key concepts

- Function overload resolution
- enable_if for conditional compilation
- Detecting member functions
- Tag dispatching
- Expression SFINAE
- Concepts-like constraints (pre-C++20)

## Sections in `main.cpp`

- PART 1: Basic SFINAE - Function Overloading
- PART 2: Detecting Member Functions
- PART 3: Using Detection for Generic Code
- PART 4: Return Type SFINAE
- PART 5: Tag Dispatching
- PART 6: Expression SFINAE
- PART 7: Practical Example - Generic Serializer
- PART 8: SFINAE vs C++20 Concepts
- MAIN

## Sample output (first lines)

```text

╔═══════════════════════════════════════════════════════╗
║     SFINAE: Substitution Failure Is Not An Error    ║
╚═══════════════════════════════════════════════════════╝

=== Basic SFINAE: Function Overloading ===

Different implementations based on type:
Processing integer: 42 * 2 = 84
Processing float: 3.14 * 1.5 = 4.71
Processing string: "Hello" (length: 5)

✅ SFINAE selects correct overload at compile-time!

```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`): GCC 14+ or a recent Clang/MSVC with `<print>` in the standard library.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 02_SFINAE_examples
./02_SFINAE_examples
```

The included `CMakeLists.txt` also asks CMake to find Qt Core; `main.cpp` does not use Qt, so the plain `g++` command is enough.

## What was tested

Checked with GCC 13.3 and a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra` without warnings.
- Runs to completion (exit code 0) and prints 128 lines.
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

- ✅ SFINAE = Compile-time function selection
- ✅ Enable/disable functions based on type properties
- ✅ Detect member functions and operations
- ✅ Create truly generic code
- ✅ Better error messages than template errors

- 💡 Common SFINAE patterns:
  - std::enable_if for conditional compilation
  - std::void_t for member detection
  - Tag dispatching for cleaner code
  - Expression SFINAE for operation validation

- 💡 Typical uses of SFINAE:
  - Generic algorithm selection
  - Interface detection
  - Type-safe APIs
  - Zero-cost abstractions

🚀 Modern C++20: Use concepts when possible!  
Concepts are clearer, but SFINAE still relevant
