// =============================================================================
// Coupling — C++23
//
// Demonstrates all major coupling types with before/after comparison.
//
// BEFORE: OrderService tightly coupled to MySqlDatabase
//   - hardcoded construction, direct method calls, global state
// AFTER:  OrderService loosely coupled via IDatabase interface
//   - constructor injection, swap DB without touching OrderService
//   - MockDatabase enables unit testing with zero real DB
// =============================================================================

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <cassert>
#include <stdexcept>
#include <print>

// =============================================================================
// ❌ BEFORE — Tight Coupling examples
// =============================================================================

namespace tight {

// ── Tight Coupling Type 1: Content coupling — reads internals directly ────────
struct BankAccount {
    std::string owner;
    double balance;       // public field — anything can touch it
};

class TightTransfer {
public:
    void transfer(BankAccount& from, BankAccount& to, double amount) {
        // Directly manipulates internal state — content coupling
        from.balance -= amount;   // bypasses any validation in BankAccount
        to.balance   += amount;
    }
};

// ── Tight Coupling Type 2: Control coupling — bool flag controls behavior ─────
class TightLogger {
public:
    void log(const std::string& msg, bool isError, bool toFile, bool toConsole) {
        // Caller controls internal behavior via flags — control coupling
        if (isError && toConsole)
            std::println("  [ERROR] {}", msg);
        else if (toConsole)
            std::println("  [INFO]  {}", msg);
        if (toFile)
            file_log_.push_back(msg);
    }
    std::vector<std::string> file_log_;
};

// ── Tight Coupling Type 3: Direct construction — can never swap implementation
class MySqlDatabase {
public:
    void saveOrder(int id, const std::string& item) {
        store_[id] = item;
        std::println("  [MySql] saved order #{}", id);
    }
    std::string getOrder(int id) const {
        auto it = store_.find(id);
        return it != store_.end() ? it->second : "";
    }
private:
    std::unordered_map<int, std::string> store_;
};

class TightOrderService {
public:
    TightOrderService() : db_(std::make_unique<MySqlDatabase>()) {}
    //                         ^^^ hardcoded — cannot ever swap without editing this class

    void placeOrder(int id, const std::string& item) {
        db_->saveOrder(id, item);   // calling concrete methods — tight coupling
    }

private:
    std::unique_ptr<MySqlDatabase> db_;   // concrete type — not an interface
};

} // namespace tight

// =============================================================================
// ✅ AFTER — Loose Coupling via interfaces and injection
// =============================================================================

namespace loose {

// ── Interface: what OrderService needs from storage ──────────────────────────
class IDatabase {
public:
    virtual ~IDatabase() = default;
    virtual void        saveOrder(int id, const std::string& item) = 0;
    virtual std::string getOrder(int id) const = 0;
    virtual int         orderCount() const = 0;
};

// ── Interface: what OrderService needs from logging ──────────────────────────
class ILogger {
public:
    virtual ~ILogger() = default;
    virtual void info (const std::string& msg) = 0;
    virtual void error(const std::string& msg) = 0;
};

// ── Concrete implementation 1: real database ─────────────────────────────────
class MySqlDatabase : public IDatabase {
public:
    void saveOrder(int id, const std::string& item) override {
        store_[id] = item;
        std::println("  [MySql]    saved order #{}", id);
    }
    std::string getOrder(int id) const override {
        auto it = store_.find(id);
        return it != store_.end() ? it->second : "";
    }
    int orderCount() const override { return static_cast<int>(store_.size()); }

private:
    std::unordered_map<int, std::string> store_;
};

// ── Concrete implementation 2: in-memory mock for testing ────────────────────
class MockDatabase : public IDatabase {
public:
    void saveOrder(int id, const std::string& item) override {
        store_[id] = item;
        save_calls_++;
    }
    std::string getOrder(int id) const override {
        auto it = store_.find(id);
        return it != store_.end() ? it->second : "";
    }
    int orderCount() const override { return static_cast<int>(store_.size()); }
    int saveCalls()  const { return save_calls_; }

private:
    std::unordered_map<int, std::string> store_;
    int save_calls_{0};
};

// ── Concrete logger: console ──────────────────────────────────────────────────
class ConsoleLogger : public ILogger {
public:
    void info (const std::string& m) override {
        std::println("  [INFO]  {}", m); info_count_++;
    }
    void error(const std::string& m) override {
        std::println("  [ERROR] {}", m); error_count_++;
    }
    int infoCount()  const { return info_count_; }
    int errorCount() const { return error_count_; }
private:
    int info_count_{0}, error_count_{0};
};

// ── Concrete logger: silent (for tests that don't need output) ────────────────
class NullLogger : public ILogger {
public:
    void info (const std::string&) override {}
    void error(const std::string&) override {}
};

// ── OrderService — coupled ONLY to interfaces, not to any concrete class ──────
class OrderService {
public:
    // Constructor injection — receive dependencies, don't create them
    OrderService(IDatabase& db, ILogger& logger)
        : db_(db), logger_(logger) {}

    bool placeOrder(int id, const std::string& item) {
        if (item.empty()) {
            logger_.error("Empty item for order #" + std::to_string(id));
            return false;
        }
        db_.saveOrder(id, item);
        logger_.info("Order #" + std::to_string(id) + " placed: " + item);
        return true;
    }

    std::string getOrder(int id) const {
        return db_.getOrder(id);
    }

    int totalOrders() const { return db_.orderCount(); }

private:
    IDatabase& db_;      // interface — swap MySql ↔ Postgres ↔ Mock freely
    ILogger&   logger_;  // interface — swap Console ↔ File ↔ Null freely
};

} // namespace loose

// =============================================================================
// main
// =============================================================================

int main()
{
    std::println("=== Coupling Demo ===");
    std::println("");

    // ── TIGHT ────────────────────────────────────────────────────────────────
    std::println("--- TIGHT Coupling (before) ---");

    // Content coupling — direct field access, no control, no validation
    tight::BankAccount acc1{"Alice", 1000.0};
    tight::BankAccount acc2{"Bob",   500.0};
    tight::TightTransfer transfer;
    // Note: name is first field but balance is second — easy to swap args!
    // This is a bug waiting to happen with struct field access
    tight::BankAccount a1{"Alice", 1000.0};
    tight::BankAccount a2{"Bob", 500.0};
    transfer.transfer(a1, a2, 200.0);
    std::println("  Alice: {}  Bob: {}", a1.balance, a2.balance);

    // Tight OrderService — cannot test without real MySql
    tight::TightOrderService tightSvc;
    tightSvc.placeOrder(1, "widget");

    std::println("");

    // ── LOOSE ────────────────────────────────────────────────────────────────
    std::println("--- LOOSE Coupling (after) ---");
    std::println("");

    // Scenario 1: production — real DB + console logger
    std::println("Scenario 1: Production");
    {
        loose::MySqlDatabase  db;
        loose::ConsoleLogger  logger;
        loose::OrderService   svc(db, logger);

        assert(svc.placeOrder(1, "widget")  == true);
        assert(svc.placeOrder(2, "gadget")  == true);
        assert(svc.placeOrder(3, "")        == false);  // empty item rejected
        assert(svc.totalOrders()            == 2);
        assert(svc.getOrder(1)              == "widget");
        assert(logger.infoCount()           == 2);
        assert(logger.errorCount()          == 1);
        std::println("  ✓ Production scenario passed");
        std::println("");
    }

    // Scenario 2: unit test — MockDB + NullLogger
    // OrderService code is IDENTICAL — only injected deps change
    std::println("Scenario 2: Unit Test (Mock)");
    {
        loose::MockDatabase  mockDb;
        loose::NullLogger    nullLog;
        loose::OrderService  svc(mockDb, nullLog);

        svc.placeOrder(10, "alpha");
        svc.placeOrder(11, "beta");

        assert(mockDb.saveCalls()   == 2);
        assert(mockDb.orderCount()  == 2);
        assert(mockDb.getOrder(10)  == "alpha");
        std::println("  ✓ Unit test scenario passed (no console spam)");
        std::println("");
    }
    return 0;
}
