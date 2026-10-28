# Compile-Time Computations with Template Metaprogramming — `03_compile_time_computation`

**Difficulty: 🔴 Advanced**

## Key concepts

- Template recursion
- Compile-time values
- constexpr functions
- Template specialization for base cases
- Modern vs Classic TMP

## Sections in `main.cpp`

- PART 1: Classic Template Metaprogramming - Factorial
- PART 2: Modern C++11 - constexpr Functions
- PART 3: Fibonacci - Both Styles
- PART 4: Power Calculation
- PART 5: GCD (Greatest Common Divisor)
- PART 6: Prime Number Check
- PART 7: Compile-Time String Length
- PART 8: Compile-Time Array Sum
- PART 9: Type Size Comparison
- PART 10: Compile-Time Selection
- MAIN

## Sample output (first lines)

```text

╔═══════════════════════════════════════════════════════╗
║       COMPILE-TIME COMPUTATION WITH TMP             ║
╚═══════════════════════════════════════════════════════╝

=== Classic TMP: Factorial ===
All computed at COMPILE-TIME!
Factorial<5>::value = 120
Factorial<10>::value = 3628800
Factorial<12>::value = 479001600

✅ Zero runtime cost - computed during compilation!

=== Modern C++11: constexpr Factorial ===
```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`): GCC 14+ or a recent Clang/MSVC with `<print>` in the standard library.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 03_compile_time_computation
./03_compile_time_computation
```

The included `CMakeLists.txt` also asks CMake to find Qt Core; `main.cpp` does not use Qt, so the plain `g++` command is enough.

## What was tested

Checked with GCC 13.3 and a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra` without warnings.
- Runs to completion (exit code 0) and prints 85 lines.
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

- ✅ Template metaprogramming = Compile-time computation
- ✅ Zero runtime cost - all work done by compiler
- ✅ Classic TMP uses template recursion + specialization
- ✅ Modern C++ uses constexpr (cleaner syntax)
- ✅ Both achieve same goal: zero-cost abstractions
- ✅ Use for: array sizes, type selection, optimization

- 💡 Typical uses of these techniques:
  - Zero-cost generic algorithms
  - Compile-time type checking
  - Optimal code generation
