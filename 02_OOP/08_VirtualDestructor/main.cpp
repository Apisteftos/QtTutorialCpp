#include <print>
#include <string>
#include <vector>
#include <memory>
#include <iostream>

using namespace std;

// ============================================
// WHAT ARE VIRTUAL DESTRUCTORS?
// ============================================

/*
 * VIRTUAL DESTRUCTORS:
 * - Destructors declared with 'virtual' keyword
 * - CRITICAL for polymorphic base classes
 * - Ensures proper cleanup in inheritance hierarchies
 * - Prevents memory leaks and undefined behavior
 *
 * WHY VIRTUAL DESTRUCTORS?
 * - Without virtual: Only base destructor called
 * - With virtual: Both derived and base destructors called
 * - Essential when deleting via base pointer
 *
 * SYNTAX:
 * - virtual ~ClassName() { }
 * - Or: virtual ~ClassName() = default;
 *
 * RULES:
 * - If class has ANY virtual function → destructor MUST be virtual
 * - If class is meant to be a base class → destructor should be virtual
 * - Virtual destructors call derived destructors first
 * - Destruction order: derived → base (reverse of construction)
 *
 * SPECIAL CASES:
 * - Pure virtual destructor: virtual ~ClassName() = 0;
 * - Default virtual destructor: virtual ~ClassName() = default;
 * - Protected non-virtual destructor (prevent polymorphic deletion)
 */

// ============================================
// EXAMPLE 1: WITHOUT VIRTUAL DESTRUCTOR (The Problem)
// ============================================

class BaseNoVirtual {
public:
    BaseNoVirtual() {
        std::println("  BaseNoVirtual constructor");
    }

    // NOT virtual - THIS IS THE PROBLEM!
    ~BaseNoVirtual() {
        std::println("  BaseNoVirtual destructor");
    }

    virtual void display() {
        std::println("  BaseNoVirtual::display()");
    }
};

class DerivedNoVirtual : public BaseNoVirtual {
private:
    int* data;  // Dynamically allocated

public:
    DerivedNoVirtual() {
        data = new int[100];
        std::println("  DerivedNoVirtual constructor (allocated 100 ints)");
    }

    ~DerivedNoVirtual() {
        delete[] data;
        std::println("  DerivedNoVirtual destructor (freed memory)");
    }

    void display() override {
        std::println("  DerivedNoVirtual::display()");
    }
};

void demonstrateWithoutVirtual() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: WITHOUT VIRTUAL        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating DerivedNoVirtual object ---");
    BaseNoVirtual* ptr = new DerivedNoVirtual();

    std::println("");
    std::println("--- Using polymorphism (works) ---");
    ptr->display();  // Calls DerivedNoVirtual::display() ✅

    std::println("");
    std::println("--- Deleting via base pointer ---");
    delete ptr;  // ⚠️ ONLY calls BaseNoVirtual destructor!
}

// ============================================
// EXAMPLE 2: WITH VIRTUAL DESTRUCTOR (The Solution)
// ============================================

class BaseWithVirtual {
public:
    BaseWithVirtual() {
        std::println("  BaseWithVirtual constructor");
    }

    // VIRTUAL destructor - THIS IS THE SOLUTION!
    virtual ~BaseWithVirtual() {
        std::println("  BaseWithVirtual destructor");
    }

    virtual void display() {
        std::println("  BaseWithVirtual::display()");
    }
};

class DerivedWithVirtual : public BaseWithVirtual {
private:
    int* data;  // Dynamically allocated

public:
    DerivedWithVirtual() {
        data = new int[100];
        std::println("  DerivedWithVirtual constructor (allocated 100 ints)");
    }

    ~DerivedWithVirtual() {
        delete[] data;
        std::println("  DerivedWithVirtual destructor (freed memory)");
    }

    void display() override {
        std::println("  DerivedWithVirtual::display()");
    }
};

void demonstrateWithVirtual() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: WITH VIRTUAL           ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating DerivedWithVirtual object ---");
    BaseWithVirtual* ptr = new DerivedWithVirtual();

    std::println("");
    std::println("--- Using polymorphism (works) ---");
    ptr->display();  // Calls DerivedWithVirtual::display() ✅

    std::println("");
    std::println("--- Deleting via base pointer ---");
    delete ptr;  // ✅ Calls BOTH destructors!
}

// ============================================
// EXAMPLE 3: COMPARISON SIDE-BY-SIDE
// ============================================

void demonstrateComparison() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: SIDE-BY-SIDE           ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- WITHOUT virtual destructor ---");
    {
        BaseNoVirtual* ptr1 = new DerivedNoVirtual();
        delete ptr1;  // Only base destructor called 💥
    }

    std::println("");
    std::println("--- WITH virtual destructor ---");
    {
        BaseWithVirtual* ptr2 = new DerivedWithVirtual();
        delete ptr2;  // Both destructors called ✅
    }

    std::println("");
    std::println("--- Direct object (no pointer) ---");
    {
        std::println("Creating DerivedNoVirtual directly:");
        DerivedNoVirtual obj1;
        std::println("Destroying (both destructors called even without virtual):");
    }
}

// ============================================
// EXAMPLE 4: DESTRUCTION ORDER
// ============================================

class GrandParent {
public:
    GrandParent() {
        std::println("  1. GrandParent constructor");
    }

    virtual ~GrandParent() {
        std::println("  3. GrandParent destructor");
    }
};

class Parent : public GrandParent {
public:
    Parent() {
        std::println("  2. Parent constructor");
    }

    ~Parent() {
        std::println("  2. Parent destructor");
    }
};

class Child : public Parent {
public:
    Child() {
        std::println("  3. Child constructor");
    }

    ~Child() {
        std::println("  1. Child destructor");
    }
};

void demonstrateDestructionOrder() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: DESTRUCTION ORDER      ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Construction order (base → derived) ---");
    GrandParent* ptr = new Child();

    std::println("");
    std::println("--- Destruction order (derived → base) ---");
    delete ptr;
}

// ============================================
// EXAMPLE 5: DEFAULT VIRTUAL DESTRUCTOR
// ============================================

class SimpleBase {
public:
    // Default virtual destructor (C++11)
    virtual ~SimpleBase() = default;

    virtual void func() { }
};

class SimpleDerived : public SimpleBase {
    // Implicitly has virtual destructor
};

void demonstrateDefaultVirtual() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 5: DEFAULT VIRTUAL        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Using = default for virtual destructor ---");
    SimpleBase* ptr = new SimpleDerived();
    delete ptr;
}

// ============================================
// EXAMPLE 6: PURE VIRTUAL DESTRUCTOR
// ============================================

class AbstractBase {
public:
    // Pure virtual destructor
    virtual ~AbstractBase() = 0;

    virtual void pureFunc() = 0;
};

// MUST provide implementation for pure virtual destructor
AbstractBase::~AbstractBase() {
    std::println("  AbstractBase destructor (pure virtual)");
}

class ConcreteClass : public AbstractBase {
public:
    ~ConcreteClass() {
        std::println("  ConcreteClass destructor");
    }

    void pureFunc() override {
        std::println("  ConcreteClass::pureFunc()");
    }
};

void demonstratePureVirtual() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 6: PURE VIRTUAL           ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    // AbstractBase obj;  // ❌ Error! Abstract class

    std::println("--- Creating concrete object ---");
    AbstractBase* ptr = new ConcreteClass();

    std::println("");
    std::println("--- Deleting ---");
    delete ptr;
}

// ============================================
// EXAMPLE 7: PROTECTED DESTRUCTOR
// ============================================

class NonPolymorphicBase {
protected:
    // Protected non-virtual destructor
    ~NonPolymorphicBase() {
        std::println("  NonPolymorphicBase destructor");
    }

public:
    void doSomething() {
        std::println("  Doing something");
    }
};

class NonPolymorphicDerived : public NonPolymorphicBase {
public:
    ~NonPolymorphicDerived() {
        std::println("  NonPolymorphicDerived destructor");
    }
};

void demonstrateProtectedDestructor() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 7: PROTECTED DESTRUCTOR   ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Protected destructor prevents polymorphic deletion ---");

    // NonPolymorphicBase* ptr = new NonPolymorphicDerived();
    // delete ptr;  // ❌ Error! Destructor is protected

    std::println("--- Can only delete derived directly ---");
    NonPolymorphicDerived* ptr = new NonPolymorphicDerived();
    delete ptr;  // ✅ OK
}

// ============================================
// EXAMPLE 8: MEMORY LEAK DEMONSTRATION
// ============================================

class Resource {
private:
    string name;
    int* data;
    static int leakCount;

public:
    Resource(string n) : name(n) {
        data = new int[1000];  // Allocate significant memory
        leakCount++;
        std::println("  Resource '{}' created (leak count: {})", name, leakCount);
    }

    ~Resource() {
        delete[] data;
        leakCount--;
        std::println("  Resource '{}' destroyed (leak count: {})", name, leakCount);
    }

    static int getLeakCount() {
        return leakCount;
    }
};

int Resource::leakCount = 0;

class BaseResource {
public:
    Resource* resource;

    BaseResource(string name) {
        resource = new Resource(name + "-Base");
    }

    // NOT virtual - problem!
    ~BaseResource() {
        delete resource;
        std::println("  BaseResource destructor");
    }

    virtual void use() {
        std::println("  Using base resource");
    }
};

class DerivedResource : public BaseResource {
private:
    Resource* derivedResource;

public:
    DerivedResource(string name) : BaseResource(name) {
        derivedResource = new Resource(name + "-Derived");
    }

    ~DerivedResource() {
        delete derivedResource;
        std::println("  DerivedResource destructor");
    }

    void use() override {
        std::println("  Using derived resource");
    }
};

class BaseResourceFixed {
public:
    Resource* resource;

    BaseResourceFixed(string name) {
        resource = new Resource(name + "-Base");
    }

    // Virtual - fixed!
    virtual ~BaseResourceFixed() {
        delete resource;
        std::println("  BaseResourceFixed destructor");
    }

    virtual void use() {
        std::println("  Using base resource");
    }
};

class DerivedResourceFixed : public BaseResourceFixed {
private:
    Resource* derivedResource;

public:
    DerivedResourceFixed(string name) : BaseResourceFixed(name) {
        derivedResource = new Resource(name + "-Derived");
    }

    ~DerivedResourceFixed() {
        delete derivedResource;
        std::println("  DerivedResourceFixed destructor");
    }

    void use() override {
        std::println("  Using derived resource");
    }
};

void demonstrateMemoryLeak() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 8: MEMORY LEAK DEMO       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- WITHOUT virtual destructor (LEAK!) ---");
    {
        BaseResource* ptr = new DerivedResource("Test1");
        delete ptr;  // Leaks derived resource!
        std::println("  Active leaks: {} 💥", Resource::getLeakCount());
    }

    std::println("");
    std::println("--- WITH virtual destructor (NO LEAK) ---");
    {
        BaseResourceFixed* ptr = new DerivedResourceFixed("Test2");
        delete ptr;  // Properly cleans up!
        std::println("  Active leaks: {} ✅", Resource::getLeakCount());
    }
}

// ============================================
// EXAMPLE 9: SMART POINTERS (Modern C++)
// ============================================

class ModernBase {
public:
    ModernBase() {
        std::println("  ModernBase constructor");
    }

    virtual ~ModernBase() {
        std::println("  ModernBase destructor");
    }

    virtual void display() {
        std::println("  ModernBase::display()");
    }
};

class ModernDerived : public ModernBase {
private:
    int* data;

public:
    ModernDerived() {
        data = new int[100];
        std::println("  ModernDerived constructor");
    }

    ~ModernDerived() {
        delete[] data;
        std::println("  ModernDerived destructor");
    }

    void display() override {
        std::println("  ModernDerived::display()");
    }
};

void demonstrateSmartPointers() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 9: SMART POINTERS         ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Using unique_ptr (automatic cleanup) ---");
    {
        unique_ptr<ModernBase> ptr = make_unique<ModernDerived>();
        ptr->display();
        std::println("  No explicit delete needed!");
    }  // Automatically calls destructors here

    std::println("");
    std::println("--- Using shared_ptr ---");
    {
        shared_ptr<ModernBase> ptr1 = make_shared<ModernDerived>();
        {
            shared_ptr<ModernBase> ptr2 = ptr1;  // Shared ownership
            std::println("  Reference count: 2");
        }
        std::println("  Reference count: 1");
    }  // Automatically calls destructors when count reaches 0
}

// ============================================
// EXAMPLE 10: REAL-WORLD - POLYMORPHIC COLLECTION
// ============================================

class Shape {
protected:
    string color;

public:
    Shape(string c) : color(c) {
        std::println("  Shape constructor: {}", color);
    }

    virtual ~Shape() {
        std::println("  Shape destructor: {}", color);
    }

    virtual double area() const = 0;
    virtual void draw() const = 0;

    string getColor() const {
        return color;
    }
};

class Circle : public Shape {
private:
    double radius;
    int* renderData;  // Simulated resource

public:
    Circle(string c, double r) : Shape(c), radius(r) {
        renderData = new int[100];
        std::println("  Circle constructor: radius {}", radius);
    }

    ~Circle() {
        delete[] renderData;
        std::println("  Circle destructor: radius {}", radius);
    }

    double area() const override {
        return 3.14159 * radius * radius;
    }

    void draw() const override {
        std::println("  Drawing {} circle, radius {}", color, radius);
    }
};

class Rectangle : public Shape {
private:
    double width, height;
    int* renderData;  // Simulated resource

public:
    Rectangle(string c, double w, double h)
        : Shape(c), width(w), height(h) {
        renderData = new int[100];
        std::println("  Rectangle constructor: {}×{}", width, height);
    }

    ~Rectangle() {
        delete[] renderData;
        std::println("  Rectangle destructor: {}×{}", width, height);
    }

    double area() const override {
        return width * height;
    }

    void draw() const override {
        std::println("  Drawing {} rectangle: {}×{}", color, width, height);
    }
};

void demonstratePolymorphicCollection() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 10: POLYMORPHIC COLLECTION║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating polymorphic collection ---");
    vector<unique_ptr<Shape>> shapes;
    shapes.push_back(make_unique<Circle>("red", 5.0));
    shapes.push_back(make_unique<Rectangle>("blue", 10.0, 5.0));
    shapes.push_back(make_unique<Circle>("green", 3.0));

    std::println("");
    std::println("--- Using shapes polymorphically ---");
    for (const auto& shape : shapes) {
        shape->draw();
        std::println("  Area: {:g}", shape->area());
        std::println("");
    }

    std::println("--- Automatic cleanup (virtual destructors) ---");
    shapes.clear();  // All destructors called properly!
}

// ============================================
// MAIN DEMONSTRATION
// ============================================

int main() {
    std::println("========================================");
    std::println("VIRTUAL DESTRUCTOR DEMONSTRATION");
    std::println("========================================");

    demonstrateWithoutVirtual();
    demonstrateWithVirtual();
    demonstrateComparison();
    demonstrateDestructionOrder();
    demonstrateDefaultVirtual();
    demonstratePureVirtual();
    demonstrateProtectedDestructor();
    demonstrateMemoryLeak();
    demonstrateSmartPointers();
    demonstratePolymorphicCollection();

    return 0;
}
