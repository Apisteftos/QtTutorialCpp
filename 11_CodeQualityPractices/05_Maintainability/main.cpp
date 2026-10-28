#include <vector>
#include <string>
#include <memory>
#include <map>
#include <fstream>
#include <sstream>
#include <print>

// ============================================
// MAINTAINABILITY PRINCIPLE
// ============================================

/*
MAINTAINABILITY PRINCIPLE:
    "Code should be written to be easily understood, modified,
     and extended by others (including your future self)."
    — Software Engineering Best Practice

WHAT IT MEANS:
    • Write code that others can understand
    • Make changes easy and safe
    • Document intent and decisions
    • Minimize cognitive load
    • Think about future maintenance

WHY IT MATTERS:
    ✅ 80% of software cost is maintenance
    ✅ Code is read 10x more than written
    ✅ Reduces time to fix bugs
    ✅ Enables faster feature development
    ✅ Lower total cost of ownership

KEY ASPECTS:
    • Readability - Easy to read and understand
    • Modifiability - Easy to change
    • Testability - Easy to test
    • Debuggability - Easy to debug
    • Documentation - Self-documenting code
    • Consistency - Predictable patterns

MAINTAINABILITY FACTORS:
    1. Clear code structure
    2. Meaningful names
    3. Small, focused functions
    4. Proper comments
    5. Error handling
    6. Consistent style
    7. Low complexity
    8. Good tests

VIOLATION SIGNS:
    🚨 Hard to understand code
    🚨 Fear of making changes
    🚨 Bugs when modifying code
    🚨 Long debugging sessions
    🚨 Need original author to explain
    🚨 "Don't touch this code!"
*/

// ============================================
// EXAMPLE 1: READABLE CODE
// ============================================

namespace bad_example1 {
// ❌ BAD: Hard to read and understand

double calc(double a, double b, int t) {
    // 🚨 What do these variables mean?
    // 🚨 What is 't'? What are the magic numbers?
    if (t == 1) {
        return a * 0.1 + b;
    } else if (t == 2) {
        return a * 0.15 + b * 1.2;
    } else {
        return a * 0.08 + b * 1.1;
    }
}

void proc(std::vector<int>& v) {
    // 🚨 What does this do?
    for (int i = 0; i < v.size(); i++) {
        if (v[i] % 2 == 0) {
            v[i] = v[i] * 2;
        } else {
            v[i] = v[i] + 1;
        }
    }
}
}

namespace good_example1 {
// ✅ GOOD: Clear and readable

enum CustomerType {
    REGULAR,
    PREMIUM,
    VIP
};

const double REGULAR_TAX_RATE = 0.10;
const double PREMIUM_TAX_RATE = 0.15;
const double VIP_TAX_RATE = 0.08;

const double PREMIUM_BONUS = 1.2;
const double VIP_BONUS = 1.1;

double calculateTotalPrice(double basePrice, double discount, CustomerType type) {
    switch (type) {
    case REGULAR:
        return basePrice * (1 + REGULAR_TAX_RATE) - discount;

    case PREMIUM:
        return basePrice * (1 + PREMIUM_TAX_RATE) - (discount * PREMIUM_BONUS);

    case VIP:
        return basePrice * (1 + VIP_TAX_RATE) - (discount * VIP_BONUS);

    default:
        return basePrice - discount;
    }
}

void normalizeNumbers(std::vector<int>& numbers) {
    // Make even numbers double, odd numbers increment by 1
    for (int& number : numbers) {
        if (number % 2 == 0) {
            number *= 2;  // Double even numbers
        } else {
            number += 1;  // Increment odd numbers
        }
    }
}
}

void example1_readability() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 1: READABLE CODE                       ║");
    std::println("╚══════════════════════════════════════════════════╝");

    double result = good_example1::calculateTotalPrice(
        100.0, 10.0, good_example1::PREMIUM
        );
    std::println("");
    std::println("   Premium customer total: ${}", result);

    std::vector<int> numbers = {1, 2, 3, 4, 5};
    std::println("");
    std::print("   Before normalization: ");
    for (int n : numbers) std::print("{} ", n);

    good_example1::normalizeNumbers(numbers);
    std::println("");
    std::print("   After normalization:  ");
    for (int n : numbers) std::print("{} ", n);
    std::println("");

}

// ============================================
// EXAMPLE 2: SMALL, FOCUSED FUNCTIONS
// ============================================

namespace bad_example2 {
// ❌ BAD: Large, complex function (low maintainability)

void processOrder(int orderId) {
    // 🚨 This function does too much!

    // Validate order
    if (orderId <= 0) {
        std::println("   ❌ Invalid order ID");
        return;
    }

    // Calculate price
    double basePrice = 100.0;
    double tax = basePrice * 0.1;
    double shipping = basePrice > 50 ? 0 : 5.99;
    double total = basePrice + tax + shipping;

    // Check inventory
    bool inStock = true;  // Simulate check
    if (!inStock) {
        std::println("   ❌ Out of stock");
        return;
    }

    // Process payment
    std::println("   💳 Processing payment: ${}", total);

}
}

namespace good_example2 {
// ✅ GOOD: Small, focused functions (high maintainability)

const double TAX_RATE = 0.10;
const double SHIPPING_FEE = 5.99;
const double FREE_SHIPPING_THRESHOLD = 50.0;

bool validateOrderId(int orderId) {
    return orderId > 0;
}

double calculateTax(double price) {
    return price * TAX_RATE;
}

double calculateShipping(double price) {
    return price > FREE_SHIPPING_THRESHOLD ? 0 : SHIPPING_FEE;
}

double calculateTotal(double basePrice) {
    double tax = calculateTax(basePrice);
    double shipping = calculateShipping(basePrice);
    return basePrice + tax + shipping;
}

bool checkInventory(int orderId) {
    // Simulate inventory check
    std::println("   📦 Checking inventory for order #{}", orderId);
    return true;
}

void processPayment(double amount) {
    std::println("   💳 Processing payment: ${}", amount);
}

void updateInventory(int orderId) {
    std::println("   📦 Updating inventory for order #{}", orderId);
}

void sendConfirmationEmail(int orderId) {
    std::println("   📧 Sending confirmation for order #{}", orderId);
}

void logTransaction(int orderId, double amount) {
    std::println("   📝 Logged: Order #{} - ${}", orderId, amount);
}

void processOrder(int orderId) {
    // Each step is a clear, testable function
    if (!validateOrderId(orderId)) {
        std::println("   ❌ Invalid order ID");
        return;
    }

    double basePrice = 100.0;
    double total = calculateTotal(basePrice);

    if (!checkInventory(orderId)) {
        std::println("   ❌ Out of stock");
        return;
    }

    processPayment(total);
    updateInventory(orderId);
    sendConfirmationEmail(orderId);
    logTransaction(orderId, total);

    std::println("   ✅ Order #{} processed successfully", orderId);
}
}

void example2_small_functions() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 2: SMALL, FOCUSED FUNCTIONS            ║");
    std::println("╚══════════════════════════════════════════════════╝");

    good_example2::processOrder(12345);

}

// ============================================
// EXAMPLE 3: ERROR HANDLING
// ============================================

namespace bad_example3 {
// ❌ BAD: Poor error handling

int divide(int a, int b) {
    // 🚨 No error handling!
    return a / b;  // Crashes if b == 0
}

std::string readFile(const std::string& filename) {
    // 🚨 No error handling!
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();  // Returns empty if file doesn't exist
}
}

namespace good_example3 {
// ✅ GOOD: Proper error handling

struct Result {
    bool success;
    int value;
    std::string error;

    Result(bool s, int v, const std::string& e = "")
        : success(s), value(v), error(e) {}
};

Result divide(int a, int b) {
    if (b == 0) {
        return Result(false, 0, "Division by zero");
    }
    return Result(true, a / b);
}

struct FileResult {
    bool success;
    std::string content;
    std::string error;

    FileResult(bool s, const std::string& c, const std::string& e = "")
        : success(s), content(c), error(e) {}
};

FileResult readFile(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        return FileResult(false, "", "Could not open file: " + filename);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    if (file.fail() && !file.eof()) {
        return FileResult(false, "", "Error reading file: " + filename);
    }

    return FileResult(true, buffer.str());
}
}

void example3_error_handling() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 3: ERROR HANDLING                      ║");
    std::println("╚══════════════════════════════════════════════════╝");

    auto result1 = good_example3::divide(10, 2);
    if (result1.success) {
        std::println("   ✅ Result: {}", result1.value);
    } else {
        std::println("   ❌ Error: {}", result1.error);
    }

    std::println("");
    std::println("   Division by zero (10 / 0):");
    auto result2 = good_example3::divide(10, 0);
    if (result2.success) {
        std::println("   ✅ Result: {}", result2.value);
    } else {
        std::println("   ❌ Error: {}", result2.error);
    }

}

// ============================================
// EXAMPLE 4: MEANINGFUL COMMENTS
// ============================================

namespace bad_example4 {
// ❌ BAD: Useless or misleading comments

class Account {
private:
    double balance;

public:
    // Set the balance  🚨 Obvious, useless comment
    void setBalance(double b) {
        balance = b;
    }

    // Calculate
    double calc() {  // 🚨 What does this calculate?
        // Add 10 to balance  🚨 Says WHAT, not WHY
        return balance + 10;
    }

    void process() {
        // TODO: Fix this later  🚨 When? What needs fixing?
        balance *= 1.05;
    }
};
}

namespace good_example4 {
// ✅ GOOD: Meaningful, helpful comments

class Account {
private:
    double balance;

public:
    void setBalance(double newBalance) {
        // No comment needed - name is self-explanatory
        balance = newBalance;
    }

    double calculateBalanceWithMonthlyBonus() const {
        // Monthly bonus: $10 reward for account activity
        const double MONTHLY_BONUS = 10.0;
        return balance + MONTHLY_BONUS;
    }

    void applyAnnualInterest() {
        // Apply 5% annual interest rate
        // Rate set by financial regulation XYZ-2024
        const double ANNUAL_INTEREST_RATE = 0.05;
        balance *= (1 + ANNUAL_INTEREST_RATE);
    }

    bool canWithdraw(double amount) const {
        // Prevent withdrawal if it would cause overdraft
        // Business rule: Maintain minimum $50 balance
        const double MINIMUM_BALANCE = 50.0;
        return (balance - amount) >= MINIMUM_BALANCE;
    }
};
}

void example4_comments() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 4: MEANINGFUL COMMENTS                 ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   ");
    std::println("   ");

    std::println("   ");
    std::println("   ");

    std::println("   ");
}

// ============================================
// EXAMPLE 5: CONSISTENT CODE STYLE
// ============================================

namespace bad_example5 {
// ❌ BAD: Inconsistent style (hard to maintain)

class user {  // lowercase class name
private:
    std::string Name;  // Capital N
    int age_;     // underscore suffix

public:
    void SetName(std::string n){  // Capital S, no space before {
        Name=n;}  // no spaces around =

    int GetAge()  // Capital G
    {  // different brace style
        return age_ ;  // space before ;
    }

    // Inconsistent spacing, naming, formatting
};
}

namespace good_example5 {
// ✅ GOOD: Consistent style (easy to maintain)

class User {  // Consistent naming: PascalCase for classes
private:
    std::string name;  // Consistent: camelCase for members
    int age;

public:
    void setName(const std::string& newName) {  // Consistent formatting
        name = newName;
    }

    int getAge() const {
        return age;
    }

    bool isAdult() const {
        const int ADULT_AGE = 18;
        return age >= ADULT_AGE;
    }
};

// Consistent:
// - Naming conventions
// - Brace placement
// - Spacing
// - Parameter passing
}

void example5_consistency() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 5: CONSISTENT CODE STYLE               ║");
    std::println("╚══════════════════════════════════════════════════╝");

}

// ============================================
// EXAMPLE 6: TESTABILITY
// ============================================

namespace bad_example6 {
// ❌ BAD: Hard to test (low maintainability)

class OrderProcessor {
public:
    void process(int orderId) {
        // 🚨 Tightly coupled to concrete classes
        // 🚨 Hard to test without real database and email

    }
};
}

namespace good_example6 {
// ✅ GOOD: Easy to test (high maintainability)

// Interfaces for dependencies
class IDatabase {
public:
    virtual ~IDatabase() = default;
    virtual void save(int orderId) = 0;
};

class IEmailService {
public:
    virtual ~IEmailService() = default;
    virtual void send(int orderId) = 0;
};

class ITimeProvider {
public:
    virtual ~ITimeProvider() = default;
    virtual std::string getCurrentTime() = 0;
};

// Testable implementation
class OrderProcessor {
private:
    IDatabase* database;
    IEmailService* emailService;
    ITimeProvider* timeProvider;

public:
    OrderProcessor(IDatabase* db, IEmailService* email, ITimeProvider* time)
        : database(db), emailService(email), timeProvider(time) {}

    void process(int orderId) {
        // All dependencies injected - easy to mock!
        std::println("   Processing order #{}", orderId);
        database->save(orderId);
        emailService->send(orderId);
        std::println("   Time: {}", timeProvider->getCurrentTime());
    }
};

// Mock implementations for testing
class MockDatabase : public IDatabase {
public:
    void save(int orderId) override {
        std::println("   🧪 Mock: Saved order #{}", orderId);
    }
};

class MockEmailService : public IEmailService {
public:
    void send(int orderId) override {
        std::println("   🧪 Mock: Sent email for order #{}", orderId);
    }
};

class MockTimeProvider : public ITimeProvider {
public:
    std::string getCurrentTime() override {
        return "2024-01-01 12:00:00";  // Predictable time
    }
};
}

void example6_testability() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 6: TESTABILITY                         ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("       ");

    good_example6::MockDatabase mockDb;
    good_example6::MockEmailService mockEmail;
    good_example6::MockTimeProvider mockTime;

    good_example6::OrderProcessor processor(&mockDb, &mockEmail, &mockTime);
    processor.process(12345);

}

// ============================================
// EXAMPLE 7: MAINTAINABILITY METRICS
// ============================================

void example7_metrics() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 7: MAINTAINABILITY METRICS             ║");
    std::println("╚══════════════════════════════════════════════════╝");

}

// ============================================
// EXAMPLE 8: BEST PRACTICES
// ============================================

void example8_best_practices() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 8: MAINTAINABILITY BEST PRACTICES      ║");
    std::println("╚══════════════════════════════════════════════════╝");

}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    std::println("╔═══════════════════════════════════════════════════════╗");
    std::println("║                                                       ║");
    std::println("║            MAINTAINABILITY PRINCIPLE                  ║");
    std::println("║                                                       ║");
    std::println("║  \"Code should be written to be easily understood,    ║");
    std::println("║   modified, and extended by others (including your    ║");
    std::println("║   future self).\"                                     ║");
    std::println("║                                                       ║");
    std::println("╚═══════════════════════════════════════════════════════╝");

    example1_readability();
    example2_small_functions();
    example3_error_handling();
    example4_comments();
    example5_consistency();
    example6_testability();
    example7_metrics();
    example8_best_practices();

    return 0;
}
