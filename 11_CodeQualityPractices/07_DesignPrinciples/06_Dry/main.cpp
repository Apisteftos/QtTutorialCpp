#include <vector>
#include <string>
#include <memory>
#include <cmath>
#include <map>
#include <print>

// ============================================
// DRY PRINCIPLE (Don't Repeat Yourself)
// ============================================

/*
DRY PRINCIPLE:
    "Every piece of knowledge must have a single, unambiguous,
     authoritative representation within a system."
    — Andy Hunt & Dave Thomas (The Pragmatic Programmer)

WHAT IT MEANS:
    • Don't duplicate code, logic, or knowledge
    • Each piece of information should exist in one place
    • Avoid copy-paste programming
    • Extract common functionality
    • Single source of truth

WHY IT MATTERS:
    ✅ Easier maintenance (change in one place)
    ✅ Fewer bugs (fix once, fixed everywhere)
    ✅ Better readability (clearer intent)
    ✅ Reduced code size
    ✅ Consistent behavior

KEY CONCEPTS:
    • Knowledge = Data, logic, algorithms, business rules
    • Duplication = Repeating the same knowledge
    • Abstraction = Extracting common patterns
    • Single Source of Truth = One definitive place
    • WET = Write Everything Twice (opposite of DRY)

VIOLATION SIGNS:
    🚨 Copy-pasted code blocks
    🚨 Same logic in multiple places
    🚨 Magic numbers/strings repeated
    🚨 Similar functions with slight variations
    🚨 Multiple places to update for one change

TYPES OF DUPLICATION:
    1. Literal duplication (exact copy-paste)
    2. Structural duplication (same pattern, different data)
    3. Semantic duplication (same meaning, different code)
    4. Algorithmic duplication (same algorithm, different implementation)
*/

// ============================================
// EXAMPLE 1: MAGIC NUMBERS AND STRINGS
// ============================================

namespace bad_example1 {
// ❌ BAD: Magic numbers and strings everywhere

void processOrder(double amount) {
    // 🚨 Magic number: What is 0.1?
    double tax = amount * 0.1;

    // 🚨 Magic number: What is 5.99?
    double shipping = amount > 100 ? 0 : 5.99;

    // 🚨 Magic number: What is 0.05?
    double discount = amount * 0.05;

    double total = amount + tax + shipping - discount;

    std::println("   Total: ${}", total);
}

void calculateTax(double amount) {
    // 🚨 DUPLICATION: Same 0.1 tax rate!
    double tax = amount * 0.1;
    std::println("   Tax: ${}", tax);
}

void validateEmail(const std::string& email) {
    // 🚨 Magic string repeated
    if (email.find("@") == std::string::npos) {
        std::println("   ❌ Invalid email: missing @");
    }
}

void sendEmail(const std::string& email) {
    // 🚨 DUPLICATION: Same validation logic!
    if (email.find("@") == std::string::npos) {
        std::println("   ❌ Invalid email: missing @");
        return;
    }
    std::println("   📧 Email sent to: {}", email);
}

// PROBLEMS:
// • Tax rate appears in multiple places
// • If tax rate changes, must update everywhere
// • Email validation duplicated
// • Magic numbers unclear meaning
}

namespace good_example1 {
// ✅ GOOD: Named constants and extracted functions

// Constants for magic numbers
const double TAX_RATE = 0.10;           // 10% tax
const double SHIPPING_FEE = 5.99;
const double FREE_SHIPPING_THRESHOLD = 100.0;
const double DISCOUNT_RATE = 0.05;      // 5% discount

// Extracted validation function (single source of truth)
bool isValidEmail(const std::string& email) {
    return email.find("@") != std::string::npos;
}

double calculateTax(double amount) {
    return amount * TAX_RATE;  // ✅ Single source for tax rate
}

double calculateShipping(double amount) {
    return amount > FREE_SHIPPING_THRESHOLD ? 0 : SHIPPING_FEE;
}

double calculateDiscount(double amount) {
    return amount * DISCOUNT_RATE;
}

void processOrder(double amount) {
    double tax = calculateTax(amount);
    double shipping = calculateShipping(amount);
    double discount = calculateDiscount(amount);
    double total = amount + tax + shipping - discount;

    std::println("   Total: ${}", total);
}

void validateEmail(const std::string& email) {
    if (!isValidEmail(email)) {  // ✅ Reusing validation
        std::println("   ❌ Invalid email");
    }
}

void sendEmail(const std::string& email) {
    if (!isValidEmail(email)) {  // ✅ Same validation logic
        std::println("   ❌ Invalid email");
        return;
    }
    std::println("   📧 Email sent to: {}", email);
}
}

void example1_magic_numbers() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 1: MAGIC NUMBERS AND STRINGS          ║");
    std::println("╚══════════════════════════════════════════════════╝");

    good_example1::processOrder(150.0);

    std::println("");
    std::println("   Validating and sending email:");
    good_example1::sendEmail("user@example.com");

}

// ============================================
// EXAMPLE 2: DUPLICATE LOGIC
// ============================================

namespace bad_example2 {
// ❌ BAD: Duplicate validation and calculation logic

class Rectangle {
public:
    double width, height;

    Rectangle(double w, double h) : width(w), height(h) {
        // 🚨 Validation logic
        if (width <= 0) {
            std::println("   ⚠️  Width must be positive, using 1");
            width = 1;
        }
        if (height <= 0) {
            std::println("   ⚠️  Height must be positive, using 1");
            height = 1;
        }
    }

    void setWidth(double w) {
        // 🚨 DUPLICATION: Same validation as constructor!
        if (w <= 0) {
            std::println("   ⚠️  Width must be positive, using 1");
            width = 1;
        } else {
            width = w;
        }
    }

    void setHeight(double h) {
        // 🚨 DUPLICATION: Same validation as constructor!
        if (h <= 0) {
            std::println("   ⚠️  Height must be positive, using 1");
            height = 1;
        } else {
            height = h;
        }
    }
};

class Circle {
public:
    double radius;

    Circle(double r) : radius(r) {
        // 🚨 Similar validation logic (duplicated pattern)
        if (radius <= 0) {
            std::println("   ⚠️  Radius must be positive, using 1");
            radius = 1;
        }
    }

    void setRadius(double r) {
        // 🚨 DUPLICATION: Same validation!
        if (r <= 0) {
            std::println("   ⚠️  Radius must be positive, using 1");
            radius = 1;
        } else {
            radius = r;
        }
    }
};
}

namespace good_example2 {
// ✅ GOOD: Extracted validation function

// Single source of truth for validation
double validatePositive(double value, const std::string& name) {
    if (value <= 0) {
        std::println("   ⚠️  {} must be positive, using 1", name);
        return 1.0;
    }
    return value;
}

class Rectangle {
private:
    double width, height;

public:
    Rectangle(double w, double h)
        : width(validatePositive(w, "Width"))
        , height(validatePositive(h, "Height")) {}

    void setWidth(double w) {
        width = validatePositive(w, "Width");  // ✅ Reuse
    }

    void setHeight(double h) {
        height = validatePositive(h, "Height");  // ✅ Reuse
    }

    double getWidth() const { return width; }
    double getHeight() const { return height; }
};

class Circle {
private:
    double radius;

public:
    Circle(double r)
        : radius(validatePositive(r, "Radius")) {}

    void setRadius(double r) {
        radius = validatePositive(r, "Radius");  // ✅ Reuse
    }

    double getRadius() const { return radius; }
};
}

void example2_duplicate_logic() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 2: DUPLICATE LOGIC                     ║");
    std::println("╚══════════════════════════════════════════════════╝");

    good_example2::Rectangle rect(-5, 10);
    good_example2::Circle circle(-3);

    std::println("");
    std::println("   Setting invalid values:");
    rect.setWidth(-2);
    circle.setRadius(-1);

}

// ============================================
// EXAMPLE 3: COPY-PASTE PROGRAMMING
// ============================================

namespace bad_example3 {
// ❌ BAD: Copy-pasted report generation

void generateUserReport(const std::vector<std::string>& users) {
    for (const auto& user : users) {
        std::println("   • {}", user);
    }

    std::println("   Total: {} users", users.size());
    std::println("   ───────────────────");
}

void generateProductReport(const std::vector<std::string>& products) {
    // 🚨 DUPLICATION: Almost identical to generateUserReport!
    std::println("");
    for (const auto& product : products) {
        std::println("   • {}", product);
    }

    std::println("   Total: {} products", products.size());
    std::println("   ───────────────────");
}

void generateOrderReport(const std::vector<std::string>& orders) {
    // 🚨 DUPLICATION: Same structure again!
    std::println("");
    for (const auto& order : orders) {
        std::println("   • {}", order);
    }

    std::println("   Total: {} orders", orders.size());
    std::println("   ───────────────────");
}

// If we want to change report format, must update 3 functions!
}

namespace good_example3 {
// ✅ GOOD: Generic report generator (DRY)

void generateReport(const std::string& title,
                    const std::vector<std::string>& items,
                    const std::string& itemType) {
    std::println("");
    std::println("   ╔═══════════════════╗");
    std::println("   ║   {}   ║", title);
    std::println("   ╚═══════════════════╝");

    for (const auto& item : items) {
        std::println("   • {}", item);
    }

    std::println("   Total: {} {}", items.size(), itemType);
    std::println("   ───────────────────");
}

void generateUserReport(const std::vector<std::string>& users) {
    generateReport("USER REPORT", users, "users");
}

void generateProductReport(const std::vector<std::string>& products) {
    generateReport("PRODUCT REPORT", products, "products");
}

void generateOrderReport(const std::vector<std::string>& orders) {
    generateReport("ORDER REPORT", orders, "orders");
}
}

void example3_copy_paste() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 3: COPY-PASTE PROGRAMMING              ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::vector<std::string> users = {"Alice", "Bob", "Charlie"};
    std::vector<std::string> products = {"Laptop", "Mouse", "Keyboard"};
    std::vector<std::string> orders = {"Order #1", "Order #2"};

    good_example3::generateUserReport(users);
    good_example3::generateProductReport(products);
    good_example3::generateOrderReport(orders);

}

// ============================================
// EXAMPLE 4: STRUCTURAL DUPLICATION
// ============================================

namespace bad_example4 {
// ❌ BAD: Similar structure repeated

double calculateCircleArea(double radius) {
    return 3.14159 * radius * radius;
}

double calculateCirclePerimeter(double radius) {
    return 2 * 3.14159 * radius;
}

double calculateSphereVolume(double radius) {
    return (4.0/3.0) * 3.14159 * radius * radius * radius;
}

double calculateSphereSurface(double radius) {
    return 4 * 3.14159 * radius * radius;
}

// 🚨 PROBLEM: PI (3.14159) repeated everywhere!
}

namespace good_example4 {
// ✅ GOOD: Constant for PI

const double PI = 3.14159265359;

double calculateCircleArea(double radius) {
    return PI * radius * radius;
}

double calculateCirclePerimeter(double radius) {
    return 2 * PI * radius;
}

double calculateSphereVolume(double radius) {
    return (4.0/3.0) * PI * radius * radius * radius;
}

double calculateSphereSurface(double radius) {
    return 4 * PI * radius * radius;
}
}

void example4_structural() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 4: STRUCTURAL DUPLICATION              ║");
    std::println("╚══════════════════════════════════════════════════╝");

    double radius = 5.0;
    std::println("");
    std::println("   Circle (radius {}):", radius);
    std::println("   Area: {}", good_example4::calculateCircleArea(radius));
    std::println("   Perimeter: {}", good_example4::calculateCirclePerimeter(radius));

    std::println("");
    std::println("   Sphere (radius {}):", radius);
    std::println("   Volume: {}", good_example4::calculateSphereVolume(radius));
    std::println("   Surface: {}", good_example4::calculateSphereSurface(radius));

}

// ============================================
// EXAMPLE 5: ALGORITHM DUPLICATION
// ============================================

namespace bad_example5 {
// ❌ BAD: Same algorithm, different data types

int findMaxInt(const std::vector<int>& numbers) {
    int max = numbers[0];
    for (int num : numbers) {
        if (num > max) {
            max = num;
        }
    }
    return max;
}

double findMaxDouble(const std::vector<double>& numbers) {
    // 🚨 DUPLICATION: Same algorithm!
    double max = numbers[0];
    for (double num : numbers) {
        if (num > max) {
            max = num;
        }
    }
    return max;
}

std::string findMaxString(const std::vector<std::string>& strings) {
    // 🚨 DUPLICATION: Same algorithm!
    std::string max = strings[0];
    for (const std::string& str : strings) {
        if (str > max) {
            max = str;
        }
    }
    return max;
}
}

namespace good_example5 {
// ✅ GOOD: Generic template function

template<typename T>
T findMax(const std::vector<T>& items) {
    if (items.empty()) {
        throw std::runtime_error("Empty vector");
    }

    T max = items[0];
    for (const T& item : items) {
        if (item > max) {
            max = item;
        }
    }
    return max;
}
}

void example5_algorithm() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 5: ALGORITHM DUPLICATION               ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::vector<int> numbers = {5, 2, 9, 1, 7};
    std::vector<double> decimals = {3.14, 2.71, 9.81, 1.41};
    std::vector<std::string> words = {"apple", "zebra", "banana"};

    std::println("");
    std::println("   Max int: {}", good_example5::findMax(numbers));
    std::println("   Max double: {}", good_example5::findMax(decimals));
    std::println("   Max string: {}", good_example5::findMax(words));

}

// ============================================
// EXAMPLE 6: WHEN NOT TO DRY (False Abstraction)
// ============================================

namespace bad_example6 {
// ❌ BAD: Over-abstraction (forced DRY)

// Two functions that LOOK similar but have different purposes
void processUserData(int data) {
    std::println("   👤 Processing user data: {}", data);
    // User-specific logic here
}

void processProductData(int data) {
    std::println("   📦 Processing product data: {}", data);
    // Product-specific logic here
}

// Someone sees "duplication" and creates this:
void processData(int data, const std::string& type) {
    // 🚨 PROBLEM: Forcing unrelated concepts together!
    if (type == "user") {
        std::println("   👤 Processing user data: {}", data);
        // User logic
    } else if (type == "product") {
        std::println("   📦 Processing product data: {}", data);
        // Product logic
    }
    // Now when user processing changes, this function changes too!
    // These were separate concepts that happened to look similar
}
}

namespace good_example6 {
// ✅ GOOD: Keep separate if they represent different concepts

void processUserData(int userId) {
    std::println("   👤 Processing user #{}", userId);
    // User-specific business logic
    // This will evolve independently from products
}

void processProductData(int productId) {
    std::println("   📦 Processing product #{}", productId);
    // Product-specific business logic
    // This will evolve independently from users
}

// Don't force them together just because they look similar NOW
}

void example6_false_abstraction() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 6: WHEN NOT TO DRY                     ║");
    std::println("╚══════════════════════════════════════════════════╝");

    good_example6::processUserData(123);
    good_example6::processProductData(456);
}

// ============================================
// EXAMPLE 7: DRY WITH CONFIGURATION
// ============================================

namespace bad_example7 {
// ❌ BAD: Database connection strings everywhere

void connectToDatabase() {
    std::string connection = "localhost:5432/mydb";  // 🚨
    std::println("   🔌 Connecting to: {}", connection);
}

void backupDatabase() {
    std::string connection = "localhost:5432/mydb";  // 🚨 Duplicated!
    std::println("   💾 Backing up: {}", connection);
}

void restoreDatabase() {
    std::string connection = "localhost:5432/mydb";  // 🚨 Duplicated!
    std::println("   ♻️  Restoring: {}", connection);
}
}

namespace good_example7 {
// ✅ GOOD: Configuration in one place

class Config {
public:
    static const std::string DB_HOST;
    static const int DB_PORT;
    static const std::string DB_NAME;

    static std::string getConnectionString() {
        return DB_HOST + ":" + std::to_string(DB_PORT) + "/" + DB_NAME;
    }
};

const std::string Config::DB_HOST = "localhost";
const int Config::DB_PORT = 5432;
const std::string Config::DB_NAME = "mydb";

void connectToDatabase() {
    std::println("   🔌 Connecting to: {}", Config::getConnectionString());
}

void backupDatabase() {
    std::println("   💾 Backing up: {}", Config::getConnectionString());
}

void restoreDatabase() {
    std::println("   ♻️  Restoring: {}", Config::getConnectionString());
}
}

void example7_configuration() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 7: DRY WITH CONFIGURATION              ║");
    std::println("╚══════════════════════════════════════════════════╝");

    good_example7::connectToDatabase();
    good_example7::backupDatabase();
    good_example7::restoreDatabase();

}

// ============================================
// EXAMPLE 8: HOW TO APPLY DRY
// ============================================

void example8_how_to_apply() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 8: HOW TO APPLY DRY                    ║");
    std::println("╚══════════════════════════════════════════════════╝");

}

// ============================================
// EXAMPLE 9: BENEFITS & SUMMARY
// ============================================

void example9_benefits() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 9: BENEFITS & SUMMARY                  ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   ");
}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    std::println("╔═══════════════════════════════════════════════════════╗");
    std::println("║                                                       ║");
    std::println("║             DRY PRINCIPLE (Don't Repeat Yourself)     ║");
    std::println("║                                                       ║");
    std::println("║  \"Every piece of knowledge must have a single,       ║");
    std::println("║   unambiguous, authoritative representation           ║");
    std::println("║   within a system.\"                                  ║");
    std::println("║                                                       ║");
    std::println("╚═══════════════════════════════════════════════════════╝");

    example1_magic_numbers();
    example2_duplicate_logic();
    example3_copy_paste();
    example4_structural();
    example5_algorithm();
    example6_false_abstraction();
    example7_configuration();
    example8_how_to_apply();
    example9_benefits();

    return 0;
}
