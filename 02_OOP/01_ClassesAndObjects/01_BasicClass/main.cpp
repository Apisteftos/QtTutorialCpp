#include <iostream>
#include <string>
using namespace std;

// ============================================
// WHAT IS A CLASS?
// ============================================

/*
 * A CLASS is a blueprint/template for creating objects
 *
 * Think of it like:
 * - Class = Blueprint for a house
 * - Object = Actual house built from blueprint
 *
 * Class defines:
 * - Data (attributes/properties)
 * - Functions (methods/behaviors)
 */

// ============================================
// EXAMPLE 1: SIMPLEST CLASS
// ============================================

class SimpleClass {
public:
    int number;

    void display() {
        cout << "Number is: " << number << "\n";
    }
};

void demonstrateSimpleClass() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: SIMPLEST CLASS        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    // Creating an object (instance of the class)
    SimpleClass obj;

    // Accessing member variable
    obj.number = 42;

    // Calling member function
    obj.display();

    cout << "✓ Object created and used successfully!\n";
}

// ============================================
// EXAMPLE 2: REAL-WORLD CLASS - STUDENT
// ============================================

class Student {
public:
    // Member variables (attributes/properties)
    string name;
    int age;
    int studentId;
    double gpa;

    // Member function (method/behavior)
    void displayInfo() {
        cout << "╔════════════════════════════════╗\n";
        cout << "║       STUDENT INFORMATION      ║\n";
        cout << "╚════════════════════════════════╝\n";
        cout << "Name:       " << name << "\n";
        cout << "Age:        " << age << " years\n";
        cout << "Student ID: " << studentId << "\n";
        cout << "GPA:        " << gpa << "\n";
    }

    void study(const string& subject) {
        cout << "📚 " << name << " is studying " << subject << "\n";
    }

    void takeExam(const string& subject, double score) {
        cout << "📝 " << name << " took " << subject << " exam\n";
        cout << "   Score: " << score << "/100\n";
    }
};

void demonstrateStudentClass() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: STUDENT CLASS          ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    // Creating a student object
    Student student1;

    // Setting member variables
    student1.name = "Alice Johnson";
    student1.age = 20;
    student1.studentId = 12345;
    student1.gpa = 3.8;

    // Calling member functions
    student1.displayInfo();

    cout << "\n--- Student Activities ---\n";
    student1.study("Mathematics");
    student1.study("Physics");
    student1.takeExam("Mathematics", 95.5);
}

// ============================================
// EXAMPLE 3: MULTIPLE OBJECTS
// ============================================

class Car {
public:
    string brand;
    string model;
    int year;
    string color;
    double price;

    void displayInfo() {
        cout << "🚗 " << year << " " << brand << " " << model << "\n";
        cout << "   Color: " << color << "\n";
        cout << "   Price: $" << price << "\n";
    }

    void drive() {
        cout << "🏁 Driving the " << brand << " " << model << "... Vroom!\n";
    }

    void honk() {
        cout << "📢 " << brand << " says: BEEP BEEP!\n";
    }
};

void demonstrateMultipleObjects() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: MULTIPLE OBJECTS       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    // Creating multiple car objects
    Car car1, car2, car3;

    // First car
    cout << "--- Car 1 ---\n";
    car1.brand = "Toyota";
    car1.model = "Camry";
    car1.year = 2024;
    car1.color = "Silver";
    car1.price = 28000.00;
    car1.displayInfo();

    // Second car
    cout << "\n--- Car 2 ---\n";
    car2.brand = "Tesla";
    car2.model = "Model 3";
    car2.year = 2024;
    car2.color = "White";
    car2.price = 42000.00;
    car2.displayInfo();

    // Third car
    cout << "\n--- Car 3 ---\n";
    car3.brand = "BMW";
    car3.model = "M3";
    car3.year = 2023;
    car3.color = "Black";
    car3.price = 75000.00;
    car3.displayInfo();

    // All cars in action
    cout << "\n--- Cars in Action ---\n";
    car1.drive();
    car2.drive();
    car3.drive();

    cout << "\n--- Traffic Jam ---\n";
    car1.honk();
    car2.honk();
    car3.honk();
}

// ============================================
// EXAMPLE 4: CLASS WITH METHODS
// ============================================

class Calculator {
public:
    double num1;
    double num2;

    double add() {
        return num1 + num2;
    }

    double subtract() {
        return num1 - num2;
    }

    double multiply() {
        return num1 * num2;
    }

    double divide() {
        if (num2 != 0) {
            return num1 / num2;
        } else {
            cout << "❌ Error: Division by zero!\n";
            return 0;
        }
    }

    void displayOperations() {
        cout << "Calculator Operations:\n";
        cout << "  " << num1 << " + " << num2 << " = " << add() << "\n";
        cout << "  " << num1 << " - " << num2 << " = " << subtract() << "\n";
        cout << "  " << num1 << " × " << num2 << " = " << multiply() << "\n";
        if (num2 != 0) {
            cout << "  " << num1 << " ÷ " << num2 << " = " << divide() << "\n";
        }
    }
};

void demonstrateCalculator() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: CALCULATOR CLASS       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Calculator calc;

    cout << "--- Operation Set 1 ---\n";
    calc.num1 = 15.5;
    calc.num2 = 3.5;
    calc.displayOperations();

    cout << "\n--- Operation Set 2 ---\n";
    calc.num1 = 100;
    calc.num2 = 25;
    calc.displayOperations();
}

// ============================================
// EXAMPLE 5: CLASS WITH DATA AND BEHAVIOR
// ============================================

class BankAccount {
public:
    string accountHolder;
    int accountNumber;
    double balance;

    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "💰 Deposited $" << amount << "\n";
            cout << "   New balance: $" << balance << "\n";
        } else {
            cout << "❌ Invalid deposit amount\n";
        }
    }

    void withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            cout << "💸 Withdrew $" << amount << "\n";
            cout << "   New balance: $" << balance << "\n";
        } else if (amount > balance) {
            cout << "❌ Insufficient funds! Balance: $" << balance << "\n";
        } else {
            cout << "❌ Invalid withdrawal amount\n";
        }
    }

    void displayBalance() {
        cout << "💵 Account Balance: $" << balance << "\n";
    }

    void displayAccountInfo() {
        cout << "╔════════════════════════════════╗\n";
        cout << "║      ACCOUNT INFORMATION       ║\n";
        cout << "╚════════════════════════════════╝\n";
        cout << "Account Holder: " << accountHolder << "\n";
        cout << "Account Number: " << accountNumber << "\n";
        cout << "Balance:        $" << balance << "\n";
    }
};

void demonstrateBankAccount() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: BANK ACCOUNT CLASS     ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    BankAccount account;

    // Initialize account
    account.accountHolder = "John Smith";
    account.accountNumber = 987654321;
    account.balance = 1000.00;

    account.displayAccountInfo();

    cout << "\n--- Transactions ---\n";
    account.deposit(500.00);
    cout << "\n";
    account.withdraw(200.00);
    cout << "\n";
    account.withdraw(2000.00);  // This should fail
    cout << "\n";
    account.displayBalance();
}

// ============================================
// EXAMPLE 6: CLASS REPRESENTING A GAME CHARACTER
// ============================================

class GameCharacter {
public:
    string name;
    int health;
    int attackPower;
    int level;

    void displayStats() {
        cout << "⚔️  " << name << " (Level " << level << ")\n";
        cout << "   ❤️  Health: " << health << " HP\n";
        cout << "   ⚡ Attack: " << attackPower << "\n";
    }

    void attack(GameCharacter& target) {
        cout << "⚔️  " << name << " attacks " << target.name << "!\n";
        target.takeDamage(attackPower);
    }

    void takeDamage(int damage) {
        health -= damage;
        cout << "   💥 Took " << damage << " damage!\n";
        if (health <= 0) {
            health = 0;
            cout << "   ☠️  " << name << " has been defeated!\n";
        } else {
            cout << "   ❤️  " << name << " has " << health << " HP remaining\n";
        }
    }

    void heal(int amount) {
        health += amount;
        cout << "💚 " << name << " healed " << amount << " HP\n";
        cout << "   Current health: " << health << " HP\n";
    }

    void levelUp() {
        level++;
        health += 20;
        attackPower += 5;
        cout << "⭐ " << name << " leveled up to Level " << level << "!\n";
        cout << "   Health: +" << 20 << " (now " << health << ")\n";
        cout << "   Attack: +" << 5 << " (now " << attackPower << ")\n";
    }
};

void demonstrateGameCharacter() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: GAME CHARACTER         ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    // Create two characters
    GameCharacter hero, monster;

    // Initialize hero
    hero.name = "Warrior";
    hero.health = 100;
    hero.attackPower = 25;
    hero.level = 5;

    // Initialize monster
    monster.name = "Dragon";
    monster.health = 80;
    monster.attackPower = 20;
    monster.level = 4;

    cout << "--- Initial Stats ---\n";
    hero.displayStats();
    cout << "\n";
    monster.displayStats();

    cout << "\n--- Battle! ---\n";
    hero.attack(monster);
    cout << "\n";
    monster.attack(hero);
    cout << "\n";
    hero.attack(monster);
    cout << "\n";
    hero.attack(monster);

    cout << "\n--- Hero Levels Up! ---\n";
    hero.levelUp();

    cout << "\n--- Hero Heals ---\n";
    hero.heal(30);
}

// ============================================
// MAIN DEMONSTRATION
// ============================================

int main() {
    cout << "========================================\n";
    cout << "BASIC CLASS DEMONSTRATION\n";
    cout << "========================================\n";

    demonstrateSimpleClass();
    demonstrateStudentClass();
    demonstrateMultipleObjects();
    demonstrateCalculator();
    demonstrateBankAccount();
    demonstrateGameCharacter();

    // ============================================
    // Comprehensive Summary
    // ============================================
    cout << "\n========================================\n";
    cout << "WHAT IS A CLASS?\n";
    cout << "========================================\n";
    cout << "A class is a user-defined data type that:\n";
    cout << "  • Groups related data (member variables)\n";
    cout << "  • Groups related functions (member functions)\n";
    cout << "  • Acts as a blueprint for creating objects\n";
    cout << "\n";
    cout << "Think of it as:\n";
    cout << "  Class  = Blueprint for a house\n";
    cout << "  Object = Actual house built from blueprint\n";

    cout << "\n========================================\n";
    cout << "CLASS STRUCTURE\n";
    cout << "========================================\n";
    cout << "class ClassName {\n";
    cout << "public:\n";
    cout << "    // Member variables (data)\n";
    cout << "    int number;\n";
    cout << "    string name;\n";
    cout << "    \n";
    cout << "    // Member functions (behavior)\n";
    cout << "    void doSomething() {\n";
    cout << "        // Function code\n";
    cout << "    }\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "CREATING AND USING OBJECTS\n";
    cout << "========================================\n";
    cout << "// 1. Create an object\n";
    cout << "Student student1;\n";
    cout << "\n";
    cout << "// 2. Set member variables\n";
    cout << "student1.name = \"Alice\";\n";
    cout << "student1.age = 20;\n";
    cout << "\n";
    cout << "// 3. Call member functions\n";
    cout << "student1.displayInfo();\n";
    cout << "student1.study(\"Math\");\n";

    cout << "\n========================================\n";
    cout << "KEY CONCEPTS\n";
    cout << "========================================\n";
    cout << "Class:\n";
    cout << "  • Template/blueprint for objects\n";
    cout << "  • Defines structure and behavior\n";
    cout << "  • Written once, used many times\n";
    cout << "\n";
    cout << "Object:\n";
    cout << "  • Instance of a class\n";
    cout << "  • Actual entity in memory\n";
    cout << "  • Can create many objects from one class\n";
    cout << "\n";
    cout << "Member Variables:\n";
    cout << "  • Data stored in the object\n";
    cout << "  • Also called: attributes, properties, fields\n";
    cout << "  • Example: name, age, balance\n";
    cout << "\n";
    cout << "Member Functions:\n";
    cout << "  • Operations on the object's data\n";
    cout << "  • Also called: methods, behaviors\n";
    cout << "  • Example: displayInfo(), deposit(), attack()\n";

    cout << "\n========================================\n";
    cout << "DOT OPERATOR (.)\n";
    cout << "========================================\n";
    cout << "The dot (.) is used to access class members:\n";
    cout << "\n";
    cout << "object.memberVariable    // Access variable\n";
    cout << "object.memberFunction()  // Call function\n";
    cout << "\n";
    cout << "Examples:\n";
    cout << "  car.brand = \"Toyota\";    // Set variable\n";
    cout << "  car.drive();              // Call function\n";
    cout << "  int x = car.year;         // Get variable\n";

    cout << "\n========================================\n";
    cout << "PUBLIC vs PRIVATE (Preview)\n";
    cout << "========================================\n";
    cout << "public:\n";
    cout << "  • Accessible from outside the class\n";
    cout << "  • Can be accessed using object.member\n";
    cout << "  • Example: obj.name, obj.display()\n";
    cout << "\n";
    cout << "private:\n";
    cout << "  • Only accessible inside the class\n";
    cout << "  • Cannot be accessed from outside\n";
    cout << "  • Used for encapsulation (next lesson!)\n";
    cout << "\n";
    cout << "Note: We'll cover this in detail in 05_Encapsulation!\n";

    cout << "\n========================================\n";
    cout << "REAL-WORLD ANALOGIES\n";
    cout << "========================================\n";
    cout << "Student Class:\n";
    cout << "  • Variables: name, age, ID, GPA\n";
    cout << "  • Functions: study(), takeExam(), displayInfo()\n";
    cout << "\n";
    cout << "Car Class:\n";
    cout << "  • Variables: brand, model, year, color\n";
    cout << "  • Functions: drive(), honk(), displayInfo()\n";
    cout << "\n";
    cout << "BankAccount Class:\n";
    cout << "  • Variables: accountNumber, balance, holder\n";
    cout << "  • Functions: deposit(), withdraw(), displayBalance()\n";
    cout << "\n";
    cout << "GameCharacter Class:\n";
    cout << "  • Variables: name, health, attackPower, level\n";
    cout << "  • Functions: attack(), takeDamage(), heal(), levelUp()\n";

    cout << "\n========================================\n";
    cout << "CLASS vs STRUCT\n";
    cout << "========================================\n";
    cout << "In C++, class and struct are almost identical:\n";
    cout << "\n";
    cout << "class MyClass {     struct MyStruct {\n";
    cout << "    // default:        // default:\n";
    cout << "    // private           // public\n";
    cout << "}                   }\n";
    cout << "\n";
    cout << "Difference:\n";
    cout << "  • class: members are private by default\n";
    cout << "  • struct: members are public by default\n";
    cout << "\n";
    cout << "Convention:\n";
    cout << "  • Use 'class' for complex types with behavior\n";
    cout << "  • Use 'struct' for simple data containers\n";

    cout << "\n========================================\n";
    cout << "BENEFITS OF CLASSES\n";
    cout << "========================================\n";
    cout << "1. Organization:\n";
    cout << "   • Groups related data and functions\n";
    cout << "   • Makes code more organized\n";
    cout << "\n";
    cout << "2. Reusability:\n";
    cout << "   • Write once, use many times\n";
    cout << "   • Create multiple objects from one class\n";
    cout << "\n";
    cout << "3. Abstraction:\n";
    cout << "   • Hide complex details\n";
    cout << "   • Show only what's necessary\n";
    cout << "\n";
    cout << "4. Maintainability:\n";
    cout << "   • Changes in one place affect all objects\n";
    cout << "   • Easier to debug and update\n";
    cout << "\n";
    cout << "5. Real-world modeling:\n";
    cout << "   • Represent real entities in code\n";
    cout << "   • Natural way to think about problems\n";

    cout << "\n========================================\n";
    cout << "COMMON TERMINOLOGY\n";
    cout << "========================================\n";
    cout << "Class:\n";
    cout << "  • Also called: Type, Blueprint, Template\n";
    cout << "\n";
    cout << "Object:\n";
    cout << "  • Also called: Instance\n";
    cout << "  • Example: student1, car2 are instances\n";
    cout << "\n";
    cout << "Member Variables:\n";
    cout << "  • Also called: Attributes, Properties, Fields, Data Members\n";
    cout << "\n";
    cout << "Member Functions:\n";
    cout << "  • Also called: Methods, Behaviors, Operations, Member Functions\n";
    cout << "\n";
    cout << "Instantiation:\n";
    cout << "  • The process of creating an object from a class\n";
    cout << "  • Example: Student s; // Instantiating a Student\n";

    cout << "\n========================================\n";
    cout << "SYNTAX PATTERNS\n";
    cout << "========================================\n";
    cout << "Declaring a class:\n";
    cout << "  class ClassName { ... };\n";
    cout << "  Note: Semicolon after closing brace!\n";
    cout << "\n";
    cout << "Creating an object:\n";
    cout << "  ClassName objectName;        // Stack\n";
    cout << "  ClassName* ptr = new ClassName; // Heap (pointer)\n";
    cout << "\n";
    cout << "Accessing members:\n";
    cout << "  object.member      // Stack object\n";
    cout << "  ptr->member        // Heap object (pointer)\n";

    cout << "\n========================================\n";
    cout << "OBJECT LIFETIME\n";
    cout << "========================================\n";
    cout << "Stack Objects:\n";
    cout << "  Student s1;  // Created on stack\n";
    cout << "  • Automatic lifetime\n";
    cout << "  • Destroyed when scope ends\n";
    cout << "  • No manual cleanup needed\n";
    cout << "\n";
    cout << "Heap Objects:\n";
    cout << "  Student* s2 = new Student;  // Created on heap\n";
    cout << "  • Manual lifetime control\n";
    cout << "  • Exists until 'delete' called\n";
    cout << "  • Must remember to delete!\n";
    cout << "  delete s2;\n";

    cout << "\n========================================\n";
    cout << "MULTIPLE OBJECTS\n";
    cout << "========================================\n";
    cout << "You can create multiple objects from one class:\n";
    cout << "\n";
    cout << "Student alice, bob, charlie;\n";
    cout << "alice.name = \"Alice\";\n";
    cout << "bob.name = \"Bob\";\n";
    cout << "charlie.name = \"Charlie\";\n";
    cout << "\n";
    cout << "Each object:\n";
    cout << "  • Has its own copy of member variables\n";
    cout << "  • Is independent of other objects\n";
    cout << "  • Shares the same member functions (code)\n";

    cout << "\n========================================\n";
    cout << "DESIGN TIPS\n";
    cout << "========================================\n";
    cout << "Good Class Design:\n";
    cout << "  ✓ Clear, descriptive class name (Student, not S)\n";
    cout << "  ✓ Related data grouped together\n";
    cout << "  ✓ Functions operate on class data\n";
    cout << "  ✓ Represents one concept/entity\n";
    cout << "\n";
    cout << "Naming Conventions:\n";
    cout << "  • Class names: PascalCase (Student, BankAccount)\n";
    cout << "  • Variables: camelCase (studentId, accountNumber)\n";
    cout << "  • Functions: camelCase (displayInfo, calculateGPA)\n";
    cout << "\n";
    cout << "Class should represent:\n";
    cout << "  ✓ A thing (Student, Car, Account)\n";
    cout << "  ✓ A concept (Calculator, Timer, Logger)\n";
    cout << "  ✓ An entity (Player, Enemy, Item)\n";

    cout << "\n========================================\n";
    cout << "WHAT'S NEXT?\n";
    cout << "========================================\n";
    cout << "Now that you understand basic classes, next topics:\n";
    cout << "\n";
    cout << "02_Constructor:\n";
    cout << "  • Automatic initialization\n";
    cout << "  • How to set initial values\n";
    cout << "\n";
    cout << "03_Destructor:\n";
    cout << "  • Automatic cleanup\n";
    cout << "  • Resource management\n";
    cout << "\n";
    cout << "05_Encapsulation:\n";
    cout << "  • Private vs public\n";
    cout << "  • Data hiding\n";
    cout << "  • Getters and setters\n";

    cout << "\n========================================\n";
    cout << "QUICK REFERENCE\n";
    cout << "========================================\n";
    cout << "// Define class\n";
    cout << "class MyClass {\n";
    cout << "public:\n";
    cout << "    int data;              // Member variable\n";
    cout << "    void doSomething();    // Member function\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Create object\n";
    cout << "MyClass obj;\n";
    cout << "\n";
    cout << "// Use object\n";
    cout << "obj.data = 42;             // Set variable\n";
    cout << "obj.doSomething();         // Call function\n";

    cout << "\n========================================\n";
    cout << "KEY TAKEAWAYS\n";
    cout << "========================================\n";
    cout << "1. Class = Blueprint, Object = Instance\n";
    cout << "2. Classes group data and functions together\n";
    cout << "3. Use dot (.) to access members\n";
    cout << "4. Can create multiple objects from one class\n";
    cout << "5. Each object has its own data\n";
    cout << "6. Objects share the same functions\n";
    cout << "7. Classes help organize and structure code\n";
    cout << "8. Represent real-world entities in code\n";

    return 0;
}
