#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <functional>
using namespace std;

// ============================================
// LOW COUPLING PRINCIPLE
// ============================================

/*
LOW COUPLING PRINCIPLE:
    "Minimize dependencies between modules"
    "Each module should know as little as possible about others"
    — Based on principles by Larry Constantine & Edward Yourdon

WHAT IT MEANS:
    • Coupling = Degree of interdependence between modules
    • Low coupling = Modules are independent
    • High coupling = Modules are tightly connected
    • Goal: Modules can change independently

WHY IT MATTERS:
    ✅ Easier to change (changes don't ripple)
    ✅ Easier to test (fewer dependencies to mock)
    ✅ Better reusability (modules work independently)
    ✅ Easier to understand (less to think about)
    ✅ Parallel development (teams work independently)

KEY CONCEPTS:
    • Tight coupling = Bad (modules depend heavily on each other)
    • Loose coupling = Good (modules are independent)
    • Dependency = One module needs another
    • Interface = Contract between modules
    • Dependency Injection = Provide dependencies from outside

TYPES OF COUPLING (Worst to Best):
    1. Content Coupling (worst) - Module modifies another's data
    2. Common Coupling - Modules share global data
    3. Control Coupling - One module controls another's flow
    4. Stamp Coupling - Modules share data structures
    5. Data Coupling - Modules share simple data
    6. Message Coupling (best) - Modules communicate via messages

VIOLATION SIGNS:
    🚨 Changes in one class require changes in many others
    🚨 Hard to test because of many dependencies
    🚨 Can't reuse class without dragging along others
    🚨 Concrete classes used everywhere (not abstractions)
    🚨 Global variables shared between modules

GOAL: Minimize dependencies between modules!
*/

// ============================================
// EXAMPLE 1: TIGHT COUPLING WITH CONCRETE CLASSES
// ============================================

namespace bad_example1 {
// ❌ BAD: Tightly coupled to concrete classes

class MySQLDatabase {
public:
    void connect() {
        cout << "MySQL: Connected" << endl;
    }

    void query(const string& sql) {
        cout << "MySQL: Executing " << sql << endl;
    }

    void disconnect() {
        cout << "MySQL: Disconnected" << endl;
    }
};

class UserService {
private:
    MySQLDatabase database;  // 🚨 Tightly coupled to MySQL!

public:
    void saveUser(const string& username) {
        database.connect();
        database.query("INSERT INTO users VALUES ('" + username + "')");
        database.disconnect();
    }

    void getUser(const string& username) {
        database.connect();
        database.query("SELECT * FROM users WHERE name = '" + username + "'");
        database.disconnect();
    }
};

// PROBLEMS:
// • UserService MUST use MySQL (can't switch databases)
// • Can't test UserService without real MySQL
// • If MySQL class changes, UserService must change
// • Can't reuse UserService with different database
// • High coupling = High maintenance cost
}

namespace good_example1 {
// ✅ GOOD: Loosely coupled through interface

// Interface (abstraction)
class Database {
public:
    virtual ~Database() = default;
    virtual void connect() = 0;
    virtual void query(const string& sql) = 0;
    virtual void disconnect() = 0;
};

// Concrete implementations
class MySQLDatabase : public Database {
public:
    void connect() override {
        cout << "MySQL: Connected" << endl;
    }

    void query(const string& sql) override {
        cout << "MySQL: Executing " << sql << endl;
    }

    void disconnect() override {
        cout << "MySQL: Disconnected" << endl;
    }
};

class PostgreSQLDatabase : public Database {
public:
    void connect() override {
        cout << "PostgreSQL: Connected" << endl;
    }

    void query(const string& sql) override {
        cout << "PostgreSQL: Executing " << sql << endl;
    }

    void disconnect() override {
        cout << "PostgreSQL: Disconnected" << endl;
    }
};

// Service depends on interface, not concrete class
class UserService {
private:
    Database& database;  // ✅ Depends on abstraction!

public:
    UserService(Database& db) : database(db) {}

    void saveUser(const string& username) {
        database.connect();
        database.query("INSERT INTO users VALUES ('" + username + "')");
        database.disconnect();
    }

    void getUser(const string& username) {
        database.connect();
        database.query("SELECT * FROM users WHERE name = '" + username + "'");
        database.disconnect();
    }
};
}

void example1_tight_coupling() {
    cout << "\n╔═══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 1: TIGHT VS LOOSE COUPLING             ║\n";
    cout << "╚═══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (Tight Coupling):\n";
    cout << "```cpp\n";
    cout << "class UserService {\n";
    cout << "private:\n";
    cout << "    MySQLDatabase database;  // 🚨 Tightly coupled!\n";
    cout << "public:\n";
    cout << "    void saveUser(string username) {\n";
    cout << "        database.connect();\n";
    cout << "        database.query(...);\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "// 🚨 MUST use MySQL, can't switch!\n";
    cout << "```\n";

    cout << "\n🚨 PROBLEMS:\n";
    cout << "   • UserService locked to MySQL\n";
    cout << "   • Can't test without real database\n";
    cout << "   • Can't switch to PostgreSQL\n";
    cout << "   • Changes in MySQL affect UserService\n";
    cout << "   • Low reusability\n";

    cout << "\n✅ GOOD CODE (Loose Coupling):\n";
    cout << "```cpp\n";
    cout << "class Database {  // Interface\n";
    cout << "    virtual void connect() = 0;\n";
    cout << "    virtual void query(string sql) = 0;\n";
    cout << "};\n";
    cout << "\n";
    cout << "class UserService {\n";
    cout << "private:\n";
    cout << "    Database& database;  // ✅ Depends on interface!\n";
    cout << "public:\n";
    cout << "    UserService(Database& db) : database(db) {}\n";
    cout << "};\n";
    cout << "```\n";

    cout << "\n✨ DEMONSTRATION:\n";

    cout << "\n--- Using MySQL ---";
    good_example1::MySQLDatabase mysql;
    good_example1::UserService service1(mysql);
    service1.saveUser("john");

    cout << "\n--- Using PostgreSQL ---";
    good_example1::PostgreSQLDatabase postgres;
    good_example1::UserService service2(postgres);
    service2.saveUser("jane");

    cout << "\n✨ BENEFITS:\n";
    cout << "   • Can switch databases easily\n";
    cout << "   • Can test with mock database\n";
    cout << "   • UserService doesn't know about concrete database\n";
    cout << "   • Changes in database don't affect service\n";
    cout << "   • High reusability\n";
}

// ============================================
// EXAMPLE 2: DEPENDENCY INJECTION
// ============================================

namespace bad_example2 {
// ❌ BAD: Creating dependencies internally

class EmailService {
public:
    void sendEmail(const string& to, const string& message) {
        cout << "Email sent to " << to << ": " << message << endl;
    }
};

class Logger {
public:
    void log(const string& message) {
        cout << "[LOG] " << message << endl;
    }
};

class OrderService {
private:
    EmailService emailService;  // 🚨 Created internally!
    Logger logger;               // 🚨 Created internally!

public:
    void placeOrder(int orderId, const string& customerEmail) {
        logger.log("Placing order " + to_string(orderId));

        // Order processing logic
        cout << "Processing order " << orderId << endl;

        emailService.sendEmail(customerEmail, "Order confirmed");
        logger.log("Order placed successfully");
    }
};

// PROBLEMS:
// • OrderService creates its own dependencies
// • Can't test with mock EmailService or Logger
// • Can't use different implementations
// • Hard to change EmailService or Logger
// • OrderService controls everything (tight coupling)
}

namespace good_example2 {
// ✅ GOOD: Dependencies injected from outside

// Interfaces
class EmailService {
public:
    virtual ~EmailService() = default;
    virtual void sendEmail(const string& to, const string& message) = 0;
};

class Logger {
public:
    virtual ~Logger() = default;
    virtual void log(const string& message) = 0;
};

// Implementations
class SMTPEmailService : public EmailService {
public:
    void sendEmail(const string& to, const string& message) override {
        cout << "[SMTP] Email sent to " << to << ": " << message << endl;
    }
};

class ConsoleLogger : public Logger {
public:
    void log(const string& message) override {
        cout << "[LOG] " << message << endl;
    }
};

class MockEmailService : public EmailService {
public:
    void sendEmail(const string& to, const string& message) override {
        cout << "[MOCK EMAIL] Would send to " << to << endl;
    }
};

// Service with injected dependencies
class OrderService {
private:
    EmailService& emailService;  // ✅ Injected!
    Logger& logger;               // ✅ Injected!

public:
    OrderService(EmailService& email, Logger& log)
        : emailService(email), logger(log) {}

    void placeOrder(int orderId, const string& customerEmail) {
        logger.log("Placing order " + to_string(orderId));

        // Order processing logic
        cout << "Processing order " << orderId << endl;

        emailService.sendEmail(customerEmail, "Order confirmed");
        logger.log("Order placed successfully");
    }
};
}

void example2_dependency_injection() {
    cout << "\n╔═══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 2: DEPENDENCY INJECTION                 ║\n";
    cout << "╚═══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (Internal Dependencies):\n";
    cout << "```cpp\n";
    cout << "class OrderService {\n";
    cout << "private:\n";
    cout << "    EmailService emailService;  // Created here!\n";
    cout << "    Logger logger;               // Created here!\n";
    cout << "};\n";
    cout << "// 🚨 Can't change implementations!\n";
    cout << "// 🚨 Can't test with mocks!\n";
    cout << "```\n";

    cout << "\n✅ GOOD CODE (Dependency Injection):\n";
    cout << "```cpp\n";
    cout << "class OrderService {\n";
    cout << "private:\n";
    cout << "    EmailService& emailService;  // Injected!\n";
    cout << "    Logger& logger;               // Injected!\n";
    cout << "public:\n";
    cout << "    OrderService(EmailService& email, Logger& log)\n";
    cout << "        : emailService(email), logger(log) {}\n";
    cout << "};\n";
    cout << "```\n";

    cout << "\n✨ DEMONSTRATION:\n";

    cout << "\n--- Production (Real Services) ---";
    good_example2::SMTPEmailService emailService;
    good_example2::ConsoleLogger logger;
    good_example2::OrderService service1(emailService, logger);
    service1.placeOrder(123, "customer@example.com");

    cout << "\n--- Testing (Mock Services) ---";
    good_example2::MockEmailService mockEmail;
    good_example2::ConsoleLogger testLogger;
    good_example2::OrderService service2(mockEmail, testLogger);
    service2.placeOrder(456, "test@example.com");

    cout << "\n✨ BENEFITS:\n";
    cout << "   • Can inject different implementations\n";
    cout << "   • Easy to test with mocks\n";
    cout << "   • OrderService doesn't control dependencies\n";
    cout << "   • Loose coupling through interfaces\n";
    cout << "   • Flexible and maintainable\n";
}

// ============================================
// EXAMPLE 3: GLOBAL STATE COUPLING
// ============================================

namespace bad_example3 {
// ❌ BAD: Coupled through global state

// Global variable! 🚨
int globalCounter = 0;

class OrderProcessor {
public:
    void processOrder() {
        globalCounter++;  // 🚨 Depends on global state!
        cout << "Order processed. Total: " << globalCounter << endl;
    }
};

class ReportGenerator {
public:
    void generateReport() {
        // 🚨 Also depends on global state!
        cout << "Total orders: " << globalCounter << endl;
    }
};

// PROBLEMS:
// • Hidden dependency on global state
// • Can't test independently
// • Race conditions in multi-threaded code
// • Hard to reason about program state
// • Multiple modules coupled through global
}

namespace good_example3 {
// ✅ GOOD: Explicit dependencies, no global state

class OrderCounter {
private:
    int count = 0;

public:
    void increment() {
        count++;
    }

    int getCount() const {
        return count;
    }
};

class OrderProcessor {
private:
    OrderCounter& counter;  // ✅ Explicit dependency!

public:
    OrderProcessor(OrderCounter& c) : counter(c) {}

    void processOrder() {
        counter.increment();
        cout << "Order processed. Total: " << counter.getCount() << endl;
    }
};

class ReportGenerator {
private:
    OrderCounter& counter;  // ✅ Explicit dependency!

public:
    ReportGenerator(OrderCounter& c) : counter(c) {}

    void generateReport() {
        cout << "Total orders: " << counter.getCount() << endl;
    }
};
}

void example3_global_state() {
    cout << "\n╔═══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 3: GLOBAL STATE COUPLING                ║\n";
    cout << "╚═══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (Global State):\n";
    cout << "```cpp\n";
    cout << "int globalCounter = 0;  // 🚨 Global!\n";
    cout << "\n";
    cout << "class OrderProcessor {\n";
    cout << "    void processOrder() {\n";
    cout << "        globalCounter++;  // Hidden dependency!\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "class ReportGenerator {\n";
    cout << "    void generateReport() {\n";
    cout << "        cout << globalCounter;  // Hidden dependency!\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "```\n";

    cout << "\n🚨 PROBLEMS:\n";
    cout << "   • Hidden coupling through global state\n";
    cout << "   • Can't test independently (shared state)\n";
    cout << "   • Race conditions in multi-threaded code\n";
    cout << "   • Hard to reason about\n";
    cout << "   • Violates encapsulation\n";

    cout << "\n✅ GOOD CODE (Explicit Dependencies):\n";
    cout << "```cpp\n";
    cout << "class OrderCounter {\n";
    cout << "    int count = 0;\n";
    cout << "    void increment();\n";
    cout << "    int getCount();\n";
    cout << "};\n";
    cout << "\n";
    cout << "class OrderProcessor {\n";
    cout << "    OrderCounter& counter;  // Explicit!\n";
    cout << "    OrderProcessor(OrderCounter& c) : counter(c) {}\n";
    cout << "};\n";
    cout << "```\n";

    cout << "\n✨ DEMONSTRATION:\n";

    good_example3::OrderCounter counter;
    good_example3::OrderProcessor processor(counter);
    good_example3::ReportGenerator reporter(counter);

    processor.processOrder();
    processor.processOrder();
    reporter.generateReport();

    cout << "\n✨ BENEFITS:\n";
    cout << "   • Explicit dependencies (no hidden coupling)\n";
    cout << "   • Can test with different counter instances\n";
    cout << "   • Thread-safe (no shared global state)\n";
    cout << "   • Easy to reason about\n";
    cout << "   • Proper encapsulation\n";
}

// ============================================
// EXAMPLE 4: CHAIN OF DEPENDENCIES
// ============================================

namespace bad_example4 {
// ❌ BAD: Long chain of dependencies

class Database {
public:
    string query(const string& sql) {
        return "result";
    }
};

class UserRepository {
public:
    Database database;  // 🚨 Exposed!

    string findUser(const string& username) {
        return database.query("SELECT * FROM users WHERE name = '" + username + "'");
    }
};

class UserService {
public:
    UserRepository repository;  // 🚨 Exposed!

    string getUser(const string& username) {
        return repository.findUser(username);
    }
};

class UserController {
public:
    UserService service;  // 🚨 Exposed!

    void handleRequest(const string& username) {
        string user = service.getUser(username);
        cout << "User: " << user << endl;

        // 🚨 Can access entire chain!
        service.repository.database.query("DELETE FROM users");
    }
};

// PROBLEMS:
// • Controller can access entire chain
// • Violations of Law of Demeter
// • High coupling across all layers
// • Hard to change any layer
// • No encapsulation
}

namespace good_example4 {
// ✅ GOOD: Proper encapsulation, limited dependencies

class Database {
public:
    string query(const string& sql) {
        return "result";
    }
};

class UserRepository {
private:
    Database& database;  // ✅ Private!

public:
    UserRepository(Database& db) : database(db) {}

    string findUser(const string& username) {
        return database.query("SELECT * FROM users WHERE name = '" + username + "'");
    }
};

class UserService {
private:
    UserRepository& repository;  // ✅ Private!

public:
    UserService(UserRepository& repo) : repository(repo) {}

    string getUser(const string& username) {
        return repository.findUser(username);
    }
};

class UserController {
private:
    UserService& service;  // ✅ Private!

public:
    UserController(UserService& svc) : service(svc) {}

    void handleRequest(const string& username) {
        string user = service.getUser(username);
        cout << "User: " << user << endl;

        // ✅ Can only access service, not entire chain!
        // service.repository.database.query()  // Won't compile!
    }
};
}

void example4_dependency_chain() {
    cout << "\n╔═══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 4: CHAIN OF DEPENDENCIES                ║\n";
    cout << "╚═══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (Exposed Chain):\n";
    cout << "```cpp\n";
    cout << "class UserController {\n";
    cout << "public:\n";
    cout << "    UserService service;  // Public!\n";
    cout << "    \n";
    cout << "    void handleRequest() {\n";
    cout << "        // 🚨 Can access entire chain!\n";
    cout << "        service.repository.database.query(...);\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "```\n";

    cout << "\n🚨 PROBLEMS:\n";
    cout << "   • Can access entire dependency chain\n";
    cout << "   • Violates Law of Demeter\n";
    cout << "   • High coupling across layers\n";
    cout << "   • No encapsulation\n";
    cout << "   • Hard to refactor\n";

    cout << "\n✅ GOOD CODE (Encapsulated):\n";
    cout << "```cpp\n";
    cout << "class UserController {\n";
    cout << "private:\n";
    cout << "    UserService& service;  // Private!\n";
    cout << "public:\n";
    cout << "    UserController(UserService& svc) : service(svc) {}\n";
    cout << "    \n";
    cout << "    void handleRequest() {\n";
    cout << "        // ✅ Can only access service\n";
    cout << "        service.getUser(...);\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "```\n";

    cout << "\n✨ BENEFITS:\n";
    cout << "   • Controller only knows about service\n";
    cout << "   • Proper encapsulation\n";
    cout << "   • Low coupling\n";
    cout << "   • Easy to refactor internals\n";
    cout << "   • Follows Law of Demeter\n";
}

// ============================================
// EXAMPLE 5: EVENT-DRIVEN LOOSE COUPLING
// ============================================

namespace bad_example5 {
// ❌ BAD: Direct dependencies between components

class InventorySystem {
public:
    void updateInventory(int productId, int quantity) {
        cout << "Inventory updated for product " << productId << endl;
    }
};

class ShippingSystem {
public:
    void scheduleShipment(int orderId) {
        cout << "Shipment scheduled for order " << orderId << endl;
    }
};

class NotificationSystem {
public:
    void sendNotification(const string& message) {
        cout << "Notification: " << message << endl;
    }
};

class OrderService {
private:
    InventorySystem inventory;      // 🚨 Direct dependency!
    ShippingSystem shipping;         // 🚨 Direct dependency!
    NotificationSystem notifications; // 🚨 Direct dependency!

public:
    void placeOrder(int orderId, int productId, int quantity) {
        cout << "Order placed: " << orderId << endl;

        // 🚨 OrderService must know about all systems!
        inventory.updateInventory(productId, quantity);
        shipping.scheduleShipment(orderId);
        notifications.sendNotification("Order placed");
    }
};

// PROBLEMS:
// • OrderService coupled to 3 systems
// • Adding new system requires changing OrderService
// • Can't test OrderService independently
// • High coupling, low flexibility
}

namespace good_example5 {
// ✅ GOOD: Event-driven, loose coupling

// Event structure
struct OrderPlacedEvent {
    int orderId;
    int productId;
    int quantity;
};

// Observer interface
class OrderEventListener {
public:
    virtual ~OrderEventListener() = default;
    virtual void onOrderPlaced(const OrderPlacedEvent& event) = 0;
};

// Concrete listeners
class InventorySystem : public OrderEventListener {
public:
    void onOrderPlaced(const OrderPlacedEvent& event) override {
        cout << "[INVENTORY] Updating for product " << event.productId << endl;
    }
};

class ShippingSystem : public OrderEventListener {
public:
    void onOrderPlaced(const OrderPlacedEvent& event) override {
        cout << "[SHIPPING] Scheduling shipment for order " << event.orderId << endl;
    }
};

class NotificationSystem : public OrderEventListener {
public:
    void onOrderPlaced(const OrderPlacedEvent& event) override {
        cout << "[NOTIFICATION] Order " << event.orderId << " placed" << endl;
    }
};

// Service publishes events
class OrderService {
private:
    vector<OrderEventListener*> listeners;

public:
    void addListener(OrderEventListener* listener) {
        listeners.push_back(listener);
    }

    void placeOrder(int orderId, int productId, int quantity) {
        cout << "Order placed: " << orderId << endl;

        // ✅ Notify listeners without knowing who they are!
        OrderPlacedEvent event{orderId, productId, quantity};
        for (auto listener : listeners) {
            listener->onOrderPlaced(event);
        }
    }
};
}

void example5_event_driven() {
    cout << "\n╔═══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 5: EVENT-DRIVEN LOOSE COUPLING          ║\n";
    cout << "╚═══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (Direct Dependencies):\n";
    cout << "```cpp\n";
    cout << "class OrderService {\n";
    cout << "    InventorySystem inventory;\n";
    cout << "    ShippingSystem shipping;\n";
    cout << "    NotificationSystem notifications;\n";
    cout << "    \n";
    cout << "    void placeOrder() {\n";
    cout << "        inventory.updateInventory();\n";
    cout << "        shipping.scheduleShipment();\n";
    cout << "        notifications.sendNotification();\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "// 🚨 Coupled to 3 systems!\n";
    cout << "```\n";

    cout << "\n✅ GOOD CODE (Event-Driven):\n";
    cout << "```cpp\n";
    cout << "class OrderService {\n";
    cout << "    vector<OrderEventListener*> listeners;\n";
    cout << "    \n";
    cout << "    void placeOrder() {\n";
    cout << "        OrderPlacedEvent event{...};\n";
    cout << "        for (auto listener : listeners) {\n";
    cout << "            listener->onOrderPlaced(event);\n";
    cout << "        }\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "// ✅ Doesn't know about specific systems!\n";
    cout << "```\n";

    cout << "\n✨ DEMONSTRATION:\n";

    good_example5::OrderService service;
    good_example5::InventorySystem inventory;
    good_example5::ShippingSystem shipping;
    good_example5::NotificationSystem notifications;

    service.addListener(&inventory);
    service.addListener(&shipping);
    service.addListener(&notifications);

    service.placeOrder(123, 456, 2);

    cout << "\n✨ BENEFITS:\n";
    cout << "   • OrderService doesn't know about specific systems\n";
    cout << "   • Can add new listeners without changing OrderService\n";
    cout << "   • Can remove listeners easily\n";
    cout << "   • Very loose coupling\n";
    cout << "   • High flexibility\n";
}

// ============================================
// EXAMPLE 6: LAW OF DEMETER
// ============================================

namespace bad_example6 {
// ❌ BAD: Violating Law of Demeter

class Address {
private:
    string street;
    string city;
    string country;

public:
    Address(const string& s, const string& c, const string& co)
        : street(s), city(c), country(co) {}

    string getCountry() const { return country; }
    bool isDomestic() const { return country == "USA"; }
};

class Customer {
private:
    Address address;
    string name;

public:
    Customer(const string& n, const Address& addr)
        : name(n), address(addr) {}

    bool hasDomesticAddress() const {
        return address.isDomestic();
    }
};

class Order {
private:
    Customer customer;
    double total;

public:
    Order(const Customer& cust, double t)
        : customer(cust), total(t) {}

    bool isDomesticOrder() const {
        return customer.hasDomesticAddress();
    }
};

class ShippingService {
public:
    void shipOrder(const Order& order) {
        // ✅ Only talks to immediate friend (Order)
        if (order.isDomesticOrder()) {
            cout << "Shipping to USA" << endl;
        } else {
            cout << "International shipping" << endl;
        }
    }
};
}

void example6_law_of_demeter() {
    cout << "\n╔═══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 6: LAW OF DEMETER                       ║\n";
    cout << "╚═══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (Violates Law of Demeter):\n";
    cout << "```cpp\n";
    cout << "class ShippingService {\n";
    cout << "    void shipOrder(Order& order) {\n";
    cout << "        // 🚨 Reaching through multiple objects!\n";
    cout << "        string country = order.customer.address.country;\n";
    cout << "        \n";
    cout << "        if (country == \"USA\") { ... }\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "```\n";

    cout << "\n🚨 PROBLEMS:\n";
    cout << "   • ShippingService knows internal structure\n";
    cout << "   • Coupled to Order, Customer, AND Address\n";
    cout << "   • Changes in any class affect ShippingService\n";
    cout << "   • Violates \"only talk to immediate friends\"\n";

    cout << "\n✅ GOOD CODE (Follows Law of Demeter):\n";
    cout << "```cpp\n";
    cout << "class Order {\n";
    cout << "    bool isDomesticOrder() {  // Encapsulation!\n";
    cout << "        return customer.hasDomesticAddress();\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "class ShippingService {\n";
    cout << "    void shipOrder(Order& order) {\n";
    cout << "        // ✅ Only talks to Order!\n";
    cout << "        if (order.isDomesticOrder()) { ... }\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "```\n";

    cout << "\n💡 LAW OF DEMETER:\n";
    cout << "   \"Only talk to your immediate friends\"\n";
    cout << "   \n";
    cout << "   A method should only call methods of:\n";
    cout << "   • Itself\n";
    cout << "   • Its parameters\n";
    cout << "   • Objects it creates\n";
    cout << "   • Its direct fields\n";
    cout << "   \n";
    cout << "   ❌ Don't: object.getX().getY().getZ()\n";
    cout << "   ✅ Do: object.doSomething()\n";

    cout << "\n✨ BENEFITS:\n";
    cout << "   • ShippingService only coupled to Order\n";
    cout << "   • Changes in Customer/Address don't affect ShippingService\n";
    cout << "   • Proper encapsulation\n";
    cout << "   • Lower coupling\n";
}

// ============================================
// EXAMPLE 7: INTERFACE SEGREGATION
// ============================================

namespace bad_example7 {
// ❌ BAD: Fat interface causes unnecessary coupling

class Worker {
public:
    virtual void work() = 0;
    virtual void eat() = 0;
    virtual void sleep() = 0;
    virtual void takeMeetings() = 0;
    virtual void writeCode() = 0;
    virtual void managePeople() = 0;
    virtual ~Worker() = default;
};

// Robot only needs work(), but forced to implement all!
class Robot : public Worker {
public:
    void work() override {
        cout << "Robot working" << endl;
    }

    void eat() override {
        // 🚨 Robots don't eat!
        throw runtime_error("Robots don't eat");
    }

    void sleep() override {
        // 🚨 Robots don't sleep!
        throw runtime_error("Robots don't sleep");
    }

    void takeMeetings() override {
        throw runtime_error("Not applicable");
    }

    void writeCode() override {
        throw runtime_error("Not applicable");
    }

    void managePeople() override {
        throw runtime_error("Not applicable");
    }
};

// PROBLEMS:
// • Robot coupled to methods it doesn't need
// • Fat interface
// • Violates Interface Segregation Principle
// • Changes to interface affect all implementations
}

namespace good_example7 {
// ✅ GOOD: Segregated interfaces, loose coupling

class Workable {
public:
    virtual void work() = 0;
    virtual ~Workable() = default;
};

class Eatable {
public:
    virtual void eat() = 0;
    virtual ~Eatable() = default;
};

class Sleepable {
public:
    virtual void sleep() = 0;
    virtual ~Sleepable() = default;
};

class Programmable {
public:
    virtual void writeCode() = 0;
    virtual ~Programmable() = default;
};

// Robot only implements what it needs
class Robot : public Workable {
public:
    void work() override {
        cout << "Robot working" << endl;
    }
};

// Human implements multiple interfaces
class Human : public Workable, public Eatable, public Sleepable {
public:
    void work() override {
        cout << "Human working" << endl;
    }

    void eat() override {
        cout << "Human eating" << endl;
    }

    void sleep() override {
        cout << "Human sleeping" << endl;
    }
};

// Programmer is a specialized human
class Programmer : public Human, public Programmable {
public:
    void writeCode() override {
        cout << "Programmer coding" << endl;
    }
};
}

void example7_interface_segregation() {
    cout << "\n╔═══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 7: INTERFACE SEGREGATION                ║\n";
    cout << "╚═══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (Fat Interface):\n";
    cout << "```cpp\n";
    cout << "class Worker {\n";
    cout << "    virtual void work() = 0;\n";
    cout << "    virtual void eat() = 0;\n";
    cout << "    virtual void sleep() = 0;\n";
    cout << "    virtual void takeMeetings() = 0;\n";
    cout << "    virtual void writeCode() = 0;\n";
    cout << "    virtual void managePeople() = 0;\n";
    cout << "};\n";
    cout << "\n";
    cout << "class Robot : public Worker {\n";
    cout << "    // 🚨 Must implement ALL methods!\n";
    cout << "    void eat() { throw \"Robots don't eat\"; }\n";
    cout << "    void sleep() { throw \"Robots don't sleep\"; }\n";
    cout << "};\n";
    cout << "```\n";

    cout << "\n✅ GOOD CODE (Segregated Interfaces):\n";
    cout << "```cpp\n";
    cout << "class Workable { virtual void work() = 0; };\n";
    cout << "class Eatable { virtual void eat() = 0; };\n";
    cout << "class Sleepable { virtual void sleep() = 0; };\n";
    cout << "\n";
    cout << "class Robot : public Workable {\n";
    cout << "    // ✅ Only implements what it needs!\n";
    cout << "    void work() override { ... }\n";
    cout << "};\n";
    cout << "\n";
    cout << "class Human : public Workable, Eatable, Sleepable {\n";
    cout << "    // ✅ Implements what humans need!\n";
    cout << "};\n";
    cout << "```\n";

    cout << "\n✨ DEMONSTRATION:\n";

    good_example7::Robot robot;
    good_example7::Human human;
    good_example7::Programmer programmer;

    robot.work();

    human.work();
    human.eat();
    human.sleep();

    programmer.work();
    programmer.writeCode();

    cout << "\n✨ BENEFITS:\n";
    cout << "   • Clients only depend on interfaces they use\n";
    cout << "   • Robot not coupled to eat/sleep methods\n";
    cout << "   • Changes to one interface don't affect others\n";
    cout << "   • Lower coupling\n";
    cout << "   • Follows Interface Segregation Principle\n";
}

// ============================================
// EXAMPLE 8: MEASURING COUPLING
// ============================================

void example8_measuring_coupling() {
    cout << "\n╔═══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 8: MEASURING COUPLING                   ║\n";
    cout << "╚═══════════════════════════════════════════════════╝\n";

    cout << "\n📊 COUPLING METRICS:\n";

    cout << "\n1. AFFERENT COUPLING (Ca):\n";
    cout << "   Number of classes that depend ON this class\n";
    cout << "   High Ca = Many classes depend on you (stable)\n";

    cout << "\n2. EFFERENT COUPLING (Ce):\n";
    cout << "   Number of classes this class depends on\n";
    cout << "   High Ce = You depend on many (unstable)\n";

    cout << "\n3. INSTABILITY (I = Ce / (Ca + Ce)):\n";
    cout << "   Range: 0 to 1\n";
    cout << "   I = 0: Stable (many depend on you, you depend on few)\n";
    cout << "   I = 1: Unstable (few depend on you, you depend on many)\n";

    cout << "\n📈 EXAMPLE:\n";
    cout << "\n   Class A:\n";
    cout << "   • 5 classes depend on A (Ca = 5)\n";
    cout << "   • A depends on 2 classes (Ce = 2)\n";
    cout << "   • Instability: I = 2 / (5 + 2) = 0.28 (Stable)\n";

    cout << "\n   Class B:\n";
    cout << "   • 1 class depends on B (Ca = 1)\n";
    cout << "   • B depends on 8 classes (Ce = 8)\n";
    cout << "   • Instability: I = 8 / (1 + 8) = 0.89 (Unstable)\n";

    cout << "\n🎯 GOALS:\n";
    cout << "   • Keep coupling low (minimize dependencies)\n";
    cout << "   • Stable classes should be abstract\n";
    cout << "   • Unstable classes should be concrete\n";
    cout << "   • Depend on stable abstractions\n";

    cout << "\n🚨 WARNING SIGNS:\n";
    cout << "   • Class depends on > 7 other classes\n";
    cout << "   • Class is depended on by > 10 classes\n";
    cout << "   • Circular dependencies\n";
    cout << "   • Deep inheritance hierarchies (> 5 levels)\n";
}

// ============================================
// EXAMPLE 9: DEPENDENCY INVERSION
// ============================================

namespace bad_example9 {
// ❌ BAD: High-level module depends on low-level module

class MySQLDatabase {
public:
    void connect() {
        cout << "MySQL: Connected" << endl;
    }

    void saveData(const string& data) {
        cout << "MySQL: Saving " << data << endl;
    }
};

// High-level business logic
class ReportGenerator {
private:
    MySQLDatabase database;  // 🚨 Depends on concrete class!

public:
    void generateReport() {
        database.connect();
        // Generate report
        database.saveData("report data");
    }
};

// PROBLEMS:
// • High-level module (ReportGenerator) depends on low-level (MySQL)
// • Can't switch databases
// • Violates Dependency Inversion Principle
// • High coupling
}

namespace good_example9 {
// ✅ GOOD: Both depend on abstraction

// Abstraction
class DatabaseInterface {
public:
    virtual ~DatabaseInterface() = default;
    virtual void connect() = 0;
    virtual void saveData(const string& data) = 0;
};

// Low-level module implements abstraction
class MySQLDatabase : public DatabaseInterface {
public:
    void connect() override {
        cout << "MySQL: Connected" << endl;
    }

    void saveData(const string& data) override {
        cout << "MySQL: Saving " << data << endl;
    }
};

class MongoDatabase : public DatabaseInterface {
public:
    void connect() override {
        cout << "MongoDB: Connected" << endl;
    }

    void saveData(const string& data) override {
        cout << "MongoDB: Saving " << data << endl;
    }
};

// High-level module depends on abstraction
class ReportGenerator {
private:
    DatabaseInterface& database;  // ✅ Depends on abstraction!

public:
    ReportGenerator(DatabaseInterface& db) : database(db) {}

    void generateReport() {
        database.connect();
        // Generate report
        database.saveData("report data");
    }
};
}

void example9_dependency_inversion() {
    cout << "\n╔═══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 9: DEPENDENCY INVERSION PRINCIPLE       ║\n";
    cout << "╚═══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (Depends on Concrete Class):\n";
    cout << "```cpp\n";
    cout << "class ReportGenerator {\n";
    cout << "    MySQLDatabase database;  // High-level depends on low-level!\n";
    cout << "};\n";
    cout << "```\n";

    cout << "\n✅ GOOD CODE (Depends on Abstraction):\n";
    cout << "```cpp\n";
    cout << "class DatabaseInterface { ... };  // Abstraction\n";
    cout << "\n";
    cout << "class MySQLDatabase : public DatabaseInterface { ... };\n";
    cout << "class MongoDatabase : public DatabaseInterface { ... };\n";
    cout << "\n";
    cout << "class ReportGenerator {\n";
    cout << "    DatabaseInterface& database;  // Depends on abstraction!\n";
    cout << "};\n";
    cout << "```\n";

    cout << "\n💡 DEPENDENCY INVERSION PRINCIPLE:\n";
    cout << "   1. High-level modules should not depend on low-level modules\n";
    cout << "      Both should depend on abstractions\n";
    cout << "   \n";
    cout << "   2. Abstractions should not depend on details\n";
    cout << "      Details should depend on abstractions\n";

    cout << "\n✨ DEMONSTRATION:\n";

    cout << "\n--- Using MySQL ---";
    good_example9::MySQLDatabase mysql;
    good_example9::ReportGenerator reporter1(mysql);
    reporter1.generateReport();

    cout << "\n--- Using MongoDB ---";
    good_example9::MongoDatabase mongo;
    good_example9::ReportGenerator reporter2(mongo);
    reporter2.generateReport();

    cout << "\n✨ BENEFITS:\n";
    cout << "   • High-level logic independent of low-level details\n";
    cout << "   • Can switch implementations easily\n";
    cout << "   • Testable with mocks\n";
    cout << "   • Low coupling\n";
    cout << "   • Follows SOLID principles\n";
}

// ============================================
// EXAMPLE 10: BENEFITS & SUMMARY
// ============================================

void example10_summary() {
    cout << "\n╔═══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 10: BENEFITS & SUMMARY                  ║\n";
    cout << "╚═══════════════════════════════════════════════════╝\n";

    cout << "\n✅ BENEFITS OF LOW COUPLING:\n";

    cout << "\n1. EASIER TO CHANGE:\n";
    cout << "   • Changes don't ripple through system\n";
    cout << "   • Modify one module without affecting others\n";
    cout << "   • Lower risk when making changes\n";

    cout << "\n2. EASIER TO TEST:\n";
    cout << "   • Test modules independently\n";
    cout << "   • Fewer dependencies to mock\n";
    cout << "   • Faster test execution\n";

    cout << "\n3. BETTER REUSABILITY:\n";
    cout << "   • Modules work independently\n";
    cout << "   • Can reuse in different contexts\n";
    cout << "   • Don't drag along dependencies\n";

    cout << "\n4. EASIER TO UNDERSTAND:\n";
    cout << "   • Less to think about at once\n";
    cout << "   • Clear interfaces\n";
    cout << "   • Simpler mental model\n";

    cout << "\n5. PARALLEL DEVELOPMENT:\n";
    cout << "   • Teams work independently\n";
    cout << "   • Less coordination needed\n";
    cout << "   • Faster development\n";

    cout << "\n6. BETTER MAINTAINABILITY:\n";
    cout << "   • Bugs isolated to modules\n";
    cout << "   • Easier to fix issues\n";
    cout << "   • Lower long-term costs\n";

    cout << "\n📊 COUPLING SPECTRUM:\n";
    cout << "\n   TIGHT COUPLING (Bad):\n";
    cout << "   ════════════════════\n";
    cout << "   • Concrete class dependencies\n";
    cout << "   • Global state\n";
    cout << "   • Long dependency chains\n";
    cout << "   • Violates Law of Demeter\n";
    cout << "   • Fat interfaces\n";

    cout << "\n   LOOSE COUPLING (Good):\n";
    cout << "   ════════════════════\n";
    cout << "   • Interface dependencies\n";
    cout << "   • Dependency injection\n";
    cout << "   • Event-driven\n";
    cout << "   • Law of Demeter\n";
    cout << "   • Interface segregation\n";

    cout << "\n🎯 KEY PRINCIPLES:\n";

    cout << "\n1. DEPEND ON ABSTRACTIONS:\n";
    cout << "   ✅ Use interfaces, not concrete classes\n";

    cout << "\n2. DEPENDENCY INJECTION:\n";
    cout << "   ✅ Inject dependencies from outside\n";

    cout << "\n3. LAW OF DEMETER:\n";
    cout << "   ✅ Only talk to immediate friends\n";

    cout << "\n4. INTERFACE SEGREGATION:\n";
    cout << "   ✅ Many small interfaces > One large interface\n";

    cout << "\n5. AVOID GLOBAL STATE:\n";
    cout << "   ✅ Explicit dependencies > Hidden coupling\n";

    cout << "\n6. EVENT-DRIVEN:\n";
    cout << "   ✅ Publish events > Direct method calls\n";

    cout << "\n💡 REMEMBER:\n";
    cout << "   \"Good design is loosely coupled and highly cohesive.\"\n";
    cout << "   \n";
    cout << "   Low Coupling = Modules are independent\n";
    cout << "   High Cohesion = Related things stay together\n";
    cout << "   \n";
    cout << "   Together, they create maintainable software!\n";

    cout << "\n🔍 CHECKLIST:\n";
    cout << "   □ Do I depend on interfaces, not concrete classes?\n";
    cout << "   □ Are dependencies injected?\n";
    cout << "   □ Can I test this module independently?\n";
    cout << "   □ Do I follow Law of Demeter?\n";
    cout << "   □ Are my interfaces focused and small?\n";
    cout << "   □ Do I avoid global state?\n";
    cout << "   □ Can this module be reused elsewhere?\n";

    cout << "\n⚠️ WARNING SIGNS:\n";
    cout << "   🚨 Changes in one class require changes in many\n";
    cout << "   🚨 Hard to test without complex setup\n";
    cout << "   🚨 Can't reuse without dragging dependencies\n";
    cout << "   🚨 Code like: obj.getX().getY().getZ()\n";
    cout << "   🚨 Many concrete class dependencies\n";
}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║              LOW COUPLING PRINCIPLE                   ║\n";
    cout << "║                                                       ║\n";
    cout << "║  \"Minimize dependencies between modules. Each         ║\n";
    cout << "║   module should know as little as possible about      ║\n";
    cout << "║   others.\"                                            ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n";

    example1_tight_coupling();
    example2_dependency_injection();
    example3_global_state();
    example4_dependency_chain();
    example5_event_driven();
    example6_law_of_demeter();
    example7_interface_segregation();
    example8_measuring_coupling();
    example9_dependency_inversion();
    example10_summary();

    cout << "\n\n";
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║         🎉 LOW COUPLING PRINCIPLE COMPLETE! 🎉        ║\n";
    cout << "║                                                       ║\n";
    cout << "║  Key Takeaway: Minimize dependencies between         ║\n";
    cout << "║  modules. Depend on abstractions, inject             ║\n";
    cout << "║  dependencies, follow Law of Demeter, and keep       ║\n";
    cout << "║  modules independent!                                ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n\n";

    return 0;
}
