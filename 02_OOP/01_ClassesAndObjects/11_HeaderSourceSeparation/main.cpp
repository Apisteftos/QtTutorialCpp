#include <print>
#include <string>
using namespace std;  // fine in a .cpp, but NEVER in a header

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
 * - Static member initialization (or `static inline` in the header, C++17)
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
 * - Use include guards or #pragma once (pick ONE style per project)
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
        std::println("  PersonSingleFile constructor: {}", name);
    }

    // Destructor
    ~PersonSingleFile() {
        std::println("  PersonSingleFile destructor: {}", name);
    }

    // Methods
    void display() const {
        std::println("  Name: {}, Age: {}", name, age);
    }

    void setAge(int a) {
        age = a;
    }

    int getAge() const {
        return age;
    }
};

void demonstrateSingleFile() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: SINGLE FILE (Before)   ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating person ---");
    PersonSingleFile person("Alice", 30);

    std::println("");
    std::println("--- Using person ---");
    person.display();

    std::println("");
    std::println("--- Modifying person ---");
    person.setAge(31);
    person.display();
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
    std::println("  PersonSeparated constructor: {}", name);
}

// Destructor implementation
PersonSeparated::~PersonSeparated() {
    std::println("  PersonSeparated destructor: {}", name);
}

// Method implementations
void PersonSeparated::display() const {
    std::println("  Name: {}, Age: {}", name, age);
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
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: SEPARATED (Better)     ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating person ---");
    PersonSeparated person("Bob", 25);

    std::println("");
    std::println("--- Using person ---");
    person.display();

    std::println("");
    std::println("--- Modifying person ---");
    person.setAge(26);
    person.display();
}

// ============================================
// EXAMPLE 3: INCLUDE GUARDS (Traditional)
// ============================================

// Simulate "the same header included twice" inside this one file.
// The guard makes the second copy disappear in the preprocessor.

// --- first inclusion of "guard_demo.h" ---
#ifndef GUARD_DEMO_H
#define GUARD_DEMO_H
inline int guardDemoValue() { return 42; }
#endif // GUARD_DEMO_H

// --- second inclusion of the SAME text (skipped: GUARD_DEMO_H is already defined) ---
#ifndef GUARD_DEMO_H
#define GUARD_DEMO_H
inline int guardDemoValue() { return 42; }
#endif // GUARD_DEMO_H

void demonstrateIncludeGuards() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: INCLUDE GUARDS         ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- The same 'header' text pasted twice ---");
    std::println("  guardDemoValue() = {}", guardDemoValue());
    std::println("  The 2nd copy was skipped by the preprocessor (GUARD_DEMO_H already defined)");
    std::println("  Without the guard: error: redefinition of 'int guardDemoValue()'");
}

// ============================================
// EXAMPLE 4: #pragma once (Modern)
// ============================================

void demonstratePragmaOnce() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: #pragma once (Modern)  ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("  #pragma once works per FILE, so it cannot be shown inside this single main.cpp");
    std::println("  (GCC even warns: '#pragma once in main file')");
    std::println("");
    std::println("✅ Either style is fine: use ONE of them consistently per project");
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
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 5: INLINE FUNCTIONS       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Using calculator ---");
    Calculator calc;
    calc.setValue(10);
    std::println("  Value: {}", calc.getValue());
    std::println("  Compute: {}", calc.compute(5, 3));
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

    // Car owns the raw Engine*, so a copy would delete it twice
    Car(const Car&) = delete;
    Car& operator=(const Car&) = delete;

    void start();
    void display() const;
};

// Now the full Engine definition (would be in Engine.h)
class Engine {
private:
    int horsepower;

public:
    Engine(int hp) : horsepower(hp) {
        std::println("  Engine created: {} HP", hp);
    }

    ~Engine() {
        std::println("  Engine destroyed");
    }

    int getHorsepower() const { return horsepower; }
};

// Car implementation (would be in Car.cpp)
Car::Car(const string& m) : engine(new Engine(200)), model(m) {
    std::println("  Car created: {}", model);
}

Car::~Car() {
    delete engine;
    std::println("  Car destroyed: {}", model);
}

void Car::start() {
    std::println("  Starting {} with {} HP engine", model, engine->getHorsepower());
}

void Car::display() const {
    std::println("  Model: {}, Engine: {} HP", model, engine->getHorsepower());
}

void demonstrateForwardDeclarations() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 6: FORWARD DECLARATIONS   ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating car ---");
    Car car("Tesla Model S");

    std::println("");
    std::println("--- Using car ---");
    car.start();
    car.display();
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

    // A copy would not increment accountCount, but its destructor would decrement it
    BankAccount(const BankAccount&) = delete;
    BankAccount& operator=(const BankAccount&) = delete;

    static int getAccountCount();
    void display() const;
};

// Static member MUST be initialized in .cpp file
int BankAccount::accountCount = 0;

BankAccount::BankAccount(const string& o, double b)
    : owner(o), balance(b) {
    accountCount++;
    std::println("  Account created for {} (Total accounts: {})", owner, accountCount);
}

BankAccount::~BankAccount() {
    accountCount--;
    std::println("  Account closed for {} (Remaining: {})", owner, accountCount);
}

int BankAccount::getAccountCount() {
    return accountCount;
}

void BankAccount::display() const {
    std::println("  Owner: {}, Balance: ${:.2f}", owner, balance);
}

void demonstrateStaticMembers() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 7: STATIC MEMBERS         ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating accounts ---");
    BankAccount* acc1 = new BankAccount("Alice", 1000.0);
    BankAccount* acc2 = new BankAccount("Bob", 2000.0);
    BankAccount* acc3 = new BankAccount("Charlie", 3000.0);

    std::println("");
    std::println("--- Account count ---");
    std::println("  Total accounts: {}", BankAccount::getAccountCount());

    std::println("");
    std::println("--- Closing accounts ---");
    delete acc1;
    delete acc2;

    std::println("");
    std::println("--- Account count after closing ---");
    std::println("  Remaining accounts: {}", BankAccount::getAccountCount());

    delete acc3;
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
        std::println("  Box created with value");
    }

    T getValue() const { return value; }
    void setValue(T v) { value = v; }

    void display() const {
        std::println("  Box contains: {}", value);
    }
};

void demonstrateTemplates() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 8: TEMPLATES (Special)    ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating boxes ---");
    Box<int> intBox(42);
    Box<string> strBox("Hello");
    Box<double> dblBox(3.14);

    std::println("");
    std::println("--- Using boxes ---");
    intBox.display();
    strBox.display();
    dblBox.display();
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
// (C++17 alternative that needs no .cpp: static inline const string VERSION = "1.0.0";)
const string Constants::VERSION = "1.0.0";

void demonstrateConstants() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 9: CONSTANTS              ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Using constants ---");
    std::println("  PI: {:g}", Constants::PI);
    std::println("  MAX_SIZE: {}", Constants::MAX_SIZE);
    std::println("  VERSION: {}", Constants::VERSION);
}

// ============================================
// EXAMPLE 10: PROPER FILE ORGANIZATION
// ============================================

void demonstrateFileOrganization() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 10: FILE ORGANIZATION     ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("  Small project:   Person.h Person.cpp main.cpp    ->  g++ *.cpp -o program");
    std::println("  Medium project:  include/*.h src/*.cpp main.cpp  ->  g++ -Iinclude src/*.cpp main.cpp -o program");
    std::println("  (all layouts and the separate compile + link steps: see the README)");
}

// ============================================
// MAIN FUNCTION - RUN ALL EXAMPLES
// ============================================

int main() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════════════╗");
    std::println("║                                                           ║");
    std::println("║         HEADER/SOURCE SEPARATION IN C++                  ║");
    std::println("║         Separating Interface from Implementation          ║");
    std::println("║                                                           ║");
    std::println("╚═══════════════════════════════════════════════════════════╝");

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

    return 0;
}
