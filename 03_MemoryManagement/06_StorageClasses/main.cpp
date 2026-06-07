#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <typeinfo>
#include <print>

// ============================================================
// STORAGE CLASSES in C++23
// ============================================================
// A storage class defines the SCOPE, LIFETIME, and VISIBILITY
// of a variable or function.
//
// Topics covered:
//   1.  auto         — type deduction (C++11 meaning)
//   2.  static       — persists across calls, file/class scope
//   3.  extern       — declared here, defined elsewhere
//   4.  mutable      — modifiable even in const objects
//   5.  register     — deprecated/removed in C++17
//   6.  thread_local — one instance per thread (C++11)
// ============================================================


// ============================================================
// Example 2 — static class member (must be at file scope)
// Local classes cannot have static data members
// ============================================================
class Counter {
public:
    static int s_count;   // shared — one copy for ALL instances
    int m_id;

    Counter() : m_id(++s_count) {
        std::println("  Counter #{} created", m_id);
    }
    ~Counter() {
        std::println("  Counter #{} destroyed", m_id);
    }
};
int Counter::s_count = 0;   // definition — must be at file scope


// ============================================================
// Example 2 — static function variable
// ============================================================
int generateId()
{
    static int s_nextId = 1000;   // initialized ONCE, persists
    return s_nextId++;
}

int nonStaticCounter()
{
    int count = 0;   // re-initialized every call
    return ++count;
}


// ============================================================
// Example 3 — extern
// Declared here — defined at the bottom of this file
// ============================================================
extern int g_sharedCounter;
extern const std::string g_appName;


// ============================================================
// Example 4 — mutable inside a class
// ============================================================
class Session {
private:
    std::string m_id;
    mutable int m_accessCount = 0;   // mutable — can change in const

public:
    explicit Session(const std::string& id) : m_id(id) {}

    std::string getId() const {
        ++m_accessCount;   // allowed — mutable!
        return m_id;
    }

    int getAccessCount() const { return m_accessCount; }
};


// ============================================================
// Example 6 — thread_local (C++11)
// ============================================================
thread_local int tl_threadId = 0;

void threadWork(int id) {
    tl_threadId = id;
    std::println("  Thread {}: tl_threadId = {}", id, tl_threadId);
}


// ============================================================
// MAIN
// ============================================================
int main()
{
    // -------------------------------------------------------
    std::cout << "=====================================================\n";
    std::cout << "Example 1: auto — type deduction (C++11)\n";
    std::cout << "=====================================================\n";

    auto a = 32;
    auto b = 3.2;
    auto c = "Konstantinos";
    auto d = 'I';
    auto e = true;
    auto f = 3.14f;

    std::println("auto a = 32            type: {}  value: {}", typeid(a).name(), a);
    std::println("auto b = 3.2           type: {}  value: {}", typeid(b).name(), b);
    std::println("auto c = Konstantinos  type: {}  value: {}", typeid(c).name(), c);
    std::println("auto d = 'I'           type: {}  value: {}", typeid(d).name(), d);
    std::println("auto e = true          type: {}  value: {}", typeid(e).name(), e);
    std::println("auto f = 3.14f         type: {}  value: {}", typeid(f).name(), f);

    // auto in range-based for — declare vector separately
    std::cout << "\nauto in range-based for:\n";
    std::vector<int> nums = {1, 2, 3, 4, 5};
    for (auto n : nums) std::cout << n << ' ';
    std::cout << '\n';

    // auto with const reference — avoids copy
    for (const auto& n : nums) std::cout << n << ' ';
    std::cout << '\n';

    std::cout << "\nNote: In C++98 'auto' meant 'automatic storage' (default).\n";
    std::cout << "      Since C++11 it means TYPE DEDUCTION.\n";
    std::cout << "      The old meaning was removed in C++11.\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: static — persists across calls\n";
    std::cout << "=====================================================\n";

    std::cout << "\nstatic local variable:\n";
    std::println("  generateId() = {}", generateId());   // 1000
    std::println("  generateId() = {}", generateId());   // 1001
    std::println("  generateId() = {}", generateId());   // 1002

    std::cout << "\nnon-static local variable:\n";
    std::println("  nonStaticCounter() = {}", nonStaticCounter());  // always 1
    std::println("  nonStaticCounter() = {}", nonStaticCounter());  // always 1
    std::println("  nonStaticCounter() = {}", nonStaticCounter());  // always 1

    std::cout << "\nstatic class member (shared across all instances):\n";
    {
        Counter c1, c2, c3;
        std::println("  Total counters alive: {}", Counter::s_count);
    }   // destructors called here
    std::println("  s_count after scope: {}", Counter::s_count);

    std::cout << "\nstatic at file scope → internal linkage\n";
    std::cout << "  Not accessible from other translation units\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: extern — shared across translation units\n";
    std::cout << "=====================================================\n";

    std::println("g_sharedCounter (extern) = {}", g_sharedCounter);
    std::println("g_appName       (extern) = {}", g_appName);

    g_sharedCounter += 10;
    std::println("g_sharedCounter after += 10 = {}", g_sharedCounter);

    std::cout << "\nextern tells the compiler:\n";
    std::cout << "  'This variable exists — just not defined HERE'\n";
    std::cout << "  The linker resolves it to the actual definition\n";
    std::cout << "  Typically: declared in .h, defined in ONE .cpp\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: mutable — modify in const context\n";
    std::cout << "=====================================================\n";

    const Session session("SES-001");

    std::println("id: {}", session.getId());
    std::println("id: {}", session.getId());
    std::println("id: {}", session.getId());
    std::println("access count: {}", session.getAccessCount());   // 3

    std::cout << "\nmutable use cases:\n";
    std::cout << "  Lazy evaluation — cache result in const getter\n";
    std::cout << "  Access counters / logging in const methods\n";
    std::cout << "  Mutex in a const thread-safe class\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: register — deprecated/removed\n";
    std::cout << "=====================================================\n";

    std::cout << "register keyword history:\n";
    std::cout << "  C++98: hint to store variable in CPU register\n";
    std::cout << "  C++11: deprecated — compiler already ignores it\n";
    std::cout << "  C++17: keyword REMOVED from the language\n\n";
    std::cout << "  // register int x = 5;   ERROR in C++17!\n\n";
    std::cout << "Modern: trust the optimizer — it allocates registers\n";
    std::cout << "  better than any manual hint could\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: thread_local — one instance per thread (C++11)\n";
    std::cout << "=====================================================\n";

    tl_threadId = 99;
    std::println("Main thread: tl_threadId = {}", tl_threadId);

    std::thread t1(threadWork, 1);
    std::thread t2(threadWork, 2);
    std::thread t3(threadWork, 3);
    t1.join(); t2.join(); t3.join();

    std::println("Main thread after: tl_threadId = {}", tl_threadId);

    std::cout << "\nthread_local use cases:\n";
    std::cout << "  Per-thread random number generators\n";
    std::cout << "  errno is thread_local in POSIX\n";
    std::cout << "  Per-thread logging context / session ID\n";
    std::cout << "  Avoid mutex overhead for thread-local data\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Summary\n";
    std::cout << "=====================================================\n";
    std::cout << "  auto         — type deduction (C++11)\n";
    std::cout << "  static       — persists between calls / internal linkage\n";
    std::cout << "  extern       — defined in another translation unit\n";
    std::cout << "  mutable      — modifiable even in const objects\n";
    std::cout << "  register     — REMOVED in C++17\n";
    std::cout << "  thread_local — per-thread copy (C++11)\n";

    std::cout << "\n=== Storage Classes complete ===\n";
    return 0;
}


// ============================================================
// extern variable DEFINITIONS
// In a real project these live in a separate .cpp file
// ============================================================
int g_sharedCounter = 0;
const std::string g_appName = "MCXApp";
