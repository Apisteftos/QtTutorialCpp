# ThreadAttributes — Thread Properties in C++23

This example demonstrates the attributes and properties that belong to each thread,
what is private per thread, and what is shared between all threads in a process.

---

## Thread vs Process memory model

```
Process Memory
├── Code segment          ← SHARED by all threads
├── Global/static vars    ← SHARED by all threads
├── Heap                  ← SHARED by all threads
│
├── Thread 1 Stack        ← PRIVATE to thread 1
├── Thread 2 Stack        ← PRIVATE to thread 2
└── Thread 3 Stack        ← PRIVATE to thread 3
```

Each thread also has its own private: **Program Counter**, **CPU Registers**, **Stack**.

---

## 1. Thread ID

Every thread has a unique identifier of type `std::thread::id`:

```cpp
// From outside the thread
std::thread t(func);
std::thread::id id = t.get_id();

// From inside the thread
std::thread::id myId = std::this_thread::get_id();
```

IDs are guaranteed unique within a running program.
They may be reused after a thread ends.

---

## 2. Thread state lifecycle

```
NEW → RUNNING → WAITING/BLOCKED → RUNNING → TERMINATED
```

| State | Description |
|-------|-------------|
| NEW | Thread created but not yet started |
| RUNNING | Currently executing on a CPU core |
| WAITING | Sleeping, waiting for mutex, I/O, condition variable |
| TERMINATED | Function returned — thread finished |

```cpp
std::thread t(func);
t.joinable();   // true  — RUNNING or finished but not yet joined
t.join();
t.joinable();   // false — TERMINATED and joined
```

---

## 3. Stack (private per thread)

Each thread has its own stack — local variables, function call frames.
Default stack size is typically **2–8 MB** depending on the OS.

```cpp
void func() {
    int localVar = 42;   // lives on THIS thread's stack
                         // other threads have different stacks
}
```

Stack overflow occurs if a thread exceeds its stack limit
(deep recursion, very large local arrays).

To change stack size, use `pthread_attr_setstacksize` via `native_handle()`.

---

## 4. Program Counter

Each thread has its own instruction pointer — it tracks which line of code
the thread is currently executing. This is what allows two threads to run
the same function independently at different points simultaneously.

---

## 5. CPU Registers

Each thread has its own set of CPU registers (general purpose, floating point, flags).
When the OS performs a **context switch** (switching which thread runs on a core),
it saves the current thread's registers and restores the next thread's registers.

This is why context switches have a small cost — register save/restore.

---

## 6. Thread priority and scheduling

```cpp
// Get native handle to access POSIX thread attributes
pthread_t native = t.native_handle();

int policy;
sched_param param;
pthread_getschedparam(native, &policy, &param);

std::cout << "Policy: " << policy << '\n';       // SCHED_OTHER = 0 (default)
std::cout << "Priority: " << param.sched_priority << '\n';  // 0 for normal threads
```

Scheduling policies on Linux:

| Policy | Description |
|--------|-------------|
| `SCHED_OTHER` | Default — fair time-sharing |
| `SCHED_FIFO` | Real-time FIFO — runs until it yields or blocks |
| `SCHED_RR` | Real-time round-robin — time sliced |

> Changing to real-time scheduling (`SCHED_FIFO`, `SCHED_RR`) requires root privileges.
> Relevant for MCX/MCPTT systems where call audio threads need real-time priority.

---

## 7. Thread-local storage

`thread_local` variables give each thread its own independent copy:

```cpp
thread_local int counter = 0;   // each thread has its own counter

void func(int id) {
    counter = id * 100;          // modifies only THIS thread's counter
    std::cout << counter;        // other threads are unaffected
}
```

Unlike global variables, `thread_local` variables are **not shared**.
Useful for: per-thread error codes, random number generators, logging context.

---

## 8. CPU affinity

By default the OS decides which core a thread runs on.
You can pin a thread to a specific core using `pthread_setaffinity_np`:

```cpp
cpu_set_t cpuset;
CPU_ZERO(&cpuset);
CPU_SET(0, &cpuset);   // pin to core 0

pthread_setaffinity_np(t.native_handle(), sizeof(cpu_set_t), &cpuset);
```

Useful in embedded/real-time systems to isolate critical threads on dedicated cores.

---

## 9. Joinable vs detached

| State | joinable() | Description |
|-------|-----------|-------------|
| Running | `true` | Thread is executing |
| Finished (not joined) | `true` | Done but resources not released |
| After `join()` | `false` | Resources released, main waited |
| After `detach()` | `false` | Running independently |
| Default constructed | `false` | No associated thread |

---

## 10. hardware_concurrency()

```cpp
unsigned int cores = std::thread::hardware_concurrency();
```

Number of logical CPU cores (includes hyperthreading).
Use this as the basis for thread pool size.

---

## What is shared between all threads

| Resource | Shared? |
|----------|---------|
| Code (text segment) | ✅ |
| Global variables | ✅ |
| Heap memory (`new`, `malloc`) | ✅ |
| File descriptors | ✅ |
| Process ID | ✅ |
| Signal handlers | ✅ |
| Stack | ❌ private |
| Program Counter | ❌ private |
| CPU Registers | ❌ private |
| `thread_local` variables | ❌ private copy per thread |
| `errno` | ❌ private per thread |
