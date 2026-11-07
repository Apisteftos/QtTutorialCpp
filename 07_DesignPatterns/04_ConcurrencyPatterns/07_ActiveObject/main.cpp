// ============================================================
// Active Object - an object with its own thread; calls become requests
// in a queue and return a future
//
// Topics:
//   1. Calling an active object returns immediately with a future
//   2. Many client threads, one servant thread: the servant needs no lock
//   3. Requests run in the order they were queued
//
// See Readme.md for the diagrams and the explanation.
// Build: g++ -std=c++23 -Wall -Wextra -pthread main.cpp -o active_object
// ============================================================

#include <condition_variable>
#include <deque>
#include <functional>
#include <future>
#include <mutex>
#include <print>
#include <thread>
#include <type_traits>
#include <vector>

// The servant: an ordinary class, NOT thread-safe, no mutex inside.
class Calculator {
public:
    int add(int value)
    {
        total_ += value;
        return total_;
    }
    int total() const { return total_; }

private:
    int total_ = 0;
};

// The active object: proxy + activation queue + scheduler thread.
class ActiveCalculator {
public:
    ActiveCalculator() : thread_([this] { run(); }) {}

    ~ActiveCalculator()
    {
        {
            std::lock_guard lock(mutex_);
            stopping_ = true;
        }
        wake_.notify_one();
        // thread_ is declared last and is joined first: the scheduler runs the
        // requests that are still queued, then returns.
    }

    // Proxy methods: they only queue a request and return a future.
    [[nodiscard]] std::future<int> add(int value)
    {
        return enqueue([this, value] { return servant_.add(value); });
    }
    [[nodiscard]] std::future<int> total()
    {
        return enqueue([this] { return servant_.total(); });
    }

private:
    template <typename F>
    std::future<std::invoke_result_t<F>> enqueue(F request)
    {
        using Result = std::invoke_result_t<F>;
        std::packaged_task<Result()> task(std::move(request));
        std::future<Result>          future = task.get_future();
        {
            std::lock_guard lock(mutex_);
            queue_.push_back(std::move(task));
        }
        wake_.notify_one();
        return future;
    }

    // The scheduler: the only thread that ever touches servant_.
    void run()
    {
        for (;;) {
            std::move_only_function<void()> request;
            {
                std::unique_lock lock(mutex_);
                wake_.wait(lock, [this] { return stopping_ || !queue_.empty(); });
                if (queue_.empty()) {
                    return;
                }
                request = std::move(queue_.front());
                queue_.pop_front();
            }
            request();
        }
    }

    Calculator                                  servant_;
    std::mutex                                  mutex_;
    std::condition_variable                     wake_;
    std::deque<std::move_only_function<void()>> queue_;
    bool                                        stopping_ = false;
    std::jthread                                thread_;     // last member
};

// === EXAMPLE 1: CALLS RETURN A FUTURE ===

void example1()
{
    std::println("=== EXAMPLE 1: CALLS RETURN A FUTURE ===");

    ActiveCalculator calc;

    std::future<int> f = calc.add(5);     // returns at once, the work happens on the other thread
    std::println("add(5) returned a future, result = {}", f.get());
    std::println("total = {}", calc.total().get());
}

// === EXAMPLE 2: MANY CLIENTS, ONE SERVANT THREAD ===

void example2()
{
    std::println("=== EXAMPLE 2: MANY CLIENTS ===");

    ActiveCalculator calc;

    std::vector<std::thread> clients;
    for (int c = 0; c < 4; ++c) {
        clients.emplace_back([&calc] {
            for (int i = 0; i < 250; ++i) {
                (void)calc.add(1);        // no lock needed in Calculator
            }
        });
    }
    for (auto& t : clients) {
        t.join();
    }

    std::println("total after 4 x 250 additions = {}", calc.total().get());
}

// === EXAMPLE 3: REQUESTS RUN IN QUEUE ORDER ===

void example3()
{
    std::println("=== EXAMPLE 3: FIFO ORDER ===");

    ActiveCalculator calc;

    std::vector<std::future<int>> futures;
    for (int i = 1; i <= 5; ++i) {
        futures.push_back(calc.add(i));
    }
    for (auto& f : futures) {
        std::println("running total: {}", f.get());
    }
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
