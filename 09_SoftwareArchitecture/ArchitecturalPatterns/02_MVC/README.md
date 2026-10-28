# 02 · MVC — Model · View · Controller

> The data knows nothing about the screen.  
> The screen knows nothing about the rules.  
> The controller is the only one who knows both.

---

## Structure

```mermaid
flowchart TD
    User("👤 User")
    C["🎮 Controller\nInterprets user actions\nUpdates Model\nSelects View"]
    M["📦 Model\nHolds data + state\nApplies domain rules\nNotifies observers"]
    V["🖥️  View\nDisplays current state\nKnows nothing about rules"]

    User -->|"input (command)"| C
    C -->|"update"| M
    M -->|"notify / data"| V
    V -->|"renders to"| User
    C -.->|"reads state"| M

    style C fill:#FF9800,color:#fff
    style M fill:#4CAF50,color:#fff
    style V fill:#2196F3,color:#fff
```

---

## Responsibility Split

```mermaid
mindmap
  root((MVC))
    Model
      Owns the data
      Enforces business rules
      Notifies when data changes
      No UI code ever
    View
      Reads from Model
      Renders to output
      No business logic
      Swappable display format
    Controller
      Receives user input
      Calls Model methods
      Chooses which View to render
      Thin — no logic of its own
```

---

## C++ Class Map

```mermaid
classDiagram
    class TaskModel {
        -tasks: vector~Task~
        -observers: vector~Observer*~
        +add(title)
        +complete(id)
        +getTasks() const
        +attach(Observer*)
        +notify()
    }
    class Observer {
        <<interface>>
        +update()*
    }
    class ConsoleView {
        +update()
        -model: TaskModel&
    }
    class TaskController {
        +handleAdd(title)
        +handleComplete(id)
        -model: TaskModel&
        -view: ConsoleView&
    }

    Observer <|-- ConsoleView
    TaskModel o-- Observer : notifies
    TaskController --> TaskModel : mutates
    TaskController --> ConsoleView : triggers
    ConsoleView --> TaskModel : reads
```

---

## MVC vs Layered

| | Layered | MVC |
|---|---|---|
| Focus | Separating technical tiers | Separating UI concerns |
| Direction | Top-down strict | Triangular (M↔V↔C) |
| Use case | Backend systems | GUI / interactive apps |
| C++ example | API server | Qt application |

---

## When to Use

✅ Any application with a UI that changes independently of data  
✅ When you want to swap views (console → GUI → web) without touching model  
✅ Qt is fundamentally MVC — signals/slots are the notification mechanism  
❌ Overkill for simple scripts with no real UI  
❌ Can be over-engineered if the model is trivial

## Notes moved from main.cpp

### Text printed by the example functions

This text used to be printed between the demo steps. It is lecture material, so it lives here.

### From `file`

┌─────────────────────────────┐  
│         TASK LIST           │  
├─────────────────────────────┤

## What was tested

- Compiled with GCC 13.3 (`-std=c++23`) using a stand-in `<print>` header, because `std::println` needs GCC 14 and it was not available here. The source files were compiled directly, not through CMake.
- Run, and the output is identical to the original `std::cout` version.
- Run once with AddressSanitizer + UndefinedBehaviorSanitizer: no findings.

## Not tested

- The real `<print>` header (GCC 14+, recent Clang or MSVC) and platforms other than Linux x86-64.
