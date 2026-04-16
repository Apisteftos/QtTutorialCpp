#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <algorithm>
#include <cmath>
using namespace std;

// ============================================
// WHAT IS CODE QUALITY?
// ============================================

/*
CODE QUALITY:
    The measure of how well code is written in terms of
    maintainability, readability, reliability, and efficiency!

CHARACTERISTICS OF QUALITY CODE:

1. READABLE:
   - Clear, self-documenting
   - Easy to understand at first glance
   - Meaningful names

2. MAINTAINABLE:
   - Easy to modify and extend
   - Well-organized
   - Modular

3. RELIABLE:
   - Works correctly
   - Handles errors gracefully
   - Tested

4. EFFICIENT:
   - Performs well
   - Uses resources wisely
   - Not prematurely optimized

5. SIMPLE:
   - No unnecessary complexity
   - Clear logic
   - Direct approach

WHY CODE QUALITY MATTERS:

   💰 COST: Poor quality costs 3-10x more to maintain
   🐛 BUGS: Quality code has fewer defects
   ⏱️  TIME: Quality code is faster to modify
   😊 TEAM: Easier for team collaboration
   📈 SCALABILITY: Quality code scales better

CODE SMELLS:
    Warning signs that indicate potential problems!

CATEGORIES OF CODE SMELLS:

1. BLOATERS: Too big, too long
   - Long methods/functions
   - Large classes
   - Long parameter lists

2. OBJECT-ORIENTATION ABUSERS:
   - God objects (do everything)
   - Refused inheritance
   - Poor abstraction

3. CHANGE PREVENTERS:
   - Divergent change
   - Shotgun surgery (one change = many files)
   - Parallel inheritance

4. DISPENSABLES:
   - Dead code
   - Duplicate code
   - Speculative generality

5. COUPLERS:
   - High coupling
   - Feature envy
   - Inappropriate intimacy

METRICS TO MEASURE QUALITY:

   • Cyclomatic Complexity (< 10 is good)
   • Lines of Code per function (< 50 is good)
   • Code Coverage (> 80% is good)
   • Technical Debt Ratio
   • Coupling/Cohesion
   • Code Duplication %
*/

// ============================================
// EXAMPLE 1: MAGIC NUMBERS (Code Smell)
// ============================================

void example1_magic_numbers() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 1: MAGIC NUMBERS (Code Smell)         ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    // ❌ BAD CODE: Magic numbers everywhere
    cout << "\n❌ BAD CODE:\n";
    cout << "```cpp\n";
    cout << "void processOrder(double amount) {\n";
    cout << "    if (amount > 1000) {\n";
    cout << "        amount *= 0.9;  // What is 0.9?\n";
    cout << "    }\n";
    cout << "    double tax = amount * 0.19;  // What is 0.19?\n";
    cout << "    double shipping = amount > 50 ? 0 : 5.99;  // What are these?\n";
    cout << "}\n";
    cout << "```\n";

    cout << "\n🚨 PROBLEMS:\n";
    cout << "   • 0.9, 0.19, 50, 5.99 - what do they mean?\n";
    cout << "   • Hard to maintain (what if tax rate changes?)\n";
    cout << "   • Can't understand without context\n";
    cout << "   • Easy to make typos (0.19 vs 0.91)\n";

    // ✅ GOOD CODE: Named constants
    cout << "\n✅ GOOD CODE:\n";
    cout << "```cpp\n";
    cout << "const double BULK_ORDER_THRESHOLD = 1000.0;\n";
    cout << "const double BULK_DISCOUNT_RATE = 0.1;  // 10% off\n";
    cout << "const double TAX_RATE = 0.19;           // 19% VAT\n";
    cout << "const double FREE_SHIPPING_MIN = 50.0;\n";
    cout << "const double STANDARD_SHIPPING = 5.99;\n\n";
    cout << "void processOrder(double amount) {\n";
    cout << "    if (amount > BULK_ORDER_THRESHOLD) {\n";
    cout << "        amount *= (1.0 - BULK_DISCOUNT_RATE);\n";
    cout << "    }\n";
    cout << "    double tax = amount * TAX_RATE;\n";
    cout << "    double shipping = amount > FREE_SHIPPING_MIN ? 0 : STANDARD_SHIPPING;\n";
    cout << "}\n";
    cout << "```\n";

    cout << "\n✨ IMPROVEMENTS:\n";
    cout << "   ✅ Self-documenting - clear meaning\n";
    cout << "   ✅ Easy to maintain - change in one place\n";
    cout << "   ✅ No confusion about values\n";
    cout << "   ✅ Less prone to typos\n";

    cout << "\n💡 KEY PRINCIPLE:\n";
    cout << "   \"Every magic number should be a named constant!\"\n";
}

// ============================================
// EXAMPLE 2: LONG FUNCTIONS (Bloater Smell)
// ============================================

void example2_long_functions() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 2: LONG FUNCTIONS (Bloater Smell)     ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    // ❌ BAD CODE: One giant function
    cout << "\n❌ BAD CODE (100+ lines in one function):\n";
    cout << "```cpp\n";
    cout << "void processUserData() {\n";
    cout << "    // Validate input (20 lines)\n";
    cout << "    if (name.empty()) { /* error */ }\n";
    cout << "    if (email.empty()) { /* error */ }\n";
    cout << "    if (!isValidEmail(email)) { /* error */ }\n";
    cout << "    // ... more validation\n\n";
    cout << "    // Parse data (30 lines)\n";
    cout << "    string firstName = extractFirstName(name);\n";
    cout << "    string lastName = extractLastName(name);\n";
    cout << "    // ... more parsing\n\n";
    cout << "    // Calculate values (20 lines)\n";
    cout << "    double discount = calculateDiscount(...);\n";
    cout << "    double tax = calculateTax(...);\n";
    cout << "    // ... more calculations\n\n";
    cout << "    // Save to database (30 lines)\n";
    cout << "    connection.open();\n";
    cout << "    // ... database operations\n";
    cout << "    connection.close();\n";
    cout << "}\n";
    cout << "```\n";

    cout << "\n🚨 PROBLEMS:\n";
    cout << "   • Hard to understand - too much at once\n";
    cout << "   • Difficult to test - tests everything\n";
    cout << "   • Hard to reuse - can't use parts separately\n";
    cout << "   • High complexity - many responsibilities\n";
    cout << "   • Difficult to debug - where is the problem?\n";

    // ✅ GOOD CODE: Small, focused functions
    cout << "\n✅ GOOD CODE (Multiple small functions):\n";
    cout << "```cpp\n";
    cout << "bool validateUserInput(const User& user) {\n";
    cout << "    return !user.name.empty() && \n";
    cout << "           !user.email.empty() && \n";
    cout << "           isValidEmail(user.email);\n";
    cout << "}\n\n";
    cout << "UserData parseUserData(const User& user) {\n";
    cout << "    UserData data;\n";
    cout << "    data.firstName = extractFirstName(user.name);\n";
    cout << "    data.lastName = extractLastName(user.name);\n";
    cout << "    return data;\n";
    cout << "}\n\n";
    cout << "OrderSummary calculateOrderDetails(const Order& order) {\n";
    cout << "    OrderSummary summary;\n";
    cout << "    summary.discount = calculateDiscount(order);\n";
    cout << "    summary.tax = calculateTax(order);\n";
    cout << "    return summary;\n";
    cout << "}\n\n";
    cout << "void saveToDatabase(const UserData& data) {\n";
    cout << "    DatabaseConnection conn;\n";
    cout << "    conn.save(data);\n";
    cout << "}\n\n";
    cout << "void processUserData(const User& user) {\n";
    cout << "    if (!validateUserInput(user)) return;\n";
    cout << "    \n";
    cout << "    auto userData = parseUserData(user);\n";
    cout << "    auto orderDetails = calculateOrderDetails(user.order);\n";
    cout << "    saveToDatabase(userData);\n";
    cout << "}\n";
    cout << "```\n";

    cout << "\n✨ IMPROVEMENTS:\n";
    cout << "   ✅ Each function has ONE clear purpose\n";
    cout << "   ✅ Easy to understand - clear names\n";
    cout << "   ✅ Easy to test - test each part separately\n";
    cout << "   ✅ Reusable - use validation elsewhere\n";
    cout << "   ✅ Low complexity - simple logic\n";

    cout << "\n💡 KEY PRINCIPLE:\n";
    cout << "   \"Functions should be small (10-20 lines) and do ONE thing!\"\n";
}

// ============================================
// EXAMPLE 3: UNCLEAR NAMING (Readability Smell)
// ============================================

void example3_unclear_naming() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 3: UNCLEAR NAMING (Readability)       ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    // ❌ BAD CODE: Cryptic names
    cout << "\n❌ BAD CODE:\n";
    cout << "```cpp\n";
    cout << "int d;           // What is d?\n";
    cout << "string s;        // What is s?\n";
    cout << "double calc(int x, int y) {  // Calculate what?\n";
    cout << "    int tmp = x * y;         // Temporary what?\n";
    cout << "    return tmp * 1.19;\n";
    cout << "}\n\n";
    cout << "void proc(vector<int>& v) {  // Process what?\n";
    cout << "    for (auto& i : v) {      // i is what?\n";
    cout << "        i *= 2;\n";
    cout << "    }\n";
    cout << "}\n\n";
    cout << "class Data {                 // What data?\n";
    cout << "    int val1, val2;          // What values?\n";
    cout << "    void do_it();            // Do what?\n";
    cout << "};\n";
    cout << "```\n";

    cout << "\n🚨 PROBLEMS:\n";
    cout << "   • d, s, x, y - meaningless single letters\n";
    cout << "   • tmp - temporary what?\n";
    cout << "   • calc(), proc() - vague verbs\n";
    cout << "   • Data, val1 - generic names\n";
    cout << "   • Need comments to understand\n";

    // ✅ GOOD CODE: Clear, descriptive names
    cout << "\n✅ GOOD CODE:\n";
    cout << "```cpp\n";
    cout << "int daysUntilExpiry;\n";
    cout << "string customerEmail;\n\n";
    cout << "double calculatePriceWithTax(int quantity, int pricePerUnit) {\n";
    cout << "    int subtotal = quantity * pricePerUnit;\n";
    cout << "    const double TAX_RATE = 1.19;  // 19% VAT\n";
    cout << "    return subtotal * TAX_RATE;\n";
    cout << "}\n\n";
    cout << "void doubleAllValues(vector<int>& numbers) {\n";
    cout << "    for (auto& number : numbers) {\n";
    cout << "        number *= 2;\n";
    cout << "    }\n";
    cout << "}\n\n";
    cout << "class CustomerAccount {\n";
    cout << "    int accountBalance;\n";
    cout << "    int creditLimit;\n";
    cout << "    void processPayment();\n";
    cout << "};\n";
    cout << "```\n";

    cout << "\n✨ IMPROVEMENTS:\n";
    cout << "   ✅ Self-explanatory names\n";
    cout << "   ✅ Clear purpose and intent\n";
    cout << "   ✅ No comments needed\n";
    cout << "   ✅ Easy to understand\n";
    cout << "   ✅ Professional code\n";

    cout << "\n💡 KEY PRINCIPLE:\n";
    cout << "   \"Names should reveal intent - code should read like prose!\"\n";
}

// ============================================
// EXAMPLE 4: GOD OBJECT (OO Abuse Smell)
// ============================================

void example4_god_object() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 4: GOD OBJECT (OO Abuse Smell)        ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    // ❌ BAD CODE: One class does everything
    cout << "\n❌ BAD CODE (God Object):\n";
    cout << "```cpp\n";
    cout << "class Application {\n";
    cout << "public:\n";
    cout << "    // User management\n";
    cout << "    void createUser();\n";
    cout << "    void deleteUser();\n";
    cout << "    void updateUser();\n";
    cout << "    \n";
    cout << "    // Database operations\n";
    cout << "    void connectDatabase();\n";
    cout << "    void executeQuery();\n";
    cout << "    void closeDatabase();\n";
    cout << "    \n";
    cout << "    // Email handling\n";
    cout << "    void sendEmail();\n";
    cout << "    void validateEmail();\n";
    cout << "    \n";
    cout << "    // Payment processing\n";
    cout << "    void processPayment();\n";
    cout << "    void refundPayment();\n";
    cout << "    \n";
    cout << "    // Logging\n";
    cout << "    void logError();\n";
    cout << "    void logInfo();\n";
    cout << "    \n";
    cout << "    // Configuration\n";
    cout << "    void loadConfig();\n";
    cout << "    void saveConfig();\n";
    cout << "    \n";
    cout << "    // ... 50+ more methods!\n";
    cout << "};\n";
    cout << "```\n";

    cout << "\n🚨 PROBLEMS:\n";
    cout << "   • One class doing EVERYTHING\n";
    cout << "   • 1000+ lines of code\n";
    cout << "   • Hard to test - tests everything\n";
    cout << "   • Hard to modify - affects everything\n";
    cout << "   • Multiple responsibilities\n";
    cout << "   • Impossible to reuse parts\n";

    // ✅ GOOD CODE: Focused, single-responsibility classes
    cout << "\n✅ GOOD CODE (Separated Responsibilities):\n";
    cout << "```cpp\n";
    cout << "class UserManager {\n";
    cout << "public:\n";
    cout << "    void createUser(const User& user);\n";
    cout << "    void deleteUser(int userId);\n";
    cout << "    void updateUser(const User& user);\n";
    cout << "};\n\n";
    cout << "class DatabaseConnection {\n";
    cout << "public:\n";
    cout << "    void connect(const string& connectionString);\n";
    cout << "    QueryResult execute(const string& query);\n";
    cout << "    void close();\n";
    cout << "};\n\n";
    cout << "class EmailService {\n";
    cout << "public:\n";
    cout << "    void send(const Email& email);\n";
    cout << "    bool validate(const string& emailAddress);\n";
    cout << "};\n\n";
    cout << "class PaymentProcessor {\n";
    cout << "public:\n";
    cout << "    bool process(const Payment& payment);\n";
    cout << "    bool refund(int transactionId);\n";
    cout << "};\n\n";
    cout << "class Logger {\n";
    cout << "public:\n";
    cout << "    void error(const string& message);\n";
    cout << "    void info(const string& message);\n";
    cout << "};\n\n";
    cout << "class ConfigurationManager {\n";
    cout << "public:\n";
    cout << "    void load(const string& path);\n";
    cout << "    void save(const string& path);\n";
    cout << "};\n";
    cout << "```\n";

    cout << "\n✨ IMPROVEMENTS:\n";
    cout << "   ✅ Each class has ONE responsibility\n";
    cout << "   ✅ Easy to test - test each separately\n";
    cout << "   ✅ Easy to modify - changes are isolated\n";
    cout << "   ✅ Reusable - use EmailService anywhere\n";
    cout << "   ✅ Clear purpose - obvious what each does\n";

    cout << "\n💡 KEY PRINCIPLE (Single Responsibility):\n";
    cout << "   \"A class should have only ONE reason to change!\"\n";
}

// ============================================
// EXAMPLE 5: DEEP NESTING (Complexity Smell)
// ============================================

void example5_deep_nesting() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 5: DEEP NESTING (Complexity Smell)    ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    // ❌ BAD CODE: Deeply nested conditions
    cout << "\n❌ BAD CODE (Arrow Anti-Pattern):\n";
    cout << "```cpp\n";
    cout << "void processOrder(Order order) {\n";
    cout << "    if (order.isValid()) {\n";
    cout << "        if (order.hasItems()) {\n";
    cout << "            if (order.customer.hasCredit()) {\n";
    cout << "                if (order.shippingAddress.isValid()) {\n";
    cout << "                    if (inventory.hasStock(order)) {\n";
    cout << "                        if (payment.process(order)) {\n";
    cout << "                            shipOrder(order);\n";
    cout << "                            sendConfirmation(order);\n";
    cout << "                        } else {\n";
    cout << "                            handlePaymentError();\n";
    cout << "                        }\n";
    cout << "                    } else {\n";
    cout << "                        notifyOutOfStock();\n";
    cout << "                    }\n";
    cout << "                } else {\n";
    cout << "                    requestValidAddress();\n";
    cout << "                }\n";
    cout << "            } else {\n";
    cout << "                rejectLowCredit();\n";
    cout << "            }\n";
    cout << "        } else {\n";
    cout << "            notifyEmptyOrder();\n";
    cout << "        }\n";
    cout << "    } else {\n";
    cout << "        rejectInvalidOrder();\n";
    cout << "    }\n";
    cout << "}\n";
    cout << "```\n";

    cout << "\n🚨 PROBLEMS:\n";
    cout << "   • 6 levels of nesting - hard to follow\n";
    cout << "   • 'Arrow' shape - code drifts to the right\n";
    cout << "   • Hard to read and understand\n";
    cout << "   • High cyclomatic complexity\n";
    cout << "   • Difficult to test all paths\n";

    // ✅ GOOD CODE: Guard clauses and early returns
    cout << "\n✅ GOOD CODE (Guard Clauses):\n";
    cout << "```cpp\n";
    cout << "void processOrder(Order order) {\n";
    cout << "    // Guard clauses - fail fast\n";
    cout << "    if (!order.isValid()) {\n";
    cout << "        rejectInvalidOrder();\n";
    cout << "        return;\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    if (!order.hasItems()) {\n";
    cout << "        notifyEmptyOrder();\n";
    cout << "        return;\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    if (!order.customer.hasCredit()) {\n";
    cout << "        rejectLowCredit();\n";
    cout << "        return;\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    if (!order.shippingAddress.isValid()) {\n";
    cout << "        requestValidAddress();\n";
    cout << "        return;\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    if (!inventory.hasStock(order)) {\n";
    cout << "        notifyOutOfStock();\n";
    cout << "        return;\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    if (!payment.process(order)) {\n";
    cout << "        handlePaymentError();\n";
    cout << "        return;\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    // Happy path - all validations passed\n";
    cout << "    shipOrder(order);\n";
    cout << "    sendConfirmation(order);\n";
    cout << "}\n";
    cout << "```\n";

    cout << "\n✨ IMPROVEMENTS:\n";
    cout << "   ✅ Flat structure - no deep nesting\n";
    cout << "   ✅ Easy to read - linear flow\n";
    cout << "   ✅ Clear error handling\n";
    cout << "   ✅ Happy path at the end\n";
    cout << "   ✅ Low complexity\n";

    cout << "\n💡 KEY PRINCIPLE:\n";
    cout << "   \"Fail fast with guard clauses - avoid the arrow!\"\n";
}

// ============================================
// EXAMPLE 6: CODE DUPLICATION (Dispensable Smell)
// ============================================

void example6_code_duplication() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 6: CODE DUPLICATION (DRY Violation)   ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    // ❌ BAD CODE: Copy-pasted code
    cout << "\n❌ BAD CODE (Repeated Logic):\n";
    cout << "```cpp\n";
    cout << "double calculateCircleArea(double radius) {\n";
    cout << "    return 3.14159 * radius * radius;\n";
    cout << "}\n\n";
    cout << "double calculateCircleCircumference(double radius) {\n";
    cout << "    return 2 * 3.14159 * radius;  // PI duplicated!\n";
    cout << "}\n\n";
    cout << "double calculateSphereVolume(double radius) {\n";
    cout << "    return (4.0/3.0) * 3.14159 * radius * radius * radius;  // PI again!\n";
    cout << "}\n\n";
    cout << "void processStudentGrade(Student s) {\n";
    cout << "    if (s.name.empty()) {\n";
    cout << "        cout << \"Error: Name is required\" << endl;\n";
    cout << "        return;\n";
    cout << "    }\n";
    cout << "    if (s.id <= 0) {\n";
    cout << "        cout << \"Error: Invalid ID\" << endl;\n";
    cout << "        return;\n";
    cout << "    }\n";
    cout << "    // Process student...\n";
    cout << "}\n\n";
    cout << "void processTeacherData(Teacher t) {\n";
    cout << "    if (t.name.empty()) {  // Same validation!\n";
    cout << "        cout << \"Error: Name is required\" << endl;\n";
    cout << "        return;\n";
    cout << "    }\n";
    cout << "    if (t.id <= 0) {  // Same validation!\n";
    cout << "        cout << \"Error: Invalid ID\" << endl;\n";
    cout << "        return;\n";
    cout << "    }\n";
    cout << "    // Process teacher...\n";
    cout << "}\n";
    cout << "```\n";

    cout << "\n🚨 PROBLEMS:\n";
    cout << "   • PI value repeated 3 times (what if it's wrong?)\n";
    cout << "   • Validation logic duplicated\n";
    cout << "   • Bug fixes needed in multiple places\n";
    cout << "   • Maintenance nightmare\n";
    cout << "   • Inconsistency risk\n";

    // ✅ GOOD CODE: DRY - Don't Repeat Yourself
    cout << "\n✅ GOOD CODE (Extract Common Logic):\n";
    cout << "```cpp\n";
    cout << "// Single definition of PI\n";
    cout << "const double PI = 3.14159265358979323846;\n\n";
    cout << "double calculateCircleArea(double radius) {\n";
    cout << "    return PI * radius * radius;\n";
    cout << "}\n\n";
    cout << "double calculateCircleCircumference(double radius) {\n";
    cout << "    return 2 * PI * radius;\n";
    cout << "}\n\n";
    cout << "double calculateSphereVolume(double radius) {\n";
    cout << "    return (4.0/3.0) * PI * radius * radius * radius;\n";
    cout << "}\n\n";
    cout << "// Base class with common validation\n";
    cout << "class Person {\n";
    cout << "protected:\n";
    cout << "    bool validateBasicInfo() const {\n";
    cout << "        if (name.empty()) {\n";
    cout << "            cout << \"Error: Name is required\" << endl;\n";
    cout << "            return false;\n";
    cout << "        }\n";
    cout << "        if (id <= 0) {\n";
    cout << "            cout << \"Error: Invalid ID\" << endl;\n";
    cout << "            return false;\n";
    cout << "        }\n";
    cout << "        return true;\n";
    cout << "    }\n";
    cout << "    string name;\n";
    cout << "    int id;\n";
    cout << "};\n\n";
    cout << "class Student : public Person {\n";
    cout << "    void process() {\n";
    cout << "        if (!validateBasicInfo()) return;\n";
    cout << "        // Process student...\n";
    cout << "    }\n";
    cout << "};\n\n";
    cout << "class Teacher : public Person {\n";
    cout << "    void process() {\n";
    cout << "        if (!validateBasicInfo()) return;\n";
    cout << "        // Process teacher...\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "```\n";

    cout << "\n✨ IMPROVEMENTS:\n";
    cout << "   ✅ Single source of truth (PI defined once)\n";
    cout << "   ✅ Validation logic in ONE place\n";
    cout << "   ✅ Bug fix needed only once\n";
    cout << "   ✅ Consistent behavior\n";
    cout << "   ✅ Easy to maintain\n";

    cout << "\n💡 KEY PRINCIPLE (DRY):\n";
    cout << "   \"Don't Repeat Yourself - every piece of knowledge\n";
    cout << "    should have a single representation!\"\n";
}

// ============================================
// EXAMPLE 7: POOR ERROR HANDLING
// ============================================

void example7_poor_error_handling() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 7: POOR ERROR HANDLING                ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    // ❌ BAD CODE: Ignoring errors
    cout << "\n❌ BAD CODE (Ignoring Errors):\n";
    cout << "```cpp\n";
    cout << "void readFile(const string& filename) {\n";
    cout << "    ifstream file(filename);\n";
    cout << "    // No check if file opened!\n";
    cout << "    \n";
    cout << "    string line;\n";
    cout << "    getline(file, line);  // Might fail!\n";
    cout << "    processLine(line);\n";
    cout << "}\n\n";
    cout << "int divide(int a, int b) {\n";
    cout << "    return a / b;  // Division by zero!\n";
    cout << "}\n\n";
    cout << "void allocateMemory() {\n";
    cout << "    int* ptr = new int[1000000000];  // Might fail!\n";
    cout << "    // Use ptr...\n";
    cout << "    delete[] ptr;\n";
    cout << "}\n";
    cout << "```\n";

    cout << "\n🚨 PROBLEMS:\n";
    cout << "   • No file opening check - crash!\n";
    cout << "   • Division by zero - undefined behavior!\n";
    cout << "   • No allocation failure check\n";
    cout << "   • Silent failures\n";
    cout << "   • Unpredictable behavior\n";

    // ✅ GOOD CODE: Proper error handling
    cout << "\n✅ GOOD CODE (Proper Error Handling):\n";
    cout << "```cpp\n";
    cout << "void readFile(const string& filename) {\n";
    cout << "    ifstream file(filename);\n";
    cout << "    if (!file.is_open()) {\n";
    cout << "        throw runtime_error(\"Cannot open file: \" + filename);\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    string line;\n";
    cout << "    if (!getline(file, line)) {\n";
    cout << "        throw runtime_error(\"Failed to read from file\");\n";
    cout << "    }\n";
    cout << "    processLine(line);\n";
    cout << "}\n\n";
    cout << "int divide(int a, int b) {\n";
    cout << "    if (b == 0) {\n";
    cout << "        throw invalid_argument(\"Division by zero\");\n";
    cout << "    }\n";
    cout << "    return a / b;\n";
    cout << "}\n\n";
    cout << "void allocateMemory() {\n";
    cout << "    // Use smart pointer - automatic cleanup\n";
    cout << "    try {\n";
    cout << "        auto ptr = make_unique<int[]>(1000000000);\n";
    cout << "        // Use ptr...\n";
    cout << "    } catch (const bad_alloc& e) {\n";
    cout << "        cerr << \"Memory allocation failed: \" << e.what() << endl;\n";
    cout << "        throw;\n";
    cout << "    }\n";
    cout << "}\n";
    cout << "```\n";

    cout << "\n✨ IMPROVEMENTS:\n";
    cout << "   ✅ Check all operations that can fail\n";
    cout << "   ✅ Throw exceptions with clear messages\n";
    cout << "   ✅ Use RAII (smart pointers) for safety\n";
    cout << "   ✅ Handle errors gracefully\n";
    cout << "   ✅ Predictable behavior\n";

    cout << "\n💡 KEY PRINCIPLE:\n";
    cout << "   \"Always check operations that can fail -\n";
    cout << "    handle errors gracefully!\"\n";
}

// ============================================
// EXAMPLE 8: COMMENTS AS CRUTCH
// ============================================

void example8_comments_as_crutch() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 8: COMMENTS AS CRUTCH (Code Smell)    ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    // ❌ BAD CODE: Need comments to understand
    cout << "\n❌ BAD CODE (Needs Comments):\n";
    cout << "```cpp\n";
    cout << "// Calculate total\n";
    cout << "double calc(vector<int>& v, bool f) {\n";
    cout << "    double t = 0;  // total\n";
    cout << "    for (auto i : v) {  // loop through items\n";
    cout << "        if (f) {  // if flag is true\n";
    cout << "            t += i * 1.19;  // add with tax\n";
    cout << "        } else {\n";
    cout << "            t += i;  // add without tax\n";
    cout << "        }\n";
    cout << "    }\n";
    cout << "    return t;\n";
    cout << "}\n\n";
    cout << "// Check user\n";
    cout << "bool chk(User u) {\n";
    cout << "    // Check if name is empty\n";
    cout << "    if (u.n.empty()) return false;\n";
    cout << "    // Check if age is valid\n";
    cout << "    if (u.a < 18) return false;\n";
    cout << "    // Check if email is valid\n";
    cout << "    if (u.e.find('@') == string::npos) return false;\n";
    cout << "    return true;\n";
    cout << "}\n";
    cout << "```\n";

    cout << "\n🚨 PROBLEMS:\n";
    cout << "   • Comments explain WHAT, not WHY\n";
    cout << "   • Comments compensate for bad names\n";
    cout << "   • Code is not self-documenting\n";
    cout << "   • Comments can become outdated\n";
    cout << "   • Extra maintenance burden\n";

    // ✅ GOOD CODE: Self-documenting
    cout << "\n✅ GOOD CODE (Self-Documenting):\n";
    cout << "```cpp\n";
    cout << "const double TAX_RATE = 1.19;  // 19% VAT\n\n";
    cout << "double calculateTotal(const vector<int>& prices, bool includeTax) {\n";
    cout << "    double total = 0.0;\n";
    cout << "    \n";
    cout << "    for (int price : prices) {\n";
    cout << "        if (includeTax) {\n";
    cout << "            total += price * TAX_RATE;\n";
    cout << "        } else {\n";
    cout << "            total += price;\n";
    cout << "        }\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    return total;\n";
    cout << "}\n\n";
    cout << "const int MINIMUM_AGE = 18;\n\n";
    cout << "bool isValidUser(const User& user) {\n";
    cout << "    bool hasName = !user.name.empty();\n";
    cout << "    bool isAdult = user.age >= MINIMUM_AGE;\n";
    cout << "    bool hasValidEmail = user.email.find('@') != string::npos;\n";
    cout << "    \n";
    cout << "    return hasName && isAdult && hasValidEmail;\n";
    cout << "}\n";
    cout << "```\n";

    cout << "\n✨ IMPROVEMENTS:\n";
    cout << "   ✅ Clear function names - no comment needed\n";
    cout << "   ✅ Descriptive variable names\n";
    cout << "   ✅ Self-explanatory logic\n";
    cout << "   ✅ Comments only for WHY, not WHAT\n";
    cout << "   ✅ Code tells the story\n";

    cout << "\n💡 KEY PRINCIPLE:\n";
    cout << "   \"Good code is self-documenting - comments should\n";
    cout << "    explain WHY, not WHAT!\"\n";
}

// ============================================
// EXAMPLE 9: PREMATURE OPTIMIZATION
// ============================================

void example9_premature_optimization() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 9: PREMATURE OPTIMIZATION (Anti-Pattern) ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    // ❌ BAD CODE: Overly optimized, hard to read
    cout << "\n❌ BAD CODE (Premature Optimization):\n";
    cout << "```cpp\n";
    cout << "// Trying to be 'fast' but sacrificing readability\n";
    cout << "int calc(int* a, int n) {\n";
    cout << "    int r = 0;\n";
    cout << "    for (int i = 0; i < n; ++i) {\n";
    cout << "        r += (a[i] << 1) - (a[i] >> 1);  // What does this do??\n";
    cout << "    }\n";
    cout << "    return r;\n";
    cout << "}\n\n";
    cout << "// Manual loop unrolling for 'speed'\n";
    cout << "void proc(vector<int>& v) {\n";
    cout << "    int n = v.size();\n";
    cout << "    int i = 0;\n";
    cout << "    // Process 4 at a time\n";
    cout << "    for (; i + 4 <= n; i += 4) {\n";
    cout << "        v[i] *= 2;\n";
    cout << "        v[i+1] *= 2;\n";
    cout << "        v[i+2] *= 2;\n";
    cout << "        v[i+3] *= 2;\n";
    cout << "    }\n";
    cout << "    // Handle remainder\n";
    cout << "    for (; i < n; ++i) {\n";
    cout << "        v[i] *= 2;\n";
    cout << "    }\n";
    cout << "}\n";
    cout << "```\n";

    cout << "\n🚨 PROBLEMS:\n";
    cout << "   • Bit shifting makes code cryptic\n";
    cout << "   • Manual loop unrolling is complex\n";
    cout << "   • Hard to maintain\n";
    cout << "   • Compiler can optimize better!\n";
    cout << "   • Premature - did you measure?\n";

    // ✅ GOOD CODE: Clear first, optimize later
    cout << "\n✅ GOOD CODE (Clear and Simple):\n";
    cout << "```cpp\n";
    cout << "int calculateSum(const vector<int>& numbers) {\n";
    cout << "    int sum = 0;\n";
    cout << "    for (int number : numbers) {\n";
    cout << "        // 2*x - x/2 = 1.5*x\n";
    cout << "        sum += static_cast<int>(number * 1.5);\n";
    cout << "    }\n";
    cout << "    return sum;\n";
    cout << "}\n\n";
    cout << "void doubleAllValues(vector<int>& numbers) {\n";
    cout << "    // Simple and clear - compiler will optimize\n";
    cout << "    for (int& number : numbers) {\n";
    cout << "        number *= 2;\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    // Or use STL algorithm\n";
    cout << "    // transform(numbers.begin(), numbers.end(), numbers.begin(),\n";
    cout << "    //          [](int n) { return n * 2; });\n";
    cout << "}\n";
    cout << "```\n";

    cout << "\n✨ IMPROVEMENTS:\n";
    cout << "   ✅ Clear, readable code\n";
    cout << "   ✅ Easy to understand intent\n";
    cout << "   ✅ Easy to maintain\n";
    cout << "   ✅ Compiler optimizes well\n";
    cout << "   ✅ Profile before optimizing!\n";

    cout << "\n💡 KEY PRINCIPLE (Donald Knuth):\n";
    cout << "   \"Premature optimization is the root of all evil.\n";
    cout << "    Write clear code first, optimize bottlenecks later!\"\n";
}

// ============================================
// EXAMPLE 10: GLOBAL STATE & COUPLING
// ============================================

void example10_global_state() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 10: GLOBAL STATE (Coupling Smell)     ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    // ❌ BAD CODE: Global variables everywhere
    cout << "\n❌ BAD CODE (Global Mutable State):\n";
    cout << "```cpp\n";
    cout << "// Global variables (bad!)\n";
    cout << "int userCount = 0;\n";
    cout << "string currentUser = \"\";\n";
    cout << "bool isLoggedIn = false;\n";
    cout << "vector<string> errors;\n\n";
    cout << "void loginUser(string name) {\n";
    cout << "    currentUser = name;  // Modifies global\n";
    cout << "    isLoggedIn = true;   // Modifies global\n";
    cout << "    userCount++;         // Modifies global\n";
    cout << "}\n\n";
    cout << "void processData() {\n";
    cout << "    if (isLoggedIn) {  // Depends on global\n";
    cout << "        // Process for currentUser...\n";
    cout << "    } else {\n";
    cout << "        errors.push_back(\"Not logged in\");  // Modifies global\n";
    cout << "    }\n";
    cout << "}\n\n";
    cout << "void showErrors() {\n";
    cout << "    for (const auto& err : errors) {  // Depends on global\n";
    cout << "        cout << err << endl;\n";
    cout << "    }\n";
    cout << "}\n";
    cout << "```\n";

    cout << "\n🚨 PROBLEMS:\n";
    cout << "   • Hidden dependencies - hard to track\n";
    cout << "   • Hard to test - state persists\n";
    cout << "   • Threading issues - race conditions\n";
    cout << "   • Tight coupling - everything connected\n";
    cout << "   • Side effects everywhere\n";

    // ✅ GOOD CODE: Explicit dependencies, local state
    cout << "\n✅ GOOD CODE (Encapsulated State):\n";
    cout << "```cpp\n";
    cout << "class UserSession {\n";
    cout << "private:\n";
    cout << "    string currentUser;\n";
    cout << "    bool isLoggedIn;\n";
    cout << "    vector<string> errors;\n";
    cout << "    \n";
    cout << "public:\n";
    cout << "    UserSession() : isLoggedIn(false) {}\n";
    cout << "    \n";
    cout << "    void login(const string& name) {\n";
    cout << "        currentUser = name;\n";
    cout << "        isLoggedIn = true;\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    bool isActive() const { return isLoggedIn; }\n";
    cout << "    string getUser() const { return currentUser; }\n";
    cout << "    \n";
    cout << "    void addError(const string& error) {\n";
    cout << "        errors.push_back(error);\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    vector<string> getErrors() const { return errors; }\n";
    cout << "};\n\n";
    cout << "class DataProcessor {\n";
    cout << "public:\n";
    cout << "    void process(UserSession& session) {  // Explicit dependency\n";
    cout << "        if (session.isActive()) {\n";
    cout << "            // Process for session.getUser()...\n";
    cout << "        } else {\n";
    cout << "            session.addError(\"Not logged in\");\n";
    cout << "        }\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "```\n";

    cout << "\n✨ IMPROVEMENTS:\n";
    cout << "   ✅ Encapsulated state - no globals\n";
    cout << "   ✅ Explicit dependencies - clear what's needed\n";
    cout << "   ✅ Easy to test - create session, test\n";
    cout << "   ✅ Thread-safe - each session separate\n";
    cout << "   ✅ Low coupling - classes independent\n";

    cout << "\n💡 KEY PRINCIPLE:\n";
    cout << "   \"Avoid global mutable state - pass dependencies explicitly!\"\n";
}

// ============================================
// EXAMPLE 11: CODE QUALITY METRICS
// ============================================

void example11_quality_metrics() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 11: CODE QUALITY METRICS              ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n📊 KEY QUALITY METRICS:\n";

    cout << "\n1. CYCLOMATIC COMPLEXITY:\n";
    cout << "   • Measures number of independent paths\n";
    cout << "   • Formula: E - N + 2P (edges - nodes + 2*connected components)\n";
    cout << "   • Or: Count decision points + 1\n";
    cout << "   • ✅ Good: < 10\n";
    cout << "   • ⚠️  Warning: 10-20\n";
    cout << "   • ❌ Bad: > 20\n";

    cout << "\n   Example:\n";
    cout << "   void simple() { }                    // Complexity: 1\n";
    cout << "   void withIf() { if(...) {...} }      // Complexity: 2\n";
    cout << "   void complex() {\n";
    cout << "       if(...) {...}                    // +1\n";
    cout << "       else if(...) {...}               // +1\n";
    cout << "       for(...) {...}                   // +1\n";
    cout << "       while(...) {...}                 // +1\n";
    cout << "   }                                    // Complexity: 5\n";

    cout << "\n2. LINES OF CODE (LOC):\n";
    cout << "   • Functions: ✅ < 50 lines (ideally 10-20)\n";
    cout << "   • Classes: ✅ < 500 lines\n";
    cout << "   • Files: ✅ < 1000 lines\n";
    cout << "   • If longer → split into smaller pieces\n";

    cout << "\n3. DEPTH OF INHERITANCE:\n";
    cout << "   • ✅ Good: ≤ 3 levels\n";
    cout << "   • ❌ Bad: > 5 levels\n";
    cout << "   • Deep hierarchies are hard to understand\n";

    cout << "\n4. COUPLING:\n";
    cout << "   • Afferent Coupling (Ca): Who depends on this?\n";
    cout << "   • Efferent Coupling (Ce): What does this depend on?\n";
    cout << "   • ✅ Goal: Low coupling (independent modules)\n";

    cout << "\n5. COHESION:\n";
    cout << "   • How related are methods in a class?\n";
    cout << "   • ✅ High cohesion: Methods work together\n";
    cout << "   • ❌ Low cohesion: Unrelated methods\n";

    cout << "\n6. CODE COVERAGE:\n";
    cout << "   • ✅ Good: > 80%\n";
    cout << "   • ⭐ Excellent: > 90%\n";
    cout << "   • But: 100% doesn't guarantee quality!\n";

    cout << "\n7. TECHNICAL DEBT RATIO:\n";
    cout << "   • Remediation cost / Development cost\n";
    cout << "   • ✅ Good: < 5%\n";
    cout << "   • ⚠️  Warning: 5-10%\n";
    cout << "   • ❌ Bad: > 10%\n";

    cout << "\n8. CODE DUPLICATION:\n";
    cout << "   • ✅ Good: < 3%\n";
    cout << "   • ⚠️  Warning: 3-5%\n";
    cout << "   • ❌ Bad: > 5%\n";

    cout << "\n💡 REMEMBER:\n";
    cout << "   \"Metrics are indicators, not goals!\n";
    cout << "    Use them to identify problems, not as targets.\"\n";
}

// ============================================
// EXAMPLE 12: CODE QUALITY CHECKLIST
// ============================================

void example12_quality_checklist() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 12: CODE QUALITY CHECKLIST            ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n✅ CODE QUALITY CHECKLIST:\n";

    cout << "\n📝 READABILITY:\n";
    cout << "   □ Clear, descriptive names (no x, tmp, data)\n";
    cout << "   □ Functions < 50 lines (ideally 10-20)\n";
    cout << "   □ Max 3 levels of nesting\n";
    cout << "   □ Self-documenting code\n";
    cout << "   □ Consistent formatting\n";
    cout << "   □ Comments explain WHY, not WHAT\n";

    cout << "\n🏗️  DESIGN:\n";
    cout << "   □ Single Responsibility Principle\n";
    cout << "   □ No god objects (< 500 LOC)\n";
    cout << "   □ Low coupling between modules\n";
    cout << "   □ High cohesion within modules\n";
    cout << "   □ Clear interfaces\n";
    cout << "   □ Proper abstraction levels\n";

    cout << "\n🔒 RELIABILITY:\n";
    cout << "   □ All errors handled\n";
    cout << "   □ Input validation\n";
    cout << "   □ No memory leaks (use RAII)\n";
    cout << "   □ Thread-safe (if multithreaded)\n";
    cout << "   □ Proper resource cleanup\n";
    cout << "   □ Exception-safe code\n";

    cout << "\n🎯 BEST PRACTICES:\n";
    cout << "   □ Const correctness\n";
    cout << "   □ Smart pointers (no raw new/delete)\n";
    cout << "   □ Modern C++ features (auto, range-for)\n";
    cout << "   □ nullptr (not NULL)\n";
    cout << "   □ enum class (not plain enum)\n";
    cout << "   □ Pass by const reference for large objects\n";

    cout << "\n🧹 CLEANLINESS:\n";
    cout << "   □ No magic numbers (use named constants)\n";
    cout << "   □ No code duplication (DRY)\n";
    cout << "   □ No dead code\n";
    cout << "   □ No commented-out code\n";
    cout << "   □ No global variables\n";
    cout << "   □ No deep nesting (use guard clauses)\n";

    cout << "\n🧪 TESTABILITY:\n";
    cout << "   □ Functions are testable\n";
    cout << "   □ Dependencies injected (not hardcoded)\n";
    cout << "   □ Side effects minimized\n";
    cout << "   □ Pure functions where possible\n";
    cout << "   □ Mock-friendly design\n";

    cout << "\n⚡ PERFORMANCE:\n";
    cout << "   □ No premature optimization\n";
    cout << "   □ Move semantics where appropriate\n";
    cout << "   □ Pass by const reference (large objects)\n";
    cout << "   □ Reserve vector capacity if known\n";
    cout << "   □ Avoid unnecessary copies\n";

    cout << "\n📚 DOCUMENTATION:\n";
    cout << "   □ Public API documented\n";
    cout << "   □ Complex algorithms explained\n";
    cout << "   □ Design decisions documented\n";
    cout << "   □ README for project/module\n";
    cout << "   □ Examples provided\n";

    cout << "\n🔧 TOOLS:\n";
    cout << "   □ Use linter (clang-tidy)\n";
    cout << "   □ Use formatter (clang-format)\n";
    cout << "   □ Static analysis\n";
    cout << "   □ Code coverage\n";
    cout << "   □ Sanitizers (Address, UB)\n";

    cout << "\n💡 GOLDEN RULES:\n";
    cout << "   1. Code is read 10x more than written\n";
    cout << "   2. Simple is better than clever\n";
    cout << "   3. Explicit is better than implicit\n";
    cout << "   4. Fail fast, fail clearly\n";
    cout << "   5. Don't repeat yourself (DRY)\n";
    cout << "   6. You aren't gonna need it (YAGNI)\n";
    cout << "   7. Keep it simple, stupid (KISS)\n";
}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║         CODE QUALITY BASICS - MASTERCLASS             ║\n";
    cout << "║                                                       ║\n";
    cout << "║    Understanding Quality Code & Code Smells!          ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n";

    example1_magic_numbers();
    example2_long_functions();
    example3_unclear_naming();
    example4_god_object();
    example5_deep_nesting();
    example6_code_duplication();
    example7_poor_error_handling();
    example8_comments_as_crutch();
    example9_premature_optimization();
    example10_global_state();
    example11_quality_metrics();
    example12_quality_checklist();

    cout << "\n\n";
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║         🎉 CODE QUALITY BASICS COMPLETE! 🎉           ║\n";
    cout << "║                                                       ║\n";
    cout << "║  You've learned what makes quality code and how       ║\n";
    cout << "║  to identify and fix common code smells!              ║\n";
    cout << "║                                                       ║\n";
    cout << "║  Next: Dive deeper into specific quality topics!     ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n\n";

    return 0;
}
