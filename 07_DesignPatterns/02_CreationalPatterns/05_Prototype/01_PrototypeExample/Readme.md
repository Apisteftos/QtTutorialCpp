# Prototype — `01_PrototypeExample`

> **Intent:** Creates new objects by copying existing ones. The client asks for a copy of a registered template and never needs to know its concrete class.

## Scenario

A sticker book contains templates (a text sticker and an image sticker). Each request returns a fresh copy that can be moved independently.

## Types in `main.cpp`

| Type | Role |
|---|---|
| `Sticker` | prototype interface with `clone()` |
| `TextSticker` / `ImageSticker` | concrete prototypes |
| `StickerBook` | registry of named templates |

## The pattern in short

- `clone()` returns `std::unique_ptr<Sticker>`, so the caller owns the copy.
- Position is part of the copy, so the templates themselves are never modified.

**Use it when:** Creating an object from scratch is expensive or complicated, or you only know the base type.

**Watch out:** Objects with pointers need a deep copy (copy constructor); here all members are values.

## Output

```text
text 'Hello!' at (10, 20)
text 'Hello!' at (200, 40)
image star.png x1.5 at (5, 5)
no template called 'unknown'
```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`): GCC 14+ or a recent Clang/MSVC with `<print>` in the standard library.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 01_PrototypeExample
./01_PrototypeExample
```

The included `CMakeLists.txt` also asks CMake to find Qt Core; `main.cpp` does not use Qt, so the plain `g++` command is enough.

## What was tested

Checked with GCC 13.3 and a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra` without warnings.
- Runs to completion (exit code 0) and prints 4 lines.
- Clean under AddressSanitizer + UBSan (no errors, no leaks reported).
- Output identical between two runs.

## Not tested

- GCC 14 / the real `<print>`; MSVC; Apple Clang; Windows/macOS.
- The CMake build.

## Notes

This example was rewritten with its own scenario, class names and output. It follows the usual structure of the prototype pattern.
