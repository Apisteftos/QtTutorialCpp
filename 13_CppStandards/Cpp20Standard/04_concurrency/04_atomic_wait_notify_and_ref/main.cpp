/*
 * std::atomic::wait / notify_one / notify_all, std::atomic_ref,
 * std::atomic<std::shared_ptr>, std::atomic_flag test (C++20)
 *
 * Compile: g++ -std=c++20 -Wall -Wextra -pthread main.cpp -o example
 */

#include <atomic>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

int main() {
    // === EXAMPLE 1: wait and notify ===
    std::cout << "=== Example 1: atomic wait / notify ===\n";
    std::atomic<int> state{0};
    std::jthread waiter([&] {
        state.wait(0);                 // sleeps while the value is still 0
        std::cout << "waiter woke up, state = " << state.load() << "\n";
    });
    state.store(1);
    state.notify_one();                // wake the waiting thread
    waiter.join();

    // === EXAMPLE 2: atomic_ref ===
    std::cout << "\n=== Example 2: atomic_ref ===\n";
    // A plain int becomes atomic only while it is accessed through an atomic_ref
    int counter = 0;
    {
        std::vector<std::jthread> threads;
        for (int i = 0; i < 4; ++i) {
            threads.emplace_back([&counter] {
                std::atomic_ref<int> ref(counter);
                for (int n = 0; n < 10000; ++n) {
                    ++ref;
                }
            });
        }
    }
    std::cout << "counter = " << counter << "\n";

    // === EXAMPLE 3: atomic shared_ptr ===
    std::cout << "\n=== Example 3: atomic<shared_ptr> ===\n";
    std::atomic<std::shared_ptr<int>> shared = std::make_shared<int>(5);
    std::shared_ptr<int> current = shared.load();
    shared.store(std::make_shared<int>(6));
    std::cout << "old value " << *current << ", new value " << *shared.load() << "\n";

    // === EXAMPLE 4: atomic_flag can be tested ===
    std::cout << "\n=== Example 4: atomic_flag::test ===\n";
    std::atomic_flag flag;             // starts clear in C++20
    std::cout << "initial: " << flag.test() << "\n";
    flag.test_and_set();
    std::cout << "after test_and_set: " << flag.test() << "\n";
    return 0;
}
