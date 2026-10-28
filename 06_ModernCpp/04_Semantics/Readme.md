# Move Semantics

## 📚 Table of Contents
- [What is Move Semantics?](#what-is-move-semantics)
- [Core Concepts](#core-concepts)
- [Why Move Semantics?](#why-move-semantics)
- [The Problem: Expensive Copies](#the-problem-expensive-copies)
- [The Solution: Move Semantics](#the-solution-move-semantics)
- [How It Works](#how-it-works)
- [Move Operations](#move-operations)
- [std::move Explained](#stdmove-explained)
- [Code Examples](#code-examples)
- [Automatic Move](#automatic-move)
- [Move-Only Types](#move-only-types)
- [Perfect Forwarding](#perfect-forwarding)
- [Rule of Five](#rule-of-five)
- [Rule of Zero](#rule-of-zero)
- [Performance Comparison](#performance-comparison)
- [Common Mistakes](#common-mistakes)
- [Best Practices](#best-practices)
- [Real-World Impact](#real-world-impact)
- [Key Takeaways](#key-takeaways)

---

## What is Move Semantics?

**Move Semantics** is a C++11 feature that enables:
- **Transfer of ownership** instead of copying
- **"Stealing" resources** from temporary objects
- **Massive performance improvements**
- **Zero-cost abstractions**

### In Simple Terms:
```
Copy:  Make a duplicate of everything (expensive) 📋➡️📋
Move:  Transfer ownership of resources (cheap)   📋➡️🗑️ ⚡
```

### Introduced in:
- **C++11** (2011)
- One of the most important C++ features ever added
- Revolutionized the language

---

## Core Concepts

### Lvalues and Rvalues

#### **Lvalues** (Left values):
- Named objects with **persistent storage**
- Can appear on the **left** side of assignment
- Can take address with `&`
- **Persist** beyond a single expression
```cpp
int x = 10;        // x is lvalue (has name, persistent)
int* ptr = &x;     // ✅ Can take address of lvalue
x = 20;            // ✅ Can appear on left of =
```

#### **Rvalues** (Right values):
- **Temporary** objects
- Can only appear on the **right** side of assignment
- Cannot take address
- **Don't persist** beyond expression
```cpp
int y = x + 5;     // (x + 5) is rvalue (temporary)
// int* ptr = &(x + 5);  ❌ ERROR: Can't take address of rvalue

int z = 42;        // 42 is rvalue (literal)
string s = "Hi";   // "Hi" is rvalue (temporary)
```

### Value Categories (Simplified):
```
        Expression
            |
      ┌─────┴─────┐
      |           |
   lvalue      rvalue
   (named)   (temporary)
   
Examples:
  int x = 5;           // x = lvalue, 5 = rvalue
  string s = "Hello";  // s = lvalue, "Hello" = rvalue
  vector<int> v;       // v = lvalue
  v.size()            // Returns rvalue (temporary)
```

### Rvalue Reference (`&&`):
```cpp
int x = 10;           // x is lvalue
int& lref = x;        // Lvalue reference (binds to lvalue)

int&& rref = 20;      // Rvalue reference (binds to rvalue)
int&& rref2 = x + 5;  // Binds to temporary

// int&& bad = x;     // ❌ Can't bind rvalue ref to lvalue
int&& good = std::move(x);  // ✅ std::move casts to rvalue
```

---

## Why Move Semantics?

### The Motivation:
Before C++11, returning large objects was **expensive**:
```cpp
vector<int> createVector() {
    vector<int> temp;
    // Fill with 1 million elements...
    return temp;  // 💰 Expensive copy in C++03!
}

vector<int> v = createVector();  // 💰 Another copy!
// Total: Copied 2 million integers!
```

### With Move Semantics (C++11+):
```cpp
vector<int> createVector() {
    vector<int> temp;
    // Fill with 1 million elements...
    return temp;  // ⚡ Move (or RVO) - instant!
}

vector<int> v = createVector();  // ⚡ Move - instant!
// Total: Just pointer swaps!
```

### Benefits:
- ✅ **100-1000x faster** for large objects
- ✅ **Enables move-only types** (`unique_ptr`)
- ✅ **Natural return-by-value** code
- ✅ **Zero overhead abstractions**

---

## The Problem: Expensive Copies

### Example: Heavy Resource Class
```cpp
class HeavyResource {
private:
    int* data;
    size_t size;
    
public:
    // Constructor
    HeavyResource(size_t s) : size(s) {
        data = new int[size];
        // Initialize data...
    }
    
    // Copy Constructor - EXPENSIVE! 💰
    HeavyResource(const HeavyResource& other) : size(other.size) {
        cout << "COPYING " << size << " elements (EXPENSIVE!)\n";
        data = new int[size];
        // Copy every single element!
        for (size_t i = 0; i < size; i++) {
            data[i] = other.data[i];  // 💰 Expensive!
        }
    }
    
    // Destructor
    ~HeavyResource() {
        delete[] data;
    }
};

// Usage:
HeavyResource createResource() {
    HeavyResource temp(1000000);  // 1 million elements
    return temp;  // 💰 Copy all 1 million! (C++03)
}

HeavyResource r = createResource();  // 💰 Another copy!
// Total: Copied 2 million integers!
```

### The Cost:
- Memory allocation: 2 times
- Element copying: 2 million integers
- Time: **~100ms** (depends on size)

---

## The Solution: Move Semantics

### Add Move Constructor:
```cpp
class OptimizedResource {
private:
    int* data;
    size_t size;
    
public:
    // Constructor
    OptimizedResource(size_t s) : size(s) {
        data = new int[size];
    }
    
    // Copy Constructor - Still available when needed
    OptimizedResource(const OptimizedResource& other) : size(other.size) {
        cout << "COPYING " << size << " elements\n";
        data = new int[size];
        for (size_t i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }
    
    // Move Constructor - CHEAP! ⚡
    OptimizedResource(OptimizedResource&& other) noexcept 
        : data(other.data), size(other.size) {
        cout << "MOVING " << size << " elements (CHEAP!)\n";
        
        // "Steal" the resource
        other.data = nullptr;
        other.size = 0;
    }
    
    // Destructor
    ~OptimizedResource() {
        delete[] data;
    }
};

// Usage:
OptimizedResource createResource() {
    OptimizedResource temp(1000000);
    return temp;  // ⚡ Move - just pointer swap!
}

OptimizedResource r = createResource();  // ⚡ Move!
// Total: Just 3 pointer operations!
```

### The Benefit:
- Memory allocation: 1 time (reused)
- Element copying: 0 (just pointer swap!)
- Time: **~0.001ms** (1000x faster!)

---

## How It Works

### Visual Comparison:

#### Copy (Deep Copy):
```
Before:
┌─────────┐           ┌─────────┐
│ Object1 │           │ Object2 │
│  data ──┼──► [1,2,3]│  data = │
└─────────┘           └─────────┘

After Copy:
┌─────────┐           ┌─────────┐
│ Object1 │           │ Object2 │
│  data ──┼──► [1,2,3]│  data ──┼──► [1,2,3] (NEW!)
└─────────┘           └─────────┘
              
Cost: Allocate + Copy all elements
```

#### Move (Steal Resources):
```
Before:
┌─────────┐           ┌─────────┐
│ Object1 │           │ Object2 │
│  data ──┼──► [1,2,3]│  data = │
└─────────┘           └─────────┘

After Move:
┌─────────┐           ┌─────────┐
│ Object1 │           │ Object2 │
│ data = ø│           │  data ──┼──► [1,2,3] (STOLEN!)
└─────────┘           └─────────┘

Cost: Just 2 pointer assignments!
```

### The Move Process:
1. **Steal** source's resources (pointers, handles)
2. **Nullify** source object (leave in valid state)
3. **Return** (no allocation or copying!)

---

## Move Operations

### Move Constructor
```cpp
class MyClass {
public:
    // Move Constructor
    MyClass(MyClass&& other) noexcept {
        // Steal resources from 'other'
        data = other.data;
        size = other.size;
        
        // Leave 'other' in valid but empty state
        other.data = nullptr;
        other.size = 0;
    }
    
private:
    int* data;
    size_t size;
};
```

**Key Points:**
- Parameter: `MyClass&&` (rvalue reference)
- Should be `noexcept` (very important!)
- Steals resources from source
- Leaves source in **valid but unspecified** state

### Move Assignment Operator
```cpp
class MyClass {
public:
    // Move Assignment Operator
    MyClass& operator=(MyClass&& other) noexcept {
        if (this != &other) {
            // Free current resources
            delete[] data;
            
            // Steal from other
            data = other.data;
            size = other.size;
            
            // Leave other in valid state
            other.data = nullptr;
            other.size = 0;
        }
        return *this;
    }
    
private:
    int* data;
    size_t size;
};
```

**Key Points:**
- Same as move constructor, but:
  - Free current resources first
  - Check for self-assignment
  - Return `*this`

---

## std::move Explained

### What is `std::move`?
```cpp
template<typename T>
typename remove_reference<T>::type&& move(T&& arg) noexcept {
    return static_cast<typename remove_reference<T>::type&&>(arg);
}
```

**Important:** `std::move` **doesn't actually move anything!**

### What it Does:
- **Casts** an lvalue to an rvalue reference
- **Signals** that the object can be moved from
- **Enables** move constructor/assignment to be called

### Usage:
```cpp
vector<string> vec;

string s1 = "Hello";
vec.push_back(s1);           // COPY (s1 is lvalue)

string s2 = "World";
vec.push_back(std::move(s2)); // MOVE (cast to rvalue)

// After std::move:
// - s2 is in "moved-from" state
// - s2 is valid but empty (implementation-defined)
// - Don't use s2 except to assign or destroy
```

### Visualization:
```cpp
MyClass obj1;                    // obj1 is lvalue

MyClass obj2 = obj1;             // Copy (obj1 is lvalue)
MyClass obj3 = std::move(obj1);  // Move (cast to rvalue)

// After std::move(obj1):
// - obj1 is "moved-from" 
// - obj1 is valid but empty
// - Safe operations: assign new value, destroy
// - Unsafe: use its value
```

---

## Code Examples

### Example 1: Simple Move Class
```cpp
class SimpleResource {
private:
    string name;
    int* value;
    
public:
    // Constructor
    SimpleResource(const string& n, int v) 
        : name(n), value(new int(v)) {
        cout << "Created: " << name << "\n";
    }
    
    // Destructor
    ~SimpleResource() {
        cout << "Destroyed: " << name;
        if (value) {
            cout << " (value=" << *value << ")\n";
            delete value;
        } else {
            cout << " (moved-from)\n";
        }
    }
    
    // Copy Constructor
    SimpleResource(const SimpleResource& other) 
        : name(other.name + "_copy"), value(new int(*other.value)) {
        cout << "Copied: " << name << "\n";
    }
    
    // Move Constructor
    SimpleResource(SimpleResource&& other) noexcept 
        : name(std::move(other.name)), value(other.value) {
        cout << "Moved: " << name << "\n";
        other.value = nullptr;
    }
    
    // Copy Assignment
    SimpleResource& operator=(const SimpleResource& other) {
        if (this != &other) {
            delete value;
            name = other.name + "_copy";
            value = new int(*other.value);
            cout << "Copy-assigned: " << name << "\n";
        }
        return *this;
    }
    
    // Move Assignment
    SimpleResource& operator=(SimpleResource&& other) noexcept {
        if (this != &other) {
            delete value;
            name = std::move(other.name);
            value = other.value;
            other.value = nullptr;
            cout << "Move-assigned: " << name << "\n";
        }
        return *this;
    }
};
```

### Example 2: Using with Containers
```cpp
void containerExample() {
    vector<SimpleResource> vec;
    
    // Method 1: Copy (expensive)
    SimpleResource r1("Resource1", 100);
    vec.push_back(r1);  // COPY
    
    // Method 2: Move (efficient)
    SimpleResource r2("Resource2", 200);
    vec.push_back(std::move(r2));  // MOVE
    
    // Method 3: Temporary (automatic move)
    vec.push_back(SimpleResource("Resource3", 300));  // MOVE
    
    // Method 4: emplace_back (best - construct in place)
    vec.emplace_back("Resource4", 400);  // No move or copy!
}
```

### Example 3: Function Returns
```cpp
// Good: Return by value
SimpleResource createResource() {
    SimpleResource local("Local", 42);
    return local;  // Automatically moved (or RVO)
}

// Usage:
SimpleResource r = createResource();  // Move or RVO
```

---

## Automatic Move

### When Moves Happen Automatically:

#### 1. Returning Local Variables:
```cpp
MyClass func() {
    MyClass local;
    return local;  // ⚡ Automatically moved (or RVO)
}
```

#### 2. Passing Temporaries:
```cpp
void process(MyClass obj) { }

process(MyClass());  // ⚡ Temporary - automatically moved
```

#### 3. Initializing from Rvalues:
```cpp
MyClass obj1 = MyClass();        // ⚡ Auto move
MyClass obj2 = createObject();   // ⚡ Auto move
```

### Return Value Optimization (RVO):

**RVO** = Compiler optimization that eliminates copy/move entirely!
```cpp
MyClass func() {
    MyClass local;
    return local;  // Compiler might optimize away the move!
}

MyClass obj = func();  // Might be constructed directly in obj
                       // No copy, no move - even better!
```

**Important:** Never use `std::move` on return values!
```cpp
// ❌ BAD - Prevents RVO
MyClass func() {
    MyClass local;
    return std::move(local);  // Don't do this!
}

// ✅ GOOD - Enables RVO
MyClass func() {
    MyClass local;
    return local;  // Correct!
}
```

---

## Move-Only Types

### What are Move-Only Types?

Types that **cannot be copied**, only **moved**:
- `unique_ptr<T>` - Exclusive ownership
- `thread` - Thread handles
- `ifstream/ofstream` - File streams
- `mutex` - Mutex locks
- `atomic<T>` - Atomic operations

### Creating Move-Only Types:
```cpp
class MoveOnlyResource {
private:
    unique_ptr<int[]> data;
    size_t size;
    
public:
    // Constructor
    MoveOnlyResource(size_t s) 
        : data(make_unique<int[]>(s)), size(s) {
    }
    
    // Delete copy operations
    MoveOnlyResource(const MoveOnlyResource&) = delete;
    MoveOnlyResource& operator=(const MoveOnlyResource&) = delete;
    
    // Move operations (default or custom)
    MoveOnlyResource(MoveOnlyResource&&) noexcept = default;
    MoveOnlyResource& operator=(MoveOnlyResource&&) noexcept = default;
};
```

### Usage:
```cpp
MoveOnlyResource r1(100);

// MoveOnlyResource r2 = r1;  // ❌ Error: copy deleted

MoveOnlyResource r2 = std::move(r1);  // ✅ Move works

// r1 is now in moved-from state
```

### Why Move-Only?

Represents **exclusive ownership** or **unique resources**:
- Only one owner at a time
- Transfer ownership explicitly with `std::move`
- Cannot accidentally duplicate

---

## Perfect Forwarding

### Universal References (`T&&`)

In **template context**, `T&&` is a **universal/forwarding reference**:
- Binds to **both** lvalues and rvalues
- Different from regular rvalue reference!
```cpp
template<typename T>
void wrapper(T&& arg) {  // Universal reference!
    // T&& can bind to lvalue or rvalue
    process(std::forward<T>(arg));
}

int x = 42;
wrapper(x);       // T = int&,  arg is lvalue ref
wrapper(42);      // T = int,   arg is rvalue ref
wrapper(std::move(x));  // T = int, arg is rvalue ref
```

### `std::forward`

Preserves the **value category** (lvalue/rvalue):
```cpp
void process(int& x) { 
    cout << "Lvalue\n"; 
}

void process(int&& x) { 
    cout << "Rvalue\n"; 
}

template<typename T>
void wrapper(T&& arg) {
    // Without forward - always calls lvalue version
    // process(arg);  // arg has a name, so it's lvalue!
    
    // With forward - preserves original category
    process(std::forward<T>(arg));  // ✅ Correct!
}

int x = 10;
wrapper(x);           // Calls process(int&)
wrapper(10);          // Calls process(int&&)
wrapper(std::move(x)); // Calls process(int&&)
```

### Perfect Forwarding Pattern:
```cpp
template<typename T, typename... Args>
shared_ptr<T> make_shared_custom(Args&&... args) {
    // Perfect forwarding to T's constructor
    return shared_ptr<T>(new T(std::forward<Args>(args)...));
}

// Usage:
auto ptr = make_shared_custom<MyClass>(arg1, arg2, arg3);
// Arguments forwarded with exact value category preserved
```

---

## Rule of Five

### The Rule:

**If you define ANY of these 5, define ALL 5:**
1. Destructor
2. Copy Constructor
3. Copy Assignment Operator
4. Move Constructor
5. Move Assignment Operator

### Why?

If you need custom behavior for one, you likely need it for all.

### Complete Implementation:
```cpp
class Resource {
private:
    int* data;
    
public:
    // Constructor
    Resource(int value) : data(new int(value)) { }
    
    // 1. Destructor
    ~Resource() {
        delete data;
    }
    
    // 2. Copy Constructor
    Resource(const Resource& other) 
        : data(new int(*other.data)) {
    }
    
    // 3. Copy Assignment
    Resource& operator=(const Resource& other) {
        if (this != &other) {
            delete data;
            data = new int(*other.data);
        }
        return *this;
    }
    
    // 4. Move Constructor
    Resource(Resource&& other) noexcept 
        : data(other.data) {
        other.data = nullptr;
    }
    
    // 5. Move Assignment
    Resource& operator=(Resource&& other) noexcept {
        if (this != &other) {
            delete data;
            data = other.data;
            other.data = nullptr;
        }
        return *this;
    }
};
```

### When to Use:

- Managing raw resources (`new`/`delete`)
- Custom memory management
- Wrapping C APIs
- Performance-critical code

---

## Rule of Zero

### The Better Rule: **Don't define ANY of them!**

Use RAII types instead:
```cpp
class ModernResource {
private:
    unique_ptr<int> data;      // RAII!
    vector<int> numbers;       // RAII!
    string name;               // RAII!
    
public:
    ModernResource(int value, const string& n) 
        : data(make_unique<int>(value)), name(n) {
    }
    
    // No need to define ANY special members!
    // Compiler generates correct versions automatically!
    
    // Automatically generated:
    // ~ModernResource()                           = default;
    // ModernResource(const ModernResource&)       = default;
    // ModernResource& operator=(const ModernResource&) = default;
    // ModernResource(ModernResource&&)            = default;
    // ModernResource& operator=(ModernResource&&) = default;
};
```

### Benefits:

- ✅ **Less code** to write
- ✅ **Less code** to maintain
- ✅ **Compiler-optimized**
- ✅ **Correct by default**
- ✅ **Harder to make mistakes**

### Prefer This Order:

1. **Rule of Zero** (best - use RAII types)
2. Rule of Five (if you must manage raw resources)
3. Delete operations (if move/copy makes no sense)

---

## Performance Comparison

### Benchmark Example:
```cpp
// Test with 1 million elements
const size_t SIZE = 1000000;

// Copy Performance:
HeavyResource r1(SIZE);
auto start = high_resolution_clock::now();
HeavyResource r2 = r1;  // COPY
auto end = high_resolution_clock::now();
// Time: ~100ms

// Move Performance:
HeavyResource r3(SIZE);
start = high_resolution_clock::now();
HeavyResource r4 = std::move(r3);  // MOVE
end = high_resolution_clock::now();
// Time: ~0.001ms

// Speedup: 100,000x faster! 🚀
```

### Complexity Comparison:

| Operation | Copy | Move | RVO |
|-----------|------|------|-----|
| **Complexity** | O(n) | O(1) | O(0) |
| **Allocations** | New | Reuse | None |
| **Elements copied** | All | None | None |
| **Time (1M elements)** | ~100ms | ~0.001ms | 0ms |

### Real-World Impact:
```cpp
// Before C++11 (Copy):
vector<string> loadFile() {
    vector<string> lines;
    // Read 1 million lines...
    return lines;  // 💰 Copy 1 million strings! (~500ms)
}

// After C++11 (Move):
vector<string> loadFile() {
    vector<string> lines;
    // Read 1 million lines...
    return lines;  // ⚡ Move - instant! (~0.001ms)
}

// 500,000x speedup! 🚀
```

---

## Common Mistakes

### ❌ Mistake 1: Using Moved-From Objects
```cpp
string s1 = "Hello";
string s2 = std::move(s1);

cout << s1;  // ⚠️ Undefined behavior! s1 is moved-from
```

**Fix:** Don't use moved-from objects (except to assign or destroy)
```cpp
string s1 = "Hello";
string s2 = std::move(s1);

s1 = "New value";  // ✅ OK - Assigning new value
cout << s1;        // ✅ OK now
```

### ❌ Mistake 2: Moving Const Objects
```cpp
const MyClass obj1;
MyClass obj2 = std::move(obj1);  // ⚠️ Copies, doesn't move!
```

**Why:** Can't steal from `const` objects!

**Fix:** Don't `std::move` const objects

### ❌ Mistake 3: std::move on Return
```cpp
// ❌ BAD - Prevents RVO
MyClass func() {
    MyClass local;
    return std::move(local);  // DON'T!
}

// ✅ GOOD - Enables RVO
MyClass func() {
    MyClass local;
    return local;  // Correct!
}
```

### ❌ Mistake 4: Forgetting noexcept
```cpp
// ⚠️ Might not be used by STL containers!
MyClass(MyClass&& other) {
    // ...
}

// ✅ Correct - Enables optimizations
MyClass(MyClass&& other) noexcept {
    // ...
}
```

**Why:** STL containers use move only if `noexcept`

### ❌ Mistake 5: Moving Temporaries
```cpp
// ❌ Redundant - already an rvalue
vec.push_back(std::move(MyClass()));

// ✅ Correct - temporary is already rvalue
vec.push_back(MyClass());
```

### ❌ Mistake 6: Not Leaving Moved-From State Valid
```cpp
// ❌ BAD - Left in invalid state
MyClass(MyClass&& other) {
    data = other.data;
    // Forgot: other.data = nullptr;
}  // 💥 Double-delete when destructors run!

// ✅ GOOD - Valid moved-from state
MyClass(MyClass&& other) noexcept {
    data = other.data;
    other.data = nullptr;  // ✅ Valid state
}
```

---

## Best Practices

### 1. Always Mark Move Operations `noexcept`
```cpp
MyClass(MyClass&&) noexcept;
MyClass& operator=(MyClass&&) noexcept;
```

**Why:** Enables STL container optimizations

### 2. Follow Rule of Zero
```cpp
class Good {
    unique_ptr<int> data;   // Use RAII types
    vector<int> numbers;
    string name;
    // No need to define any special members!
};
```

### 3. Use `std::move` Explicitly for Lvalues
```cpp
MyClass obj1;
MyClass obj2 = std::move(obj1);  // ✅ Explicit intent
```

### 4. Never `std::move` Return Values
```cpp
return local;              // ✅ Correct
return std::move(local);   // ❌ Wrong - Prevents RVO
```

### 5. Don't Use Moved-From Objects
```cpp
auto obj2 = std::move(obj1);
// obj1 is now moved-from
// ✅ OK: obj1 = newValue;
// ✅ OK: obj1.~MyClass();
// ❌ NOT OK: obj1.doSomething();
```

### 6. Prefer `emplace_back` Over `push_back`
```cpp
vec.emplace_back(args...);  // ✅ Best - construct in place
vec.push_back(MyClass());   // Good - move
vec.push_back(obj);         // OK - copy
```

### 7. Document Move-Only Types
```cpp
/// Move-only type representing exclusive ownership
class MoveOnly {
    MoveOnly(const MoveOnly&) = delete;  // Explicitly deleted
};
```

### 8. Test Moved-From State
```cpp
// Ensure moved-from objects are in valid state
MyClass obj1;
MyClass obj2 = std::move(obj1);
obj1 = MyClass();  // Should work without issues
```

### 9. Use Perfect Forwarding for Generic Code
```cpp
template<typename T>
void wrapper(T&& arg) {
    process(std::forward<T>(arg));  // ✅ Preserves value category
}
```

### 10. Profile Before Optimizing
```cpp
// Measure actual performance benefit
// Sometimes copy is fast enough
// Don't optimize prematurely
```

---

## Real-World Impact

### Before Move Semantics (C++03):

**Problems:**
- Returning large objects was expensive
- Had to use output parameters
- Awkward code patterns
- Performance sacrifices
```cpp
// C++03 - Awkward!
void getData(vector<int>& out) {  // Output parameter
    out = vector<int>(1000000);   // Still copies!
}

// Or use pointer
vector<int>* getData() {
    return new vector<int>(1000000);  // Manual memory management
}
```

### After Move Semantics (C++11+):

**Solutions:**
- Natural return-by-value
- Zero-cost abstractions
- Clean, readable code
- Massive performance gains
```cpp
// C++11+ - Natural!
vector<int> getData() {
    return vector<int>(1000000);  // ⚡ Move - instant!
}

vector<int> data = getData();  // Clean and fast!
```

### Performance Wins:
```cpp
// String operations:
string s = "Hello, " + "World" + "!";  // Multiple moves - nearly free

// Container operations:
vector<string> v1 = loadStrings();     // Move - instant
vector<vector<int>> v2 = loadVectors(); // Move - instant

// Factory functions:
unique_ptr<Heavy> obj = Factory::create();  // Move - instant
```

### Enabled Features:

Move Semantics made possible:
- ✅ `unique_ptr` - Exclusive ownership
- ✅ Move-only types
- ✅ Efficient standard library
- ✅ Modern C++ style
- ✅ Zero-cost abstractions

---

## Key Takeaways

### 🎯 Core Concepts:

1. **Move = Transfer ownership** (cheap)
2. **Copy = Duplicate resources** (expensive)
3. **`std::move` = Cast to rvalue** (enables move)
4. **Moved-from = Valid but empty** (don't use)
5. **`noexcept` = Required** (for optimization)

### 📊 Value Categories:
```
Lvalue:  Named objects, persistent
Rvalue:  Temporaries, can be stolen from
std::move: Cast lvalue to rvalue
```

### ⚡ Performance:
```
Copy:     O(n) - Expensive
Move:     O(1) - Cheap
RVO:      O(0) - Free (best!)
```

### 🔧 Operations:
```cpp
MyClass(MyClass&& other) noexcept;        // Move constructor
MyClass& operator=(MyClass&&) noexcept;   // Move assignment
std::move(obj);                           // Cast to rvalue
std::forward<T>(arg);                     // Perfect forwarding
```

### 📚 Best Practices:

1. Follow **Rule of Zero** (use RAII types)
2. Mark moves **`noexcept`**
3. **Never** `std::move` return values
4. **Don't use** moved-from objects
5. Prefer **`emplace_back`** over `push_back`

### 🚀 Impact:

Move Semantics **revolutionized** C++:
- **100-1000x** faster for large objects
- Enabled **modern C++ style**
- Made **zero-cost abstractions** real
- **Natural** return-by-value code

---

## Summary Table

| Aspect | Copy | Move | RVO |
|--------|------|------|-----|
| **Cost** | Expensive | Cheap | Free |
| **Allocations** | New memory | Reuse existing | None |
| **Complexity** | O(n) | O(1) | O(0) |
| **Use Case** | Need both | Done with source | Automatic |
| **Syntax** | `obj2 = obj1` | `obj2 = std::move(obj1)` | `return local` |
| **Result** | Two valid objects | One valid, one moved-from | One valid object |

---

## Further Reading

### C++ Standard:
- **C++11**: Introduced Move Semantics
- **C++14**: Refinements
- **C++17**: Guaranteed copy elision
- **C++20**: Further improvements

### Books:
- "Effective Modern C++" by Scott Meyers
- "C++ Move Semantics - The Complete Guide" by Nicolai Josuttis
- "A Tour of C++" by Bjarne Stroustrup

### Online Resources:
- [cppreference.com - Move semantics](https://en.cppreference.com/w/cpp/language/move_constructor)
- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/)
- [Herb Sutter's Blog](https://herbsutter.com/)

---

## Conclusion

Move Semantics is one of the **most important features** ever added to C++. It enables:

- ✅ **Zero-cost abstractions** (truly!)
- ✅ **Natural, readable code**
- ✅ **Massive performance gains**
- ✅ **Modern C++ style**

### The Revolution:

**Before C++11:**
```cpp
// Awkward, slow, or complex code
void getData(vector<int>& out);  // Output parameter
vector<int>* createData();       // Manual memory
```

**After C++11:**
```cpp
// Natural, fast, simple code
vector<int> getData();           // Just works! ⚡
auto data = createData();        // Clean and fast!
```

### Final Advice:

> **"When in doubt, follow Rule of Zero"**
>
> Use RAII types, let the compiler handle moves automatically, and enjoy the performance! 🚀

---

*Last Updated: 2024*
*C++ Standard: C++11 and later*
*Feature: Move Semantics*

## Notes moved from main.cpp

This text used to be printed by `main()`. It is lecture material, so it lives here and the program only prints the demo output.

### WHAT IS MOVE SEMANTICS?

**Move Semantics (C++11):**

- Optimization technique
- Transfer ownership instead of copying
- "Steal" resources from temporary objects
- Massive performance improvement

Goal: Avoid expensive deep copies

```cpp
Copy:  obj2 = obj1;              // Deep copy (expensive)
Move:  obj2 = std::move(obj1);   // Transfer ownership (cheap)
```

### CORE CONCEPTS

**Lvalues:**

- Named objects
- Persist beyond expression
- Can take address (&x)
- Example: int x = 5;

**Rvalues:**

- Temporaries
- Exist only during expression
- Cannot take address
- Example: int x = 5 + 3;  // (5+3) is rvalue

**Rvalue Reference (&&):**

- Binds to rvalues (temporaries)
- Enables move semantics
- Syntax: Type&& name

### MOVE OPERATIONS

**Move Constructor:**

```cpp
MyClass(MyClass&& other) noexcept {
    // Steal resources from 'other'
    data = other.data;
    other.data = nullptr;
}
```

**Move Assignment:**

```cpp
MyClass& operator=(MyClass&& other) noexcept {
    if (this != &other) {
        delete data;  // Free current resources
        data = other.data;  // Steal resources
        other.data = nullptr;
    }
    return *this;
}
```

### std::move

**What it does:**

- Casts lvalue to rvalue reference
- Doesn't actually move anything!
- Enables move constructor/assignment to be called

**Usage:**

```cpp
  MyClass obj1;                    // lvalue
  MyClass obj2 = std::move(obj1);  // Cast to rvalue
  // obj1 is now in 'moved-from' state

After std::move:
```

- Original object is in valid but unspecified state
- Can assign new value or destroy
- Don't use it otherwise!

### WHEN MOVES HAPPEN AUTOMATICALLY

1. Returning local variables:

```cpp
MyClass func() {
    MyClass local;
    return local;  // Automatically moved (or RVO)
}
```

2. Passing temporaries:

```cpp
vec.push_back(MyClass());  // Temporary - auto moved
```

3. Initializing from rvalue:

```cpp
MyClass obj = createObject();  // Auto moved
```

### WHEN TO USE std::move

```cpp
Use std::move when:
```

- ✓ You're done with an object
- ✓ Transferring ownership explicitly
- ✓ Inserting into containers when copy not needed
- ✓ Implementing move operations

```cpp
Don't use std::move:
```

- ❌ On const objects (won't move, will copy)
- ❌ On return of local variable (prevents RVO)
- ❌ When you still need the object
- ❌ On temporaries (already rvalues)

### PERFORMANCE COMPARISON

**Copy (Deep Copy):**

- Allocate new memory
- Copy every element
- O(n) complexity
- Example: Copying 1M integers = 1M copies

**Move (Transfer Ownership):**

- Swap/steal pointers
- No element copying
- O(1) complexity
- Example: Moving 1M integers = 3 pointer ops

**Speed Difference:**

Copy:  ~100ms  (for 1M elements)  
Move:  ~0.001ms (1000x faster!)

### RETURN VALUE OPTIMIZATION (RVO)

```cpp
RVO = Compiler optimization that eliminates copy/move
```

**Correct:**

```cpp
MyClass func() {
    MyClass local;
    return local;  ✅ Enables RVO
}
```

**Wrong:**

```cpp
  MyClass func() {
      MyClass local;
      return std::move(local);  ❌ Prevents RVO!
  }

Rule: Never std::move return values!
```

### MOVE-ONLY TYPES

**Some types cannot be copied, only moved:**

- unique_ptr<T>     - Exclusive ownership
- thread            - Thread handles
- ifstream/ofstream - File streams
- mutex             - Mutex locks
- atomic<T>         - Atomic operations

**Creating move-only types:**

```cpp
class MoveOnly {
    // Delete copy operations
    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;

    // Define/default move operations
    MoveOnly(MoveOnly&&) = default;
    MoveOnly& operator=(MoveOnly&&) = default;
};
```

### PERFECT FORWARDING

Universal/Forwarding Reference: T&&

- In template context only
- Binds to both lvalues and rvalues
- Used with std::forward

**Example:**

```cpp
  template<typename T>
  void wrapper(T&& arg) {  // Universal reference
      func(std::forward<T>(arg));  // Perfect forwarding
  }

std::forward:
```

- Preserves lvalue/rvalue-ness
- lvalue → forwards as lvalue
- rvalue → forwards as rvalue

### RULE OF FIVE

**If you define ANY of these, define ALL FIVE:**

```cpp
1. Destructor              ~MyClass()
```

2. Copy Constructor        MyClass(const MyClass&)
3. Copy Assignment         operator=(const MyClass&)
4. Move Constructor        MyClass(MyClass&&)
5. Move Assignment         operator=(MyClass&&)

```cpp
Or use = default / = delete
```

**When to use:**

- Managing raw resources (new/delete)
- Custom memory management
- Wrapping C APIs

### RULE OF ZERO (PREFERRED!)

Don't define ANY special members!

**Instead, use RAII types:**

```cpp
class MyClass {
    unique_ptr<int> data;    // Instead of int*
    vector<int> numbers;     // Instead of int*
    string name;             // Instead of char*

    // No need to define any special members!
    // Compiler generates correct versions!
};
```

**Benefits:**

- ✅ Less code
- ✅ Less bugs
- ✅ Compiler-optimized
- ✅ Automatically correct

### COMMON MISTAKES

1. Using moved-from objects:
- ❌ MyClass obj1;

```cpp
MyClass obj2 = std::move(obj1);
obj1.doSomething();  // Undefined behavior!
```

2. Moving const objects:
- ❌ const MyClass obj1;

```cpp
      MyClass obj2 = std::move(obj1);  // Copies, doesn't move!

3. std::move in return:
```

- ❌ return std::move(local);  // Prevents RVO
- ✅ return local;             // Correct

4. Forgetting noexcept:
- ❌ MyClass(MyClass&& other)  // Might not be used!
- ✅ MyClass(MyClass&& other) noexcept  // Preferred

5. Moving temporaries:
- ❌ vec.push_back(std::move(MyClass()));  // Redundant
- ✅ vec.push_back(MyClass());             // Already moved

### BEST PRACTICES

1. Follow Rule of Zero when possible
2. Mark move operations noexcept
3. Leave moved-from objects in valid state

```cpp
4. Don't std::move const objects
5. Don't std::move return values
6. Use std::move explicitly for lvalues
```

7. Document move-only types clearly
8. Test moved-from state is valid
9. Prefer emplace_back over push_back

```cpp
10. Use std::forward for perfect forwarding
```

### SYNTAX REFERENCE

**Rvalue Reference:**

```cpp
  MyClass&& rref = MyClass();  // Binds to temporary

std::move:
  MyClass obj1;
  MyClass&& rref = std::move(obj1);  // Cast to rvalue
```

**Move Constructor:**

```cpp
MyClass(MyClass&& other) noexcept;
```

**Move Assignment:**

```cpp
MyClass& operator=(MyClass&& other) noexcept;
```

**Universal Reference:**

```cpp
template<typename T>
void func(T&& arg);  // Universal reference
```

**Perfect Forwarding:**

```cpp
std::forward<T>(arg);
```

### BENEFITS

- ✅ Massive Performance Gain
  - Avoid expensive copies
  - O(1) instead of O(n)
  - Can be 100-1000x faster

- ✅ Enables Move-Only Types
  - unique_ptr for exclusive ownership
  - Resources that can't be copied

- ✅ Better Resource Management
  - Clear ownership transfer
  - No accidental copies

- ✅ More Expressive Code
  - Intent is clear (move vs copy)
  - Ownership semantics explicit

### CONTAINER OPERATIONS

**vector operations with move:**

```cpp
vector<MyClass> vec;

MyClass obj;
vec.push_back(obj);              // Copy
vec.push_back(std::move(obj));   // Move
vec.push_back(MyClass());        // Move (temporary)
vec.emplace_back(args...);       // Construct in-place (best!)
```

**Performance ranking:**

```cpp
1. emplace_back()           - Best (no move/copy)
```

2. push_back(temporary)     - Good (auto move)

```cpp
3. push_back(std::move())   - Good (explicit move)
```

4. push_back(lvalue)        - Worst (copy)

### WHY noexcept MATTERS

**Move operations should be noexcept:**

```cpp
MyClass(MyClass&&) noexcept;  ✅
MyClass(MyClass&&);           ⚠️ Might not be used!
```

**Why:**

- STL containers need strong exception guarantee
- If move can throw, they use copy instead
- noexcept enables optimizations

**Example:**

```cpp
vector::resize() will:
```

  - Use move if noexcept
  - Use copy if not noexcept

### REAL-WORLD IMPACT

**Before Move Semantics (C++03):**

- Expensive string/vector returns
- Awkward workarounds (swap trick)
- Performance sacrifices

**After Move Semantics (C++11+):**

- Zero-cost string/vector returns
- Natural, efficient code
- 10-100x speedup in many cases

**Example:**

```cpp
vector<string> readFile() {
    vector<string> lines;
    // ... read millions of lines ...
    return lines;  // C++03: Copy millions!
                   // C++11: Move (instant!)
}
```

### KEY TAKEAWAYS

```cpp
1. Move = Transfer ownership (cheap)
2. Copy = Duplicate resources (expensive)
3. std::move = Cast to rvalue reference
4. Moved-from = Valid but unspecified state
5. noexcept = Required for optimal performance
6. RVO = Compiler optimization (better than move)
7. Rule of Zero = Use RAII types (best!)
8. Rule of Five = If custom, define all five
```

### COMPARISON TABLE

┌──────────────┬──────────┬──────────┬────────────┐  
│ Operation    │ Copy     │ Move     │ Emplace    │  
├──────────────┼──────────┼──────────┼────────────┤  
│ Complexity   │ O(n)     │ O(1)     │ O(1)       │  
│ Allocations  │ New      │ Steal    │ In-place   │  
│ Performance  │ Slow     │ Fast     │ Fastest    │  
│ Use Case     │ Need     │ Done     │ Create     │  
│              │ both     │ with src │ new object │  
└──────────────┴──────────┴──────────┴────────────┘

### THE MOVE REVOLUTION (closing summary)

**Move Semantics fundamentally changed C++:**

**Before (C++03):**

- Returning vectors was slow
- Pass-by-reference everywhere
- Complex ownership patterns

**After (C++11+):**

- Return by value is fast
- Natural, modern code
- Clear ownership with unique_ptr

**Move Semantics enabled:**

- unique_ptr (exclusive ownership)
- Move-only types
- Efficient standard library
- Modern C++ style

## Notes moved from the example functions

The demo functions in `main.cpp` used to print this explanation between the demo steps. It is lecture material, so it lives here and the program only prints the demo output.

### Example 1: Lvalues vs Rvalues

```cpp
int x = 10;           // x is lvalue (has name)
int y = 20;           // y is lvalue (has name)
int* ptr = &x;        // Can take address of lvalue
```

**Rvalues (Right values / Temporaries)**

```cpp
int z = x + y;        // (x + y) is rvalue (temporary)
// int* bad = &(x + y);  ❌ ERROR: can't take address of rvalue

int result = getValue();  // Function return is rvalue
```

**String Literals**

```cpp
string s1 = "Hello";      // String literal is rvalue
// string& ref = "Hello";  ❌ Can't bind lvalue ref to rvalue
// const string& ref = "Hello"; ✅ Can bind const lvalue ref to rvalue
```

**Summary**

**Lvalues:**

- Have names
- Persist beyond expression
- Can appear on left of '='
- Can take address with &

**Rvalues:**

- Temporaries
- Exist only during expression
- Can only appear on right of '='
- Cannot take address

### Example 2: The Copy Problem

- ⚠️  Problem: We just copied 1,000,000 integers!

This is expensive and unnecessary if we don't need original anymore.

### Example 3: Move Constructor

✨ Move is just pointer swapping - O(1) instead of O(n)!

### Example 4: std::move Usage

```cpp
**What is std::move?**
std::move() doesn't actually move anything!
```

It just casts an lvalue to an rvalue reference.  
This allows the move constructor/assignment to be called.

- ⚠️  Important: Don't use moved-from objects!

r1 and r2 are now in 'moved-from' state.  
Only safe operations: assign new value or destroy.

### Example 5: Move With Vectors

**✨ Performance Ranking:**

```cpp
1. emplace_back()      - Construct in-place (best)
```

2. push_back(temporary) - Automatic move

```cpp
3. push_back(std::move) - Explicit move
```

4. push_back(lvalue)    - Copy (worst)

### Example 6: RVO (Return Value Optimization)

- ✓ Compiler optimizes away copy/move (RVO)

- ❌ std::move prevents RVO optimization!

- 💡 Rule: Never use std::move on return of local variable!

```cpp
return local;              ✅ Correct (RVO or auto-move)
return std::move(local);   ❌ Wrong (prevents RVO)
```

### Example 7: Move-Only Types

**Attempting copy (would fail)**

```cpp
// MoveOnlyResource res2 = res1;  ❌ Error: copy deleted
```

**Move-only types in STL**

**Examples of move-only types:**

- unique_ptr<T>     - Exclusive ownership
- thread            - Thread handle
- ifstream/ofstream - File streams
- mutex             - Mutex locks
- atomic<T>         - Atomic types

- ✓ Move-only types can be stored in containers

### Example 8: Perfect Forwarding

--- What is T&&? ---  
T&& is a 'forwarding reference' (or 'universal reference')  
It can bind to BOTH lvalues and rvalues

```cpp
int x = 42;
wrapper(x);  // x is lvalue

wrapper(100);  // 100 is rvalue

wrapper(std::move(x));  // x cast to rvalue
```

- 💡 std::forward preserves the value category!

If arg is lvalue → forwards as lvalue  
If arg is rvalue → forwards as rvalue

### Example 9: Rule Of Five

**If you define ANY of these 5, define ALL:**

1. Destructor
2. Copy Constructor
3. Copy Assignment
4. Move Constructor
5. Move Assignment

### Example 10: Rule Of Zero (Best!)

--- Rule of Zero: Don't define ANY special members! ---  
Instead: Use RAII types (smart pointers, containers)  
Compiler auto-generates correct move/copy operations!

- ✓ Moved successfully (compiler-generated)

- 💡 Rule of Zero is BETTER than Rule of Five!
  - Less code to write
  - Less code to maintain
  - Compiler generates optimal code
  - Harder to make mistakes

## What was tested

- Compiled with GCC 13.3 (`-std=c++23`) using a stand-in `<print>` header, because `std::println` needs GCC 14 and it was not available here. The source files were compiled directly, not through CMake.
- Run, and the output matches the original `std::cout` version except that the lecture text moved into *Notes moved from main.cpp* is no longer printed (and the remaining example titles were renumbered where an example was removed).
- Run once with AddressSanitizer + UndefinedBehaviorSanitizer: no findings.

## Not tested

- The real `<print>` header (GCC 14+, recent Clang or MSVC) and platforms other than Linux x86-64.
