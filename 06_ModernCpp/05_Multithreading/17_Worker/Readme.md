# Worker thread — `17_Worker`

A worker is **one thread that waits for jobs**. It takes them from a queue one at a time and runs them. The calling code only puts jobs into the queue and can go on with its own work. Several workers sharing one queue make a thread pool (see `11_ThreadPool`).

## Sections in `main.cpp`

- EXAMPLE 1: Jobs run in order
- EXAMPLE 2: A job returns a value

## How it works

```
main thread ── submit(job) ──► [ queue ] ──► worker thread: wait, take the next job, run it
```

| Part | Role |
|---|---|
| `std::queue<std::move_only_function<void()>>` | the jobs that are waiting |
| `std::mutex` | protects the queue, because `submit()` (caller thread) and `run()` (worker thread) both touch it |
| `std::condition_variable_any` | lets the worker sleep while the queue is empty and wakes it when a job arrives |
| `std::jthread` + `std::stop_token` | the worker thread; leaving the scope of the `Worker` requests a stop and joins the thread |
| `std::packaged_task` / `std::future` | give the caller the result of a job |

Points to notice:

- The job runs **outside** the lock, so a long job never blocks `submit()`.
- `wakeup_.wait(lock, stop, predicate)` returns when there is a job **or** a stop was requested. Jobs that are already in the queue are still executed before the worker ends, so no job is lost at shutdown.
- `thread_` is the last member of the class. Members are destroyed in reverse order, so the thread is joined first, while the queue, mutex and condition variable still exist.
- Each job is run by exactly one thread, so jobs of one worker never run at the same time. Data that is only used inside the jobs of one worker needs no lock.

## Output

```text
=== Example 1: Jobs run in order ===
  job 1 done
  job 2 done
  job 3 done
worker finished

=== Example 2: A job returns a value ===
sum of 1..100 = 5050
text = hello from the worker
```

The output is the same on every run, because the program prints only from the jobs (in queue order) and after the worker has finished. If `main` printed while the worker prints, the order of those lines would not be fixed.

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`) and `std::move_only_function` (C++23): GCC 14+ or a recent Clang/MSVC.

```bash
g++ -std=c++23 -Wall -Wextra -pthread main.cpp -o worker
./worker
```

## When do you need a worker?

- Work that is slow (disk, network, calculation) and must not block the main thread or the user interface.
- Work that must run **one at a time and in order**, for example writing to a log file or a database, without every caller needing a lock.
- A background thread that lives for the whole program, instead of creating a new `std::thread` for every task.

For many jobs at the same time, use several workers with one shared queue (a thread pool).

In Qt the same idea is a worker object that is moved to a `QThread` with `moveToThread()`; it does the slow work there and reports back with signals. See `16_QThread`.

## What was tested

Checked with GCC 13.3 and a small `<print>` stand-in header (GCC 13 has no `<print>`), so `std::println` itself was not compiled with a real GCC 14/15 library:

- Builds with `-std=c++23 -Wall -Wextra -pthread` without warnings.
- Runs to completion (exit code 0) and prints 9 lines.
- Clean under AddressSanitizer + UBSan and under ThreadSanitizer (no data race reported).
- Run 300 times: same output every time, no hang.

## Not tested

- GCC 14/15 and the real `<print>`; MSVC; Apple Clang; Windows/macOS.
- The CMake build.
