# 02 · Task Scheduler

> A real system always has more work than it can do right now.  
> A task scheduler decides **what runs next**, **when**, and **how many times to retry**.

---

## Architecture

```mermaid
flowchart TD
    P1["📥 Producer A\nHTTP request handler"]
    P2["📥 Producer B\nCron job"]
    P3["📥 Producer C\nEvent listener"]

    PQ["⚖️ Priority Queue\nSorted by priority + deadline\nO(log N) insert/pop"]

    W1["⚙️ Worker 1"]
    W2["⚙️ Worker 2"]
    W3["⚙️ Worker 3"]

    RQ["🔄 Retry Queue\nFailed tasks requeued\nwith backoff + attempt count"]

    DLQ["💀 Dead Letter Queue\nExhausted max retries\nAwaiting manual review"]

    P1 & P2 & P3 -->|"submit(task)"| PQ
    PQ -->|"pop highest priority"| W1 & W2 & W3
    W1 & W2 & W3 -->|"on failure"| RQ
    RQ -->|"requeue"| PQ
    RQ -->|"max retries exceeded"| DLQ

    style PQ fill:#FF9800,color:#fff
    style RQ fill:#2196F3,color:#fff
    style DLQ fill:#F44336,color:#fff
```

---

## Priority + Deadline Ordering

```mermaid
flowchart LR
    subgraph QUEUE["Priority Queue (sorted)"]
        T1["🔴 CRITICAL\nSend alert\np=10 deadline=now"]
        T2["🟠 HIGH\nProcess payment\np=7 deadline=+1s"]
        T3["🟡 NORMAL\nSend email\np=5 deadline=+5s"]
        T4["🟢 LOW\nClean logs\np=1 deadline=+60s"]
    end
    T1 --> T2 --> T3 --> T4
```

---

## Retry with Exponential Backoff

```mermaid
sequenceDiagram
    participant S as Scheduler
    participant W as Worker
    participant RQ as RetryQueue

    S->>W: execute(task, attempt=1)
    W-->>S: FAILED
    S->>RQ: requeue(task, attempt=2, delay=1s)

    Note over RQ: wait 1s

    S->>W: execute(task, attempt=2)
    W-->>S: FAILED
    S->>RQ: requeue(task, attempt=3, delay=2s)

    Note over RQ: wait 2s (exponential)

    S->>W: execute(task, attempt=3)
    W-->>S: SUCCESS
```

---

## Task Lifecycle

```mermaid
stateDiagram-v2
    [*] --> Pending : submit()
    Pending --> Running : worker picks up
    Running --> Done : success
    Running --> Retrying : failure + attempts left
    Retrying --> Pending : backoff expired
    Running --> Dead : max retries exceeded
    Dead --> [*]
    Done --> [*]
```

---

## When to Use

✅ Background job processing — email, reports, notifications  
✅ Rate-limited external APIs — queue and throttle  
✅ Job queues — order incoming requests by priority  
✅ Any system where tasks arrive faster than they can be processed  
❌ Real-time hard deadlines — use a RTOS scheduler instead  
❌ Simple sequential scripts with no concurrency

## What was tested

- Compiled with GCC 13.3 (`-std=c++23`) using a stand-in `<print>` header, because `std::println` needs GCC 14 and it was not available here. The source files were compiled directly, not through CMake.
- Run and compared with the original `std::cout` output line by line. Differences: `bool` values print as `true`/`false` instead of `1`/`0`, floating-point numbers print in their shortest exact form instead of 6 significant digits, and values that change on every run (thread ids, timings, random data). I sampled the differing lines; I did not review every one.
- Run once with AddressSanitizer + UndefinedBehaviorSanitizer: no findings.

## Not tested

- The real `<print>` header (GCC 14+, recent Clang or MSVC) and platforms other than Linux x86-64.
