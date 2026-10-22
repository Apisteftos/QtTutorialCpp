#include <print>
#include <string>
#include <vector>
#include <memory>
#include <typeinfo>
#include <functional>

using namespace std;

// ============================================
// WHAT IS POLYMORPHISM?
// ============================================

/*
 * POLYMORPHISM = "many forms"
 * The ability of different objects to be treated
 * as instances of the same type, while behaving
 * differently based on their actual type.
 *
 * TWO TYPES IN C++:
 *
 * 1. COMPILE-TIME (Static) Polymorphism
 *    - Resolved at compile time
 *    - Function overloading
 *    - Operator overloading
 *    - Templates
 *    - No runtime cost
 *
 * 2. RUNTIME (Dynamic) Polymorphism
 *    - Resolved at runtime via vtable
 *    - Virtual functions
 *    - Base class pointers/references
 *    - Small runtime cost (vtable lookup)
 *
 * KEY INSIGHT:
 *    Same call → different behavior
 *    Based on ACTUAL type, not declared type
 */

// ============================================
// EXAMPLE 1: RUNTIME POLYMORPHISM — THE CORE IDEA
// ============================================

/*
 * Without polymorphism you'd write:
 *   if (type == "dog") dog.speak();
 *   else if (type == "cat") cat.speak();
 *
 * With polymorphism:
 *   animal->speak();  // correct version called automatically
 */

class Animal {
protected:
    string name;
public:
    Animal(const string& n) : name(n) {}
    virtual ~Animal() = default;

    virtual void speak() const = 0;
    virtual void move()  const = 0;

    string getName() const { return name; }
};

class Dog : public Animal {
public:
    Dog(const string& n) : Animal(n) {}
    void speak() const override { std::println("  {}: Woof!", name); }
    void move()  const override { std::println("  {} runs on four legs", name); }
};

class Cat : public Animal {
public:
    Cat(const string& n) : Animal(n) {}
    void speak() const override { std::println("  {}: Meow!", name); }
    void move()  const override { std::println("  {} pads silently", name); }
};

class Bird : public Animal {
public:
    Bird(const string& n) : Animal(n) {}
    void speak() const override { std::println("  {}: Tweet!", name); }
    void move()  const override { std::println("  {} flies through the air", name); }
};

// One function handles ALL animal types — this is the power
void makeAnimalAct(const Animal& a) {
    a.speak();
    a.move();
}

void demonstrateRuntimePolymorphism() {
    std::println("");
    std::println("╔══════════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: RUNTIME POLYMORPHISM         ║");
    std::println("╚══════════════════════════════════════════╝");
    std::println("");

    Dog  dog("Rex");
    Cat  cat("Whiskers");
    Bird bird("Tweety");

    std::println("--- Calling makeAnimalAct() on different types ---");
    std::println("  Same function call → different behavior:");
    std::println("");
    makeAnimalAct(dog);
    std::println("");
    makeAnimalAct(cat);
    std::println("");
    makeAnimalAct(bird);

    std::println("");
    std::println("--- Processing a mixed collection ---");
    vector<unique_ptr<Animal>> zoo;
    zoo.push_back(make_unique<Dog>("Buddy"));
    zoo.push_back(make_unique<Cat>("Luna"));
    zoo.push_back(make_unique<Bird>("Sky"));
    zoo.push_back(make_unique<Dog>("Max"));

    for (const auto& animal : zoo) {
        animal->speak();   // correct version for each
    }
}

// ============================================
// EXAMPLE 2: VTABLE — HOW RUNTIME DISPATCH WORKS
// ============================================

/*
 * Every class with virtual functions has a VTABLE:
 * a table of function pointers, one per virtual function.
 *
 * Each object stores a hidden VPTR pointing to its class's vtable.
 *
 *   Dog object:    [vptr] → Dog's vtable → { Dog::speak, Dog::move }
 *   Cat object:    [vptr] → Cat's vtable → { Cat::speak, Cat::move }
 *
 * When you call animal->speak():
 *   1. Follow vptr to vtable
 *   2. Look up speak() slot
 *   3. Call the function found there
 *
 * Cost: one extra pointer dereference per virtual call.
 * Worth it for the flexibility gained.
 */

class Base {
public:
    virtual void show() const { std::println("  Base::show()"); }
    virtual void display() const { std::println("  Base::display()"); }
    virtual ~Base() = default;
};

class Derived : public Base {
public:
    void show() const override { std::println("  Derived::show()"); }
    // display() NOT overridden — inherits Base version
};

void demonstrateVtable() {
    std::println("");
    std::println("╔══════════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: VTABLE DISPATCH              ║");
    std::println("╚══════════════════════════════════════════╝");
    std::println("");

    Derived d;
    Base* ptr = &d;   // Base pointer → Derived object

    std::println("--- ptr points to a Derived object ---");
    ptr->show();      // Derived::show()    — overridden
    ptr->display();   // Base::display()    — not overridden, uses Base

    std::println("");
    std::println("--- Demonstrating slice vs pointer ---");
    Base copy = d;    // Object slicing — Derived part is cut off
    std::println("  Via sliced copy (no polymorphism):");
    copy.show();      // Base::show() — vptr was NOT copied

    std::println("  Via pointer (polymorphism):");
    ptr->show();      // Derived::show() — vptr intact
}

// ============================================
// EXAMPLE 3: COMPILE-TIME POLYMORPHISM
// ============================================

/*
 * Resolved entirely at compile time — zero runtime cost.
 * The compiler picks the right version based on types.
 */

// --- Function overloading ---
void print(int x)         { std::println("  int: {}", x); }
void print(double x)      { std::println("  double: {}", x); }
void print(const string& x) { std::println("  string: {}", x); }

// --- Operator overloading ---
class Vector2D {
public:
    double x, y;
    Vector2D(double x, double y) : x(x), y(y) {}

    Vector2D operator+(const Vector2D& other) const {
        return { x + other.x, y + other.y };
    }

    bool operator==(const Vector2D& other) const {
        return x == other.x && y == other.y;
    }

    void print() const {
        std::println("  ({}, {})", x, y);
    }
};

// --- Template polymorphism ---
template<typename T>
T maxOf(T a, T b) {
    return (a > b) ? a : b;
}

template<typename T>
void printContainer(const vector<T>& v) {
    for (const auto& item : v) {
        std::print("  {}", item);
    }
    std::println("");
}

void demonstrateCompileTime() {
    std::println("");
    std::println("╔══════════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: COMPILE-TIME POLYMORPHISM    ║");
    std::println("╚══════════════════════════════════════════╝");
    std::println("");

    std::println("--- Function overloading ---");
    print(42);
    print(3.14);
    print(string("hello"));

    std::println("");
    std::println("--- Operator overloading ---");
    Vector2D v1(1.0, 2.0), v2(3.0, 4.0);
    Vector2D v3 = v1 + v2;
    std::print("  v1 + v2 = ");
    v3.print();

    std::println("");
    std::println("--- Template polymorphism ---");
    std::println("  maxOf(3, 7)       = {}", maxOf(3, 7));
    std::println("  maxOf(1.5, 0.8)   = {}", maxOf(1.5, 0.8));
    std::println("  maxOf('a', 'z')   = {}", maxOf('a', 'z'));

    std::println("");
    std::print("  printContainer<int>:    ");
    printContainer<int>({1,2,3,4,5});
    std::print("  printContainer<string>: ");
    printContainer<string>({"foo","bar","baz"});
}

// ============================================
// EXAMPLE 4: POLYMORPHISM IN A REAL SYSTEM
// ============================================

/*
 * Payment processing system.
 * Focus: how polymorphism lets you ADD new payment types
 * without changing any existing code (Open/Closed Principle).
 */

class PaymentMethod {
public:
    virtual bool   processPayment(double amount) = 0;
    virtual string getProviderName()       const = 0;
    virtual double getFeePercent()         const = 0;
    virtual ~PaymentMethod() = default;

    // Non-virtual — same for all, uses virtual calls internally
    void execute(double amount) {
        double fee   = amount * getFeePercent() / 100.0;
        double total = amount + fee;
        std::println("  Provider : {}", getProviderName());
        std::println("  Amount   : €{:.2f}", amount);
        std::println("  Fee      : €{:.2f} ({}%)", fee, getFeePercent());
        std::println("  Total    : €{:.2f}", total);
        bool ok = processPayment(total);
        std::println("  Status   : {}", (ok ? "✅ Approved" : "❌ Declined"));
    }
};

class CreditCard : public PaymentMethod {
    string last4;
public:
    CreditCard(const string& l) : last4(l) {}
    bool   processPayment(double amount) override {
        std::println("  Charging card **** {}", last4);
        return true;
    }
    string getProviderName() const override { return "Visa/Mastercard"; }
    double getFeePercent()   const override { return 1.5; }
};

class PayPal : public PaymentMethod {
    string email;
public:
    PayPal(const string& e) : email(e) {}
    bool   processPayment(double amount) override {
        std::println("  Sending PayPal request to {}", email);
        return true;
    }
    string getProviderName() const override { return "PayPal"; }
    double getFeePercent()   const override { return 2.9; }
};

class Crypto : public PaymentMethod {
    string wallet;
public:
    Crypto(const string& w) : wallet(w) {}
    bool   processPayment(double amount) override {
        std::println("  Broadcasting to wallet {}...", wallet.substr(0,8));
        return true;
    }
    string getProviderName() const override { return "Bitcoin"; }
    double getFeePercent()   const override { return 0.1; }
};

// Checkout knows NOTHING about specific payment types
void checkout(PaymentMethod& method, double amount) {
    std::println("");
    std::println("  --- Processing payment ---");
    method.execute(amount);
}

void demonstratePaymentSystem() {
    std::println("");
    std::println("╔══════════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: REAL-WORLD PAYMENT SYSTEM    ║");
    std::println("╚══════════════════════════════════════════╝");
    std::println("");

    CreditCard card("4242");
    PayPal     paypal("kostas@email.com");
    Crypto     crypto("1A2B3C4D5E6F7G8H");

    checkout(card,   99.99);
    checkout(paypal, 49.50);
    checkout(crypto, 200.00);

    std::println("");
    std::println("--- Same checkout() function, three different behaviors ---");
}

// ============================================
// EXAMPLE 5: POLYMORPHISM vs TYPE CHECKING
// ============================================

/*
 * ANTI-PATTERN — what code looks like WITHOUT polymorphism:
 */

enum class ShapeType { CIRCLE, SQUARE, TRIANGLE };

struct ShapeBad {
    ShapeType type;
    double size;
};

double areaBad(const ShapeBad& s) {
    // Every time you add a shape you must edit this switch
    switch (s.type) {
    case ShapeType::CIRCLE:   return 3.14159 * s.size * s.size;
    case ShapeType::SQUARE:   return s.size * s.size;
    case ShapeType::TRIANGLE: return 0.5 * s.size * s.size;
    default: return 0;
    }
}

/*
 * CORRECT PATTERN — with polymorphism:
 */

class ShapeGood {
public:
    virtual double area() const = 0;
    virtual string typeName() const = 0;
    virtual ~ShapeGood() = default;
};

class Circle   : public ShapeGood {
    double r;
public:
    Circle(double r) : r(r) {}
    double area()     const override { return 3.14159 * r * r; }
    string typeName() const override { return "Circle"; }
};

class Square   : public ShapeGood {
    double s;
public:
    Square(double s) : s(s) {}
    double area()     const override { return s * s; }
    string typeName() const override { return "Square"; }
};

class Triangle : public ShapeGood {
    double b, h;
public:
    Triangle(double b, double h) : b(b), h(h) {}
    double area()     const override { return 0.5 * b * h; }
    string typeName() const override { return "Triangle"; }
};

void demonstrateVsTypechecking() {
    std::println("");
    std::println("╔══════════════════════════════════════════╗");
    std::println("║  EXAMPLE 5: POLYMORPHISM vs TYPE-CHECK   ║");
    std::println("╚══════════════════════════════════════════╝");
    std::println("");

    std::println("--- Anti-pattern: switch/if-else on type ---");
    vector<ShapeBad> badShapes = {
        {ShapeType::CIRCLE, 5.0},
        {ShapeType::SQUARE, 4.0},
        {ShapeType::TRIANGLE, 3.0}
    };
    for (const auto& s : badShapes) {
        std::println("  area = {}", areaBad(s));
    }
    std::println("  ❌ Adding a new shape requires editing areaBad()");

    std::println("");
    std::println("--- Correct: polymorphic dispatch ---");
    vector<unique_ptr<ShapeGood>> shapes;
    shapes.push_back(make_unique<Circle>(5.0));
    shapes.push_back(make_unique<Square>(4.0));
    shapes.push_back(make_unique<Triangle>(3.0, 6.0));

    for (const auto& s : shapes) {
        std::println("  {} area = {}", s->typeName(), s->area());
    }
    std::println("  ✅ Adding a new shape: just add a new class, nothing else changes");
}

// ============================================
// EXAMPLE 6: COVARIANT RETURN TYPES
// ============================================

/*
 * A special polymorphism feature:
 * An overriding function can return a MORE DERIVED type
 * than the base class virtual function declares.
 */

class Document {
public:
    virtual Document* clone() const {
        return new Document(*this);
    }
    virtual string type() const { return "Document"; }
    virtual ~Document() = default;
};

class PDFDocument : public Document {
public:
    // Returns PDFDocument* — more derived than Document*
    // This is valid covariant return
    PDFDocument* clone() const override {
        return new PDFDocument(*this);
    }
    string type() const override { return "PDFDocument"; }
};

void demonstrateCovariant() {
    std::println("");
    std::println("╔══════════════════════════════════════════╗");
    std::println("║  EXAMPLE 6: COVARIANT RETURN TYPES       ║");
    std::println("╚══════════════════════════════════════════╝");
    std::println("");

    PDFDocument original;
    Document* basePtr = &original;

    // Via base pointer — returns Document*
    Document* cloned = basePtr->clone();
    std::println("  Cloned via base pointer: {}", cloned->type());
    delete cloned;

    // Via derived directly — returns PDFDocument* (no cast needed)
    PDFDocument* pdfClone = original.clone();
    std::println("  Cloned via derived:      {}", pdfClone->type());
    delete pdfClone;
}

// ============================================
// MAIN
// ============================================

int main() {
    std::println("============================================");
    std::println("  POLYMORPHISM IN C++ — FULL DEMONSTRATION");
    std::println("============================================");

    demonstrateRuntimePolymorphism();
    demonstrateVtable();
    demonstrateCompileTime();
    demonstratePaymentSystem();
    demonstrateVsTypechecking();
    demonstrateCovariant();

    return 0;
}
