/*
 * std::jthread, std::stop_token, std::stop_source, std::stop_callback
 * (<thread>, <stop_token>, C++20)
 *
 * jthread joins automatically in its destructor and supports cooperative
 * cancellation through a stop token.
 *
 * Compile: g++ -std=c++20 -Wall -Wextra -pthread main.cpp -o example
 */

#include <atomic>
#include <chrono>
#include <iostream>
#include <stop_token>
#include <thread>

int main() {
    // === EXAMPLE 1: automatic join ===
    std::cout << "=== Example 1: jthread joins in the destructor ===\n";
    {
        std::jthread worker([] { std::cout << "worker ran\n"; });
    }   // no explicit join() needed, unlike std::thread
    std::cout << "worker finished before this line\n";

    // === EXAMPLE 2: stop_token ===
    std::cout << "\n=== Example 2: cooperative cancellation ===\n";
    std::atomic<int> loops{0};
    {
        // a jthread passes a stop_token as first argument if the callable takes one
        std::jthread counter([&loops](std::stop_token token) {
            while (!token.stop_requested()) {
                ++loops;
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
            std::cout << "stop requested, leaving the loop\n";
        });
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
        counter.request_stop();    // also done automatically by the destructor
    }
    std::cout << "loop ran at least once: " << (loops > 0) << "\n";

    // === EXAMPLE 3: stop_callback ===
    std::cout << "\n=== Example 3: stop_source and stop_callback ===\n";
    std::stop_source source;
    std::stop_callback callback(source.get_token(), [] { std::cout << "callback: stop was requested\n"; });
    std::cout << "stop possible: " << source.stop_possible() << "\n";
    source.request_stop();     // runs the callback in this thread
    std::cout << "stop_requested: " << source.stop_requested() << "\n";
    return 0;
}
