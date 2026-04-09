#include <iostream>
#include <string>
#include <vector>
#include <memory>
using namespace std;

// ============================================
// WHAT ARE STATIC MEMBERS?
// ============================================

/*
 * STATIC MEMBERS:
 * - Belong to the class, not individual objects
 * - Shared by all instances of the class
 * - Exist even if no objects are created
 * - Only one copy exists (class-level, not object-level)
 *
 * STATIC DATA MEMBERS:
 * - Shared variable across all objects
 * - Must be initialized outside class definition
 * - Accessed via ClassName::member or object.member
 *
 * STATIC MEMBER FUNCTIONS:
 * - Can be called without an object
 * - No 'this' pointer
 * - Can only access static members
 * - Accessed via ClassName::function()
 *
 * COMMON USES:
 * 1. Object counting
 * 2. Shared configuration
 * 3. Factory methods
 * 4. Utility functions
 * 5. Singleton pattern
 */

// ============================================
// EXAMPLE 1: BASIC STATIC MEMBERS
// ============================================

class Counter {
private:
    int instanceValue;        // Instance member (each object has own)
    static int objectCount;   // Static member (shared by all)

public:
    Counter(int value) : instanceValue(value) {
        objectCount++;  // Increment shared counter
        cout << "  Counter created. Total objects: " << objectCount << "\n";
    }

    ~Counter() {
        objectCount--;  // Decrement shared counter
        cout << "  Counter destroyed. Remaining objects: " << objectCount << "\n";
    }

    void displayInstance() const {
        cout << "  Instance value: " << instanceValue << "\n";
    }

    static void displayCount() {
        cout << "  Total Counter objects: " << objectCount << "\n";
        // cout << instanceValue;  // ❌ Error! Can't access instance members
    }

    static int getCount() {
        return objectCount;
    }
};

// CRITICAL: Static members must be initialized outside class
int Counter::objectCount = 0;  // Initialize static member

void demonstrateBasicStatic() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: BASIC STATIC MEMBERS   ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Initial state ---\n";
    Counter::displayCount();  // Can call without object!

    cout << "\n--- Creating objects ---\n";
    Counter c1(10);
    Counter c2(20);
    Counter c3(30);

    cout << "\n--- Checking count ---\n";
    Counter::displayCount();  // Via class name
    c1.displayCount();        // Via object (also works)

    cout << "\n--- Using getCount() ---\n";
    cout << "  Count via static function: " << Counter::getCount() << "\n";

    cout << "\n💡 Static members:\n";
    cout << "   • Shared by ALL objects\n";
    cout << "   • Only ONE copy exists\n";
    cout << "   • Accessible via ClassName::member\n";

    cout << "\n--- Cleanup ---\n";
}

// ============================================
// EXAMPLE 2: STATIC vs INSTANCE MEMBERS
// ============================================

class Student {
private:
    string name;              // Instance - each object has own
    int grade;                // Instance - each object has own
    static int totalStudents; // Static - shared by all
    static string schoolName; // Static - shared by all

public:
    Student(string n, int g) : name(n), grade(g) {
        totalStudents++;
    }

    ~Student() {
        totalStudents--;
    }

    // Instance function - has 'this', can access all members
    void displayStudent() const {
        cout << "  Student: " << name
             << ", Grade: " << grade
             << ", School: " << schoolName << "\n";
    }

    // Static function - no 'this', can only access static members
    static void displaySchoolInfo() {
        cout << "  School: " << schoolName << "\n";
        cout << "  Total Students: " << totalStudents << "\n";
        // cout << name;  // ❌ Error! No 'this' pointer
    }

    static void setSchoolName(const string& name) {
        schoolName = name;
    }

    static int getTotalStudents() {
        return totalStudents;
    }
};

// Initialize static members
int Student::totalStudents = 0;
string Student::schoolName = "Unknown School";

void demonstrateStaticVsInstance() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: STATIC vs INSTANCE     ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Setting school name (no objects yet) ---\n";
    Student::setSchoolName("MIT");
    Student::displaySchoolInfo();

    cout << "\n--- Creating students ---\n";
    Student s1("Alice", 95);
    Student s2("Bob", 87);
    Student s3("Charlie", 92);

    cout << "\n--- Display individual students ---\n";
    s1.displayStudent();
    s2.displayStudent();
    s3.displayStudent();

    cout << "\n--- Display school info ---\n";
    Student::displaySchoolInfo();

    cout << "\n--- Memory visualization ---\n";
    cout << "  INSTANCE MEMBERS (each object has own):\n";
    cout << "    s1.name = \"Alice\"\n";
    cout << "    s2.name = \"Bob\"\n";
    cout << "    s3.name = \"Charlie\"\n";
    cout << "\n";
    cout << "  STATIC MEMBERS (shared by all):\n";
    cout << "    Student::schoolName = \"MIT\" (ONE copy)\n";
    cout << "    Student::totalStudents = 3 (ONE copy)\n";

    cout << "\n--- Cleanup ---\n";
}

// ============================================
// EXAMPLE 3: OBJECT COUNTING
// ============================================

class Tracker {
private:
    string name;
    static int activeObjects;
    static int totalCreated;
    static int totalDestroyed;

public:
    Tracker(const string& n) : name(n) {
        activeObjects++;
        totalCreated++;
        cout << "  [" << name << "] created. Active: " << activeObjects << "\n";
    }

    // Copy constructor
    Tracker(const Tracker& other) : name(other.name + "_Copy") {
        activeObjects++;
        totalCreated++;
        cout << "  [" << name << "] copied. Active: " << activeObjects << "\n";
    }

    ~Tracker() {
        activeObjects--;
        totalDestroyed++;
        cout << "  [" << name << "] destroyed. Active: " << activeObjects << "\n";
    }

    static void displayStats() {
        cout << "\n  === Statistics ===\n";
        cout << "  Currently active: " << activeObjects << "\n";
        cout << "  Total created:    " << totalCreated << "\n";
        cout << "  Total destroyed:  " << totalDestroyed << "\n";
    }

    static int getActiveCount() { return activeObjects; }
};

// Initialize static members
int Tracker::activeObjects = 0;
int Tracker::totalCreated = 0;
int Tracker::totalDestroyed = 0;

void demonstrateObjectCounting() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: OBJECT COUNTING        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Initial statistics ---\n";
    Tracker::displayStats();

    cout << "\n--- Creating objects in scope ---\n";
    {
        Tracker t1("First");
        Tracker t2("Second");
        Tracker::displayStats();

        cout << "\n--- Creating copy ---\n";
        Tracker t3 = t1;
        Tracker::displayStats();

        cout << "\n--- Exiting scope ---\n";
    }

    cout << "\n--- After scope exit ---\n";
    Tracker::displayStats();

    cout << "\n✅ Static members track objects across their lifetime!\n";
}

// ============================================
// EXAMPLE 4: SHARED CONFIGURATION
// ============================================

class DatabaseConnection {
private:
    string connectionId;
    static string serverAddress;
    static int serverPort;
    static int maxConnections;
    static int currentConnections;

public:
    DatabaseConnection(const string& id) : connectionId(id) {
        if (currentConnections >= maxConnections) {
            cout << "  ❌ Cannot create connection: limit reached\n";
            return;
        }
        currentConnections++;
        cout << "  ✅ Connection [" << connectionId << "] established to "
             << serverAddress << ":" << serverPort << "\n";
    }

    ~DatabaseConnection() {
        currentConnections--;
        cout << "  Connection [" << connectionId << "] closed\n";
    }

    // Static configuration methods
    static void configure(const string& addr, int port, int maxConn) {
        serverAddress = addr;
        serverPort = port;
        maxConnections = maxConn;
        cout << "  Database configured: " << addr << ":" << port
             << " (max: " << maxConn << ")\n";
    }

    static void displayConfig() {
        cout << "  Server: " << serverAddress << ":" << serverPort << "\n";
        cout << "  Max connections: " << maxConnections << "\n";
        cout << "  Current connections: " << currentConnections << "\n";
    }

    static int getAvailableConnections() {
        return maxConnections - currentConnections;
    }
};

// Initialize static members
string DatabaseConnection::serverAddress = "localhost";
int DatabaseConnection::serverPort = 3306;
int DatabaseConnection::maxConnections = 5;
int DatabaseConnection::currentConnections = 0;

void demonstrateSharedConfiguration() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: SHARED CONFIGURATION   ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Default configuration ---\n";
    DatabaseConnection::displayConfig();

    cout << "\n--- Changing configuration (affects ALL connections) ---\n";
    DatabaseConnection::configure("db.example.com", 5432, 3);
    DatabaseConnection::displayConfig();

    cout << "\n--- Creating connections ---\n";
    DatabaseConnection conn1("App-1");
    DatabaseConnection conn2("App-2");
    DatabaseConnection conn3("App-3");

    cout << "\n--- Trying to exceed limit ---\n";
    DatabaseConnection conn4("App-4");  // Should fail

    cout << "\n--- Current status ---\n";
    DatabaseConnection::displayConfig();

    cout << "\n💡 Static members = shared configuration!\n";
    cout << "   All objects use same settings\n";

    cout << "\n--- Cleanup ---\n";
}

// ============================================
// EXAMPLE 5: STATIC CONST MEMBERS
// ============================================

class MathConstants {
private:
    // Static const can be initialized in class (integral types)
    static const int MAX_ITERATIONS = 1000;
    static const int BUFFER_SIZE = 256;

    // Non-integral static const must be initialized outside
    static const double PI;
    static const double E;

public:
    static void displayConstants() {
        cout << "  MAX_ITERATIONS: " << MAX_ITERATIONS << "\n";
        cout << "  BUFFER_SIZE:    " << BUFFER_SIZE << "\n";
        cout << "  PI:             " << PI << "\n";
        cout << "  E:              " << E << "\n";
    }

    static double circleArea(double radius) {
        return PI * radius * radius;
    }

    static double sphereVolume(double radius) {
        return (4.0 / 3.0) * PI * radius * radius * radius;
    }

    static int getMaxIterations() { return MAX_ITERATIONS; }
};

// Initialize non-integral static const members
const double MathConstants::PI = 3.14159265359;
const double MathConstants::E = 2.71828182846;

void demonstrateStaticConst() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: STATIC CONST MEMBERS   ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Math constants ---\n";
    MathConstants::displayConstants();

    cout << "\n--- Using constants ---\n";
    double radius = 5.0;
    cout << "  Circle area (r=" << radius << "): "
         << MathConstants::circleArea(radius) << "\n";
    cout << "  Sphere volume (r=" << radius << "): "
         << MathConstants::sphereVolume(radius) << "\n";

    cout << "\n💡 Static const members:\n";
    cout << "   • Shared constants for all objects\n";
    cout << "   • Integral types can be initialized in-class\n";
    cout << "   • Non-integral must be initialized outside\n";
    cout << "   • Common for configuration constants\n";
}

// ============================================
// EXAMPLE 6: STATIC MEMBER FUNCTION RESTRICTIONS
// ============================================

class RestrictionsDemo {
private:
    int instanceVar;
    static int staticVar;

public:
    RestrictionsDemo(int val) : instanceVar(val) { }

    // Non-static function - can access everything
    void nonStaticFunction() {
        cout << "  Non-static function:\n";
        cout << "    Can access instanceVar: " << instanceVar << " ✅\n";
        cout << "    Can access staticVar: " << staticVar << " ✅\n";
        cout << "    Has 'this' pointer ✅\n";
    }

    // Static function - restricted access
    static void staticFunction() {
        cout << "  Static function:\n";
        // cout << instanceVar;  // ❌ Error! No 'this' pointer
        cout << "    Can access staticVar: " << staticVar << " ✅\n";
        cout << "    Cannot access instanceVar ❌\n";
        cout << "    No 'this' pointer ❌\n";

        // Can call other static functions
        helperStaticFunction();

        // Cannot call non-static functions directly
        // nonStaticFunction();  // ❌ Error!
    }

    static void helperStaticFunction() {
        cout << "    Helper static function called\n";
    }

    // Static function can work with objects passed as parameters
    static void processObject(const RestrictionsDemo& obj) {
        cout << "  Static processing object with instanceVar: "
             << obj.instanceVar << "\n";
    }
};

int RestrictionsDemo::staticVar = 100;

void demonstrateStaticRestrictions() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: STATIC RESTRICTIONS    ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    RestrictionsDemo obj(42);

    cout << "--- Non-static function ---\n";
    obj.nonStaticFunction();

    cout << "\n--- Static function ---\n";
    RestrictionsDemo::staticFunction();

    cout << "\n--- Static function with object parameter ---\n";
    RestrictionsDemo::processObject(obj);

    cout << "\n💡 Static function restrictions:\n";
    cout << "   ❌ No 'this' pointer\n";
    cout << "   ❌ Cannot access instance members directly\n";
    cout << "   ✅ Can access static members\n";
    cout << "   ✅ Can call other static functions\n";
    cout << "   ✅ Can work with objects passed as parameters\n";
}

// ============================================
// EXAMPLE 7: FACTORY PATTERN
// ============================================

class Product {
private:
    string name;
    int id;
    static int nextId;  // Auto-incrementing ID

    // Private constructor - can only be created via factory
    Product(const string& n) : name(n), id(nextId++) {
        cout << "  Product created: " << name << " (ID: " << id << ")\n";
    }

public:
    // Static factory methods
    static Product createBasic() {
        return Product("Basic Product");
    }

    static Product createPremium() {
        return Product("Premium Product");
    }

    static Product createCustom(const string& name) {
        return Product(name);
    }

    void display() const {
        cout << "  Product: " << name << " (ID: " << id << ")\n";
    }

    static int getNextId() {
        return nextId;
    }

    static void resetIdCounter() {
        nextId = 1;
        cout << "  ID counter reset to 1\n";
    }
};

int Product::nextId = 1;

void demonstrateFactoryPattern() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 7: FACTORY PATTERN        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating products via factory methods ---\n";
    Product p1 = Product::createBasic();
    Product p2 = Product::createPremium();
    Product p3 = Product::createCustom("Special Edition");

    cout << "\n--- Displaying products ---\n";
    p1.display();
    p2.display();
    p3.display();

    cout << "\n--- Next ID ---\n";
    cout << "  Next product will have ID: " << Product::getNextId() << "\n";

    cout << "\n✅ Factory pattern benefits:\n";
    cout << "   • Controlled object creation\n";
    cout << "   • Auto-incrementing IDs\n";
    cout << "   • Named constructors\n";
    cout << "   • Private constructor ensures factory use\n";
}

// ============================================
// EXAMPLE 8: SINGLETON PATTERN
// ============================================

class Logger {
private:
    string filename;
    static Logger* instance;  // Single instance

    // Private constructor - prevents direct instantiation
    Logger(const string& file) : filename(file) {
        cout << "  Logger initialized with file: " << filename << "\n";
    }

    // Delete copy constructor and assignment
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

public:
    // Static method to get the single instance
    static Logger* getInstance() {
        if (instance == nullptr) {
            instance = new Logger("application.log");
        }
        return instance;
    }

    void log(const string& message) {
        cout << "  [LOG to " << filename << "]: " << message << "\n";
    }

    static void cleanup() {
        if (instance != nullptr) {
            delete instance;
            instance = nullptr;
            cout << "  Logger cleaned up\n";
        }
    }
};

Logger* Logger::instance = nullptr;

void demonstrateSingleton() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 8: SINGLETON PATTERN      ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Getting logger instance ---\n";
    Logger* logger1 = Logger::getInstance();
    logger1->log("First message");

    cout << "\n--- Getting logger instance again ---\n";
    Logger* logger2 = Logger::getInstance();
    logger2->log("Second message");

    cout << "\n--- Checking if same instance ---\n";
    cout << "  logger1 address: " << logger1 << "\n";
    cout << "  logger2 address: " << logger2 << "\n";
    cout << "  Same instance? " << (logger1 == logger2 ? "Yes ✅" : "No ❌") << "\n";

    cout << "\n✅ Singleton pattern:\n";
    cout << "   • Only ONE instance exists\n";
    cout << "   • Global access point\n";
    cout << "   • Private constructor\n";
    cout << "   • Static instance pointer\n";

    cout << "\n--- Cleanup ---\n";
    Logger::cleanup();
}

// ============================================
// EXAMPLE 9: UTILITY CLASS
// ============================================

class StringUtils {
private:
    // All static - no instance needed
    StringUtils() = delete;  // Prevent instantiation

public:
    static string toUpper(const string& str) {
        string result = str;
        for (char& c : result) {
            c = toupper(c);
        }
        return result;
    }

    static string toLower(const string& str) {
        string result = str;
        for (char& c : result) {
            c = tolower(c);
        }
        return result;
    }

    static string reverse(const string& str) {
        string result = str;
        int n = result.length();
        for (int i = 0; i < n / 2; i++) {
            swap(result[i], result[n - 1 - i]);
        }
        return result;
    }

    static bool isPalindrome(const string& str) {
        return str == reverse(str);
    }

    static int countWords(const string& str) {
        int count = 0;
        bool inWord = false;
        for (char c : str) {
            if (isspace(c)) {
                inWord = false;
            } else if (!inWord) {
                inWord = true;
                count++;
            }
        }
        return count;
    }
};

void demonstrateUtilityClass() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 9: UTILITY CLASS          ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    string text = "Hello World";

    cout << "--- String utilities (all static) ---\n";
    cout << "  Original:     " << text << "\n";
    cout << "  Upper case:   " << StringUtils::toUpper(text) << "\n";
    cout << "  Lower case:   " << StringUtils::toLower(text) << "\n";
    cout << "  Reversed:     " << StringUtils::reverse(text) << "\n";
    cout << "  Word count:   " << StringUtils::countWords(text) << "\n";

    cout << "\n--- Palindrome check ---\n";
    string word1 = "radar";
    string word2 = "hello";
    cout << "  \"" << word1 << "\" is palindrome? "
         << (StringUtils::isPalindrome(word1) ? "Yes" : "No") << "\n";
    cout << "  \"" << word2 << "\" is palindrome? "
         << (StringUtils::isPalindrome(word2) ? "Yes" : "No") << "\n";

    cout << "\n✅ Utility class pattern:\n";
    cout << "   • All methods static\n";
    cout << "   • No instance needed\n";
    cout << "   • Constructor deleted\n";
    cout << "   • Pure functionality\n";
}

// ============================================
// EXAMPLE 10: REAL-WORLD - ID GENERATOR
// ============================================

class Employee {
private:
    int id;
    string name;
    string department;

    static int nextId;
    static int totalEmployees;
    static vector<int> availableIds;  // Recycled IDs

public:
    Employee(const string& n, const string& dept)
        : name(n), department(dept) {

        // Use recycled ID if available
        if (!availableIds.empty()) {
            id = availableIds.back();
            availableIds.pop_back();
            cout << "  Recycled ID " << id << " for " << name << "\n";
        } else {
            id = nextId++;
            cout << "  Assigned new ID " << id << " to " << name << "\n";
        }

        totalEmployees++;
    }

    ~Employee() {
        totalEmployees--;
        availableIds.push_back(id);  // Recycle ID
        cout << "  Employee " << name << " (ID: " << id << ") removed. ID recycled.\n";
    }

    void display() const {
        cout << "  [" << id << "] " << name << " - " << department << "\n";
    }

    static void displayStatistics() {
        cout << "\n  === Employee Statistics ===\n";
        cout << "  Total employees: " << totalEmployees << "\n";
        cout << "  Next new ID: " << nextId << "\n";
        cout << "  Recycled IDs available: " << availableIds.size() << "\n";
        if (!availableIds.empty()) {
            cout << "  Recycled IDs: ";
            for (int id : availableIds) {
                cout << id << " ";
            }
            cout << "\n";
        }
    }

    static int getTotalEmployees() {
        return totalEmployees;
    }
};

int Employee::nextId = 1000;
int Employee::totalEmployees = 0;
vector<int> Employee::availableIds;

void demonstrateIdGenerator() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 10: ID GENERATOR          ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating employees ---\n";
    Employee* e1 = new Employee("Alice", "Engineering");
    Employee* e2 = new Employee("Bob", "Marketing");
    Employee* e3 = new Employee("Charlie", "Sales");

    Employee::displayStatistics();

    cout << "\n--- Removing some employees ---\n";
    delete e1;
    delete e3;

    Employee::displayStatistics();

    cout << "\n--- Creating new employees (will reuse IDs) ---\n";
    Employee* e4 = new Employee("David", "HR");
    Employee* e5 = new Employee("Eve", "Finance");

    Employee::displayStatistics();

    cout << "\n--- Current employees ---\n";
    e2->display();
    e4->display();
    e5->display();

    cout << "\n✅ Static members enable:\n";
    cout << "   • Auto-incrementing IDs\n";
    cout << "   • ID recycling\n";
    cout << "   • Global statistics\n";
    cout << "   • Resource management\n";

    cout << "\n--- Cleanup ---\n";
    delete e2;
    delete e4;
    delete e5;
}

// ============================================
// MAIN DEMONSTRATION
// ============================================

int main() {
    cout << "========================================\n";
    cout << "STATIC MEMBERS DEMONSTRATION\n";
    cout << "========================================\n";

    demonstrateBasicStatic();
    demonstrateStaticVsInstance();
    demonstrateObjectCounting();
    demonstrateSharedConfiguration();
    demonstrateStaticConst();
    demonstrateStaticRestrictions();
    demonstrateFactoryPattern();
    demonstrateSingleton();
    demonstrateUtilityClass();
    demonstrateIdGenerator();

    // ============================================
    // Comprehensive Summary
    // ============================================
    cout << "\n========================================\n";
    cout << "WHAT ARE STATIC MEMBERS?\n";
    cout << "========================================\n";
    cout << "Static members belong to the CLASS, not individual objects.\n";
    cout << "\n";
    cout << "Key characteristics:\n";
    cout << "  • Shared by ALL objects of the class\n";
    cout << "  • Only ONE copy exists (class-level)\n";
    cout << "  • Exist even if no objects created\n";
    cout << "  • Can be accessed without an object\n";
    cout << "\n";
    cout << "Two types:\n";
    cout << "  1. Static data members (variables)\n";
    cout << "  2. Static member functions\n";

    cout << "\n========================================\n";
    cout << "STATIC DATA MEMBERS\n";
    cout << "========================================\n";
    cout << "Declaration in class:\n";
    cout << "  class MyClass {\n";
    cout << "      static int count;  // Declaration\n";
    cout << "  };\n";
    cout << "\n";
    cout << "Definition outside class (REQUIRED):\n";
    cout << "  int MyClass::count = 0;  // Definition & initialization\n";
    cout << "\n";
    cout << "Access:\n";
    cout << "  MyClass::count = 10;     // Via class name\n";
    cout << "  obj.count = 10;          // Via object (also works)\n";
    cout << "\n";
    cout << "Special case - static const integral:\n";
    cout << "  class MyClass {\n";
    cout << "      static const int MAX = 100;  // Can initialize in class\n";
    cout << "  };\n";

    cout << "\n========================================\n";
    cout << "STATIC MEMBER FUNCTIONS\n";
    cout << "========================================\n";
    cout << "Declaration:\n";
    cout << "  class MyClass {\n";
    cout << "  public:\n";
    cout << "      static void func() {\n";
    cout << "          // Implementation\n";
    cout << "      }\n";
    cout << "  };\n";
    cout << "\n";
    cout << "Call without object:\n";
    cout << "  MyClass::func();  // No object needed!\n";
    cout << "\n";
    cout << "Call with object (also works):\n";
    cout << "  MyClass obj;\n";
    cout << "  obj.func();       // Via object\n";
    cout << "\n";
    cout << "Restrictions:\n";
    cout << "  • No 'this' pointer\n";
    cout << "  • Can only access static members\n";
    cout << "  • Cannot access instance members directly\n";
    cout << "  • Cannot be const (no 'this' to be const)\n";

    cout << "\n========================================\n";
    cout << "INSTANCE vs STATIC\n";
    cout << "========================================\n";
    cout << "┌─────────────────────┬─────────────────────┐\n";
    cout << "│ INSTANCE MEMBERS    │ STATIC MEMBERS      │\n";
    cout << "├─────────────────────┼─────────────────────┤\n";
    cout << "│ Each object has own │ One copy for class  │\n";
    cout << "│ Requires object     │ No object needed    │\n";
    cout << "│ Has 'this' pointer  │ No 'this' pointer   │\n";
    cout << "│ Can access all      │ Only static members │\n";
    cout << "│ Object-level        │ Class-level         │\n";
    cout << "└─────────────────────┴─────────────────────┘\n";

    cout << "\n========================================\n";
    cout << "MEMORY LAYOUT\n";
    cout << "========================================\n";
    cout << "class MyClass {\n";
    cout << "    int instanceVar;      // Each object has own\n";
    cout << "    static int staticVar; // Shared by all\n";
    cout << "};\n";
    cout << "\n";
    cout << "Memory:\n";
    cout << "┌──────────────┐\n";
    cout << "│ obj1         │\n";
    cout << "│ instanceVar  │ ──┐\n";
    cout << "└──────────────┘   │\n";
    cout << "┌──────────────┐   │\n";
    cout << "│ obj2         │   │  Each object has\n";
    cout << "│ instanceVar  │ ──┤  its own copy\n";
    cout << "└──────────────┘   │\n";
    cout << "┌──────────────┐   │\n";
    cout << "│ obj3         │   │\n";
    cout << "│ instanceVar  │ ──┘\n";
    cout << "└──────────────┘\n";
    cout << "\n";
    cout << "┌──────────────┐\n";
    cout << "│ staticVar    │ ◄─── Only ONE copy\n";
    cout << "│ (shared)     │      for ALL objects\n";
    cout << "└──────────────┘\n";

    cout << "\n========================================\n";
    cout << "INITIALIZATION\n";
    cout << "========================================\n";
    cout << "Static data members MUST be initialized outside class:\n";
    cout << "\n";
    cout << "// In header (.h):\n";
    cout << "class MyClass {\n";
    cout << "    static int count;\n";
    cout << "    static double rate;\n";
    cout << "};\n";
    cout << "\n";
    cout << "// In source (.cpp):\n";
    cout << "int MyClass::count = 0;      // Required!\n";
    cout << "double MyClass::rate = 1.5;  // Required!\n";
    cout << "\n";
    cout << "Exception - static const integral:\n";
    cout << "class MyClass {\n";
    cout << "    static const int MAX = 100;  // ✅ OK in class\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "COMMON USE CASES\n";
    cout << "========================================\n";
    cout << "1. Object counting:\n";
    cout << "   static int objectCount;\n";
    cout << "   Constructor: objectCount++;\n";
    cout << "   Destructor: objectCount--;\n";
    cout << "\n";
    cout << "2. Shared configuration:\n";
    cout << "   static string serverAddress;\n";
    cout << "   All objects use same server\n";
    cout << "\n";
    cout << "3. Factory methods:\n";
    cout << "   static MyClass create();\n";
    cout << "   Controlled object creation\n";
    cout << "\n";
    cout << "4. Singleton pattern:\n";
    cout << "   static MyClass* instance;\n";
    cout << "   static MyClass* getInstance();\n";
    cout << "\n";
    cout << "5. Utility functions:\n";
    cout << "   static int max(int a, int b);\n";
    cout << "   No state, pure functionality\n";
    cout << "\n";
    cout << "6. Constants:\n";
    cout << "   static const double PI;\n";
    cout << "   Shared constants\n";

    cout << "\n========================================\n";
    cout << "STATIC MEMBER FUNCTION RESTRICTIONS\n";
    cout << "========================================\n";
    cout << "class MyClass {\n";
    cout << "    int instanceVar;\n";
    cout << "    static int staticVar;\n";
    cout << "    \n";
    cout << "    void instanceFunc() {\n";
    cout << "        instanceVar = 10;  // ✅ OK\n";
    cout << "        staticVar = 20;    // ✅ OK\n";
    cout << "        this->instanceVar; // ✅ Has 'this'\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    static void staticFunc() {\n";
    cout << "        // instanceVar = 10;  // ❌ Error! No 'this'\n";
    cout << "        staticVar = 20;       // ✅ OK\n";
    cout << "        // this->instanceVar; // ❌ No 'this'\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "What static functions CAN do:\n";
    cout << "  ✅ Access static data members\n";
    cout << "  ✅ Call other static functions\n";
    cout << "  ✅ Work with parameters\n";
    cout << "  ✅ Be called without object\n";
    cout << "\n";
    cout << "What static functions CANNOT do:\n";
    cout << "  ❌ Access instance members directly\n";
    cout << "  ❌ Call non-static functions directly\n";
    cout << "  ❌ Use 'this' pointer\n";
    cout << "  ❌ Be const (no 'this' to be const)\n";

    cout << "\n========================================\n";
    cout << "BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "✅ DO:\n";
    cout << "  • Use static for class-wide data\n";
    cout << "  • Initialize static members outside class\n";
    cout << "  • Use static for utility functions\n";
    cout << "  • Use static const for constants\n";
    cout << "  • Document thread-safety for static data\n";
    cout << "  • Use static for factory methods\n";
    cout << "  • Access via ClassName:: for clarity\n";
    cout << "\n";
    cout << "❌ DON'T:\n";
    cout << "  • Forget to initialize static members\n";
    cout << "  • Try to access instance members from static\n";
    cout << "  • Use static when instance member better\n";
    cout << "  • Overuse static (breaks encapsulation)\n";
    cout << "  • Forget thread-safety for mutable static\n";
    cout << "  • Initialize static in constructor\n";

    cout << "\n========================================\n";
    cout << "COMMON PATTERNS\n";
    cout << "========================================\n";
    cout << "1. OBJECT COUNTING:\n";
    cout << "class MyClass {\n";
    cout << "    static int count;\n";
    cout << "public:\n";
    cout << "    MyClass() { count++; }\n";
    cout << "    ~MyClass() { count--; }\n";
    cout << "    static int getCount() { return count; }\n";
    cout << "};\n";
    cout << "int MyClass::count = 0;\n";
    cout << "\n";
    cout << "2. SINGLETON:\n";
    cout << "class Singleton {\n";
    cout << "    static Singleton* instance;\n";
    cout << "    Singleton() { }  // Private\n";
    cout << "public:\n";
    cout << "    static Singleton* getInstance() {\n";
    cout << "        if (!instance) instance = new Singleton();\n";
    cout << "        return instance;\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "Singleton* Singleton::instance = nullptr;\n";
    cout << "\n";
    cout << "3. FACTORY:\n";
    cout << "class Product {\n";
    cout << "    Product() { }  // Private\n";
    cout << "public:\n";
    cout << "    static Product create() {\n";
    cout << "        return Product();\n";
    cout << "    }\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "COMMON MISTAKES\n";
    cout << "========================================\n";
    cout << "❌ Mistake 1: Forgetting to initialize\n";
    cout << "class Bad {\n";
    cout << "    static int count;  // Declared\n";
    cout << "};\n";
    cout << "// Missing: int Bad::count = 0;\n";
    cout << "// Result: Linker error!\n";
    cout << "\n";
    cout << "❌ Mistake 2: Initializing in constructor\n";
    cout << "class Bad {\n";
    cout << "    static int count;\n";
    cout << "    Bad() { count = 0; }  // ❌ Wrong!\n";
    cout << "};\n";
    cout << "// Resets count for EVERY object created!\n";
    cout << "\n";
    cout << "❌ Mistake 3: Accessing instance from static\n";
    cout << "class Bad {\n";
    cout << "    int value;\n";
    cout << "    static void func() {\n";
    cout << "        value = 10;  // ❌ Error! No 'this'\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "❌ Mistake 4: Wrong access method\n";
    cout << "// Declaration\n";
    cout << "static void func();\n";
    cout << "// Definition - DON'T repeat 'static'\n";
    cout << "void MyClass::func() { }  // ✅ Correct\n";
    cout << "// static void MyClass::func() { }  // ❌ Wrong!\n";

    cout << "\n========================================\n";
    cout << "QUICK REFERENCE\n";
    cout << "========================================\n";
    cout << "// DECLARATION\n";
    cout << "class MyClass {\n";
    cout << "    static int count;           // Static data\n";
    cout << "    static void display();      // Static function\n";
    cout << "    static const int MAX = 100; // Static const (integral)\n";
    cout << "};\n";
    cout << "\n";
    cout << "// DEFINITION (outside class)\n";
    cout << "int MyClass::count = 0;\n";
    cout << "void MyClass::display() { }\n";
    cout << "\n";
    cout << "// USAGE\n";
    cout << "MyClass::count = 10;     // Via class\n";
    cout << "MyClass::display();      // Via class\n";
    cout << "obj.count = 10;          // Via object (works too)\n";
    cout << "obj.display();           // Via object (works too)\n";

    cout << "\n========================================\n";
    cout << "KEY TAKEAWAYS\n";
    cout << "========================================\n";
    cout << "1. Static members belong to CLASS, not objects\n";
    cout << "2. Only ONE copy exists (shared by all)\n";
    cout << "3. Static data must be initialized outside class\n";
    cout << "4. Static functions have NO 'this' pointer\n";
    cout << "5. Static functions can only access static members\n";
    cout << "6. Can be accessed without creating object\n";
    cout << "7. Access via ClassName::member\n";
    cout << "8. Used for counting, configuration, factories\n";
    cout << "9. Essential for Singleton pattern\n";
    cout << "10. Perfect for utility classes\n";

    return 0;
}
