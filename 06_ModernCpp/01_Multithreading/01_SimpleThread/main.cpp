#include <iostream>
#include <thread>
#include <chrono>
#include <string>

// ============================================================
// SIMPLE THREAD — Fundamentals of std::thread in C++23
// ============================================================
// Topics covered:
//   1. What a thread is (process vs thread)
//   2. Creating a thread with a free function
//   3. Creating a thread with a lambda
//   4. join() — wait for thread to finish
//   5. detach() — let thread run independently
//   6. Threads run CONCURRENTLY — order is not guaranteed
//   7. Each thread has its own stack
//   8. joinable() check
//   9. Thread ID
//  10. Main thread is also a thread
// ============================================================


// ============================================================
// Helper: small delay to make concurrency visible in output
// ============================================================
void sleepMs(int ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}


// ============================================================
// Example 1 — The simplest possible thread
// ============================================================
void helloFromThread()
{
    std::cout << "[thread] Hello from a worker thread!\n";
}


// ============================================================
// Example 2 — Thread with parameters
// ============================================================
void printMessage(int id, const std::string& msg)
{
    std::cout << "[thread " << id << "] " << msg << '\n';
}


// ============================================================
// Example 3 — Showing concurrency: order is NOT guaranteed
// ============================================================
void workerA()
{
    for (int i = 0; i < 5; ++i) {
        std::cout << "[A] step " << i << '\n';
        sleepMs(10);
    }
}

void workerB()
{
    for (int i = 0; i < 5; ++i) {
        std::cout << "[B] step " << i << '\n';
        sleepMs(15);
    }
}


// ============================================================
// Example 4 — Each thread has its own STACK
// Local variables are private per thread
// ============================================================
void showStack(int threadId)
{
    int localVar  = threadId * 100;   // private to THIS thread's stack
    int localVar2 = threadId * 200;

    std::cout << "[thread " << threadId << "] "
              << "localVar="  << localVar
              << " localVar2=" << localVar2
              << " (stack address: " << &localVar << ")\n";
}


// ============================================================
// Example 5 — detach(): fire and forget
// ============================================================
void backgroundTask(int id)
{
    sleepMs(50);
    std::cout << "[detached " << id << "] background task done\n";
}


// ============================================================
// MAIN
// ============================================================
int main()
{
    // -------------------------------------------------------
    // What is a thread?
    // -------------------------------------------------------
    // A thread is an independent sequence of execution inside
    // a process. All threads share:
    //   - heap memory
    //   - global variables
    //   - code
    // Each thread has its own:
    //   - stack
    //   - program counter (where it is in the code)
    //   - CPU registers
    //
    // std::thread starts a new thread immediately at construction.
    // There is no .start() method — it runs right away.
    // -------------------------------------------------------

    std::cout << "=====================================================\n";
    std::cout << "Example 1: Simplest thread — free function\n";
    std::cout << "=====================================================\n";

    std::cout << "[main] About to create thread...\n";
    std::thread t1(helloFromThread);    // thread starts HERE immediately
    std::cout << "[main] Thread created — now calling join()\n";
    t1.join();                          // main waits here until t1 finishes
    std::cout << "[main] Thread finished — continuing\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: Thread with parameters\n";
    std::cout << "=====================================================\n";

    // Arguments are passed after the callable
    // They are COPIED into the thread by default
    std::thread t2(printMessage, 1, "First message");
    std::thread t3(printMessage, 2, "Second message");

    t2.join();
    t3.join();


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: Lambda thread\n";
    std::cout << "=====================================================\n";

    // Lambdas are the most common way to create threads
    std::thread t4([]() {
        std::cout << "[lambda thread] Running inline code\n";
    });
    t4.join();

    // Lambda with capture
    int value = 42;
    std::thread t5([value]() {
        std::cout << "[lambda thread] Captured value = " << value << '\n';
    });
    t5.join();


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: Concurrency — order is NOT guaranteed\n";
    std::cout << "=====================================================\n";

    std::cout << "[main] Starting workerA and workerB — watch the interleaving:\n";
    std::thread tA(workerA);
    std::thread tB(workerB);

    // Both run concurrently — you will see A and B steps mixed
    tA.join();
    tB.join();
    std::cout << "[main] Both workers done\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: Each thread has its own STACK\n";
    std::cout << "=====================================================\n";

    std::cout << "[main] Launching 3 threads — each has own local variables:\n";
    std::thread ts1(showStack, 1);
    std::thread ts2(showStack, 2);
    std::thread ts3(showStack, 3);

    ts1.join();
    ts2.join();
    ts3.join();
    std::cout << "[main] Notice: different stack addresses for each thread\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: Thread ID\n";
    std::cout << "=====================================================\n";

    std::cout << "[main] Main thread ID: " << std::this_thread::get_id() << '\n';

    std::thread tid1([]() {
        std::cout << "[thread] My ID: " << std::this_thread::get_id() << '\n';
    });
    std::thread tid2([]() {
        std::cout << "[thread] My ID: " << std::this_thread::get_id() << '\n';
    });

    std::cout << "[main] tid1 ID (from outside): " << tid1.get_id() << '\n';
    std::cout << "[main] tid2 ID (from outside): " << tid2.get_id() << '\n';

    tid1.join();
    tid2.join();


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: joinable() check\n";
    std::cout << "=====================================================\n";

    std::thread tj([]() { sleepMs(20); });

    std::cout << "Before join — joinable: " << (tj.joinable() ? "YES" : "NO") << '\n';
    tj.join();
    std::cout << "After  join — joinable: " << (tj.joinable() ? "YES" : "NO") << '\n';

    // Default constructed thread — no associated thread
    std::thread empty;
    std::cout << "Default thread — joinable: " << (empty.joinable() ? "YES" : "NO") << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: detach() — fire and forget\n";
    std::cout << "=====================================================\n";

    std::cout << "[main] Launching detached background threads...\n";

    std::thread td1(backgroundTask, 1);
    std::thread td2(backgroundTask, 2);

    td1.detach();   // runs independently — main does NOT wait
    td2.detach();

    std::cout << "[main] Detached — main continues without waiting\n";
    std::cout << "[main] td1 joinable after detach: "
              << (td1.joinable() ? "YES" : "NO") << '\n';

    // Give detached threads time to finish before main exits
    // In real code you would use a synchronization mechanism instead
    sleepMs(200);
    std::cout << "[main] (gave detached threads time to finish)\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 9: What happens WITHOUT join or detach\n";
    std::cout << "=====================================================\n";

    // If a joinable thread goes out of scope: std::terminate() → crash
    // This is commented out to avoid crashing the program:
    //
    // {
    //     std::thread bad([]() { sleepMs(100); });
    // }  // CRASH — bad is joinable but neither joined nor detached
    //
    // Always join or detach every thread.

    std::cout << "[main] (crash example skipped — see comments in code)\n";
    std::cout << "[main] Rule: EVERY thread must be joined OR detached\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 10: hardware_concurrency\n";
    std::cout << "=====================================================\n";

    unsigned int cores = std::thread::hardware_concurrency();
    std::cout << "Logical CPU cores available: " << cores << '\n';
    std::cout << "This is the ideal number of threads for CPU-bound work\n";


    std::cout << "\n=== SimpleThread complete ===\n";
    return 0;
}
