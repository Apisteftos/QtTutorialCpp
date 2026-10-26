# RAII - Complete C++ Guide
### Resource Acquisition Is Initialization

A comprehensive, example-driven guide to understanding and using RAII, one of C++'s most powerful programming idioms.

## 📋 Overview

RAII (Resource Acquisition Is Initialization) is a C++ programming technique where resource lifetime is bound to object lifetime. This tutorial provides 15 examples showing why RAII is essential, how to use it correctly, and how it makes your code safer and cleaner. Examples 2-9 and 15 are runnable programs in `main.cpp`; examples 1 and 10-14 are text only and live in this README (see [Notes moved from main.cpp](#-notes-moved-from-maincpp)).

## 🎯 What You'll Learn

### Core Concepts
- **What is RAII?** - Tying resource lifetime to object lifetime
- **The Problem** - Why manual resource management fails
- **The Solution** - Constructor acquires, destructor releases
- **Compiler Guarantees** - The destructor of a fully constructed object runs when it goes out of scope, also when an exception is thrown (not on `std::abort()`, and not for local variables when `std::exit()` is called - see [When Destructor is Called](#when-destructor-is-called))

### Key Benefits
- ✅ **Automatic Cleanup** - Can't forget to release resources
- ✅ **Exception Safe** - Cleanup happens even when exceptions thrown
- ✅ **Low Overhead** - No garbage collector and no extra runtime machinery; you pay for the destructor call you would write by hand
- ✅ **Correct Order** - Destruction in reverse of construction
- ✅ **Local Reasoning** - Clear ownership semantics

### Advanced Topics
- 🔒 **Mutex Locking** - Exception-safe synchronization
- 📁 **File Handling** - Automatic file closure
- 🎯 **Smart Pointers** - Memory management
- 🗃️ **Database Connections** - Transaction safety
- 🛡️ **Scope Guards** - Generic cleanup wrappers
- 🔄 **State Management** - Temporary state changes

## 📚 Examples Covered

### Example 1: The Problem - No RAII
Demonstrates the five main problems with manual resource management:
1. **Memory leaks** - Forgetting to delete
2. **Exception unsafe** - Cleanup skipped when exceptions thrown
3. **Early return leaks** - Cleanup bypassed on early exit
4. **Complex cleanup order** - Easy to get wrong
5. **Multiple error paths** - Code duplication

The five problems in code:
```cpp
// 1. Memory leak
void leak() {
    int* data = new int[1000];
    processData(data);
    // Forgot delete[]!
}

// 2. Exception unsafe
void unsafe() {
    int* data = new int[1000];
    processData(data);   // What if this throws?
    delete[] data;       // Never reached!
}

// 3. Early return leak
void early() {
    int* data = new int[1000];
    if (error) return;   // Early exit
    delete[] data;       // Never reached!
}

// 4. Complex cleanup order
void order() {
    FILE* f1 = fopen("a.txt", "r");
    FILE* f2 = fopen("b.txt", "r");
    int* data = new int[1000];
    delete[] data;       // Cleanup has to be in reverse order...
    fclose(f2);          // ...at least when a later resource
    fclose(f1);          // depends on an earlier one
}

// 5. Multiple error paths
void paths() {
    int* data = new int[1000];
    if (error1) { delete[] data; return; }
    if (error2) { delete[] data; return; }
    if (error3) { delete[] data; return; }
    delete[] data;       // Repetitive and error-prone
}
```
RAII turns every one of these into a one-liner: the destructor does the cleanup on every path.

### Example 2: RAII Solution - Basic
Introduction to the RAII pattern:
```cpp
class SimpleRAII {
    int* data;
public:
    explicit SimpleRAII(size_t size) {
        data = new int[size];  // Acquire
    }
    ~SimpleRAII() {
        delete[] data;  // Release
    }
    // Delete copy to prevent double-delete
    SimpleRAII(const SimpleRAII&) = delete;
    SimpleRAII& operator=(const SimpleRAII&) = delete;
};
```

Benefits demonstrated:
- Normal scope exit → automatic cleanup ✅
- Exception thrown → automatic cleanup ✅
- Early return → automatic cleanup ✅

### Example 3: File Handle RAII
Wrapping C-style file handles:
```cpp
class FileHandle {
    FILE* file;
public:
    FileHandle(const char* name, const char* mode) {
        file = fopen(name, mode);
        if (!file) throw runtime_error("Failed to open: " + string(name));
    }
    ~FileHandle() {
        if (file) fclose(file);
    }
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;
    FILE* get() const { return file; }
};

// Without RAII
FILE* f = fopen("file.txt", "w");
processFile(f);   // If this throws...
fclose(f);        // ...never reached!

// With RAII
FileHandle f("file.txt", "w");
processFile(f.get());   // If this throws, the destructor still closes the file
```
(The version in `main.cpp` also has a move constructor, so a `FileHandle` can be returned from a function.)

Shows how RAII makes file handling exception-safe and prevents resource leaks.

### Example 4: Mutex Lock RAII
Thread-safe locking with RAII:
```cpp
void threadSafeFunction() {
    lock_guard<mutex> lock(mtx);  // Lock acquired
    // Critical section
    // Lock automatically released!
}
```

Demonstrates:
- `lock_guard` - Simple RAII lock
- `unique_lock` - Flexible RAII lock
- `shared_lock` - Reader lock (C++14); it needs a mutex with `lock_shared()`, i.e. `shared_mutex` (C++17) or `shared_timed_mutex` (C++14). `shared_lock<mutex>` does not compile (tested: `'std::mutex' has no member named 'lock_shared'`)
- `scoped_lock` - Multiple mutexes (C++17)

### Example 5: Smart Pointers as RAII
Memory management through RAII:
```cpp
// ❌ Without RAII
int* ptr = new int(42);
process(ptr);  // If throws, leak!
delete ptr;

// ✅ With RAII
auto ptr = make_unique<int>(42);
process(ptr.get());
// Automatic deletion!
```

Covers:
- `unique_ptr` - Exclusive ownership
- `shared_ptr` - Shared ownership
- `vector`, `string` - RAII containers
- `weak_ptr` - Non-owning reference
- `auto_ptr` - Removed from the standard in C++17, don't use it (libstdc++ still ships it and only warns: `'std::auto_ptr' is deprecated`, tested with GCC 13 in `-std=c++17` and `-std=c++20`)

### Example 6: Database Connection RAII
Managing database resources:
```cpp
class DatabaseConnection {
public:
    explicit DatabaseConnection(const string& connStr) {
        // Open connection
    }
    ~DatabaseConnection() {
        // Close connection
    }
    void beginTransaction();
    void commit();
    void rollback();
};

class Transaction {
    DatabaseConnection& db;
    bool committed = false;
public:
    explicit Transaction(DatabaseConnection& conn) : db(conn) {  // a reference member MUST be initialized here
        db.beginTransaction();
    }
    ~Transaction() {
        if (!committed) db.rollback();  // Auto-rollback!
    }
    Transaction(const Transaction&) = delete;
    Transaction& operator=(const Transaction&) = delete;
    void commit() {
        db.commit();
        committed = true;
    }
};
```
> The first version of this snippet had `Transaction(DatabaseConnection& conn) { db.beginTransaction(); }`, which does not compile: `uninitialized reference member in 'struct DatabaseConnection&'` (tested).

Demonstrates automatic transaction rollback on exceptions.

### Example 7: Scope Guard
Generic RAII wrapper for any cleanup:
```cpp
template<typename Func>
class ScopeGuard {
    Func cleanup;
    bool dismissed = false;
public:
    explicit ScopeGuard(Func f) : cleanup(std::move(f)) {}
    ~ScopeGuard() {
        if (!dismissed) cleanup();
    }
    void dismiss() { dismissed = true; }
    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;
};

template<typename Func>
ScopeGuard<Func> makeScopeGuard(Func f) {
    return ScopeGuard<Func>(std::move(f));  // C++17: no copy/move needed (guaranteed elision)
}

// Usage
auto guard = makeScopeGuard([]() {
    cleanupResource();
});
// Cleanup runs automatically!
```
Guards are destroyed in reverse order of creation (the demo prints `ptr3`, `ptr2`, `ptr1`). The cleanup function must not throw: the destructor is `noexcept`, so a throw would call `std::terminate`.

Perfect for:
- C API resources (malloc/free, fopen/fclose)
- Temporary state changes
- Complex error paths
- Non-RAII libraries

### Example 8: State Management RAII
Managing temporary state changes:
```cpp
class BoolGuard {
    bool& flag;
    bool oldValue;
public:
    BoolGuard(bool& f, bool newVal) : flag(f), oldValue(f) {
        flag = newVal;
    }
    ~BoolGuard() {
        flag = oldValue;  // Restore!
    }
};
```

Use cases:
- Scoped flag changes
- Indent level management
- Debug mode toggles
- Profiler scopes

### Example 9: Construction Order
Understanding construction and destruction order:
```cpp
class Database {
    FileHandle logFile;      // Constructed FIRST
    Connection connection;   // Constructed SECOND
    // Destroyed in reverse: connection, then logFile
};
```

Critical rules:
- Members constructed in declaration order
- Destructed in reverse order
- Declare dependencies first

Members are constructed in declaration order, not in the order of the constructor's initializer list (GCC warns about a different order with `-Wreorder`). Why this matters:
```cpp
class Database {
    FileHandle logFile;      // Constructed FIRST
    Connection connection;   // Constructed SECOND (can use logFile)
};
// Destruction: connection first (it can still log its closing), then logFile
```

### Example 10: Exception Safety Levels
Four levels of exception safety:

1. **No Guarantee** ❌ - Resources may leak
2. **Basic Guarantee** ⚠️ - No leaks, valid state (RAII covers the "no leaks" half; keeping the object's invariants intact is still your job)
3. **Strong Guarantee** ✅ - All or nothing (rollback)
4. **No-Throw** 🌟 - Never throws (noexcept)

RAII gives you the no-leak part of the basic guarantee for free.

### Example 11: Common RAII Patterns
Standard library RAII types:

**Memory Management:**
- `unique_ptr<T>`, `shared_ptr<T>`
- `vector<T>`, `string` (`std::array` owns no heap memory, so it is not RAII in this sense)

**File Handling:**
- `fstream`, `ifstream`, `ofstream`

**Locking:**
- `lock_guard<mutex>`
- `unique_lock<mutex>`
- `shared_lock<shared_mutex>`
- `scoped_lock` (C++17)

**Thread Management:**
- `jthread` (C++20) - joins in its destructor
- `thread` is **not** RAII: destroying a joinable `std::thread` calls `std::terminate` (tested). Join it yourself or wrap it in a guard

**Custom Wrappers:**
- Database connections
- Network sockets
- GPU resources
- OS handles

### Example 12: Common Mistakes
What to avoid:

1. **Forgetting destructor** - Resource never released
2. **Allowing copies** - Double deletion
3. **Throwing destructor** - Destructors are `noexcept` by default (C++11), so a throw that leaves one calls `std::terminate()` right away (tested)
4. **Unsafe multi-resource acquisition** - First leaks if second throws
5. **Not checking null** - Crash in destructor
6. **Using after move** - Moved-from object is empty

### Example 13: Best Practices
Comprehensive guide to correct RAII usage:

1. **Acquire in constructor** - Get resource or throw
2. **Release in destructor** - Mark noexcept
3. **Disable copy or deep copy** - Prevent double-delete
4. **One resource per class** - Simplifies exception safety
5. **Prefer standard types** - Use existing RAII classes
6. **Member order matters** - Dependencies first
7. **Make destructors noexcept** - Never throw in destructor
8. **Use scope guards for C APIs** - Wrap non-RAII resources
9. **Document ownership** - Clear semantics
10. **Test exception paths** - Verify no leaks

### Example 14: RAII vs Manual Comparison
Side-by-side comparison showing:

**Manual (25 lines, error-prone):**
```cpp
void badFunction() {
    FILE* f1 = fopen("a.txt", "r");
    if (!f1) return;
    
    FILE* f2 = fopen("b.txt", "r");
    if (!f2) {
        fclose(f1);  // Must remember!
        return;
    }
    
    int* data = new int[1000];
    
    if (error) {
        delete[] data;
        fclose(f2);
        fclose(f1);
        return;
    }
    
    process(data);  // If throws? Leak!
    
    delete[] data;
    fclose(f2);
    fclose(f1);
}
```

**RAII (15 lines, safe):**
```cpp
void goodFunction() {
    ifstream f1("a.txt");
    if (!f1) return;
    
    ifstream f2("b.txt");
    if (!f2) return;
    
    vector<int> data(1000);
    
    if (error) return;  // Automatic cleanup!
    
    process(data.data());  // Exception safe!
    
    // No manual cleanup needed!
}
```

**Result: 10 fewer lines (about 40% less) and no cleanup code to forget** (line counts include braces and blank lines of the two snippets above)

### Example 15: Real-World Example
Complete real-world scenario combining multiple RAII types:
```cpp
void processFiles() {
    Timer timer("Processing");        // Timing RAII
    ifstream input("data.txt");       // File RAII
    ofstream output("result.txt");    // File RAII
    lock_guard<mutex> lock(mtx);      // Lock RAII
    
    // Process data...
    
    // Cleanup in reverse order of construction:
    // lock released, output closed, input closed, then the timer reports
}
```

## 🔧 Compilation & Usage

### Requirements
- C++23 (`main.cpp` uses `std::println` from `<print>`), so GCC 14+ or a current Clang with a C++23 standard library (versions from memory, not checked)
- Pthread support (for the mutex/thread example)

### Compile
```bash
g++ -std=c++23 -Wall -Wextra -pthread main.cpp -o raii
```

### Run
```bash
./raii
```

### Expected Output
The program runs examples 2-9 and 15 in order:
- Clear section headers with box-drawing
- Emoji indicators for important points
- The order of the `Thread N completed` lines in example 4 changes from run to run (the final counter is always 50)
- The files `/tmp/test_raii.txt`, `/tmp/test_exception.txt` and `/tmp/test_output.txt` are created

## 📊 Key Takeaways

### The RAII Pattern

```cpp
class RAIIWrapper {
    Resource* resource;
    
public:
    // Constructor: ACQUIRE resource
    RAIIWrapper() {
        resource = acquireResource();
        if (!resource) throw exception();
    }
    
    // Destructor: RELEASE resource
    ~RAIIWrapper() noexcept {
        if (resource) {
            releaseResource(resource);
        }
    }
    
    // Delete copy (or implement deep copy)
    RAIIWrapper(const RAIIWrapper&) = delete;
    RAIIWrapper& operator=(const RAIIWrapper&) = delete;
    
    // Allow move (optional)
    RAIIWrapper(RAIIWrapper&& other) noexcept {
        resource = other.resource;
        other.resource = nullptr;
    }
};
```

### When Destructor is Called

✅ **Called for a fully constructed object when:**
- Normal scope exit
- Exception thrown (stack unwinding)
- Early return statement
- `goto` out of the scope (tested)
- Parent object destroyed
- Static/global objects: at normal program exit (also after `std::exit()`)

❌ **NOT called:**
- `std::abort()` - no destructors run at all (tested)
- `std::terminate()` - the destructors of local objects do not run (tested with GCC 13)
- `std::exit()` - destroys static/global objects but NOT the locals of the functions that are still active (tested)
- If the constructor threw - that object never existed (its already-built members and bases are destroyed)

### Exception Safety Guarantees

| Level | Description | RAII Helps? |
|-------|-------------|-------------|
| **No Guarantee** | Leaks possible | ❌ Don't do this |
| **Basic** | No leaks, valid state | ✅ RAII provides this! |
| **Strong** | All or nothing | ✅ Build on RAII |
| **No-Throw** | Never throws | ✅ For destructors |

### Standard RAII Types

#### Memory
```cpp
unique_ptr<T>         // Exclusive ownership
shared_ptr<T>         // Shared ownership
vector<T>             // Dynamic array
string                // Dynamic string
// std::array owns no heap memory - not RAII in this sense
```

#### Files
```cpp
ifstream              // Input file
ofstream              // Output file
fstream               // Input/output file
```

#### Locking
```cpp
lock_guard<mutex>         // Simple lock
unique_lock<mutex>        // Flexible lock
shared_lock<shared_mutex> // Reader lock (needs a shared mutex type)
scoped_lock               // Multiple mutexes at once (C++17)
```

#### Threads
```cpp
jthread               // Joins in its destructor (C++20)
// std::thread is NOT RAII: a joinable thread that is destroyed calls std::terminate
```

### Custom RAII Template

```cpp
template<typename ResourceType, typename ReleaseFunc>
class GenericRAII {
    ResourceType resource;
    ReleaseFunc releaser;
    
public:
    GenericRAII(ResourceType res, ReleaseFunc release)
        : resource(res), releaser(std::move(release)) {
        if (!resource) throw runtime_error("acquire failed");
    }
    
    ~GenericRAII() noexcept {
        if (resource) releaser(resource);
    }
    
    // Delete copy
    GenericRAII(const GenericRAII&) = delete;
    GenericRAII& operator=(const GenericRAII&) = delete;
    
    ResourceType get() const { return resource; }
};

// Usage (C++17: the template arguments are deduced)
GenericRAII file(fopen("file.txt", "r"), [](FILE* f) { fclose(f); });
```
> The first version took an `AcquireFunc` as a third template parameter and was used as `GenericRAII<FILE*>(...)`, which does not compile: `wrong number of template arguments (1, should be 3)` (tested). It also threw an unqualified `exception()`.

## 🚫 Common Pitfalls

### 1. Forgetting the Destructor
```cpp
// ❌ WRONG
class BadRAII {
    int* data;
public:
    BadRAII() : data(new int[100]) {}
    // No destructor! LEAK!
};

// ✅ CORRECT
class GoodRAII {
    int* data;
public:
    GoodRAII() : data(new int[100]) {}
    ~GoodRAII() { delete[] data; }
};
```

### 2. Shallow Copy Problem
```cpp
// ❌ WRONG - Default copy is shallow
class BadRAII {
    int* data;
public:
    BadRAII() : data(new int) {}
    ~BadRAII() { delete data; }
    // Double delete when copied!
};

// ✅ CORRECT - Delete copy or deep copy
class GoodRAII {
    int* data;
public:
    GoodRAII() : data(new int) {}
    ~GoodRAII() { delete data; }
    GoodRAII(const GoodRAII&) = delete;
    GoodRAII& operator=(const GoodRAII&) = delete;
};
```

### 3. Throwing Destructor
```cpp
// ❌ WRONG - Destructor can throw
~BadRAII() {
    cleanup();  // Might throw!
    // Destructors are noexcept by default (C++11), so the program calls
    // std::terminate() right away - not only "during stack unwinding" (tested)
}

// ✅ CORRECT - Never throw from destructor
~GoodRAII() noexcept {
    try {
        cleanup();
    } catch (...) {
        // Log error, but don't throw
    }
}
```

### 4. Multi-Resource Acquisition
```cpp
// ❌ WRONG - Second allocation leaks if third throws
class BadRAII {
    int* p1;
    int* p2;
    int* p3;
public:
    BadRAII() {
        p1 = new int;  // OK
        p2 = new int;  // OK
        p3 = new int;  // If throws, p1 & p2 leak!
    }
};

// ✅ CORRECT - Use RAII for each resource
class GoodRAII {
    unique_ptr<int> p1;  // RAII!
    unique_ptr<int> p2;  // RAII!
    unique_ptr<int> p3;  // RAII!
public:
    GoodRAII() 
        : p1(make_unique<int>())
        , p2(make_unique<int>())
        , p3(make_unique<int>()) {
        // If any throws, previous ones cleaned up!
    }
};
```

## 🎓 Learning Path

### Beginner
1. Understand the problem (Example 1)
2. Learn basic pattern (Example 2)
3. See file example (Example 3)
4. Try smart pointers (Example 5)

### Intermediate
5. Master lock guards (Example 4)
6. Study scope guards (Example 7)
7. Learn construction order (Example 9)
8. Review common mistakes (Example 12)

### Advanced
9. Database patterns (Example 6)
10. State management (Example 8)
11. Exception safety (Example 10)
12. Best practices (Example 13)
13. Real-world integration (Example 15)

## 📖 Related Topics

- **Smart Pointers** - `unique_ptr`, `shared_ptr`, `weak_ptr`
- **Move Semantics** - Efficient resource transfer
- **Rule of Five** - Special member functions
- **Exception Safety** - Strong guarantee, no-throw
- **Copy-and-Swap** - Exception-safe assignment
- **Custom Deleters** - Specialized cleanup

## 🔗 Design Philosophy

### The Core Principle
**"Resource acquisition is initialization"**
- Acquire resource → Constructor succeeds
- Constructor fails → No object created
- Object exists → Resource is valid
- Object destroyed → Resource released

### Why This Works
1. **Compiler guaranteed** - Destructors run on every normal exit path (see "When Destructor is Called")
2. **Stack-based** - Automatic lifetime management
3. **Exception-safe** - Unwinding calls destructors
4. **Low overhead** - No garbage collector; you pay for the destructor call you would write by hand
5. **Local reasoning** - Ownership is clear

### Comparison to Other Languages

**C:** Manual management
- Explicit malloc/free
- Easy to forget
- No automatic cleanup (no destructors)

**Java/C#:** Garbage collection
- Automatic memory
- Non-deterministic cleanup
- Non-memory resources need try-with-resources / `using` / try-finally (the programmer has to remember)

**Rust:** Ownership system
- Similar to RAII
- Compile-time enforced
- Drop trait = destructor

**Python:** Context managers (`with`)
- Similar concept
- Runtime overhead
- `__enter__` and `__exit__`

**C++ RAII:** (my opinion) a strong combination
- No garbage collector
- Destructor calls are inserted by the compiler
- Works for ANY resource
- Exception-safe by design

## 💡 Pro Tips

1. **Default to RAII**: Any resource → wrap in RAII class
2. **One resource per class**: Simplifies exception safety
3. **Prefer standard types**: Don't reinvent smart pointers
4. **Member order matters**: Dependencies first, used resources last
5. **Mark destructors noexcept**: Should never throw
6. **Test with exceptions**: Verify no leaks in error paths
7. **Use scope guards**: Perfect for C APIs
8. **Document ownership**: Make it crystal clear
9. **Leverage move**: Enable efficient transfers
10. **Think in lifetimes**: Match resource lifetime to object lifetime

## 📝 Notes moved from main.cpp

`main.cpp` runs examples 2-9 and 15. Examples 1 and 10-14 were only printed text (code listings and bullet lists), so they were removed from the program; most of their content was already in the sections above. What was only in `main.cpp`:

### Example 12 - mistakes 5 and 6 (code)
```cpp
// Mistake 5: no null check in the destructor
~BadRAII()  { fclose(file); }              // fclose(nullptr) crashes (tested: segmentation fault with glibc)
~GoodRAII() { if (file) fclose(file); }    // Check before cleanup
```
If the constructor throws on failure, the destructor never runs, so the check only matters when a "null state" is possible (for example a moved-from object).
```cpp
// Mistake 6: using an RAII object after it was moved from
unique_ptr<int> p1 = make_unique<int>(42);
unique_ptr<int> p2 = move(p1);
// *p1;   // p1 is nullptr after the move - dereferencing it is undefined behavior
```

### Example 10 - the four levels in code
```cpp
// 1. No guarantee: p leaks if doWork() throws
void func() { int* p = new int; doWork(); delete p; }

// 2. Basic guarantee: no leaks
void func() { unique_ptr<int> p = make_unique<int>(); doWork(); }

// 3. Strong guarantee: all or nothing (copy-and-swap)
MyClass& operator=(const MyClass& other) {
    MyClass temp(other);   // May throw - *this is untouched
    swap(temp);            // Never throws
    return *this;
}

// 4. No-throw guarantee
void swap(MyClass& other) noexcept { /* ... */ }
```

### Example 5 and the thread claims
The old text listed `auto_ptr` as "deprecated" (it was removed in C++17) and `thread with join in destructor` as an RAII type (a plain `std::thread` is not; `jthread` is). Both are corrected above.

### Things found while testing (GCC 13)
- `shared_lock<mutex>` does not compile (needs `lock_shared()`); the README listed it as a reader lock.
- The `Transaction` snippet (uninitialized reference member) and the `GenericRAII` template/usage did not compile; both are fixed above.
- Destroying a joinable `std::thread` aborts with `terminate called without an active exception`.
- Throwing out of a destructor calls `terminate` at once, not only during unwinding.
- `std::abort()` and `std::terminate()` skip local destructors, `std::exit()` skips the locals but runs static/global ones, and `goto` out of a scope runs the destructor.
- In example 15 the comment listed the cleanup order wrongly (timer first); it is the reverse of construction: lock, output file, input file, timer.
- The "18 lines vs 8 lines (55% less)" claim did not match the snippets (25 vs 15 lines, about 40%); "zero bugs" was removed.
- The original `main.cpp` compiled without warnings and was clean under AddressSanitizer, UBSan and ThreadSanitizer; the changes there are style only (see the chat summary).

### Open points
- This folder has no number and no Previous/Next links, and overlaps with the other RAII folder.
- The comparison with Java/C#/Rust/Python is from memory, not tested.
- The Stroustrup quote at the end of the old README (`"RAII: The difference between C and C++, and the reason we have destructors." - Bjarne Stroustrup (paraphrased)`) could not be verified, so it was removed.
- `bool` values in example 8 now print as `true`/`false` (they printed `1`/`0` with `cout`).

---

## 🤝 Contributing

This is a learning resource. Suggestions welcome for:
- Additional examples
- Clearer explanations
- Real-world use cases
- Common pattern additions

## 📝 License

Educational material provided as-is for learning purposes.

---

## 🎯 Quick Reference Card

### RAII Checklist
- [ ] Acquire in constructor
- [ ] Release in destructor (noexcept)
- [ ] Delete copy or implement deep copy
- [ ] Support move if appropriate
- [ ] One resource per class
- [ ] Check for null before cleanup
- [ ] Members in dependency order
- [ ] Test exception paths
- [ ] Document ownership
- [ ] Prefer standard RAII types

### Decision Tree
```
Need to manage a resource?
├─ Is it memory?
│  ├─ Single owner? → unique_ptr
│  ├─ Shared owner? → shared_ptr
│  └─ Dynamic size? → vector
├─ Is it a file?
│  └─ Use fstream
├─ Is it a lock?
│  └─ Use lock_guard
├─ Is it something else?
│  ├─ Standard library has RAII? → Use it
│  └─ Write custom RAII wrapper
└─ Can't use RAII?
   └─ Use scope guard!
```

---

**Happy Coding! 🚀**

Remember: If you acquire a resource, RAII should own it!