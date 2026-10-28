#include <string>
#include <vector>
#include <memory>
#include <map>
#include <print>

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
    virtual void sendMessage(const std::string& message, User* sender) = 0;

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
    std::string name;

public:
    User(const std::string& n, ChatMediator* med)
        : name(n), mediator(med) {}

    virtual ~User() {}

    std::string getName() const { return name; }

    // Send message through mediator
    virtual void send(const std::string& message) {
        std::println("💬 {} sends: {}", name, message);
        mediator->sendMessage(message, this);
    }

    // Receive message from mediator
    virtual void receive(const std::string& message, const std::string& senderName) {
        std::println("📩 {} received from {}: {}", name, senderName, message);
    }
};

/**
 * Concrete Mediator: Chat Room
 * Coordinates communication between users
 */
class ChatRoom : public ChatMediator {
private:
    std::vector<User*> users;  // All users in chat room

public:
    // Add user to chat room
    void addUser(User* user) override {
        users.push_back(user);
        std::println("✅ {} joined the chat room", user->getName());
    }

    // Broadcast message to all users except sender
    void sendMessage(const std::string& message, User* sender) override {
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
    std::string flightNumber;
    ATCMediator* atc;

public:
    Aircraft(const std::string& flight, ATCMediator* mediator)
        : flightNumber(flight), atc(mediator) {
        atc->registerAircraft(this);
    }

    virtual ~Aircraft() {}

    std::string getFlightNumber() const { return flightNumber; }

    // Request to land
    void requestLanding() {
        std::println("✈️  {} requesting landing clearance...", flightNumber);
        if (atc->requestLanding(this)) {
            std::println("✅ {} cleared to land", flightNumber);
            land();
        } else {
            std::println("❌ {} denied - runway busy", flightNumber);
        }
    }

    // Request to takeoff
    void requestTakeoff() {
        std::println("🛫 {} requesting takeoff clearance...", flightNumber);
        if (atc->requestTakeoff(this)) {
            std::println("✅ {} cleared for takeoff", flightNumber);
            takeoff();
        } else {
            std::println("❌ {} denied - runway busy", flightNumber);
        }
    }

private:
    void land() {
        std::println("🛬 {} landing...", flightNumber);
        atc->notifyLanded(this);
    }

    void takeoff() {
        std::println("🚀 {} taking off...", flightNumber);
        atc->notifyTakenOff(this);
    }
};

/**
 * Concrete Mediator: Air Traffic Control Tower
 * Coordinates all aircraft operations
 */
class ControlTower : public ATCMediator {
private:
    std::vector<Aircraft*> aircrafts;
    bool runwayAvailable;

public:
    ControlTower() : runwayAvailable(true) {}

    void registerAircraft(Aircraft* aircraft) override {
        aircrafts.push_back(aircraft);
        std::println("📡 {} registered with ATC", aircraft->getFlightNumber());
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
        std::println("🏁 {} has landed safely", aircraft->getFlightNumber());
        runwayAvailable = true;  // Runway now free
    }

    void notifyTakenOff(Aircraft* aircraft) override {
        std::println("🌤️  {} has taken off", aircraft->getFlightNumber());
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
    virtual void notify(SmartDevice* sender, const std::string& event) = 0;
};

/**
 * Smart Device Base Class
 */
class SmartDevice {
protected:
    std::string name;
    SmartHomeMediator* hub;

public:
    SmartDevice(const std::string& n, SmartHomeMediator* mediator)
        : name(n), hub(mediator) {
        hub->registerDevice(this);
    }

    virtual ~SmartDevice() {}

    std::string getName() const { return name; }

    virtual void performAction(const std::string& action) = 0;
};

/**
 * Concrete Device: Motion Sensor
 */
class MotionSensor : public SmartDevice {
public:
    MotionSensor(const std::string& n, SmartHomeMediator* mediator)
        : SmartDevice(n, mediator) {}

    void detectMotion() {
        std::println("🚶 {} detected motion!", name);
        hub->notify(this, "motion_detected");
    }

    void performAction(const std::string& action) override {
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
    Light(const std::string& n, SmartHomeMediator* mediator)
        : SmartDevice(n, mediator), isOn(false) {}

    void performAction(const std::string& action) override {
        if (action == "turn_on") {
            isOn = true;
            std::println("💡 {} turned ON", name);
        } else if (action == "turn_off") {
            isOn = false;
            std::println("🌑 {} turned OFF", name);
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
    Thermostat(const std::string& n, SmartHomeMediator* mediator)
        : SmartDevice(n, mediator), temperature(20) {}

    void performAction(const std::string& action) override {
        if (action == "increase_temp") {
            temperature += 2;
            std::println("🌡️  {} increased to {}°C", name, temperature);
        } else if (action == "decrease_temp") {
            temperature -= 2;
            std::println("❄️  {} decreased to {}°C", name, temperature);
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
    SecurityCamera(const std::string& n, SmartHomeMediator* mediator)
        : SmartDevice(n, mediator), recording(false) {}

    void performAction(const std::string& action) override {
        if (action == "start_recording") {
            recording = true;
            std::println("📹 {} started recording", name);
        } else if (action == "stop_recording") {
            recording = false;
            std::println("⏹️  {} stopped recording", name);
        }
    }
};

/**
 * Concrete Mediator: Smart Home Hub
 * Coordinates all smart devices
 */
class SmartHomeHub : public SmartHomeMediator {
private:
    std::map<std::string, SmartDevice*> devices;

public:
    void registerDevice(SmartDevice* device) override {
        devices[device->getName()] = device;
        std::println("🏠 {} registered with Smart Home Hub", device->getName());
    }

    void notify(SmartDevice* sender, const std::string& event) override {
        std::println("🔔 Hub received: {} from {}", event, sender->getName());

        // Coordinate responses based on events
        if (event == "motion_detected") {
            // Turn on lights
            for (auto& pair : devices) {
                if (pair.first.find("Light") != std::string::npos) {
                    pair.second->performAction("turn_on");
                }
            }
            // Start camera recording
            for (auto& pair : devices) {
                if (pair.first.find("Camera") != std::string::npos) {
                    pair.second->performAction("start_recording");
                }
            }
        }
    }

    // Manual control through hub
    void controlDevice(const std::string& deviceName, const std::string& action) {
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
    std::string name;
    AuctionMediator* auction;

public:
    Bidder(const std::string& n, AuctionMediator* med)
        : name(n), auction(med) {
        auction->registerBidder(this);
    }

    std::string getName() const { return name; }

    void bid(int amount) {
        std::println("💰 {} bids ${}", name, amount);
        auction->placeBid(this, amount);
    }

    void notifyOutbid(const std::string& bidderName, int amount) {
        std::println("📢 {} notified: {} bid ${} (you were outbid)", name, bidderName, amount);
    }

    void notifyWinner() {
        std::println("🏆 {} WON the auction!", name);
    }
};

/**
 * Concrete Mediator: Auction House
 */
class AuctionHouse : public AuctionMediator {
private:
    std::vector<Bidder*> bidders;
    Bidder* highestBidder;
    int highestBid;
    std::string itemName;

public:
    AuctionHouse(const std::string& item)
        : itemName(item), highestBidder(nullptr), highestBid(0) {}

    void registerBidder(Bidder* bidder) override {
        bidders.push_back(bidder);
        std::println("✅ {} registered for auction: {}", bidder->getName(), itemName);
    }

    void placeBid(Bidder* bidder, int amount) override {
        if (amount > highestBid) {
            // Notify previous highest bidder
            if (highestBidder != nullptr) {
                highestBidder->notifyOutbid(bidder->getName(), amount);
            }

            highestBid = amount;
            highestBidder = bidder;

            std::println("🔨 New highest bid: ${} by {}", amount, bidder->getName());
        } else {
            std::println("❌ Bid rejected: Must be higher than ${}", highestBid);
        }
    }

    void closeAuction() {
        std::println("");
        std::println("🔚 Auction closed for: {}", itemName);
        if (highestBidder) {
            highestBidder->notifyWinner();
            std::println("Final price: ${}", highestBid);
        }
    }
};

// ============================================
// DEMONSTRATION FUNCTIONS
// ============================================

void demonstrateChatRoom() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: CHAT ROOM              ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

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

    std::println("");
    std::println("--- Chat Messages ---");
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
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: AIR TRAFFIC CONTROL    ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    // Create mediator
    ControlTower* tower = new ControlTower();

    // Create aircraft (automatically registers with tower)
    Aircraft* flight1 = new Aircraft("AA101", tower);
    Aircraft* flight2 = new Aircraft("UA202", tower);
    Aircraft* flight3 = new Aircraft("DL303", tower);

    std::println("");
    std::println("--- Flight Operations ---");
    flight1->requestLanding();
    flight2->requestLanding();  // Should be denied - runway busy
    flight3->requestTakeoff();   // Should be denied - runway busy

    std::println("");
    std::println("--- After Flight 1 Lands ---");
    flight2->requestTakeoff();   // Should succeed now

    // Cleanup
    delete flight1;
    delete flight2;
    delete flight3;
    delete tower;
}

void demonstrateSmartHome() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: SMART HOME SYSTEM      ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    // Create mediator
    SmartHomeHub* hub = new SmartHomeHub();

    // Create devices (automatically registers with hub)
    MotionSensor* sensor = new MotionSensor("Entry Motion Sensor", hub);
    Light* livingRoomLight = new Light("Living Room Light", hub);
    Light* hallwayLight = new Light("Hallway Light", hub);
    SecurityCamera* camera = new SecurityCamera("Front Door Camera", hub);
    Thermostat* thermostat = new Thermostat("Main Thermostat", hub);

    std::println("");
    std::println("--- Motion Detected Event ---");
    sensor->detectMotion();  // Should trigger lights and camera

    std::println("");
    std::println("--- Manual Control ---");
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
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: ONLINE AUCTION         ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    // Create mediator
    AuctionHouse* auction = new AuctionHouse("Vintage Guitar");

    // Create bidders (automatically registers)
    Bidder* john = new Bidder("John", auction);
    Bidder* sarah = new Bidder("Sarah", auction);
    Bidder* mike = new Bidder("Mike", auction);

    std::println("");
    std::println("--- Bidding Process ---");
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
    std::println("========================================");
    std::println("MEDIATOR PATTERN DEMO");
    std::println("========================================");

    demonstrateChatRoom();
    demonstrateAirTrafficControl();
    demonstrateSmartHome();
    demonstrateAuction();

    return 0;
}
