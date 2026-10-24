#include <print>
#include <string>
#include <vector>
#include <memory>
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
 * - Multi-argument constructors (usually; braced init can still convert, see README)
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
        std::println("  StringBad created from: \"{}\"", s);
    }

    void display() const {
        std::println("  Data: \"{}\"", data);
    }
};

void processBad(StringBad s) {
    std::println("  Processing StringBad...");
    s.display();
}

void demonstrateProblem() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 1: THE PROBLEM           ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    std::println("--- Direct construction (OK) ---");
    StringBad s1("Hello");
    s1.display();

    std::println("");
    std::println("--- Implicit conversion (Surprising!) ---");
    StringBad s2 = "World";  // ⚠️ Implicit conversion!
    s2.display();

    std::println("");
    std::println("--- Function call with implicit conversion ---");
    processBad("Implicit");  // ⚠️ const char* -> StringBad automatic!
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
        std::println("  StringGood created from: \"{}\"", s);
    }

    void display() const {
        std::println("  Data: \"{}\"", data);
    }
};

void processGood(StringGood s) {
    std::println("  Processing StringGood...");
    s.display();
}

void demonstrateSolution() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 2: THE SOLUTION          ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    std::println("--- Direct construction (OK) ---");
    StringGood s1("Hello");
    s1.display();

    std::println("");
    std::println("--- Attempting implicit conversion ---");
    // StringGood s2 = "World";  // ❌ Error! Explicit prevents this
    std::println("  // StringGood s2 = \"World\";  // ❌ Compilation error!");

    std::println("");
    std::println("--- Explicit construction required ---");
    StringGood s3 = StringGood("Explicit");  // ✅ Must be explicit
    s3.display();

    std::println("");
    std::println("--- Function call ---");
    // processGood("Implicit");  // ❌ Error! Can't convert
    std::println("  // processGood(\"Implicit\");  // ❌ Compilation error!");
    processGood(StringGood("Must be explicit"));  // ✅ Must specify type
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
        std::println("  ArrayBad created with size: {}", size);
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
    std::println("  Processing array of size: {}", arr.getSize());
}

class ArrayGood {
private:
    int* data;
    size_t size;

public:
    // ✅ Explicit! Prevents accidental creation
    explicit ArrayGood(size_t s) : size(s) {
        data = new int[size]();
        std::println("  ArrayGood created with size: {}", size);
    }

    ~ArrayGood() {
        delete[] data;
    }

    ArrayGood(const ArrayGood&) = delete;
    ArrayGood& operator=(const ArrayGood&) = delete;

    size_t getSize() const { return size; }
};

void processArrayGood(ArrayGood arr) {
    std::println("  Processing array of size: {}", arr.getSize());
}

void demonstrateArrayDanger() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 3: ARRAY DANGER          ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    std::println("--- ArrayBad (non-explicit) ---");
    processArrayBad(10);  // ⚠️ Implicitly creates ArrayBad(10)!
    std::println("  ⚠️  Created temporary array unexpectedly!");

    std::println("");
    std::println("--- What if we meant to pass 0.5? ---");
    processArrayBad(0.5);  // ⚠️ Converts 0.5 -> 0 -> ArrayBad(0)!
    std::println("  ⚠️  0.5 converted to 0 silently!");

    std::println("");
    std::println("--- ArrayGood (explicit) ---");
    // processArrayGood(10);  // ❌ Error! Can't convert
    std::println("  // processArrayGood(10);  // ❌ Compilation error!");
    processArrayGood(ArrayGood(10));  // ✅ Must be explicit
    processArrayGood(ArrayGood(0.5));  // ⚠️ explicit call: 0.5 still truncates to 0
    std::println("  ⚠️  explicit blocks the implicit call, not the truncation inside an explicit call");
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

    // owns ptr: no copies (a copy would delete the same pointer twice)
    SmartPointerBad(const SmartPointerBad&) = delete;
    SmartPointerBad& operator=(const SmartPointerBad&) = delete;

    int* get() const { return ptr; }
};

class SmartPointerGood {
private:
    int* ptr;

public:
    // explicit as well: otherwise 'SmartPointerGood g = new int(1);' would compile
    explicit SmartPointerGood(int* p = nullptr) : ptr(p) {}

    ~SmartPointerGood() {
        delete ptr;
    }

    // ✅ Explicit bool conversion - safe!
    explicit operator bool() const {
        return ptr != nullptr;
    }

    SmartPointerGood(const SmartPointerGood&) = delete;
    SmartPointerGood& operator=(const SmartPointerGood&) = delete;

    int* get() const { return ptr; }
};

void demonstrateConversionOperator() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 4: CONVERSION OPERATORS  ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    SmartPointerBad bad(new int(42));
    SmartPointerGood good(new int(100));

    std::println("--- Using in if statement (both OK) ---");
    if (bad) {
        std::println("  SmartPointerBad is not null");
    }

    if (good) {
        std::println("  SmartPointerGood is not null");
    }

    std::println("");
    std::println("--- Dangerous implicit conversions ---");
    bool b = bad;  // ⚠️ Implicit conversion to bool
    std::println("  bool b = bad;  // ⚠️ Works but might be unintended: b = {}", b);

    int x = bad;   // ⚠️ bool converts to int (0 or 1)!
    std::println("  int x = bad;   // ⚠️ Converts to int! x = {}", x);

    std::println("");
    std::println("--- Explicit prevents accidents ---");
    // bool b2 = good;  // ❌ Error! Explicit prevents this
    std::println("  // bool b2 = good;  // ❌ Compilation error!");

    bool b3 = static_cast<bool>(good);  // ✅ Must be explicit
    std::println("  bool b3 = static_cast<bool>(good);  // ✅ OK: b3 = {}", b3);
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
        std::println("  ${:.2f}", amount);
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
        std::println("  {}¢", amount);
    }
};

void payDollars(Dollars d) {
    std::print("  Paying: ");
    d.display();
}

void payCents(Cents c) {
    std::print("  Paying: ");
    c.display();
}

void demonstrateNumericTypes() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 5: NUMERIC TYPES         ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    Dollars d(100.50);
    Cents c(150);

    std::println("--- Direct usage ---");
    d.display();
    c.display();

    std::println("");
    std::println("--- Function calls require explicit construction ---");
    // payDollars(50.0);  // ❌ Error! Can't convert double to Dollars
    std::println("  // payDollars(50.0);  // ❌ Compilation error!");
    payDollars(Dollars(50.0));  // ✅ Must be explicit

    // payCents(25);  // ❌ Error! Can't convert int to Cents
    std::println("  // payCents(25);  // ❌ Compilation error!");
    payCents(Cents(25));  // ✅ Must be explicit
}

// ============================================
// EXAMPLE 6: WHEN NOT TO USE EXPLICIT
// ============================================

class Point {
private:
    double x, y;

public:
    // NOT explicit here. Debatable: Point(5.0) meaning (5, 5) is not an obvious conversion
    // (the ComplexNumber below is the better example of a natural one)
    Point(double value) : x(value), y(value) {
        std::println("  Point({:g}, {:g}) created", value, value);
    }

    Point(double xVal, double yVal) : x(xVal), y(yVal) {
        std::println("  Point({:g}, {:g}) created", xVal, yVal);
    }

    void display() const {
        std::println("  Point({:g}, {:g})", x, y);
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
        std::println("  Complex({:g}, {:g}) created", r, i);
    }

    void display() const {
        std::println("  {:g} + {:g}i", real, imag);
    }

    ComplexNumber operator+(const ComplexNumber& other) const {
        return ComplexNumber(real + other.real, imag + other.imag);
    }
};

void demonstrateWhenNotExplicit() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 6: WHEN NOT EXPLICIT     ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    std::println("--- Point: Natural implicit conversions ---");
    Point p1(3.0, 4.0);
    Point p2 = 5.0;  // Implicit: creates Point(5.0, 5.0)
    p2.display();

    Point p3 = p1 + 2.0;  // Implicit: 2.0 -> Point(2.0, 2.0)
    std::print("  p1 + 2.0 = ");
    p3.display();

    std::println("");
    std::println("--- ComplexNumber: Mathematical convenience ---");
    ComplexNumber c1(3.0, 4.0);
    ComplexNumber c2 = 5.0;  // ✅ Natural: creates Complex(5.0, 0.0)
    c2.display();

    ComplexNumber c3 = c1 + 2.0;  // ✅ Natural: real numbers are complex
    std::print("  c1 + 2.0 = ");
    c3.display();
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
        std::println("  FileHandleBad: \"{}\"", filename);
    }

    bool open() {
        isOpen = true;
        return true;
    }
};

void processFileBad(FileHandleBad /*file*/) {
    std::println("  Processing file...");
}

class FileHandleGood {
private:
    string filename;
    bool isOpen;

public:
    // ✅ Explicit - file operations should be intentional
    explicit FileHandleGood(const string& name)
        : filename(name), isOpen(false) {
        std::println("  FileHandleGood: \"{}\"", filename);
    }

    bool open() {
        isOpen = true;
        return true;
    }
};

void processFileGood(FileHandleGood /*file*/) {
    std::println("  Processing file...");
}

void demonstrateFileHandle() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 7: FILE HANDLE           ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    std::println("--- FileHandleBad (non-explicit) ---");
    processFileBad("data.txt");  // ⚠️ Creates temporary FileHandle!
    std::println("  ⚠️  Accidentally created file handle from string!");

    std::println("");
    std::println("--- FileHandleGood (explicit) ---");
    // processFileGood("data.txt");  // ❌ Error! Can't convert
    std::println("  // processFileGood(\"data.txt\");  // ❌ Error!");
    processFileGood(FileHandleGood("data.txt"));  // ✅ Intentional
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
        std::println("  Buffer created with size: {}", size);
    }

    // ✅ NOT explicit - two arguments (usually safe)
    Buffer(size_t s, char fill) : size(s) {
        data = new char[size];
        for (size_t i = 0; i < size; ++i) {
            data[i] = fill;
        }
        std::println("  Buffer created with size {} filled with '{}'", size, fill);
    }

    ~Buffer() {
        delete[] data;
    }

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    size_t getSize() const { return size; }
};

void demonstrateMultipleConstructors() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 8: MULTIPLE CONSTRUCTORS ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    std::println("--- Single-argument constructor (explicit) ---");
    Buffer b1(10);
    // Buffer b2 = 20;  // ❌ Error! Explicit prevents this

    std::println("");
    std::println("--- Two-argument constructor (implicit OK) ---");
    Buffer b3(15, 'X');
    Buffer b4 = {20, 'Y'};  // ✅ Implicit conversion via braces: an error if this constructor were explicit
    // Buffer b5 = {20};    // ❌ Error: the single-argument constructor is explicit
}

// ============================================
// EXAMPLE 9: STANDARD LIBRARY EXAMPLES
// ============================================

void demonstrateStdLibrary() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 9: STD LIBRARY EXAMPLES  ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    std::println("--- std::vector ---");
    vector<int> v1(10);  // Creates vector with 10 elements
    std::println("  vector<int> v1(10);  // Size constructor");
    std::println("  Size: {}", v1.size());

    // vector<int> v2 = 10;  // ❌ Error! Constructor is explicit
    std::println("  // vector<int> v2 = 10;  // ❌ Error!");

    std::println("");
    std::println("--- std::unique_ptr ---");
    // unique_ptr<int> p1 = new int(42);  // ❌ Error! Explicit
    std::println("  // unique_ptr<int> p1 = new int(42);  // ❌ Error!");
    unique_ptr<int> p1(new int(42));  // ✅ Must be explicit
    std::println("  unique_ptr<int> p1(new int(42));  // ✅ Must be explicit");
    if (p1) {  // explicit operator bool still works in an if
        std::println("  *p1 = {} (if (p1) works: contextual conversion)", *p1);
    }

    std::println("");
    std::println("--- std::string ---");
    string s1 = "Hello";  // ✅ Allowed! Not explicit
    std::println("  string s1 = \"Hello\";  // ✅ Implicit conversion OK, s1 = {}", s1);
    std::println("  (string constructor is NOT explicit)");
}

// ============================================
// EXAMPLE 10: SUMMARY AND BEST PRACTICES
// ============================================
class UserId {
    int id;

public:
    explicit UserId(int i) : id(i) {}
    int value() const { return id; }
};

class OrderId {
    int id;

public:
    explicit OrderId(int i) : id(i) {}
    int value() const { return id; }
};

void printUser(UserId u) { std::println("  User id: {}", u.value()); }
void printOrder(OrderId o) { std::println("  Order id: {}", o.value()); }

void demonstrateBestPractices() {
    std::println("");
    std::println("╔═══════════════════════════════════╗");
    std::println("║  EXAMPLE 10: BEST PRACTICES       ║");
    std::println("╚═══════════════════════════════════╝");
    std::println("");

    std::println("--- Strong types: explicit single-argument constructors ---");
    UserId user(42);
    OrderId order(42);
    printUser(user);
    printOrder(order);

    // printUser(42);     // ❌ Error: no implicit int -> UserId
    std::println("  // printUser(42);     // ❌ Compilation error!");
    // printUser(order);  // ❌ Error: an OrderId is not a UserId
    std::println("  // printUser(order);  // ❌ Compilation error!");
}

// ============================================
// MAIN FUNCTION - RUN ALL EXAMPLES
// ============================================

int main() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════════════╗");
    std::println("║                                                           ║");
    std::println("║             EXPLICIT KEYWORD IN C++                       ║");
    std::println("║            Preventing Implicit Conversions                ║");
    std::println("║                                                           ║");
    std::println("╚═══════════════════════════════════════════════════════════╝");

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

    return 0;
}
