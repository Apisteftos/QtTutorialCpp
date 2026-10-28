# ConditionVariable — Thread Signaling in C++23

`std::condition_variable` lets threads **wait** for a condition and **notify** others when it changes.

---

## Basic pattern

```cpp
std::mutex mtx;
std::condition_variable cv;
bool ready = false;

// Waiting thread
std::unique_lock<std::mutex> lock(mtx);
cv.wait(lock, []() { return ready; });   // releases lock while waiting

// Notifying thread
{ std::lock_guard lock(mtx); ready = true; }
cv.notify_one();
```

---

## ALWAYS use a predicate

```cpp
cv.wait(lock);                              // WRONG — spurious wakeups!
cv.wait(lock, []() { return ready; });      // CORRECT
// Equivalent to: while (!ready) cv.wait(lock);
```

---

## notify_one vs notify_all

```cpp
cv.notify_one();   // wake ONE waiting thread
cv.notify_all();   // wake ALL waiting threads (broadcast)
```

---

## Bounded producer/consumer (two CVs)

```cpp
std::condition_variable notEmpty, notFull;

// Producer
notFull.wait(lock, []() { return queue.size() < MAX; });
queue.push(item);
notEmpty.notify_one();

// Consumer
notEmpty.wait(lock, []() { return !queue.empty() || done; });
auto item = queue.front(); queue.pop();
notFull.notify_one();
```

---

## wait_for — timeout

```cpp
bool signaled = cv.wait_for(lock, 100ms, []() { return ready; });
// true  → condition became true
// false → timed out
```

---

## wait_until — absolute deadline

```cpp
auto deadline = std::chrono::steady_clock::now() + 200ms;
cv.wait_until(lock, deadline, []() { return ready; });
```

---

## Barrier pattern

```cpp
// Wait until N threads all arrive
if (++arrived == N) {
    released = true;
    cv.notify_all();   // last thread releases everyone
} else {
    cv.wait(lock, []() { return released; });
}
```

---

## Common mistakes

```cpp
cv.wait(lock);              // no predicate — spurious wakeup bug
std::lock_guard lock(mtx);
cv.wait(lock);              // ERROR — needs unique_lock, not lock_guard
```

---

## Summary

| Method | Description |
|--------|-------------|
| `wait(lock, pred)` | Wait until pred — always use predicate |
| `wait_for(lock, dur, pred)` | Relative timeout |
| `wait_until(lock, tp, pred)` | Absolute deadline |
| `notify_one()` | Wake one thread |
| `notify_all()` | Wake all threads |

## What was tested

- Compiled with GCC 13.3 (`-std=c++23`) using a stand-in `<print>` header, because `std::println` needs GCC 14 and it was not available here. The source files were compiled directly, not through CMake.
- Run and compared with the original `std::cout` output line by line. Differences: `bool` values print as `true`/`false` instead of `1`/`0`, floating-point numbers print in their shortest exact form instead of 6 significant digits, and values that change on every run (thread ids, timings, random data). I sampled the differing lines; I did not review every one.
- Run once with AddressSanitizer + UndefinedBehaviorSanitizer: no findings.

## Not tested

- The real `<print>` header (GCC 14+, recent Clang or MSVC) and platforms other than Linux x86-64.
