#include <future>
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <stdexcept>
#include <numeric>
#include <mutex>
#include <print>

using namespace std::chrono_literals;

std::mutex g_coutMutex;
void safePrint(const std::string& msg) {
    std::lock_guard<std::mutex> lock(g_coutMutex);
    std::println("{}", msg);
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
    std::println("=====================================================");
    std::println("Example 1: std::async + std::future");
    std::println("=====================================================");
    auto sumFuture = std::async(std::launch::async, computeSum, 1, 100);
    std::println("[main] doing other work...");
    std::this_thread::sleep_for(20ms);
    std::println("[main] sum result: {}", sumFuture.get());

    std::println("");
    std::println("=====================================================");
    std::println("Example 2: launch::async vs launch::deferred");
    std::println("=====================================================");
    auto f1 = std::async(std::launch::async, []() {
        safePrint("[async] runs immediately in new thread"); return 1; });
    auto f2 = std::async(std::launch::deferred, []() {
        safePrint("[deferred] runs at .get() in calling thread"); return 2; });
    std::println("[main] before get()");
    std::println("async={} deferred={}", f1.get(), f2.get());

    std::println("");
    std::println("=====================================================");
    std::println("Example 3: std::promise + std::future");
    std::println("=====================================================");
    std::promise<int> prom; auto fut = prom.get_future();
    std::thread t1(workerWithPromise, std::move(prom), 21);
    std::println("[main] got: {}", fut.get()); t1.join();

    std::promise<int> promEx; auto futEx = promEx.get_future();
    std::thread t2(workerWithException, std::move(promEx));
    try { futEx.get(); }
    catch (const std::exception& e) {
        std::println("[main] caught from promise: {}", e.what()); }
    t2.join();

    std::println("");
    std::println("=====================================================");
    std::println("Example 4: std::packaged_task");
    std::println("=====================================================");
    std::packaged_task<int(int,int)> task(multiply);
    auto taskFut = task.get_future();
    std::thread t3(std::move(task), 6, 7);
    std::println("packaged_task result: {}", taskFut.get()); t3.join();

    std::println("");
    std::println("=====================================================");
    std::println("Example 5: Exception propagation");
    std::println("=====================================================");
    auto badTask = std::async(std::launch::async, []() -> int {
        throw std::runtime_error("async task failed!"); return 0; });
    try { badTask.get(); }
    catch (const std::exception& e) {
        std::println("[main] caught from async: {}", e.what()); }

    std::println("");
    std::println("=====================================================");
    std::println("Example 6: wait_for / wait_until");
    std::println("=====================================================");
    auto slowTask = std::async(std::launch::async, []() {
        std::this_thread::sleep_for(150ms); return 99; });
    auto status = slowTask.wait_for(50ms);
    if (status == std::future_status::timeout)
        std::println("[main] not ready yet — timeout");
    slowTask.wait();
    std::println("[main] now ready: {}", slowTask.get());

    std::println("");
    std::println("=====================================================");
    std::println("Example 7: std::shared_future (multiple consumers)");
    std::println("=====================================================");
    std::promise<std::string> sharedProm;
    std::shared_future<std::string> sharedFut = sharedProm.get_future().share();
    std::vector<std::thread> consumers;
    for (int i=0;i<4;++i)
        consumers.emplace_back([sharedFut, i]() {
            safePrint("[consumer " + std::to_string(i) + "] got: " + sharedFut.get()); });
    std::this_thread::sleep_for(30ms);
    sharedProm.set_value("broadcast message");
    for (auto& c : consumers) c.join();

    std::println("");
    std::println("=====================================================");
    std::println("Example 8: Parallel computation");
    std::println("=====================================================");
    const int SIZE = 1000000;
    std::vector<int> data(SIZE); std::iota(data.begin(), data.end(), 1);
    unsigned int cores = std::thread::hardware_concurrency();
    auto t_start = std::chrono::high_resolution_clock::now();
    long long result = parallelSum(data, cores);
    double ms = std::chrono::duration<double,std::milli>(
                    std::chrono::high_resolution_clock::now()-t_start).count();
    std::println("Parallel sum: {} in {} ms", result, ms);

    std::println("");
    std::println("=== Futures complete ===");
    return 0;
}
