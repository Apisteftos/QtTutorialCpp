# Prototype — `02_PrototypeExample`

> **Intent:** Creates new objects by copying an existing object instead of constructing from scratch.

## What this example contains

Sections in `main.cpp`:

- PROTOTYPE INTERFACE
- EXAMPLE 1: GAME CHARACTERS
- SPECIFIC CHARACTER TYPES
- EXAMPLE 2: DOCUMENT SYSTEM
- PROTOTYPE REGISTRY (PROTOTYPE MANAGER)
- EXAMPLE 3: SHAPE CLONING (DEEP vs SHALLOW COPY)
- CLIENT CODE & DEMONSTRATIONS
- MAIN - DEMONSTRATING THE PATTERN
- Benefits Summary

Types defined in `main.cpp`:

| Type | Derives from |
|---|---|
| `Prototype` | — |
| `GameCharacter` | `Prototype` |
| `Warrior` | `GameCharacter` |
| `Mage` | `GameCharacter` |
| `Archer` | `GameCharacter` |
| `Document` | `Prototype` |
| `BusinessReport` | `Document` |
| `ResearchPaper` | `Document` |
| `PrototypeRegistry` | — |
| `Point` | — |
| `Shape` | `Prototype` |

## The pattern in short

- A prototype interface with a clone() method.
- Concrete prototypes that implement clone().

**Use it when:** Creating an object is expensive or you want to copy an object without knowing its concrete type.

**Watch out:** Decide between shallow and deep copy; a clone() that returns a raw pointer must be owned by someone (prefer std::unique_ptr).

## Sample output (first lines)

```text
========================================
PROTOTYPE PATTERN DEMO
========================================

╔════════════════════════════════════╗
║  GAME CHARACTER PROTOTYPES         ║
╚════════════════════════════════════╝

--- Registering Character Prototypes ---
⚙️  [EXPENSIVE] Creating new character from scratch: Warrior
   Loading character model...
   Initializing stats...
   Loading textures...
   Setting up animations...
```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`), so you need GCC 14+ or a recent Clang/MSVC with a standard library that ships `<print>`.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 02_PrototypeExample
./02_PrototypeExample
```

The included `CMakeLists.txt` sets C++23 but also asks CMake to find Qt Core. `main.cpp` itself does not use Qt, so the plain `g++` command above is enough.

### Text printed by the example functions

This text used to be printed between the demo steps. It is lecture material, so it lives here.

### From `file`

Loading character model...  
Initializing stats...  
Loading textures...  
Setting up animations...

### From `file`

Loading template...  
Applying formatting...  
Setting up sections...

## What was tested

Checked with GCC 13.3 using a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra`: 3 warning(s) (-Wreorder). They come from the original code and were left as they were.
- Runs to completion (exit code 0) and prints 229 lines.
- Clean under AddressSanitizer + UBSan (no errors, no leaks reported).
- Output is identical between two consecutive runs.
- The output of the `std::cout` → `std::println` conversion matched the original. Afterwards the lecture text that `main()` used to print was moved into *Notes moved from main.cpp* below. I compared the output before and after that move: the only difference is that the moved lines are no longer printed (values that change on every run, such as addresses, thread ids or timings, were ignored)..

## Not tested

- Behaviour on Windows/macOS or with MSVC/Apple Clang.
- The CMake build (Qt is not installed in the sandbox).

## Changes made to `main.cpp`

- `std::cout` replaced by `std::println` / `std::print`; no `"\n"` at line ends, `std::println("")` for blank lines.
- `using namespace std;` removed; standard names are written with `std::`.
- Types that only had a stream `operator<<` get a `std::formatter` (or the address is cast to `const void*`) because `std::println` cannot use `operator<<`.

## Notes moved from main.cpp

This text used to be printed by `main()`. It is lecture material, so it lives here and the program only prints the demo output.

### BENEFITS OF PROTOTYPE PATTERN

- ✅ Avoids expensive object creation
- ✅ Reduces subclassing (no need for Factory hierarchy)
- ✅ Add/remove products at runtime
- ✅ Clone complex objects easily
- ✅ Alternative to inheritance for configuration
- ✅ Hide complexity of object creation

### WHEN TO USE PROTOTYPE

1. Object creation is expensive (DB queries, network)
2. Want to avoid factory/builder hierarchy
3. Few state combinations (reuse configured objects)
4. Need to create objects at runtime
5. Want to reduce number of classes

### SHALLOW vs DEEP COPY

**Shallow Copy:**

- Copies object fields
- Pointers still point to same memory
- Changes affect both objects

**Deep Copy:**

- Copies object AND pointed-to objects
- Completely independent copies
- Changes don't affect original

### IMPLEMENTATION APPROACHES

1. Clone method in base class
2. Copy constructor
3. Prototype registry/manager
4. Prototype factory

### REAL-WORLD EXAMPLES (closing summary)

- Game character templates (spawn enemies)
- Document templates (Word, Photoshop)
- Database record caching
- UI widget templates
- Configuration presets
- Cell division in biology
- Object pooling systems
