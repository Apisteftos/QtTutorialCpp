#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <memory>
#include <map>
using namespace std;

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
        cout << "=== Create User ===" << endl;
        cout << "Enter username: ";
        string username;
        cin >> username;

        cout << "Enter email: ";
        string email;
        cin >> email;

        cout << "Enter password: ";
        string password;
        cin >> password;

        // Validation mixed in
        if (username.empty()) {
            cout << "Error: Username required!" << endl;
            return;
        }

        if (email.find("@") == string::npos) {
            cout << "Error: Invalid email!" << endl;
            return;
        }

        if (password.length() < 8) {
            cout << "Error: Password too short!" << endl;
            return;
        }

        // Database logic mixed in
        cout << "Saving to database..." << endl;
        // Imagine actual database code here

        // Logging mixed in
        cout << "User created successfully!" << endl;
        cout << "Username: " << username << endl;
        cout << "Email: " << email << endl;
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
    string username;
    string email;
    string password;

    User(const string& u, const string& e, const string& p)
        : username(u), email(e), password(p) {}
};

// CONCERN 2: Validation (business rules)
class UserValidator {
public:
    static bool validateUsername(const string& username) {
        return !username.empty() && username.length() >= 3;
    }

    static bool validateEmail(const string& email) {
        return email.find("@") != string::npos &&
               email.find(".") != string::npos;
    }

    static bool validatePassword(const string& password) {
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
    User createUser(const string& username, const string& email,
                    const string& password) {
        User user(username, email, password);

        if (!UserValidator::validateUser(user)) {
            throw runtime_error("Invalid user data");
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
        cout << "[DB] Saving user: " << user.username << endl;
    }
};

// CONCERN 5: User interface
class UserUI {
private:
    UserService service;
    UserRepository repository;

public:
    void createUserInterface() {
        cout << "\n=== Create User ===" << endl;

        string username, email, password;

        cout << "Enter username: ";
        cin >> username;

        cout << "Enter email: ";
        cin >> email;

        cout << "Enter password: ";
        cin >> password;

        try {
            // Use services
            User user = service.createUser(username, email, password);
            repository.save(user);

            cout << "\n✅ User created successfully!" << endl;
            cout << "Username: " << user.username << endl;
            cout << "Email: " << user.email << endl;

        } catch (const exception& e) {
            cout << "\n❌ Error: " << e.what() << endl;
        }
    }
};
}

void example1_mixed_concerns() {
    cout << "\n╔═══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 1: MIXED UI AND BUSINESS LOGIC          ║\n";
    cout << "╚═══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (Everything mixed):\n";
    cout << "```cpp\n";
    cout << "class UserManager {\n";
    cout << "    void createUser() {\n";
    cout << "        // UI code\n";
    cout << "        cout << \"Enter username: \";\n";
    cout << "        cin >> username;\n";
    cout << "        \n";
    cout << "        // Validation code\n";
    cout << "        if (username.empty()) { ... }\n";
    cout << "        \n";
    cout << "        // Database code\n";
    cout << "        saveToDatabase(...);\n";
    cout << "        \n";
    cout << "        // Logging code\n";
    cout << "        cout << \"User created\";\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "// 🚨 Everything tangled together!\n";
    cout << "```\n";

    cout << "\n🚨 PROBLEMS:\n";
    cout << "   • Can't test business logic without UI\n";
    cout << "   • Can't reuse validation elsewhere\n";
    cout << "   • Can't change UI without touching business logic\n";
    cout << "   • Hard to maintain and understand\n";
    cout << "   • Different concerns all mixed together\n";

    cout << "\n✅ GOOD CODE (Separated concerns):\n";
    cout << "```cpp\n";
    cout << "// Concern 1: Data\n";
    cout << "struct User { string username, email, password; };\n";
    cout << "\n";
    cout << "// Concern 2: Validation\n";
    cout << "class UserValidator { bool validate(...); };\n";
    cout << "\n";
    cout << "// Concern 3: Business logic\n";
    cout << "class UserService { User createUser(...); };\n";
    cout << "\n";
    cout << "// Concern 4: Database\n";
    cout << "class UserRepository { void save(User); };\n";
    cout << "\n";
    cout << "// Concern 5: UI\n";
    cout << "class UserUI { void createUserInterface(); };\n";
    cout << "```\n";

    cout << "\n✨ BENEFITS:\n";
    cout << "   • Each class has ONE clear responsibility\n";
    cout << "   • Can test each concern independently\n";
    cout << "   • Can reuse validation, business logic, etc.\n";
    cout << "   • Changes isolated to relevant concern\n";
    cout << "   • Much easier to understand and maintain\n";
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
        cout << "SELECT * FROM orders WHERE id = " << orderId << endl;
        // Imagine fetching order from database

        double total = 100.0;  // Pretend we got this from DB

        // Business logic
        if (total > 1000) {
            total *= 0.9;  // 10% discount
            cout << "Discount applied!" << endl;
        }

        // 🚨 More database concerns
        cout << "UPDATE orders SET total = " << total
             << " WHERE id = " << orderId << endl;

        // 🚨 Email concerns mixed in
        cout << "Sending email confirmation..." << endl;
        cout << "To: customer@example.com" << endl;
        cout << "Subject: Order confirmed" << endl;

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
    string customerEmail;

    Order(int i, double t, const string& email)
        : id(i), total(t), customerEmail(email) {}
};

// CONCERN 2: Data access
class OrderRepository {
public:
    Order findById(int orderId) {
        cout << "[DB] Fetching order " << orderId << endl;
        // Database access only
        return Order(orderId, 100.0, "customer@example.com");
    }

    void update(const Order& order) {
        cout << "[DB] Updating order " << order.id
             << " with total " << order.total << endl;
    }
};

// CONCERN 3: Business logic
class OrderService {
public:
    void applyDiscount(Order& order) {
        // Pure business logic
        if (order.total > 1000) {
            order.total *= 0.9;  // 10% discount
            cout << "[BUSINESS] Discount applied: $"
                 << order.total << endl;
        }
    }
};

// CONCERN 4: Notification
class EmailService {
public:
    void sendOrderConfirmation(const Order& order) {
        // Email concerns only
        cout << "[EMAIL] Sending confirmation to: "
             << order.customerEmail << endl;
        cout << "[EMAIL] Order #" << order.id
             << " - Total: $" << order.total << endl;
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
    cout << "\n╔═══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 2: DATA ACCESS IN BUSINESS LOGIC        ║\n";
    cout << "╚═══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (Mixed concerns):\n";
    cout << "   Single class doing:\n";
    cout << "   • Database queries\n";
    cout << "   • Business logic (discount calculation)\n";
    cout << "   • Email sending\n";
    cout << "   🚨 Can't test discount logic without database!\n";

    cout << "\n✅ GOOD CODE (Separated concerns):\n";
    cout << "   • OrderRepository: Database access\n";
    cout << "   • OrderService: Business logic\n";
    cout << "   • EmailService: Notifications\n";
    cout << "   • OrderProcessor: Orchestrates the flow\n";

    cout << "\n✨ DEMONSTRATION:\n";

    good_example2::OrderProcessor processor;
    processor.processOrder(123);

    cout << "\n✨ BENEFITS:\n";
    cout << "   • Can test business logic with mock repository\n";
    cout << "   • Can reuse OrderService for other operations\n";
    cout << "   • Can change database without touching business logic\n";
    cout << "   • Can change email service independently\n";
    cout << "   • Each concern clear and focused\n";
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
        cout << "Connecting to database..." << endl;
        vector<double> sales = {100, 200, 150, 300, 250};

        // 🚨 Business logic (calculations)
        double total = 0;
        for (double sale : sales) {
            total += sale;
        }
        double average = total / sales.size();

        // 🚨 Formatting/presentation
        cout << "\n===== SALES REPORT =====" << endl;
        cout << "Total Sales: $" << total << endl;
        cout << "Average Sale: $" << average << endl;
        cout << "Number of Sales: " << sales.size() << endl;
        cout << "========================" << endl;

        // 🚨 File writing
        cout << "Writing to file..." << endl;
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
    vector<double> sales;
    double total;
    double average;
    int count;
};

// CONCERN 2: Data access
class SalesRepository {
public:
    vector<double> fetchSales() {
        cout << "[DB] Fetching sales data..." << endl;
        return {100, 200, 150, 300, 250};
    }
};

// CONCERN 3: Business logic (calculations)
class SalesCalculator {
public:
    SalesData calculate(const vector<double>& sales) {
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
    string formatConsole(const SalesData& data) {
        stringstream ss;
        ss << "\n===== SALES REPORT =====\n";
        ss << "Total Sales: $" << data.total << "\n";
        ss << "Average Sale: $" << data.average << "\n";
        ss << "Number of Sales: " << data.count << "\n";
        ss << "========================\n";
        return ss.str();
    }

    string formatHTML(const SalesData& data) {
        stringstream ss;
        ss << "<div class='report'>\n";
        ss << "  <h2>Sales Report</h2>\n";
        ss << "  <p>Total: $" << data.total << "</p>\n";
        ss << "  <p>Average: $" << data.average << "</p>\n";
        ss << "  <p>Count: " << data.count << "</p>\n";
        ss << "</div>\n";
        return ss.str();
    }

    string formatJSON(const SalesData& data) {
        stringstream ss;
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
    void writeToFile(const string& content, const string& filename) {
        cout << "[FILE] Writing report to " << filename << endl;
        // File writing logic
    }

    void writeToConsole(const string& content) {
        cout << content;
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
        vector<double> sales = repository.fetchSales();
        SalesData data = calculator.calculate(sales);
        string report = formatter.formatConsole(data);
        writer.writeToConsole(report);
    }

    void generateHTMLReport() {
        vector<double> sales = repository.fetchSales();
        SalesData data = calculator.calculate(sales);
        string report = formatter.formatHTML(data);
        writer.writeToFile(report, "report.html");
    }

    void generateJSONReport() {
        vector<double> sales = repository.fetchSales();
        SalesData data = calculator.calculate(sales);
        string report = formatter.formatJSON(data);
        writer.writeToFile(report, "report.json");
    }
};
}

void example3_reporting() {
    cout << "\n╔═══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 3: REPORTING WITH MIXED CONCERNS        ║\n";
    cout << "╚═══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE:\n";
    cout << "   One method doing:\n";
    cout << "   • Fetching data from database\n";
    cout << "   • Calculating totals and averages\n";
    cout << "   • Formatting output\n";
    cout << "   • Writing to file\n";
    cout << "   🚨 Impossible to reuse or test separately!\n";

    cout << "\n✅ GOOD CODE:\n";
    cout << "   • SalesRepository: Data access\n";
    cout << "   • SalesCalculator: Business logic\n";
    cout << "   • ReportFormatter: Presentation (Console, HTML, JSON)\n";
    cout << "   • ReportWriter: File I/O\n";
    cout << "   • SalesReportGenerator: Orchestration\n";

    cout << "\n✨ DEMONSTRATION:\n";

    good_example3::SalesReportGenerator generator;

    cout << "\n--- Console Report ---";
    generator.generateConsoleReport();

    cout << "\n--- HTML Report ---";
    generator.generateHTMLReport();

    cout << "\n--- JSON Report ---";
    generator.generateJSONReport();

    cout << "\n✨ BENEFITS:\n";
    cout << "   • Can test calculations without database\n";
    cout << "   • Can easily add new formats (XML, CSV)\n";
    cout << "   • Can reuse calculator for other reports\n";
    cout << "   • Can change data source without affecting calculations\n";
    cout << "   • Each component has single, clear purpose\n";
}

// ============================================
// EXAMPLE 4: LOGGING MIXED EVERYWHERE
// ============================================

namespace bad_example4 {
// ❌ BAD: Logging mixed throughout business logic

class PaymentProcessor {
public:
    bool processPayment(double amount) {
        cout << "[LOG] Starting payment processing" << endl;
        cout << "[LOG] Amount: $" << amount << endl;

        // Business logic
        if (amount <= 0) {
            cout << "[LOG] Invalid amount: " << amount << endl;
            return false;
        }

        cout << "[LOG] Validating payment method" << endl;
        // Validation logic

        cout << "[LOG] Contacting payment gateway" << endl;
        // Payment processing

        cout << "[LOG] Payment successful" << endl;
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
    virtual void log(const string& message) = 0;
    virtual ~Logger() = default;
};

class ConsoleLogger : public Logger {
public:
    void log(const string& message) override {
        cout << "[LOG] " << message << endl;
    }
};

class FileLogger : public Logger {
public:
    void log(const string& message) override {
        // Write to file
        cout << "[FILE LOG] " << message << endl;
    }
};

class NullLogger : public Logger {
public:
    void log(const string& message) override {
        // Do nothing - for when logging disabled
    }
};

// CONCERN 2: Pure business logic
class PaymentProcessor {
private:
    shared_ptr<Logger> logger;

public:
    PaymentProcessor(shared_ptr<Logger> log) : logger(log) {}

    bool processPayment(double amount) {
        logger->log("Starting payment processing");
        logger->log("Amount: $" + to_string(amount));

        // Pure business logic - clean and clear
        if (amount <= 0) {
            logger->log("Invalid amount: " + to_string(amount));
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
    cout << "\n╔═══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 4: LOGGING MIXED EVERYWHERE             ║\n";
    cout << "╚═══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE:\n";
    cout << "   • cout << \"[LOG]\" scattered everywhere\n";
    cout << "   • Hard to disable logging\n";
    cout << "   • Hard to change output destination\n";
    cout << "   • Business logic obscured\n";

    cout << "\n✅ GOOD CODE:\n";
    cout << "   • Logger interface for logging concern\n";
    cout << "   • Multiple implementations (Console, File, Null)\n";
    cout << "   • Business logic clean and focused\n";
    cout << "   • Easy to swap logging strategies\n";

    cout << "\n✨ DEMONSTRATION:\n";

    cout << "\n--- With Console Logger ---";
    auto consoleLogger = make_shared<good_example4::ConsoleLogger>();
    good_example4::PaymentProcessor processor1(consoleLogger);
    processor1.processPayment(100.50);

    cout << "\n--- With Null Logger (no output) ---";
    auto nullLogger = make_shared<good_example4::NullLogger>();
    good_example4::PaymentProcessor processor2(nullLogger);
    processor2.processPayment(100.50);
    cout << "(Business logic executed, but no logs)";

    cout << "\n\n✨ BENEFITS:\n";
    cout << "   • Can disable logging easily\n";
    cout << "   • Can log to file, console, or network\n";
    cout << "   • Business logic stays clean\n";
    cout << "   • Can test without logging noise\n";
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
    string name;
    double price;
    int stock;

    Product(int i, const string& n, double p, int s)
        : id(i), name(n), price(p), stock(s) {}
};
}

// LAYER 2: Data Access Layer (Repositories)
namespace data {
using namespace domain;

class ProductRepository {
public:
    Product findById(int id) {
        cout << "[DATA LAYER] Fetching product " << id << endl;
        return Product(id, "Sample Product", 29.99, 100);
    }

    void save(const Product& product) {
        cout << "[DATA LAYER] Saving product " << product.id << endl;
    }

    vector<Product> findAll() {
        cout << "[DATA LAYER] Fetching all products" << endl;
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
        cout << "[BUSINESS LAYER] Processing purchase" << endl;

        Product product = repository.findById(productId);

        // Business logic
        if (product.stock < quantity) {
            cout << "[BUSINESS LAYER] Insufficient stock" << endl;
            return false;
        }

        product.stock -= quantity;
        repository.save(product);

        cout << "[BUSINESS LAYER] Purchase successful" << endl;
        return true;
    }

    double calculateTotalValue() {
        cout << "[BUSINESS LAYER] Calculating inventory value" << endl;

        vector<Product> products = repository.findAll();
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
        cout << "\n[PRESENTATION LAYER] Displaying product" << endl;

        Product product = service.getProduct(id);

        cout << "╔════════════════════════════╗" << endl;
        cout << "║ Product Details            ║" << endl;
        cout << "╠════════════════════════════╣" << endl;
        cout << "║ ID: " << product.id << "                      ║" << endl;
        cout << "║ Name: " << product.name << "     ║" << endl;
        cout << "║ Price: $" << product.price << "            ║" << endl;
        cout << "║ Stock: " << product.stock << " units          ║" << endl;
        cout << "╚════════════════════════════╝" << endl;
    }

    void handlePurchase(int productId, int quantity) {
        cout << "\n[PRESENTATION LAYER] Handling purchase request" << endl;

        bool success = service.purchaseProduct(productId, quantity);

        if (success) {
            cout << "✅ Purchase completed successfully!" << endl;
        } else {
            cout << "❌ Purchase failed - insufficient stock" << endl;
        }
    }

    void displayInventoryValue() {
        cout << "\n[PRESENTATION LAYER] Displaying inventory value" << endl;

        double total = service.calculateTotalValue();

        cout << "╔════════════════════════════╗" << endl;
        cout << "║ Inventory Value            ║" << endl;
        cout << "╠════════════════════════════╣" << endl;
        cout << "║ Total: $" << total << "         ║" << endl;
        cout << "╚════════════════════════════╝" << endl;
    }
};
}
}

void example5_layered_architecture() {
    cout << "\n╔═══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 5: LAYERED ARCHITECTURE                 ║\n";
    cout << "╚═══════════════════════════════════════════════════╝\n";

    cout << "\n✅ LAYERED ARCHITECTURE:\n";
    cout << "   Layer 4: Presentation (UI/Controllers)\n";
    cout << "              ↓ uses\n";
    cout << "   Layer 3: Business Logic (Services)\n";
    cout << "              ↓ uses\n";
    cout << "   Layer 2: Data Access (Repositories)\n";
    cout << "              ↓ uses\n";
    cout << "   Layer 1: Domain (Models/Entities)\n";

    cout << "\n🔒 RULES:\n";
    cout << "   • Each layer only depends on layers below\n";
    cout << "   • No skipping layers\n";
    cout << "   • Clear separation of concerns\n";

    cout << "\n✨ DEMONSTRATION:\n";

    good_example5::presentation::ProductController controller;

    controller.displayProduct(1);
    controller.handlePurchase(1, 5);
    controller.displayInventoryValue();

    cout << "\n✨ BENEFITS:\n";
    cout << "   • Clear organization\n";
    cout << "   • Easy to test each layer\n";
    cout << "   • Can replace layers (e.g., swap database)\n";
    cout << "   • Changes isolated to relevant layer\n";
    cout << "   • Standard pattern, easy to understand\n";
}

// ============================================
// EXAMPLE 6: GOD CLASS ANTI-PATTERN
// ============================================

namespace bad_example6 {
// ❌ BAD: God class doing everything

class Application {
private:
    map<string, string> users;
    map<int, double> inventory;
    vector<string> logs;

public:
    // User management
    void registerUser(const string& username, const string& password) {
        users[username] = password;
        logs.push_back("User registered: " + username);
    }

    bool loginUser(const string& username, const string& password) {
        if (users[username] == password) {
            logs.push_back("User logged in: " + username);
            return true;
        }
        return false;
    }

    // Inventory management
    void addProduct(int id, double price) {
        inventory[id] = price;
        logs.push_back("Product added: " + to_string(id));
    }

    double getProductPrice(int id) {
        return inventory[id];
    }

    // Order processing
    void processOrder(int productId, const string& username) {
        double price = inventory[productId];
        logs.push_back("Order processed for " + username);
        // ... more logic
    }

    // Reporting
    void generateReport() {
        cout << "Total users: " << users.size() << endl;
        cout << "Total products: " << inventory.size() << endl;
    }

    // Logging
    void printLogs() {
        for (const auto& log : logs) {
            cout << log << endl;
        }
    }

    // Configuration
    void loadConfig() { /* ... */ }
    void saveConfig() { /* ... */ }

    // Email
    void sendEmail(const string& to, const string& message) { /* ... */ }

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
    map<string, string> users;
public:
    void registerUser(const string& username, const string& password) {
        users[username] = password;
    }

    bool authenticate(const string& username, const string& password) {
        return users.count(username) && users[username] == password;
    }

    int getUserCount() const { return users.size(); }
};

class InventoryManager {
private:
    map<int, double> inventory;
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

    bool processOrder(int productId, const string& username) {
        double price = inventory.getPrice(productId);
        if (price > 0) {
            cout << "Processing order for " << username << endl;
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
        cout << "\n=== System Report ===" << endl;
        cout << "Total users: " << userManager.getUserCount() << endl;
        cout << "Total products: " << inventoryManager.getProductCount() << endl;
    }
};

class Logger {
private:
    vector<string> logs;
public:
    void log(const string& message) {
        logs.push_back(message);
    }

    void printAll() const {
        for (const auto& log : logs) {
            cout << "[LOG] " << log << endl;
        }
    }
};
}

void example6_god_class() {
    cout << "\n╔═══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 6: GOD CLASS ANTI-PATTERN               ║\n";
    cout << "╚═══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (God Class):\n";
    cout << "```cpp\n";
    cout << "class Application {\n";
    cout << "    // User management\n";
    cout << "    void registerUser();\n";
    cout << "    void loginUser();\n";
    cout << "    \n";
    cout << "    // Inventory\n";
    cout << "    void addProduct();\n";
    cout << "    \n";
    cout << "    // Orders\n";
    cout << "    void processOrder();\n";
    cout << "    \n";
    cout << "    // Reporting\n";
    cout << "    void generateReport();\n";
    cout << "    \n";
    cout << "    // Logging, Email, Files...\n";
    cout << "    // ... 50+ methods\n";
    cout << "};\n";
    cout << "// 🚨 One class doing EVERYTHING!\n";
    cout << "```\n";

    cout << "\n🚨 PROBLEMS:\n";
    cout << "   • 1000+ lines in one class\n";
    cout << "   • Hard to understand what it does\n";
    cout << "   • Impossible to test properly\n";
    cout << "   • Multiple people can't work on it\n";
    cout << "   • Changes affect everything\n";
    cout << "   • Violates Single Responsibility Principle\n";

    cout << "\n✅ GOOD CODE (Separated classes):\n";
    cout << "   • UserManager: User operations only\n";
    cout << "   • InventoryManager: Inventory operations only\n";
    cout << "   • OrderProcessor: Order processing only\n";
    cout << "   • ReportGenerator: Reporting only\n";
    cout << "   • Logger: Logging only\n";

    cout << "\n✨ DEMONSTRATION:\n";

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

    cout << "\n";
    logger.printAll();

    cout << "\n✨ BENEFITS:\n";
    cout << "   • Each class focused on one concern\n";
    cout << "   • Easy to understand each component\n";
    cout << "   • Easy to test independently\n";
    cout << "   • Multiple people can work in parallel\n";
    cout << "   • Changes isolated to relevant class\n";
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
    static bool checkPermission(const string& user, const string& action) {
        cout << "[SECURITY] Checking " << user << " for " << action << endl;
        return true;  // Simplified
    }
};

class CacheAspect {
private:
    static map<string, string> cache;
public:
    static bool hasCache(const string& key) {
        return cache.count(key) > 0;
    }

    static string getCache(const string& key) {
        cout << "[CACHE] Hit for " << key << endl;
        return cache[key];
    }

    static void putCache(const string& key, const string& value) {
        cout << "[CACHE] Storing " << key << endl;
        cache[key] = value;
    }
};

map<string, string> CacheAspect::cache;

class LoggingAspect {
public:
    static void logEntry(const string& method) {
        cout << "[TRACE] Entering " << method << endl;
    }

    static void logExit(const string& method) {
        cout << "[TRACE] Exiting " << method << endl;
    }
};

// Business service using cross-cutting concerns
class DataService {
public:
    string getData(const string& user, const string& key) {
        LoggingAspect::logEntry("getData");

        // Security concern
        if (!SecurityAspect::checkPermission(user, "read")) {
            LoggingAspect::logExit("getData");
            return "Access denied";
        }

        // Caching concern
        if (CacheAspect::hasCache(key)) {
            string result = CacheAspect::getCache(key);
            LoggingAspect::logExit("getData");
            return result;
        }

        // Business logic
        string data = "Data for " + key;

        // Cache result
        CacheAspect::putCache(key, data);

        LoggingAspect::logExit("getData");
        return data;
    }
};
}

void example7_cross_cutting() {
    cout << "\n╔═══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 7: CROSS-CUTTING CONCERNS               ║\n";
    cout << "╚═══════════════════════════════════════════════════╝\n";

    cout << "\n📚 CROSS-CUTTING CONCERNS:\n";
    cout << "   Concerns that span multiple modules:\n";
    cout << "   • Logging\n";
    cout << "   • Security/Authentication\n";
    cout << "   • Caching\n";
    cout << "   • Error handling\n";
    cout << "   • Transactions\n";
    cout << "   • Monitoring\n";

    cout << "\n✅ SOLUTION: Aspect-Oriented approach\n";
    cout << "   • Separate aspect classes\n";
    cout << "   • Applied consistently across system\n";
    cout << "   • Don't duplicate in every method\n";

    cout << "\n✨ DEMONSTRATION:\n";

    example7::DataService service;

    cout << "\n--- First call (no cache) ---";
    string result1 = service.getData("john", "user:123");
    cout << "Result: " << result1 << "\n";

    cout << "\n--- Second call (cached) ---";
    string result2 = service.getData("john", "user:123");
    cout << "Result: " << result2 << "\n";

    cout << "\n✨ BENEFITS:\n";
    cout << "   • Logging consistent everywhere\n";
    cout << "   • Security applied uniformly\n";
    cout << "   • Caching reusable\n";
    cout << "   • Easy to modify cross-cutting logic\n";
    cout << "   • Business logic stays clean\n";
}

// ============================================
// EXAMPLE 8: IDENTIFYING CONCERNS
// ============================================

void example8_identifying_concerns() {
    cout << "\n╔═══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 8: IDENTIFYING CONCERNS                 ║\n";
    cout << "╚═══════════════════════════════════════════════════╝\n";

    cout << "\n🔍 HOW TO IDENTIFY CONCERNS:\n";

    cout << "\n1. LOOK FOR DIFFERENT RESPONSIBILITIES:\n";
    cout << "   ❌ Bad: class UserManager {\n";
    cout << "           void saveUser();      // Database\n";
    cout << "           void validateUser();  // Validation\n";
    cout << "           void displayUser();   // UI\n";
    cout << "           void emailUser();     // Communication\n";
    cout << "       }\n";
    cout << "   \n";
    cout << "   ✅ Good: Separate classes for each concern\n";

    cout << "\n2. APPLY THE 'REASON TO CHANGE' TEST:\n";
    cout << "   Ask: Why would this class need to change?\n";
    cout << "   • Change database? → Data access concern\n";
    cout << "   • Change validation rules? → Validation concern\n";
    cout << "   • Change UI? → Presentation concern\n";
    cout << "   • Change email format? → Communication concern\n";
    cout << "   \n";
    cout << "   If multiple reasons → Multiple concerns → Separate!\n";

    cout << "\n3. IDENTIFY TECHNICAL VS BUSINESS CONCERNS:\n";
    cout << "   Technical concerns:\n";
    cout << "   • Database access\n";
    cout << "   • File I/O\n";
    cout << "   • Network communication\n";
    cout << "   • Caching\n";
    cout << "   • Logging\n";
    cout << "   \n";
    cout << "   Business concerns:\n";
    cout << "   • Business rules\n";
    cout << "   • Calculations\n";
    cout << "   • Workflows\n";
    cout << "   • Domain logic\n";
    cout << "   \n";
    cout << "   Keep them separate!\n";

    cout << "\n4. USE VERB ANALYSIS:\n";
    cout << "   Different verbs often indicate different concerns:\n";
    cout << "   • Validate → Validation concern\n";
    cout << "   • Calculate → Business logic concern\n";
    cout << "   • Save/Load → Data access concern\n";
    cout << "   • Display/Show → Presentation concern\n";
    cout << "   • Send/Notify → Communication concern\n";

    cout << "\n5. WATCH FOR MIXED ABSTRACTIONS:\n";
    cout << "   ❌ class ReportGenerator {\n";
    cout << "           void connectToDatabase();  // Low-level\n";
    cout << "           void generateReport();     // High-level\n";
    cout << "       }\n";
    cout << "   Different levels of abstraction = Different concerns\n";
}

// ============================================
// EXAMPLE 9: BENEFITS & BEST PRACTICES
// ============================================

void example9_benefits() {
    cout << "\n╔═══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 9: BENEFITS & BEST PRACTICES            ║\n";
    cout << "╚═══════════════════════════════════════════════════╝\n";

    cout << "\n✅ BENEFITS OF SEPARATION OF CONCERNS:\n";

    cout << "\n1. MAINTAINABILITY:\n";
    cout << "   • Changes isolated to one concern\n";
    cout << "   • Don't break unrelated functionality\n";
    cout << "   • Easy to find where to make changes\n";

    cout << "\n2. TESTABILITY:\n";
    cout << "   • Test each concern independently\n";
    cout << "   • Mock dependencies easily\n";
    cout << "   • Smaller, focused tests\n";

    cout << "\n3. REUSABILITY:\n";
    cout << "   • Concerns can be reused\n";
    cout << "   • Validation logic reusable\n";
    cout << "   • Data access reusable\n";

    cout << "\n4. UNDERSTANDABILITY:\n";
    cout << "   • Each module has clear purpose\n";
    cout << "   • Easier to learn codebase\n";
    cout << "   • Less cognitive load\n";

    cout << "\n5. PARALLEL DEVELOPMENT:\n";
    cout << "   • Different people work on different concerns\n";
    cout << "   • Less merge conflicts\n";
    cout << "   • Faster development\n";

    cout << "\n6. FLEXIBILITY:\n";
    cout << "   • Swap implementations easily\n";
    cout << "   • Change database without affecting business logic\n";
    cout << "   • Change UI without affecting data access\n";

    cout << "\n🎯 BEST PRACTICES:\n";

    cout << "\n1. SINGLE RESPONSIBILITY PRINCIPLE:\n";
    cout << "   Each class should have one reason to change\n";

    cout << "\n2. HIGH COHESION:\n";
    cout << "   Keep related things together\n";

    cout << "\n3. LOW COUPLING:\n";
    cout << "   Minimize dependencies between concerns\n";

    cout << "\n4. CLEAR BOUNDARIES:\n";
    cout << "   Define interfaces between concerns\n";

    cout << "\n5. CONSISTENT PATTERNS:\n";
    cout << "   Use layered architecture or similar patterns\n";

    cout << "\n6. AVOID GOD CLASSES:\n";
    cout << "   No class should do everything\n";

    cout << "\n💡 COMMON PATTERNS:\n";
    cout << "   • MVC (Model-View-Controller)\n";
    cout << "   • Layered Architecture\n";
    cout << "   • Repository Pattern\n";
    cout << "   • Service Layer Pattern\n";
    cout << "   • Dependency Injection\n";
}

// ============================================
// EXAMPLE 10: SUMMARY
// ============================================

void example10_summary() {
    cout << "\n╔═══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 10: SUMMARY                             ║\n";
    cout << "╚═══════════════════════════════════════════════════╝\n";

    cout << "\n🎯 SEPARATION OF CONCERNS: KEY POINTS\n";

    cout << "\n1. WHAT IS A CONCERN?\n";
    cout << "   A distinct feature or behavior of the system\n";
    cout << "   Examples:\n";
    cout << "   • Data storage\n";
    cout << "   • Business rules\n";
    cout << "   • User interface\n";
    cout << "   • Validation\n";
    cout << "   • Communication\n";

    cout << "\n2. WHY SEPARATE?\n";
    cout << "   • Easier to understand\n";
    cout << "   • Easier to maintain\n";
    cout << "   • Easier to test\n";
    cout << "   • Easier to reuse\n";
    cout << "   • Easier to modify\n";

    cout << "\n3. HOW TO SEPARATE?\n";
    cout << "   • Identify different concerns\n";
    cout << "   • Create separate classes/modules\n";
    cout << "   • Define clear interfaces\n";
    cout << "   • Keep concerns independent\n";

    cout << "\n4. SIGNS OF GOOD SEPARATION:\n";
    cout << "   ✅ Each class has one clear purpose\n";
    cout << "   ✅ Can explain what class does in one sentence\n";
    cout << "   ✅ Can test each concern independently\n";
    cout << "   ✅ Can change one concern without affecting others\n";
    cout << "   ✅ Can reuse concerns in different contexts\n";

    cout << "\n5. SIGNS OF POOR SEPARATION:\n";
    cout << "   🚨 Classes doing many different things\n";
    cout << "   🚨 Hard to explain what class does\n";
    cout << "   🚨 Can't test without complex setup\n";
    cout << "   🚨 Changes ripple across unrelated code\n";
    cout << "   🚨 Code duplication across concerns\n";

    cout << "\n📊 BEFORE vs AFTER:\n";

    cout << "\n   WITHOUT Separation of Concerns:\n";
    cout << "   ═════════════════════════════════\n";
    cout << "   • 1 God class, 1000+ lines\n";
    cout << "   • Everything tangled together\n";
    cout << "   • Hard to understand\n";
    cout << "   • Hard to test\n";
    cout << "   • Changes break everything\n";
    cout << "   • Can't reuse anything\n";

    cout << "\n   WITH Separation of Concerns:\n";
    cout << "   ════════════════════════════\n";
    cout << "   • 10 focused classes, 100 lines each\n";
    cout << "   • Each handles one concern\n";
    cout << "   • Easy to understand\n";
    cout << "   • Easy to test\n";
    cout << "   • Changes isolated\n";
    cout << "   • High reusability\n";

    cout << "\n💭 REMEMBER:\n";
    cout << "   \"Separation of Concerns is the art of dividing\n";
    cout << "    software into distinct sections, each addressing\n";
    cout << "    a separate concern.\"\n";

    cout << "\n   Each module should:\n";
    cout << "   • Do ONE thing well\n";
    cout << "   • Have a clear responsibility\n";
    cout << "   • Be independent of other concerns\n";

    cout << "\n🎓 FINAL WISDOM:\n";
    cout << "   Good separation of concerns leads to:\n";
    cout << "   • Maintainable code\n";
    cout << "   • Testable code\n";
    cout << "   • Reusable code\n";
    cout << "   • Flexible code\n";
    cout << "   • Professional code\n";
}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║          SEPARATION OF CONCERNS PRINCIPLE             ║\n";
    cout << "║                                                       ║\n";
    cout << "║  \"Separate different concerns into distinct           ║\n";
    cout << "║   sections. Each module should have a single,         ║\n";
    cout << "║   well-defined responsibility.\"                       ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n";

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

    cout << "\n\n";
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║       🎉 SEPARATION OF CONCERNS COMPLETE! 🎉          ║\n";
    cout << "║                                                       ║\n";
    cout << "║  Key Takeaway: Break complex problems into focused   ║\n";
    cout << "║  pieces. Each piece should handle ONE concern well.  ║\n";
    cout << "║  Keep concerns separate and independent!             ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n\n";

    return 0;
}
