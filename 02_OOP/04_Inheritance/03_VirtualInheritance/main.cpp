#include <print>
#include <istream>
#include <string>
#include <type_traits>

// Compile-time checks used by the demos below
template<typename T>
constexpr bool canEat = requires(T t) { t.eat(); };

template<typename To, typename From>
constexpr bool canStaticDowncast = requires(From* p) { static_cast<To*>(p); };

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
    std::string name;

public:
    explicit Animal(const std::string& n) : name(n) {
        std::println("  Animal constructor: {}", name);
    }

    void eat() {
        std::println("  {} is eating", name);
    }

    void breathe() {
        std::println("  {} is breathing", name);
    }
};

class Mammal : public Animal {
public:
    explicit Mammal(const std::string& n) : Animal(n) {
        std::println("  Mammal constructor");
    }

    void nurse() {
        std::println("  {} is nursing babies", name);
    }
};

class Bird : public Animal {
public:
    explicit Bird(const std::string& n) : Animal(n) {
        std::println("  Bird constructor");
    }

    void fly() {
        std::println("  {} is flying", name);
    }
};

// Bat inherits from both Mammal and Bird
class Bat : public Mammal, public Bird {
public:
    explicit Bat(const std::string& n)
        : Mammal(n), Bird(n) {
        std::println("  Bat constructor");
    }
};

void demonstrateDiamondProblem() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: DIAMOND PROBLEM        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating Bat (Diamond Hierarchy) ---");
    Bat bat("BruceWayne");

    std::println("");
    std::println("--- Attempting to use methods ---");
    // bat.eat();  // ❌ ERROR: Ambiguous!
    // bat.breathe();  // ❌ ERROR: Ambiguous!

    std::println("");
    std::println("--- Must specify which Animal ---");
    bat.Mammal::eat();
    bat.Bird::breathe();
    std::println("");
    std::println("--- Two Animal subobjects ---");
    std::println("  Bat* converts to Animal*: {} (ambiguous)", std::is_convertible_v<Bat*, Animal*>);
    std::println("  bat.eat() compiles: {}", canEat<Bat>);
    std::println("  sizeof(Bat): {} bytes (GCC x86-64)", sizeof(Bat));
}

// ============================================
// EXAMPLE 2: SOLUTION WITH VIRTUAL INHERITANCE
// ============================================

class AnimalVirtual {
protected:
    std::string name;

public:
    explicit AnimalVirtual(const std::string& n) : name(n) {
        std::println("  AnimalVirtual constructor: {}", name);
    }

    void eat() {
        std::println("  {} is eating", name);
    }

    void breathe() {
        std::println("  {} is breathing", name);
    }
};

// Virtual inheritance!
class MammalVirtual : virtual public AnimalVirtual {
public:
    explicit MammalVirtual(const std::string& n) : AnimalVirtual(n) {
        std::println("  MammalVirtual constructor");
    }

    void nurse() {
        std::println("  {} is nursing babies", name);
    }
};

// Virtual inheritance!
class BirdVirtual : virtual public AnimalVirtual {
public:
    explicit BirdVirtual(const std::string& n) : AnimalVirtual(n) {
        std::println("  BirdVirtual constructor");
    }

    void fly() {
        std::println("  {} is flying", name);
    }
};

class BatVirtual : public MammalVirtual, public BirdVirtual {
public:
    // Most derived class initializes virtual base!
    explicit BatVirtual(const std::string& n)
        : AnimalVirtual(n),  // Virtual base initialization
        MammalVirtual(n), BirdVirtual(n) {
        std::println("  BatVirtual constructor");
    }
};

void demonstrateSolution() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: VIRTUAL INHERITANCE    ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating BatVirtual (Virtual Inheritance) ---");
    BatVirtual bat("BruceWayne");

    std::println("");
    std::println("--- Using methods (no ambiguity!) ---");
    bat.eat();      // ✅ Works! No ambiguity
    bat.breathe();  // ✅ Works! No ambiguity
    bat.nurse();
    bat.fly();
    std::println("");
    std::println("--- One AnimalVirtual subobject ---");
    std::println("  BatVirtual* converts to AnimalVirtual*: {}", std::is_convertible_v<BatVirtual*, AnimalVirtual*>);
    std::println("  sizeof(BatVirtual): {} bytes (GCC x86-64)", sizeof(BatVirtual));
}

// ============================================
// EXAMPLE 3: INITIALIZATION ORDER
// ============================================

class Base {
public:
    Base() {
        std::println("  1. Base constructor");
    }

    ~Base() {
        std::println("  4. Base destructor");
    }
};

class Left : virtual public Base {
public:
    Left() {
        std::println("  2. Left constructor");
    }

    ~Left() {
        std::println("  3. Left destructor");
    }
};

class Right : virtual public Base {
public:
    Right() {
        std::println("  3. Right constructor");
    }

    ~Right() {
        std::println("  2. Right destructor");
    }
};

class Derived : public Left, public Right {
public:
    Derived() {
        std::println("  4. Derived constructor");
    }

    ~Derived() {
        std::println("  1. Derived destructor");
    }
};

void demonstrateInitOrder() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: INITIALIZATION ORDER   ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating Derived object ---");
    Derived d;
    std::println("");
    std::println("--- Destruction (reverse order) ---");
}

// ============================================
// EXAMPLE 4: MOST DERIVED CLASS INITIALIZES
// ============================================

class Device {
protected:
    int deviceId;

public:
    explicit Device(int id) : deviceId(id) {
        std::println("  Device({})", id);
    }

    void showId() {
        std::println("  Device ID: {}", deviceId);
    }
};

class Input : virtual public Device {
public:
    explicit Input(int id) : Device(id) {
        std::println("  Input constructor");
    }
};

class Output : virtual public Device {
public:
    explicit Output(int id) : Device(id) {
        std::println("  Output constructor");
    }
};

class IODevice : public Input, public Output {
public:
    // MUST initialize Device directly!
    explicit IODevice(int id)
        : Device(id),      // Most derived initializes virtual base
        Input(0),        // This is ignored!
        Output(0) {      // This is ignored!
        std::println("  IODevice constructor");
    }
};

void demonstrateMostDerivedInit() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: MOST DERIVED INIT      ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating IODevice ---");
    IODevice io(42);

    std::println("");
    std::println("--- Checking device ID ---");
    io.showId();
}

// ============================================
// EXAMPLE 5: MEMORY LAYOUT
// ============================================

class BaseA {
public:
    int a;
    BaseA() : a(1) {
        std::println("  BaseA constructor");
    }
};

// Regular inheritance
// class DerivedRegular : public BaseA, public BaseA {
//     // ❌ Error: duplicate base type 'BaseA' invalid
// };

// With virtual inheritance
class LeftVirtual : virtual public BaseA {
public:
    int left;
    LeftVirtual() : left(2) {
        std::println("  LeftVirtual constructor");
    }
};

class RightVirtual : virtual public BaseA {
public:
    int right;
    RightVirtual() : right(3) {
        std::println("  RightVirtual constructor");
    }
};

class BothVirtual : public LeftVirtual, public RightVirtual {
public:
    int both;
    BothVirtual() : both(4) {
        std::println("  BothVirtual constructor");
    }

    void showMembers() {
        std::println("  a (from BaseA): {}", a);
        std::println("  left: {}", left);
        std::println("  right: {}", right);
        std::println("  both: {}", both);
    }
};

void demonstrateMemoryLayout() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 5: MEMORY LAYOUT          ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating BothVirtual ---");
    BothVirtual obj;

    std::println("");
    std::println("--- Memory members ---");
    obj.showMembers();

    std::println("");
    std::println("Size comparison (GCC x86-64):");
    std::println("  sizeof(BaseA): {} bytes", sizeof(BaseA));
    std::println("  sizeof(LeftVirtual): {} bytes", sizeof(LeftVirtual));
    std::println("  sizeof(BothVirtual): {} bytes", sizeof(BothVirtual));
}

// ============================================
// EXAMPLE 6: INTERFACE PATTERN
// ============================================

// Pure virtual interface
class IReadable {
public:
    virtual ~IReadable() = default;
    virtual std::string read() const = 0;
};

class IWritable {
public:
    virtual ~IWritable() = default;
    virtual void write(const std::string& data) = 0;
};

// Implementation base.
// NOTE: "virtual" is not needed here - there is only ONE path to each interface,
// so there is no diamond. It would matter if two classes derived from IReadable
// were combined into a third one.
class Storage : virtual public IReadable, virtual public IWritable {
protected:
    std::string data;

public:
    Storage() : data("") {}
};

class FileStorage : public Storage {
public:
    std::string read() const override {
        return "File: " + data;
    }

    void write(const std::string& d) override {
        data = "file_" + d;
    }
};

class NetworkStorage : public Storage {
public:
    std::string read() const override {
        return "Network: " + data;
    }

    void write(const std::string& d) override {
        data = "net_" + d;
    }
};

void demonstrateInterfacePattern() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 6: INTERFACE PATTERN      ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Using FileStorage ---");
    FileStorage file;
    file.write("data.txt");
    std::println("Read: {}", file.read());

    std::println("");
    std::println("--- Using NetworkStorage ---");
    NetworkStorage net;
    net.write("192.168.1.1");
    std::println("Read: {}", net.read());
}

// ============================================
// EXAMPLE 7: CASTING WITH VIRTUAL INHERITANCE
// ============================================

class Grandparent {
public:
    virtual ~Grandparent() = default;
    void grandparentFunc() {
        std::println("  Grandparent function");
    }
};

class ParentA : virtual public Grandparent {
public:
    void parentAFunc() {
        std::println("  ParentA function");
    }
};

class ParentB : virtual public Grandparent {
public:
    void parentBFunc() {
        std::println("  ParentB function");
    }
};

class Child : public ParentA, public ParentB {
public:
    void childFunc() {
        std::println("  Child function");
    }
};

void demonstrateCasting() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 7: CASTING                ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Child child;

    std::println("--- Upcasting to bases ---");
    Grandparent* gp = &child;  // ✅ OK
    ParentA* pa = &child;      // ✅ OK
    ParentB* pb = &child;      // ✅ OK

    std::println("All upcasts successful");

    std::println("");
    std::println("--- Using through base pointer ---");
    gp->grandparentFunc();

    std::println("");
    std::println("--- Dynamic cast ---");
    Grandparent* base = &child;
    Child* derived = dynamic_cast<Child*>(base);
    if (derived) {
        std::println("  Dynamic cast successful");
        derived->childFunc();
    }
    std::println("");
    std::println("--- Cross-cast with dynamic_cast ---");
    ParentB* sideways = dynamic_cast<ParentB*>(pa);
    std::println("  ParentA* -> ParentB* works: {}", sideways == pb);

    // Child* c = static_cast<Child*>(gp);  // ❌ Error: the base is virtual
    std::println("  static_cast down from Grandparent* possible: {}", canStaticDowncast<Child, Grandparent>);
}

// ============================================
// EXAMPLE 8: REAL-WORLD EXAMPLE - IOSTREAM
// ============================================

// Simplified iostream hierarchy.
// The real one: std::basic_istream and std::basic_ostream derive VIRTUALLY from
// std::basic_ios, which derives (non-virtually) from std::ios_base.
class IOSBase {
protected:
    bool goodBit;

public:
    IOSBase() : goodBit(true) {
        std::println("  IOSBase constructor");
    }

    bool good() const { return goodBit; }
    void setGood(bool g) { goodBit = g; }
};

class IStream : virtual public IOSBase {
public:
    IStream() {
        std::println("  IStream constructor");
    }

    void read() {
        std::println("  Reading data");
    }
};

class OStream : virtual public IOSBase {
public:
    OStream() {
        std::println("  OStream constructor");
    }

    void write() {
        std::println("  Writing data");
    }
};

class IOStream : public IStream, public OStream {
public:
    IOStream() {
        std::println("  IOStream constructor");
    }
};

void demonstrateIOStream() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 8: IOSTREAM PATTERN       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating IOStream ---");
    IOStream io;

    std::println("");
    std::println("--- Using iostream ---");
    io.read();
    io.write();
    std::println("  Stream good: {}", (io.good() ? "Yes" : "No"));

    io.setGood(false);
    std::println("  After error: {}", (io.good() ? "Yes" : "No"));
    std::println("");
    std::println("--- The real standard library ---");
    std::println("  std::istream derives from std::basic_ios<char>: {}", std::is_base_of_v<std::basic_ios<char>, std::istream>);
    std::println("  basic_ios* can be static_cast to iostream*: {} (virtual base)", canStaticDowncast<std::iostream, std::basic_ios<char>>);
}

// ============================================
// EXAMPLE 9: WHEN NOT TO USE
// ============================================

// A hierarchy WITHOUT a diamond
struct Plain { int x = 0; };
struct PlainDerived : Plain { };
struct PlainVirtual : virtual Plain { };  // 'virtual' buys nothing here

void demonstrateWhenNotToUse() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 9: WHEN NOT TO USE        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Cost of 'virtual' when there is no diamond ---");
    std::println("  sizeof(PlainDerived): {} bytes", sizeof(PlainDerived));
    std::println("  sizeof(PlainVirtual): {} bytes (GCC x86-64: hidden pointer added)", sizeof(PlainVirtual));

    std::println("");
    std::println("--- Can a Plain* be static_cast down to the derived class? ---");
    std::println("  to PlainDerived: {}", canStaticDowncast<PlainDerived, Plain>);
    std::println("  to PlainVirtual: {}", canStaticDowncast<PlainVirtual, Plain>);
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
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 10: COMPARISON            ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("WITHOUT VIRTUAL INHERITANCE:");
    NonVirtual::Derived nv;
    nv.Left::value = 1;
    nv.Right::value = 2;
    std::println("  Left::value = {}, Right::value = {}  (two separate Base objects)", nv.Left::value, nv.Right::value);
    std::println("  sizeof(NonVirtual::Derived): {} bytes (GCC x86-64)", sizeof(NonVirtual::Derived));
    std::println("");

    std::println("WITH VIRTUAL INHERITANCE:");
    Virtual::Derived v;
    v.accessValue();
    std::println("  value = {}, via Left: {}, via Right: {}  (one shared Base object)", v.value, v.Left::value, v.Right::value);
    std::println("  sizeof(Virtual::Derived): {} bytes (GCC x86-64)", sizeof(Virtual::Derived));
}

// ============================================
// MAIN FUNCTION - RUN ALL EXAMPLES
// ============================================

int main() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════════════╗");
    std::println("║                                                           ║");
    std::println("║            VIRTUAL INHERITANCE IN C++                    ║");
    std::println("║            Solving the Diamond Problem                    ║");
    std::println("║                                                           ║");
    std::println("╚═══════════════════════════════════════════════════════════╝");

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

    return 0;
}

