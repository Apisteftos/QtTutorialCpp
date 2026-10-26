#include <algorithm>
#include <map>
#include <memory>
#include <print>
#include <string>
#include <vector>

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
    std::string name;
    std::shared_ptr<BadNode> next;    // ❌ Circular reference!
    std::shared_ptr<BadNode> prev;    // ❌ Circular reference!

    BadNode(const std::string& n) : name(n) {
        std::println("🔴 [{}] BadNode created", name);
    }

    ~BadNode() {
        std::println("🔴 [{}] BadNode destroyed", name);
    }
};

void demonstrateCircularProblem() {
    std::println("");
    std::println("╔════════════════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: CIRCULAR REFERENCE PROBLEM        ║");
    std::println("╚════════════════════════════════════════════════╝");
    std::println("");

    std::println("Creating a cycle with shared_ptr:");
    {
        auto node1 = std::make_shared<BadNode>("Node1");
        auto node2 = std::make_shared<BadNode>("Node2");

        std::println("");
        std::println("Initial reference counts:");
        std::println("  node1: {}", node1.use_count());
        std::println("  node2: {}", node2.use_count());

        std::println("");
        std::println("Creating circular references...");
        node1->next = node2;  // node1 owns node2 (ref_count = 2)
        node2->prev = node1;  // node2 owns node1 (ref_count = 2)

        std::println("");
        std::println("After creating cycle:");
        std::println("  node1: {} (owned by: main + node2)", node1.use_count());
        std::println("  node2: {} (owned by: main + node1)", node2.use_count());

        std::println("");
        std::println("--- Leaving scope ---");
        // node1 and node2 go out of scope
        // - node1 ref_count: 2 → 1 (still alive because node2 owns it)
        // - node2 ref_count: 2 → 1 (still alive because node1 owns it)
        // MEMORY LEAK! Both objects leaked!
    }

    std::println("");
    std::println("💥 NOTICE: Destructors NOT called! MEMORY LEAKED!");
}

// ============================================
// EXAMPLE 2: WEAK_PTR SOLUTION
// ============================================

class GoodNode {
public:
    std::string name;
    std::shared_ptr<GoodNode> next;   // Ownership direction →
    std::weak_ptr<GoodNode> prev;     // ✅ Back-reference (non-owning)

    GoodNode(const std::string& n) : name(n) {
        std::println("🟢 [{}] GoodNode created", name);
    }

    ~GoodNode() {
        std::println("🟢 [{}] GoodNode destroyed", name);
    }

    void showPrev() const {
        if (auto p = prev.lock()) {  // Convert weak_ptr to shared_ptr
            std::println("   [{}] prev = {}", name, p->name);
        } else {
            std::println("   [{}] prev is expired (or null)", name);
        }
    }
};

void demonstrateWeakPtrSolution() {
    std::println("");
    std::println("╔════════════════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: WEAK_PTR SOLUTION                 ║");
    std::println("╚════════════════════════════════════════════════╝");
    std::println("");

    {
        auto node1 = std::make_shared<GoodNode>("Node1");
        auto node2 = std::make_shared<GoodNode>("Node2");

        std::println("");
        std::println("Initial reference counts:");
        std::println("  node1: {}", node1.use_count());
        std::println("  node2: {}", node2.use_count());

        std::println("");
        std::println("Connecting nodes (breaking the cycle)...");
        node1->next = node2;  // node1 owns node2 (ref_count = 2)
        node2->prev = node1;  // node2 observes node1 (ref_count STILL 1!)

        std::println("");
        std::println("After connecting:");
        std::println("  node1: {} (owned by: main only!)", node1.use_count());
        std::println("  node2: {} (owned by: main + node1)", node2.use_count());

        std::println("");
        std::println("Accessing prev through weak_ptr:");
        node2->showPrev();

        std::println("");
        std::println("--- Leaving scope ---");
        // node2 (local) is destroyed first: ref_count 2 → 1 (node1->next still owns it)
        // node1 (local) is destroyed next: ref_count 1 → 0 → node1 destroyed,
        //   which releases node1->next → node2 ref_count 1 → 0 → node2 destroyed
    }

    std::println("");
    std::println("✅ SUCCESS: All destructors called! No memory leak!");
}

// ============================================
// EXAMPLE 3: WEAK_PTR BASIC OPERATIONS
// ============================================

void demonstrateBasicOperations() {
    std::println("");
    std::println("╔════════════════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: WEAK_PTR BASIC OPERATIONS         ║");
    std::println("╚════════════════════════════════════════════════╝");
    std::println("");

    std::println("1. CREATING weak_ptr:");
    {
        auto shared = std::make_shared<int>(42);
        std::weak_ptr<int> weak = shared;  // Observe shared

        std::println("  shared ref_count: {} (not affected by weak_ptr!)", shared.use_count());
        std::println("");
    }

    std::println("2. ACCESSING through lock():");
    {
        auto shared = std::make_shared<int>(42);
        std::weak_ptr<int> weak = shared;

        if (auto locked = weak.lock()) {
            std::println("  ✅ Object exists! Value = {}", *locked);
            std::println("  locked is a shared_ptr, ref_count = {}", locked.use_count());
        } else {
            std::println("  ❌ Object was destroyed");
        }
        std::println("");
    }

    std::println("3. CHECKING if expired:");
    {
        auto shared = std::make_shared<int>(42);
        std::weak_ptr<int> weak = shared;

        std::println("  Before reset: weak.expired() = {}", weak.expired());

        shared.reset();  // Destroy the object

        std::println("  After reset:  weak.expired() = {}", weak.expired());
        std::println("");
    }

    std::println("4. WEAK_PTR use_count():");
    {
        auto shared = std::make_shared<int>(42);
        std::weak_ptr<int> weak = shared;

        std::println("  shared.use_count() = {}", shared.use_count());
        std::println("  weak.use_count()   = {}", weak.use_count());
        std::println("");
    }

    std::println("5. COPYING weak_ptr:");
    {
        auto shared = std::make_shared<int>(42);
        std::weak_ptr<int> weak1 = shared;
        std::weak_ptr<int> weak2 = weak1;  // Copy weak_ptr

        std::println("  shared ref_count: {} (unchanged!)", shared.use_count());
        std::println("");
    }

    std::println("6. RESETTING weak_ptr:");
    {
        auto shared = std::make_shared<int>(42);
        std::weak_ptr<int> weak = shared;

        std::println("  Before reset: weak.expired() = {}", weak.expired());

        weak.reset();  // Stop observing

        std::println("  After reset:  weak.expired() = {}", weak.expired());
    }
}

// ============================================
// EXAMPLE 4: TREE STRUCTURE (Parent-Child)
// ============================================

class TreeNode : public std::enable_shared_from_this<TreeNode> {
public:
    int value;
    std::string name;
    std::weak_ptr<TreeNode> parent;                // ✅ Child observes parent
    std::vector<std::shared_ptr<TreeNode>> children;    // Parent owns children

    TreeNode(int v, const std::string& n) : value(v), name(n) {
        std::println("🌳 [{}] TreeNode created (value={})", name, value);
    }

    ~TreeNode() {
        std::println("🌳 [{}] TreeNode destroyed", name);
    }

    // Must be called on an object that is already owned by a shared_ptr
    // (shared_from_this() throws std::bad_weak_ptr otherwise — e.g. inside a constructor)
    void addChild(std::shared_ptr<TreeNode> child) {
        children.push_back(child);
        child->parent = shared_from_this();  // Child observes parent
        std::println("   [{}] added child [{}]", name, child->name);
    }

    void showParent() const {
        if (auto p = parent.lock()) {
            std::println("   [{}] parent = {} (value={})", name, p->name, p->value);
        } else {
            std::println("   [{}] no parent", name);
        }
    }
};

void demonstrateTreeStructure() {
    std::println("");
    std::println("╔════════════════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: TREE STRUCTURE (Parent-Child)     ║");
    std::println("╚════════════════════════════════════════════════╝");
    std::println("");

    {
        auto root = std::make_shared<TreeNode>(1, "Root");
        auto child1 = std::make_shared<TreeNode>(2, "Child1");
        auto child2 = std::make_shared<TreeNode>(3, "Child2");
        auto grandchild = std::make_shared<TreeNode>(4, "Grandchild");

        std::println("");
        std::println("Building tree:");
        root->addChild(child1);
        root->addChild(child2);
        child1->addChild(grandchild);

        std::println("");
        std::println("Reference counts:");
        std::println("  root: {} (main owns it)", root.use_count());
        std::println("  child1: {} (main + root own it)", child1.use_count());
        std::println("  child2: {} (main + root own it)", child2.use_count());
        std::println("  grandchild: {} (main + child1 own it)", grandchild.use_count());

        std::println("");
        std::println("Checking parent relationships:");
        root->showParent();
        child1->showParent();
        grandchild->showParent();

        std::println("");
        std::println("--- Leaving scope ---");
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
    std::string name;
    int state;
    std::vector<std::weak_ptr<class Observer>> observers;  // Subject doesn't keep observers alive

public:
    Subject(const std::string& n, int s) : name(n), state(s) {
        std::println("📡 Subject '{}' created (state={})", name, state);
    }

    ~Subject() {
        std::println("📡 Subject '{}' destroyed", name);
    }

    void attach(std::shared_ptr<Observer> obs) {
        observers.push_back(obs);
        std::println("   Subject attached an observer");
    }

    void setState(int s) {
        state = s;
        std::println("   Subject state changed to {}", state);
        notify();
    }

    int getState() const { return state; }
    std::string getName() const { return name; }

    void notify();  // Defined after Observer class
};

class Observer {
private:
    std::string name;
    std::weak_ptr<Subject> subject;  // ✅ Observer doesn't own subject

public:
    // Note: the constructor must NOT call subject->attach(shared_from_this()) —
    // the object is not owned by a shared_ptr yet while its constructor runs,
    // so shared_from_this() would throw std::bad_weak_ptr. Attach after make_shared instead.
    Observer(const std::string& n, std::shared_ptr<Subject> s) : name(n), subject(s) {
        std::println("👁️  Observer '{}' created", name);
    }

    ~Observer() {
        std::println("👁️  Observer '{}' destroyed", name);
    }

    void update() {
        if (auto subj = subject.lock()) {
            std::println("   [{}] notified: {} = {}", name, subj->getName(), subj->getState());
        } else {
            std::println("   [{}] Subject no longer exists!", name);
        }
    }

    void checkSubject() {
        if (auto subj = subject.lock()) {
            std::println("   [{}] Subject exists: {} = {}", name, subj->getName(), subj->getState());
        } else {
            std::println("   [{}] Subject expired!", name);
        }
    }
};

void Subject::notify() {
    // Remove expired observers while notifying
    observers.erase(
        std::remove_if(observers.begin(), observers.end(),
                  [](const std::weak_ptr<Observer>& w) {
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
    std::println("");
    std::println("╔════════════════════════════════════════════════╗");
    std::println("║  EXAMPLE 5: OBSERVER PATTERN                  ║");
    std::println("╚════════════════════════════════════════════════╝");
    std::println("");

    auto subject = std::make_shared<Subject>("Temperature", 20);

    {
        auto obs1 = std::make_shared<Observer>("Thermometer", subject);
        subject->attach(obs1);
        auto obs2 = std::make_shared<Observer>("Display", subject);
        subject->attach(obs2);

        std::println("");
        std::println("📊 Observers checking subject:");
        obs1->checkSubject();
        obs2->checkSubject();

        std::println("");
        std::println("📊 Changing subject state:");
        subject->setState(25);

        std::println("");
        std::println("📊 Observers checking again:");
        obs1->checkSubject();
        obs2->checkSubject();

        std::println("");
        std::println("--- Observers destroyed first ---");
    }

    std::println("");
    std::println("Subject still exists (ref_count={})", subject.use_count());

    {
        auto obs3 = std::make_shared<Observer>("LateObserver", subject);
        subject->attach(obs3);

        std::println("");
        std::println("--- Destroying subject while observer exists ---");
        subject.reset();

        std::println("");
        std::println("📊 LateObserver checking subject:");
        obs3->checkSubject();  // Subject expired!

        std::println("");
        std::println("--- LateObserver destroyed ---");
    }
}

// ============================================
// EXAMPLE 6: CACHING WITH WEAK_PTR
// ============================================

class ExpensiveData {
private:
    std::string key;
    std::vector<int> data;

public:
    ExpensiveData(const std::string& k, int size) : key(k), data(size, 42) {
        std::println("💾 Loading expensive data '{}' ({} elements)", key, size);
        // Simulate expensive loading
    }

    ~ExpensiveData() {
        std::println("💾 Unloading data '{}'", key);
    }

    std::string getKey() const { return key; }
    size_t getSize() const { return data.size(); }
};

class Cache {
private:
    std::map<std::string, std::weak_ptr<ExpensiveData>> cache;  // ✅ Cache doesn't keep data alive

public:
    std::shared_ptr<ExpensiveData> get(const std::string& key) {
        auto it = cache.find(key);

        if (it != cache.end()) {
            // Try to get cached data
            if (auto data = it->second.lock()) {
                std::println("✅ Cache HIT for '{}'", key);
                return data;
            } else {
                std::println("⚠️  Cache EXPIRED for '{}', reloading...", key);
                cache.erase(it);  // Clean up expired entry
            }
        } else {
            std::println("❌ Cache MISS for '{}', loading...", key);
        }

        // Load new data
        auto data = std::make_shared<ExpensiveData>(key, 1000);
        cache[key] = data;  // Store weak_ptr
        return data;
    }

    // Only reports the status — expired entries are removed later, in get()
    void showCacheStatus() {
        std::println("");
        std::println("📊 Cache status:");
        for (const auto& [key, weakData] : cache) {
            if (weakData.expired()) {
                std::println("   '{}': EXPIRED (will be cleaned up)", key);
            } else {
                std::println("   '{}': VALID", key);
            }
        }
    }
};

void demonstrateCaching() {
    std::println("");
    std::println("╔════════════════════════════════════════════════╗");
    std::println("║  EXAMPLE 6: CACHING WITH WEAK_PTR             ║");
    std::println("╚════════════════════════════════════════════════╝");
    std::println("");

    Cache cache;

    std::println("--- First access (cache miss) ---");
    {
        auto data1 = cache.get("UserData");
        std::println("  Using data: {} ({} elements)", data1->getKey(), data1->getSize());

        std::println("");
        std::println("--- Second access (cache hit) ---");
        auto data2 = cache.get("UserData");
        std::println("  data1 == data2: {}", data1 == data2 ? "true" : "false");

        cache.showCacheStatus();

        std::println("");
        std::println("--- Data still in use ---");
        // data1 and data2 keep the data alive
    }

    std::println("");
    std::println("--- Data destroyed (no more users) ---");
    cache.showCacheStatus();

    std::println("");
    std::println("--- Third access (cache expired) ---");
    auto data3 = cache.get("UserData");

    cache.showCacheStatus();
}

// ============================================
// EXAMPLE 7: CALLBACK WITH WEAK_PTR
// ============================================

class AsyncTask {
public:
    std::string name;

    AsyncTask(const std::string& n) : name(n) {
        std::println("⚡ AsyncTask '{}' created", name);
    }

    ~AsyncTask() {
        std::println("⚡ AsyncTask '{}' destroyed", name);
    }

    void execute() {
        std::println("   [{}] Task executing...", name);
    }
};

class EventHandler {
private:
    std::weak_ptr<AsyncTask> task;  // ✅ Handler doesn't own task
    std::string name;

public:
    EventHandler(const std::string& n, std::shared_ptr<AsyncTask> t)
        : task(t), name(n) {
        std::println("🎯 EventHandler '{}' created", name);
    }

    ~EventHandler() {
        std::println("🎯 EventHandler '{}' destroyed", name);
    }

    void onEvent() {
        std::println("");
        std::println("📬 [{}] Event triggered!", name);
        if (auto t = task.lock()) {
            std::println("   Task still exists, executing...");
            t->execute();
        } else {
            std::println("   Task was destroyed, callback is safe (no crash)!");
        }
    }
};

void demonstrateCallback() {
    std::println("");
    std::println("╔════════════════════════════════════════════════╗");
    std::println("║  EXAMPLE 7: CALLBACK WITH WEAK_PTR            ║");
    std::println("╚════════════════════════════════════════════════╝");
    std::println("");

    {
        auto task = std::make_shared<AsyncTask>("DataSync");
        auto handler = std::make_shared<EventHandler>("Handler1", task);

        std::println("");
        std::println("--- Event 1: Task exists ---");
        handler->onEvent();

        std::println("");
        std::println("--- Destroying task ---");
        task.reset();

        std::println("");
        std::println("--- Event 2: Task destroyed ---");
        handler->onEvent();  // Safe! No crash!

        std::println("");
        std::println("--- Destroying handler ---");
    }
}

// ============================================
// EXAMPLE 8: REFERENCE COUNTING DETAILS
// ============================================

void demonstrateRefCounting() {
    std::println("");
    std::println("╔════════════════════════════════════════════════╗");
    std::println("║  EXAMPLE 8: REFERENCE COUNTING DETAILS        ║");
    std::println("╚════════════════════════════════════════════════╝");
    std::println("");

    {
        std::println("--- Creating shared_ptr ---");
        auto shared1 = std::make_shared<int>(42);
        std::println("  strong_count: {}", shared1.use_count());

        std::println("");
        std::println("--- Creating weak_ptr ---");
        std::weak_ptr<int> weak1 = shared1;
        std::println("  strong_count: {} (unchanged!)", shared1.use_count());
        std::println("  weak1.expired(): {}", weak1.expired());

        std::println("");
        std::println("--- Creating another shared_ptr ---");
        auto shared2 = shared1;
        std::println("  strong_count: {}", shared2.use_count());

        std::println("");
        std::println("--- Creating another weak_ptr ---");
        std::weak_ptr<int> weak2 = shared1;
        std::println("  strong_count: {} (still unchanged!)", shared1.use_count());

        {
            std::println("");
            std::println("--- Locking weak_ptr ---");
            auto locked = weak1.lock();
            std::println("  strong_count: {} (temporarily increased!)", shared1.use_count());

            std::println("");
            std::println("--- locked goes out of scope ---");
        }
        std::println("  strong_count: {} (back to 2)", shared1.use_count());

        std::println("");
        std::println("--- Destroying shared1 ---");
        shared1.reset();
        std::println("  strong_count: {}", shared2.use_count());

        std::println("");
        std::println("--- Destroying shared2 ---");
        shared2.reset();
        std::println("  weak1.expired(): {}", weak1.expired());
        std::println("  weak2.expired(): {}", weak2.expired());

        std::println("");
        std::println("--- weak_ptrs go out of scope ---");
    }
}

// ============================================
// EXAMPLE 9: PERFORMANCE CONSIDERATIONS
// ============================================

void demonstratePerformance() {
    std::println("");
    std::println("╔════════════════════════════════════════════════╗");
    std::println("║  EXAMPLE 9: PERFORMANCE CONSIDERATIONS        ║");
    std::println("╚════════════════════════════════════════════════╝");
    std::println("");

    std::println("Memory overhead:");
    std::println("  sizeof(weak_ptr<int>): {} bytes", sizeof(std::weak_ptr<int>));
    std::println("  sizeof(shared_ptr<int>): {} bytes", sizeof(std::shared_ptr<int>));
}

// ============================================
// MAIN - RUN ALL EXAMPLES
// ============================================

int main() {
    std::println("╔══════════════════════════════════════════════════════════╗");
    std::println("║                                                          ║");
    std::println("║              WEAK_PTR - COMPLETE GUIDE                   ║");
    std::println("║         Breaking Circular References & More              ║");
    std::println("║                                                          ║");
    std::println("╚══════════════════════════════════════════════════════════╝");

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

    } catch (const std::exception& e) {
        std::println("💥 Exception: {}", e.what());
    }

    return 0;
}
