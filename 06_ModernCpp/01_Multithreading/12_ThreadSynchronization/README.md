# ThreadSynchronization — Synchronization Primitives in C++23

When multiple threads share data, synchronization ensures that only one thread
modifies the data at a time, or that reads and writes happen in a defined order.
Without synchronization you get **race conditions** — unpredictable, hard-to-reproduce bugs.

---

## The problem — race condition

```cpp
int counter = 0;

void increment() {
    counter++;   // NOT atomic — three CPU instructions
}

std::thread t1(increment);
std::thread t2(increment);
t1.join(); t2.join();
// counter may be 1 instead of 2!
```

`counter++` compiles to: READ → ADD → WRITE.
If both threads READ before either WRITE, one increment is silently lost.

---

## Example 1 — std::mutex + std::lock_guard

`std::mutex` is the basic mutual exclusion primitive.
`std::lock_guard` is the RAII wrapper — locks on construction, unlocks on destruction.

```cpp
std::mutex mtx;
int counter = 0;

void increment() {
    std::lock_guard<std::mutex> lock(mtx);   // locks here
    counter++;
}   // lock released automatically — even if exception is thrown
```

### Rules
- Only one thread can hold the lock at a time
- Other threads block at `lock_guard` until the lock is released
- Never lock the same `std::mutex` twice from the same thread — deadlock
- Always use RAII wrappers — never call `.lock()`/`.unlock()` manually

---

## Example 2 — std::unique_lock (flexible)

`std::unique_lock` is like `lock_guard` but allows manual lock/unlock:

```cpp
std::unique_lock<std::mutex> lock(mtx);   // locks immediately

counter++;

lock.unlock();       // release early — other threads can proceed
doExpensiveWork();   // no lock needed here

lock.lock();         // re-acquire when needed
counter++;
```

| | `lock_guard` | `unique_lock` |
|--|-------------|--------------|
| RAII auto-unlock | ✅ | ✅ |
| Manual unlock/relock | ❌ | ✅ |
| Works with condition_variable | ❌ | ✅ |
| Moveable | ❌ | ✅ |
| Overhead | Lower | Slightly higher |

**Use `lock_guard` by default. Use `unique_lock` when you need manual control
or condition variables.**

---

## Example 3 — std::recursive_mutex

A normal `std::mutex` deadlocks if the same thread tries to lock it twice.
`std::recursive_mutex` allows the same thread to lock it multiple times:

```cpp
std::recursive_mutex rmtx;

void recursiveFunction(int depth) {
    std::lock_guard<std::recursive_mutex> lock(rmtx);
    if (depth > 0)
        recursiveFunction(depth - 1);   // same thread locks again — OK
}
```

Each lock must be matched by an unlock — released when lock count returns to zero.

> Use sparingly — recursive mutexes usually indicate a design that needs refactoring.

---

## Example 4 — std::timed_mutex

Allows trying to acquire a lock with a timeout — avoids waiting forever:

```cpp
std::timed_mutex tmtx;

if (tmtx.try_lock_for(std::chrono::milliseconds(50))) {
    doWork();
    tmtx.unlock();
} else {
    std::cout << "Timeout — lock not available\n";
}
```

Also available: `try_lock_until(time_point)` for an absolute deadline.

---

## Example 5 — std::shared_mutex (readers-writer lock)

Allows **multiple concurrent readers** but **exclusive writers**:

```cpp
std::shared_mutex rwMutex;
std::string data;

// Reader — shared lock (multiple readers allowed simultaneously)
void read() {
    std::shared_lock<std::shared_mutex> lock(rwMutex);
    std::cout << data;
}

// Writer — exclusive lock (no other readers or writers)
void write(const std::string& val) {
    std::unique_lock<std::shared_mutex> lock(rwMutex);
    data = val;
}
```

### When to use
Perfect for data that is **read frequently but written rarely**:
- Configuration settings, cached data, lookup tables
- MCX channel state tables

---

## Example 6 — std::scoped_lock (C++17)

Acquires **multiple mutexes simultaneously** without deadlock:

```cpp
std::mutex m1, m2;

// DEADLOCK RISK — two threads locking in different order:
// Thread 1: lock(m1) then lock(m2)
// Thread 2: lock(m2) then lock(m1)  ← deadlock!

// SAFE — scoped_lock uses deadlock avoidance algorithm:
void safeTask() {
    std::scoped_lock lock(m1, m2);   // both acquired atomically
}
```

Always prefer `scoped_lock` when you need to hold multiple mutexes.

---

## Example 7 — std::condition_variable

Used to make a thread **wait** until a condition is true.
Requires a `std::unique_lock` to work.

### Producer / Consumer pattern

```cpp
std::mutex              mtx;
std::condition_variable cv;
std::vector<int>        queue;
bool                    done = false;

// Producer
void producer() {
    for (int i = 0; i < 5; ++i) {
        { std::lock_guard lock(mtx); queue.push_back(i); }
        cv.notify_one();   // wake one waiting consumer
    }
    { std::lock_guard lock(mtx); done = true; }
    cv.notify_all();
}

// Consumer
void consumer() {
    while (true) {
        std::unique_lock lock(mtx);
        cv.wait(lock, []() { return !queue.empty() || done; });

        while (!queue.empty()) {
            int val = queue.front();
            queue.erase(queue.begin());
            lock.unlock();
            process(val);
            lock.lock();
        }
        if (done && queue.empty()) break;
    }
}
```

### Why the predicate lambda matters

```cpp
cv.wait(lock);                               // WRONG — spurious wakeups
cv.wait(lock, []() { return !q.empty(); }); // CORRECT — re-checks condition
```

`condition_variable::wait` can wake up **spuriously** without `notify` being called.
Always provide a predicate to re-check the condition after waking.

---

## Example 8 — std::atomic\<T\>

Lock-free synchronization for simple types — no mutex overhead:

```cpp
std::atomic<int>  counter{0};
std::atomic<bool> flag{false};

counter++;                  // atomic increment
counter.fetch_add(1);       // explicit fetch_add
flag.store(true);           // atomic store
bool val = flag.load();     // atomic load

// Compare-and-swap (CAS) — foundation of lock-free algorithms
int expected = 0;
counter.compare_exchange_strong(expected, 1);
// If counter == 0: sets counter = 1, returns true
// If counter != 0: sets expected = counter, returns false
```

### When to use atomic vs mutex

| | `std::atomic` | `std::mutex` |
|--|--------------|-------------|
| Use for | Single variable | Multiple variables or complex operations |
| Performance | Very fast (hardware instructions) | Slower (OS kernel involvement) |
| Composability | Single operation only | Can protect any block of code |
| Types supported | int, bool, pointer, trivially copyable | Any type |

---

## Example 9 — std::osyncstream (C++20)

Synchronized output stream — prevents garbled `std::cout` without a mutex:

```cpp
#include <syncstream>

void threadFunc(int id) {
    std::osyncstream(std::cout) << "Thread " << id << ": hello\n";
}
```

Without `osyncstream`, output from multiple threads interleaves:
```
ThTrhreread a1d: :h ehllelloo
```

With `osyncstream`, each line is always complete:
```
Thread 1: hello
Thread 2: hello
```

---

## Mutex type summary

| Type | Use case |
|------|----------|
| `std::mutex` | Standard mutual exclusion — use by default |
| `std::recursive_mutex` | Same thread needs to lock multiple times |
| `std::timed_mutex` | Need timeout on lock acquisition |
| `std::shared_mutex` | Many readers, few writers |

## Lock wrapper summary

| Wrapper | Characteristic |
|---------|---------------|
| `std::lock_guard` | Simple RAII — lock on construct, unlock on destruct |
| `std::unique_lock` | Flexible — manual lock/unlock, works with condition_variable |
| `std::shared_lock` | Read lock for `shared_mutex` |
| `std::scoped_lock` | Lock multiple mutexes atomically (C++17) |

---

## Golden rules of synchronization

1. **Protect all shared mutable state** — if two threads access the same variable and one writes, use synchronization
2. **Use RAII locks** — never call `.lock()`/`.unlock()` manually
3. **Keep critical sections short** — hold locks for the minimum time necessary
4. **Never lock inside a locked section** unless using `recursive_mutex` or `scoped_lock`
5. **Always lock in the same order** when acquiring multiple mutexes — or use `scoped_lock`
6. **Prefer `atomic` for single variables** — faster than mutex for simple counters/flags
7. **Use `condition_variable` with a predicate** — never `wait()` without one
