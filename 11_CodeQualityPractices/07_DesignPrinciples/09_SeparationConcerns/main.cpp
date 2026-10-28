#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <memory>
#include <map>
#include <print>

// ============================================
// SEPARATION OF CONCERNS (SoC)
// ============================================

/*
SEPARATION OF CONCERNS PRINCIPLE:
    "Separate different concerns into distinct sections"
    "Each module should have a single, well-defined responsibility"
    — Edsger W. Dijkstra

WHAT IT MEANS:
    • Break complex problems into smaller, focused pieces
    • Each piece handles ONE concern/responsibility
    • Different concerns shouldn't be mixed together
    • Related functionality grouped together
    • Unrelated functionality kept separate

WHY IT MATTERS:
    ✅ Easier to understand (focus on one thing at a time)
    ✅ Easier to maintain (changes isolated)
    ✅ Easier to test (test each concern independently)
    ✅ Better reusability (concerns can be reused)
    ✅ Parallel development (different people, different concerns)

KEY CONCEPTS:
    • Concern = A specific aspect or responsibility
    • Cohesion = How related things are within a module
    • Coupling = How dependent modules are on each other
    • High cohesion + Low coupling = Good separation

COMMON CONCERNS:
    • Data persistence (database, files)
    • Business logic (rules, calculations)
    • User interface (display, input)
    • Validation (checking data)
    • Logging (recording events)
    • Error handling
    • Configuration
    • Communication (network, APIs)

VIOLATION SIGNS:
    🚨 Classes doing too many different things
    🚨 Mixed business logic with UI code
    🚨 Database queries in presentation layer
    🚨 Hard to test because everything is tangled
    🚨 Changes in one area break unrelated areas
    🚨 Can't reuse code because it's too coupled

GOAL: Each module should do ONE thing well!
*/

// ============================================
// EXAMPLE 1: MIXED UI AND BUSINESS LOGIC
// ============================================

namespace bad_example1 {
// ❌ BAD: Everything mixed together

class UserManager {
public:
    void createUser() {
        // 🚨 UI concerns mixed with business logic
        std::println("=== Create User ===");
        std::print("Enter username: ");
        std::string username;
        std::cin >> username;

        std::print("Enter email: ");
        std::string email;
        std::cin >> email;

        std::print("Enter password: ");
        std::string password;
        std::cin >> password;

        // Validation mixed in
        if (username.empty()) {
            std::println("Error: Username required!");
            return;
        }

        if (email.find("@") == std::string::npos) {
            std::println("Error: Invalid email!");
            return;
        }

        if (password.length() < 8) {
            std::println("Error: Password too short!");
            return;
        }

        // Database logic mixed in
        std::println("Saving to database...");
        // Imagine actual database code here

        // Logging mixed in
        std::println("User created successfully!");
        std::println("Username: {}", username);
        std::println("Email: {}", email);
    }

    // PROBLEMS:
    // • UI, validation, business logic, database all mixed
    // • Can't test business logic without UI
    // • Can't reuse validation
    // • Can't change UI without touching business logic
    // • Hard to maintain
};
}

namespace good_example1 {
// ✅ GOOD: Concerns separated

// CONCERN 1: Data model (pure data)
struct User {
    std::string username;
    std::string email;
    std::string password;

    User(const std::string& u, const std::string& e, const std::string& p)
        : username(u), email(e), password(p) {}
};

// CONCERN 2: Validation (business rules)
class UserValidator {
public:
    static bool validateUsername(const std::string& username) {
        return !username.empty() && username.length() >= 3;
    }

    static bool validateEmail(const std::string& email) {
        return email.find("@") != std::string::npos &&
               email.find(".") != std::string::npos;
    }

    static bool validatePassword(const std::string& password) {
        return password.length() >= 8;
    }

    static bool validateUser(const User& user) {
        return validateUsername(user.username) &&
               validateEmail(user.email) &&
               validatePassword(user.password);
    }
};

// CONCERN 3: Business logic
class UserService {
public:
    User createUser(const std::string& username, const std::string& email,
                    const std::string& password) {
        User user(username, email, password);

        if (!UserValidator::validateUser(user)) {
            throw std::runtime_error("Invalid user data");
        }

        // Business logic: create user
        return user;
    }
};

// CONCERN 4: Data persistence
class UserRepository {
public:
    void save(const User& user) {
        // Database operations only
        std::println("[DB] Saving user: {}", user.username);
    }
};

// CONCERN 5: User interface
class UserUI {
private:
    UserService service;
    UserRepository repository;

public:
    void createUserInterface() {
        std::println("");
        std::println("=== Create User ===");

        std::string username, email, password;

        std::print("Enter username: ");
        std::cin >> username;

        std::print("Enter email: ");
        std::cin >> email;

        std::print("Enter password: ");
        std::cin >> password;

        try {
            // Use services
            User user = service.createUser(username, email, password);
            repository.save(user);

            std::println("");
            std::println("✅ User created successfully!");
            std::println("Username: {}", user.username);
            std::println("Email: {}", user.email);

        } catch (const std::exception& e) {
            std::println("");
            std::println("❌ Error: {}", e.what());
        }
    }
};
}

void example1_mixed_concerns() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 1: MIXED UI AND BUSINESS LOGIC          ║");
    std::println("╚═══════════════════════════════════════════════════╝");

    std::println("        ");
    std::println("        ");
    std::println("        ");

}

// ============================================
// EXAMPLE 2: DATA ACCESS IN BUSINESS LOGIC
// ============================================

namespace bad_example2 {
// ❌ BAD: Business logic mixed with data access

class OrderProcessor {
public:
    void processOrder(int orderId) {
        // 🚨 Database concerns in business logic
        std::println("SELECT * FROM orders WHERE id = {}", orderId);
        // Imagine fetching order from database

        double total = 100.0;  // Pretend we got this from DB

        // Business logic
        if (total > 1000) {
            total *= 0.9;  // 10% discount
            std::println("Discount applied!");
        }

        // 🚨 More database concerns
        std::println("UPDATE orders SET total = {} WHERE id = {}", total, orderId);

        // PROBLEMS:
        // • Can't test business logic without database
        // • Can't reuse discount logic
        // • Can't change database without changing business logic
        // • Three different concerns tangled together
    }
};
}

namespace good_example2 {
// ✅ GOOD: Concerns properly separated

// CONCERN 1: Data model
struct Order {
    int id;
    double total;
    std::string customerEmail;

    Order(int i, double t, const std::string& email)
        : id(i), total(t), customerEmail(email) {}
};

// CONCERN 2: Data access
class OrderRepository {
public:
    Order findById(int orderId) {
        std::println("[DB] Fetching order {}", orderId);
        // Database access only
        return Order(orderId, 100.0, "customer@example.com");
    }

    void update(const Order& order) {
        std::println("[DB] Updating order {} with total {}", order.id, order.total);
    }
};

// CONCERN 3: Business logic
class OrderService {
public:
    void applyDiscount(Order& order) {
        // Pure business logic
        if (order.total > 1000) {
            order.total *= 0.9;  // 10% discount
            std::println("[BUSINESS] Discount applied: ${}", order.total);
        }
    }
};

// CONCERN 4: Notification
class EmailService {
public:
    void sendOrderConfirmation(const Order& order) {
        // Email concerns only
        std::println("[EMAIL] Sending confirmation to: {}", order.customerEmail);
        std::println("[EMAIL] Order #{} - Total: ${}", order.id, order.total);
    }
};

// CONCERN 5: Orchestration (coordinates other concerns)
class OrderProcessor {
private:
    OrderRepository repository;
    OrderService service;
    EmailService emailService;

public:
    void processOrder(int orderId) {
        // Clean orchestration
        Order order = repository.findById(orderId);
        service.applyDiscount(order);
        repository.update(order);
        emailService.sendOrderConfirmation(order);
    }
};
}

void example2_data_access() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 2: DATA ACCESS IN BUSINESS LOGIC        ║");
    std::println("╚═══════════════════════════════════════════════════╝");

    good_example2::OrderProcessor processor;
    processor.processOrder(123);

}

// ============================================
// EXAMPLE 3: REPORTING WITH MIXED CONCERNS
// ============================================

namespace bad_example3 {
// ❌ BAD: Data fetching, calculation, and formatting mixed

class SalesReport {
public:
    void generateReport() {
        // 🚨 Data access
        std::println("Connecting to database...");
        std::vector<double> sales = {100, 200, 150, 300, 250};

        // 🚨 Business logic (calculations)
        double total = 0;
        for (double sale : sales) {
            total += sale;
        }
        double average = total / sales.size();

        // 🚨 Formatting/presentation
        std::println("");
        std::println("===== SALES REPORT =====");
        std::println("Total Sales: ${}", total);
        std::println("Average Sale: ${}", average);
        std::println("Number of Sales: {}", sales.size());
        std::println("========================");

        // 🚨 File writing
        std::println("Writing to file...");
        // Imagine file writing code

        // PROBLEMS:
        // • Can't test calculations without database
        // • Can't reuse calculations
        // • Can't change output format easily
        // • Can't generate different report formats
    }
};
}

namespace good_example3 {
// ✅ GOOD: Each concern separated

// CONCERN 1: Data model
struct SalesData {
    std::vector<double> sales;
    double total;
    double average;
    int count;
};

// CONCERN 2: Data access
class SalesRepository {
public:
    std::vector<double> fetchSales() {
        std::println("[DB] Fetching sales data...");
        return {100, 200, 150, 300, 250};
    }
};

// CONCERN 3: Business logic (calculations)
class SalesCalculator {
public:
    SalesData calculate(const std::vector<double>& sales) {
        SalesData data;
        data.sales = sales;
        data.count = sales.size();

        data.total = 0;
        for (double sale : sales) {
            data.total += sale;
        }

        data.average = data.count > 0 ? data.total / data.count : 0;

        return data;
    }
};

// CONCERN 4: Formatting (presentation)
class ReportFormatter {
public:
    std::string formatConsole(const SalesData& data) {
        std::stringstream ss;
        ss << "\n===== SALES REPORT =====\n";
        ss << "Total Sales: $" << data.total << "\n";
        ss << "Average Sale: $" << data.average << "\n";
        ss << "Number of Sales: " << data.count << "\n";
        ss << "========================\n";
        return ss.str();
    }

    std::string formatHTML(const SalesData& data) {
        std::stringstream ss;
        ss << "<div class='report'>\n";
        ss << "  <h2>Sales Report</h2>\n";
        ss << "  <p>Total: $" << data.total << "</p>\n";
        ss << "  <p>Average: $" << data.average << "</p>\n";
        ss << "  <p>Count: " << data.count << "</p>\n";
        ss << "</div>\n";
        return ss.str();
    }

    std::string formatJSON(const SalesData& data) {
        std::stringstream ss;
        ss << "{\n";
        ss << "  \"total\": " << data.total << ",\n";
        ss << "  \"average\": " << data.average << ",\n";
        ss << "  \"count\": " << data.count << "\n";
        ss << "}\n";
        return ss.str();
    }
};

// CONCERN 5: Output (file writing)
class ReportWriter {
public:
    void writeToFile(const std::string& content, const std::string& filename) {
        std::println("[FILE] Writing report to {}", filename);
        // File writing logic
    }

    void writeToConsole(const std::string& content) {
        std::print("{}", content);
    }
};

// CONCERN 6: Orchestration
class SalesReportGenerator {
private:
    SalesRepository repository;
    SalesCalculator calculator;
    ReportFormatter formatter;
    ReportWriter writer;

public:
    void generateConsoleReport() {
        std::vector<double> sales = repository.fetchSales();
        SalesData data = calculator.calculate(sales);
        std::string report = formatter.formatConsole(data);
        writer.writeToConsole(report);
    }

    void generateHTMLReport() {
        std::vector<double> sales = repository.fetchSales();
        SalesData data = calculator.calculate(sales);
        std::string report = formatter.formatHTML(data);
        writer.writeToFile(report, "report.html");
    }

    void generateJSONReport() {
        std::vector<double> sales = repository.fetchSales();
        SalesData data = calculator.calculate(sales);
        std::string report = formatter.formatJSON(data);
        writer.writeToFile(report, "report.json");
    }
};
}

void example3_reporting() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 3: REPORTING WITH MIXED CONCERNS        ║");
    std::println("╚═══════════════════════════════════════════════════╝");

    good_example3::SalesReportGenerator generator;

    std::println("");
    std::print("--- Console Report ---");
    generator.generateConsoleReport();

    std::println("");
    std::print("--- HTML Report ---");
    generator.generateHTMLReport();

    std::println("");
    std::print("--- JSON Report ---");
    generator.generateJSONReport();

}

// ============================================
// EXAMPLE 4: LOGGING MIXED EVERYWHERE
// ============================================

namespace bad_example4 {
// ❌ BAD: Logging mixed throughout business logic

class PaymentProcessor {
public:
    bool processPayment(double amount) {
        std::println("[LOG] Starting payment processing");
        std::println("[LOG] Amount: ${}", amount);

        // Business logic
        if (amount <= 0) {
            std::println("[LOG] Invalid amount: {}", amount);
            return false;
        }

        // Validation logic

        // Payment processing
        return true;
    }

    // PROBLEMS:
    // • Logging scattered everywhere
    // • Hard to disable logging
    // • Hard to change log format
    // • Business logic obscured by logging
    // • Can't easily log to different destinations
};
}

namespace good_example4 {
// ✅ GOOD: Logging separated

// CONCERN 1: Logging interface
class Logger {
public:
    virtual void log(const std::string& message) = 0;
    virtual ~Logger() = default;
};

class ConsoleLogger : public Logger {
public:
    void log(const std::string& message) override {
        std::println("[LOG] {}", message);
    }
};

class FileLogger : public Logger {
public:
    void log(const std::string& message) override {
        // Write to file
        std::println("[FILE LOG] {}", message);
    }
};

class NullLogger : public Logger {
public:
    void log(const std::string& message) override {
        // Do nothing - for when logging disabled
    }
};

// CONCERN 2: Pure business logic
class PaymentProcessor {
private:
    std::shared_ptr<Logger> logger;

public:
    PaymentProcessor(std::shared_ptr<Logger> log) : logger(log) {}

    bool processPayment(double amount) {
        logger->log("Starting payment processing");
        logger->log("Amount: $" + std::to_string(amount));

        // Pure business logic - clean and clear
        if (amount <= 0) {
            logger->log("Invalid amount: " + std::to_string(amount));
            return false;
        }

        logger->log("Validating payment method");
        // Validation logic

        logger->log("Contacting payment gateway");
        // Payment processing

        logger->log("Payment successful");
        return true;
    }
};
}

void example4_logging() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 4: LOGGING MIXED EVERYWHERE             ║");
    std::println("╚═══════════════════════════════════════════════════╝");

    std::print("--- With Console Logger ---");
    auto consoleLogger = std::make_shared<good_example4::ConsoleLogger>();
    good_example4::PaymentProcessor processor1(consoleLogger);
    processor1.processPayment(100.50);

    std::println("");
    std::print("--- With Null Logger (no output) ---");
    auto nullLogger = std::make_shared<good_example4::NullLogger>();
    good_example4::PaymentProcessor processor2(nullLogger);
    processor2.processPayment(100.50);
    std::print("(Business logic executed, but no logs)");

    std::println("");
}

// ============================================
// EXAMPLE 5: LAYERED ARCHITECTURE
// ============================================

namespace good_example5 {
// ✅ GOOD: Classic layered architecture

// LAYER 1: Data/Domain Layer (Models)
namespace domain {
struct Product {
    int id;
    std::string name;
    double price;
    int stock;

    Product(int i, const std::string& n, double p, int s)
        : id(i), name(n), price(p), stock(s) {}
};
}

// LAYER 2: Data Access Layer (Repositories)
namespace data {
using namespace domain;

class ProductRepository {
public:
    Product findById(int id) {
        std::println("[DATA LAYER] Fetching product {}", id);
        return Product(id, "Sample Product", 29.99, 100);
    }

    void save(const Product& product) {
        std::println("[DATA LAYER] Saving product {}", product.id);
    }

    std::vector<Product> findAll() {
        std::println("[DATA LAYER] Fetching all products");
        return {
            Product(1, "Product A", 29.99, 100),
            Product(2, "Product B", 39.99, 50)
        };
    }
};
}

// LAYER 3: Business Logic Layer (Services)
namespace business {
using namespace domain;
using namespace data;

class ProductService {
private:
    ProductRepository repository;

public:
    Product getProduct(int id) {
        return repository.findById(id);
    }

    bool purchaseProduct(int productId, int quantity) {
        std::println("[BUSINESS LAYER] Processing purchase");

        Product product = repository.findById(productId);

        // Business logic
        if (product.stock < quantity) {
            std::println("[BUSINESS LAYER] Insufficient stock");
            return false;
        }

        product.stock -= quantity;
        repository.save(product);

        std::println("[BUSINESS LAYER] Purchase successful");
        return true;
    }

    double calculateTotalValue() {
        std::println("[BUSINESS LAYER] Calculating inventory value");

        std::vector<Product> products = repository.findAll();
        double total = 0;

        for (const auto& product : products) {
            total += product.price * product.stock;
        }

        return total;
    }
};
}

// LAYER 4: Presentation Layer (UI/API)
namespace presentation {
using namespace business;

class ProductController {
private:
    ProductService service;

public:
    void displayProduct(int id) {
        std::println("");
        std::println("[PRESENTATION LAYER] Displaying product");

        Product product = service.getProduct(id);
        std::println("╔═════════════════════════════╗");
        std::println("║ ID: {}                       ║", product.id);
        std::println("║ Name: {}        ║", product.name);
        std::println("║ Price: ${}               ║", product.price);
        std::println("║ Stock: {} units            ║", product.stock);
        std::println("╚═════════════════════════════╝");
    }

    void handlePurchase(int productId, int quantity) {
        std::println("");
        std::println("[PRESENTATION LAYER] Handling purchase request");

        bool success = service.purchaseProduct(productId, quantity);

        if (success) {
            std::println("✅ Purchase completed successfully!");
        } else {
            std::println("❌ Purchase failed - insufficient stock");
        }
    }

    void displayInventoryValue() {
        std::println("");
        std::println("[PRESENTATION LAYER] Displaying inventory value");

        double total = service.calculateTotalValue();
        std::println("╔════════════════════════════╗");
        std::println("║ Total: ${}             ║", total);
        std::println("╚════════════════════════════╝");
    }
};
}
}

void example5_layered_architecture() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 5: LAYERED ARCHITECTURE                 ║");
    std::println("╚═══════════════════════════════════════════════════╝");

    good_example5::presentation::ProductController controller;

    controller.displayProduct(1);
    controller.handlePurchase(1, 5);
    controller.displayInventoryValue();

}

// ============================================
// EXAMPLE 6: GOD CLASS ANTI-PATTERN
// ============================================

namespace bad_example6 {
// ❌ BAD: God class doing everything

class Application {
private:
    std::map<std::string, std::string> users;
    std::map<int, double> inventory;
    std::vector<std::string> logs;

public:
    // User management
    void registerUser(const std::string& username, const std::string& password) {
        users[username] = password;
        logs.push_back("User registered: " + username);
    }

    bool loginUser(const std::string& username, const std::string& password) {
        if (users[username] == password) {
            logs.push_back("User logged in: " + username);
            return true;
        }
        return false;
    }

    // Inventory management
    void addProduct(int id, double price) {
        inventory[id] = price;
        logs.push_back("Product added: " + std::to_string(id));
    }

    double getProductPrice(int id) {
        return inventory[id];
    }

    // Order processing
    void processOrder(int productId, const std::string& username) {
        double price = inventory[productId];
        logs.push_back("Order processed for " + username);
        // ... more logic
    }

    // Reporting
    void generateReport() {
        std::println("Total users: {}", users.size());
        std::println("Total products: {}", inventory.size());
    }

    // Logging
    void printLogs() {
        for (const auto& log : logs) {
            std::println("{}", log);
        }
    }

    // Configuration
    void loadConfig() { /* ... */ }
    void saveConfig() { /* ... */ }

    // Email
    void sendEmail(const std::string& to, const std::string& message) { /* ... */ }

    // File operations
    void saveToFile() { /* ... */ }
    void loadFromFile() { /* ... */ }

    // ... 50+ more methods

    // 🚨 PROBLEMS:
    // • One class doing EVERYTHING
    // • 1000+ lines of code
    // • Hard to understand
    // • Hard to test
    // • Hard to maintain
    // • Changes affect everything
    // • Multiple people can't work on it
};
}

namespace good_example6 {
// ✅ GOOD: Separated into focused classes

class UserManager {
private:
    std::map<std::string, std::string> users;
public:
    void registerUser(const std::string& username, const std::string& password) {
        users[username] = password;
    }

    bool authenticate(const std::string& username, const std::string& password) {
        return users.count(username) && users[username] == password;
    }

    int getUserCount() const { return users.size(); }
};

class InventoryManager {
private:
    std::map<int, double> inventory;
public:
    void addProduct(int id, double price) {
        inventory[id] = price;
    }

    double getPrice(int id) const {
        return inventory.count(id) ? inventory.at(id) : 0;
    }

    int getProductCount() const { return inventory.size(); }
};

class OrderProcessor {
private:
    InventoryManager& inventory;
public:
    OrderProcessor(InventoryManager& inv) : inventory(inv) {}

    bool processOrder(int productId, const std::string& username) {
        double price = inventory.getPrice(productId);
        if (price > 0) {
            std::println("Processing order for {}", username);
            return true;
        }
        return false;
    }
};

class ReportGenerator {
private:
    UserManager& userManager;
    InventoryManager& inventoryManager;
public:
    ReportGenerator(UserManager& um, InventoryManager& im)
        : userManager(um), inventoryManager(im) {}

    void generate() {
        std::println("");
        std::println("=== System Report ===");
        std::println("Total users: {}", userManager.getUserCount());
        std::println("Total products: {}", inventoryManager.getProductCount());
    }
};

class Logger {
private:
    std::vector<std::string> logs;
public:
    void log(const std::string& message) {
        logs.push_back(message);
    }

    void printAll() const {
        for (const auto& log : logs) {
            std::println("[LOG] {}", log);
        }
    }
};
}

void example6_god_class() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 6: GOD CLASS ANTI-PATTERN               ║");
    std::println("╚═══════════════════════════════════════════════════╝");

    std::println("    ");
    std::println("    ");
    std::println("    ");
    std::println("    ");

    good_example6::UserManager userMgr;
    good_example6::InventoryManager invMgr;
    good_example6::OrderProcessor orderProc(invMgr);
    good_example6::ReportGenerator reporter(userMgr, invMgr);
    good_example6::Logger logger;

    userMgr.registerUser("john", "pass123");
    logger.log("User registered: john");

    invMgr.addProduct(1, 29.99);
    logger.log("Product added: 1");

    orderProc.processOrder(1, "john");
    logger.log("Order processed");

    reporter.generate();

    std::println("");
    logger.printAll();

}

// ============================================
// EXAMPLE 7: CROSS-CUTTING CONCERNS
// ============================================

namespace example7 {
// Cross-cutting concerns: Apply across multiple parts

// Examples: Logging, Security, Caching, Transactions

// Pattern: Aspect-Oriented Programming (AOP) style

class SecurityAspect {
public:
    static bool checkPermission(const std::string& user, const std::string& action) {
        std::println("[SECURITY] Checking {} for {}", user, action);
        return true;  // Simplified
    }
};

class CacheAspect {
private:
    static std::map<std::string, std::string> cache;
public:
    static bool hasCache(const std::string& key) {
        return cache.count(key) > 0;
    }

    static std::string getCache(const std::string& key) {
        std::println("[CACHE] Hit for {}", key);
        return cache[key];
    }

    static void putCache(const std::string& key, const std::string& value) {
        std::println("[CACHE] Storing {}", key);
        cache[key] = value;
    }
};

std::map<std::string, std::string> CacheAspect::cache;

class LoggingAspect {
public:
    static void logEntry(const std::string& method) {
        std::println("[TRACE] Entering {}", method);
    }

    static void logExit(const std::string& method) {
        std::println("[TRACE] Exiting {}", method);
    }
};

// Business service using cross-cutting concerns
class DataService {
public:
    std::string getData(const std::string& user, const std::string& key) {
        LoggingAspect::logEntry("getData");

        // Security concern
        if (!SecurityAspect::checkPermission(user, "read")) {
            LoggingAspect::logExit("getData");
            return "Access denied";
        }

        // Caching concern
        if (CacheAspect::hasCache(key)) {
            std::string result = CacheAspect::getCache(key);
            LoggingAspect::logExit("getData");
            return result;
        }

        // Business logic
        std::string data = "Data for " + key;

        // Cache result
        CacheAspect::putCache(key, data);

        LoggingAspect::logExit("getData");
        return data;
    }
};
}

void example7_cross_cutting() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 7: CROSS-CUTTING CONCERNS               ║");
    std::println("╚═══════════════════════════════════════════════════╝");

    example7::DataService service;

    std::println("");
    std::print("--- First call (no cache) ---");
    std::string result1 = service.getData("john", "user:123");
    std::println("Result: {}", result1);

    std::println("");
    std::print("--- Second call (cached) ---");
    std::string result2 = service.getData("john", "user:123");
    std::println("Result: {}", result2);

}

// ============================================
// EXAMPLE 8: IDENTIFYING CONCERNS
// ============================================

void example8_identifying_concerns() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 8: IDENTIFYING CONCERNS                 ║");
    std::println("╚═══════════════════════════════════════════════════╝");

    std::println("   ");

    std::println("   ");

    std::println("   ");
    std::println("   ");

}

// ============================================
// EXAMPLE 9: BENEFITS & BEST PRACTICES
// ============================================

void example9_benefits() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 9: BENEFITS & BEST PRACTICES            ║");
    std::println("╚═══════════════════════════════════════════════════╝");

}

// ============================================
// EXAMPLE 10: SUMMARY
// ============================================

void example10_summary() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 10: SUMMARY                             ║");
    std::println("╚═══════════════════════════════════════════════════╝");

    std::println("   ═════════════════════════════════");

    std::println("   ════════════════════════════");

}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    std::println("╔═══════════════════════════════════════════════════════╗");
    std::println("║                                                       ║");
    std::println("║          SEPARATION OF CONCERNS PRINCIPLE             ║");
    std::println("║                                                       ║");
    std::println("║  \"Separate different concerns into distinct          ║");
    std::println("║   sections. Each module should have a single,         ║");
    std::println("║   well-defined responsibility.\"                      ║");
    std::println("║                                                       ║");
    std::println("╚═══════════════════════════════════════════════════════╝");

    example1_mixed_concerns();
    example2_data_access();
    example3_reporting();
    example4_logging();
    example5_layered_architecture();
    example6_god_class();
    example7_cross_cutting();
    example8_identifying_concerns();
    example9_benefits();
    example10_summary();

    return 0;
}
