# C++ weak_ptr - Complete Guide

## 📚 Table of Contents
- [What is weak_ptr?](#what-is-weak_ptr)
- [The Problem: Circular References](#the-problem-circular-references)
- [The Solution: weak_ptr](#the-solution-weak_ptr)
- [Basic Operations](#basic-operations)
- [Common Use Cases](#common-use-cases)
- [Parent-Child Relationships](#parent-child-relationships)
- [Observer Pattern](#observer-pattern)
- [Caching with weak_ptr](#caching-with-weak_ptr)
- [Reference Counting Internals](#reference-counting-internals)
- [Common Mistakes](#common-mistakes)
- [Best Practices](#best-practices)
- [Performance Analysis](#performance-analysis)
- [Decision Tree](#decision-tree)
- [Real-World Examples](#real-world-examples)

---

## What is weak_ptr?

**weak_ptr** is a smart pointer that **observes** but **does not own** a shared_ptr-managed object.

### Key Characteristics

```cpp
#include <memory>

std::shared_ptr<int> shared = std::make_shared<int>(42);
std::weak_ptr<int> weak = shared;  // Observe, don't own
```

| Feature | shared_ptr | weak_ptr |
|---------|------------|----------|
| **Ownership** | Yes (owns object) | No (observes only) |
| **Reference Count** | Increases | **Does NOT increase** ✅ |
| **Prevents Destruction** | Yes | **No** ✅ |
| **Direct Access** | Yes (`*ptr`) | **No** (must use `lock()`) |
| **Size** | 16 bytes | 16 bytes |
| **Main Purpose** | Ownership | **Break cycles** ✅ |

### The Big Picture

```
shared_ptr<T>                weak_ptr<T>
┌──────────────┐            ┌──────────────┐
│ ptr to       │            │ ptr to       │
│ Control Block├───────────►│ Control Block│
├──────────────┤            ├──────────────┤
│ ptr to Object│            │ ptr to Object│
└──────────────┘            └──────────────┘
     │                           │
     │ OWNS                      │ OBSERVES
     ▼                           ▼
┌──────────────┐            (same object)
│   Object     │
└──────────────┘

Control Block:
┌────────────────────────┐
│ Strong count (shared)  │ → When 0: destroy OBJECT
│ Weak count (weak)      │ → When 0: destroy CONTROL BLOCK
│ Deleter                │
│ Allocator              │
└────────────────────────┘
```

---

## The Problem: Circular References

### Memory Leak with shared_ptr

```cpp
// ❌ MEMORY LEAK!
class Node {
public:
    std::shared_ptr<Node> next;  // Node owns next
    std::shared_ptr<Node> prev;  // Node owns prev
};

auto node1 = std::make_shared<Node>();
auto node2 = std::make_shared<Node>();

node1->next = node2;  // node1 owns node2 (ref_count = 2)
node2->prev = node1;  // node2 owns node1 (ref_count = 2)

// When node1 and node2 go out of scope:
// - node1 destroyed? NO! node2 keeps it alive
// - node2 destroyed? NO! node1 keeps it alive
// - BOTH LEAK FOREVER! 💥
```

### The Circular Reference Diagram

```
Before connecting:
┌─────────┐         ┌─────────┐
│  main   │         │  main   │
│         │         │         │
│ node1 ──┼────────►│ Node1   │
└─────────┘         └─────────┘
ref_count: 1

After circular reference:
┌─────────┐         ┌─────────┐◄────┐
│  main   │         │ Node1   │     │
│         │         │         │     │
│ node1 ──┼────────►│ next ───┼─┐   │
└─────────┘         └─────────┘ │   │
ref_count: 2                    │   │
                                │   │
┌─────────┐         ┌─────────┐ │   │
│  main   │         │ Node2   │◄┘   │
│         │         │         │     │
│ node2 ──┼────────►│ prev ───┼─────┘
└─────────┘         └─────────┘
ref_count: 2

After main scope ends:
┌─────────┐◄────────┐
│ Node1   │         │
│         │         │
│ next ───┼─┐       │
└─────────┘ │       │
ref_count: 1│       │ CIRCULAR
            │       │ REFERENCE!
┌─────────┐ │       │ MEMORY
│ Node2   │◄┘       │ LEAK!
│         │         │
│ prev ───┼─────────┘
└─────────┘
ref_count: 1

Neither can be destroyed!
```

### Why This Happens

1. **node1** owns **node2** via `next` (shared_ptr)
2. **node2** owns **node1** via `prev` (shared_ptr)
3. When scope ends:
   - **node1** ref_count: 2 → 1 (main releases, but node2 still owns it)
   - **node2** ref_count: 2 → 1 (main releases, but node1 still owns it)
4. **Both objects leak!** Neither can reach ref_count = 0

---

## The Solution: weak_ptr

### Breaking the Cycle

```cpp
// ✅ NO LEAK!
class Node {
public:
    std::shared_ptr<Node> next;  // Ownership →
    std::weak_ptr<Node> prev;    // Observation ← (breaks cycle!)
};

auto node1 = std::make_shared<Node>();
auto node2 = std::make_shared<Node>();

node1->next = node2;  // node1 owns node2 (ref_count = 2)
node2->prev = node1;  // node2 observes node1 (ref_count STILL 1!)

// When node1 and node2 go out of scope:
// - node1 ref_count: 1 → 0 (DESTROYED!)
// - node2 ref_count: 2 → 1 → 0 (DESTROYED!)
// - SUCCESS! ✅
```

### Solution Diagram

```
After connecting with weak_ptr:
┌─────────┐         ┌─────────┐◄- - - -┐
│  main   │         │ Node1   │        │
│         │         │         │        │
│ node1 ──┼────────►│ next ───┼─┐      │
└─────────┘         └─────────┘ │      │
ref_count: 1                    │      │
                                │      │ weak_ptr
┌─────────┐         ┌─────────┐ │      │ (dashed line
│  main   │         │ Node2   │◄┘      │  = observe)
│         │         │         │        │
│ node2 ──┼────────►│ prev ···┼········┘
└─────────┘         └─────────┘
ref_count: 2

After main scope ends:
┌─────────┐
│ Node2   │  ← Only node2 survives temporarily
│         │    because node1->next owns it
│ next ───┼─┐
└─────────┘ │
ref_count: 1│
            │
            ▼
        (node1 destroyed)
        
Then node2 destroyed too!
✅ All memory cleaned up!
```

### Key Insight

**One-way ownership prevents cycles:**
- Parent → Child: **shared_ptr** (parent owns child)
- Child → Parent: **weak_ptr** (child observes parent)

---

## Basic Operations

### Creating weak_ptr

```cpp
// Create from shared_ptr
std::shared_ptr<int> shared = std::make_shared<int>(42);
std::weak_ptr<int> weak = shared;

// Note: ref_count NOT increased!
std::cout << shared.use_count();  // 1 (not 2!)
```

### Accessing: lock()

```cpp
std::weak_ptr<int> weak = shared;

// ✅ CORRECT: Use lock()
if (auto locked = weak.lock()) {
    // locked is a shared_ptr
    std::cout << *locked;  // Safe access
    // ref_count temporarily increased
} else {
    // Object was destroyed
}

// ❌ WRONG: Cannot access directly
// *weak;  // ERROR! Doesn't compile
```

### Checking: expired()

```cpp
std::weak_ptr<int> weak = shared;

// Check if object still exists
if (weak.expired()) {
    std::cout << "Object destroyed\n";
} else {
    std::cout << "Object still exists\n";
}

// Equivalent to:
if (weak.use_count() == 0) { /* expired */ }
```

### Getting Reference Count

```cpp
std::weak_ptr<int> weak = shared;

// Get strong reference count
std::cout << weak.use_count();  // Number of shared_ptrs

// Same as:
std::cout << shared.use_count();
```

### Resetting

```cpp
std::weak_ptr<int> weak = shared;

// Stop observing
weak.reset();

// Now:
weak.expired() == true
weak.lock() == nullptr
```

---

## Common Use Cases

### 1. Breaking Circular References

**Problem:** Two objects own each other
**Solution:** One owns, one observes

```cpp
class Node {
    std::shared_ptr<Node> next;  // Ownership
    std::weak_ptr<Node> prev;    // Observation
};
```

### 2. Parent-Child Relationships

**Problem:** Child needs to access parent
**Solution:** Parent owns children, children observe parent

```cpp
class TreeNode {
    std::weak_ptr<TreeNode> parent;           // Child → Parent
    std::vector<std::shared_ptr<TreeNode>> children;  // Parent → Children
};
```

### 3. Observer Pattern

**Problem:** Observers don't own subject
**Solution:** Subject tracked by observers using weak_ptr

```cpp
class Observer {
    std::weak_ptr<Subject> subject;  // Observe, don't own
    
    void notify() {
        if (auto s = subject.lock()) {
            s->getState();  // Safe!
        }
    }
};
```

### 4. Caching

**Problem:** Cache shouldn't keep unused objects alive
**Solution:** Cache stores weak_ptr

```cpp
class Cache {
    std::map<std::string, std::weak_ptr<Data>> cache;
    
    std::shared_ptr<Data> get(const std::string& key) {
        if (auto data = cache[key].lock()) {
            return data;  // Cache hit!
        }
        // Cache miss - reload
        auto data = load(key);
        cache[key] = data;
        return data;
    }
};
```

### 5. Callbacks

**Problem:** Callback target might be destroyed
**Solution:** Use weak_ptr for safe callback

```cpp
class EventHandler {
    std::weak_ptr<Target> target;
    
    void onEvent() {
        if (auto t = target.lock()) {
            t->handle();  // Safe!
        }
        // If target destroyed, no crash!
    }
};
```

---

## Parent-Child Relationships

### The Pattern

```cpp
class TreeNode : public std::enable_shared_from_this<TreeNode> {
public:
    std::weak_ptr<TreeNode> parent;           // ← observe parent
    std::vector<std::shared_ptr<TreeNode>> children;  // → own children
    
    void addChild(std::shared_ptr<TreeNode> child) {
        children.push_back(child);
        child->parent = shared_from_this();  // Set parent
    }
    
    void showParent() const {
        if (auto p = parent.lock()) {
            std::cout << "Parent: " << p->value << "\n";
        } else {
            std::cout << "No parent (root)\n";
        }
    }
};
```

### Why This Works

```
Tree Structure:
        Root
        /  \
    Child1  Child2
       |
   Grandchild

Ownership:
Root ───owns──→ Child1
Root ───owns──→ Child2
Child1 ──owns──→ Grandchild

Observation (weak_ptr):
Child1 ··observes··→ Root
Child2 ··observes··→ Root
Grandchild ··observes··→ Child1

✅ No cycles!
✅ Children destroyed when parent destroyed
✅ Children can access parent while it exists
```

### Destruction Order

```cpp
{
    auto root = std::make_shared<TreeNode>(1);
    auto child1 = std::make_shared<TreeNode>(2);
    root->addChild(child1);
    
    // Scope ends:
    // 1. root destroyed (ref_count → 0)
    // 2. child1 destroyed (owned by root)
    // 3. child1's weak_ptr to root already expired
}
```

---

## Observer Pattern

### Classic Implementation

```cpp
class Subject {
    std::vector<std::weak_ptr<Observer>> observers;
    
public:
    void attach(std::shared_ptr<Observer> obs) {
        observers.push_back(obs);
    }
    
    void notify() {
        // Remove expired observers while notifying
        observers.erase(
            std::remove_if(observers.begin(), observers.end(),
                [](const std::weak_ptr<Observer>& w) {
                    if (auto obs = w.lock()) {
                        obs->update();  // Notify
                        return false;   // Keep
                    }
                    return true;  // Remove expired
                }),
            observers.end()
        );
    }
};

class Observer {
    std::weak_ptr<Subject> subject;
    
public:
    Observer(std::shared_ptr<Subject> s) : subject(s) {
        s->attach(shared_from_this());
    }
    
    void update() {
        if (auto s = subject.lock()) {
            // Access subject state
        }
    }
};
```

### Benefits

✅ **Automatic cleanup:** Destroyed observers are automatically removed
✅ **No memory leaks:** Subject doesn't keep observers alive
✅ **Safe access:** Observers check if subject still exists
✅ **Decoupling:** Observer lifetime independent of subject

### Usage Pattern

```cpp
auto subject = std::make_shared<Subject>();

{
    auto obs1 = std::make_shared<Observer>(subject);
    auto obs2 = std::make_shared<Observer>(subject);
    
    subject->notify();  // Both observers notified
    
} // obs1 and obs2 destroyed

subject->notify();  // No observers (automatically cleaned up)
```

---

## Caching with weak_ptr

### The Problem

Traditional cache with shared_ptr:
```cpp
// ❌ BAD: Cache keeps everything alive forever
std::map<std::string, std::shared_ptr<Data>> cache;

auto data = cache["key"];
// Data never destroyed even if unused!
```

### The Solution

```cpp
// ✅ GOOD: Cache allows expiration
class Cache {
    std::map<std::string, std::weak_ptr<Data>> cache;
    
public:
    std::shared_ptr<Data> get(const std::string& key) {
        auto it = cache.find(key);
        
        if (it != cache.end()) {
            if (auto data = it->second.lock()) {
                return data;  // Cache HIT
            }
            cache.erase(it);  // Clean up expired entry
        }
        
        // Cache MISS - load data
        auto data = loadData(key);
        cache[key] = data;
        return data;
    }
    
    void cleanup() {
        // Remove all expired entries
        for (auto it = cache.begin(); it != cache.end();) {
            if (it->second.expired()) {
                it = cache.erase(it);
            } else {
                ++it;
            }
        }
    }
};
```

### How It Works

```
Timeline:

1. Client requests "data1"
   Cache loads data, returns shared_ptr
   cache["data1"] = weak_ptr to data
   
2. Client uses data1
   data1 has ref_count = 1 (client owns it)
   
3. Client done with data1
   shared_ptr destroyed, ref_count = 0
   Data object DESTROYED
   weak_ptr in cache becomes expired
   
4. Another client requests "data1"
   Cache checks: expired? YES
   Reload data (cache miss)
   
✅ Automatic memory management!
✅ No memory waste on unused data!
```

### Benefits

✅ **Automatic expiration:** Unused entries disappear
✅ **Memory efficient:** Only keeps data that's actively used
✅ **Performance:** Reuses data while it's "hot"
✅ **No manual cleanup:** Self-managing cache

---

## Reference Counting Internals

### Control Block Structure

```cpp
// When you create shared_ptr:
auto shared = std::make_shared<int>(42);

// Creates:
┌─────────────────────┐
│   Control Block     │
├─────────────────────┤
│ Strong count: 1     │ ← shared_ptr count
│ Weak count: 1       │ ← weak_ptr count + 1
│ Object: int(42)     │ ← The actual data
│ Deleter: default    │
│ Allocator: default  │
└─────────────────────┘
```

### Reference Counting Lifecycle

```cpp
// Step 1: Create shared_ptr
auto shared1 = std::make_shared<int>(42);
// strong_count = 1, weak_count = 1

// Step 2: Create weak_ptr
std::weak_ptr<int> weak1 = shared1;
// strong_count = 1 (unchanged!)
// weak_count = 2 (increased)

// Step 3: Create another shared_ptr
auto shared2 = shared1;
// strong_count = 2
// weak_count = 2

// Step 4: Destroy shared1
shared1.reset();
// strong_count = 1
// weak_count = 2

// Step 5: Destroy shared2
shared2.reset();
// strong_count = 0
// 💥 OBJECT DESTROYED!
// weak_count = 1 (control block survives!)
// weak1.expired() == true

// Step 6: Destroy weak1
weak1.reset();
// weak_count = 0
// 💥 CONTROL BLOCK DESTROYED!
```

### Key Insights

1. **Object destruction:** When `strong_count` reaches 0
2. **Control block destruction:** When `weak_count` reaches 0
3. **weak_count base:** Always starts at 1 (for the control block itself)
4. **Atomic operations:** All counts are thread-safe (atomic increments/decrements)

### Why Control Block Survives

```cpp
auto shared = std::make_shared<int>(42);
std::weak_ptr<int> weak = shared;

shared.reset();  // Object destroyed

// weak_ptr still needs to know object is gone!
// Control block keeps this information:
weak.expired() == true  // ← Control block answers this
weak.lock() == nullptr  // ← Control block answers this

weak.reset();  // Now control block can be destroyed
```

---

## Common Mistakes

### Mistake 1: Accessing Without lock()

```cpp
// ❌ WRONG: Cannot access directly
std::weak_ptr<int> weak = shared;
*weak;  // ERROR! Doesn't compile

// ✅ CORRECT: Use lock()
if (auto locked = weak.lock()) {
    *locked;  // Safe!
}
```

### Mistake 2: Not Checking lock() Result

```cpp
// ❌ WRONG: May crash
auto locked = weak.lock();
locked->method();  // Crash if nullptr!

// ✅ CORRECT: Always check
if (auto locked = weak.lock()) {
    locked->method();  // Safe!
}
```

### Mistake 3: Using weak_ptr for Ownership

```cpp
// ❌ WRONG: weak_ptr doesn't own
class Widget {
    std::weak_ptr<Resource> resource;  // Resource might be destroyed!
};

// ✅ CORRECT: Use shared_ptr for ownership
class Widget {
    std::shared_ptr<Resource> resource;  // Widget owns resource
};
```

### Mistake 4: Creating Circular References

```cpp
// ❌ WRONG: Both are shared_ptr (circular!)
class Node {
    std::shared_ptr<Node> parent;  // Circular reference!
    std::shared_ptr<Node> child;
};

// ✅ CORRECT: Break cycle with weak_ptr
class Node {
    std::weak_ptr<Node> parent;     // Observe parent
    std::shared_ptr<Node> child;    // Own child
};
```

### Mistake 5: Not Cleaning Up Expired weak_ptrs

```cpp
// ⚠️ SUBOPTIMAL: Expired weak_ptrs accumulate
std::vector<std::weak_ptr<Observer>> observers;
// Over time, many expired weak_ptrs remain

// ✅ BETTER: Periodic cleanup
void cleanupObservers() {
    observers.erase(
        std::remove_if(observers.begin(), observers.end(),
            [](const auto& w) { return w.expired(); }),
        observers.end()
    );
}
```

### Mistake 6: Race Conditions

```cpp
// ⚠️ POTENTIAL ISSUE: Check and use are separate
if (!weak.expired()) {
    // Object might be destroyed HERE (between check and use)!
    auto locked = weak.lock();  // May return nullptr
}

// ✅ CORRECT: Atomic check-and-lock
if (auto locked = weak.lock()) {
    // Safe! lock() is atomic
}
```

### Mistake 7: Creating weak_ptr from Raw Pointer

```cpp
// ❌ WRONG: Doesn't compile
int* raw = new int(42);
std::weak_ptr<int> weak(raw);  // ERROR!

// ✅ CORRECT: Create from shared_ptr
auto shared = std::make_shared<int>(42);
std::weak_ptr<int> weak = shared;
```

---

## Best Practices

### 1. Always Use lock() for Access

```cpp
// ✅ BEST PRACTICE
if (auto locked = weak.lock()) {
    locked->method();
} else {
    // Handle object destruction
}
```

### 2. Use weak_ptr for Back-References

```cpp
// ✅ BEST PRACTICE: One-way ownership
class Child {
    std::weak_ptr<Parent> parent;  // Back-reference
};

class Parent {
    std::vector<std::shared_ptr<Child>> children;  // Ownership
};
```

### 3. Clean Up Expired Entries

```cpp
// ✅ BEST PRACTICE: Periodic cleanup
void removeExpired() {
    container.erase(
        std::remove_if(container.begin(), container.end(),
            [](const auto& weak) { return weak.expired(); }),
        container.end()
    );
}
```

### 4. Use expired() for Simple Checks

```cpp
// ✅ BEST PRACTICE: Quick existence check
if (weak.expired()) {
    // Don't need to access, just check existence
}

// But if you need to access:
if (auto locked = weak.lock()) {
    // Use locked
}
```

### 5. Document Ownership Relationships

```cpp
// ✅ BEST PRACTICE: Clear comments
class Node {
    std::shared_ptr<Node> next;  // Owns next node
    std::weak_ptr<Node> prev;    // Observes previous node (breaks cycle)
};
```

### 6. Prefer weak_ptr Over Raw Pointers

```cpp
// ❌ RISKY: Raw pointer might dangle
class Observer {
    Subject* subject;  // Might be deleted!
};

// ✅ SAFE: weak_ptr detects deletion
class Observer {
    std::weak_ptr<Subject> subject;  // Automatically detects deletion
};
```

### 7. Use enable_shared_from_this

```cpp
// ✅ BEST PRACTICE: For getting weak_ptr to 'this'
class MyClass : public std::enable_shared_from_this<MyClass> {
public:
    void registerCallback() {
        // Safe way to get weak_ptr to this
        callback.setTarget(shared_from_this());
    }
};
```

### 8. Thread Safety Considerations

```cpp
// ✅ BEST PRACTICE: lock() is thread-safe
std::weak_ptr<Data> weak = shared;  // Shared between threads

// Thread-safe access:
if (auto locked = weak.lock()) {
    // Safe! lock() is atomic
    locked->method();
}
```

---

## Performance Analysis

### Memory Overhead

```cpp
sizeof(int*)                 = 8 bytes
sizeof(std::unique_ptr<int>) = 8 bytes
sizeof(std::shared_ptr<int>) = 16 bytes
sizeof(std::weak_ptr<int>)   = 16 bytes  ← Same as shared_ptr
```

**Both shared_ptr and weak_ptr store:**
- Pointer to control block (8 bytes)
- Pointer to object (8 bytes)

### Operation Costs

| Operation | Cost | Notes |
|-----------|------|-------|
| Creation | ~10ns | Atomic increment of weak_count |
| Copy | ~10ns | Atomic increment of weak_count |
| Move | ~1ns | Just copies pointers |
| lock() | ~20ns | Atomic check + increment if valid |
| expired() | ~5ns | Read strong_count |
| Destruction | ~10ns | Atomic decrement, may delete control block |

### Control Block Lifetime

```cpp
auto shared = std::make_shared<int>(42);
std::weak_ptr<int> weak = shared;

shared.reset();  // Object destroyed

// ⚠️ Control block still exists!
// Memory: sizeof(control block) ~= 32 bytes

weak.reset();  // Control block destroyed
```

**Implication:** Many expired weak_ptrs can keep control blocks alive
**Solution:** Periodically clean up expired weak_ptrs

### Compared to Alternatives

| Approach | Safety | Performance | Memory |
|----------|--------|-------------|--------|
| Raw pointer | ❌ | ✅ Fast | ✅ Small |
| shared_ptr | ⚠️ (cycles) | ⚠️ Moderate | ⚠️ Larger |
| weak_ptr | ✅ Safe | ⚠️ Moderate | ⚠️ Larger |
| Observer pattern | ✅ Safe | ✅ Fast | ✅ Small |

### When weak_ptr Overhead is Worth It

✅ **Use weak_ptr when:**
- Breaking circular references (prevents memory leaks!)
- Observer pattern (safety > performance)
- Caching (automatic expiration is valuable)
- Callbacks (crash prevention is critical)

⚠️ **Consider alternatives when:**
- Performance is critical and object lifetime is clear
- Objects have clear parent-child relationship (use references)
- Object guaranteed to outlive observer

---

## Decision Tree

```
Need to reference an object?
│
├─ Do you need to keep it alive?
│  │
│  ├─ YES → Need ownership
│  │  │
│  │  ├─ Single owner? → ✅ unique_ptr
│  │  └─ Multiple owners? → ✅ shared_ptr
│  │
│  └─ NO → Just observing
│     │
│     ├─ Lifetime guaranteed? → Use reference or raw pointer
│     │
│     └─ Lifetime uncertain? → ✅ weak_ptr
│
└─ Part of a cycle?
   │
   ├─ YES → ✅ weak_ptr (break cycle!)
   │
   └─ NO → See above
```

### Specific Scenarios

**Scenario: Parent-Child Tree**
```
Parent owns children? → shared_ptr
Child references parent? → weak_ptr ✅
```

**Scenario: Observer Pattern**
```
Subject owns observers? → shared_ptr (or store directly)
Observer references subject? → weak_ptr ✅
```

**Scenario: Caching**
```
Cache controls lifetime? → shared_ptr
Cache just tracks? → weak_ptr ✅
```

**Scenario: Callbacks**
```
Callback owns target? → shared_ptr
Callback might outlive target? → weak_ptr ✅
```

---

## Real-World Examples

### Example 1: Game Entity System

```cpp
class Entity {
    std::weak_ptr<Entity> target;  // AI target
    
public:
    void update() {
        if (auto t = target.lock()) {
            // Target still exists
            aimAt(t->getPosition());
        } else {
            // Target was destroyed - find new target
            findNewTarget();
        }
    }
};
```

**Why weak_ptr:**
- Entities can be destroyed at any time
- AI shouldn't keep dead entities alive
- Automatic detection of target destruction

### Example 2: UI Framework

```cpp
class Widget {
    std::weak_ptr<Widget> parent;           // Child → Parent
    std::vector<std::shared_ptr<Widget>> children;  // Parent → Children
    
public:
    void handleEvent(Event e) {
        if (auto p = parent.lock()) {
            p->bubbleEvent(e);  // Event bubbling
        }
    }
};
```

**Why weak_ptr:**
- Prevents parent-child circular references
- Parent can destroy children
- Children can access parent while it exists

### Example 3: Resource Pool

```cpp
class TexturePool {
    std::map<std::string, std::weak_ptr<Texture>> textures;
    
public:
    std::shared_ptr<Texture> getTexture(const std::string& name) {
        if (auto tex = textures[name].lock()) {
            return tex;  // Reuse existing
        }
        // Load new texture
        auto tex = loadTexture(name);
        textures[name] = tex;
        return tex;
    }
};
```

**Why weak_ptr:**
- Reuse textures while in use
- Automatic cleanup when unused
- No memory waste on idle resources

### Example 4: Document References

```cpp
class Document {
    std::vector<std::shared_ptr<Paragraph>> paragraphs;
};

class Bookmark {
    std::weak_ptr<Paragraph> target;
    
public:
    void jump() {
        if (auto para = target.lock()) {
            scrollTo(para);
        } else {
            showError("Target paragraph was deleted");
        }
    }
};
```

**Why weak_ptr:**
- Bookmarks don't prevent paragraph deletion
- Bookmarks automatically become invalid
- No memory leaks

### Example 5: Network Connections

```cpp
class ConnectionPool {
    std::vector<std::weak_ptr<Connection>> connections;
    
public:
    void cleanup() {
        // Remove closed connections
        connections.erase(
            std::remove_if(connections.begin(), connections.end(),
                [](const auto& w) { return w.expired(); }),
            connections.end()
        );
    }
};
```

**Why weak_ptr:**
- Pool tracks but doesn't own connections
- Connections can be closed independently
- Automatic cleanup of dead connections

---

## Quick Reference

### Creation

```cpp
std::shared_ptr<T> shared = std::make_shared<T>(args);
std::weak_ptr<T> weak = shared;  // From shared_ptr
std::weak_ptr<T> weak2 = weak;   // Copy weak_ptr
```

### Access

```cpp
// Lock (returns shared_ptr or nullptr)
if (auto locked = weak.lock()) {
    locked->method();
}

// Check if expired
if (weak.expired()) {
    // Object destroyed
}

// Get reference count
int count = weak.use_count();
```

### Modification

```cpp
weak.reset();  // Stop observing
weak = other;  // Observe different object
```

### Common Patterns

```cpp
// Parent-Child
class Node {
    std::weak_ptr<Node> parent;
    std::vector<std::shared_ptr<Node>> children;
};

// Observer
class Observer {
    std::weak_ptr<Subject> subject;
};

// Cache
std::map<Key, std::weak_ptr<Value>> cache;
```

---

## Summary

### Key Concepts

**1. weak_ptr is a non-owning observer**
- Does NOT increase reference count
- Does NOT prevent destruction
- Main use: Breaking circular references

**2. Always use lock() to access**
```cpp
if (auto locked = weak.lock()) {
    locked->method();  // Safe!
}
```

**3. Common use cases:**
- Breaking circular references
- Parent-child relationships
- Observer pattern
- Caching with expiration
- Safe callbacks

**4. Best practices:**
- Use weak_ptr for back-references
- Check lock() before accessing
- Clean up expired entries periodically
- Document ownership relationships

### The Golden Rules

```
1. weak_ptr for observation, shared_ptr for ownership
2. Always use lock() before accessing
3. Check if lock() succeeded (may return nullptr)
4. Use weak_ptr to break circular references
5. One-way ownership prevents cycles
6. Clean up expired weak_ptrs periodically
```

### When to Use weak_ptr

```
✅ Use weak_ptr when:
  - Breaking circular references
  - Observer pattern
  - Caching (auto-expiration)
  - Callbacks (safe deletion)
  - Parent-child (child → parent)

❌ Don't use weak_ptr when:
  - You need ownership (use shared_ptr)
  - Performance critical (consider alternatives)
  - Lifetime guaranteed (use reference)
```

---

## Further Reading

- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
  - R.24: Use weak_ptr to break cycles
- [Effective Modern C++](https://www.oreilly.com/library/view/effective-modern-c/9781491908419/) by Scott Meyers
  - Item 20: Use weak_ptr for shared_ptr-like pointers that can dangle
- [cppreference.com - weak_ptr](https://en.cppreference.com/w/cpp/memory/weak_ptr)

---

## Compilation

```bash
# Compile with C++17 or later
g++ -std=c++17 -Wall -Wextra weak_ptr.cpp -o weak_ptr

# Run
./weak_ptr
```

---

**Happy Coding!** 🚀

*Remember: weak_ptr is your friend for breaking cycles and safe observation!*