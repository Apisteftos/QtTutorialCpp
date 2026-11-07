// ============================================================
// Thread-Specific Storage - every thread gets its own copy of a variable
//
// Topics:
//   1. thread_local: separate counter per thread, no lock
//   2. Per-thread accumulation, merged once at the end
//   3. thread_local vs one shared atomic
//
// See Readme.md for the diagrams and the explanation.
// Build: g++ -std=c++23 -Wall -Wextra -pthread main.cpp -o thread_specific
// ============================================================

#include <atomic>
#include <mutex>
#include <print>
#include <thread>
#include <vector>

// === EXAMPLE 1: A SEPARATE COUNTER PER THREAD ===

thread_local int tlsCounter = 0;      // every thread starts with its own 0

int bump()
{
    return ++tlsCounter;              // no lock: only this thread sees this copy
}

void example1()
{
    std::println("=== EXAMPLE 1: thread_local COUNTER ===");

    std::vector<int>         results(3, 0);
    std::vector<std::thread> threads;
    for (int i = 0; i < 3; ++i) {
        threads.emplace_back([&results, i] {
            int last = 0;
            for (int k = 0; k <= i; ++k) {     // thread i bumps (i + 1) times
                last = bump();
            }
            results[i] = last;
        });
    }
    for (auto& t : threads) {
        t.join();
    }

    for (int i = 0; i < 3; ++i) {
        std::println("thread {} bumped {} time(s), its counter = {}", i, i + 1, results[i]);
    }
    std::println("main thread counter is untouched: {}", tlsCounter);
}

// === EXAMPLE 2: PER-THREAD ACCUMULATION ===

thread_local long long partialSum = 0;

class Totals {
public:
    void add(long long value)
    {
        std::lock_guard lock(mutex_);
        total_ += value;
    }
    long long total() const { return total_; }

private:
    std::mutex mutex_;
    long long  total_ = 0;
};

void example2()
{
    std::println("=== EXAMPLE 2: ACCUMULATE LOCALLY, MERGE ONCE ===");

    Totals totals;
    std::vector<std::thread> threads;
    for (int t = 0; t < 4; ++t) {
        threads.emplace_back([&totals] {
            for (int i = 1; i <= 100000; ++i) {
                partialSum += i;               // hot loop: no lock, no atomic
            }
            totals.add(partialSum);            // one locked operation per thread
        });
    }
    for (auto& t : threads) {
        t.join();
    }

    std::println("total = {} (expected {})", totals.total(), 4LL * 100000 * 100001 / 2);
}

// === EXAMPLE 3: thread_local vs SHARED ATOMIC ===

void example3()
{
    std::println("=== EXAMPLE 3: thread_local vs ATOMIC ===");

    std::atomic<int> shared{0};
    std::vector<int> perThread(4, 0);

    std::vector<std::thread> threads;
    for (int t = 0; t < 4; ++t) {
        threads.emplace_back([&shared, &perThread, t] {
            for (int i = 0; i < 1000; ++i) {
                shared.fetch_add(1);           // all threads touch ONE variable
                ++tlsCounter;                  // each thread touches ITS OWN variable
            }
            perThread[t] = tlsCounter;
        });
    }
    for (auto& t : threads) {
        t.join();
    }

    std::println("shared atomic = {}", shared.load());
    std::println("per-thread counters = {} {} {} {}",
                 perThread[0], perThread[1], perThread[2], perThread[3]);
}

int main()
{
    example1();
    std::println("");
    example2();
    std::println("");
    example3();
    return 0;
}
