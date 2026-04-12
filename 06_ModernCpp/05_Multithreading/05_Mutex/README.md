# Mutex — Mutual Exclusion in C++23

A **mutex** ensures that only one thread can access a shared resource at a time.

---

## The problem — race condition

```cpp
int counter = 0;
void increment() { counter++; }   // READ → ADD → WRITE (3 ops, not atomic)
// Two threads may both READ before either WRITE → result is wrong
```

---

## std::mutex + std::lock_guard (default choice)

```cpp
std::mutex mtx;
int counter = 0;

void increment() {
    std::lock_guard<std::mutex> lock(mtx);  // locks on construction
    counter++;
}   // unlocks on destruction — even if exception thrown
```

**Use `lock_guard` by default** — simplest, no overhead.

---

## std::unique_lock (flexible)

```cpp
std::unique_lock<std::mutex> lock(mtx);
counter++;
lock.unlock();        // release early for slow I/O
doNetworkCall();
lock.lock();          // re-acquire
counter++;
```

| | `lock_guard` | `unique_lock` |
|--|-------------|--------------|
| Auto-unlock | ✅ | ✅ |
| Manual unlock/relock | ❌ | ✅ |
| Works with condition_variable | ❌ | ✅ |

---

## std::recursive_mutex

Allows the **same thread** to lock multiple times:
```cpp
std::recursive_mutex rmtx;
void process(int depth) {
    std::lock_guard<std::recursive_mutex> lock(rmtx);
    if (depth > 0) process(depth - 1);   // re-locks — OK
}
```

---

## std::timed_mutex

```cpp
if (tmtx.try_lock_for(50ms)) {
    doWork();
    tmtx.unlock();
} else {
    std::cout << "Timeout\n";
}
```

---

## std::shared_mutex (readers-writer)

```cpp
std::shared_mutex rwmtx;
void read()  { std::shared_lock lock(rwmtx); /* many readers OK */ }
void write() { std::unique_lock lock(rwmtx); /* exclusive */ }
```

Use when data is **read often, written rarely** — config, caches, MCX state.

---

## std::scoped_lock (C++17) — multiple mutexes

```cpp
std::scoped_lock lock(m1, m2);   // both acquired atomically — deadlock-free
```

---

## try_lock — non-blocking

```cpp
if (mtx.try_lock()) { doWork(); mtx.unlock(); }
else                { doSomethingElse(); }
```

---

## Mutex type summary

| Type | Use case |
|------|----------|
| `std::mutex` | Default |
| `std::recursive_mutex` | Same thread locks multiple times |
| `std::timed_mutex` | Timeout on acquisition |
| `std::shared_mutex` | Many readers, few writers |

## Golden rules

1. Always use RAII wrappers — never `.lock()`/`.unlock()` manually
2. Keep critical sections short
3. Never lock the same `std::mutex` twice from the same thread
4. Use `scoped_lock` for multiple mutexes
5. Prefer `atomic<T>` for single variables
