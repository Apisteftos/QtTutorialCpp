# SimpleThreadOperations — std::thread Operations in C++23

This example demonstrates all common operations available on `std::thread` objects
and the `std::this_thread` namespace.

---

## Thread creation with parameters

```cpp
void task(int id, std::string msg) {
    std::cout << id << ": " << msg << '\n';
}

std::thread t(task, 1, "Hello");   // parameters passed after the callable
t.join();
```

All arguments are **copied** into the thread by default.
Use `std::ref()` to pass by reference (see Callables example).

---

## Thread ID

```cpp
std::thread t(func);

t.get_id();                    // ID of thread t (from outside)
std::this_thread::get_id();    // ID of the current thread (from inside)
```

Thread IDs are unique within a running program but may be reused after a thread ends.

---

## joinable()

```cpp
std::thread t(func);
t.joinable();    // true  — thread is running or finished but not yet joined/detached

t.join();
t.joinable();    // false — thread has been joined

std::thread empty;
empty.joinable() // false — default-constructed thread has no associated thread
```

Always check `joinable()` before calling `join()` or `detach()`:
```cpp
if (t.joinable()) t.join();
```

---

## hardware_concurrency()

```cpp
unsigned int cores = std::thread::hardware_concurrency();
```

Returns the number of logical CPU cores available.
Use this to decide how many threads to create in a thread pool.
Returns `0` if the value is not computable.

---

## native_handle()

```cpp
auto handle = t.native_handle();   // pthread_t on Linux
```

Returns the platform-specific handle — `pthread_t` on Linux/macOS,
`HANDLE` on Windows. Needed for platform-specific operations like
setting thread priority or CPU affinity.

---

## Move semantics — threads cannot be copied

```cpp
std::thread t1(func);

std::thread t2 = t1;             // ERROR — copy deleted
std::thread t2 = std::move(t1); // OK — t1 is now empty (nullptr)

t1.joinable();   // false — t1 no longer owns the thread
t2.joinable();   // true  — t2 owns it now
t2.join();
```

Use `std::move` to transfer thread ownership to containers or other variables.

---

## swap()

```cpp
std::thread t1(func1);
std::thread t2(func2);

t1.swap(t2);   // t1 now runs func2, t2 now runs func1
// or:
std::swap(t1, t2);
```

---

## Vector of threads

```cpp
std::vector<std::thread> threads;

for (int i = 0; i < 4; ++i)
    threads.emplace_back([i]() { /* work */ });

for (auto& t : threads)
    if (t.joinable()) t.join();
```

Use `emplace_back` instead of `push_back` — threads cannot be copied so
`push_back` would fail. `emplace_back` constructs in place.

---

## this_thread namespace

| Function | Description |
|----------|-------------|
| `std::this_thread::get_id()` | ID of the current thread |
| `std::this_thread::sleep_for(duration)` | Sleep for a duration |
| `std::this_thread::sleep_until(time_point)` | Sleep until a specific time |
| `std::this_thread::yield()` | Hint to OS to reschedule — give up CPU |

```cpp
// Sleep for 500ms
std::this_thread::sleep_for(std::chrono::milliseconds(500));

// Sleep until a specific time point
auto wake = std::chrono::system_clock::now() + std::chrono::seconds(1);
std::this_thread::sleep_until(wake);

// Yield — cooperative multitasking hint
std::this_thread::yield();
```

---

## yield() — when to use

`yield()` hints to the OS scheduler that the current thread is willing to give up
its CPU time slice. Useful in busy-wait loops to avoid burning CPU:

```cpp
// Busy-wait without yield — burns 100% CPU on one core
while (!ready) { }

// Busy-wait with yield — friendlier to other threads
while (!ready) { std::this_thread::yield(); }

// Best practice — use condition variables instead of busy-wait
```

---

## join() vs detach() summary

| | `join()` | `detach()` |
|--|---------|-----------|
| Main thread waits | ✅ | ❌ |
| Thread runs independently | ❌ | ✅ |
| Thread object still usable | ❌ | ❌ |
| Safe if main exits | ✅ | ⚠️ risky |
| Use when | You need the result | Fire-and-forget background tasks |
