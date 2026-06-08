#include <iostream>
#include <mutex>
#include <atomic>
#include <thread>
#include <memory>
#include <vector>

using namespace std;

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
        cout << "🏗️  MeyersSingleton: Constructor called\n";
        cout << "   Instance created with Meyers' pattern (C++11)\n";
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    MeyersSingleton(const MeyersSingleton&) = delete;
    MeyersSingleton& operator=(const MeyersSingleton&) = delete;

public:
    static MeyersSingleton& getInstance() {
        cout << "📞 MeyersSingleton: getInstance() called by thread "
             << this_thread::get_id() << "\n";

        // Magic static - thread-safe initialization guaranteed by C++11
        static MeyersSingleton instance;
        return instance;
    }

    void setValue(int v) { value = v; }
    int getValue() const { return value; }

    void doSomething() {
        cout << "✨ MeyersSingleton: Doing something with value = " << value << "\n";
    }
};


void demonstrateMeyersSingleton() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  TYPE 5: MEYERS' SINGLETON (C++11) ║\n";
    cout << "╚════════════════════════════════════╝\n";
    cout << "RECOMMENDED: Thread-safe by C++11 standard\n\n";

    vector<thread> threads;
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
    cout << "s1 address: " << &s1 << "\n";
    cout << "s2 address: " << &s2 << "\n";
    cout << "Same instance? " << (&s1 == &s2 ? "✅ Yes" : "❌ No") << "\n";

    cout << "\n✅ BEST PRACTICE: Simple, thread-safe, lazy, automatic cleanup!\n";
}


int main()
{


    demonstrateMeyersSingleton();

    return 0;
}


