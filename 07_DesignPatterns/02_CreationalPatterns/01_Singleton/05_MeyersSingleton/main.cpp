#include <mutex>
#include <atomic>
#include <thread>
#include <memory>
#include <vector>
#include <print>


// ============================================
// MEYERS' SINGLETON (C++11)
// Best practice - uses static local variable
// ============================================

/**
 * Meyers' Singleton (C++11)
 * Uses static local variable
 * Thread-safe by C++11 standard
 * Lazy initialization
 * Automatic cleanup
 * RECOMMENDED APPROACH!
 */

class MeyersSingleton {
private:
    int value;

    MeyersSingleton() : value(0) {
        std::println("🏗️  MeyersSingleton: Constructor called");
        std::println("   Instance created with Meyers' pattern (C++11)");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    MeyersSingleton(const MeyersSingleton&) = delete;
    MeyersSingleton& operator=(const MeyersSingleton&) = delete;

public:
    static MeyersSingleton& getInstance() {
        std::println("📞 MeyersSingleton: getInstance() called by thread {}", std::this_thread::get_id());

        // Magic static - thread-safe initialization guaranteed by C++11
        static MeyersSingleton instance;
        return instance;
    }

    void setValue(int v) { value = v; }
    int getValue() const { return value; }

    void doSomething() {
        std::println("✨ MeyersSingleton: Doing something with value = {}", value);
    }
};


void demonstrateMeyersSingleton() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  TYPE 5: MEYERS' SINGLETON (C++11) ║");
    std::println("╚════════════════════════════════════╝");
    std::println("RECOMMENDED: Thread-safe by C++11 standard");
    std::println("");

    std::vector<std::thread> threads;
    for (int i = 0; i < 3; i++) {
        threads.emplace_back([]() {
            auto& s = MeyersSingleton::getInstance();
            s.doSomething();
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    auto& s1 = MeyersSingleton::getInstance();
    auto& s2 = MeyersSingleton::getInstance();
    std::println("s1 address: {}", static_cast<const void*>(&s1));
    std::println("s2 address: {}", static_cast<const void*>(&s2));
    std::println("Same instance? {}", (&s1 == &s2 ? "✅ Yes" : "❌ No"));

    std::println("");
    std::println("✅ BEST PRACTICE: Simple, thread-safe, lazy, automatic cleanup!");
}


int main()
{


    demonstrateMeyersSingleton();

    return 0;
}


