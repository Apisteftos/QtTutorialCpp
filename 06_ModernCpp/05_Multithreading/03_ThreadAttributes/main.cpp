#include <iostream>
#include <thread>
#include <chrono>
#include <pthread.h>  // For POSIX thread attributes (Linux/Unix)
#include <sched.h>    // For scheduling policies
#include <unistd.h>
using namespace std;

// Global variable to demonstrate shared memory
int shared_counter = 0;

void demonstrate_thread_attributes() {
    // 1. THREAD ID (unique identifier)
    thread::id tid = this_thread::get_id();
    cout << "Thread ID: " << tid << "\n";

    // 2. STACK (each thread has its own stack)
    int local_var = 42;  // Lives on this thread's stack
    cout << "Local variable address (on thread's stack): " << &local_var << "\n";

    // 3. ACCESS TO SHARED MEMORY (heap and globals)
    shared_counter++;
    cout << "Shared counter (heap/global): " << shared_counter << "\n";
    cout << "Shared counter address: " << &shared_counter << "\n";

    this_thread::sleep_for(chrono::milliseconds(100));
}

void demonstrate_thread_state(int id) {
    cout << "Thread " << id << " - RUNNING state\n";

    // Thread enters BLOCKED/WAITING state during sleep
    this_thread::sleep_for(chrono::milliseconds(200));

    cout << "Thread " << id << " - Back to RUNNING\n";
    // Thread enters TERMINATED state after function completes
}

void demonstrate_program_counter() {
    // Each thread has its own Program Counter (instruction pointer)
    cout << "Thread executing at different code locations\n";
    for (int i = 0; i < 3; i++) {
        cout << "  Iteration " << i << " in thread " << this_thread::get_id() << "\n";
        this_thread::sleep_for(chrono::milliseconds(50));
    }
}

void demonstrate_registers(int value) {
    // Each thread has its own CPU registers
    // When context switching occurs, register values are saved/restored
    int a = value;
    int b = value * 2;
    int c = a + b;
    cout << "Thread " << this_thread::get_id()
         << " register values: a=" << a << ", b=" << b << ", c=" << c << "\n";
}

// Function to show thread-local storage
thread_local int thread_local_var = 0;

void demonstrate_thread_local_storage(int id) {
    // Each thread gets its own copy of thread_local_var
    thread_local_var = id * 100;
    this_thread::sleep_for(chrono::milliseconds(50));
    cout << "Thread " << id << " - thread_local_var = " << thread_local_var << "\n";
}

int main() {
    cout << "========================================\n";
    cout << "THREAD ATTRIBUTES AND PROPERTIES\n";
    cout << "========================================\n\n";

    // 1. THREAD ID (Unique Identifier)
    cout << "=== 1. THREAD ID ===\n";
    thread t1([]() {
        cout << "Thread ID: " << this_thread::get_id() << "\n";
    });
    cout << "From main - t1 ID: " << t1.get_id() << "\n";
    t1.join();
    cout << "\n";

    // 2. THREAD STATE (Lifecycle)
    cout << "=== 2. THREAD STATE ===\n";
    cout << "States: NEW -> RUNNING -> WAITING/BLOCKED -> RUNNING -> TERMINATED\n";
    thread t2(demonstrate_thread_state, 2);
    cout << "t2 is joinable (RUNNING): " << t2.joinable() << "\n";
    t2.join();
    cout << "t2 is joinable (TERMINATED): " << t2.joinable() << "\n";
    cout << "\n";

    // 3. STACK (Private per thread)
    cout << "=== 3. STACK (Private Memory) ===\n";
    thread t3(demonstrate_thread_attributes);
    thread t4(demonstrate_thread_attributes);
    t3.join();
    t4.join();
    cout << "Note: Each thread has different stack addresses\n";
    cout << "But shared_counter address is the same!\n\n";

    // 4. PROGRAM COUNTER (Instruction Pointer)
    cout << "=== 4. PROGRAM COUNTER ===\n";
    cout << "Each thread has its own instruction pointer:\n";
    thread t5(demonstrate_program_counter);
    thread t6(demonstrate_program_counter);
    t5.join();
    t6.join();
    cout << "\n";

    // 5. CPU REGISTERS (Context)
    cout << "=== 5. CPU REGISTERS (Context) ===\n";
    cout << "Each thread has its own register set:\n";
    thread t7(demonstrate_registers, 10);
    thread t8(demonstrate_registers, 20);
    t7.join();
    t8.join();
    cout << "\n";

    // 6. PRIORITY (Scheduling)
    cout << "=== 6. THREAD PRIORITY ===\n";
    thread t9([]() {
        cout << "Thread with scheduling attributes\n";
    });

    // Get native handle to access platform-specific attributes
    pthread_t native = t9.native_handle();

    // Get scheduling policy and priority (Linux/Unix)
    int policy;
    sched_param param;
    pthread_getschedparam(native, &policy, &param);

    cout << "Scheduling policy: " << policy << "\n";
    cout << "Thread priority: " << param.sched_priority << "\n";

    // Note: Modifying priority often requires root privileges
    // param.sched_priority = 50;
    // pthread_setschedparam(native, SCHED_FIFO, &param);

    t9.join();
    cout << "\n";

    // 7. THREAD-LOCAL STORAGE
    cout << "=== 7. THREAD-LOCAL STORAGE ===\n";
    cout << "Each thread has its own copy of thread_local variables:\n";
    thread t10(demonstrate_thread_local_storage, 1);
    thread t11(demonstrate_thread_local_storage, 2);
    thread t12(demonstrate_thread_local_storage, 3);
    t10.join();
    t11.join();
    t12.join();
    cout << "\n";

    // 8. PARENT PROCESS
    cout << "=== 8. PARENT PROCESS ===\n";
    cout << "All threads belong to the same process\n";
    cout << "Process ID (getpid): " << getpid() << "\n";
    thread t13([]() {
        cout << "Thread's process ID: " << getpid() << " (same as main)\n";
    });
    t13.join();
    cout << "\n";

    // 9. EXECUTION STATE (Joinable/Detached)
    cout << "=== 9. EXECUTION STATE ===\n";
    thread t14([]() {
        this_thread::sleep_for(chrono::milliseconds(100));
        cout << "Joinable thread completed\n";
    });

    thread t15([]() {
        this_thread::sleep_for(chrono::milliseconds(100));
        cout << "Detached thread completed\n";
    });

    cout << "t14 is joinable: " << t14.joinable() << "\n";
    cout << "t15 is joinable: " << t15.joinable() << "\n";

    t14.join();
    t15.detach();

    cout << "After join/detach:\n";
    cout << "t14 is joinable: " << t14.joinable() << "\n";
    cout << "t15 is joinable: " << t15.joinable() << "\n";
    this_thread::sleep_for(chrono::milliseconds(200));
    cout << "\n";

    // 10. HARDWARE AFFINITY (which CPU core)
    cout << "=== 10. CPU AFFINITY ===\n";
    cout << "Threads can be bound to specific CPU cores\n";
    cout << "Available CPU cores: " << thread::hardware_concurrency() << "\n";

    thread t16([]() {
        cout << "Thread running (OS decides which CPU core)\n";
        // On Linux: pthread_setaffinity_np can pin thread to specific core
    });
    t16.join();
    cout << "\n";

    // 11. RESOURCE LIMITS
    cout << "=== 11. RESOURCE LIMITS ===\n";
    thread t17([]() {
        // Each thread has stack size limits (usually 2-8 MB default)
        cout << "Default stack size varies by system\n";
        cout << "Can be set using pthread_attr_setstacksize\n";
    });
    t17.join();
    cout << "\n";

    cout << "========================================\n";
    cout << "SUMMARY OF THREAD ATTRIBUTES:\n";
    cout << "========================================\n";
    cout << "1.  Thread ID (unique identifier)\n";
    cout << "2.  Thread State (NEW, RUNNING, WAITING, TERMINATED)\n";
    cout << "3.  Stack (private memory for local variables)\n";
    cout << "4.  Program Counter (instruction pointer)\n";
    cout << "5.  CPU Registers (context)\n";
    cout << "6.  Priority (scheduling priority)\n";
    cout << "7.  Thread-Local Storage (private global variables)\n";
    cout << "8.  Parent Process (all threads share same process)\n";
    cout << "9.  Execution State (joinable/detached)\n";
    cout << "10. CPU Affinity (which core it runs on)\n";
    cout << "11. Resource Limits (stack size, etc.)\n";
    cout << "12. Native Handle (platform-specific handle)\n";
    cout << "13. Signal Mask (which signals it receives - Unix)\n";
    cout << "14. errno value (thread-specific error number)\n";
    cout << "\n";

    cout << "SHARED BETWEEN THREADS:\n";
    cout << "- Code (text segment)\n";
    cout << "- Global variables\n";
    cout << "- Heap memory\n";
    cout << "- File descriptors\n";
    cout << "- Process ID\n";

    return 0;
}
