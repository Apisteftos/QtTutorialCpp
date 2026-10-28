#include <vector>
#include <string>
#include <memory>
#include <cmath>
#include <print>

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
    double calculateTotalArea(const std::vector<std::unique_ptr<Shape>>& shapes) {
        double total = 0.0;
        for (const auto& shape : shapes) {
            total += shape->calculateArea();
        }
        return total;
    }
};
}

void example1_shape_calculation() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 1: SHAPE AREA CALCULATION (Classic)    ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::vector<std::unique_ptr<good_example1::Shape>> shapes;
    shapes.push_back(std::make_unique<good_example1::Circle>(5.0));
    shapes.push_back(std::make_unique<good_example1::Rectangle>(4.0, 6.0));
    shapes.push_back(std::make_unique<good_example1::Triangle>(3.0, 4.0));
    shapes.push_back(std::make_unique<good_example1::Pentagon>(5.0));

    good_example1::AreaCalculator calculator;
    double total = calculator.calculateTotalArea(shapes);

    std::println("   Total area of all shapes: {}", total);

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
            std::println("Processing ${} via Credit Card", amount);
            // Credit card logic
            break;

        case PaymentType::PAYPAL:
            std::println("Processing ${} via PayPal", amount);
            // PayPal logic
            break;

        case PaymentType::BITCOIN:
            std::println("Processing ${} via Bitcoin", amount);
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
        std::println("   💳 Processing ${} via Credit Card", amount);
        // Credit card specific logic
    }
};

class PayPalPayment : public PaymentMethod {
public:
    void processPayment(double amount) override {
        std::println("   🅿️  Processing ${} via PayPal", amount);
        // PayPal specific logic
    }
};

class BitcoinPayment : public PaymentMethod {
public:
    void processPayment(double amount) override {
        std::println("   ₿  Processing ${} via Bitcoin", amount);
        // Bitcoin specific logic
    }
};

// Easy to add new payment methods!
class ApplePayPayment : public PaymentMethod {
public:
    void processPayment(double amount) override {
        std::println("   🍎 Processing ${} via Apple Pay", amount);
        // Apple Pay specific logic
    }
};

class GooglePayPayment : public PaymentMethod {
public:
    void processPayment(double amount) override {
        std::println("   🔵 Processing ${} via Google Pay", amount);
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
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 2: PAYMENT PROCESSING (Real-World)     ║");
    std::println("╚══════════════════════════════════════════════════╝");

    good_example2::PaymentProcessor processor;

    good_example2::CreditCardPayment creditCard;
    good_example2::PayPalPayment paypal;
    good_example2::ApplePayPayment applePay;

    processor.process(creditCard, 100.0);
    processor.process(paypal, 50.0);
    processor.process(applePay, 75.0);

    std::println("");
    std::println("💡 KEY INSIGHT:");
    std::println("   \"New payment method = New class, not modified switch!\"");
}

// ============================================
// EXAMPLE 3: NOTIFICATION SYSTEM
// ============================================

namespace bad_example3 {
// ❌ BAD: Violates OCP

class NotificationService {
public:
    void send(const std::string& type, const std::string& message) {
        // 🚨 VIOLATION: if-else chain
        if (type == "email") {
            std::println("📧 Sending email: {}", message);
        }
        else if (type == "sms") {
            std::println("📱 Sending SMS: {}", message);
        }
        else if (type == "push") {
            std::println("🔔 Sending push notification: {}", message);
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
    virtual void send(const std::string& message) = 0;
};

class EmailNotification : public Notification {
public:
    void send(const std::string& message) override {
        std::println("   📧 Email: {}", message);
    }
};

class SMSNotification : public Notification {
public:
    void send(const std::string& message) override {
        std::println("   📱 SMS: {}", message);
    }
};

class PushNotification : public Notification {
public:
    void send(const std::string& message) override {
        std::println("   🔔 Push: {}", message);
    }
};

// Easy to add!
class SlackNotification : public Notification {
public:
    void send(const std::string& message) override {
        std::println("   💬 Slack: {}", message);
    }
};

class NotificationService {
private:
    std::vector<std::unique_ptr<Notification>> channels;

public:
    void addChannel(std::unique_ptr<Notification> channel) {
        channels.push_back(move(channel));
    }

    void sendAll(const std::string& message) {
        for (auto& channel : channels) {
            channel->send(message);
        }
    }
};
}

void example3_notification_system() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 3: NOTIFICATION SYSTEM                 ║");
    std::println("╚══════════════════════════════════════════════════╝");

    good_example3::NotificationService service;

    service.addChannel(std::make_unique<good_example3::EmailNotification>());
    service.addChannel(std::make_unique<good_example3::SMSNotification>());
    service.addChannel(std::make_unique<good_example3::PushNotification>());
    service.addChannel(std::make_unique<good_example3::SlackNotification>());

    service.sendAll("Your order has been shipped!");

}

// ============================================
// EXAMPLE 4: REPORT GENERATION
// ============================================

namespace bad_example4 {
// ❌ BAD: Violates OCP

class ReportGenerator {
public:
    void generate(const std::string& format, const std::string& data) {
        // 🚨 VIOLATION: Must modify for new formats
        if (format == "PDF") {
            std::println("Generating PDF report...");
            std::println("PDF: {}", data);
        }
        else if (format == "HTML") {
            std::println("Generating HTML report...");
            std::println("<html>{}</html>", data);
        }
        else if (format == "CSV") {
            std::println("Generating CSV report...");
            std::println("csv,{}", data);
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
    virtual void generate(const std::string& data) = 0;
};

class PDFFormatter : public ReportFormatter {
public:
    void generate(const std::string& data) override {
        std::println("   📄 PDF Report: {}", data);
    }
};

class HTMLFormatter : public ReportFormatter {
public:
    void generate(const std::string& data) override {
        std::println("   🌐 HTML Report: <html>{}</html>", data);
    }
};

class CSVFormatter : public ReportFormatter {
public:
    void generate(const std::string& data) override {
        std::println("   📊 CSV Report: csv,{}", data);
    }
};

// Easy to add!
class XMLFormatter : public ReportFormatter {
public:
    void generate(const std::string& data) override {
        std::println("   📑 XML Report: <xml>{}</xml>", data);
    }
};

class ReportGenerator {
public:
    void generate(ReportFormatter& formatter, const std::string& data) {
        formatter.generate(data);
    }
};
}

void example4_report_generation() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 4: REPORT GENERATION                   ║");
    std::println("╚══════════════════════════════════════════════════╝");

    good_example4::ReportGenerator generator;
    std::string reportData = "Sales Report Q4 2024";

    good_example4::PDFFormatter pdf;
    good_example4::HTMLFormatter html;
    good_example4::XMLFormatter xml;

    generator.generate(pdf, reportData);
    generator.generate(html, reportData);
    generator.generate(xml, reportData);

    std::println("");
    std::println("💡 KEY INSIGHT:");
    std::println("   \"New format = New formatter class, no modifications!\"");
}

// ============================================
// EXAMPLE 5: HOW TO ACHIEVE OCP
// ============================================

void example5_how_to_achieve() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 5: HOW TO ACHIEVE OCP                  ║");
    std::println("╚══════════════════════════════════════════════════╝");

}

// ============================================
// EXAMPLE 6: BENEFITS & TRADEOFFS
// ============================================

void example6_benefits_tradeoffs() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 6: BENEFITS & TRADEOFFS                ║");
    std::println("╚══════════════════════════════════════════════════╝");

}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    std::println("╔═══════════════════════════════════════════════════════╗");
    std::println("║                                                       ║");
    std::println("║          OPEN/CLOSED PRINCIPLE (OCP)                  ║");
    std::println("║                                                       ║");
    std::println("║  \"Open for extension, closed for modification\"      ║");
    std::println("║                                                       ║");
    std::println("╚═══════════════════════════════════════════════════════╝");

    example1_shape_calculation();
    example2_payment_processing();
    example3_notification_system();
    example4_report_generation();
    example5_how_to_achieve();
    example6_benefits_tradeoffs();

    return 0;
}
