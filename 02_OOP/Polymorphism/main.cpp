#include <iostream>
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
    void speak() const override { cout << "  " << name << ": Woof!\n"; }
    void move()  const override { cout << "  " << name << " runs on four legs\n"; }
};

class Cat : public Animal {
public:
    Cat(const string& n) : Animal(n) {}
    void speak() const override { cout << "  " << name << ": Meow!\n"; }
    void move()  const override { cout << "  " << name << " pads silently\n"; }
};

class Bird : public Animal {
public:
    Bird(const string& n) : Animal(n) {}
    void speak() const override { cout << "  " << name << ": Tweet!\n"; }
    void move()  const override { cout << "  " << name << " flies through the air\n"; }
};

// One function handles ALL animal types — this is the power
void makeAnimalAct(const Animal& a) {
    a.speak();
    a.move();
}

void demonstrateRuntimePolymorphism() {
    cout << "\n╔══════════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: RUNTIME POLYMORPHISM         ║\n";
    cout << "╚══════════════════════════════════════════╝\n\n";

    Dog  dog("Rex");
    Cat  cat("Whiskers");
    Bird bird("Tweety");

    cout << "--- Calling makeAnimalAct() on different types ---\n";
    cout << "  Same function call → different behavior:\n\n";
    makeAnimalAct(dog);
    cout << "\n";
    makeAnimalAct(cat);
    cout << "\n";
    makeAnimalAct(bird);

    cout << "\n--- Processing a mixed collection ---\n";
    vector<unique_ptr<Animal>> zoo;
    zoo.push_back(make_unique<Dog>("Buddy"));
    zoo.push_back(make_unique<Cat>("Luna"));
    zoo.push_back(make_unique<Bird>("Sky"));
    zoo.push_back(make_unique<Dog>("Max"));

    for (const auto& animal : zoo) {
        animal->speak();   // correct version for each
    }

    cout << "\n✅ Key point: We never asked 'what type is this?'\n";
    cout << "   The object itself knows what to do.\n";
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
    virtual void show() const { cout << "  Base::show()\n"; }
    virtual void display() const { cout << "  Base::display()\n"; }
    virtual ~Base() = default;
};

class Derived : public Base {
public:
    void show() const override { cout << "  Derived::show()\n"; }
    // display() NOT overridden — inherits Base version
};

void demonstrateVtable() {
    cout << "\n╔══════════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: VTABLE DISPATCH              ║\n";
    cout << "╚══════════════════════════════════════════╝\n\n";

    Derived d;
    Base* ptr = &d;   // Base pointer → Derived object

    cout << "--- ptr points to a Derived object ---\n";
    ptr->show();      // Derived::show()    — overridden
    ptr->display();   // Base::display()    — not overridden, uses Base

    cout << "\n--- Demonstrating slice vs pointer ---\n";
    Base copy = d;    // Object slicing — Derived part is cut off
    cout << "  Via sliced copy (no polymorphism):\n";
    copy.show();      // Base::show() — vptr was NOT copied

    cout << "  Via pointer (polymorphism):\n";
    ptr->show();      // Derived::show() — vptr intact

    cout << "\n✅ Always use pointer or reference for polymorphism.\n";
    cout << "   Object slicing kills it.\n";
}

// ============================================
// EXAMPLE 3: COMPILE-TIME POLYMORPHISM
// ============================================

/*
 * Resolved entirely at compile time — zero runtime cost.
 * The compiler picks the right version based on types.
 */

// --- Function overloading ---
void print(int x)         { cout << "  int: "    << x       << "\n"; }
void print(double x)      { cout << "  double: " << x       << "\n"; }
void print(const string& x) { cout << "  string: " << x     << "\n"; }

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
        cout << "  (" << x << ", " << y << ")\n";
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
        cout << "  " << item;
    }
    cout << "\n";
}

void demonstrateCompileTime() {
    cout << "\n╔══════════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: COMPILE-TIME POLYMORPHISM    ║\n";
    cout << "╚══════════════════════════════════════════╝\n\n";

    cout << "--- Function overloading ---\n";
    print(42);
    print(3.14);
    print(string("hello"));

    cout << "\n--- Operator overloading ---\n";
    Vector2D v1(1.0, 2.0), v2(3.0, 4.0);
    Vector2D v3 = v1 + v2;
    cout << "  v1 + v2 = ";
    v3.print();

    cout << "\n--- Template polymorphism ---\n";
    cout << "  maxOf(3, 7)       = " << maxOf(3, 7)          << "\n";
    cout << "  maxOf(1.5, 0.8)   = " << maxOf(1.5, 0.8)      << "\n";
    cout << "  maxOf('a', 'z')   = " << maxOf('a', 'z')       << "\n";

    cout << "\n  printContainer<int>:    "; printContainer<int>({1,2,3,4,5});
    cout << "  printContainer<string>: "; printContainer<string>({"foo","bar","baz"});

    cout << "\n✅ Compile-time polymorphism: zero runtime cost,\n";
    cout << "   compiler resolves everything before the program runs.\n";
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
        cout << "  Provider : " << getProviderName() << "\n";
        cout << "  Amount   : €" << amount << "\n";
        cout << "  Fee      : €" << fee    << " (" << getFeePercent() << "%)\n";
        cout << "  Total    : €" << total  << "\n";
        bool ok = processPayment(total);
        cout << "  Status   : " << (ok ? "✅ Approved" : "❌ Declined") << "\n";
    }
};

class CreditCard : public PaymentMethod {
    string last4;
public:
    CreditCard(const string& l) : last4(l) {}
    bool   processPayment(double amount) override {
        cout << "  Charging card **** " << last4 << "\n";
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
        cout << "  Sending PayPal request to " << email << "\n";
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
        cout << "  Broadcasting to wallet " << wallet.substr(0,8) << "...\n";
        return true;
    }
    string getProviderName() const override { return "Bitcoin"; }
    double getFeePercent()   const override { return 0.1; }
};

// Checkout knows NOTHING about specific payment types
void checkout(PaymentMethod& method, double amount) {
    cout << "\n  --- Processing payment ---\n";
    method.execute(amount);
}

void demonstratePaymentSystem() {
    cout << "\n╔══════════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: REAL-WORLD PAYMENT SYSTEM    ║\n";
    cout << "╚══════════════════════════════════════════╝\n\n";

    CreditCard card("4242");
    PayPal     paypal("kostas@email.com");
    Crypto     crypto("1A2B3C4D5E6F7G8H");

    checkout(card,   99.99);
    checkout(paypal, 49.50);
    checkout(crypto, 200.00);

    cout << "\n--- Same checkout() function, three different behaviors ---\n";
    cout << "✅ Adding a new payment type (e.g. SEPA) requires:\n";
    cout << "   • One new class inheriting PaymentMethod\n";
    cout << "   • Zero changes to checkout() or any existing code\n";
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
    cout << "\n╔══════════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: POLYMORPHISM vs TYPE-CHECK   ║\n";
    cout << "╚══════════════════════════════════════════╝\n\n";

    cout << "--- Anti-pattern: switch/if-else on type ---\n";
    vector<ShapeBad> badShapes = {
        {ShapeType::CIRCLE, 5.0},
        {ShapeType::SQUARE, 4.0},
        {ShapeType::TRIANGLE, 3.0}
    };
    for (const auto& s : badShapes) {
        cout << "  area = " << areaBad(s) << "\n";
    }
    cout << "  ❌ Adding a new shape requires editing areaBad()\n";

    cout << "\n--- Correct: polymorphic dispatch ---\n";
    vector<unique_ptr<ShapeGood>> shapes;
    shapes.push_back(make_unique<Circle>(5.0));
    shapes.push_back(make_unique<Square>(4.0));
    shapes.push_back(make_unique<Triangle>(3.0, 6.0));

    for (const auto& s : shapes) {
        cout << "  " << s->typeName() << " area = " << s->area() << "\n";
    }
    cout << "  ✅ Adding a new shape: just add a new class, nothing else changes\n";
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
    cout << "\n╔══════════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: COVARIANT RETURN TYPES       ║\n";
    cout << "╚══════════════════════════════════════════╝\n\n";

    PDFDocument original;
    Document* basePtr = &original;

    // Via base pointer — returns Document*
    Document* cloned = basePtr->clone();
    cout << "  Cloned via base pointer: " << cloned->type() << "\n";
    delete cloned;

    // Via derived directly — returns PDFDocument* (no cast needed)
    PDFDocument* pdfClone = original.clone();
    cout << "  Cloned via derived:      " << pdfClone->type() << "\n";
    delete pdfClone;

    cout << "\n✅ Covariant return: override can return derived type.\n";
    cout << "   Avoids casts when working with concrete types directly.\n";
}

// ============================================
// MAIN
// ============================================

int main() {
    cout << "============================================\n";
    cout << "  POLYMORPHISM IN C++ — FULL DEMONSTRATION\n";
    cout << "============================================\n";

    demonstrateRuntimePolymorphism();
    demonstrateVtable();
    demonstrateCompileTime();
    demonstratePaymentSystem();
    demonstrateVsTypechecking();
    demonstrateCovariant();

    cout << "\n============================================\n";
    cout << "  SUMMARY\n";
    cout << "============================================\n";
    cout << "\nTWO KINDS:\n";
    cout << "  Compile-time  → overloading, templates    (zero cost)\n";
    cout << "  Runtime       → virtual functions, vptr   (one indirection)\n";

    cout << "\nTHE RULE:\n";
    cout << "  Always use pointer or reference for runtime polymorphism.\n";
    cout << "  Object slicing destroys it.\n";

    cout << "\nWHEN TO USE:\n";
    cout << "  ✅ Mixed collections of related types\n";
    cout << "  ✅ Swappable implementations (plugins, drivers)\n";
    cout << "  ✅ Extending a system without modifying it\n";
    cout << "  ❌ Simple structs with no behavior\n";
    cout << "  ❌ Performance-critical inner loops (prefer templates)\n";

    cout << "\nRELATION TO OTHER TOPICS:\n";
    cout << "  AbstractClass  → defines the interface  (what)\n";
    cout << "  Overriding     → provides the behavior  (how)\n";
    cout << "  Polymorphism   → dispatches at runtime  (which)\n";

    return 0;
}
