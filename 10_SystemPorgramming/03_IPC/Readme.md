# IPC — Inter-Process Communication in C++

## What is IPC?

When two separate programs (processes) are running on Linux, they each live in their **own isolated memory space**. One process cannot directly read or write the memory of another — this is enforced by the OS for safety and stability.

**IPC (Inter-Process Communication)** is the set of mechanisms the OS provides so that separate processes can:

- Exchange data
- Synchronize their actions
- Send notifications to each other

---

## Why Do We Need IPC?

Consider two console applications running at the same time:

```
[ Client A ]  ←── how do they talk? ───→  [ Client B ]
```

They cannot share a `std::shared_ptr` or a global variable — those only exist within a single process. IPC is the solution.

Real-world use cases:

- A **server** process and a **client** process exchanging data
- A **logger** process collecting output from multiple apps
- A **GUI** process displaying data produced by a **backend** process
- Two **MCPTT clients** sharing session state (relevant for MCX systems)

---

## IPC Mechanisms Covered in This Folder

| Mechanism | Folder | Best For |
|---|---|---|
| Shared Memory | `SharedMemory/` | Large/frequent data, fastest option |
| Pipes | `Pipes/` | Simple one-way data streams |
| Sockets | `Sockets/` | Network or local flexible communication |
| Message Queues | `MessageQueues/` | Structured, decoupled message passing |
| Signals | `Signals/` | Lightweight notifications, no data payload |

---

## Comparison: All IPC Mechanisms

### Speed

```
Shared Memory  ██████████  Fastest  (direct memory access)
Pipes          ████████    Fast     (kernel buffer)
Message Queues ███████     Medium   (kernel managed)
Sockets        ██████      Medium   (protocol overhead)
Signals        █████       Instant  (but no data)
```

### Feature Comparison

| Feature | Shared Memory | Pipes | Sockets | Msg Queues | Signals |
|---|---|---|---|---|---|
| **Speed** | ⚡ Fastest | Fast | Medium | Medium | Instant |
| **Data size** | Large | Stream | Stream | Limited | None |
| **Direction** | Both ways | One-way | Both ways | Both ways | One-way |
| **Same machine only** | ✅ | ✅ | ❌ (network too) | ✅ | ✅ |
| **Needs sync** | ✅ (semaphore) | ❌ (built-in) | ❌ (built-in) | ❌ (built-in) | ❌ |
| **Persistence** | Until unlinked | Until closed | Until closed | Until unlinked | None |
| **Complexity** | Medium | Low | Medium | Medium | Low |

---

## When to Use Which?

```
Need to share LARGE amounts of data fast?
    → Shared Memory

Simple parent ↔ child data flow?
    → Anonymous Pipe

Two unrelated processes, simple stream?
    → Named Pipe (FIFO)

Need to communicate over a NETWORK?
    → Sockets

Sending structured messages, processes decoupled?
    → Message Queues

Just need to notify a process (no data)?
    → Signals
```

---

## How Linux IPC Works (Big Picture)

```
┌─────────────────────────────────────────────────┐
│                  Linux Kernel                   │
│                                                 │
│  ┌──────────┐  ┌───────┐  ┌──────────────────┐ │
│  │  Shared  │  │ Pipes │  │ Message Queues / │ │
│  │  Memory  │  │ FIFOs │  │    Sockets       │ │
│  └────┬─────┘  └───┬───┘  └────────┬─────────┘ │
│       │             │               │           │
└───────┼─────────────┼───────────────┼───────────┘
        │             │               │
   ┌────▼────┐   ┌────▼────┐   ┌─────▼────┐
   │ Process │   │ Process │   │ Process  │
   │    A    │   │    B    │   │    C     │
   └─────────┘   └─────────┘   └──────────┘
```

---

## Folder Structure

```
IPC/
├── README.md                  ← You are here
├── SharedMemory/
│   ├── README.md
│   ├── CMakeLists.txt
│   ├── shared_data.h
│   ├── client_a.cpp
│   └── client_b.cpp
├── Pipes/
│   ├── README.md
│   ├── CMakeLists.txt
│   ├── anonymous_pipe.cpp
│   ├── named_pipe_writer.cpp
│   └── named_pipe_reader.cpp
├── Sockets/
│   ├── README.md
│   ├── CMakeLists.txt
│   ├── server.cpp
│   └── client.cpp
├── MessageQueues/
│   ├── README.md
│   ├── CMakeLists.txt
│   ├── sender.cpp
│   └── receiver.cpp
└── Signals/
    ├── README.md
    ├── CMakeLists.txt
    └── signal_example.cpp
```

---

## Key Headers per Mechanism

```cpp
// Shared Memory
#include <fcntl.h>        // shm_open, O_CREAT, O_RDWR
#include <sys/mman.h>     // mmap, munmap
#include <semaphore.h>    // sem_open, sem_wait, sem_post

// Pipes
#include <unistd.h>       // pipe(), fork(), read(), write()
#include <sys/stat.h>     // mkfifo()

// Sockets
#include <sys/socket.h>   // socket(), bind(), listen(), accept()
#include <netinet/in.h>   // sockaddr_in

// Message Queues
#include <mqueue.h>       // mq_open, mq_send, mq_receive

// Signals
#include <signal.h>       // signal(), kill(), sigaction()
```

---

## Build All Examples

Each subfolder has its own `CMakeLists.txt`. To build a specific one:

```bash
cd SharedMemory   # or Pipes, Sockets, etc.
mkdir build && cd build
cmake ..
make
```