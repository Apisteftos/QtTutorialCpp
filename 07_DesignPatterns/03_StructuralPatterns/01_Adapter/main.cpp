#include <string>
#include <memory>
#include <cmath>
#include <print>

// ============================================
// EXAMPLE 1: MEDIA PLAYER ADAPTER
// Classic adapter pattern example
// ============================================

/**
 * Target Interface
 * This is what the client expects
 */
class MediaPlayer {
public:
    virtual ~MediaPlayer() {}
    virtual void play(const std::string& audioType, const std::string& fileName) = 0;
};

/**
 * Adaptee 1 - Advanced MP4 Player
 * Existing class with incompatible interface
 */
class MP4Player {
public:
    void playMP4(const std::string& fileName) {
        std::println("🎬 Playing MP4 file: {}", fileName);
    }
};

/**
 * Adaptee 2 - VLC Player
 * Another existing class with incompatible interface
 */
class VLCPlayer {
public:
    void playVLC(const std::string& fileName) {
        std::println("🎥 Playing VLC file: {}", fileName);
    }
};

/**
 * Adapter - Makes MP4Player and VLCPlayer work with MediaPlayer interface
 * This is an OBJECT ADAPTER (uses composition)
 */
class MediaAdapter : public MediaPlayer {
private:
    std::unique_ptr<MP4Player> mp4Player;
    std::unique_ptr<VLCPlayer> vlcPlayer;

public:
    MediaAdapter() {
        mp4Player = std::make_unique<MP4Player>();
        vlcPlayer = std::make_unique<VLCPlayer>();
    }

    void play(const std::string& audioType, const std::string& fileName) override {
        if (audioType == "mp4") {
            mp4Player->playMP4(fileName);
        } else if (audioType == "vlc") {
            vlcPlayer->playVLC(fileName);
        } else {
            std::println("❌ Invalid media type: {}", audioType);
        }
    }
};

/**
 * Concrete implementation of Target interface
 * Can play standard audio formats directly
 */
class AudioPlayer : public MediaPlayer {
private:
    std::unique_ptr<MediaAdapter> mediaAdapter;

public:
    void play(const std::string& audioType, const std::string& fileName) override {
        // Built-in support for mp3
        if (audioType == "mp3") {
            std::println("🎵 Playing MP3 file: {}", fileName);
        }
        // Use adapter for other formats
        else if (audioType == "mp4" || audioType == "vlc") {
            if (!mediaAdapter) {
                mediaAdapter = std::make_unique<MediaAdapter>();
            }
            mediaAdapter->play(audioType, fileName);
        }
        else {
            std::println("❌ Invalid media type: {}", audioType);
        }
    }
};

// ============================================
// EXAMPLE 2: SHAPE ADAPTER (GEOMETRIC)
// Adapting incompatible shape interfaces
// ============================================

/**
 * Legacy Rectangle class (Adaptee)
 * Uses position and dimensions
 */
class LegacyRectangle {
private:
    int x, y, width, height;

public:
    LegacyRectangle(int x, int y, int w, int h)
        : x(x), y(y), width(w), height(h) {}

    void draw() {
        std::println("📐 Drawing legacy rectangle at ({},{}) with width={} height={}", x, y, width, height);
    }

    int getX() const { return x; }
    int getY() const { return y; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
};

/**
 * Modern Shape interface (Target)
 * Uses coordinates for corners
 */
class Shape {
public:
    virtual ~Shape() {}
    virtual void draw() = 0;
    virtual double getArea() = 0;
};

/**
 * Modern Circle implementation
 */
class Circle : public Shape {
private:
    int x, y, radius;

public:
    Circle(int x, int y, int r) : x(x), y(y), radius(r) {}

    void draw() override {
        std::println("⭕ Drawing circle at ({},{}) with radius={}", x, y, radius);
    }

    double getArea() override {
        return 3.14159 * radius * radius;
    }
};

/**
 * Rectangle Adapter
 * Adapts LegacyRectangle to work with modern Shape interface
 */
class RectangleAdapter : public Shape {
private:
    std::unique_ptr<LegacyRectangle> legacyRect;

public:
    RectangleAdapter(int x, int y, int w, int h) {
        legacyRect = std::make_unique<LegacyRectangle>(x, y, w, h);
    }

    void draw() override {
        legacyRect->draw();
    }

    double getArea() override {
        return legacyRect->getWidth() * legacyRect->getHeight();
    }
};

// ============================================
// EXAMPLE 3: TEMPERATURE SENSOR ADAPTER
// Real-world IoT scenario
// ============================================

/**
 * Old Temperature Sensor (Adaptee)
 * Returns temperature in Fahrenheit
 */
class FahrenheitSensor {
public:
    double getTemperatureFahrenheit() {
        // Simulate reading from sensor
        return 98.6;  // Body temperature in Fahrenheit
    }

    std::string getSensorInfo() {
        return "Legacy Fahrenheit Sensor v1.0";
    }
};

/**
 * Target Interface
 * System expects temperature in Celsius
 */
class TemperatureSensor {
public:
    virtual ~TemperatureSensor() {}
    virtual double getTemperature() = 0;  // Returns Celsius
    virtual std::string getInfo() = 0;
};

/**
 * Temperature Adapter
 * Converts Fahrenheit to Celsius
 */
class TemperatureAdapter : public TemperatureSensor {
private:
    std::unique_ptr<FahrenheitSensor> fahrenheitSensor;

    double fahrenheitToCelsius(double f) {
        return (f - 32.0) * 5.0 / 9.0;
    }

public:
    TemperatureAdapter() {
        fahrenheitSensor = std::make_unique<FahrenheitSensor>();
    }

    double getTemperature() override {
        double fahrenheit = fahrenheitSensor->getTemperatureFahrenheit();
        double celsius = fahrenheitToCelsius(fahrenheit);

        std::println("🌡️  Adapting: {}°F → {}°C", fahrenheit, celsius);

        return celsius;
    }

    std::string getInfo() override {
        return "Adapted: " + fahrenheitSensor->getSensorInfo();
    }
};

// ============================================
// EXAMPLE 4: PAYMENT GATEWAY ADAPTER
// Third-party API integration
// ============================================

/**
 * Third-party PayPal API (Adaptee)
 * Has its own interface
 */
class PayPalAPI {
public:
    void sendPayment(const std::string& email, double amount) {
        std::println("💳 PayPal: Sending ${} to {}", amount, email);
    }

    bool verifyAccount(const std::string& email) {
        std::println("✓ PayPal: Verifying account {}", email);
        return true;
    }
};

/**
 * Third-party Stripe API (Adaptee)
 * Different interface
 */
class StripeAPI {
public:
    void makePayment(const std::string& token, int amountInCents) {
        std::println("💳 Stripe: Processing payment of {} cents with token {}", amountInCents, token);
    }

    std::string createToken(const std::string& cardNumber) {
        return "tok_" + cardNumber.substr(cardNumber.length() - 4);
    }
};

/**
 * Target Interface
 * What our application expects
 */
class PaymentProcessor {
public:
    virtual ~PaymentProcessor() {}
    virtual void processPayment(const std::string& account, double amount) = 0;
    virtual std::string getProcessorName() = 0;
};

/**
 * PayPal Adapter
 */
class PayPalAdapter : public PaymentProcessor {
private:
    std::unique_ptr<PayPalAPI> paypal;

public:
    PayPalAdapter() {
        paypal = std::make_unique<PayPalAPI>();
    }

    void processPayment(const std::string& account, double amount) override {
        if (paypal->verifyAccount(account)) {
            paypal->sendPayment(account, amount);
        }
    }

    std::string getProcessorName() override {
        return "PayPal Adapter";
    }
};

/**
 * Stripe Adapter
 */
class StripeAdapter : public PaymentProcessor {
private:
    std::unique_ptr<StripeAPI> stripe;

public:
    StripeAdapter() {
        stripe = std::make_unique<StripeAPI>();
    }

    void processPayment(const std::string& account, double amount) override {
        // Convert dollars to cents
        int amountInCents = static_cast<int>(amount * 100);

        // Create token from account (card number)
        std::string token = stripe->createToken(account);

        // Process payment
        stripe->makePayment(token, amountInCents);
    }

    std::string getProcessorName() override {
        return "Stripe Adapter";
    }
};

// ============================================
// EXAMPLE 5: DATABASE ADAPTER
// Adapting different database APIs
// ============================================

/**
 * MySQL Database (Adaptee)
 */
class MySQLDatabase {
public:
    void mysqlConnect(const std::string& host, int port) {
        std::println("🗄️  MySQL: Connecting to {}:{}", host, port);
    }

    void mysqlExecute(const std::string& query) {
        std::println("🗄️  MySQL: Executing query: {}", query);
    }
};

/**
 * MongoDB Database (Adaptee)
 */
class MongoDB {
public:
    void connect(const std::string& uri) {
        std::println("🗄️  MongoDB: Connecting to {}", uri);
    }

    void runCommand(const std::string& command) {
        std::println("🗄️  MongoDB: Running command: {}", command);
    }
};

/**
 * Target Interface
 * Unified database interface for our application
 */
class Database {
public:
    virtual ~Database() {}
    virtual void connect(const std::string& connectionString) = 0;
    virtual void execute(const std::string& query) = 0;
    virtual std::string getDatabaseType() = 0;
};

/**
 * MySQL Adapter
 */
class MySQLAdapter : public Database {
private:
    std::unique_ptr<MySQLDatabase> mysql;

public:
    MySQLAdapter() {
        mysql = std::make_unique<MySQLDatabase>();
    }

    void connect(const std::string& connectionString) override {
        // Parse connection string (simplified)
        mysql->mysqlConnect("localhost", 3306);
    }

    void execute(const std::string& query) override {
        mysql->mysqlExecute(query);
    }

    std::string getDatabaseType() override {
        return "MySQL (Adapted)";
    }
};

/**
 * MongoDB Adapter
 */
class MongoDBAdapter : public Database {
private:
    std::unique_ptr<MongoDB> mongo;

public:
    MongoDBAdapter() {
        mongo = std::make_unique<MongoDB>();
    }

    void connect(const std::string& connectionString) override {
        mongo->connect(connectionString);
    }

    void execute(const std::string& query) override {
        // Convert SQL-like query to MongoDB command (simplified)
        std::string command = "{ find: '" + query + "' }";
        mongo->runCommand(command);
    }

    std::string getDatabaseType() override {
        return "MongoDB (Adapted)";
    }
};

// ============================================
// CLIENT CODE & DEMONSTRATIONS
// ============================================

void demonstrateMediaPlayer() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: MEDIA PLAYER           ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    AudioPlayer player;

    player.play("mp3", "song.mp3");
    player.play("mp4", "video.mp4");
    player.play("vlc", "movie.vlc");
    player.play("avi", "clip.avi");
}

void demonstrateShapes() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: SHAPE ADAPTER          ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    // Modern shapes
    Circle circle(10, 20, 5);
    circle.draw();
    std::println("Area: {}", circle.getArea());
    std::println("");

    // Legacy rectangle adapted to modern interface
    RectangleAdapter rect(0, 0, 10, 5);
    rect.draw();
    std::println("Area: {}", rect.getArea());
}

void demonstrateTemperature() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: TEMPERATURE SENSOR     ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    TemperatureAdapter sensor;

    std::println("Sensor: {}", sensor.getInfo());
    double temp = sensor.getTemperature();
    std::println("Temperature: {}°C", temp);

    if (temp > 37.0) {
        std::println("⚠️  Temperature above normal!");
    }
}

void demonstratePayment() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: PAYMENT GATEWAY        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Processing with PayPal ---");
    PayPalAdapter paypal;
    std::println("Processor: {}", paypal.getProcessorName());
    paypal.processPayment("user@example.com", 99.99);

    std::println("");
    std::println("--- Processing with Stripe ---");
    StripeAdapter stripe;
    std::println("Processor: {}", stripe.getProcessorName());
    stripe.processPayment("4111111111111111", 149.99);
}

void demonstrateDatabase() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 5: DATABASE ADAPTER       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Using MySQL ---");
    MySQLAdapter mysql;
    std::println("Database: {}", mysql.getDatabaseType());
    mysql.connect("mysql://localhost:3306");
    mysql.execute("SELECT * FROM users");

    std::println("");
    std::println("--- Using MongoDB ---");
    MongoDBAdapter mongo;
    std::println("Database: {}", mongo.getDatabaseType());
    mongo.connect("mongodb://localhost:27017");
    mongo.execute("users");
}

// ============================================
// MAIN - DEMONSTRATING THE PATTERN
// ============================================

int main() {
    std::println("========================================");
    std::println("ADAPTER PATTERN DEMO");
    std::println("========================================");

    demonstrateMediaPlayer();
    demonstrateShapes();
    demonstrateTemperature();
    demonstratePayment();
    demonstrateDatabase();

    return 0;
}
