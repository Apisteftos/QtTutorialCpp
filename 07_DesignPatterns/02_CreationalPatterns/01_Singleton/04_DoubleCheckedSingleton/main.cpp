#include <iostream>
#include <mutex>
#include <atomic>
#include <thread>
#include <memory>
#include <vector>

using namespace std;
// ============================================
// DOUBLE-CHECKED LOCKING
// Optimized thread-safe singleton
// ============================================

/**
 * Double-Checked Locking Singleton
 * Checks instance twice to avoid unnecessary locking
 * Only locks when instance is null
 * More efficient than simple thread-safe version
 *
 * FIXED: Use atomic<bool> instead of atomic<DoubleCheckedSingleton*>
 */
class DoubleCheckedSingleton {
private:
    static DoubleCheckedSingleton* instance;
    static mutex mtx;
    static atomic<bool> initialized;  // ✅ Use bool instead of pointer
    int value;

    DoubleCheckedSingleton() : value(0) {
        cout << "🏗️  DoubleCheckedSingleton: Constructor called\n";
        cout << "   Instance created with double-checked locking\n";
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    DoubleCheckedSingleton(const DoubleCheckedSingleton&) = delete;
    DoubleCheckedSingleton& operator=(const DoubleCheckedSingleton&) = delete;

public:
    static DoubleCheckedSingleton* getInstance() {
        cout << "📞 DoubleCheckedSingleton: getInstance() called by thread "
             << this_thread::get_id() << "\n";

        // First check (without lock)
        if (!initialized.load(memory_order_acquire)) {
            lock_guard<mutex> lock(mtx);

            // Second check (with lock)
            if (!initialized.load(memory_order_relaxed)) {
                cout << "⚠️  Creating instance with double-checked locking...\n";
                instance = new DoubleCheckedSingleton();
                initialized.store(true, memory_order_release);
            }
        }
        return instance;
    }

    void setValue(int v) { value = v; }
    int getValue() const { return value; }

    void doSomething() {
        cout << "✨ DoubleCheckedSingleton: Doing something with value = " << value << "\n";
    }
};

DoubleCheckedSingleton* DoubleCheckedSingleton::instance = nullptr;
mutex DoubleCheckedSingleton::mtx;
atomic<bool> DoubleCheckedSingleton::initialized{false};  // ✅ Define the atomic bool



void demonstrateDoubleCheckedSingleton() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║     DOUBLE-CHECKED LOCKING         ║\n";
    cout << "╚════════════════════════════════════╝\n";
    cout << "Optimized: Only locks during creation\n\n";

    vector<thread> threads;
    for (int i = 0; i < 3; i++) {
        threads.emplace_back([]() {
            auto s = DoubleCheckedSingleton::getInstance();
            s->doSomething();
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    cout << "\n✅ Thread-safe and efficient (only locks during creation)\n";
}


int main() {

    demonstrateDoubleCheckedSingleton();

    return 0;
}

