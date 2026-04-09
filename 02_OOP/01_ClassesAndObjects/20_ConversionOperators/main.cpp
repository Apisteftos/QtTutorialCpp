#include <iostream>
#include <string>
#include <cmath>
using namespace std;

// ============================================
// WHAT ARE CONVERSION OPERATORS?
// ============================================

/*
 * CONVERSION OPERATORS:
 * - Allow objects to be converted to other types
 * - Also called "type cast operators"
 * - Enable implicit or explicit conversions
 * - Syntax: operator Type() const
 * - No return type specified (it's implicit)
 *
 * TYPES OF CONVERSIONS:
 * 1. Implicit conversions (automatic)
 * 2. Explicit conversions (must use cast)
 *
 * WHY USE CONVERSION OPERATORS?
 * - Natural type conversions
 * - Integrate with built-in types
 * - Enable polymorphic behavior
 * - Simplify usage in conditions
 * - Make custom types work like built-in types
 *
 * SYNTAX:
 * operator Type() const { return ...; }
 * explicit operator Type() const { return ...; }
 *
 * COMMON USES:
 * - operator bool() - use in conditions
 * - operator int() - convert to integer
 * - operator double() - convert to floating-point
 * - operator string() - convert to string
 *
 * BEST PRACTICES:
 * - Use explicit for most conversions
 * - Only implicit for safe, obvious conversions
 * - operator bool should almost always be explicit
 * - Make const (doesn't modify object)
 * - Consider safety vs convenience
 *
 * IMPLICIT VS EXPLICIT:
 * - Implicit: Automatic conversion (can be dangerous)
 * - Explicit: Requires static_cast or explicit syntax
 * - C++11 added explicit conversion operators
 */

// ============================================
// EXAMPLE 1: BASIC CONVERSION OPERATOR
// ============================================

class Fraction {
private:
    int numerator;
    int denominator;

public:
    Fraction(int n, int d) : numerator(n), denominator(d) {
        cout << "  Fraction created: " << n << "/" << d << "\n";
    }

    // ✅ Conversion operator to double
    operator double() const {
        cout << "  Converting to double...\n";
        return static_cast<double>(numerator) / denominator;
    }

    void display() const {
        cout << "  Fraction: " << numerator << "/" << denominator << "\n";
    }
};

void demonstrateBasic() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: BASIC CONVERSION      ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "--- Creating fraction ---\n";
    Fraction f(3, 4);
    f.display();

    cout << "\n--- Implicit conversion to double ---\n";
    double d = f;  // ⚠️ Implicit conversion!
    cout << "  Double value: " << d << "\n";

    cout << "\n--- Using in arithmetic ---\n";
    double result = f + 0.5;  // ⚠️ Converts f to double
    cout << "  Result: " << result << "\n";

    cout << "\n💡 BASIC CONVERSION OPERATOR:\n";
    cout << "   • operator Type() const\n";
    cout << "   • No return type specified\n";
    cout << "   • Called automatically\n";
    cout << "   • Can be implicit or explicit\n";
}

// ============================================
// EXAMPLE 2: THE PROBLEM WITH IMPLICIT CONVERSIONS
// ============================================

class IntegerBad {
private:
    int value;

public:
    IntegerBad(int v) : value(v) {
        cout << "  IntegerBad(" << v << ")\n";
    }

    // ❌ Non-explicit bool conversion - dangerous!
    explicit operator bool() const {
        cout << "  Converting to bool...\n";
        return value != 0;
    }

    // ❌ Non-explicit int conversion
    explicit operator int() const {
        cout << "  Converting to int...\n";
        return value;
    }
};

void demonstrateProblem() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: THE PROBLEM           ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    IntegerBad num(42);

    cout << "--- Implicit conversions happen unexpectedly ---\n";
    // int x = num;  // ❌ won't compile
    int x = static_cast<int>(num);  // ✅ explicit
    cout << "  x = " << x << "\n";

    cout << "\n--- Unwanted arithmetic ---\n";
    // int result = num + 10; // ❌ won't compile
    int result = static_cast<int>(num) + 10; // ✅ explicit
    cout << "  result = " << result << "\n";

    cout << "\n--- Ambiguous operations ---\n";
    // bool b = num;  // ❌ won't compile
    bool b = static_cast<bool>(num);  // ✅ explicit
    cout << "  bool = " << (b ? "true" : "false") << "\n";

    cout << "\n--- Can even multiply! ---\n";
    // int product = num * 2;  // ❌ won't compile
    int product = static_cast<int>(num) * 2;  // ✅ explicit
    cout << "  product = " << product << "\n";

    cout << "\n⚠️  THE PROBLEM:\n";
    cout << "   • Too many implicit conversions\n";
    cout << "   • Unexpected behavior\n";
    cout << "   • Can lead to bugs\n";
    cout << "   • Hard to track conversions\n";
}

// ============================================
// EXAMPLE 3: EXPLICIT CONVERSION OPERATORS
// ============================================

class IntegerGood {
private:
    int value;

public:
    IntegerGood(int v) : value(v) {
        cout << "  IntegerGood(" << v << ")\n";
    }

    // ✅ Explicit bool conversion - safe!
    explicit operator bool() const {
        cout << "  Explicit bool conversion\n";
        return value != 0;
    }

    // ✅ Explicit int conversion
    explicit operator int() const {
        cout << "  Explicit int conversion\n";
        return value;
    }

    void display() const {
        cout << "  Value: " << value << "\n";
    }
};

void demonstrateExplicit() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: EXPLICIT CONVERSION   ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    IntegerGood num(42);

    cout << "--- Implicit conversions prevented ---\n";
    // int x = num;  // ❌ Error! Must be explicit
    cout << "  // int x = num;  // ❌ Compilation error!\n";

    cout << "\n--- Explicit conversion required ---\n";
    int x = static_cast<int>(num);  // ✅ Must use cast
    cout << "  x = " << x << "\n";

    cout << "\n--- Bool in conditions (contextual) ---\n";
    if (num) {  // ✅ Works! Contextual conversion
        cout << "  num is non-zero (contextual bool)\n";
    }

    cout << "\n--- Explicit cast needed for arithmetic ---\n";
    // int result = num + 10;  // ❌ Error!
    int result = static_cast<int>(num) + 10;  // ✅ Explicit
    cout << "  result = " << result << "\n";

    cout << "\n✅ THE SOLUTION:\n";
    cout << "   • explicit prevents implicit conversions\n";
    cout << "   • Must use static_cast\n";
    cout << "   • Works in boolean contexts (if, while)\n";
    cout << "   • Safer, more intentional code\n";
}

// ============================================
// EXAMPLE 4: OPERATOR BOOL FOR SMART POINTERS
// ============================================

template<typename T>
class SmartPointer {
private:
    T* ptr;

public:
    SmartPointer(T* p = nullptr) : ptr(p) {
        if (ptr) {
            cout << "  SmartPointer acquired resource\n";
        } else {
            cout << "  SmartPointer is null\n";
        }
    }

    ~SmartPointer() {
        delete ptr;
        cout << "  SmartPointer released resource\n";
    }

    // ✅ Explicit bool conversion
    explicit operator bool() const {
        return ptr != nullptr;
    }

    T& operator*() const {
        return *ptr;
    }

    T* operator->() const {
        return ptr;
    }

    T* get() const {
        return ptr;
    }
};

void demonstrateSmartPointer() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: SMART POINTER BOOL    ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "--- Creating valid smart pointer ---\n";
    SmartPointer<int> sp1(new int(42));

    if (sp1) {  // ✅ Contextual bool conversion
        cout << "  sp1 is valid, value: " << *sp1 << "\n";
    }

    cout << "\n--- Creating null smart pointer ---\n";
    SmartPointer<int> sp2(nullptr);

    if (!sp2) {  // ✅ Contextual bool conversion
        cout << "  sp2 is null\n";
    }

    cout << "\n--- Using in logical operations ---\n";
    if (sp1 && !sp2) {  // ✅ Both work in boolean context
        cout << "  sp1 valid and sp2 null\n";
    }

    cout << "\n💡 SMART POINTER PATTERN:\n";
    cout << "   • explicit operator bool()\n";
    cout << "   • Works in if, while, for conditions\n";
    cout << "   • Prevents accidental conversions\n";
    cout << "   • Same as std::unique_ptr, std::shared_ptr\n";
}

// ============================================
// EXAMPLE 5: MULTIPLE CONVERSION OPERATORS
// ============================================

class Complex {
private:
    double real;
    double imag;

public:
    Complex(double r, double i) : real(r), imag(i) {
        cout << "  Complex(" << r << ", " << i << ")\n";
    }

    // ✅ Explicit conversion to double (magnitude)
    explicit operator double() const {
        cout << "  Converting to double (magnitude)...\n";
        return sqrt(real * real + imag * imag);
    }

    // ✅ Explicit conversion to bool
    explicit operator bool() const {
        cout << "  Converting to bool...\n";
        return (real != 0) || (imag != 0);
    }

    void display() const {
        cout << "  Complex: " << real;
        if (imag >= 0) cout << " + " << imag << "i\n";
        else cout << " - " << -imag << "i\n";
    }
};

void demonstrateMultiple() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: MULTIPLE CONVERSIONS  ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    Complex c(3.0, 4.0);
    c.display();

    cout << "\n--- Converting to double (magnitude) ---\n";
    double magnitude = static_cast<double>(c);
    cout << "  Magnitude: " << magnitude << "\n";

    cout << "\n--- Using in boolean context ---\n";
    if (c) {  // ✅ Contextual bool conversion
        cout << "  Complex number is non-zero\n";
    }

    cout << "\n--- Zero complex number ---\n";
    Complex zero(0.0, 0.0);
    zero.display();

    if (!zero) {  // ✅ Contextual bool conversion
        cout << "  Complex number is zero\n";
    }

    cout << "\n💡 MULTIPLE CONVERSIONS:\n";
    cout << "   • Can have multiple conversion operators\n";
    cout << "   • Each converts to different type\n";
    cout << "   • All should be explicit (usually)\n";
    cout << "   • Clear purpose for each\n";
}

// ============================================
// EXAMPLE 6: STRING CONVERSION
// ============================================

class Person {
private:
    string firstName;
    string lastName;
    int age;

public:
    Person(const string& first, const string& last, int a)
        : firstName(first), lastName(last), age(a) {
        cout << "  Person created: " << firstName << " " << lastName << "\n";
    }

    // ✅ Explicit string conversion
    explicit operator string() const {
        cout << "  Converting to string...\n";
        return firstName + " " + lastName + " (age " + to_string(age) + ")";
    }

    void display() const {
        cout << "  Name: " << firstName << " " << lastName << "\n";
        cout << "  Age: " << age << "\n";
    }
};

void demonstrateString() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: STRING CONVERSION     ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    Person p("Alice", "Johnson", 30);
    p.display();

    cout << "\n--- Converting to string ---\n";
    string str = static_cast<string>(p);
    cout << "  String: " << str << "\n";

    cout << "\n--- Using with string functions ---\n";
    string description = "Person: " + static_cast<string>(p);
    cout << "  Description: " << description << "\n";

    cout << "\n💡 STRING CONVERSION:\n";
    cout << "   • Useful for logging and display\n";
    cout << "   • Should be explicit\n";
    cout << "   • Provides string representation\n";
    cout << "   • Similar to toString() in other languages\n";
}

// ============================================
// EXAMPLE 7: NUMERIC CONVERSIONS
// ============================================

class Temperature {
private:
    double celsius;

public:
    explicit Temperature(double c) : celsius(c) {
        cout << "  Temperature: " << c << "°C\n";
    }

    // ✅ Explicit conversion to Fahrenheit
    explicit operator double() const {
        cout << "  Converting to Fahrenheit...\n";
        return (celsius * 9.0 / 5.0) + 32.0;
    }

    double getCelsius() const {
        return celsius;
    }

    void display() const {
        cout << "  " << celsius << "°C ("
             << static_cast<double>(*this) << "°F)\n";
    }
};

void demonstrateNumeric() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 7: NUMERIC CONVERSIONS   ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    Temperature temp(100.0);  // 100°C
    temp.display();

    cout << "\n--- Converting to Fahrenheit ---\n";
    double fahrenheit = static_cast<double>(temp);
    cout << "  Fahrenheit: " << fahrenheit << "°F\n";

    cout << "\n--- Another temperature ---\n";
    Temperature freezing(0.0);  // 0°C
    freezing.display();

    double f = static_cast<double>(freezing);
    cout << "  Freezing in Fahrenheit: " << f << "°F\n";

    cout << "\n💡 NUMERIC CONVERSIONS:\n";
    cout << "   • Convert between related units\n";
    cout << "   • Should be explicit\n";
    cout << "   • Clear semantic meaning\n";
    cout << "   • Type-safe conversions\n";
}

// ============================================
// EXAMPLE 8: CONTEXTUAL CONVERSIONS
// ============================================

class File {
private:
    string filename;
    bool isOpen;

public:
    File(const string& name) : filename(name), isOpen(false) {
        cout << "  File object created: " << name << "\n";
    }

    void open() {
        isOpen = true;
        cout << "  File opened: " << filename << "\n";
    }

    void close() {
        isOpen = false;
        cout << "  File closed: " << filename << "\n";
    }

    // ✅ Explicit bool conversion for checking if open
    explicit operator bool() const {
        return isOpen;
    }

    string getName() const {
        return filename;
    }
};

void demonstrateContextual() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 8: CONTEXTUAL CONVERSIONS║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    File file("data.txt");

    cout << "--- Checking file state ---\n";
    if (!file) {  // ✅ Contextual conversion to bool
        cout << "  File is not open\n";
    }

    cout << "\n--- Opening file ---\n";
    file.open();

    if (file) {  // ✅ Contextual conversion to bool
        cout << "  File is open, processing...\n";
    }

    cout << "\n--- Using in loops ---\n";
    while (file) {  // ✅ Contextual conversion
        cout << "  Reading from " << file.getName() << "...\n";
        file.close();  // Close to exit loop
    }

    cout << "\n--- Using with logical operators ---\n";
    File file2("backup.txt");
    file2.open();

    if (file || file2) {  // ✅ Both use contextual conversion
        cout << "  At least one file is open\n";
    }

    file2.close();

    cout << "\n💡 CONTEXTUAL CONVERSIONS:\n";
    cout << "   • Work in boolean contexts: if, while, for\n";
    cout << "   • Work with logical operators: &&, ||, !\n";
    cout << "   • explicit operator bool() enables this\n";
    cout << "   • Perfect for validity checks\n";
}

// ============================================
// EXAMPLE 9: CONVERSION AMBIGUITY
// ============================================

class Ambiguous {
private:
    int value;

public:
    Ambiguous(int v) : value(v) {}

    // Multiple conversion operators can cause ambiguity
    operator int() const { return value; }
    operator double() const { return static_cast<double>(value); }
};

void demonstrateAmbiguity() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 9: CONVERSION AMBIGUITY  ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    Ambiguous amb(42);

    cout << "--- Explicit conversions work ---\n";
    int i = static_cast<int>(amb);
    cout << "  As int: " << i << "\n";

    double d = static_cast<double>(amb);
    cout << "  As double: " << d << "\n";

    cout << "\n⚠️  POTENTIAL AMBIGUITY:\n";
    cout << "   • Multiple conversion operators can cause issues\n";
    cout << "   • Some operations might be ambiguous\n";
    cout << "   • Example: amb + 5  // Which conversion?\n";
    cout << "   • Solution: Use explicit conversions\n";
    cout << "   • Or make operators explicit\n";

    cout << "\n💡 BEST PRACTICE:\n";
    cout << "   • Make most conversions explicit\n";
    cout << "   • Avoid multiple implicit conversions\n";
    cout << "   • Be intentional about conversions\n";
}

// ============================================
// EXAMPLE 10: BEST PRACTICES
// ============================================

void demonstrateBestPractices() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 10: BEST PRACTICES       ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "✅ DO:\n";
    cout << "   • Use explicit for most conversions\n";
    cout << "   • Make conversions const\n";
    cout << "   • Have clear semantic meaning\n";
    cout << "   • operator bool should be explicit\n";
    cout << "   • Document conversion behavior\n";
    cout << "   • Test conversion scenarios\n";
    cout << "   • Consider safety over convenience\n";
    cout << "\n";
    cout << "❌ DON'T:\n";
    cout << "   • Don't make everything implicit\n";
    cout << "   • Don't have surprising conversions\n";
    cout << "   • Don't forget const qualifier\n";
    cout << "   • Don't create ambiguous conversions\n";
    cout << "   • Don't use for unrelated types\n";
    cout << "   • Don't bypass type safety\n";
    cout << "\n";
    cout << "🎯 GUIDELINES:\n";
    cout << "   • Implicit: Only for safe, obvious conversions\n";
    cout << "   • Explicit: For most user-defined conversions\n";
    cout << "   • operator bool: Always explicit (C++11+)\n";
    cout << "   • Numeric conversions: Usually explicit\n";
    cout << "   • String conversions: Usually explicit\n";
    cout << "\n";
    cout << "📋 SYNTAX PATTERNS:\n";
    cout << "   explicit operator Type() const { }  // Modern C++\n";
    cout << "   operator Type() const { }           // Older style\n";
    cout << "\n";
    cout << "💡 WHEN TO USE:\n";
    cout << "   • Smart pointers → bool (for validity)\n";
    cout << "   • Numeric wrappers → underlying type\n";
    cout << "   • Strong types → primitive types\n";
    cout << "   • Custom types → standard types\n";
}

// ============================================
// MAIN FUNCTION - RUN ALL EXAMPLES
// ============================================

int main() {
    cout << "\n";
    cout << "╔═══════════════════════════════════════════════════════════╗\n";
    cout << "║                                                           ║\n";
    cout << "║           CONVERSION OPERATORS IN C++                     ║\n";
    cout << "║              Type Casting and Conversions                 ║\n";
    cout << "║                                                           ║\n";
    cout << "╚═══════════════════════════════════════════════════════════╝\n";

    demonstrateBasic();
    demonstrateProblem();
    demonstrateExplicit();
    demonstrateSmartPointer();
    demonstrateMultiple();
    demonstrateString();
    demonstrateNumeric();
    demonstrateContextual();
    demonstrateAmbiguity();
    demonstrateBestPractices();

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
    cout << "WHAT ARE CONVERSION OPERATORS?\n";
    cout << "========================================\n";
    cout << "Definition:\n";
    cout << "  Conversion operators allow objects of a class to be\n";
    cout << "  converted to other types, either implicitly or explicitly.\n";
    cout << "\n";
    cout << "BASIC SYNTAX:\n";
    cout << "class MyClass {\n";
    cout << "public:\n";
    cout << "    operator Type() const {\n";
    cout << "        // Return value of Type\n";
    cout << "        return ...;\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "KEY FEATURES:\n";
    cout << "  • No return type specified (it's implicit)\n";
    cout << "  • Usually const (doesn't modify object)\n";
    cout << "  • Can be implicit or explicit\n";
    cout << "  • Called automatically or with cast\n";

    cout << "\n========================================\n";
    cout << "IMPLICIT VS EXPLICIT\n";
    cout << "========================================\n";
    cout << "IMPLICIT CONVERSION:\n";
    cout << "class MyClass {\n";
    cout << "public:\n";
    cout << "    operator int() const { return 42; }  // Implicit\n";
    cout << "};\n";
    cout << "\n";
    cout << "MyClass obj;\n";
    cout << "int x = obj;  // ⚠️  Automatic conversion!\n";
    cout << "\n";
    cout << "EXPLICIT CONVERSION (C++11):\n";
    cout << "class MyClass {\n";
    cout << "public:\n";
    cout << "    explicit operator int() const { return 42; }  // Explicit\n";
    cout << "};\n";
    cout << "\n";
    cout << "MyClass obj;\n";
    cout << "// int x = obj;  // ❌ Error!\n";
    cout << "int x = static_cast<int>(obj);  // ✅ Must be explicit\n";
    cout << "\n";
    cout << "WHEN TO USE EACH:\n";
    cout << "  Implicit:\n";
    cout << "    • Very rare in modern C++\n";
    cout << "    • Only for completely safe conversions\n";
    cout << "    • Example: std::string from const char*\n";
    cout << "  \n";
    cout << "  Explicit:\n";
    cout << "    • Default choice for user-defined types\n";
    cout << "    • Prevents surprises\n";
    cout << "    • Safer, more intentional\n";

    cout << "\n========================================\n";
    cout << "OPERATOR BOOL - SPECIAL CASE\n";
    cout << "========================================\n";
    cout << "Most important conversion operator!\n";
    cout << "\n";
    cout << "OLD WAY (Pre-C++11) - Dangerous:\n";
    cout << "class Pointer {\n";
    cout << "public:\n";
    cout << "    operator bool() const { return ptr != nullptr; }\n";
    cout << "};\n";
    cout << "\n";
    cout << "Pointer p;\n";
    cout << "if (p) { }      // ✅ OK\n";
    cout << "int x = p;      // ⚠️  Also works! Converts to int!\n";
    cout << "p + 5;          // ⚠️  Crazy! Converts to int!\n";
    cout << "\n";
    cout << "MODERN WAY (C++11+) - Safe:\n";
    cout << "class Pointer {\n";
    cout << "public:\n";
    cout << "    explicit operator bool() const { return ptr != nullptr; }\n";
    cout << "};\n";
    cout << "\n";
    cout << "Pointer p;\n";
    cout << "if (p) { }      // ✅ OK - contextual conversion\n";
    cout << "// int x = p;   // ❌ Error!\n";
    cout << "// p + 5;       // ❌ Error!\n";
    cout << "\n";
    cout << "CONTEXTUAL CONVERSIONS:\n";
    cout << "explicit operator bool() works in:\n";
    cout << "  • if (obj) { }\n";
    cout << "  • while (obj) { }\n";
    cout << "  • for (; obj;) { }\n";
    cout << "  • obj ? a : b\n";
    cout << "  • !obj\n";
    cout << "  • obj && other\n";
    cout << "  • obj || other\n";

    cout << "\n========================================\n";
    cout << "COMMON PATTERNS\n";
    cout << "========================================\n";
    cout << "PATTERN 1: Smart Pointer Bool\n";
    cout << "template<typename T>\n";
    cout << "class SmartPtr {\n";
    cout << "    T* ptr;\n";
    cout << "public:\n";
    cout << "    explicit operator bool() const {\n";
    cout << "        return ptr != nullptr;\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "PATTERN 2: Numeric Wrapper\n";
    cout << "class Integer {\n";
    cout << "    int value;\n";
    cout << "public:\n";
    cout << "    explicit operator int() const {\n";
    cout << "        return value;\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "PATTERN 3: String Representation\n";
    cout << "class Person {\n";
    cout << "    string name;\n";
    cout << "public:\n";
    cout << "    explicit operator string() const {\n";
    cout << "        return name;\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "PATTERN 4: Unit Conversion\n";
    cout << "class Meters {\n";
    cout << "    double value;\n";
    cout << "public:\n";
    cout << "    explicit operator double() const {  // Convert to feet\n";
    cout << "        return value * 3.28084;\n";
    cout << "    }\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "✅ DO:\n";
    cout << "  • Make most conversions explicit\n";
    cout << "  • operator bool should ALWAYS be explicit (C++11+)\n";
    cout << "  • Make conversion operators const\n";
    cout << "  • Have clear semantic meaning\n";
    cout << "  • Document what the conversion does\n";
    cout << "  • Test conversion scenarios\n";
    cout << "  • Consider safety over convenience\n";
    cout << "  • Use static_cast for explicit conversions\n";
    cout << "\n";
    cout << "❌ DON'T:\n";
    cout << "  • Don't make everything implicitly convertible\n";
    cout << "  • Don't create surprising conversions\n";
    cout << "  • Don't forget const qualifier\n";
    cout << "  • Don't create ambiguous conversions\n";
    cout << "  • Don't use for unrelated types\n";
    cout << "  • Don't have lossy conversions be implicit\n";
    cout << "  • Don't bypass type safety\n";
    cout << "  • Don't use to make bad designs 'work'\n";

    cout << "\n========================================\n";
    cout << "SYNTAX DETAILS\n";
    cout << "========================================\n";
    cout << "BASIC FORM:\n";
    cout << "operator Type() const {\n";
    cout << "    return value_of_Type;\n";
    cout << "}\n";
    cout << "\n";
    cout << "IMPORTANT POINTS:\n";
    cout << "  • No return type specified\n";
    cout << "  • Name is 'operator Type'\n";
    cout << "  • Usually const (doesn't modify)\n";
    cout << "  • Can't have parameters\n";
    cout << "  • Returns value of target Type\n";
    cout << "\n";
    cout << "EXPLICIT MODIFIER:\n";
    cout << "explicit operator Type() const {\n";
    cout << "    return value_of_Type;\n";
    cout << "}\n";
    cout << "\n";
    cout << "EXAMPLES:\n";
    cout << "operator int() const;          // Convert to int\n";
    cout << "operator double() const;       // Convert to double\n";
    cout << "explicit operator bool() const;// Convert to bool\n";
    cout << "operator string() const;       // Convert to string\n";

    cout << "\n========================================\n";
    cout << "CONVERSION AMBIGUITY\n";
    cout << "========================================\n";
    cout << "PROBLEM:\n";
    cout << "class Bad {\n";
    cout << "public:\n";
    cout << "    operator int() const;     // Converts to int\n";
    cout << "    operator double() const;  // Converts to double\n";
    cout << "};\n";
    cout << "\n";
    cout << "Bad b;\n";
    cout << "auto x = b + 5;  // ⚠️  Ambiguous! int or double?\n";
    cout << "\n";
    cout << "SOLUTION 1: Make explicit\n";
    cout << "class Good {\n";
    cout << "public:\n";
    cout << "    explicit operator int() const;\n";
    cout << "    explicit operator double() const;\n";
    cout << "};\n";
    cout << "\n";
    cout << "Good g;\n";
    cout << "auto x = static_cast<int>(g) + 5;  // ✅ Clear!\n";
    cout << "\n";
    cout << "SOLUTION 2: Provide named functions\n";
    cout << "class Better {\n";
    cout << "public:\n";
    cout << "    int toInt() const;\n";
    cout << "    double toDouble() const;\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "REAL-WORLD EXAMPLES\n";
    cout << "========================================\n";
    cout << "STANDARD LIBRARY:\n";
    cout << "\n";
    cout << "1. std::unique_ptr<T>\n";
    cout << "   explicit operator bool() const;\n";
    cout << "   • Check if pointer is not null\n";
    cout << "   • Use: if (ptr) { ... }\n";
    cout << "\n";
    cout << "2. std::shared_ptr<T>\n";
    cout << "   explicit operator bool() const;\n";
    cout << "   • Check if pointer is not null\n";
    cout << "   • Use: if (ptr) { ... }\n";
    cout << "\n";
    cout << "3. std::optional<T> (C++17)\n";
    cout << "   explicit operator bool() const;\n";
    cout << "   • Check if optional has value\n";
    cout << "   • Use: if (opt) { ... }\n";
    cout << "\n";
    cout << "4. std::string\n";
    cout << "   // No bool conversion (good design!)\n";
    cout << "   • Use .empty() instead\n";
    cout << "\n";
    cout << "5. Stream classes\n";
    cout << "   explicit operator bool() const;\n";
    cout << "   • Check stream state\n";
    cout << "   • Use: if (stream) { ... }\n";

    cout << "\n========================================\n";
    cout << "WHEN TO USE CONVERSIONS\n";
    cout << "========================================\n";
    cout << "✅ GOOD USE CASES:\n";
    cout << "  • Smart pointer → bool (validity check)\n";
    cout << "  • Wrapper type → underlying type\n";
    cout << "  • Strong type → primitive (with care)\n";
    cout << "  • Numeric units → compatible unit\n";
    cout << "  • Custom type → string (for display)\n";
    cout << "  • Stream/file → bool (state check)\n";
    cout << "\n";
    cout << "❌ BAD USE CASES:\n";
    cout << "  • Unrelated types\n";
    cout << "  • Lossy conversions (implicit)\n";
    cout << "  • Surprising conversions\n";
    cout << "  • Multiple implicit conversions\n";
    cout << "  • Making bad design work\n";

    cout << "\n========================================\n";
    cout << "CONST CORRECTNESS\n";
    cout << "========================================\n";
    cout << "Conversion operators should be const:\n";
    cout << "\n";
    cout << "✅ CORRECT:\n";
    cout << "class MyClass {\n";
    cout << "public:\n";
    cout << "    operator int() const {  // const!\n";
    cout << "        return value;\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "❌ WRONG:\n";
    cout << "class MyClass {\n";
    cout << "public:\n";
    cout << "    operator int() {  // Not const!\n";
    cout << "        return value;\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "WHY CONST:\n";
    cout << "  • Conversion doesn't modify object\n";
    cout << "  • Works with const objects\n";
    cout << "  • Allows conversion in const contexts\n";
    cout << "  • Better const correctness\n";

    cout << "\n========================================\n";
    cout << "DECISION TREE\n";
    cout << "========================================\n";
    cout << "Need conversion operator?\n";
    cout << "  NO  → Don't add one!\n";
    cout << "  YES → Continue...\n";
    cout << "\n";
    cout << "Is it operator bool()?\n";
    cout << "  YES → ALWAYS make it explicit ✅\n";
    cout << "  NO  → Continue...\n";
    cout << "\n";
    cout << "Is conversion safe and obvious?\n";
    cout << "  NO  → Make it explicit ✅\n";
    cout << "  YES → Continue...\n";
    cout << "\n";
    cout << "Is it a numeric or string conversion?\n";
    cout << "  YES → Make it explicit ✅\n";
    cout << "  NO  → Continue...\n";
    cout << "\n";
    cout << "Could it cause surprises?\n";
    cout << "  YES → Make it explicit ✅\n";
    cout << "  NO  → Still consider explicit ✅\n";
    cout << "\n";
    cout << "DEFAULT: Make it explicit!\n";

    cout << "\n========================================\n";
    cout << "QUICK REFERENCE\n";
    cout << "========================================\n";
    cout << "// Basic conversion operator\n";
    cout << "class MyClass {\n";
    cout << "public:\n";
    cout << "    operator int() const { return value; }\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Explicit conversion operator (recommended)\n";
    cout << "class MyClass {\n";
    cout << "public:\n";
    cout << "    explicit operator int() const { return value; }\n";
    cout << "};\n";
    cout << "\n";
    cout << "// operator bool (always explicit!)\n";
    cout << "class MyClass {\n";
    cout << "public:\n";
    cout << "    explicit operator bool() const { return valid; }\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Using conversions\n";
    cout << "MyClass obj;\n";
    cout << "int x = static_cast<int>(obj);  // Explicit cast\n";
    cout << "if (obj) { }                    // Contextual bool\n";

    cout << "\n========================================\n";
    cout << "KEY TAKEAWAYS\n";
    cout << "========================================\n";
    cout << "1. Conversion operators enable type conversions\n";
    cout << "2. Syntax: operator Type() const { }\n";
    cout << "3. explicit prevents implicit conversions\n";
    cout << "4. operator bool should ALWAYS be explicit\n";
    cout << "5. Contextual conversions work with explicit\n";
    cout << "6. Make conversions const (usually)\n";
    cout << "7. Prefer explicit over implicit\n";
    cout << "8. Avoid ambiguous conversions\n";
    cout << "9. Document conversion behavior\n";
    cout << "10. Safety over convenience!\n";

    cout << "\n========================================\n";
    cout << "COMPARISON TABLE\n";
    cout << "========================================\n";
    cout << "┌──────────────────┬──────────┬────────────┐\n";
    cout << "│ Feature          │ Implicit │ Explicit   │\n";
    cout << "├──────────────────┼──────────┼────────────┤\n";
    cout << "│ Automatic        │ Yes      │ No         │\n";
    cout << "│ Requires cast    │ No       │ Yes        │\n";
    cout << "│ Boolean context  │ Yes      │ Yes        │\n";
    cout << "│ Safety           │ Lower    │ Higher     │\n";
    cout << "│ Surprises        │ More     │ Fewer      │\n";
    cout << "│ Type safety      │ Weaker   │ Stronger   │\n";
    cout << "│ Recommended      │ Rare     │ Default    │\n";
    cout << "│ operator bool    │ Avoid    │ Use        │\n";
    cout << "└──────────────────┴──────────┴────────────┘\n";

    return 0;
}
