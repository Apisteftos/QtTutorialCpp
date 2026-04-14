#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
using namespace std;

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
    virtual string getDescription() const = 0;
    virtual double cost() const = 0;
};

/**
 * Concrete Component 1: Espresso
 */
class Espresso : public Beverage {
public:
    string getDescription() const override {
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
    string getDescription() const override {
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
    string getDescription() const override {
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
    string getDescription() const override {
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
    shared_ptr<Beverage> beverage;

public:
    CondimentDecorator(shared_ptr<Beverage> bev) : beverage(bev) {}
    virtual ~CondimentDecorator() {}
};

/**
 * Concrete Decorator 1: Milk
 */
class Milk : public CondimentDecorator {
public:
    Milk(shared_ptr<Beverage> bev) : CondimentDecorator(bev) {}

    string getDescription() const override {
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
    Mocha(shared_ptr<Beverage> bev) : CondimentDecorator(bev) {}

    string getDescription() const override {
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
    Soy(shared_ptr<Beverage> bev) : CondimentDecorator(bev) {}

    string getDescription() const override {
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
    Whip(shared_ptr<Beverage> bev) : CondimentDecorator(bev) {}

    string getDescription() const override {
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
    virtual string getContent() const = 0;
    virtual void display() const = 0;
};

/**
 * Concrete Component: PlainText
 */
class PlainText : public Text {
private:
    string content;

public:
    PlainText(const string& text) : content(text) {}

    string getContent() const override {
        return content;
    }

    void display() const override {
        cout << content;
    }
};

/**
 * Base Decorator for Text
 */
class TextDecorator : public Text {
protected:
    shared_ptr<Text> text;

public:
    TextDecorator(shared_ptr<Text> t) : text(t) {}
    virtual ~TextDecorator() {}
};

/**
 * Concrete Decorator: Bold
 */
class BoldText : public TextDecorator {
public:
    BoldText(shared_ptr<Text> t) : TextDecorator(t) {}

    string getContent() const override {
        return "<b>" + text->getContent() + "</b>";
    }

    void display() const override {
        cout << "**" << text->getContent() << "**";
    }
};

/**
 * Concrete Decorator: Italic
 */
class ItalicText : public TextDecorator {
public:
    ItalicText(shared_ptr<Text> t) : TextDecorator(t) {}

    string getContent() const override {
        return "<i>" + text->getContent() + "</i>";
    }

    void display() const override {
        cout << "_" << text->getContent() << "_";
    }
};

/**
 * Concrete Decorator: Underline
 */
class UnderlineText : public TextDecorator {
public:
    UnderlineText(shared_ptr<Text> t) : TextDecorator(t) {}

    string getContent() const override {
        return "<u>" + text->getContent() + "</u>";
    }

    void display() const override {
        cout << text->getContent();
        cout << "\n";
        for (size_t i = 0; i < text->getContent().length(); i++) {
            cout << "-";
        }
    }
};

/**
 * Concrete Decorator: Color
 */
class ColorText : public TextDecorator {
private:
    string color;

public:
    ColorText(shared_ptr<Text> t, const string& c)
        : TextDecorator(t), color(c) {}

    string getContent() const override {
        return "<span style='color:" + color + "'>" +
               text->getContent() + "</span>";
    }

    void display() const override {
        cout << "[" << color << "]" << text->getContent() << "[/" << color << "]";
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
    virtual void writeData(const string& data) = 0;
    virtual string readData() = 0;
};

/**
 * Concrete Component: FileDataSource
 */
class FileDataSource : public DataSource {
private:
    string filename;
    string content;

public:
    FileDataSource(const string& name) : filename(name) {}

    void writeData(const string& data) override {
        content = data;
        cout << "📁 Writing to file '" << filename << "': " << data << "\n";
    }

    string readData() override {
        cout << "📁 Reading from file '" << filename << "'\n";
        return content;
    }
};

/**
 * Base Decorator for DataSource
 */
class DataSourceDecorator : public DataSource {
protected:
    shared_ptr<DataSource> wrappee;

public:
    DataSourceDecorator(shared_ptr<DataSource> source) : wrappee(source) {}
    virtual ~DataSourceDecorator() {}

    void writeData(const string& data) override {
        wrappee->writeData(data);
    }

    string readData() override {
        return wrappee->readData();
    }
};

/**
 * Concrete Decorator: Encryption
 */
class EncryptionDecorator : public DataSourceDecorator {
public:
    EncryptionDecorator(shared_ptr<DataSource> source)
        : DataSourceDecorator(source) {}

    void writeData(const string& data) override {
        cout << "🔒 Encrypting data...\n";
        string encrypted = encode(data);
        wrappee->writeData(encrypted);
    }

    string readData() override {
        string data = wrappee->readData();
        cout << "🔓 Decrypting data...\n";
        return decode(data);
    }

private:
    string encode(const string& data) {
        // Simple encoding (reverse string for demo)
        string result = data;
        reverse(result.begin(), result.end());
        return "[ENCRYPTED]" + result;
    }

    string decode(const string& data) {
        // Remove prefix and reverse
        string result = data.substr(11); // Remove "[ENCRYPTED]"
        reverse(result.begin(), result.end());
        return result;
    }
};

/**
 * Concrete Decorator: Compression
 */
class CompressionDecorator : public DataSourceDecorator {
public:
    CompressionDecorator(shared_ptr<DataSource> source)
        : DataSourceDecorator(source) {}

    void writeData(const string& data) override {
        cout << "📦 Compressing data (ratio: " << getCompressionRatio(data) << "%)\n";
        string compressed = compress(data);
        wrappee->writeData(compressed);
    }

    string readData() override {
        string data = wrappee->readData();
        cout << "📦 Decompressing data...\n";
        return decompress(data);
    }

private:
    string compress(const string& data) {
        return "[COMPRESSED]" + data;
    }

    string decompress(const string& data) {
        return data.substr(12); // Remove "[COMPRESSED]"
    }

    int getCompressionRatio(const string& data) {
        return 30 + (data.length() % 20); // Simulated ratio
    }
};

/**
 * Concrete Decorator: Logging
 */
class LoggingDecorator : public DataSourceDecorator {
public:
    LoggingDecorator(shared_ptr<DataSource> source)
        : DataSourceDecorator(source) {}

    void writeData(const string& data) override {
        cout << "📝 [LOG] Writing data of size: " << data.length() << " bytes\n";
        wrappee->writeData(data);
        cout << "📝 [LOG] Write completed\n";
    }

    string readData() override {
        cout << "📝 [LOG] Reading data...\n";
        string data = wrappee->readData();
        cout << "📝 [LOG] Read " << data.length() << " bytes\n";
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
    virtual void send(const string& message) = 0;
};

/**
 * Concrete Component: BasicNotifier
 */
class BasicNotifier : public Notifier {
public:
    void send(const string& message) override {
        cout << "📧 Sending basic notification: " << message << "\n";
    }
};

/**
 * Base Decorator: NotifierDecorator
 */
class NotifierDecorator : public Notifier {
protected:
    shared_ptr<Notifier> notifier;

public:
    NotifierDecorator(shared_ptr<Notifier> n) : notifier(n) {}
    virtual ~NotifierDecorator() {}

    void send(const string& message) override {
        notifier->send(message);
    }
};

/**
 * Concrete Decorator: SMS
 */
class SMSDecorator : public NotifierDecorator {
public:
    SMSDecorator(shared_ptr<Notifier> n) : NotifierDecorator(n) {}

    void send(const string& message) override {
        notifier->send(message);
        sendSMS(message);
    }

private:
    void sendSMS(const string& message) {
        cout << "📱 SMS: " << message << "\n";
    }
};

/**
 * Concrete Decorator: Slack
 */
class SlackDecorator : public NotifierDecorator {
public:
    SlackDecorator(shared_ptr<Notifier> n) : NotifierDecorator(n) {}

    void send(const string& message) override {
        notifier->send(message);
        sendSlack(message);
    }

private:
    void sendSlack(const string& message) {
        cout << "💬 Slack: " << message << "\n";
    }
};

/**
 * Concrete Decorator: Facebook
 */
class FacebookDecorator : public NotifierDecorator {
public:
    FacebookDecorator(shared_ptr<Notifier> n) : NotifierDecorator(n) {}

    void send(const string& message) override {
        notifier->send(message);
        sendFacebook(message);
    }

private:
    void sendFacebook(const string& message) {
        cout << "📘 Facebook: " << message << "\n";
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
    virtual string getDescription() = 0;
};

/**
 * Concrete Component: SimpleWindow
 */
class SimpleWindow : public Window {
public:
    void draw() override {
        cout << "┌─────────────┐\n";
        cout << "│             │\n";
        cout << "│   Window    │\n";
        cout << "│             │\n";
        cout << "└─────────────┘\n";
    }

    string getDescription() override {
        return "Simple Window";
    }
};

/**
 * Base Decorator: WindowDecorator
 */
class WindowDecorator : public Window {
protected:
    shared_ptr<Window> window;

public:
    WindowDecorator(shared_ptr<Window> w) : window(w) {}
    virtual ~WindowDecorator() {}
};

/**
 * Concrete Decorator: Scrollbar
 */
class ScrollbarDecorator : public WindowDecorator {
public:
    ScrollbarDecorator(shared_ptr<Window> w) : WindowDecorator(w) {}

    void draw() override {
        window->draw();
        drawScrollbar();
    }

    string getDescription() override {
        return window->getDescription() + " + Scrollbar";
    }

private:
    void drawScrollbar() {
        cout << "║ [Adding vertical scrollbar]\n";
    }
};

/**
 * Concrete Decorator: Border
 */
class BorderDecorator : public WindowDecorator {
public:
    BorderDecorator(shared_ptr<Window> w) : WindowDecorator(w) {}

    void draw() override {
        drawBorder();
        window->draw();
    }

    string getDescription() override {
        return window->getDescription() + " + Border";
    }

private:
    void drawBorder() {
        cout << "╔═════════════════╗\n";
        cout << "║ [Border added]  ║\n";
    }
};

// ============================================
// CLIENT CODE & DEMONSTRATIONS
// ============================================

void demonstrateCoffeeShop() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: COFFEE SHOP            ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    // Order 1: Simple espresso
    cout << "--- Order 1 ---\n";
    shared_ptr<Beverage> beverage1 = make_shared<Espresso>();
    cout << beverage1->getDescription() << " $" << beverage1->cost() << "\n\n";

    // Order 2: Dark Roast with Mocha and Whip
    cout << "--- Order 2 ---\n";
    shared_ptr<Beverage> beverage2 = make_shared<DarkRoast>();
    beverage2 = make_shared<Mocha>(beverage2);
    beverage2 = make_shared<Whip>(beverage2);
    cout << beverage2->getDescription() << " $" << beverage2->cost() << "\n\n";

    // Order 3: House Blend with Soy, Mocha, and Whip
    cout << "--- Order 3 ---\n";
    shared_ptr<Beverage> beverage3 = make_shared<HouseBlend>();
    beverage3 = make_shared<Soy>(beverage3);
    beverage3 = make_shared<Mocha>(beverage3);
    beverage3 = make_shared<Whip>(beverage3);
    cout << beverage3->getDescription() << " $" << beverage3->cost() << "\n\n";

    // Order 4: Double Mocha Decaf with Milk
    cout << "--- Order 4 ---\n";
    shared_ptr<Beverage> beverage4 = make_shared<Decaf>();
    beverage4 = make_shared<Mocha>(beverage4);
    beverage4 = make_shared<Mocha>(beverage4);  // Double mocha!
    beverage4 = make_shared<Milk>(beverage4);
    cout << beverage4->getDescription() << " $" << beverage4->cost() << "\n\n";

    cout << "💡 Notice: Same component, different combinations!\n";
    cout << "   Each decorator adds functionality dynamically.\n";
}

void demonstrateTextFormatting() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: TEXT FORMATTING        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    // Plain text
    cout << "--- Plain Text ---\n";
    shared_ptr<Text> text1 = make_shared<PlainText>("Hello World");
    cout << "Display: ";
    text1->display();
    cout << "\nHTML: " << text1->getContent() << "\n\n";

    // Bold text
    cout << "--- Bold Text ---\n";
    shared_ptr<Text> text2 = make_shared<PlainText>("Hello World");
    text2 = make_shared<BoldText>(text2);
    cout << "Display: ";
    text2->display();
    cout << "\nHTML: " << text2->getContent() << "\n\n";

    // Bold + Italic
    cout << "--- Bold + Italic ---\n";
    shared_ptr<Text> text3 = make_shared<PlainText>("Hello World");
    text3 = make_shared<BoldText>(text3);
    text3 = make_shared<ItalicText>(text3);
    cout << "Display: ";
    text3->display();
    cout << "\nHTML: " << text3->getContent() << "\n\n";

    // Bold + Italic + Color
    cout << "--- Bold + Italic + Color ---\n";
    shared_ptr<Text> text4 = make_shared<PlainText>("Hello World");
    text4 = make_shared<BoldText>(text4);
    text4 = make_shared<ItalicText>(text4);
    text4 = make_shared<ColorText>(text4, "red");
    cout << "Display: ";
    text4->display();
    cout << "\nHTML: " << text4->getContent() << "\n\n";

    // Underline only
    cout << "--- Underline ---\n";
    shared_ptr<Text> text5 = make_shared<PlainText>("Important");
    text5 = make_shared<UnderlineText>(text5);
    cout << "Display: ";
    text5->display();
    cout << "\n\n";

    cout << "💡 Notice: Text formatting stacked dynamically!\n";
}

void demonstrateDataStreams() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: DATA STREAMS           ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    string data = "Important secret data";

    // Plain file
    cout << "--- Plain File ---\n";
    auto source1 = make_shared<FileDataSource>("data.txt");
    source1->writeData(data);
    string read1 = source1->readData();
    cout << "Read: " << read1 << "\n\n";

    // Encrypted file
    cout << "--- Encrypted File ---\n";
    shared_ptr<DataSource> source2 = make_shared<FileDataSource>("encrypted.txt");
    source2 = make_shared<EncryptionDecorator>(source2);
    source2->writeData(data);
    string read2 = source2->readData();
    cout << "Read: " << read2 << "\n\n";

    // Compressed + Encrypted
    cout << "--- Compressed + Encrypted ---\n";
    shared_ptr<DataSource> source3 = make_shared<FileDataSource>("secure.dat");
    source3 = make_shared<CompressionDecorator>(source3);
    source3 = make_shared<EncryptionDecorator>(source3);
    source3->writeData(data);
    string read3 = source3->readData();
    cout << "Read: " << read3 << "\n\n";

    // All features: Logging + Compression + Encryption
    cout << "--- Full Pipeline ---\n";
    shared_ptr<DataSource> source4 = make_shared<FileDataSource>("full.dat");
    source4 = make_shared<LoggingDecorator>(source4);
    source4 = make_shared<CompressionDecorator>(source4);
    source4 = make_shared<EncryptionDecorator>(source4);
    source4->writeData(data);
    string read4 = source4->readData();
    cout << "Read: " << read4 << "\n\n";

    cout << "💡 Notice: Decorators applied in layers (pipeline)!\n";
}

void demonstrateNotifications() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: NOTIFICATIONS          ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    string message = "Server is down!";

    // Basic notification
    cout << "--- Basic Only ---\n";
    shared_ptr<Notifier> notifier1 = make_shared<BasicNotifier>();
    notifier1->send(message);
    cout << "\n";

    // Email + SMS
    cout << "--- Email + SMS ---\n";
    shared_ptr<Notifier> notifier2 = make_shared<BasicNotifier>();
    notifier2 = make_shared<SMSDecorator>(notifier2);
    notifier2->send(message);
    cout << "\n";

    // Email + SMS + Slack
    cout << "--- Email + SMS + Slack ---\n";
    shared_ptr<Notifier> notifier3 = make_shared<BasicNotifier>();
    notifier3 = make_shared<SMSDecorator>(notifier3);
    notifier3 = make_shared<SlackDecorator>(notifier3);
    notifier3->send(message);
    cout << "\n";

    // All channels
    cout << "--- All Channels ---\n";
    shared_ptr<Notifier> notifier4 = make_shared<BasicNotifier>();
    notifier4 = make_shared<SMSDecorator>(notifier4);
    notifier4 = make_shared<SlackDecorator>(notifier4);
    notifier4 = make_shared<FacebookDecorator>(notifier4);
    notifier4->send(message);
    cout << "\n";

    cout << "💡 Notice: Same message, multiple channels!\n";
}

void demonstrateWindow() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: WINDOW DECORATORS      ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    // Simple window
    cout << "--- Simple Window ---\n";
    shared_ptr<Window> window1 = make_shared<SimpleWindow>();
    cout << window1->getDescription() << "\n";
    window1->draw();
    cout << "\n";

    // Window with scrollbar
    cout << "--- Window + Scrollbar ---\n";
    shared_ptr<Window> window2 = make_shared<SimpleWindow>();
    window2 = make_shared<ScrollbarDecorator>(window2);
    cout << window2->getDescription() << "\n";
    window2->draw();
    cout << "\n";

    // Window with border and scrollbar
    cout << "--- Window + Border + Scrollbar ---\n";
    shared_ptr<Window> window3 = make_shared<SimpleWindow>();
    window3 = make_shared<BorderDecorator>(window3);
    window3 = make_shared<ScrollbarDecorator>(window3);
    cout << window3->getDescription() << "\n";
    window3->draw();
    cout << "\n";

    cout << "💡 Notice: Features added without modifying Window class!\n";
}

// ============================================
// MAIN - DEMONSTRATING THE PATTERN
// ============================================

int main() {
    cout << "========================================\n";
    cout << "DECORATOR PATTERN DEMO\n";
    cout << "========================================\n";

    demonstrateCoffeeShop();
    demonstrateTextFormatting();
    demonstrateDataStreams();
    demonstrateNotifications();
    demonstrateWindow();

    // ============================================
    // Benefits Summary
    // ============================================
    cout << "\n========================================\n";
    cout << "BENEFITS OF DECORATOR PATTERN\n";
    cout << "========================================\n";
    cout << "✅ Add responsibilities dynamically\n";
    cout << "✅ More flexible than static inheritance\n";
    cout << "✅ Avoids feature-laden classes\n";
    cout << "✅ Supports Open/Closed Principle\n";
    cout << "✅ Can combine multiple decorators\n";
    cout << "✅ Pay-per-use (only add what you need)\n";
    cout << "✅ Responsibilities can be added/removed at runtime\n";

    cout << "\n========================================\n";
    cout << "WHEN TO USE DECORATOR\n";
    cout << "========================================\n";
    cout << "1. Add responsibilities dynamically and transparently\n";
    cout << "2. Responsibilities can be withdrawn\n";
    cout << "3. Extension by subclassing is impractical\n";
    cout << "4. Add features to individual objects, not classes\n";
    cout << "5. Avoid class explosion from combinations\n";
    cout << "6. Follow Open/Closed Principle\n";

    cout << "\n========================================\n";
    cout << "STRUCTURE\n";
    cout << "========================================\n";
    cout << "Component (interface)\n";
    cout << "├── ConcreteComponent (base object)\n";
    cout << "└── Decorator (wraps Component)\n";
    cout << "    ├── ConcreteDecoratorA\n";
    cout << "    └── ConcreteDecoratorB\n";
    cout << "\nDecorators wrap components recursively!\n";

    cout << "\n========================================\n";
    cout << "DECORATOR vs OTHER PATTERNS\n";
    cout << "========================================\n";
    cout << "Decorator:\n";
    cout << "  • Adds responsibilities (wrapper chain)\n";
    cout << "  • Same interface\n";
    cout << "  • Changes behavior\n";
    cout << "\nAdapter:\n";
    cout << "  • Converts interface\n";
    cout << "  • Different interface\n";
    cout << "  • Enables compatibility\n";
    cout << "\nComposite:\n";
    cout << "  • Tree structure\n";
    cout << "  • Part-whole hierarchy\n";
    cout << "  • Aggregates children\n";
    cout << "\nProxy:\n";
    cout << "  • Controls access\n";
    cout << "  • Same interface\n";
    cout << "  • Adds control layer\n";

    cout << "\n========================================\n";
    cout << "WITHOUT DECORATOR (CLASS EXPLOSION)\n";
    cout << "========================================\n";
    cout << "Espresso\n";
    cout << "EspressoWithMilk\n";
    cout << "EspressoWithMocha\n";
    cout << "EspressoWithMilkAndMocha\n";
    cout << "EspressoWithMilkAndMochaAndWhip\n";
    cout << "... exponential growth!\n";

    cout << "\n========================================\n";
    cout << "WITH DECORATOR (CLEAN)\n";
    cout << "========================================\n";
    cout << "Component: Espresso, HouseBlend, DarkRoast\n";
    cout << "Decorators: Milk, Mocha, Whip, Soy\n";
    cout << "Total: 7 classes for unlimited combinations!\n";

    cout << "\n========================================\n";
    cout << "REAL-WORLD EXAMPLES\n";
    cout << "========================================\n";
    cout << "• Java I/O streams (BufferedInputStream, etc.)\n";
    cout << "• GUI components (borders, scrollbars)\n";
    cout << "• Text formatting (bold, italic, color)\n";
    cout << "• Coffee shop beverages (condiments)\n";
    cout << "• Data encryption/compression\n";
    cout << "• Notification systems (multi-channel)\n";
    cout << "• HTTP middleware (logging, auth)\n";
    cout << "• Pizza toppings\n";

    return 0;
}
