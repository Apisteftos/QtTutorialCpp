#include <iostream>
#include <coroutine>
#include <string>
#include <vector>
#include <optional>
#include <print>

// ============================================================
// C++20 COROUTINES
// ============================================================
// Topics covered:
//   1.  What are coroutines
//   2.  co_return — simplest coroutine
//   3.  co_yield  — generator pattern
//   4.  co_await  — suspending and resuming
//   5.  Generator<T> — lazy sequence generator
//   6.  Task<T>   — async task
//   7.  Real use case — infinite sequence generator
// ============================================================


// ============================================================
// INFRASTRUCTURE — promise types
// These are the boilerplate needed to make coroutines work.
// In production use cppcoro or std::generator (C++23).
// ============================================================

// ---- Simple Task<T> — a coroutine that returns a value ----
template<typename T>
struct Task {
    struct promise_type {
        T value;

        Task get_return_object() {
            return Task{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_never  initial_suspend() { return {}; }
        std::suspend_always final_suspend()   noexcept { return {}; }
        void return_value(T v)  { value = v; }
        void unhandled_exception() { std::terminate(); }
    };

    std::coroutine_handle<promise_type> handle;

    explicit Task(std::coroutine_handle<promise_type> h) : handle(h) {}
    ~Task() { if (handle) handle.destroy(); }

    T result() { return handle.promise().value; }
};

// ---- Generator<T> — yields a sequence of values lazily ----
template<typename T>
struct Generator {
    struct promise_type {
        T current_value;

        Generator get_return_object() {
            return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend()   noexcept { return {}; }
        std::suspend_always yield_value(T v) {
            current_value = v;
            return {};
        }
        void return_void()         {}
        void unhandled_exception() { std::terminate(); }
    };

    std::coroutine_handle<promise_type> handle;

    explicit Generator(std::coroutine_handle<promise_type> h) : handle(h) {}
    ~Generator() { if (handle) handle.destroy(); }

    // Move only
    Generator(Generator&& o) : handle(o.handle) { o.handle = nullptr; }

    // Iterator support — makes Generator usable in range-based for
    struct iterator {
        std::coroutine_handle<promise_type> handle;
        bool done;

        iterator& operator++() {
            handle.resume();
            done = handle.done();
            return *this;
        }
        T operator*() const { return handle.promise().current_value; }
        bool operator!=(const iterator& o) const { return done != o.done; }
    };

    iterator begin() {
        handle.resume();
        return {handle, handle.done()};
    }
    iterator end() { return {handle, true}; }
};


// ============================================================
// COROUTINE FUNCTIONS
// ============================================================

// Example 2 — co_return: simplest coroutine
Task<int> simpleTask(int a, int b) {
    co_return a + b;
}

Task<std::string> greet(const std::string& name) {
    co_return "Hello, " + name + "!";
}


// Example 3 — co_yield: generator
Generator<int> range(int start, int end) {
    for (int i = start; i < end; ++i)
        co_yield i;
}

Generator<int> evenNumbers(int limit) {
    for (int i = 0; i <= limit; i += 2)
        co_yield i;
}

Generator<int> fibonacci() {
    int a = 0, b = 1;
    while (true) {
        co_yield a;
        int next = a + b;
        a = b;
        b = next;
    }
}

Generator<int> squares(int count) {
    for (int i = 1; i <= count; ++i)
        co_yield i * i;
}

// Example 7 — MCX use case: session ID generator
Generator<std::string> sessionIdGenerator(const std::string& prefix) {
    int counter = 1;
    while (true) {
        co_yield prefix + "-" + std::to_string(counter++);
    }
}

Generator<int> filteredRange(int start, int end, int divisor) {
    for (int i = start; i < end; ++i)
        if (i % divisor == 0)
            co_yield i;
}


// ============================================================
// MAIN
// ============================================================
int main()
{
    // -------------------------------------------------------
    std::cout << "=====================================================\n";
    std::cout << "Example 1: What are Coroutines?\n";
    std::cout << "=====================================================\n";

    std::cout << "A coroutine is a function that can:\n";
    std::cout << "  SUSPEND — pause execution and return control to caller\n";
    std::cout << "  RESUME  — continue from where it was suspended\n\n";

    std::cout << "Three keywords:\n";
    std::cout << "  co_return — return a value and finish the coroutine\n";
    std::cout << "  co_yield  — suspend and produce a value (generator)\n";
    std::cout << "  co_await  — suspend until an async operation completes\n\n";

    std::cout << "A function is a coroutine if it contains ANY of these.\n";
    std::cout << "The compiler transforms it into a state machine.\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: co_return — simplest coroutine\n";
    std::cout << "=====================================================\n";

    auto t1 = simpleTask(10, 32);
    std::println("simpleTask(10, 32) = {}", t1.result());

    auto t2 = greet("Kostas");
    std::println("greet('Kostas') = {}", t2.result());

    auto t3 = simpleTask(100, 200);
    std::println("simpleTask(100, 200) = {}", t3.result());


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: co_yield — generator pattern\n";
    std::cout << "=====================================================\n";

    std::cout << "range(1, 6):  ";
    for (int n : range(1, 6)) std::print("{} ", n);
    std::cout << '\n';

    std::cout << "evens(10):    ";
    for (int n : evenNumbers(10)) std::print("{} ", n);
    std::cout << '\n';

    std::cout << "squares(5):   ";
    for (int n : squares(5)) std::print("{} ", n);
    std::cout << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: Fibonacci — infinite generator\n";
    std::cout << "=====================================================\n";

    std::cout << "fibonacci (first 10): ";
    int count = 0;
    for (int n : fibonacci()) {
        std::print("{} ", n);
        if (++count >= 10) break;   // must stop infinite generator manually
    }
    std::cout << '\n';

    std::cout << "\nKey point: fibonacci() is INFINITE but LAZY\n";
    std::cout << "  Values computed ON DEMAND — not stored\n";
    std::cout << "  Memory O(1) regardless of how many values we take\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: Coroutine execution flow\n";
    std::cout << "=====================================================\n";

    std::cout << "Generator execution flow:\n\n";
    std::cout << "  Generator<int> gen = range(1, 4);\n";
    std::cout << "  // coroutine created but NOT started yet\n\n";
    std::cout << "  for (int n : gen) {\n";
    std::cout << "      // begin() called → coroutine resumes\n";
    std::cout << "      // runs until co_yield 1 → suspends\n";
    std::cout << "      // n = 1, loop body runs\n";
    std::cout << "      // ++it → coroutine resumes\n";
    std::cout << "      // runs until co_yield 2 → suspends\n";
    std::cout << "      // ...\n";
    std::cout << "  }\n";
    std::cout << "  // coroutine reaches end → done\n\n";

    std::cout << "Actual output:\n";
    std::cout << "  ";
    for (int n : range(1, 6)) std::print("{} ", n);
    std::cout << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: Filtered generator\n";
    std::cout << "=====================================================\n";

    std::cout << "multiples of 3 in [1,20): ";
    for (int n : filteredRange(1, 20, 3)) std::print("{} ", n);
    std::cout << '\n';

    std::cout << "multiples of 7 in [1,50): ";
    for (int n : filteredRange(1, 50, 7)) std::print("{} ", n);
    std::cout << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: MCX session ID generator\n";
    std::cout << "=====================================================\n";

    std::cout << "Generating session IDs:\n";
    auto sessions = sessionIdGenerator("SES");
    int generated = 0;
    for (const auto& id : sessions) {
        std::println("  {}", id);
        if (++generated >= 5) break;
    }

    std::cout << "\nGenerating group IDs:\n";
    auto groups = sessionIdGenerator("GRP-ALPHA");
    generated = 0;
    for (const auto& id : groups) {
        std::println("  {}", id);
        if (++generated >= 3) break;
    }

    std::cout << "\nCoroutine advantages for ID generation:\n";
    std::cout << "  No vector pre-allocation needed\n";
    std::cout << "  IDs generated on demand\n";
    std::cout << "  Infinite sequence — stop whenever\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: co_await concept (explanation)\n";
    std::cout << "=====================================================\n";

    std::cout << "co_await suspends coroutine until an awaitable completes:\n\n";
    std::cout << "  Task<string> fetchData() {\n";
    std::cout << "      auto result = co_await httpGet('url');  // suspends\n";
    std::cout << "      co_return result;                       // resumes here\n";
    std::cout << "  }\n\n";
    std::cout << "  Typical use cases:\n";
    std::cout << "  - Network I/O — co_await socket.read()\n";
    std::cout << "  - File I/O    — co_await file.read()\n";
    std::cout << "  - Timers      — co_await sleep(1s)\n";
    std::cout << "  - Tasks       — co_await otherCoroutine()\n\n";
    std::cout << "  Full co_await needs an async framework:\n";
    std::cout << "  - cppcoro (C++ coroutine library)\n";
    std::cout << "  - Boost.Asio (network + coroutines)\n";
    std::cout << "  - Qt (QCoro library for Qt coroutines)\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Summary: Coroutines vs Alternatives\n";
    std::cout << "=====================================================\n";

    std::cout << "Generator<T> vs std::vector:\n";
    std::cout << "  vector   — stores ALL values in memory upfront\n";
    std::cout << "  Generator— computes values ON DEMAND, O(1) memory\n\n";

    std::cout << "Coroutine vs callback:\n";
    std::cout << "  Callback — control flow fragmented, hard to follow\n";
    std::cout << "  Coroutine— reads like synchronous code, suspends cleanly\n\n";

    std::cout << "Coroutine vs thread:\n";
    std::cout << "  Thread   — OS-managed, heavy (MB stack, scheduling)\n";
    std::cout << "  Coroutine— user-space, lightweight (KB stack, no OS)\n";

    std::cout << "\n=== Coroutines complete ===\n";
    return 0;
}
