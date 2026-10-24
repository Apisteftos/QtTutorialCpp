#include <print>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <cstdio>
#include <utility>
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
 * - Keeps a type trivial where possible (see the README)
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
    int value = 0;  // default member initializer: without it 'value' would be indeterminate
    string name;

public:
    // The compiler generates the default constructor automatically.
    // It default-constructs 'name' and applies the initializer to 'value'.

    void display() const {
        std::println("  Value: {}, Name: '{}'", value, name);
    }
};

// With default keyword (explicit)
class ExplicitDefault {
private:
    int value = 0;
    string name;

public:
    // Explicitly request default constructor
    ExplicitDefault() = default;

    // Custom constructor
    ExplicitDefault(int v, const string& n) : value(v), name(n) {}

    void display() const {
        std::println("  Value: {}, Name: '{}'", value, name);
    }
};

void demonstrateDefaultConstructor() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 1: DEFAULT CONSTRUCTOR   ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    SimpleClass obj1;
    std::println("--- SimpleClass (implicit default) ---");
    obj1.display();

    ExplicitDefault obj2;
    std::println("");
    std::println("--- ExplicitDefault (= default) ---");
    obj2.display();

    ExplicitDefault obj3(42, "Test");
    std::println("");
    std::println("--- ExplicitDefault (custom constructor) ---");
    obj3.display();
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
        std::println("  NonCopyable created: {}", name);
    }

    ~NonCopyable() {
        if (data) {
            delete data;
            std::println("  NonCopyable destroyed: {}", name);
        }
    }

    // ❌ Delete copy operations
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;

    // ✅ Allow move operations (manual implementation needed!)
    NonCopyable(NonCopyable&& other) noexcept
        : data(other.data), name(move(other.name)) {
        other.data = nullptr;  // Critical: prevent double-delete!
        std::println("  NonCopyable moved");
    }

    NonCopyable& operator=(NonCopyable&& other) noexcept {
        if (this != &other) {
            delete data;  // Clean up our own resource
            data = other.data;
            name = move(other.name);
            other.data = nullptr;  // Critical: prevent double-delete!
            std::println("  NonCopyable move-assigned");
        }
        return *this;
    }

    void display() const {
        if (data) {
            std::println("  Name: {}, Data: {}", name, *data);
        } else {
            std::println("  Name: {}, Data: null (moved-from)", name);
        }
    }
};

void demonstrateNonCopyable() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 2: DELETE COPY OPS       ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    NonCopyable obj1("Original");
    obj1.display();

    // NonCopyable obj2 = obj1;  // ❌ Error - copy deleted!
    // NonCopyable obj3("Test");
    // obj3 = obj1;              // ❌ Error - copy assignment deleted!

    std::println("");
    std::println("--- Move is allowed ---");
    NonCopyable obj2 = move(obj1);  // ✅ Move works!
    obj2.display();

    std::println("");
    std::println("--- Original after move (moved-from state) ---");
    obj1.display();  // obj1 is now empty
}

// ============================================
// EXAMPLE 3: SINGLETON PATTERN
// ============================================

class Singleton {
private:
    int value;

    // Private constructor
    Singleton() : value(0) {
        std::println("  Singleton created");
    }

public:
    // Delete copy and move operations
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;

    // Meyers singleton: created on the first call, destroyed at program exit.
    // Since C++11 the initialization of a function-local static is thread-safe.
    static Singleton& getInstance() {
        static Singleton instance;
        return instance;
    }

    void setValue(int v) { value = v; }
    int getValue() const { return value; }

    ~Singleton() {
        std::println("  Singleton destroyed");
    }
};

void demonstrateSingleton() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 3: SINGLETON PATTERN     ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    Singleton& s1 = Singleton::getInstance();
    s1.setValue(42);

    Singleton& s2 = Singleton::getInstance();
    std::println("--- Both references refer to the same instance ---");
    std::println("s1 value: {}", s1.getValue());
    std::println("s2 value: {}", s2.getValue());
    std::println("Same instance: {}", (&s1 == &s2 ? "Yes" : "No"));

    // Singleton copy = s1;  // ❌ Error - copy deleted!
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
        std::println("  Constructor: {}", name);
    }

    // All five explicitly defaulted (the Rule of Zero would be to declare none of them)
    ~RuleOfFiveDefault() = default;
    RuleOfFiveDefault(const RuleOfFiveDefault&) = default;
    RuleOfFiveDefault& operator=(const RuleOfFiveDefault&) = default;
    RuleOfFiveDefault(RuleOfFiveDefault&&) = default;
    RuleOfFiveDefault& operator=(RuleOfFiveDefault&&) = default;

    void addData(int value) {
        data.push_back(value);
    }

    void display() const {
        std::println("  Name: {}, Data count: {}", name, data.size());
    }
};

void demonstrateRuleOfFiveDefault() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 4: RULE OF FIVE DEFAULT  ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    RuleOfFiveDefault obj1("Original");
    obj1.addData(1);
    obj1.addData(2);
    obj1.display();

    std::println("");
    std::println("--- Copy construction ---");
    RuleOfFiveDefault obj2 = obj1;  // Copy
    obj2.display();

    std::println("");
    std::println("--- Move construction ---");
    RuleOfFiveDefault obj3 = move(obj1);  // Move
    obj3.display();
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
    // Constructor (the initializer list follows the declaration order: data, size, name)
    RuleOfFiveCustom(const string& n, size_t s)
        : data(new int[s]), size(s), name(n) {
        for (size_t i = 0; i < size; ++i) {
            data[i] = static_cast<int>(i);
        }
        std::println("  Constructor: {} (size={})", name, size);
    }

    // Destructor
    ~RuleOfFiveCustom() {
        delete[] data;
        std::println("  Destructor: {}", name);
    }

    // Copy constructor
    RuleOfFiveCustom(const RuleOfFiveCustom& other)
        : data(new int[other.size]), size(other.size), name(other.name + "_copy") {
        std::copy(other.data, other.data + size, data);
        std::println("  Copy constructor: {}", name);
    }

    // Copy assignment
    RuleOfFiveCustom& operator=(const RuleOfFiveCustom& other) {
        if (this != &other) {
            int* newData = new int[other.size];  // allocate first: if new throws, *this stays valid
            std::copy(other.data, other.data + other.size, newData);
            delete[] data;
            data = newData;
            size = other.size;
            name = other.name + "_assigned";
            std::println("  Copy assignment: {}", name);
        }
        return *this;
    }

    // Move constructor
    // (the name suffix only makes the demo output visible; a real move would std::move the name)
    RuleOfFiveCustom(RuleOfFiveCustom&& other) noexcept
        : data(other.data), size(other.size), name(other.name + "_moved") {
        other.data = nullptr;
        other.size = 0;
        std::println("  Move constructor: {}", name);
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
            std::println("  Move assignment: {}", name);
        }
        return *this;
    }

    void display() const {
        std::println("  Name: {}, Size: {}, Data: {}", name, size, (data ? "valid" : "null"));
    }
};

void demonstrateRuleOfFiveCustom() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 5: RULE OF FIVE CUSTOM   ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    RuleOfFiveCustom obj1("Original", 5);
    obj1.display();

    std::println("");
    std::println("--- Copy construction ---");
    RuleOfFiveCustom obj2 = obj1;
    obj2.display();
    obj1.display();

    std::println("");
    std::println("--- Copy assignment ---");
    RuleOfFiveCustom obj3("Temp", 3);
    obj3 = obj1;
    obj3.display();

    std::println("");
    std::println("--- Move construction ---");
    RuleOfFiveCustom obj4 = move(obj1);
    obj4.display();
    obj1.display();  // obj1 is now empty

    std::println("");
    std::println("--- Move assignment ---");
    RuleOfFiveCustom obj5("Another", 2);
    obj5 = move(obj2);
    obj5.display();
    obj2.display();  // obj2 is now empty
}

// ============================================
// EXAMPLE 6: DELETING SPECIFIC OVERLOADS
// ============================================

class SafeInteger {
private:
    int value;

public:
    SafeInteger(int v) : value(v) {}
    SafeInteger(double) = delete;  // otherwise SafeInteger(3.14) would silently truncate

    // Allow int
    void setValue(int v) {
        value = v;
        std::println("  setValue(int): {}", v);
    }

    // Delete double to prevent implicit conversion
    void setValue(double) = delete;

    // Delete pointer to prevent accidents
    void setValue(void*) = delete;

    int getValue() const { return value; }
};

void demonstrateDeleteOverloads() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 6: DELETE OVERLOADS      ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    SafeInteger num(42);

    std::println("--- Setting int value ---");
    num.setValue(100);  // ✅ OK
    std::println("Value: {}", num.getValue());

    // num.setValue(3.14);     // ❌ Error - double deleted!
    // num.setValue(nullptr);  // ❌ Error - pointer deleted!
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
        : data(make_unique<int[]>(s)), size(s), name(n) {
        std::println("  Created: {}", name);
    }

    // Delete copy operations
    MoveOnlyResource(const MoveOnlyResource&) = delete;
    MoveOnlyResource& operator=(const MoveOnlyResource&) = delete;

    // Default move operations
    MoveOnlyResource(MoveOnlyResource&&) = default;
    MoveOnlyResource& operator=(MoveOnlyResource&&) = default;

    ~MoveOnlyResource() {
        std::println("  Destroyed: {}", name);
    }

    void display() const {
        std::println("  Name: {}, Size: {}", name, size);
    }
};

MoveOnlyResource createResource(const string& name) {
    return MoveOnlyResource(name, 100);
}

void demonstrateMoveOnly() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 7: MOVE-ONLY TYPE        ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    MoveOnlyResource res1("Resource1", 10);
    res1.display();

    std::println("");
    std::println("--- Move construction ---");
    MoveOnlyResource res2 = move(res1);
    res2.display();

    std::println("");
    std::println("--- Return from a function (C++17: built in place, no move) ---");
    MoveOnlyResource res3 = createResource("Resource3");
    res3.display();

    // MoveOnlyResource res4 = res2;  // ❌ Error - copy deleted!
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
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 8: DEFAULT DECLARATION   ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    DefaultDeclaration obj1;
    DefaultDeclaration obj2 = obj1;

    std::println("Objects created and copied");
}

// ============================================
// EXAMPLE 9: WHEN NOT TO USE
// ============================================

// A member that reports when it is copied or moved
struct Probe {
    Probe() = default;
    Probe(const Probe&) { std::println("    Probe copied"); }
    Probe(Probe&&) noexcept { std::println("    Probe moved"); }
    Probe& operator=(const Probe&) = default;
    Probe& operator=(Probe&&) = default;
};

// ✅ GOOD: declares nothing, so the compiler generates all six
class Plain {
    Probe probe;
};

// ❌ BAD: "unnecessary" = default on the destructor and the copy operations.
// Declaring them SUPPRESSES the implicit move operations, so a move silently copies!
class Unnecessary {
    Probe probe;

public:
    Unnecessary() = default;
    ~Unnecessary() = default;
    Unnecessary(const Unnecessary&) = default;
    Unnecessary& operator=(const Unnecessary&) = default;
};

// ✅ GOOD: = default is needed here, because the custom constructor suppresses the default one
class OnlyWhenNeeded {
public:
    OnlyWhenNeeded(int) {}
    OnlyWhenNeeded() = default;
};

void demonstrateWhenNotToUse() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 9: WHEN NOT TO USE       ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    std::println("--- No special members declared ---");
    Plain plain1;
    Plain plain2 = move(plain1);
    (void)plain2;

    std::println("");
    std::println("--- 'Unnecessary' = default on destructor and copy operations ---");
    Unnecessary unn1;
    Unnecessary unn2 = move(unn1);
    (void)unn2;

    std::println("");
    std::println("--- = default needed to restore the default constructor ---");
    OnlyWhenNeeded withArg(5);
    OnlyWhenNeeded noArg;
    (void)withArg;
    (void)noArg;
    std::println("  Both objects created");
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
            std::println("  File opened: {}", filename);
        } else {
            std::println("  Failed to open: {}", filename);
        }
    }

    ~FileHandle() {
        if (file) {
            fclose(file);
            std::println("  File closed: {}", filename);
        }
    }

    // Delete copy - file handles shouldn't be copied
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;

    // Allow move - transfer ownership
    FileHandle(FileHandle&& other) noexcept
        : file(other.file), filename(move(other.filename)) {
        other.file = nullptr;
        std::println("  File handle moved: {}", filename);
    }

    FileHandle& operator=(FileHandle&& other) noexcept {
        if (this != &other) {
            if (file) {
                fclose(file);
            }
            file = other.file;
            filename = move(other.filename);
            other.file = nullptr;
            std::println("  File handle move-assigned: {}", filename);
        }
        return *this;
    }

    bool isOpen() const { return file != nullptr; }

    void display() const {
        std::println("  File: {} ({})", filename, (isOpen() ? "open" : "closed"));
    }
};

void demonstrateFileHandle() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 10: FILE HANDLE          ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    // Create a small file so that opening succeeds
    FILE* tmp = fopen("test.txt", "w");
    if (tmp) {
        fputs("hello\n", tmp);
        fclose(tmp);
    }

    {
        std::println("--- Creating file handle ---");
        FileHandle handle1("test.txt");
        handle1.display();

        // FileHandle handle2 = handle1;  // ❌ Error - copy deleted!

        std::println("");
        std::println("--- Moving file handle ---");
        FileHandle handle2 = move(handle1);
        handle2.display();
        handle1.display();  // Moved-from state

        std::println("");
        std::println("--- End of block: the file is closed exactly once ---");
    }

    std::remove("test.txt");
}

// ============================================
// MAIN FUNCTION - RUN ALL EXAMPLES
// ============================================

int main() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════════════╗");
    std::println("║                                                           ║");
    std::println("║           DEFAULT AND DELETE KEYWORDS IN C++             ║");
    std::println("║              Special Member Functions                     ║");
    std::println("║                                                           ║");
    std::println("╚═══════════════════════════════════════════════════════════╝");

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

    return 0;
}
