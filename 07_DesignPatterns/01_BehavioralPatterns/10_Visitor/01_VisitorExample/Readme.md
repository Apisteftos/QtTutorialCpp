# Visitor — `01_VisitorExample`

> **Intent:** Adds new operations to a class hierarchy without changing the classes. Each element only has `accept()`; every operation is a visitor class (double dispatch).

## Scenario

A file-system tree has files and folders. One visitor adds up the sizes, another prints the tree with indentation.

## Types in `main.cpp`

| Type | Role |
|---|---|
| `Entry` | element base class with `accept()` |
| `FileEntry` / `FolderEntry` | concrete elements |
| `EntryVisitor` | visitor interface with one `visit()` per element type |
| `SizeVisitor` / `PrintVisitor` | concrete operations |

## The pattern in short

- `accept()` calls `visitor.visit(*this)`; the overload is chosen by the real element type.
- The folder visitor decides how to walk the children.

**Use it when:** The class hierarchy is stable, but you often need new operations on it.

**Watch out:** Adding a new element class means changing every visitor; if elements change often, Visitor is a poor fit (consider `std::variant` + `std::visit`).

## Output

```text
project/
  main.cpp (1200 bytes)
  docs/
    readme.md (800 bytes)
    notes.txt (150 bytes)
total size: 2150 bytes
```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`): GCC 14+ or a recent Clang/MSVC with `<print>` in the standard library.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 01_VisitorExample
./01_VisitorExample
```

The included `CMakeLists.txt` also asks CMake to find Qt Core; `main.cpp` does not use Qt, so the plain `g++` command is enough.

## What was tested

Checked with GCC 13.3 and a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra` without warnings.
- Runs to completion (exit code 0) and prints 6 lines.
- Clean under AddressSanitizer + UBSan (no errors, no leaks reported).
- Output identical between two runs.

## Not tested

- GCC 14 / the real `<print>`; MSVC; Apple Clang; Windows/macOS.
- The CMake build.

## Notes

This example was rewritten with its own scenario, class names and output. It follows the usual structure of the visitor pattern.
