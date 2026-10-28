# Semaphores — Counting Synchronization in C++20/23

A semaphore maintains a count. `acquire()` decrements (blocks at 0), `release()` increments.

---

## Two types

```cpp
#include <semaphore>   // C++20

std::counting_semaphore<N> sem(initialCount);   // 0 to N
std::binary_semaphore      bsem(0);             // 0 or 1
```

---

## Core operations

```cpp
sem.acquire();                     // block if count == 0
sem.release();                     // increment, wake waiter
sem.release(n);                    // increment by n
bool ok = sem.try_acquire();       // non-blocking
bool ok = sem.try_acquire_for(dur);
bool ok = sem.try_acquire_until(tp);
```

---

## Limit concurrent access

```cpp
std::counting_semaphore<3> sem(3);   // max 3 at once

void worker() {
    sem.acquire();   // enter — blocks if 3 already inside
    doWork();
    sem.release();   // leave
}
```

---

## Binary semaphore as signal

```cpp
std::binary_semaphore signal(0);

void worker() { signal.acquire(); doWork(); }   // waits
void main()   { signal.release(); }             // sends signal
```

Unlike `mutex`, can be released by a **different thread** than acquired.

---

## Connection pool

```cpp
std::counting_semaphore<3> pool(3);

void query() {
    pool.acquire();   // get connection
    runQuery();
    pool.release();   // return connection
}
```

---

## Producer/Consumer

```cpp
std::counting_semaphore<MAX> itemsAvail(0);
std::counting_semaphore<MAX> spaceAvail(MAX);

void producer() { spaceAvail.acquire(); addItem(); itemsAvail.release(); }
void consumer() { itemsAvail.acquire(); takeItem(); spaceAvail.release(); }
```

---

## Semaphore vs Mutex

| | `mutex` | `semaphore` |
|--|---------|-------------|
| Count | 0 or 1 | 0 to N |
| Release by different thread | ❌ | ✅ |
| Ownership | ✅ | ❌ |
| Use for | Mutual exclusion | Counting / signaling |

---

## When to use

| Scenario | Tool |
|----------|------|
| Protect shared data | `mutex` |
| Signal between threads | `binary_semaphore` |
| Limit N concurrent | `counting_semaphore` |
| Producer/consumer buffer | Two `counting_semaphore` |

## What was tested

- Compiled with GCC 13.3 (`-std=c++23`) using a stand-in `<print>` header, because `std::println` needs GCC 14 and it was not available here. The source files were compiled directly, not through CMake.
- Run and compared with the original `std::cout` output line by line. Differences: `bool` values print as `true`/`false` instead of `1`/`0`, floating-point numbers print in their shortest exact form instead of 6 significant digits, and values that change on every run (thread ids, timings, random data). I sampled the differing lines; I did not review every one.
- Run once with AddressSanitizer + UndefinedBehaviorSanitizer: no findings.

## Not tested

- The real `<print>` header (GCC 14+, recent Clang or MSVC) and platforms other than Linux x86-64.
