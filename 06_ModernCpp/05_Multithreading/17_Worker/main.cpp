/*
 * Worker thread
 *
 * A worker is one thread that waits for jobs, takes them from a queue one at a
 * time and runs them. The caller only puts jobs into the queue.
 *
 * Compile: g++ -std=c++23 -Wall -Wextra -pthread main.cpp -o example
 */

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <numeric>
#include <print>
#include <queue>
#include <string>
#include <stop_token>
#include <thread>
#include <utility>
#include <vector>

class Worker {
public:
    Worker() : thread_([this](std::stop_token stop) { run(stop); }) {}

    // Puts a job into the queue and returns a future for its result.
    template <typename Function>
    auto submit(Function function) {
        using Result = decltype(function());
        std::packaged_task<Result()> task(std::move(function));
        auto future = task.get_future();
        {
            std::lock_guard lock(mutex_);
            jobs_.push([task = std::move(task)]() mutable { task(); });
        }
        wakeup_.notify_one();  // tell the worker that there is something to do
        return future;
    }

private:
    void run(std::stop_token stop) {
        while (true) {
            std::move_only_function<void()> job;
            {
                std::unique_lock lock(mutex_);
                // Sleeps until a job exists or a stop is requested.
                // Jobs that are already queued are still run before the worker ends.
                wakeup_.wait(lock, stop, [this] { return !jobs_.empty(); });
                if (jobs_.empty()) {
                    return;  // stop requested and nothing left to do
                }
                job = std::move(jobs_.front());
                jobs_.pop();
            }
            job();  // run the job outside the lock, so submit() is never blocked by it
        }
    }

    std::mutex mutex_;
    std::condition_variable_any wakeup_;
    std::queue<std::move_only_function<void()>> jobs_;
    std::jthread thread_;  // declared last: destroyed first, requests stop and joins
};

int main() {
    // === EXAMPLE 1: One worker runs jobs in the order they were submitted ===
    std::println("=== Example 1: Jobs run in order ===");
    {
        Worker worker;
        for (int i = 1; i <= 3; ++i) {
            worker.submit([i] { std::println("  job {} done", i); });
        }
    }  // leaving the scope: the worker finishes the queue, then its thread is joined
    std::println("worker finished");

    // === EXAMPLE 2: A job with a result ===
    std::println("");
    std::println("=== Example 2: A job returns a value ===");
    {
        Worker worker;
        auto sum = worker.submit([] {
            std::vector<int> numbers(100);
            std::iota(numbers.begin(), numbers.end(), 1);
            return std::accumulate(numbers.begin(), numbers.end(), 0);
        });
        auto text = worker.submit([] { return std::string("hello from the worker"); });

        std::println("sum of 1..100 = {}", sum.get());  // waits until the job is done
        std::println("text = {}", text.get());
    }
    return 0;
}
