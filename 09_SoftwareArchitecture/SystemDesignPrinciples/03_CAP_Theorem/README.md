# 03 · CAP Theorem

> In a distributed system you can only **guarantee two** of three properties:  
> **C**onsistency · **A**vailability · **P**artition Tolerance  
> — Eric Brewer, 2000

---

## The Triangle

```mermaid
flowchart TD
    C["C — Consistency\nEvery read receives\nthe most recent write\nor an error"]
    A["A — Availability\nEvery request receives\na response (not error)\nbut possibly stale"]
    P["P — Partition Tolerance\nSystem continues operating\nwhen network splits\nnodes apart"]

    C --- A
    A --- P
    P --- C

    NOTE["In practice: P is not optional.\nNetworks always partition eventually.\nSo the real choice is C vs A."]

    style C fill:#2196F3,color:#fff
    style A fill:#4CAF50,color:#fff
    style P fill:#FF9800,color:#fff
    style NOTE fill:#FFF3E0
```

---

## CP vs AP Systems

```mermaid
flowchart LR
    subgraph CP["CP Systems\nConsistency + Partition Tolerance"]
        direction TB
        CP1["When partition occurs:\n→ Some nodes go offline\n→ No stale reads possible\n→ Requests may fail"]
        CP2["Examples:\nHBase, Zookeeper, etcd\nTraditional RDBMS cluster\nRaft-based systems"]
    end

    subgraph AP["AP Systems\nAvailability + Partition Tolerance"]
        direction TB
        AP1["When partition occurs:\n→ All nodes stay online\n→ May serve stale data\n→ Eventually consistent"]
        AP2["Examples:\nCassandra, DynamoDB\nCouchDB, Riak\nDNS"]
    end

    style CP fill:#2196F3,color:#fff
    style AP fill:#4CAF50,color:#fff
```

---

## What Happens During a Partition

```mermaid
sequenceDiagram
    participant C as Client
    participant N1 as Node 1
    participant N2 as Node 2 (partitioned)

    C->>N1: write(x = 42)
    N1->>N1: x = 42
    Note over N1,N2: Network partition!

    C->>N2: read(x)

    alt CP System (choose consistency)
        N2-->>C: ERROR — cannot guarantee consistency
    else AP System (choose availability)
        N2-->>C: x = 0  (stale — last known value)
    end
```

---

## Eventual Consistency

AP systems use eventual consistency — after a partition heals, all nodes converge:

```mermaid
flowchart LR
    A["Node A\nx=42 (new)"] -->|"partition heals\nsync"| B["Node B\nx=0 (stale)"]
    B -->|"after sync"| C["Node B\nx=42 (converged) ✓"]
```

---

## Real-World Choice Guide

| Use case | Choose | Reason |
|---|---|---|
| Bank account balance | CP | Wrong balance = fraud |
| Social media likes count | AP | Approximate is fine |
| MCX PTT floor control | CP | Two talkers = catastrophic |
| Product search results | AP | Slightly stale is fine |
| Distributed lock | CP | Must be exclusive |
| User profile display | AP | Eventually correct is OK |
