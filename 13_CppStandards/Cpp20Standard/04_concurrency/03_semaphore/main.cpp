/*
 * std::counting_semaphore and std::binary_semaphore (<semaphore>, C++20)
 *
 * Compile: g++ -std=c++20 -Wall -Wextra -pthread main.cpp -o example
 */

#include <atomic>
#include <chrono>
#include <iostream>
#include <semaphore>
#include <thread>
#include <vector>

int main() {
    // === EXAMPLE 1: counting_semaphore limits parallel access ===
    std::cout << "=== Example 1: at most 2 threads inside ===\n";
    std::counting_semaphore<4> slots(2);
    std::atomic<int> inside{0};
    std::atomic<int> maxInside{0};
    {
        std::vector<std::jthread> threads;
        for (int i = 0; i < 6; ++i) {
            threads.emplace_back([&] {
                slots.acquire();
                int now = ++inside;
                int seen = maxInside.load();
                while (now > seen && !maxInside.compare_exchange_weak(seen, now)) {
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                --inside;
                slots.release();
            });
        }
    }
    std::cout << "maximum number of threads inside at the same time: " << maxInside << "\n";

    // === EXAMPLE 2: binary_semaphore as signal ===
    std::cout << "\n=== Example 2: binary_semaphore as signal ===\n";
    std::binary_semaphore signal(0);
    int data = 0;
    std::jthread producer([&] {
        data = 42;
        signal.release();          // tell the consumer that data is ready
    });
    signal.acquire();              // blocks until released
    std::cout << "consumer got " << data << "\n";

    // === EXAMPLE 3: try_acquire ===
    std::cout << "\n=== Example 3: try_acquire ===\n";
    std::binary_semaphore lock(1);
    std::cout << "first try_acquire:  " << std::boolalpha << lock.try_acquire() << "\n";
    std::cout << "second try_acquire: " << lock.try_acquire() << "\n";
    return 0;
}
