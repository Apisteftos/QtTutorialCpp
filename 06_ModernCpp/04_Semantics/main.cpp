#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <print>

// ============================================
// MOVE SEMANTICS
// C++11 Feature for Performance Optimization
// ============================================

// ============================================
// EXAMPLE 1: UNDERSTANDING LVALUES AND RVALUES
// ============================================

void demonstrateLvaluesRvalues() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: LVALUES vs RVALUES    ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    // LVALUES - have names, persistent, can take address
    std::println("--- Lvalues (Left values) ---");
    int x = 10;
    int y = 20;
    [[maybe_unused]] int* ptr = &x;
    std::println("x address: {}", static_cast<const void*>(&x));
    std::println("y address: {}", static_cast<const void*>(&y));

    // RVALUES - temporaries, no persistent address
    [[maybe_unused]] int z = x + y;  // (x + y) is rvalue

    std::println("");
    std::println("--- Function Returns (Rvalues) ---");
    auto getValue = []() -> int {
        std::println("   Creating temporary value...");
        return 42;
    };

    int result = getValue();  // getValue() returns rvalue
    std::println("Result: {}", result);

    std::string s1 = "Hello";  // "Hello" is rvalue (temporary)
}

// ============================================
// EXAMPLE 2: THE COPY PROBLEM
// ============================================

class HeavyResource {
private:
    int* data;
    size_t size;
    std::string name;

public:
    // Constructor
    HeavyResource(size_t s, const std::string& n) : size(s), name(n) {
        data = new int[size];
        std::println("🏗️  [{}] Constructor: Allocated {} elements", name, size);
        // Initialize with some data
        for (size_t i = 0; i < size; i++) {
            data[i] = i;
        }
    }

    // Copy Constructor - EXPENSIVE!
    HeavyResource(const HeavyResource& other)
        : size(other.size), name(other.name + "_copy") {
        std::println("📋 [{}] COPY Constructor: Deep copying {} elements (EXPENSIVE! 💰)", name, size);
        data = new int[size];
        // Deep copy - copies every element
        for (size_t i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }

    // Destructor
    ~HeavyResource() {
        std::println("💥 [{}] Destructor: Freeing {} elements", name, size);
        delete[] data;
    }

    size_t getSize() const { return size; }
    std::string getName() const { return name; }
};

void demonstrateCopyProblem() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: THE COPY PROBLEM       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating original resource ---");
    HeavyResource original(1000000, "Original");  // 1 million elements

    std::println("");
    std::println("--- Making a copy (expensive!) ---");
    HeavyResource copy = original;  // Deep copy!


    std::println("");
    std::println("--- Destruction phase ---");
}

// ============================================
// EXAMPLE 3: MOVE CONSTRUCTOR SOLUTION
// ============================================

class OptimizedResource {
private:
    int* data;
    size_t size;
    std::string name;

public:
    // Constructor
    OptimizedResource(size_t s, const std::string& n) : size(s), name(n) {
        data = new int[size];
        std::println("🏗️  [{}] Constructor: Allocated {} elements", name, size);
        for (size_t i = 0; i < size; i++) {
            data[i] = i;
        }
    }

    // Copy Constructor - EXPENSIVE (still needed sometimes)
    OptimizedResource(const OptimizedResource& other)
        : size(other.size), name(other.name + "_copy") {
        std::println("📋 [{}] COPY Constructor: Deep copying {} elements (expensive)", name, size);
        data = new int[size];
        for (size_t i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }

    // Move Constructor - CHEAP! (C++11)
    OptimizedResource(OptimizedResource&& other) noexcept
        : data(other.data), size(other.size), name(other.name + "_moved") {
        std::println("🚀 [{}] MOVE Constructor: Stealing {} elements (CHEAP! ⚡)", name, size);

        // "Steal" the resource from other
        other.data = nullptr;
        other.size = 0;
        other.name = other.name + "_moved-from";
    }

    // Copy Assignment
    OptimizedResource& operator=(const OptimizedResource& other) {
        std::println("📋 [{}] COPY Assignment", name);
        if (this != &other) {
            delete[] data;
            size = other.size;
            name = other.name + "_copy";
            data = new int[size];
            for (size_t i = 0; i < size; i++) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    // Move Assignment (C++11)
    OptimizedResource& operator=(OptimizedResource&& other) noexcept {
        std::println("🚀 [{}] MOVE Assignment", name);
        if (this != &other) {
            delete[] data;

            // "Steal" the resources
            data = other.data;
            size = other.size;
            name = other.name + "_moved";

            // Leave other in valid but empty state
            other.data = nullptr;
            other.size = 0;
            other.name = other.name + "_moved-from";
        }
        return *this;
    }

    // Destructor
    ~OptimizedResource() {
        if (data) {
            std::println("💥 [{}] Destructor: Freeing {} elements", name, size);
            delete[] data;
        } else {
            std::println("💥 [{}] Destructor: Empty (moved-from state)", name);
        }
    }

    size_t getSize() const { return size; }
    std::string getName() const { return name; }
};

void demonstrateMoveConstructor() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: MOVE CONSTRUCTOR       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating original ---");
    OptimizedResource original(1000000, "Original");

    std::println("");
    std::println("--- Copy (still expensive when needed) ---");
    OptimizedResource copy = original;  // Copy constructor

    std::println("");
    std::println("--- Move (cheap! using std::move) ---");
    OptimizedResource moved = std::move(original);  // Move constructor
    // std::move casts 'original' to rvalue reference

    std::println("");
    std::println("📊 After move:");
    std::println("   original.size: {} (moved-from)", original.getSize());
    std::println("   moved.size: {} (has the data)", moved.getSize());


    std::println("");
    std::println("--- Destruction phase ---");
}

// ============================================
// EXAMPLE 4: std::move IN ACTION
// ============================================

class SimpleResource {
private:
    std::string name;
    int* value;

public:
    SimpleResource(const std::string& n, int v) : name(n), value(new int(v)) {
        std::println("➕ Created: {} (value={})", name, *value);
    }

    ~SimpleResource() {
        std::print("➖ Destroyed: {}", name);
        if (value) {
            std::println(" (value={})", *value);
            delete value;
        } else {
            std::println(" (moved-from, empty)");
        }
    }

    // Move constructor
    SimpleResource(SimpleResource&& other) noexcept
        : name(std::move(other.name)), value(other.value) {
        std::println("🔄 Moved: {}", name);
        other.value = nullptr;
    }

    // Move assignment
    SimpleResource& operator=(SimpleResource&& other) noexcept {
        if (this != &other) {
            delete value;
            name = std::move(other.name);
            value = other.value;
            other.value = nullptr;
            std::println("🔄 Move-assigned: {}", name);
        }
        return *this;
    }

    // Delete copy operations
    SimpleResource(const SimpleResource&) = delete;
    SimpleResource& operator=(const SimpleResource&) = delete;

    int getValue() const { return value ? *value : -1; }
};

void demonstrateStdMove() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: std::move USAGE        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating resources ---");
    SimpleResource r1("Resource1", 100);
    SimpleResource r2("Resource2", 200);

    std::println("");
    std::println("--- Moving with std::move ---");
    std::println("Before move: r1.value = {}", r1.getValue());

    SimpleResource r3 = std::move(r1);  // r1 is now moved-from
    std::println("After move: r1.value = {} (moved-from)", r1.getValue());
    std::println("After move: r3.value = {} (has the data)", r3.getValue());

    std::println("");
    std::println("--- Move assignment ---");
    r3 = std::move(r2);  // Move r2 into r3


    std::println("");
    std::println("--- Destruction phase ---");
}

// ============================================
// EXAMPLE 5: MOVE WITH VECTORS
// ============================================

class BigData {
private:
    std::vector<int> data;
    std::string name;

public:
    BigData(size_t size, const std::string& n) : name(n), data(size) {
        std::println("🏗️  [{}] Created with {} elements", name, size);
        for (size_t i = 0; i < size; i++) {
            data[i] = i;
        }
    }

    // Copy constructor
    BigData(const BigData& other) : data(other.data), name(other.name + "_copy") {
        std::println("📋 [{}] COPY: {} elements", name, data.size());
    }

    // Move constructor
    BigData(BigData&& other) noexcept
        : data(std::move(other.data)), name(std::move(other.name) + "_moved") {
        std::println("🚀 [{}] MOVE: {} elements", name, data.size());
    }

    ~BigData() {
        std::println("💥 [{}] Destroyed ({} elements)", name, data.size());
    }

    size_t size() const { return data.size(); }
};

void demonstrateMoveWithVectors() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 5: MOVE WITH VECTORS      ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Method 1: Copy (expensive) ---");
    {
        std::vector<BigData> vec;
        BigData data(100, "Data1");

        std::println("Pushing lvalue (will copy):");
        vec.push_back(data);  // COPY - data is lvalue
        std::println("Original still valid, size: {}", data.size());
    }

    std::println("");
    std::println("--- Method 2: Move with std::move (efficient) ---");
    {
        std::vector<BigData> vec;
        BigData data(100, "Data2");

        std::println("Pushing with std::move (will move):");
        vec.push_back(std::move(data));  // MOVE
        std::println("Original moved-from, size: {} (empty)", data.size());
    }

    std::println("");
    std::println("--- Method 3: Temporary (automatic move) ---");
    {
        std::vector<BigData> vec;

        std::println("Pushing temporary (automatic move):");
        vec.push_back(BigData(100, "Data3"));  // Temporary - automatically moved
    }

    std::println("");
    std::println("--- Method 4: emplace_back (construct in-place) ---");
    {
        std::vector<BigData> vec;

        std::println("Using emplace_back (best!):");
        vec.emplace_back(100, "Data4");  // Construct directly in vector
        std::println("No move or copy needed!");
    }

}

// ============================================
// EXAMPLE 6: RETURN VALUE OPTIMIZATION (RVO)
// ============================================

class ReturnTest {
private:
    std::string name;

public:
    ReturnTest(const std::string& n) : name(n) {
        std::println("➕ [{}] Constructed", name);
    }

    ReturnTest(const ReturnTest& other) : name(other.name + "_copy") {
        std::println("📋 [{}] Copy constructed", name);
    }

    ReturnTest(ReturnTest&& other) noexcept : name(std::move(other.name) + "_moved") {
        std::println("🚀 [{}] Move constructed", name);
    }

    ~ReturnTest() {
        std::println("➖ [{}] Destroyed", name);
    }
};

// Good: Return by value (RVO or move)
ReturnTest createObject() {
    std::println("   Creating local object...");
    ReturnTest local("Local");
    std::println("   Returning...");
    return local;  // RVO or move (never std::move here!)
}

// Bad: Returning std::move of local (prevents RVO)
ReturnTest createObjectBad() {
    std::println("   Creating local object...");
    ReturnTest local("LocalBad");
    std::println("   Returning with std::move (BAD!)...");
    return std::move(local);  // ❌ Prevents RVO!
}

void demonstrateRVO() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 6: RVO (RETURN VALUE OPT) ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Good: Return by value (RVO or automatic move) ---");
    ReturnTest obj1 = createObject();

    std::println("");
    std::println("--- Bad: Return std::move(local) ---");
    ReturnTest obj2 = createObjectBad();

}

// ============================================
// EXAMPLE 7: MOVE-ONLY TYPES
// ============================================

class MoveOnlyResource {
private:
    std::unique_ptr<int[]> data;
    size_t size;
    std::string name;

public:
    MoveOnlyResource(size_t s, const std::string& n)
        : data(std::make_unique<int[]>(s)), size(s), name(n) {
        std::println("🔒 [{}] Created (move-only)", name);
    }

    // Delete copy operations
    MoveOnlyResource(const MoveOnlyResource&) = delete;
    MoveOnlyResource& operator=(const MoveOnlyResource&) = delete;

    // Move operations (defaulted or custom)
    MoveOnlyResource(MoveOnlyResource&& other) noexcept = default;
    MoveOnlyResource& operator=(MoveOnlyResource&& other) noexcept = default;

    ~MoveOnlyResource() {
        std::println("🔓 [{}] Destroyed", name);
    }

    size_t getSize() const { return size; }
};

void demonstrateMoveOnly() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 7: MOVE-ONLY TYPES        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating move-only resource ---");
    MoveOnlyResource res1(100, "Resource1");


    std::println("");
    std::println("--- Moving (works!) ---");
    MoveOnlyResource res2 = std::move(res1);  // ✅ Move works
    std::println("✓ Move succeeded");


    std::println("");
    std::println("--- Using in containers ---");
    std::vector<MoveOnlyResource> vec;
    vec.push_back(MoveOnlyResource(50, "VecItem1"));  // Temporary - moved
    vec.emplace_back(75, "VecItem2");                  // Construct in-place
}

// ============================================
// EXAMPLE 8: PERFECT FORWARDING
// ============================================

void processValue(int& value) {
    std::println("   Processing lvalue: {}", value);
}

void processValue(int&& value) {
    std::println("   Processing rvalue: {}", value);
}

// Universal reference with perfect forwarding
template<typename T>
void wrapper(T&& arg) {
    std::println("   Wrapper called");
    // std::forward preserves lvalue/rvalue-ness
    processValue(std::forward<T>(arg));
}

void demonstratePerfectForwarding() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 8: PERFECT FORWARDING     ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Forwarding lvalue ---");
    int x = 42;
    wrapper(x);

    std::println("");
    std::println("--- Forwarding rvalue ---");
    wrapper(100);

    std::println("");
    std::println("--- Forwarding moved value ---");
    wrapper(std::move(x));

}

// ============================================
// EXAMPLE 9: RULE OF FIVE
// ============================================

class RuleOfFive {
private:
    int* data;
    std::string name;

public:
    // Constructor
    RuleOfFive(int value, const std::string& n) : name(n) {
        data = new int(value);
        std::println("🏗️  [{}] Constructor (value={})", name, *data);
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

    // 4. Move Constructor
    RuleOfFive(RuleOfFive&& other) noexcept
        : data(other.data), name(other.name + "_moved") {
        std::println("🚀 [{}] Move Constructor", name);
        other.data = nullptr;
    }

    // 5. Move Assignment
    RuleOfFive& operator=(RuleOfFive&& other) noexcept {
        std::println("🚀 [{}] Move Assignment", name);
        if (this != &other) {
            delete data;
            data = other.data;
            name = other.name + "_moved";
            other.data = nullptr;
        }
        return *this;
    }

    int getValue() const { return data ? *data : -1; }
};

void demonstrateRuleOfFive() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 9: RULE OF FIVE           ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Testing all five special members ---");

    RuleOfFive obj1(100, "Obj1");

    std::println("");
    std::println("[Copy Constructor]");
    RuleOfFive obj2 = obj1;

    std::println("");
    std::println("[Move Constructor]");
    RuleOfFive obj3 = std::move(obj1);

    std::println("");
    std::println("[Copy Assignment]");
    RuleOfFive obj4(200, "Obj4");
    obj4 = obj2;

    std::println("");
    std::println("[Move Assignment]");
    RuleOfFive obj5(300, "Obj5");
    obj5 = std::move(obj2);

    std::println("");
    std::println("--- Destruction phase ---");
}

// ============================================
// EXAMPLE 10: RULE OF ZERO (BETTER!)
// ============================================

class RuleOfZero {
private:
    std::unique_ptr<int> data;     // RAII!
    std::string name;              // RAII!
    std::vector<int> numbers;      // RAII!

public:
    RuleOfZero(int value, const std::string& n)
        : data(std::make_unique<int>(value)), name(n), numbers{1, 2, 3} {
        std::println("🎯 [{}] Constructor (using RAII types)", name);
    }

    // No need to define ANY of the special members!
    // Compiler generates correct versions automatically!

    // ~RuleOfZero()                              = default;
    // RuleOfZero(const RuleOfZero&)              = default;
    // RuleOfZero& operator=(const RuleOfZero&)   = default;
    // RuleOfZero(RuleOfZero&&)                   = default;
    // RuleOfZero& operator=(RuleOfZero&&)        = default;

    int getValue() const { return data ? *data : -1; }
};

void demonstrateRuleOfZero() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 10: RULE OF ZERO (BEST!)  ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Testing auto-generated operations ---");

    RuleOfZero obj1(100, "Obj1");

    std::println("");
    std::println("[Auto-generated Move]");
    RuleOfZero obj2 = std::move(obj1);

}

// ============================================
// MAIN DEMONSTRATION
// ============================================

int main() {
    std::println("========================================");
    std::println("MOVE SEMANTICS COMPREHENSIVE GUIDE");
    std::println("========================================");

    demonstrateLvaluesRvalues();
    demonstrateCopyProblem();
    demonstrateMoveConstructor();
    demonstrateStdMove();
    demonstrateMoveWithVectors();
    demonstrateRVO();
    demonstrateMoveOnly();
    demonstratePerfectForwarding();
    demonstrateRuleOfFive();
    demonstrateRuleOfZero();

    return 0;
}
