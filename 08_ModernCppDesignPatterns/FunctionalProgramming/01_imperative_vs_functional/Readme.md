# Side-by-side comparison of Imperative vs Functional programming styles — `01_imperative_vs_functional`

**Difficulty: 🟢 Beginner**

## Key concepts

- Imperative: Focus on HOW (step-by-step instructions)
- Functional: Focus on WHAT (declarative transformations)

## Sections in `main.cpp`

- EXAMPLE 1: Filtering - Get even numbers
- EXAMPLE 2: Transformation - Square all numbers
- EXAMPLE 3: Filter + Transform - Even numbers squared
- EXAMPLE 4: Reduction - Sum of squares
- EXAMPLE 5: Finding - First element > 50
- EXAMPLE 6: Complex Pipeline - Real-world scenario
- EXAMPLE 7: Sorting - Different approaches
- EXAMPLE 8: State Management - Counter
- EXAMPLE 9: Lazy Evaluation - Performance benefit
- EXAMPLE 10: Composition - Building complex operations
- MAIN - Run all examples

## Sample output (first lines)

```text

╔═══════════════════════════════════════════════════════╗
║   IMPERATIVE vs FUNCTIONAL PROGRAMMING IN C++         ║
╚═══════════════════════════════════════════════════════╝

=== Example 1: Filtering Even Numbers ===

Imperative (HOW): 2 4 6 8 10 
Functional (WHAT): 2 4 6 8 10 

=== Example 2: Squaring Numbers ===

```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`): GCC 14+ or a recent Clang/MSVC with `<print>` in the standard library.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 01_imperative_vs_functional
./01_imperative_vs_functional
```

The included `CMakeLists.txt` also asks CMake to find Qt Core; `main.cpp` does not use Qt, so the plain `g++` command is enough.

## What was tested

Checked with GCC 13.3 and a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra` without warnings.
- Runs to completion (exit code 0) and prints 66 lines.
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

- ✅ Functional style is more DECLARATIVE (WHAT not HOW)
- ✅ Pipelines are COMPOSABLE and REUSABLE
- ✅ Ranges provide LAZY EVALUATION (better performance)
- ✅ No MUTABLE STATE (fewer bugs)
- ✅ Code is more READABLE and MAINTAINABLE
- ✅ Better for PARALLEL processing (no shared state)

- 💡 Style: prefer generic algorithms and ranges over hand-written loops.

### Remarks printed after each example

Each example used to end with a one-line remark. They are the point of each comparison:

1. **Filtering:** ✅ Functional is more declarative - says WHAT not HOW
2. **Transformation:** ✅ Functional expresses transformation clearly
3. **Filter + transform:** ✅ Pipeline clearly shows data flow: filter → transform
4. **Reduction:** ✅ Functional separates transformation from aggregation
5. **Finding:** ✅ Functional is more expressive about intent
6. **Complex pipeline:** ✅ Functional pipeline reads like English: `'people' | filter(conditions) | transform(extract name)`
7. **Sorting:** ✅ Functional hides complexity, focuses on intent
8. **State management:** ✅ Functional avoids mutable state
9. **Lazy evaluation:** ✅ Ranges are lazy - compute only what's needed!
10. **Composition:** ✅ Functional allows reusable, composable operations

### Compile and run (comment block that was at the end of main.cpp)

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o imperative_vs_functional
./imperative_vs_functional
```

(The old comment said `-std=c++20`; `main.cpp` uses `std::println`, so it needs `-std=c++23`.)

**Expected output:** clear side-by-side comparisons showing

- how imperative focuses on steps (HOW)
- how functional focuses on transformations (WHAT)
- the benefits of the functional style

**Key learning points:**

1. Imperative = step-by-step instructions, mutable state
2. Functional = declarative transformations, immutable data
3. Ranges = lazy evaluation + composition
4. Pipelines = readable data flow

### Key takeaways heading

The closing block `KEY TAKEAWAYS:` in `main()` printed only two separator lines and no text, so it was removed. The takeaways are the list above.
