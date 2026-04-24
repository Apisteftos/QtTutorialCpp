# 03 · PTT Simplified — Push-to-Talk State Machine

> A real MCX/MCPTT client manages a call state machine.  
> This is a simplified but architecturally faithful simulation of  
> how a MCPTT client handles a group call with PTT arbitration.

---

## MCX Group Call Architecture

```mermaid
flowchart TD
    CLIENT["📱 MCPTT Client\n(this example)"]
    GMS["🌐 Group Management Server\nMCX Core Network"]
    FLOOR["⚖️ Floor Control Server\nArbitrates who speaks"]
    MEDIA["🔊 Media Plane\nRTP Audio Stream"]

    CLIENT -->|"SIP: GROUP_CALL_REQUEST"| GMS
    GMS -->|"SIP: 200 OK"| CLIENT
    CLIENT -->|"MCCP: FLOOR_REQUEST"| FLOOR
    FLOOR -->|"MCCP: FLOOR_GRANTED / FLOOR_DENIED"| CLIENT
    CLIENT -->|"RTP audio"| MEDIA
    MEDIA -->|"RTP audio (others)"| CLIENT

    style CLIENT fill:#2196F3,color:#fff
    style FLOOR fill:#FF9800,color:#fff
    style GMS fill:#4CAF50,color:#fff
```

---

## Call State Machine

```mermaid
stateDiagram-v2
    [*] --> Idle

    Idle --> Joining : user presses JOIN\nSend GROUP_CALL_REQUEST

    Joining --> Active : server returns 200 OK\ncall established
    Joining --> Idle : timeout or rejection

    Active --> FloorRequesting : user presses PTT\nSend FLOOR_REQUEST
    Active --> Idle : call ended\nGROUP_CALL_RELEASE

    FloorRequesting --> Transmitting : FLOOR_GRANTED received\nstart RTP TX
    FloorRequesting --> Active : FLOOR_DENIED or\nFLOOR_TAKEN by other user

    Transmitting --> Active : user releases PTT\nSend FLOOR_RELEASE\nstop RTP TX
    Transmitting --> Active : FLOOR_REVOKED by server

    Active --> Receiving : FLOOR_TAKEN by other user\nstart RTP RX
    Receiving --> Active : other user releases floor\nFLOOR_IDLE
```

---

## Floor Control Arbitration

```mermaid
sequenceDiagram
    participant UA as User A (this client)
    participant FC as Floor Controller
    participant UB as User B

    UA->>FC: FLOOR_REQUEST
    Note over FC: check who has floor

    alt Floor is idle
        FC-->>UA: FLOOR_GRANTED
        UA->>UA: state = Transmitting
        UA->>FC: RTP audio stream

    else Floor taken by B
        FC-->>UA: FLOOR_DENIED (queue position=1)
        Note over UB: B releases PTT
        UB->>FC: FLOOR_RELEASE
        FC-->>UA: FLOOR_GRANTED (queued request fulfilled)
    end

    UA->>FC: FLOOR_RELEASE
    FC-->>UA: FLOOR_IDLE
    UA->>UA: state = Active
```

---

## Patterns Used

| Pattern | Where |
|---|---|
| State Machine | `ClientState` enum + `transition()` |
| Event-Driven | `FloorController` emits events to client |
| Layered | Signalling layer / Floor layer / Media layer |
| Observer | Client subscribes to floor events |

---

## Real MCX Standards Reference

This example simplifies ETSI TS 124 380 (MCPTT floor control) and  
3GPP TS 23.280 (MCX common architecture).  
Real implementations use SIP/SDP for call setup and RTCP for floor control.
