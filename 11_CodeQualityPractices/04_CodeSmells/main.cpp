#include <vector>
#include <string>
#include <memory>
#include <map>
#include <algorithm>
#include <cmath>
#include <print>

// ============================================
// CODE SMELLS PRINCIPLE
// ============================================

/*
CODE SMELLS:
    "Code smells are indicators of potential problems in code.
     They are not bugs but signs that code may need refactoring."
    — Martin Fowler

WHAT THEY ARE:
    • Surface indicators of deeper problems
    • Warning signs, not definitive problems
    • Symptoms of poor design decisions
    • Hints that code needs refactoring
    • Often violate best practices

WHY THEY MATTER:
    ✅ Early warning system for problems
    ✅ Guide refactoring efforts
    ✅ Improve code maintainability
    ✅ Reduce technical debt
    ✅ Prevent bugs before they happen

KEY CHARACTERISTICS:
    • Easy to spot with practice
    • Often indicate design flaws
    • Can accumulate over time
    • Should be addressed proactively
    • Not always straightforward to fix

COMMON CODE SMELLS:
    1. Long Method (God Method)
    2. Large Class (God Class)
    3. Duplicate Code
    4. Long Parameter List
    5. Feature Envy
    6. Data Clumps
    7. Primitive Obsession
    8. Switch Statements
    9. Comments (excessive)
    10. Dead Code

DETECTION SIGNS:
    🚨 Code is hard to understand
    🚨 Changes require multiple edits
    🚨 Similar code in multiple places
    🚨 Functions with many parameters
    🚨 Classes doing too much
    🚨 Excessive comments explaining code
*/

// ============================================
// EXAMPLE 1: LONG METHOD (GOD METHOD)
// ============================================

namespace bad_example1 {
// ❌ BAD: Long method doing too much

void processUserRegistration(const std::string& username,
                             const std::string& email,
                             const std::string& password) {
    // 🚨 This method does EVERYTHING!

    // Validate username
    if (username.length() < 3 || username.length() > 20) {
        std::println("   ❌ Invalid username length");
        return;
    }
    for (char c : username) {
        if (!isalnum(c) && c != '_') {
            std::println("   ❌ Invalid username characters");
            return;
        }
    }

    // Validate email
    if (email.find('@') == std::string::npos) {
        std::println("   ❌ Invalid email format");
        return;
    }
    size_t atPos = email.find('@');
    if (atPos == 0 || atPos == email.length() - 1) {
        std::println("   ❌ Invalid email format");
        return;
    }

    // Validate password
    if (password.length() < 8) {
        std::println("   ❌ Password too short");
        return;
    }
    bool hasUpper = false, hasLower = false, hasDigit = false;
    for (char c : password) {
        if (isupper(c)) hasUpper = true;
        if (islower(c)) hasLower = true;
        if (isdigit(c)) hasDigit = true;
    }
    if (!hasUpper || !hasLower || !hasDigit) {
        std::println("   ❌ Password must contain upper, lower, and digit");
        return;
    }

    // Hash password (simplified)
    std::string hashedPassword = "hashed_" + password;

    // Check if user exists
    // Simulate database check
    bool userExists = false;
    if (userExists) {
        std::println("   ❌ User already exists");
        return;
    }

    // Save to database
    std::println("   💾 Saving user to database...");

    // Send welcome email
    std::println("   📧 Sending welcome email to {}", email);

}
}

namespace good_example1 {
// ✅ GOOD: Small, focused methods

struct ValidationResult {
    bool valid;
    std::string error;
};

ValidationResult validateUsername(const std::string& username) {
    if (username.length() < 3 || username.length() > 20) {
        return {false, "Username must be 3-20 characters"};
    }

    for (char c : username) {
        if (!isalnum(c) && c != '_') {
            return {false, "Username can only contain letters, numbers, and underscore"};
        }
    }

    return {true, ""};
}

ValidationResult validateEmail(const std::string& email) {
    size_t atPos = email.find('@');

    if (atPos == std::string::npos || atPos == 0 || atPos == email.length() - 1) {
        return {false, "Invalid email format"};
    }

    return {true, ""};
}

ValidationResult validatePassword(const std::string& password) {
    if (password.length() < 8) {
        return {false, "Password must be at least 8 characters"};
    }

    bool hasUpper = false, hasLower = false, hasDigit = false;
    for (char c : password) {
        if (isupper(c)) hasUpper = true;
        if (islower(c)) hasLower = true;
        if (isdigit(c)) hasDigit = true;
    }

    if (!hasUpper || !hasLower || !hasDigit) {
        return {false, "Password must contain uppercase, lowercase, and digit"};
    }

    return {true, ""};
}

std::string hashPassword(const std::string& password) {
    return "hashed_" + password;  // Simplified
}

bool userExists(const std::string& username) {
    return false;  // Simulate database check
}

void saveUser(const std::string& username, const std::string& email, const std::string& hashedPassword) {
    std::println("   💾 Saving user to database...");
}

void sendWelcomeEmail(const std::string& email) {
    std::println("   📧 Sending welcome email to {}", email);
}

void logRegistration(const std::string& username) {
    std::println("   📝 Logging registration event for {}", username);
}

void updateAnalytics() {
    std::println("   📊 Updating user statistics");
}

void processUserRegistration(const std::string& username,
                             const std::string& email,
                             const std::string& password) {
    // Validate inputs
    auto usernameResult = validateUsername(username);
    if (!usernameResult.valid) {
        std::println("   ❌ {}", usernameResult.error);
        return;
    }

    auto emailResult = validateEmail(email);
    if (!emailResult.valid) {
        std::println("   ❌ {}", emailResult.error);
        return;
    }

    auto passwordResult = validatePassword(password);
    if (!passwordResult.valid) {
        std::println("   ❌ {}", passwordResult.error);
        return;
    }

    // Check user existence
    if (userExists(username)) {
        std::println("   ❌ User already exists");
        return;
    }

    // Process registration
    std::string hashedPassword = hashPassword(password);
    saveUser(username, email, hashedPassword);
    sendWelcomeEmail(email);
    logRegistration(username);
    updateAnalytics();

    std::println("   ✅ Registration successful!");
}
}

void example1_long_method() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 1: LONG METHOD (GOD METHOD)           ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   ");

    good_example1::processUserRegistration("john_doe", "john@example.com", "SecurePass123");

}

// ============================================
// EXAMPLE 2: DUPLICATE CODE
// ============================================

namespace bad_example2 {
// ❌ BAD: Code duplication everywhere

void calculateMonthlyPayment(double principal, double rate, int months) {
    double monthlyRate = rate / 12.0 / 100.0;
    double payment = principal * monthlyRate *
                     pow(1 + monthlyRate, months) /
                     (pow(1 + monthlyRate, months) - 1);
    std::println("   Monthly Payment: ${}", payment);
}

void calculateTotalPayment(double principal, double rate, int months) {
    // 🚨 Same calculation duplicated!
    double monthlyRate = rate / 12.0 / 100.0;
    double monthlyPayment = principal * monthlyRate *
                            pow(1 + monthlyRate, months) /
                            (pow(1 + monthlyRate, months) - 1);
    double total = monthlyPayment * months;
    std::println("   Total Payment: ${}", total);
}

void calculateInterest(double principal, double rate, int months) {
    // 🚨 Same calculation again!
    double monthlyRate = rate / 12.0 / 100.0;
    double monthlyPayment = principal * monthlyRate *
                            pow(1 + monthlyRate, months) /
                            (pow(1 + monthlyRate, months) - 1);
    double total = monthlyPayment * months;
    double interest = total - principal;
    std::println("   Total Interest: ${}", interest);
}
}

namespace good_example2 {
// ✅ GOOD: DRY (Don't Repeat Yourself)

double calculateMonthlyRate(double annualRate) {
    return annualRate / 12.0 / 100.0;
}

double calculateMonthlyPayment(double principal, double rate, int months) {
    double monthlyRate = calculateMonthlyRate(rate);
    return principal * monthlyRate *
           pow(1 + monthlyRate, months) /
           (pow(1 + monthlyRate, months) - 1);
}

void displayMonthlyPayment(double principal, double rate, int months) {
    double payment = calculateMonthlyPayment(principal, rate, months);
    std::println("   Monthly Payment: ${}", payment);
}

void displayTotalPayment(double principal, double rate, int months) {
    double monthlyPayment = calculateMonthlyPayment(principal, rate, months);
    double total = monthlyPayment * months;
    std::println("   Total Payment: ${}", total);
}

void displayInterest(double principal, double rate, int months) {
    double monthlyPayment = calculateMonthlyPayment(principal, rate, months);
    double total = monthlyPayment * months;
    double interest = total - principal;
    std::println("   Total Interest: ${}", interest);
}
}

void example2_duplicate_code() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 2: DUPLICATE CODE                      ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   ");

    std::println("   ");

    good_example2::displayMonthlyPayment(200000, 5.0, 360);
    good_example2::displayTotalPayment(200000, 5.0, 360);
    good_example2::displayInterest(200000, 5.0, 360);

}

// ============================================
// EXAMPLE 3: LONG PARAMETER LIST
// ============================================

namespace bad_example3 {
// ❌ BAD: Too many parameters

void createUser(const std::string& firstName,
                const std::string& lastName,
                const std::string& email,
                const std::string& phone,
                const std::string& address,
                const std::string& city,
                const std::string& country,
                const std::string& postalCode,
                int age,
                bool isActive) {
    // 🚨 10 parameters! Hard to use correctly
    std::println("   Creating user: {} {}", firstName, lastName);
}
}

namespace good_example3 {
// ✅ GOOD: Use data structures

struct Address {
    std::string street;
    std::string city;
    std::string country;
    std::string postalCode;
};

struct UserInfo {
    std::string firstName;
    std::string lastName;
    std::string email;
    std::string phone;
    Address address;
    int age;
    bool isActive;
};

void createUser(const UserInfo& user) {
    std::println("   Creating user: {} {}", user.firstName, user.lastName);
}

// Builder pattern for complex construction
class UserBuilder {
private:
    UserInfo user;

public:
    UserBuilder& setName(const std::string& first, const std::string& last) {
        user.firstName = first;
        user.lastName = last;
        return *this;
    }

    UserBuilder& setEmail(const std::string& email) {
        user.email = email;
        return *this;
    }

    UserBuilder& setAddress(const Address& addr) {
        user.address = addr;
        return *this;
    }

    UserBuilder& setAge(int age) {
        user.age = age;
        return *this;
    }

    UserInfo build() {
        return user;
    }
};
}

void example3_long_parameter_list() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 3: LONG PARAMETER LIST                 ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   ");

    good_example3::UserInfo user;
    user.firstName = "John";
    user.lastName = "Doe";
    user.email = "john@example.com";
    good_example3::createUser(user);

    std::println("");
    std::println("   Using Builder Pattern:");
    auto user2 = good_example3::UserBuilder()
                     .setName("Jane", "Smith")
                     .setEmail("jane@example.com")
                     .setAge(30)
                     .build();
    good_example3::createUser(user2);

}

// ============================================
// EXAMPLE 4: PRIMITIVE OBSESSION
// ============================================

namespace bad_example4 {
// ❌ BAD: Using primitives for domain concepts

void processPayment(double amount,    // 🚨 Just a number
                    std::string currency,   // 🚨 Just a string
                    std::string cardNumber, // 🚨 Just a string
                    std::string cvv) {      // 🚨 Just a string

    // No validation, no type safety
    if (amount <= 0) {
        std::println("   ❌ Invalid amount");
        return;
    }

    if (cardNumber.length() != 16) {
        std::println("   ❌ Invalid card number");
        return;
    }

    std::println("   💳 Processing {} {}", amount, currency);
}
}

namespace good_example4 {
// ✅ GOOD: Use value objects

class Money {
private:
    double amount;
    std::string currency;

public:
    Money(double amt, const std::string& curr) : amount(amt), currency(curr) {
        if (amt < 0) throw std::invalid_argument("Amount cannot be negative");
        if (curr.length() != 3) throw std::invalid_argument("Invalid currency code");
    }

    double getAmount() const { return amount; }
    std::string getCurrency() const { return currency; }

    Money operator+(const Money& other) const {
        if (currency != other.currency) {
            throw std::invalid_argument("Cannot add different currencies");
        }
        return Money(amount + other.amount, currency);
    }

    void display() const {
        std::print("   {} {}", amount, currency);
    }
};

class CreditCard {
private:
    std::string number;
    std::string cvv;

public:
    CreditCard(const std::string& num, const std::string& cvvCode) {
        if (num.length() != 16) {
            throw std::invalid_argument("Card number must be 16 digits");
        }
        if (cvvCode.length() != 3) {
            throw std::invalid_argument("CVV must be 3 digits");
        }
        number = num;
        cvv = cvvCode;
    }

    std::string getNumber() const { return number; }
    bool isValid() const { return number.length() == 16; }
};

void processPayment(const Money& payment, const CreditCard& card) {
    if (!card.isValid()) {
        std::println("   ❌ Invalid card");
        return;
    }

    std::print("   💳 Processing payment: ");
    payment.display();
    std::println("");
}
}

void example4_primitive_obsession() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 4: PRIMITIVE OBSESSION                 ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   ");

    std::println("   ");
    std::println("   ");

    try {
        good_example4::Money payment(100.50, "USD");
        good_example4::CreditCard card("1234567890123456", "123");
        good_example4::processPayment(payment, card);

        // Type safety prevents errors
        std::println("   ");
        std::println("   Adding money:");
        good_example4::Money payment1(50.0, "USD");
        good_example4::Money payment2(25.0, "USD");
        good_example4::Money total = payment1 + payment2;
        std::print("   Total: ");
        total.display();
        std::println("");

    } catch (const std::exception& e) {
        std::println("   ❌ {}", e.what());
    }

}

// ============================================
// EXAMPLE 5: FEATURE ENVY
// ============================================

namespace bad_example5 {
// ❌ BAD: Method is more interested in another class

class Account {
public:
    std::string accountNumber;
    double balance;
    std::string currency;
};

class TransactionProcessor {
public:
    void processTransfer(Account& from, Account& to, double amount) {
        // 🚨 Accessing 'from' data multiple times
        if (from.balance < amount) {
            std::println("   ❌ Insufficient funds in {}", from.accountNumber);
            return;
        }

        // 🚨 Manipulating 'from' directly
        from.balance -= amount;

        // 🚨 Manipulating 'to' directly
        to.balance += amount;

        std::println("   ✅ Transferred {} from {} to {}", amount, from.accountNumber, to.accountNumber);
    }
};
}

namespace good_example5 {
// ✅ GOOD: Move behavior to where the data is

class Account {
private:
    std::string accountNumber;
    double balance;
    std::string currency;

public:
    Account(const std::string& number, double initialBalance, const std::string& curr)
        : accountNumber(number), balance(initialBalance), currency(curr) {}

    bool hasEnoughBalance(double amount) const {
        return balance >= amount;
    }

    void withdraw(double amount) {
        if (!hasEnoughBalance(amount)) {
            throw std::invalid_argument("Insufficient funds");
        }
        balance -= amount;
    }

    void deposit(double amount) {
        balance += amount;
    }

    std::string getAccountNumber() const { return accountNumber; }
    double getBalance() const { return balance; }
};

class TransactionProcessor {
public:
    void processTransfer(Account& from, Account& to, double amount) {
        // Now the accounts handle their own state
        if (!from.hasEnoughBalance(amount)) {
            std::println("   ❌ Insufficient funds");
            return;
        }

        from.withdraw(amount);
        to.deposit(amount);

        std::println("   ✅ Transferred {} from {} to {}",
            amount, from.getAccountNumber(), to.getAccountNumber());
    }
};
}

void example5_feature_envy() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 5: FEATURE ENVY                        ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   ");

    good_example5::Account account1("ACC001", 1000.0, "USD");
    good_example5::Account account2("ACC002", 500.0, "USD");
    good_example5::TransactionProcessor processor;

    processor.processTransfer(account1, account2, 200.0);

    std::println("");
    std::println("   Account 1 balance: ${}", account1.getBalance());
    std::println("   Account 2 balance: ${}", account2.getBalance());

}

// ============================================
// EXAMPLE 6: DATA CLUMPS
// ============================================

namespace bad_example6 {
// ❌ BAD: Same group of parameters everywhere

void drawRectangle(int x, int y, int width, int height) {
    std::println("   Drawing at ({},{}) size {}x{}", x, y, width, height);
}

void moveRectangle(int& x, int& y, int width, int height, int dx, int dy) {
    // 🚨 Same parameters repeated
    x += dx;
    y += dy;
    std::println("   Moved to ({},{})", x, y);
}

bool isInside(int x, int y, int width, int height, int px, int py) {
    // 🚨 Again the same parameters!
    return px >= x && px <= x + width &&
           py >= y && py <= y + height;
}
}

namespace good_example6 {
// ✅ GOOD: Group related data

struct Point {
    int x, y;

    Point(int x = 0, int y = 0) : x(x), y(y) {}

    Point operator+(const Point& other) const {
        return Point(x + other.x, y + other.y);
    }

    void display() const {
        std::print("({},{})", x, y);
    }
};

struct Size {
    int width, height;

    Size(int w = 0, int h = 0) : width(w), height(h) {}

    void display() const {
        std::print("{}x{}", width, height);
    }
};

class Rectangle {
private:
    Point position;
    Size dimensions;

public:
    Rectangle(const Point& pos, const Size& size)
        : position(pos), dimensions(size) {}

    void draw() const {
        std::print("   Drawing at ");
        position.display();
        std::print(" size ");
        dimensions.display();
        std::println("");
    }

    void move(const Point& delta) {
        position = position + delta;
        std::print("   Moved to ");
        position.display();
        std::println("");
    }

    bool contains(const Point& point) const {
        return point.x >= position.x &&
               point.x <= position.x + dimensions.width &&
               point.y >= position.y &&
               point.y <= position.y + dimensions.height;
    }
};
}

void example6_data_clumps() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 6: DATA CLUMPS                         ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   ");

    std::println("   ");
    std::println("       ");

    good_example6::Rectangle rect(good_example6::Point(10, 20),
                                  good_example6::Size(100, 50));
    rect.draw();
    rect.move(good_example6::Point(5, 5));

    std::println("   Point (15, 25) inside? {}", (rect.contains(good_example6::Point(15, 25)) ? "Yes" : "No"));

}

// ============================================
// EXAMPLE 7: COMMENTS AS SMELL
// ============================================

namespace bad_example7 {
// ❌ BAD: Comments masking bad code

void process() {
    // Get the list of active users
    // Filter out invalid ones
    // Sort by registration date
    // Take the first 10
    // Send them an email

    // 🚨 Comments explaining what code does
    // This means the code is unclear!

    std::println("   Processing...");
}

// This function calculates the total price
// including taxes and shipping
// For premium customers, apply discount
// Returns the final amount
double calc(double p, bool prem) {  // 🚨 Needs comment to understand!
    double t = p * 0.1;
    double s = p > 50 ? 0 : 5.99;
    if (prem) {
        return (p + t + s) * 0.9;
    }
    return p + t + s;
}
}

namespace good_example7 {
// ✅ GOOD: Self-documenting code

const double TAX_RATE = 0.10;
const double SHIPPING_FEE = 5.99;
const double FREE_SHIPPING_THRESHOLD = 50.0;
const double PREMIUM_DISCOUNT = 0.10;

double calculateTax(double price) {
    return price * TAX_RATE;
}

double calculateShipping(double price) {
    return price >= FREE_SHIPPING_THRESHOLD ? 0.0 : SHIPPING_FEE;
}

double applyPremiumDiscount(double amount) {
    return amount * (1.0 - PREMIUM_DISCOUNT);
}

double calculateTotalPrice(double basePrice, bool isPremiumCustomer) {
    // No comment needed - code explains itself!
    double subtotal = basePrice +
                      calculateTax(basePrice) +
                      calculateShipping(basePrice);

    if (isPremiumCustomer) {
        return applyPremiumDiscount(subtotal);
    }

    return subtotal;
}

// Only comment WHY, not WHAT
// BUSINESS RULE: Premium discount applied to final amount to encourage
// customers to upgrade their membership tier
}

void example7_comments_smell() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 7: COMMENTS AS SMELL                   ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   ");
    std::println("   ");
    std::println("   ");

    double total1 = good_example7::calculateTotalPrice(100.0, false);
    double total2 = good_example7::calculateTotalPrice(100.0, true);

    std::println("   Regular customer: ${}", total1);
    std::println("   Premium customer: ${}", total2);

}

// ============================================
// EXAMPLE 8: SWITCH STATEMENTS SMELL
// ============================================

namespace bad_example8 {
// ❌ BAD: Switch statements scattered everywhere

enum EmployeeType { ENGINEER, MANAGER, SALESPERSON };

double calculateSalary(EmployeeType type, int performance) {
    // 🚨 Switch statement based on type
    switch (type) {
    case ENGINEER:
        return 80000 + performance * 1000;
    case MANAGER:
        return 100000 + performance * 1500;
    case SALESPERSON:
        return 60000 + performance * 2000;
    }
    return 0;
}

double calculateBonus(EmployeeType type, int performance) {
    // 🚨 Same switch repeated!
    switch (type) {
    case ENGINEER:
        return performance * 500;
    case MANAGER:
        return performance * 800;
    case SALESPERSON:
        return performance * 1000;
    }
    return 0;
}

// 🚨 Adding new type requires changing multiple functions!
}

namespace good_example8 {
// ✅ GOOD: Polymorphism instead of switches

class Employee {
protected:
    int performanceScore;

public:
    Employee(int performance) : performanceScore(performance) {}
    virtual ~Employee() = default;

    virtual double calculateSalary() const = 0;
    virtual double calculateBonus() const = 0;
    virtual std::string getType() const = 0;
};

class Engineer : public Employee {
public:
    Engineer(int performance) : Employee(performance) {}

    double calculateSalary() const override {
        return 80000 + performanceScore * 1000;
    }

    double calculateBonus() const override {
        return performanceScore * 500;
    }

    std::string getType() const override { return "Engineer"; }
};

class Manager : public Employee {
public:
    Manager(int performance) : Employee(performance) {}

    double calculateSalary() const override {
        return 100000 + performanceScore * 1500;
    }

    double calculateBonus() const override {
        return performanceScore * 800;
    }

    std::string getType() const override { return "Manager"; }
};

class Salesperson : public Employee {
public:
    Salesperson(int performance) : Employee(performance) {}

    double calculateSalary() const override {
        return 60000 + performanceScore * 2000;
    }

    double calculateBonus() const override {
        return performanceScore * 1000;
    }

    std::string getType() const override { return "Salesperson"; }
};

void processEmployee(const Employee& employee) {
    std::println("   {}:", employee.getType());
    std::println("   Salary: ${}", employee.calculateSalary());
    std::println("   Bonus: ${}", employee.calculateBonus());
}
}

void example8_switch_statements() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 8: SWITCH STATEMENTS SMELL             ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   ");

    std::println("   ");
    std::println("   ");

    good_example8::Engineer eng(8);
    good_example8::Manager mgr(9);
    good_example8::Salesperson sales(10);

    good_example8::processEmployee(eng);
    good_example8::processEmployee(mgr);
    good_example8::processEmployee(sales);

}

// ============================================
// EXAMPLE 9: DETECTION SUMMARY
// ============================================

void example9_detection_summary() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 9: CODE SMELL DETECTION GUIDE          ║");
    std::println("╚══════════════════════════════════════════════════╝");

}

// ============================================
// EXAMPLE 10: REFACTORING STRATEGIES
// ============================================

void example10_refactoring_strategies() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 10: REFACTORING STRATEGIES             ║");
    std::println("╚══════════════════════════════════════════════════╝");

}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    std::println("╔═══════════════════════════════════════════════════════╗");
    std::println("║                                                       ║");
    std::println("║                CODE SMELLS PRINCIPLE                  ║");
    std::println("║                                                       ║");
    std::println("║  \"Code smells are indicators of potential problems.   ║");
    std::println("║   They are not bugs but signs that code may need      ║");
    std::println("║   refactoring.\"                                      ║");
    std::println("║                       — Martin Fowler                 ║");
    std::println("║                                                       ║");
    std::println("╚═══════════════════════════════════════════════════════╝");

    example1_long_method();
    example2_duplicate_code();
    example3_long_parameter_list();
    example4_primitive_obsession();
    example5_feature_envy();
    example6_data_clumps();
    example7_comments_smell();
    example8_switch_statements();
    example9_detection_summary();
    example10_refactoring_strategies();

    return 0;
}
