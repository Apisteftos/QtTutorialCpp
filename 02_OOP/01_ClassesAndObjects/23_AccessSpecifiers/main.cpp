// 02_OOP / 01_ClassesAndObjects / 23_AccessSpecifiers
// -----------------------------------------------------
#include <iostream>
using namespace std;

// ============================================
// EXAMPLE: ACCESS SPECIFIERS
// ============================================

class AccessSpecifierDemo {
private:
    // PRIVATE: Only accessible inside this class
    int privateData;

    void privateMethod() {
        cout << "   Private method called\n";
    }

protected:
    // PROTECTED: Accessible in this class and derived classes
    int protectedData;

    void protectedMethod() {
        cout << "   Protected method called\n";
    }

public:
    // PUBLIC: Accessible from anywhere
    int publicData;

    AccessSpecifierDemo() : privateData(10), protectedData(20), publicData(30) { }

    void publicMethod() {
        cout << "   Public method called\n";

        // Can access all members inside the class
        cout << "   Accessing private: " << privateData << "\n";
        cout << "   Accessing protected: " << protectedData << "\n";
        cout << "   Accessing public: " << publicData << "\n";

        privateMethod();
        protectedMethod();
    }
};

void demonstrateAccessSpecifiers() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE: ACCESS SPECIFIERS        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    AccessSpecifierDemo obj;

    cout << "--- From outside the class ---\n\n";

    cout << "✅ Can access public:\n";
    obj.publicData = 100;
    cout << "   Public data: " << obj.publicData << "\n";
    obj.publicMethod();

    cout << "\n❌ Cannot access private:\n";
    cout << "   // obj.privateData = 100;  // Error!\n";
    cout << "   // obj.privateMethod();     // Error!\n";

    cout << "\n❌ Cannot access protected (from outside):\n";
    cout << "   // obj.protectedData = 100; // Error!\n";
    cout << "   // obj.protectedMethod();    // Error!\n";

    cout << "\n💡 Access Levels:\n";
    cout << "   private:   Only this class\n";
    cout << "   protected: This class + derived classes\n";
    cout << "   public:    Anywhere\n";
}

int main() {
    demonstrateAccessSpecifiers();
    return 0;
}
