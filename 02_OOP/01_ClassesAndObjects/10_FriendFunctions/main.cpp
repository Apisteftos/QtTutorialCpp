#include <iostream>
#include <string>
#include <cmath>
#include <vector>
using namespace std;

// ============================================
// WHAT ARE FRIEND FUNCTIONS?
// ============================================

/*
 * FRIEND FUNCTIONS:
 * - Non-member functions that can access private/protected members
 * - Declared with 'friend' keyword inside class
 * - Not member functions (no 'this' pointer)
 * - Break encapsulation (use sparingly!)
 *
 * TYPES:
 * 1. Friend function (standalone function)
 * 2. Friend class (entire class is friend)
 * 3. Friend member function (specific function from another class)
 *
 * COMMON USES:
 * 1. Operator overloading (especially <<, >>)
 * 2. Two-way interaction between classes
 * 3. Factory functions
 * 4. Testing and debugging
 * 5. Performance-critical code
 *
 * FRIENDSHIP RULES:
 * - Friendship is NOT mutual (if A friends B, B doesn't friend A)
 * - Friendship is NOT inherited
 * - Friendship is NOT transitive (A friends B, B friends C ≠ A friends C)
 */

// ============================================
// EXAMPLE 1: BASIC FRIEND FUNCTION
// ============================================

class Box {
private:
    double width;
    double height;
    double depth;

public:
    Box(double w, double h, double d) : width(w), height(h), depth(d) {
        cout << "  Box created: " << width << "×" << height << "×" << depth << "\n";
    }

    // Declare friend function
    // Friend can access private members!
    friend double calculateVolume(const Box& box);
    friend void displayBox(const Box& box);
};

// Define friend function (outside class)
// Note: NO 'friend' keyword in definition
double calculateVolume(const Box& box) {
    // Can access private members!
    return box.width * box.height * box.depth;
}

void displayBox(const Box& box) {
    // Can access private members!
    cout << "  Box: " << box.width << " × "
         << box.height << " × " << box.depth << "\n";
}

void demonstrateBasicFriend() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: BASIC FRIEND FUNCTION  ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Box box(10.0, 5.0, 3.0);

    cout << "\n--- Using friend functions ---\n";
    displayBox(box);
    cout << "  Volume: " << calculateVolume(box) << "\n";

    cout << "\n💡 Friend functions:\n";
    cout << "   • Not member functions\n";
    cout << "   • Can access private members\n";
    cout << "   • Called like regular functions\n";
    cout << "   • No 'this' pointer\n";
}

// ============================================
// EXAMPLE 2: FRIEND VS MEMBER FUNCTION
// ============================================

class Point {
private:
    double x, y;

public:
    Point(double xVal, double yVal) : x(xVal), y(yVal) { }

    // MEMBER FUNCTION
    double distanceFromOrigin() const {
        // Has 'this' pointer
        // Access members directly
        return sqrt(x * x + y * y);
    }

    // Declare FRIEND FUNCTION
    friend double distanceBetween(const Point& p1, const Point& p2);
};

// Define FRIEND FUNCTION (not a member)
double distanceBetween(const Point& p1, const Point& p2) {
    // No 'this' pointer
    // Access members via parameters
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return sqrt(dx * dx + dy * dy);
}

void demonstrateFriendVsMember() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: FRIEND vs MEMBER       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Point p1(3.0, 4.0);
    Point p2(6.0, 8.0);

    cout << "--- Member function (needs one object) ---\n";
    cout << "  p1.distanceFromOrigin(): " << p1.distanceFromOrigin() << "\n";

    cout << "\n--- Friend function (takes two objects) ---\n";
    cout << "  distanceBetween(p1, p2): " << distanceBetween(p1, p2) << "\n";

    cout << "\n💡 Member vs Friend:\n";
    cout << "   Member:  obj.func()       - has 'this'\n";
    cout << "   Friend:  func(obj1, obj2) - no 'this', symmetric\n";
}

// ============================================
// EXAMPLE 3: FRIEND FOR OPERATOR OVERLOADING
// ============================================

class Complex {
private:
    double real;
    double imag;

public:
    Complex(double r = 0, double i = 0) : real(r), imag(i) { }

    // Member operator for Complex + Complex
    Complex operator+(const Complex& other) const {
        return Complex(real + other.real, imag + other.imag);
    }

    // MUST be friend for symmetric operations
    friend Complex operator*(double scalar, const Complex& c);

    // Stream operators MUST be friend
    friend ostream& operator<<(ostream& os, const Complex& c);
    friend istream& operator>>(istream& is, Complex& c);
};

// Friend function for scalar * Complex
Complex operator*(double scalar, const Complex& c) {
    return Complex(c.real * scalar, c.imag * scalar);
}

// Friend function for output
ostream& operator<<(ostream& os, const Complex& c) {
    os << c.real;
    if (c.imag >= 0) {
        os << " + " << c.imag << "i";
    } else {
        os << " - " << -c.imag << "i";
    }
    return os;
}

// Friend function for input
istream& operator>>(istream& is, Complex& c) {
    cout << "Enter real part: ";
    is >> c.real;
    cout << "Enter imaginary part: ";
    is >> c.imag;
    return is;
}

void demonstrateFriendOperators() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: FRIEND OPERATORS       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Complex c1(3.0, 4.0);
    Complex c2(1.0, 2.0);

    cout << "--- Complex numbers ---\n";
    cout << "  c1 = " << c1 << "\n";
    cout << "  c2 = " << c2 << "\n";

    cout << "\n--- Operations ---\n";
    cout << "  c1 + c2 = " << (c1 + c2) << "\n";
    cout << "  2.0 * c1 = " << (2.0 * c1) << "\n";

    cout << "\n💡 Why friend for operators?\n";
    cout << "   • Stream operators (<<, >>) need left operand as stream\n";
    cout << "   • Symmetric operations (2.0 * c and c * 2.0)\n";
    cout << "   • Natural syntax\n";
}

// ============================================
// EXAMPLE 4: FRIEND CLASS
// ============================================

class Engine {
private:
    int horsepower;
    double fuelEfficiency;

public:
    Engine(int hp, double efficiency)
        : horsepower(hp), fuelEfficiency(efficiency) {
        cout << "  Engine: " << horsepower << "hp, "
             << fuelEfficiency << "L/100km\n";
    }

    // Declare entire Car class as friend
    friend class Car;
};

class Car {
private:
    string model;
    Engine engine;

public:
    Car(string m, int hp, double efficiency)
        : model(m), engine(hp, efficiency) {
        cout << "  Car created: " << model << "\n";
    }

    void displayDetails() {
        cout << "  Model: " << model << "\n";
        // Can access Engine's private members!
        cout << "  Engine: " << engine.horsepower << "hp\n";
        cout << "  Efficiency: " << engine.fuelEfficiency << "L/100km\n";
    }

    void tuneEngine() {
        // Can modify Engine's private members!
        engine.horsepower += 50;
        engine.fuelEfficiency += 0.5;
        cout << "  Engine tuned! New power: " << engine.horsepower << "hp\n";
    }
};

void demonstrateFriendClass() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: FRIEND CLASS           ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating car ---\n";
    Car car("BMW M3", 500, 12.5);

    cout << "\n--- Car details ---\n";
    car.displayDetails();

    cout << "\n--- Tuning engine ---\n";
    car.tuneEngine();

    cout << "\n--- Updated details ---\n";
    car.displayDetails();

    cout << "\n💡 Friend class:\n";
    cout << "   • ALL members of Car can access Engine's private members\n";
    cout << "   • Use for tightly coupled classes\n";
    cout << "   • Friendship is NOT mutual!\n";
}

// ============================================
// EXAMPLE 5: FRIEND MEMBER FUNCTION
// ============================================

// Forward declarations
class Rectangle;
class Triangle;

class Triangle {
private:
    double base, height;

public:
    Triangle(double b, double h) : base(b), height(h) { }

    double area() const {
        return 0.5 * base * height;
    }

    // This function can access Rectangle's private members
    bool isLargerThan(const Rectangle& rect) const;
};


class Rectangle {
private:
    double width, height;

public:
    Rectangle(double w, double h) : width(w), height(h) { }

    double area() const {
        return width * height;
    }

    // Declare specific Triangle member as friend
    friend bool Triangle::isLargerThan(const Rectangle& rect) const;
};

bool Triangle::isLargerThan(const Rectangle& rect) const {
    // Can access rect.width and rect.height directly!
    double rectArea = rect.width * rect.height;
    double triArea = 0.5 * base * height;
    return triArea > rectArea;
}



void demonstrateFriendMemberFunction() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: FRIEND MEMBER FUNCTION ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Rectangle rect(5.0, 4.0);
    Triangle tri(8.0, 6.0);

    cout << "--- Shapes ---\n";
    cout << "  Rectangle area: " << rect.area() << "\n";
    cout << "  Triangle area: " << tri.area() << "\n";

    cout << "\n--- Comparison ---\n";
    if (tri.isLargerThan(rect)) {
        cout << "  Triangle is larger than rectangle\n";
    } else {
        cout << "  Rectangle is larger than triangle\n";
    }

    cout << "\n💡 Friend member function:\n";
    cout << "   • Only ONE specific function is friend\n";
    cout << "   • More fine-grained control than friend class\n";
    cout << "   • Requires forward declarations\n";
}

// ============================================
// EXAMPLE 6: FRIENDSHIP IS NOT MUTUAL
// ============================================
class ClassB;  // Forward declaration

class ClassA {
private:
    int secretA;

public:
    ClassA(int val) : secretA(val) { }

    // ClassB is friend of ClassA
    friend class ClassB;

    void tryAccessB(const ClassB& b);
};

class ClassB {
private:
    int secretB;

public:
    ClassB(int val) : secretB(val) { }

    void accessA(const ClassA& a) {
        // ClassB is friend of ClassA
        // So ClassB CAN access ClassA's private members
        cout << "  ClassB accessing ClassA::secretA = " << a.secretA << " ✅\n";
    }

    int getSecretB() const { return secretB; }
};

void ClassA::tryAccessB(const ClassB& b) {
    // ClassA is NOT friend of ClassB
    // So ClassA CANNOT access ClassB's private members
    // cout << b.secretB;  // ❌ Error! Cannot access
    cout << "  ClassA cannot access ClassB::secretB directly ❌\n";
    cout << "  ClassA can only use public methods: secretB = "
         << b.getSecretB() << "\n";
}

void demonstrateFriendshipNotMutual() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: NOT MUTUAL             ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    ClassA a(100);
    ClassB b(200);

    cout << "--- ClassB accessing ClassA ---\n";
    b.accessA(a);

    cout << "\n--- ClassA trying to access ClassB ---\n";
    a.tryAccessB(b);

    cout << "\n💡 Friendship is NOT mutual:\n";
    cout << "   • If A friends B, B can access A's private members\n";
    cout << "   • But A CANNOT access B's private members\n";
    cout << "   • Need explicit friend declaration in both directions\n";
}

// ============================================
// EXAMPLE 7: FRIENDSHIP IS NOT TRANSITIVE
// ============================================

class Alpha {
private:
    int alphaSecret;

public:
    Alpha(int val) : alphaSecret(val) { }
    friend class Beta;
};

class Beta {
private:
    int betaSecret;

public:
    Beta(int val) : betaSecret(val) { }
    friend class Gamma;

    void accessAlpha(const Alpha& a) {
        cout << "  Beta accessing Alpha::alphaSecret = "
             << a.alphaSecret << " ✅\n";
    }
};

class Gamma {
public:
    void accessBeta(const Beta& b) {
        cout << "  Gamma accessing Beta::betaSecret = "
             << b.betaSecret << " ✅\n";
    }

    void tryAccessAlpha(const Alpha& a) {
        // Gamma is friend of Beta
        // Beta is friend of Alpha
        // But Gamma is NOT friend of Alpha!
        // cout << a.alphaSecret;  // ❌ Error!
        cout << "  Gamma cannot access Alpha::alphaSecret ❌\n";
        cout << "  (Friendship is not transitive)\n";
    }
};

void demonstrateFriendshipNotTransitive() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 7: NOT TRANSITIVE         ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Alpha alpha(100);
    Beta beta(200);
    Gamma gamma;

    cout << "--- Beta accessing Alpha (friend) ---\n";
    beta.accessAlpha(alpha);

    cout << "\n--- Gamma accessing Beta (friend) ---\n";
    gamma.accessBeta(beta);

    cout << "\n--- Gamma trying to access Alpha (not friend) ---\n";
    gamma.tryAccessAlpha(alpha);

    cout << "\n💡 Friendship is NOT transitive:\n";
    cout << "   • Alpha friends Beta\n";
    cout << "   • Beta friends Gamma\n";
    cout << "   • But Gamma is NOT friend of Alpha\n";
    cout << "   • Each friendship must be explicit\n";
}

// ============================================
// EXAMPLE 8: REAL-WORLD - MATRIX AND VECTOR
// ============================================

class Vector;  // Forward declaration

class Matrix {
private:
    int rows, cols;
    double** data;

public:
    Matrix(int r, int c) : rows(r), cols(c) {
        data = new double*[rows];
        for (int i = 0; i < rows; i++) {
            data[i] = new double[cols];
            for (int j = 0; j < cols; j++) {
                data[i][j] = 0;
            }
        }
    }

    ~Matrix() {
        for (int i = 0; i < rows; i++) {
            delete[] data[i];
        }
        delete[] data;
    }

    void set(int i, int j, double val) {
        data[i][j] = val;
    }

    void display() const {
        cout << "  Matrix " << rows << "×" << cols << ":\n";
        for (int i = 0; i < rows; i++) {
            cout << "  [";
            for (int j = 0; j < cols; j++) {
                cout << data[i][j];
                if (j < cols - 1) cout << ", ";
            }
            cout << "]\n";
        }
    }

    // Friend function for Matrix * Vector
    friend Vector operator*(const Matrix& m, const Vector& v);
};

class Vector {
private:
    int size;
    double* data;

public:
    Vector(int s) : size(s) {
        data = new double[size];
        for (int i = 0; i < size; i++) {
            data[i] = 0;
        }
    }

    ~Vector() {
        delete[] data;
    }

    void set(int i, double val) {
        data[i] = val;
    }

    double get(int i) const {
        return data[i];
    }

    void display() const {
        cout << "  Vector [";
        for (int i = 0; i < size; i++) {
            cout << data[i];
            if (i < size - 1) cout << ", ";
        }
        cout << "]\n";
    }

    // Friend function for Matrix * Vector
    friend Vector operator*(const Matrix& m, const Vector& v);
};

// Friend function - can access private members of both classes
Vector operator*(const Matrix& m, const Vector& v) {
    Vector result(m.rows);
    for (int i = 0; i < m.rows; i++) {
        double sum = 0;
        for (int j = 0; j < m.cols; j++) {
            sum += m.data[i][j] * v.data[j];
        }
        result.data[i] = sum;
    }
    return result;
}

void demonstrateMatrixVector() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 8: MATRIX × VECTOR        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Matrix m(2, 2);
    m.set(0, 0, 1); m.set(0, 1, 2);
    m.set(1, 0, 3); m.set(1, 1, 4);

    Vector v(2);
    v.set(0, 5);
    v.set(1, 6);

    cout << "--- Matrix ---\n";
    m.display();

    cout << "\n--- Vector ---\n";
    v.display();

    cout << "\n--- Matrix × Vector ---\n";
    Vector result = m * v;
    result.display();

    cout << "\n✅ Friend function enables natural operator:\n";
    cout << "   result = matrix * vector\n";
    cout << "   Needs access to private members of BOTH classes\n";
}

// ============================================
// EXAMPLE 9: WHEN TO USE FRIEND
// ============================================

class Account {
private:
    double balance;

public:
    Account(double initial) : balance(initial) { }

    // Public interface
    double getBalance() const { return balance; }
    void deposit(double amount) { balance += amount; }

    // Friend for special operations
    friend void transferFunds(Account& from, Account& to, double amount);

    // Could use public methods instead:
    static void transferViaMethods(Account& from, Account& to, double amount) {
        if (from.getBalance() >= amount) {
            from.balance -= amount;
            to.balance += amount;
        }
    }
};

// Friend function - direct access
void transferFunds(Account& from, Account& to, double amount) {
    if (from.balance >= amount) {
        from.balance -= amount;
        to.balance += amount;
        cout << "  Transferred $" << amount << " (via friend)\n";
    }
}

void demonstrateWhenToUseFriend() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 9: WHEN TO USE FRIEND     ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Account acc1(1000);
    Account acc2(500);

    cout << "--- Initial balances ---\n";
    cout << "  Account 1: $" << acc1.getBalance() << "\n";
    cout << "  Account 2: $" << acc2.getBalance() << "\n";

    cout << "\n--- Transfer using friend function ---\n";
    transferFunds(acc1, acc2, 200);
    cout << "  Account 1: $" << acc1.getBalance() << "\n";
    cout << "  Account 2: $" << acc2.getBalance() << "\n";

    cout << "\n💡 When to use friend:\n";
    cout << "   ✅ Operator overloading (<<, >>, symmetric ops)\n";
    cout << "   ✅ Tightly coupled classes\n";
    cout << "   ✅ Performance-critical code\n";
    cout << "   ✅ Two-way interactions\n";
    cout << "   ❌ When public interface is sufficient\n";
    cout << "   ❌ When it breaks encapsulation unnecessarily\n";
}

// ============================================
// EXAMPLE 10: TESTING WITH FRIEND
// ============================================

class Stack {
private:
    int* data;
    int top;
    int capacity;

public:
    Stack(int cap) : top(-1), capacity(cap) {
        data = new int[capacity];
    }

    ~Stack() {
        delete[] data;
    }

    void push(int value) {
        if (top < capacity - 1) {
            data[++top] = value;
        }
    }

    int pop() {
        if (top >= 0) {
            return data[top--];
        }
        return -1;
    }

    bool isEmpty() const {
        return top == -1;
    }

    // Friend for testing
    friend class StackTester;
};

class StackTester {
public:
    static void runTests(Stack& stack) {
        cout << "  Running internal tests...\n";

        // Can access private members for testing!
        cout << "    Internal capacity: " << stack.capacity << "\n";
        cout << "    Current top index: " << stack.top << "\n";

        // Verify internal state
        if (stack.top == -1) {
            cout << "    ✅ Stack properly initialized (top = -1)\n";
        }

        // Check data array
        if (stack.data != nullptr) {
            cout << "    ✅ Data array allocated\n";
        }
    }

    static void verifyIntegrity(const Stack& stack) {
        bool valid = true;

        if (stack.top >= stack.capacity) {
            cout << "    ❌ Error: top >= capacity\n";
            valid = false;
        }

        if (stack.top < -1) {
            cout << "    ❌ Error: top < -1\n";
            valid = false;
        }

        if (valid) {
            cout << "    ✅ Stack integrity verified\n";
        }
    }
};

void demonstrateTestingWithFriend() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 10: TESTING WITH FRIEND   ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Stack stack(10);

    cout << "--- Initial tests ---\n";
    StackTester::runTests(stack);
    StackTester::verifyIntegrity(stack);

    cout << "\n--- After operations ---\n";
    stack.push(1);
    stack.push(2);
    stack.push(3);
    StackTester::runTests(stack);
    StackTester::verifyIntegrity(stack);

    cout << "\n✅ Friend for testing:\n";
    cout << "   • Access internal state\n";
    cout << "   • Verify invariants\n";
    cout << "   • White-box testing\n";
    cout << "   • Debugging\n";
}

// ============================================
// MAIN DEMONSTRATION
// ============================================

int main() {
    cout << "========================================\n";
    cout << "FRIEND FUNCTIONS DEMONSTRATION\n";
    cout << "========================================\n";

    demonstrateBasicFriend();
    demonstrateFriendVsMember();
    demonstrateFriendOperators();
    demonstrateFriendClass();
    demonstrateFriendMemberFunction();
    demonstrateFriendshipNotMutual();
    demonstrateFriendshipNotTransitive();
    demonstrateMatrixVector();
    demonstrateWhenToUseFriend();
    demonstrateTestingWithFriend();

    // ============================================
    // Comprehensive Summary
    // ============================================
    cout << "\n========================================\n";
    cout << "WHAT ARE FRIEND FUNCTIONS?\n";
    cout << "========================================\n";
    cout << "Friend functions are non-member functions that have\n";
    cout << "access to private and protected members of a class.\n";
    cout << "\n";
    cout << "Key characteristics:\n";
    cout << "  • Not member functions (no 'this' pointer)\n";
    cout << "  • Can access private/protected members\n";
    cout << "  • Declared with 'friend' keyword in class\n";
    cout << "  • Break encapsulation (use carefully!)\n";
    cout << "\n";
    cout << "Three types:\n";
    cout << "  1. Friend function (standalone)\n";
    cout << "  2. Friend class (entire class)\n";
    cout << "  3. Friend member function (specific function)\n";

    cout << "\n========================================\n";
    cout << "SYNTAX\n";
    cout << "========================================\n";
    cout << "FRIEND FUNCTION:\n";
    cout << "class MyClass {\n";
    cout << "    int privateData;\n";
    cout << "    \n";
    cout << "    // Declare friend\n";
    cout << "    friend void friendFunc(const MyClass& obj);\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Define friend (NO 'friend' keyword)\n";
    cout << "void friendFunc(const MyClass& obj) {\n";
    cout << "    // Can access obj.privateData\n";
    cout << "}\n";
    cout << "\n";
    cout << "FRIEND CLASS:\n";
    cout << "class MyClass {\n";
    cout << "    int privateData;\n";
    cout << "    \n";
    cout << "    friend class FriendClass;\n";
    cout << "};\n";
    cout << "\n";
    cout << "FRIEND MEMBER FUNCTION:\n";
    cout << "class MyClass {\n";
    cout << "    int privateData;\n";
    cout << "    \n";
    cout << "    friend void OtherClass::specificFunc(const MyClass&);\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "FRIEND vs MEMBER FUNCTION\n";
    cout << "========================================\n";
    cout << "┌─────────────────────┬─────────────────────┐\n";
    cout << "│ MEMBER FUNCTION     │ FRIEND FUNCTION     │\n";
    cout << "├─────────────────────┼─────────────────────┤\n";
    cout << "│ Part of class       │ Not part of class   │\n";
    cout << "│ Has 'this' pointer  │ No 'this' pointer   │\n";
    cout << "│ obj.func()          │ func(obj)           │\n";
    cout << "│ Access directly     │ Access via param    │\n";
    cout << "│ One object          │ Multiple objects    │\n";
    cout << "│ Asymmetric          │ Can be symmetric    │\n";
    cout << "└─────────────────────┴─────────────────────┘\n";
    cout << "\n";
    cout << "Example:\n";
    cout << "Member:  double dist = point.distanceFromOrigin();\n";
    cout << "Friend:  double dist = distanceBetween(p1, p2);\n";

    cout << "\n========================================\n";
    cout << "FRIENDSHIP RULES\n";
    cout << "========================================\n";
    cout << "1. NOT MUTUAL:\n";
    cout << "   class A {\n";
    cout << "       friend class B;  // B can access A's private\n";
    cout << "   };\n";
    cout << "   class B { };  // A CANNOT access B's private\n";
    cout << "\n";
    cout << "2. NOT INHERITED:\n";
    cout << "   class Base {\n";
    cout << "       friend class Friend;\n";
    cout << "   };\n";
    cout << "   class Derived : public Base { };\n";
    cout << "   // Friend CANNOT access Derived's private members\n";
    cout << "\n";
    cout << "3. NOT TRANSITIVE:\n";
    cout << "   class A { friend class B; };\n";
    cout << "   class B { friend class C; };\n";
    cout << "   // C is NOT friend of A\n";
    cout << "\n";
    cout << "Summary: Friendship must be EXPLICITLY granted!\n";

    cout << "\n========================================\n";
    cout << "WHEN TO USE FRIEND\n";
    cout << "========================================\n";
    cout << "✅ GOOD USES:\n";
    cout << "1. Operator overloading:\n";
    cout << "   friend ostream& operator<<(ostream&, const MyClass&);\n";
    cout << "   • Stream operators MUST be non-member\n";
    cout << "   • Symmetric operators (2 * obj and obj * 2)\n";
    cout << "\n";
    cout << "2. Tightly coupled classes:\n";
    cout << "   • Engine and Car\n";
    cout << "   • Node and LinkedList\n";
    cout << "   • Iterator and Container\n";
    cout << "\n";
    cout << "3. Performance:\n";
    cout << "   • Direct access faster than getters/setters\n";
    cout << "   • Performance-critical code\n";
    cout << "\n";
    cout << "4. Testing:\n";
    cout << "   • Unit testing internal state\n";
    cout << "   • Debugging\n";
    cout << "   • Verification\n";
    cout << "\n";
    cout << "❌ BAD USES:\n";
    cout << "• When public interface is sufficient\n";
    cout << "• Breaking encapsulation without good reason\n";
    cout << "• Making everything friend (defeats purpose)\n";
    cout << "• Lazy design (friend instead of proper interface)\n";

    cout << "\n========================================\n";
    cout << "OPERATOR OVERLOADING WITH FRIEND\n";
    cout << "========================================\n";
    cout << "Some operators MUST be friend:\n";
    cout << "\n";
    cout << "STREAM OPERATORS:\n";
    cout << "friend ostream& operator<<(ostream& os, const MyClass& obj) {\n";
    cout << "    os << obj.privateData;\n";
    cout << "    return os;\n";
    cout << "}\n";
    cout << "Usage: cout << obj;  // Natural syntax\n";
    cout << "\n";
    cout << "SYMMETRIC OPERATORS:\n";
    cout << "class Vector {\n";
    cout << "    // Member: v * 2\n";
    cout << "    Vector operator*(double s) const;\n";
    cout << "    \n";
    cout << "    // Friend: 2 * v (symmetry!)\n";
    cout << "    friend Vector operator*(double s, const Vector& v);\n";
    cout << "};\n";
    cout << "\n";
    cout << "Why friend?\n";
    cout << "  • Left operand is not our class\n";
    cout << "  • Must be non-member\n";
    cout << "  • Needs access to private members\n";

    cout << "\n========================================\n";
    cout << "BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "✅ DO:\n";
    cout << "  • Use friend for stream operators\n";
    cout << "  • Use friend for symmetric operations\n";
    cout << "  • Document why friendship is needed\n";
    cout << "  • Keep friend count minimal\n";
    cout << "  • Use friend for tightly coupled classes\n";
    cout << "  • Declare friend in class definition\n";
    cout << "  • Consider friend for testing\n";
    cout << "\n";
    cout << "❌ DON'T:\n";
    cout << "  • Overuse friend (breaks encapsulation)\n";
    cout << "  • Make everything friend\n";
    cout << "  • Use friend instead of public interface\n";
    cout << "  • Repeat 'friend' in function definition\n";
    cout << "  • Assume friendship is mutual\n";
    cout << "  • Assume friendship is inherited\n";
    cout << "  • Use friend as lazy design\n";

    cout << "\n========================================\n";
    cout << "COMMON MISTAKES\n";
    cout << "========================================\n";
    cout << "❌ Mistake 1: Repeating 'friend' in definition\n";
    cout << "class MyClass {\n";
    cout << "    friend void func(const MyClass&);\n";
    cout << "};\n";
    cout << "// ❌ Wrong:\n";
    cout << "friend void func(const MyClass& obj) { }\n";
    cout << "// ✅ Correct:\n";
    cout << "void func(const MyClass& obj) { }\n";
    cout << "\n";
    cout << "❌ Mistake 2: Assuming mutual friendship\n";
    cout << "class A { friend class B; };\n";
    cout << "class B {\n";
    cout << "    void func(A& a) {\n";
    cout << "        // Can access A's private ✅\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "// But A cannot access B's private! ❌\n";
    cout << "\n";
    cout << "❌ Mistake 3: Wrong order with friend member\n";
    cout << "// Need forward declaration:\n";
    cout << "class B;  // Forward declare\n";
    cout << "class A {\n";
    cout << "    friend void B::func(A&);  // ✅\n";
    cout << "};\n";
    cout << "class B {\n";
    cout << "    void func(A&);\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "QUICK REFERENCE\n";
    cout << "========================================\n";
    cout << "// FRIEND FUNCTION\n";
    cout << "class MyClass {\n";
    cout << "    int data;\n";
    cout << "    friend void func(const MyClass&);\n";
    cout << "};\n";
    cout << "void func(const MyClass& obj) {\n";
    cout << "    cout << obj.data;  // Can access private\n";
    cout << "}\n";
    cout << "\n";
    cout << "// FRIEND CLASS\n";
    cout << "class MyClass {\n";
    cout << "    int data;\n";
    cout << "    friend class FriendClass;\n";
    cout << "};\n";
    cout << "\n";
    cout << "// FRIEND OPERATOR\n";
    cout << "class MyClass {\n";
    cout << "    int data;\n";
    cout << "    friend ostream& operator<<(ostream&, const MyClass&);\n";
    cout << "};\n";
    cout << "ostream& operator<<(ostream& os, const MyClass& obj) {\n";
    cout << "    return os << obj.data;\n";
    cout << "}\n";

    cout << "\n========================================\n";
    cout << "KEY TAKEAWAYS\n";
    cout << "========================================\n";
    cout << "1. Friend functions can access private members\n";
    cout << "2. Not member functions (no 'this' pointer)\n";
    cout << "3. Declared with 'friend' keyword in class\n";
    cout << "4. Friendship is NOT mutual\n";
    cout << "5. Friendship is NOT inherited\n";
    cout << "6. Friendship is NOT transitive\n";
    cout << "7. Essential for stream operators (<<, >>)\n";
    cout << "8. Useful for symmetric operations\n";
    cout << "9. Use sparingly (breaks encapsulation)\n";
    cout << "10. Must be explicitly granted\n";

    return 0;
}
