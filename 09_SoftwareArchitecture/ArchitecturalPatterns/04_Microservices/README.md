# 04 · Microservices Architecture

> Each service owns one capability.  
> Services communicate via messages — not function calls.  
> A service can fail, restart, or be replaced without touching others.

---

## Structure

```mermaid
flowchart TD
    GW["🌐 API Gateway\nSingle entry point\nRoutes requests to services"]

    US["👤 UserService\nAuthentication\nUser profiles"]
    OS["📦 OrderService\nOrder creation\nOrder state"]
    NS["🔔 NotifyService\nEmail / SMS\nPush notifications"]
    IS["📊 InventoryService\nStock levels\nReservation"]

    MB["📨 Message Broker\nAsynchronous events\nbetween services"]

    Client["💻 Client"] --> GW
    GW --> US
    GW --> OS
    OS -->|"ORDER_PLACED event"| MB
    MB --> NS
    MB --> IS

    style GW fill:#FF9800,color:#fff
    style MB fill:#9C27B0,color:#fff
    style US fill:#2196F3,color:#fff
    style OS fill:#2196F3,color:#fff
    style NS fill:#4CAF50,color:#fff
    style IS fill:#4CAF50,color:#fff
```

---

## Synchronous vs Asynchronous Communication

```mermaid
sequenceDiagram
    participant Client
    participant Gateway
    participant OrderService
    participant MessageBroker
    participant InventoryService
    participant NotifyService

    Client->>Gateway: POST /order
    Gateway->>OrderService: createOrder(payload)
    OrderService-->>Gateway: OrderID=42
    Gateway-->>Client: 201 Created

    Note over OrderService: publishes async event
    OrderService->>MessageBroker: ORDER_PLACED {id=42}
    MessageBroker->>InventoryService: reserve stock
    MessageBroker->>NotifyService: send confirmation
```

---

## Service Boundaries

```mermaid
mindmap
  root((Microservices))
    Each service
      Owns its own data store
      Exposes a clean API
      Deployed independently
      Has its own CMake target
    Communication
      Sync: request/response
      Async: message broker
      Never shared memory
    Failure isolation
      One service crashes
      Others keep running
      Circuit breaker pattern
```

---

## When to Use

✅ Large teams — each team owns one service  
✅ When services scale independently (OrderService needs 10x NotifyService)  
✅ When you need to deploy parts independently  
❌ Small teams — monolith first, extract services when needed  
❌ High-frequency inter-service calls — latency adds up  
❌ Transactions spanning services — distributed transactions are painful  

---

## In This Example

Simulated in-process using a **MessageBroker** class (queue + dispatch).  
Each `Service` class runs in its own method scope — simulating independent deployment.  
In production: replace `MessageBroker` with RabbitMQ, Kafka, or MCX message queue.
