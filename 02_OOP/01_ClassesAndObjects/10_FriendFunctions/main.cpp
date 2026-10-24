#include <print>
#include <string>
#include <cmath>
#include <vector>
#include <iostream>   // only for the stream operators in Example 3
#include <sstream>
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
 * 5. (Not for speed: trivial getters are inlined by the compiler anyway)
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
        std::println("  Box created: {:g}×{:g}×{:g}", width, height, depth);
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
    std::println("  Box: {:g} × {:g} × {:g}", box.width, box.height, box.depth);
}

void demonstrateBasicFriend() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: BASIC FRIEND FUNCTION  ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Box box(10.0, 5.0, 3.0);

    std::println("");
    std::println("--- Using friend functions ---");
    displayBox(box);
    std::println("  Volume: {:g}", calculateVolume(box));
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
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: FRIEND vs MEMBER       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Point p1(3.0, 4.0);
    Point p2(6.0, 8.0);

    std::println("--- Member function (needs one object) ---");
    std::println("  p1.distanceFromOrigin(): {:g}", p1.distanceFromOrigin());

    std::println("");
    std::println("--- Friend function (takes two objects) ---");
    std::println("  distanceBetween(p1, p2): {:g}", distanceBetween(p1, p2));
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

    // Stream operators MUST be non-member (the left operand is the stream).
    // They are friends here only because they read the private data;
    // with public getters they would not need friendship.
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

// Friend function for input (reads "real imag"; an input operator should not print prompts)
istream& operator>>(istream& is, Complex& c) {
    is >> c.real >> c.imag;
    return is;
}

void demonstrateFriendOperators() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: FRIEND OPERATORS       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Complex c1(3.0, 4.0);
    Complex c2(1.0, 2.0);

    std::println("--- Complex numbers ---");
    std::cout << "  c1 = " << c1 << "\n";
    std::cout << "  c2 = " << c2 << "\n";

    std::println("");
    std::println("--- Operations ---");
    std::cout << "  c1 + c2 = " << (c1 + c2) << "\n";
    std::cout << "  2.0 * c1 = " << (2.0 * c1) << "\n";

    std::println("");
    std::println("--- Reading with the friend operator>> ---");
    istringstream input("5 -6");
    Complex c3;
    input >> c3;
    std::cout << "  read \"5 -6\": c3 = " << c3 << "\n";
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
        std::println("  Engine: {}hp, {:g}L/100km", horsepower, fuelEfficiency);
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
        std::println("  Car created: {}", model);
    }

    void displayDetails() {
        std::println("  Model: {}", model);
        // Can access Engine's private members!
        std::println("  Engine: {}hp", engine.horsepower);
        std::println("  Efficiency: {:g}L/100km", engine.fuelEfficiency);
    }

    void tuneEngine() {
        // Can modify Engine's private members!
        engine.horsepower += 50;
        engine.fuelEfficiency += 0.5;
        std::println("  Engine tuned! New power: {}hp", engine.horsepower);
    }
};

void demonstrateFriendClass() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: FRIEND CLASS           ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating car ---");
    Car car("BMW M3", 500, 12.5);

    std::println("");
    std::println("--- Car details ---");
    car.displayDetails();

    std::println("");
    std::println("--- Tuning engine ---");
    car.tuneEngine();

    std::println("");
    std::println("--- Updated details ---");
    car.displayDetails();
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
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 5: FRIEND MEMBER FUNCTION ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Rectangle rect(5.0, 4.0);
    Triangle tri(8.0, 6.0);

    std::println("--- Shapes ---");
    std::println("  Rectangle area: {}", rect.area());
    std::println("  Triangle area: {}", tri.area());

    std::println("");
    std::println("--- Comparison ---");
    if (tri.isLargerThan(rect)) {
        std::println("  Triangle is larger than rectangle");
    } else {
        std::println("  Rectangle is larger than triangle");
    }
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
        std::println("  ClassB accessing ClassA::secretA = {} ✅", a.secretA);
    }

    int getSecretB() const { return secretB; }
};

void ClassA::tryAccessB(const ClassB& b) {
    // ClassA is NOT friend of ClassB
    // So ClassA CANNOT access ClassB's private members
    // std::println("{}", b.secretB);  // ❌ Error! Cannot access
    std::println("  ClassA cannot access ClassB::secretB directly ❌");
    std::println("  ClassA can only use public methods: secretB = {}", b.getSecretB());
}

void demonstrateFriendshipNotMutual() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 6: NOT MUTUAL             ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    ClassA a(100);
    ClassB b(200);

    std::println("--- ClassB accessing ClassA ---");
    b.accessA(a);

    std::println("");
    std::println("--- ClassA trying to access ClassB ---");
    a.tryAccessB(b);
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
        std::println("  Beta accessing Alpha::alphaSecret = {} ✅", a.alphaSecret);
    }
};

class Gamma {
public:
    void accessBeta(const Beta& b) {
        std::println("  Gamma accessing Beta::betaSecret = {} ✅", b.betaSecret);
    }

    void tryAccessAlpha(const Alpha& /*a*/) {
        // Gamma is friend of Beta
        // Beta is friend of Alpha
        // But Gamma is NOT friend of Alpha!
        // std::println("{}", a.alphaSecret);  // ❌ Error!
        std::println("  Gamma cannot access Alpha::alphaSecret ❌");
        std::println("  (Friendship is not transitive)");
    }
};

void demonstrateFriendshipNotTransitive() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 7: NOT TRANSITIVE         ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Alpha alpha(100);
    Beta beta(200);
    Gamma gamma;

    std::println("--- Beta accessing Alpha (friend) ---");
    beta.accessAlpha(alpha);

    std::println("");
    std::println("--- Gamma accessing Beta (friend) ---");
    gamma.accessBeta(beta);

    std::println("");
    std::println("--- Gamma trying to access Alpha (not friend) ---");
    gamma.tryAccessAlpha(alpha);
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

    // Owns raw memory: copying is not needed here, so forbid it (Rule of Three)
    Matrix(const Matrix&) = delete;
    Matrix& operator=(const Matrix&) = delete;

    void set(int i, int j, double val) {
        data[i][j] = val;
    }

    void display() const {
        std::println("  Matrix {}×{}:", rows, cols);
        for (int i = 0; i < rows; i++) {
            std::print("  [");
            for (int j = 0; j < cols; j++) {
                std::print("{}", data[i][j]);
                if (j < cols - 1) std::print(", ");
            }
            std::println("]");
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

    // operator* returns a Vector BY VALUE, so Vector needs a correct copy constructor.
    // Without one the pointer would be copied and deleted twice whenever the compiler
    // does not elide the copy (the optimization is allowed, not guaranteed).
    Vector(const Vector& other) : size(other.size), data(new double[other.size]) {
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }
    Vector& operator=(const Vector&) = delete;

    void set(int i, double val) {
        data[i] = val;
    }

    double get(int i) const {
        return data[i];
    }

    void display() const {
        std::print("  Vector [");
        for (int i = 0; i < size; i++) {
            std::print("{}", data[i]);
            if (i < size - 1) std::print(", ");
        }
        std::println("]");
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
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 8: MATRIX × VECTOR        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Matrix m(2, 2);
    m.set(0, 0, 1); m.set(0, 1, 2);
    m.set(1, 0, 3); m.set(1, 1, 4);

    Vector v(2);
    v.set(0, 5);
    v.set(1, 6);

    std::println("--- Matrix ---");
    m.display();

    std::println("");
    std::println("--- Vector ---");
    v.display();

    std::println("");
    std::println("--- Matrix × Vector ---");
    Vector result = m * v;
    result.display();
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

    // The same operation with the public interface only: no friend needed
    static void transferViaMethods(Account& from, Account& to, double amount) {
        if (from.getBalance() >= amount) {
            from.deposit(-amount);
            to.deposit(amount);
        }
    }
};

// Friend function - direct access
void transferFunds(Account& from, Account& to, double amount) {
    if (from.balance >= amount) {
        from.balance -= amount;
        to.balance += amount;
        std::println("  Transferred ${:.2f} (via friend)", amount);
    }
}

void demonstrateWhenToUseFriend() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 9: WHEN TO USE FRIEND     ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Account acc1(1000);
    Account acc2(500);

    std::println("--- Initial balances ---");
    std::println("  Account 1: ${:.2f}", acc1.getBalance());
    std::println("  Account 2: ${:.2f}", acc2.getBalance());

    std::println("");
    std::println("--- Transfer using friend function ---");
    transferFunds(acc1, acc2, 200);
    std::println("  Account 1: ${:.2f}", acc1.getBalance());
    std::println("  Account 2: ${:.2f}", acc2.getBalance());

    std::println("");
    std::println("--- Same transfer back, public interface only ---");
    Account::transferViaMethods(acc2, acc1, 200);
    std::println("  Account 1: ${:.2f}", acc1.getBalance());
    std::println("  Account 2: ${:.2f}", acc2.getBalance());
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
        std::println("  Running internal tests...");

        // Can access private members for testing!
        std::println("    Internal capacity: {}", stack.capacity);
        std::println("    Current top index: {}", stack.top);

        // Verify internal state
        if (stack.top == -1) {
            std::println("    ✅ Stack properly initialized (top = -1)");
        }

        // Check data array
        if (stack.data != nullptr) {
            std::println("    ✅ Data array allocated");
        }
    }

    static void verifyIntegrity(const Stack& stack) {
        bool valid = true;

        if (stack.top >= stack.capacity) {
            std::println("    ❌ Error: top >= capacity");
            valid = false;
        }

        if (stack.top < -1) {
            std::println("    ❌ Error: top < -1");
            valid = false;
        }

        if (valid) {
            std::println("    ✅ Stack integrity verified");
        }
    }
};

void demonstrateTestingWithFriend() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 10: TESTING WITH FRIEND   ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Stack stack(10);

    std::println("--- Initial tests ---");
    StackTester::runTests(stack);
    StackTester::verifyIntegrity(stack);

    std::println("");
    std::println("--- After operations ---");
    stack.push(1);
    stack.push(2);
    stack.push(3);
    StackTester::runTests(stack);
    StackTester::verifyIntegrity(stack);
}

// ============================================
// MAIN DEMONSTRATION
// ============================================

int main() {
    std::println("========================================");
    std::println("FRIEND FUNCTIONS DEMONSTRATION");
    std::println("========================================");

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

    return 0;
}
