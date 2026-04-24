# 02 · Publish-Subscribe

> Publishers emit topics. Subscribers listen to topics.  
> They never know each other exist.  
> The broker is the only shared infrastructure.

---

## Structure

```mermaid
flowchart TD
    P1["📡 Publisher\nWeatherStation"]
    P2["📡 Publisher\nTrafficSensor"]
    BR["🗄️ Message Broker\nRoutes by topic\nQueues messages\nManages subscriptions"]
    S1["🔔 Subscriber\nMobileApp"]
    S2["🔔 Subscriber\nDashboard"]
    S3["🔔 Subscriber\nAlertService"]

    P1 -->|"publish: weather.update"| BR
    P2 -->|"publish: traffic.jam"| BR
    BR -->|"weather.update"| S1
    BR -->|"weather.update"| S2
    BR -->|"traffic.jam"| S2
    BR -->|"traffic.jam + weather.update"| S3

    style BR fill:#9C27B0,color:#fff
    style P1 fill:#2196F3,color:#fff
    style P2 fill:#2196F3,color:#fff
    style S1 fill:#4CAF50,color:#fff
    style S2 fill:#4CAF50,color:#fff
    style S3 fill:#4CAF50,color:#fff
```

---

## Client-Server vs Pub-Sub

```mermaid
flowchart LR
    subgraph CS["Client-Server\n(pull / request-response)"]
        C[Client] -->|"GET /weather"| S[Server]
        S -->|"response"| C
        Note1["Client must ask.\nIf client is offline it misses nothing\n— it just asks later."]
    end

    subgraph PS["Pub-Sub\n(push / event-driven)"]
        PB[Publisher] -->|"weather.update"| B[Broker]
        B -->|"push"| SB[Subscriber]
        Note2["Broker pushes immediately.\nIf subscriber is offline\n— message may be lost or queued."]
    end
```

---

## Topic Hierarchy

```mermaid
flowchart TD
    ROOT["#  (wildcard all)"]
    W["weather.*"]
    T["traffic.*"]
    W1["weather.update"]
    W2["weather.alert"]
    T1["traffic.jam"]
    T2["traffic.clear"]

    ROOT --> W & T
    W --> W1 & W2
    T --> T1 & T2

    S1["MobileApp\nsubscribes: weather.*"] -.-> W
    S2["Dashboard\nsubscribes: weather.* + traffic.*"] -.-> W & T
    S3["AlertService\nsubscribes: weather.alert + traffic.jam"] -.-> W2 & T1
```

---

## Message Delivery Guarantees

| Mode | Behaviour | Use case |
|---|---|---|
| At-most-once | May lose messages | Live telemetry, metrics |
| At-least-once | May duplicate | Orders, commands |
| Exactly-once | No loss, no duplication | Financial transactions |

*This example implements at-most-once (fire-and-forget).*

---

## When to Use

✅ One event → multiple independent consumers  
✅ MCX group call dispatch — one PTT event → audio + display + logger  
✅ IoT sensor data → multiple dashboards  
✅ When publishers and subscribers must be decoupled in time and space  
❌ When you need a guaranteed reply from a specific recipient — use Client-Server  
❌ When message order within a topic is critical and strict — use a queue
