#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include <print>

// ============================================
// EXAMPLE 1: COFFEE SHOP (BEVERAGES)
// Classic decorator pattern example
// ============================================

/**
 * Component Interface
 * Base interface for both concrete components and decorators
 */
class Beverage {
public:
    virtual ~Beverage() {}
    virtual std::string getDescription() const = 0;
    virtual double cost() const = 0;
};

/**
 * Concrete Component 1: Espresso
 */
class Espresso : public Beverage {
public:
    std::string getDescription() const override {
        return "Espresso";
    }

    double cost() const override {
        return 1.99;
    }
};

/**
 * Concrete Component 2: HouseBlend
 */
class HouseBlend : public Beverage {
public:
    std::string getDescription() const override {
        return "House Blend Coffee";
    }

    double cost() const override {
        return 0.89;
    }
};

/**
 * Concrete Component 3: DarkRoast
 */
class DarkRoast : public Beverage {
public:
    std::string getDescription() const override {
        return "Dark Roast Coffee";
    }

    double cost() const override {
        return 0.99;
    }
};

/**
 * Concrete Component 4: Decaf
 */
class Decaf : public Beverage {
public:
    std::string getDescription() const override {
        return "Decaf Coffee";
    }

    double cost() const override {
        return 1.05;
    }
};

/**
 * Base Decorator
 * Wraps a Beverage component
 */
class CondimentDecorator : public Beverage {
protected:
    std::shared_ptr<Beverage> beverage;

public:
    CondimentDecorator(std::shared_ptr<Beverage> bev) : beverage(bev) {}
    virtual ~CondimentDecorator() {}
};

/**
 * Concrete Decorator 1: Milk
 */
class Milk : public CondimentDecorator {
public:
    Milk(std::shared_ptr<Beverage> bev) : CondimentDecorator(bev) {}

    std::string getDescription() const override {
        return beverage->getDescription() + ", Milk";
    }

    double cost() const override {
        return beverage->cost() + 0.10;
    }
};

/**
 * Concrete Decorator 2: Mocha
 */
class Mocha : public CondimentDecorator {
public:
    Mocha(std::shared_ptr<Beverage> bev) : CondimentDecorator(bev) {}

    std::string getDescription() const override {
        return beverage->getDescription() + ", Mocha";
    }

    double cost() const override {
        return beverage->cost() + 0.20;
    }
};

/**
 * Concrete Decorator 3: Soy
 */
class Soy : public CondimentDecorator {
public:
    Soy(std::shared_ptr<Beverage> bev) : CondimentDecorator(bev) {}

    std::string getDescription() const override {
        return beverage->getDescription() + ", Soy";
    }

    double cost() const override {
        return beverage->cost() + 0.15;
    }
};

/**
 * Concrete Decorator 4: Whip
 */
class Whip : public CondimentDecorator {
public:
    Whip(std::shared_ptr<Beverage> bev) : CondimentDecorator(bev) {}

    std::string getDescription() const override {
        return beverage->getDescription() + ", Whipped Cream";
    }

    double cost() const override {
        return beverage->cost() + 0.10;
    }
};

// ============================================
// EXAMPLE 2: TEXT FORMATTING
// Dynamic text decorations
// ============================================

/**
 * Component: Text
 */
class Text {
public:
    virtual ~Text() {}
    virtual std::string getContent() const = 0;
    virtual void display() const = 0;
};

/**
 * Concrete Component: PlainText
 */
class PlainText : public Text {
private:
    std::string content;

public:
    PlainText(const std::string& text) : content(text) {}

    std::string getContent() const override {
        return content;
    }

    void display() const override {
        std::print("{}", content);
    }
};

/**
 * Base Decorator for Text
 */
class TextDecorator : public Text {
protected:
    std::shared_ptr<Text> text;

public:
    TextDecorator(std::shared_ptr<Text> t) : text(t) {}
    virtual ~TextDecorator() {}
};

/**
 * Concrete Decorator: Bold
 */
class BoldText : public TextDecorator {
public:
    BoldText(std::shared_ptr<Text> t) : TextDecorator(t) {}

    std::string getContent() const override {
        return "<b>" + text->getContent() + "</b>";
    }

    void display() const override {
        std::print("**{}**", text->getContent());
    }
};

/**
 * Concrete Decorator: Italic
 */
class ItalicText : public TextDecorator {
public:
    ItalicText(std::shared_ptr<Text> t) : TextDecorator(t) {}

    std::string getContent() const override {
        return "<i>" + text->getContent() + "</i>";
    }

    void display() const override {
        std::print("_{}_", text->getContent());
    }
};

/**
 * Concrete Decorator: Underline
 */
class UnderlineText : public TextDecorator {
public:
    UnderlineText(std::shared_ptr<Text> t) : TextDecorator(t) {}

    std::string getContent() const override {
        return "<u>" + text->getContent() + "</u>";
    }

    void display() const override {
        std::print("{}", text->getContent());
        std::println("");
        for (size_t i = 0; i < text->getContent().length(); i++) {
            std::print("-");
        }
    }
};

/**
 * Concrete Decorator: Color
 */
class ColorText : public TextDecorator {
private:
    std::string color;

public:
    ColorText(std::shared_ptr<Text> t, const std::string& c)
        : TextDecorator(t), color(c) {}

    std::string getContent() const override {
        return "<span style='color:" + color + "'>" +
               text->getContent() + "</span>";
    }

    void display() const override {
        std::print("[{}]{}[/{}]", color, text->getContent(), color);
    }
};

// ============================================
// EXAMPLE 3: DATA STREAMS
// Stream decorators for I/O operations
// ============================================

/**
 * Component: DataSource
 */
class DataSource {
public:
    virtual ~DataSource() {}
    virtual void writeData(const std::string& data) = 0;
    virtual std::string readData() = 0;
};

/**
 * Concrete Component: FileDataSource
 */
class FileDataSource : public DataSource {
private:
    std::string filename;
    std::string content;

public:
    FileDataSource(const std::string& name) : filename(name) {}

    void writeData(const std::string& data) override {
        content = data;
        std::println("📁 Writing to file '{}': {}", filename, data);
    }

    std::string readData() override {
        std::println("📁 Reading from file '{}'", filename);
        return content;
    }
};

/**
 * Base Decorator for DataSource
 */
class DataSourceDecorator : public DataSource {
protected:
    std::shared_ptr<DataSource> wrappee;

public:
    DataSourceDecorator(std::shared_ptr<DataSource> source) : wrappee(source) {}
    virtual ~DataSourceDecorator() {}

    void writeData(const std::string& data) override {
        wrappee->writeData(data);
    }

    std::string readData() override {
        return wrappee->readData();
    }
};

/**
 * Concrete Decorator: Encryption
 */
class EncryptionDecorator : public DataSourceDecorator {
public:
    EncryptionDecorator(std::shared_ptr<DataSource> source)
        : DataSourceDecorator(source) {}

    void writeData(const std::string& data) override {
        std::println("🔒 Encrypting data...");
        std::string encrypted = encode(data);
        wrappee->writeData(encrypted);
    }

    std::string readData() override {
        std::string data = wrappee->readData();
        std::println("🔓 Decrypting data...");
        return decode(data);
    }

private:
    std::string encode(const std::string& data) {
        // Simple encoding (reverse string for demo)
        std::string result = data;
        reverse(result.begin(), result.end());
        return "[ENCRYPTED]" + result;
    }

    std::string decode(const std::string& data) {
        // Remove prefix and reverse
        std::string result = data.substr(11); // Remove "[ENCRYPTED]"
        reverse(result.begin(), result.end());
        return result;
    }
};

/**
 * Concrete Decorator: Compression
 */
class CompressionDecorator : public DataSourceDecorator {
public:
    CompressionDecorator(std::shared_ptr<DataSource> source)
        : DataSourceDecorator(source) {}

    void writeData(const std::string& data) override {
        std::println("📦 Compressing data (ratio: {}%)", getCompressionRatio(data));
        std::string compressed = compress(data);
        wrappee->writeData(compressed);
    }

    std::string readData() override {
        std::string data = wrappee->readData();
        std::println("📦 Decompressing data...");
        return decompress(data);
    }

private:
    std::string compress(const std::string& data) {
        return "[COMPRESSED]" + data;
    }

    std::string decompress(const std::string& data) {
        return data.substr(12); // Remove "[COMPRESSED]"
    }

    int getCompressionRatio(const std::string& data) {
        return 30 + (data.length() % 20); // Simulated ratio
    }
};

/**
 * Concrete Decorator: Logging
 */
class LoggingDecorator : public DataSourceDecorator {
public:
    LoggingDecorator(std::shared_ptr<DataSource> source)
        : DataSourceDecorator(source) {}

    void writeData(const std::string& data) override {
        std::println("📝 [LOG] Writing data of size: {} bytes", data.length());
        wrappee->writeData(data);
        std::println("📝 [LOG] Write completed");
    }

    std::string readData() override {
        std::println("📝 [LOG] Reading data...");
        std::string data = wrappee->readData();
        std::println("📝 [LOG] Read {} bytes", data.length());
        return data;
    }
};

// ============================================
// EXAMPLE 4: NOTIFICATION SYSTEM
// Multiple notification channels
// ============================================

/**
 * Component: Notifier
 */
class Notifier {
public:
    virtual ~Notifier() {}
    virtual void send(const std::string& message) = 0;
};

/**
 * Concrete Component: BasicNotifier
 */
class BasicNotifier : public Notifier {
public:
    void send(const std::string& message) override {
        std::println("📧 Sending basic notification: {}", message);
    }
};

/**
 * Base Decorator: NotifierDecorator
 */
class NotifierDecorator : public Notifier {
protected:
    std::shared_ptr<Notifier> notifier;

public:
    NotifierDecorator(std::shared_ptr<Notifier> n) : notifier(n) {}
    virtual ~NotifierDecorator() {}

    void send(const std::string& message) override {
        notifier->send(message);
    }
};

/**
 * Concrete Decorator: SMS
 */
class SMSDecorator : public NotifierDecorator {
public:
    SMSDecorator(std::shared_ptr<Notifier> n) : NotifierDecorator(n) {}

    void send(const std::string& message) override {
        notifier->send(message);
        sendSMS(message);
    }

private:
    void sendSMS(const std::string& message) {
        std::println("📱 SMS: {}", message);
    }
};

/**
 * Concrete Decorator: Slack
 */
class SlackDecorator : public NotifierDecorator {
public:
    SlackDecorator(std::shared_ptr<Notifier> n) : NotifierDecorator(n) {}

    void send(const std::string& message) override {
        notifier->send(message);
        sendSlack(message);
    }

private:
    void sendSlack(const std::string& message) {
        std::println("💬 Slack: {}", message);
    }
};

/**
 * Concrete Decorator: Facebook
 */
class FacebookDecorator : public NotifierDecorator {
public:
    FacebookDecorator(std::shared_ptr<Notifier> n) : NotifierDecorator(n) {}

    void send(const std::string& message) override {
        notifier->send(message);
        sendFacebook(message);
    }

private:
    void sendFacebook(const std::string& message) {
        std::println("📘 Facebook: {}", message);
    }
};

// ============================================
// EXAMPLE 5: WINDOW WITH FEATURES
// UI components with optional features
// ============================================

/**
 * Component: Window
 */
class Window {
public:
    virtual ~Window() {}
    virtual void draw() = 0;
    virtual std::string getDescription() = 0;
};

/**
 * Concrete Component: SimpleWindow
 */
class SimpleWindow : public Window {
public:
    void draw() override {
    }

    std::string getDescription() override {
        return "Simple Window";
    }
};

/**
 * Base Decorator: WindowDecorator
 */
class WindowDecorator : public Window {
protected:
    std::shared_ptr<Window> window;

public:
    WindowDecorator(std::shared_ptr<Window> w) : window(w) {}
    virtual ~WindowDecorator() {}
};

/**
 * Concrete Decorator: Scrollbar
 */
class ScrollbarDecorator : public WindowDecorator {
public:
    ScrollbarDecorator(std::shared_ptr<Window> w) : WindowDecorator(w) {}

    void draw() override {
        window->draw();
        drawScrollbar();
    }

    std::string getDescription() override {
        return window->getDescription() + " + Scrollbar";
    }

private:
    void drawScrollbar() {
        std::println("║ [Adding vertical scrollbar]");
    }
};

/**
 * Concrete Decorator: Border
 */
class BorderDecorator : public WindowDecorator {
public:
    BorderDecorator(std::shared_ptr<Window> w) : WindowDecorator(w) {}

    void draw() override {
        drawBorder();
        window->draw();
    }

    std::string getDescription() override {
        return window->getDescription() + " + Border";
    }

private:
    void drawBorder() {
        std::println("╔═════════════════╗");
        std::println("║ [Border added]  ║");
    }
};

// ============================================
// CLIENT CODE & DEMONSTRATIONS
// ============================================

void demonstrateCoffeeShop() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: COFFEE SHOP            ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    // Order 1: Simple espresso
    std::println("--- Order 1 ---");
    std::shared_ptr<Beverage> beverage1 = std::make_shared<Espresso>();
    std::println("{} ${}", beverage1->getDescription(), beverage1->cost());
    std::println("");

    // Order 2: Dark Roast with Mocha and Whip
    std::println("--- Order 2 ---");
    std::shared_ptr<Beverage> beverage2 = std::make_shared<DarkRoast>();
    beverage2 = std::make_shared<Mocha>(beverage2);
    beverage2 = std::make_shared<Whip>(beverage2);
    std::println("{} ${}", beverage2->getDescription(), beverage2->cost());
    std::println("");

    // Order 3: House Blend with Soy, Mocha, and Whip
    std::println("--- Order 3 ---");
    std::shared_ptr<Beverage> beverage3 = std::make_shared<HouseBlend>();
    beverage3 = std::make_shared<Soy>(beverage3);
    beverage3 = std::make_shared<Mocha>(beverage3);
    beverage3 = std::make_shared<Whip>(beverage3);
    std::println("{} ${}", beverage3->getDescription(), beverage3->cost());
    std::println("");

    // Order 4: Double Mocha Decaf with Milk
    std::println("--- Order 4 ---");
    std::shared_ptr<Beverage> beverage4 = std::make_shared<Decaf>();
    beverage4 = std::make_shared<Mocha>(beverage4);
    beverage4 = std::make_shared<Mocha>(beverage4);  // Double mocha!
    beverage4 = std::make_shared<Milk>(beverage4);
    std::println("{} ${}", beverage4->getDescription(), beverage4->cost());
    std::println("");

    std::println("💡 Notice: Same component, different combinations!");
    std::println("   Each decorator adds functionality dynamically.");
}

void demonstrateTextFormatting() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: TEXT FORMATTING        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    // Plain text
    std::println("--- Plain Text ---");
    std::shared_ptr<Text> text1 = std::make_shared<PlainText>("Hello World");
    std::print("Display: ");
    text1->display();
    std::println("");
    std::println("HTML: {}", text1->getContent());
    std::println("");

    // Bold text
    std::println("--- Bold Text ---");
    std::shared_ptr<Text> text2 = std::make_shared<PlainText>("Hello World");
    text2 = std::make_shared<BoldText>(text2);
    std::print("Display: ");
    text2->display();
    std::println("");
    std::println("HTML: {}", text2->getContent());
    std::println("");

    // Bold + Italic
    std::println("--- Bold + Italic ---");
    std::shared_ptr<Text> text3 = std::make_shared<PlainText>("Hello World");
    text3 = std::make_shared<BoldText>(text3);
    text3 = std::make_shared<ItalicText>(text3);
    std::print("Display: ");
    text3->display();
    std::println("");
    std::println("HTML: {}", text3->getContent());
    std::println("");

    // Bold + Italic + Color
    std::println("--- Bold + Italic + Color ---");
    std::shared_ptr<Text> text4 = std::make_shared<PlainText>("Hello World");
    text4 = std::make_shared<BoldText>(text4);
    text4 = std::make_shared<ItalicText>(text4);
    text4 = std::make_shared<ColorText>(text4, "red");
    std::print("Display: ");
    text4->display();
    std::println("");
    std::println("HTML: {}", text4->getContent());
    std::println("");

    // Underline only
    std::println("--- Underline ---");
    std::shared_ptr<Text> text5 = std::make_shared<PlainText>("Important");
    text5 = std::make_shared<UnderlineText>(text5);
    std::print("Display: ");
    text5->display();
    std::println("");
    std::println("");

    std::println("💡 Notice: Text formatting stacked dynamically!");
}

void demonstrateDataStreams() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: DATA STREAMS           ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::string data = "Important secret data";

    // Plain file
    std::println("--- Plain File ---");
    auto source1 = std::make_shared<FileDataSource>("data.txt");
    source1->writeData(data);
    std::string read1 = source1->readData();
    std::println("Read: {}", read1);
    std::println("");

    // Encrypted file
    std::println("--- Encrypted File ---");
    std::shared_ptr<DataSource> source2 = std::make_shared<FileDataSource>("encrypted.txt");
    source2 = std::make_shared<EncryptionDecorator>(source2);
    source2->writeData(data);
    std::string read2 = source2->readData();
    std::println("Read: {}", read2);
    std::println("");

    // Compressed + Encrypted
    std::println("--- Compressed + Encrypted ---");
    std::shared_ptr<DataSource> source3 = std::make_shared<FileDataSource>("secure.dat");
    source3 = std::make_shared<CompressionDecorator>(source3);
    source3 = std::make_shared<EncryptionDecorator>(source3);
    source3->writeData(data);
    std::string read3 = source3->readData();
    std::println("Read: {}", read3);
    std::println("");

    // All features: Logging + Compression + Encryption
    std::println("--- Full Pipeline ---");
    std::shared_ptr<DataSource> source4 = std::make_shared<FileDataSource>("full.dat");
    source4 = std::make_shared<LoggingDecorator>(source4);
    source4 = std::make_shared<CompressionDecorator>(source4);
    source4 = std::make_shared<EncryptionDecorator>(source4);
    source4->writeData(data);
    std::string read4 = source4->readData();
    std::println("Read: {}", read4);
    std::println("");

    std::println("💡 Notice: Decorators applied in layers (pipeline)!");
}

void demonstrateNotifications() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: NOTIFICATIONS          ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::string message = "Server is down!";

    // Basic notification
    std::println("--- Basic Only ---");
    std::shared_ptr<Notifier> notifier1 = std::make_shared<BasicNotifier>();
    notifier1->send(message);
    std::println("");

    // Email + SMS
    std::println("--- Email + SMS ---");
    std::shared_ptr<Notifier> notifier2 = std::make_shared<BasicNotifier>();
    notifier2 = std::make_shared<SMSDecorator>(notifier2);
    notifier2->send(message);
    std::println("");

    // Email + SMS + Slack
    std::println("--- Email + SMS + Slack ---");
    std::shared_ptr<Notifier> notifier3 = std::make_shared<BasicNotifier>();
    notifier3 = std::make_shared<SMSDecorator>(notifier3);
    notifier3 = std::make_shared<SlackDecorator>(notifier3);
    notifier3->send(message);
    std::println("");

    // All channels
    std::println("--- All Channels ---");
    std::shared_ptr<Notifier> notifier4 = std::make_shared<BasicNotifier>();
    notifier4 = std::make_shared<SMSDecorator>(notifier4);
    notifier4 = std::make_shared<SlackDecorator>(notifier4);
    notifier4 = std::make_shared<FacebookDecorator>(notifier4);
    notifier4->send(message);
    std::println("");

    std::println("💡 Notice: Same message, multiple channels!");
}

void demonstrateWindow() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 5: WINDOW DECORATORS      ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    // Simple window
    std::println("--- Simple Window ---");
    std::shared_ptr<Window> window1 = std::make_shared<SimpleWindow>();
    std::println("{}", window1->getDescription());
    window1->draw();
    std::println("");

    // Window with scrollbar
    std::println("--- Window + Scrollbar ---");
    std::shared_ptr<Window> window2 = std::make_shared<SimpleWindow>();
    window2 = std::make_shared<ScrollbarDecorator>(window2);
    std::println("{}", window2->getDescription());
    window2->draw();
    std::println("");

    // Window with border and scrollbar
    std::println("--- Window + Border + Scrollbar ---");
    std::shared_ptr<Window> window3 = std::make_shared<SimpleWindow>();
    window3 = std::make_shared<BorderDecorator>(window3);
    window3 = std::make_shared<ScrollbarDecorator>(window3);
    std::println("{}", window3->getDescription());
    window3->draw();
    std::println("");

    std::println("💡 Notice: Features added without modifying Window class!");
}

// ============================================
// MAIN - DEMONSTRATING THE PATTERN
// ============================================

int main() {
    std::println("========================================");
    std::println("DECORATOR PATTERN DEMO");
    std::println("========================================");

    demonstrateCoffeeShop();
    demonstrateTextFormatting();
    demonstrateDataStreams();
    demonstrateNotifications();
    demonstrateWindow();

    return 0;
}
