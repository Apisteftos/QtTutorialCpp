#include <iostream>
using namespace std;

// ===============================
// BASE + DERIVED (for dynamic_cast)
// ===============================
class Base {
public:
    virtual void show() {
        cout << "Base show()" << endl;
    }

    virtual ~Base() {}
};

class Derived : public Base {
public:
    void show() override {
        cout << "Derived show()" << endl;
    }

    void onlyDerived() {
        cout << "Only Derived function" << endl;
    }
};

// ===============================
// MAIN
// ===============================
int main() {

    cout << "===============================" << endl;
    cout << "1. STATIC_CAST" << endl;
    cout << "===============================" << endl;

    int a = 10;
    double b = static_cast<double>(a);
    cout << "int -> double: " << b << endl;

    float f = 5.7f;
    int i = static_cast<int>(f);
    cout << "float -> int: " << i << endl;

    Base* basePtr1 = new Derived();
    Derived* derivedPtr1 = static_cast<Derived*>(basePtr1);
    derivedPtr1->onlyDerived();

    cout << "\n===============================" << endl;
    cout << "2. DYNAMIC_CAST" << endl;
    cout << "===============================" << endl;

    Base* basePtr2 = new Derived();

    Derived* derivedPtr2 = dynamic_cast<Derived*>(basePtr2);
    if (derivedPtr2) {
        derivedPtr2->show();
        derivedPtr2->onlyDerived();
    } else {
        cout << "Invalid dynamic_cast" << endl;
    }

    // Safe failure case
    Base* baseOnly = new Base();
    Derived* failCast = dynamic_cast<Derived*>(baseOnly);

    if (failCast == nullptr) {
        cout << "dynamic_cast failed safely (nullptr)" << endl;
    }

    cout << "\n===============================" << endl;
    cout << "3. CONST_CAST" << endl;
    cout << "===============================" << endl;

    const int x = 100;
    cout << "Original const value: " << x << endl;

    int* modifiable = const_cast<int*>(&x);
    cout << "After const_cast (unsafe to modify): " << *modifiable << endl;

    // ⚠️ modifying x here is undefined behavior (NOT recommended)
    // *modifiable = 200;

    cout << "\n===============================" << endl;
    cout << "4. REINTERPRET_CAST" << endl;
    cout << "===============================" << endl;

    int value = 65;
    char* charPtr = reinterpret_cast<char*>(&value);

    cout << "int value: " << value << endl;
    cout << "as char (ASCII): " << *charPtr << endl;

    Base* basePtr3 = new Base();
    long address = reinterpret_cast<long>(basePtr3);
    cout << "Pointer as integer address: " << address << endl;

    cout << "\n===============================" << endl;
    cout << "5. C-STYLE CAST" << endl;
    cout << "===============================" << endl;

    double d = 9.99;
    int cStyleInt = (int)d;

    cout << "double -> int (C-style): " << cStyleInt << endl;

    Base* basePtr4 = new Derived();
    Derived* cStyleDerived = (Derived*)basePtr4;

    cStyleDerived->show();

    cout << "\n===============================" << endl;
    cout << "END OF PROGRAM" << endl;
    cout << "===============================" << endl;

    delete basePtr1;
    delete basePtr2;
    delete baseOnly;
    delete basePtr3;
    delete basePtr4;

    return 0;
}
