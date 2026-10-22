#include <print>
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
        std::println("  Shape constructor: {}", name);
    }

    // Virtual destructor (IMPORTANT!)
    virtual ~Shape() {
        std::println("  Shape destructor: {}", name);
    }

    // Pure virtual functions - MUST be implemented by derived classes
    virtual double area() const = 0;
    virtual double perimeter() const = 0;

    // Regular virtual function - can be overridden
    virtual void draw() const {
        std::println("  Drawing {} {}", color, name);
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
        std::println("  Circle constructor");
    }

    ~Circle() {
        std::println("  Circle destructor");
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
        std::println("  Drawing {} circle with radius {}", color, radius);
    }
};

class Rectangle : public Shape {
private:
    double width, height;

public:
    Rectangle(string c, double w, double h)
        : Shape("Rectangle", c), width(w), height(h) {
        std::println("  Rectangle constructor");
    }

    ~Rectangle() {
        std::println("  Rectangle destructor");
    }

    // MUST implement pure virtual functions
    double area() const override {
        return width * height;
    }

    double perimeter() const override {
        return 2 * (width + height);
    }

    void draw() const override {
        std::println("  Drawing {} rectangle: {}×{}", color, width, height);
    }
};

void demonstrateBasicAbstract() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: BASIC ABSTRACT CLASS   ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    // Shape shape("Generic", "white");  // ❌ Error! Cannot instantiate

    std::println("--- Creating concrete shapes ---");
    Circle circle("red", 5.0);
    Rectangle rect("blue", 10.0, 5.0);

    std::println("");
    std::println("--- Using abstract class pointers ---");
    Shape* shapes[2];
    shapes[0] = &circle;
    shapes[1] = &rect;

    for (int i = 0; i < 2; i++) {
        shapes[i]->draw();
        std::println("  Area: {}", shapes[i]->area());
        std::println("  Perimeter: {}", shapes[i]->perimeter());
        std::println("");
    }

    std::println("");
    std::println("--- Destruction order ---");
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
        std::println("  Drawing document: {}", title);
        std::println("  Content: {}", content);
    }

    void resize(double factor) override {
        std::println("  Resizing document by factor {}", factor);
    }

    // Implement IPrintable
    void print() const override {
        std::println("  Printing document: {}", title);
        std::println("  {}", content);
    }

    void printToFile(const string& filename) const override {
        std::println("  Printing to file: {}", filename);
        std::println("  Title: {}", title);
    }
};

void demonstratePureInterface() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: PURE INTERFACE         ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Document doc("Report", "This is a business report.");

    std::println("--- Using as IDrawable ---");
    IDrawable* drawable = &doc;
    drawable->draw();
    drawable->resize(1.5);

    std::println("");
    std::println("--- Using as IPrintable ---");
    IPrintable* printable = &doc;
    printable->print();
    printable->printToFile("report.pdf");
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
        std::println("  {} is sleeping... Zzz", name);
    }

    // Non-virtual - cannot override
    void displayInfo() const {
        std::println("  Name: {}, Age: {}", name, age);
    }
};

class Dog : public Animal {
public:
    Dog(string n, int a) : Animal(n, a) { }

    void makeSound() const override {
        std::println("  {} says: Woof! Woof!", name);
    }

    void move() const override {
        std::println("  {} runs on four legs", name);
    }

    // Override sleep with dog-specific behavior
    void sleep() const override {
        std::println("  {} curls up and sleeps... Zzz", name);
    }
};

class Bird : public Animal {
public:
    Bird(string n, int a) : Animal(n, a) { }

    void makeSound() const override {
        std::println("  {} says: Tweet! Tweet!", name);
    }

    void move() const override {
        std::println("  {} flies in the sky", name);
    }

    // Uses default sleep implementation
};

void demonstrateAbstractWithImplementation() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: ABSTRACT + IMPL        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Dog dog("Buddy", 3);
    Bird bird("Tweety", 1);

    std::println("--- Dog ---");
    dog.displayInfo();
    dog.makeSound();
    dog.move();
    dog.sleep();

    std::println("");
    std::println("--- Bird ---");
    bird.displayInfo();
    bird.makeSound();
    bird.move();
    bird.sleep();  // Uses default implementation
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
        std::println("  Starting land vehicle...");
    }

    void stop() override {
        std::println("  Stopping land vehicle...");
    }

    // refuel() still pure virtual - still abstract!

    void displayWheels() const {
        std::println("  Number of wheels: {}", wheels);
    }
};

// Concrete - implements all pure virtual functions
class Car : public LandVehicle {
public:
    Car() : LandVehicle(4) { }

    void refuel() override {
        std::println("  Refueling car with gasoline...");
    }
};

class Bicycle : public LandVehicle {
public:
    Bicycle() : LandVehicle(2) { }

    void refuel() override {
        std::println("  Bicycle doesn't need fuel - human powered!");
    }
};

void demonstratePartiallyAbstract() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: PARTIALLY ABSTRACT     ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    // Vehicle v;        // ❌ Error! Abstract
    // LandVehicle lv;   // ❌ Error! Still abstract

    Car car;
    Bicycle bike;

    std::println("--- Car ---");
    car.displayWheels();
    car.start();
    car.refuel();
    car.stop();

    std::println("");
    std::println("--- Bicycle ---");
    bike.displayWheels();
    bike.start();
    bike.refuel();
    bike.stop();
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
        std::println("  Connecting to MySQL: {}", connectionString);
        connected = true;
        return true;
    }

    void disconnect() override {
        std::println("  Disconnecting from MySQL");
        connected = false;
    }

    bool executeQuery(const string& query) override {
        if (!connected) {
            std::println("  Error: Not connected!");
            return false;
        }
        std::println("  Executing MySQL query: {}", query);
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
        std::println("  Connecting to PostgreSQL: {}", connectionString);
        connected = true;
        return true;
    }

    void disconnect() override {
        std::println("  Disconnecting from PostgreSQL");
        connected = false;
    }

    bool executeQuery(const string& query) override {
        if (!connected) {
            std::println("  Error: Not connected!");
            return false;
        }
        std::println("  Executing PostgreSQL query: {}", query);
        lastResult = "PostgreSQL result for: " + query;
        return true;
    }

    string getResult() const override {
        return lastResult;
    }
};

void demonstrateInterface() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 5: ABSTRACT AS INTERFACE  ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Using MySQL database ---");
    unique_ptr<IDatabase> mysql = make_unique<MySQLDatabase>();
    mysql->connect("mysql://localhost:3306/mydb");
    mysql->executeQuery("SELECT * FROM users");
    std::println("  Result: {}", mysql->getResult());
    mysql->disconnect();

    std::println("");
    std::println("--- Using PostgreSQL database ---");
    unique_ptr<IDatabase> postgres = make_unique<PostgreSQLDatabase>();
    postgres->connect("postgresql://localhost:5432/mydb");
    postgres->executeQuery("SELECT * FROM users");
    std::println("  Result: {}", postgres->getResult());
    postgres->disconnect();
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
        std::println("  Loading data...");
    }

    virtual void validateData() {
        std::println("  Validating data...");
    }

    // Pure virtual - must implement
    virtual void processData() = 0;

    virtual void saveData() {
        std::println("  Saving data...");
    }
};

class XMLProcessor : public DataProcessor {
protected:
    void processData() override {
        std::println("  Processing XML data...");
        std::println("    - Parsing XML");
        std::println("    - Transforming elements");
        std::println("    - Validating schema");
    }
};

class JSONProcessor : public DataProcessor {
protected:
    void loadData() override {
        std::println("  Loading JSON data from API...");
    }

    void processData() override {
        std::println("  Processing JSON data...");
        std::println("    - Parsing JSON");
        std::println("    - Extracting fields");
        std::println("    - Converting types");
    }

    void saveData() override {
        std::println("  Saving to NoSQL database...");
    }
};

void demonstrateTemplateMethod() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 6: TEMPLATE METHOD        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Processing XML ---");
    XMLProcessor xmlProc;
    xmlProc.process();

    std::println("");
    std::println("--- Processing JSON ---");
    JSONProcessor jsonProc;
    jsonProc.process();
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
        std::println("  Sorting with Bubble Sort...");
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
        std::println("  Sorting with Quick Sort...");
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
        std::println("  Using strategy: {}", strategy->getName());
        strategy->sort(data);
    }
};

void demonstrateStrategy() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 7: STRATEGY PATTERN       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    vector<int> data1 = {5, 2, 8, 1, 9};
    vector<int> data2 = {7, 3, 6, 4, 0};

    BubbleSort bubbleSort;
    QuickSort quickSort;

    Sorter sorter(&bubbleSort);

    std::println("--- Sorting with Bubble Sort ---");
    sorter.sortData(data1);
    std::print("  Result: ");
    for (int x : data1) std::print("{} ", x);
    std::println("");

    std::println("");
    std::println("--- Changing strategy to Quick Sort ---");
    sorter.setStrategy(&quickSort);
    sorter.sortData(data2);
    std::print("  Result: ");
    for (int x : data2) std::print("{} ", x);
    std::println("");
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
        std::println("  Attached observer: {}", observer->getName());
    }

    void detach(Observer* observer) {
        auto it = find(observers.begin(), observers.end(), observer);
        if (it != observers.end()) {
            observers.erase(it);
            std::println("  Detached observer: {}", observer->getName());
        }
    }

    void setState(const string& newState) {
        state = newState;
        notify();
    }

    void notify() {
        std::println("  Notifying observers about state change: {}", state);
        for (Observer* observer : observers) {
            observer->update(state);
        }
    }
};

class EmailObserver : public Observer {
public:
    void update(const string& message) override {
        std::println("    EmailObserver: Sending email about '{}'", message);
    }

    string getName() const override {
        return "EmailObserver";
    }
};

class SMSObserver : public Observer {
public:
    void update(const string& message) override {
        std::println("    SMSObserver: Sending SMS about '{}'", message);
    }

    string getName() const override {
        return "SMSObserver";
    }
};

class LogObserver : public Observer {
public:
    void update(const string& message) override {
        std::println("    LogObserver: Logging event '{}'", message);
    }

    string getName() const override {
        return "LogObserver";
    }
};

void demonstrateObserver() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 8: OBSERVER PATTERN       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Subject subject;

    EmailObserver emailObs;
    SMSObserver smsObs;
    LogObserver logObs;

    std::println("--- Attaching observers ---");
    subject.attach(&emailObs);
    subject.attach(&smsObs);
    subject.attach(&logObs);

    std::println("");
    std::println("--- State change 1 ---");
    subject.setState("User registered");

    std::println("");
    std::println("--- Detaching SMS observer ---");
    subject.detach(&smsObs);

    std::println("");
    std::println("--- State change 2 ---");
    subject.setState("User logged in");
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
        std::println("  Rendering Windows-style button");
    }

    void onClick() const override {
        std::println("  Windows button clicked");
    }
};

class MacOSButton : public Button {
public:
    void render() const override {
        std::println("  Rendering MacOS-style button");
    }

    void onClick() const override {
        std::println("  MacOS button clicked");
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
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 9: FACTORY METHOD         ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Windows Dialog ---");
    WindowsDialog winDialog;
    winDialog.render();

    std::println("");
    std::println("--- MacOS Dialog ---");
    MacOSDialog macDialog;
    macDialog.render();
}

// ============================================
// EXAMPLE 10: REAL-WORLD - PAYMENT GATEWAY
// ============================================

class PaymentGateway {
public:
    // Template method
    bool processPayment(double amount) {
        if (!validateAmount(amount)) {
            std::println("  ❌ Invalid amount");
            return false;
        }

        if (!authenticate()) {
            std::println("  ❌ Authentication failed");
            return false;
        }

        if (!charge(amount)) {
            std::println("  ❌ Charge failed");
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
        std::println("  Logging transaction: ${}", amount);
    }
};

class StripeGateway : public PaymentGateway {
protected:
    bool authenticate() override {
        std::println("  Authenticating with Stripe...");
        return true;
    }

    bool charge(double amount) override {
        std::println("  Charging ${} via Stripe", amount);
        logTransaction(amount);
        return true;
    }

    void sendReceipt(double amount) override {
        std::println("  Sending Stripe receipt for ${}", amount);
    }
};

class PayPalGateway : public PaymentGateway {
protected:
    bool authenticate() override {
        std::println("  Authenticating with PayPal...");
        return true;
    }

    bool charge(double amount) override {
        std::println("  Charging ${} via PayPal", amount);
        logTransaction(amount);
        return true;
    }

    void sendReceipt(double amount) override {
        std::println("  Sending PayPal receipt for ${}", amount);
    }

    void logTransaction(double amount) override {
        std::println("  PayPal transaction log: ${}", amount);
    }
};

void demonstratePaymentGateway() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 10: PAYMENT GATEWAY       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Processing payment via Stripe ---");
    unique_ptr<PaymentGateway> stripe = make_unique<StripeGateway>();
    bool success1 = stripe->processPayment(99.99);
    std::println("  Result: {}", (success1 ? "✅ Success" : "❌ Failed"));

    std::println("");
    std::println("--- Processing payment via PayPal ---");
    unique_ptr<PaymentGateway> paypal = make_unique<PayPalGateway>();
    bool success2 = paypal->processPayment(149.99);
    std::println("  Result: {}", (success2 ? "✅ Success" : "❌ Failed"));
}

// ============================================
// MAIN DEMONSTRATION
// ============================================

int main() {
    std::println("========================================");
    std::println("ABSTRACT CLASSES DEMONSTRATION");
    std::println("========================================");

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

    return 0;
}
