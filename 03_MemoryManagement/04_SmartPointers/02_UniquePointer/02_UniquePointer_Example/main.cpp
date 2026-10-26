#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <memory>
#include <print>
#include <stdexcept>
#include <string>
#include <vector>

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
        std::println("  Widget {} created", this->id);
    }
    ~Widget() {
        std::println("  Widget {} destroyed", id);
    }
    void doWork() {
        std::println("  Widget {} working...", id);
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

// ❌ Problem 2: Memory leak on exception (not called — shown for the code only)
void problematic_function2() {
    Widget* ptr = new Widget(2);

    // Imagine some code that might throw
    if (rand() % 2) {
        throw std::runtime_error("Oops!");  // ❌ MEMORY LEAK!
    }

    delete ptr;  // Never reached if exception thrown
}

// ❌ Problem 3: Double deletion (not called — it would be undefined behavior)
void problematic_function3() {
    Widget* ptr = new Widget(3);
    delete ptr;
    // ... complex code ...
    delete ptr;  // ❌ DOUBLE DELETE! Undefined behavior!
}

void demonstrate() {
    std::println("");
    std::println("=== Example 1: Raw Pointer Problems ===");

    std::println("");
    std::println("❌ Problem 1: Early return");
    problematic_function1(true);  // Memory leak!
    std::println("  (no 'destroyed' line above — the Widget leaked)");
}
}

// ============================================
// EXAMPLE 2: Basic unique_ptr
// ============================================

namespace BasicUniquePtr {

class Widget {
public:
    Widget(int id) : id(id) {
        std::println("  Widget {} created", this->id);
    }
    ~Widget() {
        std::println("  Widget {} destroyed", id);
    }
    void doWork() {
        std::println("  Widget {} working...", id);
    }
private:
    int id;
};

void demonstrate() {
    std::println("");
    std::println("=== Example 2: Basic unique_ptr ===");

    std::println("");
    std::println("Creating unique_ptr:");
    {
        std::unique_ptr<Widget> ptr = std::make_unique<Widget>(1);
        ptr->doWork();

        // Check if valid
        if (ptr) {
            std::println("  ptr is valid");
        }

        // Get raw pointer (rarely needed)
        Widget* raw = ptr.get();
        std::println("  Raw pointer: {}", static_cast<const void*>(raw));

    }  // ✅ Widget automatically destroyed here!
}
}

// ============================================
// EXAMPLE 3: make_unique (C++14)
// ============================================

namespace MakeUnique {

class Point {
public:
    Point(int x, int y) : x(x), y(y) {
        std::println("  Point({}, {}) created", this->x, this->y);
    }
    ~Point() {
        std::println("  Point destroyed");
    }
    void display() const {
        std::println("  Point: ({}, {})", x, y);
    }
private:
    int x, y;
};

void demonstrate() {
    std::println("");
    std::println("=== Example 3: make_unique ===");

    std::println("");
    std::println("✅ Preferred way (C++14+):");
    auto p1 = std::make_unique<Point>(10, 20);
    p1->display();

    std::println("");
    std::println("❌ Old way (C++11, still works):");
    std::unique_ptr<Point> p2(new Point(30, 40));
    p2->display();
}
}

// ============================================
// EXAMPLE 4: Moving unique_ptr
// ============================================

namespace MovingUniquePtr {

class Resource {
public:
    Resource(std::string name) : name(name) {
        std::println("  Resource \"{}\" created", this->name);
    }
    ~Resource() {
        std::println("  Resource \"{}\" destroyed", name);
    }
    void use() {
        std::println("  Using resource \"{}\"", name);
    }
private:
    std::string name;
};

// Function takes ownership by value
void take_ownership(std::unique_ptr<Resource> ptr) {
    std::println("  take_ownership() received resource");
    ptr->use();
    // ptr destroyed at end of function
}

// Function returns ownership
std::unique_ptr<Resource> create_resource(std::string name) {
    std::println("  create_resource() creating resource");
    return std::make_unique<Resource>(name);
    // Ownership transferred to caller
}

void demonstrate() {
    std::println("");
    std::println("=== Example 4: Moving unique_ptr ===");

    std::println("");
    std::println("Transferring ownership:");
    std::unique_ptr<Resource> r1 = std::make_unique<Resource>("R1");

    // ❌ Can't copy unique_ptr
    // unique_ptr<Resource> r2 = r1;  // Won't compile!

    // ✅ Move ownership with std::move
    std::unique_ptr<Resource> r2 = std::move(r1);

    if (!r1) {
        std::println("  r1 is now nullptr (ownership moved)");
    }
    if (r2) {
        std::println("  r2 now owns the resource");
    }

    std::println("");
    std::println("Passing to function:");
    std::unique_ptr<Resource> r3 = std::make_unique<Resource>("R3");
    take_ownership(std::move(r3));  // Transfer ownership

    if (!r3) {
        std::println("  r3 is now nullptr (ownership transferred)");
    }

    std::println("");
    std::println("Returning from function:");
    std::unique_ptr<Resource> r4 = create_resource("R4");
    r4->use();
}
}

// ============================================
// EXAMPLE 5: reset() and release()
// ============================================

namespace ResetAndRelease {

class Widget {
public:
    Widget(int id) : id(id) {
        std::println("  Widget {} created", this->id);
    }
    ~Widget() {
        std::println("  Widget {} destroyed", id);
    }
    int getId() const { return id; }
private:
    int id;
};

void demonstrate() {
    std::println("");
    std::println("=== Example 5: reset() and release() ===");

    std::println("");
    std::println("reset() - Delete current and optionally set new:");
    std::unique_ptr<Widget> ptr = std::make_unique<Widget>(1);
    std::println("  Created Widget 1");

    ptr.reset();  // Delete Widget 1, set to nullptr
    std::println("  After reset(): ptr is {}", ptr ? "valid" : "nullptr");

    ptr.reset(new Widget(2));  // Delete old (if any) and take new
    std::println("  After reset(new Widget(2)): ptr is valid");

    std::println("");
    std::println("release() - Give up ownership WITHOUT deleting:");
    std::unique_ptr<Widget> ptr2 = std::make_unique<Widget>(3);
    Widget* raw = ptr2.release();  // ptr2 becomes nullptr

    std::println("  After release(): ptr2 is {}", ptr2 ? "valid" : "nullptr");
    std::println("  Raw pointer: {}", static_cast<const void*>(raw));
    std::println("  ⚠️  Must manually delete raw pointer!");
    delete raw;  // Manual cleanup required!
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
            std::println("  Closing file...");
            fclose(file);
        }
    }
};

// Custom deleter for array (though use unique_ptr<T[]> instead)
struct ArrayDeleter {
    void operator()(int* ptr) const {
        if (ptr) {
            std::println("  Deleting array...");
            delete[] ptr;
        }
    }
};

// Example resource
struct Connection {
    Connection() { std::println("  Connection opened"); }
    void send(const std::string& msg) {
        std::println("  Sending: {}", msg);
    }
    // Custom cleanup function
    static void cleanup(Connection* conn) {
        if (conn) {
            std::println("  Connection closed");
            delete conn;
        }
    }
};

void demonstrate() {
    std::println("");
    std::println("=== Example 6: Custom Deleters ===");

    std::println("");
    std::println("Custom deleter for FILE*:");
    {
        std::unique_ptr<FILE, FileDeleter> file(
            fopen("test.txt", "w"),
            FileDeleter{}
            );

        if (file) {
            fprintf(file.get(), "Hello, World!\n");
        }
    }  // FileDeleter called automatically

    std::println("");
    std::println("Custom deleter with function pointer:");
    {
        std::unique_ptr<Connection, decltype(&Connection::cleanup)> conn(
            new Connection(),
            &Connection::cleanup
            );

        conn->send("Hello");
    }  // Connection::cleanup called automatically

    std::println("");
    std::println("Lambda deleter:");
    {
        auto deleter = [](int* ptr) {
            if (ptr) {
                std::println("  Lambda deleting: {}", *ptr);
                delete ptr;
            }
        };

        std::unique_ptr<int, decltype(deleter)> ptr(new int(42), deleter);
        std::println("  Value: {}", *ptr);
    }  // Lambda called automatically
}
}

// ============================================
// EXAMPLE 7: Arrays with unique_ptr
// ============================================

namespace ArraysWithUniquePtr {

void demonstrate() {
    std::println("");
    std::println("=== Example 7: Arrays with unique_ptr ===");

    std::println("");
    std::println("Dynamic array (prefer std::vector!):");
    {
        // ✅ Correct: unique_ptr<T[]> for arrays
        std::unique_ptr<int[]> arr = std::make_unique<int[]>(5);

        // Initialize
        for (int i = 0; i < 5; ++i) {
            arr[i] = i * 10;
        }

        // Access with []
        std::print("  Array: [ ");
        for (int i = 0; i < 5; ++i) {
            std::print("{} ", arr[i]);
        }
        std::println("]");

        // ✅ Automatically uses delete[] instead of delete
    }  // Array properly deleted with delete[]
}
}

// ============================================
// EXAMPLE 8: Returning from Functions
// ============================================

namespace ReturningFromFunctions {

class Widget {
public:
    Widget(int id) : id(id) {
        std::println("  Widget {} created", this->id);
    }
    ~Widget() {
        std::println("  Widget {} destroyed", id);
    }
    void show() const {
        std::println("  Widget {}", id);
    }
private:
    int id;
};

// Return unique_ptr by value - ownership transferred to caller
std::unique_ptr<Widget> createWidget(int id) {
    std::println("  createWidget() creating widget");
    auto widget = std::make_unique<Widget>(id);
    // Process widget...
    return widget;  // returned by value: NRVO usually applies, otherwise it is moved — never copied
    // Or: return make_unique<Widget>(id);  (copy elision is guaranteed here)
}

// Conditional creation
std::unique_ptr<Widget> maybeCreateWidget(bool create) {
    if (create) {
        return std::make_unique<Widget>(99);
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
        std::println("  Drawing Circle");
    }
};

class Square : public Shape {
public:
    void draw() const override {
        std::println("  Drawing Square");
    }
};

std::unique_ptr<Shape> createShape(std::string type) {
    if (type == "circle") {
        return std::make_unique<Circle>();
    } else if (type == "square") {
        return std::make_unique<Square>();
    }
    return nullptr;
}

void demonstrate() {
    std::println("");
    std::println("=== Example 8: Returning from Functions ===");

    std::println("");
    std::println("Returning unique_ptr:");
    std::unique_ptr<Widget> w1 = createWidget(1);
    w1->show();

    std::println("");
    std::println("Conditional return:");
    std::unique_ptr<Widget> w2 = maybeCreateWidget(true);
    if (w2) {
        w2->show();
    }

    std::unique_ptr<Widget> w3 = maybeCreateWidget(false);
    if (!w3) {
        std::println("  w3 is nullptr");
    }

    std::println("");
    std::println("Factory pattern:");
    auto shape1 = createShape("circle");
    if (shape1) shape1->draw();

    auto shape2 = createShape("square");
    if (shape2) shape2->draw();
}
}

// ============================================
// EXAMPLE 9: Polymorphism with unique_ptr
// ============================================

namespace PolymorphismWithUniquePtr {

class Animal {
public:
    virtual ~Animal() {
        std::println("  Animal destroyed");
    }
    virtual void makeSound() const = 0;
    virtual void move() const = 0;
};

class Dog : public Animal {
public:
    ~Dog() override {
        std::println("  Dog destroyed");
    }
    void makeSound() const override {
        std::println("  Woof! Woof!");
    }
    void move() const override {
        std::println("  Running on four legs");
    }
};

class Cat : public Animal {
public:
    ~Cat() override {
        std::println("  Cat destroyed");
    }
    void makeSound() const override {
        std::println("  Meow!");
    }
    void move() const override {
        std::println("  Sneaking silently");
    }
};

class Bird : public Animal {
public:
    ~Bird() override {
        std::println("  Bird destroyed");
    }
    void makeSound() const override {
        std::println("  Chirp! Chirp!");
    }
    void move() const override {
        std::println("  Flying in the sky");
    }
};

void demonstrate() {
    std::println("");
    std::println("=== Example 9: Polymorphism with unique_ptr ===");

    std::println("");
    std::println("Polymorphic collection:");
    std::vector<std::unique_ptr<Animal>> animals;

    animals.push_back(std::make_unique<Dog>());
    animals.push_back(std::make_unique<Cat>());
    animals.push_back(std::make_unique<Bird>());

    std::println("");
    std::println("Making sounds:");
    for (const auto& animal : animals) {
        animal->makeSound();
    }

    std::println("");
    std::println("Moving:");
    for (const auto& animal : animals) {
        animal->move();
    }

}  // All animals destroyed in vector order (first element first); each one runs ~Derived then ~Animal
}

// ============================================
// EXAMPLE 10: Real-world - RAII Resource Manager
// ============================================

namespace ResourceManager {

// Resource that needs cleanup
class Database {
public:
    Database(const std::string& name) : name(name) {
        std::println("  [DB] Connecting to {}...", this->name);
        connected = true;
    }

    ~Database() {
        if (connected) {
            std::println("  [DB] Disconnecting from {}...", name);
        }
    }

    void query(const std::string& sql) {
        if (connected) {
            std::println("  [DB] Executing: {}", sql);
        }
    }

    void close() {
        if (connected) {
            std::println("  [DB] Closing connection");
            connected = false;
        }
    }

private:
    std::string name;
    bool connected;
};

// File handler
class LogFile {
public:
    LogFile(const std::string& filename) : filename(filename) {
        std::println("  [LOG] Opening {}", this->filename);
    }

    ~LogFile() {
        std::println("  [LOG] Closing {}", filename);
    }

    void write(const std::string& message) {
        std::println("  [LOG] {}", message);
    }

private:
    std::string filename;
};

// Manager class using unique_ptr for all resources
class Application {
public:
    Application() {
        std::println("");
        std::println("[APP] Starting application...");

        // Initialize resources with unique_ptr
        db = std::make_unique<Database>("production_db");
        logger = std::make_unique<LogFile>("app.log");

        logger->write("Application started");
    }

    ~Application() {
        logger->write("Application shutting down");
        std::println("[APP] Destroying application...");
        // unique_ptr automatically cleans up resources
    }

    void run() {
        logger->write("Running application");

        try {
            db->query("SELECT * FROM users");
            db->query("UPDATE stats SET count = count + 1");

            // Simulate work
            logger->write("Processing complete");

        } catch (const std::exception& e) {
            logger->write(std::string("Error: ") + e.what());
            // ✅ unique_ptr ensures cleanup even on exception!
        }
    }

    void shutdown() {
        logger->write("Shutdown requested");
        db->close();
    }

private:
    std::unique_ptr<Database> db;
    std::unique_ptr<LogFile> logger;
    // ✅ Automatically cleaned up in destructor
    // ✅ Members are destroyed in reverse declaration order: logger first, then db
};

void demonstrate() {
    std::println("");
    std::println("=== Example 10: RAII Resource Manager ===");

    {
        Application app;
        app.run();
        app.shutdown();

    }  // ✅ All resources automatically cleaned up!
}
}

// ============================================
// MAIN
// ============================================

int main() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════════╗");
    std::println("║            unique_ptr IN C++                          ║");
    std::println("║                                                       ║");
    std::println("║  Automatic Memory Management with Zero Overhead!     ║");
    std::println("╚═══════════════════════════════════════════════════════╝");

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

    return 0;
}
