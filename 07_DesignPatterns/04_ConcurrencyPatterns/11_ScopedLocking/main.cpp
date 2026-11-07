// ============================================================
// Scoped Locking - tie a lock to a scope (RAII) so it is always released
//
// Topics:
//   1. Manual lock()/unlock() leaks the lock when an exception is thrown
//   2. std::lock_guard releases the lock on every exit path
//   3. std::scoped_lock locks several mutexes without deadlock
//
// See Readme.md for the diagrams and the explanation.
// Build: g++ -std=c++23 -Wall -Wextra -pthread main.cpp -o scoped_locking
// ============================================================

#include <mutex>
#include <print>
#include <stdexcept>
#include <thread>

// Ask from ANOTHER thread whether a mutex is free. A thread that already
// owns a std::mutex must not call try_lock on it, so main cannot test itself.
bool isFreeSeenFromOtherThread(std::mutex& m)
{
    bool free = false;
    std::thread probe([&] {
        free = m.try_lock();
        if (free) {
            m.unlock();
        }
    });
    probe.join();
    return free;
}

// === EXAMPLE 1: MANUAL LOCK AND UNLOCK ===

void riskyManual(std::mutex& m, bool fail)
{
    m.lock();
    if (fail) {
        throw std::runtime_error("something went wrong");   // unlock() is skipped!
    }
    m.unlock();
}

void example1()
{
    std::println("=== EXAMPLE 1: MANUAL LOCK LEAKS ===");

    std::mutex m;
    try {
        riskyManual(m, true);
    } catch (const std::exception& e) {
        std::println("caught: {}", e.what());
    }

    std::println("mutex is free after the exception: {}", isFreeSeenFromOtherThread(m));
    m.unlock();                                              // clean up by hand
}

// === EXAMPLE 2: LOCK_GUARD ===

void riskyScoped(std::mutex& m, bool fail)
{
    std::lock_guard lock(m);                                 // locked here
    if (fail) {
        throw std::runtime_error("something went wrong");    // destructor unlocks
    }
}                                                            // destructor unlocks

void example2()
{
    std::println("=== EXAMPLE 2: lock_guard RELEASES ALWAYS ===");

    std::mutex m;
    try {
        riskyScoped(m, true);
    } catch (const std::exception& e) {
        std::println("caught: {}", e.what());
    }

    std::println("mutex is free after the exception: {}", isFreeSeenFromOtherThread(m));
}

// === EXAMPLE 3: SCOPED_LOCK FOR SEVERAL MUTEXES ===

struct Account {
    std::mutex mutex;
    int        balance;
};

void transfer(Account& from, Account& to, int amount)
{
    // Locks both mutexes with a deadlock-avoidance algorithm, whatever order
    // the caller uses. Locking them one by one can deadlock (see the
    // DeadLock example in 05_Multithreading).
    std::scoped_lock lock(from.mutex, to.mutex);
    from.balance -= amount;
    to.balance += amount;
}

void example3()
{
    std::println("=== EXAMPLE 3: scoped_lock WITH TWO MUTEXES ===");

    Account a{{}, 1000};
    Account b{{}, 1000};

    std::thread t1([&] { for (int i = 0; i < 5000; ++i) transfer(a, b, 1); });
    std::thread t2([&] { for (int i = 0; i < 5000; ++i) transfer(b, a, 1); });   // opposite order
    t1.join();
    t2.join();

    std::println("balances: a = {}, b = {}, total = {}", a.balance, b.balance, a.balance + b.balance);
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
