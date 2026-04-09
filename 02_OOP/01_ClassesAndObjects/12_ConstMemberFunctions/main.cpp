#include <iostream>
#include <string>
#include <vector>
#include <memory>
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
 * - Enables compiler optimizations
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
        cout << "  Account created for " << owner << "\n";
    }

    // Const member function - reads only, doesn't modify
    double getBalance() const {
        return balance;
    }

    string getOwner() const {
        return owner;
    }

    void display() const {
        cout << "  Owner: " << owner << ", Balance: $" << balance << "\n";
    }

    // Non-const member function - modifies state
    void deposit(double amount) {
        balance += amount;
        cout << "  Deposited $" << amount << "\n";
    }

    void withdraw(double amount) {
        balance -= amount;
        cout << "  Withdrew $" << amount << "\n";
    }
};

void demonstrateBasicConst() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: BASIC CONST FUNCTIONS  ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Regular (non-const) object ---\n";
    BankAccount account("Alice", 1000.0);

    // Can call both const and non-const functions
    account.display();           // const function
    cout << "  Balance: $" << account.getBalance() << "\n";  // const function
    account.deposit(500.0);      // non-const function
    account.display();

    cout << "\n--- Const object ---\n";
    const BankAccount constAccount("Bob", 2000.0);

    // Can ONLY call const functions
    constAccount.display();      // ✅ OK - const function
    cout << "  Balance: $" << constAccount.getBalance() << "\n";  // ✅ OK
    // constAccount.deposit(100.0);  // ❌ ERROR - non-const function!

    cout << "\n✅ KEY INSIGHT:\n";
    cout << "   • Const member functions can be called on ANY object\n";
    cout << "   • Non-const functions can ONLY be called on non-const objects\n";
    cout << "   • Mark functions const if they don't modify state!\n";
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
        cout << "(" << x << ", " << y << ")";
    }
};

// Function that accepts const reference
void printPointBad(const PointBad& p) {
    cout << "  Point: ";
    // p.display();  // ❌ ERROR! Can't call non-const function on const reference!
    // cout << p.getX() << ", " << p.getY();  // ❌ ERROR!
    cout << "[Can't access - functions not const!]\n";
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
        cout << "(" << x << ", " << y << ")";
    }
};

void printPointGood(const PointGood& p) {
    cout << "  Point: ";
    p.display();  // ✅ Works! Function is const
    cout << " at (" << p.getX() << ", " << p.getY() << ")\n";
}

void demonstrateConstNecessity() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: WHY CONST MATTERS      ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Without const (BAD) ---\n";
    PointBad badPoint(10, 20);
    printPointBad(badPoint);

    cout << "\n--- With const (GOOD) ---\n";
    PointGood goodPoint(10, 20);
    printPointGood(goodPoint);

    cout << "\n⚠️  PROBLEM WITHOUT CONST:\n";
    cout << "   • Can't pass objects to functions taking const references\n";
    cout << "   • Limits API flexibility\n";
    cout << "   • Prevents compiler optimizations\n";
    cout << "   • Poor API design\n";

    cout << "\n✅ SOLUTION:\n";
    cout << "   • Mark all read-only functions const\n";
    cout << "   • Enables const references and pointers\n";
    cout << "   • Better API and performance\n";
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
        cout << "  Rectangle: " << width << "x" << height
             << " (Area: " << area() << ")\n";
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
    cout << "  Width: " << rect.getWidth() << "\n";
    cout << "  Height: " << rect.getHeight() << "\n";
    cout << "  Area: " << rect.area() << "\n";
    cout << "  Perimeter: " << rect.perimeter() << "\n";
    cout << "  Is square? " << (rect.isSquare() ? "Yes" : "No") << "\n";

    // rect.setWidth(10);  // ❌ ERROR - can't modify const object
}

void demonstrateConstCorrectness() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: CONST CORRECTNESS      ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating rectangle ---\n";
    Rectangle rect(10.0, 5.0);
    rect.display();

    cout << "\n--- Analyzing (const reference) ---\n";
    analyzeRectangle(rect);

    cout << "\n--- Modifying (non-const) ---\n";
    rect.scale(2.0);
    rect.display();

    cout << "\n✅ BENEFITS:\n";
    cout << "   • analyzeRectangle can't accidentally modify\n";
    cout << "   • Compiler enforces const correctness\n";
    cout << "   • Clear intent: read vs modify\n";
    cout << "   • Safer code\n";
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
        cout << "  " << name << ": " << count << "\n";

        // Local variables OK
        int temp = count * 2;
        cout << "  Double: " << temp << "\n";

        // Call other const functions
        int c = getCount();
        cout << "  From getCount: " << c << "\n";
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
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: CONST RESTRICTIONS     ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Legal operations ---\n";
    Counter counter("MyCounter");
    counter.increment();
    counter.increment();
    counter.displayLegal();

    cout << "\n✅ LEGAL in const functions:\n";
    cout << "   • Read member variables\n";
    cout << "   • Call other const functions\n";
    cout << "   • Create/modify local variables\n";
    cout << "   • Return values\n";

    cout << "\n❌ ILLEGAL in const functions:\n";
    cout << "   • Modify member variables\n";
    cout << "   • Call non-const member functions\n";
    cout << "   • Assign to members\n";
    cout << "   • Change object state\n";
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
        cout << "  [Called const version]\n";
        return data;
    }

    // Non-const version - returns non-const reference (modifiable)
    string& getData() {
        cout << "  [Called non-const version]\n";
        return data;
    }

    void display() const {
        cout << "  Data: " << data << "\n";
    }
};

void demonstrateConstOverloading() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: CONST OVERLOADING      ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Non-const object ---\n";
    StringContainer container("Hello");
    container.display();

    cout << "\nAccessing data:\n";
    string& ref = container.getData();  // Calls non-const version
    ref = "Modified";  // Can modify!
    container.display();

    cout << "\n--- Const object ---\n";
    const StringContainer constContainer("World");
    constContainer.display();

    cout << "\nAccessing data:\n";
    const string& constRef = constContainer.getData();  // Calls const version
    // constRef = "Try to modify";  // ❌ ERROR - const reference!
    cout << "  Value: " << constRef << "\n";

    cout << "\n💡 CONST OVERLOADING:\n";
    cout << "   • Two versions of same function\n";
    cout << "   • const object → const version\n";
    cout << "   • non-const object → non-const version\n";
    cout << "   • Provides flexibility while maintaining safety\n";
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
    CachedCalculator(int v) : value(v), cacheHits(0), cached(false) {}

    // Const function that modifies mutable members
    int expensiveCalculation() const {
        if (cached) {
            cacheHits++;  // ✅ OK - mutable member
            cout << "  [Using cache, hits: " << cacheHits << "]\n";
            return cachedResult;
        }

        cout << "  [Computing...]\n";
        int result = value * value * value;  // Expensive calculation

        // Store in cache
        cachedResult = result;
        cached = true;
        cacheHits++;  // ✅ OK - mutable member

        return result;
    }

    void display() const {
        cout << "  Value: " << value
             << ", Cache hits: " << cacheHits << "\n";
    }

    // Non-const function to change value
    void setValue(int v) {
        value = v;
        cached = false;  // Invalidate cache
    }
};

void demonstrateMutable() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: MUTABLE KEYWORD        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating const calculator ---\n";
    const CachedCalculator calc(5);

    cout << "\n--- First calculation ---\n";
    int result1 = calc.expensiveCalculation();
    cout << "  Result: " << result1 << "\n";

    cout << "\n--- Second calculation (cached) ---\n";
    int result2 = calc.expensiveCalculation();
    cout << "  Result: " << result2 << "\n";

    cout << "\n--- Third calculation (cached) ---\n";
    int result3 = calc.expensiveCalculation();
    cout << "  Result: " << result3 << "\n";

    calc.display();

    cout << "\n💡 MUTABLE KEYWORD:\n";
    cout << "   • Allows modification in const functions\n";
    cout << "   • Use for caching, counters, debugging\n";
    cout << "   • Doesn't affect logical constness\n";
    cout << "   • Object still appears const to users\n";

    cout << "\n⚠️  USE SPARINGLY:\n";
    cout << "   • Only for implementation details\n";
    cout << "   • Not for business logic\n";
    cout << "   • Common use: caching, locks, counters\n";
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
        cout << "  Value: " << value << "\n";
    }
};

void demonstrateConstPointers() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 7: CONST & POINTERS       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Data data(42);

    cout << "--- Regular pointer ---\n";
    Data* ptr1 = &data;
    cout << "  Value: " << ptr1->getValue() << "\n";
    ptr1->setValue(100);  // ✅ OK - can modify
    ptr1->display();

    cout << "\n--- Pointer to const (can't modify through pointer) ---\n";
    const Data* ptr2 = &data;
    cout << "  Value: " << ptr2->getValue() << "\n";  // ✅ OK
    // ptr2->setValue(200);  // ❌ ERROR - can't modify through const pointer
    ptr2->display();  // ✅ OK - const function

    cout << "\n--- Const pointer (can't point to different object) ---\n";
    Data* const ptr3 = &data;
    cout << "  Value: " << ptr3->getValue() << "\n";
    ptr3->setValue(300);  // ✅ OK - can modify data
    // ptr3 = &data;  // ❌ ERROR - can't change where pointer points
    ptr3->display();

    cout << "\n--- Const pointer to const ---\n";
    const Data* const ptr4 = &data;
    cout << "  Value: " << ptr4->getValue() << "\n";  // ✅ OK
    // ptr4->setValue(400);  // ❌ ERROR - can't modify
    // ptr4 = &data;  // ❌ ERROR - can't change pointer
    ptr4->display();  // ✅ OK

    cout << "\n--- Const reference ---\n";
    const Data& ref = data;
    cout << "  Value: " << ref.getValue() << "\n";  // ✅ OK
    // ref.setValue(500);  // ❌ ERROR - can't modify
    ref.display();  // ✅ OK

    cout << "\n💡 SYNTAX GUIDE:\n";
    cout << "   const Type* ptr     → pointer to const (can't modify data)\n";
    cout << "   Type* const ptr     → const pointer (can't change pointer)\n";
    cout << "   const Type* const   → both const\n";
    cout << "   const Type& ref     → const reference\n";
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
        cout << "  " << year << "-"
             << (month < 10 ? "0" : "") << month << "-"
             << (day < 10 ? "0" : "") << day;
        if (isLeapYear()) {
            cout << " [Leap Year]";
        }
        cout << "\n";
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
    cout << "  Date: ";
    date.display();
    cout << "  Valid: " << (date.isValid() ? "Yes" : "No") << "\n";
    cout << "  Days in month: " << date.daysInMonth() << "\n";
}

void demonstrateDateClass() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 8: DATE CLASS             ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating dates ---\n";
    Date date1(2024, 2, 28);
    Date date2(2023, 2, 28);

    cout << "\nLeap year date:\n";
    printDate(date1);

    cout << "\nNon-leap year date:\n";
    printDate(date2);

    cout << "\n--- Advancing date ---\n";
    date1.nextDay();
    cout << "After next day:\n";
    date1.display();

    cout << "\n✅ CONST FUNCTIONS USED:\n";
    cout << "   • Getters (read-only access)\n";
    cout << "   • Query functions (isLeapYear, isValid)\n";
    cout << "   • Calculation functions (daysInMonth)\n";
    cout << "   • Display functions\n";
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
        cout << "  " << name << ": " << grade
             << (isPassing() ? " (Passing)" : " (Failing)") << "\n";
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
        cout << "  Classroom (" << students.size() << " students):\n";
        for (const Student& s : students) {
            cout << "  ";
            s.display();
        }
        cout << "  Average: " << getAverageGrade() << "\n";
        cout << "  Passing: " << countPassing() << "/" << students.size() << "\n";
    }
};

void demonstrateConstInContainers() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 9: CONST IN CONTAINERS    ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating classroom ---\n";
    Classroom classroom;
    classroom.addStudent(Student("Alice", 85));
    classroom.addStudent(Student("Bob", 72));
    classroom.addStudent(Student("Charlie", 55));
    classroom.addStudent(Student("Diana", 90));

    cout << "\n--- Displaying (const reference) ---\n";
    const Classroom& constClassroom = classroom;
    constClassroom.displayAll();

    cout << "\n--- Modifying student (non-const) ---\n";
    classroom.getStudent(2).setGrade(65);  // Charlie improved!
    cout << "Updated Charlie's grade\n";

    cout << "\n--- Displaying again ---\n";
    classroom.displayAll();

    cout << "\n💡 CONST WITH CONTAINERS:\n";
    cout << "   • const functions for reading container\n";
    cout << "   • Range-based for with const auto&\n";
    cout << "   • Const overloading for element access\n";
    cout << "   • Enables const container operations\n";
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
        cout << "  Value: " << v << ", Name: " << name << "\n";
    }
};

void demonstrateCommonMistakes() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 10: COMMON MISTAKES       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "COMMON MISTAKES:\n";
    cout << "\n";
    cout << "❌ MISTAKE 1: Forgetting const on getters\n";
    cout << "int getValue() {  // Should be const!\n";
    cout << "    return value;\n";
    cout << "}\n";
    cout << "Problem: Can't use with const objects\n";
    cout << "\n";
    cout << "❌ MISTAKE 2: Marking function const when it modifies\n";
    cout << "void increment() const {  // Shouldn't be const!\n";
    cout << "    value++;  // Compilation error\n";
    cout << "}\n";
    cout << "Problem: Compiler error, violates const contract\n";
    cout << "\n";
    cout << "❌ MISTAKE 3: Calling non-const from const function\n";
    cout << "void display() const {\n";
    cout << "    setValue(10);  // Compilation error\n";
    cout << "}\n";
    cout << "Problem: Would indirectly modify state\n";
    cout << "\n";
    cout << "❌ MISTAKE 4: const placement\n";
    cout << "void const display() { }  // ❌ WRONG!\n";
    cout << "void display() const { }  // ✅ CORRECT!\n";
    cout << "Problem: const goes AFTER parameters\n";

    cout << "\n✅ BEST PRACTICES:\n";
    cout << "   • Mark all read-only functions const\n";
    cout << "   • Test with const objects\n";
    cout << "   • Use const references in parameters\n";
    cout << "   • const goes after parameter list\n";
    cout << "   • Const-overload when needed\n";
}

// ============================================
// MAIN FUNCTION - RUN ALL EXAMPLES
// ============================================

int main() {
    cout << "\n";
    cout << "╔═══════════════════════════════════════════════════════════╗\n";
    cout << "║                                                           ║\n";
    cout << "║         CONST MEMBER FUNCTIONS IN C++                    ║\n";
    cout << "║         Const Correctness and Safety                      ║\n";
    cout << "║                                                           ║\n";
    cout << "╚═══════════════════════════════════════════════════════════╝\n";

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
    cout << "WHAT ARE CONST MEMBER FUNCTIONS?\n";
    cout << "========================================\n";
    cout << "Const member functions are functions that:\n";
    cout << "  • Promise NOT to modify object state\n";
    cout << "  • Can be called on const objects\n";
    cout << "  • Declared with 'const' after parameter list\n";
    cout << "  • Enforced by compiler at compile-time\n";
    cout << "\n";
    cout << "SYNTAX:\n";
    cout << "class MyClass {\n";
    cout << "    int value;\n";
    cout << "public:\n";
    cout << "    // Const member function\n";
    cout << "    int getValue() const {\n";
    cout << "        return value;  // Read only\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    // Non-const member function\n";
    cout << "    void setValue(int v) {\n";
    cout << "        value = v;  // Modifies\n";
    cout << "    }\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "WHY USE CONST MEMBER FUNCTIONS?\n";
    cout << "========================================\n";
    cout << "1. COMMUNICATE INTENT:\n";
    cout << "   • Clear which functions read vs modify\n";
    cout << "   • Self-documenting code\n";
    cout << "   • Better API design\n";
    cout << "\n";
    cout << "2. ENABLE CONST OBJECTS:\n";
    cout << "   • const objects can only call const functions\n";
    cout << "   • Without const, many operations impossible\n";
    cout << "   • Critical for const references/pointers\n";
    cout << "\n";
    cout << "3. COMPILE-TIME SAFETY:\n";
    cout << "   • Compiler prevents accidental modifications\n";
    cout << "   • Catches bugs early\n";
    cout << "   • No runtime overhead\n";
    cout << "\n";
    cout << "4. ENABLE OPTIMIZATIONS:\n";
    cout << "   • Compiler can optimize better\n";
    cout << "   • Knows function doesn't modify state\n";
    cout << "   • Can avoid copies and locks\n";
    cout << "\n";
    cout << "5. BETTER CONST CORRECTNESS:\n";
    cout << "   • Fundamental to const correctness\n";
    cout << "   • Propagates through codebase\n";
    cout << "   • Industry best practice\n";

    cout << "\n========================================\n";
    cout << "CONST MEMBER FUNCTION RULES\n";
    cout << "========================================\n";
    cout << "WHAT YOU CAN DO:\n";
    cout << "  ✅ Read member variables\n";
    cout << "  ✅ Return member values\n";
    cout << "  ✅ Call other const member functions\n";
    cout << "  ✅ Create/modify local variables\n";
    cout << "  ✅ Output to console/files\n";
    cout << "  ✅ Perform calculations\n";
    cout << "  ✅ Modify mutable members\n";
    cout << "\n";
    cout << "WHAT YOU CANNOT DO:\n";
    cout << "  ❌ Modify member variables (except mutable)\n";
    cout << "  ❌ Call non-const member functions\n";
    cout << "  ❌ Assign to member variables\n";
    cout << "  ❌ Change object state\n";
    cout << "\n";
    cout << "EXAMPLES:\n";
    cout << "class Example {\n";
    cout << "    int value;\n";
    cout << "    mutable int counter;\n";
    cout << "    \n";
    cout << "    void modify() { value++; }\n";
    cout << "    \n";
    cout << "public:\n";
    cout << "    // Const function\n";
    cout << "    void legal() const {\n";
    cout << "        int x = value;      // ✅ Read OK\n";
    cout << "        int y = x * 2;      // ✅ Local OK\n";
    cout << "        counter++;          // ✅ Mutable OK\n";
    cout << "        cout << value;      // ✅ Output OK\n";
    cout << "        \n";
    cout << "        // value = 10;      // ❌ Can't modify\n";
    cout << "        // modify();        // ❌ Can't call non-const\n";
    cout << "    }\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "CONST OBJECTS\n";
    cout << "========================================\n";
    cout << "When object is const:\n";
    cout << "  • Can ONLY call const member functions\n";
    cout << "  • Cannot call non-const functions\n";
    cout << "  • Compiler enforces this strictly\n";
    cout << "\n";
    cout << "class Point {\n";
    cout << "    int x, y;\n";
    cout << "public:\n";
    cout << "    int getX() const { return x; }\n";
    cout << "    void setX(int v) { x = v; }\n";
    cout << "};\n";
    cout << "\n";
    cout << "const Point p(10, 20);\n";
    cout << "int x = p.getX();  // ✅ OK - const function\n";
    cout << "p.setX(30);        // ❌ ERROR - non-const function\n";
    cout << "\n";
    cout << "COMMON SCENARIOS:\n";
    cout << "  • Const references: void func(const Point& p)\n";
    cout << "  • Const pointers: const Point* ptr\n";
    cout << "  • Const objects: const Point p;\n";
    cout << "  • Const return: const Point& getPoint() const\n";

    cout << "\n========================================\n";
    cout << "CONST OVERLOADING\n";
    cout << "========================================\n";
    cout << "Can have TWO versions of same function:\n";
    cout << "  • Const version for const objects\n";
    cout << "  • Non-const version for non-const objects\n";
    cout << "\n";
    cout << "class Container {\n";
    cout << "    string data;\n";
    cout << "public:\n";
    cout << "    // Const version - read-only access\n";
    cout << "    const string& getData() const {\n";
    cout << "        return data;\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    // Non-const version - modifiable access\n";
    cout << "    string& getData() {\n";
    cout << "        return data;\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "USAGE:\n";
    cout << "Container c;\n";
    cout << "c.getData() = \"New\";  // Calls non-const, can modify\n";
    cout << "\n";
    cout << "const Container cc;\n";
    cout << "string s = cc.getData();  // Calls const, read-only\n";
    cout << "\n";
    cout << "WHEN TO USE:\n";
    cout << "  • Element access (operator[])\n";
    cout << "  • Getting internal data\n";
    cout << "  • Iterator access (begin/end)\n";
    cout << "  • Container operations\n";

    cout << "\n========================================\n";
    cout << "MUTABLE KEYWORD\n";
    cout << "========================================\n";
    cout << "Mutable members can be modified in const functions:\n";
    cout << "\n";
    cout << "class Cache {\n";
    cout << "    int value;\n";
    cout << "    mutable bool cached;\n";
    cout << "    mutable int cachedResult;\n";
    cout << "    \n";
    cout << "public:\n";
    cout << "    int compute() const {\n";
    cout << "        if (cached) {\n";
    cout << "            return cachedResult;  // From cache\n";
    cout << "        }\n";
    cout << "        \n";
    cout << "        // Expensive computation\n";
    cout << "        int result = value * value;\n";
    cout << "        \n";
    cout << "        // Store in cache (OK - mutable!)\n";
    cout << "        cachedResult = result;\n";
    cout << "        cached = true;\n";
    cout << "        \n";
    cout << "        return result;\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "WHEN TO USE MUTABLE:\n";
    cout << "  ✅ Caching mechanisms\n";
    cout << "  ✅ Debug counters\n";
    cout << "  ✅ Performance metrics\n";
    cout << "  ✅ Mutexes for thread safety\n";
    cout << "  ✅ Lazy initialization\n";
    cout << "\n";
    cout << "WHEN NOT TO USE:\n";
    cout << "  ❌ Business logic state\n";
    cout << "  ❌ Core object data\n";
    cout << "  ❌ Making non-const functions \"work\"\n";
    cout << "  ❌ Avoiding proper const correctness\n";

    cout << "\n========================================\n";
    cout << "CONST AND POINTERS/REFERENCES\n";
    cout << "========================================\n";
    cout << "Different const placements mean different things:\n";
    cout << "\n";
    cout << "1. POINTER TO CONST:\n";
    cout << "   const Type* ptr;\n";
    cout << "   • Can't modify what's pointed to\n";
    cout << "   • Can change where pointer points\n";
    cout << "   • ptr = &other;  // ✅ OK\n";
    cout << "   • *ptr = value;  // ❌ ERROR\n";
    cout << "\n";
    cout << "2. CONST POINTER:\n";
    cout << "   Type* const ptr;\n";
    cout << "   • Can modify what's pointed to\n";
    cout << "   • Can't change where pointer points\n";
    cout << "   • ptr = &other;  // ❌ ERROR\n";
    cout << "   • *ptr = value;  // ✅ OK\n";
    cout << "\n";
    cout << "3. CONST POINTER TO CONST:\n";
    cout << "   const Type* const ptr;\n";
    cout << "   • Can't modify what's pointed to\n";
    cout << "   • Can't change where pointer points\n";
    cout << "   • ptr = &other;  // ❌ ERROR\n";
    cout << "   • *ptr = value;  // ❌ ERROR\n";
    cout << "\n";
    cout << "4. CONST REFERENCE:\n";
    cout << "   const Type& ref;\n";
    cout << "   • Can't modify through reference\n";
    cout << "   • Reference itself can't be rebound anyway\n";
    cout << "   • ref = value;  // ❌ ERROR\n";
    cout << "\n";
    cout << "MEMORY TRICK:\n";
    cout << "Read right to left:\n";
    cout << "  const int* ptr     → ptr is a pointer to const int\n";
    cout << "  int* const ptr     → ptr is a const pointer to int\n";
    cout << "  const int* const   → ptr is a const pointer to const int\n";

    cout << "\n========================================\n";
    cout << "CONST CORRECTNESS CHAIN\n";
    cout << "========================================\n";
    cout << "Const correctness propagates through code:\n";
    cout << "\n";
    cout << "// If parameter is const...\n";
    cout << "void display(const Person& p) {\n";
    cout << "    // ...can only call const functions\n";
    cout << "    p.getName();  // Must be const!\n";
    cout << "    p.getAge();   // Must be const!\n";
    cout << "}\n";
    cout << "\n";
    cout << "// So Person class needs const functions:\n";
    cout << "class Person {\n";
    cout << "public:\n";
    cout << "    string getName() const { return name; }\n";
    cout << "    int getAge() const { return age; }\n";
    cout << "};\n";
    cout << "\n";
    cout << "This creates a chain:\n";
    cout << "  const parameter\n";
    cout << "  → needs const functions\n";
    cout << "  → which can only call other const functions\n";
    cout << "  → propagates throughout codebase\n";

    cout << "\n========================================\n";
    cout << "BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "✅ DO:\n";
    cout << "  • Mark ALL read-only functions const\n";
    cout << "  • Use const references for parameters\n";
    cout << "  • Const-overload when returning references\n";
    cout << "  • Test with const objects\n";
    cout << "  • Use mutable sparingly and appropriately\n";
    cout << "  • const after parameter list: func() const\n";
    cout << "  • Make getters const by default\n";
    cout << "  • Const member functions for queries\n";
    cout << "\n";
    cout << "❌ DON'T:\n";
    cout << "  • Forget const on getters\n";
    cout << "  • Mark functions const that modify state\n";
    cout << "  • Abuse mutable for business logic\n";
    cout << "  • Put const before parameters: const func()\n";
    cout << "  • Ignore compiler const errors\n";
    cout << "  • Use const_cast unless absolutely necessary\n";
    cout << "  • Make everything const without reason\n";
    cout << "  • Forget to test with const objects\n";

    cout << "\n========================================\n";
    cout << "COMMON MISTAKES\n";
    cout << "========================================\n";
    cout << "❌ Mistake 1: Forgetting const on getters\n";
    cout << "int getValue() {  // ❌ Should be const\n";
    cout << "    return value;\n";
    cout << "}\n";
    cout << "Fix: int getValue() const { return value; }\n";
    cout << "\n";
    cout << "❌ Mistake 2: Wrong const placement\n";
    cout << "void const display() { }  // ❌ WRONG\n";
    cout << "Fix: void display() const { }\n";
    cout << "\n";
    cout << "❌ Mistake 3: Modifying in const function\n";
    cout << "void func() const {\n";
    cout << "    value = 10;  // ❌ ERROR\n";
    cout << "}\n";
    cout << "Fix: Remove const or don't modify\n";
    cout << "\n";
    cout << "❌ Mistake 4: Calling non-const from const\n";
    cout << "void display() const {\n";
    cout << "    modify();  // ❌ ERROR if modify() not const\n";
    cout << "}\n";
    cout << "Fix: Make modify() const or remove const from display\n";
    cout << "\n";
    cout << "❌ Mistake 5: Not testing with const objects\n";
    cout << "// Never tested with const - problems hidden!\n";
    cout << "Fix: Always test: const MyClass obj;\n";

    cout << "\n========================================\n";
    cout << "WHEN TO USE CONST FUNCTIONS\n";
    cout << "========================================\n";
    cout << "ALWAYS CONST:\n";
    cout << "  ✅ Getters (getValue, getName, etc.)\n";
    cout << "  ✅ Query functions (isEmpty, isValid, etc.)\n";
    cout << "  ✅ Calculations (area, length, count, etc.)\n";
    cout << "  ✅ Display/output functions\n";
    cout << "  ✅ Comparison operations\n";
    cout << "  ✅ Conversion functions\n";
    cout << "\n";
    cout << "NEVER CONST:\n";
    cout << "  ❌ Setters (setValue, setName, etc.)\n";
    cout << "  ❌ Modifiers (increment, clear, etc.)\n";
    cout << "  ❌ State-changing operations\n";
    cout << "  ❌ Initialization functions\n";
    cout << "  ❌ Resource acquisition/release\n";
    cout << "\n";
    cout << "GENERAL RULE:\n";
    cout << "If function doesn't modify object state\n";
    cout << "→ Mark it const!\n";

    cout << "\n========================================\n";
    cout << "QUICK REFERENCE\n";
    cout << "========================================\n";
    cout << "// Basic const member function\n";
    cout << "class MyClass {\n";
    cout << "    int value;\n";
    cout << "    mutable int counter;\n";
    cout << "    \n";
    cout << "public:\n";
    cout << "    // Const - read only\n";
    cout << "    int getValue() const {\n";
    cout << "        counter++;  // OK - mutable\n";
    cout << "        return value;\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    // Non-const - modifies\n";
    cout << "    void setValue(int v) {\n";
    cout << "        value = v;\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    // Const overload - read only\n";
    cout << "    const int& get() const {\n";
    cout << "        return value;\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    // Non-const overload - modifiable\n";
    cout << "    int& get() {\n";
    cout << "        return value;\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Usage\n";
    cout << "MyClass obj;\n";
    cout << "obj.get() = 10;  // Non-const version\n";
    cout << "\n";
    cout << "const MyClass constObj;\n";
    cout << "int x = constObj.get();  // Const version\n";

    cout << "\n========================================\n";
    cout << "KEY TAKEAWAYS\n";
    cout << "========================================\n";
    cout << "1. Const member functions promise not to modify state\n";
    cout << "2. Required for const objects to call functions\n";
    cout << "3. Syntax: return_type func() const { }\n";
    cout << "4. Can read members but not modify (except mutable)\n";
    cout << "5. Can call other const functions only\n";
    cout << "6. Const overloading provides flexibility\n";
    cout << "7. Mutable allows modification in const functions\n";
    cout << "8. Critical for const correctness\n";
    cout << "9. Enables compiler optimizations\n";
    cout << "10. Mark ALL read-only functions const!\n";

    cout << "\n========================================\n";
    cout << "GOLDEN RULES\n";
    cout << "========================================\n";
    cout << "⚠️  RULE 1: Mark all read-only functions const\n";
    cout << "   If it doesn't modify → make it const!\n";
    cout << "   \n";
    cout << "   int getValue() const { return value; }\n";
    cout << "\n";
    cout << "⚠️  RULE 2: Test with const objects\n";
    cout << "   Always verify your API works with const!\n";
    cout << "   \n";
    cout << "   const MyClass obj;\n";
    cout << "   obj.getValue();  // Must work!\n";
    cout << "\n";
    cout << "⚠️  RULE 3: Const goes AFTER parameters\n";
    cout << "   Not before, not in middle!\n";
    cout << "   \n";
    cout << "   void func() const { }  // ✅ CORRECT\n";
    cout << "   void const func() { }  // ❌ WRONG\n";
    cout << "\n";
    cout << "⚠️  RULE 4: Use const references for parameters\n";
    cout << "   Avoid copies, enable const objects!\n";
    cout << "   \n";
    cout << "   void process(const MyClass& obj) { }\n";
    cout << "\n";
    cout << "⚠️  RULE 5: Mutable only for implementation details\n";
    cout << "   Not for business logic!\n";
    cout << "   \n";
    cout << "   mutable int cacheHits;  // ✅ OK\n";
    cout << "   mutable int balance;    // ❌ BAD\n";

    return 0;
}
