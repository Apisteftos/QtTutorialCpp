#include <string>
#include <memory>
#include <print>

// Abstract Handler
class SupportHandler {
protected:
    std::shared_ptr<SupportHandler> nextHandler;

public:
    virtual ~SupportHandler() {}

    void setNext(std::shared_ptr<SupportHandler> handler) {
        nextHandler = handler;
    }

    virtual void handleRequest(const std::string& issue, int priority) = 0;
};

// Concrete Handler 1: Level 1 Support
class Level1Support : public SupportHandler {
public:
    void handleRequest(const std::string& issue, int priority) override {
        if (priority <= 1) {
            std::println("Level 1 Support: Handling issue - {}", issue);
        } else if (nextHandler) {
            std::println("Level 1 Support: Escalating to next level...");
            nextHandler->handleRequest(issue, priority);
        } else {
            std::println("Level 1 Support: Cannot handle and no next handler!");
        }
    }
};

// Concrete Handler 2: Level 2 Support
class Level2Support : public SupportHandler {
public:
    void handleRequest(const std::string& issue, int priority) override {
        if (priority <= 2) {
            std::println("Level 2 Support: Handling issue - {}", issue);
        } else if (nextHandler) {
            std::println("Level 2 Support: Escalating to next level...");
            nextHandler->handleRequest(issue, priority);
        } else {
            std::println("Level 2 Support: Cannot handle and no next handler!");
        }
    }
};

// Concrete Handler 3: Level 3 Support (Manager)
class ManagerSupport : public SupportHandler {
public:
    void handleRequest(const std::string& issue, int priority) override {
        if (priority <= 3) {
            std::println("Manager Support: Handling critical issue - {}", issue);
        } else if (nextHandler) {
            std::println("Manager Support: Escalating to director...");
            nextHandler->handleRequest(issue, priority);
        } else {
            std::println("Manager Support: This is the highest level, handling anyway - {}", issue);
        }
    }
};

int main() {
    // Create handlers
    auto level1 = std::make_shared<Level1Support>();
    auto level2 = std::make_shared<Level2Support>();
    auto manager = std::make_shared<ManagerSupport>();

    // Set up the chain
    level1->setNext(level2);
    level2->setNext(manager);

    std::println("=== Customer Support System ===");
    std::println("");

    // Test various requests
    std::println("Request 1 (Priority 1):");
    level1->handleRequest("Password reset", 1);
    std::println("");

    std::println("Request 2 (Priority 2):");
    level1->handleRequest("Software installation issue", 2);
    std::println("");

    std::println("Request 3 (Priority 3):");
    level1->handleRequest("System outage", 3);
    std::println("");

    std::println("Request 4 (Priority 4):");
    level1->handleRequest("Major security breach", 4);

    return 0;
}
