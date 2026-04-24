# 01 · Scalability

> Scalability is the ability to handle **growing load** without degrading performance.  
> The question isn't "does it work now?" — it's "does it work at 10x load?"

---

## Vertical vs Horizontal Scaling

```mermaid
flowchart LR
    subgraph VERT["Vertical Scaling (Scale Up)"]
        V1["Server\n2 CPU / 4GB"] -->|"upgrade"| V2["Server\n32 CPU / 256GB"]
        VN["✅ Simple — no code changes\n❌ Hard limit — biggest machine\n❌ Single point of failure\n❌ Expensive"]
    end

    subgraph HORIZ["Horizontal Scaling (Scale Out)"]
        H1["Server 1"] & H2["Server 2"] & H3["Server 3"]
        LB["Load Balancer"] --> H1 & H2 & H3
        HN["✅ Theoretically unlimited\n✅ No single point of failure\n❌ Needs load balancer\n❌ State must be shared"]
    end
```

---

## The Scalability Bottlenecks

```mermaid
flowchart TD
    REQ[High request volume] --> BOT{Bottleneck?}
    BOT -->|CPU bound| CPU["Add more servers\nHorizontal scaling"]
    BOT -->|Memory bound| MEM["Cache layer\nRedis / Memcached"]
    BOT -->|DB bound| DB["Read replicas\nSharding\nConnection pooling"]
    BOT -->|Network bound| NET["CDN\nEdge caching\nCompression"]
    BOT -->|Single thread| ST["Async I/O\nThread pool\nNon-blocking"]
```

---

## Caching Layers

```mermaid
flowchart LR
    C[Client] -->|"request"| L1["L1: In-process cache\n~0ms hit"]
    L1 -->|"miss"| L2["L2: Shared cache\nRedis ~1ms hit"]
    L2 -->|"miss"| L3["L3: Database\n~10-100ms"]

    style L1 fill:#4CAF50,color:#fff
    style L2 fill:#FF9800,color:#fff
    style L3 fill:#F44336,color:#fff
```

---

## What This Example Shows

```mermaid
flowchart LR
    A[ThreadPool\nN worker threads\nprocess tasks concurrently] --> B[Cache\nLRU eviction\navoid redundant work]
    B --> C[ConnectionPool\nreuse expensive resources\nno per-request overhead]
    C --> D[Benchmark\nmeasure throughput\nprove the improvement]
```

---

## When to Think About Scalability

✅ Before you hit the wall — add caching before latency degrades  
✅ When one component is clearly the bottleneck — profile first  
✅ When designing the data model — sharding later is painful  
❌ Premature optimisation — build simple first, scale when needed  
❌ Scaling what isn't measured — always profile before optimising
