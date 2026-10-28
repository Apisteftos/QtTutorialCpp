# Generic templates and metaprogramming — `01_generic_templates`

## Sections in `main.cpp`

- EXAMPLE 1: Compile-Time vs Runtime Polymorphism
- EXAMPLE 2: Generic Algorithms (Type-Independent Code)
- EXAMPLE 3: Type Traits and SFINAE (Conditional Compilation)
- EXAMPLE 4: Compile-Time Computation
- EXAMPLE 5: Type-Safe Units (Compile-Time Dimensional Analysis)
- EXAMPLE 6: Template Specialization (Optimization for Specific Types)
- EXAMPLE 7: Variadic Templates (Arbitrary Number of Arguments)
- EXAMPLE 8: Perfect Forwarding (Avoiding Copies)
- MAIN - Demonstrate All Examples

## Sample output (first lines)

```text
╔═══════════════════════════════════════════════════════╗
║ GENERIC TEMPLATES AND METAPROGRAMMING                 ║
╚═══════════════════════════════════════════════════════╝

=== Example 1: Polymorphism Comparison ===

❌ Runtime (virtual):  78.53975
✅ Compile-time (template): 78.53975

=== Example 2: Generic Algorithms ===

❌ Type-specific functions:
   int sum: 15
   double sum: 7.5

✅ Generic algorithm (works with both!):
```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`): GCC 14+ or a recent Clang/MSVC with `<print>` in the standard library.

```bash
g++ -std=c++23 -O3 -Wall -Wextra main.cpp -o 01_generic_templates
./01_generic_templates
```

The included `CMakeLists.txt` also asks CMake to find Qt Core; `main.cpp` does not use Qt, so the plain `g++` command is enough.

## What was tested

Checked with GCC 13.3 and a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra`: 1 warning (`-Wunused-parameter` in `process_by_value`, from the original code, where the parameter is unused on purpose).
- Runs to completion (exit code 0) and prints 74 lines.
- Clean under AddressSanitizer + UBSan (no errors, no leaks reported).
- Output identical between two runs.
- After the text clean-up, every remaining output line is also in the output of the previous version; the lines that are gone are the moved remarks, the old company-themed banner and the closing banner.

## Not tested

- GCC 14 / the real `<print>`; MSVC; Apple Clang; Windows/macOS.
- The CMake build.
- The performance remarks below (zero cost, `-O3`) were not measured here.

## Differences in output

- `std::println` prints floating-point numbers with the shortest text that round-trips (`78.53975`), while `std::cout` used 6 significant digits (`78.5397`).
- Where an argument is a call that prints by itself (the `square(...)` demos), that output now appears **before** the line it belongs to, because the arguments are evaluated before `std::println` prints. With `cout <<` the label was already printed.
- The last item of Example 7 printed a company name before; it now prints `C++`.

## Changes made to `main.cpp`

- `std::cout` replaced by `std::println` / `std::print`; no `"\n"` at line ends, `std::println("")` for blank lines.
- `using namespace std;` removed; standard names are written with `std::`.
- Folder, project and target were renamed, and all references to a specific company were removed (comments, banner, quotes, printed text).

## Notes moved from the example functions

This text used to be printed between the demo steps or sat in comments at the end of `main.cpp`. It is lecture material, so it lives here.

### Polymorphism

- Template version: no virtual function overhead, the function can be inlined, errors are caught at compile time.

### GenericAlgorithms

- Write once, use everywhere; type-safe; no runtime overhead.

### TypeTraits

- Different implementations for different types; errors are caught at compile time; no runtime overhead.

### CompileTime

- Compile-time computation costs nothing at run time; the result is a literal in the binary; good for constants and configuration.

### TypeSafeUnits

- Units are checked at compile time; incompatible units cannot be mixed; no runtime overhead.

### TemplateSpecialization

- Specialized behavior for specific types, all resolved at compile time.

### VariadicTemplates

- Any number of arguments, type-safe, efficient (inlined).

### PerfectForwarding

- Avoids unnecessary copies, preserves value categories, essential for high-performance code.

## Notes moved from main.cpp

### Compile and run

```bash
g++ -std=c++23 -O3 -Wall -Wextra main.cpp -o example
./example
```

Requirements: a C++20 compiler for concepts and `if constexpr` (C++23 for `<print>`). The `-O3` flag is what you would use to look at the generated code when you compare abstractions with hand-written code.

### Key concepts demonstrated

1. Compile-time vs runtime polymorphism
2. Generic algorithms
3. Type traits and SFINAE
4. Compile-time computation
5. Type-safe units
6. Template specialization
7. Variadic templates
8. Perfect forwarding

### Principles

- Compute at compile time when possible.
- Use templates for generic code.
- Type safety without runtime cost.
- Use advanced techniques when they improve the code.
- Zero-cost abstractions.

### When to use templates

- Generic algorithms
- Type-safe interfaces
- Compile-time computation
- Zero-cost polymorphism
- Type manipulation

### When not to use templates

- When runtime polymorphism is needed
- When they hurt readability significantly
- For code that does not benefit from genericity
- When simpler solutions exist

### Next steps

- Study `../../TemplateMeta/` for advanced patterns.
- Combine with ranges (`../02_range_pipelines/`) for powerful abstractions.
- Practice building generic libraries.
- Read the C++20 concepts documentation.
