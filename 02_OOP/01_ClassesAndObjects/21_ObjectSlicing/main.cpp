#include <print>
#include <string>
#include <vector>
#include <memory>
#include <typeinfo>
#include <type_traits>
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
 * - Detect it: clang-tidy check cppcoreguidelines-slicing
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
    explicit Animal(const string& n) : name(n) {
        std::println("  Animal constructor: {}", name);
    }

    virtual void speak() const {
        std::println("  {} makes a sound", name);
    }

    virtual void display() const {
        std::println("  Animal: {}", name);
    }

    virtual ~Animal() {
        std::println("  Animal destructor: {}", name);
    }
};

class Dog : public Animal {
private:
    string breed;

public:
    Dog(const string& n, const string& b)
        : Animal(n), breed(b) {
        std::println("  Dog constructor: {} ({})", name, breed);
    }

    void speak() const override {
        std::println("  {} barks: Woof! Woof!", name);
    }

    void display() const override {
        std::println("  Dog: {} (Breed: {})", name, breed);
    }

    ~Dog() {
        std::println("  Dog destructor: {}", name);
    }
};

void demonstrateProblem() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 1: THE PROBLEM           ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    std::println("--- Creating a Dog object ---");
    Dog dog("Buddy", "Golden Retriever");
    dog.display();
    dog.speak();

    std::println("");
    std::println("--- Object Slicing: Assigning Dog to Animal ---");
    Animal animal = dog;  // ⚠️ SLICING HAPPENS HERE!
    animal.display();     // ⚠️ Calls Animal::display, not Dog::display
    animal.speak();       // ⚠️ Calls Animal::speak, not Dog::speak
}

// ============================================
// EXAMPLE 2: SLICING IN FUNCTION CALLS
// ============================================

void processAnimalByValue(Animal a) {  // ⚠️ Pass by value - causes slicing!
    std::println("  Processing animal:");
    a.display();
    a.speak();
}

void processAnimalByReference(const Animal& a) {  // ✅ Pass by reference - no slicing
    std::println("  Processing animal:");
    a.display();
    a.speak();
}

void processAnimalByPointer(const Animal* a) {  // ✅ Pass by pointer - no slicing
    std::println("  Processing animal:");
    a->display();
    a->speak();
}

void demonstrateFunctionSlicing() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 2: FUNCTION SLICING      ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    Dog dog("Max", "German Shepherd");

    std::println("--- Pass by value (SLICING!) ---");
    processAnimalByValue(dog);  // ⚠️ Dog is sliced to Animal

    std::println("");
    std::println("--- Pass by reference (NO slicing) ---");
    processAnimalByReference(dog);  // ✅ Polymorphism works!

    std::println("");
    std::println("--- Pass by pointer (NO slicing) ---");
    processAnimalByPointer(&dog);  // ✅ Polymorphism works!
}

// ============================================
// EXAMPLE 3: SLICING IN CONTAINERS
// ============================================

void demonstrateContainerSlicing() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 3: CONTAINER SLICING     ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    Dog dog1("Rocky", "Bulldog");
    Dog dog2("Luna", "Husky");

    std::println("--- BAD: vector of Animal (value) - SLICING! ---");
    vector<Animal> animals;  // ⚠️ Stores by value!
    animals.push_back(dog1);  // ⚠️ Dog is sliced!
    animals.push_back(dog2);  // ⚠️ Dog is sliced!

    std::println("Iterating over animals:");
    for (const auto& a : animals) {
        a.display();  // ⚠️ Only calls Animal::display
        a.speak();    // ⚠️ Only calls Animal::speak
    }

    std::println("");
    std::println("--- GOOD: vector of pointers - NO slicing ---");
    vector<Animal*> animalPtrs;  // ✅ Stores pointers!
    animalPtrs.push_back(new Dog("Charlie", "Beagle"));
    animalPtrs.push_back(new Dog("Daisy", "Poodle"));

    std::println("Iterating over animal pointers:");
    for (const auto* a : animalPtrs) {
        a->display();  // ✅ Calls Dog::display
        a->speak();    // ✅ Calls Dog::speak
    }

    // Clean up
    for (auto* a : animalPtrs) {
        delete a;
    }

    std::println("");
    std::println("--- BEST: vector of smart pointers ---");
    vector<unique_ptr<Animal>> smartPtrs;
    smartPtrs.push_back(make_unique<Dog>("Bella", "Labrador"));
    smartPtrs.push_back(make_unique<Dog>("Cooper", "Boxer"));

    std::println("Iterating over smart pointers:");
    for (const auto& a : smartPtrs) {
        a->display();  // ✅ Calls Dog::display
        a->speak();    // ✅ Calls Dog::speak
    }
    // Automatic cleanup!
}

// ============================================
// EXAMPLE 4: COPY CONSTRUCTOR AND SLICING
// ============================================

class Shape {
protected:
    string color;

public:
    explicit Shape(const string& c) : color(c) {
        std::println("  Shape({})", color);
    }

    virtual double area() const {
        return 0.0;
    }

    virtual void display() const {
        std::println("  Shape, color: {}", color);
    }

    virtual ~Shape() {
        std::println("  ~Shape()");
    }
};

class Circle : public Shape {
private:
    double radius;

public:
    Circle(const string& c, double r)
        : Shape(c), radius(r) {
        std::println("  Circle(radius={})", radius);
    }

    double area() const override {
        return 3.14159 * radius * radius;
    }

    void display() const override {
        std::println("  Circle, color: {}, radius: {:g}, area: {:g}", color, radius, area());
    }

    ~Circle() {
        std::println("  ~Circle()");
    }
};

void demonstrateCopySlicing() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 4: COPY SLICING          ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    std::println("--- Creating Circle ---");
    Circle circle("Red", 5.0);
    circle.display();

    std::println("");
    std::println("--- Copy to Shape (SLICING) ---");
    Shape shape = circle;  // ⚠️ Slicing!
    shape.display();       // ⚠️ Only Shape part
    std::println("  Area: {:g}", shape.area());  // ⚠️ Returns 0, not circle area!

    std::println("");
    std::println("--- Copy via reference (NO slicing) ---");
    const Shape& shapeRef = circle;  // ✅ Reference
    shapeRef.display();              // ✅ Calls Circle::display
    std::println("  Area: {:g}", shapeRef.area());  // ✅ Correct area!
}

// ============================================
// EXAMPLE 5: PREVENTING SLICING - DELETE COPY
// ============================================

class Vehicle {
protected:
    string brand;

public:
    explicit Vehicle(const string& b) : brand(b) {}

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
        std::println("  Driving {} car with {} doors", brand, doors);
    }
};

void demonstratePreventSlicing() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 5: PREVENTING SLICING    ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    Car car("Toyota", 4);

    // Vehicle v = car;  // ❌ Compilation error! Copy deleted
    // Vehicle v2;       // ❌ Can't create Vehicle (abstract)

    std::println("--- Using through reference (correct way) ---");
    const Vehicle& vRef = car;
    vRef.drive();

    std::println("");
    std::println("--- Using through pointer (correct way) ---");
    Vehicle* vPtr = &car;
    vPtr->drive();
}

// ============================================
// EXAMPLE 6: VIRTUAL DESTRUCTOR AND SLICING
// ============================================

class Base {
public:
    Base() {
        std::println("  Base constructor");
    }

    virtual ~Base() {  // ✅ Virtual destructor
        std::println("  Base destructor");
    }
};

class Derived : public Base {
private:
    int* data;

public:
    Derived() : data(new int[100]) {
        std::println("  Derived constructor (allocated memory)");
    }

    ~Derived() {
        delete[] data;
        std::println("  Derived destructor (freed memory)");
    }

    // Owns a raw array: copying a Derived would delete it twice
    Derived(const Derived&) = delete;
    Derived& operator=(const Derived&) = delete;
};

void demonstrateDestructorSlicing() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 6: DESTRUCTOR & SLICING  ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    std::println("--- Delete through a base pointer (the virtual destructor makes it safe) ---");
    Base* ptr = new Derived();
    delete ptr;  // ✅ Virtual destructor calls both destructors

    std::println("");
    std::println("--- Slicing in variable assignment ---");
    {
        Derived d;
        Base b = d;  // ⚠️ Slicing! Only base part copied
        // b is a complete Base object: at the end of the scope ~Base() runs for b,
        // and ~Derived() + ~Base() run for d. Nothing leaks - slicing copies, it destroys nothing.
    }
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
        std::println("  {}: ${}", name, salary);
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
        std::println("  {}: ${} + ${} bonus = ${}", name, salary, bonus, (salary + bonus));
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
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 7: RETURN VALUE SLICING  ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    std::println("--- Return by value (SLICING) ---");
    Employee emp = getEmployeeBad();
    emp.displaySalary();  // ⚠️ Bonus is lost!

    std::println("");
    std::println("--- Return by pointer (NO slicing) ---");
    Employee* empPtr = getEmployeeGood();
    empPtr->displaySalary();  // ✅ Bonus included!
    delete empPtr;

    std::println("");
    std::println("--- Return smart pointer (BEST) ---");
    auto empSmart = getEmployeeBest();
    empSmart->displaySalary();  // ✅ Bonus included!
    // Automatic cleanup
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
        std::println("  Book: {} (${})", title, price);
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
        std::println("  EBook: {} (${}) Format: {}", title, price, format);
    }
};

void demonstrateAssignmentSlicing() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 8: ASSIGNMENT SLICING    ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    EBook ebook("Modern C++", 49.99, "PDF");
    Book book("Old C++", 29.99);

    std::println("--- Before assignment ---");
    ebook.display();
    book.display();

    std::println("");
    std::println("--- Assignment (SLICING) ---");
    book = ebook;  // ⚠️ Only base part assigned, format is lost!

    std::println("");
    std::println("--- After assignment ---");
    ebook.display();  // ✅ Still has format
    book.display();   // ⚠️ Format was sliced off
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
    explicit SafeDerived(const string& d) : data(d) {}

    void display() const {
        std::println("  SafeDerived: {}", data);
    }
};

// Alternative to "no copy at all": protected copy operations + virtual clone()
class ClonableBase {
protected:
    ClonableBase() = default;
    ClonableBase(const ClonableBase&) = default;  // protected: no slicing from outside
    ClonableBase& operator=(const ClonableBase&) = default;

public:
    virtual unique_ptr<ClonableBase> clone() const = 0;
    virtual void speak() const = 0;
    virtual ~ClonableBase() = default;
};

class ClonableDog : public ClonableBase {
private:
    string name;

public:
    explicit ClonableDog(const string& n) : name(n) {}

    unique_ptr<ClonableBase> clone() const override {
        return make_unique<ClonableDog>(*this);
    }

    void speak() const override {
        std::println("  {} barks", name);
    }
};

void demonstrateCompileTimeDetection() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 9: COMPILE-TIME DETECTION║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    SafeDerived obj("Test");
    obj.display();

    // SafeDerived copy = obj;  // ❌ Compilation error!
    // NonCopyableBase base = obj;  // ❌ Compilation error!

    std::println("--- Using through reference (OK) ---");
    const NonCopyableBase& ref = obj;
    (void)ref;  // display() is not in the base, so there is nothing to call here

    std::println("");
    std::println("--- Alternative: protected copy + clone() ---");
    ClonableDog rex("Rex");
    ClonableDog copy = rex;  // ✅ copying the DERIVED type is fine
    copy.speak();

    unique_ptr<ClonableBase> original = make_unique<ClonableDog>("Fido");
    unique_ptr<ClonableBase> cloned = original->clone();  // ✅ copy through a base pointer
    cloned->speak();
    // ClonableBase b = rex;  // ❌ Error: copy constructor is protected
}

// ============================================
// EXAMPLE 10: BEST PRACTICES SUMMARY
// ============================================

void demonstrateBestPractices() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 10: BEST PRACTICES       ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    std::println("--- Creating objects ---");
    Dog dog("Rex", "Collie");
    Animal sliced = dog;         // ⚠️ copy into an Animal
    const Animal& ref = dog;     // ✅ no copy

    std::println("");
    std::println("--- What is the dynamic type? ---");
    std::println("  typeid(sliced) == typeid(Animal): {}", (typeid(sliced) == typeid(Animal)));
    std::println("  typeid(ref)    == typeid(Dog):    {}", (typeid(ref) == typeid(Dog)));
    std::println("  sizeof(Dog) > sizeof(Animal):     {}", (sizeof(Dog) > sizeof(Animal)));

    std::println("");
    std::println("--- Which types can be copied (= sliced) at all? ---");
    std::println("  Animal copyable?      {}", is_copy_constructible_v<Animal>);
    std::println("  Vehicle copyable?     {}", is_copy_constructible_v<Vehicle>);
    std::println("  Car copyable?         {}", is_copy_constructible_v<Car>);
    std::println("  SafeDerived copyable? {}", is_copy_constructible_v<SafeDerived>);
    std::println("  Vehicle abstract?     {}", is_abstract_v<Vehicle>);

    // Tool support: clang-tidy -checks=-*,cppcoreguidelines-slicing main.cpp
    // reports every "Animal animal = dog;" style slicing in this file.
}

// ============================================
// MAIN FUNCTION - RUN ALL EXAMPLES
// ============================================

int main() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════════════╗");
    std::println("║                                                           ║");
    std::println("║             OBJECT SLICING IN C++                         ║");
    std::println("║          The Hidden Danger of Inheritance                 ║");
    std::println("║                                                           ║");
    std::println("╚═══════════════════════════════════════════════════════════╝");

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

    return 0;
}
