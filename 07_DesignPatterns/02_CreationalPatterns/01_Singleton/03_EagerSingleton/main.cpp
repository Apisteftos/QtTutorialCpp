#include <iostream>
#include <string>
#include <mutex>
#include <thread>
#include <chrono>
#include <vector>
#include <atomic>
#include <memory>
using namespace std;

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
        cout << "🏗️  EagerSingleton: Constructor called\n";
        cout << "   Instance created at program startup\n";
    }

    // Delete copy constructor and assignment operator
    EagerSingleton(const EagerSingleton&) = delete;
    EagerSingleton& operator=(const EagerSingleton&) = delete;

public:
    static EagerSingleton* getInstance() {
        cout << "📞 EagerSingleton: getInstance() called\n";
        return instance;
    }

    void setValue(int v) { value = v; }
    int getValue() const { return value; }

    void doSomething() {
        cout << "✨ EagerSingleton: Doing something with value = " << value << "\n";
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
        cout << "🏗️  LazySingleton: Constructor called\n";
        cout << "   Instance created on first use (lazy)\n";
        this_thread::sleep_for(chrono::milliseconds(100)); // Simulate work
    }

    LazySingleton(const LazySingleton&) = delete;
    LazySingleton& operator=(const LazySingleton&) = delete;

public:
    static LazySingleton* getInstance() {
        cout << "📞 LazySingleton: getInstance() called by thread "
             << this_thread::get_id() << "\n";

        if (instance == nullptr) {
            cout << "⚠️  Creating instance...\n";
            instance = new LazySingleton();  // NOT THREAD-SAFE!
        }
        return instance;
    }

    void setValue(int v) { value = v; }
    int getValue() const { return value; }

    void doSomething() {
        cout << "✨ LazySingleton: Doing something with value = " << value << "\n";
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
    static mutex mtx;
    int value;

    ThreadSafeSingleton() : value(0) {
        cout << "🏗️  ThreadSafeSingleton: Constructor called\n";
        cout << "   Instance created safely with mutex\n";
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    ThreadSafeSingleton(const ThreadSafeSingleton&) = delete;
    ThreadSafeSingleton& operator=(const ThreadSafeSingleton&) = delete;

public:
    static ThreadSafeSingleton* getInstance() {
        lock_guard<mutex> lock(mtx);  // Lock on EVERY call (slow!)

        cout << "📞 ThreadSafeSingleton: getInstance() called by thread "
             << this_thread::get_id() << "\n";

        if (instance == nullptr) {
            cout << "⚠️  Creating instance with mutex protection...\n";
            instance = new ThreadSafeSingleton();
        }
        return instance;
    }

    void setValue(int v) { value = v; }
    int getValue() const { return value; }

    void doSomething() {
        cout << "✨ ThreadSafeSingleton: Doing something with value = " << value << "\n";
    }
};

ThreadSafeSingleton* ThreadSafeSingleton::instance = nullptr;
mutex ThreadSafeSingleton::mtx;


// ============================================
// DEMONSTRATION FUNCTIONS
// ============================================

void demonstrateEagerSingleton() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  TYPE 1: EAGER SINGLETON           ║\n";
    cout << "╚════════════════════════════════════╝\n";
    cout << "Instance created BEFORE main() runs!\n\n";

    auto s1 = EagerSingleton::getInstance();
    s1->setValue(42);
    s1->doSomething();

    auto s2 = EagerSingleton::getInstance();
    cout << "s1 value: " << s1->getValue() << "\n";
    cout << "s2 value: " << s2->getValue() << "\n";
    cout << "s1 address: " << s1 << "\n";
    cout << "s2 address: " << s2 << "\n";
    cout << "Same instance? " << (s1 == s2 ? "✅ Yes" : "❌ No") << "\n";
}

void demonstrateLazySingleton() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  TYPE 2: LAZY SINGLETON            ║\n";
    cout << "╚════════════════════════════════════╝\n";
    cout << "Instance created on FIRST use\n\n";

    cout << "Before first call - instance doesn't exist yet\n";
    auto s1 = LazySingleton::getInstance();
    s1->setValue(99);
    s1->doSomething();

    auto s2 = LazySingleton::getInstance();
    cout << "Same instance? " << (s1 == s2 ? "✅ Yes" : "❌ No") << "\n";

    cout << "\n⚠️  WARNING: Not thread-safe! Multiple threads might create multiple instances!\n";
}

void demonstrateThreadSafeSingleton() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  TYPE 3: THREAD-SAFE SINGLETON     ║\n";
    cout << "╚════════════════════════════════════╝\n";
    cout << "Protected with mutex on EVERY call\n\n";

    vector<thread> threads;
    for (int i = 0; i < 3; i++) {
        threads.emplace_back([]() {
            auto s = ThreadSafeSingleton::getInstance();
            s->doSomething();
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    cout << "\n✅ Thread-safe, but locks on every getInstance() call (slower)\n";
}


// ============================================
// MAIN
// ============================================

int main() {
    cout << "========================================\n";
    cout << "SINGLETON PATTERN - ALL TYPES\n";
    cout << "========================================\n";

    demonstrateEagerSingleton();
    demonstrateLazySingleton();
    demonstrateThreadSafeSingleton();


    cout << "\n========================================\n";
    cout << "⭐ RECOMMENDED: Use Meyers' Singleton!\n";
    cout << "========================================\n";

    return 0;
}
