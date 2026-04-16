#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <algorithm>
#include <cmath>
using namespace std;

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

void processUserRegistration(const string& username,
                             const string& email,
                             const string& password) {
    // 🚨 This method does EVERYTHING!

    // Validate username
    if (username.length() < 3 || username.length() > 20) {
        cout << "   ❌ Invalid username length" << endl;
        return;
    }
    for (char c : username) {
        if (!isalnum(c) && c != '_') {
            cout << "   ❌ Invalid username characters" << endl;
            return;
        }
    }

    // Validate email
    if (email.find('@') == string::npos) {
        cout << "   ❌ Invalid email format" << endl;
        return;
    }
    size_t atPos = email.find('@');
    if (atPos == 0 || atPos == email.length() - 1) {
        cout << "   ❌ Invalid email format" << endl;
        return;
    }

    // Validate password
    if (password.length() < 8) {
        cout << "   ❌ Password too short" << endl;
        return;
    }
    bool hasUpper = false, hasLower = false, hasDigit = false;
    for (char c : password) {
        if (isupper(c)) hasUpper = true;
        if (islower(c)) hasLower = true;
        if (isdigit(c)) hasDigit = true;
    }
    if (!hasUpper || !hasLower || !hasDigit) {
        cout << "   ❌ Password must contain upper, lower, and digit" << endl;
        return;
    }

    // Hash password (simplified)
    string hashedPassword = "hashed_" + password;

    // Check if user exists
    // Simulate database check
    bool userExists = false;
    if (userExists) {
        cout << "   ❌ User already exists" << endl;
        return;
    }

    // Save to database
    cout << "   💾 Saving user to database..." << endl;

    // Send welcome email
    cout << "   📧 Sending welcome email to " << email << endl;

    // Log registration
    cout << "   📝 Logging registration event" << endl;

    // Update analytics
    cout << "   📊 Updating user statistics" << endl;

    cout << "   ✅ Registration successful!" << endl;
}
}

namespace good_example1 {
// ✅ GOOD: Small, focused methods

struct ValidationResult {
    bool valid;
    string error;
};

ValidationResult validateUsername(const string& username) {
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

ValidationResult validateEmail(const string& email) {
    size_t atPos = email.find('@');

    if (atPos == string::npos || atPos == 0 || atPos == email.length() - 1) {
        return {false, "Invalid email format"};
    }

    return {true, ""};
}

ValidationResult validatePassword(const string& password) {
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

string hashPassword(const string& password) {
    return "hashed_" + password;  // Simplified
}

bool userExists(const string& username) {
    return false;  // Simulate database check
}

void saveUser(const string& username, const string& email, const string& hashedPassword) {
    cout << "   💾 Saving user to database..." << endl;
}

void sendWelcomeEmail(const string& email) {
    cout << "   📧 Sending welcome email to " << email << endl;
}

void logRegistration(const string& username) {
    cout << "   📝 Logging registration event for " << username << endl;
}

void updateAnalytics() {
    cout << "   📊 Updating user statistics" << endl;
}

void processUserRegistration(const string& username,
                             const string& email,
                             const string& password) {
    // Validate inputs
    auto usernameResult = validateUsername(username);
    if (!usernameResult.valid) {
        cout << "   ❌ " << usernameResult.error << endl;
        return;
    }

    auto emailResult = validateEmail(email);
    if (!emailResult.valid) {
        cout << "   ❌ " << emailResult.error << endl;
        return;
    }

    auto passwordResult = validatePassword(password);
    if (!passwordResult.valid) {
        cout << "   ❌ " << passwordResult.error << endl;
        return;
    }

    // Check user existence
    if (userExists(username)) {
        cout << "   ❌ User already exists" << endl;
        return;
    }

    // Process registration
    string hashedPassword = hashPassword(password);
    saveUser(username, email, hashedPassword);
    sendWelcomeEmail(email);
    logRegistration(username);
    updateAnalytics();

    cout << "   ✅ Registration successful!" << endl;
}
}

void example1_long_method() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 1: LONG METHOD (GOD METHOD)           ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE:\n";
    cout << "   void processUserRegistration(...) {\n";
    cout << "       // Validate username (10 lines)\n";
    cout << "       // Validate email (8 lines)\n";
    cout << "       // Validate password (15 lines)\n";
    cout << "       // Hash password\n";
    cout << "       // Check database\n";
    cout << "       // Save to database\n";
    cout << "       // Send email\n";
    cout << "       // Log event\n";
    cout << "       // Update analytics\n";
    cout << "       // 60+ lines in ONE method! 🚨\n";
    cout << "   }\n";

    cout << "\n🚨 SMELL INDICATORS:\n";
    cout << "   • Method is 50+ lines long\n";
    cout << "   • Does multiple unrelated things\n";
    cout << "   • Hard to understand at a glance\n";
    cout << "   • Difficult to test\n";
    cout << "   • Hard to reuse parts of logic\n";

    cout << "\n✅ GOOD CODE (Refactored):\n";
    cout << "   ValidationResult validateUsername(...);\n";
    cout << "   ValidationResult validateEmail(...);\n";
    cout << "   ValidationResult validatePassword(...);\n";
    cout << "   string hashPassword(...);\n";
    cout << "   void saveUser(...);\n";
    cout << "   void sendWelcomeEmail(...);\n";
    cout << "   \n";
    cout << "   void processUserRegistration(...) {\n";
    cout << "       // Now it's clear and readable!\n";
    cout << "       if (!validateUsername(...).valid) return;\n";
    cout << "       if (!validateEmail(...).valid) return;\n";
    cout << "       // ... orchestrates other methods\n";
    cout << "   }\n";

    cout << "\n✨ DEMONSTRATION:\n";
    good_example1::processUserRegistration("john_doe", "john@example.com", "SecurePass123");

    cout << "\n✨ BENEFITS:\n";
    cout << "   • Each method has single responsibility\n";
    cout << "   • Easy to test individually\n";
    cout << "   • Reusable validation logic\n";
    cout << "   • Clear and maintainable\n";
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
    cout << "   Monthly Payment: $" << payment << endl;
}

void calculateTotalPayment(double principal, double rate, int months) {
    // 🚨 Same calculation duplicated!
    double monthlyRate = rate / 12.0 / 100.0;
    double monthlyPayment = principal * monthlyRate *
                            pow(1 + monthlyRate, months) /
                            (pow(1 + monthlyRate, months) - 1);
    double total = monthlyPayment * months;
    cout << "   Total Payment: $" << total << endl;
}

void calculateInterest(double principal, double rate, int months) {
    // 🚨 Same calculation again!
    double monthlyRate = rate / 12.0 / 100.0;
    double monthlyPayment = principal * monthlyRate *
                            pow(1 + monthlyRate, months) /
                            (pow(1 + monthlyRate, months) - 1);
    double total = monthlyPayment * months;
    double interest = total - principal;
    cout << "   Total Interest: $" << interest << endl;
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
    cout << "   Monthly Payment: $" << payment << endl;
}

void displayTotalPayment(double principal, double rate, int months) {
    double monthlyPayment = calculateMonthlyPayment(principal, rate, months);
    double total = monthlyPayment * months;
    cout << "   Total Payment: $" << total << endl;
}

void displayInterest(double principal, double rate, int months) {
    double monthlyPayment = calculateMonthlyPayment(principal, rate, months);
    double total = monthlyPayment * months;
    double interest = total - principal;
    cout << "   Total Interest: $" << interest << endl;
}
}

void example2_duplicate_code() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 2: DUPLICATE CODE                      ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE:\n";
    cout << "   // Same calculation in 3 different methods!\n";
    cout << "   void calculateMonthlyPayment(...) {\n";
    cout << "       double monthlyRate = rate / 12.0 / 100.0;\n";
    cout << "       double payment = principal * monthlyRate * ...;\n";
    cout << "   }\n";
    cout << "   \n";
    cout << "   void calculateTotalPayment(...) {\n";
    cout << "       double monthlyRate = rate / 12.0 / 100.0; 🚨\n";
    cout << "       double payment = principal * monthlyRate * ...;\n";
    cout << "   }\n";

    cout << "\n🚨 SMELL INDICATORS:\n";
    cout << "   • Same code appears multiple times\n";
    cout << "   • Copy-paste programming\n";
    cout << "   • Changes require multiple edits\n";
    cout << "   • High chance of inconsistencies\n";
    cout << "   • Violates DRY principle\n";

    cout << "\n✅ GOOD CODE:\n";
    cout << "   // Extract common calculation\n";
    cout << "   double calculateMonthlyPayment(...) {\n";
    cout << "       // Core calculation logic\n";
    cout << "   }\n";
    cout << "   \n";
    cout << "   void displayTotalPayment(...) {\n";
    cout << "       double payment = calculateMonthlyPayment(...); ✅\n";
    cout << "       double total = payment * months;\n";
    cout << "   }\n";

    cout << "\n✨ DEMONSTRATION:\n";
    good_example2::displayMonthlyPayment(200000, 5.0, 360);
    good_example2::displayTotalPayment(200000, 5.0, 360);
    good_example2::displayInterest(200000, 5.0, 360);

    cout << "\n✨ BENEFITS:\n";
    cout << "   • Single source of truth\n";
    cout << "   • Change once, affect everywhere\n";
    cout << "   • Easier to maintain\n";
    cout << "   • Reduced chance of bugs\n";
}

// ============================================
// EXAMPLE 3: LONG PARAMETER LIST
// ============================================

namespace bad_example3 {
// ❌ BAD: Too many parameters

void createUser(const string& firstName,
                const string& lastName,
                const string& email,
                const string& phone,
                const string& address,
                const string& city,
                const string& country,
                const string& postalCode,
                int age,
                bool isActive) {
    // 🚨 10 parameters! Hard to use correctly
    cout << "   Creating user: " << firstName << " " << lastName << endl;
}
}

namespace good_example3 {
// ✅ GOOD: Use data structures

struct Address {
    string street;
    string city;
    string country;
    string postalCode;
};

struct UserInfo {
    string firstName;
    string lastName;
    string email;
    string phone;
    Address address;
    int age;
    bool isActive;
};

void createUser(const UserInfo& user) {
    cout << "   Creating user: " << user.firstName << " " << user.lastName << endl;
}

// Builder pattern for complex construction
class UserBuilder {
private:
    UserInfo user;

public:
    UserBuilder& setName(const string& first, const string& last) {
        user.firstName = first;
        user.lastName = last;
        return *this;
    }

    UserBuilder& setEmail(const string& email) {
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
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 3: LONG PARAMETER LIST                 ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE:\n";
    cout << "   void createUser(\n";
    cout << "       const string& firstName,    // Parameter 1\n";
    cout << "       const string& lastName,     // Parameter 2\n";
    cout << "       const string& email,        // Parameter 3\n";
    cout << "       const string& phone,        // Parameter 4\n";
    cout << "       const string& address,      // Parameter 5\n";
    cout << "       const string& city,         // Parameter 6\n";
    cout << "       const string& country,      // Parameter 7\n";
    cout << "       const string& postalCode,   // Parameter 8\n";
    cout << "       int age,                    // Parameter 9\n";
    cout << "       bool isActive               // Parameter 10 🚨\n";
    cout << "   );\n";

    cout << "\n🚨 SMELL INDICATORS:\n";
    cout << "   • More than 3-4 parameters\n";
    cout << "   • Hard to remember parameter order\n";
    cout << "   • Easy to pass wrong values\n";
    cout << "   • Changes require updating all calls\n";
    cout << "   • Poor readability at call site\n";

    cout << "\n✅ GOOD CODE:\n";
    cout << "   struct UserInfo {\n";
    cout << "       string firstName, lastName;\n";
    cout << "       string email, phone;\n";
    cout << "       Address address;\n";
    cout << "       int age;\n";
    cout << "       bool isActive;\n";
    cout << "   };\n";
    cout << "   \n";
    cout << "   void createUser(const UserInfo& user); ✅\n";

    cout << "\n✨ DEMONSTRATION:\n";

    good_example3::UserInfo user;
    user.firstName = "John";
    user.lastName = "Doe";
    user.email = "john@example.com";
    good_example3::createUser(user);

    cout << "\n   Using Builder Pattern:\n";
    auto user2 = good_example3::UserBuilder()
                     .setName("Jane", "Smith")
                     .setEmail("jane@example.com")
                     .setAge(30)
                     .build();
    good_example3::createUser(user2);

    cout << "\n✨ BENEFITS:\n";
    cout << "   • Clear and organized data\n";
    cout << "   • Easy to add new fields\n";
    cout << "   • Self-documenting\n";
    cout << "   • Easier to test\n";
}

// ============================================
// EXAMPLE 4: PRIMITIVE OBSESSION
// ============================================

namespace bad_example4 {
// ❌ BAD: Using primitives for domain concepts

void processPayment(double amount,    // 🚨 Just a number
                    string currency,   // 🚨 Just a string
                    string cardNumber, // 🚨 Just a string
                    string cvv) {      // 🚨 Just a string

    // No validation, no type safety
    if (amount <= 0) {
        cout << "   ❌ Invalid amount" << endl;
        return;
    }

    if (cardNumber.length() != 16) {
        cout << "   ❌ Invalid card number" << endl;
        return;
    }

    cout << "   💳 Processing " << amount << " " << currency << endl;
}
}

namespace good_example4 {
// ✅ GOOD: Use value objects

class Money {
private:
    double amount;
    string currency;

public:
    Money(double amt, const string& curr) : amount(amt), currency(curr) {
        if (amt < 0) throw invalid_argument("Amount cannot be negative");
        if (curr.length() != 3) throw invalid_argument("Invalid currency code");
    }

    double getAmount() const { return amount; }
    string getCurrency() const { return currency; }

    Money operator+(const Money& other) const {
        if (currency != other.currency) {
            throw invalid_argument("Cannot add different currencies");
        }
        return Money(amount + other.amount, currency);
    }

    void display() const {
        cout << "   " << amount << " " << currency;
    }
};

class CreditCard {
private:
    string number;
    string cvv;

public:
    CreditCard(const string& num, const string& cvvCode) {
        if (num.length() != 16) {
            throw invalid_argument("Card number must be 16 digits");
        }
        if (cvvCode.length() != 3) {
            throw invalid_argument("CVV must be 3 digits");
        }
        number = num;
        cvv = cvvCode;
    }

    string getNumber() const { return number; }
    bool isValid() const { return number.length() == 16; }
};

void processPayment(const Money& payment, const CreditCard& card) {
    if (!card.isValid()) {
        cout << "   ❌ Invalid card" << endl;
        return;
    }

    cout << "   💳 Processing payment: ";
    payment.display();
    cout << endl;
}
}

void example4_primitive_obsession() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 4: PRIMITIVE OBSESSION                 ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE:\n";
    cout << "   void processPayment(\n";
    cout << "       double amount,        // Just a number 🚨\n";
    cout << "       string currency,      // Just a string 🚨\n";
    cout << "       string cardNumber,    // Just a string 🚨\n";
    cout << "       string cvv            // Just a string 🚨\n";
    cout << "   );\n";
    cout << "   \n";
    cout << "   processPayment(100.50, \"USD\", \"1234\", \"123\");\n";
    cout << "   // No type safety! Can pass anything!\n";

    cout << "\n🚨 SMELL INDICATORS:\n";
    cout << "   • Using primitives for domain concepts\n";
    cout << "   • Validation scattered everywhere\n";
    cout << "   • No type safety\n";
    cout << "   • Easy to make mistakes\n";
    cout << "   • Difficult to enforce business rules\n";

    cout << "\n✅ GOOD CODE:\n";
    cout << "   class Money {\n";
    cout << "       double amount;\n";
    cout << "       string currency;\n";
    cout << "       // Validation in constructor\n";
    cout << "       // Business operations (add, subtract)\n";
    cout << "   };\n";
    cout << "   \n";
    cout << "   class CreditCard {\n";
    cout << "       string number;\n";
    cout << "       // Validation and business logic\n";
    cout << "   };\n";
    cout << "   \n";
    cout << "   void processPayment(const Money&, const CreditCard&); ✅\n";

    cout << "\n✨ DEMONSTRATION:\n";

    try {
        good_example4::Money payment(100.50, "USD");
        good_example4::CreditCard card("1234567890123456", "123");
        good_example4::processPayment(payment, card);

        // Type safety prevents errors
        cout << "   \n   Adding money:\n";
        good_example4::Money payment1(50.0, "USD");
        good_example4::Money payment2(25.0, "USD");
        good_example4::Money total = payment1 + payment2;
        cout << "   Total: ";
        total.display();
        cout << endl;

    } catch (const exception& e) {
        cout << "   ❌ " << e.what() << endl;
    }

    cout << "\n✨ BENEFITS:\n";
    cout << "   • Type safety at compile time\n";
    cout << "   • Validation in one place\n";
    cout << "   • Business logic encapsulated\n";
    cout << "   • Self-documenting code\n";
    cout << "   • Prevents many bugs\n";
}

// ============================================
// EXAMPLE 5: FEATURE ENVY
// ============================================

namespace bad_example5 {
// ❌ BAD: Method is more interested in another class

class Account {
public:
    string accountNumber;
    double balance;
    string currency;
};

class TransactionProcessor {
public:
    void processTransfer(Account& from, Account& to, double amount) {
        // 🚨 Accessing 'from' data multiple times
        if (from.balance < amount) {
            cout << "   ❌ Insufficient funds in " << from.accountNumber << endl;
            return;
        }

        // 🚨 Manipulating 'from' directly
        from.balance -= amount;

        // 🚨 Manipulating 'to' directly
        to.balance += amount;

        cout << "   ✅ Transferred " << amount << " from "
             << from.accountNumber << " to " << to.accountNumber << endl;
    }
};
}

namespace good_example5 {
// ✅ GOOD: Move behavior to where the data is

class Account {
private:
    string accountNumber;
    double balance;
    string currency;

public:
    Account(const string& number, double initialBalance, const string& curr)
        : accountNumber(number), balance(initialBalance), currency(curr) {}

    bool hasEnoughBalance(double amount) const {
        return balance >= amount;
    }

    void withdraw(double amount) {
        if (!hasEnoughBalance(amount)) {
            throw invalid_argument("Insufficient funds");
        }
        balance -= amount;
    }

    void deposit(double amount) {
        balance += amount;
    }

    string getAccountNumber() const { return accountNumber; }
    double getBalance() const { return balance; }
};

class TransactionProcessor {
public:
    void processTransfer(Account& from, Account& to, double amount) {
        // Now the accounts handle their own state
        if (!from.hasEnoughBalance(amount)) {
            cout << "   ❌ Insufficient funds" << endl;
            return;
        }

        from.withdraw(amount);
        to.deposit(amount);

        cout << "   ✅ Transferred " << amount << " from "
             << from.getAccountNumber() << " to "
             << to.getAccountNumber() << endl;
    }
};
}

void example5_feature_envy() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 5: FEATURE ENVY                        ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE:\n";
    cout << "   class TransactionProcessor {\n";
    cout << "       void processTransfer(Account& from, ...) {\n";
    cout << "           if (from.balance < amount) {...}  🚨\n";
    cout << "           from.balance -= amount;           🚨\n";
    cout << "           // Accessing 'from' data too much!\n";
    cout << "       }\n";
    cout << "   };\n";

    cout << "\n🚨 SMELL INDICATORS:\n";
    cout << "   • Method uses another object's data excessively\n";
    cout << "   • Behavior is in wrong class\n";
    cout << "   • Violates encapsulation\n";
    cout << "   • Poor cohesion\n";
    cout << "   • Changes to data class affect this class\n";

    cout << "\n✅ GOOD CODE:\n";
    cout << "   class Account {\n";
    cout << "       bool hasEnoughBalance(double amount);\n";
    cout << "       void withdraw(double amount);\n";
    cout << "       void deposit(double amount);\n";
    cout << "       // Behavior is with the data! ✅\n";
    cout << "   };\n";
    cout << "   \n";
    cout << "   class TransactionProcessor {\n";
    cout << "       void processTransfer(Account& from, ...) {\n";
    cout << "           from.withdraw(amount);  // Delegate!\n";
    cout << "           to.deposit(amount);\n";
    cout << "       }\n";
    cout << "   };\n";

    cout << "\n✨ DEMONSTRATION:\n";

    good_example5::Account account1("ACC001", 1000.0, "USD");
    good_example5::Account account2("ACC002", 500.0, "USD");
    good_example5::TransactionProcessor processor;

    processor.processTransfer(account1, account2, 200.0);

    cout << "\n   Account 1 balance: $" << account1.getBalance() << endl;
    cout << "   Account 2 balance: $" << account2.getBalance() << endl;

    cout << "\n✨ BENEFITS:\n";
    cout << "   • Better encapsulation\n";
    cout << "   • Behavior with data\n";
    cout << "   • Easier to maintain\n";
    cout << "   • Clearer responsibilities\n";
}

// ============================================
// EXAMPLE 6: DATA CLUMPS
// ============================================

namespace bad_example6 {
// ❌ BAD: Same group of parameters everywhere

void drawRectangle(int x, int y, int width, int height) {
    cout << "   Drawing at (" << x << "," << y
         << ") size " << width << "x" << height << endl;
}

void moveRectangle(int& x, int& y, int width, int height, int dx, int dy) {
    // 🚨 Same parameters repeated
    x += dx;
    y += dy;
    cout << "   Moved to (" << x << "," << y << ")" << endl;
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
        cout << "(" << x << "," << y << ")";
    }
};

struct Size {
    int width, height;

    Size(int w = 0, int h = 0) : width(w), height(h) {}

    void display() const {
        cout << width << "x" << height;
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
        cout << "   Drawing at ";
        position.display();
        cout << " size ";
        dimensions.display();
        cout << endl;
    }

    void move(const Point& delta) {
        position = position + delta;
        cout << "   Moved to ";
        position.display();
        cout << endl;
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
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 6: DATA CLUMPS                         ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE:\n";
    cout << "   void drawRectangle(int x, int y, int width, int height);\n";
    cout << "   void moveRectangle(int& x, int& y, int width, int height, ...);\n";
    cout << "   bool isInside(int x, int y, int width, int height, ...);\n";
    cout << "   \n";
    cout << "   // 🚨 x, y, width, height always together!\n";

    cout << "\n🚨 SMELL INDICATORS:\n";
    cout << "   • Same parameters appear together repeatedly\n";
    cout << "   • High coupling between unrelated functions\n";
    cout << "   • Changes affect multiple function signatures\n";
    cout << "   • Missing abstraction\n";
    cout << "   • Poor cohesion\n";

    cout << "\n✅ GOOD CODE:\n";
    cout << "   struct Point { int x, y; };\n";
    cout << "   struct Size { int width, height; };\n";
    cout << "   \n";
    cout << "   class Rectangle {\n";
    cout << "       Point position;\n";
    cout << "       Size dimensions;\n";
    cout << "       \n";
    cout << "       void draw() const;\n";
    cout << "       void move(const Point& delta);\n";
    cout << "       bool contains(const Point& point) const;\n";
    cout << "   };\n";

    cout << "\n✨ DEMONSTRATION:\n";

    good_example6::Rectangle rect(good_example6::Point(10, 20),
                                  good_example6::Size(100, 50));
    rect.draw();
    rect.move(good_example6::Point(5, 5));

    cout << "   Point (15, 25) inside? "
         << (rect.contains(good_example6::Point(15, 25)) ? "Yes" : "No") << endl;

    cout << "\n✨ BENEFITS:\n";
    cout << "   • Clear abstraction\n";
    cout << "   • Reduced parameter count\n";
    cout << "   • Better encapsulation\n";
    cout << "   • Easier to extend\n";
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

    cout << "   Processing..." << endl;
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
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 7: COMMENTS AS SMELL                   ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE:\n";
    cout << "   // Calculate total including tax and shipping\n";
    cout << "   // For premium customers apply discount 🚨\n";
    cout << "   double calc(double p, bool prem) {\n";
    cout << "       double t = p * 0.1;  // tax 🚨\n";
    cout << "       double s = p > 50 ? 0 : 5.99;  // shipping 🚨\n";
    cout << "       // 🚨 If code needs this many comments...\n";
    cout << "       // ... it's probably too unclear!\n";
    cout << "   }\n";

    cout << "\n🚨 SMELL INDICATORS:\n";
    cout << "   • Excessive comments explaining WHAT code does\n";
    cout << "   • Comments make up > 30% of file\n";
    cout << "   • Outdated comments\n";
    cout << "   • Comments compensating for bad names\n";
    cout << "   • Code can't stand on its own\n";

    cout << "\n✅ GOOD CODE:\n";
    cout << "   const double TAX_RATE = 0.10;\n";
    cout << "   \n";
    cout << "   double calculateTax(double price) {\n";
    cout << "       return price * TAX_RATE;  // Clear! ✅\n";
    cout << "   }\n";
    cout << "   \n";
    cout << "   double calculateTotalPrice(double basePrice,\n";
    cout << "                             bool isPremiumCustomer) {\n";
    cout << "       // Self-explanatory code ✅\n";
    cout << "   }\n";
    cout << "   \n";
    cout << "   // Only comment WHY:\n";
    cout << "   // BUSINESS RULE: Premium discount encourages upgrades\n";

    cout << "\n✨ DEMONSTRATION:\n";

    double total1 = good_example7::calculateTotalPrice(100.0, false);
    double total2 = good_example7::calculateTotalPrice(100.0, true);

    cout << "   Regular customer: $" << total1 << endl;
    cout << "   Premium customer: $" << total2 << endl;

    cout << "\n✨ BENEFITS:\n";
    cout << "   • Code speaks for itself\n";
    cout << "   • Comments explain WHY, not WHAT\n";
    cout << "   • Easier to maintain\n";
    cout << "   • Less chance of outdated docs\n";
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
    virtual string getType() const = 0;
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

    string getType() const override { return "Engineer"; }
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

    string getType() const override { return "Manager"; }
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

    string getType() const override { return "Salesperson"; }
};

void processEmployee(const Employee& employee) {
    cout << "   " << employee.getType() << ":" << endl;
    cout << "   Salary: $" << employee.calculateSalary() << endl;
    cout << "   Bonus: $" << employee.calculateBonus() << endl;
}
}

void example8_switch_statements() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 8: SWITCH STATEMENTS SMELL             ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE:\n";
    cout << "   double calculateSalary(EmployeeType type, ...) {\n";
    cout << "       switch (type) {  🚨\n";
    cout << "           case ENGINEER: return 80000 + ...;\n";
    cout << "           case MANAGER: return 100000 + ...;\n";
    cout << "       }\n";
    cout << "   }\n";
    cout << "   \n";
    cout << "   double calculateBonus(EmployeeType type, ...) {\n";
    cout << "       switch (type) {  🚨 Same switch again!\n";
    cout << "           case ENGINEER: return ...;\n";
    cout << "           case MANAGER: return ...;\n";
    cout << "       }\n";
    cout << "   }\n";

    cout << "\n🚨 SMELL INDICATORS:\n";
    cout << "   • Same switch repeated in multiple places\n";
    cout << "   • Adding new type requires multiple changes\n";
    cout << "   • Violates Open-Closed Principle\n";
    cout << "   • Poor extensibility\n";
    cout << "   • Type-based branching\n";

    cout << "\n✅ GOOD CODE:\n";
    cout << "   class Employee {\n";
    cout << "       virtual double calculateSalary() const = 0;\n";
    cout << "       virtual double calculateBonus() const = 0;\n";
    cout << "   };\n";
    cout << "   \n";
    cout << "   class Engineer : public Employee {\n";
    cout << "       double calculateSalary() const override {...}\n";
    cout << "       double calculateBonus() const override {...}\n";
    cout << "   };\n";
    cout << "   \n";
    cout << "   // Add new type? Just create new class! ✅\n";

    cout << "\n✨ DEMONSTRATION:\n";

    good_example8::Engineer eng(8);
    good_example8::Manager mgr(9);
    good_example8::Salesperson sales(10);

    good_example8::processEmployee(eng);
    good_example8::processEmployee(mgr);
    good_example8::processEmployee(sales);

    cout << "\n✨ BENEFITS:\n";
    cout << "   • No switch statements\n";
    cout << "   • Easy to add new types\n";
    cout << "   • Better encapsulation\n";
    cout << "   • Open-Closed Principle\n";
    cout << "   • Polymorphism power\n";
}

// ============================================
// EXAMPLE 9: DETECTION SUMMARY
// ============================================

void example9_detection_summary() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 9: CODE SMELL DETECTION GUIDE          ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n📋 QUICK REFERENCE GUIDE:\n";

    cout << "\n1. LONG METHOD:\n";
    cout << "   🔍 Detection: Method > 20 lines\n";
    cout << "   💊 Cure: Extract smaller methods\n";
    cout << "   ✅ Result: Single responsibility per method\n";

    cout << "\n2. LARGE CLASS:\n";
    cout << "   🔍 Detection: Class > 200 lines, many methods\n";
    cout << "   💊 Cure: Split into smaller classes\n";
    cout << "   ✅ Result: High cohesion, low coupling\n";

    cout << "\n3. DUPLICATE CODE:\n";
    cout << "   🔍 Detection: Copy-paste code blocks\n";
    cout << "   💊 Cure: Extract to reusable functions\n";
    cout << "   ✅ Result: DRY principle\n";

    cout << "\n4. LONG PARAMETER LIST:\n";
    cout << "   🔍 Detection: > 3-4 parameters\n";
    cout << "   💊 Cure: Parameter objects/builders\n";
    cout << "   ✅ Result: Clear, manageable APIs\n";

    cout << "\n5. FEATURE ENVY:\n";
    cout << "   🔍 Detection: Method uses other class's data\n";
    cout << "   💊 Cure: Move method to data's class\n";
    cout << "   ✅ Result: Better encapsulation\n";

    cout << "\n6. DATA CLUMPS:\n";
    cout << "   🔍 Detection: Same parameters together\n";
    cout << "   💊 Cure: Create data class\n";
    cout << "   ✅ Result: Clear abstractions\n";

    cout << "\n7. PRIMITIVE OBSESSION:\n";
    cout << "   🔍 Detection: Primitives for domain concepts\n";
    cout << "   💊 Cure: Value objects\n";
    cout << "   ✅ Result: Type safety, validation\n";

    cout << "\n8. SWITCH STATEMENTS:\n";
    cout << "   🔍 Detection: Repeated type-based switches\n";
    cout << "   💊 Cure: Polymorphism\n";
    cout << "   ✅ Result: Open-Closed Principle\n";

    cout << "\n9. EXCESSIVE COMMENTS:\n";
    cout << "   🔍 Detection: Comments explaining WHAT\n";
    cout << "   💊 Cure: Self-documenting code\n";
    cout << "   ✅ Result: Clear, maintainable code\n";

    cout << "\n10. DEAD CODE:\n";
    cout << "   🔍 Detection: Unused methods/variables\n";
    cout << "   💊 Cure: Delete it!\n";
    cout << "   ✅ Result: Cleaner codebase\n";
}

// ============================================
// EXAMPLE 10: REFACTORING STRATEGIES
// ============================================

void example10_refactoring_strategies() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 10: REFACTORING STRATEGIES             ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n🔧 REFACTORING TECHNIQUES:\n";

    cout << "\n1. EXTRACT METHOD:\n";
    cout << "   • Break large method into smaller ones\n";
    cout << "   • Each method does one thing\n";
    cout << "   • Improves readability and reusability\n";

    cout << "\n2. EXTRACT CLASS:\n";
    cout << "   • Split large class into focused classes\n";
    cout << "   • Each class has single responsibility\n";
    cout << "   • Better organization and maintenance\n";

    cout << "\n3. INTRODUCE PARAMETER OBJECT:\n";
    cout << "   • Group related parameters\n";
    cout << "   • Reduce parameter lists\n";
    cout << "   • Create meaningful abstractions\n";

    cout << "\n4. REPLACE TYPE CODE WITH CLASS:\n";
    cout << "   • Convert primitives to value objects\n";
    cout << "   • Add validation and behavior\n";
    cout << "   • Improve type safety\n";

    cout << "\n5. MOVE METHOD:\n";
    cout << "   • Relocate method to proper class\n";
    cout << "   • Place behavior with data\n";
    cout << "   • Fix feature envy\n";

    cout << "\n6. REPLACE CONDITIONAL WITH POLYMORPHISM:\n";
    cout << "   • Convert switch to inheritance\n";
    cout << "   • Use polymorphism\n";
    cout << "   • Easier to extend\n";

    cout << "\n💡 REFACTORING WORKFLOW:\n";
    cout << "\n   1. Identify the smell 🔍\n";
    cout << "   2. Write tests first ✅\n";
    cout << "   3. Make small changes incrementally 🔧\n";
    cout << "   4. Run tests after each change 🧪\n";
    cout << "   5. Commit frequently 💾\n";
    cout << "   6. Review and iterate 🔄\n";

    cout << "\n⚠️  REFACTORING RULES:\n";
    cout << "\n   • Never refactor without tests\n";
    cout << "   • Make small, incremental changes\n";
    cout << "   • Keep code working at each step\n";
    cout << "   • Refactor OR add features, not both\n";
    cout << "   • Don't change behavior\n";
}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║                CODE SMELLS PRINCIPLE                  ║\n";
    cout << "║                                                       ║\n";
    cout << "║  \"Code smells are indicators of potential problems.   ║\n";
    cout << "║   They are not bugs but signs that code may need      ║\n";
    cout << "║   refactoring.\"                                      ║\n";
    cout << "║                       — Martin Fowler                 ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n";

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

    cout << "\n\n";
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║          🎉 CODE SMELLS PRINCIPLE COMPLETE! 🎉        ║\n";
    cout << "║                                                       ║\n";
    cout << "║  Key Takeaway: Code smells are your early warning     ║\n";
    cout << "║  system. Learn to recognize them, and refactor       ║\n";
    cout << "║  proactively before small problems become big ones!   ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n\n";

    return 0;
}
