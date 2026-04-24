# 01 · Client-Server

> The client asks. The server answers.  
> The most fundamental distributed pattern — everything else builds on top of it.

---

## Structure

```mermaid
sequenceDiagram
    participant C as Client
    participant S as Server

    C->>S: connect()
    S-->>C: accepted

    C->>S: request(GET /users/42)
    Note over S: process request\nlook up user 42
    S-->>C: response(200 OK, {id:42, name:"Kostas"})

    C->>S: request(POST /orders)
    Note over S: validate\npersist\nnotify
    S-->>C: response(201 Created, {orderId:7})

    C->>S: disconnect()
```

---

## Request-Response Lifecycle

```mermaid
flowchart TD
    A[Client sends Request] --> B{Server receives}
    B --> C[Parse request]
    C --> D[Route to handler]
    D --> E{Handler result}
    E -->|success| F[Build 200 response]
    E -->|not found| G[Build 404 response]
    E -->|error| H[Build 500 response]
    F & G & H --> I[Send response to client]
    I --> J[Client processes response]
```

---

## C++ Class Map

```mermaid
classDiagram
    class Request {
        +method: string
        +path: string
        +body: string
        +header(key) string
    }
    class Response {
        +statusCode: int
        +body: string
        +ok() bool
    }
    class IHandler {
        <<interface>>
        +handle(Request) Response
        +path() string
    }
    class Server {
        +registerHandler(handler)
        +dispatch(Request) Response
        -handlers: map~string,IHandler~
    }
    class Client {
        +get(path) Response
        +post(path, body) Response
        -server: Server&
    }

    Server --> IHandler : dispatches to
    Client --> Server : sends requests to
    IHandler --> Request : reads
    IHandler --> Response : produces
```

---

## Synchronous vs Asynchronous

```mermaid
flowchart LR
    subgraph SYNC["Synchronous (this example)"]
        SC[Client] -->|"blocks waiting"| SS[Server]
        SS -->|"response"| SC
    end

    subgraph ASYNC["Asynchronous"]
        AC[Client] -->|"fire and forget"| AS[Server]
        AS -.->|"callback later"| AC
    end
```

---

## When to Use

✅ Simple request/response interactions  
✅ When the client needs the result before continuing  
✅ HTTP APIs, database queries, MCX SIP registration  
❌ When the server takes too long — client blocks  
❌ When you need broadcast (one → many) — use Pub-Sub instead
