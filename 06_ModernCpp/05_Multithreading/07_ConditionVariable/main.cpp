#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <string>
#include <chrono>
#include <atomic>

using namespace std::chrono_literals;

std::mutex g_coutMutex;
void safePrint(const std::string& msg) {
    std::lock_guard<std::mutex> lock(g_coutMutex);
    std::cout << msg << '\n';
}

// ---- Example 1: Basic wait/notify ----
std::mutex g_basicMutex;
std::condition_variable g_basicCv;
bool g_dataReady = false;
int  g_data      = 0;

void basicProducer() {
    std::this_thread::sleep_for(50ms);
    { std::lock_guard<std::mutex> lock(g_basicMutex); g_data = 42; g_dataReady = true; }
    safePrint("[producer] data ready: 42");
    g_basicCv.notify_one();
}
void basicConsumer() {
    std::unique_lock<std::mutex> lock(g_basicMutex);
    safePrint("[consumer] waiting...");
    g_basicCv.wait(lock, []() { return g_dataReady; });
    safePrint("[consumer] received: " + std::to_string(g_data));
}

// ---- Example 2: notify_all broadcast ----
std::mutex g_broadcastMutex;
std::condition_variable g_broadcastCv;
bool g_startSignal = false;

void racingWorker(int id) {
    std::unique_lock<std::mutex> lock(g_broadcastMutex);
    safePrint("[racer " + std::to_string(id) + "] waiting...");
    g_broadcastCv.wait(lock, []() { return g_startSignal; });
    safePrint("[racer " + std::to_string(id) + "] GO!");
}
void startRace() {
    std::this_thread::sleep_for(50ms);
    { std::lock_guard<std::mutex> lock(g_broadcastMutex); g_startSignal = true; }
    safePrint("[starter] READY SET...");
    g_broadcastCv.notify_all();
}

// ---- Example 3: Bounded producer/consumer ----
std::mutex g_queueMutex;
std::condition_variable g_notEmpty, g_notFull;
std::queue<int> g_boundedQueue;
const int MAX_SIZE = 3;
bool g_producingDone = false;

void boundedProducer(int count) {
    for (int i=1;i<=count;++i) {
        std::unique_lock<std::mutex> lock(g_queueMutex);
        g_notFull.wait(lock, []() { return (int)g_boundedQueue.size() < MAX_SIZE; });
        g_boundedQueue.push(i);
        safePrint("[producer] pushed " + std::to_string(i)
                  + " (size:" + std::to_string(g_boundedQueue.size()) + ")");
        g_notEmpty.notify_one();
    }
    { std::lock_guard<std::mutex> lock(g_queueMutex); g_producingDone = true; }
    g_notEmpty.notify_all();
}
void boundedConsumer(int id) {
    while (true) {
        std::unique_lock<std::mutex> lock(g_queueMutex);
        g_notEmpty.wait(lock, []() { return !g_boundedQueue.empty() || g_producingDone; });
        if (g_boundedQueue.empty() && g_producingDone) break;
        int item = g_boundedQueue.front(); g_boundedQueue.pop();
        safePrint("[consumer " + std::to_string(id) + "] popped " + std::to_string(item));
        g_notFull.notify_one();
    }
}

// ---- Example 4: wait_for timeout ----
std::mutex g_timeoutMutex;
std::condition_variable g_timeoutCv;
bool g_timeoutReady = false;

void timeoutConsumer() {
    std::unique_lock<std::mutex> lock(g_timeoutMutex);
    bool signaled = g_timeoutCv.wait_for(lock, 80ms, []() { return g_timeoutReady; });
    safePrint(signaled ? "[timeout consumer] signal received!"
                       : "[timeout consumer] TIMED OUT");
}
void slowProducer() {
    std::this_thread::sleep_for(200ms);
    { std::lock_guard<std::mutex> lock(g_timeoutMutex); g_timeoutReady = true; }
    g_timeoutCv.notify_one();
}

// ---- Example 5: Barrier ----
std::mutex g_barrierMutex;
std::condition_variable g_barrierCv;
int  g_barrierCount  = 0;
bool g_barrierPassed = false;
const int BARRIER_N  = 4;

void barrierWorker(int id) {
    std::this_thread::sleep_for(std::chrono::milliseconds(id * 10));
    safePrint("[barrier " + std::to_string(id) + "] reached barrier");
    {
        std::unique_lock<std::mutex> lock(g_barrierMutex);
        if (++g_barrierCount == BARRIER_N) {
            g_barrierPassed = true;
            g_barrierCv.notify_all();
            safePrint("[barrier] ALL reached — releasing!");
        } else {
            g_barrierCv.wait(lock, []() { return g_barrierPassed; });
        }
    }
    safePrint("[barrier " + std::to_string(id) + "] past barrier");
}

int main()
{
    std::cout << "=====================================================\n";
    std::cout << "Example 1: Basic wait / notify_one\n";
    std::cout << "=====================================================\n";
    g_dataReady = false;
    std::thread p1(basicProducer), c1(basicConsumer);
    p1.join(); c1.join();

    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: notify_all — broadcast\n";
    std::cout << "=====================================================\n";
    g_startSignal = false;
    std::vector<std::thread> racers;
    for (int i=1;i<=4;++i) racers.emplace_back(racingWorker, i);
    std::thread starter(startRace);
    for (auto& t : racers) t.join();
    starter.join();

    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: Bounded producer/consumer\n";
    std::cout << "=====================================================\n";
    g_producingDone = false;
    while (!g_boundedQueue.empty()) g_boundedQueue.pop();
    std::thread prod(boundedProducer, 6);
    std::thread cons1(boundedConsumer, 1), cons2(boundedConsumer, 2);
    prod.join(); cons1.join(); cons2.join();

    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: wait_for — timeout\n";
    std::cout << "=====================================================\n";
    g_timeoutReady = false;
    std::thread tc(timeoutConsumer), ts(slowProducer);
    tc.join(); ts.join();

    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: Barrier — wait for N threads\n";
    std::cout << "=====================================================\n";
    g_barrierCount = 0; g_barrierPassed = false;
    std::vector<std::thread> bw;
    for (int i=1;i<=BARRIER_N;++i) bw.emplace_back(barrierWorker, i);
    for (auto& t : bw) t.join();

    std::cout << "\n=== ConditionVariable complete ===\n";
    return 0;
}
