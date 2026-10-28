# Memento — `01_MementoExample`

> **Intent:** Saves and restores the state of an object without exposing its internals.

## Scenario

A whiteboard keeps text lines. A history object stores snapshots and restores them step by step (undo).

## Types in `main.cpp`

| Type | Role |
|---|---|
| `Whiteboard` | originator; creates and restores snapshots |
| `Whiteboard::Snapshot` | memento; its data is private and only the whiteboard is a friend |
| `History` | caretaker; stores snapshots but cannot read them |

## The pattern in short

- The caretaker only stores and returns snapshots.
- The private constructor and `friend` make the memento opaque.

**Use it when:** You need undo/redo or checkpoints and do not want to expose the internal state.

**Watch out:** Snapshots of big objects use a lot of memory; store only the differences if needed.

## Output

```text
whiteboard (3 lines):
  Agenda
  1. Budget
  2. Hiring
history: restored snapshot with 1 lines
whiteboard (1 lines):
  Agenda
history: restored snapshot with 0 lines
whiteboard (0 lines):
history: nothing to undo
```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`): GCC 14+ or a recent Clang/MSVC with `<print>` in the standard library.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 01_MementoExample
./01_MementoExample
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

This example was rewritten with its own scenario, class names and output. It follows the usual structure of the memento pattern.
