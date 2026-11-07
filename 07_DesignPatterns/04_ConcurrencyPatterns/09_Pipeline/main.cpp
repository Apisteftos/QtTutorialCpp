// ============================================================
// Pipeline - a chain of stages, each in its own thread, connected by queues
//
// Topics:
//   1. Three stages: generate -> square -> filter
//   2. Order is preserved (one thread per stage)
//   3. Closing the first queue shuts the whole chain down
//
// See Readme.md for the diagrams and the explanation.
// Build: g++ -std=c++23 -Wall -Wextra -pthread main.cpp -o pipeline
// ============================================================

#include <condition_variable>
#include <deque>
#include <mutex>
#include <optional>
#include <print>
#include <thread>
#include <vector>

// A small unbounded channel between two stages.
template <typename T>
class Channel {
public:
    void send(T value)
    {
        {
            std::lock_guard lock(mutex_);
            items_.push_back(std::move(value));
        }
        ready_.notify_one();
    }

    void close()
    {
        {
            std::lock_guard lock(mutex_);
            closed_ = true;
        }
        ready_.notify_all();
    }

    // Empty optional: closed and nothing left.
    std::optional<T> receive()
    {
        std::unique_lock lock(mutex_);
        ready_.wait(lock, [this] { return !items_.empty() || closed_; });
        if (items_.empty()) {
            return std::nullopt;
        }
        T value = std::move(items_.front());
        items_.pop_front();
        return value;
    }

private:
    std::mutex              mutex_;
    std::condition_variable ready_;
    std::deque<T>           items_;
    bool                    closed_ = false;
};

// === EXAMPLE 1: THREE STAGES ===

void example1()
{
    std::println("=== EXAMPLE 1: GENERATE -> SQUARE -> FILTER ===");

    Channel<int> numbers;      // stage 1 -> stage 2
    Channel<int> squares;      // stage 2 -> stage 3
    std::vector<int> kept;     // written only by stage 3

    {
        std::jthread stage1([&] {
            for (int i = 1; i <= 20; ++i) {
                numbers.send(i);
            }
            numbers.close();                       // tells stage 2: no more input
        });

        std::jthread stage2([&] {
            while (auto n = numbers.receive()) {
                squares.send(*n * *n);
            }
            squares.close();                       // pass the end marker on
        });

        std::jthread stage3([&] {
            while (auto s = squares.receive()) {
                if (*s % 3 == 0) {                 // keep multiples of 3
                    kept.push_back(*s);
                }
            }
        });
    }   // the jthreads join here

    std::println("kept {} values", kept.size());
    for (int v : kept) {
        std::println("  {}", v);
    }
}

// === EXAMPLE 2: ORDER IS PRESERVED ===

void example2()
{
    std::println("=== EXAMPLE 2: ORDER PRESERVED ===");

    Channel<int> in;
    Channel<int> out;
    std::vector<int> result;

    {
        std::jthread producer([&] {
            for (int i = 0; i < 10; ++i) {
                in.send(i);
            }
            in.close();
        });
        std::jthread doubler([&] {
            while (auto v = in.receive()) {
                out.send(*v * 2);
            }
            out.close();
        });
        std::jthread collector([&] {
            while (auto v = out.receive()) {
                result.push_back(*v);
            }
        });
    }

    bool ordered = true;
    for (int i = 0; i < 10; ++i) {
        ordered = ordered && result[i] == i * 2;
    }
    std::println("10 values arrived in the original order: {}", ordered);
}

// === EXAMPLE 3: SHUTDOWN WITH NO INPUT ===

void example3()
{
    std::println("=== EXAMPLE 3: CLOSING AN EMPTY PIPELINE ===");

    Channel<int> in;
    Channel<int> out;
    int          count = 0;

    {
        std::jthread middle([&] {
            while (auto v = in.receive()) {
                out.send(*v);
            }
            out.close();
        });
        std::jthread last([&] {
            while (out.receive()) {
                ++count;
            }
        });
        in.close();             // no data at all: both stages must still end
    }

    std::println("processed {} items, all stages ended", count);
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
