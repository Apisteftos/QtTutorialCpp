# Flyweight — `02_FlyweightExample`

> **Intent:** Saves memory by sharing the common (intrinsic) part of many similar objects instead of storing it in each one.

## What this example contains

Types defined in `main.cpp`:

| Type | Derives from |
|---|---|
| `CharacterStyle` | — |
| `StyleFactory` | — |
| `Character` | — |
| `Document` | — |

## The pattern in short

- A flyweight holding the shared state.
- A factory/pool that returns the existing flyweight for a given key.
- Context objects holding the unique (extrinsic) state.

**Use it when:** A huge number of similar objects would otherwise duplicate the same data.

**Watch out:** Shared state must be immutable; the factory lookup costs time, so measure before using it.

## Sample output (first lines)

```text
=== Flyweight Pattern Demo - Text Editor ===
Creating a document with formatted text...

Adding title 'HELLO':
Creating new style: Arial-24-Blue-Bold
  -> Creating new style: Arial-24-Blue-Bold
Creating new style: Arial-24-Blue-Bold
Creating new style: Arial-24-Blue-Bold
  -> Reusing existing style: Arial-24-Blue-Bold
Creating new style: Arial-24-Blue-Bold
  -> Reusing existing style: Arial-24-Blue-Bold
Creating new style: Arial-24-Blue-Bold
  -> Reusing existing style: Arial-24-Blue-Bold
Creating new style: Arial-24-Blue-Bold
```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`), so you need GCC 14+ or a recent Clang/MSVC with a standard library that ships `<print>`.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 02_FlyweightExample
./02_FlyweightExample
```

The included `CMakeLists.txt` sets C++23 but also asks CMake to find Qt Core. `main.cpp` itself does not use Qt, so the plain `g++` command above is enough.

## What was tested

Checked with GCC 13.3 using a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra` without warnings.
- Runs to completion (exit code 0) and prints 118 lines.
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

### Closing summary

=== Key Observations ===

- Characters 'H', 'E', 'L', 'L', 'O' all share the same style object
- Each character only stores its unique position (x, y)
- Style objects are created once and reused
- Significant memory savings with many characters
