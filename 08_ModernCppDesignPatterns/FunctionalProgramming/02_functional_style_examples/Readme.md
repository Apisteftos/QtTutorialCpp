# Core Functional Programming Concepts in C++ — `02_functional_style_examples`

**Difficulty: 🟢 Beginner**

## Key concepts

- Pure Functions
- Immutability
- Higher-Order Functions
- Function Composition
- Currying
- First-Class Functions

## Sections in `main.cpp`

- CONCEPT 1: PURE FUNCTIONS
- A pure function:
- CONCEPT 2: IMMUTABILITY
- Prefer const and avoid mutation
- CONCEPT 3: HIGHER-ORDER FUNCTIONS
- Functions that take functions as parameters or return functions
- CONCEPT 4: FUNCTION COMPOSITION
- Combining simple functions to create complex ones
- CONCEPT 5: CURRYING
- Transforming multi-argument function into sequence of single-argument functions
- CONCEPT 6: FIRST-CLASS FUNCTIONS
- Functions as values (can be stored, passed, returned)
- REAL-WORLD EXAMPLE: Event System using Functional Concepts
- MAIN

## Sample output (first lines)

```text
╔═══════════════════════════════════════════════════════╗
║       FUNCTIONAL PROGRAMMING CONCEPTS IN C++          ║
╚═══════════════════════════════════════════════════════╝

=== Pure Functions ===
Pure add(5, 3): 8 (always 8)
Pure square(4): 16 (always 16)

Impure addAndCount(5, 3): 8 (counter now: 1)
Impure addAndCount(5, 3): 8 (counter now: 2)

=== Immutability ===
Original: 1 2 3 4 5 
After mutable operation: 2 4 6 8 10  (original changed if same object!)
```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`): GCC 14+ or a recent Clang/MSVC with `<print>` in the standard library.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 02_functional_style_examples
./02_functional_style_examples
```

The included `CMakeLists.txt` also asks CMake to find Qt Core; `main.cpp` does not use Qt, so the plain `g++` command is enough.

## Notes moved from the example functions

This text used to be printed between the demo steps. It is lecture material, so it lives here.

### PureFunctions

- Impure functions give different side effects each time.
- Benefits of pure functions: easy to test, easy to reason about, can be memoized (cached), thread-safe.

### Immutability

- Benefits of immutability: no unexpected side effects, thread-safe by default, easier to reason about, data can be shared safely.

### HigherOrderFunctions

- Higher-order functions enable abstraction over behavior, code reuse and flexible composition.

### FunctionComposition

- Composition allows building complex functions from simple ones, reusable building blocks and mathematical elegance.

### Currying

- Currying enables partial application, specialized functions and more flexible composition.

### FirstClassFunctions

- Functions can be stored in variables, passed as arguments, returned from functions and selected dynamically.

### RealWorldExample

- Handlers are first-class functions, event processing is higher-order, and filtering is done by composition.

## What was tested

Checked with GCC 13.3 and a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra` without warnings.
- Runs to completion (exit code 0) and prints 61 lines.
- Clean under AddressSanitizer + UBSan (no errors, no leaks reported).
- Output identical between two runs.
- The output of the `std::cout` → `std::println` conversion matched the original. Afterwards the lecture text that `main()` used to print was moved into the notes sections below. I compared the output before and after that move: the only difference is that the moved lines are no longer printed (values that change on every run, such as addresses, thread ids or timings, were ignored).

## Not tested

- GCC 14 / the real `<print>`; MSVC; Apple Clang; Windows/macOS.
- The CMake build.

## Changes made to `main.cpp`

- `std::cout` replaced by `std::println` / `std::print`; no `"\n"` at line ends, `std::println("")` for blank lines.
- `using namespace std;` removed; standard names are written with `std::`.

## Notes moved from main.cpp

The closing banner of `main()` (a title between two separator lines, with no text) and the comment block at the end of the file are lecture material and live here.

### Summary of the six concepts

1. PURE FUNCTIONS — no side effects, deterministic
2. IMMUTABILITY — prefer `const`, create new values instead of modifying
3. HIGHER-ORDER — functions that work with functions
4. COMPOSITION — build complex from simple
5. CURRYING — partial application of functions
6. FIRST-CLASS — functions as values

### Compile and run

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o functional_style
./functional_style
```

### Learning objectives

1. Understand what makes a function "pure"
2. Learn the benefits of immutability
3. Master higher-order functions (map, filter, reduce)
4. Practice function composition
5. Understand currying and partial application
6. Use functions as first-class citizens

### Next steps

- `../03_lambda_composition/` (advanced lambda usage)
- `../04_ranges_introduction/` (C++20 ranges)
- `../../RangesAndGenericStyle/` (ranges and generic style)
