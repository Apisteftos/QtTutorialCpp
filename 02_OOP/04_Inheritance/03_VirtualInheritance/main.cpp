#include <iostream>
#include <string>
#include <vector>
#include <memory>
using namespace std;

// ============================================
// WHAT IS VIRTUAL INHERITANCE?
// ============================================

/*
 * VIRTUAL INHERITANCE:
 * - Special form of inheritance to solve diamond problem
 * - Only ONE shared base class instance
 * - Uses 'virtual' keyword in inheritance
 * - Prevents duplicate base class subobjects
 *
 * THE DIAMOND PROBLEM:
 * - Class D inherits from B and C
 * - Both B and C inherit from A
 * - Without virtual: D has TWO copies of A
 * - With virtual: D has ONE copy of A
 *
 * SYNTAX:
 * - class Derived : virtual public Base { };
 * - 'virtual' goes before 'public/protected/private'
 * - Both intermediate classes must use virtual
 *
 * WHY VIRTUAL INHERITANCE?
 * - Solve ambiguity in diamond hierarchy
 * - Share single base class instance
 * - Enable proper multiple inheritance
 * - Avoid duplicate data members
 *
 * INITIALIZATION:
 * - Most derived class initializes virtual base
 * - Intermediate classes' initialization ignored
 * - Special initialization rules
 * - Order: virtual bases first, then others
 *
 * PERFORMANCE:
 * - Extra indirection (virtual table pointer)
 * - Slight memory overhead
 * - Slower than regular inheritance
 * - Use only when needed!
 *
 * COMMON USES:
 * - Multiple inheritance hierarchies
 * - Interface multiple inheritance
 * - Mixin classes
 * - When diamond pattern unavoidable
 */

// ============================================
// EXAMPLE 1: THE DIAMOND PROBLEM (Without Virtual)
// ============================================

class Animal {
protected:
    string name;

public:
    Animal(const string& n) : name(n) {
        cout << "  Animal constructor: " << name << "\n";
    }

    void eat() {
        cout << "  " << name << " is eating\n";
    }

    void breathe() {
        cout << "  " << name << " is breathing\n";
    }
};

class Mammal : public Animal {
public:
    Mammal(const string& n) : Animal(n) {
        cout << "  Mammal constructor\n";
    }

    void nurse() {
        cout << "  " << name << " is nursing babies\n";
    }
};

class Bird : public Animal {
public:
    Bird(const string& n) : Animal(n) {
        cout << "  Bird constructor\n";
    }

    void fly() {
        cout << "  " << name << " is flying\n";
    }
};

// Bat inherits from both Mammal and Bird
class Bat : public Mammal, public Bird {
public:
    Bat(const string& n)
        : Mammal(n), Bird(n) {
        cout << "  Bat constructor\n";
    }
};

void demonstrateDiamondProblem() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: DIAMOND PROBLEM        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating Bat (Diamond Hierarchy) ---\n";
    Bat bat("BruceWayne");

    cout << "\n⚠️  THE PROBLEM:\n";
    cout << "   • Bat has TWO Animal subobjects!\n";
    cout << "   • One through Mammal, one through Bird\n";
    cout << "   • Two 'name' members!\n";
    cout << "   • Ambiguous which to use!\n";

    cout << "\n--- Attempting to use methods ---\n";
    // bat.eat();  // ❌ ERROR: Ambiguous!
    // bat.breathe();  // ❌ ERROR: Ambiguous!
    cout << "  bat.eat() is ambiguous!\n";
    cout << "  bat.breathe() is ambiguous!\n";

    cout << "\n--- Must specify which Animal ---\n";
    bat.Mammal::eat();
    bat.Bird::breathe();

    cout << "\n💥 DIAMOND PROBLEM:\n";
    cout << "\n";
    cout << "       Animal\n";
    cout << "       /    \\\n";
    cout << "   Mammal  Bird\n";
    cout << "       \\    /\n";
    cout << "        Bat\n";
    cout << "\n";
    cout << "   • Bat has TWO Animal instances\n";
    cout << "   • Wasteful memory\n";
    cout << "   • Ambiguous access\n";
    cout << "   • Need to specify path\n";
}

// ============================================
// EXAMPLE 2: SOLUTION WITH VIRTUAL INHERITANCE
// ============================================

class AnimalVirtual {
protected:
    string name;

public:
    AnimalVirtual(const string& n) : name(n) {
        cout << "  AnimalVirtual constructor: " << name << "\n";
    }

    void eat() {
        cout << "  " << name << " is eating\n";
    }

    void breathe() {
        cout << "  " << name << " is breathing\n";
    }
};

// Virtual inheritance!
class MammalVirtual : virtual public AnimalVirtual {
public:
    MammalVirtual(const string& n) : AnimalVirtual(n) {
        cout << "  MammalVirtual constructor\n";
    }

    void nurse() {
        cout << "  " << name << " is nursing babies\n";
    }
};

// Virtual inheritance!
class BirdVirtual : virtual public AnimalVirtual {
public:
    BirdVirtual(const string& n) : AnimalVirtual(n) {
        cout << "  BirdVirtual constructor\n";
    }

    void fly() {
        cout << "  " << name << " is flying\n";
    }
};

class BatVirtual : public MammalVirtual, public BirdVirtual {
public:
    // Most derived class initializes virtual base!
    BatVirtual(const string& n)
        : AnimalVirtual(n),  // Virtual base initialization
        MammalVirtual(n), BirdVirtual(n) {
        cout << "  BatVirtual constructor\n";
    }
};

void demonstrateSolution() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: VIRTUAL INHERITANCE    ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating BatVirtual (Virtual Inheritance) ---\n";
    BatVirtual bat("BruceWayne");

    cout << "\n--- Using methods (no ambiguity!) ---\n";
    bat.eat();      // ✅ Works! No ambiguity
    bat.breathe();  // ✅ Works! No ambiguity
    bat.nurse();
    bat.fly();

    cout << "\n✅ SOLUTION:\n";
    cout << "   • BatVirtual has ONE Animal instance\n";
    cout << "   • Shared between Mammal and Bird\n";
    cout << "   • No ambiguity\n";
    cout << "   • Memory efficient\n";

    cout << "\n💡 VIRTUAL INHERITANCE:\n";
    cout << "\n";
    cout << "       Animal (one instance)\n";
    cout << "       /    \\\n";
    cout << "   Mammal  Bird (both virtual)\n";
    cout << "       \\    /\n";
    cout << "        Bat\n";
    cout << "\n";
    cout << "   • Only ONE Animal instance\n";
    cout << "   • Shared by all paths\n";
    cout << "   • No ambiguity!\n";
}

// ============================================
// EXAMPLE 3: INITIALIZATION ORDER
// ============================================

class Base {
public:
    Base() {
        cout << "  1. Base constructor\n";
    }
};

class Left : virtual public Base {
public:
    Left() {
        cout << "  2. Left constructor\n";
    }
};

class Right : virtual public Base {
public:
    Right() {
        cout << "  3. Right constructor\n";
    }
};

class Derived : public Left, public Right {
public:
    Derived() {
        cout << "  4. Derived constructor\n";
    }
};

void demonstrateInitOrder() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: INITIALIZATION ORDER   ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating Derived object ---\n";
    Derived d;

    cout << "\n💡 INITIALIZATION ORDER:\n";
    cout << "   1. Virtual base classes (Base)\n";
    cout << "   2. Left base\n";
    cout << "   3. Right base\n";
    cout << "   4. Derived class\n";
    cout << "\n";
    cout << "KEY RULE:\n";
    cout << "   Virtual bases initialized FIRST\n";
    cout << "   by most derived class!\n";
}

// ============================================
// EXAMPLE 4: MOST DERIVED CLASS INITIALIZES
// ============================================

class Device {
protected:
    int deviceId;

public:
    Device(int id) : deviceId(id) {
        cout << "  Device(" << id << ")\n";
    }

    void showId() {
        cout << "  Device ID: " << deviceId << "\n";
    }
};

class Input : virtual public Device {
public:
    Input(int id) : Device(id) {
        cout << "  Input constructor\n";
    }
};

class Output : virtual public Device {
public:
    Output(int id) : Device(id) {
        cout << "  Output constructor\n";
    }
};

class IODevice : public Input, public Output {
public:
    // MUST initialize Device directly!
    IODevice(int id)
        : Device(id),      // Most derived initializes virtual base
        Input(0),        // This is ignored!
        Output(0) {      // This is ignored!
        cout << "  IODevice constructor\n";
    }
};

void demonstrateMostDerivedInit() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: MOST DERIVED INIT      ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating IODevice ---\n";
    IODevice io(42);

    cout << "\n--- Checking device ID ---\n";
    io.showId();

    cout << "\n⚠️  CRITICAL RULE:\n";
    cout << "   • Most derived class initializes virtual base\n";
    cout << "   • IODevice initializes Device directly\n";
    cout << "   • Input(0) and Output(0) ignored!\n";
    cout << "   • This prevents multiple initialization\n";
}

// ============================================
// EXAMPLE 5: MEMORY LAYOUT
// ============================================

class BaseA {
public:
    int a;
    BaseA() : a(1) {
        cout << "  BaseA constructor\n";
    }
};

// Regular inheritance
// class DerivedRegular : public BaseA, public BaseA {
//     // ❌ Error: BaseA specified more than once
// };

// With virtual inheritance
class LeftVirtual : virtual public BaseA {
public:
    int left;
    LeftVirtual() : left(2) {
        cout << "  LeftVirtual constructor\n";
    }
};

class RightVirtual : virtual public BaseA {
public:
    int right;
    RightVirtual() : right(3) {
        cout << "  RightVirtual constructor\n";
    }
};

class BothVirtual : public LeftVirtual, public RightVirtual {
public:
    int both;
    BothVirtual() : both(4) {
        cout << "  BothVirtual constructor\n";
    }

    void showMembers() {
        cout << "  a (from BaseA): " << a << "\n";
        cout << "  left: " << left << "\n";
        cout << "  right: " << right << "\n";
        cout << "  both: " << both << "\n";
    }
};

void demonstrateMemoryLayout() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: MEMORY LAYOUT          ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating BothVirtual ---\n";
    BothVirtual obj;

    cout << "\n--- Memory members ---\n";
    obj.showMembers();

    cout << "\n💡 MEMORY LAYOUT:\n";
    cout << "   • ONE BaseA instance (shared)\n";
    cout << "   • LeftVirtual adds 'left'\n";
    cout << "   • RightVirtual adds 'right'\n";
    cout << "   • BothVirtual adds 'both'\n";
    cout << "   • Virtual base at end usually\n";
    cout << "   • Extra pointer for virtual base\n";

    cout << "\nSize comparison:\n";
    cout << "  sizeof(BaseA): " << sizeof(BaseA) << " bytes\n";
    cout << "  sizeof(LeftVirtual): " << sizeof(LeftVirtual) << " bytes\n";
    cout << "  sizeof(BothVirtual): " << sizeof(BothVirtual) << " bytes\n";
}

// ============================================
// EXAMPLE 6: INTERFACE PATTERN
// ============================================

// Pure virtual interface
class IReadable {
public:
    virtual ~IReadable() = default;
    virtual string read() const = 0;
};

class IWritable {
public:
    virtual ~IWritable() = default;
    virtual void write(const string& data) = 0;
};

// Implementation base with virtual inheritance
class Storage : virtual public IReadable, virtual public IWritable {
protected:
    string data;

public:
    Storage() : data("") {}
};

class FileStorage : public Storage {
public:
    string read() const override {
        return "File: " + data;
    }

    void write(const string& d) override {
        data = "file_" + d;
    }
};

class NetworkStorage : public Storage {
public:
    string read() const override {
        return "Network: " + data;
    }

    void write(const string& d) override {
        data = "net_" + d;
    }
};

void demonstrateInterfacePattern() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: INTERFACE PATTERN      ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Using FileStorage ---\n";
    FileStorage file;
    file.write("data.txt");
    cout << "Read: " << file.read() << "\n";

    cout << "\n--- Using NetworkStorage ---\n";
    NetworkStorage net;
    net.write("192.168.1.1");
    cout << "Read: " << net.read() << "\n";

    cout << "\n💡 INTERFACE PATTERN:\n";
    cout << "   • Multiple interfaces (IReadable, IWritable)\n";
    cout << "   • Common implementation base (Storage)\n";
    cout << "   • Virtual inheritance for interfaces\n";
    cout << "   • Clean separation of concerns\n";
}

// ============================================
// EXAMPLE 7: CASTING WITH VIRTUAL INHERITANCE
// ============================================

class Grandparent {
public:
    virtual ~Grandparent() = default;
    void grandparentFunc() {
        cout << "  Grandparent function\n";
    }
};

class ParentA : virtual public Grandparent {
public:
    void parentAFunc() {
        cout << "  ParentA function\n";
    }
};

class ParentB : virtual public Grandparent {
public:
    void parentBFunc() {
        cout << "  ParentB function\n";
    }
};

class Child : public ParentA, public ParentB {
public:
    void childFunc() {
        cout << "  Child function\n";
    }
};

void demonstrateCasting() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 7: CASTING                ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Child child;

    cout << "--- Upcasting to bases ---\n";
    Grandparent* gp = &child;  // ✅ OK
    ParentA* pa = &child;      // ✅ OK
    ParentB* pb = &child;      // ✅ OK

    cout << "All upcasts successful\n";

    cout << "\n--- Using through base pointer ---\n";
    gp->grandparentFunc();

    cout << "\n--- Dynamic cast ---\n";
    Grandparent* base = &child;
    Child* derived = dynamic_cast<Child*>(base);
    if (derived) {
        cout << "  Dynamic cast successful\n";
        derived->childFunc();
    }

    cout << "\n💡 CASTING WORKS:\n";
    cout << "   • Single path to Grandparent\n";
    cout << "   • No ambiguity\n";
    cout << "   • dynamic_cast works correctly\n";
}

// ============================================
// EXAMPLE 8: REAL-WORLD EXAMPLE - IOSTREAM
// ============================================

// Simplified iostream hierarchy
class IOSBase {
protected:
    bool goodBit;

public:
    IOSBase() : goodBit(true) {
        cout << "  IOSBase constructor\n";
    }

    bool good() const { return goodBit; }
    void setGood(bool g) { goodBit = g; }
};

class IStream : virtual public IOSBase {
public:
    IStream() {
        cout << "  IStream constructor\n";
    }

    void read() {
        cout << "  Reading data\n";
    }
};

class OStream : virtual public IOSBase {
public:
    OStream() {
        cout << "  OStream constructor\n";
    }

    void write() {
        cout << "  Writing data\n";
    }
};

class IOStream : public IStream, public OStream {
public:
    IOStream() {
        cout << "  IOStream constructor\n";
    }
};

void demonstrateIOStream() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 8: IOSTREAM PATTERN       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating IOStream ---\n";
    IOStream io;

    cout << "\n--- Using iostream ---\n";
    io.read();
    io.write();
    cout << "  Stream good: " << (io.good() ? "Yes" : "No") << "\n";

    io.setGood(false);
    cout << "  After error: " << (io.good() ? "Yes" : "No") << "\n";

    cout << "\n💡 REAL-WORLD USAGE:\n";
    cout << "   • std::iostream uses this pattern\n";
    cout << "   • One shared IOSBase instance\n";
    cout << "   • Both input and output share state\n";
    cout << "   • Classic virtual inheritance example\n";

    cout << "\n  std::ios_base (virtual base)\n";
    cout << "       /        \\\n";
    cout << "  std::istream  std::ostream\n";
    cout << "       \\        /\n";
    cout << "     std::iostream\n";
}

// ============================================
// EXAMPLE 9: WHEN NOT TO USE
// ============================================

void demonstrateWhenNotToUse() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 9: WHEN NOT TO USE        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "❌ DON'T USE VIRTUAL INHERITANCE WHEN:\n";
    cout << "   • No diamond hierarchy exists\n";
    cout << "   • Single inheritance is sufficient\n";
    cout << "   • Performance is critical\n";
    cout << "   • Simpler design available\n";
    cout << "   • Composition would work better\n";
    cout << "\n";
    cout << "✅ USE VIRTUAL INHERITANCE WHEN:\n";
    cout << "   • Diamond problem exists\n";
    cout << "   • Multiple paths to same base\n";
    cout << "   • Sharing base instance needed\n";
    cout << "   • Multiple interfaces with common base\n";
    cout << "   • Following iostream pattern\n";
    cout << "\n";
    cout << "⚠️  COSTS OF VIRTUAL INHERITANCE:\n";
    cout << "   • Extra pointer per object\n";
    cout << "   • Indirection overhead\n";
    cout << "   • Initialization complexity\n";
    cout << "   • Slightly larger objects\n";
    cout << "   • Can't inline some operations\n";
    cout << "\n";
    cout << "💡 GENERAL RULE:\n";
    cout << "   Use regular inheritance by default.\n";
    cout << "   Use virtual inheritance ONLY when\n";
    cout << "   diamond problem cannot be avoided!\n";
}

// ============================================
// EXAMPLE 10: COMPARISON
// ============================================

// Without virtual inheritance
namespace NonVirtual {
class Base {
public:
    int value;
    Base() : value(0) { }
};

class Left : public Base { };
class Right : public Base { };

class Derived : public Left, public Right {
public:
    void accessValue() {
        // Left::value = 1;  // Must specify which
        // Right::value = 2;
    }
};
}

// With virtual inheritance
namespace Virtual {
class Base {
public:
    int value;
    Base() : value(0) { }
};

class Left : virtual public Base { };
class Right : virtual public Base { };

class Derived : public Left, public Right {
public:
    void accessValue() {
        value = 1;  // ✅ No ambiguity!
    }
};
}

void demonstrateComparison() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 10: COMPARISON            ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "WITHOUT VIRTUAL INHERITANCE:\n";
    cout << "  • Two Base instances\n";
    cout << "  • Ambiguous access\n";
    cout << "  • Must use scope resolution\n";
    cout << "  • More memory\n";
    cout << "  • Faster (no indirection)\n";
    cout << "\n";

    cout << "WITH VIRTUAL INHERITANCE:\n";
    cout << "  • One Base instance\n";
    cout << "  • Direct access\n";
    cout << "  • No ambiguity\n";
    cout << "  • Less duplication\n";
    cout << "  • Slight overhead\n";

    cout << "\nSize comparison:\n";
    cout << "  NonVirtual::Derived: "
         << sizeof(NonVirtual::Derived) << " bytes\n";
    cout << "  Virtual::Derived: "
         << sizeof(Virtual::Derived) << " bytes\n";

    cout << "\n💡 TRADE-OFFS:\n";
    cout << "   Regular: Faster, simpler, but duplicates\n";
    cout << "   Virtual: Cleaner, but adds overhead\n";
    cout << "   Choose based on your needs!\n";
}

// ============================================
// MAIN FUNCTION - RUN ALL EXAMPLES
// ============================================

int main() {
    cout << "\n";
    cout << "╔═══════════════════════════════════════════════════════════╗\n";
    cout << "║                                                           ║\n";
    cout << "║            VIRTUAL INHERITANCE IN C++                    ║\n";
    cout << "║            Solving the Diamond Problem                    ║\n";
    cout << "║                                                           ║\n";
    cout << "╚═══════════════════════════════════════════════════════════╝\n";

    demonstrateDiamondProblem();
    demonstrateSolution();
    demonstrateInitOrder();
    demonstrateMostDerivedInit();
    demonstrateMemoryLayout();
    demonstrateInterfacePattern();
    demonstrateCasting();
    demonstrateIOStream();
    demonstrateWhenNotToUse();
    demonstrateComparison();

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
    cout << "WHAT IS VIRTUAL INHERITANCE?\n";
    cout << "========================================\n";
    cout << "Virtual inheritance:\n";
    cout << "  • Special inheritance form\n";
    cout << "  • Solves diamond problem\n";
    cout << "  • Single shared base instance\n";
    cout << "  • Uses 'virtual' keyword\n";
    cout << "\n";
    cout << "SYNTAX:\n";
    cout << "class Base { };\n";
    cout << "\n";
    cout << "class Left : virtual public Base { };\n";
    cout << "class Right : virtual public Base { };\n";
    cout << "\n";
    cout << "class Derived : public Left, public Right {\n";
    cout << "    // Only ONE Base instance!\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "THE DIAMOND PROBLEM\n";
    cout << "========================================\n";
    cout << "WITHOUT VIRTUAL INHERITANCE:\n";
    cout << "\n";
    cout << "       Base\n";
    cout << "       /  \\\n";
    cout << "    Left  Right\n";
    cout << "       \\  /\n";
    cout << "      Derived\n";
    cout << "\n";
    cout << "Problem:\n";
    cout << "  • Derived has TWO Base instances\n";
    cout << "  • One from Left path\n";
    cout << "  • One from Right path\n";
    cout << "  • Ambiguous which to use\n";
    cout << "  • Duplicate data members\n";
    cout << "  • Wasted memory\n";
    cout << "\n";
    cout << "class Derived : public Left, public Right {\n";
    cout << "    void func() {\n";
    cout << "        // value = 1;  // ❌ Ambiguous!\n";
    cout << "        Left::value = 1;   // Must specify\n";
    cout << "        Right::value = 2;  // Different values!\n";
    cout << "    }\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "SOLUTION WITH VIRTUAL INHERITANCE\n";
    cout << "========================================\n";
    cout << "WITH VIRTUAL INHERITANCE:\n";
    cout << "\n";
    cout << "       Base (shared)\n";
    cout << "       /  \\\n";
    cout << "    Left  Right (both virtual)\n";
    cout << "       \\  /\n";
    cout << "      Derived\n";
    cout << "\n";
    cout << "Solution:\n";
    cout << "  • Derived has ONE Base instance\n";
    cout << "  • Shared by both paths\n";
    cout << "  • No ambiguity\n";
    cout << "  • No duplication\n";
    cout << "  • Memory efficient\n";
    cout << "\n";
    cout << "class Left : virtual public Base { };\n";
    cout << "class Right : virtual public Base { };\n";
    cout << "\n";
    cout << "class Derived : public Left, public Right {\n";
    cout << "    void func() {\n";
    cout << "        value = 1;  // ✅ No ambiguity!\n";
    cout << "    }\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "INITIALIZATION RULES\n";
    cout << "========================================\n";
    cout << "CRITICAL: Most derived class initializes virtual base!\n";
    cout << "\n";
    cout << "class Base {\n";
    cout << "public:\n";
    cout << "    Base(int x) { }\n";
    cout << "};\n";
    cout << "\n";
    cout << "class Left : virtual public Base {\n";
    cout << "public:\n";
    cout << "    Left(int x) : Base(x) { }  // Ignored!\n";
    cout << "};\n";
    cout << "\n";
    cout << "class Right : virtual public Base {\n";
    cout << "public:\n";
    cout << "    Right(int x) : Base(x) { }  // Ignored!\n";
    cout << "};\n";
    cout << "\n";
    cout << "class Derived : public Left, public Right {\n";
    cout << "public:\n";
    cout << "    Derived(int x) \n";
    cout << "        : Base(x),      // ✅ This is used!\n";
    cout << "          Left(0),      // Base(0) ignored\n";
    cout << "          Right(0) { }  // Base(0) ignored\n";
    cout << "};\n";
    cout << "\n";
    cout << "WHY?\n";
    cout << "  • Prevents multiple initialization\n";
    cout << "  • Most derived knows full context\n";
    cout << "  • Intermediate classes can't coordinate\n";

    cout << "\n========================================\n";
    cout << "INITIALIZATION ORDER\n";
    cout << "========================================\n";
    cout << "Order of construction:\n";
    cout << "  1. Virtual base classes (depth-first, left-to-right)\n";
    cout << "  2. Non-virtual base classes (left-to-right)\n";
    cout << "  3. Member variables (declaration order)\n";
    cout << "  4. Constructor body\n";
    cout << "\n";
    cout << "Example:\n";
    cout << "class Derived : public Left, public Right {\n";
    cout << "    int member;\n";
    cout << "};\n";
    cout << "\n";
    cout << "Construction order:\n";
    cout << "  1. Base (virtual base)\n";
    cout << "  2. Left\n";
    cout << "  3. Right\n";
    cout << "  4. member\n";
    cout << "  5. Derived constructor body\n";
    cout << "\n";
    cout << "Destruction: Reverse order\n";

    cout << "\n========================================\n";
    cout << "MEMORY LAYOUT\n";
    cout << "========================================\n";
    cout << "WITHOUT VIRTUAL:\n";
    cout << "  Derived object:\n";
    cout << "  +------------------+\n";
    cout << "  | Left (with Base) |\n";
    cout << "  +------------------+\n";
    cout << "  | Right (with Base)|\n";
    cout << "  +------------------+\n";
    cout << "  | Derived members  |\n";
    cout << "  +------------------+\n";
    cout << "  Two Base instances!\n";
    cout << "\n";
    cout << "WITH VIRTUAL:\n";
    cout << "  Derived object:\n";
    cout << "  +------------------+\n";
    cout << "  | Left (+ vptr)    |\n";
    cout << "  +------------------+\n";
    cout << "  | Right (+ vptr)   |\n";
    cout << "  +------------------+\n";
    cout << "  | Derived members  |\n";
    cout << "  +------------------+\n";
    cout << "  | Base (shared)    |\n";
    cout << "  +------------------+\n";
    cout << "  One Base instance!\n";
    cout << "  Extra pointers for virtual base\n";

    cout << "\n========================================\n";
    cout << "WHEN TO USE\n";
    cout << "========================================\n";
    cout << "✅ USE VIRTUAL INHERITANCE:\n";
    cout << "  • Diamond hierarchy unavoidable\n";
    cout << "  • Multiple paths to same base\n";
    cout << "  • Need shared base instance\n";
    cout << "  • Multiple interfaces pattern\n";
    cout << "  • iostream-like hierarchies\n";
    cout << "\n";
    cout << "❌ DON'T USE IF:\n";
    cout << "  • No diamond problem\n";
    cout << "  • Single inheritance sufficient\n";
    cout << "  • Performance critical\n";
    cout << "  • Composition possible instead\n";
    cout << "  • Simpler design available\n";
    cout << "\n";
    cout << "ALTERNATIVES:\n";
    cout << "  • Composition over inheritance\n";
    cout << "  • Redesign to avoid diamond\n";
    cout << "  • Interface segregation\n";
    cout << "  • Multiple interfaces without common base\n";

    cout << "\n========================================\n";
    cout << "PERFORMANCE IMPACT\n";
    cout << "========================================\n";
    cout << "OVERHEAD:\n";
    cout << "  • Extra pointer per virtual base\n";
    cout << "  • Indirection through pointer\n";
    cout << "  • Slightly larger objects\n";
    cout << "  • Can't inline some operations\n";
    cout << "\n";
    cout << "TYPICAL OVERHEAD:\n";
    cout << "  • 4-8 bytes per virtual base (pointer)\n";
    cout << "  • One extra indirection per access\n";
    cout << "  • Usually negligible\n";
    cout << "  • Only matters in tight loops\n";
    cout << "\n";
    cout << "WHEN IT MATTERS:\n";
    cout << "  • Embedded systems\n";
    cout << "  • Performance-critical code\n";
    cout << "  • Millions of small objects\n";
    cout << "  • Cache-sensitive algorithms\n";

    cout << "\n========================================\n";
    cout << "BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "✅ DO:\n";
    cout << "  • Use only when diamond exists\n";
    cout << "  • Make intermediate classes virtual\n";
    cout << "  • Initialize in most derived class\n";
    cout << "  • Document virtual inheritance\n";
    cout << "  • Use for interface patterns\n";
    cout << "  • Keep virtual bases simple\n";
    cout << "  • Test thoroughly\n";
    cout << "  • Understand initialization order\n";
    cout << "\n";
    cout << "❌ DON'T:\n";
    cout << "  • Use unnecessarily\n";
    cout << "  • Forget to initialize virtual base\n";
    cout << "  • Mix virtual and non-virtual\n";
    cout << "  • Create deep hierarchies\n";
    cout << "  • Use for every inheritance\n";
    cout << "  • Ignore performance impact\n";
    cout << "  • Overcomplicate design\n";
    cout << "  • Use without understanding\n";

    cout << "\n========================================\n";
    cout << "REAL-WORLD EXAMPLES\n";
    cout << "========================================\n";
    cout << "1. STD::IOSTREAM:\n";
    cout << "   std::ios_base\n";
    cout << "     /         \\\n";
    cout << "  istream    ostream\n";
    cout << "     \\         /\n";
    cout << "     iostream\n";
    cout << "   • Classic virtual inheritance\n";
    cout << "   • Shares format state\n";
    cout << "\n";
    cout << "2. MULTIPLE INTERFACES:\n";
    cout << "   IReadable  IWritable\n";
    cout << "        \\      /\n";
    cout << "       Storage\n";
    cout << "   • Common implementation base\n";
    cout << "   • Multiple capabilities\n";
    cout << "\n";
    cout << "3. GUI HIERARCHIES:\n";
    cout << "   Widget\n";
    cout << "    / \\\n";
    cout << "  A    B\n";
    cout << "   \\  /\n";
    cout << "    AB\n";
    cout << "   • Combining UI capabilities\n";

    cout << "\n========================================\n";
    cout << "QUICK REFERENCE\n";
    cout << "========================================\n";
    cout << "// Virtual inheritance syntax\n";
    cout << "class Base { };\n";
    cout << "\n";
    cout << "class Left : virtual public Base { };\n";
    cout << "class Right : virtual public Base { };\n";
    cout << "\n";
    cout << "class Derived : public Left, public Right {\n";
    cout << "public:\n";
    cout << "    Derived() : Base() { }  // Initialize virtual base\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Usage\n";
    cout << "Derived d;\n";
    cout << "d.baseMethod();  // No ambiguity!\n";

    cout << "\n========================================\n";
    cout << "KEY TAKEAWAYS\n";
    cout << "========================================\n";
    cout << "1. Virtual inheritance solves diamond problem\n";
    cout << "2. Creates single shared base instance\n";
    cout << "3. Use 'virtual' keyword in inheritance\n";
    cout << "4. Most derived class initializes virtual base\n";
    cout << "5. Adds slight performance overhead\n";
    cout << "6. Essential for iostream pattern\n";
    cout << "7. Use only when diamond unavoidable\n";
    cout << "8. Prefer composition when possible\n";
    cout << "9. Initialization order: virtual bases first\n";
    cout << "10. Understand before using!\n";

    cout << "\n========================================\n";
    cout << "GOLDEN RULES\n";
    cout << "========================================\n";
    cout << "⚠️  RULE 1: Use only for diamond problem\n";
    cout << "   Don't use unnecessarily!\n";
    cout << "   \n";
    cout << "   class Left : virtual public Base { };  // Only if needed\n";
    cout << "\n";
    cout << "⚠️  RULE 2: Most derived initializes virtual base\n";
    cout << "   Critical initialization rule!\n";
    cout << "   \n";
    cout << "   Derived() : Base(x), Left(), Right() { }\n";
    cout << "\n";
    cout << "⚠️  RULE 3: Both intermediate classes must be virtual\n";
    cout << "   Consistency required!\n";
    cout << "   \n";
    cout << "   class Left : virtual public Base { };\n";
    cout << "   class Right : virtual public Base { };  // Both!\n";
    cout << "\n";
    cout << "⚠️  RULE 4: Prefer composition over inheritance\n";
    cout << "   Simpler alternative!\n";
    cout << "   \n";
    cout << "   Composition often better than virtual inheritance\n";
    cout << "\n";
    cout << "⚠️  RULE 5: Understand the cost\n";
    cout << "   Know the trade-offs!\n";
    cout << "   \n";
    cout << "   Extra indirection, larger objects\n";
    cout << "   Use when benefit outweighs cost\n";

    return 0;
}
