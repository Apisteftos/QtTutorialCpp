# Advanced Lambda Expressions and Composition in C++ — `03_lambda_composition`

**Difficulty: 🟡 Intermediate**

## Key concepts

- Lambda syntax and captures
- Generic lambdas (C++14)
- Recursive lambdas
- Lambda composition
- Perfect forwarding in lambdas
- Mutable lambdas
- Immediately invoked lambdas (IIFE)

## Sections in `main.cpp`

- PART 1: Lambda Basics and Capture Modes
- PART 2: Generic Lambdas (C++14)
- PART 3: Mutable Lambdas
- PART 4: Recursive Lambdas
- PART 5: Lambda Composition
- PART 6: Immediately Invoked Lambdas (IIFE)
- PART 7: Advanced Lambda Patterns
- REAL-WORLD EXAMPLE: Data Processing Pipeline
- MAIN

## Sample output (first lines)

```text

╔═══════════════════════════════════════════════════════╗
║          ADVANCED LAMBDA COMPOSITION IN C++         ║
╚═══════════════════════════════════════════════════════╝

=== Lambda Basics & Captures ===
No capture: 42
Capture by value [=]: 30
After x=100: 30 (still 30)
Capture by reference [&]: 30
After x=100: 120 (now 120)
Capture [x, &y]: 30
Init capture [z = x+y]: 30

```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`): GCC 14+ or a recent Clang/MSVC with `<print>` in the standard library.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 03_lambda_composition
./03_lambda_composition
```

The included `CMakeLists.txt` also asks CMake to find Qt Core; `main.cpp` does not use Qt, so the plain `g++` command is enough.

### Text printed by the example functions

This text used to be printed between the demo steps. It is lecture material, so it lives here.

### From `demonstrate()`

- ✅ Capture modes:

[]       - No capture  
[=]      - Capture all by value  
[&]      - Capture all by reference  
[x, &y]  - Capture x by value, y by ref

```cpp
[z = expr] - Init capture (C++14)
```

### From `demonstrate()`

- ✅ Generic lambdas enable:
  - Type-generic algorithms
  - Less template boilerplate
  - Compile-time polymorphism

### From `demonstrate()`

- ✅ Mutable lambdas:
  - Can modify captured values
  - Useful for stateful operations
  - Don't affect original variables

### From `demonstrate()`

- ✅ Recursive lambdas:
  - Need std::function for self-reference
  - Useful for tree/graph algorithms
  - Can replace small recursive functions

### From `demonstrate()`

- ✅ Lambda composition:
  - Build complex from simple
  - Readable data flow
  - Reusable transformations

### From `demonstrate()`

Setup resources...  
Process data...  
Cleanup resources...

### From `demonstrate()`

- ✅ IIFE useful for:
  - Complex const initialization
  - Scoped temporary variables
  - RAII patterns
  - One-time setup code

### From `demonstrate()`

- ✅ Advanced patterns:
  - Currying for flexible application
  - Partial application for specialization
  - Strategy pattern with lambdas

### From `demonstrate()`

- ✅ Lambda patterns in production:
  - Reusable predicates
  - Composable transformations
  - Parameterized operations
  - Type-safe callbacks

## What was tested

Checked with GCC 13.3 and a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra` without warnings.
- Runs to completion (exit code 0) and prints 158 lines.
- Clean under AddressSanitizer + UBSan (no errors, no leaks reported).
- Output identical between two runs.
- The output of the `std::cout` → `std::println` conversion matched the original. Afterwards the lecture text that `main()` used to print was moved into *Notes moved from main.cpp* below. I compared the output before and after that move: the only difference is that the moved lines are no longer printed (values that change on every run, such as addresses, thread ids or timings, were ignored).

## Not tested

- GCC 14 / the real `<print>`; MSVC; Apple Clang; Windows/macOS.
- The CMake build.

## Changes made to `main.cpp`

- `std::cout` replaced by `std::println` / `std::print`; no `"\n"` at line ends, `std::println("")` for blank lines.
- `using namespace std;` removed; standard names are written with `std::`.

## Notes moved from main.cpp

This text used to be printed by `main()`. It is lecture material, so it lives here and the program only prints the demo output.

### Notes

- ✅ Understand all capture modes [=], [&], [x], [&y]
- ✅ Use generic lambdas with auto parameters
- ✅ Know when to use mutable lambdas
- ✅ Implement recursive algorithms with lambdas
- ✅ Compose lambdas to build complex operations
- ✅ Use IIFE for complex initialization
- ✅ Apply currying and partial application
- ✅ Build production-ready pipelines

- 💡 Lambda composition helps build elegant, reusable algorithms.
