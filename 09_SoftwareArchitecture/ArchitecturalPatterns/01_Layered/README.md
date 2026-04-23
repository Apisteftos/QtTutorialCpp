# 01 · Layered Architecture

> Each layer has one responsibility.  
> A layer only communicates with the layer **directly below** it.  
> No layer skips layers.

---

## Structure

```mermaid
flowchart TD
    P["🖥️  Presentation Layer\nHandles user I/O\nknows nothing about data storage"]
    B["⚙️  Business Logic Layer\nApplies rules and decisions\nknows nothing about display"]
    D["🗄️  Data Layer\nReads and writes storage\nknows nothing about rules"]

    P -->|calls| B
    B -->|calls| D
    D -.->|returns data| B
    B -.->|returns result| P

    style P fill:#2196F3,color:#fff
    style B fill:#FF9800,color:#fff
    style D fill:#4CAF50,color:#fff
```

---

## The Rule

```mermaid
flowchart LR
    OK1[Presentation → Business] -->|✅ allowed| X1[ ]
    OK2[Business → Data] -->|✅ allowed| X2[ ]
    BAD1[Presentation → Data] -->|❌ layer skip| X3[ ]
    BAD2[Data → Business] -->|❌ upward call| X4[ ]

    style OK1 fill:#4CAF50,color:#fff
    style OK2 fill:#4CAF50,color:#fff
    style BAD1 fill:#F44336,color:#fff
    style BAD2 fill:#F44336,color:#fff
```

---

## C++ Class Map

```mermaid
classDiagram
    class PresentationLayer {
        +run()
        -business: BusinessLayer&
    }
    class BusinessLayer {
        +processOrder(item, qty) bool
        +getStock(item) int
        -data: DataLayer&
    }
    class DataLayer {
        +load(item) int
        +save(item, qty) void
        -db: map~string,int~
    }

    PresentationLayer --> BusinessLayer : uses
    BusinessLayer --> DataLayer : uses
```

---

## When to Use

✅ Enterprise applications, web backends, OS design  
✅ When you want to swap the database without touching the UI  
✅ When teams own separate layers  
❌ Not good for real-time systems where call overhead matters  
❌ Not good when layers would be artificially thin  

---

## Key Insight

The dependency arrows point **downward only**.  
If you ever find yourself calling upward (Data calling Business), you've broken the pattern.  
In C++ this manifests as: lower layers must **never** `#include` higher layer headers.
