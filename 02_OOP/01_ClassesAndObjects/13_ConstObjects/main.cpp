#include <print>
#include <string>
#include <vector>
#include <thread>
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
 * - Safe to read from several threads (as long as nothing is mutable)
 * - (Not a speed feature: it is about correctness)
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
        std::println("  Point({}, {}) created", x, y);
    }

    // Const functions - can be called on const objects
    int getX() const { return x; }
    int getY() const { return y; }

    void display() const {
        std::println("  ({}, {})", x, y);
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
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: BASIC CONST OBJECTS    ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Regular (non-const) object ---");
    Point p1(10, 20);

    // Can call both const and non-const functions
    std::println("X: {}", p1.getX());  // ✅ const function
    p1.display();                         // ✅ const function
    p1.setX(30);                         // ✅ non-const function
    p1.move(5, 5);                       // ✅ non-const function
    std::println("After modification:");
    p1.display();

    std::println("");
    std::println("--- Const object ---");
    const Point p2(100, 200);

    // Can ONLY call const functions
    std::println("X: {}", p2.getX());  // ✅ const function
    p2.display();                         // ✅ const function
    std::println("Distance: {:g}", p2.distance());  // ✅ const function

    // p2.setX(300);     // ❌ ERROR: non-const function
    // p2.move(10, 10);  // ❌ ERROR: non-const function
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
        std::println("  Student {} created", name);
    }

    // Const functions
    string getName() const { return name; }
    int getId() const { return id; }
    double getGPA() const { return gpa; }

    void display() const {
        std::println("  {} (ID: {}, GPA: {:g})", name, id, gpa);
    }

    // Non-const functions
    void setGPA(double g) { gpa = g; }
};

void demonstrateConstInitialization() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: INITIALIZATION         ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Various initialization methods ---");

    // Method 1: Constructor with parentheses
    const Student s1("Alice", 101, 3.8);

    // Method 2: Copy initialization
    const Student s2 = Student("Bob", 102, 3.5);  // C++17: no copy, built in place

    // Method 3: Uniform initialization (C++11)
    const Student s3{"Charlie", 103, 3.9};

    std::println("");
    std::println("--- Using const objects ---");
    s1.display();
    s2.display();
    s3.display();

    // ❌ Cannot create uninitialized const object
    // const Student s4;  // ERROR: const must be initialized

    // ❌ Cannot modify after creation
    // s1.setGPA(4.0);  // ERROR: non-const function
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
        std::println("  \"{}\" by {} ({} pages)", title, author, pages);
    }

    bool isLongBook() const {
        return pages > 500;
    }
};

// ✅ BEST PRACTICE: Use const reference for parameters
void printBook(const Book& book) {
    std::println("Book info:");
    book.display();

    if (book.isLongBook()) {
        std::println("  This is a long book!");
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
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: CONST REFERENCES       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating book ---");
    Book book("1984", "George Orwell", 328);

    std::println("");
    std::println("--- Passing by const reference (BEST) ---");
    printBook(book);

    std::println("");
    std::println("--- Can pass temporary objects ---");
    printBook(Book("Brave New World", "Aldous Huxley", 288));

    // printBookBad2(Book("Temp", "Author", 100));  // ❌ ERROR
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
        std::println("  {} RGB({}, {}, {})", name, r, g, b);
    }
};

void demonstrateConstInContainers() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: CONST IN CONTAINERS    ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating color palette ---");
    const vector<Color> palette = {
        Color(255, 0, 0, "Red"),
        Color(0, 255, 0, "Green"),
        Color(0, 0, 255, "Blue"),
        Color(255, 255, 0, "Yellow")
    };

    std::println("");
    std::println("--- Accessing colors (const container) ---");
    for (const Color& color : palette) {  // const reference!
        color.display();
    }

    std::println("");
    std::println("--- Accessing by index ---");
    const Color& firstColor = palette[0];  // const reference!
    std::println("First color: {}", firstColor.getName());

    // palette.push_back(Color(0, 0, 0, "Black"));  // ❌ ERROR: const container
    // palette[0].setRed(128);  // ❌ ERROR if this existed
}

// ============================================
// EXAMPLE 5: CONST POINTERS
// ============================================

class Data {
private:
    int value;

public:
    Data(int v) : value(v) {
        std::println("  Data({}) created", value);
    }

    int getValue() const { return value; }
    void setValue(int v) { value = v; }

    void display() const {
        std::println("  Value: {}", value);
    }
};

void demonstrateConstPointers() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 5: CONST POINTERS         ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Data d1(10);
    Data d2(20);

    std::println("--- 1. Regular pointer (no const) ---");
    Data* ptr1 = &d1;
    ptr1->display();
    ptr1->setValue(15);        // ✅ Can modify data
    ptr1 = &d2;                // ✅ Can change pointer
    ptr1->display();

    std::println("");
    std::println("--- 2. Pointer to const (can't modify data) ---");
    const Data* ptr2 = &d1;
    ptr2->display();           // ✅ Can read
    // ptr2->setValue(25);     // ❌ ERROR: can't modify through ptr
    ptr2 = &d2;                // ✅ Can change pointer
    ptr2->display();

    std::println("");
    std::println("--- 3. Const pointer (can't change pointer) ---");
    Data* const ptr3 = &d1;
    ptr3->display();
    ptr3->setValue(30);        // ✅ Can modify data
    // ptr3 = &d2;             // ❌ ERROR: can't change pointer
    ptr3->display();

    std::println("");
    std::println("--- 4. Const pointer to const (both const) ---");
    const Data* const ptr4 = &d1;
    ptr4->display();           // ✅ Can read
    // ptr4->setValue(40);     // ❌ ERROR: can't modify data
    // ptr4 = &d2;             // ❌ ERROR: can't change pointer
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
        std::println("  Configuration created: {}", serverName);
    }

    // Const functions
    int getMaxConnections() const { return maxConnections; }
    string getServerName() const { return serverName; }
    int getCurrentConnections() const { return currentConnections; }

    void display() const {
        std::println("  Server: {}", serverName);
        std::println("  Max connections: {}", maxConnections);
        std::println("  Current: {}", currentConnections);
    }

    // Non-const function
    void addConnection() {
        if (currentConnections < maxConnections) {
            currentConnections++;
        }
    }
};

void demonstrateConstWithConstructors() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 6: CONST & CONSTRUCTORS   ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating const configuration ---");
    const Configuration config(100, "MainServer");
    config.display();

    std::println("");
    std::println("--- Const members never change ---");
    std::println("Max connections: {}", config.getMaxConnections());
    std::println("Server name: {}", config.getServerName());

    // config.addConnection();  // ❌ ERROR: non-const function

    std::println("");
    std::println("--- Non-const configuration ---");
    Configuration config2(50, "BackupServer");
    config2.display();
    config2.addConnection();
    config2.addConnection();
    std::println("");
    std::println("After adding connections:");
    config2.display();
}

// ============================================
// EXAMPLE 7: CONST OBJECTS AND LIFETIME
// ============================================

class Timer {
private:
    string name;

public:
    Timer(const string& n) : name(n) {
        std::println("  [{}] Started", name);
    }

    ~Timer() {
        std::println("  [{}] Stopped", name);
    }

    void tick() const {
        std::println("  [{}] Tick", name);
    }
};

void demonstrateLifetime() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 7: LIFETIME               ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Block scope ---");
    {
        const Timer t1("BlockTimer");
        t1.tick();
        // t1 destroyed at end of block
    }
    std::println("  (Timer destroyed)");

    std::println("");
    std::println("--- Function scope ---");
    const Timer t2("FunctionTimer");
    t2.tick();

    std::println("");
    std::println("--- Temporary const object ---");
    Timer("TempTimer").tick();  // Created and destroyed immediately
    std::println("  (Temp destroyed)");


    std::println("");
    std::println("--- End of function ---");
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
        std::println("  Rectangle: {:g}x{:g} (Area: {:g})", width, height, area());
    }
};

// Pass by const reference (BEST)
double calculateArea(const Rectangle& rect) {
    return rect.area();
}

// Return by value. Don't write 'const Rectangle' as the return type: it gains nothing
// and stops the result from being moved (see the README).
Rectangle createSquare(double size) {
    return Rectangle(size, size);
}

// Function with const local object
void processRectangle() {
    const Rectangle localRect(10, 20);
    std::println("  Processing local rectangle");
    localRect.display();
}

void demonstrateConstInFunctions() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 8: CONST IN FUNCTIONS     ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Rectangle rect(15, 25);

    std::println("--- Pass by const reference ---");
    double area = calculateArea(rect);
    std::println("  Area: {:g}", area);

    std::println("");
    std::println("--- Return const object ---");
    const Rectangle square = createSquare(10);
    square.display();

    std::println("");
    std::println("--- Const local object ---");
    processRectangle();
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
        std::println("  {}: {}", name, value);
    }

    // Non-const function - not safe without synchronization
    void setValue(int v) {
        value = v;
    }
};

void demonstrateThreadSafety() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 9: THREAD SAFETY          ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    const SharedData constData(42, "Immutable");

    std::println("--- Const object (thread-safe for reading) ---");
    constData.display();

    std::println("");
    std::println("--- Two threads read the same const object ---");
    // Only const functions can be called, so nothing is written: no data race.
    int value1 = 0, value2 = 0;
    thread t1([&] { value1 = constData.getValue(); });
    thread t2([&] { value2 = constData.getValue(); });
    t1.join();
    t2.join();
    std::println("  Thread 1 read {}, thread 2 read {}", value1, value2);
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
        std::println("  Date created: {}-{}-{}", year, month, day);
    }

    // All members are const, so all functions are naturally const
    int getYear() const { return year; }
    int getMonth() const { return month; }
    int getDay() const { return day; }

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

    void display() const {
        std::print("  {}-{:02}-{:02}", year, month, day);
        if (isLeapYear()) {
            std::print(" [Leap Year]");
        }
        std::println("");
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
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 10: IMMUTABLE DATE        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating immutable date ---");
    const ImmutableDate date1(2024, 2, 28);
    date1.display();

    std::println("");
    std::println("--- Query operations ---");
    std::println("  Is leap year? {}", (date1.isLeapYear() ? "Yes" : "No"));
    std::println("  Days in month: {}", date1.daysInMonth());

    std::println("");
    std::println("--- Creating next day (returns NEW object) ---");
    const ImmutableDate date2 = date1.nextDay();
    std::println("Original date:");
    date1.display();
    std::println("Next date:");
    date2.display();
}

// ============================================
// MAIN FUNCTION - RUN ALL EXAMPLES
// ============================================

int main() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════════════╗");
    std::println("║                                                           ║");
    std::println("║              CONST OBJECTS IN C++                        ║");
    std::println("║              Immutability and Safety                      ║");
    std::println("║                                                           ║");
    std::println("╚═══════════════════════════════════════════════════════════╝");

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

    return 0;
}
