# Signals

## What are Signals?

Signals are **software interrupts** sent from one process to another (or from the OS to a process). They carry **no data** — just a number that means "something happened."

```
Process A                        Process B
┌──────────┐   kill(pid, SIG)   ┌──────────┐
│          │ ─────────────────→ │ INTERRUPT│
│          │                    │    ↓     │
│          │                    │ handler()│
└──────────┘                    └──────────┘
```

Think of signals like a **doorbell** — it tells you someone is there, but carries no message about who or why.

---

## Common Signals

| Signal | Number | Default Action | Description |
|---|---|---|---|
| `SIGINT` | 2 | Terminate | Ctrl+C pressed |
| `SIGTERM` | 15 | Terminate | Polite shutdown request |
| `SIGKILL` | 9 | Terminate | Force kill (**cannot be caught!**) |
| `SIGUSR1` | 10 | Terminate | User-defined — your custom use |
| `SIGUSR2` | 12 | Terminate | User-defined — your custom use |
| `SIGALRM` | 14 | Terminate | Timer expired (`alarm()`) |
| `SIGCHLD` | 17 | Ignore | Child process terminated |
| `SIGSEGV` | 11 | Core dump | Invalid memory access |
| `SIGPIPE` | 13 | Terminate | Write to closed pipe |

---

## Three Ways to Handle a Signal

```cpp
// 1. Custom handler
signal(SIGUSR1, myHandler);

// 2. Ignore it
signal(SIGTERM, SIG_IGN);

// 3. Restore default OS behavior
signal(SIGTERM, SIG_DFL);
```

> ⚠️ `SIGKILL` and `SIGSTOP` can **never** be caught, ignored, or overridden.

---

## Files

| File | Description |
|---|---|
| `signal_example.cpp` | Four examples: basic handlers, sigaction, sending signals, ignoring |
| `CMakeLists.txt` | Build configuration |

---

## Build & Run

```bash
mkdir build && cd build
cmake ..
make
./signal_example
```

While it's running, open a second terminal and try:
```bash
kill -SIGUSR1 <PID>     # PID is printed by the app
kill -SIGUSR2 <PID>
kill -SIGTERM <PID>
```

---

## Examples Covered

### Example 1 — Basic Handlers
Register handlers with `signal()` and wait for SIGINT, SIGTERM, SIGUSR1, SIGUSR2.

### Example 2 — `sigaction()` (Recommended over `signal()`)
```cpp
struct sigaction sa;
sa.sa_handler = myHandler;
sa.sa_flags   = SA_RESTART;   // don't break read()/write() calls
sigaction(SIGALRM, &sa, nullptr);
```
`sigaction()` is preferred because it's more portable and gives finer control.

### Example 3 — Sending Signals Between Processes
```cpp
pid_t pid = fork();
// ... in parent:
kill(pid, SIGUSR1);   // send signal to child's PID
```

### Example 4 — Ignoring Signals
```cpp
signal(SIGTERM, SIG_IGN);   // process won't die on SIGTERM
// ... do critical work ...
signal(SIGTERM, SIG_DFL);   // restore
```

---

## Signal Handler Rules

Signal handlers run **asynchronously** — they can interrupt any line of code. This means:

| ✅ Safe inside handler | ❌ Unsafe inside handler |
|---|---|
| `write()` (raw syscall) | `printf()` / `std::cout` |
| Set `volatile sig_atomic_t` flag | `malloc()` / `new` |
| `_exit()` | Mutex locks |
| `kill()` | Most STL operations |

**Best practice:** set a flag in the handler, check it in the main loop:

```cpp
volatile sig_atomic_t running = 1;

void handler(int) { running = 0; }

int main() {
    signal(SIGTERM, handler);
    while (running) {
        // do work
    }
}
```

---

## Signals vs Other IPC

| | Signals | Pipes | Shared Memory | Sockets |
|---|---|---|---|---|
| **Carries data** | ❌ None | ✅ Stream | ✅ Any | ✅ Stream |
| **Speed** | ⚡ Instant | Fast | ⚡ Fastest | Medium |
| **Direction** | One-way | One-way | Both ways | Both ways |
| **Use case** | Notifications | Data stream | Large data | Networked data |
| **Complexity** | Low | Low | Medium | Medium |

---

## Common Mistakes

| Mistake | Consequence |
|---|---|
| Calling `printf` inside handler | Undefined behavior (not async-signal-safe) |
| Using regular `int` instead of `sig_atomic_t` | Race condition on flag read/write |
| Trying to catch `SIGKILL` | Compile error / silently ignored |
| Not calling `waitpid()` after child exits | Zombie process (SIGCHLD not handled) |
