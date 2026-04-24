# 02 · Reliability

> A reliable system keeps working **when things go wrong**.  
> Not if — when. Networks fail. Services crash. Disks fill up.  
> Reliability patterns are your safety nets.

---

## The Three Patterns

```mermaid
flowchart TD
    F[Failure occurs] --> R{Pattern}

    R --> RET["🔁 Retry\nTry again automatically\nwith exponential backoff"]
    R --> CB["⚡ Circuit Breaker\nStop trying when\nfailure rate is high"]
    R --> FB["🛡️ Fallback\nReturn cached/default data\nwhen primary fails"]

    RET -->|"still failing after N retries"| CB
    CB -->|"open circuit"| FB

    style RET fill:#4CAF50,color:#fff
    style CB  fill:#FF9800,color:#fff
    style FB  fill:#2196F3,color:#fff
```

---

## Retry with Exponential Backoff

```mermaid
sequenceDiagram
    participant C as Client
    participant S as Service (flaky)

    C->>S: attempt 1
    S-->>C: 503 error
    Note over C: wait 100ms

    C->>S: attempt 2
    S-->>C: 503 error
    Note over C: wait 200ms

    C->>S: attempt 3
    S-->>C: 200 OK ✓
```

---

## Circuit Breaker States

```mermaid
stateDiagram-v2
    [*] --> Closed : start (normal operation)

    Closed --> Open : failure rate > threshold\n(e.g. 5 failures in 10s)
    Open --> HalfOpen : timeout elapsed\n(e.g. 30 seconds)
    HalfOpen --> Closed : probe request succeeds
    HalfOpen --> Open : probe request fails

    Closed : ✅ Requests pass through
    Open : ❌ Requests fail fast\nno calls to service
    HalfOpen : 🔍 One probe allowed\ntest if service recovered
```

---

## Fallback Strategy

```mermaid
flowchart LR
    A[Request] --> B[Primary Service]
    B -->|"success"| C[Return result]
    B -->|"failure"| D{Fallback}
    D -->|"cache hit"| E[Return cached value]
    D -->|"cache miss"| F[Return default value]
    D -->|"critical path"| G[Fail fast with clear error]
```

---

## When to Use Each

| Pattern | Use when | Don't use when |
|---|---|---|
| Retry | Transient failures (network blip) | Service is truly down (wastes resources) |
| Circuit Breaker | Known-flaky downstream service | The call is critical and has no alternative |
| Fallback | User-facing features with degraded mode | The result must be accurate (financial data) |
