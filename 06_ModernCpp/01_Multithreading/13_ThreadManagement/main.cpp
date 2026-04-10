#include <iostream>
#include <thread>
#include <stop_token>     // C++20 std::jthread, std::stop_token
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>
#include <stdexcept>
#include <exception>
#include <chrono>
#include <string>
#include <future>


using namespace std::chrono_literals;

// ============================================================
// THREAD MANAGEMENT in C++23
// ============================================================
// Topics covered:
//   1.  std::jthread — auto-joining thread (C++20)
//   2.  Stopping threads gracefully with std::stop_token
//   3.  std::stop_source and std::stop_callback
//   4.  Stopping threads with std::atomic<bool> flag (C++11 way)
//   5.  Exception handling across thread boundaries
//   6.  Joining multiple threads (vector)
//   7.  Thread groups — wait for ANY thread to finish
//   8.  Simple thread pool
//   9.  RAII thread guard — safe cleanup
//  10.  Thread lifetime and object ownership
// ============================================================


// ============================================================
// Example 4 helper — atomic stop flag (pre-C++20 approach)
// ============================================================
std::atomic<bool> g_stopFlag{false};

void workerWithFlag(int id)
{
    std::cout << "[flag worker " << id << "] started\n";
    while (!g_stopFlag.load()) {
        std::this_thread::sleep_for(30ms);
        std::cout << "[flag worker " << id << "] working...\n";
    }
    std::cout << "[flag worker " << id << "] stopped gracefully\n";
}


// ============================================================
// Example 5 helper — exception propagation across threads
// ============================================================
std::exception_ptr g_exceptionPtr = nullptr;

void workerThatThrows()
{
    try {
        std::this_thread::sleep_for(20ms);
        throw std::runtime_error("Error inside worker thread!");
    } catch (...) {
        g_exceptionPtr = std::current_exception();   // capture exception
    }
}


// ============================================================
// Example 8 — Simple thread pool
// ============================================================
class ThreadPool {
private:
    std::vector<std::thread>          m_workers;
    std::queue<std::function<void()>> m_tasks;
    std::mutex                        m_mutex;
    std::condition_variable           m_cv;
    std::atomic<bool>                 m_stop{false};
    std::atomic<int>                  m_activeTasks{0};

public:
    explicit ThreadPool(int numThreads)
    {
        for (int i = 0; i < numThreads; ++i) {
            m_workers.emplace_back([this, i]() {
                std::cout << "[pool worker " << i << "] ready\n";
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(m_mutex);
                        m_cv.wait(lock, [this]() {
                            return !m_tasks.empty() || m_stop.load();
                        });

                        if (m_stop && m_tasks.empty()) {
                            std::cout << "[pool worker " << i << "] exiting\n";
                            return;
                        }

                        task = std::move(m_tasks.front());
                        m_tasks.pop();
                        ++m_activeTasks;
                    }
                    task();
                    --m_activeTasks;
                }
            });
        }
    }

    void submit(std::function<void()> task)
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_tasks.push(std::move(task));
        }
        m_cv.notify_one();
    }

    void waitAll()
    {
        // Wait until all tasks are done
        while (true) {
            std::unique_lock<std::mutex> lock(m_mutex);
            if (m_tasks.empty() && m_activeTasks == 0) break;
            lock.unlock();
            std::this_thread::sleep_for(5ms);
        }
    }

    ~ThreadPool()
    {
        m_stop = true;
        m_cv.notify_all();
        for (auto& t : m_workers)
            if (t.joinable()) t.join();
    }

    int workerCount() const { return (int)m_workers.size(); }
};


// ============================================================
// Example 9 — RAII thread guard
// Ensures threads are always joined when scope exits
// ============================================================
class ThreadGuard {
private:
    std::vector<std::thread>& m_threads;

public:
    explicit ThreadGuard(std::vector<std::thread>& threads)
        : m_threads(threads) {}

    ~ThreadGuard() {
        std::cout << "[ThreadGuard] joining " << m_threads.size() << " threads\n";
        for (auto& t : m_threads)
            if (t.joinable()) t.join();
    }

    // Non-copyable
    ThreadGuard(const ThreadGuard&)            = delete;
    ThreadGuard& operator=(const ThreadGuard&) = delete;
};


// ============================================================
// MAIN
// ============================================================
int main()
{
    // -------------------------------------------------------
    std::cout << "=====================================================\n";
    std::cout << "Example 1: std::jthread — auto-joining thread (C++20)\n";
    std::cout << "=====================================================\n";

    // std::jthread automatically joins in its destructor
    // No need to call join() or detach() — no std::terminate() risk
    {
        std::jthread jt([]() {
            std::this_thread::sleep_for(30ms);
            std::cout << "[jthread] work done\n";
        });
        std::cout << "[main] jthread created — leaving scope\n";
    }   // jt destructor calls join() automatically here
    std::cout << "[main] scope exited — jthread was joined automatically\n";

    // Compare with std::thread risk:
    // {
    //     std::thread t(func);
    // }  // CRASH if not joined/detached — std::terminate()

    // jthread in a vector — all auto-joined on destruction
    {
        std::vector<std::jthread> workers;
        for (int i = 0; i < 3; ++i) {
            workers.emplace_back([i]() {
                std::this_thread::sleep_for(20ms);
                std::cout << "[jthread " << i << "] done\n";
            });
        }
        std::cout << "[main] all jthreads will auto-join when vector is destroyed\n";
    }
    std::cout << "[main] vector destroyed — all jthreads joined\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: Stopping threads with std::stop_token\n";
    std::cout << "=====================================================\n";

    // std::jthread passes a stop_token automatically as first parameter
    {
        std::jthread worker([](std::stop_token stopToken) {
            int iteration = 0;
            while (!stopToken.stop_requested()) {
                std::cout << "[stop_token worker] iteration " << iteration++ << '\n';
                std::this_thread::sleep_for(25ms);
            }
            std::cout << "[stop_token worker] stop requested — exiting cleanly\n";
        });

        std::this_thread::sleep_for(100ms);
        std::cout << "[main] requesting stop...\n";
        worker.request_stop();   // sets the stop token
        // jthread destructor will join automatically
    }
    std::cout << "[main] worker stopped and joined\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: std::stop_source and std::stop_callback\n";
    std::cout << "=====================================================\n";

    std::stop_source stopSource;
    std::stop_token  stopToken = stopSource.get_token();

    // Register a callback that fires when stop is requested
    std::stop_callback callback(stopToken, []() {
        std::cout << "[stop_callback] stop was requested — running cleanup!\n";
    });

    std::jthread worker2([stopToken]() {
        while (!stopToken.stop_requested()) {
            std::this_thread::sleep_for(20ms);
            std::cout << "[worker2] running\n";
        }
        std::cout << "[worker2] exiting\n";
    });

    std::this_thread::sleep_for(80ms);
    std::cout << "[main] calling stopSource.request_stop()\n";
    stopSource.request_stop();   // triggers callback + notifies stop_token
    // worker2 auto-joins via jthread destructor


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: Stopping with atomic flag (pre-C++20 way)\n";
    std::cout << "=====================================================\n";

    g_stopFlag = false;
    std::thread fw1(workerWithFlag, 1);
    std::thread fw2(workerWithFlag, 2);

    std::this_thread::sleep_for(100ms);
    std::cout << "[main] setting stop flag\n";
    g_stopFlag = true;

    fw1.join();
    fw2.join();
    std::cout << "[main] both flag workers stopped\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: Exception handling across threads\n";
    std::cout << "=====================================================\n";

    // Exceptions do NOT propagate automatically across thread boundaries
    // Use std::exception_ptr to transfer them

    g_exceptionPtr = nullptr;
    std::thread thrower(workerThatThrows);
    thrower.join();

    // Re-throw in main thread
    if (g_exceptionPtr) {
        try {
            std::rethrow_exception(g_exceptionPtr);
        } catch (const std::exception& e) {
            std::cout << "[main] caught exception from thread: " << e.what() << '\n';
        }
    }

    // With std::async — exceptions are captured automatically in the future
    std::cout << "\nWith std::async — exception captured in future:\n";
    auto fut = std::async(std::launch::async, []() -> int {
        throw std::runtime_error("async worker error!");
        return 42;
    });

    try {
        int result = fut.get();   // rethrows the exception here
        std::cout << "Result: " << result << '\n';
    } catch (const std::exception& e) {
        std::cout << "[main] caught from future: " << e.what() << '\n';
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: Joining multiple threads (vector)\n";
    std::cout << "=====================================================\n";

    std::vector<std::thread> threads;

    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([i]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(10 * (i + 1)));
            std::cout << "[thread " << i << "] done\n";
        });
    }

    std::cout << "[main] waiting for " << threads.size() << " threads...\n";

    // Join all
    for (auto& t : threads)
        if (t.joinable()) t.join();

    std::cout << "[main] all threads joined\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: Detach and check completion with atomic\n";
    std::cout << "=====================================================\n";

    std::atomic<int> completedCount{0};
    int totalTasks = 4;

    for (int i = 0; i < totalTasks; ++i) {
        std::thread([i, &completedCount]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(20 * (i + 1)));
            std::cout << "[detached " << i << "] finished\n";
            ++completedCount;
        }).detach();
    }

    // Poll until all detached threads finish
    while (completedCount < totalTasks) {
        std::this_thread::sleep_for(10ms);
    }
    std::cout << "[main] all " << completedCount << " detached tasks completed\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: Simple Thread Pool\n";
    std::cout << "=====================================================\n";

    unsigned int cores = std::thread::hardware_concurrency();
    std::cout << "Creating thread pool with " << cores << " workers\n";

    {
        ThreadPool pool(cores);

        // Submit 8 tasks to the pool
        std::mutex printMutex;
        for (int i = 0; i < 8; ++i) {
            pool.submit([i, &printMutex]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(20 + i * 5));
                std::lock_guard<std::mutex> lock(printMutex);
                std::cout << "[task " << i << "] completed on thread "
                          << std::this_thread::get_id() << '\n';
            });
        }

        pool.waitAll();
        std::cout << "[main] all pool tasks done\n";
    }   // pool destructor joins all workers


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 9: RAII ThreadGuard — safe cleanup\n";
    std::cout << "=====================================================\n";

    {
        std::vector<std::thread> guardedThreads;
        ThreadGuard guard(guardedThreads);   // will join all on scope exit

        for (int i = 0; i < 3; ++i) {
            guardedThreads.emplace_back([i]() {
                std::this_thread::sleep_for(20ms);
                std::cout << "[guarded thread " << i << "] done\n";
            });
        }

        std::cout << "[main] leaving scope — ThreadGuard will join all\n";
    }   // ThreadGuard destructor joins everything — even if exception thrown
    std::cout << "[main] scope exited safely\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 10: Thread lifetime and object ownership\n";
    std::cout << "=====================================================\n";

    // RULE: object must outlive any thread that references it
    std::string data = "shared data";

    {
        // SAFE — thread joined before data goes out of scope
        std::jthread safe([&data]() {
            std::this_thread::sleep_for(20ms);
            std::cout << "[safe thread] data = " << data << '\n';
        });
    }   // jthread joined here — data still alive

    // DANGEROUS — would be UB if data went out of scope before thread finished
    // std::thread dangerous([&data]() { use(data); });
    // dangerous.detach();  // data may be destroyed while thread runs!
    // } // data destroyed — thread reads garbage

    std::cout << "[main] object lifetime rule: object must outlive the thread\n";
    std::cout << "[main] use jthread + capture by value for safety with detached threads\n";


    std::cout << "\n=== ThreadManagement complete ===\n";
    return 0;
}
