#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#include <print>

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
        std::println("   🔌 Connected to MySQL database");
    }

    void query(const std::string& sql) {
        std::println("   📊 Executing MySQL query: {}", sql);
    }

    void disconnect() {
        std::println("   🔌 Disconnected from MySQL");
    }
};

// 🚨 High-level module depends on concrete MySQLDatabase
class UserRepository {
private:
    MySQLDatabase database;  // 🚨 Tight coupling!

public:
    void getUser(int id) {
        database.connect();
        database.query("SELECT * FROM users WHERE id = " + std::to_string(id));
        database.disconnect();
    }

    void saveUser(const std::string& name) {
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
    virtual void query(const std::string& sql) = 0;
    virtual void disconnect() = 0;
};

// Low-level implementations
class MySQLDatabase : public IDatabase {
public:
    void connect() override {
        std::println("   🔌 Connected to MySQL database");
    }

    void query(const std::string& sql) override {
        std::println("   📊 Executing MySQL query: {}", sql);
    }

    void disconnect() override {
        std::println("   🔌 Disconnected from MySQL");
    }
};

class PostgreSQLDatabase : public IDatabase {
public:
    void connect() override {
        std::println("   🔌 Connected to PostgreSQL database");
    }

    void query(const std::string& sql) override {
        std::println("   📊 Executing PostgreSQL query: {}", sql);
    }

    void disconnect() override {
        std::println("   🔌 Disconnected from PostgreSQL");
    }
};

class MongoDatabase : public IDatabase {
public:
    void connect() override {
        std::println("   🔌 Connected to MongoDB");
    }

    void query(const std::string& sql) override {
        std::println("   📊 Executing MongoDB query: {}", sql);
    }

    void disconnect() override {
        std::println("   🔌 Disconnected from MongoDB");
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
        database->query("SELECT * FROM users WHERE id = " + std::to_string(id));
        database->disconnect();
    }

    void saveUser(const std::string& name) {
        database->connect();
        database->query("INSERT INTO users (name) VALUES ('" + name + "')");
        database->disconnect();
    }
};
}

void example1_database() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 1: DATABASE CONNECTION                 ║");
    std::println("╚══════════════════════════════════════════════════╝");

    // Can use any database implementation!
    good_example1::MySQLDatabase mysql;
    good_example1::PostgreSQLDatabase postgres;
    good_example1::MongoDatabase mongo;

    std::println("");
    std::println("   Using MySQL:");
    good_example1::UserRepository repo1(&mysql);
    repo1.getUser(1);

    std::println("");
    std::println("   Using PostgreSQL:");
    good_example1::UserRepository repo2(&postgres);
    repo2.saveUser("Alice");

    std::println("");
    std::println("   Using MongoDB:");
    good_example1::UserRepository repo3(&mongo);
    repo3.getUser(2);

}

// ============================================
// EXAMPLE 2: NOTIFICATION SYSTEM
// ============================================

namespace bad_example2 {
// ❌ BAD: Depends on concrete notification classes

class EmailService {
public:
    void sendEmail(const std::string& message) {
        std::println("   📧 Sending email: {}", message);
    }
};

class SMSService {
public:
    void sendSMS(const std::string& message) {
        std::println("   📱 Sending SMS: {}", message);
    }
};

// 🚨 High-level depends on concrete implementations
class NotificationService {
private:
    EmailService emailService;  // 🚨 Tight coupling!
    SMSService smsService;      // 🚨 Tight coupling!

public:
    void sendNotification(const std::string& message, bool useEmail) {
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
    virtual void send(const std::string& message) = 0;
};

class EmailNotifier : public INotifier {
public:
    void send(const std::string& message) override {
        std::println("   📧 Sending email: {}", message);
    }
};

class SMSNotifier : public INotifier {
public:
    void send(const std::string& message) override {
        std::println("   📱 Sending SMS: {}", message);
    }
};

class PushNotifier : public INotifier {
public:
    void send(const std::string& message) override {
        std::println("   🔔 Sending push notification: {}", message);
    }
};

class SlackNotifier : public INotifier {
public:
    void send(const std::string& message) override {
        std::println("   💬 Sending Slack message: {}", message);
    }
};

// High-level depends on abstraction
class NotificationService {
private:
    std::vector<INotifier*> notifiers;  // ✅ Depends on interface!

public:
    void addNotifier(INotifier* notifier) {
        notifiers.push_back(notifier);
    }

    void sendNotification(const std::string& message) {
        for (auto notifier : notifiers) {
            notifier->send(message);
        }
    }
};
}

void example2_notifications() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 2: NOTIFICATION SYSTEM                 ║");
    std::println("╚══════════════════════════════════════════════════╝");

    good_example2::EmailNotifier email;
    good_example2::SMSNotifier sms;
    good_example2::PushNotifier push;
    good_example2::SlackNotifier slack;

    good_example2::NotificationService service;

    std::println("");
    std::println("   Sending via Email and SMS:");
    service.addNotifier(&email);
    service.addNotifier(&sms);
    service.sendNotification("Hello, World!");

    std::println("");
    std::println("   Adding Push and Slack notifications:");
    service.addNotifier(&push);
    service.addNotifier(&slack);
    service.sendNotification("New feature released!");

}

// ============================================
// EXAMPLE 3: PAYMENT PROCESSING
// ============================================

namespace bad_example3 {
// ❌ BAD: Depends on concrete payment gateways

class PayPalGateway {
public:
    void processPayment(double amount) {
        std::println("   💳 Processing ${} via PayPal", amount);
    }
};

class StripeGateway {
public:
    void processPayment(double amount) {
        std::println("   💳 Processing ${} via Stripe", amount);
    }
};

// 🚨 High-level depends on concrete gateways
class PaymentProcessor {
private:
    PayPalGateway paypal;  // 🚨 Tight coupling!
    StripeGateway stripe;  // 🚨 Tight coupling!

public:
    void processPayment(double amount, const std::string& gateway) {
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
    virtual std::string getName() const = 0;
};

class PayPalGateway : public IPaymentGateway {
public:
    void processPayment(double amount) override {
        std::println("   💳 Processing ${} via PayPal", amount);
    }

    std::string getName() const override {
        return "PayPal";
    }
};

class StripeGateway : public IPaymentGateway {
public:
    void processPayment(double amount) override {
        std::println("   💳 Processing ${} via Stripe", amount);
    }

    std::string getName() const override {
        return "Stripe";
    }
};

class SquareGateway : public IPaymentGateway {
public:
    void processPayment(double amount) override {
        std::println("   💳 Processing ${} via Square", amount);
    }

    std::string getName() const override {
        return "Square";
    }
};

class CryptoGateway : public IPaymentGateway {
public:
    void processPayment(double amount) override {
        std::println("   ₿ Processing ${} via Cryptocurrency", amount);
    }

    std::string getName() const override {
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
        std::println("   🔄 Using {} gateway", gateway->getName());
        gateway->processPayment(amount);
        std::println("   ✅ Payment successful");
    }

    void setGateway(IPaymentGateway* gw) {
        gateway = gw;
    }
};
}

void example3_payment() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 3: PAYMENT PROCESSING                  ║");
    std::println("╚══════════════════════════════════════════════════╝");

    good_example3::PayPalGateway paypal;
    good_example3::StripeGateway stripe;
    good_example3::SquareGateway square;
    good_example3::CryptoGateway crypto;

    std::println("");
    std::println("   Processing with PayPal:");
    good_example3::PaymentProcessor processor1(&paypal);
    processor1.processPayment(100.00);

    std::println("");
    std::println("   Switching to Stripe:");
    processor1.setGateway(&stripe);
    processor1.processPayment(250.00);

    std::println("");
    std::println("   Processing with Square:");
    good_example3::PaymentProcessor processor2(&square);
    processor2.processPayment(75.50);

    std::println("");
    std::println("   Processing with Crypto:");
    good_example3::PaymentProcessor processor3(&crypto);
    processor3.processPayment(500.00);

}

// ============================================
// EXAMPLE 4: LOGGING SYSTEM
// ============================================

namespace bad_example4 {
// ❌ BAD: Depends on concrete logger

class FileLogger {
public:
    void log(const std::string& message) {
        std::println("   📝 [FILE] {}", message);
    }
};

// 🚨 Business logic depends on concrete logger
class OrderService {
private:
    FileLogger logger;  // 🚨 Tight coupling!

public:
    void placeOrder(const std::string& product) {
        logger.log("Order placed for: " + product);
        std::println("   ✅ Order processed");
    }
};
}

namespace good_example4 {
// ✅ GOOD: Depends on abstraction

class ILogger {
public:
    virtual ~ILogger() = default;
    virtual void log(const std::string& message) = 0;
};

class FileLogger : public ILogger {
public:
    void log(const std::string& message) override {
        std::println("   📝 [FILE] {}", message);
    }
};

class ConsoleLogger : public ILogger {
public:
    void log(const std::string& message) override {
        std::println("   🖥️  [CONSOLE] {}", message);
    }
};

class DatabaseLogger : public ILogger {
public:
    void log(const std::string& message) override {
        std::println("   💾 [DATABASE] {}", message);
    }
};

class CloudLogger : public ILogger {
public:
    void log(const std::string& message) override {
        std::println("   ☁️  [CLOUD] {}", message);
    }
};

// Business logic depends on abstraction
class OrderService {
private:
    ILogger* logger;  // ✅ Depends on interface!

public:
    OrderService(ILogger* log) : logger(log) {}

    void placeOrder(const std::string& product) {
        logger->log("Order placed for: " + product);
        std::println("   ✅ Order processed");
    }

    void cancelOrder(int orderId) {
        logger->log("Order cancelled: #" + std::to_string(orderId));
        std::println("   ❌ Order cancelled");
    }
};
}

void example4_logging() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 4: LOGGING SYSTEM                      ║");
    std::println("╚══════════════════════════════════════════════════╝");

    good_example4::FileLogger fileLogger;
    good_example4::ConsoleLogger consoleLogger;
    good_example4::DatabaseLogger dbLogger;
    good_example4::CloudLogger cloudLogger;

    std::println("");
    std::println("   Using File Logger:");
    good_example4::OrderService service1(&fileLogger);
    service1.placeOrder("Laptop");

    std::println("");
    std::println("   Using Console Logger:");
    good_example4::OrderService service2(&consoleLogger);
    service2.placeOrder("Mouse");

    std::println("");
    std::println("   Using Database Logger:");
    good_example4::OrderService service3(&dbLogger);
    service3.cancelOrder(12345);

    std::println("");
    std::println("   Using Cloud Logger:");
    good_example4::OrderService service4(&cloudLogger);
    service4.placeOrder("Keyboard");

}

// ============================================
// EXAMPLE 5: DEPENDENCY INJECTION PATTERNS
// ============================================

namespace example5 {
// Interface
class IMessageService {
public:
    virtual ~IMessageService() = default;
    virtual void sendMessage(const std::string& message) = 0;
};

class EmailService : public IMessageService {
public:
    void sendMessage(const std::string& message) override {
        std::println("   📧 Email sent: {}", message);
    }
};

class SMSService : public IMessageService {
public:
    void sendMessage(const std::string& message) override {
        std::println("   📱 SMS sent: {}", message);
    }
};

// 1. Constructor Injection (Most common)
class UserService_Constructor {
private:
    IMessageService* messageService;

public:
    UserService_Constructor(IMessageService* service)
        : messageService(service) {}

    void notifyUser(const std::string& username) {
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

    void notifyUser(const std::string& username) {
        if (messageService) {
            messageService->sendMessage("Hello, " + username);
        }
    }
};

// 3. Method Injection
class UserService_Method {
public:
    void notifyUser(const std::string& username, IMessageService* service) {
        service->sendMessage("Hello, " + username);
    }
};
}

void example5_dependency_injection() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 5: DEPENDENCY INJECTION PATTERNS       ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("");
    std::println("✅ THREE WAYS TO INJECT DEPENDENCIES:");

    example5::EmailService email;
    example5::SMSService sms;

    example5::UserService_Constructor service1(&email);
    service1.notifyUser("Alice");

    example5::UserService_Setter service2;
    service2.setMessageService(&sms);
    service2.notifyUser("Bob");

    example5::UserService_Method service3;
    service3.notifyUser("Charlie", &email);
    service3.notifyUser("David", &sms);

}

// ============================================
// EXAMPLE 6: HOW TO FOLLOW DIP
// ============================================

void example6_how_to_follow() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 6: HOW TO FOLLOW DIP                   ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   ");

}

// ============================================
// EXAMPLE 7: BENEFITS & SUMMARY
// ============================================

void example7_benefits() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 7: BENEFITS & SUMMARY                  ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   ");
}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    std::println("╔═══════════════════════════════════════════════════════╗");
    std::println("║                                                       ║");
    std::println("║      DEPENDENCY INVERSION PRINCIPLE (DIP)             ║");
    std::println("║                                                       ║");
    std::println("║  \"Depend upon abstractions, not concretions.\"       ║");
    std::println("║  \"High-level modules should not depend on            ║");
    std::println("║   low-level modules. Both should depend on            ║");
    std::println("║   abstractions.\"                                     ║");
    std::println("║                                                       ║");
    std::println("╚═══════════════════════════════════════════════════════╝");

    example1_database();
    example2_notifications();
    example3_payment();
    example4_logging();
    example5_dependency_injection();
    example6_how_to_follow();
    example7_benefits();

    return 0;
}
