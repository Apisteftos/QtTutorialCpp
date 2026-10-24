#include <print>
#include <stdexcept>
#include <cctype>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

// ============================================
// WHAT ARE GETTERS AND SETTERS?
// ============================================

/*
 * GETTERS (Accessors):
 * - Functions that return the value of private members
 * - Should always be const (don't modify object)
 * - Provide read access to private data
 *
 * SETTERS (Mutators):
 * - Functions that modify the value of private members
 * - Should validate input before setting
 * - Provide controlled write access
 *
 * Benefits:
 * - Encapsulation: Hide internal representation
 * - Validation: Control what values are allowed
 * - Flexibility: Can change implementation
 * - Debugging: Add logging, breakpoints
 */

// ============================================
// EXAMPLE 1: BASIC GETTERS AND SETTERS
// ============================================

class Person {
private:
    string name;
    int age;

public:
    Person(string n, int a) : name(n), age(a) { }

    // GETTER - Read access (always const!)
    string getName() const {
        return name;
    }

    // SETTER - Write access (with validation)
    void setName(const string& n) {
        if (!n.empty()) {
            name = n;
        } else {
            std::println("⚠️  Name cannot be empty");
        }
    }

    // GETTER for age
    int getAge() const {
        return age;
    }

    // SETTER for age with validation
    void setAge(int a) {
        if (a >= 0 && a <= 150) {
            age = a;
        } else {
            std::println("⚠️  Invalid age: {}", a);
        }
    }

    void display() const {
        std::println("👤 Person: {}, Age: {}", name, age);
    }
};

void demonstrateBasicGettersSetters() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: BASIC GETTERS/SETTERS ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Person person("Alice", 25);

    std::println("--- Using getters ---");
    std::println("Name: {}", person.getName());
    std::println("Age: {}", person.getAge());

    std::println("");
    std::println("--- Using setters ---");
    person.setName("Alice Johnson");
    person.setAge(26);
    person.display();

    std::println("");
    std::println("--- Invalid attempts ---");
    person.setName("");      // Empty name - rejected
    person.setAge(-5);       // Negative age - rejected
    person.setAge(200);      // Too old - rejected

    std::println("");
    std::println("--- After invalid attempts ---");
    person.display();
    std::println("✅ Data integrity maintained!");
}

// ============================================
// EXAMPLE 2: DIFFERENT GETTER PATTERNS
// ============================================

class GetterPatterns {
private:
    string name;
    vector<int> data;
    int* dynamicData;
    mutable int cachedValue;
    mutable bool cacheValid;

public:
    GetterPatterns() : dynamicData(new int(100)), cachedValue(0), cacheValid(false) {
        name = "Example";
        data = {1, 2, 3, 4, 5};
    }

    ~GetterPatterns() {
        delete dynamicData;
    }

    // Pattern 1: Return by value (for simple types)
    string getName() const {
        return name;  // Returns copy
    }

    // Pattern 2: Return by const reference (for large objects)
    const vector<int>& getData() const {
        return data;  // Returns const reference (can't modify)
    }

    // Pattern 3: Return pointer (when nullptr is possible)
    const int* getDynamicData() const {
        return dynamicData;  // Can check for nullptr
    }

    // Pattern 4: Computed/calculated value
    int getSum() const {
        int sum = 0;
        for (int num : data) {
            sum += num;
        }
        return sum;
    }

    // Pattern 5: Lazy evaluation with caching
    int getExpensiveValue() const {
        if (!cacheValid) {
            std::println("   [Computing expensive value...]");
            // Simulate expensive calculation
            cachedValue = 0;
            for (int i = 0; i < 1000000; i++) {
                cachedValue += i % 100;
            }
            cacheValid = true;
        } else {
            std::println("   [Using cached value]");
        }
        return cachedValue;
    }

    // Setter that invalidates cache
    void addData(int value) {
        data.push_back(value);
        cacheValid = false;  // Invalidate cache when data changes
    }
};

void demonstrateGetterPatterns() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: GETTER PATTERNS        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    GetterPatterns obj;

    std::println("--- Pattern 1: Return by value ---");
    string name = obj.getName();
    std::println("Name: {}", name);

    std::println("");
    std::println("--- Pattern 2: Return by const reference ---");
    const vector<int>& data = obj.getData();
    std::println("Data size: {}", data.size());
    // data.push_back(10);  // ❌ Error! Can't modify const reference

    std::println("");
    std::println("--- Pattern 3: Return pointer ---");
    const int* ptr = obj.getDynamicData();
    if (ptr) {
        std::println("Dynamic data: {}", *ptr);
    }

    std::println("");
    std::println("--- Pattern 4: Computed value ---");
    std::println("Sum: {}", obj.getSum());

    std::println("");
    std::println("--- Pattern 5: Lazy evaluation ---");
    std::println("First call:");
    obj.getExpensiveValue();
    std::println("Second call:");
    obj.getExpensiveValue();

    std::println("");
    std::println("After modifying data:");
    obj.addData(10);
    obj.getExpensiveValue();  // Cache invalidated
}

// ============================================
// EXAMPLE 3: DIFFERENT SETTER PATTERNS
// ============================================

class SetterPatterns {
private:
    string name;
    int age;
    double salary;
    vector<string> tags;

public:
    SetterPatterns() : age(0), salary(0.0) { }

    // Pattern 1: Basic validation
    void setAge(int a) {
        if (a >= 0 && a <= 150) {
            age = a;
        } else {
            std::println("⚠️  Invalid age");
        }
    }

    // Pattern 2: Throw exception on invalid
    void setAgeStrict(int a) {
        if (a < 0 || a > 150) {
            throw invalid_argument("Age must be between 0 and 150");
        }
        age = a;
    }

    // Pattern 3: Return bool indicating success
    bool setSalary(double s) {
        if (s >= 0) {
            salary = s;
            return true;
        }
        std::println("⚠️  Salary cannot be negative");
        return false;
    }

    // Pattern 4: Clamp to valid range
    void setAgeClamp(int a) {
        if (a < 0) {
            age = 0;
            std::println("⚠️  Age clamped to 0");
        } else if (a > 150) {
            age = 150;
            std::println("⚠️  Age clamped to 150");
        } else {
            age = a;
        }
    }

    // Pattern 5: Normalize/sanitize input
    void setName(string n) {
        // Remove leading/trailing whitespace
        size_t start = n.find_first_not_of(" \t\n\r");
        size_t end = n.find_last_not_of(" \t\n\r");

        if (start == string::npos) {
            name = "";
            std::println("⚠️  Empty name after trimming");
        } else {
            name = n.substr(start, end - start + 1);
            // Capitalize first letter
            if (!name.empty()) {
                name[0] = toupper(name[0]);
            }
        }
    }

    // Pattern 6: Method chaining (fluent interface)
    SetterPatterns& setNameChain(const string& n) {
        setName(n);
        return *this;
    }

    SetterPatterns& setAgeChain(int a) {
        setAge(a);
        return *this;
    }

    SetterPatterns& setSalaryChain(double s) {
        setSalary(s);
        return *this;
    }

    // Pattern 7: Batch setter
    void setPersonInfo(const string& n, int a, double s) {
        setName(n);
        setAge(a);
        setSalary(s);
    }

    void display() const {
        std::println("Name: {}, Age: {}, Salary: ${:.2f}", name, age, salary);
    }
};

void demonstrateSetterPatterns() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: SETTER PATTERNS        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    SetterPatterns obj;

    std::println("--- Pattern 1: Basic validation ---");
    obj.setAge(25);
    obj.setAge(-5);  // Invalid

    std::println("");
    std::println("--- Pattern 2: Exception on invalid ---");
    try {
        obj.setAgeStrict(200);  // Throws exception
    } catch (const exception& e) {
        std::println("❌ Exception: {}", e.what());
    }

    std::println("");
    std::println("--- Pattern 3: Return bool ---");
    bool success = obj.setSalary(50000);
    std::println("Set salary: {}", (success ? "✅ Success" : "❌ Failed"));

    success = obj.setSalary(-1000);
    std::println("Set negative: {}", (success ? "✅ Success" : "❌ Failed"));

    std::println("");
    std::println("--- Pattern 4: Clamp to range ---");
    obj.setAgeClamp(200);  // Clamped to 150

    std::println("");
    std::println("--- Pattern 5: Normalize input ---");
    obj.setName("  alice johnson  ");  // Trimmed and capitalized
    obj.display();

    std::println("");
    std::println("--- Pattern 6: Method chaining ---");
    obj.setNameChain("Bob Smith")
        .setAgeChain(30)
        .setSalaryChain(60000);
    obj.display();

    std::println("");
    std::println("--- Pattern 7: Batch setter ---");
    obj.setPersonInfo("Charlie Brown", 35, 70000);
    obj.display();
}

// ============================================
// EXAMPLE 4: READ-ONLY AND WRITE-ONLY PROPERTIES
// ============================================

class AccessControl {
private:
    string readOnlyValue;
    string writeOnlyValue;
    string fullAccessValue;

public:
    AccessControl() : readOnlyValue("READ-ONLY"), writeOnlyValue(""), fullAccessValue("FULL-ACCESS") { }

    // READ-ONLY: Only getter, no setter
    string getReadOnly() const {
        return readOnlyValue;
    }

    // WRITE-ONLY: Only setter, no getter
    void setWriteOnly(const string& value) {
        writeOnlyValue = value;
        std::println("✅ Write-only value set (but can't read it back!)");
    }

    // FULL ACCESS: Both getter and setter
    string getFullAccess() const {
        return fullAccessValue;
    }

    void setFullAccess(const string& value) {
        fullAccessValue = value;
    }
};

void demonstrateAccessControl() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: ACCESS CONTROL         ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    AccessControl obj;

    std::println("--- Read-only property ---");
    std::println("Value: {}", obj.getReadOnly());
    // obj.setReadOnly("new");  // ❌ No setter available
    std::println("✓ Can read, but cannot modify");

    std::println("");
    std::println("--- Write-only property ---");
    obj.setWriteOnly("secret_password");
    // string val = obj.getWriteOnly();  // ❌ No getter available
    std::println("✓ Can write, but cannot read back");

    std::println("");
    std::println("--- Full access property ---");
    std::println("Value: {}", obj.getFullAccess());
    obj.setFullAccess("modified");
    std::println("New value: {}", obj.getFullAccess());
    std::println("✓ Can both read and write");
}

// ============================================
// EXAMPLE 5: VALIDATION STRATEGIES
// ============================================

class ValidationExample {
private:
    string email;
    string phoneNumber;
    int zipCode;

    // Private validation methods
    bool isValidEmail(const string& email) const {
        size_t atPos = email.find('@');
        size_t dotPos = email.find('.', atPos);
        return atPos != string::npos &&
               dotPos != string::npos &&
               atPos > 0 &&
               dotPos > atPos + 1;
    }

    bool isValidPhoneNumber(const string& phone) const {
        // Simple validation: 10 digits
        int digitCount = 0;
        for (char c : phone) {
            if (isdigit(c)) digitCount++;
        }
        return digitCount == 10;
    }

    bool isValidZipCode(int zip) const {
        return zip >= 10000 && zip <= 99999;
    }

public:
    ValidationExample() : zipCode(0) { }

    // Email with validation
    void setEmail(const string& e) {
        if (isValidEmail(e)) {
            email = e;
            std::println("✅ Email set: {}", email);
        } else {
            std::println("❌ Invalid email format: {}", e);
        }
    }

    string getEmail() const {
        return email;
    }

    // Phone with validation and formatting
    void setPhoneNumber(const string& phone) {
        if (isValidPhoneNumber(phone)) {
            // Extract digits
            string digits;
            for (char c : phone) {
                if (isdigit(c)) digits += c;
            }
            // Format as (XXX) XXX-XXXX
            phoneNumber = "(" + digits.substr(0, 3) + ") " +
                          digits.substr(3, 3) + "-" + digits.substr(6, 4);
            std::println("✅ Phone set: {}", phoneNumber);
        } else {
            std::println("❌ Invalid phone number: {}", phone);
        }
    }

    string getPhoneNumber() const {
        return phoneNumber;
    }

    // Zip code with validation
    void setZipCode(int zip) {
        if (isValidZipCode(zip)) {
            zipCode = zip;
            std::println("✅ Zip code set: {}", zipCode);
        } else {
            std::println("❌ Invalid zip code: {} (must be 5 digits)", zip);
        }
    }

    int getZipCode() const {
        return zipCode;
    }

    void display() const {
        std::println("📧 Email: {}", (email.empty() ? "[not set]" : email));
        std::println("📞 Phone: {}", (phoneNumber.empty() ? "[not set]" : phoneNumber));
        std::println("📮 Zip: {}", (zipCode == 0 ? "[not set]" : to_string(zipCode)));
    }
};

void demonstrateValidation() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 5: VALIDATION STRATEGIES  ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    ValidationExample obj;

    std::println("--- Email validation ---");
    obj.setEmail("user@example.com");    // ✅ Valid
    obj.setEmail("invalid.email");       // ❌ Invalid
    obj.setEmail("@example.com");        // ❌ Invalid

    std::println("");
    std::println("--- Phone validation and formatting ---");
    obj.setPhoneNumber("1234567890");    // ✅ Valid, formatted
    obj.setPhoneNumber("(123) 456-7890"); // ✅ Valid, reformatted
    obj.setPhoneNumber("123");           // ❌ Invalid

    std::println("");
    std::println("--- Zip code validation ---");
    obj.setZipCode(12345);               // ✅ Valid
    obj.setZipCode(123);                 // ❌ Invalid
    obj.setZipCode(123456);              // ❌ Invalid

    std::println("");
    std::println("--- Final state ---");
    obj.display();
}

// ============================================
// EXAMPLE 6: COMPUTED PROPERTIES
// ============================================

class Rectangle {
private:
    double width;
    double height;

public:
    Rectangle(double w, double h) : width(w), height(h) { }

    // Regular getters/setters
    double getWidth() const { return width; }
    void setWidth(double w) {
        if (w > 0) width = w;
    }

    double getHeight() const { return height; }
    void setHeight(double h) {
        if (h > 0) height = h;
    }

    // COMPUTED properties - calculated on demand
    double getArea() const {
        return width * height;
    }

    double getPerimeter() const {
        return 2 * (width + height);
    }

    double getDiagonal() const {
        return sqrt(width * width + height * height);
    }

    bool isSquare() const {
        return width == height;
    }

    // No setters for computed properties!
    // They're calculated from width and height

    void display() const {
        std::println("📐 Rectangle: {:g} × {:g}", width, height);
        std::println("   Area: {:g}", getArea());
        std::println("   Perimeter: {:g}", getPerimeter());
        std::println("   Diagonal: {:g}", getDiagonal());
        std::println("   Is square? {}", (isSquare() ? "Yes" : "No"));
    }
};

void demonstrateComputedProperties() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 6: COMPUTED PROPERTIES    ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Rectangle rect(5.0, 3.0);

    std::println("--- Initial rectangle ---");
    rect.display();

    std::println("");
    std::println("--- After changing dimensions ---");
    rect.setWidth(4.0);
    rect.setHeight(4.0);
    rect.display();
}

// ============================================
// EXAMPLE 7: DEPENDENT PROPERTIES
// ============================================

class Temperature {
private:
    double celsius;  // Internal representation

public:
    Temperature(double c = 0) : celsius(c) { }

    // Celsius - direct access
    double getCelsius() const {
        return celsius;
    }

    void setCelsius(double c) {
        if (c >= -273.15) {  // Absolute zero
            celsius = c;
        } else {
            std::println("⚠️  Temperature below absolute zero!");
        }
    }

    // Fahrenheit - computed from Celsius
    double getFahrenheit() const {
        return celsius * 9.0 / 5.0 + 32.0;
    }

    void setFahrenheit(double f) {
        setCelsius((f - 32.0) * 5.0 / 9.0);
    }

    // Kelvin - computed from Celsius
    double getKelvin() const {
        return celsius + 273.15;
    }

    void setKelvin(double k) {
        setCelsius(k - 273.15);
    }

    void display() const {
        std::println("🌡️  Temperature:");
        std::println("   Celsius:    {:g}°C", getCelsius());
        std::println("   Fahrenheit: {:g}°F", getFahrenheit());
        std::println("   Kelvin:     {:g}K", getKelvin());
    }
};

void demonstrateDependentProperties() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 7: DEPENDENT PROPERTIES   ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Temperature temp(25.0);  // 25°C

    std::println("--- Initial (25°C) ---");
    temp.display();

    std::println("");
    std::println("--- Set Fahrenheit to 100 ---");
    temp.setFahrenheit(100);
    temp.display();

    std::println("");
    std::println("--- Set Kelvin to 300 ---");
    temp.setKelvin(300);
    temp.display();
}

// ============================================
// EXAMPLE 8: RANGE-BASED PROPERTIES
// ============================================

class Percentage {
private:
    double value;  // 0.0 to 1.0 internally

public:
    Percentage(double v = 0) {
        setValue(v);
    }

    // Get as decimal (0.0 - 1.0)
    double getValue() const {
        return value;
    }

    // Set as decimal, clamp to range
    void setValue(double v) {
        if (v < 0) {
            value = 0;
            std::println("⚠️  Clamped to 0%");
        } else if (v > 1) {
            value = 1;
            std::println("⚠️  Clamped to 100%");
        } else {
            value = v;
        }
    }

    // Get as percentage (0 - 100)
    double getPercent() const {
        return value * 100.0;
    }

    // Set as percentage
    void setPercent(double p) {
        setValue(p / 100.0);
    }

    void display() const {
        std::println("📊 Percentage: {:g}% (decimal: {:g})", getPercent(), getValue());
    }
};

void demonstrateRangeProperties() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 8: RANGE PROPERTIES       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Percentage pct;

    std::println("--- Setting as decimal ---");
    pct.setValue(0.75);
    pct.display();

    std::println("");
    std::println("--- Setting as percentage ---");
    pct.setPercent(50);
    pct.display();

    std::println("");
    std::println("--- Out of range values ---");
    pct.setPercent(150);  // Clamped to 100%
    pct.display();

    pct.setPercent(-20);  // Clamped to 0%
    pct.display();
}

// ============================================
// EXAMPLE 9: REAL-WORLD - USER PROFILE
// ============================================

class UserProfile {
private:
    string username;
    string email;
    int age;
    string bio;
    bool isVerified;

    // Validation helpers
    bool isValidUsername(const string& user) const {
        if (user.length() < 3 || user.length() > 20) return false;
        for (char c : user) {
            if (!isalnum(c) && c != '_') return false;
        }
        return true;
    }

    bool isValidEmail(const string& email) const {
        size_t atPos = email.find('@');
        size_t dotPos = email.find('.', atPos);
        return atPos != string::npos && dotPos != string::npos;
    }

public:
    UserProfile() : age(0), isVerified(false) { }

    // Username - alphanumeric + underscore, 3-20 chars
    string getUsername() const { return username; }

    bool setUsername(const string& user) {
        if (isValidUsername(user)) {
            username = user;
            std::println("✅ Username set: {}", username);
            return true;
        }
        std::println("❌ Invalid username (3-20 chars, alphanumeric + underscore)");
        return false;
    }

    // Email - with validation
    string getEmail() const { return email; }

    bool setEmail(const string& e) {
        if (isValidEmail(e)) {
            email = e;
            std::println("✅ Email set: {}", email);
            return true;
        }
        std::println("❌ Invalid email format");
        return false;
    }

    // Age - 13-120 range
    int getAge() const { return age; }

    bool setAge(int a) {
        if (a >= 13 && a <= 120) {
            age = a;
            return true;
        }
        std::println("❌ Age must be between 13 and 120");
        return false;
    }

    // Bio - max 500 characters
    string getBio() const { return bio; }

    void setBio(const string& b) {
        if (b.length() <= 500) {
            bio = b;
        } else {
            bio = b.substr(0, 500);
            std::println("⚠️  Bio truncated to 500 characters");
        }
    }

    // Verified - read-only for users
    bool getIsVerified() const { return isVerified; }

    // Only the system should verify a profile. In real code this would be private
    // (or reachable only through a friend); it is public here to keep the demo short.
    void verify() {
        isVerified = true;
        std::println("✅ Profile verified!");
    }

    void display() const {
        std::println("╔════════════════════════════════╗");
        std::println("║       USER PROFILE             ║");
        std::println("╚════════════════════════════════╝");
        std::println("Username: {}{}", username, (isVerified ? " ✓" : ""));
        std::println("Email:    {}", email);
        std::println("Age:      {}", age);
        std::println("Bio:      {}{}", (bio.empty() ? "[empty]" : bio.substr(0, 50)), (bio.length() > 50 ? "..." : ""));
    }

    // Demo helper that stands in for "the system" calling verify()
    void systemVerify() { verify(); }
};

void demonstrateUserProfile() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 9: USER PROFILE           ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    UserProfile profile;

    std::println("--- Setting up profile ---");
    profile.setUsername("alice_2024");
    profile.setEmail("alice@example.com");
    profile.setAge(25);
    profile.setBio("Software developer passionate about C++ and system programming");

    std::println("");
    std::println("--- Invalid attempts ---");
    profile.setUsername("ab");            // Too short
    profile.setUsername("user@invalid");  // Invalid character
    profile.setEmail("invalid");          // Invalid format
    profile.setAge(10);                   // Too young

    std::println("");
    std::println("--- Profile display ---");
    profile.display();

    std::println("");
    std::println("--- System verification ---");
    profile.systemVerify();
    profile.display();
}

// ============================================
// EXAMPLE 10: REAL-WORLD - PRODUCT INVENTORY
// ============================================

class Product {
private:
    string name;
    double price;
    int quantity;
    string sku;  // Stock Keeping Unit

public:
    Product(const string& n, double p, int q, const string& s)
        : name(n), price(0), quantity(0), sku(s) {
        setPrice(p);
        setQuantity(q);
    }

    // Name
    string getName() const { return name; }
    void setName(const string& n) {
        if (!n.empty()) {
            name = n;
        }
    }

    // Price with validation
    double getPrice() const { return price; }

    bool setPrice(double p) {
        if (p >= 0) {
            price = p;
            return true;
        }
        std::println("❌ Price cannot be negative");
        return false;
    }

    // Quantity with validation
    int getQuantity() const { return quantity; }

    bool setQuantity(int q) {
        if (q >= 0) {
            quantity = q;
            return true;
        }
        std::println("❌ Quantity cannot be negative");
        return false;
    }

    // SKU - read-only after creation
    string getSku() const { return sku; }

    // Stock operations
    bool addStock(int amount) {
        if (amount > 0) {
            quantity += amount;
            std::println("✅ Added {} units. New stock: {}", amount, quantity);
            return true;
        }
        return false;
    }

    bool removeStock(int amount) {
        if (amount > 0 && amount <= quantity) {
            quantity -= amount;
            std::println("✅ Removed {} units. Remaining: {}", amount, quantity);
            return true;
        }
        if (amount > quantity) {
            std::println("❌ Insufficient stock");
        }
        return false;
    }

    // Computed properties
    double getTotalValue() const {
        return price * quantity;
    }

    bool isInStock() const {
        return quantity > 0;
    }

    bool isLowStock() const {
        return quantity > 0 && quantity < 10;
    }

    void display() const {
        std::println("📦 Product: {} [{}]", name, sku);
        std::println("   Price: ${:.2f}", price);
        std::print("   Stock: {}", quantity);
        if (isLowStock()) std::print(" ⚠️  LOW STOCK");
        if (!isInStock()) std::print(" ❌ OUT OF STOCK");
        std::println("");
        std::println("   Total Value: ${:.2f}", getTotalValue());
    }
};

void demonstrateProductInventory() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 10: PRODUCT INVENTORY     ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Product product("Laptop", 999.99, 50, "LAP-001");

    std::println("--- Initial product ---");
    product.display();

    std::println("");
    std::println("--- Stock operations ---");
    product.removeStock(45);  // Sell 45 units
    product.display();

    std::println("");
    product.removeStock(3);   // Sell 3 more (low stock warning)
    product.display();

    std::println("");
    product.addStock(100);    // Restock
    product.display();

    std::println("");
    std::println("--- Invalid operations ---");
    product.setPrice(-100);   // Negative price - rejected
    product.removeStock(200); // Insufficient stock
}

// ============================================
// MAIN DEMONSTRATION
// ============================================

int main() {
    std::println("========================================");
    std::println("GETTERS AND SETTERS DEMONSTRATION");
    std::println("========================================");

    demonstrateBasicGettersSetters();
    demonstrateGetterPatterns();
    demonstrateSetterPatterns();
    demonstrateAccessControl();
    demonstrateValidation();
    demonstrateComputedProperties();
    demonstrateDependentProperties();
    demonstrateRangeProperties();
    demonstrateUserProfile();
    demonstrateProductInventory();

    return 0;
}
