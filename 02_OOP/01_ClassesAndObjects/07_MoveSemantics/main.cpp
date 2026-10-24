#include <print>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>  // for std::move
#include <cstring>
#include <chrono>
using namespace std;

// ============================================
// WHAT ARE MOVE SEMANTICS?
// ============================================

/*
 * MOVE SEMANTICS (C++11+):
 * - Transfer ownership of resources instead of copying
 * - Much faster for objects with dynamic memory
 * - Uses rvalue references (&&)
 * - Leaves source object in a valid state (for our classes: empty)
 *
 * LVALUE vs RVALUE:
 * - Lvalue: Has a name/identity, you can take its address
 * - Rvalue: Temporary (or std::move'd), about to be destroyed
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
        std::println("  Constructor: Created {} ({} elements)", name, size);
    }

    // Copy constructor - EXPENSIVE!
    ExpensiveClass(const ExpensiveClass& other) : size(other.size), name(other.name + "_Copy") {
        data = new int[size];
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
        std::println("  Copy Constructor: Copied {} elements (EXPENSIVE!)", size);
    }

    ~ExpensiveClass() {
        std::println("  Destructor: Deleting {}", name);
        delete[] data;
    }

    void display() const {
        std::println("  {}: {} elements", name, size);
    }
};

ExpensiveClass createObject() {
    ExpensiveClass temp(1000000, "Temporary");
    return temp;  // No move constructor: copy, unless the compiler applies NRVO
}

void demonstrateProblem() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: THE PROBLEM            ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Copying a big object ---");
    ExpensiveClass original(1000000, "Original");
    ExpensiveClass copy = original;   // copy constructor: allocates and copies 1M ints
    copy.display();

    std::println("");
    std::println("--- Returning a local object from a function ---");
    ExpensiveClass fromFunction = createObject();   // no copy here: see the note below
    fromFunction.display();
    // Why no copy? Since the class has no move constructor, the compiler would have to
    // COPY the returned object. GCC avoids that with NRVO (an optional optimization), but
    // the standard does not require it. A move constructor is the safety net (Example 7).

    std::println("");
    std::println("⚠️  Every real copy of a 1-million-element object allocates and copies 4 MB");
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
        std::println("  Constructor: Created {} at {}", name, static_cast<const void*>(data));
    }

    // Copy constructor
    MoveableClass(const MoveableClass& other) : size(other.size), name(other.name + "_Copy") {
        data = new int[size];
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
        std::println("  Copy Constructor: Copied {} elements", size);
    }

    // MOVE CONSTRUCTOR
    MoveableClass(MoveableClass&& other) noexcept
        : data(other.data), size(other.size), name(other.name + "_Moved") {

        // Steal resources
        // (Already done in initializer list)

        // Leave source in valid state
        other.data = nullptr;
        other.size = 0;

        std::println("  Move Constructor: Moved resources from {} (fast!)", other.name);
    }

    ~MoveableClass() {
        std::print("  Destructor: Deleting {}", name);
        if (data) {
            std::println(" at {}", static_cast<const void*>(data));
            delete[] data;
        } else {
            std::println(" (already moved)");
        }
    }

    void display() const {
        std::print("  {}: ", name);
        if (data) {
            std::println("{} elements at {}", size, static_cast<const void*>(data));
        } else {
            std::println("empty (moved from)");
        }
    }
};

void demonstrateMoveConstructor() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: MOVE CONSTRUCTOR       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating original object ---");
    MoveableClass obj1(5, "Original");
    obj1.display();

    std::println("");
    std::println("--- Move construction with std::move ---");
    MoveableClass obj2(std::move(obj1));  // Move constructor

    std::println("");
    std::println("--- After move ---");
    std::print("obj1 (source): ");
    obj1.display();
    std::print("obj2 (dest):   ");
    obj2.display();

    std::println("");
    std::println("✅ Resources transferred, not copied!");
    std::println("✅ obj1 is empty but valid");
    std::println("✅ Much faster than copying!");

    std::println("");
    std::println("--- Cleanup ---");
}

// ============================================
// EXAMPLE 3: LVALUE vs RVALUE
// ============================================

// Two overloads: the compiler picks one depending on the VALUE CATEGORY of the argument
void whichRef(int&)  { std::println("  -> lvalue reference overload (int&)"); }
void whichRef(int&&) { std::println("  -> rvalue reference overload (int&&)"); }

void demonstrateLvalueRvalue() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: LVALUE vs RVALUE       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    int x = 10;
    int y = 20;

    std::println("--- Which overload does the compiler pick? ---");
    std::print("whichRef(x):            ");
    whichRef(x);             // x has a name -> lvalue
    std::print("whichRef(x + y):        ");
    whichRef(x + y);         // temporary result -> rvalue
    std::print("whichRef(42):           ");
    whichRef(42);            // literal -> rvalue
    std::print("whichRef(std::move(x)): ");
    whichRef(std::move(x));  // std::move casts x to an rvalue

    std::println("");
    std::println("--- A named rvalue reference is an lvalue ---");
    int&& rr = 42;                // rr is declared as an rvalue REFERENCE...
    std::print("whichRef(rr):            ");
    whichRef(rr);              // ...but it has a name, so the expression rr is an lvalue
    std::print("whichRef(std::move(rr)): ");
    whichRef(std::move(rr));   // cast it again to treat it as an rvalue
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
        data = new int[size]();   // () zero-initializes; the copy operations read these values
        std::println("  Constructor: {}", name);
    }

    // Copy constructor
    FullyMoveable(const FullyMoveable& other) : size(other.size), name(other.name + "_Copy") {
        data = new int[size];
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
        std::println("  Copy Constructor");
    }

    // Copy assignment
    FullyMoveable& operator=(const FullyMoveable& other) {
        std::println("  Copy Assignment");
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
        std::println("  Move Constructor");
    }

    // MOVE ASSIGNMENT OPERATOR
    FullyMoveable& operator=(FullyMoveable&& other) noexcept {
        std::println("  Move Assignment");

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
        std::println("  Destructor: {}", name);
        delete[] data;
    }

    void display() const {
        std::println("  {}: {}", name, (data ? "has data" : "empty"));
    }
};

void demonstrateMoveAssignment() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: MOVE ASSIGNMENT        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating objects ---");
    FullyMoveable obj1(5, "First");
    FullyMoveable obj2(3, "Second");

    std::println("");
    std::println("--- Copy assignment (obj3 = obj1) ---");
    FullyMoveable obj3(2, "Third");
    obj3 = obj1;  // Copy assignment

    std::println("");
    std::println("--- Move assignment (obj2 = std::move(obj1)) ---");
    obj2 = std::move(obj1);  // Move assignment

    std::println("");
    std::println("--- After move assignment ---");
    obj1.display();
    obj2.display();

    std::println("");
    std::println("--- Cleanup ---");
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
        data = new int[size]();   // () zero-initializes; the copy operations read these values
        std::println("  1. Constructor");
    }

    // 1. Destructor
    ~RuleOfFive() {
        std::println("  1. Destructor");
        delete[] data;
    }

    // 2. Copy Constructor
    RuleOfFive(const RuleOfFive& other) : size(other.size) {
        data = new int[size];
        copy(other.data, other.data + size, data);
        std::println("  2. Copy Constructor");
    }

    // 3. Copy Assignment
    RuleOfFive& operator=(const RuleOfFive& other) {
        std::println("  3. Copy Assignment");
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
        std::println("  4. Move Constructor");
    }

    // 5. Move Assignment
    RuleOfFive& operator=(RuleOfFive&& other) noexcept {
        std::println("  5. Move Assignment");
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
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 5: RULE OF FIVE           ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");


    std::println("--- Demonstrations ---");
    RuleOfFive obj1(5);                    // Constructor
    RuleOfFive obj2 = obj1;                // Copy constructor
    RuleOfFive obj3(3);
    obj3 = obj1;                           // Copy assignment
    RuleOfFive obj4 = std::move(obj1);     // Move constructor
    RuleOfFive obj5(2);
    obj5 = std::move(obj2);                // Move assignment

    std::println("");
    std::println("--- Cleanup ---");
}

// ============================================
// EXAMPLE 6: STD::MOVE EXPLAINED
// ============================================

class Tracker {
private:
    string name;

public:
    Tracker(string n) : name(n) {
        std::println("  Created {}", name);
    }

    Tracker(const Tracker& other) : name(other.name + "_Copy") {
        std::println("  Copy: {} → {}", other.name, name);
    }

    Tracker(Tracker&& other) noexcept : name(other.name + "_Moved") {
        std::println("  Move: {} → {}", other.name, name);
        other.name = "[moved]";
    }

    ~Tracker() {
        std::println("  Destroyed {}", name);
    }

    string getName() const { return name; }
};

void demonstrateStdMove() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 6: STD::MOVE              ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Without std::move (copy) ---");
    Tracker t1("Original");
    Tracker t2 = t1;  // Copy constructor
    std::println("t1: {}", t1.getName());
    std::println("t2: {}", t2.getName());

    std::println("");
    std::println("--- With std::move (move) ---");
    Tracker t3("Source");
    Tracker t4 = std::move(t3);  // Move constructor
    std::println("t3: {}", t3.getName());
    std::println("t4: {}", t4.getName());

    std::println("");
    std::println("--- Cleanup ---");
}

// ============================================
// EXAMPLE 7: WHEN MOVES HAPPEN AUTOMATICALLY
// ============================================

class AutoMove {
private:
    vector<int> data;
    string name;

public:
    AutoMove(string n, int size) : data(size), name(n) {
        std::println("  Created {}", name);
    }

    AutoMove(const AutoMove& other) : data(other.data), name(other.name + "_Copy") {
        std::println("  Copied {}", name);
    }

    AutoMove(AutoMove&& other) noexcept : data(std::move(other.data)), name(other.name + "_Moved") {
        std::println("  Moved {}", name);
    }

    ~AutoMove() {
        std::println("  Destroyed {}", name);
    }
};

AutoMove createAutoMove() {
    AutoMove temp("Temp", 100);
    return temp;  // NRVO (no move at all) or, if not applied, an implicit move
}

void demonstrateAutomaticMoves() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 7: AUTOMATIC MOVES        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Return from function (NRVO: no copy, no move) ---");
    AutoMove obj1 = createAutoMove();

    std::println("");
    std::println("--- Initializing from temporary (C++17: elided) ---");
    AutoMove obj2 = AutoMove("Temp2", 50);  // C++17: guaranteed elision, nothing is moved

    std::println("");
    std::println("--- Container operations ---");
    vector<AutoMove> vec;
    vec.push_back(AutoMove("Vec1", 10));  // Move into vector

    std::println("");
    std::println("--- Cleanup ---");
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
        data = new int[size]();   // () zero-initializes; the copy constructor reads these values
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
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 8: COPY vs MOVE SPEED     ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    const int SIZE = 1000000;
    using Clock = chrono::steady_clock;

    PerformanceTest original(SIZE);

    auto t0 = Clock::now();
    PerformanceTest copied(original);             // allocates 4 MB and copies 1M ints
    auto t1 = Clock::now();
    PerformanceTest moved(std::move(original));   // copies one pointer and one int
    auto t2 = Clock::now();

    auto copyUs = chrono::duration_cast<chrono::microseconds>(t1 - t0).count();
    auto moveNs = chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count();

    std::println("Copy: {} microseconds ({} bytes copied)", copyUs, SIZE * sizeof(int));
    std::println("Move: {} nanoseconds (one pointer + one int)", moveNs);
    std::println("✅ original is now empty: size = {}", original.getSize());
    std::println("   (timings differ on every run and machine)");
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
        std::println("  Constructed: \"{}\"", buffer);
    }

    // Copy constructor
    String(const String& other) : length(other.length) {
        buffer = new char[length + 1];
        strcpy(buffer, other.buffer);
        std::println("  Copied: \"{}\"", buffer);
    }

    // Move constructor
    String(String&& other) noexcept : buffer(other.buffer), length(other.length) {
        other.buffer = nullptr;
        other.length = 0;
        std::println("  Moved: \"{}\"", buffer);
    }

    // Copy assignment
    String& operator=(const String& other) {
        if (this != &other) {
            delete[] buffer;
            length = other.length;
            buffer = new char[length + 1];
            strcpy(buffer, other.buffer);
            std::println("  Copy assigned: \"{}\"", buffer);
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
            std::println("  Move assigned: \"{}\"", buffer);
        }
        return *this;
    }

    ~String() {
        if (buffer) {
            std::println("  Destroyed: \"{}\"", buffer);
        } else {
            std::println("  Destroyed: [empty]");
        }
        delete[] buffer;
    }

    const char* c_str() const { return buffer ? buffer : ""; }
};

void demonstrateStringMove() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 9: STRING CLASS           ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Copy operations ---");
    String s1("Hello");
    String s2 = s1;  // Copy
    String s3("World");
    s3 = s1;  // Copy assignment

    std::println("");
    std::println("--- Move operations ---");
    String s4 = std::move(s1);  // Move
    String s5("Temp");
    s5 = std::move(s2);  // Move assignment

    std::println("");
    std::println("--- Values ---");
    std::println("s1: \"{}\" (moved from)", s1.c_str());
    std::println("s2: \"{}\" (moved from)", s2.c_str());
    std::println("s3: \"{}\"", s3.c_str());
    std::println("s4: \"{}\" (moved to)", s4.c_str());
    std::println("s5: \"{}\" (moved to)", s5.c_str());

    std::println("");
    std::println("--- Cleanup ---");
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
        std::println("  UniquePtr: Created");
    }

    // Delete copy operations
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    // Move constructor
    UniquePtr(UniquePtr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
        std::println("  UniquePtr: Moved");
    }

    // Move assignment
    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
            std::println("  UniquePtr: Move assigned");
        }
        return *this;
    }

    ~UniquePtr() {
        delete ptr;
        std::println("  UniquePtr: Destroyed");
    }

    T* get() const { return ptr; }
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
};

void demonstrateUniquePtr() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 10: UNIQUE POINTER        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating unique_ptr ---");
    UniquePtr<int> ptr1(new int(42));
    std::println("Value: {}", *ptr1);

    std::println("");
    std::println("--- Cannot copy (deleted) ---");
    // UniquePtr<int> ptr2 = ptr1;  // ❌ Error! Copy deleted
    std::println("⚠️  Copy constructor is deleted");

    std::println("");
    std::println("--- Can move (transfers ownership) ---");
    UniquePtr<int> ptr2 = std::move(ptr1);  // ✅ Move OK
    std::println("ptr2 value: {}", *ptr2);
    std::println("ptr1 is now nullptr");

    std::println("");
    std::println("--- Cleanup ---");
}

// ============================================
// MAIN DEMONSTRATION
// ============================================

int main() {
    std::println("========================================");
    std::println("MOVE SEMANTICS DEMONSTRATION (C++11)");
    std::println("========================================");

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


    return 0;
}
