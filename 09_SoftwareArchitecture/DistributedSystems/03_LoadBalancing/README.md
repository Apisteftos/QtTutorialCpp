# 03 · Load Balancing

> One server cannot handle infinite load.  
> A load balancer distributes requests across a pool of servers.  
> When a server fails, the balancer routes around it automatically.

---

## Structure

```mermaid
flowchart TD
    C1["💻 Client 1"]
    C2["💻 Client 2"]
    C3["💻 Client 3"]
    LB["⚖️ Load Balancer\nSelects next server\nHealth checks\nConnection tracking"]
    S1["🖥️ Server A\nactive"]
    S2["🖥️ Server B\nactive"]
    S3["🖥️ Server C\nactive"]
    S4["🖥️ Server D\n❌ down"]

    C1 & C2 & C3 --> LB
    LB -->|"round-robin"| S1
    LB -->|"round-robin"| S2
    LB -->|"round-robin"| S3
    LB -.->|"skipped (health check failed)"| S4

    style LB fill:#FF9800,color:#fff
    style S4 fill:#F44336,color:#fff
```

---

## Balancing Algorithms

```mermaid
flowchart TD
    A[Request arrives] --> B{Strategy?}

    B -->|Round-Robin| C["Take next server\nin circular order\n✅ Simple\n✅ Even distribution\n❌ Ignores server load"]

    B -->|Least Connections| D["Pick server with\nfewest active requests\n✅ Adapts to load\n❌ Needs tracking"]

    B -->|Weighted| E["Servers have weights\nStronger server → higher weight\n✅ Heterogeneous pools\n❌ Static config"]

    B -->|IP Hash| F["hash(client_ip) % N\nSame client → same server\n✅ Session stickiness\n❌ Uneven if few clients"]
```

---

## Health Checks

```mermaid
sequenceDiagram
    participant LB as Load Balancer
    participant SA as Server A (healthy)
    participant SB as Server B (failing)

    loop every 5 seconds
        LB->>SA: GET /health
        SA-->>LB: 200 OK
        LB->>SB: GET /health
        SB-->>LB: timeout / 500
        Note over LB: mark Server B as DOWN
        Note over LB: remove from rotation
    end

    Note over LB,SB: Server B recovers
    LB->>SB: GET /health
    SB-->>LB: 200 OK
    Note over LB: mark Server B as UP
    Note over LB: add back to rotation
```

---

## When to Use

✅ High-traffic services that need horizontal scaling  
✅ Zero-downtime deployments — remove one server, update, add back  
✅ Geographic distribution — route to nearest server  
✅ MCX: multiple dispatch servers handling call setup load  
❌ Single-server systems with no replication  
❌ When session state can't be shared across servers (solve with sticky sessions or shared cache)
