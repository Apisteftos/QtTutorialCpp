#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cmath>
#include <algorithm>


using namespace std;

// ============================================
// WHAT ARE ABSTRACT CLASSES?
// ============================================

/*
 * ABSTRACT CLASSES:
 * - Classes that cannot be instantiated
 * - Have at least one pure virtual function
 * - Used as base classes for derived classes
 * - Define interfaces/contracts
 *
 * PURE VIRTUAL FUNCTION:
 * - Declared with = 0
 * - virtual ReturnType functionName() = 0;
 * - No implementation (usually)
 * - Derived classes MUST implement
 *
 * WHY ABSTRACT CLASSES?
 * - Define common interface
 * - Enforce implementation in derived classes
 * - Achieve polymorphism
 * - Separate interface from implementation
 * - Enable design by contract
 *
 * RULES:
 * - Cannot create objects of abstract class
 * - Can have pointers/references to abstract class
 * - Can have constructors (called by derived)
 * - Can have data members
 * - Can have regular methods
 * - Should have virtual destructor
 *
 * CONCRETE CLASS:
 * - Implements all pure virtual functions
 * - Can be instantiated
 * - Provides actual implementation
 */

// ============================================
// EXAMPLE 1: BASIC ABSTRACT CLASS
// ============================================

// Abstract class - has pure virtual function
class Shape {
protected:
    string name;
    string color;

public:
    // Constructor (can have in abstract class)
    Shape(string n, string c) : name(n), color(c) {
        cout << "  Shape constructor: " << name << "\n";
    }

    // Virtual destructor (IMPORTANT!)
    virtual ~Shape() {
        cout << "  Shape destructor: " << name << "\n";
    }

    // Pure virtual functions - MUST be implemented by derived classes
    virtual double area() const = 0;
    virtual double perimeter() const = 0;

    // Regular virtual function - can be overridden
    virtual void draw() const {
        cout << "  Drawing " << color << " " << name << "\n";
    }

    // Non-virtual function - cannot be overridden
    void setColor(string c) {
        color = c;
    }

    string getName() const {
        return name;
    }
};

class Circle : public Shape {
private:
    double radius;

public:
    Circle(string c, double r) : Shape("Circle", c), radius(r) {
        cout << "  Circle constructor\n";
    }

    ~Circle() {
        cout << "  Circle destructor\n";
    }

    // MUST implement pure virtual functions
    double area() const override {
        return M_PI * radius * radius;
    }

    double perimeter() const override {
        return 2 * M_PI * radius;
    }

    // Can override regular virtual functions
    void draw() const override {
        cout << "  Drawing " << color << " circle with radius " << radius << "\n";
    }
};

class Rectangle : public Shape {
private:
    double width, height;

public:
    Rectangle(string c, double w, double h)
        : Shape("Rectangle", c), width(w), height(h) {
        cout << "  Rectangle constructor\n";
    }

    ~Rectangle() {
        cout << "  Rectangle destructor\n";
    }

    // MUST implement pure virtual functions
    double area() const override {
        return width * height;
    }

    double perimeter() const override {
        return 2 * (width + height);
    }

    void draw() const override {
        cout << "  Drawing " << color << " rectangle: "
             << width << "×" << height << "\n";
    }
};

void demonstrateBasicAbstract() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: BASIC ABSTRACT CLASS  ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    // Shape shape("Generic", "white");  // ❌ Error! Cannot instantiate

    cout << "--- Creating concrete shapes ---\n";
    Circle circle("red", 5.0);
    Rectangle rect("blue", 10.0, 5.0);

    cout << "\n--- Using abstract class pointers ---\n";
    Shape* shapes[2];
    shapes[0] = &circle;
    shapes[1] = &rect;

    for (int i = 0; i < 2; i++) {
        shapes[i]->draw();
        cout << "  Area: " << shapes[i]->area() << "\n";
        cout << "  Perimeter: " << shapes[i]->perimeter() << "\n\n";
    }

    cout << "✅ Abstract classes:\n";
    cout << "   • Cannot be instantiated\n";
    cout << "   • Can have pointers/references\n";
    cout << "   • Define interface contract\n";
    cout << "   • Force derived classes to implement\n";

    cout << "\n--- Destruction order ---\n";
}

// ============================================
// EXAMPLE 2: PURE INTERFACE (No Implementation)
// ============================================

// Pure interface - only pure virtual functions
class IDrawable {
public:
    virtual void draw() const = 0;
    virtual void resize(double factor) = 0;
    virtual ~IDrawable() = default;
};

class IPrintable {
public:
    virtual void print() const = 0;
    virtual void printToFile(const string& filename) const = 0;
    virtual ~IPrintable() = default;
};

// Implements both interfaces
class Document : public IDrawable, public IPrintable {
private:
    string title;
    string content;

public:
    Document(string t, string c) : title(t), content(c) { }

    // Implement IDrawable
    void draw() const override {
        cout << "  Drawing document: " << title << "\n";
        cout << "  Content: " << content << "\n";
    }

    void resize(double factor) override {
        cout << "  Resizing document by factor " << factor << "\n";
    }

    // Implement IPrintable
    void print() const override {
        cout << "  Printing document: " << title << "\n";
        cout << "  " << content << "\n";
    }

    void printToFile(const string& filename) const override {
        cout << "  Printing to file: " << filename << "\n";
        cout << "  Title: " << title << "\n";
    }
};

void demonstratePureInterface() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: PURE INTERFACE         ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Document doc("Report", "This is a business report.");

    cout << "--- Using as IDrawable ---\n";
    IDrawable* drawable = &doc;
    drawable->draw();
    drawable->resize(1.5);

    cout << "\n--- Using as IPrintable ---\n";
    IPrintable* printable = &doc;
    printable->print();
    printable->printToFile("report.pdf");

    cout << "\n✅ Pure interfaces:\n";
    cout << "   • Only pure virtual functions\n";
    cout << "   • No data members (usually)\n";
    cout << "   • Define behavior contract\n";
    cout << "   • Enable multiple inheritance of interfaces\n";
}

// ============================================
// EXAMPLE 3: ABSTRACT CLASS WITH IMPLEMENTATION
// ============================================

// Abstract class can have implementation for some methods
class Animal {
protected:
    string name;
    int age;

public:
    Animal(string n, int a) : name(n), age(a) { }

    virtual ~Animal() = default;

    // Pure virtual - must implement
    virtual void makeSound() const = 0;
    virtual void move() const = 0;

    // Regular virtual - can override
    virtual void sleep() const {
        cout << "  " << name << " is sleeping... Zzz\n";
    }

    // Non-virtual - cannot override
    void displayInfo() const {
        cout << "  Name: " << name << ", Age: " << age << "\n";
    }
};

class Dog : public Animal {
public:
    Dog(string n, int a) : Animal(n, a) { }

    void makeSound() const override {
        cout << "  " << name << " says: Woof! Woof!\n";
    }

    void move() const override {
        cout << "  " << name << " runs on four legs\n";
    }

    // Override sleep with dog-specific behavior
    void sleep() const override {
        cout << "  " << name << " curls up and sleeps... Zzz\n";
    }
};

class Bird : public Animal {
public:
    Bird(string n, int a) : Animal(n, a) { }

    void makeSound() const override {
        cout << "  " << name << " says: Tweet! Tweet!\n";
    }

    void move() const override {
        cout << "  " << name << " flies in the sky\n";
    }

    // Uses default sleep implementation
};

void demonstrateAbstractWithImplementation() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: ABSTRACT + IMPL        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Dog dog("Buddy", 3);
    Bird bird("Tweety", 1);

    cout << "--- Dog ---\n";
    dog.displayInfo();
    dog.makeSound();
    dog.move();
    dog.sleep();

    cout << "\n--- Bird ---\n";
    bird.displayInfo();
    bird.makeSound();
    bird.move();
    bird.sleep();  // Uses default implementation

    cout << "\n✅ Abstract class can have:\n";
    cout << "   • Pure virtual functions (must implement)\n";
    cout << "   • Virtual functions (can override)\n";
    cout << "   • Non-virtual functions (cannot override)\n";
    cout << "   • Default implementations\n";
}

// ============================================
// EXAMPLE 4: PARTIALLY ABSTRACT CLASS
// ============================================

// Abstract base
class Vehicle {
public:
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void refuel() = 0;
    virtual ~Vehicle() = default;
};

// Still abstract - implements some but not all
class LandVehicle : public Vehicle {
protected:
    int wheels;

public:
    LandVehicle(int w) : wheels(w) { }

    // Implements some pure virtual functions
    void start() override {
        cout << "  Starting land vehicle...\n";
    }

    void stop() override {
        cout << "  Stopping land vehicle...\n";
    }

    // refuel() still pure virtual - still abstract!

    void displayWheels() const {
        cout << "  Number of wheels: " << wheels << "\n";
    }
};

// Concrete - implements all pure virtual functions
class Car : public LandVehicle {
public:
    Car() : LandVehicle(4) { }

    void refuel() override {
        cout << "  Refueling car with gasoline...\n";
    }
};

class Bicycle : public LandVehicle {
public:
    Bicycle() : LandVehicle(2) { }

    void refuel() override {
        cout << "  Bicycle doesn't need fuel - human powered!\n";
    }
};

void demonstratePartiallyAbstract() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: PARTIALLY ABSTRACT     ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    // Vehicle v;        // ❌ Error! Abstract
    // LandVehicle lv;   // ❌ Error! Still abstract

    Car car;
    Bicycle bike;

    cout << "--- Car ---\n";
    car.displayWheels();
    car.start();
    car.refuel();
    car.stop();

    cout << "\n--- Bicycle ---\n";
    bike.displayWheels();
    bike.start();
    bike.refuel();
    bike.stop();

    cout << "\n✅ Partially abstract classes:\n";
    cout << "   • Implement some pure virtual functions\n";
    cout << "   • Still abstract if any pure virtual left\n";
    cout << "   • Provide common implementation\n";
    cout << "   • Reduce code duplication\n";
}

// ============================================
// EXAMPLE 5: ABSTRACT CLASS AS INTERFACE
// ============================================

// Database interface
class IDatabase {
public:
    virtual bool connect(const string& connectionString) = 0;
    virtual void disconnect() = 0;
    virtual bool executeQuery(const string& query) = 0;
    virtual string getResult() const = 0;
    virtual ~IDatabase() = default;
};

class MySQLDatabase : public IDatabase {
private:
    bool connected;
    string lastResult;

public:
    MySQLDatabase() : connected(false) { }

    bool connect(const string& connectionString) override {
        cout << "  Connecting to MySQL: " << connectionString << "\n";
        connected = true;
        return true;
    }

    void disconnect() override {
        cout << "  Disconnecting from MySQL\n";
        connected = false;
    }

    bool executeQuery(const string& query) override {
        if (!connected) {
            cout << "  Error: Not connected!\n";
            return false;
        }
        cout << "  Executing MySQL query: " << query << "\n";
        lastResult = "MySQL result for: " + query;
        return true;
    }

    string getResult() const override {
        return lastResult;
    }
};

class PostgreSQLDatabase : public IDatabase {
private:
    bool connected;
    string lastResult;

public:
    PostgreSQLDatabase() : connected(false) { }

    bool connect(const string& connectionString) override {
        cout << "  Connecting to PostgreSQL: " << connectionString << "\n";
        connected = true;
        return true;
    }

    void disconnect() override {
        cout << "  Disconnecting from PostgreSQL\n";
        connected = false;
    }

    bool executeQuery(const string& query) override {
        if (!connected) {
            cout << "  Error: Not connected!\n";
            return false;
        }
        cout << "  Executing PostgreSQL query: " << query << "\n";
        lastResult = "PostgreSQL result for: " + query;
        return true;
    }

    string getResult() const override {
        return lastResult;
    }
};

void demonstrateInterface() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: ABSTRACT AS INTERFACE  ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Using MySQL database ---\n";
    unique_ptr<IDatabase> mysql = make_unique<MySQLDatabase>();
    mysql->connect("mysql://localhost:3306/mydb");
    mysql->executeQuery("SELECT * FROM users");
    cout << "  Result: " << mysql->getResult() << "\n";
    mysql->disconnect();

    cout << "\n--- Using PostgreSQL database ---\n";
    unique_ptr<IDatabase> postgres = make_unique<PostgreSQLDatabase>();
    postgres->connect("postgresql://localhost:5432/mydb");
    postgres->executeQuery("SELECT * FROM users");
    cout << "  Result: " << postgres->getResult() << "\n";
    postgres->disconnect();

    cout << "\n✅ Interface benefits:\n";
    cout << "   • Swap implementations easily\n";
    cout << "   • Dependency inversion\n";
    cout << "   • Testability (mock implementations)\n";
    cout << "   • Loose coupling\n";
}

// ============================================
// EXAMPLE 6: TEMPLATE METHOD PATTERN
// ============================================

class DataProcessor {
public:
    // Template method - defines algorithm skeleton
    void process() {
        loadData();
        validateData();
        processData();  // Abstract - must implement
        saveData();
    }

    virtual ~DataProcessor() = default;

protected:
    // Hook methods - can override
    virtual void loadData() {
        cout << "  Loading data...\n";
    }

    virtual void validateData() {
        cout << "  Validating data...\n";
    }

    // Pure virtual - must implement
    virtual void processData() = 0;

    virtual void saveData() {
        cout << "  Saving data...\n";
    }
};

class XMLProcessor : public DataProcessor {
protected:
    void processData() override {
        cout << "  Processing XML data...\n";
        cout << "    - Parsing XML\n";
        cout << "    - Transforming elements\n";
        cout << "    - Validating schema\n";
    }
};

class JSONProcessor : public DataProcessor {
protected:
    void loadData() override {
        cout << "  Loading JSON data from API...\n";
    }

    void processData() override {
        cout << "  Processing JSON data...\n";
        cout << "    - Parsing JSON\n";
        cout << "    - Extracting fields\n";
        cout << "    - Converting types\n";
    }

    void saveData() override {
        cout << "  Saving to NoSQL database...\n";
    }
};

void demonstrateTemplateMethod() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: TEMPLATE METHOD        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Processing XML ---\n";
    XMLProcessor xmlProc;
    xmlProc.process();

    cout << "\n--- Processing JSON ---\n";
    JSONProcessor jsonProc;
    jsonProc.process();

    cout << "\n✅ Template Method Pattern:\n";
    cout << "   • Define algorithm skeleton in base\n";
    cout << "   • Let derived classes implement steps\n";
    cout << "   • Preserve overall structure\n";
    cout << "   • Code reuse with flexibility\n";
}

// ============================================
// EXAMPLE 7: STRATEGY PATTERN
// ============================================

// Strategy interface
class SortStrategy {
public:
    virtual void sort(vector<int>& data) = 0;
    virtual string getName() const = 0;
    virtual ~SortStrategy() = default;
};

class BubbleSort : public SortStrategy {
public:
    void sort(vector<int>& data) override {
        cout << "  Sorting with Bubble Sort...\n";
        for (size_t i = 0; i < data.size(); i++) {
            for (size_t j = 0; j < data.size() - 1; j++) {
                if (data[j] > data[j + 1]) {
                    swap(data[j], data[j + 1]);
                }
            }
        }
    }

    string getName() const override {
        return "Bubble Sort";
    }
};

class QuickSort : public SortStrategy {
public:
    void sort(vector<int>& data) override {
        cout << "  Sorting with Quick Sort...\n";
        // Simplified - actual quicksort implementation
        std::sort(data.begin(), data.end());
    }

    string getName() const override {
        return "Quick Sort";
    }
};

class Sorter {
private:
    SortStrategy* strategy;

public:
    Sorter(SortStrategy* s) : strategy(s) { }

    void setStrategy(SortStrategy* s) {
        strategy = s;
    }

    void sortData(vector<int>& data) {
        cout << "  Using strategy: " << strategy->getName() << "\n";
        strategy->sort(data);
    }
};

void demonstrateStrategy() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 7: STRATEGY PATTERN       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    vector<int> data1 = {5, 2, 8, 1, 9};
    vector<int> data2 = {7, 3, 6, 4, 0};

    BubbleSort bubbleSort;
    QuickSort quickSort;

    Sorter sorter(&bubbleSort);

    cout << "--- Sorting with Bubble Sort ---\n";
    sorter.sortData(data1);
    cout << "  Result: ";
    for (int x : data1) cout << x << " ";
    cout << "\n";

    cout << "\n--- Changing strategy to Quick Sort ---\n";
    sorter.setStrategy(&quickSort);
    sorter.sortData(data2);
    cout << "  Result: ";
    for (int x : data2) cout << x << " ";
    cout << "\n";

    cout << "\n✅ Strategy Pattern:\n";
    cout << "   • Define family of algorithms\n";
    cout << "   • Encapsulate each algorithm\n";
    cout << "   • Make them interchangeable\n";
    cout << "   • Select algorithm at runtime\n";
}

// ============================================
// EXAMPLE 8: OBSERVER PATTERN
// ============================================

class Observer {
public:
    virtual void update(const string& message) = 0;
    virtual string getName() const = 0;
    virtual ~Observer() = default;
};

class Subject {
private:
    vector<Observer*> observers;
    string state;

public:
    void attach(Observer* observer) {
        observers.push_back(observer);
        cout << "  Attached observer: " << observer->getName() << "\n";
    }

    void detach(Observer* observer) {
        auto it = find(observers.begin(), observers.end(), observer);
        if (it != observers.end()) {
            observers.erase(it);
            cout << "  Detached observer: " << observer->getName() << "\n";
        }
    }

    void setState(const string& newState) {
        state = newState;
        notify();
    }

    void notify() {
        cout << "  Notifying observers about state change: " << state << "\n";
        for (Observer* observer : observers) {
            observer->update(state);
        }
    }
};

class EmailObserver : public Observer {
public:
    void update(const string& message) override {
        cout << "    EmailObserver: Sending email about '" << message << "'\n";
    }

    string getName() const override {
        return "EmailObserver";
    }
};

class SMSObserver : public Observer {
public:
    void update(const string& message) override {
        cout << "    SMSObserver: Sending SMS about '" << message << "'\n";
    }

    string getName() const override {
        return "SMSObserver";
    }
};

class LogObserver : public Observer {
public:
    void update(const string& message) override {
        cout << "    LogObserver: Logging event '" << message << "'\n";
    }

    string getName() const override {
        return "LogObserver";
    }
};

void demonstrateObserver() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 8: OBSERVER PATTERN       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Subject subject;

    EmailObserver emailObs;
    SMSObserver smsObs;
    LogObserver logObs;

    cout << "--- Attaching observers ---\n";
    subject.attach(&emailObs);
    subject.attach(&smsObs);
    subject.attach(&logObs);

    cout << "\n--- State change 1 ---\n";
    subject.setState("User registered");

    cout << "\n--- Detaching SMS observer ---\n";
    subject.detach(&smsObs);

    cout << "\n--- State change 2 ---\n";
    subject.setState("User logged in");

    cout << "\n✅ Observer Pattern:\n";
    cout << "   • Define one-to-many dependency\n";
    cout << "   • Notify all dependents automatically\n";
    cout << "   • Loose coupling\n";
    cout << "   • Dynamic subscription\n";
}

// ============================================
// EXAMPLE 9: FACTORY METHOD PATTERN
// ============================================

// Product interface
class Button {
public:
    virtual void render() const = 0;
    virtual void onClick() const = 0;
    virtual ~Button() = default;
};

class WindowsButton : public Button {
public:
    void render() const override {
        cout << "  Rendering Windows-style button\n";
    }

    void onClick() const override {
        cout << "  Windows button clicked\n";
    }
};

class MacOSButton : public Button {
public:
    void render() const override {
        cout << "  Rendering MacOS-style button\n";
    }

    void onClick() const override {
        cout << "  MacOS button clicked\n";
    }
};

// Creator (abstract factory)
class Dialog {
public:
    void render() const {
        Button* button = createButton();
        button->render();
        button->onClick();
        delete button;
    }

    virtual ~Dialog() = default;

protected:
    // Factory method - must implement
    virtual Button* createButton() const = 0;
};

class WindowsDialog : public Dialog {
protected:
    Button* createButton() const override {
        return new WindowsButton();
    }
};

class MacOSDialog : public Dialog {
protected:
    Button* createButton() const override {
        return new MacOSButton();
    }
};

void demonstrateFactoryMethod() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 9: FACTORY METHOD         ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Windows Dialog ---\n";
    WindowsDialog winDialog;
    winDialog.render();

    cout << "\n--- MacOS Dialog ---\n";
    MacOSDialog macDialog;
    macDialog.render();

    cout << "\n✅ Factory Method Pattern:\n";
    cout << "   • Define interface for creating objects\n";
    cout << "   • Let subclasses decide which class to instantiate\n";
    cout << "   • Defer instantiation to subclasses\n";
    cout << "   • Promote loose coupling\n";
}

// ============================================
// EXAMPLE 10: REAL-WORLD - PAYMENT GATEWAY
// ============================================

class PaymentGateway {
public:
    // Template method
    bool processPayment(double amount) {
        if (!validateAmount(amount)) {
            cout << "  ❌ Invalid amount\n";
            return false;
        }

        if (!authenticate()) {
            cout << "  ❌ Authentication failed\n";
            return false;
        }

        if (!charge(amount)) {
            cout << "  ❌ Charge failed\n";
            return false;
        }

        sendReceipt(amount);
        return true;
    }

    virtual ~PaymentGateway() = default;

protected:
    bool validateAmount(double amount) {
        return amount > 0;
    }

    // Abstract methods - must implement
    virtual bool authenticate() = 0;
    virtual bool charge(double amount) = 0;
    virtual void sendReceipt(double amount) = 0;

    // Hook - can override
    virtual void logTransaction(double amount) {
        cout << "  Logging transaction: $" << amount << "\n";
    }
};

class StripeGateway : public PaymentGateway {
protected:
    bool authenticate() override {
        cout << "  Authenticating with Stripe...\n";
        return true;
    }

    bool charge(double amount) override {
        cout << "  Charging $" << amount << " via Stripe\n";
        logTransaction(amount);
        return true;
    }

    void sendReceipt(double amount) override {
        cout << "  Sending Stripe receipt for $" << amount << "\n";
    }
};

class PayPalGateway : public PaymentGateway {
protected:
    bool authenticate() override {
        cout << "  Authenticating with PayPal...\n";
        return true;
    }

    bool charge(double amount) override {
        cout << "  Charging $" << amount << " via PayPal\n";
        logTransaction(amount);
        return true;
    }

    void sendReceipt(double amount) override {
        cout << "  Sending PayPal receipt for $" << amount << "\n";
    }

    void logTransaction(double amount) override {
        cout << "  PayPal transaction log: $" << amount << "\n";
    }
};

void demonstratePaymentGateway() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 10: PAYMENT GATEWAY       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Processing payment via Stripe ---\n";
    unique_ptr<PaymentGateway> stripe = make_unique<StripeGateway>();
    bool success1 = stripe->processPayment(99.99);
    cout << "  Result: " << (success1 ? "✅ Success" : "❌ Failed") << "\n";

    cout << "\n--- Processing payment via PayPal ---\n";
    unique_ptr<PaymentGateway> paypal = make_unique<PayPalGateway>();
    bool success2 = paypal->processPayment(149.99);
    cout << "  Result: " << (success2 ? "✅ Success" : "❌ Failed") << "\n";

    cout << "\n✅ Real-world abstract classes:\n";
    cout << "   • Define payment processing workflow\n";
    cout << "   • Each gateway implements details\n";
    cout << "   • Easy to add new gateways\n";
    cout << "   • Consistent interface\n";
}

// ============================================
// MAIN DEMONSTRATION
// ============================================

int main() {
    cout << "========================================\n";
    cout << "ABSTRACT CLASSES DEMONSTRATION\n";
    cout << "========================================\n";

    demonstrateBasicAbstract();
    demonstratePureInterface();
    demonstrateAbstractWithImplementation();
    demonstratePartiallyAbstract();
    demonstrateInterface();
    demonstrateTemplateMethod();
    demonstrateStrategy();
    demonstrateObserver();
    demonstrateFactoryMethod();
    demonstratePaymentGateway();

    // ============================================
    // Comprehensive Summary
    // ============================================
    cout << "\n========================================\n";
    cout << "WHAT ARE ABSTRACT CLASSES?\n";
    cout << "========================================\n";
    cout << "Abstract classes are classes that cannot be\n";
    cout << "instantiated and contain at least one pure\n";
    cout << "virtual function. They define interfaces and\n";
    cout << "contracts for derived classes.\n";
    cout << "\n";
    cout << "Key characteristics:\n";
    cout << "  • Have at least one pure virtual function\n";
    cout << "  • Cannot be instantiated\n";
    cout << "  • Can have pointers/references\n";
    cout << "  • Define interface contracts\n";
    cout << "  • Force implementation in derived classes\n";

    cout << "\n========================================\n";
    cout << "SYNTAX\n";
    cout << "========================================\n";
    cout << "ABSTRACT CLASS:\n";
    cout << "class Abstract {\n";
    cout << "public:\n";
    cout << "    // Pure virtual function\n";
    cout << "    virtual void pureFunc() = 0;\n";
    cout << "    \n";
    cout << "    // Regular virtual function\n";
    cout << "    virtual void virtualFunc() {\n";
    cout << "        // Default implementation\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    // Non-virtual function\n";
    cout << "    void regularFunc() { }\n";
    cout << "    \n";
    cout << "    // Virtual destructor\n";
    cout << "    virtual ~Abstract() = default;\n";
    cout << "};\n";
    cout << "\n";
    cout << "CONCRETE CLASS:\n";
    cout << "class Concrete : public Abstract {\n";
    cout << "public:\n";
    cout << "    // MUST implement pure virtual\n";
    cout << "    void pureFunc() override {\n";
    cout << "        // Implementation\n";
    cout << "    }\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "PURE VIRTUAL FUNCTIONS\n";
    cout << "========================================\n";
    cout << "Declaration:\n";
    cout << "  virtual ReturnType functionName() = 0;\n";
    cout << "\n";
    cout << "Characteristics:\n";
    cout << "  • Declared with = 0\n";
    cout << "  • Usually no implementation\n";
    cout << "  • Makes class abstract\n";
    cout << "  • Derived classes MUST implement\n";
    cout << "  • Defines interface contract\n";
    cout << "\n";
    cout << "Example:\n";
    cout << "class Shape {\n";
    cout << "public:\n";
    cout << "    virtual double area() const = 0;  // Pure virtual\n";
    cout << "};\n";
    cout << "\n";
    cout << "class Circle : public Shape {\n";
    cout << "public:\n";
    cout << "    double area() const override {  // Must implement\n";
    cout << "        return 3.14 * r * r;\n";
    cout << "    }\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "ABSTRACT CLASS RULES\n";
    cout << "========================================\n";
    cout << "❌ CANNOT:\n";
    cout << "  • Instantiate abstract class\n";
    cout << "    Abstract obj;  // Error!\n";
    cout << "\n";
    cout << "✅ CAN:\n";
    cout << "  • Have pointers to abstract class\n";
    cout << "    Abstract* ptr = new Concrete();\n";
    cout << "  \n";
    cout << "  • Have references to abstract class\n";
    cout << "    Abstract& ref = concrete;\n";
    cout << "  \n";
    cout << "  • Have constructors (called by derived)\n";
    cout << "  • Have data members\n";
    cout << "  • Have regular methods\n";
    cout << "  • Have virtual methods\n";
    cout << "  • Have pure virtual methods\n";

    cout << "\n========================================\n";
    cout << "ABSTRACT vs CONCRETE\n";
    cout << "========================================\n";
    cout << "┌─────────────────────┬─────────────────────┐\n";
    cout << "│ ABSTRACT CLASS      │ CONCRETE CLASS      │\n";
    cout << "├─────────────────────┼─────────────────────┤\n";
    cout << "│ Has pure virtual    │ No pure virtual     │\n";
    cout << "│ Cannot instantiate  │ Can instantiate     │\n";
    cout << "│ Defines interface   │ Provides impl       │\n";
    cout << "│ Used as base        │ Used as object      │\n";
    cout << "│ Pointer/reference   │ Actual objects      │\n";
    cout << "└─────────────────────┴─────────────────────┘\n";
    cout << "\n";
    cout << "Example:\n";
    cout << "// Abstract obj;     // ❌ Error!\n";
    cout << "Concrete obj;         // ✅ OK!\n";
    cout << "Abstract* ptr = &obj; // ✅ OK!\n";

    cout << "\n========================================\n";
    cout << "PURE INTERFACE\n";
    cout << "========================================\n";
    cout << "Pure interface = Only pure virtual functions\n";
    cout << "\n";
    cout << "class IInterface {\n";
    cout << "public:\n";
    cout << "    virtual void method1() = 0;\n";
    cout << "    virtual void method2() = 0;\n";
    cout << "    virtual ~IInterface() = default;\n";
    cout << "    \n";
    cout << "    // No data members\n";
    cout << "    // No implementation\n";
    cout << "};\n";
    cout << "\n";
    cout << "Benefits:\n";
    cout << "  • Clear contract definition\n";
    cout << "  • Multiple inheritance safe\n";
    cout << "  • Dependency inversion\n";
    cout << "  • Testability (mocking)\n";
    cout << "  • Loose coupling\n";
    cout << "\n";
    cout << "Convention: Prefix with 'I' (IDrawable, IPrintable)\n";

    cout << "\n========================================\n";
    cout << "ABSTRACT CLASS WITH IMPLEMENTATION\n";
    cout << "========================================\n";
    cout << "Abstract classes can have:\n";
    cout << "  • Pure virtual functions (= 0)\n";
    cout << "  • Virtual functions (default impl)\n";
    cout << "  • Non-virtual functions\n";
    cout << "  • Data members\n";
    cout << "\n";
    cout << "Example:\n";
    cout << "class Animal {\n";
    cout << "protected:\n";
    cout << "    string name;  // Data member\n";
    cout << "public:\n";
    cout << "    Animal(string n) : name(n) { }  // Constructor\n";
    cout << "    \n";
    cout << "    virtual void makeSound() = 0;   // Pure virtual\n";
    cout << "    \n";
    cout << "    virtual void sleep() {          // Virtual\n";
    cout << "        cout << name << \" sleeps\";\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    void displayName() {            // Non-virtual\n";
    cout << "        cout << name;\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "Mix of abstract and concrete behavior!\n";

    cout << "\n========================================\n";
    cout << "DESIGN PATTERNS WITH ABSTRACT CLASSES\n";
    cout << "========================================\n";
    cout << "1. TEMPLATE METHOD:\n";
    cout << "   • Define algorithm skeleton in base\n";
    cout << "   • Let derived implement steps\n";
    cout << "   • Preserve overall structure\n";
    cout << "\n";
    cout << "2. STRATEGY:\n";
    cout << "   • Define family of algorithms\n";
    cout << "   • Encapsulate each one\n";
    cout << "   • Make them interchangeable\n";
    cout << "\n";
    cout << "3. OBSERVER:\n";
    cout << "   • Define one-to-many dependency\n";
    cout << "   • Notify all dependents automatically\n";
    cout << "   • Subject and Observer interfaces\n";
    cout << "\n";
    cout << "4. FACTORY METHOD:\n";
    cout << "   • Define interface for creating objects\n";
    cout << "   • Let subclasses decide type\n";
    cout << "   • Defer instantiation\n";
    cout << "\n";
    cout << "All use abstract classes/interfaces!\n";

    cout << "\n========================================\n";
    cout << "WHEN TO USE ABSTRACT CLASSES\n";
    cout << "========================================\n";
    cout << "✅ USE ABSTRACT CLASSES WHEN:\n";
    cout << "  • Need to define interface contract\n";
    cout << "  • Want to force implementation in derived\n";
    cout << "  • Have common behavior to share\n";
    cout << "  • Building framework/library\n";
    cout << "  • Implementing design patterns\n";
    cout << "  • Need polymorphic behavior\n";
    cout << "  • Want to enforce design\n";
    cout << "\n";
    cout << "❌ DON'T USE WHEN:\n";
    cout << "  • All methods have implementation\n";
    cout << "  • No polymorphism needed\n";
    cout << "  • Simple data structures\n";
    cout << "  • All concrete implementations\n";

    cout << "\n========================================\n";
    cout << "INTERFACE vs ABSTRACT CLASS\n";
    cout << "========================================\n";
    cout << "┌─────────────────────┬─────────────────────┐\n";
    cout << "│ INTERFACE (Pure)    │ ABSTRACT CLASS      │\n";
    cout << "├─────────────────────┼─────────────────────┤\n";
    cout << "│ Only pure virtual   │ Mix of pure/virtual │\n";
    cout << "│ No data members     │ Can have data       │\n";
    cout << "│ No implementation   │ Can have impl       │\n";
    cout << "│ Multiple inherit OK │ Single inherit      │\n";
    cout << "│ Pure contract       │ Base + contract     │\n";
    cout << "└─────────────────────┴─────────────────────┘\n";
    cout << "\n";
    cout << "C++ doesn't have 'interface' keyword,\n";
    cout << "but we create them using pure virtual functions.\n";

    cout << "\n========================================\n";
    cout << "BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "✅ DO:\n";
    cout << "  • Make destructor virtual (always!)\n";
    cout << "  • Use pure virtual for required behavior\n";
    cout << "  • Provide default implementations when sensible\n";
    cout << "  • Document interface contracts\n";
    cout << "  • Keep interfaces small and focused\n";
    cout << "  • Use override keyword in derived classes\n";
    cout << "  • Prefix interface names with 'I'\n";
    cout << "  • Think 'interface' vs 'base class'\n";
    cout << "\n";
    cout << "❌ DON'T:\n";
    cout << "  • Forget virtual destructor\n";
    cout << "  • Make interfaces too large\n";
    cout << "  • Add implementation to pure interfaces\n";
    cout << "  • Try to instantiate abstract classes\n";
    cout << "  • Forget to implement pure virtuals\n";
    cout << "  • Mix too many concerns in one abstract class\n";
    cout << "  • Use abstract classes for everything\n";

    cout << "\n========================================\n";
    cout << "COMMON MISTAKES\n";
    cout << "========================================\n";
    cout << "❌ Mistake 1: Trying to instantiate\n";
    cout << "class Abstract {\n";
    cout << "    virtual void func() = 0;\n";
    cout << "};\n";
    cout << "Abstract obj;  // ❌ Error!\n";
    cout << "\n";
    cout << "❌ Mistake 2: Forgetting to implement\n";
    cout << "class Derived : public Abstract {\n";
    cout << "    // Forgot to implement func()!\n";
    cout << "};\n";
    cout << "Derived d;  // ❌ Error! Still abstract\n";
    cout << "\n";
    cout << "❌ Mistake 3: Non-virtual destructor\n";
    cout << "class Abstract {\n";
    cout << "    ~Abstract() { }  // ❌ Should be virtual!\n";
    cout << "};\n";
    cout << "Fix: virtual ~Abstract() = default;\n";

    cout << "\n========================================\n";
    cout << "QUICK REFERENCE\n";
    cout << "========================================\n";
    cout << "// Abstract class\n";
    cout << "class Abstract {\n";
    cout << "public:\n";
    cout << "    // Pure virtual - must implement\n";
    cout << "    virtual void pureFunc() = 0;\n";
    cout << "    \n";
    cout << "    // Virtual - can override\n";
    cout << "    virtual void virtualFunc() {\n";
    cout << "        // Default implementation\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    // Virtual destructor\n";
    cout << "    virtual ~Abstract() = default;\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Concrete class\n";
    cout << "class Concrete : public Abstract {\n";
    cout << "public:\n";
    cout << "    // Implement pure virtual\n";
    cout << "    void pureFunc() override {\n";
    cout << "        cout << \"Implementation\";\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Usage\n";
    cout << "// Abstract a;          // ❌ Error!\n";
    cout << "Concrete c;             // ✅ OK!\n";
    cout << "Abstract* ptr = &c;     // ✅ OK!\n";
    cout << "ptr->pureFunc();        // ✅ OK!\n";

    cout << "\n========================================\n";
    cout << "KEY TAKEAWAYS\n";
    cout << "========================================\n";
    cout << "1. Abstract classes cannot be instantiated\n";
    cout << "2. Have at least one pure virtual function (= 0)\n";
    cout << "3. Used to define interfaces/contracts\n";
    cout << "4. Derived classes must implement pure virtuals\n";
    cout << "5. Can have mix of pure, virtual, and regular functions\n";
    cout << "6. Can have data members and constructors\n";
    cout << "7. Always make destructor virtual\n";
    cout << "8. Enable polymorphism and design patterns\n";
    cout << "9. Use pointers/references for polymorphic behavior\n";
    cout << "10. Foundation for interface-based design\n";

    return 0;
}
