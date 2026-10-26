#include <algorithm>
#include <memory>
#include <print>
#include <string>
#include <vector>

// ============================================
// WHAT IS THE RULE OF THREE/FIVE/ZERO?
// ============================================

/*
THE BIG PROBLEM:
    When your class manages resources (memory, files, sockets, etc.),
    C++ provides default versions of special member functions that
    do SHALLOW COPIES - this causes DOUBLE DELETION and MEMORY LEAKS!

THE SOLUTION - THREE RULES:

1. RULE OF THREE (C++98):
   If you need ONE of these, you need ALL THREE:
   - Destructor
   - Copy Constructor
   - Copy Assignment Operator

2. RULE OF FIVE (C++11):
   Add move semantics for performance:
   - Destructor
   - Copy Constructor
   - Copy Assignment Operator
   - Move Constructor                    ← NEW
   - Move Assignment Operator            ← NEW

3. RULE OF ZERO (Modern C++):
   BEST APPROACH - Don't define ANY!
   Use RAII types (unique_ptr, vector, string) instead.
   Let the compiler generate everything correctly.

GOLDEN RULE: Prefer Rule of Zero > Rule of Five > Rule of Three
*/

// ============================================
// === EXAMPLE 1: THE PROBLEM - NO RULE OF THREE ===
// ============================================

class BadResource {
private:
    int* data;
    std::string name;

public:
    BadResource(int value, const std::string& n) : name(n) {
        data = new int(value);
        std::println("✅ [{}] Constructor: allocated data = {}", name, *data);
    }

    ~BadResource() {
        std::println("💥 [{}] Destructor: deleting data", name);
        delete data;
    }

    // Problem: Using compiler-generated copy constructor and copy assignment!
    // They do SHALLOW copies - both objects share the same pointer!

    void display() const {
        std::println("   [{}] data = {}", name, data ? std::to_string(*data) : "nullptr");
    }
};

void demonstrateProblem() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: THE PROBLEM            ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("Creating obj1...");
    BadResource obj1(42, "Obj1");
    obj1.display();

    std::println("");
    std::println("Copying obj1 to obj2 (shallow copy!)...");
    BadResource obj2 = obj1;  // DANGER! Both now share the same pointer!
    obj2.display();

    std::println("");
    std::println("--- Destruction phase (watch for crash!) ---");

    // When obj2 and obj1 are destroyed:
    // obj2 destructor: delete data; ✅ OK
    // obj1 destructor: delete data; 💥 CRASH! Already deleted!
}

// ============================================
// === EXAMPLE 2: RULE OF THREE - THE SOLUTION ===
// ============================================

class GoodResource {
private:
    int* data;
    std::string name;

public:
    // Constructor
    GoodResource(int value, const std::string& n) : name(n) {
        data = new int(value);
        std::println("✅ [{}] Constructor: data = {}", name, *data);
    }

    // 1. Destructor
    ~GoodResource() {
        std::println("💥 [{}] Destructor", name);
        delete data;
    }

    // 2. Copy Constructor (DEEP COPY!)
    GoodResource(const GoodResource& other) : name(other.name + "_copy") {
        data = new int(*other.data);  // Allocate new memory and copy value
        std::println("📋 [{}] Copy Constructor (deep copy)", name);
    }

    // 3. Copy Assignment Operator (DEEP COPY!)
    GoodResource& operator=(const GoodResource& other) {
        std::println("📋 [{}] Copy Assignment", name);

        // Check for self-assignment
        if (this == &other) {
            std::println("   Self-assignment detected, skipping");
            return *this;
        }

        // Clean up existing resource
        delete data;

        // Deep copy
        data = new int(*other.data);
        name = other.name + "_copy";

        return *this;
    }

    void display() const {
        std::println("   [{}] data = {}", name, *data);
    }
};

void demonstrateRuleOfThree() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: RULE OF THREE          ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating obj1 ---");
    GoodResource obj1(100, "Obj1");
    obj1.display();

    std::println("");
    std::println("--- Copy Construction (obj2 = obj1) ---");
    GoodResource obj2 = obj1;  // Calls copy constructor
    obj2.display();

    std::println("");
    std::println("--- Copy Assignment (obj3 = obj1) ---");
    GoodResource obj3(200, "Obj3");
    obj3 = obj1;  // Calls copy assignment operator
    obj3.display();

    std::println("");
    std::println("--- Self-Assignment Test (obj1 = obj1) ---");
    obj1 = obj1;  // Should detect and skip

    std::println("");
    std::println("--- Destruction phase ---");
}

// ============================================
// === EXAMPLE 3: RULE OF FIVE - WITH MOVE ===
// ============================================

class RuleOfFive {
private:
    int* data;
    std::string name;

public:
    // Constructor
    RuleOfFive(int value, const std::string& n) : name(n) {
        data = new int(value);
        std::println("✅ [{}] Constructor: data = {}", name, *data);
    }

    // 1. Destructor
    ~RuleOfFive() {
        std::println("💥 [{}] Destructor", name);
        delete data;
    }

    // 2. Copy Constructor
    RuleOfFive(const RuleOfFive& other) : name(other.name + "_copy") {
        data = new int(*other.data);
        std::println("📋 [{}] Copy Constructor", name);
    }

    // 3. Copy Assignment
    RuleOfFive& operator=(const RuleOfFive& other) {
        std::println("📋 [{}] Copy Assignment", name);
        if (this != &other) {
            delete data;
            data = new int(*other.data);
            name = other.name + "_copy";
        }
        return *this;
    }

    // 4. Move Constructor (NEW in C++11!)
    RuleOfFive(RuleOfFive&& other) noexcept
        : data(other.data), name(other.name + "_moved") {
        std::println("🚀 [{}] Move Constructor", name);
        other.data = nullptr;  // Leave source in valid state
    }

    // 5. Move Assignment (NEW in C++11!)
    RuleOfFive& operator=(RuleOfFive&& other) noexcept {
        std::println("🚀 [{}] Move Assignment", name);
        if (this != &other) {
            delete data;           // Clean up our resource
            data = other.data;     // Steal other's resource
            name = other.name + "_moved";
            other.data = nullptr;  // Leave source in valid state
        }
        return *this;
    }

    int getValue() const { return data ? *data : -1; }
    void display() const {
        std::println("   [{}] data = {}", name, data ? std::to_string(*data) : "nullptr");
    }
};

void demonstrateRuleOfFive() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: RULE OF FIVE           ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating obj1 ---");
    RuleOfFive obj1(100, "Obj1");
    obj1.display();

    std::println("");
    std::println("--- Copy Constructor (obj2 = obj1) ---");
    RuleOfFive obj2 = obj1;
    obj2.display();
    obj1.display();  // obj1 still valid

    std::println("");
    std::println("--- Move Constructor (obj3 = move(obj1)) ---");
    RuleOfFive obj3 = std::move(obj1);  // Transfer ownership
    obj3.display();
    obj1.display();  // obj1 now empty (data = nullptr)

    std::println("");
    std::println("--- Copy Assignment (obj4 = obj2) ---");
    RuleOfFive obj4(200, "Obj4");
    obj4 = obj2;
    obj4.display();

    std::println("");
    std::println("--- Move Assignment (obj5 = move(obj2)) ---");
    RuleOfFive obj5(300, "Obj5");
    obj5 = std::move(obj2);  // Transfer ownership
    obj5.display();
    obj2.display();  // obj2 now empty

    std::println("");
    std::println("--- Destruction phase ---");
}

// ============================================
// === EXAMPLE 4: RULE OF ZERO - THE BEST! ===
// ============================================

// BAD: Manual resource management (Rule of Five)
class ManualResource {
private:
    int* data;           // Raw pointer - needs custom special members
    char* buffer;        // Another raw pointer - more work!
    int size;

public:
    ManualResource(int s) : size(s) {
        data = new int[size];
        buffer = new char[size];
    }

    // Need to define all 5 special members! 😫
    ~ManualResource() {
        delete[] data;
        delete[] buffer;
    }

    ManualResource(const ManualResource& other) : size(other.size) {
        data = new int[size];
        buffer = new char[size];
        std::copy(other.data, other.data + size, data);
        std::copy(other.buffer, other.buffer + size, buffer);
    }

    ManualResource& operator=(const ManualResource& other) {
        if (this != &other) {
            delete[] data;
            delete[] buffer;
            size = other.size;
            data = new int[size];
            buffer = new char[size];
            std::copy(other.data, other.data + size, data);
            std::copy(other.buffer, other.buffer + size, buffer);
        }
        return *this;
    }

    ManualResource(ManualResource&& other) noexcept
        : data(other.data), buffer(other.buffer), size(other.size) {
        other.data = nullptr;
        other.buffer = nullptr;
    }

    ManualResource& operator=(ManualResource&& other) noexcept {
        if (this != &other) {
            delete[] data;
            delete[] buffer;
            data = other.data;
            buffer = other.buffer;
            size = other.size;
            other.data = nullptr;
            other.buffer = nullptr;
        }
        return *this;
    }
};

// GOOD: Use RAII types (Rule of Zero!)
class AutoResource {
private:
    std::vector<int> data;      // RAII - manages its own memory!
    std::vector<char> buffer;   // RAII - manages its own memory!
    std::string name;           // RAII - manages its own memory!
    std::unique_ptr<int> ptr;   // RAII - manages its own memory!

public:
    AutoResource(int size, const std::string& n)
        : data(size), buffer(size), name(n), ptr(std::make_unique<int>(42)) {
        std::println("✅ [{}] Constructor (using RAII types)", name);
    }

    // NO NEED TO DEFINE ANY SPECIAL MEMBERS! 🎉
    // The compiler generates what is correct for the members:
    // - Destructor          ✅ Auto-generated
    // - Copy Constructor    ❌ implicitly DELETED (unique_ptr is move-only)
    // - Copy Assignment     ❌ implicitly DELETED (unique_ptr is move-only)
    // - Move Constructor    ✅ Auto-generated
    // - Move Assignment     ✅ Auto-generated

    void display() const {
        std::println("   [{}] data.size = {}, ptr = {}", name, data.size(), *ptr);
    }
};

void demonstrateRuleOfZero() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: RULE OF ZERO (BEST!)   ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Testing Rule of Zero class ---");
    AutoResource obj1(1000, "Obj1");
    obj1.display();

    std::println("");
    std::println("--- Copy constructor (deleted) ---");
    // AutoResource obj2 = obj1;  // ❌ ERROR: Copy is deleted!
    // obj2.display();
    std::println("   Copy constructor is deleted - would cause error");

    std::println("");
    std::println("--- Move (auto-generated move constructor) ---");
    AutoResource obj3 = std::move(obj1);
    obj3.display();

    std::println("");
    std::println("--- Destruction phase (auto-generated destructors) ---");
}

// ============================================
// === EXAMPLE 5: DEFAULT AND DELETE ===
// ============================================

class NonCopyable {
private:
    std::unique_ptr<int> data;

public:
    NonCopyable(int value) : data(std::make_unique<int>(value)) {
        std::println("✅ NonCopyable: Constructor");
    }

    // Delete copy operations
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;

    // Default move operations
    NonCopyable(NonCopyable&&) = default;
    NonCopyable& operator=(NonCopyable&&) = default;

    // Default destructor
    ~NonCopyable() = default;
};

void demonstrateDefaultDelete() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 5: = DEFAULT & = DELETE   ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Testing ---");
    NonCopyable obj1(42);

    // NonCopyable obj2 = obj1;  // ❌ Compile error! Deleted!

    NonCopyable obj3 = std::move(obj1);  // ✅ OK! Move is allowed
    std::println("✅ Move succeeded!");
}

// ============================================
// === EXAMPLE 6: COPY VS MOVE ===
// ============================================

class HeavyResource {
private:
    std::vector<int> data;
    std::string name;

public:
    HeavyResource(int size, const std::string& n) : data(size, 42), name(n) {}

    // Copy - EXPENSIVE!
    HeavyResource(const HeavyResource& other) : data(other.data), name(other.name + "_copy") {
        std::println("📋 Copy: {} elements (EXPENSIVE!)", data.size());
    }

    // Move - CHEAP!
    HeavyResource(HeavyResource&& other) noexcept
        : data(std::move(other.data)), name(std::move(other.name) + "_moved") {
        std::println("🚀 Move: Just pointer swap (CHEAP!)");
    }
};

void demonstratePerformance() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 6: COPY VS MOVE           ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("Creating large object (1 million elements)...");
    HeavyResource obj1(1000000, "Heavy");

    std::println("");
    std::println("--- Copy (expensive) ---");
    HeavyResource obj2 = obj1;  // Copies 1 million elements!

    std::println("");
    std::println("--- Move (cheap) ---");
    HeavyResource obj3 = std::move(obj1);  // Just swaps pointers!
}

// ============================================
// MAIN - RUN ALL EXAMPLES
// ============================================

int main() {
    std::println("╔══════════════════════════════════════════════════════════╗");
    std::println("║                                                          ║");
    std::println("║       RULE OF THREE / FIVE / ZERO - COMPLETE GUIDE       ║");
    std::println("║                                                          ║");
    std::println("╚══════════════════════════════════════════════════════════╝");

    try {
        // Note: Example 1 will crash! Run others separately.
        // demonstrateProblem();          // Shows the problem

        demonstrateRuleOfThree();         // Solution: Rule of Three
        demonstrateRuleOfFive();          // Complete: Rule of Five
        demonstrateRuleOfZero();          // Best: Rule of Zero
        demonstrateDefaultDelete();       // = default & = delete
        demonstratePerformance();         // Copy vs Move

    } catch (const std::exception& e) {
        std::println("💥 Exception: {}", e.what());
    }

    return 0;
}
