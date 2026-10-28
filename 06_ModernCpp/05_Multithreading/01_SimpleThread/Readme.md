# SimpleThread — `std::thread` fundamentals (C++23)

Ten short examples that introduce `std::thread`: creating threads, passing arguments, join/detach, thread ids, and what each thread owns.

## What each example shows

| # | Example | Key point |
|---|---------|-----------|
| 1 | Free function | `std::thread t(func);` starts the thread immediately; there is no `start()`. `join()` waits for it. |
| 2 | Parameters | Arguments are written after the callable and are **copied** into the thread. Use `std::ref` to pass a reference. |
| 3 | Lambdas | The usual way to create a thread; capture by value is safe. |
| 4 | Concurrency | Two workers with different sleeps; their output interleaves. The order is not guaranteed. |
| 5 | Own stack | Each thread has private local variables and therefore its own stack address. |
| 6 | Thread ID | `std::this_thread::get_id()` inside, `t.get_id()` from outside. |
| 7 | `joinable()` | True before `join()`, false after, and false for a default-constructed `std::thread`. |
| 8 | `detach()` | The thread runs on its own; after `detach()` the object is no longer joinable. |
| 9 | No join/detach | Not executed (see below). |
| 10 | `hardware_concurrency()` | Number of logical cores, a hint for CPU-bound work. |

## Notes moved from main.cpp

**What is a thread?** An independent sequence of execution inside a process.

- All threads share: heap memory, global variables, code.
- Each thread has its own: stack, program counter, CPU registers.

**Every thread must be joined or detached.** If a joinable `std::thread` is destroyed, the program calls `std::terminate()`:

```cpp
{
    std::thread bad([]() { /* ... */ });
}   // bad is still joinable -> std::terminate()
```

The example in `main.cpp` is commented out so the program does not crash. I ran this exact snippet separately: it aborts with `terminate called without an active exception`.

**Detached threads and `main`.** The program sleeps 200 ms so the detached threads can finish before `main` returns. That is only acceptable in a demo; real code should use a synchronisation mechanism (a future, a latch, a condition variable). If `main` returns first, detached threads are killed in the middle of their work.

## Build and run

```bash
g++ -std=c++23 -Wall -Wextra -pthread main.cpp -o simple_thread
./simple_thread
```

Needs a standard library with `<print>` (GCC 14+). Example 6 prints `std::thread::id` with `std::println`, which needs library support for formatting thread ids (C++23, P2693). As far as I know GCC 14 has it; this was not checked on a real GCC 14 here.

## What was tested

Compiled with GCC 13.3 and a small `<print>` stand-in header (GCC 13 has no `<print>`), with `-Wall -Wextra -fsanitize=address,undefined`:

- Builds without warnings and runs to exit code 0; no sanitizer reports.
- Example 4 interleaves A and B steps as described. The exact order differs between runs.
- Example 5 prints a different stack address per thread (the addresses change from run to run).
- The `std::terminate` behaviour of Example 9 was verified with a separate 3-line program.

## Not tested

- GCC 14 / real `std::println` and `std::formatter<std::thread::id>`.
- Windows / macOS.
- The CMake build.

## Changes made to `main.cpp`

- `std::cout` replaced by `std::println`.
- Stack addresses are printed through `static_cast<const void*>(...)` because `std::println` has no formatter for `int*`.


### Example 9: What happens WITHOUT join or detach

[main] (crash example skipped — see comments in code)  
[main] Rule: EVERY thread must be joined OR detached
