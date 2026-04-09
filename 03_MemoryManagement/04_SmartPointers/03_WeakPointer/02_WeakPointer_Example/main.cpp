#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <algorithm>
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
        cout << "🔴 [" << name << "] BadNode created\n";
    }

    ~BadNode() {
        cout << "🔴 [" << name << "] BadNode destroyed\n";
    }
};

void demonstrateCircularProblem() {
    cout << "\n╔════════════════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: CIRCULAR REFERENCE PROBLEM        ║\n";
    cout << "╚════════════════════════════════════════════════╝\n\n";

    cout << "THE PROBLEM: shared_ptr circular references\n";
    cout << "\n";
    cout << "Creating a cycle with shared_ptr:\n";
    {
        auto node1 = make_shared<BadNode>("Node1");
        auto node2 = make_shared<BadNode>("Node2");

        cout << "\nInitial reference counts:\n";
        cout << "  node1: " << node1.use_count() << "\n";
        cout << "  node2: " << node2.use_count() << "\n";

        cout << "\nCreating circular references...\n";
        node1->next = node2;  // node1 owns node2 (ref_count = 2)
        node2->prev = node1;  // node2 owns node1 (ref_count = 2)

        cout << "\nAfter creating cycle:\n";
        cout << "  node1: " << node1.use_count() << " (owned by: main + node2)\n";
        cout << "  node2: " << node2.use_count() << " (owned by: main + node1)\n";

        cout << "\n⚠️  CIRCULAR REFERENCE DETECTED!\n";
        cout << "⚠️  node1 keeps node2 alive\n";
        cout << "⚠️  node2 keeps node1 alive\n";
        cout << "⚠️  Both will NEVER be destroyed!\n";

        cout << "\n--- Leaving scope ---\n";
        // node1 and node2 go out of scope
        // - node1 ref_count: 2 → 1 (still alive because node2 owns it)
        // - node2 ref_count: 2 → 1 (still alive because node1 owns it)
        // MEMORY LEAK! Both objects leaked!
    }

    cout << "\n💥 NOTICE: Destructors NOT called! MEMORY LEAKED!\n";
    cout << "This is why we need weak_ptr!\n";
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
        cout << "🟢 [" << name << "] GoodNode created\n";
    }

    ~GoodNode() {
        cout << "🟢 [" << name << "] GoodNode destroyed\n";
    }

    void showPrev() const {
        if (auto p = prev.lock()) {  // Convert weak_ptr to shared_ptr
            cout << "   [" << name << "] prev = " << p->name << "\n";
        } else {
            cout << "   [" << name << "] prev is expired (or null)\n";
        }
    }
};

void demonstrateWeakPtrSolution() {
    cout << "\n╔════════════════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: WEAK_PTR SOLUTION                 ║\n";
    cout << "╚════════════════════════════════════════════════╝\n\n";

    cout << "THE SOLUTION: Use weak_ptr for back-references\n";
    cout << "\n";
    cout << "class GoodNode {\n";
    cout << "    shared_ptr<GoodNode> next;  // Ownership →\n";
    cout << "    weak_ptr<GoodNode> prev;    // Observe ←\n";
    cout << "};\n";
    cout << "\n";

    {
        auto node1 = make_shared<GoodNode>("Node1");
        auto node2 = make_shared<GoodNode>("Node2");

        cout << "\nInitial reference counts:\n";
        cout << "  node1: " << node1.use_count() << "\n";
        cout << "  node2: " << node2.use_count() << "\n";

        cout << "\nConnecting nodes (breaking the cycle)...\n";
        node1->next = node2;  // node1 owns node2 (ref_count = 2)
        node2->prev = node1;  // node2 observes node1 (ref_count STILL 1!)

        cout << "\nAfter connecting:\n";
        cout << "  node1: " << node1.use_count() << " (owned by: main only!)\n";
        cout << "  node2: " << node2.use_count() << " (owned by: main + node1)\n";

        cout << "\n✅ NO CIRCULAR REFERENCE!\n";
        cout << "✅ node2 observes node1 but doesn't own it\n";

        cout << "\nAccessing prev through weak_ptr:\n";
        node2->showPrev();

        cout << "\n--- Leaving scope ---\n";
        // node1 and node2 go out of scope
        // - node1 ref_count: 1 → 0 (DESTROYED!)
        // - node2 ref_count: 2 → 1 → 0 (DESTROYED!)
        // SUCCESS! All memory cleaned up!
    }

    cout << "\n✅ SUCCESS: All destructors called! No memory leak!\n";
}

// ============================================
// EXAMPLE 3: WEAK_PTR BASIC OPERATIONS
// ============================================

void demonstrateBasicOperations() {
    cout << "\n╔════════════════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: WEAK_PTR BASIC OPERATIONS         ║\n";
    cout << "╚════════════════════════════════════════════════╝\n\n";

    cout << "1. CREATING weak_ptr:\n";
    {
        auto shared = make_shared<int>(42);
        weak_ptr<int> weak = shared;  // Observe shared

        cout << "  shared ref_count: " << shared.use_count() << " (not affected by weak_ptr!)\n";
        cout << "\n";
    }

    cout << "2. ACCESSING through lock():\n";
    {
        auto shared = make_shared<int>(42);
        weak_ptr<int> weak = shared;

        if (auto locked = weak.lock()) {
            cout << "  ✅ Object exists! Value = " << *locked << "\n";
            cout << "  locked is a shared_ptr, ref_count = " << locked.use_count() << "\n";
        } else {
            cout << "  ❌ Object was destroyed\n";
        }
        cout << "\n";
    }

    cout << "3. CHECKING if expired:\n";
    {
        auto shared = make_shared<int>(42);
        weak_ptr<int> weak = shared;

        cout << "  Before reset: weak.expired() = "
             << (weak.expired() ? "true" : "false") << "\n";

        shared.reset();  // Destroy the object

        cout << "  After reset:  weak.expired() = "
             << (weak.expired() ? "true" : "false") << "\n";
        cout << "\n";
    }

    cout << "4. WEAK_PTR use_count():\n";
    {
        auto shared = make_shared<int>(42);
        weak_ptr<int> weak = shared;

        cout << "  shared.use_count() = " << shared.use_count() << "\n";
        cout << "  weak.use_count()   = " << weak.use_count() << "\n";
        cout << "  (Both return the number of shared_ptr owners)\n";
        cout << "\n";
    }

    cout << "5. COPYING weak_ptr:\n";
    {
        auto shared = make_shared<int>(42);
        weak_ptr<int> weak1 = shared;
        weak_ptr<int> weak2 = weak1;  // Copy weak_ptr

        cout << "  shared ref_count: " << shared.use_count() << " (unchanged!)\n";
        cout << "  Multiple weak_ptrs can observe same object\n";
        cout << "\n";
    }

    cout << "6. RESETTING weak_ptr:\n";
    {
        auto shared = make_shared<int>(42);
        weak_ptr<int> weak = shared;

        cout << "  Before reset: weak.expired() = "
             << (weak.expired() ? "true" : "false") << "\n";

        weak.reset();  // Stop observing

        cout << "  After reset:  weak.expired() = "
             << (weak.expired() ? "true" : "false") << "\n";
        cout << "  weak is now empty (doesn't observe anything)\n";
    }
}

// ============================================
// EXAMPLE 4: TREE STRUCTURE (Parent-Child)
// ============================================

class TreeNode: public enable_shared_from_this<TreeNode> {
public:
    int value;
    string name;
    weak_ptr<TreeNode> parent;           // ✅ Child observes parent
    vector<shared_ptr<TreeNode>> children;  // Parent owns children

    TreeNode(int v, const string& n) : value(v), name(n) {
        cout << "🌳 [" << name << "] TreeNode created (value=" << value << ")\n";
    }

    ~TreeNode() {
        cout << "🌳 [" << name << "] TreeNode destroyed\n";
    }

    void addChild(shared_ptr<TreeNode> child) {
        children.push_back(child);
        child->parent = shared_from_this();  // Child observes parent
        cout << "   [" << name << "] added child [" << child->name << "]\n";
    }

    void showParent() const {
        if (auto p = parent.lock()) {
            cout << "   [" << name << "] parent = " << p->name << "\n";
        } else {
            cout << "   [" << name << "] no parent (root or parent destroyed)\n";
        }
    }

    void showChildren() const {
        cout << "   [" << name << "] children: ";
        if (children.empty()) {
            cout << "none\n";
        } else {
            for (const auto& child : children) {
                cout << child->name << " ";
            }
            cout << "\n";
        }
    }
};

// TreeNode needs to inherit from enable_shared_from_this
class TreeNodeV2 : public enable_shared_from_this<TreeNodeV2> {
public:
    int value;
    string name;
    weak_ptr<TreeNodeV2> parent;
    vector<shared_ptr<TreeNodeV2>> children;

    TreeNodeV2(int v, const string& n) : value(v), name(n) {
        cout << "🌳 [" << name << "] TreeNode created (value=" << value << ")\n";
    }

    ~TreeNodeV2() {
        cout << "🌳 [" << name << "] TreeNode destroyed\n";
    }

    void addChild(shared_ptr<TreeNodeV2> child) {
        children.push_back(child);
        child->parent = shared_from_this();
        cout << "   [" << name << "] added child [" << child->name << "]\n";
    }

    void showParent() const {
        if (auto p = parent.lock()) {
            cout << "   [" << name << "] parent = " << p->name << " (value=" << p->value << ")\n";
        } else {
            cout << "   [" << name << "] no parent\n";
        }
    }
};

void demonstrateTreeStructure() {
    cout << "\n╔════════════════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: TREE STRUCTURE (Parent-Child)     ║\n";
    cout << "╚════════════════════════════════════════════════╝\n\n";

    cout << "Tree structure with weak_ptr:\n";
    cout << "  - Parent owns children (shared_ptr)\n";
    cout << "  - Children observe parent (weak_ptr)\n";
    cout << "  - No circular references!\n";
    cout << "\n";

    {
        auto root = make_shared<TreeNodeV2>(1, "Root");
        auto child1 = make_shared<TreeNodeV2>(2, "Child1");
        auto child2 = make_shared<TreeNodeV2>(3, "Child2");
        auto grandchild = make_shared<TreeNodeV2>(4, "Grandchild");

        cout << "\nBuilding tree:\n";
        root->addChild(child1);
        root->addChild(child2);
        child1->addChild(grandchild);

        cout << "\nTree structure:\n";
        cout << "        Root(1)\n";
        cout << "        /     \\\n";
        cout << "   Child1(2)  Child2(3)\n";
        cout << "      |\n";
        cout << "  Grandchild(4)\n";
        cout << "\n";

        cout << "Reference counts:\n";
        cout << "  root: " << root.use_count() << " (main owns it)\n";
        cout << "  child1: " << child1.use_count() << " (main + root own it)\n";
        cout << "  child2: " << child2.use_count() << " (main + root own it)\n";
        cout << "  grandchild: " << grandchild.use_count() << " (main + child1 own it)\n";

        cout << "\nChecking parent relationships:\n";
        root->showParent();
        child1->showParent();
        grandchild->showParent();

        cout << "\n--- Leaving scope ---\n";
        // Destruction order:
        // 1. grandchild destroyed (no more references from main or child1)
        // 2. child2 destroyed (no more references from main or root)
        // 3. child1 destroyed (no more references from main or root)
        // 4. root destroyed (no more references from main)
    }

    cout << "\n✅ All nodes properly destroyed in correct order!\n";
}

// ============================================
// EXAMPLE 5: OBSERVER PATTERN
// ============================================

class Subject{
private:
    string name;
    int state;
    vector<weak_ptr<class Observer>> observers;  // Observers don't own subject

public:
    Subject(const string& n, int s) : name(n), state(s) {
        cout << "📡 Subject '" << name << "' created (state=" << state << ")\n";
    }

    ~Subject() {
        cout << "📡 Subject '" << name << "' destroyed\n";
    }

    void attach(shared_ptr<Observer> obs) {
        observers.push_back(obs);
        cout << "   Subject attached an observer\n";
    }

    void setState(int s) {
        state = s;
        cout << "   Subject state changed to " << state << "\n";
        notify();
    }

    int getState() const { return state; }
    string getName() const { return name; }

    void notify();  // Defined after Observer class
};

class Observer : public enable_shared_from_this<Observer>{
private:
    string name;
    weak_ptr<Subject> subject;  // ✅ Observer doesn't own subject

public:
    Observer(const string& n, shared_ptr<Subject> s) : name(n), subject(s) {
        cout << "👁️  Observer '" << name << "' created\n";
        s->attach(shared_from_this());  // Would need enable_shared_from_this
    }

    ~Observer() {
        cout << "👁️  Observer '" << name << "' destroyed\n";
    }

    void update() {
        if (auto subj = subject.lock()) {
            cout << "   [" << name << "] notified: "
                 << subj->getName() << " = " << subj->getState() << "\n";
        } else {
            cout << "   [" << name << "] Subject no longer exists!\n";
        }
    }

    void checkSubject() {
        if (auto subj = subject.lock()) {
            cout << "   [" << name << "] Subject exists: "
                 << subj->getName() << " = " << subj->getState() << "\n";
        } else {
            cout << "   [" << name << "] Subject expired!\n";
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
    cout << "\n╔════════════════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: OBSERVER PATTERN                  ║\n";
    cout << "╚════════════════════════════════════════════════╝\n\n";

    cout << "Observer Pattern with weak_ptr:\n";
    cout << "  - Subject is observed by multiple observers\n";
    cout << "  - Observers use weak_ptr (don't keep subject alive)\n";
    cout << "  - Observers automatically detect when subject dies\n";
    cout << "\n";

    auto subject = make_shared<Subject>("Temperature", 20);

    {
        auto obs1 = make_shared<Observer>("Thermometer", subject);
        auto obs2 = make_shared<Observer>("Display", subject);

        cout << "\n📊 Observers checking subject:\n";
        obs1->checkSubject();
        obs2->checkSubject();

        cout << "\n📊 Changing subject state:\n";
        subject->setState(25);

        cout << "\n📊 Observers checking again:\n";
        obs1->checkSubject();
        obs2->checkSubject();

        cout << "\n--- Observers destroyed first ---\n";
    }

    cout << "\nSubject still exists (ref_count=" << subject.use_count() << ")\n";
    cout << "Subject can continue to exist after observers are destroyed\n";

    {
        auto obs3 = make_shared<Observer>("LateObserver", subject);

        cout << "\n--- Destroying subject while observer exists ---\n";
        subject.reset();

        cout << "\n📊 LateObserver checking subject:\n";
        obs3->checkSubject();  // Subject expired!

        cout << "\n--- LateObserver destroyed ---\n";
    }

    cout << "\n✅ Observer pattern works perfectly with weak_ptr!\n";
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
        cout << "💾 Loading expensive data '" << key
             << "' (" << size << " elements)\n";
        // Simulate expensive loading
    }

    ~ExpensiveData() {
        cout << "💾 Unloading data '" << key << "'\n";
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
                cout << "✅ Cache HIT for '" << key << "'\n";
                return data;
            } else {
                cout << "⚠️  Cache EXPIRED for '" << key << "', reloading...\n";
                cache.erase(it);  // Clean up expired entry
            }
        } else {
            cout << "❌ Cache MISS for '" << key << "', loading...\n";
        }

        // Load new data
        auto data = make_shared<ExpensiveData>(key, 1000);
        cache[key] = data;  // Store weak_ptr
        return data;
    }

    void showCacheStatus() {
        cout << "\n📊 Cache status:\n";
        for (auto it = cache.begin(); it != cache.end();) {
            if (it->second.expired()) {
                cout << "   '" << it->first << "': EXPIRED (will be cleaned up)\n";
                it = cache.erase(it);
            } else {
                cout << "   '" << it->first << "': VALID\n";
                ++it;
            }
        }
    }
};

void demonstrateCaching() {
    cout << "\n╔════════════════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: CACHING WITH WEAK_PTR             ║\n";
    cout << "╚════════════════════════════════════════════════╝\n\n";

    cout << "Caching with weak_ptr:\n";
    cout << "  - Cache stores weak_ptr to data\n";
    cout << "  - Data lives only while clients use it\n";
    cout << "  - Cache entries automatically expire\n";
    cout << "  - No memory waste on unused cached items\n";
    cout << "\n";

    Cache cache;

    cout << "--- First access (cache miss) ---\n";
    {
        auto data1 = cache.get("UserData");
        cout << "  Using data: " << data1->getKey()
             << " (" << data1->getSize() << " elements)\n";

        cout << "\n--- Second access (cache hit) ---\n";
        auto data2 = cache.get("UserData");
        cout << "  data1 == data2: " << (data1 == data2 ? "true" : "false") << "\n";

        cache.showCacheStatus();

        cout << "\n--- Data still in use ---\n";
        // data1 and data2 keep the data alive
    }

    cout << "\n--- Data destroyed (no more users) ---\n";
    cache.showCacheStatus();

    cout << "\n--- Third access (cache expired) ---\n";
    auto data3 = cache.get("UserData");

    cache.showCacheStatus();

    cout << "\n✅ Cache automatically manages memory!\n";
}

// ============================================
// EXAMPLE 7: CALLBACK WITH WEAK_PTR
// ============================================

class AsyncTask {
public:
    string name;

    AsyncTask(const string& n) : name(n) {
        cout << "⚡ AsyncTask '" << name << "' created\n";
    }

    ~AsyncTask() {
        cout << "⚡ AsyncTask '" << name << "' destroyed\n";
    }

    void execute() {
        cout << "   [" << name << "] Task executing...\n";
    }
};

class EventHandler {
private:
    weak_ptr<AsyncTask> task;  // ✅ Handler doesn't own task
    string name;

public:
    EventHandler(const string& n, shared_ptr<AsyncTask> t)
        : task(t), name(n) {
        cout << "🎯 EventHandler '" << name << "' created\n";
    }

    ~EventHandler() {
        cout << "🎯 EventHandler '" << name << "' destroyed\n";
    }

    void onEvent() {
        cout << "\n📬 [" << name << "] Event triggered!\n";
        if (auto t = task.lock()) {
            cout << "   Task still exists, executing...\n";
            t->execute();
        } else {
            cout << "   Task was destroyed, callback is safe (no crash)!\n";
        }
    }
};

void demonstrateCallback() {
    cout << "\n╔════════════════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 7: CALLBACK WITH WEAK_PTR            ║\n";
    cout << "╚════════════════════════════════════════════════╝\n\n";

    cout << "Callbacks with weak_ptr:\n";
    cout << "  - Callback uses weak_ptr to target object\n";
    cout << "  - Safe if object is destroyed before callback fires\n";
    cout << "  - No dangling pointer risk\n";
    cout << "\n";

    {
        auto task = make_shared<AsyncTask>("DataSync");
        auto handler = make_shared<EventHandler>("Handler1", task);

        cout << "\n--- Event 1: Task exists ---\n";
        handler->onEvent();

        cout << "\n--- Destroying task ---\n";
        task.reset();

        cout << "\n--- Event 2: Task destroyed ---\n";
        handler->onEvent();  // Safe! No crash!

        cout << "\n--- Destroying handler ---\n";
    }

    cout << "\n✅ Callback safely handled task destruction!\n";
}

// ============================================
// EXAMPLE 8: REFERENCE COUNTING DETAILS
// ============================================

void demonstrateRefCounting() {
    cout << "\n╔════════════════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 8: REFERENCE COUNTING DETAILS        ║\n";
    cout << "╚════════════════════════════════════════════════╝\n\n";

    cout << "Control Block contains TWO counters:\n";
    cout << "  1. Strong count (shared_ptr count)\n";
    cout << "  2. Weak count (weak_ptr count)\n";
    cout << "\n";
    cout << "Object lifetime:\n";
    cout << "  - Object destroyed when strong_count reaches 0\n";
    cout << "  - Control block destroyed when weak_count reaches 0\n";
    cout << "\n";

    {
        cout << "--- Creating shared_ptr ---\n";
        auto shared1 = make_shared<int>(42);
        cout << "  strong_count: " << shared1.use_count() << "\n";
        cout << "  (control block created)\n";

        cout << "\n--- Creating weak_ptr ---\n";
        weak_ptr<int> weak1 = shared1;
        cout << "  strong_count: " << shared1.use_count() << " (unchanged!)\n";
        cout << "  weak_count: 1 (weak_ptr added)\n";
        cout << "  weak1.expired(): " << (weak1.expired() ? "true" : "false") << "\n";

        cout << "\n--- Creating another shared_ptr ---\n";
        auto shared2 = shared1;
        cout << "  strong_count: " << shared2.use_count() << "\n";

        cout << "\n--- Creating another weak_ptr ---\n";
        weak_ptr<int> weak2 = shared1;
        cout << "  strong_count: " << shared1.use_count() << " (still unchanged!)\n";
        cout << "  weak_count: 2\n";

        {
            cout << "\n--- Locking weak_ptr ---\n";
            auto locked = weak1.lock();
            cout << "  strong_count: " << shared1.use_count() << " (temporarily increased!)\n";
            cout << "  locked is a shared_ptr that keeps object alive\n";

            cout << "\n--- locked goes out of scope ---\n";
        }
        cout << "  strong_count: " << shared1.use_count() << " (back to 2)\n";

        cout << "\n--- Destroying shared1 ---\n";
        shared1.reset();
        cout << "  strong_count: " << shared2.use_count() << "\n";

        cout << "\n--- Destroying shared2 ---\n";
        shared2.reset();
        cout << "  strong_count: 0\n";
        cout << "  💥 OBJECT DESTROYED!\n";
        cout << "  Control block still exists (for weak_ptrs)\n";
        cout << "  weak1.expired(): " << (weak1.expired() ? "true" : "false") << "\n";
        cout << "  weak2.expired(): " << (weak2.expired() ? "true" : "false") << "\n";

        cout << "\n--- weak_ptrs go out of scope ---\n";
    }
    cout << "  weak_count: 0\n";
    cout << "  💥 CONTROL BLOCK DESTROYED!\n";

    cout << "\n✅ Control block lifecycle:\n";
    cout << "   1. Created with first shared_ptr\n";
    cout << "   2. Object destroyed when strong_count = 0\n";
    cout << "   3. Control block destroyed when weak_count = 0\n";
}

// ============================================
// EXAMPLE 9: COMMON MISTAKES
// ============================================

void demonstrateCommonMistakes() {
    cout << "\n╔════════════════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 9: COMMON MISTAKES                   ║\n";
    cout << "╚════════════════════════════════════════════════╝\n\n";

    cout << "MISTAKE 1: Using weak_ptr without lock()\n";
    cout << "  ❌ WRONG:\n";
    cout << "     weak_ptr<int> weak = shared;\n";
    cout << "     *weak;  // ERROR! Can't dereference weak_ptr!\n";
    cout << "\n";
    cout << "  ✅ CORRECT:\n";
    cout << "     weak_ptr<int> weak = shared;\n";
    cout << "     if (auto locked = weak.lock()) {\n";
    cout << "         *locked;  // Safe!\n";
    cout << "     }\n";
    cout << "\n";

    cout << "MISTAKE 2: Not checking if lock() succeeds\n";
    cout << "  ❌ WRONG:\n";
    cout << "     auto locked = weak.lock();\n";
    cout << "     locked->method();  // May crash if nullptr!\n";
    cout << "\n";
    cout << "  ✅ CORRECT:\n";
    cout << "     if (auto locked = weak.lock()) {\n";
    cout << "         locked->method();  // Safe!\n";
    cout << "     }\n";
    cout << "\n";

    cout << "MISTAKE 3: Using weak_ptr instead of shared_ptr for ownership\n";
    cout << "  ❌ WRONG:\n";
    cout << "     class Widget {\n";
    cout << "         weak_ptr<Resource> resource;  // Should own it!\n";
    cout << "     };\n";
    cout << "\n";
    cout << "  ✅ CORRECT:\n";
    cout << "     class Widget {\n";
    cout << "         shared_ptr<Resource> resource;  // Ownership\n";
    cout << "     };\n";
    cout << "\n";

    cout << "MISTAKE 4: Using shared_ptr where weak_ptr should be used\n";
    cout << "  ❌ WRONG:\n";
    cout << "     class Node {\n";
    cout << "         shared_ptr<Node> parent;  // Circular reference!\n";
    cout << "     };\n";
    cout << "\n";
    cout << "  ✅ CORRECT:\n";
    cout << "     class Node {\n";
    cout << "         weak_ptr<Node> parent;  // Break cycle!\n";
    cout << "     };\n";
    cout << "\n";

    cout << "MISTAKE 5: Storing weak_ptr in containers without cleanup\n";
    cout << "  ⚠️  SUBOPTIMAL:\n";
    cout << "     vector<weak_ptr<Observer>> observers;\n";
    cout << "     // Expired weak_ptrs accumulate!\n";
    cout << "\n";
    cout << "  ✅ BETTER:\n";
    cout << "     // Periodically remove expired weak_ptrs\n";
    cout << "     observers.erase(\n";
    cout << "         remove_if(observers.begin(), observers.end(),\n";
    cout << "             [](auto& w) { return w.expired(); }),\n";
    cout << "         observers.end()\n";
    cout << "     );\n";
    cout << "\n";

    cout << "MISTAKE 6: Creating weak_ptr from raw pointer\n";
    cout << "  ❌ WRONG:\n";
    cout << "     int* raw = new int(42);\n";
    cout << "     weak_ptr<int> weak(raw);  // ERROR! Doesn't compile!\n";
    cout << "\n";
    cout << "  ✅ CORRECT:\n";
    cout << "     auto shared = make_shared<int>(42);\n";
    cout << "     weak_ptr<int> weak = shared;  // OK!\n";
}

// ============================================
// EXAMPLE 10: BEST PRACTICES
// ============================================

void demonstrateBestPractices() {
    cout << "\n╔════════════════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 10: BEST PRACTICES                   ║\n";
    cout << "╚════════════════════════════════════════════════╝\n\n";

    cout << "✅ BEST PRACTICES:\n";
    cout << "\n";
    cout << "1. ALWAYS use lock() before accessing\n";
    cout << "   if (auto locked = weak.lock()) {\n";
    cout << "       locked->method();  // Safe!\n";
    cout << "   }\n";
    cout << "\n";
    cout << "2. Check expired() for simple existence check\n";
    cout << "   if (!weak.expired()) {\n";
    cout << "       // Object still exists (might change!)\n";
    cout << "   }\n";
    cout << "\n";
    cout << "3. Use weak_ptr for back-references\n";
    cout << "   - Parent → Child: shared_ptr\n";
    cout << "   - Child → Parent: weak_ptr\n";
    cout << "\n";
    cout << "4. Use weak_ptr in observer pattern\n";
    cout << "   - Subject owns observers: shared_ptr\n";
    cout << "   - Observers observe subject: weak_ptr\n";
    cout << "\n";
    cout << "5. Use weak_ptr for caching\n";
    cout << "   - Cache stores weak_ptr\n";
    cout << "   - Entries expire when no longer used\n";
    cout << "\n";
    cout << "6. Clean up expired weak_ptrs periodically\n";
    cout << "   observers.erase(\n";
    cout << "       remove_if(..., [](auto& w) { return w.expired(); }),\n";
    cout << "       observers.end()\n";
    cout << "   );\n";
    cout << "\n";
    cout << "7. Use weak_ptr for callbacks\n";
    cout << "   - Safe if target object is destroyed\n";
    cout << "   - No dangling pointer risk\n";
    cout << "\n";
    cout << "8. Don't use weak_ptr for ownership\n";
    cout << "   - weak_ptr is for observation\n";
    cout << "   - Use shared_ptr for ownership\n";
    cout << "   - Use unique_ptr for exclusive ownership\n";
}

// ============================================
// EXAMPLE 11: PERFORMANCE CONSIDERATIONS
// ============================================

void demonstratePerformance() {
    cout << "\n╔════════════════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 11: PERFORMANCE CONSIDERATIONS       ║\n";
    cout << "╚════════════════════════════════════════════════╝\n\n";

    cout << "Memory overhead:\n";
    cout << "  sizeof(weak_ptr<int>): " << sizeof(weak_ptr<int>) << " bytes\n";
    cout << "  sizeof(shared_ptr<int>): " << sizeof(shared_ptr<int>) << " bytes\n";
    cout << "  (Same size! Both store 2 pointers)\n";
    cout << "\n";

    cout << "Performance characteristics:\n";
    cout << "\n";
    cout << "1. CREATION:\n";
    cout << "   weak_ptr<T> weak = shared;  // Fast (no allocation)\n";
    cout << "   - Just copies pointers\n";
    cout << "   - Increments weak_count (atomic operation)\n";
    cout << "\n";
    cout << "2. LOCK:\n";
    cout << "   auto locked = weak.lock();  // Moderate cost\n";
    cout << "   - Atomic check + increment\n";
    cout << "   - Creates temporary shared_ptr\n";
    cout << "\n";
    cout << "3. EXPIRED CHECK:\n";
    cout << "   bool expired = weak.expired();  // Fast\n";
    cout << "   - Just reads strong_count\n";
    cout << "   - No allocation\n";
    cout << "\n";
    cout << "4. DESTRUCTION:\n";
    cout << "   ~weak_ptr()  // Fast\n";
    cout << "   - Decrements weak_count (atomic)\n";
    cout << "   - May delete control block if last weak_ptr\n";
    cout << "\n";

    cout << "When to use:\n";
    cout << "  ✅ Use when you need to observe without owning\n";
    cout << "  ✅ Use to break circular references\n";
    cout << "  ✅ Small overhead compared to avoiding memory leaks\n";
    cout << "  ⚠️  lock() has some cost (atomic operations)\n";
    cout << "  ⚠️  Control block persists until all weak_ptrs destroyed\n";
}

// ============================================
// EXAMPLE 12: REAL-WORLD SCENARIOS
// ============================================

void demonstrateRealWorldScenarios() {
    cout << "\n╔════════════════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 12: REAL-WORLD SCENARIOS             ║\n";
    cout << "╚════════════════════════════════════════════════╝\n\n";

    cout << "SCENARIO 1: Event System\n";
    cout << "  class EventDispatcher {\n";
    cout << "      vector<weak_ptr<EventListener>> listeners;\n";
    cout << "  };\n";
    cout << "  - Listeners can be destroyed at any time\n";
    cout << "  - Dispatcher doesn't keep them alive\n";
    cout << "  - Automatically removes dead listeners\n";
    cout << "\n";

    cout << "SCENARIO 2: UI Framework\n";
    cout << "  class Window {\n";
    cout << "      weak_ptr<Window> parent;  // Child observes parent\n";
    cout << "      vector<shared_ptr<Widget>> children;  // Parent owns children\n";
    cout << "  };\n";
    cout << "  - No circular references\n";
    cout << "  - Automatic cleanup when window destroyed\n";
    cout << "\n";

    cout << "SCENARIO 3: Resource Pool\n";
    cout << "  class ResourcePool {\n";
    cout << "      map<string, weak_ptr<Resource>> pool;\n";
    cout << "  };\n";
    cout << "  - Reuse resources while they're alive\n";
    cout << "  - Automatic cleanup when no longer used\n";
    cout << "  - No memory waste on idle resources\n";
    cout << "\n";

    cout << "SCENARIO 4: Document Editor\n";
    cout << "  class Document {\n";
    cout << "      shared_ptr<Paragraph> paragraphs;\n";
    cout << "  };\n";
    cout << "  class Bookmark {\n";
    cout << "      weak_ptr<Paragraph> target;  // Don't own paragraph\n";
    cout << "  };\n";
    cout << "  - Bookmarks observe paragraphs\n";
    cout << "  - Bookmarks become invalid if paragraph deleted\n";
    cout << "  - No memory leaks\n";
    cout << "\n";

    cout << "SCENARIO 5: Game Engine\n";
    cout << "  class Entity {\n";
    cout << "      weak_ptr<Entity> target;  // Target for AI\n";
    cout << "  };\n";
    cout << "  - AI targets other entities\n";
    cout << "  - Target can be destroyed\n";
    cout << "  - AI handles missing targets gracefully\n";
}

// ============================================
// MAIN - RUN ALL EXAMPLES
// ============================================

int main() {
    cout << "╔══════════════════════════════════════════════════════════╗\n";
    cout << "║                                                          ║\n";
    cout << "║              WEAK_PTR - COMPLETE GUIDE                   ║\n";
    cout << "║         Breaking Circular References & More              ║\n";
    cout << "║                                                          ║\n";
    cout << "╚══════════════════════════════════════════════════════════╝\n";

    try {
        demonstrateCircularProblem();     // The circular reference problem
        demonstrateWeakPtrSolution();     // How weak_ptr solves it
        demonstrateBasicOperations();     // Basic weak_ptr operations
        demonstrateTreeStructure();       // Parent-child relationships
        demonstrateObserverPattern();     // Observer pattern
        demonstrateCaching();             // Caching with weak_ptr
        demonstrateCallback();            // Safe callbacks
        demonstrateRefCounting();         // Reference counting details
        demonstrateCommonMistakes();      // Common mistakes
        demonstrateBestPractices();       // Best practices
        demonstratePerformance();         // Performance considerations
        demonstrateRealWorldScenarios();  // Real-world use cases

    } catch (const exception& e) {
        cout << "💥 Exception: " << e.what() << "\n";
    }

    cout << "\n╔══════════════════════════════════════════════════════════╗\n";
    cout << "║                    KEY TAKEAWAYS                         ║\n";
    cout << "╚══════════════════════════════════════════════════════════╝\n\n";

    cout << "1. WHAT IS WEAK_PTR:\n";
    cout << "   - Non-owning observer of shared_ptr\n";
    cout << "   - Does NOT increase reference count\n";
    cout << "   - Does NOT prevent object destruction\n";
    cout << "   - Must use lock() to access object\n";
    cout << "\n";

    cout << "2. WHY USE WEAK_PTR:\n";
    cout << "   ✅ Break circular references (MAIN USE)\n";
    cout << "   ✅ Observer pattern (observe without owning)\n";
    cout << "   ✅ Caching (entries can expire)\n";
    cout << "   ✅ Callbacks (safe if target destroyed)\n";
    cout << "   ✅ Parent-child relationships (child → parent)\n";
    cout << "\n";

    cout << "3. HOW TO USE:\n";
    cout << "   // Create from shared_ptr\n";
    cout << "   shared_ptr<T> shared = make_shared<T>();\n";
    cout << "   weak_ptr<T> weak = shared;\n";
    cout << "\n";
    cout << "   // Access through lock()\n";
    cout << "   if (auto locked = weak.lock()) {\n";
    cout << "       locked->method();  // Safe!\n";
    cout << "   }\n";
    cout << "\n";
    cout << "   // Check if expired\n";
    cout << "   if (weak.expired()) {\n";
    cout << "       // Object was destroyed\n";
    cout << "   }\n";
    cout << "\n";

    cout << "4. CIRCULAR REFERENCE SOLUTION:\n";
    cout << "   class Node {\n";
    cout << "       shared_ptr<Node> next;  // Ownership →\n";
    cout << "       weak_ptr<Node> prev;    // Observe ←\n";
    cout << "   };\n";
    cout << "   ✅ No memory leaks!\n";
    cout << "\n";

    cout << "5. BEST PRACTICES:\n";
    cout << "   ✅ ALWAYS use lock() before accessing\n";
    cout << "   ✅ Check if lock() succeeded\n";
    cout << "   ✅ Use weak_ptr for back-references\n";
    cout << "   ✅ Clean up expired weak_ptrs periodically\n";
    cout << "   ✅ Use weak_ptr in observer pattern\n";
    cout << "   ❌ DON'T use weak_ptr for ownership\n";
    cout << "   ❌ DON'T forget to check if expired\n";
    cout << "\n";

    cout << "6. DECISION GUIDE:\n";
    cout << "   Need to own object?\n";
    cout << "   YES → Use shared_ptr (or unique_ptr)\n";
    cout << "   NO → Need to observe?\n";
    cout << "        YES → Use weak_ptr ✅\n";
    cout << "        NO → Use raw pointer or reference\n";
    cout << "\n";

    return 0;
}

/*
COMPILATION:
    g++ -std=c++17 -Wall -Wextra weak_ptr.cpp -o weak_ptr

RUN:
    ./weak_ptr

TOPICS COVERED:
    ✅ Circular reference problem
    ✅ weak_ptr solution
    ✅ Basic operations (lock, expired, use_count)
    ✅ Tree structures (parent-child)
    ✅ Observer pattern
    ✅ Caching with expiration
    ✅ Safe callbacks
    ✅ Reference counting internals
    ✅ Common mistakes
    ✅ Best practices
    ✅ Performance considerations
    ✅ Real-world scenarios

KEY CONCEPTS:
    - weak_ptr observes without owning
    - Use lock() to access safely
    - Automatically detects object destruction
    - Perfect for breaking circular references
    - Essential for observer patterns
    - Enables automatic cache expiration

OUTPUT: Complete demonstrations with clear explanations!
*/
