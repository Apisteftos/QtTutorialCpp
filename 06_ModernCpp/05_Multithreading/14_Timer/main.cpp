#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <iomanip>    // std::put_time
#include <ctime>      // std::time_t, std::tm
#include <sstream>    // std::ostringstream
#include <vector>
#include <algorithm>  // std::sort
#include <print>      // C++23

using namespace std::chrono_literals;

// ============================================================
// TIMER & CHRONO in C++23
// ============================================================
// Topics covered:
//   1.  Clocks — high_resolution, steady, system
//   2.  Duration types and literals
//   3.  Duration arithmetic and casting
//   4.  Time points
//   5.  Timer class (measure elapsed time)
//   6.  system_clock — wall clock, current date/time
//   7.  steady_clock — monotonic clock, timeouts
//   8.  high_resolution_clock — benchmarking
//   9.  sleep_for and sleep_until
//  10.  Benchmarking helper
//  11.  Timeout pattern
//  12.  C++23 chrono formatting with std::print
// ============================================================


// ============================================================
// Reusable Timer class (upgraded from original)
// ============================================================
class Timer {
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_end;
    std::string m_name;
    bool        m_running;

public:
    explicit Timer(const std::string& name = "")
        : m_name(name), m_running(false) {}

    void start() {
        m_start   = std::chrono::high_resolution_clock::now();
        m_running = true;
    }

    void stop() {
        m_end     = std::chrono::high_resolution_clock::now();
        m_running = false;
    }

    void reset() {
        m_running = false;
    }

    // Elapsed time — works while running OR after stop()
    double nanoseconds() const {
        auto end = m_running
                       ? std::chrono::high_resolution_clock::now() : m_end;
        return std::chrono::duration<double, std::nano>(end - m_start).count();
    }

    double microseconds() const { return nanoseconds() / 1000.0; }
    double milliseconds() const { return nanoseconds() / 1e6;    }
    double seconds()      const { return nanoseconds() / 1e9;    }

    void print() const {
        double ns = nanoseconds();
        if      (ns < 1000.0)        std::println("[{}] {} ns", m_name, ns);
        else if (ns < 1'000'000.0)   std::println("[{}] {} µs", m_name, ns/1000.0);
        else if (ns < 1'000'000'000) std::println("[{}] {} ms", m_name, ns/1e6);
        else                         std::println("[{}] {} s", m_name, ns/1e9);
    }

    bool isRunning() const { return m_running; }
};


// ============================================================
// RAII ScopedTimer — prints elapsed on destruction
// ============================================================
class ScopedTimer {
private:
    Timer       m_timer;
    std::string m_label;

public:
    explicit ScopedTimer(const std::string& label) : m_label(label) {
        m_timer.start();
    }

    ~ScopedTimer() {
        m_timer.stop();
        std::println("[ScopedTimer] '{}' took {} ms", m_label, m_timer.milliseconds());
    }
};


// ============================================================
// Benchmarking helper — runs callable N times, reports stats
// ============================================================
template<typename Func>
void benchmark(const std::string& name, Func&& func, int runs = 5)
{
    std::vector<double> times;
    times.reserve(runs);

    for (int i = 0; i < runs; ++i) {
        Timer t;
        t.start();
        func();
        t.stop();
        times.push_back(t.milliseconds());
    }

    std::sort(times.begin(), times.end());
    double total = 0;
    for (double d : times) total += d;
    double avg = total / runs;

    std::println("[benchmark] {}\n  runs:   {}\n  min:    {} ms\n  max:    {} ms\n  avg:    {} ms\n  median: {} ms",
        name, runs, times.front(), times.back(), avg, times[runs/2]);
}


int main()
{
    // -------------------------------------------------------
    std::println("=====================================================");
    std::println("Example 1: The Three Clocks");
    std::println("=====================================================");

    // --- high_resolution_clock ---
    // Most precise clock available on the platform
    // Use for: benchmarking, measuring short durations
    // Note: may not be steady (can go backwards on some platforms)
    auto hrNow = std::chrono::high_resolution_clock::now();
    std::println("high_resolution_clock tick period: {}/{} s",
        std::chrono::high_resolution_clock::period::num, std::chrono::high_resolution_clock::period::den);
    std::println("is_steady: {}", (std::chrono::high_resolution_clock::is_steady ? "YES" : "NO"));

    // --- steady_clock ---
    // Monotonic — NEVER goes backwards
    // Use for: timeouts, measuring intervals, waiting
    auto stNow = std::chrono::steady_clock::now();
    std::println("");
    std::println("steady_clock is_steady: {}", (std::chrono::steady_clock::is_steady ? "YES" : "NO"));
    std::println("Use steady_clock for timeouts and intervals");

    // --- system_clock ---
    // Wall clock — represents real-world time
    // Can go backwards (NTP adjustments, DST changes)
    // Use for: timestamps, displaying current date/time
    auto syNow = std::chrono::system_clock::now();
    std::println("");
    std::println("system_clock is_steady: {}", (std::chrono::system_clock::is_steady ? "YES" : "NO"));
    std::println("Use system_clock for timestamps and real-world time");


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 2: Duration Types and Literals");
    std::println("=====================================================");

    // Duration types
    std::chrono::nanoseconds  ns(1000);
    std::chrono::microseconds us(1000);
    std::chrono::milliseconds ms(1000);
    std::chrono::seconds      s(60);
    std::chrono::minutes      min(60);
    std::chrono::hours        h(24);

    std::println("nanoseconds(1000):  {} ns", ns.count());
    std::println("microseconds(1000): {} µs", us.count());
    std::println("milliseconds(1000): {} ms", ms.count());
    std::println("seconds(60):        {} s", s.count());
    std::println("minutes(60):        {} min", min.count());
    std::println("hours(24):          {} h", h.count());

    // C++14 duration literals (using namespace std::chrono_literals)
    std::println("");
    std::println("Duration literals:");
    auto d1 = 500ns;
    auto d2 = 1500us;
    auto d3 = 250ms;
    auto d4 = 3s;
    auto d5 = 2min;
    auto d6 = 1h;

    std::println("500ns  = {} ns", d1.count());
    std::println("1500us = {} µs", d2.count());
    std::println("250ms  = {} ms", d3.count());
    std::println("3s     = {} s", d4.count());
    std::println("2min   = {} min", d5.count());
    std::println("1h     = {} h", d6.count());


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 3: Duration Arithmetic and Casting");
    std::println("=====================================================");

    auto total = 1h + 30min + 45s;
    std::println("1h + 30min + 45s = {} seconds total",
        std::chrono::duration_cast<std::chrono::seconds>(total).count());

    // Cast between units — duration_cast truncates (no rounding)
    std::chrono::milliseconds bigMs(3723456);
    auto inSeconds = std::chrono::duration_cast<std::chrono::seconds>(bigMs);
    auto inMinutes = std::chrono::duration_cast<std::chrono::minutes>(bigMs);
    auto inHours   = std::chrono::duration_cast<std::chrono::hours>(bigMs);

    std::println("");
    std::println("3723456 ms =");
    std::println("  {} seconds (truncated)", inSeconds.count());
    std::println("  {} minutes (truncated)", inMinutes.count());
    std::println("  {} hours   (truncated)", inHours.count());

    // Floating point duration — no truncation
    std::chrono::duration<double> exactSeconds(bigMs);
    std::println("  {} seconds (exact)", exactSeconds.count());

    // Comparison
    std::println("");
    std::println("500ms > 0.4s? {}", (500ms > 0.4s ? "YES" : "NO"));
    std::println("1min == 60s?  {}", (1min == 60s ? "YES" : "NO"));
    std::println("1h == 3600s?  {}", (1h == 3600s ? "YES" : "NO"));


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 4: Time Points");
    std::println("=====================================================");

    auto now  = std::chrono::steady_clock::now();
    auto later = now + 5s;

    std::println("Time point arithmetic:");
    std::println("later - now = {} seconds",
        std::chrono::duration_cast<std::chrono::seconds>(later - now).count());

    // Epoch — time since clock's starting point
    auto sysNow  = std::chrono::system_clock::now();
    auto sinceEpoch = sysNow.time_since_epoch();
    auto epochSec   = std::chrono::duration_cast<std::chrono::seconds>(sinceEpoch);
    std::println("Seconds since Unix epoch: {}", epochSec.count());


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 5: Timer Class — Measuring Elapsed Time");
    std::println("=====================================================");

    Timer t("computation");
    t.start();

    // Simulate work
    long long sum = 0;
    for (int i = 0; i < 100'000'000; ++i)
        sum += i;

    t.stop();
    std::println("Sum: {}", sum);
    std::println("Elapsed: {} ms", t.milliseconds());
    std::println("Elapsed: {} µs", t.microseconds());
    t.print();   // auto-selects best unit


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 6: ScopedTimer — RAII automatic timing");
    std::println("=====================================================");

    {
        ScopedTimer st("sorting 1M elements");
        std::vector<int> v(1'000'000);
        for (int i = 0; i < (int)v.size(); ++i) v[i] = (int)v.size() - i;
        std::sort(v.begin(), v.end());
    }   // prints elapsed automatically on scope exit


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 7: system_clock — Current Date and Time");
    std::println("=====================================================");

    auto systemNow = std::chrono::system_clock::now();

    // Convert to time_t for formatting with C API
    std::time_t timeT = std::chrono::system_clock::to_time_t(systemNow);
    std::tm*    tmPtr = std::localtime(&timeT);

    auto fmtTime = [tmPtr](const char* fmt) {
        char buf[64];
        std::strftime(buf, sizeof buf, fmt, tmPtr);
        return std::string(buf);
    };
    std::println("Current date/time: {}", fmtTime("%Y-%m-%d %H:%M:%S"));
    std::println("Day of week:       {}", fmtTime("%A"));
    std::println("ISO 8601:          {}", fmtTime("%Y-%m-%dT%H:%M:%S"));

    // Time since epoch
    auto epochMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        systemNow.time_since_epoch());
    std::println("Unix timestamp ms: {}", epochMs.count());


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 8: steady_clock — Timeout Pattern");
    std::println("=====================================================");

    auto deadline = std::chrono::steady_clock::now() + 200ms;

    int iterations = 0;
    while (std::chrono::steady_clock::now() < deadline) {
        ++iterations;
        std::this_thread::yield();
    }
    std::println("Ran {} iterations in 200ms timeout", iterations);

    // try_lock_for style timeout
    auto startTime = std::chrono::steady_clock::now();
    std::this_thread::sleep_for(150ms);
    auto elapsed = std::chrono::steady_clock::now() - startTime;
    bool timedOut = elapsed > 100ms;
    std::println("Timed out (>100ms)? {}", (timedOut ? "YES" : "NO"));


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 9: sleep_for and sleep_until");
    std::println("=====================================================");

    // sleep_for — relative duration
    std::println("Sleeping 100ms with sleep_for...");
    Timer sleepTimer("sleep_for 100ms");
    sleepTimer.start();
    std::this_thread::sleep_for(100ms);
    sleepTimer.stop();
    sleepTimer.print();

    // sleep_until — absolute time point
    std::println("Sleeping until now+200ms with sleep_until...");
    Timer sleepTimer2("sleep_until +200ms");
    sleepTimer2.start();
    auto wakeTime = std::chrono::steady_clock::now() + 200ms;
    std::this_thread::sleep_until(wakeTime);
    sleepTimer2.stop();
    sleepTimer2.print();

    std::println("Note: actual sleep may be slightly longer than requested");
    std::println("      OS scheduler resolution is typically 1-15ms");


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 10: Benchmarking Helper");
    std::println("=====================================================");

    // Benchmark a computation over multiple runs
    benchmark("summing 10M integers", []() {
        long long s = 0;
        for (int i = 0; i < 10'000'000; ++i) s += i;
        (void)s;
    }, 5);

    std::println("");

    benchmark("sorting 100K integers", []() {
        std::vector<int> v(100'000);
        for (int i = 0; i < (int)v.size(); ++i) v[i] = (int)v.size() - i;
        std::sort(v.begin(), v.end());
    }, 3);


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 11: Clock Comparison");
    std::println("=====================================================");

    // Measure the same sleep with all three clocks
    auto hrStart = std::chrono::high_resolution_clock::now();
    auto stStart = std::chrono::steady_clock::now();
    auto syStart = std::chrono::system_clock::now();

    std::this_thread::sleep_for(50ms);

    auto hrElapsed = std::chrono::high_resolution_clock::now() - hrStart;
    auto stElapsed = std::chrono::steady_clock::now()          - stStart;
    auto syElapsed = std::chrono::system_clock::now()          - syStart;

    std::println("Same 50ms sleep measured by:");
    std::println("  high_resolution_clock: {} µs",
        std::chrono::duration_cast<std::chrono::microseconds>(hrElapsed).count());
    std::println("  steady_clock:          {} µs",
        std::chrono::duration_cast<std::chrono::microseconds>(stElapsed).count());
    std::println("  system_clock:          {} µs",
        std::chrono::duration_cast<std::chrono::microseconds>(syElapsed).count());


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 12: C++23 chrono formatting with std::print");
    std::println("=====================================================");

    auto now23 = std::chrono::system_clock::now();

    // C++23 — format durations directly
    std::println("Current time (raw):    {}", now23.time_since_epoch());
    std::println("Duration 1500ms:       {}", 1500ms);
    std::println("Duration 3s:           {}", 3s);
    std::println("Duration 2h 30min:     {}", 2h + 30min);

    std::println("");
    std::println("=== Timer & Chrono complete ===");
    return 0;
}
