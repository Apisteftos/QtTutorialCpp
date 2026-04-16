#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <cmath>
using namespace std;

// ============================================
// OPEN/CLOSED PRINCIPLE (OCP)
// ============================================

/*
OPEN/CLOSED PRINCIPLE:
    "Software entities (classes, modules, functions) should be
     OPEN for extension, but CLOSED for modification."
    — Bertrand Meyer

WHAT IT MEANS:
    • OPEN for extension: Can add new functionality
    • CLOSED for modification: Don't change existing code

WHY IT MATTERS:
    ✅ Add features without breaking existing code
    ✅ Reduce risk of bugs
    ✅ Easier testing (don't retest old code)
    ✅ Better stability
    ✅ Follows "Don't touch working code"

HOW TO ACHIEVE:
    • Abstraction (interfaces, abstract classes)
    • Inheritance (extend behavior)
    • Polymorphism (dynamic dispatch)
    • Composition (plugin architecture)
    • Strategy pattern
    • Template Method pattern

VIOLATION SIGNS:
    🚨 if-else chains checking types
    🚨 switch statements on type
    🚨 Modifying class to add new behavior
    🚨 Ripple effect of changes
*/

// ============================================
// EXAMPLE 1: SHAPE AREA CALCULATION (Classic)
// ============================================

namespace bad_example1 {
// ❌ BAD: Violates OCP - Must modify for new shapes

enum class ShapeType {
    CIRCLE,
    RECTANGLE,
    TRIANGLE
};

class Shape {
public:
    ShapeType type;
    double radius;        // For circle
    double width, height; // For rectangle
    double base, h;       // For triangle

    Shape(ShapeType t) : type(t), radius(0), width(0), height(0), base(0), h(0) {}
};

class AreaCalculator {
public:
    double calculateArea(const Shape& shape) {
        // 🚨 VIOLATION: Must modify this function for each new shape!
        switch (shape.type) {
        case ShapeType::CIRCLE:
            return 3.14159 * shape.radius * shape.radius;

        case ShapeType::RECTANGLE:
            return shape.width * shape.height;

        case ShapeType::TRIANGLE:
            return 0.5 * shape.base * shape.h;

        default:
            return 0.0;
        }
        // What if we add Pentagon? Hexagon? → Modify this code!
    }
};
}

namespace good_example1 {
// ✅ GOOD: Follows OCP - Can extend without modification

class Shape {
public:
    virtual ~Shape() = default;
    virtual double calculateArea() const = 0;
};

class Circle : public Shape {
private:
    double radius;

public:
    Circle(double r) : radius(r) {}

    double calculateArea() const override {
        return 3.14159 * radius * radius;
    }
};

class Rectangle : public Shape {
private:
    double width, height;

public:
    Rectangle(double w, double h) : width(w), height(h) {}

    double calculateArea() const override {
        return width * height;
    }
};

class Triangle : public Shape {
private:
    double base, height;

public:
    Triangle(double b, double h) : base(b), height(h) {}

    double calculateArea() const override {
        return 0.5 * base * height;
    }
};

// Easy to add new shapes without modifying existing code!
class Pentagon : public Shape {
private:
    double side;

public:
    Pentagon(double s) : side(s) {}

    double calculateArea() const override {
        return 1.72 * side * side;  // Approximate formula
    }
};

// Calculator doesn't need to know about specific shapes!
class AreaCalculator {
public:
    double calculateTotalArea(const vector<unique_ptr<Shape>>& shapes) {
        double total = 0.0;
        for (const auto& shape : shapes) {
            total += shape->calculateArea();
        }
        return total;
    }
};
}

void example1_shape_calculation() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 1: SHAPE AREA CALCULATION (Classic)   ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (Violates OCP):\n";
    cout << "```cpp\n";
    cout << "class AreaCalculator {\n";
    cout << "    double calculateArea(const Shape& shape) {\n";
    cout << "        switch (shape.type) {\n";
    cout << "            case CIRCLE: return 3.14 * r * r;\n";
    cout << "            case RECTANGLE: return w * h;\n";
    cout << "            case TRIANGLE: return 0.5 * b * h;\n";
    cout << "            // Add Pentagon? → Modify this code!\n";
    cout << "        }\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "```\n";

    cout << "\n🚨 PROBLEMS:\n";
    cout << "   • Must modify AreaCalculator for every new shape\n";
    cout << "   • Violates OCP (not closed for modification)\n";
    cout << "   • Risk of breaking existing calculations\n";
    cout << "   • Must retest entire class\n";

    cout << "\n✅ GOOD CODE (Follows OCP):\n";
    cout << "```cpp\n";
    cout << "class Shape {\n";
    cout << "public:\n";
    cout << "    virtual double calculateArea() const = 0;\n";
    cout << "};\n\n";
    cout << "class Circle : public Shape {\n";
    cout << "    double calculateArea() const override { return 3.14*r*r; }\n";
    cout << "};\n\n";
    cout << "class Rectangle : public Shape {\n";
    cout << "    double calculateArea() const override { return w*h; }\n";
    cout << "};\n\n";
    cout << "// Add new shape? Just inherit, don't modify!\n";
    cout << "class Pentagon : public Shape {\n";
    cout << "    double calculateArea() const override { return 1.72*s*s; }\n";
    cout << "};\n";
    cout << "```\n";

    cout << "\n🔧 DEMONSTRATION:\n";

    vector<unique_ptr<good_example1::Shape>> shapes;
    shapes.push_back(make_unique<good_example1::Circle>(5.0));
    shapes.push_back(make_unique<good_example1::Rectangle>(4.0, 6.0));
    shapes.push_back(make_unique<good_example1::Triangle>(3.0, 4.0));
    shapes.push_back(make_unique<good_example1::Pentagon>(5.0));

    good_example1::AreaCalculator calculator;
    double total = calculator.calculateTotalArea(shapes);

    cout << "   Total area of all shapes: " << total << "\n";

    cout << "\n✨ BENEFITS:\n";
    cout << "   ✅ Add Pentagon without modifying AreaCalculator\n";
    cout << "   ✅ No risk of breaking existing shapes\n";
    cout << "   ✅ No need to retest old code\n";
    cout << "   ✅ Each shape encapsulates its logic\n";
}

// ============================================
// EXAMPLE 2: PAYMENT PROCESSING (Real-World)
// ============================================

namespace bad_example2 {
// ❌ BAD: Violates OCP

enum class PaymentType {
    CREDIT_CARD,
    PAYPAL,
    BITCOIN
};

class PaymentProcessor {
public:
    void processPayment(PaymentType type, double amount) {
        // 🚨 VIOLATION: Must modify for each new payment method!
        switch (type) {
        case PaymentType::CREDIT_CARD:
            cout << "Processing $" << amount << " via Credit Card\n";
            // Credit card logic
            break;

        case PaymentType::PAYPAL:
            cout << "Processing $" << amount << " via PayPal\n";
            // PayPal logic
            break;

        case PaymentType::BITCOIN:
            cout << "Processing $" << amount << " via Bitcoin\n";
            // Bitcoin logic
            break;

            // Add Apple Pay? → Modify this code!
        }
    }
};
}

namespace good_example2 {
// ✅ GOOD: Follows OCP

class PaymentMethod {
public:
    virtual ~PaymentMethod() = default;
    virtual void processPayment(double amount) = 0;
};

class CreditCardPayment : public PaymentMethod {
public:
    void processPayment(double amount) override {
        cout << "   💳 Processing $" << amount << " via Credit Card\n";
        // Credit card specific logic
    }
};

class PayPalPayment : public PaymentMethod {
public:
    void processPayment(double amount) override {
        cout << "   🅿️  Processing $" << amount << " via PayPal\n";
        // PayPal specific logic
    }
};

class BitcoinPayment : public PaymentMethod {
public:
    void processPayment(double amount) override {
        cout << "   ₿  Processing $" << amount << " via Bitcoin\n";
        // Bitcoin specific logic
    }
};

// Easy to add new payment methods!
class ApplePayPayment : public PaymentMethod {
public:
    void processPayment(double amount) override {
        cout << "   🍎 Processing $" << amount << " via Apple Pay\n";
        // Apple Pay specific logic
    }
};

class GooglePayPayment : public PaymentMethod {
public:
    void processPayment(double amount) override {
        cout << "   🔵 Processing $" << amount << " via Google Pay\n";
        // Google Pay specific logic
    }
};

class PaymentProcessor {
public:
    void process(PaymentMethod& method, double amount) {
        method.processPayment(amount);
    }
};
}

void example2_payment_processing() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 2: PAYMENT PROCESSING (Real-World)    ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD: Switch statement on payment type\n";
    cout << "   • Add Apple Pay → Modify switch statement\n";
    cout << "   • Risk breaking existing payments\n";
    cout << "   • Must retest all payment methods\n";

    cout << "\n✅ GOOD: Strategy pattern\n";
    cout << "   • Add Apple Pay → Create new class\n";
    cout << "   • No changes to existing code\n";
    cout << "   • Only test new payment method\n";

    cout << "\n🔧 DEMONSTRATION:\n";

    good_example2::PaymentProcessor processor;

    good_example2::CreditCardPayment creditCard;
    good_example2::PayPalPayment paypal;
    good_example2::ApplePayPayment applePay;

    processor.process(creditCard, 100.0);
    processor.process(paypal, 50.0);
    processor.process(applePay, 75.0);

    cout << "\n💡 KEY INSIGHT:\n";
    cout << "   \"New payment method = New class, not modified switch!\"\n";
}

// ============================================
// EXAMPLE 3: NOTIFICATION SYSTEM
// ============================================

namespace bad_example3 {
// ❌ BAD: Violates OCP

class NotificationService {
public:
    void send(const string& type, const string& message) {
        // 🚨 VIOLATION: if-else chain
        if (type == "email") {
            cout << "📧 Sending email: " << message << "\n";
        }
        else if (type == "sms") {
            cout << "📱 Sending SMS: " << message << "\n";
        }
        else if (type == "push") {
            cout << "🔔 Sending push notification: " << message << "\n";
        }
        // Add Slack? → Modify this code!
    }
};
}

namespace good_example3 {
// ✅ GOOD: Follows OCP

class Notification {
public:
    virtual ~Notification() = default;
    virtual void send(const string& message) = 0;
};

class EmailNotification : public Notification {
public:
    void send(const string& message) override {
        cout << "   📧 Email: " << message << "\n";
    }
};

class SMSNotification : public Notification {
public:
    void send(const string& message) override {
        cout << "   📱 SMS: " << message << "\n";
    }
};

class PushNotification : public Notification {
public:
    void send(const string& message) override {
        cout << "   🔔 Push: " << message << "\n";
    }
};

// Easy to add!
class SlackNotification : public Notification {
public:
    void send(const string& message) override {
        cout << "   💬 Slack: " << message << "\n";
    }
};

class NotificationService {
private:
    vector<unique_ptr<Notification>> channels;

public:
    void addChannel(unique_ptr<Notification> channel) {
        channels.push_back(move(channel));
    }

    void sendAll(const string& message) {
        for (auto& channel : channels) {
            channel->send(message);
        }
    }
};
}

void example3_notification_system() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 3: NOTIFICATION SYSTEM                 ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD: if-else chain on notification type\n";
    cout << "✅ GOOD: Polymorphic notification channels\n";

    cout << "\n🔧 DEMONSTRATION:\n";
    cout << "Sending notifications through multiple channels:\n";

    good_example3::NotificationService service;

    service.addChannel(make_unique<good_example3::EmailNotification>());
    service.addChannel(make_unique<good_example3::SMSNotification>());
    service.addChannel(make_unique<good_example3::PushNotification>());
    service.addChannel(make_unique<good_example3::SlackNotification>());

    service.sendAll("Your order has been shipped!");

    cout << "\n✨ BENEFITS:\n";
    cout << "   ✅ Add Slack notification → Just create SlackNotification\n";
    cout << "   ✅ No modification to NotificationService\n";
    cout << "   ✅ Can add channels at runtime\n";
}

// ============================================
// EXAMPLE 4: REPORT GENERATION
// ============================================

namespace bad_example4 {
// ❌ BAD: Violates OCP

class ReportGenerator {
public:
    void generate(const string& format, const string& data) {
        // 🚨 VIOLATION: Must modify for new formats
        if (format == "PDF") {
            cout << "Generating PDF report...\n";
            cout << "PDF: " << data << "\n";
        }
        else if (format == "HTML") {
            cout << "Generating HTML report...\n";
            cout << "<html>" << data << "</html>\n";
        }
        else if (format == "CSV") {
            cout << "Generating CSV report...\n";
            cout << "csv," << data << "\n";
        }
        // Add XML? → Modify this code!
    }
};
}

namespace good_example4 {
// ✅ GOOD: Follows OCP

class ReportFormatter {
public:
    virtual ~ReportFormatter() = default;
    virtual void generate(const string& data) = 0;
};

class PDFFormatter : public ReportFormatter {
public:
    void generate(const string& data) override {
        cout << "   📄 PDF Report: " << data << "\n";
    }
};

class HTMLFormatter : public ReportFormatter {
public:
    void generate(const string& data) override {
        cout << "   🌐 HTML Report: <html>" << data << "</html>\n";
    }
};

class CSVFormatter : public ReportFormatter {
public:
    void generate(const string& data) override {
        cout << "   📊 CSV Report: csv," << data << "\n";
    }
};

// Easy to add!
class XMLFormatter : public ReportFormatter {
public:
    void generate(const string& data) override {
        cout << "   📑 XML Report: <xml>" << data << "</xml>\n";
    }
};

class ReportGenerator {
public:
    void generate(ReportFormatter& formatter, const string& data) {
        formatter.generate(data);
    }
};
}

void example4_report_generation() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 4: REPORT GENERATION                   ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD: if-else chain checking format\n";
    cout << "✅ GOOD: Strategy pattern with formatters\n";

    cout << "\n🔧 DEMONSTRATION:\n";

    good_example4::ReportGenerator generator;
    string reportData = "Sales Report Q4 2024";

    good_example4::PDFFormatter pdf;
    good_example4::HTMLFormatter html;
    good_example4::XMLFormatter xml;

    generator.generate(pdf, reportData);
    generator.generate(html, reportData);
    generator.generate(xml, reportData);

    cout << "\n💡 KEY INSIGHT:\n";
    cout << "   \"New format = New formatter class, no modifications!\"\n";
}

// ============================================
// EXAMPLE 5: HOW TO ACHIEVE OCP
// ============================================

void example5_how_to_achieve() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 5: HOW TO ACHIEVE OCP                  ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n🎯 TECHNIQUES TO ACHIEVE OCP:\n";

    cout << "\n1. ABSTRACTION (Interfaces/Abstract Classes):\n";
    cout << "   class PaymentMethod {\n";
    cout << "   public:\n";
    cout << "       virtual void process(double amount) = 0;\n";
    cout << "   };\n";

    cout << "\n2. INHERITANCE (Extend behavior):\n";
    cout << "   class CreditCard : public PaymentMethod {\n";
    cout << "       void process(double amount) override { }\n";
    cout << "   };\n";

    cout << "\n3. POLYMORPHISM (Dynamic dispatch):\n";
    cout << "   PaymentMethod* method = new CreditCard();\n";
    cout << "   method->process(100);  // Calls CreditCard::process\n";

    cout << "\n4. STRATEGY PATTERN:\n";
    cout << "   class Context {\n";
    cout << "       Strategy* strategy;\n";
    cout << "   public:\n";
    cout << "       void setStrategy(Strategy* s) { strategy = s; }\n";
    cout << "       void execute() { strategy->algorithm(); }\n";
    cout << "   };\n";

    cout << "\n5. TEMPLATE METHOD PATTERN:\n";
    cout << "   class Algorithm {\n";
    cout << "   public:\n";
    cout << "       void execute() {\n";
    cout << "           step1(); step2(); step3();\n";
    cout << "       }\n";
    cout << "   protected:\n";
    cout << "       virtual void step2() = 0;  // Customize this\n";
    cout << "   };\n";

    cout << "\n📊 COMPARISON:\n";
    cout << "\n   ❌ CLOSED for Extension (Hard to extend):\n";
    cout << "      • Final classes\n";
    cout << "      • Private/hidden interfaces\n";
    cout << "      • Tight coupling\n";

    cout << "\n   ✅ OPEN for Extension (Easy to extend):\n";
    cout << "      • Abstract base classes\n";
    cout << "      • Virtual functions\n";
    cout << "      • Plugin architecture\n";
}

// ============================================
// EXAMPLE 6: BENEFITS & TRADEOFFS
// ============================================

void example6_benefits_tradeoffs() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 6: BENEFITS & TRADEOFFS               ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n✅ BENEFITS OF OCP:\n";

    cout << "\n1. STABILITY:\n";
    cout << "   • Existing code doesn't change\n";
    cout << "   • Less risk of breaking features\n";
    cout << "   • \"If it works, don't touch it!\"\n";

    cout << "\n2. MAINTAINABILITY:\n";
    cout << "   • Add features without fear\n";
    cout << "   • No need to understand all old code\n";
    cout << "   • Changes are isolated\n";

    cout << "\n3. TESTABILITY:\n";
    cout << "   • No need to retest old code\n";
    cout << "   • Test only new extensions\n";
    cout << "   • Faster test cycles\n";

    cout << "\n4. FLEXIBILITY:\n";
    cout << "   • Easy to add new behaviors\n";
    cout << "   • Plugin architecture\n";
    cout << "   • Runtime configuration\n";

    cout << "\n⚠️  TRADEOFFS:\n";

    cout << "\n1. COMPLEXITY:\n";
    cout << "   • More classes and interfaces\n";
    cout << "   • Deeper inheritance hierarchies\n";
    cout << "   • Need to design abstractions upfront\n";

    cout << "\n2. OVERHEAD:\n";
    cout << "   • Virtual function calls\n";
    cout << "   • Slight performance cost\n";
    cout << "   • More memory for vtables\n";

    cout << "\n3. OVER-ENGINEERING:\n";
    cout << "   • Can lead to too many abstractions\n";
    cout << "   • YAGNI violation (You Aren't Gonna Need It)\n";
    cout << "   • Balance needed\n";

    cout << "\n💡 WHEN TO APPLY OCP:\n";
    cout << "   ✅ You expect variations in behavior\n";
    cout << "   ✅ You have if-else/switch on types\n";
    cout << "   ✅ Adding features requires code changes\n";
    cout << "   ✅ Code is stable and used by many\n";

    cout << "\n   ❌ Simple, unlikely to change\n";
    cout << "   ❌ Performance critical (virtual calls overhead)\n";
    cout << "   ❌ Over-engineering simple code\n";

    cout << "\n🎯 GOLDEN RULE:\n";
    cout << "   \"Make abstractions when you need them,\n";
    cout << "    not when you might need them!\"\n";
}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║          OPEN/CLOSED PRINCIPLE (OCP)                  ║\n";
    cout << "║                                                       ║\n";
    cout << "║  \"Open for extension, closed for modification\"        ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n";

    example1_shape_calculation();
    example2_payment_processing();
    example3_notification_system();
    example4_report_generation();
    example5_how_to_achieve();
    example6_benefits_tradeoffs();

    cout << "\n\n";
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║           🎉 OPEN/CLOSED PRINCIPLE COMPLETE! 🎉       ║\n";
    cout << "║                                                       ║\n";
    cout << "║  Key Takeaway: Extend behavior through inheritance,  ║\n";
    cout << "║  not by modifying existing code!                     ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n\n";

    return 0;
}
