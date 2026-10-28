# 04 · Dependency Inversion Principle (DIP)

> **High-level modules should not depend on low-level modules.**  
> Both should depend on abstractions.  
> Abstractions should not depend on details.  
> Details should depend on abstractions.  
> — Robert C. Martin

---

## The Inversion

```mermaid
flowchart TD
    subgraph BEFORE["❌ Before DIP — high depends on low"]
        direction TB
        BL["BusinessLayer\n(high-level)"] -->|"#include MySqlDB.h\ndepends directly"| DL["MySqlDatabase\n(low-level)"]
    end

    subgraph AFTER["✅ After DIP — both depend on abstraction"]
        direction TB
        AL["BusinessLayer\n(high-level)"] -->|"depends on"| AB{{"«interface»\nIDatabase"}}
        DB["MySqlDatabase\n(low-level)"] -->|"implements"| AB
    end

    style BEFORE fill:#FF5722,color:#fff
    style AFTER fill:#4CAF50,color:#fff
```

---

## Who Owns the Interface?

This is the key insight of DIP — the **high-level module owns the interface**.

```mermaid
flowchart LR
    subgraph HL["High-Level Package\n(Business Rules)"]
        BL[BusinessLayer]
        I{{"IDatabase\n← defined HERE\nnot in the DB package"}}
        BL --> I
    end

    subgraph LL["Low-Level Package\n(Infrastructure)"]
        DB[MySqlDatabase]
        DB -->|"implements"| I
    end

    style HL fill:#2196F3,color:#fff
    style LL fill:#4CAF50,color:#fff
```

The arrow of dependency now points **from low-level toward high-level** — it is *inverted* from the naive approach.

---

## DIP vs Dependency Injection

These are related but different:

```mermaid
mindmap
  root((Dependency\nManagement))
    DIP
      Design principle
      Who owns the abstraction?
      Direction of dependency
      Compile-time concern
    Dependency Injection
      Implementation technique
      How to pass dependencies in
      Constructor / setter / method
      Runtime concern
    Together
      DIP tells you WHAT to depend on
      DI tells you HOW to supply it
```

---

## Without DIP — The Compile-Time Problem

```mermaid
flowchart LR
    A[ReportGenerator] -->|"#include"| B[PdfRenderer]
    A -->|"#include"| C[HtmlRenderer]
    A -->|"#include"| D[CsvRenderer]

    note["To add ExcelRenderer:\n→ Edit ReportGenerator\n→ Recompile everything\n→ Risk breaking existing renders"]

    style note fill:#FF5722,color:#fff
```

---

## With DIP — The Open/Closed Result

```mermaid
flowchart LR
    A[ReportGenerator] -->|"IRenderer&"| I{{"IRenderer"}}
    I --> B[PdfRenderer]
    I --> C[HtmlRenderer]
    I --> D[CsvRenderer]
    I --> E[ExcelRenderer\nnew addition]

    note2["To add ExcelRenderer:\n→ Create ExcelRenderer.cpp\n→ Zero changes to ReportGenerator\n→ Zero recompile risk"]

    style note2 fill:#4CAF50,color:#fff
    style E fill:#FF9800,color:#fff
```

---

## When to Apply DIP

✅ Any boundary between business logic and infrastructure  
✅ Anything that touches I/O: database, network, file system, hardware  
✅ Anything you want to mock in tests  
✅ Anything that might be swapped: cloud provider, compression library, storage backend  
❌ Internal helpers that are stable and never tested in isolation  
❌ Standard library types — `std::vector`, `std::string` are stable enough

## Notes moved from main.cpp

This text used to be printed by `main()`. It is lecture material, so it lives here and the program only prints the demo output.

### Closing summary

**Key difference:**

BEFORE — add CSV → edit ReportGenerator + enum + switch  
AFTER  — add Markdown → new file only, zero edits to generator

All assertions passed.

## What was tested

- Compiled with GCC 13.3 (`-std=c++23`) using a stand-in `<print>` header, because `std::println` needs GCC 14 and it was not available here. The source files were compiled directly, not through CMake.
- Run, and the output matches the original `std::cout` version except that the lecture text moved into *Notes moved from main.cpp* is no longer printed (and the remaining example titles were renumbered where an example was removed).
- Run once with AddressSanitizer + UndefinedBehaviorSanitizer: no findings.

## Not tested

- The real `<print>` header (GCC 14+, recent Clang or MSVC) and platforms other than Linux x86-64.
