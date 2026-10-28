# Real-World Examples

> Theory applied to real systems.  
> Each example combines **multiple patterns** from previous sections  
> into one coherent, working program.

---

## Pattern Usage Map

```mermaid
flowchart TD
    subgraph CHAT["01 Chat Application"]
        C1[Event-Driven]
        C2[Client-Server]
        C3[Pub-Sub]
        C4[Layered]
    end

    subgraph SCHED["02 Task Scheduler"]
        S1[Priority Queue]
        S2[Thread Pool]
        S3[State Machine]
        S4[Observer]
    end

    subgraph ARB["03 Shared Printer Arbiter"]
        P1[State Machine]
        P2[Event-Driven]
        P3[Observer]
        P4[Layered]
    end

    style CHAT fill:#2196F3,color:#fff
    style SCHED fill:#4CAF50,color:#fff
    style ARB fill:#FF5722,color:#fff
```

---

## Why These Three

| Example | Why it matters to you |
|---|---|
| Chat Application | Classic distributed system — taught in every system design interview |
| Task Scheduler | Priority queues + threads — core of any real-time system |
| Shared Printer Arbiter | State machine + exclusive-access arbitration with a waiting queue — the core of any shared-resource controller |
