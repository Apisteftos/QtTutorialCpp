#include <iostream>

class Singleton {
private:
    // Private constructor prevents direct instantiation
    /**
     * Naive Singleton - NOT thread-safe
     * Instance is created on first use
     * PROBLEM: Not safe in multithreaded environment
     */
    Singleton() {
        std::cout << "Singleton instance created" << std::endl;
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
        std::cout << "Singleton is doing something" << std::endl;
    }

    // Destructor
    ~Singleton() {
        std::cout << "Singleton instance destroyed" << std::endl;
    }
};

// Initialize the static member
Singleton* Singleton::instance = nullptr;

int main() {
    // Get the singleton instance
    Singleton* s1 = Singleton::getInstance();
    s1->doSomething();

    // Try to get another instance - will return the same instance
    Singleton* s2 = Singleton::getInstance();
    s2->doSomething();

    // Both pointers point to the same instance
    std::cout << "Are s1 and s2 the same instance? " << (s1 == s2 ? "Yes" : "No") << std::endl;

    // Note: This basic implementation has a memory leak since we never delete the instance.
    // In a real application, you would need proper memory management.

    return 0;
}
