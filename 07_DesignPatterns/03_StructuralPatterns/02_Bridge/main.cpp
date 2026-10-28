#include <string>
#include <memory>
#include <vector>
#include <print>

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
    virtual std::string getDeviceName() = 0;
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
        std::println("📺 TV: Turning ON");
    }

    void disable() override {
        on = false;
        std::println("📺 TV: Turning OFF");
    }

    int getVolume() override { return volume; }

    void setVolume(int percent) override {
        volume = percent;
        std::println("📺 TV: Volume set to {}%", volume);
    }

    int getChannel() override { return channel; }

    void setChannel(int ch) override {
        channel = ch;
        std::println("📺 TV: Channel changed to {}", channel);
    }

    std::string getDeviceName() override { return "TV"; }
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
        std::println("📻 Radio: Turning ON");
    }

    void disable() override {
        on = false;
        std::println("📻 Radio: Turning OFF");
    }

    int getVolume() override { return volume; }

    void setVolume(int percent) override {
        volume = percent;
        std::println("📻 Radio: Volume set to {}%", volume);
    }

    int getChannel() override { return channel; }

    void setChannel(int ch) override {
        channel = ch;
        std::println("📻 Radio: Frequency changed to {} FM", channel);
    }

    std::string getDeviceName() override { return "Radio"; }
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
        std::println("📽️  Projector: Turning ON");
    }

    void disable() override {
        on = false;
        std::println("📽️  Projector: Turning OFF");
    }

    int getVolume() override { return brightness; }

    void setVolume(int percent) override {
        brightness = percent;
        std::println("📽️  Projector: Brightness set to {}%", brightness);
    }

    int getChannel() override { return input; }

    void setChannel(int inputNum) override {
        input = inputNum;
        std::println("📽️  Projector: Input changed to HDMI {}", input);
    }

    std::string getDeviceName() override { return "Projector"; }
};

/**
 * Abstraction
 * Defines the interface for the "control" part
 */
class RemoteControl {
protected:
    std::shared_ptr<Device> device;

public:
    RemoteControl(std::shared_ptr<Device> dev) : device(dev) {}
    virtual ~RemoteControl() {}

    virtual void togglePower() {
        std::println("");
        std::println("🎮 Remote: Toggle power button pressed");
        if (device->isEnabled()) {
            device->disable();
        } else {
            device->enable();
        }
    }

    virtual void volumeDown() {
        std::println("🎮 Remote: Volume down button pressed");
        int vol = device->getVolume();
        device->setVolume(vol - 10);
    }

    virtual void volumeUp() {
        std::println("🎮 Remote: Volume up button pressed");
        int vol = device->getVolume();
        device->setVolume(vol + 10);
    }

    virtual void channelDown() {
        std::println("🎮 Remote: Channel down button pressed");
        int ch = device->getChannel();
        device->setChannel(ch - 1);
    }

    virtual void channelUp() {
        std::println("🎮 Remote: Channel up button pressed");
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
    AdvancedRemote(std::shared_ptr<Device> dev) : RemoteControl(dev) {}

    void mute() {
        std::println("🎮 Advanced Remote: Mute button pressed");
        device->setVolume(0);
    }

    void setChannel(int channel) {
        std::println("🎮 Advanced Remote: Direct channel selection");
        device->setChannel(channel);
    }
};

/**
 * Refined Abstraction 2: Voice Remote
 */
class VoiceRemote : public RemoteControl {
public:
    VoiceRemote(std::shared_ptr<Device> dev) : RemoteControl(dev) {}

    void voiceCommand(const std::string& command) {
        std::println("🎤 Voice Remote: \"{}\"", command);

        if (command == "turn on") {
            if (!device->isEnabled()) device->enable();
        } else if (command == "turn off") {
            if (device->isEnabled()) device->disable();
        } else if (command == "louder") {
            volumeUp();
        } else if (command == "quieter") {
            volumeDown();
        } else {
            std::println("❌ Command not recognized");
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
    virtual std::string getRendererName() = 0;
};

/**
 * Concrete Implementation: Vector Renderer
 */
class VectorRenderer : public Renderer {
public:
    void renderCircle(float x, float y, float radius) override {
        std::println("🎨 Vector: Drawing circle at ({},{}) radius={}", x, y, radius);
    }

    void renderSquare(float x, float y, float side) override {
        std::println("🎨 Vector: Drawing square at ({},{}) side={}", x, y, side);
    }

    std::string getRendererName() override { return "Vector Renderer"; }
};

/**
 * Concrete Implementation: Raster Renderer
 */
class RasterRenderer : public Renderer {
public:
    void renderCircle(float x, float y, float radius) override {
        std::println("🖼️  Raster: Rendering circle at ({},{}) radius={} pixels", x, y, radius);
    }

    void renderSquare(float x, float y, float side) override {
        std::println("🖼️  Raster: Rendering square at ({},{}) side={} pixels", x, y, side);
    }

    std::string getRendererName() override { return "Raster Renderer"; }
};

/**
 * Concrete Implementation: OpenGL Renderer
 */
class OpenGLRenderer : public Renderer {
public:
    void renderCircle(float x, float y, float radius) override {
        std::println("🎮 OpenGL: glBegin(GL_TRIANGLE_FAN) circle at ({},{})", x, y);
    }

    void renderSquare(float x, float y, float side) override {
        std::println("🎮 OpenGL: glBegin(GL_QUADS) square at ({},{})", x, y);
    }

    std::string getRendererName() override { return "OpenGL Renderer"; }
};

/**
 * Abstraction: Shape
 */
class Shape {
protected:
    std::shared_ptr<Renderer> renderer;
    float x, y;

public:
    Shape(std::shared_ptr<Renderer> r, float x, float y)
        : renderer(r), x(x), y(y) {}
    virtual ~Shape() {}

    virtual void draw() = 0;
    virtual void move(float newX, float newY) {
        x = newX;
        y = newY;
        std::println("📍 Shape moved to ({},{})", x, y);
    }
};

/**
 * Refined Abstraction: Circle
 */
class Circle : public Shape {
private:
    float radius;

public:
    Circle(std::shared_ptr<Renderer> r, float x, float y, float rad)
        : Shape(r, x, y), radius(rad) {}

    void draw() override {
        std::println("⭕ Circle: draw() called");
        renderer->renderCircle(x, y, radius);
    }

    void resize(float newRadius) {
        radius = newRadius;
        std::println("⭕ Circle resized to radius={}", radius);
    }
};

/**
 * Refined Abstraction: Square
 */
class Square : public Shape {
private:
    float side;

public:
    Square(std::shared_ptr<Renderer> r, float x, float y, float s)
        : Shape(r, x, y), side(s) {}

    void draw() override {
        std::println("◻️  Square: draw() called");
        renderer->renderSquare(x, y, side);
    }

    void resize(float newSide) {
        side = newSide;
        std::println("◻️  Square resized to side={}", side);
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
    virtual void sendMessage(const std::string& recipient, const std::string& message) = 0;
    virtual std::string getPlatformName() = 0;
};

/**
 * Concrete Implementation: Email
 */
class EmailSender : public MessageSender {
public:
    void sendMessage(const std::string& recipient, const std::string& message) override {
        std::println("📧 Email: Sending to {}", recipient);
        std::println("   Subject: Message");
        std::println("   Body: {}", message);
    }

    std::string getPlatformName() override { return "Email"; }
};

/**
 * Concrete Implementation: SMS
 */
class SMSSender : public MessageSender {
public:
    void sendMessage(const std::string& recipient, const std::string& message) override {
        std::println("📱 SMS: Sending to {}", recipient);
        std::println("   Text: {}", message.substr(0, 160));
        if (message.length() > 160) {
            std::println("   ⚠️  Message truncated to 160 characters");
        }
    }

    std::string getPlatformName() override { return "SMS"; }
};

/**
 * Concrete Implementation: Slack
 */
class SlackSender : public MessageSender {
public:
    void sendMessage(const std::string& recipient, const std::string& message) override {
        std::println("💬 Slack: Posting to #{}", recipient);
        std::println("   Message: {}", message);
        std::println("   [Sent via Slack API]");
    }

    std::string getPlatformName() override { return "Slack"; }
};

/**
 * Abstraction: Message
 */
class Message {
protected:
    std::shared_ptr<MessageSender> sender;
    std::string recipient;
    std::string content;

public:
    Message(std::shared_ptr<MessageSender> s, const std::string& r, const std::string& c)
        : sender(s), recipient(r), content(c) {}
    virtual ~Message() {}

    virtual void send() = 0;
};

/**
 * Refined Abstraction: Text Message
 */
class TextMessage : public Message {
public:
    TextMessage(std::shared_ptr<MessageSender> s, const std::string& r, const std::string& c)
        : Message(s, r, c) {}

    void send() override {
        std::println("");
        std::println("📤 Sending text message via {}", sender->getPlatformName());
        sender->sendMessage(recipient, content);
    }
};

/**
 * Refined Abstraction: Encrypted Message
 */
class EncryptedMessage : public Message {
public:
    EncryptedMessage(std::shared_ptr<MessageSender> s, const std::string& r, const std::string& c)
        : Message(s, r, c) {}

    void send() override {
        std::println("");
        std::println("🔒 Encrypting and sending via {}", sender->getPlatformName());
        std::string encrypted = "[ENCRYPTED] " + content;
        sender->sendMessage(recipient, encrypted);
    }
};

/**
 * Refined Abstraction: Urgent Message
 */
class UrgentMessage : public Message {
public:
    UrgentMessage(std::shared_ptr<MessageSender> s, const std::string& r, const std::string& c)
        : Message(s, r, c) {}

    void send() override {
        std::println("");
        std::println("🚨 URGENT message via {}", sender->getPlatformName());
        std::string urgent = "⚠️ URGENT: " + content;
        sender->sendMessage(recipient, urgent);
    }
};

// ============================================
// CLIENT CODE & DEMONSTRATIONS
// ============================================

void demonstrateRemoteControl() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: REMOTE & DEVICE        ║");
    std::println("╚════════════════════════════════════╝");

    // Create devices
    auto tv = std::make_shared<TV>();
    auto radio = std::make_shared<Radio>();
    auto projector = std::make_shared<Projector>();

    // Basic remote with TV
    std::println("");
    std::println("--- Basic Remote with TV ---");
    RemoteControl basicRemote(tv);
    basicRemote.togglePower();
    basicRemote.volumeUp();
    basicRemote.channelUp();
    basicRemote.togglePower();

    // Advanced remote with Radio
    std::println("");
    std::println("--- Advanced Remote with Radio ---");
    AdvancedRemote advancedRemote(radio);
    advancedRemote.togglePower();
    advancedRemote.setChannel(105);
    advancedRemote.volumeUp();
    advancedRemote.mute();

    // Voice remote with Projector
    std::println("");
    std::println("--- Voice Remote with Projector ---");
    VoiceRemote voiceRemote(projector);
    voiceRemote.voiceCommand("turn on");
    voiceRemote.voiceCommand("louder");
    voiceRemote.channelUp();

    std::println("");
    std::println("💡 Notice: Same remote works with different devices!");
    std::println("   Abstraction (Remote) is independent of Implementation (Device)");
}

void demonstrateShapeRendering() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: SHAPE & RENDERING      ║");
    std::println("╚════════════════════════════════════╝");

    // Create renderers
    auto vectorRenderer = std::make_shared<VectorRenderer>();
    auto rasterRenderer = std::make_shared<RasterRenderer>();
    auto openglRenderer = std::make_shared<OpenGLRenderer>();

    // Circle with different renderers
    std::println("");
    std::println("--- Circle with Vector Renderer ---");
    Circle circle1(vectorRenderer, 10, 10, 5);
    circle1.draw();
    circle1.resize(7);
    circle1.draw();

    std::println("");
    std::println("--- Circle with Raster Renderer ---");
    Circle circle2(rasterRenderer, 20, 20, 8);
    circle2.draw();

    std::println("");
    std::println("--- Circle with OpenGL Renderer ---");
    Circle circle3(openglRenderer, 30, 30, 10);
    circle3.draw();

    // Square with different renderers
    std::println("");
    std::println("--- Square with Vector Renderer ---");
    Square square1(vectorRenderer, 0, 0, 10);
    square1.draw();
    square1.move(5, 5);
    square1.draw();

    std::println("");
    std::println("--- Square with OpenGL Renderer ---");
    Square square2(openglRenderer, 15, 15, 12);
    square2.draw();

    std::println("");
    std::println("💡 Notice: Shapes work with any renderer!");
    std::println("   Shape hierarchy is independent of Renderer hierarchy");
}

void demonstrateMessaging() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: MESSAGE SENDER         ║");
    std::println("╚════════════════════════════════════╝");

    // Create senders
    auto email = std::make_shared<EmailSender>();
    auto sms = std::make_shared<SMSSender>();
    auto slack = std::make_shared<SlackSender>();

    // Text message via different platforms
    std::println("");
    std::println("--- Text Message via Email ---");
    TextMessage msg1(email, "user@example.com", "Hello from email!");
    msg1.send();

    std::println("");
    std::println("--- Text Message via SMS ---");
    TextMessage msg2(sms, "+1234567890", "Hello from SMS!");
    msg2.send();

    std::println("");
    std::println("--- Encrypted Message via Slack ---");
    EncryptedMessage msg3(slack, "general", "Secret information");
    msg3.send();

    std::println("");
    std::println("--- Urgent Message via Email ---");
    UrgentMessage msg4(email, "boss@company.com", "Server is down!");
    msg4.send();

    std::println("");
    std::println("--- Urgent Message via SMS ---");
    UrgentMessage msg5(sms, "+9876543210", "Emergency meeting now!");
    msg5.send();

    std::println("");
    std::println("💡 Notice: Message types work with any sender!");
    std::println("   Message abstraction is independent of Sender implementation");
}

// ============================================
// MAIN - DEMONSTRATING THE PATTERN
// ============================================

int main() {
    std::println("========================================");
    std::println("BRIDGE PATTERN DEMO");
    std::println("========================================");

    demonstrateRemoteControl();
    demonstrateShapeRendering();
    demonstrateMessaging();

    return 0;
}
