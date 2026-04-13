#include <iostream>
#include <mutex>


// https://refactoring.guru/design-patterns/singleton/cpp/example#example-1


/**
 * Thread-Safe Singleton using Meyers' Singleton pattern
 * This implementation is thread-safe in C++11 and later due to guaranteed thread-safe initialization of function-local static variables.
 * The instance is created on first use and destroyed automatically when the program ends.
 */
class Singleton {
private:
    // Private constructor
    Singleton() {
        std::cout << "Singleton instance created" << std::endl;
    }

    // Delete copy constructor and assignment operator
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    // For thread safety
    static std::mutex mutex;

public:
    // Static method to access the singleton instance
    static Singleton& getInstance() {
        // Using Scott Meyers' Singleton pattern
        static Singleton instance;
        return instance;
    }

    void doSomething() {
        std::cout << "Singleton is doing something" << std::endl;
    }

    ~Singleton() {
        std::cout << "Singleton instance destroyed" << std::endl;
    }
};

std::mutex Singleton::mutex;

int main() {
    // Get the singleton instance
    Singleton& s1 = Singleton::getInstance();
    s1.doSomething();

    // Get another reference to the same instance
    Singleton& s2 = Singleton::getInstance();
    s2.doSomething();

    // Both references refer to the same instance
    std::cout << "Are s1 and s2 at the same address? "
              << (&s1 == &s2 ? "Yes" : "No") << std::endl;

    return 0;
}
