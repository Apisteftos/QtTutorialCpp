#include <iostream>
#include <string>
#include <memory>
#include <vector>
using namespace std;

// ============================================
// EXAMPLE 1: HOME THEATER SYSTEM
// Classic facade pattern example
// ============================================

/**
 * Subsystem Component 1: DVD Player
 */
class DVDPlayer {
public:
    void on() {
        cout << "📀 DVD Player: Turning ON\n";
    }

    void off() {
        cout << "📀 DVD Player: Turning OFF\n";
    }

    void play(const string& movie) {
        cout << "📀 DVD Player: Playing '" << movie << "'\n";
    }

    void stop() {
        cout << "📀 DVD Player: Stopped\n";
    }

    void eject() {
        cout << "📀 DVD Player: Ejecting disc\n";
    }
};

/**
 * Subsystem Component 2: Projector
 */
class Projector {
public:
    void on() {
        cout << "📽️  Projector: Turning ON\n";
    }

    void off() {
        cout << "📽️  Projector: Turning OFF\n";
    }

    void wideScreenMode() {
        cout << "📽️  Projector: Setting wide screen mode (16:9)\n";
    }

    void tvMode() {
        cout << "📽️  Projector: Setting TV mode (4:3)\n";
    }
};

/**
 * Subsystem Component 3: Amplifier
 */
class Amplifier {
public:
    void on() {
        cout << "🔊 Amplifier: Turning ON\n";
    }

    void off() {
        cout << "🔊 Amplifier: Turning OFF\n";
    }

    void setDVD() {
        cout << "🔊 Amplifier: Setting DVD as input source\n";
    }

    void setSurroundSound() {
        cout << "🔊 Amplifier: Setting surround sound mode (5.1)\n";
    }

    void setVolume(int level) {
        cout << "🔊 Amplifier: Setting volume to " << level << "\n";
    }
};

/**
 * Subsystem Component 4: Lights
 */
class TheaterLights {
public:
    void dim(int level) {
        cout << "💡 Lights: Dimming to " << level << "%\n";
    }

    void on() {
        cout << "💡 Lights: Turning ON to 100%\n";
    }
};

/**
 * Subsystem Component 5: Screen
 */
class Screen {
public:
    void down() {
        cout << "🎬 Screen: Going DOWN\n";
    }

    void up() {
        cout << "🎬 Screen: Going UP\n";
    }
};

/**
 * Subsystem Component 6: Popcorn Maker
 */
class PopcornPopper {
public:
    void on() {
        cout << "🍿 Popcorn Maker: Turning ON\n";
    }

    void off() {
        cout << "🍿 Popcorn Maker: Turning OFF\n";
    }

    void pop() {
        cout << "🍿 Popcorn Maker: Popping popcorn!\n";
    }
};

/**
 * FACADE: HomeTheaterFacade
 * Provides simple interface to complex home theater subsystem
 */
class HomeTheaterFacade {
private:
    shared_ptr<DVDPlayer> dvd;
    shared_ptr<Projector> projector;
    shared_ptr<Amplifier> amp;
    shared_ptr<TheaterLights> lights;
    shared_ptr<Screen> screen;
    shared_ptr<PopcornPopper> popper;

public:
    HomeTheaterFacade(
        shared_ptr<DVDPlayer> d,
        shared_ptr<Projector> p,
        shared_ptr<Amplifier> a,
        shared_ptr<TheaterLights> l,
        shared_ptr<Screen> s,
        shared_ptr<PopcornPopper> pop
        ) : dvd(d), projector(p), amp(a), lights(l), screen(s), popper(pop) {}

    void watchMovie(const string& movie) {
        cout << "\n🎥 Get ready to watch a movie...\n";
        cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";

        popper->on();
        popper->pop();
        lights->dim(10);
        screen->down();
        projector->on();
        projector->wideScreenMode();
        amp->on();
        amp->setDVD();
        amp->setSurroundSound();
        amp->setVolume(5);
        dvd->on();
        dvd->play(movie);

        cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
        cout << "🎬 Movie is now playing! Enjoy!\n";
    }

    void endMovie() {
        cout << "\n🛑 Shutting down home theater...\n";
        cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";

        popper->off();
        lights->on();
        screen->up();
        projector->off();
        amp->off();
        dvd->stop();
        dvd->eject();
        dvd->off();

        cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
        cout << "✅ Home theater shut down complete!\n";
    }

    void listenToRadio(double frequency) {
        cout << "\n📻 Preparing to listen to radio...\n";
        cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";

        amp->on();
        amp->setVolume(3);
        cout << "📻 Tuning to " << frequency << " FM\n";

        cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
        cout << "🎵 Radio is now playing!\n";
    }
};

// ============================================
// EXAMPLE 2: COMPUTER SYSTEM
// Simplifying computer startup/shutdown
// ============================================

/**
 * Subsystem: CPU
 */
class CPU {
public:
    void freeze() {
        cout << "🔷 CPU: Freezing...\n";
    }

    void jump(long position) {
        cout << "🔷 CPU: Jumping to position " << position << "\n";
    }

    void execute() {
        cout << "🔷 CPU: Executing instructions\n";
    }
};

/**
 * Subsystem: Memory
 */
class Memory {
public:
    void load(long position, const string& data) {
        cout << "💾 Memory: Loading '" << data << "' at position " << position << "\n";
    }
};

/**
 * Subsystem: HardDrive
 */
class HardDrive {
public:
    string read(long lba, int size) {
        cout << "💿 HardDrive: Reading " << size << " bytes from sector " << lba << "\n";
        return "BOOT_DATA";
    }
};

/**
 * FACADE: ComputerFacade
 */
class ComputerFacade {
private:
    shared_ptr<CPU> cpu;
    shared_ptr<Memory> memory;
    shared_ptr<HardDrive> hd;

    const long BOOT_ADDRESS = 0x0000;
    const long BOOT_SECTOR = 0;
    const int SECTOR_SIZE = 512;

public:
    ComputerFacade() {
        cpu = make_shared<CPU>();
        memory = make_shared<Memory>();
        hd = make_shared<HardDrive>();
    }

    void start() {
        cout << "\n💻 Starting computer...\n";
        cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";

        cpu->freeze();
        string bootData = hd->read(BOOT_SECTOR, SECTOR_SIZE);
        memory->load(BOOT_ADDRESS, bootData);
        cpu->jump(BOOT_ADDRESS);
        cpu->execute();

        cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
        cout << "✅ Computer started successfully!\n";
    }
};

// ============================================
// EXAMPLE 3: ONLINE SHOPPING
// E-commerce order processing
// ============================================

/**
 * Subsystem: Inventory
 */
class Inventory {
public:
    bool checkStock(const string& product, int quantity) {
        cout << "📦 Inventory: Checking stock for " << quantity
             << "x " << product << "\n";
        cout << "📦 Inventory: Stock available ✓\n";
        return true;
    }

    void reduceStock(const string& product, int quantity) {
        cout << "📦 Inventory: Reducing stock by " << quantity << "\n";
    }
};

/**
 * Subsystem: Payment
 */
class PaymentProcessor {
public:
    bool processPayment(const string& cardNumber, double amount) {
        cout << "💳 Payment: Processing $" << amount
             << " on card ending " << cardNumber.substr(cardNumber.length() - 4) << "\n";
        cout << "💳 Payment: Transaction approved ✓\n";
        return true;
    }
};

/**
 * Subsystem: Shipping
 */
class ShippingService {
public:
    string scheduleShipping(const string& address, const string& product) {
        cout << "🚚 Shipping: Scheduling delivery to " << address << "\n";
        cout << "🚚 Shipping: Package prepared for " << product << "\n";
        string trackingNumber = "TRACK-" + to_string(rand() % 10000);
        cout << "🚚 Shipping: Tracking number: " << trackingNumber << "\n";
        return trackingNumber;
    }
};

/**
 * Subsystem: Notification
 */
class NotificationService {
public:
    void sendEmail(const string& email, const string& message) {
        cout << "📧 Notification: Sending email to " << email << "\n";
        cout << "📧 Message: " << message << "\n";
    }

    void sendSMS(const string& phone, const string& message) {
        cout << "📱 Notification: Sending SMS to " << phone << "\n";
        cout << "📱 Message: " << message << "\n";
    }
};

/**
 * FACADE: OrderFacade
 */
class OrderFacade {
private:
    shared_ptr<Inventory> inventory;
    shared_ptr<PaymentProcessor> payment;
    shared_ptr<ShippingService> shipping;
    shared_ptr<NotificationService> notification;

public:
    OrderFacade() {
        inventory = make_shared<Inventory>();
        payment = make_shared<PaymentProcessor>();
        shipping = make_shared<ShippingService>();
        notification = make_shared<NotificationService>();
    }

    bool placeOrder(
        const string& product,
        int quantity,
        const string& cardNumber,
        double amount,
        const string& shippingAddress,
        const string& email,
        const string& phone
        ) {
        cout << "\n🛒 Processing order...\n";
        cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";

        // Step 1: Check inventory
        if (!inventory->checkStock(product, quantity)) {
            cout << "❌ Order failed: Out of stock\n";
            return false;
        }

        // Step 2: Process payment
        if (!payment->processPayment(cardNumber, amount)) {
            cout << "❌ Order failed: Payment declined\n";
            return false;
        }

        // Step 3: Reduce inventory
        inventory->reduceStock(product, quantity);

        // Step 4: Schedule shipping
        string trackingNumber = shipping->scheduleShipping(shippingAddress, product);

        // Step 5: Send notifications
        notification->sendEmail(email, "Order confirmed! Tracking: " + trackingNumber);
        notification->sendSMS(phone, "Your order is on the way!");

        cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
        cout << "✅ Order placed successfully!\n";
        return true;
    }
};

// ============================================
// EXAMPLE 4: BANK OPERATIONS
// Complex banking system simplified
// ============================================

/**
 * Subsystem: Account Manager
 */
class AccountManager {
public:
    bool verifyAccount(const string& accountNumber) {
        cout << "🏦 Account: Verifying account " << accountNumber << "\n";
        return true;
    }

    double getBalance(const string& accountNumber) {
        cout << "🏦 Account: Fetching balance for " << accountNumber << "\n";
        return 5000.0;
    }

    void debit(const string& accountNumber, double amount) {
        cout << "🏦 Account: Debiting $" << amount << " from " << accountNumber << "\n";
    }

    void credit(const string& accountNumber, double amount) {
        cout << "🏦 Account: Crediting $" << amount << " to " << accountNumber << "\n";
    }
};

/**
 * Subsystem: Security Manager
 */
class SecurityManager {
public:
    bool authenticateUser(const string& username, const string& password) {
        cout << "🔒 Security: Authenticating user '" << username << "'\n";
        cout << "🔒 Security: Authentication successful ✓\n";
        return true;
    }

    bool authorize(const string& username, const string& operation) {
        cout << "🔒 Security: Authorizing '" << operation << "' for " << username << "\n";
        return true;
    }
};

/**
 * Subsystem: Transaction Logger
 */
class TransactionLogger {
public:
    void logTransaction(const string& type, double amount, const string& account) {
        cout << "📝 Logger: Recording " << type << " of $" << amount
             << " for account " << account << "\n";
    }
};

/**
 * FACADE: BankingFacade
 */
class BankingFacade {
private:
    shared_ptr<AccountManager> accountMgr;
    shared_ptr<SecurityManager> securityMgr;
    shared_ptr<TransactionLogger> logger;

public:
    BankingFacade() {
        accountMgr = make_shared<AccountManager>();
        securityMgr = make_shared<SecurityManager>();
        logger = make_shared<TransactionLogger>();
    }

    bool transfer(
        const string& username,
        const string& password,
        const string& fromAccount,
        const string& toAccount,
        double amount
        ) {
        cout << "\n💸 Processing transfer...\n";
        cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";

        // Step 1: Authenticate
        if (!securityMgr->authenticateUser(username, password)) {
            cout << "❌ Authentication failed\n";
            return false;
        }

        // Step 2: Authorize
        if (!securityMgr->authorize(username, "TRANSFER")) {
            cout << "❌ Not authorized for transfers\n";
            return false;
        }

        // Step 3: Verify accounts
        if (!accountMgr->verifyAccount(fromAccount) ||
            !accountMgr->verifyAccount(toAccount)) {
            cout << "❌ Invalid account\n";
            return false;
        }

        // Step 4: Check balance
        double balance = accountMgr->getBalance(fromAccount);
        if (balance < amount) {
            cout << "❌ Insufficient funds\n";
            return false;
        }

        // Step 5: Perform transfer
        accountMgr->debit(fromAccount, amount);
        accountMgr->credit(toAccount, amount);

        // Step 6: Log transaction
        logger->logTransaction("TRANSFER", amount, fromAccount);

        cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
        cout << "✅ Transfer completed successfully!\n";
        return true;
    }
};

// ============================================
// EXAMPLE 5: HOTEL BOOKING
// Simplified hotel reservation system
// ============================================

/**
 * Subsystem: Room Service
 */
class RoomService {
public:
    bool checkAvailability(const string& roomType, const string& date) {
        cout << "🏨 Room: Checking " << roomType << " availability for " << date << "\n";
        cout << "🏨 Room: Available ✓\n";
        return true;
    }

    void reserveRoom(const string& roomType, const string& date) {
        cout << "🏨 Room: Reserving " << roomType << " for " << date << "\n";
    }
};

/**
 * Subsystem: Restaurant
 */
class RestaurantService {
public:
    void bookTable(int guests, const string& time) {
        cout << "🍽️  Restaurant: Booking table for " << guests
             << " guests at " << time << "\n";
    }
};

/**
 * Subsystem: Event Manager
 */
class EventManager {
public:
    void bookConferenceRoom(const string& date, int hours) {
        cout << "📊 Events: Booking conference room for " << hours
             << " hours on " << date << "\n";
    }
};

/**
 * FACADE: HotelFacade
 */
class HotelFacade {
private:
    shared_ptr<RoomService> rooms;
    shared_ptr<RestaurantService> restaurant;
    shared_ptr<EventManager> events;

public:
    HotelFacade() {
        rooms = make_shared<RoomService>();
        restaurant = make_shared<RestaurantService>();
        events = make_shared<EventManager>();
    }

    bool bookBusinessPackage(
        const string& date,
        const string& roomType,
        int dinnerGuests,
        const string& dinnerTime,
        int conferenceHours
        ) {
        cout << "\n🏢 Booking Business Package...\n";
        cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";

        if (!rooms->checkAvailability(roomType, date)) {
            return false;
        }

        rooms->reserveRoom(roomType, date);
        restaurant->bookTable(dinnerGuests, dinnerTime);
        events->bookConferenceRoom(date, conferenceHours);

        cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
        cout << "✅ Business package booked successfully!\n";
        return true;
    }
};

// ============================================
// CLIENT CODE & DEMONSTRATIONS
// ============================================

void demonstrateHomeTheater() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: HOME THEATER           ║\n";
    cout << "╚════════════════════════════════════╝\n";

    // Create subsystem components
    auto dvd = make_shared<DVDPlayer>();
    auto projector = make_shared<Projector>();
    auto amp = make_shared<Amplifier>();
    auto lights = make_shared<TheaterLights>();
    auto screen = make_shared<Screen>();
    auto popper = make_shared<PopcornPopper>();

    // Create facade
    HomeTheaterFacade homeTheater(dvd, projector, amp, lights, screen, popper);

    // Simple interface - complex operations hidden!
    homeTheater.watchMovie("Inception");

    cout << "\n⏸️  ... Movie is playing ...\n";

    homeTheater.endMovie();

    cout << "\n💡 Notice: Client uses simple interface, complexity hidden!\n";
}

void demonstrateComputer() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: COMPUTER SYSTEM        ║\n";
    cout << "╚════════════════════════════════════╝\n";

    ComputerFacade computer;
    computer.start();

    cout << "\n💡 Notice: Complex boot sequence simplified to one method!\n";
}

void demonstrateOnlineShopping() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: ONLINE SHOPPING        ║\n";
    cout << "╚════════════════════════════════════╝\n";

    OrderFacade orderSystem;

    orderSystem.placeOrder(
        "Laptop",                    // product
        1,                           // quantity
        "4111111111111111",         // card
        999.99,                      // amount
        "123 Main St, City",        // address
        "user@example.com",         // email
        "+1234567890"               // phone
        );

    cout << "\n💡 Notice: Multiple subsystems coordinated through one call!\n";
}

void demonstrateBanking() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: BANKING SYSTEM         ║\n";
    cout << "╚════════════════════════════════════╝\n";

    BankingFacade bank;

    bank.transfer(
        "john_doe",              // username
        "password123",           // password
        "ACC-001",              // from account
        "ACC-002",              // to account
        500.0                   // amount
        );

    cout << "\n💡 Notice: Complex banking operations simplified!\n";
}

void demonstrateHotel() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: HOTEL BOOKING          ║\n";
    cout << "╚════════════════════════════════════╝\n";

    HotelFacade hotel;

    hotel.bookBusinessPackage(
        "2025-11-15",           // date
        "Executive Suite",      // room type
        4,                      // dinner guests
        "19:00",               // dinner time
        3                       // conference hours
        );

    cout << "\n💡 Notice: Package booking simplified into one call!\n";
}

// ============================================
// MAIN - DEMONSTRATING THE PATTERN
// ============================================

int main() {
    cout << "========================================\n";
    cout << "FACADE PATTERN DEMO\n";
    cout << "========================================\n";

    demonstrateHomeTheater();
    demonstrateComputer();
    demonstrateOnlineShopping();
    demonstrateBanking();
    demonstrateHotel();

    // ============================================
    // Benefits Summary
    // ============================================
    cout << "\n========================================\n";
    cout << "BENEFITS OF FACADE PATTERN\n";
    cout << "========================================\n";
    cout << "✅ Simplifies complex subsystem\n";
    cout << "✅ Reduces dependencies on subsystem\n";
    cout << "✅ Provides single entry point\n";
    cout << "✅ Decouples client from subsystem\n";
    cout << "✅ Promotes loose coupling\n";
    cout << "✅ Makes subsystem easier to use\n";
    cout << "✅ Layers the system architecture\n";

    cout << "\n========================================\n";
    cout << "WHEN TO USE FACADE\n";
    cout << "========================================\n";
    cout << "1. Provide simple interface to complex subsystem\n";
    cout << "2. Many dependencies between clients and subsystem\n";
    cout << "3. Want to layer subsystems\n";
    cout << "4. Decouple subsystem from clients\n";
    cout << "5. Wrap poorly designed APIs\n";
    cout << "6. Reduce complexity for clients\n";

    cout << "\n========================================\n";
    cout << "STRUCTURE\n";
    cout << "========================================\n";
    cout << "Client\n";
    cout << "  ↓ (uses simple interface)\n";
    cout << "Facade\n";
    cout << "  ↓ (delegates to)\n";
    cout << "Subsystem Classes\n";
    cout << "  • Component1\n";
    cout << "  • Component2\n";
    cout << "  • Component3\n";
    cout << "  • ...\n";

    cout << "\n========================================\n";
    cout << "FACADE vs OTHER PATTERNS\n";
    cout << "========================================\n";
    cout << "Facade:\n";
    cout << "  • Simplifies interface\n";
    cout << "  • Single entry point\n";
    cout << "  • Hides complexity\n";
    cout << "\nAdapter:\n";
    cout << "  • Converts interface\n";
    cout << "  • Makes compatible\n";
    cout << "  • Usually one-to-one\n";
    cout << "\nDecorator:\n";
    cout << "  • Adds responsibilities\n";
    cout << "  • Wraps objects\n";
    cout << "  • Same interface\n";
    cout << "\nProxy:\n";
    cout << "  • Controls access\n";
    cout << "  • Same interface\n";
    cout << "  • Adds control layer\n";

    cout << "\n========================================\n";
    cout << "WITHOUT FACADE (Complex)\n";
    cout << "========================================\n";
    cout << "// Client code without facade:\n";
    cout << "popper->on();\n";
    cout << "popper->pop();\n";
    cout << "lights->dim(10);\n";
    cout << "screen->down();\n";
    cout << "projector->on();\n";
    cout << "projector->wideScreenMode();\n";
    cout << "amp->on();\n";
    cout << "amp->setDVD();\n";
    cout << "amp->setSurroundSound();\n";
    cout << "amp->setVolume(5);\n";
    cout << "dvd->on();\n";
    cout << "dvd->play(movie);\n";
    cout << "// 12 lines of complex code!\n";

    cout << "\n========================================\n";
    cout << "WITH FACADE (Simple)\n";
    cout << "========================================\n";
    cout << "// Client code with facade:\n";
    cout << "homeTheater.watchMovie(\"Inception\");\n";
    cout << "// 1 line - simple!\n";

    cout << "\n========================================\n";
    cout << "KEY POINTS\n";
    cout << "========================================\n";
    cout << "• Facade doesn't prevent access to subsystem\n";
    cout << "• Clients can still use subsystem directly if needed\n";
    cout << "• Facade adds no new functionality\n";
    cout << "• Facade only provides convenient interface\n";
    cout << "• Can have multiple facades for same subsystem\n";
    cout << "• Facade can be coupled to multiple subsystems\n";

    cout << "\n========================================\n";
    cout << "REAL-WORLD EXAMPLES\n";
    cout << "========================================\n";
    cout << "• Home theater systems (watchMovie)\n";
    cout << "• Computer startup (complex boot sequence)\n";
    cout << "• Online shopping (order processing)\n";
    cout << "• Banking operations (money transfer)\n";
    cout << "• Compiler facade (compile, link, execute)\n";
    cout << "• Database access (connection pooling)\n";
    cout << "• Web frameworks (routing, middleware)\n";
    cout << "• Library APIs (simplified interfaces)\n";

    return 0;
}
