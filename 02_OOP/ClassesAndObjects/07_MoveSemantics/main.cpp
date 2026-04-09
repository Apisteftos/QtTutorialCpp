#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>  // for std::move
#include <cstring>
using namespace std;

// ============================================
// WHAT ARE MOVE SEMANTICS?
// ============================================

/*
 * MOVE SEMANTICS (C++11+):
 * - Transfer ownership of resources instead of copying
 * - Much faster for objects with dynamic memory
 * - Uses rvalue references (&&)
 * - Leaves source object in valid but empty state
 *
 * LVALUE vs RVALUE:
 * - Lvalue: Has a name, can appear on left of =
 * - Rvalue: Temporary, no name, about to be destroyed
 *
 * MOVE CONSTRUCTOR:
 * - ClassName(ClassName&& other) noexcept
 * - Steals resources from source
 * - Source left empty but valid
 *
 * MOVE ASSIGNMENT:
 * - ClassName& operator=(ClassName&& other) noexcept
 * - Similar to move constructor
 * - Must handle self-assignment
 */

// ============================================
// EXAMPLE 1: THE PROBLEM - EXPENSIVE COPIES
// ============================================

class ExpensiveClass {
private:
    int* data;
    int size;
    string name;

public:
    ExpensiveClass(int s, string n) : size(s), name(n) {
        data = new int[size];
        for (int i = 0; i < size; i++) {
            data[i] = i;
        }
        cout << "  Constructor: Created " << name << " (" << size << " elements)\n";
    }

    // Copy constructor - EXPENSIVE!
    ExpensiveClass(const ExpensiveClass& other) : size(other.size), name(other.name + "_Copy") {
        data = new int[size];
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
        cout << "  Copy Constructor: Copied " << size << " elements (EXPENSIVE!)\n";
    }

    ~ExpensiveClass() {
        cout << "  Destructor: Deleting " << name << "\n";
        delete[] data;
    }

    void display() const {
        cout << "  " << name << ": " << size << " elements\n";
    }
};

ExpensiveClass createObject() {
    ExpensiveClass temp(1000000, "Temporary");
    return temp;  // Expensive copy!
}

void demonstrateProblem() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: THE PROBLEM            ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating object from function ---\n";
    // ExpensiveClass obj = createObject();  // Would copy 1M elements!
    cout << "⚠️  Without move semantics, this would copy 1 million elements!\n";
    cout << "⚠️  Very slow and wasteful for temporary objects\n";

    cout << "\n💡 Move semantics solves this by STEALING resources\n";
    cout << "   instead of copying them!\n";
}

// ============================================
// EXAMPLE 2: MOVE CONSTRUCTOR BASICS
// ============================================

class MoveableClass {
private:
    int* data;
    int size;
    string name;

public:
    // Constructor
    MoveableClass(int s, string n) : size(s), name(n) {
        data = new int[size];
        for (int i = 0; i < size; i++) {
            data[i] = i;
        }
        cout << "  Constructor: Created " << name << " at " << data << "\n";
    }

    // Copy constructor
    MoveableClass(const MoveableClass& other) : size(other.size), name(other.name + "_Copy") {
        data = new int[size];
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
        cout << "  Copy Constructor: Copied " << size << " elements\n";
    }

    // MOVE CONSTRUCTOR
    MoveableClass(MoveableClass&& other) noexcept
        : data(other.data), size(other.size), name(other.name + "_Moved") {

        // Steal resources
        // (Already done in initializer list)

        // Leave source in valid state
        other.data = nullptr;
        other.size = 0;

        cout << "  Move Constructor: Moved resources from " << other.name << " (fast!)\n";
    }

    ~MoveableClass() {
        cout << "  Destructor: Deleting " << name;
        if (data) {
            cout << " at " << data << "\n";
            delete[] data;
        } else {
            cout << " (already moved)\n";
        }
    }

    void display() const {
        cout << "  " << name << ": ";
        if (data) {
            cout << size << " elements at " << data << "\n";
        } else {
            cout << "empty (moved from)\n";
        }
    }
};

void demonstrateMoveConstructor() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: MOVE CONSTRUCTOR       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating original object ---\n";
    MoveableClass obj1(5, "Original");
    obj1.display();

    cout << "\n--- Move construction with std::move ---\n";
    MoveableClass obj2(std::move(obj1));  // Move constructor

    cout << "\n--- After move ---\n";
    cout << "obj1 (source): ";
    obj1.display();
    cout << "obj2 (dest):   ";
    obj2.display();

    cout << "\n✅ Resources transferred, not copied!\n";
    cout << "✅ obj1 is empty but valid\n";
    cout << "✅ Much faster than copying!\n";

    cout << "\n--- Cleanup ---\n";
}

// ============================================
// EXAMPLE 3: LVALUE vs RVALUE
// ============================================

void demonstrateLvalueRvalue() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: LVALUE vs RVALUE       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    int x = 10;
    int y = 20;

    cout << "LVALUES (have names, persist):\n";
    cout << "  int x = 10;           // x is lvalue\n";
    cout << "  int y = 20;           // y is lvalue\n";
    cout << "  x = y;                // both are lvalues\n";
    cout << "  int& ref = x;         // can bind to lvalue reference\n";

    cout << "\nRVALUES (temporaries, no names):\n";
    cout << "  int z = x + y;        // (x + y) is rvalue\n";
    cout << "  int a = 5;            // 5 is rvalue\n";
    cout << "  func(x + 10);         // (x + 10) is rvalue\n";
    cout << "  vector<int>{1,2,3};   // temporary vector is rvalue\n";

    cout << "\nREFERENCE TYPES:\n";
    cout << "  int& lr = x;          // lvalue reference (binds to lvalue)\n";
    cout << "  int&& rr = 42;        // rvalue reference (binds to rvalue)\n";
    cout << "  int&& rr2 = x + y;    // rvalue reference to temporary\n";

    cout << "\n💡 Key points:\n";
    cout << "   • Lvalues: persistent, have address, can appear on left of =\n";
    cout << "   • Rvalues: temporary, about to be destroyed\n";
    cout << "   • && creates rvalue reference\n";
    cout << "   • Move semantics works with rvalues\n";
}

// ============================================
// EXAMPLE 4: MOVE ASSIGNMENT OPERATOR
// ============================================

class FullyMoveable {
private:
    int* data;
    int size;
    string name;

public:
    // Constructor
    FullyMoveable(int s, string n) : size(s), name(n) {
        data = new int[size];
        cout << "  Constructor: " << name << "\n";
    }

    // Copy constructor
    FullyMoveable(const FullyMoveable& other) : size(other.size), name(other.name + "_Copy") {
        data = new int[size];
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
        cout << "  Copy Constructor\n";
    }

    // Copy assignment
    FullyMoveable& operator=(const FullyMoveable& other) {
        cout << "  Copy Assignment\n";
        if (this != &other) {
            delete[] data;
            size = other.size;
            name = other.name + "_CopyAssigned";
            data = new int[size];
            for (int i = 0; i < size; i++) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    // Move constructor
    FullyMoveable(FullyMoveable&& other) noexcept
        : data(other.data), size(other.size), name(other.name + "_Moved") {
        other.data = nullptr;
        other.size = 0;
        cout << "  Move Constructor\n";
    }

    // MOVE ASSIGNMENT OPERATOR
    FullyMoveable& operator=(FullyMoveable&& other) noexcept {
        cout << "  Move Assignment\n";

        if (this != &other) {
            // Delete current resources
            delete[] data;

            // Steal resources
            data = other.data;
            size = other.size;
            name = other.name + "_MoveAssigned";

            // Leave source empty
            other.data = nullptr;
            other.size = 0;
        }
        return *this;
    }

    ~FullyMoveable() {
        cout << "  Destructor: " << name << "\n";
        delete[] data;
    }

    void display() const {
        cout << "  " << name << ": " << (data ? "has data" : "empty") << "\n";
    }
};

void demonstrateMoveAssignment() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: MOVE ASSIGNMENT        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating objects ---\n";
    FullyMoveable obj1(5, "First");
    FullyMoveable obj2(3, "Second");

    cout << "\n--- Copy assignment (obj2 = obj1) ---\n";
    FullyMoveable obj3(2, "Third");
    obj3 = obj1;  // Copy assignment

    cout << "\n--- Move assignment (obj2 = std::move(obj1)) ---\n";
    obj2 = std::move(obj1);  // Move assignment

    cout << "\n--- After move assignment ---\n";
    obj1.display();
    obj2.display();

    cout << "\n--- Cleanup ---\n";
}

// ============================================
// EXAMPLE 5: THE RULE OF FIVE
// ============================================

class RuleOfFive {
private:
    int* data;
    int size;

public:
    // Constructor
    RuleOfFive(int s) : size(s) {
        data = new int[size];
        cout << "  1. Constructor\n";
    }

    // 1. Destructor
    ~RuleOfFive() {
        cout << "  1. Destructor\n";
        delete[] data;
    }

    // 2. Copy Constructor
    RuleOfFive(const RuleOfFive& other) : size(other.size) {
        data = new int[size];
        copy(other.data, other.data + size, data);
        cout << "  2. Copy Constructor\n";
    }

    // 3. Copy Assignment
    RuleOfFive& operator=(const RuleOfFive& other) {
        cout << "  3. Copy Assignment\n";
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = new int[size];
            copy(other.data, other.data + size, data);
        }
        return *this;
    }

    // 4. Move Constructor
    RuleOfFive(RuleOfFive&& other) noexcept
        : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
        cout << "  4. Move Constructor\n";
    }

    // 5. Move Assignment
    RuleOfFive& operator=(RuleOfFive&& other) noexcept {
        cout << "  5. Move Assignment\n";
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
        }
        return *this;
    }
};

void demonstrateRuleOfFive() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: RULE OF FIVE           ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "RULE OF FIVE (C++11+):\n";
    cout << "If you define ONE, define ALL FIVE:\n";
    cout << "  1. Destructor\n";
    cout << "  2. Copy Constructor\n";
    cout << "  3. Copy Assignment\n";
    cout << "  4. Move Constructor        ← New in C++11\n";
    cout << "  5. Move Assignment         ← New in C++11\n";
    cout << "\n";

    cout << "--- Demonstrations ---\n";
    RuleOfFive obj1(5);                    // Constructor
    RuleOfFive obj2 = obj1;                // Copy constructor
    RuleOfFive obj3(3);
    obj3 = obj1;                           // Copy assignment
    RuleOfFive obj4 = std::move(obj1);     // Move constructor
    RuleOfFive obj5(2);
    obj5 = std::move(obj2);                // Move assignment

    cout << "\n--- Cleanup ---\n";
}

// ============================================
// EXAMPLE 6: STD::MOVE EXPLAINED
// ============================================

class Tracker {
private:
    string name;

public:
    Tracker(string n) : name(n) {
        cout << "  Created " << name << "\n";
    }

    Tracker(const Tracker& other) : name(other.name + "_Copy") {
        cout << "  Copy: " << other.name << " → " << name << "\n";
    }

    Tracker(Tracker&& other) noexcept : name(other.name + "_Moved") {
        cout << "  Move: " << other.name << " → " << name << "\n";
        other.name = "[moved]";
    }

    ~Tracker() {
        cout << "  Destroyed " << name << "\n";
    }

    string getName() const { return name; }
};

void demonstrateStdMove() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: STD::MOVE              ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Without std::move (copy) ---\n";
    Tracker t1("Original");
    Tracker t2 = t1;  // Copy constructor
    cout << "t1: " << t1.getName() << "\n";
    cout << "t2: " << t2.getName() << "\n";

    cout << "\n--- With std::move (move) ---\n";
    Tracker t3("Source");
    Tracker t4 = std::move(t3);  // Move constructor
    cout << "t3: " << t3.getName() << "\n";
    cout << "t4: " << t4.getName() << "\n";

    cout << "\n💡 std::move doesn't actually move anything!\n";
    cout << "   It just casts lvalue to rvalue reference\n";
    cout << "   Enables move constructor/assignment to be called\n";

    cout << "\n⚠️  After std::move, don't use the source object!\n";
    cout << "   (except to destroy or assign a new value)\n";

    cout << "\n--- Cleanup ---\n";
}

// ============================================
// EXAMPLE 7: WHEN MOVES HAPPEN AUTOMATICALLY
// ============================================

class AutoMove {
private:
    vector<int> data;
    string name;

public:
    AutoMove(string n, int size) : name(n), data(size) {
        cout << "  Created " << name << "\n";
    }

    AutoMove(const AutoMove& other) : name(other.name + "_Copy"), data(other.data) {
        cout << "  Copied " << name << "\n";
    }

    AutoMove(AutoMove&& other) noexcept : name(other.name + "_Moved"), data(std::move(other.data)) {
        cout << "  Moved " << name << "\n";
    }

    ~AutoMove() {
        cout << "  Destroyed " << name << "\n";
    }
};

AutoMove createAutoMove() {
    AutoMove temp("Temp", 100);
    return temp;  // Automatic move (or RVO)
}

void demonstrateAutomaticMoves() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 7: AUTOMATIC MOVES        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "Moves happen automatically when:\n";
    cout << "1. Returning local object from function\n";
    cout << "2. Passing temporary to function\n";
    cout << "3. Initializing from temporary\n";
    cout << "4. Container operations (push_back, etc.)\n";
    cout << "\n";

    cout << "--- Return from function (automatic move) ---\n";
    AutoMove obj1 = createAutoMove();

    cout << "\n--- Initializing from temporary ---\n";
    AutoMove obj2 = AutoMove("Temp2", 50);  // Move or elision

    cout << "\n--- Container operations ---\n";
    vector<AutoMove> vec;
    vec.push_back(AutoMove("Vec1", 10));  // Move into vector

    cout << "\n💡 Compiler automatically uses move when it knows object is temporary!\n";

    cout << "\n--- Cleanup ---\n";
}

// ============================================
// EXAMPLE 8: COPY vs MOVE PERFORMANCE
// ============================================

class PerformanceTest {
private:
    int* data;
    int size;

public:
    PerformanceTest(int s) : size(s) {
        data = new int[size];
    }

    // Copy constructor
    PerformanceTest(const PerformanceTest& other) : size(other.size) {
        data = new int[size];
        copy(other.data, other.data + size, data);
    }

    // Move constructor
    PerformanceTest(PerformanceTest&& other) noexcept
        : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
    }

    ~PerformanceTest() {
        delete[] data;
    }

    int getSize() const { return size; }
};

void demonstratePerformance() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 8: COPY vs MOVE PERFORMANCE║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    const int SIZE = 1000000;

    cout << "COPY (expensive):\n";
    cout << "  • Allocate NEW memory: " << SIZE * sizeof(int) << " bytes\n";
    cout << "  • Copy " << SIZE << " elements\n";
    cout << "  • O(n) time complexity\n";
    cout << "  • Original unchanged\n";

    cout << "\nMOVE (cheap):\n";
    cout << "  • Steal pointer: copy " << sizeof(int*) << " bytes\n";
    cout << "  • No element copying\n";
    cout << "  • O(1) time complexity\n";
    cout << "  • Original left empty\n";

    cout << "\n📊 Performance comparison:\n";
    cout << "   Copy: ~" << SIZE << " operations\n";
    cout << "   Move: ~1 operation (just pointer copy)\n";
    cout << "   Speedup: ~" << SIZE << "x faster!\n";
}

// ============================================
// EXAMPLE 9: REAL-WORLD - STRING CLASS
// ============================================

class String {
private:
    char* buffer;
    size_t length;

public:
    // Constructor
    String(const char* str = "") {
        length = strlen(str);
        buffer = new char[length + 1];
        strcpy(buffer, str);
        cout << "  Constructed: \"" << buffer << "\"\n";
    }

    // Copy constructor
    String(const String& other) : length(other.length) {
        buffer = new char[length + 1];
        strcpy(buffer, other.buffer);
        cout << "  Copied: \"" << buffer << "\"\n";
    }

    // Move constructor
    String(String&& other) noexcept : buffer(other.buffer), length(other.length) {
        other.buffer = nullptr;
        other.length = 0;
        cout << "  Moved: \"" << buffer << "\"\n";
    }

    // Copy assignment
    String& operator=(const String& other) {
        if (this != &other) {
            delete[] buffer;
            length = other.length;
            buffer = new char[length + 1];
            strcpy(buffer, other.buffer);
            cout << "  Copy assigned: \"" << buffer << "\"\n";
        }
        return *this;
    }

    // Move assignment
    String& operator=(String&& other) noexcept {
        if (this != &other) {
            delete[] buffer;
            buffer = other.buffer;
            length = other.length;
            other.buffer = nullptr;
            other.length = 0;
            cout << "  Move assigned: \"" << buffer << "\"\n";
        }
        return *this;
    }

    ~String() {
        if (buffer) {
            cout << "  Destroyed: \"" << buffer << "\"\n";
        } else {
            cout << "  Destroyed: [empty]\n";
        }
        delete[] buffer;
    }

    const char* c_str() const { return buffer ? buffer : ""; }
};

void demonstrateStringMove() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 9: STRING CLASS           ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Copy operations ---\n";
    String s1("Hello");
    String s2 = s1;  // Copy
    String s3("World");
    s3 = s1;  // Copy assignment

    cout << "\n--- Move operations ---\n";
    String s4 = std::move(s1);  // Move
    String s5("Temp");
    s5 = std::move(s2);  // Move assignment

    cout << "\n--- Values ---\n";
    cout << "s1: \"" << s1.c_str() << "\" (moved from)\n";
    cout << "s2: \"" << s2.c_str() << "\" (moved from)\n";
    cout << "s3: \"" << s3.c_str() << "\"\n";
    cout << "s4: \"" << s4.c_str() << "\" (moved to)\n";
    cout << "s5: \"" << s5.c_str() << "\" (moved to)\n";

    cout << "\n--- Cleanup ---\n";
}

// ============================================
// EXAMPLE 10: REAL-WORLD - UNIQUE POINTER
// ============================================

template<typename T>
class UniquePtr {
private:
    T* ptr;

public:
    // Constructor
    explicit UniquePtr(T* p = nullptr) : ptr(p) {
        cout << "  UniquePtr: Created\n";
    }

    // Delete copy operations
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    // Move constructor
    UniquePtr(UniquePtr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
        cout << "  UniquePtr: Moved\n";
    }

    // Move assignment
    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
            cout << "  UniquePtr: Move assigned\n";
        }
        return *this;
    }

    ~UniquePtr() {
        delete ptr;
        cout << "  UniquePtr: Destroyed\n";
    }

    T* get() const { return ptr; }
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
};

void demonstrateUniquePtr() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 10: UNIQUE POINTER        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating unique_ptr ---\n";
    UniquePtr<int> ptr1(new int(42));
    cout << "Value: " << *ptr1 << "\n";

    cout << "\n--- Cannot copy (deleted) ---\n";
    // UniquePtr<int> ptr2 = ptr1;  // ❌ Error! Copy deleted
    cout << "⚠️  Copy constructor is deleted\n";

    cout << "\n--- Can move (transfers ownership) ---\n";
    UniquePtr<int> ptr2 = std::move(ptr1);  // ✅ Move OK
    cout << "ptr2 value: " << *ptr2 << "\n";
    cout << "ptr1 is now nullptr\n";

    cout << "\n💡 Unique ownership enforced through move-only!\n";
    cout << "   Similar to std::unique_ptr\n";

    cout << "\n--- Cleanup ---\n";
}

// ============================================
// MAIN DEMONSTRATION
// ============================================

int main() {
    cout << "========================================\n";
    cout << "MOVE SEMANTICS DEMONSTRATION (C++11)\n";
    cout << "========================================\n";

    demonstrateProblem();
    demonstrateMoveConstructor();
    demonstrateLvalueRvalue();
    demonstrateMoveAssignment();
    demonstrateRuleOfFive();
    demonstrateStdMove();
    demonstrateAutomaticMoves();
    demonstratePerformance();
    demonstrateStringMove();
    demonstrateUniquePtr();

    // ============================================
    // Comprehensive Summary
    // ============================================
    cout << "\n========================================\n";
    cout << "WHAT ARE MOVE SEMANTICS?\n";
    cout << "========================================\n";
    cout << "Move semantics (C++11) allow efficient transfer of resources\n";
    cout << "from one object to another without copying.\n";
    cout << "\n";
    cout << "Key concepts:\n";
    cout << "  • Transfer ownership instead of copying\n";
    cout << "  • Much faster for large objects\n";
    cout << "  • Use rvalue references (&&)\n";
    cout << "  • Leave source valid but empty\n";
    cout << "  • Automatic for temporaries\n";

    cout << "\n========================================\n";
    cout << "LVALUE vs RVALUE\n";
    cout << "========================================\n";
    cout << "LVALUE:\n";
    cout << "  • Has a name\n";
    cout << "  • Persists beyond expression\n";
    cout << "  • Can take address with &\n";
    cout << "  • Can appear on left of =\n";
    cout << "  • Examples: variables, array elements\n";
    cout << "\n";
    cout << "RVALUE:\n";
    cout << "  • Temporary, no name\n";
    cout << "  • Expires at end of expression\n";
    cout << "  • Cannot take address\n";
    cout << "  • Can only appear on right of =\n";
    cout << "  • Examples: literals, temporaries, (a + b)\n";
    cout << "\n";
    cout << "REFERENCES:\n";
    cout << "  • int&   - lvalue reference (binds to lvalue)\n";
    cout << "  • int&&  - rvalue reference (binds to rvalue)\n";

    cout << "\n========================================\n";
    cout << "MOVE CONSTRUCTOR\n";
    cout << "========================================\n";
    cout << "Signature:\n";
    cout << "  ClassName(ClassName&& other) noexcept\n";
    cout << "\n";
    cout << "Template:\n";
    cout << "  MyClass(MyClass&& other) noexcept\n";
    cout << "      : data(other.data), size(other.size) {\n";
    cout << "      \n";
    cout << "      // Steal resources (done in initializer)\n";
    cout << "      \n";
    cout << "      // Leave source empty\n";
    cout << "      other.data = nullptr;\n";
    cout << "      other.size = 0;\n";
    cout << "  }\n";
    cout << "\n";
    cout << "Key points:\n";
    cout << "  • Takes rvalue reference (&&)\n";
    cout << "  • Should be noexcept\n";
    cout << "  • Steals resources\n";
    cout << "  • Leaves source valid but empty\n";

    cout << "\n========================================\n";
    cout << "MOVE ASSIGNMENT OPERATOR\n";
    cout << "========================================\n";
    cout << "Signature:\n";
    cout << "  ClassName& operator=(ClassName&& other) noexcept\n";
    cout << "\n";
    cout << "Template:\n";
    cout << "  MyClass& operator=(MyClass&& other) noexcept {\n";
    cout << "      if (this != &other) {\n";
    cout << "          // Delete current resources\n";
    cout << "          delete[] data;\n";
    cout << "          \n";
    cout << "          // Steal resources\n";
    cout << "          data = other.data;\n";
    cout << "          size = other.size;\n";
    cout << "          \n";
    cout << "          // Leave source empty\n";
    cout << "          other.data = nullptr;\n";
    cout << "          other.size = 0;\n";
    cout << "      }\n";
    cout << "      return *this;\n";
    cout << "  }\n";
    cout << "\n";
    cout << "Key points:\n";
    cout << "  • Clean up current resources first\n";
    cout << "  • Check for self-assignment\n";
    cout << "  • Should be noexcept\n";
    cout << "  • Return *this\n";

    cout << "\n========================================\n";
    cout << "THE RULE OF FIVE (C++11+)\n";
    cout << "========================================\n";
    cout << "If you define ONE, define ALL FIVE:\n";
    cout << "\n";
    cout << "1. Destructor\n";
    cout << "   ~MyClass();\n";
    cout << "\n";
    cout << "2. Copy Constructor\n";
    cout << "   MyClass(const MyClass& other);\n";
    cout << "\n";
    cout << "3. Copy Assignment\n";
    cout << "   MyClass& operator=(const MyClass& other);\n";
    cout << "\n";
    cout << "4. Move Constructor (NEW in C++11)\n";
    cout << "   MyClass(MyClass&& other) noexcept;\n";
    cout << "\n";
    cout << "5. Move Assignment (NEW in C++11)\n";
    cout << "   MyClass& operator=(MyClass&& other) noexcept;\n";
    cout << "\n";
    cout << "Or use = default for all, or = delete for all.\n";

    cout << "\n========================================\n";
    cout << "STD::MOVE\n";
    cout << "========================================\n";
    cout << "std::move casts lvalue to rvalue reference:\n";
    cout << "\n";
    cout << "Without std::move:\n";
    cout << "  MyClass b = a;  // Copy constructor (a is lvalue)\n";
    cout << "\n";
    cout << "With std::move:\n";
    cout << "  MyClass b = std::move(a);  // Move constructor\n";
    cout << "\n";
    cout << "Important:\n";
    cout << "  • std::move doesn't actually move!\n";
    cout << "  • Just casts to rvalue reference\n";
    cout << "  • Enables move constructor/assignment\n";
    cout << "  • After std::move, don't use source object\n";
    cout << "    (except to destroy or assign new value)\n";

    cout << "\n========================================\n";
    cout << "WHEN MOVES HAPPEN AUTOMATICALLY\n";
    cout << "========================================\n";
    cout << "Compiler uses move automatically when:\n";
    cout << "\n";
    cout << "1. Returning local object:\n";
    cout << "   MyClass func() {\n";
    cout << "       MyClass temp;\n";
    cout << "       return temp;  // Automatic move\n";
    cout << "   }\n";
    cout << "\n";
    cout << "2. Initializing from temporary:\n";
    cout << "   MyClass obj = MyClass();  // Move\n";
    cout << "\n";
    cout << "3. Container operations:\n";
    cout << "   vec.push_back(MyClass());  // Move into vector\n";
    cout << "\n";
    cout << "No need for explicit std::move in these cases!\n";

    cout << "\n========================================\n";
    cout << "COPY vs MOVE\n";
    cout << "========================================\n";
    cout << "COPY:\n";
    cout << "  • Creates independent duplicate\n";
    cout << "  • Allocates new memory\n";
    cout << "  • Copies all elements\n";
    cout << "  • O(n) time complexity\n";
    cout << "  • Original unchanged\n";
    cout << "  • Expensive for large objects\n";
    cout << "\n";
    cout << "MOVE:\n";
    cout << "  • Transfers ownership\n";
    cout << "  • Steals pointer\n";
    cout << "  • No element copying\n";
    cout << "  • O(1) time complexity\n";
    cout << "  • Original left empty\n";
    cout << "  • Very cheap\n";
    cout << "\n";
    cout << "When to use:\n";
    cout << "  Copy: Need both objects\n";
    cout << "  Move: Source object no longer needed\n";

    cout << "\n========================================\n";
    cout << "NOEXCEPT\n";
    cout << "========================================\n";
    cout << "Move operations should be noexcept:\n";
    cout << "\n";
    cout << "  MyClass(MyClass&& other) noexcept { }\n";
    cout << "  MyClass& operator=(MyClass&& other) noexcept { }\n";
    cout << "\n";
    cout << "Why noexcept is important:\n";
    cout << "  • STL containers check for noexcept\n";
    cout << "  • If not noexcept, may use copy instead\n";
    cout << "  • Stronger exception guarantee\n";
    cout << "  • Better performance\n";
    cout << "\n";
    cout << "Example:\n";
    cout << "  vector::push_back checks:\n";
    cout << "  • If move is noexcept → use move\n";
    cout << "  • If move can throw → use copy (safer)\n";

    cout << "\n========================================\n";
    cout << "BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "✅ DO:\n";
    cout << "  • Implement move for classes with resources\n";
    cout << "  • Make move operations noexcept\n";
    cout << "  • Leave moved-from object valid but empty\n";
    cout << "  • Use std::move for lvalues you're done with\n";
    cout << "  • Follow Rule of Five\n";
    cout << "  • Let compiler auto-move temporaries\n";
    cout << "  • Use move in return statements if needed\n";
    cout << "  • Consider move-only types (unique_ptr)\n";
    cout << "\n";
    cout << "❌ DON'T:\n";
    cout << "  • Use moved-from objects (except destroy/assign)\n";
    cout << "  • Forget noexcept on move operations\n";
    cout << "  • std::move return values (prevents RVO)\n";
    cout << "  • std::move const objects (won't move)\n";
    cout << "  • Assume move is free (still has cost)\n";
    cout << "  • Move when copy is needed\n";

    cout << "\n========================================\n";
    cout << "COMMON MISTAKES\n";
    cout << "========================================\n";
    cout << "❌ Mistake 1: Using moved-from object\n";
    cout << "   MyClass a;\n";
    cout << "   MyClass b = std::move(a);\n";
    cout << "   a.doSomething();  // ❌ Undefined behavior!\n";
    cout << "\n";
    cout << "❌ Mistake 2: Missing noexcept\n";
    cout << "   MyClass(MyClass&& other) {  // Missing noexcept\n";
    cout << "       // STL may use copy instead!\n";
    cout << "   }\n";
    cout << "\n";
    cout << "❌ Mistake 3: Moving return value\n";
    cout << "   MyClass func() {\n";
    cout << "       MyClass temp;\n";
    cout << "       return std::move(temp);  // ❌ Prevents RVO!\n";
    cout << "   }\n";
    cout << "\n";
    cout << "❌ Mistake 4: Moving const object\n";
    cout << "   const MyClass a;\n";
    cout << "   MyClass b = std::move(a);  // ❌ Won't move, will copy!\n";

    cout << "\n========================================\n";
    cout << "QUICK REFERENCE\n";
    cout << "========================================\n";
    cout << "// Complete moveable class\n";
    cout << "class MyClass {\n";
    cout << "    int* data;\n";
    cout << "    int size;\n";
    cout << "    \n";
    cout << "public:\n";
    cout << "    // Move constructor\n";
    cout << "    MyClass(MyClass&& other) noexcept\n";
    cout << "        : data(other.data), size(other.size) {\n";
    cout << "        other.data = nullptr;\n";
    cout << "        other.size = 0;\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    // Move assignment\n";
    cout << "    MyClass& operator=(MyClass&& other) noexcept {\n";
    cout << "        if (this != &other) {\n";
    cout << "            delete[] data;\n";
    cout << "            data = other.data;\n";
    cout << "            size = other.size;\n";
    cout << "            other.data = nullptr;\n";
    cout << "            other.size = 0;\n";
    cout << "        }\n";
    cout << "        return *this;\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Usage\n";
    cout << "MyClass a;\n";
    cout << "MyClass b = std::move(a);  // Move constructor\n";
    cout << "MyClass c;\n";
    cout << "c = std::move(b);          // Move assignment\n";

    cout << "\n========================================\n";
    cout << "KEY TAKEAWAYS\n";
    cout << "========================================\n";
    cout << "1. Move semantics = transfer ownership, not copy\n";
    cout << "2. Rvalue references (&&) enable move semantics\n";
    cout << "3. Move constructor: ClassName(ClassName&& other) noexcept\n";
    cout << "4. Move assignment: ClassName& operator=(ClassName&& other) noexcept\n";
    cout << "5. Rule of Five: add move ctor/assign to Rule of Three\n";
    cout << "6. std::move casts lvalue to rvalue reference\n";
    cout << "7. Don't use moved-from objects\n";
    cout << "8. Always mark move operations noexcept\n";
    cout << "9. Much faster than copy for large objects\n";
    cout << "10. Automatic for temporaries - compiler optimizes\n";

    return 0;
}
