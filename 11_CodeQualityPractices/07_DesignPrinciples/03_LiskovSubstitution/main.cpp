#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
using namespace std;

// ============================================
// LISKOV SUBSTITUTION PRINCIPLE (LSP)
// ============================================

/*
LISKOV SUBSTITUTION PRINCIPLE:
    "Objects of a superclass should be replaceable with objects
     of a subclass without breaking the application."
    — Barbara Liskov

WHAT IT MEANS:
    • Subclass must be substitutable for base class
    • Child class must honor parent's contract
    • No surprising behavior changes
    • Client code shouldn't need to know about subclass

WHY IT MATTERS:
    ✅ Polymorphism works correctly
    ✅ Code is predictable and reliable
    ✅ Inheritance makes sense
    ✅ No runtime surprises
    ✅ Type safety

CONTRACT RULES:
    1. Preconditions cannot be strengthened
    2. Postconditions cannot be weakened
    3. Invariants must be preserved
    4. No new exceptions (unless subclass of expected)
    5. History constraint (immutable properties stay immutable)

VIOLATION SIGNS:
    🚨 instanceof/dynamic_cast checks
    🚨 Overridden method throws new exceptions
    🚨 Subclass changes expected behavior
    🚨 Empty or throwing overridden methods
    🚨 Square-Rectangle problem
*/

// ============================================
// EXAMPLE 1: RECTANGLE-SQUARE PROBLEM (Classic)
// ============================================

namespace bad_example1 {
// ❌ BAD: Violates LSP

class Rectangle {
protected:
    int width;
    int height;

public:
    Rectangle(int w, int h) : width(w), height(h) {}

    virtual void setWidth(int w) { width = w; }
    virtual void setHeight(int h) { height = h; }

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    int getArea() const { return width * height; }
};

class Square : public Rectangle {
public:
    Square(int side) : Rectangle(side, side) {}

    // 🚨 VIOLATION: Changes contract!
    void setWidth(int w) override {
        width = w;
        height = w;  // Side effect!
    }

    void setHeight(int h) override {
        width = h;   // Side effect!
        height = h;
    }
};

void processRectangle(Rectangle& rect) {
    // This should work for any Rectangle...
    rect.setWidth(5);
    rect.setHeight(4);

    // Expected: 5 * 4 = 20
    cout << "   Expected area: 20" << endl;
    cout << "   Actual area: " << rect.getArea() << endl;

    // Works for Rectangle, but breaks for Square!
    if (rect.getArea() != 20) {
        cout << "   🚨 VIOLATION: Area is not 20!" << endl;
    }
}
}

namespace good_example1 {
// ✅ GOOD: Follows LSP

class Shape {
public:
    virtual ~Shape() = default;
    virtual int getArea() const = 0;
};

class Rectangle : public Shape {
protected:
    int width;
    int height;

public:
    Rectangle(int w, int h) : width(w), height(h) {}

    void setWidth(int w) { width = w; }
    void setHeight(int h) { height = h; }

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    int getArea() const override { return width * height; }
};

class Square : public Shape {
private:
    int side;

public:
    Square(int s) : side(s) {}

    void setSide(int s) { side = s; }
    int getSide() const { return side; }

    int getArea() const override { return side * side; }
};

// Now both can be used as Shape without issues
}

void example1_rectangle_square() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 1: RECTANGLE-SQUARE PROBLEM           ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (Violates LSP):\n";
    cout << "```cpp\n";
    cout << "class Square : public Rectangle {\n";
    cout << "    void setWidth(int w) override {\n";
    cout << "        width = w;\n";
    cout << "        height = w;  // Changes both!\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "```\n";

    cout << "\n🚨 DEMONSTRATION OF VIOLATION:\n";

    bad_example1::Rectangle rect(3, 4);
    cout << "\n   Testing with Rectangle:\n";
    bad_example1::processRectangle(rect);

    bad_example1::Square square(5);
    cout << "\n   Testing with Square (should work the same):\n";
    bad_example1::processRectangle(square);

    cout << "\n🚨 PROBLEMS:\n";
    cout << "   • Square changes both dimensions when setting one\n";
    cout << "   • Client code breaks when using Square\n";
    cout << "   • Violates client expectations\n";
    cout << "   • Not truly substitutable\n";

    cout << "\n✅ GOOD CODE (Follows LSP):\n";
    cout << "```cpp\n";
    cout << "class Shape {\n";
    cout << "    virtual int getArea() const = 0;\n";
    cout << "};\n\n";
    cout << "class Rectangle : public Shape { };\n";
    cout << "class Square : public Shape { };\n";
    cout << "// Square is NOT a Rectangle!\n";
    cout << "```\n";

    cout << "\n✨ SOLUTION:\n";
    cout << "   • Square and Rectangle are siblings, not parent-child\n";
    cout << "   • Both inherit from Shape\n";
    cout << "   • No violated expectations\n";
}

// ============================================
// EXAMPLE 2: BIRD-PENGUIN PROBLEM
// ============================================

namespace bad_example2 {
// ❌ BAD: Violates LSP

class Bird {
public:
    virtual ~Bird() = default;

    virtual void fly() {
        cout << "   🦅 Flying in the sky!" << endl;
    }
};

class Sparrow : public Bird {
public:
    void fly() override {
        cout << "   🐦 Sparrow flying!" << endl;
    }
};

class Penguin : public Bird {
public:
    // 🚨 VIOLATION: Penguin can't fly!
    void fly() override {
        throw runtime_error("Penguins can't fly!");
    }
};

void makeBirdFly(Bird& bird) {
    bird.fly();  // Should work for all birds... right?
}
}

namespace good_example2 {
// ✅ GOOD: Follows LSP

class Bird {
public:
    virtual ~Bird() = default;
    virtual void eat() {
        cout << "   🍴 Bird is eating" << endl;
    }
};

class FlyingBird : public Bird {
public:
    virtual void fly() {
        cout << "   🦅 Flying!" << endl;
    }
};

class FlightlessBird : public Bird {
public:
    virtual void walk() {
        cout << "   🚶 Walking!" << endl;
    }
};

class Sparrow : public FlyingBird {
public:
    void fly() override {
        cout << "   🐦 Sparrow flying!" << endl;
    }
};

class Penguin : public FlightlessBird {
public:
    void walk() override {
        cout << "   🐧 Penguin waddling!" << endl;
    }

    void swim() {
        cout << "   🏊 Penguin swimming!" << endl;
    }
};
}

void example2_bird_penguin() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 2: BIRD-PENGUIN PROBLEM               ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (Violates LSP):\n";
    cout << "```cpp\n";
    cout << "class Penguin : public Bird {\n";
    cout << "    void fly() override {\n";
    cout << "        throw runtime_error(\"Can't fly!\");\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "```\n";

    cout << "\n🚨 DEMONSTRATION OF VIOLATION:\n";

    bad_example2::Sparrow sparrow;
    cout << "\n   Making Sparrow fly:\n";
    bad_example2::makeBirdFly(sparrow);

    bad_example2::Penguin penguin;
    cout << "\n   Making Penguin fly:\n";
    try {
        bad_example2::makeBirdFly(penguin);
    } catch (const exception& e) {
        cout << "   💥 Exception: " << e.what() << endl;
    }

    cout << "\n🚨 PROBLEMS:\n";
    cout << "   • Penguin throws exception when calling fly()\n";
    cout << "   • Client code must know about Penguin specifically\n";
    cout << "   • Can't treat all Birds the same way\n";
    cout << "   • Violates substitutability\n";

    cout << "\n✅ GOOD CODE (Follows LSP):\n";
    cout << "```cpp\n";
    cout << "class Bird { };\n";
    cout << "class FlyingBird : public Bird {\n";
    cout << "    virtual void fly();\n";
    cout << "};\n";
    cout << "class FlightlessBird : public Bird { };\n\n";
    cout << "class Sparrow : public FlyingBird { };\n";
    cout << "class Penguin : public FlightlessBird { };\n";
    cout << "```\n";

    cout << "\n🔧 DEMONSTRATION:\n";

    good_example2::Sparrow goodSparrow;
    good_example2::Penguin goodPenguin;

    cout << "\n   Sparrow (FlyingBird):\n";
    goodSparrow.fly();

    cout << "\n   Penguin (FlightlessBird):\n";
    goodPenguin.walk();
    goodPenguin.swim();

    cout << "\n✨ SOLUTION:\n";
    cout << "   • Separate FlyingBird and FlightlessBird\n";
    cout << "   • Each subclass can do what it promises\n";
    cout << "   • No unexpected exceptions\n";
}

// ============================================
// EXAMPLE 3: CONTRACT VIOLATION
// ============================================

namespace bad_example3 {
// ❌ BAD: Violates preconditions

class FileReader {
public:
    virtual ~FileReader() = default;

    // Contract: filename must not be empty
    virtual string read(const string& filename) {
        if (filename.empty()) {
            throw invalid_argument("Filename cannot be empty");
        }
        return "File content from: " + filename;
    }
};

class RestrictedFileReader : public FileReader {
public:
    // 🚨 VIOLATION: Strengthens precondition!
    string read(const string& filename) override {
        // Now also requires .txt extension
        if (filename.empty() || filename.find(".txt") == string::npos) {
            throw invalid_argument("Must be a .txt file");
        }
        return "Restricted file content from: " + filename;
    }
};
}

namespace good_example3 {
// ✅ GOOD: Honors contract

class FileReader {
public:
    virtual ~FileReader() = default;

    virtual string read(const string& filename) {
        if (filename.empty()) {
            throw invalid_argument("Filename cannot be empty");
        }
        return "File content from: " + filename;
    }
};

class TextFileReader : public FileReader {
public:
    // ✅ Doesn't strengthen precondition
    // Just provides specialized behavior
    string read(const string& filename) override {
        if (filename.empty()) {
            throw invalid_argument("Filename cannot be empty");
        }

        // Additional behavior is fine, but doesn't reject valid inputs
        string content = "Text file content from: " + filename;
        if (filename.find(".txt") != string::npos) {
            content += " [Optimized for .txt]";
        }
        return content;
    }
};
}

void example3_contract_violation() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 3: CONTRACT VIOLATION                  ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (Strengthens Precondition):\n";
    cout << "```cpp\n";
    cout << "class FileReader {\n";
    cout << "    // Requires: filename not empty\n";
    cout << "    virtual string read(const string& filename);\n";
    cout << "};\n\n";
    cout << "class RestrictedFileReader : public FileReader {\n";
    cout << "    // 🚨 Now requires: filename not empty AND .txt extension\n";
    cout << "    string read(const string& filename) override;\n";
    cout << "};\n";
    cout << "```\n";

    cout << "\n🚨 DEMONSTRATION:\n";

    cout << "\n   FileReader with 'data.csv':\n";
    bad_example3::FileReader* reader1 = new bad_example3::FileReader();
    try {
        cout << "   ✅ " << reader1->read("data.csv") << endl;
    } catch (const exception& e) {
        cout << "   ❌ " << e.what() << endl;
    }

    cout << "\n   RestrictedFileReader with 'data.csv':\n";
    bad_example3::FileReader* reader2 = new bad_example3::RestrictedFileReader();
    try {
        cout << "   " << reader2->read("data.csv") << endl;
    } catch (const exception& e) {
        cout << "   ❌ " << e.what() << endl;
    }

    delete reader1;
    delete reader2;

    cout << "\n🚨 PROBLEM:\n";
    cout << "   • Subclass strengthens precondition\n";
    cout << "   • Client code that works with FileReader breaks with subclass\n";
    cout << "   • Not truly substitutable\n";

    cout << "\n✅ GOOD CODE:\n";
    cout << "   • Subclass accepts all inputs parent accepts\n";
    cout << "   • Can provide additional behavior\n";
    cout << "   • But doesn't reject valid inputs\n";

    delete reader1;
    delete reader2;
}

// ============================================
// EXAMPLE 4: EMPTY/THROWING METHODS
// ============================================

namespace bad_example4 {
// ❌ BAD: Empty override

class Account {
public:
    virtual ~Account() = default;

    virtual void withdraw(double amount) {
        cout << "   💰 Withdrawing $" << amount << endl;
    }

    virtual void deposit(double amount) {
        cout << "   💵 Depositing $" << amount << endl;
    }
};

class FixedDepositAccount : public Account {
public:
    // 🚨 VIOLATION: Empty override changes behavior
    void withdraw(double amount) override {
        // Can't withdraw from fixed deposit!
        cout << "   ❌ Cannot withdraw from fixed deposit" << endl;
    }
};
}

namespace good_example4 {
// ✅ GOOD: Proper hierarchy

class Account {
public:
    virtual ~Account() = default;
    virtual void deposit(double amount) {
        cout << "   💵 Depositing $" << amount << endl;
    }
};

class WithdrawableAccount : public Account {
public:
    virtual void withdraw(double amount) {
        cout << "   💰 Withdrawing $" << amount << endl;
    }
};

class SavingsAccount : public WithdrawableAccount {
    // Can both deposit and withdraw
};

class FixedDepositAccount : public Account {
    // Can only deposit, no withdraw method
};
}

void example4_empty_methods() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 4: EMPTY/THROWING METHODS              ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (Empty Override):\n";
    cout << "```cpp\n";
    cout << "class FixedDepositAccount : public Account {\n";
    cout << "    void withdraw(double amount) override {\n";
    cout << "        // Empty! Does nothing!\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "```\n";

    cout << "\n🚨 DEMONSTRATION:\n";

    bad_example4::Account* account = new bad_example4::FixedDepositAccount();
    cout << "\n   Trying to withdraw from FixedDepositAccount:\n";
    account->withdraw(100);

    delete account;

    cout << "\n🚨 PROBLEM:\n";
    cout << "   • Client expects withdraw() to work\n";
    cout << "   • Subclass silently does nothing\n";
    cout << "   • Violates expectation\n";

    cout << "\n✅ GOOD CODE:\n";
    cout << "   • Create WithdrawableAccount for accounts that can withdraw\n";
    cout << "   • FixedDeposit only inherits deposit capability\n";
    cout << "   • Type system prevents invalid operations\n";
}

// ============================================
// EXAMPLE 5: HOW TO FOLLOW LSP
// ============================================

void example5_how_to_follow() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 5: HOW TO FOLLOW LSP                   ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n✅ GUIDELINES TO FOLLOW LSP:\n";

    cout << "\n1. HONOR THE CONTRACT:\n";
    cout << "   • Don't strengthen preconditions\n";
    cout << "   • Don't weaken postconditions\n";
    cout << "   • Preserve invariants\n";
    cout << "   • Don't throw new exceptions\n";

    cout << "\n2. USE \"IS-A\" TEST PROPERLY:\n";
    cout << "   ❌ Square IS-A Rectangle (mathematically yes, OOP no!)\n";
    cout << "   ✅ Sparrow IS-A FlyingBird (behavioral yes)\n";
    cout << "   ✅ Penguin IS-A FlightlessBird (behavioral yes)\n";

    cout << "\n3. AVOID THESE PATTERNS:\n";
    cout << "   ❌ Empty overridden methods\n";
    cout << "   ❌ Throwing NotImplementedException\n";
    cout << "   ❌ Checking type with instanceof/dynamic_cast\n";
    cout << "   ❌ Refusing bequest (not using inherited methods)\n";

    cout << "\n4. DESIGN HIERARCHY BASED ON BEHAVIOR:\n";
    cout << "   • Not just conceptual relationships\n";
    cout << "   • Think about substitutability\n";
    cout << "   • Client expectations matter most\n";

    cout << "\n5. USE COMPOSITION WHEN NEEDED:\n";
    cout << "   • \"HAS-A\" relationship might be better\n";
    cout << "   • Example: Square HAS-A side, not IS-A Rectangle\n";

    cout << "\n🔍 CHECKLIST:\n";
    cout << "   □ Can subclass be used anywhere parent is used?\n";
    cout << "   □ Does subclass accept all inputs parent accepts?\n";
    cout << "   □ Does subclass provide all outputs parent promises?\n";
    cout << "   □ Does subclass maintain all invariants?\n";
    cout << "   □ No need for type checks in client code?\n";
}

// ============================================
// EXAMPLE 6: BENEFITS & SUMMARY
// ============================================

void example6_benefits() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 6: BENEFITS & SUMMARY                  ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n✅ BENEFITS OF LSP:\n";

    cout << "\n1. RELIABLE POLYMORPHISM:\n";
    cout << "   • Subclasses work correctly when substituted\n";
    cout << "   • No surprises or unexpected behavior\n";
    cout << "   • Type system helps catch errors\n";

    cout << "\n2. CODE REUSABILITY:\n";
    cout << "   • Client code works with entire hierarchy\n";
    cout << "   • Don't need special cases for subclasses\n";
    cout << "   • Generic algorithms work correctly\n";

    cout << "\n3. MAINTAINABILITY:\n";
    cout << "   • Add new subclasses without breaking clients\n";
    cout << "   • Clear contracts and expectations\n";
    cout << "   • Easier to understand inheritance\n";

    cout << "\n4. TESTABILITY:\n";
    cout << "   • Tests for base class apply to subclasses\n";
    cout << "   • Consistent behavior across hierarchy\n";
    cout << "   • Fewer special cases to test\n";

    cout << "\n⚠️  COMMON VIOLATIONS:\n";
    cout << "   • Square-Rectangle problem\n";
    cout << "   • Bird-Penguin problem\n";
    cout << "   • Strengthening preconditions\n";
    cout << "   • Weakening postconditions\n";
    cout << "   • Empty/throwing overridden methods\n";

    cout << "\n💡 KEY INSIGHT:\n";
    cout << "   \"Design inheritance based on behavior, not concepts!\n";
    cout << "    Ask: Can I substitute? Not: Is it conceptually related?\"\n";

    cout << "\n🎯 REMEMBER:\n";
    cout << "   • Substitutability is about behavior\n";
    cout << "   • Honor the contract\n";
    cout << "   • Client expectations matter\n";
    cout << "   • When in doubt, use composition\n";
}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║       LISKOV SUBSTITUTION PRINCIPLE (LSP)             ║\n";
    cout << "║                                                       ║\n";
    cout << "║  \"Subclasses must be substitutable for base class\"    ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n";

    example1_rectangle_square();
    example2_bird_penguin();
    example3_contract_violation();
    example4_empty_methods();
    example5_how_to_follow();
    example6_benefits();

    cout << "\n\n";
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║      🎉 LISKOV SUBSTITUTION PRINCIPLE COMPLETE! 🎉    ║\n";
    cout << "║                                                       ║\n";
    cout << "║  Key Takeaway: Design inheritance based on behavior,  ║\n";
    cout << "║  not concepts. Subclasses must be truly              ║\n";
    cout << "║  substitutable!                                      ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n\n";

    return 0;
}
