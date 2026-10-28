#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <chrono>
#include <string>
#include <memory>
#include <syncstream>
#include <print>

using namespace std::chrono_literals;

// ============================================================
// COMMON THREAD ISSUES in C++23
// ============================================================
// Topics covered:
//   1.  Race condition
//   2.  Deadlock
//   3.  Livelock
//   4.  Starvation
//   5.  Dangling reference in lambda capture
//   6.  Forgetting join/detach (std::terminate)
//   7.  Garbled std::cout output
//   8.  Missing std::ref — pass by value vs reference
//   9.  Use-after-move on thread
//  10.  Accessing destroyed object from detached thread
//  11.  False sharing (cache line contention)
//  12.  Double-checked locking — use std::call_once
// ============================================================

std::mutex g_coutMutex;
void safePrint(const std::string& msg)
{
    std::lock_guard<std::mutex> lock(g_coutMutex);
    std::println("{}", msg);
}


// ============================================================
// Issue 1 — Race condition
// ============================================================
int              g_racyCounter{0};
int              g_safeCounter{0};
std::mutex       g_counterMutex;
std::atomic<int> g_atomicCounter{0};

void racyIncrement(int n)
{
    for (int i = 0; i < n; ++i)
        g_racyCounter++;   // data race!
}

void safeIncrement(int n)
{
    for (int i = 0; i < n; ++i) {
        std::lock_guard<std::mutex> lock(g_counterMutex);
        g_safeCounter++;
    }
}

void atomicIncrement(int n)
{
    for (int i = 0; i < n; ++i)
        g_atomicCounter++;
}


// ============================================================
// Issue 2 — Deadlock (fixed version shown)
// ============================================================
std::mutex g_dA, g_dB;

void deadlock_fixed()
{
    std::scoped_lock lock(g_dA, g_dB);   // FIX: atomic acquisition
    safePrint("[deadlock fixed] holds both locks safely");
}


// ============================================================
// Issue 3 — Livelock
// ============================================================
std::mutex g_resourceMutex;
bool       g_resourceAvailable = true;

void livelockThread(int id, int attempts)
{
    for (int i = 0; i < attempts; ++i) {
        if (g_resourceMutex.try_lock()) {
            if (g_resourceAvailable) {
                g_resourceAvailable = false;
                safePrint("[livelock " + std::to_string(id) + "] acquired resource");
                std::this_thread::sleep_for(5ms);
                g_resourceAvailable = true;
                g_resourceMutex.unlock();
                safePrint("[livelock " + std::to_string(id) + "] released resource");
                return;
            }
            g_resourceMutex.unlock();
        }
        std::this_thread::yield();
    }
    safePrint("[livelock " + std::to_string(id) + "] gave up after "
              + std::to_string(attempts) + " attempts");
}


// ============================================================
// Issue 4 — Starvation
// ============================================================
std::mutex       g_starveMutex;
std::atomic<int> g_starvationCount{0};

void greedyThread(int id)
{
    for (int i = 0; i < 3; ++i) {
        std::lock_guard<std::mutex> lock(g_starveMutex);
        safePrint("[greedy " + std::to_string(id) + "] holding lock iteration "
                  + std::to_string(i));
        std::this_thread::sleep_for(20ms);
    }
}

void starvingThread()
{
    for (int attempt = 0; attempt < 50; ++attempt) {
        if (g_starveMutex.try_lock()) {
            ++g_starvationCount;
            safePrint("[starving] finally got lock! (attempt "
                      + std::to_string(attempt) + ")");
            g_starveMutex.unlock();
            return;
        }
        std::this_thread::sleep_for(1ms);
    }
    safePrint("[starving] STARVED — could not acquire lock in 50 attempts");
}


// ============================================================
// Issue 7 — Garbled cout
// ============================================================
void garbledPrint(int id)
{
    std::print("[thread {}] ", id);
    std::this_thread::sleep_for(1ms);
    std::println("line {}", id);
}

void syncedPrint(int id)
{
    std::osyncstream(std::cout)
        << "[thread " << id << "] line " << id << '\n';
}


// ============================================================
// Issue 10 — Accessing destroyed object from detached thread
// ============================================================
class ResourceOwner {
public:
    int data = 42;

    void startUnsafeThread() {
        // DANGEROUS — captures 'this', but owner may be destroyed
        // before thread runs. Shown as comment only:
        safePrint("[unsafe] would capture 'this' — dangerous if owner destroyed");
    }

    void startSafeThread() {
        auto shared = std::make_shared<int>(data);   // own a copy
        std::thread([shared]() {
            std::this_thread::sleep_for(50ms);
            safePrint("[safe thread] data = " + std::to_string(*shared));
        }).detach();
    }
};


// ============================================================
// Issue 11 — False sharing (cache line contention)
// ============================================================
struct BadCounters {
    int a{0};
    int b{0};
};

struct alignas(64) AlignedCounter {
    int value{0};
};


// ============================================================
// Issue 12 — Double-checked locking — use call_once
// ============================================================
std::once_flag g_initFlag;
int*           g_singleton = nullptr;

void correctInit()
{
    std::call_once(g_initFlag, []() {
        g_singleton = new int(42);
        safePrint("[call_once] singleton initialized: "
                  + std::to_string(*g_singleton));
    });
}


// ============================================================
// MAIN
// ============================================================
int main()
{
    // -------------------------------------------------------
    std::println("=====================================================");
    std::println("Issue 1: Race Condition");
    std::println("=====================================================");

    g_racyCounter = 0; g_safeCounter = 0; g_atomicCounter = 0;

    std::thread r1(racyIncrement, 100000);
    std::thread r2(racyIncrement, 100000);
    r1.join(); r2.join();
    std::println("Racy:   expected=200000 actual={} (may differ)", g_racyCounter);

    std::thread s1(safeIncrement, 100000);
    std::thread s2(safeIncrement, 100000);
    s1.join(); s2.join();
    std::println("Mutex:  expected=200000 actual={} (always correct)", g_safeCounter);

    std::thread a1(atomicIncrement, 100000);
    std::thread a2(atomicIncrement, 100000);
    a1.join(); a2.join();
    std::println("Atomic: expected=200000 actual={} (always correct)", g_atomicCounter.load());


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Issue 2: Deadlock");
    std::println("=====================================================");
    std::thread d1(deadlock_fixed);
    std::thread d2(deadlock_fixed);
    d1.join(); d2.join();


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Issue 3: Livelock");
    std::println("=====================================================");

    std::println("Livelock: threads keep yielding — no progress");
    std::println("Fix: randomized backoff before retrying");
    g_resourceAvailable = true;
    std::thread ll1(livelockThread, 1, 10);
    std::thread ll2(livelockThread, 2, 10);
    ll1.join(); ll2.join();


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Issue 4: Starvation");
    std::println("=====================================================");

    std::thread greedy1(greedyThread, 1);
    std::thread greedy2(greedyThread, 2);
    std::thread starving(starvingThread);
    greedy1.join(); greedy2.join(); starving.join();


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Issue 5: Dangling reference in lambda capture");
    std::println("=====================================================");

    {
        int local = 42;
        std::thread t([local]() {   // capture by value — safe
            safePrint("[lambda safe] local = " + std::to_string(local));
        });
        t.join();
    }


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Issue 6: Forgetting join/detach");
    std::println("=====================================================");
    {
        std::jthread jt([]() {
            safePrint("[jthread] auto-joined on scope exit");
        });
    }


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Issue 7: Garbled cout output");
    std::println("=====================================================");

    std::println("--- Without sync (may be garbled) ---");
    {
        std::vector<std::thread> garbled;
        for (int i = 1; i <= 4; ++i)
            garbled.emplace_back(garbledPrint, i);
        for (auto& t : garbled) t.join();
    }

    std::println("--- With osyncstream (always clean) ---");
    {
        std::vector<std::thread> synced;
        for (int i = 1; i <= 4; ++i)
            synced.emplace_back(syncedPrint, i);
        for (auto& t : synced) t.join();
    }


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Issue 8: pass by value vs reference in thread");
    std::println("=====================================================");

    // std::thread copies all arguments by default.
    // To demonstrate "no ref" vs "with ref" we use lambdas:

    int val = 0;

    // Lambda captures by VALUE — original unchanged
    std::thread t1([val]() mutable {
        val = 999;
        safePrint("[by value] inside thread val = " + std::to_string(val));
    });
    t1.join();
    std::println("After by-value thread: val = {} (unchanged — 0)", val);

    // Lambda captures by REFERENCE — original changed
    std::thread t2([&val]() {
        val = 999;
        safePrint("[by ref] inside thread val = " + std::to_string(val));
    });
    t2.join();
    std::println("After by-ref thread:   val = {} (changed — 999)", val);

    // For free functions taking references, use std::ref:
    // void modify(int& n) { n = 42; }
    // std::thread t(modify, std::ref(val));  // std::ref required!
    std::println("For free functions: std::thread t(modify, std::ref(val))");


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Issue 9: Use-after-move on thread");
    std::println("=====================================================");

    std::thread moved([]() { safePrint("[moved thread] running"); });
    std::thread owner = std::move(moved);

    std::println("moved.joinable()  = {} (false — empty after move)", moved.joinable());
    std::println("owner.joinable()  = {} (true — owns the thread)", owner.joinable());
    // moved.join();   // CRASH — moved is empty
    owner.join();      // correct


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Issue 10: Accessing destroyed object from detached thread");
    std::println("=====================================================");

    {
        ResourceOwner ro;
        ro.startUnsafeThread();
        ro.startSafeThread();
    }   // ro destroyed — safe thread holds shared_ptr copy

    std::this_thread::sleep_for(150ms);
    std::println("[main] safe thread used shared_ptr — no crash");


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Issue 11: False sharing");
    std::println("=====================================================");

    // Bad: a and b may share a cache line
    BadCounters bad;
    {
        std::thread ba([&bad]() { for (int i=0;i<1000000;++i) bad.a++; });
        std::thread bb([&bad]() { for (int i=0;i<1000000;++i) bad.b++; });
        auto t1s = std::chrono::high_resolution_clock::now();
        ba.join(); bb.join();
        double ms = std::chrono::duration<double,std::milli>(
                        std::chrono::high_resolution_clock::now()-t1s).count();
        std::println("False sharing:  {} ms", ms);
    }

    // Good: each counter on its own cache line
    AlignedCounter good_a, good_b;
    {
        std::thread ga([&good_a]() { for (int i=0;i<1000000;++i) good_a.value++; });
        std::thread gb([&good_b]() { for (int i=0;i<1000000;++i) good_b.value++; });
        auto t2s = std::chrono::high_resolution_clock::now();
        ga.join(); gb.join();
        double ms = std::chrono::duration<double,std::milli>(
                        std::chrono::high_resolution_clock::now()-t2s).count();
        std::println("Cache aligned:  {} ms", ms);
    }
    std::println("(aligned is typically faster — no cache line invalidation)");


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Issue 12: Double-checked locking — use call_once");
    std::println("=====================================================");

    std::vector<std::thread> initThreads;
    for (int i = 0; i < 5; ++i)
        initThreads.emplace_back(correctInit);
    for (auto& t : initThreads) t.join();
    std::println("call_once guarantees exactly one initialization");
    delete g_singleton;


    std::println("");
    std::println("=== CommonThreadIssues complete ===");
    return 0;
}
