// ============================================================
// Blocking Queue (unbounded) - a queue whose pop() waits while it is empty
//
// "Blocking" means the CONSUMER waits when there is nothing to take.
// This queue has NO size limit, so push() never waits.
// For a queue with a size limit see ../16_BoundedQueue.
//
// Topics:
//   1. FIFO order, try_pop on an empty queue
//   2. pop() blocks until a producer pushes
//   3. pop_for(): wait with a timeout
//   4. close() wakes up every waiting consumer
//   5. Items pushed before close() are still delivered
//   6. No size limit: push() never waits
//
// See Readme.md for the diagrams and the explanation.
// Build: g++ -std=c++23 -Wall -Wextra -pthread main.cpp -o blocking_queue
// ============================================================

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <deque>
#include <mutex>
#include <optional>
#include <print>
#include <thread>
#include <vector>

template <typename T>
class BlockingQueue {
public:
    // Never waits. Returns false if the queue was closed.
    bool push(T value)
    {
        {
            std::lock_guard lock(mutex_);
            if (closed_) {
                return false;
            }
            items_.push_back(std::move(value));
        }
        notEmpty_.notify_one();
        return true;
    }

    // Waits until an item is available. Returns an empty optional when the
    // queue is closed and has no items left.
    std::optional<T> pop()
    {
        std::unique_lock lock(mutex_);
        notEmpty_.wait(lock, [this] { return !items_.empty() || closed_; });
        return takeFront();
    }

    // Does not wait. Empty optional if there is no item right now.
    std::optional<T> try_pop()
    {
        std::lock_guard lock(mutex_);
        return takeFront();
    }

    // Waits at most 'timeout'. Empty optional on timeout or when closed and empty.
    template <typename Rep, typename Period>
    std::optional<T> pop_for(std::chrono::duration<Rep, Period> timeout)
    {
        std::unique_lock lock(mutex_);
        notEmpty_.wait_for(lock, timeout, [this] { return !items_.empty() || closed_; });
        return takeFront();
    }

    // After close(): push() fails, pop() delivers the remaining items and then
    // returns empty optionals. All waiting threads are woken up.
    void close()
    {
        {
            std::lock_guard lock(mutex_);
            closed_ = true;
        }
        notEmpty_.notify_all();
    }

    std::size_t size() const
    {
        std::lock_guard lock(mutex_);
        return items_.size();
    }

private:
    // The caller holds the lock.
    std::optional<T> takeFront()
    {
        if (items_.empty()) {
            return std::nullopt;
        }
        T value = std::move(items_.front());
        items_.pop_front();
        return value;
    }

    mutable std::mutex      mutex_;
    std::condition_variable notEmpty_;
    std::deque<T>           items_;
    bool                    closed_ = false;
};

// === EXAMPLE 1: FIFO ORDER AND try_pop ===

void example1()
{
    std::println("=== EXAMPLE 1: FIFO AND try_pop ===");

    BlockingQueue<int> queue;
    (void)queue.push(1);
    (void)queue.push(2);
    (void)queue.push(3);

    int first  = *queue.pop();
    int second = *queue.pop();
    int third  = *queue.pop();
    std::println("pop: {} {} {}", first, second, third);
    std::println("try_pop on an empty queue has a value: {}", queue.try_pop().has_value());
}

// === EXAMPLE 2: pop() BLOCKS UNTIL A PRODUCER PUSHES ===

void example2()
{
    std::println("=== EXAMPLE 2: pop() WAITS ===");

    BlockingQueue<int> queue;
    int                received = 0;

    std::thread consumer([&] {
        auto item = queue.pop();               // the queue is empty: waits here
        received  = item.value_or(-1);
    });

    (void)queue.push(42);                      // wakes the consumer
    consumer.join();

    std::println("consumer received {}", received);
}

// === EXAMPLE 3: WAITING WITH A TIMEOUT ===

void example3()
{
    std::println("=== EXAMPLE 3: pop_for ===");

    BlockingQueue<int> queue;

    auto none = queue.pop_for(std::chrono::milliseconds(20));
    std::println("empty queue, 20 ms: has value = {}", none.has_value());

    (void)queue.push(7);
    auto some = queue.pop_for(std::chrono::milliseconds(20));
    std::println("after push(7): has value = {}, value = {}", some.has_value(), some.value_or(-1));
}

// === EXAMPLE 4: close() WAKES EVERY WAITING CONSUMER ===

void example4()
{
    std::println("=== EXAMPLE 4: close() WAKES ALL CONSUMERS ===");

    BlockingQueue<int> queue;
    std::atomic<int>   ended{0};

    std::vector<std::thread> consumers;
    for (int i = 0; i < 3; ++i) {
        consumers.emplace_back([&] {
            while (queue.pop()) {              // ends when pop() returns an empty optional
            }
            ended.fetch_add(1);
        });
    }

    queue.close();
    for (auto& t : consumers) {
        t.join();
    }

    std::println("consumers that ended: {}", ended.load());
}

// === EXAMPLE 5: ITEMS BEFORE close() ARE STILL DELIVERED ===

void example5()
{
    std::println("=== EXAMPLE 5: DRAIN AFTER close() ===");

    BlockingQueue<int> queue;
    (void)queue.push(1);
    (void)queue.push(2);
    queue.close();

    std::println("push after close succeeded: {}", queue.push(3));
    int first  = *queue.pop();
    int second = *queue.pop();
    std::println("pop: {} {}", first, second);
    std::println("pop on a closed, empty queue has a value: {}", queue.pop().has_value());
}

// === EXAMPLE 6: NO SIZE LIMIT ===

void example6()
{
    std::println("=== EXAMPLE 6: NO SIZE LIMIT ===");

    BlockingQueue<int> queue;
    for (int i = 0; i < 100000; ++i) {
        (void)queue.push(i);                   // never waits, even without a consumer
    }
    std::println("items waiting in the queue: {}", queue.size());
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
    std::println("");
    example5();
    std::println("");
    example6();
    return 0;
}
