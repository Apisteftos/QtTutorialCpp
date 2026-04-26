# Sockets

## What are Sockets?

A socket is a **communication endpoint**. Two connected sockets form a **bidirectional channel** — unlike pipes which are one-way. Most importantly, sockets work both **on the same machine** and **across a network**, making them the most versatile IPC mechanism.

```
Client                         Server
┌──────────┐                  ┌──────────┐
│          │    TCP/IP        │          │
│  socket  │ ←────────────→  │  socket  │
│          │   (both ways)    │          │
└──────────┘                  └──────────┘
  localhost:XXXX           localhost:9090
```

---

## Why Use Sockets?

- ✅ **Bidirectional** — both sides can send and receive
- ✅ Works **locally** (localhost) or over a **network**
- ✅ Foundation of all internet communication (HTTP, FTP, MCX/MCPTT)
- ✅ Well-supported on every OS
- ✅ Can handle **multiple clients** simultaneously

---

## Socket Workflows

### Server
```
socket()   → Create endpoint
bind()     → Attach to IP:port
listen()   → Start accepting connections
accept()   → Wait for a client (blocks)
recv/send  → Communicate
close()    → Disconnect
```

### Client
```
socket()   → Create endpoint
connect()  → Connect to server IP:port
recv/send  → Communicate
close()    → Disconnect
```

---

## Files

| File | Description |
|---|---|
| `server.cpp` | Binds to port 9090, accepts one client, echoes messages back |
| `client.cpp` | Connects to server, sends 5 messages, receives responses |
| `CMakeLists.txt` | Build configuration |

---

## Build & Run

```bash
mkdir build && cd build
cmake ..
make

# Terminal 1 — start server first
./server

# Terminal 2 — start client
./client
```

> ⚠️ Always start the server first. The client will fail to connect if no server is listening.

---

## Expected Output

```
# Terminal 1 (server)                       # Terminal 2 (client)
[Server] Socket created.                    [Client] Connecting to 127.0.0.1:9090...
[Server] Bound to port 9090.
[Server] Listening for connections...
                                            [Client] Connected!
[Server] Client connected from 127.0.0.1
                                            [Client] Sent:     "Hello Server!"
[Server] Received: "Hello Server!"
[Server] Sent:     "Server ACK #1 — got: Hello Server!"
                                            [Client] Received: "Server ACK #1 — got: Hello Server!"

                                            [Client] Sent:     "quit"
[Server] Client sent quit. Closing.
[Server] Shutdown complete.                 [Client] Connection closed.
```

---

## Key Concepts

### Port Numbers

```cpp
const int PORT = 9090;
// Ports 0-1023   → reserved (HTTP=80, HTTPS=443, SSH=22)
// Ports 1024-65535 → available for your apps
```

### Byte Order (Endianness)

Network protocols use **big-endian** byte order. Always convert:

```cpp
htons(PORT)   // host-to-network short  (for port)
htonl(addr)   // host-to-network long   (for IP)
ntohs(port)   // network-to-host short  (when reading)
```

### SO_REUSEADDR

```cpp
int opt = 1;
setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
```

Without this, restarting the server quickly causes "Address already in use" error.

### recv() Return Values

```cpp
ssize_t bytes = recv(fd, buffer, size, 0);
// bytes > 0  → data received
// bytes == 0 → client disconnected cleanly
// bytes == -1 → error
```

---

## Extending to Multiple Clients

The current example handles **one client**. For multiple clients, use:

```cpp
// Option 1: fork() a new process per client
pid_t pid = fork();
if (pid == 0) { /* handle client */ }

// Option 2: Thread per client
std::thread t(handleClient, client_fd);
t.detach();

// Option 3: select()/poll()/epoll() — non-blocking I/O multiplexing
// (used in production servers, MCX/MCPTT systems)
```

---

## Sockets vs Other IPC

| | Sockets | Shared Memory | Pipes | Msg Queues |
|---|---|---|---|---|
| **Direction** | Both ways | Both ways | One-way | Both ways |
| **Network** | ✅ Yes | ❌ No | ❌ No | ❌ No |
| **Speed** | Medium | ⚡ Fastest | Fast | Medium |
| **Message boundaries** | ❌ Stream | ❌ Raw | ❌ Stream | ✅ Discrete |
| **Needs sync** | ❌ Built-in | ✅ Manual | ❌ Built-in | ❌ Built-in |
| **Multiple clients** | ✅ Yes | ✅ Yes | ❌ | ✅ Yes |
| **Complexity** | Medium | Medium | Low | Medium |

---

## Common Mistakes

| Mistake | Consequence |
|---|---|
| Starting client before server | `connect()` fails immediately |
| Forgetting `htons()` on port | Wrong port, connection refused |
| Not setting `SO_REUSEADDR` | "Address already in use" on restart |
| Assuming one `recv()` = one `send()` | TCP is a stream — data may be split or combined |
| Not checking `recv()` return value | Miss disconnect, read garbage |
