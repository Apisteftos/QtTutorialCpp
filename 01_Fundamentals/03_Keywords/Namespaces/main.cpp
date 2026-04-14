#include <iostream>
using namespace std;

// ===============================
// 1. BASIC NAMESPACE
// ===============================
namespace Math {

int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}
}

// ===============================
// 2. NESTED NAMESPACE
// ===============================
namespace Physics {

namespace Mechanics {
double velocity(double distance, double time) {
    return distance / time;
}
}
}

// ===============================
// 3. INLINE NAMESPACE (versioning concept)
// ===============================
namespace Library {

inline namespace v1 {
void print() {
    cout << "Library version 1" << endl;
}
}

namespace v2 {
void print() {
    cout << "Library version 2" << endl;
}
}
}

// ===============================
// 4. ANONYMOUS NAMESPACE (internal linkage)
// ===============================
namespace {
int secretValue = 999;

void hiddenFunction() {
    cout << "Hidden function inside anonymous namespace" << endl;
}
}


namespace A {
int value = 100;
}

namespace B {
int value = 200;
}


// ===============================
// MAIN
// ===============================
int main() {

    cout << "===============================" << endl;
    cout << "1. BASIC NAMESPACE ACCESS (::)" << endl;
    cout << "===============================" << endl;

    cout << "Add: " << Math::add(5, 3) << endl;
    cout << "Multiply: " << Math::multiply(4, 6) << endl;

    cout << "\n===============================" << endl;
    cout << "2. NESTED NAMESPACE" << endl;
    cout << "===============================" << endl;

    cout << "Velocity: "
         << Physics::Mechanics::velocity(100, 5)
         << endl;

    cout << "\n===============================" << endl;
    cout << "3. USING KEYWORD (selective import)" << endl;
    cout << "===============================" << endl;

    using Math::add;
    cout << "Using add directly: " << add(10, 20) << endl;

    cout << "\n===============================" << endl;
    cout << "4. USING NAMESPACE (global import)" << endl;
    cout << "===============================" << endl;

    using namespace Physics::Mechanics;

    cout << "Velocity using import: "
         << velocity(200, 4)
         << endl;

    cout << "\n===============================" << endl;
    cout << "5. INLINE NAMESPACE (versioning)" << endl;
    cout << "===============================" << endl;

    // v1 is automatically active because it's inline
    Library::print();

    // explicitly accessing v2
    Library::v2::print();

    cout << "\n===============================" << endl;
    cout << "6. ANONYMOUS NAMESPACE" << endl;
    cout << "===============================" << endl;

    cout << "Secret value: " << secretValue << endl;
    hiddenFunction();

    cout << "\n===============================" << endl;
    cout << "7. NAME CONFLICT RESOLUTION" << endl;
    cout << "===============================" << endl;



    cout << "A value: " << A::value << endl;
    cout << "B value: " << B::value << endl;

    cout << "\n===============================" << endl;
    cout << "END OF PROGRAM" << endl;
    cout << "===============================" << endl;

    return 0;
}
