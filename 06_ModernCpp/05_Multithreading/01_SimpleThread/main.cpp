#include <thread>
#include <chrono>
#include <string>
#include <print>

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
    std::println("[thread] Hello from a worker thread!");
}


// ============================================================
// Example 2 — Thread with parameters
// ============================================================
void printMessage(int id, const std::string& msg)
{
    std::println("[thread {}] {}", id, msg);
}


// ============================================================
// Example 3 — Showing concurrency: order is NOT guaranteed
// ============================================================
void workerA()
{
    for (int i = 0; i < 5; ++i) {
        std::println("[A] step {}", i);
        sleepMs(10);
    }
}

void workerB()
{
    for (int i = 0; i < 5; ++i) {
        std::println("[B] step {}", i);
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

    std::println("[thread {}] localVar={} localVar2={} (stack address: {})",
        threadId, localVar, localVar2, static_cast<const void*>(&localVar));
}


// ============================================================
// Example 5 — detach(): fire and forget
// ============================================================
void backgroundTask(int id)
{
    sleepMs(50);
    std::println("[detached {}] background task done", id);
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

    std::println("=====================================================");
    std::println("Example 1: Simplest thread — free function");
    std::println("=====================================================");

    std::println("[main] About to create thread...");
    std::thread t1(helloFromThread);    // thread starts HERE immediately
    std::println("[main] Thread created — now calling join()");
    t1.join();                          // main waits here until t1 finishes
    std::println("[main] Thread finished — continuing");


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 2: Thread with parameters");
    std::println("=====================================================");

    // Arguments are passed after the callable
    // They are COPIED into the thread by default
    std::thread t2(printMessage, 1, "First message");
    std::thread t3(printMessage, 2, "Second message");

    t2.join();
    t3.join();


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 3: Lambda thread");
    std::println("=====================================================");

    // Lambdas are the most common way to create threads
    std::thread t4([]() {
        std::println("[lambda thread] Running inline code");
    });
    t4.join();

    // Lambda with capture
    int value = 42;
    std::thread t5([value]() {
        std::println("[lambda thread] Captured value = {}", value);
    });
    t5.join();


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 4: Concurrency — order is NOT guaranteed");
    std::println("=====================================================");

    std::println("[main] Starting workerA and workerB — watch the interleaving:");
    std::thread tA(workerA);
    std::thread tB(workerB);

    // Both run concurrently — you will see A and B steps mixed
    tA.join();
    tB.join();
    std::println("[main] Both workers done");


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 5: Each thread has its own STACK");
    std::println("=====================================================");

    std::println("[main] Launching 3 threads — each has own local variables:");
    std::thread ts1(showStack, 1);
    std::thread ts2(showStack, 2);
    std::thread ts3(showStack, 3);

    ts1.join();
    ts2.join();
    ts3.join();
    std::println("[main] Notice: different stack addresses for each thread");


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 6: Thread ID");
    std::println("=====================================================");

    std::println("[main] Main thread ID: {}", std::this_thread::get_id());

    std::thread tid1([]() {
        std::println("[thread] My ID: {}", std::this_thread::get_id());
    });
    std::thread tid2([]() {
        std::println("[thread] My ID: {}", std::this_thread::get_id());
    });

    std::println("[main] tid1 ID (from outside): {}", tid1.get_id());
    std::println("[main] tid2 ID (from outside): {}", tid2.get_id());

    tid1.join();
    tid2.join();


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 7: joinable() check");
    std::println("=====================================================");

    std::thread tj([]() { sleepMs(20); });

    std::println("Before join — joinable: {}", (tj.joinable() ? "YES" : "NO"));
    tj.join();
    std::println("After  join — joinable: {}", (tj.joinable() ? "YES" : "NO"));

    // Default constructed thread — no associated thread
    std::thread empty;
    std::println("Default thread — joinable: {}", (empty.joinable() ? "YES" : "NO"));


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 8: detach() — fire and forget");
    std::println("=====================================================");

    std::println("[main] Launching detached background threads...");

    std::thread td1(backgroundTask, 1);
    std::thread td2(backgroundTask, 2);

    td1.detach();   // runs independently — main does NOT wait
    td2.detach();

    std::println("[main] Detached — main continues without waiting");
    std::println("[main] td1 joinable after detach: {}", (td1.joinable() ? "YES" : "NO"));

    // Give detached threads time to finish before main exits
    // In real code you would use a synchronization mechanism instead
    sleepMs(200);
    std::println("[main] (gave detached threads time to finish)");


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 9: hardware_concurrency");
    std::println("=====================================================");

    unsigned int cores = std::thread::hardware_concurrency();
    std::println("Logical CPU cores available: {}", cores);
    std::println("This is the ideal number of threads for CPU-bound work");


    std::println("");
    std::println("=== SimpleThread complete ===");
    return 0;
}
