#include <print>
#include <string>
#include <cmath>
#include <memory>
#include <optional>
#include <fstream>
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
        std::println("  Fraction created: {}/{}", n, d);
    }

    // ✅ Conversion operator to double
    operator double() const {
        std::println("  Converting to double...");
        return static_cast<double>(numerator) / denominator;
    }

    void display() const {
        std::println("  Fraction: {}/{}", numerator, denominator);
    }
};

void demonstrateBasic() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 1: BASIC CONVERSION      ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    std::println("--- Creating fraction ---");
    Fraction f(3, 4);
    f.display();

    std::println("");
    std::println("--- Implicit conversion to double ---");
    double d = f;  // ⚠️ Implicit conversion!
    std::println("  Double value: {}", d);

    std::println("");
    std::println("--- Using in arithmetic ---");
    double result = f + 0.5;  // ⚠️ Converts f to double
    std::println("  Result: {}", result);
}

// ============================================
// EXAMPLE 2: THE PROBLEM WITH IMPLICIT CONVERSIONS
// ============================================

class IntegerBad {
private:
    int value;

public:
    explicit IntegerBad(int v) : value(v) {
        std::println("  IntegerBad({})", v);
    }

    // ❌ Non-explicit int conversion - dangerous!
    operator int() const {
        std::println("  Converting to int...");
        return value;
    }
};

class FlagBad {
public:
    // ❌ Non-explicit bool conversion - dangerous!
    operator bool() const {
        return true;
    }
};

void demonstrateProblem() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 2: THE PROBLEM           ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    IntegerBad num(42);

    std::println("--- Implicit conversions happen unexpectedly ---");
    int x = num;  // ⚠️ compiles - no cast needed
    std::println("  x = {}", x);

    std::println("");
    std::println("--- Unwanted arithmetic ---");
    int result = num + 10;  // ⚠️ an IntegerBad is used like an int
    std::println("  result = {}", result);

    std::println("");
    std::println("--- Can even multiply! ---");
    int product = num * 2;  // ⚠️
    std::println("  product = {}", product);

    std::println("");
    std::println("--- A non-explicit operator bool is even worse ---");
    FlagBad flag;
    FlagBad other;
    int asInt = flag;                 // ⚠️ bool becomes int
    int sum = flag + 5;               // ⚠️ arithmetic on a "flag"
    bool same = (flag == other);      // ⚠️ compares two bools, not two FlagBad objects
    std::println("  asInt = {}, flag + 5 = {}, flag == other: {}", asInt, sum, same);
}

// ============================================
// EXAMPLE 3: EXPLICIT CONVERSION OPERATORS
// ============================================

class IntegerGood {
private:
    int value;

public:
    explicit IntegerGood(int v) : value(v) {
        std::println("  IntegerGood({})", v);
    }

    // ✅ Explicit bool conversion - safe!
    explicit operator bool() const {
        std::println("  Explicit bool conversion");
        return value != 0;
    }

    // ✅ Explicit int conversion
    explicit operator int() const {
        std::println("  Explicit int conversion");
        return value;
    }

    void display() const {
        std::println("  Value: {}", value);
    }
};

void demonstrateExplicit() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 3: EXPLICIT CONVERSION   ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    IntegerGood num(42);

    std::println("--- Implicit conversions prevented ---");
    // int x = num;  // ❌ Error! Must be explicit
    std::println("  // int x = num;  // ❌ Compilation error!");

    std::println("");
    std::println("--- Explicit conversion required ---");
    int x = static_cast<int>(num);  // ✅ Must use cast
    std::println("  x = {}", x);

    std::println("");
    std::println("--- Bool in conditions (contextual) ---");
    if (num) {  // ✅ Works! Contextual conversion
        std::println("  num is non-zero (contextual bool)");
    }

    std::println("");
    std::println("--- Explicit cast needed for arithmetic ---");
    // int result = num + 10;  // ❌ Error!
    int result = static_cast<int>(num) + 10;  // ✅ Explicit
    std::println("  result = {}", result);
}

// ============================================
// EXAMPLE 4: OPERATOR BOOL FOR SMART POINTERS
// ============================================

template<typename T>
class SmartPointer {
private:
    T* ptr;

public:
    explicit SmartPointer(T* p = nullptr) : ptr(p) {
        if (ptr) {
            std::println("  SmartPointer acquired resource");
        } else {
            std::println("  SmartPointer is null");
        }
    }

    ~SmartPointer() {
        delete ptr;
        std::println("  SmartPointer released resource");
    }

    // Owns a raw pointer: copying would delete it twice
    SmartPointer(const SmartPointer&) = delete;
    SmartPointer& operator=(const SmartPointer&) = delete;

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
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 4: SMART POINTER BOOL    ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    std::println("--- Creating valid smart pointer ---");
    SmartPointer<int> sp1(new int(42));

    if (sp1) {  // ✅ Contextual bool conversion
        std::println("  sp1 is valid, value: {}", *sp1);
    }

    std::println("");
    std::println("--- Creating null smart pointer ---");
    SmartPointer<int> sp2(nullptr);

    if (!sp2) {  // ✅ Contextual bool conversion
        std::println("  sp2 is null");
    }

    std::println("");
    std::println("--- Using in logical operations ---");
    if (sp1 && !sp2) {  // ✅ Both work in boolean context
        std::println("  sp1 valid and sp2 null");
    }
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
        std::println("  Complex({}, {})", r, i);
    }

    // ✅ Explicit conversion to double (magnitude)
    explicit operator double() const {
        std::println("  Converting to double (magnitude)...");
        return sqrt(real * real + imag * imag);
    }

    // ✅ Explicit conversion to bool
    explicit operator bool() const {
        std::println("  Converting to bool...");
        return (real != 0) || (imag != 0);
    }

    void display() const {
        std::print("  Complex: {}", real);
        if (imag >= 0) std::println(" + {}i", imag);
        else std::println(" - {}i", -imag);
    }
};

void demonstrateMultiple() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 5: MULTIPLE CONVERSIONS  ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    Complex c(3.0, 4.0);
    c.display();

    std::println("");
    std::println("--- Converting to double (magnitude) ---");
    double magnitude = static_cast<double>(c);
    std::println("  Magnitude: {}", magnitude);

    std::println("");
    std::println("--- Using in boolean context ---");
    if (c) {  // ✅ Contextual bool conversion
        std::println("  Complex number is non-zero");
    }

    std::println("");
    std::println("--- Zero complex number ---");
    Complex zero(0.0, 0.0);
    zero.display();

    if (!zero) {  // ✅ Contextual bool conversion
        std::println("  Complex number is zero");
    }
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
        std::println("  Person created: {} {}", firstName, lastName);
    }

    // ✅ Explicit string conversion
    explicit operator string() const {
        std::println("  Converting to string...");
        return firstName + " " + lastName + " (age " + to_string(age) + ")";
    }

    void display() const {
        std::println("  Name: {} {}", firstName, lastName);
        std::println("  Age: {}", age);
    }
};

void demonstrateString() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 6: STRING CONVERSION     ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    Person p("Alice", "Johnson", 30);
    p.display();

    std::println("");
    std::println("--- Converting to string ---");
    string str = static_cast<string>(p);
    std::println("  String: {}", str);

    std::println("");
    std::println("--- Using with string functions ---");
    string description = "Person: " + static_cast<string>(p);
    std::println("  Description: {}", description);
}

// ============================================
// EXAMPLE 7: NUMERIC CONVERSIONS
// ============================================

class Temperature {
private:
    double celsius;

public:
    explicit Temperature(double c) : celsius(c) {
        std::println("  Temperature: {}°C", c);
    }

    // ✅ Explicit conversion to the underlying type (the stored Celsius value)
    explicit operator double() const {
        std::println("  Converting to double (Celsius)...");
        return celsius;
    }

    // A UNIT conversion deserves a name: an operator double() that silently
    // returned Fahrenheit would be a surprising conversion.
    double toFahrenheit() const {
        return (celsius * 9.0 / 5.0) + 32.0;
    }

    void display() const {
        std::println("  {}°C ({}°F)", celsius, toFahrenheit());
    }
};

void demonstrateNumeric() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 7: NUMERIC CONVERSIONS   ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    Temperature temp(100.0);  // 100°C
    temp.display();

    std::println("");
    std::println("--- Conversion to the underlying type ---");
    double c = static_cast<double>(temp);
    std::println("  Celsius: {}°C", c);

    std::println("");
    std::println("--- Unit conversion by a named function ---");
    Temperature freezing(0.0);  // 0°C
    freezing.display();

    std::println("  Freezing in Fahrenheit: {}°F", freezing.toFahrenheit());
}

// ============================================
// EXAMPLE 8: CONTEXTUAL CONVERSIONS
// ============================================

class File {
private:
    string filename;
    bool isOpen;

public:
    explicit File(const string& name) : filename(name), isOpen(false) {
        std::println("  File object created: {}", name);
    }

    void open() {
        isOpen = true;
        std::println("  File opened: {}", filename);
    }

    void close() {
        isOpen = false;
        std::println("  File closed: {}", filename);
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
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 8: CONTEXTUAL CONVERSIONS║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    File file("data.txt");

    std::println("--- Checking file state ---");
    if (!file) {  // ✅ Contextual conversion to bool
        std::println("  File is not open");
    }

    std::println("");
    std::println("--- Opening file ---");
    file.open();

    if (file) {  // ✅ Contextual conversion to bool
        std::println("  File is open, processing...");
    }

    std::println("");
    std::println("--- Using in loops ---");
    while (file) {  // ✅ Contextual conversion
        std::println("  Reading from {}...", file.getName());
        file.close();  // Close to exit loop
    }

    std::println("");
    std::println("--- Using with logical operators ---");
    File file2("backup.txt");
    file2.open();

    if (file || file2) {  // ✅ Both use contextual conversion
        std::println("  At least one file is open");
    }

    file2.close();
}

// ============================================
// EXAMPLE 9: CONVERSION AMBIGUITY
// ============================================

class Ambiguous {
private:
    int value;

public:
    explicit Ambiguous(int v) : value(v) {}

    // Multiple conversion operators can cause ambiguity
    operator int() const { return value; }
    operator double() const { return static_cast<double>(value); }
};

void demonstrateAmbiguity() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 9: CONVERSION AMBIGUITY  ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    Ambiguous amb(42);

    std::println("--- Explicit conversions work ---");
    int i = static_cast<int>(amb);
    std::println("  As int: {}", i);

    double d = static_cast<double>(amb);
    std::println("  As double: {}", d);

    std::println("");
    std::println("--- Which conversion does the compiler pick? ---");
    double viaDouble = amb;  // exact match: operator double() is chosen
    int viaInt = amb;        // exact match: operator int() is chosen
    std::println("  double d = amb -> {}, int i = amb -> {}", viaDouble, viaInt);

    // long l = amb;      // ❌ Error: conversion is ambiguous (int or double?)
    // auto x = amb + 5;  // ❌ Error: ambiguous overload for 'operator+'
    std::println("  // long l = amb;      // ❌ Error: ambiguous (int or double?)");
    std::println("  // auto x = amb + 5;  // ❌ Error: ambiguous operator+");
}

// ============================================
// EXAMPLE 10: BEST PRACTICES
// ============================================

// A conversion operator without const cannot be used on a const object
class NotConstBad {
public:
    explicit operator int() { return 1; }  // ❌ not const
};

class ConstGood {
public:
    explicit operator int() const { return 2; }  // ✅ const
};

void demonstrateBestPractices() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 10: BEST PRACTICES       ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    std::println("--- The standard library uses explicit operator bool ---");
    unique_ptr<int> up = make_unique<int>(7);
    shared_ptr<int> sp;   // empty
    optional<int> opt;    // empty
    ifstream missing("no_such_file.txt");

    std::println("  unique_ptr valid?   {}", (up ? "yes" : "no"));
    std::println("  shared_ptr valid?   {}", (sp ? "yes" : "no"));
    std::println("  optional has value? {}", (opt ? "yes" : "no"));
    std::println("  ifstream opened?    {}", (missing ? "yes" : "no"));

    // bool b = up;                // ❌ Error: explicit
    bool b = static_cast<bool>(up);  // ✅
    std::println("  static_cast<bool>(unique_ptr) = {}", b);
    // weak_ptr has NO operator bool at all: use expired() or lock()

    std::println("");
    std::println("--- Conversion operators should be const ---");
    const ConstGood good;
    std::println("  const ConstGood -> {}", static_cast<int>(good));
    // const NotConstBad bad;
    // int i = static_cast<int>(bad);  // ❌ Error: 'this' argument discards qualifiers
    std::println("  // static_cast<int>(const NotConstBad) // ❌ Error: not const");
}

// ============================================
// MAIN FUNCTION - RUN ALL EXAMPLES
// ============================================

int main() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════════════╗");
    std::println("║                                                           ║");
    std::println("║           CONVERSION OPERATORS IN C++                     ║");
    std::println("║              Type Casting and Conversions                 ║");
    std::println("║                                                           ║");
    std::println("╚═══════════════════════════════════════════════════════════╝");

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

    return 0;
}
