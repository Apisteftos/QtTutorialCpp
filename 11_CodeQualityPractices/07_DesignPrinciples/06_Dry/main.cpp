#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <cmath>
#include <map>
using namespace std;

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

    cout << "   Total: $" << total << endl;
}

void calculateTax(double amount) {
    // 🚨 DUPLICATION: Same 0.1 tax rate!
    double tax = amount * 0.1;
    cout << "   Tax: $" << tax << endl;
}

void validateEmail(const string& email) {
    // 🚨 Magic string repeated
    if (email.find("@") == string::npos) {
        cout << "   ❌ Invalid email: missing @" << endl;
    }
}

void sendEmail(const string& email) {
    // 🚨 DUPLICATION: Same validation logic!
    if (email.find("@") == string::npos) {
        cout << "   ❌ Invalid email: missing @" << endl;
        return;
    }
    cout << "   📧 Email sent to: " << email << endl;
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
bool isValidEmail(const string& email) {
    return email.find("@") != string::npos;
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

    cout << "   Total: $" << total << endl;
}

void validateEmail(const string& email) {
    if (!isValidEmail(email)) {  // ✅ Reusing validation
        cout << "   ❌ Invalid email" << endl;
    }
}

void sendEmail(const string& email) {
    if (!isValidEmail(email)) {  // ✅ Same validation logic
        cout << "   ❌ Invalid email" << endl;
        return;
    }
    cout << "   📧 Email sent to: " << email << endl;
}
}

void example1_magic_numbers() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 1: MAGIC NUMBERS AND STRINGS          ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (Magic Numbers & Duplication):\n";
    cout << "```cpp\n";
    cout << "double tax = amount * 0.1;  // What is 0.1?\n";
    cout << "// Later in another function:\n";
    cout << "double tax = amount * 0.1;  // 🚨 Duplicated!\n";
    cout << "```\n";

    cout << "\n🚨 PROBLEMS:\n";
    cout << "   • Tax rate (0.1) appears multiple times\n";
    cout << "   • If tax changes, must find/update all occurrences\n";
    cout << "   • Magic numbers unclear meaning\n";
    cout << "   • Email validation logic duplicated\n";

    cout << "\n✅ GOOD CODE (Named Constants & Functions):\n";
    cout << "```cpp\n";
    cout << "const double TAX_RATE = 0.10;  // Clear meaning\n";
    cout << "double calculateTax(double amount) {\n";
    cout << "    return amount * TAX_RATE;  // ✅ Single source\n";
    cout << "}\n";
    cout << "```\n";

    cout << "\n✨ DEMONSTRATION:\n";

    cout << "\n   Processing order with good design:\n";
    good_example1::processOrder(150.0);

    cout << "\n   Validating and sending email:\n";
    good_example1::sendEmail("user@example.com");

    cout << "\n✨ BENEFITS:\n";
    cout << "   • Change tax rate in ONE place\n";
    cout << "   • Clear, self-documenting code\n";
    cout << "   • Email validation reused everywhere\n";
    cout << "   • Consistent behavior guaranteed\n";
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
            cout << "   ⚠️  Width must be positive, using 1" << endl;
            width = 1;
        }
        if (height <= 0) {
            cout << "   ⚠️  Height must be positive, using 1" << endl;
            height = 1;
        }
    }

    void setWidth(double w) {
        // 🚨 DUPLICATION: Same validation as constructor!
        if (w <= 0) {
            cout << "   ⚠️  Width must be positive, using 1" << endl;
            width = 1;
        } else {
            width = w;
        }
    }

    void setHeight(double h) {
        // 🚨 DUPLICATION: Same validation as constructor!
        if (h <= 0) {
            cout << "   ⚠️  Height must be positive, using 1" << endl;
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
            cout << "   ⚠️  Radius must be positive, using 1" << endl;
            radius = 1;
        }
    }

    void setRadius(double r) {
        // 🚨 DUPLICATION: Same validation!
        if (r <= 0) {
            cout << "   ⚠️  Radius must be positive, using 1" << endl;
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
double validatePositive(double value, const string& name) {
    if (value <= 0) {
        cout << "   ⚠️  " << name << " must be positive, using 1" << endl;
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
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 2: DUPLICATE LOGIC                     ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE:\n";
    cout << "   Validation logic repeated in:\n";
    cout << "   • Rectangle constructor\n";
    cout << "   • Rectangle::setWidth()\n";
    cout << "   • Rectangle::setHeight()\n";
    cout << "   • Circle constructor\n";
    cout << "   • Circle::setRadius()\n";

    cout << "\n✅ GOOD CODE:\n";
    cout << "   Single validatePositive() function used everywhere\n";

    cout << "\n✨ DEMONSTRATION:\n";

    cout << "\n   Creating shapes with validation:\n";
    good_example2::Rectangle rect(-5, 10);
    good_example2::Circle circle(-3);

    cout << "\n   Setting invalid values:\n";
    rect.setWidth(-2);
    circle.setRadius(-1);

    cout << "\n✨ BENEFITS:\n";
    cout << "   • Change validation logic in ONE place\n";
    cout << "   • Consistent validation everywhere\n";
    cout << "   • Less code to maintain\n";
}

// ============================================
// EXAMPLE 3: COPY-PASTE PROGRAMMING
// ============================================

namespace bad_example3 {
// ❌ BAD: Copy-pasted report generation

void generateUserReport(const vector<string>& users) {
    cout << "\n   ╔═══════════════════╗" << endl;
    cout << "   ║   USER REPORT     ║" << endl;
    cout << "   ╚═══════════════════╝" << endl;

    for (const auto& user : users) {
        cout << "   • " << user << endl;
    }

    cout << "   Total: " << users.size() << " users" << endl;
    cout << "   ───────────────────" << endl;
}

void generateProductReport(const vector<string>& products) {
    // 🚨 DUPLICATION: Almost identical to generateUserReport!
    cout << "\n   ╔═══════════════════╗" << endl;
    cout << "   ║  PRODUCT REPORT   ║" << endl;
    cout << "   ╚═══════════════════╝" << endl;

    for (const auto& product : products) {
        cout << "   • " << product << endl;
    }

    cout << "   Total: " << products.size() << " products" << endl;
    cout << "   ───────────────────" << endl;
}

void generateOrderReport(const vector<string>& orders) {
    // 🚨 DUPLICATION: Same structure again!
    cout << "\n   ╔═══════════════════╗" << endl;
    cout << "   ║   ORDER REPORT    ║" << endl;
    cout << "   ╚═══════════════════╝" << endl;

    for (const auto& order : orders) {
        cout << "   • " << order << endl;
    }

    cout << "   Total: " << orders.size() << " orders" << endl;
    cout << "   ───────────────────" << endl;
}

// If we want to change report format, must update 3 functions!
}

namespace good_example3 {
// ✅ GOOD: Generic report generator (DRY)

void generateReport(const string& title,
                    const vector<string>& items,
                    const string& itemType) {
    cout << "\n   ╔═══════════════════╗" << endl;
    cout << "   ║   " << title << "   ║" << endl;
    cout << "   ╚═══════════════════╝" << endl;

    for (const auto& item : items) {
        cout << "   • " << item << endl;
    }

    cout << "   Total: " << items.size() << " " << itemType << endl;
    cout << "   ───────────────────" << endl;
}

void generateUserReport(const vector<string>& users) {
    generateReport("USER REPORT", users, "users");
}

void generateProductReport(const vector<string>& products) {
    generateReport("PRODUCT REPORT", products, "products");
}

void generateOrderReport(const vector<string>& orders) {
    generateReport("ORDER REPORT", orders, "orders");
}
}

void example3_copy_paste() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 3: COPY-PASTE PROGRAMMING              ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE:\n";
    cout << "   Three nearly identical report functions\n";
    cout << "   To change format: update 3 functions!\n";

    cout << "\n✅ GOOD CODE:\n";
    cout << "   One generic generateReport() function\n";
    cout << "   To change format: update 1 function!\n";

    cout << "\n✨ DEMONSTRATION:\n";

    vector<string> users = {"Alice", "Bob", "Charlie"};
    vector<string> products = {"Laptop", "Mouse", "Keyboard"};
    vector<string> orders = {"Order #1", "Order #2"};

    good_example3::generateUserReport(users);
    good_example3::generateProductReport(products);
    good_example3::generateOrderReport(orders);

    cout << "\n✨ BENEFITS:\n";
    cout << "   • Change report format in ONE place\n";
    cout << "   • 70% less code\n";
    cout << "   • Easier to maintain and extend\n";
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
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 4: STRUCTURAL DUPLICATION              ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE:\n";
    cout << "   PI value (3.14159) hardcoded in every function\n";

    cout << "\n✅ GOOD CODE:\n";
    cout << "   const double PI = 3.14159265359;\n";
    cout << "   Used everywhere, defined once\n";

    cout << "\n✨ DEMONSTRATION:\n";

    double radius = 5.0;
    cout << "\n   Circle (radius " << radius << "):\n";
    cout << "   Area: " << good_example4::calculateCircleArea(radius) << endl;
    cout << "   Perimeter: " << good_example4::calculateCirclePerimeter(radius) << endl;

    cout << "\n   Sphere (radius " << radius << "):\n";
    cout << "   Volume: " << good_example4::calculateSphereVolume(radius) << endl;
    cout << "   Surface: " << good_example4::calculateSphereSurface(radius) << endl;

    cout << "\n✨ BENEFITS:\n";
    cout << "   • Need more precision? Change ONE constant\n";
    cout << "   • Consistent value everywhere\n";
    cout << "   • Self-documenting code\n";
}

// ============================================
// EXAMPLE 5: ALGORITHM DUPLICATION
// ============================================

namespace bad_example5 {
// ❌ BAD: Same algorithm, different data types

int findMaxInt(const vector<int>& numbers) {
    int max = numbers[0];
    for (int num : numbers) {
        if (num > max) {
            max = num;
        }
    }
    return max;
}

double findMaxDouble(const vector<double>& numbers) {
    // 🚨 DUPLICATION: Same algorithm!
    double max = numbers[0];
    for (double num : numbers) {
        if (num > max) {
            max = num;
        }
    }
    return max;
}

string findMaxString(const vector<string>& strings) {
    // 🚨 DUPLICATION: Same algorithm!
    string max = strings[0];
    for (const string& str : strings) {
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
T findMax(const vector<T>& items) {
    if (items.empty()) {
        throw runtime_error("Empty vector");
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
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 5: ALGORITHM DUPLICATION               ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE:\n";
    cout << "   Three functions: findMaxInt, findMaxDouble, findMaxString\n";
    cout << "   Same algorithm, different types\n";

    cout << "\n✅ GOOD CODE:\n";
    cout << "   One template function: findMax<T>\n";
    cout << "   Works with any comparable type\n";

    cout << "\n✨ DEMONSTRATION:\n";

    vector<int> numbers = {5, 2, 9, 1, 7};
    vector<double> decimals = {3.14, 2.71, 9.81, 1.41};
    vector<string> words = {"apple", "zebra", "banana"};

    cout << "\n   Max int: " << good_example5::findMax(numbers) << endl;
    cout << "   Max double: " << good_example5::findMax(decimals) << endl;
    cout << "   Max string: " << good_example5::findMax(words) << endl;

    cout << "\n✨ BENEFITS:\n";
    cout << "   • One function for all types\n";
    cout << "   • Fix bug once, fixed for all types\n";
    cout << "   • Less code to maintain\n";
}

// ============================================
// EXAMPLE 6: WHEN NOT TO DRY (False Abstraction)
// ============================================

namespace bad_example6 {
// ❌ BAD: Over-abstraction (forced DRY)

// Two functions that LOOK similar but have different purposes
void processUserData(int data) {
    cout << "   👤 Processing user data: " << data << endl;
    // User-specific logic here
}

void processProductData(int data) {
    cout << "   📦 Processing product data: " << data << endl;
    // Product-specific logic here
}

// Someone sees "duplication" and creates this:
void processData(int data, const string& type) {
    // 🚨 PROBLEM: Forcing unrelated concepts together!
    if (type == "user") {
        cout << "   👤 Processing user data: " << data << endl;
        // User logic
    } else if (type == "product") {
        cout << "   📦 Processing product data: " << data << endl;
        // Product logic
    }
    // Now when user processing changes, this function changes too!
    // These were separate concepts that happened to look similar
}
}

namespace good_example6 {
// ✅ GOOD: Keep separate if they represent different concepts

void processUserData(int userId) {
    cout << "   👤 Processing user #" << userId << endl;
    // User-specific business logic
    // This will evolve independently from products
}

void processProductData(int productId) {
    cout << "   📦 Processing product #" << productId << endl;
    // Product-specific business logic
    // This will evolve independently from users
}

// Don't force them together just because they look similar NOW
}

void example6_false_abstraction() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 6: WHEN NOT TO DRY                     ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n⚠️  WARNING: Don't Force DRY!\n";

    cout << "\n❌ BAD (Over-abstraction):\n";
    cout << "   Combining user and product processing\n";
    cout << "   Just because they LOOK similar\n";
    cout << "   Different concepts forced together\n";

    cout << "\n✅ GOOD (Separate Concerns):\n";
    cout << "   Keep them separate\n";
    cout << "   They represent different business concepts\n";
    cout << "   They will evolve differently\n";

    cout << "\n💡 RULE OF THUMB:\n";
    cout << "   • Same KNOWLEDGE → DRY (extract)\n";
    cout << "   • Same CODE, different PURPOSE → Keep separate\n";
    cout << "   • Coincidental similarity → Don't abstract yet\n";

    cout << "\n📝 THREE STRIKES RULE:\n";
    cout << "   1st time: Write it\n";
    cout << "   2nd time: Copy it (wince)\n";
    cout << "   3rd time: Abstract it (DRY)\n";

    cout << "\n✨ DEMONSTRATION:\n";

    good_example6::processUserData(123);
    good_example6::processProductData(456);
}

// ============================================
// EXAMPLE 7: DRY WITH CONFIGURATION
// ============================================

namespace bad_example7 {
// ❌ BAD: Database connection strings everywhere

void connectToDatabase() {
    string connection = "localhost:5432/mydb";  // 🚨
    cout << "   🔌 Connecting to: " << connection << endl;
}

void backupDatabase() {
    string connection = "localhost:5432/mydb";  // 🚨 Duplicated!
    cout << "   💾 Backing up: " << connection << endl;
}

void restoreDatabase() {
    string connection = "localhost:5432/mydb";  // 🚨 Duplicated!
    cout << "   ♻️  Restoring: " << connection << endl;
}
}

namespace good_example7 {
// ✅ GOOD: Configuration in one place

class Config {
public:
    static const string DB_HOST;
    static const int DB_PORT;
    static const string DB_NAME;

    static string getConnectionString() {
        return DB_HOST + ":" + to_string(DB_PORT) + "/" + DB_NAME;
    }
};

const string Config::DB_HOST = "localhost";
const int Config::DB_PORT = 5432;
const string Config::DB_NAME = "mydb";

void connectToDatabase() {
    cout << "   🔌 Connecting to: " << Config::getConnectionString() << endl;
}

void backupDatabase() {
    cout << "   💾 Backing up: " << Config::getConnectionString() << endl;
}

void restoreDatabase() {
    cout << "   ♻️  Restoring: " << Config::getConnectionString() << endl;
}
}

void example7_configuration() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 7: DRY WITH CONFIGURATION              ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE:\n";
    cout << "   Connection string hardcoded in 3 functions\n";
    cout << "   Change database? Update 3 places!\n";

    cout << "\n✅ GOOD CODE:\n";
    cout << "   Configuration class with constants\n";
    cout << "   Change database? Update 1 place!\n";

    cout << "\n✨ DEMONSTRATION:\n";

    good_example7::connectToDatabase();
    good_example7::backupDatabase();
    good_example7::restoreDatabase();

    cout << "\n✨ BENEFITS:\n";
    cout << "   • Single source of truth for config\n";
    cout << "   • Easy to switch environments\n";
    cout << "   • Can load from config file\n";
}

// ============================================
// EXAMPLE 8: HOW TO APPLY DRY
// ============================================

void example8_how_to_apply() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 8: HOW TO APPLY DRY                    ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n✅ STRATEGIES TO APPLY DRY:\n";

    cout << "\n1. EXTRACT FUNCTIONS:\n";
    cout << "   ❌ Copy-pasted code blocks\n";
    cout << "   ✅ Extract to reusable function\n";

    cout << "\n2. USE CONSTANTS:\n";
    cout << "   ❌ Magic numbers: 3.14159, 0.1, 100\n";
    cout << "   ✅ Named constants: PI, TAX_RATE, MAX_SIZE\n";

    cout << "\n3. EXTRACT CONFIGURATION:\n";
    cout << "   ❌ Hardcoded URLs, connection strings\n";
    cout << "   ✅ Config class or config file\n";

    cout << "\n4. USE TEMPLATES:\n";
    cout << "   ❌ findMaxInt(), findMaxDouble(), findMaxString()\n";
    cout << "   ✅ template<T> findMax()\n";

    cout << "\n5. CREATE BASE CLASSES:\n";
    cout << "   ❌ Duplicate methods in multiple classes\n";
    cout << "   ✅ Common functionality in base class\n";

    cout << "\n6. EXTRACT VALIDATION:\n";
    cout << "   ❌ Same validation in multiple places\n";
    cout << "   ✅ Validator class or validation functions\n";

    cout << "\n🔍 DETECTION CHECKLIST:\n";
    cout << "   □ Same code in multiple places?\n";
    cout << "   □ Magic numbers/strings repeated?\n";
    cout << "   □ Similar functions with slight variations?\n";
    cout << "   □ Copy-paste history in version control?\n";
    cout << "   □ Multiple places to update for one change?\n";

    cout << "\n⚖️  BALANCE DRY WITH:\n";
    cout << "   • Readability - Don't over-abstract\n";
    cout << "   • Simplicity - Don't create complex abstractions\n";
    cout << "   • Performance - Sometimes duplication is faster\n";
    cout << "   • Different concepts - Don't force unrelated code together\n";

    cout << "\n📏 RULE: Apply DRY when you have:\n";
    cout << "   • Same KNOWLEDGE (business rule, algorithm)\n";
    cout << "   • Same LOGIC (validation, calculation)\n";
    cout << "   • Same CONFIGURATION (constants, settings)\n";

    cout << "\n📏 DON'T Apply DRY when you have:\n";
    cout << "   • Coincidental similarity (looks same, different purpose)\n";
    cout << "   • Different concepts (will evolve separately)\n";
    cout << "   • Only seen twice (wait for third occurrence)\n";
}

// ============================================
// EXAMPLE 9: BENEFITS & SUMMARY
// ============================================

void example9_benefits() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 9: BENEFITS & SUMMARY                  ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n✅ BENEFITS OF DRY:\n";

    cout << "\n1. EASIER MAINTENANCE:\n";
    cout << "   • Change in one place\n";
    cout << "   • No hunting for duplicates\n";
    cout << "   • Less chance of missing updates\n";

    cout << "\n2. FEWER BUGS:\n";
    cout << "   • Fix once, fixed everywhere\n";
    cout << "   • Consistent behavior\n";
    cout << "   • Single source of truth\n";

    cout << "\n3. BETTER READABILITY:\n";
    cout << "   • Named constants explain meaning\n";
    cout << "   • Functions document intent\n";
    cout << "   • Less code to read\n";

    cout << "\n4. REDUCED CODE SIZE:\n";
    cout << "   • 30-70% less code\n";
    cout << "   • Smaller codebase\n";
    cout << "   • Faster compilation\n";

    cout << "\n5. CONSISTENT BEHAVIOR:\n";
    cout << "   • Same logic everywhere\n";
    cout << "   • No subtle differences\n";
    cout << "   • Predictable system\n";

    cout << "\n⚠️  WITHOUT DRY (WET Code):\n";
    cout << "   • Changes require multiple updates\n";
    cout << "   • Easy to miss locations\n";
    cout << "   • Inconsistent behavior\n";
    cout << "   • More bugs\n";
    cout << "   • Harder to maintain\n";

    cout << "\n💡 KEY INSIGHT:\n";
    cout << "   \"Every piece of knowledge should have a single,\n";
    cout << "    unambiguous, authoritative representation.\"\n";

    cout << "\n🎯 REMEMBER:\n";
    cout << "   • Don't repeat yourself\n";
    cout << "   • Extract common patterns\n";
    cout << "   • Single source of truth\n";
    cout << "   • But don't over-abstract!\n";

    cout << "\n📊 DRY METRICS:\n";
    cout << "   • Code duplication < 5%\n";
    cout << "   • Magic numbers = 0\n";
    cout << "   • Copy-paste commits = 0\n";
    cout << "   • Configuration centralized\n";

    cout << "\n🔄 DRY vs WET:\n";
    cout << "   DRY: Don't Repeat Yourself\n";
    cout << "   WET: Write Everything Twice\n";
    cout << "   AHA: Avoid Hasty Abstractions\n";
    cout << "   \n";
    cout << "   Balance: DRY when useful, AHA when uncertain!\n";
}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║             DRY PRINCIPLE (Don't Repeat Yourself)     ║\n";
    cout << "║                                                       ║\n";
    cout << "║  \"Every piece of knowledge must have a single,        ║\n";
    cout << "║   unambiguous, authoritative representation           ║\n";
    cout << "║   within a system.\"                                  ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n";

    example1_magic_numbers();
    example2_duplicate_logic();
    example3_copy_paste();
    example4_structural();
    example5_algorithm();
    example6_false_abstraction();
    example7_configuration();
    example8_how_to_apply();
    example9_benefits();

    cout << "\n\n";
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║           🎉 DRY PRINCIPLE COMPLETE! 🎉               ║\n";
    cout << "║                                                       ║\n";
    cout << "║  Key Takeaway: Don't repeat yourself! Extract common  ║\n";
    cout << "║  patterns, use constants, and maintain a single       ║\n";
    cout << "║  source of truth. But don't over-abstract!           ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n\n";

    return 0;
}
