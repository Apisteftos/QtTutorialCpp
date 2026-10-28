# 02 · Coupling

> Coupling measures how much one component **knows** about another.  
> The more it knows, the more it breaks when the other changes.  
> Low coupling = components that can evolve independently.

---

## Tight vs Loose Coupling

```mermaid
flowchart TD
    subgraph TIGHT["❌ Tight Coupling"]
        direction LR
        A1[OrderService] -->|"new MySqlDatabase()\ndirect construction"| B1[MySqlDatabase]
        A1 -->|"calls specific\nMySql methods"| B1
        B1 -.->|"change DB schema\n→ OrderService breaks"| A1
    end

    subgraph LOOSE["✅ Loose Coupling"]
        direction LR
        A2[OrderService] -->|"IDatabase& db\nonly interface"| B2{IDatabase}
        B2 --> C2[MySqlDatabase]
        B2 --> D2[PostgresDatabase]
        B2 --> E2[MockDatabase]
    end

    style TIGHT fill:#FF5722,color:#fff
    style LOOSE fill:#4CAF50,color:#fff
```

---

## Types of Coupling (worst → best)

```mermaid
flowchart TD
    C1["Content Coupling ❌\nA reads B's private fields directly"] -->
    C2["Common Coupling ❌\nA and B share global state"] -->
    C3["Control Coupling ⚠️\nA passes flags that control B's behavior"] -->
    C4["Stamp Coupling ⚠️\nA passes a whole struct when B needs one field"] -->
    C5["Data Coupling ✅\nA passes only the data B actually needs"] -->
    C6["Message Coupling ✅\nA sends a message, B decides what to do"]

    style C1 fill:#F44336,color:#fff
    style C2 fill:#FF5722,color:#fff
    style C3 fill:#FF9800,color:#fff
    style C4 fill:#FFC107,color:#000
    style C5 fill:#4CAF50,color:#fff
    style C6 fill:#2E7D32,color:#fff
```

---

## Coupling Metrics

```mermaid
mindmap
  root((Coupling\nIndicators))
    High Coupling signs
      Constructor calls new ConcreteClass
      Uses global variables
      Includes many headers
      Method takes bool flag
      Large parameter structs
    Low Coupling signs
      Constructor takes interface ref
      No global state
      Minimal includes
      Narrow parameters
      Message or event based
```

---

## The Ripple Effect

```mermaid
flowchart LR
    subgraph TIGHTLY["Tightly coupled system"]
        X1[A] --> X2[B]
        X2 --> X3[C]
        X3 --> X4[D]
        X1 --> X3
        X2 --> X4
    end

    subgraph LOOSELY["Loosely coupled system"]
        Y1[A] --> I1{I1}
        Y2[B] --> I1
        I1 --> Y3[C]
        I1 --> Y4[D]
    end

    CHANGE["Change in C"] -.->|"breaks A,B,D"| TIGHTLY
    CHANGE2["Change in C"] -.->|"no effect on A,B"| LOOSELY

    style CHANGE fill:#F44336,color:#fff
    style CHANGE2 fill:#4CAF50,color:#fff
```

---

## When Coupling Is Unavoidable

Some coupling is necessary and healthy:

| Type | Example | Verdict |
|---|---|---|
| Interface coupling | `ILogger& log` | ✅ Good — depend on abstraction |
| Data coupling | Pass `int id` not whole `User` struct | ✅ Good — minimal data |
| Concrete coupling | `std::vector`, `std::string` | ✅ OK — stable standard types |
| Framework coupling | Qt signals/slots | ⚠️ Acceptable — framework is stable |
| Concrete coupling | `MySqlDatabase db` | ❌ Bad — tight to one vendor |

## Notes moved from main.cpp

This text used to be printed by `main()`. It is lecture material, so it lives here and the program only prints the demo output.

### Closing summary

**Key difference:**

TIGHT — OrderService hardcodes MySqlDatabase  
Cannot test without a real DB  
Cannot swap DB without editing OrderService

LOOSE — OrderService knows only IDatabase + ILogger  
Swap MySql→Postgres: zero changes to OrderService  
Unit test with Mock: zero changes to OrderService

All assertions passed.

## What was tested

- Compiled with GCC 13.3 (`-std=c++23`) using a stand-in `<print>` header, because `std::println` needs GCC 14 and it was not available here. The source files were compiled directly, not through CMake.
- Run, and the output matches the original `std::cout` version except that the lecture text moved into *Notes moved from main.cpp* is no longer printed (and the remaining example titles were renumbered where an example was removed).
- Run once with AddressSanitizer + UndefinedBehaviorSanitizer: no findings.

## Not tested

- The real `<print>` header (GCC 14+, recent Clang or MSVC) and platforms other than Linux x86-64.
