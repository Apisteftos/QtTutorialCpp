# Factory Method — `02_FactoryMethodExample`

> **Intent:** Defines an interface for creating an object but lets subclasses (or a function) decide which concrete class to instantiate.

## What this example contains

Sections in `main.cpp`:

- PRODUCT INTERFACE
- CONCRETE PRODUCTS
- CREATOR (Abstract Factory)
- CONCRETE CREATORS
- CLIENT CODE
- FACTORY SELECTOR
- REAL-WORLD EXAMPLE 2: Document Creator
- Product interface
- MAIN - DEMONSTRATING THE PATTERN
- Example 1: Road Logistics
- Example 2: Sea Logistics
- Example 3: Air Logistics
- Example 4: Rail Logistics
- Example 5: Drone Logistics
- Example 6: Dynamic Selection
- Example 7: Document Application
- Benefits Summary

Types defined in `main.cpp`:

| Type | Derives from |
|---|---|
| `Transport` | — |
| `Truck` | `Transport` |
| `Ship` | `Transport` |
| `Plane` | `Transport` |
| `Train` | `Transport` |
| `Drone` | `Transport` |
| `Logistics` | — |
| `RoadLogistics` | `Logistics` |
| `SeaLogistics` | `Logistics` |
| `AirLogistics` | `Logistics` |
| `RailLogistics` | `Logistics` |
| `DroneLogistics` | `Logistics` |
| `Document` | — |
| `PDFDocument` | `Document` |
| `WordDocument` | `Document` |
| `Application` | — |
| `PDFApplication` | `Application` |
| `WordApplication` | `Application` |
| `DeliveryScenario` | — |

## The pattern in short

- A product interface.
- Concrete products.
- A creator with a factory method that returns a product.

**Use it when:** Client code should depend on the product interface, not on concrete classes.

**Watch out:** Returning std::unique_ptr makes ownership explicit.

## Sample output (first lines)

```text
========================================
FACTORY METHOD PATTERN DEMO
========================================

=== EXAMPLE 1: Road Logistics ===

╔════════════════════════════════════╗
║      PLANNING DELIVERY             ║
╚════════════════════════════════════╝
🏭 RoadLogistics: Creating Truck transport...
Transport Type: Truck
Capacity: 20000 kg
Cargo Weight: 15000 kg
✅ Cargo fits in Truck
```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`), so you need GCC 14+ or a recent Clang/MSVC with a standard library that ships `<print>`.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 02_FactoryMethodExample
./02_FactoryMethodExample
```

The included `CMakeLists.txt` sets C++23 but also asks CMake to find Qt Core. `main.cpp` itself does not use Qt, so the plain `g++` command above is enough.

### Text printed by the example functions

This text used to be printed between the demo steps. It is lecture material, so it lives here.

### From `file`

🚚 Delivering by land in a truck  
Route: Using highways and roads  
Speed: 80 km/h average

### From `file`

🚢 Delivering by sea in a container ship  
Route: Using ocean shipping lanes  
Speed: 40 km/h average

### From `file`

✈️  Delivering by air in a cargo plane  
Route: Direct flight path  
Speed: 800 km/h average

### From `file`

🚂 Delivering by rail in a freight train  
Route: Using railway network  
Speed: 100 km/h average

### From `file`

🚁 Delivering by air with autonomous drone  
Route: Direct GPS coordinates  
Speed: 60 km/h average

## What was tested

Checked with GCC 13.3 using a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra` without warnings.
- Runs to completion (exit code 0) and prints 285 lines.
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

### BENEFITS OF FACTORY METHOD

- ✅ Avoids tight coupling between creator and concrete products
- ✅ Single Responsibility: Product creation code in one place
- ✅ Open/Closed: Can introduce new products without breaking code
- ✅ Code works with products through common interface
- ✅ Easy to extend with new product types

### WHEN TO USE FACTORY METHOD

1. Don't know exact types beforehand
2. Want to provide library extension points
3. Want to save resources by reusing objects
4. Localize knowledge of which class to instantiate

### FACTORY METHOD vs ABSTRACT FACTORY

**Factory Method:**

- Creates ONE product
- Uses inheritance (subclass decides which to create)
- Returns different types of a SINGLE product

**Abstract Factory:**

- Creates FAMILIES of related products
- Uses composition (factory object has methods)
- Returns MULTIPLE related products

### REAL-WORLD EXAMPLES (closing summary)

- Logistics/shipping systems (Truck, Ship, Plane)
- Document editors (PDF, Word, Excel)
- Database connections (MySQL, PostgreSQL, MongoDB)
- Payment processors (PayPal, Stripe, Square)
- UI element frameworks (Button, Dialog, Menu)
- Game character creation (Warrior, Mage, Archer)
- Report generators (HTML, PDF, Excel)
