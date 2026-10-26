#include <mutex>
#include <print>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

/**
 * Thread-Safe Singleton using lock_guard
 * Instance is created on first use (lazy initialization)
 * Protected by a mutex on every getInstance() call
 * Thread-safe but slower than Double-Checked Locking
 * NOTE: This is the stepping stone to understanding
 *       why Meyers' Singleton is preferred
 */
class Singleton {
private:
    static Singleton* instance;
    static std::mutex mtx;

    Singleton() {
        std::println("Singleton instance created");
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    // std::thread::id has no std::formatter on GCC 13, so go through a stream
    static std::string threadIdString() {
        std::ostringstream oss;
        oss << std::this_thread::get_id();
        return oss.str();
    }

public:
    static Singleton* getInstance() {
        std::lock_guard<std::mutex> lock(mtx); // locks on EVERY call
        if (instance == nullptr) {
            instance = new Singleton();
        }
        return instance;
    }

    void doSomething() {
        std::println("Singleton is doing something, thread: {}", threadIdString());
    }

    ~Singleton() {
        std::println("Singleton instance destroyed");
    }
};

Singleton* Singleton::instance = nullptr;
std::mutex Singleton::mtx;

int main() {
    // === EXAMPLE 1: Thread-safe Singleton with a mutex ===
    std::vector<std::thread> threads;
    for (int i = 0; i < 3; i++) {
        threads.emplace_back([]() {
            Singleton* s = Singleton::getInstance();
            s->doSomething();
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    // Verify same instance
    Singleton* s1 = Singleton::getInstance();
    Singleton* s2 = Singleton::getInstance();
    std::println("Same instance? {}", s1 == s2 ? "Yes" : "No");

    return 0;
}
