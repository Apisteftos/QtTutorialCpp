# Iterator — `01_IteratorExample`

> **Intent:** Lets you go through the elements of a collection without exposing how the collection stores them.

## Scenario

A fixed-size ring buffer overwrites its oldest element when it is full. Its nested `Iterator` follows the C++ convention (`begin()`, `end()`, `operator*`, `++`, `!=`), so a range-based `for` loop works.

## Types in `main.cpp`

| Type | Role |
|---|---|
| `RingBuffer<T` / `Capacity>` | the collection; stores elements in a `std::array` |
| `RingBuffer::Iterator` | the iterator; knows how to walk the ring from the oldest to the newest element |

## The pattern in short

- The loop `for (int v : numbers)` never sees head, size or array index.
- A C++ iterator needs `operator*`, `operator++` and `operator!=` for range-for to work.

**Use it when:** You write your own container and want it to work with range-for and the standard algorithms.

**Watch out:** A full standard iterator needs more members (`iterator_category`, `value_type`, ...) before standard algorithms accept it; this example keeps only what range-for needs.

## Output

```text
numbers (capacity 4, pushed 1..6):
  3
  4
  5
  6
words (2 stored):
  alpha
  beta
```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`): GCC 14+ or a recent Clang/MSVC with `<print>` in the standard library.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 01_IteratorExample
./01_IteratorExample
```

The included `CMakeLists.txt` also asks CMake to find Qt Core; `main.cpp` does not use Qt, so the plain `g++` command is enough.

## What was tested

Checked with GCC 13.3 and a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra` without warnings.
- Runs to completion (exit code 0) and prints 8 lines.
- Clean under AddressSanitizer + UBSan (no errors, no leaks reported).
- Output identical between two runs.

## Not tested

- GCC 14 / the real `<print>`; MSVC; Apple Clang; Windows/macOS.
- The CMake build.

## Notes

This example was rewritten with its own scenario, class names and output. It follows the usual structure of the iterator pattern.
