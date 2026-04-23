# Architectural Patterns

> How to structure a system at the highest level.  
> These patterns decide how your components talk to each other — before you write a single class.

---

## What Is an Architectural Pattern?

A design pattern solves a problem inside a class or a few classes.  
An architectural pattern solves a problem across an **entire system**.

```mermaid
flowchart TD
    A[You have a system to build] --> B{What is the\nprimary concern?}

    B -->|Separation of layers| C[01 Layered\nPresentation → Business → Data]
    B -->|UI reacts to data changes| D[02 MVC\nModel · View · Controller]
    B -->|Components react to events| E[03 Event-Driven\nPublisher · Subscriber · Bus]
    B -->|Independent deployable services| F[04 Microservices\nService · Message · Gateway]
    B -->|Transform data through stages| G[05 Pipe and Filter\nSource → Filter → Filter → Sink]

    style C fill:#4CAF50,color:#fff
    style D fill:#2196F3,color:#fff
    style E fill:#FF9800,color:#fff
    style F fill:#9C27B0,color:#fff
    style G fill:#F44336,color:#fff
```

---

## Patterns at a Glance

| # | Pattern | Key idea | Real-world example |
|---|---|---|---|
| 01 | Layered | Each layer only talks to the layer below | OS kernel, web backend |
| 02 | MVC | Data, display, and logic are separate objects | GUI apps, web frameworks |
| 03 | Event-Driven | Components communicate via events, not direct calls | MCX PTT dispatch, Qt signals |
| 04 | Microservices | Each function is an independent process | Cloud backends, APIs |
| 05 | Pipe & Filter | Data flows through a chain of transformations | FFmpeg, Unix pipes, compilers |

---

## Learning Path

```mermaid
flowchart LR
    A[01 Layered\nFoundation] --> B[02 MVC\nAdds UI separation]
    B --> C[03 Event-Driven\nAdds async reaction]
    C --> D[04 Microservices\nAdds deployment isolation]
    C --> E[05 Pipe Filter\nAdds data transformation]
```
