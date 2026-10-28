#include <coroutine>
#include <string>
#include <vector>
#include <optional>
#include <print>

// ============================================================
// C++20 COROUTINES
// ============================================================
// Topics covered:
//   1.  co_return — simplest coroutine
//   2.  co_yield  — generator pattern
//   3.  Generator<T> — lazy sequence generator
//   4.  Task<T>   — async task
//   5.  Real use case — infinite sequence generator
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

// Example 7 — ID generator
// NOTE: `prefix` is taken BY VALUE on purpose. A coroutine keeps its parameters in its
// heap-allocated frame; a reference parameter would still point at the caller's temporary
// ("ORD") after the first suspension, which is a dangling reference.
Generator<std::string> idGenerator(std::string prefix) {
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
    std::println("");
    std::println("=====================================================");
    std::println("Example 1: co_return — simplest coroutine");
    std::println("=====================================================");

    auto t1 = simpleTask(10, 32);
    std::println("simpleTask(10, 32) = {}", t1.result());

    auto t2 = greet("Kostas");
    std::println("greet('Kostas') = {}", t2.result());

    auto t3 = simpleTask(100, 200);
    std::println("simpleTask(100, 200) = {}", t3.result());


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 2: co_yield — generator pattern");
    std::println("=====================================================");

    std::print("range(1, 6):  ");
    for (int n : range(1, 6)) std::print("{} ", n);
    std::println("");

    std::print("evens(10):    ");
    for (int n : evenNumbers(10)) std::print("{} ", n);
    std::println("");

    std::print("squares(5):   ");
    for (int n : squares(5)) std::print("{} ", n);
    std::println("");


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 3: Fibonacci — infinite generator");
    std::println("=====================================================");

    std::print("fibonacci (first 10): ");
    int count = 0;
    for (int n : fibonacci()) {
        std::print("{} ", n);
        if (++count >= 10) break;   // must stop infinite generator manually
    }


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 4: Coroutine execution flow");
    std::println("=====================================================");
    std::print("  ");
    for (int n : range(1, 6)) std::print("{} ", n);
    std::println("");


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 5: Filtered generator");
    std::println("=====================================================");

    std::print("multiples of 3 in [1,20): ");
    for (int n : filteredRange(1, 20, 3)) std::print("{} ", n);
    std::println("");

    std::print("multiples of 7 in [1,50): ");
    for (int n : filteredRange(1, 50, 7)) std::print("{} ", n);
    std::println("");


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 6: ID generator");
    std::println("=====================================================");

    std::println("Generating order IDs:");
    auto orders = idGenerator("ORD");
    int generated = 0;
    for (const auto& id : orders) {
        std::println("  {}", id);
        if (++generated >= 5) break;
    }

    std::println("");
    std::println("Generating invoice IDs:");
    auto invoices = idGenerator("INV");
    generated = 0;
    for (const auto& id : invoices) {
        std::println("  {}", id);
        if (++generated >= 3) break;
    }
    return 0;
}
