# Async — Asynchronous Programming in C++23

C++ provides a high-level async model through `<future>` that lets you run
tasks concurrently and retrieve results later — without managing threads directly.

---

## Header

```cpp
#include <future>   // std::async, std::future, std::promise, std::packaged_task
#include <thread>   // std::this_thread::sleep_for
```

---

## The three tools

| Tool | Who sets the value | Use case |
|------|--------------------|----------|
| `std::async` | Automatic | Simplest — fire-and-forget task |
| `std::promise` | Manual (you call `set_value`) | Signal between threads |
| `std::packaged_task` | Wraps callable | Queue tasks for later execution |

All three give you a `std::future<T>` to retrieve the result.

---

## std::async — simplest async launch

```cpp
// Launch task on new thread immediately
auto fut = std::async(std::launch::async, myFunction, arg1, arg2);

doOtherWork();   // runs concurrently

int result = fut.get();   // blocks until ready
```

**Always specify `std::launch::async`** — the default policy may choose deferred.

---

## std::future — retrieving results

```cpp
std::future<int> fut = std::async(std::launch::async, compute);

fut.valid()       // true if future holds a shared state
fut.get()         // block until ready, return value, invalidate future
fut.wait()        // block until ready (no return value)

// Timed variants
fut.wait_for(50ms)    // returns future_status
fut.wait_until(tp)    // wait until absolute time point
```

### future_status values

```cpp
std::future_status::ready    // result is available
std::future_status::timeout  // timed out — not ready yet
std::future_status::deferred // task not started (launch::deferred)
```

**Important:** `get()` can only be called **once** — future becomes invalid after.

---

## launch::async vs launch::deferred

```cpp
// async — new thread, starts immediately
auto f1 = std::async(std::launch::async, task);

// deferred — no thread created, runs in calling thread at .get()
auto f2 = std::async(std::launch::deferred, task);
f2.get();   // task runs HERE in the calling thread

// Default (avoid!) — implementation chooses
auto f3 = std::async(task);   // may be async OR deferred
```

---

## std::promise + std::future — manual signaling

```cpp
std::promise<int> promise;
std::future<int>  future = promise.get_future();

// Producer thread
std::thread t([&promise]() {
    promise.set_value(42);   // sends value to consumer
});

// Consumer
int val = future.get();   // blocks until set_value called
t.join();
```

### void promise — one-shot signal

```cpp
std::promise<void> ready;
std::future<void>  signal = ready.get_future();

std::thread t([&ready]() {
    doInit();
    ready.set_value();   // signal — no value needed
});

signal.get();   // wait for signal
t.join();
```

---

## std::packaged_task — wrap callable

```cpp
std::packaged_task<int(int,int)> task(multiply);
std::future<int> fut = task.get_future();

std::thread t(std::move(task), 6, 7);   // run on thread
int result = fut.get();                  // 42
t.join();
```

Useful for task queues — create task, store future, execute later:

```cpp
std::vector<std::packaged_task<int()>> queue;
std::vector<std::future<int>>          results;

for (int i : {1,2,3,4}) {
    std::packaged_task<int()> pt([i]() { return i*i; });
    results.push_back(pt.get_future());
    queue.push_back(std::move(pt));
}

// Execute all
for (auto& t : queue) std::thread(std::move(t)).detach();

for (auto& r : results) std::cout << r.get() << ' ';
```

---

## Exception propagation

Exceptions thrown inside async tasks are stored in the future
and **rethrown** when `.get()` is called:

```cpp
auto fut = std::async(std::launch::async, []() -> int {
    throw std::runtime_error("task failed!");
    return 0;
});

try {
    fut.get();   // rethrows stored exception here
} catch (const std::runtime_error& e) {
    std::cout << e.what();
}
```

Promise can also store an exception:

```cpp
try { doRiskyWork(); }
catch (...) { promise.set_exception(std::current_exception()); }
```

---

## wait_for — non-blocking poll

```cpp
auto fut = std::async(std::launch::async, slowTask);

// Poll every 20ms
while (fut.wait_for(20ms) != std::future_status::ready) {
    updateProgressBar();
}
int result = fut.get();
```

---

## std::shared_future — multiple consumers

```cpp
// Regular future — only ONE get() allowed
std::future<int> fut = ...;

// shared_future — MANY get() calls, shareable between threads
std::shared_future<int> sf = fut.share();

// Multiple threads can all call .get()
std::thread t1([sf]() { std::cout << sf.get(); });
std::thread t2([sf]() { std::cout << sf.get(); });
// Can also call sf.get() from main thread — all get the same value
```

---

## Parallel computation pattern

```cpp
unsigned int cores = std::thread::hardware_concurrency();
int chunk = SIZE / cores;

std::vector<std::future<long long>> futures;
for (int i = 0; i < cores; ++i) {
    int from = i * chunk + 1;
    int to   = (i == cores-1) ? SIZE : (i+1)*chunk;
    futures.push_back(
        std::async(std::launch::async, [from, to]() {
            long long s = 0;
            for (int j = from; j <= to; ++j) s += j;
            return s;
        })
    );
}

long long total = 0;
for (auto& f : futures) total += f.get();
```

---

## Concurrent independent operations

```cpp
// Fire all at once — run concurrently
auto f1 = std::async(std::launch::async, fetchUser,   userId);
auto f2 = std::async(std::launch::async, fetchConfig, "server");
auto f3 = std::async(std::launch::async, fetchGroups, userId);

// Total time ≈ max(f1,f2,f3) — not sum
auto user   = f1.get();
auto config = f2.get();
auto groups = f3.get();
```

---

## async vs thread — when to use each

| | `std::async` | `std::thread` |
|--|-------------|--------------|
| Returns value | ✅ via future | ❌ manual |
| Exception propagation | ✅ automatic | ❌ manual |
| Simplicity | ✅ highest | Medium |
| Detach/join control | ❌ | ✅ |
| Use for | Tasks with results | Fire-and-forget workers |

---

## Common mistakes

```cpp
// 1. Forgetting launch::async — may run synchronously!
auto fut = std::async(task);          // DANGER — may be deferred
auto fut = std::async(std::launch::async, task);  // CORRECT

// 2. Calling get() twice — throws std::future_error
int r1 = fut.get();
int r2 = fut.get();   // THROWS — future already consumed

// 3. Future destructor blocks for launch::async
{
    auto fut = std::async(std::launch::async, longTask);
}   // BLOCKS here — destructor waits for task to finish!

// 4. Ignoring the future — same blocking problem
std::async(std::launch::async, task);   // blocks immediately!
auto fut = std::async(std::launch::async, task);   // correct — keep the future

// 5. Using regular future from multiple threads
std::future<int> fut = ...;
// t1: fut.get();   // OK
// t2: fut.get();   // DATA RACE — use shared_future instead
```

---

## When to use async

✅ Background computation while UI stays responsive
✅ Fetching multiple resources concurrently
✅ Parallel data processing (split workload across cores)
✅ MCX call setup pipeline (auth + group resolve + floor config in parallel)
✅ Any task where you want a result later without managing threads manually

❌ Fire-and-forget with no result → `std::thread` + detach
❌ Long-running background service → dedicated thread
❌ Many short tasks → thread pool (`QThreadPool` or custom)
❌ Qt signal/slot across threads → `QThread` + `moveToThread`
