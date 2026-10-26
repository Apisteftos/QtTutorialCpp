#include <print>

// ============================================================
// Naive Singleton — NOT thread-safe
// ============================================================
// Instance is created on first use (lazy initialization).
// PROBLEM: not safe in a multithreaded environment (see README).
// ============================================================

class Singleton {
private:
    // Private constructor prevents direct instantiation
    Singleton() {
        std::println("Singleton instance created");
    }

    // Delete copy constructor and assignment operator
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    // The single instance
    static Singleton* instance;

public:
    // Static method to access the singleton instance
    static Singleton* getInstance() {
        if (instance == nullptr) {
            instance = new Singleton();
        }
        return instance;
    }

    void doSomething() {
        std::println("Singleton is doing something");
    }

    // Destructor
    ~Singleton() {
        std::println("Singleton instance destroyed");
    }
};

// Initialize the static member
Singleton* Singleton::instance = nullptr;

int main() {
    // === EXAMPLE 1: Naive Singleton ===

    // Get the singleton instance
    Singleton* s1 = Singleton::getInstance();
    s1->doSomething();

    // Try to get another instance - will return the same instance
    Singleton* s2 = Singleton::getInstance();
    s2->doSomething();

    // Both pointers point to the same instance
    std::println("Are s1 and s2 the same instance? {}", s1 == s2 ? "Yes" : "No");

    return 0;
}
