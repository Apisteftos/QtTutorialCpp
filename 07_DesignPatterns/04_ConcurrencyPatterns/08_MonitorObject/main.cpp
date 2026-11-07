// ============================================================
// Monitor Object - one object protects its own state: every public method
// takes the same lock, and waiting happens inside the object
//
// Topics:
//   1. A class whose methods are all synchronized
//   2. Blocking inside the monitor with a condition variable
//   3. Waiting with a timeout
//
// See Readme.md for the diagrams and the explanation.
// Build: g++ -std=c++23 -Wall -Wextra -pthread main.cpp -o monitor_object
// ============================================================

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <print>
#include <thread>

class Inventory {
public:
    void add(int amount)
    {
        {
            std::lock_guard lock(mutex_);
            stock_ += amount;
        }
        changed_.notify_all();             // wake threads waiting in take()
    }

    // Blocks until enough stock is available.
    void take(int amount)
    {
        std::unique_lock lock(mutex_);
        changed_.wait(lock, [&] { return stock_ >= amount; });
        stock_ -= amount;
    }

    // Gives up after the timeout.
    bool takeFor(int amount, std::chrono::milliseconds timeout)
    {
        std::unique_lock lock(mutex_);
        if (!changed_.wait_for(lock, timeout, [&] { return stock_ >= amount; })) {
            return false;
        }
        stock_ -= amount;
        return true;
    }

    int stock() const
    {
        std::lock_guard lock(mutex_);
        return stock_;
    }

private:
    mutable std::mutex      mutex_;        // the monitor lock
    std::condition_variable changed_;      // the monitor condition
    int                     stock_ = 0;
};

// === EXAMPLE 1: ALL METHODS SYNCHRONIZED ===

void example1()
{
    std::println("=== EXAMPLE 1: SYNCHRONIZED METHODS ===");

    Inventory inventory;

    std::thread a([&] { for (int i = 0; i < 1000; ++i) inventory.add(1); });
    std::thread b([&] { for (int i = 0; i < 1000; ++i) inventory.add(1); });
    a.join();
    b.join();

    std::println("stock after 2 x 1000 adds = {}", inventory.stock());
}

// === EXAMPLE 2: BLOCKING INSIDE THE MONITOR ===

void example2()
{
    std::println("=== EXAMPLE 2: take() WAITS FOR STOCK ===");

    Inventory inventory;

    // The consumer wants 5 units but the stock is 0, so take() blocks.
    std::thread consumer([&] { inventory.take(5); });

    for (int i = 0; i < 5; ++i) {
        inventory.add(1);                  // each add() wakes the consumer to re-check
    }
    consumer.join();

    std::println("consumer got its 5 units, stock left = {}", inventory.stock());
}

// === EXAMPLE 3: WAITING WITH A TIMEOUT ===

void example3()
{
    std::println("=== EXAMPLE 3: TIMEOUT ===");

    Inventory inventory;

    bool ok = inventory.takeFor(3, std::chrono::milliseconds(20));
    std::println("takeFor(3) with empty stock succeeded: {}", ok);

    inventory.add(3);
    ok = inventory.takeFor(3, std::chrono::milliseconds(20));
    std::println("takeFor(3) after add(3) succeeded:     {}", ok);
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
