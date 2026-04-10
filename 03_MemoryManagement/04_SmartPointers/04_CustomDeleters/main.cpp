#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cstdio>
#include <cstdlib>
#include <functional>
using namespace std;

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
// EXAMPLE 1: THE PROBLEM - DEFAULT DELETERS
// ============================================

void demonstrateProblem() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: DEFAULT DELETER ISSUES ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "Problem 1: C API Resources\n";
    cout << "---\n";
    cout << "FILE* f = fopen(\"file.txt\", \"r\");\n";
    cout << "unique_ptr<FILE> ptr(f);  // ❌ WRONG!\n";
    cout << "// Destructor calls 'delete f' not 'fclose(f)'! 💥\n";
    cout << "\n";

    cout << "Problem 2: Arrays\n";
    cout << "---\n";
    cout << "int* arr = new int[10];\n";
    cout << "unique_ptr<int> ptr(arr);  // ❌ WRONG!\n";
    cout << "// Destructor calls 'delete' not 'delete[]'! 💥\n";
    cout << "// Undefined behavior!\n";
    cout << "\n";

    cout << "Problem 3: malloc/free\n";
    cout << "---\n";
    cout << "void* mem = malloc(100);\n";
    cout << "unique_ptr<void> ptr(mem);  // ❌ WRONG!\n";
    cout << "// Destructor calls 'delete' not 'free()'! 💥\n";
    cout << "\n";

    cout << "Problem 4: Custom Cleanup\n";
    cout << "---\n";
    cout << "Resource* r = createResource();\n";
    cout << "unique_ptr<Resource> ptr(r);  // ❌ INCOMPLETE!\n";
    cout << "// Need to log, update metrics, notify observers\n";
    cout << "// Default 'delete' doesn't do this!\n";
    cout << "\n";

    cout << "Problem 5: Non-owning Observation\n";
    cout << "---\n";
    cout << "Resource* r = getGlobalResource();\n";
    cout << "unique_ptr<Resource> ptr(r);  // ❌ WRONG!\n";
    cout << "// We don't OWN this, shouldn't delete!\n";
    cout << "// Need a no-op deleter\n";
}

// ============================================
// EXAMPLE 2: LAMBDA DELETERS
// ============================================

void demonstrateLambdaDeleters() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: LAMBDA DELETERS        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "Lambda deleters: Inline, convenient, modern!\n";
    cout << "\n";

    cout << "--- Example 1: FILE* with fclose ---\n";
    {
        FILE* f = fopen("/tmp/test_deleter.txt", "w");
        if (f) {
            auto fileDeleter = [](FILE* fp) {
                cout << "   🔒 Lambda: Closing file\n";
                fclose(fp);
            };
            unique_ptr<FILE, decltype(fileDeleter)> file(f, fileDeleter);

            fprintf(file.get(), "Hello Custom Deleters!\n");
            cout << "   File written\n";
        }
    }
    cout << "   ✅ File automatically closed with fclose!\n";

    cout << "\n--- Example 2: malloc/free ---\n";
    {
        void* mem = malloc(100);
        auto mallocDeleter = [](void* p) {
            cout << "   🔒 Lambda: Freeing memory\n";
            free(p);
        };
        unique_ptr<void, decltype(mallocDeleter)> ptr(mem, mallocDeleter);

        cout << "   Memory allocated: " << ptr.get() << "\n";
    }
    cout << "   ✅ Memory freed with free()!\n";

    cout << "\n--- Example 3: Array deleter ---\n";
    {
        int* arr = new int[5]{1, 2, 3, 4, 5};
        auto arrayDeleter = [](int* p) {
            cout << "   🔒 Lambda: delete[] for array\n";
            delete[] p;
        };
        unique_ptr<int, decltype(arrayDeleter)> ptr(arr, arrayDeleter);

        cout << "   Array[0] = " << ptr.get()[0] << "\n";
    }
    cout << "   ✅ Array properly deleted!\n";

    cout << "\n--- Example 4: Logging deleter ---\n";
    {
        int* p = new int(42);
        auto loggingDeleter = [](int* ptr) {
            cout << "   📊 Lambda: Logging deletion of value " << *ptr << "\n";
            delete ptr;
        };
        unique_ptr<int, decltype(loggingDeleter)> smart_ptr(p, loggingDeleter);

        cout << "   Value: " << *smart_ptr << "\n";
    }

    cout << "\nLambda Deleter Benefits:\n";
    cout << "  ✅ Inline definition (no separate function)\n";
    cout << "  ✅ Can capture context (if needed)\n";
    cout << "  ✅ Type-safe\n";
    cout << "  ✅ Modern C++ style\n";
    cout << "  ⚠️  Verbose type declaration\n";
}

// ============================================
// EXAMPLE 3: FUNCTION POINTER DELETERS
// ============================================

void fileFree(FILE* f) {
    if (f) {
        cout << "   🔒 Function: Closing file\n";
        fclose(f);
    }
}

void mallocFree(void* p) {
    if (p) {
        cout << "   🔒 Function: Freeing malloc memory\n";
        free(p);
    }
}

void arrayDelete(int* p) {
    cout << "   🔒 Function: delete[] array\n";
    delete[] p;
}

void demonstrateFunctionPointerDeleters() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: FUNCTION POINTER       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "Function pointer deleters: Reusable, clear, classic!\n";
    cout << "\n";

    cout << "--- Example 1: FILE* deleter ---\n";
    {
        FILE* f = fopen("/tmp/test_func.txt", "w");
        if (f) {
            unique_ptr<FILE, decltype(&fileFree)> file(f, fileFree);
            fprintf(file.get(), "Function pointer deleter!\n");
            cout << "   File written\n";
        }
    }
    cout << "   ✅ Closed with function pointer!\n";

    cout << "\n--- Example 2: malloc deleter ---\n";
    {
        void* mem = malloc(256);
        unique_ptr<void, decltype(&mallocFree)> ptr(mem, mallocFree);
        cout << "   Memory at: " << ptr.get() << "\n";
    }
    cout << "   ✅ Freed with function pointer!\n";

    cout << "\n--- Example 3: Array deleter ---\n";
    {
        int* arr = new int[5]{10, 20, 30, 40, 50};
        unique_ptr<int, decltype(&arrayDelete)> ptr(arr, arrayDelete);
        cout << "   Array[2] = " << ptr.get()[2] << "\n";
    }
    cout << "   ✅ Array deleted correctly!\n";

    cout << "\nFunction Pointer Benefits:\n";
    cout << "  ✅ Reusable across multiple smart pointers\n";
    cout << "  ✅ Clear, named functions\n";
    cout << "  ✅ Easy to test independently\n";
    cout << "  ✅ No extra template instantiations\n";
    cout << "  ✅ Classic C++ style\n";
}

// ============================================
// EXAMPLE 4: FUNCTOR DELETERS
// ============================================

struct FileDeleter {
    void operator()(FILE* f) const {
        if (f) {
            cout << "   🔒 Functor: Closing file\n";
            fclose(f);
        }
    }
};

struct MallocDeleter {
    void operator()(void* p) const {
        if (p) {
            cout << "   🔒 Functor: Freeing memory\n";
            free(p);
        }
    }
};

struct ArrayDeleter {
    template<typename T>
    void operator()(T* p) const {
        cout << "   🔒 Functor: delete[] for array\n";
        delete[] p;
    }
};

struct LoggingDeleter {
    string name;

    LoggingDeleter(const string& n) : name(n) {}

    template<typename T>
    void operator()(T* p) const {
        cout << "   📊 Functor [" << name << "]: Deleting\n";
        delete p;
    }
};

void demonstrateFunctorDeleters() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: FUNCTOR DELETERS       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "Functor deleters: Flexible, stateful, powerful!\n";
    cout << "\n";

    cout << "--- Example 1: Stateless functor (FILE) ---\n";
    {
        FILE* f = fopen("/tmp/test_functor.txt", "w");
        if (f) {
            unique_ptr<FILE, FileDeleter> file(f);
            fprintf(file.get(), "Functor deleter!\n");
            cout << "   File written\n";
        }
    }
    cout << "   ✅ Stateless functor (zero overhead!)\n";

    cout << "\n--- Example 2: Stateless functor (malloc) ---\n";
    {
        void* mem = malloc(128);
        unique_ptr<void, MallocDeleter> ptr(mem);
        cout << "   Memory at: " << ptr.get() << "\n";
    }
    cout << "   ✅ Zero overhead functor!\n";

    cout << "\n--- Example 3: Template functor (arrays) ---\n";
    {
        int* arr = new int[5]{1, 2, 3, 4, 5};
        unique_ptr<int, ArrayDeleter> ptr(arr);
        cout << "   Array[0] = " << ptr.get()[0] << "\n";
    }
    cout << "   ✅ Template functor works for any type!\n";

    cout << "\n--- Example 4: Stateful functor (logging) ---\n";
    {
        int* p = new int(99);
        unique_ptr<int, LoggingDeleter> ptr(p, LoggingDeleter("MyResource"));
        cout << "   Value: " << *ptr << "\n";
    }
    cout << "   ✅ Stateful functor with data!\n";

    cout << "\nFunctor Benefits:\n";
    cout << "  ✅ Can be stateless (zero overhead)\n";
    cout << "  ✅ Can be stateful (store data)\n";
    cout << "  ✅ Can be templated (generic)\n";
    cout << "  ✅ Reusable type\n";
    cout << "  ✅ Clear intent\n";

    cout << "\nSize comparison:\n";
    cout << "  sizeof(unique_ptr<int>) = "
         << sizeof(unique_ptr<int>) << " bytes\n";
    cout << "  sizeof(unique_ptr<int, ArrayDeleter>) = "
         << sizeof(unique_ptr<int, ArrayDeleter>) << " bytes (stateless!)\n";
    cout << "  sizeof(unique_ptr<int, LoggingDeleter>) = "
         << sizeof(unique_ptr<int, LoggingDeleter>) << " bytes (has state)\n";
}

// ============================================
// EXAMPLE 5: UNIQUE_PTR VS SHARED_PTR DELETERS
// ============================================

void demonstrateUniqueVsShared() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: UNIQUE VS SHARED       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "UNIQUE_PTR DELETERS:\n";
    cout << "---\n";
    cout << "Deleter is part of the TYPE\n";
    cout << "\n";
    cout << "unique_ptr<FILE, FileDeleter> ptr1(f1);\n";
    cout << "unique_ptr<FILE, MallocDeleter> ptr2(f2);  // Different types!\n";
    cout << "// ptr1 = ptr2;  // ❌ ERROR: Different types!\n";
    cout << "\n";
    cout << "Characteristics:\n";
    cout << "  ✅ Zero overhead if deleter is stateless\n";
    cout << "  ✅ Type-safe at compile time\n";
    cout << "  ✅ Fast (no indirection)\n";
    cout << "  ❌ Different deleters = different types\n";
    cout << "  ❌ Can't convert between deleter types\n";
    cout << "\n";

    cout << "SHARED_PTR DELETERS:\n";
    cout << "---\n";
    cout << "Deleter stored in control block (type-erased)\n";
    cout << "\n";
    cout << "shared_ptr<FILE> ptr1(f1, FileDeleter{});\n";
    cout << "shared_ptr<FILE> ptr2(f2, MallocDeleter{});  // Same type!\n";
    cout << "ptr1 = ptr2;  // ✅ OK: Same type (FILE*)!\n";
    cout << "\n";
    cout << "Characteristics:\n";
    cout << "  ✅ Same type regardless of deleter\n";
    cout << "  ✅ Can convert between different deleters\n";
    cout << "  ✅ Flexible\n";
    cout << "  ❌ Small overhead (function pointer call)\n";
    cout << "  ❌ Control block stores deleter\n";
    cout << "\n";

    cout << "--- Testing unique_ptr ---\n";
    {
        auto deleter = [](int* p) {
            cout << "   🔒 Unique_ptr deleter\n";
            delete p;
        };
        unique_ptr<int, decltype(deleter)> ptr(new int(42), deleter);
        cout << "   Value: " << *ptr << "\n";
    }

    cout << "\n--- Testing shared_ptr ---\n";
    {
        shared_ptr<int> ptr(new int(99), [](int* p) {
            cout << "   🔒 Shared_ptr deleter\n";
            delete p;
        });
        cout << "   Value: " << *ptr << "\n";

        shared_ptr<int> ptr2 = ptr;  // ✅ Can copy!
        cout << "   Shared count: " << ptr.use_count() << "\n";
    }

    cout << "\nWHEN TO USE:\n";
    cout << "  unique_ptr: Performance critical, single ownership\n";
    cout << "  shared_ptr: Need flexibility, shared ownership\n";
}

// ============================================
// EXAMPLE 6: C API RESOURCE MANAGEMENT
// ============================================

class FileWrapper {
public:
    using FilePtr = unique_ptr<FILE, decltype(&fclose)>;

    static FilePtr open(const string& filename, const char* mode) {
        FILE* f = fopen(filename.c_str(), mode);
        if (!f) {
            throw runtime_error("Failed to open: " + filename);
        }
        cout << "   ✅ Opened: " << filename << "\n";
        return FilePtr(f, fclose);
    }
};

void demonstrateCAPIManagement() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: C API MANAGEMENT       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "Managing C API resources with custom deleters!\n";
    cout << "\n";

    cout << "--- FILE* management ---\n";
    try {
        auto file = FileWrapper::open("/tmp/test_capi.txt", "w");
        fprintf(file.get(), "Hello from C API!\n");
        fprintf(file.get(), "Automatic cleanup!\n");
        cout << "   Data written\n";
    } catch (const exception& e) {
        cout << "   Error: " << e.what() << "\n";
    }
    cout << "   💥 File automatically closed by deleter\n";

    cout << "\n--- malloc/free management ---\n";
    {
        auto deleter = [](void* p) {
            cout << "   💥 Calling free()\n";
            free(p);
        };
        unique_ptr<void, decltype(deleter)> mem(malloc(1024), deleter);
        cout << "   Memory allocated: " << mem.get() << "\n";
    }
    cout << "   ✅ Freed with free(), not delete!\n";

    cout << "\n--- Multiple FILE* resources ---\n";
    {
        auto f1 = FileWrapper::open("/tmp/input.txt", "w");
        auto f2 = FileWrapper::open("/tmp/output.txt", "w");

        fprintf(f1.get(), "Input data\n");
        fprintf(f2.get(), "Output data\n");

        cout << "   Both files written\n";
    }
    cout << "   💥 Both files closed (reverse order)\n";
    cout << "   ✅ Exception-safe!\n";

    cout << "\nCommon C API Patterns:\n";
    cout << "  FILE*:       fopen/fclose\n";
    cout << "  void*:       malloc/free\n";
    cout << "  int socket:  socket/close\n";
    cout << "  HANDLE:      CreateFile/CloseHandle (Windows)\n";
    cout << "  sqlite3*:    sqlite3_open/sqlite3_close\n";
    cout << "  All benefit from custom deleters!\n";
}

// ============================================
// EXAMPLE 7: ARRAY DELETERS
// ============================================

void demonstrateArrayDeleters() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 7: ARRAY DELETERS         ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "Arrays need delete[], not delete!\n";
    cout << "\n";

    cout << "--- Wrong: unique_ptr<T> for arrays ---\n";
    cout << "int* arr = new int[10];\n";
    cout << "unique_ptr<int> ptr(arr);  // ❌ WRONG!\n";
    cout << "// Calls 'delete' not 'delete[]'! Undefined behavior!\n";
    cout << "\n";

    cout << "--- Solution 1: unique_ptr<T[]> (C++11) ---\n";
    {
        unique_ptr<int[]> arr(new int[5]{1, 2, 3, 4, 5});
        cout << "   arr[0] = " << arr[0] << "\n";
        cout << "   arr[4] = " << arr[4] << "\n";
    }
    cout << "   💥 Automatically calls delete[]!\n";
    cout << "\n";

    cout << "--- Solution 2: Custom deleter ---\n";
    {
        auto deleter = [](int* p) {
            cout << "   💥 Custom: delete[] called\n";
            delete[] p;
        };
        unique_ptr<int, decltype(deleter)> arr(new int[5], deleter);
        arr.get()[0] = 100;
        cout << "   arr[0] = " << arr.get()[0] << "\n";
    }

    cout << "\n--- Solution 3: Use vector instead! (BEST) ---\n";
    {
        vector<int> vec = {1, 2, 3, 4, 5};
        cout << "   vec[0] = " << vec[0] << "\n";
        cout << "   vec.size() = " << vec.size() << "\n";
    }
    cout << "   💥 RAII automatic cleanup\n";
    cout << "   ✅ No need for custom deleter!\n";

    cout << "\nRECOMMENDATION:\n";
    cout << "  1st choice: vector<T>  (BEST!)\n";
    cout << "  2nd choice: unique_ptr<T[]>\n";
    cout << "  3rd choice: unique_ptr<T, custom_deleter>\n";
    cout << "  Never:      unique_ptr<T> for arrays ❌\n";
}

// ============================================
// EXAMPLE 8: NO-OP DELETERS
// ============================================

struct NoOpDeleter {
    template<typename T>
    void operator()(T*) const {
        // Do nothing - don't delete!
        cout << "   ⚠️  No-op deleter: Not deleting\n";
    }
};

void demonstrateNoOpDeleters() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 8: NO-OP DELETERS         ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "No-op deleters: For non-owning observation\n";
    cout << "\n";

    cout << "Use case: Observing without owning\n";
    cout << "  - Global resources\n";
    cout << "  - Stack-allocated objects\n";
    cout << "  - Borrowed pointers\n";
    cout << "\n";

    cout << "--- Example: Observing stack object ---\n";
    {
        int stackValue = 42;

        // Observe but don't own
        shared_ptr<int> observer(&stackValue, [](int*) {
            cout << "   ⚠️  Lambda no-op: Not deleting stack object\n";
        });

        cout << "   Observed value: " << *observer << "\n";
        cout << "   use_count: " << observer.use_count() << "\n";
    }
    cout << "   ✅ Stack object NOT deleted by smart pointer\n";

    cout << "\n--- Example: Observing global resource ---\n";
    {
        static int globalResource = 100;

        unique_ptr<int, NoOpDeleter> ptr(&globalResource);
        cout << "   Global value: " << *ptr << "\n";
    }
    cout << "   ✅ Global NOT deleted\n";

    cout << "\n--- Example: Multiple observers ---\n";
    {
        int value = 999;

        auto noOpDeleter = [](int*) {
            cout << "   ⚠️  Observer deleter\n";
        };

        shared_ptr<int> obs1(&value, noOpDeleter);
        shared_ptr<int> obs2 = obs1;
        shared_ptr<int> obs3 = obs1;

        cout << "   Observers: " << obs1.use_count() << "\n";
        cout << "   Value: " << *obs1 << "\n";
    }
    cout << "   ✅ All observers released, value NOT deleted\n";

    cout << "\nWARNING:\n";
    cout << "  Be careful! Object must outlive all observers!\n";
    cout << "  If object destroyed → observers become dangling!\n";
}

// ============================================
// EXAMPLE 9: STATEFUL DELETERS
// ============================================

class MetricsDeleter {
private:
    string resourceName;
    int* deleteCount;

public:
    MetricsDeleter(const string& name, int* counter)
        : resourceName(name), deleteCount(counter) {}

    template<typename T>
    void operator()(T* p) const {
        cout << "   📊 Metrics [" << resourceName << "]: Deleting\n";
        if (deleteCount) {
            (*deleteCount)++;
            cout << "   📊 Total deletions: " << *deleteCount << "\n";
        }
        delete p;
    }
};

class LoggerDeleter {
private:
    string prefix;

public:
    LoggerDeleter(const string& p) : prefix(p) {}

    template<typename T>
    void operator()(T* p) const {
        cout << "   📝 [" << prefix << "] Deleting pointer: " << p << "\n";
        delete p;
    }
};

void demonstrateStatefulDeleters() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 9: STATEFUL DELETERS      ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "Stateful deleters: Store data for cleanup logic\n";
    cout << "\n";

    cout << "--- Example 1: Metrics tracking ---\n";
    {
        int deleteCount = 0;

        {
            unique_ptr<int, MetricsDeleter> ptr1(
                new int(10),
                MetricsDeleter("Resource1", &deleteCount)
                );
            cout << "   ptr1 value: " << *ptr1 << "\n";
        }

        {
            unique_ptr<int, MetricsDeleter> ptr2(
                new int(20),
                MetricsDeleter("Resource2", &deleteCount)
                );
            cout << "   ptr2 value: " << *ptr2 << "\n";
        }

        cout << "   Final delete count: " << deleteCount << "\n";
    }

    cout << "\n--- Example 2: Logging with context ---\n";
    {
        unique_ptr<int, LoggerDeleter> ptr1(
            new int(100),
            LoggerDeleter("MAIN")
            );

        unique_ptr<int, LoggerDeleter> ptr2(
            new int(200),
            LoggerDeleter("WORKER")
            );

        cout << "   ptr1: " << *ptr1 << "\n";
        cout << "   ptr2: " << *ptr2 << "\n";
    }

    cout << "\n--- Size implications ---\n";
    cout << "sizeof(unique_ptr<int>): "
         << sizeof(unique_ptr<int>) << " bytes\n";
    cout << "sizeof(unique_ptr<int, MetricsDeleter>): "
         << sizeof(unique_ptr<int, MetricsDeleter>) << " bytes\n";
    cout << "  ⚠️  Stateful deleter increases size!\n";

    cout << "\nStateful Deleter Use Cases:\n";
    cout << "  1. Metrics and monitoring\n";
    cout << "  2. Logging with context\n";
    cout << "  3. Resource pool management\n";
    cout << "  4. Debug tracking\n";
    cout << "  5. Custom cleanup state\n";
}

// ============================================
// EXAMPLE 10: DELETER TYPE DEDUCTION
// ============================================

void demonstrateDeleterDeduction() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 10: TYPE DEDUCTION        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "Type deduction for deleters can be tricky!\n";
    cout << "\n";

    cout << "--- Problem: Lambda type deduction ---\n";
    cout << "auto deleter = [](int* p) { delete p; };\n";
    cout << "unique_ptr<int, DeleterType> ptr(new int, deleter);\n";
    cout << "// What type? Lambda type is unique and unnamed!\n";
    cout << "\n";

    cout << "--- Solution 1: decltype ---\n";
    {
        auto deleter = [](int* p) {
            cout << "   💥 decltype deleter\n";
            delete p;
        };
        unique_ptr<int, decltype(deleter)> ptr(new int(42), deleter);
        cout << "   Value: " << *ptr << "\n";
    }
    cout << "   ✅ decltype(deleter) captures lambda type\n";

    cout << "\n--- Solution 2: std::function (type erasure) ---\n";
    {
        function<void(int*)> deleter = [](int* p) {
            cout << "   💥 std::function deleter\n";
            delete p;
        };
        unique_ptr<int, decltype(deleter)> ptr(new int(99), deleter);
        cout << "   Value: " << *ptr << "\n";
    }
    cout << "   ⚠️  std::function has overhead!\n";

    cout << "\n--- Solution 3: Function pointer (simple) ---\n";
    {
        auto deleter = +[](int* p) {  // + converts to function pointer
            cout << "   💥 Function pointer deleter\n";
            delete p;
        };
        unique_ptr<int, decltype(deleter)> ptr(new int(77), deleter);
        cout << "   Value: " << *ptr << "\n";
    }
    cout << "   ✅ Clean function pointer type\n";

    cout << "\n--- Solution 4: shared_ptr (easiest) ---\n";
    {
        shared_ptr<int> ptr(new int(55), [](int* p) {
            cout << "   💥 shared_ptr: No type needed!\n";
            delete p;
        });
        cout << "   Value: " << *ptr << "\n";
    }
    cout << "   ✅ shared_ptr: Deleter type-erased!\n";

    cout << "\nRECOMMENDATIONS:\n";
    cout << "  unique_ptr: Use decltype or function pointer\n";
    cout << "  shared_ptr: Just pass the lambda (easiest!)\n";
    cout << "  Avoid std::function: Unnecessary overhead\n";
}

// ============================================
// EXAMPLE 11: PERFORMANCE IMPLICATIONS
// ============================================

void demonstratePerformance() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 11: PERFORMANCE           ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "UNIQUE_PTR SIZE:\n";
    cout << "---\n";
    cout << "Default deleter (stateless):\n";
    cout << "  sizeof(unique_ptr<int>) = "
         << sizeof(unique_ptr<int>) << " bytes\n";
    cout << "  → Same as raw pointer! Zero overhead! ✅\n";
    cout << "\n";

    cout << "Function pointer deleter:\n";
    auto fpDeleter = [](int* p) { delete p; };
    cout << "  sizeof(unique_ptr<int, decltype(&fpDeleter)>) = "
         << sizeof(unique_ptr<int, decltype(&fpDeleter)>) << " bytes\n";
    cout << "  → Still pointer-sized! ✅\n";
    cout << "\n";

    cout << "Stateless lambda/functor:\n";
    struct EmptyDeleter {
        void operator()(int* p) const { delete p; }
    };
    cout << "  sizeof(unique_ptr<int, EmptyDeleter>) = "
         << sizeof(unique_ptr<int, EmptyDeleter>) << " bytes\n";
    cout << "  → Same size! Empty base optimization! ✅\n";
    cout << "\n";

    cout << "Stateful deleter:\n";
    struct StatefulDeleter {
        int state;
        void operator()(int* p) const { delete p; }
    };
    cout << "  sizeof(unique_ptr<int, StatefulDeleter>) = "
         << sizeof(unique_ptr<int, StatefulDeleter>) << " bytes\n";
    cout << "  → Larger! Stores state! ⚠️\n";
    cout << "\n";

    cout << "SHARED_PTR SIZE:\n";
    cout << "---\n";
    cout << "  sizeof(shared_ptr<int>) = "
         << sizeof(shared_ptr<int>) << " bytes\n";
    cout << "  → Two pointers (object + control block)\n";
    cout << "  → Deleter stored in control block\n";
    cout << "  → Same size regardless of deleter ✅\n";
    cout << "\n";

    cout << "DELETION PERFORMANCE:\n";
    cout << "---\n";
    cout << "Default delete:\n";
    cout << "  - Direct 'delete p' call\n";
    cout << "  - Inlined by compiler\n";
    cout << "  - Fastest ✅\n";
    cout << "\n";
    cout << "Stateless custom deleter:\n";
    cout << "  - Direct function call\n";
    cout << "  - Can be inlined\n";
    cout << "  - Nearly same speed ✅\n";
    cout << "\n";
    cout << "Function pointer deleter:\n";
    cout << "  - Indirect call through pointer\n";
    cout << "  - Cannot inline\n";
    cout << "  - Slightly slower ⚠️\n";
    cout << "\n";
    cout << "shared_ptr deleter:\n";
    cout << "  - Indirect call through control block\n";
    cout << "  - Type-erased\n";
    cout << "  - Slower but flexible ⚠️\n";
    cout << "\n";

    cout << "RECOMMENDATIONS:\n";
    cout << "  Performance critical: Stateless deleter\n";
    cout << "  Need flexibility: shared_ptr\n";
    cout << "  Need state: Accept overhead or use shared_ptr\n";
}

// ============================================
// EXAMPLE 12: COMMON USE CASES
// ============================================

void demonstrateUseCases() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 12: COMMON USE CASES      ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "USE CASE 1: FILE HANDLES\n";
    cout << "---\n";
    cout << "using FilePtr = unique_ptr<FILE, decltype(&fclose)>;\n";
    cout << "FilePtr file(fopen(\"data.txt\", \"r\"), fclose);\n";
    cout << "✅ Automatic fclose on exception or scope exit\n";
    cout << "\n";

    cout << "USE CASE 2: MEMORY (malloc/free)\n";
    cout << "---\n";
    cout << "auto deleter = [](void* p) { free(p); };\n";
    cout << "unique_ptr<void, decltype(deleter)> mem(malloc(1024), deleter);\n";
    cout << "✅ Automatic free(), not delete\n";
    cout << "\n";

    cout << "USE CASE 3: ARRAYS\n";
    cout << "---\n";
    cout << "unique_ptr<int[]> arr(new int[100]);\n";
    cout << "✅ Automatic delete[], not delete\n";
    cout << "Better: vector<int> arr(100); // No deleter needed!\n";
    cout << "\n";

    cout << "USE CASE 4: DATABASE CONNECTIONS\n";
    cout << "---\n";
    cout << "auto deleter = [](sqlite3* db) { sqlite3_close(db); };\n";
    cout << "unique_ptr<sqlite3, decltype(deleter)> db(handle, deleter);\n";
    cout << "✅ Automatic connection close\n";
    cout << "\n";

    cout << "USE CASE 5: NETWORK SOCKETS\n";
    cout << "---\n";
    cout << "auto deleter = [](int* sock) { close(*sock); delete sock; };\n";
    cout << "unique_ptr<int, decltype(deleter)> socket(new int(fd), deleter);\n";
    cout << "✅ Automatic socket close\n";
    cout << "\n";

    cout << "USE CASE 6: WINDOWS HANDLES\n";
    cout << "---\n";
    cout << "auto deleter = [](HANDLE* h) { CloseHandle(*h); delete h; };\n";
    cout << "unique_ptr<HANDLE, decltype(deleter)> handle(new HANDLE(...), deleter);\n";
    cout << "✅ Automatic handle close\n";
    cout << "\n";

    cout << "USE CASE 7: RESOURCE POOLS\n";
    cout << "---\n";
    cout << "auto deleter = [&pool](Resource* r) { pool.release(r); };\n";
    cout << "unique_ptr<Resource, decltype(deleter)> res(pool.acquire(), deleter);\n";
    cout << "✅ Automatic return to pool\n";
    cout << "\n";

    cout << "USE CASE 8: LOGGING/METRICS\n";
    cout << "---\n";
    cout << "auto deleter = [](Widget* w) { \n";
    cout << "    LOG(\"Deleting widget\"); \n";
    cout << "    delete w; \n";
    cout << "};\n";
    cout << "unique_ptr<Widget, decltype(deleter)> widget(new Widget, deleter);\n";
    cout << "✅ Automatic logging on deletion\n";
}

// ============================================
// EXAMPLE 13: COMMON MISTAKES
// ============================================

void demonstrateMistakes() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 13: COMMON MISTAKES       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "MISTAKE 1: Wrong delete for arrays\n";
    cout << "---\n";
    cout << "❌ int* arr = new int[10];\n";
    cout << "   unique_ptr<int> ptr(arr);  // Calls delete, not delete[]!\n";
    cout << "\n";
    cout << "✅ unique_ptr<int[]> ptr(arr);  // Calls delete[]\n";
    cout << "✅ vector<int> vec(10);  // Even better!\n";
    cout << "\n";

    cout << "MISTAKE 2: Forgetting deleter parameter\n";
    cout << "---\n";
    cout << "❌ auto deleter = [](int* p) { delete p; };\n";
    cout << "   unique_ptr<int> ptr(new int, deleter);  // Wrong type!\n";
    cout << "\n";
    cout << "✅ unique_ptr<int, decltype(deleter)> ptr(new int, deleter);\n";
    cout << "\n";

    cout << "MISTAKE 3: Deleting stack objects\n";
    cout << "---\n";
    cout << "❌ int value = 42;\n";
    cout << "   shared_ptr<int> ptr(&value);  // Will try to delete stack!\n";
    cout << "\n";
    cout << "✅ shared_ptr<int> ptr(&value, [](int*){});  // No-op deleter\n";
    cout << "\n";

    cout << "MISTAKE 4: Using wrong deleter for C API\n";
    cout << "---\n";
    cout << "❌ FILE* f = fopen(\"file.txt\", \"r\");\n";
    cout << "   unique_ptr<FILE> ptr(f);  // Calls delete, not fclose!\n";
    cout << "\n";
    cout << "✅ unique_ptr<FILE, decltype(&fclose)> ptr(f, fclose);\n";
    cout << "\n";

    cout << "MISTAKE 5: Stateful deleter with unique_ptr\n";
    cout << "---\n";
    cout << "⚠️  struct BigDeleter {\n";
    cout << "      char buffer[1024];  // Large state!\n";
    cout << "      void operator()(int* p) { delete p; }\n";
    cout << "   };\n";
    cout << "   unique_ptr<int, BigDeleter> ptr;  // Huge overhead!\n";
    cout << "\n";
    cout << "✅ Use shared_ptr for stateful deleters\n";
    cout << "✅ Or keep deleter stateless\n";
    cout << "\n";

    cout << "MISTAKE 6: Capturing this in deleter\n";
    cout << "---\n";
    cout << "❌ class Manager {\n";
    cout << "     unique_ptr<Resource, DeleterType> res;\n";
    cout << "     Manager() : res(new Resource, [this](Resource* r) {\n";
    cout << "         this->cleanup(r);  // Dangerous! this might be gone!\n";
    cout << "     }) {}\n";
    cout << "   };\n";
    cout << "\n";
    cout << "✅ Use shared_ptr or ensure lifetime\n";
    cout << "\n";

    cout << "MISTAKE 7: Type mismatch\n";
    cout << "---\n";
    cout << "❌ auto deleter1 = [](int* p) { delete p; };\n";
    cout << "   auto deleter2 = [](int* p) { delete p; };\n";
    cout << "   unique_ptr<int, decltype(deleter1)> p1(new int, deleter1);\n";
    cout << "   unique_ptr<int, decltype(deleter2)> p2(new int, deleter2);\n";
    cout << "   p1 = move(p2);  // ERROR! Different lambda types!\n";
    cout << "\n";
    cout << "✅ Use same deleter or shared_ptr\n";
}

// ============================================
// EXAMPLE 14: BEST PRACTICES
// ============================================

void demonstrateBestPractices() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 14: BEST PRACTICES        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "✅ BEST PRACTICES:\n";
    cout << "\n";

    cout << "1. PREFER STATELESS DELETERS\n";
    cout << "   - Zero overhead with unique_ptr\n";
    cout << "   - Empty base optimization\n";
    cout << "   - Can be inlined\n";
    cout << "\n";

    cout << "2. USE TYPE ALIASES\n";
    cout << "   using FilePtr = unique_ptr<FILE, decltype(&fclose)>;\n";
    cout << "   FilePtr file(fopen(...), fclose);\n";
    cout << "   ✅ Reusable, clear intent\n";
    cout << "\n";

    cout << "3. SHARED_PTR FOR FLEXIBILITY\n";
    cout << "   shared_ptr<T> ptr(resource, deleter);\n";
    cout << "   ✅ Type-erased deleter\n";
    cout << "   ✅ Can copy/share\n";
    cout << "\n";

    cout << "4. UNIQUE_PTR FOR PERFORMANCE\n";
    cout << "   unique_ptr<T, Deleter> ptr(resource, deleter);\n";
    cout << "   ✅ Zero overhead if stateless\n";
    cout << "   ✅ Move-only, exclusive ownership\n";
    cout << "\n";

    cout << "5. USE STANDARD CONTAINERS\n";
    cout << "   ❌ unique_ptr<int[]> arr(new int[100]);\n";
    cout << "   ✅ vector<int> arr(100);\n";
    cout << "   No custom deleter needed!\n";
    cout << "\n";

    cout << "6. FACTORY FUNCTIONS\n";
    cout << "   auto makeFile(const string& name) {\n";
    cout << "       FILE* f = fopen(name.c_str(), \"r\");\n";
    cout << "       return unique_ptr<FILE, decltype(&fclose)>(f, fclose);\n";
    cout << "   }\n";
    cout << "   ✅ Encapsulates creation + deleter\n";
    cout << "\n";

    cout << "7. NO-OP FOR OBSERVATION\n";
    cout << "   shared_ptr<T> ptr(stack_obj, [](T*){});\n";
    cout << "   ✅ Observe without owning\n";
    cout << "   ⚠️  Ensure object outlives pointer!\n";
    cout << "\n";

    cout << "8. DOCUMENT OWNERSHIP\n";
    cout << "   // Takes ownership, closes on destruction\n";
    cout << "   unique_ptr<FILE, decltype(&fclose)> file(...);\n";
    cout << "   ✅ Clear contract\n";
    cout << "\n";

    cout << "9. CHECK FOR NULL\n";
    cout << "   auto deleter = [](FILE* f) {\n";
    cout << "       if (f) fclose(f);  // ✅ Check first!\n";
    cout << "   };\n";
    cout << "\n";

    cout << "10. TEST DELETION PATHS\n";
    cout << "    Verify deleter is called:\n";
    cout << "    - Normal exit\n";
    cout << "    - Exception thrown\n";
    cout << "    - Early return\n";
}

// ============================================
// EXAMPLE 15: REAL-WORLD EXAMPLE
// ============================================

class DatabaseHandle {
public:
    using DBPtr = unique_ptr<FILE, decltype(&fclose)>;

    static DBPtr open(const string& dbPath) {
        FILE* db = fopen(dbPath.c_str(), "r+");
        if (!db) {
            throw runtime_error("Failed to open database: " + dbPath);
        }
        cout << "   📂 Database opened: " << dbPath << "\n";
        return DBPtr(db, fclose);
    }
};

class Transaction {
private:
    DatabaseHandle::DBPtr db;
    bool committed = false;

public:
    Transaction(DatabaseHandle::DBPtr&& database)
        : db(move(database)) {
        cout << "   🔄 Transaction started\n";
    }

    ~Transaction() {
        if (!committed) {
            cout << "   ↩️  Rolling back transaction\n";
        }
    }

    void execute(const string& query) {
        if (db) {
            fprintf(db.get(), "%s\n", query.c_str());
            cout << "   ✏️  Executed: " << query << "\n";
        }
    }

    void commit() {
        cout << "   ✅ Transaction committed\n";
        committed = true;
    }
};

void demonstrateRealWorld() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 15: REAL-WORLD EXAMPLE    ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "Real-world: Database transaction with custom deleters\n";
    cout << "\n";

    cout << "--- Successful transaction ---\n";
    try {
        auto db = DatabaseHandle::open("/tmp/database.txt");
        Transaction txn(move(db));

        txn.execute("INSERT INTO users VALUES (...)");
        txn.execute("UPDATE accounts SET ...");
        txn.commit();

    } catch (const exception& e) {
        cout << "   ❌ Error: " << e.what() << "\n";
    }
    cout << "   💥 Database closed automatically\n";

    cout << "\n--- Failed transaction (auto-rollback) ---\n";
    try {
        auto db = DatabaseHandle::open("/tmp/database2.txt");
        Transaction txn(move(db));

        txn.execute("INSERT INTO users VALUES (...)");
        throw runtime_error("Network error!");
        txn.commit();  // Never reached

    } catch (const exception& e) {
        cout << "   ❌ Error: " << e.what() << "\n";
    }
    cout << "   💥 Transaction rolled back\n";
    cout << "   💥 Database closed automatically\n";

    cout << "\n✅ Benefits:\n";
    cout << "  - Automatic resource cleanup (fclose)\n";
    cout << "  - Exception-safe\n";
    cout << "  - Clear ownership semantics\n";
    cout << "  - RAII + custom deleters = bulletproof!\n";
}

// ============================================
// MAIN - RUN ALL EXAMPLES
// ============================================

int main() {
    cout << "╔══════════════════════════════════════════════════════════╗\n";
    cout << "║                                                          ║\n";
    cout << "║           CUSTOM DELETERS - COMPLETE GUIDE               ║\n";
    cout << "║                                                          ║\n";
    cout << "╚══════════════════════════════════════════════════════════╝\n";

    try {
        demonstrateProblem();                 // Why custom deleters needed
        demonstrateLambdaDeleters();          // Lambda deleters
        demonstrateFunctionPointerDeleters(); // Function pointer deleters
        demonstrateFunctorDeleters();         // Functor deleters
        demonstrateUniqueVsShared();          // unique_ptr vs shared_ptr
        demonstrateCAPIManagement();          // C API resources
        demonstrateArrayDeleters();           // Array handling
        demonstrateNoOpDeleters();            // No-op deleters
        demonstrateStatefulDeleters();        // Stateful deleters
        demonstrateDeleterDeduction();        // Type deduction
        demonstratePerformance();             // Performance implications
        demonstrateUseCases();                // Common use cases
        demonstrateMistakes();                // Common mistakes
        demonstrateBestPractices();           // Best practices
        demonstrateRealWorld();               // Real-world example

    } catch (const exception& e) {
        cout << "💥 Exception: " << e.what() << "\n";
    }

    cout << "\n╔══════════════════════════════════════════════════════════╗\n";
    cout << "║                    KEY TAKEAWAYS                         ║\n";
    cout << "╚══════════════════════════════════════════════════════════╝\n\n";

    cout << "1. WHAT ARE CUSTOM DELETERS?\n";
    cout << "   Specialized cleanup logic for smart pointers\n";
    cout << "   - Beyond default 'delete' or 'delete[]'\n";
    cout << "   - For C APIs, arrays, custom cleanup\n";
    cout << "   - Lambda, function pointer, or functor\n";
    cout << "\n";

    cout << "2. WHEN TO USE:\n";
    cout << "   ✅ C API resources (FILE*, malloc, etc.)\n";
    cout << "   ✅ Arrays (delete[] vs delete)\n";
    cout << "   ✅ Custom cleanup logic\n";
    cout << "   ✅ Non-owning observation (no-op)\n";
    cout << "   ✅ Platform resources (handles, sockets)\n";
    cout << "\n";

    cout << "3. UNIQUE_PTR DELETERS:\n";
    cout << "   unique_ptr<T, DeleterType> ptr(res, deleter);\n";
    cout << "   - Deleter is part of TYPE\n";
    cout << "   - Zero overhead if stateless\n";
    cout << "   - Fast, type-safe\n";
    cout << "   - Different deleters = different types\n";
    cout << "\n";

    cout << "4. SHARED_PTR DELETERS:\n";
    cout << "   shared_ptr<T> ptr(res, deleter);\n";
    cout << "   - Deleter stored in control block\n";
    cout << "   - Type-erased\n";
    cout << "   - Can convert between deleters\n";
    cout << "   - Small overhead\n";
    cout << "\n";

    cout << "5. THREE DELETER STYLES:\n";
    cout << "   Lambda:           Inline, convenient\n";
    cout << "   Function pointer: Reusable, clear\n";
    cout << "   Functor:          Flexible, can be stateful\n";
    cout << "\n";

    cout << "6. PERFORMANCE:\n";
    cout << "   Stateless deleter:  Zero overhead ✅\n";
    cout << "   Stateful deleter:   Increases size ⚠️\n";
    cout << "   Function pointer:   Slight indirection\n";
    cout << "   shared_ptr:         Type-erased overhead\n";
    cout << "\n";

    cout << "7. COMMON PATTERNS:\n";
    cout << "   FILE*:     unique_ptr<FILE, decltype(&fclose)>\n";
    cout << "   malloc:    lambda with free()\n";
    cout << "   Arrays:    unique_ptr<T[]> or vector<T>\n";
    cout << "   No-op:     lambda with empty body\n";
    cout << "   Logging:   functor with state\n";
    cout << "\n";

    cout << "8. TYPE DEDUCTION:\n";
    cout << "   unique_ptr: Use decltype(deleter)\n";
    cout << "   shared_ptr: Just pass lambda (easiest!)\n";
    cout << "   Avoid std::function (unnecessary overhead)\n";
    cout << "\n";

    cout << "9. BEST PRACTICES:\n";
    cout << "   ✅ Prefer stateless deleters\n";
    cout << "   ✅ Use type aliases for readability\n";
    cout << "   ✅ shared_ptr for flexibility\n";
    cout << "   ✅ unique_ptr for performance\n";
    cout << "   ✅ Check for null in deleter\n";
    cout << "   ✅ Document ownership\n";
    cout << "\n";

    cout << "10. REMEMBER:\n";
    cout << "    Custom deleters = RAII for ANY resource!\n";
    cout << "    \n";
    cout << "    Not just memory:\n";
    cout << "    Files, locks, sockets, handles, connections,\n";
    cout << "    database transactions, GPU resources...\n";
    cout << "    If it needs cleanup → use custom deleter!\n";
    cout << "\n";

    return 0;
}

/*
COMPILATION:
    g++ -std=c++17 -Wall -Wextra custom_deleters.cpp -o custom_deleters

RUN:
    ./custom_deleters

TOPICS COVERED:
    ✅ What are custom deleters
    ✅ Why they're needed
    ✅ Lambda deleters
    ✅ Function pointer deleters
    ✅ Functor deleters
    ✅ unique_ptr vs shared_ptr deleters
    ✅ C API resource management
    ✅ Array deleters
    ✅ No-op deleters
    ✅ Stateful deleters
    ✅ Type deduction
    ✅ Performance implications
    ✅ Common use cases
    ✅ Common mistakes
    ✅ Best practices
    ✅ Real-world examples

OUTPUT: Complete demonstrations with emojis and clear sections!
*/
