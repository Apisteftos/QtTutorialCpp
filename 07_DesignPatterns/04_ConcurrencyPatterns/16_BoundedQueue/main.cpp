// ============================================================
// Bounded Queue - a blocking queue with a size limit
//
// Both sides can wait:
//   push() waits while the queue is FULL   (back pressure on the producer)
//   pop()  waits while the queue is EMPTY  (the consumer has nothing to do)
//
// For a queue without a limit see ../15_BlockingQueue.
//
// Topics:
//   1. try_push / try_pop never wait
//   2. push() waits for room, the size never exceeds the capacity
//   3. push_for(): wait for room with a timeout
//   4. close() wakes a producer that is stuck on a full queue
//
// See Readme.md for the diagrams and the explanation.
// Build: g++ -std=c++23 -Wall -Wextra -pthread main.cpp -o bounded_queue
// ============================================================

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
class BoundedQueue {
public:
    explicit BoundedQueue(std::size_t capacity) : capacity_(capacity) {}

    // Waits while the queue is full. Returns false if the queue was closed.
    bool push(T value)
    {
        std::unique_lock lock(mutex_);
        notFull_.wait(lock, [this] { return items_.size() < capacity_ || closed_; });
        return put(std::move(value));
    }

    // Does not wait. Returns false if the queue is full or closed.
    bool try_push(T value)
    {
        std::lock_guard lock(mutex_);
        if (items_.size() >= capacity_) {
            return false;
        }
        return put(std::move(value));
    }

    // Waits at most 'timeout' for room. Returns false on timeout or when closed.
    template <typename Rep, typename Period>
    bool push_for(T value, std::chrono::duration<Rep, Period> timeout)
    {
        std::unique_lock lock(mutex_);
        if (!notFull_.wait_for(lock, timeout, [this] { return items_.size() < capacity_ || closed_; })) {
            return false;
        }
        return put(std::move(value));
    }

    // Waits while the queue is empty. Empty optional when closed and drained.
    std::optional<T> pop()
    {
        std::unique_lock lock(mutex_);
        notEmpty_.wait(lock, [this] { return !items_.empty() || closed_; });
        return take();
    }

    // Does not wait.
    std::optional<T> try_pop()
    {
        std::lock_guard lock(mutex_);
        return take();
    }

    // Wakes ALL waiting threads, producers and consumers.
    void close()
    {
        {
            std::lock_guard lock(mutex_);
            closed_ = true;
        }
        notEmpty_.notify_all();
        notFull_.notify_all();
    }

    std::size_t size() const
    {
        std::lock_guard lock(mutex_);
        return items_.size();
    }

    std::size_t capacity() const { return capacity_; }

private:
    // The caller holds the lock and has checked that there is room (or closed).
    bool put(T value)
    {
        if (closed_) {
            return false;
        }
        items_.push_back(std::move(value));
        notEmpty_.notify_one();
        return true;
    }

    std::optional<T> take()
    {
        if (items_.empty()) {
            return std::nullopt;
        }
        T value = std::move(items_.front());
        items_.pop_front();
        notFull_.notify_one();
        return value;
    }

    mutable std::mutex      mutex_;
    std::condition_variable notEmpty_;
    std::condition_variable notFull_;
    std::deque<T>           items_;
    std::size_t             capacity_;
    bool                    closed_ = false;
};

// === EXAMPLE 1: try_push AND try_pop NEVER WAIT ===

void example1()
{
    std::println("=== EXAMPLE 1: try_push / try_pop ===");

    BoundedQueue<int> queue(2);
    std::println("try_push(1): {}", queue.try_push(1));
    std::println("try_push(2): {}", queue.try_push(2));
    std::println("try_push(3) on a full queue: {}", queue.try_push(3));

    int first  = *queue.try_pop();
    int second = *queue.try_pop();
    std::println("try_pop: {} {}", first, second);
    std::println("try_pop on an empty queue has a value: {}", queue.try_pop().has_value());
}

// === EXAMPLE 2: push() WAITS FOR ROOM ===

void example2()
{
    std::println("=== EXAMPLE 2: BACK PRESSURE ===");

    BoundedQueue<int> queue(2);
    bool              neverTooBig = true;
    std::vector<int>  received;

    std::thread producer([&] {
        for (int i = 1; i <= 10; ++i) {
            (void)queue.push(i);               // waits whenever 2 items are already queued
            if (queue.size() > queue.capacity()) {
                neverTooBig = false;
            }
        }
        queue.close();
    });

    std::thread consumer([&] {
        while (auto item = queue.pop()) {
            received.push_back(*item);
        }
    });

    producer.join();
    consumer.join();

    std::println("received {} items, first = {}, last = {}", received.size(), received.front(), received.back());
    std::println("size never exceeded the capacity: {}", neverTooBig);
}

// === EXAMPLE 3: push_for ===

void example3()
{
    std::println("=== EXAMPLE 3: push_for ===");

    BoundedQueue<int> queue(1);
    (void)queue.push(1);                       // now full

    std::println("push_for on a full queue, 20 ms: {}", queue.push_for(99, std::chrono::milliseconds(20)));

    (void)queue.pop();                         // make room
    std::println("push_for after making room:     {}", queue.push_for(99, std::chrono::milliseconds(20)));
}

// === EXAMPLE 4: close() WAKES A STUCK PRODUCER ===

void example4()
{
    std::println("=== EXAMPLE 4: close() WAKES A PRODUCER ===");

    BoundedQueue<int> queue(3);
    int               pushed = 0;

    std::thread producer([&] {
        while (queue.push(pushed)) {           // nobody consumes: it gets stuck when full
            ++pushed;
        }
    });

    while (queue.size() < queue.capacity()) {  // wait until the queue is full
        std::this_thread::yield();
    }
    queue.close();                             // push() returns false, the producer ends
    producer.join();

    std::println("items pushed before the producer got stuck: {}", pushed);
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
