#include <iostream>
#include <string>
#include <vector>
#include <memory>
using namespace std;

// ============================================
// WHAT IS HEADER/SOURCE SEPARATION?
// ============================================

/*
 * HEADER/SOURCE SEPARATION:
 * - Separating class declaration from implementation
 * - Header (.h/.hpp): Interface (what the class does)
 * - Source (.cpp): Implementation (how the class does it)
 * - Fundamental practice in professional C++ development
 *
 * WHY SEPARATE?
 * - Faster compilation (change implementation, not header)
 * - Information hiding (hide implementation details)
 * - Reusability (one header, many implementations)
 * - Modularity (organize large projects)
 * - Cleaner code (separate interface from details)
 *
 * FILE TYPES:
 * - .h   : C-style header (works for C and C++)
 * - .hpp : C++ header (more explicit)
 * - .cpp : C++ implementation (source file)
 *
 * HEADER FILE CONTAINS:
 * - Class declarations
 * - Function prototypes
 * - Inline functions (small functions)
 * - Template definitions
 * - Constants and enums
 * - Include guards or #pragma once
 *
 * SOURCE FILE CONTAINS:
 * - Function implementations
 * - Member function definitions
 * - Static member initialization
 * - Non-inline code
 *
 * INCLUDE GUARDS:
 * - Prevent multiple inclusion
 * - #ifndef / #define / #endif pattern
 * - Or: #pragma once (modern, simpler)
 *
 * COMPILATION PROCESS:
 * 1. Header included by source files
 * 2. Each .cpp compiled separately to .o (object file)
 * 3. Object files linked together
 * 4. Creates final executable
 *
 * BEST PRACTICES:
 * - Use #pragma once (simpler than include guards)
 * - Forward declarations to reduce dependencies
 * - Include what you use (IWYU principle)
 * - Minimize includes in headers
 * - Put implementation in .cpp when possible
 */

// ============================================
// EXAMPLE 1: SINGLE FILE (Before Separation)
// ============================================

// Everything in one file - NOT recommended for real projects!
class PersonSingleFile {
private:
    string name;
    int age;

public:
    // Constructor
    PersonSingleFile(const string& n, int a) : name(n), age(a) {
        cout << "  PersonSingleFile constructor: " << name << "\n";
    }

    // Destructor
    ~PersonSingleFile() {
        cout << "  PersonSingleFile destructor: " << name << "\n";
    }

    // Methods
    void display() const {
        cout << "  Name: " << name << ", Age: " << age << "\n";
    }

    void setAge(int a) {
        age = a;
    }

    int getAge() const {
        return age;
    }
};

void demonstrateSingleFile() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: SINGLE FILE (Before)   ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating person ---\n";
    PersonSingleFile person("Alice", 30);

    cout << "\n--- Using person ---\n";
    person.display();

    cout << "\n--- Modifying person ---\n";
    person.setAge(31);
    person.display();

    cout << "\n⚠️  PROBLEMS WITH SINGLE FILE:\n";
    cout << "   • Changes to implementation require recompilation\n";
    cout << "   • No separation of interface and implementation\n";
    cout << "   • Difficult to maintain in large projects\n";
    cout << "   • Harder to create libraries\n";
    cout << "   • Implementation details exposed\n";
}

// ============================================
// EXAMPLE 2: SIMULATED HEADER/SOURCE SEPARATION
// ============================================

// This simulates what would be in Person.h
// (In real projects, this would be a separate file)

class PersonSeparated {
private:
    string name;
    int age;

public:
    // Declarations only (interface)
    PersonSeparated(const string& n, int a);
    ~PersonSeparated();

    void display() const;
    void setAge(int a);
    int getAge() const;
    string getName() const;
};

// This simulates what would be in Person.cpp
// (In real projects, this would be a separate file)

// Constructor implementation
PersonSeparated::PersonSeparated(const string& n, int a)
    : name(n), age(a) {
    cout << "  PersonSeparated constructor: " << name << "\n";
}

// Destructor implementation
PersonSeparated::~PersonSeparated() {
    cout << "  PersonSeparated destructor: " << name << "\n";
}

// Method implementations
void PersonSeparated::display() const {
    cout << "  Name: " << name << ", Age: " << age << "\n";
}

void PersonSeparated::setAge(int a) {
    age = a;
}

int PersonSeparated::getAge() const {
    return age;
}

string PersonSeparated::getName() const {
    return name;
}

void demonstrateSeparation() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: SEPARATED (Better)     ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating person ---\n";
    PersonSeparated person("Bob", 25);

    cout << "\n--- Using person ---\n";
    person.display();

    cout << "\n--- Modifying person ---\n";
    person.setAge(26);
    person.display();

    cout << "\n✅ BENEFITS OF SEPARATION:\n";
    cout << "   • Change implementation without recompiling users\n";
    cout << "   • Clear interface in header file\n";
    cout << "   • Implementation details hidden\n";
    cout << "   • Better organization\n";
    cout << "   • Faster incremental compilation\n";
}

// ============================================
// EXAMPLE 3: INCLUDE GUARDS (Traditional)
// ============================================

void demonstrateIncludeGuards() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: INCLUDE GUARDS         ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "TRADITIONAL INCLUDE GUARD:\n";
    cout << "\n";
    cout << "// Person.h\n";
    cout << "#ifndef PERSON_H\n";
    cout << "#define PERSON_H\n";
    cout << "\n";
    cout << "#include <string>\n";
    cout << "\n";
    cout << "class Person {\n";
    cout << "private:\n";
    cout << "    std::string name;\n";
    cout << "    int age;\n";
    cout << "    \n";
    cout << "public:\n";
    cout << "    Person(const std::string& n, int a);\n";
    cout << "    ~Person();\n";
    cout << "    \n";
    cout << "    void display() const;\n";
    cout << "    void setAge(int a);\n";
    cout << "    int getAge() const;\n";
    cout << "};\n";
    cout << "\n";
    cout << "#endif // PERSON_H\n";

    cout << "\n✅ BENEFITS:\n";
    cout << "   • Prevents multiple inclusion\n";
    cout << "   • Avoids redefinition errors\n";
    cout << "   • Standard C++ practice\n";
    cout << "   • Works on all compilers\n";

    cout << "\n⚠️  DRAWBACKS:\n";
    cout << "   • More boilerplate code\n";
    cout << "   • Easy to make mistakes\n";
    cout << "   • Must match guard names carefully\n";
}

// ============================================
// EXAMPLE 4: #pragma once (Modern)
// ============================================

void demonstratePragmaOnce() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: #pragma once (Modern)  ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "MODERN #pragma once:\n";
    cout << "\n";
    cout << "// Person.h\n";
    cout << "#pragma once\n";
    cout << "\n";
    cout << "#include <string>\n";
    cout << "\n";
    cout << "class Person {\n";
    cout << "private:\n";
    cout << "    std::string name;\n";
    cout << "    int age;\n";
    cout << "    \n";
    cout << "public:\n";
    cout << "    Person(const std::string& n, int a);\n";
    cout << "    ~Person();\n";
    cout << "    \n";
    cout << "    void display() const;\n";
    cout << "    void setAge(int a);\n";
    cout << "    int getAge() const;\n";
    cout << "};\n";

    cout << "\n✅ BENEFITS:\n";
    cout << "   • Simpler syntax (one line!)\n";
    cout << "   • No naming conflicts\n";
    cout << "   • Faster compilation\n";
    cout << "   • Less error-prone\n";
    cout << "   • Supported by all modern compilers\n";

    cout << "\n💡 RECOMMENDATION:\n";
    cout << "   Use #pragma once for new projects!\n";
    cout << "   Only use include guards if you need C++98 compatibility\n";
}

// ============================================
// EXAMPLE 5: INLINE FUNCTIONS IN HEADERS
// ============================================

// Simulating header file
class Calculator {
private:
    int value;

public:
    Calculator() : value(0) {}

    // Small functions can be inline (in header)
    void setValue(int v) { value = v; }
    int getValue() const { return value; }

    // Larger functions should be in .cpp
    int compute(int a, int b);
    void complexOperation();
};

// Simulating source file
int Calculator::compute(int a, int b) {
    // Complex calculation
    int result = 0;
    for (int i = 0; i < a; ++i) {
        result += b * i;
    }
    return result;
}

void Calculator::complexOperation() {
    // Many lines of code
    for (int i = 0; i < 100; ++i) {
        value += i * 2;
    }
}

void demonstrateInlineFunctions() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: INLINE FUNCTIONS       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Using calculator ---\n";
    Calculator calc;
    calc.setValue(10);
    cout << "  Value: " << calc.getValue() << "\n";
    cout << "  Compute: " << calc.compute(5, 3) << "\n";

    cout << "\n💡 GUIDELINE:\n";
    cout << "   • INLINE (in header): Simple getters/setters\n";
    cout << "   • IN SOURCE: Complex logic, loops, algorithms\n";
    cout << "\n";
    cout << "RULE OF THUMB:\n";
    cout << "   If function > 3 lines → put in .cpp\n";
    cout << "   If function ≤ 3 lines → can inline in .h\n";
}

// ============================================
// EXAMPLE 6: FORWARD DECLARATIONS
// ============================================

// Forward declaration reduces header dependencies
class Engine;  // Forward declaration

class Car {
private:
    Engine* engine;  // Pointer to Engine (forward declaration enough)
    string model;

public:
    Car(const string& m);
    ~Car();

    void start();
    void display() const;
};

// Now the full Engine definition (would be in Engine.h)
class Engine {
private:
    int horsepower;

public:
    Engine(int hp) : horsepower(hp) {
        cout << "  Engine created: " << hp << " HP\n";
    }

    ~Engine() {
        cout << "  Engine destroyed\n";
    }

    int getHorsepower() const { return horsepower; }
};

// Car implementation (would be in Car.cpp)
Car::Car(const string& m) : model(m), engine(new Engine(200)) {
    cout << "  Car created: " << model << "\n";
}

Car::~Car() {
    delete engine;
    cout << "  Car destroyed: " << model << "\n";
}

void Car::start() {
    cout << "  Starting " << model << " with "
         << engine->getHorsepower() << " HP engine\n";
}

void Car::display() const {
    cout << "  Model: " << model
         << ", Engine: " << engine->getHorsepower() << " HP\n";
}

void demonstrateForwardDeclarations() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: FORWARD DECLARATIONS   ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating car ---\n";
    Car car("Tesla Model S");

    cout << "\n--- Using car ---\n";
    car.start();
    car.display();

    cout << "\n✅ BENEFITS:\n";
    cout << "   • Reduces header dependencies\n";
    cout << "   • Faster compilation\n";
    cout << "   • Breaks circular dependencies\n";
    cout << "   • Car.h doesn't need #include \"Engine.h\"\n";
    cout << "\n";
    cout << "💡 WHEN TO USE:\n";
    cout << "   • When using pointers/references only\n";
    cout << "   • No need for full definition\n";
    cout << "   • Reduces coupling between files\n";
}

// ============================================
// EXAMPLE 7: STATIC MEMBER INITIALIZATION
// ============================================

class BankAccount {
private:
    string owner;
    double balance;
    static int accountCount;  // Static member declaration

public:
    BankAccount(const string& o, double b);
    ~BankAccount();

    static int getAccountCount();
    void display() const;
};

// Static member MUST be initialized in .cpp file
int BankAccount::accountCount = 0;

BankAccount::BankAccount(const string& o, double b)
    : owner(o), balance(b) {
    accountCount++;
    cout << "  Account created for " << owner
         << " (Total accounts: " << accountCount << ")\n";
}

BankAccount::~BankAccount() {
    accountCount--;
    cout << "  Account closed for " << owner
         << " (Remaining: " << accountCount << ")\n";
}

int BankAccount::getAccountCount() {
    return accountCount;
}

void BankAccount::display() const {
    cout << "  Owner: " << owner << ", Balance: $" << balance << "\n";
}

void demonstrateStaticMembers() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 7: STATIC MEMBERS         ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating accounts ---\n";
    BankAccount* acc1 = new BankAccount("Alice", 1000.0);
    BankAccount* acc2 = new BankAccount("Bob", 2000.0);
    BankAccount* acc3 = new BankAccount("Charlie", 3000.0);

    cout << "\n--- Account count ---\n";
    cout << "  Total accounts: " << BankAccount::getAccountCount() << "\n";

    cout << "\n--- Closing accounts ---\n";
    delete acc1;
    delete acc2;

    cout << "\n--- Account count after closing ---\n";
    cout << "  Remaining accounts: " << BankAccount::getAccountCount() << "\n";

    delete acc3;

    cout << "\n💡 KEY POINTS:\n";
    cout << "   • Static members declared in header\n";
    cout << "   • Static members initialized in .cpp\n";
    cout << "   • Only ONE copy shared by all objects\n";
    cout << "   • Must be defined outside class\n";
}

// ============================================
// EXAMPLE 8: TEMPLATE CLASSES (Special Case)
// ============================================

// Templates MUST be in header file!
template<typename T>
class Box {
private:
    T value;

public:
    Box(T v) : value(v) {
        cout << "  Box created with value\n";
    }

    T getValue() const { return value; }
    void setValue(T v) { value = v; }

    void display() const {
        cout << "  Box contains: " << value << "\n";
    }
};

void demonstrateTemplates() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 8: TEMPLATES (Special)    ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating boxes ---\n";
    Box<int> intBox(42);
    Box<string> strBox("Hello");
    Box<double> dblBox(3.14);

    cout << "\n--- Using boxes ---\n";
    intBox.display();
    strBox.display();
    dblBox.display();

    cout << "\n⚠️  IMPORTANT:\n";
    cout << "   • Templates CANNOT be separated into .h and .cpp!\n";
    cout << "   • Full template definition must be in header\n";
    cout << "   • This is a limitation of how templates work\n";
    cout << "   • Compiler needs full code to instantiate\n";
    cout << "\n";
    cout << "WORKAROUND (if needed):\n";
    cout << "   • Use .hpp extension\n";
    cout << "   • Or include .cpp at end of .h file\n";
    cout << "   • Or explicit template instantiation (advanced)\n";
}

// ============================================
// EXAMPLE 9: CONST AND CONSTEXPR IN HEADERS
// ============================================

class Constants {
public:
    // These can be in header
    static constexpr double PI = 3.14159265359;
    static constexpr int MAX_SIZE = 100;

    // But regular static const needs .cpp initialization
    static const string VERSION;
};

// Must be in .cpp file
const string Constants::VERSION = "1.0.0";

void demonstrateConstants() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 9: CONSTANTS              ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Using constants ---\n";
    cout << "  PI: " << Constants::PI << "\n";
    cout << "  MAX_SIZE: " << Constants::MAX_SIZE << "\n";
    cout << "  VERSION: " << Constants::VERSION << "\n";

    cout << "\n💡 RULES:\n";
    cout << "   • static constexpr: Can initialize in header\n";
    cout << "   • static const (non-integral): Must initialize in .cpp\n";
    cout << "   • static const (integral): Can initialize in header (C++11)\n";
}

// ============================================
// EXAMPLE 10: PROPER FILE ORGANIZATION
// ============================================

void demonstrateFileOrganization() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 10: FILE ORGANIZATION     ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "PROPER PROJECT STRUCTURE:\n";
    cout << "\n";
    cout << "project/\n";
    cout << "├── include/              (Header files)\n";
    cout << "│   ├── Person.h\n";
    cout << "│   ├── Car.h\n";
    cout << "│   └── Engine.h\n";
    cout << "├── src/                  (Source files)\n";
    cout << "│   ├── Person.cpp\n";
    cout << "│   ├── Car.cpp\n";
    cout << "│   └── Engine.cpp\n";
    cout << "└── main.cpp              (Main program)\n";

    cout << "\nCOMPILATION:\n";
    cout << "\n";
    cout << "# Compile each .cpp to object file\n";
    cout << "g++ -c -Iinclude src/Person.cpp -o obj/Person.o\n";
    cout << "g++ -c -Iinclude src/Car.cpp -o obj/Car.o\n";
    cout << "g++ -c -Iinclude src/Engine.cpp -o obj/Engine.o\n";
    cout << "g++ -c -Iinclude main.cpp -o obj/main.o\n";
    cout << "\n";
    cout << "# Link all object files\n";
    cout << "g++ obj/*.o -o program\n";

    cout << "\n✅ BENEFITS:\n";
    cout << "   • Clear separation of interface and implementation\n";
    cout << "   • Easy to find files\n";
    cout << "   • Better modularity\n";
    cout << "   • Scalable to large projects\n";

    cout << "\nALTERNATIVE STRUCTURE (Single directory):\n";
    cout << "\n";
    cout << "project/\n";
    cout << "├── Person.h\n";
    cout << "├── Person.cpp\n";
    cout << "├── Car.h\n";
    cout << "├── Car.cpp\n";
    cout << "├── Engine.h\n";
    cout << "├── Engine.cpp\n";
    cout << "└── main.cpp\n";
    cout << "\n";
    cout << "# Simpler for small projects\n";
    cout << "g++ *.cpp -o program\n";
}

// ============================================
// MAIN FUNCTION - RUN ALL EXAMPLES
// ============================================

int main() {
    cout << "\n";
    cout << "╔═══════════════════════════════════════════════════════════╗\n";
    cout << "║                                                           ║\n";
    cout << "║         HEADER/SOURCE SEPARATION IN C++                  ║\n";
    cout << "║         Separating Interface from Implementation          ║\n";
    cout << "║                                                           ║\n";
    cout << "╚═══════════════════════════════════════════════════════════╝\n";

    demonstrateSingleFile();
    demonstrateSeparation();
    demonstrateIncludeGuards();
    demonstratePragmaOnce();
    demonstrateInlineFunctions();
    demonstrateForwardDeclarations();
    demonstrateStaticMembers();
    demonstrateTemplates();
    demonstrateConstants();
    demonstrateFileOrganization();

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
    cout << "WHAT IS HEADER/SOURCE SEPARATION?\n";
    cout << "========================================\n";
    cout << "Header/Source separation is the practice of:\n";
    cout << "  • Putting class DECLARATIONS in header files (.h/.hpp)\n";
    cout << "  • Putting class IMPLEMENTATIONS in source files (.cpp)\n";
    cout << "\n";
    cout << "HEADER FILE (.h/.hpp):\n";
    cout << "  • Class declarations\n";
    cout << "  • Function prototypes\n";
    cout << "  • Inline functions (small ones)\n";
    cout << "  • Constants and enums\n";
    cout << "  • Template definitions\n";
    cout << "  • The 'WHAT' (interface)\n";
    cout << "\n";
    cout << "SOURCE FILE (.cpp):\n";
    cout << "  • Function implementations\n";
    cout << "  • Member function definitions\n";
    cout << "  • Static member initialization\n";
    cout << "  • Complex logic\n";
    cout << "  • The 'HOW' (implementation)\n";

    cout << "\n========================================\n";
    cout << "WHY SEPARATE?\n";
    cout << "========================================\n";
    cout << "1. FASTER COMPILATION:\n";
    cout << "   • Change .cpp → recompile only that file\n";
    cout << "   • Change .h → recompile all files that include it\n";
    cout << "   • Keep headers minimal!\n";
    cout << "\n";
    cout << "2. INFORMATION HIDING:\n";
    cout << "   • Users see interface (header)\n";
    cout << "   • Implementation details hidden (.cpp)\n";
    cout << "   • Encapsulation at file level\n";
    cout << "\n";
    cout << "3. REUSABILITY:\n";
    cout << "   • One header, multiple implementations\n";
    cout << "   • Platform-specific .cpp files\n";
    cout << "   • Different algorithms, same interface\n";
    cout << "\n";
    cout << "4. MODULARITY:\n";
    cout << "   • Clear module boundaries\n";
    cout << "   • Easy to create libraries\n";
    cout << "   • Better code organization\n";
    cout << "\n";
    cout << "5. TEAM COLLABORATION:\n";
    cout << "   • Multiple people work on different .cpp files\n";
    cout << "   • Less merge conflicts\n";
    cout << "   • Clearer responsibilities\n";

    cout << "\n========================================\n";
    cout << "BASIC STRUCTURE\n";
    cout << "========================================\n";
    cout << "// Person.h (Header file)\n";
    cout << "#pragma once\n";
    cout << "#include <string>\n";
    cout << "\n";
    cout << "class Person {\n";
    cout << "private:\n";
    cout << "    std::string name;\n";
    cout << "    int age;\n";
    cout << "    \n";
    cout << "public:\n";
    cout << "    // Declarations only\n";
    cout << "    Person(const std::string& n, int a);\n";
    cout << "    ~Person();\n";
    cout << "    \n";
    cout << "    void display() const;\n";
    cout << "    int getAge() const;\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Person.cpp (Source file)\n";
    cout << "#include \"Person.h\"\n";
    cout << "#include <iostream>\n";
    cout << "\n";
    cout << "// Implementations\n";
    cout << "Person::Person(const std::string& n, int a) \n";
    cout << "    : name(n), age(a) { }\n";
    cout << "\n";
    cout << "Person::~Person() { }\n";
    cout << "\n";
    cout << "void Person::display() const {\n";
    cout << "    std::cout << name << \", \" << age << std::endl;\n";
    cout << "}\n";
    cout << "\n";
    cout << "int Person::getAge() const {\n";
    cout << "    return age;\n";
    cout << "}\n";

    cout << "\n========================================\n";
    cout << "INCLUDE GUARDS vs #pragma once\n";
    cout << "========================================\n";
    cout << "TRADITIONAL (Include Guards):\n";
    cout << "#ifndef PERSON_H\n";
    cout << "#define PERSON_H\n";
    cout << "// ... class definition ...\n";
    cout << "#endif // PERSON_H\n";
    cout << "\n";
    cout << "MODERN (#pragma once):\n";
    cout << "#pragma once\n";
    cout << "// ... class definition ...\n";
    cout << "\n";
    cout << "COMPARISON:\n";
    cout << "Include Guards:\n";
    cout << "  ✅ Standard C++\n";
    cout << "  ✅ Works everywhere\n";
    cout << "  ❌ More code\n";
    cout << "  ❌ Easy to make mistakes\n";
    cout << "\n";
    cout << "#pragma once:\n";
    cout << "  ✅ Simpler (1 line!)\n";
    cout << "  ✅ Faster compilation\n";
    cout << "  ✅ No naming conflicts\n";
    cout << "  ✅ Supported by all modern compilers\n";
    cout << "  ❌ Not in C++ standard (but universally supported)\n";
    cout << "\n";
    cout << "RECOMMENDATION: Use #pragma once for new projects!\n";

    cout << "\n========================================\n";
    cout << "FORWARD DECLARATIONS\n";
    cout << "========================================\n";
    cout << "Forward declarations reduce dependencies:\n";
    cout << "\n";
    cout << "// Car.h\n";
    cout << "#pragma once\n";
    cout << "#include <string>\n";
    cout << "\n";
    cout << "class Engine;  // Forward declaration\n";
    cout << "\n";
    cout << "class Car {\n";
    cout << "private:\n";
    cout << "    Engine* engine;  // Pointer - forward decl OK\n";
    cout << "    std::string model;\n";
    cout << "    \n";
    cout << "public:\n";
    cout << "    Car(const std::string& m);\n";
    cout << "    ~Car();\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Car.cpp\n";
    cout << "#include \"Car.h\"\n";
    cout << "#include \"Engine.h\"  // Full definition here\n";
    cout << "\n";
    cout << "BENEFITS:\n";
    cout << "  • Car.h doesn't need Engine.h\n";
    cout << "  • Reduces compilation dependencies\n";
    cout << "  • Faster builds\n";
    cout << "  • Breaks circular dependencies\n";
    cout << "\n";
    cout << "WHEN TO USE:\n";
    cout << "  ✅ Pointers to class\n";
    cout << "  ✅ References to class\n";
    cout << "  ❌ Class members by value (need full definition)\n";
    cout << "  ❌ Inheritance (need full definition)\n";

    cout << "\n========================================\n";
    cout << "INLINE FUNCTIONS\n";
    cout << "========================================\n";
    cout << "Small functions can be inline in header:\n";
    cout << "\n";
    cout << "class Person {\n";
    cout << "private:\n";
    cout << "    int age;\n";
    cout << "    \n";
    cout << "public:\n";
    cout << "    // Simple getter - inline in header\n";
    cout << "    int getAge() const { return age; }\n";
    cout << "    \n";
    cout << "    // Simple setter - inline in header\n";
    cout << "    void setAge(int a) { age = a; }\n";
    cout << "    \n";
    cout << "    // Complex function - declare only\n";
    cout << "    void complexCalculation();\n";
    cout << "};\n";
    cout << "\n";
    cout << "RULE OF THUMB:\n";
    cout << "  ≤ 3 lines → Can inline in header\n";
    cout << "  > 3 lines → Put in .cpp file\n";
    cout << "\n";
    cout << "BENEFITS OF INLINE:\n";
    cout << "  • Faster execution (no function call overhead)\n";
    cout << "  • Simple getters/setters\n";
    cout << "\n";
    cout << "DRAWBACKS:\n";
    cout << "  • Code in header → slower compilation\n";
    cout << "  • Changes require recompilation\n";

    cout << "\n========================================\n";
    cout << "STATIC MEMBERS\n";
    cout << "========================================\n";
    cout << "Static members need special handling:\n";
    cout << "\n";
    cout << "// BankAccount.h\n";
    cout << "class BankAccount {\n";
    cout << "private:\n";
    cout << "    static int accountCount;  // Declaration\n";
    cout << "    \n";
    cout << "public:\n";
    cout << "    BankAccount();\n";
    cout << "    static int getCount();\n";
    cout << "};\n";
    cout << "\n";
    cout << "// BankAccount.cpp\n";
    cout << "#include \"BankAccount.h\"\n";
    cout << "\n";
    cout << "// Definition - MUST be in .cpp!\n";
    cout << "int BankAccount::accountCount = 0;\n";
    cout << "\n";
    cout << "BankAccount::BankAccount() {\n";
    cout << "    accountCount++;\n";
    cout << "}\n";
    cout << "\n";
    cout << "int BankAccount::getCount() {\n";
    cout << "    return accountCount;\n";
    cout << "}\n";
    cout << "\n";
    cout << "EXCEPTION (C++17):\n";
    cout << "static inline int accountCount = 0;  // Can be in header\n";

    cout << "\n========================================\n";
    cout << "TEMPLATES (SPECIAL CASE)\n";
    cout << "========================================\n";
    cout << "Templates MUST be fully defined in headers:\n";
    cout << "\n";
    cout << "// Box.h\n";
    cout << "#pragma once\n";
    cout << "\n";
    cout << "template<typename T>\n";
    cout << "class Box {\n";
    cout << "private:\n";
    cout << "    T value;\n";
    cout << "    \n";
    cout << "public:\n";
    cout << "    Box(T v) : value(v) { }  // Must be here\n";
    cout << "    \n";
    cout << "    T getValue() const {     // Must be here\n";
    cout << "        return value;\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "WHY?\n";
    cout << "  • Compiler needs full code to instantiate\n";
    cout << "  • Templates are NOT compiled until used\n";
    cout << "  • Each instantiation needs the full code\n";
    cout << "\n";
    cout << "WORKAROUNDS (if separation needed):\n";
    cout << "  1. Include .cpp at end of .h\n";
    cout << "  2. Use .hpp extension\n";
    cout << "  3. Explicit template instantiation (advanced)\n";

    cout << "\n========================================\n";
    cout << "COMPILATION PROCESS\n";
    cout << "========================================\n";
    cout << "Step-by-step:\n";
    cout << "\n";
    cout << "1. PREPROCESSING:\n";
    cout << "   • #include copies header contents\n";
    cout << "   • Macros expanded\n";
    cout << "   • Conditional compilation\n";
    cout << "\n";
    cout << "2. COMPILATION:\n";
    cout << "   Person.cpp → Person.o (object file)\n";
    cout << "   Car.cpp → Car.o\n";
    cout << "   main.cpp → main.o\n";
    cout << "\n";
    cout << "3. LINKING:\n";
    cout << "   Person.o + Car.o + main.o → program (executable)\n";
    cout << "\n";
    cout << "COMMANDS:\n";
    cout << "# Compile to object files\n";
    cout << "g++ -c Person.cpp -o Person.o\n";
    cout << "g++ -c Car.cpp -o Car.o\n";
    cout << "g++ -c main.cpp -o main.o\n";
    cout << "\n";
    cout << "# Link object files\n";
    cout << "g++ Person.o Car.o main.o -o program\n";
    cout << "\n";
    cout << "# Or all at once\n";
    cout << "g++ Person.cpp Car.cpp main.cpp -o program\n";

    cout << "\n========================================\n";
    cout << "FILE ORGANIZATION PATTERNS\n";
    cout << "========================================\n";
    cout << "PATTERN 1: Small Projects\n";
    cout << "project/\n";
    cout << "├── MyClass.h\n";
    cout << "├── MyClass.cpp\n";
    cout << "└── main.cpp\n";
    cout << "\n";
    cout << "PATTERN 2: Medium Projects\n";
    cout << "project/\n";
    cout << "├── include/\n";
    cout << "│   └── MyClass.h\n";
    cout << "├── src/\n";
    cout << "│   └── MyClass.cpp\n";
    cout << "└── main.cpp\n";
    cout << "\n";
    cout << "PATTERN 3: Large Projects\n";
    cout << "project/\n";
    cout << "├── include/\n";
    cout << "│   └── myproject/\n";
    cout << "│       ├── MyClass.h\n";
    cout << "│       └── Utils.h\n";
    cout << "├── src/\n";
    cout << "│   ├── MyClass.cpp\n";
    cout << "│   └── Utils.cpp\n";
    cout << "├── tests/\n";
    cout << "│   └── test_MyClass.cpp\n";
    cout << "└── main.cpp\n";

    cout << "\n========================================\n";
    cout << "BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "✅ DO:\n";
    cout << "  • Use #pragma once (simpler than guards)\n";
    cout << "  • Forward declare when possible\n";
    cout << "  • Keep headers minimal\n";
    cout << "  • Include what you use (IWYU)\n";
    cout << "  • Put complex code in .cpp\n";
    cout << "  • Organize by module/feature\n";
    cout << "  • Use consistent naming\n";
    cout << "  • Document headers well\n";
    cout << "\n";
    cout << "❌ DON'T:\n";
    cout << "  • Put implementations in headers (except templates)\n";
    cout << "  • Use 'using namespace' in headers\n";
    cout << "  • Forget include guards/#pragma once\n";
    cout << "  • Include unnecessary headers\n";
    cout << "  • Mix declarations and implementations\n";
    cout << "  • Forget to initialize static members\n";
    cout << "  • Put large functions inline\n";
    cout << "  • Create circular dependencies\n";

    cout << "\n========================================\n";
    cout << "COMMON MISTAKES\n";
    cout << "========================================\n";
    cout << "❌ Mistake 1: Implementation in Header\n";
    cout << "// Bad.h\n";
    cout << "class Bad {\n";
    cout << "    void complexFunction() {\n";
    cout << "        // 50 lines of code here!\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "Fix: Move to .cpp!\n";
    cout << "\n";
    cout << "❌ Mistake 2: Forgetting Include Guard\n";
    cout << "// Person.h (missing guard)\n";
    cout << "class Person { };\n";
    cout << "// Results in multiple definition errors!\n";
    cout << "\n";
    cout << "Fix: Add #pragma once\n";
    cout << "\n";
    cout << "❌ Mistake 3: Not Initializing Static Members\n";
    cout << "// Person.h\n";
    cout << "class Person {\n";
    cout << "    static int count;\n";
    cout << "};\n";
    cout << "// Forgot: int Person::count = 0; in .cpp\n";
    cout << "\n";
    cout << "❌ Mistake 4: Circular Dependencies\n";
    cout << "// A.h includes B.h\n";
    cout << "// B.h includes A.h\n";
    cout << "// Compilation fails!\n";
    cout << "\n";
    cout << "Fix: Use forward declarations\n";

    cout << "\n========================================\n";
    cout << "WHEN TO USE WHAT\n";
    cout << "========================================\n";
    cout << "IN HEADER (.h/.hpp):\n";
    cout << "  ✅ Class declarations\n";
    cout << "  ✅ Function prototypes\n";
    cout << "  ✅ Small inline functions (≤3 lines)\n";
    cout << "  ✅ Template definitions (must!)\n";
    cout << "  ✅ constexpr constants\n";
    cout << "  ✅ Enums\n";
    cout << "  ✅ Forward declarations\n";
    cout << "\n";
    cout << "IN SOURCE (.cpp):\n";
    cout << "  ✅ Function implementations\n";
    cout << "  ✅ Member function definitions\n";
    cout << "  ✅ Static member initialization\n";
    cout << "  ✅ Complex algorithms\n";
    cout << "  ✅ Private helper functions\n";
    cout << "  ✅ File-local utilities\n";

    cout << "\n========================================\n";
    cout << "QUICK REFERENCE\n";
    cout << "========================================\n";
    cout << "// MyClass.h\n";
    cout << "#pragma once\n";
    cout << "#include <string>\n";
    cout << "\n";
    cout << "class MyClass {\n";
    cout << "private:\n";
    cout << "    int value;\n";
    cout << "    static int count;\n";
    cout << "    \n";
    cout << "public:\n";
    cout << "    MyClass(int v);\n";
    cout << "    ~MyClass();\n";
    cout << "    \n";
    cout << "    // Inline (simple)\n";
    cout << "    int getValue() const { return value; }\n";
    cout << "    \n";
    cout << "    // In .cpp (complex)\n";
    cout << "    void complexOperation();\n";
    cout << "    \n";
    cout << "    static int getCount();\n";
    cout << "};\n";
    cout << "\n";
    cout << "// MyClass.cpp\n";
    cout << "#include \"MyClass.h\"\n";
    cout << "\n";
    cout << "// Static member initialization\n";
    cout << "int MyClass::count = 0;\n";
    cout << "\n";
    cout << "MyClass::MyClass(int v) : value(v) {\n";
    cout << "    count++;\n";
    cout << "}\n";
    cout << "\n";
    cout << "MyClass::~MyClass() {\n";
    cout << "    count--;\n";
    cout << "}\n";
    cout << "\n";
    cout << "void MyClass::complexOperation() {\n";
    cout << "    // Implementation here\n";
    cout << "}\n";
    cout << "\n";
    cout << "int MyClass::getCount() {\n";
    cout << "    return count;\n";
    cout << "}\n";

    cout << "\n========================================\n";
    cout << "KEY TAKEAWAYS\n";
    cout << "========================================\n";
    cout << "1. Separate interface (header) from implementation (source)\n";
    cout << "2. Use #pragma once (modern and simpler)\n";
    cout << "3. Forward declare to reduce dependencies\n";
    cout << "4. Keep headers minimal for faster compilation\n";
    cout << "5. Inline only simple functions (≤3 lines)\n";
    cout << "6. Initialize static members in .cpp\n";
    cout << "7. Templates must be in headers\n";
    cout << "8. Never 'using namespace' in headers\n";
    cout << "9. Include what you use (IWYU principle)\n";
    cout << "10. Organize by feature/module for clarity\n";

    cout << "\n========================================\n";
    cout << "GOLDEN RULES\n";
    cout << "========================================\n";
    cout << "⚠️  RULE 1: Always use include guards or #pragma once\n";
    cout << "   Without them, you'll get multiple definition errors!\n";
    cout << "\n";
    cout << "⚠️  RULE 2: Keep implementation in .cpp files\n";
    cout << "   Headers should be declarations, not definitions!\n";
    cout << "\n";
    cout << "⚠️  RULE 3: Forward declare whenever possible\n";
    cout << "   Reduces compilation time and dependencies!\n";
    cout << "\n";
    cout << "⚠️  RULE 4: Templates are the exception\n";
    cout << "   They MUST be fully defined in headers!\n";
    cout << "\n";
    cout << "⚠️  RULE 5: Initialize static members in .cpp\n";
    cout << "   (Except static constexpr in C++11+)\n";

    return 0;
}
