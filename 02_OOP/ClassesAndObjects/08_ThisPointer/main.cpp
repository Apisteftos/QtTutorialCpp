#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <cmath>
using namespace std;

// ============================================
// WHAT IS THE 'THIS' POINTER?
// ============================================

/*
 * THE 'THIS' POINTER:
 * - Hidden pointer available in every non-static member function
 * - Points to the object for which the member function is called
 * - Type: ClassName* const (constant pointer to object)
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

        cout << "  Person created at address: " << this << "\n";
    }

    void display() {
        cout << "  Person '" << name << "' (age " << age << ") at " << this << "\n";
    }

    // Get pointer to current object
    Person* getPointer() {
        return this;
    }

    // Get address of current object
    void showAddress() {
        cout << "  Object address (using this): " << this << "\n";
        cout << "  Object address (using &):    " << this << "\n";
    }
};

void demonstrateBasicThis() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: BASIC 'THIS' POINTER  ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating persons ---\n";
    Person p1("Alice", 25);
    Person p2("Bob", 30);

    cout << "\n--- Display with 'this' pointer ---\n";
    p1.display();
    p2.display();

    cout << "\n--- Getting pointers ---\n";
    Person* ptr1 = p1.getPointer();
    cout << "Pointer to p1: " << ptr1 << "\n";
    cout << "Address of p1: " << &p1 << "\n";
    cout << "Same? " << (ptr1 == &p1 ? "Yes ✅" : "No ❌") << "\n";

    cout << "\n--- Show address ---\n";
    p1.showAddress();

    cout << "\n💡 'this' is a pointer to the current object\n";
    cout << "   Type: Person* const (constant pointer)\n";
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

        cout << "  Rectangle: " << this->width << "×" << this->height << "\n";
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
        cout << "  " << width << "×" << height << "\n";
    }
};

void demonstrateNameConflicts() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: NAME CONFLICTS         ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating rectangle ---\n";
    Rectangle r1(10.0, 5.0);

    cout << "\n--- Using setters with 'this' ---\n";
    r1.setWidth(15.0);
    r1.setHeight(8.0);
    r1.display();

    cout << "\n💡 Use 'this->' when parameter names match member names\n";
    cout << "   Without 'this':\n";
    cout << "     width = width;  // Assigns parameter to itself! ❌\n";
    cout << "   With 'this':\n";
    cout << "     this->width = width;  // Assigns to member ✅\n";
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
        cout << "  Builder created\n";
    }

    // Return *this (reference to current object) for chaining
    Builder& setName(const string& name) {
        this->name = name;
        cout << "  Name set to: " << name << "\n";
        return *this;  // Return reference to current object
    }

    Builder& setAge(int age) {
        this->age = age;
        cout << "  Age set to: " << age << "\n";
        return *this;
    }

    Builder& setCity(const string& city) {
        this->city = city;
        cout << "  City set to: " << city << "\n";
        return *this;
    }

    Builder& setOccupation(const string& occupation) {
        this->occupation = occupation;
        cout << "  Occupation set to: " << occupation << "\n";
        return *this;
    }

    void display() const {
        cout << "\n  Profile:\n";
        cout << "    Name: " << name << "\n";
        cout << "    Age: " << age << "\n";
        cout << "    City: " << city << "\n";
        cout << "    Occupation: " << occupation << "\n";
    }
};

void demonstrateMethodChaining() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: METHOD CHAINING        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Building object with method chaining ---\n";
    Builder person;
    person.setName("Alice")
        .setAge(28)
        .setCity("New York")
        .setOccupation("Software Engineer");

    person.display();

    cout << "\n--- Without chaining (traditional way) ---\n";
    Builder person2;
    person2.setName("Bob");
    person2.setAge(35);
    person2.setCity("London");
    person2.setOccupation("Teacher");
    person2.display();

    cout << "\n✅ Method chaining with 'return *this':\n";
    cout << "   • More fluent syntax\n";
    cout << "   • Builder pattern\n";
    cout << "   • jQuery-style API\n";
    cout << "   • Return reference, not copy!\n";
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
        cout << "(" << x << ", " << y << ")";
    }
};

void demonstrateComparingObjects() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: COMPARING OBJECTS      ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Point p1(3, 4);
    Point p2(3, 4);
    Point p3(5, 5);

    cout << "p1 = "; p1.display(); cout << "\n";
    cout << "p2 = "; p2.display(); cout << "\n";
    cout << "p3 = "; p3.display(); cout << "\n\n";

    cout << "p1.equals(p2): " << (p1.equals(p2) ? "true" : "false") << "\n";
    cout << "p1.equals(p3): " << (p1.equals(p3) ? "true" : "false") << "\n\n";

    cout << "p1.isCloserToOrigin(p3): "
         << (p1.isCloserToOrigin(p3) ? "true" : "false") << "\n";

    cout << "Distance from p1 to p3: " << p1.distanceTo(p3) << "\n";

    cout << "\n💡 'this' used to compare current object with parameter\n";
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
        cout << "  Constructed: " << name << "\n";
    }

    // Copy constructor uses 'this'
    Data(const Data& other) : size(other.size), name(other.name + "_Copy") {
        values = new int[size];
        for (int i = 0; i < size; i++) {
            this->values[i] = other.values[i];  // 'this' is new object
        }
        cout << "  Copy constructed: " << this->name << " from " << other.name << "\n";
    }

    // Copy assignment operator - critical use of 'this'
    Data& operator=(const Data& other) {
        cout << "  Copy assignment: " << this->name << " = " << other.name << "\n";

        // CRITICAL: Check for self-assignment
        if (this == &other) {
            cout << "    Self-assignment detected! Skipping.\n";
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
        cout << "  Destructed: " << name << "\n";
        delete[] values;
    }

    void display() const {
        cout << "  " << name << ": ";
        for (int i = 0; i < size; i++) {
            cout << values[i] << " ";
        }
        cout << "\n";
    }
};

void demonstrateCopyAndAssignment() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: COPY & ASSIGNMENT      ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating original ---\n";
    Data d1("Original", 3);
    d1.display();

    cout << "\n--- Copy construction ---\n";
    Data d2 = d1;  // Copy constructor
    d2.display();

    cout << "\n--- Copy assignment ---\n";
    Data d3("Third", 2);
    d3 = d1;  // Copy assignment
    d3.display();

    cout << "\n--- Self-assignment test ---\n";
    d1 = d1;  // Self-assignment - 'this' check prevents disaster!

    cout << "\n💡 'this' in assignment:\n";
    cout << "   • Check self-assignment: if (this == &other)\n";
    cout << "   • Return *this for chaining\n";

    cout << "\n--- Cleanup ---\n";
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
        cout << "  Logged: " << transaction << "\n";
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
        cout << "\n  Account: " << accountNumber << "\n";
        cout << "  Balance: $" << balance << "\n";
        cout << "  Transaction History:\n";
        for (const auto& transaction : transactionLog) {
            cout << "    - " << transaction << "\n";
        }
    }
};

void demonstrateCallingMemberFunctions() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: CALLING MEMBERS        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating account ---\n";
    Account acc("12345", 1000.0);

    cout << "\n--- Transactions ---\n";
    acc.deposit(500.0);
    acc.withdraw(200.0);
    acc.withdraw(2000.0);  // Will fail

    acc.display();

    cout << "\n💡 'this->' can call other member functions\n";
    cout << "   Usually omitted (implicit)\n";
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
        // 'this' has type: ConstDemo* const
        this->value = v;
    }

    // Const member function
    int getValue() const {
        // 'this' has type: const ConstDemo* const
        // Can't modify members through 'this'
        // this->value = 0;  // ❌ Error! Can't modify

        this->accessCount++;  // ✅ OK - mutable member

        return this->value;  // ✅ OK - reading
    }

    // Demonstrate 'this' pointer type
    void showThisType() {
        cout << "  In non-const function:\n";
        cout << "    'this' type: ConstDemo* const\n";
        cout << "    Can modify members\n";
    }

    void showThisTypeConst() const {
        cout << "  In const function:\n";
        cout << "    'this' type: const ConstDemo* const\n";
        cout << "    Cannot modify members (except mutable)\n";
    }
};

void demonstrateConstThis() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 7: 'THIS' WITH CONST      ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    ConstDemo obj(42);

    cout << "--- Non-const function ---\n";
    obj.showThisType();

    cout << "\n--- Const function ---\n";
    obj.showThisTypeConst();

    cout << "\n💡 'this' pointer type changes with const:\n";
    cout << "   Non-const function: ClassName* const\n";
    cout << "   Const function:     const ClassName* const\n";
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
        cout << "  Non-static function:\n";
        cout << "    'this' pointer: " << this << "\n";
        cout << "    Can access: this->instanceVar = " << this->instanceVar << "\n";
        cout << "    Can access: staticVar = " << staticVar << "\n";
    }

    // Static member function - NO 'this'
    static void staticFunction() {
        cout << "  Static function:\n";
        cout << "    NO 'this' pointer!\n";
        // cout << this->instanceVar;  // ❌ Error! No 'this'
        cout << "    Can only access: staticVar = " << staticVar << "\n";
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
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 8: STATIC vs NON-STATIC  ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    StaticDemo obj(42);

    cout << "--- Calling non-static function ---\n";
    obj.nonStaticFunction();

    cout << "\n--- Calling static function ---\n";
    StaticDemo::staticFunction();

    cout << "\n💡 'this' pointer:\n";
    cout << "   Non-static functions: ✅ Have 'this'\n";
    cout << "   Static functions:     ❌ No 'this'\n";
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
        cout << "  Node created with value " << data << " at " << this << "\n";
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
        cout << this->data;
        if (this->next != nullptr) {
            cout << " -> ";
            this->next->display();
        }
    }

    Node* getNext() const { return next; }
    int getData() const { return data; }
};

void demonstrateLinkedList() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 9: LINKED LIST NODE       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating nodes ---\n";
    Node n1(10);
    Node n2(20);
    Node n3(30);

    cout << "\n--- Linking nodes ---\n";
    n1.setNext(&n2).setNext(&n3);  // Won't work as expected, but shows chaining
    n1.setNext(&n2);
    n2.setNext(&n3);

    cout << "\n--- Displaying list ---\n";
    cout << "List: ";
    n1.display();
    cout << "\n";

    cout << "\n--- Node information ---\n";
    cout << "n1.isLast(): " << (n1.isLast() ? "true" : "false") << "\n";
    cout << "n3.isLast(): " << (n3.isLast() ? "true" : "false") << "\n";
    cout << "Contains 20: " << (n1.contains(20) ? "true" : "false") << "\n";
    cout << "Contains 99: " << (n1.contains(99) ? "true" : "false") << "\n";
    cout << "Count: " << n1.count() << " nodes\n";

    cout << "\n✅ 'this' used for:\n";
    cout << "   • Recursive calls (this->next->contains())\n";
    cout << "   • Self-reference\n";
    cout << "   • Method chaining\n";
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
        cout << "\n  Executing HTTP Request:\n";
        cout << "  ==================\n";
        cout << "  Method:  " << method << "\n";
        cout << "  URL:     " << url << "\n";
        cout << "  Timeout: " << timeout << "s\n";
        if (!body.empty()) {
            cout << "  Body:    " << body << "\n";
        }
        if (!headers.empty()) {
            cout << "  Headers:\n";
            for (const auto& header : headers) {
                cout << "    " << header.first << ": " << header.second << "\n";
            }
        }
    }
};

void demonstrateBuilderPattern() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 10: BUILDER PATTERN       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Building HTTP request with fluent API ---\n";

    HttpRequest request;
    request.setUrl("https://api.example.com/users")
        .setMethod("POST")
        .setBody("{\"name\":\"Alice\",\"age\":25}")
        .addHeader("Content-Type", "application/json")
        .addHeader("Authorization", "Bearer token123")
        .setTimeout(60)
        .execute();

    cout << "\n✅ Builder pattern benefits:\n";
    cout << "   • Fluent, readable API\n";
    cout << "   • Optional parameters\n";
    cout << "   • All done via 'return *this'\n";
}

// ============================================
// MAIN DEMONSTRATION
// ============================================

int main() {
    cout << "========================================\n";
    cout << "'THIS' POINTER DEMONSTRATION\n";
    cout << "========================================\n";

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

    // ============================================
    // Comprehensive Summary
    // ============================================
    cout << "\n========================================\n";
    cout << "WHAT IS THE 'THIS' POINTER?\n";
    cout << "========================================\n";
    cout << "'this' is a hidden pointer available in every non-static\n";
    cout << "member function that points to the object for which the\n";
    cout << "function was called.\n";
    cout << "\n";
    cout << "Type: ClassName* const\n";
    cout << "      (constant pointer to object)\n";
    cout << "\n";
    cout << "In const member functions:\n";
    cout << "Type: const ClassName* const\n";
    cout << "      (constant pointer to constant object)\n";

    cout << "\n========================================\n";
    cout << "WHEN TO USE 'THIS'\n";
    cout << "========================================\n";
    cout << "1. Resolve name conflicts:\n";
    cout << "   void setX(int x) {\n";
    cout << "       this->x = x;  // member = parameter\n";
    cout << "   }\n";
    cout << "\n";
    cout << "2. Method chaining:\n";
    cout << "   MyClass& setValue(int v) {\n";
    cout << "       value = v;\n";
    cout << "       return *this;  // Return current object\n";
    cout << "   }\n";
    cout << "\n";
    cout << "3. Self-assignment check:\n";
    cout << "   MyClass& operator=(const MyClass& other) {\n";
    cout << "       if (this == &other) return *this;\n";
    cout << "       // ...\n";
    cout << "   }\n";
    cout << "\n";
    cout << "4. Compare with another object:\n";
    cout << "   bool equals(const MyClass& other) const {\n";
    cout << "       return this->data == other.data;\n";
    cout << "   }\n";
    cout << "\n";
    cout << "5. Get object address:\n";
    cout << "   MyClass* getPointer() {\n";
    cout << "       return this;\n";
    cout << "   }\n";
    cout << "\n";
    cout << "6. Pass to other functions:\n";
    cout << "   void process() {\n";
    cout << "       externalFunction(this);\n";
    cout << "   }\n";

    cout << "\n========================================\n";
    cout << "'THIS' POINTER TYPE\n";
    cout << "========================================\n";
    cout << "class MyClass {\n";
    cout << "    void nonConstFunction() {\n";
    cout << "        // 'this' type: MyClass* const\n";
    cout << "        // Can modify members\n";
    cout << "        this->value = 10;  // ✅\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    void constFunction() const {\n";
    cout << "        // 'this' type: const MyClass* const\n";
    cout << "        // Cannot modify members\n";
    cout << "        // this->value = 10;  // ❌ Error\n";
    cout << "        int x = this->value;   // ✅ Read OK\n";
    cout << "    }\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "USING 'THIS' vs NOT USING 'THIS'\n";
    cout << "========================================\n";
    cout << "class MyClass {\n";
    cout << "    int value;\n";
    cout << "    \n";
    cout << "public:\n";
    cout << "    void setValue(int v) {\n";
    cout << "        // Both are equivalent when no name conflict:\n";
    cout << "        value = v;        // Implicit 'this'\n";
    cout << "        this->value = v;  // Explicit 'this'\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    void setValueConflict(int value) {\n";
    cout << "        // MUST use 'this' to disambiguate:\n";
    cout << "        this->value = value;  // member = parameter\n";
    cout << "        // value = value;     // ❌ Assigns to itself!\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "General rule:\n";
    cout << "  • Use 'this->' when there's name conflict\n";
    cout << "  • Use 'this->' for clarity if desired\n";
    cout << "  • Can omit when no conflict (common practice)\n";

    cout << "\n========================================\n";
    cout << "METHOD CHAINING PATTERN\n";
    cout << "========================================\n";
    cout << "class Builder {\n";
    cout << "    string data;\n";
    cout << "    \n";
    cout << "public:\n";
    cout << "    Builder& setData(const string& d) {\n";
    cout << "        data = d;\n";
    cout << "        return *this;  // Return reference\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    Builder& process() {\n";
    cout << "        // Do something\n";
    cout << "        return *this;\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "Usage:\n";
    cout << "  Builder b;\n";
    cout << "  b.setData(\"test\").process().setData(\"done\");\n";
    cout << "\n";
    cout << "Key points:\n";
    cout << "  • Return *this (not this)\n";
    cout << "  • Return by reference (MyClass&)\n";
    cout << "  • Enables fluent interface\n";

    cout << "\n========================================\n";
    cout << "SELF-ASSIGNMENT CHECK\n";
    cout << "========================================\n";
    cout << "MyClass& operator=(const MyClass& other) {\n";
    cout << "    // CRITICAL: Check for self-assignment\n";
    cout << "    if (this == &other) {\n";
    cout << "        return *this;  // Skip if same object\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    // Delete old resources\n";
    cout << "    delete[] data;\n";
    cout << "    \n";
    cout << "    // Copy new data\n";
    cout << "    data = new int[other.size];\n";
    cout << "    // ...\n";
    cout << "    \n";
    cout << "    return *this;\n";
    cout << "}\n";
    cout << "\n";
    cout << "Why check?\n";
    cout << "  obj = obj;  // Without check:\n";
    cout << "              // 1. Delete obj's data\n";
    cout << "              // 2. Try to copy from deleted data ❌\n";

    cout << "\n========================================\n";
    cout << "STATIC vs NON-STATIC\n";
    cout << "========================================\n";
    cout << "class MyClass {\n";
    cout << "    int instanceVar;\n";
    cout << "    static int staticVar;\n";
    cout << "    \n";
    cout << "public:\n";
    cout << "    // Non-static: HAS 'this'\n";
    cout << "    void nonStatic() {\n";
    cout << "        this->instanceVar = 10;  // ✅\n";
    cout << "        staticVar = 20;          // ✅\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    // Static: NO 'this'\n";
    cout << "    static void staticFunc() {\n";
    cout << "        // this->instanceVar = 10;  // ❌ Error!\n";
    cout << "        staticVar = 20;             // ✅\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "Rule:\n";
    cout << "  • Static functions have NO 'this' pointer\n";
    cout << "  • Can only access static members\n";

    cout << "\n========================================\n";
    cout << "COMMON USE CASES\n";
    cout << "========================================\n";
    cout << "1. Constructor with same parameter names:\n";
    cout << "   MyClass(int x, int y) : x(x), y(y) { }\n";
    cout << "   // Or: this->x = x; this->y = y;\n";
    cout << "\n";
    cout << "2. Builder pattern (fluent interface):\n";
    cout << "   return *this;  // For chaining\n";
    cout << "\n";
    cout << "3. Copy assignment operator:\n";
    cout << "   if (this == &other) return *this;  // Self-check\n";
    cout << "\n";
    cout << "4. Comparison operations:\n";
    cout << "   return this->data == other.data;\n";
    cout << "\n";
    cout << "5. Passing current object:\n";
    cout << "   externalFunction(this);\n";
    cout << "\n";
    cout << "6. Recursive calls:\n";
    cout << "   return this->next->process();\n";

    cout << "\n========================================\n";
    cout << "BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "✅ DO:\n";
    cout << "  • Use 'this->' for name conflicts\n";
    cout << "  • Return *this for method chaining\n";
    cout << "  • Check 'this == &other' in assignment\n";
    cout << "  • Use 'this' for clarity when helpful\n";
    cout << "  • Return reference (MyClass&) for chaining\n";
    cout << "\n";
    cout << "❌ DON'T:\n";
    cout << "  • Use 'this' in static functions (won't compile)\n";
    cout << "  • Return 'this' instead of '*this' for chaining\n";
    cout << "  • Return *this by value for chaining\n";
    cout << "  • Overuse 'this->' when not needed\n";
    cout << "  • Forget self-assignment check in operator=\n";

    cout << "\n========================================\n";
    cout << "COMMON MISTAKES\n";
    cout << "========================================\n";
    cout << "❌ Mistake 1: Returning 'this' instead of '*this'\n";
    cout << "   MyClass setValue(int v) {\n";
    cout << "       value = v;\n";
    cout << "       return this;  // ❌ Wrong type!\n";
    cout << "   }\n";
    cout << "   Fix: return *this;\n";
    cout << "\n";
    cout << "❌ Mistake 2: Returning by value for chaining\n";
    cout << "   MyClass setValue(int v) {  // ❌ Returns copy\n";
    cout << "       value = v;\n";
    cout << "       return *this;\n";
    cout << "   }\n";
    cout << "   Fix: MyClass& setValue(int v)\n";
    cout << "\n";
    cout << "❌ Mistake 3: No self-assignment check\n";
    cout << "   MyClass& operator=(const MyClass& o) {\n";
    cout << "       delete[] data;  // ❌ If this == &o, disaster!\n";
    cout << "       // ...\n";
    cout << "   }\n";
    cout << "   Fix: if (this == &other) return *this;\n";
    cout << "\n";
    cout << "❌ Mistake 4: Using 'this' in static function\n";
    cout << "   static void func() {\n";
    cout << "       this->value = 10;  // ❌ Error! No 'this'\n";
    cout << "   }\n";

    cout << "\n========================================\n";
    cout << "QUICK REFERENCE\n";
    cout << "========================================\n";
    cout << "// Access member\n";
    cout << "this->memberVariable\n";
    cout << "this->memberFunction()\n";
    cout << "\n";
    cout << "// Get object address\n";
    cout << "this  // Pointer to current object\n";
    cout << "\n";
    cout << "// Return current object reference\n";
    cout << "*this  // Reference to current object\n";
    cout << "\n";
    cout << "// Method chaining\n";
    cout << "MyClass& func() {\n";
    cout << "    return *this;\n";
    cout << "}\n";
    cout << "\n";
    cout << "// Self-comparison\n";
    cout << "if (this == &other)\n";
    cout << "\n";
    cout << "// Type in non-const function\n";
    cout << "MyClass* const\n";
    cout << "\n";
    cout << "// Type in const function\n";
    cout << "const MyClass* const\n";

    cout << "\n========================================\n";
    cout << "KEY TAKEAWAYS\n";
    cout << "========================================\n";
    cout << "1. 'this' is a pointer to the current object\n";
    cout << "2. Type: ClassName* const (or const ClassName* const)\n";
    cout << "3. Automatically available in non-static member functions\n";
    cout << "4. Use 'this->' to resolve name conflicts\n";
    cout << "5. Return '*this' for method chaining (by reference!)\n";
    cout << "6. Check 'this == &other' in assignment operator\n";
    cout << "7. Static functions don't have 'this'\n";
    cout << "8. Use '*this' to refer to current object\n";
    cout << "9. Use 'this' to get pointer to current object\n";
    cout << "10. Essential for builder pattern and fluent APIs\n";

    return 0;
}
