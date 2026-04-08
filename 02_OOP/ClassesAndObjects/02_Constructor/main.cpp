#include <iostream>
#include <string>
#include <cmath>
using namespace std;

// ============================================
// WHAT IS A CONSTRUCTOR?
// ============================================

/*
 * A CONSTRUCTOR is a special member function that:
 * - Has the SAME NAME as the class
 * - NO RETURN TYPE (not even void)
 * - Automatically called when object is created
 * - Used to INITIALIZE the object
 *
 * Purpose: Set initial values for member variables
 */

// ============================================
// EXAMPLE 1: CLASS WITHOUT CONSTRUCTOR (PROBLEM)
// ============================================

class StudentWithoutConstructor {
public:
    string name;
    int age;
    int id;

    void display() {
        cout << "Name: " << name << ", Age: " << age << ", ID: " << id << "\n";
    }
};

void demonstrateWithoutConstructor() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: WITHOUT CONSTRUCTOR    ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Problem: Uninitialized Variables ---\n";
    StudentWithoutConstructor student;

    cout << "⚠️  Without constructor, variables have garbage values:\n";
    // student.display();  // Would show garbage values!

    cout << "Must manually initialize:\n";
    student.name = "Alice";
    student.age = 20;
    student.id = 12345;
    student.display();

    cout << "\n❌ Problems:\n";
    cout << "   • Easy to forget initialization\n";
    cout << "   • Repetitive code for each object\n";
    cout << "   • Error-prone\n";
}

// ============================================
// EXAMPLE 2: DEFAULT CONSTRUCTOR
// ============================================

class StudentWithConstructor {
public:
    string name;
    int age;
    int id;

    // Default Constructor
    // Called when: Student s;
    StudentWithConstructor() {
        cout << "🏗️  Default constructor called!\n";
        name = "Unknown";
        age = 0;
        id = 0;
    }

    void display() {
        cout << "Name: " << name << ", Age: " << age << ", ID: " << id << "\n";
    }
};

void demonstrateDefaultConstructor() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: DEFAULT CONSTRUCTOR    ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating object with default constructor ---\n";
    StudentWithConstructor student;  // Constructor automatically called!

    cout << "\n✅ Variables automatically initialized:\n";
    student.display();

    cout << "\nBenefits:\n";
    cout << "   ✓ Automatic initialization\n";
    cout << "   ✓ No garbage values\n";
    cout << "   ✓ Safe to use immediately\n";
}

// ============================================
// EXAMPLE 3: PARAMETERIZED CONSTRUCTOR
// ============================================

class Car {
public:
    string brand;
    string model;
    int year;
    double price;

    // Parameterized Constructor
    // Called when: Car c("Toyota", "Camry", 2024, 28000);
    Car(string b, string m, int y, double p) {
        cout << "🏗️  Parameterized constructor called for " << b << "\n";
        brand = b;
        model = m;
        year = y;
        price = p;
    }

    void display() {
        cout << "🚗 " << year << " " << brand << " " << model
             << " - $" << price << "\n";
    }
};

void demonstrateParameterizedConstructor() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: PARAMETERIZED CONSTRUCTOR ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating cars with initial values ---\n";
    Car car1("Toyota", "Camry", 2024, 28000);
    Car car2("Tesla", "Model 3", 2024, 42000);
    Car car3("BMW", "M3", 2023, 75000);

    cout << "\n--- Displaying cars ---\n";
    car1.display();
    car2.display();
    car3.display();

    cout << "\n✅ Objects created and initialized in one step!\n";
}

// ============================================
// EXAMPLE 4: CONSTRUCTOR OVERLOADING
// ============================================

class BankAccount {
public:
    string accountHolder;
    int accountNumber;
    double balance;

    // Constructor 1: Default old method
    // BankAccount() {
    //     cout << "🏗️  Default constructor called\n";
    //     accountHolder = "Unknown";
    //     accountNumber = 0;
    //     balance = 0.0;
    // }

    BankAccount():
        accountHolder("Unknown"),
        accountNumber(0),
        balance(0.0)
    {
        cout << "🏗️  Default constructor called\n";
    }


    // Constructor 2: With holder and number only
    BankAccount(string holder, int number) {
        cout << "🏗️  Constructor with 2 parameters called\n";
        accountHolder = holder;
        accountNumber = number;
        balance = 0.0;  // Default balance
    }

    // Constructor 3: With all parameters
    BankAccount(string holder, int number, double initialBalance) {
        cout << "🏗️  Constructor with 3 parameters called\n";
        accountHolder = holder;
        accountNumber = number;
        balance = initialBalance;
    }

    void display() {
        cout << "💳 Account: " << accountHolder
             << " (#" << accountNumber << ") - Balance: $" << balance << "\n";
    }
};

void demonstrateConstructorOverloading() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: CONSTRUCTOR OVERLOADING║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating accounts with different constructors ---\n\n";

    cout << "Account 1: Default constructor\n";
    BankAccount acc1;
    acc1.display();

    cout << "\nAccount 2: Constructor with holder and number\n";
    BankAccount acc2("Alice Johnson", 12345);
    acc2.display();

    cout << "\nAccount 3: Constructor with all parameters\n";
    BankAccount acc3("Bob Smith", 67890, 5000.00);
    acc3.display();

    cout << "\n✅ Same class, different ways to create objects!\n";
}

// ============================================
// EXAMPLE 5: CONSTRUCTOR WITH DEFAULT ARGUMENTS
// ============================================

class Rectangle {
public:
    double width;
    double height;

    // Constructor with default arguments
    Rectangle(double w = 1.0, double h = 1.0) {
        cout << "🏗️  Constructor called with width=" << w
             << ", height=" << h << "\n";
        width = w;
        height = h;
    }

    double area() {
        return width * height;
    }

    void display() {
        cout << "📐 Rectangle: " << width << " × " << height
             << " = " << area() << " sq units\n";
    }
};

void demonstrateDefaultArguments() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: DEFAULT ARGUMENTS      ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating rectangles with different arguments ---\n\n";

    cout << "Rectangle 1: No arguments (uses defaults)\n";
    Rectangle rect1;
    rect1.display();

    cout << "\nRectangle 2: One argument (width)\n";
    Rectangle rect2(5.0);
    rect2.display();

    cout << "\nRectangle 3: Both arguments\n";
    Rectangle rect3(4.0, 6.0);
    rect3.display();

    cout << "\n✅ One constructor handles all cases!\n";
}

// ============================================
// EXAMPLE 6: MEMBER INITIALIZER LIST
// ============================================

class Point {
public:
    const int x;  // const member
    const int y;  // const member

    // Constructor using member initializer list
    // This is the ONLY way to initialize const members
    Point(int xVal, int yVal) : x(xVal), y(yVal) {
        cout << "🏗️  Point constructor: (" << x << ", " << y << ")\n";
        // x = xVal;  // ❌ Error! Can't assign to const
        // y = yVal;  // ❌ Error! Can't assign to const
    }

    void display() {
        cout << "📍 Point: (" << x << ", " << y << ")\n";
    }
};

class Circle {
public:
    Point center;
    double radius;

    // Must use initializer list for Point (no default constructor)
    Circle(int x, int y, double r) : center(x, y), radius(r) {
        cout << "🏗️  Circle constructor\n";
    }

    double area() {
        return 3.14159 * radius * radius;
    }

    void display() {
        cout << "⭕ Circle at ";
        center.display();
        cout << "   Radius: " << radius << ", Area: " << area() << "\n";
    }
};

void demonstrateMemberInitializerList() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: MEMBER INITIALIZER LIST║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating Point with const members ---\n";
    Point p(10, 20);
    p.display();

    cout << "\n--- Creating Circle with Point member ---\n";
    Circle c(5, 5, 10.0);
    c.display();

    cout << "\n💡 Initializer List Benefits:\n";
    cout << "   ✓ Can initialize const members\n";
    cout << "   ✓ Can initialize reference members\n";
    cout << "   ✓ More efficient (direct initialization)\n";
    cout << "   ✓ Required for members without default constructor\n";
}

// ============================================
// EXAMPLE 7: CONSTRUCTOR INITIALIZATION ORDER
// ============================================

class InitializationOrder {
public:
    int first;
    int second;
    int third;

    InitializationOrder(int a, int b, int c) : third(c), first(a), second(b) {
        cout << "🏗️  Constructor body\n";
        cout << "   Note: Members initialized in DECLARATION order,\n";
        cout << "         not initializer list order!\n";
    }

    void display() {
        cout << "Values: first=" << first << ", second=" << second
             << ", third=" << third << "\n";
    }
};

void demonstrateInitializationOrder() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 7: INITIALIZATION ORDER   ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Order matters! ---\n";
    cout << "Initializer list: third(c), first(a), second(b)\n";
    cout << "But initialization happens in declaration order:\n";
    cout << "   1. first (declared first)\n";
    cout << "   2. second (declared second)\n";
    cout << "   3. third (declared third)\n\n";

    InitializationOrder obj(1, 2, 3);
    obj.display();

    cout << "\n⚠️  Always list initializers in declaration order!\n";
}

// ============================================
// EXAMPLE 8: DELEGATING CONSTRUCTORS (C++11)
// ============================================

class Employee {
public:
    string name;
    int id;
    double salary;
    string department;

    // Main constructor
    Employee(string n, int i, double s, string d)
        : name(n), id(i), salary(s), department(d) {
        cout << "🏗️  Main constructor called for " << name << "\n";
    }

    // Delegating constructor 1: Just name
    Employee(string n) : Employee(n, 0, 0.0, "Unassigned") {
        cout << "   Delegated from name-only constructor\n";
    }

    // Delegating constructor 2: Name and ID
    Employee(string n, int i) : Employee(n, i, 0.0, "Unassigned") {
        cout << "   Delegated from name-id constructor\n";
    }

    void display() {
        cout << "👤 Employee: " << name << " (#" << id << ")\n";
        cout << "   Dept: " << department << ", Salary: $" << salary << "\n";
    }
};

void demonstrateDelegatingConstructors() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 8: DELEGATING CONSTRUCTORS║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Employee 1: Name only ---\n";
    Employee emp1("Alice");
    emp1.display();

    cout << "\n--- Employee 2: Name and ID ---\n";
    Employee emp2("Bob", 12345);
    emp2.display();

    cout << "\n--- Employee 3: All parameters ---\n";
    Employee emp3("Charlie", 67890, 75000, "Engineering");
    emp3.display();

    cout << "\n✅ Delegating reduces code duplication!\n";
}

// ============================================
// EXAMPLE 9: REAL-WORLD - GAME CHARACTER
// ============================================

class GameCharacter {
public:
    string name;
    int level;
    int health;
    int maxHealth;
    int attackPower;
    int defense;

    // Constructor with calculated values
    GameCharacter(string n, int lvl) : name(n), level(lvl) {
        // Calculate stats based on level
        maxHealth = 100 + (level * 20);
        health = maxHealth;
        attackPower = 10 + (level * 5);
        defense = 5 + (level * 2);

        cout << "🎮 Created " << name << " (Level " << level << ")\n";
        cout << "   ❤️  HP: " << health << "/" << maxHealth << "\n";
        cout << "   ⚔️  Attack: " << attackPower << "\n";
        cout << "   🛡️  Defense: " << defense << "\n";
    }

    void displayStats() {
        cout << "\n⚔️  " << name << " - Level " << level << "\n";
        cout << "   ❤️  HP: " << health << "/" << maxHealth << "\n";
        cout << "   ⚔️  ATK: " << attackPower << "\n";
        cout << "   🛡️  DEF: " << defense << "\n";
    }
};

void demonstrateGameCharacter() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 9: GAME CHARACTER         ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating characters at different levels ---\n\n";

    GameCharacter warrior("Warrior", 1);
    GameCharacter mage("Mage", 5);
    GameCharacter boss("Dragon Boss", 10);

    cout << "\n--- All Character Stats ---\n";
    warrior.displayStats();
    mage.displayStats();
    boss.displayStats();

    cout << "\n✅ Constructor calculates all stats automatically!\n";
}

// ============================================
// EXAMPLE 10: VALIDATION IN CONSTRUCTOR
// ============================================

class Temperature {
public:
    double celsius;

    // Constructor with validation
    Temperature(double temp) {
        cout << "🏗️  Creating Temperature object...\n";

        // Validate input
        if (temp < -273.15) {  // Absolute zero
            cout << "⚠️  Temperature below absolute zero! Setting to -273.15°C\n";
            celsius = -273.15;
        } else {
            celsius = temp;
        }

        cout << "   Temperature set to: " << celsius << "°C\n";
    }

    double toFahrenheit() {
        return (celsius * 9.0 / 5.0) + 32;
    }

    double toKelvin() {
        return celsius + 273.15;
    }

    void display() {
        cout << "🌡️  Temperature:\n";
        cout << "   Celsius:    " << celsius << "°C\n";
        cout << "   Fahrenheit: " << toFahrenheit() << "°F\n";
        cout << "   Kelvin:     " << toKelvin() << "K\n";
    }
};

void demonstrateValidation() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 10: VALIDATION            ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Valid temperature ---\n";
    Temperature t1(25.0);
    t1.display();

    cout << "\n--- Invalid temperature (too low) ---\n";
    Temperature t2(-300.0);  // Below absolute zero!
    t2.display();

    cout << "\n✅ Constructor ensures object is always in valid state!\n";
}

// ============================================
// MAIN DEMONSTRATION
// ============================================

int main() {
    cout << "========================================\n";
    cout << "CONSTRUCTOR DEMONSTRATION\n";
    cout << "========================================\n";

    demonstrateWithoutConstructor();
    demonstrateDefaultConstructor();
    demonstrateParameterizedConstructor();
    demonstrateConstructorOverloading();
    demonstrateDefaultArguments();
    demonstrateMemberInitializerList();
    demonstrateInitializationOrder();
    demonstrateDelegatingConstructors();
    demonstrateGameCharacter();
    demonstrateValidation();

    // ============================================
    // Comprehensive Summary
    // ============================================
    cout << "\n========================================\n";
    cout << "WHAT IS A CONSTRUCTOR?\n";
    cout << "========================================\n";
    cout << "A constructor is a special member function that:\n";
    cout << "  • Has the SAME NAME as the class\n";
    cout << "  • Has NO RETURN TYPE (not even void)\n";
    cout << "  • Automatically called when object is created\n";
    cout << "  • Used to INITIALIZE the object\n";
    cout << "\n";
    cout << "Purpose:\n";
    cout << "  • Set initial values for member variables\n";
    cout << "  • Allocate resources if needed\n";
    cout << "  • Validate input\n";
    cout << "  • Ensure object starts in valid state\n";

    cout << "\n========================================\n";
    cout << "CONSTRUCTOR SYNTAX\n";
    cout << "========================================\n";
    cout << "class MyClass {\n";
    cout << "public:\n";
    cout << "    int data;\n";
    cout << "    \n";
    cout << "    // Constructor\n";
    cout << "    MyClass() {              // Same name as class\n";
    cout << "        data = 0;            // No return type!\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "MyClass obj;  // Constructor automatically called!\n";

    cout << "\n========================================\n";
    cout << "TYPES OF CONSTRUCTORS\n";
    cout << "========================================\n";
    cout << "1. Default Constructor:\n";
    cout << "   • No parameters\n";
    cout << "   • MyClass() { }\n";
    cout << "   • Called: MyClass obj;\n";
    cout << "\n";
    cout << "2. Parameterized Constructor:\n";
    cout << "   • Takes parameters\n";
    cout << "   • MyClass(int x) { }\n";
    cout << "   • Called: MyClass obj(42);\n";
    cout << "\n";
    cout << "3. Copy Constructor:\n";
    cout << "   • Creates copy of object\n";
    cout << "   • MyClass(const MyClass& other) { }\n";
    cout << "   • Detailed in 07_CopyConstructor\n";

    cout << "\n========================================\n";
    cout << "CONSTRUCTOR OVERLOADING\n";
    cout << "========================================\n";
    cout << "You can have multiple constructors:\n";
    cout << "\n";
    cout << "class MyClass {\n";
    cout << "public:\n";
    cout << "    MyClass() { }              // Constructor 1\n";
    cout << "    MyClass(int x) { }         // Constructor 2\n";
    cout << "    MyClass(int x, int y) { }  // Constructor 3\n";
    cout << "};\n";
    cout << "\n";
    cout << "MyClass obj1;           // Calls constructor 1\n";
    cout << "MyClass obj2(10);       // Calls constructor 2\n";
    cout << "MyClass obj3(10, 20);   // Calls constructor 3\n";

    cout << "\n========================================\n";
    cout << "DEFAULT ARGUMENTS\n";
    cout << "========================================\n";
    cout << "class MyClass {\n";
    cout << "public:\n";
    cout << "    MyClass(int x = 0, int y = 0) {\n";
    cout << "        // x and y have default values\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "MyClass obj1;           // x=0, y=0\n";
    cout << "MyClass obj2(10);       // x=10, y=0\n";
    cout << "MyClass obj3(10, 20);   // x=10, y=20\n";
    cout << "\n";
    cout << "⚠️  Don't mix default arguments with overloading!\n";
    cout << "   Can cause ambiguity.\n";

    cout << "\n========================================\n";
    cout << "MEMBER INITIALIZER LIST\n";
    cout << "========================================\n";
    cout << "Preferred way to initialize members:\n";
    cout << "\n";
    cout << "class MyClass {\n";
    cout << "public:\n";
    cout << "    int x;\n";
    cout << "    int y;\n";
    cout << "    \n";
    cout << "    // Using initializer list\n";
    cout << "    MyClass(int a, int b) : x(a), y(b) {\n";
    cout << "        // x and y already initialized!\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "Benefits:\n";
    cout << "  ✓ More efficient (direct initialization)\n";
    cout << "  ✓ Required for const members\n";
    cout << "  ✓ Required for reference members\n";
    cout << "  ✓ Required when member has no default constructor\n";

    cout << "\n========================================\n";
    cout << "DELEGATING CONSTRUCTORS (C++11)\n";
    cout << "========================================\n";
    cout << "One constructor can call another:\n";
    cout << "\n";
    cout << "class MyClass {\n";
    cout << "public:\n";
    cout << "    // Main constructor\n";
    cout << "    MyClass(int x, int y) : x(x), y(y) { }\n";
    cout << "    \n";
    cout << "    // Delegates to main constructor\n";
    cout << "    MyClass() : MyClass(0, 0) { }\n";
    cout << "    MyClass(int x) : MyClass(x, 0) { }\n";
    cout << "};\n";
    cout << "\n";
    cout << "Benefits:\n";
    cout << "  ✓ Reduces code duplication\n";
    cout << "  ✓ Centralizes initialization logic\n";

    cout << "\n========================================\n";
    cout << "INITIALIZATION vs ASSIGNMENT\n";
    cout << "========================================\n";
    cout << "Two ways to set values in constructor:\n";
    cout << "\n";
    cout << "1. Assignment (in constructor body):\n";
    cout << "   MyClass(int a) {\n";
    cout << "       x = a;  // Assignment\n";
    cout << "   }\n";
    cout << "   • Member default-constructed first\n";
    cout << "   • Then assigned new value\n";
    cout << "   • Less efficient\n";
    cout << "\n";
    cout << "2. Initialization (initializer list):\n";
    cout << "   MyClass(int a) : x(a) {  // Initialization\n";
    cout << "   }\n";
    cout << "   • Member constructed with value directly\n";
    cout << "   • More efficient\n";
    cout << "   • Preferred way!\n";

    cout << "\n========================================\n";
    cout << "WHEN CONSTRUCTORS ARE CALLED\n";
    cout << "========================================\n";
    cout << "Automatically called when:\n";
    cout << "\n";
    cout << "1. Creating object:\n";
    cout << "   MyClass obj;           // Constructor called\n";
    cout << "   MyClass obj(10);       // Constructor called\n";
    cout << "\n";
    cout << "2. Dynamic allocation:\n";
    cout << "   MyClass* ptr = new MyClass();  // Constructor called\n";
    cout << "\n";
    cout << "3. Temporary objects:\n";
    cout << "   function(MyClass(10));  // Constructor called\n";
    cout << "\n";
    cout << "4. Array of objects:\n";
    cout << "   MyClass arr[5];  // Default constructor called 5 times\n";

    cout << "\n========================================\n";
    cout << "COMMON USES OF CONSTRUCTORS\n";
    cout << "========================================\n";
    cout << "1. Initialize member variables\n";
    cout << "   MyClass(int x) : data(x) { }\n";
    cout << "\n";
    cout << "2. Allocate resources\n";
    cout << "   MyClass() { buffer = new int[100]; }\n";
    cout << "\n";
    cout << "3. Validate input\n";
    cout << "   MyClass(int age) {\n";
    cout << "       if (age < 0) age = 0;\n";
    cout << "       this->age = age;\n";
    cout << "   }\n";
    cout << "\n";
    cout << "4. Calculate derived values\n";
    cout << "   Circle(double r) : radius(r) {\n";
    cout << "       area = 3.14159 * r * r;\n";
    cout << "   }\n";
    cout << "\n";
    cout << "5. Open files/connections\n";
    cout << "   Database(string conn) {\n";
    cout << "       connection = connect(conn);\n";
    cout << "   }\n";

    cout << "\n========================================\n";
    cout << "CONSTRUCTOR BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "✅ DO:\n";
    cout << "  • Always initialize all member variables\n";
    cout << "  • Use initializer lists (more efficient)\n";
    cout << "  • Validate input parameters\n";
    cout << "  • Keep constructors simple\n";
    cout << "  • Use delegating constructors to reduce duplication\n";
    cout << "  • Make objects usable after construction\n";
    cout << "\n";
    cout << "❌ DON'T:\n";
    cout << "  • Return values (constructors have no return type)\n";
    cout << "  • Call virtual functions in constructor\n";
    cout << "  • Throw exceptions unless necessary\n";
    cout << "  • Do complex operations\n";
    cout << "  • Leave members uninitialized\n";

    cout << "\n========================================\n";
    cout << "COMPILER-GENERATED CONSTRUCTORS\n";
    cout << "========================================\n";
    cout << "If you don't define any constructors:\n";
    cout << "  • Compiler generates default constructor\n";
    cout << "  • MyClass() { }  // Empty, does nothing\n";
    cout << "\n";
    cout << "If you define ANY constructor:\n";
    cout << "  • Compiler does NOT generate default constructor\n";
    cout << "  • Must explicitly define it if you need it\n";
    cout << "\n";
    cout << "Example:\n";
    cout << "  class MyClass {\n";
    cout << "      MyClass(int x) { }  // Custom constructor\n";
    cout << "  };\n";
    cout << "  \n";
    cout << "  MyClass obj;     // ❌ Error! No default constructor\n";
    cout << "  MyClass obj(10); // ✅ OK\n";

    cout << "\n========================================\n";
    cout << "EXPLICIT KEYWORD (Preview)\n";
    cout << "========================================\n";
    cout << "Prevents implicit conversions:\n";
    cout << "\n";
    cout << "class MyClass {\n";
    cout << "public:\n";
    cout << "    explicit MyClass(int x) { }\n";
    cout << "};\n";
    cout << "\n";
    cout << "MyClass obj1(10);    // ✅ OK\n";
    cout << "MyClass obj2 = 10;   // ❌ Error! Can't implicitly convert\n";
    cout << "\n";
    cout << "Use 'explicit' for single-parameter constructors\n";
    cout << "to prevent unexpected conversions.\n";

    cout << "\n========================================\n";
    cout << "COMMON MISTAKES\n";
    cout << "========================================\n";
    cout << "❌ Mistake 1: Forgetting to initialize\n";
    cout << "   MyClass() {  // data not initialized!\n";
    cout << "   }\n";
    cout << "\n";
    cout << "❌ Mistake 2: Wrong initialization order\n";
    cout << "   MyClass(int a, int b) : y(b), x(a) { }\n";
    cout << "   // But x declared before y - confusing!\n";
    cout << "\n";
    cout << "❌ Mistake 3: Assignment instead of initialization\n";
    cout << "   MyClass(const string& s) {\n";
    cout << "       name = s;  // Assignment (less efficient)\n";
    cout << "   }\n";
    cout << "   Better: MyClass(const string& s) : name(s) { }\n";
    cout << "\n";
    cout << "❌ Mistake 4: Most vexing parse\n";
    cout << "   MyClass obj();  // Not a default constructor call!\n";
    cout << "                   // It's a function declaration!\n";
    cout << "   Correct: MyClass obj;  or  MyClass obj{};\n";

    cout << "\n========================================\n";
    cout << "CONSTRUCTOR vs DESTRUCTOR\n";
    cout << "========================================\n";
    cout << "Constructor:\n";
    cout << "  • Called when object is created\n";
    cout << "  • Initializes the object\n";
    cout << "  • Can have parameters\n";
    cout << "  • Can be overloaded\n";
    cout << "\n";
    cout << "Destructor:\n";
    cout << "  • Called when object is destroyed\n";
    cout << "  • Cleans up resources\n";
    cout << "  • No parameters\n";
    cout << "  • Cannot be overloaded\n";
    cout << "  • Covered in 03_Destructor\n";

    cout << "\n========================================\n";
    cout << "QUICK REFERENCE\n";
    cout << "========================================\n";
    cout << "// Default Constructor\n";
    cout << "class MyClass {\n";
    cout << "    MyClass() { x = 0; }\n";
    cout << "};\n";
    cout << "MyClass obj;  // Calls default constructor\n";
    cout << "\n";
    cout << "// Parameterized Constructor\n";
    cout << "class MyClass {\n";
    cout << "    MyClass(int a) : x(a) { }\n";
    cout << "};\n";
    cout << "MyClass obj(42);  // Calls parameterized constructor\n";
    cout << "\n";
    cout << "// Constructor Overloading\n";
    cout << "class MyClass {\n";
    cout << "    MyClass() { }\n";
    cout << "    MyClass(int a) { }\n";
    cout << "    MyClass(int a, int b) { }\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "KEY TAKEAWAYS\n";
    cout << "========================================\n";
    cout << "1. Constructor = Special function to initialize objects\n";
    cout << "2. Same name as class, no return type\n";
    cout << "3. Automatically called when object created\n";
    cout << "4. Can be overloaded (multiple versions)\n";
    cout << "5. Use initializer list (more efficient)\n";
    cout << "6. Always initialize all member variables\n";
    cout << "7. Validate input in constructor\n";
    cout << "8. Keep constructors simple and focused\n";

    return 0;
}
