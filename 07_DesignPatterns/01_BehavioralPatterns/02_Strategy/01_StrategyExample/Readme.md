# Strategy — `01_StrategyExample`

> **Intent:** Puts a family of algorithms behind one interface so the client can exchange them at run time.

## Scenario

A checkout calculates the amount to pay. The discount rule (none, percent, fixed amount) is a strategy object that can be replaced between two payments.

## Types in `main.cpp`

| Type | Role |
|---|---|
| `DiscountStrategy` | interface of all discount rules |
| `NoDiscount` / `PercentDiscount` / `FixedDiscount` | concrete strategies |
| `Checkout` | context; owns the current strategy and delegates to it |

## The pattern in short

- The context holds a pointer to the strategy interface and does not know the concrete class.
- Switching the rule is one call to `setStrategy()`.

**Use it when:** You have several ways to do the same job and want to choose one at run time without if/else chains.

**Watch out:** The client must know the differences between strategies to choose one; if the choice never changes at run time, a template parameter is enough.

## Output

```text
items: 100  rule: no discount  to pay: 100
items: 100  rule: 10% off  to pay: 90
items: 100  rule: 15 off  to pay: 85
items: 100  rule: 500 off  to pay: 0
```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`): GCC 14+ or a recent Clang/MSVC with `<print>` in the standard library.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 01_StrategyExample
./01_StrategyExample
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

This example was rewritten with its own scenario, class names and output. It follows the usual structure of the strategy pattern.
