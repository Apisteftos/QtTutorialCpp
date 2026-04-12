# Atomics — Lock-Free Synchronization in C++23

`std::atomic<T>` provides thread-safe operations on a single value **without a mutex**.
Operations are guaranteed to be indivisible — no thread can observe a partial state.

---

## Why atomics?

```cpp
int counter = 0;
counter++;   // READ → ADD → WRITE — three separate instructions
             // Two threads can interleave → wrong result
```

```cpp
std::atomic<int> counter{0};
counter++;   // single indivisible instruction — always correct
```

---

## Basic operations

```cpp
std::atomic<int> val{10};

val.load()         // read value
val.store(20)      // write value
val.exchange(99)   // write and return old value

++val              // atomic pre-increment
--val              // atomic pre-decrement
val += 5           // atomic add-assign
val -= 3           // atomic sub-assign

val.is_lock_free() // true if implemented without a mutex (usually true for int)
```

---

## std::atomic\<bool\> — flag patterns

### Ready flag
```cpp
std::atomic<bool> ready{false};

// Worker waits
while (!ready.load()) std::this_thread::yield();

// Main signals
ready.store(true);
```

### Stop flag
```cpp
std::atomic<bool> stop{false};

void worker() {
    while (!stop.load(std::memory_order_relaxed))
        doWork();
}

// From main:
stop = true;   // worker exits on next check
```

---

## std::atomic\<T*\> — atomic pointer

```cpp
int arr[3] = {10, 20, 30};
std::atomic<int*> ptr{arr};

ptr.store(&arr[1]);     // point to element 1
ptr.fetch_add(1);       // advance by sizeof(int)
int val = *ptr.load();
```

---

## fetch operations (return old value)

```cpp
std::atomic<int> n{100};

int old = n.fetch_add(10);   // n = 110, old = 100
int old = n.fetch_sub(5);    // n = 105, old = 110

// Bitwise
n.fetch_and(0b0101);   // n &= 0b0101
n.fetch_or(0b1010);    // n |= 0b1010
n.fetch_xor(0b1111);   // n ^= 0b1111
```

All fetch operations return the **previous** value atomically.

---

## Compare-and-Swap (CAS)

The foundation of all lock-free algorithms:

```cpp
std::atomic<int> val{0};
int expected = 0;

// If val == expected: set val = desired, return true
// If val != expected: set expected = val, return false
bool ok = val.compare_exchange_strong(expected, 42);
```

### compare_exchange_strong vs weak

```cpp
// strong — guaranteed to succeed if expected matches
val.compare_exchange_strong(expected, desired);

// weak — may spuriously fail even when expected matches
//        use in loops — can be faster on some architectures
while (!val.compare_exchange_weak(expected, desired)) {}
```

### Lock-free stack with CAS

```cpp
struct Node { int value; Node* next; };
std::atomic<Node*> top{nullptr};

void push(int val) {
    Node* n = new Node{val, top.load()};
    while (!top.compare_exchange_weak(n->next, n)) {}
}
```

---

## Memory ordering

Controls how atomic operations synchronize with non-atomic memory:

| Order | Description |
|-------|-------------|
| `memory_order_relaxed` | No sync — just atomicity. Fastest. Use for counters. |
| `memory_order_acquire` | All writes before a `release` are visible after this `acquire` |
| `memory_order_release` | Pairs with `acquire` — publishes all prior writes |
| `memory_order_acq_rel` | Both acquire and release (for read-modify-write) |
| `memory_order_seq_cst` | Total order across all threads. Safest. **Default.** |

### Producer-consumer with acquire/release

```cpp
std::atomic<int>  data{0};
std::atomic<bool> ready{false};

// Producer
data.store(42, std::memory_order_relaxed);        // write data
ready.store(true, std::memory_order_release);     // publish — ensures data is visible

// Consumer
while (!ready.load(std::memory_order_acquire)) {} // wait — sees producer's stores
int val = data.load(std::memory_order_relaxed);   // val == 42 guaranteed
```

**Rule:** use `seq_cst` (default) unless you have a measured performance problem
and understand the memory model deeply.

---

## std::atomic_flag — guaranteed lock-free boolean

`atomic_flag` is the only type guaranteed lock-free on every platform:

```cpp
std::atomic_flag flag = ATOMIC_FLAG_INIT;

flag.test_and_set()   // sets flag, returns old value (true if was already set)
flag.clear()          // clears the flag
flag.test()           // C++20: read without setting
```

---

## Spinlock using atomic_flag

```cpp
class Spinlock {
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
public:
    void lock() {
        while (flag.test_and_set(std::memory_order_acquire))
            std::this_thread::yield();   // spin
    }
    void unlock() {
        flag.clear(std::memory_order_release);
    }
};
```

**Use spinlock only for very short critical sections** — spinning wastes CPU.
For longer sections, use `std::mutex` (puts thread to sleep).

---

## Atomic vs Mutex — when to use which

| | `std::atomic` | `std::mutex` |
|--|--------------|-------------|
| Use for | Single variable | Multiple variables or complex logic |
| Performance | Very fast (hardware instruction) | Slower (OS kernel involvement) |
| Composable | Single op only | Any block of code |
| Types | int, bool, pointer, trivially copyable | Any type |

### Performance difference

Typical speedup of atomic over mutex for a simple counter increment: **5-20x**
depending on contention level and hardware.

---

## std::atomic\<shared_ptr\> (C++20)

```cpp
#include <memory>
std::atomic<std::shared_ptr<MyData>> g_data;

// Thread-safe read
auto local = g_data.load();

// Thread-safe update
g_data.store(std::make_shared<MyData>(newValue));

// Thread-safe compare-and-swap
auto expected = g_data.load();
g_data.compare_exchange_strong(expected, newData);
```

Replaces the old `std::atomic_load` / `std::atomic_store` free functions.

---

## Summary

```cpp
// Counter — always use atomic
std::atomic<int> counter{0};
counter++;
counter.fetch_add(1);

// Flag — atomic<bool> or atomic_flag
std::atomic<bool> stop{false};
stop.store(true);
while (!stop.load()) {}

// CAS — compare-and-swap for lock-free algorithms
int exp = 0;
if (val.compare_exchange_strong(exp, newVal)) { /* success */ }

// Memory order — default seq_cst is safe
val.store(42);                                    // seq_cst
val.store(42, std::memory_order_release);         // explicit
val.load(std::memory_order_acquire);              // explicit
val.fetch_add(1, std::memory_order_relaxed);      // counter — relaxed is fine
```
