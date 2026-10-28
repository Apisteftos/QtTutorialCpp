#include <string>
#include <mutex>
#include <thread>
#include <chrono>
#include <vector>
#include <atomic>
#include <memory>
#include <print>

// ============================================
// TYPE 1: EAGER INITIALIZATION (CLASSIC)
// Instance created at program start
// ============================================

/**
 * Classic Singleton - Eager Initialization
 * Instance is created when class is loaded
 * Thread-safe (initialized before main)
 * Memory is always allocated even if never used
 */
class EagerSingleton {
private:
    static EagerSingleton* instance;
    int value;

    // Private constructor prevents external instantiation
    EagerSingleton() : value(0) {
        std::println("🏗️  EagerSingleton: Constructor called");
        std::println("   Instance created at program startup");
    }

    // Delete copy constructor and assignment operator
    EagerSingleton(const EagerSingleton&) = delete;
    EagerSingleton& operator=(const EagerSingleton&) = delete;

public:
    static EagerSingleton* getInstance() {
        std::println("📞 EagerSingleton: getInstance() called");
        return instance;
    }

    void setValue(int v) { value = v; }
    int getValue() const { return value; }

    void doSomething() {
        std::println("✨ EagerSingleton: Doing something with value = {}", value);
    }
};

// Static instance created at program start (before main())
EagerSingleton* EagerSingleton::instance = new EagerSingleton();

// ============================================
// TYPE 2: LAZY INITIALIZATION (NOT THREAD-SAFE)
// Instance created on first use
// ============================================

/**
 * Lazy Singleton - NOT thread-safe
 * Instance created only when first needed
 * Saves memory if singleton is never used
 * PROBLEM: Not safe in multithreaded environment
 */
class LazySingleton {
private:
    static LazySingleton* instance;
    int value;

    LazySingleton() : value(0) {
        std::println("🏗️  LazySingleton: Constructor called");
        std::println("   Instance created on first use (lazy)");
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate work
    }

    LazySingleton(const LazySingleton&) = delete;
    LazySingleton& operator=(const LazySingleton&) = delete;

public:
    static LazySingleton* getInstance() {
        std::println("📞 LazySingleton: getInstance() called by thread {}", std::this_thread::get_id());

        if (instance == nullptr) {
            std::println("⚠️  Creating instance...");
            instance = new LazySingleton();  // NOT THREAD-SAFE!
        }
        return instance;
    }

    void setValue(int v) { value = v; }
    int getValue() const { return value; }

    void doSomething() {
        std::println("✨ LazySingleton: Doing something with value = {}", value);
    }
};

LazySingleton* LazySingleton::instance = nullptr;

// ============================================
// TYPE 3: THREAD-SAFE SINGLETON (WITH MUTEX)
// Lazy initialization with mutex protection
// ============================================

/**
 * Thread-Safe Singleton using mutex
 * Instance created on first use
 * Safe in multithreaded environment
 * PROBLEM: Mutex lock on every getInstance() call (slower)
 */
class ThreadSafeSingleton {
private:
    static ThreadSafeSingleton* instance;
    static std::mutex mtx;
    int value;

    ThreadSafeSingleton() : value(0) {
        std::println("🏗️  ThreadSafeSingleton: Constructor called");
        std::println("   Instance created safely with mutex");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    ThreadSafeSingleton(const ThreadSafeSingleton&) = delete;
    ThreadSafeSingleton& operator=(const ThreadSafeSingleton&) = delete;

public:
    static ThreadSafeSingleton* getInstance() {
        std::lock_guard<std::mutex> lock(mtx);  // Lock on EVERY call (slow!)

        std::println("📞 ThreadSafeSingleton: getInstance() called by thread {}", std::this_thread::get_id());

        if (instance == nullptr) {
            std::println("⚠️  Creating instance with mutex protection...");
            instance = new ThreadSafeSingleton();
        }
        return instance;
    }

    void setValue(int v) { value = v; }
    int getValue() const { return value; }

    void doSomething() {
        std::println("✨ ThreadSafeSingleton: Doing something with value = {}", value);
    }
};

ThreadSafeSingleton* ThreadSafeSingleton::instance = nullptr;
std::mutex ThreadSafeSingleton::mtx;


// ============================================
// DEMONSTRATION FUNCTIONS
// ============================================

void demonstrateEagerSingleton() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  TYPE 1: EAGER SINGLETON           ║");
    std::println("╚════════════════════════════════════╝");
    std::println("Instance created BEFORE main() runs!");
    std::println("");

    auto s1 = EagerSingleton::getInstance();
    s1->setValue(42);
    s1->doSomething();

    auto s2 = EagerSingleton::getInstance();
    std::println("s1 value: {}", s1->getValue());
    std::println("s2 value: {}", s2->getValue());
    std::println("s1 address: {}", static_cast<const void*>(s1));
    std::println("s2 address: {}", static_cast<const void*>(s2));
    std::println("Same instance? {}", (s1 == s2 ? "✅ Yes" : "❌ No"));
}

void demonstrateLazySingleton() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  TYPE 2: LAZY SINGLETON            ║");
    std::println("╚════════════════════════════════════╝");
    std::println("Instance created on FIRST use");
    std::println("");

    std::println("Before first call - instance doesn't exist yet");
    auto s1 = LazySingleton::getInstance();
    s1->setValue(99);
    s1->doSomething();

    auto s2 = LazySingleton::getInstance();
    std::println("Same instance? {}", (s1 == s2 ? "✅ Yes" : "❌ No"));

    std::println("");
    std::println("⚠️  WARNING: Not thread-safe! Multiple threads might create multiple instances!");
}

void demonstrateThreadSafeSingleton() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  TYPE 3: THREAD-SAFE SINGLETON     ║");
    std::println("╚════════════════════════════════════╝");
    std::println("Protected with mutex on EVERY call");
    std::println("");

    std::vector<std::thread> threads;
    for (int i = 0; i < 3; i++) {
        threads.emplace_back([]() {
            auto s = ThreadSafeSingleton::getInstance();
            s->doSomething();
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    std::println("");
    std::println("✅ Thread-safe, but locks on every getInstance() call (slower)");
}


// ============================================
// MAIN
// ============================================

int main() {
    std::println("========================================");
    std::println("SINGLETON PATTERN - ALL TYPES");
    std::println("========================================");

    demonstrateEagerSingleton();
    demonstrateLazySingleton();
    demonstrateThreadSafeSingleton();

    return 0;
}
