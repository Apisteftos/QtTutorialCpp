// =============================================================================
// Microservices Architecture — C++23
//
// Example: Order processing system with 4 independent services.
//
// Simulated in-process — each Service class represents an independently
// deployable unit. MessageBroker simulates RabbitMQ/Kafka.
// Gateway routes synchronous client requests.
//
// Services:
//   UserService      — authentication
//   OrderService     — order creation (publishes events)
//   InventoryService — stock reservation (subscribes to events)
//   NotifyService    — confirmations (subscribes to events)
// =============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <functional>
#include <stdexcept>
#include <cassert>
#include <print>

// =============================================================================
// Message Broker — async communication between services
// In production: replace with RabbitMQ, Kafka, NATS
// =============================================================================

struct Message {
    std::string topic;
    std::string payload;
};

class MessageBroker {
public:
    void publish(const std::string& topic, const std::string& payload) {
        queue_.push({topic, payload});
    }

    void subscribe(const std::string& topic,
                   std::function<void(const Message&)> handler) {
        subscribers_[topic].push_back(std::move(handler));
    }

    // Dispatch all pending messages (simulates broker processing loop)
    void flush() {
        while (!queue_.empty()) {
            Message msg = queue_.front(); queue_.pop();
            auto it = subscribers_.find(msg.topic);
            if (it != subscribers_.end())
                for (auto& h : it->second) h(msg);
        }
    }

    int pending() const { return static_cast<int>(queue_.size()); }

private:
    std::queue<Message> queue_;
    std::unordered_map<std::string,
                       std::vector<std::function<void(const Message&)>>> subscribers_;
};

// =============================================================================
// UserService — owns authentication, user data
// =============================================================================

class UserService {
public:
    // Synchronous API (called via Gateway)
    bool authenticate(const std::string& user, const std::string& token) const {
        // Simplified: any token "valid-*" is accepted
        bool ok = token.substr(0, 6) == "valid-";
        std::println("  [UserService]   auth({}) → {}", user, (ok ? "✓" : "✗"));
        return ok;
    }
};

// =============================================================================
// InventoryService — owns stock, subscribes to ORDER_PLACED
// =============================================================================

class InventoryService {
public:
    explicit InventoryService(MessageBroker& broker) {
        // Self-register to broker — no coupling to OrderService
        broker.subscribe("ORDER_PLACED",
                         [this](const Message& m){ onOrderPlaced(m); });

        // Seed stock
        stock_["widget"]  = 50;
        stock_["gadget"]  = 5;
    }

    int stock(const std::string& item) const {
        auto it = stock_.find(item);
        return it != stock_.end() ? it->second : 0;
    }

    int reservedCount() const { return reserved_; }

private:
    void onOrderPlaced(const Message& m) {
        std::println("  [InventoryService] reserving stock for: {}", m.payload);
        reserved_++;
        // Parse item from payload (simplified)
        auto pos = m.payload.find("item=");
        if (pos != std::string::npos) {
            std::string item = m.payload.substr(pos + 5,
                                                m.payload.find(' ', pos + 5) - pos - 5);
            if (stock_.count(item)) stock_[item]--;
        }
    }

    std::unordered_map<std::string, int> stock_;
    int reserved_{0};
};

// =============================================================================
// NotifyService — sends confirmations, subscribes to ORDER_PLACED + ORDER_FAILED
// =============================================================================

class NotifyService {
public:
    explicit NotifyService(MessageBroker& broker) {
        broker.subscribe("ORDER_PLACED",
                         [this](const Message& m){ onOrderPlaced(m); });
        broker.subscribe("ORDER_FAILED",
                         [this](const Message& m){ onOrderFailed(m); });
    }

    int sentCount() const { return sent_; }

private:
    void onOrderPlaced(const Message& m) {
        sent_++;
        std::println("  [NotifyService] 📧 Confirmation sent — {}", m.payload);
    }
    void onOrderFailed(const Message& m) {
        sent_++;
        std::println("  [NotifyService] ⚠️  Failure notice sent — {}", m.payload);
    }

    int sent_{0};
};

// =============================================================================
// OrderService — creates orders, publishes events
// =============================================================================

class OrderService {
public:
    explicit OrderService(MessageBroker& broker) : broker_(broker) {}

    // Synchronous: called by Gateway, returns order ID or throws
    int createOrder(const std::string& user,
                    const std::string& item, int qty) {
        if (qty <= 0)
            throw std::invalid_argument("Quantity must be positive");

        int id = next_id_++;
        std::println("  [OrderService]  order #{} created: {}x {} for {}", id, qty, item, user);

        // Publish async event — no direct call to Inventory or Notify
        broker_.publish("ORDER_PLACED",
                        "id=" + std::to_string(id)
                            + " item=" + item
                            + " qty=" + std::to_string(qty)
                            + " user=" + user);
        return id;
    }

    void cancelOrder(int id) {
        std::println("  [OrderService]  order #{} cancelled", id);
        broker_.publish("ORDER_FAILED",
                        "id=" + std::to_string(id) + " reason=cancelled");
    }

    int orderCount() const { return next_id_ - 1; }

private:
    MessageBroker& broker_;
    int next_id_{1};
};

// =============================================================================
// API Gateway — single entry point, routes to services, handles auth
// =============================================================================

class ApiGateway {
public:
    ApiGateway(UserService& users,
               OrderService& orders,
               MessageBroker& broker)
        : users_(users), orders_(orders), broker_(broker) {}

    void handleCreateOrder(const std::string& user, const std::string& token,
                           const std::string& item, int qty) {
        std::println("");
        std::println("→ Gateway: POST /order");
        if (!users_.authenticate(user, token)) {
            std::println("  [Gateway] 401 Unauthorized");
            return;
        }
        try {
            int id = orders_.createOrder(user, item, qty);
            broker_.flush();  // dispatch async messages
            std::println("  [Gateway] 201 Created — order #{}", id);
        } catch (const std::exception& e) {
            std::println("  [Gateway] 400 Bad Request: {}", e.what());
        }
    }

    void handleCancelOrder(const std::string& user, const std::string& token,
                           int id) {
        std::println("");
        std::println("→ Gateway: DELETE /order/{}", id);
        if (!users_.authenticate(user, token)) {
            std::println("  [Gateway] 401 Unauthorized");
            return;
        }
        orders_.cancelOrder(id);
        broker_.flush();
        std::println("  [Gateway] 200 OK");
    }

private:
    UserService&   users_;
    OrderService&  orders_;
    MessageBroker& broker_;
};

// =============================================================================
// main
// =============================================================================

int main()
{
    std::println("=== Microservices Architecture Demo ===");

    // Infrastructure
    MessageBroker broker;

    // Services (self-register to broker in constructors)
    UserService      users;
    InventoryService inventory(broker);
    NotifyService    notify(broker);
    OrderService     orders(broker);

    // Gateway
    ApiGateway gateway(users, orders, broker);

    // --- Client requests ---

    // Successful order
    gateway.handleCreateOrder("kostas", "valid-abc123", "widget", 2);

    // Auth failure
    gateway.handleCreateOrder("hacker", "bad-token", "widget", 10);

    // Another successful order
    gateway.handleCreateOrder("kostas", "valid-abc123", "gadget", 1);

    // Cancel
    gateway.handleCancelOrder("kostas", "valid-abc123", 1);

    // Bad request
    gateway.handleCreateOrder("kostas", "valid-abc123", "widget", -1);

    // Verify
    std::println("");
    std::println("=== Summary ===");
    std::println("Orders created     : {}", orders.orderCount());
    std::println("Notifications sent : {}", notify.sentCount());
    std::println("Reservations made  : {}", inventory.reservedCount());
    std::println("widget stock left  : {}", inventory.stock("widget"));

    assert(orders.orderCount()       == 2);  // 2 valid orders (bad-token + bad-qty rejected)
    assert(notify.sentCount()        == 3);  // 2 placed + 1 cancel
    assert(inventory.reservedCount() == 2);  // 2 reservations for the 2 valid orders
    std::println("All assertions passed.");

    return 0;
}
