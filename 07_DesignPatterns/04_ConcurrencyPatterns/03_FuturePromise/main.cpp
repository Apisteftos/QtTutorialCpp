// ============================================================
// Future / Promise - one thread delivers a result, another waits for it
//
// Topics:
//   1. std::promise and std::future between two threads
//   2. std::async: promise and thread created for you
//   3. Exceptions travel through the future
//   4. std::shared_future: several readers of one result
//
// See Readme.md for the diagrams and the explanation.
// Build: g++ -std=c++23 -Wall -Wextra -pthread main.cpp -o future_promise
// ============================================================

#include <future>
#include <print>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

// === EXAMPLE 1: PROMISE AND FUTURE ===

void example1()
{
    std::println("=== EXAMPLE 1: PROMISE AND FUTURE ===");

    std::promise<int> promise;                    // write end
    std::future<int>  future = promise.get_future(); // read end

    std::thread worker([p = std::move(promise)]() mutable {
        int result = 6 * 7;                       // pretend this takes a while
        p.set_value(result);                      // deliver the result
    });

    std::println("waiting ...");
    std::println("result = {}", future.get());    // blocks until set_value
    worker.join();
}

// === EXAMPLE 2: std::async ===

int slowSquare(int x)
{
    return x * x;
}

void example2()
{
    std::println("=== EXAMPLE 2: std::async ===");

    // launch::async forces a new thread. The returned future must be kept:
    // ignoring it would make the call block (std::async is [[nodiscard]]).
    std::future<int> a = std::async(std::launch::async, slowSquare, 12);
    std::future<int> b = std::async(std::launch::async, slowSquare, 13);

    std::println("12^2 = {}, 13^2 = {}", a.get(), b.get());
}

// === EXAMPLE 3: EXCEPTIONS TRAVEL THROUGH THE FUTURE ===

void example3()
{
    std::println("=== EXAMPLE 3: EXCEPTIONS ===");

    std::promise<std::string> promise;
    std::future<std::string>  future = promise.get_future();

    std::thread worker([p = std::move(promise)]() mutable {
        try {
            throw std::runtime_error("file not found");
        } catch (...) {
            p.set_exception(std::current_exception());
        }
    });

    try {
        std::println("{}", future.get());
    } catch (const std::exception& e) {
        std::println("get() rethrew: {}", e.what());
    }
    worker.join();

    // With std::async the exception is stored automatically.
    auto f = std::async(std::launch::async, []() -> int { throw std::logic_error("bad input"); });
    try {
        (void)f.get();
    } catch (const std::exception& e) {
        std::println("async rethrew: {}", e.what());
    }
}

// === EXAMPLE 4: SHARED FUTURE ===

void example4()
{
    std::println("=== EXAMPLE 4: SHARED FUTURE ===");

    std::promise<int>       promise;
    std::shared_future<int> shared = promise.get_future().share();

    std::vector<int>         seen(3, 0);
    std::vector<std::thread> readers;
    for (int i = 0; i < 3; ++i) {
        readers.emplace_back([shared, &seen, i] {
            seen[i] = shared.get();               // every reader gets the same value
        });
    }

    promise.set_value(99);
    for (auto& t : readers) {
        t.join();
    }
    std::println("readers saw: {} {} {}", seen[0], seen[1], seen[2]);
}

int main()
{
    example1();
    std::println("");
    example2();
    std::println("");
    example3();
    std::println("");
    example4();
    return 0;
}
