#include <print>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <mutex>
#include <thread>
#include <chrono>
#include <cstddef>
#include <cstdio>
#include <stdexcept>
#include <utility>

// ============================================
// WHAT IS RAII?
// ============================================

/*
RAII = Resource Acquisition Is Initialization

THE BIG IDEA:
    TIE RESOURCE LIFETIME TO OBJECT LIFETIME
    - Acquire resource in constructor
    - Release resource in destructor
    - Automatic cleanup when object goes out of scope
    - Exception-safe by design

THE PROBLEM WITHOUT RAII:
    Manual resource management = BUGS!
    1. Memory leaks (forget to delete)
    2. Resource leaks (forget to close file)
    3. Exception unsafe (cleanup code skipped)
    4. Double deletion/close
    5. Order dependencies (cleanup in wrong order)

THE RAII SOLUTION:
    Constructor:  Acquire resource
    Destructor:   Release resource
    Compiler:     Guarantees destructor is called!

    When object destroyed:
    - Normal scope exit      → destructor called ✅
    - Exception thrown       → destructor called ✅
    - Early return           → destructor called ✅
    - goto statement         → destructor called ✅

COMMON RAII TYPES IN C++:
    - unique_ptr, shared_ptr    (memory)
    - fstream                   (files)
    - lock_guard, unique_lock   (mutexes)
    - vector, string            (dynamic arrays)
    - jthread (C++20)           (joins in its destructor; std::thread is NOT RAII:
                                 destroying a joinable std::thread calls std::terminate)
    - Custom RAII wrappers      (any resource!)

KEY PRINCIPLE:
    "If you acquire it, RAII should own it!"

    Resource = Anything that needs cleanup:
    - Memory (new/delete)
    - Files (open/close)
    - Locks (lock/unlock)
    - Sockets (open/close)
    - Database connections
    - GPU resources
    - OS handles
*/

// ============================================
// EXAMPLE 2: RAII SOLUTION - BASIC
// ============================================

class SimpleRAII {
private:
    int* data;
    std::size_t size;
    std::string name;

public:
    // Constructor: ACQUIRE resource
    SimpleRAII(std::size_t s, const std::string& n) : size(s), name(n) {
        data = new int[size];
        std::println("✅ [{}] ACQUIRED: {} ints", name, size);
    }

    // Destructor: RELEASE resource
    ~SimpleRAII() {
        delete[] data;
        std::println("💥 [{}] RELEASED: {} ints", name, size);
    }

    // Delete copy to prevent double-delete
    SimpleRAII(const SimpleRAII&) = delete;
    SimpleRAII& operator=(const SimpleRAII&) = delete;

    int* get() { return data; }
};

void demonstrateBasicRaii() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: RAII SOLUTION          ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Normal scope exit ---");
    {
        SimpleRAII obj(100, "Obj1");
        std::println("   Using object...");
        // No manual cleanup needed!
    }  // ← Destructor automatically called here!
    std::println("   ✅ Automatic cleanup!");

    std::println("");
    std::println("--- Exception safety ---");
    try {
        SimpleRAII obj(200, "Obj2");
        std::println("   Before exception...");
        throw std::runtime_error("Oops!");
        std::println("   This never executes");
    } catch (const std::exception& e) {
        std::println("   Caught: {}", e.what());
        std::println("   ✅ Object was automatically cleaned up!");
    }

    std::println("");
    std::println("--- Early return ---");
    auto testEarlyReturn = []() {
        SimpleRAII obj(300, "Obj3");
        std::println("   Before early return...");
        return;  // Early exit!
        std::println("   This never executes");
    };
    testEarlyReturn();
    std::println("   ✅ Automatic cleanup on early return!");
}

// ============================================
// EXAMPLE 3: FILE HANDLE RAII
// ============================================

class FileHandle {
private:
    std::FILE* file;
    std::string filename;

public:
    FileHandle(const std::string& fname, const char* mode) : filename(fname) {
        file = std::fopen(fname.c_str(), mode);
        if (!file) {
            throw std::runtime_error("Failed to open: " + fname);
        }
        std::println("✅ Opened file: {}", filename);
    }

    ~FileHandle() {
        if (file) {
            std::fclose(file);
            std::println("💥 Closed file: {}", filename);
        }
    }

    // Delete copy
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;

    // Allow move
    FileHandle(FileHandle&& other) noexcept
        : file(other.file), filename(std::move(other.filename)) {
        other.file = nullptr;
    }

    std::FILE* get() const { return file; }

    void write(const std::string& data) {
        if (file) {
            std::fprintf(file, "%s", data.c_str());
        }
    }
};

void demonstrateFileRaii() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: FILE HANDLE RAII       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating test file ---");
    {
        FileHandle file("/tmp/test_raii.txt", "w");
        file.write("Hello RAII!\n");
        file.write("Automatic cleanup!\n");
        std::println("   File written");
    }  // File automatically closed here!
    std::println("   ✅ File handle cleaned up!");

    std::println("");
    std::println("--- Exception safety test ---");
    try {
        FileHandle file("/tmp/test_exception.txt", "w");
        file.write("Before exception\n");
        throw std::runtime_error("Test exception");
    } catch (const std::exception& e) {
        std::println("   Caught: {}", e.what());
        std::println("   ✅ File was automatically closed!");
    }
}

// ============================================
// EXAMPLE 4: MUTEX LOCK RAII
// ============================================

class SharedResource {
private:
    int counter = 0;
    mutable std::mutex mtx;

public:
    void incrementBad() {
        // ❌ BAD: Manual lock/unlock
        mtx.lock();
        counter++;
        // If exception here, mutex stays locked! 💥
        mtx.unlock();  // Might never execute!
    }

    void incrementGood() {
        // ✅ GOOD: RAII with lock_guard
        std::lock_guard<std::mutex> lock(mtx);
        counter++;
        // Mutex automatically unlocked on scope exit!
    }

    int getCounter() const {
        std::lock_guard<std::mutex> lock(mtx);
        return counter;
    }
};

void demonstrateLockRaii() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: MUTEX LOCK RAII        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Testing thread-safe increment ---");
    SharedResource resource;

    // Spawn threads
    std::vector<std::thread> threads;
    for (int i = 0; i < 5; i++) {
        threads.emplace_back([&resource, i]() {
            for (int j = 0; j < 10; j++) {
                resource.incrementGood();
            }
            std::println("   Thread {} completed", i);
        });
    }

    // Wait for all threads
    for (auto& t : threads) {
        t.join();
    }

    std::println("   Final counter: {}", resource.getCounter());
    std::println("   ✅ All locks automatically released!");
}

// ============================================
// EXAMPLE 5: SMART POINTERS AS RAII
// ============================================

void demonstrateSmartPointerRaii() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 5: SMART POINTERS (RAII)  ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- unique_ptr example ---");
    {
        auto ptr = std::make_unique<int>(100);
        std::println("   Created unique_ptr: {}", *ptr);
    }  // Automatically deleted here!
    std::println("   ✅ Memory automatically freed!");

    std::println("");
    std::println("--- shared_ptr example ---");
    {
        auto ptr1 = std::make_shared<int>(200);
        std::println("   ptr1 use_count: {}", ptr1.use_count());
        {
            auto ptr2 = ptr1;
            std::println("   ptr2 created, use_count: {}", ptr1.use_count());
        }
        std::println("   ptr2 destroyed, use_count: {}", ptr1.use_count());
    }  // Memory freed when last owner destroyed!
    std::println("   ✅ Reference counting + automatic cleanup!");

    std::println("");
    std::println("--- vector example (RAII container) ---");
    {
        std::vector<int> vec = {1, 2, 3, 4, 5};
        std::println("   Vector size: {}", vec.size());
    }  // Internal array automatically freed!
    std::println("   ✅ Dynamic array automatically freed!");
}

// ============================================
// EXAMPLE 6: CUSTOM RAII - DATABASE
// ============================================

class DatabaseConnection {
private:
    std::string connectionString;
    bool connected;

public:
    explicit DatabaseConnection(const std::string& connStr)
        : connectionString(connStr), connected(false) {
        // Acquire: Open database connection
        std::println("✅ Opening DB: {}", connectionString);
        connected = true;
        // Simulate connection
    }

    ~DatabaseConnection() {
        if (connected) {
            // Release: Close database connection
            std::println("💥 Closing DB: {}", connectionString);
            connected = false;
        }
    }

    // Delete copy
    DatabaseConnection(const DatabaseConnection&) = delete;
    DatabaseConnection& operator=(const DatabaseConnection&) = delete;

    void executeQuery(const std::string& query) {
        if (connected) {
            std::println("   Executing: {}", query);
        }
    }

    void beginTransaction() {
        std::println("   BEGIN TRANSACTION");
    }

    void commit() {
        std::println("   COMMIT");
    }

    void rollback() {
        std::println("   ROLLBACK");
    }
};

class Transaction {
private:
    DatabaseConnection& db;
    bool committed;

public:
    explicit Transaction(DatabaseConnection& conn) : db(conn), committed(false) {
        db.beginTransaction();
    }

    Transaction(const Transaction&) = delete;
    Transaction& operator=(const Transaction&) = delete;

    ~Transaction() {
        if (!committed) {
            db.rollback();  // Auto-rollback if not committed!
        }
    }

    void commit() {
        db.commit();
        committed = true;
    }
};

void demonstrateDatabaseRaii() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 6: DATABASE RAII          ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Successful transaction ---");
    {
        DatabaseConnection db("localhost:5432/mydb");
        {
            Transaction txn(db);
            db.executeQuery("INSERT INTO users VALUES (...)");
            db.executeQuery("UPDATE accounts SET ...");
            txn.commit();  // Explicit commit
        }  // Transaction object destroyed
    }  // Database connection closed
    std::println("   ✅ Connection automatically closed!");

    std::println("");
    std::println("--- Failed transaction (auto-rollback) ---");
    try {
        DatabaseConnection db("localhost:5432/mydb");
        {
            Transaction txn(db);
            db.executeQuery("INSERT INTO users VALUES (...)");
            throw std::runtime_error("Something went wrong!");
            txn.commit();  // Never reached!
        }  // txn destructor → automatic ROLLBACK!
    } catch (const std::exception& e) {
        std::println("   Caught: {}", e.what());
        std::println("   ✅ Transaction automatically rolled back!");
    }
}

// ============================================
// EXAMPLE 7: RAII WITH SCOPE GUARD
// ============================================

template<typename Func>
class ScopeGuard {
private:
    Func cleanup;
    bool dismissed;

public:
    explicit ScopeGuard(Func f) : cleanup(std::move(f)), dismissed(false) {}

    ~ScopeGuard() {
        if (!dismissed) {
            cleanup();
        }
    }

    void dismiss() {
        dismissed = true;
    }

    // Delete copy (this also suppresses the implicit move)
    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;
};

template<typename Func>
ScopeGuard<Func> makeScopeGuard(Func f) {
    return ScopeGuard<Func>(std::move(f));
}

void demonstrateScopeGuard() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 7: SCOPE GUARD            ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Example 1: Cleanup on scope exit ---");
    {
        std::println("   Entering scope");
        auto guard = makeScopeGuard([]() {
            std::println("   💥 Cleanup executed!");
        });
        std::println("   Doing work...");
    }  // Guard destructor calls cleanup

    std::println("");
    std::println("--- Example 2: Multi-resource cleanup ---");
    {
        int* ptr1 = new int(10);
        int* ptr2 = new int(20);
        int* ptr3 = new int(30);

        auto g1 = makeScopeGuard([ptr1]() { delete ptr1; std::println("   💥 Freed ptr1"); });
        auto g2 = makeScopeGuard([ptr2]() { delete ptr2; std::println("   💥 Freed ptr2"); });
        auto g3 = makeScopeGuard([ptr3]() { delete ptr3; std::println("   💥 Freed ptr3"); });

        std::println("   All resources acquired");
    }  // All guards clean up in reverse order!

    std::println("");
    std::println("--- Example 3: Dismissible guard ---");
    {
        std::println("   Starting operation");
        auto guard = makeScopeGuard([]() {
            std::println("   💥 Rollback executed!");
        });

        // Do work...
        std::println("   Work completed successfully");
        guard.dismiss();  // Success! Don't rollback
    }
    std::println("   ✅ Rollback was dismissed");

    std::println("");
    std::println("--- Example 4: Exception safety ---");
    try {
        auto guard = makeScopeGuard([]() {
            std::println("   💥 Emergency cleanup!");
        });
        std::println("   Before exception...");
        throw std::runtime_error("Error!");
    } catch (const std::exception& e) {
        std::println("   Caught: {}", e.what());
        std::println("   ✅ Guard ensured cleanup!");
    }
}

// ============================================
// EXAMPLE 8: RAII FOR STATE MANAGEMENT
// ============================================

class BoolGuard {
private:
    bool& flag;
    bool oldValue;

public:
    BoolGuard(bool& f, bool newValue) : flag(f), oldValue(f) {
        flag = newValue;
        std::println("   ✅ Flag: {} → {}", oldValue, newValue);
    }

    ~BoolGuard() {
        flag = oldValue;
        std::println("   💥 Flag restored: {}", oldValue);
    }
};

class IndentGuard {
private:
    int& level;

public:
    explicit IndentGuard(int& lvl) : level(lvl) {
        level++;
    }

    ~IndentGuard() {
        level--;
    }
};

void demonstrateStateRaii() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 8: STATE MANAGEMENT RAII  ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Boolean flag guard ---");
    bool isProcessing = false;
    std::println("   Initial flag: {}", isProcessing);
    {
        BoolGuard guard(isProcessing, true);
        std::println("   Inside scope, flag: {}", isProcessing);
    }
    std::println("   After scope, flag: {}", isProcessing);

    std::println("");
    std::println("--- Indent level management ---");
    int indentLevel = 0;

    auto printIndented = [&](const std::string& msg) {
        std::println("{}{}", std::string(indentLevel * 2, ' '), msg);
    };

    printIndented("Function start");
    {
        IndentGuard guard1(indentLevel);
        printIndented("Level 1");
        {
            IndentGuard guard2(indentLevel);
            printIndented("Level 2");
            {
                IndentGuard guard3(indentLevel);
                printIndented("Level 3");
            }
            printIndented("Back to Level 2");
        }
        printIndented("Back to Level 1");
    }
    printIndented("Function end");
    std::println("   ✅ Indent automatically managed!");
}

// ============================================
// EXAMPLE 9: RAII CONSTRUCTION ORDER
// ============================================

class RAIIOrdered {
private:
    std::string name;

public:
    explicit RAIIOrdered(const std::string& n) : name(n) {
        std::println("   ✅ Construct: {}", name);
    }

    ~RAIIOrdered() {
        std::println("   💥 Destruct:  {}", name);
    }
};

void demonstrateConstructionOrder() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 9: CONSTRUCTION ORDER     ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Stack objects ---");
    {
        RAIIOrdered obj1("First");
        RAIIOrdered obj2("Second");
        RAIIOrdered obj3("Third");
        std::println("   All constructed");
    }  // Destroyed in reverse: Third, Second, First
    std::println("   ✅ Reverse order ensures correct cleanup!");
}

// ============================================
// EXAMPLE 15: REAL-WORLD EXAMPLE
// ============================================

class Timer {
private:
    std::string name;
    std::chrono::steady_clock::time_point start;

public:
    explicit Timer(const std::string& n) : name(n) {
        start = std::chrono::steady_clock::now();
        std::println("   ⏱️  [{}] Started", name);
    }

    ~Timer() {
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::println("   ⏱️  [{}] Completed: {}ms", name, duration.count());
    }
};

void demonstrateRealWorld() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 15: REAL-WORLD EXAMPLE    ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    {
        Timer timer("File Processing");

        // Open input file (RAII)
        std::ifstream input("/tmp/test_raii.txt");
        if (!input) {
            std::println("   ⚠️  File not found (expected)");
            return;
        }

        // Create output file (RAII)
        std::ofstream output("/tmp/test_output.txt");

        // Lock for thread-safe operation (RAII)
        static std::mutex mtx;
        std::lock_guard<std::mutex> lock(mtx);

        std::println("   Processing files...");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // All cleanup happens automatically, in reverse order of construction:
        // 1. Lock released
        // 2. Output file closed
        // 3. Input file closed
        // 4. Timer reports duration (constructed first, so destroyed last)
    }

    std::println("");
    std::println("   ✅ Everything cleaned up automatically!");
    std::println("   ✅ Even if exceptions occur!");
    std::println("   ✅ Correct cleanup order guaranteed!");
}

// ============================================
// MAIN - RUN ALL EXAMPLES
// ============================================

int main() {
    std::println("╔══════════════════════════════════════════════════════════╗");
    std::println("║                                                          ║");
    std::println("║                RAII - COMPLETE GUIDE                     ║");
    std::println("║         Resource Acquisition Is Initialization           ║");
    std::println("║                                                          ║");
    std::println("╚══════════════════════════════════════════════════════════╝");

    try {
        demonstrateBasicRaii();               // Basic RAII pattern
        demonstrateFileRaii();                // File handling
        demonstrateLockRaii();                // Mutex locking
        demonstrateSmartPointerRaii();        // Memory management
        demonstrateDatabaseRaii();            // Database connections
        demonstrateScopeGuard();              // Generic RAII wrapper
        demonstrateStateRaii();               // State management
        demonstrateConstructionOrder();       // Construction/destruction order
        demonstrateRealWorld();               // Real-world example

    } catch (const std::exception& e) {
        std::println("💥 Exception: {}", e.what());
    }

    return 0;
}

/*
COMPILATION:
    g++ -std=c++23 -Wall -Wextra -pthread main.cpp -o raii

RUN:
    ./raii

TOPICS COVERED (examples 2-9 and 15 run in main.cpp, the rest is in the README):
    ✅ What is RAII
    ✅ Why RAII is essential
    ✅ Problems without RAII
    ✅ Basic RAII pattern
    ✅ File handling RAII
    ✅ Mutex lock RAII
    ✅ Smart pointers as RAII
    ✅ Database connection RAII
    ✅ Scope guards
    ✅ State management RAII
    ✅ Construction/destruction order
    ✅ Exception safety levels
    ✅ Common RAII patterns
    ✅ Common mistakes
    ✅ Best practices
    ✅ RAII vs manual comparison
    ✅ Real-world examples

OUTPUT: Complete demonstrations with emojis and clear sections!
*/
