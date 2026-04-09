#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cmath>
using namespace std;

// ============================================
// WHAT ARE CONST OBJECTS?
// ============================================

/*
 * CONST OBJECTS:
 * - Objects declared with 'const' keyword
 * - Cannot be modified after initialization
 * - Can only call const member functions
 * - Must be initialized at creation
 *
 * WHY CONST OBJECTS?
 * - Prevent accidental modifications
 * - Express intent (this won't change)
 * - Compiler enforces immutability
 * - Thread-safe for reading
 * - Better optimization opportunities
 *
 * SYNTAX:
 * - const Type obj(args);
 * - const Type obj = value;
 * - const Type obj{args};  (C++11)
 *
 * RULES:
 * - Must be initialized when created
 * - Cannot modify member variables
 * - Can only call const member functions
 * - Cannot call non-const member functions
 * - Lifetime: same as non-const objects
 *
 * CONST REFERENCES:
 * - const Type& ref = obj;
 * - Commonly used in function parameters
 * - Prevents copying, prevents modification
 * - Most common use of const in C++
 *
 * CONST POINTERS:
 * - const Type* ptr = &obj;
 * - Type* const ptr = &obj;
 * - const Type* const ptr = &obj;
 * - Different meanings for each
 *
 * BENEFITS:
 * - Compile-time safety
 * - Clear intent
 * - Thread safety for reads
 * - Compiler optimizations
 * - Prevents bugs
 */

// ============================================
// EXAMPLE 1: BASIC CONST OBJECTS
// ============================================

class Point {
private:
    int x, y;

public:
    Point(int x, int y) : x(x), y(y) {
        cout << "  Point(" << x << ", " << y << ") created\n";
    }

    // Const functions - can be called on const objects
    int getX() const { return x; }
    int getY() const { return y; }

    void display() const {
        cout << "  (" << x << ", " << y << ")\n";
    }

    double distance() const {
        return sqrt(x*x + y*y);
    }

    // Non-const functions - cannot be called on const objects
    void setX(int newX) { x = newX; }
    void setY(int newY) { y = newY; }

    void move(int dx, int dy) {
        x += dx;
        y += dy;
    }
};

void demonstrateBasicConstObjects() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: BASIC CONST OBJECTS    ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Regular (non-const) object ---\n";
    Point p1(10, 20);

    // Can call both const and non-const functions
    cout << "X: " << p1.getX() << "\n";  // ✅ const function
    p1.display();                         // ✅ const function
    p1.setX(30);                         // ✅ non-const function
    p1.move(5, 5);                       // ✅ non-const function
    cout << "After modification:\n";
    p1.display();

    cout << "\n--- Const object ---\n";
    const Point p2(100, 200);

    // Can ONLY call const functions
    cout << "X: " << p2.getX() << "\n";  // ✅ const function
    p2.display();                         // ✅ const function
    cout << "Distance: " << p2.distance() << "\n";  // ✅ const function

    // p2.setX(300);     // ❌ ERROR: non-const function
    // p2.move(10, 10);  // ❌ ERROR: non-const function

    cout << "\n✅ KEY INSIGHTS:\n";
    cout << "   • Const objects can only call const member functions\n";
    cout << "   • Prevents any modification of object state\n";
    cout << "   • Enforced at compile-time (no runtime overhead)\n";
    cout << "   • Must be initialized when declared\n";
}

// ============================================
// EXAMPLE 2: CONST OBJECT INITIALIZATION
// ============================================

class Student {
private:
    string name;
    int id;
    double gpa;

public:
    Student(const string& n, int i, double g)
        : name(n), id(i), gpa(g) {
        cout << "  Student " << name << " created\n";
    }

    // Const functions
    string getName() const { return name; }
    int getId() const { return id; }
    double getGPA() const { return gpa; }

    void display() const {
        cout << "  " << name << " (ID: " << id << ", GPA: " << gpa << ")\n";
    }

    // Non-const functions
    void setGPA(double g) { gpa = g; }
};

void demonstrateConstInitialization() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: INITIALIZATION         ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Various initialization methods ---\n";

    // Method 1: Constructor with parentheses
    const Student s1("Alice", 101, 3.8);

    // Method 2: Copy initialization
    const Student s2 = Student("Bob", 102, 3.5);

    // Method 3: Uniform initialization (C++11)
    const Student s3{"Charlie", 103, 3.9};

    cout << "\n--- Using const objects ---\n";
    s1.display();
    s2.display();
    s3.display();

    // ❌ Cannot create uninitialized const object
    // const Student s4;  // ERROR: const must be initialized

    // ❌ Cannot modify after creation
    // s1.setGPA(4.0);  // ERROR: non-const function

    cout << "\n✅ INITIALIZATION RULES:\n";
    cout << "   • Const objects MUST be initialized\n";
    cout << "   • Cannot be default-constructed (unless class allows)\n";
    cout << "   • Cannot be modified after initialization\n";
    cout << "   • All initialization forms work ((), =, {})\n";
}

// ============================================
// EXAMPLE 3: CONST REFERENCES (Most Common!)
// ============================================

class Book {
private:
    string title;
    string author;
    int pages;

public:
    Book(const string& t, const string& a, int p)
        : title(t), author(a), pages(p) {}

    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    int getPages() const { return pages; }

    void display() const {
        cout << "  \"" << title << "\" by " << author
             << " (" << pages << " pages)\n";
    }

    bool isLongBook() const {
        return pages > 500;
    }
};

// ✅ BEST PRACTICE: Use const reference for parameters
void printBook(const Book& book) {
    cout << "Book info:\n";
    book.display();

    if (book.isLongBook()) {
        cout << "  This is a long book!\n";
    }

    // book.setPages(100);  // ❌ ERROR if this existed
}

// ❌ BAD: Pass by value (creates copy)
void printBookBad(Book book) {  // Expensive copy!
    book.display();
}

// ❌ BAD: Non-const reference (can't accept temporaries)
void printBookBad2(Book& book) {
    book.display();
}

void demonstrateConstReferences() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: CONST REFERENCES       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating book ---\n";
    Book book("1984", "George Orwell", 328);

    cout << "\n--- Passing by const reference (BEST) ---\n";
    printBook(book);

    cout << "\n--- Can pass temporary objects ---\n";
    printBook(Book("Brave New World", "Aldous Huxley", 288));

    // printBookBad2(Book("Temp", "Author", 100));  // ❌ ERROR

    cout << "\n✅ CONST REFERENCE BENEFITS:\n";
    cout << "   • No copying (efficient)\n";
    cout << "   • Can't modify (safe)\n";
    cout << "   • Can accept temporaries\n";
    cout << "   • Most common const usage!\n";

    cout << "\n💡 GOLDEN RULE:\n";
    cout << "   Always use const& for function parameters\n";
    cout << "   unless you need to modify the object!\n";
}

// ============================================
// EXAMPLE 4: CONST OBJECTS IN CONTAINERS
// ============================================

class Color {
private:
    int r, g, b;
    string name;

public:
    Color(int r, int g, int b, const string& n)
        : r(r), g(g), b(b), name(n) {}

    int getRed() const { return r; }
    int getGreen() const { return g; }
    int getBlue() const { return b; }
    string getName() const { return name; }

    void display() const {
        cout << "  " << name << " RGB(" << r << ", " << g << ", " << b << ")\n";
    }
};

void demonstrateConstInContainers() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: CONST IN CONTAINERS    ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating color palette ---\n";
    const vector<Color> palette = {
        Color(255, 0, 0, "Red"),
        Color(0, 255, 0, "Green"),
        Color(0, 0, 255, "Blue"),
        Color(255, 255, 0, "Yellow")
    };

    cout << "\n--- Accessing colors (const container) ---\n";
    for (const Color& color : palette) {  // const reference!
        color.display();
    }

    cout << "\n--- Accessing by index ---\n";
    const Color& firstColor = palette[0];  // const reference!
    cout << "First color: " << firstColor.getName() << "\n";

    // palette.push_back(Color(0, 0, 0, "Black"));  // ❌ ERROR: const container
    // palette[0].setRed(128);  // ❌ ERROR if this existed

    cout << "\n✅ CONST CONTAINERS:\n";
    cout << "   • Elements are const\n";
    cout << "   • Can't add/remove elements\n";
    cout << "   • Can't modify elements\n";
    cout << "   • Use const& in range-based for\n";
}

// ============================================
// EXAMPLE 5: CONST POINTERS
// ============================================

class Data {
private:
    int value;

public:
    Data(int v) : value(v) {
        cout << "  Data(" << value << ") created\n";
    }

    int getValue() const { return value; }
    void setValue(int v) { value = v; }

    void display() const {
        cout << "  Value: " << value << "\n";
    }
};

void demonstrateConstPointers() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: CONST POINTERS         ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Data d1(10);
    Data d2(20);

    cout << "--- 1. Regular pointer (no const) ---\n";
    Data* ptr1 = &d1;
    ptr1->display();
    ptr1->setValue(15);        // ✅ Can modify data
    ptr1 = &d2;                // ✅ Can change pointer
    ptr1->display();

    cout << "\n--- 2. Pointer to const (can't modify data) ---\n";
    const Data* ptr2 = &d1;
    ptr2->display();           // ✅ Can read
    // ptr2->setValue(25);     // ❌ ERROR: can't modify through ptr
    ptr2 = &d2;                // ✅ Can change pointer
    ptr2->display();

    cout << "\n--- 3. Const pointer (can't change pointer) ---\n";
    Data* const ptr3 = &d1;
    ptr3->display();
    ptr3->setValue(30);        // ✅ Can modify data
    // ptr3 = &d2;             // ❌ ERROR: can't change pointer
    ptr3->display();

    cout << "\n--- 4. Const pointer to const (both const) ---\n";
    const Data* const ptr4 = &d1;
    ptr4->display();           // ✅ Can read
    // ptr4->setValue(40);     // ❌ ERROR: can't modify data
    // ptr4 = &d2;             // ❌ ERROR: can't change pointer

    cout << "\n💡 CONST POINTER GUIDE:\n";
    cout << "   const Type* ptr      → pointer to const (can't modify data)\n";
    cout << "   Type* const ptr      → const pointer (can't change pointer)\n";
    cout << "   const Type* const    → both const\n";
    cout << "\n";
    cout << "MEMORY TRICK: Read right to left!\n";
    cout << "   const int* ptr  → ptr is a pointer to const int\n";
    cout << "   int* const ptr  → ptr is a const pointer to int\n";
}

// ============================================
// EXAMPLE 6: CONST OBJECTS AND CONSTRUCTORS
// ============================================

class Configuration {
private:
    const int maxConnections;  // Const member variable
    const string serverName;
    int currentConnections;

public:
    // Must initialize const members in initializer list
    Configuration(int max, const string& name)
        : maxConnections(max), serverName(name), currentConnections(0) {
        cout << "  Configuration created: " << serverName << "\n";
    }

    // Const functions
    int getMaxConnections() const { return maxConnections; }
    string getServerName() const { return serverName; }
    int getCurrentConnections() const { return currentConnections; }

    void display() const {
        cout << "  Server: " << serverName << "\n";
        cout << "  Max connections: " << maxConnections << "\n";
        cout << "  Current: " << currentConnections << "\n";
    }

    // Non-const function
    void addConnection() {
        if (currentConnections < maxConnections) {
            currentConnections++;
        }
    }
};

void demonstrateConstWithConstructors() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: CONST & CONSTRUCTORS   ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating const configuration ---\n";
    const Configuration config(100, "MainServer");
    config.display();

    cout << "\n--- Const members never change ---\n";
    cout << "Max connections: " << config.getMaxConnections() << "\n";
    cout << "Server name: " << config.getServerName() << "\n";

    // config.addConnection();  // ❌ ERROR: non-const function

    cout << "\n--- Non-const configuration ---\n";
    Configuration config2(50, "BackupServer");
    config2.display();
    config2.addConnection();
    config2.addConnection();
    cout << "\nAfter adding connections:\n";
    config2.display();

    cout << "\n💡 CONST MEMBERS:\n";
    cout << "   • Must be initialized in constructor initializer list\n";
    cout << "   • Cannot be modified after construction\n";
    cout << "   • Part of object's immutable state\n";
    cout << "   • Different from const object!\n";
}

// ============================================
// EXAMPLE 7: CONST OBJECTS AND LIFETIME
// ============================================

class Timer {
private:
    string name;

public:
    Timer(const string& n) : name(n) {
        cout << "  [" << name << "] Started\n";
    }

    ~Timer() {
        cout << "  [" << name << "] Stopped\n";
    }

    void tick() const {
        cout << "  [" << name << "] Tick\n";
    }
};

void demonstrateLifetime() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 7: LIFETIME               ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Block scope ---\n";
    {
        const Timer t1("BlockTimer");
        t1.tick();
        // t1 destroyed at end of block
    }
    cout << "  (Timer destroyed)\n";

    cout << "\n--- Function scope ---\n";
    const Timer t2("FunctionTimer");
    t2.tick();

    cout << "\n--- Temporary const object ---\n";
    Timer("TempTimer").tick();  // Created and destroyed immediately
    cout << "  (Temp destroyed)\n";

    cout << "\n✅ LIFETIME RULES:\n";
    cout << "   • Same as non-const objects\n";
    cout << "   • Destroyed at end of scope\n";
    cout << "   • Temporaries destroyed immediately\n";
    cout << "   • Const doesn't affect lifetime\n";

    cout << "\n--- End of function ---\n";
    // t2 will be destroyed here
}

// ============================================
// EXAMPLE 8: CONST OBJECTS IN FUNCTIONS
// ============================================

class Rectangle {
private:
    double width, height;

public:
    Rectangle(double w, double h) : width(w), height(h) {}

    double getWidth() const { return width; }
    double getHeight() const { return height; }
    double area() const { return width * height; }
    double perimeter() const { return 2 * (width + height); }

    void display() const {
        cout << "  Rectangle: " << width << "x" << height
             << " (Area: " << area() << ")\n";
    }
};

// Pass by const reference (BEST)
double calculateArea(const Rectangle& rect) {
    return rect.area();
}

// Return const object
const Rectangle createSquare(double size) {
    return Rectangle(size, size);
}

// Function with const local object
void processRectangle() {
    const Rectangle localRect(10, 20);
    cout << "  Processing local rectangle\n";
    localRect.display();
}

void demonstrateConstInFunctions() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 8: CONST IN FUNCTIONS     ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Rectangle rect(15, 25);

    cout << "--- Pass by const reference ---\n";
    double area = calculateArea(rect);
    cout << "  Area: " << area << "\n";

    cout << "\n--- Return const object ---\n";
    const Rectangle square = createSquare(10);
    square.display();

    cout << "\n--- Const local object ---\n";
    processRectangle();

    cout << "\n✅ FUNCTION PATTERNS:\n";
    cout << "   • Parameter: const Type& (most common)\n";
    cout << "   • Return: const Type (rare, prevents modification)\n";
    cout << "   • Local: const Type (when won't change)\n";
}

// ============================================
// EXAMPLE 9: CONST OBJECTS AND THREAD SAFETY
// ============================================

class SharedData {
private:
    int value;
    string name;

public:
    SharedData(int v, const string& n) : value(v), name(n) {}

    // Const functions - safe for multiple threads to call simultaneously
    int getValue() const { return value; }
    string getName() const { return name; }

    void display() const {
        cout << "  " << name << ": " << value << "\n";
    }

    // Non-const function - not safe without synchronization
    void setValue(int v) {
        value = v;
    }
};

void demonstrateThreadSafety() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 9: THREAD SAFETY          ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    const SharedData constData(42, "Immutable");

    cout << "--- Const object (thread-safe for reading) ---\n";
    constData.display();

    cout << "\n✅ CONST AND THREAD SAFETY:\n";
    cout << "   • Const objects safe to read from multiple threads\n";
    cout << "   • No modification possible = no race conditions\n";
    cout << "   • No synchronization needed for const operations\n";
    cout << "   • Only const member functions callable\n";

    cout << "\n⚠️  NON-CONST OBJECTS:\n";
    cout << "   • Need synchronization (mutex, locks)\n";
    cout << "   • Race conditions possible\n";
    cout << "   • More complex to use safely\n";

    cout << "\n💡 BEST PRACTICE:\n";
    cout << "   • Use const objects when data won't change\n";
    cout << "   • Automatically thread-safe for reading\n";
    cout << "   • Simpler concurrent code\n";
}

// ============================================
// EXAMPLE 10: REAL-WORLD EXAMPLE - IMMUTABLE DATE
// ============================================

class ImmutableDate {
private:
    const int year;
    const int month;
    const int day;

public:
    ImmutableDate(int y, int m, int d)
        : year(y), month(m), day(d) {
        cout << "  Date created: " << year << "-" << month << "-" << day << "\n";
    }

    // All members are const, so all functions are naturally const
    int getYear() const { return year; }
    int getMonth() const { return month; }
    int getDay() const { return day; }

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

    void display() const {
        cout << "  " << year << "-"
             << (month < 10 ? "0" : "") << month << "-"
             << (day < 10 ? "0" : "") << day;
        if (isLeapYear()) {
            cout << " [Leap Year]";
        }
        cout << "\n";
    }

    // Returns NEW date, doesn't modify this
    ImmutableDate nextDay() const {
        int newDay = day + 1;
        int newMonth = month;
        int newYear = year;

        if (newDay > daysInMonth()) {
            newDay = 1;
            newMonth++;
            if (newMonth > 12) {
                newMonth = 1;
                newYear++;
            }
        }

        return ImmutableDate(newYear, newMonth, newDay);
    }
};

void demonstrateImmutableDate() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 10: IMMUTABLE DATE        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating immutable date ---\n";
    const ImmutableDate date1(2024, 2, 28);
    date1.display();

    cout << "\n--- Query operations ---\n";
    cout << "  Is leap year? " << (date1.isLeapYear() ? "Yes" : "No") << "\n";
    cout << "  Days in month: " << date1.daysInMonth() << "\n";

    cout << "\n--- Creating next day (returns NEW object) ---\n";
    const ImmutableDate date2 = date1.nextDay();
    cout << "Original date:\n";
    date1.display();
    cout << "Next date:\n";
    date2.display();

    cout << "\n✅ IMMUTABLE OBJECT BENEFITS:\n";
    cout << "   • Cannot be modified (compile-time enforced)\n";
    cout << "   • Thread-safe by design\n";
    cout << "   • Easier to reason about\n";
    cout << "   • No defensive copying needed\n";
    cout << "   • Can be shared freely\n";

    cout << "\n💡 IMMUTABILITY PATTERN:\n";
    cout << "   • Make all members const\n";
    cout << "   • Initialize in constructor\n";
    cout << "   • Operations return NEW objects\n";
    cout << "   • Popular in functional programming\n";
}

// ============================================
// MAIN FUNCTION - RUN ALL EXAMPLES
// ============================================

int main() {
    cout << "\n";
    cout << "╔═══════════════════════════════════════════════════════════╗\n";
    cout << "║                                                           ║\n";
    cout << "║              CONST OBJECTS IN C++                        ║\n";
    cout << "║              Immutability and Safety                      ║\n";
    cout << "║                                                           ║\n";
    cout << "╚═══════════════════════════════════════════════════════════╝\n";

    demonstrateBasicConstObjects();
    demonstrateConstInitialization();
    demonstrateConstReferences();
    demonstrateConstInContainers();
    demonstrateConstPointers();
    demonstrateConstWithConstructors();
    demonstrateLifetime();
    demonstrateConstInFunctions();
    demonstrateThreadSafety();
    demonstrateImmutableDate();

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
    cout << "WHAT ARE CONST OBJECTS?\n";
    cout << "========================================\n";
    cout << "Const objects are objects declared with 'const':\n";
    cout << "  • Cannot be modified after initialization\n";
    cout << "  • Can only call const member functions\n";
    cout << "  • Must be initialized when created\n";
    cout << "  • Compiler enforces immutability\n";
    cout << "\n";
    cout << "SYNTAX:\n";
    cout << "const Type objName(args);  // Constructor\n";
    cout << "const Type objName = value;  // Copy\n";
    cout << "const Type objName{args};  // Uniform (C++11)\n";
    cout << "\n";
    cout << "EXAMPLE:\n";
    cout << "const Point p(10, 20);  // Const object\n";
    cout << "p.getX();  // ✅ OK if getX() is const\n";
    cout << "p.setX(30);  // ❌ ERROR if setX() not const\n";

    cout << "\n========================================\n";
    cout << "WHY USE CONST OBJECTS?\n";
    cout << "========================================\n";
    cout << "1. PREVENT ACCIDENTAL MODIFICATION:\n";
    cout << "   • Compiler stops you from changing\n";
    cout << "   • Catches bugs at compile-time\n";
    cout << "   • No runtime cost\n";
    cout << "\n";
    cout << "2. EXPRESS INTENT:\n";
    cout << "   • \"This object won't change\"\n";
    cout << "   • Self-documenting code\n";
    cout << "   • Clear semantics\n";
    cout << "\n";
    cout << "3. THREAD SAFETY:\n";
    cout << "   • Safe to read from multiple threads\n";
    cout << "   • No race conditions\n";
    cout << "   • No synchronization needed\n";
    cout << "\n";
    cout << "4. OPTIMIZATION:\n";
    cout << "   • Compiler can optimize better\n";
    cout << "   • Knows value won't change\n";
    cout << "   • Can avoid defensive copies\n";
    cout << "\n";
    cout << "5. API DESIGN:\n";
    cout << "   • Force correct usage\n";
    cout << "   • Enable const parameters\n";
    cout << "   • Better interfaces\n";

    cout << "\n========================================\n";
    cout << "CONST OBJECT RULES\n";
    cout << "========================================\n";
    cout << "MUST:\n";
    cout << "  ✅ Be initialized when declared\n";
    cout << "  ✅ Have const constructor (implicitly)\n";
    cout << "  ✅ Only call const member functions\n";
    cout << "\n";
    cout << "CANNOT:\n";
    cout << "  ❌ Be modified after creation\n";
    cout << "  ❌ Call non-const member functions\n";
    cout << "  ❌ Have members changed\n";
    cout << "  ❌ Be default-initialized (in most cases)\n";
    cout << "\n";
    cout << "EXAMPLES:\n";
    cout << "const Point p(10, 20);  // ✅ Initialized\n";
    cout << "int x = p.getX();  // ✅ const function\n";
    cout << "p.setX(30);  // ❌ non-const function\n";
    cout << "\n";
    cout << "const Point p2;  // ❌ Not initialized\n";

    cout << "\n========================================\n";
    cout << "CONST REFERENCES (MOST IMPORTANT!)\n";
    cout << "========================================\n";
    cout << "Const references are THE MOST COMMON use:\n";
    cout << "\n";
    cout << "void processData(const MyClass& obj) {\n";
    cout << "    // Can read but not modify\n";
    cout << "    obj.display();  // OK if display() is const\n";
    cout << "}\n";
    cout << "\n";
    cout << "BENEFITS:\n";
    cout << "  ✅ No copying (efficient)\n";
    cout << "  ✅ Can't modify (safe)\n";
    cout << "  ✅ Can bind to temporaries\n";
    cout << "  ✅ Standard function parameter pattern\n";
    cout << "\n";
    cout << "GOLDEN RULE:\n";
    cout << "  Use const& for ALL function parameters\n";
    cout << "  unless you need to modify or it's cheap to copy!\n";
    cout << "\n";
    cout << "EXAMPLES:\n";
    cout << "// ✅ GOOD\n";
    cout << "void print(const string& s) { }\n";
    cout << "void display(const vector<int>& v) { }\n";
    cout << "void process(const MyClass& obj) { }\n";
    cout << "\n";
    cout << "// ❌ BAD\n";
    cout << "void print(string s) { }  // Unnecessary copy!\n";
    cout << "void display(vector<int> v) { }  // Expensive copy!\n";

    cout << "\n========================================\n";
    cout << "CONST POINTERS\n";
    cout << "========================================\n";
    cout << "Four variations:\n";
    cout << "\n";
    cout << "1. POINTER TO CONST:\n";
    cout << "   const Type* ptr;\n";
    cout << "   *ptr = value;  // ❌ Can't modify data\n";
    cout << "   ptr = &other;  // ✅ Can change pointer\n";
    cout << "\n";
    cout << "2. CONST POINTER:\n";
    cout << "   Type* const ptr;\n";
    cout << "   *ptr = value;  // ✅ Can modify data\n";
    cout << "   ptr = &other;  // ❌ Can't change pointer\n";
    cout << "\n";
    cout << "3. CONST POINTER TO CONST:\n";
    cout << "   const Type* const ptr;\n";
    cout << "   *ptr = value;  // ❌ Can't modify data\n";
    cout << "   ptr = &other;  // ❌ Can't change pointer\n";
    cout << "\n";
    cout << "4. REFERENCE TO CONST:\n";
    cout << "   const Type& ref = obj;\n";
    cout << "   ref = value;  // ❌ Can't modify\n";
    cout << "   (ref can't be rebound anyway)\n";
    cout << "\n";
    cout << "MEMORY TRICK (read right to left):\n";
    cout << "  const int* ptr  → ptr is pointer to const int\n";
    cout << "  int* const ptr  → ptr is const pointer to int\n";

    cout << "\n========================================\n";
    cout << "CONST MEMBERS VS CONST OBJECTS\n";
    cout << "========================================\n";
    cout << "CONST MEMBER VARIABLES:\n";
    cout << "class Config {\n";
    cout << "    const int maxSize;  // Member is const\n";
    cout << "public:\n";
    cout << "    Config(int s) : maxSize(s) { }\n";
    cout << "};\n";
    cout << "• Member can't be changed\n";
    cout << "• Object can still have non-const members\n";
    cout << "• Object can be non-const\n";
    cout << "\n";
    cout << "CONST OBJECT:\n";
    cout << "const Config c(100);  // Object is const\n";
    cout << "• Entire object immutable\n";
    cout << "• Can only call const functions\n";
    cout << "• All members effectively const\n";
    cout << "\n";
    cout << "KEY DIFFERENCE:\n";
    cout << "• Const member: One field unchangeable\n";
    cout << "• Const object: Entire object unchangeable\n";

    cout << "\n========================================\n";
    cout << "CONST OBJECTS IN CONTAINERS\n";
    cout << "========================================\n";
    cout << "Const containers:\n";
    cout << "\n";
    cout << "const vector<int> numbers = {1, 2, 3};\n";
    cout << "// numbers.push_back(4);  // ❌ Can't modify\n";
    cout << "int x = numbers[0];  // ✅ Can read\n";
    cout << "\n";
    cout << "const vector<MyClass> objects;\n";
    cout << "for (const MyClass& obj : objects) {  // const reference!\n";
    cout << "    obj.display();  // ✅ const function\n";
    cout << "}\n";
    cout << "\n";
    cout << "RULES:\n";
    cout << "  • Can't add/remove elements\n";
    cout << "  • Can't modify elements\n";
    cout << "  • Elements accessed as const\n";
    cout << "  • Use const& in range-for loops\n";

    cout << "\n========================================\n";
    cout << "IMMUTABILITY PATTERN\n";
    cout << "========================================\n";
    cout << "Make objects truly immutable:\n";
    cout << "\n";
    cout << "class ImmutablePoint {\n";
    cout << "    const int x, y;  // All members const\n";
    cout << "    \n";
    cout << "public:\n";
    cout << "    ImmutablePoint(int x, int y) : x(x), y(y) { }\n";
    cout << "    \n";
    cout << "    // All functions const (naturally)\n";
    cout << "    int getX() const { return x; }\n";
    cout << "    int getY() const { return y; }\n";
    cout << "    \n";
    cout << "    // Operations return NEW objects\n";
    cout << "    ImmutablePoint move(int dx, int dy) const {\n";
    cout << "        return ImmutablePoint(x + dx, y + dy);\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "BENEFITS:\n";
    cout << "  ✅ Cannot be modified (impossible!)\n";
    cout << "  ✅ Thread-safe by design\n";
    cout << "  ✅ Easy to reason about\n";
    cout << "  ✅ No side effects\n";
    cout << "  ✅ Functional programming style\n";

    cout << "\n========================================\n";
    cout << "LIFETIME\n";
    cout << "========================================\n";
    cout << "Const objects have same lifetime as non-const:\n";
    cout << "\n";
    cout << "void func() {\n";
    cout << "    const MyClass obj(10);  // Created\n";
    cout << "    // Use obj...\n";
    cout << "}  // Destroyed at end of scope\n";
    cout << "\n";
    cout << "RULES:\n";
    cout << "  • Created when declared\n";
    cout << "  • Destroyed at end of scope\n";
    cout << "  • Constructor/destructor still run\n";
    cout << "  • Const doesn't affect lifetime\n";
    cout << "  • Same RAII principles apply\n";

    cout << "\n========================================\n";
    cout << "BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "✅ DO:\n";
    cout << "  • Use const& for function parameters\n";
    cout << "  • Make objects const when won't change\n";
    cout << "  • Use const for configuration data\n";
    cout << "  • Mark all getters const\n";
    cout << "  • Test with const objects\n";
    cout << "  • Prefer const over non-const\n";
    cout << "  • Use const for thread-safe shared data\n";
    cout << "  • Make lookup tables const\n";
    cout << "\n";
    cout << "❌ DON'T:\n";
    cout << "  • Pass by value when const& works\n";
    cout << "  • Use non-const when const would work\n";
    cout << "  • Cast away const (const_cast)\n";
    cout << "  • Forget const on member functions\n";
    cout << "  • Leave objects mutable unnecessarily\n";
    cout << "  • Ignore const correctness\n";
    cout << "  • Use mutable to bypass const\n";
    cout << "  • Create non-const copies unnecessarily\n";

    cout << "\n========================================\n";
    cout << "COMMON PATTERNS\n";
    cout << "========================================\n";
    cout << "1. FUNCTION PARAMETERS (most common!):\n";
    cout << "   void process(const MyClass& obj) { }\n";
    cout << "\n";
    cout << "2. CONFIGURATION DATA:\n";
    cout << "   const Config config(\"settings.ini\");\n";
    cout << "\n";
    cout << "3. LOOKUP TABLES:\n";
    cout << "   const vector<string> DAYS = {\"Mon\", \"Tue\", ...};\n";
    cout << "\n";
    cout << "4. IMMUTABLE OBJECTS:\n";
    cout << "   const ImmutableDate date(2024, 1, 1);\n";
    cout << "\n";
    cout << "5. RANGE-BASED FOR:\n";
    cout << "   for (const auto& item : container) { }\n";
    cout << "\n";
    cout << "6. TEMPORARY OBJECTS:\n";
    cout << "   processData(MyClass(10, 20));\n";

    cout << "\n========================================\n";
    cout << "QUICK REFERENCE\n";
    cout << "========================================\n";
    cout << "// Const object\n";
    cout << "const MyClass obj(args);\n";
    cout << "obj.constFunc();  // ✅ OK\n";
    cout << "obj.nonConstFunc();  // ❌ ERROR\n";
    cout << "\n";
    cout << "// Const reference (parameters)\n";
    cout << "void func(const MyClass& obj) {\n";
    cout << "    obj.constFunc();  // ✅ OK\n";
    cout << "}\n";
    cout << "\n";
    cout << "// Const pointer variations\n";
    cout << "const Type* ptr;  // pointer to const\n";
    cout << "Type* const ptr;  // const pointer\n";
    cout << "const Type* const ptr;  // both const\n";
    cout << "const Type& ref;  // const reference\n";
    cout << "\n";
    cout << "// Const container\n";
    cout << "const vector<MyClass> vec;\n";
    cout << "for (const auto& item : vec) { }\n";

    cout << "\n========================================\n";
    cout << "KEY TAKEAWAYS\n";
    cout << "========================================\n";
    cout << "1. Const objects cannot be modified after initialization\n";
    cout << "2. Can only call const member functions\n";
    cout << "3. Must be initialized when declared\n";
    cout << "4. const& most common use (parameters)\n";
    cout << "5. Prevents accidental modification (compile-time)\n";
    cout << "6. Thread-safe for reading\n";
    cout << "7. Express intent clearly\n";
    cout << "8. Enable compiler optimizations\n";
    cout << "9. Same lifetime as non-const objects\n";
    cout << "10. Fundamental to const correctness!\n";

    cout << "\n========================================\n";
    cout << "GOLDEN RULES\n";
    cout << "========================================\n";
    cout << "⚠️  RULE 1: Use const& for parameters\n";
    cout << "   Unless you need to modify or it's cheap!\n";
    cout << "   \n";
    cout << "   void process(const MyClass& obj) { }  // ✅\n";
    cout << "   void process(MyClass obj) { }  // ❌ (usually)\n";
    cout << "\n";
    cout << "⚠️  RULE 2: Make objects const when possible\n";
    cout << "   If it won't change, make it const!\n";
    cout << "   \n";
    cout << "   const Config config(\"settings\");  // ✅\n";
    cout << "\n";
    cout << "⚠️  RULE 3: All member functions must be const\n";
    cout << "   For const objects to be useful!\n";
    cout << "   \n";
    cout << "   int getValue() const { return value; }  // ✅\n";
    cout << "\n";
    cout << "⚠️  RULE 4: const objects must be initialized\n";
    cout << "   Can't exist uninitialized!\n";
    cout << "   \n";
    cout << "   const Point p(10, 20);  // ✅\n";
    cout << "   const Point p;  // ❌ ERROR\n";
    cout << "\n";
    cout << "⚠️  RULE 5: Prefer const over non-const\n";
    cout << "   When in doubt, use const!\n";
    cout << "   \n";
    cout << "   Const by default, mutable by choice!\n";

    return 0;
}
