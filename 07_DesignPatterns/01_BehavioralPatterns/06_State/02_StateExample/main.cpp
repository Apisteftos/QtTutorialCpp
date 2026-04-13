#include <iostream>
#include <string>
#include <memory>
using namespace std;

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
    virtual string getStateName() const = 0;
};

/**
 * Context: Vending Machine
 * Maintains current state and delegates state-specific behavior
 */
class VendingMachine {
private:
    shared_ptr<VendingMachineState> currentState;
    int productCount;
    bool hasCoin;

public:
    VendingMachine(int count);

    void setState(shared_ptr<VendingMachineState> state) {
        currentState = state;
        cout << "🔄 State changed to: " << state->getStateName() << "\n";
    }

    shared_ptr<VendingMachineState> getState() const {
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
            cout << "📦 Product dispensed! Remaining: " << productCount << "\n";
        }
    }

    void refill(int count) {
        productCount += count;
        cout << "🔄 Refilled with " << count << " products. Total: " << productCount << "\n";
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
        cout << "❌ No coin to eject\n";
    }

    void selectProduct(VendingMachine* machine) override {
        cout << "❌ Insert coin first\n";
    }

    void dispense(VendingMachine* machine) override {
        cout << "❌ Payment required\n";
    }

    string getStateName() const override {
        return "NoCoinState";
    }
};

/**
 * Concrete State: Coin Inserted
 */
class HasCoinState : public VendingMachineState {
public:
    void insertCoin(VendingMachine* machine) override {
        cout << "❌ Coin already inserted\n";
    }

    void ejectCoin(VendingMachine* machine) override {
        cout << "💰 Coin ejected\n";
        machine->removeCoin();
        machine->setState(make_shared<NoCoinState>());
    }

    void selectProduct(VendingMachine* machine) override;

    void dispense(VendingMachine* machine) override {
        cout << "❌ Select product first\n";
    }

    string getStateName() const override {
        return "HasCoinState";
    }
};

/**
 * Concrete State: Dispensing Product
 */
class DispensingState : public VendingMachineState {
public:
    void insertCoin(VendingMachine* machine) override {
        cout << "❌ Please wait, dispensing product\n";
    }

    void ejectCoin(VendingMachine* machine) override {
        cout << "❌ Too late, product is being dispensed\n";
    }

    void selectProduct(VendingMachine* machine) override {
        cout << "❌ Product already selected\n";
    }

    void dispense(VendingMachine* machine) override;

    string getStateName() const override {
        return "DispensingState";
    }
};

/**
 * Concrete State: Sold Out
 */
class SoldOutState : public VendingMachineState {
public:
    void insertCoin(VendingMachine* machine) override {
        cout << "❌ Sold out, coin rejected\n";
    }

    void ejectCoin(VendingMachine* machine) override {
        cout << "❌ No coin inserted\n";
    }

    void selectProduct(VendingMachine* machine) override {
        cout << "❌ Sold out\n";
    }

    void dispense(VendingMachine* machine) override {
        cout << "❌ Sold out\n";
    }

    string getStateName() const override {
        return "SoldOutState";
    }
};

// Implementation of methods that need all states defined
VendingMachine::VendingMachine(int count) : productCount(count), hasCoin(false) {
    if (count > 0) {
        currentState = make_shared<NoCoinState>();
    } else {
        currentState = make_shared<SoldOutState>();
    }
    cout << "🏪 Vending Machine initialized with " << count << " products\n";
    cout << "🔄 Initial state: " << currentState->getStateName() << "\n";
}

void NoCoinState::insertCoin(VendingMachine* machine) {
    cout << "💰 Coin inserted\n";
    machine->addCoin();
    machine->setState(make_shared<HasCoinState>());
}

void HasCoinState::selectProduct(VendingMachine* machine) {
    cout << "🎯 Product selected\n";
    machine->setState(make_shared<DispensingState>());
    machine->dispense();  // Automatically dispense
}

void DispensingState::dispense(VendingMachine* machine) {
    machine->releaseProduct();
    machine->removeCoin();

    if (machine->getProductCount() > 0) {
        machine->setState(make_shared<NoCoinState>());
    } else {
        cout << "⚠️  Out of products!\n";
        machine->setState(make_shared<SoldOutState>());
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
    virtual void send(TCPConnection* connection, const string& data) = 0;
    virtual void receive(TCPConnection* connection) = 0;
    virtual string getStateName() const = 0;
};

/**
 * Context: TCP Connection
 */
class TCPConnection {
private:
    shared_ptr<TCPState> currentState;
    string connectionId;

public:
    TCPConnection(const string& id);

    void setState(shared_ptr<TCPState> state) {
        currentState = state;
        cout << "🔄 [" << connectionId << "] State: " << state->getStateName() << "\n";
    }

    string getId() const { return connectionId; }

    // Delegate to current state
    void open() { currentState->open(this); }
    void close() { currentState->close(this); }
    void send(const string& data) { currentState->send(this, data); }
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
        cout << "❌ [" << connection->getId() << "] Already closed\n";
    }

    void send(TCPConnection* connection, const string& data) override {
        cout << "❌ [" << connection->getId() << "] Cannot send, connection closed\n";
    }

    void receive(TCPConnection* connection) override {
        cout << "❌ [" << connection->getId() << "] Cannot receive, connection closed\n";
    }

    string getStateName() const override { return "CLOSED"; }
};

/**
 * Concrete State: Listen
 */
class TCPListen : public TCPState {
public:
    void open(TCPConnection* connection) override {
        cout << "❌ [" << connection->getId() << "] Already listening\n";
    }

    void close(TCPConnection* connection) override;

    void send(TCPConnection* connection, const string& data) override;

    void receive(TCPConnection* connection) override {
        cout << "📨 [" << connection->getId() << "] Accepting connection...\n";
    }

    string getStateName() const override { return "LISTEN"; }
};

/**
 * Concrete State: Established
 */
class TCPEstablished : public TCPState {
public:
    void open(TCPConnection* connection) override {
        cout << "❌ [" << connection->getId() << "] Already established\n";
    }

    void close(TCPConnection* connection) override;

    void send(TCPConnection* connection, const string& data) override {
        cout << "📤 [" << connection->getId() << "] Sending: \"" << data << "\"\n";
    }

    void receive(TCPConnection* connection) override {
        cout << "📥 [" << connection->getId() << "] Receiving data...\n";
    }

    string getStateName() const override { return "ESTABLISHED"; }
};

// Implementations
TCPConnection::TCPConnection(const string& id) : connectionId(id) {
    currentState = make_shared<TCPClosed>();
    cout << "🌐 TCP Connection [" << id << "] created\n";
}

void TCPClosed::open(TCPConnection* connection) {
    cout << "🔌 [" << connection->getId() << "] Opening connection...\n";
    connection->setState(make_shared<TCPListen>());
}

void TCPListen::close(TCPConnection* connection) {
    cout << "🔌 [" << connection->getId() << "] Closing connection...\n";
    connection->setState(make_shared<TCPClosed>());
}

void TCPListen::send(TCPConnection* connection, const string& data) {
    cout << "✅ [" << connection->getId() << "] Connection established\n";
    connection->setState(make_shared<TCPEstablished>());
    connection->send(data);  // Resend in new state
}

void TCPEstablished::close(TCPConnection* connection) {
    cout << "🔌 [" << connection->getId() << "] Closing established connection...\n";
    connection->setState(make_shared<TCPClosed>());
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
    virtual string getStateName() const = 0;
};

/**
 * Context: Document
 */
class Document {
private:
    shared_ptr<DocumentState> currentState;
    string title;
    string content;

public:
    Document(const string& t);

    void setState(shared_ptr<DocumentState> state) {
        currentState = state;
        cout << "📝 [" << title << "] Status: " << state->getStateName() << "\n";
    }

    string getTitle() const { return title; }

    void setContent(const string& c) { content = c; }
    string getContent() const { return content; }

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
        cout << "✏️  [" << doc->getTitle() << "] Editing draft...\n";
    }

    void submit(Document* doc) override;

    void approve(Document* doc) override {
        cout << "❌ [" << doc->getTitle() << "] Must submit for moderation first\n";
    }

    void reject(Document* doc) override {
        cout << "❌ [" << doc->getTitle() << "] Cannot reject draft\n";
    }

    void publish(Document* doc) override {
        cout << "❌ [" << doc->getTitle() << "] Cannot publish without approval\n";
    }

    string getStateName() const override { return "DRAFT"; }
};

/**
 * Concrete State: Moderation
 */
class ModerationState : public DocumentState {
public:
    void edit(Document* doc) override {
        cout << "❌ [" << doc->getTitle() << "] Cannot edit during moderation\n";
    }

    void submit(Document* doc) override {
        cout << "❌ [" << doc->getTitle() << "] Already in moderation\n";
    }

    void approve(Document* doc) override;

    void reject(Document* doc) override;

    void publish(Document* doc) override {
        cout << "❌ [" << doc->getTitle() << "] Must be approved first\n";
    }

    string getStateName() const override { return "MODERATION"; }
};

/**
 * Concrete State: Approved
 */
class ApprovedState : public DocumentState {
public:
    void edit(Document* doc) override;

    void submit(Document* doc) override {
        cout << "❌ [" << doc->getTitle() << "] Already approved\n";
    }

    void approve(Document* doc) override {
        cout << "❌ [" << doc->getTitle() << "] Already approved\n";
    }

    void reject(Document* doc) override {
        cout << "❌ [" << doc->getTitle() << "] Cannot reject approved document\n";
    }

    void publish(Document* doc) override;

    string getStateName() const override { return "APPROVED"; }
};

/**
 * Concrete State: Published
 */
class PublishedState : public DocumentState {
public:
    void edit(Document* doc) override {
        cout << "❌ [" << doc->getTitle() << "] Cannot edit published document\n";
    }

    void submit(Document* doc) override {
        cout << "❌ [" << doc->getTitle() << "] Already published\n";
    }

    void approve(Document* doc) override {
        cout << "❌ [" << doc->getTitle() << "] Already published\n";
    }

    void reject(Document* doc) override {
        cout << "❌ [" << doc->getTitle() << "] Cannot reject published document\n";
    }

    void publish(Document* doc) override {
        cout << "❌ [" << doc->getTitle() << "] Already published\n";
    }

    string getStateName() const override { return "PUBLISHED"; }
};

// Implementations
Document::Document(const string& t) : title(t) {
    currentState = make_shared<DraftState>();
    cout << "📄 Document \"" << title << "\" created\n";
}

void DraftState::submit(Document* doc) {
    cout << "📤 [" << doc->getTitle() << "] Submitted for moderation\n";
    doc->setState(make_shared<ModerationState>());
}

void ModerationState::approve(Document* doc) {
    cout << "✅ [" << doc->getTitle() << "] Approved by moderator\n";
    doc->setState(make_shared<ApprovedState>());
}

void ModerationState::reject(Document* doc) {
    cout << "❌ [" << doc->getTitle() << "] Rejected, returned to draft\n";
    doc->setState(make_shared<DraftState>());
}

void ApprovedState::edit(Document* doc) {
    cout << "✏️  [" << doc->getTitle() << "] Editing approved document (back to draft)\n";
    doc->setState(make_shared<DraftState>());
}

void ApprovedState::publish(Document* doc) {
    cout << "🚀 [" << doc->getTitle() << "] Published!\n";
    doc->setState(make_shared<PublishedState>());
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
    virtual string getColor() const = 0;
    virtual int getDuration() const = 0;
};

/**
 * Context: Traffic Light
 */
class TrafficLight {
private:
    shared_ptr<LightState> currentState;
    string location;

public:
    TrafficLight(const string& loc);

    void setState(shared_ptr<LightState> state) {
        currentState = state;
        cout << "🚦 [" << location << "] " << state->getColor()
             << " light (" << state->getDuration() << "s)\n";
    }

    void change() {
        currentState->next(this);
    }

    string getLocation() const { return location; }
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

    string getColor() const override { return "🔴 RED"; }
    int getDuration() const override { return 30; }
};

/**
 * Concrete State: Yellow Light
 */
class YellowLight : public LightState {
public:
    void next(TrafficLight* light) override;

    string getColor() const override { return "🟡 YELLOW"; }
    int getDuration() const override { return 5; }
};

/**
 * Concrete State: Green Light
 */
class GreenLight : public LightState {
public:
    void next(TrafficLight* light) override;

    string getColor() const override { return "🟢 GREEN"; }
    int getDuration() const override { return 45; }
};

// Implementations
TrafficLight::TrafficLight(const string& loc) : location(loc) {
    currentState = make_shared<RedLight>();
    cout << "🚦 Traffic Light at " << location << " initialized\n";
}

void RedLight::next(TrafficLight* light) {
    cout << "⏭️  [" << light->getLocation() << "] Changing from RED to GREEN\n";
    light->setState(make_shared<GreenLight>());
}

void GreenLight::next(TrafficLight* light) {
    cout << "⏭️  [" << light->getLocation() << "] Changing from GREEN to YELLOW\n";
    light->setState(make_shared<YellowLight>());
}

void YellowLight::next(TrafficLight* light) {
    cout << "⏭️  [" << light->getLocation() << "] Changing from YELLOW to RED\n";
    light->setState(make_shared<RedLight>());
}

// ============================================
// DEMONSTRATION FUNCTIONS
// ============================================

void demonstrateVendingMachine() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: VENDING MACHINE        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    VendingMachine machine(3);

    cout << "\n--- Successful Purchase ---\n";
    machine.insertCoin();
    machine.selectProduct();

    cout << "\n--- Try Without Coin ---\n";
    machine.selectProduct();

    cout << "\n--- Insert and Eject Coin ---\n";
    machine.insertCoin();
    machine.ejectCoin();

    cout << "\n--- Two More Purchases ---\n";
    machine.insertCoin();
    machine.selectProduct();

    machine.insertCoin();
    machine.selectProduct();

    cout << "\n--- Try When Sold Out ---\n";
    machine.insertCoin();

    cout << "\n--- Refill Machine ---\n";
    machine.refill(5);
    machine.insertCoin();
    machine.selectProduct();
}

void demonstrateTCPConnection() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: TCP CONNECTION         ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    TCPConnection conn("192.168.1.100:8080");

    cout << "\n--- Try Operations on Closed Connection ---\n";
    conn.send("Hello");
    conn.receive();

    cout << "\n--- Open Connection ---\n";
    conn.open();

    cout << "\n--- Establish and Send Data ---\n";
    conn.send("GET /index.html HTTP/1.1");
    conn.receive();

    cout << "\n--- Close Connection ---\n";
    conn.close();
}

void demonstrateDocument() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: DOCUMENT WORKFLOW      ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Document doc("Annual Report 2024");

    cout << "\n--- Edit Draft ---\n";
    doc.edit();
    doc.edit();

    cout << "\n--- Submit for Moderation ---\n";
    doc.submit();

    cout << "\n--- Try to Edit During Moderation ---\n";
    doc.edit();

    cout << "\n--- Reject Document ---\n";
    doc.reject();

    cout << "\n--- Resubmit ---\n";
    doc.submit();

    cout << "\n--- Approve ---\n";
    doc.approve();

    cout << "\n--- Publish ---\n";
    doc.publish();

    cout << "\n--- Try to Edit Published ---\n";
    doc.edit();
}

void demonstrateTrafficLight() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: TRAFFIC LIGHT          ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    TrafficLight light("Main St & 5th Ave");

    cout << "\n--- Light Cycle ---\n";
    light.change();  // Red -> Green
    light.change();  // Green -> Yellow
    light.change();  // Yellow -> Red
    light.change();  // Red -> Green
}

// ============================================
// MAIN - DEMONSTRATING THE PATTERN
// ============================================

int main() {
    cout << "========================================\n";
    cout << "STATE PATTERN DEMO\n";
    cout << "========================================\n";

    demonstrateVendingMachine();
    demonstrateTCPConnection();
    demonstrateDocument();
    demonstrateTrafficLight();

    // ============================================
    // Benefits Summary
    // ============================================
    cout << "\n========================================\n";
    cout << "BENEFITS OF STATE PATTERN\n";
    cout << "========================================\n";
    cout << "✅ Single Responsibility: Each state encapsulates behavior\n";
    cout << "✅ Open/Closed: Add new states without modifying existing ones\n";
    cout << "✅ Eliminates complex conditionals: No large if/switch statements\n";
    cout << "✅ Explicit state transitions: Clear state machine logic\n";
    cout << "✅ Context simplification: Context delegates to state objects\n";
    cout << "✅ Easier maintenance: State-specific code is localized\n";

    cout << "\n========================================\n";
    cout << "WHEN TO USE STATE\n";
    cout << "========================================\n";
    cout << "1. Object behavior depends on its state\n";
    cout << "2. Operations have large conditional statements on state\n";
    cout << "3. State transitions are complex\n";
    cout << "4. Need to avoid code duplication across states\n";
    cout << "5. State-specific behavior changes frequently\n";
    cout << "6. Want explicit state machine implementation\n";

    cout << "\n========================================\n";
    cout << "STATE vs STRATEGY\n";
    cout << "========================================\n";
    cout << "State Pattern:\n";
    cout << "  • Encapsulates state-dependent behavior\n";
    cout << "  • States can transition to other states\n";
    cout << "  • Context changes behavior as state changes\n";
    cout << "  • Example: TCP connection states\n";
    cout << "\nStrategy Pattern:\n";
    cout << "  • Encapsulates interchangeable algorithms\n";
    cout << "  • Strategies don't know about each other\n";
    cout << "  • Client chooses strategy explicitly\n";
    cout << "  • Example: Different sorting algorithms\n";

    cout << "\n========================================\n";
    cout << "REAL-WORLD EXAMPLES\n";
    cout << "========================================\n";
    cout << "• Vending machines\n";
    cout << "• TCP/IP connection states\n";
    cout << "• Document workflow (draft, review, published)\n";
    cout << "• Traffic lights\n";
    cout << "• Order processing (pending, shipped, delivered)\n";
    cout << "• Game character states (idle, walking, jumping)\n";
    cout << "• Audio player (stopped, playing, paused)\n";
    cout << "• Login systems (logged out, logged in, locked)\n";
    cout << "• Elevator control systems\n";
    cout << "• Phone call states (idle, ringing, connected)\n";
    cout << "• ATM machine states\n";
    cout << "• Thread states (new, runnable, blocked, terminated)\n";

    cout << "\n========================================\n";
    cout << "KEY COMPONENTS\n";
    cout << "========================================\n";
    cout << "1. Context: Maintains current state, delegates requests\n";
    cout << "2. State Interface: Defines interface for state-specific behavior\n";
    cout << "3. Concrete States: Implement behavior for each state\n";
    cout << "4. State Transitions: States can change context's state\n";

    cout << "\n========================================\n";
    cout << "STATE TRANSITION APPROACHES\n";
    cout << "========================================\n";
    cout << "States Control Transitions:\n";
    cout << "  • States know about other states\n";
    cout << "  • States call context.setState()\n";
    cout << "  • More flexible, states are independent\n";
    cout << "  • Example: All examples above\n";
    cout << "\nContext Controls Transitions:\n";
    cout << "  • Context decides next state\n";
    cout << "  • States just execute behavior\n";
    cout << "  • Better encapsulation of state machine logic\n";
    cout << "  • Centralized transition rules\n";
    cout << "\nTable-Driven Transitions:\n";
    cout << "  • Transition table/matrix\n";
    cout << "  • Current state + event → next state\n";
    cout << "  • Easy to visualize and modify\n";
    cout << "  • Good for complex state machines\n";

    cout << "\n========================================\n";
    cout << "STRUCTURE\n";
    cout << "========================================\n";
    cout << "┌─────────────────┐\n";
    cout << "│    Context      │\n";
    cout << "│  (VendingMach.) │\n";
    cout << "│                 │\n";
    cout << "│ - state ────────┼─────┐\n";
    cout << "│ + request()     │     │ delegates\n";
    cout << "└─────────────────┘     │\n";
    cout << "                        ▼\n";
    cout << "                 ┌─────────────┐\n";
    cout << "                 │    State    │\n";
    cout << "                 │ (Interface) │\n";
    cout << "                 │             │\n";
    cout << "                 │ + handle()  │\n";
    cout << "                 └─────────────┘\n";
    cout << "                        △\n";
    cout << "        ┌───────────────┼───────────────┐\n";
    cout << "        │               │               │\n";
    cout << "┌─────────────┐ ┌─────────────┐ ┌─────────────┐\n";
    cout << "│  StateA     │ │  StateB     │ │  StateC     │\n";
    cout << "│ (NoCoin)    │ │ (HasCoin)   │ │ (Dispensing)│\n";
    cout << "│             │ │             │ │             │\n";
    cout << "│ + handle()  │ │ + handle()  │ │ + handle()  │\n";
    cout << "└─────────────┘ └─────────────┘ └─────────────┘\n";

    cout << "\n========================================\n";
    cout << "ADVANTAGES\n";
    cout << "========================================\n";
    cout << "• Eliminates complex conditional logic\n";
    cout << "• Localizes state-specific behavior\n";
    cout << "• Makes state transitions explicit\n";
    cout << "• Easy to add new states\n";
    cout << "• Improves code organization\n";
    cout << "• States are reusable\n";
    cout << "• Follows Single Responsibility Principle\n";
    cout << "• Easier to test individual states\n";

    cout << "\n========================================\n";
    cout << "DISADVANTAGES\n";
    cout << "========================================\n";
    cout << "• Increases number of classes\n";
    cout << "• Can be overkill for simple state machines\n";
    cout << "• State transitions can be hard to follow\n";
    cout << "• May require many small classes\n";
    cout << "• States may depend on each other\n";

    cout << "\n========================================\n";
    cout << "BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "• Keep states independent when possible\n";
    cout << "• Document state transition diagram\n";
    cout << "• Use shared_ptr for state objects\n";
    cout << "• Consider state object pooling for performance\n";
    cout << "• Handle invalid transitions gracefully\n";
    cout << "• Log state changes for debugging\n";
    cout << "• Consider using state machines libraries\n";
    cout << "• Test all state transitions thoroughly\n";

    cout << "\n========================================\n";
    cout << "IMPLEMENTATION VARIATIONS\n";
    cout << "========================================\n";
    cout << "Singleton States:\n";
    cout << "  • One instance per state (flyweight)\n";
    cout << "  • More memory efficient\n";
    cout << "  • States must be stateless\n";
    cout << "  • Example: Traffic light colors\n";
    cout << "\nStateful States:\n";
    cout << "  • New state instance per transition\n";
    cout << "  • Can store state-specific data\n";
    cout << "  • More flexible but uses more memory\n";
    cout << "  • Example: Document with version history\n";
    cout << "\nHierarchical States:\n";
    cout << "  • Nested state machines\n";
    cout << "  • Super-states and sub-states\n";
    cout << "  • Useful for complex systems\n";
    cout << "  • Example: Game with multiple levels\n";

    cout << "\n========================================\n";
    cout << "COMMON ISSUES & SOLUTIONS\n";
    cout << "========================================\n";
    cout << "State Explosion:\n";
    cout << "  • Issue: Too many states\n";
    cout << "  • Solution: Hierarchical states, composite pattern\n";
    cout << "\nShared Behavior:\n";
    cout << "  • Issue: Code duplication across states\n";
    cout << "  • Solution: Abstract base state with common methods\n";
    cout << "\nState Dependencies:\n";
    cout << "  • Issue: States need to know about each other\n";
    cout << "  • Solution: Use factory or registry pattern\n";
    cout << "\nContext Bloat:\n";
    cout << "  • Issue: Context has too many methods\n";
    cout << "  • Solution: Group related operations, use delegation\n";

    cout << "\n========================================\n";
    cout << "STATE MACHINE TOOLS\n";
    cout << "========================================\n";
    cout << "State Diagram:\n";
    cout << "  • Visual representation of states and transitions\n";
    cout << "  • UML state diagrams\n";
    cout << "  • Essential for documentation\n";
    cout << "\nState Table:\n";
    cout << "  • Matrix of current state × event → next state\n";
    cout << "  • Easy to verify completeness\n";
    cout << "  • Good for code generation\n";
    cout << "\nLibraries:\n";
    cout << "  • Boost.MSM (Meta State Machine)\n";
    cout << "  • Boost.Statechart\n";
    cout << "  • TinyFSM (lightweight)\n";
    cout << "  • Custom implementations\n";

    cout << "\n========================================\n";
    cout << "RELATED PATTERNS\n";
    cout << "========================================\n";
    cout << "Strategy:\n";
    cout << "  • State is like Strategy with self-referencing\n";
    cout << "  • States can change context's behavior\n";
    cout << "\nFlyweight:\n";
    cout << "  • Can share state objects (singleton states)\n";
    cout << "  • Reduces memory for stateless states\n";
    cout << "\nSingleton:\n";
    cout << "  • Often used for state objects\n";
    cout << "  • One instance per state type\n";
    cout << "\nCommand:\n";
    cout << "  • State transitions can be commands\n";
    cout << "  • Enables undo/redo of state changes\n";

    cout << "\n========================================\n";
    cout << "TESTING STATE MACHINES\n";
    cout << "========================================\n";
    cout << "Unit Tests:\n";
    cout << "  • Test each state independently\n";
    cout << "  • Verify state-specific behavior\n";
    cout << "  • Mock context if needed\n";
    cout << "\nIntegration Tests:\n";
    cout << "  • Test state transitions\n";
    cout << "  • Verify complete workflows\n";
    cout << "  • Test invalid transition handling\n";
    cout << "\nCoverage Goals:\n";
    cout << "  • All states visited\n";
    cout << "  • All transitions tested\n";
    cout << "  • All edge cases covered\n";
    cout << "  • Invalid operations handled\n";

    return 0;
}
