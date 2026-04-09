#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <mutex>
#include <thread>
#include <chrono>
using namespace std;

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
    - thread                    (OS threads)
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
// EXAMPLE 1: THE PROBLEM - NO RAII
// ============================================

void demonstrateProblemWithoutRaii() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: PROBLEMS WITHOUT RAII  ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "Problem 1: MEMORY LEAK\n";
    cout << "---\n";
    cout << "void badFunction() {\n";
    cout << "    int* data = new int[1000];\n";
    cout << "    processData(data);\n";
    cout << "    // Forgot to delete[]! 💥\n";
    cout << "}\n";
    cout << "Result: Memory leak! Data never freed.\n";
    cout << "\n";

    cout << "Problem 2: EXCEPTION UNSAFE\n";
    cout << "---\n";
    cout << "void badFunction() {\n";
    cout << "    int* data = new int[1000];\n";
    cout << "    processData(data);  // What if this throws?\n";
    cout << "    delete[] data;      // Never reached! 💥\n";
    cout << "}\n";
    cout << "Result: Exception bypasses cleanup!\n";
    cout << "\n";

    cout << "Problem 3: EARLY RETURN LEAK\n";
    cout << "---\n";
    cout << "void badFunction() {\n";
    cout << "    int* data = new int[1000];\n";
    cout << "    if (error) return;  // Early exit 💥\n";
    cout << "    delete[] data;      // Never reached!\n";
    cout << "}\n";
    cout << "Result: Early return bypasses cleanup!\n";
    cout << "\n";

    cout << "Problem 4: COMPLEX CLEANUP ORDER\n";
    cout << "---\n";
    cout << "void badFunction() {\n";
    cout << "    FILE* f1 = fopen(\"a.txt\", \"r\");\n";
    cout << "    FILE* f2 = fopen(\"b.txt\", \"r\");\n";
    cout << "    int* data = new int[1000];\n";
    cout << "    \n";
    cout << "    // Must cleanup in REVERSE order!\n";
    cout << "    delete[] data;\n";
    cout << "    fclose(f2);\n";
    cout << "    fclose(f1);\n";
    cout << "    // What if we get the order wrong? 💥\n";
    cout << "}\n";
    cout << "\n";

    cout << "Problem 5: MULTIPLE ERROR PATHS\n";
    cout << "---\n";
    cout << "void badFunction() {\n";
    cout << "    int* data = new int[1000];\n";
    cout << "    if (error1) { delete[] data; return; }\n";
    cout << "    if (error2) { delete[] data; return; }\n";
    cout << "    if (error3) { delete[] data; return; }\n";
    cout << "    delete[] data;\n";
    cout << "    // Repetitive! Error-prone! 💥\n";
    cout << "}\n";
}

// ============================================
// EXAMPLE 2: RAII SOLUTION - BASIC
// ============================================

class SimpleRAII {
private:
    int* data;
    size_t size;
    string name;

public:
    // Constructor: ACQUIRE resource
    SimpleRAII(size_t s, const string& n) : size(s), name(n) {
        data = new int[size];
        cout << "✅ [" << name << "] ACQUIRED: " << size << " ints\n";
    }

    // Destructor: RELEASE resource
    ~SimpleRAII() {
        delete[] data;
        cout << "💥 [" << name << "] RELEASED: " << size << " ints\n";
    }

    // Delete copy to prevent double-delete
    SimpleRAII(const SimpleRAII&) = delete;
    SimpleRAII& operator=(const SimpleRAII&) = delete;

    int* get() { return data; }
};

void demonstrateBasicRaii() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: RAII SOLUTION          ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "RAII Pattern:\n";
    cout << "  Constructor → Acquire resource\n";
    cout << "  Destructor  → Release resource\n";
    cout << "\n";

    cout << "--- Normal scope exit ---\n";
    {
        SimpleRAII obj(100, "Obj1");
        cout << "   Using object...\n";
        // No manual cleanup needed!
    }  // ← Destructor automatically called here!
    cout << "   ✅ Automatic cleanup!\n";

    cout << "\n--- Exception safety ---\n";
    try {
        SimpleRAII obj(200, "Obj2");
        cout << "   Before exception...\n";
        throw runtime_error("Oops!");
        cout << "   This never executes\n";
    } catch (const exception& e) {
        cout << "   Caught: " << e.what() << "\n";
        cout << "   ✅ Object was automatically cleaned up!\n";
    }

    cout << "\n--- Early return ---\n";
    auto testEarlyReturn = []() {
        SimpleRAII obj(300, "Obj3");
        cout << "   Before early return...\n";
        return;  // Early exit!
        cout << "   This never executes\n";
    };
    testEarlyReturn();
    cout << "   ✅ Automatic cleanup on early return!\n";

    cout << "\n";
    cout << "KEY BENEFITS:\n";
    cout << "  ✅ Can't forget to cleanup\n";
    cout << "  ✅ Exception safe automatically\n";
    cout << "  ✅ Correct cleanup order (reverse construction)\n";
    cout << "  ✅ No code duplication\n";
    cout << "  ✅ Compile-time guaranteed\n";
}

// ============================================
// EXAMPLE 3: FILE HANDLE RAII
// ============================================

class FileHandle {
private:
    FILE* file;
    string filename;

public:
    FileHandle(const string& fname, const char* mode) : filename(fname) {
        file = fopen(fname.c_str(), mode);
        if (!file) {
            throw runtime_error("Failed to open: " + fname);
        }
        cout << "✅ Opened file: " << filename << "\n";
    }

    ~FileHandle() {
        if (file) {
            fclose(file);
            cout << "💥 Closed file: " << filename << "\n";
        }
    }

    // Delete copy
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;

    // Allow move
    FileHandle(FileHandle&& other) noexcept
        : file(other.file), filename(move(other.filename)) {
        other.file = nullptr;
    }

    FILE* get() const { return file; }

    void write(const string& data) {
        if (file) {
            fprintf(file, "%s", data.c_str());
        }
    }
};

void demonstrateFileRaii() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: FILE HANDLE RAII       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "Without RAII:\n";
    cout << "  FILE* f = fopen(\"file.txt\", \"w\");\n";
    cout << "  processFile(f);  // If this throws...\n";
    cout << "  fclose(f);       // ...never reached! 💥\n";
    cout << "\n";

    cout << "With RAII:\n";
    cout << "  FileHandle f(\"file.txt\", \"w\");\n";
    cout << "  processFile(f.get());  // If this throws...\n";
    cout << "  // Destructor ALWAYS closes file! ✅\n";
    cout << "\n";

    cout << "--- Creating test file ---\n";
    {
        FileHandle file("/tmp/test_raii.txt", "w");
        file.write("Hello RAII!\n");
        file.write("Automatic cleanup!\n");
        cout << "   File written\n";
    }  // File automatically closed here!
    cout << "   ✅ File handle cleaned up!\n";

    cout << "\n--- Exception safety test ---\n";
    try {
        FileHandle file("/tmp/test_exception.txt", "w");
        file.write("Before exception\n");
        throw runtime_error("Test exception");
    } catch (const exception& e) {
        cout << "   Caught: " << e.what() << "\n";
        cout << "   ✅ File was automatically closed!\n";
    }
}

// ============================================
// EXAMPLE 4: MUTEX LOCK RAII
// ============================================

class SharedResource {
private:
    int counter = 0;
    mutable mutex mtx;

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
        lock_guard<mutex> lock(mtx);
        counter++;
        // Mutex automatically unlocked on scope exit!
    }

    int getCounter() const {
        lock_guard<mutex> lock(mtx);
        return counter;
    }
};

void demonstrateLockRaii() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: MUTEX LOCK RAII        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "Without RAII:\n";
    cout << "  mutex mtx;\n";
    cout << "  mtx.lock();\n";
    cout << "  processData();  // If this throws...\n";
    cout << "  mtx.unlock();   // ...mutex stays locked! 💥 DEADLOCK!\n";
    cout << "\n";

    cout << "With RAII:\n";
    cout << "  mutex mtx;\n";
    cout << "  lock_guard<mutex> lock(mtx);  // Lock acquired\n";
    cout << "  processData();  // If this throws...\n";
    cout << "  // lock's destructor ALWAYS unlocks! ✅\n";
    cout << "\n";

    cout << "--- Testing thread-safe increment ---\n";
    SharedResource resource;

    // Spawn threads
    vector<thread> threads;
    for (int i = 0; i < 5; i++) {
        threads.emplace_back([&resource, i]() {
            for (int j = 0; j < 10; j++) {
                resource.incrementGood();
            }
            cout << "   Thread " << i << " completed\n";
        });
    }

    // Wait for all threads
    for (auto& t : threads) {
        t.join();
    }

    cout << "   Final counter: " << resource.getCounter() << "\n";
    cout << "   ✅ All locks automatically released!\n";
    cout << "\n";

    cout << "LOCK RAII TYPES:\n";
    cout << "  lock_guard:   Simple RAII lock (C++11)\n";
    cout << "  unique_lock:  Flexible RAII lock (can unlock early)\n";
    cout << "  shared_lock:  Shared (reader) lock (C++14)\n";
    cout << "  scoped_lock:  Multi-mutex lock (C++17)\n";
}

// ============================================
// EXAMPLE 5: SMART POINTERS AS RAII
// ============================================

void demonstrateSmartPointerRaii() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: SMART POINTERS (RAII)  ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "Smart pointers = RAII for memory!\n";
    cout << "\n";

    cout << "❌ WITHOUT RAII:\n";
    cout << "  int* ptr = new int(42);\n";
    cout << "  processData(ptr);  // If exception...\n";
    cout << "  delete ptr;        // ...never reached! 💥 LEAK!\n";
    cout << "\n";

    cout << "✅ WITH RAII (unique_ptr):\n";
    cout << "  unique_ptr<int> ptr = make_unique<int>(42);\n";
    cout << "  processData(ptr.get());  // If exception...\n";
    cout << "  // ptr's destructor ALWAYS calls delete! ✅\n";
    cout << "\n";

    cout << "--- unique_ptr example ---\n";
    {
        auto ptr = make_unique<int>(100);
        cout << "   Created unique_ptr: " << *ptr << "\n";
    }  // Automatically deleted here!
    cout << "   ✅ Memory automatically freed!\n";

    cout << "\n--- shared_ptr example ---\n";
    {
        auto ptr1 = make_shared<int>(200);
        cout << "   ptr1 use_count: " << ptr1.use_count() << "\n";
        {
            auto ptr2 = ptr1;
            cout << "   ptr2 created, use_count: " << ptr1.use_count() << "\n";
        }
        cout << "   ptr2 destroyed, use_count: " << ptr1.use_count() << "\n";
    }  // Memory freed when last owner destroyed!
    cout << "   ✅ Reference counting + automatic cleanup!\n";

    cout << "\n--- vector example (RAII container) ---\n";
    {
        vector<int> vec = {1, 2, 3, 4, 5};
        cout << "   Vector size: " << vec.size() << "\n";
    }  // Internal array automatically freed!
    cout << "   ✅ Dynamic array automatically freed!\n";

    cout << "\n";
    cout << "ALL SMART POINTERS USE RAII:\n";
    cout << "  unique_ptr   → Exclusive ownership\n";
    cout << "  shared_ptr   → Shared ownership (ref counted)\n";
    cout << "  weak_ptr     → Non-owning reference\n";
    cout << "  auto_ptr     → Deprecated (don't use!)\n";
}

// ============================================
// EXAMPLE 6: CUSTOM RAII - DATABASE
// ============================================

class DatabaseConnection {
private:
    string connectionString;
    bool connected;

public:
    DatabaseConnection(const string& connStr)
        : connectionString(connStr), connected(false) {
        // Acquire: Open database connection
        cout << "✅ Opening DB: " << connectionString << "\n";
        connected = true;
        // Simulate connection
    }

    ~DatabaseConnection() {
        if (connected) {
            // Release: Close database connection
            cout << "💥 Closing DB: " << connectionString << "\n";
            connected = false;
        }
    }

    // Delete copy
    DatabaseConnection(const DatabaseConnection&) = delete;
    DatabaseConnection& operator=(const DatabaseConnection&) = delete;

    void executeQuery(const string& query) {
        if (connected) {
            cout << "   Executing: " << query << "\n";
        }
    }

    void beginTransaction() {
        cout << "   BEGIN TRANSACTION\n";
    }

    void commit() {
        cout << "   COMMIT\n";
    }

    void rollback() {
        cout << "   ROLLBACK\n";
    }
};

class Transaction {
private:
    DatabaseConnection& db;
    bool committed;

public:
    Transaction(DatabaseConnection& conn) : db(conn), committed(false) {
        db.beginTransaction();
    }

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
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: DATABASE RAII          ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Successful transaction ---\n";
    {
        DatabaseConnection db("localhost:5432/mydb");
        {
            Transaction txn(db);
            db.executeQuery("INSERT INTO users VALUES (...)");
            db.executeQuery("UPDATE accounts SET ...");
            txn.commit();  // Explicit commit
        }  // Transaction object destroyed
    }  // Database connection closed
    cout << "   ✅ Connection automatically closed!\n";

    cout << "\n--- Failed transaction (auto-rollback) ---\n";
    try {
        DatabaseConnection db("localhost:5432/mydb");
        {
            Transaction txn(db);
            db.executeQuery("INSERT INTO users VALUES (...)");
            throw runtime_error("Something went wrong!");
            txn.commit();  // Never reached!
        }  // txn destructor → automatic ROLLBACK!
    } catch (const exception& e) {
        cout << "   Caught: " << e.what() << "\n";
        cout << "   ✅ Transaction automatically rolled back!\n";
    }

    cout << "\n";
    cout << "RAII Pattern Benefits:\n";
    cout << "  ✅ Connection always closed\n";
    cout << "  ✅ Transaction always committed or rolled back\n";
    cout << "  ✅ No resource leaks on exceptions\n";
    cout << "  ✅ Clean, readable code\n";
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
    ScopeGuard(Func f) : cleanup(move(f)), dismissed(false) {}

    ~ScopeGuard() {
        if (!dismissed) {
            cleanup();
        }
    }

    void dismiss() {
        dismissed = true;
    }

    // Delete copy/move
    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;
};

template<typename Func>
ScopeGuard<Func> makeScopeGuard(Func f) {
    return ScopeGuard<Func>(move(f));
}

void demonstrateScopeGuard() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 7: SCOPE GUARD            ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "Scope Guard: Generic RAII wrapper for any cleanup!\n";
    cout << "\n";

    cout << "--- Example 1: Cleanup on scope exit ---\n";
    {
        cout << "   Entering scope\n";
        auto guard = makeScopeGuard([]() {
            cout << "   💥 Cleanup executed!\n";
        });
        cout << "   Doing work...\n";
    }  // Guard destructor calls cleanup

    cout << "\n--- Example 2: Multi-resource cleanup ---\n";
    {
        int* ptr1 = new int(10);
        int* ptr2 = new int(20);
        int* ptr3 = new int(30);

        auto g1 = makeScopeGuard([ptr1]() { delete ptr1; cout << "   💥 Freed ptr1\n"; });
        auto g2 = makeScopeGuard([ptr2]() { delete ptr2; cout << "   💥 Freed ptr2\n"; });
        auto g3 = makeScopeGuard([ptr3]() { delete ptr3; cout << "   💥 Freed ptr3\n"; });

        cout << "   All resources acquired\n";
    }  // All guards clean up in reverse order!

    cout << "\n--- Example 3: Dismissible guard ---\n";
    {
        cout << "   Starting operation\n";
        auto guard = makeScopeGuard([]() {
            cout << "   💥 Rollback executed!\n";
        });

        // Do work...
        cout << "   Work completed successfully\n";
        guard.dismiss();  // Success! Don't rollback
    }
    cout << "   ✅ Rollback was dismissed\n";

    cout << "\n--- Example 4: Exception safety ---\n";
    try {
        auto guard = makeScopeGuard([]() {
            cout << "   💥 Emergency cleanup!\n";
        });
        cout << "   Before exception...\n";
        throw runtime_error("Error!");
    } catch (const exception& e) {
        cout << "   Caught: " << e.what() << "\n";
        cout << "   ✅ Guard ensured cleanup!\n";
    }

    cout << "\n";
    cout << "Scope Guard Use Cases:\n";
    cout << "  1. C API resources (fopen/fclose, malloc/free)\n";
    cout << "  2. Temporary state changes (restore on exit)\n";
    cout << "  3. Cleanup in complex error paths\n";
    cout << "  4. Non-RAII libraries\n";
    cout << "  5. Custom cleanup logic\n";
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
        cout << "   ✅ Flag: " << oldValue << " → " << newValue << "\n";
    }

    ~BoolGuard() {
        flag = oldValue;
        cout << "   💥 Flag restored: " << oldValue << "\n";
    }
};

class IndentGuard {
private:
    int& level;

public:
    IndentGuard(int& lvl) : level(lvl) {
        level++;
    }

    ~IndentGuard() {
        level--;
    }
};

void demonstrateStateRaii() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 8: STATE MANAGEMENT RAII  ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "RAII for temporary state changes!\n";
    cout << "\n";

    cout << "--- Boolean flag guard ---\n";
    bool isProcessing = false;
    cout << "   Initial flag: " << isProcessing << "\n";
    {
        BoolGuard guard(isProcessing, true);
        cout << "   Inside scope, flag: " << isProcessing << "\n";
    }
    cout << "   After scope, flag: " << isProcessing << "\n";

    cout << "\n--- Indent level management ---\n";
    int indentLevel = 0;

    auto printIndented = [&](const string& msg) {
        cout << string(indentLevel * 2, ' ') << msg << "\n";
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
    cout << "   ✅ Indent automatically managed!\n";

    cout << "\n";
    cout << "State RAII Use Cases:\n";
    cout << "  1. Temporarily change global state\n";
    cout << "  2. Scoped logging level changes\n";
    cout << "  3. Debug mode toggles\n";
    cout << "  4. Profiler scopes\n";
    cout << "  5. Mock/stub activation\n";
}

// ============================================
// EXAMPLE 9: RAII CONSTRUCTION ORDER
// ============================================

class RAIIOrdered {
private:
    string name;

public:
    RAIIOrdered(const string& n) : name(n) {
        cout << "   ✅ Construct: " << name << "\n";
    }

    ~RAIIOrdered() {
        cout << "   💥 Destruct:  " << name << "\n";
    }
};

void demonstrateConstructionOrder() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 9: CONSTRUCTION ORDER     ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "CRITICAL RULE: Destruction order is REVERSE of construction!\n";
    cout << "\n";

    cout << "--- Stack objects ---\n";
    {
        RAIIOrdered obj1("First");
        RAIIOrdered obj2("Second");
        RAIIOrdered obj3("Third");
        cout << "   All constructed\n";
    }  // Destroyed in reverse: Third, Second, First
    cout << "   ✅ Reverse order ensures correct cleanup!\n";

    cout << "\n--- Why order matters ---\n";
    cout << "  class Database {\n";
    cout << "      FileHandle logFile;      // Constructed FIRST\n";
    cout << "      Connection connection;   // Constructed SECOND\n";
    cout << "  };\n";
    cout << "\n";
    cout << "  Destruction order:\n";
    cout << "    1. connection destroyed  (closes DB)\n";
    cout << "    2. logFile destroyed     (closes log)\n";
    cout << "\n";
    cout << "  ✅ Connection closed before log file → Can log closure!\n";
    cout << "  If reversed, we'd log AFTER log file closed! 💥\n";

    cout << "\n";
    cout << "MEMBER ORDER MATTERS:\n";
    cout << "  class MyClass {\n";
    cout << "      A a;  // ← Constructed FIRST\n";
    cout << "      B b;  // ← Constructed SECOND (can use 'a')\n";
    cout << "      C c;  // ← Constructed THIRD (can use 'a' and 'b')\n";
    cout << "      \n";
    cout << "      // Destructed in reverse: c, b, a\n";
    cout << "  };\n";
    cout << "\n";
    cout << "RULE: Declare members in dependency order!\n";
}

// ============================================
// EXAMPLE 10: EXCEPTION SAFETY LEVELS
// ============================================

void demonstrateExceptionSafety() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 10: EXCEPTION SAFETY      ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "EXCEPTION SAFETY LEVELS:\n";
    cout << "\n";

    cout << "1. NO GUARANTEE (❌ Bad)\n";
    cout << "   - Resources may leak\n";
    cout << "   - Object in invalid state\n";
    cout << "   Example:\n";
    cout << "     void func() {\n";
    cout << "         int* p = new int;\n";
    cout << "         doWork();  // If throws, p leaks!\n";
    cout << "         delete p;\n";
    cout << "     }\n";
    cout << "\n";

    cout << "2. BASIC GUARANTEE (⚠️  OK)\n";
    cout << "   - No resources leak\n";
    cout << "   - Object in valid but unspecified state\n";
    cout << "   - RAII ensures cleanup\n";
    cout << "   Example:\n";
    cout << "     void func() {\n";
    cout << "         unique_ptr<int> p = make_unique<int>();\n";
    cout << "         doWork();  // If throws, p auto-freed ✅\n";
    cout << "     }\n";
    cout << "\n";

    cout << "3. STRONG GUARANTEE (✅ Good)\n";
    cout << "   - Operation succeeds completely OR\n";
    cout << "   - Has no effect (rollback)\n";
    cout << "   - Copy-and-swap idiom\n";
    cout << "   Example:\n";
    cout << "     MyClass& operator=(const MyClass& other) {\n";
    cout << "         MyClass temp(other);  // May throw\n";
    cout << "         swap(temp);           // Never throws\n";
    cout << "         return *this;\n";
    cout << "     }\n";
    cout << "\n";

    cout << "4. NO-THROW GUARANTEE (🌟 Best)\n";
    cout << "   - Never throws exceptions\n";
    cout << "   - Usually marked noexcept\n";
    cout << "   Example:\n";
    cout << "     void swap(MyClass& other) noexcept {\n";
    cout << "         // Swap implementation\n";
    cout << "     }\n";
    cout << "\n";

    cout << "RAII PROVIDES AT LEAST BASIC GUARANTEE!\n";
    cout << "  - Resources always cleaned up\n";
    cout << "  - No leaks even on exceptions\n";
    cout << "  - Foundation for strong guarantee\n";
}

// ============================================
// EXAMPLE 11: COMMON RAII PATTERNS
// ============================================

void demonstrateCommonPatterns() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 11: COMMON RAII PATTERNS  ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "1. MEMORY MANAGEMENT\n";
    cout << "   ✅ unique_ptr<T>\n";
    cout << "   ✅ shared_ptr<T>\n";
    cout << "   ✅ vector<T>, string\n";
    cout << "   ❌ Raw new/delete\n";
    cout << "\n";

    cout << "2. FILE HANDLING\n";
    cout << "   ✅ fstream, ifstream, ofstream\n";
    cout << "   ✅ Custom FileHandle wrapper\n";
    cout << "   ❌ Raw fopen/fclose\n";
    cout << "\n";

    cout << "3. LOCKING\n";
    cout << "   ✅ lock_guard<mutex>\n";
    cout << "   ✅ unique_lock<mutex>\n";
    cout << "   ✅ shared_lock<shared_mutex>\n";
    cout << "   ✅ scoped_lock (C++17)\n";
    cout << "   ❌ Manual lock/unlock\n";
    cout << "\n";

    cout << "4. THREAD MANAGEMENT\n";
    cout << "   ✅ thread with join in destructor\n";
    cout << "   ✅ jthread (C++20 - auto-joins)\n";
    cout << "   ❌ Manual thread creation/join\n";
    cout << "\n";

    cout << "5. DATABASE CONNECTIONS\n";
    cout << "   ✅ Connection wrapper with RAII\n";
    cout << "   ✅ Transaction with auto-rollback\n";
    cout << "   ❌ Manual connect/disconnect\n";
    cout << "\n";

    cout << "6. NETWORK SOCKETS\n";
    cout << "   ✅ Socket wrapper with RAII\n";
    cout << "   ✅ Auto-close on destruction\n";
    cout << "   ❌ Manual socket/close\n";
    cout << "\n";

    cout << "7. TEMPORARY STATE\n";
    cout << "   ✅ Scope guards\n";
    cout << "   ✅ State wrappers (BoolGuard, etc.)\n";
    cout << "   ❌ Manual state save/restore\n";
    cout << "\n";

    cout << "8. GPU RESOURCES\n";
    cout << "   ✅ Texture/Buffer wrappers\n";
    cout << "   ✅ Context managers\n";
    cout << "   ❌ Manual create/destroy\n";
}

// ============================================
// EXAMPLE 12: COMMON MISTAKES
// ============================================

void demonstrateCommonMistakes() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 12: COMMON MISTAKES       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "MISTAKE 1: Forgetting destructor\n";
    cout << "---\n";
    cout << "❌ class BadRAII {\n";
    cout << "     FILE* file;\n";
    cout << "     BadRAII(const char* name) { file = fopen(name, \"r\"); }\n";
    cout << "     // No destructor! File never closed! 💥\n";
    cout << "   };\n";
    cout << "\n";
    cout << "✅ class GoodRAII {\n";
    cout << "     FILE* file;\n";
    cout << "     GoodRAII(const char* name) { file = fopen(name, \"r\"); }\n";
    cout << "     ~GoodRAII() { if (file) fclose(file); }\n";
    cout << "   };\n";
    cout << "\n";

    cout << "MISTAKE 2: Allowing copies without deep copy\n";
    cout << "---\n";
    cout << "❌ class BadRAII {\n";
    cout << "     int* data;\n";
    cout << "     BadRAII() : data(new int) {}\n";
    cout << "     ~BadRAII() { delete data; }\n";
    cout << "     // Default copy = shallow copy = double delete! 💥\n";
    cout << "   };\n";
    cout << "\n";
    cout << "✅ class GoodRAII {\n";
    cout << "     int* data;\n";
    cout << "     GoodRAII() : data(new int) {}\n";
    cout << "     ~GoodRAII() { delete data; }\n";
    cout << "     GoodRAII(const GoodRAII&) = delete;  // Disable copy\n";
    cout << "     GoodRAII& operator=(const GoodRAII&) = delete;\n";
    cout << "   };\n";
    cout << "\n";

    cout << "MISTAKE 3: Destructor that can throw\n";
    cout << "---\n";
    cout << "❌ ~BadRAII() {\n";
    cout << "     cleanup();  // Might throw! 💥\n";
    cout << "     // If exception during stack unwinding → terminate!\n";
    cout << "   }\n";
    cout << "\n";
    cout << "✅ ~GoodRAII() noexcept {  // Mark noexcept!\n";
    cout << "     try {\n";
    cout << "         cleanup();\n";
    cout << "     } catch (...) {\n";
    cout << "         // Log error, but don't throw\n";
    cout << "     }\n";
    cout << "   }\n";
    cout << "\n";

    cout << "MISTAKE 4: Acquiring multiple resources unsafely\n";
    cout << "---\n";
    cout << "❌ class BadRAII {\n";
    cout << "     int* p1;\n";
    cout << "     int* p2;\n";
    cout << "     BadRAII() {\n";
    cout << "         p1 = new int;  // Allocated\n";
    cout << "         p2 = new int;  // If this throws, p1 leaks! 💥\n";
    cout << "     }\n";
    cout << "   };\n";
    cout << "\n";
    cout << "✅ class GoodRAII {\n";
    cout << "     unique_ptr<int> p1;  // RAII!\n";
    cout << "     unique_ptr<int> p2;  // RAII!\n";
    cout << "     GoodRAII() \n";
    cout << "         : p1(make_unique<int>())   // If p2 throws,\n";
    cout << "         , p2(make_unique<int>()) { // p1 auto-freed!\n";
    cout << "     }\n";
    cout << "   };\n";
    cout << "\n";

    cout << "MISTAKE 5: Not checking for null in destructor\n";
    cout << "---\n";
    cout << "❌ ~BadRAII() {\n";
    cout << "     fclose(file);  // What if file is nullptr? 💥\n";
    cout << "   }\n";
    cout << "\n";
    cout << "✅ ~GoodRAII() {\n";
    cout << "     if (file) {  // Check before cleanup\n";
    cout << "         fclose(file);\n";
    cout << "     }\n";
    cout << "   }\n";
    cout << "\n";

    cout << "MISTAKE 6: Using RAII object after move\n";
    cout << "---\n";
    cout << "❌ unique_ptr<int> p1 = make_unique<int>(42);\n";
    cout << "   unique_ptr<int> p2 = move(p1);\n";
    cout << "   *p1;  // 💥 p1 is nullptr after move!\n";
    cout << "\n";
    cout << "✅ unique_ptr<int> p1 = make_unique<int>(42);\n";
    cout << "   unique_ptr<int> p2 = move(p1);\n";
    cout << "   // Don't use p1 anymore\n";
}

// ============================================
// EXAMPLE 13: BEST PRACTICES
// ============================================

void demonstrateBestPractices() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 13: BEST PRACTICES        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "✅ BEST PRACTICES:\n";
    cout << "\n";

    cout << "1. ACQUIRE IN CONSTRUCTOR\n";
    cout << "   - Get resource in constructor\n";
    cout << "   - Throw exception if acquisition fails\n";
    cout << "   - Leave object in valid state or don't construct\n";
    cout << "\n";

    cout << "2. RELEASE IN DESTRUCTOR\n";
    cout << "   - Always release in destructor\n";
    cout << "   - Mark destructor noexcept\n";
    cout << "   - Handle cleanup errors gracefully\n";
    cout << "\n";

    cout << "3. DISABLE COPY OR IMPLEMENT DEEP COPY\n";
    cout << "   - Delete copy operations if not needed\n";
    cout << "   - Or implement proper deep copy\n";
    cout << "   - Consider move semantics\n";
    cout << "\n";

    cout << "4. ONE RESOURCE PER RAII CLASS\n";
    cout << "   - Each RAII class manages ONE resource\n";
    cout << "   - Compose multiple RAII objects\n";
    cout << "   - Simplifies exception safety\n";
    cout << "\n";

    cout << "5. PREFER STANDARD RAII TYPES\n";
    cout << "   - Use unique_ptr, shared_ptr\n";
    cout << "   - Use fstream, not FILE*\n";
    cout << "   - Use lock_guard, not manual lock\n";
    cout << "   - Only write custom RAII when necessary\n";
    cout << "\n";

    cout << "6. MEMBER INITIALIZATION ORDER\n";
    cout << "   - Initialize in declaration order\n";
    cout << "   - Dependencies first\n";
    cout << "   - Destruction is reverse of construction\n";
    cout << "\n";
    cout << "7. MAKE DESTRUCTORS NOEXCEPT\n";
    cout << "   - Destructors should never throw\n";
    cout << "   - Catch and log exceptions internally\n";
    cout << "   - Prevents terminate() during unwinding\n";
    cout << "\n";

    cout << "8. USE SCOPE GUARDS FOR C APIs\n";
    cout << "   - Wrap non-RAII resources\n";
    cout << "   - Generic cleanup with lambdas\n";
    cout << "   - Better than manual cleanup\n";
    cout << "\n";

    cout << "9. DOCUMENT OWNERSHIP\n";
    cout << "   - Clear ownership semantics\n";
    cout << "   - Who's responsible for cleanup?\n";
    cout << "   - Use smart pointers to express intent\n";
    cout << "\n";

    cout << "10. TEST EXCEPTION PATHS\n";
    cout << "    - Verify cleanup on exceptions\n";
    cout << "    - Check for resource leaks\n";
    cout << "    - Use tools: Valgrind, ASan\n";
}

// ============================================
// EXAMPLE 14: RAII VS MANUAL COMPARISON
// ============================================

void demonstrateComparison() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 14: RAII VS MANUAL        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "SCENARIO: Function with multiple resources\n";
    cout << "\n";

    cout << "❌ MANUAL MANAGEMENT (Error-prone!):\n";
    cout << "---\n";
    cout << "void badFunction() {\n";
    cout << "    FILE* f1 = fopen(\"a.txt\", \"r\");\n";
    cout << "    if (!f1) return;\n";
    cout << "    \n";
    cout << "    FILE* f2 = fopen(\"b.txt\", \"r\");\n";
    cout << "    if (!f2) {\n";
    cout << "        fclose(f1);  // Must remember to cleanup!\n";
    cout << "        return;\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    int* data = new int[1000];\n";
    cout << "    \n";
    cout << "    if (errorCondition) {\n";
    cout << "        delete[] data;  // Must cleanup in right order!\n";
    cout << "        fclose(f2);\n";
    cout << "        fclose(f1);\n";
    cout << "        return;\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    processData(data);  // If throws? LEAK! 💥\n";
    cout << "    \n";
    cout << "    delete[] data;  // Easy to forget!\n";
    cout << "    fclose(f2);\n";
    cout << "    fclose(f1);\n";
    cout << "}\n";
    cout << "\n";
    cout << "Problems:\n";
    cout << "  - 18 lines of cleanup code\n";
    cout << "  - Multiple error paths\n";
    cout << "  - Easy to forget cleanup\n";
    cout << "  - Not exception-safe\n";
    cout << "  - Cleanup order errors\n";
    cout << "\n";

    cout << "✅ RAII (Clean and safe!):\n";
    cout << "---\n";
    cout << "void goodFunction() {\n";
    cout << "    ifstream f1(\"a.txt\");\n";
    cout << "    if (!f1) return;\n";
    cout << "    \n";
    cout << "    ifstream f2(\"b.txt\");\n";
    cout << "    if (!f2) return;\n";
    cout << "    \n";
    cout << "    vector<int> data(1000);\n";
    cout << "    \n";
    cout << "    if (errorCondition) return;  // Automatic cleanup!\n";
    cout << "    \n";
    cout << "    processData(data.data());  // Exception? Still safe! ✅\n";
    cout << "    \n";
    cout << "    // No manual cleanup needed!\n";
    cout << "}\n";
    cout << "\n";
    cout << "Benefits:\n";
    cout << "  - 8 lines total (55% less code!)\n";
    cout << "  - No manual cleanup\n";
    cout << "  - Can't forget cleanup\n";
    cout << "  - Exception-safe automatically\n";
    cout << "  - Correct cleanup order guaranteed\n";
    cout << "\n";

    cout << "CODE REDUCTION: 55% less code with RAII! 🚀\n";
    cout << "BUGS: Nearly zero with RAII vs many without!\n";
}

// ============================================
// EXAMPLE 15: REAL-WORLD EXAMPLE
// ============================================

class Timer {
private:
    string name;
    chrono::high_resolution_clock::time_point start;

public:
    Timer(const string& n) : name(n) {
        start = chrono::high_resolution_clock::now();
        cout << "   ⏱️  [" << name << "] Started\n";
    }

    ~Timer() {
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
        cout << "   ⏱️  [" << name << "] Completed: "
             << duration.count() << "ms\n";
    }
};

void demonstrateRealWorld() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 15: REAL-WORLD EXAMPLE    ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "Real-world scenario: Processing files with timing\n";
    cout << "\n";

    {
        Timer timer("File Processing");

        // Open input file (RAII)
        ifstream input("/tmp/test_raii.txt");
        if (!input) {
            cout << "   ⚠️  File not found (expected)\n";
            return;
        }

        // Create output file (RAII)
        ofstream output("/tmp/test_output.txt");

        // Lock for thread-safe operation (RAII)
        static mutex mtx;
        lock_guard<mutex> lock(mtx);

        cout << "   Processing files...\n";
        this_thread::sleep_for(chrono::milliseconds(100));

        // All cleanup happens automatically:
        // 1. Timer reports duration
        // 2. Lock released
        // 3. Output file closed
        // 4. Input file closed
        // All in correct reverse order!
    }

    cout << "\n   ✅ Everything cleaned up automatically!\n";
    cout << "   ✅ Even if exceptions occur!\n";
    cout << "   ✅ Correct cleanup order guaranteed!\n";
}

// ============================================
// MAIN - RUN ALL EXAMPLES
// ============================================

int main() {
    cout << "╔══════════════════════════════════════════════════════════╗\n";
    cout << "║                                                          ║\n";
    cout << "║                RAII - COMPLETE GUIDE                     ║\n";
    cout << "║         Resource Acquisition Is Initialization           ║\n";
    cout << "║                                                          ║\n";
    cout << "╚══════════════════════════════════════════════════════════╝\n";

    try {
        demonstrateProblemWithoutRaii();      // Why we need RAII
        demonstrateBasicRaii();               // Basic RAII pattern
        demonstrateFileRaii();                // File handling
        demonstrateLockRaii();                // Mutex locking
        demonstrateSmartPointerRaii();        // Memory management
        demonstrateDatabaseRaii();            // Database connections
        demonstrateScopeGuard();              // Generic RAII wrapper
        demonstrateStateRaii();               // State management
        demonstrateConstructionOrder();       // Construction/destruction order
        demonstrateExceptionSafety();         // Exception safety levels
        demonstrateCommonPatterns();          // Common RAII patterns
        demonstrateCommonMistakes();          // What to avoid
        demonstrateBestPractices();           // Best practices
        demonstrateComparison();              // RAII vs manual
        demonstrateRealWorld();               // Real-world example

    } catch (const exception& e) {
        cout << "💥 Exception: " << e.what() << "\n";
    }

    cout << "\n╔══════════════════════════════════════════════════════════╗\n";
    cout << "║                    KEY TAKEAWAYS                         ║\n";
    cout << "╚══════════════════════════════════════════════════════════╝\n\n";

    cout << "1. WHAT IS RAII?\n";
    cout << "   Resource Acquisition Is Initialization\n";
    cout << "   - Acquire resource in constructor\n";
    cout << "   - Release resource in destructor\n";
    cout << "   - Compiler guarantees destructor runs\n";
    cout << "\n";

    cout << "2. WHY RAII?\n";
    cout << "   ✅ Can't forget cleanup\n";
    cout << "   ✅ Exception-safe automatically\n";
    cout << "   ✅ Correct cleanup order\n";
    cout << "   ✅ No code duplication\n";
    cout << "   ✅ Compile-time guaranteed\n";
    cout << "\n";

    cout << "3. WHEN DESTRUCTOR CALLED:\n";
    cout << "   ✅ Normal scope exit\n";
    cout << "   ✅ Exception thrown\n";
    cout << "   ✅ Early return\n";
    cout << "   ✅ goto statement\n";
    cout << "   → ALWAYS called! (unless terminate())\n";
    cout << "\n";

    cout << "4. COMMON RAII TYPES:\n";
    cout << "   Memory:       unique_ptr, shared_ptr, vector, string\n";
    cout << "   Files:        fstream, ifstream, ofstream\n";
    cout << "   Locks:        lock_guard, unique_lock, scoped_lock\n";
    cout << "   Threads:      thread (with join), jthread (C++20)\n";
    cout << "   Custom:       Your own RAII wrappers!\n";
    cout << "\n";

    cout << "5. RAII RULES:\n";
    cout << "   1. Acquire in constructor\n";
    cout << "   2. Release in destructor (noexcept)\n";
    cout << "   3. One resource per class\n";
    cout << "   4. Delete copy or deep copy\n";
    cout << "   5. Support move when appropriate\n";
    cout << "\n";

    cout << "6. EXCEPTION SAFETY:\n";
    cout << "   RAII provides at least BASIC GUARANTEE:\n";
    cout << "   - No resource leaks\n";
    cout << "   - Object in valid state\n";
    cout << "   - Can build STRONG GUARANTEE on top\n";
    cout << "\n";

    cout << "7. BEST PRACTICES:\n";
    cout << "   ✅ Use standard RAII types\n";
    cout << "   ✅ Prefer unique_ptr over raw pointers\n";
    cout << "   ✅ Use fstream over FILE*\n";
    cout << "   ✅ Use lock_guard over manual lock/unlock\n";
    cout << "   ✅ Write custom RAII only when needed\n";
    cout << "\n";

    cout << "8. SCOPE GUARDS:\n";
    cout << "   Generic RAII for ANY cleanup:\n";
    cout << "   auto guard = makeScopeGuard([](){ cleanup(); });\n";
    cout << "   Perfect for C APIs!\n";
    cout << "\n";

    cout << "9. CONSTRUCTION ORDER:\n";
    cout << "   Members constructed in DECLARATION order\n";
    cout << "   Destructed in REVERSE order\n";
    cout << "   → Declare dependencies first!\n";
    cout << "\n";

    cout << "10. REMEMBER:\n";
    cout << "    \"If you acquire it, RAII should own it!\"\n";
    cout << "    \n";
    cout << "    Any resource that needs cleanup:\n";
    cout << "    memory, files, locks, sockets, connections,\n";
    cout << "    handles, GPU resources → Wrap in RAII!\n";
    cout << "\n";

    return 0;
}

/*
COMPILATION:
    g++ -std=c++17 -Wall -Wextra -pthread raii_example.cpp -o raii_example

RUN:
    ./raii_example

TOPICS COVERED:
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
