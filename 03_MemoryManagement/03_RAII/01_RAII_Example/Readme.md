# RAII (Resource Acquisition Is Initialization)

## 📚 Table of Contents
- [What is RAII?](#what-is-raii)
- [Is RAII a Design Pattern?](#is-raii-a-design-pattern)
- [Core Principle](#core-principle)
- [Why RAII?](#why-raii)
- [Benefits](#benefits)
- [How It Works](#how-it-works)
- [RAII Lifecycle](#raii-lifecycle)
- [Code Examples](#code-examples)
- [Standard Library RAII Types](#standard-library-raii-types)
- [Creating RAII Classes](#creating-raii-classes)
- [Types of Resources](#types-of-resources)
- [RAII vs Other Languages](#raii-vs-other-languages)
- [Common Mistakes](#common-mistakes)
- [Best Practices](#best-practices)
- [Real-World Examples](#real-world-examples)
- [Key Takeaways](#key-takeaways)
- [How to Run](#how-to-run)
- [Notes moved from main.cpp](#notes-moved-from-maincpp)

---

## What is RAII?

**RAII** stands for **Resource Acquisition Is Initialization**.

It's a C++ programming idiom/principle where:
- **Resource acquisition** happens in the **constructor**
- **Resource release** happens in the **destructor**
- Cleanup is **automatic** and **deterministic**

### Resources Include:
- Memory (heap allocations)
- File handles
- Network sockets
- Database connections
- Mutex locks
- Graphics contexts
- Any resource requiring cleanup

---

## Is RAII a Design Pattern?

### ❌ **NO**, RAII is NOT a traditional Design Pattern!

| Design Pattern | RAII |
|----------------|------|
| ✓ From Gang of Four (GoF) | ✓ C++ Programming Idiom |
| ✓ A design choice, used when the problem fits | ✓ The recommended default for owning resources in C++ |
| ✓ Multiple solutions exist | ✓ Alternatives exist (manual cleanup), but they are error-prone |
| ✓ Problem-specific | ✓ Fundamental principle |
| ✓ Example: Singleton, Factory | ✓ Example: smart pointers, RAII wrappers |

### Classification Hierarchy:
```
Programming Concepts:
├── Principles ────────── (SOLID, DRY, KISS)
├── Idioms ────────────── (RAII, PIMPL, Copy-and-Swap) ⬅️ RAII is HERE
├── Design Patterns ───── (GoF: Singleton, Factory, Observer)
└── Architectures ─────── (MVC, Microservices, Layered)
```

### What Makes RAII Different:
- **Not a pattern you pick per problem** - It's the recommended default way to manage resources in C++
- **Relies on a language guarantee** - The destructor of a fully constructed object runs when its scope ends, also while an exception unwinds the stack
- **Foundation** - Smart pointers, containers, streams and lock guards are all built on it
- **Zero overhead** - No runtime cost compared with writing the cleanup by hand (unlike garbage collection). "Zero overhead" does not mean free: `shared_ptr` still pays for reference counting, for example

> 💡 **Think of it this way:**
> - **Design Patterns** = Tools you choose from your toolbox
> - **RAII** = A basic technique that many of those tools (smart pointers, lock guards) are built with

---

## Core Principle
```cpp
class RAIIExample {
public:
    // Constructor: ACQUIRE resource
    RAIIExample() {
        resource = acquireResource();
    }
    
    // Destructor: RELEASE resource (automatic!)
    ~RAIIExample() {
        releaseResource(resource);
    }
    
private:
    Resource* resource;
};
```

### Three Simple Rules:
1. **Constructor acquires** the resource
2. **Destructor releases** the resource
3. **Compiler guarantees** the destructor is called for every fully constructed object (if a constructor throws, that object's destructor does *not* run, but its already-built members and bases are destroyed - tested)

---

## Why RAII?

### The Problem (Without RAII):
```cpp
void problematic() {
    FILE* file = fopen("data.txt", "r");
    
    // ... do work ...
    
    if (error) {
        return;  // 💥 FORGOT TO CLOSE FILE! Resource leak!
    }
    
    fclose(file);  // Never reached!
}
```

### The Solution (With RAII):
```cpp
void solution() {
    FileHandle file("data.txt", "r");  // Opens in constructor
    
    // ... do work ...
    
    if (error) {
        return;  // ✅ File automatically closed by destructor!
    }
    
    // File automatically closed when scope ends
}
```

---

## Benefits

### ✅ **Exception Safety**
Resources are cleaned up even if exceptions are thrown.
```cpp
void exceptionSafe() {
    unique_ptr<int[]> data(new int[1000]);
    
    doWork();  // If this throws, data is still freed!
    
    // No try-catch needed - automatic cleanup!
}
```

### ✅ **No Memory Leaks**
Impossible to forget cleanup - it's automatic!
```cpp
void noLeaks() {
    auto ptr = make_unique<MyClass>();
    // Automatically deleted - can't forget!
}
```

### ✅ **No Dangling Resources**
Files always closed, locks always released, connections always terminated.

### ✅ **Clear Ownership**
Who owns the resource is obvious from the object lifetime.

### ✅ **Less Code**
No manual cleanup code needed - more maintainable.

### ✅ **Composable**
RAII objects can contain other RAII objects - cleanup happens in correct order.

---

## How It Works

### Automatic Cleanup Triggers:
```cpp
void demonstration() {
    {
        unique_ptr<int> ptr(new int(42));
        
        // Destructor called when:
        // 1. Scope ends (here)
        
    }  // ← ptr's destructor called here
    
    if (condition) {
        unique_ptr<int> ptr2(new int(100));
        return;  // 2. Return statement - destructor still called!
    }
    
    try {
        unique_ptr<int> ptr3(new int(200));
        throw exception();  // 3. Exception thrown - destructor still called!
    } catch (...) {
        // ptr3 already cleaned up
    }
    
}  // 4. Function exits
```

---

## RAII Lifecycle
```
1. Object Creation
   └─► Constructor runs
       └─► Resource ACQUIRED (memory, file, lock, etc.)

2. Object Usage
   └─► Resource accessed and used safely
   
3. Object Destruction (Automatic!)
   └─► Destructor runs
       └─► Resource RELEASED
       
Destructor called when:
  • Scope ends
  • Return statement
  • Exception thrown  
  • delete called (if heap-allocated)
  • Program exits normally (global/static objects only - the locals of a function that calls std::exit() are NOT destroyed, tested)
```

### Visual Flow:
```
┌─────────────────────────┐
│   Object Created        │
│   Constructor { }       │  ⬅️ ACQUIRE resource
└───────────┬─────────────┘
            │
            ▼
┌─────────────────────────┐
│   Object Used           │
│   member_functions()    │  ⬅️ USE resource
└───────────┬─────────────┘
            │
            ▼
┌─────────────────────────┐
│   Object Destroyed      │
│   ~Destructor { }       │  ⬅️ RELEASE resource
└─────────────────────────┘  (AUTOMATIC!)
```

---

## Code Examples

### Example 1: File Handle (Custom RAII)
```cpp
class FileHandle {
private:
    FILE* file;
    string filename;
    
public:
    // Constructor: ACQUIRE
    FileHandle(const string& fname, const char* mode) 
        : filename(fname) {
        file = fopen(fname.c_str(), mode);
        if (!file) {
            throw runtime_error("Failed to open file");
        }
    }
    
    // Destructor: RELEASE
    ~FileHandle() {
        if (file) {
            fclose(file);
        }
    }
    
    // Delete copy (files shouldn't be copied)
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;
    
    // Write method
    void write(const string& data) {
        fprintf(file, "%s", data.c_str());
    }
};

// Usage:
void useFile() {
    FileHandle file("data.txt", "w");
    file.write("Hello, RAII!");
    
    // File automatically closed - no manual fclose()!
}
```

### Example 2: Mutex Lock Guard (Custom RAII)
```cpp
class MutexGuard {
private:
    mutex& mtx;
    
public:
    // Constructor: ACQUIRE lock
    MutexGuard(mutex& m) : mtx(m) {
        mtx.lock();
    }
    
    // Destructor: RELEASE lock
    ~MutexGuard() {
        mtx.unlock();
    }
    
    MutexGuard(const MutexGuard&) = delete;
    MutexGuard& operator=(const MutexGuard&) = delete;
};

// Usage:
mutex globalMutex;

void threadSafeFunction() {
    MutexGuard lock(globalMutex);
    
    // Critical section
    sharedData++;
    
    // Lock automatically released - even if exception thrown!
}
```

### Example 3: Timer (RAII for Profiling)
```cpp
class Timer {
private:
    string name;
    chrono::steady_clock::time_point start;
    
public:
    explicit Timer(const string& n) : name(n) {
        start = chrono::steady_clock::now();
    }
    
    ~Timer() {
        auto end = chrono::steady_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
        cout << name << " took " << duration.count() << "ms\n";
    }
};

// Usage:
void measurePerformance() {
    Timer t("Operation");
    // Do expensive work...
}  // Automatically prints elapsed time
```
> `steady_clock` is the right clock for measuring intervals (it never jumps backwards). The original used `high_resolution_clock`, which in libstdc++ is just another name for `system_clock` (tested with `static_assert`), and that clock can be adjusted while you measure.

### Example 4: Database Connection
```cpp
class DatabaseConnection {
    bool connected = false;
    int pending = 0;                       // queries not yet committed
public:
    explicit DatabaseConnection(const string& connStr) {
        // ... connect ...
        connected = true;
    }
    ~DatabaseConnection() {
        if (connected) {
            if (pending > 0) { /* roll back */ }
            // ... close ...
        }
    }
    DatabaseConnection(const DatabaseConnection&) = delete;
    DatabaseConnection& operator=(const DatabaseConnection&) = delete;

    void execute(const string& query) { ++pending; }
    void commit() { pending = 0; }
};
```
If an exception is thrown between `execute` and `commit`, the destructor still runs and rolls back.

### Example 5: Memory Pool (malloc/free)
```cpp
class MemoryPool {
    void* memory;
    size_t size;
public:
    explicit MemoryPool(size_t s) : memory(malloc(s)), size(s) {
        if (!memory) throw bad_alloc();
    }
    ~MemoryPool() { free(memory); }
    MemoryPool(const MemoryPool&) = delete;
    MemoryPool& operator=(const MemoryPool&) = delete;
};
```
A constructor that throws leaves no object behind, so the destructor does not run. That is fine here because nothing was acquired yet.

### Example 6: Bad vs Good
See [Why RAII?](#why-raii): `main.cpp` runs the manual `FILE*` version (leaks on early return) and the `FileHandle` version side by side.

### Example 7: Standard Library (Smart Pointers, Containers, Streams, Locks)
```cpp
void standardRAII() {
    {
        auto ptr = make_unique<int>(42);        // unique_ptr: exclusive ownership
        auto shared = make_shared<string>("RAII");
        auto another = shared;                  // Reference count = 2
    }  // Deleted when the last reference goes away

    {
        vector<int> numbers = {1, 2, 3};        // containers free their memory
        ofstream out("log.txt");                // stream closes the file
        mutex mtx;
        lock_guard<mutex> lock(mtx);            // lock released at scope end
    }
    // No manual delete / close / unlock anywhere!
}
```

---

## Standard Library RAII Types

### Memory Management:
```cpp
unique_ptr<T>       // Exclusive ownership
shared_ptr<T>       // Shared ownership
weak_ptr<T>         // Non-owning observer
```

### Containers (All use RAII):
```cpp
vector<T>           // Dynamic array
string              // String (dynamic char array)
map<K, V>           // Associative container
set<T>              // Unique elements
deque<T>            // Double-ended queue
// All automatically manage their memory!
```

### File Streams:
```cpp
ifstream            // Input file
ofstream            // Output file
fstream             // Input/output file
// All automatically close files
```

### Synchronization:
```cpp
lock_guard<mutex>   // Basic lock
unique_lock<mutex>  // Advanced lock
scoped_lock         // Multiple mutex lock (C++17)
// All automatically release locks
```

---

## Creating RAII Classes

### Template for RAII Class:
```cpp
class RAIIWrapper {
private:
    ResourceType* resource;  // The resource to manage
    
public:
    // Constructor: ACQUIRE resource
    RAIIWrapper() {
        resource = acquireResource();
        if (!resource) {
            throw runtime_error("Failed to acquire resource");
        }
    }
    
    // Destructor: RELEASE resource
    ~RAIIWrapper() {
        if (resource) {
            releaseResource(resource);
            resource = nullptr;
        }
    }
    
    // Delete copy operations (or implement properly)
    RAIIWrapper(const RAIIWrapper&) = delete;
    RAIIWrapper& operator=(const RAIIWrapper&) = delete;
    
    // Optionally allow move operations
    RAIIWrapper(RAIIWrapper&& other) noexcept 
        : resource(other.resource) {
        other.resource = nullptr;
    }
    
    RAIIWrapper& operator=(RAIIWrapper&& other) noexcept {
        if (this != &other) {
            if (resource) releaseResource(resource);
            resource = other.resource;
            other.resource = nullptr;
        }
        return *this;
    }
    
    // Access methods
    ResourceType* get() const { return resource; }
};
```

### Checklist for RAII Classes:
- ☑️ Constructor acquires resource
- ☑️ Destructor releases resource
- ☑️ Destructor is `noexcept` (implicit in C++11+, even for a user-written destructor, unless a member or base class has a `noexcept(false)` destructor - tested)
- ☑️ Copy operations deleted or properly implemented
- ☑️ Move operations if needed
- ☑️ Exception-safe acquisition
- ☑️ Handle null/invalid state
- ☑️ Mark destructor `virtual` if base class

---

## Types of Resources

### Memory:
```cpp
unique_ptr<T>       // Heap memory
shared_ptr<T>       // Shared heap memory
vector<T>           // Dynamic array
string              // Dynamic string
```

### Files:
```cpp
ifstream/ofstream   // File streams
FILE* with wrapper  // C-style files
Custom FileHandle   // Your wrapper
```

### Locks/Synchronization:
```cpp
lock_guard<mutex>   // Mutex lock
unique_lock<mutex>  // Advanced mutex lock
scoped_lock         // Multiple locks
```

### Network:
```cpp
SocketHandle        // Network sockets
Connection          // Database/network connections
```

### Database:
```cpp
Connection          // DB connection
Transaction         // Transaction scope
PreparedStatement   // Prepared query
```

### Graphics:
```cpp
GLContext           // OpenGL context
Texture             // GPU texture
Buffer              // GPU buffer
SwapChain           // Rendering swap chain
```

### Operating System:
```cpp
FileDescriptor      // OS file descriptor
ProcessHandle       // Process handle
ThreadHandle        // Thread handle
EventHandle         // Event object
```

---

## RAII vs Other Languages

### C++ (RAII):
```cpp
void cppWay() {
    FileHandle file("data.txt", "r");
    // Use file...
}  // File automatically closed - DETERMINISTIC
```
- ✅ Deterministic cleanup (destructor called at end of scope)
- ✅ Exception-safe by default
- ✅ Zero runtime overhead
- ✅ No manual cleanup needed

### Java (Garbage Collection + try-with-resources):
```java
void javaWay() throws IOException {
    try (FileInputStream file = new FileInputStream("data.txt")) {
        // Use file...
    }  // file.close() called automatically (AutoCloseable)
}
```
The older style (before Java 7) needed `try`/`finally`:
```java
void oldJavaWay() throws IOException {
    FileInputStream file = new FileInputStream("data.txt");
    try {
        // Use file...
    } finally {
        file.close();  // Must remember!
    }
}
```
- ✅ try-with-resources is similar to RAII, but the programmer has to remember to write it
- ⚠️ Non-deterministic GC for memory
- ⚠️ Runtime overhead (GC)
- ❌ Nothing forces you to use try-with-resources (a plain `new FileInputStream(...)` compiles fine)

### C# (using statement):
```csharp
void csharpWay() {
    using (var file = File.OpenRead("data.txt")) {
        // Use file...
    }  // Disposed automatically
}
```
- ✅ using statement similar to RAII
- ⚠️ Only for IDisposable types
- ⚠️ Non-deterministic GC for memory
- ⚠️ Runtime overhead (GC)

### C (Manual):
```c
void cWay() {
    FILE* file = fopen("data.txt", "r");
    // Use file...
    fclose(file);  // Must remember!
}
```
- ❌ Completely manual
- ❌ Easy to forget cleanup
- ❌ No automatic cleanup on early return (C has no destructors)
- ❌ Error-prone

### Python (with statement):
```python
def pythonWay():
    with open("data.txt", "r") as file:
        data = file.read()
    # File closed automatically
```
- ✅ with statement similar to RAII
- ⚠️ Runtime overhead (interpreter)
- ⚠️ GC for memory

### 🏆 **C++ RAII is Unique:**
- Deterministic, automatic cleanup
- Zero overhead abstraction
- Exception-safe by design
- No need for try-finally or using blocks
- **This is C++'s superpower!** 🦸

---

## Common Mistakes

### ❌ Mistake 1: Not Making Destructor Virtual in Base Class
```cpp
class Base {
public:
    ~Base() { }  // ❌ Not virtual!
};

class Derived : public Base {
public:
    ~Derived() { cleanup(); }
};

Base* ptr = new Derived();
delete ptr;  // 💥 Undefined behavior! In practice only ~Base() runs, so Derived's resources leak
```
> Tested: AddressSanitizer reports `new-delete-type-mismatch` for this. Note that with `class` and no `public:` the destructors are private, and the code does not even compile (`'Base::~Base()' is private within this context`).

**Fix:**
```cpp
class Base {
public:
    virtual ~Base() { }  // ✅ Virtual destructor
};
```

### ❌ Mistake 2: Forgetting to Delete Copy Operations
```cpp
class RAIIFile {
    FILE* file;
public:
    RAIIFile(const char* name) { file = fopen(name, "r"); }
    ~RAIIFile() { fclose(file); }
    // ❌ Default copy constructor copies pointer!
};

RAIIFile f1("data.txt");
RAIIFile f2 = f1;  // 💥 Both point to same file!
// 💥 Double-close when destructors run! (tested with glibc: the program aborts with `free(): double free detected`)
```

**Fix:**
```cpp
class RAIIFile {
    FILE* file;
public:
    RAIIFile(const char* name) { file = fopen(name, "r"); }
    ~RAIIFile() { fclose(file); }
    
    // ✅ Delete copy operations
    RAIIFile(const RAIIFile&) = delete;
    RAIIFile& operator=(const RAIIFile&) = delete;
};
```

### ❌ Mistake 3: Throwing Exceptions in Destructor
```cpp
class Bad {
public:
    ~Bad() {
        throw runtime_error("Error!");  // 💥 std::terminate() is called!
    }
};
```
> Since C++11 destructors are `noexcept` by default, so a `throw` that leaves the destructor calls `std::terminate()` (tested: the program aborts, `catch` is never reached; GCC even warns `'throw' will always call 'terminate'`). It is not "undefined behavior" - the behavior is well defined, and it is fatal.

**Fix:**
```cpp
class Good {
public:
    ~Good() noexcept {  // ✅ noexcept (implicit in C++11+)
        try {
            // Cleanup code
        } catch (...) {
            // Handle or log, don't rethrow
        }
    }
};
```

### ❌ Mistake 4: Returning Pointers to Local RAII Objects
```cpp
MyClass* getBad() {
    MyClass obj;
    return &obj;  // 💥 Dangling pointer!
}  // obj destroyed here
```

**Fix:**
```cpp
unique_ptr<MyClass> getGood() {
    return make_unique<MyClass>();  // ✅ Returns ownership
}
```

### ❌ Mistake 5: Manual new/delete with RAII
```cpp
void bad() {
    MyClass* ptr = new MyClass();  // ❌ Manual management
    // ... use ptr ...
    delete ptr;  // Must remember!
}
```

**Fix:**
```cpp
void good() {
    auto ptr = make_unique<MyClass>();  // ✅ RAII
    // ... use ptr ...
}  // Automatic cleanup
```

---

## Best Practices

### 1. Always Prefer RAII Over Manual Management
```cpp
// ❌ BAD
void manual() {
    int* data = new int[100];
    // ... work ...
    delete[] data;
}

// ✅ GOOD
void raii() {
    auto data = make_unique<int[]>(100);
    // ... work ...
}  // Automatic cleanup
```

### 2. Use Smart Pointers
```cpp
unique_ptr<T>  // Default choice - exclusive ownership
shared_ptr<T>  // When multiple owners needed
weak_ptr<T>    // For non-owning observers
```

### 3. Use Standard Containers
```cpp
vector<T>      // Instead of T*
string         // Instead of char*
map<K,V>       // Managed storage
```

### 4. Make Destructors Virtual in Base Classes
```cpp
class Base {
public:
    virtual ~Base() = default;  // ✅
};
```

### 5. Delete Copy Operations or Implement Rule of Five
```cpp
class MyRAII {
public:
    // Option 1: Delete copy (common)
    MyRAII(const MyRAII&) = delete;
    MyRAII& operator=(const MyRAII&) = delete;
    
    // Option 2: Implement Rule of Five (if needed)
    // Destructor, Copy Constructor, Copy Assignment,
    // Move Constructor, Move Assignment
};
```

### 6. Mark Destructors noexcept
```cpp
~MyClass() noexcept {  // Implicit in C++11+, but be explicit
    // Cleanup code
}
```

### 7. Acquire Resources in Constructor Only
```cpp
class FileHandle {
public:
    FileHandle(const string& name) {  // ✅ Acquire in constructor
        file = fopen(name.c_str(), "r");
    }
    
    // ❌ Don't do: void open(const string& name);
};
```

### 8. Release Resources in Destructor Only
```cpp
class FileHandle {
public:
    ~FileHandle() {  // ✅ Release in destructor
        if (file) fclose(file);
    }
    
    // ❌ Don't do: void close();
};
```

### 9. Follow Rule of Zero When Possible
```cpp
class Modern {
    unique_ptr<int> data;    // ✅ RAII type
    vector<int> numbers;     // ✅ RAII type
    string name;             // ✅ RAII type
    
    // No need to define any special members!
    // Compiler generates correct versions!
};
```

### 10. Compose RAII Objects
```cpp
class CompositeRAII {
    unique_ptr<Connection> connection;   // RAII
    vector<unique_ptr<Buffer>> buffers;  // RAII
    ofstream log;                        // RAII
    
    // All automatically cleaned up in reverse order of declaration!
};
```
> Don't make a `lock_guard` a data member: it would hold the lock for the whole life of the object, and it has no default constructor, so `CompositeRAII c;` fails with `no matching function for call to 'std::lock_guard<std::mutex>::lock_guard()'` (tested).

---

## Real-World Examples

### Game Engine:
```cpp
class TextureManager {
public:
    TextureManager() {
        // Load textures
    }
    
    ~TextureManager() = default;   // Textures automatically unloaded
    
private:
    unique_ptr<Texture> mainTexture;
    vector<unique_ptr<Sprite>> sprites;
};
```

### Web Server:
```cpp
class ConnectionHandler {
public:
    explicit ConnectionHandler(unique_ptr<Socket> s)
        : socket(move(s)) {
        // Connection established
    }
    // No destructor needed: unique_ptr closes the socket (Rule of Zero)
    
private:
    unique_ptr<Socket> socket;
};
```
> Take ownership with a `unique_ptr` parameter, not a raw `Socket*`. And don't keep a `lock_guard` member that holds a global mutex for the whole life of the connection - take locks in short scopes.

### Database Application:
```cpp
class Transaction {
public:
    explicit Transaction(DatabaseConnection& c) : conn(c) {
        conn.beginTransaction();
    }
    
    ~Transaction() {
        if (!committed) {
            conn.rollback();  // Auto-rollback if not committed
        }
    }
    
    Transaction(const Transaction&) = delete;
    Transaction& operator=(const Transaction&) = delete;
    
    void commit() {
        conn.commit();
        committed = true;
    }
    
private:
    DatabaseConnection& conn;
    bool committed = false;
};
```

### File Processing:
```cpp
void processFile(const string& filename) {
    Timer timer("File Processing");           // RAII: Timer
    ifstream file(filename);                   // RAII: File
    lock_guard<mutex> lock(fileMutex);        // RAII: Lock
    auto buffer = make_unique<char[]>(1024);  // RAII: Memory
    
    // Process file...
    
}  // All resources automatically cleaned up in reverse order!
```

---

## Key Takeaways

### 🎯 Core Concepts:
1. **RAII = Constructor acquires, Destructor releases**
2. **Automatic, deterministic cleanup**
3. **Exception-safe by design**
4. **No extra runtime cost** compared with hand-written cleanup

### 📊 Classification:
- ❌ **NOT** a Gang of Four Design Pattern
- ✅ **IS** a C++ Programming Idiom/Principle
- ✅ **IS** the foundation for modern C++
- ✅ **IS** the standard way to manage resources in professional C++ code

### 💪 Benefits:
- No memory leaks
- No resource leaks
- Exception-safe
- Less error-prone
- More maintainable
- No garbage-collector overhead

### 🚀 In Practice:
```cpp
// Always use RAII types:
unique_ptr<T>        // Not: T*
vector<T>            // Not: T*
string               // Not: char*
ifstream             // Not: FILE*
lock_guard<mutex>    // Not: manual lock/unlock
```

### 🎓 The RAII Philosophy:
> **"If you can't make it easy, make it impossible to get wrong."**
*(I could not find where this quote comes from. The closest well-known principle is Scott Meyers' "Make interfaces easy to use correctly and hard to use incorrectly" from Effective C++ - quoted from memory, not checked.)*

RAII makes it much harder to:
- Forget to release resources
- Leak memory
- Leave files open
- Leave a mutex locked when an exception is thrown

It does not make it impossible: `shared_ptr` cycles still leak, and `release()` or raw `new` bypass the protection.

### 🏆 Why RAII Makes C++ Special:
**This is a big part of why C++ gets by without garbage collection!**
- Deterministic cleanup (not whenever GC runs)
- Zero overhead (no GC pauses)
- Works for all resources (not just memory)
- Automatic and reliable

---

## Summary

### What RAII Is:
- ✅ C++ programming idiom
- ✅ Resource management principle
- ✅ Foundation of modern C++
- ✅ Core language philosophy

### What RAII Is NOT:
- ❌ A Gang of Four design pattern
- ❌ Optional (it's the recommended way)
- ❌ Complex (it's actually simple!)
- ❌ Costly (no garbage collector, no extra runtime machinery)

### When to Use RAII:
- ✅ **Always!** For any resource management
- ✅ Memory allocation → `unique_ptr`, `shared_ptr`
- ✅ File operations → `ifstream`, `ofstream`, custom wrappers
- ✅ Locks → `lock_guard`, `unique_lock`
- ✅ Database connections → Custom RAII wrappers
- ✅ Any resource requiring cleanup → Create RAII wrapper

### The Rule:
> **If you write `new`, you should use a RAII wrapper!**
>
> **Better yet: Don't write `new` - use RAII types!**

---

## How to Run

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o raii && ./raii
```
`main.cpp` uses `std::println` from `<print>` (C++23). It needs a recent compiler (GCC 14+ or a current Clang with a C++23 standard library - versions from memory, not checked). The program creates `example.txt`, `manual.txt`, `raii.txt` and `stream_example.txt` in the current directory.

---

## Notes moved from main.cpp

The big "summary" printout at the end of `main()` was removed. Most of it is already in this README (Core Principle, Lifecycle, Why RAII, Benefits, Types of Resources, Standard Library RAII Types, Creating RAII Classes, Common Mistakes, Best Practices, Key Takeaways). What was only in `main.cpp`:

### What each example in main.cpp shows
- **Example 1 (FileHandle)** - the destructor prints `📁 Closing file: example.txt` after each block; opening fails with an exception instead of returning a bad handle.
- **Example 2 (MutexGuard)** - prints acquire/release around each critical section; the counter goes 1, then 6.
- **Example 3 (Timer)** - the destructor reports elapsed milliseconds; nested timers are destroyed inner first, then outer.
- **Example 4 (DatabaseConnection)** - after the thrown `Network error!` the destructor prints `Rolling back 2 uncommitted transactions` and closes the connection.
- **Example 5 (MemoryPool)** - two pools; they are freed in reverse order of creation (Pool2, then Pool1).
- **Example 6 (Bad vs Good)** - the manual version prints `FILE LEAKED!` and never closes the file; the `FileHandle` version prints `Closing file: raii.txt` even though it returns early.
- **Example 7 (Standard library)** - `unique_ptr`/`shared_ptr`, `vector`/`string`, `ofstream` and `lock_guard`.

### Memory management evolution
```cpp
// C-style (manual)
int* p = (int*)malloc(sizeof(int));
*p = 42;
free(p);  // Must remember!

// C++ old (new/delete)
int* p = new int(42);
delete p;  // Must remember!

// C++ modern (RAII)
auto p = make_unique<int>(42);
// Automatic cleanup!
```

### More real-world areas
- **Desktop application:** window handles, file operations, Windows registry keys.
- **Embedded systems:** hardware peripheral access, interrupt handling, memory-mapped I/O.

### Things found while testing (GCC 13, sanitizers on)
- The original timer loops (`int sum`, up to 1,000,000 iterations) overflowed `int`, which is undefined behavior (UBSan: `signed integer overflow`); the printed results 704982704 and 1783293664 were wrapped values. `main.cpp` now uses `long long`, so the results are 4999950000 and 499999500000.
- `FileHandle` listed its members in a different order than its initializer list (`-Wreorder`); fixed.
- `badExample()` really leaks its `FILE*`, but AddressSanitizer/LeakSanitizer does not report it (glibc still holds the stream in its list of open files). The leak is real even though the tool is silent.
- `std::exit()` does not run the destructors of local variables, only those of static/global objects (tested).

### Open points
- This folder has no number and no Previous/Next links.
- The Java (try-with-resources) and C# (`File.OpenRead`) snippets are from memory; no Java or C# compiler was available to check them.
- The links in "Further Reading" were not opened. The cppreference URL comes from a search result; the Core Guidelines anchors (`#rr-raii`, `#re-raii`) come from the guidelines text.

---

## Further Reading

### C++ Standard:
- Smart Pointers (C++11): `unique_ptr`, `shared_ptr`, `weak_ptr`
- RAII in STL: Containers, streams, locks

### Books:
- "Effective C++" by Scott Meyers
- "C++ Core Guidelines" edited by Bjarne Stroustrup and Herb Sutter
- "The C++ Programming Language" by Bjarne Stroustrup

### Online Resources:
- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
  - [R.1: Manage resources automatically using resource handles and RAII](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rr-raii)
  - [E.6: Use RAII to prevent leaks](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#re-raii)
- [cppreference.com - RAII](https://en.cppreference.com/cpp/language/raii)

---

## Conclusion

RAII is not just a technique - it's the **fundamental philosophy** of C++ resource management. It's what makes C++ unique among programming languages and enables low-overhead abstraction with strong safety guarantees.

**Master RAII, and you master modern C++!** 🚀

---

*Last Updated: 2024*
*C++ Version: C++11 and later*