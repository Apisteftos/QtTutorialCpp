# Memento — `02_MementoExample`

> **Intent:** Captures an object's state in an opaque snapshot so it can be restored later without breaking encapsulation.

## What this example contains

Sections in `main.cpp`:

- EXAMPLE 1: TEXT EDITOR WITH UNDO/REDO
- EXAMPLE 2: GAME CHARACTER CHECKPOINT
- EXAMPLE 3: DRAWING CANVAS WITH SNAPSHOTS
- EXAMPLE 4: TRANSACTION MANAGER
- DEMONSTRATION FUNCTIONS
- MAIN - DEMONSTRATING THE PATTERN
- Benefits Summary

Types defined in `main.cpp`:

| Type | Derives from |
|---|---|
| `TextMemento` | — |
| `TextEditor` | — |
| `History` | — |
| `GameMemento` | — |
| `GameCharacter` | — |
| `SaveManager` | — |
| `CanvasMemento` | — |
| `Canvas` | — |
| `SnapshotManager` | — |
| `AccountMemento` | — |
| `BankAccount` | — |

## The pattern in short

- An originator that creates and restores snapshots.
- A memento that stores the state.
- A caretaker that keeps the snapshots (for example an undo stack).

**Use it when:** Undo/rollback or checkpoints.

**Watch out:** Snapshots can be expensive if the state is large; storing deltas or limiting the history helps.

## Sample output (first lines)

```text
========================================
MEMENTO PATTERN DEMO
========================================

╔════════════════════════════════════╗
║  EXAMPLE 1: TEXT EDITOR            ║
╚════════════════════════════════════╝

--- Writing Text ---
💾 Saving state...
✍️  Wrote: "Hello "
   Current text: "Hello "
💾 Saving state...
✍️  Wrote: "World"
```

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`), so you need GCC 14+ or a recent Clang/MSVC with a standard library that ships `<print>`.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o 02_MementoExample
./02_MementoExample
```

The included `CMakeLists.txt` sets C++23 but also asks CMake to find Qt Core. `main.cpp` itself does not use Qt, so the plain `g++` command above is enough.

## What was tested

Checked with GCC 13.3 using a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14 library:

- Builds with `-std=c++23 -Wall -Wextra`: 6 warning(s) (-Wsign-compare). They come from the original code and were left as they were.
- Runs to completion (exit code 0) and prints 307 lines.
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

### BENEFITS OF MEMENTO PATTERN

- ✅ Preserves encapsulation: Internal state not exposed
- ✅ Simplifies originator: Doesn't track state history
- ✅ Undo/Redo functionality: Easy to implement
- ✅ Rollback support: Transaction-like behavior
- ✅ State snapshots: Save/restore at any point
- ✅ Time travel: Navigate through state history

### WHEN TO USE MEMENTO

1. Need to save and restore object state
2. Implement undo/redo functionality
3. Want to preserve encapsulation boundaries
4. Direct interface to state would expose implementation
5. Need transaction rollback capability
6. Create checkpoints or snapshots

### MEMENTO vs COMMAND

**Memento Pattern:**

- Stores complete object state
- Used for undo by restoring state
- Better for complex state
- Example: Save entire document state

**Command Pattern:**

- Stores operations/actions
- Used for undo by reversing operations
- Better for simple operations
- Example: Store 'delete 5 chars' command

### REAL-WORLD EXAMPLES

- Text editors (undo/redo)
- Photo editors (history states)
- Video games (save points/checkpoints)
- Database transactions (rollback)
- Version control systems (commits)
- Browser history (back/forward)
- Drawing applications (snapshots)
- Form wizards (step back/forward)
- Simulation software (state rewind)
- Configuration management (backup/restore)

### KEY COMPONENTS

1. Memento: Stores internal state of originator
2. Originator: Creates memento and restores from it
3. Caretaker: Manages mementos (history/save manager)
4. State: The data being saved/restored

### IMPLEMENTATION VARIATIONS

**Wide Interface:**

- Memento exposes all state to originator
- Used when originator needs full state access
- Example: friend class relationship

**Narrow Interface:**

- Memento hides state from caretaker
- Caretaker can't inspect or modify state
- Better encapsulation

**Incremental Memento:**

- Stores only changes, not full state
- Saves memory for large objects
- More complex to implement

### STRUCTURE

┌──────────────┐  
│  Caretaker   │  
│ (History)    │────┐  
└──────────────┘    │  
│ manages  
┌──────────────┐    ▼  
│  Originator  │  ┌──────────────┐  
│ (Editor)     │  │   Memento    │

```cpp
│ + save()     │──│ (Snapshot)   │
│ + restore()  │  └──────────────┘
```

└──────────────┘

### ADVANTAGES

- Doesn't violate encapsulation
- Simplifies originator code
- Easy to implement undo/redo
- Can save state at any point
- Supports multiple save points
- Caretaker doesn't depend on originator internals

### DISADVANTAGES

- Can be expensive if state is large
- Requires memory for each memento
- May need to copy large amounts of data
- Caretaker must track memento lifecycle
- Can create many mementos if saved frequently

### BEST PRACTICES

- Use smart pointers for automatic memory management
- Limit number of mementos to prevent memory issues
- Consider incremental saves for large objects
- Make mementos immutable
- Use friend classes to maintain encapsulation
- Compress mementos if they're large
- Implement memento expiration/cleanup

### OPTIMIZATION TECHNIQUES

**Delta Encoding:**

- Store only differences between states
- Reduces memory usage significantly
- Example: Store 'insert 5 chars' instead of full text

**Compression:**

- Compress memento data before storage
- Trade CPU time for memory savings
- Useful for large state data

**Copy-on-Write:**

- Share unchanged parts between mementos
- Only copy modified sections
- Reduces memory and improves performance

**Limited History:**

- Keep only N most recent mementos
- Automatically discard old states
- Prevents unbounded memory growth

### COMMON USE CASES (closing summary)

**Undo/Redo Systems:**

- Text/code editors
- Graphic design tools
- Maintain dual stacks (undo/redo)

**Game Saves:**

- Save player progress
- Multiple save slots
- Quick save/load functionality

**Transactions:**

- Database rollback
- Financial operations
- All-or-nothing execution

**Snapshots:**

- Virtual machine states
- Application configuration
- System restore points
