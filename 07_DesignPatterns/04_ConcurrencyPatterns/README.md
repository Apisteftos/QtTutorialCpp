# 04_ConcurrencyPatterns

Design patterns for programs with several threads. The Gang of Four patterns in the other three folders are about objects; these are about how threads share data, wait for each other and hand over work.

Each folder has `main.cpp`, `Readme.md` (intent, roles, class diagram, sequence diagram, pitfalls) and `CMakeLists.txt`. All examples are written from scratch for this repository and use only the standard library.

| # | Pattern | In one sentence | Main tools |
|---|---|---|---|
| 01 | Producer-Consumer | a queue decouples the thread that creates work from the one that handles it | `mutex`, `condition_variable` |
| 02 | Thread Pool | a fixed set of workers runs many small tasks | `jthread`, `packaged_task`, `future` |
| 03 | Future / Promise | one thread delivers a result (or an exception), another waits for it | `promise`, `future`, `async` |
| 04 | Reader-Writer Lock | many readers at once, or one writer | `shared_mutex`, `shared_lock` |
| 05 | Thread-Specific Storage | every thread has its own copy, no lock needed | `thread_local` |
| 06 | Barrier and Latch | threads wait for each other at a meeting point | `std::latch`, `std::barrier` |
| 07 | Active Object | an object with its own thread; calls become queued requests | queue, `future` |
| 08 | Monitor Object | the object holds its own lock and does its own waiting | `mutex`, `condition_variable` |
| 09 | Pipeline | stages in separate threads connected by queues | channel (queue), `jthread` |
| 10 | Fork-Join | split, run the parts in parallel, combine the results | `std::async`, `span` |
| 11 | Scoped Locking | a lock is tied to a scope (RAII) | `lock_guard`, `scoped_lock` |
| 12 | Thread-Safe Interface | public methods lock once, private methods assume the lock | `mutex` |
| 13 | Immutable Object | data that never changes needs no lock | `const`, `atomic<shared_ptr>` |
| 14 | Guarded Suspension and Balking | wait until the state is right, or refuse at once | `condition_variable` |

## Relation to `06_ModernCpp/05_Multithreading`

`05_Multithreading` is organized by **tool** (mutex, condition variable, futures, atomics). This folder is organized by **pattern**. Several patterns are built from the same tools, and the patterns here are written as separate small examples, not copies.

| Pattern | Related tool folder |
|---|---|
| Producer-Consumer | `07_ConditionVariable`, `10_Semaphores` |
| Thread Pool | `11_ThreadPool` |
| Future / Promise | `08_Futures`, `06_Async` |
| Reader-Writer Lock | `05_Mutex`, `12_ThreadSynchronization` |
| Thread-Specific Storage | `03_ThreadAttributes` |
| Barrier and Latch | `12_ThreadSynchronization` |
| Active Object | `17_Worker` |
| Monitor Object, Guarded Suspension | `07_ConditionVariable` |
| Scoped Locking | `05_Mutex`, `06_DeadLock` |

## Choosing a pattern

| You want to... | Use |
|---|---|
| hand work from one thread to another | Producer-Consumer, Pipeline |
| run many short tasks on a limited number of threads | Thread Pool |
| get a result back from another thread | Future / Promise |
| share read-mostly data | Reader-Writer Lock, Immutable Object |
| avoid sharing at all | Thread-Specific Storage, Immutable Object, Active Object |
| make several threads advance in steps | Barrier and Latch |
| speed up a big calculation | Fork-Join |
| protect one class | Monitor Object, Thread-Safe Interface, Scoped Locking |
| deal with a call at the wrong moment | Guarded Suspension, Balking |

## Not covered

- **Reactor and Proactor:** event demultiplexing and asynchronous I/O (needs `epoll` or `io_uring`).
- **Actor Model:** objects that communicate only by messages.
- **Work Stealing:** idle workers take tasks from the queues of busy workers.
- **Half-Sync/Half-Async and Leader/Followers:** server designs that combine the above.
- **Double-Checked Locking:** see the Singleton examples in `02_CreationalPatterns/01_Singleton`.
- **Read-Copy-Update and lock-free structures:** see `DataStructuresCpp`, `07_Concurrent` for a lock-free ring buffer.

## Build and test

Every example builds on its own:

```bash
cd 01_ProducerConsumer
g++ -std=c++23 -Wall -Wextra -pthread main.cpp -o example && ./example
```

To check for data races, build with ThreadSanitizer:

```bash
g++ -std=c++23 -Wall -Wextra -pthread -fsanitize=thread -g main.cpp -o example && ./example
```

All 14 examples were built with GCC 15.2.0 without warnings. ThreadSanitizer, AddressSanitizer and UBSan report nothing, and the output is the same in repeated runs.
