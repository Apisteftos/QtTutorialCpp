#include <print>
#include <string>
#include <vector>
#include <cstring>
#include <cmath>
#include <type_traits>
using namespace std;

// ============================================
// WHAT IS THE 'THIS' POINTER?
// ============================================

/*
 * THE 'THIS' POINTER:
 * - Hidden pointer available in every non-static member function
 * - Points to the object for which the member function is called
 * - Type: ClassName* (const ClassName* in const functions). It is a prvalue, so it
 *   cannot be reassigned: it behaves like a ClassName* const
 * - Automatically passed to member functions by compiler
 *
 * COMMON USES:
 * 1. Resolve naming conflicts (parameter vs member)
 * 2. Return reference to current object (for chaining)
 * 3. Compare current object with another
 * 4. Get address of current object
 * 5. Pass current object to other functions
 * 6. Call other member functions explicitly
 *
 * NOTE:
 * - Static member functions don't have 'this'
 * - Friend functions don't have 'this'
 */

// ============================================
// EXAMPLE 1: BASIC 'THIS' POINTER
// ============================================

class Person {
private:
    string name;
    int age;

public:
    Person(string name, int age) {
        // 'this' points to the current object
        this->name = name;  // this->name = member, name = parameter
        this->age = age;    // this->age = member, age = parameter

        std::println("  Person created at address: {}", static_cast<const void*>(this));
    }

    void display() {
        std::println("  Person '{}' (age {}) at {}", name, age, static_cast<const void*>(this));
    }

    // Get pointer to current object
    Person* getPointer() {
        return this;
    }

    // Get address of current object
    void showAddress() {
        std::println("  Object address (using this):   {}", static_cast<const void*>(this));
        std::println("  Object address (using &*this): {}", static_cast<const void*>(&*this));
    }
};

void demonstrateBasicThis() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: BASIC 'THIS' POINTER  ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating persons ---");
    Person p1("Alice", 25);
    Person p2("Bob", 30);

    std::println("");
    std::println("--- Display with 'this' pointer ---");
    p1.display();
    p2.display();

    std::println("");
    std::println("--- Getting pointers ---");
    Person* ptr1 = p1.getPointer();
    std::println("Pointer to p1: {}", static_cast<const void*>(ptr1));
    std::println("Address of p1: {}", static_cast<const void*>(&p1));
    std::println("Same? {}", (ptr1 == &p1 ? "Yes ✅" : "No ❌"));

    std::println("");
    std::println("--- Show address ---");
    p1.showAddress();
}

// ============================================
// EXAMPLE 2: RESOLVING NAME CONFLICTS
// ============================================

class Rectangle {
private:
    double width;
    double height;

public:
    // Constructor with same parameter names as members
    Rectangle(double width, double height) {
        // Without 'this', this would assign parameter to itself!
        this->width = width;    // this->width is the member
        this->height = height;  // width is the parameter

        std::println("  Rectangle: {:g}×{:g}", this->width, this->height);
    }

    // Setter with same parameter name
    void setWidth(double width) {
        // 'this->' disambiguates
        this->width = width;
    }

    void setHeight(double height) {
        this->height = height;
    }

    // For comparison - no name conflict, 'this' optional
    void setDimensions(double w, double h) {
        width = w;   // No conflict, can use directly
        height = h;  // Or: this->width = w; (equivalent)
    }

    void display() const {
        std::println("  {:g}×{:g}", width, height);
    }
};

void demonstrateNameConflicts() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: NAME CONFLICTS         ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating rectangle ---");
    Rectangle r1(10.0, 5.0);

    std::println("");
    std::println("--- Using setters with 'this' ---");
    r1.setWidth(15.0);
    r1.setHeight(8.0);
    r1.display();
}

// ============================================
// EXAMPLE 3: METHOD CHAINING (FLUENT INTERFACE)
// ============================================

class Builder {
private:
    string name;
    int age;
    string city;
    string occupation;

public:
    Builder() : age(0) {
        std::println("  Builder created");
    }

    // Return *this (reference to current object) for chaining
    Builder& setName(const string& name) {
        this->name = name;
        std::println("  Name set to: {}", name);
        return *this;  // Return reference to current object
    }

    Builder& setAge(int age) {
        this->age = age;
        std::println("  Age set to: {}", age);
        return *this;
    }

    Builder& setCity(const string& city) {
        this->city = city;
        std::println("  City set to: {}", city);
        return *this;
    }

    Builder& setOccupation(const string& occupation) {
        this->occupation = occupation;
        std::println("  Occupation set to: {}", occupation);
        return *this;
    }

    void display() const {
        std::println("");
        std::println("  Profile:");
        std::println("    Name: {}", name);
        std::println("    Age: {}", age);
        std::println("    City: {}", city);
        std::println("    Occupation: {}", occupation);
    }
};

void demonstrateMethodChaining() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: METHOD CHAINING        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Building object with method chaining ---");
    Builder person;
    person.setName("Alice")
        .setAge(28)
        .setCity("New York")
        .setOccupation("Software Engineer");

    person.display();

    std::println("");
    std::println("--- Without chaining (traditional way) ---");
    Builder person2;
    person2.setName("Bob");
    person2.setAge(35);
    person2.setCity("London");
    person2.setOccupation("Teacher");
    person2.display();
}

// ============================================
// EXAMPLE 4: COMPARING OBJECTS
// ============================================

class Point {
private:
    int x, y;

public:
    Point(int x, int y) : x(x), y(y) { }

    // Compare current object with another
    bool equals(const Point& other) const {
        // Compare 'this' object with 'other' object
        return this->x == other.x && this->y == other.y;
    }

    // Check if this point is closer to origin than other
    bool isCloserToOrigin(const Point& other) const {
        int thisDist = this->x * this->x + this->y * this->y;
        int otherDist = other.x * other.x + other.y * other.y;
        return thisDist < otherDist;
    }

    // Get distance from this point to another
    double distanceTo(const Point& other) const {
        int dx = this->x - other.x;
        int dy = this->y - other.y;
        return sqrt(dx * dx + dy * dy);
    }

    void display() const {
        std::print("({}, {})", x, y);
    }
};

void demonstrateComparingObjects() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: COMPARING OBJECTS      ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Point p1(3, 4);
    Point p2(3, 4);
    Point p3(5, 5);

    std::print("p1 = ");
    p1.display();
    std::println("");
    std::print("p2 = ");
    p2.display();
    std::println("");
    std::print("p3 = ");
    p3.display();
    std::println("");
    std::println("");

    std::println("p1.equals(p2): {}", (p1.equals(p2) ? "true" : "false"));
    std::println("p1.equals(p3): {}", (p1.equals(p3) ? "true" : "false"));
    std::println("");

    std::println("p1.isCloserToOrigin(p3): {}", (p1.isCloserToOrigin(p3) ? "true" : "false"));

    std::println("Distance from p1 to p3: {:g}", p1.distanceTo(p3));
}

// ============================================
// EXAMPLE 5: COPY CONSTRUCTOR AND ASSIGNMENT
// ============================================

class Data {
private:
    int* values;
    int size;
    string name;

public:
    Data(const string& name, int size) : size(size), name(name) {
        values = new int[size];
        for (int i = 0; i < size; i++) {
            values[i] = i * 10;
        }
        std::println("  Constructed: {}", name);
    }

    // Copy constructor uses 'this'
    Data(const Data& other) : size(other.size), name(other.name + "_Copy") {
        values = new int[size];
        for (int i = 0; i < size; i++) {
            this->values[i] = other.values[i];  // 'this' is new object
        }
        std::println("  Copy constructed: {} from {}", this->name, other.name);
    }

    // Copy assignment operator - critical use of 'this'
    Data& operator=(const Data& other) {
        std::println("  Copy assignment: {} = {}", this->name, other.name);

        // CRITICAL: Check for self-assignment
        if (this == &other) {
            std::println("    Self-assignment detected! Skipping.");
            return *this;
        }

        // Delete old data
        delete[] this->values;

        // Copy new data
        this->size = other.size;
        this->name = other.name + "_Assigned";
        this->values = new int[size];
        for (int i = 0; i < size; i++) {
            this->values[i] = other.values[i];
        }

        return *this;  // Return reference to current object
    }

    ~Data() {
        std::println("  Destructed: {}", name);
        delete[] values;
    }

    void display() const {
        std::print("  {}: ", name);
        for (int i = 0; i < size; i++) {
            std::print("{} ", values[i]);
        }
        std::println("");
    }
};

void demonstrateCopyAndAssignment() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 5: COPY & ASSIGNMENT      ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating original ---");
    Data d1("Original", 3);
    d1.display();

    std::println("");
    std::println("--- Copy construction ---");
    Data d2 = d1;  // Copy constructor
    d2.display();

    std::println("");
    std::println("--- Copy assignment ---");
    Data d3("Third", 2);
    d3 = d1;  // Copy assignment
    d3.display();

    std::println("");
    std::println("--- Self-assignment test ---");
    d1 = d1;  // Self-assignment - 'this' check prevents disaster!
    std::println("");
    std::println("--- Cleanup ---");
}

// ============================================
// EXAMPLE 6: CALLING OTHER MEMBER FUNCTIONS
// ============================================

class Account {
private:
    string accountNumber;
    double balance;
    vector<string> transactionLog;

    // Private helper function
    void logTransaction(const string& transaction) {
        transactionLog.push_back(transaction);
        std::println("  Logged: {}", transaction);
    }

public:
    Account(string accNum, double initialBalance)
        : accountNumber(accNum), balance(initialBalance) {
        // 'this' can call other member functions
        this->logTransaction("Account opened with $" + to_string(initialBalance));
    }

    void deposit(double amount) {
        balance += amount;
        // Can use 'this->' explicitly or omit it
        this->logTransaction("Deposit: $" + to_string(amount));
        // logTransaction("..."); // Same effect
    }

    void withdraw(double amount) {
        if (amount <= balance) {
            balance -= amount;
            this->logTransaction("Withdrawal: $" + to_string(amount));
        } else {
            this->logTransaction("Withdrawal failed: insufficient funds");
        }
    }

    void display() const {
        std::println("");
        std::println("  Account: {}", accountNumber);
        std::println("  Balance: ${:.2f}", balance);
        std::println("  Transaction History:");
        for (const auto& transaction : transactionLog) {
            std::println("    - {}", transaction);
        }
    }
};

void demonstrateCallingMemberFunctions() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 6: CALLING MEMBERS        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating account ---");
    Account acc("12345", 1000.0);

    std::println("");
    std::println("--- Transactions ---");
    acc.deposit(500.0);
    acc.withdraw(200.0);
    acc.withdraw(2000.0);  // Will fail

    acc.display();
}

// ============================================
// EXAMPLE 7: 'THIS' WITH CONST MEMBERS
// ============================================

class ConstDemo {
private:
    int value;
    mutable int accessCount;  // Mutable allows modification in const functions

public:
    ConstDemo(int v) : value(v), accessCount(0) { }

    // Non-const member function
    void setValue(int v) {
        // 'this' has type: ConstDemo*  (checked below at compile time)
        static_assert(is_same_v<decltype(this), ConstDemo*>);
        this->value = v;
    }

    // Const member function
    int getValue() const {
        // 'this' has type: const ConstDemo*  (checked below at compile time)
        static_assert(is_same_v<decltype(this), const ConstDemo*>);
        // Can't modify members through 'this'
        // this->value = 0;  // ❌ Error! Can't modify

        this->accessCount++;  // ✅ OK - mutable member

        return this->value;  // ✅ OK - reading
    }

    // Demonstrate 'this' pointer type
    void showThisType() {
        static_assert(is_same_v<decltype(this), ConstDemo*>);        // compile-time check
        std::println("  In non-const function:");
        std::println("    'this' type: ConstDemo*  (static_assert passed)");
        std::println("    Can modify members");
    }

    void showThisTypeConst() const {
        static_assert(is_same_v<decltype(this), const ConstDemo*>);  // compile-time check
        std::println("  In const function:");
        std::println("    'this' type: const ConstDemo*  (static_assert passed)");
        std::println("    Cannot modify members (except mutable)");
    }

    int getAccessCount() const { return accessCount; }
};

void demonstrateConstThis() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 7: 'THIS' WITH CONST      ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    ConstDemo obj(42);

    std::println("--- Non-const function ---");
    obj.showThisType();

    std::println("");
    std::println("--- Const function ---");
    obj.showThisTypeConst();

    std::println("");
    std::println("--- mutable member changed inside a const function ---");
    std::println("getValue() = {}", obj.getValue());
    std::println("getValue() = {}", obj.getValue());
    std::println("accessCount = {}  (getValue() is const but counts its calls)", obj.getAccessCount());
}

// ============================================
// EXAMPLE 8: 'THIS' IN STATIC VS NON-STATIC
// ============================================

class StaticDemo {
private:
    int instanceVar;
    static int staticVar;

public:
    StaticDemo(int v) : instanceVar(v) { }

    // Non-static member function - HAS 'this'
    void nonStaticFunction() {
        std::println("  Non-static function:");
        std::println("    'this' pointer: {}", static_cast<const void*>(this));
        std::println("    Can access: this->instanceVar = {}", this->instanceVar);
        std::println("    Can access: staticVar = {}", staticVar);
    }

    // Static member function - NO 'this'
    static void staticFunction() {
        std::println("  Static function:");
        std::println("    NO 'this' pointer!");
        // std::println("{}", this->instanceVar);  // ❌ Error! No 'this'
        std::println("    Can only access: staticVar = {}", staticVar);
    }

    int getInstanceVar() const {
        return this->instanceVar;  // ✅ Has 'this'
    }

    static int getStaticVar() {
        // return this->instanceVar;  // ❌ Error! No 'this'
        return staticVar;  // ✅ Can access static
    }
};

int StaticDemo::staticVar = 100;

void demonstrateStaticVsNonStatic() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 8: STATIC vs NON-STATIC  ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    StaticDemo obj(42);

    std::println("--- Calling non-static function ---");
    obj.nonStaticFunction();

    std::println("");
    std::println("--- Calling static function ---");
    StaticDemo::staticFunction();
}

// ============================================
// EXAMPLE 9: REAL-WORLD - LINKED LIST NODE
// ============================================

class Node {
private:
    int data;
    Node* next;

public:
    Node(int value) : data(value), next(nullptr) {
        std::println("  Node created with value {} at {}", data, static_cast<const void*>(this));
    }

    // Set next node - returns reference for chaining
    Node& setNext(Node* node) {
        this->next = node;
        return *this;
    }

    // Check if this node is the last
    bool isLast() const {
        return this->next == nullptr;
    }

    // Find if this node contains a value
    bool contains(int value) const {
        if (this->data == value) {
            return true;
        }
        if (this->next != nullptr) {
            return this->next->contains(value);  // Recursive
        }
        return false;
    }

    // Count nodes starting from this node
    int count() const {
        if (this->next == nullptr) {
            return 1;
        }
        return 1 + this->next->count();
    }

    // Display from this node onwards
    void display() const {
        std::print("{}", this->data);
        if (this->next != nullptr) {
            std::print(" -> ");
            this->next->display();
        }
    }

    Node* getNext() const { return next; }
    int getData() const { return data; }
};

void demonstrateLinkedList() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 9: LINKED LIST NODE       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating nodes ---");
    Node n1(10);
    Node n2(20);
    Node n3(30);

    std::println("");
    std::println("--- Linking nodes ---");
    n1.setNext(&n2).setNext(&n3);  // Chaining: both calls act on n1, so n1.next ends up as n3 (not what we want)
    n1.setNext(&n2);               // relink correctly: n1 -> n2 -> n3
    n2.setNext(&n3);

    std::println("");
    std::println("--- Displaying list ---");
    std::print("List: ");
    n1.display();
    std::println("");

    std::println("");
    std::println("--- Node information ---");
    std::println("n1.isLast(): {}", (n1.isLast() ? "true" : "false"));
    std::println("n3.isLast(): {}", (n3.isLast() ? "true" : "false"));
    std::println("Contains 20: {}", (n1.contains(20) ? "true" : "false"));
    std::println("Contains 99: {}", (n1.contains(99) ? "true" : "false"));
    std::println("Count: {} nodes", n1.count());
}

// ============================================
// EXAMPLE 10: REAL-WORLD - BUILDER PATTERN
// ============================================

class HttpRequest {
private:
    string url;
    string method;
    string body;
    vector<pair<string, string>> headers;
    int timeout;

public:
    HttpRequest() : method("GET"), timeout(30) { }

    // Builder methods - all return *this for chaining
    HttpRequest& setUrl(const string& url) {
        this->url = url;
        return *this;
    }

    HttpRequest& setMethod(const string& method) {
        this->method = method;
        return *this;
    }

    HttpRequest& setBody(const string& body) {
        this->body = body;
        return *this;
    }

    HttpRequest& addHeader(const string& key, const string& value) {
        this->headers.push_back({key, value});
        return *this;
    }

    HttpRequest& setTimeout(int seconds) {
        this->timeout = seconds;
        return *this;
    }

    // Execute request (simulated)
    void execute() const {
        std::println("");
        std::println("  Executing HTTP Request:");
        std::println("  ==================");
        std::println("  Method:  {}", method);
        std::println("  URL:     {}", url);
        std::println("  Timeout: {}s", timeout);
        if (!body.empty()) {
            std::println("  Body:    {}", body);
        }
        if (!headers.empty()) {
            std::println("  Headers:");
            for (const auto& header : headers) {
                std::println("    {}: {}", header.first, header.second);
            }
        }
    }
};

void demonstrateBuilderPattern() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 10: BUILDER PATTERN       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Building HTTP request with fluent API ---");

    HttpRequest request;
    request.setUrl("https://api.example.com/users")
        .setMethod("POST")
        .setBody("{\"name\":\"Alice\",\"age\":25}")
        .addHeader("Content-Type", "application/json")
        .addHeader("Authorization", "Bearer token123")
        .setTimeout(60)
        .execute();
}

// ============================================
// MAIN DEMONSTRATION
// ============================================

int main() {
    std::println("========================================");
    std::println("'THIS' POINTER DEMONSTRATION");
    std::println("========================================");

    demonstrateBasicThis();
    demonstrateNameConflicts();
    demonstrateMethodChaining();
    demonstrateComparingObjects();
    demonstrateCopyAndAssignment();
    demonstrateCallingMemberFunctions();
    demonstrateConstThis();
    demonstrateStaticVsNonStatic();
    demonstrateLinkedList();
    demonstrateBuilderPattern();

    return 0;
}
