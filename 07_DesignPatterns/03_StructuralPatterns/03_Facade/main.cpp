#include <string>
#include <memory>
#include <vector>
#include <print>

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
        std::println("📀 DVD Player: Turning ON");
    }

    void off() {
        std::println("📀 DVD Player: Turning OFF");
    }

    void play(const std::string& movie) {
        std::println("📀 DVD Player: Playing '{}'", movie);
    }

    void stop() {
        std::println("📀 DVD Player: Stopped");
    }

    void eject() {
        std::println("📀 DVD Player: Ejecting disc");
    }
};

/**
 * Subsystem Component 2: Projector
 */
class Projector {
public:
    void on() {
        std::println("📽️  Projector: Turning ON");
    }

    void off() {
        std::println("📽️  Projector: Turning OFF");
    }

    void wideScreenMode() {
        std::println("📽️  Projector: Setting wide screen mode (16:9)");
    }

    void tvMode() {
        std::println("📽️  Projector: Setting TV mode (4:3)");
    }
};

/**
 * Subsystem Component 3: Amplifier
 */
class Amplifier {
public:
    void on() {
        std::println("🔊 Amplifier: Turning ON");
    }

    void off() {
        std::println("🔊 Amplifier: Turning OFF");
    }

    void setDVD() {
        std::println("🔊 Amplifier: Setting DVD as input source");
    }

    void setSurroundSound() {
        std::println("🔊 Amplifier: Setting surround sound mode (5.1)");
    }

    void setVolume(int level) {
        std::println("🔊 Amplifier: Setting volume to {}", level);
    }
};

/**
 * Subsystem Component 4: Lights
 */
class TheaterLights {
public:
    void dim(int level) {
        std::println("💡 Lights: Dimming to {}%", level);
    }

    void on() {
        std::println("💡 Lights: Turning ON to 100%");
    }
};

/**
 * Subsystem Component 5: Screen
 */
class Screen {
public:
    void down() {
        std::println("🎬 Screen: Going DOWN");
    }

    void up() {
        std::println("🎬 Screen: Going UP");
    }
};

/**
 * Subsystem Component 6: Popcorn Maker
 */
class PopcornPopper {
public:
    void on() {
        std::println("🍿 Popcorn Maker: Turning ON");
    }

    void off() {
        std::println("🍿 Popcorn Maker: Turning OFF");
    }

    void pop() {
        std::println("🍿 Popcorn Maker: Popping popcorn!");
    }
};

/**
 * FACADE: HomeTheaterFacade
 * Provides simple interface to complex home theater subsystem
 */
class HomeTheaterFacade {
private:
    std::shared_ptr<DVDPlayer> dvd;
    std::shared_ptr<Projector> projector;
    std::shared_ptr<Amplifier> amp;
    std::shared_ptr<TheaterLights> lights;
    std::shared_ptr<Screen> screen;
    std::shared_ptr<PopcornPopper> popper;

public:
    HomeTheaterFacade(
        std::shared_ptr<DVDPlayer> d,
        std::shared_ptr<Projector> p,
        std::shared_ptr<Amplifier> a,
        std::shared_ptr<TheaterLights> l,
        std::shared_ptr<Screen> s,
        std::shared_ptr<PopcornPopper> pop
        ) : dvd(d), projector(p), amp(a), lights(l), screen(s), popper(pop) {}

    void watchMovie(const std::string& movie) {
        std::println("");
        std::println("🎥 Get ready to watch a movie...");
        std::println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");

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

        std::println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
        std::println("🎬 Movie is now playing! Enjoy!");
    }

    void endMovie() {
        std::println("");
        std::println("🛑 Shutting down home theater...");
        std::println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");

        popper->off();
        lights->on();
        screen->up();
        projector->off();
        amp->off();
        dvd->stop();
        dvd->eject();
        dvd->off();

        std::println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
        std::println("✅ Home theater shut down complete!");
    }

    void listenToRadio(double frequency) {
        std::println("");
        std::println("📻 Preparing to listen to radio...");
        std::println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");

        amp->on();
        amp->setVolume(3);
        std::println("📻 Tuning to {} FM", frequency);

        std::println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
        std::println("🎵 Radio is now playing!");
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
        std::println("🔷 CPU: Freezing...");
    }

    void jump(long position) {
        std::println("🔷 CPU: Jumping to position {}", position);
    }

    void execute() {
        std::println("🔷 CPU: Executing instructions");
    }
};

/**
 * Subsystem: Memory
 */
class Memory {
public:
    void load(long position, const std::string& data) {
        std::println("💾 Memory: Loading '{}' at position {}", data, position);
    }
};

/**
 * Subsystem: HardDrive
 */
class HardDrive {
public:
    std::string read(long lba, int size) {
        std::println("💿 HardDrive: Reading {} bytes from sector {}", size, lba);
        return "BOOT_DATA";
    }
};

/**
 * FACADE: ComputerFacade
 */
class ComputerFacade {
private:
    std::shared_ptr<CPU> cpu;
    std::shared_ptr<Memory> memory;
    std::shared_ptr<HardDrive> hd;

    const long BOOT_ADDRESS = 0x0000;
    const long BOOT_SECTOR = 0;
    const int SECTOR_SIZE = 512;

public:
    ComputerFacade() {
        cpu = std::make_shared<CPU>();
        memory = std::make_shared<Memory>();
        hd = std::make_shared<HardDrive>();
    }

    void start() {
        std::println("");
        std::println("💻 Starting computer...");
        std::println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");

        cpu->freeze();
        std::string bootData = hd->read(BOOT_SECTOR, SECTOR_SIZE);
        memory->load(BOOT_ADDRESS, bootData);
        cpu->jump(BOOT_ADDRESS);
        cpu->execute();

        std::println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
        std::println("✅ Computer started successfully!");
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
    bool checkStock(const std::string& product, int quantity) {
        std::println("📦 Inventory: Checking stock for {}x {}", quantity, product);
        std::println("📦 Inventory: Stock available ✓");
        return true;
    }

    void reduceStock(const std::string& product, int quantity) {
        std::println("📦 Inventory: Reducing stock by {}", quantity);
    }
};

/**
 * Subsystem: Payment
 */
class PaymentProcessor {
public:
    bool processPayment(const std::string& cardNumber, double amount) {
        std::println("💳 Payment: Processing ${} on card ending {}",
            amount, cardNumber.substr(cardNumber.length() - 4));
        std::println("💳 Payment: Transaction approved ✓");
        return true;
    }
};

/**
 * Subsystem: Shipping
 */
class ShippingService {
public:
    std::string scheduleShipping(const std::string& address, const std::string& product) {
        std::println("🚚 Shipping: Scheduling delivery to {}", address);
        std::println("🚚 Shipping: Package prepared for {}", product);
        std::string trackingNumber = "TRACK-" + std::to_string(rand() % 10000);
        std::println("🚚 Shipping: Tracking number: {}", trackingNumber);
        return trackingNumber;
    }
};

/**
 * Subsystem: Notification
 */
class NotificationService {
public:
    void sendEmail(const std::string& email, const std::string& message) {
        std::println("📧 Notification: Sending email to {}", email);
        std::println("📧 Message: {}", message);
    }

    void sendSMS(const std::string& phone, const std::string& message) {
        std::println("📱 Notification: Sending SMS to {}", phone);
        std::println("📱 Message: {}", message);
    }
};

/**
 * FACADE: OrderFacade
 */
class OrderFacade {
private:
    std::shared_ptr<Inventory> inventory;
    std::shared_ptr<PaymentProcessor> payment;
    std::shared_ptr<ShippingService> shipping;
    std::shared_ptr<NotificationService> notification;

public:
    OrderFacade() {
        inventory = std::make_shared<Inventory>();
        payment = std::make_shared<PaymentProcessor>();
        shipping = std::make_shared<ShippingService>();
        notification = std::make_shared<NotificationService>();
    }

    bool placeOrder(
        const std::string& product,
        int quantity,
        const std::string& cardNumber,
        double amount,
        const std::string& shippingAddress,
        const std::string& email,
        const std::string& phone
        ) {
        std::println("");
        std::println("🛒 Processing order...");
        std::println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");

        // Step 1: Check inventory
        if (!inventory->checkStock(product, quantity)) {
            std::println("❌ Order failed: Out of stock");
            return false;
        }

        // Step 2: Process payment
        if (!payment->processPayment(cardNumber, amount)) {
            std::println("❌ Order failed: Payment declined");
            return false;
        }

        // Step 3: Reduce inventory
        inventory->reduceStock(product, quantity);

        // Step 4: Schedule shipping
        std::string trackingNumber = shipping->scheduleShipping(shippingAddress, product);

        // Step 5: Send notifications
        notification->sendEmail(email, "Order confirmed! Tracking: " + trackingNumber);
        notification->sendSMS(phone, "Your order is on the way!");

        std::println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
        std::println("✅ Order placed successfully!");
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
    bool verifyAccount(const std::string& accountNumber) {
        std::println("🏦 Account: Verifying account {}", accountNumber);
        return true;
    }

    double getBalance(const std::string& accountNumber) {
        std::println("🏦 Account: Fetching balance for {}", accountNumber);
        return 5000.0;
    }

    void debit(const std::string& accountNumber, double amount) {
        std::println("🏦 Account: Debiting ${} from {}", amount, accountNumber);
    }

    void credit(const std::string& accountNumber, double amount) {
        std::println("🏦 Account: Crediting ${} to {}", amount, accountNumber);
    }
};

/**
 * Subsystem: Security Manager
 */
class SecurityManager {
public:
    bool authenticateUser(const std::string& username, const std::string& password) {
        std::println("🔒 Security: Authenticating user '{}'", username);
        std::println("🔒 Security: Authentication successful ✓");
        return true;
    }

    bool authorize(const std::string& username, const std::string& operation) {
        std::println("🔒 Security: Authorizing '{}' for {}", operation, username);
        return true;
    }
};

/**
 * Subsystem: Transaction Logger
 */
class TransactionLogger {
public:
    void logTransaction(const std::string& type, double amount, const std::string& account) {
        std::println("📝 Logger: Recording {} of ${} for account {}", type, amount, account);
    }
};

/**
 * FACADE: BankingFacade
 */
class BankingFacade {
private:
    std::shared_ptr<AccountManager> accountMgr;
    std::shared_ptr<SecurityManager> securityMgr;
    std::shared_ptr<TransactionLogger> logger;

public:
    BankingFacade() {
        accountMgr = std::make_shared<AccountManager>();
        securityMgr = std::make_shared<SecurityManager>();
        logger = std::make_shared<TransactionLogger>();
    }

    bool transfer(
        const std::string& username,
        const std::string& password,
        const std::string& fromAccount,
        const std::string& toAccount,
        double amount
        ) {
        std::println("");
        std::println("💸 Processing transfer...");
        std::println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");

        // Step 1: Authenticate
        if (!securityMgr->authenticateUser(username, password)) {
            std::println("❌ Authentication failed");
            return false;
        }

        // Step 2: Authorize
        if (!securityMgr->authorize(username, "TRANSFER")) {
            std::println("❌ Not authorized for transfers");
            return false;
        }

        // Step 3: Verify accounts
        if (!accountMgr->verifyAccount(fromAccount) ||
            !accountMgr->verifyAccount(toAccount)) {
            std::println("❌ Invalid account");
            return false;
        }

        // Step 4: Check balance
        double balance = accountMgr->getBalance(fromAccount);
        if (balance < amount) {
            std::println("❌ Insufficient funds");
            return false;
        }

        // Step 5: Perform transfer
        accountMgr->debit(fromAccount, amount);
        accountMgr->credit(toAccount, amount);

        // Step 6: Log transaction
        logger->logTransaction("TRANSFER", amount, fromAccount);

        std::println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
        std::println("✅ Transfer completed successfully!");
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
    bool checkAvailability(const std::string& roomType, const std::string& date) {
        std::println("🏨 Room: Checking {} availability for {}", roomType, date);
        std::println("🏨 Room: Available ✓");
        return true;
    }

    void reserveRoom(const std::string& roomType, const std::string& date) {
        std::println("🏨 Room: Reserving {} for {}", roomType, date);
    }
};

/**
 * Subsystem: Restaurant
 */
class RestaurantService {
public:
    void bookTable(int guests, const std::string& time) {
        std::println("🍽️  Restaurant: Booking table for {} guests at {}", guests, time);
    }
};

/**
 * Subsystem: Event Manager
 */
class EventManager {
public:
    void bookConferenceRoom(const std::string& date, int hours) {
        std::println("📊 Events: Booking conference room for {} hours on {}", hours, date);
    }
};

/**
 * FACADE: HotelFacade
 */
class HotelFacade {
private:
    std::shared_ptr<RoomService> rooms;
    std::shared_ptr<RestaurantService> restaurant;
    std::shared_ptr<EventManager> events;

public:
    HotelFacade() {
        rooms = std::make_shared<RoomService>();
        restaurant = std::make_shared<RestaurantService>();
        events = std::make_shared<EventManager>();
    }

    bool bookBusinessPackage(
        const std::string& date,
        const std::string& roomType,
        int dinnerGuests,
        const std::string& dinnerTime,
        int conferenceHours
        ) {
        std::println("");
        std::println("🏢 Booking Business Package...");
        std::println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");

        if (!rooms->checkAvailability(roomType, date)) {
            return false;
        }

        rooms->reserveRoom(roomType, date);
        restaurant->bookTable(dinnerGuests, dinnerTime);
        events->bookConferenceRoom(date, conferenceHours);

        std::println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
        std::println("✅ Business package booked successfully!");
        return true;
    }
};

// ============================================
// CLIENT CODE & DEMONSTRATIONS
// ============================================

void demonstrateHomeTheater() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: HOME THEATER           ║");
    std::println("╚════════════════════════════════════╝");

    // Create subsystem components
    auto dvd = std::make_shared<DVDPlayer>();
    auto projector = std::make_shared<Projector>();
    auto amp = std::make_shared<Amplifier>();
    auto lights = std::make_shared<TheaterLights>();
    auto screen = std::make_shared<Screen>();
    auto popper = std::make_shared<PopcornPopper>();

    // Create facade
    HomeTheaterFacade homeTheater(dvd, projector, amp, lights, screen, popper);

    // Simple interface - complex operations hidden!
    homeTheater.watchMovie("Inception");

    std::println("");
    std::println("⏸️  ... Movie is playing ...");

    homeTheater.endMovie();

    std::println("");
    std::println("💡 Notice: Client uses simple interface, complexity hidden!");
}

void demonstrateComputer() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: COMPUTER SYSTEM        ║");
    std::println("╚════════════════════════════════════╝");

    ComputerFacade computer;
    computer.start();

    std::println("");
    std::println("💡 Notice: Complex boot sequence simplified to one method!");
}

void demonstrateOnlineShopping() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: ONLINE SHOPPING        ║");
    std::println("╚════════════════════════════════════╝");

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

    std::println("");
    std::println("💡 Notice: Multiple subsystems coordinated through one call!");
}

void demonstrateBanking() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: BANKING SYSTEM         ║");
    std::println("╚════════════════════════════════════╝");

    BankingFacade bank;

    bank.transfer(
        "john_doe",              // username
        "password123",           // password
        "ACC-001",              // from account
        "ACC-002",              // to account
        500.0                   // amount
        );

    std::println("");
    std::println("💡 Notice: Complex banking operations simplified!");
}

void demonstrateHotel() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 5: HOTEL BOOKING          ║");
    std::println("╚════════════════════════════════════╝");

    HotelFacade hotel;

    hotel.bookBusinessPackage(
        "2025-11-15",           // date
        "Executive Suite",      // room type
        4,                      // dinner guests
        "19:00",               // dinner time
        3                       // conference hours
        );

    std::println("");
    std::println("💡 Notice: Package booking simplified into one call!");
}

// ============================================
// MAIN - DEMONSTRATING THE PATTERN
// ============================================

int main() {
    std::println("========================================");
    std::println("FACADE PATTERN DEMO");
    std::println("========================================");

    demonstrateHomeTheater();
    demonstrateComputer();
    demonstrateOnlineShopping();
    demonstrateBanking();
    demonstrateHotel();

    return 0;
}
