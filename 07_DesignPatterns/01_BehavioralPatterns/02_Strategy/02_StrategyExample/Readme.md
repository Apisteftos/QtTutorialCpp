# Strategy — `02_StrategyExample`

> **Intent:** Puts interchangeable algorithms behind a common interface so the caller can choose or swap one at run time.

## What this example contains

Sections in `main.cpp`:

- EXAMPLE 1: PAYMENT PROCESSING
- EXAMPLE 2: SORTING STRATEGIES
- EXAMPLE 3: COMPRESSION STRATEGIES
- EXAMPLE 4: NAVIGATION STRATEGIES
- DEMONSTRATION FUNCTIONS
- MAIN - DEMONSTRATING THE PATTERN
- Benefits Summary

Types defined in `main.cpp`:

| Type | Derives from |
|---|---|
| `PaymentStrategy` | — |
| `CreditCardPayment` | `PaymentStrategy` |
| `PayPalPayment` | `PaymentStrategy` |
| `CryptoPayment` | `PaymentStrategy` |
| `ShoppingCart` | — |
| `SortStrategy` | — |
| `BubbleSort` | `SortStrategy` |
| `QuickSort` | `SortStrategy` |
| `MergeSort` | `SortStrategy` |
| `DataSorter` | — |
| `CompressionStrategy` | — |
| `ZipCompression` | `CompressionStrategy` |
| `RarCompression` | `CompressionStrategy` |
| `SevenZipCompression` | `CompressionStrategy` |
| `FileCompressor` | — |
| `NavigationStrategy` | — |
| `CarNavigation` | `NavigationStrategy` |
| `WalkingNavigation` | `NavigationStrategy` |
| `PublicTransportNavigation` | `NavigationStrategy` |
| `BicycleNavigation` | `NavigationStrategy` |
| `Navigator` | — |

## The pattern in short

- A strategy interface.
- Concrete strategies, one per algorithm.
- A context object that holds a strategy and delegates to it.

**Use it when:** You have several ways to do the same job and want to select between them without if/else chains.

**Watch out:** The caller must know the strategies to choose one. In C++ a std::function or a template parameter can replace the interface when the strategy is a single callable.

## Sample output (first lines)

```text
========================================
STRATEGY PATTERN DEMO
========================================

╔════════════════════════════════════╗
║  EXAMPLE 1: PAYMENT PROCESSING     ║
╚════════════════════════════════════╝

--- Adding Items to Cart ---
🛒 Added: Laptop - $999.99
🛒 Added: Mouse - $29.99
🛒 Added: Keyboard - $79.99

--- Paying with Credit Card ---
```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`), so you need GCC 14+ or a recent Clang/MSVC with a standard library that ships `<print>`.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 02_StrategyExample
./02_StrategyExample
```

The included `CMakeLists.txt` sets C++23 but also asks CMake to find Qt Core. `main.cpp` itself does not use Qt, so the plain `g++` command above is enough.

### Text printed by the example functions

This text used to be printed between the demo steps. It is lecture material, so it lives here.

### From `file`

  - Taking highways
  - Avoiding toll roads
  - ETA: 45 minutes
  - Distance: 30 miles

### From `file`

  - Using pedestrian paths
  - Avoiding busy streets
  - ETA: 2 hours
  - Distance: 5 miles

### From `file`

  - Bus #45 to Central Station
  - Transfer to Metro Line 2
  - ETA: 1 hour 15 minutes
  - Cost: $3.50

### From `file`

  - Using bike lanes
  - Avoiding steep hills
  - ETA: 1 hour
  - Distance: 8 miles

## What was tested

Checked with GCC 13.3 using a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra` without warnings.
- Runs to completion (exit code 0) and prints 402 lines.
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

### BENEFITS OF STRATEGY PATTERN

- ✅ Open/Closed: Add new strategies without modifying context
- ✅ Single Responsibility: Isolates algorithm implementation
- ✅ Runtime flexibility: Switch algorithms at runtime
- ✅ Eliminates conditionals: No large if/switch statements
- ✅ Testability: Easy to test strategies independently
- ✅ Reusability: Strategies can be reused across contexts

### WHEN TO USE STRATEGY

1. Need different variants of an algorithm
2. Want to switch algorithms at runtime
3. Have many related classes differing only in behavior
4. Need to isolate algorithm implementation details
5. Class has massive conditional statements
6. Want to provide multiple implementations of same behavior

### STRATEGY vs STATE

**Strategy Pattern:**

- Encapsulates interchangeable algorithms
- Client chooses strategy explicitly
- Strategies don't know about each other
- Example: Different payment methods

**State Pattern:**

- Encapsulates state-dependent behavior
- States can transition to other states
- Context behavior changes with state
- Example: TCP connection states

### REAL-WORLD EXAMPLES

- Payment gateways (credit card, PayPal, crypto)
- Sorting algorithms (bubble, quick, merge)
- Compression algorithms (ZIP, RAR, 7z)
- Navigation routes (car, walk, public transport)
- Validation strategies (email, phone, format)
- Pricing strategies (regular, discount, seasonal)
- Authentication methods (password, OAuth, biometric)
- Rendering engines (SVG, Canvas, WebGL)
- Search algorithms (linear, binary, hash-based)
- Notification delivery (email, SMS, push)
- Export formats (PDF, Excel, CSV)
- Encryption algorithms (AES, RSA, DES)

### KEY COMPONENTS

1. Strategy Interface: Defines common interface
2. Concrete Strategies: Implement specific algorithms
3. Context: Uses a strategy, can switch at runtime
4. Client: Chooses appropriate strategy

### STRUCTURE

┌─────────────────┐  
│     Client      │  
└────────┬────────┘  
│ configures  
▼  
┌─────────────────┐  
│    Context      │  
│  (ShoppingCart) │  
│                 │  
│ - strategy ─────┼─────┐ uses

```cpp
│ + setStrategy() │     │
│ + executeAlg()  │     │
```

└─────────────────┘     │  
▼  
┌─────────────┐  
│  Strategy   │  
│ (Interface) │  
│             │

```cpp
│ + execute() │
```

└─────────────┘  
△  
┌───────────────┼───────────────┐  
│               │               │  
┌─────────────┐ ┌─────────────┐ ┌─────────────┐  
│ StrategyA   │ │ StrategyB   │ │ StrategyC   │  
│ (CreditCard)│ │ (PayPal)    │ │ (Crypto)    │  
│             │ │             │ │             │

```cpp
│ + execute() │ │ + execute() │ │ + execute() │
```

└─────────────┘ └─────────────┘ └─────────────┘

### ADVANTAGES

- Algorithms are interchangeable
- Easy to add new strategies
- Eliminates conditional statements
- Runtime algorithm selection
- Isolates algorithm implementation
- Promotes composition over inheritance
- Follows Open/Closed Principle
- Each strategy can be tested independently

### DISADVANTAGES

- Increases number of objects/classes
- Clients must be aware of different strategies
- Communication overhead between context and strategy
- May be overkill for simple variations
- Context may pass data strategy doesn't need

### BEST PRACTICES

- Keep strategy interface focused and cohesive
- Use dependency injection for strategies
- Consider default strategy in context
- Document when each strategy should be used
- Make strategies stateless when possible
- Use factory pattern to create strategies
- Consider strategy combination/composition
- Profile performance of different strategies

### IMPLEMENTATION VARIATIONS

**Basic Strategy:**

- Context has reference to strategy
- Strategy set via setter method
- Example: All examples above

**Function Pointers/Lambdas:**

- Use std::function instead of interface
- More lightweight, less boilerplate
- Good for simple strategies

**Template-Based:**

- Strategy as template parameter
- Compile-time binding
- Better performance, no virtual calls

**Strategy with Parameters:**

- Pass context data to strategy
- Strategy accesses context methods
- More flexible data exchange

### STRATEGY SELECTION APPROACHES

**Client Selection:**

- Client explicitly chooses strategy
- Most common approach
- Example: User selects payment method

**Automatic Selection:**

- Context chooses based on conditions
- Rules-based selection
- Example: Choose sort based on data size

**Factory Pattern:**

- Factory creates appropriate strategy
- Centralized strategy creation
- Example: Strategy factory based on config

**Configuration-Based:**

- Strategy loaded from config file
- Runtime configuration
- Example: Plugin-based architectures

### COMMON ISSUES & SOLUTIONS

**Too Many Strategies:**

- Issue: Overwhelming number of strategy classes
- Solution: Group related strategies, use composition

**Strategy Coupling:**

- Issue: Strategies depend on each other
- Solution: Keep strategies independent

**Data Passing:**

- Issue: Context passes unnecessary data
- Solution: Use specific interfaces, callbacks

**Performance:**

- Issue: Virtual function overhead
- Solution: Use template-based strategies

### RELATED PATTERNS

**State:**

- Strategy focuses on algorithms
- State focuses on state-dependent behavior

**Template Method:**

- Strategy uses composition
- Template Method uses inheritance
- Strategy is more flexible

**Factory:**

- Often used to create strategies
- Centralizes strategy instantiation

**Decorator:**

- Decorator adds responsibilities
- Strategy replaces entire algorithm

### MODERN C++ IMPLEMENTATIONS

```cpp
Using std::function:
```

- No need for interface hierarchy
- Can use lambdas or function pointers
- Example: std::function<void(int)> strategy

**Using Templates:**

- Compile-time polymorphism
- Better performance
- Example: template<typename Strategy> class Context

**Using Variants:**

- std::variant for type-safe strategies
- No virtual calls
- Example: std::variant<StrategyA, StrategyB>

### TESTING STRATEGIES

**Unit Tests:**

- Test each strategy independently
- Mock context if needed
- Verify algorithm correctness

**Integration Tests:**

- Test context with different strategies
- Verify strategy switching
- Test with real data

**Performance Tests:**

- Benchmark different strategies
- Compare execution times
- Profile memory usage

### USE CASES BY DOMAIN (closing summary)

**E-commerce:**

- Payment methods
- Shipping calculators
- Discount strategies
- Tax calculation

**Data Processing:**

- Sorting algorithms
- Search strategies
- Validation rules
- Export formats

**Gaming:**

- AI behaviors
- Movement strategies
- Combat tactics
- Pathfinding algorithms

**UI/UX:**

- Layout strategies
- Rendering methods
- Animation timing
- Theme variations
