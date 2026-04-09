#include <iostream>
#include <string>
#include <fstream>
#include <vector>
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
        cout << "🏗️  Constructor: Creating " << name << "\n";
    }

    // Destructor
    ~SimpleClass() {
        cout << "💥 Destructor: Destroying " << name << "\n";
    }
};

void demonstrateBasicDestructor() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: BASIC DESTRUCTOR       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating object in scope ---\n";
    {
        SimpleClass obj("Object1");
        cout << "   Object is alive and usable\n";
    }  // ← Destructor called here when scope ends!

    cout << "--- After scope ended ---\n";
    cout << "✓ Destructor automatically called!\n";
}

// ============================================
// EXAMPLE 2: WHEN DESTRUCTORS ARE CALLED
// ============================================

class LifetimeDemo {
public:
    string name;

    LifetimeDemo(string n) : name(n) {
        cout << "   ➕ Created: " << name << "\n";
    }

    ~LifetimeDemo() {
        cout << "   ➖ Destroyed: " << name << "\n";
    }
};

void innerFunction() {
    cout << "  [Inside innerFunction]\n";
    LifetimeDemo local("LocalInFunction");
    cout << "  [Exiting innerFunction]\n";
}

void demonstrateWhenDestructorsCalled() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: WHEN CALLED            ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Case 1: Scope ends ---\n";
    {
        LifetimeDemo obj1("ScopeObject");
    }  // Destructor called here

    cout << "\n--- Case 2: Function returns ---\n";
    innerFunction();  // obj destroyed when function returns

    cout << "\n--- Case 3: Return statement ---\n";
    {
        LifetimeDemo obj2("EarlyReturn");
        if (true) {
            cout << "  Returning early...\n";
            // Destructor called before return
            return;  // Destructor called here!
        }
        cout << "  This won't print\n";
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
        cout << "🏗️  Allocated " << size << " integers\n";

        // Initialize with values
        for (size_t i = 0; i < size; i++) {
            data[i] = i * 10;
        }
    }

    // Destructor - FREE THE MEMORY!
    ~DynamicArray() {
        cout << "💥 Freeing " << size << " integers\n";
        delete[] data;  // ← Critical! Must free memory
    }

    void display() {
        cout << "Array contents: ";
        for (size_t i = 0; i < size && i < 5; i++) {
            cout << data[i] << " ";
        }
        if (size > 5) cout << "...";
        cout << "\n";
    }
};

void demonstrateMemoryCleanup() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: MEMORY CLEANUP         ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating array ---\n";
    {
        DynamicArray arr(100);
        arr.display();
        cout << "--- Using array... ---\n";
    }  // ← Destructor automatically frees memory!

    cout << "\n✅ Memory automatically freed!\n";
    cout << "   Without destructor → Memory leak! 💔\n";
    cout << "   With destructor → Safe and automatic! ✨\n";
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
            cout << "📂 Opened file: " << filename << "\n";
        } else {
            cout << "❌ Failed to open file\n";
        }
    }

    // Destructor - CLOSE THE FILE!
    ~FileManager() {
        if (file) {
            fclose(file);
            cout << "📁 Closed file: " << filename << "\n";
        }
    }

    void write(const string& text) {
        if (file) {
            fprintf(file, "%s\n", text.c_str());
            cout << "✍️  Wrote: " << text << "\n";
        }
    }
};

void demonstrateFileCleanup() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: FILE CLEANUP           ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Using file manager ---\n";
    {
        FileManager file("test_output.txt");
        file.write("Line 1: Hello from destructor example!");
        file.write("Line 2: File will be closed automatically");
        cout << "--- Doing other work... ---\n";
    }  // ← File automatically closed by destructor!

    cout << "\n✅ File automatically closed!\n";
    cout << "   Even if exception thrown → file still closed!\n";
}

// ============================================
// EXAMPLE 5: DESTRUCTOR ORDER
// ============================================

class OrderDemo {
public:
    string name;

    OrderDemo(string n) : name(n) {
        cout << "  ➕ Constructed: " << name << "\n";
    }

    ~OrderDemo() {
        cout << "  ➖ Destructed: " << name << "\n";
    }
};

void demonstrateDestructorOrder() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: DESTRUCTOR ORDER       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating objects ---\n";
    {
        OrderDemo obj1("First");
        OrderDemo obj2("Second");
        OrderDemo obj3("Third");

        cout << "\n--- All objects created ---\n";
        cout << "--- Scope ending... ---\n\n";
    }  // Destructors called in REVERSE order!

    cout << "\n💡 Destructors called in REVERSE order of construction!\n";
    cout << "   Construction: First → Second → Third\n";
    cout << "   Destruction:  Third → Second → First\n";
}

// ============================================
// EXAMPLE 6: COMPOSITION - NESTED OBJECTS
// ============================================

class Engine {
public:
    Engine() {
        cout << "    🔧 Engine constructed\n";
    }

    ~Engine() {
        cout << "    🔧 Engine destructed\n";
    }
};

class Wheels {
public:
    Wheels() {
        cout << "    🛞 Wheels constructed\n";
    }

    ~Wheels() {
        cout << "    🛞 Wheels destructed\n";
    }
};

class CarComposite {
private:
    Engine engine;
    Wheels wheels;

public:
    CarComposite() {
        cout << "  🚗 Car constructed\n";
    }

    ~CarComposite() {
        cout << "  🚗 Car destructed\n";
    }
};

void demonstrateComposition() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: COMPOSITION            ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating car ---\n";
    {
        CarComposite car;
    }

    cout << "\n💡 Member destructors called automatically!\n";
    cout << "   Order: Car → Wheels → Engine (reverse of construction)\n";
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
        cout << "🏗️  Resource allocated\n";
    }

    ~ExceptionSafeResource() {
        delete[] data;
        cout << "💥 Resource freed (even with exception!)\n";
    }

    void riskyOperation() {
        cout << "⚠️  Performing risky operation...\n";
        throw runtime_error("Something went wrong!");
    }
};

void demonstrateExceptionSafety() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 7: EXCEPTION SAFETY       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Testing exception safety ---\n";
    try {
        ExceptionSafeResource resource;
        resource.riskyOperation();
    } catch (const exception& e) {
        cout << "🚨 Caught exception: " << e.what() << "\n";
    }

    cout << "\n✅ Destructor called even when exception thrown!\n";
    cout << "   This is RAII (Resource Acquisition Is Initialization)\n";
    cout << "   Resources are ALWAYS cleaned up!\n";
}

// ============================================
// EXAMPLE 8: ARRAY OF OBJECTS
// ============================================

class ArrayElement {
private:
    int id;

public:
    ArrayElement() : id(0) {
        cout << "  ➕ Element constructed\n";
    }

    ArrayElement(int i) : id(i) {
        cout << "  ➕ Element " << id << " constructed\n";
    }

    ~ArrayElement() {
        cout << "  ➖ Element " << id << " destructed\n";
    }

    void setId(int i) { id = i; }
};

void demonstrateArrayDestruction() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 8: ARRAY DESTRUCTION      ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating array of 3 objects ---\n";
    {
        ArrayElement arr[3];
        arr[0].setId(1);
        arr[1].setId(2);
        arr[2].setId(3);

        cout << "\n--- Array in use ---\n";
    }  // Destructors called for all 3 objects!

    cout << "\n✅ All array elements automatically destroyed!\n";
}

// ============================================
// EXAMPLE 9: HEAP vs STACK DESTRUCTION
// ============================================

class HeapStackDemo {
public:
    string location;

    HeapStackDemo(string loc) : location(loc) {
        cout << "  ➕ Created on " << location << "\n";
    }

    ~HeapStackDemo() {
        cout << "  ➖ Destroyed from " << location << "\n";
    }
};

void demonstrateHeapVsStack() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 9: HEAP vs STACK          ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Stack object (automatic) ---\n";
    {
        HeapStackDemo stackObj("STACK");
    }  // Destructor called automatically

    cout << "\n--- Heap object (manual) ---\n";
    HeapStackDemo* heapObj = new HeapStackDemo("HEAP");
    cout << "  Object created, still exists...\n";
    delete heapObj;  // Must manually delete - calls destructor

    cout << "\n--- What if we forget delete? ---\n";
    HeapStackDemo* forgotten = new HeapStackDemo("HEAP-FORGOTTEN");
    // delete forgotten;  // ❌ Forgot to delete!
    cout << "  ⚠️  MEMORY LEAK! Destructor never called!\n";

    cout << "\n💡 Stack objects: Automatic cleanup\n";
    cout << "💡 Heap objects: Must manually delete\n";
    cout << "💡 Best practice: Use smart pointers (unique_ptr, shared_ptr)\n";
}

// ============================================
// EXAMPLE 10: VIRTUAL DESTRUCTOR
// ============================================

class Base {
public:
    Base() {
        cout << "  🏗️  Base constructor\n";
    }

    // Virtual destructor (IMPORTANT for polymorphism!)
    virtual ~Base() {
        cout << "  💥 Base destructor\n";
    }
};

class Derived : public Base {
private:
    int* data;

public:
    Derived() {
        data = new int[100];
        cout << "  🏗️  Derived constructor (allocated memory)\n";
    }

    ~Derived() {
        delete[] data;
        cout << "  💥 Derived destructor (freed memory)\n";
    }
};

void demonstrateVirtualDestructor() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 10: VIRTUAL DESTRUCTOR    ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Without virtual destructor would be BAD ---\n";
    cout << "--- With virtual destructor (GOOD) ---\n\n";

    cout << "Creating derived object via base pointer:\n";
    {
        Base* ptr = new Derived();
        cout << "\n  Deleting via base pointer...\n\n";
        delete ptr;  // Calls Derived destructor then Base destructor!
    }

    cout << "\n✅ Both destructors called!\n";
    cout << "💡 Without 'virtual' → Only Base destructor called → MEMORY LEAK!\n";
    cout << "💡 With 'virtual' → Both called in correct order → SAFE!\n";

    cout << "\n⚠️  RULE: Always make destructor virtual in base classes!\n";
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
        cout << "🔌 Connecting to database...\n";
        cout << "   Connection string: " << connectionString << "\n";
        connected = true;
        cout << "✅ Connected!\n";
    }

    ~DatabaseConnection() {
        if (connected) {
            cout << "🔌 Disconnecting from database...\n";
            connected = false;
            cout << "✅ Disconnected safely!\n";
        }
    }

    void executeQuery(const string& query) {
        if (connected) {
            cout << "📊 Executing: " << query << "\n";
        }
    }
};

void demonstrateDatabaseConnection() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 11: DATABASE CONNECTION   ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Using database ---\n";
    {
        DatabaseConnection db("server=localhost;database=mydb");
        db.executeQuery("SELECT * FROM users");
        db.executeQuery("UPDATE users SET status='active'");
        cout << "--- Done with database ---\n";
    }  // Connection automatically closed!

    cout << "\n✅ Connection automatically closed!\n";
    cout << "   No need to remember to disconnect!\n";
    cout << "   Even if exception thrown → still disconnected!\n";
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
        cout << "⏱️  Started: " << taskName << "\n";
    }

    ~Timer() {
        clock_t endTime = clock();
        double elapsed = double(endTime - startTime) / CLOCKS_PER_SEC * 1000;
        cout << "⏱️  Finished: " << taskName << " (" << elapsed << " ms)\n";
    }
};

void expensiveOperation() {
    int sum = 0;
    for (int i = 0; i < 10000000; i++) {
        sum += i;
    }
}

void demonstrateRAII() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 12: RAII PATTERN          ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Automatic timing with RAII ---\n";
    {
        Timer timer("Expensive Operation");
        expensiveOperation();
    }  // Timer automatically reports elapsed time!

    cout << "\n✅ Destructor automatically reports timing!\n";
    cout << "   No need to manually stop timer!\n";
    cout << "   This is RAII in action!\n";
}

// ============================================
// MAIN DEMONSTRATION
// ============================================

int main() {
    cout << "========================================\n";
    cout << "DESTRUCTOR DEMONSTRATION\n";
    cout << "========================================\n";

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

    // ============================================
    // Comprehensive Summary
    // ============================================
    cout << "\n========================================\n";
    cout << "WHAT IS A DESTRUCTOR?\n";
    cout << "========================================\n";
    cout << "A destructor is a special member function that:\n";
    cout << "  • Has the SAME NAME as class with ~ prefix\n";
    cout << "  • Has NO RETURN TYPE (not even void)\n";
    cout << "  • Has NO PARAMETERS (cannot be overloaded)\n";
    cout << "  • Automatically called when object is destroyed\n";
    cout << "  • Used to CLEAN UP resources\n";
    cout << "\n";
    cout << "Purpose:\n";
    cout << "  • Free allocated memory (delete)\n";
    cout << "  • Close files and connections\n";
    cout << "  • Release system resources\n";
    cout << "  • Ensure proper cleanup\n";

    cout << "\n========================================\n";
    cout << "DESTRUCTOR SYNTAX\n";
    cout << "========================================\n";
    cout << "class MyClass {\n";
    cout << "public:\n";
    cout << "    // Constructor\n";
    cout << "    MyClass() {\n";
    cout << "        // Initialize\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    // Destructor\n";
    cout << "    ~MyClass() {              // ~ prefix!\n";
    cout << "        // Cleanup            // No parameters!\n";
    cout << "    }                         // No return type!\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "WHEN DESTRUCTORS ARE CALLED\n";
    cout << "========================================\n";
    cout << "1. Scope ends:\n";
    cout << "   { MyClass obj; }  // Destructor called here\n";
    cout << "\n";
    cout << "2. Function returns:\n";
    cout << "   void func() {\n";
    cout << "       MyClass obj;\n";
    cout << "   }  // Destructor called\n";
    cout << "\n";
    cout << "3. delete called:\n";
    cout << "   MyClass* ptr = new MyClass();\n";
    cout << "   delete ptr;  // Destructor called\n";
    cout << "\n";
    cout << "4. Program exits:\n";
    cout << "   // All remaining objects destroyed\n";
    cout << "\n";
    cout << "5. Exception thrown:\n";
    cout << "   try {\n";
    cout << "       MyClass obj;\n";
    cout << "       throw exception();\n";
    cout << "   }  // Destructor called before exception propagates\n";

    cout << "\n========================================\n";
    cout << "DESTRUCTION ORDER\n";
    cout << "========================================\n";
    cout << "Objects are destroyed in REVERSE order of construction:\n";
    cout << "\n";
    cout << "MyClass obj1;  // Created first\n";
    cout << "MyClass obj2;  // Created second\n";
    cout << "MyClass obj3;  // Created third\n";
    cout << "\n";
    cout << "// Destroyed: obj3, obj2, obj1 (reverse!)\n";
    cout << "\n";
    cout << "This ensures:\n";
    cout << "  • Dependencies are handled correctly\n";
    cout << "  • Last created, first destroyed\n";
    cout << "  • LIFO (Last In, First Out) order\n";

    cout << "\n========================================\n";
    cout << "MEMBER DESTRUCTION ORDER\n";
    cout << "========================================\n";
    cout << "For objects with member objects:\n";
    cout << "\n";
    cout << "class Car {\n";
    cout << "    Engine engine;\n";
    cout << "    Wheels wheels;\n";
    cout << "};\n";
    cout << "\n";
    cout << "Destruction order:\n";
    cout << "  1. Car destructor body executes\n";
    cout << "  2. wheels destructor called\n";
    cout << "  3. engine destructor called\n";
    cout << "  (reverse order of declaration)\n";

    cout << "\n========================================\n";
    cout << "STACK vs HEAP DESTRUCTION\n";
    cout << "========================================\n";
    cout << "Stack objects (automatic):\n";
    cout << "  MyClass obj;              // Stack\n";
    cout << "  // Destructor called automatically when scope ends\n";
    cout << "  ✅ Automatic cleanup\n";
    cout << "  ✅ Cannot forget\n";
    cout << "  ✅ Exception-safe\n";
    cout << "\n";
    cout << "Heap objects (manual):\n";
    cout << "  MyClass* ptr = new MyClass();  // Heap\n";
    cout << "  delete ptr;  // MUST manually delete!\n";
    cout << "  ❌ Can forget to delete\n";
    cout << "  ❌ Causes memory leaks\n";
    cout << "  ⚠️  Use smart pointers instead!\n";

    cout << "\n========================================\n";
    cout << "VIRTUAL DESTRUCTORS\n";
    cout << "========================================\n";
    cout << "When using inheritance and polymorphism:\n";
    cout << "\n";
    cout << "class Base {\n";
    cout << "    virtual ~Base() { }  // ← virtual!\n";
    cout << "};\n";
    cout << "\n";
    cout << "class Derived : public Base {\n";
    cout << "    ~Derived() { }\n";
    cout << "};\n";
    cout << "\n";
    cout << "Base* ptr = new Derived();\n";
    cout << "delete ptr;  // Calls Derived, then Base destructor\n";
    cout << "\n";
    cout << "⚠️  CRITICAL RULE:\n";
    cout << "   Always make destructor virtual in base classes!\n";
    cout << "   Without virtual → Only base destructor called → LEAK!\n";
    cout << "   With virtual → Both destructors called → SAFE!\n";

    cout << "\n========================================\n";
    cout << "COMMON USES OF DESTRUCTORS\n";
    cout << "========================================\n";
    cout << "1. Free dynamic memory:\n";
    cout << "   ~MyClass() { delete[] data; }\n";
    cout << "\n";
    cout << "2. Close files:\n";
    cout << "   ~FileManager() { fclose(file); }\n";
    cout << "\n";
    cout << "3. Release locks:\n";
    cout << "   ~LockGuard() { mutex.unlock(); }\n";
    cout << "\n";
    cout << "4. Close connections:\n";
    cout << "   ~Database() { connection.close(); }\n";
    cout << "\n";
    cout << "5. Log/timing:\n";
    cout << "   ~Timer() { reportElapsedTime(); }\n";

    cout << "\n========================================\n";
    cout << "RAII (Resource Acquisition Is Initialization)\n";
    cout << "========================================\n";
    cout << "RAII Pattern:\n";
    cout << "  • Constructor acquires resource\n";
    cout << "  • Destructor releases resource\n";
    cout << "  • Automatic, exception-safe cleanup\n";
    cout << "\n";
    cout << "Example:\n";
    cout << "  class FileHandle {\n";
    cout << "      FileHandle(name) { file = open(name); }  // Acquire\n";
    cout << "      ~FileHandle() { close(file); }           // Release\n";
    cout << "  };\n";
    cout << "\n";
    cout << "Benefits:\n";
    cout << "  ✅ Cannot forget cleanup\n";
    cout << "  ✅ Exception-safe\n";
    cout << "  ✅ Simple to use\n";
    cout << "  ✅ No manual resource management\n";

    cout << "\n========================================\n";
    cout << "EXCEPTION SAFETY\n";
    cout << "========================================\n";
    cout << "Destructors are called even when exceptions thrown:\n";
    cout << "\n";
    cout << "try {\n";
    cout << "    MyClass obj;  // Constructor\n";
    cout << "    throw exception();\n";
    cout << "}  // obj's destructor still called!\n";
    cout << "\n";
    cout << "This guarantees:\n";
    cout << "  ✅ Resources always cleaned up\n";
    cout << "  ✅ No leaks even with exceptions\n";
    cout << "  ✅ Code is exception-safe by default\n";
    cout << "\n";
    cout << "⚠️  IMPORTANT:\n";
    cout << "   Never throw exceptions from destructors!\n";
    cout << "   Can cause program termination!\n";

    cout << "\n========================================\n";
    cout << "DESTRUCTOR BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "✅ DO:\n";
    cout << "  • Free all allocated resources\n";
    cout << "  • Make destructor noexcept (implicit)\n";
    cout << "  • Make virtual in base classes\n";
    cout << "  • Keep simple and focused\n";
    cout << "  • Use RAII pattern\n";
    cout << "  • Handle null pointers safely\n";
    cout << "\n";
    cout << "❌ DON'T:\n";
    cout << "  • Throw exceptions from destructor\n";
    cout << "  • Call virtual functions\n";
    cout << "  • Do complex operations\n";
    cout << "  • Access other objects (might be destroyed)\n";
    cout << "  • Forget virtual in base classes\n";

    cout << "\n========================================\n";
    cout << "COMMON MISTAKES\n";
    cout << "========================================\n";
    cout << "❌ Mistake 1: Memory leak\n";
    cout << "   ~MyClass() {  // Forgot to delete!\n";
    cout << "   }\n";
    cout << "\n";
    cout << "❌ Mistake 2: Non-virtual in base class\n";
    cout << "   class Base {\n";
    cout << "       ~Base() { }  // Should be virtual!\n";
    cout << "   };\n";
    cout << "\n";
    cout << "❌ Mistake 3: Throwing exception\n";
    cout << "   ~MyClass() {\n";
    cout << "       throw exception();  // DON'T!\n";
    cout << "   }\n";
    cout << "\n";
    cout << "❌ Mistake 4: Forgetting to delete heap object\n";
    cout << "   MyClass* ptr = new MyClass();\n";
    cout << "   // Forgot: delete ptr;\n";

    cout << "\n========================================\n";
    cout << "SMART POINTERS - MODERN SOLUTION\n";
    cout << "========================================\n";
    cout << "Instead of manual new/delete:\n";
    cout << "\n";
    cout << "Old way (manual):\n";
    cout << "  MyClass* ptr = new MyClass();\n";
    cout << "  delete ptr;  // Must remember!\n";
    cout << "\n";
    cout << "Modern way (automatic):\n";
    cout << "  unique_ptr<MyClass> ptr = make_unique<MyClass>();\n";
    cout << "  // Automatically deleted!\n";
    cout << "\n";
    cout << "Benefits:\n";
    cout << "  ✅ Cannot forget to delete\n";
    cout << "  ✅ Exception-safe\n";
    cout << "  ✅ RAII built-in\n";
    cout << "  ✅ Modern C++ style\n";

    cout << "\n========================================\n";
    cout << "CONSTRUCTOR vs DESTRUCTOR\n";
    cout << "========================================\n";
    cout << "┌──────────────┬────────────────┬──────────────┐\n";
    cout << "│ Feature      │ Constructor    │ Destructor   │\n";
    cout << "├──────────────┼────────────────┼──────────────┤\n";
    cout << "│ Name         │ ClassName      │ ~ClassName   │\n";
    cout << "│ Parameters   │ Yes (any)      │ No (none)    │\n";
    cout << "│ Overloading  │ Yes (multiple) │ No (one only)│\n";
    cout << "│ Called       │ Object created │ Object dies  │\n";
    cout << "│ Purpose      │ Initialize     │ Cleanup      │\n";
    cout << "│ Can throw    │ Yes            │ No (danger!) │\n";
    cout << "└──────────────┴────────────────┴──────────────┘\n";

    cout << "\n========================================\n";
    cout << "QUICK REFERENCE\n";
    cout << "========================================\n";
    cout << "// Basic Destructor\n";
    cout << "class MyClass {\n";
    cout << "public:\n";
    cout << "    ~MyClass() {              // ~ prefix\n";
    cout << "        // Cleanup code       // No return type\n";
    cout << "    }                         // No parameters\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Virtual Destructor (for inheritance)\n";
    cout << "class Base {\n";
    cout << "public:\n";
    cout << "    virtual ~Base() {         // virtual keyword\n";
    cout << "        // Cleanup\n";
    cout << "    }\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "KEY TAKEAWAYS\n";
    cout << "========================================\n";
    cout << "1. Destructor = Automatic cleanup when object destroyed\n";
    cout << "2. ~ prefix, no return type, no parameters\n";
    cout << "3. Called automatically (scope, delete, exception)\n";
    cout << "4. Destruction order = Reverse of construction\n";
    cout << "5. Always make virtual in base classes\n";
    cout << "6. Never throw exceptions from destructor\n";
    cout << "7. RAII pattern = Constructor acquires, destructor releases\n";
    cout << "8. Stack objects: automatic, Heap objects: manual\n";

    return 0;
}
