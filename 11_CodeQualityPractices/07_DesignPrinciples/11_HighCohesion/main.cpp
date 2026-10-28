#include <vector>
#include <string>
#include <memory>
#include <map>
#include <fstream>
#include <print>

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
    std::string username;
    std::string email;
    std::vector<std::string> orders;

public:
    // User-related operations
    void setUserName(const std::string& name) { username = name; }
    void setEmail(const std::string& email_) { email = email_; }
    std::string getUserName() const { return username; }

    // 🚨 Unrelated: Database operations
    void connectToDatabase() {
        std::println("   🔌 Connecting to database...");
    }

    void saveToDatabase() {
        std::println("   💾 Saving user to database...");
    }

    // 🚨 Unrelated: Email operations
    void sendWelcomeEmail() {
        std::println("   📧 Sending welcome email to: {}", email);
    }

    void sendPasswordResetEmail() {
        std::println("   📧 Sending password reset email...");
    }

    // 🚨 Unrelated: Logging operations
    void logActivity(const std::string& activity) {
        std::println("   📝 Logging: {}", activity);
    }

    void logError(const std::string& error) {
        std::println("   ❌ Error: {}", error);
    }

    // 🚨 Unrelated: Validation
    bool validateEmail(const std::string& email_) {
        return email_.find("@") != std::string::npos;
    }

    bool validatePassword(const std::string& password) {
        return password.length() >= 8;
    }

    // 🚨 Unrelated: Order operations
    void addOrder(const std::string& order) {
        orders.push_back(order);
    }

    std::vector<std::string> getOrders() const {
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
    std::string username;
    std::string email;

public:
    User(const std::string& name, const std::string& email_)
        : username(name), email(email_) {}

    std::string getUserName() const { return username; }
    std::string getEmail() const { return email; }
    void setUserName(const std::string& name) { username = name; }
    void setEmail(const std::string& email_) { email = email_; }
};

// Database operations only (high cohesion)
class UserRepository {
public:
    void save(const User& user) {
        std::println("   💾 Saving user to database: {}", user.getUserName());
    }

    User* load(const std::string& username) {
        std::println("   📂 Loading user from database: {}", username);
        return new User(username, "user@example.com");
    }
};

// Email operations only (high cohesion)
class EmailService {
public:
    void sendWelcomeEmail(const User& user) {
        std::println("   📧 Sending welcome email to: {}", user.getEmail());
    }

    void sendPasswordResetEmail(const User& user) {
        std::println("   📧 Sending password reset to: {}", user.getEmail());
    }
};

// Validation only (high cohesion)
class Validator {
public:
    bool isValidEmail(const std::string& email) {
        return email.find("@") != std::string::npos;
    }

    bool isValidPassword(const std::string& password) {
        return password.length() >= 8;
    }
};

// Logging only (high cohesion)
class Logger {
public:
    void logActivity(const std::string& activity) {
        std::println("   📝 {}", activity);
    }

    void logError(const std::string& error) {
        std::println("   ❌ {}", error);
    }
};
}

void example1_god_class() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 1: LOW COHESION (God Class)            ║");
    std::println("╚══════════════════════════════════════════════════╝");

    good_example1::User user("Alice", "alice@example.com");
    good_example1::UserRepository repo;
    good_example1::EmailService emailService;
    good_example1::Validator validator;
    good_example1::Logger logger;

    std::println("");
    std::println("   Creating and saving user:");
    repo.save(user);
    logger.logActivity("User created: " + user.getUserName());

    std::println("");
    std::println("   Sending welcome email:");
    if (validator.isValidEmail(user.getEmail())) {
        emailService.sendWelcomeEmail(user);
    }

}

// ============================================
// EXAMPLE 2: UTILITY CLASS (Low Cohesion)
// ============================================

namespace bad_example2 {
// ❌ BAD: Low cohesion - random utility methods

class Utils {
public:
    // 🚨 String operation
    static std::string toUpperCase(const std::string& str) {
        std::string result = str;
        for (char& c : result) c = toupper(c);
        return result;
    }

    // 🚨 Math operation
    static int factorial(int n) {
        return n <= 1 ? 1 : n * factorial(n - 1);
    }

    // 🚨 Date operation
    static std::string getCurrentDate() {
        return "2024-01-01";
    }

    // 🚨 File operation
    static bool fileExists(const std::string& filename) {
        std::ifstream file(filename);
        return file.good();
    }

    // 🚨 Network operation
    static bool isValidIP(const std::string& ip) {
        return ip.find(".") != std::string::npos;
    }

    // 🚨 Completely unrelated operations in one class!
};

// PROBLEM: No cohesion - random unrelated utilities
}

namespace good_example2 {
// ✅ GOOD: High cohesion - grouped by purpose

class StringUtils {
public:
    static std::string toUpperCase(const std::string& str) {
        std::string result = str;
        for (char& c : result) c = toupper(c);
        return result;
    }

    static std::string toLowerCase(const std::string& str) {
        std::string result = str;
        for (char& c : result) c = tolower(c);
        return result;
    }

    static std::string trim(const std::string& str) {
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
    static bool exists(const std::string& filename) {
        std::ifstream file(filename);
        return file.good();
    }

    static std::string readFile(const std::string& filename) {
        // Read implementation
        return "";
    }

    static void writeFile(const std::string& filename, const std::string& content) {
        // Write implementation
    }
};

class NetworkUtils {
public:
    static bool isValidIP(const std::string& ip) {
        return ip.find(".") != std::string::npos;
    }

    static bool isValidURL(const std::string& url) {
        return url.find("http") == 0;
    }
};
}

void example2_utility_class() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 2: UTILITY CLASS (Low Cohesion)        ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   HELLO -> {}", good_example2::StringUtils::toUpperCase("hello"));

    std::println("");
    std::println("   Math operations:");
    std::println("   5! = {}", good_example2::MathUtils::factorial(5));
    std::println("   Is 7 prime? {}", (good_example2::MathUtils::isPrime(7) ? "Yes" : "No"));

}

// ============================================
// EXAMPLE 3: DATA AND OPERATIONS TOGETHER
// ============================================

namespace bad_example3 {
// ❌ BAD: Data in one place, operations elsewhere (low cohesion)

struct OrderData {
    int orderId;
    double amount;
    std::string status;
};

class OrderOperations {
public:
    // 🚨 Operations separated from data
    double calculateTotal(const OrderData& order, double taxRate) {
        return order.amount * (1 + taxRate);
    }

    void updateStatus(OrderData& order, const std::string& newStatus) {
        order.status = newStatus;
    }

    bool isValid(const OrderData& order) {
        return order.amount > 0;
    }

    void print(const OrderData& order) {
        std::println("   Order #{}: ${} - {}", order.orderId, order.amount, order.status);
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
    std::string status;

public:
    Order(int id, double amt)
        : orderId(id), amount(amt), status("pending") {}

    // Operations work with the data in this class
    double calculateTotal(double taxRate) const {
        return amount * (1 + taxRate);
    }

    void updateStatus(const std::string& newStatus) {
        status = newStatus;
    }

    bool isValid() const {
        return amount > 0;
    }

    void print() const {
        std::println("   Order #{}: ${} - {}", orderId, amount, status);
    }

    int getId() const { return orderId; }
    double getAmount() const { return amount; }
    std::string getStatus() const { return status; }
};
}

void example3_data_operations() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 3: DATA AND OPERATIONS TOGETHER        ║");
    std::println("╚══════════════════════════════════════════════════╝");

    good_example3::Order order(101, 100.0);

    std::println("");
    std::println("   Order details:");
    order.print();

    std::println("");
    std::println("   Calculating total with 10% tax:");
    std::println("   Total: ${}", order.calculateTotal(0.10));

    std::println("");
    std::println("   Updating status:");
    order.updateStatus("shipped");
    order.print();

}

// ============================================
// EXAMPLE 4: COHESION LEVELS DEMONSTRATION
// ============================================

namespace example4 {
// 🏆 FUNCTIONAL COHESION (Best) - Single purpose
class PasswordHasher {
private:
    std::string salt;

public:
    PasswordHasher(const std::string& salt_) : salt(salt_) {}

    std::string hash(const std::string& password) {
        // All methods work toward single purpose: hashing passwords
        std::string salted = addSalt(password);
        return computeHash(salted);
    }

private:
    std::string addSalt(const std::string& password) {
        return password + salt;
    }

    std::string computeHash(const std::string& input) {
        // Simplified hash
        return "hash_" + input;
    }
};

// ⚠️ SEQUENTIAL COHESION - Output of one is input of another
class DataProcessor {
public:
    std::string process(const std::string& rawData) {
        std::string validated = validateData(rawData);
        std::string cleaned = cleanData(validated);
        std::string formatted = formatData(cleaned);
        return formatted;
    }

private:
    std::string validateData(const std::string& data) { return data; }
    std::string cleanData(const std::string& data) { return data; }
    std::string formatData(const std::string& data) { return data; }
};

// 🚨 COINCIDENTAL COHESION (Worst) - No relationship
class MiscellaneousHelper {
public:
    // Random unrelated methods
    int add(int a, int b) { return a + b; }
    std::string getCurrentTime() { return "12:00"; }
    bool isEven(int n) { return n % 2 == 0; }
    void printMessage(const std::string& msg) { std::println("{}", msg); }
    // No cohesion whatsoever!
};
}

void example4_cohesion_levels() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 4: COHESION LEVELS                     ║");
    std::println("╚══════════════════════════════════════════════════╝");

    example4::PasswordHasher hasher("secret_salt");
    std::println("");
    std::println("   Hashing password:");
    std::println("   {}", hasher.hash("mypassword"));

}

// ============================================
// EXAMPLE 5: MEASURING COHESION
// ============================================

namespace example5 {
// ❌ LOW COHESION: Methods use different data
class LowCohesionClass {
private:
    std::string name;
    int age;
    double salary;
    std::string address;

public:
    // Only uses name
    void printName() {
        std::println("   Name: {}", name);
    }

    // Only uses age
    void printAge() {
        std::println("   Age: {}", age);
    }

    // Only uses salary
    void calculateTax() {
        std::println("   Tax: ${}", salary * 0.2);
    }

    // Only uses address
    void printAddress() {
        std::println("   Address: {}", address);
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
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 5: MEASURING COHESION                  ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("       ");

    std::println("       ");

}

// ============================================
// EXAMPLE 6: IMPROVING COHESION
// ============================================

namespace example6 {
// ❌ BEFORE: Low cohesion
class EmployeeBad {
private:
    std::string name;
    double salary;
    std::string department;

    // Unrelated: Report generation
    std::string reportFormat;
    std::string reportHeader;

public:
    void setSalary(double s) { salary = s; }
    void setDepartment(const std::string& d) { department = d; }

    // 🚨 Unrelated to Employee data
    void generateReport() {
        std::println("   {}", reportHeader);
        std::println("   Employee: {}", name);
        std::println("   Format: {}", reportFormat);
    }
};

// ✅ AFTER: High cohesion
class Employee {
private:
    std::string name;
    double salary;
    std::string department;

public:
    Employee(const std::string& n, double s, const std::string& d)
        : name(n), salary(s), department(d) {}

    void setSalary(double s) { salary = s; }
    void setDepartment(const std::string& d) { department = d; }

    std::string getName() const { return name; }
    double getSalary() const { return salary; }
    std::string getDepartment() const { return department; }
};

// Report generation separated (high cohesion)
class EmployeeReportGenerator {
private:
    std::string format;
    std::string header;

public:
    EmployeeReportGenerator(const std::string& fmt, const std::string& hdr)
        : format(fmt), header(hdr) {}

    void generateReport(const Employee& employee) {
        std::println("   {}", header);
        std::println("   Employee: {}", employee.getName());
        std::println("   Salary: ${}", employee.getSalary());
        std::println("   Department: {}", employee.getDepartment());
        std::println("   Format: {}", format);
    }
};
}

void example6_improving_cohesion() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 6: IMPROVING COHESION                  ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("       ");

    std::println("   ");

    example6::Employee emp("Alice", 75000, "Engineering");
    example6::EmployeeReportGenerator generator("PDF", "=== Employee Report ===");

    std::println("");
    std::println("   Generating report:");
    generator.generateReport(emp);

}

// ============================================
// EXAMPLE 7: HIGH COHESION BENEFITS
// ============================================

void example7_benefits() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 7: HIGH COHESION BENEFITS              ║");
    std::println("╚══════════════════════════════════════════════════╝");

}

// ============================================
// EXAMPLE 8: HOW TO ACHIEVE HIGH COHESION
// ============================================

void example8_how_to_achieve() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 8: HOW TO ACHIEVE HIGH COHESION        ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   ");
}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    std::println("╔═══════════════════════════════════════════════════════╗");
    std::println("║                                                       ║");
    std::println("║              HIGH COHESION PRINCIPLE                  ║");
    std::println("║                                                       ║");
    std::println("║  \"A class should have high cohesion - all its methods║");
    std::println("║   and data should be closely related and work toward  ║");
    std::println("║   a single purpose.\"                                 ║");
    std::println("║                                                       ║");
    std::println("╚═══════════════════════════════════════════════════════╝");

    example1_god_class();
    example2_utility_class();
    example3_data_operations();
    example4_cohesion_levels();
    example5_measuring_cohesion();
    example6_improving_cohesion();
    example7_benefits();
    example8_how_to_achieve();

    return 0;
}
