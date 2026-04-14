#include <iostream>
#include <string>
#include <memory>
#include <cmath>
using namespace std;

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
    virtual void play(const string& audioType, const string& fileName) = 0;
};

/**
 * Adaptee 1 - Advanced MP4 Player
 * Existing class with incompatible interface
 */
class MP4Player {
public:
    void playMP4(const string& fileName) {
        cout << "🎬 Playing MP4 file: " << fileName << "\n";
    }
};

/**
 * Adaptee 2 - VLC Player
 * Another existing class with incompatible interface
 */
class VLCPlayer {
public:
    void playVLC(const string& fileName) {
        cout << "🎥 Playing VLC file: " << fileName << "\n";
    }
};

/**
 * Adapter - Makes MP4Player and VLCPlayer work with MediaPlayer interface
 * This is an OBJECT ADAPTER (uses composition)
 */
class MediaAdapter : public MediaPlayer {
private:
    unique_ptr<MP4Player> mp4Player;
    unique_ptr<VLCPlayer> vlcPlayer;

public:
    MediaAdapter() {
        mp4Player = make_unique<MP4Player>();
        vlcPlayer = make_unique<VLCPlayer>();
    }

    void play(const string& audioType, const string& fileName) override {
        if (audioType == "mp4") {
            mp4Player->playMP4(fileName);
        } else if (audioType == "vlc") {
            vlcPlayer->playVLC(fileName);
        } else {
            cout << "❌ Invalid media type: " << audioType << "\n";
        }
    }
};

/**
 * Concrete implementation of Target interface
 * Can play standard audio formats directly
 */
class AudioPlayer : public MediaPlayer {
private:
    unique_ptr<MediaAdapter> mediaAdapter;

public:
    void play(const string& audioType, const string& fileName) override {
        // Built-in support for mp3
        if (audioType == "mp3") {
            cout << "🎵 Playing MP3 file: " << fileName << "\n";
        }
        // Use adapter for other formats
        else if (audioType == "mp4" || audioType == "vlc") {
            if (!mediaAdapter) {
                mediaAdapter = make_unique<MediaAdapter>();
            }
            mediaAdapter->play(audioType, fileName);
        }
        else {
            cout << "❌ Invalid media type: " << audioType << "\n";
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
        cout << "📐 Drawing legacy rectangle at (" << x << "," << y
             << ") with width=" << width << " height=" << height << "\n";
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
        cout << "⭕ Drawing circle at (" << x << "," << y
             << ") with radius=" << radius << "\n";
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
    unique_ptr<LegacyRectangle> legacyRect;

public:
    RectangleAdapter(int x, int y, int w, int h) {
        legacyRect = make_unique<LegacyRectangle>(x, y, w, h);
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

    string getSensorInfo() {
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
    virtual string getInfo() = 0;
};

/**
 * Temperature Adapter
 * Converts Fahrenheit to Celsius
 */
class TemperatureAdapter : public TemperatureSensor {
private:
    unique_ptr<FahrenheitSensor> fahrenheitSensor;

    double fahrenheitToCelsius(double f) {
        return (f - 32.0) * 5.0 / 9.0;
    }

public:
    TemperatureAdapter() {
        fahrenheitSensor = make_unique<FahrenheitSensor>();
    }

    double getTemperature() override {
        double fahrenheit = fahrenheitSensor->getTemperatureFahrenheit();
        double celsius = fahrenheitToCelsius(fahrenheit);

        cout << "🌡️  Adapting: " << fahrenheit << "°F → "
             << celsius << "°C\n";

        return celsius;
    }

    string getInfo() override {
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
    void sendPayment(const string& email, double amount) {
        cout << "💳 PayPal: Sending $" << amount << " to " << email << "\n";
    }

    bool verifyAccount(const string& email) {
        cout << "✓ PayPal: Verifying account " << email << "\n";
        return true;
    }
};

/**
 * Third-party Stripe API (Adaptee)
 * Different interface
 */
class StripeAPI {
public:
    void makePayment(const string& token, int amountInCents) {
        cout << "💳 Stripe: Processing payment of " << amountInCents
             << " cents with token " << token << "\n";
    }

    string createToken(const string& cardNumber) {
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
    virtual void processPayment(const string& account, double amount) = 0;
    virtual string getProcessorName() = 0;
};

/**
 * PayPal Adapter
 */
class PayPalAdapter : public PaymentProcessor {
private:
    unique_ptr<PayPalAPI> paypal;

public:
    PayPalAdapter() {
        paypal = make_unique<PayPalAPI>();
    }

    void processPayment(const string& account, double amount) override {
        if (paypal->verifyAccount(account)) {
            paypal->sendPayment(account, amount);
        }
    }

    string getProcessorName() override {
        return "PayPal Adapter";
    }
};

/**
 * Stripe Adapter
 */
class StripeAdapter : public PaymentProcessor {
private:
    unique_ptr<StripeAPI> stripe;

public:
    StripeAdapter() {
        stripe = make_unique<StripeAPI>();
    }

    void processPayment(const string& account, double amount) override {
        // Convert dollars to cents
        int amountInCents = static_cast<int>(amount * 100);

        // Create token from account (card number)
        string token = stripe->createToken(account);

        // Process payment
        stripe->makePayment(token, amountInCents);
    }

    string getProcessorName() override {
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
    void mysqlConnect(const string& host, int port) {
        cout << "🗄️  MySQL: Connecting to " << host << ":" << port << "\n";
    }

    void mysqlExecute(const string& query) {
        cout << "🗄️  MySQL: Executing query: " << query << "\n";
    }
};

/**
 * MongoDB Database (Adaptee)
 */
class MongoDB {
public:
    void connect(const string& uri) {
        cout << "🗄️  MongoDB: Connecting to " << uri << "\n";
    }

    void runCommand(const string& command) {
        cout << "🗄️  MongoDB: Running command: " << command << "\n";
    }
};

/**
 * Target Interface
 * Unified database interface for our application
 */
class Database {
public:
    virtual ~Database() {}
    virtual void connect(const string& connectionString) = 0;
    virtual void execute(const string& query) = 0;
    virtual string getDatabaseType() = 0;
};

/**
 * MySQL Adapter
 */
class MySQLAdapter : public Database {
private:
    unique_ptr<MySQLDatabase> mysql;

public:
    MySQLAdapter() {
        mysql = make_unique<MySQLDatabase>();
    }

    void connect(const string& connectionString) override {
        // Parse connection string (simplified)
        mysql->mysqlConnect("localhost", 3306);
    }

    void execute(const string& query) override {
        mysql->mysqlExecute(query);
    }

    string getDatabaseType() override {
        return "MySQL (Adapted)";
    }
};

/**
 * MongoDB Adapter
 */
class MongoDBAdapter : public Database {
private:
    unique_ptr<MongoDB> mongo;

public:
    MongoDBAdapter() {
        mongo = make_unique<MongoDB>();
    }

    void connect(const string& connectionString) override {
        mongo->connect(connectionString);
    }

    void execute(const string& query) override {
        // Convert SQL-like query to MongoDB command (simplified)
        string command = "{ find: '" + query + "' }";
        mongo->runCommand(command);
    }

    string getDatabaseType() override {
        return "MongoDB (Adapted)";
    }
};

// ============================================
// CLIENT CODE & DEMONSTRATIONS
// ============================================

void demonstrateMediaPlayer() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: MEDIA PLAYER           ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    AudioPlayer player;

    player.play("mp3", "song.mp3");
    player.play("mp4", "video.mp4");
    player.play("vlc", "movie.vlc");
    player.play("avi", "clip.avi");
}

void demonstrateShapes() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: SHAPE ADAPTER          ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    // Modern shapes
    Circle circle(10, 20, 5);
    circle.draw();
    cout << "Area: " << circle.getArea() << "\n\n";

    // Legacy rectangle adapted to modern interface
    RectangleAdapter rect(0, 0, 10, 5);
    rect.draw();
    cout << "Area: " << rect.getArea() << "\n";
}

void demonstrateTemperature() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: TEMPERATURE SENSOR     ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    TemperatureAdapter sensor;

    cout << "Sensor: " << sensor.getInfo() << "\n";
    double temp = sensor.getTemperature();
    cout << "Temperature: " << temp << "°C\n";

    if (temp > 37.0) {
        cout << "⚠️  Temperature above normal!\n";
    }
}

void demonstratePayment() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: PAYMENT GATEWAY        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Processing with PayPal ---\n";
    PayPalAdapter paypal;
    cout << "Processor: " << paypal.getProcessorName() << "\n";
    paypal.processPayment("user@example.com", 99.99);

    cout << "\n--- Processing with Stripe ---\n";
    StripeAdapter stripe;
    cout << "Processor: " << stripe.getProcessorName() << "\n";
    stripe.processPayment("4111111111111111", 149.99);
}

void demonstrateDatabase() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: DATABASE ADAPTER       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Using MySQL ---\n";
    MySQLAdapter mysql;
    cout << "Database: " << mysql.getDatabaseType() << "\n";
    mysql.connect("mysql://localhost:3306");
    mysql.execute("SELECT * FROM users");

    cout << "\n--- Using MongoDB ---\n";
    MongoDBAdapter mongo;
    cout << "Database: " << mongo.getDatabaseType() << "\n";
    mongo.connect("mongodb://localhost:27017");
    mongo.execute("users");
}

// ============================================
// MAIN - DEMONSTRATING THE PATTERN
// ============================================

int main() {
    cout << "========================================\n";
    cout << "ADAPTER PATTERN DEMO\n";
    cout << "========================================\n";

    demonstrateMediaPlayer();
    demonstrateShapes();
    demonstrateTemperature();
    demonstratePayment();
    demonstrateDatabase();

    // ============================================
    // Benefits Summary
    // ============================================
    cout << "\n========================================\n";
    cout << "BENEFITS OF ADAPTER PATTERN\n";
    cout << "========================================\n";
    cout << "✅ Single Responsibility: Separates interface conversion\n";
    cout << "✅ Open/Closed: Add new adapters without changing existing code\n";
    cout << "✅ Works with legacy code without modification\n";
    cout << "✅ Integrates third-party libraries easily\n";
    cout << "✅ Makes incompatible interfaces work together\n";
    cout << "✅ Promotes code reusability\n";

    cout << "\n========================================\n";
    cout << "WHEN TO USE ADAPTER\n";
    cout << "========================================\n";
    cout << "1. Use existing class with incompatible interface\n";
    cout << "2. Create reusable class for unrelated classes\n";
    cout << "3. Work with legacy code you can't modify\n";
    cout << "4. Integrate third-party libraries\n";
    cout << "5. Need uniform interface for similar classes\n";

    cout << "\n========================================\n";
    cout << "OBJECT ADAPTER vs CLASS ADAPTER\n";
    cout << "========================================\n";
    cout << "Object Adapter (Composition):\n";
    cout << "  • Uses composition\n";
    cout << "  • More flexible (can adapt multiple classes)\n";
    cout << "  • Preferred in C++ (no multiple inheritance issues)\n";
    cout << "  • Example: All examples above\n";
    cout << "\nClass Adapter (Inheritance):\n";
    cout << "  • Uses multiple inheritance\n";
    cout << "  • Less flexible\n";
    cout << "  • Can override adaptee behavior\n";
    cout << "  • Not commonly used in C++\n";

    cout << "\n========================================\n";
    cout << "REAL-WORLD EXAMPLES\n";
    cout << "========================================\n";
    cout << "• Power plug adapters (US to EU)\n";
    cout << "• Card readers (SD card to USB)\n";
    cout << "• Media players (different formats)\n";
    cout << "• Database drivers (ODBC, JDBC)\n";
    cout << "• Payment gateways (PayPal, Stripe)\n";
    cout << "• Legacy system integration\n";
    cout << "• Third-party API wrappers\n";
    cout << "• Temperature converters\n";

    return 0;
}
