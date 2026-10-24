#include <print>
#include <string>
#include <vector>
#include <initializer_list>
#include <utility>
using namespace std;

// ============================================
// WHAT ARE INHERITING CONSTRUCTORS?
// ============================================

/*
 * INHERITING CONSTRUCTORS (C++11):
 * - Derived class inherits base class constructors
 * - Reduces boilerplate code
 * - Uses "using" declaration
 * - Automatic forwarding to base constructors
 * - Simplifies derived class creation
 *
 * WHY INHERITING CONSTRUCTORS?
 * - Eliminate repetitive forwarding constructors
 * - Reduce code duplication
 * - Cleaner, more maintainable code
 * - Automatic parameter forwarding
 * - Less error-prone
 *
 * SYNTAX:
 * class Derived : public Base {
 * public:
 *     using Base::Base;  // Inherit all constructors
 * };
 *
 * BEFORE C++11:
 * - Had to manually write forwarding constructors
 * - Lots of boilerplate code
 * - Error-prone parameter forwarding
 *
 * RULES:
 * - Access is that of the base constructor (private ones cannot be used)
 * - Default constructor not inherited
 * - Copy/move constructors not inherited
 * - Can selectively override
 * - Works with constructor delegation
 *
 * BEST PRACTICES:
 * - Use when derived class doesn't add members
 * - Use for simple wrapper classes
 * - Override specific constructors when needed
 * - Document inherited constructors
 */

// ============================================
// EXAMPLE 1: THE PROBLEM - WITHOUT INHERITING
// ============================================

class BaseBad {
protected:
    string name;
    int id;
    double value;

public:
    BaseBad(const string& n, int i, double v)
        : name(n), id(i), value(v) {
        std::println("  BaseBad(string, int, double)");
    }

    BaseBad(const string& n, int i)
        : name(n), id(i), value(0.0) {
        std::println("  BaseBad(string, int)");
    }

    BaseBad(const string& n)
        : name(n), id(0), value(0.0) {
        std::println("  BaseBad(string)");
    }

    void display() const {
        std::println("  Name: {}, ID: {}, Value: {:g}", name, id, value);
    }
};

// ❌ Without inheriting constructors - must forward everything!
class DerivedBad : public BaseBad {
private:
    string category;

public:
    // ❌ Must manually forward ALL base constructors!
    DerivedBad(const string& n, int i, double v)
        : BaseBad(n, i, v), category("General") {
        std::println("  DerivedBad(string, int, double)");
    }

    DerivedBad(const string& n, int i)
        : BaseBad(n, i), category("General") {
        std::println("  DerivedBad(string, int)");
    }

    DerivedBad(const string& n)
        : BaseBad(n), category("General") {
        std::println("  DerivedBad(string)");
    }

    void showCategory() const {
        std::println("  Category: {}", category);
    }
};

void demonstrateProblem() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 1: THE PROBLEM           ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    std::println("--- Creating DerivedBad objects (manual forwarding) ---");
    DerivedBad d1("Item1", 101, 99.9);
    d1.display();
    d1.showCategory();

    std::println("");
    DerivedBad d2("Item2", 102);
    d2.display();

    std::println("");
    DerivedBad d3("Item3");
    d3.display();
}

// ============================================
// EXAMPLE 2: THE SOLUTION - INHERITING CONSTRUCTORS
// ============================================

class BaseGood {
protected:
    string name;
    int id;
    double value;

public:
    BaseGood(const string& n, int i, double v)
        : name(n), id(i), value(v) {
        std::println("  BaseGood(string, int, double)");
    }

    BaseGood(const string& n, int i)
        : name(n), id(i), value(0.0) {
        std::println("  BaseGood(string, int)");
    }

    BaseGood(const string& n)
        : name(n), id(0), value(0.0) {
        std::println("  BaseGood(string)");
    }

    void display() const {
        std::println("  Name: {}, ID: {}, Value: {:g}", name, id, value);
    }
};

// ✅ With inheriting constructors - automatic!
class DerivedGood : public BaseGood {
private:
    string category = "General";  // Default member initializer

public:
    // ✅ Inherit all base constructors!
    using BaseGood::BaseGood;

    void showCategory() const {
        std::println("  Category: {}", category);
    }
};

void demonstrateSolution() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 2: THE SOLUTION          ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    std::println("--- Creating DerivedGood objects (inherited constructors) ---");
    DerivedGood d1("Item1", 101, 99.9);
    d1.display();
    d1.showCategory();

    std::println("");
    DerivedGood d2("Item2", 102);
    d2.display();

    std::println("");
    DerivedGood d3("Item3");
    d3.display();
}

// ============================================
// EXAMPLE 3: SIMPLE WRAPPER CLASS
// ============================================

class Container {
protected:
    vector<int> data;

public:
    Container(size_t size, int value) : data(size, value) {
        std::println("  Container({}, {})", size, value);
    }

    Container(initializer_list<int> init) : data(init) {
        std::println("  Container(initializer_list)");
    }

    void display() const {
        std::print("  Data: ");
        for (int val : data) {
            std::print("{} ", val);
        }
        std::println("");
    }

    size_t size() const { return data.size(); }
};

// Simple wrapper - inherits all constructors
class NamedContainer : public Container {
private:
    string name;

public:
    using Container::Container;  // ✅ Inherit all constructors!

    void setName(const string& n) { name = n; }

    void show() const {
        std::println("  Container name: {}", name);
        display();
    }
};

void demonstrateWrapper() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 3: WRAPPER CLASS         ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    std::println("--- Using inherited constructors ---");
    NamedContainer c1(5, 10);  // Uses Container(size_t, int)
    c1.setName("Numbers");
    c1.show();

    std::println("");
    NamedContainer c2{1, 2, 3, 4, 5};  // Uses Container(initializer_list)
    c2.setName("Sequence");
    c2.show();

    std::println("");
    std::println("--- Careful: braces prefer the initializer_list constructor ---");
    NamedContainer c3{5, 10};  // NOT 5 elements of value 10: two elements, 5 and 10
    c3.setName("Braces");
    c3.show();
}

// ============================================
// EXAMPLE 4: WITH ADDITIONAL MEMBERS
// ============================================

class Shape {
protected:
    double x, y;
    string color;

public:
    Shape(double xPos, double yPos, const string& c)
        : x(xPos), y(yPos), color(c) {
        std::println("  Shape({:g}, {:g}, {})", x, y, c);
    }

    Shape(double xPos, double yPos)
        : Shape(xPos, yPos, "black") {
    }

    void display() const {
        std::println("  Position: ({:g}, {:g})", x, y);
        std::println("  Color: {}", color);
    }
};

class Circle : public Shape {
private:
    double radius = 1.0;  // Default member initializer

public:
    using Shape::Shape;  // ✅ Inherit constructors

    // Additional constructor specific to Circle
    Circle(double xPos, double yPos, double r, const string& c)
        : Shape(xPos, yPos, c), radius(r) {
        std::println("  Circle with radius {:g}", r);
    }

    void show() const {
        display();
        std::println("  Radius: {:g}", radius);
    }
};

void demonstrateAdditionalMembers() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 4: ADDITIONAL MEMBERS    ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    std::println("--- Using inherited constructors ---");
    Circle c1(10.0, 20.0, "red");
    c1.show();

    std::println("");
    std::println("--- Using Circle-specific constructor ---");
    Circle c2(5.0, 15.0, 2.5, "blue");
    c2.show();
}

// ============================================
// EXAMPLE 5: OVERRIDING SPECIFIC CONSTRUCTOR
// ============================================

class Base {
protected:
    int value;

public:
    Base(int v) : value(v) {
        std::println("  Base({})", v);
    }

    Base(int v, bool verbose) : value(v) {
        if (verbose) {
            std::println("  Base({}, verbose)", v);
        }
    }

    void show() const {
        std::println("  Value: {}", value);
    }
};

class Derived : public Base {
private:
    string label = "Default";

public:
    using Base::Base;  // Inherit most constructors

    // ✅ Override specific constructor for custom behavior
    Derived(int v, bool verbose) : Base(v, false), label("Custom") {
        if (verbose) {
            std::println("  Derived({}, verbose) - custom implementation", v);
        }
    }

    void display() const {
        show();
        std::println("  Label: {}", label);
    }
};

void demonstrateOverriding() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 5: OVERRIDING            ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    std::println("--- Using inherited constructor ---");
    Derived d1(42);
    d1.display();

    std::println("");
    std::println("--- Using overridden constructor ---");
    Derived d2(100, true);
    d2.display();
}

// ============================================
// EXAMPLE 6: MULTIPLE INHERITANCE
// ============================================

class Base1 {
protected:
    int id;

public:
    Base1() : id(0) {
        std::println("  Base1() default");
    }

    Base1(int i) : id(i) {
        std::println("  Base1({})", i);
    }

    void showId() const {
        std::println("  ID: {}", id);
    }
};

class Base2 {
protected:
    string name;

public:
    Base2() : name("default") {
        std::println("  Base2() default");
    }

    Base2(const string& n) : name(n) {
        std::println("  Base2({})", n);
    }

    void showName() const {
        std::println("  Name: {}", name);
    }
};

class MultiDerived : public Base1, public Base2 {
public:
    using Base1::Base1;  // ✅ Inherit Base1(int)
    using Base2::Base2;  // ✅ Inherit Base2(const string&)

    // An inherited constructor initializes only ITS base; the other base is
    // default-constructed. To initialize both bases, write your own constructor:
    MultiDerived(int i, const string& n) : Base1(i), Base2(n) {
        std::println("  MultiDerived combined constructor");
    }

    void display() const {
        showId();
        showName();
    }
};

void demonstrateMultipleInheritance() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 6: MULTIPLE INHERITANCE  ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    std::println("--- Inherited Base1(int): Base2 is default-constructed ---");
    MultiDerived m1(101);
    m1.display();

    std::println("");
    std::println("--- Inherited Base2(const string&): Base1 is default-constructed ---");
    MultiDerived m2("Alice");
    m2.display();

    std::println("");
    std::println("--- Own combined constructor: both bases initialized ---");
    MultiDerived m3(102, "Bob");
    m3.display();
}

// ============================================
// EXAMPLE 7: WHAT ISN'T INHERITED
// ============================================

class BaseSpecial {
public:
    // Default constructor
    BaseSpecial() {
        std::println("  BaseSpecial default constructor");
    }

    // Regular constructor
    BaseSpecial(int x) {
        std::println("  BaseSpecial({})", x);
    }

    // Copy constructor
    BaseSpecial(const BaseSpecial&) {
        std::println("  BaseSpecial copy constructor");
    }

    // Move constructor
    BaseSpecial(BaseSpecial&&) {
        std::println("  BaseSpecial move constructor");
    }
};

class DerivedSpecial : public BaseSpecial {
public:
    using BaseSpecial::BaseSpecial;  // What gets inherited?
};

void demonstrateWhatNotInherited() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 7: WHAT ISN'T INHERITED  ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    std::println("--- Regular constructor (inherited) ---");
    DerivedSpecial d1(42);  // ✅ Works - inherited

    std::println("");
    std::println("--- Default constructor (NOT inherited) ---");
    DerivedSpecial d2;  // ✅ Compiler-generated default

    std::println("");
    std::println("--- Copy constructor (NOT inherited) ---");
    DerivedSpecial d3(d1);  // ✅ Compiler-generated copy

    std::println("");
    std::println("--- Move constructor (NOT inherited) ---");
    DerivedSpecial d4(move(d2));  // ✅ Compiler-generated move
}

// ============================================
// EXAMPLE 8: PRIVATE BASE CONSTRUCTORS
// ============================================

class BasePrivate {
private:
    int secret;

    // Private constructor
    BasePrivate(int s) : secret(s) {
        std::println("  BasePrivate private constructor");
    }

protected:
    // Protected constructor
    BasePrivate(int s, bool) : secret(s) {
        std::println("  BasePrivate protected constructor");
    }

public:
    // Public constructor
    BasePrivate(const string& tag) : secret(static_cast<int>(tag.size())) {
        std::println("  BasePrivate public constructor: \"{}\"", tag);
    }

    void show() const {
        std::println("  Secret: {}", secret);
    }
};

class DerivedPrivate : public BasePrivate {
public:
    using BasePrivate::BasePrivate;  // The access is that of the BASE constructor
};

// A further derived class can use the inherited protected constructor
class Leaf : public DerivedPrivate {
public:
    Leaf() : DerivedPrivate(7, true) {}
};

void demonstratePrivateConstructors() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 8: PRIVATE CONSTRUCTORS  ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    std::println("--- Public constructor (inherited) ---");
    DerivedPrivate d1("public");
    d1.show();

    std::println("");
    std::println("--- Protected constructor (usable from a further derived class) ---");
    Leaf leaf;
    leaf.show();

    // DerivedPrivate d2(42);        // ❌ Error - BasePrivate(int) is private
    std::println("");
    std::println("  // DerivedPrivate d2(42);        // ❌ Error: private");
    // DerivedPrivate d3(42, true);  // ❌ Error - protected, not accessible from outside
    std::println("  // DerivedPrivate d3(42, true);  // ❌ Error: protected");
}

// ============================================
// EXAMPLE 9: REAL-WORLD - EXCEPTION CLASSES
// ============================================

class BaseException {
protected:
    string message;
    int errorCode;

public:
    BaseException(const string& msg, int code)
        : message(msg), errorCode(code) {
    }

    BaseException(const string& msg)
        : message(msg), errorCode(0) {
    }

    virtual void display() const {
        std::println("  Error [{}]: {}", errorCode, message);
    }

    virtual ~BaseException() = default;
};

class NetworkException : public BaseException {
public:
    using BaseException::BaseException;  // ✅ Inherit all constructors

    void display() const override {
        std::println("  Network Error [{}]: {}", errorCode, message);
    }
};

class DatabaseException : public BaseException {
public:
    using BaseException::BaseException;  // ✅ Inherit all constructors

    void display() const override {
        std::println("  Database Error [{}]: {}", errorCode, message);
    }
};

class FileException : public BaseException {
public:
    using BaseException::BaseException;  // ✅ Inherit all constructors

    void display() const override {
        std::println("  File Error [{}]: {}", errorCode, message);
    }
};

void demonstrateExceptions() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 9: EXCEPTION CLASSES     ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    std::println("--- Creating various exceptions ---");
    NetworkException ne1("Connection timeout", 404);
    ne1.display();

    std::println("");
    DatabaseException de1("Query failed");
    de1.display();

    std::println("");
    FileException fe1("File not found", 2);
    fe1.display();

    std::println("");
    std::println("--- Thrown as the derived type, caught by base reference ---");
    try {
        throw NetworkException("Connection lost", 503);
    } catch (const BaseException& e) {
        e.display();  // virtual call: NetworkException::display()
    }
}

// ============================================
// EXAMPLE 10: WHEN NOT TO USE
// ============================================
// 1. Inheriting constructors forward the arguments unchanged - no validation possible
class Account {
protected:
    int balance;

public:
    Account(int b) : balance(b) {}
    int getBalance() const { return balance; }
};

class UncheckedAccount : public Account {
public:
    using Account::Account;  // ⚠️ Account(-50) is accepted as it is
};

class CheckedAccount : public Account {
public:
    // Own constructor: adds the validation
    CheckedAccount(int b) : Account(b < 0 ? 0 : b) {}
};

// 2. Initialization order: the base class first, then the derived members
class Noisy {
public:
    Noisy(const char* what) { std::println("  Noisy member: {}", what); }
};

class OrderBase {
public:
    OrderBase(int) { std::println("  OrderBase(int)"); }
};

class OrderDerived : public OrderBase {
    Noisy member = Noisy("default member initializer");

public:
    using OrderBase::OrderBase;
};

void demonstrateWhenNotToUse() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 10: WHEN NOT TO USE      ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    std::println("--- No validation through inheriting constructors ---");
    UncheckedAccount unchecked(-50);
    std::println("  UncheckedAccount(-50): balance = {}", unchecked.getBalance());
    CheckedAccount checked(-50);
    std::println("  CheckedAccount(-50):   balance = {}", checked.getBalance());

    std::println("");
    std::println("--- Order: base constructor first, then the derived members ---");
    OrderDerived od(1);
}

// ============================================
// MAIN FUNCTION - RUN ALL EXAMPLES
// ============================================

int main() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════════════╗");
    std::println("║                                                           ║");
    std::println("║          INHERITING CONSTRUCTORS IN C++                   ║");
    std::println("║            Eliminating Forwarding Boilerplate             ║");
    std::println("║                                                           ║");
    std::println("╚═══════════════════════════════════════════════════════════╝");

    demonstrateProblem();
    demonstrateSolution();
    demonstrateWrapper();
    demonstrateAdditionalMembers();
    demonstrateOverriding();
    demonstrateMultipleInheritance();
    demonstrateWhatNotInherited();
    demonstratePrivateConstructors();
    demonstrateExceptions();
    demonstrateWhenNotToUse();

    return 0;
}
