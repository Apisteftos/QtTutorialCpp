#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <fstream>
#include <sstream>
using namespace std;

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

void proc(vector<int>& v) {
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

void normalizeNumbers(vector<int>& numbers) {
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
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 1: READABLE CODE                       ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (Hard to Read):\n";
    cout << "```cpp\n";
    cout << "double calc(double a, double b, int t) {\n";
    cout << "    if (t == 1) return a * 0.1 + b;\n";
    cout << "    else if (t == 2) return a * 0.15 + b * 1.2;\n";
    cout << "    // What are a, b, t? What are the magic numbers?\n";
    cout << "}\n";
    cout << "```\n";

    cout << "\n🚨 PROBLEMS:\n";
    cout << "   • Unclear variable names (a, b, t)\n";
    cout << "   • Magic numbers (0.1, 0.15, 1.2)\n";
    cout << "   • No comments explaining logic\n";
    cout << "   • Hard to understand intent\n";

    cout << "\n✅ GOOD CODE (Clear and Readable):\n";
    cout << "```cpp\n";
    cout << "enum CustomerType { REGULAR, PREMIUM, VIP };\n";
    cout << "const double REGULAR_TAX_RATE = 0.10;\n\n";
    cout << "double calculateTotalPrice(double basePrice,\n";
    cout << "                          double discount,\n";
    cout << "                          CustomerType type) {\n";
    cout << "    // Clear intent, meaningful names\n";
    cout << "}\n";
    cout << "```\n";

    cout << "\n✨ DEMONSTRATION:\n";

    double result = good_example1::calculateTotalPrice(
        100.0, 10.0, good_example1::PREMIUM
        );
    cout << "\n   Premium customer total: $" << result << endl;

    vector<int> numbers = {1, 2, 3, 4, 5};
    cout << "\n   Before normalization: ";
    for (int n : numbers) cout << n << " ";

    good_example1::normalizeNumbers(numbers);
    cout << "\n   After normalization:  ";
    for (int n : numbers) cout << n << " ";
    cout << endl;

    cout << "\n✨ BENEFITS:\n";
    cout << "   • Immediately understand what code does\n";
    cout << "   • Self-documenting with clear names\n";
    cout << "   • Easy to modify or extend\n";
    cout << "   • Anyone can maintain it\n";
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
        cout << "   ❌ Invalid order ID" << endl;
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
        cout << "   ❌ Out of stock" << endl;
        return;
    }

    // Process payment
    cout << "   💳 Processing payment: $" << total << endl;

    // Update inventory
    cout << "   📦 Updating inventory" << endl;

    // Send confirmation email
    cout << "   📧 Sending confirmation email" << endl;

    // Log transaction
    cout << "   📝 Logging transaction" << endl;

    cout << "   ✅ Order processed successfully" << endl;
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
    cout << "   📦 Checking inventory for order #" << orderId << endl;
    return true;
}

void processPayment(double amount) {
    cout << "   💳 Processing payment: $" << amount << endl;
}

void updateInventory(int orderId) {
    cout << "   📦 Updating inventory for order #" << orderId << endl;
}

void sendConfirmationEmail(int orderId) {
    cout << "   📧 Sending confirmation for order #" << orderId << endl;
}

void logTransaction(int orderId, double amount) {
    cout << "   📝 Logged: Order #" << orderId << " - $" << amount << endl;
}

void processOrder(int orderId) {
    // Each step is a clear, testable function
    if (!validateOrderId(orderId)) {
        cout << "   ❌ Invalid order ID" << endl;
        return;
    }

    double basePrice = 100.0;
    double total = calculateTotal(basePrice);

    if (!checkInventory(orderId)) {
        cout << "   ❌ Out of stock" << endl;
        return;
    }

    processPayment(total);
    updateInventory(orderId);
    sendConfirmationEmail(orderId);
    logTransaction(orderId, total);

    cout << "   ✅ Order #" << orderId << " processed successfully" << endl;
}
}

void example2_small_functions() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 2: SMALL, FOCUSED FUNCTIONS           ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE:\n";
    cout << "   One large function doing:\n";
    cout << "   • Validation\n";
    cout << "   • Price calculation\n";
    cout << "   • Inventory check\n";
    cout << "   • Payment processing\n";
    cout << "   • Email sending\n";
    cout << "   • Logging\n";
    cout << "   All in 50+ lines!\n";

    cout << "\n✅ GOOD CODE:\n";
    cout << "   Separate functions for each task:\n";
    cout << "   • validateOrderId()\n";
    cout << "   • calculateTotal()\n";
    cout << "   • checkInventory()\n";
    cout << "   • processPayment()\n";
    cout << "   • sendConfirmationEmail()\n";
    cout << "   • logTransaction()\n";
    cout << "   Each function: 3-5 lines, single purpose\n";

    cout << "\n✨ DEMONSTRATION:\n";

    good_example2::processOrder(12345);

    cout << "\n✨ BENEFITS:\n";
    cout << "   • Easy to understand each step\n";
    cout << "   • Easy to test individual functions\n";
    cout << "   • Easy to modify one aspect\n";
    cout << "   • Reusable functions\n";
    cout << "   • Clear intent\n";
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

string readFile(const string& filename) {
    // 🚨 No error handling!
    ifstream file(filename);
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();  // Returns empty if file doesn't exist
}
}

namespace good_example3 {
// ✅ GOOD: Proper error handling

struct Result {
    bool success;
    int value;
    string error;

    Result(bool s, int v, const string& e = "")
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
    string content;
    string error;

    FileResult(bool s, const string& c, const string& e = "")
        : success(s), content(c), error(e) {}
};

FileResult readFile(const string& filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        return FileResult(false, "", "Could not open file: " + filename);
    }

    stringstream buffer;
    buffer << file.rdbuf();

    if (file.fail() && !file.eof()) {
        return FileResult(false, "", "Error reading file: " + filename);
    }

    return FileResult(true, buffer.str());
}
}

void example3_error_handling() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 3: ERROR HANDLING                      ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE:\n";
    cout << "   int divide(int a, int b) {\n";
    cout << "       return a / b;  // 🚨 Crashes if b == 0\n";
    cout << "   }\n";

    cout << "\n✅ GOOD CODE:\n";
    cout << "   Result divide(int a, int b) {\n";
    cout << "       if (b == 0) {\n";
    cout << "           return Result(false, 0, \"Division by zero\");\n";
    cout << "       }\n";
    cout << "       return Result(true, a / b);\n";
    cout << "   }\n";

    cout << "\n✨ DEMONSTRATION:\n";

    cout << "\n   Valid division (10 / 2):\n";
    auto result1 = good_example3::divide(10, 2);
    if (result1.success) {
        cout << "   ✅ Result: " << result1.value << endl;
    } else {
        cout << "   ❌ Error: " << result1.error << endl;
    }

    cout << "\n   Division by zero (10 / 0):\n";
    auto result2 = good_example3::divide(10, 0);
    if (result2.success) {
        cout << "   ✅ Result: " << result2.value << endl;
    } else {
        cout << "   ❌ Error: " << result2.error << endl;
    }

    cout << "\n✨ BENEFITS:\n";
    cout << "   • No crashes from unexpected input\n";
    cout << "   • Clear error messages\n";
    cout << "   • Easy to debug problems\n";
    cout << "   • Graceful degradation\n";
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
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 4: MEANINGFUL COMMENTS                 ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD COMMENTS:\n";
    cout << "   // Set the balance     🚨 Obvious\n";
    cout << "   void setBalance(double b) { balance = b; }\n";
    cout << "   \n";
    cout << "   // Calculate           🚨 Calculate what?\n";
    cout << "   double calc() { }\n";
    cout << "   \n";
    cout << "   // TODO: Fix this      🚨 What needs fixing?\n";

    cout << "\n✅ GOOD COMMENTS:\n";
    cout << "   // Monthly bonus: $10 reward for account activity\n";
    cout << "   // Explains WHY, not WHAT\n";
    cout << "   \n";
    cout << "   // Apply 5% annual interest rate\n";
    cout << "   // Rate set by financial regulation XYZ-2024\n";
    cout << "   // Provides context and reason\n";
    cout << "   \n";
    cout << "   // Business rule: Maintain minimum $50 balance\n";
    cout << "   // Explains business logic\n";

    cout << "\n💡 COMMENT GUIDELINES:\n";
    cout << "   ✅ DO comment:\n";
    cout << "      • WHY code does something (intent)\n";
    cout << "      • Business rules and constraints\n";
    cout << "      • Complex algorithms\n";
    cout << "      • Workarounds and their reasons\n";
    cout << "   \n";
    cout << "   ❌ DON'T comment:\n";
    cout << "      • WHAT code does (should be obvious)\n";
    cout << "      • Obvious operations\n";
    cout << "      • Redundant information\n";
    cout << "      • Outdated TODOs\n";
}

// ============================================
// EXAMPLE 5: CONSISTENT CODE STYLE
// ============================================

namespace bad_example5 {
// ❌ BAD: Inconsistent style (hard to maintain)

class user {  // lowercase class name
private:
    string Name;  // Capital N
    int age_;     // underscore suffix

public:
    void SetName(string n){  // Capital S, no space before {
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
    string name;  // Consistent: camelCase for members
    int age;

public:
    void setName(const string& newName) {  // Consistent formatting
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
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 5: CONSISTENT CODE STYLE               ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (Inconsistent):\n";
    cout << "   class user {              // lowercase\n";
    cout << "       string Name;          // Capital N\n";
    cout << "       void SetName(string n){  // no space\n";
    cout << "           Name=n;}          // no spaces\n";
    cout << "       int GetAge()          // different style\n";
    cout << "       {                     // different brace\n";
    cout << "   };\n";

    cout << "\n✅ GOOD CODE (Consistent):\n";
    cout << "   class User {              // PascalCase\n";
    cout << "       string name;          // camelCase\n";
    cout << "       void setName(const string& newName) {\n";
    cout << "           name = newName;\n";
    cout << "       }\n";
    cout << "       int getAge() const {\n";
    cout << "           return age;\n";
    cout << "       }\n";
    cout << "   };\n";

    cout << "\n📐 CONSISTENCY MATTERS:\n";
    cout << "   ✅ Naming conventions (PascalCase, camelCase)\n";
    cout << "   ✅ Brace placement (same style everywhere)\n";
    cout << "   ✅ Spacing (consistent indentation)\n";
    cout << "   ✅ Parameter passing (const references)\n";
    cout << "   ✅ Const correctness (mark what doesn't change)\n";

    cout << "\n💡 BENEFITS:\n";
    cout << "   • Predictable code structure\n";
    cout << "   • Faster to read and understand\n";
    cout << "   • Easier to spot anomalies\n";
    cout << "   • Professional appearance\n";
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

        // Database access (can't mock)
        cout << "   💾 Saving to real database..." << endl;

        // Email sending (can't mock)
        cout << "   📧 Sending real email..." << endl;

        // Time dependency (not testable)
        cout << "   🕐 Using current time..." << endl;
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
    virtual string getCurrentTime() = 0;
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
        cout << "   Processing order #" << orderId << endl;
        database->save(orderId);
        emailService->send(orderId);
        cout << "   Time: " << timeProvider->getCurrentTime() << endl;
    }
};

// Mock implementations for testing
class MockDatabase : public IDatabase {
public:
    void save(int orderId) override {
        cout << "   🧪 Mock: Saved order #" << orderId << endl;
    }
};

class MockEmailService : public IEmailService {
public:
    void send(int orderId) override {
        cout << "   🧪 Mock: Sent email for order #" << orderId << endl;
    }
};

class MockTimeProvider : public ITimeProvider {
public:
    string getCurrentTime() override {
        return "2024-01-01 12:00:00";  // Predictable time
    }
};
}

void example6_testability() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 6: TESTABILITY                         ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE:\n";
    cout << "   class OrderProcessor {\n";
    cout << "       void process(int orderId) {\n";
    cout << "           // Direct database access 🚨\n";
    cout << "           // Direct email sending 🚨\n";
    cout << "           // Current time dependency 🚨\n";
    cout << "           // Can't test without real resources!\n";
    cout << "       }\n";
    cout << "   };\n";

    cout << "\n✅ GOOD CODE:\n";
    cout << "   class OrderProcessor {\n";
    cout << "       IDatabase* database;        // Injected\n";
    cout << "       IEmailService* email;       // Injected\n";
    cout << "       ITimeProvider* time;        // Injected\n";
    cout << "       \n";
    cout << "       OrderProcessor(IDatabase*, IEmailService*, ITimeProvider*);\n";
    cout << "       // Can inject mocks for testing!\n";
    cout << "   };\n";

    cout << "\n✨ DEMONSTRATION (Testing with Mocks):\n";

    good_example6::MockDatabase mockDb;
    good_example6::MockEmailService mockEmail;
    good_example6::MockTimeProvider mockTime;

    good_example6::OrderProcessor processor(&mockDb, &mockEmail, &mockTime);
    processor.process(12345);

    cout << "\n✨ BENEFITS:\n";
    cout << "   • Easy to write unit tests\n";
    cout << "   • No need for real database/email\n";
    cout << "   • Fast test execution\n";
    cout << "   • Predictable test results\n";
    cout << "   • Better confidence in code\n";
}

// ============================================
// EXAMPLE 7: MAINTAINABILITY METRICS
// ============================================

void example7_metrics() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 7: MAINTAINABILITY METRICS             ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n📊 KEY MAINTAINABILITY METRICS:\n";

    cout << "\n1. CYCLOMATIC COMPLEXITY:\n";
    cout << "   Measures number of paths through code\n";
    cout << "   • 1-10:  Simple, easy to maintain ✅\n";
    cout << "   • 11-20: Moderate complexity ⚠️\n";
    cout << "   • 21+:   High complexity, hard to maintain 🚨\n";

    cout << "\n2. LINES OF CODE PER FUNCTION:\n";
    cout << "   • 1-20 lines:  Ideal ✅\n";
    cout << "   • 21-50 lines: Acceptable ⚠️\n";
    cout << "   • 51+ lines:   Too long, split it 🚨\n";

    cout << "\n3. NESTING DEPTH:\n";
    cout << "   How many levels of if/for/while\n";
    cout << "   • 1-2 levels: Good ✅\n";
    cout << "   • 3 levels:   Maximum acceptable ⚠️\n";
    cout << "   • 4+ levels:  Too deep, refactor 🚨\n";

    cout << "\n4. CODE CHURN:\n";
    cout << "   How often code changes\n";
    cout << "   • Low churn:  Stable code ✅\n";
    cout << "   • High churn: Potential quality issues 🚨\n";

    cout << "\n5. TEST COVERAGE:\n";
    cout << "   • 80-100%: Excellent ✅\n";
    cout << "   • 60-79%:  Good ⚠️\n";
    cout << "   • <60%:    Risky 🚨\n";

    cout << "\n6. COMMENT DENSITY:\n";
    cout << "   • 10-20%:  Good balance ✅\n";
    cout << "   • <5%:     Needs more explanation ⚠️\n";
    cout << "   • >30%:    Overly commented or unclear code 🚨\n";

    cout << "\n💡 MAINTAINABILITY INDEX:\n";
    cout << "   Combined metric from above factors:\n";
    cout << "   • 85-100: Highly maintainable ✅\n";
    cout << "   • 65-84:  Moderately maintainable ⚠️\n";
    cout << "   • 0-64:   Difficult to maintain 🚨\n";
}

// ============================================
// EXAMPLE 8: BEST PRACTICES
// ============================================

void example8_best_practices() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 8: MAINTAINABILITY BEST PRACTICES      ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n✅ BEST PRACTICES FOR MAINTAINABILITY:\n";

    cout << "\n1. WRITE SELF-DOCUMENTING CODE:\n";
    cout << "   • Meaningful variable names\n";
    cout << "   • Clear function names\n";
    cout << "   • Obvious control flow\n";
    cout << "   • Named constants instead of magic numbers\n";

    cout << "\n2. KEEP FUNCTIONS SMALL:\n";
    cout << "   • One function, one purpose\n";
    cout << "   • 10-20 lines ideal\n";
    cout << "   • Easy to understand at a glance\n";
    cout << "   • Easy to test\n";

    cout << "\n3. MINIMIZE NESTING:\n";
    cout << "   • Use early returns\n";
    cout << "   • Extract nested logic to functions\n";
    cout << "   • Avoid deep if-else chains\n";
    cout << "   • Use guard clauses\n";

    cout << "\n4. HANDLE ERRORS PROPERLY:\n";
    cout << "   • Check for error conditions\n";
    cout << "   • Provide meaningful error messages\n";
    cout << "   • Don't ignore errors\n";
    cout << "   • Fail gracefully\n";

    cout << "\n5. WRITE TESTS:\n";
    cout << "   • Unit tests for functions\n";
    cout << "   • Integration tests for components\n";
    cout << "   • Tests document expected behavior\n";
    cout << "   • Confidence to refactor\n";

    cout << "\n6. USE CONSISTENT STYLE:\n";
    cout << "   • Follow project conventions\n";
    cout << "   • Use code formatters\n";
    cout << "   • Consistent naming\n";
    cout << "   • Consistent structure\n";

    cout << "\n7. AVOID PREMATURE OPTIMIZATION:\n";
    cout << "   • Write clear code first\n";
    cout << "   • Optimize only if needed\n";
    cout << "   • Measure before optimizing\n";
    cout << "   • Clear code > Clever code\n";

    cout << "\n8. DOCUMENT DECISIONS:\n";
    cout << "   • Why you chose an approach\n";
    cout << "   • Known limitations\n";
    cout << "   • Future considerations\n";
    cout << "   • Business rules and constraints\n";

    cout << "\n🎯 GOLDEN RULE:\n";
    cout << "   \"Always code as if the person who ends up maintaining\n";
    cout << "    your code is a violent psychopath who knows where\n";
    cout << "    you live.\"\n";
    cout << "   — Martin Golding\n";
}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║            MAINTAINABILITY PRINCIPLE                  ║\n";
    cout << "║                                                       ║\n";
    cout << "║  \"Code should be written to be easily understood,     ║\n";
    cout << "║   modified, and extended by others (including your    ║\n";
    cout << "║   future self).\"                                     ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n";

    example1_readability();
    example2_small_functions();
    example3_error_handling();
    example4_comments();
    example5_consistency();
    example6_testability();
    example7_metrics();
    example8_best_practices();

    cout << "\n\n";
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║       🎉 MAINTAINABILITY PRINCIPLE COMPLETE! 🎉       ║\n";
    cout << "║                                                       ║\n";
    cout << "║  Key Takeaway: Write code for humans first,           ║\n";
    cout << "║  computers second. Your future self will thank you!   ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n\n";

    return 0;
}
