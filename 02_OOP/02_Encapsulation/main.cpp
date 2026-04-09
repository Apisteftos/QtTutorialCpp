#include <iostream>
#include <string>
#include <vector>
#include <ctime>
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
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: WITHOUT ENCAPSULATION  ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    BankAccountBad account("123456", 1000.00);

    cout << "--- The Problem ---\n";
    cout << "Initial balance: $" << account.balance << "\n\n";

    cout << "❌ Anyone can modify balance directly:\n";
    account.balance = -5000.00;  // ❌ Negative balance!
    cout << "   Set to: $" << account.balance << "\n\n";

    account.balance = 999999999.99;  // ❌ Unrealistic amount!
    cout << "   Set to: $" << account.balance << "\n\n";

    cout << "Problems:\n";
    cout << "  • No validation\n";
    cout << "  • No control over modifications\n";
    cout << "  • Data integrity compromised\n";
    cout << "  • Business rules violated\n";
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
            cout << "✅ Deposited $" << amount << "\n";
            return true;
        }
        cout << "❌ Invalid deposit amount\n";
        return false;
    }

    bool withdraw(double amount) {
        if (amount <= 0) {
            cout << "❌ Invalid withdrawal amount\n";
            return false;
        }
        if (amount > balance) {
            cout << "❌ Insufficient funds\n";
            return false;
        }
        balance -= amount;
        cout << "✅ Withdrew $" << amount << "\n";
        return true;
    }

    void displayInfo() const {
        cout << "Account: " << accountNumber << "\n";
        cout << "Balance: $" << balance << "\n";
    }
};

void demonstrateWithEncapsulation() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: WITH ENCAPSULATION     ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    BankAccountGood account("123456", 1000.00);

    cout << "--- The Solution ---\n";
    account.displayInfo();

    cout << "\n✅ Controlled modifications:\n";
    // account.balance = -5000;  // ❌ Error! balance is private

    account.deposit(500.00);
    account.withdraw(200.00);
    account.withdraw(2000.00);  // Will fail - insufficient funds

    cout << "\n";
    account.displayInfo();

    cout << "\nBenefits:\n";
    cout << "  ✅ Validation enforced\n";
    cout << "  ✅ Data integrity maintained\n";
    cout << "  ✅ Business rules respected\n";
    cout << "  ✅ Controlled access\n";
}

// ============================================
// EXAMPLE 3: ACCESS SPECIFIERS
// ============================================

class AccessSpecifierDemo {
private:
    // PRIVATE: Only accessible inside this class
    int privateData;

    void privateMethod() {
        cout << "   Private method called\n";
    }

protected:
    // PROTECTED: Accessible in this class and derived classes
    int protectedData;

    void protectedMethod() {
        cout << "   Protected method called\n";
    }

public:
    // PUBLIC: Accessible from anywhere
    int publicData;

    AccessSpecifierDemo() : privateData(10), protectedData(20), publicData(30) { }

    void publicMethod() {
        cout << "   Public method called\n";

        // Can access all members inside the class
        cout << "   Accessing private: " << privateData << "\n";
        cout << "   Accessing protected: " << protectedData << "\n";
        cout << "   Accessing public: " << publicData << "\n";

        privateMethod();
        protectedMethod();
    }
};

void demonstrateAccessSpecifiers() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: ACCESS SPECIFIERS      ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    AccessSpecifierDemo obj;

    cout << "--- From outside the class ---\n\n";

    cout << "✅ Can access public:\n";
    obj.publicData = 100;
    cout << "   Public data: " << obj.publicData << "\n";
    obj.publicMethod();

    cout << "\n❌ Cannot access private:\n";
    cout << "   // obj.privateData = 100;  // Error!\n";
    cout << "   // obj.privateMethod();     // Error!\n";

    cout << "\n❌ Cannot access protected (from outside):\n";
    cout << "   // obj.protectedData = 100; // Error!\n";
    cout << "   // obj.protectedMethod();    // Error!\n";

    cout << "\n💡 Access Levels:\n";
    cout << "   private:   Only this class\n";
    cout << "   protected: This class + derived classes\n";
    cout << "   public:    Anywhere\n";
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
            cout << "⚠️  Name cannot be empty. Using 'Unknown'\n";
            name = "Unknown";
        } else if (n.length() > 50) {
            cout << "⚠️  Name too long. Truncating to 50 characters\n";
            name = n.substr(0, 50);
        } else {
            name = n;
        }
    }

    void setAge(int a) {
        if (a < 0) {
            cout << "⚠️  Age cannot be negative. Setting to 0\n";
            age = 0;
        } else if (a > 150) {
            cout << "⚠️  Age seems unrealistic. Setting to 150\n";
            age = 150;
        } else {
            age = a;
        }
    }

    void setGpa(double g) {
        if (g < 0.0) {
            cout << "⚠️  GPA cannot be negative. Setting to 0.0\n";
            gpa = 0.0;
        } else if (g > 4.0) {
            cout << "⚠️  GPA cannot exceed 4.0. Setting to 4.0\n";
            gpa = 4.0;
        } else {
            gpa = g;
        }
    }

    void display() const {
        cout << "👤 Student: " << name << ", Age: " << age << ", GPA: " << gpa << "\n";
    }
};

void demonstrateDataValidation() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: DATA VALIDATION        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating student with valid data ---\n";
    Student s1("Alice Johnson", 20, 3.8);
    s1.display();

    cout << "\n--- Attempting invalid modifications ---\n";
    s1.setAge(-5);      // Invalid - will be rejected
    s1.setAge(200);     // Invalid - will be capped
    s1.setGpa(5.0);     // Invalid - will be capped
    s1.setName("");     // Invalid - will use default

    cout << "\n--- After validation ---\n";
    s1.display();

    cout << "\n✅ Encapsulation ensures data integrity!\n";
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
            cout << "⚠️  Invalid width. Using 1.0\n";
            width = 1.0;
        }
    }

    void setHeight(double h) {
        if (isValidDimension(h)) {
            height = h;
        } else {
            cout << "⚠️  Invalid height. Using 1.0\n";
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
        cout << "📐 Rectangle: " << width << "×" << height << "\n";
        cout << "   Area: " << area() << "\n";
        cout << "   Perimeter: " << perimeter() << "\n";
    }
};

void demonstrateInformationHiding() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: INFORMATION HIDING     ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Rectangle rect(5.0, 3.0);

    cout << "--- Using public interface ---\n";
    rect.display();

    cout << "\n--- Modifying through setters ---\n";
    rect.setWidth(10.0);
    rect.setHeight(4.0);
    rect.display();

    cout << "\n--- Attempting invalid values ---\n";
    rect.setWidth(-5.0);   // Will be rejected
    rect.setWidth(99999);  // Will be rejected

    cout << "\n💡 Benefits:\n";
    cout << "   • Internal representation is hidden\n";
    cout << "   • Can change implementation later\n";
    cout << "   • User doesn't need to know internals\n";
    cout << "   • Validation is centralized\n";
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
            cout << "✅ Email set: " << address << "\n";
        } else {
            cout << "❌ Invalid email format: " << email << "\n";
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
            cout << "📧 No email address set\n";
        } else {
            cout << "📧 Email: " << address << "\n";
        }
    }
};

void demonstrateEmailValidation() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: EMAIL VALIDATION       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Email email;

    cout << "--- Testing various email addresses ---\n\n";

    email.setAddress("user@example.com");     // ✅ Valid
    email.setAddress("invalid.email");        // ❌ Invalid
    email.setAddress("@example.com");         // ❌ Invalid
    email.setAddress("user@.com");            // ❌ Invalid
    email.setAddress("alice.bob@company.co.uk");  // ✅ Valid

    cout << "\n--- Final state ---\n";
    email.display();

    cout << "\n✅ Encapsulation ensures only valid emails are stored!\n";
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
            cout << "⚠️  Password too short (min 8 characters)\n";
            return false;
        }

        bool hasUpper = false, hasLower = false, hasDigit = false;
        for (char c : password) {
            if (isupper(c)) hasUpper = true;
            if (islower(c)) hasLower = true;
            if (isdigit(c)) hasDigit = true;
        }

        if (!hasUpper) {
            cout << "⚠️  Password needs uppercase letter\n";
            return false;
        }
        if (!hasLower) {
            cout << "⚠️  Password needs lowercase letter\n";
            return false;
        }
        if (!hasDigit) {
            cout << "⚠️  Password needs digit\n";
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
        cout << "✅ Password set successfully\n";
        return true;
    }

    bool verify(const string& password) const {
        if (!isSet) {
            cout << "❌ No password set\n";
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
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 7: PASSWORD SECURITY      ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Password pwd;

    cout << "--- Attempting weak passwords ---\n";
    pwd.setPassword("weak");           // Too short
    pwd.setPassword("nouppercase1");   // No uppercase
    pwd.setPassword("NOLOWERCASE1");   // No lowercase
    pwd.setPassword("NoDigits");       // No digits

    cout << "\n--- Setting strong password ---\n";
    pwd.setPassword("StrongPass123");  // ✅ Valid

    cout << "\n--- Verifying passwords ---\n";
    cout << "Verify 'WrongPass': " << (pwd.verify("WrongPass") ? "✅" : "❌") << "\n";
    cout << "Verify 'StrongPass123': " << (pwd.verify("StrongPass123") ? "✅" : "❌") << "\n";

    cout << "\n💡 Security through encapsulation:\n";
    cout << "   • Password never stored in plaintext\n";
    cout << "   • No getter to retrieve password\n";
    cout << "   • Validation enforced\n";
    cout << "   • Only verification is possible\n";
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
            cout << "⚠️  Radius must be positive\n";
        }
    }

    double getRadius() const {
        return radius;
    }

    double getArea() const {
        if (!areaCacheValid) {
            cachedArea = 3.14159 * radius * radius;
            areaCacheValid = true;
            cout << "   [Calculating area...]\n";
        } else {
            cout << "   [Using cached area]\n";
        }
        return cachedArea;
    }

    double getCircumference() const {
        return 2 * 3.14159 * radius;
    }

    void display() const {
        cout << "⭕ Circle:\n";
        cout << "   Radius: " << radius << "\n";
        cout << "   Area: " << getArea() << "\n";
        cout << "   Circumference: " << getCircumference() << "\n";
    }
};

void demonstrateCalculationEncapsulation() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 8: CALCULATED PROPERTIES  ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Circle circle(5.0);

    cout << "--- First access ---\n";
    circle.display();

    cout << "\n--- Second access (cached) ---\n";
    cout << "Area: " << circle.getArea() << "\n";

    cout << "\n--- After radius change ---\n";
    circle.setRadius(10.0);
    cout << "Area: " << circle.getArea() << "\n";

    cout << "\n💡 Benefits:\n";
    cout << "   • Users don't see caching logic\n";
    cout << "   • Implementation can be optimized\n";
    cout << "   • Clean interface\n";
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
        cout << "📍 Point(" << x << ", " << y << ")\n";
    }
};

void demonstrateImmutability() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 9: IMMUTABLE CLASS        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Point p1(10, 20);
    cout << "Original: ";
    p1.display();

    // Can't modify - create new object instead
    Point p2 = p1.movedBy(5, 5);
    cout << "After move: ";
    p2.display();

    cout << "Original unchanged: ";
    p1.display();

    cout << "\n💡 Immutable objects:\n";
    cout << "   • Cannot be modified after creation\n";
    cout << "   • Thread-safe by design\n";
    cout << "   • Easier to reason about\n";
    cout << "   • No setters, only getters\n";
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
            cout << "❌ Invalid price\n";
            return;
        }
        items.push_back(item);
        prices.push_back(price);
        cout << "✅ Added: " << item << " ($" << price << ")\n";
    }

    void removeItem(size_t index) {
        if (index >= items.size()) {
            cout << "❌ Invalid item index\n";
            return;
        }
        cout << "✅ Removed: " << items[index] << "\n";
        items.erase(items.begin() + index);
        prices.erase(prices.begin() + index);
    }

    void setDiscount(double percent) {
        if (percent < 0 || percent > 100) {
            cout << "❌ Invalid discount (0-100)\n";
            return;
        }
        discountPercent = percent;
        cout << "✅ Discount set to " << percent << "%\n";
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
        cout << "🛒 Shopping Cart:\n";
        cout << "================\n";
        for (size_t i = 0; i < items.size(); i++) {
            cout << i + 1 << ". " << items[i] << " - $" << prices[i] << "\n";
        }
        cout << "----------------\n";
        cout << "Subtotal: $" << calculateSubtotal() << "\n";
        if (discountPercent > 0) {
            cout << "Discount: " << discountPercent << "%\n";
        }
        cout << "TOTAL: $" << getTotal() << "\n";
    }
};

void demonstrateShoppingCart() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 10: SHOPPING CART         ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    ShoppingCart cart;

    cout << "--- Adding items ---\n";
    cart.addItem("Laptop", 999.99);
    cart.addItem("Mouse", 25.50);
    cart.addItem("Keyboard", 75.00);
    cart.addItem("Invalid Item", -10.00);  // Will be rejected

    cout << "\n--- Current cart ---\n";
    cart.display();

    cout << "\n--- Applying discount ---\n";
    cart.setDiscount(10);

    cout << "\n--- Final cart ---\n";
    cart.display();

    cout << "\n--- Removing item ---\n";
    cart.removeItem(1);  // Remove mouse
    cart.display();

    cout << "\n💡 Encapsulation benefits:\n";
    cout << "   • Items/prices kept in sync automatically\n";
    cout << "   • Validation prevents invalid data\n";
    cout << "   • Internal calculation hidden\n";
    cout << "   • Clean public interface\n";
}

// ============================================
// MAIN DEMONSTRATION
// ============================================

int main() {
    cout << "========================================\n";
    cout << "ENCAPSULATION DEMONSTRATION\n";
    cout << "========================================\n";

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

    // ============================================
    // Comprehensive Summary
    // ============================================
    cout << "\n========================================\n";
    cout << "WHAT IS ENCAPSULATION?\n";
    cout << "========================================\n";
    cout << "Encapsulation is:\n";
    cout << "  • Bundling data and methods together\n";
    cout << "  • Hiding internal details\n";
    cout << "  • Providing controlled access\n";
    cout << "  • One of the four pillars of OOP\n";
    cout << "\n";
    cout << "Two main aspects:\n";
    cout << "  1. Data Hiding: Make members private\n";
    cout << "  2. Abstraction: Provide public interface\n";

    cout << "\n========================================\n";
    cout << "ACCESS SPECIFIERS\n";
    cout << "========================================\n";
    cout << "private:\n";
    cout << "  • Accessible only within the class\n";
    cout << "  • Default for class members\n";
    cout << "  • Use for: Internal data, helper methods\n";
    cout << "\n";
    cout << "protected:\n";
    cout << "  • Accessible in class and derived classes\n";
    cout << "  • Use for: Inheritance scenarios\n";
    cout << "  • Covered more in inheritance topic\n";
    cout << "\n";
    cout << "public:\n";
    cout << "  • Accessible from anywhere\n";
    cout << "  • Use for: Interface to outside world\n";
    cout << "  • Constructors, getters, setters, methods\n";

    cout << "\n========================================\n";
    cout << "TYPICAL CLASS STRUCTURE\n";
    cout << "========================================\n";
    cout << "class MyClass {\n";
    cout << "private:\n";
    cout << "    // Hidden data\n";
    cout << "    int data;\n";
    cout << "    \n";
    cout << "    // Private helper methods\n";
    cout << "    bool validate(int value) {\n";
    cout << "        return value > 0;\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "public:\n";
    cout << "    // Public interface\n";
    cout << "    void setData(int value) {\n";
    cout << "        if (validate(value)) {\n";
    cout << "            data = value;\n";
    cout << "        }\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    int getData() const {\n";
    cout << "        return data;\n";
    cout << "    }\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "WHY ENCAPSULATION?\n";
    cout << "========================================\n";
    cout << "1. Data Integrity:\n";
    cout << "   • Validation before modification\n";
    cout << "   • Prevent invalid states\n";
    cout << "   • Maintain invariants\n";
    cout << "\n";
    cout << "2. Flexibility:\n";
    cout << "   • Change implementation without breaking code\n";
    cout << "   • Internal changes don't affect users\n";
    cout << "   • Easy to maintain and extend\n";
    cout << "\n";
    cout << "3. Security:\n";
    cout << "   • Hide sensitive data\n";
    cout << "   • Control access to critical operations\n";
    cout << "   • Prevent unauthorized modifications\n";
    cout << "\n";
    cout << "4. Modularity:\n";
    cout << "   • Clear separation of interface and implementation\n";
    cout << "   • Easier to understand and use\n";
    cout << "   • Better code organization\n";
    cout << "\n";
    cout << "5. Debugging:\n";
    cout << "   • Controlled access points\n";
    cout << "   • Add logging easily\n";
    cout << "   • Track modifications\n";

    cout << "\n========================================\n";
    cout << "BENEFITS OF ENCAPSULATION\n";
    cout << "========================================\n";
    cout << "✅ Data Protection:\n";
    cout << "   • Private members can't be accessed directly\n";
    cout << "   • Only through controlled interface\n";
    cout << "\n";
    cout << "✅ Validation:\n";
    cout << "   • Check values before setting\n";
    cout << "   • Enforce business rules\n";
    cout << "   • Maintain consistency\n";
    cout << "\n";
    cout << "✅ Flexibility to Change:\n";
    cout << "   • Modify internal representation\n";
    cout << "   • Users don't need to change their code\n";
    cout << "   • Implementation details hidden\n";
    cout << "\n";
    cout << "✅ Ease of Use:\n";
    cout << "   • Simple public interface\n";
    cout << "   • Hide complexity\n";
    cout << "   • Clear API\n";
    cout << "\n";
    cout << "✅ Maintainability:\n";
    cout << "   • Changes localized to class\n";
    cout << "   • Easier to debug\n";
    cout << "   • Better code organization\n";

    cout << "\n========================================\n";
    cout << "ENCAPSULATION BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "✅ DO:\n";
    cout << "  • Make data members private\n";
    cout << "  • Provide getters and setters\n";
    cout << "  • Validate in setters\n";
    cout << "  • Make getters const\n";
    cout << "  • Hide implementation details\n";
    cout << "  • Provide minimal public interface\n";
    cout << "  • Use private helper methods\n";
    cout << "\n";
    cout << "❌ DON'T:\n";
    cout << "  • Make data public (unless constant)\n";
    cout << "  • Return non-const references to private data\n";
    cout << "  • Expose internal representation\n";
    cout << "  • Forget validation in setters\n";
    cout << "  • Make unnecessary members public\n";

    cout << "\n========================================\n";
    cout << "GETTERS AND SETTERS PATTERN\n";
    cout << "========================================\n";
    cout << "class Person {\n";
    cout << "private:\n";
    cout << "    string name;\n";
    cout << "    int age;\n";
    cout << "    \n";
    cout << "public:\n";
    cout << "    // Getter - const, returns value\n";
    cout << "    string getName() const {\n";
    cout << "        return name;\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    // Setter - validates, then sets\n";
    cout << "    void setName(const string& n) {\n";
    cout << "        if (!n.empty()) {\n";
    cout << "            name = n;\n";
    cout << "        }\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    int getAge() const {\n";
    cout << "        return age;\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    void setAge(int a) {\n";
    cout << "        if (a >= 0 && a <= 150) {\n";
    cout << "            age = a;\n";
    cout << "        }\n";
    cout << "    }\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "INFORMATION HIDING\n";
    cout << "========================================\n";
    cout << "Hide implementation details from users:\n";
    cout << "\n";
    cout << "class Stack {\n";
    cout << "private:\n";
    cout << "    // Users don't need to know we use vector\n";
    cout << "    vector<int> data;\n";
    cout << "    \n";
    cout << "public:\n";
    cout << "    // Simple interface\n";
    cout << "    void push(int value) { data.push_back(value); }\n";
    cout << "    int pop() { /* ... */ }\n";
    cout << "    int top() const { /* ... */ }\n";
    cout << "};\n";
    cout << "\n";
    cout << "Benefits:\n";
    cout << "  • Can change from vector to array\n";
    cout << "  • Users' code doesn't break\n";
    cout << "  • Implementation is flexible\n";

    cout << "\n========================================\n";
    cout << "REAL-WORLD EXAMPLES\n";
    cout << "========================================\n";
    cout << "Bank Account:\n";
    cout << "  • Balance is private\n";
    cout << "  • Deposits/withdrawals validated\n";
    cout << "  • Can't set negative balance\n";
    cout << "\n";
    cout << "Email Address:\n";
    cout << "  • Validation on set\n";
    cout << "  • Can't store invalid email\n";
    cout << "  • Format checking enforced\n";
    cout << "\n";
    cout << "Password:\n";
    cout << "  • Never stored in plaintext\n";
    cout << "  • No getter for password\n";
    cout << "  • Only verification possible\n";
    cout << "  • Strength validation\n";
    cout << "\n";
    cout << "Shopping Cart:\n";
    cout << "  • Items and prices kept in sync\n";
    cout << "  • Total calculated automatically\n";
    cout << "  • Discount logic hidden\n";

    cout << "\n========================================\n";
    cout << "COMMON MISTAKES\n";
    cout << "========================================\n";
    cout << "❌ Mistake 1: Public data members\n";
    cout << "   class Bad {\n";
    cout << "   public:\n";
    cout << "       int age;  // Anyone can set to -1!\n";
    cout << "   };\n";
    cout << "\n";
    cout << "❌ Mistake 2: Returning non-const reference\n";
    cout << "   class Bad {\n";
    cout << "       vector<int> data;\n";
    cout << "   public:\n";
    cout << "       vector<int>& getData() {  // Exposes internals!\n";
    cout << "           return data;\n";
    cout << "       }\n";
    cout << "   };\n";
    cout << "\n";
    cout << "❌ Mistake 3: No validation in setters\n";
    cout << "   void setAge(int a) {\n";
    cout << "       age = a;  // No checking!\n";
    cout << "   }\n";
    cout << "\n";
    cout << "❌ Mistake 4: Non-const getters\n";
    cout << "   int getAge() {  // Should be const!\n";
    cout << "       return age;\n";
    cout << "   }\n";

    cout << "\n========================================\n";
    cout << "WHEN TO USE PUBLIC DATA\n";
    cout << "========================================\n";
    cout << "Rarely! But acceptable for:\n";
    cout << "\n";
    cout << "1. POD (Plain Old Data) structures:\n";
    cout << "   struct Point {\n";
    cout << "       int x, y;  // Simple data container\n";
    cout << "   };\n";
    cout << "\n";
    cout << "2. Constants:\n";
    cout << "   class MyClass {\n";
    cout << "   public:\n";
    cout << "       static const int MAX_SIZE = 100;  // OK\n";
    cout << "   };\n";
    cout << "\n";
    cout << "Otherwise: Always use private data with getters/setters!\n";

    cout << "\n========================================\n";
    cout << "QUICK REFERENCE\n";
    cout << "========================================\n";
    cout << "// Encapsulated class template\n";
    cout << "class MyClass {\n";
    cout << "private:\n";
    cout << "    // 1. Private data\n";
    cout << "    int value;\n";
    cout << "    \n";
    cout << "    // 2. Private helper methods\n";
    cout << "    bool isValid(int v) const {\n";
    cout << "        return v > 0;\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "public:\n";
    cout << "    // 3. Constructor\n";
    cout << "    MyClass(int v) {\n";
    cout << "        setValue(v);\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    // 4. Getter (const)\n";
    cout << "    int getValue() const {\n";
    cout << "        return value;\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    // 5. Setter (with validation)\n";
    cout << "    void setValue(int v) {\n";
    cout << "        if (isValid(v)) {\n";
    cout << "            value = v;\n";
    cout << "        }\n";
    cout << "    }\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "KEY TAKEAWAYS\n";
    cout << "========================================\n";
    cout << "1. Encapsulation = Data hiding + Controlled access\n";
    cout << "2. Make data members private\n";
    cout << "3. Provide public getters (const) and setters\n";
    cout << "4. Validate in setters\n";
    cout << "5. Hide implementation details\n";
    cout << "6. Use access specifiers: private, protected, public\n";
    cout << "7. Benefits: Security, flexibility, maintainability\n";
    cout << "8. One of the four pillars of OOP\n";

    cout << "\n========================================\n";
    cout << "THE FOUR PILLARS OF OOP\n";
    cout << "========================================\n";
    cout << "1. Encapsulation ✅ (This topic)\n";
    cout << "   • Data hiding and controlled access\n";
    cout << "\n";
    cout << "2. Abstraction\n";
    cout << "   • Hiding complexity\n";
    cout << "   • Showing only essential features\n";
    cout << "\n";
    cout << "3. Inheritance\n";
    cout << "   • Reusing code from base classes\n";
    cout << "   • \"Is-a\" relationship\n";
    cout << "\n";
    cout << "4. Polymorphism\n";
    cout << "   • Many forms of same interface\n";
    cout << "   • Virtual functions\n";

    return 0;
}
