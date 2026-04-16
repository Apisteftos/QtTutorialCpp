#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <fstream>
using namespace std;

// ============================================
// HIGH COHESION PRINCIPLE
// ============================================

/*
HIGH COHESION PRINCIPLE:
    "A class should have high cohesion - all its methods and data
     should be closely related and work toward a single purpose."
    — Software Engineering Principle

WHAT IT MEANS:
    • Group related functionality together
    • Each class/module has a single, well-defined purpose
    • Methods in a class work with the same data
    • Changes to functionality affect fewer classes
    • Strong relationship between class members

WHY IT MATTERS:
    ✅ Easier to understand (clear purpose)
    ✅ Easier to maintain (changes localized)
    ✅ More reusable (focused functionality)
    ✅ Easier to test (single responsibility)
    ✅ Better organization (logical grouping)

KEY CONCEPTS:
    • Cohesion = Degree of relatedness within a module
    • High cohesion = Strong relationship between members
    • Low cohesion = Weak relationship, unrelated members
    • Functional cohesion = Best type (single purpose)
    • Related to Single Responsibility Principle (SRP)

COHESION LEVELS (Best to Worst):
    1. Functional - Single, well-defined purpose (Best!)
    2. Sequential - Output of one is input of another
    3. Communicational - Operate on same data
    4. Procedural - Sequential execution
    5. Temporal - Executed at same time
    6. Logical - Similar but different operations
    7. Coincidental - No meaningful relationship (Worst!)

VIOLATION SIGNS:
    🚨 Class does many unrelated things
    🚨 Methods don't use class data
    🚨 Half the class unused in each operation
    🚨 Many dependencies between classes
    🚨 Hard to name the class/module
    🚨 "UtilityClass" or "Helper" in name
*/

// ============================================
// EXAMPLE 1: LOW COHESION (God Class)
// ============================================

namespace bad_example1 {
// ❌ BAD: Low cohesion - class does everything!

class UserManager {
private:
    string username;
    string email;
    vector<string> orders;

public:
    // User-related operations
    void setUserName(const string& name) { username = name; }
    void setEmail(const string& email_) { email = email_; }
    string getUserName() const { return username; }

    // 🚨 Unrelated: Database operations
    void connectToDatabase() {
        cout << "   🔌 Connecting to database..." << endl;
    }

    void saveToDatabase() {
        cout << "   💾 Saving user to database..." << endl;
    }

    // 🚨 Unrelated: Email operations
    void sendWelcomeEmail() {
        cout << "   📧 Sending welcome email to: " << email << endl;
    }

    void sendPasswordResetEmail() {
        cout << "   📧 Sending password reset email..." << endl;
    }

    // 🚨 Unrelated: Logging operations
    void logActivity(const string& activity) {
        cout << "   📝 Logging: " << activity << endl;
    }

    void logError(const string& error) {
        cout << "   ❌ Error: " << error << endl;
    }

    // 🚨 Unrelated: Validation
    bool validateEmail(const string& email_) {
        return email_.find("@") != string::npos;
    }

    bool validatePassword(const string& password) {
        return password.length() >= 8;
    }

    // 🚨 Unrelated: Order operations
    void addOrder(const string& order) {
        orders.push_back(order);
    }

    vector<string> getOrders() const {
        return orders;
    }
};

// PROBLEMS:
// • One class doing: user data, database, email, logging, validation, orders
// • Very low cohesion - unrelated responsibilities
// • Hard to maintain and test
// • Changes to email affect user management
// • Difficult to reuse individual pieces
}

namespace good_example1 {
// ✅ GOOD: High cohesion - separate focused classes

// User data only (high cohesion)
class User {
private:
    string username;
    string email;

public:
    User(const string& name, const string& email_)
        : username(name), email(email_) {}

    string getUserName() const { return username; }
    string getEmail() const { return email; }
    void setUserName(const string& name) { username = name; }
    void setEmail(const string& email_) { email = email_; }
};

// Database operations only (high cohesion)
class UserRepository {
public:
    void save(const User& user) {
        cout << "   💾 Saving user to database: " << user.getUserName() << endl;
    }

    User* load(const string& username) {
        cout << "   📂 Loading user from database: " << username << endl;
        return new User(username, "user@example.com");
    }
};

// Email operations only (high cohesion)
class EmailService {
public:
    void sendWelcomeEmail(const User& user) {
        cout << "   📧 Sending welcome email to: " << user.getEmail() << endl;
    }

    void sendPasswordResetEmail(const User& user) {
        cout << "   📧 Sending password reset to: " << user.getEmail() << endl;
    }
};

// Validation only (high cohesion)
class Validator {
public:
    bool isValidEmail(const string& email) {
        return email.find("@") != string::npos;
    }

    bool isValidPassword(const string& password) {
        return password.length() >= 8;
    }
};

// Logging only (high cohesion)
class Logger {
public:
    void logActivity(const string& activity) {
        cout << "   📝 " << activity << endl;
    }

    void logError(const string& error) {
        cout << "   ❌ " << error << endl;
    }
};
}

void example1_god_class() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 1: LOW COHESION (God Class)           ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (Low Cohesion - God Class):\n";
    cout << "```cpp\n";
    cout << "class UserManager {\n";
    cout << "    // User data\n";
    cout << "    // Database operations 🚨\n";
    cout << "    // Email operations 🚨\n";
    cout << "    // Logging operations 🚨\n";
    cout << "    // Validation operations 🚨\n";
    cout << "    // Order operations 🚨\n";
    cout << "};\n";
    cout << "// One class doing everything!\n";
    cout << "```\n";

    cout << "\n🚨 PROBLEMS:\n";
    cout << "   • Very low cohesion - unrelated responsibilities\n";
    cout << "   • Changes to email affect user management\n";
    cout << "   • Hard to test individual pieces\n";
    cout << "   • Cannot reuse email logic elsewhere\n";
    cout << "   • Difficult to maintain\n";

    cout << "\n✅ GOOD CODE (High Cohesion - Focused Classes):\n";
    cout << "```cpp\n";
    cout << "class User { /* user data only */ };\n";
    cout << "class UserRepository { /* database only */ };\n";
    cout << "class EmailService { /* email only */ };\n";
    cout << "class Validator { /* validation only */ };\n";
    cout << "class Logger { /* logging only */ };\n";
    cout << "// Each class has single, clear purpose!\n";
    cout << "```\n";

    cout << "\n✨ DEMONSTRATION:\n";

    good_example1::User user("Alice", "alice@example.com");
    good_example1::UserRepository repo;
    good_example1::EmailService emailService;
    good_example1::Validator validator;
    good_example1::Logger logger;

    cout << "\n   Creating and saving user:\n";
    repo.save(user);
    logger.logActivity("User created: " + user.getUserName());

    cout << "\n   Sending welcome email:\n";
    if (validator.isValidEmail(user.getEmail())) {
        emailService.sendWelcomeEmail(user);
    }

    cout << "\n✨ BENEFITS:\n";
    cout << "   • Each class has clear, single purpose\n";
    cout << "   • EmailService reusable anywhere\n";
    cout << "   • Easy to test each component\n";
    cout << "   • Changes localized to specific class\n";
    cout << "   • Clear organization\n";
}

// ============================================
// EXAMPLE 2: UTILITY CLASS (Low Cohesion)
// ============================================

namespace bad_example2 {
// ❌ BAD: Low cohesion - random utility methods

class Utils {
public:
    // 🚨 String operation
    static string toUpperCase(const string& str) {
        string result = str;
        for (char& c : result) c = toupper(c);
        return result;
    }

    // 🚨 Math operation
    static int factorial(int n) {
        return n <= 1 ? 1 : n * factorial(n - 1);
    }

    // 🚨 Date operation
    static string getCurrentDate() {
        return "2024-01-01";
    }

    // 🚨 File operation
    static bool fileExists(const string& filename) {
        ifstream file(filename);
        return file.good();
    }

    // 🚨 Network operation
    static bool isValidIP(const string& ip) {
        return ip.find(".") != string::npos;
    }

    // 🚨 Completely unrelated operations in one class!
};

// PROBLEM: No cohesion - random unrelated utilities
}

namespace good_example2 {
// ✅ GOOD: High cohesion - grouped by purpose

class StringUtils {
public:
    static string toUpperCase(const string& str) {
        string result = str;
        for (char& c : result) c = toupper(c);
        return result;
    }

    static string toLowerCase(const string& str) {
        string result = str;
        for (char& c : result) c = tolower(c);
        return result;
    }

    static string trim(const string& str) {
        // Trim implementation
        return str;
    }
};

class MathUtils {
public:
    static int factorial(int n) {
        return n <= 1 ? 1 : n * factorial(n - 1);
    }

    static int fibonacci(int n) {
        return n <= 1 ? n : fibonacci(n-1) + fibonacci(n-2);
    }

    static bool isPrime(int n) {
        if (n <= 1) return false;
        for (int i = 2; i * i <= n; i++) {
            if (n % i == 0) return false;
        }
        return true;
    }
};

class FileUtils {
public:
    static bool exists(const string& filename) {
        ifstream file(filename);
        return file.good();
    }

    static string readFile(const string& filename) {
        // Read implementation
        return "";
    }

    static void writeFile(const string& filename, const string& content) {
        // Write implementation
    }
};

class NetworkUtils {
public:
    static bool isValidIP(const string& ip) {
        return ip.find(".") != string::npos;
    }

    static bool isValidURL(const string& url) {
        return url.find("http") == 0;
    }
};
}

void example2_utility_class() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 2: UTILITY CLASS (Low Cohesion)       ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE:\n";
    cout << "   class Utils {\n";
    cout << "       toUpperCase()    // String\n";
    cout << "       factorial()      // Math\n";
    cout << "       getCurrentDate() // Date\n";
    cout << "       fileExists()     // File\n";
    cout << "       isValidIP()      // Network\n";
    cout << "   };\n";
    cout << "   🚨 Completely unrelated operations!\n";

    cout << "\n✅ GOOD CODE:\n";
    cout << "   class StringUtils { /* string operations */ };\n";
    cout << "   class MathUtils { /* math operations */ };\n";
    cout << "   class FileUtils { /* file operations */ };\n";
    cout << "   class NetworkUtils { /* network operations */ };\n";
    cout << "   ✅ Each class groups related operations!\n";

    cout << "\n✨ DEMONSTRATION:\n";

    cout << "\n   String operations:\n";
    cout << "   HELLO -> " << good_example2::StringUtils::toUpperCase("hello") << endl;

    cout << "\n   Math operations:\n";
    cout << "   5! = " << good_example2::MathUtils::factorial(5) << endl;
    cout << "   Is 7 prime? " << (good_example2::MathUtils::isPrime(7) ? "Yes" : "No") << endl;

    cout << "\n✨ BENEFITS:\n";
    cout << "   • Clear organization by purpose\n";
    cout << "   • Easy to find related operations\n";
    cout << "   • Can use StringUtils without pulling in network code\n";
    cout << "   • Each class has high cohesion\n";
}

// ============================================
// EXAMPLE 3: DATA AND OPERATIONS TOGETHER
// ============================================

namespace bad_example3 {
// ❌ BAD: Data in one place, operations elsewhere (low cohesion)

struct OrderData {
    int orderId;
    double amount;
    string status;
};

class OrderOperations {
public:
    // 🚨 Operations separated from data
    double calculateTotal(const OrderData& order, double taxRate) {
        return order.amount * (1 + taxRate);
    }

    void updateStatus(OrderData& order, const string& newStatus) {
        order.status = newStatus;
    }

    bool isValid(const OrderData& order) {
        return order.amount > 0;
    }

    void print(const OrderData& order) {
        cout << "   Order #" << order.orderId
             << ": $" << order.amount
             << " - " << order.status << endl;
    }
};

// PROBLEM: Data and operations separated (low cohesion)
}

namespace good_example3 {
// ✅ GOOD: Data and operations together (high cohesion)

class Order {
private:
    int orderId;
    double amount;
    string status;

public:
    Order(int id, double amt)
        : orderId(id), amount(amt), status("pending") {}

    // Operations work with the data in this class
    double calculateTotal(double taxRate) const {
        return amount * (1 + taxRate);
    }

    void updateStatus(const string& newStatus) {
        status = newStatus;
    }

    bool isValid() const {
        return amount > 0;
    }

    void print() const {
        cout << "   Order #" << orderId
             << ": $" << amount
             << " - " << status << endl;
    }

    int getId() const { return orderId; }
    double getAmount() const { return amount; }
    string getStatus() const { return status; }
};
}

void example3_data_operations() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 3: DATA AND OPERATIONS TOGETHER        ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (Low Cohesion):\n";
    cout << "   struct OrderData { ... };  // Just data\n";
    cout << "   class OrderOperations {    // Separate operations\n";
    cout << "       calculateTotal(OrderData&)\n";
    cout << "       updateStatus(OrderData&)\n";
    cout << "   };\n";
    cout << "   🚨 Data and operations separated!\n";

    cout << "\n✅ GOOD CODE (High Cohesion):\n";
    cout << "   class Order {\n";
    cout << "       int orderId;           // Data\n";
    cout << "       double amount;\n";
    cout << "       calculateTotal()       // Operations\n";
    cout << "       updateStatus()\n";
    cout << "   };\n";
    cout << "   ✅ Data and operations together!\n";

    cout << "\n✨ DEMONSTRATION:\n";

    good_example3::Order order(101, 100.0);

    cout << "\n   Order details:\n";
    order.print();

    cout << "\n   Calculating total with 10% tax:\n";
    cout << "   Total: $" << order.calculateTotal(0.10) << endl;

    cout << "\n   Updating status:\n";
    order.updateStatus("shipped");
    order.print();

    cout << "\n✨ BENEFITS:\n";
    cout << "   • Data and operations naturally together\n";
    cout << "   • Methods use the data they need\n";
    cout << "   • High cohesion within Order class\n";
    cout << "   • Object-oriented design\n";
}

// ============================================
// EXAMPLE 4: COHESION LEVELS DEMONSTRATION
// ============================================

namespace example4 {
// 🏆 FUNCTIONAL COHESION (Best) - Single purpose
class PasswordHasher {
private:
    string salt;

public:
    PasswordHasher(const string& salt_) : salt(salt_) {}

    string hash(const string& password) {
        // All methods work toward single purpose: hashing passwords
        string salted = addSalt(password);
        return computeHash(salted);
    }

private:
    string addSalt(const string& password) {
        return password + salt;
    }

    string computeHash(const string& input) {
        // Simplified hash
        return "hash_" + input;
    }
};

// ⚠️ SEQUENTIAL COHESION - Output of one is input of another
class DataProcessor {
public:
    string process(const string& rawData) {
        string validated = validateData(rawData);
        string cleaned = cleanData(validated);
        string formatted = formatData(cleaned);
        return formatted;
    }

private:
    string validateData(const string& data) { return data; }
    string cleanData(const string& data) { return data; }
    string formatData(const string& data) { return data; }
};

// 🚨 COINCIDENTAL COHESION (Worst) - No relationship
class MiscellaneousHelper {
public:
    // Random unrelated methods
    int add(int a, int b) { return a + b; }
    string getCurrentTime() { return "12:00"; }
    bool isEven(int n) { return n % 2 == 0; }
    void printMessage(const string& msg) { cout << msg << endl; }
    // No cohesion whatsoever!
};
}

void example4_cohesion_levels() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 4: COHESION LEVELS                     ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n📊 COHESION LEVELS (Best to Worst):\n";

    cout << "\n1. 🏆 FUNCTIONAL COHESION (Best):\n";
    cout << "   class PasswordHasher {\n";
    cout << "       hash()      // Single purpose\n";
    cout << "       addSalt()   // Support hashing\n";
    cout << "       computeHash() // Support hashing\n";
    cout << "   };\n";
    cout << "   ✅ All methods work toward single goal\n";

    cout << "\n2. ⚠️  SEQUENTIAL COHESION:\n";
    cout << "   class DataProcessor {\n";
    cout << "       validate() → clean() → format()\n";
    cout << "   };\n";
    cout << "   ⚠️  Output of one is input of next\n";

    cout << "\n7. 🚨 COINCIDENTAL COHESION (Worst):\n";
    cout << "   class MiscHelper {\n";
    cout << "       add()            // Math\n";
    cout << "       getCurrentTime() // Time\n";
    cout << "       isEven()         // Math\n";
    cout << "       printMessage()   // I/O\n";
    cout << "   };\n";
    cout << "   🚨 No relationship between methods!\n";

    cout << "\n✨ DEMONSTRATION:\n";

    example4::PasswordHasher hasher("secret_salt");
    cout << "\n   Hashing password:\n";
    cout << "   " << hasher.hash("mypassword") << endl;

    cout << "\n💡 GOAL:\n";
    cout << "   Always aim for FUNCTIONAL COHESION!\n";
    cout << "   Each class = one clear purpose\n";
}

// ============================================
// EXAMPLE 5: MEASURING COHESION
// ============================================

namespace example5 {
// ❌ LOW COHESION: Methods use different data
class LowCohesionClass {
private:
    string name;
    int age;
    double salary;
    string address;

public:
    // Only uses name
    void printName() {
        cout << "   Name: " << name << endl;
    }

    // Only uses age
    void printAge() {
        cout << "   Age: " << age << endl;
    }

    // Only uses salary
    void calculateTax() {
        cout << "   Tax: $" << salary * 0.2 << endl;
    }

    // Only uses address
    void printAddress() {
        cout << "   Address: " << address << endl;
    }

    // 🚨 Each method uses different data (low cohesion)
};

// ✅ HIGH COHESION: Methods use same data
class HighCohesionClass {
private:
    double balance;
    double interestRate;

public:
    HighCohesionClass(double bal, double rate)
        : balance(bal), interestRate(rate) {}

    // Uses balance and interestRate
    double calculateInterest() {
        return balance * interestRate;
    }

    // Uses balance and interestRate
    void addInterest() {
        balance += calculateInterest();
    }

    // Uses balance
    double getBalance() const {
        return balance;
    }

    // Uses balance
    void deposit(double amount) {
        balance += amount;
    }

    // ✅ All methods work with the same data (high cohesion)
};
}

void example5_measuring_cohesion() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 5: MEASURING COHESION                  ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n📏 HOW TO MEASURE COHESION:\n";

    cout << "\n❌ LOW COHESION:\n";
    cout << "   class LowCohesion {\n";
    cout << "       name, age, salary, address  // 4 fields\n";
    cout << "       \n";
    cout << "       printName()     → uses only 'name'\n";
    cout << "       printAge()      → uses only 'age'\n";
    cout << "       calculateTax()  → uses only 'salary'\n";
    cout << "       printAddress()  → uses only 'address'\n";
    cout << "   };\n";
    cout << "   🚨 Each method uses 25% of data (low cohesion)\n";

    cout << "\n✅ HIGH COHESION:\n";
    cout << "   class HighCohesion {\n";
    cout << "       balance, interestRate  // 2 fields\n";
    cout << "       \n";
    cout << "       calculateInterest() → uses both fields\n";
    cout << "       addInterest()       → uses both fields\n";
    cout << "       getBalance()        → uses balance\n";
    cout << "       deposit()           → uses balance\n";
    cout << "   };\n";
    cout << "   ✅ Methods use 50-100% of data (high cohesion)\n";

    cout << "\n📊 COHESION METRIC:\n";
    cout << "   LCOM (Lack of Cohesion of Methods):\n";
    cout << "   • Count methods that don't share fields\n";
    cout << "   • Lower LCOM = Higher cohesion\n";
    cout << "   • LCOM = 0 is ideal (all methods share data)\n";

    cout << "\n💡 RULE OF THUMB:\n";
    cout << "   If methods use < 50% of class data:\n";
    cout << "   → Consider splitting the class!\n";
}

// ============================================
// EXAMPLE 6: IMPROVING COHESION
// ============================================

namespace example6 {
// ❌ BEFORE: Low cohesion
class EmployeeBad {
private:
    string name;
    double salary;
    string department;

    // Unrelated: Report generation
    string reportFormat;
    string reportHeader;

public:
    void setSalary(double s) { salary = s; }
    void setDepartment(const string& d) { department = d; }

    // 🚨 Unrelated to Employee data
    void generateReport() {
        cout << "   " << reportHeader << endl;
        cout << "   Employee: " << name << endl;
        cout << "   Format: " << reportFormat << endl;
    }
};

// ✅ AFTER: High cohesion
class Employee {
private:
    string name;
    double salary;
    string department;

public:
    Employee(const string& n, double s, const string& d)
        : name(n), salary(s), department(d) {}

    void setSalary(double s) { salary = s; }
    void setDepartment(const string& d) { department = d; }

    string getName() const { return name; }
    double getSalary() const { return salary; }
    string getDepartment() const { return department; }
};

// Report generation separated (high cohesion)
class EmployeeReportGenerator {
private:
    string format;
    string header;

public:
    EmployeeReportGenerator(const string& fmt, const string& hdr)
        : format(fmt), header(hdr) {}

    void generateReport(const Employee& employee) {
        cout << "   " << header << endl;
        cout << "   Employee: " << employee.getName() << endl;
        cout << "   Salary: $" << employee.getSalary() << endl;
        cout << "   Department: " << employee.getDepartment() << endl;
        cout << "   Format: " << format << endl;
    }
};
}

void example6_improving_cohesion() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 6: IMPROVING COHESION                  ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n🔧 HOW TO IMPROVE COHESION:\n";

    cout << "\n❌ BEFORE (Low Cohesion):\n";
    cout << "   class Employee {\n";
    cout << "       // Employee data\n";
    cout << "       name, salary, department\n";
    cout << "       \n";
    cout << "       // Unrelated: Report data 🚨\n";
    cout << "       reportFormat, reportHeader\n";
    cout << "       generateReport()\n";
    cout << "   };\n";

    cout << "\n✅ AFTER (High Cohesion):\n";
    cout << "   class Employee {\n";
    cout << "       name, salary, department  // Related data\n";
    cout << "       setSalary(), setDepartment()\n";
    cout << "   };\n";
    cout << "   \n";
    cout << "   class EmployeeReportGenerator {\n";
    cout << "       format, header            // Related data\n";
    cout << "       generateReport(Employee&)\n";
    cout << "   };\n";

    cout << "\n✨ DEMONSTRATION:\n";

    example6::Employee emp("Alice", 75000, "Engineering");
    example6::EmployeeReportGenerator generator("PDF", "=== Employee Report ===");

    cout << "\n   Generating report:\n";
    generator.generateReport(emp);

    cout << "\n📋 IMPROVEMENT STEPS:\n";
    cout << "   1. Identify unrelated data/methods\n";
    cout << "   2. Group related items together\n";
    cout << "   3. Create new class for unrelated group\n";
    cout << "   4. Move unrelated items to new class\n";
    cout << "   5. Verify each class has single purpose\n";
}

// ============================================
// EXAMPLE 7: HIGH COHESION BENEFITS
// ============================================

void example7_benefits() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 7: HIGH COHESION BENEFITS              ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n✅ BENEFITS OF HIGH COHESION:\n";

    cout << "\n1. EASIER TO UNDERSTAND:\n";
    cout << "   • Class has clear, single purpose\n";
    cout << "   • Related functionality in one place\n";
    cout << "   • Easy to name the class\n";

    cout << "\n2. EASIER TO MAINTAIN:\n";
    cout << "   • Changes localized to one class\n";
    cout << "   • Less ripple effect\n";
    cout << "   • Fewer dependencies\n";

    cout << "\n3. MORE REUSABLE:\n";
    cout << "   • Focused functionality\n";
    cout << "   • Can use class independently\n";
    cout << "   • No unnecessary baggage\n";

    cout << "\n4. EASIER TO TEST:\n";
    cout << "   • Test single responsibility\n";
    cout << "   • Fewer test cases\n";
    cout << "   • Clear test scenarios\n";

    cout << "\n5. BETTER ORGANIZATION:\n";
    cout << "   • Logical grouping\n";
    cout << "   • Clear structure\n";
    cout << "   • Easy to navigate codebase\n";

    cout << "\n⚠️  LOW COHESION PROBLEMS:\n";
    cout << "   • Hard to understand purpose\n";
    cout << "   • Changes affect multiple areas\n";
    cout << "   • Hard to reuse\n";
    cout << "   • Difficult to test\n";
    cout << "   • Poor organization\n";
    cout << "   • God classes\n";

    cout << "\n💡 KEY INSIGHT:\n";
    cout << "   High cohesion means:\n";
    cout << "   \"Everything in this class belongs together\n";
    cout << "    and nothing else should be here.\"\n";

    cout << "\n🎯 RELATIONSHIP TO OTHER PRINCIPLES:\n";
    cout << "   High Cohesion + Low Coupling = Good Design\n";
    cout << "   High Cohesion ≈ Single Responsibility Principle\n";
    cout << "   High Cohesion → Easier to apply SOLID\n";
}

// ============================================
// EXAMPLE 8: HOW TO ACHIEVE HIGH COHESION
// ============================================

void example8_how_to_achieve() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 8: HOW TO ACHIEVE HIGH COHESION        ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n✅ STRATEGIES FOR HIGH COHESION:\n";

    cout << "\n1. SINGLE RESPONSIBILITY:\n";
    cout << "   • Each class does ONE thing\n";
    cout << "   • One reason to change\n";
    cout << "   • Clear, focused purpose\n";

    cout << "\n2. GROUP RELATED DATA AND OPERATIONS:\n";
    cout << "   • Data and methods that work with it together\n";
    cout << "   • Don't separate data from operations\n";
    cout << "   • Object-oriented design\n";

    cout << "\n3. AVOID UTILITY/HELPER CLASSES:\n";
    cout << "   ❌ class Utils { /* random methods */ }\n";
    cout << "   ✅ class StringUtils { /* string methods */ }\n";
    cout << "   ✅ class MathUtils { /* math methods */ }\n";

    cout << "\n4. CHECK METHOD-DATA USAGE:\n";
    cout << "   • Do methods use most of the class data?\n";
    cout << "   • If not, consider splitting\n";
    cout << "   • Aim for 50%+ data usage per method\n";

    cout << "\n5. ONE CONCEPT PER CLASS:\n";
    cout << "   • User management → UserManager\n";
    cout << "   • Email sending → EmailService\n";
    cout << "   • Database access → Repository\n";
    cout << "   • Don't mix concepts\n";

    cout << "\n6. EXTRACT UNRELATED FUNCTIONALITY:\n";
    cout << "   • If methods don't use class data → extract\n";
    cout << "   • If functionality unrelated → new class\n";
    cout << "   • Split large classes\n";

    cout << "\n🔍 COHESION CHECKLIST:\n";
    cout << "   □ Does class have single, clear purpose?\n";
    cout << "   □ Can you name it without 'and' or 'or'?\n";
    cout << "   □ Do methods use most of the data?\n";
    cout << "   □ Are all methods related to the purpose?\n";
    cout << "   □ Would you reuse this class as-is?\n";
    cout << "   □ Is it easy to explain what class does?\n";

    cout << "\n⚠️  WARNING SIGNS (Low Cohesion):\n";
    cout << "   🚨 Class name has 'Manager', 'Handler', 'Helper', 'Utils'\n";
    cout << "   🚨 Methods use different subsets of data\n";
    cout << "   🚨 Hard to name the class\n";
    cout << "   🚨 Class doing multiple unrelated things\n";
    cout << "   🚨 Changes affect multiple areas\n";

    cout << "\n📏 MEASURING COHESION:\n";
    cout << "   Simple formula:\n";
    cout << "   Cohesion = (Methods using field) / (Total methods)\n";
    cout << "   \n";
    cout << "   Average across all fields:\n";
    cout << "   • 80-100% = High cohesion ✅\n";
    cout << "   • 50-79%  = Medium cohesion ⚠️\n";
    cout << "   • < 50%   = Low cohesion 🚨\n";
}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║              HIGH COHESION PRINCIPLE                  ║\n";
    cout << "║                                                       ║\n";
    cout << "║  \"A class should have high cohesion - all its methods ║\n";
    cout << "║   and data should be closely related and work toward  ║\n";
    cout << "║   a single purpose.\"                                 ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n";

    example1_god_class();
    example2_utility_class();
    example3_data_operations();
    example4_cohesion_levels();
    example5_measuring_cohesion();
    example6_improving_cohesion();
    example7_benefits();
    example8_how_to_achieve();

    cout << "\n\n";
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║         🎉 HIGH COHESION PRINCIPLE COMPLETE! 🎉       ║\n";
    cout << "║                                                       ║\n";
    cout << "║  Key Takeaway: Keep related functionality together!   ║\n";
    cout << "║  Each class should have a single, well-defined        ║\n";
    cout << "║  purpose with all methods working toward that goal.   ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n\n";

    return 0;
}
