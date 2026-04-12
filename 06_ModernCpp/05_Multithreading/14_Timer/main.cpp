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
        if      (ns < 1000.0)        std::cout << "[" << m_name << "] " << ns         << " ns\n";
        else if (ns < 1'000'000.0)   std::cout << "[" << m_name << "] " << ns/1000.0  << " µs\n";
        else if (ns < 1'000'000'000) std::cout << "[" << m_name << "] " << ns/1e6     << " ms\n";
        else                         std::cout << "[" << m_name << "] " << ns/1e9     << " s\n";
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
        std::cout << "[ScopedTimer] '" << m_label << "' took "
                  << m_timer.milliseconds() << " ms\n";
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

    std::cout << "[benchmark] " << name << "\n"
              << "  runs:   " << runs         << '\n'
              << "  min:    " << times.front() << " ms\n"
              << "  max:    " << times.back()  << " ms\n"
              << "  avg:    " << avg           << " ms\n"
              << "  median: " << times[runs/2] << " ms\n";
}


int main()
{
    // -------------------------------------------------------
    std::cout << "=====================================================\n";
    std::cout << "Example 1: The Three Clocks\n";
    std::cout << "=====================================================\n";

    // --- high_resolution_clock ---
    // Most precise clock available on the platform
    // Use for: benchmarking, measuring short durations
    // Note: may not be steady (can go backwards on some platforms)
    auto hrNow = std::chrono::high_resolution_clock::now();
    std::cout << "high_resolution_clock tick period: "
              << std::chrono::high_resolution_clock::period::num << "/"
              << std::chrono::high_resolution_clock::period::den << " s\n";
    std::cout << "is_steady: "
              << (std::chrono::high_resolution_clock::is_steady ? "YES" : "NO") << '\n';

    // --- steady_clock ---
    // Monotonic — NEVER goes backwards
    // Use for: timeouts, measuring intervals, waiting
    auto stNow = std::chrono::steady_clock::now();
    std::cout << "\nsteady_clock is_steady: "
              << (std::chrono::steady_clock::is_steady ? "YES" : "NO") << '\n';
    std::cout << "Use steady_clock for timeouts and intervals\n";

    // --- system_clock ---
    // Wall clock — represents real-world time
    // Can go backwards (NTP adjustments, DST changes)
    // Use for: timestamps, displaying current date/time
    auto syNow = std::chrono::system_clock::now();
    std::cout << "\nsystem_clock is_steady: "
              << (std::chrono::system_clock::is_steady ? "YES" : "NO") << '\n';
    std::cout << "Use system_clock for timestamps and real-world time\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: Duration Types and Literals\n";
    std::cout << "=====================================================\n";

    // Duration types
    std::chrono::nanoseconds  ns(1000);
    std::chrono::microseconds us(1000);
    std::chrono::milliseconds ms(1000);
    std::chrono::seconds      s(60);
    std::chrono::minutes      min(60);
    std::chrono::hours        h(24);

    std::cout << "nanoseconds(1000):  " << ns.count()  << " ns\n";
    std::cout << "microseconds(1000): " << us.count()  << " µs\n";
    std::cout << "milliseconds(1000): " << ms.count()  << " ms\n";
    std::cout << "seconds(60):        " << s.count()   << " s\n";
    std::cout << "minutes(60):        " << min.count() << " min\n";
    std::cout << "hours(24):          " << h.count()   << " h\n";

    // C++14 duration literals (using namespace std::chrono_literals)
    std::cout << "\nDuration literals:\n";
    auto d1 = 500ns;
    auto d2 = 1500us;
    auto d3 = 250ms;
    auto d4 = 3s;
    auto d5 = 2min;
    auto d6 = 1h;

    std::cout << "500ns  = " << d1.count() << " ns\n";
    std::cout << "1500us = " << d2.count() << " µs\n";
    std::cout << "250ms  = " << d3.count() << " ms\n";
    std::cout << "3s     = " << d4.count() << " s\n";
    std::cout << "2min   = " << d5.count() << " min\n";
    std::cout << "1h     = " << d6.count() << " h\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: Duration Arithmetic and Casting\n";
    std::cout << "=====================================================\n";

    auto total = 1h + 30min + 45s;
    std::cout << "1h + 30min + 45s = "
              << std::chrono::duration_cast<std::chrono::seconds>(total).count()
              << " seconds total\n";

    // Cast between units — duration_cast truncates (no rounding)
    std::chrono::milliseconds bigMs(3723456);
    auto inSeconds = std::chrono::duration_cast<std::chrono::seconds>(bigMs);
    auto inMinutes = std::chrono::duration_cast<std::chrono::minutes>(bigMs);
    auto inHours   = std::chrono::duration_cast<std::chrono::hours>(bigMs);

    std::cout << "\n3723456 ms =\n";
    std::cout << "  " << inSeconds.count() << " seconds (truncated)\n";
    std::cout << "  " << inMinutes.count() << " minutes (truncated)\n";
    std::cout << "  " << inHours.count()   << " hours   (truncated)\n";

    // Floating point duration — no truncation
    std::chrono::duration<double> exactSeconds(bigMs);
    std::cout << "  " << exactSeconds.count() << " seconds (exact)\n";

    // Comparison
    std::cout << "\n500ms > 0.4s? "  << (500ms > 0.4s  ? "YES" : "NO") << '\n';
    std::cout << "1min == 60s?  "   << (1min == 60s   ? "YES" : "NO") << '\n';
    std::cout << "1h == 3600s?  "   << (1h   == 3600s ? "YES" : "NO") << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: Time Points\n";
    std::cout << "=====================================================\n";

    auto now  = std::chrono::steady_clock::now();
    auto later = now + 5s;

    std::cout << "Time point arithmetic:\n";
    std::cout << "later - now = "
              << std::chrono::duration_cast<std::chrono::seconds>(later - now).count()
              << " seconds\n";

    // Epoch — time since clock's starting point
    auto sysNow  = std::chrono::system_clock::now();
    auto sinceEpoch = sysNow.time_since_epoch();
    auto epochSec   = std::chrono::duration_cast<std::chrono::seconds>(sinceEpoch);
    std::cout << "Seconds since Unix epoch: " << epochSec.count() << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: Timer Class — Measuring Elapsed Time\n";
    std::cout << "=====================================================\n";

    Timer t("computation");
    t.start();

    // Simulate work
    long long sum = 0;
    for (int i = 0; i < 100'000'000; ++i)
        sum += i;

    t.stop();
    std::cout << "Sum: " << sum << '\n';
    std::cout << "Elapsed: " << t.milliseconds() << " ms\n";
    std::cout << "Elapsed: " << t.microseconds() << " µs\n";
    t.print();   // auto-selects best unit


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: ScopedTimer — RAII automatic timing\n";
    std::cout << "=====================================================\n";

    {
        ScopedTimer st("sorting 1M elements");
        std::vector<int> v(1'000'000);
        for (int i = 0; i < (int)v.size(); ++i) v[i] = (int)v.size() - i;
        std::sort(v.begin(), v.end());
    }   // prints elapsed automatically on scope exit


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: system_clock — Current Date and Time\n";
    std::cout << "=====================================================\n";

    auto systemNow = std::chrono::system_clock::now();

    // Convert to time_t for formatting with C API
    std::time_t timeT = std::chrono::system_clock::to_time_t(systemNow);
    std::tm*    tmPtr = std::localtime(&timeT);

    std::cout << "Current date/time: " << std::put_time(tmPtr, "%Y-%m-%d %H:%M:%S") << '\n';
    std::cout << "Day of week:       " << std::put_time(tmPtr, "%A") << '\n';
    std::cout << "ISO 8601:          " << std::put_time(tmPtr, "%Y-%m-%dT%H:%M:%S") << '\n';

    // Time since epoch
    auto epochMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        systemNow.time_since_epoch());
    std::cout << "Unix timestamp ms: " << epochMs.count() << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: steady_clock — Timeout Pattern\n";
    std::cout << "=====================================================\n";

    auto deadline = std::chrono::steady_clock::now() + 200ms;

    int iterations = 0;
    while (std::chrono::steady_clock::now() < deadline) {
        ++iterations;
        std::this_thread::yield();
    }
    std::cout << "Ran " << iterations << " iterations in 200ms timeout\n";

    // try_lock_for style timeout
    auto startTime = std::chrono::steady_clock::now();
    std::this_thread::sleep_for(150ms);
    auto elapsed = std::chrono::steady_clock::now() - startTime;
    bool timedOut = elapsed > 100ms;
    std::cout << "Timed out (>100ms)? " << (timedOut ? "YES" : "NO") << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 9: sleep_for and sleep_until\n";
    std::cout << "=====================================================\n";

    // sleep_for — relative duration
    std::cout << "Sleeping 100ms with sleep_for...\n";
    Timer sleepTimer("sleep_for 100ms");
    sleepTimer.start();
    std::this_thread::sleep_for(100ms);
    sleepTimer.stop();
    sleepTimer.print();

    // sleep_until — absolute time point
    std::cout << "Sleeping until now+200ms with sleep_until...\n";
    Timer sleepTimer2("sleep_until +200ms");
    sleepTimer2.start();
    auto wakeTime = std::chrono::steady_clock::now() + 200ms;
    std::this_thread::sleep_until(wakeTime);
    sleepTimer2.stop();
    sleepTimer2.print();

    std::cout << "Note: actual sleep may be slightly longer than requested\n";
    std::cout << "      OS scheduler resolution is typically 1-15ms\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 10: Benchmarking Helper\n";
    std::cout << "=====================================================\n";

    // Benchmark a computation over multiple runs
    benchmark("summing 10M integers", []() {
        long long s = 0;
        for (int i = 0; i < 10'000'000; ++i) s += i;
        (void)s;
    }, 5);

    std::cout << '\n';

    benchmark("sorting 100K integers", []() {
        std::vector<int> v(100'000);
        for (int i = 0; i < (int)v.size(); ++i) v[i] = (int)v.size() - i;
        std::sort(v.begin(), v.end());
    }, 3);


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 11: Clock Comparison\n";
    std::cout << "=====================================================\n";

    // Measure the same sleep with all three clocks
    auto hrStart = std::chrono::high_resolution_clock::now();
    auto stStart = std::chrono::steady_clock::now();
    auto syStart = std::chrono::system_clock::now();

    std::this_thread::sleep_for(50ms);

    auto hrElapsed = std::chrono::high_resolution_clock::now() - hrStart;
    auto stElapsed = std::chrono::steady_clock::now()          - stStart;
    auto syElapsed = std::chrono::system_clock::now()          - syStart;

    std::cout << "Same 50ms sleep measured by:\n";
    std::cout << "  high_resolution_clock: "
              << std::chrono::duration_cast<std::chrono::microseconds>(hrElapsed).count()
              << " µs\n";
    std::cout << "  steady_clock:          "
              << std::chrono::duration_cast<std::chrono::microseconds>(stElapsed).count()
              << " µs\n";
    std::cout << "  system_clock:          "
              << std::chrono::duration_cast<std::chrono::microseconds>(syElapsed).count()
              << " µs\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 12: C++23 chrono formatting with std::print\n";
    std::cout << "=====================================================\n";

    auto now23 = std::chrono::system_clock::now();

    // C++23 — format durations directly
    std::println("Current time (raw):    {}", now23.time_since_epoch());
    std::println("Duration 1500ms:       {}", 1500ms);
    std::println("Duration 3s:           {}", 3s);
    std::println("Duration 2h 30min:     {}", 2h + 30min);

    std::cout << "\n=== Timer & Chrono complete ===\n";
    return 0;
}
