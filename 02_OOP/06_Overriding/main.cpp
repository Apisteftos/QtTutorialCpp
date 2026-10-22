#include <print>
#include <string>
#include <vector>
#include <memory>
using namespace std;

// ============================================
// WHAT ARE VIRTUAL FUNCTIONS?
// ============================================

/*
 * VIRTUAL FUNCTIONS:
 * - Functions that can be overridden in derived classes
 * - Enable runtime polymorphism (dynamic binding)
 * - Declared with 'virtual' keyword in base class
 * - Allow pointer/reference to base to call derived implementation
 *
 * WHY VIRTUAL FUNCTIONS?
 * - Without virtual: static binding (compile-time)
 * - With virtual: dynamic binding (runtime)
 * - Enables polymorphic behavior
 *
 * SYNTAX:
 * - Base: virtual ReturnType functionName();
 * - Derived: ReturnType functionName() override;
 *
 * IMPORTANT RULES:
 * - Virtual functions in base → can override in derived
 * - Pure virtual (= 0) → abstract class, must override
 * - Virtual destructor → ALWAYS use in polymorphic base
 * - Override keyword → C++11, prevents mistakes
 * - Final keyword → prevents further overriding
 *
 * VTABLE MECHANISM:
 * - Each class with virtual functions has a vtable
 * - Each object has a vptr (pointer to vtable)
 * - Runtime lookup determines which function to call
 */

// ============================================
// EXAMPLE 1: WITHOUT VIRTUAL (Static Binding)
// ============================================

class AnimalNoVirtual {
protected:
    string name;

public:
    AnimalNoVirtual(string n) : name(n) { }

    // NOT virtual
    void makeSound() {
        std::println("  Generic animal sound");
    }

    void displayInfo() {
        std::println("  Animal: {}", name);
    }
};

class DogNoVirtual : public AnimalNoVirtual {
public:
    DogNoVirtual(string n) : AnimalNoVirtual(n) { }

    // Overrides base, but NOT virtual
    void makeSound() {
        std::println("  {} says: Woof! Woof!", name);
    }
};

class CatNoVirtual : public AnimalNoVirtual {
public:
    CatNoVirtual(string n) : AnimalNoVirtual(n) { }

    // Overrides base, but NOT virtual
    void makeSound() {
        std::println("  {} says: Meow!", name);
    }
};

void demonstrateWithoutVirtual() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: WITHOUT VIRTUAL        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    DogNoVirtual dog("Buddy");
    CatNoVirtual cat("Whiskers");

    std::println("--- Direct calls (works as expected) ---");
    dog.makeSound();  // Calls DogNoVirtual::makeSound()
    cat.makeSound();  // Calls CatNoVirtual::makeSound()

    std::println("");
    std::println("--- Using base class pointer (PROBLEM!) ---");
    AnimalNoVirtual* animals[2];
    animals[0] = &dog;
    animals[1] = &cat;

    for (int i = 0; i < 2; i++) {
        animals[i]->makeSound();  // Always calls AnimalNoVirtual::makeSound()!
    }
}

// ============================================
// EXAMPLE 2: WITH VIRTUAL (Dynamic Binding)
// ============================================

class Animal {
protected:
    string name;

public:
    Animal(string n) : name(n) {
        std::println("  Animal constructor: {}", name);
    }

    // Virtual destructor (IMPORTANT!)
    virtual ~Animal() {
        std::println("  Animal destructor: {}", name);
    }

    // Virtual function
    virtual void makeSound() {
        std::println("  Generic animal sound");
    }

    // Virtual function
    virtual void move() {
        std::println("  {} is moving", name);
    }

    // Non-virtual function
    void displayInfo() {
        std::println("  Animal: {}", name);
    }
};

class Dog : public Animal {
public:
    Dog(string n) : Animal(n) {
        std::println("  Dog constructor: {}", name);
    }

    ~Dog() {
        std::println("  Dog destructor: {}", name);
    }

    // Override virtual function
    void makeSound() override {
        std::println("  {} says: Woof! Woof!", name);
    }

    void move() override {
        std::println("  {} runs on four legs", name);
    }
};

class Cat : public Animal {
public:
    Cat(string n) : Animal(n) {
        std::println("  Cat constructor: {}", name);
    }

    ~Cat() {
        std::println("  Cat destructor: {}", name);
    }

    // Override virtual function
    void makeSound() override {
        std::println("  {} says: Meow!", name);
    }

    void move() override {
        std::println("  {} walks silently", name);
    }
};

void demonstrateWithVirtual() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: WITH VIRTUAL           ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating animals ---");
    Dog dog("Buddy");
    Cat cat("Whiskers");

    std::println("");
    std::println("--- Direct calls ---");
    dog.makeSound();
    cat.makeSound();

    std::println("");
    std::println("--- Using base class pointer (WORKS!) ---");
    Animal* animals[2];
    animals[0] = &dog;
    animals[1] = &cat;

    for (int i = 0; i < 2; i++) {
        animals[i]->makeSound();  // Calls correct derived version!
        animals[i]->move();       // Polymorphic behavior!
    }


    std::println("");
    std::println("--- Destruction ---");
}

// ============================================
// EXAMPLE 3: VIRTUAL DESTRUCTOR (CRITICAL!)
// ============================================

class BaseWithoutVirtualDestructor {
public:
    BaseWithoutVirtualDestructor() {
        std::println("  Base constructor");
    }

    // NOT virtual (PROBLEM!)
    ~BaseWithoutVirtualDestructor() {
        std::println("  Base destructor");
    }
};

class DerivedWithoutVirtualDestructor : public BaseWithoutVirtualDestructor {
private:
    int* data;

public:
    DerivedWithoutVirtualDestructor() {
        data = new int[100];
        std::println("  Derived constructor (allocated memory)");
    }

    ~DerivedWithoutVirtualDestructor() {
        delete[] data;
        std::println("  Derived destructor (freed memory)");
    }
};

class BaseWithVirtualDestructor {
public:
    BaseWithVirtualDestructor() {
        std::println("  Base constructor");
    }

    // Virtual destructor (CORRECT!)
    virtual ~BaseWithVirtualDestructor() {
        std::println("  Base destructor");
    }
};

class DerivedWithVirtualDestructor : public BaseWithVirtualDestructor {
private:
    int* data;

public:
    DerivedWithVirtualDestructor() {
        data = new int[100];
        std::println("  Derived constructor (allocated memory)");
    }

    ~DerivedWithVirtualDestructor() {
        delete[] data;
        std::println("  Derived destructor (freed memory)");
    }
};

void demonstrateVirtualDestructor() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: VIRTUAL DESTRUCTOR     ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- WITHOUT virtual destructor (MEMORY LEAK!) ---");
    {
        BaseWithoutVirtualDestructor* ptr = new DerivedWithoutVirtualDestructor();
        delete ptr;  // Only calls Base destructor! MEMORY LEAK!
    }

    std::println("");
    std::println("--- WITH virtual destructor (CORRECT!) ---");
    {
        BaseWithVirtualDestructor* ptr = new DerivedWithVirtualDestructor();
        delete ptr;  // Calls both Derived and Base destructors!
    }
}

// ============================================
// EXAMPLE 4: OVERRIDE KEYWORD (C++11)
// ============================================

class BaseOverride {
public:
    virtual void func1() {
        std::println("  Base::func1()");
    }

    virtual void func2(int x) {
        std::println("  Base::func2({})", x);
    }

    virtual void func3() const {
        std::println("  Base::func3() const");
    }

    virtual ~BaseOverride() = default;
};

class DerivedOverride : public BaseOverride {
public:
    // Correct override
    void func1() override {
        std::println("  Derived::func1()");
    }

    // Correct override
    void func2(int x) override {
        std::println("  Derived::func2({})", x);
    }

    // Correct override
    void func3() const override {
        std::println("  Derived::func3() const");
    }

    // ❌ These would cause compile errors with 'override':
    // void func1(int x) override { }  // Different signature
    // void func2() override { }       // Missing parameter
    // void func3() override { }       // Missing const
};

void demonstrateOverride() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: OVERRIDE KEYWORD       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Using override keyword ---");
    DerivedOverride derived;
    BaseOverride* ptr = &derived;

    ptr->func1();
    ptr->func2(42);
    ptr->func3();
}

// ============================================
// EXAMPLE 5: FINAL KEYWORD (C++11)
// ============================================

class BaseFinal {
public:
    virtual void canOverride() {
        std::println("  BaseFinal::canOverride()");
    }

    virtual void cannotOverride() final {  // FINAL - cannot override
        std::println("  BaseFinal::cannotOverride() - FINAL");
    }

    virtual ~BaseFinal() = default;
};

class DerivedFinal : public BaseFinal {
public:
    // OK - can override
    void canOverride() override {
        std::println("  DerivedFinal::canOverride()");
    }

    // ❌ Error! Cannot override final function
    // void cannotOverride() override { }
};

// Final class - cannot be inherited
class FinalClass final {
public:
    void someMethod() {
        std::println("  FinalClass::someMethod()");
    }
};

// ❌ Error! Cannot inherit from final class
// class CannotDerive : public FinalClass { };

void demonstrateFinal() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 5: FINAL KEYWORD          ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Using final keyword ---");
    DerivedFinal derived;
    BaseFinal* ptr = &derived;

    ptr->canOverride();       // Calls derived version
    ptr->cannotOverride();    // Calls base version (final)
}

// ============================================
// EXAMPLE 6: PURE VIRTUAL FUNCTIONS (Abstract Classes)
// ============================================

// Abstract class - has pure virtual function
class Shape {
protected:
    string color;

public:
    Shape(string c) : color(c) { }

    virtual ~Shape() = default;

    // Pure virtual function (= 0)
    virtual double area() const = 0;
    virtual double perimeter() const = 0;

    // Regular virtual function
    virtual void draw() const {
        std::println("  Drawing a {} shape", color);
    }

    // Non-virtual function
    void setColor(string c) {
        color = c;
    }
};

class Circle : public Shape {
private:
    double radius;

public:
    Circle(string c, double r) : Shape(c), radius(r) { }

    // MUST implement pure virtual functions
    double area() const override {
        return 3.14159 * radius * radius;
    }

    double perimeter() const override {
        return 2 * 3.14159 * radius;
    }

    void draw() const override {
        std::println("  Drawing a {} circle (radius={})", color, radius);
    }
};

class Rectangle : public Shape {
private:
    double width, height;

public:
    Rectangle(string c, double w, double h) : Shape(c), width(w), height(h) { }

    // MUST implement pure virtual functions
    double area() const override {
        return width * height;
    }

    double perimeter() const override {
        return 2 * (width + height);
    }

    void draw() const override {
        std::println("  Drawing a {} rectangle ({}×{})", color, width, height);
    }
};

void demonstratePureVirtual() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 6: PURE VIRTUAL           ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    // Shape shape("red");  // ❌ Error! Cannot instantiate abstract class

    std::println("--- Creating concrete shapes ---");
    Circle circle("red", 5.0);
    Rectangle rect("blue", 10.0, 5.0);

    std::println("");
    std::println("--- Using polymorphism ---");
    vector<Shape*> shapes = { &circle, &rect };

    for (Shape* shape : shapes) {
        shape->draw();
        std::println("    Area: {}", shape->area());
        std::println("    Perimeter: {}", shape->perimeter());
        std::println("");
    }
}

// ============================================
// EXAMPLE 7: SLICING PROBLEM
// ============================================

class BaseSlice {
protected:
    string baseName;

public:
    BaseSlice(string n) : baseName(n) { }

    virtual void display() const {
        std::println("  Base: {}", baseName);
    }

    virtual ~BaseSlice() = default;
};

class DerivedSlice : public BaseSlice {
private:
    string derivedName;

public:
    DerivedSlice(string b, string d) : BaseSlice(b), derivedName(d) { }

    void display() const override {
        std::println("  Base: {}, Derived: {}", baseName, derivedName);
    }
};

void demonstrateSlicing() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 7: SLICING PROBLEM        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    DerivedSlice derived("BaseData", "DerivedData");

    std::println("--- Correct: Using pointer/reference ---");
    BaseSlice* ptr = &derived;
    ptr->display();  // Polymorphic - calls DerivedSlice::display()

    BaseSlice& ref = derived;
    ref.display();   // Polymorphic - calls DerivedSlice::display()

    std::println("");
    std::println("--- PROBLEM: Object slicing (by value) ---");
    BaseSlice sliced = derived;  // Object slicing!
    sliced.display();  // Calls BaseSlice::display() - derived part lost!
}

// ============================================
// EXAMPLE 8: VIRTUAL FUNCTION TABLE (vtable)
// ============================================

class BaseVTable {
public:
    virtual void func1() { std::println("  BaseVTable::func1()"); }
    virtual void func2() { std::println("  BaseVTable::func2()"); }
    virtual void func3() { std::println("  BaseVTable::func3()"); }

    void nonVirtual() { std::println("  BaseVTable::nonVirtual()"); }

    virtual ~BaseVTable() = default;
};

class DerivedVTable : public BaseVTable {
public:
    void func1() override { std::println("  DerivedVTable::func1()"); }
    void func2() override { std::println("  DerivedVTable::func2()"); }
    // func3 not overridden - uses base version

    void nonVirtual() { std::println("  DerivedVTable::nonVirtual()"); }
};

void demonstrateVTable() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 8: VTABLE MECHANISM       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- How virtual functions work ---");
    DerivedVTable derived;
    BaseVTable* ptr = &derived;

    std::println("");
    std::println("Calling virtual functions (dynamic binding):");
    ptr->func1();  // Lookup in vtable → DerivedVTable::func1()
    ptr->func2();  // Lookup in vtable → DerivedVTable::func2()
    ptr->func3();  // Lookup in vtable → BaseVTable::func3()

    std::println("");
    std::println("Calling non-virtual function (static binding):");
    ptr->nonVirtual();  // Direct call → BaseVTable::nonVirtual()
}

// ============================================
// EXAMPLE 9: REAL-WORLD - PAYMENT SYSTEM
// ============================================

class Payment {
protected:
    double amount;
    string transactionId;

public:
    Payment(double amt, string id) : amount(amt), transactionId(id) { }

    virtual ~Payment() = default;

    // Pure virtual - must implement
    virtual bool processPayment() = 0;
    virtual void printReceipt() const = 0;

    // Regular virtual - can override
    virtual string getPaymentType() const {
        return "Generic Payment";
    }

    // Non-virtual
    double getAmount() const {
        return amount;
    }
};

class CreditCardPayment : public Payment {
private:
    string cardNumber;
    string cvv;

public:
    CreditCardPayment(double amt, string id, string card, string c)
        : Payment(amt, id), cardNumber(card), cvv(c) { }

    bool processPayment() override {
        std::println("  Processing credit card payment...");
        std::println("  Card: ****{}", cardNumber.substr(cardNumber.length() - 4));
        std::println("  Amount: ${}", amount);
        std::println("  Status: Approved ✅");
        return true;
    }

    void printReceipt() const override {
        std::println("");
        std::println("  ╔══════════════════════════════╗");
        std::println("  ║      CREDIT CARD RECEIPT     ║");
        std::println("  ╠══════════════════════════════╣");
        std::println("  ║ Transaction: {}          ║", transactionId);
        std::println("  ║ Card: ****{}               ║", cardNumber.substr(cardNumber.length() - 4));
        std::println("  ║ Amount: ${}                 ║", amount);
        std::println("  ╚══════════════════════════════╝");
    }

    string getPaymentType() const override {
        return "Credit Card";
    }
};

class PayPalPayment : public Payment {
private:
    string email;

public:
    PayPalPayment(double amt, string id, string e)
        : Payment(amt, id), email(e) { }

    bool processPayment() override {
        std::println("  Processing PayPal payment...");
        std::println("  Email: {}", email);
        std::println("  Amount: ${}", amount);
        std::println("  Status: Approved ✅");
        return true;
    }

    void printReceipt() const override {
        std::println("");
        std::println("  ╔══════════════════════════════╗");
        std::println("  ║       PAYPAL RECEIPT         ║");
        std::println("  ╠══════════════════════════════╣");
        std::println("  ║ Transaction: {}          ║", transactionId);
        std::println("  ║ Email: {}      ║", email);
        std::println("  ║ Amount: ${}                ║", amount);
        std::println("  ╚══════════════════════════════╝");
    }

    string getPaymentType() const override {
        return "PayPal";
    }
};

class CryptoPayment : public Payment {
private:
    string walletAddress;
    string cryptocurrency;

public:
    CryptoPayment(double amt, string id, string wallet, string crypto)
        : Payment(amt, id), walletAddress(wallet), cryptocurrency(crypto) { }

    bool processPayment() override {
        std::println("  Processing cryptocurrency payment...");
        std::println("  Wallet: {}...", walletAddress.substr(0, 10));
        std::println("  Currency: {}", cryptocurrency);
        std::println("  Amount: ${}", amount);
        std::println("  Status: Confirmed ✅");
        return true;
    }

    void printReceipt() const override {
        std::println("");
        std::println("  ╔══════════════════════════════╗");
        std::println("  ║      CRYPTO RECEIPT          ║");
        std::println("  ╠══════════════════════════════╣");
        std::println("  ║ Transaction: {}          ║", transactionId);
        std::println("  ║ Wallet: {}...        ║", walletAddress.substr(0, 10));
        std::println("  ║ Currency: {}            ║", cryptocurrency);
        std::println("  ║ Amount: ${}                 ║", amount);
        std::println("  ╚══════════════════════════════╝");
    }

    string getPaymentType() const override {
        return "Cryptocurrency";
    }
};

void demonstratePaymentSystem() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 9: PAYMENT SYSTEM         ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating different payment methods ---");
    vector<unique_ptr<Payment>> payments;
    payments.push_back(make_unique<CreditCardPayment>(100.0, "TXN001", "1234567890123456", "123"));
    payments.push_back(make_unique<PayPalPayment>(75.50, "TXN002", "user@example.com"));
    payments.push_back(make_unique<CryptoPayment>(200.0, "TXN003", "1A2b3C4d5E6f7G8h", "Bitcoin"));

    std::println("");
    std::println("--- Processing payments polymorphically ---");
    for (const auto& payment : payments) {
        std::println("");
        std::println("Payment type: {}", payment->getPaymentType());
        payment->processPayment();
        payment->printReceipt();
    }
}

// ============================================
// EXAMPLE 10: COVARIANT RETURN TYPES
// ============================================

class BaseCloneable {
public:
    virtual BaseCloneable* clone() const {
        std::println("  Cloning BaseCloneable");
        return new BaseCloneable(*this);
    }

    virtual ~BaseCloneable() = default;
};

class DerivedCloneable : public BaseCloneable {
public:
    // Covariant return type - can return DerivedCloneable*
    DerivedCloneable* clone() const override {
        std::println("  Cloning DerivedCloneable");
        return new DerivedCloneable(*this);
    }
};

void demonstrateCovariantReturn() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 10: COVARIANT RETURN      ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Covariant return types ---");
    DerivedCloneable derived;

    BaseCloneable* basePtr = &derived;
    BaseCloneable* cloned1 = basePtr->clone();  // Returns BaseCloneable*

    DerivedCloneable* cloned2 = derived.clone(); // Returns DerivedCloneable*

    delete cloned1;
    delete cloned2;
}

// ============================================
// MAIN DEMONSTRATION
// ============================================

int main() {
    std::println("========================================");
    std::println("VIRTUAL FUNCTIONS DEMONSTRATION");
    std::println("========================================");

    demonstrateWithoutVirtual();
    demonstrateWithVirtual();
    demonstrateVirtualDestructor();
    demonstrateOverride();
    demonstrateFinal();
    demonstratePureVirtual();
    demonstrateSlicing();
    demonstrateVTable();
    demonstratePaymentSystem();
    demonstrateCovariantReturn();

    return 0;
}
