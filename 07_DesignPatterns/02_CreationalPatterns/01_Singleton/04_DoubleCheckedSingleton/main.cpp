#include <mutex>
#include <atomic>
#include <thread>
#include <memory>
#include <vector>
#include <print>

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
    static std::mutex mtx;
    static std::atomic<bool> initialized;  // ✅ Use bool instead of pointer
    int value;

    DoubleCheckedSingleton() : value(0) {
        std::println("🏗️  DoubleCheckedSingleton: Constructor called");
        std::println("   Instance created with double-checked locking");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    DoubleCheckedSingleton(const DoubleCheckedSingleton&) = delete;
    DoubleCheckedSingleton& operator=(const DoubleCheckedSingleton&) = delete;

public:
    static DoubleCheckedSingleton* getInstance() {
        std::println("📞 DoubleCheckedSingleton: getInstance() called by thread {}", std::this_thread::get_id());

        // First check (without lock)
        if (!initialized.load(std::memory_order_acquire)) {
            std::lock_guard<std::mutex> lock(mtx);

            // Second check (with lock)
            if (!initialized.load(std::memory_order_relaxed)) {
                std::println("⚠️  Creating instance with double-checked locking...");
                instance = new DoubleCheckedSingleton();
                initialized.store(true, std::memory_order_release);
            }
        }
        return instance;
    }

    void setValue(int v) { value = v; }
    int getValue() const { return value; }

    void doSomething() {
        std::println("✨ DoubleCheckedSingleton: Doing something with value = {}", value);
    }
};

DoubleCheckedSingleton* DoubleCheckedSingleton::instance = nullptr;
std::mutex DoubleCheckedSingleton::mtx;
std::atomic<bool> DoubleCheckedSingleton::initialized{false};  // ✅ Define the atomic bool



void demonstrateDoubleCheckedSingleton() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║     DOUBLE-CHECKED LOCKING         ║");
    std::println("╚════════════════════════════════════╝");
    std::println("Optimized: Only locks during creation");
    std::println("");

    std::vector<std::thread> threads;
    for (int i = 0; i < 3; i++) {
        threads.emplace_back([]() {
            auto s = DoubleCheckedSingleton::getInstance();
            s->doSomething();
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    std::println("");
    std::println("✅ Thread-safe and efficient (only locks during creation)");
}


int main() {

    demonstrateDoubleCheckedSingleton();

    return 0;
}

