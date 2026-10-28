// =============================================================================
// Layered Architecture — C++23
//
// Example: Warehouse inventory system with 3 strict layers.
//
// Layer dependency rule:
//   Presentation → Business → Data
//   No skipping. No upward calls.
//   Lower layers do NOT #include higher layer headers.
//
// Structure:
//   DataLayer       — stores inventory (simulates a database)
//   BusinessLayer   — applies stock rules (min qty, reorder logic)
//   PresentationLayer — handles user interaction (console I/O)
// =============================================================================

#include <string>
#include <unordered_map>
#include <stdexcept>
#include <iomanip>
#include <print>

// =============================================================================
// LAYER 1 — Data Layer
// Knows only about storage. No business rules. No UI.
// =============================================================================

class DataLayer {
public:
    DataLayer() {
        // Simulate pre-loaded database
        db_["widget"]  = 100;
        db_["gadget"]  = 25;
        db_["doohick"] = 0;
    }

    int load(const std::string& item) const {
        auto it = db_.find(item);
        if (it == db_.end())
            throw std::runtime_error("Item not found: " + item);
        return it->second;
    }

    void save(const std::string& item, int qty) {
        db_[item] = qty;
    }

    std::unordered_map<std::string, int> loadAll() const {
        return db_;
    }

private:
    std::unordered_map<std::string, int> db_;
};

// =============================================================================
// LAYER 2 — Business Logic Layer
// Knows business rules. Calls DataLayer. Knows nothing about console I/O.
// =============================================================================

class BusinessLayer {
public:
    static constexpr int REORDER_THRESHOLD = 10;
    static constexpr int REORDER_QTY       = 50;

    explicit BusinessLayer(DataLayer& data) : data_(data) {}

    // Business rule: cannot order more than current stock
    bool processOrder(const std::string& item, int qty) {
        int stock = data_.load(item);
        if (qty <= 0)
            throw std::invalid_argument("Order quantity must be positive");
        if (qty > stock)
            return false;  // insufficient stock — order rejected
        data_.save(item, stock - qty);
        return true;
    }

    // Business rule: auto-reorder if stock falls below threshold
    void checkAndReorder(const std::string& item) {
        int stock = data_.load(item);
        if (stock < REORDER_THRESHOLD) {
            data_.save(item, stock + REORDER_QTY);
        }
    }

    int getStock(const std::string& item) const {
        return data_.load(item);
    }

    std::unordered_map<std::string, int> getAllStock() const {
        return data_.loadAll();
    }

private:
    DataLayer& data_;   // dependency points DOWNWARD only
};

// =============================================================================
// LAYER 3 — Presentation Layer
// Knows only about display and user input. Calls BusinessLayer.
// No direct access to DataLayer — that would skip a layer.
// =============================================================================

class PresentationLayer {
public:
    explicit PresentationLayer(BusinessLayer& business)
        : business_(business) {}

    void run() {
        std::println("=== Warehouse Inventory System ===");
        std::println("");
        showAllStock();
        placeOrder("widget",  10);
        placeOrder("gadget",  30);  // should fail — insufficient stock
        placeOrder("doohick",  1);  // should trigger reorder
        std::println("");
        std::println("--- Stock after orders ---");
        showAllStock();
    }

private:
    void showAllStock() const {
        std::println("{:<12}{:<8}", "Item", "Stock");
        std::println("{}", std::string(20, '-'));
        for (const auto& [item, qty] : business_.getAllStock())
            std::println("{:<12}{:<8}", item, qty);
        std::println("");
    }

    void placeOrder(const std::string& item, int qty) {
        std::print("Order: {}x {} → ", qty, item);
        try {
            bool ok = business_.processOrder(item, qty);
            if (ok) {
                std::println("✓ Accepted  (stock now: {})", business_.getStock(item));
                business_.checkAndReorder(item);
            } else {
                std::println("✗ Rejected  (insufficient stock: {})", business_.getStock(item));
            }
        } catch (const std::exception& e) {
            std::println("✗ Error: {}", e.what());
        }
    }

    BusinessLayer& business_;   // dependency points DOWNWARD only
        // NOTE: NO DataLayer& here — that skips a layer
};

// =============================================================================
// main — wire the layers together (dependency injection)
// =============================================================================

int main()
{
    // Composition root: create layers bottom-up, inject downward
    DataLayer         data;
    BusinessLayer     business(data);
    PresentationLayer presentation(business);

    presentation.run();
    return 0;
}
