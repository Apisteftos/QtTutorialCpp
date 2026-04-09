#include <iostream>
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
        cout << "🏗️  Rectangle created: " << width << "×" << height << "\n";
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
        cout << "📐 Rectangle: " << width << "×" << height << "\n";
        cout << "   Area: " << area() << "\n";
        cout << "   Perimeter: " << perimeter() << "\n";
    }

    // Member function to check if square
    bool isSquare() {
        return width == height;
    }
};

void demonstrateBasicMemberFunctions() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: BASIC MEMBER FUNCTIONS ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Rectangle rect(5.0, 3.0);

    cout << "\n--- Calling member functions ---\n";
    cout << "Area: " << rect.area() << "\n";
    cout << "Perimeter: " << rect.perimeter() << "\n";
    cout << "Is square? " << (rect.isSquare() ? "Yes" : "No") << "\n";

    cout << "\n--- Display method ---\n";
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
        cout << "💳 Account created for " << holder << "\n";
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
        cout << "Account: " << getAccountHolder() << "\n";
        cout << "Balance: $" << getBalance() << "\n";
    }

    // NON-CONST member function - modifies object
    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "💰 Deposited $" << amount << "\n";
        }
    }

    // NON-CONST member function - modifies object
    void withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            cout << "💸 Withdrew $" << amount << "\n";
        }
    }
};

void demonstrateConstMemberFunctions() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: CONST MEMBER FUNCTIONS ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    BankAccount account("Alice", 1000.00);

    cout << "\n--- Calling const member functions ---\n";
    cout << "Holder: " << account.getAccountHolder() << "\n";
    cout << "Balance: $" << account.getBalance() << "\n";

    cout << "\n--- Calling non-const member functions ---\n";
    account.deposit(500.00);
    account.withdraw(200.00);

    cout << "\n--- Const object can only call const functions ---\n";
    const BankAccount constAccount("Bob", 2000.00);
    cout << "Balance: $" << constAccount.getBalance() << "\n";
    // constAccount.deposit(100);  // ❌ Error! Can't call non-const on const object

    cout << "\n💡 Const member functions:\n";
    cout << "   • Promise not to modify object\n";
    cout << "   • Can be called on const objects\n";
    cout << "   • Should be used for getters and read-only operations\n";
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
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: DEFINITION STYLES      ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    DefinitionStyles obj;

    cout << "Three ways to define member functions:\n";
    cout << "1. Inside class (implicit inline)\n";
    cout << "2. Outside class (ClassName::functionName)\n";
    cout << "3. Explicit inline keyword\n";

    obj.setValueInline(10);
    cout << "After inline: " << obj.getValue() << "\n";

    obj.setValueOutside(20);
    cout << "After outside: " << obj.getValue() << "\n";

    obj.setValueExplicitInline(30);
    cout << "After explicit inline: " << obj.getValue() << "\n";
}

// ============================================
// EXAMPLE 4: FUNCTION OVERLOADING
// ============================================

class Calculator {
public:
    // Overloaded functions with different parameters

    // Add two integers
    int add(int a, int b) {
        cout << "  Adding two ints: " << a << " + " << b << "\n";
        return a + b;
    }

    // Add three integers
    int add(int a, int b, int c) {
        cout << "  Adding three ints: " << a << " + " << b << " + " << c << "\n";
        return a + b + c;
    }

    // Add two doubles
    double add(double a, double b) {
        cout << "  Adding two doubles: " << a << " + " << b << "\n";
        return a + b;
    }

    // Add vector of numbers
    int add(const vector<int>& numbers) {
        cout << "  Adding vector of " << numbers.size() << " numbers\n";
        int sum = 0;
        for (int num : numbers) {
            sum += num;
        }
        return sum;
    }
};

void demonstrateFunctionOverloading() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: FUNCTION OVERLOADING   ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Calculator calc;

    cout << "--- Different overloaded versions ---\n";
    cout << "Result: " << calc.add(5, 3) << "\n\n";
    cout << "Result: " << calc.add(5, 3, 2) << "\n\n";
    cout << "Result: " << calc.add(5.5, 3.2) << "\n\n";

    vector<int> nums = {1, 2, 3, 4, 5};
    cout << "Result: " << calc.add(nums) << "\n";

    cout << "\n💡 Function overloading:\n";
    cout << "   • Same name, different parameters\n";
    cout << "   • Compiler chooses based on arguments\n";
    cout << "   • Makes code more intuitive\n";
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
        cout << "  Counter #" << instanceId << " created\n";
    }

    ~Counter() {
        cout << "  Counter #" << instanceId << " destroyed\n";
    }

    // Static member function - belongs to class, not objects
    static int getCount() {
        // instanceId = 0;  // ❌ Error! Can't access non-static members
        return count;
    }

    // Static member function
    static void resetCount() {
        count = 0;
        cout << "  Counter reset to 0\n";
    }

    int getId() const {
        return instanceId;
    }
};

// Initialize static member variable
int Counter::count = 0;

void demonstrateStaticMemberFunctions() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: STATIC MEMBER FUNCTIONS║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating objects ---\n";
    cout << "Total count: " << Counter::getCount() << "\n\n";

    {
        Counter c1;
        cout << "Total count: " << Counter::getCount() << "\n\n";

        Counter c2;
        cout << "Total count: " << Counter::getCount() << "\n\n";

        Counter c3;
        cout << "Total count: " << Counter::getCount() << "\n\n";
    }

    cout << "\n--- After scope (objects destroyed) ---\n";
    cout << "Total count still: " << Counter::getCount() << "\n";

    Counter::resetCount();
    cout << "After reset: " << Counter::getCount() << "\n";

    cout << "\n💡 Static member functions:\n";
    cout << "   • Called on class, not object: Counter::getCount()\n";
    cout << "   • Can only access static members\n";
    cout << "   • No 'this' pointer\n";
    cout << "   • Used for class-level operations\n";
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

        cout << "  Created: " << this->name << " with value " << this->value << "\n";
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
        cout << "  " << name << ": " << value << "\n";
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
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: THIS POINTER           ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating objects ---\n";
    ThisPointerDemo obj1(10, "Object1");
    ThisPointerDemo obj2(20, "Object2");

    cout << "\n--- Method chaining with 'this' ---\n";
    obj1.setValue(100).setName("Modified");
    obj1.display();

    cout << "\n--- Comparing objects ---\n";
    if (obj2.isGreaterThan(obj1)) {
        cout << "Object2 is greater than Object1\n";
    } else {
        cout << "Object1 is greater than or equal to Object2\n";
    }

    cout << "\n--- Getting pointer ---\n";
    ThisPointerDemo* ptr = obj1.getPointer();
    cout << "Pointer address: " << ptr << "\n";
    cout << "Object address:  " << &obj1 << "\n";
    cout << "Same? " << (ptr == &obj1 ? "Yes" : "No") << "\n";

    cout << "\n💡 'this' pointer:\n";
    cout << "   • Points to current object\n";
    cout << "   • Used to resolve name conflicts\n";
    cout << "   • Return *this for method chaining\n";
    cout << "   • Type: ClassName* const\n";
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
            cout << "⚠️  Name cannot be empty\n";
        }
    }

    void setAge(int a) {
        if (a > 0 && a < 150) {
            age = a;
        } else {
            cout << "⚠️  Invalid age: " << a << "\n";
        }
    }

    void setGpa(double g) {
        if (g >= 0.0 && g <= 4.0) {
            gpa = g;
        } else {
            cout << "⚠️  GPA must be between 0.0 and 4.0\n";
        }
    }

    void display() const {
        cout << "👤 " << name << ", Age: " << age << ", GPA: " << gpa << "\n";
    }
};

void demonstrateAccessorsMutators() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 7: GETTERS AND SETTERS    ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Student student("Alice", 20, 3.8);

    cout << "--- Using getters ---\n";
    cout << "Name: " << student.getName() << "\n";
    cout << "Age: " << student.getAge() << "\n";
    cout << "GPA: " << student.getGpa() << "\n";

    cout << "\n--- Using setters with validation ---\n";
    student.setAge(21);
    cout << "Age updated to 21\n";

    student.setAge(-5);  // Invalid - will be rejected

    student.setGpa(3.9);
    cout << "GPA updated to 3.9\n";

    student.setGpa(5.0);  // Invalid - will be rejected

    cout << "\n--- Final state ---\n";
    student.display();

    cout << "\n💡 Getters/Setters benefits:\n";
    cout << "   • Encapsulation - hide internal representation\n";
    cout << "   • Validation - check values before setting\n";
    cout << "   • Flexibility - change implementation without changing interface\n";
    cout << "   • Control - read-only, write-only, or both\n";
}

// ============================================
// EXAMPLE 8: MEMBER FUNCTION WITH DEFAULT ARGUMENTS
// ============================================

class Printer {
public:
    void print(string message, int copies = 1, bool color = false) {
        cout << "Printing: \"" << message << "\"\n";
        cout << "  Copies: " << copies << "\n";
        cout << "  Color: " << (color ? "Yes" : "No") << "\n";
    }
};

void demonstrateDefaultArguments() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 8: DEFAULT ARGUMENTS      ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Printer printer;

    cout << "--- Different argument combinations ---\n\n";

    printer.print("Hello");
    cout << "\n";

    printer.print("Report", 3);
    cout << "\n";

    printer.print("Flyer", 100, true);

    cout << "\n💡 Default arguments:\n";
    cout << "   • Provide default values for parameters\n";
    cout << "   • Make function flexible\n";
    cout << "   • Can skip trailing arguments\n";
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
            cout << "❌ Invalid deposit amount\n";
            return false;
        }

        balance += amount;
        logTransaction("Deposit: +" + to_string(amount));
        cout << "✅ Deposited $" << amount << "\n";
        return true;
    }

    bool withdraw(double amount) {
        if (amount <= 0) {
            cout << "❌ Invalid withdrawal amount\n";
            return false;
        }

        if (amount > balance) {
            cout << "❌ Insufficient funds\n";
            return false;
        }

        balance -= amount;
        logTransaction("Withdrawal: -" + to_string(amount));
        cout << "✅ Withdrew $" << amount << "\n";
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
        cout << "╔════════════════════════════════╗\n";
        cout << "║     ACCOUNT INFORMATION        ║\n";
        cout << "╚════════════════════════════════╝\n";
        cout << "Account: " << accountNumber << "\n";
        cout << "Holder:  " << holderName << "\n";
        cout << "Balance: $" << balance << "\n";
    }

    void displayHistory() const {
        cout << "\n--- Transaction History ---\n";
        for (size_t i = 0; i < transactionHistory.size(); i++) {
            cout << i + 1 << ". " << transactionHistory[i] << "\n";
        }
    }
};

void demonstrateBankAccount() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 9: BANK ACCOUNT           ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    AdvancedBankAccount acc1("123456", "Alice", 1000.00);
    AdvancedBankAccount acc2("789012", "Bob", 500.00);

    acc1.displayInfo();
    cout << "\n";
    acc2.displayInfo();

    cout << "\n--- Transactions ---\n";
    acc1.deposit(500.00);
    acc1.withdraw(200.00);
    acc1.transfer(acc2, 300.00);

    cout << "\n--- Final Balances ---\n";
    cout << "Alice: $" << acc1.getBalance() << "\n";
    cout << "Bob:   $" << acc2.getBalance() << "\n";

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
            cout << "☠️  " << name << " has been defeated!\n";
        }
    }

public:
    GameCharacter(string n, int hp, int atk)
        : name(n), health(hp), maxHealth(hp), attackPower(atk), isAlive(true) {
        cout << "⚔️  " << name << " enters the battle!\n";
    }

    void attack(GameCharacter& target) {
        if (!isAlive) {
            cout << "☠️  " << name << " cannot attack (defeated)\n";
            return;
        }

        cout << "⚔️  " << name << " attacks " << target.getName() << "!\n";
        target.takeDamage(attackPower);
    }

    void takeDamage(int damage) {
        if (!isAlive) return;

        health -= damage;
        if (health < 0) health = 0;

        cout << "💥 " << name << " takes " << damage << " damage! ";
        cout << "(" << health << "/" << maxHealth << " HP)\n";

        checkAlive();
    }

    void heal(int amount) {
        if (!isAlive) {
            cout << "☠️  Cannot heal defeated character\n";
            return;
        }

        health += amount;
        if (health > maxHealth) health = maxHealth;

        cout << "💚 " << name << " healed " << amount << " HP! ";
        cout << "(" << health << "/" << maxHealth << " HP)\n";
    }

    // Getters
    string getName() const { return name; }
    int getHealth() const { return health; }
    bool alive() const { return isAlive; }

    void displayStats() const {
        cout << "⚔️  " << name << " - ";
        cout << "HP: " << health << "/" << maxHealth << " ";
        cout << "ATK: " << attackPower << " ";
        cout << (isAlive ? "✅" : "☠️") << "\n";
    }
};

void demonstrateGameCharacter() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 10: GAME CHARACTER        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    GameCharacter hero("Warrior", 100, 25);
    GameCharacter enemy("Dragon", 80, 20);

    cout << "\n--- Initial Stats ---\n";
    hero.displayStats();
    enemy.displayStats();

    cout << "\n--- Battle! ---\n";
    hero.attack(enemy);
    enemy.attack(hero);
    hero.attack(enemy);
    hero.attack(enemy);
    enemy.attack(hero);

    cout << "\n--- Healing ---\n";
    hero.heal(30);

    cout << "\n--- Final Stats ---\n";
    hero.displayStats();
    enemy.displayStats();
}

// ============================================
// MAIN DEMONSTRATION
// ============================================

int main() {
    cout << "========================================\n";
    cout << "MEMBER FUNCTIONS DEMONSTRATION\n";
    cout << "========================================\n";

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

    // ============================================
    // Comprehensive Summary
    // ============================================
    cout << "\n========================================\n";
    cout << "WHAT ARE MEMBER FUNCTIONS?\n";
    cout << "========================================\n";
    cout << "Member functions (methods) are:\n";
    cout << "  • Functions defined inside a class\n";
    cout << "  • Operate on object's data (member variables)\n";
    cout << "  • Can access all members (public and private)\n";
    cout << "  • Define the behavior of objects\n";
    cout << "  • Called using object: obj.function()\n";

    cout << "\n========================================\n";
    cout << "BASIC SYNTAX\n";
    cout << "========================================\n";
    cout << "class MyClass {\n";
    cout << "private:\n";
    cout << "    int data;\n";
    cout << "    \n";
    cout << "public:\n";
    cout << "    // Member function\n";
    cout << "    void setData(int value) {\n";
    cout << "        data = value;  // Access member variable\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    int getData() const {\n";
    cout << "        return data;\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Usage:\n";
    cout << "MyClass obj;\n";
    cout << "obj.setData(42);      // Call member function\n";
    cout << "int value = obj.getData();\n";

    cout << "\n========================================\n";
    cout << "TYPES OF MEMBER FUNCTIONS\n";
    cout << "========================================\n";
    cout << "1. Regular Member Functions:\n";
    cout << "   void doSomething() { }\n";
    cout << "   • Can access and modify all members\n";
    cout << "   • Called on specific object\n";
    cout << "\n";
    cout << "2. Const Member Functions:\n";
    cout << "   int getValue() const { }\n";
    cout << "   • Promise not to modify object\n";
    cout << "   • Can be called on const objects\n";
    cout << "   • Use for getters and read-only operations\n";
    cout << "\n";
    cout << "3. Static Member Functions:\n";
    cout << "   static int getCount() { }\n";
    cout << "   • Belong to class, not objects\n";
    cout << "   • Called on class: MyClass::getCount()\n";
    cout << "   • Can only access static members\n";
    cout << "\n";
    cout << "4. Inline Member Functions:\n";
    cout << "   inline int getValue() { return value; }\n";
    cout << "   • Hint to compiler for optimization\n";
    cout << "   • Functions defined in class are implicitly inline\n";

    cout << "\n========================================\n";
    cout << "CONST MEMBER FUNCTIONS\n";
    cout << "========================================\n";
    cout << "Syntax: ReturnType functionName() const\n";
    cout << "\n";
    cout << "class MyClass {\n";
    cout << "    int value;\n";
    cout << "public:\n";
    cout << "    int getValue() const {  // const function\n";
    cout << "        // value = 10;  ❌ Error! Can't modify\n";
    cout << "        return value;   ✅ Can read\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "Benefits:\n";
    cout << "  ✅ Prevents accidental modification\n";
    cout << "  ✅ Can be called on const objects\n";
    cout << "  ✅ Documents intent (read-only)\n";
    cout << "  ✅ Compiler enforces const-correctness\n";
    cout << "\n";
    cout << "Rule: Always make getters and read-only functions const!\n";

    cout << "\n========================================\n";
    cout << "STATIC MEMBER FUNCTIONS\n";
    cout << "========================================\n";
    cout << "class Counter {\n";
    cout << "    static int count;\n";
    cout << "public:\n";
    cout << "    static int getCount() {  // Static function\n";
    cout << "        return count;\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "int Counter::count = 0;  // Initialize static member\n";
    cout << "\n";
    cout << "// Usage:\n";
    cout << "int c = Counter::getCount();  // Call on class, not object\n";
    cout << "\n";
    cout << "Characteristics:\n";
    cout << "  • No 'this' pointer\n";
    cout << "  • Can only access static members\n";
    cout << "  • Called on class, not objects\n";
    cout << "  • Used for class-level operations\n";

    cout << "\n========================================\n";
    cout << "FUNCTION OVERLOADING\n";
    cout << "========================================\n";
    cout << "class Calculator {\n";
    cout << "public:\n";
    cout << "    int add(int a, int b) { }\n";
    cout << "    int add(int a, int b, int c) { }\n";
    cout << "    double add(double a, double b) { }\n";
    cout << "};\n";
    cout << "\n";
    cout << "Calculator calc;\n";
    cout << "calc.add(5, 3);        // Calls int version\n";
    cout << "calc.add(5, 3, 2);     // Calls 3-parameter version\n";
    cout << "calc.add(5.5, 3.2);    // Calls double version\n";
    cout << "\n";
    cout << "Requirements:\n";
    cout << "  • Same name\n";
    cout << "  • Different parameter lists\n";
    cout << "  • Compiler chooses based on arguments\n";

    cout << "\n========================================\n";
    cout << "THIS POINTER\n";
    cout << "========================================\n";
    cout << "'this' is a pointer to the current object\n";
    cout << "\n";
    cout << "class MyClass {\n";
    cout << "    int value;\n";
    cout << "public:\n";
    cout << "    void setValue(int value) {\n";
    cout << "        this->value = value;  // Distinguish member from parameter\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    MyClass& setValueChain(int value) {\n";
    cout << "        this->value = value;\n";
    cout << "        return *this;  // Return current object for chaining\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "Uses:\n";
    cout << "  • Resolve name conflicts (parameter vs member)\n";
    cout << "  • Return current object: return *this\n";
    cout << "  • Method chaining: obj.set(1).set(2).set(3)\n";
    cout << "  • Compare with other objects\n";
    cout << "\n";
    cout << "Type: MyClass* const this\n";

    cout << "\n========================================\n";
    cout << "GETTERS AND SETTERS\n";
    cout << "========================================\n";
    cout << "class Student {\n";
    cout << "private:\n";
    cout << "    int age;\n";
    cout << "    \n";
    cout << "public:\n";
    cout << "    // Getter (Accessor) - const\n";
    cout << "    int getAge() const {\n";
    cout << "        return age;\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    // Setter (Mutator) - with validation\n";
    cout << "    void setAge(int a) {\n";
    cout << "        if (a > 0 && a < 150) {\n";
    cout << "            age = a;\n";
    cout << "        }\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "Benefits:\n";
    cout << "  ✅ Encapsulation - hide internal representation\n";
    cout << "  ✅ Validation - check values before setting\n";
    cout << "  ✅ Flexibility - change implementation later\n";
    cout << "  ✅ Control - read-only, write-only, or both\n";
    cout << "  ✅ Debugging - add logging/breakpoints\n";

    cout << "\n========================================\n";
    cout << "MEMBER FUNCTION DEFINITION STYLES\n";
    cout << "========================================\n";
    cout << "Style 1: Inside class (inline)\n";
    cout << "  class MyClass {\n";
    cout << "      void func() { }  // Defined inside\n";
    cout << "  };\n";
    cout << "\n";
    cout << "Style 2: Outside class\n";
    cout << "  class MyClass {\n";
    cout << "      void func();  // Declaration\n";
    cout << "  };\n";
    cout << "  \n";
    cout << "  void MyClass::func() {  // Definition outside\n";
    cout << "      // ...\n";
    cout << "  }\n";
    cout << "\n";
    cout << "When to use each:\n";
    cout << "  Inside:  Short, simple functions\n";
    cout << "  Outside: Long functions, cleaner header\n";

    cout << "\n========================================\n";
    cout << "DEFAULT ARGUMENTS\n";
    cout << "========================================\n";
    cout << "class Printer {\n";
    cout << "public:\n";
    cout << "    void print(string msg, int copies = 1, bool color = false) {\n";
    cout << "        // ...\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "Printer p;\n";
    cout << "p.print(\"Hello\");              // copies=1, color=false\n";
    cout << "p.print(\"Report\", 3);          // color=false\n";
    cout << "p.print(\"Flyer\", 100, true);   // All specified\n";
    cout << "\n";
    cout << "Rules:\n";
    cout << "  • Default arguments at end\n";
    cout << "  • Can skip trailing arguments\n";
    cout << "  • Specified in declaration, not definition\n";

    cout << "\n========================================\n";
    cout << "BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "✅ DO:\n";
    cout << "  • Make getters const\n";
    cout << "  • Validate input in setters\n";
    cout << "  • Use meaningful function names\n";
    cout << "  • Keep functions short and focused\n";
    cout << "  • Use const whenever possible\n";
    cout << "  • Document complex functions\n";
    cout << "  • Return references for method chaining\n";
    cout << "\n";
    cout << "❌ DON'T:\n";
    cout << "  • Forget const on read-only functions\n";
    cout << "  • Make functions too long (>50 lines)\n";
    cout << "  • Access members directly from outside\n";
    cout << "  • Return references to local variables\n";
    cout << "  • Modify object in const functions\n";

    cout << "\n========================================\n";
    cout << "COMMON PATTERNS\n";
    cout << "========================================\n";
    cout << "1. Getter pattern:\n";
    cout << "   Type getMember() const { return member; }\n";
    cout << "\n";
    cout << "2. Setter pattern:\n";
    cout << "   void setMember(Type value) {\n";
    cout << "       if (valid(value)) member = value;\n";
    cout << "   }\n";
    cout << "\n";
    cout << "3. Method chaining:\n";
    cout << "   MyClass& doSomething() {\n";
    cout << "       // ...\n";
    cout << "       return *this;\n";
    cout << "   }\n";
    cout << "\n";
    cout << "4. Query methods (const):\n";
    cout << "   bool isEmpty() const { return size == 0; }\n";
    cout << "\n";
    cout << "5. Command methods:\n";
    cout << "   void clear() { size = 0; }\n";

    cout << "\n========================================\n";
    cout << "ACCESS CONTROL WITH FUNCTIONS\n";
    cout << "========================================\n";
    cout << "class BankAccount {\n";
    cout << "private:\n";
    cout << "    double balance;  // Private data\n";
    cout << "    \n";
    cout << "public:\n";
    cout << "    // Public interface - controlled access\n";
    cout << "    double getBalance() const { return balance; }\n";
    cout << "    \n";
    cout << "    void deposit(double amount) {\n";
    cout << "        if (amount > 0) balance += amount;\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Can't do: account.balance = -1000;  ❌\n";
    cout << "// Must use: account.deposit(1000);    ✅\n";
    cout << "\n";
    cout << "Benefits:\n";
    cout << "  • Validation before modification\n";
    cout << "  • Encapsulation of internal state\n";
    cout << "  • Can change implementation later\n";
    cout << "  • Clear interface for users\n";

    cout << "\n========================================\n";
    cout << "QUICK REFERENCE\n";
    cout << "========================================\n";
    cout << "// Regular function\n";
    cout << "void doSomething() { }\n";
    cout << "\n";
    cout << "// Const function\n";
    cout << "int getValue() const { }\n";
    cout << "\n";
    cout << "// Static function\n";
    cout << "static int getCount() { }\n";
    cout << "\n";
    cout << "// Function with default arguments\n";
    cout << "void func(int x = 0, int y = 0) { }\n";
    cout << "\n";
    cout << "// Method chaining\n";
    cout << "MyClass& setX(int x) { this->x = x; return *this; }\n";
    cout << "\n";
    cout << "// Overloaded functions\n";
    cout << "void print(int x) { }\n";
    cout << "void print(double x) { }\n";
    cout << "void print(string x) { }\n";

    cout << "\n========================================\n";
    cout << "KEY TAKEAWAYS\n";
    cout << "========================================\n";
    cout << "1. Member functions define object behavior\n";
    cout << "2. Always make getters const\n";
    cout << "3. Validate input in setters\n";
    cout << "4. Use 'this' to resolve ambiguity and chain methods\n";
    cout << "5. Static functions belong to class, not objects\n";
    cout << "6. Overloading provides multiple versions of same function\n";
    cout << "7. Getters/setters provide controlled access\n";
    cout << "8. Keep functions short and focused\n";

    return 0;
}
