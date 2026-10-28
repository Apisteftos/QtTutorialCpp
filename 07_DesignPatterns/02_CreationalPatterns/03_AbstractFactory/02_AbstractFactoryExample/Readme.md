# Abstract Factory — `02_AbstractFactoryExample`

> **Intent:** Creates families of related objects without naming their concrete classes.

## What this example contains

Each distinct product of a product family should have a base interface. All variants of the product must implement this interface.

Types defined in `main.cpp`:

| Type | Derives from |
|---|---|
| `AbstractProductA` | — |
| `ConcreteProductA1` | `AbstractProductA` |
| `ConcreteProductA2` | `AbstractProductA` |
| `AbstractProductB` | — |
| `ConcreteProductB1` | `AbstractProductB` |
| `ConcreteProductB2` | `AbstractProductB` |
| `AbstractFactory` | — |
| `ConcreteFactory1` | `AbstractFactory` |
| `ConcreteFactory2` | `AbstractFactory` |

## The pattern in short

- An abstract factory with one creation method per product type.
- Concrete factories, one per family.
- Abstract product interfaces and their concrete variants.

**Use it when:** Products must be used together and you need to switch the whole family at once (for example a UI theme).

**Watch out:** Adding a new product type means changing the abstract factory and every concrete factory.

## Sample output (first lines)

```text
Client: Testing client code with the first factory type:
The result of the product B1.
The result of the B1 collaborating with ( The result of the product A1. )

Client: Testing the same client code with the second factory type:
The result of the product B2.
The result of the B2 collaborating with ( The result of the product A2. )
```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`), so you need GCC 14+ or a recent Clang/MSVC with a standard library that ships `<print>`.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 02_AbstractFactoryExample
./02_AbstractFactoryExample
```

The included `CMakeLists.txt` sets C++23 but also asks CMake to find Qt Core. `main.cpp` itself does not use Qt, so the plain `g++` command above is enough.

## What was tested

Checked with GCC 13.3 using a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra` without warnings.
- Runs to completion (exit code 0) and prints 7 lines.
- Clean under AddressSanitizer + UBSan (no errors, no leaks reported).
- Output is identical between two consecutive runs.
- Output of the converted program is byte-identical to the original `std::cout` version.

## Not tested

- Behaviour on Windows/macOS or with MSVC/Apple Clang.
- The CMake build (Qt is not installed in the sandbox).

## Changes made to `main.cpp`

- `std::cout` replaced by `std::println` / `std::print`; no `"\n"` at line ends, `std::println("")` for blank lines.
- `using namespace std;` removed; standard names are written with `std::`.

