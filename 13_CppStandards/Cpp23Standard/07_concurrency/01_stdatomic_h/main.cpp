/*
 * <stdatomic.h> compatibility with C: _Atomic(T) and the atomic_* functions
 * (C++23)
 *
 * The header lets code share atomic declarations between C and C++.
 * In pure C++ code std::atomic<T> is the normal choice.
 *
 * Compile: g++ -std=c++23 -Wall -Wextra -pthread main.cpp -o example
 */

#include <stdatomic.h>

#include <print>
#include <thread>
#include <vector>

int main() {
    // === EXAMPLE 1: _Atomic(T) and atomic_fetch_add ===
    std::println("=== Example 1: counter shared by several threads ===");
    _Atomic(int) counter = 0;   // same type as std::atomic<int>

    std::vector<std::jthread> threads;
    for (int t = 0; t < 4; ++t) {
        threads.emplace_back([&counter] {
            for (int i = 0; i < 10000; ++i) {
                atomic_fetch_add(&counter, 1);
            }
        });
    }
    threads.clear();   // jthread joins in the destructor
    std::println("counter = {}", atomic_load(&counter));

    // === EXAMPLE 2: exchange and compare_exchange ===
    std::println("");
    std::println("=== Example 2: exchange and compare_exchange ===");
    int old = atomic_exchange(&counter, 5);
    std::println("exchange: old {}, new {}", old, atomic_load(&counter));

    int expected = 5;
    bool changed = atomic_compare_exchange_strong(&counter, &expected, 9);
    std::println("compare_exchange(5 -> 9): {}, value {}", changed, atomic_load(&counter));
    return 0;
}
