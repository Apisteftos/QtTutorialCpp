// shared_ptr — runnable demonstrations.
// Full explanations, diagrams, mistakes and best-practice lists: see Readme.md
// This file keeps only the code that actually executes and shows real behavior.

#include <algorithm>
#include <memory>
#include <print>
#include <string>
#include <vector>

class Resource {
    std::string name;
    int id;
public:
    Resource(const std::string& n, int i) : name(n), id(i) {
        std::println("Created: {}-{}", name, id);
    }
    ~Resource() {
        std::println("Destroyed: {}-{}", name, id);
    }
    void use() const {
        std::println("Using: {}-{}", name, id);
    }
};

// === EXAMPLE 1: basic sharing and reference counting ===
void demonstrateSharedPtrBasics() {
    std::println("");
    std::println("--- shared_ptr basics ---");
    auto ptr1 = std::make_shared<Resource>("Shared", 1);
    std::println("ptr1 use_count = {}", ptr1.use_count());

    {
        auto ptr2 = ptr1; // shares ownership, +1 refcount
        auto ptr3 = ptr2; // +1 again
        std::println("after ptr2, ptr3 copy: use_count = {}", ptr1.use_count());
    } // ptr2, ptr3 destroyed here — refcount drops back to 1

    std::println("after inner scope: use_count = {}", ptr1.use_count());
} // ptr1 destroyed here — Resource is deleted, refcount 0

// === EXAMPLE 2: make_shared vs new — see Readme.md for the allocation-layout diagram ===
void demonstrateMakeShared() {
    std::println("");
    std::println("--- make_shared vs new ---");
    auto ptr1 = std::make_shared<Resource>("MakeShared", 1); // single allocation — always prefer this
}

// === EXAMPLE 3: custom deleters — for resources shared_ptr doesn't know how to destroy by default ===
void demonstrateCustomDeleters() {
    std::println("");
    std::println("--- custom deleters ---");
    std::shared_ptr<Resource> ptr(
        new Resource("Custom", 1),
        [](Resource* p) { std::println("Custom deleter called"); delete p; }
        );
}

// === EXAMPLE 4: the circular reference problem — see Readme.md for the explanation of why this leaks ===
class Node {
public:
    std::string name;
    std::shared_ptr<Node> next;   // shared_ptr here is what creates the cycle
    Node(const std::string& n) : name(n) { std::println("Node [{}] created", name); }
    ~Node() { std::println("Node [{}] destroyed", name); }
};

void demonstrateCircularReference() {
    std::println("");
    std::println("--- circular reference (leak) ---");
    {
        auto node1 = std::make_shared<Node>("A");
        auto node2 = std::make_shared<Node>("B");
        node1->next = node2;
        node2->next = node1;   // cycle
        std::println("node1.use_count = {}", node1.use_count());
        std::println("node2.use_count = {}", node2.use_count());
    }
    std::println("(scope ended — no destructor lines above means they leaked)");
}

// === EXAMPLE 5: weak_ptr breaks the same cycle — see Readme.md for the full mechanism ===
class GoodNode {
public:
    std::string name;
    std::weak_ptr<GoodNode> next;   // weak_ptr — does not keep the other node alive
    GoodNode(const std::string& n) : name(n) { std::println("GoodNode [{}] created", name); }
    ~GoodNode() { std::println("GoodNode [{}] destroyed", name); }
};

void demonstrateWeakPtr() {
    std::println("");
    std::println("--- weak_ptr (no leak) ---");
    {
        auto node1 = std::make_shared<GoodNode>("A");
        auto node2 = std::make_shared<GoodNode>("B");
        node1->next = node2;
        node2->next = node1;
        std::println("node1.use_count = {}", node1.use_count());
        std::println("node2.use_count = {}", node2.use_count());

        if (auto locked = node1->next.lock()) {
            std::println("node1's next is alive: {}", locked->name);
        }
    }
    std::println("(scope ended — both destructors above fired correctly)");

    auto shared = std::make_shared<Resource>("Weak", 1);
    std::weak_ptr<Resource> weak = shared;
    std::println("weak.expired() before reset: {}", weak.expired());
    shared.reset();
    std::println("weak.expired() after reset: {}", weak.expired());
}

// === EXAMPLE 6: observer pattern — Subject holds weak_ptr, never keeps observers alive ===
class Subject {
    std::vector<std::weak_ptr<Resource>> observers;
public:
    void attach(std::shared_ptr<Resource> observer) {
        observers.push_back(observer);
    }
    void notify() {
        observers.erase(
            std::remove_if(observers.begin(), observers.end(),
                      [](const std::weak_ptr<Resource>& wp) { return wp.expired(); }),
            observers.end());
        for (auto& wp : observers) {
            if (auto obs = wp.lock()) obs->use();
        }
    }
    size_t observerCount() const {
        return count_if(observers.begin(), observers.end(),
                        [](const std::weak_ptr<Resource>& wp) { return !wp.expired(); });
    }
};

void demonstrateObserverPattern() {
    std::println("");
    std::println("--- observer pattern ---");
    Subject subject;
    {
        auto obs1 = std::make_shared<Resource>("Observer1", 1);
        auto obs2 = std::make_shared<Resource>("Observer2", 2);
        subject.attach(obs1);
        subject.attach(obs2);
        subject.notify();
        std::println("active observers: {}", subject.observerCount());
    } // obs1, obs2 destroyed — subject never kept them alive
    subject.notify();
    std::println("active observers after scope end: {}", subject.observerCount());
}

// === EXAMPLE 7: aliasing constructor — shared_ptr to a sub-object that keeps the whole parent alive ===
struct Data {
    int value;
    std::string name;
    Data(int v, const std::string& n) : value(v), name(n) { std::println("Data [{}] created", name); }
    ~Data() { std::println("Data [{}] destroyed", name); }
};

void demonstrateAliasing() {
    std::println("");
    std::println("--- aliasing constructor ---");
    auto data_ptr = std::make_shared<Data>(42, "Parent");
    std::shared_ptr<int> value_ptr(data_ptr, &data_ptr->value);   // points at member, shares ownership of whole object

    std::println("data_ptr.use_count = {}", data_ptr.use_count());
    data_ptr.reset();
    std::println("after data_ptr.reset(): value_ptr.use_count = {}", value_ptr.use_count());
    std::println("*value_ptr = {} (still valid — value_ptr kept Data alive)", *value_ptr);
}

// === EXAMPLE 8: enable_shared_from_this — the correct way to hand out shared_ptr<this> ===
class Node2 : public std::enable_shared_from_this<Node2> {
    std::string name;
public:
    Node2(const std::string& n) : name(n) { std::println("Node2 [{}] created", name); }
    ~Node2() { std::println("Node2 [{}] destroyed", name); }
    std::shared_ptr<Node2> getPtr() { return shared_from_this(); }   // reuses the existing control block
};

void demonstrateEnableSharedFromThis() {
    std::println("");
    std::println("--- enable_shared_from_this ---");
    auto node = std::make_shared<Node2>("Test");
    auto node_ptr = node->getPtr();
    std::println("node.use_count = {}", node.use_count());   // 2 — same control block, not a new one
}

int main() {
    demonstrateSharedPtrBasics();
    demonstrateMakeShared();
    demonstrateCustomDeleters();
    demonstrateCircularReference();
    demonstrateWeakPtr();
    demonstrateObserverPattern();
    demonstrateAliasing();
    demonstrateEnableSharedFromThis();

    return 0;
}
