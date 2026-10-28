# Observer — `02_ObserverExample`

> **Intent:** Lets one object (the subject) notify any number of dependent objects (observers) when its state changes.

## What this example contains

Sections in `main.cpp`:

- EXAMPLE 1: WEATHER STATION
- EXAMPLE 2: STOCK MARKET
- EXAMPLE 3: NEWS AGENCY & SUBSCRIBERS
- EXAMPLE 4: EVENT SYSTEM / BUTTON CLICKS
- DEMONSTRATION FUNCTIONS
- MAIN - DEMONSTRATING THE PATTERN
- Benefits Summary

Types defined in `main.cpp`:

| Type | Derives from |
|---|---|
| `Subject` | — |
| `Observer` | — |
| `WeatherStation` | `Subject` |
| `CurrentConditionsDisplay` | `Observer` |
| `StatisticsDisplay` | `Observer` |
| `ForecastDisplay` | `Observer` |
| `Stock` | `Subject` |
| `Investor` | `Observer` |
| `StockDisplayBoard` | `Observer` |
| `NewsAgency` | `Subject` |
| `NewsSubscriber` | `Observer` |
| `MobileNewsApp` | `Observer` |
| `EventData` | — |
| `EventPublisher` | — |
| `Button` | — |

## The pattern in short

- A subject that keeps a list of observers and notifies them.
- An observer interface with an update/notify method.
- Concrete observers that react.

**Use it when:** Several objects must react to changes in another without the subject knowing their concrete types.

**Watch out:** Lifetime is the classic trap: a subject that stores raw pointers to observers dangles if an observer is destroyed without detaching. Notification order is usually unspecified, and an observer that modifies the subject list during a notification needs care.

## Sample output (first lines)

```text
========================================
OBSERVER PATTERN DEMO
========================================

╔════════════════════════════════════╗
║  EXAMPLE 1: WEATHER STATION        ║
╚════════════════════════════════════╝

--- Registering Observers ---
✅ Observer attached (Total: 1)
✅ Observer attached (Total: 2)
✅ Observer attached (Total: 3)

--- First Measurement ---
```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`), so you need GCC 14+ or a recent Clang/MSVC with a standard library that ships `<print>`.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 02_ObserverExample
./02_ObserverExample
```

The included `CMakeLists.txt` sets C++23 but also asks CMake to find Qt Core. `main.cpp` itself does not use Qt, so the plain `g++` command above is enough.

## What was tested

Checked with GCC 13.3 using a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra`: 6 warning(s) (-Wreorder). They come from the original code and were left as they were.
- Runs to completion (exit code 0) and prints 378 lines.
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

### BENEFITS OF OBSERVER PATTERN

- ✅ Loose Coupling: Subject and observers are loosely coupled
- ✅ Open/Closed: Add new observers without modifying subject
- ✅ Broadcast Communication: One-to-many dependency
- ✅ Dynamic Relationships: Observers can subscribe/unsubscribe
- ✅ Reusable Components: Subjects and observers can be reused
- ✅ Event-Driven: Enables reactive programming patterns

### WHEN TO USE OBSERVER

1. Change to one object requires changing others
2. Don't know how many objects need to be changed
3. Object should notify others without assumptions
4. Need event handling or publish-subscribe system
5. State changes need to trigger multiple updates
6. Want to decouple senders from receivers

### OBSERVER vs MEDIATOR

**Observer Pattern:**

- One-way communication (subject → observers)
- Subject doesn't know about observer details
- Observers just receive notifications
- Example: Newsletter notifies subscribers

**Mediator Pattern:**

- Two-way communication
- Mediator knows all participants
- Coordinates complex interactions
- Example: Chat room coordinates messages

### REAL-WORLD EXAMPLES

- GUI event handling (button clicks, mouse events)
- News/RSS feed subscriptions
- Stock market price monitoring
- Weather monitoring stations
- Social media notifications (likes, comments)
- Email distribution lists
- Model-View-Controller (MVC) architecture
- Real-time data dashboards
- Auction bidding systems
- IoT sensor networks
- Push notifications in mobile apps
- Chat applications

### KEY COMPONENTS

1. Subject: Maintains list of observers, notifies them
2. Observer: Defines update interface
3. Concrete Subject: Stores state, sends notifications
4. Concrete Observer: Implements update to stay consistent

### PUSH vs PULL MODEL

**Push Model:**

- Subject sends detailed data to observers
- update(subject, data)
- More efficient if all data is needed
- Less flexible - observers get all data

**Pull Model:**

- Subject sends minimal notification
- Observers query subject for needed data
- update(subject) then observer calls getters
- More flexible - observers get what they need
- Used in examples above

### STRUCTURE

┌──────────────┐  
│   Subject    │  
│  (Observable)│  
│              │

```cpp
│ + attach()   │
│ + detach()   │───────┐
│ + notify()   │       │ notifies
```

└──────────────┘       │  
△                ▼  
│         ┌──────────────┐  
│         │   Observer   │  
│         │              │

```cpp
┌──────────────┐ │ + update()   │
```

│   Concrete   │ └──────────────┘  
│   Subject    │        △  
│              │        │  
│ - state      │ ┌──────────────┐

```cpp
│ + getState() │ │   Concrete   │
│ + setState() │ │   Observer   │
```

└──────────────┘ └──────────────┘

### ADVANTAGES

- Supports broadcast communication
- Loose coupling between subject and observers
- Dynamic relationships (runtime subscription)
- Subject doesn't need to know observer details
- Observers can be added/removed independently
- Follows Open/Closed Principle
- Reusable subjects and observers

### DISADVANTAGES

- Unexpected updates (observers don't know about each other)
- Can cause memory leaks if not detached properly
- Order of notification is not guaranteed
- Performance impact with many observers
- Can lead to update cascades (chain reactions)
- Difficult to debug (indirect relationships)

### BEST PRACTICES

- Always detach observers in destructor
- Use weak pointers to prevent circular references
- Consider async notifications for heavy processing
- Document update order if it matters
- Limit number of observers for performance
- Handle exceptions in observer updates
- Consider filtering which observers to notify

### IMPLEMENTATION VARIATIONS

**Simple Subject-Observer:**

- Direct references between subject and observers
- Manual attach/detach
- Example: Weather station above

**Event System:**

- Centralized event publisher
- Type-based subscriptions
- Example: GUI events above

**Callback/Delegate Pattern:**

- Function pointers or lambdas
- More lightweight
- Example: Modern C++ with std::function

**Reactive Extensions (Rx):**

- Observable streams
- Functional operations (map, filter)
- Advanced reactive programming

### COMMON ISSUES & SOLUTIONS

**Memory Leaks:**

- Issue: Observers not detached before deletion
- Solution: Use smart pointers, RAII, auto-detach

**Update Storms:**

- Issue: Too many rapid notifications
- Solution: Batch updates, throttling, debouncing

**Circular Dependencies:**

- Issue: Observer modifies subject during update
- Solution: Use flags, defer updates, queue changes

**Performance:**

- Issue: Many observers slow down notifications
- Solution: Async notifications, selective notify

### RELATED PATTERNS

**Mediator:**

- Observer distributes communication
- Mediator centralizes communication

**MVC (Model-View-Controller):**

- Model is Subject
- Views are Observers
- Uses Observer for model-view sync

**Publish-Subscribe:**

- More decoupled than Observer
- Uses message broker/event bus
- Topic-based subscriptions

### NOTIFICATION STRATEGIES

**Immediate Notification:**

- Notify observers immediately after state change
- Simple and straightforward
- Can cause update storms

**Deferred Notification:**

- Collect changes and notify once
- More efficient for multiple changes
- Requires explicit commit/flush

**Asynchronous Notification:**

- Notify observers on separate thread
- Non-blocking for subject
- Requires thread synchronization

**Selective Notification:**

- Only notify relevant observers
- Filter by event type or criteria
- More efficient for targeted updates

### MODERN C++ IMPLEMENTATIONS (closing summary)

```cpp
Using std::function and lambdas:
```

- More flexible than interface-based
- No need for Observer base class
- Example: Event system above

**Using signals/slots (like Qt):**

- Type-safe connections
- Automatic disconnection
- Meta-object system

```cpp
Using std::weak_ptr:
```

- Prevents circular references
- Auto-cleanup of dead observers
- Safer memory management
