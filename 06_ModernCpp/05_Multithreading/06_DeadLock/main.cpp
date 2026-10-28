#include <thread>
#include <mutex>
#include <chrono>
#include <string>
#include <print>

using namespace std::chrono_literals;

std::mutex g_coutMutex;
void safePrint(const std::string& msg) {
    std::lock_guard<std::mutex> lock(g_coutMutex);
    std::println("{}", msg);
}

std::mutex g_mutexA, g_mutexB;

// Fix 1 — consistent ordering
void thread1_ordered() {
    std::lock_guard<std::mutex> lA(g_mutexA);
    std::this_thread::sleep_for(5ms);
    std::lock_guard<std::mutex> lB(g_mutexB);
    safePrint("[ordered T1] holds A and B");
}
void thread2_ordered() {
    std::lock_guard<std::mutex> lA(g_mutexA);
    std::this_thread::sleep_for(5ms);
    std::lock_guard<std::mutex> lB(g_mutexB);
    safePrint("[ordered T2] holds A and B");
}

// Fix 2 — scoped_lock
void thread1_scoped() {
    std::scoped_lock lock(g_mutexA, g_mutexB);
    safePrint("[scoped T1] holds A and B atomically");
    std::this_thread::sleep_for(10ms);
}
void thread2_scoped() {
    std::scoped_lock lock(g_mutexB, g_mutexA);   // different order — still safe
    safePrint("[scoped T2] holds B and A atomically");
    std::this_thread::sleep_for(10ms);
}

// Fix 3 — std::lock + adopt_lock (C++11)
void thread1_stdlock() {
    std::lock(g_mutexA, g_mutexB);
    std::lock_guard<std::mutex> lgA(g_mutexA, std::adopt_lock);
    std::lock_guard<std::mutex> lgB(g_mutexB, std::adopt_lock);
    safePrint("[stdlock T1] holds A and B via std::lock");
}
void thread2_stdlock() {
    std::lock(g_mutexB, g_mutexA);
    std::lock_guard<std::mutex> lgB(g_mutexB, std::adopt_lock);
    std::lock_guard<std::mutex> lgA(g_mutexA, std::adopt_lock);
    safePrint("[stdlock T2] holds B and A via std::lock");
}

// Self-deadlock demo
void selfDeadlockDemo() {
    safePrint("[self-deadlock] std::mutex would deadlock if locked twice");
    safePrint("[self-deadlock] fix: use std::recursive_mutex");
}

// Livelock simulation
std::mutex g_resourceMutex;
bool g_resourceAvailable = true;
void livelockThread(int id, int attempts) {
    for (int i = 0; i < attempts; ++i) {
        if (g_resourceMutex.try_lock()) {
            if (g_resourceAvailable) {
                g_resourceAvailable = false;
                safePrint("[livelock " + std::to_string(id) + "] acquired resource");
                std::this_thread::sleep_for(5ms);
                g_resourceAvailable = true;
                g_resourceMutex.unlock();
                return;
            }
            g_resourceMutex.unlock();
        }
        std::this_thread::yield();
    }
    safePrint("[livelock " + std::to_string(id) + "] gave up after "
              + std::to_string(attempts) + " attempts");
}

// Exception safety
std::mutex g_exMutex;
int        g_criticalData = 0;
void safeWithException() {
    std::lock_guard<std::mutex> lock(g_exMutex);
    g_criticalData++;
    // even if exception thrown here — RAII unlocks
}

int main()
{
    std::println("=====================================================");
    std::println("Example 1: Fix 1 — Consistent lock ordering");
    std::println("=====================================================");
    std::thread t1(thread1_ordered);
    std::thread t2(thread2_ordered);
    t1.join(); t2.join();
    std::println("No deadlock");

    std::println("");
    std::println("=====================================================");
    std::println("Example 2: Fix 2 — std::scoped_lock (preferred)");
    std::println("=====================================================");
    std::thread t3(thread1_scoped);
    std::thread t4(thread2_scoped);
    t3.join(); t4.join();
    std::println("Different lock orders — scoped_lock still safe");

    std::println("");
    std::println("=====================================================");
    std::println("Example 3: Fix 3 — std::lock() + adopt_lock (C++11)");
    std::println("=====================================================");
    std::thread t5(thread1_stdlock);
    std::thread t6(thread2_stdlock);
    t5.join(); t6.join();

    std::println("");
    std::println("=====================================================");
    std::println("Example 4: Self-deadlock");
    std::println("=====================================================");
    std::thread t7(selfDeadlockDemo);
    t7.join();

    std::println("");
    std::println("=====================================================");
    std::println("Example 5: Livelock simulation");
    std::println("=====================================================");
    g_resourceAvailable = true;
    std::thread ll1(livelockThread, 1, 10);
    std::thread ll2(livelockThread, 2, 10);
    ll1.join(); ll2.join();

    std::println("");
    std::println("=====================================================");
    std::println("Example 6: Exception safety — RAII lock");
    std::println("=====================================================");
    std::thread t8(safeWithException);
    t8.join();
    std::println("lock_guard always unlocks — even with exception");

    std::println("");
    std::println("=== Deadlock complete ===");
    return 0;
}
