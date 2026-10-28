# Range pipelines — `02_range_pipelines`

## Sections in `main.cpp`

- EXAMPLE 1: Data Processing Pipeline
- EXAMPLE 2: Report Data Processing
- EXAMPLE 3: Range Composition (Reusable Named Operations)
- EXAMPLE 4: Lazy Evaluation (Performance Benefit)
- EXAMPLE 5: Working with Maps (Key-Value Processing)
- EXAMPLE 6: Index-Based Processing (When You Need It)
- EXAMPLE 7: Range Algorithms (STL Algorithms with Ranges)
- EXAMPLE 8: Real-World: Text Processing Pipeline
- MAIN - Demonstrate All Examples

## Sample output (first lines)

```text
╔═══════════════════════════════════════════════════════╗
║ RANGE-BASED PROGRAMMING                               ║
╚═══════════════════════════════════════════════════════╝

=== Example 1: Data Processing Pipeline ===

Input: -5 2 -3 4 6 1 8 -2 10 

Imperative result: 4 16 36 64 100 

Functional result:  4 16 36 64 100 

=== Example 2: Report Data Processing ===
```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`): GCC 14+ or a recent Clang/MSVC with `<print>` in the standard library.

```bash
g++ -std=c++23 -O3 -Wall -Wextra main.cpp -o 02_range_pipelines
./02_range_pipelines
```

The included `CMakeLists.txt` also asks CMake to find Qt Core; `main.cpp` does not use Qt, so the plain `g++` command is enough.

## What was tested

Checked with GCC 13.3 and a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra` without warnings.
- Runs to completion (exit code 0) and prints 96 lines.
- Clean under AddressSanitizer + UBSan (no errors, no leaks reported).
- Output identical between two runs.
- After the text clean-up, every remaining output line is also in the output of the previous version; the lines that are gone are the moved remarks, the old company-themed banner and the closing banner.

## Not tested

- GCC 14 / the real `<print>`; MSVC; Apple Clang; Windows/macOS.
- The CMake build.
- The performance remarks below (zero cost, `-O3`) were not measured here.

## Changes made to `main.cpp`

- `std::cout` replaced by `std::println` / `std::print`; no `"\n"` at line ends, `std::println("")` for blank lines.
- `using namespace std;` removed; standard names are written with `std::`.
- Folder, project and target were renamed, and all references to a specific company were removed (comments, banner, quotes, printed text).

## Notes moved from the example functions

This text used to be printed after the examples or sat in comments at the end of `main.cpp`. It is lecture material, so it lives here.

### DataProcessing

- The functional version is more readable and composable.
- Same performance expected (zero-cost abstraction) — not measured here.

### ChartProcessing

- The pipeline shows its steps: filter → transform.
- It is easy to add more processing steps.

### RangeComposition

- Pipelines are composable and reusable.
- Named operations express intent clearly.

### LazyEvaluation

- Lazy evaluation computes only what is actually used (see the `[Computing n]` lines in the output).

### MapProcessing

- Ranges work with maps; structured bindings make the loop body cleaner.

### IndexedProcessing

- Sometimes you need indices, and that is fine. Use the right tool for the job.

### RangeAlgorithms

- Range algorithms are cleaner than iterator pairs and work directly on containers.

### TextProcessing

- A clear data-extraction pipeline that is easy to extend with more filters or transforms.

## Notes moved from main.cpp

### Compile and run

```bash
g++ -std=c++23 -O3 -Wall -Wextra main.cpp -o example
./example
```

Requirements: a C++20 compiler for ranges (C++23 for `<print>`). Use `-O3` when you compare the generated code with hand-written loops.

### Key concepts demonstrated

1. Imperative vs functional comparison
2. Data processing pipelines
3. Range composition and reuse
4. Lazy evaluation
5. Working with complex data structures
6. Range algorithms
7. Real-world text processing

### Principles

- Prefer ranges over raw loops.
- Build pipelines from composable parts.
- Name operations to express intent.
- Use lazy evaluation where it saves work.
- Trust the compiler for optimization, and measure.

### When to use ranges

- Data transformation pipelines
- Filtering and mapping operations
- Complex multi-step processing
- When you need composability

### When to use loops

- Simple iteration without transformation
- When you need low-level control
- Performance-critical tight loops (profile first)
- Very simple operations (for display, etc.)

### Next steps

- Study `../01_generic_templates/` for template patterns.
- Practice building your own range pipelines.
- Explore Boost.Range for more advanced features.
- Read the C++23 ranges additions (`views::enumerate`, etc.).
