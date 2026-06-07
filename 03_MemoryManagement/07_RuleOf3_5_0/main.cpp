#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
using namespace std;

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
// EXAMPLE 1: THE PROBLEM - NO RULE OF THREE
// ============================================

class BadResource {
private:
    int* data;
    string name;

public:
    BadResource(int value, const string& n) : name(n) {
        data = new int(value);
        cout << "✅ [" << name << "] Constructor: allocated data = " << *data << "\n";
    }

    ~BadResource() {
        cout << "💥 [" << name << "] Destructor: deleting data\n";
        delete data;
    }

    // Problem: Using compiler-generated copy constructor and copy assignment!
    // They do SHALLOW copies - both objects share the same pointer!

    void display() const {
        cout << "   [" << name << "] data = " << (data ? to_string(*data) : "nullptr") << "\n";
    }
};

void demonstrateProblem() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: THE PROBLEM            ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "Creating obj1...\n";
    BadResource obj1(42, "Obj1");
    obj1.display();

    cout << "\nCopying obj1 to obj2 (shallow copy!)...\n";
    BadResource obj2 = obj1;  // DANGER! Both now share the same pointer!
    obj2.display();

    cout << "\n⚠️  Both objects point to the same memory!\n";
    cout << "⚠️  When they go out of scope, DOUBLE DELETE crash!\n";
    cout << "\n--- Destruction phase (watch for crash!) ---\n";

    // When obj2 and obj1 are destroyed:
    // obj2 destructor: delete data; ✅ OK
    // obj1 destructor: delete data; 💥 CRASH! Already deleted!
}

// ============================================
// EXAMPLE 2: RULE OF THREE - THE SOLUTION
// ============================================

class GoodResource {
private:
    int* data;
    string name;

public:
    // Constructor
    GoodResource(int value, const string& n) : name(n) {
        data = new int(value);
        cout << "✅ [" << name << "] Constructor: data = " << *data << "\n";
    }

    // 1. Destructor
    ~GoodResource() {
        cout << "💥 [" << name << "] Destructor\n";
        delete data;
    }

    // 2. Copy Constructor (DEEP COPY!)
    GoodResource(const GoodResource& other) : name(other.name + "_copy") {
        data = new int(*other.data);  // Allocate new memory and copy value
        cout << "📋 [" << name << "] Copy Constructor (deep copy)\n";
    }

    // 3. Copy Assignment Operator (DEEP COPY!)
    GoodResource& operator=(const GoodResource& other) {
        cout << "📋 [" << name << "] Copy Assignment\n";

        // Check for self-assignment
        if (this == &other) {
            cout << "   Self-assignment detected, skipping\n";
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
        cout << "   [" << name << "] data = " << *data << "\n";
    }
};

void demonstrateRuleOfThree() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: RULE OF THREE          ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "RULE OF THREE (C++98):\n";
    cout << "If you need ONE, you need ALL THREE:\n";
    cout << "  1. Destructor\n";
    cout << "  2. Copy Constructor\n";
    cout << "  3. Copy Assignment Operator\n";
    cout << "\n";

    cout << "--- Creating obj1 ---\n";
    GoodResource obj1(100, "Obj1");
    obj1.display();

    cout << "\n--- Copy Construction (obj2 = obj1) ---\n";
    GoodResource obj2 = obj1;  // Calls copy constructor
    obj2.display();

    cout << "\n--- Copy Assignment (obj3 = obj1) ---\n";
    GoodResource obj3(200, "Obj3");
    obj3 = obj1;  // Calls copy assignment operator
    obj3.display();

    cout << "\n--- Self-Assignment Test (obj1 = obj1) ---\n";
    obj1 = obj1;  // Should detect and skip

    cout << "\n✅ Each object has its own copy of data\n";
    cout << "✅ No double-delete problem!\n";

    cout << "\n--- Destruction phase ---\n";
}

// ============================================
// EXAMPLE 3: WHY SELF-ASSIGNMENT CHECK MATTERS
// ============================================

class SelfAssignmentDemo {
private:
    int* data;

public:
    SelfAssignmentDemo(int value) {
        data = new int(value);
    }

    ~SelfAssignmentDemo() {
        delete data;
    }

    // BAD: No self-assignment check
    SelfAssignmentDemo& operator=(const SelfAssignmentDemo& other) {
        delete data;                    // Delete our data
        data = new int(*other.data);    // Copy from other's data
        // But if this == &other, we already deleted it! 💥
        return *this;
    }
};

void demonstrateSelfAssignment() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: SELF-ASSIGNMENT        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "Why we need self-assignment check:\n";
    cout << "\n";
    cout << "WITHOUT check:\n";
    cout << "  operator=(const T& other) {\n";
    cout << "      delete data;               // Delete our data\n";
    cout << "      data = new int(*other.data);  // Copy from other\n";
    cout << "      // If this == &other, we deleted other.data! 💥\n";
    cout << "  }\n";
    cout << "\n";
    cout << "WITH check:\n";
    cout << "  operator=(const T& other) {\n";
    cout << "      if (this == &other) return *this;  // ✅ Safe!\n";
    cout << "      delete data;\n";
    cout << "      data = new int(*other.data);\n";
    cout << "      return *this;\n";
    cout << "  }\n";
    cout << "\n";
    cout << "⚠️  Self-assignment can happen:\n";
    cout << "   - obj = obj;                    // Direct\n";
    cout << "   - *ptr1 = *ptr2;  (if ptr1 == ptr2)  // Indirect\n";
    cout << "   - arr[i] = arr[j];  (if i == j)      // Array\n";
}

// ============================================
// EXAMPLE 4: MOVE SEMANTICS INTRODUCTION
// ============================================

class ExpensiveToCopy {
private:
    int* data;
    int size;

public:
    ExpensiveToCopy(int s) : size(s) {
        data = new int[size];
        for (int i = 0; i < size; i++) data[i] = i;
        cout << "✅ Constructor: allocated " << size << " ints\n";
    }

    ~ExpensiveToCopy() {
        delete[] data;
    }

    // Copy constructor - EXPENSIVE!
    ExpensiveToCopy(const ExpensiveToCopy& other) : size(other.size) {
        data = new int[size];
        copy(other.data, other.data + size, data);
        cout << "📋 Copy Constructor: deep copied " << size << " ints (EXPENSIVE!)\n";
    }

    ExpensiveToCopy& operator=(const ExpensiveToCopy& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = new int[size];
            copy(other.data, other.data + size, data);
            cout << "📋 Copy Assignment: deep copied " << size << " ints (EXPENSIVE!)\n";
        }
        return *this;
    }
};

ExpensiveToCopy createLargeObject() {
    ExpensiveToCopy obj(1000000);
    return obj;  // C++98: EXPENSIVE COPY! (unless RVO kicks in)
}

void demonstrateMoveNeed() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: WHY MOVE SEMANTICS?    ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "Problem: Copying large objects is EXPENSIVE\n";
    cout << "\n";
    cout << "Example: Returning a vector with 1 million elements\n";
    cout << "\n";
    cout << "vector<int> createVector() {\n";
    cout << "    vector<int> v(1000000);\n";
    cout << "    return v;  // C++98: COPIES entire vector! 😱\n";
    cout << "}\n";
    cout << "\n";
    cout << "Solution: MOVE instead of copy!\n";
    cout << "- Transfer ownership (steal the pointer)\n";
    cout << "- Leave source in valid but empty state\n";
    cout << "- Much faster!\n";
}

// ============================================
// EXAMPLE 5: RULE OF FIVE - WITH MOVE
// ============================================

class RuleOfFive {
private:
    int* data;
    string name;

public:
    // Constructor
    RuleOfFive(int value, const string& n) : name(n) {
        data = new int(value);
        cout << "✅ [" << name << "] Constructor: data = " << *data << "\n";
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

    // 4. Move Constructor (NEW in C++11!)
    RuleOfFive(RuleOfFive&& other) noexcept
        : data(other.data), name(other.name + "_moved") {
        cout << "🚀 [" << name << "] Move Constructor\n";
        other.data = nullptr;  // Leave source in valid state
    }

    // 5. Move Assignment (NEW in C++11!)
    RuleOfFive& operator=(RuleOfFive&& other) noexcept {
        cout << "🚀 [" << name << "] Move Assignment\n";
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
        cout << "   [" << name << "] data = "
             << (data ? to_string(*data) : "nullptr") << "\n";
    }
};

void demonstrateRuleOfFive() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: RULE OF FIVE           ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "RULE OF FIVE (C++11):\n";
    cout << "If you need ONE, you need ALL FIVE:\n";
    cout << "  1. Destructor\n";
    cout << "  2. Copy Constructor\n";
    cout << "  3. Copy Assignment Operator\n";
    cout << "  4. Move Constructor              ← NEW!\n";
    cout << "  5. Move Assignment Operator      ← NEW!\n";
    cout << "\n";

    cout << "--- Creating obj1 ---\n";
    RuleOfFive obj1(100, "Obj1");
    obj1.display();

    cout << "\n--- Copy Constructor (obj2 = obj1) ---\n";
    RuleOfFive obj2 = obj1;
    obj2.display();
    obj1.display();  // obj1 still valid

    cout << "\n--- Move Constructor (obj3 = move(obj1)) ---\n";
    RuleOfFive obj3 = std::move(obj1);  // Transfer ownership
    obj3.display();
    obj1.display();  // obj1 now empty (data = nullptr)

    cout << "\n--- Copy Assignment (obj4 = obj2) ---\n";
    RuleOfFive obj4(200, "Obj4");
    obj4 = obj2;
    obj4.display();

    cout << "\n--- Move Assignment (obj5 = move(obj2)) ---\n";
    RuleOfFive obj5(300, "Obj5");
    obj5 = std::move(obj2);  // Transfer ownership
    obj5.display();
    obj2.display();  // obj2 now empty

    cout << "\n--- Destruction phase ---\n";
}

// ============================================
// EXAMPLE 6: STD::MOVE EXPLAINED
// ============================================

void demonstrateStdMove() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: STD::MOVE EXPLAINED    ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "What is std::move?\n";
    cout << "\n";
    cout << "std::move does NOT move anything!\n";
    cout << "It just CASTS an lvalue to an rvalue reference.\n";
    cout << "\n";
    cout << "Example:\n";
    cout << "  RuleOfFive obj1(100, \"Obj1\");\n";
    cout << "  \n";
    cout << "  // obj1 is an lvalue (has a name)\n";
    cout << "  RuleOfFive obj2 = obj1;           // Copy constructor\n";
    cout << "  \n";
    cout << "  // std::move(obj1) casts obj1 to rvalue\n";
    cout << "  RuleOfFive obj3 = std::move(obj1); // Move constructor\n";
    cout << "\n";
    cout << "After std::move(obj1):\n";
    cout << "  ✅ obj1 is still valid (can be destroyed safely)\n";
    cout << "  ✅ obj1 is in a valid but unspecified state\n";
    cout << "  ⚠️  Don't use obj1 anymore (except assign/destroy)\n";
    cout << "\n";
    cout << "Common uses:\n";
    cout << "  1. Return by value:     return std::move(local);\n";
    cout << "  2. Transfer to container: vec.push_back(std::move(obj));\n";
    cout << "  3. Swap:                swap(a, b) uses move internally\n";
    cout << "  4. Done with object:    unique_ptr = std::move(other);\n";
}

// ============================================
// EXAMPLE 7: RULE OF ZERO - THE BEST!
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
        copy(other.data, other.data + size, data);
        copy(other.buffer, other.buffer + size, buffer);
    }

    ManualResource& operator=(const ManualResource& other) {
        if (this != &other) {
            delete[] data;
            delete[] buffer;
            size = other.size;
            data = new int[size];
            buffer = new char[size];
            copy(other.data, other.data + size, data);
            copy(other.buffer, other.buffer + size, buffer);
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
    vector<int> data;      // RAII - manages its own memory!
    vector<char> buffer;   // RAII - manages its own memory!
    string name;           // RAII - manages its own memory!
    unique_ptr<int> ptr;   // RAII - manages its own memory!

public:
    AutoResource(int size, const string& n)
        : data(size), buffer(size), name(n), ptr(make_unique<int>(42)) {
        cout << "✅ [" << name << "] Constructor (using RAII types)\n";
    }

    // NO NEED TO DEFINE ANY SPECIAL MEMBERS! 🎉
    // Compiler generates ALL of them correctly:
    // - Destructor          ✅ Auto-generated
    // - Copy Constructor    ✅ Auto-generated
    // - Copy Assignment     ✅ Auto-generated
    // - Move Constructor    ✅ Auto-generated
    // - Move Assignment     ✅ Auto-generated

    void display() const {
        cout << "   [" << name << "] data.size = " << data.size()
             << ", ptr = " << *ptr << "\n";
    }
};

void demonstrateRuleOfZero() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 7: RULE OF ZERO (BEST!)   ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "RULE OF ZERO (Modern C++):\n";
    cout << "DON'T define ANY special members!\n";
    cout << "Use RAII types instead:\n";
    cout << "  - unique_ptr, shared_ptr (smart pointers)\n";
    cout << "  - vector, string (containers)\n";
    cout << "  - fstream (file handling)\n";
    cout << "  - Custom RAII types\n";
    cout << "\n";

    cout << "Benefits:\n";
    cout << "  ✅ LESS CODE to write and maintain\n";
    cout << "  ✅ Compiler generates OPTIMAL implementations\n";
    cout << "  ✅ CORRECT by default\n";
    cout << "  ✅ HARDER to make mistakes\n";
    cout << "  ✅ Automatic exception safety\n";
    cout << "\n";

    cout << "--- Testing Rule of Zero class ---\n";
    AutoResource obj1(1000, "Obj1");
    obj1.display();

    cout << "\n--- Copy constructor (deleted) ---\n";
    // AutoResource obj2 = obj1;  // ❌ ERROR: Copy is deleted!
    // obj2.display();
    cout << "   Copy constructor is deleted - would cause error\n";

    cout << "\n--- Move (auto-generated move constructor) ---\n";
    AutoResource obj3 = std::move(obj1);
    obj3.display();

    cout << "\n✅ All special members work perfectly!\n";
    cout << "✅ No manual memory management needed!\n";

    cout << "\n--- Destruction phase (auto-generated destructors) ---\n";
}

// ============================================
// EXAMPLE 8: WHEN TO USE EACH RULE
// ============================================

void demonstrateWhenToUse() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 8: WHEN TO USE EACH RULE  ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "DECISION TREE:\n";
    cout << "\n";
    cout << "1. Can you use RAII types?\n";
    cout << "   (vector, string, unique_ptr, etc.)\n";
    cout << "   YES → Use RULE OF ZERO ✅ (BEST!)\n";
    cout << "   NO → Continue to 2\n";
    cout << "\n";
    cout << "2. Do you manage raw resources?\n";
    cout << "   (new/delete, fopen/fclose, etc.)\n";
    cout << "   NO → Use RULE OF ZERO ✅\n";
    cout << "   YES → Continue to 3\n";
    cout << "\n";
    cout << "3. Do you need move semantics?\n";
    cout << "   (C++11+ and performance matters)\n";
    cout << "   YES → Use RULE OF FIVE ✅\n";
    cout << "   NO → Use RULE OF THREE ✅\n";
    cout << "\n";
    cout << "PREFERENCE ORDER:\n";
    cout << "  1st: RULE OF ZERO     (use RAII types)\n";
    cout << "  2nd: RULE OF FIVE     (if manual resources + C++11+)\n";
    cout << "  3rd: RULE OF THREE    (if manual resources + C++98)\n";
    cout << "\n";
    cout << "EXAMPLES:\n";
    cout << "\n";
    cout << "Rule of Zero:\n";
    cout << "  class MyClass {\n";
    cout << "      vector<int> data;     // RAII!\n";
    cout << "      unique_ptr<Foo> ptr;  // RAII!\n";
    cout << "      string name;          // RAII!\n";
    cout << "  };\n";
    cout << "  // No special members needed!\n";
    cout << "\n";
    cout << "Rule of Five:\n";
    cout << "  class MyClass {\n";
    cout << "      int* data;  // Raw pointer\n";
    cout << "  public:\n";
    cout << "      ~MyClass();                          // 1\n";
    cout << "      MyClass(const MyClass&);             // 2\n";
    cout << "      MyClass& operator=(const MyClass&);  // 3\n";
    cout << "      MyClass(MyClass&&) noexcept;         // 4\n";
    cout << "      MyClass& operator=(MyClass&&) noexcept; // 5\n";
    cout << "  };\n";
    cout << "\n";
    cout << "Rule of Three:\n";
    cout << "  class MyClass {\n";
    cout << "      int* data;  // Raw pointer (C++98)\n";
    cout << "  public:\n";
    cout << "      ~MyClass();                          // 1\n";
    cout << "      MyClass(const MyClass&);             // 2\n";
    cout << "      MyClass& operator=(const MyClass&);  // 3\n";
    cout << "  };\n";
}

// ============================================
// EXAMPLE 9: DEFAULT AND DELETE
// ============================================

class NonCopyable {
private:
    unique_ptr<int> data;

public:
    NonCopyable(int value) : data(make_unique<int>(value)) {
        cout << "✅ NonCopyable: Constructor\n";
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
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 9: = DEFAULT & = DELETE   ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "= default: Ask compiler to generate default version\n";
    cout << "= delete:  Explicitly forbid this operation\n";
    cout << "\n";

    cout << "Example: Non-copyable but moveable class\n";
    cout << "\n";
    cout << "class NonCopyable {\n";
    cout << "    unique_ptr<int> data;\n";
    cout << "public:\n";
    cout << "    // Delete copy operations\n";
    cout << "    NonCopyable(const NonCopyable&) = delete;\n";
    cout << "    NonCopyable& operator=(const NonCopyable&) = delete;\n";
    cout << "    \n";
    cout << "    // Default move operations\n";
    cout << "    NonCopyable(NonCopyable&&) = default;\n";
    cout << "    NonCopyable& operator=(NonCopyable&&) = default;\n";
    cout << "};\n";
    cout << "\n";

    cout << "--- Testing ---\n";
    NonCopyable obj1(42);

    // NonCopyable obj2 = obj1;  // ❌ Compile error! Deleted!

    NonCopyable obj3 = std::move(obj1);  // ✅ OK! Move is allowed
    cout << "✅ Move succeeded!\n";

    cout << "\n";
    cout << "Common patterns:\n";
    cout << "  1. Non-copyable: delete copy, default/custom move\n";
    cout << "  2. Non-moveable: delete move, default/custom copy\n";
    cout << "  3. Neither:      delete both (singletons)\n";
    cout << "  4. Both:         default both (aggregate types)\n";
}

// ============================================
// EXAMPLE 10: PERFORMANCE COMPARISON
// ============================================

class HeavyResource {
private:
    vector<int> data;
    string name;

public:
    HeavyResource(int size, const string& n) : data(size, 42), name(n) {}

    // Copy - EXPENSIVE!
    HeavyResource(const HeavyResource& other) : data(other.data), name(other.name + "_copy") {
        cout << "📋 Copy: " << data.size() << " elements (EXPENSIVE!)\n";
    }

    // Move - CHEAP!
    HeavyResource(HeavyResource&& other) noexcept
        : data(std::move(other.data)), name(std::move(other.name) + "_moved") {
        cout << "🚀 Move: Just pointer swap (CHEAP!)\n";
    }
};

void demonstratePerformance() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 10: PERFORMANCE           ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "Creating large object (1 million elements)...\n";
    HeavyResource obj1(1000000, "Heavy");

    cout << "\n--- Copy (expensive) ---\n";
    HeavyResource obj2 = obj1;  // Copies 1 million elements!

    cout << "\n--- Move (cheap) ---\n";
    HeavyResource obj3 = std::move(obj1);  // Just swaps pointers!

    cout << "\n";
    cout << "Performance difference:\n";
    cout << "  Copy: O(n) - copies all elements\n";
    cout << "  Move: O(1) - just swaps pointers\n";
    cout << "\n";
    cout << "For 1 million elements:\n";
    cout << "  Copy: ~4MB data transfer\n";
    cout << "  Move: ~16 bytes (2 pointers)\n";
    cout << "  Speedup: ~250,000x faster! 🚀\n";
}

// ============================================
// EXAMPLE 11: COMMON MISTAKES
// ============================================

void demonstrateCommonMistakes() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 11: COMMON MISTAKES       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "MISTAKE 1: Forgetting self-assignment check\n";
    cout << "  ❌ operator=(const T& other) {\n";
    cout << "       delete data;\n";
    cout << "       data = new int(*other.data); // 💥 if this == &other\n";
    cout << "     }\n";
    cout << "  ✅ if (this == &other) return *this;\n";
    cout << "\n";

    cout << "MISTAKE 2: Missing noexcept on move operations\n";
    cout << "  ❌ MyClass(MyClass&& other) { ... }\n";
    cout << "  ✅ MyClass(MyClass&& other) noexcept { ... }\n";
    cout << "  Why: STL won't use moves without noexcept!\n";
    cout << "\n";

    cout << "MISTAKE 3: Not leaving moved-from object in valid state\n";
    cout << "  ❌ MyClass(MyClass&& other) : data(other.data) {}\n";
    cout << "  ✅ MyClass(MyClass&& other) : data(other.data) {\n";
    cout << "       other.data = nullptr;  // Valid state!\n";
    cout << "     }\n";
    cout << "\n";

    cout << "MISTAKE 4: Defining some but not all (Rule of Three/Five)\n";
    cout << "  ❌ Only defining destructor\n";
    cout << "  ❌ Only defining copy constructor\n";
    cout << "  ✅ Define all three (Rule of Three)\n";
    cout << "  ✅ Define all five (Rule of Five)\n";
    cout << "  ✅ Define none (Rule of Zero) ← BEST!\n";
    cout << "\n";

    cout << "MISTAKE 5: Not using RAII types\n";
    cout << "  ❌ int* data = new int[size];\n";
    cout << "  ✅ vector<int> data(size);\n";
    cout << "  ✅ unique_ptr<int[]> data(new int[size]);\n";
    cout << "\n";

    cout << "MISTAKE 6: Using moved-from object\n";
    cout << "  ❌ auto obj2 = std::move(obj1);\n";
    cout << "     obj1.doSomething();  // 💥 Undefined behavior!\n";
    cout << "  ✅ auto obj2 = std::move(obj1);\n";
    cout << "     // Don't use obj1 anymore\n";
    cout << "\n";

    cout << "MISTAKE 7: Unnecessary std::move on return\n";
    cout << "  ❌ return std::move(local);  // Prevents RVO!\n";
    cout << "  ✅ return local;             // Let compiler optimize\n";
}

// ============================================
// EXAMPLE 12: BEST PRACTICES
// ============================================

void demonstrateBestPractices() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 12: BEST PRACTICES        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "✅ BEST PRACTICES:\n";
    cout << "\n";
    cout << "1. PREFER RULE OF ZERO\n";
    cout << "   Use vector, string, unique_ptr, shared_ptr\n";
    cout << "   Let compiler generate special members\n";
    cout << "\n";
    cout << "2. IF YOU MUST MANAGE RESOURCES:\n";
    cout << "   - Use Rule of Five (C++11+)\n";
    cout << "   - Use Rule of Three (C++98)\n";
    cout << "   - Define ALL or NONE\n";
    cout << "\n";
    cout << "3. ALWAYS:\n";
    cout << "   - Check for self-assignment in copy assignment\n";
    cout << "   - Add noexcept to move operations\n";
    cout << "   - Leave moved-from object in valid state\n";
    cout << "   - Prefer pass-by-value for assignment when appropriate\n";
    cout << "\n";
    cout << "4. MOVE SEMANTICS:\n";
    cout << "   - Use std::move when done with object\n";
    cout << "   - DON'T use std::move on return (let RVO work)\n";
    cout << "   - DON'T use moved-from objects (except assign/destroy)\n";
    cout << "\n";
    cout << "5. PERFORMANCE:\n";
    cout << "   - Pass large objects by const& (avoid copies)\n";
    cout << "   - Return by value (let compiler optimize)\n";
    cout << "   - Use emplace_back() instead of push_back()\n";
    cout << "   - Reserve capacity for containers\n";
    cout << "\n";
    cout << "6. SAFETY:\n";
    cout << "   - Use smart pointers (unique_ptr, shared_ptr)\n";
    cout << "   - Use RAII for all resources\n";
    cout << "   - Avoid raw new/delete\n";
    cout << "   - Make destructors virtual in base classes\n";
}

// ============================================
// MAIN - RUN ALL EXAMPLES
// ============================================

int main() {
    cout << "╔══════════════════════════════════════════════════════════╗\n";
    cout << "║                                                          ║\n";
    cout << "║       RULE OF THREE / FIVE / ZERO - COMPLETE GUIDE       ║\n";
    cout << "║                                                          ║\n";
    cout << "╚══════════════════════════════════════════════════════════╝\n";

    try {
        // Note: Example 1 will crash! Run others separately.
        // demonstrateProblem();          // Shows the problem

        demonstrateRuleOfThree();         // Solution: Rule of Three
        demonstrateSelfAssignment();      // Why self-assignment matters
        demonstrateMoveNeed();            // Why we need move semantics
        demonstrateRuleOfFive();          // Complete: Rule of Five
        demonstrateStdMove();             // Understanding std::move
        demonstrateRuleOfZero();          // Best: Rule of Zero
        demonstrateWhenToUse();           // Decision tree
        demonstrateDefaultDelete();       // = default & = delete
        demonstratePerformance();         // Copy vs Move performance
        demonstrateCommonMistakes();      // Mistakes to avoid
        demonstrateBestPractices();       // Best practices

    } catch (const exception& e) {
        cout << "💥 Exception: " << e.what() << "\n";
    }

    cout << "\n╔══════════════════════════════════════════════════════════╗\n";
    cout << "║                    KEY TAKEAWAYS                         ║\n";
    cout << "╚══════════════════════════════════════════════════════════╝\n\n";

    cout << "1. RULE OF THREE (C++98):\n";
    cout << "   If you define ONE, define ALL THREE:\n";
    cout << "   - Destructor\n";
    cout << "   - Copy Constructor\n";
    cout << "   - Copy Assignment Operator\n";
    cout << "\n";

    cout << "2. RULE OF FIVE (C++11):\n";
    cout << "   Add move semantics to Rule of Three:\n";
    cout << "   - Destructor\n";
    cout << "   - Copy Constructor\n";
    cout << "   - Copy Assignment Operator\n";
    cout << "   - Move Constructor (noexcept!)\n";
    cout << "   - Move Assignment Operator (noexcept!)\n";
    cout << "\n";

    cout << "3. RULE OF ZERO (Modern C++ - BEST!):\n";
    cout << "   DON'T define ANY special members!\n";
    cout << "   Use RAII types:\n";
    cout << "   - vector, string, array\n";
    cout << "   - unique_ptr, shared_ptr\n";
    cout << "   - fstream, thread, mutex\n";
    cout << "\n";

    cout << "4. PREFERENCE ORDER:\n";
    cout << "   1st choice: Rule of Zero  (use RAII)\n";
    cout << "   2nd choice: Rule of Five  (if C++11+ and raw resources)\n";
    cout << "   3rd choice: Rule of Three (if C++98 and raw resources)\n";
    cout << "\n";

    cout << "5. REMEMBER:\n";
    cout << "   ✅ Self-assignment check in copy assignment\n";
    cout << "   ✅ noexcept on move operations\n";
    cout << "   ✅ Leave moved-from in valid state\n";
    cout << "   ✅ Prefer RAII over manual management\n";
    cout << "   ✅ Move is 100,000x faster than copy!\n";
    cout << "\n";

    return 0;
}

/*
COMPILATION:
    g++ -std=c++17 -Wall -Wextra RuleOf3_5_0.cpp -o RuleOf3_5_0

RUN:
    ./RuleOf3_5_0

TOPICS COVERED:
    ✅ Rule of Three (C++98)
    ✅ Rule of Five (C++11)
    ✅ Rule of Zero (Modern C++)
    ✅ The problem (shallow copy)
    ✅ Deep copy solution
    ✅ Move semantics
    ✅ std::move explained
    ✅ Self-assignment
    ✅ noexcept importance
    ✅ RAII types
    ✅ = default and = delete
    ✅ Performance comparison
    ✅ Common mistakes
    ✅ Best practices
    ✅ When to use each rule
    ✅ Decision tree

OUTPUT: Complete demonstrations with emojis and clear sections!
*/
