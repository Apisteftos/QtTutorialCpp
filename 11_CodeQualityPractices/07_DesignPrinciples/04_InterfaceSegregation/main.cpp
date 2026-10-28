#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#include <print>

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

    virtual void print(const std::string& document) = 0;
    virtual void scan(const std::string& document) = 0;
    virtual void fax(const std::string& document) = 0;
    virtual void staple() = 0;
};

// ✅ This works fine for multifunction printer
class MultiFunctionPrinter : public IMachine {
public:
    void print(const std::string& document) override {
        std::println("   🖨️  Printing: {}", document);
    }

    void scan(const std::string& document) override {
        std::println("   📄 Scanning: {}", document);
    }

    void fax(const std::string& document) override {
        std::println("   📠 Faxing: {}", document);
    }

    void staple() override {
        std::println("   📎 Stapling document");
    }
};

// 🚨 PROBLEM: Simple printer forced to implement everything!
class SimplePrinter : public IMachine {
public:
    void print(const std::string& document) override {
        std::println("   🖨️  Printing: {}", document);
    }

    // 🚨 VIOLATION: Forced to implement unused methods
    void scan(const std::string& document) override {
        throw std::runtime_error("Simple printer can't scan!");
    }

    void fax(const std::string& document) override {
        throw std::runtime_error("Simple printer can't fax!");
    }

    void staple() override {
        throw std::runtime_error("Simple printer can't staple!");
    }
};
}

namespace good_example1 {
// ✅ GOOD: Segregated interfaces

class IPrinter {
public:
    virtual ~IPrinter() = default;
    virtual void print(const std::string& document) = 0;
};

class IScanner {
public:
    virtual ~IScanner() = default;
    virtual void scan(const std::string& document) = 0;
};

class IFax {
public:
    virtual ~IFax() = default;
    virtual void fax(const std::string& document) = 0;
};

class IStapler {
public:
    virtual ~IStapler() = default;
    virtual void staple() = 0;
};

// Simple printer only implements what it needs
class SimplePrinter : public IPrinter {
public:
    void print(const std::string& document) override {
        std::println("   🖨️  Printing: {}", document);
    }
};

// Multifunction printer implements multiple interfaces
class MultiFunctionPrinter : public IPrinter, public IScanner, public IFax, public IStapler {
public:
    void print(const std::string& document) override {
        std::println("   🖨️  Printing: {}", document);
    }

    void scan(const std::string& document) override {
        std::println("   📄 Scanning: {}", document);
    }

    void fax(const std::string& document) override {
        std::println("   📠 Faxing: {}", document);
    }

    void staple() override {
        std::println("   📎 Stapling document");
    }
};

// Scanner-only device
class Scanner : public IScanner {
public:
    void scan(const std::string& document) override {
        std::println("   📄 Scanning: {}", document);
    }
};
}

void example1_printer() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 1: MULTIFUNCTION PRINTER               ║");
    std::println("╚══════════════════════════════════════════════════╝");

    bad_example1::SimplePrinter simplePrinter;
    std::println("");
    std::println("   Simple printer printing:");
    simplePrinter.print("Report.pdf");

    std::println("");
    std::println("   Simple printer trying to scan (will fail):");
    try {
        simplePrinter.scan("Document.pdf");
    } catch (const std::exception& e) {
        std::println("   ❌ Error: {}", e.what());
    }

    good_example1::SimplePrinter goodSimple;
    std::println("");
    std::println("   Simple printer (only prints):");
    goodSimple.print("Report.pdf");

    good_example1::MultiFunctionPrinter goodMulti;
    std::println("");
    std::println("   Multifunction printer:");
    goodMulti.print("Document.pdf");
    goodMulti.scan("Document.pdf");
    goodMulti.fax("Document.pdf");

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
        std::println("   👷 Human working");
    }

    void eat() override {
        std::println("   🍔 Human eating");
    }

    void sleep() override {
        std::println("   😴 Human sleeping");
    }
};

class RobotWorker : public IWorker {
public:
    void work() override {
        std::println("   🤖 Robot working");
    }

    // 🚨 VIOLATION: Robots don't eat or sleep!
    void eat() override {
        throw std::runtime_error("Robots don't eat!");
    }

    void sleep() override {
        throw std::runtime_error("Robots don't sleep!");
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
        std::println("   👷 Human working");
    }

    void eat() override {
        std::println("   🍔 Human eating");
    }

    void sleep() override {
        std::println("   😴 Human sleeping");
    }
};

class RobotWorker : public IWorkable {
public:
    void work() override {
        std::println("   🤖 Robot working 24/7");
    }
    // No eat() or sleep() - type system prevents calling them!
};
}

void example2_worker() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 2: WORKER INTERFACE                    ║");
    std::println("╚══════════════════════════════════════════════════╝");

    bad_example2::RobotWorker badRobot;
    std::println("");
    std::println("   Robot working:");
    badRobot.work();

    std::println("");
    std::println("   Trying to make robot eat:");
    try {
        badRobot.eat();
    } catch (const std::exception& e) {
        std::println("   ❌ Error: {}", e.what());
    }

    good_example2::RobotWorker goodRobot;
    std::println("");
    std::println("   Robot working:");
    goodRobot.work();

    good_example2::HumanWorker goodHuman;
    std::println("");
    std::println("   Human worker:");
    goodHuman.work();
    goodHuman.eat();
    goodHuman.sleep();

}

// ============================================
// EXAMPLE 3: DOCUMENT INTERFACE (Read/Write)
// ============================================

namespace bad_example3 {
// ❌ BAD: All documents forced to be read-write

class IDocument {
public:
    virtual ~IDocument() = default;

    virtual std::string read() = 0;
    virtual void write(const std::string& content) = 0;
    virtual void save() = 0;
    virtual void delete_doc() = 0;
};

class EditableDocument : public IDocument {
private:
    std::string content;

public:
    std::string read() override {
        return content;
    }

    void write(const std::string& newContent) override {
        content = newContent;
    }

    void save() override {
        std::println("   💾 Saving document");
    }

    void delete_doc() override {
        std::println("   🗑️  Deleting document");
    }
};

class ReadOnlyDocument : public IDocument {
private:
    std::string content;

public:
    ReadOnlyDocument(const std::string& initialContent) : content(initialContent) {}

    std::string read() override {
        return content;
    }

    // 🚨 VIOLATION: Forced to implement write operations
    void write(const std::string& newContent) override {
        throw std::runtime_error("Document is read-only!");
    }

    void save() override {
        throw std::runtime_error("Cannot save read-only document!");
    }

    void delete_doc() override {
        throw std::runtime_error("Cannot delete read-only document!");
    }
};
}

namespace good_example3 {
// ✅ GOOD: Segregated read and write interfaces

class IReadable {
public:
    virtual ~IReadable() = default;
    virtual std::string read() = 0;
};

class IWritable {
public:
    virtual ~IWritable() = default;
    virtual void write(const std::string& content) = 0;
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
    std::string content;

public:
    ReadOnlyDocument(const std::string& initialContent) : content(initialContent) {}

    std::string read() override {
        return content;
    }
};

class EditableDocument : public IReadable, public IWritable, public ISaveable, public IDeletable {
private:
    std::string content;

public:
    std::string read() override {
        return content;
    }

    void write(const std::string& newContent) override {
        content = newContent;
    }

    void save() override {
        std::println("   💾 Saving document");
    }

    void delete_doc() override {
        std::println("   🗑️  Deleting document");
    }
};
}

void example3_document() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 3: DOCUMENT INTERFACE                  ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("");
    std::println("❌ BAD CODE:");
    std::println("   ReadOnlyDocument forced to implement write(), save(), delete()");

    bad_example3::ReadOnlyDocument badReadOnly("Important data");
    std::println("");
    std::println("   Reading read-only document:");
    std::println("   📄 Content: {}", badReadOnly.read());

    std::println("");
    std::println("   Trying to write to read-only document:");
    try {
        badReadOnly.write("New content");
    } catch (const std::exception& e) {
        std::println("   ❌ Error: {}", e.what());
    }

    std::println("");
    std::println("✅ GOOD CODE:");
    std::println("   ReadOnlyDocument only implements IReadable");

    good_example3::ReadOnlyDocument goodReadOnly("Important data");
    std::println("");
    std::println("   Reading read-only document:");
    std::println("   📄 Content: {}", goodReadOnly.read());
    // Can't even call write() - compiler prevents it!

    std::println("");
    std::println("   EditableDocument has all capabilities:");
    good_example3::EditableDocument editable;
    editable.write("New content");
    editable.save();

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
        throw std::runtime_error("Circle is 2D, no volume!");
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
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 4: SHAPE INTERFACE                     ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("");
    std::println("❌ BAD CODE:");
    std::println("   All shapes forced to implement getVolume()");

    bad_example4::Circle badCircle(5.0);
    std::println("");
    std::println("   Circle area: {}", badCircle.getArea());
    std::println("   Trying to get circle volume:");
    try {
        badCircle.getVolume();
    } catch (const std::exception& e) {
        std::println("   ❌ Error: {}", e.what());
    }

    good_example4::Circle goodCircle(5.0);
    std::println("");
    std::println("   Circle (2D): Area = {}", goodCircle.getArea());

    good_example4::Sphere sphere(5.0);
    std::println("   Sphere (3D): Surface Area = {}", sphere.getSurfaceArea());
    std::println("   Sphere (3D): Volume = {}", sphere.getVolume());

}

// ============================================
// EXAMPLE 5: HOW TO FOLLOW ISP
// ============================================

void example5_how_to_follow() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 5: HOW TO FOLLOW ISP                   ║");
    std::println("╚══════════════════════════════════════════════════╝");

}

// ============================================
// EXAMPLE 6: BENEFITS & SUMMARY
// ============================================

void example6_benefits() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 6: BENEFITS & SUMMARY                  ║");
    std::println("╚══════════════════════════════════════════════════╝");

}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    std::println("╔═══════════════════════════════════════════════════════╗");
    std::println("║                                                       ║");
    std::println("║      INTERFACE SEGREGATION PRINCIPLE (ISP)            ║");
    std::println("║                                                       ║");
    std::println("║  \"No client should be forced to depend on methods    ║");
    std::println("║   it does not use.\"                                  ║");
    std::println("║                                                       ║");
    std::println("╚═══════════════════════════════════════════════════════╝");

    example1_printer();
    example2_worker();
    example3_document();
    example4_shapes();
    example5_how_to_follow();
    example6_benefits();

    return 0;
}
