# Pipes

## What are Pipes?

A pipe is a **one-directional data channel** between two processes. Data written to one end comes out the other end — like a physical pipe with water flowing through it.

```
Process A                          Process B
┌──────────┐                      ┌──────────┐
│          │   ==================  │          │
│  Writer  │──→  kernel buffer  ──→│  Reader  │
│          │   ==================  │          │
└──────────┘    (pipe)             └──────────┘
```

The kernel manages the buffer in between — no shared memory mapping needed.

---

## Two Types of Pipes

### 1. Anonymous Pipe
- Created with `pipe()`
- Works **only between a parent and its child process**
- Created before `fork()` so both processes inherit the file descriptors
- Destroyed automatically when both ends are closed
- No name on the filesystem

### 2. Named Pipe (FIFO)
- Created with `mkfifo()`
- Works between **any two unrelated processes**
- Has a **name on the filesystem** (e.g. `/tmp/my_fifo`)
- Must be opened by both sides before data can flow
- Persists on the filesystem until `unlink()` is called

---

## Files

| File | Description |
|---|---|
| `anonymous_pipe.cpp` | Parent writes, child reads — single executable, forks itself |
| `named_pipe_writer.cpp` | Creates a FIFO and writes messages into it |
| `named_pipe_reader.cpp` | Opens the FIFO and reads messages from it |
| `CMakeLists.txt` | Build configuration |

---

## Build & Run

```bash
mkdir build && cd build
cmake ..
make
```

**Anonymous pipe** — single terminal, forks automatically:
```bash
./anonymous_pipe
```

**Named pipe** — two terminals:
```bash
# Terminal 1 — writer (creates the FIFO)
./named_pipe_writer

# Terminal 2 — reader
./named_pipe_reader
```

> ⚠️ For named pipes, both processes block at `open()` until the **other side connects**.
> Start writer in Terminal 1, then reader in Terminal 2.

---

## Expected Output

**Anonymous pipe:**
```
=== Anonymous Pipe Example ===

[Parent] Writing: Hello from Parent!
[Child]  Received: Hello from Parent!
[Parent] Writing: Message #2 from Parent
[Child]  Received: Message #2 from Parent
[Parent] Writing: Goodbye from Parent!
[Child]  Received: Goodbye from Parent!
[Child]  Pipe closed. Exiting.
[Parent] Child finished. Done.
```

**Named pipe:**
```
# Terminal 1 (Writer)                    # Terminal 2 (Reader)
=== Named Pipe - Writer ===              === Named Pipe - Reader ===
FIFO created at: /tmp/my_fifo
Waiting for reader to connect...         Waiting for writer to connect...
Reader connected! Starting to write...   Writer connected! Reading...

[Writer] Sending: First message          [Reader] Received: First message
[Writer] Sending: Second message         [Reader] Received: Second message
[Writer] Sending: Third message          [Reader] Received: Third message

[Writer] Done. FIFO removed.             [Reader] Pipe closed. Done.
```

---

## Key Concepts

### Anonymous Pipe — File Descriptors

```cpp
int pipe_fd[2];
pipe(pipe_fd);
// pipe_fd[0] = read end
// pipe_fd[1] = write end
```

After `fork()`, both parent and child have both ends. Each process must **close the end it doesn't use**:

```cpp
// Parent writes → close read end
close(pipe_fd[0]);
write(pipe_fd[1], data, size);

// Child reads → close write end
close(pipe_fd[1]);
read(pipe_fd[0], buffer, size);
```

Closing unused ends is important — the reader only gets EOF when **all** write ends are closed.

### Named Pipe — Blocking Behavior

```cpp
// Both of these BLOCK until the other side opens:
int fd = open(FIFO_PATH, O_WRONLY);  // Writer blocks until reader opens
int fd = open(FIFO_PATH, O_RDONLY);  // Reader blocks until writer opens
```

This built-in synchronization is a feature — no semaphore needed.

---

## Anonymous vs Named Pipe

| | Anonymous Pipe | Named Pipe (FIFO) |
|---|---|---|
| **Processes** | Parent ↔ Child only | Any two processes |
| **Created with** | `pipe()` | `mkfifo()` |
| **Filesystem entry** | No | Yes (`/tmp/my_fifo`) |
| **Lifetime** | Until both ends closed | Until `unlink()` |
| **Synchronization** | Built-in | Built-in (blocks on open) |
| **Use case** | Shell piping, subprocesses | Simple IPC between apps |

---

## Pipes vs Other IPC

| | Anonymous Pipe | Named Pipe | Shared Memory | Sockets |
|---|---|---|---|---|
| **Speed** | Fast | Fast | ⚡ Fastest | Medium |
| **Direction** | One-way | One-way | Both ways | Both ways |
| **Any processes** | ❌ Parent/child | ✅ | ✅ | ✅ |
| **Network** | ❌ | ❌ | ❌ | ✅ |
| **Needs sync** | ❌ Built-in | ❌ Built-in | ✅ Manual | ❌ Built-in |
| **Complexity** | Low | Low | Medium | Medium |

---

## Common Mistakes

| Mistake | Consequence |
|---|---|
| Not closing unused pipe ends | Reader never gets EOF, blocks forever |
| Starting reader before writer (named pipe) | Blocks waiting — this is normal, not an error |
| Writing to closed pipe | `SIGPIPE` signal / `EPIPE` error |
| Assuming atomic writes for large data | Only writes ≤ `PIPE_BUF` (4096 bytes) are atomic |