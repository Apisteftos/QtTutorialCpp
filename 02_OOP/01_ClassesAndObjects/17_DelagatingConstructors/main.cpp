#include <iostream>
#include <string>
#include <vector>
#include <ctime>
using namespace std;

// ============================================
// WHAT ARE DELEGATING CONSTRUCTORS?
// ============================================

/*
 * DELEGATING CONSTRUCTORS (C++11):
 * - One constructor calls another constructor
 * - Reduces code duplication
 * - Centralizes initialization logic
 * - Improves maintainability
 * - Part of member initializer list
 *
 * WHY DELEGATING CONSTRUCTORS?
 * - Avoid code duplication
 * - Single source of truth for initialization
 * - Easier to maintain and modify
 * - Cleaner, more readable code
 * - Reduces chance of errors
 *
 * SYNTAX:
 * ClassName() : ClassName(default_values) { }
 *
 * RULES:
 * - Delegation must be the ONLY thing in initializer list
 * - Can't delegate and initialize members together
 * - Delegating constructor called first, then body
 * - No circular delegation allowed
 *
 * BEFORE C++11:
 * - Had to use init() helper functions
 * - Code duplication across constructors
 * - More error-prone
 *
 * BEST PRACTICES:
 * - Delegate to most complete constructor
 * - Put common logic in one place
 * - Use for default parameter simulation
 * - Keep delegation chains simple
 */

// ============================================
// EXAMPLE 1: THE PROBLEM - WITHOUT DELEGATION
// ============================================

class EmployeeBad {
private:
    string name;
    int id;
    double salary;
    string department;

public:
    // ❌ Code duplication - same validation in each constructor!
    EmployeeBad(const string& n, int i, double s, const string& d)
        : name(n), id(i), salary(s), department(d) {
        cout << "  Full constructor called\n";
        // Validation logic
        if (salary < 0) salary = 0;
        if (name.empty()) name = "Unknown";
        if (department.empty()) department = "General";
    }

    EmployeeBad(const string& n, int i, double s)
        : name(n), id(i), salary(s), department("General") {
        cout << "  3-param constructor called\n";
        // ❌ DUPLICATE validation logic!
        if (salary < 0) salary = 0;
        if (name.empty()) name = "Unknown";
    }

    EmployeeBad(const string& n, int i)
        : name(n), id(i), salary(0.0), department("General") {
        cout << "  2-param constructor called\n";
        // ❌ DUPLICATE validation logic!
        if (name.empty()) name = "Unknown";
    }

    EmployeeBad()
        : name("Unknown"), id(0), salary(0.0), department("General") {
        cout << "  Default constructor called\n";
    }

    void display() const {
        cout << "  Name: " << name << ", ID: " << id
             << ", Salary: $" << salary << ", Dept: " << department << "\n";
    }
};

void demonstrateProblem() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: THE PROBLEM           ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "--- Creating employees (pre-C++11 style) ---\n";
    EmployeeBad e1("Alice", 101, 50000, "Engineering");
    e1.display();

    cout << "\n";
    EmployeeBad e2("Bob", 102, 45000);
    e2.display();

    cout << "\n";
    EmployeeBad e3("Charlie", 103);
    e3.display();

    cout << "\n";
    EmployeeBad e4;
    e4.display();

    cout << "\n⚠️  THE PROBLEM:\n";
    cout << "   • Code duplication in each constructor\n";
    cout << "   • Validation logic repeated\n";
    cout << "   • Hard to maintain - change one, change all\n";
    cout << "   • Error-prone - easy to forget updates\n";
    cout << "   • No single source of truth\n";
}

// ============================================
// EXAMPLE 2: THE SOLUTION - DELEGATING CONSTRUCTORS
// ============================================

class EmployeeGood {
private:
    string name;
    int id;
    double salary;
    string department;

public:
    // ✅ Main constructor - all initialization logic here!
    EmployeeGood(const string& n, int i, double s, const string& d)
        : name(n), id(i), salary(s), department(d) {
        cout << "  Main constructor called\n";
        // All validation in ONE place!
        if (salary < 0) salary = 0;
        if (name.empty()) name = "Unknown";
        if (department.empty()) department = "General";
    }

    // ✅ Delegates to main constructor
    EmployeeGood(const string& n, int i, double s)
        : EmployeeGood(n, i, s, "General") {  // Delegation!
        cout << "  3-param constructor body\n";
    }

    // ✅ Delegates to 3-param constructor
    EmployeeGood(const string& n, int i)
        : EmployeeGood(n, i, 0.0) {  // Delegation!
        cout << "  2-param constructor body\n";
    }

    // ✅ Delegates to 2-param constructor
    EmployeeGood()
        : EmployeeGood("Unknown", 0) {  // Delegation!
        cout << "  Default constructor body\n";
    }

    void display() const {
        cout << "  Name: " << name << ", ID: " << id
             << ", Salary: $" << salary << ", Dept: " << department << "\n";
    }
};

void demonstrateSolution() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: THE SOLUTION          ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "--- Creating employees (C++11 delegation) ---\n";
    EmployeeGood e1("Alice", 101, 50000, "Engineering");
    e1.display();

    cout << "\n";
    EmployeeGood e2("Bob", 102, 45000);
    e2.display();

    cout << "\n";
    EmployeeGood e3("Charlie", 103);
    e3.display();

    cout << "\n";
    EmployeeGood e4;
    e4.display();

    cout << "\n✅ THE SOLUTION:\n";
    cout << "   • No code duplication!\n";
    cout << "   • Validation in ONE place\n";
    cout << "   • Easy to maintain\n";
    cout << "   • Single source of truth\n";
    cout << "   • Safer and cleaner code\n";
}

// ============================================
// EXAMPLE 3: EXECUTION ORDER
// ============================================

class ExecutionOrder {
private:
    int value;
    string name;

public:
    // Target constructor
    ExecutionOrder(int v, const string& n) : value(v), name(n) {
        cout << "  Target constructor: value=" << value
             << ", name=" << name << "\n";
        cout << "  Target body executing\n";
    }

    // Delegating constructor
    ExecutionOrder(int v) : ExecutionOrder(v, "Default") {
        cout << "  Delegating constructor body executing\n";
        // Target constructor completes BEFORE this body runs!
    }

    void display() const {
        cout << "  Value: " << value << ", Name: " << name << "\n";
    }
};

void demonstrateExecutionOrder() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: EXECUTION ORDER       ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "--- Creating object with delegation ---\n";
    ExecutionOrder obj(42);
    obj.display();

    cout << "\n💡 EXECUTION ORDER:\n";
    cout << "   1. Delegating constructor starts\n";
    cout << "   2. Target constructor called\n";
    cout << "   3. Target constructor body runs\n";
    cout << "   4. Target constructor completes\n";
    cout << "   5. Delegating constructor body runs\n";
    cout << "   6. Delegating constructor completes\n";
}

// ============================================
// EXAMPLE 4: COMMON PATTERN - DEFAULT PARAMETERS SIMULATION
// ============================================

class Rectangle {
private:
    double width;
    double height;
    string color;

public:
    // Main constructor
    Rectangle(double w, double h, const string& c)
        : width(w), height(h), color(c) {
        cout << "  Rectangle(" << w << ", " << h << ", \"" << c << "\")\n";
        if (width < 0) width = 0;
        if (height < 0) height = 0;
    }

    // Simulate default color parameter
    Rectangle(double w, double h)
        : Rectangle(w, h, "white") {
    }

    // Square (same width and height)
    Rectangle(double size)
        : Rectangle(size, size, "white") {
    }

    // Default - unit square
    Rectangle()
        : Rectangle(1.0, 1.0, "white") {
    }

    void display() const {
        cout << "  " << width << "x" << height << " " << color << " rectangle\n";
    }

    double area() const {
        return width * height;
    }
};

void demonstrateDefaultParams() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: DEFAULT PARAMS        ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "--- Various rectangle constructions ---\n";
    Rectangle r1(5.0, 3.0, "red");
    r1.display();

    cout << "\n";
    Rectangle r2(4.0, 2.0);  // Default color
    r2.display();

    cout << "\n";
    Rectangle r3(3.0);  // Square with default color
    r3.display();

    cout << "\n";
    Rectangle r4;  // Unit square with default color
    r4.display();

    cout << "\n💡 PATTERN:\n";
    cout << "   • Simulates default parameters\n";
    cout << "   • Flexible construction options\n";
    cout << "   • All logic in main constructor\n";
    cout << "   • Clean and maintainable\n";
}

// ============================================
// EXAMPLE 5: RESOURCE MANAGEMENT WITH DELEGATION
// ============================================

class Buffer {
private:
    char* data;
    size_t size;
    string name;

    void allocate() {
        data = new char[size];
        for (size_t i = 0; i < size; ++i) {
            data[i] = 0;
        }
        cout << "  Buffer \"" << name << "\" allocated (" << size << " bytes)\n";
    }

public:
    // Main constructor - handles all allocation
    Buffer(size_t s, const string& n) : size(s), name(n), data(nullptr) {
        allocate();
    }

    // Delegates with default name
    Buffer(size_t s) : Buffer(s, "Unnamed") {
    }

    // Delegates with default size and name
    Buffer() : Buffer(1024, "Default") {
    }

    ~Buffer() {
        delete[] data;
        cout << "  Buffer \"" << name << "\" deallocated\n";
    }

    // Delete copy operations for simplicity
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    void display() const {
        cout << "  Buffer: " << name << ", Size: " << size << " bytes\n";
    }
};

void demonstrateResourceManagement() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: RESOURCE MANAGEMENT   ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "--- Creating buffers ---\n";
    Buffer b1(2048, "MainBuffer");
    b1.display();

    cout << "\n";
    Buffer b2(512);
    b2.display();

    cout << "\n";
    Buffer b3;
    b3.display();

    cout << "\n💡 BENEFIT:\n";
    cout << "   • Resource allocation in ONE place\n";
    cout << "   • Consistent initialization\n";
    cout << "   • Safer resource management\n";
    cout << "   • Easier to maintain\n";

    cout << "\n--- Destructors will be called ---\n";
}

// ============================================
// EXAMPLE 6: DELEGATION WITH VALIDATION
// ============================================

class BankAccount {
private:
    string accountNumber;
    string ownerName;
    double balance;

    void validate() {
        if (balance < 0) {
            cout << "  ⚠️  Negative balance adjusted to 0\n";
            balance = 0;
        }
        if (ownerName.empty()) {
            ownerName = "Unknown";
            cout << "  ⚠️  Empty name set to 'Unknown'\n";
        }
        if (accountNumber.empty()) {
            accountNumber = "UNASSIGNED";
            cout << "  ⚠️  Empty account number set to 'UNASSIGNED'\n";
        }
    }

public:
    // Main constructor with validation
    BankAccount(const string& accNum, const string& owner, double bal)
        : accountNumber(accNum), ownerName(owner), balance(bal) {
        cout << "  Creating account: " << accNum << "\n";
        validate();
    }

    // Delegate with default balance
    BankAccount(const string& accNum, const string& owner)
        : BankAccount(accNum, owner, 0.0) {
    }

    // Delegate with default owner and balance
    BankAccount(const string& accNum)
        : BankAccount(accNum, "Unknown", 0.0) {
    }

    // Default constructor
    BankAccount()
        : BankAccount("UNASSIGNED", "Unknown", 0.0) {
    }

    void display() const {
        cout << "  Account: " << accountNumber
             << ", Owner: " << ownerName
             << ", Balance: $" << balance << "\n";
    }
};

void demonstrateValidation() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: VALIDATION            ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "--- Valid account ---\n";
    BankAccount acc1("ACC001", "Alice", 1000.0);
    acc1.display();

    cout << "\n--- Account with validation triggers ---\n";
    BankAccount acc2("ACC002", "", -500.0);  // Empty name, negative balance
    acc2.display();

    cout << "\n--- Using delegation ---\n";
    BankAccount acc3("ACC003");
    acc3.display();

    cout << "\n--- Default account ---\n";
    BankAccount acc4;
    acc4.display();

    cout << "\n💡 VALIDATION:\n";
    cout << "   • All validation in main constructor\n";
    cout << "   • Delegated constructors get validation automatically\n";
    cout << "   • Consistent validation across all constructors\n";
    cout << "   • No duplicate validation code\n";
}

// ============================================
// EXAMPLE 7: DELEGATION CHAIN
// ============================================

class Point3D {
private:
    double x, y, z;

public:
    // Most complete constructor
    Point3D(double xVal, double yVal, double zVal)
        : x(xVal), y(yVal), z(zVal) {
        cout << "  Point3D(" << x << ", " << y << ", " << z << ")\n";
    }

    // 2D point (z = 0)
    Point3D(double xVal, double yVal)
        : Point3D(xVal, yVal, 0.0) {  // Delegates to 3-param
        cout << "  2D point created\n";
    }

    // Point on X axis
    Point3D(double xVal)
        : Point3D(xVal, 0.0) {  // Delegates to 2-param
        cout << "  1D point created\n";
    }

    // Origin
    Point3D()
        : Point3D(0.0) {  // Delegates to 1-param
        cout << "  Origin created\n";
    }

    void display() const {
        cout << "  Point(" << x << ", " << y << ", " << z << ")\n";
    }
};

void demonstrateDelegationChain() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 7: DELEGATION CHAIN      ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "--- Creating origin (delegation chain) ---\n";
    Point3D p1;
    p1.display();

    cout << "\n--- 1D point ---\n";
    Point3D p2(5.0);
    p2.display();

    cout << "\n--- 2D point ---\n";
    Point3D p3(3.0, 4.0);
    p3.display();

    cout << "\n--- 3D point ---\n";
    Point3D p4(1.0, 2.0, 3.0);
    p4.display();

    cout << "\n💡 DELEGATION CHAIN:\n";
    cout << "   • Each constructor delegates to more complete one\n";
    cout << "   • Forms a chain: default → 1-param → 2-param → 3-param\n";
    cout << "   • Clear progression of initialization\n";
    cout << "   • Easy to understand and maintain\n";
}

// ============================================
// EXAMPLE 8: COMMON MISTAKE - CIRCULAR DELEGATION
// ============================================

void demonstrateCommonMistakes() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 8: COMMON MISTAKES       ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "❌ MISTAKE 1: Circular Delegation\n";
    cout << "class Bad {\n";
    cout << "    Bad() : Bad(0) { }      // Delegates to Bad(int)\n";
    cout << "    Bad(int x) : Bad() { }  // Delegates to Bad()\n";
    cout << "};\n";
    cout << "// ❌ Infinite loop! Compilation error!\n";
    cout << "\n";

    cout << "❌ MISTAKE 2: Delegation + Member Initialization\n";
    cout << "class Bad {\n";
    cout << "    int x, y;\n";
    cout << "    Bad() : Bad(0), x(0) { }  // ❌ Can't do both!\n";
    cout << "};\n";
    cout << "// ❌ Can't delegate AND initialize members!\n";
    cout << "\n";

    cout << "❌ MISTAKE 3: Wrong Order Assumption\n";
    cout << "class Bad {\n";
    cout << "    int x;\n";
    cout << "    Bad() : Bad(10) {\n";
    cout << "        x = 20;  // ⚠️  Overwrites x from target constructor!\n";
    cout << "    }\n";
    cout << "    Bad(int val) : x(val) { }\n";
    cout << "};\n";
    cout << "// Target constructor runs first, then delegating body\n";
    cout << "\n";

    cout << "✅ CORRECT PATTERNS:\n";
    cout << "\n";
    cout << "1. Delegate to most complete constructor:\n";
    cout << "   MyClass() : MyClass(default_values) { }\n";
    cout << "\n";
    cout << "2. Delegation is the ONLY initializer:\n";
    cout << "   MyClass(int x) : MyClass(x, 0, 0) { }  // ✅ OK\n";
    cout << "   // NOT: MyClass(int x) : MyClass(x, 0), y(0) { }  // ❌\n";
    cout << "\n";
    cout << "3. Keep chains simple and linear:\n";
    cout << "   Constructor1() → Constructor2() → Main Constructor\n";
}

// ============================================
// EXAMPLE 9: OLD STYLE VS NEW STYLE
// ============================================

// Old style (pre-C++11) - using init() helper
class OldStyle {
private:
    int x, y;
    string name;

    void init(int xVal, int yVal, const string& n) {
        x = xVal;
        y = yVal;
        name = n;
        // Validation
        if (x < 0) x = 0;
        if (y < 0) y = 0;
        if (name.empty()) name = "Default";
    }

public:
    OldStyle(int xVal, int yVal, const string& n) {
        init(xVal, yVal, n);
    }

    OldStyle(int xVal, int yVal) {
        init(xVal, yVal, "Default");
    }

    OldStyle() {
        init(0, 0, "Default");
    }

    void display() const {
        cout << "  OldStyle: " << name << " (" << x << ", " << y << ")\n";
    }
};

// New style (C++11+) - using delegation
class NewStyle {
private:
    int x, y;
    string name;

public:
    NewStyle(int xVal, int yVal, const string& n)
        : x(xVal), y(yVal), name(n) {
        // Validation in constructor
        if (x < 0) x = 0;
        if (y < 0) y = 0;
        if (name.empty()) name = "Default";
    }

    NewStyle(int xVal, int yVal)
        : NewStyle(xVal, yVal, "Default") {
    }

    NewStyle()
        : NewStyle(0, 0, "Default") {
    }

    void display() const {
        cout << "  NewStyle: " << name << " (" << x << ", " << y << ")\n";
    }
};

void demonstrateOldVsNew() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 9: OLD VS NEW STYLE      ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "--- Old Style (init helper) ---\n";
    OldStyle old1(10, 20, "Test");
    old1.display();

    OldStyle old2(5, 10);
    old2.display();

    cout << "\n--- New Style (delegation) ---\n";
    NewStyle new1(10, 20, "Test");
    new1.display();

    NewStyle new2(5, 10);
    new2.display();

    cout << "\n📊 COMPARISON:\n";
    cout << "Old Style (pre-C++11):\n";
    cout << "  • Uses init() helper function\n";
    cout << "  • Assignment, not initialization\n";
    cout << "  • Can't initialize const members\n";
    cout << "  • Can't initialize references\n";
    cout << "  • Extra function call overhead\n";
    cout << "\n";
    cout << "New Style (C++11+):\n";
    cout << "  • True constructor delegation\n";
    cout << "  • Proper initialization\n";
    cout << "  • Can initialize const members\n";
    cout << "  • Can initialize references\n";
    cout << "  • Compiler can optimize better\n";
}

// ============================================
// EXAMPLE 10: REAL-WORLD - LOGGING SYSTEM
// ============================================

class Logger {
private:
    string filename;
    string level;
    bool timestamped;
    bool consoleOutput;

    void initialize() {
        cout << "  Logger initialized:\n";
        cout << "    File: " << filename << "\n";
        cout << "    Level: " << level << "\n";
        cout << "    Timestamps: " << (timestamped ? "Yes" : "No") << "\n";
        cout << "    Console: " << (consoleOutput ? "Yes" : "No") << "\n";
    }

public:
    // Main constructor - all configuration
    Logger(const string& file, const string& lvl, bool ts, bool console)
        : filename(file), level(lvl), timestamped(ts), consoleOutput(console) {
        initialize();
    }

    // Production logger
    Logger(const string& file)
        : Logger(file, "INFO", true, false) {
        cout << "  Production logger created\n";
    }

    // Development logger
    Logger()
        : Logger("debug.log", "DEBUG", true, true) {
        cout << "  Development logger created\n";
    }

    void log(const string& message) const {
        cout << "  [" << level << "] " << message << "\n";
    }
};

void demonstrateRealWorld() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 10: REAL-WORLD LOGGER    ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "--- Production logger ---\n";
    Logger prodLogger("app.log");
    prodLogger.log("Application started");

    cout << "\n--- Development logger ---\n";
    Logger devLogger;
    devLogger.log("Debug information");

    cout << "\n--- Custom logger ---\n";
    Logger customLogger("custom.log", "WARNING", false, true);
    customLogger.log("Custom warning");

    cout << "\n💡 REAL-WORLD BENEFITS:\n";
    cout << "   • Easy configuration presets\n";
    cout << "   • Consistent initialization\n";
    cout << "   • Simple to add new constructors\n";
    cout << "   • All setup logic centralized\n";
}

// ============================================
// MAIN FUNCTION - RUN ALL EXAMPLES
// ============================================

int main() {
    cout << "\n";
    cout << "╔═══════════════════════════════════════════════════════════╗\n";
    cout << "║                                                           ║\n";
    cout << "║          DELEGATING CONSTRUCTORS IN C++                   ║\n";
    cout << "║             Reducing Code Duplication                     ║\n";
    cout << "║                                                           ║\n";
    cout << "╚═══════════════════════════════════════════════════════════╝\n";

    demonstrateProblem();
    demonstrateSolution();
    demonstrateExecutionOrder();
    demonstrateDefaultParams();
    demonstrateResourceManagement();
    demonstrateValidation();
    demonstrateDelegationChain();
    demonstrateCommonMistakes();
    demonstrateOldVsNew();
    demonstrateRealWorld();

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
    cout << "WHAT ARE DELEGATING CONSTRUCTORS?\n";
    cout << "========================================\n";
    cout << "Definition:\n";
    cout << "  A delegating constructor calls another constructor\n";
    cout << "  of the same class in its member initializer list.\n";
    cout << "\n";
    cout << "SYNTAX:\n";
    cout << "class MyClass {\n";
    cout << "public:\n";
    cout << "    MyClass(int x, int y) : x(x), y(y) { }\n";
    cout << "    \n";
    cout << "    MyClass(int x) : MyClass(x, 0) { }  // Delegates!\n";
    cout << "    MyClass() : MyClass(0, 0) { }       // Delegates!\n";
    cout << "};\n";
    cout << "\n";
    cout << "KEY FEATURES:\n";
    cout << "  • Part of member initializer list\n";
    cout << "  • Calls another constructor first\n";
    cout << "  • Reduces code duplication\n";
    cout << "  • Available since C++11\n";

    cout << "\n========================================\n";
    cout << "WHY USE DELEGATING CONSTRUCTORS?\n";
    cout << "========================================\n";
    cout << "BEFORE C++11 (Problems):\n";
    cout << "class Data {\n";
    cout << "    int x, y;\n";
    cout << "public:\n";
    cout << "    Data(int a, int b) : x(a), y(b) {\n";
    cout << "        // Validation logic\n";
    cout << "        if (x < 0) x = 0;\n";
    cout << "        if (y < 0) y = 0;\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    Data(int a) : x(a), y(0) {\n";
    cout << "        // ❌ DUPLICATE validation logic!\n";
    cout << "        if (x < 0) x = 0;\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "PROBLEMS:\n";
    cout << "  • Code duplication\n";
    cout << "  • Hard to maintain\n";
    cout << "  • Easy to forget updates\n";
    cout << "  • Inconsistent behavior possible\n";
    cout << "\n";
    cout << "WITH C++11 (Solution):\n";
    cout << "class Data {\n";
    cout << "    int x, y;\n";
    cout << "public:\n";
    cout << "    Data(int a, int b) : x(a), y(b) {\n";
    cout << "        // Validation logic in ONE place\n";
    cout << "        if (x < 0) x = 0;\n";
    cout << "        if (y < 0) y = 0;\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    Data(int a) : Data(a, 0) { }  // ✅ Delegates!\n";
    cout << "};\n";
    cout << "\n";
    cout << "BENEFITS:\n";
    cout << "  • No code duplication\n";
    cout << "  • Single source of truth\n";
    cout << "  • Easy to maintain\n";
    cout << "  • Consistent behavior guaranteed\n";

    cout << "\n========================================\n";
    cout << "EXECUTION ORDER\n";
    cout << "========================================\n";
    cout << "IMPORTANT: Target constructor runs COMPLETELY first!\n";
    cout << "\n";
    cout << "class Example {\n";
    cout << "    int x;\n";
    cout << "public:\n";
    cout << "    Example(int val) : x(val) {\n";
    cout << "        cout << \"Target constructor body\\n\";\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    Example() : Example(10) {\n";
    cout << "        cout << \"Delegating constructor body\\n\";\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "Example obj;  // What happens?\n";
    cout << "\n";
    cout << "EXECUTION ORDER:\n";
    cout << "1. Example() starts\n";
    cout << "2. Calls Example(10)\n";
    cout << "3. Example(10) initializes x = 10\n";
    cout << "4. Example(10) body runs → prints \"Target constructor body\"\n";
    cout << "5. Example(10) completes\n";
    cout << "6. Example() body runs → prints \"Delegating constructor body\"\n";
    cout << "7. Example() completes\n";
    cout << "\n";
    cout << "KEY POINT: Target is FULLY constructed before delegating body runs!\n";

    cout << "\n========================================\n";
    cout << "RULES AND RESTRICTIONS\n";
    cout << "========================================\n";
    cout << "RULE 1: Delegation must be ONLY initializer\n";
    cout << "❌ WRONG:\n";
    cout << "class Bad {\n";
    cout << "    int x, y;\n";
    cout << "    Bad() : Bad(10), y(20) { }  // ❌ Can't do both!\n";
    cout << "};\n";
    cout << "\n";
    cout << "✅ CORRECT:\n";
    cout << "class Good {\n";
    cout << "    int x, y;\n";
    cout << "    Good() : Good(10, 20) { }  // ✅ Only delegation\n";
    cout << "    Good(int a, int b) : x(a), y(b) { }\n";
    cout << "};\n";
    cout << "\n";
    cout << "RULE 2: No circular delegation\n";
    cout << "❌ WRONG:\n";
    cout << "class Bad {\n";
    cout << "    Bad() : Bad(0) { }\n";
    cout << "    Bad(int x) : Bad() { }  // ❌ Circular!\n";
    cout << "};\n";
    cout << "\n";
    cout << "RULE 3: Delegation happens in initializer list only\n";
    cout << "❌ WRONG:\n";
    cout << "class Bad {\n";
    cout << "    Bad() {\n";
    cout << "        Bad(10);  // ❌ Creates temporary, doesn't delegate!\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "✅ CORRECT:\n";
    cout << "class Good {\n";
    cout << "    Good() : Good(10) { }  // ✅ In initializer list\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "COMMON PATTERNS\n";
    cout << "========================================\n";
    cout << "PATTERN 1: Default Parameters Simulation\n";
    cout << "class Widget {\n";
    cout << "public:\n";
    cout << "    Widget(int x, int y, int z) { /* main */ }\n";
    cout << "    Widget(int x, int y) : Widget(x, y, 0) { }\n";
    cout << "    Widget(int x) : Widget(x, 0, 0) { }\n";
    cout << "    Widget() : Widget(0, 0, 0) { }\n";
    cout << "};\n";
    cout << "\n";
    cout << "PATTERN 2: Validation in One Place\n";
    cout << "class Data {\n";
    cout << "    int value;\n";
    cout << "public:\n";
    cout << "    Data(int v) : value(v) {\n";
    cout << "        if (value < 0) value = 0;  // Validation\n";
    cout << "    }\n";
    cout << "    Data() : Data(0) { }  // Gets validation!\n";
    cout << "};\n";
    cout << "\n";
    cout << "PATTERN 3: Resource Allocation\n";
    cout << "class Buffer {\n";
    cout << "    char* data;\n";
    cout << "public:\n";
    cout << "    Buffer(size_t size) {\n";
    cout << "        data = new char[size];  // Allocate once\n";
    cout << "    }\n";
    cout << "    Buffer() : Buffer(1024) { }  // Default size\n";
    cout << "};\n";
    cout << "\n";
    cout << "PATTERN 4: Delegation Chain\n";
    cout << "class Point {\n";
    cout << "    double x, y, z;\n";
    cout << "public:\n";
    cout << "    Point(double a, double b, double c) : x(a), y(b), z(c) { }\n";
    cout << "    Point(double a, double b) : Point(a, b, 0) { }\n";
    cout << "    Point(double a) : Point(a, 0) { }\n";
    cout << "    Point() : Point(0) { }\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "OLD STYLE VS NEW STYLE\n";
    cout << "========================================\n";
    cout << "OLD STYLE (pre-C++11): init() helper\n";
    cout << "class OldWay {\n";
    cout << "    int x, y;\n";
    cout << "    \n";
    cout << "    void init(int a, int b) {  // Helper function\n";
    cout << "        x = a;  // Assignment, not initialization\n";
    cout << "        y = b;\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "public:\n";
    cout << "    OldWay(int a, int b) { init(a, b); }\n";
    cout << "    OldWay(int a) { init(a, 0); }\n";
    cout << "    OldWay() { init(0, 0); }\n";
    cout << "};\n";
    cout << "\n";
    cout << "PROBLEMS WITH OLD STYLE:\n";
    cout << "  • Uses assignment, not initialization\n";
    cout << "  • Can't initialize const members\n";
    cout << "  • Can't initialize reference members\n";
    cout << "  • Can't initialize members without default constructors\n";
    cout << "  • Extra function call overhead\n";
    cout << "  • Not as clear\n";
    cout << "\n";
    cout << "NEW STYLE (C++11+): Delegation\n";
    cout << "class NewWay {\n";
    cout << "    int x, y;\n";
    cout << "    \n";
    cout << "public:\n";
    cout << "    NewWay(int a, int b) : x(a), y(b) { }  // Real init\n";
    cout << "    NewWay(int a) : NewWay(a, 0) { }       // Delegate\n";
    cout << "    NewWay() : NewWay(0, 0) { }            // Delegate\n";
    cout << "};\n";
    cout << "\n";
    cout << "BENEFITS OF NEW STYLE:\n";
    cout << "  • True initialization\n";
    cout << "  • Can initialize const members\n";
    cout << "  • Can initialize reference members\n";
    cout << "  • Works with all member types\n";
    cout << "  • Compiler can optimize better\n";
    cout << "  • Clearer intent\n";

    cout << "\n========================================\n";
    cout << "BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "✅ DO:\n";
    cout << "  • Delegate to most complete constructor\n";
    cout << "  • Put all initialization logic in one place\n";
    cout << "  • Use for default parameter simulation\n";
    cout << "  • Keep delegation chains simple and linear\n";
    cout << "  • Use for validation centralization\n";
    cout << "  • Document which is the \"main\" constructor\n";
    cout << "  • Prefer delegation over init() helpers\n";
    cout << "\n";
    cout << "❌ DON'T:\n";
    cout << "  • Create circular delegation\n";
    cout << "  • Mix delegation with member initialization\n";
    cout << "  • Create complex delegation hierarchies\n";
    cout << "  • Forget that target runs completely first\n";
    cout << "  • Try to delegate in constructor body\n";
    cout << "  • Use when simple default parameters work\n";

    cout << "\n========================================\n";
    cout << "COMMON MISTAKES\n";
    cout << "========================================\n";
    cout << "MISTAKE 1: Mixing delegation and initialization\n";
    cout << "❌ class Bad {\n";
    cout << "    int x, y;\n";
    cout << "    Bad() : Bad(10), y(20) { }  // ERROR!\n";
    cout << "};\n";
    cout << "\n";
    cout << "MISTAKE 2: Circular delegation\n";
    cout << "❌ class Bad {\n";
    cout << "    Bad() : Bad(0) { }\n";
    cout << "    Bad(int) : Bad() { }  // ERROR!\n";
    cout << "};\n";
    cout << "\n";
    cout << "MISTAKE 3: Delegating in body\n";
    cout << "❌ class Bad {\n";
    cout << "    Bad() {\n";
    cout << "        Bad(10);  // Creates temporary, doesn't delegate!\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "MISTAKE 4: Assuming delegating body runs first\n";
    cout << "❌ class Bad {\n";
    cout << "    int x;\n";
    cout << "    Bad() : Bad(10) {\n";
    cout << "        x = 5;  // Overwrites value from target!\n";
    cout << "    }\n";
    cout << "    Bad(int v) : x(v) { }\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "WHEN TO USE DELEGATION\n";
    cout << "========================================\n";
    cout << "✅ USE DELEGATION WHEN:\n";
    cout << "  • Multiple constructors share initialization logic\n";
    cout << "  • Need to centralize validation\n";
    cout << "  • Want to avoid code duplication\n";
    cout << "  • Simulating default parameters\n";
    cout << "  • Managing resources in one place\n";
    cout << "  • Need consistent initialization\n";
    cout << "\n";
    cout << "❌ DON'T USE DELEGATION WHEN:\n";
    cout << "  • Simple default parameters work fine:\n";
    cout << "    // Instead of:\n";
    cout << "    MyClass(int x, int y);\n";
    cout << "    MyClass(int x) : MyClass(x, 0) { }\n";
    cout << "    \n";
    cout << "    // Just use:\n";
    cout << "    MyClass(int x, int y = 0);\n";
    cout << "\n";
    cout << "  • Each constructor does completely different things\n";
    cout << "  • It makes code more complex, not simpler\n";

    cout << "\n========================================\n";
    cout << "QUICK REFERENCE\n";
    cout << "========================================\n";
    cout << "// Basic syntax\n";
    cout << "class MyClass {\n";
    cout << "public:\n";
    cout << "    MyClass(int x, int y) : x(x), y(y) { }\n";
    cout << "    MyClass(int x) : MyClass(x, 0) { }  // Delegate\n";
    cout << "    MyClass() : MyClass(0) { }          // Delegate\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Execution order\n";
    cout << "MyClass obj;  // Calls MyClass()\n";
    cout << "// 1. MyClass() delegates to MyClass(0)\n";
    cout << "// 2. MyClass(0) delegates to MyClass(0, 0)\n";
    cout << "// 3. MyClass(0, 0) body runs\n";
    cout << "// 4. MyClass(0) body runs\n";
    cout << "// 5. MyClass() body runs\n";
    cout << "\n";
    cout << "// ✅ Correct patterns\n";
    cout << "MyClass() : MyClass(default_values) { }\n";
    cout << "\n";
    cout << "// ❌ Wrong patterns\n";
    cout << "// MyClass() : MyClass(0), x(5) { }     // Can't mix!\n";
    cout << "// MyClass() { MyClass(0); }            // Not delegation!\n";

    cout << "\n========================================\n";
    cout << "KEY TAKEAWAYS\n";
    cout << "========================================\n";
    cout << "1. Delegating constructors call other constructors\n";
    cout << "2. Reduces code duplication dramatically\n";
    cout << "3. Centralizes initialization and validation\n";
    cout << "4. Delegation must be ONLY thing in initializer list\n";
    cout << "5. Target constructor runs COMPLETELY first\n";
    cout << "6. No circular delegation allowed\n";
    cout << "7. Available since C++11\n";
    cout << "8. Prefer over init() helper functions\n";
    cout << "9. Delegate to most complete constructor\n";
    cout << "10. Makes code easier to maintain and understand\n";

    cout << "\n========================================\n";
    cout << "COMPARISON TABLE\n";
    cout << "========================================\n";
    cout << "┌──────────────────────┬─────────────┬─────────────┐\n";
    cout << "│ Feature              │ Pre-C++11   │ C++11+      │\n";
    cout << "├──────────────────────┼─────────────┼─────────────┤\n";
    cout << "│ Code duplication     │ Yes         │ No          │\n";
    cout << "│ Initialization       │ Assignment  │ True init   │\n";
    cout << "│ Const members        │ Hard        │ Easy        │\n";
    cout << "│ Reference members    │ Hard        │ Easy        │\n";
    cout << "│ Maintainability      │ Lower       │ Higher      │\n";
    cout << "│ Single source truth  │ No          │ Yes         │\n";
    cout << "│ Compiler optimization│ Limited     │ Better      │\n";
    cout << "│ Code clarity         │ Good        │ Excellent   │\n";
    cout << "└──────────────────────┴─────────────┴─────────────┘\n";

    return 0;
}
