#include <print>
#include <string>
#include <type_traits>

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
    std::string name;
    int age;

public:
    Animal(std::string n, int a) : name(n), age(a) {
        std::println("  Animal constructor: {}", name);
    }

    ~Animal() {
        std::println("  Animal destructor: {}", name);
    }

    void eat() {
        std::println("  {} is eating", name);
    }

    void sleep() {
        std::println("  {} is sleeping", name);
    }

    void displayInfo() {
        std::println("  Name: {}, Age: {}", name, age);
    }
};

// Dog inherits from Animal
class Dog : public Animal {  // public inheritance
private:
    std::string breed;

public:
    // Constructor must call base constructor
    Dog(std::string n, int a, std::string b) : Animal(n, a), breed(b) {
        std::println("  Dog constructor: {} ({})", name, breed);
    }

    ~Dog() {
        std::println("  Dog destructor: {}", name);
    }

    // Dog-specific method
    void bark() {
        std::println("  {} says: Woof! Woof!", name);
    }

    // Dog can access protected members from Animal
    void displayDogInfo() {
        std::println("  Dog: {}, Age: {}, Breed: {}", name, age, breed);
    }
};

void demonstrateBasicInheritance() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: BASIC INHERITANCE      ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating Dog object ---");
    Dog dog("Buddy", 3, "Golden Retriever");

    std::println("");
    std::println("--- Using inherited methods ---");
    dog.eat();      // Inherited from Animal
    dog.sleep();    // Inherited from Animal

    std::println("");
    std::println("--- Using Dog-specific methods ---");
    dog.bark();     // Dog's own method

    std::println("");
    std::println("--- Displaying info ---");
    dog.displayInfo();     // Inherited from Animal
    dog.displayDogInfo();  // Dog's own method

    std::println("");
    std::println("--- Destruction order (reverse of construction) ---");
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
        std::println("  Base - private: {}, protected: {}, public: {}", privateData, protectedData, publicData);
    }
};

// Public inheritance (most common)
class PublicDerived : public Base {
public:
    void showAccess() {
        std::println("  PublicDerived can access:");
        // std::println("{}", privateData);     // ❌ Error! Private not accessible
        std::println("    protected: {} ✅", protectedData);
        std::println("    public: {} ✅", publicData);
    }
};

// Protected inheritance (rarely used)
class ProtectedDerived : protected Base {
public:
    void showAccess() {
        std::println("  ProtectedDerived can access:");
        // std::println("{}", privateData);     // ❌ Error! Private not accessible
        std::println("    protected: {} ✅", protectedData);
        std::println("    public: {} ✅", publicData);
    }
};

// Private inheritance (composition alternative)
class PrivateDerived : private Base {
public:
    void showAccess() {
        std::println("  PrivateDerived can access:");
        // std::println("{}", privateData);     // ❌ Error! Private not accessible
        std::println("    protected: {} ✅", protectedData);
        std::println("    public: {} ✅", publicData);
    }
};

void demonstrateAccessSpecifiers() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: ACCESS SPECIFIERS      ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Public inheritance ---");
    PublicDerived pub;
    pub.showAccess();
    pub.publicData = 100;  // ✅ Public members accessible
    pub.showBase();        // ✅ Public methods accessible

    std::println("");
    std::println("--- Protected inheritance ---");
    ProtectedDerived prot;
    prot.showAccess();
    // prot.publicData = 100;  // ❌ Error! Now protected
    // prot.showBase();        // ❌ Error! Now protected

    std::println("");
    std::println("--- Private inheritance ---");
    PrivateDerived priv;
    priv.showAccess();
    // priv.publicData = 100;  // ❌ Error! Now private
    // priv.showBase();        // ❌ Error! Now private
    std::println("\n--- Which pointer conversions to Base* exist outside the class? ---");
    std::println("  PublicDerived*    -> Base*: {}", std::is_convertible_v<PublicDerived*, Base*>);
    std::println("  ProtectedDerived* -> Base*: {}", std::is_convertible_v<ProtectedDerived*, Base*>);
    std::println("  PrivateDerived*   -> Base*: {}", std::is_convertible_v<PrivateDerived*, Base*>);
}

// ============================================
// EXAMPLE 3: CONSTRUCTOR AND DESTRUCTOR CALLS
// ============================================

class GrandParent {
public:
    GrandParent() {
        std::println("  1. GrandParent constructor");
    }

    ~GrandParent() {
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

void demonstrateConstructorDestructor() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: CONSTRUCTOR/DESTRUCTOR ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating Child object ---");
    std::println("Construction order (base to derived):");
    Child child;

    std::println("");
    std::println("--- Destroying Child object ---");
    std::println("Destruction order (derived to base):");
}

// ============================================
// EXAMPLE 4: FUNCTION HIDING (draw() is not virtual)
// ============================================

class Shape {
protected:
    std::string color;

public:
    explicit Shape(std::string c) : color(c) {
        std::println("  Shape created: {}", color);
    }

    // Not virtual: derived classes can only HIDE this function
    void draw() {
        std::println("  Drawing a generic shape");
    }

    void setColor(std::string c) {
        color = c;
    }
};

class Circle : public Shape {
private:
    double radius;

public:
    Circle(std::string c, double r) : Shape(c), radius(r) { }

    // Hides Shape::draw (no virtual, so this is hiding, not overriding)
    void draw() {
        std::println("  Drawing a {} circle with radius {:g}", color, radius);
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
    Rectangle(std::string c, double w, double h) : Shape(c), width(w), height(h) { }

    // Hides Shape::draw (no virtual, so this is hiding, not overriding)
    void draw() {
        std::println("  Drawing a {} rectangle: {:g}×{:g}", color, width, height);
    }

    // Rectangle-specific function
    double area() {
        return width * height;
    }
};

void demonstrateFunctionOverriding() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: FUNCTION HIDING        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating shapes ---");
    Shape shape("white");
    Circle circle("red", 5.0);
    Rectangle rect("blue", 10.0, 5.0);

    std::println("");
    std::println("--- Drawing shapes ---");
    shape.draw();     // Generic shape
    circle.draw();    // Circle's overridden version
    rect.draw();      // Rectangle's overridden version

    std::println("");
    std::println("--- Calculating areas ---");
    std::println("  Circle area: {:g}", circle.area());
    std::println("  Rectangle area: {:g}", rect.area());
    std::println("\n--- Through a Shape pointer (draw is not virtual) ---");
    Shape* ptr = &circle;
    ptr->draw();  // Shape::draw - the static type decides
}

// ============================================
// EXAMPLE 5: PROTECTED ACCESS MODIFIER
// ============================================

class BankAccount {
private:
    std::string accountNumber;  // Only BankAccount can access

protected:
    double balance;        // Derived classes can access

public:
    BankAccount(std::string accNum, double bal)
        : accountNumber(accNum), balance(bal) { }

    void deposit(double amount) {
        balance += amount;
        std::println("  Deposited ${:g}, Balance: ${:g}", amount, balance);
    }

    double getBalance() const {
        return balance;
    }
};

class SavingsAccount : public BankAccount {
private:
    double interestRate;

public:
    SavingsAccount(std::string accNum, double bal, double rate)
        : BankAccount(accNum, bal), interestRate(rate) { }

    void addInterest() {
        // Can access protected balance
        double interest = balance * interestRate;
        balance += interest;
        std::println("  Interest added: ${:g}, New balance: ${:g}", interest, balance);
    }

    void showDetails() {
        // std::println("{}", accountNumber);  // ❌ Error! Private not accessible
        std::println("  Balance: ${:g} ✅", balance);  // Protected accessible
        std::println("  Interest rate: {:g}%", interestRate * 100);
    }
};

void demonstrateProtectedAccess() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 5: PROTECTED ACCESS       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating savings account ---");
    SavingsAccount savings("SA-12345", 1000.0, 0.05);

    std::println("");
    std::println("--- Operations ---");
    savings.deposit(500.0);
    savings.addInterest();

    std::println("");
    std::println("--- Account details ---");
    savings.showDetails();
}

// ============================================
// EXAMPLE 6: MULTIPLE INHERITANCE
// ============================================

class Flyer {
protected:
    double maxAltitude;

public:
    explicit Flyer(double alt) : maxAltitude(alt) {
        std::println("  Flyer constructor");
    }

    void fly() {
        std::println("  Flying at altitude: {:g}m", maxAltitude);
    }
};

class Swimmer {
protected:
    double maxDepth;

public:
    explicit Swimmer(double depth) : maxDepth(depth) {
        std::println("  Swimmer constructor");
    }

    void swim() {
        std::println("  Swimming at depth: {:g}m", maxDepth);
    }
};

// Duck inherits from both Flyer and Swimmer
class Duck : public Flyer, public Swimmer {
private:
    std::string name;

public:
    Duck(std::string n, double alt, double depth)
        : Flyer(alt), Swimmer(depth), name(n) {
        std::println("  Duck constructor: {}", name);
    }

    void quack() {
        std::println("  {} says: Quack! Quack!", name);
    }

    void showAbilities() {
        std::println("  {} can:", name);
        fly();
        swim();
        quack();
    }
};

void demonstrateMultipleInheritance() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 6: MULTIPLE INHERITANCE   ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating duck ---");
    Duck duck("Donald", 100.0, 5.0);

    std::println("");
    std::println("--- Duck abilities ---");
    duck.showAbilities();
}

// ============================================
// EXAMPLE 7: DIAMOND PROBLEM
// ============================================

class Device {
protected:
    std::string name;

public:
    explicit Device(std::string n) : name(n) {
        std::println("  Device constructor: {}", name);
    }

    void powerOn() {
        std::println("  {} powered on", name);
    }
};

// Phone and Camera both inherit from Device
class Phone : public Device {
public:
    explicit Phone(std::string n) : Device(n) {
        std::println("  Phone constructor");
    }

    void makeCall() {
        std::println("  Making a call");
    }
};

class Camera : public Device {
public:
    explicit Camera(std::string n) : Device(n) {
        std::println("  Camera constructor");
    }

    void takePicture() {
        std::println("  Taking a picture");
    }
};

// SmartPhone inherits from both Phone and Camera
// This creates TWO copies of Device! (Diamond problem)
class SmartPhone : public Phone, public Camera {
public:
    explicit SmartPhone(std::string n)
        : Phone(n + "-Phone"), Camera(n + "-Camera") {
        std::println("  SmartPhone constructor");
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
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 7: DIAMOND PROBLEM        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating smartphone ---");
    std::println("Notice: TWO Device constructors called!");
    SmartPhone phone("MyPhone");

    std::println("");
    std::println("--- Using features ---");
    phone.useFeatures();
    std::println("\n--- Two Device subobjects ---");
    std::println("  SmartPhone* converts to Device*: {} (ambiguous)", std::is_convertible_v<SmartPhone*, Device*>);
    std::println("  sizeof(SmartPhone): {} (GCC x86-64)", sizeof(SmartPhone));
}

// ============================================
// EXAMPLE 8: VIRTUAL INHERITANCE (Diamond Solution)
// ============================================

class VDevice {
protected:
    std::string name;

public:
    explicit VDevice(std::string n) : name(n) {
        std::println("  VDevice constructor: {}", name);
    }

    void powerOn() {
        std::println("  {} powered on", name);
    }
};

// VIRTUAL inheritance
class VPhone : virtual public VDevice {
public:
    explicit VPhone(std::string n) : VDevice(n) {
        std::println("  VPhone constructor");
    }

    void makeCall() {
        std::println("  Making a call");
    }
};

// VIRTUAL inheritance
class VCamera : virtual public VDevice {
public:
    explicit VCamera(std::string n) : VDevice(n) {
        std::println("  VCamera constructor");
    }

    void takePicture() {
        std::println("  Taking a picture");
    }
};

// Now only ONE copy of VDevice!
class VSmartPhone : public VPhone, public VCamera {
public:
    explicit VSmartPhone(std::string n)
        : VDevice(n), VPhone(n), VCamera(n) {  // Must call VDevice explicitly
        std::println("  VSmartPhone constructor");
    }

    void useFeatures() {
        makeCall();
        takePicture();
        powerOn();  // ✅ No ambiguity now!
    }
};

void demonstrateVirtualInheritance() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 8: VIRTUAL INHERITANCE    ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating smartphone with virtual inheritance ---");
    std::println("Notice: Only ONE VDevice constructor called!");
    VSmartPhone phone("MyVPhone");

    std::println("");
    std::println("--- Using features ---");
    phone.useFeatures();
    std::println("\n--- One shared VDevice ---");
    std::println("  VSmartPhone* converts to VDevice*: {}", std::is_convertible_v<VSmartPhone*, VDevice*>);
    std::println("  sizeof(VSmartPhone): {} (GCC x86-64)", sizeof(VSmartPhone));
}

// ============================================
// EXAMPLE 9: REAL-WORLD - EMPLOYEE HIERARCHY
// ============================================

class Employee {
protected:
    std::string name;
    int employeeId;
    double baseSalary;

public:
    Employee(std::string n, int id, double salary)
        : name(n), employeeId(id), baseSalary(salary) { }

    virtual ~Employee() { }  // Virtual destructor for polymorphism

    virtual void displayInfo() {
        std::println("  ID: {}, Name: {}, Salary: ${:g}", employeeId, name, baseSalary);
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
    Manager(std::string n, int id, double salary, int team, double b)
        : Employee(n, id, salary), teamSize(team), bonus(b) { }

    void displayInfo() override {
        Employee::displayInfo();  // Call base version
        std::println("  Role: Manager, Team size: {}, Bonus: ${:g}", teamSize, bonus);
    }

    double getTotalCompensation() const {
        return baseSalary + bonus;
    }
};

class Developer : public Employee {
private:
    std::string programmingLanguage;
    std::string project;

public:
    Developer(std::string n, int id, double salary, std::string lang, std::string proj)
        : Employee(n, id, salary), programmingLanguage(lang), project(proj) { }

    void displayInfo() override {
        Employee::displayInfo();
        std::println("  Role: Developer, Language: {}, Project: {}", programmingLanguage, project);
    }

    void code() {
        std::println("  {} is coding in {}", name, programmingLanguage);
    }
};

void demonstrateEmployeeHierarchy() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 9: EMPLOYEE HIERARCHY     ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating employees ---");
    Manager manager("Alice", 1001, 100000, 5, 20000);
    Developer dev1("Bob", 1002, 80000, "C++", "Database Engine");
    Developer dev2("Charlie", 1003, 85000, "Python", "ML Platform");

    std::println("");
    std::println("--- Manager info ---");
    manager.displayInfo();
    std::println("  Total compensation: ${:g}", manager.getTotalCompensation());

    std::println("");
    std::println("--- Developer 1 info ---");
    dev1.displayInfo();
    dev1.code();

    std::println("");
    std::println("--- Developer 2 info ---");
    dev2.displayInfo();
    dev2.code();
    std::println("\n--- Through Employee pointers (displayInfo is virtual) ---");
    Employee* staff[] = {&manager, &dev1};
    for (Employee* e : staff) {
        e->displayInfo();
    }
}

// ============================================
// EXAMPLE 10: IS-A vs HAS-A
// ============================================

// IS-A relationship (inheritance)
class Vehicle {
protected:
    std::string brand;
    int year;

public:
    Vehicle(std::string b, int y) : brand(b), year(y) { }

    void start() {
        std::println("  {} vehicle starting", brand);
    }
};

class Car : public Vehicle {  // Car IS-A Vehicle
private:
    int doors;

public:
    Car(std::string b, int y, int d) : Vehicle(b, y), doors(d) { }

    void drive() {
        std::println("  Driving {} car with {} doors", brand, doors);
    }
};

// HAS-A relationship (composition)
class Engine {
private:
    int horsepower;

public:
    explicit Engine(int hp) : horsepower(hp) { }

    void run() {
        std::println("  Engine running: {}hp", horsepower);
    }

    int getHorsepower() const {
        return horsepower;
    }
};

class Motorcycle {  // Motorcycle HAS-A Engine
private:
    std::string brand;
    Engine engine;  // Composition

public:
    Motorcycle(std::string b, int hp) : brand(b), engine(hp) { }

    void ride() {
        std::println("  Riding {} motorcycle", brand);
        engine.run();
    }
};

void demonstrateIsAvsHasA() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 10: IS-A vs HAS-A         ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- IS-A relationship (inheritance) ---");
    Car car("Toyota", 2024, 4);
    car.start();  // Inherited from Vehicle
    car.drive();  // Car's own method
    std::println("  Car IS-A Vehicle ✅");

    std::println("");
    std::println("--- HAS-A relationship (composition) ---");
    Motorcycle bike("Harley", 150);
    bike.ride();
    std::println("  Motorcycle HAS-A Engine ✅");
    std::println("\n--- Checking the relationships at compile time ---");
    std::println("  Car* converts to Vehicle*:        {}", std::is_convertible_v<Car*, Vehicle*>);
    std::println("  Motorcycle* converts to Engine*:  {}", std::is_convertible_v<Motorcycle*, Engine*>);
}

// ============================================
// MAIN DEMONSTRATION
// ============================================

int main() {
    std::println("========================================");
    std::println("INHERITANCE DEMONSTRATION");
    std::println("========================================");

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

    return 0;
}

