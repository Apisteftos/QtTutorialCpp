// 02_OOP / 01_ClassesAndObjects / 23_AccessSpecifiers
// -----------------------------------------------------
#include <print>

// ============================================
// EXAMPLE: ACCESS SPECIFIERS
// ============================================

class AccessSpecifierDemo {
private:
    // PRIVATE: Only accessible inside this class
    int privateData;

    void privateMethod() {
        std::println("   Private method called");
    }

protected:
    // PROTECTED: Accessible in this class and derived classes
    int protectedData;

    void protectedMethod() {
        std::println("   Protected method called");
    }

public:
    // PUBLIC: Accessible from anywhere
    int publicData;

    AccessSpecifierDemo() : privateData(10), protectedData(20), publicData(30) { }

    void publicMethod() {
        std::println("   Public method called");

        // Can access all members inside the class
        std::println("   Accessing private: {}", privateData);
        std::println("   Accessing protected: {}", protectedData);
        std::println("   Accessing public: {}", publicData);

        privateMethod();
        protectedMethod();
    }
};

// A derived class sees the protected and public members, but not the private ones
class DerivedDemo : public AccessSpecifierDemo {
public:
    void showInherited() {
        std::println("   Derived reads protected: {}", protectedData);  // ✅
        std::println("   Derived reads public: {}", publicData);        // ✅
        protectedMethod();                                              // ✅

        // std::println("{}", privateData);  // ❌ Error: private in the base class
        // privateMethod();                  // ❌ Error: private in the base class
    }
};

void demonstrateAccessSpecifiers() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE: ACCESS SPECIFIERS        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    AccessSpecifierDemo obj;

    std::println("--- From outside the class ---");
    std::println("");

    std::println("✅ Can access public:");
    obj.publicData = 100;
    std::println("   Public data: {}", obj.publicData);
    obj.publicMethod();

    std::println("");
    std::println("❌ Cannot access private:");
    std::println("   // obj.privateData = 100;  // Error!");
    std::println("   // obj.privateMethod();     // Error!");

    std::println("");
    std::println("❌ Cannot access protected (from outside):");
    std::println("   // obj.protectedData = 100; // Error!");
    std::println("   // obj.protectedMethod();    // Error!");

    std::println("");
    std::println("--- From a derived class ---");
    std::println("");

    DerivedDemo derived;
    derived.showInherited();
}

int main() {
    demonstrateAccessSpecifiers();
    return 0;
}
