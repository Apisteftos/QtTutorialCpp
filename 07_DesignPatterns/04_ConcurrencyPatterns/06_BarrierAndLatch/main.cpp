// ============================================================
// Barrier and Latch - threads wait for each other at a meeting point
//
// Topics:
//   1. std::latch as a start gate and as a "all done" counter
//   2. std::barrier: threads work in phases and meet after each phase
//   3. Latch is single-use, barrier is reusable
//
// See Readme.md for the diagrams and the explanation.
// Build: g++ -std=c++23 -Wall -Wextra -pthread main.cpp -o barrier_latch
// ============================================================

#include <barrier>
#include <latch>
#include <numeric>
#include <print>
#include <thread>
#include <vector>

// === EXAMPLE 1: LATCH AS START GATE AND COMPLETION COUNTER ===

void example1()
{
    std::println("=== EXAMPLE 1: LATCH ===");

    constexpr int workers = 4;

    std::latch startGate(1);        // opens when its counter reaches 0
    std::latch finished(workers);   // one count_down per worker

    std::vector<int>         results(workers, 0);
    std::vector<std::thread> threads;
    for (int i = 0; i < workers; ++i) {
        threads.emplace_back([&, i] {
            startGate.wait();                // all workers start together
            results[i] = (i + 1) * 10;       // the work
            finished.count_down();           // tell main: I am done
        });
    }

    startGate.count_down();                  // open the gate
    finished.wait();                         // wait until all 4 are done

    std::println("sum of results = {}", std::accumulate(results.begin(), results.end(), 0));

    for (auto& t : threads) {
        t.join();
    }
}

// === EXAMPLE 2: BARRIER WITH PHASES ===

void example2()
{
    std::println("=== EXAMPLE 2: BARRIER ===");

    constexpr int threadsCount = 3;
    constexpr int phases       = 3;

    std::vector<int> data(threadsCount, 0);
    int              phase = 0;

    // The completion function runs once per phase, after all threads arrived
    // and before any of them continues.
    auto onPhaseDone = [&]() noexcept {
        int sum = std::accumulate(data.begin(), data.end(), 0);
        std::println("phase {} complete, sum of all values = {}", phase, sum);
        ++phase;
    };
    std::barrier sync(threadsCount, onPhaseDone);

    std::vector<std::thread> threads;
    for (int i = 0; i < threadsCount; ++i) {
        threads.emplace_back([&, i] {
            for (int p = 0; p < phases; ++p) {
                data[i] = (i + 1) * (p + 1);   // every thread writes its own slot
                sync.arrive_and_wait();        // wait until all threads wrote
            }
        });
    }
    for (auto& t : threads) {
        t.join();
    }
}

// === EXAMPLE 3: SINGLE-USE vs REUSABLE ===

void example3()
{
    std::println("=== EXAMPLE 3: LATCH IS SINGLE-USE ===");

    std::latch once(2);
    std::println("try_wait before count_down: {}", once.try_wait());
    once.count_down(2);
    std::println("try_wait after count_down:  {}", once.try_wait());
    // There is no reset(): the latch stays open. A std::barrier starts a new
    // phase automatically (example 2).
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
