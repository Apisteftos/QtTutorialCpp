# State — `02_StateExample`

> **Intent:** Lets an object change its behaviour when its internal state changes, by delegating to a state object (or by an explicit state machine).

## What this example contains

Sections in `main.cpp`:

- EXAMPLE 1: VENDING MACHINE
- EXAMPLE 2: TCP CONNECTION
- EXAMPLE 3: DOCUMENT WORKFLOW
- EXAMPLE 4: TRAFFIC LIGHT
- DEMONSTRATION FUNCTIONS
- MAIN - DEMONSTRATING THE PATTERN
- Benefits Summary

Types defined in `main.cpp`:

| Type | Derives from |
|---|---|
| `VendingMachineState` | — |
| `VendingMachine` | — |
| `NoCoinState` | `VendingMachineState` |
| `HasCoinState` | `VendingMachineState` |
| `DispensingState` | `VendingMachineState` |
| `SoldOutState` | `VendingMachineState` |
| `TCPState` | — |
| `TCPConnection` | — |
| `TCPClosed` | `TCPState` |
| `TCPListen` | `TCPState` |
| `TCPEstablished` | `TCPState` |
| `DocumentState` | — |
| `Document` | — |
| `DraftState` | `DocumentState` |
| `ModerationState` | `DocumentState` |
| `ApprovedState` | `DocumentState` |
| `PublishedState` | `DocumentState` |
| `LightState` | — |
| `TrafficLight` | — |
| `RedLight` | `LightState` |
| `YellowLight` | `LightState` |
| `GreenLight` | `LightState` |

## The pattern in short

- A context with a current state.
- A state interface or enum.
- Concrete states that implement the behaviour and decide the transitions.

**Use it when:** An object has several modes with different behaviour and many state-dependent if/switch statements.

**Watch out:** More classes than a switch. For small machines an enum plus switch is simpler; for large ones the transition table is easier to review than scattered code.

## Sample output (first lines)

```text
========================================
STATE PATTERN DEMO
========================================

╔════════════════════════════════════╗
║  EXAMPLE 1: VENDING MACHINE        ║
╚════════════════════════════════════╝

🏪 Vending Machine initialized with 3 products
🔄 Initial state: NoCoinState

--- Successful Purchase ---
💰 Coin inserted
🔄 State changed to: HasCoinState
```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`), so you need GCC 14+ or a recent Clang/MSVC with a standard library that ships `<print>`.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 02_StateExample
./02_StateExample
```

The included `CMakeLists.txt` sets C++23 but also asks CMake to find Qt Core. `main.cpp` itself does not use Qt, so the plain `g++` command above is enough.

## What was tested

Checked with GCC 13.3 using a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra`: 13 warning(s) (-Wunused-parameter). They come from the original code and were left as they were.
- Runs to completion (exit code 0) and prints 363 lines.
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

### BENEFITS OF STATE PATTERN

- ✅ Single Responsibility: Each state encapsulates behavior
- ✅ Open/Closed: Add new states without modifying existing ones
- ✅ Eliminates complex conditionals: No large if/switch statements
- ✅ Explicit state transitions: Clear state machine logic
- ✅ Context simplification: Context delegates to state objects
- ✅ Easier maintenance: State-specific code is localized

### WHEN TO USE STATE

1. Object behavior depends on its state
2. Operations have large conditional statements on state
3. State transitions are complex
4. Need to avoid code duplication across states
5. State-specific behavior changes frequently
6. Want explicit state machine implementation

### STATE vs STRATEGY

**State Pattern:**

- Encapsulates state-dependent behavior
- States can transition to other states
- Context changes behavior as state changes
- Example: TCP connection states

**Strategy Pattern:**

- Encapsulates interchangeable algorithms
- Strategies don't know about each other
- Client chooses strategy explicitly
- Example: Different sorting algorithms

### REAL-WORLD EXAMPLES

- Vending machines
- TCP/IP connection states
- Document workflow (draft, review, published)
- Traffic lights
- Order processing (pending, shipped, delivered)
- Game character states (idle, walking, jumping)
- Audio player (stopped, playing, paused)
- Login systems (logged out, logged in, locked)
- Elevator control systems
- Phone call states (idle, ringing, connected)
- ATM machine states
- Thread states (new, runnable, blocked, terminated)

### KEY COMPONENTS

1. Context: Maintains current state, delegates requests
2. State Interface: Defines interface for state-specific behavior
3. Concrete States: Implement behavior for each state
4. State Transitions: States can change context's state

### STATE TRANSITION APPROACHES

**States Control Transitions:**

- States know about other states
- States call context.setState()
- More flexible, states are independent
- Example: All examples above

**Context Controls Transitions:**

- Context decides next state
- States just execute behavior
- Better encapsulation of state machine logic
- Centralized transition rules

**Table-Driven Transitions:**

- Transition table/matrix
- Current state + event → next state
- Easy to visualize and modify
- Good for complex state machines

### STRUCTURE

┌─────────────────┐  
│    Context      │  
│  (VendingMach.) │  
│                 │  
│ - state ────────┼─────┐

```cpp
│ + request()     │     │ delegates
```

└─────────────────┘     │  
▼  
┌─────────────┐  
│    State    │  
│ (Interface) │  
│             │

```cpp
│ + handle()  │
```

└─────────────┘  
△  
┌───────────────┼───────────────┐  
│               │               │  
┌─────────────┐ ┌─────────────┐ ┌─────────────┐  
│  StateA     │ │  StateB     │ │  StateC     │  
│ (NoCoin)    │ │ (HasCoin)   │ │ (Dispensing)│  
│             │ │             │ │             │

```cpp
│ + handle()  │ │ + handle()  │ │ + handle()  │
```

└─────────────┘ └─────────────┘ └─────────────┘

### ADVANTAGES

- Eliminates complex conditional logic
- Localizes state-specific behavior
- Makes state transitions explicit
- Easy to add new states
- Improves code organization
- States are reusable
- Follows Single Responsibility Principle
- Easier to test individual states

### DISADVANTAGES

- Increases number of classes
- Can be overkill for simple state machines
- State transitions can be hard to follow
- May require many small classes
- States may depend on each other

### BEST PRACTICES

- Keep states independent when possible
- Document state transition diagram
- Use shared_ptr for state objects
- Consider state object pooling for performance
- Handle invalid transitions gracefully
- Log state changes for debugging
- Consider using state machines libraries
- Test all state transitions thoroughly

### IMPLEMENTATION VARIATIONS

**Singleton States:**

- One instance per state (flyweight)
- More memory efficient
- States must be stateless
- Example: Traffic light colors

**Stateful States:**

- New state instance per transition
- Can store state-specific data
- More flexible but uses more memory
- Example: Document with version history

**Hierarchical States:**

- Nested state machines
- Super-states and sub-states
- Useful for complex systems
- Example: Game with multiple levels

### COMMON ISSUES & SOLUTIONS

**State Explosion:**

- Issue: Too many states
- Solution: Hierarchical states, composite pattern

**Shared Behavior:**

- Issue: Code duplication across states
- Solution: Abstract base state with common methods

**State Dependencies:**

- Issue: States need to know about each other
- Solution: Use factory or registry pattern

**Context Bloat:**

- Issue: Context has too many methods
- Solution: Group related operations, use delegation

### STATE MACHINE TOOLS

**State Diagram:**

- Visual representation of states and transitions
- UML state diagrams
- Essential for documentation

**State Table:**

- Matrix of current state × event → next state
- Easy to verify completeness
- Good for code generation

**Libraries:**

- Boost.MSM (Meta State Machine)
- Boost.Statechart
- TinyFSM (lightweight)
- Custom implementations

### RELATED PATTERNS

**Strategy:**

- State is like Strategy with self-referencing
- States can change context's behavior

**Flyweight:**

- Can share state objects (singleton states)
- Reduces memory for stateless states

**Singleton:**

- Often used for state objects
- One instance per state type

**Command:**

- State transitions can be commands
- Enables undo/redo of state changes

### TESTING STATE MACHINES (closing summary)

**Unit Tests:**

- Test each state independently
- Verify state-specific behavior
- Mock context if needed

**Integration Tests:**

- Test state transitions
- Verify complete workflows
- Test invalid transition handling

**Coverage Goals:**

- All states visited
- All transitions tested
- All edge cases covered
- Invalid operations handled
