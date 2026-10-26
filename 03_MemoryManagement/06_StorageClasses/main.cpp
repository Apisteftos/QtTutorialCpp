#include <print>
#include <string>
#include <thread>
#include <typeinfo>
#include <vector>

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
//   5.  register     — deprecated/removed in C++17 (Readme only, no code)
//   6.  thread_local — one instance per thread (C++11)
// ============================================================


// === EXAMPLE 2: static class member (must be at file scope) ===
// Local classes cannot have static data members
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


// === EXAMPLE 2: static function variable ===
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


// === EXAMPLE 3: extern ===
// Declared here — defined at the bottom of this file
extern int g_sharedCounter;
extern const std::string g_appName;


// === EXAMPLE 4: mutable inside a class ===
class Document {
private:
    std::string m_id;
    mutable int m_accessCount = 0;   // mutable — can change in const

public:
    explicit Document(const std::string& id) : m_id(id) {}

    std::string getId() const {
        ++m_accessCount;   // allowed — mutable!
        return m_id;
    }

    int getAccessCount() const { return m_accessCount; }
};


// === EXAMPLE 5: thread_local (C++11) ===
thread_local int tl_threadId = 0;

void threadWork(int id) {
    tl_threadId = id;
    std::println("  Thread {}: tl_threadId = {}", id, tl_threadId);
}


int main()
{
    // === EXAMPLE 1: auto — type deduction (C++11) ===
    std::println("=====================================================");
    std::println("Example 1: auto — type deduction (C++11)");
    std::println("=====================================================");

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
    std::println("");
    std::println("auto in range-based for:");
    std::vector<int> nums = {1, 2, 3, 4, 5};
    for (auto n : nums) std::print("{} ", n);
    std::println("");

    // auto with const reference — avoids copy
    for (const auto& n : nums) std::print("{} ", n);
    std::println("");


    // === EXAMPLE 2: static — persists across calls ===
    std::println("");
    std::println("=====================================================");
    std::println("Example 2: static — persists across calls");
    std::println("=====================================================");

    std::println("");
    std::println("static local variable:");
    std::println("  generateId() = {}", generateId());   // 1000
    std::println("  generateId() = {}", generateId());   // 1001
    std::println("  generateId() = {}", generateId());   // 1002

    std::println("");
    std::println("non-static local variable:");
    std::println("  nonStaticCounter() = {}", nonStaticCounter());  // always 1
    std::println("  nonStaticCounter() = {}", nonStaticCounter());  // always 1
    std::println("  nonStaticCounter() = {}", nonStaticCounter());  // always 1

    std::println("");
    std::println("static class member (shared across all instances):");
    {
        Counter c1, c2, c3;
        std::println("  Total counters alive: {}", Counter::s_count);
    }   // destructors called here
    std::println("  s_count after scope: {}", Counter::s_count);


    // === EXAMPLE 3: extern — shared across translation units ===
    std::println("");
    std::println("=====================================================");
    std::println("Example 3: extern — shared across translation units");
    std::println("=====================================================");

    std::println("g_sharedCounter (extern) = {}", g_sharedCounter);
    std::println("g_appName       (extern) = {}", g_appName);

    g_sharedCounter += 10;
    std::println("g_sharedCounter after += 10 = {}", g_sharedCounter);


    // === EXAMPLE 4: mutable — modify in const context ===
    std::println("");
    std::println("=====================================================");
    std::println("Example 4: mutable — modify in const context");
    std::println("=====================================================");

    const Document doc("DOC-001");

    std::println("id: {}", doc.getId());
    std::println("id: {}", doc.getId());
    std::println("id: {}", doc.getId());
    std::println("access count: {}", doc.getAccessCount());   // 3


    // === EXAMPLE 5: thread_local — one instance per thread (C++11) ===
    // (the register keyword has no example: it no longer exists, see Readme)
    std::println("");
    std::println("=====================================================");
    std::println("Example 5: thread_local — one instance per thread (C++11)");
    std::println("=====================================================");

    tl_threadId = 99;
    std::println("Main thread: tl_threadId = {}", tl_threadId);

    std::thread t1(threadWork, 1);
    std::thread t2(threadWork, 2);
    std::thread t3(threadWork, 3);
    t1.join(); t2.join(); t3.join();

    std::println("Main thread after: tl_threadId = {}", tl_threadId);

    std::println("");
    std::println("=== Storage Classes complete ===");
    return 0;
}


// ============================================================
// extern variable DEFINITIONS
// In a real project these live in a separate .cpp file
// ============================================================
int g_sharedCounter = 0;
const std::string g_appName = "StorageDemo";
