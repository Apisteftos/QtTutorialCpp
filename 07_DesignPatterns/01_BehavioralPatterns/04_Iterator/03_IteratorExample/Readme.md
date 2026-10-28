# Iterator — `03_IteratorExample`

> **Intent:** Gives sequential access to the elements of a collection without exposing its internal representation.

## What this example contains

Sections in `main.cpp`:

- Menu Item Class
- Iterator Interface
- Pancake House Menu (uses std::vector)
- Diner Menu (uses fixed-size array)
- Waitress - Uses iterators uniformly
- Main - Test the Restaurant System

Types defined in `main.cpp`:

| Type | Derives from |
|---|---|
| `MenuItem` | — |
| `MenuIterator` | — |
| `PancakeHouseMenu` | — |
| `PancakeIterator` | `MenuIterator` |
| `DinerMenu` | — |
| `DinerIterator` | `MenuIterator` |
| `Waitress` | — |

## The pattern in short

- A collection that can create an iterator.
- An iterator type with the usual operations (dereference, advance, compare).

**Use it when:** You want to traverse a custom container with range-for or the standard algorithms.

**Watch out:** To work with the standard algorithms an iterator must provide the iterator_traits typedefs (or satisfy the C++20 iterator concepts).

## Sample output (first lines)

```text
===== RESTAURANT MENU SYSTEM =====

======= BREAKFAST MENU =======

K&B Pancake Breakfast - $2.99 (V)
  Pancakes with scrambled eggs and toast

Regular Pancake Breakfast - $2.99
  Pancakes with fried eggs and sausage

Blueberry Pancakes - $3.49 (V)
  Pancakes made with fresh blueberries

Waffles - $3.59 (V)
```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`), so you need GCC 14+ or a recent Clang/MSVC with a standard library that ships `<print>`.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 03_IteratorExample
./03_IteratorExample
```

The included `CMakeLists.txt` sets C++23 but also asks CMake to find Qt Core. `main.cpp` itself does not use Qt, so the plain `g++` command above is enough.

## What was tested

Checked with GCC 13.3 using a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra` without warnings.
- Runs to completion (exit code 0) and prints 47 lines.
- Clean under AddressSanitizer + UBSan (no errors, no leaks reported).
- Output is identical between two consecutive runs.
- Output of the converted program is byte-identical to the original `std::cout` version.

## Not tested

- Behaviour on Windows/macOS or with MSVC/Apple Clang.
- The CMake build (Qt is not installed in the sandbox).

## Changes made to `main.cpp`

- `std::cout` replaced by `std::println` / `std::print`; no `"\n"` at line ends, `std::println("")` for blank lines.
- `using namespace std;` removed; standard names are written with `std::`.

