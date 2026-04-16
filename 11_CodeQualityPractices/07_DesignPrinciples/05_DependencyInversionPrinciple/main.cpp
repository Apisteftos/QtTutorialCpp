#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
using namespace std;

// ============================================
// DEPENDENCY INVERSION PRINCIPLE (DIP)
// ============================================

/*
DEPENDENCY INVERSION PRINCIPLE:
    "High-level modules should not depend on low-level modules.
     Both should depend on abstractions.
     Abstractions should not depend on details.
     Details should depend on abstractions."
    — Robert C. Martin

WHAT IT MEANS:
    • Depend on interfaces/abstractions, not concrete classes
    • High-level policy should not depend on low-level details
    • Both should depend on abstract interfaces
    • Invert the dependency direction

WHY IT MATTERS:
    ✅ Flexible and maintainable code
    ✅ Easy to swap implementations
    ✅ Testable with mock objects
    ✅ Reduced coupling
    ✅ Better code organization

KEY CONCEPTS:
    • High-level = Business logic, policies
    • Low-level = Implementation details, I/O, databases
    • Abstraction = Interface or abstract class
    • Dependency Injection = Provide dependencies from outside
    • Inversion of Control (IoC) = Framework controls flow

VIOLATION SIGNS:
    🚨 new keyword everywhere (tight coupling)
    🚨 Concrete class types in method parameters
    🚨 High-level modules importing low-level modules
    🚨 Difficult to test (can't mock dependencies)
    🚨 Changes ripple through the system

TRADITIONAL DEPENDENCY:
    High-level → Low-level

INVERTED DEPENDENCY:
    High-level → Abstraction ← Low-level
*/

// ============================================
// EXAMPLE 1: DATABASE CONNECTION (Classic)
// ============================================

namespace bad_example1 {
// ❌ BAD: High-level depends on low-level concrete class

// Low-level module
class MySQLDatabase {
public:
    void connect() {
        cout << "   🔌 Connected to MySQL database" << endl;
    }

    void query(const string& sql) {
        cout << "   📊 Executing MySQL query: " << sql << endl;
    }

    void disconnect() {
        cout << "   🔌 Disconnected from MySQL" << endl;
    }
};

// 🚨 High-level module depends on concrete MySQLDatabase
class UserRepository {
private:
    MySQLDatabase database;  // 🚨 Tight coupling!

public:
    void getUser(int id) {
        database.connect();
        database.query("SELECT * FROM users WHERE id = " + to_string(id));
        database.disconnect();
    }

    void saveUser(const string& name) {
        database.connect();
        database.query("INSERT INTO users (name) VALUES ('" + name + "')");
        database.disconnect();
    }
};

// PROBLEMS:
// • UserRepository tightly coupled to MySQLDatabase
// • Can't switch to PostgreSQL without changing UserRepository
// • Can't test UserRepository without real database
// • High-level depends on low-level implementation
}

namespace good_example1 {
// ✅ GOOD: Both depend on abstraction

// Abstraction (interface)
class IDatabase {
public:
    virtual ~IDatabase() = default;
    virtual void connect() = 0;
    virtual void query(const string& sql) = 0;
    virtual void disconnect() = 0;
};

// Low-level implementations
class MySQLDatabase : public IDatabase {
public:
    void connect() override {
        cout << "   🔌 Connected to MySQL database" << endl;
    }

    void query(const string& sql) override {
        cout << "   📊 Executing MySQL query: " << sql << endl;
    }

    void disconnect() override {
        cout << "   🔌 Disconnected from MySQL" << endl;
    }
};

class PostgreSQLDatabase : public IDatabase {
public:
    void connect() override {
        cout << "   🔌 Connected to PostgreSQL database" << endl;
    }

    void query(const string& sql) override {
        cout << "   📊 Executing PostgreSQL query: " << sql << endl;
    }

    void disconnect() override {
        cout << "   🔌 Disconnected from PostgreSQL" << endl;
    }
};

class MongoDatabase : public IDatabase {
public:
    void connect() override {
        cout << "   🔌 Connected to MongoDB" << endl;
    }

    void query(const string& sql) override {
        cout << "   📊 Executing MongoDB query: " << sql << endl;
    }

    void disconnect() override {
        cout << "   🔌 Disconnected from MongoDB" << endl;
    }
};

// High-level module depends on abstraction
class UserRepository {
private:
    IDatabase* database;  // ✅ Depends on interface!

public:
    // Dependency Injection through constructor
    UserRepository(IDatabase* db) : database(db) {}

    void getUser(int id) {
        database->connect();
        database->query("SELECT * FROM users WHERE id = " + to_string(id));
        database->disconnect();
    }

    void saveUser(const string& name) {
        database->connect();
        database->query("INSERT INTO users (name) VALUES ('" + name + "')");
        database->disconnect();
    }
};
}

void example1_database() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 1: DATABASE CONNECTION                 ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (Concrete Dependency):\n";
    cout << "```cpp\n";
    cout << "class UserRepository {\n";
    cout << "    MySQLDatabase database;  // 🚨 Tight coupling!\n";
    cout << "};\n";
    cout << "```\n";

    cout << "\n🚨 PROBLEMS:\n";
    cout << "   • Tightly coupled to MySQL\n";
    cout << "   • Can't switch databases without changing code\n";
    cout << "   • Can't test without real database\n";
    cout << "   • High-level depends on low-level\n";

    cout << "\n✅ GOOD CODE (Depends on Abstraction):\n";
    cout << "```cpp\n";
    cout << "class IDatabase { virtual void query() = 0; };\n\n";
    cout << "class UserRepository {\n";
    cout << "    IDatabase* database;  // ✅ Depends on interface!\n";
    cout << "public:\n";
    cout << "    UserRepository(IDatabase* db) : database(db) {}\n";
    cout << "};\n";
    cout << "```\n";

    cout << "\n✨ DEMONSTRATION:\n";

    // Can use any database implementation!
    good_example1::MySQLDatabase mysql;
    good_example1::PostgreSQLDatabase postgres;
    good_example1::MongoDatabase mongo;

    cout << "\n   Using MySQL:\n";
    good_example1::UserRepository repo1(&mysql);
    repo1.getUser(1);

    cout << "\n   Using PostgreSQL:\n";
    good_example1::UserRepository repo2(&postgres);
    repo2.saveUser("Alice");

    cout << "\n   Using MongoDB:\n";
    good_example1::UserRepository repo3(&mongo);
    repo3.getUser(2);

    cout << "\n✨ BENEFITS:\n";
    cout << "   • Easy to switch database implementations\n";
    cout << "   • UserRepository unchanged when adding new databases\n";
    cout << "   • Can test with mock database\n";
    cout << "   • High-level and low-level both depend on abstraction\n";
}

// ============================================
// EXAMPLE 2: NOTIFICATION SYSTEM
// ============================================

namespace bad_example2 {
// ❌ BAD: Depends on concrete notification classes

class EmailService {
public:
    void sendEmail(const string& message) {
        cout << "   📧 Sending email: " << message << endl;
    }
};

class SMSService {
public:
    void sendSMS(const string& message) {
        cout << "   📱 Sending SMS: " << message << endl;
    }
};

// 🚨 High-level depends on concrete implementations
class NotificationService {
private:
    EmailService emailService;  // 🚨 Tight coupling!
    SMSService smsService;      // 🚨 Tight coupling!

public:
    void sendNotification(const string& message, bool useEmail) {
        if (useEmail) {
            emailService.sendEmail(message);
        } else {
            smsService.sendSMS(message);
        }
    }

    // To add push notifications, we'd need to:
    // 1. Add PushService member
    // 2. Modify sendNotification method
    // 3. Recompile and test everything
};
}

namespace good_example2 {
// ✅ GOOD: Depends on abstraction

class INotifier {
public:
    virtual ~INotifier() = default;
    virtual void send(const string& message) = 0;
};

class EmailNotifier : public INotifier {
public:
    void send(const string& message) override {
        cout << "   📧 Sending email: " << message << endl;
    }
};

class SMSNotifier : public INotifier {
public:
    void send(const string& message) override {
        cout << "   📱 Sending SMS: " << message << endl;
    }
};

class PushNotifier : public INotifier {
public:
    void send(const string& message) override {
        cout << "   🔔 Sending push notification: " << message << endl;
    }
};

class SlackNotifier : public INotifier {
public:
    void send(const string& message) override {
        cout << "   💬 Sending Slack message: " << message << endl;
    }
};

// High-level depends on abstraction
class NotificationService {
private:
    vector<INotifier*> notifiers;  // ✅ Depends on interface!

public:
    void addNotifier(INotifier* notifier) {
        notifiers.push_back(notifier);
    }

    void sendNotification(const string& message) {
        for (auto notifier : notifiers) {
            notifier->send(message);
        }
    }
};
}

void example2_notifications() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 2: NOTIFICATION SYSTEM                 ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE:\n";
    cout << "   NotificationService depends on EmailService and SMSService\n";
    cout << "   To add push notifications, need to modify NotificationService\n";

    cout << "\n✅ GOOD CODE:\n";
    cout << "   NotificationService depends on INotifier interface\n";
    cout << "   Can add new notifiers without changing NotificationService\n";

    cout << "\n✨ DEMONSTRATION:\n";

    good_example2::EmailNotifier email;
    good_example2::SMSNotifier sms;
    good_example2::PushNotifier push;
    good_example2::SlackNotifier slack;

    good_example2::NotificationService service;

    cout << "\n   Sending via Email and SMS:\n";
    service.addNotifier(&email);
    service.addNotifier(&sms);
    service.sendNotification("Hello, World!");

    cout << "\n   Adding Push and Slack notifications:\n";
    service.addNotifier(&push);
    service.addNotifier(&slack);
    service.sendNotification("New feature released!");

    cout << "\n✨ BENEFITS:\n";
    cout << "   • Easy to add new notification methods\n";
    cout << "   • NotificationService never changes\n";
    cout << "   • Can test with mock notifiers\n";
    cout << "   • Open/Closed Principle achieved\n";
}

// ============================================
// EXAMPLE 3: PAYMENT PROCESSING
// ============================================

namespace bad_example3 {
// ❌ BAD: Depends on concrete payment gateways

class PayPalGateway {
public:
    void processPayment(double amount) {
        cout << "   💳 Processing $" << amount << " via PayPal" << endl;
    }
};

class StripeGateway {
public:
    void processPayment(double amount) {
        cout << "   💳 Processing $" << amount << " via Stripe" << endl;
    }
};

// 🚨 High-level depends on concrete gateways
class PaymentProcessor {
private:
    PayPalGateway paypal;  // 🚨 Tight coupling!
    StripeGateway stripe;  // 🚨 Tight coupling!

public:
    void processPayment(double amount, const string& gateway) {
        if (gateway == "paypal") {
            paypal.processPayment(amount);
        } else if (gateway == "stripe") {
            stripe.processPayment(amount);
        }
        // Need to modify this method for each new gateway!
    }
};
}

namespace good_example3 {
// ✅ GOOD: Depends on abstraction

class IPaymentGateway {
public:
    virtual ~IPaymentGateway() = default;
    virtual void processPayment(double amount) = 0;
    virtual string getName() const = 0;
};

class PayPalGateway : public IPaymentGateway {
public:
    void processPayment(double amount) override {
        cout << "   💳 Processing $" << amount << " via PayPal" << endl;
    }

    string getName() const override {
        return "PayPal";
    }
};

class StripeGateway : public IPaymentGateway {
public:
    void processPayment(double amount) override {
        cout << "   💳 Processing $" << amount << " via Stripe" << endl;
    }

    string getName() const override {
        return "Stripe";
    }
};

class SquareGateway : public IPaymentGateway {
public:
    void processPayment(double amount) override {
        cout << "   💳 Processing $" << amount << " via Square" << endl;
    }

    string getName() const override {
        return "Square";
    }
};

class CryptoGateway : public IPaymentGateway {
public:
    void processPayment(double amount) override {
        cout << "   ₿ Processing $" << amount << " via Cryptocurrency" << endl;
    }

    string getName() const override {
        return "Crypto";
    }
};

// High-level depends on abstraction
class PaymentProcessor {
private:
    IPaymentGateway* gateway;  // ✅ Depends on interface!

public:
    PaymentProcessor(IPaymentGateway* gw) : gateway(gw) {}

    void processPayment(double amount) {
        cout << "   🔄 Using " << gateway->getName() << " gateway" << endl;
        gateway->processPayment(amount);
        cout << "   ✅ Payment successful" << endl;
    }

    void setGateway(IPaymentGateway* gw) {
        gateway = gw;
    }
};
}

void example3_payment() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 3: PAYMENT PROCESSING                  ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE:\n";
    cout << "   PaymentProcessor has if/else for each gateway\n";
    cout << "   Must modify code to add new payment gateway\n";

    cout << "\n✅ GOOD CODE:\n";
    cout << "   PaymentProcessor depends on IPaymentGateway\n";
    cout << "   Can inject any gateway implementation\n";

    cout << "\n✨ DEMONSTRATION:\n";

    good_example3::PayPalGateway paypal;
    good_example3::StripeGateway stripe;
    good_example3::SquareGateway square;
    good_example3::CryptoGateway crypto;

    cout << "\n   Processing with PayPal:\n";
    good_example3::PaymentProcessor processor1(&paypal);
    processor1.processPayment(100.00);

    cout << "\n   Switching to Stripe:\n";
    processor1.setGateway(&stripe);
    processor1.processPayment(250.00);

    cout << "\n   Processing with Square:\n";
    good_example3::PaymentProcessor processor2(&square);
    processor2.processPayment(75.50);

    cout << "\n   Processing with Crypto:\n";
    good_example3::PaymentProcessor processor3(&crypto);
    processor3.processPayment(500.00);

    cout << "\n✨ BENEFITS:\n";
    cout << "   • PaymentProcessor unchanged when adding gateways\n";
    cout << "   • Runtime gateway switching\n";
    cout << "   • Easy to test with mock gateway\n";
    cout << "   • Follows Open/Closed Principle\n";
}

// ============================================
// EXAMPLE 4: LOGGING SYSTEM
// ============================================

namespace bad_example4 {
// ❌ BAD: Depends on concrete logger

class FileLogger {
public:
    void log(const string& message) {
        cout << "   📝 [FILE] " << message << endl;
    }
};

// 🚨 Business logic depends on concrete logger
class OrderService {
private:
    FileLogger logger;  // 🚨 Tight coupling!

public:
    void placeOrder(const string& product) {
        logger.log("Order placed for: " + product);
        cout << "   ✅ Order processed" << endl;
    }
};
}

namespace good_example4 {
// ✅ GOOD: Depends on abstraction

class ILogger {
public:
    virtual ~ILogger() = default;
    virtual void log(const string& message) = 0;
};

class FileLogger : public ILogger {
public:
    void log(const string& message) override {
        cout << "   📝 [FILE] " << message << endl;
    }
};

class ConsoleLogger : public ILogger {
public:
    void log(const string& message) override {
        cout << "   🖥️  [CONSOLE] " << message << endl;
    }
};

class DatabaseLogger : public ILogger {
public:
    void log(const string& message) override {
        cout << "   💾 [DATABASE] " << message << endl;
    }
};

class CloudLogger : public ILogger {
public:
    void log(const string& message) override {
        cout << "   ☁️  [CLOUD] " << message << endl;
    }
};

// Business logic depends on abstraction
class OrderService {
private:
    ILogger* logger;  // ✅ Depends on interface!

public:
    OrderService(ILogger* log) : logger(log) {}

    void placeOrder(const string& product) {
        logger->log("Order placed for: " + product);
        cout << "   ✅ Order processed" << endl;
    }

    void cancelOrder(int orderId) {
        logger->log("Order cancelled: #" + to_string(orderId));
        cout << "   ❌ Order cancelled" << endl;
    }
};
}

void example4_logging() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 4: LOGGING SYSTEM                      ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE:\n";
    cout << "   OrderService hardcoded to use FileLogger\n";
    cout << "   Can't change logging destination without modifying code\n";

    cout << "\n✅ GOOD CODE:\n";
    cout << "   OrderService depends on ILogger interface\n";
    cout << "   Logger injected via constructor\n";

    cout << "\n✨ DEMONSTRATION:\n";

    good_example4::FileLogger fileLogger;
    good_example4::ConsoleLogger consoleLogger;
    good_example4::DatabaseLogger dbLogger;
    good_example4::CloudLogger cloudLogger;

    cout << "\n   Using File Logger:\n";
    good_example4::OrderService service1(&fileLogger);
    service1.placeOrder("Laptop");

    cout << "\n   Using Console Logger:\n";
    good_example4::OrderService service2(&consoleLogger);
    service2.placeOrder("Mouse");

    cout << "\n   Using Database Logger:\n";
    good_example4::OrderService service3(&dbLogger);
    service3.cancelOrder(12345);

    cout << "\n   Using Cloud Logger:\n";
    good_example4::OrderService service4(&cloudLogger);
    service4.placeOrder("Keyboard");

    cout << "\n✨ BENEFITS:\n";
    cout << "   • OrderService doesn't know about logging details\n";
    cout << "   • Can switch loggers without changing business logic\n";
    cout << "   • Easy to test with mock logger\n";
    cout << "   • Separation of concerns\n";
}

// ============================================
// EXAMPLE 5: DEPENDENCY INJECTION PATTERNS
// ============================================

namespace example5 {
// Interface
class IMessageService {
public:
    virtual ~IMessageService() = default;
    virtual void sendMessage(const string& message) = 0;
};

class EmailService : public IMessageService {
public:
    void sendMessage(const string& message) override {
        cout << "   📧 Email sent: " << message << endl;
    }
};

class SMSService : public IMessageService {
public:
    void sendMessage(const string& message) override {
        cout << "   📱 SMS sent: " << message << endl;
    }
};

// 1. Constructor Injection (Most common)
class UserService_Constructor {
private:
    IMessageService* messageService;

public:
    UserService_Constructor(IMessageService* service)
        : messageService(service) {}

    void notifyUser(const string& username) {
        messageService->sendMessage("Hello, " + username);
    }
};

// 2. Setter Injection
class UserService_Setter {
private:
    IMessageService* messageService = nullptr;

public:
    void setMessageService(IMessageService* service) {
        messageService = service;
    }

    void notifyUser(const string& username) {
        if (messageService) {
            messageService->sendMessage("Hello, " + username);
        }
    }
};

// 3. Method Injection
class UserService_Method {
public:
    void notifyUser(const string& username, IMessageService* service) {
        service->sendMessage("Hello, " + username);
    }
};
}

void example5_dependency_injection() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 5: DEPENDENCY INJECTION PATTERNS       ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n✅ THREE WAYS TO INJECT DEPENDENCIES:\n";

    example5::EmailService email;
    example5::SMSService sms;

    cout << "\n1. CONSTRUCTOR INJECTION (Recommended):\n";
    cout << "   • Dependencies required at construction\n";
    cout << "   • Immutable after creation\n";
    cout << "   • Clear requirements\n";
    example5::UserService_Constructor service1(&email);
    service1.notifyUser("Alice");

    cout << "\n2. SETTER INJECTION:\n";
    cout << "   • Optional dependencies\n";
    cout << "   • Can change dependencies after creation\n";
    cout << "   • More flexible but less clear\n";
    example5::UserService_Setter service2;
    service2.setMessageService(&sms);
    service2.notifyUser("Bob");

    cout << "\n3. METHOD INJECTION:\n";
    cout << "   • Dependency needed only for specific method\n";
    cout << "   • Different dependency per method call\n";
    cout << "   • Good for occasional dependencies\n";
    example5::UserService_Method service3;
    service3.notifyUser("Charlie", &email);
    service3.notifyUser("David", &sms);

    cout << "\n💡 RECOMMENDATION:\n";
    cout << "   • Use Constructor Injection as default\n";
    cout << "   • Use Setter Injection for optional dependencies\n";
    cout << "   • Use Method Injection for per-call dependencies\n";
}

// ============================================
// EXAMPLE 6: HOW TO FOLLOW DIP
// ============================================

void example6_how_to_follow() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 6: HOW TO FOLLOW DIP                   ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n✅ GUIDELINES TO FOLLOW DIP:\n";

    cout << "\n1. IDENTIFY ABSTRACTIONS:\n";
    cout << "   • What are the high-level policies?\n";
    cout << "   • What are the low-level details?\n";
    cout << "   • Create interfaces for volatile dependencies\n";

    cout << "\n2. DEPEND ON INTERFACES:\n";
    cout << "   ❌ MyClass myField;           // Concrete\n";
    cout << "   ✅ IMyInterface* myField;     // Interface\n";
    cout << "   ✅ unique_ptr<IMyInterface>   // Smart pointer\n";

    cout << "\n3. USE DEPENDENCY INJECTION:\n";
    cout << "   • Constructor injection (preferred)\n";
    cout << "   • Setter injection (optional deps)\n";
    cout << "   • Method injection (per-call deps)\n";

    cout << "\n4. AVOID THESE PATTERNS:\n";
    cout << "   ❌ new ConcreteClass() in high-level code\n";
    cout << "   ❌ Concrete types in method parameters\n";
    cout << "   ❌ High-level importing low-level modules\n";
    cout << "   ❌ if/else chains for different implementations\n";

    cout << "\n5. FOLLOW THE DEPENDENCY RULE:\n";
    cout << "   Source code dependencies must point inward\n";
    cout << "   \n";
    cout << "   ┌─────────────────────┐\n";
    cout << "   │   High-Level        │\n";
    cout << "   │   (Business Logic)  │\n";
    cout << "   └──────────┬──────────┘\n";
    cout << "              │ depends on\n";
    cout << "              ↓\n";
    cout << "   ┌──────────────────────┐\n";
    cout << "   │    Abstraction       │\n";
    cout << "   │    (Interface)       │\n";
    cout << "   └──────────┬───────────┘\n";
    cout << "              ↑ implements\n";
    cout << "              │\n";
    cout << "   ┌──────────┴──────────┐\n";
    cout << "   │   Low-Level         │\n";
    cout << "   │   (Implementation)  │\n";
    cout << "   └─────────────────────┘\n";

    cout << "\n6. STABLE VS VOLATILE:\n";
    cout << "   • Stable: Doesn't change often (interfaces)\n";
    cout << "   • Volatile: Changes frequently (implementations)\n";
    cout << "   • Depend on stable abstractions, not volatile details\n";

    cout << "\n🔍 CHECKLIST:\n";
    cout << "   □ High-level modules depend on abstractions?\n";
    cout << "   □ Low-level modules implement abstractions?\n";
    cout << "   □ Dependencies injected from outside?\n";
    cout << "   □ No 'new' keyword in high-level code?\n";
    cout << "   □ Easy to swap implementations?\n";
    cout << "   □ Can test with mock objects?\n";
}

// ============================================
// EXAMPLE 7: BENEFITS & SUMMARY
// ============================================

void example7_benefits() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 7: BENEFITS & SUMMARY                  ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n✅ BENEFITS OF DIP:\n";

    cout << "\n1. FLEXIBILITY:\n";
    cout << "   • Easy to swap implementations\n";
    cout << "   • Can change low-level details without affecting high-level\n";
    cout << "   • Runtime selection of implementations\n";

    cout << "\n2. TESTABILITY:\n";
    cout << "   • Can inject mock objects for testing\n";
    cout << "   • Test high-level logic without dependencies\n";
    cout << "   • Isolated unit tests\n";

    cout << "\n3. MAINTAINABILITY:\n";
    cout << "   • Changes localized to implementations\n";
    cout << "   • High-level code stable\n";
    cout << "   • Clear separation of concerns\n";

    cout << "\n4. REUSABILITY:\n";
    cout << "   • High-level modules can be reused\n";
    cout << "   • Different implementations for different contexts\n";
    cout << "   • Plug-and-play architecture\n";

    cout << "\n5. PARALLEL DEVELOPMENT:\n";
    cout << "   • Teams can work on interfaces first\n";
    cout << "   • Implement high-level and low-level independently\n";
    cout << "   • Mock implementations for early testing\n";

    cout << "\n⚠️  WITHOUT DIP:\n";
    cout << "   • Tight coupling between layers\n";
    cout << "   • Hard to test\n";
    cout << "   • Changes ripple through system\n";
    cout << "   • Difficult to swap implementations\n";
    cout << "   • High-level depends on low-level details\n";

    cout << "\n💡 KEY INSIGHT:\n";
    cout << "   \"Depend upon abstractions, not concretions.\n";
    cout << "    Invert the traditional dependency direction!\"\n";

    cout << "\n🎯 REMEMBER:\n";
    cout << "   • High-level should not depend on low-level\n";
    cout << "   • Both depend on abstractions\n";
    cout << "   • Abstractions should not depend on details\n";
    cout << "   • Details depend on abstractions\n";

    cout << "\n📊 DIP ACHIEVES:\n";
    cout << "   ✅ Loose coupling\n";
    cout << "   ✅ High cohesion\n";
    cout << "   ✅ Easy testing\n";
    cout << "   ✅ Flexible architecture\n";
    cout << "   ✅ Open/Closed Principle\n";

    cout << "\n🔄 THE INVERSION:\n";
    cout << "   Before DIP: High → Low\n";
    cout << "   After DIP:  High → Interface ← Low\n";
    cout << "   \n";
    cout << "   Dependencies now flow through abstraction layer!\n";
}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║      DEPENDENCY INVERSION PRINCIPLE (DIP)             ║\n";
    cout << "║                                                       ║\n";
    cout << "║  \"Depend upon abstractions, not concretions.\"         ║\n";
    cout << "║  \"High-level modules should not depend on             ║\n";
    cout << "║   low-level modules. Both should depend on            ║\n";
    cout << "║   abstractions.\"                                     ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n";

    example1_database();
    example2_notifications();
    example3_payment();
    example4_logging();
    example5_dependency_injection();
    example6_how_to_follow();
    example7_benefits();

    cout << "\n\n";
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║    🎉 DEPENDENCY INVERSION PRINCIPLE COMPLETE! 🎉     ║\n";
    cout << "║                                                       ║\n";
    cout << "║  Key Takeaway: Invert dependencies by depending on    ║\n";
    cout << "║  abstractions, not concrete implementations.          ║\n";
    cout << "║  High-level and low-level both depend on interfaces!  ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n\n";

    return 0;
}
