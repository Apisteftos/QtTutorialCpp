#include <algorithm>
#include <map>
#include <memory>
#include <print>
#include <string>
#include <vector>
using namespace std;

// ============================================
// WHAT IS WEAK_PTR?
// ============================================

/*
THE BIG PROBLEM:
    shared_ptr can create CIRCULAR REFERENCES that cause MEMORY LEAKS!

    Example:
        Parent owns Child (shared_ptr)
        Child owns Parent (shared_ptr)
        → Both keep each other alive FOREVER!
        → MEMORY LEAK even though using smart pointers!

THE SOLUTION: weak_ptr
    - NON-OWNING reference to a shared_ptr
    - Does NOT increase reference count
    - Does NOT prevent object destruction
    - Must convert to shared_ptr to access (using lock())
    - Main use: Breaking circular references

THREE KEY CONCEPTS:

1. OBSERVING WITHOUT OWNING:
   - weak_ptr watches an object but doesn't keep it alive
   - If all shared_ptrs are destroyed, object is deleted
   - weak_ptr becomes "expired"

2. SAFE ACCESS:
   - Cannot access object directly through weak_ptr
   - Must call lock() to get a shared_ptr
   - lock() returns nullptr if object was destroyed

3. BREAKING CYCLES:
   - Use shared_ptr for ownership direction
   - Use weak_ptr for back-references
   - Prevents circular reference memory leaks

WHEN TO USE weak_ptr:
✅ Parent-child relationships (child → parent)
✅ Breaking circular references
✅ Observer pattern (observers don't own subject)
✅ Caching (cache entries can expire)
✅ Callbacks that might outlive the object
✅ Knowing if object exists without keeping it alive
*/

// ============================================
// EXAMPLE 1: THE CIRCULAR REFERENCE PROBLEM
// ============================================

class BadNode {
public:
    string name;
    shared_ptr<BadNode> next;    // ❌ Circular reference!
    shared_ptr<BadNode> prev;    // ❌ Circular reference!

    BadNode(const string& n) : name(n) {
        println("🔴 [{}] BadNode created", name);
    }

    ~BadNode() {
        println("🔴 [{}] BadNode destroyed", name);
    }
};

void demonstrateCircularProblem() {
    println("");
    println("╔════════════════════════════════════════════════╗");
    println("║  EXAMPLE 1: CIRCULAR REFERENCE PROBLEM        ║");
    println("╚════════════════════════════════════════════════╝");
    println("");

    println("Creating a cycle with shared_ptr:");
    {
        auto node1 = make_shared<BadNode>("Node1");
        auto node2 = make_shared<BadNode>("Node2");

        println("");
        println("Initial reference counts:");
        println("  node1: {}", node1.use_count());
        println("  node2: {}", node2.use_count());

        println("");
        println("Creating circular references...");
        node1->next = node2;  // node1 owns node2 (ref_count = 2)
        node2->prev = node1;  // node2 owns node1 (ref_count = 2)

        println("");
        println("After creating cycle:");
        println("  node1: {} (owned by: main + node2)", node1.use_count());
        println("  node2: {} (owned by: main + node1)", node2.use_count());

        println("");
        println("--- Leaving scope ---");
        // node1 and node2 go out of scope
        // - node1 ref_count: 2 → 1 (still alive because node2 owns it)
        // - node2 ref_count: 2 → 1 (still alive because node1 owns it)
        // MEMORY LEAK! Both objects leaked!
    }

    println("");
    println("💥 NOTICE: Destructors NOT called! MEMORY LEAKED!");
}

// ============================================
// EXAMPLE 2: WEAK_PTR SOLUTION
// ============================================

class GoodNode {
public:
    string name;
    shared_ptr<GoodNode> next;   // Ownership direction →
    weak_ptr<GoodNode> prev;     // ✅ Back-reference (non-owning)

    GoodNode(const string& n) : name(n) {
        println("🟢 [{}] GoodNode created", name);
    }

    ~GoodNode() {
        println("🟢 [{}] GoodNode destroyed", name);
    }

    void showPrev() const {
        if (auto p = prev.lock()) {  // Convert weak_ptr to shared_ptr
            println("   [{}] prev = {}", name, p->name);
        } else {
            println("   [{}] prev is expired (or null)", name);
        }
    }
};

void demonstrateWeakPtrSolution() {
    println("");
    println("╔════════════════════════════════════════════════╗");
    println("║  EXAMPLE 2: WEAK_PTR SOLUTION                 ║");
    println("╚════════════════════════════════════════════════╝");
    println("");

    {
        auto node1 = make_shared<GoodNode>("Node1");
        auto node2 = make_shared<GoodNode>("Node2");

        println("");
        println("Initial reference counts:");
        println("  node1: {}", node1.use_count());
        println("  node2: {}", node2.use_count());

        println("");
        println("Connecting nodes (breaking the cycle)...");
        node1->next = node2;  // node1 owns node2 (ref_count = 2)
        node2->prev = node1;  // node2 observes node1 (ref_count STILL 1!)

        println("");
        println("After connecting:");
        println("  node1: {} (owned by: main only!)", node1.use_count());
        println("  node2: {} (owned by: main + node1)", node2.use_count());

        println("");
        println("Accessing prev through weak_ptr:");
        node2->showPrev();

        println("");
        println("--- Leaving scope ---");
        // node2 (local) is destroyed first: ref_count 2 → 1 (node1->next still owns it)
        // node1 (local) is destroyed next: ref_count 1 → 0 → node1 destroyed,
        //   which releases node1->next → node2 ref_count 1 → 0 → node2 destroyed
    }

    println("");
    println("✅ SUCCESS: All destructors called! No memory leak!");
}

// ============================================
// EXAMPLE 3: WEAK_PTR BASIC OPERATIONS
// ============================================

void demonstrateBasicOperations() {
    println("");
    println("╔════════════════════════════════════════════════╗");
    println("║  EXAMPLE 3: WEAK_PTR BASIC OPERATIONS         ║");
    println("╚════════════════════════════════════════════════╝");
    println("");

    println("1. CREATING weak_ptr:");
    {
        auto shared = make_shared<int>(42);
        weak_ptr<int> weak = shared;  // Observe shared

        println("  shared ref_count: {} (not affected by weak_ptr!)", shared.use_count());
        println("");
    }

    println("2. ACCESSING through lock():");
    {
        auto shared = make_shared<int>(42);
        weak_ptr<int> weak = shared;

        if (auto locked = weak.lock()) {
            println("  ✅ Object exists! Value = {}", *locked);
            println("  locked is a shared_ptr, ref_count = {}", locked.use_count());
        } else {
            println("  ❌ Object was destroyed");
        }
        println("");
    }

    println("3. CHECKING if expired:");
    {
        auto shared = make_shared<int>(42);
        weak_ptr<int> weak = shared;

        println("  Before reset: weak.expired() = {}", weak.expired());

        shared.reset();  // Destroy the object

        println("  After reset:  weak.expired() = {}", weak.expired());
        println("");
    }

    println("4. WEAK_PTR use_count():");
    {
        auto shared = make_shared<int>(42);
        weak_ptr<int> weak = shared;

        println("  shared.use_count() = {}", shared.use_count());
        println("  weak.use_count()   = {}", weak.use_count());
        println("");
    }

    println("5. COPYING weak_ptr:");
    {
        auto shared = make_shared<int>(42);
        weak_ptr<int> weak1 = shared;
        weak_ptr<int> weak2 = weak1;  // Copy weak_ptr

        println("  shared ref_count: {} (unchanged!)", shared.use_count());
        println("");
    }

    println("6. RESETTING weak_ptr:");
    {
        auto shared = make_shared<int>(42);
        weak_ptr<int> weak = shared;

        println("  Before reset: weak.expired() = {}", weak.expired());

        weak.reset();  // Stop observing

        println("  After reset:  weak.expired() = {}", weak.expired());
    }
}

// ============================================
// EXAMPLE 4: TREE STRUCTURE (Parent-Child)
// ============================================

class TreeNode : public enable_shared_from_this<TreeNode> {
public:
    int value;
    string name;
    weak_ptr<TreeNode> parent;                // ✅ Child observes parent
    vector<shared_ptr<TreeNode>> children;    // Parent owns children

    TreeNode(int v, const string& n) : value(v), name(n) {
        println("🌳 [{}] TreeNode created (value={})", name, value);
    }

    ~TreeNode() {
        println("🌳 [{}] TreeNode destroyed", name);
    }

    // Must be called on an object that is already owned by a shared_ptr
    // (shared_from_this() throws std::bad_weak_ptr otherwise — e.g. inside a constructor)
    void addChild(shared_ptr<TreeNode> child) {
        children.push_back(child);
        child->parent = shared_from_this();  // Child observes parent
        println("   [{}] added child [{}]", name, child->name);
    }

    void showParent() const {
        if (auto p = parent.lock()) {
            println("   [{}] parent = {} (value={})", name, p->name, p->value);
        } else {
            println("   [{}] no parent", name);
        }
    }
};

void demonstrateTreeStructure() {
    println("");
    println("╔════════════════════════════════════════════════╗");
    println("║  EXAMPLE 4: TREE STRUCTURE (Parent-Child)     ║");
    println("╚════════════════════════════════════════════════╝");
    println("");

    {
        auto root = make_shared<TreeNode>(1, "Root");
        auto child1 = make_shared<TreeNode>(2, "Child1");
        auto child2 = make_shared<TreeNode>(3, "Child2");
        auto grandchild = make_shared<TreeNode>(4, "Grandchild");

        println("");
        println("Building tree:");
        root->addChild(child1);
        root->addChild(child2);
        child1->addChild(grandchild);

        println("");
        println("Reference counts:");
        println("  root: {} (main owns it)", root.use_count());
        println("  child1: {} (main + root own it)", child1.use_count());
        println("  child2: {} (main + root own it)", child2.use_count());
        println("  grandchild: {} (main + child1 own it)", grandchild.use_count());

        println("");
        println("Checking parent relationships:");
        root->showParent();
        child1->showParent();
        grandchild->showParent();

        println("");
        println("--- Leaving scope ---");
        // Locals are destroyed in reverse order: grandchild, child2, child1, root.
        // The first three only drop from 2 to 1 (their parent still owns them).
        // root drops from 1 to 0 → Root destroyed → its children vector is destroyed:
        //   Child1 (→ its children: Grandchild), then Child2.
        // Printed order: Root, Child1, Grandchild, Child2.
    }
}

// ============================================
// EXAMPLE 5: OBSERVER PATTERN
// ============================================

class Subject{
private:
    string name;
    int state;
    vector<weak_ptr<class Observer>> observers;  // Subject doesn't keep observers alive

public:
    Subject(const string& n, int s) : name(n), state(s) {
        println("📡 Subject '{}' created (state={})", name, state);
    }

    ~Subject() {
        println("📡 Subject '{}' destroyed", name);
    }

    void attach(shared_ptr<Observer> obs) {
        observers.push_back(obs);
        println("   Subject attached an observer");
    }

    void setState(int s) {
        state = s;
        println("   Subject state changed to {}", state);
        notify();
    }

    int getState() const { return state; }
    string getName() const { return name; }

    void notify();  // Defined after Observer class
};

class Observer {
private:
    string name;
    weak_ptr<Subject> subject;  // ✅ Observer doesn't own subject

public:
    // Note: the constructor must NOT call subject->attach(shared_from_this()) —
    // the object is not owned by a shared_ptr yet while its constructor runs,
    // so shared_from_this() would throw std::bad_weak_ptr. Attach after make_shared instead.
    Observer(const string& n, shared_ptr<Subject> s) : name(n), subject(s) {
        println("👁️  Observer '{}' created", name);
    }

    ~Observer() {
        println("👁️  Observer '{}' destroyed", name);
    }

    void update() {
        if (auto subj = subject.lock()) {
            println("   [{}] notified: {} = {}", name, subj->getName(), subj->getState());
        } else {
            println("   [{}] Subject no longer exists!", name);
        }
    }

    void checkSubject() {
        if (auto subj = subject.lock()) {
            println("   [{}] Subject exists: {} = {}", name, subj->getName(), subj->getState());
        } else {
            println("   [{}] Subject expired!", name);
        }
    }
};

void Subject::notify() {
    // Remove expired observers while notifying
    observers.erase(
        remove_if(observers.begin(), observers.end(),
                  [](const weak_ptr<Observer>& w) {
                      if (auto obs = w.lock()) {
                          obs->update();
                          return false;  // Keep this observer
                      }
                      return true;  // Remove expired observer
                  }),
        observers.end()
        );
}

void demonstrateObserverPattern() {
    println("");
    println("╔════════════════════════════════════════════════╗");
    println("║  EXAMPLE 5: OBSERVER PATTERN                  ║");
    println("╚════════════════════════════════════════════════╝");
    println("");

    auto subject = make_shared<Subject>("Temperature", 20);

    {
        auto obs1 = make_shared<Observer>("Thermometer", subject);
        subject->attach(obs1);
        auto obs2 = make_shared<Observer>("Display", subject);
        subject->attach(obs2);

        println("");
        println("📊 Observers checking subject:");
        obs1->checkSubject();
        obs2->checkSubject();

        println("");
        println("📊 Changing subject state:");
        subject->setState(25);

        println("");
        println("📊 Observers checking again:");
        obs1->checkSubject();
        obs2->checkSubject();

        println("");
        println("--- Observers destroyed first ---");
    }

    println("");
    println("Subject still exists (ref_count={})", subject.use_count());

    {
        auto obs3 = make_shared<Observer>("LateObserver", subject);
        subject->attach(obs3);

        println("");
        println("--- Destroying subject while observer exists ---");
        subject.reset();

        println("");
        println("📊 LateObserver checking subject:");
        obs3->checkSubject();  // Subject expired!

        println("");
        println("--- LateObserver destroyed ---");
    }
}

// ============================================
// EXAMPLE 6: CACHING WITH WEAK_PTR
// ============================================

class ExpensiveData {
private:
    string key;
    vector<int> data;

public:
    ExpensiveData(const string& k, int size) : key(k), data(size, 42) {
        println("💾 Loading expensive data '{}' ({} elements)", key, size);
        // Simulate expensive loading
    }

    ~ExpensiveData() {
        println("💾 Unloading data '{}'", key);
    }

    string getKey() const { return key; }
    size_t getSize() const { return data.size(); }
};

class Cache {
private:
    map<string, weak_ptr<ExpensiveData>> cache;  // ✅ Cache doesn't keep data alive

public:
    shared_ptr<ExpensiveData> get(const string& key) {
        auto it = cache.find(key);

        if (it != cache.end()) {
            // Try to get cached data
            if (auto data = it->second.lock()) {
                println("✅ Cache HIT for '{}'", key);
                return data;
            } else {
                println("⚠️  Cache EXPIRED for '{}', reloading...", key);
                cache.erase(it);  // Clean up expired entry
            }
        } else {
            println("❌ Cache MISS for '{}', loading...", key);
        }

        // Load new data
        auto data = make_shared<ExpensiveData>(key, 1000);
        cache[key] = data;  // Store weak_ptr
        return data;
    }

    // Only reports the status — expired entries are removed later, in get()
    void showCacheStatus() {
        println("");
        println("📊 Cache status:");
        for (const auto& [key, weakData] : cache) {
            if (weakData.expired()) {
                println("   '{}': EXPIRED (will be cleaned up)", key);
            } else {
                println("   '{}': VALID", key);
            }
        }
    }
};

void demonstrateCaching() {
    println("");
    println("╔════════════════════════════════════════════════╗");
    println("║  EXAMPLE 6: CACHING WITH WEAK_PTR             ║");
    println("╚════════════════════════════════════════════════╝");
    println("");

    Cache cache;

    println("--- First access (cache miss) ---");
    {
        auto data1 = cache.get("UserData");
        println("  Using data: {} ({} elements)", data1->getKey(), data1->getSize());

        println("");
        println("--- Second access (cache hit) ---");
        auto data2 = cache.get("UserData");
        println("  data1 == data2: {}", data1 == data2 ? "true" : "false");

        cache.showCacheStatus();

        println("");
        println("--- Data still in use ---");
        // data1 and data2 keep the data alive
    }

    println("");
    println("--- Data destroyed (no more users) ---");
    cache.showCacheStatus();

    println("");
    println("--- Third access (cache expired) ---");
    auto data3 = cache.get("UserData");

    cache.showCacheStatus();
}

// ============================================
// EXAMPLE 7: CALLBACK WITH WEAK_PTR
// ============================================

class AsyncTask {
public:
    string name;

    AsyncTask(const string& n) : name(n) {
        println("⚡ AsyncTask '{}' created", name);
    }

    ~AsyncTask() {
        println("⚡ AsyncTask '{}' destroyed", name);
    }

    void execute() {
        println("   [{}] Task executing...", name);
    }
};

class EventHandler {
private:
    weak_ptr<AsyncTask> task;  // ✅ Handler doesn't own task
    string name;

public:
    EventHandler(const string& n, shared_ptr<AsyncTask> t)
        : task(t), name(n) {
        println("🎯 EventHandler '{}' created", name);
    }

    ~EventHandler() {
        println("🎯 EventHandler '{}' destroyed", name);
    }

    void onEvent() {
        println("");
        println("📬 [{}] Event triggered!", name);
        if (auto t = task.lock()) {
            println("   Task still exists, executing...");
            t->execute();
        } else {
            println("   Task was destroyed, callback is safe (no crash)!");
        }
    }
};

void demonstrateCallback() {
    println("");
    println("╔════════════════════════════════════════════════╗");
    println("║  EXAMPLE 7: CALLBACK WITH WEAK_PTR            ║");
    println("╚════════════════════════════════════════════════╝");
    println("");

    {
        auto task = make_shared<AsyncTask>("DataSync");
        auto handler = make_shared<EventHandler>("Handler1", task);

        println("");
        println("--- Event 1: Task exists ---");
        handler->onEvent();

        println("");
        println("--- Destroying task ---");
        task.reset();

        println("");
        println("--- Event 2: Task destroyed ---");
        handler->onEvent();  // Safe! No crash!

        println("");
        println("--- Destroying handler ---");
    }
}

// ============================================
// EXAMPLE 8: REFERENCE COUNTING DETAILS
// ============================================

void demonstrateRefCounting() {
    println("");
    println("╔════════════════════════════════════════════════╗");
    println("║  EXAMPLE 8: REFERENCE COUNTING DETAILS        ║");
    println("╚════════════════════════════════════════════════╝");
    println("");

    {
        println("--- Creating shared_ptr ---");
        auto shared1 = make_shared<int>(42);
        println("  strong_count: {}", shared1.use_count());

        println("");
        println("--- Creating weak_ptr ---");
        weak_ptr<int> weak1 = shared1;
        println("  strong_count: {} (unchanged!)", shared1.use_count());
        println("  weak1.expired(): {}", weak1.expired());

        println("");
        println("--- Creating another shared_ptr ---");
        auto shared2 = shared1;
        println("  strong_count: {}", shared2.use_count());

        println("");
        println("--- Creating another weak_ptr ---");
        weak_ptr<int> weak2 = shared1;
        println("  strong_count: {} (still unchanged!)", shared1.use_count());

        {
            println("");
            println("--- Locking weak_ptr ---");
            auto locked = weak1.lock();
            println("  strong_count: {} (temporarily increased!)", shared1.use_count());

            println("");
            println("--- locked goes out of scope ---");
        }
        println("  strong_count: {} (back to 2)", shared1.use_count());

        println("");
        println("--- Destroying shared1 ---");
        shared1.reset();
        println("  strong_count: {}", shared2.use_count());

        println("");
        println("--- Destroying shared2 ---");
        shared2.reset();
        println("  weak1.expired(): {}", weak1.expired());
        println("  weak2.expired(): {}", weak2.expired());

        println("");
        println("--- weak_ptrs go out of scope ---");
    }
}

// ============================================
// EXAMPLE 9: PERFORMANCE CONSIDERATIONS
// ============================================

void demonstratePerformance() {
    println("");
    println("╔════════════════════════════════════════════════╗");
    println("║  EXAMPLE 9: PERFORMANCE CONSIDERATIONS        ║");
    println("╚════════════════════════════════════════════════╝");
    println("");

    println("Memory overhead:");
    println("  sizeof(weak_ptr<int>): {} bytes", sizeof(weak_ptr<int>));
    println("  sizeof(shared_ptr<int>): {} bytes", sizeof(shared_ptr<int>));
}

// ============================================
// MAIN - RUN ALL EXAMPLES
// ============================================

int main() {
    println("╔══════════════════════════════════════════════════════════╗");
    println("║                                                          ║");
    println("║              WEAK_PTR - COMPLETE GUIDE                   ║");
    println("║         Breaking Circular References & More              ║");
    println("║                                                          ║");
    println("╚══════════════════════════════════════════════════════════╝");

    try {
        demonstrateCircularProblem();     // The circular reference problem
        demonstrateWeakPtrSolution();     // How weak_ptr solves it
        demonstrateBasicOperations();     // Basic weak_ptr operations
        demonstrateTreeStructure();       // Parent-child relationships
        demonstrateObserverPattern();     // Observer pattern
        demonstrateCaching();             // Caching with weak_ptr
        demonstrateCallback();            // Safe callbacks
        demonstrateRefCounting();         // Reference counting details
        demonstratePerformance();         // Size of weak_ptr vs shared_ptr

    } catch (const exception& e) {
        println("💥 Exception: {}", e.what());
    }

    return 0;
}
