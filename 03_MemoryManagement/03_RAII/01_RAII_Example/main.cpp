#include <print>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <mutex>
#include <chrono>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <new>
#include <stdexcept>

// ============================================
// RAII: Resource Acquisition Is Initialization
// ============================================

// ============================================
// EXAMPLE 1: FILE HANDLE RAII
// ============================================

class FileHandle {
private:
    std::FILE* file;
    std::string filename;

public:
    // Constructor: ACQUIRE resource
    FileHandle(const std::string& fname, const char* mode)
        : file(nullptr), filename(fname) {
        std::println("📂 Opening file: {}", filename);
        file = std::fopen(fname.c_str(), mode);
        if (!file) {
            throw std::runtime_error("Failed to open file: " + filename);
        }
        std::println("✅ File opened successfully");
    }

    // Destructor: RELEASE resource (automatically called!)
    ~FileHandle() {
        if (file) {
            std::println("📁 Closing file: {}", filename);
            std::fclose(file);
            file = nullptr;
        }
    }

    // Delete copy operations (file handles shouldn't be copied)
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;

    // Write to file
    void write(const std::string& data) {
        if (file) {
            std::fprintf(file, "%s", data.c_str());
            std::print("✍️  Written: {}", data);
        }
    }

    // Read from file
    std::string read() {
        if (!file) return "";

        std::fseek(file, 0, SEEK_END);
        long size = std::ftell(file);
        if (size < 0) return "";
        std::fseek(file, 0, SEEK_SET);

        std::string content(size, '\0');
        std::fread(&content[0], 1, size, file);
        return content;
    }

    std::FILE* get() const { return file; }
};

void demonstrateFileRAII() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: FILE HANDLE RAII      ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Writing to file ---");
    try {
        FileHandle file("example.txt", "w");
        file.write("Hello, RAII!\n");
        file.write("Automatic resource management!\n");
        file.write("No manual cleanup needed!\n");

        // File automatically closed when 'file' goes out of scope
        // Even if exception is thrown!

    } catch (const std::exception& e) {
        std::println("❌ Error: {}", e.what());
    }
    std::println("✓ File is guaranteed to be closed!");

    std::println("");
    std::println("--- Reading from file ---");
    try {
        FileHandle file("example.txt", "r");
        std::string content = file.read();
        std::print("📄 Content:\n{}", content);

    } catch (const std::exception& e) {
        std::println("❌ Error: {}", e.what());
    }
}

// ============================================
// EXAMPLE 2: MUTEX LOCK GUARD
// ============================================

class MutexGuard {
private:
    std::mutex& mtx;
    std::string name;

public:
    // Constructor: ACQUIRE lock
    explicit MutexGuard(std::mutex& m, const std::string& n = "Lock") : mtx(m), name(n) {
        std::println("🔒 [{}] Acquiring lock...", name);
        mtx.lock();
        std::println("✅ [{}] Lock acquired", name);
    }

    // Destructor: RELEASE lock
    ~MutexGuard() {
        std::println("🔓 [{}] Releasing lock...", name);
        mtx.unlock();
        std::println("✅ [{}] Lock released", name);
    }

    // Delete copy operations
    MutexGuard(const MutexGuard&) = delete;
    MutexGuard& operator=(const MutexGuard&) = delete;
};

std::mutex globalMutex;
int sharedCounter = 0;

void demonstrateLockRAII() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: MUTEX LOCK GUARD       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Critical Section 1 ---");
    {
        MutexGuard guard(globalMutex, "CriticalSection1");

        sharedCounter++;
        std::println("📊 Incremented counter: {}", sharedCounter);
        std::println("💼 Doing protected work...");

        // Even if exception thrown here, lock is released!

    }  // Lock automatically released here!
    std::println("✓ Exited critical section");

    std::println("");
    std::println("--- Critical Section 2 ---");
    {
        MutexGuard guard(globalMutex, "CriticalSection2");

        sharedCounter += 5;
        std::println("📊 Incremented counter: {}", sharedCounter);
        std::println("💼 Doing more protected work...");
    }
    std::println("✓ Exited critical section");

    std::println("");
    std::println("📈 Final counter value: {}", sharedCounter);
}

// ============================================
// EXAMPLE 3: TIMER (SCOPE-BASED PROFILING)
// ============================================

class Timer {
private:
    std::string name;
    std::chrono::steady_clock::time_point start;

public:
    // Constructor: START timer
    explicit Timer(const std::string& n) : name(n) {
        start = std::chrono::steady_clock::now();
        std::println("⏱️  [{}] Timer started", name);
    }

    // Destructor: STOP timer and report
    ~Timer() {
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::println("⏱️  [{}] Elapsed time: {}ms", name, duration.count());
    }
};

void demonstrateTimerRAII() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: TIMER (PROFILING)     ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Operation 1: Fast Task ---");
    {
        Timer t("FastTask");

        // Simulate some work
        long long sum = 0;
        for (int i = 0; i < 100000; i++) {
            sum += i;
        }
        std::println("   Result: {}", sum);

    }  // Timer automatically stops and reports!

    std::println("");
    std::println("--- Operation 2: Slower Task ---");
    {
        Timer t("SlowerTask");

        // Simulate more work
        long long sum = 0;
        for (int i = 0; i < 1000000; i++) {
            sum += i;
        }
        std::println("   Result: {}", sum);
    }

    std::println("");
    std::println("--- Nested Timers ---");
    {
        Timer outer("OuterOperation");

        std::println("   Starting inner operations...");
        {
            Timer inner("InnerOperation1");
            long long x = 0;
            for (int i = 0; i < 500000; i++) x += i;
            std::println("   Result: {}", x);
        }

        {
            Timer inner("InnerOperation2");
            long long x = 0;
            for (int i = 0; i < 500000; i++) x += i;
            std::println("   Result: {}", x);
        }
    }
}

// ============================================
// EXAMPLE 4: DATABASE CONNECTION
// ============================================

class DatabaseConnection {
private:
    std::string connectionString;
    bool connected;
    int transactionCount;

public:
    // Constructor: OPEN connection
    explicit DatabaseConnection(const std::string& connStr)
        : connectionString(connStr), connected(false), transactionCount(0) {
        std::println("🗄️  Connecting to database...");
        std::println("   Connection string: {}", connectionString);

        // Simulate connection
        connected = true;
        std::println("✅ Database connected successfully!");
    }

    // Destructor: CLOSE connection
    ~DatabaseConnection() {
        if (connected) {
            if (transactionCount > 0) {
                std::println("⚠️  Rolling back {} uncommitted transactions", transactionCount);
            }
            std::println("🗄️  Closing database connection...");
            connected = false;
            std::println("✅ Database connection closed");
        }
    }

    // Delete copy (a move constructor is not implemented in this demo)
    DatabaseConnection(const DatabaseConnection&) = delete;
    DatabaseConnection& operator=(const DatabaseConnection&) = delete;

    void executeQuery(const std::string& query) {
        if (connected) {
            std::println("📝 Executing query: {}", query);
            transactionCount++;
        } else {
            std::println("❌ Cannot execute query: not connected");
        }
    }

    void commit() {
        if (connected && transactionCount > 0) {
            std::println("✅ Committing {} transactions", transactionCount);
            transactionCount = 0;
        }
    }
};

void demonstrateDatabaseRAII() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: DATABASE CONNECTION    ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Successful Transaction ---");
    {
        DatabaseConnection db("localhost:5432/mydb");
        db.executeQuery("SELECT * FROM users");
        db.executeQuery("INSERT INTO logs VALUES (1, 'test')");
        db.commit();

        // Connection automatically closed when scope ends!
    }

    std::println("");
    std::println("--- Transaction with Exception ---");
    try {
        DatabaseConnection db("localhost:5432/testdb");
        db.executeQuery("UPDATE accounts SET balance = 1000");
        db.executeQuery("DELETE FROM logs WHERE id = 5");

        // Simulate error before commit
        throw std::runtime_error("Network error!");

        db.commit();  // Never reached (commit is skipped, so the destructor reports a rollback)

    } catch (const std::exception& e) {
        std::println("❌ Exception: {}", e.what());
        // Database connection still closes properly (RAII!)
    }
    std::println("✓ Connection closed despite exception");
}

// ============================================
// EXAMPLE 5: MEMORY RESOURCE (CUSTOM ALLOCATOR)
// ============================================

class MemoryPool {
private:
    void* memory;
    std::size_t size;
    std::string name;

public:
    // Constructor: ALLOCATE memory
    MemoryPool(std::size_t s, const std::string& n)
        : size(s), name(n) {
        std::println("💾 [{}] Allocating {} bytes", name, size);
        memory = std::malloc(size);
        if (!memory) {
            throw std::bad_alloc();
        }
        std::println("✅ [{}] Memory allocated at {}", name, memory);
    }

    // Destructor: FREE memory
    ~MemoryPool() {
        if (memory) {
            std::println("💾 [{}] Freeing {} bytes at {}", name, size, memory);
            std::free(memory);
            memory = nullptr;
        }
    }

    // Delete copy operations
    MemoryPool(const MemoryPool&) = delete;
    MemoryPool& operator=(const MemoryPool&) = delete;

    void* getMemory() const { return memory; }
    std::size_t getSize() const { return size; }
};

void demonstrateMemoryPoolRAII() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 5: MEMORY POOL            ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Allocating Memory Pools ---");
    {
        MemoryPool pool1(1024, "Pool1");
        MemoryPool pool2(2048, "Pool2");

        std::println("");
        std::println("💼 Using memory pools...");
        std::println("   Pool1: {} bytes", pool1.getSize());
        std::println("   Pool2: {} bytes", pool2.getSize());

        // Memory automatically freed when scope ends!
    }
    std::println("");
    std::println("✓ All memory automatically freed!");
}

// ============================================
// EXAMPLE 6: COMPARING BAD vs GOOD
// ============================================

void badExample() {
    std::println("");
    std::println("❌ BAD EXAMPLE - Manual Management");
    std::println("==================================");

    std::FILE* file = std::fopen("manual.txt", "w");  // opened, but never closed
    if (!file) {
        std::println("Failed to open file");
        return;
    }

    std::fprintf(file, "Hello\n");

    // Simulate error
    if (true) {
        std::println("💥 Early return - FILE LEAKED!");
        return;  // 💥 FORGOT TO CLOSE FILE!
    }

    std::fclose(file);  // Never reached!
}

void goodExample() {
    std::println("");
    std::println("✅ GOOD EXAMPLE - RAII");
    std::println("======================");

    try {
        FileHandle file("raii.txt", "w");
        file.write("Hello\n");

        // Simulate error
        if (true) {
            std::println("✓ Early return - file automatically closed!");
            return;  // ✓ FILE AUTOMATICALLY CLOSED
        }

    } catch (const std::exception& e) {
        std::println("Error: {}", e.what());
    }
    // File guaranteed to be closed!
}

void demonstrateBadVsGood() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 6: BAD vs GOOD            ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    badExample();
    goodExample();
}

// ============================================
// EXAMPLE 7: STANDARD LIBRARY RAII
// ============================================

void demonstrateStandardRAII() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 7: STANDARD LIBRARY RAII  ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Smart Pointers (RAII) ---");
    {
        auto ptr = std::make_unique<int>(42);
        std::println("🔢 unique_ptr value: {}", *ptr);

        auto shared = std::make_shared<std::string>("RAII rocks!");
        std::println("📝 shared_ptr value: {}", *shared);

        // Automatically deleted when scope ends
    }
    std::println("✓ Smart pointers automatically deleted");

    std::println("");
    std::println("--- Containers (RAII) ---");
    {
        std::vector<int> numbers = {1, 2, 3, 4, 5};
        std::println("📦 Vector size: {}", numbers.size());

        std::string text = "Dynamic memory managed!";
        std::println("📝 String: {}", text);

        // Automatically freed when scope ends
    }
    std::println("✓ Containers automatically freed");

    std::println("");
    std::println("--- File Streams (RAII) ---");
    {
        std::ofstream outFile("stream_example.txt");
        outFile << "File streams use RAII!\n";
        outFile << "Automatically closed!\n";
        std::println("📄 Written to file");

        // File automatically closed when scope ends
    }
    std::println("✓ File stream automatically closed");

    std::println("");
    std::println("--- Lock Guard (RAII) ---");
    {
        std::mutex mtx;
        {
            std::lock_guard<std::mutex> lock(mtx);
            std::println("🔒 Lock acquired via lock_guard");
            // Do thread-safe work
        }
        std::println("✓ Lock automatically released");
    }
}

// ============================================
// MAIN DEMONSTRATION
// ============================================

int main() {
    std::println("========================================");
    std::println("RAII (Resource Acquisition Is Initialization)");
    std::println("========================================");

    demonstrateFileRAII();
    demonstrateLockRAII();
    demonstrateTimerRAII();
    demonstrateDatabaseRAII();
    demonstrateMemoryPoolRAII();
    demonstrateBadVsGood();
    demonstrateStandardRAII();

    return 0;
}
