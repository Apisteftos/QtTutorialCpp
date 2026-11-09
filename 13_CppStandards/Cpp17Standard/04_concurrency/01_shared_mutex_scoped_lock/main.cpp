/*
 * std::shared_mutex, std::shared_lock, std::scoped_lock,
 * std::hardware_destructive_interference_size (C++17)
 *
 * Compile: g++ -std=c++17 -Wall -Wextra -pthread main.cpp -o example
 */

#include <atomic>
#include <iostream>
#include <map>
#include <mutex>
#include <new>
#include <shared_mutex>
#include <string>
#include <thread>
#include <vector>

class Registry {
public:
    void set(const std::string& key, int value) {
        std::unique_lock lock(mutex_);     // exclusive: one writer
        data_[key] = value;
    }
    int get(const std::string& key) const {
        std::shared_lock lock(mutex_);     // shared: many readers at once
        auto it = data_.find(key);
        return it == data_.end() ? -1 : it->second;
    }

private:
    mutable std::shared_mutex mutex_;
    std::map<std::string, int> data_;
};

struct Account {
    std::mutex mutex;
    int balance = 100;
};

void transfer(Account& from, Account& to, int amount) {
    std::scoped_lock lock(from.mutex, to.mutex);   // locks both without a deadlock
    from.balance -= amount;
    to.balance += amount;
}

int main() {
    // === EXAMPLE 1: shared_mutex ===
    std::cout << "=== Example 1: shared_mutex ===\n";
    Registry registry;
    registry.set("answer", 42);
    std::vector<std::thread> readers;
    std::atomic<int> sum{0};
    for (int i = 0; i < 4; ++i) {
        readers.emplace_back([&] { sum += registry.get("answer"); });
    }
    for (auto& t : readers) {
        t.join();
    }
    std::cout << "4 readers read the value, sum = " << sum << "\n";

    // === EXAMPLE 2: scoped_lock with two mutexes ===
    std::cout << "\n=== Example 2: scoped_lock ===\n";
    Account a;
    Account b;
    std::thread t1([&] { for (int i = 0; i < 1000; ++i) transfer(a, b, 1); });
    std::thread t2([&] { for (int i = 0; i < 1000; ++i) transfer(b, a, 1); });   // opposite order
    t1.join();
    t2.join();
    std::cout << "balances: " << a.balance << " and " << b.balance << " (total " << a.balance + b.balance << ")\n";

    // === EXAMPLE 3: avoiding false sharing ===
    std::cout << "\n=== Example 3: interference size ===\n";
#ifdef __cpp_lib_hardware_interference_size
    struct alignas(std::hardware_destructive_interference_size) PaddedCounter {
        std::atomic<int> value{0};
    };
    std::cout << "destructive interference size: " << std::hardware_destructive_interference_size << " bytes\n";
    std::cout << "sizeof(PaddedCounter): " << sizeof(PaddedCounter) << "\n";
#else
    std::cout << "hardware_destructive_interference_size is not available\n";
#endif
    return 0;
}
