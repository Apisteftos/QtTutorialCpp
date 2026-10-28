#include <map>
#include <string>
#include <memory>
#include <mutex>
#include <print>

template <typename T, typename Key = std::string>

/**
 * Multiton pattern implementation
 * This allows creation of multiple instances identified by a key
 * Each key corresponds to a single instance, and the same instance is returned for the same key
 * Thread-safe implementation using mutex for instance management
 * Example usage is provided with a Configuration class that inherits from Multiton
 * Note: In a real application, you might want to add error handling and consider the lifecycle of instances more carefully
 * This implementation uses std::shared_ptr to manage instance lifetimes and ensure proper cleanup when instances are released.
 * The Multiton class is designed to be inherited by concrete classes that want to use the multiton pattern. The concrete class must declare the Multiton class as a friend
 * to allow it to access the protected constructor. The getInstance method is used to retrieve instances based on a key, and the releaseInstance and releaseAllInstances methods are provided to manage the lifecycle of instances.
 */
class Multiton {
protected:
    // Protected constructor so only derived classes can instantiate
    Multiton() {
        std::println("Created a new instance");
    }

    // Delete copy constructor and assignment operator
    Multiton(const Multiton&) = delete;
    Multiton& operator=(const Multiton&) = delete;

    virtual ~Multiton() {
        std::println("Instance destroyed");
    }

private:
    // Storage for instances by key
    static std::map<Key, std::shared_ptr<T>> instances;
    static std::mutex instanceLock;

public:
    // Get instance by key
    static std::shared_ptr<T> getInstance(const Key& key) {
        std::lock_guard<std::mutex> lock(instanceLock);

        auto it = instances.find(key);
        if (it == instances.end()) {
            // Create a new instance for this key
            std::shared_ptr<T> instance(new T());
            instances[key] = instance;
            return instance;
        }

        return it->second;
    }

    // Remove a specific instance
    static void releaseInstance(const Key& key) {
        std::lock_guard<std::mutex> lock(instanceLock);
        instances.erase(key);
    }

    // Remove all instances
    static void releaseAllInstances() {
        std::lock_guard<std::mutex> lock(instanceLock);
        instances.clear();
    }

    // Get the number of instances
    static size_t getInstanceCount() {
        std::lock_guard<std::mutex> lock(instanceLock);
        return instances.size();
    }
};

template <typename T, typename Key>
std::map<Key, std::shared_ptr<T>> Multiton<T, Key>::instances;

template <typename T, typename Key>
std::mutex Multiton<T, Key>::instanceLock;

// Example usage with a concrete class
class Configuration : public Multiton<Configuration, std::string> {
    friend class Multiton<Configuration, std::string>;

private:
    std::string data;

    // Private constructor, only accessible by the Multiton base class
    Configuration() : data("Default config") {}

public:
    void setData(const std::string& newData) {
        data = newData;
    }

    std::string getData() const {
        return data;
    }
};

int main() {
    // Get the "database" configuration singleton
    auto dbConfig = Configuration::getInstance("database");
    dbConfig->setData("Database connection string: localhost:5432");

    // Get the "network" configuration singleton
    auto netConfig = Configuration::getInstance("network");
    netConfig->setData("Network settings: TCP/IP");

    // Getting the same instance again
    auto dbConfigAgain = Configuration::getInstance("database");
    std::println("Database config: {}", dbConfigAgain->getData());
    std::println("Network config: {}", netConfig->getData());

    // Check if both dbConfig and dbConfigAgain point to the same instance
    std::println("Are dbConfig and dbConfigAgain the same instance? {}",
        (dbConfig == dbConfigAgain ? "Yes" : "No"));

    // How many instances do we have?
    std::println("Number of instances: {}", Configuration::getInstanceCount());

    // Release one instance
    Configuration::releaseInstance("database");
    std::println("After releasing database, instances: {}", Configuration::getInstanceCount());

    // Release all remaining instances
    Configuration::releaseAllInstances();
    std::println("After releasing all, instances: {}", Configuration::getInstanceCount());

    return 0;
}
