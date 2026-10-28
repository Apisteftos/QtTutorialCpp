# Mediator — `02_MediatorExample`

> **Intent:** Centralises the communication between objects so they do not refer to each other directly.

## What this example contains

Sections in `main.cpp`:

- EXAMPLE 1: CHAT ROOM MEDIATOR
- Forward declarations
- EXAMPLE 2: AIR TRAFFIC CONTROL
- Forward declarations
- EXAMPLE 3: SMART HOME MEDIATOR
- Forward declarations
- EXAMPLE 4: ONLINE AUCTION MEDIATOR
- DEMONSTRATION FUNCTIONS
- MAIN - DEMONSTRATING THE PATTERN
- Benefits Summary

Types defined in `main.cpp`:

| Type | Derives from |
|---|---|
| `ChatMediator` | — |
| `User` | — |
| `ChatRoom` | `ChatMediator` |
| `ATCMediator` | — |
| `Aircraft` | — |
| `ControlTower` | `ATCMediator` |
| `SmartHomeMediator` | — |
| `SmartDevice` | — |
| `MotionSensor` | `SmartDevice` |
| `Light` | `SmartDevice` |
| `Thermostat` | `SmartDevice` |
| `SecurityCamera` | `SmartDevice` |
| `SmartHomeHub` | `SmartHomeMediator` |
| `AuctionMediator` | — |
| `Bidder` | — |
| `AuctionHouse` | `AuctionMediator` |

## The pattern in short

- A mediator interface.
- A concrete mediator that knows the colleagues and coordinates them.
- Colleagues that talk only to the mediator.

**Use it when:** Many objects interact in complicated ways and the dependencies between them are hard to follow.

**Watch out:** The mediator can grow into a large class that knows everything.

## Sample output (first lines)

```text
========================================
MEDIATOR PATTERN DEMO
========================================

╔════════════════════════════════════╗
║  EXAMPLE 1: CHAT ROOM              ║
╚════════════════════════════════════╝

✅ Alice joined the chat room
✅ Bob joined the chat room
✅ Charlie joined the chat room

--- Chat Messages ---
💬 Alice sends: Hello everyone!
```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`), so you need GCC 14+ or a recent Clang/MSVC with a standard library that ships `<print>`.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 02_MediatorExample
./02_MediatorExample
```

The included `CMakeLists.txt` sets C++23 but also asks CMake to find Qt Core. `main.cpp` itself does not use Qt, so the plain `g++` command above is enough.

## What was tested

Checked with GCC 13.3 using a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra`: 9 warning(s) (-Wreorder, -Wunused-parameter). They come from the original code and were left as they were.
- Runs to completion (exit code 0) and prints 212 lines.
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

### BENEFITS OF MEDIATOR PATTERN

- ✅ Single Responsibility: Centralizes communication logic
- ✅ Open/Closed: Add new colleagues without changing others
- ✅ Reduces coupling: Objects don't reference each other directly
- ✅ Simplifies object protocols: One-to-many relationships
- ✅ Reusable components: Colleagues are more reusable
- ✅ Easier maintenance: Communication logic in one place

### WHEN TO USE MEDIATOR

1. Objects communicate in complex but well-defined ways
2. Reusing objects is difficult due to many dependencies
3. Behavior distributed among classes should be customizable
4. Too many relationships make system hard to understand
5. Need centralized control over object interactions

### MEDIATOR vs OBSERVER

**Mediator Pattern:**

- Two-way communication
- Colleagues can request actions from mediator
- Centralized control logic
- Example: Chat room coordinates user messages

**Observer Pattern:**

- One-way communication (subject → observers)
- Observers just receive notifications
- Distributed notification logic
- Example: Newsletter notifies subscribers

### REAL-WORLD EXAMPLES

- Chat rooms and messaging apps
- Air traffic control systems
- Smart home automation hubs
- Online auction platforms
- GUI dialog boxes (coordinate widgets)
- Game multiplayer servers
- Stock exchange trading systems
- Airport gate assignment systems
- Conference call coordinators
- Workflow management systems

### KEY COMPONENTS

1. Mediator Interface: Defines communication protocol
2. Concrete Mediator: Implements coordination logic
3. Colleague Classes: Objects that communicate via mediator
4. Reference: Each colleague has reference to mediator

### STRUCTURE

**Without Mediator (N² connections):**

Colleague1 ←→ Colleague2  
↕           ↕  
Colleague3 ←→ Colleague4

**With Mediator (N connections):**

Colleague1 ←→ Mediator ←→ Colleague3  
↕  
Colleague2 ←→ Mediator ←→ Colleague4

### ADVANTAGES

- Decouples colleagues from each other
- Reduces number of connections (N² → N)
- Centralizes control logic
- Simplifies object protocols
- Easier to understand and maintain
- Colleagues become more reusable
- Can introduce new mediators without changing colleagues

### DISADVANTAGES

- Mediator can become a 'god object' (too complex)
- May be harder to debug (centralized control)
- Can reduce performance (extra indirection)
- Mediator itself can be hard to maintain if too complex

### BEST PRACTICES

- Keep mediator focused on coordination, not business logic
- Use events/messages instead of direct method calls
- Consider breaking complex mediators into smaller ones
- Colleagues should be unaware of each other
- Document the communication protocol clearly

### COMMON IMPLEMENTATIONS (closing summary)

**Event-Based Mediator:**

- Colleagues send events to mediator
- Mediator dispatches to appropriate handlers
- Most flexible approach

**Method-Based Mediator:**

- Mediator has explicit methods for each interaction
- More type-safe
- Example: ATC with requestLanding(), requestTakeoff()

**Message Queue Mediator:**

- Asynchronous communication
- Useful for distributed systems
- Example: Message brokers (RabbitMQ, Kafka)
