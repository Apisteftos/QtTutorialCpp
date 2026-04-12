#include <iostream>
#include <future>
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <numeric>
#include <stdexcept>
#include <print>

using namespace std::chrono_literals;

// ============================================================
// ASYNC in C++23
// ============================================================
// Topics covered:
//   1.  std::async — simplest async launch
//   2.  std::future — retrieving async results
//   3.  launch::async vs launch::deferred
//   4.  std::promise + std::future — manual signaling
//   5.  std::packaged_task — wrap callable for async use
//   6.  Exception propagation through futures
//   7.  wait, wait_for, wait_until — polling and timeouts
//   8.  std::shared_future — multiple consumers
//   9.  Parallel computation with multiple futures
//  10.  Practical: async MCX operations pipeline
// ============================================================


// ============================================================
// Helper functions
// ============================================================
int  computeSum(int from, int to)
{
    std::this_thread::sleep_for(80ms);   // simulate work
    int sum = 0;
    for (int i = from; i <= to; ++i) sum += i;
    return sum;
}

std::string fetchConfig(const std::string& key)
{
    std::this_thread::sleep_for(60ms);
    if (key == "server")   return "192.168.1.100";
    if (key == "port")     return "5060";
    if (key == "codec")    return "AMR-NB";
    throw std::runtime_error("unknown config key: " + key);
}

long long heavyComputation(int n)
{
    std::this_thread::sleep_for(50ms);
    long long result = 0;
    for (int i = 1; i <= n; ++i) result += (long long)i * i;
    return result;
}


int main()
{
    // -------------------------------------------------------
    std::cout << "=====================================================\n";
    std::cout << "Example 1: std::async — simplest async launch\n";
    std::cout << "=====================================================\n";

    {
        // std::async launches a task on a new thread
        // returns std::future<T> to retrieve the result later
        auto fut = std::async(std::launch::async, computeSum, 1, 100);

        std::cout << "[main] doing other work while async runs...\n";
        std::this_thread::sleep_for(20ms);
        std::cout << "[main] still working...\n";

        // .get() blocks until result is ready
        int result = fut.get();
        std::println("[main] sum(1..100) = {}", result);
    }

    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: std::future — states and operations\n";
    std::cout << "=====================================================\n";

    {
        auto fut = std::async(std::launch::async, []() -> int {
            std::this_thread::sleep_for(100ms);
            return 42;
        });

        // valid() — true if future holds a shared state
        std::println("valid() before get: {}", fut.valid());

        int val = fut.get();
        std::println("got value: {}", val);

        // After get(), future is no longer valid
        std::println("valid() after get:  {}", fut.valid());

        // Cannot call get() twice — throws std::future_error
        try {
            fut.get();
        } catch (const std::future_error& e) {
            std::println("second get() threw: {}", e.what());
        }
    }

    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: launch::async vs launch::deferred\n";
    std::cout << "=====================================================\n";

    {
        // launch::async — runs immediately in a new thread
        auto asyncFut = std::async(std::launch::async, []() {
            std::println("[async]    running on thread: {}",
                         std::hash<std::thread::id>{}(std::this_thread::get_id()));
            return 1;
        });

        // launch::deferred — runs lazily in the calling thread at .get()
        auto deferredFut = std::async(std::launch::deferred, []() {
            std::println("[deferred] running on thread: {}",
                         std::hash<std::thread::id>{}(std::this_thread::get_id()));
            return 2;
        });

        std::println("[main] thread: {}",
                     std::hash<std::thread::id>{}(std::this_thread::get_id()));
        std::cout << "[main] before deferred.get()\n";

        int r1 = asyncFut.get();
        int r2 = deferredFut.get();   // deferred runs HERE, in main thread

        std::println("async={} deferred={}", r1, r2);
        std::cout << "Note: deferred runs in the CALLING thread at .get()\n";
        std::cout << "Note: default policy may choose either — always specify!\n";
    }

    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: std::promise + std::future — manual signaling\n";
    std::cout << "=====================================================\n";

    {
        // promise: the producer side — sets the value
        // future:  the consumer side — gets the value
        std::promise<int>  promise;
        std::future<int>   future = promise.get_future();

        // Producer thread — sets value via promise
        std::thread producer([&promise]() {
            std::this_thread::sleep_for(50ms);
            std::println("[producer] computing and setting value...");
            promise.set_value(99);
        });

        // Consumer — waits via future
        std::println("[consumer] waiting for value...");
        int val = future.get();
        std::println("[consumer] received: {}", val);
        producer.join();
    }

    {
        // Promise used as one-shot signal (no value needed)
        std::promise<void> ready;
        std::future<void>  signal = ready.get_future();

        std::thread worker([&ready]() {
            std::cout << "[worker] initializing...\n";
            std::this_thread::sleep_for(40ms);
            std::cout << "[worker] ready — signaling\n";
            ready.set_value();   // void signal
        });

        signal.get();   // blocks until worker signals
        std::cout << "[main] worker is ready — proceeding\n";
        worker.join();
    }

    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: std::packaged_task — wrap callable\n";
    std::cout << "=====================================================\n";

    {
        // packaged_task wraps a callable and gives it a future
        std::packaged_task<int(int, int)> task(
            [](int a, int b) {
                std::this_thread::sleep_for(30ms);
                return a * b;
            }
            );

        std::future<int> fut = task.get_future();

        // Run task on a separate thread
        std::thread t(std::move(task), 6, 7);
        std::println("packaged_task result: {}", fut.get());
        t.join();
    }

    {
        // packaged_task in a queue — deferred execution
        std::vector<std::packaged_task<int()>> taskQueue;
        std::vector<std::future<int>>          futures;

        for (int i = 1; i <= 4; ++i) {
            std::packaged_task<int()> pt([i]() {
                std::this_thread::sleep_for(20ms);
                return i * i;
            });
            futures.push_back(pt.get_future());
            taskQueue.push_back(std::move(pt));
        }

        // Execute all tasks on threads
        std::vector<std::thread> workers;
        for (auto& t : taskQueue)
            workers.emplace_back(std::move(t));
        for (auto& w : workers) w.join();

        std::cout << "queued task results: ";
        for (auto& f : futures) std::cout << f.get() << ' ';
        std::cout << '\n';
    }

    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: Exception propagation through futures\n";
    std::cout << "=====================================================\n";

    {
        // Exception in async task is stored in the future
        // and rethrown when .get() is called
        auto badTask = std::async(std::launch::async, []() -> int {
            std::this_thread::sleep_for(20ms);
            throw std::runtime_error("async task failed!");
            return 0;
        });

        try {
            int val = badTask.get();   // rethrows stored exception
            (void)val;
        } catch (const std::runtime_error& e) {
            std::println("caught from async: {}", e.what());
        }
    }

    {
        // Exception from fetchConfig — thrown for bad key
        auto cfg1 = std::async(std::launch::async, fetchConfig, "server");
        auto cfg2 = std::async(std::launch::async, fetchConfig, "badkey");

        std::println("server: {}", cfg1.get());

        try {
            std::println("badkey: {}", cfg2.get());
        } catch (const std::runtime_error& e) {
            std::println("caught bad key: {}", e.what());
        }
    }

    {
        // Promise propagating exception
        std::promise<int> prom;
        std::future<int>  fut = prom.get_future();

        std::thread t([&prom]() {
            try {
                throw std::overflow_error("value too large");
            } catch (...) {
                prom.set_exception(std::current_exception());
            }
        });

        try {
            fut.get();
        } catch (const std::overflow_error& e) {
            std::println("caught from promise: {}", e.what());
        }
        t.join();
    }

    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: wait, wait_for, wait_until\n";
    std::cout << "=====================================================\n";

    {
        auto slowTask = std::async(std::launch::async, []() {
            std::this_thread::sleep_for(150ms);
            return 99;
        });

        // wait_for — non-blocking poll with timeout
        auto status = slowTask.wait_for(50ms);
        if (status == std::future_status::timeout)
            std::cout << "[main] not ready yet — doing other work\n";
        else if (status == std::future_status::ready)
            std::cout << "[main] ready already!\n";

        // wait() — block until done (no result)
        slowTask.wait();
        std::println("[main] now ready: {}", slowTask.get());
    }

    {
        // wait_until — absolute deadline
        auto task = std::async(std::launch::async, []() {
            std::this_thread::sleep_for(80ms);
            return 42;
        });

        auto deadline = std::chrono::steady_clock::now() + 200ms;
        auto status   = task.wait_until(deadline);

        if (status == std::future_status::ready)
            std::println("wait_until: ready before deadline: {}", task.get());
        else
            std::cout << "wait_until: timed out\n";
    }

    {
        // Polling loop pattern
        auto bgTask = std::async(std::launch::async, []() {
            std::this_thread::sleep_for(120ms);
            return std::string("background result");
        });

        std::cout << "polling: ";
        int polls = 0;
        while (bgTask.wait_for(20ms) != std::future_status::ready) {
            std::cout << '.' ;
            ++polls;
        }
        std::println(" done after {} polls, result: {}", polls, bgTask.get());
    }

    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: std::shared_future — multiple consumers\n";
    std::cout << "=====================================================\n";

    {
        // shared_future can be copied and .get() called multiple times
        std::promise<std::string>    prom;
        std::shared_future<std::string> sharedFut = prom.get_future().share();

        std::vector<std::thread> consumers;
        for (int i = 0; i < 4; ++i) {
            consumers.emplace_back([sharedFut, i]() {
                std::string val = sharedFut.get();   // all consumers get same value
                std::println("[consumer {}] received: {}", i, val);
            });
        }

        std::this_thread::sleep_for(30ms);
        std::println("[producer] broadcasting to {} consumers", consumers.size());
        prom.set_value("broadcast message from producer");

        for (auto& c : consumers) c.join();

        // Can call .get() multiple times on shared_future — unlike regular future
        std::println("shared_future.get() again: {}", sharedFut.get());
    }

    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 9: Parallel computation — multiple futures\n";
    std::cout << "=====================================================\n";

    {
        // Parallel sum of large range using multiple async tasks
        const int TOTAL = 1000000;
        unsigned int cores = std::thread::hardware_concurrency();
        int chunkSize = TOTAL / cores;

        std::cout << "parallel sum 1.." << TOTAL
                  << " using " << cores << " cores\n";

        std::vector<std::future<long long>> futures;
        for (unsigned int i = 0; i < cores; ++i) {
            int from = i * chunkSize + 1;
            int to   = (i == cores-1) ? TOTAL : (i+1) * chunkSize;
            futures.push_back(
                std::async(std::launch::async,
                           [from, to]() -> long long {
                               long long s = 0;
                               for (int j = from; j <= to; ++j) s += j;
                               return s;
                           })
                );
        }

        long long total = 0;
        for (auto& f : futures) total += f.get();

        long long expected = (long long)TOTAL * (TOTAL + 1) / 2;
        std::println("parallel result: {}", total);
        std::println("expected:        {}", expected);
        std::println("correct:         {}", total == expected);
    }

    {
        // Fire multiple independent async operations simultaneously
        auto t0 = std::chrono::high_resolution_clock::now();

        auto f1 = std::async(std::launch::async, fetchConfig, "server");
        auto f2 = std::async(std::launch::async, fetchConfig, "port");
        auto f3 = std::async(std::launch::async, fetchConfig, "codec");
        auto f4 = std::async(std::launch::async, heavyComputation, 10000);

        // All 4 run concurrently — total time ≈ max(each), not sum
        std::string server = f1.get();
        std::string port   = f2.get();
        std::string codec  = f3.get();
        long long   comp   = f4.get();

        double ms = std::chrono::duration<double,std::milli>(
                        std::chrono::high_resolution_clock::now() - t0).count();

        std::println("server={} port={} codec={}", server, port, codec);
        std::println("computation={}", comp);
        std::println("total time: {:.1f}ms (concurrent, not sequential)", ms);
    }

    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 10: Practical — async MCX operations pipeline\n";
    std::cout << "=====================================================\n";

    {
        // Simulate an MCX call setup pipeline where stages run asynchronously

        // Stage 1: Authenticate user (async)
        auto authFut = std::async(std::launch::async, [](int userId) -> bool {
            std::this_thread::sleep_for(40ms);
            std::println("[auth] user {} authenticated", userId);
            return userId > 0;
        }, 42);

        // Stage 2: Resolve group (async — runs concurrently with auth)
        auto groupFut = std::async(std::launch::async,
                                   [](const std::string& groupId) -> std::string {
                                       std::this_thread::sleep_for(30ms);
                                       std::println("[group] resolved group {}", groupId);
                                       return "GROUP_" + groupId + "_MCPTT";
                                   }, "ALPHA");

        // Stage 3: Fetch floor control config (async)
        auto floorFut = std::async(std::launch::async, []() -> int {
            std::this_thread::sleep_for(25ms);
            std::println("[floor] floor config fetched");
            return 30;   // floor hold timeout in seconds
        });

        // Wait for all prerequisites
        bool authenticated = authFut.get();
        std::string groupName = groupFut.get();
        int floorTimeout = floorFut.get();

        if (!authenticated) {
            std::cout << "[MCX] authentication failed — aborting\n";
            return 1;
        }

        // Stage 4: Initiate the call (depends on all above)
        auto callFut = std::async(std::launch::async,
                                  [&groupName, floorTimeout]() -> int {
                                      std::this_thread::sleep_for(20ms);
                                      std::println("[call] starting {} with floor timeout {}s",
                                                   groupName, floorTimeout);
                                      return 1001;   // call ID
                                  });

        int callId = callFut.get();
        std::println("[MCX] call {} active on group {}", callId, groupName);

        // Stage 5: Request floor asynchronously while call is active
        std::promise<bool> floorPromise;
        std::future<bool>  floorGranted = floorPromise.get_future();

        std::thread floorController([&floorPromise, callId]() {
            std::this_thread::sleep_for(30ms);
            std::println("[floor controller] granting floor for call {}", callId);
            floorPromise.set_value(true);
        });

        if (floorGranted.get())
            std::println("[MCX] floor granted on call {} — transmitting", callId);

        floorController.join();

        // Stage 6: End call asynchronously
        auto endFut = std::async(std::launch::async, [callId]() {
            std::this_thread::sleep_for(15ms);
            std::println("[call] {} released", callId);
        });
        endFut.get();

        std::cout << "[MCX] call pipeline complete\n";
    }

    std::cout << "\n=== Async complete ===\n";
    return 0;
}
