#include <print>
#include <string>
#include <vector>
#include <ctime>
#include <cctype>   // isupper, islower, isdigit
using namespace std;

// ============================================
// WHAT IS ENCAPSULATION?
// ============================================

/*
 * ENCAPSULATION is the bundling of data and methods that operate on that data
 * within a single unit (class), and restricting direct access to some of the
 * object's components.
 *
 * Key Concepts:
 * - Data Hiding: Private members are hidden from outside
 * - Controlled Access: Public interface to interact with private data
 * - Access Specifiers: public, private, protected
 *
 * Benefits:
 * - Security: Prevent unauthorized access
 * - Flexibility: Change implementation without breaking code
 * - Validation: Control how data is modified
 * - Maintenance: Easier to modify and debug
 */

// ============================================
// EXAMPLE 1: WITHOUT ENCAPSULATION (BAD!)
// ============================================

class BankAccountBad {
public:
    string accountNumber;
    double balance;  // ❌ Public - anyone can modify!

    BankAccountBad(string acc, double bal) : accountNumber(acc), balance(bal) { }
};

void demonstrateWithoutEncapsulation() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: WITHOUT ENCAPSULATION  ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    BankAccountBad account("123456", 1000.00);

    std::println("--- The Problem ---");
    std::println("Initial balance: ${}", account.balance);
    std::println("");

    std::println("❌ Anyone can modify balance directly:");
    account.balance = -5000.00;  // ❌ Negative balance!
    std::println("   Set to: ${}", account.balance);
    std::println("");

    account.balance = 999999999.99;  // ❌ Unrealistic amount!
    std::println("   Set to: ${}", account.balance);
    std::println("");
}

// ============================================
// EXAMPLE 2: WITH ENCAPSULATION (GOOD!)
// ============================================

class BankAccountGood {
private:
    // ✅ Private data - hidden from outside
    string accountNumber;
    double balance;

public:
    BankAccountGood(string acc, double bal) : accountNumber(acc), balance(bal) { }

    // ✅ Controlled access through public interface
    double getBalance() const {
        return balance;
    }

    bool deposit(double amount) {
        if (amount > 0) {  // ✅ Validation!
            balance += amount;
            std::println("✅ Deposited ${}", amount);
            return true;
        }
        std::println("❌ Invalid deposit amount");
        return false;
    }

    bool withdraw(double amount) {
        if (amount <= 0) {
            std::println("❌ Invalid withdrawal amount");
            return false;
        }
        if (amount > balance) {
            std::println("❌ Insufficient funds");
            return false;
        }
        balance -= amount;
        std::println("✅ Withdrew ${}", amount);
        return true;
    }

    void displayInfo() const {
        std::println("Account: {}", accountNumber);
        std::println("Balance: ${}", balance);
    }
};

void demonstrateWithEncapsulation() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: WITH ENCAPSULATION     ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    BankAccountGood account("123456", 1000.00);

    std::println("--- The Solution ---");
    account.displayInfo();

    std::println("");
    std::println("✅ Controlled modifications:");
    // account.balance = -5000;  // ❌ Error! balance is private

    account.deposit(500.00);
    account.withdraw(200.00);
    account.withdraw(2000.00);  // Will fail - insufficient funds

    std::println("");
    account.displayInfo();
}

// ============================================
// EXAMPLE 3: ACCESS SPECIFIERS
// ============================================

class AccessSpecifierDemo {
private:
    // PRIVATE: Only accessible inside this class
    int privateData;

    void privateMethod() {
        std::println("   Private method called");
    }

protected:
    // PROTECTED: Accessible in this class and derived classes
    int protectedData;

    void protectedMethod() {
        std::println("   Protected method called");
    }

public:
    // PUBLIC: Accessible from anywhere
    int publicData;

    AccessSpecifierDemo() : privateData(10), protectedData(20), publicData(30) { }

    void publicMethod() {
        std::println("   Public method called");

        // Can access all members inside the class
        std::println("   Accessing private: {}", privateData);
        std::println("   Accessing protected: {}", protectedData);
        std::println("   Accessing public: {}", publicData);

        privateMethod();
        protectedMethod();
    }
};

void demonstrateAccessSpecifiers() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: ACCESS SPECIFIERS      ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    AccessSpecifierDemo obj;

    std::println("--- From outside the class ---");
    std::println("");

    std::println("✅ Can access public:");
    obj.publicData = 100;
    std::println("   Public data: {}", obj.publicData);
    obj.publicMethod();

    std::println("");
    std::println("❌ Cannot access private:");
    std::println("   // obj.privateData = 100;  // Error!");
    std::println("   // obj.privateMethod();     // Error!");

    std::println("");
    std::println("❌ Cannot access protected (from outside):");
    std::println("   // obj.protectedData = 100; // Error!");
    std::println("   // obj.protectedMethod();    // Error!");
}

// ============================================
// EXAMPLE 4: DATA VALIDATION
// ============================================

class Student {
private:
    string name;
    int age;
    double gpa;

public:
    Student(string n, int a, double g) : name(n), age(0), gpa(0.0) {
        setName(n);
        setAge(a);
        setGpa(g);
    }

    // Getters - read-only access
    string getName() const { return name; }
    int getAge() const { return age; }
    double getGpa() const { return gpa; }

    // Setters - with validation
    void setName(string n) {
        if (n.empty()) {
            std::println("⚠️  Name cannot be empty. Using 'Unknown'");
            name = "Unknown";
        } else if (n.length() > 50) {
            std::println("⚠️  Name too long. Truncating to 50 characters");
            name = n.substr(0, 50);
        } else {
            name = n;
        }
    }

    void setAge(int a) {
        if (a < 0) {
            std::println("⚠️  Age cannot be negative. Setting to 0");
            age = 0;
        } else if (a > 150) {
            std::println("⚠️  Age seems unrealistic. Setting to 150");
            age = 150;
        } else {
            age = a;
        }
    }

    void setGpa(double g) {
        if (g < 0.0) {
            std::println("⚠️  GPA cannot be negative. Setting to 0.0");
            gpa = 0.0;
        } else if (g > 4.0) {
            std::println("⚠️  GPA cannot exceed 4.0. Setting to 4.0");
            gpa = 4.0;
        } else {
            gpa = g;
        }
    }

    void display() const {
        std::println("👤 Student: {}, Age: {}, GPA: {}", name, age, gpa);
    }
};

void demonstrateDataValidation() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: DATA VALIDATION        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating student with valid data ---");
    Student s1("Alice Johnson", 20, 3.8);
    s1.display();

    std::println("");
    std::println("--- Attempting invalid modifications ---");
    s1.setAge(-5);      // Invalid - will be rejected
    s1.setAge(200);     // Invalid - will be capped
    s1.setGpa(5.0);     // Invalid - will be capped
    s1.setName("");     // Invalid - will use default

    std::println("");
    std::println("--- After validation ---");
    s1.display();

    std::println("");
    std::println("✅ Encapsulation ensures data integrity!");
}

// ============================================
// EXAMPLE 5: INFORMATION HIDING
// ============================================

class Rectangle {
private:
    // Internal representation - hidden!
    double width;
    double height;

    // Private helper method
    bool isValidDimension(double value) const {
        return value > 0 && value < 10000;
    }

public:
    Rectangle(double w, double h) : width(0), height(0) {
        setWidth(w);
        setHeight(h);
    }

    // Public interface - what users see
    void setWidth(double w) {
        if (isValidDimension(w)) {
            width = w;
        } else {
            std::println("⚠️  Invalid width. Using 1.0");
            width = 1.0;
        }
    }

    void setHeight(double h) {
        if (isValidDimension(h)) {
            height = h;
        } else {
            std::println("⚠️  Invalid height. Using 1.0");
            height = 1.0;
        }
    }

    double getWidth() const { return width; }
    double getHeight() const { return height; }

    // Calculated properties
    double area() const {
        return width * height;
    }

    double perimeter() const {
        return 2 * (width + height);
    }

    void display() const {
        std::println("📐 Rectangle: {}×{}", width, height);
        std::println("   Area: {}", area());
        std::println("   Perimeter: {}", perimeter());
    }
};

void demonstrateInformationHiding() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 5: INFORMATION HIDING     ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Rectangle rect(5.0, 3.0);

    std::println("--- Using public interface ---");
    rect.display();

    std::println("");
    std::println("--- Modifying through setters ---");
    rect.setWidth(10.0);
    rect.setHeight(4.0);
    rect.display();

    std::println("");
    std::println("--- Attempting invalid values ---");
    rect.setWidth(-5.0);   // Will be rejected
    rect.setWidth(99999);  // Will be rejected
}

// ============================================
// EXAMPLE 6: REAL-WORLD - EMAIL CLASS
// ============================================

class Email {
private:
    string address;

    // Private validation method
    bool isValidEmail(const string& email) const {
        // Simple validation
        size_t atPos = email.find('@');
        size_t dotPos = email.find('.', atPos);

        return atPos != string::npos &&
               dotPos != string::npos &&
               atPos > 0 &&
               dotPos > atPos + 1 &&
               dotPos < email.length() - 1;
    }

public:
    Email() : address("") { }

    Email(const string& email) {
        setAddress(email);
    }

    void setAddress(const string& email) {
        if (isValidEmail(email)) {
            address = email;
            std::println("✅ Email set: {}", address);
        } else {
            std::println("❌ Invalid email format: {}", email);
            address = "";
        }
    }

    string getAddress() const {
        return address;
    }

    bool isEmpty() const {
        return address.empty();
    }

    void display() const {
        if (isEmpty()) {
            std::println("📧 No email address set");
        } else {
            std::println("📧 Email: {}", address);
        }
    }
};

void demonstrateEmailValidation() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 6: EMAIL VALIDATION       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Email email;

    std::println("--- Testing various email addresses ---");
    std::println("");

    email.setAddress("user@example.com");     // ✅ Valid
    email.setAddress("invalid.email");        // ❌ Invalid
    email.setAddress("@example.com");         // ❌ Invalid
    email.setAddress("user@.com");            // ❌ Invalid
    email.setAddress("alice.bob@company.co.uk");  // ✅ Valid

    std::println("");
    std::println("--- Final state ---");
    email.display();

    std::println("");
    std::println("✅ Encapsulation ensures only valid emails are stored!");
}

// ============================================
// EXAMPLE 7: REAL-WORLD - PASSWORD CLASS
// ============================================

class Password {
private:
    string hashedPassword;  // Never store plaintext!
    bool isSet;

    // Private: Simple hash function (in real code, use proper hashing!)
    string hash(const string& password) const {
        // This is just for demonstration - use proper crypto in real code!
        unsigned long hashValue = 5381;
        for (char c : password) {
            hashValue = ((hashValue << 5) + hashValue) + c;
        }
        return to_string(hashValue);
    }

    // Private: Validate password strength
    bool isStrongPassword(const string& password) const {
        if (password.length() < 8) {
            std::println("⚠️  Password too short (min 8 characters)");
            return false;
        }

        bool hasUpper = false, hasLower = false, hasDigit = false;
        for (char c : password) {
            if (isupper(c)) hasUpper = true;
            if (islower(c)) hasLower = true;
            if (isdigit(c)) hasDigit = true;
        }

        if (!hasUpper) {
            std::println("⚠️  Password needs uppercase letter");
            return false;
        }
        if (!hasLower) {
            std::println("⚠️  Password needs lowercase letter");
            return false;
        }
        if (!hasDigit) {
            std::println("⚠️  Password needs digit");
            return false;
        }

        return true;
    }

public:
    Password() : hashedPassword(""), isSet(false) { }

    bool setPassword(const string& password) {
        if (!isStrongPassword(password)) {
            return false;
        }

        hashedPassword = hash(password);
        isSet = true;
        std::println("✅ Password set successfully");
        return true;
    }

    bool verify(const string& password) const {
        if (!isSet) {
            std::println("❌ No password set");
            return false;
        }

        return hash(password) == hashedPassword;
    }

    bool passwordIsSet() const {
        return isSet;
    }

    // Note: No getter for password - it's never exposed!
};

void demonstratePasswordEncapsulation() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 7: PASSWORD SECURITY      ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Password pwd;

    std::println("--- Attempting weak passwords ---");
    pwd.setPassword("weak");           // Too short
    pwd.setPassword("nouppercase1");   // No uppercase
    pwd.setPassword("NOLOWERCASE1");   // No lowercase
    pwd.setPassword("NoDigits");       // No digits

    std::println("");
    std::println("--- Setting strong password ---");
    pwd.setPassword("StrongPass123");  // ✅ Valid

    std::println("");
    std::println("--- Verifying passwords ---");
    std::println("Verify 'WrongPass': {}", (pwd.verify("WrongPass") ? "✅" : "❌"));
    std::println("Verify 'StrongPass123': {}", (pwd.verify("StrongPass123") ? "✅" : "❌"));
}

// ============================================
// EXAMPLE 8: ENCAPSULATION WITH CALCULATION
// ============================================

class Circle {
private:
    double radius;
    mutable double cachedArea;      // Mutable for caching
    mutable bool areaCacheValid;

    void invalidateCache() {
        areaCacheValid = false;
    }

public:
    Circle(double r) : radius(r), cachedArea(0), areaCacheValid(false) {
        setRadius(r);
    }

    void setRadius(double r) {
        if (r > 0) {
            radius = r;
            invalidateCache();
        } else {
            std::println("⚠️  Radius must be positive");
        }
    }

    double getRadius() const {
        return radius;
    }

    double getArea() const {
        if (!areaCacheValid) {
            cachedArea = 3.14159 * radius * radius;
            areaCacheValid = true;
            std::println("   [Calculating area...]");
        } else {
            std::println("   [Using cached area]");
        }
        return cachedArea;
    }

    double getCircumference() const {
        return 2 * 3.14159 * radius;
    }

    void display() const {
        std::println("⭕ Circle:");
        std::println("   Radius: {}", radius);
        std::println("   Area: {}", getArea());
        std::println("   Circumference: {}", getCircumference());
    }
};

void demonstrateCalculationEncapsulation() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 8: CALCULATED PROPERTIES  ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Circle circle(5.0);

    std::println("--- First access ---");
    circle.display();

    std::println("");
    std::println("--- Second access (cached) ---");
    std::println("Area: {}", circle.getArea());

    std::println("");
    std::println("--- After radius change ---");
    circle.setRadius(10.0);
    std::println("Area: {}", circle.getArea());
}

// ============================================
// EXAMPLE 9: IMMUTABLE CLASS
// ============================================

class Point {
private:
    const int x;
    const int y;

public:
    Point(int xVal, int yVal) : x(xVal), y(yVal) { }

    // Only getters, no setters - immutable!
    int getX() const { return x; }
    int getY() const { return y; }

    // To "modify", create new object
    Point movedBy(int dx, int dy) const {
        return Point(x + dx, y + dy);
    }

    void display() const {
        std::println("📍 Point({}, {})", x, y);
    }
};

void demonstrateImmutability() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 9: IMMUTABLE CLASS        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Point p1(10, 20);
    std::print("Original: ");
    p1.display();

    // Can't modify - create new object instead
    Point p2 = p1.movedBy(5, 5);
    std::print("After move: ");
    p2.display();

    std::print("Original unchanged: ");
    p1.display();
}

// ============================================
// EXAMPLE 10: REAL-WORLD - SHOPPING CART
// ============================================

class ShoppingCart {
private:
    vector<string> items;
    vector<double> prices;
    double discountPercent;

    // Private: Calculate total before discount
    double calculateSubtotal() const {
        double total = 0;
        for (double price : prices) {
            total += price;
        }
        return total;
    }

public:
    ShoppingCart() : discountPercent(0) { }

    void addItem(const string& item, double price) {
        if (price <= 0) {
            std::println("❌ Invalid price");
            return;
        }
        items.push_back(item);
        prices.push_back(price);
        std::println("✅ Added: {} (${})", item, price);
    }

    void removeItem(size_t index) {
        if (index >= items.size()) {
            std::println("❌ Invalid item index");
            return;
        }
        std::println("✅ Removed: {}", items[index]);
        items.erase(items.begin() + index);
        prices.erase(prices.begin() + index);
    }

    void setDiscount(double percent) {
        if (percent < 0 || percent > 100) {
            std::println("❌ Invalid discount (0-100)");
            return;
        }
        discountPercent = percent;
        std::println("✅ Discount set to {}%", percent);
    }

    double getTotal() const {
        double subtotal = calculateSubtotal();
        double discount = subtotal * (discountPercent / 100.0);
        return subtotal - discount;
    }

    int getItemCount() const {
        return items.size();
    }

    void display() const {
        std::println("🛒 Shopping Cart:");
        std::println("================");
        for (size_t i = 0; i < items.size(); i++) {
            std::println("{}. {} - ${}", i + 1, items[i], prices[i]);
        }
        std::println("----------------");
        std::println("Subtotal: ${}", calculateSubtotal());
        if (discountPercent > 0) {
            std::println("Discount: {}%", discountPercent);
        }
        std::println("TOTAL: ${}", getTotal());
    }
};

void demonstrateShoppingCart() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 10: SHOPPING CART         ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    ShoppingCart cart;

    std::println("--- Adding items ---");
    cart.addItem("Laptop", 999.99);
    cart.addItem("Mouse", 25.50);
    cart.addItem("Keyboard", 75.00);
    cart.addItem("Invalid Item", -10.00);  // Will be rejected

    std::println("");
    std::println("--- Current cart ---");
    cart.display();

    std::println("");
    std::println("--- Applying discount ---");
    cart.setDiscount(10);

    std::println("");
    std::println("--- Final cart ---");
    cart.display();

    std::println("");
    std::println("--- Removing item ---");
    cart.removeItem(1);  // Remove mouse
    cart.display();
}

// ============================================
// MAIN DEMONSTRATION
// ============================================

int main() {
    std::println("========================================");
    std::println("ENCAPSULATION DEMONSTRATION");
    std::println("========================================");

    demonstrateWithoutEncapsulation();
    demonstrateWithEncapsulation();
    demonstrateAccessSpecifiers();
    demonstrateDataValidation();
    demonstrateInformationHiding();
    demonstrateEmailValidation();
    demonstratePasswordEncapsulation();
    demonstrateCalculationEncapsulation();
    demonstrateImmutability();
    demonstrateShoppingCart();

    return 0;
}
