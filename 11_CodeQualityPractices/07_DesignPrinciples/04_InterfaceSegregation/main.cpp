#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
using namespace std;

// ============================================
// INTERFACE SEGREGATION PRINCIPLE (ISP)
// ============================================

/*
INTERFACE SEGREGATION PRINCIPLE:
    "No client should be forced to depend on methods it does not use."
    — Robert C. Martin

WHAT IT MEANS:
    • Many small, focused interfaces are better than one large interface
    • Clients should not depend on methods they don't use
    • Split large interfaces into smaller, more specific ones
    • Classes implement only what they need

WHY IT MATTERS:
    ✅ No forced implementation of unused methods
    ✅ Better code organization
    ✅ Easier to understand and maintain
    ✅ Reduces coupling
    ✅ More flexible design

KEY CONCEPTS:
    • Fat Interface = Too many methods
    • Client-Specific Interfaces = Tailored to needs
    • Role Interfaces = Different roles, different interfaces
    • Interface Pollution = Forcing unnecessary dependencies

VIOLATION SIGNS:
    🚨 Empty method implementations (throwing NotImplementedException)
    🚨 Methods that do nothing or return null/default
    🚨 Classes implementing large interfaces but using few methods
    🚨 Method names with comments like "not supported"
    🚨 Frequent use of NotImplementedException

RED FLAGS:
    • "We'll just throw an exception in that method"
    • "We don't need this method but have to implement it"
    • "Let's just return null/false/0"
    • Interface with 10+ methods
*/

// ============================================
// EXAMPLE 1: MULTIFUNCTION PRINTER (Classic)
// ============================================

namespace bad_example1 {
// ❌ BAD: Fat interface forces all implementations

class IMachine {
public:
    virtual ~IMachine() = default;

    virtual void print(const string& document) = 0;
    virtual void scan(const string& document) = 0;
    virtual void fax(const string& document) = 0;
    virtual void staple() = 0;
};

// ✅ This works fine for multifunction printer
class MultiFunctionPrinter : public IMachine {
public:
    void print(const string& document) override {
        cout << "   🖨️  Printing: " << document << endl;
    }

    void scan(const string& document) override {
        cout << "   📄 Scanning: " << document << endl;
    }

    void fax(const string& document) override {
        cout << "   📠 Faxing: " << document << endl;
    }

    void staple() override {
        cout << "   📎 Stapling document" << endl;
    }
};

// 🚨 PROBLEM: Simple printer forced to implement everything!
class SimplePrinter : public IMachine {
public:
    void print(const string& document) override {
        cout << "   🖨️  Printing: " << document << endl;
    }

    // 🚨 VIOLATION: Forced to implement unused methods
    void scan(const string& document) override {
        throw runtime_error("Simple printer can't scan!");
    }

    void fax(const string& document) override {
        throw runtime_error("Simple printer can't fax!");
    }

    void staple() override {
        throw runtime_error("Simple printer can't staple!");
    }
};
}

namespace good_example1 {
// ✅ GOOD: Segregated interfaces

class IPrinter {
public:
    virtual ~IPrinter() = default;
    virtual void print(const string& document) = 0;
};

class IScanner {
public:
    virtual ~IScanner() = default;
    virtual void scan(const string& document) = 0;
};

class IFax {
public:
    virtual ~IFax() = default;
    virtual void fax(const string& document) = 0;
};

class IStapler {
public:
    virtual ~IStapler() = default;
    virtual void staple() = 0;
};

// Simple printer only implements what it needs
class SimplePrinter : public IPrinter {
public:
    void print(const string& document) override {
        cout << "   🖨️  Printing: " << document << endl;
    }
};

// Multifunction printer implements multiple interfaces
class MultiFunctionPrinter : public IPrinter, public IScanner, public IFax, public IStapler {
public:
    void print(const string& document) override {
        cout << "   🖨️  Printing: " << document << endl;
    }

    void scan(const string& document) override {
        cout << "   📄 Scanning: " << document << endl;
    }

    void fax(const string& document) override {
        cout << "   📠 Faxing: " << document << endl;
    }

    void staple() override {
        cout << "   📎 Stapling document" << endl;
    }
};

// Scanner-only device
class Scanner : public IScanner {
public:
    void scan(const string& document) override {
        cout << "   📄 Scanning: " << document << endl;
    }
};
}

void example1_printer() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 1: MULTIFUNCTION PRINTER               ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (Fat Interface):\n";
    cout << "```cpp\n";
    cout << "class IMachine {\n";
    cout << "    virtual void print() = 0;\n";
    cout << "    virtual void scan() = 0;\n";
    cout << "    virtual void fax() = 0;\n";
    cout << "    virtual void staple() = 0;\n";
    cout << "};\n\n";
    cout << "class SimplePrinter : public IMachine {\n";
    cout << "    void scan() override { throw exception(); }  // 🚨\n";
    cout << "    void fax() override { throw exception(); }   // 🚨\n";
    cout << "};\n";
    cout << "```\n";

    cout << "\n🚨 DEMONSTRATION OF VIOLATION:\n";

    bad_example1::SimplePrinter simplePrinter;
    cout << "\n   Simple printer printing:\n";
    simplePrinter.print("Report.pdf");

    cout << "\n   Simple printer trying to scan (will fail):\n";
    try {
        simplePrinter.scan("Document.pdf");
    } catch (const exception& e) {
        cout << "   ❌ Error: " << e.what() << endl;
    }

    cout << "\n🚨 PROBLEMS:\n";
    cout << "   • SimplePrinter forced to implement scan(), fax(), staple()\n";
    cout << "   • Methods throw exceptions (runtime errors!)\n";
    cout << "   • Violates ISP - depends on methods it doesn't use\n";
    cout << "   • Client code might call unsupported methods\n";

    cout << "\n✅ GOOD CODE (Segregated Interfaces):\n";
    cout << "```cpp\n";
    cout << "class IPrinter {\n";
    cout << "    virtual void print() = 0;\n";
    cout << "};\n\n";
    cout << "class IScanner {\n";
    cout << "    virtual void scan() = 0;\n";
    cout << "};\n\n";
    cout << "class SimplePrinter : public IPrinter {\n";
    cout << "    void print() override { /* only this! */ }\n";
    cout << "};\n\n";
    cout << "class MultiFunctionPrinter : public IPrinter, IScanner {\n";
    cout << "    void print() override { /* ... */ }\n";
    cout << "    void scan() override { /* ... */ }\n";
    cout << "};\n";
    cout << "```\n";

    cout << "\n✨ DEMONSTRATION OF SOLUTION:\n";

    good_example1::SimplePrinter goodSimple;
    cout << "\n   Simple printer (only prints):\n";
    goodSimple.print("Report.pdf");

    good_example1::MultiFunctionPrinter goodMulti;
    cout << "\n   Multifunction printer:\n";
    goodMulti.print("Document.pdf");
    goodMulti.scan("Document.pdf");
    goodMulti.fax("Document.pdf");

    cout << "\n✨ SOLUTION:\n";
    cout << "   • Each interface has single responsibility\n";
    cout << "   • SimplePrinter only implements IPrinter\n";
    cout << "   • MultiFunctionPrinter implements multiple interfaces\n";
    cout << "   • No forced implementation of unused methods\n";
}

// ============================================
// EXAMPLE 2: WORKER INTERFACE (Robot vs Human)
// ============================================

namespace bad_example2 {
// ❌ BAD: Fat interface with methods not applicable to all

class IWorker {
public:
    virtual ~IWorker() = default;

    virtual void work() = 0;
    virtual void eat() = 0;      // Not all workers eat!
    virtual void sleep() = 0;    // Not all workers sleep!
};

class HumanWorker : public IWorker {
public:
    void work() override {
        cout << "   👷 Human working" << endl;
    }

    void eat() override {
        cout << "   🍔 Human eating" << endl;
    }

    void sleep() override {
        cout << "   😴 Human sleeping" << endl;
    }
};

class RobotWorker : public IWorker {
public:
    void work() override {
        cout << "   🤖 Robot working" << endl;
    }

    // 🚨 VIOLATION: Robots don't eat or sleep!
    void eat() override {
        throw runtime_error("Robots don't eat!");
    }

    void sleep() override {
        throw runtime_error("Robots don't sleep!");
    }
};
}

namespace good_example2 {
// ✅ GOOD: Segregated interfaces

class IWorkable {
public:
    virtual ~IWorkable() = default;
    virtual void work() = 0;
};

class IFeedable {
public:
    virtual ~IFeedable() = default;
    virtual void eat() = 0;
};

class ISleepable {
public:
    virtual ~ISleepable() = default;
    virtual void sleep() = 0;
};

class HumanWorker : public IWorkable, public IFeedable, public ISleepable {
public:
    void work() override {
        cout << "   👷 Human working" << endl;
    }

    void eat() override {
        cout << "   🍔 Human eating" << endl;
    }

    void sleep() override {
        cout << "   😴 Human sleeping" << endl;
    }
};

class RobotWorker : public IWorkable {
public:
    void work() override {
        cout << "   🤖 Robot working 24/7" << endl;
    }
    // No eat() or sleep() - type system prevents calling them!
};
}

void example2_worker() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 2: WORKER INTERFACE                    ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (Fat Interface):\n";
    cout << "```cpp\n";
    cout << "class IWorker {\n";
    cout << "    virtual void work() = 0;\n";
    cout << "    virtual void eat() = 0;\n";
    cout << "    virtual void sleep() = 0;\n";
    cout << "};\n\n";
    cout << "class RobotWorker : public IWorker {\n";
    cout << "    void eat() override { throw exception(); }   // 🚨\n";
    cout << "    void sleep() override { throw exception(); } // 🚨\n";
    cout << "};\n";
    cout << "```\n";

    cout << "\n🚨 DEMONSTRATION:\n";

    bad_example2::RobotWorker badRobot;
    cout << "\n   Robot working:\n";
    badRobot.work();

    cout << "\n   Trying to make robot eat:\n";
    try {
        badRobot.eat();
    } catch (const exception& e) {
        cout << "   ❌ Error: " << e.what() << endl;
    }

    cout << "\n✅ GOOD CODE (Segregated Interfaces):\n";
    cout << "```cpp\n";
    cout << "class IWorkable { virtual void work() = 0; };\n";
    cout << "class IFeedable { virtual void eat() = 0; };\n";
    cout << "class ISleepable { virtual void sleep() = 0; };\n\n";
    cout << "class HumanWorker : public IWorkable, IFeedable, ISleepable { };\n";
    cout << "class RobotWorker : public IWorkable { /* only work! */ };\n";
    cout << "```\n";

    cout << "\n✨ DEMONSTRATION:\n";

    good_example2::RobotWorker goodRobot;
    cout << "\n   Robot working:\n";
    goodRobot.work();

    good_example2::HumanWorker goodHuman;
    cout << "\n   Human worker:\n";
    goodHuman.work();
    goodHuman.eat();
    goodHuman.sleep();

    cout << "\n✨ BENEFITS:\n";
    cout << "   • RobotWorker only implements IWorkable\n";
    cout << "   • No forced implementation of eat() and sleep()\n";
    cout << "   • Type system prevents calling non-existent methods\n";
    cout << "   • Cleaner, more maintainable code\n";
}

// ============================================
// EXAMPLE 3: DOCUMENT INTERFACE (Read/Write)
// ============================================

namespace bad_example3 {
// ❌ BAD: All documents forced to be read-write

class IDocument {
public:
    virtual ~IDocument() = default;

    virtual string read() = 0;
    virtual void write(const string& content) = 0;
    virtual void save() = 0;
    virtual void delete_doc() = 0;
};

class EditableDocument : public IDocument {
private:
    string content;

public:
    string read() override {
        return content;
    }

    void write(const string& newContent) override {
        content = newContent;
    }

    void save() override {
        cout << "   💾 Saving document" << endl;
    }

    void delete_doc() override {
        cout << "   🗑️  Deleting document" << endl;
    }
};

class ReadOnlyDocument : public IDocument {
private:
    string content;

public:
    ReadOnlyDocument(const string& initialContent) : content(initialContent) {}

    string read() override {
        return content;
    }

    // 🚨 VIOLATION: Forced to implement write operations
    void write(const string& newContent) override {
        throw runtime_error("Document is read-only!");
    }

    void save() override {
        throw runtime_error("Cannot save read-only document!");
    }

    void delete_doc() override {
        throw runtime_error("Cannot delete read-only document!");
    }
};
}

namespace good_example3 {
// ✅ GOOD: Segregated read and write interfaces

class IReadable {
public:
    virtual ~IReadable() = default;
    virtual string read() = 0;
};

class IWritable {
public:
    virtual ~IWritable() = default;
    virtual void write(const string& content) = 0;
};

class ISaveable {
public:
    virtual ~ISaveable() = default;
    virtual void save() = 0;
};

class IDeletable {
public:
    virtual ~IDeletable() = default;
    virtual void delete_doc() = 0;
};

class ReadOnlyDocument : public IReadable {
private:
    string content;

public:
    ReadOnlyDocument(const string& initialContent) : content(initialContent) {}

    string read() override {
        return content;
    }
};

class EditableDocument : public IReadable, public IWritable, public ISaveable, public IDeletable {
private:
    string content;

public:
    string read() override {
        return content;
    }

    void write(const string& newContent) override {
        content = newContent;
    }

    void save() override {
        cout << "   💾 Saving document" << endl;
    }

    void delete_doc() override {
        cout << "   🗑️  Deleting document" << endl;
    }
};
}

void example3_document() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 3: DOCUMENT INTERFACE                  ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE:\n";
    cout << "   ReadOnlyDocument forced to implement write(), save(), delete()\n";

    bad_example3::ReadOnlyDocument badReadOnly("Important data");
    cout << "\n   Reading read-only document:\n";
    cout << "   📄 Content: " << badReadOnly.read() << endl;

    cout << "\n   Trying to write to read-only document:\n";
    try {
        badReadOnly.write("New content");
    } catch (const exception& e) {
        cout << "   ❌ Error: " << e.what() << endl;
    }

    cout << "\n✅ GOOD CODE:\n";
    cout << "   ReadOnlyDocument only implements IReadable\n";

    good_example3::ReadOnlyDocument goodReadOnly("Important data");
    cout << "\n   Reading read-only document:\n";
    cout << "   📄 Content: " << goodReadOnly.read() << endl;
    // Can't even call write() - compiler prevents it!

    cout << "\n   EditableDocument has all capabilities:\n";
    good_example3::EditableDocument editable;
    editable.write("New content");
    editable.save();

    cout << "\n✨ BENEFITS:\n";
    cout << "   • Type system enforces read-only nature\n";
    cout << "   • No runtime exceptions for unsupported operations\n";
    cout << "   • Clear intent from interface\n";
}

// ============================================
// EXAMPLE 4: SHAPE INTERFACE
// ============================================

namespace bad_example4 {
// ❌ BAD: All shapes forced to have volume

class IShape {
public:
    virtual ~IShape() = default;

    virtual double getArea() = 0;
    virtual double getVolume() = 0;  // Not all shapes have volume!
};

class Circle : public IShape {
private:
    double radius;

public:
    Circle(double r) : radius(r) {}

    double getArea() override {
        return 3.14159 * radius * radius;
    }

    // 🚨 VIOLATION: 2D shape doesn't have volume
    double getVolume() override {
        throw runtime_error("Circle is 2D, no volume!");
    }
};

class Sphere : public IShape {
private:
    double radius;

public:
    Sphere(double r) : radius(r) {}

    double getArea() override {
        return 4 * 3.14159 * radius * radius;
    }

    double getVolume() override {
        return (4.0/3.0) * 3.14159 * radius * radius * radius;
    }
};
}

namespace good_example4 {
// ✅ GOOD: Separate 2D and 3D interfaces

class IShape {
public:
    virtual ~IShape() = default;
};

class IShape2D : public IShape {
public:
    virtual double getArea() = 0;
};

class IShape3D : public IShape {
public:
    virtual double getSurfaceArea() = 0;
    virtual double getVolume() = 0;
};

class Circle : public IShape2D {
private:
    double radius;

public:
    Circle(double r) : radius(r) {}

    double getArea() override {
        return 3.14159 * radius * radius;
    }
};

class Rectangle : public IShape2D {
private:
    double width, height;

public:
    Rectangle(double w, double h) : width(w), height(h) {}

    double getArea() override {
        return width * height;
    }
};

class Sphere : public IShape3D {
private:
    double radius;

public:
    Sphere(double r) : radius(r) {}

    double getSurfaceArea() override {
        return 4 * 3.14159 * radius * radius;
    }

    double getVolume() override {
        return (4.0/3.0) * 3.14159 * radius * radius * radius;
    }
};
}

void example4_shapes() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 4: SHAPE INTERFACE                     ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE:\n";
    cout << "   All shapes forced to implement getVolume()\n";

    bad_example4::Circle badCircle(5.0);
    cout << "\n   Circle area: " << badCircle.getArea() << endl;
    cout << "   Trying to get circle volume:\n";
    try {
        badCircle.getVolume();
    } catch (const exception& e) {
        cout << "   ❌ Error: " << e.what() << endl;
    }

    cout << "\n✅ GOOD CODE:\n";
    cout << "   2D shapes implement IShape2D\n";
    cout << "   3D shapes implement IShape3D\n";

    good_example4::Circle goodCircle(5.0);
    cout << "\n   Circle (2D): Area = " << goodCircle.getArea() << endl;

    good_example4::Sphere sphere(5.0);
    cout << "   Sphere (3D): Surface Area = " << sphere.getSurfaceArea() << endl;
    cout << "   Sphere (3D): Volume = " << sphere.getVolume() << endl;

    cout << "\n✨ BENEFITS:\n";
    cout << "   • 2D shapes don't have getVolume() method\n";
    cout << "   • 3D shapes have both surface area and volume\n";
    cout << "   • Type system enforces correct usage\n";
}

// ============================================
// EXAMPLE 5: HOW TO FOLLOW ISP
// ============================================

void example5_how_to_follow() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 5: HOW TO FOLLOW ISP                   ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n✅ GUIDELINES TO FOLLOW ISP:\n";

    cout << "\n1. IDENTIFY CLIENT NEEDS:\n";
    cout << "   • What does each client actually use?\n";
    cout << "   • Group related functionality\n";
    cout << "   • Create role-based interfaces\n";

    cout << "\n2. SPLIT FAT INTERFACES:\n";
    cout << "   ❌ interface IWorker { work(); eat(); sleep(); }\n";
    cout << "   ✅ interface IWorkable { work(); }\n";
    cout << "   ✅ interface IFeedable { eat(); }\n";
    cout << "   ✅ interface ISleepable { sleep(); }\n";

    cout << "\n3. USE MULTIPLE SMALL INTERFACES:\n";
    cout << "   • Each interface = one responsibility\n";
    cout << "   • Classes can implement multiple interfaces\n";
    cout << "   • More flexible composition\n";

    cout << "\n4. AVOID THESE PATTERNS:\n";
    cout << "   ❌ Empty implementations\n";
    cout << "   ❌ Throwing NotImplementedException\n";
    cout << "   ❌ Returning null/default for unsupported methods\n";
    cout << "   ❌ Comments like \"not supported\" or \"not applicable\"\n";

    cout << "\n5. PREFER COHESIVE INTERFACES:\n";
    cout << "   • Methods should be related\n";
    cout << "   • High cohesion = methods work together\n";
    cout << "   • Low coupling = independent interfaces\n";

    cout << "\n6. CONSIDER THE CLIENT:\n";
    cout << "   • Design from client perspective\n";
    cout << "   • What does the client need?\n";
    cout << "   • Don't force unnecessary dependencies\n";

    cout << "\n🔍 CHECKLIST:\n";
    cout << "   □ Does the class implement methods it doesn't use?\n";
    cout << "   □ Are there empty or throwing method implementations?\n";
    cout << "   □ Can the interface be split into smaller parts?\n";
    cout << "   □ Do clients depend on methods they don't call?\n";
    cout << "   □ Is the interface too large (10+ methods)?\n";

    cout << "\n💡 INTERFACE SIZE GUIDELINE:\n";
    cout << "   • 1-3 methods = Good ✅\n";
    cout << "   • 4-6 methods = Acceptable ⚠️  (consider splitting)\n";
    cout << "   • 7+ methods = Too large 🚨 (definitely split)\n";
}

// ============================================
// EXAMPLE 6: BENEFITS & SUMMARY
// ============================================

void example6_benefits() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 6: BENEFITS & SUMMARY                  ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n✅ BENEFITS OF ISP:\n";

    cout << "\n1. CLEANER CODE:\n";
    cout << "   • No forced empty implementations\n";
    cout << "   • Clear intent from interface name\n";
    cout << "   • Easier to understand what class does\n";

    cout << "\n2. BETTER FLEXIBILITY:\n";
    cout << "   • Classes implement only what they need\n";
    cout << "   • Easy to add new implementations\n";
    cout << "   • Multiple small interfaces = more combinations\n";

    cout << "\n3. REDUCED COUPLING:\n";
    cout << "   • Clients depend only on methods they use\n";
    cout << "   • Changes to unused methods don't affect clients\n";
    cout << "   • Less ripple effect from changes\n";

    cout << "\n4. COMPILE-TIME SAFETY:\n";
    cout << "   • Type system prevents calling unsupported methods\n";
    cout << "   • No runtime exceptions for \"not implemented\"\n";
    cout << "   • Errors caught during compilation\n";

    cout << "\n5. EASIER TESTING:\n";
    cout << "   • Mock only the methods you need\n";
    cout << "   • Smaller interfaces = simpler mocks\n";
    cout << "   • More focused unit tests\n";

    cout << "\n⚠️  COMMON VIOLATIONS:\n";
    cout << "   • Multifunction printer with all methods\n";
    cout << "   • Worker interface with eat/sleep for robots\n";
    cout << "   • Document interface forcing write on read-only\n";
    cout << "   • Shape interface with volume for 2D shapes\n";

    cout << "\n💡 KEY INSIGHT:\n";
    cout << "   \"Make fine-grained interfaces that are client-specific.\n";
    cout << "    Many small interfaces beat one large interface!\"\n";

    cout << "\n🎯 REMEMBER:\n";
    cout << "   • No client should depend on methods it doesn't use\n";
    cout << "   • Split large interfaces into smaller, focused ones\n";
    cout << "   • Design from client perspective\n";
    cout << "   • Prefer multiple inheritance of small interfaces\n";

    cout << "\n📊 ISP vs OTHER PRINCIPLES:\n";
    cout << "   • ISP + SRP = Well-designed interfaces\n";
    cout << "   • ISP + OCP = Easy to extend\n";
    cout << "   • ISP + LSP = Proper inheritance\n";
    cout << "   • ISP + DIP = Flexible dependencies\n";
}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║      INTERFACE SEGREGATION PRINCIPLE (ISP)            ║\n";
    cout << "║                                                       ║\n";
    cout << "║  \"No client should be forced to depend on methods     ║\n";
    cout << "║   it does not use.\"                                  ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n";

    example1_printer();
    example2_worker();
    example3_document();
    example4_shapes();
    example5_how_to_follow();
    example6_benefits();

    cout << "\n\n";
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║    🎉 INTERFACE SEGREGATION PRINCIPLE COMPLETE! 🎉    ║\n";
    cout << "║                                                       ║\n";
    cout << "║  Key Takeaway: Many small, focused interfaces are     ║\n";
    cout << "║  better than one large, fat interface. Design from   ║\n";
    cout << "║  the client's perspective!                           ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n\n";

    return 0;
}
