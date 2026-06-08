#include <iostream>
#include <mutex>
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
        std::cout << "Singleton instance created\n";
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

public:
    static Singleton* getInstance() {
        std::lock_guard<std::mutex> lock(mtx); // locks on EVERY call
        if (instance == nullptr) {
            instance = new Singleton();
        }
        return instance;
    }

    void doSomething() {
        std::cout << "Singleton is doing something, thread: "
                  << std::this_thread::get_id() << "\n";
    }

    ~Singleton() {
        std::cout << "Singleton instance destroyed\n";
    }
};

Singleton* Singleton::instance = nullptr;
std::mutex Singleton::mtx;

int main() {
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
    std::cout << "Same instance? " << (s1 == s2 ? "Yes" : "No") << "\n";

    return 0;
}
