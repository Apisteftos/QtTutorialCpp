#include <thread>
#include <chrono>
#include <vector>
#include <print>

void task1() {
    std::println("Thread 1 is running. ID: {}", std::this_thread::get_id());
    // Simulate some work
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::println("Thread 1 finished work");
}

void task2() {
    std::println("Thread 2 is running. ID: {}", std::this_thread::get_id());
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    std::println("Thread 2 finished work");
}

// Function that takes parameters
void task_with_params(int id, std::string message) {
    std::println("Thread {} says: {}", id, message);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

// Function that demonstrates yielding
void task_yield() {
    for (int i = 0; i < 5; i++) {
        std::println("Yielding thread iteration {}", i);
        // this_thread::yield() gives up CPU to other threads voluntarily
        std::this_thread::yield();
    }
}

int main() {
    std::println("=== MAIN THREAD INFO ===");
    // Get main thread ID
    std::println("Main thread ID: {}", std::this_thread::get_id());

    // Get number of concurrent threads supported by hardware
    unsigned int num_threads = std::thread::hardware_concurrency();
    std::println("Hardware concurrency (CPU cores): {}", num_threads);
    std::println("");

    std::println("=== BASIC THREAD OPERATIONS ===");
    std::thread t1(task1);
    std::thread t2(task2);

    // Get thread IDs (native handles to thread objects)
    std::println("t1 ID: {}", t1.get_id());
    std::println("t2 ID: {}", t2.get_id());

    // Get native thread handle (platform-specific)
    auto native_handle = t1.native_handle();
    std::println("t1 native handle: {}", native_handle);
    std::println("");

    // Check if threads are joinable
    std::println("t1 joinable? {}", (t1.joinable() ? "Yes" : "No"));
    std::println("t2 joinable? {}", (t2.joinable() ? "Yes" : "No"));
    std::println("");

    // Join t1 - main thread waits for t1 to complete
    if (t1.joinable()) {
        t1.join();
        std::println("t1 joined (completed)");
        // After join, thread is no longer joinable
        std::println("t1 joinable after join? {}", (t1.joinable() ? "Yes" : "No"));
        std::println("");
    }

    // Detach t2 - t2 runs independently, no need to wait
    if (t2.joinable()) {
        t2.detach();
        std::println("t2 detached (running independently)");
        // After detach, thread is no longer joinable
        std::println("t2 joinable after detach? {}", (t2.joinable() ? "Yes" : "No"));
        std::println("");
    }

    std::println("=== THREAD WITH PARAMETERS ===");
    // Create thread with parameters - pass by value
    std::thread t3(task_with_params, 3, "Hello from thread 3!");
    t3.join();
    std::println("");

    std::println("=== THREAD WITH LAMBDA ===");
    // Create thread using lambda function
    std::thread t4([]() {
        std::println("Lambda thread ID: {}", std::this_thread::get_id());
        std::println("Lambda thread executing!");
    });
    t4.join();
    std::println("");

    std::println("=== THREAD YIELDING ===");
    // Thread that yields CPU voluntarily
    std::thread t5(task_yield);
    t5.join();
    std::println("");

    std::println("=== MOVE SEMANTICS ===");
    std::thread t6([]() {
        std::println("Thread t6 executing");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    });

    // Move thread ownership (threads cannot be copied, only moved)
    std::thread t7 = move(t6);
    std::println("t6 ID after move: {} (empty thread)", t6.get_id());
    std::println("t7 ID after move: {} (owns the thread)", t7.get_id());
    std::println("t6 joinable? {}", (t6.joinable() ? "Yes" : "No"));
    std::println("t7 joinable? {}", (t7.joinable() ? "Yes" : "No"));
    t7.join();
    std::println("");

    std::println("=== MULTIPLE THREADS (VECTOR) ===");
    // Create and manage multiple threads in a container
    std::vector<std::thread> threads;
    for (int i = 0; i < 3; i++) {
        // emplace_back constructs thread in-place
        threads.emplace_back([i]() {
            std::println("Worker thread {} ID: {}", i, std::this_thread::get_id());
            std::this_thread::sleep_for(std::chrono::milliseconds(100 * (i + 1)));
            std::println("Worker thread {} done", i);
        });
    }

    // Join all threads
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
    std::println("");

    std::println("=== SWAP OPERATION ===");
    std::thread t8([]() { std::println("Thread t8"); });
    std::thread t9([]() { std::println("Thread t9"); });

    std::println("Before swap - t8 ID: {}, t9 ID: {}", t8.get_id(), t9.get_id());
    // Swap two threads
    t8.swap(t9);
    std::println("After swap - t8 ID: {}, t9 ID: {}", t8.get_id(), t9.get_id());
    t8.join();
    t9.join();
    std::println("");

    std::println("=== SLEEP OPERATIONS ===");
    auto start = std::chrono::high_resolution_clock::now();

    // Sleep for exact duration
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::println("Slept for approximately: {} milliseconds", duration.count());

    // Sleep until specific time point
    auto wake_time = std::chrono::system_clock::now() + std::chrono::milliseconds(200);
    std::this_thread::sleep_until(wake_time);
    return 0;
}
