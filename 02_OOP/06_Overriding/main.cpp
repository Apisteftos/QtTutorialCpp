#include <iostream>
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
        cout << "  Generic animal sound\n";
    }

    void displayInfo() {
        cout << "  Animal: " << name << "\n";
    }
};

class DogNoVirtual : public AnimalNoVirtual {
public:
    DogNoVirtual(string n) : AnimalNoVirtual(n) { }

    // Overrides base, but NOT virtual
    void makeSound() {
        cout << "  " << name << " says: Woof! Woof!\n";
    }
};

class CatNoVirtual : public AnimalNoVirtual {
public:
    CatNoVirtual(string n) : AnimalNoVirtual(n) { }

    // Overrides base, but NOT virtual
    void makeSound() {
        cout << "  " << name << " says: Meow!\n";
    }
};

void demonstrateWithoutVirtual() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: WITHOUT VIRTUAL        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    DogNoVirtual dog("Buddy");
    CatNoVirtual cat("Whiskers");

    cout << "--- Direct calls (works as expected) ---\n";
    dog.makeSound();  // Calls DogNoVirtual::makeSound()
    cat.makeSound();  // Calls CatNoVirtual::makeSound()

    cout << "\n--- Using base class pointer (PROBLEM!) ---\n";
    AnimalNoVirtual* animals[2];
    animals[0] = &dog;
    animals[1] = &cat;

    for (int i = 0; i < 2; i++) {
        animals[i]->makeSound();  // Always calls AnimalNoVirtual::makeSound()!
    }

    cout << "\n⚠️  Problem:\n";
    cout << "   • Base pointer calls base version\n";
    cout << "   • Static binding (compile-time)\n";
    cout << "   • No polymorphism\n";
    cout << "   • Solution: Use virtual functions!\n";
}

// ============================================
// EXAMPLE 2: WITH VIRTUAL (Dynamic Binding)
// ============================================

class Animal {
protected:
    string name;

public:
    Animal(string n) : name(n) {
        cout << "  Animal constructor: " << name << "\n";
    }

    // Virtual destructor (IMPORTANT!)
    virtual ~Animal() {
        cout << "  Animal destructor: " << name << "\n";
    }

    // Virtual function
    virtual void makeSound() {
        cout << "  Generic animal sound\n";
    }

    // Virtual function
    virtual void move() {
        cout << "  " << name << " is moving\n";
    }

    // Non-virtual function
    void displayInfo() {
        cout << "  Animal: " << name << "\n";
    }
};

class Dog : public Animal {
public:
    Dog(string n) : Animal(n) {
        cout << "  Dog constructor: " << name << "\n";
    }

    ~Dog() {
        cout << "  Dog destructor: " << name << "\n";
    }

    // Override virtual function
    void makeSound() override {
        cout << "  " << name << " says: Woof! Woof!\n";
    }

    void move() override {
        cout << "  " << name << " runs on four legs\n";
    }
};

class Cat : public Animal {
public:
    Cat(string n) : Animal(n) {
        cout << "  Cat constructor: " << name << "\n";
    }

    ~Cat() {
        cout << "  Cat destructor: " << name << "\n";
    }

    // Override virtual function
    void makeSound() override {
        cout << "  " << name << " says: Meow!\n";
    }

    void move() override {
        cout << "  " << name << " walks silently\n";
    }
};

void demonstrateWithVirtual() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: WITH VIRTUAL           ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating animals ---\n";
    Dog dog("Buddy");
    Cat cat("Whiskers");

    cout << "\n--- Direct calls ---\n";
    dog.makeSound();
    cat.makeSound();

    cout << "\n--- Using base class pointer (WORKS!) ---\n";
    Animal* animals[2];
    animals[0] = &dog;
    animals[1] = &cat;

    for (int i = 0; i < 2; i++) {
        animals[i]->makeSound();  // Calls correct derived version!
        animals[i]->move();       // Polymorphic behavior!
    }

    cout << "\n✅ Virtual functions enable:\n";
    cout << "   • Dynamic binding (runtime)\n";
    cout << "   • Polymorphic behavior\n";
    cout << "   • Base pointer → derived implementation\n";

    cout << "\n--- Destruction ---\n";
}

// ============================================
// EXAMPLE 3: VIRTUAL DESTRUCTOR (CRITICAL!)
// ============================================

class BaseWithoutVirtualDestructor {
public:
    BaseWithoutVirtualDestructor() {
        cout << "  Base constructor\n";
    }

    // NOT virtual (PROBLEM!)
    ~BaseWithoutVirtualDestructor() {
        cout << "  Base destructor\n";
    }
};

class DerivedWithoutVirtualDestructor : public BaseWithoutVirtualDestructor {
private:
    int* data;

public:
    DerivedWithoutVirtualDestructor() {
        data = new int[100];
        cout << "  Derived constructor (allocated memory)\n";
    }

    ~DerivedWithoutVirtualDestructor() {
        delete[] data;
        cout << "  Derived destructor (freed memory)\n";
    }
};

class BaseWithVirtualDestructor {
public:
    BaseWithVirtualDestructor() {
        cout << "  Base constructor\n";
    }

    // Virtual destructor (CORRECT!)
    virtual ~BaseWithVirtualDestructor() {
        cout << "  Base destructor\n";
    }
};

class DerivedWithVirtualDestructor : public BaseWithVirtualDestructor {
private:
    int* data;

public:
    DerivedWithVirtualDestructor() {
        data = new int[100];
        cout << "  Derived constructor (allocated memory)\n";
    }

    ~DerivedWithVirtualDestructor() {
        delete[] data;
        cout << "  Derived destructor (freed memory)\n";
    }
};

void demonstrateVirtualDestructor() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: VIRTUAL DESTRUCTOR     ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- WITHOUT virtual destructor (MEMORY LEAK!) ---\n";
    {
        BaseWithoutVirtualDestructor* ptr = new DerivedWithoutVirtualDestructor();
        delete ptr;  // Only calls Base destructor! MEMORY LEAK!
    }

    cout << "\n--- WITH virtual destructor (CORRECT!) ---\n";
    {
        BaseWithVirtualDestructor* ptr = new DerivedWithVirtualDestructor();
        delete ptr;  // Calls both Derived and Base destructors!
    }

    cout << "\n⚠️  CRITICAL RULE:\n";
    cout << "   • ALWAYS make base destructor virtual\n";
    cout << "   • If you have virtual functions, destructor must be virtual\n";
    cout << "   • Otherwise: memory leaks, undefined behavior\n";
}

// ============================================
// EXAMPLE 4: OVERRIDE KEYWORD (C++11)
// ============================================

class BaseOverride {
public:
    virtual void func1() {
        cout << "  Base::func1()\n";
    }

    virtual void func2(int x) {
        cout << "  Base::func2(" << x << ")\n";
    }

    virtual void func3() const {
        cout << "  Base::func3() const\n";
    }

    virtual ~BaseOverride() = default;
};

class DerivedOverride : public BaseOverride {
public:
    // Correct override
    void func1() override {
        cout << "  Derived::func1()\n";
    }

    // Correct override
    void func2(int x) override {
        cout << "  Derived::func2(" << x << ")\n";
    }

    // Correct override
    void func3() const override {
        cout << "  Derived::func3() const\n";
    }

    // ❌ These would cause compile errors with 'override':
    // void func1(int x) override { }  // Different signature
    // void func2() override { }       // Missing parameter
    // void func3() override { }       // Missing const
};

void demonstrateOverride() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: OVERRIDE KEYWORD       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Using override keyword ---\n";
    DerivedOverride derived;
    BaseOverride* ptr = &derived;

    ptr->func1();
    ptr->func2(42);
    ptr->func3();

    cout << "\n✅ Benefits of 'override':\n";
    cout << "   • Catches typos in function names\n";
    cout << "   • Catches signature mismatches\n";
    cout << "   • Makes intent explicit\n";
    cout << "   • Compile-time checking\n";
    cout << "\n";
    cout << "   ALWAYS use 'override' in derived classes!\n";
}

// ============================================
// EXAMPLE 5: FINAL KEYWORD (C++11)
// ============================================

class BaseFinal {
public:
    virtual void canOverride() {
        cout << "  BaseFinal::canOverride()\n";
    }

    virtual void cannotOverride() final {  // FINAL - cannot override
        cout << "  BaseFinal::cannotOverride() - FINAL\n";
    }

    virtual ~BaseFinal() = default;
};

class DerivedFinal : public BaseFinal {
public:
    // OK - can override
    void canOverride() override {
        cout << "  DerivedFinal::canOverride()\n";
    }

    // ❌ Error! Cannot override final function
    // void cannotOverride() override { }
};

// Final class - cannot be inherited
class FinalClass final {
public:
    void someMethod() {
        cout << "  FinalClass::someMethod()\n";
    }
};

// ❌ Error! Cannot inherit from final class
// class CannotDerive : public FinalClass { };

void demonstrateFinal() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: FINAL KEYWORD          ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Using final keyword ---\n";
    DerivedFinal derived;
    BaseFinal* ptr = &derived;

    ptr->canOverride();       // Calls derived version
    ptr->cannotOverride();    // Calls base version (final)

    cout << "\n✅ Uses of 'final':\n";
    cout << "   • Prevent function overriding: virtual void func() final;\n";
    cout << "   • Prevent class inheritance: class MyClass final { };\n";
    cout << "   • Optimization (compiler knows no override)\n";
    cout << "   • Design intent (this is the final implementation)\n";
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
        cout << "  Drawing a " << color << " shape\n";
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
        cout << "  Drawing a " << color << " circle (radius=" << radius << ")\n";
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
        cout << "  Drawing a " << color << " rectangle ("
             << width << "×" << height << ")\n";
    }
};

void demonstratePureVirtual() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: PURE VIRTUAL           ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    // Shape shape("red");  // ❌ Error! Cannot instantiate abstract class

    cout << "--- Creating concrete shapes ---\n";
    Circle circle("red", 5.0);
    Rectangle rect("blue", 10.0, 5.0);

    cout << "\n--- Using polymorphism ---\n";
    vector<Shape*> shapes = { &circle, &rect };

    for (Shape* shape : shapes) {
        shape->draw();
        cout << "    Area: " << shape->area() << "\n";
        cout << "    Perimeter: " << shape->perimeter() << "\n\n";
    }

    cout << "✅ Pure virtual functions:\n";
    cout << "   • Declared with = 0\n";
    cout << "   • Makes class abstract (cannot instantiate)\n";
    cout << "   • Derived classes MUST implement\n";
    cout << "   • Defines interface contract\n";
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
        cout << "  Base: " << baseName << "\n";
    }

    virtual ~BaseSlice() = default;
};

class DerivedSlice : public BaseSlice {
private:
    string derivedName;

public:
    DerivedSlice(string b, string d) : BaseSlice(b), derivedName(d) { }

    void display() const override {
        cout << "  Base: " << baseName << ", Derived: " << derivedName << "\n";
    }
};

void demonstrateSlicing() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 7: SLICING PROBLEM        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    DerivedSlice derived("BaseData", "DerivedData");

    cout << "--- Correct: Using pointer/reference ---\n";
    BaseSlice* ptr = &derived;
    ptr->display();  // Polymorphic - calls DerivedSlice::display()

    BaseSlice& ref = derived;
    ref.display();   // Polymorphic - calls DerivedSlice::display()

    cout << "\n--- PROBLEM: Object slicing (by value) ---\n";
    BaseSlice sliced = derived;  // Object slicing!
    sliced.display();  // Calls BaseSlice::display() - derived part lost!

    cout << "\n⚠️  Object slicing:\n";
    cout << "   • Passing derived by value to base\n";
    cout << "   • Derived part is 'sliced off'\n";
    cout << "   • Only base part remains\n";
    cout << "   • Virtual functions don't work!\n";
    cout << "   • Solution: Always use pointers/references\n";
}

// ============================================
// EXAMPLE 8: VIRTUAL FUNCTION TABLE (vtable)
// ============================================

class BaseVTable {
public:
    virtual void func1() { cout << "  BaseVTable::func1()\n"; }
    virtual void func2() { cout << "  BaseVTable::func2()\n"; }
    virtual void func3() { cout << "  BaseVTable::func3()\n"; }

    void nonVirtual() { cout << "  BaseVTable::nonVirtual()\n"; }

    virtual ~BaseVTable() = default;
};

class DerivedVTable : public BaseVTable {
public:
    void func1() override { cout << "  DerivedVTable::func1()\n"; }
    void func2() override { cout << "  DerivedVTable::func2()\n"; }
    // func3 not overridden - uses base version

    void nonVirtual() { cout << "  DerivedVTable::nonVirtual()\n"; }
};

void demonstrateVTable() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 8: VTABLE MECHANISM       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- How virtual functions work ---\n";
    DerivedVTable derived;
    BaseVTable* ptr = &derived;

    cout << "\nCalling virtual functions (dynamic binding):\n";
    ptr->func1();  // Lookup in vtable → DerivedVTable::func1()
    ptr->func2();  // Lookup in vtable → DerivedVTable::func2()
    ptr->func3();  // Lookup in vtable → BaseVTable::func3()

    cout << "\nCalling non-virtual function (static binding):\n";
    ptr->nonVirtual();  // Direct call → BaseVTable::nonVirtual()

    cout << "\n💡 vtable mechanism:\n";
    cout << "   1. Each class with virtual functions has a vtable\n";
    cout << "   2. vtable stores pointers to virtual functions\n";
    cout << "   3. Each object has a vptr (points to class vtable)\n";
    cout << "   4. At runtime: follow vptr → vtable → function\n";
    cout << "   5. Small overhead: one pointer per object\n";
    cout << "\n";
    cout << "   BaseVTable vtable:       DerivedVTable vtable:\n";
    cout << "   ┌──────────────────┐     ┌──────────────────────┐\n";
    cout << "   │ func1() → Base   │     │ func1() → Derived    │\n";
    cout << "   │ func2() → Base   │     │ func2() → Derived    │\n";
    cout << "   │ func3() → Base   │     │ func3() → Base       │\n";
    cout << "   │ ~BaseVTable()    │     │ ~DerivedVTable()     │\n";
    cout << "   └──────────────────┘     └──────────────────────┘\n";
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
        cout << "  Processing credit card payment...\n";
        cout << "  Card: ****" << cardNumber.substr(cardNumber.length() - 4) << "\n";
        cout << "  Amount: $" << amount << "\n";
        cout << "  Status: Approved ✅\n";
        return true;
    }

    void printReceipt() const override {
        cout << "\n  ╔══════════════════════════════╗\n";
        cout << "  ║      CREDIT CARD RECEIPT     ║\n";
        cout << "  ╠══════════════════════════════╣\n";
        cout << "  ║ Transaction: " << transactionId << "     ║\n";
        cout << "  ║ Card: ****" << cardNumber.substr(cardNumber.length() - 4) << "             ║\n";
        cout << "  ║ Amount: $" << amount << "            ║\n";
        cout << "  ╚══════════════════════════════╝\n";
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
        cout << "  Processing PayPal payment...\n";
        cout << "  Email: " << email << "\n";
        cout << "  Amount: $" << amount << "\n";
        cout << "  Status: Approved ✅\n";
        return true;
    }

    void printReceipt() const override {
        cout << "\n  ╔══════════════════════════════╗\n";
        cout << "  ║       PAYPAL RECEIPT         ║\n";
        cout << "  ╠══════════════════════════════╣\n";
        cout << "  ║ Transaction: " << transactionId << "     ║\n";
        cout << "  ║ Email: " << email << "      ║\n";
        cout << "  ║ Amount: $" << amount << "            ║\n";
        cout << "  ╚══════════════════════════════╝\n";
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
        cout << "  Processing cryptocurrency payment...\n";
        cout << "  Wallet: " << walletAddress.substr(0, 10) << "...\n";
        cout << "  Currency: " << cryptocurrency << "\n";
        cout << "  Amount: $" << amount << "\n";
        cout << "  Status: Confirmed ✅\n";
        return true;
    }

    void printReceipt() const override {
        cout << "\n  ╔══════════════════════════════╗\n";
        cout << "  ║      CRYPTO RECEIPT          ║\n";
        cout << "  ╠══════════════════════════════╣\n";
        cout << "  ║ Transaction: " << transactionId << "     ║\n";
        cout << "  ║ Wallet: " << walletAddress.substr(0, 10) << "...    ║\n";
        cout << "  ║ Currency: " << cryptocurrency << "          ║\n";
        cout << "  ║ Amount: $" << amount << "            ║\n";
        cout << "  ╚══════════════════════════════╝\n";
    }

    string getPaymentType() const override {
        return "Cryptocurrency";
    }
};

void demonstratePaymentSystem() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 9: PAYMENT SYSTEM         ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating different payment methods ---\n";
    vector<unique_ptr<Payment>> payments;
    payments.push_back(make_unique<CreditCardPayment>(100.0, "TXN001", "1234567890123456", "123"));
    payments.push_back(make_unique<PayPalPayment>(75.50, "TXN002", "user@example.com"));
    payments.push_back(make_unique<CryptoPayment>(200.0, "TXN003", "1A2b3C4d5E6f7G8h", "Bitcoin"));

    cout << "\n--- Processing payments polymorphically ---\n";
    for (const auto& payment : payments) {
        cout << "\nPayment type: " << payment->getPaymentType() << "\n";
        payment->processPayment();
        payment->printReceipt();
    }

    cout << "\n✅ Virtual functions enable:\n";
    cout << "   • Common interface (Payment)\n";
    cout << "   • Different implementations (CreditCard, PayPal, Crypto)\n";
    cout << "   • Extensible design (easy to add new payment types)\n";
    cout << "   • Clean polymorphic code\n";
}

// ============================================
// EXAMPLE 10: COVARIANT RETURN TYPES
// ============================================

class BaseCloneable {
public:
    virtual BaseCloneable* clone() const {
        cout << "  Cloning BaseCloneable\n";
        return new BaseCloneable(*this);
    }

    virtual ~BaseCloneable() = default;
};

class DerivedCloneable : public BaseCloneable {
public:
    // Covariant return type - can return DerivedCloneable*
    DerivedCloneable* clone() const override {
        cout << "  Cloning DerivedCloneable\n";
        return new DerivedCloneable(*this);
    }
};

void demonstrateCovariantReturn() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 10: COVARIANT RETURN      ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Covariant return types ---\n";
    DerivedCloneable derived;

    BaseCloneable* basePtr = &derived;
    BaseCloneable* cloned1 = basePtr->clone();  // Returns BaseCloneable*

    DerivedCloneable* cloned2 = derived.clone(); // Returns DerivedCloneable*

    delete cloned1;
    delete cloned2;

    cout << "\n✅ Covariant return types:\n";
    cout << "   • Override can return derived pointer/reference\n";
    cout << "   • More specific return type in derived class\n";
    cout << "   • Useful for clone(), create() patterns\n";
}

// ============================================
// MAIN DEMONSTRATION
// ============================================

int main() {
    cout << "========================================\n";
    cout << "VIRTUAL FUNCTIONS DEMONSTRATION\n";
    cout << "========================================\n";

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

    // ============================================
    // Comprehensive Summary
    // ============================================
    cout << "\n========================================\n";
    cout << "WHAT ARE VIRTUAL FUNCTIONS?\n";
    cout << "========================================\n";
    cout << "Virtual functions enable runtime polymorphism,\n";
    cout << "allowing derived classes to override base class\n";
    cout << "behavior and base pointers to call derived\n";
    cout << "implementations.\n";
    cout << "\n";
    cout << "Key concepts:\n";
    cout << "  • Dynamic binding (runtime)\n";
    cout << "  • Polymorphic behavior\n";
    cout << "  • vtable mechanism\n";
    cout << "  • Pure virtual functions\n";
    cout << "  • Abstract classes\n";

    cout << "\n========================================\n";
    cout << "SYNTAX\n";
    cout << "========================================\n";
    cout << "BASE CLASS:\n";
    cout << "class Base {\n";
    cout << "public:\n";
    cout << "    // Virtual function\n";
    cout << "    virtual void func() {\n";
    cout << "        cout << \"Base::func()\";\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    // Pure virtual function\n";
    cout << "    virtual void pureFunc() = 0;\n";
    cout << "    \n";
    cout << "    // Virtual destructor (IMPORTANT!)\n";
    cout << "    virtual ~Base() { }\n";
    cout << "};\n";
    cout << "\n";
    cout << "DERIVED CLASS:\n";
    cout << "class Derived : public Base {\n";
    cout << "public:\n";
    cout << "    // Override with 'override' keyword\n";
    cout << "    void func() override {\n";
    cout << "        cout << \"Derived::func()\";\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    // Must implement pure virtual\n";
    cout << "    void pureFunc() override {\n";
    cout << "        cout << \"Derived::pureFunc()\";\n";
    cout << "    }\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "STATIC vs DYNAMIC BINDING\n";
    cout << "========================================\n";
    cout << "STATIC BINDING (without virtual):\n";
    cout << "  • Resolved at compile-time\n";
    cout << "  • Based on pointer/reference type\n";
    cout << "  • Faster (direct call)\n";
    cout << "  • No polymorphism\n";
    cout << "\n";
    cout << "Example:\n";
    cout << "Base* ptr = new Derived();\n";
    cout << "ptr->nonVirtualFunc();  // Calls Base version\n";
    cout << "\n";
    cout << "DYNAMIC BINDING (with virtual):\n";
    cout << "  • Resolved at runtime\n";
    cout << "  • Based on actual object type\n";
    cout << "  • Slightly slower (vtable lookup)\n";
    cout << "  • Enables polymorphism\n";
    cout << "\n";
    cout << "Example:\n";
    cout << "Base* ptr = new Derived();\n";
    cout << "ptr->virtualFunc();  // Calls Derived version\n";

    cout << "\n========================================\n";
    cout << "VIRTUAL DESTRUCTOR (CRITICAL!)\n";
    cout << "========================================\n";
    cout << "WITHOUT virtual destructor:\n";
    cout << "class Base {\n";
    cout << "    ~Base() { }  // ❌ NOT virtual\n";
    cout << "};\n";
    cout << "\n";
    cout << "Base* ptr = new Derived();\n";
    cout << "delete ptr;  // Only calls Base destructor!\n";
    cout << "             // Derived destructor NOT called!\n";
    cout << "             // MEMORY LEAK! ⚠️\n";
    cout << "\n";
    cout << "WITH virtual destructor:\n";
    cout << "class Base {\n";
    cout << "    virtual ~Base() { }  // ✅ Virtual\n";
    cout << "};\n";
    cout << "\n";
    cout << "Base* ptr = new Derived();\n";
    cout << "delete ptr;  // Calls Derived destructor first,\n";
    cout << "             // then Base destructor. ✅\n";
    cout << "\n";
    cout << "⚠️  RULE: If you have ANY virtual function,\n";
    cout << "    destructor MUST be virtual!\n";

    cout << "\n========================================\n";
    cout << "OVERRIDE KEYWORD (C++11)\n";
    cout << "========================================\n";
    cout << "WITHOUT override:\n";
    cout << "class Derived : public Base {\n";
    cout << "    void func(int x) { }  // Typo! Different signature\n";
    cout << "};\n";
    cout << "// Compiles but doesn't override! ❌\n";
    cout << "\n";
    cout << "WITH override:\n";
    cout << "class Derived : public Base {\n";
    cout << "    void func(int x) override { }  // Compiler error! ✅\n";
    cout << "};\n";
    cout << "// Compiler catches the mistake!\n";
    cout << "\n";
    cout << "Benefits:\n";
    cout << "  • Catches typos in function names\n";
    cout << "  • Catches signature mismatches\n";
    cout << "  • Makes intent explicit\n";
    cout << "  • Compile-time safety\n";
    cout << "\n";
    cout << "⚠️  RULE: ALWAYS use 'override' in derived classes!\n";

    cout << "\n========================================\n";
    cout << "FINAL KEYWORD (C++11)\n";
    cout << "========================================\n";
    cout << "FINAL FUNCTION:\n";
    cout << "class Base {\n";
    cout << "    virtual void func() final {  // Cannot override\n";
    cout << "        // This is the final implementation\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "class Derived : public Base {\n";
    cout << "    void func() override { }  // ❌ Error!\n";
    cout << "};\n";
    cout << "\n";
    cout << "FINAL CLASS:\n";
    cout << "class MyClass final {  // Cannot inherit\n";
    cout << "    // ...\n";
    cout << "};\n";
    cout << "\n";
    cout << "class CannotDerive : public MyClass { };  // ❌ Error!\n";
    cout << "\n";
    cout << "Uses:\n";
    cout << "  • Prevent further overriding\n";
    cout << "  • Prevent inheritance\n";
    cout << "  • Optimization opportunities\n";
    cout << "  • Design intent\n";

    cout << "\n========================================\n";
    cout << "PURE VIRTUAL FUNCTIONS\n";
    cout << "========================================\n";
    cout << "Syntax:\n";
    cout << "class Abstract {\n";
    cout << "public:\n";
    cout << "    // Pure virtual function\n";
    cout << "    virtual void func() = 0;\n";
    cout << "    \n";
    cout << "    virtual ~Abstract() = default;\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Abstract obj;  // ❌ Error! Cannot instantiate\n";
    cout << "\n";
    cout << "class Concrete : public Abstract {\n";
    cout << "public:\n";
    cout << "    // MUST implement pure virtual function\n";
    cout << "    void func() override {\n";
    cout << "        // Implementation\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "Concrete obj;  // ✅ OK! All pure virtuals implemented\n";
    cout << "\n";
    cout << "Characteristics:\n";
    cout << "  • Declared with = 0\n";
    cout << "  • Makes class abstract\n";
    cout << "  • Cannot instantiate abstract class\n";
    cout << "  • Derived classes MUST implement\n";
    cout << "  • Defines interface contract\n";

    cout << "\n========================================\n";
    cout << "ABSTRACT CLASSES\n";
    cout << "========================================\n";
    cout << "A class is abstract if:\n";
    cout << "  • Has at least one pure virtual function\n";
    cout << "  • OR inherits pure virtual without implementing\n";
    cout << "\n";
    cout << "Abstract class rules:\n";
    cout << "  ❌ Cannot instantiate\n";
    cout << "  ✅ Can have pointers/references\n";
    cout << "  ✅ Can have constructors\n";
    cout << "  ✅ Can have data members\n";
    cout << "  ✅ Can have non-pure virtual functions\n";
    cout << "  ✅ Can have regular functions\n";
    cout << "\n";
    cout << "Example:\n";
    cout << "class Shape {  // Abstract\n";
    cout << "public:\n";
    cout << "    virtual double area() = 0;  // Pure virtual\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Shape s;  // ❌ Error!\n";
    cout << "Shape* ptr;  // ✅ OK!\n";

    cout << "\n========================================\n";
    cout << "OBJECT SLICING\n";
    cout << "========================================\n";
    cout << "Problem:\n";
    cout << "Derived derived;\n";
    cout << "Base base = derived;  // Object slicing!\n";
    cout << "\n";
    cout << "What happens:\n";
    cout << "  • Derived part is 'sliced off'\n";
    cout << "  • Only Base part copied\n";
    cout << "  • Virtual functions don't work\n";
    cout << "  • Polymorphism lost\n";
    cout << "\n";
    cout << "Solution:\n";
    cout << "Base* ptr = &derived;   // ✅ Use pointer\n";
    cout << "Base& ref = derived;    // ✅ Use reference\n";
    cout << "\n";
    cout << "⚠️  RULE: Always use pointers or references\n";
    cout << "    for polymorphic behavior!\n";

    cout << "\n========================================\n";
    cout << "VTABLE MECHANISM\n";
    cout << "========================================\n";
    cout << "How virtual functions work:\n";
    cout << "\n";
    cout << "1. Each class with virtual functions has a vtable\n";
    cout << "   (virtual function table)\n";
    cout << "\n";
    cout << "2. vtable stores pointers to virtual functions\n";
    cout << "\n";
    cout << "3. Each object has a vptr (virtual pointer)\n";
    cout << "   pointing to class's vtable\n";
    cout << "\n";
    cout << "4. When calling virtual function:\n";
    cout << "   a. Follow vptr to vtable\n";
    cout << "   b. Lookup function pointer in vtable\n";
    cout << "   c. Call the function\n";
    cout << "\n";
    cout << "Visual:\n";
    cout << "┌─────────────┐\n";
    cout << "│  Object     │\n";
    cout << "├─────────────┤\n";
    cout << "│ vptr    ────┼──┐\n";
    cout << "│ data        │  │\n";
    cout << "└─────────────┘  │\n";
    cout << "                 ▼\n";
    cout << "         ┌──────────────┐\n";
    cout << "         │    vtable    │\n";
    cout << "         ├──────────────┤\n";
    cout << "         │ func1() ───► │ Function pointer\n";
    cout << "         │ func2() ───► │ Function pointer\n";
    cout << "         │ ~Destructor()│ Function pointer\n";
    cout << "         └──────────────┘\n";
    cout << "\n";
    cout << "Cost:\n";
    cout << "  • Space: One vptr per object (8 bytes on 64-bit)\n";
    cout << "  • Time: One extra indirection per virtual call\n";
    cout << "  • Usually negligible in practice\n";

    cout << "\n========================================\n";
    cout << "BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "✅ DO:\n";
    cout << "  • Make destructor virtual in polymorphic base classes\n";
    cout << "  • Use 'override' keyword in derived classes\n";
    cout << "  • Use 'final' to prevent overriding when needed\n";
    cout << "  • Use pure virtual for interface contracts\n";
    cout << "  • Use pointers/references for polymorphism\n";
    cout << "  • Make functions virtual if they might be overridden\n";
    cout << "  • Document which functions are meant to be overridden\n";
    cout << "\n";
    cout << "❌ DON'T:\n";
    cout << "  • Forget virtual destructor (memory leaks!)\n";
    cout << "  • Use virtual in derived without override\n";
    cout << "  • Call virtual functions in constructors/destructors\n";
    cout << "  • Overuse virtual (performance cost)\n";
    cout << "  • Pass polymorphic objects by value (slicing!)\n";
    cout << "  • Mix up function signatures (use override)\n";
    cout << "  • Make everything virtual unnecessarily\n";

    cout << "\n========================================\n";
    cout << "COMMON MISTAKES\n";
    cout << "========================================\n";
    cout << "❌ Mistake 1: Non-virtual destructor\n";
    cout << "class Base {\n";
    cout << "    ~Base() { }  // ❌ Memory leak!\n";
    cout << "};\n";
    cout << "Fix: virtual ~Base() { }\n";
    cout << "\n";
    cout << "❌ Mistake 2: Not using override\n";
    cout << "class Derived : public Base {\n";
    cout << "    void func(int x) { }  // Typo, doesn't override!\n";
    cout << "};\n";
    cout << "Fix: void func(int x) override { }\n";
    cout << "\n";
    cout << "❌ Mistake 3: Passing by value\n";
    cout << "void process(Base obj) {  // Slicing!\n";
    cout << "    obj.virtualFunc();\n";
    cout << "}\n";
    cout << "Fix: void process(Base& obj) { }\n";
    cout << "\n";
    cout << "❌ Mistake 4: Calling virtual in constructor\n";
    cout << "class Base {\n";
    cout << "    Base() {\n";
    cout << "        virtualFunc();  // Calls Base version!\n";
    cout << "    }\n";
    cout << "    virtual void virtualFunc() { }\n";
    cout << "};\n";
    cout << "Reason: Derived part not constructed yet\n";

    cout << "\n========================================\n";
    cout << "QUICK REFERENCE\n";
    cout << "========================================\n";
    cout << "// Base class\n";
    cout << "class Base {\n";
    cout << "public:\n";
    cout << "    // Virtual function\n";
    cout << "    virtual void func() {\n";
    cout << "        cout << \"Base::func()\";\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    // Pure virtual function\n";
    cout << "    virtual void pureFunc() = 0;\n";
    cout << "    \n";
    cout << "    // Virtual destructor\n";
    cout << "    virtual ~Base() { }\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Derived class\n";
    cout << "class Derived : public Base {\n";
    cout << "public:\n";
    cout << "    // Override virtual function\n";
    cout << "    void func() override {\n";
    cout << "        cout << \"Derived::func()\";\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    // Implement pure virtual\n";
    cout << "    void pureFunc() override {\n";
    cout << "        cout << \"Derived::pureFunc()\";\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Usage\n";
    cout << "Base* ptr = new Derived();\n";
    cout << "ptr->func();      // Calls Derived::func()\n";
    cout << "ptr->pureFunc();  // Calls Derived::pureFunc()\n";
    cout << "delete ptr;       // Calls both destructors\n";

    cout << "\n========================================\n";
    cout << "KEY TAKEAWAYS\n";
    cout << "========================================\n";
    cout << "1. Virtual functions enable runtime polymorphism\n";
    cout << "2. Use 'virtual' keyword in base class\n";
    cout << "3. Use 'override' keyword in derived class\n";
    cout << "4. ALWAYS make destructor virtual\n";
    cout << "5. Pure virtual (= 0) creates abstract class\n";
    cout << "6. Abstract classes cannot be instantiated\n";
    cout << "7. Use pointers/references for polymorphism\n";
    cout << "8. Object slicing loses polymorphic behavior\n";
    cout << "9. vtable mechanism has small overhead\n";
    cout << "10. 'final' keyword prevents further overriding\n";

    return 0;
}
