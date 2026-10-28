# 04 · Consensus Basics — Leader Election

> In a distributed system, nodes must sometimes **agree** on one thing.  
> Who is the leader? Which value is committed? Is this transaction valid?  
> Consensus is the mechanism by which they agree — even when some nodes fail.

---

## The Problem

```mermaid
flowchart TD
    A["3 nodes.\nAll think they might be leader.\nAll can fail at any time."] --> B{How do they agree?}

    B --> C[Election\nNodes vote for a candidate\nMajority wins]
    B --> D[Quorum\nNeed ⌊N/2⌋+1 votes to decide\nGuarantees no split-brain]
    B --> E[Term\nEach election has a term number\nHigher term = more recent]

    style A fill:#FF5722,color:#fff
    style C fill:#4CAF50,color:#fff
    style D fill:#2196F3,color:#fff
    style E fill:#9C27B0,color:#fff
```

---

## Raft Leader Election (simplified)

```mermaid
sequenceDiagram
    participant F1 as Follower 1
    participant F2 as Follower 2 (timeout)
    participant F3 as Follower 3

    Note over F1,F3: All start as Followers. Election timers fire randomly.
    Note over F2: F2's timer fires first

    F2->>F2: become Candidate, term++, vote for self
    F2->>F1: RequestVote(term=2, candidateId=F2)
    F2->>F3: RequestVote(term=2, candidateId=F2)

    F1-->>F2: VoteGranted=true
    F3-->>F2: VoteGranted=true

    Note over F2: received 3/3 votes (quorum = 2)
    F2->>F2: become Leader

    F2->>F1: Heartbeat (AppendEntries)
    F2->>F3: Heartbeat (AppendEntries)
    F1-->>F2: ACK
    F3-->>F2: ACK
```

---

## State Machine

```mermaid
stateDiagram-v2
    [*] --> Follower : start

    Follower --> Candidate : election timeout\n(no heartbeat received)
    Candidate --> Leader : quorum votes received
    Candidate --> Follower : higher term seen\nor split vote
    Leader --> Follower : higher term seen\n(another node became leader)

    Leader --> Leader : send heartbeats\nevery interval
    Follower --> Follower : reset timer\non heartbeat
```

---

## Quorum Rule

```mermaid
flowchart LR
    A["N = 5 nodes\nQuorum = ⌊5/2⌋+1 = 3"] --> B{Votes received}
    B -->|"3, 4 or 5"| C["✅ Leader elected\nMajority guaranteed"]
    B -->|"1 or 2"| D["❌ No quorum\nElection fails\nRetry with new term"]

    style C fill:#4CAF50,color:#fff
    style D fill:#F44336,color:#fff
```

---

## Why Quorum Prevents Split-Brain

If you need a majority to elect a leader:  
- At most **one** partition can have a majority  
- Two leaders cannot exist simultaneously  
- This is the core safety guarantee of Raft, Paxos, and ZooKeeper

---

## When to Use

✅ Distributed databases — who accepts writes?  
✅ Service registries — which node is the primary?  
✅ High-availability clusters — which server is the active one?  
✅ Any system where exactly one node must be authoritative  
❌ Single-node systems — no consensus needed  
❌ When eventual consistency is acceptable — use simpler gossip protocol

## What was tested

- Compiled with GCC 13.3 (`-std=c++23`) using a stand-in `<print>` header, because `std::println` needs GCC 14 and it was not available here. The source files were compiled directly, not through CMake.
- Run, and the output is identical to the original `std::cout` version.
- Run once with AddressSanitizer + UndefinedBehaviorSanitizer: no findings.

## Not tested

- The real `<print>` header (GCC 14+, recent Clang or MSVC) and platforms other than Linux x86-64.
