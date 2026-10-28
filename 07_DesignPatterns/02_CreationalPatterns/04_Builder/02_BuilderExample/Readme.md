# Builder — `02_BuilderExample`

> **Intent:** Separates the construction of a complex object from its representation so the same process can create different results, step by step.

## What this example contains

Sections in `main.cpp`:

- PRODUCT - The complex object being built
- BUILDER INTERFACE
- CONCRETE BUILDER 1 - Gaming PC
- CONCRETE BUILDER 2 - Office PC
- CONCRETE BUILDER 3 - Budget PC
- CONCRETE BUILDER 4 - Workstation
- DIRECTOR
- FLUENT BUILDER (Alternative Approach)
- MAIN - DEMONSTRATING THE PATTERN
- Example 1: Gaming PC
- Example 2: Office PC
- Example 3: Budget PC (Minimal Build)
- Example 4: Workstation
- Example 5: Manual Building (Without Director)
- Example 6: Fluent Builder (Method Chaining)
- Benefits Summary

Types defined in `main.cpp`:

| Type | Derives from |
|---|---|
| `Computer` | — |
| `ComputerBuilder` | — |
| `GamingPCBuilder` | `ComputerBuilder` |
| `OfficePCBuilder` | `ComputerBuilder` |
| `BudgetPCBuilder` | `ComputerBuilder` |
| `WorkstationBuilder` | `ComputerBuilder` |
| `ComputerDirector` | — |
| `FluentComputerBuilder` | — |

## The pattern in short

- A builder with step methods (often returning *this for chaining).
- A product.
- Optionally a director that knows the build order.

**Use it when:** An object has many optional parts or a constructor with many parameters.

**Watch out:** More code than a constructor; validate in build() so incomplete objects cannot escape.

## Sample output (first lines)

```text
========================================
BUILDER PATTERN DEMO
========================================

=== EXAMPLE 1: Gaming PC ===

╔════════════════════════════════════╗
║      COMPUTER SPECIFICATIONS       ║
╚════════════════════════════════════╝
CPU:            Intel Core i9-13900K (24-core, 5.8GHz)
RAM:            64GB DDR5 6000MHz RGB
Storage:        2TB NVMe SSD + 4TB HDD
GPU:            NVIDIA RTX 4090 24GB
Motherboard:    ASUS ROG Maximus Z790
```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`), so you need GCC 14+ or a recent Clang/MSVC with a standard library that ships `<print>`.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 02_BuilderExample
./02_BuilderExample
```

The included `CMakeLists.txt` sets C++23 but also asks CMake to find Qt Core. `main.cpp` itself does not use Qt, so the plain `g++` command above is enough.

## What was tested

Checked with GCC 13.3 using a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra` without warnings.
- Runs to completion (exit code 0) and prints 132 lines.
- Clean under AddressSanitizer + UBSan (no errors, no leaks reported).
- Output is identical between two consecutive runs.
- The output of the `std::cout` → `std::println` conversion matched the original. Afterwards the lecture text that `main()` used to print was moved into *Notes moved from main.cpp* below. I compared the output before and after that move: the only difference is that the moved lines are no longer printed (values that change on every run, such as addresses, thread ids or timings, were ignored).

## Not tested

- Behaviour on Windows/macOS or with MSVC/Apple Clang.
- The CMake build (Qt is not installed in the sandbox).

## Changes made to `main.cpp`

- `std::cout` replaced by `std::println` / `std::print`; no `"\n"` at line ends, `std::println("")` for blank lines.
- `using namespace std;` removed; standard names are written with `std::`.

## Notes moved from main.cpp

This text used to be printed by `main()`. It is lecture material, so it lives here and the program only prints the demo output.

### BENEFITS OF BUILDER PATTERN

- ✅ Construct objects step-by-step
- ✅ Use same construction code for different representations
- ✅ Isolate complex construction code from business logic
- ✅ Avoid 'telescoping constructor' antipattern
- ✅ Build objects that require multi-step construction
- ✅ Create immutable objects with many optional fields

### WHEN TO USE BUILDER

1. Object has many optional parameters
2. Construction process is complex
3. Need different representations of same object
4. Want to avoid telescoping constructors
5. Building immutable objects

### REAL-WORLD EXAMPLES (closing summary)

- StringBuilder in Java/C#
- SQL Query builders (SELECT, WHERE, JOIN)
- HTML/XML document builders
- HTTP request builders
- Configuration objects
- Fast food meal builders
- Game character creators
