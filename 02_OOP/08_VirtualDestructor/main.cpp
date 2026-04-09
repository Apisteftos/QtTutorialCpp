#include <iostream>
#include <string>
#include <vector>
#include <memory>
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
        cout << "  BaseNoVirtual constructor\n";
    }

    // NOT virtual - THIS IS THE PROBLEM!
    ~BaseNoVirtual() {
        cout << "  BaseNoVirtual destructor\n";
    }

    virtual void display() {
        cout << "  BaseNoVirtual::display()\n";
    }
};

class DerivedNoVirtual : public BaseNoVirtual {
private:
    int* data;  // Dynamically allocated

public:
    DerivedNoVirtual() {
        data = new int[100];
        cout << "  DerivedNoVirtual constructor (allocated 100 ints)\n";
    }

    ~DerivedNoVirtual() {
        delete[] data;
        cout << "  DerivedNoVirtual destructor (freed memory)\n";
    }

    void display() override {
        cout << "  DerivedNoVirtual::display()\n";
    }
};

void demonstrateWithoutVirtual() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: WITHOUT VIRTUAL        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating DerivedNoVirtual object ---\n";
    BaseNoVirtual* ptr = new DerivedNoVirtual();

    cout << "\n--- Using polymorphism (works) ---\n";
    ptr->display();  // Calls DerivedNoVirtual::display() ✅

    cout << "\n--- Deleting via base pointer ---\n";
    delete ptr;  // ⚠️ ONLY calls BaseNoVirtual destructor!

    cout << "\n⚠️  CRITICAL PROBLEM:\n";
    cout << "   • Only BaseNoVirtual destructor was called!\n";
    cout << "   • DerivedNoVirtual destructor was NOT called!\n";
    cout << "   • Memory allocated in derived class NOT freed!\n";
    cout << "   • MEMORY LEAK! 💥\n";
    cout << "   • Undefined behavior!\n";
}

// ============================================
// EXAMPLE 2: WITH VIRTUAL DESTRUCTOR (The Solution)
// ============================================

class BaseWithVirtual {
public:
    BaseWithVirtual() {
        cout << "  BaseWithVirtual constructor\n";
    }

    // VIRTUAL destructor - THIS IS THE SOLUTION!
    virtual ~BaseWithVirtual() {
        cout << "  BaseWithVirtual destructor\n";
    }

    virtual void display() {
        cout << "  BaseWithVirtual::display()\n";
    }
};

class DerivedWithVirtual : public BaseWithVirtual {
private:
    int* data;  // Dynamically allocated

public:
    DerivedWithVirtual() {
        data = new int[100];
        cout << "  DerivedWithVirtual constructor (allocated 100 ints)\n";
    }

    ~DerivedWithVirtual() {
        delete[] data;
        cout << "  DerivedWithVirtual destructor (freed memory)\n";
    }

    void display() override {
        cout << "  DerivedWithVirtual::display()\n";
    }
};

void demonstrateWithVirtual() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: WITH VIRTUAL           ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating DerivedWithVirtual object ---\n";
    BaseWithVirtual* ptr = new DerivedWithVirtual();

    cout << "\n--- Using polymorphism (works) ---\n";
    ptr->display();  // Calls DerivedWithVirtual::display() ✅

    cout << "\n--- Deleting via base pointer ---\n";
    delete ptr;  // ✅ Calls BOTH destructors!

    cout << "\n✅ CORRECT BEHAVIOR:\n";
    cout << "   • DerivedWithVirtual destructor called first!\n";
    cout << "   • BaseWithVirtual destructor called second!\n";
    cout << "   • Memory properly freed!\n";
    cout << "   • No memory leak! ✅\n";
    cout << "   • Proper cleanup!\n";
}

// ============================================
// EXAMPLE 3: COMPARISON SIDE-BY-SIDE
// ============================================

void demonstrateComparison() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: SIDE-BY-SIDE           ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- WITHOUT virtual destructor ---\n";
    {
        BaseNoVirtual* ptr1 = new DerivedNoVirtual();
        delete ptr1;  // Only base destructor called 💥
    }

    cout << "\n--- WITH virtual destructor ---\n";
    {
        BaseWithVirtual* ptr2 = new DerivedWithVirtual();
        delete ptr2;  // Both destructors called ✅
    }

    cout << "\n--- Direct object (no pointer) ---\n";
    {
        cout << "Creating DerivedNoVirtual directly:\n";
        DerivedNoVirtual obj1;
        cout << "Destroying (both destructors called even without virtual):\n";
    }
    cout << "\n";

    cout << "💡 Key insight:\n";
    cout << "   • Direct objects: Both destructors called (no polymorphism)\n";
    cout << "   • Base pointer: Need virtual destructor for proper cleanup\n";
}

// ============================================
// EXAMPLE 4: DESTRUCTION ORDER
// ============================================

class GrandParent {
public:
    GrandParent() {
        cout << "  1. GrandParent constructor\n";
    }

    virtual ~GrandParent() {
        cout << "  3. GrandParent destructor\n";
    }
};

class Parent : public GrandParent {
public:
    Parent() {
        cout << "  2. Parent constructor\n";
    }

    ~Parent() {
        cout << "  2. Parent destructor\n";
    }
};

class Child : public Parent {
public:
    Child() {
        cout << "  3. Child constructor\n";
    }

    ~Child() {
        cout << "  1. Child destructor\n";
    }
};

void demonstrateDestructionOrder() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: DESTRUCTION ORDER      ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Construction order (base → derived) ---\n";
    GrandParent* ptr = new Child();

    cout << "\n--- Destruction order (derived → base) ---\n";
    delete ptr;

    cout << "\n✅ Destruction order (reverse of construction):\n";
    cout << "   Construction: GrandParent → Parent → Child\n";
    cout << "   Destruction:  Child → Parent → GrandParent\n";
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
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: DEFAULT VIRTUAL        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Using = default for virtual destructor ---\n";
    SimpleBase* ptr = new SimpleDerived();
    delete ptr;

    cout << "\n✅ '= default' syntax:\n";
    cout << "   • virtual ~ClassName() = default;\n";
    cout << "   • Compiler generates destructor\n";
    cout << "   • Still virtual!\n";
    cout << "   • Clean and efficient\n";
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
    cout << "  AbstractBase destructor (pure virtual)\n";
}

class ConcreteClass : public AbstractBase {
public:
    ~ConcreteClass() {
        cout << "  ConcreteClass destructor\n";
    }

    void pureFunc() override {
        cout << "  ConcreteClass::pureFunc()\n";
    }
};

void demonstratePureVirtual() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: PURE VIRTUAL           ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    // AbstractBase obj;  // ❌ Error! Abstract class

    cout << "--- Creating concrete object ---\n";
    AbstractBase* ptr = new ConcreteClass();

    cout << "\n--- Deleting ---\n";
    delete ptr;

    cout << "\n💡 Pure virtual destructor:\n";
    cout << "   • Makes class abstract\n";
    cout << "   • MUST provide implementation\n";
    cout << "   • Still called during destruction\n";
    cout << "   • Rare but valid pattern\n";
}

// ============================================
// EXAMPLE 7: PROTECTED DESTRUCTOR
// ============================================

class NonPolymorphicBase {
protected:
    // Protected non-virtual destructor
    ~NonPolymorphicBase() {
        cout << "  NonPolymorphicBase destructor\n";
    }

public:
    void doSomething() {
        cout << "  Doing something\n";
    }
};

class NonPolymorphicDerived : public NonPolymorphicBase {
public:
    ~NonPolymorphicDerived() {
        cout << "  NonPolymorphicDerived destructor\n";
    }
};

void demonstrateProtectedDestructor() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 7: PROTECTED DESTRUCTOR   ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Protected destructor prevents polymorphic deletion ---\n";

    // NonPolymorphicBase* ptr = new NonPolymorphicDerived();
    // delete ptr;  // ❌ Error! Destructor is protected

    cout << "--- Can only delete derived directly ---\n";
    NonPolymorphicDerived* ptr = new NonPolymorphicDerived();
    delete ptr;  // ✅ OK

    cout << "\n💡 Protected destructor:\n";
    cout << "   • Prevents deletion via base pointer\n";
    cout << "   • No need for virtual (can't be used polymorphically)\n";
    cout << "   • Forces proper usage\n";
    cout << "   • Prevents accidental misuse\n";
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
        cout << "  Resource '" << name << "' created (leak count: "
             << leakCount << ")\n";
    }

    ~Resource() {
        delete[] data;
        leakCount--;
        cout << "  Resource '" << name << "' destroyed (leak count: "
             << leakCount << ")\n";
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
        cout << "  BaseResource destructor\n";
    }

    virtual void use() {
        cout << "  Using base resource\n";
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
        cout << "  DerivedResource destructor\n";
    }

    void use() override {
        cout << "  Using derived resource\n";
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
        cout << "  BaseResourceFixed destructor\n";
    }

    virtual void use() {
        cout << "  Using base resource\n";
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
        cout << "  DerivedResourceFixed destructor\n";
    }

    void use() override {
        cout << "  Using derived resource\n";
    }
};

void demonstrateMemoryLeak() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 8: MEMORY LEAK DEMO       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- WITHOUT virtual destructor (LEAK!) ---\n";
    {
        BaseResource* ptr = new DerivedResource("Test1");
        delete ptr;  // Leaks derived resource!
        cout << "  Active leaks: " << Resource::getLeakCount() << " 💥\n";
    }

    cout << "\n--- WITH virtual destructor (NO LEAK) ---\n";
    {
        BaseResourceFixed* ptr = new DerivedResourceFixed("Test2");
        delete ptr;  // Properly cleans up!
        cout << "  Active leaks: " << Resource::getLeakCount() << " ✅\n";
    }

    cout << "\n⚠️  Memory leak consequences:\n";
    cout << "   • Wasted memory\n";
    cout << "   • Program slowdown over time\n";
    cout << "   • Eventually crashes\n";
    cout << "   • Hard to debug\n";
    cout << "   • FIX: Always use virtual destructor!\n";
}

// ============================================
// EXAMPLE 9: SMART POINTERS (Modern C++)
// ============================================

class ModernBase {
public:
    ModernBase() {
        cout << "  ModernBase constructor\n";
    }

    virtual ~ModernBase() {
        cout << "  ModernBase destructor\n";
    }

    virtual void display() {
        cout << "  ModernBase::display()\n";
    }
};

class ModernDerived : public ModernBase {
private:
    int* data;

public:
    ModernDerived() {
        data = new int[100];
        cout << "  ModernDerived constructor\n";
    }

    ~ModernDerived() {
        delete[] data;
        cout << "  ModernDerived destructor\n";
    }

    void display() override {
        cout << "  ModernDerived::display()\n";
    }
};

void demonstrateSmartPointers() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 9: SMART POINTERS         ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Using unique_ptr (automatic cleanup) ---\n";
    {
        unique_ptr<ModernBase> ptr = make_unique<ModernDerived>();
        ptr->display();
        cout << "  No explicit delete needed!\n";
    }  // Automatically calls destructors here

    cout << "\n--- Using shared_ptr ---\n";
    {
        shared_ptr<ModernBase> ptr1 = make_shared<ModernDerived>();
        {
            shared_ptr<ModernBase> ptr2 = ptr1;  // Shared ownership
            cout << "  Reference count: 2\n";
        }
        cout << "  Reference count: 1\n";
    }  // Automatically calls destructors when count reaches 0

    cout << "\n✅ Smart pointers:\n";
    cout << "   • Automatic memory management\n";
    cout << "   • No explicit delete needed\n";
    cout << "   • Still need virtual destructor!\n";
    cout << "   • Exception safe\n";
    cout << "   • Modern C++ best practice\n";
}

// ============================================
// EXAMPLE 10: REAL-WORLD - POLYMORPHIC COLLECTION
// ============================================

class Shape {
protected:
    string color;

public:
    Shape(string c) : color(c) {
        cout << "  Shape constructor: " << color << "\n";
    }

    virtual ~Shape() {
        cout << "  Shape destructor: " << color << "\n";
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
        cout << "  Circle constructor: radius " << radius << "\n";
    }

    ~Circle() {
        delete[] renderData;
        cout << "  Circle destructor: radius " << radius << "\n";
    }

    double area() const override {
        return 3.14159 * radius * radius;
    }

    void draw() const override {
        cout << "  Drawing " << color << " circle, radius " << radius << "\n";
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
        cout << "  Rectangle constructor: " << width << "×" << height << "\n";
    }

    ~Rectangle() {
        delete[] renderData;
        cout << "  Rectangle destructor: " << width << "×" << height << "\n";
    }

    double area() const override {
        return width * height;
    }

    void draw() const override {
        cout << "  Drawing " << color << " rectangle: "
             << width << "×" << height << "\n";
    }
};

void demonstratePolymorphicCollection() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 10: POLYMORPHIC COLLECTION║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating polymorphic collection ---\n";
    vector<unique_ptr<Shape>> shapes;
    shapes.push_back(make_unique<Circle>("red", 5.0));
    shapes.push_back(make_unique<Rectangle>("blue", 10.0, 5.0));
    shapes.push_back(make_unique<Circle>("green", 3.0));

    cout << "\n--- Using shapes polymorphically ---\n";
    for (const auto& shape : shapes) {
        shape->draw();
        cout << "  Area: " << shape->area() << "\n\n";
    }

    cout << "--- Automatic cleanup (virtual destructors) ---\n";
    shapes.clear();  // All destructors called properly!

    cout << "\n✅ Virtual destructor enables:\n";
    cout << "   • Polymorphic collections\n";
    cout << "   • Proper cleanup of derived classes\n";
    cout << "   • Safe use with smart pointers\n";
    cout << "   • Memory leak prevention\n";
}

// ============================================
// MAIN DEMONSTRATION
// ============================================

int main() {
    cout << "========================================\n";
    cout << "VIRTUAL DESTRUCTOR DEMONSTRATION\n";
    cout << "========================================\n";

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

    // ============================================
    // Comprehensive Summary
    // ============================================
    cout << "\n========================================\n";
    cout << "WHAT ARE VIRTUAL DESTRUCTORS?\n";
    cout << "========================================\n";
    cout << "Virtual destructors are destructors declared with\n";
    cout << "the 'virtual' keyword. They ensure proper cleanup\n";
    cout << "in inheritance hierarchies when deleting via base\n";
    cout << "class pointers.\n";
    cout << "\n";
    cout << "Critical for:\n";
    cout << "  • Polymorphic base classes\n";
    cout << "  • Preventing memory leaks\n";
    cout << "  • Proper resource cleanup\n";
    cout << "  • Avoiding undefined behavior\n";

    cout << "\n========================================\n";
    cout << "THE PROBLEM\n";
    cout << "========================================\n";
    cout << "WITHOUT virtual destructor:\n";
    cout << "\n";
    cout << "class Base {\n";
    cout << "    ~Base() { }  // ❌ NOT virtual\n";
    cout << "};\n";
    cout << "\n";
    cout << "class Derived : public Base {\n";
    cout << "    int* data;\n";
    cout << "    Derived() { data = new int[100]; }\n";
    cout << "    ~Derived() { delete[] data; }\n";
    cout << "};\n";
    cout << "\n";
    cout << "Base* ptr = new Derived();\n";
    cout << "delete ptr;  // ⚠️ ONLY calls Base destructor!\n";
    cout << "             // Derived destructor NOT called!\n";
    cout << "             // MEMORY LEAK! 💥\n";
    cout << "\n";
    cout << "Problems:\n";
    cout << "  ❌ Only base destructor called\n";
    cout << "  ❌ Derived destructor NOT called\n";
    cout << "  ❌ Memory not freed\n";
    cout << "  ❌ Resources not released\n";
    cout << "  ❌ Undefined behavior\n";
    cout << "  ❌ MEMORY LEAK!\n";

    cout << "\n========================================\n";
    cout << "THE SOLUTION\n";
    cout << "========================================\n";
    cout << "WITH virtual destructor:\n";
    cout << "\n";
    cout << "class Base {\n";
    cout << "    virtual ~Base() { }  // ✅ Virtual!\n";
    cout << "};\n";
    cout << "\n";
    cout << "class Derived : public Base {\n";
    cout << "    int* data;\n";
    cout << "    Derived() { data = new int[100]; }\n";
    cout << "    ~Derived() { delete[] data; }\n";
    cout << "};\n";
    cout << "\n";
    cout << "Base* ptr = new Derived();\n";
    cout << "delete ptr;  // ✅ Calls BOTH destructors!\n";
    cout << "             // 1. Derived destructor first\n";
    cout << "             // 2. Base destructor second\n";
    cout << "             // Memory properly freed! ✅\n";
    cout << "\n";
    cout << "Benefits:\n";
    cout << "  ✅ Both destructors called\n";
    cout << "  ✅ Memory properly freed\n";
    cout << "  ✅ Resources properly released\n";
    cout << "  ✅ No memory leak\n";
    cout << "  ✅ Defined behavior\n";
    cout << "  ✅ Safe polymorphism\n";

    cout << "\n========================================\n";
    cout << "SYNTAX\n";
    cout << "========================================\n";
    cout << "BASIC VIRTUAL DESTRUCTOR:\n";
    cout << "class Base {\n";
    cout << "public:\n";
    cout << "    virtual ~Base() {\n";
    cout << "        // Cleanup code\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "DEFAULT VIRTUAL DESTRUCTOR (C++11):\n";
    cout << "class Base {\n";
    cout << "public:\n";
    cout << "    virtual ~Base() = default;\n";
    cout << "};\n";
    cout << "\n";
    cout << "PURE VIRTUAL DESTRUCTOR (rare):\n";
    cout << "class Abstract {\n";
    cout << "public:\n";
    cout << "    virtual ~Abstract() = 0;\n";
    cout << "};\n";
    cout << "\n";
    cout << "// MUST provide implementation\n";
    cout << "Abstract::~Abstract() { }\n";

    cout << "\n========================================\n";
    cout << "WHEN TO USE VIRTUAL DESTRUCTOR\n";
    cout << "========================================\n";
    cout << "✅ ALWAYS USE when:\n";
    cout << "  • Class has ANY virtual function\n";
    cout << "  • Class is meant to be a base class\n";
    cout << "  • Class is polymorphic\n";
    cout << "  • Objects deleted via base pointer\n";
    cout << "  • Using inheritance hierarchies\n";
    cout << "\n";
    cout << "❌ NOT NEEDED when:\n";
    cout << "  • Final class (no derivation)\n";
    cout << "  • No virtual functions\n";
    cout << "  • Never deleted polymorphically\n";
    cout << "  • Protected destructor used\n";
    cout << "\n";
    cout << "⚠️  GOLDEN RULE:\n";
    cout << "   If a class has ANY virtual function,\n";
    cout << "   destructor MUST be virtual!\n";

    cout << "\n========================================\n";
    cout << "DESTRUCTION ORDER\n";
    cout << "========================================\n";
    cout << "With virtual destructor:\n";
    cout << "\n";
    cout << "class GrandParent {\n";
    cout << "    virtual ~GrandParent() { }\n";
    cout << "};\n";
    cout << "\n";
    cout << "class Parent : public GrandParent {\n";
    cout << "    ~Parent() { }\n";
    cout << "};\n";
    cout << "\n";
    cout << "class Child : public Parent {\n";
    cout << "    ~Child() { }\n";
    cout << "};\n";
    cout << "\n";
    cout << "GrandParent* ptr = new Child();\n";
    cout << "delete ptr;\n";
    cout << "\n";
    cout << "Destruction order (derived to base):\n";
    cout << "  1. ~Child()        ← Most derived first\n";
    cout << "  2. ~Parent()\n";
    cout << "  3. ~GrandParent()  ← Base last\n";
    cout << "\n";
    cout << "This is REVERSE of construction order!\n";

    cout << "\n========================================\n";
    cout << "SPECIAL CASES\n";
    cout << "========================================\n";
    cout << "1. DEFAULT VIRTUAL DESTRUCTOR:\n";
    cout << "   virtual ~ClassName() = default;\n";
    cout << "   • Compiler generates destructor\n";
    cout << "   • Still virtual\n";
    cout << "   • Clean and efficient\n";
    cout << "\n";
    cout << "2. PURE VIRTUAL DESTRUCTOR:\n";
    cout << "   virtual ~ClassName() = 0;\n";
    cout << "   • Makes class abstract\n";
    cout << "   • MUST provide implementation\n";
    cout << "   • Still called during destruction\n";
    cout << "   • Rare pattern\n";
    cout << "\n";
    cout << "3. PROTECTED NON-VIRTUAL DESTRUCTOR:\n";
    cout << "protected:\n";
    cout << "   ~ClassName() { }\n";
    cout << "   • Prevents polymorphic deletion\n";
    cout << "   • No need for virtual\n";
    cout << "   • Forces proper usage\n";
    cout << "   • Can't delete via base pointer\n";

    cout << "\n========================================\n";
    cout << "SMART POINTERS\n";
    cout << "========================================\n";
    cout << "Smart pointers STILL require virtual destructor!\n";
    cout << "\n";
    cout << "class Base {\n";
    cout << "    virtual ~Base() = default;  // Still needed!\n";
    cout << "};\n";
    cout << "\n";
    cout << "class Derived : public Base { };\n";
    cout << "\n";
    cout << "// unique_ptr\n";
    cout << "unique_ptr<Base> ptr = make_unique<Derived>();\n";
    cout << "// Automatically calls destructors when ptr goes out of scope\n";
    cout << "\n";
    cout << "// shared_ptr\n";
    cout << "shared_ptr<Base> ptr = make_shared<Derived>();\n";
    cout << "// Automatically calls destructors when ref count reaches 0\n";
    cout << "\n";
    cout << "Benefits:\n";
    cout << "  ✅ Automatic memory management\n";
    cout << "  ✅ No explicit delete needed\n";
    cout << "  ✅ Exception safe\n";
    cout << "  ✅ Still need virtual destructor!\n";

    cout << "\n========================================\n";
    cout << "BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "✅ DO:\n";
    cout << "  • Make destructor virtual in polymorphic base\n";
    cout << "  • Use '= default' when no cleanup needed\n";
    cout << "  • Use smart pointers (unique_ptr, shared_ptr)\n";
    cout << "  • Always test polymorphic deletion\n";
    cout << "  • Document inheritance relationships\n";
    cout << "  • Check for memory leaks\n";
    cout << "\n";
    cout << "❌ DON'T:\n";
    cout << "  • Forget virtual when class has virtual functions\n";
    cout << "  • Call virtual functions in destructors\n";
    cout << "  • Assume non-virtual is OK\n";
    cout << "  • Ignore memory leak warnings\n";
    cout << "  • Skip destructor for 'simple' classes\n";
    cout << "  • Delete polymorphic objects without virtual\n";

    cout << "\n========================================\n";
    cout << "COMMON MISTAKES\n";
    cout << "========================================\n";
    cout << "❌ Mistake 1: Forgetting virtual\n";
    cout << "class Base {\n";
    cout << "    ~Base() { }  // ❌ Not virtual\n";
    cout << "    virtual void func() { }\n";
    cout << "};\n";
    cout << "Fix: virtual ~Base() { }\n";
    cout << "\n";
    cout << "❌ Mistake 2: Calling virtual functions in destructor\n";
    cout << "class Base {\n";
    cout << "    virtual ~Base() {\n";
    cout << "        virtualFunc();  // ❌ Dangerous!\n";
    cout << "    }\n";
    cout << "    virtual void virtualFunc() { }\n";
    cout << "};\n";
    cout << "Problem: Derived part already destroyed!\n";
    cout << "\n";
    cout << "❌ Mistake 3: Not providing pure virtual implementation\n";
    cout << "class Abstract {\n";
    cout << "    virtual ~Abstract() = 0;\n";
    cout << "};\n";
    cout << "// Missing: Abstract::~Abstract() { }\n";
    cout << "Fix: MUST provide implementation!\n";

    cout << "\n========================================\n";
    cout << "PERFORMANCE\n";
    cout << "========================================\n";
    cout << "Virtual destructor cost:\n";
    cout << "  • Space: One vptr per object (~8 bytes)\n";
    cout << "  • Time: One extra indirection\n";
    cout << "  • Usually negligible\n";
    cout << "\n";
    cout << "When to avoid:\n";
    cout << "  • Performance-critical tight loops\n";
    cout << "  • Final classes (use 'final' keyword)\n";
    cout << "  • No inheritance hierarchy\n";
    cout << "\n";
    cout << "General rule:\n";
    cout << "  • Correctness > Performance\n";
    cout << "  • Memory leaks are worse than tiny overhead\n";
    cout << "  • Always use virtual for polymorphic classes\n";

    cout << "\n========================================\n";
    cout << "QUICK REFERENCE\n";
    cout << "========================================\n";
    cout << "// Basic virtual destructor\n";
    cout << "class Base {\n";
    cout << "public:\n";
    cout << "    virtual ~Base() {\n";
    cout << "        // Cleanup\n";
    cout << "    }\n";
    cout << "    virtual void func() { }\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Default virtual destructor\n";
    cout << "class Base {\n";
    cout << "public:\n";
    cout << "    virtual ~Base() = default;\n";
    cout << "    virtual void func() { }\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Derived class (implicit virtual)\n";
    cout << "class Derived : public Base {\n";
    cout << "    ~Derived() {  // Implicitly virtual\n";
    cout << "        // Cleanup\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Usage with smart pointers\n";
    cout << "unique_ptr<Base> ptr = make_unique<Derived>();\n";
    cout << "// Automatically calls destructors\n";

    cout << "\n========================================\n";
    cout << "KEY TAKEAWAYS\n";
    cout << "========================================\n";
    cout << "1. Virtual destructor ensures proper cleanup\n";
    cout << "2. CRITICAL for polymorphic base classes\n";
    cout << "3. Prevents memory leaks and undefined behavior\n";
    cout << "4. Destruction order: derived → base (reverse)\n";
    cout << "5. Use '= default' when no cleanup needed\n";
    cout << "6. Smart pointers still need virtual destructor\n";
    cout << "7. If ANY virtual function → destructor must be virtual\n";
    cout << "8. Protected non-virtual prevents polymorphic deletion\n";
    cout << "9. Pure virtual destructor needs implementation\n";
    cout << "10. ALWAYS test polymorphic deletion!\n";

    cout << "\n========================================\n";
    cout << "GOLDEN RULE\n";
    cout << "========================================\n";
    cout << "⚠️  If a class has ANY virtual function,\n";
    cout << "   its destructor MUST be virtual!\n";
    cout << "\n";
    cout << "   virtual ~ClassName() = default;\n";
    cout << "\n";
    cout << "   This prevents memory leaks and ensures\n";
    cout << "   proper cleanup in inheritance hierarchies.\n";

    return 0;
}
