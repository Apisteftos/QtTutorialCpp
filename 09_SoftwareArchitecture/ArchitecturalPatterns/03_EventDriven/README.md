# 03 · Event-Driven Architecture

> Components don't call each other.  
> They emit events. Others listen.  
> The emitter doesn't know who's listening — and doesn't care.

---

## Structure

```mermaid
flowchart TD
    P1["📡 Publisher A\n(PTT Button)"]
    P2["📡 Publisher B\n(Network Monitor)"]
    EB["🚌 Event Bus\nRoutes events by type\nDecouples producers from consumers"]
    S1["🔔 Subscriber 1\nAudio Engine"]
    S2["🔔 Subscriber 2\nDisplay Module"]
    S3["🔔 Subscriber 3\nLogger"]

    P1 -->|"emit: PTT_PRESSED"| EB
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

    Publisher->>EventBus: emit(PTT_PRESSED, payload)
    Note over EventBus: look up subscribers for PTT_PRESSED
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
        +unsubscribe(type, handler)
        +emit(event)
        -handlers: map~EventType, vector~Handler~~
    }
    class AudioEngine {
        +onPttPressed(event)
        +onLinkDown(event)
    }
    class DisplayModule {
        +onPttPressed(event)
        +onLinkDown(event)
    }
    class Logger {
        +onAnyEvent(event)
    }

    EventBus --> Event : routes
    AudioEngine ..> EventBus : subscribes to
    DisplayModule ..> EventBus : subscribes to
    Logger ..> EventBus : subscribes to
```

---

## Event-Driven vs Direct Call

```mermaid
flowchart LR
    subgraph Direct["❌ Direct Call — tight coupling"]
        A1[PTT Button] -->|calls| B1[Audio Engine]
        A1 -->|calls| C1[Display]
        A1 -->|calls| D1[Logger]
    end

    subgraph EventDriven["✅ Event-Driven — loose coupling"]
        A2[PTT Button] -->|emit| BUS[Event Bus]
        BUS --> B2[Audio Engine]
        BUS --> C2[Display]
        BUS --> D2[Logger]
    end
```

---

## When to Use

✅ MCX/MCPTT dispatch — PTT events, call state changes, link status  
✅ Qt applications — signals/slots are event-driven  
✅ Any system where producers and consumers must be independent  
✅ Adding new subscribers without changing existing code (Open/Closed Principle)  
❌ When you need guaranteed synchronous call order  
❌ When debugging — event chains can be hard to trace  
