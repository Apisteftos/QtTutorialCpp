#include <iostream>
#include <string>
#include <vector>
using namespace std;

// ============================================
// WHAT IS INHERITANCE?
// ============================================

/*
 * INHERITANCE:
 * - Mechanism to create new classes from existing classes
 * - "IS-A" relationship (Dog IS-A Animal)
 * - Derived class inherits members from base class
 * - Promotes code reuse and hierarchical classification
 *
 * TERMINOLOGY:
 * - Base class / Parent class / Superclass
 * - Derived class / Child class / Subclass
 *
 * TYPES OF INHERITANCE:
 * 1. Single inheritance (A → B)
 * 2. Multiple inheritance (A, B → C)
 * 3. Multilevel inheritance (A → B → C)
 * 4. Hierarchical inheritance (A → B, A → C)
 * 5. Hybrid inheritance (combination)
 *
 * ACCESS SPECIFIERS:
 * - public inheritance: IS-A relationship (most common)
 * - protected inheritance: rarely used
 * - private inheritance: HAS-A relationship (composition)
 *
 * WHAT IS INHERITED:
 * ✅ Public members (as public in derived)
 * ✅ Protected members (as protected in derived)
 * ❌ Private members (exist but not accessible)
 * ❌ Constructors (not inherited, but called)
 * ❌ Destructors (not inherited, but called)
 * ❌ Assignment operators (not inherited)
 */

// ============================================
// EXAMPLE 1: BASIC INHERITANCE
// ============================================

class Animal {
protected:  // Protected: accessible to derived classes
    string name;
    int age;

public:
    Animal(string n, int a) : name(n), age(a) {
        cout << "  Animal constructor: " << name << "\n";
    }

    ~Animal() {
        cout << "  Animal destructor: " << name << "\n";
    }

    void eat() {
        cout << "  " << name << " is eating\n";
    }

    void sleep() {
        cout << "  " << name << " is sleeping\n";
    }

    void displayInfo() {
        cout << "  Name: " << name << ", Age: " << age << "\n";
    }
};

// Dog inherits from Animal
class Dog : public Animal {  // public inheritance
private:
    string breed;

public:
    // Constructor must call base constructor
    Dog(string n, int a, string b) : Animal(n, a), breed(b) {
        cout << "  Dog constructor: " << name << " (" << breed << ")\n";
    }

    ~Dog() {
        cout << "  Dog destructor: " << name << "\n";
    }

    // Dog-specific method
    void bark() {
        cout << "  " << name << " says: Woof! Woof!\n";
    }

    // Dog can access protected members from Animal
    void displayDogInfo() {
        cout << "  Dog: " << name << ", Age: " << age << ", Breed: " << breed << "\n";
    }
};

void demonstrateBasicInheritance() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: BASIC INHERITANCE      ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating Dog object ---\n";
    Dog dog("Buddy", 3, "Golden Retriever");

    cout << "\n--- Using inherited methods ---\n";
    dog.eat();      // Inherited from Animal
    dog.sleep();    // Inherited from Animal

    cout << "\n--- Using Dog-specific methods ---\n";
    dog.bark();     // Dog's own method

    cout << "\n--- Displaying info ---\n";
    dog.displayInfo();     // Inherited from Animal
    dog.displayDogInfo();  // Dog's own method

    cout << "\n💡 Inheritance benefits:\n";
    cout << "   • Code reuse (eat, sleep inherited)\n";
    cout << "   • Hierarchical organization\n";
    cout << "   • Add specialized behavior (bark)\n";

    cout << "\n--- Destruction order (reverse of construction) ---\n";
}

// ============================================
// EXAMPLE 2: ACCESS SPECIFIERS IN INHERITANCE
// ============================================

class Base {
private:
    int privateData;      // Only Base can access

protected:
    int protectedData;    // Base and derived can access

public:
    int publicData;       // Everyone can access

    Base() : privateData(1), protectedData(2), publicData(3) { }

    void showBase() {
        cout << "  Base - private: " << privateData
             << ", protected: " << protectedData
             << ", public: " << publicData << "\n";
    }
};

// Public inheritance (most common)
class PublicDerived : public Base {
public:
    void showAccess() {
        cout << "  PublicDerived can access:\n";
        // cout << privateData;     // ❌ Error! Private not accessible
        cout << "    protected: " << protectedData << " ✅\n";
        cout << "    public: " << publicData << " ✅\n";
    }
};

// Protected inheritance (rarely used)
class ProtectedDerived : protected Base {
public:
    void showAccess() {
        cout << "  ProtectedDerived can access:\n";
        // cout << privateData;     // ❌ Error! Private not accessible
        cout << "    protected: " << protectedData << " ✅\n";
        cout << "    public: " << publicData << " ✅\n";
    }
};

// Private inheritance (composition alternative)
class PrivateDerived : private Base {
public:
    void showAccess() {
        cout << "  PrivateDerived can access:\n";
        // cout << privateData;     // ❌ Error! Private not accessible
        cout << "    protected: " << protectedData << " ✅\n";
        cout << "    public: " << publicData << " ✅\n";
    }
};

void demonstrateAccessSpecifiers() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: ACCESS SPECIFIERS      ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Public inheritance ---\n";
    PublicDerived pub;
    pub.showAccess();
    pub.publicData = 100;  // ✅ Public members accessible
    pub.showBase();        // ✅ Public methods accessible

    cout << "\n--- Protected inheritance ---\n";
    ProtectedDerived prot;
    prot.showAccess();
    // prot.publicData = 100;  // ❌ Error! Now protected
    // prot.showBase();        // ❌ Error! Now protected

    cout << "\n--- Private inheritance ---\n";
    PrivateDerived priv;
    priv.showAccess();
    // priv.publicData = 100;  // ❌ Error! Now private
    // priv.showBase();        // ❌ Error! Now private

    cout << "\n💡 Inheritance types:\n";
    cout << "   • public:    IS-A relationship (use 99% of time)\n";
    cout << "   • protected: Rarely used\n";
    cout << "   • private:   HAS-A (use composition instead)\n";
}

// ============================================
// EXAMPLE 3: CONSTRUCTOR AND DESTRUCTOR CALLS
// ============================================

class GrandParent {
public:
    GrandParent() {
        cout << "  1. GrandParent constructor\n";
    }

    ~GrandParent() {
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

void demonstrateConstructorDestructor() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: CONSTRUCTOR/DESTRUCTOR ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating Child object ---\n";
    cout << "Construction order (base to derived):\n";
    Child child;

    cout << "\n--- Destroying Child object ---\n";
    cout << "Destruction order (derived to base):\n";
}

// ============================================
// EXAMPLE 4: FUNCTION OVERRIDING
// ============================================

class Shape {
protected:
    string color;

public:
    Shape(string c) : color(c) {
        cout << "  Shape created: " << color << "\n";
    }

    // This function can be overridden
    void draw() {
        cout << "  Drawing a generic shape\n";
    }

    void setColor(string c) {
        color = c;
    }
};

class Circle : public Shape {
private:
    double radius;

public:
    Circle(string c, double r) : Shape(c), radius(r) { }

    // Override base class function
    void draw() {
        cout << "  Drawing a " << color << " circle with radius " << radius << "\n";
    }

    // Circle-specific function
    double area() {
        return 3.14159 * radius * radius;
    }
};

class Rectangle : public Shape {
private:
    double width, height;

public:
    Rectangle(string c, double w, double h) : Shape(c), width(w), height(h) { }

    // Override base class function
    void draw() {
        cout << "  Drawing a " << color << " rectangle: "
             << width << "×" << height << "\n";
    }

    // Rectangle-specific function
    double area() {
        return width * height;
    }
};

void demonstrateFunctionOverriding() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: FUNCTION OVERRIDING    ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating shapes ---\n";
    Shape shape("white");
    Circle circle("red", 5.0);
    Rectangle rect("blue", 10.0, 5.0);

    cout << "\n--- Drawing shapes ---\n";
    shape.draw();     // Generic shape
    circle.draw();    // Circle's overridden version
    rect.draw();      // Rectangle's overridden version

    cout << "\n--- Calculating areas ---\n";
    cout << "  Circle area: " << circle.area() << "\n";
    cout << "  Rectangle area: " << rect.area() << "\n";

    cout << "\n💡 Function overriding:\n";
    cout << "   • Derived class provides new implementation\n";
    cout << "   • Same function signature as base\n";
    cout << "   • Without 'virtual', no polymorphism (yet!)\n";
}

// ============================================
// EXAMPLE 5: PROTECTED ACCESS MODIFIER
// ============================================

class BankAccount {
private:
    string accountNumber;  // Only BankAccount can access

protected:
    double balance;        // Derived classes can access

public:
    BankAccount(string accNum, double bal)
        : accountNumber(accNum), balance(bal) { }

    void deposit(double amount) {
        balance += amount;
        cout << "  Deposited $" << amount << ", Balance: $" << balance << "\n";
    }

    double getBalance() const {
        return balance;
    }
};

class SavingsAccount : public BankAccount {
private:
    double interestRate;

public:
    SavingsAccount(string accNum, double bal, double rate)
        : BankAccount(accNum, bal), interestRate(rate) { }

    void addInterest() {
        // Can access protected balance
        double interest = balance * interestRate;
        balance += interest;
        cout << "  Interest added: $" << interest
             << ", New balance: $" << balance << "\n";
    }

    void showDetails() {
        // cout << accountNumber;  // ❌ Error! Private not accessible
        cout << "  Balance: $" << balance << " ✅\n";  // Protected accessible
        cout << "  Interest rate: " << interestRate * 100 << "%\n";
    }
};

void demonstrateProtectedAccess() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: PROTECTED ACCESS       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating savings account ---\n";
    SavingsAccount savings("SA-12345", 1000.0, 0.05);

    cout << "\n--- Operations ---\n";
    savings.deposit(500.0);
    savings.addInterest();

    cout << "\n--- Account details ---\n";
    savings.showDetails();

    cout << "\n💡 Protected members:\n";
    cout << "   • Accessible to derived classes\n";
    cout << "   • Not accessible to outside code\n";
    cout << "   • Balance between private and public\n";
}

// ============================================
// EXAMPLE 6: MULTIPLE INHERITANCE
// ============================================

class Flyer {
protected:
    double maxAltitude;

public:
    Flyer(double alt) : maxAltitude(alt) {
        cout << "  Flyer constructor\n";
    }

    void fly() {
        cout << "  Flying at altitude: " << maxAltitude << "m\n";
    }
};

class Swimmer {
protected:
    double maxDepth;

public:
    Swimmer(double depth) : maxDepth(depth) {
        cout << "  Swimmer constructor\n";
    }

    void swim() {
        cout << "  Swimming at depth: " << maxDepth << "m\n";
    }
};

// Duck inherits from both Flyer and Swimmer
class Duck : public Flyer, public Swimmer {
private:
    string name;

public:
    Duck(string n, double alt, double depth)
        : Flyer(alt), Swimmer(depth), name(n) {
        cout << "  Duck constructor: " << name << "\n";
    }

    void quack() {
        cout << "  " << name << " says: Quack! Quack!\n";
    }

    void showAbilities() {
        cout << "  " << name << " can:\n";
        fly();
        swim();
        quack();
    }
};

void demonstrateMultipleInheritance() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: MULTIPLE INHERITANCE   ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating duck ---\n";
    Duck duck("Donald", 100.0, 5.0);

    cout << "\n--- Duck abilities ---\n";
    duck.showAbilities();

    cout << "\n💡 Multiple inheritance:\n";
    cout << "   • Inherit from multiple base classes\n";
    cout << "   • Duck IS-A Flyer AND IS-A Swimmer\n";
    cout << "   • Can lead to diamond problem (see next)\n";
}

// ============================================
// EXAMPLE 7: DIAMOND PROBLEM
// ============================================

class Device {
protected:
    string name;

public:
    Device(string n) : name(n) {
        cout << "  Device constructor: " << name << "\n";
    }

    void powerOn() {
        cout << "  " << name << " powered on\n";
    }
};

// Phone and Camera both inherit from Device
class Phone : public Device {
public:
    Phone(string n) : Device(n) {
        cout << "  Phone constructor\n";
    }

    void makeCall() {
        cout << "  Making a call\n";
    }
};

class Camera : public Device {
public:
    Camera(string n) : Device(n) {
        cout << "  Camera constructor\n";
    }

    void takePicture() {
        cout << "  Taking a picture\n";
    }
};

// SmartPhone inherits from both Phone and Camera
// This creates TWO copies of Device! (Diamond problem)
class SmartPhone : public Phone, public Camera {
public:
    SmartPhone(string n)
        : Phone(n + "-Phone"), Camera(n + "-Camera") {
        cout << "  SmartPhone constructor\n";
    }

    void useFeatures() {
        makeCall();
        takePicture();
        // powerOn();  // ❌ Ambiguous! Which Device?
        Phone::powerOn();   // Must specify
        Camera::powerOn();  // Or this one
    }
};

void demonstrateDiamondProblem() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 7: DIAMOND PROBLEM        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating smartphone ---\n";
    cout << "Notice: TWO Device constructors called!\n";
    SmartPhone phone("MyPhone");

    cout << "\n--- Using features ---\n";
    phone.useFeatures();

    cout << "\n⚠️  Diamond problem:\n";
    cout << "   • SmartPhone has TWO copies of Device\n";
    cout << "   • Ambiguity when accessing Device members\n";
    cout << "   • Solution: Virtual inheritance (next example)\n";
}

// ============================================
// EXAMPLE 8: VIRTUAL INHERITANCE (Diamond Solution)
// ============================================

class VDevice {
protected:
    string name;

public:
    VDevice(string n) : name(n) {
        cout << "  VDevice constructor: " << name << "\n";
    }

    void powerOn() {
        cout << "  " << name << " powered on\n";
    }
};

// VIRTUAL inheritance
class VPhone : virtual public VDevice {
public:
    VPhone(string n) : VDevice(n) {
        cout << "  VPhone constructor\n";
    }

    void makeCall() {
        cout << "  Making a call\n";
    }
};

// VIRTUAL inheritance
class VCamera : virtual public VDevice {
public:
    VCamera(string n) : VDevice(n) {
        cout << "  VCamera constructor\n";
    }

    void takePicture() {
        cout << "  Taking a picture\n";
    }
};

// Now only ONE copy of VDevice!
class VSmartPhone : public VPhone, public VCamera {
public:
    VSmartPhone(string n)
        : VDevice(n), VPhone(n), VCamera(n) {  // Must call VDevice explicitly
        cout << "  VSmartPhone constructor\n";
    }

    void useFeatures() {
        makeCall();
        takePicture();
        powerOn();  // ✅ No ambiguity now!
    }
};

void demonstrateVirtualInheritance() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 8: VIRTUAL INHERITANCE    ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating smartphone with virtual inheritance ---\n";
    cout << "Notice: Only ONE VDevice constructor called!\n";
    VSmartPhone phone("MyVPhone");

    cout << "\n--- Using features ---\n";
    phone.useFeatures();

    cout << "\n✅ Virtual inheritance solves diamond problem:\n";
    cout << "   • Only ONE copy of VDevice\n";
    cout << "   • No ambiguity\n";
    cout << "   • Most derived class calls base constructor\n";
}

// ============================================
// EXAMPLE 9: REAL-WORLD - EMPLOYEE HIERARCHY
// ============================================

class Employee {
protected:
    string name;
    int employeeId;
    double baseSalary;

public:
    Employee(string n, int id, double salary)
        : name(n), employeeId(id), baseSalary(salary) { }

    virtual ~Employee() { }  // Virtual destructor for polymorphism

    void displayInfo() {
        cout << "  ID: " << employeeId << ", Name: " << name
             << ", Salary: $" << baseSalary << "\n";
    }

    double getSalary() const {
        return baseSalary;
    }
};

class Manager : public Employee {
private:
    int teamSize;
    double bonus;

public:
    Manager(string n, int id, double salary, int team, double b)
        : Employee(n, id, salary), teamSize(team), bonus(b) { }

    void displayInfo() {
        Employee::displayInfo();  // Call base version
        cout << "  Role: Manager, Team size: " << teamSize
             << ", Bonus: $" << bonus << "\n";
    }

    double getTotalCompensation() const {
        return baseSalary + bonus;
    }
};

class Developer : public Employee {
private:
    string programmingLanguage;
    string project;

public:
    Developer(string n, int id, double salary, string lang, string proj)
        : Employee(n, id, salary), programmingLanguage(lang), project(proj) { }

    void displayInfo() {
        Employee::displayInfo();
        cout << "  Role: Developer, Language: " << programmingLanguage
             << ", Project: " << project << "\n";
    }

    void code() {
        cout << "  " << name << " is coding in " << programmingLanguage << "\n";
    }
};

void demonstrateEmployeeHierarchy() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 9: EMPLOYEE HIERARCHY     ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating employees ---\n";
    Manager manager("Alice", 1001, 100000, 5, 20000);
    Developer dev1("Bob", 1002, 80000, "C++", "Database Engine");
    Developer dev2("Charlie", 1003, 85000, "Python", "ML Platform");

    cout << "\n--- Manager info ---\n";
    manager.displayInfo();
    cout << "  Total compensation: $" << manager.getTotalCompensation() << "\n";

    cout << "\n--- Developer 1 info ---\n";
    dev1.displayInfo();
    dev1.code();

    cout << "\n--- Developer 2 info ---\n";
    dev2.displayInfo();
    dev2.code();

    cout << "\n✅ Real-world inheritance:\n";
    cout << "   • Common properties in base (Employee)\n";
    cout << "   • Specialized properties in derived (Manager, Developer)\n";
    cout << "   • Code reuse and organization\n";
}

// ============================================
// EXAMPLE 10: IS-A vs HAS-A
// ============================================

// IS-A relationship (inheritance)
class Vehicle {
protected:
    string brand;
    int year;

public:
    Vehicle(string b, int y) : brand(b), year(y) { }

    void start() {
        cout << "  " << brand << " vehicle starting\n";
    }
};

class Car : public Vehicle {  // Car IS-A Vehicle
private:
    int doors;

public:
    Car(string b, int y, int d) : Vehicle(b, y), doors(d) { }

    void drive() {
        cout << "  Driving " << brand << " car with " << doors << " doors\n";
    }
};

// HAS-A relationship (composition)
class Engine {
private:
    int horsepower;

public:
    Engine(int hp) : horsepower(hp) { }

    void run() {
        cout << "  Engine running: " << horsepower << "hp\n";
    }

    int getHorsepower() const {
        return horsepower;
    }
};

class Motorcycle {  // Motorcycle HAS-A Engine
private:
    string brand;
    Engine engine;  // Composition

public:
    Motorcycle(string b, int hp) : brand(b), engine(hp) { }

    void ride() {
        cout << "  Riding " << brand << " motorcycle\n";
        engine.run();
    }
};

void demonstrateIsAvsHasA() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 10: IS-A vs HAS-A         ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- IS-A relationship (inheritance) ---\n";
    Car car("Toyota", 2024, 4);
    car.start();  // Inherited from Vehicle
    car.drive();  // Car's own method
    cout << "  Car IS-A Vehicle ✅\n";

    cout << "\n--- HAS-A relationship (composition) ---\n";
    Motorcycle bike("Harley", 150);
    bike.ride();
    cout << "  Motorcycle HAS-A Engine ✅\n";

    cout << "\n💡 When to use:\n";
    cout << "   IS-A (inheritance): Natural hierarchy, substitutability\n";
    cout << "   HAS-A (composition): One object contains another\n";
    cout << "\n";
    cout << "   Examples:\n";
    cout << "   • Dog IS-A Animal ✅ (inheritance)\n";
    cout << "   • Car HAS-A Engine ✅ (composition)\n";
    cout << "   • Student IS-A Person ✅ (inheritance)\n";
    cout << "   • Book HAS-A Author ✅ (composition)\n";
}

// ============================================
// MAIN DEMONSTRATION
// ============================================

int main() {
    cout << "========================================\n";
    cout << "INHERITANCE DEMONSTRATION\n";
    cout << "========================================\n";

    demonstrateBasicInheritance();
    demonstrateAccessSpecifiers();
    demonstrateConstructorDestructor();
    demonstrateFunctionOverriding();
    demonstrateProtectedAccess();
    demonstrateMultipleInheritance();
    demonstrateDiamondProblem();
    demonstrateVirtualInheritance();
    demonstrateEmployeeHierarchy();
    demonstrateIsAvsHasA();

    // ============================================
    // Comprehensive Summary
    // ============================================
    cout << "\n========================================\n";
    cout << "WHAT IS INHERITANCE?\n";
    cout << "========================================\n";
    cout << "Inheritance is a mechanism to create new classes\n";
    cout << "from existing classes, promoting code reuse and\n";
    cout << "hierarchical organization.\n";
    cout << "\n";
    cout << "Key concepts:\n";
    cout << "  • Base class (parent/superclass)\n";
    cout << "  • Derived class (child/subclass)\n";
    cout << "  • IS-A relationship\n";
    cout << "  • Code reuse\n";
    cout << "  • Specialization\n";

    cout << "\n========================================\n";
    cout << "BASIC SYNTAX\n";
    cout << "========================================\n";
    cout << "class Base {\n";
    cout << "protected:\n";
    cout << "    int data;\n";
    cout << "public:\n";
    cout << "    Base(int d) : data(d) { }\n";
    cout << "    void baseMethod() { }\n";
    cout << "};\n";
    cout << "\n";
    cout << "class Derived : public Base {\n";
    cout << "private:\n";
    cout << "    int derivedData;\n";
    cout << "public:\n";
    cout << "    // Must call base constructor\n";
    cout << "    Derived(int d, int dd) : Base(d), derivedData(dd) { }\n";
    cout << "    \n";
    cout << "    void derivedMethod() {\n";
    cout << "        baseMethod();  // Can call base methods\n";
    cout << "        data = 10;     // Can access protected members\n";
    cout << "    }\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "TYPES OF INHERITANCE\n";
    cout << "========================================\n";
    cout << "1. SINGLE INHERITANCE:\n";
    cout << "   A → B\n";
    cout << "   class B : public A { };\n";
    cout << "\n";
    cout << "2. MULTIPLE INHERITANCE:\n";
    cout << "   A, B → C\n";
    cout << "   class C : public A, public B { };\n";
    cout << "\n";
    cout << "3. MULTILEVEL INHERITANCE:\n";
    cout << "   A → B → C\n";
    cout << "   class B : public A { };\n";
    cout << "   class C : public B { };\n";
    cout << "\n";
    cout << "4. HIERARCHICAL INHERITANCE:\n";
    cout << "   A → B, A → C\n";
    cout << "   class B : public A { };\n";
    cout << "   class C : public A { };\n";
    cout << "\n";
    cout << "5. HYBRID INHERITANCE:\n";
    cout << "   Combination of above\n";

    cout << "\n========================================\n";
    cout << "ACCESS SPECIFIERS IN INHERITANCE\n";
    cout << "========================================\n";
    cout << "PUBLIC INHERITANCE (most common):\n";
    cout << "class Derived : public Base { };\n";
    cout << "  • Base's public    → Derived's public\n";
    cout << "  • Base's protected → Derived's protected\n";
    cout << "  • Base's private   → Not accessible\n";
    cout << "  • IS-A relationship\n";
    cout << "\n";
    cout << "PROTECTED INHERITANCE (rarely used):\n";
    cout << "class Derived : protected Base { };\n";
    cout << "  • Base's public    → Derived's protected\n";
    cout << "  • Base's protected → Derived's protected\n";
    cout << "  • Base's private   → Not accessible\n";
    cout << "\n";
    cout << "PRIVATE INHERITANCE (composition alternative):\n";
    cout << "class Derived : private Base { };\n";
    cout << "  • Base's public    → Derived's private\n";
    cout << "  • Base's protected → Derived's private\n";
    cout << "  • Base's private   → Not accessible\n";
    cout << "  • HAS-A relationship (use composition instead)\n";

    cout << "\n========================================\n";
    cout << "WHAT IS INHERITED?\n";
    cout << "========================================\n";
    cout << "✅ INHERITED:\n";
    cout << "  • Public members (as public/protected/private)\n";
    cout << "  • Protected members (as protected/private)\n";
    cout << "  • Member functions\n";
    cout << "  • Static members\n";
    cout << "\n";
    cout << "❌ NOT INHERITED (but called):\n";
    cout << "  • Constructors (must be called explicitly)\n";
    cout << "  • Destructors (called automatically)\n";
    cout << "  • Copy constructors\n";
    cout << "  • Assignment operators\n";
    cout << "\n";
    cout << "❌ NEVER ACCESSIBLE:\n";
    cout << "  • Private members of base class\n";

    cout << "\n========================================\n";
    cout << "CONSTRUCTOR/DESTRUCTOR ORDER\n";
    cout << "========================================\n";
    cout << "CONSTRUCTION (base to derived):\n";
    cout << "  1. Base class constructor\n";
    cout << "  2. Member objects constructors\n";
    cout << "  3. Derived class constructor\n";
    cout << "\n";
    cout << "DESTRUCTION (derived to base - reverse):\n";
    cout << "  1. Derived class destructor\n";
    cout << "  2. Member objects destructors\n";
    cout << "  3. Base class destructor\n";
    cout << "\n";
    cout << "Example:\n";
    cout << "class A {\n";
    cout << "    A() { cout << \"A constructor\"; }\n";
    cout << "    ~A() { cout << \"A destructor\"; }\n";
    cout << "};\n";
    cout << "class B : public A {\n";
    cout << "    B() { cout << \"B constructor\"; }  // Calls A() first\n";
    cout << "    ~B() { cout << \"B destructor\"; }  // Calls ~A() after\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "PROTECTED ACCESS MODIFIER\n";
    cout << "========================================\n";
    cout << "protected members:\n";
    cout << "  • NOT accessible to outside code\n";
    cout << "  • Accessible to derived classes\n";
    cout << "  • Useful for data that derived classes need\n";
    cout << "\n";
    cout << "Example:\n";
    cout << "class Base {\n";
    cout << "private:\n";
    cout << "    int secret;        // Only Base can access\n";
    cout << "protected:\n";
    cout << "    int shared;        // Base and derived can access\n";
    cout << "public:\n";
    cout << "    int publicData;    // Everyone can access\n";
    cout << "};\n";
    cout << "\n";
    cout << "class Derived : public Base {\n";
    cout << "    void func() {\n";
    cout << "        // secret = 10;     // ❌ Error! Private\n";
    cout << "        shared = 20;        // ✅ OK! Protected\n";
    cout << "        publicData = 30;    // ✅ OK! Public\n";
    cout << "    }\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "MULTIPLE INHERITANCE\n";
    cout << "========================================\n";
    cout << "Syntax:\n";
    cout << "class Derived : public Base1, public Base2 {\n";
    cout << "    Derived() : Base1(), Base2() { }\n";
    cout << "};\n";
    cout << "\n";
    cout << "Advantages:\n";
    cout << "  • Inherit from multiple classes\n";
    cout << "  • Combine functionality\n";
    cout << "\n";
    cout << "Disadvantages:\n";
    cout << "  • Complexity\n";
    cout << "  • Diamond problem\n";
    cout << "  • Ambiguity issues\n";
    cout << "\n";
    cout << "⚠️  Use sparingly! Consider composition instead.\n";

    cout << "\n========================================\n";
    cout << "DIAMOND PROBLEM\n";
    cout << "========================================\n";
    cout << "Problem:\n";
    cout << "       A\n";
    cout << "      / \\\n";
    cout << "     B   C\n";
    cout << "      \\ /\n";
    cout << "       D\n";
    cout << "\n";
    cout << "D inherits from B and C,\n";
    cout << "B and C both inherit from A.\n";
    cout << "Result: D has TWO copies of A!\n";
    cout << "\n";
    cout << "Solution: VIRTUAL INHERITANCE\n";
    cout << "class B : virtual public A { };\n";
    cout << "class C : virtual public A { };\n";
    cout << "class D : public B, public C { };\n";
    cout << "\n";
    cout << "Now D has only ONE copy of A!\n";

    cout << "\n========================================\n";
    cout << "FUNCTION OVERRIDING\n";
    cout << "========================================\n";
    cout << "Derived class can override base class functions:\n";
    cout << "\n";
    cout << "class Base {\n";
    cout << "public:\n";
    cout << "    void display() {\n";
    cout << "        cout << \"Base display\";\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "class Derived : public Base {\n";
    cout << "public:\n";
    cout << "    void display() {  // Overrides Base::display()\n";
    cout << "        cout << \"Derived display\";\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "To call base version from derived:\n";
    cout << "void Derived::display() {\n";
    cout << "    Base::display();  // Call base version\n";
    cout << "    cout << \"Additional\";\n";
    cout << "}\n";

    cout << "\n========================================\n";
    cout << "IS-A vs HAS-A\n";
    cout << "========================================\n";
    cout << "IS-A relationship (INHERITANCE):\n";
    cout << "  • \"Dog IS-A Animal\"\n";
    cout << "  • \"Car IS-A Vehicle\"\n";
    cout << "  • Use public inheritance\n";
    cout << "  • Substitutability principle\n";
    cout << "\n";
    cout << "HAS-A relationship (COMPOSITION):\n";
    cout << "  • \"Car HAS-A Engine\"\n";
    cout << "  • \"Book HAS-A Author\"\n";
    cout << "  • Use member objects\n";
    cout << "  • More flexible\n";
    cout << "\n";
    cout << "Example:\n";
    cout << "// IS-A (inheritance)\n";
    cout << "class Dog : public Animal { };\n";
    cout << "\n";
    cout << "// HAS-A (composition)\n";
    cout << "class Car {\n";
    cout << "    Engine engine;  // Car HAS-A Engine\n";
    cout << "};\n";
    cout << "\n";
    cout << "General rule: Prefer composition over inheritance!\n";

    cout << "\n========================================\n";
    cout << "BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "✅ DO:\n";
    cout << "  • Use public inheritance for IS-A relationships\n";
    cout << "  • Make base destructors virtual (for polymorphism)\n";
    cout << "  • Call base constructors explicitly\n";
    cout << "  • Use protected for data derived classes need\n";
    cout << "  • Keep inheritance hierarchies shallow\n";
    cout << "  • Override functions with same signature\n";
    cout << "  • Document inheritance relationships\n";
    cout << "\n";
    cout << "❌ DON'T:\n";
    cout << "  • Overuse inheritance (favor composition)\n";
    cout << "  • Use multiple inheritance without good reason\n";
    cout << "  • Forget to call base constructors\n";
    cout << "  • Make everything public just to inherit it\n";
    cout << "  • Create deep inheritance hierarchies\n";
    cout << "  • Use private/protected inheritance (use composition)\n";
    cout << "  • Ignore the diamond problem\n";

    cout << "\n========================================\n";
    cout << "COMMON MISTAKES\n";
    cout << "========================================\n";
    cout << "❌ Mistake 1: Not calling base constructor\n";
    cout << "class Derived : public Base {\n";
    cout << "    Derived() { }  // ❌ Base() called implicitly\n";
    cout << "};\n";
    cout << "Fix: Derived() : Base(args) { }\n";
    cout << "\n";
    cout << "❌ Mistake 2: Trying to access private base members\n";
    cout << "class Derived : public Base {\n";
    cout << "    void func() {\n";
    cout << "        privateBaseMember = 10;  // ❌ Error!\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "Fix: Use protected or public interface\n";
    cout << "\n";
    cout << "❌ Mistake 3: Forgetting virtual destructor\n";
    cout << "class Base {\n";
    cout << "    ~Base() { }  // ❌ Not virtual\n";
    cout << "};\n";
    cout << "Fix: virtual ~Base() { }\n";

    cout << "\n========================================\n";
    cout << "QUICK REFERENCE\n";
    cout << "========================================\n";
    cout << "// Basic inheritance\n";
    cout << "class Base {\n";
    cout << "protected:\n";
    cout << "    int data;\n";
    cout << "public:\n";
    cout << "    Base(int d) : data(d) { }\n";
    cout << "    virtual ~Base() { }\n";
    cout << "    void baseFunc() { }\n";
    cout << "};\n";
    cout << "\n";
    cout << "class Derived : public Base {\n";
    cout << "private:\n";
    cout << "    int derivedData;\n";
    cout << "public:\n";
    cout << "    Derived(int d, int dd) : Base(d), derivedData(dd) { }\n";
    cout << "    void derivedFunc() { }\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Multiple inheritance\n";
    cout << "class Multi : public Base1, public Base2 {\n";
    cout << "    Multi() : Base1(), Base2() { }\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Virtual inheritance (diamond problem)\n";
    cout << "class B : virtual public A { };\n";
    cout << "class C : virtual public A { };\n";
    cout << "class D : public B, public C { };\n";

    cout << "\n========================================\n";
    cout << "KEY TAKEAWAYS\n";
    cout << "========================================\n";
    cout << "1. Inheritance creates IS-A relationships\n";
    cout << "2. Derived class inherits base class members\n";
    cout << "3. Public inheritance is most common\n";
    cout << "4. Protected members accessible to derived classes\n";
    cout << "5. Constructors called base-to-derived\n";
    cout << "6. Destructors called derived-to-base\n";
    cout << "7. Use virtual inheritance for diamond problem\n";
    cout << "8. Prefer composition over inheritance\n";
    cout << "9. Make base destructors virtual\n";
    cout << "10. Keep hierarchies simple and shallow\n";

    return 0;
}
