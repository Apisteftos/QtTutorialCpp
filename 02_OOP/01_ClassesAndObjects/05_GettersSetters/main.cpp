#include <iostream>
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
            cout << "⚠️  Name cannot be empty\n";
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
            cout << "⚠️  Invalid age: " << a << "\n";
        }
    }

    void display() const {
        cout << "👤 Person: " << name << ", Age: " << age << "\n";
    }
};

void demonstrateBasicGettersSetters() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: BASIC GETTERS/SETTERS ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Person person("Alice", 25);

    cout << "--- Using getters ---\n";
    cout << "Name: " << person.getName() << "\n";
    cout << "Age: " << person.getAge() << "\n";

    cout << "\n--- Using setters ---\n";
    person.setName("Alice Johnson");
    person.setAge(26);
    person.display();

    cout << "\n--- Invalid attempts ---\n";
    person.setName("");      // Empty name - rejected
    person.setAge(-5);       // Negative age - rejected
    person.setAge(200);      // Too old - rejected

    cout << "\n--- After invalid attempts ---\n";
    person.display();
    cout << "✅ Data integrity maintained!\n";
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
            cout << "   [Computing expensive value...]\n";
            // Simulate expensive calculation
            cachedValue = 0;
            for (int i = 0; i < 1000000; i++) {
                cachedValue += i % 100;
            }
            cacheValid = true;
        } else {
            cout << "   [Using cached value]\n";
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
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: GETTER PATTERNS        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    GetterPatterns obj;

    cout << "--- Pattern 1: Return by value ---\n";
    string name = obj.getName();
    cout << "Name: " << name << "\n";

    cout << "\n--- Pattern 2: Return by const reference ---\n";
    const vector<int>& data = obj.getData();
    cout << "Data size: " << data.size() << "\n";
    // data.push_back(10);  // ❌ Error! Can't modify const reference

    cout << "\n--- Pattern 3: Return pointer ---\n";
    const int* ptr = obj.getDynamicData();
    if (ptr) {
        cout << "Dynamic data: " << *ptr << "\n";
    }

    cout << "\n--- Pattern 4: Computed value ---\n";
    cout << "Sum: " << obj.getSum() << "\n";

    cout << "\n--- Pattern 5: Lazy evaluation ---\n";
    cout << "First call:\n";
    obj.getExpensiveValue();
    cout << "Second call:\n";
    obj.getExpensiveValue();

    cout << "\nAfter modifying data:\n";
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
            cout << "⚠️  Invalid age\n";
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
        cout << "⚠️  Salary cannot be negative\n";
        return false;
    }

    // Pattern 4: Clamp to valid range
    void setAgeClamp(int a) {
        if (a < 0) {
            age = 0;
            cout << "⚠️  Age clamped to 0\n";
        } else if (a > 150) {
            age = 150;
            cout << "⚠️  Age clamped to 150\n";
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
            cout << "⚠️  Empty name after trimming\n";
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
        cout << "Name: " << name << ", Age: " << age << ", Salary: $" << salary << "\n";
    }
};

void demonstrateSetterPatterns() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: SETTER PATTERNS        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    SetterPatterns obj;

    cout << "--- Pattern 1: Basic validation ---\n";
    obj.setAge(25);
    obj.setAge(-5);  // Invalid

    cout << "\n--- Pattern 2: Exception on invalid ---\n";
    try {
        obj.setAgeStrict(200);  // Throws exception
    } catch (const exception& e) {
        cout << "❌ Exception: " << e.what() << "\n";
    }

    cout << "\n--- Pattern 3: Return bool ---\n";
    bool success = obj.setSalary(50000);
    cout << "Set salary: " << (success ? "✅ Success" : "❌ Failed") << "\n";

    success = obj.setSalary(-1000);
    cout << "Set negative: " << (success ? "✅ Success" : "❌ Failed") << "\n";

    cout << "\n--- Pattern 4: Clamp to range ---\n";
    obj.setAgeClamp(200);  // Clamped to 150

    cout << "\n--- Pattern 5: Normalize input ---\n";
    obj.setName("  alice johnson  ");  // Trimmed and capitalized
    obj.display();

    cout << "\n--- Pattern 6: Method chaining ---\n";
    obj.setNameChain("Bob Smith")
        .setAgeChain(30)
        .setSalaryChain(60000);
    obj.display();

    cout << "\n--- Pattern 7: Batch setter ---\n";
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
        cout << "✅ Write-only value set (but can't read it back!)\n";
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
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: ACCESS CONTROL         ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    AccessControl obj;

    cout << "--- Read-only property ---\n";
    cout << "Value: " << obj.getReadOnly() << "\n";
    // obj.setReadOnly("new");  // ❌ No setter available
    cout << "✓ Can read, but cannot modify\n";

    cout << "\n--- Write-only property ---\n";
    obj.setWriteOnly("secret_password");
    // string val = obj.getWriteOnly();  // ❌ No getter available
    cout << "✓ Can write, but cannot read back\n";

    cout << "\n--- Full access property ---\n";
    cout << "Value: " << obj.getFullAccess() << "\n";
    obj.setFullAccess("modified");
    cout << "New value: " << obj.getFullAccess() << "\n";
    cout << "✓ Can both read and write\n";

    cout << "\n💡 Use cases:\n";
    cout << "   Read-only: ID, creation date, calculated values\n";
    cout << "   Write-only: Passwords, secure tokens\n";
    cout << "   Full access: Normal properties\n";
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
            cout << "✅ Email set: " << email << "\n";
        } else {
            cout << "❌ Invalid email format: " << e << "\n";
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
            cout << "✅ Phone set: " << phoneNumber << "\n";
        } else {
            cout << "❌ Invalid phone number: " << phone << "\n";
        }
    }

    string getPhoneNumber() const {
        return phoneNumber;
    }

    // Zip code with validation
    void setZipCode(int zip) {
        if (isValidZipCode(zip)) {
            zipCode = zip;
            cout << "✅ Zip code set: " << zipCode << "\n";
        } else {
            cout << "❌ Invalid zip code: " << zip << " (must be 5 digits)\n";
        }
    }

    int getZipCode() const {
        return zipCode;
    }

    void display() const {
        cout << "📧 Email: " << (email.empty() ? "[not set]" : email) << "\n";
        cout << "📞 Phone: " << (phoneNumber.empty() ? "[not set]" : phoneNumber) << "\n";
        cout << "📮 Zip: " << (zipCode == 0 ? "[not set]" : to_string(zipCode)) << "\n";
    }
};

void demonstrateValidation() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: VALIDATION STRATEGIES  ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    ValidationExample obj;

    cout << "--- Email validation ---\n";
    obj.setEmail("user@example.com");    // ✅ Valid
    obj.setEmail("invalid.email");       // ❌ Invalid
    obj.setEmail("@example.com");        // ❌ Invalid

    cout << "\n--- Phone validation and formatting ---\n";
    obj.setPhoneNumber("1234567890");    // ✅ Valid, formatted
    obj.setPhoneNumber("(123) 456-7890"); // ✅ Valid, reformatted
    obj.setPhoneNumber("123");           // ❌ Invalid

    cout << "\n--- Zip code validation ---\n";
    obj.setZipCode(12345);               // ✅ Valid
    obj.setZipCode(123);                 // ❌ Invalid
    obj.setZipCode(123456);              // ❌ Invalid

    cout << "\n--- Final state ---\n";
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
        cout << "📐 Rectangle: " << width << " × " << height << "\n";
        cout << "   Area: " << getArea() << "\n";
        cout << "   Perimeter: " << getPerimeter() << "\n";
        cout << "   Diagonal: " << getDiagonal() << "\n";
        cout << "   Is square? " << (isSquare() ? "Yes" : "No") << "\n";
    }
};

void demonstrateComputedProperties() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: COMPUTED PROPERTIES    ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Rectangle rect(5.0, 3.0);

    cout << "--- Initial rectangle ---\n";
    rect.display();

    cout << "\n--- After changing dimensions ---\n";
    rect.setWidth(4.0);
    rect.setHeight(4.0);
    rect.display();

    cout << "\n💡 Computed properties:\n";
    cout << "   • Not stored, calculated on demand\n";
    cout << "   • Always consistent with base data\n";
    cout << "   • Only getters, no setters\n";
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
            cout << "⚠️  Temperature below absolute zero!\n";
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
        cout << "🌡️  Temperature:\n";
        cout << "   Celsius:    " << getCelsius() << "°C\n";
        cout << "   Fahrenheit: " << getFahrenheit() << "°F\n";
        cout << "   Kelvin:     " << getKelvin() << "K\n";
    }
};

void demonstrateDependentProperties() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 7: DEPENDENT PROPERTIES   ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Temperature temp(25.0);  // 25°C

    cout << "--- Initial (25°C) ---\n";
    temp.display();

    cout << "\n--- Set Fahrenheit to 100 ---\n";
    temp.setFahrenheit(100);
    temp.display();

    cout << "\n--- Set Kelvin to 300 ---\n";
    temp.setKelvin(300);
    temp.display();

    cout << "\n💡 All three properties stay in sync!\n";
    cout << "   • Only Celsius is stored\n";
    cout << "   • Others computed on the fly\n";
    cout << "   • Setting any updates all\n";
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
            cout << "⚠️  Clamped to 0%\n";
        } else if (v > 1) {
            value = 1;
            cout << "⚠️  Clamped to 100%\n";
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
        cout << "📊 Percentage: " << getPercent() << "% (decimal: " << getValue() << ")\n";
    }
};

void demonstrateRangeProperties() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 8: RANGE PROPERTIES       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Percentage pct;

    cout << "--- Setting as decimal ---\n";
    pct.setValue(0.75);
    pct.display();

    cout << "\n--- Setting as percentage ---\n";
    pct.setPercent(50);
    pct.display();

    cout << "\n--- Out of range values ---\n";
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
            cout << "✅ Username set: " << username << "\n";
            return true;
        }
        cout << "❌ Invalid username (3-20 chars, alphanumeric + underscore)\n";
        return false;
    }

    // Email - with validation
    string getEmail() const { return email; }

    bool setEmail(const string& e) {
        if (isValidEmail(e)) {
            email = e;
            cout << "✅ Email set: " << email << "\n";
            return true;
        }
        cout << "❌ Invalid email format\n";
        return false;
    }

    // Age - 13-120 range
    int getAge() const { return age; }

    bool setAge(int a) {
        if (a >= 13 && a <= 120) {
            age = a;
            return true;
        }
        cout << "❌ Age must be between 13 and 120\n";
        return false;
    }

    // Bio - max 500 characters
    string getBio() const { return bio; }

    void setBio(const string& b) {
        if (b.length() <= 500) {
            bio = b;
        } else {
            bio = b.substr(0, 500);
            cout << "⚠️  Bio truncated to 500 characters\n";
        }
    }

    // Verified - read-only for users
    bool getIsVerified() const { return isVerified; }

    // Private method - only system can verify
    void verify() {
        isVerified = true;
        cout << "✅ Profile verified!\n";
    }

    void display() const {
        cout << "╔════════════════════════════════╗\n";
        cout << "║       USER PROFILE             ║\n";
        cout << "╚════════════════════════════════╝\n";
        cout << "Username: " << username << (isVerified ? " ✓" : "") << "\n";
        cout << "Email:    " << email << "\n";
        cout << "Age:      " << age << "\n";
        cout << "Bio:      " << (bio.empty() ? "[empty]" : bio.substr(0, 50))
             << (bio.length() > 50 ? "..." : "") << "\n";
    }

    // For demo purposes - normally private
    void systemVerify() { verify(); }
};

void demonstrateUserProfile() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 9: USER PROFILE           ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    UserProfile profile;

    cout << "--- Setting up profile ---\n";
    profile.setUsername("alice_2024");
    profile.setEmail("alice@example.com");
    profile.setAge(25);
    profile.setBio("Software developer passionate about C++ and system programming");

    cout << "\n--- Invalid attempts ---\n";
    profile.setUsername("ab");            // Too short
    profile.setUsername("user@invalid");  // Invalid character
    profile.setEmail("invalid");          // Invalid format
    profile.setAge(10);                   // Too young

    cout << "\n--- Profile display ---\n";
    profile.display();

    cout << "\n--- System verification ---\n";
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
        cout << "❌ Price cannot be negative\n";
        return false;
    }

    // Quantity with validation
    int getQuantity() const { return quantity; }

    bool setQuantity(int q) {
        if (q >= 0) {
            quantity = q;
            return true;
        }
        cout << "❌ Quantity cannot be negative\n";
        return false;
    }

    // SKU - read-only after creation
    string getSku() const { return sku; }

    // Stock operations
    bool addStock(int amount) {
        if (amount > 0) {
            quantity += amount;
            cout << "✅ Added " << amount << " units. New stock: " << quantity << "\n";
            return true;
        }
        return false;
    }

    bool removeStock(int amount) {
        if (amount > 0 && amount <= quantity) {
            quantity -= amount;
            cout << "✅ Removed " << amount << " units. Remaining: " << quantity << "\n";
            return true;
        }
        if (amount > quantity) {
            cout << "❌ Insufficient stock\n";
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
        cout << "📦 Product: " << name << " [" << sku << "]\n";
        cout << "   Price: $" << price << "\n";
        cout << "   Stock: " << quantity;
        if (isLowStock()) cout << " ⚠️  LOW STOCK";
        if (!isInStock()) cout << " ❌ OUT OF STOCK";
        cout << "\n";
        cout << "   Total Value: $" << getTotalValue() << "\n";
    }
};

void demonstrateProductInventory() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 10: PRODUCT INVENTORY     ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Product product("Laptop", 999.99, 50, "LAP-001");

    cout << "--- Initial product ---\n";
    product.display();

    cout << "\n--- Stock operations ---\n";
    product.removeStock(45);  // Sell 45 units
    product.display();

    cout << "\n";
    product.removeStock(3);   // Sell 3 more (low stock warning)
    product.display();

    cout << "\n";
    product.addStock(100);    // Restock
    product.display();

    cout << "\n--- Invalid operations ---\n";
    product.setPrice(-100);   // Negative price - rejected
    product.removeStock(200); // Insufficient stock
}

// ============================================
// MAIN DEMONSTRATION
// ============================================

int main() {
    cout << "========================================\n";
    cout << "GETTERS AND SETTERS DEMONSTRATION\n";
    cout << "========================================\n";

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

    // ============================================
    // Comprehensive Summary
    // ============================================
    cout << "\n========================================\n";
    cout << "WHAT ARE GETTERS AND SETTERS?\n";
    cout << "========================================\n";
    cout << "GETTERS (Accessors):\n";
    cout << "  • Functions that return private member values\n";
    cout << "  • Should always be const\n";
    cout << "  • Provide read access to data\n";
    cout << "  • Naming: get + PropertyName (e.g., getName())\n";
    cout << "\n";
    cout << "SETTERS (Mutators):\n";
    cout << "  • Functions that modify private member values\n";
    cout << "  • Should validate input\n";
    cout << "  • Provide controlled write access\n";
    cout << "  • Naming: set + PropertyName (e.g., setName())\n";

    cout << "\n========================================\n";
    cout << "BASIC PATTERN\n";
    cout << "========================================\n";
    cout << "class MyClass {\n";
    cout << "private:\n";
    cout << "    int value;\n";
    cout << "    \n";
    cout << "public:\n";
    cout << "    // Getter - const, returns value\n";
    cout << "    int getValue() const {\n";
    cout << "        return value;\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    // Setter - validates, then sets\n";
    cout << "    void setValue(int v) {\n";
    cout << "        if (v >= 0) {  // Validation\n";
    cout << "            value = v;\n";
    cout << "        }\n";
    cout << "    }\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "GETTER PATTERNS\n";
    cout << "========================================\n";
    cout << "1. Return by value (simple types):\n";
    cout << "   int getAge() const { return age; }\n";
    cout << "\n";
    cout << "2. Return by const reference (large objects):\n";
    cout << "   const string& getName() const { return name; }\n";
    cout << "\n";
    cout << "3. Return pointer (nullable):\n";
    cout << "   const Data* getData() const { return ptr; }\n";
    cout << "\n";
    cout << "4. Computed/calculated:\n";
    cout << "   double getArea() const { return w * h; }\n";
    cout << "\n";
    cout << "5. Lazy evaluation:\n";
    cout << "   int getValue() const {\n";
    cout << "       if (!cached) calculate();\n";
    cout << "       return cachedValue;\n";
    cout << "   }\n";

    cout << "\n========================================\n";
    cout << "SETTER PATTERNS\n";
    cout << "========================================\n";
    cout << "1. Basic validation:\n";
    cout << "   void setValue(int v) {\n";
    cout << "       if (valid(v)) value = v;\n";
    cout << "   }\n";
    cout << "\n";
    cout << "2. Throw exception:\n";
    cout << "   void setValue(int v) {\n";
    cout << "       if (!valid(v)) throw invalid_argument(\"...\");\n";
    cout << "       value = v;\n";
    cout << "   }\n";
    cout << "\n";
    cout << "3. Return bool:\n";
    cout << "   bool setValue(int v) {\n";
    cout << "       if (valid(v)) { value = v; return true; }\n";
    cout << "       return false;\n";
    cout << "   }\n";
    cout << "\n";
    cout << "4. Clamp to range:\n";
    cout << "   void setValue(int v) {\n";
    cout << "       value = max(min(v, MAX), MIN);\n";
    cout << "   }\n";
    cout << "\n";
    cout << "5. Normalize/sanitize:\n";
    cout << "   void setName(string n) {\n";
    cout << "       name = trim(capitalize(n));\n";
    cout << "   }\n";
    cout << "\n";
    cout << "6. Method chaining:\n";
    cout << "   MyClass& setValue(int v) {\n";
    cout << "       value = v;\n";
    cout << "       return *this;\n";
    cout << "   }\n";

    cout << "\n========================================\n";
    cout << "WHY USE GETTERS/SETTERS?\n";
    cout << "========================================\n";
    cout << "1. Validation:\n";
    cout << "   • Check values before setting\n";
    cout << "   • Enforce business rules\n";
    cout << "   • Prevent invalid states\n";
    cout << "\n";
    cout << "2. Encapsulation:\n";
    cout << "   • Hide internal representation\n";
    cout << "   • Users don't know how data is stored\n";
    cout << "   • Can change implementation later\n";
    cout << "\n";
    cout << "3. Flexibility:\n";
    cout << "   • Store data differently internally\n";
    cout << "   • Compute values on demand\n";
    cout << "   • Cache expensive calculations\n";
    cout << "\n";
    cout << "4. Control:\n";
    cout << "   • Read-only properties (getter only)\n";
    cout << "   • Write-only properties (setter only)\n";
    cout << "   • Full access (both)\n";
    cout << "\n";
    cout << "5. Debugging:\n";
    cout << "   • Add logging in setter\n";
    cout << "   • Set breakpoints easily\n";
    cout << "   • Track modifications\n";
    cout << "\n";
    cout << "6. Side effects:\n";
    cout << "   • Notify observers\n";
    cout << "   • Update dependent values\n";
    cout << "   • Trigger events\n";

    cout << "\n========================================\n";
    cout << "TYPES OF PROPERTIES\n";
    cout << "========================================\n";
    cout << "1. Stored Properties:\n";
    cout << "   • Directly stored in member variable\n";
    cout << "   • getter returns value, setter stores\n";
    cout << "   • Example: name, age, price\n";
    cout << "\n";
    cout << "2. Computed Properties:\n";
    cout << "   • Calculated on demand\n";
    cout << "   • Only getter, no setter\n";
    cout << "   • Example: area, total, isValid\n";
    cout << "\n";
    cout << "3. Dependent Properties:\n";
    cout << "   • Multiple views of same data\n";
    cout << "   • Setting one updates others\n";
    cout << "   • Example: Celsius/Fahrenheit/Kelvin\n";
    cout << "\n";
    cout << "4. Validated Properties:\n";
    cout << "   • Setter enforces rules\n";
    cout << "   • Invalid values rejected\n";
    cout << "   • Example: email, phone, age\n";
    cout << "\n";
    cout << "5. Lazy Properties:\n";
    cout << "   • Calculated once, then cached\n";
    cout << "   • Expensive to compute\n";
    cout << "   • Example: file content, database query\n";

    cout << "\n========================================\n";
    cout << "ACCESS CONTROL PATTERNS\n";
    cout << "========================================\n";
    cout << "Read-Only (getter only):\n";
    cout << "  class MyClass {\n";
    cout << "      int id;\n";
    cout << "  public:\n";
    cout << "      int getId() const { return id; }\n";
    cout << "      // No setter - can't modify after creation\n";
    cout << "  };\n";
    cout << "\n";
    cout << "Write-Only (setter only):\n";
    cout << "  class MyClass {\n";
    cout << "      string password;\n";
    cout << "  public:\n";
    cout << "      void setPassword(string p) { password = hash(p); }\n";
    cout << "      // No getter - security!\n";
    cout << "      bool verify(string p) { return hash(p) == password; }\n";
    cout << "  };\n";
    cout << "\n";
    cout << "Full Access (both):\n";
    cout << "  class MyClass {\n";
    cout << "      string name;\n";
    cout << "  public:\n";
    cout << "      string getName() const { return name; }\n";
    cout << "      void setName(string n) { name = n; }\n";
    cout << "  };\n";

    cout << "\n========================================\n";
    cout << "VALIDATION STRATEGIES\n";
    cout << "========================================\n";
    cout << "1. Range checking:\n";
    cout << "   if (age >= 0 && age <= 150)\n";
    cout << "\n";
    cout << "2. Format validation:\n";
    cout << "   if (email.contains('@') && email.contains('.'))\n";
    cout << "\n";
    cout << "3. Length validation:\n";
    cout << "   if (username.length() >= 3 && username.length() <= 20)\n";
    cout << "\n";
    cout << "4. Pattern matching:\n";
    cout << "   if (regex_match(phone, phonePattern))\n";
    cout << "\n";
    cout << "5. Business rules:\n";
    cout << "   if (price > 0 && quantity >= 0 && price * quantity < MAX)\n";

    cout << "\n========================================\n";
    cout << "BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "✅ DO:\n";
    cout << "  • Always make getters const\n";
    cout << "  • Validate in setters\n";
    cout << "  • Return const reference for large objects\n";
    cout << "  • Use meaningful names (getValue, setValue)\n";
    cout << "  • Keep getters/setters simple\n";
    cout << "  • Document validation rules\n";
    cout << "  • Consider computed properties\n";
    cout << "  • Use read-only for immutable data\n";
    cout << "\n";
    cout << "❌ DON'T:\n";
    cout << "  • Forget const on getters\n";
    cout << "  • Skip validation in setters\n";
    cout << "  • Return non-const reference to private data\n";
    cout << "  • Make getters/setters too complex\n";
    cout << "  • Expose internal representation\n";
    cout << "  • Allow invalid states\n";

    cout << "\n========================================\n";
    cout << "COMMON MISTAKES\n";
    cout << "========================================\n";
    cout << "❌ Mistake 1: Non-const getter\n";
    cout << "   int getValue() { return value; }  // Missing const!\n";
    cout << "\n";
    cout << "❌ Mistake 2: No validation\n";
    cout << "   void setAge(int a) { age = a; }  // No checking!\n";
    cout << "\n";
    cout << "❌ Mistake 3: Returning non-const reference\n";
    cout << "   vector<int>& getData() { return data; }  // Exposes internals!\n";
    cout << "\n";
    cout << "❌ Mistake 4: Setter modifying other state\n";
    cout << "   void setX(int x) {\n";
    cout << "       this->x = x;\n";
    cout << "       y = 0;  // Side effect - confusing!\n";
    cout << "   }\n";

    cout << "\n========================================\n";
    cout << "WHEN NOT TO USE GETTERS/SETTERS\n";
    cout << "========================================\n";
    cout << "For simple data structures (POD):\n";
    cout << "\n";
    cout << "struct Point {  // Simple container\n";
    cout << "    int x, y;   // Public is OK\n";
    cout << "};\n";
    cout << "\n";
    cout << "Use public data when:\n";
    cout << "  • Pure data container (no behavior)\n";
    cout << "  • No validation needed\n";
    cout << "  • No computed properties\n";
    cout << "  • Simple aggregation\n";
    cout << "\n";
    cout << "Otherwise: Use getters/setters!\n";

    cout << "\n========================================\n";
    cout << "QUICK REFERENCE\n";
    cout << "========================================\n";
    cout << "// Simple getter/setter\n";
    cout << "class MyClass {\n";
    cout << "    int value;\n";
    cout << "public:\n";
    cout << "    int getValue() const { return value; }\n";
    cout << "    void setValue(int v) { value = v; }\n";
    cout << "};\n";
    cout << "\n";
    cout << "// With validation\n";
    cout << "void setAge(int a) {\n";
    cout << "    if (a >= 0 && a <= 150) age = a;\n";
    cout << "}\n";
    cout << "\n";
    cout << "// Computed property\n";
    cout << "double getArea() const {\n";
    cout << "    return width * height;\n";
    cout << "}\n";
    cout << "\n";
    cout << "// Method chaining\n";
    cout << "MyClass& setValue(int v) {\n";
    cout << "    value = v;\n";
    cout << "    return *this;\n";
    cout << "}\n";

    cout << "\n========================================\n";
    cout << "KEY TAKEAWAYS\n";
    cout << "========================================\n";
    cout << "1. Getters provide read access (always const)\n";
    cout << "2. Setters provide write access (with validation)\n";
    cout << "3. Encapsulation: Hide internal representation\n";
    cout << "4. Validation: Prevent invalid states\n";
    cout << "5. Flexibility: Can change implementation\n";
    cout << "6. Computed properties: Calculate on demand\n";
    cout << "7. Dependent properties: Keep values in sync\n";
    cout << "8. Access control: Read-only, write-only, full\n";
    cout << "9. Always validate in setters\n";
    cout << "10. Foundation of data encapsulation\n";

    return 0;
}
