#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cstring>
using namespace std;

// ============================================
// WHAT ARE DEFAULT AND DELETE?
// ============================================

/*
 * DEFAULT KEYWORD:
 * - Explicitly requests compiler-generated implementation
 * - Used for special member functions
 * - Makes intent explicit
 * - Can restore default behavior
 * - Better performance than manual implementation
 *
 * DELETE KEYWORD:
 * - Explicitly deletes functions
 * - Prevents function usage
 * - Enforces design constraints
 * - Better than private + undefined
 * - Clear compiler errors
 *
 * SPECIAL MEMBER FUNCTIONS:
 * 1. Default constructor
 * 2. Destructor
 * 3. Copy constructor
 * 4. Copy assignment operator
 * 5. Move constructor
 * 6. Move assignment operator
 *
 * RULE OF ZERO:
 * - If you can avoid defining special members, do so
 * - Use RAII types (string, vector, unique_ptr)
 * - Let compiler generate everything
 *
 * RULE OF FIVE:
 * - If you define one, consider all five
 * - Destructor, copy/move constructor, copy/move assignment
 * - Or use = default / = delete
 *
 * SYNTAX:
 * ClassName() = default;           // Request default
 * ClassName(const ClassName&) = delete;  // Delete function
 */

// ============================================
// EXAMPLE 1: DEFAULT CONSTRUCTOR
// ============================================

// Without default keyword (implicit)
class SimpleClass {
private:
    int value;
    string name;

public:
    // Compiler generates default constructor automatically
    // SimpleClass() : value(0), name() {}

    void display() const {
        cout << "  Value: " << value << ", Name: '" << name << "'\n";
    }
};

// With default keyword (explicit)
class ExplicitDefault {
private:
    int value;
    string name;

public:
    // Explicitly request default constructor
    ExplicitDefault() = default;

    // Custom constructor
    ExplicitDefault(int v, const string& n) : value(v), name(n) {}

    void display() const {
        cout << "  Value: " << value << ", Name: '" << name << "'\n";
    }
};

void demonstrateDefaultConstructor() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: DEFAULT CONSTRUCTOR   ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    SimpleClass obj1;
    cout << "--- SimpleClass (implicit default) ---\n";
    obj1.display();

    ExplicitDefault obj2;
    cout << "\n--- ExplicitDefault (= default) ---\n";
    obj2.display();

    ExplicitDefault obj3(42, "Test");
    cout << "\n--- ExplicitDefault (custom constructor) ---\n";
    obj3.display();

    cout << "\n💡 WHY USE = DEFAULT:\n";
    cout << "   • Makes intent explicit\n";
    cout << "   • Documents code better\n";
    cout << "   • Can restore default when deleted\n";
    cout << "   • Better performance\n";
}

// ============================================
// EXAMPLE 2: DELETING COPY OPERATIONS
// ============================================

class NonCopyable {
private:
    int* data;
    string name;

public:
    NonCopyable(const string& n) : name(n) {
        data = new int(42);
        cout << "  NonCopyable created: " << name << "\n";
    }

    ~NonCopyable() {
        if (data) {
            delete data;
            cout << "  NonCopyable destroyed: " << name << "\n";
        }
    }

    // ❌ Delete copy operations
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;

    // ✅ Allow move operations (manual implementation needed!)
    NonCopyable(NonCopyable&& other) noexcept
        : data(other.data), name(move(other.name)) {
        other.data = nullptr;  // Critical: prevent double-delete!
        cout << "  NonCopyable moved\n";
    }

    NonCopyable& operator=(NonCopyable&& other) noexcept {
        if (this != &other) {
            delete data;  // Clean up our own resource
            data = other.data;
            name = move(other.name);
            other.data = nullptr;  // Critical: prevent double-delete!
            cout << "  NonCopyable move-assigned\n";
        }
        return *this;
    }

    void display() const {
        if (data) {
            cout << "  Name: " << name << ", Data: " << *data << "\n";
        } else {
            cout << "  Name: " << name << ", Data: null (moved-from)\n";
        }
    }
};

void demonstrateNonCopyable() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: DELETE COPY OPS       ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    NonCopyable obj1("Original");
    obj1.display();

    // NonCopyable obj2 = obj1;  // ❌ Error - copy deleted!
    // NonCopyable obj3("Test");
    // obj3 = obj1;              // ❌ Error - copy assignment deleted!

    cout << "\n--- Move is allowed ---\n";
    NonCopyable obj2 = move(obj1);  // ✅ Move works!
    obj2.display();

    cout << "\n--- Original after move (moved-from state) ---\n";
    obj1.display();  // obj1 is now empty

    cout << "\n💡 USE CASE:\n";
    cout << "   • Prevent accidental copies\n";
    cout << "   • Unique ownership (like unique_ptr)\n";
    cout << "   • Resource management\n";
    cout << "   • Clear compiler errors\n";
    cout << "   • Move operations must nullify source!\n";
}

// ============================================
// EXAMPLE 3: SINGLETON PATTERN
// ============================================

class Singleton {
private:
    static Singleton* instance;
    int value;

    // Private constructor
    Singleton() : value(0) {
        cout << "  Singleton created\n";
    }

public:
    // Delete copy and move operations
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;

    static Singleton* getInstance() {
        if (!instance) {
            instance = new Singleton();
        }
        return instance;
    }

    void setValue(int v) { value = v; }
    int getValue() const { return value; }

    ~Singleton() {
        cout << "  Singleton destroyed\n";
    }
};

Singleton* Singleton::instance = nullptr;

void demonstrateSingleton() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: SINGLETON PATTERN     ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    Singleton* s1 = Singleton::getInstance();
    s1->setValue(42);

    Singleton* s2 = Singleton::getInstance();
    cout << "--- Both pointers point to same instance ---\n";
    cout << "s1 value: " << s1->getValue() << "\n";
    cout << "s2 value: " << s2->getValue() << "\n";
    cout << "Same instance: " << (s1 == s2 ? "Yes" : "No") << "\n";

    // Singleton copy = *s1;  // ❌ Error - copy deleted!

    cout << "\n💡 SINGLETON BENEFITS:\n";
    cout << "   • Only one instance exists\n";
    cout << "   • Can't copy or move\n";
    cout << "   • Global access point\n";
    cout << "   • Controlled instantiation\n";
}

// ============================================
// EXAMPLE 4: RULE OF FIVE - ALL DEFAULTS
// ============================================

class RuleOfFiveDefault {
private:
    string name;
    vector<int> data;

public:
    RuleOfFiveDefault(const string& n) : name(n) {
        cout << "  Constructor: " << name << "\n";
    }

    // Rule of Five - all default (Rule of Zero)
    ~RuleOfFiveDefault() = default;
    RuleOfFiveDefault(const RuleOfFiveDefault&) = default;
    RuleOfFiveDefault& operator=(const RuleOfFiveDefault&) = default;
    RuleOfFiveDefault(RuleOfFiveDefault&&) = default;
    RuleOfFiveDefault& operator=(RuleOfFiveDefault&&) = default;

    void addData(int value) {
        data.push_back(value);
    }

    void display() const {
        cout << "  Name: " << name << ", Data count: " << data.size() << "\n";
    }
};

void demonstrateRuleOfFiveDefault() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: RULE OF FIVE DEFAULT  ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    RuleOfFiveDefault obj1("Original");
    obj1.addData(1);
    obj1.addData(2);
    obj1.display();

    cout << "\n--- Copy construction ---\n";
    RuleOfFiveDefault obj2 = obj1;  // Copy
    obj2.display();

    cout << "\n--- Move construction ---\n";
    RuleOfFiveDefault obj3 = move(obj1);  // Move
    obj3.display();

    cout << "\n💡 RULE OF ZERO:\n";
    cout << "   • Use RAII types (string, vector)\n";
    cout << "   • No raw pointers\n";
    cout << "   • Let compiler generate everything\n";
    cout << "   • = default makes intent clear\n";
}

// ============================================
// EXAMPLE 5: RULE OF FIVE - CUSTOM
// ============================================

class RuleOfFiveCustom {
private:
    int* data;
    size_t size;
    string name;

public:
    // Constructor
    RuleOfFiveCustom(const string& n, size_t s)
        : name(n), size(s) {
        data = new int[size];
        for (size_t i = 0; i < size; ++i) {
            data[i] = i;
        }
        cout << "  Constructor: " << name << " (size=" << size << ")\n";
    }

    // Destructor
    ~RuleOfFiveCustom() {
        delete[] data;
        cout << "  Destructor: " << name << "\n";
    }

    // Copy constructor
    RuleOfFiveCustom(const RuleOfFiveCustom& other)
        : name(other.name + "_copy"), size(other.size) {
        data = new int[size];
        memcpy(data, other.data, size * sizeof(int));
        cout << "  Copy constructor: " << name << "\n";
    }

    // Copy assignment
    RuleOfFiveCustom& operator=(const RuleOfFiveCustom& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            name = other.name + "_assigned";
            data = new int[size];
            memcpy(data, other.data, size * sizeof(int));
            cout << "  Copy assignment: " << name << "\n";
        }
        return *this;
    }

    // Move constructor
    RuleOfFiveCustom(RuleOfFiveCustom&& other) noexcept
        : name(other.name + "_moved"), size(other.size), data(other.data) {
        other.data = nullptr;
        other.size = 0;
        cout << "  Move constructor: " << name << "\n";
    }

    // Move assignment
    RuleOfFiveCustom& operator=(RuleOfFiveCustom&& other) noexcept {
        if (this != &other) {
            delete[] data;
            name = other.name + "_move_assigned";
            size = other.size;
            data = other.data;
            other.data = nullptr;
            other.size = 0;
            cout << "  Move assignment: " << name << "\n";
        }
        return *this;
    }

    void display() const {
        cout << "  Name: " << name << ", Size: " << size
             << ", Data: " << (data ? "valid" : "null") << "\n";
    }
};

void demonstrateRuleOfFiveCustom() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: RULE OF FIVE CUSTOM   ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    RuleOfFiveCustom obj1("Original", 5);
    obj1.display();

    cout << "\n--- Copy construction ---\n";
    RuleOfFiveCustom obj2 = obj1;
    obj2.display();
    obj1.display();

    cout << "\n--- Copy assignment ---\n";
    RuleOfFiveCustom obj3("Temp", 3);
    obj3 = obj1;
    obj3.display();

    cout << "\n--- Move construction ---\n";
    RuleOfFiveCustom obj4 = move(obj1);
    obj4.display();
    obj1.display();  // obj1 is now empty

    cout << "\n--- Move assignment ---\n";
    RuleOfFiveCustom obj5("Another", 2);
    obj5 = move(obj2);
    obj5.display();
    obj2.display();  // obj2 is now empty

    cout << "\n💡 RULE OF FIVE:\n";
    cout << "   • Define all five if you manage resources\n";
    cout << "   • Or use = default / = delete\n";
    cout << "   • Destructor cleans up\n";
    cout << "   • Copy makes deep copy\n";
    cout << "   • Move transfers ownership\n";
}

// ============================================
// EXAMPLE 6: DELETING SPECIFIC OVERLOADS
// ============================================

class SafeInteger {
private:
    int value;

public:
    SafeInteger(int v) : value(v) {}

    // Allow int
    void setValue(int v) {
        value = v;
        cout << "  setValue(int): " << v << "\n";
    }

    // Delete double to prevent implicit conversion
    void setValue(double) = delete;

    // Delete pointer to prevent accidents
    void setValue(void*) = delete;

    int getValue() const { return value; }
};

void demonstrateDeleteOverloads() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: DELETE OVERLOADS      ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    SafeInteger num(42);

    cout << "--- Setting int value ---\n";
    num.setValue(100);  // ✅ OK
    cout << "Value: " << num.getValue() << "\n";

    // num.setValue(3.14);     // ❌ Error - double deleted!
    // num.setValue(nullptr);  // ❌ Error - pointer deleted!

    cout << "\n💡 USE CASE:\n";
    cout << "   • Prevent implicit conversions\n";
    cout << "   • Type safety\n";
    cout << "   • Clear compiler errors\n";
    cout << "   • Better than runtime checks\n";
}

// ============================================
// EXAMPLE 7: MOVE-ONLY TYPE
// ============================================

class MoveOnlyResource {
private:
    unique_ptr<int[]> data;
    size_t size;
    string name;

public:
    MoveOnlyResource(const string& n, size_t s)
        : name(n), size(s), data(new int[s]) {
        cout << "  Created: " << name << "\n";
    }

    // Delete copy operations
    MoveOnlyResource(const MoveOnlyResource&) = delete;
    MoveOnlyResource& operator=(const MoveOnlyResource&) = delete;

    // Default move operations
    MoveOnlyResource(MoveOnlyResource&&) = default;
    MoveOnlyResource& operator=(MoveOnlyResource&&) = default;

    ~MoveOnlyResource() {
        cout << "  Destroyed: " << name << "\n";
    }

    void display() const {
        cout << "  Name: " << name << ", Size: " << size << "\n";
    }
};

MoveOnlyResource createResource(const string& name) {
    return MoveOnlyResource(name, 100);
}

void demonstrateMoveOnly() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 7: MOVE-ONLY TYPE        ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    MoveOnlyResource res1("Resource1", 10);
    res1.display();

    cout << "\n--- Move construction ---\n";
    MoveOnlyResource res2 = move(res1);
    res2.display();

    cout << "\n--- Move from function ---\n";
    MoveOnlyResource res3 = createResource("Resource3");
    res3.display();

    // MoveOnlyResource res4 = res2;  // ❌ Error - copy deleted!

    cout << "\n💡 MOVE-ONLY BENEFITS:\n";
    cout << "   • Unique ownership\n";
    cout << "   • No accidental copies\n";
    cout << "   • Efficient transfers\n";
    cout << "   • Clear semantics\n";
}

// ============================================
// EXAMPLE 8: DEFAULT ON DECLARATION
// ============================================

class DefaultDeclaration {
public:
    // Declare in class
    DefaultDeclaration();
    ~DefaultDeclaration();
    DefaultDeclaration(const DefaultDeclaration&);
    DefaultDeclaration& operator=(const DefaultDeclaration&);
};

// Define as default outside class
DefaultDeclaration::DefaultDeclaration() = default;
DefaultDeclaration::~DefaultDeclaration() = default;
DefaultDeclaration::DefaultDeclaration(const DefaultDeclaration&) = default;
DefaultDeclaration& DefaultDeclaration::operator=(const DefaultDeclaration&) = default;

void demonstrateDefaultDeclaration() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 8: DEFAULT DECLARATION   ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    DefaultDeclaration obj1;
    DefaultDeclaration obj2 = obj1;

    cout << "Objects created and copied\n";

    cout << "\n💡 FLEXIBILITY:\n";
    cout << "   • Declare in header\n";
    cout << "   • Define in source file\n";
    cout << "   • Separates interface/implementation\n";
    cout << "   • Useful for pImpl idiom\n";
}

// ============================================
// EXAMPLE 9: WHEN NOT TO USE
// ============================================

// ❌ BAD: Unnecessary default
class Unnecessary {
public:
    Unnecessary() = default;  // ❌ Would be generated anyway!
    ~Unnecessary() = default;
    Unnecessary(const Unnecessary&) = default;
    Unnecessary& operator=(const Unnecessary&) = default;
};

// ✅ GOOD: Only when needed
class OnlyWhenNeeded {
public:
    OnlyWhenNeeded(int) {}  // Custom constructor
    OnlyWhenNeeded() = default;  // ✅ Needed - restores default
};

void demonstrateWhenNotToUse() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 9: WHEN NOT TO USE       ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "❌ DON'T USE = DEFAULT WHEN:\n";
    cout << "   • Function would be generated anyway\n";
    cout << "   • No other constructors defined\n";
    cout << "   • Just adding noise\n";
    cout << "   • Over-documenting\n";
    cout << "\n";
    cout << "2. Making intent explicit:\n";
    cout << "   class B {\n";
    cout << "       ~B() = default;   // Explicitly defaulted\n";
    cout << "   };\n";
    cout << "\n";
    cout << "3. Out-of-line definition:\n";
    cout << "   class C {\n";
    cout << "       C();\n";
    cout << "   };\n";
    cout << "   C::C() = default;  // Define outside\n";
    cout << "\n";
    cout << "4. Rule of Five:\n";
    cout << "   // All five = default (Rule of Zero style)\n";

    cout << "\n========================================\n";
    cout << "WHEN TO USE = DELETE\n";
    cout << "========================================\n";
    cout << "✅ USE = DELETE:\n";
    cout << "1. Prevent copying:\n";
    cout << "   class NoCopy {\n";
    cout << "       NoCopy(const NoCopy&) = delete;\n";
    cout << "       NoCopy& operator=(const NoCopy&) = delete;\n";
    cout << "   };\n";
    cout << "\n";
    cout << "2. Prevent implicit conversions:\n";
    cout << "   class SafeInt {\n";
    cout << "       void setValue(int);     // OK\n";
    cout << "       void setValue(double) = delete;  // Prevent\n";
    cout << "   };\n";
    cout << "\n";
    cout << "3. Singleton pattern:\n";
    cout << "   class Singleton {\n";
    cout << "       Singleton(const Singleton&) = delete;\n";
    cout << "       Singleton& operator=(const Singleton&) = delete;\n";
    cout << "   };\n";
    cout << "\n";
    cout << "4. Move-only types:\n";
    cout << "   class MoveOnly {\n";
    cout << "       MoveOnly(const MoveOnly&) = delete;  // No copy\n";
    cout << "       MoveOnly(MoveOnly&&) = default;      // Allow move\n";
    cout << "   };\n";

    cout << "\n========================================\n";
    cout << "COMMON PATTERNS\n";
    cout << "========================================\n";
    cout << "PATTERN 1: Non-Copyable Class\n";
    cout << "class NonCopyable {\n";
    cout << "public:\n";
    cout << "    NonCopyable() = default;\n";
    cout << "    NonCopyable(const NonCopyable&) = delete;\n";
    cout << "    NonCopyable& operator=(const NonCopyable&) = delete;\n";
    cout << "};\n";
    cout << "\n";
    cout << "PATTERN 2: Move-Only Class\n";
    cout << "class MoveOnly {\n";
    cout << "public:\n";
    cout << "    MoveOnly() = default;\n";
    cout << "    MoveOnly(const MoveOnly&) = delete;\n";
    cout << "    MoveOnly& operator=(const MoveOnly&) = delete;\n";
    cout << "    MoveOnly(MoveOnly&&) = default;\n";
    cout << "    MoveOnly& operator=(MoveOnly&&) = default;\n";
    cout << "};\n";
    cout << "\n";
    cout << "PATTERN 3: Interface Base Class\n";
    cout << "class Interface {\n";
    cout << "public:\n";
    cout << "    virtual ~Interface() = default;\n";
    cout << "    virtual void doWork() = 0;\n";
    cout << "protected:\n";
    cout << "    Interface() = default;\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "DEFAULT VS MANUAL IMPLEMENTATION\n";
    cout << "========================================\n";
    cout << "MANUAL IMPLEMENTATION:\n";
    cout << "class Manual {\n";
    cout << "    int x, y;\n";
    cout << "public:\n";
    cout << "    Manual() : x(0), y(0) {}  // Manual\n";
    cout << "};\n";
    cout << "\n";
    cout << "DEFAULT IMPLEMENTATION:\n";
    cout << "class Default {\n";
    cout << "    int x, y;\n";
    cout << "public:\n";
    cout << "    Default() = default;  // Compiler-generated\n";
    cout << "};\n";
    cout << "\n";
    cout << "DIFFERENCES:\n";
    cout << "  • = default may be faster (compiler optimizations)\n";
    cout << "  • = default is trivial (important for some uses)\n";
    cout << "  • Manual gives you control\n";
    cout << "  • Manual can initialize differently\n";

    cout << "\n========================================\n";
    cout << "TRIVIAL TYPES\n";
    cout << "========================================\n";
    cout << "A type is TRIVIAL if:\n";
    cout << "  • Has trivial default constructor\n";
    cout << "  • Has trivial copy/move operations\n";
    cout << "  • Has trivial destructor\n";
    cout << "  • No virtual functions\n";
    cout << "\n";
    cout << "WHY IT MATTERS:\n";
    cout << "  • Can be memcpy'd\n";
    cout << "  • Can be used in unions\n";
    cout << "  • Better performance\n";
    cout << "  • Can use in low-level code\n";
    cout << "\n";
    cout << "class Trivial {\n";
    cout << "    int x;\n";
    cout << "public:\n";
    cout << "    Trivial() = default;  // Trivial!\n";
    cout << "};\n";
    cout << "\n";
    cout << "class NonTrivial {\n";
    cout << "    int x;\n";
    cout << "public:\n";
    cout << "    NonTrivial() : x(0) {}  // Not trivial\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "DELETING INHERITED FUNCTIONS\n";
    cout << "========================================\n";
    cout << "class Base {\n";
    cout << "public:\n";
    cout << "    virtual void func(int) {}\n";
    cout << "};\n";
    cout << "\n";
    cout << "class Derived : public Base {\n";
    cout << "public:\n";
    cout << "    void func(int) override {}\n";
    cout << "    void func(double) = delete;  // Prevent overload\n";
    cout << "};\n";
    cout << "\n";
    cout << "USE CASE:\n";
    cout << "  • Prevent specific overloads\n";
    cout << "  • Block implicit conversions\n";
    cout << "  • Type safety\n";

    cout << "\n========================================\n";
    cout << "BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "✅ DO:\n";
    cout << "  • Prefer Rule of Zero\n";
    cout << "  • Use RAII types (string, vector, unique_ptr)\n";
    cout << "  • Use = default for intent\n";
    cout << "  • Use = delete to prevent operations\n";
    cout << "  • Define all five if you define one\n";
    cout << "  • Make destructors virtual in base classes\n";
    cout << "  • Use = default for trivial types\n";
    cout << "  • Delete copy for move-only types\n";
    cout << "\n";
    cout << "❌ DON'T:\n";
    cout << "  • Over-use = default unnecessarily\n";
    cout << "  • Mix manual and default carelessly\n";
    cout << "  • Forget to handle all five\n";
    cout << "  • Use old-style private + undefined\n";
    cout << "  • Manage raw resources without need\n";
    cout << "  • Make everything non-copyable\n";

    cout << "\n========================================\n";
    cout << "COMMON MISTAKES\n";
    cout << "========================================\n";
    cout << "❌ MISTAKE 1: Unnecessary = default\n";
    cout << "class Bad {\n";
    cout << "public:\n";
    cout << "    Bad() = default;  // ❌ Would be generated anyway!\n";
    cout << "};\n";
    cout << "\n";
    cout << "✅ BETTER:\n";
    cout << "class Good {\n";
    cout << "public:\n";
    cout << "    // Nothing - let compiler generate\n";
    cout << "};\n";
    cout << "\n";
    cout << "❌ MISTAKE 2: Partial Rule of Five\n";
    cout << "class Bad {\n";
    cout << "public:\n";
    cout << "    ~Bad();  // Only destructor defined!\n";
    cout << "    // ❌ Missing copy/move operations\n";
    cout << "};\n";
    cout << "\n";
    cout << "✅ BETTER:\n";
    cout << "class Good {\n";
    cout << "public:\n";
    cout << "    ~Good();\n";
    cout << "    Good(const Good&) = delete;\n";
    cout << "    Good& operator=(const Good&) = delete;\n";
    cout << "    Good(Good&&) = default;\n";
    cout << "    Good& operator=(Good&&) = default;\n";
    cout << "};\n";
    cout << "\n";
    cout << "❌ MISTAKE 3: Deleting destructor\n";
    cout << "class Bad {\n";
    cout << "public:\n";
    cout << "    ~Bad() = delete;  // ❌ Can't destroy objects!\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "QUICK REFERENCE\n";
    cout << "========================================\n";
    cout << "// NON-COPYABLE\n";
    cout << "ClassName(const ClassName&) = delete;\n";
    cout << "ClassName& operator=(const ClassName&) = delete;\n";
    cout << "\n";
    cout << "// MOVE-ONLY\n";
    cout << "ClassName(const ClassName&) = delete;\n";
    cout << "ClassName& operator=(const ClassName&) = delete;\n";
    cout << "ClassName(ClassName&&) = default;\n";
    cout << "ClassName& operator=(ClassName&&) = default;\n";
    cout << "\n";
    cout << "// RULE OF FIVE (all default)\n";
    cout << "~ClassName() = default;\n";
    cout << "ClassName(const ClassName&) = default;\n";
    cout << "ClassName& operator=(const ClassName&) = default;\n";
    cout << "ClassName(ClassName&&) = default;\n";
    cout << "ClassName& operator=(ClassName&&) = default;\n";
    cout << "\n";
    cout << "// RESTORE DEFAULT CONSTRUCTOR\n";
    cout << "ClassName(int);  // Custom constructor\n";
    cout << "ClassName() = default;  // Restore default\n";

    cout << "\n========================================\n";
    cout << "KEY TAKEAWAYS\n";
    cout << "========================================\n";
    cout << "1. = default requests compiler-generated version\n";
    cout << "2. = delete prevents function usage\n";
    cout << "3. Prefer Rule of Zero (use RAII types)\n";
    cout << "4. If you define one, define all five\n";
    cout << "5. Use = delete for non-copyable types\n";
    cout << "6. Use = delete to prevent conversions\n";
    cout << "7. = default can restore deleted functions\n";
    cout << "8. = default makes intent explicit\n";
    cout << "9. Move-only types: delete copy, default move\n";
    cout << "10. Better errors than private + undefined\n";

    cout << "\n========================================\n";
    cout << "DECISION TREE\n";
    cout << "========================================\n";
    cout << "Managing resources?\n";
    cout << "  NO  → Use Rule of Zero (do nothing)\n";
    cout << "  YES → Can you use unique_ptr/string/vector?\n";
    cout << "         YES → Use Rule of Zero\n";
    cout << "         NO  → Use Rule of Five\n";
    cout << "\n";
    cout << "Want to prevent copying?\n";
    cout << "  YES → Delete copy operations\n";
    cout << "         Allow move? YES → = default\n";
    cout << "                     NO  → = delete\n";
    cout << "\n";
    cout << "Custom constructor defined?\n";
    cout << "  YES → Want default constructor too?\n";
    cout << "         YES → ClassName() = default;\n";
    cout << "         NO  → Do nothing\n";

    cout << "\n";


    cout << "✅ DO USE = DEFAULT WHEN:\n";
    cout << "   • Restoring deleted function\n";
    cout << "   • Making intent explicit\n";
    cout << "   • Part of Rule of Five\n";
    cout << "   • Out-of-line definition\n";
    cout << "\n";
    cout << "✅ DO USE = DELETE WHEN:\n";
    cout << "   • Preventing operations\n";
    cout << "   • Enforcing constraints\n";
    cout << "   • Type safety\n";
    cout << "   • Clear design intent\n";
}

// ============================================
// EXAMPLE 10: REAL-WORLD - FILE HANDLE
// ============================================

class FileHandle {
private:
    FILE* file;
    string filename;

public:
    FileHandle(const string& name) : filename(name) {
        file = fopen(name.c_str(), "r");
        if (file) {
            cout << "  File opened: " << filename << "\n";
        } else {
            cout << "  Failed to open: " << filename << "\n";
        }
    }

    ~FileHandle() {
        if (file) {
            fclose(file);
            cout << "  File closed: " << filename << "\n";
        }
    }

    // Delete copy - file handles shouldn't be copied
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;

    // Allow move - transfer ownership
    FileHandle(FileHandle&& other) noexcept
        : file(other.file), filename(move(other.filename)) {
        other.file = nullptr;
        cout << "  File handle moved: " << filename << "\n";
    }

    FileHandle& operator=(FileHandle&& other) noexcept {
        if (this != &other) {
            if (file) {
                fclose(file);
            }
            file = other.file;
            filename = move(other.filename);
            other.file = nullptr;
            cout << "  File handle move-assigned: " << filename << "\n";
        }
        return *this;
    }

    bool isOpen() const { return file != nullptr; }

    void display() const {
        cout << "  File: " << filename
             << " (" << (isOpen() ? "open" : "closed") << ")\n";
    }
};

void demonstrateFileHandle() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 10: FILE HANDLE          ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "--- Creating file handle ---\n";
    FileHandle handle1("test.txt");
    handle1.display();

    // FileHandle handle2 = handle1;  // ❌ Error - copy deleted!

    cout << "\n--- Moving file handle ---\n";
    FileHandle handle2 = move(handle1);
    handle2.display();
    handle1.display();  // Moved-from state

    cout << "\n💡 REAL-WORLD PATTERN:\n";
    cout << "   • Unique resource ownership\n";
    cout << "   • RAII (Resource Acquisition Is Initialization)\n";
    cout << "   • No accidental copies\n";
    cout << "   • Safe transfer of ownership\n";
    cout << "   • Similar to unique_ptr\n";
}

// ============================================
// MAIN FUNCTION - RUN ALL EXAMPLES
// ============================================

int main() {
    cout << "\n";
    cout << "╔═══════════════════════════════════════════════════════════╗\n";
    cout << "║                                                           ║\n";
    cout << "║           DEFAULT AND DELETE KEYWORDS IN C++             ║\n";
    cout << "║              Special Member Functions                     ║\n";
    cout << "║                                                           ║\n";
    cout << "╚═══════════════════════════════════════════════════════════╝\n";

    demonstrateDefaultConstructor();
    demonstrateNonCopyable();
    demonstrateSingleton();
    demonstrateRuleOfFiveDefault();
    demonstrateRuleOfFiveCustom();
    demonstrateDeleteOverloads();
    demonstrateMoveOnly();
    demonstrateDefaultDeclaration();
    demonstrateWhenNotToUse();
    demonstrateFileHandle();

    // ============================================
    // COMPREHENSIVE EXPLANATION
    // ============================================

    cout << "\n\n";
    cout << "╔═══════════════════════════════════════════════════════════╗\n";
    cout << "║                                                           ║\n";
    cout << "║              COMPREHENSIVE EXPLANATION                    ║\n";
    cout << "║                                                           ║\n";
    cout << "╚═══════════════════════════════════════════════════════════╝\n";

    cout << "\n========================================\n";
    cout << "WHAT ARE DEFAULT AND DELETE?\n";
    cout << "========================================\n";
    cout << "= DEFAULT:\n";
    cout << "  • Explicitly requests compiler-generated version\n";
    cout << "  • Used for special member functions\n";
    cout << "  • Makes intent clear\n";
    cout << "  • Can restore deleted functions\n";
    cout << "  • Better performance\n";
    cout << "\n";
    cout << "= DELETE:\n";
    cout << "  • Explicitly deletes functions\n";
    cout << "  • Prevents function usage\n";
    cout << "  • Enforces design constraints\n";
    cout << "  • Clear compiler errors\n";
    cout << "  • Better than private + undefined\n";
    cout << "\n";
    cout << "SYNTAX:\n";
    cout << "class MyClass {\n";
    cout << "public:\n";
    cout << "    MyClass() = default;              // Request default\n";
    cout << "    MyClass(const MyClass&) = delete; // Delete copy\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "SPECIAL MEMBER FUNCTIONS\n";
    cout << "========================================\n";
    cout << "THE BIG SIX:\n";
    cout << "1. Default Constructor\n";
    cout << "   MyClass();\n";
    cout << "\n";
    cout << "2. Destructor\n";
    cout << "   ~MyClass();\n";
    cout << "\n";
    cout << "3. Copy Constructor\n";
    cout << "   MyClass(const MyClass&);\n";
    cout << "\n";
    cout << "4. Copy Assignment Operator\n";
    cout << "   MyClass& operator=(const MyClass&);\n";
    cout << "\n";
    cout << "5. Move Constructor\n";
    cout << "   MyClass(MyClass&&);\n";
    cout << "\n";
    cout << "6. Move Assignment Operator\n";
    cout << "   MyClass& operator=(MyClass&&);\n";

    cout << "\n========================================\n";
    cout << "RULE OF ZERO\n";
    cout << "========================================\n";
    cout << "BEST PRACTICE:\n";
    cout << "If you can avoid defining special members, DO SO!\n";
    cout << "\n";
    cout << "class RuleOfZero {\n";
    cout << "private:\n";
    cout << "    string name;      // RAII type\n";
    cout << "    vector<int> data; // RAII type\n";
    cout << "    \n";
    cout << "public:\n";
    cout << "    // No special members needed!\n";
    cout << "    // Compiler generates everything correctly\n";
    cout << "};\n";
    cout << "\n";
    cout << "WHY IT WORKS:\n";
    cout << "  • string and vector manage their own memory\n";
    cout << "  • No raw pointers\n";
    cout << "  • No manual resource management\n";
    cout << "  • Compiler-generated functions work perfectly\n";

    cout << "\n========================================\n";
    cout << "RULE OF FIVE\n";
    cout << "========================================\n";
    cout << "IF YOU DEFINE ONE, DEFINE ALL FIVE:\n";
    cout << "class RuleOfFive {\n";
    cout << "public:\n";
    cout << "    ~RuleOfFive();                              // 1\n";
    cout << "    RuleOfFive(const RuleOfFive&);             // 2\n";
    cout << "    RuleOfFive& operator=(const RuleOfFive&);  // 3\n";
    cout << "    RuleOfFive(RuleOfFive&&);                  // 4\n";
    cout << "    RuleOfFive& operator=(RuleOfFive&&);       // 5\n";
    cout << "};\n";
    cout << "\n";
    cout << "OR USE = DEFAULT / = DELETE:\n";
    cout << "class RuleOfFiveDefault {\n";
    cout << "public:\n";
    cout << "    ~RuleOfFiveDefault() = default;\n";
    cout << "    RuleOfFiveDefault(const RuleOfFiveDefault&) = default;\n";
    cout << "    RuleOfFiveDefault& operator=(const RuleOfFiveDefault&) = default;\n";
    cout << "    RuleOfFiveDefault(RuleOfFiveDefault&&) = default;\n";
    cout << "    RuleOfFiveDefault& operator=(RuleOfFiveDefault&&) = default;\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "WHEN TO USE = DEFAULT\n";
    cout << "========================================\n";
    cout << "✅ USE = DEFAULT:\n";
    cout << "1. Restoring default behavior:\n";
    cout << "   class A {\n";
    cout << "       A(int);           // Custom constructor\n";
    cout << "       A() = default;    // Restore default constructor\n";
    cout << "   };\n";
    cout << "\n";
    cout << "2. Making intent explicit:\n";
    cout << "   class B {\n";
    cout << "       ~B() = default;   // Explicitly defaulted\n";
    cout << "   };\n";
    cout << "\n";
    cout << "3. Out-of-line definition:\n";
    cout << "   class C {\n";
    cout << "       C();\n";
    cout << "   };\n";
    cout << "   C::C() = default;  // Define outside\n";
    cout << "\n";
    cout << "4. Rule of Five:\n";
    cout << "   // All five = default (Rule of Zero style)\n";

    cout << "\n========================================\n";
    cout << "WHEN TO USE = DELETE\n";
    cout << "========================================\n";
    cout << "✅ USE = DELETE:\n";
    cout << "1. Prevent copying:\n";
    cout << "   class NoCopy {\n";
    cout << "       NoCopy(const NoCopy&) = delete;\n";
    cout << "       NoCopy& operator=(const NoCopy&) = delete;\n";
    cout << "   };\n";
    cout << "\n";
    cout << "2. Prevent implicit conversions:\n";
    cout << "   class SafeInt {\n";
    cout << "       void setValue(int);     // OK\n";
    cout << "       void setValue(double) = delete;  // Prevent\n";
    cout << "   };\n";
    cout << "\n";
    cout << "3. Singleton pattern:\n";
    cout << "   class Singleton {\n";
    cout << "       Singleton(const Singleton&) = delete;\n";
    cout << "       Singleton& operator=(const Singleton&) = delete;\n";
    cout << "   };\n";
    cout << "\n";
    cout << "4. Move-only types:\n";
    cout << "   class MoveOnly {\n";
    cout << "       MoveOnly(const MoveOnly&) = delete;  // No copy\n";
    cout << "       MoveOnly(MoveOnly&&) = default;      // Allow move\n";
    cout << "   };\n";

    cout << "\n========================================\n";
    cout << "COMMON PATTERNS\n";
    cout << "========================================\n";
    cout << "PATTERN 1: Non-Copyable Class\n";
    cout << "class NonCopyable {\n";
    cout << "public:\n";
    cout << "    NonCopyable() = default;\n";
    cout << "    NonCopyable(const NonCopyable&) = delete;\n";
    cout << "    NonCopyable& operator=(const NonCopyable&) = delete;\n";
    cout << "};\n";
    cout << "\n";
    cout << "PATTERN 2: Move-Only Class\n";
    cout << "class MoveOnly {\n";
    cout << "public:\n";
    cout << "    MoveOnly() = default;\n";
    cout << "    MoveOnly(const MoveOnly&) = delete;\n";
    cout << "    MoveOnly& operator=(const MoveOnly&) = delete;\n";
    cout << "    MoveOnly(MoveOnly&&) = default;\n";
    cout << "    MoveOnly& operator=(MoveOnly&&) = default;\n";
    cout << "};\n";
    cout << "\n";
    cout << "PATTERN 3: Interface Base Class\n";
    cout << "class Interface {\n";
    cout << "";
    cout << "public:\n";
    cout << "    virtual ~Interface() = default;\n";
    cout << "    virtual void doWork() = 0;\n";
    cout << "protected:\n";
    cout << "    Interface() = default;\n";
    cout << "};\n";
    cout << "\n";
    cout << "USE CASE:\n";
    cout << "  • Prevent specific overloads\n";
    cout << "  • Block implicit conversions\n";
    cout << "  • Type safety\n";
    cout << "\n========================================\n";
    cout << "BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "✅ DO:\n";
    cout << "  • Prefer Rule of Zero\n";
    cout << "  • Use RAII types (string, vector, unique_ptr)\n";
    cout << "  • Use = default for intent\n";
    cout << "  • Use = delete to prevent operations\n";
    cout << "  • Define all five if you define one\n";
    cout << "  • Make destructors virtual in base classes\n";
    cout << "  • Use = default for trivial types\n";
    cout << "  • Delete copy for move-only types\n";
    cout << "\n";
    cout << "❌ DON'T:\n";
    cout << "  • Over-use = default unnecessarily\n";
    cout << "  • Mix manual and default carelessly\n";
    cout << "  • Forget to handle all five\n";
    cout << "  • Use old-style private + undefined\n";
    cout << "  • Manage raw resources without need\n";
    cout << "  • Make everything non-copyable\n";
    cout << "\n========================================\n";
    cout << "COMMON MISTAKES\n";
    cout << "========================================\n";
    cout << "❌ MISTAKE 1: Unnecessary = default\n";
    cout << "class Bad {\n";
    cout << "public:\n";
    cout << "    Bad() = default;  // ❌ Would be generated anyway!\n";
    cout << "};\n";
    cout << "\n";
    cout << "✅ BETTER:\n";
    cout << "class Good {\n";
    cout << "public:\n";
    cout << "    // Nothing - let compiler generate\n";
    cout << "};\n";
    cout << "\n";
    cout << "❌ MISTAKE 2: Partial Rule of Five\n";
    cout << "class Bad {\n";
    cout << "public:\n";
    cout << "    ~Bad();  // Only destructor defined!\n";
    cout << "    // ❌ Missing copy/move operations\n";
    cout << "};\n";
    cout << "\n";
    cout << "✅ BETTER:\n";
    cout << "class Good {\n";
    cout << "public:\n";
    cout << "    ~Good();\n";
    cout << "    Good(const Good&) = delete;\n";
    cout << "    Good& operator=(const Good&) = delete;\n";
    cout << "    Good(Good&&) = default;\n";
    cout << "    Good& operator=(Good&&) = default;\n";
    cout << "};\n";
    cout << "\n";
    cout << "❌ MISTAKE 3: Deleting destructor\n";
    cout << "class Bad {\n";
    cout << "public:\n";
    cout << "    ~Bad() = delete;  // ❌ Can't destroy objects!\n";
    cout << "};\n";
    cout << "\n========================================\n";
    cout << "QUICK REFERENCE\n";
    cout << "========================================\n";
    cout << "// NON-COPYABLE\n";
    cout << "ClassName(const ClassName&) = delete;\n";
    cout << "ClassName& operator=(const ClassName&) = delete;\n";
    cout << "\n";
    cout << "// MOVE-ONLY\n";
    cout << "ClassName(const ClassName&) = delete;\n";
    cout << "ClassName& operator=(const ClassName&) = delete;\n";
    cout << "ClassName(ClassName&&) = default;\n";
    cout << "ClassName& operator=(ClassName&&) = default;\n";
    cout << "\n";
    cout << "// RULE OF FIVE (all default)\n";
    cout << "~ClassName() = default;\n";
    cout << "ClassName(const ClassName&) = default;\n";
    cout << "ClassName& operator=(const ClassName&) = default;\n";
    cout << "ClassName(ClassName&&) = default;\n";
    cout << "ClassName& operator=(ClassName&&) = default;\n";
    cout << "\n";
    cout << "// RESTORE DEFAULT CONSTRUCTOR\n";
    cout << "ClassName(int);  // Custom constructor\n";
    cout << "ClassName() = default;  // Restore default\n";
    cout << "\n========================================\n";
    cout << "KEY TAKEAWAYS\n";
    cout << "========================================\n";
    cout << "1. = default requests compiler-generated version\n";
    cout << "2. = delete prevents function usage\n";
    cout << "3. Prefer Rule of Zero (use RAII types)\n";
    cout << "4. If you define one, define all five\n";
    cout << "5. Use = delete for non-copyable types\n";
    cout << "6. Use = delete to prevent conversions\n";
    cout << "7. = default can restore deleted functions\n";
    cout << "8. = default makes intent explicit\n";
    cout << "9. Move-only types: delete copy, default move\n";
    cout << "10. Better errors than private + undefined\n";
    cout << "\n========================================\n";
    cout << "DECISION TREE\n";
    cout << "========================================\n";
    cout << "Managing resources?\n";
    cout << "  NO  → Use Rule of Zero (do nothing)\n";
    cout << "  YES → Can you use unique_ptr/string/vector?\n";
    cout << "         YES → Use Rule of Zero\n";
    cout << "         NO  → Use Rule of Five\n";
    cout << "\n";
    cout << "Want to prevent copying?\n";
    cout << "  YES → Delete copy operations\n";
    cout << "         Allow move? YES → = default\n";
    cout << "                     NO  → = delete\n";
    cout << "\n";
    cout << "Custom constructor defined?\n";
    cout << "  YES → Want default constructor too?\n";
    cout << "         YES → ClassName() = default;\n";
    cout << "         NO  → Do nothing\n";
    cout << "\n";
    return 0;
}
