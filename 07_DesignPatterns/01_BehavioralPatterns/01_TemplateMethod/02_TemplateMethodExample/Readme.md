# Template Method — `02_TemplateMethodExample`

> **Intent:** Defines the skeleton of an algorithm in a base class and lets subclasses override individual steps without changing the overall order.

## What this example contains

Sections in `main.cpp`:

- EXAMPLE 1: BEVERAGE PREPARATION
- EXAMPLE 2: DATA MINING
- EXAMPLE 3: GAME AI
- EXAMPLE 4: HOUSE BUILDING
- DEMONSTRATION FUNCTIONS
- MAIN - DEMONSTRATING THE PATTERN
- Benefits Summary

Types defined in `main.cpp`:

| Type | Derives from |
|---|---|
| `Beverage` | — |
| `Coffee` | `Beverage` |
| `Tea` | `Beverage` |
| `HotChocolate` | `Beverage` |
| `BlackCoffee` | `Coffee` |
| `DataMiner` | — |
| `PDFDataMiner` | `DataMiner` |
| `CSVDataMiner` | `DataMiner` |
| `WordDataMiner` | `DataMiner` |
| `GameAI` | — |
| `AggressiveAI` | `GameAI` |
| `DefensiveAI` | `GameAI` |
| `BalancedAI` | `GameAI` |
| `HouseBuilder` | — |
| `WoodenHouse` | `HouseBuilder` |
| `BrickHouse` | `HouseBuilder` |
| `LuxuryVilla` | `HouseBuilder` |

## The pattern in short

- A base class with a non-virtual method that calls the steps in a fixed order.
- Steps are virtual (must be overridden) or have defaults/hooks (may be overridden).
- Subclasses supply the step behaviour.

**Use it when:** Several algorithms share the same sequence of steps but differ in some of them.

**Watch out:** Inheritance-based: the order of steps is fixed at compile time and a subclass cannot easily change it. If the steps vary a lot, Strategy (composition) is usually more flexible.

## Sample output (first lines)

```text
========================================
TEMPLATE METHOD PATTERN DEMO
========================================

╔════════════════════════════════════╗
║  EXAMPLE 1: BEVERAGE PREPARATION   ║
╚════════════════════════════════════╝

--- Making Different Beverages ---

☕ Preparing Coffee...
================================
1. 💧 Boiling water...
2. ☕ Brewing coffee grounds...
```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`), so you need GCC 14+ or a recent Clang/MSVC with a standard library that ships `<print>`.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 02_TemplateMethodExample
./02_TemplateMethodExample
```

The included `CMakeLists.txt` sets C++23 but also asks CMake to find Qt Core. `main.cpp` itself does not use Qt, so the plain `g++` command above is enough.

### Text printed by the example functions

This text used to be printed between the demo steps. It is lecture material, so it lives here.

### From `file`

6. ⭐ Adding luxury features:
  - 🏊 Swimming pool
  - 🌴 Garden with fountain
  - 🚗 Underground garage

## What was tested

Checked with GCC 13.3 using a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra`: 12 warning(s) (-Wunused-parameter). They come from the original code and were left as they were.
- Runs to completion (exit code 0) and prints 483 lines.
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

### BENEFITS OF TEMPLATE METHOD PATTERN

- ✅ Code Reuse: Common algorithm in base class
- ✅ Control: Parent controls algorithm structure
- ✅ Flexibility: Subclasses customize specific steps
- ✅ Maintainability: Algorithm changes in one place
- ✅ Inversion of Control: Framework calls subclass methods
- ✅ Enforces Consistency: All subclasses follow same structure

### WHEN TO USE TEMPLATE METHOD

1. Multiple classes have similar algorithms
2. Want to control algorithm structure from parent
3. Common behavior should be in base class
4. Want to avoid code duplication
5. Need to let subclasses extend specific steps
6. Want to enforce algorithm sequence

### TEMPLATE METHOD vs STRATEGY

**Template Method:**

- Uses inheritance (class-level)
- Defines algorithm skeleton in base class
- Compile-time binding
- Changes part of algorithm
- Example: Beverage preparation steps

**Strategy Pattern:**

- Uses composition (object-level)
- Encapsulates entire algorithm
- Runtime binding
- Replaces entire algorithm
- Example: Different payment methods

### REAL-WORLD EXAMPLES

- Beverage/food preparation (coffee, tea, recipes)
- Data mining and processing pipelines
- Game AI behavior sequences
- House/building construction processes
- Document generation (reports, invoices)
- Test frameworks (setUp, test, tearDown)
- Web page rendering (header, content, footer)
- Database connection workflow
- File parsers (XML, JSON, CSV)
- Network protocols (handshake, data, close)
- Compilation process (parse, compile, link)
- Authentication flows (validate, authorize, log)

### KEY COMPONENTS

1. Abstract Class: Defines template method and steps
2. Template Method: Defines algorithm skeleton (final)
3. Abstract Methods: Must be implemented by subclasses
4. Concrete Methods: Common implementation for all
5. Hook Methods: Optional override points
6. Concrete Classes: Implement abstract steps

### METHOD TYPES IN TEMPLATE

**Template Method:**

- Defines algorithm structure
- Calls other methods in sequence
- Should not be overridden (final)
- Example: prepareBeverage(), buildHouse()

**Abstract Methods:**

- Must be implemented by subclasses
- Different for each subclass
- Pure virtual (= 0)
- Example: brew(), addCondiments()

**Concrete Methods:**

- Common implementation
- Same for all subclasses
- Not overridden
- Example: boilWater(), pourInCup()

**Hook Methods:**

- Optional override points
- Default implementation provided
- Subclasses can customize
- Example: customerWantsCondiments()

### STRUCTURE

┌─────────────────────────┐  
│   AbstractClass         │  
│   (Beverage)            │  
│                         │

```cpp
│ + templateMethod() {    │  ← Defines algorithm
│     step1();            │    structure
│     step2();            │
│     step3();            │
│   }                     │
```

│                         │

```cpp
│ # step1()               │  ← Concrete method
│ # step2() = 0           │  ← Abstract method
│ # step3()               │  ← Hook method
```

└─────────────────────────┘  
△  
│ inherits  
┌───────┴───────┐  
│               │  
┌───────────┐  ┌───────────┐  
│ ConcreteA │  │ ConcreteB │  
│ (Coffee)  │  │ (Tea)     │  
│           │  │           │

```cpp
│ # step2() │  │ # step2() │  ← Override
│ # step3() │  │           │    abstract
```

└───────────┘  └───────────┘    methods

### ADVANTAGES

- Reuses common code in base class
- Controls algorithm structure
- Easy to add new implementations
- Enforces consistent algorithm sequence
- Reduces code duplication
- Follows Don't Repeat Yourself (DRY)
- Follows Hollywood Principle (Don't call us, we'll call you)
- Clear separation of variant and invariant parts

### DISADVANTAGES

- Requires inheritance (less flexible than composition)
- Difficult to add new steps to algorithm
- Can lead to class proliferation
- Subclasses depend on base class implementation
- Harder to understand flow (jumping between classes)
- Violates Liskov Substitution if not careful

### BEST PRACTICES

- Make template method final/non-virtual
- Minimize number of abstract methods
- Use meaningful names for steps
- Document algorithm flow clearly
- Provide sensible defaults in hooks
- Keep template method simple
- Use protected access for step methods
- Consider final keyword in C++ (virtual final)

### HOOK METHODS

**What are Hooks:**

- Optional steps in algorithm
- Have default implementation
- Subclasses can override if needed
- Provide flexibility without forcing implementation

**When to Use Hooks:**

- Step is optional
- Default behavior exists
- Not all subclasses need customization
- Want to provide extension points

**Examples:**

- customerWantsCondiments() in Beverage
- hasAdditionalFeatures() in HouseBuilder
- canUseSpecialAbility() in GameAI

### HOLLYWOOD PRINCIPLE

"Don't call us, we'll call you"

**How it applies:**

- High-level component (template) controls flow
- Low-level components (subclasses) are called
- Prevents dependency rot
- Inversion of control

**Example:**

- prepareBeverage() calls brew()
- Subclass doesn't call parent methods
- Parent orchestrates entire process

### IMPLEMENTATION VARIATIONS

**Classic Template Method:**

- Abstract base class
- Pure virtual methods
- Protected inheritance
- Example: All examples above

**Non-Virtual Template Interface (NVI):**

- Public non-virtual template method
- Private virtual implementation methods
- Better encapsulation
- Separates interface from implementation

**CRTP (Curiously Recurring Template Pattern):**

- Compile-time polymorphism
- No virtual functions
- Better performance
- Example: template<typename T> class Base

### COMMON ISSUES & SOLUTIONS

**Too Many Abstract Methods:**

- Issue: Subclasses forced to implement many methods
- Solution: Use hooks with defaults, split template

**Rigid Algorithm:**

- Issue: Hard to vary algorithm structure
- Solution: Consider Strategy pattern instead

**Base Class Changes:**

- Issue: Changing base affects all subclasses
- Solution: Careful design, versioning, deprecation

**Testing Difficulties:**

- Issue: Hard to test individual steps
- Solution: Extract steps to separate methods

### RELATED PATTERNS

**Strategy:**

- Template Method uses inheritance
- Strategy uses composition
- Strategy more flexible at runtime

**Factory Method:**

- Special case of Template Method
- Focuses on object creation
- Uses same inheritance structure

**Builder:**

- Template Method defines construction algorithm
- Builder provides fluent interface
- Can be combined

### TESTING TEMPLATE METHODS

**Unit Testing:**

- Test each concrete class independently
- Verify each step is called
- Test hook method overrides

**Integration Testing:**

- Test complete algorithm flow
- Verify step sequence
- Test with real data

**Mocking:**

- Mock abstract methods
- Verify template method calls them
- Check call order and count

### DESIGN GUIDELINES

**Do:**

- Keep template method simple
- Use clear, descriptive names
- Document the algorithm flow
- Provide good default implementations
- Make template method non-virtual

**Don't:**

- Put business logic in template method
- Make too many steps abstract
- Change algorithm structure frequently
- Use when composition would work better
- Create deep inheritance hierarchies

### TEMPLATE METHOD IN FRAMEWORKS

**Testing Frameworks:**

- setUp() - prepare test environment
- test() - run actual test
- tearDown() - cleanup
- Example: JUnit, Google Test

**Web Frameworks:**

- initialize() - setup request
- processRequest() - handle logic
- renderResponse() - send output
- Example: Servlet lifecycle

**GUI Frameworks:**

- paintComponent() template
- Event handling sequences
- Widget lifecycle methods

### REFACTORING TO TEMPLATE METHOD

**Signs You Need Template Method:**

- Similar code across multiple classes
- Same algorithm with minor variations
- Copy-pasted code with small changes
- Parallel class hierarchies

**Refactoring Steps:**

1. Identify common algorithm
2. Create abstract base class
3. Extract template method
4. Identify variant steps → abstract methods
5. Identify invariant steps → concrete methods
6. Identify optional steps → hooks
7. Move subclasses to inherit from base

### PERFORMANCE CONSIDERATIONS (closing summary)

**Virtual Function Overhead:**

- Virtual calls have small performance cost
- Usually negligible
- Consider CRTP for hot paths

**Memory:**

- Each object has vtable pointer
- Minimal memory overhead
- Shared vtable across instances

**Optimizations:**

- Use final keyword for leaf classes
- Enable compiler optimizations
- Profile before optimizing
