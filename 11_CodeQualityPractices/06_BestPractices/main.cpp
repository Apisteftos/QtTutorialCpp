#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <mutex>
#include <cstring>
using namespace std;

// ============================================
// C++ BEST PRACTICES
// ============================================

/*
C++ BEST PRACTICES:
    "Follow proven patterns and idioms that make
     C++ code safer, more efficient, and maintainable."
    — Modern C++ Guidelines

WHAT IT MEANS:
    • Use language features correctly
    • Prevent common bugs and pitfalls
    • Write safe, efficient code
    • Follow C++ idioms and patterns
    • Leverage compiler and type system

WHY IT MATTERS:
    ✅ Prevents memory leaks and crashes
    ✅ Eliminates resource leaks
    ✅ Reduces undefined behavior
    ✅ Makes code exception-safe
    ✅ Improves performance
    ✅ Easier to maintain and debug

KEY BEST PRACTICES COVERED:
    1. RAII - Resource Acquisition Is Initialization
    2. Rule of Zero/Three/Five
    3. Const Correctness
    4. Initialize All Variables
    5. Prefer References Over Pointers
    6. Use Smart Pointers
    7. Avoid Raw new/delete
    8. Pass by Const Reference

VIOLATION SIGNS:
    🚨 Memory leaks
    🚨 Double free errors
    🚨 Dangling pointers
    🚨 Resource leaks (files, sockets, locks)
    🚨 Uninitialized variables
    🚨 Null pointer dereferences
    🚨 Manual memory management
*/

// ============================================
// EXAMPLE 1: RAII (Resource Acquisition Is Initialization)
// ============================================

namespace bad_example1 {
// ❌ BAD: Manual resource management (error-prone)

void processFile() {
    FILE* file = fopen("data.txt", "r");
    if (!file) {
        cout << "   ❌ Can't open file" << endl;
        return;  // 🚨 No cleanup needed here, but...
    }

    // Do some processing
    char buffer[100];
    if (!fgets(buffer, 100, file)) {
        cout << "   ❌ Read error" << endl;
        // 🚨 FORGOT TO CLOSE FILE! Memory leak!
        return;
    }

    // More processing that might throw
    try {
        // What if this throws?
        if (strlen(buffer) == 0) {
            throw runtime_error("Empty file");
        }
    } catch (...) {
        // 🚨 FORGOT TO CLOSE FILE! Memory leak!
        throw;
    }

    fclose(file);  // Only closed in happy path!
}

class DatabaseConnection {
private:
    bool connected;
public:
    DatabaseConnection() : connected(false) {}

    void connect() {
        connected = true;
        cout << "   📡 Connected to database" << endl;
    }

    void disconnect() {
        if (connected) {
            cout << "   🔌 Disconnected from database" << endl;
            connected = false;
        }
    }

    // 🚨 No automatic cleanup!
};

void queryDatabase() {
    DatabaseConnection db;
    db.connect();

    // Do query
    // 🚨 What if exception occurs?
    // 🚨 User must remember to call disconnect()!

    db.disconnect();  // Easy to forget!
}
}

namespace good_example1 {
// ✅ GOOD: RAII - Automatic resource management

class FileHandle {
private:
    FILE* file;

public:
    FileHandle(const string& filename, const char* mode) {
        file = fopen(filename.c_str(), mode);
        if (!file) {
            throw runtime_error("Can't open file: " + filename);
        }
        cout << "   📂 Opened file: " << filename << endl;
    }

    ~FileHandle() {
        if (file) {
            fclose(file);
            cout << "   📂 Closed file automatically" << endl;
        }
    }

    // Delete copy operations
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;

    FILE* get() { return file; }
};

void processFile() {
    try {
        FileHandle file("data.txt", "r");  // RAII: Acquires resource

        char buffer[100];
        if (!fgets(buffer, 100, file.get())) {
            return;  // ✅ File automatically closed!
        }

        if (strlen(buffer) == 0) {
            throw runtime_error("Empty");  // ✅ File automatically closed!
        }

        // More processing...

    } // ✅ Destructor called here, file ALWAYS closed!
    catch (const exception& e) {
        cout << "   ❌ Error: " << e.what() << endl;
    }
}

class DatabaseConnection {
private:
    bool connected;

public:
    DatabaseConnection() : connected(false) {
        connected = true;
        cout << "   📡 Connected to database" << endl;
    }

    ~DatabaseConnection() {
        if (connected) {
            cout << "   🔌 Disconnected automatically" << endl;
            connected = false;
        }
    }

    // Delete copy operations
    DatabaseConnection(const DatabaseConnection&) = delete;
    DatabaseConnection& operator=(const DatabaseConnection&) = delete;

    void query() {
        cout << "   🔍 Executing query..." << endl;
    }
};

void queryDatabase() {
    DatabaseConnection db;  // RAII: Constructor connects
    db.query();

    // Exception or early return?
    // No problem! Destructor ALWAYS called!

}  // ✅ Destructor automatically disconnects
}

void example1_raii() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 1: RAII                                ║\n";
    cout << "║   (Resource Acquisition Is Initialization)       ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (Manual Resource Management):\n";
    cout << "   FILE* file = fopen(\"data.txt\", \"r\");\n";
    cout << "   if (!file) return;\n";
    cout << "   \n";
    cout << "   // Do work...\n";
    cout << "   if (error) {\n";
    cout << "       return;  // 🚨 LEAK! Forgot to close file\n";
    cout << "   }\n";
    cout << "   \n";
    cout << "   fclose(file);  // Only in happy path!\n";

    cout << "\n✅ GOOD CODE (RAII):\n";
    cout << "   class FileHandle {\n";
    cout << "       FILE* file;\n";
    cout << "   public:\n";
    cout << "       FileHandle(const string& name) {\n";
    cout << "           file = fopen(name.c_str(), \"r\");\n";
    cout << "       }\n";
    cout << "       ~FileHandle() {\n";
    cout << "           if (file) fclose(file);  // ALWAYS called!\n";
    cout << "       }\n";
    cout << "   };\n";

    cout << "\n✨ DEMONSTRATION:\n";
    cout << "\n   Database connection example:\n";
    good_example1::queryDatabase();

    cout << "\n💡 RAII BENEFITS:\n";
    cout << "   ✅ Automatic cleanup - no leaks\n";
    cout << "   ✅ Exception-safe - destructor always runs\n";
    cout << "   ✅ No manual cleanup code needed\n";
    cout << "   ✅ Impossible to forget to release\n";
    cout << "   ✅ Scope-based lifetime management\n";

    cout << "\n🎯 RAII PATTERN:\n";
    cout << "   • Constructor acquires resource\n";
    cout << "   • Destructor releases resource\n";
    cout << "   • Lifetime tied to object scope\n";
    cout << "   • Works with stack unwinding\n";
}

// ============================================
// EXAMPLE 2: RULE OF ZERO/THREE/FIVE
// ============================================

namespace bad_example2 {
// ❌ BAD: Partial implementation (dangerous!)

class Buffer {
private:
    int* data;
    size_t size;

public:
    Buffer(size_t s) : size(s) {
        data = new int[size];
        cout << "   📦 Allocated buffer of size " << size << endl;
    }

    ~Buffer() {
        delete[] data;
        cout << "   🗑️  Deleted buffer" << endl;
    }

    // 🚨 NO COPY CONSTRUCTOR!
    // 🚨 NO COPY ASSIGNMENT!
    // This is the "Rule of Three" violation!
};

void demonstrateProblem() {
    Buffer buf1(10);
    Buffer buf2 = buf1;  // 🚨 Shallow copy! Both point to same data!
    // 🚨 When both destructors run → DOUBLE DELETE → CRASH!
}
}

namespace good_example2 {
// ✅ GOOD: Rule of Zero - Let compiler handle it

class BufferRuleOfZero {
private:
    vector<int> data;  // ✅ Standard container handles everything

public:
    BufferRuleOfZero(size_t size) : data(size) {
        cout << "   📦 Created buffer (Rule of Zero)" << endl;
    }

    // ✅ Compiler-generated destructor is perfect!
    // ✅ Compiler-generated copy is perfect!
    // ✅ Compiler-generated move is perfect!
};

// ✅ GOOD: Rule of Five - Full implementation

class BufferRuleOfFive {
private:
    int* data;
    size_t size;

public:
    // Constructor
    BufferRuleOfFive(size_t s) : size(s) {
        data = new int[size];
        cout << "   📦 Allocated " << size << " ints" << endl;
    }

    // Destructor
    ~BufferRuleOfFive() {
        delete[] data;
        cout << "   🗑️  Freed memory" << endl;
    }

    // Copy Constructor
    BufferRuleOfFive(const BufferRuleOfFive& other) : size(other.size) {
        data = new int[size];
        copy(other.data, other.data + size, data);
        cout << "   📋 Copy constructed" << endl;
    }

    // Copy Assignment
    BufferRuleOfFive& operator=(const BufferRuleOfFive& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = new int[size];
            copy(other.data, other.data + size, data);
            cout << "   📋 Copy assigned" << endl;
        }
        return *this;
    }

    // Move Constructor
    BufferRuleOfFive(BufferRuleOfFive&& other) noexcept
        : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
        cout << "   🚚 Move constructed" << endl;
    }

    // Move Assignment
    BufferRuleOfFive& operator=(BufferRuleOfFive&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
            cout << "   🚚 Move assigned" << endl;
        }
        return *this;
    }
};
}

void example2_rule_of_five() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 2: RULE OF ZERO/THREE/FIVE            ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n📜 THE RULES:\n";
    cout << "\n   RULE OF ZERO:\n";
    cout << "   • Use standard containers (vector, string, etc.)\n";
    cout << "   • Let compiler generate all special members\n";
    cout << "   • Preferred approach!\n";

    cout << "\n   RULE OF THREE (C++98):\n";
    cout << "   If you define ONE of:\n";
    cout << "   • Destructor\n";
    cout << "   • Copy constructor\n";
    cout << "   • Copy assignment\n";
    cout << "   You must define ALL THREE!\n";

    cout << "\n   RULE OF FIVE (C++11+):\n";
    cout << "   If you define ONE of:\n";
    cout << "   • Destructor\n";
    cout << "   • Copy constructor\n";
    cout << "   • Copy assignment\n";
    cout << "   • Move constructor\n";
    cout << "   • Move assignment\n";
    cout << "   You must define ALL FIVE!\n";

    cout << "\n❌ BAD CODE (Violates Rule of Three):\n";
    cout << "   class Buffer {\n";
    cout << "       int* data;\n";
    cout << "   public:\n";
    cout << "       ~Buffer() { delete[] data; }\n";
    cout << "       // 🚨 Missing copy constructor!\n";
    cout << "       // 🚨 Missing copy assignment!\n";
    cout << "       // Result: Double delete → CRASH!\n";
    cout << "   };\n";

    cout << "\n✅ GOOD CODE (Rule of Zero):\n";
    cout << "   class Buffer {\n";
    cout << "       vector<int> data;  // ✅ Perfect!\n";
    cout << "   public:\n";
    cout << "       Buffer(size_t size) : data(size) {}\n";
    cout << "       // Compiler generates everything else!\n";
    cout << "   };\n";

    cout << "\n✨ DEMONSTRATION (Rule of Five):\n";
    {
        good_example2::BufferRuleOfFive buf1(5);
        good_example2::BufferRuleOfFive buf2 = buf1;  // Copy constructor
        good_example2::BufferRuleOfFive buf3(10);
        buf3 = buf1;  // Copy assignment
        good_example2::BufferRuleOfFive buf4 = std::move(buf1);  // Move constructor
    }

    cout << "\n💡 RECOMMENDATION:\n";
    cout << "   🥇 First choice: Rule of Zero (use std containers)\n";
    cout << "   🥈 Only if needed: Rule of Five (full implementation)\n";
    cout << "   ⚠️  Never: Partial implementation!\n";
}

// ============================================
// EXAMPLE 3: CONST CORRECTNESS
// ============================================

namespace bad_example3 {
// ❌ BAD: No const correctness

class Person {
private:
    string name;
    int age;

public:
    Person(string n, int a) : name(n), age(a) {}

    // 🚨 Should be const but isn't!
    string getName() { return name; }
    int getAge() { return age; }

    // 🚨 Can modify when it shouldn't!
    void print() {
        cout << name << ", " << age << endl;
    }
};

// 🚨 Can't pass const Person to these functions!
void printPerson(Person& p) {  // Should be const&
    p.print();
}
}

namespace good_example3 {
// ✅ GOOD: Proper const correctness

class Person {
private:
    string name;
    int age;

public:
    Person(const string& n, int a) : name(n), age(a) {}

    // ✅ Const member functions
    string getName() const { return name; }
    int getAge() const { return age; }

    void print() const {
        cout << "   " << name << ", age " << age << endl;
    }

    // ✅ Non-const for modification
    void setName(const string& n) { name = n; }
    void setAge(int a) { age = a; }
};

// ✅ Const parameters when not modifying
void printPerson(const Person& p) {
    p.print();
}

// ✅ Const reference return for members
class Team {
private:
    vector<Person> members;

public:
    void addMember(const Person& p) {
        members.push_back(p);
    }

    // ✅ Const version
    const vector<Person>& getMembers() const {
        return members;
    }

    // ✅ Non-const version for modification
    vector<Person>& getMembers() {
        return members;
    }
};
}

void example3_const_correctness() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 3: CONST CORRECTNESS                   ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (No Const):\n";
    cout << "   class Person {\n";
    cout << "   public:\n";
    cout << "       string getName() { return name; }  // 🚨 Not const!\n";
    cout << "       void print() { /* ... */ }         // 🚨 Not const!\n";
    cout << "   };\n";
    cout << "   \n";
    cout << "   void func(Person& p) {  // 🚨 Can't accept const!\n";
    cout << "       p.print();\n";
    cout << "   }\n";

    cout << "\n✅ GOOD CODE (Const Correct):\n";
    cout << "   class Person {\n";
    cout << "   public:\n";
    cout << "       string getName() const { return name; }  // ✅\n";
    cout << "       void print() const { /* ... */ }         // ✅\n";
    cout << "       void setName(const string& n) { }        // ✅\n";
    cout << "   };\n";
    cout << "   \n";
    cout << "   void func(const Person& p) {  // ✅ Can accept const\n";
    cout << "       p.print();  // Works!\n";
    cout << "   }\n";

    cout << "\n✨ DEMONSTRATION:\n";

    good_example3::Person john("John Doe", 30);
    cout << "\n   Created person:\n";
    john.print();

    const good_example3::Person jane("Jane Smith", 28);
    cout << "\n   Const person can still be read:\n";
    jane.print();
    // jane.setAge(29);  // ❌ Won't compile - const!

    cout << "\n💡 CONST CORRECTNESS RULES:\n";
    cout << "   ✅ Mark member functions const if they don't modify\n";
    cout << "   ✅ Pass by const reference when not modifying\n";
    cout << "   ✅ Use const for variables that don't change\n";
    cout << "   ✅ Provide both const and non-const overloads\n";

    cout << "\n🎯 BENEFITS:\n";
    cout << "   • Compiler enforces immutability\n";
    cout << "   • Self-documenting intent\n";
    cout << "   • Enables optimizations\n";
    cout << "   • Prevents accidental modifications\n";
}

// ============================================
// EXAMPLE 4: INITIALIZE ALL VARIABLES
// ============================================

namespace bad_example4 {
// ❌ BAD: Uninitialized variables

void processData() {
    int count;  // 🚨 Uninitialized! Contains garbage!
    double total;  // 🚨 Uninitialized!
    bool flag;  // 🚨 Uninitialized!

    // 🚨 Using uninitialized variables = undefined behavior!
    if (flag) {  // Could be true or false randomly!
        count++;  // Adding to garbage value!
    }
}

class User {
private:
    string name;  // ✅ OK - string initializes itself
    int age;      // 🚨 Not initialized in constructor!
    bool active;  // 🚨 Not initialized!

public:
    User(const string& n) : name(n) {
        // 🚨 age and active contain garbage!
    }
};
}

namespace good_example4 {
// ✅ GOOD: Always initialize

void processData() {
    int count = 0;        // ✅ Initialized
    double total = 0.0;   // ✅ Initialized
    bool flag = false;    // ✅ Initialized

    vector<int> numbers;  // ✅ Empty, but valid state
    string text;          // ✅ Empty string

    // All variables have defined values!
    if (flag) {
        count++;
    }
}

class User {
private:
    string name;
    int age;
    bool active;

public:
    // ✅ Initialize all members in constructor
    User(const string& n, int a = 0)
        : name(n), age(a), active(true) {
        cout << "   ✅ User created: " << name
             << ", age " << age << endl;
    }

    // ✅ Or use default member initializers (C++11)
};

class Account {
private:
    string accountNumber;
    double balance = 0.0;      // ✅ Default initializer
    bool isActive = true;      // ✅ Default initializer
    int transactionCount = 0;  // ✅ Default initializer

public:
    Account(const string& num) : accountNumber(num) {
        cout << "   ✅ Account created with defaults" << endl;
    }
};
}

void example4_initialize_variables() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 4: INITIALIZE ALL VARIABLES            ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (Uninitialized):\n";
    cout << "   int count;      // 🚨 Contains garbage!\n";
    cout << "   bool flag;      // 🚨 Random value!\n";
    cout << "   double total;   // 🚨 Undefined!\n";
    cout << "   \n";
    cout << "   if (flag) {     // 🚨 Undefined behavior!\n";
    cout << "       count++;    // 🚨 Adding to garbage!\n";
    cout << "   }\n";

    cout << "\n✅ GOOD CODE (Always Initialize):\n";
    cout << "   int count = 0;        // ✅ Defined value\n";
    cout << "   bool flag = false;    // ✅ Defined value\n";
    cout << "   double total = 0.0;   // ✅ Defined value\n";
    cout << "   \n";
    cout << "   // Or use uniform initialization:\n";
    cout << "   int count{0};\n";
    cout << "   bool flag{false};\n";
    cout << "   double total{0.0};\n";

    cout << "\n✨ DEMONSTRATION:\n";

    good_example4::User user("Alice", 25);
    good_example4::Account account("ACC-12345");

    cout << "\n💡 INITIALIZATION METHODS:\n";
    cout << "\n   1. Direct initialization:\n";
    cout << "      int x = 5;\n";
    cout << "   \n";
    cout << "   2. Uniform initialization (C++11):\n";
    cout << "      int x{5};  // Preferred - prevents narrowing\n";
    cout << "   \n";
    cout << "   3. Member initializer list:\n";
    cout << "      MyClass(int x) : member(x) { }\n";
    cout << "   \n";
    cout << "   4. Default member initializers (C++11):\n";
    cout << "      class MyClass {\n";
    cout << "          int member = 0;  // Default value\n";
    cout << "      };\n";

    cout << "\n🎯 BENEFITS:\n";
    cout << "   ✅ No undefined behavior\n";
    cout << "   ✅ No garbage values\n";
    cout << "   ✅ Predictable program behavior\n";
    cout << "   ✅ Easier debugging\n";
    cout << "   ✅ Catches errors at compile time\n";
}

// ============================================
// EXAMPLE 5: PREFER REFERENCES OVER POINTERS
// ============================================

namespace bad_example5 {
// ❌ BAD: Unnecessary pointer usage

void processUser(User* user) {  // 🚨 Can be null!
    if (user) {  // 🚨 Need null check
        user->print();
    }
}

User* findUser(int id) {  // 🚨 Can return null
    if (id < 0) {
        return nullptr;  // Caller must check!
    }
    return new User("John", 30);  // 🚨 Who deletes this?
}

class Team {
private:
    User* leader;  // 🚨 Can be null, ownership unclear

public:
    void setLeader(User* l) {
        leader = l;  // 🚨 Who owns this? Do we delete?
    }
};
}

namespace good_example5 {
// ✅ GOOD: Use references when possible

void processUser(const Person& user) {  // ✅ Can't be null!
    user.print();  // No null check needed
}

// ✅ For optional values, use optional<T>
#include <optional>
optional<Person> findUser(int id) {
    if (id < 0) {
        return nullopt;  // Clear: no value
    }
    return Person("John", 30);
}

class Team {
private:
    Person& leader;  // ✅ Must exist, no null

public:
    // Reference must be initialized in constructor
    Team(Person& l) : leader(l) {
        cout << "   ✅ Team created with leader: " << l.getName() << endl;
    }

    Person& getLeader() {
        return leader;
    }
};

// ✅ When ownership is needed, use smart pointers
class Company {
private:
    unique_ptr<Person> ceo;  // ✅ Clear ownership

public:
    void setCEO(unique_ptr<Person> p) {
        ceo = std::move(p);
        cout << "   ✅ CEO set (ownership transferred)" << endl;
    }
};
}

void example5_references_vs_pointers() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 5: PREFER REFERENCES OVER POINTERS     ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (Unnecessary Pointers):\n";
    cout << "   void func(User* user) {  // 🚨 Can be null!\n";
    cout << "       if (user) {          // 🚨 Need null check\n";
    cout << "           user->process();\n";
    cout << "       }\n";
    cout << "   }\n";
    cout << "   \n";
    cout << "   User* u = findUser(123);  // 🚨 Might be null\n";
    cout << "   if (u) {                  // 🚨 Always checking\n";
    cout << "       func(u);\n";
    cout << "   }\n";

    cout << "\n✅ GOOD CODE (Use References):\n";
    cout << "   void func(const User& user) {  // ✅ Can't be null!\n";
    cout << "       user.process();            // ✅ No check needed\n";
    cout << "   }\n";
    cout << "   \n";
    cout << "   User& u = getUser(123);  // ✅ Must exist\n";
    cout << "   func(u);                 // ✅ No null checks\n";
    cout << "   \n";
    cout << "   // For optional values:\n";
    cout << "   optional<User> maybeUser = findUser(123);\n";
    cout << "   if (maybeUser) { /* use *maybeUser */ }\n";

    cout << "\n✨ DEMONSTRATION:\n";

    good_example3::Person alice("Alice", 30);
    cout << "\n   Using reference (can't be null):\n";
    good_example5::processUser(alice);

    cout << "\n   Creating team with reference:\n";
    good_example5::Team team(alice);

    cout << "\n📊 WHEN TO USE EACH:\n";
    cout << "\n   USE REFERENCES when:\n";
    cout << "   ✅ Value must exist (not optional)\n";
    cout << "   ✅ No ownership transfer\n";
    cout << "   ✅ No reassignment needed\n";
    cout << "   ✅ Passing to/from functions\n";

    cout << "\n   USE POINTERS when:\n";
    cout << "   ⚠️  Value is optional (can be null)\n";
    cout << "   ⚠️  Need to reassign (point to different object)\n";
    cout << "   ⚠️  Polymorphism with ownership (use unique_ptr)\n";
    cout << "   ⚠️  Shared ownership (use shared_ptr)\n";

    cout << "\n🎯 BENEFITS OF REFERENCES:\n";
    cout << "   ✅ No null pointer dereferences\n";
    cout << "   ✅ Cleaner syntax (. instead of ->)\n";
    cout << "   ✅ No null checks needed\n";
    cout << "   ✅ Clear intent: value must exist\n";
}

// ============================================
// EXAMPLE 6: USE SMART POINTERS
// ============================================

namespace bad_example6 {
// ❌ BAD: Raw pointers (manual memory management)

void createUser() {
    User* user = new User("John", 30);  // 🚨 Who deletes?

    // Do work...

    if (/* some condition */) {
        return;  // 🚨 MEMORY LEAK!
    }

    delete user;  // Only in happy path!
}

class UserManager {
private:
    User* users[100];  // 🚨 Who owns these?
    int count;

public:
    void addUser(User* u) {
        users[count++] = u;  // 🚨 Taking ownership?
    }

    ~UserManager() {
        // 🚨 Should we delete the users?
        // 🚨 What if someone else is using them?
    }
};
}

namespace good_example6 {
// ✅ GOOD: Smart pointers (automatic memory management)

void createUser() {
    unique_ptr<Person> user = make_unique<Person>("John", 30);

    cout << "   ✅ User created with unique_ptr" << endl;

    // Do work...

    if (/* some condition */) {
        return;  // ✅ Automatically deleted!
    }

}  // ✅ user automatically deleted here

class UserManager {
private:
    vector<unique_ptr<Person>> users;  // ✅ Clear ownership

public:
    void addUser(unique_ptr<Person> user) {
        users.push_back(std::move(user));  // ✅ Takes ownership
        cout << "   ✅ Added user (ownership transferred)" << endl;
    }

    // ✅ Destructor automatically cleans up all users
};

// ✅ shared_ptr for shared ownership
class Team {
private:
    shared_ptr<Person> leader;  // ✅ Can be shared

public:
    void setLeader(shared_ptr<Person> l) {
        leader = l;  // ✅ Reference count increased
        cout << "   ✅ Leader set (shared ownership)" << endl;
    }

    shared_ptr<Person> getLeader() {
        return leader;  // ✅ Safe to share
    }
};

// ✅ weak_ptr to break cycles
class Employee {
private:
    shared_ptr<Team> team;
    weak_ptr<Person> manager;  // ✅ Doesn't increase ref count

public:
    void setManager(shared_ptr<Person> m) {
        manager = m;  // ✅ Weak reference
    }

    void contactManager() {
        if (auto m = manager.lock()) {  // ✅ Check if still alive
            cout << "   ✅ Manager is: " << m->getName() << endl;
        } else {
            cout << "   ⚠️  Manager no longer available" << endl;
        }
    }
};
}

void example6_smart_pointers() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 6: USE SMART POINTERS                  ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (Raw Pointers):\n";
    cout << "   User* user = new User();  // 🚨 Who deletes?\n";
    cout << "   \n";
    cout << "   // Do work...\n";
    cout << "   if (error) {\n";
    cout << "       return;  // 🚨 MEMORY LEAK!\n";
    cout << "   }\n";
    cout << "   \n";
    cout << "   delete user;  // Must remember!\n";

    cout << "\n✅ GOOD CODE (Smart Pointers):\n";
    cout << "   unique_ptr<User> user = make_unique<User>();\n";
    cout << "   \n";
    cout << "   // Do work...\n";
    cout << "   if (error) {\n";
    cout << "       return;  // ✅ Automatically deleted!\n";
    cout << "   }\n";
    cout << "   // ✅ Automatically deleted at end of scope\n";

    cout << "\n✨ DEMONSTRATION:\n";

    cout << "\n   unique_ptr (exclusive ownership):\n";
    {
        unique_ptr<good_example3::Person> john =
            make_unique<good_example3::Person>("John", 30);
        cout << "   ✅ Created unique_ptr" << endl;
    }
    cout << "   ✅ Automatically deleted when scope ends" << endl;

    cout << "\n   shared_ptr (shared ownership):\n";
    {
        shared_ptr<good_example3::Person> alice =
            make_shared<good_example3::Person>("Alice", 28);
        {
            shared_ptr<good_example3::Person> aliceRef = alice;
            cout << "   ✅ Reference count: 2" << endl;
        }
        cout << "   ✅ Reference count: 1 (inner scope ended)" << endl;
    }
    cout << "   ✅ Deleted when last reference goes away" << endl;

    cout << "\n📊 SMART POINTER TYPES:\n";
    cout << "\n   unique_ptr:\n";
    cout << "   • Exclusive ownership\n";
    cout << "   • Can't be copied, only moved\n";
    cout << "   • Zero overhead\n";
    cout << "   • Use by default\n";

    cout << "\n   shared_ptr:\n";
    cout << "   • Shared ownership\n";
    cout << "   • Reference counted\n";
    cout << "   • Slight overhead\n";
    cout << "   • Use when multiple owners needed\n";

    cout << "\n   weak_ptr:\n";
    cout << "   • Non-owning reference\n";
    cout << "   • Doesn't increase ref count\n";
    cout << "   • Prevents cycles\n";
    cout << "   • Must check if still valid\n";

    cout << "\n🎯 BENEFITS:\n";
    cout << "   ✅ No memory leaks\n";
    cout << "   ✅ Exception-safe\n";
    cout << "   ✅ Clear ownership semantics\n";
    cout << "   ✅ RAII automatic cleanup\n";
}

// ============================================
// EXAMPLE 7: AVOID RAW new/delete
// ============================================

namespace bad_example7 {
// ❌ BAD: Manual new/delete

void processData() {
    int* array = new int[100];  // 🚨 Must remember to delete[]

    // Process data...

    for (int i = 0; i < 100; i++) {
        if (array[i] < 0) {
            return;  // 🚨 MEMORY LEAK!
        }
    }

    delete[] array;  // Only reached in happy path!
}

class DataProcessor {
private:
    int* buffer;
    size_t size;

public:
    DataProcessor(size_t s) : size(s) {
        buffer = new int[size];  // 🚨 Manual allocation
    }

    ~DataProcessor() {
        delete[] buffer;  // 🚨 Must remember delete[]
    }

    // 🚨 What about copy constructor?
    // 🚨 What about assignment operator?
    // Need Rule of Five!
};
}

namespace good_example7 {
// ✅ GOOD: Use containers and smart pointers

void processData() {
    vector<int> array(100);  // ✅ Automatic management

    // Process data...

    for (int i = 0; i < 100; i++) {
        if (array[i] < 0) {
            return;  // ✅ No leak! vector cleans up
        }
    }

}  // ✅ vector automatically destroyed

class DataProcessor {
private:
    vector<int> buffer;  // ✅ Automatic management

public:
    DataProcessor(size_t size) : buffer(size) {
        cout << "   ✅ Buffer created (vector manages memory)" << endl;
    }

    // ✅ Compiler-generated destructor is perfect
    // ✅ Compiler-generated copy/move are perfect
    // ✅ Rule of Zero achieved!
};

// ✅ If you must allocate dynamically, use smart pointers
class CustomAllocator {
private:
    unique_ptr<int[]> buffer;  // ✅ Smart pointer for arrays
    size_t size;

public:
    CustomAllocator(size_t s)
        : buffer(make_unique<int[]>(s)), size(s) {
        cout << "   ✅ Buffer allocated with unique_ptr" << endl;
    }

    // ✅ Automatically cleaned up
    // ✅ Can't be copied (unique ownership)
};
}

void example7_avoid_raw_new_delete() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 7: AVOID RAW new/delete                ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (Manual new/delete):\n";
    cout << "   int* array = new int[100];  // 🚨 Manual!\n";
    cout << "   \n";
    cout << "   // Process...\n";
    cout << "   if (error) {\n";
    cout << "       return;  // 🚨 LEAK! Forgot delete[]\n";
    cout << "   }\n";
    cout << "   \n";
    cout << "   delete[] array;  // Must remember!\n";

    cout << "\n✅ GOOD CODE (Use Containers):\n";
    cout << "   vector<int> array(100);  // ✅ Automatic!\n";
    cout << "   \n";
    cout << "   // Process...\n";
    cout << "   if (error) {\n";
    cout << "       return;  // ✅ No leak!\n";
    cout << "   }\n";
    cout << "   // ✅ Automatic cleanup\n";

    cout << "\n✨ DEMONSTRATION:\n";

    cout << "\n   Using vector (preferred):\n";
    {
        good_example7::DataProcessor proc(1000);
    }
    cout << "   ✅ Memory automatically freed" << endl;

    cout << "\n   Using smart pointer (when needed):\n";
    {
        good_example7::CustomAllocator alloc(500);
    }
    cout << "   ✅ Memory automatically freed" << endl;

    cout << "\n🎯 ALTERNATIVES TO new/delete:\n";
    cout << "\n   1. Use standard containers:\n";
    cout << "      vector<T>     - Dynamic array\n";
    cout << "      string        - Text\n";
    cout << "      array<T, N>   - Fixed-size array\n";
    cout << "      map<K, V>     - Key-value pairs\n";

    cout << "\n   2. Use smart pointers:\n";
    cout << "      unique_ptr<T> - Single ownership\n";
    cout << "      shared_ptr<T> - Shared ownership\n";
    cout << "      make_unique<T>() - Preferred creation\n";
    cout << "      make_shared<T>() - Preferred creation\n";

    cout << "\n   3. Use stack allocation:\n";
    cout << "      int array[100];  - Fixed size\n";
    cout << "      MyClass obj;     - Local object\n";

    cout << "\n💡 BENEFITS:\n";
    cout << "   ✅ No memory leaks\n";
    cout << "   ✅ Exception-safe\n";
    cout << "   ✅ Less code to write\n";
    cout << "   ✅ No manual cleanup needed\n";
    cout << "   ✅ Rule of Zero achieved\n";
}

// ============================================
// EXAMPLE 8: PASS BY CONST REFERENCE
// ============================================

namespace bad_example8 {
// ❌ BAD: Pass large objects by value

void processPerson(good_example3::Person p) {  // 🚨 Copies entire object!
    p.print();
    // Expensive copy for just reading!
}

void processVector(vector<int> v) {  // 🚨 Copies entire vector!
    for (int x : v) {
        cout << x << " ";
    }
    // Could be thousands of elements copied!
}

vector<int> getLargeVector() {
    vector<int> result(10000);
    // Fill vector...
    return result;  // 🚨 Potential copy (though RVO helps)
}
}

namespace good_example8 {
// ✅ GOOD: Pass by const reference

void processPerson(const good_example3::Person& p) {  // ✅ No copy!
    p.print();
    // Just a reference, no copying
}

void processVector(const vector<int>& v) {  // ✅ No copy!
    cout << "   Processing " << v.size() << " elements: ";
    for (int x : v) {
        cout << x << " ";
    }
    cout << endl;
}

// ✅ When modifying, pass by non-const reference
void modifyVector(vector<int>& v) {
    for (int& x : v) {
        x *= 2;
    }
}

// ✅ For output, return by value (RVO/move)
vector<int> getLargeVector() {
    vector<int> result(10);
    for (int i = 0; i < 10; i++) {
        result[i] = i;
    }
    return result;  // ✅ Move, not copy
}

// ✅ Small types can be passed by value
void processInt(int value) {  // ✅ OK - int is small
    cout << "   Value: " << value << endl;
}

void processDouble(double value) {  // ✅ OK - double is small
    cout << "   Value: " << value << endl;
}
}

void example8_pass_by_const_ref() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 8: PASS BY CONST REFERENCE             ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (Pass by Value):\n";
    cout << "   void processVector(vector<int> v) {  // 🚨 COPIES!\n";
    cout << "       // Entire vector copied unnecessarily\n";
    cout << "       for (int x : v) { /* ... */ }\n";
    cout << "   }\n";
    cout << "   \n";
    cout << "   vector<int> data(10000);  // Large vector\n";
    cout << "   processVector(data);      // 🚨 10000 ints copied!\n";

    cout << "\n✅ GOOD CODE (Pass by Const Reference):\n";
    cout << "   void processVector(const vector<int>& v) {  // ✅ No copy!\n";
    cout << "       // Just a reference, no copying\n";
    cout << "       for (int x : v) { /* ... */ }\n";
    cout << "   }\n";
    cout << "   \n";
    cout << "   vector<int> data(10000);\n";
    cout << "   processVector(data);  // ✅ Just passes reference!\n";

    cout << "\n✨ DEMONSTRATION:\n";

    vector<int> numbers = {1, 2, 3, 4, 5};

    cout << "\n   Passing by const reference (no copy):\n";
    good_example8::processVector(numbers);

    cout << "\n   Modifying with non-const reference:\n";
    cout << "   Before: ";
    for (int x : numbers) cout << x << " ";
    cout << endl;

    good_example8::modifyVector(numbers);
    cout << "   After:  ";
    for (int x : numbers) cout << x << " ";
    cout << endl;

    cout << "\n📊 PARAMETER PASSING GUIDELINES:\n";
    cout << "\n   Pass by VALUE:\n";
    cout << "   • Built-in types (int, double, char, bool)\n";
    cout << "   • Small types (sizeof <= 2*pointer)\n";
    cout << "   • When you need a copy anyway\n";

    cout << "\n   Pass by CONST REFERENCE:\n";
    cout << "   • Large objects (vector, string, custom classes)\n";
    cout << "   • When reading only (not modifying)\n";
    cout << "   • Default for user-defined types\n";

    cout << "\n   Pass by NON-CONST REFERENCE:\n";
    cout << "   • When function needs to modify argument\n";
    cout << "   • Output parameters\n";

    cout << "\n   Pass by POINTER:\n";
    cout << "   • When value is optional (can be nullptr)\n";
    cout << "   • When need to reassign\n";

    cout << "\n   Pass by RVALUE REFERENCE (&&):\n";
    cout << "   • For perfect forwarding\n";
    cout << "   • When taking ownership\n";

    cout << "\n🎯 BENEFITS:\n";
    cout << "   ✅ No unnecessary copies\n";
    cout << "   ✅ Better performance\n";
    cout << "   ✅ Const prevents accidental modification\n";
    cout << "   ✅ Clear intent (reading vs modifying)\n";
}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║              C++ BEST PRACTICES                       ║\n";
    cout << "║                                                       ║\n";
    cout << "║  \"Follow proven patterns and idioms that make         ║\n";
    cout << "║   C++ code safer, more efficient, and maintainable.\" ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n";

    example1_raii();
    example2_rule_of_five();
    example3_const_correctness();
    example4_initialize_variables();
    example5_references_vs_pointers();
    example6_smart_pointers();
    example7_avoid_raw_new_delete();
    example8_pass_by_const_ref();

    cout << "\n\n";
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║         🎉 C++ BEST PRACTICES COMPLETE! 🎉            ║\n";
    cout << "║                                                       ║\n";
    cout << "║  Key Takeaway: Modern C++ provides powerful tools     ║\n";
    cout << "║  to write safe, efficient code. Use them!             ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n\n";

    return 0;
}
