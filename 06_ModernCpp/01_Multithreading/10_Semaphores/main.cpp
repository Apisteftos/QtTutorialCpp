#include <iostream>
#include <thread>
#include <semaphore>
#include <mutex>
#include <vector>
#include <chrono>
#include <string>
#include <atomic>

using namespace std::chrono_literals;

std::mutex g_coutMutex;
void safePrint(const std::string& msg) {
    std::lock_guard<std::mutex> lock(g_coutMutex);
    std::cout << msg << '\n';
}

// ---- Example 1: counting_semaphore ----
std::counting_semaphore<3> g_semaphore(3);
std::atomic<int> g_activeWorkers{0};

void limitedWorker(int id) {
    safePrint("[worker " + std::to_string(id) + "] waiting...");
    g_semaphore.acquire();
    ++g_activeWorkers;
    safePrint("[worker " + std::to_string(id) + "] ENTERED (active:"
              + std::to_string(g_activeWorkers.load()) + ")");
    std::this_thread::sleep_for(50ms);
    --g_activeWorkers;
    safePrint("[worker " + std::to_string(id) + "] leaving");
    g_semaphore.release();
}

// ---- Example 2: binary_semaphore signal ----
std::binary_semaphore g_startSignal(0), g_doneSignal(0);

void signalWorker() {
    safePrint("[signal worker] waiting for start...");
    g_startSignal.acquire();
    safePrint("[signal worker] received start — working");
    std::this_thread::sleep_for(40ms);
    g_doneSignal.release();
}

// ---- Example 3: Connection pool ----
const int MAX_CONN = 3;
std::counting_semaphore<MAX_CONN> g_pool(MAX_CONN);
std::atomic<int> g_connId{0};

void databaseQuery(int clientId) {
    safePrint("[client " + std::to_string(clientId) + "] requesting connection...");
    g_pool.acquire();
    int id = ++g_connId;
    safePrint("[client " + std::to_string(clientId) + "] got conn #" + std::to_string(id));
    std::this_thread::sleep_for(std::chrono::milliseconds(20 + clientId * 5));
    safePrint("[client " + std::to_string(clientId) + "] done — returning conn");
    g_pool.release();
}

// ---- Example 4: try_acquire rate limit ----
std::counting_semaphore<5> g_rateLimit(5);

void apiRequest(int id) {
    if (g_rateLimit.try_acquire()) {
        safePrint("[request " + std::to_string(id) + "] processing");
        std::this_thread::sleep_for(20ms);
        safePrint("[request " + std::to_string(id) + "] done");
        g_rateLimit.release();
    } else {
        safePrint("[request " + std::to_string(id) + "] RATE LIMITED");
    }
}

// ---- Example 5: try_acquire_for timeout ----
std::counting_semaphore<2> g_timedSem(2);

void timedWorker(int id) {
    if (g_timedSem.try_acquire_for(60ms)) {
        safePrint("[timed " + std::to_string(id) + "] acquired");
        std::this_thread::sleep_for(40ms);
        g_timedSem.release();
    } else {
        safePrint("[timed " + std::to_string(id) + "] TIMEOUT");
    }
}

// ---- Example 6: Producer/Consumer with semaphores ----
std::counting_semaphore<10> g_itemsAvail(0), g_spaceAvail(10);
std::mutex g_bufMutex;
std::vector<int> g_buffer;
std::atomic<bool> g_semDone{false};

void semProducer(int count) {
    for (int i=1;i<=count;++i) {
        g_spaceAvail.acquire();
        { std::lock_guard<std::mutex> lock(g_bufMutex); g_buffer.push_back(i); }
        safePrint("[sem producer] produced: " + std::to_string(i));
        g_itemsAvail.release();
    }
    g_semDone = true;
    for (int i=0;i<3;++i) g_itemsAvail.release();
}
void semConsumer(int id) {
    while (true) {
        g_itemsAvail.acquire();
        int item = -1;
        {
            std::lock_guard<std::mutex> lock(g_bufMutex);
            if (g_buffer.empty()) { if (g_semDone) break; g_itemsAvail.release(); continue; }
            item = g_buffer.front(); g_buffer.erase(g_buffer.begin());
        }
        if (item >= 0) {
            safePrint("[sem consumer " + std::to_string(id) + "] consumed: " + std::to_string(item));
            g_spaceAvail.release();
        }
        if (g_semDone && g_buffer.empty()) break;
    }
}

int main()
{
    std::cout << "=====================================================\n";
    std::cout << "Example 1: counting_semaphore — max 3 concurrent\n";
    std::cout << "=====================================================\n";
    std::vector<std::thread> workers;
    for (int i=1;i<=7;++i) workers.emplace_back(limitedWorker, i);
    for (auto& t : workers) t.join();

    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: binary_semaphore — one-shot signal\n";
    std::cout << "=====================================================\n";
    std::thread sw(signalWorker);
    std::this_thread::sleep_for(30ms);
    safePrint("[main] sending start signal");
    g_startSignal.release();
    g_doneSignal.acquire();
    safePrint("[main] worker done");
    sw.join();

    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: Connection pool (pool size: 3, clients: 7)\n";
    std::cout << "=====================================================\n";
    std::vector<std::thread> clients;
    for (int i=1;i<=7;++i) clients.emplace_back(databaseQuery, i);
    for (auto& t : clients) t.join();

    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: Rate limiting with try_acquire\n";
    std::cout << "=====================================================\n";
    std::vector<std::thread> requests;
    for (int i=1;i<=10;++i) requests.emplace_back(apiRequest, i);
    for (auto& t : requests) t.join();

    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: try_acquire_for timeout\n";
    std::cout << "=====================================================\n";
    std::vector<std::thread> tw;
    for (int i=1;i<=5;++i) tw.emplace_back(timedWorker, i);
    for (auto& t : tw) t.join();

    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: Producer/Consumer with semaphores\n";
    std::cout << "=====================================================\n";
    g_semDone = false; g_buffer.clear();
    std::thread sp(semProducer, 6);
    std::thread sc1(semConsumer, 1), sc2(semConsumer, 2);
    sp.join(); sc1.join(); sc2.join();

    std::cout << "\n=== Semaphores complete ===\n";
    return 0;
}
