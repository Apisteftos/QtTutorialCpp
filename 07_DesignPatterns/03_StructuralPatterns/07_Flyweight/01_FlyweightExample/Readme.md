# Flyweight — `01_FlyweightExample`

> **Intent:** Saves memory by sharing the data that many objects have in common and keeping only the individual data in each object.

## Scenario

A forest has thousands of trees. The tree type (name, colour, texture) is shared; each tree stores only its position and a pointer to the type.

## Types in `main.cpp`

| Type | Role |
|---|---|
| `TreeType` | flyweight; shared and immutable |
| `TreeTypeFactory` | creates a type once and returns the same object afterwards |
| `Tree` | context; position plus shared type |
| `Forest` | client; plants and draws the trees |

## The pattern in short

- The factory output shows when a type is created and when it is reused.
- `std::shared_ptr<const TreeType>` makes the shared part read-only.

**Use it when:** A program needs very many similar objects and most of their data is the same.

**Watch out:** Shared data must be immutable; extracting it adds a little complexity.

## Output

```text
factory: creating new type oak/green/rough
factory: creating new type birch/white/smooth
factory: creating new type oak/autumn/rough
forest has 6 trees but only 3 tree types:
  oak (green, rough) at (1, 2)
  oak (green, rough) at (5, 3)
  birch (white, smooth) at (9, 7)
  oak (green, rough) at (12, 1)
  birch (white, smooth) at (4, 8)
  oak (autumn, rough) at (7, 7)
```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`): GCC 14+ or a recent Clang/MSVC with `<print>` in the standard library.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 01_FlyweightExample
./01_FlyweightExample
```

The included `CMakeLists.txt` also asks CMake to find Qt Core; `main.cpp` does not use Qt, so the plain `g++` command is enough.

## What was tested

Checked with GCC 13.3 and a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra` without warnings.
- Runs to completion (exit code 0) and prints 10 lines.
- Clean under AddressSanitizer + UBSan (no errors, no leaks reported).
- Output identical between two runs.

## Not tested

- GCC 14 / the real `<print>`; MSVC; Apple Clang; Windows/macOS.
- The CMake build.

## Notes

This example was rewritten with its own scenario, class names and output. It follows the usual structure of the flyweight pattern.
