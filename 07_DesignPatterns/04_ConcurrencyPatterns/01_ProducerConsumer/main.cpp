// ============================================================
// Producer-Consumer - decouple the thread that creates work from the
// thread that handles it, with a bounded queue in between
//
// Topics:
//   1. One producer, one consumer
//   2. Several producers and several consumers
//   3. Back pressure: a full queue blocks the producer
//
// See Readme.md for the diagrams and the explanation.
// Build: g++ -std=c++23 -Wall -Wextra -pthread main.cpp -o producer_consumer
// ============================================================

#include <condition_variable>
#include <cstddef>
#include <deque>
#include <mutex>
#include <optional>
#include <print>
#include <thread>
#include <vector>

// The shared buffer. pop() returns an empty optional once the queue is
// closed and drained, which tells the consumer to stop.
template <typename T>
class BoundedQueue {
public:
    explicit BoundedQueue(std::size_t capacity) : capacity_(capacity) {}

    void push(T value)
    {
        std::unique_lock lock(mutex_);
        notFull_.wait(lock, [this] { return items_.size() < capacity_; });
        items_.push_back(std::move(value));
        notEmpty_.notify_one();
    }

    std::optional<T> pop()
    {
        std::unique_lock lock(mutex_);
        notEmpty_.wait(lock, [this] { return !items_.empty() || closed_; });
        if (items_.empty()) {
            return std::nullopt;
        }
        T value = std::move(items_.front());
        items_.pop_front();
        notFull_.notify_one();
        return value;
    }

    void close()
    {
        {
            std::lock_guard lock(mutex_);
            closed_ = true;
        }
        notEmpty_.notify_all();
    }

private:
    std::mutex              mutex_;
    std::condition_variable notEmpty_;
    std::condition_variable notFull_;
    std::deque<T>           items_;
    std::size_t             capacity_;
    bool                    closed_ = false;
};

// === EXAMPLE 1: ONE PRODUCER, ONE CONSUMER ===

void example1()
{
    std::println("=== EXAMPLE 1: ONE PRODUCER, ONE CONSUMER ===");

    BoundedQueue<int> queue(4);
    long long         sum   = 0;
    int               count = 0;

    std::thread consumer([&] {
        while (auto item = queue.pop()) {
            sum += *item;
            ++count;
        }
    });

    std::thread producer([&] {
        for (int i = 1; i <= 100; ++i) {
            queue.push(i);
        }
        queue.close();
    });

    producer.join();
    consumer.join();

    std::println("consumed {} items, sum = {}", count, sum);
}

// === EXAMPLE 2: SEVERAL PRODUCERS AND CONSUMERS ===

void example2()
{
    std::println("=== EXAMPLE 2: 3 PRODUCERS, 2 CONSUMERS ===");

    BoundedQueue<int> queue(8);

    constexpr int producers     = 3;
    constexpr int itemsEach     = 1000;
    constexpr int consumerCount = 2;

    std::vector<long long> partialSums(consumerCount, 0);
    std::vector<int>       partialCounts(consumerCount, 0);

    std::vector<std::thread> consumers;
    for (int c = 0; c < consumerCount; ++c) {
        consumers.emplace_back([&, c] {
            while (auto item = queue.pop()) {
                partialSums[c] += *item;
                ++partialCounts[c];
            }
        });
    }

    std::vector<std::thread> producerThreads;
    for (int p = 0; p < producers; ++p) {
        producerThreads.emplace_back([&] {
            for (int i = 1; i <= itemsEach; ++i) {
                queue.push(i);
            }
        });
    }

    for (auto& t : producerThreads) {
        t.join();
    }
    queue.close();                 // only after ALL producers are done
    for (auto& t : consumers) {
        t.join();
    }

    long long total      = partialSums[0] + partialSums[1];
    int       totalCount = partialCounts[0] + partialCounts[1];
    std::println("consumed {} items (expected {})", totalCount, producers * itemsEach);
    std::println("sum = {} (expected {})", total,
                 static_cast<long long>(producers) * itemsEach * (itemsEach + 1) / 2);
}

// === EXAMPLE 3: BACK PRESSURE ===

void example3()
{
    std::println("=== EXAMPLE 3: BACK PRESSURE ===");

    // Capacity 1: the producer can be at most one item ahead of the consumer.
    BoundedQueue<int> queue(1);
    int               maxAhead = 0;
    int               produced = 0;
    int               consumed = 0;
    std::mutex        statsMutex;

    std::thread consumer([&] {
        while (auto item = queue.pop()) {
            std::lock_guard lock(statsMutex);
            ++consumed;
            (void)item;
        }
    });

    std::thread producer([&] {
        for (int i = 0; i < 1000; ++i) {
            queue.push(i);
            std::lock_guard lock(statsMutex);
            ++produced;
            maxAhead = std::max(maxAhead, produced - consumed);
        }
        queue.close();
    });

    producer.join();
    consumer.join();

    // 1 item in the queue plus 1 the consumer has taken but not counted yet.
    std::println("producer was never more than 2 items ahead: {}", maxAhead <= 2);
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
