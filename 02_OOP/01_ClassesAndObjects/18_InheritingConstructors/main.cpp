#include <iostream>
#include <string>
#include <vector>
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
 * - Only public/protected constructors inherited
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
        cout << "  BaseBad(string, int, double)\n";
    }

    BaseBad(const string& n, int i)
        : name(n), id(i), value(0.0) {
        cout << "  BaseBad(string, int)\n";
    }

    BaseBad(const string& n)
        : name(n), id(0), value(0.0) {
        cout << "  BaseBad(string)\n";
    }

    void display() const {
        cout << "  Name: " << name << ", ID: " << id
             << ", Value: " << value << "\n";
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
        cout << "  DerivedBad(string, int, double)\n";
    }

    DerivedBad(const string& n, int i)
        : BaseBad(n, i), category("General") {
        cout << "  DerivedBad(string, int)\n";
    }

    DerivedBad(const string& n)
        : BaseBad(n), category("General") {
        cout << "  DerivedBad(string)\n";
    }

    void showCategory() const {
        cout << "  Category: " << category << "\n";
    }
};

void demonstrateProblem() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: THE PROBLEM           ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "--- Creating DerivedBad objects (manual forwarding) ---\n";
    DerivedBad d1("Item1", 101, 99.9);
    d1.display();
    d1.showCategory();

    cout << "\n";
    DerivedBad d2("Item2", 102);
    d2.display();

    cout << "\n";
    DerivedBad d3("Item3");
    d3.display();

    cout << "\n⚠️  THE PROBLEM:\n";
    cout << "   • Must manually write forwarding constructors\n";
    cout << "   • Repetitive boilerplate code\n";
    cout << "   • Easy to forget a constructor\n";
    cout << "   • Hard to maintain - change base, update all\n";
    cout << "   • Error-prone parameter forwarding\n";
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
        cout << "  BaseGood(string, int, double)\n";
    }

    BaseGood(const string& n, int i)
        : name(n), id(i), value(0.0) {
        cout << "  BaseGood(string, int)\n";
    }

    BaseGood(const string& n)
        : name(n), id(0), value(0.0) {
        cout << "  BaseGood(string)\n";
    }

    void display() const {
        cout << "  Name: " << name << ", ID: " << id
             << ", Value: " << value << "\n";
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
        cout << "  Category: " << category << "\n";
    }
};

void demonstrateSolution() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: THE SOLUTION          ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "--- Creating DerivedGood objects (inherited constructors) ---\n";
    DerivedGood d1("Item1", 101, 99.9);
    d1.display();
    d1.showCategory();

    cout << "\n";
    DerivedGood d2("Item2", 102);
    d2.display();

    cout << "\n";
    DerivedGood d3("Item3");
    d3.display();

    cout << "\n✅ THE SOLUTION:\n";
    cout << "   • No manual forwarding needed!\n";
    cout << "   • One line: using BaseGood::BaseGood;\n";
    cout << "   • All base constructors automatically available\n";
    cout << "   • Easy to maintain\n";
    cout << "   • Less code, fewer errors\n";
}

// ============================================
// EXAMPLE 3: SIMPLE WRAPPER CLASS
// ============================================

class Container {
protected:
    vector<int> data;

public:
    Container(size_t size, int value) : data(size, value) {
        cout << "  Container(" << size << ", " << value << ")\n";
    }

    Container(initializer_list<int> init) : data(init) {
        cout << "  Container(initializer_list)\n";
    }

    void display() const {
        cout << "  Data: ";
        for (int val : data) {
            cout << val << " ";
        }
        cout << "\n";
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
        cout << "  Container name: " << name << "\n";
        display();
    }
};

void demonstrateWrapper() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: WRAPPER CLASS         ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "--- Using inherited constructors ---\n";
    NamedContainer c1(5, 10);  // Uses Container(size_t, int)
    c1.setName("Numbers");
    c1.show();

    cout << "\n";
    NamedContainer c2{1, 2, 3, 4, 5};  // Uses Container(initializer_list)
    c2.setName("Sequence");
    c2.show();

    cout << "\n💡 USE CASE:\n";
    cout << "   • Perfect for simple wrappers\n";
    cout << "   • Minimal code needed\n";
    cout << "   • All base functionality available\n";
    cout << "   • Easy to extend\n";
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
        cout << "  Shape(" << x << ", " << y << ", " << c << ")\n";
    }

    Shape(double xPos, double yPos)
        : Shape(xPos, yPos, "black") {
    }

    void display() const {
        cout << "  Position: (" << x << ", " << y << ")\n";
        cout << "  Color: " << color << "\n";
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
        cout << "  Circle with radius " << r << "\n";
    }

    void show() const {
        display();
        cout << "  Radius: " << radius << "\n";
    }
};

void demonstrateAdditionalMembers() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: ADDITIONAL MEMBERS    ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "--- Using inherited constructors ---\n";
    Circle c1(10.0, 20.0, "red");
    c1.show();

    cout << "\n--- Using Circle-specific constructor ---\n";
    Circle c2(5.0, 15.0, 2.5, "blue");
    c2.show();

    cout << "\n💡 KEY POINT:\n";
    cout << "   • Inherited constructors use default member initializers\n";
    cout << "   • radius = 1.0 by default for inherited constructors\n";
    cout << "   • Can add specific constructors for derived class\n";
    cout << "   • Best of both worlds!\n";
}

// ============================================
// EXAMPLE 5: OVERRIDING SPECIFIC CONSTRUCTOR
// ============================================

class Base {
protected:
    int value;

public:
    Base(int v) : value(v) {
        cout << "  Base(" << v << ")\n";
    }

    Base(int v, bool verbose) : value(v) {
        if (verbose) {
            cout << "  Base(" << v << ", verbose)\n";
        }
    }

    void show() const {
        cout << "  Value: " << value << "\n";
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
            cout << "  Derived(" << v << ", verbose) - custom implementation\n";
        }
    }

    void display() const {
        show();
        cout << "  Label: " << label << "\n";
    }
};

void demonstrateOverriding() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: OVERRIDING            ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "--- Using inherited constructor ---\n";
    Derived d1(42);
    d1.display();

    cout << "\n--- Using overridden constructor ---\n";
    Derived d2(100, true);
    d2.display();

    cout << "\n💡 FLEXIBILITY:\n";
    cout << "   • Can inherit most constructors\n";
    cout << "   • Can override specific ones for custom behavior\n";
    cout << "   • Best of both worlds\n";
    cout << "   • Maximum flexibility\n";
}

// ============================================
// EXAMPLE 6: MULTIPLE INHERITANCE
// ============================================

class Base1 {
protected:
    int id;

public:
    Base1(int i) : id(i) {
        cout << "  Base1(" << i << ")\n";
    }

    void showId() const {
        cout << "  ID: " << id << "\n";
    }
};

class Base2 {
protected:
    string name;

public:
    Base2(const string& n) : name(n) {
        cout << "  Base2(" << n << ")\n";
    }

    void showName() const {
        cout << "  Name: " << name << "\n";
    }
};

class MultiDerived : public Base1, public Base2 {
public:
    using Base1::Base1;  // ✅ Inherit Base1 constructors
    using Base2::Base2;  // ✅ Inherit Base2 constructors

    // Default constructor (must explicitly provide values)
    MultiDerived() : Base1(0), Base2("default") {
        cout << "MultiDerived default constructor\n";
    }

    // Constructor that only initializes Base1
    MultiDerived(int i) : Base1(i), Base2("default") {
        cout << "MultiDerived Base1 constructor\n";
    }

    // Constructor that only initializes Base2
    MultiDerived(const string& n) : Base1(0), Base2(n) {
        cout << "MultiDerived Base2 constructor\n";
    }

    // Can also add combined constructor
    MultiDerived(int i, const string& n) : Base1(i), Base2(n) {
        cout << "  MultiDerived combined constructor\n";
    }

    void display() const {
        showId();
        showName();
    }
};

void demonstrateMultipleInheritance() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: MULTIPLE INHERITANCE  ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "--- Using Base1 constructor ---\n";
    MultiDerived m1(101);
    m1.display();

    cout << "\n--- Using Base2 constructor ---\n";
    MultiDerived m2("Alice");
    m2.display();

    cout << "\n--- Using combined constructor ---\n";
    MultiDerived m3(102, "Bob");
    m3.display();

    cout << "\n💡 MULTIPLE INHERITANCE:\n";
    cout << "   • Can inherit from multiple bases\n";
    cout << "   • Each using declaration imports one base\n";
    cout << "   • Can combine with custom constructors\n";
}

// ============================================
// EXAMPLE 7: WHAT ISN'T INHERITED
// ============================================

class BaseSpecial {
public:
    // Default constructor
    BaseSpecial() {
        cout << "  BaseSpecial default constructor\n";
    }

    // Regular constructor
    BaseSpecial(int x) {
        cout << "  BaseSpecial(" << x << ")\n";
    }

    // Copy constructor
    BaseSpecial(const BaseSpecial& other) {
        cout << "  BaseSpecial copy constructor\n";
    }

    // Move constructor
    BaseSpecial(BaseSpecial&& other) {
        cout << "  BaseSpecial move constructor\n";
    }
};

class DerivedSpecial : public BaseSpecial {
public:
    using BaseSpecial::BaseSpecial;  // What gets inherited?
};

void demonstrateWhatNotInherited() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 7: WHAT ISN'T INHERITED  ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "--- Regular constructor (inherited) ---\n";
    DerivedSpecial d1(42);  // ✅ Works - inherited

    cout << "\n--- Default constructor (NOT inherited) ---\n";
    DerivedSpecial d2;  // ✅ Compiler-generated default

    cout << "\n--- Copy constructor (NOT inherited) ---\n";
    DerivedSpecial d3(d1);  // ✅ Compiler-generated copy

    cout << "\n--- Move constructor (NOT inherited) ---\n";
    DerivedSpecial d4(move(d2));  // ✅ Compiler-generated move

    cout << "\n📋 NOT INHERITED:\n";
    cout << "   ❌ Default constructor\n";
    cout << "   ❌ Copy constructor\n";
    cout << "   ❌ Move constructor\n";
    cout << "   ❌ Copy assignment operator\n";
    cout << "   ❌ Move assignment operator\n";
    cout << "\n";
    cout << "✅ INHERITED:\n";
    cout << "   ✅ All other constructors\n";
    cout << "   ✅ Public and protected constructors\n";
    cout << "   ✅ Constructors with any number of parameters\n";
}

// ============================================
// EXAMPLE 8: PRIVATE BASE CONSTRUCTORS
// ============================================

class BasePrivate {
private:
    int secret;

    // Private constructor
    BasePrivate(int s) : secret(s) {
        cout << "  BasePrivate private constructor\n";
    }

protected:
    // Protected constructor
    BasePrivate(int s, bool) : secret(s) {
        cout << "  BasePrivate protected constructor\n";
    }

public:
    // Public constructor
    BasePrivate() : secret(0) {
        cout << "  BasePrivate public constructor\n";
    }

    void show() const {
        cout << "  Secret: " << secret << "\n";
    }
};

class DerivedPrivate : public BasePrivate {
public:
    using BasePrivate::BasePrivate;  // Only inherits public/protected
};

void demonstratePrivateConstructors() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 8: PRIVATE CONSTRUCTORS  ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "--- Public constructor (inherited) ---\n";
    DerivedPrivate d1;  // ✅ Works - public constructor
    d1.show();

    // DerivedPrivate d2(42);  // ❌ Error - private not inherited
    // DerivedPrivate d3(42, true);  // ❌ Error - protected not inherited

    cout << "\n📋 INHERITANCE RULES:\n";
    cout << "   ✅ Public constructors inherited\n";
    cout << "   ❌ Protected constructors NOT inherited (for external users)\n";
    cout << "   ❌ Private constructors NOT inherited\n";
    cout << "   • Only public base constructors become public in derived\n";
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
        cout << "  Error [" << errorCode << "]: " << message << "\n";
    }

    virtual ~BaseException() = default;
};

class NetworkException : public BaseException {
public:
    using BaseException::BaseException;  // ✅ Inherit all constructors

    void display() const override {
        cout << "  Network Error [" << errorCode << "]: " << message << "\n";
    }
};

class DatabaseException : public BaseException {
public:
    using BaseException::BaseException;  // ✅ Inherit all constructors

    void display() const override {
        cout << "  Database Error [" << errorCode << "]: " << message << "\n";
    }
};

class FileException : public BaseException {
public:
    using BaseException::BaseException;  // ✅ Inherit all constructors

    void display() const override {
        cout << "  File Error [" << errorCode << "]: " << message << "\n";
    }
};

void demonstrateExceptions() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 9: EXCEPTION CLASSES     ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "--- Creating various exceptions ---\n";
    NetworkException ne1("Connection timeout", 404);
    ne1.display();

    cout << "\n";
    DatabaseException de1("Query failed");
    de1.display();

    cout << "\n";
    FileException fe1("File not found", 2);
    fe1.display();

    cout << "\n💡 REAL-WORLD BENEFIT:\n";
    cout << "   • Exception hierarchies are perfect use case\n";
    cout << "   • Minimal code for each exception type\n";
    cout << "   • All base constructors available\n";
    cout << "   • Easy to extend with new exception types\n";
    cout << "   • Consistent interface across exceptions\n";
}

// ============================================
// EXAMPLE 10: WHEN NOT TO USE
// ============================================

void demonstrateWhenNotToUse() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 10: WHEN NOT TO USE      ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "❌ DON'T USE WHEN:\n";
    cout << "\n";
    cout << "1. Derived class has additional members needing initialization:\n";
    cout << "   class Bad : public Base {\n";
    cout << "       string name;  // ⚠️  Won't be initialized!\n";
    cout << "       int id;       // ⚠️  Won't be initialized!\n";
    cout << "   public:\n";
    cout << "       using Base::Base;  // ⚠️  Members uninitialized\n";
    cout << "   };\n";
    cout << "   • Use default member initializers instead\n";
    cout << "\n";
    cout << "2. Need custom initialization logic:\n";
    cout << "   class Bad : public Base {\n";
    cout << "   public:\n";
    cout << "       using Base::Base;  // Can't add validation!\n";
    cout << "   };\n";
    cout << "   • Write explicit constructors instead\n";
    cout << "\n";
    cout << "3. Need to validate base parameters:\n";
    cout << "   • Can't intercept parameter validation\n";
    cout << "   • Write explicit forwarding constructors\n";
    cout << "\n";
    cout << "✅ DO USE WHEN:\n";
    cout << "   • Derived class adds no new data members\n";
    cout << "   • Or uses default member initializers\n";
    cout << "   • Simple wrapper classes\n";
    cout << "   • Exception hierarchies\n";
    cout << "   • No custom initialization needed\n";
}

// ============================================
// MAIN FUNCTION - RUN ALL EXAMPLES
// ============================================

int main() {
    cout << "\n";
    cout << "╔═══════════════════════════════════════════════════════════╗\n";
    cout << "║                                                           ║\n";
    cout << "║          INHERITING CONSTRUCTORS IN C++                   ║\n";
    cout << "║            Eliminating Forwarding Boilerplate             ║\n";
    cout << "║                                                           ║\n";
    cout << "╚═══════════════════════════════════════════════════════════╝\n";

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
    cout << "WHAT ARE INHERITING CONSTRUCTORS?\n";
    cout << "========================================\n";
    cout << "Definition:\n";
    cout << "  Inheriting constructors allow a derived class to\n";
    cout << "  automatically use the constructors of its base class.\n";
    cout << "\n";
    cout << "SYNTAX:\n";
    cout << "class Base {\n";
    cout << "public:\n";
    cout << "    Base(int x) { }\n";
    cout << "    Base(int x, int y) { }\n";
    cout << "};\n";
    cout << "\n";
    cout << "class Derived : public Base {\n";
    cout << "public:\n";
    cout << "    using Base::Base;  // ✅ Inherit all constructors!\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Now Derived has:\n";
    cout << "Derived d1(10);      // Uses Base(int)\n";
    cout << "Derived d2(10, 20);  // Uses Base(int, int)\n";
    cout << "\n";
    cout << "KEY FEATURES:\n";
    cout << "  • One line inherits all base constructors\n";
    cout << "  • Eliminates forwarding boilerplate\n";
    cout << "  • Available since C++11\n";
    cout << "  • Works with default member initializers\n";

    cout << "\n========================================\n";
    cout << "WHY USE INHERITING CONSTRUCTORS?\n";
    cout << "========================================\n";
    cout << "BEFORE C++11 (The Problem):\n";
    cout << "class Base {\n";
    cout << "public:\n";
    cout << "    Base(int x) { }\n";
    cout << "    Base(int x, int y) { }\n";
    cout << "    Base(int x, int y, int z) { }\n";
    cout << "};\n";
    cout << "\n";
    cout << "class Derived : public Base {\n";
    cout << "public:\n";
    cout << "    // ❌ Must manually forward ALL constructors!\n";
    cout << "    Derived(int x) : Base(x) { }\n";
    cout << "    Derived(int x, int y) : Base(x, y) { }\n";
    cout << "    Derived(int x, int y, int z) : Base(x, y, z) { }\n";
    cout << "};\n";
    cout << "\n";
    cout << "PROBLEMS:\n";
    cout << "  • Lots of boilerplate code\n";
    cout << "  • Easy to forget a constructor\n";
    cout << "  • Error-prone parameter forwarding\n";
    cout << "  • Hard to maintain\n";
    cout << "  • Doesn't scale\n";
    cout << "\n";
    cout << "WITH C++11 (The Solution):\n";
    cout << "class Derived : public Base {\n";
    cout << "public:\n";
    cout << "    using Base::Base;  // ✅ One line!\n";
    cout << "};\n";
    cout << "\n";
    cout << "BENEFITS:\n";
    cout << "  • No boilerplate code\n";
    cout << "  • All constructors automatically available\n";
    cout << "  • Easy to maintain\n";
    cout << "  • Scales perfectly\n";
    cout << "  • Less error-prone\n";

    cout << "\n========================================\n";
    cout << "WHAT GETS INHERITED?\n";
    cout << "========================================\n";
    cout << "✅ INHERITED:\n";
    cout << "  • All public base constructors\n";
    cout << "  • All protected base constructors (stay protected)\n";
    cout << "  • Constructors with any parameter types\n";
    cout << "  • Constructors with any number of parameters\n";
    cout << "  • Template constructors\n";
    cout << "\n";
    cout << "❌ NOT INHERITED:\n";
    cout << "  • Default constructor\n";
    cout << "  • Copy constructor\n";
    cout << "  • Move constructor\n";
    cout << "  • Private base constructors\n";
    cout << "  • Copy assignment operator\n";
    cout << "  • Move assignment operator\n";
    cout << "\n";
    cout << "WHY NOT INHERITED:\n";
    cout << "  • Default/copy/move are special\n";
    cout << "  • Compiler generates them for derived class\n";
    cout << "  • Private constructors are inaccessible\n";
    cout << "  • Each class needs its own special members\n";

    cout << "\n========================================\n";
    cout << "RULES AND RESTRICTIONS\n";
    cout << "========================================\n";
    cout << "RULE 1: Only public/protected constructors\n";
    cout << "class Base {\n";
    cout << "private:\n";
    cout << "    Base(int);        // ❌ Not inherited\n";
    cout << "protected:\n";
    cout << "    Base(double);     // ✅ Inherited (protected)\n";
    cout << "public:\n";
    cout << "    Base(string);     // ✅ Inherited (public)\n";
    cout << "};\n";
    cout << "\n";
    cout << "RULE 2: Can override specific constructors\n";
    cout << "class Derived : public Base {\n";
    cout << "public:\n";
    cout << "    using Base::Base;           // Inherit all\n";
    cout << "    Derived(int x) : Base(x) {  // Override one\n";
    cout << "        // Custom logic\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "RULE 3: Works with default member initializers\n";
    cout << "class Derived : public Base {\n";
    cout << "    int value = 42;  // ✅ Initialized for inherited ctors\n";
    cout << "public:\n";
    cout << "    using Base::Base;\n";
    cout << "};\n";
    cout << "\n";
    cout << "RULE 4: Multiple inheritance\n";
    cout << "class Derived : public Base1, public Base2 {\n";
    cout << "public:\n";
    cout << "    using Base1::Base1;  // Inherit from Base1\n";
    cout << "    using Base2::Base2;  // Inherit from Base2\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "DEFAULT MEMBER INITIALIZERS\n";
    cout << "========================================\n";
    cout << "CRITICAL PATTERN:\n";
    cout << "When using inheriting constructors, derived class\n";
    cout << "members MUST use default member initializers!\n";
    cout << "\n";
    cout << "❌ BAD - Members uninitialized:\n";
    cout << "class Bad : public Base {\n";
    cout << "    string name;   // ⚠️  Uninitialized!\n";
    cout << "    int id;        // ⚠️  Uninitialized!\n";
    cout << "public:\n";
    cout << "    using Base::Base;\n";
    cout << "};\n";
    cout << "\n";
    cout << "✅ GOOD - Use default member initializers:\n";
    cout << "class Good : public Base {\n";
    cout << "    string name = \"Unknown\";  // ✅ Default value\n";
    cout << "    int id = 0;                // ✅ Default value\n";
    cout << "public:\n";
    cout << "    using Base::Base;\n";
    cout << "};\n";
    cout << "\n";
    cout << "WHY THIS WORKS:\n";
    cout << "  • Default member initializers execute before base constructor\n";
    cout << "  • Ensures all members properly initialized\n";
    cout << "  • Works seamlessly with inherited constructors\n";

    cout << "\n========================================\n";
    cout << "COMMON PATTERNS\n";
    cout << "========================================\n";
    cout << "PATTERN 1: Simple Wrapper\n";
    cout << "class Wrapper : public Base {\n";
    cout << "public:\n";
    cout << "    using Base::Base;  // No additional members\n";
    cout << "    \n";
    cout << "    // Add functionality without storage\n";
    cout << "    void extraMethod() { /* ... */ }\n";
    cout << "};\n";
    cout << "\n";
    cout << "PATTERN 2: Exception Hierarchy\n";
    cout << "class BaseException {\n";
    cout << "public:\n";
    cout << "    BaseException(const string& msg);\n";
    cout << "};\n";
    cout << "\n";
    cout << "class NetworkException : public BaseException {\n";
    cout << "public:\n";
    cout << "    using BaseException::BaseException;\n";
    cout << "};\n";
    cout << "\n";
    cout << "PATTERN 3: With Default Members\n";
    cout << "class Enhanced : public Base {\n";
    cout << "    string label = \"Default\";  // Default initializer\n";
    cout << "public:\n";
    cout << "    using Base::Base;\n";
    cout << "};\n";
    cout << "\n";
    cout << "PATTERN 4: Override Specific Constructor\n";
    cout << "class Selective : public Base {\n";
    cout << "public:\n";
    cout << "    using Base::Base;  // Inherit most\n";
    cout << "    \n";
    cout << "    // Override one for special handling\n";
    cout << "    Selective(int x) : Base(x) {\n";
    cout << "        // Custom initialization\n";
    cout << "    }\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "✅ DO:\n";
    cout << "  • Use for simple wrapper classes\n";
    cout << "  • Use for exception hierarchies\n";
    cout << "  • Use with default member initializers\n";
    cout << "  • Override specific constructors when needed\n";
    cout << "  • Document that constructors are inherited\n";
    cout << "  • Use when derived adds no state\n";
    cout << "  • Combine with virtual functions\n";
    cout << "\n";
    cout << "❌ DON'T:\n";
    cout << "  • Use with uninitialized members\n";
    cout << "  • Use when custom initialization needed\n";
    cout << "  • Use when validation needed\n";
    cout << "  • Forget about default member initializers\n";
    cout << "  • Use just to save typing if logic needed\n";
    cout << "  • Use with complex initialization\n";

    cout << "\n========================================\n";
    cout << "WHEN TO USE\n";
    cout << "========================================\n";
    cout << "✅ PERFECT FOR:\n";
    cout << "  • Exception class hierarchies\n";
    cout << "  • Simple wrapper classes\n";
    cout << "  • Tag/marker classes\n";
    cout << "  • Derived classes without state\n";
    cout << "  • Policy-based designs\n";
    cout << "  • Type-safe wrappers\n";
    cout << "\n";
    cout << "❌ NOT SUITABLE FOR:\n";
    cout << "  • Classes with additional members needing complex init\n";
    cout << "  • When parameter validation needed\n";
    cout << "  • When custom initialization logic required\n";
    cout << "  • When constructors need different behavior\n";
    cout << "  • Classes that manage resources differently\n";

    cout << "\n========================================\n";
    cout << "COMPARISON WITH ALTERNATIVES\n";
    cout << "========================================\n";
    cout << "OPTION 1: Manual Forwarding (Pre-C++11)\n";
    cout << "class Derived : public Base {\n";
    cout << "public:\n";
    cout << "    Derived(int x) : Base(x) { }\n";
    cout << "    Derived(int x, int y) : Base(x, y) { }\n";
    cout << "};\n";
    cout << "Pros: Full control, can customize\n";
    cout << "Cons: Boilerplate, maintenance burden\n";
    cout << "\n";
    cout << "OPTION 2: Inheriting Constructors (C++11+)\n";
    cout << "class Derived : public Base {\n";
    cout << "public:\n";
    cout << "    using Base::Base;\n";
    cout << "};\n";
    cout << "Pros: Concise, automatic, maintainable\n";
    cout << "Cons: Less control, needs default initializers\n";
    cout << "\n";
    cout << "OPTION 3: Hybrid Approach\n";
    cout << "class Derived : public Base {\n";
    cout << "public:\n";
    cout << "    using Base::Base;           // Inherit most\n";
    cout << "    Derived(int x) : Base(x) {  // Override some\n";
    cout << "        // Custom logic\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "Pros: Best of both worlds\n";
    cout << "Cons: Slightly more complex\n";

    cout << "\n========================================\n";
    cout << "COMMON MISTAKES\n";
    cout << "========================================\n";
    cout << "MISTAKE 1: Uninitialized members\n";
    cout << "❌ class Bad : public Base {\n";
    cout << "    string name;  // Uninitialized!\n";
    cout << "public:\n";
    cout << "    using Base::Base;\n";
    cout << "};\n";
    cout << "\n";
    cout << "✅ class Good : public Base {\n";
    cout << "    string name = \"Default\";  // Initialized\n";
    cout << "public:\n";
    cout << "    using Base::Base;\n";
    cout << "};\n";
    cout << "\n";
    cout << "MISTAKE 2: Expecting validation\n";
    cout << "❌ class Bad : public Base {\n";
    cout << "public:\n";
    cout << "    using Base::Base;  // Can't validate parameters!\n";
    cout << "};\n";
    cout << "\n";
    cout << "✅ class Good : public Base {\n";
    cout << "public:\n";
    cout << "    Good(int x) : Base(x) {\n";
    cout << "        if (x < 0) throw invalid_argument(\"x must be >= 0\");\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "MISTAKE 3: Forgetting special members aren't inherited\n";
    cout << "• Default constructor still needs to be defined if required\n";
    cout << "• Copy/move constructors are compiler-generated\n";

    cout << "\n========================================\n";
    cout << "REAL-WORLD EXAMPLES\n";
    cout << "========================================\n";
    cout << "EXAMPLE 1: Exception Hierarchy\n";
    cout << "class Exception {\n";
    cout << "    string msg;\n";
    cout << "public:\n";
    cout << "    Exception(const string& m) : msg(m) { }\n";
    cout << "};\n";
    cout << "\n";
    cout << "class IOError : public Exception {\n";
    cout << "public:\n";
    cout << "    using Exception::Exception;\n";
    cout << "};\n";
    cout << "\n";
    cout << "class NetworkError : public Exception {\n";
    cout << "public:\n";
    cout << "    using Exception::Exception;\n";
    cout << "};\n";
    cout << "\n";
    cout << "EXAMPLE 2: Type-Safe ID Wrapper\n";
    cout << "template<typename Tag>\n";
    cout << "class ID {\n";
    cout << "    int value;\n";
    cout << "public:\n";
    cout << "    explicit ID(int v) : value(v) { }\n";
    cout << "};\n";
    cout << "\n";
    cout << "struct UserTag {};\n";
    cout << "struct OrderTag {};\n";
    cout << "\n";
    cout << "class UserID : public ID<UserTag> {\n";
    cout << "public:\n";
    cout << "    using ID::ID;\n";
    cout << "};\n";
    cout << "\n";
    cout << "class OrderID : public ID<OrderTag> {\n";
    cout << "public:\n";
    cout << "    using ID::ID;\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "QUICK REFERENCE\n";
    cout << "========================================\n";
    cout << "// Basic syntax\n";
    cout << "class Derived : public Base {\n";
    cout << "public:\n";
    cout << "    using Base::Base;  // Inherit all base constructors\n";
    cout << "};\n";
    cout << "\n";
    cout << "// With default members\n";
    cout << "class Derived : public Base {\n";
    cout << "    string name = \"Default\";  // Must use default initializer\n";
    cout << "public:\n";
    cout << "    using Base::Base;\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Override specific constructor\n";
    cout << "class Derived : public Base {\n";
    cout << "public:\n";
    cout << "    using Base::Base;           // Inherit most\n";
    cout << "    Derived(int x) : Base(x) {  // Override one\n";
    cout << "        // Custom logic\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Multiple inheritance\n";
    cout << "class Derived : public Base1, public Base2 {\n";
    cout << "public:\n";
    cout << "    using Base1::Base1;\n";
    cout << "    using Base2::Base2;\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "KEY TAKEAWAYS\n";
    cout << "========================================\n";
    cout << "1. Inheriting constructors eliminate forwarding boilerplate\n";
    cout << "2. One line: using Base::Base;\n";
    cout << "3. Only public/protected constructors inherited\n";
    cout << "4. Default/copy/move NOT inherited (compiler-generated)\n";
    cout << "5. Use default member initializers for derived members\n";
    cout << "6. Can override specific constructors when needed\n";
    cout << "7. Perfect for wrappers and exception hierarchies\n";
    cout << "8. Available since C++11\n";
    cout << "9. Reduces maintenance burden significantly\n";
    cout << "10. Best for classes without complex initialization\n";

    cout << "\n========================================\n";
    cout << "DECISION TREE\n";
    cout << "========================================\n";
    cout << "Does derived class add new data members?\n";
    cout << "  NO  → Use inheriting constructors! ✅\n";
    cout << "  YES → Continue...\n";
    cout << "\n";
    cout << "Can members use default initializers?\n";
    cout << "  YES → Use inheriting constructors! ✅\n";
    cout << "  NO  → Continue...\n";
    cout << "\n";
    cout << "Need custom initialization logic?\n";
    cout << "  YES → Write explicit constructors ❌\n";
    cout << "  NO  → Continue...\n";
    cout << "\n";
    cout << "Need parameter validation?\n";
    cout << "  YES → Write explicit constructors ❌\n";
    cout << "  NO  → Use inheriting constructors! ✅\n";
    cout << "\n";
    cout << "GENERAL RULE:\n";
    cout << "  If derived class is a simple wrapper/extension,\n";
    cout << "  use inheriting constructors.\n";

    cout << "\n========================================\n";
    cout << "COMPARISON TABLE\n";
    cout << "========================================\n";
    cout << "┌────────────────────┬──────────────┬──────────────┐\n";
    cout << "│ Aspect             │ Manual       │ Inheriting   │\n";
    cout << "├────────────────────┼──────────────┼──────────────┤\n";
    cout << "│ Code amount        │ High         │ Minimal      │\n";
    cout << "│ Maintenance        │ Hard         │ Easy         │\n";
    cout << "│ Error-prone        │ Yes          │ No           │\n";
    cout << "│ Scalability        │ Poor         │ Excellent    │\n";
    cout << "│ Custom logic       │ Easy         │ Override     │\n";
    cout << "│ Validation         │ Easy         │ Need override│\n";
    cout << "│ Flexibility        │ Full         │ Good         │\n";
    cout << "│ Readability        │ Verbose      │ Clean        │\n";
    cout << "└────────────────────┴──────────────┴──────────────┘\n";

    return 0;
}
