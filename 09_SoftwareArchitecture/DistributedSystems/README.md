# Distributed Systems

> A single machine has limits — CPU, memory, bandwidth, uptime.  
> Distributed systems spread work across many machines.  
> The cost: you must now deal with **partial failures, network delays, and consistency**.

---

## The Core Challenge

```mermaid
flowchart TD
    A[Single Machine\nSimple. Fast. Fails completely.] -->|scale| B[Distributed System\nComplex. Scalable. Fails partially.]

    B --> C{Three hard problems}
    C --> D[Consistency\nDo all nodes see the same data?]
    C --> E[Availability\nDoes the system respond even under failure?]
    C --> F[Partition Tolerance\nDoes it work when nodes can't talk?]

    D & E & F --> G[CAP Theorem\nYou can only guarantee TWO of three]

    style G fill:#FF9800,color:#fff
    style B fill:#2196F3,color:#fff
```

---

## What Is in This Section

```mermaid
flowchart LR
    A[01 Client-Server\nRequest · Response\nThe foundation] --> B[02 Pub-Sub\nPublish · Subscribe\nAsync decoupled comms]
    B --> C[03 Load Balancing\nRound-Robin · Least-Conn\nSpread the work]
    C --> D[04 Consensus\nLeader Election\nAgree despite failures]

    style A fill:#4CAF50,color:#fff
    style B fill:#2196F3,color:#fff
    style C fill:#FF9800,color:#fff
    style D fill:#9C27B0,color:#fff
```

---

## Quick Reference

| Pattern | Sync/Async | Failure mode | Real examples |
|---|---|---|---|
| Client-Server | Synchronous | Server down → client blocked | HTTP, MCX SIP signalling |
| Pub-Sub | Asynchronous | Broker down → messages lost | MQTT, MCX group calls |
| Load Balancing | Transparent | Node down → traffic rerouted | Nginx, cloud ALB |
| Consensus | Negotiated | Split-brain → quorum decides | Raft, ETCD, ZooKeeper |

---

## All examples are simulated in-process

Real distributed systems use sockets, message queues, and separate processes.  
These examples isolate the **logic and pattern** in pure C++23, no networking required.  
The patterns are identical — only the transport layer changes in production.
