# Decorator — `04_Decorator`

> **Intent:** Adds behaviour to an object dynamically by wrapping it in another object with the same interface.

## What this example contains

Sections in `main.cpp`:

- EXAMPLE 1: COFFEE SHOP (BEVERAGES)
- EXAMPLE 2: TEXT FORMATTING
- EXAMPLE 3: DATA STREAMS
- EXAMPLE 4: NOTIFICATION SYSTEM
- EXAMPLE 5: WINDOW WITH FEATURES
- CLIENT CODE & DEMONSTRATIONS
- MAIN - DEMONSTRATING THE PATTERN
- Benefits Summary

Types defined in `main.cpp`:

| Type | Derives from |
|---|---|
| `Beverage` | — |
| `Espresso` | `Beverage` |
| `HouseBlend` | `Beverage` |
| `DarkRoast` | `Beverage` |
| `Decaf` | `Beverage` |
| `CondimentDecorator` | `Beverage` |
| `Milk` | `CondimentDecorator` |
| `Mocha` | `CondimentDecorator` |
| `Soy` | `CondimentDecorator` |
| `Whip` | `CondimentDecorator` |
| `Text` | — |
| `PlainText` | `Text` |
| `TextDecorator` | `Text` |
| `BoldText` | `TextDecorator` |
| `ItalicText` | `TextDecorator` |
| `UnderlineText` | `TextDecorator` |
| `ColorText` | `TextDecorator` |
| `DataSource` | — |
| `FileDataSource` | `DataSource` |
| `DataSourceDecorator` | `DataSource` |
| `EncryptionDecorator` | `DataSourceDecorator` |
| `CompressionDecorator` | `DataSourceDecorator` |
| `LoggingDecorator` | `DataSourceDecorator` |
| `Notifier` | — |
| `BasicNotifier` | `Notifier` |
| `NotifierDecorator` | `Notifier` |
| `SMSDecorator` | `NotifierDecorator` |
| `SlackDecorator` | `NotifierDecorator` |
| `FacebookDecorator` | `NotifierDecorator` |
| `Window` | — |
| `SimpleWindow` | `Window` |
| `WindowDecorator` | `Window` |
| `ScrollbarDecorator` | `WindowDecorator` |
| `BorderDecorator` | `WindowDecorator` |

## The pattern in short

- A component interface.
- A concrete component.
- Decorators that hold a component and add behaviour before/after forwarding.

**Use it when:** You need optional, combinable extras without a subclass for every combination.

**Watch out:** Many small wrapper objects; identity checks and debugging are harder because the object you hold is the outermost wrapper.

## Sample output (first lines)

```text
========================================
DECORATOR PATTERN DEMO
========================================

╔════════════════════════════════════╗
║  EXAMPLE 1: COFFEE SHOP            ║
╚════════════════════════════════════╝

--- Order 1 ---
Espresso $1.99

--- Order 2 ---
Dark Roast Coffee, Mocha, Whipped Cream $1.29

```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`), so you need GCC 14+ or a recent Clang/MSVC with a standard library that ships `<print>`.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 04_Decorator
./04_Decorator
```

The included `CMakeLists.txt` sets C++23 but also asks CMake to find Qt Core. `main.cpp` itself does not use Qt, so the plain `g++` command above is enough.

### Text printed by the example functions

This text used to be printed between the demo steps. It is lecture material, so it lives here.

### From `file`

┌─────────────┐  
│             │  
│   Window    │  
│             │  
└─────────────┘

## What was tested

Checked with GCC 13.3 using a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra` without warnings.
- Runs to completion (exit code 0) and prints 230 lines.
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

### BENEFITS OF DECORATOR PATTERN

- ✅ Add responsibilities dynamically
- ✅ More flexible than static inheritance
- ✅ Avoids feature-laden classes
- ✅ Supports Open/Closed Principle
- ✅ Can combine multiple decorators
- ✅ Pay-per-use (only add what you need)
- ✅ Responsibilities can be added/removed at runtime

### WHEN TO USE DECORATOR

1. Add responsibilities dynamically and transparently
2. Responsibilities can be withdrawn
3. Extension by subclassing is impractical
4. Add features to individual objects, not classes
5. Avoid class explosion from combinations
6. Follow Open/Closed Principle

### STRUCTURE

Component (interface)  
├── ConcreteComponent (base object)  
└── Decorator (wraps Component)  
├── ConcreteDecoratorA  
└── ConcreteDecoratorB

Decorators wrap components recursively!

### DECORATOR vs OTHER PATTERNS

**Decorator:**

- Adds responsibilities (wrapper chain)
- Same interface
- Changes behavior

**Adapter:**

- Converts interface
- Different interface
- Enables compatibility

**Composite:**

- Tree structure
- Part-whole hierarchy
- Aggregates children

**Proxy:**

- Controls access
- Same interface
- Adds control layer

### WITHOUT DECORATOR (CLASS EXPLOSION)

Espresso  
EspressoWithMilk  
EspressoWithMocha  
EspressoWithMilkAndMocha  
EspressoWithMilkAndMochaAndWhip  
... exponential growth!

### WITH DECORATOR (CLEAN)

Component: Espresso, HouseBlend, DarkRoast  
Decorators: Milk, Mocha, Whip, Soy  
Total: 7 classes for unlimited combinations!

### REAL-WORLD EXAMPLES (closing summary)

- Java I/O streams (BufferedInputStream, etc.)
- GUI components (borders, scrollbars)
- Text formatting (bold, italic, color)
- Coffee shop beverages (condiments)
- Data encryption/compression
- Notification systems (multi-channel)
- HTTP middleware (logging, auth)
- Pizza toppings
