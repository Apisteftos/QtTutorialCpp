#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <string>

using namespace std::chrono_literals;

std::mutex g_coutMutex;
void safePrint(const std::string& msg) {
    std::lock_guard<std::mutex> lock(g_coutMutex);
    std::cout << msg << '\n';
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
    std::cout << "=====================================================\n";
    std::cout << "Example 1: Classic Deadlock (shown in comments)\n";
    std::cout << "=====================================================\n";
    std::cout << "T1: lock(A) -> sleep -> lock(B)  <- waits forever\n";
    std::cout << "T2: lock(B) -> sleep -> lock(A)  <- waits forever\n";
    std::cout << "(not executed — would hang the program)\n";

    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: Fix 1 — Consistent lock ordering\n";
    std::cout << "=====================================================\n";
    std::thread t1(thread1_ordered);
    std::thread t2(thread2_ordered);
    t1.join(); t2.join();
    std::cout << "No deadlock\n";

    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: Fix 2 — std::scoped_lock (preferred)\n";
    std::cout << "=====================================================\n";
    std::thread t3(thread1_scoped);
    std::thread t4(thread2_scoped);
    t3.join(); t4.join();
    std::cout << "Different lock orders — scoped_lock still safe\n";

    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: Fix 3 — std::lock() + adopt_lock (C++11)\n";
    std::cout << "=====================================================\n";
    std::thread t5(thread1_stdlock);
    std::thread t6(thread2_stdlock);
    t5.join(); t6.join();

    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: Self-deadlock\n";
    std::cout << "=====================================================\n";
    std::thread t7(selfDeadlockDemo);
    t7.join();

    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: Livelock simulation\n";
    std::cout << "=====================================================\n";
    g_resourceAvailable = true;
    std::thread ll1(livelockThread, 1, 10);
    std::thread ll2(livelockThread, 2, 10);
    ll1.join(); ll2.join();

    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: Exception safety — RAII lock\n";
    std::cout << "=====================================================\n";
    std::thread t8(safeWithException);
    t8.join();
    std::cout << "lock_guard always unlocks — even with exception\n";

    std::cout << "\n=== Deadlock complete ===\n";
    return 0;
}
