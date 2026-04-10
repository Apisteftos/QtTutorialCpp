#include <iostream>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <atomic>
#include <vector>
#include <chrono>
#include <string>
#include <syncstream>   // C++20 osyncstream

using namespace std::chrono_literals;

// ============================================================
// THREAD SYNCHRONIZATION in C++23
// ============================================================
// Topics covered:
//   1. The problem — race condition without synchronization
//   2. std::mutex + std::lock_guard
//   3. std::mutex + std::unique_lock
//   4. std::recursive_mutex
//   5. std::timed_mutex
//   6. std::shared_mutex (readers-writer lock)
//   7. std::scoped_lock (multiple mutexes, C++17)
//   8. std::condition_variable — producer/consumer
//   9. std::atomic<T> — lock-free synchronization
//  10. std::osyncstream — synchronized cout (C++20)
// ============================================================


// ============================================================
// Shared state used across examples
// ============================================================
std::mutex          g_mutex;
std::shared_mutex   g_sharedMutex;
std::atomic<int>    g_atomicCounter{0};


// ============================================================
// Example 1 — Race condition (NO synchronization)
// ============================================================
int g_unsafeCounter = 0;

void unsafeIncrement(int iterations)
{
    for (int i = 0; i < iterations; ++i)
        g_unsafeCounter++;   // READ-MODIFY-WRITE — not atomic!
}


// ============================================================
// Example 2 — std::mutex + std::lock_guard (RAII lock)
// ============================================================
int g_safeCounter = 0;

void safeIncrement(int iterations)
{
    for (int i = 0; i < iterations; ++i) {
        std::lock_guard<std::mutex> lock(g_mutex);   // locks here
        g_safeCounter++;
    }   // lock released automatically when lock goes out of scope
}


// ============================================================
// Example 3 — std::unique_lock (flexible lock)
// ============================================================
int g_uniqueCounter = 0;
std::mutex g_uniqueMutex;

void uniqueLockExample(int id)
{
    std::unique_lock<std::mutex> lock(g_uniqueMutex);   // locks immediately

    g_uniqueCounter++;
    std::cout << "[unique_lock " << id << "] counter = " << g_uniqueCounter << '\n';

    lock.unlock();    // manually unlock — unique_lock allows this
    // do work that doesn't need the lock
    std::this_thread::sleep_for(10ms);

    lock.lock();      // re-lock when needed
    std::cout << "[unique_lock " << id << "] back with lock\n";
}   // lock released here


// ============================================================
// Example 4 — std::recursive_mutex
// A thread can lock it multiple times without deadlocking
// ============================================================
std::recursive_mutex g_recursiveMutex;

void recursiveFunction(int depth)
{
    std::lock_guard<std::recursive_mutex> lock(g_recursiveMutex);
    std::cout << "[recursive] depth = " << depth << '\n';
    if (depth > 0)
        recursiveFunction(depth - 1);   // same thread locks again — OK with recursive_mutex
}


// ============================================================
// Example 5 — std::timed_mutex
// Try to lock with a timeout — don't wait forever
// ============================================================
std::timed_mutex g_timedMutex;

void timedLockWorker(int id)
{
    if (g_timedMutex.try_lock_for(50ms)) {
        std::cout << "[timed " << id << "] acquired lock\n";
        std::this_thread::sleep_for(30ms);
        g_timedMutex.unlock();
    } else {
        std::cout << "[timed " << id << "] could not acquire lock — timeout!\n";
    }
}


// ============================================================
// Example 6 — std::shared_mutex (readers-writer lock)
// Multiple readers can hold the lock simultaneously
// Writers get exclusive access
// ============================================================
std::string g_sharedData = "initial value";
int         g_readCount  = 0;

void reader(int id)
{
    std::shared_lock<std::shared_mutex> lock(g_sharedMutex);
    g_readCount++;
    std::cout << "[reader " << id << "] reading: '"
              << g_sharedData << "' (concurrent readers: " << g_readCount << ")\n";
    std::this_thread::sleep_for(20ms);
    g_readCount--;
}

void writer(int id, const std::string& newValue)
{
    std::unique_lock<std::shared_mutex> lock(g_sharedMutex);
    std::cout << "[writer " << id << "] writing: '" << newValue << "'\n";
    g_sharedData = newValue;
    std::this_thread::sleep_for(10ms);
}


// ============================================================
// Example 7 — std::scoped_lock (multiple mutexes, C++17)
// Acquires all locks atomically — prevents deadlock
// ============================================================
std::mutex g_mutexA;
std::mutex g_mutexB;

void taskA()
{
    std::scoped_lock lock(g_mutexA, g_mutexB);
    std::cout << "[taskA] holding mutexA and mutexB\n";
    std::this_thread::sleep_for(10ms);
}

void taskB()
{
    std::scoped_lock lock(g_mutexA, g_mutexB);
    std::cout << "[taskB] holding mutexA and mutexB\n";
    std::this_thread::sleep_for(10ms);
}


// ============================================================
// Example 8 — std::condition_variable
// Producer / Consumer pattern
// ============================================================
std::mutex              g_cvMutex;
std::condition_variable g_cv;
std::vector<int>        g_queue;
bool                    g_done = false;

void producer()
{
    for (int i = 1; i <= 5; ++i) {
        std::this_thread::sleep_for(20ms);
        {
            std::lock_guard<std::mutex> lock(g_cvMutex);
            g_queue.push_back(i);
            std::cout << "[producer] produced: " << i << '\n';
        }
        g_cv.notify_one();
    }
    {
        std::lock_guard<std::mutex> lock(g_cvMutex);
        g_done = true;
    }
    g_cv.notify_all();
}

void consumer(int id)
{
    while (true) {
        std::unique_lock<std::mutex> lock(g_cvMutex);

        g_cv.wait(lock, []() {
            return !g_queue.empty() || g_done;
        });

        while (!g_queue.empty()) {
            int value = g_queue.front();
            g_queue.erase(g_queue.begin());
            lock.unlock();
            std::cout << "[consumer " << id << "] consumed: " << value << '\n';
            lock.lock();
        }

        if (g_done && g_queue.empty())
            break;
    }
}


// ============================================================
// Example 9 — std::atomic<T>
// Lock-free synchronization for simple types
// ============================================================
void atomicIncrement(int iterations)
{
    for (int i = 0; i < iterations; ++i)
        g_atomicCounter++;
}

std::atomic<bool> g_flag{false};

void waitForFlag()
{
    while (!g_flag.load())
        std::this_thread::yield();
    std::cout << "[waiter] flag was set!\n";
}

void setFlag()
{
    std::this_thread::sleep_for(50ms);
    g_flag.store(true);
    std::cout << "[setter] flag set\n";
}


// ============================================================
// Example 10 — std::osyncstream (C++20)
// Synchronized output — no mutex needed for cout
// ============================================================
void syncPrint(int id)
{
    for (int i = 0; i < 3; ++i) {
        std::osyncstream(std::cout)
            << "[osyncstream " << id << "] line " << i << '\n';
        std::this_thread::sleep_for(5ms);
    }
}


// ============================================================
// MAIN
// ============================================================
int main()
{
    // -------------------------------------------------------
    std::cout << "=====================================================\n";
    std::cout << "Example 1: Race Condition — NO synchronization\n";
    std::cout << "=====================================================\n";

    g_unsafeCounter = 0;
    std::thread u1(unsafeIncrement, 100000);
    std::thread u2(unsafeIncrement, 100000);
    u1.join(); u2.join();

    std::cout << "Expected: 200000\n";
    std::cout << "Actual:   " << g_unsafeCounter << '\n';
    std::cout << "(result is non-deterministic — run multiple times)\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: std::mutex + std::lock_guard\n";
    std::cout << "=====================================================\n";

    g_safeCounter = 0;
    std::thread s1(safeIncrement, 100000);
    std::thread s2(safeIncrement, 100000);
    s1.join(); s2.join();

    std::cout << "Expected: 200000\n";
    std::cout << "Actual:   " << g_safeCounter << " (always correct)\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: std::unique_lock (manual lock/unlock)\n";
    std::cout << "=====================================================\n";

    g_uniqueCounter = 0;
    std::thread ul1(uniqueLockExample, 1);
    std::thread ul2(uniqueLockExample, 2);
    ul1.join(); ul2.join();


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: std::recursive_mutex\n";
    std::cout << "=====================================================\n";

    std::thread rm(recursiveFunction, 3);
    rm.join();


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: std::timed_mutex\n";
    std::cout << "=====================================================\n";

    g_timedMutex.lock();
    std::thread tm1(timedLockWorker, 1);
    std::thread tm2(timedLockWorker, 2);
    std::this_thread::sleep_for(100ms);
    g_timedMutex.unlock();
    tm1.join(); tm2.join();


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: std::shared_mutex (readers-writer lock)\n";
    std::cout << "=====================================================\n";

    g_readCount = 0;
    std::vector<std::thread> rwThreads;
    rwThreads.emplace_back(reader, 1);
    rwThreads.emplace_back(reader, 2);
    rwThreads.emplace_back(reader, 3);
    rwThreads.emplace_back(writer, 1, "updated value");
    rwThreads.emplace_back(reader, 4);
    rwThreads.emplace_back(reader, 5);
    for (auto& t : rwThreads) t.join();


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: std::scoped_lock (deadlock-free multi-lock)\n";
    std::cout << "=====================================================\n";

    std::thread ta(taskA);
    std::thread tb(taskB);
    ta.join(); tb.join();
    std::cout << "No deadlock — scoped_lock acquired both atomically\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: std::condition_variable (producer/consumer)\n";
    std::cout << "=====================================================\n";

    g_done = false;
    g_queue.clear();
    std::thread prod(producer);
    std::thread cons1(consumer, 1);
    std::thread cons2(consumer, 2);
    prod.join(); cons1.join(); cons2.join();


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 9: std::atomic\n";
    std::cout << "=====================================================\n";

    g_atomicCounter = 0;
    std::thread a1(atomicIncrement, 100000);
    std::thread a2(atomicIncrement, 100000);
    a1.join(); a2.join();
    std::cout << "atomic counter expected: 200000\n";
    std::cout << "atomic counter actual:   " << g_atomicCounter << " (always correct)\n";

    std::thread waiter(waitForFlag);
    std::thread setter(setFlag);
    waiter.join(); setter.join();


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 10: std::osyncstream (synchronized cout)\n";
    std::cout << "=====================================================\n";

    std::vector<std::thread> printThreads;
    for (int i = 1; i <= 4; ++i)
        printThreads.emplace_back(syncPrint, i);
    for (auto& t : printThreads) t.join();
    std::cout << "(no garbled output — each line printed atomically)\n";


    std::cout << "\n=== ThreadSynchronization complete ===\n";
    return 0;
}
