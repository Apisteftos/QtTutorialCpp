#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <algorithm>
#include <mutex>
using namespace std;

// ============================================
// WHAT IS SHARED_PTR?
// ============================================

/*
THE PROBLEM WITH RAW POINTERS:
    1. Memory leaks (forget to delete)
    2. Double deletion (delete twice)
    3. Dangling pointers (use after delete)
    4. Unclear ownership (who deletes?)
    5. Exception unsafe

THE SOLUTION - SHARED_PTR:
    Smart pointer with SHARED OWNERSHIP
    - Multiple owners can share the same object
    - Reference counted (tracks how many owners)
    - Automatically deletes when last owner dies
    - Exception safe
    - Thread-safe reference counting

KEY CONCEPTS:
    1. Reference Counting: Tracks number of shared_ptr instances
    2. Control Block: Stores ref count, deleter, allocator
    3. Automatic Cleanup: delete called when count reaches 0
    4. Copy = Share: Copying increases ref count
    5. Weak_ptr: Non-owning reference (breaks cycles)

WHEN TO USE:
    ✅ Shared ownership (multiple owners)
    ✅ Object needs to outlive creator
    ✅ Storing in containers (vector<shared_ptr<T>>)
    ✅ Callback/observer patterns
    ❌ Single ownership → use unique_ptr instead
    ❌ Performance critical → consider unique_ptr
*/

// ============================================
// EXAMPLE 1: RAW POINTER PROBLEMS
// ============================================

class Resource {
private:
    string name;
    int id;

public:
    Resource(const string& n, int i) : name(n), id(i) {
        cout << "✅ [" << name << "-" << id << "] Created\n";
    }

    ~Resource() {
        cout << "💥 [" << name << "-" << id << "] Destroyed\n";
    }

    void use() const {
        cout << "   Using [" << name << "-" << id << "]\n";
    }
};

void demonstrateRawPointerProblems() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: RAW POINTER PROBLEMS   ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "Problem 1: MEMORY LEAK\n";
    cout << "---\n";
    cout << "Resource* ptr = new Resource(\"Leak\", 1);\n";
    cout << "// Forget to delete → Memory leak! 💥\n";
    cout << "\n";

    cout << "Problem 2: DOUBLE DELETE\n";
    cout << "---\n";
    cout << "Resource* ptr1 = new Resource(\"Double\", 2);\n";
    cout << "Resource* ptr2 = ptr1;  // Both point to same object\n";
    cout << "delete ptr1;  // OK\n";
    cout << "delete ptr2;  // 💥 CRASH! Already deleted!\n";
    cout << "\n";

    cout << "Problem 3: DANGLING POINTER\n";
    cout << "---\n";
    cout << "Resource* ptr1 = new Resource(\"Dangling\", 3);\n";
    cout << "Resource* ptr2 = ptr1;\n";
    cout << "delete ptr1;  // Destroys object\n";
    cout << "ptr2->use();  // 💥 CRASH! Points to deleted memory\n";
    cout << "\n";

    cout << "Problem 4: EXCEPTION SAFETY\n";
    cout << "---\n";
    cout << "Resource* ptr = new Resource(\"Exception\", 4);\n";
    cout << "doSomething();  // If exception thrown here\n";
    cout << "delete ptr;     // This never executes → Leak! 💥\n";
    cout << "\n";

    cout << "Problem 5: UNCLEAR OWNERSHIP\n";
    cout << "---\n";
    cout << "void func(Resource* ptr) {\n";
    cout << "    // Who owns this? Should I delete it?\n";
    cout << "    // Should the caller delete it?\n";
    cout << "    // Very confusing! 😕\n";
    cout << "}\n";
}

// ============================================
// EXAMPLE 2: SHARED_PTR BASICS
// ============================================

void demonstrateSharedPtrBasics() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: SHARED_PTR BASICS      ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "Creating shared_ptr...\n";
    shared_ptr<Resource> ptr1 = make_shared<Resource>("Shared", 1);
    cout << "   ptr1 use_count = " << ptr1.use_count() << "\n";
    ptr1->use();

    cout << "\n--- Sharing ownership (copying ptr) ---\n";
    {
        shared_ptr<Resource> ptr2 = ptr1;  // Share ownership
        cout << "   ptr1 use_count = " << ptr1.use_count() << "\n";
        cout << "   ptr2 use_count = " << ptr2.use_count() << "\n";
        ptr2->use();

        cout << "\n--- Creating third owner ---\n";
        shared_ptr<Resource> ptr3 = ptr2;
        cout << "   ptr1 use_count = " << ptr1.use_count() << "\n";
        cout << "   ptr2 use_count = " << ptr2.use_count() << "\n";
        cout << "   ptr3 use_count = " << ptr3.use_count() << "\n";

        cout << "\n--- ptr3 going out of scope ---\n";
    }
    cout << "   After ptr3 destroyed:\n";
    cout << "   ptr1 use_count = " << ptr1.use_count() << "\n";

    cout << "\n--- ptr2 already destroyed ---\n";
    cout << "   ptr1 use_count = " << ptr1.use_count() << "\n";

    cout << "\n--- ptr1 going out of scope ---\n";
    cout << "   This will trigger Resource destruction!\n";
}

// ============================================
// EXAMPLE 3: REFERENCE COUNTING EXPLAINED
// ============================================

void demonstrateReferenceCounting() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: REFERENCE COUNTING     ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "How shared_ptr works internally:\n";
    cout << "\n";
    cout << "┌─────────────────┐\n";
    cout << "│   CONTROL BLOCK │\n";
    cout << "├─────────────────┤\n";
    cout << "│ Ref Count: 3    │  ← Number of shared_ptr owners\n";
    cout << "│ Weak Count: 1   │  ← Number of weak_ptr observers\n";
    cout << "│ Deleter: func   │  ← Custom delete function\n";
    cout << "│ Allocator: ...  │  ← Memory allocator\n";
    cout << "├─────────────────┤\n";
    cout << "│ Object: [Data]  │  ← Your actual object\n";
    cout << "└─────────────────┘\n";
    cout << "\n";

    cout << "Operations and ref count:\n";
    auto ptr1 = make_shared<Resource>("Count", 1);
    cout << "   make_shared()        → count = " << ptr1.use_count() << "\n";

    auto ptr2 = ptr1;
    cout << "   auto ptr2 = ptr1     → count = " << ptr1.use_count() << "\n";

    auto ptr3 = ptr1;
    cout << "   auto ptr3 = ptr1     → count = " << ptr1.use_count() << "\n";

    ptr2.reset();
    cout << "   ptr2.reset()         → count = " << ptr1.use_count() << "\n";

    ptr3 = nullptr;
    cout << "   ptr3 = nullptr       → count = " << ptr1.use_count() << "\n";

    cout << "\n   When count reaches 0 → Object is deleted!\n";
}

// ============================================
// EXAMPLE 4: SHARED_PTR VS UNIQUE_PTR
// ============================================

void demonstrateSharedVsUnique() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: SHARED VS UNIQUE       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "UNIQUE_PTR (Exclusive ownership):\n";
    cout << "---\n";
    cout << "unique_ptr<Resource> ptr1 = make_unique<Resource>(...);\n";
    cout << "// unique_ptr<Resource> ptr2 = ptr1;  // ❌ ERROR! Can't copy\n";
    cout << "unique_ptr<Resource> ptr2 = move(ptr1);  // ✅ Transfer ownership\n";
    cout << "// ptr1 is now nullptr\n";
    cout << "\n";
    cout << "Characteristics:\n";
    cout << "  ✅ Zero overhead (size = 1 pointer)\n";
    cout << "  ✅ Fastest smart pointer\n";
    cout << "  ✅ Move-only (exclusive ownership)\n";
    cout << "  ✅ Can convert to shared_ptr\n";
    cout << "  ❌ Can't share ownership\n";
    cout << "\n";

    cout << "SHARED_PTR (Shared ownership):\n";
    cout << "---\n";
    cout << "shared_ptr<Resource> ptr1 = make_shared<Resource>(...);\n";
    cout << "shared_ptr<Resource> ptr2 = ptr1;  // ✅ Both own the object\n";
    cout << "shared_ptr<Resource> ptr3 = ptr1;  // ✅ Three owners now\n";
    cout << "// Object destroyed when ALL go out of scope\n";
    cout << "\n";
    cout << "Characteristics:\n";
    cout << "  ✅ Multiple owners allowed\n";
    cout << "  ✅ Automatic lifetime management\n";
    cout << "  ✅ Thread-safe ref counting\n";
    cout << "  ❌ Overhead (control block + ref count)\n";
    cout << "  ❌ Slightly slower than unique_ptr\n";
    cout << "  ❌ Can't convert to unique_ptr\n";
    cout << "\n";

    cout << "DECISION TREE:\n";
    cout << "---\n";
    cout << "Need shared ownership?\n";
    cout << "  YES → Use shared_ptr\n";
    cout << "  NO  → Use unique_ptr (faster!)\n";
    cout << "\n";
    cout << "Object outlives creating function?\n";
    cout << "  YES (and multiple owners) → shared_ptr\n";
    cout << "  YES (single owner) → unique_ptr\n";
    cout << "  NO → unique_ptr or stack allocation\n";
}

// ============================================
// EXAMPLE 5: MAKE_SHARED VS NEW
// ============================================

void demonstrateMakeShared() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: MAKE_SHARED VS NEW     ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "BAD: Using new (2 allocations)\n";
    cout << "---\n";
    cout << "shared_ptr<Resource> ptr(new Resource(...));\n";
    cout << "\n";
    cout << "Memory layout:\n";
    cout << "┌─────────────────┐  Allocation 1 (operator new)\n";
    cout << "│ Resource object │\n";
    cout << "└─────────────────┘\n";
    cout << "┌─────────────────┐  Allocation 2 (shared_ptr constructor)\n";
    cout << "│ Control Block   │\n";
    cout << "└─────────────────┘\n";
    cout << "\n";
    cout << "Problems:\n";
    cout << "  ❌ Two allocations (slower)\n";
    cout << "  ❌ Cache unfriendly (non-contiguous)\n";
    cout << "  ❌ Exception unsafe:\n";
    cout << "     func(shared_ptr<T>(new T), shared_ptr<U>(new U));\n";
    cout << "     If second new throws, first leaks!\n";
    cout << "\n";

    cout << "GOOD: Using make_shared (1 allocation)\n";
    cout << "---\n";
    cout << "shared_ptr<Resource> ptr = make_shared<Resource>(...);\n";
    cout << "\n";
    cout << "Memory layout:\n";
    cout << "┌─────────────────┐  Single allocation\n";
    cout << "│ Control Block   │  ← Ref counts, deleter\n";
    cout << "├─────────────────┤\n";
    cout << "│ Resource object │  ← Your object\n";
    cout << "└─────────────────┘\n";
    cout << "\n";
    cout << "Benefits:\n";
    cout << "  ✅ Single allocation (faster!)\n";
    cout << "  ✅ Cache friendly (contiguous memory)\n";
    cout << "  ✅ Exception safe\n";
    cout << "  ✅ Less memory overhead\n";
    cout << "  ✅ Better performance\n";
    cout << "\n";

    cout << "--- Creating with make_shared ---\n";
    auto ptr1 = make_shared<Resource>("MakeShared", 1);
    cout << "\n";

    cout << "WHEN TO USE NEW:\n";
    cout << "  1. Need custom deleter\n";
    cout << "  2. Adopting raw pointer from C API\n";
    cout << "  3. Creating from existing object\n";
    cout << "  Otherwise: ALWAYS use make_shared!\n";
}

// ============================================
// EXAMPLE 6: CUSTOM DELETERS
// ============================================

struct FileCloser {
    void operator()(FILE* fp) const {
        if (fp) {
            cout << "   🔒 Custom deleter: closing file\n";
            fclose(fp);
        }
    }
};

void demonstrateCustomDeleters() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: CUSTOM DELETERS        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "Use case: Managing C resources (FILE*, handles, etc.)\n";
    cout << "\n";

    cout << "--- Lambda deleter ---\n";
    {
        shared_ptr<Resource> ptr(
            new Resource("Custom", 1),
            [](Resource* p) {
                cout << "   🔧 Lambda deleter called\n";
                delete p;
            }
            );
    }

    cout << "\n--- Function pointer deleter ---\n";
    {
        auto deleter = [](Resource* p) {
            cout << "   🔧 Function deleter called\n";
            delete p;
        };
        shared_ptr<Resource> ptr(new Resource("Function", 2), deleter);
    }

    cout << "\n--- Array deleter ---\n";
    {
        shared_ptr<int> arr(
            new int[5],
            [](int* p) {
                cout << "   🔧 Array deleter: delete[]\n";
                delete[] p;
            }
            );
    }

    cout << "\n";
    cout << "Common custom deleters:\n";
    cout << "  1. FILE*:        fclose(fp)\n";
    cout << "  2. Arrays:       delete[] ptr\n";
    cout << "  3. C resources:  free(ptr)\n";
    cout << "  4. Handles:      CloseHandle(h)\n";
    cout << "  5. No-op:        [](T*){} (don't delete)\n";
    cout << "\n";
    cout << "Note: make_shared can't use custom deleters!\n";
    cout << "Use shared_ptr<T>(new T, deleter) instead.\n";
}

// ============================================
// EXAMPLE 7: CIRCULAR REFERENCE PROBLEM
// ============================================

class Node {
public:
    string name;
    shared_ptr<Node> next;  // 🚨 Problem: shared_ptr creates cycle!

    Node(const string& n) : name(n) {
        cout << "   ✅ Node [" << name << "] created\n";
    }

    ~Node() {
        cout << "   💥 Node [" << name << "] destroyed\n";
    }
};

void demonstrateCircularReference() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 7: CIRCULAR REFERENCE     ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "Creating circular reference...\n";
    {
        auto node1 = make_shared<Node>("A");
        auto node2 = make_shared<Node>("B");

        node1->next = node2;  // A → B
        node2->next = node1;  // B → A  (CYCLE!)

        cout << "\n   Cycle created:\n";
        cout << "   node1.use_count = " << node1.use_count() << " (A points to B)\n";
        cout << "   node2.use_count = " << node2.use_count() << " (B points to A)\n";

        cout << "\n--- Going out of scope ---\n";
        cout << "   Expected: Both nodes destroyed\n";
        cout << "   Reality:  MEMORY LEAK! 💥\n";
        cout << "   Why: A keeps B alive, B keeps A alive\n";
    }
    cout << "   ⚠️  Nodes were NOT destroyed! Memory leak!\n";
}

// ============================================
// EXAMPLE 8: WEAK_PTR SOLUTION
// ============================================

class GoodNode {
public:
    string name;
    weak_ptr<GoodNode> next;  // ✅ Use weak_ptr to break cycle!

    GoodNode(const string& n) : name(n) {
        cout << "   ✅ GoodNode [" << name << "] created\n";
    }

    ~GoodNode() {
        cout << "   💥 GoodNode [" << name << "] destroyed\n";
    }
};

void demonstrateWeakPtr() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 8: WEAK_PTR SOLUTION      ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "WEAK_PTR: Non-owning reference\n";
    cout << "  - Doesn't increase ref count\n";
    cout << "  - Doesn't prevent deletion\n";
    cout << "  - Can check if object still exists\n";
    cout << "  - Can convert to shared_ptr (lock())\n";
    cout << "\n";

    cout << "--- Creating nodes with weak_ptr ---\n";
    {
        auto node1 = make_shared<GoodNode>("A");
        auto node2 = make_shared<GoodNode>("B");

        node1->next = node2;  // A → B (weak_ptr)
        node2->next = node1;  // B → A (weak_ptr)

        cout << "\n   References created (no cycle!):\n";
        cout << "   node1.use_count = " << node1.use_count() << " (only node1 owns A)\n";
        cout << "   node2.use_count = " << node2.use_count() << " (only node2 owns B)\n";

        cout << "\n--- Using weak_ptr ---\n";
        if (auto locked = node1->next.lock()) {  // Convert to shared_ptr
            cout << "   Node A's next exists: " << locked->name << "\n";
        }

        cout << "\n--- Going out of scope ---\n";
    }
    cout << "   ✅ Both nodes properly destroyed!\n";

    cout << "\n";
    cout << "WEAK_PTR OPERATIONS:\n";
    cout << "---\n";
    auto shared = make_shared<Resource>("Weak", 1);
    weak_ptr<Resource> weak = shared;

    cout << "   weak.expired()       → " << (weak.expired() ? "true" : "false") << "\n";
    cout << "   weak.use_count()     → " << weak.use_count() << "\n";
    cout << "   auto sp = weak.lock()  → ";
    if (auto sp = weak.lock()) {
        cout << "valid shared_ptr\n";
        sp->use();
    }

    shared.reset();
    cout << "\n   After shared.reset():\n";
    cout << "   weak.expired()       → " << (weak.expired() ? "true" : "false") << "\n";
    if (auto sp = weak.lock()) {
        cout << "   weak.lock()          → valid\n";
    } else {
        cout << "   weak.lock()          → nullptr (object destroyed)\n";
    }
}

// ============================================
// EXAMPLE 9: OBSERVER PATTERN
// ============================================

class Subject {
private:
    vector<weak_ptr<Resource>> observers;

public:
    void attach(shared_ptr<Resource> observer) {
        observers.push_back(observer);
        cout << "   ✅ Observer attached\n";
    }

    void notify() {
        cout << "   📢 Notifying observers...\n";

        // Remove expired observers
        observers.erase(
            remove_if(observers.begin(), observers.end(),
                      [](const weak_ptr<Resource>& wp) { return wp.expired(); }),
            observers.end()
            );

        // Notify valid observers
        for (auto& weak_obs : observers) {
            if (auto obs = weak_obs.lock()) {
                obs->use();
            }
        }
    }

    size_t observerCount() const {
        return count_if(observers.begin(), observers.end(),
                        [](const weak_ptr<Resource>& wp) { return !wp.expired(); });
    }
};

void demonstrateObserverPattern() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 9: OBSERVER PATTERN       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "Observer pattern with weak_ptr:\n";
    cout << "  - Subject doesn't own observers\n";
    cout << "  - Observers can be destroyed independently\n";
    cout << "  - No circular references\n";
    cout << "\n";

    Subject subject;

    {
        auto obs1 = make_shared<Resource>("Observer1", 1);
        auto obs2 = make_shared<Resource>("Observer2", 2);

        subject.attach(obs1);
        subject.attach(obs2);

        cout << "\n--- Notifying with 2 observers ---\n";
        subject.notify();
        cout << "   Active observers: " << subject.observerCount() << "\n";

        cout << "\n--- obs1 going out of scope ---\n";
    }

    cout << "\n--- Notifying after obs1 destroyed ---\n";
    subject.notify();
    cout << "   Active observers: " << subject.observerCount() << "\n";
    cout << "   ✅ Subject automatically detected destroyed observer!\n";
}

// ============================================
// EXAMPLE 10: THREAD SAFETY
// ============================================

void demonstrateThreadSafety() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 10: THREAD SAFETY         ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "SHARED_PTR THREAD SAFETY:\n";
    cout << "\n";
    cout << "✅ THREAD-SAFE:\n";
    cout << "  1. Reference counting (atomic operations)\n";
    cout << "  2. Creating shared_ptr from another shared_ptr\n";
    cout << "  3. Copying shared_ptr across threads\n";
    cout << "  4. use_count() reading\n";
    cout << "\n";
    cout << "❌ NOT THREAD-SAFE:\n";
    cout << "  1. Modifying same shared_ptr from multiple threads\n";
    cout << "     (need mutex for ptr = other or ptr.reset())\n";
    cout << "  2. Accessing the OBJECT itself\n";
    cout << "     (object needs its own synchronization)\n";
    cout << "\n";

    cout << "SAFE PATTERN:\n";
    cout << "---\n";
    cout << "shared_ptr<Resource> global_ptr;\n";
    cout << "\n";
    cout << "Thread 1:\n";
    cout << "  auto local_copy = global_ptr;  // ✅ Safe (atomic ref count)\n";
    cout << "  local_copy->use();              // ⚠️  Need mutex if object not thread-safe\n";
    cout << "\n";
    cout << "Thread 2:\n";
    cout << "  auto local_copy = global_ptr;  // ✅ Safe\n";
    cout << "  local_copy->use();              // ⚠️  Need mutex\n";
    cout << "\n";

    cout << "UNSAFE PATTERN:\n";
    cout << "---\n";
    cout << "Thread 1:  global_ptr.reset(new Resource(...));\n";
    cout << "Thread 2:  global_ptr.reset(new Resource(...));\n";
    cout << "❌ RACE CONDITION! Need mutex around reset()\n";
    cout << "\n";

    cout << "SOLUTION:\n";
    cout << "---\n";
    cout << "mutex mtx;\n";
    cout << "shared_ptr<Resource> global_ptr;\n";
    cout << "\n";
    cout << "Thread 1:\n";
    cout << "  {\n";
    cout << "    lock_guard<mutex> lock(mtx);\n";
    cout << "    global_ptr.reset(new Resource(...));  // ✅ Safe\n";
    cout << "  }\n";
}

// ============================================
// EXAMPLE 11: ALIASING CONSTRUCTOR
// ============================================

struct Data {
    int value;
    string name;

    Data(int v, const string& n) : value(v), name(n) {
        cout << "   ✅ Data [" << name << "] created\n";
    }

    ~Data() {
        cout << "   💥 Data [" << name << "] destroyed\n";
    }
};

void demonstrateAliasing() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 11: ALIASING CONSTRUCTOR  ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "Aliasing: shared_ptr points to sub-object\n";
    cout << "but shares ownership of parent object\n";
    cout << "\n";

    cout << "Use case: Point to member while keeping object alive\n";
    cout << "\n";

    auto data_ptr = make_shared<Data>(42, "Parent");

    // Create aliased shared_ptr to the 'value' member
    shared_ptr<int> value_ptr(data_ptr, &data_ptr->value);

    cout << "   data_ptr points to: Data object\n";
    cout << "   value_ptr points to: value member\n";
    cout << "   data_ptr.use_count = " << data_ptr.use_count() << "\n";
    cout << "   value_ptr.use_count = " << value_ptr.use_count() << "\n";
    cout << "   *value_ptr = " << *value_ptr << "\n";

    cout << "\n--- Releasing data_ptr ---\n";
    data_ptr.reset();
    cout << "   Data object still alive! (value_ptr keeps it alive)\n";
    cout << "   value_ptr.use_count = " << value_ptr.use_count() << "\n";
    cout << "   *value_ptr = " << *value_ptr << " (still valid!)\n";

    cout << "\n--- Releasing value_ptr ---\n";
    cout << "   Now Data object will be destroyed:\n";
}

// ============================================
// EXAMPLE 12: ENABLE_SHARED_FROM_THIS
// ============================================

class Node2 : public enable_shared_from_this<Node2> {
private:
    string name;

public:
    Node2(const string& n) : name(n) {
        cout << "   ✅ Node2 [" << name << "] created\n";
    }

    ~Node2() {
        cout << "   💥 Node2 [" << name << "] destroyed\n";
    }

    // Return shared_ptr to 'this' safely!
    shared_ptr<Node2> getPtr() {
        return shared_from_this();
    }

    void display() const {
        cout << "   Node2: " << name << "\n";
    }
};

void demonstrateEnableSharedFromThis() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 12: ENABLE_SHARED_FROM_THIS║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "Problem: How to get shared_ptr from 'this'?\n";
    cout << "\n";
    cout << "BAD:\n";
    cout << "  class Foo {\n";
    cout << "    shared_ptr<Foo> getPtr() {\n";
    cout << "      return shared_ptr<Foo>(this);  // ❌ Creates new control block!\n";
    cout << "    }                                 // 💥 Double delete!\n";
    cout << "  };\n";
    cout << "\n";
    cout << "GOOD:\n";
    cout << "  class Foo : public enable_shared_from_this<Foo> {\n";
    cout << "    shared_ptr<Foo> getPtr() {\n";
    cout << "      return shared_from_this();     // ✅ Shares existing control block\n";
    cout << "    }\n";
    cout << "  };\n";
    cout << "\n";

    cout << "--- Testing enable_shared_from_this ---\n";
    auto node = make_shared<Node2>("Test");
    cout << "   node.use_count = " << node.use_count() << "\n";

    auto node_ptr = node->getPtr();
    cout << "   After getPtr():\n";
    cout << "   node.use_count = " << node.use_count() << "\n";
    cout << "   node_ptr.use_count = " << node_ptr.use_count() << "\n";
    cout << "   ✅ Both share same control block!\n";

    cout << "\n";
    cout << "Use cases:\n";
    cout << "  1. Callbacks: register 'this' as callback\n";
    cout << "  2. Async operations: keep object alive during operation\n";
    cout << "  3. Return 'this' from member function\n";
    cout << "\n";
    cout << "IMPORTANT: Object MUST be owned by shared_ptr already!\n";
    cout << "           Calling shared_from_this() on stack object → exception!\n";
}

// ============================================
// EXAMPLE 13: COMMON MISTAKES
// ============================================

void demonstrateCommonMistakes() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 13: COMMON MISTAKES       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "MISTAKE 1: Creating shared_ptr from raw pointer twice\n";
    cout << "---\n";
    cout << "❌ Resource* raw = new Resource(...);\n";
    cout << "   shared_ptr<Resource> ptr1(raw);\n";
    cout << "   shared_ptr<Resource> ptr2(raw);  // 💥 Double delete!\n";
    cout << "\n";
    cout << "✅ Use make_shared or pass by shared_ptr\n";
    cout << "\n";

    cout << "MISTAKE 2: Circular references without weak_ptr\n";
    cout << "---\n";
    cout << "❌ struct Node {\n";
    cout << "     shared_ptr<Node> parent;  // Cycle!\n";
    cout << "     shared_ptr<Node> child;\n";
    cout << "   };\n";
    cout << "\n";
    cout << "✅ struct Node {\n";
    cout << "     weak_ptr<Node> parent;    // Break cycle\n";
    cout << "     shared_ptr<Node> child;\n";
    cout << "   };\n";
    cout << "\n";

    cout << "MISTAKE 3: Not using make_shared\n";
    cout << "---\n";
    cout << "❌ shared_ptr<Resource> ptr(new Resource(...));  // 2 allocations\n";
    cout << "✅ auto ptr = make_shared<Resource>(...);        // 1 allocation\n";
    cout << "\n";

    cout << "MISTAKE 4: Returning shared_ptr<this> wrong\n";
    cout << "---\n";
    cout << "❌ return shared_ptr<Foo>(this);  // 💥 New control block!\n";
    cout << "✅ return shared_from_this();     // Reuse control block\n";
    cout << "\n";

    cout << "MISTAKE 5: Storing weak_ptr when you need shared_ptr\n";
    cout << "---\n";
    cout << "❌ class Owner {\n";
    cout << "     weak_ptr<Resource> resource;  // Object can disappear!\n";
    cout << "   };\n";
    cout << "\n";
    cout << "✅ Use shared_ptr if you need to keep object alive\n";
    cout << "   Use weak_ptr only for non-owning references\n";
    cout << "\n";

    cout << "MISTAKE 6: Passing shared_ptr by value unnecessarily\n";
    cout << "---\n";
    cout << "❌ void func(shared_ptr<Resource> ptr);  // Copies! Slow!\n";
    cout << "✅ void func(const shared_ptr<Resource>& ptr);  // No copy\n";
    cout << "✅ void func(Resource& res);  // Even better if not storing\n";
    cout << "\n";

    cout << "MISTAKE 7: Using shared_ptr for arrays (C++17+)\n";
    cout << "---\n";
    cout << "❌ shared_ptr<int> arr(new int[10]);  // Wrong deleter!\n";
    cout << "❌ shared_ptr<int[]> arr(new int[10]); // C++17+, but why?\n";
    cout << "✅ vector<int> arr(10);  // RAII, better!\n";
    cout << "✅ unique_ptr<int[]> arr(new int[10]);  // If really needed\n";
}

// ============================================
// EXAMPLE 14: BEST PRACTICES
// ============================================

void demonstrateBestPractices() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 14: BEST PRACTICES        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "✅ BEST PRACTICES:\n";
    cout << "\n";
    cout << "1. PREFER UNIQUE_PTR OVER SHARED_PTR\n";
    cout << "   - Use shared_ptr only when you NEED shared ownership\n";
    cout << "   - unique_ptr is faster and has zero overhead\n";
    cout << "   - Can convert unique_ptr → shared_ptr (not reverse)\n";
    cout << "\n";
    cout << "2. ALWAYS USE MAKE_SHARED\n";
    cout << "   ✅ auto ptr = make_shared<T>(args);\n";
    cout << "   ❌ shared_ptr<T> ptr(new T(args));\n";
    cout << "   Exception: When you need custom deleter\n";
    cout << "\n";
    cout << "3. USE WEAK_PTR TO BREAK CYCLES\n";
    cout << "   - Parent ← child: shared_ptr (child)\n";
    cout << "   - Parent → child: weak_ptr (parent)\n";
    cout << "   - Observer pattern: weak_ptr\n";
    cout << "\n";
    cout << "4. PASS SHARED_PTR CORRECTLY\n";
    cout << "   ✅ void func(const shared_ptr<T>& ptr);  // Read only\n";
    cout << "   ✅ void func(shared_ptr<T> ptr);  // If storing/transferring\n";
    cout << "   ✅ void func(T& obj);  // Best if not storing pointer\n";
    cout << "   ❌ void func(shared_ptr<T>* ptr);  // Never!\n";
    cout << "\n";
    cout << "5. USE ENABLE_SHARED_FROM_THIS\n";
    cout << "   When class needs to return shared_ptr to itself\n";
    cout << "   Common in: callbacks, async operations, events\n";
    cout << "\n";
    cout << "6. THREAD SAFETY\n";
    cout << "   ✅ Ref counting is thread-safe\n";
    cout << "   ✅ Can copy shared_ptr across threads\n";
    cout << "   ❌ Modifying same shared_ptr → need mutex\n";
    cout << "   ❌ Object access → add your own sync\n";
    cout << "\n";
    cout << "7. AVOID PREMATURE SHARED_PTR\n";
    cout << "   Start with:\n";
    cout << "   1. Stack allocation (if possible)\n";
    cout << "   2. unique_ptr (if heap needed)\n";
    cout << "   3. shared_ptr (only if shared ownership)\n";
    cout << "\n";
    cout << "8. CHECK WEAK_PTR BEFORE USE\n";
    cout << "   ✅ if (auto sp = weak.lock()) { sp->use(); }\n";
    cout << "   ❌ weak.lock()->use();  // Might be nullptr!\n";
    cout << "\n";
    cout << "9. PERFORMANCE TIPS\n";
    cout << "   - make_shared > shared_ptr<T>(new T)\n";
    cout << "   - Pass by const& when not storing\n";
    cout << "   - Use unique_ptr when possible\n";
    cout << "   - Reserve capacity in vector<shared_ptr<T>>\n";
    cout << "\n";
    cout << "10. AVOID ARRAYS\n";
    cout << "    ❌ shared_ptr<T[]>\n";
    cout << "    ✅ vector<T>\n";
    cout << "    ✅ vector<unique_ptr<T>>\n";
}

// ============================================
// EXAMPLE 15: PERFORMANCE COMPARISON
// ============================================

void demonstratePerformance() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 15: PERFORMANCE           ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "MEMORY OVERHEAD:\n";
    cout << "---\n";
    cout << "Raw pointer:        8 bytes (64-bit)\n";
    cout << "unique_ptr:         8 bytes (same as raw!)\n";
    cout << "shared_ptr:        16 bytes (ptr + control block ptr)\n";
    cout << "Control block:    ~24 bytes (counts, deleter, etc.)\n";
    cout << "\n";
    cout << "Total for shared_ptr: ~40 bytes overhead!\n";
    cout << "\n";

    cout << "OPERATION COSTS:\n";
    cout << "---\n";
    cout << "Creation:\n";
    cout << "  new T:              O(1) + allocation\n";
    cout << "  make_unique:        O(1) + allocation\n";
    cout << "  make_shared:        O(1) + allocation (1x)\n";
    cout << "  shared_ptr(new T):  O(1) + allocation (2x!) ❌\n";
    cout << "\n";
    cout << "Copy:\n";
    cout << "  Raw pointer:        O(1) - just copy value\n";
    cout << "  unique_ptr:         ❌ Can't copy (move only)\n";
    cout << "  shared_ptr:         O(1) - atomic increment\n";
    cout << "\n";
    cout << "Destruction:\n";
    cout << "  Raw pointer:        O(1) - manual delete\n";
    cout << "  unique_ptr:         O(1) - delete in destructor\n";
    cout << "  shared_ptr:         O(1) - atomic decrement\n";
    cout << "                      (+ delete if count == 0)\n";
    cout << "\n";

    cout << "WHEN TO USE EACH:\n";
    cout << "---\n";
    cout << "Performance critical + single owner:\n";
    cout << "  → unique_ptr (zero overhead!)\n";
    cout << "\n";
    cout << "Need shared ownership:\n";
    cout << "  → shared_ptr (worth the cost)\n";
    cout << "\n";
    cout << "Object lifetime clear + local:\n";
    cout << "  → Stack allocation (fastest!)\n";
    cout << "\n";

    cout << "BENCHMARK EXAMPLE:\n";
    cout << "---\n";
    cout << "Operation                 | Time (relative)\n";
    cout << "--------------------------|----------------\n";
    cout << "Stack allocation          |   1x (baseline)\n";
    cout << "unique_ptr                |   1x (same!)\n";
    cout << "make_shared               |   2-3x\n";
    cout << "shared_ptr(new)           |   3-4x\n";
    cout << "shared_ptr copy           |   atomic ops\n";
    cout << "\n";
    cout << "Conclusion: Use shared_ptr when you NEED it,\n";
    cout << "            not as default!\n";
}

// ============================================
// MAIN - RUN ALL EXAMPLES
// ============================================

int main() {
    cout << "╔══════════════════════════════════════════════════════════╗\n";
    cout << "║                                                          ║\n";
    cout << "║              SHARED_PTR - COMPLETE GUIDE                 ║\n";
    cout << "║                                                          ║\n";
    cout << "╚══════════════════════════════════════════════════════════╝\n";

    try {
        demonstrateRawPointerProblems();      // Why we need smart pointers
        demonstrateSharedPtrBasics();         // Basic usage
        demonstrateReferenceCounting();       // How it works internally
        demonstrateSharedVsUnique();          // When to use each
        demonstrateMakeShared();              // make_shared vs new
        demonstrateCustomDeleters();          // Custom cleanup
        demonstrateCircularReference();       // The cycle problem
        demonstrateWeakPtr();                 // Breaking cycles
        demonstrateObserverPattern();         // Real-world pattern
        demonstrateThreadSafety();            // Multithreading
        demonstrateAliasing();                // Aliasing constructor
        demonstrateEnableSharedFromThis();    // Getting shared_ptr from this
        demonstrateCommonMistakes();          // What to avoid
        demonstrateBestPractices();           // How to use correctly
        demonstratePerformance();             // Performance implications

    } catch (const exception& e) {
        cout << "💥 Exception: " << e.what() << "\n";
    }

    cout << "\n╔══════════════════════════════════════════════════════════╗\n";
    cout << "║                    KEY TAKEAWAYS                         ║\n";
    cout << "╚══════════════════════════════════════════════════════════╝\n\n";

    cout << "1. WHAT IS SHARED_PTR?\n";
    cout << "   Smart pointer with SHARED OWNERSHIP\n";
    cout << "   - Reference counted\n";
    cout << "   - Automatically deletes when count reaches 0\n";
    cout << "   - Thread-safe reference counting\n";
    cout << "\n";

    cout << "2. WHEN TO USE:\n";
    cout << "   ✅ Multiple owners needed\n";
    cout << "   ✅ Object outlives creator\n";
    cout << "   ✅ Storing in containers\n";
    cout << "   ✅ Callbacks/observers\n";
    cout << "   ❌ Single ownership → use unique_ptr\n";
    cout << "\n";

    cout << "3. CREATION:\n";
    cout << "   ✅ auto ptr = make_shared<T>(args);  // BEST!\n";
    cout << "   ❌ shared_ptr<T> ptr(new T(args));   // 2 allocations\n";
    cout << "\n";

    cout << "4. REFERENCE COUNTING:\n";
    cout << "   - Tracks number of shared_ptr owners\n";
    cout << "   - use_count() to check\n";
    cout << "   - Delete when count reaches 0\n";
    cout << "\n";

    cout << "5. WEAK_PTR:\n";
    cout << "   - Non-owning reference\n";
    cout << "   - Breaks circular references\n";
    cout << "   - Use lock() to access object\n";
    cout << "   - Check expired() before use\n";
    cout << "\n";

    cout << "6. THREAD SAFETY:\n";
    cout << "   ✅ Ref counting is atomic\n";
    cout << "   ✅ Can copy across threads\n";
    cout << "   ❌ Modifying same ptr needs mutex\n";
    cout << "   ❌ Object access needs own sync\n";
    cout << "\n";

    cout << "7. PERFORMANCE:\n";
    cout << "   - 16 bytes per shared_ptr\n";
    cout << "   - ~24 bytes control block\n";
    cout << "   - Atomic ops for ref counting\n";
    cout << "   - Use unique_ptr when possible!\n";
    cout << "\n";

    cout << "8. COMMON PATTERNS:\n";
    cout << "   - Parent/child: parent has shared_ptr, child has weak_ptr\n";
    cout << "   - Observer: store weak_ptr to observers\n";
    cout << "   - Factory: return shared_ptr\n";
    cout << "   - Callbacks: capture shared_ptr\n";
    cout << "\n";

    cout << "9. AVOID:\n";
    cout << "   ❌ shared_ptr<T[]> for arrays\n";
    cout << "   ❌ Creating from raw pointer twice\n";
    cout << "   ❌ Circular references without weak_ptr\n";
    cout << "   ❌ Using when unique_ptr would work\n";
    cout << "\n";

    cout << "10. REMEMBER:\n";
    cout << "    Prefer: stack > unique_ptr > shared_ptr\n";
    cout << "    Always: make_shared > shared_ptr(new)\n";
    cout << "    Cycles: Use weak_ptr to break them\n";
    cout << "    Pass: By const& unless storing\n";
    cout << "\n";

    return 0;
}

/*
COMPILATION:
    g++ -std=c++17 -Wall -Wextra shared_ptr.cpp -o shared_ptr

RUN:
    ./shared_ptr

TOPICS COVERED:
    ✅ What is shared_ptr
    ✅ Why use smart pointers
    ✅ Reference counting mechanism
    ✅ make_shared vs new
    ✅ Custom deleters
    ✅ shared_ptr vs unique_ptr
    ✅ Circular references problem
    ✅ weak_ptr solution
    ✅ Observer pattern
    ✅ Thread safety
    ✅ Aliasing constructor
    ✅ enable_shared_from_this
    ✅ Common mistakes
    ✅ Best practices
    ✅ Performance implications
    ✅ When to use each smart pointer

OUTPUT: Complete demonstrations with emojis and clear sections!
*/
