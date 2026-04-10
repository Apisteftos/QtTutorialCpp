# ThreadManagement — Managing Thread Lifecycles in C++23

Thread management covers how to safely create, control, stop, and clean up threads.
The modern C++20/23 approach with `std::jthread` eliminates most of the manual
bookkeeping that `std::thread` required.

---

## std::thread vs std::jthread

| | `std::thread` (C++11) | `std::jthread` (C++20) |
|--|----------------------|----------------------|
| Auto-join on destruction | ❌ — crashes if forgotten | ✅ |
| Stop token support | ❌ | ✅ built-in |
| Cooperative cancellation | Manual (`atomic<bool>`) | `std::stop_token` |
| Drop-in replacement | — | ✅ mostly compatible |

**Prefer `std::jthread` for all new code.**

---

## Example 1 — std::jthread: auto-joining thread

```cpp
{
    std::jthread jt([]() {
        std::this_thread::sleep_for(30ms);
        std::cout << "work done\n";
    });
}   // jt destructor calls join() automatically — no crash, no leak
```

With `std::thread` this would crash:
```cpp
{
    std::thread t(func);
}   // CRASH — t is joinable but destructor called → std::terminate()
```

`std::jthread` in a vector — all auto-joined when vector is destroyed:
```cpp
std::vector<std::jthread> workers;
for (int i = 0; i < 4; ++i)
    workers.emplace_back([i]() { doWork(i); });
// all threads joined automatically when workers goes out of scope
```

---

## Example 2 — Stopping threads with std::stop_token

`std::jthread` automatically passes a `std::stop_token` as the first parameter
if the callable accepts one:

```cpp
std::jthread worker([](std::stop_token token) {
    while (!token.stop_requested()) {
        doWork();
        std::this_thread::sleep_for(25ms);
    }
    std::cout << "stopped cleanly\n";
});

std::this_thread::sleep_for(100ms);
worker.request_stop();   // sets the token — thread exits on next check
// jthread destructor joins automatically
```

This is **cooperative cancellation** — the thread checks the token and exits
gracefully, releasing all its resources properly.

---

## Example 3 — std::stop_source and std::stop_callback

`std::stop_source` lets you control the stop signal from outside the thread:

```cpp
std::stop_source source;
std::stop_token  token = source.get_token();

// Register a callback — runs when stop is requested
std::stop_callback cb(token, []() {
    std::cout << "cleanup callback fired!\n";
});

std::jthread worker([token]() {
    while (!token.stop_requested())
        doWork();
});

source.request_stop();   // fires callback + sets token
// worker exits on next check, jthread auto-joins
```

`stop_callback` is useful for waking up sleeping threads or releasing resources
when cancellation is requested.

---

## Example 4 — Stopping with atomic flag (pre-C++20)

Before `std::jthread`, the standard pattern was an `std::atomic<bool>` flag:

```cpp
std::atomic<bool> stopFlag{false};

void worker() {
    while (!stopFlag.load()) {
        doWork();
        std::this_thread::sleep_for(30ms);
    }
    std::cout << "stopped\n";
}

std::thread t(worker);
std::this_thread::sleep_for(100ms);
stopFlag = true;
t.join();
```

This still works and is useful when you can't use C++20, or when you need
to stop threads from C code or legacy systems.

---

## Example 5 — Exception handling across threads

Exceptions do **not** propagate across thread boundaries automatically:

```cpp
std::thread t([]() {
    throw std::runtime_error("error");   // NOT caught by main thread
});
t.join();   // exception is lost — thread terminates silently
```

### Fix — use std::exception_ptr

```cpp
std::exception_ptr eptr;

std::thread t([&eptr]() {
    try {
        throw std::runtime_error("error in thread");
    } catch (...) {
        eptr = std::current_exception();   // capture it
    }
});
t.join();

if (eptr) {
    try {
        std::rethrow_exception(eptr);      // re-throw in main
    } catch (const std::exception& e) {
        std::cout << "Caught: " << e.what() << '\n';
    }
}
```

### std::async handles this automatically

```cpp
auto fut = std::async(std::launch::async, []() -> int {
    throw std::runtime_error("async error");
    return 42;
});

try {
    int result = fut.get();   // rethrows the exception here
} catch (const std::exception& e) {
    std::cout << "Caught: " << e.what() << '\n';
}
```

`std::future::get()` automatically rethrows any exception from the async task.

---

## Example 6 — Joining multiple threads

```cpp
std::vector<std::thread> threads;

for (int i = 0; i < 5; ++i)
    threads.emplace_back([i]() { doWork(i); });

// Join all
for (auto& t : threads)
    if (t.joinable()) t.join();
```

Always use `emplace_back` not `push_back` — threads are not copyable.
Always check `joinable()` before joining.

---

## Example 7 — Detach with completion tracking

If you must use `detach()`, track completion with an atomic counter:

```cpp
std::atomic<int> done{0};
int total = 4;

for (int i = 0; i < total; ++i) {
    std::thread([i, &done]() {
        doWork(i);
        ++done;
    }).detach();
}

while (done < total)
    std::this_thread::sleep_for(10ms);

std::cout << "all detached tasks complete\n";
```

---

## Example 8 — Thread pool

A thread pool pre-creates N worker threads and distributes tasks to them,
avoiding the overhead of creating/destroying threads for each task:

```cpp
ThreadPool pool(std::thread::hardware_concurrency());

for (int i = 0; i < 100; ++i) {
    pool.submit([i]() {
        processItem(i);
    });
}

pool.waitAll();   // wait for all submitted tasks to finish
// pool destructor joins all workers
```

### When to use a thread pool
- Many short tasks (per-request processing, event handling)
- When thread creation overhead matters
- When you want to limit the number of concurrent threads
- MCX message processing — one pool worker per incoming packet

---

## Example 9 — RAII ThreadGuard

Ensures threads are always joined when a scope exits, even if an exception
is thrown:

```cpp
class ThreadGuard {
    std::vector<std::thread>& threads;
public:
    explicit ThreadGuard(std::vector<std::thread>& t) : threads(t) {}
    ~ThreadGuard() {
        for (auto& t : threads)
            if (t.joinable()) t.join();
    }
};

{
    std::vector<std::thread> workers;
    ThreadGuard guard(workers);   // joins on scope exit

    for (int i = 0; i < 3; ++i)
        workers.emplace_back(doWork, i);

    // even if exception thrown here — guard joins all threads
}
```

With `std::jthread` this is no longer needed — `jthread` is its own RAII guard.

---

## Example 10 — Thread lifetime and object ownership

**The most important rule:** an object must **outlive** every thread that references it.

```cpp
// SAFE — jthread joined before data is destroyed
std::string data = "hello";
{
    std::jthread t([&data]() { use(data); });
}   // t joined here — data still alive

// DANGEROUS — data destroyed while thread may still run
void bad() {
    std::string data = "hello";
    std::thread t([&data]() { use(data); });
    t.detach();
}   // data destroyed here — thread reads garbage!
```

### Ownership rules
- **Join before data goes out of scope** — guaranteed safe
- **Capture by value** in detached thread lambdas — own the data
- **Use `shared_ptr`** when thread needs to outlive the creator
- **Never reference stack variables** from detached threads

```cpp
// SAFE detach pattern — thread owns its own copy
void safeDetach() {
    auto data = std::make_shared<std::string>("hello");
    std::thread([data]() {   // shared_ptr copy — thread owns it
        use(*data);
    }).detach();
}   // original shared_ptr destroyed, but thread holds its own copy
```

---

## Summary — modern thread management

```cpp
// C++20/23 preferred pattern:
std::jthread worker([](std::stop_token token) {
    while (!token.stop_requested()) {
        doWork();
    }
});
// No join needed — auto-joins on destruction
// Call worker.request_stop() to stop it
```

| Task | Modern C++20 | Legacy C++11 |
|------|-------------|-------------|
| Create thread | `std::jthread` | `std::thread` + manual join |
| Stop thread | `std::stop_token` | `std::atomic<bool>` flag |
| Auto-cleanup | `std::jthread` destructor | `ThreadGuard` RAII wrapper |
| Exception transfer | `std::async` + `future.get()` | `std::exception_ptr` |
| Multiple threads | `vector<std::jthread>` | `vector<std::thread>` + join loop |
