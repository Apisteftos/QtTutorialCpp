#include <print>
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
        std::println("Number is: {}", number);
    }
};

void demonstrateSimpleClass() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: SIMPLEST CLASS         ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    // Creating an object (instance of the class)
    SimpleClass obj;

    // Accessing member variable
    obj.number = 42;

    // Calling member function
    obj.display();

    std::println("✓ Object created and used successfully!");
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
        std::println("╔════════════════════════════════╗");
        std::println("║       STUDENT INFORMATION      ║");
        std::println("╚════════════════════════════════╝");
        std::println("Name:       {}", name);
        std::println("Age:        {} years", age);
        std::println("Student ID: {}", studentId);
        std::println("GPA:        {}", gpa);
    }

    void study(const string& subject) {
        std::println("📚 {} is studying {}", name, subject);
    }

    void takeExam(const string& subject, double score) {
        std::println("📝 {} took {} exam", name, subject);
        std::println("   Score: {}/100", score);
    }
};

void demonstrateStudentClass() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: STUDENT CLASS          ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    // Creating a student object
    Student student1;

    // Setting member variables
    student1.name = "Alice Johnson";
    student1.age = 20;
    student1.studentId = 12345;
    student1.gpa = 3.8;

    // Calling member functions
    student1.displayInfo();

    std::println("");
    std::println("--- Student Activities ---");
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
        std::println("🚗 {} {} {}", year, brand, model);
        std::println("   Color: {}", color);
        std::println("   Price: ${}", price);
    }

    void drive() {
        std::println("🏁 Driving the {} {}... Vroom!", brand, model);
    }

    void honk() {
        std::println("📢 {} says: BEEP BEEP!", brand);
    }
};

void demonstrateMultipleObjects() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: MULTIPLE OBJECTS       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    // Creating multiple car objects
    Car car1, car2, car3;

    // First car
    std::println("--- Car 1 ---");
    car1.brand = "Toyota";
    car1.model = "Camry";
    car1.year = 2024;
    car1.color = "Silver";
    car1.price = 28000.00;
    car1.displayInfo();

    // Second car
    std::println("");
    std::println("--- Car 2 ---");
    car2.brand = "Tesla";
    car2.model = "Model 3";
    car2.year = 2024;
    car2.color = "White";
    car2.price = 42000.00;
    car2.displayInfo();

    // Third car
    std::println("");
    std::println("--- Car 3 ---");
    car3.brand = "BMW";
    car3.model = "M3";
    car3.year = 2023;
    car3.color = "Black";
    car3.price = 75000.00;
    car3.displayInfo();

    // All cars in action
    std::println("");
    std::println("--- Cars in Action ---");
    car1.drive();
    car2.drive();
    car3.drive();

    std::println("");
    std::println("--- Traffic Jam ---");
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
            std::println("❌ Error: Division by zero!");
            return 0;
        }
    }

    void displayOperations() {
        std::println("Calculator Operations:");
        std::println("  {} + {} = {}", num1, num2, add());
        std::println("  {} - {} = {}", num1, num2, subtract());
        std::println("  {} × {} = {}", num1, num2, multiply());
        if (num2 != 0) {
            std::println("  {} ÷ {} = {}", num1, num2, divide());
        }
    }
};

void demonstrateCalculator() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: CALCULATOR CLASS       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Calculator calc;

    std::println("--- Operation Set 1 ---");
    calc.num1 = 15.5;
    calc.num2 = 3.5;
    calc.displayOperations();

    std::println("");
    std::println("--- Operation Set 2 ---");
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
            std::println("💰 Deposited ${}", amount);
            std::println("   New balance: ${}", balance);
        } else {
            std::println("❌ Invalid deposit amount");
        }
    }

    void withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            std::println("💸 Withdrew ${}", amount);
            std::println("   New balance: ${}", balance);
        } else if (amount > balance) {
            std::println("❌ Insufficient funds! Balance: ${}", balance);
        } else {
            std::println("❌ Invalid withdrawal amount");
        }
    }

    void displayBalance() {
        std::println("💵 Account Balance: ${}", balance);
    }

    void displayAccountInfo() {
        std::println("╔════════════════════════════════╗");
        std::println("║      ACCOUNT INFORMATION       ║");
        std::println("╚════════════════════════════════╝");
        std::println("Account Holder: {}", accountHolder);
        std::println("Account Number: {}", accountNumber);
        std::println("Balance:        ${}", balance);
    }
};

void demonstrateBankAccount() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 5: BANK ACCOUNT CLASS     ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    BankAccount account;

    // Initialize account
    account.accountHolder = "John Smith";
    account.accountNumber = 987654321;
    account.balance = 1000.00;

    account.displayAccountInfo();

    std::println("");
    std::println("--- Transactions ---");
    account.deposit(500.00);
    std::println("");
    account.withdraw(200.00);
    std::println("");
    account.withdraw(2000.00);  // This should fail
    std::println("");
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
        std::println("⚔️  {} (Level {})", name, level);
        std::println("   ❤️  Health: {} HP", health);
        std::println("   ⚡ Attack: {}", attackPower);
    }

    void attack(GameCharacter& target) {
        std::println("⚔️  {} attacks {}!", name, target.name);
        target.takeDamage(attackPower);
    }

    void takeDamage(int damage) {
        health -= damage;
        std::println("   💥 Took {} damage!", damage);
        if (health <= 0) {
            health = 0;
            std::println("   ☠️  {} has been defeated!", name);
        } else {
            std::println("   ❤️  {} has {} HP remaining", name, health);
        }
    }

    void heal(int amount) {
        health += amount;
        std::println("💚 {} healed {} HP", name, amount);
        std::println("   Current health: {} HP", health);
    }

    void levelUp() {
        level++;
        health += 20;
        attackPower += 5;
        std::println("⭐ {} leveled up to Level {}!", name, level);
        std::println("   Health: +{} (now {})", 20, health);
        std::println("   Attack: +{} (now {})", 5, attackPower);
    }
};

void demonstrateGameCharacter() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 6: GAME CHARACTER         ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

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

    std::println("--- Initial Stats ---");
    hero.displayStats();
    std::println("");
    monster.displayStats();

    std::println("");
    std::println("--- Battle! ---");
    hero.attack(monster);
    std::println("");
    monster.attack(hero);
    std::println("");
    hero.attack(monster);
    std::println("");
    hero.attack(monster);

    std::println("");
    std::println("--- Hero Levels Up! ---");
    hero.levelUp();

    std::println("");
    std::println("--- Hero Heals ---");
    hero.heal(30);
}

// ============================================
// MAIN DEMONSTRATION
// ============================================

int main() {
    std::println("========================================");
    std::println("BASIC CLASS DEMONSTRATION");
    std::println("========================================");

    demonstrateSimpleClass();
    demonstrateStudentClass();
    demonstrateMultipleObjects();
    demonstrateCalculator();
    demonstrateBankAccount();
    demonstrateGameCharacter();

    return 0;
}
