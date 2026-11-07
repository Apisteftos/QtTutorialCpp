// ============================================================
// Thread-Safe Interface - public methods take the lock once;
// private helpers assume the lock is already held
//
// Topics:
//   1. A public method that needs another public method (self-deadlock)
//   2. The fix: public locks, private "Unlocked" does the work
//   3. One lock for one consistent snapshot
//
// See Readme.md for the diagrams and the explanation.
// Build: g++ -std=c++23 -Wall -Wextra -pthread main.cpp -o thread_safe_interface
// ============================================================

#include <mutex>
#include <print>
#include <thread>
#include <utility>
#include <vector>

class Account {
public:
    // Public interface: lock once, call the private implementation.
    void deposit(int amount)
    {
        std::lock_guard lock(mutex_);
        depositUnlocked(amount);
    }

    void depositTwice(int amount)
    {
        std::lock_guard lock(mutex_);
        depositUnlocked(amount);          // NOT deposit(): it would lock mutex_ again
        depositUnlocked(amount);
    }

    // Both values come from ONE locked region, so they always match.
    std::pair<int, int> snapshot() const
    {
        std::lock_guard lock(mutex_);
        return {balance_, deposits_};
    }

    //! void depositTwiceBad(int amount)
    //! {
    //!     std::lock_guard lock(mutex_);
    //!     deposit(amount);              // locks mutex_ a second time: deadlock / undefined behavior
    //!     deposit(amount);
    //! }

private:
    // Private implementation: the caller must hold mutex_.
    void depositUnlocked(int amount)
    {
        balance_ += amount;
        ++deposits_;
    }

    mutable std::mutex mutex_;
    int                balance_  = 0;
    int                deposits_ = 0;
};

// === EXAMPLE 1: A PUBLIC METHOD USING SHARED PRIVATE LOGIC ===

void example1()
{
    std::println("=== EXAMPLE 1: depositTwice ===");

    Account account;
    account.depositTwice(50);

    auto [balance, deposits] = account.snapshot();
    std::println("balance = {}, deposits = {}", balance, deposits);
}

// === EXAMPLE 2: SEVERAL THREADS ===

void example2()
{
    std::println("=== EXAMPLE 2: 4 THREADS ===");

    Account account;
    std::vector<std::thread> threads;
    for (int t = 0; t < 4; ++t) {
        threads.emplace_back([&account] {
            for (int i = 0; i < 1000; ++i) {
                account.depositTwice(1);
            }
        });
    }
    for (auto& t : threads) {
        t.join();
    }

    auto [balance, deposits] = account.snapshot();
    std::println("balance = {} (expected 8000), deposits = {} (expected 8000)", balance, deposits);
}

// === EXAMPLE 3: ONE CONSISTENT SNAPSHOT ===

void example3()
{
    std::println("=== EXAMPLE 3: CONSISTENT SNAPSHOT ===");

    Account account;
    bool    consistent = true;

    std::thread writer([&] {
        for (int i = 0; i < 20000; ++i) {
            account.deposit(1);           // balance and deposits always grow together
        }
    });

    std::thread reader([&] {
        for (int i = 0; i < 20000; ++i) {
            auto [balance, deposits] = account.snapshot();
            if (balance != deposits) {    // would break with two separate getters
                consistent = false;
            }
        }
    });

    writer.join();
    reader.join();
    std::println("balance == deposits in every snapshot: {}", consistent);
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
