#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

// ============================================
// WHAT IS unique_ptr?
// ============================================

/*
THE PROBLEM - Manual Memory Management:
    Widget* ptr = new Widget();
    // ... use ptr ...
    delete ptr;  // ❌ Forget this? MEMORY LEAK!

    // ❌ Other problems:
    // - Double deletion
    // - Exception safety
    // - Early returns
    // - Complex control flow

THE SOLUTION - unique_ptr:
    std::unique_ptr<Widget> ptr = std::make_unique<Widget>();
    // ... use ptr ...
    // ✅ Automatically deleted when ptr goes out of scope!

KEY CONCEPTS:
    1. Unique ownership       - Only ONE owner at a time
    2. Automatic deletion     - Destructor calls delete
    3. Move semantics         - Transfer ownership with std::move
    4. Zero overhead          - Same performance as raw pointers
    5. Exception safe         - Always cleans up
    6. No copying             - Copy constructor deleted
    7. RAII pattern           - Resource Acquisition Is Initialization

SYNTAX:
    std::unique_ptr<T> ptr = std::make_unique<T>(args...);  // Create
    ptr.get()                                               // Get raw pointer
    ptr.reset()                                             // Delete and set to nullptr
    ptr.release()                                           // Release ownership
    ptr.operator->()                                        // Access members
    ptr.operator*()                                         // Dereference
    std::move(ptr)                                          // Transfer ownership

GOLDEN RULE:
    Use unique_ptr by default for ALL heap allocations!
    Never use raw new/delete in modern C++!
*/

// ============================================
// EXAMPLE 1: The Problem - Raw Pointers
// ============================================

namespace RawPointerProblems {

class Widget {
public:
    Widget(int id) : id(id) {
        cout << "  Widget " << id << " created\n";
    }
    ~Widget() {
        cout << "  Widget " << id << " destroyed\n";
    }
    void doWork() {
        cout << "  Widget " << id << " working...\n";
    }
private:
    int id;
};

// ❌ Problem 1: Memory leak on early return
void problematic_function1(bool condition) {
    Widget* ptr = new Widget(1);

    if (condition) {
        return;  // ❌ MEMORY LEAK! Forgot to delete
    }

    ptr->doWork();
    delete ptr;  // Never reached if condition is true
}

// ❌ Problem 2: Memory leak on exception
void problematic_function2() {
    Widget* ptr = new Widget(2);

    // Imagine some code that might throw
    if (rand() % 2) {
        throw runtime_error("Oops!");  // ❌ MEMORY LEAK!
    }

    delete ptr;  // Never reached if exception thrown
}

// ❌ Problem 3: Double deletion
void problematic_function3() {
    Widget* ptr = new Widget(3);
    delete ptr;
    // ... complex code ...
    delete ptr;  // ❌ DOUBLE DELETE! Undefined behavior!
}

void demonstrate() {
    cout << "\n=== Example 1: Raw Pointer Problems ===\n";

    cout << "\n❌ Problem 1: Early return\n";
    problematic_function1(true);  // Memory leak!

    cout << "\n❌ Problems with raw pointers:\n";
    cout << "  • Memory leaks (forgot delete)\n";
    cout << "  • Double deletion (deleted twice)\n";
    cout << "  • Exception safety (exception before delete)\n";
    cout << "  • Complex ownership (who deletes?)\n";
    cout << "  • Dangling pointers (deleted, still used)\n";
    cout << "\n✅ Solution: Use unique_ptr!\n";
}
}

// ============================================
// EXAMPLE 2: Basic unique_ptr
// ============================================

namespace BasicUniquePtr {

class Widget {
public:
    Widget(int id) : id(id) {
        cout << "  Widget " << id << " created\n";
    }
    ~Widget() {
        cout << "  Widget " << id << " destroyed\n";
    }
    void doWork() {
        cout << "  Widget " << id << " working...\n";
    }
private:
    int id;
};

void demonstrate() {
    cout << "\n=== Example 2: Basic unique_ptr ===\n";

    cout << "\nCreating unique_ptr:\n";
    {
        unique_ptr<Widget> ptr = make_unique<Widget>(1);
        ptr->doWork();

        // Check if valid
        if (ptr) {
            cout << "  ptr is valid\n";
        }

        // Get raw pointer (rarely needed)
        Widget* raw = ptr.get();
        cout << "  Raw pointer: " << raw << "\n";

    }  // ✅ Widget automatically destroyed here!

    cout << "\n✅ Advantages:\n";
    cout << "  • Automatic cleanup (destructor calls delete)\n";
    cout << "  • Exception safe (RAII)\n";
    cout << "  • Can't forget to delete\n";
    cout << "  • Zero overhead vs raw pointer\n";
}
}

// ============================================
// EXAMPLE 3: make_unique (C++14)
// ============================================

namespace MakeUnique {

class Point {
public:
    Point(int x, int y) : x(x), y(y) {
        cout << "  Point(" << x << ", " << y << ") created\n";
    }
    ~Point() {
        cout << "  Point destroyed\n";
    }
    void display() const {
        cout << "  Point: (" << x << ", " << y << ")\n";
    }
private:
    int x, y;
};

void demonstrate() {
    cout << "\n=== Example 3: make_unique ===\n";

    cout << "\n✅ Preferred way (C++14+):\n";
    auto p1 = make_unique<Point>(10, 20);
    p1->display();

    cout << "\n❌ Old way (C++11, still works):\n";
    unique_ptr<Point> p2(new Point(30, 40));
    p2->display();

    cout << "\nWhy prefer make_unique?\n";
    cout << "  1. Exception safe in function calls\n";
    cout << "  2. No need to repeat type\n";
    cout << "  3. Cannot accidentally use raw pointer\n";
    cout << "  4. Consistent with make_shared\n";

    cout << "\nException safety example:\n";
    cout << "  // ❌ Not exception safe:\n";
    cout << "  func(unique_ptr<T>(new T), other_func());\n";
    cout << "  // If other_func() throws, T leaks!\n";
    cout << "\n";
    cout << "  // ✅ Exception safe:\n";
    cout << "  func(make_unique<T>(), other_func());\n";
    cout << "  // Guaranteed no leak!\n";
}
}

// ============================================
// EXAMPLE 4: Moving unique_ptr
// ============================================

namespace MovingUniquePtr {

class Resource {
public:
    Resource(string name) : name(name) {
        cout << "  Resource \"" << name << "\" created\n";
    }
    ~Resource() {
        cout << "  Resource \"" << name << "\" destroyed\n";
    }
    void use() {
        cout << "  Using resource \"" << name << "\"\n";
    }
private:
    string name;
};

// Function takes ownership by value
void take_ownership(unique_ptr<Resource> ptr) {
    cout << "  take_ownership() received resource\n";
    ptr->use();
    // ptr destroyed at end of function
}

// Function returns ownership
unique_ptr<Resource> create_resource(string name) {
    cout << "  create_resource() creating resource\n";
    return make_unique<Resource>(name);
    // Ownership transferred to caller
}

void demonstrate() {
    cout << "\n=== Example 4: Moving unique_ptr ===\n";

    cout << "\nTransferring ownership:\n";
    unique_ptr<Resource> r1 = make_unique<Resource>("R1");

    // ❌ Can't copy unique_ptr
    // unique_ptr<Resource> r2 = r1;  // Won't compile!

    // ✅ Move ownership with std::move
    unique_ptr<Resource> r2 = std::move(r1);

    if (!r1) {
        cout << "  r1 is now nullptr (ownership moved)\n";
    }
    if (r2) {
        cout << "  r2 now owns the resource\n";
    }

    cout << "\nPassing to function:\n";
    unique_ptr<Resource> r3 = make_unique<Resource>("R3");
    take_ownership(std::move(r3));  // Transfer ownership

    if (!r3) {
        cout << "  r3 is now nullptr (ownership transferred)\n";
    }

    cout << "\nReturning from function:\n";
    unique_ptr<Resource> r4 = create_resource("R4");
    r4->use();

    cout << "\n✅ Move semantics prevent double deletion!\n";
}
}

// ============================================
// EXAMPLE 5: reset() and release()
// ============================================

namespace ResetAndRelease {

class Widget {
public:
    Widget(int id) : id(id) {
        cout << "  Widget " << id << " created\n";
    }
    ~Widget() {
        cout << "  Widget " << id << " destroyed\n";
    }
    int getId() const { return id; }
private:
    int id;
};

void demonstrate() {
    cout << "\n=== Example 5: reset() and release() ===\n";

    cout << "\nreset() - Delete current and optionally set new:\n";
    unique_ptr<Widget> ptr = make_unique<Widget>(1);
    cout << "  Created Widget 1\n";

    ptr.reset();  // Delete Widget 1, set to nullptr
    cout << "  After reset(): ptr is " << (ptr ? "valid" : "nullptr") << "\n";

    ptr.reset(new Widget(2));  // Delete old (if any) and take new
    cout << "  After reset(new Widget(2)): ptr is valid\n";

    cout << "\nrelease() - Give up ownership WITHOUT deleting:\n";
    unique_ptr<Widget> ptr2 = make_unique<Widget>(3);
    Widget* raw = ptr2.release();  // ptr2 becomes nullptr

    cout << "  After release(): ptr2 is " << (ptr2 ? "valid" : "nullptr") << "\n";
    cout << "  Raw pointer: " << raw << "\n";
    cout << "  ⚠️  Must manually delete raw pointer!\n";
    delete raw;  // Manual cleanup required!

    cout << "\nComparison:\n";
    cout << "  reset()   - Deletes current object, takes new ownership\n";
    cout << "  release() - Gives up ownership, returns raw pointer\n";
    cout << "              (YOU must delete!)\n";
}
}

// ============================================
// EXAMPLE 6: Custom Deleters
// ============================================

namespace CustomDeleters {

// Custom deleter for FILE*
struct FileDeleter {
    void operator()(FILE* file) const {
        if (file) {
            cout << "  Closing file...\n";
            fclose(file);
        }
    }
};

// Custom deleter for array (though use unique_ptr<T[]> instead)
struct ArrayDeleter {
    void operator()(int* ptr) const {
        if (ptr) {
            cout << "  Deleting array...\n";
            delete[] ptr;
        }
    }
};

// Example resource
struct Connection {
    Connection() { cout << "  Connection opened\n"; }
    void send(const string& msg) {
        cout << "  Sending: " << msg << "\n";
    }
    // Custom cleanup function
    static void cleanup(Connection* conn) {
        if (conn) {
            cout << "  Connection closed\n";
            delete conn;
        }
    }
};

void demonstrate() {
    cout << "\n=== Example 6: Custom Deleters ===\n";

    cout << "\nCustom deleter for FILE*:\n";
    {
        unique_ptr<FILE, FileDeleter> file(
            fopen("test.txt", "w"),
            FileDeleter{}
            );

        if (file) {
            fprintf(file.get(), "Hello, World!\n");
        }
    }  // FileDeleter called automatically

    cout << "\nCustom deleter with lambda:\n";
    {
        unique_ptr<Connection, decltype(&Connection::cleanup)> conn(
            new Connection(),
            &Connection::cleanup
            );

        conn->send("Hello");
    }  // Connection::cleanup called automatically

    cout << "\nLambda deleter:\n";
    {
        auto deleter = [](int* ptr) {
            if (ptr) {
                cout << "  Lambda deleting: " << *ptr << "\n";
                delete ptr;
            }
        };

        unique_ptr<int, decltype(deleter)> ptr(new int(42), deleter);
        cout << "  Value: " << *ptr << "\n";
    }  // Lambda called automatically

    cout << "\n✅ Custom deleters for non-standard cleanup!\n";
}
}

// ============================================
// EXAMPLE 7: Arrays with unique_ptr
// ============================================

namespace ArraysWithUniquePtr {

void demonstrate() {
    cout << "\n=== Example 7: Arrays with unique_ptr ===\n";

    cout << "\nDynamic array (prefer std::vector!):\n";
    {
        // ✅ Correct: unique_ptr<T[]> for arrays
        unique_ptr<int[]> arr = make_unique<int[]>(5);

        // Initialize
        for (int i = 0; i < 5; ++i) {
            arr[i] = i * 10;
        }

        // Access with []
        cout << "  Array: [ ";
        for (int i = 0; i < 5; ++i) {
            cout << arr[i] << " ";
        }
        cout << "]\n";

        // ✅ Automatically uses delete[] instead of delete
    }  // Array properly deleted with delete[]

    cout << "\n⚠️  Important:\n";
    cout << "  • Use unique_ptr<T[]> for arrays\n";
    cout << "  • Automatically calls delete[] instead of delete\n";
    cout << "  • Supports [] operator\n";
    cout << "  • BUT: Prefer std::vector in most cases!\n";

    cout << "\n❌ Don't do this:\n";
    cout << "  unique_ptr<int> arr(new int[5]);  // ❌ Wrong!\n";
    cout << "  // Will use delete instead of delete[]!\n";
    cout << "\n";
    cout << "  ✅ Do this:\n";
    cout << "  unique_ptr<int[]> arr = make_unique<int[]>(5);\n";

    cout << "\n✅✅ Or better - use vector:\n";
    cout << "  vector<int> arr(5);  // Best choice!\n";
}
}

// ============================================
// EXAMPLE 8: Returning from Functions
// ============================================

namespace ReturningFromFunctions {

class Widget {
public:
    Widget(int id) : id(id) {
        cout << "  Widget " << id << " created\n";
    }
    ~Widget() {
        cout << "  Widget " << id << " destroyed\n";
    }
    void show() const {
        cout << "  Widget " << id << "\n";
    }
private:
    int id;
};

// Return unique_ptr by value - ownership transferred to caller
unique_ptr<Widget> createWidget(int id) {
    cout << "  createWidget() creating widget\n";
    auto widget = make_unique<Widget>(id);
    // Process widget...
    return widget;  // RVO/NRVO applies - no copy, no move!
    // Or: return make_unique<Widget>(id);
}

// Conditional creation
unique_ptr<Widget> maybeCreateWidget(bool create) {
    if (create) {
        return make_unique<Widget>(99);
    }
    return nullptr;  // Return empty unique_ptr
}

// Factory pattern
class Shape {
public:
    virtual ~Shape() = default;
    virtual void draw() const = 0;
};

class Circle : public Shape {
public:
    void draw() const override {
        cout << "  Drawing Circle\n";
    }
};

class Square : public Shape {
public:
    void draw() const override {
        cout << "  Drawing Square\n";
    }
};

unique_ptr<Shape> createShape(string type) {
    if (type == "circle") {
        return make_unique<Circle>();
    } else if (type == "square") {
        return make_unique<Square>();
    }
    return nullptr;
}

void demonstrate() {
    cout << "\n=== Example 8: Returning from Functions ===\n";

    cout << "\nReturning unique_ptr:\n";
    unique_ptr<Widget> w1 = createWidget(1);
    w1->show();

    cout << "\nConditional return:\n";
    unique_ptr<Widget> w2 = maybeCreateWidget(true);
    if (w2) {
        w2->show();
    }

    unique_ptr<Widget> w3 = maybeCreateWidget(false);
    if (!w3) {
        cout << "  w3 is nullptr\n";
    }

    cout << "\nFactory pattern:\n";
    auto shape1 = createShape("circle");
    if (shape1) shape1->draw();

    auto shape2 = createShape("square");
    if (shape2) shape2->draw();

    cout << "\n✅ Ownership automatically transferred to caller!\n";
    cout << "✅ No manual memory management needed!\n";
}
}

// ============================================
// EXAMPLE 9: Polymorphism with unique_ptr
// ============================================

namespace PolymorphismWithUniquePtr {

class Animal {
public:
    virtual ~Animal() {
        cout << "  Animal destroyed\n";
    }
    virtual void makeSound() const = 0;
    virtual void move() const = 0;
};

class Dog : public Animal {
public:
    ~Dog() override {
        cout << "  Dog destroyed\n";
    }
    void makeSound() const override {
        cout << "  Woof! Woof!\n";
    }
    void move() const override {
        cout << "  Running on four legs\n";
    }
};

class Cat : public Animal {
public:
    ~Cat() override {
        cout << "  Cat destroyed\n";
    }
    void makeSound() const override {
        cout << "  Meow!\n";
    }
    void move() const override {
        cout << "  Sneaking silently\n";
    }
};

class Bird : public Animal {
public:
    ~Bird() override {
        cout << "  Bird destroyed\n";
    }
    void makeSound() const override {
        cout << "  Chirp! Chirp!\n";
    }
    void move() const override {
        cout << "  Flying in the sky\n";
    }
};

void demonstrate() {
    cout << "\n=== Example 9: Polymorphism with unique_ptr ===\n";

    cout << "\nPolymorphic collection:\n";
    vector<unique_ptr<Animal>> animals;

    animals.push_back(make_unique<Dog>());
    animals.push_back(make_unique<Cat>());
    animals.push_back(make_unique<Bird>());

    cout << "\nMaking sounds:\n";
    for (const auto& animal : animals) {
        animal->makeSound();
    }

    cout << "\nMoving:\n";
    for (const auto& animal : animals) {
        animal->move();
    }

    cout << "\n✅ Polymorphic behavior with automatic cleanup!\n";
    cout << "✅ Virtual destructor ensures proper cleanup!\n";

}  // All animals automatically destroyed in reverse order
}

// ============================================
// EXAMPLE 10: Real-world - RAII Resource Manager
// ============================================

namespace ResourceManager {

// Resource that needs cleanup
class Database {
public:
    Database(const string& name) : name(name) {
        cout << "  [DB] Connecting to " << name << "...\n";
        connected = true;
    }

    ~Database() {
        if (connected) {
            cout << "  [DB] Disconnecting from " << name << "...\n";
        }
    }

    void query(const string& sql) {
        if (connected) {
            cout << "  [DB] Executing: " << sql << "\n";
        }
    }

    void close() {
        if (connected) {
            cout << "  [DB] Closing connection\n";
            connected = false;
        }
    }

private:
    string name;
    bool connected;
};

// File handler
class LogFile {
public:
    LogFile(const string& filename) : filename(filename) {
        cout << "  [LOG] Opening " << filename << "\n";
    }

    ~LogFile() {
        cout << "  [LOG] Closing " << filename << "\n";
    }

    void write(const string& message) {
        cout << "  [LOG] " << message << "\n";
    }

private:
    string filename;
};

// Manager class using unique_ptr for all resources
class Application {
public:
    Application() {
        cout << "\n[APP] Starting application...\n";

        // Initialize resources with unique_ptr
        db = make_unique<Database>("production_db");
        logger = make_unique<LogFile>("app.log");

        logger->write("Application started");
    }

    ~Application() {
        logger->write("Application shutting down");
        cout << "[APP] Destroying application...\n";
        // unique_ptr automatically cleans up resources
    }

    void run() {
        logger->write("Running application");

        try {
            db->query("SELECT * FROM users");
            db->query("UPDATE stats SET count = count + 1");

            // Simulate work
            logger->write("Processing complete");

        } catch (const exception& e) {
            logger->write(string("Error: ") + e.what());
            // ✅ unique_ptr ensures cleanup even on exception!
        }
    }

    void shutdown() {
        logger->write("Shutdown requested");
        db->close();
    }

private:
    unique_ptr<Database> db;
    unique_ptr<LogFile> logger;
    // ✅ Automatically cleaned up in destructor
    // ✅ Order: logger destroyed after db (reverse declaration)
};

void demonstrate() {
    cout << "\n=== Example 10: RAII Resource Manager ===\n";

    {
        Application app;
        app.run();
        app.shutdown();

    }  // ✅ All resources automatically cleaned up!

    cout << "\n✅ Benefits:\n";
    cout << "  • Automatic resource cleanup\n";
    cout << "  • Exception safe\n";
    cout << "  • No manual memory management\n";
    cout << "  • Clear ownership semantics\n";
    cout << "  • Destructor order guaranteed\n";
}
}

// ============================================
// COMPREHENSIVE OUTPUT
// ============================================

void printComprehensiveInfo() {
    cout << "\n" << string(60, '=') << "\n";
    cout << "unique_ptr - COMPLETE REFERENCE\n";
    cout << string(60, '=') << "\n";

    cout << "\n📚 WHAT IS unique_ptr?\n";
    cout << string(60, '-') << "\n";
    cout << "Smart pointer with unique ownership semantics:\n";
    cout << "  • Only ONE owner at a time\n";
    cout << "  • Automatically deletes when destroyed\n";
    cout << "  • Move-only (cannot copy)\n";
    cout << "  • Zero overhead vs raw pointer\n";
    cout << "  • Exception safe (RAII)\n";

    cout << "\n📊 KEY OPERATIONS\n";
    cout << string(60, '-') << "\n";
    cout << "Creation:\n";
    cout << "  auto ptr = make_unique<T>(args);     // ✅ Preferred (C++14)\n";
    cout << "  unique_ptr<T> ptr(new T(args));      // ✅ OK (C++11)\n";
    cout << "\n";
    cout << "Access:\n";
    cout << "  ptr->member()        // Access member\n";
    cout << "  *ptr                 // Dereference\n";
    cout << "  ptr.get()            // Get raw pointer\n";
    cout << "\n";
    cout << "Ownership:\n";
    cout << "  auto p2 = std::move(ptr);  // Transfer ownership\n";
    cout << "  ptr.reset()                // Delete and set to nullptr\n";
    cout << "  ptr.release()              // Give up ownership\n";
    cout << "\n";
    cout << "Check:\n";
    cout << "  if (ptr) { }         // Check if valid\n";
    cout << "  if (!ptr) { }        // Check if nullptr\n";

    cout << "\n🔧 SPECIAL CASES\n";
    cout << string(60, '-') << "\n";
    cout << "Arrays:\n";
    cout << "  unique_ptr<T[]> arr = make_unique<T[]>(size);\n";
    cout << "  arr[i] = value;  // Supports []\n";
    cout << "\n";
    cout << "Custom deleters:\n";
    cout << "  unique_ptr<FILE, FileDeleter> file(fopen(...));\n";
    cout << "  unique_ptr<T, decltype(&func)> ptr(new T, &func);\n";

    cout << "\n💡 COMMON PATTERNS\n";
    cout << string(60, '-') << "\n";
    cout << "Factory function:\n";
    cout << "  unique_ptr<Base> create() {\n";
    cout << "      return make_unique<Derived>();\n";
    cout << "  }\n";
    cout << "\n";
    cout << "Taking ownership:\n";
    cout << "  void take(unique_ptr<T> ptr) {\n";
    cout << "      // Owns ptr, deletes when done\n";
    cout << "  }\n";
    cout << "  take(std::move(my_ptr));  // Transfer\n";
    cout << "\n";
    cout << "Polymorphism:\n";
    cout << "  vector<unique_ptr<Base>> items;\n";
    cout << "  items.push_back(make_unique<Derived>());\n";

    cout << "\n⚠️  COMMON MISTAKES\n";
    cout << string(60, '-') << "\n";
    cout << "❌ Trying to copy:\n";
    cout << "   auto p2 = p1;  // Won't compile!\n";
    cout << "   ✅ Use std::move(p1)\n";
    cout << "\n";
    cout << "❌ Using new T[size] without []:\n";
    cout << "   unique_ptr<T> ptr(new T[5]);  // ❌ Wrong!\n";
    cout << "   ✅ unique_ptr<T[]> ptr = make_unique<T[]>(5);\n";
    cout << "\n";
    cout << "❌ Storing raw pointer elsewhere:\n";
    cout << "   T* raw = ptr.get();\n";
    cout << "   // ptr destroyed → raw is dangling!\n";
    cout << "   ✅ Don't store raw pointers from get()\n";
    cout << "\n";
    cout << "❌ Double ownership:\n";
    cout << "   T* raw = new T;\n";
    cout << "   unique_ptr<T> p1(raw);\n";
    cout << "   unique_ptr<T> p2(raw);  // ❌ Double delete!\n";
    cout << "   ✅ Only one unique_ptr per object\n";

    cout << "\n🎯 BEST PRACTICES\n";
    cout << string(60, '-') << "\n";
    cout << "✅ Prefer make_unique over new:\n";
    cout << "   auto ptr = make_unique<T>(args);  // ✅ Best\n";
    cout << "\n";
    cout << "✅ Return by value:\n";
    cout << "   unique_ptr<T> create() { return make_unique<T>(); }\n";
    cout << "\n";
    cout << "✅ Use for polymorphism:\n";
    cout << "   vector<unique_ptr<Base>> items;\n";
    cout << "\n";
    cout << "✅ Move when transferring:\n";
    cout << "   func(std::move(ptr));  // Transfer ownership\n";
    cout << "\n";
    cout << "✅ Check before use:\n";
    cout << "   if (ptr) { ptr->foo(); }\n";
    cout << "\n";
    cout << "✅ Prefer vector over unique_ptr<T[]>:\n";
    cout << "   vector<T> arr;  // Better than unique_ptr<T[]>\n";

    cout << "\n⚡ PERFORMANCE\n";
    cout << string(60, '-') << "\n";
    cout << "✅ Zero overhead:\n";
    cout << "  • Same size as raw pointer\n";
    cout << "  • No runtime cost vs raw pointer\n";
    cout << "  • Destructor call is free (would happen anyway)\n";
    cout << "  • Inline optimization possible\n";
    cout << "\n";
    cout << "Move vs Copy:\n";
    cout << "  Move:  ~1-2 cycles (pointer copy)\n";
    cout << "  Copy:  ❌ Deleted (not allowed)\n";

    cout << "\n🚀 C++ EVOLUTION\n";
    cout << string(60, '-') << "\n";
    cout << "C++11: unique_ptr introduced\n";
    cout << "C++14: make_unique added (finally!)\n";
    cout << "C++17: Better deduction\n";
    cout << "C++20: make_unique_for_overwrite\n";
}

// ============================================
// MAIN
// ============================================

int main() {
    cout << "\n╔═══════════════════════════════════════════════════════╗\n";
    cout << "║            unique_ptr IN C++                          ║\n";
    cout << "║                                                       ║\n";
    cout << "║  Automatic Memory Management with Zero Overhead!     ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n";

    RawPointerProblems::demonstrate();
    BasicUniquePtr::demonstrate();
    MakeUnique::demonstrate();
    MovingUniquePtr::demonstrate();
    ResetAndRelease::demonstrate();
    CustomDeleters::demonstrate();
    ArraysWithUniquePtr::demonstrate();
    ReturningFromFunctions::demonstrate();
    PolymorphismWithUniquePtr::demonstrate();
    ResourceManager::demonstrate();

    printComprehensiveInfo();

    cout << "\n" << string(60, '=') << "\n";
    cout << "🎓 KEY TAKEAWAYS\n";
    cout << string(60, '=') << "\n";
    cout << "1. unique_ptr = automatic memory management\n";
    cout << "2. Unique ownership - only one owner at a time\n";
    cout << "3. Use make_unique (exception safe, cleaner)\n";
    cout << "4. Move with std::move (can't copy)\n";
    cout << "5. Zero overhead vs raw pointers\n";
    cout << "6. Exception safe (RAII pattern)\n";
    cout << "7. Perfect for polymorphism\n";
    cout << "8. Return by value from functions\n";
    cout << "9. Never use raw new/delete in modern C++\n";
    cout << "10. Default choice for heap allocations\n";
    cout << string(60, '=') << "\n\n";

    return 0;
}
