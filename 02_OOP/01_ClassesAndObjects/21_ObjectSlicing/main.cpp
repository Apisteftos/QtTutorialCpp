#include <iostream>
#include <string>
#include <vector>
#include <memory>
using namespace std;

// ============================================
// WHAT IS OBJECT SLICING?
// ============================================

/*
 * OBJECT SLICING:
 * - When derived object is assigned to base object
 * - Derived part is "sliced off"
 * - Only base part is copied
 * - Data loss occurs
 * - Polymorphic behavior is lost
 *
 * WHY IT HAPPENS:
 * - Passing derived by value to base parameter
 * - Assigning derived to base variable
 * - Storing derived in base container
 * - Copy construction from derived to base
 *
 * THE PROBLEM:
 * - Silent data loss
 * - Incorrect behavior
 * - No compiler warning (usually)
 * - Hard to detect bugs
 * - Loss of polymorphism
 *
 * HOW TO PREVENT:
 * - Use pointers (raw or smart)
 * - Use references
 * - Delete copy operations
 * - Make base class abstract
 * - Use containers of pointers
 *
 * BEST PRACTICES:
 * - Never pass polymorphic objects by value
 * - Always use pointers/references for polymorphism
 * - Store pointers in containers
 * - Consider abstract base classes
 * - Document non-copyable classes
 */

// ============================================
// EXAMPLE 1: THE PROBLEM - OBJECT SLICING
// ============================================

class Animal {
protected:
    string name;

public:
    Animal(const string& n) : name(n) {
        cout << "  Animal constructor: " << name << "\n";
    }

    virtual void speak() const {
        cout << "  " << name << " makes a sound\n";
    }

    virtual void display() const {
        cout << "  Animal: " << name << "\n";
    }

    virtual ~Animal() {
        cout << "  Animal destructor: " << name << "\n";
    }
};

class Dog : public Animal {
private:
    string breed;

public:
    Dog(const string& n, const string& b)
        : Animal(n), breed(b) {
        cout << "  Dog constructor: " << name << " (" << breed << ")\n";
    }

    void speak() const override {
        cout << "  " << name << " barks: Woof! Woof!\n";
    }

    void display() const override {
        cout << "  Dog: " << name << " (Breed: " << breed << ")\n";
    }

    ~Dog() {
        cout << "  Dog destructor: " << name << "\n";
    }
};

void demonstrateProblem() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: THE PROBLEM           ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "--- Creating a Dog object ---\n";
    Dog dog("Buddy", "Golden Retriever");
    dog.display();
    dog.speak();

    cout << "\n--- Object Slicing: Assigning Dog to Animal ---\n";
    Animal animal = dog;  // ⚠️ SLICING HAPPENS HERE!
    animal.display();     // ⚠️ Calls Animal::display, not Dog::display
    animal.speak();       // ⚠️ Calls Animal::speak, not Dog::speak

    cout << "\n⚠️  WHAT HAPPENED:\n";
    cout << "   • Dog object was sliced\n";
    cout << "   • breed member was lost\n";
    cout << "   • Polymorphism is lost\n";
    cout << "   • Only base part copied\n";
    cout << "   • Incorrect behavior!\n";
}

// ============================================
// EXAMPLE 2: SLICING IN FUNCTION CALLS
// ============================================

void processAnimalByValue(Animal a) {  // ⚠️ Pass by value - causes slicing!
    cout << "  Processing animal:\n";
    a.display();
    a.speak();
}

void processAnimalByReference(const Animal& a) {  // ✅ Pass by reference - no slicing
    cout << "  Processing animal:\n";
    a.display();
    a.speak();
}

void processAnimalByPointer(const Animal* a) {  // ✅ Pass by pointer - no slicing
    cout << "  Processing animal:\n";
    a->display();
    a->speak();
}

void demonstrateFunctionSlicing() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: FUNCTION SLICING      ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    Dog dog("Max", "German Shepherd");

    cout << "--- Pass by value (SLICING!) ---\n";
    processAnimalByValue(dog);  // ⚠️ Dog is sliced to Animal

    cout << "\n--- Pass by reference (NO slicing) ---\n";
    processAnimalByReference(dog);  // ✅ Polymorphism works!

    cout << "\n--- Pass by pointer (NO slicing) ---\n";
    processAnimalByPointer(&dog);  // ✅ Polymorphism works!

    cout << "\n💡 KEY LESSON:\n";
    cout << "   • NEVER pass polymorphic objects by value\n";
    cout << "   • ALWAYS use references or pointers\n";
    cout << "   • Pass by value loses derived information\n";
    cout << "   • Pass by reference/pointer preserves polymorphism\n";
}

// ============================================
// EXAMPLE 3: SLICING IN CONTAINERS
// ============================================

void demonstrateContainerSlicing() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: CONTAINER SLICING     ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    Dog dog1("Rocky", "Bulldog");
    Dog dog2("Luna", "Husky");

    cout << "--- BAD: vector of Animal (value) - SLICING! ---\n";
    vector<Animal> animals;  // ⚠️ Stores by value!
    animals.push_back(dog1);  // ⚠️ Dog is sliced!
    animals.push_back(dog2);  // ⚠️ Dog is sliced!

    cout << "Iterating over animals:\n";
    for (const auto& a : animals) {
        a.display();  // ⚠️ Only calls Animal::display
        a.speak();    // ⚠️ Only calls Animal::speak
    }

    cout << "\n--- GOOD: vector of pointers - NO slicing ---\n";
    vector<Animal*> animalPtrs;  // ✅ Stores pointers!
    animalPtrs.push_back(new Dog("Charlie", "Beagle"));
    animalPtrs.push_back(new Dog("Daisy", "Poodle"));

    cout << "Iterating over animal pointers:\n";
    for (const auto* a : animalPtrs) {
        a->display();  // ✅ Calls Dog::display
        a->speak();    // ✅ Calls Dog::speak
    }

    // Clean up
    for (auto* a : animalPtrs) {
        delete a;
    }

    cout << "\n--- BEST: vector of smart pointers ---\n";
    vector<unique_ptr<Animal>> smartPtrs;
    smartPtrs.push_back(make_unique<Dog>("Bella", "Labrador"));
    smartPtrs.push_back(make_unique<Dog>("Cooper", "Boxer"));

    cout << "Iterating over smart pointers:\n";
    for (const auto& a : smartPtrs) {
        a->display();  // ✅ Calls Dog::display
        a->speak();    // ✅ Calls Dog::speak
    }
    // Automatic cleanup!

    cout << "\n💡 CONTAINER RULES:\n";
    cout << "   • NEVER use vector<Base> for polymorphic objects\n";
    cout << "   • Use vector<Base*> (manual memory management)\n";
    cout << "   • BEST: vector<unique_ptr<Base>> (automatic cleanup)\n";
}

// ============================================
// EXAMPLE 4: COPY CONSTRUCTOR AND SLICING
// ============================================

class Shape {
protected:
    string color;

public:
    Shape(const string& c) : color(c) {
        cout << "  Shape(" << color << ")\n";
    }

    virtual double area() const {
        return 0.0;
    }

    virtual void display() const {
        cout << "  Shape, color: " << color << "\n";
    }

    virtual ~Shape() {
        cout << "  ~Shape()\n";
    }
};

class Circle : public Shape {
private:
    double radius;

public:
    Circle(const string& c, double r)
        : Shape(c), radius(r) {
        cout << "  Circle(radius=" << radius << ")\n";
    }

    double area() const override {
        return 3.14159 * radius * radius;
    }

    void display() const override {
        cout << "  Circle, color: " << color
             << ", radius: " << radius
             << ", area: " << area() << "\n";
    }

    ~Circle() {
        cout << "  ~Circle()\n";
    }
};

void demonstrateCopySlicing() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: COPY SLICING          ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "--- Creating Circle ---\n";
    Circle circle("Red", 5.0);
    circle.display();

    cout << "\n--- Copy to Shape (SLICING) ---\n";
    Shape shape = circle;  // ⚠️ Slicing!
    shape.display();       // ⚠️ Only Shape part
    cout << "  Area: " << shape.area() << "\n";  // ⚠️ Returns 0, not circle area!

    cout << "\n--- Copy via reference (NO slicing) ---\n";
    const Shape& shapeRef = circle;  // ✅ Reference
    shapeRef.display();              // ✅ Calls Circle::display
    cout << "  Area: " << shapeRef.area() << "\n";  // ✅ Correct area!

    cout << "\n💡 COPY OPERATIONS:\n";
    cout << "   • Copy construction can cause slicing\n";
    cout << "   • Assignment can cause slicing\n";
    cout << "   • Use references to avoid slicing\n";
    cout << "   • Consider making base non-copyable\n";
}

// ============================================
// EXAMPLE 5: PREVENTING SLICING - DELETE COPY
// ============================================

class Vehicle {
protected:
    string brand;

public:
    Vehicle(const string& b) : brand(b) {}

    // ✅ Delete copy operations to prevent slicing
    Vehicle(const Vehicle&) = delete;
    Vehicle& operator=(const Vehicle&) = delete;

    virtual void drive() const = 0;  // Pure virtual
    virtual ~Vehicle() = default;
};

class Car : public Vehicle {
private:
    int doors;

public:
    Car(const string& b, int d) : Vehicle(b), doors(d) {}

    void drive() const override {
        cout << "  Driving " << brand << " car with " << doors << " doors\n";
    }
};

void demonstratePreventSlicing() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: PREVENTING SLICING    ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    Car car("Toyota", 4);

    // Vehicle v = car;  // ❌ Compilation error! Copy deleted
    // Vehicle v2;       // ❌ Can't create Vehicle (abstract)

    cout << "--- Using through reference (correct way) ---\n";
    const Vehicle& vRef = car;
    vRef.drive();

    cout << "\n--- Using through pointer (correct way) ---\n";
    Vehicle* vPtr = &car;
    vPtr->drive();

    cout << "\n✅ PREVENTION TECHNIQUES:\n";
    cout << "   • Delete copy constructor\n";
    cout << "   • Delete copy assignment operator\n";
    cout << "   • Make base class abstract (pure virtual)\n";
    cout << "   • Force use of pointers/references\n";
    cout << "   • Compiler errors instead of runtime bugs!\n";
}

// ============================================
// EXAMPLE 6: VIRTUAL DESTRUCTOR AND SLICING
// ============================================

class Base {
public:
    Base() {
        cout << "  Base constructor\n";
    }

    virtual ~Base() {  // ✅ Virtual destructor
        cout << "  Base destructor\n";
    }
};

class Derived : public Base {
private:
    int* data;

public:
    Derived() : data(new int[100]) {
        cout << "  Derived constructor (allocated memory)\n";
    }

    ~Derived() {
        delete[] data;
        cout << "  Derived destructor (freed memory)\n";
    }
};

void demonstrateDestructorSlicing() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: DESTRUCTOR & SLICING  ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "--- WRONG: Delete through base pointer (no slicing) ---\n";
    Base* ptr = new Derived();
    delete ptr;  // ✅ Virtual destructor calls both destructors

    cout << "\n--- Slicing in variable assignment ---\n";
    {
        Derived d;
        Base b = d;  // ⚠️ Slicing! Only base part copied
        // When b goes out of scope, only Base destructor called
        // Derived destructor NOT called for sliced part
    }

    cout << "\n💡 VIRTUAL DESTRUCTOR:\n";
    cout << "   • Always virtual in base classes\n";
    cout << "   • Ensures proper cleanup\n";
    cout << "   • Prevents memory leaks\n";
    cout << "   • Related to slicing issues\n";
}

// ============================================
// EXAMPLE 7: RETURN VALUE SLICING
// ============================================

class Employee {
protected:
    string name;
    double salary;

public:
    Employee(const string& n, double s) : name(n), salary(s) {}

    virtual void displaySalary() const {
        cout << "  " << name << ": $" << salary << "\n";
    }

    virtual ~Employee() = default;
};

class Manager : public Employee {
private:
    double bonus;

public:
    Manager(const string& n, double s, double b)
        : Employee(n, s), bonus(b) {}

    void displaySalary() const override {
        cout << "  " << name << ": $" << salary
             << " + $" << bonus << " bonus = $"
             << (salary + bonus) << "\n";
    }
};

// ❌ Returns by value - causes slicing!
Employee getEmployeeBad() {
    Manager mgr("Alice", 80000, 20000);
    return mgr;  // ⚠️ Sliced to Employee!
}

// ✅ Returns pointer - no slicing
Employee* getEmployeeGood() {
    return new Manager("Bob", 85000, 25000);
}

// ✅ Returns smart pointer - no slicing
unique_ptr<Employee> getEmployeeBest() {
    return make_unique<Manager>("Charlie", 90000, 30000);
}

void demonstrateReturnSlicing() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 7: RETURN VALUE SLICING  ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "--- Return by value (SLICING) ---\n";
    Employee emp = getEmployeeBad();
    emp.displaySalary();  // ⚠️ Bonus is lost!

    cout << "\n--- Return by pointer (NO slicing) ---\n";
    Employee* empPtr = getEmployeeGood();
    empPtr->displaySalary();  // ✅ Bonus included!
    delete empPtr;

    cout << "\n--- Return smart pointer (BEST) ---\n";
    auto empSmart = getEmployeeBest();
    empSmart->displaySalary();  // ✅ Bonus included!
    // Automatic cleanup

    cout << "\n💡 RETURN VALUES:\n";
    cout << "   • Never return polymorphic objects by value\n";
    cout << "   • Use pointers or references\n";
    cout << "   • Prefer smart pointers\n";
    cout << "   • Avoid slicing in return values\n";
}

// ============================================
// EXAMPLE 8: ASSIGNMENT OPERATOR SLICING
// ============================================

class Book {
protected:
    string title;
    double price;

public:
    Book(const string& t, double p) : title(t), price(p) {}

    virtual void display() const {
        cout << "  Book: " << title << " ($" << price << ")\n";
    }

    virtual ~Book() = default;
};

class EBook : public Book {
private:
    string format;  // PDF, EPUB, etc.

public:
    EBook(const string& t, double p, const string& f)
        : Book(t, p), format(f) {}

    void display() const override {
        cout << "  EBook: " << title << " ($" << price
             << ") Format: " << format << "\n";
    }
};

void demonstrateAssignmentSlicing() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 8: ASSIGNMENT SLICING    ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    EBook ebook("Modern C++", 49.99, "PDF");
    Book book("Old C++", 29.99);

    cout << "--- Before assignment ---\n";
    ebook.display();
    book.display();

    cout << "\n--- Assignment (SLICING) ---\n";
    book = ebook;  // ⚠️ Only base part assigned, format is lost!

    cout << "\n--- After assignment ---\n";
    ebook.display();  // ✅ Still has format
    book.display();   // ⚠️ Format was sliced off

    cout << "\n⚠️  WHAT HAPPENED:\n";
    cout << "   • Only Book part of EBook was copied\n";
    cout << "   • format member was lost\n";
    cout << "   • book is still a Book, not an EBook\n";
    cout << "   • Assignment operator doesn't change type\n";
}

// ============================================
// EXAMPLE 9: DETECTING SLICING AT COMPILE TIME
// ============================================

// ✅ Non-copyable base class
class NonCopyableBase {
protected:
    NonCopyableBase() = default;
    ~NonCopyableBase() = default;

public:
    NonCopyableBase(const NonCopyableBase&) = delete;
    NonCopyableBase& operator=(const NonCopyableBase&) = delete;
};

class SafeDerived : public NonCopyableBase {
private:
    string data;

public:
    SafeDerived(const string& d) : data(d) {}

    void display() const {
        cout << "  SafeDerived: " << data << "\n";
    }
};

void demonstrateCompileTimeDetection() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 9: COMPILE-TIME DETECTION║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    SafeDerived obj("Test");
    obj.display();

    // SafeDerived copy = obj;  // ❌ Compilation error!
    // NonCopyableBase base = obj;  // ❌ Compilation error!

    cout << "--- Using through reference (OK) ---\n";
    const NonCopyableBase& ref = obj;
    // ref.display();  // Would work if display() was in base

    cout << "\n✅ COMPILE-TIME DETECTION:\n";
    cout << "   • Delete copy operations in base\n";
    cout << "   • Compiler catches slicing attempts\n";
    cout << "   • Errors at compile time, not runtime\n";
    cout << "   • Forces correct usage\n";
    cout << "   • Safer design\n";
}

// ============================================
// EXAMPLE 10: BEST PRACTICES SUMMARY
// ============================================

void demonstrateBestPractices() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 10: BEST PRACTICES       ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "✅ PREVENTING OBJECT SLICING:\n";
    cout << "\n";
    cout << "1. Use Pointers or References:\n";
    cout << "   void func(Base& obj);          // ✅ Reference\n";
    cout << "   void func(Base* obj);          // ✅ Pointer\n";
    cout << "   void func(Base obj);           // ❌ Value - slicing!\n";
    cout << "\n";
    cout << "2. Use Smart Pointers in Containers:\n";
    cout << "   vector<unique_ptr<Base>> v;    // ✅ Smart pointers\n";
    cout << "   vector<Base*> v;               // ⚠️  Raw pointers\n";
    cout << "   vector<Base> v;                // ❌ Value - slicing!\n";
    cout << "\n";
    cout << "3. Delete Copy Operations:\n";
    cout << "   Base(const Base&) = delete;\n";
    cout << "   Base& operator=(const Base&) = delete;\n";
    cout << "\n";
    cout << "4. Make Base Class Abstract:\n";
    cout << "   virtual void func() = 0;       // Pure virtual\n";
    cout << "\n";
    cout << "5. Always Virtual Destructor:\n";
    cout << "   virtual ~Base() = default;\n";
    cout << "\n";
    cout << "❌ COMMON MISTAKES:\n";
    cout << "   • Passing polymorphic objects by value\n";
    cout << "   • Using vector<Base> instead of vector<Base*>\n";
    cout << "   • Returning derived objects by value as base\n";
    cout << "   • Assigning derived to base variable\n";
    cout << "   • Not using virtual destructor\n";
    cout << "\n";
    cout << "💡 GOLDEN RULE:\n";
    cout << "   For polymorphic types, ALWAYS use:\n";
    cout << "   • Pointers (preferably smart pointers)\n";
    cout << "   • References\n";
    cout << "   NEVER use:\n";
    cout << "   • Pass by value\n";
    cout << "   • Store by value\n";
    cout << "   • Return by value\n";
}

// ============================================
// MAIN FUNCTION - RUN ALL EXAMPLES
// ============================================

int main() {
    cout << "\n";
    cout << "╔═══════════════════════════════════════════════════════════╗\n";
    cout << "║                                                           ║\n";
    cout << "║             OBJECT SLICING IN C++                         ║\n";
    cout << "║          The Hidden Danger of Inheritance                 ║\n";
    cout << "║                                                           ║\n";
    cout << "╚═══════════════════════════════════════════════════════════╝\n";

    demonstrateProblem();
    demonstrateFunctionSlicing();
    demonstrateContainerSlicing();
    demonstrateCopySlicing();
    demonstratePreventSlicing();
    demonstrateDestructorSlicing();
    demonstrateReturnSlicing();
    demonstrateAssignmentSlicing();
    demonstrateCompileTimeDetection();
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
    cout << "WHAT IS OBJECT SLICING?\n";
    cout << "========================================\n";
    cout << "Definition:\n";
    cout << "  Object slicing occurs when a derived class object is\n";
    cout << "  assigned to a base class object. The derived part is\n";
    cout << "  'sliced off', causing data loss and loss of polymorphism.\n";
    cout << "\n";
    cout << "EXAMPLE:\n";
    cout << "class Base {\n";
    cout << "    int x;\n";
    cout << "};\n";
    cout << "\n";
    cout << "class Derived : public Base {\n";
    cout << "    int y;  // Additional member\n";
    cout << "};\n";
    cout << "\n";
    cout << "Derived d;\n";
    cout << "Base b = d;  // ⚠️  SLICING! y is lost\n";
    cout << "\n";
    cout << "WHAT GETS LOST:\n";
    cout << "  • Derived class members\n";
    cout << "  • Polymorphic behavior\n";
    cout << "  • Virtual function dispatch\n";
    cout << "  • Type information\n";

    cout << "\n========================================\n";
    cout << "WHY SLICING HAPPENS\n";
    cout << "========================================\n";
    cout << "1. Pass by Value:\n";
    cout << "   void func(Base b);  // ⚠️  Slicing!\n";
    cout << "   Derived d;\n";
    cout << "   func(d);  // Only base part copied\n";
    cout << "\n";
    cout << "2. Assignment:\n";
    cout << "   Derived d;\n";
    cout << "   Base b = d;  // ⚠️  Slicing!\n";
    cout << "\n";
    cout << "3. Containers of Values:\n";
    cout << "   vector<Base> v;  // ⚠️  Slicing!\n";
    cout << "   v.push_back(derived);  // Sliced\n";
    cout << "\n";
    cout << "4. Return by Value:\n";
    cout << "   Base func() {\n";
    cout << "       Derived d;\n";
    cout << "       return d;  // ⚠️  Slicing!\n";
    cout << "   }\n";

    cout << "\n========================================\n";
    cout << "THE DANGERS\n";
    cout << "========================================\n";
    cout << "SILENT DATA LOSS:\n";
    cout << "  • Derived members disappear\n";
    cout << "  • No compiler warning\n";
    cout << "  • Hard to detect\n";
    cout << "  • Runtime bugs\n";
    cout << "\n";
    cout << "LOSS OF POLYMORPHISM:\n";
    cout << "  • Virtual functions don't work\n";
    cout << "  • Wrong function called\n";
    cout << "  • Incorrect behavior\n";
    cout << "  • Breaks OOP principles\n";
    cout << "\n";
    cout << "MEMORY ISSUES:\n";
    cout << "  • Destructor might not be called\n";
    cout << "  • Memory leaks possible\n";
    cout << "  • Resource cleanup fails\n";

    cout << "\n========================================\n";
    cout << "HOW TO PREVENT SLICING\n";
    cout << "========================================\n";
    cout << "SOLUTION 1: Use References\n";
    cout << "void func(const Base& b);  // ✅ No slicing\n";
    cout << "Derived d;\n";
    cout << "func(d);  // Polymorphism works!\n";
    cout << "\n";
    cout << "SOLUTION 2: Use Pointers\n";
    cout << "void func(Base* b);  // ✅ No slicing\n";
    cout << "Derived d;\n";
    cout << "func(&d);  // Polymorphism works!\n";
    cout << "\n";
    cout << "SOLUTION 3: Smart Pointers\n";
    cout << "vector<unique_ptr<Base>> v;  // ✅ No slicing\n";
    cout << "v.push_back(make_unique<Derived>());\n";
    cout << "\n";
    cout << "SOLUTION 4: Delete Copy Operations\n";
    cout << "class Base {\n";
    cout << "    Base(const Base&) = delete;\n";
    cout << "    Base& operator=(const Base&) = delete;\n";
    cout << "};\n";
    cout << "\n";
    cout << "SOLUTION 5: Abstract Base Class\n";
    cout << "class Base {\n";
    cout << "    virtual void func() = 0;  // Pure virtual\n";
    cout << "};\n";
    cout << "// Can't create Base objects directly\n";

    cout << "\n========================================\n";
    cout << "BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "✅ DO:\n";
    cout << "  • Use pointers/references for polymorphic types\n";
    cout << "  • Prefer smart pointers (unique_ptr, shared_ptr)\n";
    cout << "  • Make base destructors virtual\n";
    cout << "  • Delete copy operations if appropriate\n";
    cout << "  • Use abstract base classes\n";
    cout << "  • Store pointers in containers\n";
    cout << "  • Pass by reference or pointer\n";
    cout << "  • Return pointers, not values\n";
    cout << "\n";
    cout << "❌ DON'T:\n";
    cout << "  • Pass polymorphic objects by value\n";
    cout << "  • Use vector<Base> for derived objects\n";
    cout << "  • Return polymorphic objects by value\n";
    cout << "  • Assign derived to base variables\n";
    cout << "  • Forget virtual destructors\n";
    cout << "  • Copy polymorphic objects carelessly\n";
    cout << "  • Ignore slicing warnings\n";

    cout << "\n========================================\n";
    cout << "QUICK REFERENCE\n";
    cout << "========================================\n";
    cout << "// ❌ WRONG - Causes slicing\n";
    cout << "void func(Base b) { }           // Pass by value\n";
    cout << "Base b = derived;               // Assignment\n";
    cout << "vector<Base> v;                 // Container of values\n";
    cout << "return derivedObject;           // Return by value\n";
    cout << "\n";
    cout << "// ✅ CORRECT - No slicing\n";
    cout << "void func(const Base& b) { }    // Pass by reference\n";
    cout << "void func(Base* b) { }          // Pass by pointer\n";
    cout << "Base& b = derived;              // Reference\n";
    cout << "Base* b = &derived;             // Pointer\n";
    cout << "vector<unique_ptr<Base>> v;     // Container of pointers\n";
    cout << "return unique_ptr<Base>(...);   // Return smart pointer\n";

    cout << "\n========================================\n";
    cout << "COMMON SCENARIOS\n";
    cout << "========================================\n";
    cout << "SCENARIO 1: Function Parameters\n";
    cout << "❌ void process(Animal a);          // Slicing\n";
    cout << "✅ void process(const Animal& a);   // No slicing\n";
    cout << "✅ void process(Animal* a);         // No slicing\n";
    cout << "\n";
    cout << "SCENARIO 2: Containers\n";
    cout << "❌ vector<Shape> shapes;            // Slicing\n";
    cout << "✅ vector<Shape*> shapes;           // No slicing\n";
    cout << "✅ vector<unique_ptr<Shape>> shapes;// Best\n";
    cout << "\n";
    cout << "SCENARIO 3: Return Values\n";
    cout << "❌ Animal getAnimal();              // Slicing\n";
    cout << "✅ Animal* getAnimal();             // No slicing\n";
    cout << "✅ unique_ptr<Animal> getAnimal();  // Best\n";
    cout << "\n";
    cout << "SCENARIO 4: Assignment\n";
    cout << "❌ Base b = derived;                // Slicing\n";
    cout << "✅ Base& b = derived;               // No slicing\n";
    cout << "✅ Base* b = &derived;              // No slicing\n";

    cout << "\n========================================\n";
    cout << "KEY TAKEAWAYS\n";
    cout << "========================================\n";
    cout << "1. Object slicing loses derived class data\n";
    cout << "2. Slicing happens with pass-by-value\n";
    cout << "3. Polymorphism is lost when sliced\n";
    cout << "4. Silent bug - usually no warning\n";
    cout << "5. Use pointers/references to prevent\n";
    cout << "6. Smart pointers are best solution\n";
    cout << "7. Delete copy operations to enforce safety\n";
    cout << "8. Virtual destructors are essential\n";
    cout << "9. Abstract base classes help prevent\n";
    cout << "10. NEVER pass polymorphic objects by value\n";

    cout << "\n========================================\n";
    cout << "MEMORY LAYOUT\n";
    cout << "========================================\n";
    cout << "BEFORE SLICING (Derived object):\n";
    cout << "┌─────────────────────┐\n";
    cout << "│ Base part:          │\n";
    cout << "│   - vptr            │\n";
    cout << "│   - base members    │\n";
    cout << "├─────────────────────┤\n";
    cout << "│ Derived part:       │\n";
    cout << "│   - derived members │\n";
    cout << "└─────────────────────┘\n";
    cout << "\n";
    cout << "AFTER SLICING (Base object):\n";
    cout << "┌─────────────────────┐\n";
    cout << "│ Base part:          │\n";
    cout << "│   - vptr (Base)     │\n";
    cout << "│   - base members    │\n";
    cout << "└─────────────────────┘\n";
    cout << "  ⚠️  Derived part LOST!\n";

    cout << "\n========================================\n";
    cout << "DECISION TREE\n";
    cout << "========================================\n";
    cout << "Working with polymorphic types?\n";
    cout << "  YES → Continue...\n";
    cout << "  NO  → Can use values\n";
    cout << "\n";
    cout << "Need to pass to function?\n";
    cout << "  YES → Use reference or pointer ✅\n";
    cout << "  NO  → Continue...\n";
    cout << "\n";
    cout << "Need to store in container?\n";
    cout << "  YES → Use vector<unique_ptr<Base>> ✅\n";
    cout << "  NO  → Continue...\n";
    cout << "\n";
    cout << "Need to return from function?\n";
    cout << "  YES → Return unique_ptr<Base> ✅\n";
    cout << "  NO  → Continue...\n";
    cout << "\n";
    cout << "Need to assign?\n";
    cout << "  YES → Use reference or pointer ✅\n";
    cout << "  NO  → Still use pointer/reference ✅\n";
    cout << "\n";
    cout << "DEFAULT: ALWAYS use pointers/references\n";
    cout << "         for polymorphic types!\n";

    cout << "\n========================================\n";
    cout << "COMPARISON TABLE\n";
    cout << "========================================\n";
    cout << "┌────────────────────┬──────────┬────────────┐\n";
    cout << "│ Approach           │ Slicing? │ Recommend  │\n";
    cout << "├────────────────────┼──────────┼────────────┤\n";
    cout << "│ Pass by value      │ YES ❌   │ Never      │\n";
    cout << "│ Pass by reference  │ NO ✅    │ Yes        │\n";
    cout << "│ Pass by pointer    │ NO ✅    │ Yes        │\n";
    cout << "│ vector<Base>       │ YES ❌   │ Never      │\n";
    cout << "│ vector<Base*>      │ NO ✅    │ OK         │\n";
    cout << "│ vector<unique_ptr> │ NO ✅    │ Best       │\n";
    cout << "│ Return by value    │ YES ❌   │ Never      │\n";
    cout << "│ Return pointer     │ NO ✅    │ OK         │\n";
    cout << "│ Return unique_ptr  │ NO ✅    │ Best       │\n";
    cout << "│ Assignment         │ YES ❌   │ Avoid      │\n";
    cout << "│ Reference          │ NO ✅    │ Yes        │\n";
    cout << "└────────────────────┴──────────┴────────────┘\n";

    return 0;
}
