#include <iostream>
#include <future>
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <stdexcept>
#include <numeric>
#include <mutex>

using namespace std::chrono_literals;

std::mutex g_coutMutex;
void safePrint(const std::string& msg) {
    std::lock_guard<std::mutex> lock(g_coutMutex);
    std::cout << msg << '\n';
}

int computeSum(int from, int to) {
    safePrint("[compute] summing " + std::to_string(from) + " to " + std::to_string(to));
    std::this_thread::sleep_for(50ms);
    int sum = 0; for (int i=from;i<=to;++i) sum+=i; return sum;
}

void workerWithPromise(std::promise<int> prom, int value) {
    std::this_thread::sleep_for(40ms);
    prom.set_value(value * 2);
    safePrint("[promise worker] set value: " + std::to_string(value * 2));
}
void workerWithException(std::promise<int> prom) {
    std::this_thread::sleep_for(20ms);
    try { throw std::runtime_error("worker failed!"); }
    catch (...) { prom.set_exception(std::current_exception()); }
}

int multiply(int a, int b) {
    std::this_thread::sleep_for(20ms);
    return a * b;
}

long long parallelSum(const std::vector<int>& data, int numTasks) {
    int chunkSize = (int)data.size() / numTasks;
    std::vector<std::future<long long>> futures;
    for (int i=0;i<numTasks;++i) {
        int start = i * chunkSize;
        int end   = (i == numTasks-1) ? (int)data.size() : start + chunkSize;
        futures.push_back(std::async(std::launch::async,
                                     [&data, start, end]() -> long long {
                                         long long s=0; for (int j=start;j<end;++j) s+=data[j]; return s;
                                     }));
    }
    long long total=0; for (auto& f : futures) total += f.get();
    return total;
}

int main()
{
    std::cout << "=====================================================\n";
    std::cout << "Example 1: std::async + std::future\n";
    std::cout << "=====================================================\n";
    auto sumFuture = std::async(std::launch::async, computeSum, 1, 100);
    std::cout << "[main] doing other work...\n";
    std::this_thread::sleep_for(20ms);
    std::cout << "[main] sum result: " << sumFuture.get() << '\n';

    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: launch::async vs launch::deferred\n";
    std::cout << "=====================================================\n";
    auto f1 = std::async(std::launch::async, []() {
        safePrint("[async] runs immediately in new thread"); return 1; });
    auto f2 = std::async(std::launch::deferred, []() {
        safePrint("[deferred] runs at .get() in calling thread"); return 2; });
    std::cout << "[main] before get()\n";
    std::cout << "async=" << f1.get() << " deferred=" << f2.get() << '\n';

    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: std::promise + std::future\n";
    std::cout << "=====================================================\n";
    std::promise<int> prom; auto fut = prom.get_future();
    std::thread t1(workerWithPromise, std::move(prom), 21);
    std::cout << "[main] got: " << fut.get() << '\n'; t1.join();

    std::promise<int> promEx; auto futEx = promEx.get_future();
    std::thread t2(workerWithException, std::move(promEx));
    try { futEx.get(); }
    catch (const std::exception& e) {
        std::cout << "[main] caught from promise: " << e.what() << '\n'; }
    t2.join();

    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: std::packaged_task\n";
    std::cout << "=====================================================\n";
    std::packaged_task<int(int,int)> task(multiply);
    auto taskFut = task.get_future();
    std::thread t3(std::move(task), 6, 7);
    std::cout << "packaged_task result: " << taskFut.get() << '\n'; t3.join();

    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: Exception propagation\n";
    std::cout << "=====================================================\n";
    auto badTask = std::async(std::launch::async, []() -> int {
        throw std::runtime_error("async task failed!"); return 0; });
    try { badTask.get(); }
    catch (const std::exception& e) {
        std::cout << "[main] caught from async: " << e.what() << '\n'; }

    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: wait_for / wait_until\n";
    std::cout << "=====================================================\n";
    auto slowTask = std::async(std::launch::async, []() {
        std::this_thread::sleep_for(150ms); return 99; });
    auto status = slowTask.wait_for(50ms);
    if (status == std::future_status::timeout)
        std::cout << "[main] not ready yet — timeout\n";
    slowTask.wait();
    std::cout << "[main] now ready: " << slowTask.get() << '\n';

    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: std::shared_future (multiple consumers)\n";
    std::cout << "=====================================================\n";
    std::promise<std::string> sharedProm;
    std::shared_future<std::string> sharedFut = sharedProm.get_future().share();
    std::vector<std::thread> consumers;
    for (int i=0;i<4;++i)
        consumers.emplace_back([sharedFut, i]() {
            safePrint("[consumer " + std::to_string(i) + "] got: " + sharedFut.get()); });
    std::this_thread::sleep_for(30ms);
    sharedProm.set_value("broadcast message");
    for (auto& c : consumers) c.join();

    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: Parallel computation\n";
    std::cout << "=====================================================\n";
    const int SIZE = 1000000;
    std::vector<int> data(SIZE); std::iota(data.begin(), data.end(), 1);
    unsigned int cores = std::thread::hardware_concurrency();
    auto t_start = std::chrono::high_resolution_clock::now();
    long long result = parallelSum(data, cores);
    double ms = std::chrono::duration<double,std::milli>(
                    std::chrono::high_resolution_clock::now()-t_start).count();
    std::cout << "Parallel sum: " << result << " in " << ms << " ms\n";

    std::cout << "\n=== Futures complete ===\n";
    return 0;
}
