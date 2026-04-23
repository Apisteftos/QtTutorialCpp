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
        I["«interface»\nIAudioCodec\n─────────────\n+encode(pcm) bytes\n+decode(bytes) pcm\n+bitrate() int\n+name() string"]
    end

    subgraph IMPL["Implementations — swap freely"]
        A[OpusCodec]
        B[AMRCodec]
        C[EVRCCodec]
        D[MockCodec\nfor testing]
    end

    subgraph USER["Users — know only the interface"]
        E[AudioEngine]
        F[RecorderModule]
        G[QualityMonitor]
    end

    I --> A & B & C & D
    E & F & G -->|"IAudioCodec&"| I

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
