#include <iostream>
#include <string>
#include <memory>
#include <vector>
using namespace std;

// ============================================
// EXAMPLE 1: DEVICE & REMOTE CONTROL
// Classic bridge pattern example
// ============================================

/**
 * Implementation Interface
 * Defines the interface for all concrete implementations
 */
class Device {
public:
    virtual ~Device() {}
    virtual bool isEnabled() = 0;
    virtual void enable() = 0;
    virtual void disable() = 0;
    virtual int getVolume() = 0;
    virtual void setVolume(int percent) = 0;
    virtual int getChannel() = 0;
    virtual void setChannel(int channel) = 0;
    virtual string getDeviceName() = 0;
};

/**
 * Concrete Implementation 1: TV
 */
class TV : public Device {
private:
    bool on;
    int volume;
    int channel;

public:
    TV() : on(false), volume(30), channel(1) {}

    bool isEnabled() override { return on; }

    void enable() override {
        on = true;
        cout << "📺 TV: Turning ON\n";
    }

    void disable() override {
        on = false;
        cout << "📺 TV: Turning OFF\n";
    }

    int getVolume() override { return volume; }

    void setVolume(int percent) override {
        volume = percent;
        cout << "📺 TV: Volume set to " << volume << "%\n";
    }

    int getChannel() override { return channel; }

    void setChannel(int ch) override {
        channel = ch;
        cout << "📺 TV: Channel changed to " << channel << "\n";
    }

    string getDeviceName() override { return "TV"; }
};

/**
 * Concrete Implementation 2: Radio
 */
class Radio : public Device {
private:
    bool on;
    int volume;
    int channel;

public:
    Radio() : on(false), volume(20), channel(1) {}

    bool isEnabled() override { return on; }

    void enable() override {
        on = true;
        cout << "📻 Radio: Turning ON\n";
    }

    void disable() override {
        on = false;
        cout << "📻 Radio: Turning OFF\n";
    }

    int getVolume() override { return volume; }

    void setVolume(int percent) override {
        volume = percent;
        cout << "📻 Radio: Volume set to " << volume << "%\n";
    }

    int getChannel() override { return channel; }

    void setChannel(int ch) override {
        channel = ch;
        cout << "📻 Radio: Frequency changed to " << channel << " FM\n";
    }

    string getDeviceName() override { return "Radio"; }
};

/**
 * Concrete Implementation 3: Projector
 */
class Projector : public Device {
private:
    bool on;
    int brightness;
    int input;

public:
    Projector() : on(false), brightness(50), input(1) {}

    bool isEnabled() override { return on; }

    void enable() override {
        on = true;
        cout << "📽️  Projector: Turning ON\n";
    }

    void disable() override {
        on = false;
        cout << "📽️  Projector: Turning OFF\n";
    }

    int getVolume() override { return brightness; }

    void setVolume(int percent) override {
        brightness = percent;
        cout << "📽️  Projector: Brightness set to " << brightness << "%\n";
    }

    int getChannel() override { return input; }

    void setChannel(int inputNum) override {
        input = inputNum;
        cout << "📽️  Projector: Input changed to HDMI " << input << "\n";
    }

    string getDeviceName() override { return "Projector"; }
};

/**
 * Abstraction
 * Defines the interface for the "control" part
 */
class RemoteControl {
protected:
    shared_ptr<Device> device;

public:
    RemoteControl(shared_ptr<Device> dev) : device(dev) {}
    virtual ~RemoteControl() {}

    virtual void togglePower() {
        cout << "\n🎮 Remote: Toggle power button pressed\n";
        if (device->isEnabled()) {
            device->disable();
        } else {
            device->enable();
        }
    }

    virtual void volumeDown() {
        cout << "🎮 Remote: Volume down button pressed\n";
        int vol = device->getVolume();
        device->setVolume(vol - 10);
    }

    virtual void volumeUp() {
        cout << "🎮 Remote: Volume up button pressed\n";
        int vol = device->getVolume();
        device->setVolume(vol + 10);
    }

    virtual void channelDown() {
        cout << "🎮 Remote: Channel down button pressed\n";
        int ch = device->getChannel();
        device->setChannel(ch - 1);
    }

    virtual void channelUp() {
        cout << "🎮 Remote: Channel up button pressed\n";
        int ch = device->getChannel();
        device->setChannel(ch + 1);
    }
};

/**
 * Refined Abstraction 1: Advanced Remote
 * Adds more features to the base remote
 */
class AdvancedRemote : public RemoteControl {
public:
    AdvancedRemote(shared_ptr<Device> dev) : RemoteControl(dev) {}

    void mute() {
        cout << "🎮 Advanced Remote: Mute button pressed\n";
        device->setVolume(0);
    }

    void setChannel(int channel) {
        cout << "🎮 Advanced Remote: Direct channel selection\n";
        device->setChannel(channel);
    }
};

/**
 * Refined Abstraction 2: Voice Remote
 */
class VoiceRemote : public RemoteControl {
public:
    VoiceRemote(shared_ptr<Device> dev) : RemoteControl(dev) {}

    void voiceCommand(const string& command) {
        cout << "🎤 Voice Remote: \"" << command << "\"\n";

        if (command == "turn on") {
            if (!device->isEnabled()) device->enable();
        } else if (command == "turn off") {
            if (device->isEnabled()) device->disable();
        } else if (command == "louder") {
            volumeUp();
        } else if (command == "quieter") {
            volumeDown();
        } else {
            cout << "❌ Command not recognized\n";
        }
    }
};

// ============================================
// EXAMPLE 2: SHAPE & RENDERING
// Graphics with different rendering engines
// ============================================

/**
 * Implementation: Rendering Engine
 */
class Renderer {
public:
    virtual ~Renderer() {}
    virtual void renderCircle(float x, float y, float radius) = 0;
    virtual void renderSquare(float x, float y, float side) = 0;
    virtual string getRendererName() = 0;
};

/**
 * Concrete Implementation: Vector Renderer
 */
class VectorRenderer : public Renderer {
public:
    void renderCircle(float x, float y, float radius) override {
        cout << "🎨 Vector: Drawing circle at (" << x << "," << y
             << ") radius=" << radius << "\n";
    }

    void renderSquare(float x, float y, float side) override {
        cout << "🎨 Vector: Drawing square at (" << x << "," << y
             << ") side=" << side << "\n";
    }

    string getRendererName() override { return "Vector Renderer"; }
};

/**
 * Concrete Implementation: Raster Renderer
 */
class RasterRenderer : public Renderer {
public:
    void renderCircle(float x, float y, float radius) override {
        cout << "🖼️  Raster: Rendering circle at (" << x << "," << y
             << ") radius=" << radius << " pixels\n";
    }

    void renderSquare(float x, float y, float side) override {
        cout << "🖼️  Raster: Rendering square at (" << x << "," << y
             << ") side=" << side << " pixels\n";
    }

    string getRendererName() override { return "Raster Renderer"; }
};

/**
 * Concrete Implementation: OpenGL Renderer
 */
class OpenGLRenderer : public Renderer {
public:
    void renderCircle(float x, float y, float radius) override {
        cout << "🎮 OpenGL: glBegin(GL_TRIANGLE_FAN) circle at ("
             << x << "," << y << ")\n";
    }

    void renderSquare(float x, float y, float side) override {
        cout << "🎮 OpenGL: glBegin(GL_QUADS) square at ("
             << x << "," << y << ")\n";
    }

    string getRendererName() override { return "OpenGL Renderer"; }
};

/**
 * Abstraction: Shape
 */
class Shape {
protected:
    shared_ptr<Renderer> renderer;
    float x, y;

public:
    Shape(shared_ptr<Renderer> r, float x, float y)
        : renderer(r), x(x), y(y) {}
    virtual ~Shape() {}

    virtual void draw() = 0;
    virtual void move(float newX, float newY) {
        x = newX;
        y = newY;
        cout << "📍 Shape moved to (" << x << "," << y << ")\n";
    }
};

/**
 * Refined Abstraction: Circle
 */
class Circle : public Shape {
private:
    float radius;

public:
    Circle(shared_ptr<Renderer> r, float x, float y, float rad)
        : Shape(r, x, y), radius(rad) {}

    void draw() override {
        cout << "⭕ Circle: draw() called\n";
        renderer->renderCircle(x, y, radius);
    }

    void resize(float newRadius) {
        radius = newRadius;
        cout << "⭕ Circle resized to radius=" << radius << "\n";
    }
};

/**
 * Refined Abstraction: Square
 */
class Square : public Shape {
private:
    float side;

public:
    Square(shared_ptr<Renderer> r, float x, float y, float s)
        : Shape(r, x, y), side(s) {}

    void draw() override {
        cout << "◻️  Square: draw() called\n";
        renderer->renderSquare(x, y, side);
    }

    void resize(float newSide) {
        side = newSide;
        cout << "◻️  Square resized to side=" << side << "\n";
    }
};

// ============================================
// EXAMPLE 3: MESSAGE SENDER
// Messages sent through different platforms
// ============================================

/**
 * Implementation: Message Sender Platform
 */
class MessageSender {
public:
    virtual ~MessageSender() {}
    virtual void sendMessage(const string& recipient, const string& message) = 0;
    virtual string getPlatformName() = 0;
};

/**
 * Concrete Implementation: Email
 */
class EmailSender : public MessageSender {
public:
    void sendMessage(const string& recipient, const string& message) override {
        cout << "📧 Email: Sending to " << recipient << "\n";
        cout << "   Subject: Message\n";
        cout << "   Body: " << message << "\n";
    }

    string getPlatformName() override { return "Email"; }
};

/**
 * Concrete Implementation: SMS
 */
class SMSSender : public MessageSender {
public:
    void sendMessage(const string& recipient, const string& message) override {
        cout << "📱 SMS: Sending to " << recipient << "\n";
        cout << "   Text: " << message.substr(0, 160) << "\n";
        if (message.length() > 160) {
            cout << "   ⚠️  Message truncated to 160 characters\n";
        }
    }

    string getPlatformName() override { return "SMS"; }
};

/**
 * Concrete Implementation: Slack
 */
class SlackSender : public MessageSender {
public:
    void sendMessage(const string& recipient, const string& message) override {
        cout << "💬 Slack: Posting to #" << recipient << "\n";
        cout << "   Message: " << message << "\n";
        cout << "   [Sent via Slack API]\n";
    }

    string getPlatformName() override { return "Slack"; }
};

/**
 * Abstraction: Message
 */
class Message {
protected:
    shared_ptr<MessageSender> sender;
    string recipient;
    string content;

public:
    Message(shared_ptr<MessageSender> s, const string& r, const string& c)
        : sender(s), recipient(r), content(c) {}
    virtual ~Message() {}

    virtual void send() = 0;
};

/**
 * Refined Abstraction: Text Message
 */
class TextMessage : public Message {
public:
    TextMessage(shared_ptr<MessageSender> s, const string& r, const string& c)
        : Message(s, r, c) {}

    void send() override {
        cout << "\n📤 Sending text message via " << sender->getPlatformName() << "\n";
        sender->sendMessage(recipient, content);
    }
};

/**
 * Refined Abstraction: Encrypted Message
 */
class EncryptedMessage : public Message {
public:
    EncryptedMessage(shared_ptr<MessageSender> s, const string& r, const string& c)
        : Message(s, r, c) {}

    void send() override {
        cout << "\n🔒 Encrypting and sending via " << sender->getPlatformName() << "\n";
        string encrypted = "[ENCRYPTED] " + content;
        sender->sendMessage(recipient, encrypted);
    }
};

/**
 * Refined Abstraction: Urgent Message
 */
class UrgentMessage : public Message {
public:
    UrgentMessage(shared_ptr<MessageSender> s, const string& r, const string& c)
        : Message(s, r, c) {}

    void send() override {
        cout << "\n🚨 URGENT message via " << sender->getPlatformName() << "\n";
        string urgent = "⚠️ URGENT: " + content;
        sender->sendMessage(recipient, urgent);
    }
};

// ============================================
// CLIENT CODE & DEMONSTRATIONS
// ============================================

void demonstrateRemoteControl() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: REMOTE & DEVICE        ║\n";
    cout << "╚════════════════════════════════════╝\n";

    // Create devices
    auto tv = make_shared<TV>();
    auto radio = make_shared<Radio>();
    auto projector = make_shared<Projector>();

    // Basic remote with TV
    cout << "\n--- Basic Remote with TV ---\n";
    RemoteControl basicRemote(tv);
    basicRemote.togglePower();
    basicRemote.volumeUp();
    basicRemote.channelUp();
    basicRemote.togglePower();

    // Advanced remote with Radio
    cout << "\n--- Advanced Remote with Radio ---\n";
    AdvancedRemote advancedRemote(radio);
    advancedRemote.togglePower();
    advancedRemote.setChannel(105);
    advancedRemote.volumeUp();
    advancedRemote.mute();

    // Voice remote with Projector
    cout << "\n--- Voice Remote with Projector ---\n";
    VoiceRemote voiceRemote(projector);
    voiceRemote.voiceCommand("turn on");
    voiceRemote.voiceCommand("louder");
    voiceRemote.channelUp();

    cout << "\n💡 Notice: Same remote works with different devices!\n";
    cout << "   Abstraction (Remote) is independent of Implementation (Device)\n";
}

void demonstrateShapeRendering() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: SHAPE & RENDERING      ║\n";
    cout << "╚════════════════════════════════════╝\n";

    // Create renderers
    auto vectorRenderer = make_shared<VectorRenderer>();
    auto rasterRenderer = make_shared<RasterRenderer>();
    auto openglRenderer = make_shared<OpenGLRenderer>();

    // Circle with different renderers
    cout << "\n--- Circle with Vector Renderer ---\n";
    Circle circle1(vectorRenderer, 10, 10, 5);
    circle1.draw();
    circle1.resize(7);
    circle1.draw();

    cout << "\n--- Circle with Raster Renderer ---\n";
    Circle circle2(rasterRenderer, 20, 20, 8);
    circle2.draw();

    cout << "\n--- Circle with OpenGL Renderer ---\n";
    Circle circle3(openglRenderer, 30, 30, 10);
    circle3.draw();

    // Square with different renderers
    cout << "\n--- Square with Vector Renderer ---\n";
    Square square1(vectorRenderer, 0, 0, 10);
    square1.draw();
    square1.move(5, 5);
    square1.draw();

    cout << "\n--- Square with OpenGL Renderer ---\n";
    Square square2(openglRenderer, 15, 15, 12);
    square2.draw();

    cout << "\n💡 Notice: Shapes work with any renderer!\n";
    cout << "   Shape hierarchy is independent of Renderer hierarchy\n";
}

void demonstrateMessaging() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: MESSAGE SENDER         ║\n";
    cout << "╚════════════════════════════════════╝\n";

    // Create senders
    auto email = make_shared<EmailSender>();
    auto sms = make_shared<SMSSender>();
    auto slack = make_shared<SlackSender>();

    // Text message via different platforms
    cout << "\n--- Text Message via Email ---\n";
    TextMessage msg1(email, "user@example.com", "Hello from email!");
    msg1.send();

    cout << "\n--- Text Message via SMS ---\n";
    TextMessage msg2(sms, "+1234567890", "Hello from SMS!");
    msg2.send();

    cout << "\n--- Encrypted Message via Slack ---\n";
    EncryptedMessage msg3(slack, "general", "Secret information");
    msg3.send();

    cout << "\n--- Urgent Message via Email ---\n";
    UrgentMessage msg4(email, "boss@company.com", "Server is down!");
    msg4.send();

    cout << "\n--- Urgent Message via SMS ---\n";
    UrgentMessage msg5(sms, "+9876543210", "Emergency meeting now!");
    msg5.send();

    cout << "\n💡 Notice: Message types work with any sender!\n";
    cout << "   Message abstraction is independent of Sender implementation\n";
}

// ============================================
// MAIN - DEMONSTRATING THE PATTERN
// ============================================

int main() {
    cout << "========================================\n";
    cout << "BRIDGE PATTERN DEMO\n";
    cout << "========================================\n";

    demonstrateRemoteControl();
    demonstrateShapeRendering();
    demonstrateMessaging();

    // ============================================
    // Benefits Summary
    // ============================================
    cout << "\n========================================\n";
    cout << "BENEFITS OF BRIDGE PATTERN\n";
    cout << "========================================\n";
    cout << "✅ Decouples abstraction from implementation\n";
    cout << "✅ Both can vary independently\n";
    cout << "✅ Improves extensibility (add abstractions/implementations)\n";
    cout << "✅ Hides implementation details from client\n";
    cout << "✅ Open/Closed Principle (open for extension)\n";
    cout << "✅ Single Responsibility (separate concerns)\n";
    cout << "✅ Can switch implementations at runtime\n";

    cout << "\n========================================\n";
    cout << "WHEN TO USE BRIDGE\n";
    cout << "========================================\n";
    cout << "1. Avoid permanent binding between abstraction/implementation\n";
    cout << "2. Both should be extensible by subclassing\n";
    cout << "3. Changes in implementation shouldn't affect clients\n";
    cout << "4. Share implementation among multiple objects\n";
    cout << "5. Prevent class explosion from combinations\n";
    cout << "6. Switch implementations at runtime\n";

    cout << "\n========================================\n";
    cout << "BRIDGE vs ADAPTER\n";
    cout << "========================================\n";
    cout << "Bridge:\n";
    cout << "  • Designed upfront\n";
    cout << "  • Lets abstraction and implementation vary independently\n";
    cout << "  • Works with new code\n";
    cout << "  • Intent: Separation of concerns\n";
    cout << "\nAdapter:\n";
    cout << "  • Used after design (retrofit)\n";
    cout << "  • Makes incompatible interfaces work together\n";
    cout << "  • Works with existing code\n";
    cout << "  • Intent: Interface compatibility\n";

    cout << "\n========================================\n";
    cout << "STRUCTURE\n";
    cout << "========================================\n";
    cout << "Abstraction Hierarchy:\n";
    cout << "  Abstraction (has-a Implementation)\n";
    cout << "  └── RefinedAbstraction1\n";
    cout << "  └── RefinedAbstraction2\n";
    cout << "\nImplementation Hierarchy:\n";
    cout << "  Implementation (interface)\n";
    cout << "  └── ConcreteImplementation1\n";
    cout << "  └── ConcreteImplementation2\n";
    cout << "\nBridge = Connection between hierarchies\n";

    cout << "\n========================================\n";
    cout << "REAL-WORLD EXAMPLES\n";
    cout << "========================================\n";
    cout << "• GUI frameworks (Window ←→ OS)\n";
    cout << "• Graphics (Shape ←→ Renderer)\n";
    cout << "• Database (API ←→ Driver)\n";
    cout << "• Remote controls (Remote ←→ Device)\n";
    cout << "• Messaging (Message ←→ Platform)\n";
    cout << "• Vehicles (Car ←→ Engine)\n";
    cout << "• Persistence (Object ←→ Storage)\n";

    return 0;
}
