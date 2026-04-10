#include <iostream>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <vector>
#include <chrono>
#include <string>

using namespace std::chrono_literals;

// ============================================================
// MUTEX in C++23
// ============================================================
// Topics:
//   1. Race condition — the problem mutex solves
//   2. std::mutex + std::lock_guard
//   3. std::mutex + std::unique_lock
//   4. std::recursive_mutex
//   5. std::timed_mutex
//   6. std::shared_mutex (readers-writer lock)
//   7. std::scoped_lock  (multi-mutex, C++17)
//   8. try_lock (non-blocking)
// ============================================================

std::mutex g_coutMutex;
void safePrint(const std::string& msg)
{
    std::lock_guard<std::mutex> lock(g_coutMutex);
    std::cout << msg << '\n';
}

// ---- Example 1 ----
int g_unsafeCounter = 0;
void unsafeIncrement(int n) { for (int i=0;i<n;++i) g_unsafeCounter++; }

// ---- Example 2 ----
int        g_safeCounter = 0;
std::mutex g_safeMutex;
void safeIncrement(int n) {
    for (int i=0;i<n;++i) {
        std::lock_guard<std::mutex> lock(g_safeMutex);
        g_safeCounter++;
    }
}

// ---- Example 3 ----
int        g_bankBalance = 1000;
std::mutex g_bankMutex;
void withdraw(int amount, const std::string& who) {
    std::unique_lock<std::mutex> lock(g_bankMutex);
    if (g_bankBalance >= amount) {
        lock.unlock();
        std::this_thread::sleep_for(5ms);
        lock.lock();
        g_bankBalance -= amount;
        safePrint("[" + who + "] withdrew " + std::to_string(amount)
                  + " | balance: " + std::to_string(g_bankBalance));
    } else {
        safePrint("[" + who + "] insufficient funds (balance: "
                  + std::to_string(g_bankBalance) + ")");
    }
}

// ---- Example 4 ----
std::recursive_mutex g_recursiveMutex;
int                  g_recursiveData = 0;
void recursiveUpdate(int depth) {
    std::lock_guard<std::recursive_mutex> lock(g_recursiveMutex);
    g_recursiveData++;
    safePrint("[recursive] depth=" + std::to_string(depth)
              + " data=" + std::to_string(g_recursiveData));
    if (depth > 0) recursiveUpdate(depth - 1);
}

// ---- Example 5 ----
std::timed_mutex g_timedMutex;
void timedWorker(int id, int holdMs) {
    if (g_timedMutex.try_lock_for(std::chrono::milliseconds(50))) {
        safePrint("[timed " + std::to_string(id) + "] acquired lock");
        std::this_thread::sleep_for(std::chrono::milliseconds(holdMs));
        g_timedMutex.unlock();
        safePrint("[timed " + std::to_string(id) + "] released lock");
    } else {
        safePrint("[timed " + std::to_string(id) + "] TIMEOUT");
    }
}

// ---- Example 6 ----
std::shared_mutex g_rwMutex;
std::string       g_config = "default_config";
void configReader(int id) {
    std::shared_lock<std::shared_mutex> lock(g_rwMutex);
    safePrint("[reader " + std::to_string(id) + "] reads: '" + g_config + "'");
    std::this_thread::sleep_for(20ms);
}
void configWriter(const std::string& newConfig) {
    std::unique_lock<std::shared_mutex> lock(g_rwMutex);
    safePrint("[writer] updating to: '" + newConfig + "'");
    std::this_thread::sleep_for(10ms);
    g_config = newConfig;
}

// ---- Example 7 ----
std::mutex g_mutexAlpha, g_mutexBeta;
void taskAlpha() {
    std::scoped_lock lock(g_mutexAlpha, g_mutexBeta);
    safePrint("[taskAlpha] holds both mutexes");
    std::this_thread::sleep_for(10ms);
}
void taskBeta() {
    std::scoped_lock lock(g_mutexAlpha, g_mutexBeta);
    safePrint("[taskBeta] holds both mutexes");
    std::this_thread::sleep_for(10ms);
}

// ---- Example 8 ----
std::mutex g_tryMutex;
void tryLockWorker(int id) {
    if (g_tryMutex.try_lock()) {
        safePrint("[try " + std::to_string(id) + "] got lock — working");
        std::this_thread::sleep_for(30ms);
        g_tryMutex.unlock();
    } else {
        safePrint("[try " + std::to_string(id) + "] lock busy — doing other work");
    }
}

int main()
{
    std::cout << "=====================================================\n";
    std::cout << "Example 1: Race Condition — NO mutex\n";
    std::cout << "=====================================================\n";
    g_unsafeCounter = 0;
    std::thread u1(unsafeIncrement, 100000);
    std::thread u2(unsafeIncrement, 100000);
    u1.join(); u2.join();
    std::cout << "Expected: 200000 | Actual: " << g_unsafeCounter << " (non-deterministic!)\n";

    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: std::mutex + std::lock_guard\n";
    std::cout << "=====================================================\n";
    g_safeCounter = 0;
    std::thread s1(safeIncrement, 100000);
    std::thread s2(safeIncrement, 100000);
    s1.join(); s2.join();
    std::cout << "Expected: 200000 | Actual: " << g_safeCounter << " (always correct)\n";

    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: std::unique_lock (bank withdrawal)\n";
    std::cout << "=====================================================\n";
    g_bankBalance = 1000;
    std::thread w1(withdraw, 300, "Alice");
    std::thread w2(withdraw, 400, "Bob");
    std::thread w3(withdraw, 500, "Carol");
    w1.join(); w2.join(); w3.join();
    std::cout << "Final balance: " << g_bankBalance << '\n';

    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: std::recursive_mutex\n";
    std::cout << "=====================================================\n";
    std::thread rm(recursiveUpdate, 3);
    rm.join();

    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: std::timed_mutex\n";
    std::cout << "=====================================================\n";
    std::thread tm1(timedWorker, 1, 80);
    std::this_thread::sleep_for(5ms);
    std::thread tm2(timedWorker, 2, 20);
    std::thread tm3(timedWorker, 3, 20);
    tm1.join(); tm2.join(); tm3.join();

    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: std::shared_mutex (readers-writer)\n";
    std::cout << "=====================================================\n";
    std::vector<std::thread> rwThreads;
    for (int i=1;i<=3;++i) rwThreads.emplace_back(configReader, i);
    rwThreads.emplace_back(configWriter, "production_v2");
    for (int i=4;i<=5;++i) rwThreads.emplace_back(configReader, i);
    for (auto& t : rwThreads) t.join();

    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: std::scoped_lock (multi-mutex)\n";
    std::cout << "=====================================================\n";
    std::thread ta(taskAlpha);
    std::thread tb(taskBeta);
    ta.join(); tb.join();
    std::cout << "No deadlock — scoped_lock is safe\n";

    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: try_lock (non-blocking)\n";
    std::cout << "=====================================================\n";
    std::thread tl1(tryLockWorker, 1);
    std::thread tl2(tryLockWorker, 2);
    std::thread tl3(tryLockWorker, 3);
    tl1.join(); tl2.join(); tl3.join();

    std::cout << "\n=== Mutex complete ===\n";
    return 0;
}
