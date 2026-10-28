# ThreadPool — Reusable Thread Management in C++23

Pre-creates N threads and distributes tasks — avoids thread creation overhead.

---

## Why

Thread creation costs ~10-50µs. For many short tasks this dominates.
A pool amortizes the cost: create once, reuse forever.

---

## Basic structure

```cpp
class ThreadPool {
    std::vector<std::thread>          workers;
    std::queue<std::function<void()>> tasks;
    std::mutex                        mutex;
    std::condition_variable           cv;
    std::atomic<bool>                 stop{false};
};
```

Workers loop: wait → take task → run → repeat.
Destructor sets stop, notifies all, joins all.

---

## Submit void task

```cpp
ThreadPool pool(4);
pool.submit([]() { doWork(); });
pool.waitAll();
```

---

## Submit with return value

```cpp
auto fut = pool.submit([](int a, int b) { return a + b; }, 10, 20);
int result = fut.get();   // 30
```

Uses `packaged_task` internally — exceptions propagate through `fut.get()`.

---

## Exception handling

```cpp
auto bad = pool.submit([]() -> int {
    throw std::runtime_error("failed"); return 0; });
try { bad.get(); }
catch (const std::exception& e) { std::cout << e.what(); }
```

---

## Parallel computation

```cpp
ThreadPool pool(cores);
std::vector<std::future<long long>> futures;

for (int i=0; i<chunks; ++i)
    futures.push_back(pool.submit(processChunk, i));

long long total = 0;
for (auto& f : futures) total += f.get();
```

---

## Task pipeline

```cpp
auto raw    = pool.submit(fetchData);
auto parsed = pool.submit(parseData, raw.get());
auto result = pool.submit(processData, parsed.get());
std::cout << result.get();
```

---

## Pool sizing

| Task type | Pool size |
|-----------|-----------|
| CPU-bound | `hardware_concurrency()` |
| I/O-bound | `hardware_concurrency() * 2-8` |

---

## Lifecycle

```cpp
{
    ThreadPool pool(4);   // 4 workers created
    pool.submit(task1);
    pool.submit(task2);
}   // destructor: stop=true, notify_all, join all workers
```

---

## Use cases

- Request processing — one task per incoming request
- Qt UI — offload computation from main thread
- HTTP server — one task per request
- Image processing — parallel tile processing

## What was tested

- Compiled with GCC 13.3 (`-std=c++23`) using a stand-in `<print>` header, because `std::println` needs GCC 14 and it was not available here. The source files were compiled directly, not through CMake.
- Run and compared with the original `std::cout` output line by line. Differences: `bool` values print as `true`/`false` instead of `1`/`0`, floating-point numbers print in their shortest exact form instead of 6 significant digits, and values that change on every run (thread ids, timings, random data). I sampled the differing lines; I did not review every one.
- Run once with AddressSanitizer + UndefinedBehaviorSanitizer: no findings.

## Not tested

- The real `<print>` header (GCC 14+, recent Clang or MSVC) and platforms other than Linux x86-64.
