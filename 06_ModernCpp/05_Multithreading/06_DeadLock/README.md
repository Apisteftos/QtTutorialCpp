# Deadlock — Causes and Solutions in C++23

A **deadlock** occurs when two or more threads wait for each other forever.

---

## The four Coffman conditions (all must be true)

| Condition | Description |
|-----------|-------------|
| Mutual exclusion | Resource held by one thread only |
| Hold and wait | Holds one, waits for another |
| No preemption | Resources can't be forcibly taken |
| Circular wait | T1 waits for T2, T2 waits for T1 |

Break **any one** and deadlock cannot occur.

---

## Classic deadlock

```cpp
// T1: lock(A) → lock(B)
// T2: lock(B) → lock(A)
// Circular wait — neither proceeds
```

---

## Fix 1 — Consistent lock ordering

```cpp
// All threads always lock m1 before m2
void thread1() { lock(m1); lock(m2); }
void thread2() { lock(m1); lock(m2); }   // same order
```

---

## Fix 2 — std::scoped_lock (preferred C++17)

```cpp
std::scoped_lock lock(m1, m2);   // atomic — deadlock-free regardless of order
```

---

## Fix 3 — std::lock + adopt_lock (C++11)

```cpp
std::lock(m1, m2);   // locks both atomically
std::lock_guard lg1(m1, std::adopt_lock);
std::lock_guard lg2(m2, std::adopt_lock);
```

---

## Self-deadlock

```cpp
std::mutex mtx;
std::lock_guard l1(mtx);
std::lock_guard l2(mtx);   // DEADLOCK — same mutex, same thread

// Fix: std::recursive_mutex
```

---

## Livelock

Threads keep retrying, never making progress. Fix: randomized backoff:
```cpp
std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 50));
```

---

## Exception safety

```cpp
// UNSAFE — exception leaves mutex locked
mtx.lock();
doWork();      // throws → unlock() never called
mtx.unlock();

// SAFE — lock_guard always unlocks
std::lock_guard lock(mtx);
doWork();      // throws → destructor unlocks
```

---

## Prevention rules

1. Use `scoped_lock` for multiple mutexes
2. Always lock in consistent order
3. Keep critical sections short
4. Never call unknown code while holding a lock
5. Use `atomic` for single values — no lock needed
