#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
using namespace std;

void task1() {
    cout << "Thread 1 is running. ID: " << this_thread::get_id() << "\n";
    // Simulate some work
    this_thread::sleep_for(chrono::milliseconds(500));
    cout << "Thread 1 finished work\n";
}

void task2() {
    cout << "Thread 2 is running. ID: " << this_thread::get_id() << "\n";
    this_thread::sleep_for(chrono::milliseconds(300));
    cout << "Thread 2 finished work\n";
}

// Function that takes parameters
void task_with_params(int id, string message) {
    cout << "Thread " << id << " says: " << message << "\n";
    this_thread::sleep_for(chrono::milliseconds(200));
}

// Function that demonstrates yielding
void task_yield() {
    for (int i = 0; i < 5; i++) {
        cout << "Yielding thread iteration " << i << "\n";
        // this_thread::yield() gives up CPU to other threads voluntarily
        this_thread::yield();
    }
}

int main() {
    cout << "=== MAIN THREAD INFO ===\n";
    // Get main thread ID
    cout << "Main thread ID: " << this_thread::get_id() << "\n";

    // Get number of concurrent threads supported by hardware
    unsigned int num_threads = thread::hardware_concurrency();
    cout << "Hardware concurrency (CPU cores): " << num_threads << "\n\n";

    cout << "=== BASIC THREAD OPERATIONS ===\n";
    thread t1(task1);
    thread t2(task2);

    // Get thread IDs (native handles to thread objects)
    cout << "t1 ID: " << t1.get_id() << "\n";
    cout << "t2 ID: " << t2.get_id() << "\n";

    // Get native thread handle (platform-specific)
    auto native_handle = t1.native_handle();
    cout << "t1 native handle: " << native_handle << "\n\n";

    // Check if threads are joinable
    cout << "t1 joinable? " << (t1.joinable() ? "Yes" : "No") << "\n";
    cout << "t2 joinable? " << (t2.joinable() ? "Yes" : "No") << "\n\n";

    // Join t1 - main thread waits for t1 to complete
    if (t1.joinable()) {
        t1.join();
        cout << "t1 joined (completed)\n";
        // After join, thread is no longer joinable
        cout << "t1 joinable after join? " << (t1.joinable() ? "Yes" : "No") << "\n\n";
    }

    // Detach t2 - t2 runs independently, no need to wait
    if (t2.joinable()) {
        t2.detach();
        cout << "t2 detached (running independently)\n";
        // After detach, thread is no longer joinable
        cout << "t2 joinable after detach? " << (t2.joinable() ? "Yes" : "No") << "\n\n";
    }

    cout << "=== THREAD WITH PARAMETERS ===\n";
    // Create thread with parameters - pass by value
    thread t3(task_with_params, 3, "Hello from thread 3!");
    t3.join();
    cout << "\n";

    cout << "=== THREAD WITH LAMBDA ===\n";
    // Create thread using lambda function
    thread t4([]() {
        cout << "Lambda thread ID: " << this_thread::get_id() << "\n";
        cout << "Lambda thread executing!\n";
    });
    t4.join();
    cout << "\n";

    cout << "=== THREAD YIELDING ===\n";
    // Thread that yields CPU voluntarily
    thread t5(task_yield);
    t5.join();
    cout << "\n";

    cout << "=== MOVE SEMANTICS ===\n";
    thread t6([]() {
        cout << "Thread t6 executing\n";
        this_thread::sleep_for(chrono::milliseconds(100));
    });

    // Move thread ownership (threads cannot be copied, only moved)
    thread t7 = move(t6);
    cout << "t6 ID after move: " << t6.get_id() << " (empty thread)\n";
    cout << "t7 ID after move: " << t7.get_id() << " (owns the thread)\n";
    cout << "t6 joinable? " << (t6.joinable() ? "Yes" : "No") << "\n";
    cout << "t7 joinable? " << (t7.joinable() ? "Yes" : "No") << "\n";
    t7.join();
    cout << "\n";

    cout << "=== MULTIPLE THREADS (VECTOR) ===\n";
    // Create and manage multiple threads in a container
    vector<thread> threads;
    for (int i = 0; i < 3; i++) {
        // emplace_back constructs thread in-place
        threads.emplace_back([i]() {
            cout << "Worker thread " << i << " ID: " << this_thread::get_id() << "\n";
            this_thread::sleep_for(chrono::milliseconds(100 * (i + 1)));
            cout << "Worker thread " << i << " done\n";
        });
    }

    // Join all threads
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
    cout << "\n";

    cout << "=== SWAP OPERATION ===\n";
    thread t8([]() { cout << "Thread t8\n"; });
    thread t9([]() { cout << "Thread t9\n"; });

    cout << "Before swap - t8 ID: " << t8.get_id() << ", t9 ID: " << t9.get_id() << "\n";
    // Swap two threads
    t8.swap(t9);
    cout << "After swap - t8 ID: " << t8.get_id() << ", t9 ID: " << t9.get_id() << "\n";
    t8.join();
    t9.join();
    cout << "\n";

    cout << "=== SLEEP OPERATIONS ===\n";
    auto start = chrono::high_resolution_clock::now();

    // Sleep for exact duration
    this_thread::sleep_for(chrono::milliseconds(500));

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Slept for approximately: " << duration.count() << " milliseconds\n";

    // Sleep until specific time point
    auto wake_time = chrono::system_clock::now() + chrono::milliseconds(200);
    this_thread::sleep_until(wake_time);
    cout << "Woke up at specified time\n\n";

    cout << "=== MAIN THREAD ENDING ===\n";
    cout << "Main thread awake and finishing.\n";

    return 0;
}
