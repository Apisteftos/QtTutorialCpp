# Open/Closed Principle (OCP)

## 📚 Overview

The **Open/Closed Principle** states:

> **"Software entities should be OPEN for extension, but CLOSED for modification."**  
> — Bertrand Meyer

### What It Means

- **OPEN for extension:** You can add new functionality
- **CLOSED for modification:** You don't change existing code

### Why It Matters

✅ **Stability** - Existing code doesn't change  
✅ **Safety** - Less risk of breaking working features  
✅ **Testability** - No need to retest old code  
✅ **Flexibility** - Easy to add new behaviors  
✅ **Maintainability** - Changes are isolated  

### The Core Idea

```
Instead of:
    Modifying existing code to add features

Do this:
    Extend behavior through inheritance/composition
```

---

## 🚨 Violation Signs

How to spot OCP violations:

- 🚨 `switch` statements on type/enum
- 🚨 `if-else` chains checking types
- 🚨 Modifying class to add new behavior
- 🚨 Ripple effect when adding features
- 🚨 Need to retest everything when adding something new

---

## 📖 Classic Example: Shape Area

### ❌ **Bad: Violates OCP**

```cpp
enum class ShapeType { CIRCLE, RECTANGLE, TRIANGLE };

class Shape {
public:
    ShapeType type;
    double radius;        // For circle
    double width, height; // For rectangle
    double base, h;       // For triangle
};

class AreaCalculator {
public:
    double calculateArea(const Shape& shape) {
        // 🚨 VIOLATION: Must modify this for each new shape!
        switch (shape.type) {
            case ShapeType::CIRCLE:
                return 3.14159 * shape.radius * shape.radius;
            case ShapeType::RECTANGLE:
                return shape.width * shape.height;
            case ShapeType::TRIANGLE:
                return 0.5 * shape.base * shape.h;
        }
        // Add Pentagon? → Modify this switch!
    }
};
```

**Problems:**
- Add Pentagon → Modify `calculateArea()`
- Add Hexagon → Modify `calculateArea()` again
- Risk breaking existing calculations
- Must retest entire class

### ✅ **Good: Follows OCP**

```cpp
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

// Easy to add new shapes!
class Pentagon : public Shape {
private:
    double side;
public:
    Pentagon(double s) : side(s) {}
    double calculateArea() const override {
        return 1.72 * side * side;
    }
};

// Calculator doesn't change!
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
```

**Benefits:**
- ✅ Add Pentagon → Just create new class
- ✅ No changes to existing code
- ✅ No risk of breaking Circle, Rectangle
- ✅ Only test Pentagon

---

## 💳 Real-World Example: Payment Processing

### ❌ **Bad: Switch on Payment Type**

```cpp
enum class PaymentType { CREDIT_CARD, PAYPAL, BITCOIN };

class PaymentProcessor {
public:
    void processPayment(PaymentType type, double amount) {
        switch (type) {
            case PaymentType::CREDIT_CARD:
                // Credit card logic
                break;
            case PaymentType::PAYPAL:
                // PayPal logic
                break;
            case PaymentType::BITCOIN:
                // Bitcoin logic
                break;
            // Add Apple Pay? → Modify this switch!
        }
    }
};
```

### ✅ **Good: Strategy Pattern**

```cpp
class PaymentMethod {
public:
    virtual ~PaymentMethod() = default;
    virtual void processPayment(double amount) = 0;
};

class CreditCardPayment : public PaymentMethod {
public:
    void processPayment(double amount) override {
        cout << "Processing $" << amount << " via Credit Card\n";
    }
};

class PayPalPayment : public PaymentMethod {
public:
    void processPayment(double amount) override {
        cout << "Processing $" << amount << " via PayPal\n";
    }
};

// Easy to add!
class ApplePayPayment : public PaymentMethod {
public:
    void processPayment(double amount) override {
        cout << "Processing $" << amount << " via Apple Pay\n";
    }
};

class PaymentProcessor {
public:
    void process(PaymentMethod& method, double amount) {
        method.processPayment(amount);
    }
};
```

---

## 🔔 Notification System Example

### ❌ **Bad: if-else Chain**

```cpp
class NotificationService {
public:
    void send(const string& type, const string& message) {
        if (type == "email") {
            // Send email
        }
        else if (type == "sms") {
            // Send SMS
        }
        else if (type == "push") {
            // Send push notification
        }
        // Add Slack? → Modify this if-else chain!
    }
};
```

### ✅ **Good: Polymorphic Channels**

```cpp
class Notification {
public:
    virtual ~Notification() = default;
    virtual void send(const string& message) = 0;
};

class EmailNotification : public Notification {
public:
    void send(const string& message) override {
        cout << "Email: " << message << "\n";
    }
};

class SMSNotification : public Notification {
public:
    void send(const string& message) override {
        cout << "SMS: " << message << "\n";
    }
};

// Easy to add!
class SlackNotification : public Notification {
public:
    void send(const string& message) override {
        cout << "Slack: " << message << "\n";
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
```

---

## 🎯 How to Achieve OCP

### 1. Abstraction (Interfaces)

```cpp
class PaymentMethod {
public:
    virtual void process(double amount) = 0;
};
```

### 2. Inheritance (Extend Behavior)

```cpp
class CreditCard : public PaymentMethod {
    void process(double amount) override {
        // Implementation
    }
};
```

### 3. Polymorphism (Dynamic Dispatch)

```cpp
PaymentMethod* method = new CreditCard();
method->process(100);  // Calls CreditCard::process
```

### 4. Strategy Pattern

```cpp
class Context {
private:
    Strategy* strategy;
public:
    void setStrategy(Strategy* s) { strategy = s; }
    void execute() { strategy->algorithm(); }
};
```

### 5. Template Method Pattern

```cpp
class Algorithm {
public:
    void execute() {
        step1();
        step2();  // Customizable
        step3();
    }
protected:
    virtual void step2() = 0;
};
```

---

## ✅ Benefits of OCP

### 1. Stability

```
Existing code:  ✅ Never changes
New features:   ✅ Added as extensions
Working code:   ✅ Stays working
```

### 2. Safety

- Less risk of breaking features
- No unintended side effects
- "If it works, don't touch it!"

### 3. Testability

- Only test new extensions
- No need to retest old code
- Faster test cycles

### 4. Flexibility

- Easy plugin architecture
- Runtime configuration
- Swap implementations easily

---

## ⚠️ Tradeoffs

### Costs of OCP

1. **More Complexity**
   - More classes and interfaces
   - Deeper inheritance hierarchies
   - Need to design abstractions

2. **Performance Overhead**
   - Virtual function calls
   - Slight runtime cost
   - Memory for vtables

3. **Risk of Over-Engineering**
   - Too many abstractions
   - Premature generalization
   - YAGNI violations

### When to Apply OCP

✅ **Apply OCP when:**
- You expect variations in behavior
- You have switch/if-else on types
- Code is stable and widely used
- Adding features requires code changes

❌ **Don't apply when:**
- Simple code unlikely to change
- Performance is critical
- Over-engineering simple logic
- Following YAGNI (You Aren't Gonna Need It)

---

## 🎨 Design Patterns That Support OCP

### Strategy Pattern

```cpp
// Define family of algorithms
class SortStrategy {
public:
    virtual void sort(vector<int>& data) = 0;
};

class QuickSort : public SortStrategy { };
class MergeSort : public SortStrategy { };

// Easy to add new sorting algorithms
class HeapSort : public SortStrategy { };
```

### Template Method Pattern

```cpp
class DataProcessor {
public:
    void process() {
        readData();
        processData();  // Customizable
        writeData();
    }
protected:
    virtual void processData() = 0;
};
```

### Factory Pattern

```cpp
class ShapeFactory {
public:
    virtual unique_ptr<Shape> createShape() = 0;
};

// Easy to add new factories
class CircleFactory : public ShapeFactory { };
class RectangleFactory : public ShapeFactory { };
```

---

## 📊 Before & After Comparison

### Before (Violates OCP)

```cpp
class ReportGenerator {
    void generate(string format, string data) {
        if (format == "PDF") {
            // PDF generation
        }
        else if (format == "HTML") {
            // HTML generation
        }
        // Add XML? → Modify this function
    }
};
```

**What happens when adding XML:**
1. Modify `generate()` function
2. Risk breaking PDF/HTML
3. Must retest everything
4. Changes ripple through code

### After (Follows OCP)

```cpp
class ReportFormatter {
public:
    virtual void generate(const string& data) = 0;
};

class PDFFormatter : public ReportFormatter { };
class HTMLFormatter : public ReportFormatter { };
class XMLFormatter : public ReportFormatter { };  // Just add!

class ReportGenerator {
    void generate(ReportFormatter& formatter, const string& data) {
        formatter.generate(data);
    }
};
```

**What happens when adding XML:**
1. Create `XMLFormatter` class
2. No changes to existing code
3. Only test XML formatter
4. Zero risk to PDF/HTML

---

## 🔍 Common Mistakes

### 1. Premature Abstraction

```cpp
// ❌ BAD: Over-engineering
class DataGetter {
    virtual Data getData() = 0;
};

class SimpleDataGetter : public DataGetter {
    Data getData() override { return data; }
};

// ✅ GOOD: Keep it simple if you only have one implementation
class DataManager {
    Data getData() { return data; }
};
```

### 2. Wrong Abstraction

```cpp
// ❌ BAD: Leaky abstraction
class PaymentMethod {
    virtual void processCreditCard() = 0;  // Too specific!
};

// ✅ GOOD: Generic abstraction
class PaymentMethod {
    virtual void process(double amount) = 0;  // Generic!
};
```

### 3. Breaking Abstraction

```cpp
// ❌ BAD: Checking concrete type defeats OCP
void process(PaymentMethod* method) {
    if (dynamic_cast<CreditCard*>(method)) {
        // Special handling
    }
}

// ✅ GOOD: Use polymorphism
void process(PaymentMethod* method) {
    method->process(100);  // Polymorphic call
}
```

---

## 💡 Key Takeaways

1. **Extend, don't modify** - Add new code, don't change old code
2. **Use abstraction** - Interfaces enable extension
3. **Think ahead** - Design for likely variations
4. **But don't over-engineer** - Apply when needed, not speculatively
5. **Test new code only** - Old code stays stable
6. **Use design patterns** - Strategy, Template Method, Factory

---

## 🎯 Quick Reference

### OCP Checklist

When adding new functionality:
- [ ] Can I add it without modifying existing classes?
- [ ] Do I have appropriate abstractions?
- [ ] Am I using polymorphism effectively?
- [ ] Will existing tests still pass?
- [ ] Am I avoiding switch/if-else on types?

### Red Flags (OCP Violations)

- ❌ `switch (type)` or `if (type == ...)`
- ❌ Modifying classes to add features
- ❌ Ripple effects when adding features
- ❌ Need to retest everything
- ❌ Many `#ifdef` or conditional compilation

### Green Flags (OCP Compliance)

- ✅ Adding new class, not modifying old ones
- ✅ Using inheritance/polymorphism
- ✅ Plugin architecture
- ✅ Only test new extensions
- ✅ Clear abstractions

---

## 💭 Quotes

> "Software entities should be open for extension,  
> but closed for modification."  
> — Bertrand Meyer

> "You should be able to extend a classes behavior,  
> without modifying it."  
> — Robert C. Martin

> "Open-Closed Principle is the most important principle  
> of object-oriented design."  
> — Robert C. Martin

---

## 🚀 Next Steps

After mastering OCP, explore:
- **Liskov Substitution Principle** - Subtypes must be substitutable
- **Interface Segregation Principle** - Many small interfaces
- **Dependency Inversion Principle** - Depend on abstractions
- **Design Patterns** - Strategy, Template Method, Factory

---

## 📚 Resources

- **"Agile Software Development" by Robert C. Martin** - OCP chapter
- **"Design Patterns" by Gang of Four** - Strategy pattern
- **"Clean Architecture" by Robert C. Martin** - Architecture and OCP
- **C++ Core Guidelines** - Abstract classes and interfaces