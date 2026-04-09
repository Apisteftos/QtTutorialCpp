#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <mutex>
#include <chrono>
using namespace std;

// ============================================
// RAII: Resource Acquisition Is Initialization
// ============================================

// ============================================
// EXAMPLE 1: FILE HANDLE RAII
// ============================================

class FileHandle {
private:
    FILE* file;
    string filename;

public:
    // Constructor: ACQUIRE resource
    FileHandle(const string& fname, const char* mode)
        : filename(fname), file(nullptr) {
        cout << "📂 Opening file: " << filename << "\n";
        file = fopen(fname.c_str(), mode);
        if (!file) {
            throw runtime_error("Failed to open file: " + filename);
        }
        cout << "✅ File opened successfully\n";
    }

    // Destructor: RELEASE resource (automatically called!)
    ~FileHandle() {
        if (file) {
            cout << "📁 Closing file: " << filename << "\n";
            fclose(file);
            file = nullptr;
        }
    }

    // Delete copy operations (file handles shouldn't be copied)
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;

    // Write to file
    void write(const string& data) {
        if (file) {
            fprintf(file, "%s", data.c_str());
            cout << "✍️  Written: " << data;
        }
    }

    // Read from file
    string read() {
        if (!file) return "";

        fseek(file, 0, SEEK_END);
        long size = ftell(file);
        fseek(file, 0, SEEK_SET);

        string content(size, '\0');
        fread(&content[0], 1, size, file);
        return content;
    }

    FILE* get() const { return file; }
};

void demonstrateFileRAII() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: FILE HANDLE RAII      ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Writing to file ---\n";
    try {
        FileHandle file("example.txt", "w");
        file.write("Hello, RAII!\n");
        file.write("Automatic resource management!\n");
        file.write("No manual cleanup needed!\n");

        // File automatically closed when 'file' goes out of scope
        // Even if exception is thrown!

    } catch (const exception& e) {
        cout << "❌ Error: " << e.what() << "\n";
    }
    cout << "✓ File is guaranteed to be closed!\n";

    cout << "\n--- Reading from file ---\n";
    try {
        FileHandle file("example.txt", "r");
        string content = file.read();
        cout << "📄 Content:\n" << content;

    } catch (const exception& e) {
        cout << "❌ Error: " << e.what() << "\n";
    }
}

// ============================================
// EXAMPLE 2: MUTEX LOCK GUARD
// ============================================

class MutexGuard {
private:
    mutex& mtx;
    string name;

public:
    // Constructor: ACQUIRE lock
    MutexGuard(mutex& m, const string& n = "Lock") : mtx(m), name(n) {
        cout << "🔒 [" << name << "] Acquiring lock...\n";
        mtx.lock();
        cout << "✅ [" << name << "] Lock acquired\n";
    }

    // Destructor: RELEASE lock
    ~MutexGuard() {
        cout << "🔓 [" << name << "] Releasing lock...\n";
        mtx.unlock();
        cout << "✅ [" << name << "] Lock released\n";
    }

    // Delete copy operations
    MutexGuard(const MutexGuard&) = delete;
    MutexGuard& operator=(const MutexGuard&) = delete;
};

mutex globalMutex;
int sharedCounter = 0;

void demonstrateLockRAII() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: MUTEX LOCK GUARD       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Critical Section 1 ---\n";
    {
        MutexGuard guard(globalMutex, "CriticalSection1");

        sharedCounter++;
        cout << "📊 Incremented counter: " << sharedCounter << "\n";
        cout << "💼 Doing protected work...\n";

        // Even if exception thrown here, lock is released!

    }  // Lock automatically released here!
    cout << "✓ Exited critical section\n";

    cout << "\n--- Critical Section 2 ---\n";
    {
        MutexGuard guard(globalMutex, "CriticalSection2");

        sharedCounter += 5;
        cout << "📊 Incremented counter: " << sharedCounter << "\n";
        cout << "💼 Doing more protected work...\n";
    }
    cout << "✓ Exited critical section\n";

    cout << "\n📈 Final counter value: " << sharedCounter << "\n";
}

// ============================================
// EXAMPLE 3: TIMER (SCOPE-BASED PROFILING)
// ============================================

class Timer {
private:
    string name;
    chrono::time_point<chrono::high_resolution_clock> start;

public:
    // Constructor: START timer
    Timer(const string& n) : name(n) {
        start = chrono::high_resolution_clock::now();
        cout << "⏱️  [" << name << "] Timer started\n";
    }

    // Destructor: STOP timer and report
    ~Timer() {
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
        cout << "⏱️  [" << name << "] Elapsed time: " << duration.count() << "ms\n";
    }
};

void demonstrateTimerRAII() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: TIMER (PROFILING)     ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Operation 1: Fast Task ---\n";
    {
        Timer t("FastTask");

        // Simulate some work
        int sum = 0;
        for (int i = 0; i < 100000; i++) {
            sum += i;
        }
        cout << "   Result: " << sum << "\n";

    }  // Timer automatically stops and reports!

    cout << "\n--- Operation 2: Slower Task ---\n";
    {
        Timer t("SlowerTask");

        // Simulate more work
        int sum = 0;
        for (int i = 0; i < 1000000; i++) {
            sum += i;
        }
        cout << "   Result: " << sum << "\n";
    }

    cout << "\n--- Nested Timers ---\n";
    {
        Timer outer("OuterOperation");

        cout << "   Starting inner operations...\n";
        {
            Timer inner("InnerOperation1");
            int x = 0;
            for (int i = 0; i < 500000; i++) x += i;
        }

        {
            Timer inner("InnerOperation2");
            int x = 0;
            for (int i = 0; i < 500000; i++) x += i;
        }
    }
}

// ============================================
// EXAMPLE 4: DATABASE CONNECTION
// ============================================

class DatabaseConnection {
private:
    string connectionString;
    bool connected;
    int transactionCount;

public:
    // Constructor: OPEN connection
    DatabaseConnection(const string& connStr)
        : connectionString(connStr), connected(false), transactionCount(0) {
        cout << "🗄️  Connecting to database...\n";
        cout << "   Connection string: " << connectionString << "\n";

        // Simulate connection
        connected = true;
        cout << "✅ Database connected successfully!\n";
    }

    // Destructor: CLOSE connection
    ~DatabaseConnection() {
        if (connected) {
            if (transactionCount > 0) {
                cout << "⚠️  Rolling back " << transactionCount << " uncommitted transactions\n";
            }
            cout << "🗄️  Closing database connection...\n";
            connected = false;
            cout << "✅ Database connection closed\n";
        }
    }

    // Delete copy, allow move
    DatabaseConnection(const DatabaseConnection&) = delete;
    DatabaseConnection& operator=(const DatabaseConnection&) = delete;

    void executeQuery(const string& query) {
        if (connected) {
            cout << "📝 Executing query: " << query << "\n";
            transactionCount++;
        } else {
            cout << "❌ Cannot execute query: not connected\n";
        }
    }

    void commit() {
        if (connected && transactionCount > 0) {
            cout << "✅ Committing " << transactionCount << " transactions\n";
            transactionCount = 0;
        }
    }
};

void demonstrateDatabaseRAII() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: DATABASE CONNECTION    ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Successful Transaction ---\n";
    {
        DatabaseConnection db("localhost:5432/mydb");
        db.executeQuery("SELECT * FROM users");
        db.executeQuery("INSERT INTO logs VALUES (1, 'test')");
        db.commit();

        // Connection automatically closed when scope ends!
    }

    cout << "\n--- Transaction with Exception ---\n";
    try {
        DatabaseConnection db("localhost:5432/testdb");
        db.executeQuery("UPDATE accounts SET balance = 1000");
        db.executeQuery("DELETE FROM logs WHERE id = 5");

        // Simulate error before commit
        throw runtime_error("Network error!");

        db.commit();  // Never reached

    } catch (const exception& e) {
        cout << "❌ Exception: " << e.what() << "\n";
        // Database connection still closes properly (RAII!)
    }
    cout << "✓ Connection closed despite exception\n";
}

// ============================================
// EXAMPLE 5: MEMORY RESOURCE (CUSTOM ALLOCATOR)
// ============================================

class MemoryPool {
private:
    void* memory;
    size_t size;
    string name;

public:
    // Constructor: ALLOCATE memory
    MemoryPool(size_t s, const string& n)
        : size(s), name(n) {
        cout << "💾 [" << name << "] Allocating " << size << " bytes\n";
        memory = malloc(size);
        if (!memory) {
            throw bad_alloc();
        }
        cout << "✅ [" << name << "] Memory allocated at " << memory << "\n";
    }

    // Destructor: FREE memory
    ~MemoryPool() {
        if (memory) {
            cout << "💾 [" << name << "] Freeing " << size << " bytes at " << memory << "\n";
            free(memory);
            memory = nullptr;
        }
    }

    // Delete copy operations
    MemoryPool(const MemoryPool&) = delete;
    MemoryPool& operator=(const MemoryPool&) = delete;

    void* getMemory() const { return memory; }
    size_t getSize() const { return size; }
};

void demonstrateMemoryPoolRAII() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: MEMORY POOL            ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Allocating Memory Pools ---\n";
    {
        MemoryPool pool1(1024, "Pool1");
        MemoryPool pool2(2048, "Pool2");

        cout << "\n💼 Using memory pools...\n";
        cout << "   Pool1: " << pool1.getSize() << " bytes\n";
        cout << "   Pool2: " << pool2.getSize() << " bytes\n";

        // Memory automatically freed when scope ends!
    }
    cout << "\n✓ All memory automatically freed!\n";
}

// ============================================
// EXAMPLE 6: COMPARING BAD vs GOOD
// ============================================

void badExample() {
    cout << "\n❌ BAD EXAMPLE - Manual Management\n";
    cout << "==================================\n";

    FILE* file = fopen("manual.txt", "w");
    if (!file) {
        cout << "Failed to open file\n";
        return;
    }

    fprintf(file, "Hello\n");

    // Simulate error
    if (true) {
        cout << "💥 Early return - FILE LEAKED!\n";
        return;  // 💥 FORGOT TO CLOSE FILE!
    }

    fclose(file);  // Never reached!
}

void goodExample() {
    cout << "\n✅ GOOD EXAMPLE - RAII\n";
    cout << "======================\n";

    try {
        FileHandle file("raii.txt", "w");
        file.write("Hello\n");

        // Simulate error
        if (true) {
            cout << "✓ Early return - file automatically closed!\n";
            return;  // ✓ FILE AUTOMATICALLY CLOSED
        }

    } catch (const exception& e) {
        cout << "Error: " << e.what() << "\n";
    }
    // File guaranteed to be closed!
}

void demonstrateBadVsGood() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: BAD vs GOOD            ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    badExample();
    goodExample();
}

// ============================================
// EXAMPLE 7: STANDARD LIBRARY RAII
// ============================================

void demonstrateStandardRAII() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 7: STANDARD LIBRARY RAII  ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Smart Pointers (RAII) ---\n";
    {
        auto ptr = make_unique<int>(42);
        cout << "🔢 unique_ptr value: " << *ptr << "\n";

        auto shared = make_shared<string>("RAII rocks!");
        cout << "📝 shared_ptr value: " << *shared << "\n";

        // Automatically deleted when scope ends
    }
    cout << "✓ Smart pointers automatically deleted\n";

    cout << "\n--- Containers (RAII) ---\n";
    {
        vector<int> numbers = {1, 2, 3, 4, 5};
        cout << "📦 Vector size: " << numbers.size() << "\n";

        string text = "Dynamic memory managed!";
        cout << "📝 String: " << text << "\n";

        // Automatically freed when scope ends
    }
    cout << "✓ Containers automatically freed\n";

    cout << "\n--- File Streams (RAII) ---\n";
    {
        ofstream outFile("stream_example.txt");
        outFile << "File streams use RAII!\n";
        outFile << "Automatically closed!\n";
        cout << "📄 Written to file\n";

        // File automatically closed when scope ends
    }
    cout << "✓ File stream automatically closed\n";

    cout << "\n--- Lock Guard (RAII) ---\n";
    {
        mutex mtx;
        {
            lock_guard<mutex> lock(mtx);
            cout << "🔒 Lock acquired via lock_guard\n";
            // Do thread-safe work
        }
        cout << "✓ Lock automatically released\n";
    }
}

// ============================================
// MAIN DEMONSTRATION
// ============================================

int main() {
    cout << "========================================\n";
    cout << "RAII (Resource Acquisition Is Initialization)\n";
    cout << "========================================\n";

    demonstrateFileRAII();
    demonstrateLockRAII();
    demonstrateTimerRAII();
    demonstrateDatabaseRAII();
    demonstrateMemoryPoolRAII();
    demonstrateBadVsGood();
    demonstrateStandardRAII();

    // ============================================
    // Comprehensive Summary
    // ============================================
    cout << "\n========================================\n";
    cout << "WHAT IS RAII?\n";
    cout << "========================================\n";
    cout << "RAII = Resource Acquisition Is Initialization\n";
    cout << "\nCore Principle:\n";
    cout << "  • ACQUIRE resource in CONSTRUCTOR\n";
    cout << "  • RELEASE resource in DESTRUCTOR\n";
    cout << "  • Automatic, deterministic cleanup\n";
    cout << "  • Exception-safe by design\n";

    cout << "\n========================================\n";
    cout << "RAII LIFECYCLE\n";
    cout << "========================================\n";
    cout << "1. Object created → Constructor runs\n";
    cout << "   └─ Resource acquired (file, memory, lock, etc.)\n";
    cout << "\n";
    cout << "2. Object used → Resource accessed safely\n";
    cout << "   └─ Normal operations\n";
    cout << "\n";
    cout << "3. Object destroyed → Destructor runs\n";
    cout << "   └─ Resource automatically released\n";
    cout << "   └─ Happens when:\n";
    cout << "      • Scope ends\n";
    cout << "      • Exception thrown\n";
    cout << "      • Return statement\n";
    cout << "      • Program exits\n";

    cout << "\n========================================\n";
    cout << "WHY RAII?\n";
    cout << "========================================\n";
    cout << "Problem without RAII:\n";
    cout << "  FILE* f = fopen(\"file.txt\", \"r\");\n";
    cout << "  // ... do work ...\n";
    cout << "  if (error) return;  // 💥 FORGOT TO CLOSE!\n";
    cout << "  fclose(f);          // Never reached!\n";
    cout << "\n";
    cout << "Solution with RAII:\n";
    cout << "  FileHandle f(\"file.txt\", \"r\");\n";
    cout << "  // ... do work ...\n";
    cout << "  if (error) return;  // ✅ Automatically closed!\n";
    cout << "  // Destructor called automatically\n";

    cout << "\n========================================\n";
    cout << "BENEFITS OF RAII\n";
    cout << "========================================\n";
    cout << "✅ Exception Safety\n";
    cout << "   • Resources cleaned up even if exception thrown\n";
    cout << "   • No try-finally needed (like in Java/C#)\n";
    cout << "\n";
    cout << "✅ No Memory Leaks\n";
    cout << "   • Impossible to forget cleanup\n";
    cout << "   • Automatic when scope ends\n";
    cout << "\n";
    cout << "✅ No Dangling Resources\n";
    cout << "   • Files always closed\n";
    cout << "   • Locks always released\n";
    cout << "   • Connections always terminated\n";
    cout << "\n";
    cout << "✅ Clear Ownership\n";
    cout << "   • Who owns resource is obvious\n";
    cout << "   • Lifetime tied to object lifetime\n";
    cout << "\n";
    cout << "✅ Less Code\n";
    cout << "   • No manual cleanup code\n";
    cout << "   • More maintainable\n";
    cout << "\n";
    cout << "✅ Composable\n";
    cout << "   • RAII objects can contain other RAII objects\n";
    cout << "   • Cleanup happens in correct order\n";

    cout << "\n========================================\n";
    cout << "TYPES OF RESOURCES\n";
    cout << "========================================\n";
    cout << "Memory:\n";
    cout << "  • unique_ptr, shared_ptr, weak_ptr\n";
    cout << "  • vector, string, map, etc.\n";
    cout << "\n";
    cout << "Files:\n";
    cout << "  • ifstream, ofstream, fstream\n";
    cout << "  • Custom FileHandle class\n";
    cout << "\n";
    cout << "Locks/Synchronization:\n";
    cout << "  • lock_guard, unique_lock, scoped_lock\n";
    cout << "  • Custom MutexGuard\n";
    cout << "\n";
    cout << "Network:\n";
    cout << "  • Socket handles\n";
    cout << "  • Connection objects\n";
    cout << "\n";
    cout << "Database:\n";
    cout << "  • Connection objects\n";
    cout << "  • Transaction scopes\n";
    cout << "\n";
    cout << "Graphics:\n";
    cout << "  • OpenGL contexts\n";
    cout << "  • DirectX resources\n";
    cout << "  • Textures, buffers\n";

    cout << "\n========================================\n";
    cout << "STANDARD LIBRARY RAII EXAMPLES\n";
    cout << "========================================\n";
    cout << "Smart Pointers:\n";
    cout << "  unique_ptr<T>  - Exclusive ownership\n";
    cout << "  shared_ptr<T>  - Shared ownership\n";
    cout << "  weak_ptr<T>    - Non-owning observer\n";
    cout << "\n";
    cout << "Containers:\n";
    cout << "  vector, string, map, set, etc.\n";
    cout << "  Automatic memory management\n";
    cout << "\n";
    cout << "File Streams:\n";
    cout << "  ifstream, ofstream, fstream\n";
    cout << "  Automatically close files\n";
    cout << "\n";
    cout << "Synchronization:\n";
    cout << "  lock_guard<mutex>\n";
    cout << "  unique_lock<mutex>\n";
    cout << "  scoped_lock (C++17)\n";

    cout << "\n========================================\n";
    cout << "CREATING RAII CLASSES\n";
    cout << "========================================\n";
    cout << "Pattern:\n";
    cout << "  class RAIIWrapper {\n";
    cout << "  private:\n";
    cout << "      ResourceType* resource;\n";
    cout << "  public:\n";
    cout << "      // Constructor: ACQUIRE\n";
    cout << "      RAIIWrapper() {\n";
    cout << "          resource = acquireResource();\n";
    cout << "      }\n";
    cout << "\n";
    cout << "      // Destructor: RELEASE\n";
    cout << "      ~RAIIWrapper() {\n";
    cout << "          releaseResource(resource);\n";
    cout << "      }\n";
    cout << "\n";
    cout << "      // Delete copy (or implement properly)\n";
    cout << "      RAIIWrapper(const RAIIWrapper&) = delete;\n";
    cout << "      RAIIWrapper& operator=(const RAIIWrapper&) = delete;\n";
    cout << "  };\n";

    cout << "\n========================================\n";
    cout << "WHEN TO USE RAII\n";
    cout << "========================================\n";
    cout << "Use RAII whenever you have:\n";
    cout << "  ✓ Dynamic memory allocation\n";
    cout << "  ✓ File operations\n";
    cout << "  ✓ Network sockets\n";
    cout << "  ✓ Database connections\n";
    cout << "  ✓ Mutex locks\n";
    cout << "  ✓ Graphics resources\n";
    cout << "  ✓ Any resource that needs cleanup\n";
    cout << "\n";
    cout << "Rule of thumb:\n";
    cout << "  If you write 'new', write a RAII wrapper!\n";
    cout << "  Better yet: Use existing RAII types!\n";

    cout << "\n========================================\n";
    cout << "RAII vs OTHER LANGUAGES\n";
    cout << "========================================\n";
    cout << "C++ (RAII):\n";
    cout << "  • Deterministic, automatic cleanup\n";
    cout << "  • Destructor called at end of scope\n";
    cout << "  • Zero overhead\n";
    cout << "\n";
    cout << "Java/C# (Garbage Collection):\n";
    cout << "  • Non-deterministic cleanup\n";
    cout << "  • Need try-finally or using blocks\n";
    cout << "  • Runtime overhead\n";
    cout << "\n";
    cout << "C (Manual):\n";
    cout << "  • Must manually free resources\n";
    cout << "  • Easy to forget\n";
    cout << "  • Error-prone\n";
    cout << "\n";
    cout << "RAII is C++'s superpower! 🦸\n";

    cout << "\n========================================\n";
    cout << "COMMON MISTAKES\n";
    cout << "========================================\n";
    cout << "❌ Returning pointers to local RAII objects\n";
    cout << "   Bad:  MyClass* getObject() {\n";
    cout << "           MyClass obj;  // Local!\n";
    cout << "           return &obj;  // Dangling pointer!\n";
    cout << "         }\n";
    cout << "\n";
    cout << "❌ Not making destructor virtual in base class\n";
    cout << "   Bad:  class Base {\n";
    cout << "           ~Base() { }  // Not virtual!\n";
    cout << "         };\n";
    cout << "   Fix:  virtual ~Base() { }\n";
    cout << "\n";
    cout << "❌ Forgetting to delete copy operations\n";
    cout << "   Bad:  Copying RAII objects without proper implementation\n";
    cout << "   Fix:  Delete copy or implement Rule of Five\n";
    cout << "\n";
    cout << "❌ Throwing exceptions in destructor\n";
    cout << "   Bad:  ~MyClass() { throw ...; }  // Undefined behavior!\n";
    cout << "   Fix:  Destructors should not throw\n";

    cout << "\n========================================\n";
    cout << "BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "1. Always prefer RAII over manual management\n";
    cout << "2. Use smart pointers (unique_ptr, shared_ptr)\n";
    cout << "3. Use standard containers (vector, string)\n";
    cout << "4. Make destructors virtual in base classes\n";
    cout << "5. Delete copy operations or implement Rule of Five\n";
    cout << "6. Never throw exceptions from destructors\n";
    cout << "7. Mark destructors noexcept (implicit in C++11+)\n";
    cout << "8. Acquire resources in constructor only\n";
    cout << "9. Release resources in destructor only\n";
    cout << "10. Compose RAII objects for complex resources\n";

    cout << "\n========================================\n";
    cout << "RAII CHECKLIST\n";
    cout << "========================================\n";
    cout << "When creating a RAII class:\n";
    cout << "  ☑ Constructor acquires resource\n";
    cout << "  ☑ Destructor releases resource\n";
    cout << "  ☑ Copy operations deleted or proper\n";
    cout << "  ☑ Move operations if needed\n";
    cout << "  ☑ Destructor is noexcept\n";
    cout << "  ☑ Exception-safe acquisition\n";
    cout << "  ☑ Null-state handling\n";

    cout << "\n========================================\n";
    cout << "STRUCTURE\n";
    cout << "========================================\n";
    cout << "┌─────────────────────────┐\n";
    cout << "│     RAII Object         │\n";
    cout << "│                         │\n";
    cout << "│  Constructor {          │  ← ACQUIRE\n";
    cout << "│    acquire_resource()   │    resource\n";
    cout << "│  }                      │\n";
    cout << "│                         │\n";
    cout << "│  // Use resource        │  ← USE\n";
    cout << "│  void doWork() { ... }  │    resource\n";
    cout << "│                         │\n";
    cout << "│  ~Destructor {          │  ← RELEASE\n";
    cout << "│    release_resource()   │    resource\n";
    cout << "│  }                      │    (automatic!)\n";
    cout << "└─────────────────────────┘\n";

    cout << "\n========================================\n";
    cout << "MEMORY MANAGEMENT EVOLUTION\n";
    cout << "========================================\n";
    cout << "C-style (Manual):\n";
    cout << "  int* p = (int*)malloc(sizeof(int));\n";
    cout << "  *p = 42;\n";
    cout << "  free(p);  // Must remember!\n";
    cout << "\n";
    cout << "C++ Old (New/Delete):\n";
    cout << "  int* p = new int(42);\n";
    cout << "  delete p;  // Must remember!\n";
    cout << "\n";
    cout << "C++ Modern (RAII):\n";
    cout << "  auto p = make_unique<int>(42);\n";
    cout << "  // Automatic cleanup! 🎉\n";

    cout << "\n========================================\n";
    cout << "REAL-WORLD EXAMPLES\n";
    cout << "========================================\n";
    cout << "Game Engine:\n";
    cout << "  • Texture loading/unloading\n";
    cout << "  • Audio buffer management\n";
    cout << "  • Graphics context setup/teardown\n";
    cout << "\n";
    cout << "Web Server:\n";
    cout << "  • Socket connection management\n";
    cout << "  • Database connection pooling\n";
    cout << "  • Request/response lifecycle\n";
    cout << "\n";
    cout << "Desktop Application:\n";
    cout << "  • Window handles\n";
    cout << "  • File operations\n";
    cout << "  • Registry keys (Windows)\n";
    cout << "\n";
    cout << "Embedded Systems:\n";
    cout << "  • Hardware peripheral access\n";
    cout << "  • Interrupt handling\n";
    cout << "  • Memory-mapped I/O\n";

    cout << "\n========================================\n";
    cout << "KEY TAKEAWAYS\n";
    cout << "========================================\n";
    cout << "1. RAII = Constructor acquires, Destructor releases\n";
    cout << "2. Automatic, deterministic cleanup\n";
    cout << "3. Exception-safe by design\n";
    cout << "4. Prevents resource leaks\n";
    cout << "5. Zero runtime overhead\n";
    cout << "6. C++'s most powerful idiom\n";
    cout << "7. Use it everywhere!\n";

    cout << "\n========================================\n";
    cout << "THE RAII PHILOSOPHY\n";
    cout << "========================================\n";
    cout << "\"If you can't make it easy,\n";
    cout << " make it impossible to get wrong.\"\n";
    cout << "\n";
    cout << "RAII makes it impossible to:\n";
    cout << "  • Forget to release resources\n";
    cout << "  • Leak memory\n";
    cout << "  • Leave files open\n";
    cout << "  • Deadlock on exceptions\n";
    cout << "\n";
    cout << "This is why C++ doesn't need\n";
    cout << "garbage collection!\n";

    return 0;
}
