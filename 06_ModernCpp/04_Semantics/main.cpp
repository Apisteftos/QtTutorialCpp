#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
using namespace std;

// ============================================
// MOVE SEMANTICS
// C++11 Feature for Performance Optimization
// ============================================

// ============================================
// EXAMPLE 1: UNDERSTANDING LVALUES AND RVALUES
// ============================================

void demonstrateLvaluesRvalues() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: LVALUES vs RVALUES    ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    // LVALUES - have names, persistent, can take address
    cout << "--- Lvalues (Left values) ---\n";
    int x = 10;
    int y = 20;
    cout << "int x = 10;           // x is lvalue (has name)\n";
    cout << "int y = 20;           // y is lvalue (has name)\n";
    cout << "int* ptr = &x;        // Can take address of lvalue\n";
    int* ptr = &x;
    cout << "x address: " << &x << "\n";
    cout << "y address: " << &y << "\n";

    // RVALUES - temporaries, no persistent address
    cout << "\n--- Rvalues (Right values / Temporaries) ---\n";
    int z = x + y;  // (x + y) is rvalue
    cout << "int z = x + y;        // (x + y) is rvalue (temporary)\n";
    cout << "// int* bad = &(x + y);  ❌ ERROR: can't take address of rvalue\n";

    cout << "\n--- Function Returns (Rvalues) ---\n";
    auto getValue = []() -> int {
        cout << "   Creating temporary value...\n";
        return 42;
    };

    int result = getValue();  // getValue() returns rvalue
    cout << "int result = getValue();  // Function return is rvalue\n";
    cout << "Result: " << result << "\n";

    cout << "\n--- String Literals ---\n";
    string s1 = "Hello";  // "Hello" is rvalue (temporary)
    cout << "string s1 = \"Hello\";      // String literal is rvalue\n";
    cout << "// string& ref = \"Hello\";  ❌ Can't bind lvalue ref to rvalue\n";
    cout << "// const string& ref = \"Hello\"; ✅ Can bind const lvalue ref to rvalue\n";

    cout << "\n--- Summary ---\n";
    cout << "Lvalues:\n";
    cout << "  • Have names\n";
    cout << "  • Persist beyond expression\n";
    cout << "  • Can appear on left of '='\n";
    cout << "  • Can take address with &\n";
    cout << "\n";
    cout << "Rvalues:\n";
    cout << "  • Temporaries\n";
    cout << "  • Exist only during expression\n";
    cout << "  • Can only appear on right of '='\n";
    cout << "  • Cannot take address\n";
}

// ============================================
// EXAMPLE 2: THE COPY PROBLEM
// ============================================

class HeavyResource {
private:
    int* data;
    size_t size;
    string name;

public:
    // Constructor
    HeavyResource(size_t s, const string& n) : size(s), name(n) {
        data = new int[size];
        cout << "🏗️  [" << name << "] Constructor: Allocated "
             << size << " elements\n";
        // Initialize with some data
        for (size_t i = 0; i < size; i++) {
            data[i] = i;
        }
    }

    // Copy Constructor - EXPENSIVE!
    HeavyResource(const HeavyResource& other)
        : size(other.size), name(other.name + "_copy") {
        cout << "📋 [" << name << "] COPY Constructor: Deep copying "
             << size << " elements (EXPENSIVE! 💰)\n";
        data = new int[size];
        // Deep copy - copies every element
        for (size_t i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }

    // Destructor
    ~HeavyResource() {
        cout << "💥 [" << name << "] Destructor: Freeing " << size << " elements\n";
        delete[] data;
    }

    size_t getSize() const { return size; }
    string getName() const { return name; }
};

void demonstrateCopyProblem() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: THE COPY PROBLEM       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating original resource ---\n";
    HeavyResource original(1000000, "Original");  // 1 million elements

    cout << "\n--- Making a copy (expensive!) ---\n";
    HeavyResource copy = original;  // Deep copy!

    cout << "\n⚠️  Problem: We just copied 1,000,000 integers!\n";
    cout << "   This is expensive and unnecessary if we don't need original anymore.\n";

    cout << "\n--- Destruction phase ---\n";
}

// ============================================
// EXAMPLE 3: MOVE CONSTRUCTOR SOLUTION
// ============================================

class OptimizedResource {
private:
    int* data;
    size_t size;
    string name;

public:
    // Constructor
    OptimizedResource(size_t s, const string& n) : size(s), name(n) {
        data = new int[size];
        cout << "🏗️  [" << name << "] Constructor: Allocated "
             << size << " elements\n";
        for (size_t i = 0; i < size; i++) {
            data[i] = i;
        }
    }

    // Copy Constructor - EXPENSIVE (still needed sometimes)
    OptimizedResource(const OptimizedResource& other)
        : size(other.size), name(other.name + "_copy") {
        cout << "📋 [" << name << "] COPY Constructor: Deep copying "
             << size << " elements (expensive)\n";
        data = new int[size];
        for (size_t i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }

    // Move Constructor - CHEAP! (C++11)
    OptimizedResource(OptimizedResource&& other) noexcept
        : data(other.data), size(other.size), name(other.name + "_moved") {
        cout << "🚀 [" << name << "] MOVE Constructor: Stealing "
             << size << " elements (CHEAP! ⚡)\n";

        // "Steal" the resource from other
        other.data = nullptr;
        other.size = 0;
        other.name = other.name + "_moved-from";
    }

    // Copy Assignment
    OptimizedResource& operator=(const OptimizedResource& other) {
        cout << "📋 [" << name << "] COPY Assignment\n";
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
        cout << "🚀 [" << name << "] MOVE Assignment\n";
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
            cout << "💥 [" << name << "] Destructor: Freeing " << size << " elements\n";
            delete[] data;
        } else {
            cout << "💥 [" << name << "] Destructor: Empty (moved-from state)\n";
        }
    }

    size_t getSize() const { return size; }
    string getName() const { return name; }
};

void demonstrateMoveConstructor() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: MOVE CONSTRUCTOR       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating original ---\n";
    OptimizedResource original(1000000, "Original");

    cout << "\n--- Copy (still expensive when needed) ---\n";
    OptimizedResource copy = original;  // Copy constructor

    cout << "\n--- Move (cheap! using std::move) ---\n";
    OptimizedResource moved = std::move(original);  // Move constructor
    // std::move casts 'original' to rvalue reference

    cout << "\n📊 After move:\n";
    cout << "   original.size: " << original.getSize() << " (moved-from)\n";
    cout << "   moved.size: " << moved.getSize() << " (has the data)\n";

    cout << "\n✨ Move is just pointer swapping - O(1) instead of O(n)!\n";

    cout << "\n--- Destruction phase ---\n";
}

// ============================================
// EXAMPLE 4: std::move IN ACTION
// ============================================

class SimpleResource {
private:
    string name;
    int* value;

public:
    SimpleResource(const string& n, int v) : name(n), value(new int(v)) {
        cout << "➕ Created: " << name << " (value=" << *value << ")\n";
    }

    ~SimpleResource() {
        cout << "➖ Destroyed: " << name;
        if (value) {
            cout << " (value=" << *value << ")\n";
            delete value;
        } else {
            cout << " (moved-from, empty)\n";
        }
    }

    // Move constructor
    SimpleResource(SimpleResource&& other) noexcept
        : name(std::move(other.name)), value(other.value) {
        cout << "🔄 Moved: " << name << "\n";
        other.value = nullptr;
    }

    // Move assignment
    SimpleResource& operator=(SimpleResource&& other) noexcept {
        if (this != &other) {
            delete value;
            name = std::move(other.name);
            value = other.value;
            other.value = nullptr;
            cout << "🔄 Move-assigned: " << name << "\n";
        }
        return *this;
    }

    // Delete copy operations
    SimpleResource(const SimpleResource&) = delete;
    SimpleResource& operator=(const SimpleResource&) = delete;

    int getValue() const { return value ? *value : -1; }
};

void demonstrateStdMove() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: std::move USAGE        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- What is std::move? ---\n";
    cout << "std::move() doesn't actually move anything!\n";
    cout << "It just casts an lvalue to an rvalue reference.\n";
    cout << "This allows the move constructor/assignment to be called.\n";

    cout << "\n--- Creating resources ---\n";
    SimpleResource r1("Resource1", 100);
    SimpleResource r2("Resource2", 200);

    cout << "\n--- Moving with std::move ---\n";
    cout << "Before move: r1.value = " << r1.getValue() << "\n";

    SimpleResource r3 = std::move(r1);  // r1 is now moved-from
    cout << "After move: r1.value = " << r1.getValue() << " (moved-from)\n";
    cout << "After move: r3.value = " << r3.getValue() << " (has the data)\n";

    cout << "\n--- Move assignment ---\n";
    r3 = std::move(r2);  // Move r2 into r3

    cout << "\n⚠️  Important: Don't use moved-from objects!\n";
    cout << "   r1 and r2 are now in 'moved-from' state.\n";
    cout << "   Only safe operations: assign new value or destroy.\n";

    cout << "\n--- Destruction phase ---\n";
}

// ============================================
// EXAMPLE 5: MOVE WITH VECTORS
// ============================================

class BigData {
private:
    vector<int> data;
    string name;

public:
    BigData(size_t size, const string& n) : name(n), data(size) {
        cout << "🏗️  [" << name << "] Created with " << size << " elements\n";
        for (size_t i = 0; i < size; i++) {
            data[i] = i;
        }
    }

    // Copy constructor
    BigData(const BigData& other) : data(other.data), name(other.name + "_copy") {
        cout << "📋 [" << name << "] COPY: " << data.size() << " elements\n";
    }

    // Move constructor
    BigData(BigData&& other) noexcept
        : data(std::move(other.data)), name(std::move(other.name) + "_moved") {
        cout << "🚀 [" << name << "] MOVE: " << data.size() << " elements\n";
    }

    ~BigData() {
        cout << "💥 [" << name << "] Destroyed (" << data.size() << " elements)\n";
    }

    size_t size() const { return data.size(); }
};

void demonstrateMoveWithVectors() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: MOVE WITH VECTORS      ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Method 1: Copy (expensive) ---\n";
    {
        vector<BigData> vec;
        BigData data(100, "Data1");

        cout << "Pushing lvalue (will copy):\n";
        vec.push_back(data);  // COPY - data is lvalue
        cout << "Original still valid, size: " << data.size() << "\n";
    }

    cout << "\n--- Method 2: Move with std::move (efficient) ---\n";
    {
        vector<BigData> vec;
        BigData data(100, "Data2");

        cout << "Pushing with std::move (will move):\n";
        vec.push_back(std::move(data));  // MOVE
        cout << "Original moved-from, size: " << data.size() << " (empty)\n";
    }

    cout << "\n--- Method 3: Temporary (automatic move) ---\n";
    {
        vector<BigData> vec;

        cout << "Pushing temporary (automatic move):\n";
        vec.push_back(BigData(100, "Data3"));  // Temporary - automatically moved
    }

    cout << "\n--- Method 4: emplace_back (construct in-place) ---\n";
    {
        vector<BigData> vec;

        cout << "Using emplace_back (best!):\n";
        vec.emplace_back(100, "Data4");  // Construct directly in vector
        cout << "No move or copy needed!\n";
    }

    cout << "\n✨ Performance Ranking:\n";
    cout << "   1. emplace_back()      - Construct in-place (best)\n";
    cout << "   2. push_back(temporary) - Automatic move\n";
    cout << "   3. push_back(std::move) - Explicit move\n";
    cout << "   4. push_back(lvalue)    - Copy (worst)\n";
}

// ============================================
// EXAMPLE 6: RETURN VALUE OPTIMIZATION (RVO)
// ============================================

class ReturnTest {
private:
    string name;

public:
    ReturnTest(const string& n) : name(n) {
        cout << "➕ [" << name << "] Constructed\n";
    }

    ReturnTest(const ReturnTest& other) : name(other.name + "_copy") {
        cout << "📋 [" << name << "] Copy constructed\n";
    }

    ReturnTest(ReturnTest&& other) noexcept : name(std::move(other.name) + "_moved") {
        cout << "🚀 [" << name << "] Move constructed\n";
    }

    ~ReturnTest() {
        cout << "➖ [" << name << "] Destroyed\n";
    }
};

// Good: Return by value (RVO or move)
ReturnTest createObject() {
    cout << "   Creating local object...\n";
    ReturnTest local("Local");
    cout << "   Returning...\n";
    return local;  // RVO or move (never std::move here!)
}

// Bad: Returning std::move of local (prevents RVO)
ReturnTest createObjectBad() {
    cout << "   Creating local object...\n";
    ReturnTest local("LocalBad");
    cout << "   Returning with std::move (BAD!)...\n";
    return std::move(local);  // ❌ Prevents RVO!
}

void demonstrateRVO() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: RVO (RETURN VALUE OPT) ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Good: Return by value (RVO or automatic move) ---\n";
    ReturnTest obj1 = createObject();
    cout << "✓ Compiler optimizes away copy/move (RVO)\n";

    cout << "\n--- Bad: Return std::move(local) ---\n";
    ReturnTest obj2 = createObjectBad();
    cout << "❌ std::move prevents RVO optimization!\n";

    cout << "\n💡 Rule: Never use std::move on return of local variable!\n";
    cout << "   return local;              ✅ Correct (RVO or auto-move)\n";
    cout << "   return std::move(local);   ❌ Wrong (prevents RVO)\n";
}

// ============================================
// EXAMPLE 7: MOVE-ONLY TYPES
// ============================================

class MoveOnlyResource {
private:
    unique_ptr<int[]> data;
    size_t size;
    string name;

public:
    MoveOnlyResource(size_t s, const string& n)
        : data(make_unique<int[]>(s)), size(s), name(n) {
        cout << "🔒 [" << name << "] Created (move-only)\n";
    }

    // Delete copy operations
    MoveOnlyResource(const MoveOnlyResource&) = delete;
    MoveOnlyResource& operator=(const MoveOnlyResource&) = delete;

    // Move operations (defaulted or custom)
    MoveOnlyResource(MoveOnlyResource&& other) noexcept = default;
    MoveOnlyResource& operator=(MoveOnlyResource&& other) noexcept = default;

    ~MoveOnlyResource() {
        cout << "🔓 [" << name << "] Destroyed\n";
    }

    size_t getSize() const { return size; }
};

void demonstrateMoveOnly() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 7: MOVE-ONLY TYPES        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating move-only resource ---\n";
    MoveOnlyResource res1(100, "Resource1");

    cout << "\n--- Attempting copy (would fail) ---\n";
    cout << "// MoveOnlyResource res2 = res1;  ❌ Error: copy deleted\n";

    cout << "\n--- Moving (works!) ---\n";
    MoveOnlyResource res2 = std::move(res1);  // ✅ Move works
    cout << "✓ Move succeeded\n";

    cout << "\n--- Move-only types in STL ---\n";
    cout << "Examples of move-only types:\n";
    cout << "  • unique_ptr<T>     - Exclusive ownership\n";
    cout << "  • thread            - Thread handle\n";
    cout << "  • ifstream/ofstream - File streams\n";
    cout << "  • mutex             - Mutex locks\n";
    cout << "  • atomic<T>         - Atomic types\n";

    cout << "\n--- Using in containers ---\n";
    vector<MoveOnlyResource> vec;
    vec.push_back(MoveOnlyResource(50, "VecItem1"));  // Temporary - moved
    vec.emplace_back(75, "VecItem2");                  // Construct in-place
    cout << "✓ Move-only types can be stored in containers\n";
}

// ============================================
// EXAMPLE 8: PERFECT FORWARDING
// ============================================

void processValue(int& value) {
    cout << "   Processing lvalue: " << value << "\n";
}

void processValue(int&& value) {
    cout << "   Processing rvalue: " << value << "\n";
}

// Universal reference with perfect forwarding
template<typename T>
void wrapper(T&& arg) {
    cout << "   Wrapper called\n";
    // std::forward preserves lvalue/rvalue-ness
    processValue(std::forward<T>(arg));
}

void demonstratePerfectForwarding() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 8: PERFECT FORWARDING     ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- What is T&&? ---\n";
    cout << "T&& is a 'forwarding reference' (or 'universal reference')\n";
    cout << "It can bind to BOTH lvalues and rvalues\n";

    cout << "\n--- Forwarding lvalue ---\n";
    int x = 42;
    cout << "int x = 42;\n";
    cout << "wrapper(x);  // x is lvalue\n";
    wrapper(x);

    cout << "\n--- Forwarding rvalue ---\n";
    cout << "wrapper(100);  // 100 is rvalue\n";
    wrapper(100);

    cout << "\n--- Forwarding moved value ---\n";
    cout << "wrapper(std::move(x));  // x cast to rvalue\n";
    wrapper(std::move(x));

    cout << "\n💡 std::forward preserves the value category!\n";
    cout << "   If arg is lvalue → forwards as lvalue\n";
    cout << "   If arg is rvalue → forwards as rvalue\n";
}

// ============================================
// EXAMPLE 9: RULE OF FIVE
// ============================================

class RuleOfFive {
private:
    int* data;
    string name;

public:
    // Constructor
    RuleOfFive(int value, const string& n) : name(n) {
        data = new int(value);
        cout << "🏗️  [" << name << "] Constructor (value=" << *data << ")\n";
    }

    // 1. Destructor
    ~RuleOfFive() {
        cout << "💥 [" << name << "] Destructor\n";
        delete data;
    }

    // 2. Copy Constructor
    RuleOfFive(const RuleOfFive& other) : name(other.name + "_copy") {
        data = new int(*other.data);
        cout << "📋 [" << name << "] Copy Constructor\n";
    }

    // 3. Copy Assignment
    RuleOfFive& operator=(const RuleOfFive& other) {
        cout << "📋 [" << name << "] Copy Assignment\n";
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
        cout << "🚀 [" << name << "] Move Constructor\n";
        other.data = nullptr;
    }

    // 5. Move Assignment
    RuleOfFive& operator=(RuleOfFive&& other) noexcept {
        cout << "🚀 [" << name << "] Move Assignment\n";
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
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 9: RULE OF FIVE           ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- If you define ANY of these 5, define ALL: ---\n";
    cout << "1. Destructor\n";
    cout << "2. Copy Constructor\n";
    cout << "3. Copy Assignment\n";
    cout << "4. Move Constructor\n";
    cout << "5. Move Assignment\n";

    cout << "\n--- Testing all five special members ---\n";

    RuleOfFive obj1(100, "Obj1");

    cout << "\n[Copy Constructor]\n";
    RuleOfFive obj2 = obj1;

    cout << "\n[Move Constructor]\n";
    RuleOfFive obj3 = std::move(obj1);

    cout << "\n[Copy Assignment]\n";
    RuleOfFive obj4(200, "Obj4");
    obj4 = obj2;

    cout << "\n[Move Assignment]\n";
    RuleOfFive obj5(300, "Obj5");
    obj5 = std::move(obj2);

    cout << "\n--- Destruction phase ---\n";
}

// ============================================
// EXAMPLE 10: RULE OF ZERO (BETTER!)
// ============================================

class RuleOfZero {
private:
    unique_ptr<int> data;     // RAII!
    string name;              // RAII!
    vector<int> numbers;      // RAII!

public:
    RuleOfZero(int value, const string& n)
        : data(make_unique<int>(value)), name(n), numbers{1, 2, 3} {
        cout << "🎯 [" << name << "] Constructor (using RAII types)\n";
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
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 10: RULE OF ZERO (BEST!)  ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Rule of Zero: Don't define ANY special members! ---\n";
    cout << "Instead: Use RAII types (smart pointers, containers)\n";
    cout << "Compiler auto-generates correct move/copy operations!\n";

    cout << "\n--- Testing auto-generated operations ---\n";

    RuleOfZero obj1(100, "Obj1");

    cout << "\n[Auto-generated Move]\n";
    RuleOfZero obj2 = std::move(obj1);
    cout << "✓ Moved successfully (compiler-generated)\n";

    cout << "\n💡 Rule of Zero is BETTER than Rule of Five!\n";
    cout << "   • Less code to write\n";
    cout << "   • Less code to maintain\n";
    cout << "   • Compiler generates optimal code\n";
    cout << "   • Harder to make mistakes\n";
}

// ============================================
// MAIN DEMONSTRATION
// ============================================

int main() {
    cout << "========================================\n";
    cout << "MOVE SEMANTICS COMPREHENSIVE GUIDE\n";
    cout << "========================================\n";

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

    // ============================================
    // Comprehensive Summary
    // ============================================
    cout << "\n========================================\n";
    cout << "WHAT IS MOVE SEMANTICS?\n";
    cout << "========================================\n";
    cout << "Move Semantics (C++11):\n";
    cout << "  • Optimization technique\n";
    cout << "  • Transfer ownership instead of copying\n";
    cout << "  • \"Steal\" resources from temporary objects\n";
    cout << "  • Massive performance improvement\n";
    cout << "\n";
    cout << "Goal: Avoid expensive deep copies\n";
    cout << "  Copy:  obj2 = obj1;              // Deep copy (expensive)\n";
    cout << "  Move:  obj2 = std::move(obj1);   // Transfer ownership (cheap)\n";

    cout << "\n========================================\n";
    cout << "CORE CONCEPTS\n";
    cout << "========================================\n";
    cout << "Lvalues:\n";
    cout << "  • Named objects\n";
    cout << "  • Persist beyond expression\n";
    cout << "  • Can take address (&x)\n";
    cout << "  • Example: int x = 5;\n";
    cout << "\n";
    cout << "Rvalues:\n";
    cout << "  • Temporaries\n";
    cout << "  • Exist only during expression\n";
    cout << "  • Cannot take address\n";
    cout << "  • Example: int x = 5 + 3;  // (5+3) is rvalue\n";
    cout << "\n";
    cout << "Rvalue Reference (&&):\n";
    cout << "  • Binds to rvalues (temporaries)\n";
    cout << "  • Enables move semantics\n";
    cout << "  • Syntax: Type&& name\n";

    cout << "\n========================================\n";
    cout << "MOVE OPERATIONS\n";
    cout << "========================================\n";
    cout << "Move Constructor:\n";
    cout << "  MyClass(MyClass&& other) noexcept {\n";
    cout << "      // Steal resources from 'other'\n";
    cout << "      data = other.data;\n";
    cout << "      other.data = nullptr;\n";
    cout << "  }\n";
    cout << "\n";
    cout << "Move Assignment:\n";
    cout << "  MyClass& operator=(MyClass&& other) noexcept {\n";
    cout << "      if (this != &other) {\n";
    cout << "          delete data;  // Free current resources\n";
    cout << "          data = other.data;  // Steal resources\n";
    cout << "          other.data = nullptr;\n";
    cout << "      }\n";
    cout << "      return *this;\n";
    cout << "  }\n";

    cout << "\n========================================\n";
    cout << "std::move\n";
    cout << "========================================\n";
    cout << "What it does:\n";
    cout << "  • Casts lvalue to rvalue reference\n";
    cout << "  • Doesn't actually move anything!\n";
    cout << "  • Enables move constructor/assignment to be called\n";
    cout << "\n";
    cout << "Usage:\n";
    cout << "  MyClass obj1;                    // lvalue\n";
    cout << "  MyClass obj2 = std::move(obj1);  // Cast to rvalue\n";
    cout << "  // obj1 is now in 'moved-from' state\n";
    cout << "\n";
    cout << "After std::move:\n";
    cout << "  • Original object is in valid but unspecified state\n";
    cout << "  • Can assign new value or destroy\n";
    cout << "  • Don't use it otherwise!\n";

    cout << "\n========================================\n";
    cout << "WHEN MOVES HAPPEN AUTOMATICALLY\n";
    cout << "========================================\n";
    cout << "1. Returning local variables:\n";
    cout << "   MyClass func() {\n";
    cout << "       MyClass local;\n";
    cout << "       return local;  // Automatically moved (or RVO)\n";
    cout << "   }\n";
    cout << "\n";
    cout << "2. Passing temporaries:\n";
    cout << "   vec.push_back(MyClass());  // Temporary - auto moved\n";
    cout << "\n";
    cout << "3. Initializing from rvalue:\n";
    cout << "   MyClass obj = createObject();  // Auto moved\n";

    cout << "\n========================================\n";
    cout << "WHEN TO USE std::move\n";
    cout << "========================================\n";
    cout << "Use std::move when:\n";
    cout << "  ✓ You're done with an object\n";
    cout << "  ✓ Transferring ownership explicitly\n";
    cout << "  ✓ Inserting into containers when copy not needed\n";
    cout << "  ✓ Implementing move operations\n";
    cout << "\n";
    cout << "Don't use std::move:\n";
    cout << "  ❌ On const objects (won't move, will copy)\n";
    cout << "  ❌ On return of local variable (prevents RVO)\n";
    cout << "  ❌ When you still need the object\n";
    cout << "  ❌ On temporaries (already rvalues)\n";

    cout << "\n========================================\n";
    cout << "PERFORMANCE COMPARISON\n";
    cout << "========================================\n";
    cout << "Copy (Deep Copy):\n";
    cout << "  • Allocate new memory\n";
    cout << "  • Copy every element\n";
    cout << "  • O(n) complexity\n";
    cout << "  • Example: Copying 1M integers = 1M copies\n";
    cout << "\n";
    cout << "Move (Transfer Ownership):\n";
    cout << "  • Swap/steal pointers\n";
    cout << "  • No element copying\n";
    cout << "  • O(1) complexity\n";
    cout << "  • Example: Moving 1M integers = 3 pointer ops\n";
    cout << "\n";
    cout << "Speed Difference:\n";
    cout << "  Copy:  ~100ms  (for 1M elements)\n";
    cout << "  Move:  ~0.001ms (1000x faster!)\n";

    cout << "\n========================================\n";
    cout << "RETURN VALUE OPTIMIZATION (RVO)\n";
    cout << "========================================\n";
    cout << "RVO = Compiler optimization that eliminates copy/move\n";
    cout << "\n";
    cout << "Correct:\n";
    cout << "  MyClass func() {\n";
    cout << "      MyClass local;\n";
    cout << "      return local;  ✅ Enables RVO\n";
    cout << "  }\n";
    cout << "\n";
    cout << "Wrong:\n";
    cout << "  MyClass func() {\n";
    cout << "      MyClass local;\n";
    cout << "      return std::move(local);  ❌ Prevents RVO!\n";
    cout << "  }\n";
    cout << "\n";
    cout << "Rule: Never std::move return values!\n";

    cout << "\n========================================\n";
    cout << "MOVE-ONLY TYPES\n";
    cout << "========================================\n";
    cout << "Some types cannot be copied, only moved:\n";
    cout << "  • unique_ptr<T>     - Exclusive ownership\n";
    cout << "  • thread            - Thread handles\n";
    cout << "  • ifstream/ofstream - File streams\n";
    cout << "  • mutex             - Mutex locks\n";
    cout << "  • atomic<T>         - Atomic operations\n";
    cout << "\n";
    cout << "Creating move-only types:\n";
    cout << "  class MoveOnly {\n";
    cout << "      // Delete copy operations\n";
    cout << "      MoveOnly(const MoveOnly&) = delete;\n";
    cout << "      MoveOnly& operator=(const MoveOnly&) = delete;\n";
    cout << "      \n";
    cout << "      // Define/default move operations\n";
    cout << "      MoveOnly(MoveOnly&&) = default;\n";
    cout << "      MoveOnly& operator=(MoveOnly&&) = default;\n";
    cout << "  };\n";

    cout << "\n========================================\n";
    cout << "PERFECT FORWARDING\n";
    cout << "========================================\n";
    cout << "Universal/Forwarding Reference: T&&\n";
    cout << "  • In template context only\n";
    cout << "  • Binds to both lvalues and rvalues\n";
    cout << "  • Used with std::forward\n";
    cout << "\n";
    cout << "Example:\n";
    cout << "  template<typename T>\n";
    cout << "  void wrapper(T&& arg) {  // Universal reference\n";
    cout << "      func(std::forward<T>(arg));  // Perfect forwarding\n";
    cout << "  }\n";
    cout << "\n";
    cout << "std::forward:\n";
    cout << "  • Preserves lvalue/rvalue-ness\n";
    cout << "  • lvalue → forwards as lvalue\n";
    cout << "  • rvalue → forwards as rvalue\n";

    cout << "\n========================================\n";
    cout << "RULE OF FIVE\n";
    cout << "========================================\n";
    cout << "If you define ANY of these, define ALL FIVE:\n";
    cout << "  1. Destructor              ~MyClass()\n";
    cout << "  2. Copy Constructor        MyClass(const MyClass&)\n";
    cout << "  3. Copy Assignment         operator=(const MyClass&)\n";
    cout << "  4. Move Constructor        MyClass(MyClass&&)\n";
    cout << "  5. Move Assignment         operator=(MyClass&&)\n";
    cout << "\n";
    cout << "Or use = default / = delete\n";
    cout << "\n";
    cout << "When to use:\n";
    cout << "  • Managing raw resources (new/delete)\n";
    cout << "  • Custom memory management\n";
    cout << "  • Wrapping C APIs\n";

    cout << "\n========================================\n";
    cout << "RULE OF ZERO (PREFERRED!)\n";
    cout << "========================================\n";
    cout << "Don't define ANY special members!\n";
    cout << "\n";
    cout << "Instead, use RAII types:\n";
    cout << "  class MyClass {\n";
    cout << "      unique_ptr<int> data;    // Instead of int*\n";
    cout << "      vector<int> numbers;     // Instead of int*\n";
    cout << "      string name;             // Instead of char*\n";
    cout << "      \n";
    cout << "      // No need to define any special members!\n";
    cout << "      // Compiler generates correct versions!\n";
    cout << "  };\n";
    cout << "\n";
    cout << "Benefits:\n";
    cout << "  ✅ Less code\n";
    cout << "  ✅ Less bugs\n";
    cout << "  ✅ Compiler-optimized\n";
    cout << "  ✅ Automatically correct\n";

    cout << "\n========================================\n";
    cout << "COMMON MISTAKES\n";
    cout << "========================================\n";
    cout << "1. Using moved-from objects:\n";
    cout << "   ❌ MyClass obj1;\n";
    cout << "      MyClass obj2 = std::move(obj1);\n";
    cout << "      obj1.doSomething();  // Undefined behavior!\n";
    cout << "\n";
    cout << "2. Moving const objects:\n";
    cout << "   ❌ const MyClass obj1;\n";
    cout << "      MyClass obj2 = std::move(obj1);  // Copies, doesn't move!\n";
    cout << "\n";
    cout << "3. std::move in return:\n";
    cout << "   ❌ return std::move(local);  // Prevents RVO\n";
    cout << "   ✅ return local;             // Correct\n";
    cout << "\n";
    cout << "4. Forgetting noexcept:\n";
    cout << "   ❌ MyClass(MyClass&& other)  // Might not be used!\n";
    cout << "   ✅ MyClass(MyClass&& other) noexcept  // Preferred\n";
    cout << "\n";
    cout << "5. Moving temporaries:\n";
    cout << "   ❌ vec.push_back(std::move(MyClass()));  // Redundant\n";
    cout << "   ✅ vec.push_back(MyClass());             // Already moved\n";

    cout << "\n========================================\n";
    cout << "BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "1. Follow Rule of Zero when possible\n";
    cout << "2. Mark move operations noexcept\n";
    cout << "3. Leave moved-from objects in valid state\n";
    cout << "4. Don't std::move const objects\n";
    cout << "5. Don't std::move return values\n";
    cout << "6. Use std::move explicitly for lvalues\n";
    cout << "7. Document move-only types clearly\n";
    cout << "8. Test moved-from state is valid\n";
    cout << "9. Prefer emplace_back over push_back\n";
    cout << "10. Use std::forward for perfect forwarding\n";

    cout << "\n========================================\n";
    cout << "SYNTAX REFERENCE\n";
    cout << "========================================\n";
    cout << "Rvalue Reference:\n";
    cout << "  MyClass&& rref = MyClass();  // Binds to temporary\n";
    cout << "\n";
    cout << "std::move:\n";
    cout << "  MyClass obj1;\n";
    cout << "  MyClass&& rref = std::move(obj1);  // Cast to rvalue\n";
    cout << "\n";
    cout << "Move Constructor:\n";
    cout << "  MyClass(MyClass&& other) noexcept;\n";
    cout << "\n";
    cout << "Move Assignment:\n";
    cout << "  MyClass& operator=(MyClass&& other) noexcept;\n";
    cout << "\n";
    cout << "Universal Reference:\n";
    cout << "  template<typename T>\n";
    cout << "  void func(T&& arg);  // Universal reference\n";
    cout << "\n";
    cout << "Perfect Forwarding:\n";
    cout << "  std::forward<T>(arg);\n";

    cout << "\n========================================\n";
    cout << "BENEFITS\n";
    cout << "========================================\n";
    cout << "✅ Massive Performance Gain\n";
    cout << "   • Avoid expensive copies\n";
    cout << "   • O(1) instead of O(n)\n";
    cout << "   • Can be 100-1000x faster\n";
    cout << "\n";
    cout << "✅ Enables Move-Only Types\n";
    cout << "   • unique_ptr for exclusive ownership\n";
    cout << "   • Resources that can't be copied\n";
    cout << "\n";
    cout << "✅ Better Resource Management\n";
    cout << "   • Clear ownership transfer\n";
    cout << "   • No accidental copies\n";
    cout << "\n";
    cout << "✅ More Expressive Code\n";
    cout << "   • Intent is clear (move vs copy)\n";
    cout << "   • Ownership semantics explicit\n";

    cout << "\n========================================\n";
    cout << "CONTAINER OPERATIONS\n";
    cout << "========================================\n";
    cout << "vector operations with move:\n";
    cout << "  vector<MyClass> vec;\n";
    cout << "  \n";
    cout << "  MyClass obj;\n";
    cout << "  vec.push_back(obj);              // Copy\n";
    cout << "  vec.push_back(std::move(obj));   // Move\n";
    cout << "  vec.push_back(MyClass());        // Move (temporary)\n";
    cout << "  vec.emplace_back(args...);       // Construct in-place (best!)\n";
    cout << "\n";
    cout << "Performance ranking:\n";
    cout << "  1. emplace_back()           - Best (no move/copy)\n";
    cout << "  2. push_back(temporary)     - Good (auto move)\n";
    cout << "  3. push_back(std::move())   - Good (explicit move)\n";
    cout << "  4. push_back(lvalue)        - Worst (copy)\n";

    cout << "\n========================================\n";
    cout << "WHY noexcept MATTERS\n";
    cout << "========================================\n";
    cout << "Move operations should be noexcept:\n";
    cout << "  MyClass(MyClass&&) noexcept;  ✅\n";
    cout << "  MyClass(MyClass&&);           ⚠️ Might not be used!\n";
    cout << "\n";
    cout << "Why:\n";
    cout << "  • STL containers need strong exception guarantee\n";
    cout << "  • If move can throw, they use copy instead\n";
    cout << "  • noexcept enables optimizations\n";
    cout << "\n";
    cout << "Example:\n";
    cout << "  vector::resize() will:\n";
    cout << "    - Use move if noexcept\n";
    cout << "    - Use copy if not noexcept\n";

    cout << "\n========================================\n";
    cout << "REAL-WORLD IMPACT\n";
    cout << "========================================\n";
    cout << "Before Move Semantics (C++03):\n";
    cout << "  • Expensive string/vector returns\n";
    cout << "  • Awkward workarounds (swap trick)\n";
    cout << "  • Performance sacrifices\n";
    cout << "\n";
    cout << "After Move Semantics (C++11+):\n";
    cout << "  • Zero-cost string/vector returns\n";
    cout << "  • Natural, efficient code\n";
    cout << "  • 10-100x speedup in many cases\n";
    cout << "\n";
    cout << "Example:\n";
    cout << "  vector<string> readFile() {\n";
    cout << "      vector<string> lines;\n";
    cout << "      // ... read millions of lines ...\n";
    cout << "      return lines;  // C++03: Copy millions!\n";
    cout << "                     // C++11: Move (instant!)\n";
    cout << "  }\n";

    cout << "\n========================================\n";
    cout << "KEY TAKEAWAYS\n";
    cout << "========================================\n";
    cout << "1. Move = Transfer ownership (cheap)\n";
    cout << "2. Copy = Duplicate resources (expensive)\n";
    cout << "3. std::move = Cast to rvalue reference\n";
    cout << "4. Moved-from = Valid but unspecified state\n";
    cout << "5. noexcept = Required for optimal performance\n";
    cout << "6. RVO = Compiler optimization (better than move)\n";
    cout << "7. Rule of Zero = Use RAII types (best!)\n";
    cout << "8. Rule of Five = If custom, define all five\n";

    cout << "\n========================================\n";
    cout << "COMPARISON TABLE\n";
    cout << "========================================\n";
    cout << "┌──────────────┬──────────┬──────────┬────────────┐\n";
    cout << "│ Operation    │ Copy     │ Move     │ Emplace    │\n";
    cout << "├──────────────┼──────────┼──────────┼────────────┤\n";
    cout << "│ Complexity   │ O(n)     │ O(1)     │ O(1)       │\n";
    cout << "│ Allocations  │ New      │ Steal    │ In-place   │\n";
    cout << "│ Performance  │ Slow     │ Fast     │ Fastest    │\n";
    cout << "│ Use Case     │ Need     │ Done     │ Create     │\n";
    cout << "│              │ both     │ with src │ new object │\n";
    cout << "└──────────────┴──────────┴──────────┴────────────┘\n";

    cout << "\n========================================\n";
    cout << "THE MOVE REVOLUTION\n";
    cout << "========================================\n";
    cout << "Move Semantics fundamentally changed C++:\n";
    cout << "\n";
    cout << "Before (C++03):\n";
    cout << "  • Returning vectors was slow\n";
    cout << "  • Pass-by-reference everywhere\n";
    cout << "  • Complex ownership patterns\n";
    cout << "\n";
    cout << "After (C++11+):\n";
    cout << "  • Return by value is fast\n";
    cout << "  • Natural, modern code\n";
    cout << "  • Clear ownership with unique_ptr\n";
    cout << "\n";
    cout << "Move Semantics enabled:\n";
    cout << "  • unique_ptr (exclusive ownership)\n";
    cout << "  • Move-only types\n";
    cout << "  • Efficient standard library\n";
    cout << "  • Modern C++ style\n";

    return 0;
}
