# Shared Memory

## What is Shared Memory?

Shared Memory is the **fastest IPC mechanism** on Linux. Instead of copying data through the kernel (like pipes or sockets do), two processes map the **same physical RAM region** directly into their own address space.

```
Process A                   Process B
┌──────────────┐            ┌──────────────┐
│ virtual mem  │            │ virtual mem  │
│              │            │              │
│  ptr ──────┐ │            │ ┌───── ptr  │
└────────────┼─┘            └─┼────────────┘
             │                │
             ▼                ▼
        ┌─────────────────────────┐
        │    Physical RAM         │
        │   (shared region)       │
        └─────────────────────────┘
```

Both processes read and write to the **same memory** — no copying, no kernel involvement after setup.

---

## Why Use Shared Memory?

- ⚡ **Fastest** IPC method — no data copying
- ✅ Good for **large data** (video frames, audio buffers, structs)
- ✅ Good for **frequent updates** (sensor data, state machines)
- ✅ Persistent until explicitly unlinked (survives process restart)

---

## How It Works — Step by Step

```
1. shm_open()     → Create or open a named shared memory block
2. ftruncate()    → Set its size
3. mmap()         → Map it into your process's address space
4. sem_open()     → Create a semaphore for synchronization
5. read/write     → Use it like normal memory
6. munmap()       → Unmap when done
7. shm_unlink()   → Delete the shared memory block (Writer only)
```

---

## Files

| File | Description |
|---|---|
| `shared_data.h` | Shared struct definition and constants used by both clients |
| `client_a.cpp` | Writer — creates shared memory, writes data every 2 seconds |
| `client_b.cpp` | Reader — opens existing shared memory, reads data |
| `CMakeLists.txt` | Build configuration |

---

## Build & Run

```bash
mkdir build && cd build
cmake ..
make

# Terminal 1 — start writer first
./client_a

# Terminal 2 — start reader
./client_b
```

> ⚠️ Always start `client_a` first — it creates the shared memory block.

---

## Expected Output

```
# Terminal 1 (client_a)                  # Terminal 2 (client_b)
=== Client A (Writer) ===                === Client B (Reader) ===
A wrote: Message #0 from Client A        B read: Message #0 from Client A
A wrote: Message #1 from Client A        B read: Message #1 from Client A
A wrote: Message #2 from Client A        B read: Message #2 from Client A
A wrote: Message #3 from Client A        B read: Message #3 from Client A
A wrote: Message #4 from Client A        B read: Message #4 from Client A
A finished and cleaned up.               B finished.
```

---

## Key Concepts

### SharedData Struct (`shared_data.h`)

```cpp
struct SharedData {
    int   counter;       // incremented by writer
    char  message[256];  // text written by writer
    bool  data_ready;    // flag: new data available
};
```

Both processes agree on this layout — it must be **identical** in both.

### Semaphore Synchronization

Without a semaphore, both processes could read/write simultaneously → **race condition** → corrupted data.

```cpp
sem_wait(sem);   // lock — only one process enters
// ... read or write shared memory ...
sem_post(sem);   // unlock — other process can now enter
```

### Named Objects

Both shared memory and semaphore use names starting with `/`:

```cpp
const char* SHM_NAME = "/ipc_shared_data";   // visible in /dev/shm/
const char* SEM_NAME = "/ipc_semaphore";
```

---

## Common Mistakes

| Mistake | Consequence |
|---|---|
| Reader starts before Writer | `shm_open` fails — block doesn't exist yet |
| Forgetting `sem_wait/post` | Race condition, corrupted data |
| Forgetting `shm_unlink` | Shared memory stays in `/dev/shm/` after exit |
| Different `SharedData` layout in A and B | Reads garbage data |

---

## Shared Memory vs Other IPC

| | Shared Memory | Pipes | Sockets |
|---|---|---|---|
| **Speed** | ⚡ Fastest | Fast | Medium |
| **Data size** | Unlimited | Stream | Stream |
| **Direction** | Both ways | One-way | Both ways |
| **Network support** | ❌ | ❌ | ✅ |
| **Needs sync** | ✅ Manual | ❌ Built-in | ❌ Built-in |
| **Complexity** | Medium | Low | Medium |