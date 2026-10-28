#include <string>
#include <memory>
#include <print>

// ============================================
// EXAMPLE 1: VENDING MACHINE
// Classic state pattern example
// ============================================

// Forward declaration
class VendingMachine;

/**
 * State Interface
 * Defines interface for encapsulating behavior associated with a state
 */
class VendingMachineState {
public:
    virtual ~VendingMachineState() {}

    virtual void insertCoin(VendingMachine* machine) = 0;
    virtual void ejectCoin(VendingMachine* machine) = 0;
    virtual void selectProduct(VendingMachine* machine) = 0;
    virtual void dispense(VendingMachine* machine) = 0;
    virtual std::string getStateName() const = 0;
};

/**
 * Context: Vending Machine
 * Maintains current state and delegates state-specific behavior
 */
class VendingMachine {
private:
    std::shared_ptr<VendingMachineState> currentState;
    int productCount;
    bool hasCoin;

public:
    VendingMachine(int count);

    void setState(std::shared_ptr<VendingMachineState> state) {
        currentState = state;
        std::println("🔄 State changed to: {}", state->getStateName());
    }

    std::shared_ptr<VendingMachineState> getState() const {
        return currentState;
    }

    // Delegate to current state
    void insertCoin() {
        currentState->insertCoin(this);
    }

    void ejectCoin() {
        currentState->ejectCoin(this);
    }

    void selectProduct() {
        currentState->selectProduct(this);
    }

    void dispense() {
        currentState->dispense(this);
    }

    // State management methods
    void addCoin() { hasCoin = true; }
    void removeCoin() { hasCoin = false; }
    bool hasCoins() const { return hasCoin; }

    int getProductCount() const { return productCount; }
    void releaseProduct() {
        if (productCount > 0) {
            productCount--;
            std::println("📦 Product dispensed! Remaining: {}", productCount);
        }
    }

    void refill(int count) {
        productCount += count;
        std::println("🔄 Refilled with {} products. Total: {}", count, productCount);
    }
};

// Forward declarations of concrete states
class NoCoinState;
class HasCoinState;
class DispensingState;
class SoldOutState;

/**
 * Concrete State: No Coin Inserted
 */
class NoCoinState : public VendingMachineState {
public:
    void insertCoin(VendingMachine* machine) override;

    void ejectCoin(VendingMachine* machine) override {
        std::println("❌ No coin to eject");
    }

    void selectProduct(VendingMachine* machine) override {
        std::println("❌ Insert coin first");
    }

    void dispense(VendingMachine* machine) override {
        std::println("❌ Payment required");
    }

    std::string getStateName() const override {
        return "NoCoinState";
    }
};

/**
 * Concrete State: Coin Inserted
 */
class HasCoinState : public VendingMachineState {
public:
    void insertCoin(VendingMachine* machine) override {
        std::println("❌ Coin already inserted");
    }

    void ejectCoin(VendingMachine* machine) override {
        std::println("💰 Coin ejected");
        machine->removeCoin();
        machine->setState(std::make_shared<NoCoinState>());
    }

    void selectProduct(VendingMachine* machine) override;

    void dispense(VendingMachine* machine) override {
        std::println("❌ Select product first");
    }

    std::string getStateName() const override {
        return "HasCoinState";
    }
};

/**
 * Concrete State: Dispensing Product
 */
class DispensingState : public VendingMachineState {
public:
    void insertCoin(VendingMachine* machine) override {
        std::println("❌ Please wait, dispensing product");
    }

    void ejectCoin(VendingMachine* machine) override {
        std::println("❌ Too late, product is being dispensed");
    }

    void selectProduct(VendingMachine* machine) override {
        std::println("❌ Product already selected");
    }

    void dispense(VendingMachine* machine) override;

    std::string getStateName() const override {
        return "DispensingState";
    }
};

/**
 * Concrete State: Sold Out
 */
class SoldOutState : public VendingMachineState {
public:
    void insertCoin(VendingMachine* machine) override {
        std::println("❌ Sold out, coin rejected");
    }

    void ejectCoin(VendingMachine* machine) override {
        std::println("❌ No coin inserted");
    }

    void selectProduct(VendingMachine* machine) override {
        std::println("❌ Sold out");
    }

    void dispense(VendingMachine* machine) override {
        std::println("❌ Sold out");
    }

    std::string getStateName() const override {
        return "SoldOutState";
    }
};

// Implementation of methods that need all states defined
VendingMachine::VendingMachine(int count) : productCount(count), hasCoin(false) {
    if (count > 0) {
        currentState = std::make_shared<NoCoinState>();
    } else {
        currentState = std::make_shared<SoldOutState>();
    }
    std::println("🏪 Vending Machine initialized with {} products", count);
    std::println("🔄 Initial state: {}", currentState->getStateName());
}

void NoCoinState::insertCoin(VendingMachine* machine) {
    std::println("💰 Coin inserted");
    machine->addCoin();
    machine->setState(std::make_shared<HasCoinState>());
}

void HasCoinState::selectProduct(VendingMachine* machine) {
    std::println("🎯 Product selected");
    machine->setState(std::make_shared<DispensingState>());
    machine->dispense();  // Automatically dispense
}

void DispensingState::dispense(VendingMachine* machine) {
    machine->releaseProduct();
    machine->removeCoin();

    if (machine->getProductCount() > 0) {
        machine->setState(std::make_shared<NoCoinState>());
    } else {
        std::println("⚠️  Out of products!");
        machine->setState(std::make_shared<SoldOutState>());
    }
}

// ============================================
// EXAMPLE 2: TCP CONNECTION
// Network connection state management
// ============================================

// Forward declaration
class TCPConnection;

/**
 * TCP State Interface
 */
class TCPState {
public:
    virtual ~TCPState() {}

    virtual void open(TCPConnection* connection) = 0;
    virtual void close(TCPConnection* connection) = 0;
    virtual void send(TCPConnection* connection, const std::string& data) = 0;
    virtual void receive(TCPConnection* connection) = 0;
    virtual std::string getStateName() const = 0;
};

/**
 * Context: TCP Connection
 */
class TCPConnection {
private:
    std::shared_ptr<TCPState> currentState;
    std::string connectionId;

public:
    TCPConnection(const std::string& id);

    void setState(std::shared_ptr<TCPState> state) {
        currentState = state;
        std::println("🔄 [{}] State: {}", connectionId, state->getStateName());
    }

    std::string getId() const { return connectionId; }

    // Delegate to current state
    void open() { currentState->open(this); }
    void close() { currentState->close(this); }
    void send(const std::string& data) { currentState->send(this, data); }
    void receive() { currentState->receive(this); }
};

// Forward declarations
class TCPClosed;
class TCPListen;
class TCPEstablished;

/**
 * Concrete State: Closed
 */
class TCPClosed : public TCPState {
public:
    void open(TCPConnection* connection) override;

    void close(TCPConnection* connection) override {
        std::println("❌ [{}] Already closed", connection->getId());
    }

    void send(TCPConnection* connection, const std::string& data) override {
        std::println("❌ [{}] Cannot send, connection closed", connection->getId());
    }

    void receive(TCPConnection* connection) override {
        std::println("❌ [{}] Cannot receive, connection closed", connection->getId());
    }

    std::string getStateName() const override { return "CLOSED"; }
};

/**
 * Concrete State: Listen
 */
class TCPListen : public TCPState {
public:
    void open(TCPConnection* connection) override {
        std::println("❌ [{}] Already listening", connection->getId());
    }

    void close(TCPConnection* connection) override;

    void send(TCPConnection* connection, const std::string& data) override;

    void receive(TCPConnection* connection) override {
        std::println("📨 [{}] Accepting connection...", connection->getId());
    }

    std::string getStateName() const override { return "LISTEN"; }
};

/**
 * Concrete State: Established
 */
class TCPEstablished : public TCPState {
public:
    void open(TCPConnection* connection) override {
        std::println("❌ [{}] Already established", connection->getId());
    }

    void close(TCPConnection* connection) override;

    void send(TCPConnection* connection, const std::string& data) override {
        std::println("📤 [{}] Sending: \"{}\"", connection->getId(), data);
    }

    void receive(TCPConnection* connection) override {
        std::println("📥 [{}] Receiving data...", connection->getId());
    }

    std::string getStateName() const override { return "ESTABLISHED"; }
};

// Implementations
TCPConnection::TCPConnection(const std::string& id) : connectionId(id) {
    currentState = std::make_shared<TCPClosed>();
    std::println("🌐 TCP Connection [{}] created", id);
}

void TCPClosed::open(TCPConnection* connection) {
    std::println("🔌 [{}] Opening connection...", connection->getId());
    connection->setState(std::make_shared<TCPListen>());
}

void TCPListen::close(TCPConnection* connection) {
    std::println("🔌 [{}] Closing connection...", connection->getId());
    connection->setState(std::make_shared<TCPClosed>());
}

void TCPListen::send(TCPConnection* connection, const std::string& data) {
    std::println("✅ [{}] Connection established", connection->getId());
    connection->setState(std::make_shared<TCPEstablished>());
    connection->send(data);  // Resend in new state
}

void TCPEstablished::close(TCPConnection* connection) {
    std::println("🔌 [{}] Closing established connection...", connection->getId());
    connection->setState(std::make_shared<TCPClosed>());
}

// ============================================
// EXAMPLE 3: DOCUMENT WORKFLOW
// Document approval process
// ============================================

// Forward declaration
class Document;

/**
 * Document State Interface
 */
class DocumentState {
public:
    virtual ~DocumentState() {}

    virtual void edit(Document* doc) = 0;
    virtual void submit(Document* doc) = 0;
    virtual void approve(Document* doc) = 0;
    virtual void reject(Document* doc) = 0;
    virtual void publish(Document* doc) = 0;
    virtual std::string getStateName() const = 0;
};

/**
 * Context: Document
 */
class Document {
private:
    std::shared_ptr<DocumentState> currentState;
    std::string title;
    std::string content;

public:
    Document(const std::string& t);

    void setState(std::shared_ptr<DocumentState> state) {
        currentState = state;
        std::println("📝 [{}] Status: {}", title, state->getStateName());
    }

    std::string getTitle() const { return title; }

    void setContent(const std::string& c) { content = c; }
    std::string getContent() const { return content; }

    // Delegate to current state
    void edit() { currentState->edit(this); }
    void submit() { currentState->submit(this); }
    void approve() { currentState->approve(this); }
    void reject() { currentState->reject(this); }
    void publish() { currentState->publish(this); }
};

// Forward declarations
class DraftState;
class ModerationState;
class ApprovedState;
class PublishedState;

/**
 * Concrete State: Draft
 */
class DraftState : public DocumentState {
public:
    void edit(Document* doc) override {
        std::println("✏️  [{}] Editing draft...", doc->getTitle());
    }

    void submit(Document* doc) override;

    void approve(Document* doc) override {
        std::println("❌ [{}] Must submit for moderation first", doc->getTitle());
    }

    void reject(Document* doc) override {
        std::println("❌ [{}] Cannot reject draft", doc->getTitle());
    }

    void publish(Document* doc) override {
        std::println("❌ [{}] Cannot publish without approval", doc->getTitle());
    }

    std::string getStateName() const override { return "DRAFT"; }
};

/**
 * Concrete State: Moderation
 */
class ModerationState : public DocumentState {
public:
    void edit(Document* doc) override {
        std::println("❌ [{}] Cannot edit during moderation", doc->getTitle());
    }

    void submit(Document* doc) override {
        std::println("❌ [{}] Already in moderation", doc->getTitle());
    }

    void approve(Document* doc) override;

    void reject(Document* doc) override;

    void publish(Document* doc) override {
        std::println("❌ [{}] Must be approved first", doc->getTitle());
    }

    std::string getStateName() const override { return "MODERATION"; }
};

/**
 * Concrete State: Approved
 */
class ApprovedState : public DocumentState {
public:
    void edit(Document* doc) override;

    void submit(Document* doc) override {
        std::println("❌ [{}] Already approved", doc->getTitle());
    }

    void approve(Document* doc) override {
        std::println("❌ [{}] Already approved", doc->getTitle());
    }

    void reject(Document* doc) override {
        std::println("❌ [{}] Cannot reject approved document", doc->getTitle());
    }

    void publish(Document* doc) override;

    std::string getStateName() const override { return "APPROVED"; }
};

/**
 * Concrete State: Published
 */
class PublishedState : public DocumentState {
public:
    void edit(Document* doc) override {
        std::println("❌ [{}] Cannot edit published document", doc->getTitle());
    }

    void submit(Document* doc) override {
        std::println("❌ [{}] Already published", doc->getTitle());
    }

    void approve(Document* doc) override {
        std::println("❌ [{}] Already published", doc->getTitle());
    }

    void reject(Document* doc) override {
        std::println("❌ [{}] Cannot reject published document", doc->getTitle());
    }

    void publish(Document* doc) override {
        std::println("❌ [{}] Already published", doc->getTitle());
    }

    std::string getStateName() const override { return "PUBLISHED"; }
};

// Implementations
Document::Document(const std::string& t) : title(t) {
    currentState = std::make_shared<DraftState>();
    std::println("📄 Document \"{}\" created", title);
}

void DraftState::submit(Document* doc) {
    std::println("📤 [{}] Submitted for moderation", doc->getTitle());
    doc->setState(std::make_shared<ModerationState>());
}

void ModerationState::approve(Document* doc) {
    std::println("✅ [{}] Approved by moderator", doc->getTitle());
    doc->setState(std::make_shared<ApprovedState>());
}

void ModerationState::reject(Document* doc) {
    std::println("❌ [{}] Rejected, returned to draft", doc->getTitle());
    doc->setState(std::make_shared<DraftState>());
}

void ApprovedState::edit(Document* doc) {
    std::println("✏️  [{}] Editing approved document (back to draft)", doc->getTitle());
    doc->setState(std::make_shared<DraftState>());
}

void ApprovedState::publish(Document* doc) {
    std::println("🚀 [{}] Published!", doc->getTitle());
    doc->setState(std::make_shared<PublishedState>());
}

// ============================================
// EXAMPLE 4: TRAFFIC LIGHT
// Simple state machine
// ============================================

// Forward declaration
class TrafficLight;

/**
 * Traffic Light State Interface
 */
class LightState {
public:
    virtual ~LightState() {}

    virtual void next(TrafficLight* light) = 0;
    virtual std::string getColor() const = 0;
    virtual int getDuration() const = 0;
};

/**
 * Context: Traffic Light
 */
class TrafficLight {
private:
    std::shared_ptr<LightState> currentState;
    std::string location;

public:
    TrafficLight(const std::string& loc);

    void setState(std::shared_ptr<LightState> state) {
        currentState = state;
        std::println("🚦 [{}] {} light ({}s)", location, state->getColor(), state->getDuration());
    }

    void change() {
        currentState->next(this);
    }

    std::string getLocation() const { return location; }
};

// Forward declarations
class RedLight;
class YellowLight;
class GreenLight;

/**
 * Concrete State: Red Light
 */
class RedLight : public LightState {
public:
    void next(TrafficLight* light) override;

    std::string getColor() const override { return "🔴 RED"; }
    int getDuration() const override { return 30; }
};

/**
 * Concrete State: Yellow Light
 */
class YellowLight : public LightState {
public:
    void next(TrafficLight* light) override;

    std::string getColor() const override { return "🟡 YELLOW"; }
    int getDuration() const override { return 5; }
};

/**
 * Concrete State: Green Light
 */
class GreenLight : public LightState {
public:
    void next(TrafficLight* light) override;

    std::string getColor() const override { return "🟢 GREEN"; }
    int getDuration() const override { return 45; }
};

// Implementations
TrafficLight::TrafficLight(const std::string& loc) : location(loc) {
    currentState = std::make_shared<RedLight>();
    std::println("🚦 Traffic Light at {} initialized", location);
}

void RedLight::next(TrafficLight* light) {
    std::println("⏭️  [{}] Changing from RED to GREEN", light->getLocation());
    light->setState(std::make_shared<GreenLight>());
}

void GreenLight::next(TrafficLight* light) {
    std::println("⏭️  [{}] Changing from GREEN to YELLOW", light->getLocation());
    light->setState(std::make_shared<YellowLight>());
}

void YellowLight::next(TrafficLight* light) {
    std::println("⏭️  [{}] Changing from YELLOW to RED", light->getLocation());
    light->setState(std::make_shared<RedLight>());
}

// ============================================
// DEMONSTRATION FUNCTIONS
// ============================================

void demonstrateVendingMachine() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: VENDING MACHINE        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    VendingMachine machine(3);

    std::println("");
    std::println("--- Successful Purchase ---");
    machine.insertCoin();
    machine.selectProduct();

    std::println("");
    std::println("--- Try Without Coin ---");
    machine.selectProduct();

    std::println("");
    std::println("--- Insert and Eject Coin ---");
    machine.insertCoin();
    machine.ejectCoin();

    std::println("");
    std::println("--- Two More Purchases ---");
    machine.insertCoin();
    machine.selectProduct();

    machine.insertCoin();
    machine.selectProduct();

    std::println("");
    std::println("--- Try When Sold Out ---");
    machine.insertCoin();

    std::println("");
    std::println("--- Refill Machine ---");
    machine.refill(5);
    machine.insertCoin();
    machine.selectProduct();
}

void demonstrateTCPConnection() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: TCP CONNECTION         ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    TCPConnection conn("192.168.1.100:8080");

    std::println("");
    std::println("--- Try Operations on Closed Connection ---");
    conn.send("Hello");
    conn.receive();

    std::println("");
    std::println("--- Open Connection ---");
    conn.open();

    std::println("");
    std::println("--- Establish and Send Data ---");
    conn.send("GET /index.html HTTP/1.1");
    conn.receive();

    std::println("");
    std::println("--- Close Connection ---");
    conn.close();
}

void demonstrateDocument() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: DOCUMENT WORKFLOW      ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Document doc("Annual Report 2024");

    std::println("");
    std::println("--- Edit Draft ---");
    doc.edit();
    doc.edit();

    std::println("");
    std::println("--- Submit for Moderation ---");
    doc.submit();

    std::println("");
    std::println("--- Try to Edit During Moderation ---");
    doc.edit();

    std::println("");
    std::println("--- Reject Document ---");
    doc.reject();

    std::println("");
    std::println("--- Resubmit ---");
    doc.submit();

    std::println("");
    std::println("--- Approve ---");
    doc.approve();

    std::println("");
    std::println("--- Publish ---");
    doc.publish();

    std::println("");
    std::println("--- Try to Edit Published ---");
    doc.edit();
}

void demonstrateTrafficLight() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: TRAFFIC LIGHT          ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    TrafficLight light("Main St & 5th Ave");

    std::println("");
    std::println("--- Light Cycle ---");
    light.change();  // Red -> Green
    light.change();  // Green -> Yellow
    light.change();  // Yellow -> Red
    light.change();  // Red -> Green
}

// ============================================
// MAIN - DEMONSTRATING THE PATTERN
// ============================================

int main() {
    std::println("========================================");
    std::println("STATE PATTERN DEMO");
    std::println("========================================");

    demonstrateVendingMachine();
    demonstrateTCPConnection();
    demonstrateDocument();
    demonstrateTrafficLight();

    return 0;
}
