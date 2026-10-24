#include <print>
#include <string>
#include <vector>
#include <memory>
#include <cctype>
#include <utility>
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
 * - Must be defined outside the class (or be 'static inline' / constexpr, C++17)
 * - Accessed via ClassName::member or object.member
 *
 * STATIC MEMBER FUNCTIONS:
 * - Can be called without an object
 * - No 'this' pointer
 * - Cannot access instance members directly (only through an object they are given)
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
        std::println("  Counter created. Total objects: {}", objectCount);
    }

    ~Counter() {
        objectCount--;  // Decrement shared counter
        std::println("  Counter destroyed. Remaining objects: {}", objectCount);
    }

    void displayInstance() const {
        std::println("  Instance value: {}", instanceValue);
    }

    static void displayCount() {
        std::println("  Total Counter objects: {}", objectCount);
        // std::println("{}", instanceValue);  // ❌ Error! Can't access instance members
    }

    static int getCount() {
        return objectCount;
    }
};

// CRITICAL: Static members must be initialized outside class
int Counter::objectCount = 0;  // Initialize static member

void demonstrateBasicStatic() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: BASIC STATIC MEMBERS   ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Initial state ---");
    Counter::displayCount();  // Can call without object!

    std::println("");
    std::println("--- Creating objects ---");
    Counter c1(10);
    Counter c2(20);
    Counter c3(30);

    std::println("");
    std::println("--- Checking count ---");
    Counter::displayCount();  // Via class name
    c1.displayCount();        // Via object (also works)

    std::println("");
    std::println("--- Using getCount() ---");
    std::println("  Count via static function: {}", Counter::getCount());

    std::println("");
    std::println("--- Cleanup ---");
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
        std::println("  Student: {}, Grade: {}, School: {}", name, grade, schoolName);
    }

    // Static function - no 'this', can only access static members
    static void displaySchoolInfo() {
        std::println("  School: {}", schoolName);
        std::println("  Total Students: {}", totalStudents);
        // std::println("{}", name);  // ❌ Error! No 'this' pointer
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
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: STATIC vs INSTANCE     ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Setting school name (no objects yet) ---");
    Student::setSchoolName("MIT");
    Student::displaySchoolInfo();

    std::println("");
    std::println("--- Creating students ---");
    Student s1("Alice", 95);
    Student s2("Bob", 87);
    Student s3("Charlie", 92);

    std::println("");
    std::println("--- Display individual students ---");
    s1.displayStudent();
    s2.displayStudent();
    s3.displayStudent();

    std::println("");
    std::println("--- Display school info ---");
    Student::displaySchoolInfo();

    std::println("");
    std::println("--- Cleanup ---");
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
        std::println("  [{}] created. Active: {}", name, activeObjects);
    }

    // Copy constructor
    Tracker(const Tracker& other) : name(other.name + "_Copy") {
        activeObjects++;
        totalCreated++;
        std::println("  [{}] copied. Active: {}", name, activeObjects);
    }

    ~Tracker() {
        activeObjects--;
        totalDestroyed++;
        std::println("  [{}] destroyed. Active: {}", name, activeObjects);
    }

    static void displayStats() {
        std::println("");
        std::println("  === Statistics ===");
        std::println("  Currently active: {}", activeObjects);
        std::println("  Total created:    {}", totalCreated);
        std::println("  Total destroyed:  {}", totalDestroyed);
    }

    static int getActiveCount() { return activeObjects; }
};

// Initialize static members
int Tracker::activeObjects = 0;
int Tracker::totalCreated = 0;
int Tracker::totalDestroyed = 0;

void demonstrateObjectCounting() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: OBJECT COUNTING        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Initial statistics ---");
    Tracker::displayStats();

    std::println("");
    std::println("--- Creating objects in scope ---");
    {
        Tracker t1("First");
        Tracker t2("Second");
        Tracker::displayStats();

        std::println("");
        std::println("--- Creating copy ---");
        Tracker t3 = t1;
        Tracker::displayStats();

        std::println("");
        std::println("--- Exiting scope ---");
    }

    std::println("");
    std::println("--- After scope exit ---");
    Tracker::displayStats();

    std::println("");
    std::println("✅ Static members track objects across their lifetime!");
}

// ============================================
// EXAMPLE 4: SHARED CONFIGURATION
// ============================================

class DatabaseConnection {
private:
    string connectionId;
    bool connected;           // false if the limit was reached (then there is nothing to close)
    static string serverAddress;
    static int serverPort;
    static int maxConnections;
    static int currentConnections;

public:
    DatabaseConnection(const string& id) : connectionId(id), connected(false) {
        if (currentConnections >= maxConnections) {
            std::println("  ❌ Cannot create connection [{}]: limit reached", connectionId);
            return;
        }
        currentConnections++;
        connected = true;
        std::println("  ✅ Connection [{}] established to {}:{}", connectionId, serverAddress, serverPort);
    }

    ~DatabaseConnection() {
        if (!connected) {
            std::println("  Connection [{}] was never established (nothing to close)", connectionId);
            return;
        }
        currentConnections--;
        std::println("  Connection [{}] closed", connectionId);
    }

    // Static configuration methods
    static void configure(const string& addr, int port, int maxConn) {
        serverAddress = addr;
        serverPort = port;
        maxConnections = maxConn;
        std::println("  Database configured: {}:{} (max: {})", addr, port, maxConn);
    }

    static void displayConfig() {
        std::println("  Server: {}:{}", serverAddress, serverPort);
        std::println("  Max connections: {}", maxConnections);
        std::println("  Current connections: {}", currentConnections);
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
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: SHARED CONFIGURATION   ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Default configuration ---");
    DatabaseConnection::displayConfig();

    std::println("");
    std::println("--- Changing configuration (affects ALL connections) ---");
    DatabaseConnection::configure("db.example.com", 5432, 3);
    DatabaseConnection::displayConfig();

    std::println("");
    std::println("--- Creating connections ---");
    DatabaseConnection conn1("App-1");
    DatabaseConnection conn2("App-2");
    DatabaseConnection conn3("App-3");

    std::println("");
    std::println("--- Trying to exceed limit ---");
    DatabaseConnection conn4("App-4");  // Should fail

    std::println("");
    std::println("--- Current status ---");
    DatabaseConnection::displayConfig();

    std::println("");
    std::println("--- Cleanup ---");
}

// ============================================
// EXAMPLE 5: STATIC CONST MEMBERS
// ============================================

class MathConstants {
private:
    // Integral constants can be initialized in the class. Use constexpr: a plain
    // 'static const int' needs an extra definition outside the class as soon as it is
    // bound to a reference (std::println does that), otherwise: linker error.
    // (C++17: static constexpr members are implicitly inline, no outside definition needed.)
    static constexpr int MAX_ITERATIONS = 1000;
    static constexpr int BUFFER_SIZE = 256;

    // Old rule: a non-integral 'static const' must be initialized outside the class
    static const double PI;
    static const double E;

public:
    static void displayConstants() {
        std::println("  MAX_ITERATIONS: {}", MAX_ITERATIONS);
        std::println("  BUFFER_SIZE:    {}", BUFFER_SIZE);
        std::println("  PI:             {:g}", PI);
        std::println("  E:              {:g}", E);
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
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 5: STATIC CONST MEMBERS   ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Math constants ---");
    MathConstants::displayConstants();

    std::println("");
    std::println("--- Using constants ---");
    double radius = 5.0;
    std::println("  Circle area (r={}): {:g}", radius, MathConstants::circleArea(radius));
    std::println("  Sphere volume (r={}): {:g}", radius, MathConstants::sphereVolume(radius));
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
        std::println("  Non-static function:");
        std::println("    Can access instanceVar: {} ✅", instanceVar);
        std::println("    Can access staticVar: {} ✅", staticVar);
        std::println("    Has 'this' pointer ✅");
    }

    // Static function - restricted access
    static void staticFunction() {
        std::println("  Static function:");
        // std::println("{}", instanceVar);  // ❌ Error! No 'this' pointer
        std::println("    Can access staticVar: {} ✅", staticVar);
        std::println("    Cannot access instanceVar ❌");
        std::println("    No 'this' pointer ❌");

        // Can call other static functions
        helperStaticFunction();

        // Cannot call non-static functions directly
        // nonStaticFunction();  // ❌ Error!
    }

    static void helperStaticFunction() {
        std::println("    Helper static function called");
    }

    // Static function can work with objects passed as parameters
    static void processObject(const RestrictionsDemo& obj) {
        std::println("  Static processing object with instanceVar: {}", obj.instanceVar);
    }
};

int RestrictionsDemo::staticVar = 100;

void demonstrateStaticRestrictions() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 6: STATIC RESTRICTIONS    ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    RestrictionsDemo obj(42);

    std::println("--- Non-static function ---");
    obj.nonStaticFunction();

    std::println("");
    std::println("--- Static function ---");
    RestrictionsDemo::staticFunction();

    std::println("");
    std::println("--- Static function with object parameter ---");
    RestrictionsDemo::processObject(obj);
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
        std::println("  Product created: {} (ID: {})", name, id);
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
        std::println("  Product: {} (ID: {})", name, id);
    }

    static int getNextId() {
        return nextId;
    }

    static void resetIdCounter() {
        nextId = 1;
        std::println("  ID counter reset to 1");
    }
};

int Product::nextId = 1;

void demonstrateFactoryPattern() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 7: FACTORY PATTERN        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating products via factory methods ---");
    Product p1 = Product::createBasic();
    Product p2 = Product::createPremium();
    Product p3 = Product::createCustom("Special Edition");

    std::println("");
    std::println("--- Displaying products ---");
    p1.display();
    p2.display();
    p3.display();

    std::println("");
    std::println("--- Next ID ---");
    std::println("  Next product will have ID: {}", Product::getNextId());
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
        std::println("  Logger initialized with file: {}", filename);
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
        std::println("  [LOG to {}]: {}", filename, message);
    }

    static void cleanup() {
        if (instance != nullptr) {
            delete instance;
            instance = nullptr;
            std::println("  Logger cleaned up");
        }
    }
};

Logger* Logger::instance = nullptr;

void demonstrateSingleton() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 8: SINGLETON PATTERN      ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Getting logger instance ---");
    Logger* logger1 = Logger::getInstance();
    logger1->log("First message");

    std::println("");
    std::println("--- Getting logger instance again ---");
    Logger* logger2 = Logger::getInstance();
    logger2->log("Second message");

    std::println("");
    std::println("--- Checking if same instance ---");
    std::println("  logger1 address: {}", static_cast<const void*>(logger1));
    std::println("  logger2 address: {}", static_cast<const void*>(logger2));
    std::println("  Same instance? {}", (logger1 == logger2 ? "Yes ✅" : "No ❌"));

    std::println("");
    std::println("--- Cleanup ---");
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
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 9: UTILITY CLASS          ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    string text = "Hello World";

    std::println("--- String utilities (all static) ---");
    std::println("  Original:     {}", text);
    std::println("  Upper case:   {}", StringUtils::toUpper(text));
    std::println("  Lower case:   {}", StringUtils::toLower(text));
    std::println("  Reversed:     {}", StringUtils::reverse(text));
    std::println("  Word count:   {}", StringUtils::countWords(text));

    std::println("");
    std::println("--- Palindrome check ---");
    string word1 = "radar";
    string word2 = "hello";
    std::println("  \"{}\" is palindrome? {}", word1, (StringUtils::isPalindrome(word1) ? "Yes" : "No"));
    std::println("  \"{}\" is palindrome? {}", word2, (StringUtils::isPalindrome(word2) ? "Yes" : "No"));
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
            std::println("  Recycled ID {} for {}", id, name);
        } else {
            id = nextId++;
            std::println("  Assigned new ID {} to {}", id, name);
        }

        totalEmployees++;
    }

    ~Employee() {
        totalEmployees--;
        availableIds.push_back(id);  // Recycle ID
        std::println("  Employee {} (ID: {}) removed. ID recycled.", name, id);
    }

    void display() const {
        std::println("  [{}] {} - {}", id, name, department);
    }

    static void displayStatistics() {
        std::println("");
        std::println("  === Employee Statistics ===");
        std::println("  Total employees: {}", totalEmployees);
        std::println("  Next new ID: {}", nextId);
        std::println("  Recycled IDs available: {}", availableIds.size());
        if (!availableIds.empty()) {
            std::print("  Recycled IDs: ");
            for (int id : availableIds) {
                std::print("{} ", id);
            }
            std::println("");
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
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 10: ID GENERATOR          ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating employees ---");
    Employee* e1 = new Employee("Alice", "Engineering");
    Employee* e2 = new Employee("Bob", "Marketing");
    Employee* e3 = new Employee("Charlie", "Sales");

    Employee::displayStatistics();

    std::println("");
    std::println("--- Removing some employees ---");
    delete e1;
    delete e3;

    Employee::displayStatistics();

    std::println("");
    std::println("--- Creating new employees (will reuse IDs) ---");
    Employee* e4 = new Employee("David", "HR");
    Employee* e5 = new Employee("Eve", "Finance");

    Employee::displayStatistics();

    std::println("");
    std::println("--- Current employees ---");
    e2->display();
    e4->display();
    e5->display();

    std::println("");
    std::println("--- Cleanup ---");
    delete e2;
    delete e4;
    delete e5;
}

// ============================================
// MAIN DEMONSTRATION
// ============================================

int main() {
    std::println("========================================");
    std::println("STATIC MEMBERS DEMONSTRATION");
    std::println("========================================");

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

    return 0;
}
