# Timer & Chrono — std::chrono in C++23

`std::chrono` is the C++ standard library for time measurement, duration arithmetic,
and clock access. It provides type-safe time units — you cannot accidentally mix
seconds and milliseconds without an explicit cast.

---

## The three clocks

| Clock | Steady | Use for |
|-------|--------|---------|
| `high_resolution_clock` | Usually yes | Benchmarking, short duration measurement |
| `steady_clock` | ✅ Always | Timeouts, intervals — NEVER goes backwards |
| `system_clock` | ❌ No | Wall clock, timestamps, displaying date/time |

```cpp
auto t1 = std::chrono::high_resolution_clock::now();
auto t2 = std::chrono::steady_clock::now();
auto t3 = std::chrono::system_clock::now();
```

### Why steady_clock matters
`system_clock` can jump backwards due to NTP time corrections or DST changes.
If you use it to measure elapsed time and a correction happens mid-measurement,
you get a negative or incorrect result.

**Rule: always use `steady_clock` for measuring elapsed time and timeouts.**
Only use `system_clock` when you need to display or record real-world time.

---

## Duration types

```cpp
std::chrono::nanoseconds   ns(1000);   // 1000 ns
std::chrono::microseconds  us(1000);   // 1000 µs
std::chrono::milliseconds  ms(500);    // 500 ms
std::chrono::seconds       s(60);      // 60 s
std::chrono::minutes       min(60);    // 60 min
std::chrono::hours         h(24);      // 24 h
```

Each type stores an integer count of its unit. Access with `.count()`:
```cpp
std::chrono::seconds s(90);
s.count();   // 90
```

---

## Duration literals (C++14)

```cpp
using namespace std::chrono_literals;

auto a = 500ns;    // nanoseconds
auto b = 1500us;   // microseconds
auto c = 250ms;    // milliseconds
auto d = 3s;       // seconds
auto e = 2min;     // minutes
auto f = 1h;       // hours
```

Literals make code much more readable:
```cpp
std::this_thread::sleep_for(250ms);        // clear
std::this_thread::sleep_for(std::chrono::milliseconds(250));  // verbose
```

---

## Duration arithmetic

```cpp
auto total = 1h + 30min + 45s;   // mixed units work automatically

// Duration cast — truncates (does not round)
auto inSec = std::chrono::duration_cast<std::chrono::seconds>(total);

// Floating-point duration — no truncation
std::chrono::duration<double> exact(3723ms);  // 3.723 seconds
```

Duration comparisons work across units:
```cpp
500ms > 0.4s    // true
1min == 60s     // true
1h   == 3600s   // true
```

---

## Time points

A time point is a specific moment on a clock's timeline:

```cpp
auto now   = std::chrono::steady_clock::now();
auto later = now + 5s;               // 5 seconds in the future
auto diff  = later - now;            // duration between two points

// Time since clock epoch
auto epoch = std::chrono::system_clock::now().time_since_epoch();
auto sec   = std::chrono::duration_cast<std::chrono::seconds>(epoch);
```

---

## Measuring elapsed time

```cpp
auto start = std::chrono::high_resolution_clock::now();

// ... work ...

auto end     = std::chrono::high_resolution_clock::now();
auto elapsed = end - start;

// Convert to desired unit
auto ms  = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
auto us  = std::chrono::duration_cast<std::chrono::microseconds>(elapsed);

// Or as floating-point
double msDouble = std::chrono::duration<double, std::milli>(elapsed).count();
```

---

## Timer class

Reusable helper that wraps the boilerplate:

```cpp
class Timer {
public:
    void   start();
    void   stop();
    double nanoseconds()  const;
    double microseconds() const;
    double milliseconds() const;
    double seconds()      const;
    void   print()        const;   // auto-selects best unit
};

Timer t("my task");
t.start();
doWork();
t.stop();
t.print();   // "[my task] 12.3 ms"
```

The timer can be queried **while still running** — useful for progress reporting.

---

## ScopedTimer — RAII automatic timing

Prints elapsed time automatically when the scope ends:

```cpp
{
    ScopedTimer st("sorting");
    std::sort(v.begin(), v.end());
}   // prints: "[ScopedTimer] 'sorting' took 45.2 ms"
```

Works correctly even if an exception is thrown — the destructor always runs.

---

## system_clock — current date and time

```cpp
auto now    = std::chrono::system_clock::now();
auto timeT  = std::chrono::system_clock::to_time_t(now);
auto* tmPtr = std::localtime(&timeT);

// Format with strftime-style format strings
std::cout << std::put_time(tmPtr, "%Y-%m-%d %H:%M:%S");   // 2026-04-10 14:30:00
std::cout << std::put_time(tmPtr, "%A");                    // Thursday
std::cout << std::put_time(tmPtr, "%Y-%m-%dT%H:%M:%S");   // ISO 8601
```

Common format specifiers:

| Specifier | Output |
|-----------|--------|
| `%Y` | 4-digit year |
| `%m` | Month (01-12) |
| `%d` | Day (01-31) |
| `%H` | Hour 24h (00-23) |
| `%M` | Minute (00-59) |
| `%S` | Second (00-60) |
| `%A` | Full weekday name |
| `%a` | Abbreviated weekday |

---

## steady_clock — timeout pattern

```cpp
auto deadline = std::chrono::steady_clock::now() + 200ms;

while (std::chrono::steady_clock::now() < deadline) {
    // do work until timeout
    std::this_thread::yield();
}

// Check if an operation exceeded a threshold
auto start = std::chrono::steady_clock::now();
doSomething();
bool tooSlow = (std::chrono::steady_clock::now() - start) > 100ms;
```

---

## sleep_for and sleep_until

```cpp
// sleep_for — relative: sleep for this much time
std::this_thread::sleep_for(500ms);
std::this_thread::sleep_for(std::chrono::seconds(2));

// sleep_until — absolute: sleep until this time point
auto wakeTime = std::chrono::steady_clock::now() + 300ms;
std::this_thread::sleep_until(wakeTime);
```

**Sleep is a minimum guarantee** — the OS may wake the thread slightly later.
Typical OS scheduler resolution is 1-15ms on desktop Linux.
For sub-millisecond precision use `clock_nanosleep` (POSIX) or busy-wait.

---

## Benchmarking pattern

```cpp
auto bench = [](auto&& func, const std::string& name, int runs = 5) {
    std::vector<double> times;
    for (int i = 0; i < runs; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        func();
        auto end = std::chrono::high_resolution_clock::now();
        times.push_back(
            std::chrono::duration<double, std::milli>(end - start).count()
        );
    }
    std::sort(times.begin(), times.end());
    std::cout << name << ": min=" << times.front()
              << "ms avg=" << (sum/runs) << "ms\n";
};

bench([]() { std::sort(v.begin(), v.end()); }, "sort 100K", 5);
```

Always run multiple times and report **min, avg, median** — single measurements
are unreliable due to OS scheduling jitter and CPU cache effects.

---

## C++23 chrono formatting with std::print

```cpp
#include <print>
#include <chrono>

auto now = std::chrono::system_clock::now();

std::println("Duration: {}", 1500ms);        // Duration: 1500ms
std::println("Duration: {}", 3s);            // Duration: 3s
std::println("Duration: {}", 2h + 30min);    // Duration: 9000s
```

---

## Clock selection guide

```
Need current date/time for display?     → system_clock
Need to measure elapsed time?           → steady_clock or high_resolution_clock
Need a timeout?                         → steady_clock
Need maximum precision for benchmarks?  → high_resolution_clock
Need Unix timestamp?                    → system_clock::now().time_since_epoch()
```

---

## Summary table

| Task | Code |
|------|------|
| Measure elapsed ms | `duration_cast<milliseconds>(end - start).count()` |
| Current date/time | `system_clock::now()` + `to_time_t()` + `localtime()` |
| Sleep 500ms | `sleep_for(500ms)` |
| Sleep until time | `sleep_until(steady_clock::now() + 200ms)` |
| Timeout check | `steady_clock::now() > deadline` |
| Convert 3s to ms | `duration_cast<milliseconds>(3s).count()` → `3000` |
| Unix timestamp | `system_clock::now().time_since_epoch()` |
