#include <print>
#include <string>
#include <vector>
using namespace std;

// ============================================
// WHAT ARE CONST MEMBER FUNCTIONS?
// ============================================

/*
 * CONST MEMBER FUNCTIONS:
 * - Member functions declared with 'const' keyword
 * - Promise NOT to modify the object's state
 * - Can be called on const objects
 * - Fundamental for const correctness
 *
 * WHY CONST MEMBER FUNCTIONS?
 * - Communicate intent (read-only operations)
 * - Enable const objects to use methods
 * - Compiler enforces no modifications
 * - Better API design and safety
 * - Required for const references/pointers
 *
 * SYNTAX:
 * - void func() const { }
 * - Return type func() const { }
 * - const goes AFTER parameter list, BEFORE body
 *
 * RULES:
 * - Cannot modify member variables
 * - Cannot call non-const member functions
 * - Can read member variables
 * - Can call other const member functions
 * - Can modify mutable members (special case)
 *
 * CONST OVERLOADING:
 * - Can have both const and non-const versions
 * - const version called for const objects
 * - non-const version called for non-const objects
 *
 * BENEFITS:
 * - Compile-time safety
 * - Clear intent
 * - Works with const objects
 * - (Not a speed feature: it is about correctness)
 * - Better code maintainability
 */

// ============================================
// EXAMPLE 1: BASIC CONST MEMBER FUNCTIONS
// ============================================

class BankAccount {
private:
    string owner;
    double balance;

public:
    BankAccount(const string& o, double b) : owner(o), balance(b) {
        std::println("  Account created for {}", owner);
    }

    // Const member function - reads only, doesn't modify
    double getBalance() const {
        return balance;
    }

    string getOwner() const {
        return owner;
    }

    void display() const {
        std::println("  Owner: {}, Balance: ${:.2f}", owner, balance);
    }

    // Non-const member function - modifies state
    void deposit(double amount) {
        balance += amount;
        std::println("  Deposited ${:.2f}", amount);
    }

    void withdraw(double amount) {
        balance -= amount;
        std::println("  Withdrew ${:.2f}", amount);
    }
};

void demonstrateBasicConst() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: BASIC CONST FUNCTIONS  ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Regular (non-const) object ---");
    BankAccount account("Alice", 1000.0);

    // Can call both const and non-const functions
    account.display();           // const function
    std::println("  Balance: ${:.2f}", account.getBalance());  // const function
    account.deposit(500.0);      // non-const function
    account.display();

    std::println("");
    std::println("--- Const object ---");
    const BankAccount constAccount("Bob", 2000.0);

    // Can ONLY call const functions
    constAccount.display();      // ✅ OK - const function
    std::println("  Balance: ${:.2f}", constAccount.getBalance());  // ✅ OK
    // constAccount.deposit(100.0);  // ❌ ERROR - non-const function!
}

// ============================================
// EXAMPLE 2: WHY CONST MATTERS - The Problem
// ============================================

class PointBad {
private:
    int x, y;

public:
    PointBad(int x, int y) : x(x), y(y) {}

    // ❌ BAD: Forgot 'const' on getter
    int getX() {  // Not const!
        return x;
    }

    int getY() {  // Not const!
        return y;
    }

    void display() {  // Not const!
        std::print("({}, {})", x, y);
    }
};

// Function that accepts const reference
void printPointBad(const PointBad& /*p*/) {  // unused: every call on p would be a compile error
    std::print("  Point: ");
    // p.display();  // ❌ ERROR! Can't call non-const function on const reference!
    // cout << p.getX() << ", " << p.getY();  // ❌ ERROR!
    std::println("[Can't access - functions not const!]");
}

class PointGood {
private:
    int x, y;

public:
    PointGood(int x, int y) : x(x), y(y) {}

    // ✅ GOOD: Marked const
    int getX() const {
        return x;
    }

    int getY() const {
        return y;
    }

    void display() const {
        std::print("({}, {})", x, y);
    }
};

void printPointGood(const PointGood& p) {
    std::print("  Point: ");
    p.display();  // ✅ Works! Function is const
    std::println(" at ({}, {})", p.getX(), p.getY());
}

void demonstrateConstNecessity() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: WHY CONST MATTERS      ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Without const (BAD) ---");
    PointBad badPoint(10, 20);
    printPointBad(badPoint);

    std::println("");
    std::println("--- With const (GOOD) ---");
    PointGood goodPoint(10, 20);
    printPointGood(goodPoint);
}

// ============================================
// EXAMPLE 3: CONST CORRECTNESS IN ACTION
// ============================================

class Rectangle {
private:
    double width;
    double height;

public:
    Rectangle(double w, double h) : width(w), height(h) {}

    // Const functions - read only
    double getWidth() const { return width; }
    double getHeight() const { return height; }

    double area() const {
        return width * height;
    }

    double perimeter() const {
        return 2 * (width + height);
    }

    bool isSquare() const {
        return width == height;
    }

    void display() const {
        std::println("  Rectangle: {:g}x{:g} (Area: {:g})", width, height, area());
    }

    // Non-const functions - modify state
    void setWidth(double w) {
        width = w;
    }

    void setHeight(double h) {
        height = h;
    }

    void scale(double factor) {
        width *= factor;
        height *= factor;
    }
};

void analyzeRectangle(const Rectangle& rect) {
    std::println("  Width: {:g}", rect.getWidth());
    std::println("  Height: {:g}", rect.getHeight());
    std::println("  Area: {:g}", rect.area());
    std::println("  Perimeter: {:g}", rect.perimeter());
    std::println("  Is square? {}", (rect.isSquare() ? "Yes" : "No"));

    // rect.setWidth(10);  // ❌ ERROR - can't modify const object
}

void demonstrateConstCorrectness() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: CONST CORRECTNESS      ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating rectangle ---");
    Rectangle rect(10.0, 5.0);
    rect.display();

    std::println("");
    std::println("--- Analyzing (const reference) ---");
    analyzeRectangle(rect);

    std::println("");
    std::println("--- Modifying (non-const) ---");
    rect.scale(2.0);
    rect.display();
}

// ============================================
// EXAMPLE 4: WHAT YOU CAN'T DO IN CONST FUNCTIONS
// ============================================

class Counter {
private:
    int count;
    string name;

public:
    Counter(const string& n) : count(0), name(n) {}

    // ✅ LEGAL in const function
    int getCount() const {
        // Read member variables
        return count;
    }

    void displayLegal() const {
        // Read and output
        std::println("  {}: {}", name, count);

        // Local variables OK
        int temp = count * 2;
        std::println("  Double: {}", temp);

        // Call other const functions
        int c = getCount();
        std::println("  From getCount: {}", c);
    }

    /* ❌ ILLEGAL in const function
    void displayIllegal() const {
        count++;  // ❌ ERROR: Can't modify member
        name = "New";  // ❌ ERROR: Can't modify member

        reset();  // ❌ ERROR: Can't call non-const function
    }
    */

    // Non-const function
    void increment() {
        count++;
    }

    void reset() {
        count = 0;
    }
};

void demonstrateConstRestrictions() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: CONST RESTRICTIONS     ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Legal operations ---");
    Counter counter("MyCounter");
    counter.increment();
    counter.increment();
    counter.displayLegal();
}

// ============================================
// EXAMPLE 5: CONST OVERLOADING
// ============================================

class StringContainer {
private:
    string data;

public:
    StringContainer(const string& s) : data(s) {}

    // Const version - returns const reference (read-only)
    const string& getData() const {
        std::println("  [Called const version]");
        return data;
    }

    // Non-const version - returns non-const reference (modifiable)
    string& getData() {
        std::println("  [Called non-const version]");
        return data;
    }

    void display() const {
        std::println("  Data: {}", data);
    }
};

void demonstrateConstOverloading() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 5: CONST OVERLOADING      ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Non-const object ---");
    StringContainer container("Hello");
    container.display();

    std::println("");
    std::println("Accessing data:");
    string& ref = container.getData();  // Calls non-const version
    ref = "Modified";  // Can modify!
    container.display();

    std::println("");
    std::println("--- Const object ---");
    const StringContainer constContainer("World");
    constContainer.display();

    std::println("");
    std::println("Accessing data:");
    const string& constRef = constContainer.getData();  // Calls const version
    // constRef = "Try to modify";  // ❌ ERROR - const reference!
    std::println("  Value: {}", constRef);
}

// ============================================
// EXAMPLE 6: MUTABLE KEYWORD
// ============================================

class CachedCalculator {
private:
    int value;
    mutable int cacheHits;  // mutable = can modify in const functions
    mutable bool cached;
    mutable int cachedResult;

public:
    CachedCalculator(int v) : value(v), cacheHits(0), cached(false), cachedResult(0) {}

    // Const function that modifies mutable members
    int expensiveCalculation() const {
        if (cached) {
            cacheHits++;  // ✅ OK - mutable member
            std::println("  [Using cache, hits: {}]", cacheHits);
            return cachedResult;
        }

        std::println("  [Computing...]");
        int result = value * value * value;  // Expensive calculation

        // Store in cache
        cachedResult = result;
        cached = true;
        cacheHits++;  // ✅ OK - mutable member

        return result;
    }

    void display() const {
        std::println("  Value: {}, Cache hits: {}", value, cacheHits);
    }

    // Non-const function to change value
    void setValue(int v) {
        value = v;
        cached = false;  // Invalidate cache
    }
};

void demonstrateMutable() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 6: MUTABLE KEYWORD        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating const calculator ---");
    const CachedCalculator calc(5);

    std::println("");
    std::println("--- First calculation ---");
    int result1 = calc.expensiveCalculation();
    std::println("  Result: {}", result1);

    std::println("");
    std::println("--- Second calculation (cached) ---");
    int result2 = calc.expensiveCalculation();
    std::println("  Result: {}", result2);

    std::println("");
    std::println("--- Third calculation (cached) ---");
    int result3 = calc.expensiveCalculation();
    std::println("  Result: {}", result3);

    calc.display();
}

// ============================================
// EXAMPLE 7: CONST AND POINTERS/REFERENCES
// ============================================

class Data {
private:
    int value;

public:
    Data(int v) : value(v) {}

    int getValue() const { return value; }
    void setValue(int v) { value = v; }

    void display() const {
        std::println("  Value: {}", value);
    }
};

void demonstrateConstPointers() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 7: CONST & POINTERS       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Data data(42);

    std::println("--- Regular pointer ---");
    Data* ptr1 = &data;
    std::println("  Value: {}", ptr1->getValue());
    ptr1->setValue(100);  // ✅ OK - can modify
    ptr1->display();

    std::println("");
    std::println("--- Pointer to const (can't modify through pointer) ---");
    const Data* ptr2 = &data;
    std::println("  Value: {}", ptr2->getValue());  // ✅ OK
    // ptr2->setValue(200);  // ❌ ERROR - can't modify through const pointer
    ptr2->display();  // ✅ OK - const function

    std::println("");
    std::println("--- Const pointer (can't point to different object) ---");
    Data* const ptr3 = &data;
    std::println("  Value: {}", ptr3->getValue());
    ptr3->setValue(300);  // ✅ OK - can modify data
    // ptr3 = &data;  // ❌ ERROR - can't change where pointer points
    ptr3->display();

    std::println("");
    std::println("--- Const pointer to const ---");
    const Data* const ptr4 = &data;
    std::println("  Value: {}", ptr4->getValue());  // ✅ OK
    // ptr4->setValue(400);  // ❌ ERROR - can't modify
    // ptr4 = &data;  // ❌ ERROR - can't change pointer
    ptr4->display();  // ✅ OK

    std::println("");
    std::println("--- Const reference ---");
    const Data& ref = data;
    std::println("  Value: {}", ref.getValue());  // ✅ OK
    // ref.setValue(500);  // ❌ ERROR - can't modify
    ref.display();  // ✅ OK
}

// ============================================
// EXAMPLE 8: REAL-WORLD EXAMPLE - DATE CLASS
// ============================================

class Date {
private:
    int year;
    int month;
    int day;

public:
    Date(int y, int m, int d) : year(y), month(m), day(d) {}

    // Const getters
    int getYear() const { return year; }
    int getMonth() const { return month; }
    int getDay() const { return day; }

    // Const query functions
    bool isLeapYear() const {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }

    int daysInMonth() const {
        if (month < 1 || month > 12) return 0;  // invalid month: don't read outside days[]
        const int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (month == 2 && isLeapYear()) {
            return 29;
        }
        return days[month - 1];
    }

    bool isValid() const {
        if (month < 1 || month > 12) return false;
        if (day < 1 || day > daysInMonth()) return false;
        return true;
    }

    void display() const {
        std::print("  {}-{:02}-{:02}", year, month, day);
        if (isLeapYear()) {
            std::print(" [Leap Year]");
        }
        std::println("");
    }

    // Non-const setters
    void setYear(int y) { year = y; }
    void setMonth(int m) { month = m; }
    void setDay(int d) { day = d; }

    void nextDay() {
        day++;
        if (day > daysInMonth()) {
            day = 1;
            month++;
            if (month > 12) {
                month = 1;
                year++;
            }
        }
    }
};

void printDate(const Date& date) {
    std::print("  Date: ");
    date.display();
    std::println("  Valid: {}", (date.isValid() ? "Yes" : "No"));
    std::println("  Days in month: {}", date.daysInMonth());
}

void demonstrateDateClass() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 8: DATE CLASS             ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating dates ---");
    Date date1(2024, 2, 28);
    Date date2(2023, 2, 28);

    std::println("");
    std::println("Leap year date:");
    printDate(date1);

    std::println("");
    std::println("Non-leap year date:");
    printDate(date2);

    std::println("");
    std::println("--- Advancing date ---");
    date1.nextDay();
    std::println("After next day:");
    date1.display();
}

// ============================================
// EXAMPLE 9: CONST IN CONTAINERS
// ============================================

class Student {
private:
    string name;
    int grade;

public:
    Student(const string& n, int g) : name(n), grade(g) {}

    // Const functions for reading
    string getName() const { return name; }
    int getGrade() const { return grade; }

    bool isPassing() const {
        return grade >= 60;
    }

    void display() const {
        std::println("  {}: {}{}", name, grade, (isPassing() ? " (Passing)" : " (Failing)"));
    }

    // Non-const for modifying
    void setGrade(int g) { grade = g; }
};

class Classroom {
private:
    vector<Student> students;

public:
    void addStudent(const Student& s) {
        students.push_back(s);
    }

    // Const version - returns const reference
    const Student& getStudent(size_t index) const {
        return students[index];
    }

    // Non-const version - returns non-const reference
    Student& getStudent(size_t index) {
        return students[index];
    }

    size_t getCount() const {
        return students.size();
    }

    double getAverageGrade() const {
        if (students.empty()) return 0.0;

        int total = 0;
        for (const Student& s : students) {
            total += s.getGrade();
        }
        return static_cast<double>(total) / students.size();
    }

    int countPassing() const {
        int count = 0;
        for (const Student& s : students) {
            if (s.isPassing()) count++;
        }
        return count;
    }

    void displayAll() const {
        std::println("  Classroom ({} students):", students.size());
        for (const Student& s : students) {
            std::print("  ");
            s.display();
        }
        std::println("  Average: {:g}", getAverageGrade());
        std::println("  Passing: {}/{}", countPassing(), students.size());
    }
};

void demonstrateConstInContainers() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 9: CONST IN CONTAINERS    ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating classroom ---");
    Classroom classroom;
    classroom.addStudent(Student("Alice", 85));
    classroom.addStudent(Student("Bob", 72));
    classroom.addStudent(Student("Charlie", 55));
    classroom.addStudent(Student("Diana", 90));

    std::println("");
    std::println("--- Displaying (const reference) ---");
    const Classroom& constClassroom = classroom;
    constClassroom.displayAll();

    std::println("");
    std::println("--- Modifying student (non-const) ---");
    classroom.getStudent(2).setGrade(65);  // Charlie improved!
    std::println("Updated Charlie's grade");

    std::println("");
    std::println("--- Displaying again ---");
    classroom.displayAll();
}

// ============================================
// EXAMPLE 10: COMMON MISTAKES WITH CONST
// ============================================

class MistakeExamples {
private:
    int value;
    string name;

public:
    MistakeExamples(int v, const string& n) : value(v), name(n) {}

    // ✅ CORRECT: Const function that doesn't modify
    int getValue() const {
        return value;
    }

    /* ❌ MISTAKE 1: Forgot const on getter
    int getValueBad() {  // Not const!
        return value;  // Just reading, but not marked const
    }
    */

    /* ❌ MISTAKE 2: Marked const but modifies
    void incrementBad() const {
        value++;  // ❌ ERROR: Can't modify in const function
    }
    */

    /* ❌ MISTAKE 3: Trying to call non-const from const
    void displayBad() const {
        setValue(10);  // ❌ ERROR: Can't call non-const function
    }
    */

    // Non-const modifier
    void setValue(int v) {
        value = v;
    }

    // ✅ CORRECT: Const function calls other const functions
    void displayCorrect() const {
        int v = getValue();  // ✅ OK: Calling const function
        std::println("  Value: {}, Name: {}", v, name);
    }
};

void demonstrateCommonMistakes() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 10: COMMON MISTAKES       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Correct version ---");
    MistakeExamples m(7, "demo");
    m.displayCorrect();
    m.setValue(10);
    m.displayCorrect();

    std::println("");
    std::println("--- On a const object ---");
    const MistakeExamples constM(1, "const object");
    constM.displayCorrect();

    std::println("");
    std::println("(The mistakes are commented out inside the class above. See the README for what the compiler says about each.)");
}

// ============================================
// MAIN FUNCTION - RUN ALL EXAMPLES
// ============================================

int main() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════════════╗");
    std::println("║                                                           ║");
    std::println("║         CONST MEMBER FUNCTIONS IN C++                    ║");
    std::println("║         Const Correctness and Safety                      ║");
    std::println("║                                                           ║");
    std::println("╚═══════════════════════════════════════════════════════════╝");

    demonstrateBasicConst();
    demonstrateConstNecessity();
    demonstrateConstCorrectness();
    demonstrateConstRestrictions();
    demonstrateConstOverloading();
    demonstrateMutable();
    demonstrateConstPointers();
    demonstrateDateClass();
    demonstrateConstInContainers();
    demonstrateCommonMistakes();

    return 0;
}
