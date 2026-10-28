# 03 · Event-Driven Architecture

> Components don't call each other.  
> They emit events. Others listen.  
> The emitter doesn't know who's listening — and doesn't care.

---

## Structure

```mermaid
flowchart TD
    P1["📡 Publisher A\n(Panel Button)"]
    P2["📡 Publisher B\n(Network Monitor)"]
    EB["🚌 Event Bus\nRoutes events by type\nDecouples producers from consumers"]
    S1["🔔 Subscriber 1\nSound Module"]
    S2["🔔 Subscriber 2\nDisplay Module"]
    S3["🔔 Subscriber 3\nLogger"]

    P1 -->|"emit: BUTTON_PRESSED"| EB
    P2 -->|"emit: LINK_DOWN"| EB
    EB -->|"dispatch"| S1
    EB -->|"dispatch"| S2
    EB -->|"dispatch"| S3

    style EB fill:#FF9800,color:#fff
    style P1 fill:#2196F3,color:#fff
    style P2 fill:#2196F3,color:#fff
    style S1 fill:#4CAF50,color:#fff
    style S2 fill:#4CAF50,color:#fff
    style S3 fill:#4CAF50,color:#fff
```

---

## Event Lifecycle

```mermaid
sequenceDiagram
    participant Publisher
    participant EventBus
    participant SubscriberA
    participant SubscriberB

    Publisher->>EventBus: emit(BUTTON_PRESSED, payload)
    Note over EventBus: look up subscribers for BUTTON_PRESSED
    EventBus->>SubscriberA: dispatch(event)
    EventBus->>SubscriberB: dispatch(event)
    SubscriberA-->>EventBus: handled
    SubscriberB-->>EventBus: handled
    Note over Publisher: knows nothing about A or B
```

---

## C++ Class Map

```mermaid
classDiagram
    class Event {
        +type: EventType
        +payload: string
        +timestamp: int
    }
    class EventBus {
        +subscribe(type, handler)
        +emit(event)
        -handlers: map~EventType, vector~Handler~~
    }
    class SoundModule {
        +onButtonPressed(event)
        +onLinkDown(event)
    }
    class DisplayModule {
        +onButtonPressed(event)
        +onLinkDown(event)
    }
    class Logger {
        +onAnyEvent(event)
    }

    EventBus --> Event : routes
    SoundModule ..> EventBus : subscribes to
    DisplayModule ..> EventBus : subscribes to
    Logger ..> EventBus : subscribes to
```

---

## Event-Driven vs Direct Call

```mermaid
flowchart LR
    subgraph Direct["❌ Direct Call — tight coupling"]
        A1[Panel Button] -->|calls| B1[Sound Module]
        A1 -->|calls| C1[Display]
        A1 -->|calls| D1[Logger]
    end

    subgraph EventDriven["✅ Event-Driven — loose coupling"]
        A2[Panel Button] -->|emit| BUS[Event Bus]
        BUS --> B2[Sound Module]
        BUS --> C2[Display]
        BUS --> D2[Logger]
    end
```

---

## When to Use

✅ Device and UI control — button events, message arrivals, link status  
✅ Qt applications — signals/slots are event-driven  
✅ Any system where producers and consumers must be independent  
✅ Adding new subscribers without changing existing code (Open/Closed Principle)  
❌ When you need guaranteed synchronous call order  
❌ When debugging — event chains can be hard to trace

## What was tested

- Compiled with GCC 13.3 (`-std=c++23`) using a stand-in `<print>` header, because `std::println` needs GCC 14 and it was not available here. The source files were compiled directly, not through CMake.
- Run and compared with the original `std::cout` output line by line. Differences: `bool` values print as `true`/`false` instead of `1`/`0`, floating-point numbers print in their shortest exact form instead of 6 significant digits, and values that change on every run (thread ids, timings, random data). I sampled the differing lines; I did not review every one.
- Run once with AddressSanitizer + UndefinedBehaviorSanitizer: no findings.

## Not tested

- The real `<print>` header (GCC 14+, recent Clang or MSVC) and platforms other than Linux x86-64.
