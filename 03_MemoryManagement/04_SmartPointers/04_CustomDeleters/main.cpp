#include <cstdio>
#include <cstdlib>
#include <functional>
#include <memory>
#include <print>
#include <stdexcept>
#include <string>
#include <vector>

// ============================================
// WHAT ARE CUSTOM DELETERS?
// ============================================

/*
CUSTOM DELETERS: Specialized cleanup logic for smart pointers

THE PROBLEM:
    Default smart pointer deleters use 'delete' or 'delete[]'
    But many resources need DIFFERENT cleanup:
    - C API: fclose(file), free(ptr), CloseHandle(h)
    - Arrays: delete[] instead of delete
    - No-op: Don't delete at all (observing only)
    - Logging: Log before deletion
    - Custom: Special cleanup logic

THE SOLUTION - CUSTOM DELETERS:
    Provide your own cleanup function!
    - Lambda functions
    - Function pointers
    - Functor objects
    - Any callable type

SMART POINTERS WITH CUSTOM DELETERS:

1. UNIQUE_PTR:
   unique_ptr<T, DeleterType> ptr(resource, deleter);
   - Deleter is part of the TYPE
   - Zero overhead if deleter is stateless
   - Can't convert between different deleter types

2. SHARED_PTR:
   shared_ptr<T> ptr(resource, deleter);
   - Deleter is stored in control block
   - Deleter is NOT part of the type
   - Can convert between different deleters
   - Small overhead (function pointer indirection)

WHEN TO USE:
    ✅ Managing C API resources (FILE*, malloc, etc.)
    ✅ Arrays (delete[] vs delete)
    ✅ Custom cleanup logic (logging, metrics, etc.)
    ✅ Non-owning pointers (no-op deleter)
    ✅ Platform-specific resources (handles, sockets)
    ✅ Third-party library resources

KEY CONCEPTS:
    - Deleter = Callable that takes pointer
    - Stateless = No data members (preferred)
    - Stateful = Has data members (larger size)
    - Type erasure in shared_ptr
    - Zero-overhead abstractions with unique_ptr
*/
// ============================================
// EXAMPLE 1: LAMBDA DELETERS
// ============================================

void demonstrateLambdaDeleters() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: LAMBDA DELETERS        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("Lambda deleters: Inline, convenient, modern!");
    std::println("");

    std::println("--- FILE* with fclose ---");
    {
        FILE* f = fopen("/tmp/test_deleter.txt", "w");
        if (f) {
            auto fileDeleter = [](FILE* fp) {
                std::println("   🔒 Lambda: Closing file");
                fclose(fp);
            };
            std::unique_ptr<FILE, decltype(fileDeleter)> file(f, fileDeleter);

            fprintf(file.get(), "Hello Custom Deleters!\n");
            std::println("   File written");
        }
    }
    std::println("   ✅ File automatically closed with fclose!");

    std::println("");
    std::println("--- malloc/free ---");
    {
        void* mem = malloc(100);
        auto mallocDeleter = [](void* p) {
            std::println("   🔒 Lambda: Freeing memory");
            free(p);
        };
        std::unique_ptr<void, decltype(mallocDeleter)> ptr(mem, mallocDeleter);

        std::println("   Memory allocated: {}", ptr.get());
    }
    std::println("   ✅ Memory freed with free()!");

    std::println("");
    std::println("--- Array deleter ---");
    {
        int* arr = new int[5]{1, 2, 3, 4, 5};
        auto arrayDeleter = [](int* p) {
            std::println("   🔒 Lambda: delete[] for array");
            delete[] p;
        };
        std::unique_ptr<int, decltype(arrayDeleter)> ptr(arr, arrayDeleter);

        std::println("   Array[0] = {}", ptr.get()[0]);
    }
    std::println("   ✅ Array properly deleted!");

    std::println("");
    std::println("--- Logging deleter ---");
    {
        int* p = new int(42);
        auto loggingDeleter = [](int* ptr) {
            std::println("   📊 Lambda: Logging deletion of value {}", *ptr);
            delete ptr;
        };
        std::unique_ptr<int, decltype(loggingDeleter)> smart_ptr(p, loggingDeleter);

        std::println("   Value: {}", *smart_ptr);
    }
}

// ============================================
// EXAMPLE 2: FUNCTION POINTER DELETERS
// ============================================

void fileFree(FILE* f) {
    if (f) {
        std::println("   🔒 Function: Closing file");
        fclose(f);
    }
}

void mallocFree(void* p) {
    if (p) {
        std::println("   🔒 Function: Freeing malloc memory");
        free(p);
    }
}

void arrayDelete(int* p) {
    std::println("   🔒 Function: delete[] array");
    delete[] p;
}

void demonstrateFunctionPointerDeleters() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: FUNCTION POINTER       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("Function pointer deleters: Reusable, clear, classic!");
    std::println("");

    std::println("--- FILE* deleter ---");
    {
        FILE* f = fopen("/tmp/test_func.txt", "w");
        if (f) {
            std::unique_ptr<FILE, decltype(&fileFree)> file(f, fileFree);
            fprintf(file.get(), "Function pointer deleter!\n");
            std::println("   File written");
        }
    }
    std::println("   ✅ Closed with function pointer!");

    std::println("");
    std::println("--- malloc deleter ---");
    {
        void* mem = malloc(256);
        std::unique_ptr<void, decltype(&mallocFree)> ptr(mem, mallocFree);
        std::println("   Memory at: {}", ptr.get());
    }
    std::println("   ✅ Freed with function pointer!");

    std::println("");
    std::println("--- Array deleter ---");
    {
        int* arr = new int[5]{10, 20, 30, 40, 50};
        std::unique_ptr<int, decltype(&arrayDelete)> ptr(arr, arrayDelete);
        std::println("   Array[2] = {}", ptr.get()[2]);
    }
    std::println("   ✅ Array deleted correctly!");
}

// ============================================
// EXAMPLE 3: FUNCTOR DELETERS
// ============================================

struct FileDeleter {
    void operator()(FILE* f) const {
        if (f) {
            std::println("   🔒 Functor: Closing file");
            fclose(f);
        }
    }
};

struct MallocDeleter {
    void operator()(void* p) const {
        if (p) {
            std::println("   🔒 Functor: Freeing memory");
            free(p);
        }
    }
};

struct ArrayDeleter {
    template<typename T>
    void operator()(T* p) const {
        std::println("   🔒 Functor: delete[] for array");
        delete[] p;
    }
};

struct LoggingDeleter {
    std::string name;

    LoggingDeleter(const std::string& n) : name(n) {}

    template<typename T>
    void operator()(T* p) const {
        std::println("   📊 Functor [{}]: Deleting", name);
        delete p;
    }
};

void demonstrateFunctorDeleters() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: FUNCTOR DELETERS       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("Functor deleters: Flexible, stateful, powerful!");
    std::println("");

    std::println("--- Stateless functor (FILE) ---");
    {
        FILE* f = fopen("/tmp/test_functor.txt", "w");
        if (f) {
            std::unique_ptr<FILE, FileDeleter> file(f);
            fprintf(file.get(), "Functor deleter!\n");
            std::println("   File written");
        }
    }
    std::println("   ✅ Stateless functor (zero overhead!)");

    std::println("");
    std::println("--- Stateless functor (malloc) ---");
    {
        void* mem = malloc(128);
        std::unique_ptr<void, MallocDeleter> ptr(mem);
        std::println("   Memory at: {}", ptr.get());
    }
    std::println("   ✅ Zero overhead functor!");

    std::println("");
    std::println("--- Template functor (arrays) ---");
    {
        int* arr = new int[5]{1, 2, 3, 4, 5};
        std::unique_ptr<int, ArrayDeleter> ptr(arr);
        std::println("   Array[0] = {}", ptr.get()[0]);
    }
    std::println("   ✅ Template functor works for any type!");

    std::println("");
    std::println("--- Stateful functor (logging) ---");
    {
        int* p = new int(99);
        std::unique_ptr<int, LoggingDeleter> ptr(p, LoggingDeleter("MyResource"));
        std::println("   Value: {}", *ptr);
    }
    std::println("   ✅ Stateful functor with data!");

    std::println("");
    std::println("Size comparison:");
    std::println("  sizeof(unique_ptr<int>) = {} bytes",
            sizeof(std::unique_ptr<int>));
    std::println("  sizeof(unique_ptr<int, ArrayDeleter>) = {} bytes (stateless)",
            sizeof(std::unique_ptr<int, ArrayDeleter>));
    std::println("  sizeof(unique_ptr<int, LoggingDeleter>) = {} bytes (has state)",
            sizeof(std::unique_ptr<int, LoggingDeleter>));
}

// ============================================
// EXAMPLE 4: UNIQUE_PTR VS SHARED_PTR DELETERS
// ============================================

void demonstrateUniqueVsShared() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: UNIQUE VS SHARED       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Testing unique_ptr ---");
    {
        auto deleter = [](int* p) {
            std::println("   🔒 Unique_ptr deleter");
            delete p;
        };
        std::unique_ptr<int, decltype(deleter)> ptr(new int(42), deleter);
        std::println("   Value: {}", *ptr);
    }

    std::println("");
    std::println("--- Testing shared_ptr ---");
    {
        std::shared_ptr<int> ptr(new int(99), [](int* p) {
            std::println("   🔒 Shared_ptr deleter");
            delete p;
        });
        std::println("   Value: {}", *ptr);

        std::shared_ptr<int> ptr2 = ptr;  // ✅ Can copy!
        std::println("   Shared count: {}", ptr.use_count());
    }
}

// ============================================
// EXAMPLE 5: C API RESOURCE MANAGEMENT
// ============================================

class FileWrapper {
public:
    // int (*)(FILE*) is the type of fclose; decltype(&fclose) is the same
    // type, but GCC warns about ignored attributes on it (see Readme).
    using FilePtr = std::unique_ptr<FILE, int (*)(FILE*)>;

    static FilePtr open(const std::string& filename, const char* mode) {
        FILE* f = fopen(filename.c_str(), mode);
        if (!f) {
            throw std::runtime_error("Failed to open: " + filename);
        }
        std::println("   ✅ Opened: {}", filename);
        return FilePtr(f, fclose);
    }
};

void demonstrateCAPIManagement() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 5: C API MANAGEMENT       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("Managing C API resources with custom deleters!");
    std::println("");

    std::println("--- FILE* management ---");
    try {
        auto file = FileWrapper::open("/tmp/test_capi.txt", "w");
        fprintf(file.get(), "Hello from C API!\n");
        fprintf(file.get(), "Automatic cleanup!\n");
        std::println("   Data written");
    } catch (const std::exception& e) {
        std::println("   Error: {}", e.what());
    }
    std::println("   💥 File automatically closed by deleter");

    std::println("");
    std::println("--- malloc/free management ---");
    {
        auto deleter = [](void* p) {
            std::println("   💥 Calling free()");
            free(p);
        };
        std::unique_ptr<void, decltype(deleter)> mem(malloc(1024), deleter);
        std::println("   Memory allocated: {}", mem.get());
    }
    std::println("   ✅ Freed with free(), not delete!");

    std::println("");
    std::println("--- Multiple FILE* resources ---");
    {
        auto f1 = FileWrapper::open("/tmp/input.txt", "w");
        auto f2 = FileWrapper::open("/tmp/output.txt", "w");

        fprintf(f1.get(), "Input data\n");
        fprintf(f2.get(), "Output data\n");

        std::println("   Both files written");
    }
    std::println("   💥 Both files closed");
}

// ============================================
// EXAMPLE 6: ARRAY DELETERS
// ============================================

void demonstrateArrayDeleters() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 6: ARRAY DELETERS         ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("Arrays need delete[], not delete!");
    std::println("");

    std::println("--- Solution 1: unique_ptr<T[]> (C++11) ---");
    {
        std::unique_ptr<int[]> arr(new int[5]{1, 2, 3, 4, 5});
        std::println("   arr[0] = {}", arr[0]);
        std::println("   arr[4] = {}", arr[4]);
    }
    std::println("   💥 Automatically calls delete[]!");
    std::println("");

    std::println("--- Solution 2: Custom deleter ---");
    {
        auto deleter = [](int* p) {
            std::println("   💥 Custom: delete[] called");
            delete[] p;
        };
        std::unique_ptr<int, decltype(deleter)> arr(new int[5], deleter);
        arr.get()[0] = 100;
        std::println("   arr[0] = {}", arr.get()[0]);
    }

    std::println("");
    std::println("--- Solution 3: Use vector instead! (BEST) ---");
    {
        std::vector<int> vec = {1, 2, 3, 4, 5};
        std::println("   vec[0] = {}", vec[0]);
        std::println("   vec.size() = {}", vec.size());
    }
    std::println("   💥 RAII automatic cleanup");
    std::println("   ✅ No need for custom deleter!");
}

// ============================================
// EXAMPLE 7: NO-OP DELETERS
// ============================================

struct NoOpDeleter {
    template<typename T>
    void operator()(T*) const {
        // Do nothing - don't delete!
        std::println("   ⚠️  No-op deleter: Not deleting");
    }
};

void demonstrateNoOpDeleters() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 7: NO-OP DELETERS         ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("No-op deleters: For non-owning observation");
    std::println("");

    std::println("--- Observing stack object ---");
    {
        int stackValue = 42;

        // Observe but don't own
        std::shared_ptr<int> observer(&stackValue, [](int*) {
            std::println("   ⚠️  Lambda no-op: Not deleting stack object");
        });

        std::println("   Observed value: {}", *observer);
        std::println("   use_count: {}", observer.use_count());
    }
    std::println("   ✅ Stack object NOT deleted by smart pointer");

    std::println("");
    std::println("--- Observing global resource ---");
    {
        static int globalResource = 100;

        std::unique_ptr<int, NoOpDeleter> ptr(&globalResource);
        std::println("   Global value: {}", *ptr);
    }
    std::println("   ✅ Global NOT deleted");

    std::println("");
    std::println("--- Multiple observers ---");
    {
        int value = 999;

        auto noOpDeleter = [](int*) {
            std::println("   ⚠️  Observer deleter");
        };

        std::shared_ptr<int> obs1(&value, noOpDeleter);
        std::shared_ptr<int> obs2 = obs1;
        std::shared_ptr<int> obs3 = obs1;

        std::println("   Observers: {}", obs1.use_count());
        std::println("   Value: {}", *obs1);
    }
    std::println("   ✅ All observers released, value NOT deleted");
}

// ============================================
// EXAMPLE 8: STATEFUL DELETERS
// ============================================

class MetricsDeleter {
private:
    std::string resourceName;
    int* deleteCount;

public:
    MetricsDeleter(const std::string& name, int* counter)
        : resourceName(name), deleteCount(counter) {}

    template<typename T>
    void operator()(T* p) const {
        std::println("   📊 Metrics [{}]: Deleting", resourceName);
        if (deleteCount) {
            (*deleteCount)++;
            std::println("   📊 Total deletions: {}", *deleteCount);
        }
        delete p;
    }
};

class LoggerDeleter {
private:
    std::string prefix;

public:
    LoggerDeleter(const std::string& p) : prefix(p) {}

    template<typename T>
    void operator()(T* p) const {
        // println needs a void* to print an address ({} on int* does not compile)
        std::println("   📝 [{}] Deleting pointer: {}", prefix, static_cast<const void*>(p));
        delete p;
    }
};

void demonstrateStatefulDeleters() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 8: STATEFUL DELETERS      ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("Stateful deleters: Store data for cleanup logic");
    std::println("");

    std::println("--- Metrics tracking ---");
    {
        int deleteCount = 0;

        {
            std::unique_ptr<int, MetricsDeleter> ptr1(
                new int(10),
                MetricsDeleter("Resource1", &deleteCount)
                );
            std::println("   ptr1 value: {}", *ptr1);
        }

        {
            std::unique_ptr<int, MetricsDeleter> ptr2(
                new int(20),
                MetricsDeleter("Resource2", &deleteCount)
                );
            std::println("   ptr2 value: {}", *ptr2);
        }

        std::println("   Final delete count: {}", deleteCount);
    }

    std::println("");
    std::println("--- Logging with context ---");
    {
        std::unique_ptr<int, LoggerDeleter> ptr1(
            new int(100),
            LoggerDeleter("MAIN")
            );

        std::unique_ptr<int, LoggerDeleter> ptr2(
            new int(200),
            LoggerDeleter("WORKER")
            );

        std::println("   ptr1: {}", *ptr1);
        std::println("   ptr2: {}", *ptr2);
    }

    std::println("");
    std::println("--- Size implications ---");
    std::println("sizeof(unique_ptr<int>): {} bytes", sizeof(std::unique_ptr<int>));
    std::println("sizeof(unique_ptr<int, MetricsDeleter>): {} bytes",
            sizeof(std::unique_ptr<int, MetricsDeleter>));
}

// ============================================
// EXAMPLE 9: DELETER TYPE DEDUCTION
// ============================================

void demonstrateDeleterDeduction() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 9: TYPE DEDUCTION         ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("Type deduction for deleters can be tricky!");
    std::println("");

    std::println("--- Solution 1: decltype ---");
    {
        auto deleter = [](int* p) {
            std::println("   💥 decltype deleter");
            delete p;
        };
        std::unique_ptr<int, decltype(deleter)> ptr(new int(42), deleter);
        std::println("   Value: {}", *ptr);
    }
    std::println("   ✅ decltype(deleter) captures lambda type");

    std::println("");
    std::println("--- Solution 2: std::function (type erasure) ---");
    {
        std::function<void(int*)> deleter = [](int* p) {
            std::println("   💥 std::function deleter");
            delete p;
        };
        std::unique_ptr<int, decltype(deleter)> ptr(new int(99), deleter);
        std::println("   Value: {}", *ptr);
    }

    std::println("");
    std::println("--- Solution 3: Function pointer (simple) ---");
    {
        auto deleter = +[](int* p) {  // + converts to function pointer
            std::println("   💥 Function pointer deleter");
            delete p;
        };
        std::unique_ptr<int, decltype(deleter)> ptr(new int(77), deleter);
        std::println("   Value: {}", *ptr);
    }
    std::println("   ✅ Clean function pointer type");

    std::println("");
    std::println("--- Solution 4: shared_ptr (easiest) ---");
    {
        std::shared_ptr<int> ptr(new int(55), [](int* p) {
            std::println("   💥 shared_ptr: No type needed!");
            delete p;
        });
        std::println("   Value: {}", *ptr);
    }
    std::println("   ✅ shared_ptr: Deleter type-erased!");
}

// ============================================
// EXAMPLE 10: SIZE IMPLICATIONS
// ============================================

void demonstrateSizes() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 10: SIZE IMPLICATIONS     ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    struct EmptyDeleter {
        void operator()(int* p) const { delete p; }
    };
    struct StatefulDeleter {
        int state;
        void operator()(int* p) const { delete p; }
    };
    auto statelessLambda = [](int* p) { delete p; };
    int captured = 0;
    auto capturingLambda = [captured](int* p) { (void)captured; delete p; };

    std::println("sizeof(int*)                                     = {} bytes", sizeof(int*));
    std::println("sizeof(unique_ptr<int>)                          = {} bytes", sizeof(std::unique_ptr<int>));
    std::println("sizeof(unique_ptr<int, EmptyDeleter>)            = {} bytes", sizeof(std::unique_ptr<int, EmptyDeleter>));
    std::println("sizeof(unique_ptr<int, stateless lambda>)        = {} bytes", sizeof(std::unique_ptr<int, decltype(statelessLambda)>));
    std::println("sizeof(unique_ptr<int, void (*)(int*)>)          = {} bytes", sizeof(std::unique_ptr<int, void (*)(int*)>));
    std::println("sizeof(unique_ptr<int, StatefulDeleter>)         = {} bytes", sizeof(std::unique_ptr<int, StatefulDeleter>));
    std::println("sizeof(unique_ptr<int, capturing lambda>)        = {} bytes", sizeof(std::unique_ptr<int, decltype(capturingLambda)>));
    std::println("sizeof(unique_ptr<int, function<void(int*)>>)    = {} bytes", sizeof(std::unique_ptr<int, std::function<void(int*)>>));
    std::println("sizeof(shared_ptr<int>)                          = {} bytes", sizeof(std::shared_ptr<int>));
}

// ============================================
// EXAMPLE 11: REAL-WORLD EXAMPLE
// ============================================

class DatabaseHandle {
public:
    using DBPtr = std::unique_ptr<FILE, int (*)(FILE*)>;

    static DBPtr open(const std::string& dbPath) {
        // "w+" creates the file if it does not exist ("r+" fails on a missing file)
        FILE* db = fopen(dbPath.c_str(), "w+");
        if (!db) {
            throw std::runtime_error("Failed to open database: " + dbPath);
        }
        std::println("   📂 Database opened: {}", dbPath);
        return DBPtr(db, fclose);
    }
};

class Transaction {
private:
    DatabaseHandle::DBPtr db;
    bool committed = false;

public:
    Transaction(DatabaseHandle::DBPtr&& database)
        : db(std::move(database)) {
        std::println("   🔄 Transaction started");
    }

    ~Transaction() {
        if (!committed) {
            std::println("   ↩️  Rolling back transaction");
        }
    }

    void execute(const std::string& query) {
        if (db) {
            fprintf(db.get(), "%s\n", query.c_str());
            std::println("   ✏️  Executed: {}", query);
        }
    }

    void commit() {
        std::println("   ✅ Transaction committed");
        committed = true;
    }
};

void demonstrateRealWorld() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 11: REAL-WORLD EXAMPLE    ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("Real-world: Database transaction with custom deleters");
    std::println("");

    std::println("--- Successful transaction ---");
    try {
        auto db = DatabaseHandle::open("/tmp/database.txt");
        Transaction txn(std::move(db));

        txn.execute("INSERT INTO users VALUES (...)");
        txn.execute("UPDATE accounts SET ...");
        txn.commit();

    } catch (const std::exception& e) {
        std::println("   ❌ Error: {}", e.what());
    }
    std::println("   💥 Database closed automatically");

    std::println("");
    std::println("--- Failed transaction (auto-rollback) ---");
    try {
        auto db = DatabaseHandle::open("/tmp/database2.txt");
        Transaction txn(std::move(db));

        txn.execute("INSERT INTO users VALUES (...)");
        throw std::runtime_error("Network error!");
        txn.commit();  // Never reached

    } catch (const std::exception& e) {
        std::println("   ❌ Error: {}", e.what());
    }
    std::println("   💥 Database closed automatically");
}

// ============================================
// MAIN - RUN ALL EXAMPLES
// ============================================

int main() {
    std::println("╔══════════════════════════════════════════════════════════╗");
    std::println("║                                                          ║");
    std::println("║           CUSTOM DELETERS - COMPLETE GUIDE               ║");
    std::println("║                                                          ║");
    std::println("╚══════════════════════════════════════════════════════════╝");

    try {
        demonstrateLambdaDeleters();          // Lambda deleters
        demonstrateFunctionPointerDeleters(); // Function pointer deleters
        demonstrateFunctorDeleters();         // Functor deleters
        demonstrateUniqueVsShared();          // unique_ptr vs shared_ptr
        demonstrateCAPIManagement();          // C API resources
        demonstrateArrayDeleters();           // Array handling
        demonstrateNoOpDeleters();            // No-op deleters
        demonstrateStatefulDeleters();        // Stateful deleters
        demonstrateDeleterDeduction();        // Type deduction
        demonstrateSizes();                   // sizeof of the different deleters
        demonstrateRealWorld();               // Real-world example

    } catch (const std::exception& e) {
        std::println("💥 Exception: {}", e.what());
    }

    return 0;
}
