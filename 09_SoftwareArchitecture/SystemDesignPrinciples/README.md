# System Design Principles

> Architectural patterns describe *shapes*.
> System design principles describe *properties* — qualities every production system must have.
> You build the shape first, then you verify it satisfies these properties.

---

## The Four Properties

```mermaid
flowchart TD
    SYS[Production System] --> SC[01 Scalability\nHandle growing load\nwithout redesign]
    SYS --> RE[02 Reliability\nKeep working\ndespite failures]
    SYS --> CA[03 CAP Theorem\nConsistency vs Availability\nvs Partition Tolerance]
    SYS --> AP[04 API Design\nClean contracts\nbetween components]

    style SC fill:#4CAF50,color:#fff
    style RE fill:#2196F3,color:#fff
    style CA fill:#FF9800,color:#fff
    style AP fill:#9C27B0,color:#fff
```

---

## Quick Reference

| Principle | The question it answers | Key trade-off |
|---|---|---|
| Scalability | Can it handle 10x the load? | Cost vs capacity |
| Reliability | Does it recover from failure? | Complexity vs uptime |
| CAP Theorem | What breaks during a network split? | Consistency vs Availability |
| API Design | Is the contract clean and stable? | Flexibility vs rigidity |

---

## Learning Order

Start with **Scalability** → **Reliability** → **CAP** → **API Design**.
