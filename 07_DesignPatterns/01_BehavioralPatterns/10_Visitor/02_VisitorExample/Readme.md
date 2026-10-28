# Visitor — `02_VisitorExample`

> **Intent:** Adds new operations to a class hierarchy without changing the classes, by moving the operation into a separate visitor object.

## What this example contains

Sections in `main.cpp`:

- EXAMPLE 1: SHAPES AND OPERATIONS
- Forward declarations
- EXAMPLE 2: EMPLOYEE HIERARCHY
- Forward declarations
- EXAMPLE 3: DOCUMENT STRUCTURE
- Forward declarations
- EXAMPLE 4: SHOPPING CART
- Forward declarations
- DEMONSTRATION FUNCTIONS
- MAIN - DEMONSTRATING THE PATTERN
- Benefits Summary

Types defined in `main.cpp`:

| Type | Derives from |
|---|---|
| `ShapeVisitor` | — |
| `Shape` | — |
| `Circle` | `Shape` |
| `Rectangle` | `Shape` |
| `Triangle` | `Shape` |
| `AreaCalculator` | `ShapeVisitor` |
| `PerimeterCalculator` | `ShapeVisitor` |
| `XMLExporter` | `ShapeVisitor` |
| `EmployeeVisitor` | — |
| `Employee` | — |
| `Manager` | `Employee` |
| `Developer` | `Employee` |
| `Designer` | `Employee` |
| `SalaryCalculator` | `EmployeeVisitor` |
| `PerformanceEvaluator` | `EmployeeVisitor` |
| `DocumentVisitor` | — |
| `DocumentElement` | — |
| `Paragraph` | `DocumentElement` |
| `Heading` | `DocumentElement` |
| `Image` | `DocumentElement` |
| `HTMLExporter` | `DocumentVisitor` |
| `MarkdownExporter` | `DocumentVisitor` |
| `PlainTextExporter` | `DocumentVisitor` |
| `ProductVisitor` | — |
| `Product` | — |
| `Book` | `Product` |
| `Electronics` | `Product` |
| `Clothing` | `Product` |
| `TaxCalculator` | `ProductVisitor` |
| `DiscountCalculator` | `ProductVisitor` |

## The pattern in short

- An element interface with an accept(visitor) method.
- A visitor interface with one visit overload per concrete element.
- Concrete visitors implementing the operations.

**Use it when:** The set of element types is stable but you keep adding operations.

**Watch out:** Adding a new element type forces a change in every visitor. In C++17 and later std::variant with std::visit is often a lighter alternative.

## Sample output (first lines)

```text
========================================
VISITOR PATTERN DEMO
========================================

╔════════════════════════════════════╗
║  EXAMPLE 1: SHAPE OPERATIONS       ║
╚════════════════════════════════════╝

--- Calculating Areas ---
📐 Circle area: 78.53975 sq units
📐 Rectangle area: 24 sq units
📐 Triangle area: 6 sq units
Total Area: 108.53975 sq units

```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`), so you need GCC 14+ or a recent Clang/MSVC with a standard library that ships `<print>`.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 02_VisitorExample
./02_VisitorExample
```

The included `CMakeLists.txt` sets C++23 but also asks CMake to find Qt Core. `main.cpp` itself does not use Qt, so the plain `g++` command above is enough.

## What was tested

Checked with GCC 13.3 using a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra` without warnings.
- Runs to completion (exit code 0) and prints 441 lines.
- Clean under AddressSanitizer + UBSan (no errors, no leaks reported).
- Output is identical between two consecutive runs.
- The output of the `std::cout` → `std::println` conversion matched the original. Afterwards the lecture text that `main()` used to print was moved into *Notes moved from main.cpp* below. I compared the output before and after that move: the only difference is that the moved lines are no longer printed (values that change on every run, such as addresses, thread ids or timings, were ignored)..

## Not tested

- Behaviour on Windows/macOS or with MSVC/Apple Clang.
- The CMake build (Qt is not installed in the sandbox).

## Changes made to `main.cpp`

- `std::cout` replaced by `std::println` / `std::print`; no `"\n"` at line ends, `std::println("")` for blank lines.
- `using namespace std;` removed; standard names are written with `std::`.

## Notes moved from main.cpp

This text used to be printed by `main()`. It is lecture material, so it lives here and the program only prints the demo output.

### BENEFITS OF VISITOR PATTERN

- ✅ Open/Closed: Add operations without modifying elements
- ✅ Single Responsibility: Operations grouped in visitor
- ✅ Easy to add new operations: Just create new visitor
- ✅ Accumulation: Visitor can accumulate state during traversal
- ✅ Related operations: Groups related operations together
- ✅ Visiting across hierarchies: Works with different class types

### WHEN TO USE VISITOR

1. Need to perform operations across unrelated classes
2. Object structure is stable but operations change often
3. Want to avoid polluting classes with many operations
4. Need to accumulate state during traversal
5. Want to group related operations in one place
6. Classes have different interfaces but need common operations

### VISITOR vs STRATEGY

**Visitor Pattern:**

- Operations distributed across class hierarchy
- Double dispatch (runtime polymorphism twice)
- Used for operations on object structures
- Example: Export shapes to different formats

**Strategy Pattern:**

- Single algorithm encapsulated
- Single dispatch (runtime polymorphism once)
- Used for algorithm selection
- Example: Different payment methods

### REAL-WORLD EXAMPLES

- Compiler AST traversal (syntax analysis, optimization)
- Document export (HTML, PDF, Markdown)
- Shopping cart calculations (tax, discount, shipping)
- File system operations (size calc, search, backup)
- Graphics rendering (draw, bounds calc, hit test)
- Report generation from data structures
- Code analysis and refactoring tools
- XML/JSON processing and transformation
- Game entity systems (rendering, physics, AI)
- UI component traversal (layout, rendering)
- Database query optimization
- 3D scene graph operations

### KEY COMPONENTS

1. Visitor Interface: Declares visit methods for each element
2. Concrete Visitors: Implement operations on elements
3. Element Interface: Declares accept(visitor) method
4. Concrete Elements: Implement accept by calling visitor
5. Object Structure: Collection of elements to visit

### DOUBLE DISPATCH

**What is Double Dispatch:**

- Operation depends on both:
1. Type of element (Circle, Rectangle)
2. Type of visitor (AreaCalculator, XMLExporter)
- Achieved through two virtual calls

**How it works:**

```cpp
1. Client: shape->accept(visitor)
2. Element: visitor->visitCircle(this)
```

3. Visitor: Execute operation on Circle

**Why needed:**

- C++ has single dispatch (only receiver type matters)
- Visitor simulates double dispatch
- Both element and visitor types determine behavior

### STRUCTURE

┌─────────────────┐  
│     Client      │  
└────────┬────────┘  
│ uses  
▼  
┌─────────────────┐       ┌─────────────────┐  
│    Visitor      │       │    Element      │  
│  (Interface)    │       │  (Interface)    │  
│                 │       │                 │

```cpp
│ + visitA()      │◄──────│ + accept(v)     │
│ + visitB()      │       └─────────────────┘
```

└─────────────────┘              △  
△                       │  
│              ┌────────┴────────┐  
┌────────┴────────┐    │                 │  
│                 │    │                 │  
│  ConcreteV1     │    │  ConcreteA      │  
│                 │    │                 │

```cpp
│ + visitA()      │    │ + accept(v) {   │
│ + visitB()      │    │     v.visitA()  │
└─────────────────┘    │   }             │
```

└─────────────────┘

### ADVANTAGES

- Easy to add new operations (new visitor)
- Groups related operations in one class
- Can accumulate state during traversal
- Separates algorithm from object structure
- Follows Single Responsibility Principle
- Follows Open/Closed Principle (for operations)
- Clean separation of concerns

### DISADVANTAGES

- Hard to add new element types
- Breaks encapsulation (visitor needs element internals)
- Complex pattern to understand
- Requires updating all visitors when element changes
- Can lead to circular dependencies
- May violate Law of Demeter

### BEST PRACTICES

- Use when element hierarchy is stable
- Group related operations in same visitor
- Consider providing default implementations
- Document visitor responsibilities clearly
- Use const correctness appropriately
- Consider acyclic visitor for flexibility
- Keep visitor interface focused
- Test each visitor independently

### IMPLEMENTATION VARIATIONS

**Classic Visitor:**

- Visit method for each concrete element
- Type-safe but rigid
- Example: All examples above

**Reflective Visitor:**

- Single visit method using RTTI
- More flexible but less type-safe
- Uses dynamic_cast or typeid

**Acyclic Visitor:**

- Visitor and elements don't know each other
- Uses multiple inheritance
- More flexible, can add elements

**External Visitor:**

- Visitor doesn't need accept method
- Works with existing classes
- Less intrusive

### WHEN NOT TO USE VISITOR

- Element hierarchy changes frequently
- Only one or two operations needed
- Elements can't expose internal state
- Operations are tightly coupled to elements
- Simple switch/if statements would suffice
- Performance is critical (virtual call overhead)

### COMMON ISSUES & SOLUTIONS

**Circular Dependencies:**

- Issue: Visitor and elements include each other
- Solution: Forward declarations, separate headers

**Adding New Elements:**

- Issue: Must update all visitors
- Solution: Consider acyclic visitor, default handlers

**Breaking Encapsulation:**

- Issue: Visitor needs element internals
- Solution: Provide getter methods, friend classes

**Complex Hierarchies:**

- Issue: Too many visit methods
- Solution: Group similar elements, use composite

### VISITOR WITH OTHER PATTERNS

**Visitor + Composite:**

- Visit composite structures
- Traverse tree/graph structures
- Example: File system traversal

**Visitor + Iterator:**

- Iterate through collection
- Apply visitor to each element
- Clean separation of concerns

**Visitor + Interpreter:**

- Visitor for AST traversal
- Interpreter evaluates expressions
- Example: Compiler design

### RELATED PATTERNS

**Composite:**

- Often used together
- Visitor traverses composite structure
- Composite handles hierarchy, Visitor handles operations

**Iterator:**

- Both traverse structures
- Iterator focuses on access
- Visitor focuses on operations

**Strategy:**

- Both encapsulate algorithms
- Strategy for single object
- Visitor for object structure

### TESTING VISITORS

**Unit Tests:**

- Test each visitor independently
- Mock element objects
- Verify correct visit methods called

**Integration Tests:**

- Test visitor with real elements
- Verify traversal order
- Test accumulated state

**Edge Cases:**

- Empty structures
- Single element
- Deep nesting
- Circular references

### PERFORMANCE CONSIDERATIONS

**Virtual Call Overhead:**

- Two virtual calls per operation
- May impact hot paths
- Profile before optimizing

**Cache Locality:**

- Visitor may jump between classes
- Poor cache performance possible
- Consider data-oriented design

**Alternatives:**

- Function pointers for simple cases
- std::variant + std::visit (C++17)
- Virtual methods in elements (if stable operations)

### MODERN C++ ALTERNATIVES

```cpp
std::variant + std::visit:
```

- Type-safe union
- Compile-time visitor
- No virtual calls
- Example: std::variant<Circle, Rectangle>

**Lambdas:**

- Inline visitor operations
- Less boilerplate
- Good for one-off operations

**Concepts (C++20):**

- Constrain visitor types
- Better error messages
- Compile-time checking

### VISITOR IN REAL SYSTEMS

**Compilers:**

- AST traversal
- Type checking
- Code generation
- Optimization passes

**Graphics Systems:**

- Scene graph traversal
- Rendering
- Collision detection
- Bounding box calculation

**Document Processing:**

- Format conversion
- Validation
- Search and replace
- Statistics gathering

### SUMMARY (closing summary)

**Use Visitor when:**

- ✓ Need to add many unrelated operations
- ✓ Element hierarchy is stable
- ✓ Operations change more than structure
- ✓ Need to accumulate state across elements

**Avoid Visitor when:**

- ✗ Element hierarchy changes frequently
- ✗ Operations are tightly coupled to elements
- ✗ Simple solutions would work
- ✗ Performance is critical
