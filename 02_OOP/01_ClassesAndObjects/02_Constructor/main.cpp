#include <print>
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
        std::println("Name: {}, Age: {}, ID: {}", name, age, id);
    }
};

void demonstrateWithoutConstructor() {
    std::println("");
    std::println("╔════════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: WITHOUT CONSTRUCTOR        ║");
    std::println("╚════════════════════════════════════════╝");
    std::println("");

    std::println("--- Problem: Uninitialized Variables ---");
    StudentWithoutConstructor student;

    std::println("⚠️  Without constructor, variables have garbage values:");
    // student.display();  // Would show garbage values!

    std::println("Must manually initialize:");
    student.name = "Alice";
    student.age = 20;
    student.id = 12345;
    student.display();
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
        std::println("🏗️  Default constructor called!");
        name = "Unknown";
        age = 0;
        id = 0;
    }

    void display() {
        std::println("Name: {}, Age: {}, ID: {}", name, age, id);
    }
};

void demonstrateDefaultConstructor() {
    std::println("");
    std::println("╔════════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: DEFAULT CONSTRUCTOR        ║");
    std::println("╚════════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating object with default constructor ---");
    StudentWithConstructor student;  // Constructor automatically called!

    std::println("");
    std::println("✅ Variables automatically initialized:");
    student.display();
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
        std::println("🏗️  Parameterized constructor called for {}", b);
        brand = b;
        model = m;
        year = y;
        price = p;
    }

    void display() {
        std::println("🚗 {} {} {} - ${}", year, brand, model, price);
    }
};

void demonstrateParameterizedConstructor() {
    std::println("");
    std::println("╔════════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: PARAMETERIZED CONSTRUCTOR  ║");
    std::println("╚════════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating cars with initial values ---");
    Car car1("Toyota", "Camry", 2024, 28000);
    Car car2("Tesla", "Model 3", 2024, 42000);
    Car car3("BMW", "M3", 2023, 75000);

    std::println("");
    std::println("--- Displaying cars ---");
    car1.display();
    car2.display();
    car3.display();

    std::println("");
    std::println("✅ Objects created and initialized in one step!");
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
        std::println("🏗️  Default constructor called");
    }


    // Constructor 2: With holder and number only
    BankAccount(string holder, int number) {
        std::println("🏗️  Constructor with 2 parameters called");
        accountHolder = holder;
        accountNumber = number;
        balance = 0.0;  // Default balance
    }

    // Constructor 3: With all parameters
    BankAccount(string holder, int number, double initialBalance) {
        std::println("🏗️  Constructor with 3 parameters called");
        accountHolder = holder;
        accountNumber = number;
        balance = initialBalance;
    }

    void display() {
        std::println("💳 Account: {} (#{}) - Balance: ${}", accountHolder, accountNumber, balance);
    }
};

void demonstrateConstructorOverloading() {
    std::println("");
    std::println("╔════════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: CONSTRUCTOR OVERLOADING    ║");
    std::println("╚════════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating accounts with different constructors ---");
    std::println("");

    std::println("Account 1: Default constructor");
    BankAccount acc1;
    acc1.display();

    std::println("");
    std::println("Account 2: Constructor with holder and number");
    BankAccount acc2("Alice Johnson", 12345);
    acc2.display();

    std::println("");
    std::println("Account 3: Constructor with all parameters");
    BankAccount acc3("Bob Smith", 67890, 5000.00);
    acc3.display();

    std::println("");
    std::println("✅ Same class, different ways to create objects!");
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
        std::println("🏗️  Constructor called with width={}, height={}", w, h);
        width = w;
        height = h;
    }

    double area() {
        return width * height;
    }

    void display() {
        std::println("📐 Rectangle: {} × {} = {} sq units", width, height, area());
    }
};

void demonstrateDefaultArguments() {
    std::println("");
    std::println("╔════════════════════════════════════════╗");
    std::println("║  EXAMPLE 5: DEFAULT ARGUMENTS          ║");
    std::println("╚════════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating rectangles with different arguments ---");
    std::println("");

    std::println("Rectangle 1: No arguments (uses defaults)");
    Rectangle rect1;
    rect1.display();

    std::println("");
    std::println("Rectangle 2: One argument (width)");
    Rectangle rect2(5.0);
    rect2.display();

    std::println("");
    std::println("Rectangle 3: Both arguments");
    Rectangle rect3(4.0, 6.0);
    rect3.display();

    std::println("");
    std::println("✅ One constructor handles all cases!");
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
        std::println("🏗️  Point constructor: ({}, {})", x, y);
        // x = xVal;  // ❌ Error! Can't assign to const
        // y = yVal;  // ❌ Error! Can't assign to const
    }

    void display() {
        std::println("📍 Point: ({}, {})", x, y);
    }
};

class Circle {
public:
    Point center;
    double radius;

    // Must use initializer list for Point (no default constructor)
    Circle(int x, int y, double r) : center(x, y), radius(r) {
        std::println("🏗️  Circle constructor");
    }

    double area() {
        return 3.14159 * radius * radius;
    }

    void display() {
        std::print("⭕ Circle at ");
        center.display();
        std::println("   Radius: {}, Area: {}", radius, area());
    }
};

void demonstrateMemberInitializerList() {
    std::println("");
    std::println("╔════════════════════════════════════════╗");
    std::println("║  EXAMPLE 6: MEMBER INITIALIZER LIST    ║");
    std::println("╚════════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating Point with const members ---");
    Point p(10, 20);
    p.display();

    std::println("");
    std::println("--- Creating Circle with Point member ---");
    Circle c(5, 5, 10.0);
    c.display();
}

// ============================================
// EXAMPLE 7: CONSTRUCTOR INITIALIZATION ORDER
// ============================================

class InitializationOrder {
public:
    int first;
    int second;
    int third;

    // The initializer list says: third, first, second.
    // But members are ALWAYS initialized in DECLARATION order:
    // first, second, third (the compiler warns about this with -Wreorder).
    InitializationOrder(int a, int b, int c) : third(c), first(a), second(b) {
        std::println("🏗️  Constructor body");
    }

    void display() {
        std::println("Values: first={}, second={}, third={}", first, second, third);
    }
};

void demonstrateInitializationOrder() {
    std::println("");
    std::println("╔════════════════════════════════════════╗");
    std::println("║  EXAMPLE 7: INITIALIZATION ORDER       ║");
    std::println("╚════════════════════════════════════════╝");
    std::println("");

    std::println("--- Order matters! ---");

    InitializationOrder obj(1, 2, 3);
    obj.display();
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
        std::println("🏗️  Main constructor called for {}", name);
    }

    // Delegating constructor 1: Just name
    Employee(string n) : Employee(n, 0, 0.0, "Unassigned") {
        std::println("   Delegated from name-only constructor");
    }

    // Delegating constructor 2: Name and ID
    Employee(string n, int i) : Employee(n, i, 0.0, "Unassigned") {
        std::println("   Delegated from name-id constructor");
    }

    void display() {
        std::println("👤 Employee: {} (#{})", name, id);
        std::println("   Dept: {}, Salary: ${}", department, salary);
    }
};

void demonstrateDelegatingConstructors() {
    std::println("");
    std::println("╔════════════════════════════════════════╗");
    std::println("║  EXAMPLE 8: DELEGATING CONSTRUCTORS    ║");
    std::println("╚════════════════════════════════════════╝");
    std::println("");

    std::println("--- Employee 1: Name only ---");
    Employee emp1("Alice");
    emp1.display();

    std::println("");
    std::println("--- Employee 2: Name and ID ---");
    Employee emp2("Bob", 12345);
    emp2.display();

    std::println("");
    std::println("--- Employee 3: All parameters ---");
    Employee emp3("Charlie", 67890, 75000, "Engineering");
    emp3.display();

    std::println("");
    std::println("✅ Delegating reduces code duplication!");
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

        std::println("🎮 Created {} (Level {})", name, level);
        std::println("   ❤️  HP: {}/{}", health, maxHealth);
        std::println("   ⚔️  Attack: {}", attackPower);
        std::println("   🛡️  Defense: {}", defense);
    }

    void displayStats() {
        std::println("");
        std::println("⚔️  {} - Level {}", name, level);
        std::println("   ❤️  HP: {}/{}", health, maxHealth);
        std::println("   ⚔️  ATK: {}", attackPower);
        std::println("   🛡️  DEF: {}", defense);
    }
};

void demonstrateGameCharacter() {
    std::println("");
    std::println("╔════════════════════════════════════════╗");
    std::println("║  EXAMPLE 9: GAME CHARACTER             ║");
    std::println("╚════════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating characters at different levels ---");
    std::println("");

    GameCharacter warrior("Warrior", 1);
    GameCharacter mage("Mage", 5);
    GameCharacter boss("Dragon Boss", 10);

    std::println("");
    std::println("--- All Character Stats ---");
    warrior.displayStats();
    mage.displayStats();
    boss.displayStats();

    std::println("");
    std::println("✅ Constructor calculates all stats automatically!");
}

// ============================================
// EXAMPLE 10: VALIDATION IN CONSTRUCTOR
// ============================================

class Temperature {
public:
    double celsius;

    // Constructor with validation
    Temperature(double temp) {
        std::println("🏗️  Creating Temperature object...");

        // Validate input
        if (temp < -273.15) {  // Absolute zero
            std::println("⚠️  Temperature below absolute zero! Setting to -273.15°C");
            celsius = -273.15;
        } else {
            celsius = temp;
        }

        std::println("   Temperature set to: {}°C", celsius);
    }

    double toFahrenheit() {
        return (celsius * 9.0 / 5.0) + 32;
    }

    double toKelvin() {
        return celsius + 273.15;
    }

    void display() {
        std::println("🌡️  Temperature:");
        std::println("   Celsius:    {}°C", celsius);
        std::println("   Fahrenheit: {}°F", toFahrenheit());
        std::println("   Kelvin:     {}K", toKelvin());
    }
};

void demonstrateValidation() {
    std::println("");
    std::println("╔════════════════════════════════════════╗");
    std::println("║  EXAMPLE 10: VALIDATION                ║");
    std::println("╚════════════════════════════════════════╝");
    std::println("");

    std::println("--- Valid temperature ---");
    Temperature t1(25.0);
    t1.display();

    std::println("");
    std::println("--- Invalid temperature (too low) ---");
    Temperature t2(-300.0);  // Below absolute zero!
    t2.display();

    std::println("");
    std::println("✅ Constructor ensures object is always in valid state!");
}

// ============================================
// MAIN DEMONSTRATION
// ============================================

int main() {
    std::println("========================================");
    std::println("CONSTRUCTOR DEMONSTRATION");
    std::println("========================================");

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

    return 0;
}
