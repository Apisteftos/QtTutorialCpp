# Observer — `01_ObserverExample`

> **Intent:** When one object changes, all objects registered with it are notified automatically, and the subject does not need to know their concrete types.

## Scenario

A thermometer reports a new temperature to every attached listener: two display panels and a frost alarm. A listener can be detached at any time.

## Types in `main.cpp`

| Type | Role |
|---|---|
| `TemperatureListener` | observer interface with `onTemperature()` |
| `Thermometer` | subject; keeps the listener list, `attach()`, `detach()`, `measure()` |
| `DisplayPanel` / `FrostAlarm` | concrete observers |

## The pattern in short

- The subject stores plain pointers; the listeners must outlive their registration (detach before they are destroyed).
- `std::erase(vector, value)` removes a listener in one call (C++20).

**Use it when:** A change in one object must trigger updates in an unknown or changing number of other objects.

**Watch out:** Observers that are destroyed without detaching leave dangling pointers; consider `std::weak_ptr` or a registration handle.

## Output

```text
thermometer: 12.5 C
  display [hall]: 12.5 C
  display [garden]: 12.5 C
thermometer: -1 C
  display [hall]: -1 C
  display [garden]: -1 C
  alarm: frost warning!

garden display detached
thermometer: 3 C
  display [hall]: 3 C
```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`): GCC 14+ or a recent Clang/MSVC with `<print>` in the standard library.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 01_ObserverExample
./01_ObserverExample
```

The included `CMakeLists.txt` also asks CMake to find Qt Core; `main.cpp` does not use Qt, so the plain `g++` command is enough.

## What was tested

Checked with GCC 13.3 and a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra` without warnings.
- Runs to completion (exit code 0) and prints 11 lines.
- Clean under AddressSanitizer + UBSan (no errors, no leaks reported).
- Output identical between two runs.

## Not tested

- GCC 14 / the real `<print>`; MSVC; Apple Clang; Windows/macOS.
- The CMake build.

## Notes

This example was rewritten with its own scenario, class names and output. It follows the usual structure of the observer pattern.
