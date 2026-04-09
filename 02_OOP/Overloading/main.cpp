#include <iostream>
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
            cout << "Error: Division by zero!\n";
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
            cout << numerator;
        } else {
            cout << numerator << "/" << denominator;
        }
    }
};

void demonstrateArithmeticOperators() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: ARITHMETIC OPERATORS   ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Fraction f1(1, 2);  // 1/2
    Fraction f2(1, 3);  // 1/3

    cout << "f1 = "; f1.display(); cout << "\n";
    cout << "f2 = "; f2.display(); cout << "\n\n";

    cout << "f1 + f2 = ";
    Fraction f3 = f1 + f2;  // Uses operator+
    f3.display();
    cout << "\n";

    cout << "f1 - f2 = ";
    Fraction f4 = f1 - f2;
    f4.display();
    cout << "\n";

    cout << "f1 * f2 = ";
    Fraction f5 = f1 * f2;
    f5.display();
    cout << "\n";

    cout << "f1 / f2 = ";
    Fraction f6 = f1 / f2;
    f6.display();
    cout << "\n";

    cout << "\n✅ Operators make code natural: f1 + f2 instead of f1.add(f2)\n";
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
        cout << "(" << x << ", " << y << ")";
    }
};

void demonstrateComparisonOperators() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: COMPARISON OPERATORS   ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Point p1(3, 4);
    Point p2(3, 4);
    Point p3(5, 5);

    cout << "p1 = "; p1.display(); cout << "\n";
    cout << "p2 = "; p2.display(); cout << "\n";
    cout << "p3 = "; p3.display(); cout << "\n\n";

    cout << "p1 == p2: " << (p1 == p2 ? "true" : "false") << "\n";
    cout << "p1 != p3: " << (p1 != p3 ? "true" : "false") << "\n";
    cout << "p1 < p3:  " << (p1 < p3 ? "true" : "false") << " (by distance from origin)\n";
    cout << "p1 > p3:  " << (p1 > p3 ? "true" : "false") << "\n";

    cout << "\n✅ Can use objects in conditions: if (p1 == p2)\n";
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
        cout << "Counter: " << value;
    }
};

void demonstrateAssignmentOperators() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: ASSIGNMENT OPERATORS   ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Counter c1(10);

    cout << "Initial: "; c1.display(); cout << "\n";

    cout << "After c1 += 5: ";
    c1 += 5;
    c1.display();
    cout << "\n";

    cout << "After c1 -= 3: ";
    c1 -= 3;
    c1.display();
    cout << "\n";

    cout << "After c1 *= 2: ";
    c1 *= 2;
    c1.display();
    cout << "\n";

    cout << "After c1 /= 4: ";
    c1 /= 4;
    c1.display();
    cout << "\n";

    cout << "\n✅ Compound assignment: c1 += 5 instead of c1 = c1 + 5\n";
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
        cout << "  Prefix ++: position now " << position << "\n";
        return *this;
    }

    // Postfix increment: obj++
    Iterator operator++(int) {  // int parameter differentiates postfix
        Iterator temp = *this;
        position++;
        cout << "  Postfix ++: position now " << position << "\n";
        return temp;  // Return old value
    }

    // Prefix decrement: --obj
    Iterator& operator--() {
        --position;
        cout << "  Prefix --: position now " << position << "\n";
        return *this;
    }

    // Postfix decrement: obj--
    Iterator operator--(int) {
        Iterator temp = *this;
        position--;
        cout << "  Postfix --: position now " << position << "\n";
        return temp;
    }

    int getPosition() const { return position; }
};

void demonstrateIncrementDecrement() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: INCREMENT/DECREMENT    ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Iterator it(5);

    cout << "Initial position: " << it.getPosition() << "\n\n";

    cout << "Prefix increment (++it):\n";
    ++it;
    cout << "Result: " << it.getPosition() << "\n\n";

    cout << "Postfix increment (it++):\n";
    Iterator old = it++;
    cout << "Returned (old): " << old.getPosition() << "\n";
    cout << "Current: " << it.getPosition() << "\n\n";

    cout << "Prefix decrement (--it):\n";
    --it;
    cout << "Result: " << it.getPosition() << "\n\n";

    cout << "💡 Prefix vs Postfix:\n";
    cout << "   Prefix (++it): Increment first, return new value\n";
    cout << "   Postfix (it++): Save old, increment, return old value\n";
    cout << "   Prefix is more efficient (no copy)\n";
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
    cout << "Enter real part: ";
    is >> c.real;
    cout << "Enter imaginary part: ";
    is >> c.imag;
    return is;
}

void demonstrateStreamOperators() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: STREAM OPERATORS       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Complex c1(3.5, 2.0);
    Complex c2(1.0, -4.5);

    cout << "Using operator<< for output:\n";
    cout << "c1 = " << c1 << "\n";
    cout << "c2 = " << c2 << "\n";

    Complex c3 = c1 + c2;
    cout << "c1 + c2 = " << c3 << "\n";

    cout << "\n✅ Stream operators allow natural I/O:\n";
    cout << "   cout << obj  instead of  obj.print()\n";
    cout << "   cin >> obj   instead of  obj.read()\n";

    // Skipping actual cin input for demo
    cout << "\n(Input operator>> demo skipped for automation)\n";
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
            cout << "Index out of range!\n";
            return data[0];  // Return first element
        }
        return data[index];
    }

    // Const version for read-only
    const int& operator[](int index) const {
        if (index < 0 || index >= size) {
            cout << "Index out of range!\n";
            return data[0];
        }
        return data[index];
    }

    int getSize() const { return size; }
};

void demonstrateSubscriptOperator() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: SUBSCRIPT OPERATOR     ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Array arr(5);

    cout << "Setting values using []:\n";
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;
    arr[3] = 40;
    arr[4] = 50;

    cout << "Reading values using []:\n";
    for (int i = 0; i < arr.getSize(); i++) {
        cout << "arr[" << i << "] = " << arr[i] << "\n";
    }

    cout << "\n✅ Array-like access: arr[i] instead of arr.get(i)\n";

    cout << "\nTrying invalid index:\n";
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
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 7: FUNCTION CALL OPERATOR ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Multiplier times2(2);
    Multiplier times10(10);

    cout << "times2(5) = " << times2(5) << "\n";
    cout << "times10(3) = " << times10(3) << "\n";
    cout << "times2(3, 4) = " << times2(3, 4) << "\n";

    cout << "\n✅ Objects can be used like functions!\n";
    cout << "   This creates 'functors' or 'function objects'\n";
    cout << "   Useful for algorithms, callbacks, etc.\n";
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
        cout << "(" << x << ", " << y << ")";
    }

    // Friend for non-member operator
    friend Vector2D operator*(double scalar, const Vector2D& v);
};

// NON-MEMBER operator (for scalar * vector)
Vector2D operator*(double scalar, const Vector2D& v) {
    return v * scalar;  // Reuse member operator
}

void demonstrateMemberVsNonMember() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 8: MEMBER vs NON-MEMBER   ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Vector2D v1(3, 4);
    Vector2D v2(1, 2);

    cout << "v1 = "; v1.display(); cout << "\n";
    cout << "v2 = "; v2.display(); cout << "\n\n";

    cout << "Member operator (v1 + v2):\n";
    Vector2D v3 = v1 + v2;
    cout << "Result: "; v3.display(); cout << "\n\n";

    cout << "Member operator (v1 * 2):\n";
    Vector2D v4 = v1 * 2.0;
    cout << "Result: "; v4.display(); cout << "\n\n";

    cout << "Non-member operator (2 * v1):\n";
    Vector2D v5 = 2.0 * v1;
    cout << "Result: "; v5.display(); cout << "\n\n";

    cout << "💡 Member vs Non-member:\n";
    cout << "   Member:     obj.operator@(args) → obj @ args\n";
    cout << "   Non-member: operator@(obj, args) → allows symmetry\n";
    cout << "   Use non-member for commutativity: 2*v and v*2\n";
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
        cout << "Matrix " << rows << "×" << cols << ":\n";
        for (int i = 0; i < rows; i++) {
            cout << "  [";
            for (int j = 0; j < cols; j++) {
                cout << data[i][j];
                if (j < cols - 1) cout << ", ";
            }
            cout << "]\n";
        }
    }
};

void demonstrateMatrix() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 9: MATRIX CLASS           ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Matrix m1(2, 2);
    m1[0][0] = 1; m1[0][1] = 2;
    m1[1][0] = 3; m1[1][1] = 4;

    Matrix m2(2, 2);
    m2[0][0] = 5; m2[0][1] = 6;
    m2[1][0] = 7; m2[1][1] = 8;

    cout << "Matrix 1:\n";
    m1.display();

    cout << "\nMatrix 2:\n";
    m2.display();

    cout << "\nm1 + m2:\n";
    Matrix m3 = m1 + m2;
    m3.display();

    cout << "\nm1 * 2:\n";
    Matrix m4 = m1 * 2.0;
    m4.display();

    cout << "\n✅ Multiple operators working together!\n";
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
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 10: STRING CLASS          ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    String s1("Hello");
    String s2(" World");
    String s3("Hello");

    cout << "s1 = " << s1 << "\n";
    cout << "s2 = " << s2 << "\n";
    cout << "s3 = " << s3 << "\n\n";

    cout << "s1 + s2 = " << (s1 + s2) << "\n";

    cout << "\ns1 == s3: " << (s1 == s3 ? "true" : "false") << "\n";
    cout << "s1 != s2: " << (s1 != s2 ? "true" : "false") << "\n";

    cout << "\ns1[0] = " << s1[0] << "\n";

    String s4 = s1;
    s4 += s2;
    cout << "\nAfter s4 += s2: " << s4 << "\n";

    cout << "\n✅ Complete string functionality through operators!\n";
}

// ============================================
// MAIN DEMONSTRATION
// ============================================

int main() {
    cout << "========================================\n";
    cout << "OPERATOR OVERLOADING DEMONSTRATION\n";
    cout << "========================================\n";

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

    // ============================================
    // Comprehensive Summary
    // ============================================
    cout << "\n========================================\n";
    cout << "WHAT IS OPERATOR OVERLOADING?\n";
    cout << "========================================\n";
    cout << "Operator overloading allows you to define custom behavior\n";
    cout << "for operators when used with user-defined types.\n";
    cout << "\n";
    cout << "Benefits:\n";
    cout << "  • More intuitive code: a + b vs a.add(b)\n";
    cout << "  • Natural syntax for custom types\n";
    cout << "  • Objects behave like built-in types\n";
    cout << "  • Better readability\n";

    cout << "\n========================================\n";
    cout << "BASIC SYNTAX\n";
    cout << "========================================\n";
    cout << "As member function:\n";
    cout << "  ReturnType operator@(parameters) { }\n";
    cout << "\n";
    cout << "As non-member function:\n";
    cout << "  ReturnType operator@(Type1, Type2) { }\n";
    cout << "\n";
    cout << "Example:\n";
    cout << "  class MyClass {\n";
    cout << "  public:\n";
    cout << "      MyClass operator+(const MyClass& other) const {\n";
    cout << "          // return sum\n";
    cout << "      }\n";
    cout << "  };\n";
    cout << "\n";
    cout << "Usage:\n";
    cout << "  MyClass a, b, c;\n";
    cout << "  c = a + b;  // Calls operator+\n";

    cout << "\n========================================\n";
    cout << "OPERATORS THAT CAN BE OVERLOADED\n";
    cout << "========================================\n";
    cout << "Arithmetic:\n";
    cout << "  +  -  *  /  %\n";
    cout << "  +=  -=  *=  /=  %=\n";
    cout << "\n";
    cout << "Comparison:\n";
    cout << "  ==  !=  <  >  <=  >=\n";
    cout << "\n";
    cout << "Logical:\n";
    cout << "  &&  ||  !\n";
    cout << "\n";
    cout << "Bitwise:\n";
    cout << "  &  |  ^  ~  <<  >>\n";
    cout << "  &=  |=  ^=  <<=  >>=\n";
    cout << "\n";
    cout << "Increment/Decrement:\n";
    cout << "  ++  --  (both prefix and postfix)\n";
    cout << "\n";
    cout << "Assignment:\n";
    cout << "  =  (copy assignment)\n";
    cout << "\n";
    cout << "Memory:\n";
    cout << "  new  delete  new[]  delete[]\n";
    cout << "\n";
    cout << "Other:\n";
    cout << "  []  ()  ->  ,  ->*\n";

    cout << "\n========================================\n";
    cout << "OPERATORS THAT CANNOT BE OVERLOADED\n";
    cout << "========================================\n";
    cout << "  ::  (scope resolution)\n";
    cout << "  .   (member access)\n";
    cout << "  .*  (member pointer access)\n";
    cout << "  ?:  (ternary conditional)\n";
    cout << "  sizeof\n";
    cout << "  typeid\n";

    cout << "\n========================================\n";
    cout << "MEMBER vs NON-MEMBER\n";
    cout << "========================================\n";
    cout << "MEMBER OPERATORS:\n";
    cout << "  • 'this' is left operand\n";
    cout << "  • Syntax: obj @ arg\n";
    cout << "  • Can access private members directly\n";
    cout << "  • MUST be member: =, [], (), ->\n";
    cout << "\n";
    cout << "NON-MEMBER OPERATORS:\n";
    cout << "  • Both operands as parameters\n";
    cout << "  • Syntax: operator@(obj1, obj2)\n";
    cout << "  • Allows type conversions on left operand\n";
    cout << "  • Usually for symmetric operators\n";
    cout << "  • Often declared as friends\n";
    cout << "\n";
    cout << "Example:\n";
    cout << "  class Vector {\n";
    cout << "      Vector operator*(double s) const;  // Member: v * 2\n";
    cout << "      friend Vector operator*(double s, const Vector& v);  // 2 * v\n";
    cout << "  };\n";

    cout << "\n========================================\n";
    cout << "SPECIFIC OPERATORS\n";
    cout << "========================================\n";
    cout << "1. Arithmetic (+, -, *, /, %):\n";
    cout << "   MyClass operator+(const MyClass& other) const;\n";
    cout << "\n";
    cout << "2. Comparison (==, !=, <, >, <=, >=):\n";
    cout << "   bool operator==(const MyClass& other) const;\n";
    cout << "\n";
    cout << "3. Assignment (=, +=, -=, etc.):\n";
    cout << "   MyClass& operator=(const MyClass& other);\n";
    cout << "   MyClass& operator+=(const MyClass& other);\n";
    cout << "\n";
    cout << "4. Increment/Decrement (++, --):\n";
    cout << "   MyClass& operator++();      // Prefix\n";
    cout << "   MyClass operator++(int);    // Postfix (int = dummy)\n";
    cout << "\n";
    cout << "5. Stream (<<, >>):\n";
    cout << "   friend ostream& operator<<(ostream& os, const MyClass& obj);\n";
    cout << "   friend istream& operator>>(istream& is, MyClass& obj);\n";
    cout << "\n";
    cout << "6. Subscript ([]):\n";
    cout << "   Type& operator[](int index);\n";
    cout << "   const Type& operator[](int index) const;\n";
    cout << "\n";
    cout << "7. Function call (()):\n";
    cout << "   ReturnType operator()(Parameters...);\n";

    cout << "\n========================================\n";
    cout << "PREFIX vs POSTFIX\n";
    cout << "========================================\n";
    cout << "Prefix (++obj):\n";
    cout << "  MyClass& operator++() {\n";
    cout << "      ++value;          // Increment\n";
    cout << "      return *this;     // Return new value\n";
    cout << "  }\n";
    cout << "\n";
    cout << "Postfix (obj++):\n";
    cout << "  MyClass operator++(int) {  // int = dummy parameter\n";
    cout << "      MyClass temp = *this;  // Save old value\n";
    cout << "      ++value;               // Increment\n";
    cout << "      return temp;           // Return old value\n";
    cout << "  }\n";
    cout << "\n";
    cout << "Key differences:\n";
    cout << "  • Postfix has dummy int parameter\n";
    cout << "  • Prefix returns reference (more efficient)\n";
    cout << "  • Postfix returns copy (less efficient)\n";
    cout << "  • Prefer prefix when value not used\n";

    cout << "\n========================================\n";
    cout << "STREAM OPERATORS\n";
    cout << "========================================\n";
    cout << "Output (<<):\n";
    cout << "  friend ostream& operator<<(ostream& os, const MyClass& obj) {\n";
    cout << "      os << obj.data;\n";
    cout << "      return os;  // For chaining\n";
    cout << "  }\n";
    cout << "\n";
    cout << "Input (>>):\n";
    cout << "  friend istream& operator>>(istream& is, MyClass& obj) {\n";
    cout << "      is >> obj.data;\n";
    cout << "      return is;  // For chaining\n";
    cout << "  }\n";
    cout << "\n";
    cout << "Usage:\n";
    cout << "  MyClass obj;\n";
    cout << "  cout << obj;  // Uses operator<<\n";
    cout << "  cin >> obj;   // Uses operator>>\n";
    cout << "  cout << obj1 << \" \" << obj2;  // Chaining\n";
    cout << "\n";
    cout << "Note: Must be non-member for natural syntax!\n";

    cout << "\n========================================\n";
    cout << "BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "✅ DO:\n";
    cout << "  • Keep consistent with built-in types\n";
    cout << "  • Return appropriate types:\n";
    cout << "    - Arithmetic: return by value\n";
    cout << "    - Assignment: return reference\n";
    cout << "    - Comparison: return bool\n";
    cout << "  • Use const for operators that don't modify\n";
    cout << "  • Implement related operators together\n";
    cout << "  • Use non-member for symmetric operators\n";
    cout << "  • Return stream references for chaining\n";
    cout << "  • Make comparison operators const\n";
    cout << "\n";
    cout << "❌ DON'T:\n";
    cout << "  • Change standard operator meanings\n";
    cout << "  • Overload &&, ||, comma (breaks short-circuit)\n";
    cout << "  • Make operator behavior surprising\n";
    cout << "  • Overload just for fun (must be useful)\n";
    cout << "  • Forget const correctness\n";
    cout << "  • Return by reference for temporary objects\n";

    cout << "\n========================================\n";
    cout << "COMMON PATTERNS\n";
    cout << "========================================\n";
    cout << "1. Implement operator== first, reuse for !=:\n";
    cout << "   bool operator!=(const T& o) const { return !(*this == o); }\n";
    cout << "\n";
    cout << "2. Implement operator< first, derive others:\n";
    cout << "   bool operator>(const T& o) const { return o < *this; }\n";
    cout << "   bool operator<=(const T& o) const { return !(*this > o); }\n";
    cout << "   bool operator>=(const T& o) const { return !(*this < o); }\n";
    cout << "\n";
    cout << "3. Implement operator+ in terms of +=:\n";
    cout << "   T operator+(const T& o) const {\n";
    cout << "       T temp = *this;\n";
    cout << "       temp += o;\n";
    cout << "       return temp;\n";
    cout << "   }\n";
    cout << "\n";
    cout << "4. Symmetric operators as non-members:\n";
    cout << "   friend T operator*(double s, const T& obj) {\n";
    cout << "       return obj * s;  // Reuse member operator\n";
    cout << "   }\n";

    cout << "\n========================================\n";
    cout << "COMMON MISTAKES\n";
    cout << "========================================\n";
    cout << "❌ Mistake 1: Wrong return type\n";
    cout << "   MyClass operator+(const MyClass& o) {  // Missing const\n";
    cout << "       // modifying this!\n";
    cout << "   }\n";
    cout << "\n";
    cout << "❌ Mistake 2: Returning by reference for temporary\n";
    cout << "   MyClass& operator+(const MyClass& o) const {  // Dangerous!\n";
    cout << "       MyClass temp;  // Local variable\n";
    cout << "       return temp;   // Returns reference to destroyed object!\n";
    cout << "   }\n";
    cout << "\n";
    cout << "❌ Mistake 3: Not returning *this in assignment\n";
    cout << "   void operator=(const MyClass& o) {  // Can't chain\n";
    cout << "       // ...\n";
    cout << "   }\n";
    cout << "\n";
    cout << "❌ Mistake 4: Ignoring const correctness\n";
    cout << "   bool operator==(const MyClass& o) {  // Missing const!\n";
    cout << "       return value == o.value;\n";
    cout << "   }\n";

    cout << "\n========================================\n";
    cout << "QUICK REFERENCE\n";
    cout << "========================================\n";
    cout << "// Arithmetic\n";
    cout << "MyClass operator+(const MyClass& o) const;\n";
    cout << "MyClass& operator+=(const MyClass& o);\n";
    cout << "\n";
    cout << "// Comparison\n";
    cout << "bool operator==(const MyClass& o) const;\n";
    cout << "bool operator<(const MyClass& o) const;\n";
    cout << "\n";
    cout << "// Assignment\n";
    cout << "MyClass& operator=(const MyClass& o);\n";
    cout << "\n";
    cout << "// Increment/Decrement\n";
    cout << "MyClass& operator++();      // Prefix\n";
    cout << "MyClass operator++(int);    // Postfix\n";
    cout << "\n";
    cout << "// Stream\n";
    cout << "friend ostream& operator<<(ostream& os, const MyClass& obj);\n";
    cout << "\n";
    cout << "// Subscript\n";
    cout << "Type& operator[](int index);\n";
    cout << "\n";
    cout << "// Function call\n";
    cout << "ReturnType operator()(Parameters...);\n";

    cout << "\n========================================\n";
    cout << "KEY TAKEAWAYS\n";
    cout << "========================================\n";
    cout << "1. Operator overloading makes custom types intuitive\n";
    cout << "2. Syntax: returnType operator@(parameters)\n";
    cout << "3. Can be member or non-member (some must be member)\n";
    cout << "4. Keep behavior consistent with built-in types\n";
    cout << "5. Use const for non-modifying operators\n";
    cout << "6. Return appropriate types (value, reference, bool)\n";
    cout << "7. Implement related operators together\n";
    cout << "8. Stream operators must be non-member\n";
    cout << "9. Prefix ++ more efficient than postfix\n";
    cout << "10. Don't overload for surprising behavior\n";

    return 0;
}
