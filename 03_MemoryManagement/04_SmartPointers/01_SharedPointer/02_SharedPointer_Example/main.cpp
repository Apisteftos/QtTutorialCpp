// shared_ptr — runnable demonstrations.
// Full explanations, diagrams, mistakes and best-practice lists: see Readme.md
// This file keeps only the code that actually executes and shows real behavior.

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
using namespace std;

class Resource {
    string name;
    int id;
public:
    Resource(const string& n, int i) : name(n), id(i) {
        cout << "Created: " << name << "-" << id << "\n";
    }
    ~Resource() {
        cout << "Destroyed: " << name << "-" << id << "\n";
    }
    void use() const {
        cout << "Using: " << name << "-" << id << "\n";
    }
};

// Example: basic sharing and reference counting
void demonstrateSharedPtrBasics() {
    cout << "\n--- shared_ptr basics ---\n";
    auto ptr1 = make_shared<Resource>("Shared", 1);
    cout << "ptr1 use_count = " << ptr1.use_count() << "\n";

    {
        auto ptr2 = ptr1; // shares ownership, +1 refcount
        auto ptr3 = ptr2; // +1 again
        cout << "after ptr2, ptr3 copy: use_count = " << ptr1.use_count() << "\n";
    } // ptr2, ptr3 destroyed here — refcount drops back to 1

    cout << "after inner scope: use_count = " << ptr1.use_count() << "\n";
} // ptr1 destroyed here — Resource is deleted, refcount 0

// Example: make_shared vs new — see Readme.md for the allocation-layout diagram
void demonstrateMakeShared() {
    cout << "\n--- make_shared vs new ---\n";
    auto ptr1 = make_shared<Resource>("MakeShared", 1); // single allocation — always prefer this
}

// Example: custom deleters — for resources shared_ptr doesn't know how to destroy by default
void demonstrateCustomDeleters() {
    cout << "\n--- custom deleters ---\n";
    shared_ptr<Resource> ptr(
        new Resource("Custom", 1),
        [](Resource* p) { cout << "Custom deleter called\n"; delete p; }
        );
}

// Example: the circular reference problem — see Readme.md for the explanation of why this leaks
class Node {
public:
    string name;
    shared_ptr<Node> next;   // shared_ptr here is what creates the cycle
    Node(const string& n) : name(n) { cout << "Node [" << name << "] created\n"; }
    ~Node() { cout << "Node [" << name << "] destroyed\n"; }
};

void demonstrateCircularReference() {
    cout << "\n--- circular reference (leak) ---\n";
    {
        auto node1 = make_shared<Node>("A");
        auto node2 = make_shared<Node>("B");
        node1->next = node2;
        node2->next = node1;   // cycle
        cout << "node1.use_count = " << node1.use_count() << "\n";
        cout << "node2.use_count = " << node2.use_count() << "\n";
    }
    cout << "(scope ended — no destructor lines above means they leaked)\n";
}

// Example: weak_ptr breaks the same cycle — see Readme.md for the full mechanism
class GoodNode {
public:
    string name;
    weak_ptr<GoodNode> next;   // weak_ptr — does not keep the other node alive
    GoodNode(const string& n) : name(n) { cout << "GoodNode [" << name << "] created\n"; }
    ~GoodNode() { cout << "GoodNode [" << name << "] destroyed\n"; }
};

void demonstrateWeakPtr() {
    cout << "\n--- weak_ptr (no leak) ---\n";
    {
        auto node1 = make_shared<GoodNode>("A");
        auto node2 = make_shared<GoodNode>("B");
        node1->next = node2;
        node2->next = node1;
        cout << "node1.use_count = " << node1.use_count() << "\n";
        cout << "node2.use_count = " << node2.use_count() << "\n";

        if (auto locked = node1->next.lock()) {
            cout << "node1's next is alive: " << locked->name << "\n";
        }
    }
    cout << "(scope ended — both destructors above fired correctly)\n";

    auto shared = make_shared<Resource>("Weak", 1);
    weak_ptr<Resource> weak = shared;
    cout << "weak.expired() before reset: " << weak.expired() << "\n";
    shared.reset();
    cout << "weak.expired() after reset: " << weak.expired() << "\n";
}

// Example: observer pattern — Subject holds weak_ptr, never keeps observers alive
class Subject {
    vector<weak_ptr<Resource>> observers;
public:
    void attach(shared_ptr<Resource> observer) {
        observers.push_back(observer);
    }
    void notify() {
        observers.erase(
            remove_if(observers.begin(), observers.end(),
                      [](const weak_ptr<Resource>& wp) { return wp.expired(); }),
            observers.end());
        for (auto& wp : observers) {
            if (auto obs = wp.lock()) obs->use();
        }
    }
    size_t observerCount() const {
        return count_if(observers.begin(), observers.end(),
                        [](const weak_ptr<Resource>& wp) { return !wp.expired(); });
    }
};

void demonstrateObserverPattern() {
    cout << "\n--- observer pattern ---\n";
    Subject subject;
    {
        auto obs1 = make_shared<Resource>("Observer1", 1);
        auto obs2 = make_shared<Resource>("Observer2", 2);
        subject.attach(obs1);
        subject.attach(obs2);
        subject.notify();
        cout << "active observers: " << subject.observerCount() << "\n";
    } // obs1, obs2 destroyed — subject never kept them alive
    subject.notify();
    cout << "active observers after scope end: " << subject.observerCount() << "\n";
}

// Example: aliasing constructor — shared_ptr to a sub-object that keeps the whole parent alive
struct Data {
    int value;
    string name;
    Data(int v, const string& n) : value(v), name(n) { cout << "Data [" << name << "] created\n"; }
    ~Data() { cout << "Data [" << name << "] destroyed\n"; }
};

void demonstrateAliasing() {
    cout << "\n--- aliasing constructor ---\n";
    auto data_ptr = make_shared<Data>(42, "Parent");
    shared_ptr<int> value_ptr(data_ptr, &data_ptr->value);   // points at member, shares ownership of whole object

    cout << "data_ptr.use_count = " << data_ptr.use_count() << "\n";
    data_ptr.reset();
    cout << "after data_ptr.reset(): value_ptr.use_count = " << value_ptr.use_count() << "\n";
    cout << "*value_ptr = " << *value_ptr << " (still valid — value_ptr kept Data alive)\n";
}

// Example: enable_shared_from_this — the correct way to hand out shared_ptr<this>
class Node2 : public enable_shared_from_this<Node2> {
    string name;
public:
    Node2(const string& n) : name(n) { cout << "Node2 [" << name << "] created\n"; }
    ~Node2() { cout << "Node2 [" << name << "] destroyed\n"; }
    shared_ptr<Node2> getPtr() { return shared_from_this(); }   // reuses the existing control block
};

void demonstrateEnableSharedFromThis() {
    cout << "\n--- enable_shared_from_this ---\n";
    auto node = make_shared<Node2>("Test");
    auto node_ptr = node->getPtr();
    cout << "node.use_count = " << node.use_count() << "\n";   // 2 — same control block, not a new one
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

    // Raw-pointer pitfalls, shared_ptr vs unique_ptr, custom-deleter catalogue, thread safety,
    // common mistakes, best practices, and performance numbers: all covered in Readme.md —
    // none of those involve executable behavior worth demonstrating here.

    return 0;
}

