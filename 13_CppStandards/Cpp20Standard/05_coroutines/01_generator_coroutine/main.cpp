/*
 * Coroutines (<coroutine>, C++20): co_yield, co_return, co_await
 *
 * C++20 provides the language support and <coroutine>; the ready-made
 * std::generator arrives only in C++23. Here a small generator type is
 * written by hand, which shows the pieces: promise_type, coroutine_handle.
 *
 * Compile: g++ -std=c++20 -Wall -Wextra main.cpp -o example
 */

#include <coroutine>
#include <iostream>
#include <utility>

template <typename T>
class Generator {
public:
    struct promise_type {
        T current{};
        Generator get_return_object() { return Generator{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_always initial_suspend() noexcept { return {}; }   // start suspended
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T value) {                      // co_yield value
            current = std::move(value);
            return {};
        }
        void return_void() {}
        void unhandled_exception() { throw; }
    };

    explicit Generator(std::coroutine_handle<promise_type> handle) : handle_(handle) {}
    Generator(Generator&& other) noexcept : handle_(std::exchange(other.handle_, {})) {}
    Generator(const Generator&) = delete;
    ~Generator() {
        if (handle_) {
            handle_.destroy();
        }
    }

    bool next() {                       // resume until the next co_yield
        handle_.resume();
        return !handle_.done();
    }
    const T& value() const { return handle_.promise().current; }

private:
    std::coroutine_handle<promise_type> handle_;
};

Generator<int> countTo(int limit) {
    for (int i = 1; i <= limit; ++i) {
        co_yield i;                      // suspend and hand a value to the caller
    }
}

Generator<long long> fibonacci() {
    long long a = 0;
    long long b = 1;
    while (true) {                       // infinite, because values are produced on demand
        co_yield a;
        a = std::exchange(b, a + b);
    }
}

int main() {
    // === EXAMPLE 1: finite generator ===
    std::cout << "=== Example 1: countTo(5) ===\n";
    auto numbers = countTo(5);
    while (numbers.next()) {
        std::cout << numbers.value() << " ";
    }
    std::cout << "\n";

    // === EXAMPLE 2: infinite generator ===
    std::cout << "\n=== Example 2: fibonacci ===\n";
    auto fib = fibonacci();
    for (int i = 0; i < 10 && fib.next(); ++i) {
        std::cout << fib.value() << " ";
    }
    std::cout << "\n";
    return 0;
}
