#include <vector>
#include <string>
#include <memory>
#include <map>
#include <functional>
#include <print>

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
        std::println("MySQL: Connected");
    }

    void query(const std::string& sql) {
        std::println("MySQL: Executing {}", sql);
    }

    void disconnect() {
        std::println("MySQL: Disconnected");
    }
};

class UserService {
private:
    MySQLDatabase database;  // 🚨 Tightly coupled to MySQL!

public:
    void saveUser(const std::string& username) {
        database.connect();
        database.query("INSERT INTO users VALUES ('" + username + "')");
        database.disconnect();
    }

    void getUser(const std::string& username) {
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
    virtual void query(const std::string& sql) = 0;
    virtual void disconnect() = 0;
};

// Concrete implementations
class MySQLDatabase : public Database {
public:
    void connect() override {
        std::println("MySQL: Connected");
    }

    void query(const std::string& sql) override {
        std::println("MySQL: Executing {}", sql);
    }

    void disconnect() override {
        std::println("MySQL: Disconnected");
    }
};

class PostgreSQLDatabase : public Database {
public:
    void connect() override {
        std::println("PostgreSQL: Connected");
    }

    void query(const std::string& sql) override {
        std::println("PostgreSQL: Executing {}", sql);
    }

    void disconnect() override {
        std::println("PostgreSQL: Disconnected");
    }
};

// Service depends on interface, not concrete class
class UserService {
private:
    Database& database;  // ✅ Depends on abstraction!

public:
    UserService(Database& db) : database(db) {}

    void saveUser(const std::string& username) {
        database.connect();
        database.query("INSERT INTO users VALUES ('" + username + "')");
        database.disconnect();
    }

    void getUser(const std::string& username) {
        database.connect();
        database.query("SELECT * FROM users WHERE name = '" + username + "'");
        database.disconnect();
    }
};
}

void example1_tight_coupling() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 1: TIGHT VS LOOSE COUPLING              ║");
    std::println("╚═══════════════════════════════════════════════════╝");

    std::print("--- Using MySQL ---");
    good_example1::MySQLDatabase mysql;
    good_example1::UserService service1(mysql);
    service1.saveUser("john");

    std::println("");
    std::print("--- Using PostgreSQL ---");
    good_example1::PostgreSQLDatabase postgres;
    good_example1::UserService service2(postgres);
    service2.saveUser("jane");

}

// ============================================
// EXAMPLE 2: DEPENDENCY INJECTION
// ============================================

namespace bad_example2 {
// ❌ BAD: Creating dependencies internally

class EmailService {
public:
    void sendEmail(const std::string& to, const std::string& message) {
        std::println("Email sent to {}: {}", to, message);
    }
};

class Logger {
public:
    void log(const std::string& message) {
        std::println("[LOG] {}", message);
    }
};

class OrderService {
private:
    EmailService emailService;  // 🚨 Created internally!
    Logger logger;               // 🚨 Created internally!

public:
    void placeOrder(int orderId, const std::string& customerEmail) {
        logger.log("Placing order " + std::to_string(orderId));

        // Order processing logic
        std::println("Processing order {}", orderId);

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
    virtual void sendEmail(const std::string& to, const std::string& message) = 0;
};

class Logger {
public:
    virtual ~Logger() = default;
    virtual void log(const std::string& message) = 0;
};

// Implementations
class SMTPEmailService : public EmailService {
public:
    void sendEmail(const std::string& to, const std::string& message) override {
        std::println("[SMTP] Email sent to {}: {}", to, message);
    }
};

class ConsoleLogger : public Logger {
public:
    void log(const std::string& message) override {
        std::println("[LOG] {}", message);
    }
};

class MockEmailService : public EmailService {
public:
    void sendEmail(const std::string& to, const std::string& message) override {
        std::println("[MOCK EMAIL] Would send to {}", to);
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

    void placeOrder(int orderId, const std::string& customerEmail) {
        logger.log("Placing order " + std::to_string(orderId));

        // Order processing logic
        std::println("Processing order {}", orderId);

        emailService.sendEmail(customerEmail, "Order confirmed");
        logger.log("Order placed successfully");
    }
};
}

void example2_dependency_injection() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 2: DEPENDENCY INJECTION                 ║");
    std::println("╚═══════════════════════════════════════════════════╝");

    std::print("--- Production (Real Services) ---");
    good_example2::SMTPEmailService emailService;
    good_example2::ConsoleLogger logger;
    good_example2::OrderService service1(emailService, logger);
    service1.placeOrder(123, "customer@example.com");

    std::println("");
    std::print("--- Testing (Mock Services) ---");
    good_example2::MockEmailService mockEmail;
    good_example2::ConsoleLogger testLogger;
    good_example2::OrderService service2(mockEmail, testLogger);
    service2.placeOrder(456, "test@example.com");

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
        std::println("Order processed. Total: {}", globalCounter);
    }
};

class ReportGenerator {
public:
    void generateReport() {
        // 🚨 Also depends on global state!
        std::println("Total orders: {}", globalCounter);
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
        std::println("Order processed. Total: {}", counter.getCount());
    }
};

class ReportGenerator {
private:
    OrderCounter& counter;  // ✅ Explicit dependency!

public:
    ReportGenerator(OrderCounter& c) : counter(c) {}

    void generateReport() {
        std::println("Total orders: {}", counter.getCount());
    }
};
}

void example3_global_state() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 3: GLOBAL STATE COUPLING                ║");
    std::println("╚═══════════════════════════════════════════════════╝");

    good_example3::OrderCounter counter;
    good_example3::OrderProcessor processor(counter);
    good_example3::ReportGenerator reporter(counter);

    processor.processOrder();
    processor.processOrder();
    reporter.generateReport();

}

// ============================================
// EXAMPLE 4: CHAIN OF DEPENDENCIES
// ============================================

namespace bad_example4 {
// ❌ BAD: Long chain of dependencies

class Database {
public:
    std::string query(const std::string& sql) {
        return "result";
    }
};

class UserRepository {
public:
    Database database;  // 🚨 Exposed!

    std::string findUser(const std::string& username) {
        return database.query("SELECT * FROM users WHERE name = '" + username + "'");
    }
};

class UserService {
public:
    UserRepository repository;  // 🚨 Exposed!

    std::string getUser(const std::string& username) {
        return repository.findUser(username);
    }
};

class UserController {
public:
    UserService service;  // 🚨 Exposed!

    void handleRequest(const std::string& username) {
        std::string user = service.getUser(username);
        std::println("User: {}", user);

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
    std::string query(const std::string& sql) {
        return "result";
    }
};

class UserRepository {
private:
    Database& database;  // ✅ Private!

public:
    UserRepository(Database& db) : database(db) {}

    std::string findUser(const std::string& username) {
        return database.query("SELECT * FROM users WHERE name = '" + username + "'");
    }
};

class UserService {
private:
    UserRepository& repository;  // ✅ Private!

public:
    UserService(UserRepository& repo) : repository(repo) {}

    std::string getUser(const std::string& username) {
        return repository.findUser(username);
    }
};

class UserController {
private:
    UserService& service;  // ✅ Private!

public:
    UserController(UserService& svc) : service(svc) {}

    void handleRequest(const std::string& username) {
        std::string user = service.getUser(username);
        std::println("User: {}", user);

        // ✅ Can only access service, not entire chain!
        // service.repository.database.query()  // Won't compile!
    }
};
}

void example4_dependency_chain() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 4: CHAIN OF DEPENDENCIES                ║");
    std::println("╚═══════════════════════════════════════════════════╝");

    std::println("    ");

    std::println("    ");
}

// ============================================
// EXAMPLE 5: EVENT-DRIVEN LOOSE COUPLING
// ============================================

namespace bad_example5 {
// ❌ BAD: Direct dependencies between components

class InventorySystem {
public:
    void updateInventory(int productId, int quantity) {
        std::println("Inventory updated for product {}", productId);
    }
};

class ShippingSystem {
public:
    void scheduleShipment(int orderId) {
        std::println("Shipment scheduled for order {}", orderId);
    }
};

class NotificationSystem {
public:
    void sendNotification(const std::string& message) {
        std::println("Notification: {}", message);
    }
};

class OrderService {
private:
    InventorySystem inventory;      // 🚨 Direct dependency!
    ShippingSystem shipping;         // 🚨 Direct dependency!
    NotificationSystem notifications; // 🚨 Direct dependency!

public:
    void placeOrder(int orderId, int productId, int quantity) {
        std::println("Order placed: {}", orderId);

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
        std::println("[INVENTORY] Updating for product {}", event.productId);
    }
};

class ShippingSystem : public OrderEventListener {
public:
    void onOrderPlaced(const OrderPlacedEvent& event) override {
        std::println("[SHIPPING] Scheduling shipment for order {}", event.orderId);
    }
};

class NotificationSystem : public OrderEventListener {
public:
    void onOrderPlaced(const OrderPlacedEvent& event) override {
        std::println("[NOTIFICATION] Order {} placed", event.orderId);
    }
};

// Service publishes events
class OrderService {
private:
    std::vector<OrderEventListener*> listeners;

public:
    void addListener(OrderEventListener* listener) {
        listeners.push_back(listener);
    }

    void placeOrder(int orderId, int productId, int quantity) {
        std::println("Order placed: {}", orderId);

        // ✅ Notify listeners without knowing who they are!
        OrderPlacedEvent event{orderId, productId, quantity};
        for (auto listener : listeners) {
            listener->onOrderPlaced(event);
        }
    }
};
}

void example5_event_driven() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 5: EVENT-DRIVEN LOOSE COUPLING          ║");
    std::println("╚═══════════════════════════════════════════════════╝");

    std::println("    ");

    std::println("    ");

    good_example5::OrderService service;
    good_example5::InventorySystem inventory;
    good_example5::ShippingSystem shipping;
    good_example5::NotificationSystem notifications;

    service.addListener(&inventory);
    service.addListener(&shipping);
    service.addListener(&notifications);

    service.placeOrder(123, 456, 2);

}

// ============================================
// EXAMPLE 6: LAW OF DEMETER
// ============================================

namespace bad_example6 {
// ❌ BAD: Violating Law of Demeter

class Address {
private:
    std::string street;
    std::string city;
    std::string country;

public:
    Address(const std::string& s, const std::string& c, const std::string& co)
        : street(s), city(c), country(co) {}

    std::string getCountry() const { return country; }
    bool isDomestic() const { return country == "USA"; }
};

class Customer {
private:
    Address address;
    std::string name;

public:
    Customer(const std::string& n, const Address& addr)
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
            std::println("Shipping to USA");
        } else {
            std::println("International shipping");
        }
    }
};
}

void example6_law_of_demeter() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 6: LAW OF DEMETER                       ║");
    std::println("╚═══════════════════════════════════════════════════╝");

    std::println("        ");

    std::println("   ");
    std::println("   ");
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
        std::println("Robot working");
    }

    void eat() override {
        // 🚨 Robots don't eat!
        throw std::runtime_error("Robots don't eat");
    }

    void sleep() override {
        // 🚨 Robots don't sleep!
        throw std::runtime_error("Robots don't sleep");
    }

    void takeMeetings() override {
        throw std::runtime_error("Not applicable");
    }

    void writeCode() override {
        throw std::runtime_error("Not applicable");
    }

    void managePeople() override {
        throw std::runtime_error("Not applicable");
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
        std::println("Robot working");
    }
};

// Human implements multiple interfaces
class Human : public Workable, public Eatable, public Sleepable {
public:
    void work() override {
        std::println("Human working");
    }

    void eat() override {
        std::println("Human eating");
    }

    void sleep() override {
        std::println("Human sleeping");
    }
};

// Programmer is a specialized human
class Programmer : public Human, public Programmable {
public:
    void writeCode() override {
        std::println("Programmer coding");
    }
};
}

void example7_interface_segregation() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 7: INTERFACE SEGREGATION                ║");
    std::println("╚═══════════════════════════════════════════════════╝");

    good_example7::Robot robot;
    good_example7::Human human;
    good_example7::Programmer programmer;

    robot.work();

    human.work();
    human.eat();
    human.sleep();

    programmer.work();
    programmer.writeCode();

}

// ============================================
// EXAMPLE 8: MEASURING COUPLING
// ============================================

void example8_measuring_coupling() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 8: MEASURING COUPLING                   ║");
    std::println("╚═══════════════════════════════════════════════════╝");

}

// ============================================
// EXAMPLE 9: DEPENDENCY INVERSION
// ============================================

namespace bad_example9 {
// ❌ BAD: High-level module depends on low-level module

class MySQLDatabase {
public:
    void connect() {
        std::println("MySQL: Connected");
    }

    void saveData(const std::string& data) {
        std::println("MySQL: Saving {}", data);
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
    virtual void saveData(const std::string& data) = 0;
};

// Low-level module implements abstraction
class MySQLDatabase : public DatabaseInterface {
public:
    void connect() override {
        std::println("MySQL: Connected");
    }

    void saveData(const std::string& data) override {
        std::println("MySQL: Saving {}", data);
    }
};

class MongoDatabase : public DatabaseInterface {
public:
    void connect() override {
        std::println("MongoDB: Connected");
    }

    void saveData(const std::string& data) override {
        std::println("MongoDB: Saving {}", data);
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
    std::println("");
    std::println("╔═══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 9: DEPENDENCY INVERSION PRINCIPLE       ║");
    std::println("╚═══════════════════════════════════════════════════╝");

    std::println("   ");

    std::print("--- Using MySQL ---");
    good_example9::MySQLDatabase mysql;
    good_example9::ReportGenerator reporter1(mysql);
    reporter1.generateReport();

    std::println("");
    std::print("--- Using MongoDB ---");
    good_example9::MongoDatabase mongo;
    good_example9::ReportGenerator reporter2(mongo);
    reporter2.generateReport();

}

// ============================================
// EXAMPLE 10: BENEFITS & SUMMARY
// ============================================

void example10_summary() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 10: BENEFITS & SUMMARY                  ║");
    std::println("╚═══════════════════════════════════════════════════╝");

    std::println("   ════════════════════");

    std::println("   ════════════════════");

    std::println("   ");
    std::println("   ");

}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    std::println("╔═══════════════════════════════════════════════════════╗");
    std::println("║                                                       ║");
    std::println("║              LOW COUPLING PRINCIPLE                   ║");
    std::println("║                                                       ║");
    std::println("║  \"Minimize dependencies between modules. Each        ║");
    std::println("║   module should know as little as possible about      ║");
    std::println("║   others.\"                                           ║");
    std::println("║                                                       ║");
    std::println("╚═══════════════════════════════════════════════════════╝");

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

    return 0;
}
