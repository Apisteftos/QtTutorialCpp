// ============================================================
// Thread Pool - a fixed set of worker threads that run submitted tasks
//
// Topics:
//   1. Submit tasks, collect results through futures
//   2. An exception in a task travels to the caller
//   3. The destructor finishes all queued tasks
//
// See Readme.md for the diagrams and the explanation.
// Build: g++ -std=c++23 -Wall -Wextra -pthread main.cpp -o thread_pool
// ============================================================

#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <deque>
#include <functional>
#include <future>
#include <mutex>
#include <print>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include <vector>

class ThreadPool {
public:
    explicit ThreadPool(std::size_t threads)
    {
        for (std::size_t i = 0; i < threads; ++i) {
            workers_.emplace_back([this] { workerLoop(); });
        }
    }

    // Members are destroyed in reverse order: workers_ is declared last, so
    // the threads are joined first. By then 'stopping_' is set and the
    // workers finish the remaining tasks before they return.
    ~ThreadPool()
    {
        {
            std::lock_guard lock(mutex_);
            stopping_ = true;
        }
        wake_.notify_all();
    }

    ThreadPool(const ThreadPool&)            = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    // std::packaged_task is move-only, std::move_only_function (C++23) can hold it.
    template <typename F>
    [[nodiscard]] auto submit(F&& callable) -> std::future<std::invoke_result_t<std::decay_t<F>>>
    {
        using Result = std::invoke_result_t<std::decay_t<F>>;

        std::packaged_task<Result()> task(std::forward<F>(callable));
        std::future<Result>          future = task.get_future();
        {
            std::lock_guard lock(mutex_);
            if (stopping_) {
                throw std::runtime_error("ThreadPool: submit() after shutdown");
            }
            tasks_.push_back(std::move(task));
        }
        wake_.notify_one();
        return future;
    }

private:
    void workerLoop()
    {
        for (;;) {
            std::move_only_function<void()> task;
            {
                std::unique_lock lock(mutex_);
                wake_.wait(lock, [this] { return stopping_ || !tasks_.empty(); });
                if (tasks_.empty()) {
                    return;                  // stopping and nothing left to do
                }
                task = std::move(tasks_.front());
                tasks_.pop_front();
            }
            task();                          // run outside the lock
        }
    }

    std::mutex                                       mutex_;
    std::condition_variable                          wake_;
    std::deque<std::move_only_function<void()>>      tasks_;
    bool                                             stopping_ = false;
    std::vector<std::jthread>                        workers_;
};

// === EXAMPLE 1: SUBMIT TASKS, COLLECT RESULTS ===

void example1()
{
    std::println("=== EXAMPLE 1: SUBMIT AND COLLECT ===");

    ThreadPool pool(4);

    std::vector<std::future<int>> results;
    for (int i = 1; i <= 8; ++i) {
        results.push_back(pool.submit([i] { return i * i; }));
    }

    // get() blocks until the task is done. Reading in submit order gives a
    // stable output even though the tasks run in parallel.
    for (int i = 0; i < 8; ++i) {
        std::println("task {} -> {}", i + 1, results[i].get());
    }
}

// === EXAMPLE 2: AN EXCEPTION IN A TASK ===

void example2()
{
    std::println("=== EXAMPLE 2: EXCEPTION IN A TASK ===");

    ThreadPool pool(2);

    auto failing = pool.submit([]() -> int { throw std::runtime_error("task failed"); });
    auto fine    = pool.submit([] { return 7; });

    std::println("other task still works: {}", fine.get());

    try {
        (void)failing.get();
    } catch (const std::exception& e) {
        std::println("caught in the caller: {}", e.what());
    }
}

// === EXAMPLE 3: THE DESTRUCTOR FINISHES QUEUED TASKS ===

void example3()
{
    std::println("=== EXAMPLE 3: DESTRUCTOR DRAINS THE QUEUE ===");

    std::atomic<int> done{0};
    {
        ThreadPool pool(2);
        for (int i = 0; i < 20; ++i) {
            (void)pool.submit([&done] { done.fetch_add(1); });
        }
    }   // pool destroyed here: waits for all 20 tasks

    std::println("tasks finished before the pool was gone: {}", done.load());
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
