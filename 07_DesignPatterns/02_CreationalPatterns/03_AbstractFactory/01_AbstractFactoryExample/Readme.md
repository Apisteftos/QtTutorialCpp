# Abstract Factory — `01_AbstractFactoryExample`

> **Intent:** Creates families of related objects without naming their concrete classes.

## What this example contains

Sections in `main.cpp`:

- ABSTRACT PRODUCTS
- CONCRETE PRODUCTS - WINDOWS FAMILY
- CONCRETE PRODUCTS - MAC FAMILY
- CONCRETE PRODUCTS - LINUX FAMILY
- ABSTRACT FACTORY
- CONCRETE FACTORIES
- CLIENT CODE
- MAIN - DEMONSTRATING THE PATTERN
- Example 1: Windows Application
- Example 2: macOS Application
- Example 3: Linux Application
- Example 4: Dynamic Factory Selection
- Benefits Summary

Types defined in `main.cpp`:

| Type | Derives from |
|---|---|
| `AbstractButton` | — |
| `AbstractCheckbox` | — |
| `WindowsButton` | `AbstractButton` |
| `WindowsCheckbox` | `AbstractCheckbox` |
| `MacButton` | `AbstractButton` |
| `MacCheckbox` | `AbstractCheckbox` |
| `LinuxButton` | `AbstractButton` |
| `LinuxCheckbox` | `AbstractCheckbox` |
| `GUIFactory` | — |
| `WindowsFactory` | `GUIFactory` |
| `MacFactory` | `GUIFactory` |
| `LinuxFactory` | `GUIFactory` |
| `Application` | — |

## The pattern in short

- An abstract factory with one creation method per product type.
- Concrete factories, one per family.
- Abstract product interfaces and their concrete variants.

**Use it when:** Products must be used together and you need to switch the whole family at once (for example a UI theme).

**Watch out:** Adding a new product type means changing the abstract factory and every concrete factory.

## Sample output (first lines)

```text
========================================
ABSTRACT FACTORY PATTERN DEMO
========================================

=== EXAMPLE 1: Windows Application ===

--- Painting UI ---
🪟 Rendering button in Windows style (flat, blue)
🪟 Rendering checkbox in Windows style (square, blue checkmark)

--- Handling User Input ---
🪟 Windows button clicked with left mouse button
🪟 Windows checkbox toggled
🪟 Windows checkbox interacts with 🪟 Rendering button in Windows style (flat, blue)
```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`), so you need GCC 14+ or a recent Clang/MSVC with a standard library that ships `<print>`.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 01_AbstractFactoryExample
./01_AbstractFactoryExample
```

The included `CMakeLists.txt` sets C++23 but also asks CMake to find Qt Core. `main.cpp` itself does not use Qt, so the plain `g++` command above is enough.

## What was tested

Checked with GCC 13.3 using a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra` without warnings.
- Runs to completion (exit code 0) and prints 74 lines.
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

### BENEFITS OF ABSTRACT FACTORY

- ✅ Products from one family are guaranteed to work together
- ✅ Easy to add new product families (just add new factory)
- ✅ Client code doesn't depend on concrete classes
- ✅ Single Responsibility: Product creation code is separate
- ✅ Open/Closed: Can add new families without changing client

### WHEN TO USE ABSTRACT FACTORY

1. System needs multiple families of related products
2. You want to enforce that products from same family are used together
3. You want to hide product implementations from client
4. You need to switch between product families at runtime

### REAL-WORLD EXAMPLES (closing summary)

- Cross-platform UI frameworks (Qt, wxWidgets)
- Database abstraction layers (ADO.NET, JDBC)
- Game themes (Medieval, Sci-Fi, Fantasy)
- Document formats (PDF, Word, HTML)
- Operating system services (File I/O, Networking)
