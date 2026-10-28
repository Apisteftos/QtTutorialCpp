# Introduction to C++20 Ranges Library — `04_ranges_introduction`

**Difficulty: 🟡 Intermediate**

## Key concepts

- Range views and adaptors
- Lazy evaluation
- Pipe operator |
- Common range algorithms
- Custom ranges
- Range-based transformations

## Sections in `main.cpp`

- PART 1: Basic Range Views
- PART 2: Lazy Evaluation - The Power of Ranges
- PART 3: Composing Multiple Views (Pipelines)
- PART 4: Common Range Algorithms
- PART 5: Working with Strings
- PART 6: iota - Generate Sequences
- PART 7: join and flatten
- PART 8: elements - Access Tuple Elements
- PART 9: Real-World Example - Data Processing
- PART 10: Performance and Best Practices
- MAIN

## Sample output (first lines)

```text

╔═══════════════════════════════════════════════════════╗
║              C++20 RANGES INTRODUCTION              ║
╚═══════════════════════════════════════════════════════╝

=== Basic Range Views ===

filter (even numbers): 2 4 6 8 10 
transform (square): 1 4 9 16 25 36 49 64 81 100 
take (first 5): 1 2 3 4 5 
drop (skip first 5): 6 7 8 9 10 
reverse: 10 9 8 7 6 5 4 3 2 1 

✅ Views are:
```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`): GCC 14+ or a recent Clang/MSVC with `<print>` in the standard library.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 04_ranges_introduction
./04_ranges_introduction
```

The included `CMakeLists.txt` also asks CMake to find Qt Core; `main.cpp` does not use Qt, so the plain `g++` command is enough.

### Text printed by the example functions

This text used to be printed between the demo steps. It is lecture material, so it lives here.

### From `demonstrate()`

- ✅ Views are:
  - Lazy (computed on-demand)
  - Composable (chain multiple views)
  - Non-owning (don't copy data)

### From `demonstrate()`

- ✅ Notice:
  - Only filtered/transformed 6 elements (not all 10!)
  - Stopped after finding 3 matches
  - This is LAZY EVALUATION!

### From `demonstrate()`

- ✅ Pipelines are:
  - Readable (left-to-right data flow)
  - Efficient (lazy evaluation)
  - Composable (build complex from simple)

### From `demonstrate()`

- ✅ Range algorithms:
  - Cleaner syntax than iterator-based
  - Work directly with ranges
  - More expressive

### From `demonstrate()`

- 💡 C++20 note: split returns subranges, not strings.

```cpp
C++23 adds ranges::to<> for easier conversion.
```

- ✅ String ranges enable:
  - Character-level transformations
  - Easy filtering
  - String splitting

### From `demonstrate()`

- ✅ iota is useful for:
  - Generating sequences
  - Index generation
  - Infinite sequences (with take)

### From `demonstrate()`

- ✅ join useful for:
  - Flattening nested containers
  - Processing multi-level data
  - Combining sequences

### From `demonstrate()`

- ✅ elements useful for:
  - Extracting from pairs/tuples
  - Working with map-like structures
  - Projecting specific fields

### From `demonstrate()`

- ✅ Ranges enable clean data pipelines:
  - Readable business logic
  - Easy to modify and extend
  - Efficient (lazy evaluation)
  - Type-safe

### From `demonstrate()`

- ✅ DO: Use ranges for clarity

```cpp
auto result = data | filter(...) | transform(...);
```

- ✅ DO: Leverage lazy evaluation

```cpp
auto result = data | view1 | view2 | take(10);
```

(Only processes what's needed!)

- ✅ DO: Compose reusable views

```cpp
auto evens = numbers | filter(is_even);
auto squared_evens = evens | transform(square);
```

- ❌ DON'T: Over-complicate simple cases

For simple loops, traditional for may be clearer

- ❌ DON'T: Materialize unless needed

Views are lazy - don't convert to vector unnecessarily

- 💡 Performance tips:
  - Views don't copy data (zero overhead)
  - Lazy evaluation = better performance
  - Composable = reusable code
  - Type-safe = fewer bugs

- 💡 C++23 upgrade: ranges::to<> makes conversions easier

```cpp
auto vec = range | ranges::to<std::vector>(); // C++23
```

## What was tested

Checked with GCC 13.3 and a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra` without warnings.
- Runs to completion (exit code 0) and prints 214 lines.
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

- ✅ Understand lazy evaluation
- ✅ Use pipe operator for composition
- ✅ Know common views (filter, transform, take, drop)
- ✅ Use range algorithms (sort, find, count, etc.)
- ✅ Work with iota for sequences
- ✅ Flatten with join
- ✅ Extract with elements/keys/values
- ✅ Build production pipelines

- 💡 Style: prefer generic algorithms and ranges over hand-written loops.

🚀 Ranges are the FUTURE of C++ collections!
