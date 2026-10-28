#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#include <print>

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
    std::println("   Expected area: 20");
    std::println("   Actual area: {}", rect.getArea());

    // Works for Rectangle, but breaks for Square!
    if (rect.getArea() != 20) {
        std::println("   🚨 VIOLATION: Area is not 20!");
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
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 1: RECTANGLE-SQUARE PROBLEM            ║");
    std::println("╚══════════════════════════════════════════════════╝");

    bad_example1::Rectangle rect(3, 4);
    std::println("");
    std::println("   Testing with Rectangle:");
    bad_example1::processRectangle(rect);

    bad_example1::Square square(5);
    std::println("");
    std::println("   Testing with Square (should work the same):");
    bad_example1::processRectangle(square);

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
        std::println("   🦅 Flying in the sky!");
    }
};

class Sparrow : public Bird {
public:
    void fly() override {
        std::println("   🐦 Sparrow flying!");
    }
};

class Penguin : public Bird {
public:
    // 🚨 VIOLATION: Penguin can't fly!
    void fly() override {
        throw std::runtime_error("Penguins can't fly!");
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
        std::println("   🍴 Bird is eating");
    }
};

class FlyingBird : public Bird {
public:
    virtual void fly() {
        std::println("   🦅 Flying!");
    }
};

class FlightlessBird : public Bird {
public:
    virtual void walk() {
        std::println("   🚶 Walking!");
    }
};

class Sparrow : public FlyingBird {
public:
    void fly() override {
        std::println("   🐦 Sparrow flying!");
    }
};

class Penguin : public FlightlessBird {
public:
    void walk() override {
        std::println("   🐧 Penguin waddling!");
    }

    void swim() {
        std::println("   🏊 Penguin swimming!");
    }
};
}

void example2_bird_penguin() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 2: BIRD-PENGUIN PROBLEM                ║");
    std::println("╚══════════════════════════════════════════════════╝");

    bad_example2::Sparrow sparrow;
    std::println("");
    std::println("   Making Sparrow fly:");
    bad_example2::makeBirdFly(sparrow);

    bad_example2::Penguin penguin;
    std::println("");
    std::println("   Making Penguin fly:");
    try {
        bad_example2::makeBirdFly(penguin);
    } catch (const std::exception& e) {
        std::println("   💥 Exception: {}", e.what());
    }

    good_example2::Sparrow goodSparrow;
    good_example2::Penguin goodPenguin;

    std::println("");
    std::println("   Sparrow (FlyingBird):");
    goodSparrow.fly();

    std::println("");
    std::println("   Penguin (FlightlessBird):");
    goodPenguin.walk();
    goodPenguin.swim();

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
    virtual std::string read(const std::string& filename) {
        if (filename.empty()) {
            throw std::invalid_argument("Filename cannot be empty");
        }
        return "File content from: " + filename;
    }
};

class RestrictedFileReader : public FileReader {
public:
    // 🚨 VIOLATION: Strengthens precondition!
    std::string read(const std::string& filename) override {
        // Now also requires .txt extension
        if (filename.empty() || filename.find(".txt") == std::string::npos) {
            throw std::invalid_argument("Must be a .txt file");
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

    virtual std::string read(const std::string& filename) {
        if (filename.empty()) {
            throw std::invalid_argument("Filename cannot be empty");
        }
        return "File content from: " + filename;
    }
};

class TextFileReader : public FileReader {
public:
    // ✅ Doesn't strengthen precondition
    // Just provides specialized behavior
    std::string read(const std::string& filename) override {
        if (filename.empty()) {
            throw std::invalid_argument("Filename cannot be empty");
        }

        // Additional behavior is fine, but doesn't reject valid inputs
        std::string content = "Text file content from: " + filename;
        if (filename.find(".txt") != std::string::npos) {
            content += " [Optimized for .txt]";
        }
        return content;
    }
};
}

void example3_contract_violation() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 3: CONTRACT VIOLATION                  ║");
    std::println("╚══════════════════════════════════════════════════╝");

    bad_example3::FileReader* reader1 = new bad_example3::FileReader();
    try {
        std::println("   ✅ {}", reader1->read("data.csv"));
    } catch (const std::exception& e) {
        std::println("   ❌ {}", e.what());
    }

    std::println("");
    std::println("   RestrictedFileReader with 'data.csv':");
    bad_example3::FileReader* reader2 = new bad_example3::RestrictedFileReader();
    try {
        std::println("   {}", reader2->read("data.csv"));
    } catch (const std::exception& e) {
        std::println("   ❌ {}", e.what());
    }

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
        std::println("   💰 Withdrawing ${}", amount);
    }

    virtual void deposit(double amount) {
        std::println("   💵 Depositing ${}", amount);
    }
};

class FixedDepositAccount : public Account {
public:
    // 🚨 VIOLATION: Empty override changes behavior
    void withdraw(double amount) override {
        // Can't withdraw from fixed deposit!
        std::println("   ❌ Cannot withdraw from fixed deposit");
    }
};
}

namespace good_example4 {
// ✅ GOOD: Proper hierarchy

class Account {
public:
    virtual ~Account() = default;
    virtual void deposit(double amount) {
        std::println("   💵 Depositing ${}", amount);
    }
};

class WithdrawableAccount : public Account {
public:
    virtual void withdraw(double amount) {
        std::println("   💰 Withdrawing ${}", amount);
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
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 4: EMPTY/THROWING METHODS              ║");
    std::println("╚══════════════════════════════════════════════════╝");

    bad_example4::Account* account = new bad_example4::FixedDepositAccount();
    std::println("");
    std::println("   Trying to withdraw from FixedDepositAccount:");
    account->withdraw(100);

    delete account;

}

// ============================================
// EXAMPLE 5: HOW TO FOLLOW LSP
// ============================================

void example5_how_to_follow() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 5: HOW TO FOLLOW LSP                   ║");
    std::println("╚══════════════════════════════════════════════════╝");

}

// ============================================
// EXAMPLE 6: BENEFITS & SUMMARY
// ============================================

void example6_benefits() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 6: BENEFITS & SUMMARY                  ║");
    std::println("╚══════════════════════════════════════════════════╝");

}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    std::println("╔═══════════════════════════════════════════════════════╗");
    std::println("║                                                       ║");
    std::println("║       LISKOV SUBSTITUTION PRINCIPLE (LSP)             ║");
    std::println("║                                                       ║");
    std::println("║  \"Subclasses must be substitutable for base class\"  ║");
    std::println("║                                                       ║");
    std::println("╚═══════════════════════════════════════════════════════╝");

    example1_rectangle_square();
    example2_bird_penguin();
    example3_contract_violation();
    example4_empty_methods();
    example5_how_to_follow();
    example6_benefits();

    return 0;
}
