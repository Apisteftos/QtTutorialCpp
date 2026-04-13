#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>
using namespace std;

// ============================================
// EXAMPLE 1: CHAT ROOM MEDIATOR
// Classic mediator pattern example
// ============================================

// Forward declarations
class User;

/**
 * Mediator Interface
 * Defines communication interface between colleagues
 */
class ChatMediator {
public:
    virtual ~ChatMediator() {}

    // Send message from one user to another or to all
    virtual void sendMessage(const string& message, User* sender) = 0;

    // Add user to chat room
    virtual void addUser(User* user) = 0;
};

/**
 * Colleague (User) Class
 * Communicates through mediator, not directly
 */
class User {
protected:
    ChatMediator* mediator;  // Reference to mediator
    string name;

public:
    User(const string& n, ChatMediator* med)
        : name(n), mediator(med) {}

    virtual ~User() {}

    string getName() const { return name; }

    // Send message through mediator
    virtual void send(const string& message) {
        cout << "💬 " << name << " sends: " << message << "\n";
        mediator->sendMessage(message, this);
    }

    // Receive message from mediator
    virtual void receive(const string& message, const string& senderName) {
        cout << "📩 " << name << " received from " << senderName
             << ": " << message << "\n";
    }
};

/**
 * Concrete Mediator: Chat Room
 * Coordinates communication between users
 */
class ChatRoom : public ChatMediator {
private:
    vector<User*> users;  // All users in chat room

public:
    // Add user to chat room
    void addUser(User* user) override {
        users.push_back(user);
        cout << "✅ " << user->getName() << " joined the chat room\n";
    }

    // Broadcast message to all users except sender
    void sendMessage(const string& message, User* sender) override {
        for (User* user : users) {
            // Don't send message back to sender
            if (user != sender) {
                user->receive(message, sender->getName());
            }
        }
    }
};

// ============================================
// EXAMPLE 2: AIR TRAFFIC CONTROL
// Aircraft coordination system
// ============================================

// Forward declarations
class Aircraft;

/**
 * Air Traffic Control Mediator Interface
 */
class ATCMediator {
public:
    virtual ~ATCMediator() {}

    // Register aircraft with ATC
    virtual void registerAircraft(Aircraft* aircraft) = 0;

    // Request landing clearance
    virtual bool requestLanding(Aircraft* aircraft) = 0;

    // Request takeoff clearance
    virtual bool requestTakeoff(Aircraft* aircraft) = 0;

    // Notify landing complete
    virtual void notifyLanded(Aircraft* aircraft) = 0;

    // Notify takeoff complete
    virtual void notifyTakenOff(Aircraft* aircraft) = 0;
};

/**
 * Aircraft Class
 * Colleague that communicates through ATC mediator
 */
class Aircraft {
protected:
    string flightNumber;
    ATCMediator* atc;

public:
    Aircraft(const string& flight, ATCMediator* mediator)
        : flightNumber(flight), atc(mediator) {
        atc->registerAircraft(this);
    }

    virtual ~Aircraft() {}

    string getFlightNumber() const { return flightNumber; }

    // Request to land
    void requestLanding() {
        cout << "✈️  " << flightNumber << " requesting landing clearance...\n";
        if (atc->requestLanding(this)) {
            cout << "✅ " << flightNumber << " cleared to land\n";
            land();
        } else {
            cout << "❌ " << flightNumber << " denied - runway busy\n";
        }
    }

    // Request to takeoff
    void requestTakeoff() {
        cout << "🛫 " << flightNumber << " requesting takeoff clearance...\n";
        if (atc->requestTakeoff(this)) {
            cout << "✅ " << flightNumber << " cleared for takeoff\n";
            takeoff();
        } else {
            cout << "❌ " << flightNumber << " denied - runway busy\n";
        }
    }

private:
    void land() {
        cout << "🛬 " << flightNumber << " landing...\n";
        atc->notifyLanded(this);
    }

    void takeoff() {
        cout << "🚀 " << flightNumber << " taking off...\n";
        atc->notifyTakenOff(this);
    }
};

/**
 * Concrete Mediator: Air Traffic Control Tower
 * Coordinates all aircraft operations
 */
class ControlTower : public ATCMediator {
private:
    vector<Aircraft*> aircrafts;
    bool runwayAvailable;

public:
    ControlTower() : runwayAvailable(true) {}

    void registerAircraft(Aircraft* aircraft) override {
        aircrafts.push_back(aircraft);
        cout << "📡 " << aircraft->getFlightNumber() << " registered with ATC\n";
    }

    bool requestLanding(Aircraft* aircraft) override {
        if (runwayAvailable) {
            runwayAvailable = false;  // Runway now occupied
            return true;
        }
        return false;
    }

    bool requestTakeoff(Aircraft* aircraft) override {
        if (runwayAvailable) {
            runwayAvailable = false;  // Runway now occupied
            return true;
        }
        return false;
    }

    void notifyLanded(Aircraft* aircraft) override {
        cout << "🏁 " << aircraft->getFlightNumber() << " has landed safely\n";
        runwayAvailable = true;  // Runway now free
    }

    void notifyTakenOff(Aircraft* aircraft) override {
        cout << "🌤️  " << aircraft->getFlightNumber() << " has taken off\n";
        runwayAvailable = true;  // Runway now free
    }
};

// ============================================
// EXAMPLE 3: SMART HOME MEDIATOR
// Home automation system
// ============================================

// Forward declarations
class SmartDevice;

/**
 * Smart Home Mediator Interface
 */
class SmartHomeMediator {
public:
    virtual ~SmartHomeMediator() {}

    // Register device with hub
    virtual void registerDevice(SmartDevice* device) = 0;

    // Notify hub of device state change
    virtual void notify(SmartDevice* sender, const string& event) = 0;
};

/**
 * Smart Device Base Class
 */
class SmartDevice {
protected:
    string name;
    SmartHomeMediator* hub;

public:
    SmartDevice(const string& n, SmartHomeMediator* mediator)
        : name(n), hub(mediator) {
        hub->registerDevice(this);
    }

    virtual ~SmartDevice() {}

    string getName() const { return name; }

    virtual void performAction(const string& action) = 0;
};

/**
 * Concrete Device: Motion Sensor
 */
class MotionSensor : public SmartDevice {
public:
    MotionSensor(const string& n, SmartHomeMediator* mediator)
        : SmartDevice(n, mediator) {}

    void detectMotion() {
        cout << "🚶 " << name << " detected motion!\n";
        hub->notify(this, "motion_detected");
    }

    void performAction(const string& action) override {
        // Motion sensor doesn't perform actions
    }
};

/**
 * Concrete Device: Light
 */
class Light : public SmartDevice {
private:
    bool isOn;

public:
    Light(const string& n, SmartHomeMediator* mediator)
        : SmartDevice(n, mediator), isOn(false) {}

    void performAction(const string& action) override {
        if (action == "turn_on") {
            isOn = true;
            cout << "💡 " << name << " turned ON\n";
        } else if (action == "turn_off") {
            isOn = false;
            cout << "🌑 " << name << " turned OFF\n";
        }
    }
};

/**
 * Concrete Device: Thermostat
 */
class Thermostat : public SmartDevice {
private:
    int temperature;

public:
    Thermostat(const string& n, SmartHomeMediator* mediator)
        : SmartDevice(n, mediator), temperature(20) {}

    void performAction(const string& action) override {
        if (action == "increase_temp") {
            temperature += 2;
            cout << "🌡️  " << name << " increased to " << temperature << "°C\n";
        } else if (action == "decrease_temp") {
            temperature -= 2;
            cout << "❄️  " << name << " decreased to " << temperature << "°C\n";
        }
    }
};

/**
 * Concrete Device: Security Camera
 */
class SecurityCamera : public SmartDevice {
private:
    bool recording;

public:
    SecurityCamera(const string& n, SmartHomeMediator* mediator)
        : SmartDevice(n, mediator), recording(false) {}

    void performAction(const string& action) override {
        if (action == "start_recording") {
            recording = true;
            cout << "📹 " << name << " started recording\n";
        } else if (action == "stop_recording") {
            recording = false;
            cout << "⏹️  " << name << " stopped recording\n";
        }
    }
};

/**
 * Concrete Mediator: Smart Home Hub
 * Coordinates all smart devices
 */
class SmartHomeHub : public SmartHomeMediator {
private:
    map<string, SmartDevice*> devices;

public:
    void registerDevice(SmartDevice* device) override {
        devices[device->getName()] = device;
        cout << "🏠 " << device->getName() << " registered with Smart Home Hub\n";
    }

    void notify(SmartDevice* sender, const string& event) override {
        cout << "🔔 Hub received: " << event << " from " << sender->getName() << "\n";

        // Coordinate responses based on events
        if (event == "motion_detected") {
            // Turn on lights
            for (auto& pair : devices) {
                if (pair.first.find("Light") != string::npos) {
                    pair.second->performAction("turn_on");
                }
            }
            // Start camera recording
            for (auto& pair : devices) {
                if (pair.first.find("Camera") != string::npos) {
                    pair.second->performAction("start_recording");
                }
            }
        }
    }

    // Manual control through hub
    void controlDevice(const string& deviceName, const string& action) {
        if (devices.find(deviceName) != devices.end()) {
            devices[deviceName]->performAction(action);
        }
    }
};

// ============================================
// EXAMPLE 4: ONLINE AUCTION MEDIATOR
// Bidding system coordination
// ============================================

class Bidder;

/**
 * Auction Mediator Interface
 */
class AuctionMediator {
public:
    virtual ~AuctionMediator() {}
    virtual void registerBidder(Bidder* bidder) = 0;
    virtual void placeBid(Bidder* bidder, int amount) = 0;
};

/**
 * Bidder Class
 */
class Bidder {
protected:
    string name;
    AuctionMediator* auction;

public:
    Bidder(const string& n, AuctionMediator* med)
        : name(n), auction(med) {
        auction->registerBidder(this);
    }

    string getName() const { return name; }

    void bid(int amount) {
        cout << "💰 " << name << " bids $" << amount << "\n";
        auction->placeBid(this, amount);
    }

    void notifyOutbid(const string& bidderName, int amount) {
        cout << "📢 " << name << " notified: " << bidderName
             << " bid $" << amount << " (you were outbid)\n";
    }

    void notifyWinner() {
        cout << "🏆 " << name << " WON the auction!\n";
    }
};

/**
 * Concrete Mediator: Auction House
 */
class AuctionHouse : public AuctionMediator {
private:
    vector<Bidder*> bidders;
    Bidder* highestBidder;
    int highestBid;
    string itemName;

public:
    AuctionHouse(const string& item)
        : itemName(item), highestBidder(nullptr), highestBid(0) {}

    void registerBidder(Bidder* bidder) override {
        bidders.push_back(bidder);
        cout << "✅ " << bidder->getName() << " registered for auction: "
             << itemName << "\n";
    }

    void placeBid(Bidder* bidder, int amount) override {
        if (amount > highestBid) {
            // Notify previous highest bidder
            if (highestBidder != nullptr) {
                highestBidder->notifyOutbid(bidder->getName(), amount);
            }

            highestBid = amount;
            highestBidder = bidder;

            cout << "🔨 New highest bid: $" << amount << " by "
                 << bidder->getName() << "\n";
        } else {
            cout << "❌ Bid rejected: Must be higher than $" << highestBid << "\n";
        }
    }

    void closeAuction() {
        cout << "\n🔚 Auction closed for: " << itemName << "\n";
        if (highestBidder) {
            highestBidder->notifyWinner();
            cout << "Final price: $" << highestBid << "\n";
        }
    }
};

// ============================================
// DEMONSTRATION FUNCTIONS
// ============================================

void demonstrateChatRoom() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: CHAT ROOM              ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    // Create mediator
    ChatRoom* chatRoom = new ChatRoom();

    // Create users
    User* alice = new User("Alice", chatRoom);
    User* bob = new User("Bob", chatRoom);
    User* charlie = new User("Charlie", chatRoom);

    // Add users to chat
    chatRoom->addUser(alice);
    chatRoom->addUser(bob);
    chatRoom->addUser(charlie);

    cout << "\n--- Chat Messages ---\n";
    alice->send("Hello everyone!");
    bob->send("Hi Alice!");
    charlie->send("Hey folks!");

    // Cleanup
    delete alice;
    delete bob;
    delete charlie;
    delete chatRoom;
}

void demonstrateAirTrafficControl() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: AIR TRAFFIC CONTROL    ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    // Create mediator
    ControlTower* tower = new ControlTower();

    // Create aircraft (automatically registers with tower)
    Aircraft* flight1 = new Aircraft("AA101", tower);
    Aircraft* flight2 = new Aircraft("UA202", tower);
    Aircraft* flight3 = new Aircraft("DL303", tower);

    cout << "\n--- Flight Operations ---\n";
    flight1->requestLanding();
    flight2->requestLanding();  // Should be denied - runway busy
    flight3->requestTakeoff();   // Should be denied - runway busy

    cout << "\n--- After Flight 1 Lands ---\n";
    flight2->requestTakeoff();   // Should succeed now

    // Cleanup
    delete flight1;
    delete flight2;
    delete flight3;
    delete tower;
}

void demonstrateSmartHome() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: SMART HOME SYSTEM      ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    // Create mediator
    SmartHomeHub* hub = new SmartHomeHub();

    // Create devices (automatically registers with hub)
    MotionSensor* sensor = new MotionSensor("Entry Motion Sensor", hub);
    Light* livingRoomLight = new Light("Living Room Light", hub);
    Light* hallwayLight = new Light("Hallway Light", hub);
    SecurityCamera* camera = new SecurityCamera("Front Door Camera", hub);
    Thermostat* thermostat = new Thermostat("Main Thermostat", hub);

    cout << "\n--- Motion Detected Event ---\n";
    sensor->detectMotion();  // Should trigger lights and camera

    cout << "\n--- Manual Control ---\n";
    hub->controlDevice("Main Thermostat", "increase_temp");

    // Cleanup
    delete sensor;
    delete livingRoomLight;
    delete hallwayLight;
    delete camera;
    delete thermostat;
    delete hub;
}

void demonstrateAuction() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: ONLINE AUCTION         ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    // Create mediator
    AuctionHouse* auction = new AuctionHouse("Vintage Guitar");

    // Create bidders (automatically registers)
    Bidder* john = new Bidder("John", auction);
    Bidder* sarah = new Bidder("Sarah", auction);
    Bidder* mike = new Bidder("Mike", auction);

    cout << "\n--- Bidding Process ---\n";
    john->bid(1000);
    sarah->bid(1200);
    mike->bid(1100);   // Should be rejected
    john->bid(1500);
    sarah->bid(2000);

    auction->closeAuction();

    // Cleanup
    delete john;
    delete sarah;
    delete mike;
    delete auction;
}

// ============================================
// MAIN - DEMONSTRATING THE PATTERN
// ============================================

int main() {
    cout << "========================================\n";
    cout << "MEDIATOR PATTERN DEMO\n";
    cout << "========================================\n";

    demonstrateChatRoom();
    demonstrateAirTrafficControl();
    demonstrateSmartHome();
    demonstrateAuction();

    // ============================================
    // Benefits Summary
    // ============================================
    cout << "\n========================================\n";
    cout << "BENEFITS OF MEDIATOR PATTERN\n";
    cout << "========================================\n";
    cout << "✅ Single Responsibility: Centralizes communication logic\n";
    cout << "✅ Open/Closed: Add new colleagues without changing others\n";
    cout << "✅ Reduces coupling: Objects don't reference each other directly\n";
    cout << "✅ Simplifies object protocols: One-to-many relationships\n";
    cout << "✅ Reusable components: Colleagues are more reusable\n";
    cout << "✅ Easier maintenance: Communication logic in one place\n";

    cout << "\n========================================\n";
    cout << "WHEN TO USE MEDIATOR\n";
    cout << "========================================\n";
    cout << "1. Objects communicate in complex but well-defined ways\n";
    cout << "2. Reusing objects is difficult due to many dependencies\n";
    cout << "3. Behavior distributed among classes should be customizable\n";
    cout << "4. Too many relationships make system hard to understand\n";
    cout << "5. Need centralized control over object interactions\n";

    cout << "\n========================================\n";
    cout << "MEDIATOR vs OBSERVER\n";
    cout << "========================================\n";
    cout << "Mediator Pattern:\n";
    cout << "  • Two-way communication\n";
    cout << "  • Colleagues can request actions from mediator\n";
    cout << "  • Centralized control logic\n";
    cout << "  • Example: Chat room coordinates user messages\n";
    cout << "\nObserver Pattern:\n";
    cout << "  • One-way communication (subject → observers)\n";
    cout << "  • Observers just receive notifications\n";
    cout << "  • Distributed notification logic\n";
    cout << "  • Example: Newsletter notifies subscribers\n";

    cout << "\n========================================\n";
    cout << "REAL-WORLD EXAMPLES\n";
    cout << "========================================\n";
    cout << "• Chat rooms and messaging apps\n";
    cout << "• Air traffic control systems\n";
    cout << "• Smart home automation hubs\n";
    cout << "• Online auction platforms\n";
    cout << "• GUI dialog boxes (coordinate widgets)\n";
    cout << "• Game multiplayer servers\n";
    cout << "• Stock exchange trading systems\n";
    cout << "• Airport gate assignment systems\n";
    cout << "• Conference call coordinators\n";
    cout << "• Workflow management systems\n";

    cout << "\n========================================\n";
    cout << "KEY COMPONENTS\n";
    cout << "========================================\n";
    cout << "1. Mediator Interface: Defines communication protocol\n";
    cout << "2. Concrete Mediator: Implements coordination logic\n";
    cout << "3. Colleague Classes: Objects that communicate via mediator\n";
    cout << "4. Reference: Each colleague has reference to mediator\n";

    cout << "\n========================================\n";
    cout << "STRUCTURE\n";
    cout << "========================================\n";
    cout << "Without Mediator (N² connections):\n";
    cout << "  Colleague1 ←→ Colleague2\n";
    cout << "      ↕           ↕\n";
    cout << "  Colleague3 ←→ Colleague4\n";
    cout << "\nWith Mediator (N connections):\n";
    cout << "  Colleague1 ←→ Mediator ←→ Colleague3\n";
    cout << "                   ↕\n";
    cout << "  Colleague2 ←→ Mediator ←→ Colleague4\n";

    cout << "\n========================================\n";
    cout << "ADVANTAGES\n";
    cout << "========================================\n";
    cout << "• Decouples colleagues from each other\n";
    cout << "• Reduces number of connections (N² → N)\n";
    cout << "• Centralizes control logic\n";
    cout << "• Simplifies object protocols\n";
    cout << "• Easier to understand and maintain\n";
    cout << "• Colleagues become more reusable\n";
    cout << "• Can introduce new mediators without changing colleagues\n";

    cout << "\n========================================\n";
    cout << "DISADVANTAGES\n";
    cout << "========================================\n";
    cout << "• Mediator can become a 'god object' (too complex)\n";
    cout << "• May be harder to debug (centralized control)\n";
    cout << "• Can reduce performance (extra indirection)\n";
    cout << "• Mediator itself can be hard to maintain if too complex\n";

    cout << "\n========================================\n";
    cout << "BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "• Keep mediator focused on coordination, not business logic\n";
    cout << "• Use events/messages instead of direct method calls\n";
    cout << "• Consider breaking complex mediators into smaller ones\n";
    cout << "• Colleagues should be unaware of each other\n";
    cout << "• Document the communication protocol clearly\n";

    cout << "\n========================================\n";
    cout << "COMMON IMPLEMENTATIONS\n";
    cout << "========================================\n";
    cout << "Event-Based Mediator:\n";
    cout << "  • Colleagues send events to mediator\n";
    cout << "  • Mediator dispatches to appropriate handlers\n";
    cout << "  • Most flexible approach\n";
    cout << "\nMethod-Based Mediator:\n";
    cout << "  • Mediator has explicit methods for each interaction\n";
    cout << "  • More type-safe\n";
    cout << "  • Example: ATC with requestLanding(), requestTakeoff()\n";
    cout << "\nMessage Queue Mediator:\n";
    cout << "  • Asynchronous communication\n";
    cout << "  • Useful for distributed systems\n";
    cout << "  • Example: Message brokers (RabbitMQ, Kafka)\n";

    return 0;
}
