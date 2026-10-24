#include <print>
#include <string>
#include <cmath>
#include <vector>
using namespace std;

// ============================================
// WHAT ARE MEMBER FUNCTIONS?
// ============================================

/*
 * MEMBER FUNCTIONS (also called METHODS) are:
 * - Functions defined inside a class
 * - Operate on object's data (member variables)
 * - Can access private and public members
 * - Define the behavior of objects
 *
 * Types:
 * - Regular member functions
 * - Const member functions
 * - Static member functions
 * - Inline member functions
 */

// ============================================
// EXAMPLE 1: BASIC MEMBER FUNCTIONS
// ============================================

class Rectangle {
private:
    double width;
    double height;

public:
    // Constructor
    Rectangle(double w, double h) : width(w), height(h) {
        std::println("🏗️  Rectangle created: {}×{}", width, height);
    }

    // Member function to calculate area
    double area() {
        return width * height;
    }

    // Member function to calculate perimeter
    double perimeter() {
        return 2 * (width + height);
    }

    // Member function to display info
    void display() {
        std::println("📐 Rectangle: {}×{}", width, height);
        std::println("   Area: {}", area());
        std::println("   Perimeter: {}", perimeter());
    }

    // Member function to check if square
    bool isSquare() {
        return width == height;
    }
};

void demonstrateBasicMemberFunctions() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: BASIC MEMBER FUNCTIONS ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Rectangle rect(5.0, 3.0);

    std::println("");
    std::println("--- Calling member functions ---");
    std::println("Area: {}", rect.area());
    std::println("Perimeter: {}", rect.perimeter());
    std::println("Is square? {}", (rect.isSquare() ? "Yes" : "No"));

    std::println("");
    std::println("--- Display method ---");
    rect.display();
}

// ============================================
// EXAMPLE 2: CONST MEMBER FUNCTIONS
// ============================================

class BankAccount {
private:
    string accountHolder;
    double balance;

public:
    BankAccount(string holder, double initialBalance)
        : accountHolder(holder), balance(initialBalance) {
        std::println("💳 Account created for {}", holder);
    }

    // CONST member function - doesn't modify object
    double getBalance() const {
        // balance = 0;  // ❌ Error! Can't modify in const function
        return balance;
    }

    // CONST member function - read-only
    string getAccountHolder() const {
        return accountHolder;
    }

    // CONST member function - can call other const functions
    void displayInfo() const {
        std::println("Account: {}", getAccountHolder());
        std::println("Balance: ${}", getBalance());
    }

    // NON-CONST member function - modifies object
    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            std::println("💰 Deposited ${}", amount);
        }
    }

    // NON-CONST member function - modifies object
    void withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            std::println("💸 Withdrew ${}", amount);
        }
    }
};

void demonstrateConstMemberFunctions() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: CONST MEMBER FUNCTIONS ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    BankAccount account("Alice", 1000.00);

    std::println("");
    std::println("--- Calling const member functions ---");
    std::println("Holder: {}", account.getAccountHolder());
    std::println("Balance: ${}", account.getBalance());

    std::println("");
    std::println("--- Calling non-const member functions ---");
    account.deposit(500.00);
    account.withdraw(200.00);

    std::println("");
    std::println("--- Const object can only call const functions ---");
    const BankAccount constAccount("Bob", 2000.00);
    std::println("Balance: ${}", constAccount.getBalance());
    // constAccount.deposit(100);  // ❌ Error! Can't call non-const on const object
}

// ============================================
// EXAMPLE 3: MEMBER FUNCTION DEFINITION STYLES
// ============================================

class DefinitionStyles {
private:
    int value;

public:
    // Style 1: Inline definition (inside class)
    void setValueInline(int v) {
        value = v;
    }

    // Style 2: Declaration only (definition outside)
    void setValueOutside(int v);

    // Style 3: Inline keyword (hint to compiler)
    inline void setValueExplicitInline(int v) {
        value = v;
    }

    int getValue() const { return value; }
};

// Definition outside class (Style 2)
void DefinitionStyles::setValueOutside(int v) {
    value = v;
}

void demonstrateDefinitionStyles() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: DEFINITION STYLES      ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    DefinitionStyles obj;


    obj.setValueInline(10);
    std::println("After inline: {}", obj.getValue());

    obj.setValueOutside(20);
    std::println("After outside: {}", obj.getValue());

    obj.setValueExplicitInline(30);
    std::println("After explicit inline: {}", obj.getValue());
}

// ============================================
// EXAMPLE 4: FUNCTION OVERLOADING
// ============================================

class Calculator {
public:
    // Overloaded functions with different parameters

    // Add two integers
    int add(int a, int b) {
        std::println("  Adding two ints: {} + {}", a, b);
        return a + b;
    }

    // Add three integers
    int add(int a, int b, int c) {
        std::println("  Adding three ints: {} + {} + {}", a, b, c);
        return a + b + c;
    }

    // Add two doubles
    double add(double a, double b) {
        std::println("  Adding two doubles: {} + {}", a, b);
        return a + b;
    }

    // Add vector of numbers
    int add(const vector<int>& numbers) {
        std::println("  Adding vector of {} numbers", numbers.size());
        int sum = 0;
        for (int num : numbers) {
            sum += num;
        }
        return sum;
    }
};

void demonstrateFunctionOverloading() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: FUNCTION OVERLOADING   ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Calculator calc;

    std::println("--- Different overloaded versions ---");
    std::println("Result: {}", calc.add(5, 3));
    std::println("");
    std::println("Result: {}", calc.add(5, 3, 2));
    std::println("");
    std::println("Result: {}", calc.add(5.5, 3.2));
    std::println("");

    vector<int> nums = {1, 2, 3, 4, 5};
    std::println("Result: {}", calc.add(nums));
}

// ============================================
// EXAMPLE 5: STATIC MEMBER FUNCTIONS
// ============================================

class Counter {
private:
    static int count;  // Static member variable (shared by all objects)
    int instanceId;

public:
    Counter() {
        count++;
        instanceId = count;
        std::println("  Counter #{} created", instanceId);
    }

    ~Counter() {
        std::println("  Counter #{} destroyed", instanceId);
    }

    // Static member function - belongs to class, not objects
    static int getCount() {
        // instanceId = 0;  // ❌ Error! Can't access non-static members
        return count;
    }

    // Static member function
    static void resetCount() {
        count = 0;
        std::println("  Counter reset to 0");
    }

    int getId() const {
        return instanceId;
    }
};

// Initialize static member variable
int Counter::count = 0;

void demonstrateStaticMemberFunctions() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 5: STATIC MEMBER FUNCTIONS║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating objects ---");
    std::println("Total count: {}", Counter::getCount());
    std::println("");

    {
        Counter c1;
        std::println("Total count: {}", Counter::getCount());
        std::println("");

        Counter c2;
        std::println("Total count: {}", Counter::getCount());
        std::println("");

        Counter c3;
        std::println("Total count: {}", Counter::getCount());
        std::println("");
    }

    std::println("");
    std::println("--- After scope (objects destroyed) ---");
    std::println("Total count still: {}", Counter::getCount());

    Counter::resetCount();
    std::println("After reset: {}", Counter::getCount());
}

// ============================================
// EXAMPLE 6: THIS POINTER
// ============================================

class ThisPointerDemo {
private:
    int value;
    string name;

public:
    ThisPointerDemo(int value, string name) {
        // Use 'this' to distinguish parameter from member
        this->value = value;  // this->value is the member
        this->name = name;    // name (parameter) vs this->name (member)

        std::println("  Created: {} with value {}", this->name, this->value);
    }

    // Return *this for method chaining
    ThisPointerDemo& setValue(int value) {
        this->value = value;
        return *this;  // Return reference to current object
    }

    ThisPointerDemo& setName(string name) {
        this->name = name;
        return *this;
    }

    void display() const {
        std::println("  {}: {}", name, value);
    }

    // Compare with another object
    bool isGreaterThan(const ThisPointerDemo& other) const {
        return this->value > other.value;
    }

    // Get pointer to current object
    ThisPointerDemo* getPointer() {
        return this;
    }
};

void demonstrateThisPointer() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 6: THIS POINTER           ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating objects ---");
    ThisPointerDemo obj1(10, "Object1");
    ThisPointerDemo obj2(20, "Object2");

    std::println("");
    std::println("--- Method chaining with 'this' ---");
    obj1.setValue(100).setName("Modified");
    obj1.display();

    std::println("");
    std::println("--- Comparing objects ---");
    if (obj2.isGreaterThan(obj1)) {
        std::println("Object2 is greater than Object1");
    } else {
        std::println("Object1 is greater than or equal to Object2");
    }

    std::println("");
    std::println("--- Getting pointer ---");
    ThisPointerDemo* ptr = obj1.getPointer();
    std::println("Pointer address: {}", static_cast<const void*>(ptr));
    std::println("Object address:  {}", static_cast<const void*>(&obj1));
    std::println("Same? {}", (ptr == &obj1 ? "Yes" : "No"));
}

// ============================================
// EXAMPLE 7: ACCESSORS AND MUTATORS (GETTERS/SETTERS)
// ============================================

class Student {
private:
    string name;
    int age;
    double gpa;

public:
    Student(string n, int a, double g) : name(n), age(a), gpa(g) { }

    // ACCESSORS (Getters) - read-only access
    string getName() const {
        return name;
    }

    int getAge() const {
        return age;
    }

    double getGpa() const {
        return gpa;
    }

    // MUTATORS (Setters) - modify with validation
    void setName(string n) {
        if (!n.empty()) {
            name = n;
        } else {
            std::println("⚠️  Name cannot be empty");
        }
    }

    void setAge(int a) {
        if (a > 0 && a < 150) {
            age = a;
        } else {
            std::println("⚠️  Invalid age: {}", a);
        }
    }

    void setGpa(double g) {
        if (g >= 0.0 && g <= 4.0) {
            gpa = g;
        } else {
            std::println("⚠️  GPA must be between 0.0 and 4.0");
        }
    }

    void display() const {
        std::println("👤 {}, Age: {}, GPA: {}", name, age, gpa);
    }
};

void demonstrateAccessorsMutators() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 7: GETTERS AND SETTERS    ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Student student("Alice", 20, 3.8);

    std::println("--- Using getters ---");
    std::println("Name: {}", student.getName());
    std::println("Age: {}", student.getAge());
    std::println("GPA: {}", student.getGpa());

    std::println("");
    std::println("--- Using setters with validation ---");
    student.setAge(21);
    std::println("Age updated to 21");

    student.setAge(-5);  // Invalid - will be rejected

    student.setGpa(3.9);
    std::println("GPA updated to 3.9");

    student.setGpa(5.0);  // Invalid - will be rejected

    std::println("");
    std::println("--- Final state ---");
    student.display();
}

// ============================================
// EXAMPLE 8: MEMBER FUNCTION WITH DEFAULT ARGUMENTS
// ============================================

class Printer {
public:
    void print(string message, int copies = 1, bool color = false) {
        std::println("Printing: \"{}\"", message);
        std::println("  Copies: {}", copies);
        std::println("  Color: {}", (color ? "Yes" : "No"));
    }
};

void demonstrateDefaultArguments() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 8: DEFAULT ARGUMENTS      ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Printer printer;

    std::println("--- Different argument combinations ---");
    std::println("");

    printer.print("Hello");
    std::println("");

    printer.print("Report", 3);
    std::println("");

    printer.print("Flyer", 100, true);
}

// ============================================
// EXAMPLE 9: REAL-WORLD - BANK ACCOUNT
// ============================================

class AdvancedBankAccount {
private:
    string accountNumber;
    string holderName;
    double balance;
    vector<string> transactionHistory;

    // Private helper function
    void logTransaction(const string& transaction) {
        transactionHistory.push_back(transaction);
    }

public:
    AdvancedBankAccount(string number, string name, double initial)
        : accountNumber(number), holderName(name), balance(initial) {
        logTransaction("Account opened with $" + to_string(initial));
    }

    // Public interface
    bool deposit(double amount) {
        if (amount <= 0) {
            std::println("❌ Invalid deposit amount");
            return false;
        }

        balance += amount;
        logTransaction("Deposit: +" + to_string(amount));
        std::println("✅ Deposited ${}", amount);
        return true;
    }

    bool withdraw(double amount) {
        if (amount <= 0) {
            std::println("❌ Invalid withdrawal amount");
            return false;
        }

        if (amount > balance) {
            std::println("❌ Insufficient funds");
            return false;
        }

        balance -= amount;
        logTransaction("Withdrawal: -" + to_string(amount));
        std::println("✅ Withdrew ${}", amount);
        return true;
    }

    bool transfer(AdvancedBankAccount& recipient, double amount) {
        if (withdraw(amount)) {
            recipient.deposit(amount);
            logTransaction("Transfer to " + recipient.accountNumber + ": -" + to_string(amount));
            return true;
        }
        return false;
    }

    double getBalance() const {
        return balance;
    }

    void displayInfo() const {
        std::println("╔════════════════════════════════╗");
        std::println("║     ACCOUNT INFORMATION        ║");
        std::println("╚════════════════════════════════╝");
        std::println("Account: {}", accountNumber);
        std::println("Holder:  {}", holderName);
        std::println("Balance: ${}", balance);
    }

    void displayHistory() const {
        std::println("");
        std::println("--- Transaction History ---");
        for (size_t i = 0; i < transactionHistory.size(); i++) {
            std::println("{}. {}", i + 1, transactionHistory[i]);
        }
    }
};

void demonstrateBankAccount() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 9: BANK ACCOUNT           ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    AdvancedBankAccount acc1("123456", "Alice", 1000.00);
    AdvancedBankAccount acc2("789012", "Bob", 500.00);

    acc1.displayInfo();
    std::println("");
    acc2.displayInfo();

    std::println("");
    std::println("--- Transactions ---");
    acc1.deposit(500.00);
    acc1.withdraw(200.00);
    acc1.transfer(acc2, 300.00);

    std::println("");
    std::println("--- Final Balances ---");
    std::println("Alice: ${}", acc1.getBalance());
    std::println("Bob:   ${}", acc2.getBalance());

    acc1.displayHistory();
}

// ============================================
// EXAMPLE 10: REAL-WORLD - GAME CHARACTER
// ============================================

class GameCharacter {
private:
    string name;
    int health;
    int maxHealth;
    int attackPower;
    bool isAlive;

    // Private helper
    void checkAlive() {
        isAlive = (health > 0);
        if (!isAlive) {
            std::println("☠️  {} has been defeated!", name);
        }
    }

public:
    GameCharacter(string n, int hp, int atk)
        : name(n), health(hp), maxHealth(hp), attackPower(atk), isAlive(true) {
        std::println("⚔️  {} enters the battle!", name);
    }

    void attack(GameCharacter& target) {
        if (!isAlive) {
            std::println("☠️  {} cannot attack (defeated)", name);
            return;
        }

        std::println("⚔️  {} attacks {}!", name, target.getName());
        target.takeDamage(attackPower);
    }

    void takeDamage(int damage) {
        if (!isAlive) return;

        health -= damage;
        if (health < 0) health = 0;

        std::print("💥 {} takes {} damage! ", name, damage);
        std::println("({}/{} HP)", health, maxHealth);

        checkAlive();
    }

    void heal(int amount) {
        if (!isAlive) {
            std::println("☠️  Cannot heal defeated character");
            return;
        }

        health += amount;
        if (health > maxHealth) health = maxHealth;

        std::print("💚 {} healed {} HP! ", name, amount);
        std::println("({}/{} HP)", health, maxHealth);
    }

    // Getters
    string getName() const { return name; }
    int getHealth() const { return health; }
    bool alive() const { return isAlive; }

    void displayStats() const {
        std::print("⚔️  {} - ", name);
        std::print("HP: {}/{} ", health, maxHealth);
        std::print("ATK: {} ", attackPower);
        std::println("{}", (isAlive ? "✅" : "☠️"));
    }
};

void demonstrateGameCharacter() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 10: GAME CHARACTER        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    GameCharacter hero("Warrior", 100, 25);
    GameCharacter enemy("Dragon", 80, 20);

    std::println("");
    std::println("--- Initial Stats ---");
    hero.displayStats();
    enemy.displayStats();

    std::println("");
    std::println("--- Battle! ---");
    hero.attack(enemy);
    enemy.attack(hero);
    hero.attack(enemy);
    hero.attack(enemy);
    enemy.attack(hero);

    std::println("");
    std::println("--- Healing ---");
    hero.heal(30);

    std::println("");
    std::println("--- Final Stats ---");
    hero.displayStats();
    enemy.displayStats();
}

// ============================================
// MAIN DEMONSTRATION
// ============================================

int main() {
    std::println("========================================");
    std::println("MEMBER FUNCTIONS DEMONSTRATION");
    std::println("========================================");

    demonstrateBasicMemberFunctions();
    demonstrateConstMemberFunctions();
    demonstrateDefinitionStyles();
    demonstrateFunctionOverloading();
    demonstrateStaticMemberFunctions();
    demonstrateThisPointer();
    demonstrateAccessorsMutators();
    demonstrateDefaultArguments();
    demonstrateBankAccount();
    demonstrateGameCharacter();

    return 0;
}
