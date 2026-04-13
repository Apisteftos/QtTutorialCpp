#include <iostream>
#include <string>
#include <memory>

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
            std::cout << "Level 1 Support: Handling issue - " << issue << std::endl;
        } else if (nextHandler) {
            std::cout << "Level 1 Support: Escalating to next level..." << std::endl;
            nextHandler->handleRequest(issue, priority);
        } else {
            std::cout << "Level 1 Support: Cannot handle and no next handler!" << std::endl;
        }
    }
};

// Concrete Handler 2: Level 2 Support
class Level2Support : public SupportHandler {
public:
    void handleRequest(const std::string& issue, int priority) override {
        if (priority <= 2) {
            std::cout << "Level 2 Support: Handling issue - " << issue << std::endl;
        } else if (nextHandler) {
            std::cout << "Level 2 Support: Escalating to next level..." << std::endl;
            nextHandler->handleRequest(issue, priority);
        } else {
            std::cout << "Level 2 Support: Cannot handle and no next handler!" << std::endl;
        }
    }
};

// Concrete Handler 3: Level 3 Support (Manager)
class ManagerSupport : public SupportHandler {
public:
    void handleRequest(const std::string& issue, int priority) override {
        if (priority <= 3) {
            std::cout << "Manager Support: Handling critical issue - " << issue << std::endl;
        } else if (nextHandler) {
            std::cout << "Manager Support: Escalating to director..." << std::endl;
            nextHandler->handleRequest(issue, priority);
        } else {
            std::cout << "Manager Support: This is the highest level, handling anyway - "
                      << issue << std::endl;
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

    std::cout << "=== Customer Support System ===" << std::endl << std::endl;

    // Test various requests
    std::cout << "Request 1 (Priority 1):" << std::endl;
    level1->handleRequest("Password reset", 1);
    std::cout << std::endl;

    std::cout << "Request 2 (Priority 2):" << std::endl;
    level1->handleRequest("Software installation issue", 2);
    std::cout << std::endl;

    std::cout << "Request 3 (Priority 3):" << std::endl;
    level1->handleRequest("System outage", 3);
    std::cout << std::endl;

    std::cout << "Request 4 (Priority 4):" << std::endl;
    level1->handleRequest("Major security breach", 4);

    return 0;
}
