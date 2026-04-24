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

    subgraph PTT["03 PTT Simplified"]
        P1[State Machine]
        P2[Event-Driven]
        P3[Circuit Breaker]
        P4[Layered]
    end

    style CHAT fill:#2196F3,color:#fff
    style SCHED fill:#4CAF50,color:#fff
    style PTT fill:#FF5722,color:#fff
```

---

## Why These Three

| Example | Why it matters to you |
|---|---|
| Chat Application | Classic distributed system — taught in every system design interview |
| Task Scheduler | Priority queues + threads — core of any real-time system |
| PTT Simplified | Your MCX domain — state machine driving a PTT floor control session |
