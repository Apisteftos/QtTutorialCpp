#include <thread>
#include <chrono>
#include <pthread.h>  // For POSIX thread attributes (Linux/Unix)
#include <sched.h>    // For scheduling policies
#include <unistd.h>
#include <print>

// Global variable to demonstrate shared memory
int shared_counter = 0;

void demonstrate_thread_attributes() {
    // 1. THREAD ID (unique identifier)
    std::thread::id tid = std::this_thread::get_id();
    std::println("Thread ID: {}", tid);

    // 2. STACK (each thread has its own stack)
    int local_var = 42;  // Lives on this thread's stack
    std::println("Local variable address (on thread's stack): {}", static_cast<const void*>(&local_var));

    // 3. ACCESS TO SHARED MEMORY (heap and globals)
    shared_counter++;
    std::println("Shared counter (heap/global): {}", shared_counter);
    std::println("Shared counter address: {}", static_cast<const void*>(&shared_counter));

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void demonstrate_thread_state(int id) {
    std::println("Thread {} - RUNNING state", id);

    // Thread enters BLOCKED/WAITING state during sleep
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    std::println("Thread {} - Back to RUNNING", id);
    // Thread enters TERMINATED state after function completes
}

void demonstrate_program_counter() {
    // Each thread has its own Program Counter (instruction pointer)
    std::println("Thread executing at different code locations");
    for (int i = 0; i < 3; i++) {
        std::println("  Iteration {} in thread {}", i, std::this_thread::get_id());
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void demonstrate_registers(int value) {
    // Each thread has its own CPU registers
    // When context switching occurs, register values are saved/restored
    int a = value;
    int b = value * 2;
    int c = a + b;
    std::println("Thread {} register values: a={}, b={}, c={}", std::this_thread::get_id(), a, b, c);
}

// Function to show thread-local storage
thread_local int thread_local_var = 0;

void demonstrate_thread_local_storage(int id) {
    // Each thread gets its own copy of thread_local_var
    thread_local_var = id * 100;
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    std::println("Thread {} - thread_local_var = {}", id, thread_local_var);
}

int main() {
    std::println("========================================");
    std::println("THREAD ATTRIBUTES AND PROPERTIES");
    std::println("========================================");
    std::println("");

    // 1. THREAD ID (Unique Identifier)
    std::println("=== 1. THREAD ID ===");
    std::thread t1([]() {
        std::println("Thread ID: {}", std::this_thread::get_id());
    });
    std::println("From main - t1 ID: {}", t1.get_id());
    t1.join();
    std::println("");

    // 2. THREAD STATE (Lifecycle)
    std::println("=== 2. THREAD STATE ===");
    std::println("States: NEW -> RUNNING -> WAITING/BLOCKED -> RUNNING -> TERMINATED");
    std::thread t2(demonstrate_thread_state, 2);
    std::println("t2 is joinable (RUNNING): {}", t2.joinable());
    t2.join();
    std::println("t2 is joinable (TERMINATED): {}", t2.joinable());
    std::println("");

    // 3. STACK (Private per thread)
    std::println("=== 3. STACK (Private Memory) ===");
    std::thread t3(demonstrate_thread_attributes);
    std::thread t4(demonstrate_thread_attributes);
    t3.join();
    t4.join();
    std::thread t5(demonstrate_program_counter);
    std::thread t6(demonstrate_program_counter);
    t5.join();
    t6.join();
    std::println("");

    // 5. CPU REGISTERS (Context)
    std::println("=== 5. CPU REGISTERS (Context) ===");
    std::println("Each thread has its own register set:");
    std::thread t7(demonstrate_registers, 10);
    std::thread t8(demonstrate_registers, 20);
    t7.join();
    t8.join();
    std::println("");

    // 6. PRIORITY (Scheduling)
    std::println("=== 6. THREAD PRIORITY ===");
    std::thread t9([]() {
        std::println("Thread with scheduling attributes");
    });

    // Get native handle to access platform-specific attributes
    pthread_t native = t9.native_handle();

    // Get scheduling policy and priority (Linux/Unix)
    int policy;
    sched_param param;
    pthread_getschedparam(native, &policy, &param);

    std::println("Scheduling policy: {}", policy);
    std::println("Thread priority: {}", param.sched_priority);

    // Note: Modifying priority often requires root privileges
    // param.sched_priority = 50;
    // pthread_setschedparam(native, SCHED_FIFO, &param);

    t9.join();
    std::println("");

    // 7. THREAD-LOCAL STORAGE
    std::println("=== 7. THREAD-LOCAL STORAGE ===");
    std::println("Each thread has its own copy of thread_local variables:");
    std::thread t10(demonstrate_thread_local_storage, 1);
    std::thread t11(demonstrate_thread_local_storage, 2);
    std::thread t12(demonstrate_thread_local_storage, 3);
    t10.join();
    t11.join();
    t12.join();
    std::println("");

    // 8. PARENT PROCESS
    std::println("=== 8. PARENT PROCESS ===");
    std::println("All threads belong to the same process");
    std::println("Process ID (getpid): {}", getpid());
    std::thread t13([]() {
        std::println("Thread's process ID: {} (same as main)", getpid());
    });
    t13.join();
    std::println("");

    // 9. EXECUTION STATE (Joinable/Detached)
    std::println("=== 9. EXECUTION STATE ===");
    std::thread t14([]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::println("Joinable thread completed");
    });

    std::thread t15([]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::println("Detached thread completed");
    });

    std::println("t14 is joinable: {}", t14.joinable());
    std::println("t15 is joinable: {}", t15.joinable());

    t14.join();
    t15.detach();

    std::println("After join/detach:");
    std::println("t14 is joinable: {}", t14.joinable());
    std::println("t15 is joinable: {}", t15.joinable());
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::println("");

    // 10. HARDWARE AFFINITY (which CPU core)
    std::println("=== 10. CPU AFFINITY ===");
    std::println("Threads can be bound to specific CPU cores");
    std::println("Available CPU cores: {}", std::thread::hardware_concurrency());

    std::thread t16([]() {
        std::println("Thread running (OS decides which CPU core)");
        // On Linux: pthread_setaffinity_np can pin thread to specific core
    });
    t16.join();
    std::println("");

    // 11. RESOURCE LIMITS
    std::println("=== 11. RESOURCE LIMITS ===");
    std::thread t17([]() {
        // Each thread has stack size limits (usually 2-8 MB default)
        std::println("Default stack size varies by system");
        std::println("Can be set using pthread_attr_setstacksize");
    });
    t17.join();
    return 0;
}
