#include <iostream>
#include <memory>
#include <vector>

// Problem: Circular reference with shared_ptr
class BadNode {
public:
    std::string name;
    std::shared_ptr<BadNode> parent;  // Creates cycle!
    std::vector<std::shared_ptr<BadNode>> children;

    BadNode(const std::string& n) : name(n) {
        std::cout << "Node '" << name << "' created\n";
    }

    ~BadNode() {
        std::cout << "Node '" << name << "' destroyed\n";
    }
};

// Solution: Use weak_ptr to break the cycle
class GoodNode {
public:
    std::string name;
    std::weak_ptr<GoodNode> parent;  // Non-owning reference!
    std::vector<std::shared_ptr<GoodNode>> children;

    GoodNode(const std::string& n) : name(n) {
        std::cout << "Node '" << name << "' created\n";
    }

    ~GoodNode() {
        std::cout << "Node '" << name << "' destroyed\n";
    }

    void printParent() {
        // Convert weak_ptr to shared_ptr to access
        if (auto p = parent.lock()) {  // Returns shared_ptr if still alive
            std::cout << name << "'s parent is: " << p->name << "\n";
        } else {
            std::cout << name << " has no parent (expired)\n";
        }
    }
};

// Observer pattern example
class Subject {
private:
    std::vector<std::weak_ptr<class Observer>> observers;
    std::string data;

public:
    void attach(std::shared_ptr<Observer> obs) {
        observers.push_back(obs);  // Store as weak_ptr
    }

    void setData(const std::string& newData) {
        data = newData;
        notify();
    }

    void notify();  // Defined after Observer class

    std::string getData() const { return data; }
};

class Observer {
private:
    std::string name;

public:
    Observer(const std::string& n) : name(n) {
        std::cout << "Observer '" << name << "' created\n";
    }

    ~Observer() {
        std::cout << "Observer '" << name << "' destroyed\n";
    }

    void update(const std::string& data) {
        std::cout << name << " received update: " << data << "\n";
    }

    std::string getName() const { return name; }
};

void Subject::notify() {
    std::cout << "Notifying observers...\n";

    // Clean up expired observers
    for (auto it = observers.begin(); it != observers.end();) {
        if (auto obs = it->lock()) {  // Still alive?
            obs->update(data);
            ++it;
        } else {
            std::cout << "  (Removing expired observer)\n";
            it = observers.erase(it);  // Remove dead reference
        }
    }
}

int main() {
    std::cout << "=== BAD EXAMPLE (Memory Leak!) ===\n";
    {
        auto badParent = std::make_shared<BadNode>("BadParent");
        auto badChild = std::make_shared<BadNode>("BadChild");

        badParent->children.push_back(badChild);
        badChild->parent = badParent;  // Circular reference!

        std::cout << "Leaving scope...\n";
    } // MEMORY LEAK! Neither destructor called

    std::cout << "\n=== GOOD EXAMPLE (No Leak) ===\n";
    {
        auto goodParent = std::make_shared<GoodNode>("GoodParent");
        auto goodChild = std::make_shared<GoodNode>("GoodChild");

        goodParent->children.push_back(goodChild);
        goodChild->parent = goodParent;  // weak_ptr breaks cycle

        goodChild->printParent();

        std::cout << "Leaving scope...\n";
    } // Properly destroyed!

    std::cout << "\n=== OBSERVER PATTERN EXAMPLE ===\n";
    {
        Subject subject;

        auto obs1 = std::make_shared<Observer>("Observer1");
        auto obs2 = std::make_shared<Observer>("Observer2");

        subject.attach(obs1);
        subject.attach(obs2);

        subject.setData("First update");

        std::cout << "\nDestroying Observer1...\n";
        obs1.reset();  // Destroy observer

        std::cout << "\nSecond update:\n";
        subject.setData("Second update");

        std::cout << "\nLeaving scope...\n";
    }

    std::cout << "\n=== CHECKING EXPIRED weak_ptr ===\n";
    {
        std::weak_ptr<GoodNode> weakNode;

        {
            auto node = std::make_shared<GoodNode>("TemporaryNode");
            weakNode = node;

            std::cout << "expired? " << weakNode.expired() << "\n";
            std::cout << "use_count: " << weakNode.use_count() << "\n";

            if (auto locked = weakNode.lock()) {
                std::cout << "Accessed: " << locked->name << "\n";
            }

            std::cout << "Leaving inner scope...\n";
        }

        std::cout << "\nAfter object destroyed:\n";
        std::cout << "expired? " << weakNode.expired() << "\n";
        std::cout << "use_count: " << weakNode.use_count() << "\n";

        if (auto locked = weakNode.lock()) {
            std::cout << "Still accessible\n";
        } else {
            std::cout << "Object no longer exists!\n";
        }
    }

    return 0;
}
