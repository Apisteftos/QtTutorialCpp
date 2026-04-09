#include <iostream>
#include <string>
#include <vector>
#include <cmath>
using namespace std;

// ============================================
// WHAT IS THE EXPLICIT KEYWORD?
// ============================================

/*
 * EXPLICIT KEYWORD:
 * - Prevents implicit type conversions
 * - Applied to constructors and conversion operators
 * - Forces explicit construction/conversion
 * - Improves type safety
 * - Prevents unexpected behavior
 *
 * WHY EXPLICIT?
 * - Avoid accidental conversions
 * - Prevent bugs from implicit conversions
 * - Make code more readable
 * - Enforce type safety
 * - Clear programmer intent
 *
 * SYNTAX:
 * explicit ConstructorName(Type param);
 * explicit operator Type();
 *
 * IMPLICIT CONVERSIONS (C++ allows by default):
 * - Single-argument constructors
 * - Conversion operators
 * - Can lead to unexpected behavior
 *
 * WHEN TO USE EXPLICIT:
 * - Single-argument constructors (usually)
 * - Conversion operators (usually)
 * - When conversion might be surprising
 * - When type safety is important
 *
 * WHEN NOT TO USE EXPLICIT:
 * - Copy/move constructors
 * - Multi-argument constructors (usually safe)
 * - When implicit conversion is intended
 * - Mathematical types (Point, Vector, etc.)
 */

// ============================================
// EXAMPLE 1: THE PROBLEM - IMPLICIT CONVERSION
// ============================================

class StringBad {
private:
    string data;

public:
    // ❌ Non-explicit constructor - allows implicit conversion!
    StringBad(const char* s) : data(s) {
        cout << "  StringBad created from: \"" << s << "\"\n";
    }

    void display() const {
        cout << "  Data: \"" << data << "\"\n";
    }
};

void processBad(StringBad s) {
    cout << "  Processing StringBad...\n";
    s.display();
}

void demonstrateProblem() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: THE PROBLEM           ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "--- Direct construction (OK) ---\n";
    StringBad s1("Hello");
    s1.display();

    cout << "\n--- Implicit conversion (Surprising!) ---\n";
    StringBad s2 = "World";  // ⚠️ Implicit conversion!
    s2.display();

    cout << "\n--- Function call with implicit conversion ---\n";
    processBad("Implicit");  // ⚠️ string -> StringBad automatic!

    cout << "\n⚠️  THE PROBLEM:\n";
    cout << "   • Constructor accepts implicit conversions\n";
    cout << "   • Can pass string where StringBad expected\n";
    cout << "   • Might create temporary objects unexpectedly\n";
    cout << "   • Can hide bugs and reduce type safety\n";
    cout << "   • Performance overhead from conversions\n";
}

// ============================================
// EXAMPLE 2: THE SOLUTION - EXPLICIT KEYWORD
// ============================================

class StringGood {
private:
    string data;

public:
    // ✅ Explicit constructor - prevents implicit conversion!
    explicit StringGood(const string& s) : data(s) {
        cout << "  StringGood created from: \"" << s << "\"\n";
    }

    void display() const {
        cout << "  Data: \"" << data << "\"\n";
    }
};

void processGood(StringGood s) {
    cout << "  Processing StringGood...\n";
    s.display();
}

void demonstrateSolution() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: THE SOLUTION          ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "--- Direct construction (OK) ---\n";
    StringGood s1("Hello");
    s1.display();

    cout << "\n--- Attempting implicit conversion ---\n";
    // StringGood s2 = "World";  // ❌ Error! Explicit prevents this
    cout << "  // StringGood s2 = \"World\";  // ❌ Compilation error!\n";

    cout << "\n--- Explicit construction required ---\n";
    StringGood s3 = StringGood("Explicit");  // ✅ Must be explicit
    s3.display();

    cout << "\n--- Function call ---\n";
    // processGood("Implicit");  // ❌ Error! Can't convert
    cout << "  // processGood(\"Implicit\");  // ❌ Compilation error!\n";
    processGood(StringGood("Must be explicit"));  // ✅ Must specify type

    cout << "\n✅ THE SOLUTION:\n";
    cout << "   • Prevents accidental conversions\n";
    cout << "   • Forces explicit construction\n";
    cout << "   • Improves type safety\n";
    cout << "   • Makes code intent clear\n";
    cout << "   • Catches bugs at compile time\n";
}

// ============================================
// EXAMPLE 3: ARRAY CLASS - DANGEROUS WITHOUT EXPLICIT
// ============================================

class ArrayBad {
private:
    int* data;
    size_t size;

public:
    // ❌ Dangerous! Allows implicit conversion from int
    ArrayBad(size_t s) : size(s) {
        data = new int[size]();
        cout << "  ArrayBad created with size: " << size << "\n";
    }

    ~ArrayBad() {
        delete[] data;
    }

    // Copy operations deleted for simplicity
    ArrayBad(const ArrayBad&) = delete;
    ArrayBad& operator=(const ArrayBad&) = delete;

    size_t getSize() const { return size; }
};

void processArrayBad(ArrayBad arr) {
    cout << "  Processing array of size: " << arr.getSize() << "\n";
}

class ArrayGood {
private:
    int* data;
    size_t size;

public:
    // ✅ Explicit! Prevents accidental creation
    explicit ArrayGood(size_t s) : size(s) {
        data = new int[size]();
        cout << "  ArrayGood created with size: " << size << "\n";
    }

    ~ArrayGood() {
        delete[] data;
    }

    ArrayGood(const ArrayGood&) = delete;
    ArrayGood& operator=(const ArrayGood&) = delete;

    size_t getSize() const { return size; }
};

void processArrayGood(ArrayGood arr) {
    cout << "  Processing array of size: " << arr.getSize() << "\n";
}

void demonstrateArrayDanger() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: ARRAY DANGER          ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "--- ArrayBad (non-explicit) ---\n";
    processArrayBad(10);  // ⚠️ Implicitly creates ArrayBad(10)!
    cout << "  ⚠️  Created temporary array unexpectedly!\n";

    cout << "\n--- What if we meant to pass 0.5? ---\n";
    processArrayBad(0.5);  // ⚠️ Converts 0.5 -> 0 -> ArrayBad(0)!
    cout << "  ⚠️  0.5 converted to 0 silently!\n";

    cout << "\n--- ArrayGood (explicit) ---\n";
    // processArrayGood(10);  // ❌ Error! Can't convert
    cout << "  // processArrayGood(10);  // ❌ Compilation error!\n";
    processArrayGood(ArrayGood(10));  // ✅ Must be explicit

    cout << "\n💡 WHY EXPLICIT MATTERS:\n";
    cout << "   • Prevents accidental array creation\n";
    cout << "   • Avoids silent conversions (0.5 -> 0)\n";
    cout << "   • Makes resource allocation explicit\n";
    cout << "   • Catches logic errors at compile time\n";
}

// ============================================
// EXAMPLE 4: EXPLICIT CONVERSION OPERATORS
// ============================================

class SmartPointerBad {
private:
    int* ptr;

public:
    SmartPointerBad(int* p = nullptr) : ptr(p) {}

    ~SmartPointerBad() {
        delete ptr;
    }

    // ❌ Non-explicit bool conversion - dangerous!
    operator bool() const {
        return ptr != nullptr;
    }

    int* get() const { return ptr; }
};

class SmartPointerGood {
private:
    int* ptr;

public:
    SmartPointerGood(int* p = nullptr) : ptr(p) {}

    ~SmartPointerGood() {
        delete ptr;
    }

    // ✅ Explicit bool conversion - safe!
    explicit operator bool() const {
        return ptr != nullptr;
    }

    int* get() const { return ptr; }
};

void demonstrateConversionOperator() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: CONVERSION OPERATORS  ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    SmartPointerBad bad(new int(42));
    SmartPointerGood good(new int(100));

    cout << "--- Using in if statement (both OK) ---\n";
    if (bad) {
        cout << "  SmartPointerBad is not null\n";
    }

    if (good) {
        cout << "  SmartPointerGood is not null\n";
    }

    cout << "\n--- Dangerous implicit conversions ---\n";
    bool b = bad;  // ⚠️ Implicit conversion to bool
    cout << "  bool b = bad;  // ⚠️ Works but might be unintended\n";

    int x = bad;   // ⚠️ bool converts to int (0 or 1)!
    cout << "  int x = bad;   // ⚠️ Converts to int! x = " << x << "\n";

    cout << "\n--- Explicit prevents accidents ---\n";
    // bool b2 = good;  // ❌ Error! Explicit prevents this
    cout << "  // bool b2 = good;  // ❌ Compilation error!\n";

    bool b3 = static_cast<bool>(good);  // ✅ Must be explicit
    cout << "  bool b3 = static_cast<bool>(good);  // ✅ OK\n";

    cout << "\n💡 EXPLICIT CONVERSION OPERATOR:\n";
    cout << "   • Works in boolean contexts (if, while, etc.)\n";
    cout << "   • Prevents other implicit conversions\n";
    cout << "   • Similar to std::unique_ptr behavior\n";
    cout << "   • Essential for safe smart pointers\n";
}

// ============================================
// EXAMPLE 5: NUMERIC TYPES - WHEN EXPLICIT MAKES SENSE
// ============================================

class Dollars {
private:
    double amount;

public:
    // ✅ Explicit - money shouldn't convert implicitly
    explicit Dollars(double amt) : amount(amt) {}

    double getAmount() const { return amount; }

    void display() const {
        cout << "  $" << amount << "\n";
    }
};

class Cents {
private:
    int amount;

public:
    // ✅ Explicit - prevent confusion with Dollars
    explicit Cents(int amt) : amount(amt) {}

    int getAmount() const { return amount; }

    void display() const {
        cout << "  " << amount << "¢\n";
    }
};

void payDollars(Dollars d) {
    cout << "  Paying: ";
    d.display();
}

void payCents(Cents c) {
    cout << "  Paying: ";
    c.display();
}

void demonstrateNumericTypes() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: NUMERIC TYPES         ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    Dollars d(100.50);
    Cents c(150);

    cout << "--- Direct usage ---\n";
    d.display();
    c.display();

    cout << "\n--- Function calls require explicit construction ---\n";
    // payDollars(50.0);  // ❌ Error! Can't convert double to Dollars
    cout << "  // payDollars(50.0);  // ❌ Compilation error!\n";
    payDollars(Dollars(50.0));  // ✅ Must be explicit

    // payCents(25);  // ❌ Error! Can't convert int to Cents
    cout << "  // payCents(25);  // ❌ Compilation error!\n";
    payCents(Cents(25));  // ✅ Must be explicit

    cout << "\n💡 WHY EXPLICIT FOR MONEY:\n";
    cout << "   • Prevents mixing dollars and cents\n";
    cout << "   • Avoids accidental conversions\n";
    cout << "   • Makes financial code safer\n";
    cout << "   • Clear intent in calculations\n";
}

// ============================================
// EXAMPLE 6: WHEN NOT TO USE EXPLICIT
// ============================================

class Point {
private:
    double x, y;

public:
    // ✅ NOT explicit - implicit conversion makes sense for Point
    Point(double value) : x(value), y(value) {
        cout << "  Point(" << value << ", " << value << ") created\n";
    }

    Point(double xVal, double yVal) : x(xVal), y(yVal) {
        cout << "  Point(" << xVal << ", " << yVal << ") created\n";
    }

    void display() const {
        cout << "  Point(" << x << ", " << y << ")\n";
    }

    Point operator+(const Point& other) const {
        return Point(x + other.x, y + other.y);
    }
};

class ComplexNumber {
private:
    double real, imag;

public:
    // ✅ NOT explicit - common in mathematical code
    ComplexNumber(double r, double i = 0.0) : real(r), imag(i) {
        cout << "  Complex(" << r << ", " << i << ") created\n";
    }

    void display() const {
        cout << "  " << real << " + " << imag << "i\n";
    }

    ComplexNumber operator+(const ComplexNumber& other) const {
        return ComplexNumber(real + other.real, imag + other.imag);
    }
};

void demonstrateWhenNotExplicit() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: WHEN NOT EXPLICIT     ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "--- Point: Natural implicit conversions ---\n";
    Point p1(3.0, 4.0);
    Point p2 = 5.0;  // ✅ Natural: creates Point(5.0, 5.0)
    p2.display();

    Point p3 = p1 + 2.0;  // ✅ Natural: 2.0 -> Point(2.0, 2.0)
    cout << "  p1 + 2.0 = ";
    p3.display();

    cout << "\n--- ComplexNumber: Mathematical convenience ---\n";
    ComplexNumber c1(3.0, 4.0);
    ComplexNumber c2 = 5.0;  // ✅ Natural: creates Complex(5.0, 0.0)
    c2.display();

    ComplexNumber c3 = c1 + 2.0;  // ✅ Natural: real numbers are complex
    cout << "  c1 + 2.0 = ";
    c3.display();

    cout << "\n✅ WHEN TO ALLOW IMPLICIT:\n";
    cout << "   • Mathematical types (Point, Complex, Vector)\n";
    cout << "   • Natural semantic conversions\n";
    cout << "   • When conversion is 'lossless'\n";
    cout << "   • Following conventions (like std::string)\n";
    cout << "   • When it improves readability\n";
}

// ============================================
// EXAMPLE 7: REAL-WORLD - FILE HANDLE
// ============================================

class FileHandleBad {
private:
    string filename;
    bool isOpen;

public:
    // ❌ Non-explicit - can accidentally convert string to FileHandle
    FileHandleBad(const char* name)
        : filename(name), isOpen(false) {
        cout << "  FileHandleBad: \"" << filename << "\"\n";
    }

    bool open() {
        isOpen = true;
        return true;
    }
};

void processFileBad(FileHandleBad file) {
    cout << "  Processing file...\n";
}

class FileHandleGood {
private:
    string filename;
    bool isOpen;

public:
    // ✅ Explicit - file operations should be intentional
    explicit FileHandleGood(const string& name)
        : filename(name), isOpen(false) {
        cout << "  FileHandleGood: \"" << filename << "\"\n";
    }

    bool open() {
        isOpen = true;
        return true;
    }
};

void processFileGood(FileHandleGood file) {
    cout << "  Processing file...\n";
}

void demonstrateFileHandle() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 7: FILE HANDLE           ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "--- FileHandleBad (non-explicit) ---\n";
    processFileBad("data.txt");  // ⚠️ Creates temporary FileHandle!
    cout << "  ⚠️  Accidentally created file handle from string!\n";

    cout << "\n--- FileHandleGood (explicit) ---\n";
    // processFileGood("data.txt");  // ❌ Error! Can't convert
    cout << "  // processFileGood(\"data.txt\");  // ❌ Error!\n";
    processFileGood(FileHandleGood("data.txt"));  // ✅ Intentional

    cout << "\n💡 REAL-WORLD BENEFIT:\n";
    cout << "   • File operations should be explicit\n";
    cout << "   • Prevents accidental file handle creation\n";
    cout << "   • Makes resource management clear\n";
    cout << "   • Catches programming mistakes\n";
}

// ============================================
// EXAMPLE 8: MULTIPLE CONSTRUCTORS
// ============================================

class Buffer {
private:
    char* data;
    size_t size;

public:
    // ✅ Explicit - size constructor
    explicit Buffer(size_t s) : size(s) {
        data = new char[size];
        cout << "  Buffer created with size: " << size << "\n";
    }

    // ✅ NOT explicit - two arguments (usually safe)
    Buffer(size_t s, char fill) : size(s) {
        data = new char[size];
        for (size_t i = 0; i < size; ++i) {
            data[i] = fill;
        }
        cout << "  Buffer created with size " << size
             << " filled with '" << fill << "'\n";
    }

    ~Buffer() {
        delete[] data;
    }

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    size_t getSize() const { return size; }
};

void demonstrateMultipleConstructors() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 8: MULTIPLE CONSTRUCTORS ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "--- Single-argument constructor (explicit) ---\n";
    Buffer b1(10);
    // Buffer b2 = 20;  // ❌ Error! Explicit prevents this

    cout << "\n--- Two-argument constructor (implicit OK) ---\n";
    Buffer b3(15, 'X');
    Buffer b4 = {20, 'Y'};  // ✅ Works with uniform initialization

    cout << "\n💡 RULE OF THUMB:\n";
    cout << "   • Single-argument: usually explicit\n";
    cout << "   • Multi-argument: usually not explicit\n";
    cout << "   • Exception: when implicit makes sense\n";
}

// ============================================
// EXAMPLE 9: STANDARD LIBRARY EXAMPLES
// ============================================

void demonstrateStdLibrary() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 9: STD LIBRARY EXAMPLES  ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "--- std::vector ---\n";
    vector<int> v1(10);  // Creates vector with 10 elements
    cout << "  vector<int> v1(10);  // Size constructor\n";
    cout << "  Size: " << v1.size() << "\n";

    // vector<int> v2 = 10;  // ❌ Error! Constructor is explicit
    cout << "  // vector<int> v2 = 10;  // ❌ Error!\n";

    cout << "\n--- std::unique_ptr ---\n";
    // unique_ptr<int> p1 = new int(42);  // ❌ Error! Explicit
    cout << "  // unique_ptr<int> p1 = new int(42);  // ❌ Error!\n";
    cout << "  unique_ptr<int> p1(new int(42));  // ✅ Must be explicit\n";

    cout << "\n--- std::string ---\n";
    string s1 = "Hello";  // ✅ Allowed! Not explicit
    cout << "  string s1 = \"Hello\";  // ✅ Implicit conversion OK\n";
    cout << "  (string constructor is NOT explicit)\n";

    cout << "\n💡 STANDARD LIBRARY USAGE:\n";
    cout << "   • vector: size constructor is explicit\n";
    cout << "   • unique_ptr: constructor is explicit\n";
    cout << "   • string: constructor is NOT explicit\n";
    cout << "   • Smart pointers generally use explicit\n";
}

// ============================================
// EXAMPLE 10: SUMMARY AND BEST PRACTICES
// ============================================

void demonstrateBestPractices() {
    cout << "\n╔═══════════════════════════════════╗\n";
    cout << "║  EXAMPLE 10: BEST PRACTICES       ║\n";
    cout << "╚═══════════════════════════════════╝\n\n";

    cout << "✅ ALWAYS USE EXPLICIT FOR:\n";
    cout << "   • Single-argument constructors (default)\n";
    cout << "   • Constructors taking numeric types\n";
    cout << "   • Resource-managing classes\n";
    cout << "   • Conversion operators (usually)\n";
    cout << "   • When in doubt, use explicit!\n";
    cout << "\n";
    cout << "❌ DON'T USE EXPLICIT FOR:\n";
    cout << "   • Copy constructors\n";
    cout << "   • Move constructors\n";
    cout << "   • Mathematical types (Point, Complex)\n";
    cout << "   • When implicit conversion is natural\n";
    cout << "   • Multi-argument constructors (usually)\n";
    cout << "\n";
    cout << "💡 MODERN C++ GUIDELINE:\n";
    cout << "   \"Make single-argument constructors explicit\n";
    cout << "    unless implicit conversion is intended.\"\n";
    cout << "\n";
    cout << "⚠️  WHEN IN DOUBT:\n";
    cout << "   Use explicit! It's easier to remove later\n";
    cout << "   than to find bugs caused by implicit conversions.\n";
}

// ============================================
// MAIN FUNCTION - RUN ALL EXAMPLES
// ============================================

int main() {
    cout << "\n";
    cout << "╔═══════════════════════════════════════════════════════════╗\n";
    cout << "║                                                           ║\n";
    cout << "║             EXPLICIT KEYWORD IN C++                       ║\n";
    cout << "║            Preventing Implicit Conversions                ║\n";
    cout << "║                                                           ║\n";
    cout << "╚═══════════════════════════════════════════════════════════╝\n";

    demonstrateProblem();
    demonstrateSolution();
    demonstrateArrayDanger();
    demonstrateConversionOperator();
    demonstrateNumericTypes();
    demonstrateWhenNotExplicit();
    demonstrateFileHandle();
    demonstrateMultipleConstructors();
    demonstrateStdLibrary();
    demonstrateBestPractices();

    // ============================================
    // COMPREHENSIVE EXPLANATION
    // ============================================

    cout << "\n\n";
    cout << "╔═══════════════════════════════════════════════════════════╗\n";
    cout << "║                                                           ║\n";
    cout << "║              COMPREHENSIVE EXPLANATION                    ║\n";
    cout << "║                                                           ║\n";
    cout << "╚═══════════════════════════════════════════════════════════╝\n";

    cout << "\n========================================\n";
    cout << "WHAT IS EXPLICIT?\n";
    cout << "========================================\n";
    cout << "The explicit keyword:\n";
    cout << "  • Prevents implicit type conversions\n";
    cout << "  • Applied to constructors\n";
    cout << "  • Applied to conversion operators\n";
    cout << "  • Forces explicit construction/conversion\n";
    cout << "  • Improves type safety\n";
    cout << "\n";
    cout << "SYNTAX:\n";
    cout << "class MyClass {\n";
    cout << "public:\n";
    cout << "    explicit MyClass(int);           // Explicit constructor\n";
    cout << "    explicit operator bool() const;  // Explicit conversion\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "WHY EXPLICIT EXISTS\n";
    cout << "========================================\n";
    cout << "THE PROBLEM:\n";
    cout << "class Array {\n";
    cout << "public:\n";
    cout << "    Array(size_t size);  // Non-explicit\n";
    cout << "};\n";
    cout << "\n";
    cout << "void process(Array arr);\n";
    cout << "\n";
    cout << "// Implicit conversions happen:\n";
    cout << "process(10);       // Creates Array(10) automatically!\n";
    cout << "process(0.5);      // Converts 0.5 -> 0 -> Array(0)!\n";
    cout << "Array a = 42;      // Implicit conversion from int!\n";
    cout << "\n";
    cout << "PROBLEMS:\n";
    cout << "  • Unexpected temporary objects\n";
    cout << "  • Silent conversions (0.5 -> 0)\n";
    cout << "  • Hard-to-find bugs\n";
    cout << "  • Performance overhead\n";
    cout << "  • Unclear code intent\n";
    cout << "\n";
    cout << "THE SOLUTION:\n";
    cout << "class Array {\n";
    cout << "public:\n";
    cout << "    explicit Array(size_t size);  // Explicit!\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Now these are errors:\n";
    cout << "// process(10);       // ❌ Error!\n";
    cout << "// Array a = 42;      // ❌ Error!\n";
    cout << "\n";
    cout << "// Must be explicit:\n";
    cout << "process(Array(10));   // ✅ Clear intent\n";
    cout << "Array a(42);          // ✅ Clear intent\n";

    cout << "\n========================================\n";
    cout << "IMPLICIT CONVERSIONS IN C++\n";
    cout << "========================================\n";
    cout << "C++ ALLOWS IMPLICIT CONVERSIONS FOR:\n";
    cout << "\n";
    cout << "1. Single-argument constructors:\n";
    cout << "   class String {\n";
    cout << "       String(const char*);  // Can convert implicitly\n";
    cout << "   };\n";
    cout << "   String s = \"hello\";  // Implicit conversion\n";
    cout << "\n";
    cout << "2. Conversion operators:\n";
    cout << "   class Integer {\n";
    cout << "       operator int() const;  // Convert to int\n";
    cout << "   };\n";
    cout << "   Integer i;\n";
    cout << "   int x = i;  // Implicit conversion\n";
    cout << "\n";
    cout << "WHY THIS CAN BE DANGEROUS:\n";
    cout << "  • Unexpected behavior\n";
    cout << "  • Performance issues\n";
    cout << "  • Ambiguous function calls\n";
    cout << "  • Silent numeric conversions\n";
    cout << "  • Hidden bugs\n";

    cout << "\n========================================\n";
    cout << "EXPLICIT WITH CONSTRUCTORS\n";
    cout << "========================================\n";
    cout << "WITHOUT EXPLICIT:\n";
    cout << "class MyClass {\n";
    cout << "public:\n";
    cout << "    MyClass(int x) {}  // Allows implicit conversion\n";
    cout << "};\n";
    cout << "\n";
    cout << "MyClass obj1(10);      // Direct initialization\n";
    cout << "MyClass obj2 = 20;     // ⚠️  Implicit conversion!\n";
    cout << "\n";
    cout << "void func(MyClass obj);\n";
    cout << "func(30);              // ⚠️  Creates temporary MyClass(30)!\n";
    cout << "\n";
    cout << "WITH EXPLICIT:\n";
    cout << "class MyClass {\n";
    cout << "public:\n";
    cout << "    explicit MyClass(int x) {}  // Prevents implicit\n";
    cout << "};\n";
    cout << "\n";
    cout << "MyClass obj1(10);      // ✅ OK\n";
    cout << "// MyClass obj2 = 20;  // ❌ Error!\n";
    cout << "\n";
    cout << "void func(MyClass obj);\n";
    cout << "// func(30);           // ❌ Error!\n";
    cout << "func(MyClass(30));     // ✅ Must be explicit\n";

    cout << "\n========================================\n";
    cout << "EXPLICIT WITH CONVERSION OPERATORS\n";
    cout << "========================================\n";
    cout << "WITHOUT EXPLICIT:\n";
    cout << "class SmartPtr {\n";
    cout << "public:\n";
    cout << "    operator bool() const;  // Implicit conversion\n";
    cout << "};\n";
    cout << "\n";
    cout << "SmartPtr p;\n";
    cout << "if (p) {}              // ✅ OK - intended\n";
    cout << "bool b = p;            // ⚠️  Implicit conversion\n";
    cout << "int x = p;             // ⚠️  Converts bool->int!\n";
    cout << "p + 5;                 // ⚠️  Converts to int!\n";
    cout << "\n";
    cout << "WITH EXPLICIT:\n";
    cout << "class SmartPtr {\n";
    cout << "public:\n";
    cout << "    explicit operator bool() const;  // Explicit\n";
    cout << "};\n";
    cout << "\n";
    cout << "SmartPtr p;\n";
    cout << "if (p) {}              // ✅ OK - contextual conversion\n";
    cout << "// bool b = p;         // ❌ Error!\n";
    cout << "// int x = p;          // ❌ Error!\n";
    cout << "// p + 5;              // ❌ Error!\n";
    cout << "\n";
    cout << "Must use explicit cast:\n";
    cout << "bool b = static_cast<bool>(p);  // ✅ OK\n";

    cout << "\n========================================\n";
    cout << "WHEN TO USE EXPLICIT\n";
    cout << "========================================\n";
    cout << "✅ USE EXPLICIT FOR:\n";
    cout << "\n";
    cout << "1. Single-argument constructors (default rule):\n";
    cout << "   explicit MyClass(int);\n";
    cout << "   explicit MyClass(const string&);\n";
    cout << "\n";
    cout << "2. Constructors with default parameters:\n";
    cout << "   explicit MyClass(int x, int y = 0);\n";
    cout << "   // Acts like single-argument constructor!\n";
    cout << "\n";
    cout << "3. Conversion operators (usually):\n";
    cout << "   explicit operator bool() const;\n";
    cout << "   explicit operator int() const;\n";
    cout << "\n";
    cout << "4. Resource-managing classes:\n";
    cout << "   explicit Array(size_t size);\n";
    cout << "   explicit FileHandle(const string& name);\n";
    cout << "\n";
    cout << "5. Numeric types with different meanings:\n";
    cout << "   explicit Dollars(double amount);\n";
    cout << "   explicit Meters(double length);\n";

    cout << "\n========================================\n";
    cout << "WHEN NOT TO USE EXPLICIT\n";
    cout << "========================================\n";
    cout << "❌ DON'T USE EXPLICIT FOR:\n";
    cout << "\n";
    cout << "1. Copy constructors:\n";
    cout << "   MyClass(const MyClass&);  // Never explicit\n";
    cout << "\n";
    cout << "2. Move constructors:\n";
    cout << "   MyClass(MyClass&&);       // Never explicit\n";
    cout << "\n";
    cout << "3. Multi-argument constructors (usually):\n";
    cout << "   MyClass(int x, int y);    // Usually not explicit\n";
    cout << "   // Can't be used for implicit conversion anyway\n";
    cout << "\n";
    cout << "4. Mathematical types:\n";
    cout << "   Point(double value);      // Natural conversion\n";
    cout << "   Complex(double real);     // Real->Complex natural\n";
    cout << "\n";
    cout << "5. When implicit conversion is intended:\n";
    cout << "   string(const char*);      // Designed for implicit use\n";

    cout << "\n========================================\n";
    cout << "CONTEXTUAL CONVERSIONS\n";
    cout << "========================================\n";
    cout << "EXPLICIT BOOL WORKS IN:\n";
    cout << "\n";
    cout << "class SmartPtr {\n";
    cout << "    explicit operator bool() const;\n";
    cout << "};\n";
    cout << "\n";
    cout << "SmartPtr p;\n";
    cout << "\n";
    cout << "// ✅ Contextual conversions allowed:\n";
    cout << "if (p) {}                    // Boolean context\n";
    cout << "while (p) {}                 // Boolean context\n";
    cout << "for (; p;) {}                // Boolean context\n";
    cout << "bool b = p ? true : false;   // Ternary operator\n";
    cout << "if (!p) {}                   // Negation\n";
    cout << "if (p && other) {}           // Logical AND\n";
    cout << "\n";
    cout << "// ❌ Other conversions prevented:\n";
    cout << "// bool b = p;               // Error\n";
    cout << "// int x = p;                // Error\n";
    cout << "// p + 1;                    // Error\n";

    cout << "\n========================================\n";
    cout << "COMMON PATTERNS\n";
    cout << "========================================\n";
    cout << "PATTERN 1: Container Size Constructor\n";
    cout << "class Container {\n";
    cout << "public:\n";
    cout << "    explicit Container(size_t size);\n";
    cout << "};\n";
    cout << "// Prevents: Container c = 10;\n";
    cout << "// Requires: Container c(10);\n";
    cout << "\n";
    cout << "PATTERN 2: Smart Pointer\n";
    cout << "template<typename T>\n";
    cout << "class UniquePtr {\n";
    cout << "public:\n";
    cout << "    explicit UniquePtr(T* ptr);\n";
    cout << "    explicit operator bool() const;\n";
    cout << "};\n";
    cout << "// Prevents accidental conversions\n";
    cout << "\n";
    cout << "PATTERN 3: Strong Types\n";
    cout << "class UserId {\n";
    cout << "    int id;\n";
    cout << "public:\n";
    cout << "    explicit UserId(int i) : id(i) {}\n";
    cout << "};\n";
    cout << "// Prevents mixing with regular ints\n";
    cout << "\n";
    cout << "PATTERN 4: Unit Types\n";
    cout << "class Meters {\n";
    cout << "    double value;\n";
    cout << "public:\n";
    cout << "    explicit Meters(double v) : value(v) {}\n";
    cout << "};\n";
    cout << "// Prevents confusion with other units\n";

    cout << "\n========================================\n";
    cout << "REAL-WORLD EXAMPLES\n";
    cout << "========================================\n";
    cout << "STANDARD LIBRARY USAGE:\n";
    cout << "\n";
    cout << "1. std::vector:\n";
    cout << "   explicit vector(size_t n);  // Size constructor\n";
    cout << "   // vector<int> v = 10;      // ❌ Error\n";
    cout << "   vector<int> v(10);          // ✅ OK\n";
    cout << "\n";
    cout << "2. std::unique_ptr:\n";
    cout << "   explicit unique_ptr(T* ptr);\n";
    cout << "   // unique_ptr<int> p = new int;  // ❌ Error\n";
    cout << "   unique_ptr<int> p(new int);      // ✅ OK\n";
    cout << "\n";
    cout << "3. std::thread:\n";
    cout << "   explicit thread(Function&& f);\n";
    cout << "   // Prevents accidental thread creation\n";
    cout << "\n";
    cout << "4. std::string (NOT explicit!):\n";
    cout << "   string(const char*);  // Intentionally implicit\n";
    cout << "   string s = \"hello\";  // ✅ Allowed by design\n";

    cout << "\n========================================\n";
    cout << "BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "✅ DO:\n";
    cout << "  • Make single-arg constructors explicit by default\n";
    cout << "  • Use explicit for conversion operators\n";
    cout << "  • Think about implicit conversion implications\n";
    cout << "  • Follow standard library conventions\n";
    cout << "  • Document when implicit is intended\n";
    cout << "  • Use explicit for resource classes\n";
    cout << "  • When in doubt, use explicit\n";
    cout << "\n";
    cout << "❌ DON'T:\n";
    cout << "  • Make copy/move constructors explicit\n";
    cout << "  • Over-use for mathematical types\n";
    cout << "  • Make multi-arg constructors explicit (usually)\n";
    cout << "  • Forget contextual bool conversions\n";
    cout << "  • Use for types designed for implicit use\n";

    cout << "\n========================================\n";
    cout << "COMMON MISTAKES\n";
    cout << "========================================\n";
    cout << "❌ MISTAKE 1: Forgetting explicit\n";
    cout << "class Array {\n";
    cout << "    Array(size_t size);  // ❌ Should be explicit!\n";
    cout << "};\n";
    cout << "// Allows: Array a = 10;  (probably unintended)\n";
    cout << "\n";
    cout << "❌ MISTAKE 2: Explicit on copy constructor\n";
    cout << "class Bad {\n";
    cout << "    explicit Bad(const Bad&);  // ❌ Wrong!\n";
    cout << "};\n";
    cout << "// Breaks normal copy semantics\n";
    cout << "\n";
    cout << "❌ MISTAKE 3: Non-explicit bool operator\n";
    cout << "class Pointer {\n";
    cout << "    operator bool();  // ❌ Should be explicit!\n";
    cout << "};\n";
    cout << "// Allows: int x = ptr;  (converts to int!)\n";
    cout << "\n";
    cout << "❌ MISTAKE 4: Explicit on natural conversions\n";
    cout << "class Complex {\n";
    cout << "    explicit Complex(double);  // ⚠️  Usually not explicit\n";
    cout << "};\n";
    cout << "// Prevents: Complex c = 3.14;  (might be intended)\n";

    cout << "\n========================================\n";
    cout << "DECISION FLOWCHART\n";
    cout << "========================================\n";
    cout << "Is it a copy/move constructor?\n";
    cout << "  YES → Never use explicit\n";
    cout << "  NO  → Continue...\n";
    cout << "\n";
    cout << "Is it a single-argument constructor?\n";
    cout << "  NO  → Usually don't need explicit\n";
    cout << "  YES → Continue...\n";
    cout << "\n";
    cout << "Is implicit conversion natural/intended?\n";
    cout << "  YES → Don't use explicit (Point, Complex, etc.)\n";
    cout << "  NO  → Continue...\n";
    cout << "\n";
    cout << "Does it manage resources?\n";
    cout << "  YES → USE EXPLICIT!\n";
    cout << "  NO  → Continue...\n";
    cout << "\n";
    cout << "Could implicit conversion cause bugs?\n";
    cout << "  YES → USE EXPLICIT!\n";
    cout << "  NO  → Continue...\n";
    cout << "\n";
    cout << "When in doubt?\n";
    cout << "  → USE EXPLICIT! (safer default)\n";

    cout << "\n========================================\n";
    cout << "QUICK REFERENCE\n";
    cout << "========================================\n";
    cout << "// Explicit constructor\n";
    cout << "class MyClass {\n";
    cout << "public:\n";
    cout << "    explicit MyClass(int x);\n";
    cout << "};\n";
    cout << "\n";
    cout << "MyClass obj1(10);           // ✅ Direct initialization\n";
    cout << "// MyClass obj2 = 10;       // ❌ Error\n";
    cout << "MyClass obj3 = MyClass(10); // ✅ Explicit construction\n";
    cout << "\n";
    cout << "// Explicit conversion operator\n";
    cout << "class SmartPtr {\n";
    cout << "public:\n";
    cout << "    explicit operator bool() const;\n";
    cout << "};\n";
    cout << "\n";
    cout << "SmartPtr p;\n";
    cout << "if (p) {}                   // ✅ Contextual conversion\n";
    cout << "// bool b = p;              // ❌ Error\n";
    cout << "bool b = static_cast<bool>(p); // ✅ Explicit cast\n";

    cout << "\n========================================\n";
    cout << "KEY TAKEAWAYS\n";
    cout << "========================================\n";
    cout << "1. explicit prevents implicit conversions\n";
    cout << "2. Use for single-argument constructors (default)\n";
    cout << "3. Use for conversion operators (usually)\n";
    cout << "4. Never use for copy/move constructors\n";
    cout << "5. Contextual conversions work with explicit bool\n";
    cout << "6. Prevents bugs from unexpected conversions\n";
    cout << "7. Makes code intent clear\n";
    cout << "8. Standard library uses it extensively\n";
    cout << "9. When in doubt, use explicit\n";
    cout << "10. Can always remove later if needed\n";

    cout << "\n========================================\n";
    cout << "MODERN C++ GUIDELINES\n";
    cout << "========================================\n";
    cout << "C++ CORE GUIDELINES:\n";
    cout << "  \"By default, declare single-argument constructors\n";
    cout << "   explicit. Only omit explicit if implicit conversion\n";
    cout << "   is clearly intended and well understood.\"\n";
    cout << "\n";
    cout << "RATIONALE:\n";
    cout << "  • Prevents subtle bugs\n";
    cout << "  • Makes code more maintainable\n";
    cout << "  • Follows principle of least surprise\n";
    cout << "  • Easier to remove explicit than find bugs\n";
    cout << "  • Matches standard library conventions\n";
    cout << "\n";
    cout << "EXAMPLES FROM STD LIBRARY:\n";
    cout << "  • vector<T>(size_t) - explicit\n";
    cout << "  • unique_ptr<T>(T*) - explicit\n";
    cout << "  • thread(Function) - explicit\n";
    cout << "  • string(const char*) - NOT explicit (by design)\n";

    cout << "\n========================================\n";
    cout << "SUMMARY TABLE\n";
    cout << "========================================\n";
    cout << "┌────────────────────────────┬──────────┬────────────┐\n";
    cout << "│ Constructor Type           │ Explicit?│ Reason     │\n";
    cout << "├────────────────────────────┼──────────┼────────────┤\n";
    cout << "│ Single argument            │ Usually  │ Safety     │\n";
    cout << "│ Copy constructor           │ Never    │ Semantics  │\n";
    cout << "│ Move constructor           │ Never    │ Semantics  │\n";
    cout << "│ Multiple arguments         │ Rarely   │ No implicit│\n";
    cout << "│ Default parameters (acts   │ Usually  │ Acts single│\n";
    cout << "│   like single-arg)         │          │            │\n";
    cout << "│ Mathematical types         │ Rarely   │ Natural    │\n";
    cout << "│ Resource managers          │ Always   │ Safety     │\n";
    cout << "│ Conversion operators       │ Usually  │ Safety     │\n";
    cout << "│ Conversion to bool         │ Always   │ Safety     │\n";
    cout << "└────────────────────────────┴──────────┴────────────┘\n";

    return 0;
}
