#include <iostream>   // only for the stream-operator examples (cout / ostream / istream)
#include <print>
#include <string>
#include <vector>
#include <cmath>
#include <cstring>
using namespace std;

// ============================================
// WHAT IS OPERATOR OVERLOADING?
// ============================================

/*
 * OPERATOR OVERLOADING:
 * - Gives special meaning to operators for user-defined types
 * - Makes code more intuitive and readable
 * - Allows objects to behave like built-in types
 * - Syntax: returnType operator@(parameters)
 *
 * CATEGORIES:
 * - Arithmetic: +, -, *, /, %
 * - Comparison: ==, !=, <, >, <=, >=
 * - Assignment: =, +=, -=, *=, /=
 * - Increment/Decrement: ++, --
 * - Stream: <<, >>
 * - Subscript: []
 * - Function call: ()
 * - Others: ->, *, &, etc.
 *
 * MEMBER vs NON-MEMBER:
 * - Member: obj.operator@(args)
 * - Non-member: operator@(obj, args)
 * - Some operators MUST be members (=, [], (), ->)
 */

// ============================================
// EXAMPLE 1: BASIC ARITHMETIC OPERATORS
// ============================================

class Fraction {
private:
    int numerator;
    int denominator;

    // Helper function to find GCD
    int gcd(int a, int b) const {
        return b == 0 ? a : gcd(b, a % b);
    }

    // Simplify fraction
    void simplify() {
        if (denominator == 0) {
            std::println("Error: Division by zero!");
            return;
        }
        int g = gcd(abs(numerator), abs(denominator));
        numerator /= g;
        denominator /= g;
        if (denominator < 0) {  // Keep denominator positive
            numerator = -numerator;
            denominator = -denominator;
        }
    }

public:
    Fraction(int num = 0, int den = 1) : numerator(num), denominator(den) {
        simplify();
    }

    // Operator + (addition)
    Fraction operator+(const Fraction& other) const {
        int num = numerator * other.denominator + other.numerator * denominator;
        int den = denominator * other.denominator;
        return Fraction(num, den);
    }

    // Operator - (subtraction)
    Fraction operator-(const Fraction& other) const {
        int num = numerator * other.denominator - other.numerator * denominator;
        int den = denominator * other.denominator;
        return Fraction(num, den);
    }

    // Operator * (multiplication)
    Fraction operator*(const Fraction& other) const {
        return Fraction(numerator * other.numerator,
                        denominator * other.denominator);
    }

    // Operator / (division)
    Fraction operator/(const Fraction& other) const {
        return Fraction(numerator * other.denominator,
                        denominator * other.numerator);
    }

    void display() const {
        if (denominator == 1) {
            std::print("{}", numerator);
        } else {
            std::print("{}/{}", numerator, denominator);
        }
    }
};

void demonstrateArithmeticOperators() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: ARITHMETIC OPERATORS   ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Fraction f1(1, 2);  // 1/2
    Fraction f2(1, 3);  // 1/3

    std::print("f1 = ");
    f1.display();
    std::println("");
    std::print("f2 = ");
    f2.display();
    std::println("");
    std::println("");

    std::print("f1 + f2 = ");
    Fraction f3 = f1 + f2;  // Uses operator+
    f3.display();
    std::println("");

    std::print("f1 - f2 = ");
    Fraction f4 = f1 - f2;
    f4.display();
    std::println("");

    std::print("f1 * f2 = ");
    Fraction f5 = f1 * f2;
    f5.display();
    std::println("");

    std::print("f1 / f2 = ");
    Fraction f6 = f1 / f2;
    f6.display();
    std::println("");

    std::println("");
    std::println("✅ Operators make code natural: f1 + f2 instead of f1.add(f2)");
}

// ============================================
// EXAMPLE 2: COMPARISON OPERATORS
// ============================================

class Point {
private:
    double x, y;

public:
    Point(double xVal = 0, double yVal = 0) : x(xVal), y(yVal) { }

    // Operator == (equality)
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    // Operator != (inequality)
    bool operator!=(const Point& other) const {
        return !(*this == other);  // Reuse operator==
    }

    // Operator < (less than) - compare by distance from origin
    bool operator<(const Point& other) const {
        double dist1 = x * x + y * y;
        double dist2 = other.x * other.x + other.y * other.y;
        return dist1 < dist2;
    }

    // Operator > (greater than)
    bool operator>(const Point& other) const {
        return other < *this;  // Reuse operator
    }

    // Operator <= (less than or equal)
    bool operator<=(const Point& other) const {
        return !(*this > other);
    }

    // Operator >= (greater than or equal)
    bool operator>=(const Point& other) const {
        return !(*this < other);
    }

    void display() const {
        std::print("({}, {})", x, y);
    }
};

void demonstrateComparisonOperators() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: COMPARISON OPERATORS   ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Point p1(3, 4);
    Point p2(3, 4);
    Point p3(5, 5);

    std::print("p1 = ");
    p1.display();
    std::println("");
    std::print("p2 = ");
    p2.display();
    std::println("");
    std::print("p3 = ");
    p3.display();
    std::println("");
    std::println("");

    std::println("p1 == p2: {}", (p1 == p2 ? "true" : "false"));
    std::println("p1 != p3: {}", (p1 != p3 ? "true" : "false"));
    std::println("p1 < p3:  {} (by distance from origin)", (p1 < p3 ? "true" : "false"));
    std::println("p1 > p3:  {}", (p1 > p3 ? "true" : "false"));

    std::println("");
    std::println("✅ Can use objects in conditions: if (p1 == p2)");
}

// ============================================
// EXAMPLE 3: ASSIGNMENT OPERATORS
// ============================================

class Counter {
private:
    int value;

public:
    Counter(int v = 0) : value(v) { }

    // Copy assignment operator (already covered)
    Counter& operator=(const Counter& other) {
        if (this != &other) {
            value = other.value;
        }
        return *this;
    }

    // Operator += (add and assign)
    Counter& operator+=(int val) {
        value += val;
        return *this;
    }

    Counter& operator+=(const Counter& other) {
        value += other.value;
        return *this;
    }

    // Operator -= (subtract and assign)
    Counter& operator-=(int val) {
        value -= val;
        return *this;
    }

    // Operator *= (multiply and assign)
    Counter& operator*=(int val) {
        value *= val;
        return *this;
    }

    // Operator /= (divide and assign)
    Counter& operator/=(int val) {
        if (val != 0) {
            value /= val;
        }
        return *this;
    }

    int getValue() const { return value; }

    void display() const {
        std::print("Counter: {}", value);
    }
};

void demonstrateAssignmentOperators() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: ASSIGNMENT OPERATORS   ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Counter c1(10);

    std::print("Initial: ");
    c1.display();
    std::println("");

    std::print("After c1 += 5: ");
    c1 += 5;
    c1.display();
    std::println("");

    std::print("After c1 -= 3: ");
    c1 -= 3;
    c1.display();
    std::println("");

    std::print("After c1 *= 2: ");
    c1 *= 2;
    c1.display();
    std::println("");

    std::print("After c1 /= 4: ");
    c1 /= 4;
    c1.display();
    std::println("");

    std::println("");
    std::println("✅ Compound assignment: c1 += 5 instead of c1 = c1 + 5");
}

// ============================================
// EXAMPLE 4: INCREMENT/DECREMENT OPERATORS
// ============================================

class Iterator {
private:
    int position;

public:
    Iterator(int pos = 0) : position(pos) { }

    // Prefix increment: ++obj
    Iterator& operator++() {
        ++position;
        std::println("  Prefix ++: position now {}", position);
        return *this;
    }

    // Postfix increment: obj++
    Iterator operator++(int) {  // int parameter differentiates postfix
        Iterator temp = *this;
        position++;
        std::println("  Postfix ++: position now {}", position);
        return temp;  // Return old value
    }

    // Prefix decrement: --obj
    Iterator& operator--() {
        --position;
        std::println("  Prefix --: position now {}", position);
        return *this;
    }

    // Postfix decrement: obj--
    Iterator operator--(int) {
        Iterator temp = *this;
        position--;
        std::println("  Postfix --: position now {}", position);
        return temp;
    }

    int getPosition() const { return position; }
};

void demonstrateIncrementDecrement() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: INCREMENT/DECREMENT    ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Iterator it(5);

    std::println("Initial position: {}", it.getPosition());
    std::println("");

    std::println("Prefix increment (++it):");
    ++it;
    std::println("Result: {}", it.getPosition());
    std::println("");

    std::println("Postfix increment (it++):");
    Iterator old = it++;
    std::println("Returned (old): {}", old.getPosition());
    std::println("Current: {}", it.getPosition());
    std::println("");

    std::println("Prefix decrement (--it):");
    --it;
    std::println("Result: {}", it.getPosition());
    std::println("");
}

// ============================================
// EXAMPLE 5: STREAM OPERATORS (<<, >>)
// ============================================

class Complex {
private:
    double real;
    double imag;

public:
    Complex(double r = 0, double i = 0) : real(r), imag(i) { }

    // Friend functions for stream operators
    // Must be non-member to allow: cout << obj

    friend ostream& operator<<(ostream& os, const Complex& c);
    friend istream& operator>>(istream& is, Complex& c);

    // Arithmetic for completeness
    Complex operator+(const Complex& other) const {
        return Complex(real + other.real, imag + other.imag);
    }
};

// Output operator (insertion)
ostream& operator<<(ostream& os, const Complex& c) {
    os << c.real;
    if (c.imag >= 0) {
        os << " + " << c.imag << "i";
    } else {
        os << " - " << -c.imag << "i";
    }
    return os;
}

// Input operator (extraction)
istream& operator>>(istream& is, Complex& c) {
    // Prompt + cin input: stay with the stream classes here
    cout << "Enter real part: ";
    is >> c.real;
    cout << "Enter imaginary part: ";
    is >> c.imag;
    return is;
}

void demonstrateStreamOperators() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 5: STREAM OPERATORS       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Complex c1(3.5, 2.0);
    Complex c2(1.0, -4.5);

    std::println("Using operator<< for output:");
    // These lines keep cout: they use our own operator<< (the topic of this example)
    cout << "c1 = " << c1 << "\n";
    cout << "c2 = " << c2 << "\n";

    Complex c3 = c1 + c2;
    cout << "c1 + c2 = " << c3 << "\n";

    // Skipping actual cin input for demo
    std::println("");
    std::println("(Input operator>> demo skipped for automation)");
}

// ============================================
// EXAMPLE 6: SUBSCRIPT OPERATOR ([])
// ============================================

class Array {
private:
    int* data;
    int size;

public:
    Array(int s) : size(s) {
        data = new int[size];
        for (int i = 0; i < size; i++) {
            data[i] = 0;
        }
    }

    ~Array() {
        delete[] data;
    }

    // Copy constructor
    Array(const Array& other) : size(other.size) {
        data = new int[size];
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }

    // Subscript operator for read/write
    int& operator[](int index) {
        if (index < 0 || index >= size) {
            std::println("Index out of range!");
            return data[0];  // Return first element
        }
        return data[index];
    }

    // Const version for read-only
    const int& operator[](int index) const {
        if (index < 0 || index >= size) {
            std::println("Index out of range!");
            return data[0];
        }
        return data[index];
    }

    int getSize() const { return size; }
};

void demonstrateSubscriptOperator() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 6: SUBSCRIPT OPERATOR     ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Array arr(5);

    std::println("Setting values using []:");
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;
    arr[3] = 40;
    arr[4] = 50;

    std::println("Reading values using []:");
    for (int i = 0; i < arr.getSize(); i++) {
        std::println("arr[{}] = {}", i, arr[i]);
    }

    std::println("");
    std::println("✅ Array-like access: arr[i] instead of arr.get(i)");

    std::println("");
    std::println("Trying invalid index:");
    int value = arr[10];  // Out of range
}

// ============================================
// EXAMPLE 7: FUNCTION CALL OPERATOR (())
// ============================================

class Multiplier {
private:
    int factor;

public:
    Multiplier(int f) : factor(f) { }

    // Function call operator - makes object callable
    int operator()(int value) const {
        return value * factor;
    }

    // Overload with different parameters
    int operator()(int a, int b) const {
        return (a + b) * factor;
    }
};

void demonstrateFunctionCallOperator() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 7: FUNCTION CALL OPERATOR ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Multiplier times2(2);
    Multiplier times10(10);

    std::println("times2(5) = {}", times2(5));
    std::println("times10(3) = {}", times10(3));
    std::println("times2(3, 4) = {}", times2(3, 4));

    std::println("");
    std::println("✅ Objects can be used like functions!");
}

// ============================================
// EXAMPLE 8: MEMBER vs NON-MEMBER OPERATORS
// ============================================

class Vector2D {
private:
    double x, y;

public:
    Vector2D(double xVal = 0, double yVal = 0) : x(xVal), y(yVal) { }

    // MEMBER operator (this is left operand)
    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y);
    }

    // Member operator for scalar multiplication
    Vector2D operator*(double scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }

    void display() const {
        std::print("({}, {})", x, y);
    }

    // Friend for non-member operator
    friend Vector2D operator*(double scalar, const Vector2D& v);
};

// NON-MEMBER operator (for scalar * vector)
Vector2D operator*(double scalar, const Vector2D& v) {
    return v * scalar;  // Reuse member operator
}

void demonstrateMemberVsNonMember() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 8: MEMBER vs NON-MEMBER   ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Vector2D v1(3, 4);
    Vector2D v2(1, 2);

    std::print("v1 = ");
    v1.display();
    std::println("");
    std::print("v2 = ");
    v2.display();
    std::println("");
    std::println("");

    std::println("Member operator (v1 + v2):");
    Vector2D v3 = v1 + v2;
    std::print("Result: ");
    v3.display();
    std::println("");
    std::println("");

    std::println("Member operator (v1 * 2):");
    Vector2D v4 = v1 * 2.0;
    std::print("Result: ");
    v4.display();
    std::println("");
    std::println("");

    std::println("Non-member operator (2 * v1):");
    Vector2D v5 = 2.0 * v1;
    std::print("Result: ");
    v5.display();
    std::println("");
    std::println("");
}

// ============================================
// EXAMPLE 9: REAL-WORLD - MATRIX CLASS
// ============================================

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

    // Copy constructor
    Matrix(const Matrix& other) : rows(other.rows), cols(other.cols) {
        data = new double*[rows];
        for (int i = 0; i < rows; i++) {
            data[i] = new double[cols];
            for (int j = 0; j < cols; j++) {
                data[i][j] = other.data[i][j];
            }
        }
    }

    // Assignment operator
    Matrix& operator=(const Matrix& other) {
        if (this != &other) {
            // Delete old data
            for (int i = 0; i < rows; i++) {
                delete[] data[i];
            }
            delete[] data;

            // Copy new data
            rows = other.rows;
            cols = other.cols;
            data = new double*[rows];
            for (int i = 0; i < rows; i++) {
                data[i] = new double[cols];
                for (int j = 0; j < cols; j++) {
                    data[i][j] = other.data[i][j];
                }
            }
        }
        return *this;
    }

    // Subscript operator for row access
    double* operator[](int row) {
        return data[row];
    }

    const double* operator[](int row) const {
        return data[row];
    }

    // Addition
    Matrix operator+(const Matrix& other) const {
        Matrix result(rows, cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                result.data[i][j] = data[i][j] + other.data[i][j];
            }
        }
        return result;
    }

    // Scalar multiplication
    Matrix operator*(double scalar) const {
        Matrix result(rows, cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                result.data[i][j] = data[i][j] * scalar;
            }
        }
        return result;
    }

    void display() const {
        std::println("Matrix {}×{}:", rows, cols);
        for (int i = 0; i < rows; i++) {
            std::print("  [");
            for (int j = 0; j < cols; j++) {
                std::print("{}", data[i][j]);
                if (j < cols - 1) std::print(", ");
            }
            std::println("]");
        }
    }
};

void demonstrateMatrix() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 9: MATRIX CLASS           ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Matrix m1(2, 2);
    m1[0][0] = 1; m1[0][1] = 2;
    m1[1][0] = 3; m1[1][1] = 4;

    Matrix m2(2, 2);
    m2[0][0] = 5; m2[0][1] = 6;
    m2[1][0] = 7; m2[1][1] = 8;

    std::println("Matrix 1:");
    m1.display();

    std::println("");
    std::println("Matrix 2:");
    m2.display();

    std::println("");
    std::println("m1 + m2:");
    Matrix m3 = m1 + m2;
    m3.display();

    std::println("");
    std::println("m1 * 2:");
    Matrix m4 = m1 * 2.0;
    m4.display();

    std::println("");
    std::println("✅ Multiple operators working together!");
}

// ============================================
// EXAMPLE 10: REAL-WORLD - STRING CLASS
// ============================================

class String {
private:
    char* buffer;
    size_t length;

public:
    // Constructor
    String(const char* str = "") {
        length = strlen(str);
        buffer = new char[length + 1];
        strcpy(buffer, str);
    }

    // Destructor
    ~String() {
        delete[] buffer;
    }

    // Copy constructor
    String(const String& other) : length(other.length) {
        buffer = new char[length + 1];
        strcpy(buffer, other.buffer);
    }

    // Assignment operator
    String& operator=(const String& other) {
        if (this != &other) {
            delete[] buffer;
            length = other.length;
            buffer = new char[length + 1];
            strcpy(buffer, other.buffer);
        }
        return *this;
    }

    // Concatenation operator
    String operator+(const String& other) const {
        String result;
        result.length = length + other.length;
        delete[] result.buffer;
        result.buffer = new char[result.length + 1];
        strcpy(result.buffer, buffer);
        strcat(result.buffer, other.buffer);
        return result;
    }

    // += operator
    String& operator+=(const String& other) {
        *this = *this + other;
        return *this;
    }

    // Equality operator
    bool operator==(const String& other) const {
        return strcmp(buffer, other.buffer) == 0;
    }

    // Inequality operator
    bool operator!=(const String& other) const {
        return !(*this == other);
    }

    // Subscript operator
    char& operator[](size_t index) {
        return buffer[index];
    }

    const char& operator[](size_t index) const {
        return buffer[index];
    }

    // Stream output
    friend ostream& operator<<(ostream& os, const String& str) {
        os << str.buffer;
        return os;
    }

    size_t size() const { return length; }
};

void demonstrateString() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 10: STRING CLASS          ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    String s1("Hello");
    String s2(" World");
    String s3("Hello");

    // These lines keep cout: they use our own operator<< for String
    cout << "s1 = " << s1 << "\n";
    cout << "s2 = " << s2 << "\n";
    cout << "s3 = " << s3 << "\n\n";

    cout << "s1 + s2 = " << (s1 + s2) << "\n";

    std::println("");
    std::println("s1 == s3: {}", (s1 == s3 ? "true" : "false"));
    std::println("s1 != s2: {}", (s1 != s2 ? "true" : "false"));

    std::println("");
    std::println("s1[0] = {}", s1[0]);

    String s4 = s1;
    s4 += s2;
    cout << "\nAfter s4 += s2: " << s4 << "\n";

    std::println("");
    std::println("✅ Complete string functionality through operators!");
}

// ============================================
// MAIN DEMONSTRATION
// ============================================

int main() {
    std::println("========================================");
    std::println("OPERATOR OVERLOADING DEMONSTRATION");
    std::println("========================================");

    demonstrateArithmeticOperators();
    demonstrateComparisonOperators();
    demonstrateAssignmentOperators();
    demonstrateIncrementDecrement();
    demonstrateStreamOperators();
    demonstrateSubscriptOperator();
    demonstrateFunctionCallOperator();
    demonstrateMemberVsNonMember();
    demonstrateMatrix();
    demonstrateString();

    return 0;
}
