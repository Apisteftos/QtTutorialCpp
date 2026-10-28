# 03 · Interfaces — Program to Abstractions

> Never depend on a concrete class when you can depend on a contract.  
> An interface defines **what** something does.  
> The implementation defines **how** it does it.  
> Callers only need to know *what*.

---

## Interface as Contract

```mermaid
flowchart TD
    subgraph CONTRACT["Interface = Contract"]
        I["«interface»\nICompressor\n─────────────\n+encode(samples) bytes\n+decode(bytes) samples\n+keptPercent() int\n+name() string"]
    end

    subgraph IMPL["Implementations — swap freely"]
        A[HalfCompressor]
        B[QuarterCompressor]
        D[MockCompressor\nfor testing]
    end

    subgraph USER["Users — know only the interface"]
        E[DataLink]
        F["BackupWriter\n(hypothetical)"]
        G["QualityMonitor\n(hypothetical)"]
    end

    I --> A & B & D
    E & F & G -->|"ICompressor&"| I

    style I fill:#FF9800,color:#fff
    style CONTRACT fill:#FFF3E0
    style IMPL fill:#E8F5E9
    style USER fill:#E3F2FD
```

---

## Interface vs Abstract Class in C++

```mermaid
flowchart LR
    subgraph INTERFACE["Pure Interface\n(preferred)"]
        PI["class IShape\n──────────\n+area() = 0\n+perimeter() = 0\n+draw() = 0\n~IShape() = default"]
    end

    subgraph ABSTRACT["Abstract Class\n(adds shared behaviour)"]
        AC["class Shape\n──────────\n+area() = 0\n+colour: string\n+describe()\n~Shape() = default"]
    end

    subgraph CONCRETE["Concrete"]
        C1[Circle]
        C2[Rectangle]
        C3[Triangle]
    end

    C1 & C2 & C3 --> PI
    C1 & C2 & C3 --> AC

    style INTERFACE fill:#FF9800,color:#fff
    style ABSTRACT fill:#2196F3,color:#fff
```

| | Pure Interface | Abstract Class |
|---|---|---|
| Has data members | ❌ | ✅ |
| Has implemented methods | ❌ | ✅ |
| Multiple inheritance | ✅ safe | ⚠️ diamond risk |
| Use when | Contract only | Shared behaviour + contract |

---

## Interface Segregation (ISP)

Don't force classes to implement methods they don't need:

```mermaid
flowchart TD
    subgraph FAT["❌ Fat Interface"]
        FI["IWorker\n+work()\n+eat()\n+sleep()"]
        R1[Robot] -->|"must implement eat()\nbut robots don't eat!"| FI
    end

    subgraph SPLIT["✅ Split Interfaces"]
        WI["IWorkable\n+work()"]
        EI["IFeedable\n+eat()\n+sleep()"]
        H[Human] --> WI & EI
        RB[Robot] --> WI
    end

    style FAT fill:#FF5722,color:#fff
    style SPLIT fill:#4CAF50,color:#fff
```

---

## When to Define an Interface

| Question | If yes → interface |
|---|---|
| Will this be mocked in tests? | ✅ |
| Could implementation swap later? | ✅ |
| Do multiple classes do the same thing differently? | ✅ |
| Is this a stable, well-understood contract? | ✅ |
| Is there only ever one implementation and it never changes? | ❌ skip it |

## What was tested

- Compiled with GCC 13.3 (`-std=c++23`) using a stand-in `<print>` header, because `std::println` needs GCC 14 and it was not available here. The source files were compiled directly, not through CMake.
- Run and compared with the original `std::cout` output line by line. Differences: `bool` values print as `true`/`false` instead of `1`/`0`, floating-point numbers print in their shortest exact form instead of 6 significant digits, and values that change on every run (thread ids, timings, random data). I sampled the differing lines; I did not review every one.
- Run once with AddressSanitizer + UndefinedBehaviorSanitizer: no findings.

## Not tested

- The real `<print>` header (GCC 14+, recent Clang or MSVC) and platforms other than Linux x86-64.
