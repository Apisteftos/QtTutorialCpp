/*
 * std::latch and std::barrier (<latch>, <barrier>, C++20)
 *
 * latch   : a one-time countdown; threads wait until it reaches zero
 * barrier : reusable; all threads wait for each other at every phase
 *
 * Compile: g++ -std=c++20 -Wall -Wextra -pthread main.cpp -o example
 */

#include <atomic>
#include <barrier>
#include <iostream>
#include <latch>
#include <thread>
#include <vector>

int main() {
    // === EXAMPLE 1: latch ===
    std::cout << "=== Example 1: latch ===\n";
    constexpr int workers = 4;
    std::latch ready(workers);
    std::atomic<int> started{0};
    {
        std::vector<std::jthread> threads;
        for (int i = 0; i < workers; ++i) {
            threads.emplace_back([&] {
                ++started;
                ready.count_down();    // one less to wait for
            });
        }
        ready.wait();                  // returns when the count is 0
        std::cout << "all " << started << " workers have started\n";
    }

    // === EXAMPLE 2: barrier with a completion step ===
    std::cout << "\n=== Example 2: barrier ===\n";
    int phase = 0;
    // the completion function runs once per phase, after all threads arrived
    std::barrier sync(3, [&phase]() noexcept { ++phase; });
    std::atomic<int> work{0};
    {
        std::vector<std::jthread> threads;
        for (int i = 0; i < 3; ++i) {
            threads.emplace_back([&] {
                for (int round = 0; round < 2; ++round) {
                    ++work;
                    sync.arrive_and_wait();   // nobody continues until all three are here
                }
            });
        }
    }
    std::cout << "phases completed: " << phase << ", total work items: " << work << "\n";
    return 0;
}
