#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <queue>
#include <vector>
#include <functional>
#include <stdexcept>
#include <atomic>
#include <chrono>
#include <string>
#include <numeric>

using namespace std::chrono_literals;

std::mutex g_coutMutex;
void safePrint(const std::string& msg) {
    std::lock_guard<std::mutex> lock(g_coutMutex);
    std::cout << msg << '\n';
}

// ============================================================
// Thread pool with futures (return values)
// ============================================================
class ThreadPool {
private:
    std::vector<std::thread>          m_workers;
    std::queue<std::function<void()>> m_tasks;
    std::mutex                        m_mutex;
    std::condition_variable           m_cv;
    std::atomic<bool>                 m_stop{false};
    std::atomic<int>                  m_pending{0};

public:
    explicit ThreadPool(int n) {
        for (int i=0;i<n;++i) {
            m_workers.emplace_back([this, i]() {
                safePrint("[pool worker " + std::to_string(i) + "] ready");
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(m_mutex);
                        m_cv.wait(lock, [this]() { return !m_tasks.empty() || m_stop.load(); });
                        if (m_stop && m_tasks.empty()) return;
                        task = std::move(m_tasks.front());
                        m_tasks.pop();
                    }
                    task();
                    --m_pending;
                }
            });
        }
    }

    template<typename Func, typename... Args>
    auto submit(Func&& func, Args&&... args)
        -> std::future<std::invoke_result_t<Func, Args...>>
    {
        using R = std::invoke_result_t<Func, Args...>;
        auto task = std::make_shared<std::packaged_task<R()>>(
            std::bind(std::forward<Func>(func), std::forward<Args>(args)...));
        std::future<R> fut = task->get_future();
        ++m_pending;
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_stop) throw std::runtime_error("Pool stopped");
            m_tasks.emplace([task]() { (*task)(); });
        }
        m_cv.notify_one();
        return fut;
    }

    void waitAll() { while (m_pending > 0) std::this_thread::sleep_for(1ms); }
    int  workers()  const { return (int)m_workers.size(); }

    ~ThreadPool() {
        m_stop = true;
        m_cv.notify_all();
        for (auto& t : m_workers) if (t.joinable()) t.join();
    }
};

int main()
{
    unsigned int cores = std::thread::hardware_concurrency();
    std::cout << "Hardware cores: " << cores << '\n';

    std::cout << "\n=====================================================\n";
    std::cout << "Example 1: Submit void tasks\n";
    std::cout << "=====================================================\n";
    {
        ThreadPool pool(4);
        for (int i=0;i<8;++i) {
            pool.submit([i]() {
                safePrint("[task " + std::to_string(i) + "] running");
                std::this_thread::sleep_for(std::chrono::milliseconds(10+i*5));
                safePrint("[task " + std::to_string(i) + "] done");
            });
        }
        pool.waitAll();
        std::cout << "All tasks done\n";
    }

    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: Submit with return values\n";
    std::cout << "=====================================================\n";
    {
        ThreadPool pool(cores);
        auto f1 = pool.submit([](int a, int b) { std::this_thread::sleep_for(20ms); return a+b; }, 10, 20);
        auto f2 = pool.submit([](int n) { long long s=0; for(int i=1;i<=n;++i)s+=i; return s; }, 100);
        auto f3 = pool.submit([]() -> std::string { std::this_thread::sleep_for(15ms); return "hello from pool"; });
        std::cout << "f1 (10+20):     " << f1.get() << '\n';
        std::cout << "f2 (sum 1-100): " << f2.get() << '\n';
        std::cout << "f3 (string):    " << f3.get() << '\n';
    }

    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: Exception handling\n";
    std::cout << "=====================================================\n";
    {
        ThreadPool pool(2);
        auto ok  = pool.submit([]() { return 42; });
        auto bad = pool.submit([]() -> int { throw std::runtime_error("task failed!"); return 0; });
        std::cout << "OK task: " << ok.get() << '\n';
        try { bad.get(); } catch (const std::exception& e) {
            std::cout << "Caught: " << e.what() << '\n'; }
    }

    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: Parallel computation\n";
    std::cout << "=====================================================\n";
    {
        ThreadPool pool(cores);
        const int SIZE = 1000000;
        std::vector<int> data(SIZE); std::iota(data.begin(), data.end(), 1);
        int chunk = SIZE / cores;
        std::vector<std::future<long long>> futures;
        for (int i=0;i<(int)cores;++i) {
            int s = i * chunk;
            int e = (i==(int)cores-1) ? SIZE : s+chunk;
            futures.push_back(pool.submit([&data,s,e]() -> long long {
                long long sum=0; for(int j=s;j<e;++j)sum+=data[j]; return sum; }));
        }
        long long total=0; for (auto& f:futures) total+=f.get();
        std::cout << "Parallel sum: " << total << '\n';
        std::cout << "Expected:     " << (long long)SIZE*(SIZE+1)/2 << '\n';
    }

    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: Task pipeline (chained futures)\n";
    std::cout << "=====================================================\n";
    {
        ThreadPool pool(4);
        auto fetch = pool.submit([]() -> std::string {
            std::this_thread::sleep_for(30ms); safePrint("[stage 1] fetched"); return "raw_data"; });
        std::string raw = fetch.get();
        auto parse = pool.submit([raw]() -> int {
            std::this_thread::sleep_for(20ms); safePrint("[stage 2] parsed: " + raw); return 123; });
        int parsed = parse.get();
        auto process = pool.submit([parsed]() -> std::string {
            safePrint("[stage 3] processed: " + std::to_string(parsed));
            return "result_" + std::to_string(parsed * 2); });
        std::cout << "Pipeline result: " << process.get() << '\n';
    }

    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: Pool vs raw threads (performance)\n";
    std::cout << "=====================================================\n";
    {
        const int N = 100;
        auto t1 = std::chrono::high_resolution_clock::now();
        { std::vector<std::thread> raw;
            for (int i=0;i<N;++i) raw.emplace_back([](){std::this_thread::sleep_for(1ms);});
            for (auto& t:raw) t.join(); }
        double rawMs = std::chrono::duration<double,std::milli>(
                           std::chrono::high_resolution_clock::now()-t1).count();

        auto t2 = std::chrono::high_resolution_clock::now();
        { ThreadPool pool(cores);
            std::vector<std::future<void>> futs;
            for (int i=0;i<N;++i) futs.push_back(pool.submit([](){std::this_thread::sleep_for(1ms);}));
            for (auto& f:futs) f.get(); }
        double poolMs = std::chrono::duration<double,std::milli>(
                            std::chrono::high_resolution_clock::now()-t2).count();

        std::cout << N << " tasks — raw: " << rawMs << " ms | pool: " << poolMs << " ms\n";
        std::cout << "Pool speedup: " << rawMs/poolMs << "x\n";
    }

    std::cout << "\n=== ThreadPool complete ===\n";
    return 0;
}
