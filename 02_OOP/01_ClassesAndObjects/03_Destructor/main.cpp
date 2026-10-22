#include <print>
#include <string>
#include <fstream>
#include <vector>
#include <cstdio>      // FILE, fopen, fclose, fprintf
#include <ctime>       // clock, CLOCKS_PER_SEC
#include <stdexcept>   // runtime_error
using namespace std;

// ============================================
// WHAT IS A DESTRUCTOR?
// ============================================

/*
 * A DESTRUCTOR is a special member function that:
 * - Has the SAME NAME as the class with ~ prefix
 * - NO RETURN TYPE (not even void)
 * - NO PARAMETERS
 * - Automatically called when object is destroyed
 * - Used to CLEAN UP resources
 *
 * Purpose: Free memory, close files, release resources
 */

// ============================================
// EXAMPLE 1: BASIC DESTRUCTOR
// ============================================

class SimpleClass {
public:
    string name;

    // Constructor
    SimpleClass(string n) : name(n) {
        std::println("🏗️  Constructor: Creating {}", name);
    }

    // Destructor
    ~SimpleClass() {
        std::println("💥 Destructor: Destroying {}", name);
    }
};

void demonstrateBasicDestructor() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: BASIC DESTRUCTOR       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating object in scope ---");
    {
        SimpleClass obj("Object1");
        std::println("   Object is alive and usable");
    }  // ← Destructor called here when scope ends!

    std::println("--- After scope ended ---");
    std::println("✓ Destructor automatically called!");
}

// ============================================
// EXAMPLE 2: WHEN DESTRUCTORS ARE CALLED
// ============================================

class LifetimeDemo {
public:
    string name;

    LifetimeDemo(string n) : name(n) {
        std::println("   ➕ Created: {}", name);
    }

    ~LifetimeDemo() {
        std::println("   ➖ Destroyed: {}", name);
    }
};

void innerFunction() {
    std::println("  [Inside innerFunction]");
    LifetimeDemo local("LocalInFunction");
    std::println("  [Exiting innerFunction]");
}

void demonstrateWhenDestructorsCalled() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: WHEN CALLED            ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Case 1: Scope ends ---");
    {
        LifetimeDemo obj1("ScopeObject");
    }  // Destructor called here

    std::println("");
    std::println("--- Case 2: Function returns ---");
    innerFunction();  // obj destroyed when function returns

    std::println("");
    std::println("--- Case 3: Return statement ---");
    {
        LifetimeDemo obj2("EarlyReturn");
        if (true) {
            std::println("  Returning early...");
            // Destructor called before return
            return;  // Destructor called here!
        }
        std::println("  This won't print");
    }
}

// ============================================
// EXAMPLE 3: RESOURCE CLEANUP - MEMORY
// ============================================

class DynamicArray {
private:
    int* data;
    size_t size;

public:
    DynamicArray(size_t s) : size(s) {
        data = new int[size];
        std::println("🏗️  Allocated {} integers", size);

        // Initialize with values
        for (size_t i = 0; i < size; i++) {
            data[i] = i * 10;
        }
    }

    // Destructor - FREE THE MEMORY!
    ~DynamicArray() {
        std::println("💥 Freeing {} integers", size);
        delete[] data;  // ← Critical! Must free memory
    }

    void display() {
        std::print("Array contents: ");
        for (size_t i = 0; i < size && i < 5; i++) {
            std::print("{} ", data[i]);
        }
        if (size > 5) std::print("...");
        std::println("");
    }
};

void demonstrateMemoryCleanup() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: MEMORY CLEANUP         ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating array ---");
    {
        DynamicArray arr(100);
        arr.display();
        std::println("--- Using array... ---");
    }  // ← Destructor automatically frees memory!

    std::println("");
    std::println("✅ Memory automatically freed!");
}

// ============================================
// EXAMPLE 4: RESOURCE CLEANUP - FILE HANDLE
// ============================================

class FileManager {
private:
    FILE* file;
    string filename;

public:
    FileManager(const string& fname) : filename(fname) {
        file = fopen(fname.c_str(), "w");
        if (file) {
            std::println("📂 Opened file: {}", filename);
        } else {
            std::println("❌ Failed to open file");
        }
    }

    // Destructor - CLOSE THE FILE!
    ~FileManager() {
        if (file) {
            fclose(file);
            std::println("📁 Closed file: {}", filename);
        }
    }

    void write(const string& text) {
        if (file) {
            fprintf(file, "%s\n", text.c_str());
            std::println("✍️  Wrote: {}", text);
        }
    }
};

void demonstrateFileCleanup() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: FILE CLEANUP           ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Using file manager ---");
    {
        FileManager file("test_output.txt");
        file.write("Line 1: Hello from destructor example!");
        file.write("Line 2: File will be closed automatically");
        std::println("--- Doing other work... ---");
    }  // ← File automatically closed by destructor!

    std::println("");
    std::println("✅ File automatically closed!");
}

// ============================================
// EXAMPLE 5: DESTRUCTOR ORDER
// ============================================

class OrderDemo {
public:
    string name;

    OrderDemo(string n) : name(n) {
        std::println("  ➕ Constructed: {}", name);
    }

    ~OrderDemo() {
        std::println("  ➖ Destructed: {}", name);
    }
};

void demonstrateDestructorOrder() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 5: DESTRUCTOR ORDER       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating objects ---");
    {
        OrderDemo obj1("First");
        OrderDemo obj2("Second");
        OrderDemo obj3("Third");

        std::println("");
        std::println("--- All objects created ---");
        std::println("--- Scope ending... ---");
        std::println("");
    }  // Destructors called in REVERSE order!
}

// ============================================
// EXAMPLE 6: COMPOSITION - NESTED OBJECTS
// ============================================

class Engine {
public:
    Engine() {
        std::println("    🔧 Engine constructed");
    }

    ~Engine() {
        std::println("    🔧 Engine destructed");
    }
};

class Wheels {
public:
    Wheels() {
        std::println("    🛞 Wheels constructed");
    }

    ~Wheels() {
        std::println("    🛞 Wheels destructed");
    }
};

class CarComposite {
private:
    Engine engine;
    Wheels wheels;

public:
    CarComposite() {
        std::println("  🚗 Car constructed");
    }

    ~CarComposite() {
        std::println("  🚗 Car destructed");
    }
};

void demonstrateComposition() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 6: COMPOSITION            ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating car ---");
    {
        CarComposite car;
    }
}

// ============================================
// EXAMPLE 7: EXCEPTION SAFETY
// ============================================

class ExceptionSafeResource {
private:
    int* data;

public:
    ExceptionSafeResource() {
        data = new int[1000];
        std::println("🏗️  Resource allocated");
    }

    ~ExceptionSafeResource() {
        delete[] data;
        std::println("💥 Resource freed (even with exception!)");
    }

    void riskyOperation() {
        std::println("⚠️  Performing risky operation...");
        throw runtime_error("Something went wrong!");
    }
};

void demonstrateExceptionSafety() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 7: EXCEPTION SAFETY       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Testing exception safety ---");
    try {
        ExceptionSafeResource resource;
        resource.riskyOperation();
    } catch (const exception& e) {
        std::println("🚨 Caught exception: {}", e.what());
    }

    std::println("");
    std::println("✅ Destructor called even when exception thrown!");
}

// ============================================
// EXAMPLE 8: ARRAY OF OBJECTS
// ============================================

class ArrayElement {
private:
    int id;

public:
    ArrayElement() : id(0) {
        std::println("  ➕ Element constructed");
    }

    ArrayElement(int i) : id(i) {
        std::println("  ➕ Element {} constructed", id);
    }

    ~ArrayElement() {
        std::println("  ➖ Element {} destructed", id);
    }

    void setId(int i) { id = i; }
};

void demonstrateArrayDestruction() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 8: ARRAY DESTRUCTION      ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating array of 3 objects ---");
    {
        ArrayElement arr[3];
        arr[0].setId(1);
        arr[1].setId(2);
        arr[2].setId(3);

        std::println("");
        std::println("--- Array in use ---");
    }  // Destructors called for all 3 objects!

    std::println("");
    std::println("✅ All array elements automatically destroyed!");
}

// ============================================
// EXAMPLE 9: HEAP vs STACK DESTRUCTION
// ============================================

class HeapStackDemo {
public:
    string location;

    HeapStackDemo(string loc) : location(loc) {
        std::println("  ➕ Created on {}", location);
    }

    ~HeapStackDemo() {
        std::println("  ➖ Destroyed from {}", location);
    }
};

void demonstrateHeapVsStack() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 9: HEAP vs STACK          ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Stack object (automatic) ---");
    {
        HeapStackDemo stackObj("STACK");
    }  // Destructor called automatically

    std::println("");
    std::println("--- Heap object (manual) ---");
    HeapStackDemo* heapObj = new HeapStackDemo("HEAP");
    std::println("  Object created, still exists...");
    delete heapObj;  // Must manually delete - calls destructor

    std::println("");
    std::println("--- What if we forget delete? ---");
    HeapStackDemo* forgotten = new HeapStackDemo("HEAP-FORGOTTEN");
    // delete forgotten;  // ❌ Forgot to delete!
    std::println("  ⚠️  MEMORY LEAK! Destructor never called!");
}

// ============================================
// EXAMPLE 10: VIRTUAL DESTRUCTOR
// ============================================

class Base {
public:
    Base() {
        std::println("  🏗️  Base constructor");
    }

    // Virtual destructor (IMPORTANT for polymorphism!)
    virtual ~Base() {
        std::println("  💥 Base destructor");
    }
};

class Derived : public Base {
private:
    int* data;

public:
    Derived() {
        data = new int[100];
        std::println("  🏗️  Derived constructor (allocated memory)");
    }

    ~Derived() {
        delete[] data;
        std::println("  💥 Derived destructor (freed memory)");
    }
};

void demonstrateVirtualDestructor() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 10: VIRTUAL DESTRUCTOR    ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Without virtual destructor would be BAD ---");
    std::println("--- With virtual destructor (GOOD) ---");
    std::println("");

    std::println("Creating derived object via base pointer:");
    {
        Base* ptr = new Derived();
        std::println("");
        std::println("  Deleting via base pointer...");
        std::println("");
        delete ptr;  // Calls Derived destructor then Base destructor!
    }

    std::println("");
    std::println("✅ Both destructors called!");
}

// ============================================
// EXAMPLE 11: REAL-WORLD - DATABASE CONNECTION
// ============================================

class DatabaseConnection {
private:
    string connectionString;
    bool connected;

public:
    DatabaseConnection(const string& connStr) : connectionString(connStr), connected(false) {
        std::println("🔌 Connecting to database...");
        std::println("   Connection string: {}", connectionString);
        connected = true;
        std::println("✅ Connected!");
    }

    ~DatabaseConnection() {
        if (connected) {
            std::println("🔌 Disconnecting from database...");
            connected = false;
            std::println("✅ Disconnected safely!");
        }
    }

    void executeQuery(const string& query) {
        if (connected) {
            std::println("📊 Executing: {}", query);
        }
    }
};

void demonstrateDatabaseConnection() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 11: DATABASE CONNECTION   ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Using database ---");
    {
        DatabaseConnection db("server=localhost;database=mydb");
        db.executeQuery("SELECT * FROM users");
        db.executeQuery("UPDATE users SET status='active'");
        std::println("--- Done with database ---");
    }  // Connection automatically closed!

    std::println("");
    std::println("✅ Connection automatically closed!");
}

// ============================================
// EXAMPLE 12: RAII PATTERN
// ============================================

class Timer {
private:
    string taskName;
    clock_t startTime;

public:
    Timer(const string& task) : taskName(task) {
        startTime = clock();
        std::println("⏱️  Started: {}", taskName);
    }

    ~Timer() {
        clock_t endTime = clock();
        double elapsed = double(endTime - startTime) / CLOCKS_PER_SEC * 1000;
        std::println("⏱️  Finished: {} ({} ms)", taskName, elapsed);
    }
};

void expensiveOperation() {
    int sum = 0;
    for (int i = 0; i < 10000000; i++) {
        sum += i;
    }
}

void demonstrateRAII() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 12: RAII PATTERN          ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Automatic timing with RAII ---");
    {
        Timer timer("Expensive Operation");
        expensiveOperation();
    }  // Timer automatically reports elapsed time!

    std::println("");
    std::println("✅ Destructor automatically reports timing!");
}

// ============================================
// MAIN DEMONSTRATION
// ============================================

int main() {
    std::println("========================================");
    std::println("DESTRUCTOR DEMONSTRATION");
    std::println("========================================");

    demonstrateBasicDestructor();
    demonstrateWhenDestructorsCalled();
    demonstrateMemoryCleanup();
    demonstrateFileCleanup();
    demonstrateDestructorOrder();
    demonstrateComposition();
    demonstrateExceptionSafety();
    demonstrateArrayDestruction();
    demonstrateHeapVsStack();
    demonstrateVirtualDestructor();
    demonstrateDatabaseConnection();
    demonstrateRAII();

    return 0;
}
