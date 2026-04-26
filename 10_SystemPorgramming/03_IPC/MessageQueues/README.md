# Message Queues

## What is a Message Queue?

A Message Queue is a **kernel-managed list of messages**. One process puts messages in, another takes them out. Unlike pipes, messages have **boundaries** (each message is discrete) and **priorities** (higher priority messages are received first, regardless of send order).

```
Sender                    Kernel Queue                   Receiver
┌──────┐  mq_send()     ┌───────────────────┐  mq_receive()  ┌──────────┐
│      │ ─────────────→ │ [P10][P5][P5][P1] │ ────────────→  │          │
└──────┘                └───────────────────┘                 └──────────┘
                          highest priority                     
                          delivered first                      
```

---

## Why Use Message Queues?

- ✅ Messages are **discrete** — no need to parse a byte stream
- ✅ Built-in **priority** system
- ✅ Sender and receiver are **decoupled** — sender doesn't need receiver to be running
- ✅ Kernel handles synchronization — no semaphores needed
- ✅ Queue **persists** in `/dev/mqueue/` until explicitly deleted

---

## How It Works — Step by Step

```
Sender:
1. mq_open()     → Create or open queue (O_CREAT | O_WRONLY)
2. mq_send()     → Put message in queue with a priority
3. mq_close()    → Close handle (queue still exists)

Receiver:
1. mq_open()     → Open existing queue (O_RDONLY)
2. mq_receive()  → Get highest-priority message
3. mq_close()    → Close handle
4. mq_unlink()   → Delete the queue
```

---

## Files

| File | Description |
|---|---|
| `sender.cpp` | Creates queue, sends 5 messages with different priorities |
| `receiver.cpp` | Opens queue, reads all messages in priority order |
| `CMakeLists.txt` | Build configuration |

---

## Build & Run

```bash
mkdir build && cd build
cmake ..
make

# Step 1 — run sender first
./sender

# Step 2 — run receiver
./receiver
```

> Unlike shared memory, the sender **does not need to be running** when the receiver starts — messages wait in the queue.

---

## Expected Output

```
# Sender                                    # Receiver
[Sender] Sent msg #1 priority=5  → "Normal priority message"
[Sender] Sent msg #2 priority=5  → "Another normal message"
[Sender] Sent msg #3 priority=10 → "HIGH PRIORITY — process me first!"
[Sender] Sent msg #4 priority=1  → "Low priority message"
[Sender] Sent msg #5 priority=5  → "Final message"

                                            Messages waiting: 5
                                            [Receiver] Got msg #3 priority=10 → "HIGH PRIORITY"
                                            [Receiver] Got msg #1 priority=5  → "Normal priority"
                                            [Receiver] Got msg #2 priority=5  → "Another normal"
                                            [Receiver] Got msg #5 priority=5  → "Final message"
                                            [Receiver] Got msg #4 priority=1  → "Low priority"
```

Notice: msg #3 (highest priority) is received **first**, even though it was sent third.

---

## Key Concepts

### Queue Attributes

```cpp
struct mq_attr attr;
attr.mq_maxmsg  = 10;    // max messages in queue at once
attr.mq_msgsize = 256;   // max bytes per message
```

If the queue is **full**, `mq_send()` blocks until space is available.
If the queue is **empty**, `mq_receive()` blocks until a message arrives.

### Priority

```cpp
mq_send(mq, buffer, size, priority);   // 0 = lowest, higher = more urgent
unsigned int priority;
mq_receive(mq, buffer, size, &priority);  // always gets highest first
```

### Viewing the Queue on Linux

```bash
ls /dev/mqueue/          # list all named queues
cat /dev/mqueue/ipc_message_queue   # see queue stats
```

---

## Message Queues vs Other IPC

| | Msg Queues | Pipes | Shared Memory | Sockets |
|---|---|---|---|---|
| **Message boundaries** | ✅ Discrete | ❌ Stream | ❌ Raw memory | ❌ Stream |
| **Priority** | ✅ Built-in | ❌ | ❌ | ❌ |
| **Needs sync** | ❌ Built-in | ❌ Built-in | ✅ Manual | ❌ Built-in |
| **Decoupled timing** | ✅ Queue persists | ❌ | ❌ | ❌ |
| **Speed** | Medium | Fast | ⚡ Fastest | Medium |
| **Network** | ❌ | ❌ | ❌ | ✅ |
| **Complexity** | Medium | Low | Medium | Medium |

---

## Common Mistakes

| Mistake | Consequence |
|---|---|
| Running receiver before sender | `mq_open` fails — queue doesn't exist |
| Message larger than `mq_msgsize` | `mq_send` returns error |
| Forgetting `-lrt` in CMake | Linker error: `mq_open` not found |
| Not calling `mq_unlink` | Queue stays in `/dev/mqueue/` after process exits |
| Queue full, no error check | `mq_send` blocks indefinitely |
