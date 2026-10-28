#include <thread>
#include <atomic>
#include <vector>
#include <chrono>
#include <string>
#include <mutex>
#include <print>

using namespace std::chrono_literals;

// ============================================================
// ATOMICS in C++23
// ============================================================
// Topics covered:
//   1.  What is an atomic — why it's needed
//   2.  std::atomic<int>  — basic operations
//   3.  std::atomic<bool> — flag pattern
//   4.  std::atomic<T*>   — atomic pointer
//   5.  fetch_add, fetch_sub, fetch_and, fetch_or, fetch_xor
//   6.  compare_exchange_strong / weak (CAS)
//   7.  Memory ordering — relaxed, acquire, release, seq_cst
//   8.  std::atomic_flag  — lock-free boolean
//   9.  Spinlock using atomic_flag
//  10.  Atomic vs mutex performance comparison
//  11.  std::atomic<shared_ptr> (C++20)
// ============================================================

std::mutex g_coutMutex;
void safePrint(const std::string& msg)
{
    std::lock_guard<std::mutex> lock(g_coutMutex);
    std::println("{}", msg);
}


// ============================================================
// Example 1 — Why atomics? Race condition without them
// ============================================================
int              g_racyCounter{0};
std::atomic<int> g_atomicCounter{0};

void racyIncrement(int n)
{
    for (int i = 0; i < n; ++i)
        g_racyCounter++;   // data race — undefined behavior
}

void atomicIncrement(int n)
{
    for (int i = 0; i < n; ++i)
        g_atomicCounter++;   // atomic — always correct
}


// ============================================================
// Example 3 — std::atomic<bool> flag
// ============================================================
std::atomic<bool> g_ready{false};
std::atomic<bool> g_stopFlag{false};

void waitForReady()
{
    safePrint("[waiter] waiting for ready flag...");
    while (!g_ready.load())
        std::this_thread::yield();
    safePrint("[waiter] flag is set — proceeding");
}

void setReady()
{
    std::this_thread::sleep_for(50ms);
    g_ready.store(true);
    safePrint("[setter] flag set to true");
}

void backgroundWorker()
{
    int count = 0;
    while (!g_stopFlag.load(std::memory_order_relaxed)) {
        ++count;
        std::this_thread::sleep_for(10ms);
    }
    safePrint("[background] stopped after " + std::to_string(count) + " iterations");
}


// ============================================================
// Example 5 — fetch operations
// ============================================================
std::atomic<int>  g_fetchCounter{100};
std::atomic<int>  g_bitFlags{0b00001111};


// ============================================================
// Example 6 — Compare-and-swap (CAS)
// Foundation of lock-free algorithms
// ============================================================
std::atomic<int> g_casValue{0};

void casWorker(int id, int expected, int desired)
{
    int exp = expected;
    bool success = g_casValue.compare_exchange_strong(exp, desired);
    if (success)
        safePrint("[CAS worker " + std::to_string(id) + "] SUCCESS: "
                  + std::to_string(expected) + " -> " + std::to_string(desired));
    else
        safePrint("[CAS worker " + std::to_string(id) + "] FAILED: expected "
                  + std::to_string(expected) + " but found " + std::to_string(exp));
}

// Lock-free stack using CAS
struct Node {
    int   value;
    Node* next;
    explicit Node(int v) : value(v), next(nullptr) {}
};

class LockFreeStack {
private:
    std::atomic<Node*> m_top{nullptr};

public:
    void push(int value) {
        Node* newNode = new Node(value);
        newNode->next = m_top.load();
        // Retry until CAS succeeds
        while (!m_top.compare_exchange_weak(newNode->next, newNode))
            ;   // newNode->next is updated automatically on failure
    }

    bool pop(int& result) {
        Node* top = m_top.load();
        while (top != nullptr) {
            if (m_top.compare_exchange_weak(top, top->next)) {
                result = top->value;
                delete top;
                return true;
            }
        }
        return false;
    }

    ~LockFreeStack() {
        int val;
        while (pop(val)) {}
    }
};


// ============================================================
// Example 7 — Memory ordering
// ============================================================
std::atomic<int>  g_data{0};
std::atomic<bool> g_dataReady{false};

void producer_mo()
{
    g_data.store(42, std::memory_order_relaxed);        // store data
    g_dataReady.store(true, std::memory_order_release); // release — ensures data visible
    safePrint("[producer] data stored with release ordering");
}

void consumer_mo()
{
    while (!g_dataReady.load(std::memory_order_acquire)) // acquire — sees producer's stores
        std::this_thread::yield();
    int val = g_data.load(std::memory_order_relaxed);
    safePrint("[consumer] data read: " + std::to_string(val)
              + " (acquire ordering guarantees correct value)");
}


// ============================================================
// Example 8 — std::atomic_flag (guaranteed lock-free)
// ============================================================
std::atomic_flag g_atomicFlag = ATOMIC_FLAG_INIT;

void atomicFlagWorker(int id)
{
    // test_and_set returns old value — true if was already set
    bool wasSet = g_atomicFlag.test_and_set();
    if (!wasSet)
        safePrint("[flag worker " + std::to_string(id) + "] acquired flag");
    else
        safePrint("[flag worker " + std::to_string(id) + "] flag already set");
}


// ============================================================
// Example 9 — Spinlock using atomic_flag
// ============================================================
class Spinlock {
private:
    std::atomic_flag m_flag = ATOMIC_FLAG_INIT;

public:
    void lock() {
        while (m_flag.test_and_set(std::memory_order_acquire))
            std::this_thread::yield();   // spin — yield to avoid wasting CPU
    }

    void unlock() {
        m_flag.clear(std::memory_order_release);
    }
};

Spinlock     g_spinlock;
int          g_spinCounter{0};

void spinlockWorker(int n)
{
    for (int i = 0; i < n; ++i) {
        g_spinlock.lock();
        ++g_spinCounter;
        g_spinlock.unlock();
    }
}


// ============================================================
// Example 10 — Performance: atomic vs mutex
// ============================================================
std::atomic<long long> g_atomicPerf{0};
long long              g_mutexPerf{0};
std::mutex             g_perfMutex;

void atomicPerfWorker(int n)
{
    for (int i = 0; i < n; ++i)
        g_atomicPerf.fetch_add(1, std::memory_order_relaxed);
}

void mutexPerfWorker(int n)
{
    for (int i = 0; i < n; ++i) {
        std::lock_guard<std::mutex> lock(g_perfMutex);
        ++g_mutexPerf;
    }
}


// ============================================================
// MAIN
// ============================================================
int main()
{
    // -------------------------------------------------------
    std::println("=====================================================");
    std::println("Example 1: Race condition vs atomic");
    std::println("=====================================================");

    g_racyCounter = 0; g_atomicCounter = 0;

    std::thread r1(racyIncrement, 100000);
    std::thread r2(racyIncrement, 100000);
    r1.join(); r2.join();
    std::println("Racy:   expected=200000 actual={} (non-deterministic)", g_racyCounter);

    std::thread a1(atomicIncrement, 100000);
    std::thread a2(atomicIncrement, 100000);
    a1.join(); a2.join();
    std::println("Atomic: expected=200000 actual={} (always correct)", g_atomicCounter.load());


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 2: std::atomic<int> — all basic operations");
    std::println("=====================================================");

    std::atomic<int> val{10};

    std::println("Initial value:   {}", val.load());
    val.store(20);
    std::println("After store(20): {}", val.load());

    int old = val.exchange(99);
    std::println("exchange(99):    new={} old={}", val.load(), old);

    ++val;
    std::println("After ++val:     {}", val.load());
    --val;
    std::println("After --val:     {}", val.load());
    val += 5;
    std::println("After val += 5:  {}", val.load());
    val -= 3;
    std::println("After val -= 3:  {}", val.load());

    std::println("is_lock_free:    {}", val.is_lock_free());


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 3: std::atomic<bool> — flag patterns");
    std::println("=====================================================");

    // Ready flag
    g_ready = false;
    std::thread waiter(waitForReady);
    std::thread setter(setReady);
    waiter.join(); setter.join();

    // Stop flag
    g_stopFlag = false;
    std::thread worker(backgroundWorker);
    std::this_thread::sleep_for(50ms);
    g_stopFlag.store(true);
    safePrint("[main] stop flag set");
    worker.join();


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 4: std::atomic<T*> — atomic pointer");
    std::println("=====================================================");

    int arr[3] = {10, 20, 30};
    std::atomic<int*> ptr{arr};

    std::println("ptr points to: {}", *ptr.load());

    ptr.store(&arr[1]);
    std::println("After store(&arr[1]): {}", *ptr.load());

    // Atomic pointer arithmetic
    ptr.fetch_add(1);   // advances by sizeof(int)
    std::println("After fetch_add(1): {}", *ptr.load());


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 5: fetch_add, fetch_sub, fetch_and, fetch_or, fetch_xor");
    std::println("=====================================================");

    g_fetchCounter = 100;
    std::println("Initial:          {}", g_fetchCounter.load());

    int prev = g_fetchCounter.fetch_add(10);
    std::println("fetch_add(10):    new={} prev={}", g_fetchCounter.load(), prev);

    prev = g_fetchCounter.fetch_sub(5);
    std::println("fetch_sub(5):     new={} prev={}", g_fetchCounter.load(), prev);

    // Bitwise operations
    g_bitFlags = 0b00001111;
    std::println("");
    std::println("Bit flags initial:      {} (0b00001111)", g_bitFlags.load());

    g_bitFlags.fetch_and(0b00000101);
    std::println("fetch_and(0b00000101):  {} (keep bits 0,2)", g_bitFlags.load());

    g_bitFlags.fetch_or(0b00110000);
    std::println("fetch_or(0b00110000):   {} (set bits 4,5)", g_bitFlags.load());

    g_bitFlags.fetch_xor(0b00000101);
    std::println("fetch_xor(0b00000101):  {} (toggle bits 0,2)", g_bitFlags.load());


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 6: Compare-and-Swap (CAS)");
    std::println("=====================================================");

    // Basic CAS
    g_casValue = 0;
    std::println("Initial casValue: {}", g_casValue.load());

    // T1 succeeds: 0 -> 42
    // T2 fails: expected 0 but finds 42
    std::thread ct1(casWorker, 1, 0, 42);
    std::thread ct2(casWorker, 2, 0, 99);
    ct1.join(); ct2.join();
    std::println("Final casValue: {}", g_casValue.load());

    // Lock-free stack
    std::println("");
    std::println("Lock-free stack:");
    LockFreeStack stack;
    std::vector<std::thread> pushers;
    for (int i = 1; i <= 5; ++i)
        pushers.emplace_back([&stack, i]() {
            stack.push(i * 10);
            safePrint("[push] pushed " + std::to_string(i * 10));
        });
    for (auto& t : pushers) t.join();

    int popped;
    while (stack.pop(popped))
        safePrint("[pop] popped " + std::to_string(popped));


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 7: Memory Ordering");
    std::println("=====================================================");

    g_data = 0; g_dataReady = false;
    std::thread prod(producer_mo);
    std::thread cons(consumer_mo);
    prod.join(); cons.join();


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 8: std::atomic_flag");
    std::println("=====================================================");

    g_atomicFlag.clear();
    std::thread af1(atomicFlagWorker, 1);
    std::thread af2(atomicFlagWorker, 2);
    std::thread af3(atomicFlagWorker, 3);
    af1.join(); af2.join(); af3.join();

    // C++20: test() without setting
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
    std::println("flag.test() before set: {}", flag.test());
    flag.test_and_set();
    std::println("flag.test() after set:  {}", flag.test());
    flag.clear();
    std::println("flag.test() after clear:{}", flag.test());


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 9: Spinlock using atomic_flag");
    std::println("=====================================================");

    g_spinCounter = 0;
    std::vector<std::thread> spinWorkers;
    for (int i = 0; i < 4; ++i)
        spinWorkers.emplace_back(spinlockWorker, 25000);
    for (auto& t : spinWorkers) t.join();

    std::println("Spinlock counter expected: 100000");
    std::println("Spinlock counter actual:   {}", g_spinCounter);


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 10: Atomic vs Mutex — performance");
    std::println("=====================================================");

    const int ITER = 1000000;

    g_atomicPerf = 0;
    auto t1 = std::chrono::high_resolution_clock::now();
    std::thread ap1(atomicPerfWorker, ITER);
    std::thread ap2(atomicPerfWorker, ITER);
    ap1.join(); ap2.join();
    double atomicMs = std::chrono::duration<double, std::milli>(
                          std::chrono::high_resolution_clock::now() - t1).count();

    g_mutexPerf = 0;
    auto t2 = std::chrono::high_resolution_clock::now();
    std::thread mp1(mutexPerfWorker, ITER);
    std::thread mp2(mutexPerfWorker, ITER);
    mp1.join(); mp2.join();
    double mutexMs = std::chrono::duration<double, std::milli>(
                         std::chrono::high_resolution_clock::now() - t2).count();

    std::println("2x{} increments:", ITER);
    std::println("  Atomic (relaxed): {} ms", atomicMs);
    std::println("  Mutex:            {} ms", mutexMs);
    std::println("  Speedup:          {}x", mutexMs / atomicMs);


    std::println("");
    std::println("=== Atomics complete ===");
    return 0;
}
