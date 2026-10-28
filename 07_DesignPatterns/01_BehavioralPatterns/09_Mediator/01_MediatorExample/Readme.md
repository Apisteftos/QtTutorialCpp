# Mediator — `01_MediatorExample`

> **Intent:** Components do not talk to each other directly. They report to a mediator, which knows the rules and updates the others.

## Scenario

A sign-up form has a text field, a check box and a submit button. The button is enabled only when the user name is not empty and the terms are accepted. The widgets do not know about each other.

## Types in `main.cpp`

| Type | Role |
|---|---|
| `Mediator` | interface with `changed()` |
| `Widget` | base class; reports changes to the mediator |
| `TextField` / `CheckBox` / `SubmitButton` | concrete components |
| `SignupForm` | concrete mediator; owns the widgets and contains the rule |

## The pattern in short

- The rule "enable submit" lives in one place.
- Widgets could be reused in another form with a different mediator.

**Use it when:** Many objects depend on each other in a tangled way.

**Watch out:** The mediator itself can become a large class that knows too much.

## Output

```text
submit: click ignored (disabled)
user name: typed 'alex'
submit: disabled
terms: checked
submit: enabled
submit: form sent
user name: typed ''
submit: disabled
submit: click ignored (disabled)
```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`): GCC 14+ or a recent Clang/MSVC with `<print>` in the standard library.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 01_MediatorExample
./01_MediatorExample
```

The included `CMakeLists.txt` also asks CMake to find Qt Core; `main.cpp` does not use Qt, so the plain `g++` command is enough.

## What was tested

Checked with GCC 13.3 and a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra` without warnings.
- Runs to completion (exit code 0) and prints 9 lines.
- Clean under AddressSanitizer + UBSan (no errors, no leaks reported).
- Output identical between two runs.

## Not tested

- GCC 14 / the real `<print>`; MSVC; Apple Clang; Windows/macOS.
- The CMake build.

## Notes

This example was rewritten with its own scenario, class names and output. It follows the usual structure of the mediator pattern.
