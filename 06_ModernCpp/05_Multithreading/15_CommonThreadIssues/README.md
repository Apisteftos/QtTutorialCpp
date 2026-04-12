# CommonThreadIssues — Threading Bugs and Fixes in C++23

A reference guide for the most common multithreading bugs, how to recognize them,
and how to fix them.

---

## Issue 1 — Race condition

**Symptom:** Wrong values, non-deterministic results, different output on each run.

```cpp
int counter = 0;
void increment() { counter++; }   // READ → ADD → WRITE (3 ops, not atomic)

std::thread t1(increment);
std::thread t2(increment);
// counter may be 1 instead of 2
```

**Fix:**
```cpp
// Option A: mutex
std::mutex mtx;
void increment() { std::lock_guard lock(mtx); counter++; }

// Option B: atomic (faster for single variables)
std::atomic<int> counter{0};
void increment() { counter++; }
```

---

## Issue 2 — Deadlock

**Symptom:** Program hangs forever — no output, no crash, just silence.

```cpp
// T1 locks A then waits for B
// T2 locks B then waits for A
// Circular wait — neither proceeds
```

**Fix:** use `std::scoped_lock` for multiple mutexes:
```cpp
std::scoped_lock lock(mutexA, mutexB);   // atomic — deadlock-free
```

---

## Issue 3 — Livelock

**Symptom:** Both threads are "running" (CPU usage high) but making no progress.

```cpp
while (true) {
    if (try_lock()) { if (ready) { use(); return; } unlock(); }
    yield();   // give other thread a turn — it does the same!
}
```

**Fix:** add randomized backoff:
```cpp
std::this_thread::sleep_for(
    std::chrono::milliseconds(rand() % 50));
```

---

## Issue 4 — Starvation

**Symptom:** One thread never gets scheduled — other threads monopolize the lock.

```cpp
// Greedy threads hold the lock for 20ms each — starving thread never gets in
```

**Fix:**
- Keep critical sections short
- Use fair scheduling primitives (`std::counting_semaphore`)
- Consider `std::shared_mutex` for read-heavy workloads

---

## Issue 5 — Dangling reference in lambda capture

**Symptom:** Crash or garbage values — often only in release builds.

```cpp
void bad() {
    int local = 42;
    std::thread t([&local]() {   // reference to stack variable
        use(local);              // local may be destroyed!
    });
    t.detach();
}   // local destroyed here
```

**Fix:** capture by value or use `shared_ptr`:
```cpp
// Capture by value — thread owns its own copy
std::thread t([local]() { use(local); });

// Or shared ownership
auto ptr = std::make_shared<int>(42);
std::thread t([ptr]() { use(*ptr); });
```

---

## Issue 6 — Forgetting join/detach

**Symptom:** `std::terminate()` called — program crashes immediately.

```cpp
{
    std::thread t(func);
}   // t is joinable — destructor calls std::terminate()
```

**Fix:**
```cpp
// Option A: always join
t.join();

// Option B: always detach
t.detach();

// Option C: use std::jthread (auto-joins)
std::jthread jt(func);   // joins automatically in destructor
```

---

## Issue 7 — Garbled std::cout output

**Symptom:** Output lines are mixed and incomplete:
```
[thre[thread 2ad 1] ] lineline 1
2
```

**Fix:**
```cpp
// Option A: mutex around cout
{ std::lock_guard lock(coutMtx); std::cout << msg << '\n'; }

// Option B: std::osyncstream (C++20) — no mutex needed
std::osyncstream(std::cout) << "[thread " << id << "] done\n";
```

---

## Issue 8 — Missing std::ref

**Symptom:** Changes in thread not visible in caller — silent copy.

```cpp
void modify(int& n) { n = 99; }

int val = 0;
std::thread t(modify, val);   // passes COPY — val stays 0
t.join();
// val is still 0!
```

**Fix:**
```cpp
std::thread t(modify, std::ref(val));   // passes reference
t.join();
// val is now 99
```

---

## Issue 9 — Use-after-move on thread

**Symptom:** Crash when calling `join()` or `get_id()` on a moved-from thread.

```cpp
std::thread t(func);
std::thread owner = std::move(t);

t.join();     // CRASH — t is empty after move
owner.join(); // correct
```

**Fix:** always use the new owner after `std::move`:
```cpp
if (t.joinable()) t.join();       // safe check
if (owner.joinable()) owner.join();
```

---

## Issue 10 — Accessing destroyed object from detached thread

**Symptom:** Crash or garbage — object destroyed while thread still uses it.

```cpp
class Widget {
    void start() {
        std::thread([this]() {
            sleep(100ms);
            doWork();   // 'this' may be invalid!
        }).detach();
    }
};
// Widget destroyed — thread still running with invalid 'this'
```

**Fix — option A:** join in destructor:
```cpp
~Widget() { if (t.joinable()) t.join(); }
```

**Fix — option B:** shared ownership:
```cpp
auto self = shared_from_this();   // requires inheriting std::enable_shared_from_this
std::thread([self]() { self->doWork(); }).detach();
```

---

## Issue 11 — False sharing

**Symptom:** Multi-threaded code is slower than single-threaded — unexpected.

Two variables on the same 64-byte cache line cause constant invalidation:

```cpp
struct Bad {
    int a;   // ← same cache line
    int b;   // ← same cache line
};

// Thread A writes a, Thread B writes b
// CPU must keep invalidating the shared cache line between cores
```

**Fix:** align to cache line boundary:
```cpp
struct alignas(64) Counter {
    int value;
};

Counter a, b;   // each on its own cache line — no false sharing
```

---

## Issue 12 — Double-checked locking (wrong pattern)

**Symptom:** Singleton initialized multiple times, or used before initialization.

```cpp
// WRONG — compiler/CPU may reorder ptr = new T before T is initialized
if (!initialized) {
    lock();
    if (!initialized) {
        ptr = new T();
        initialized = true;   // may be seen before T is constructed!
    }
}
```

**Fix:** use `std::call_once`:
```cpp
std::once_flag flag;
T* ptr = nullptr;

std::call_once(flag, [&]() {
    ptr = new T();
});
// ptr is guaranteed initialized after call_once
```

---

## Quick reference

| Issue | Symptom | Fix |
|-------|---------|-----|
| Race condition | Wrong values, non-deterministic | `mutex` or `atomic` |
| Deadlock | Hangs forever | `scoped_lock`, consistent order |
| Livelock | High CPU, no progress | Randomized backoff |
| Starvation | Thread never runs | Short critical sections |
| Dangling ref | Crash in detach | Capture by value |
| Forgot join | `std::terminate` | `jthread` or explicit join |
| Garbled cout | Mixed output | `osyncstream` or mutex |
| Missing ref | Changes lost | `std::ref()` |
| Use-after-move | Crash | Use new owner |
| Destroyed object | Crash/garbage | Join in destructor or `shared_ptr` |
| False sharing | Slower than expected | `alignas(64)` |
| Double-checked lock | Init race | `std::call_once` |
