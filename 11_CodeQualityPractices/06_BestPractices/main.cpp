#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <mutex>
#include <cstring>
#include <optional>
#include <print>

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
        std::println("   ❌ Can't open file");
        return;  // 🚨 No cleanup needed here, but...
    }

    // Do some processing
    char buffer[100];
    if (!fgets(buffer, 100, file)) {
        std::println("   ❌ Read error");
        // 🚨 FORGOT TO CLOSE FILE! Memory leak!
        return;
    }

    // More processing that might throw
    try {
        // What if this throws?
        if (strlen(buffer) == 0) {
            throw std::runtime_error("Empty file");
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
        std::println("   📡 Connected to database");
    }

    void disconnect() {
        if (connected) {
            std::println("   🔌 Disconnected from database");
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
    FileHandle(const std::string& filename, const char* mode) {
        file = fopen(filename.c_str(), mode);
        if (!file) {
            throw std::runtime_error("Can't open file: " + filename);
        }
        std::println("   📂 Opened file: {}", filename);
    }

    ~FileHandle() {
        if (file) {
            fclose(file);
            std::println("   📂 Closed file automatically");
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
            throw std::runtime_error("Empty");  // ✅ File automatically closed!
        }

        // More processing...

    } // ✅ Destructor called here, file ALWAYS closed!
    catch (const std::exception& e) {
        std::println("   ❌ Error: {}", e.what());
    }
}

class DatabaseConnection {
private:
    bool connected;

public:
    DatabaseConnection() : connected(false) {
        connected = true;
        std::println("   📡 Connected to database");
    }

    ~DatabaseConnection() {
        if (connected) {
            std::println("   🔌 Disconnected automatically");
            connected = false;
        }
    }

    // Delete copy operations
    DatabaseConnection(const DatabaseConnection&) = delete;
    DatabaseConnection& operator=(const DatabaseConnection&) = delete;

    void query() {
        std::println("   🔍 Executing query...");
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
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 1: RAII                                ║");
    std::println("║   (Resource Acquisition Is Initialization)       ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   ");
    std::println("   ");

    good_example1::queryDatabase();

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
        std::println("   📦 Allocated buffer of size {}", size);
    }

    ~Buffer() {
        delete[] data;
        std::println("   🗑️  Deleted buffer");
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
    std::vector<int> data;  // ✅ Standard container handles everything

public:
    BufferRuleOfZero(size_t size) : data(size) {
        std::println("   📦 Created buffer (Rule of Zero)");
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
        std::println("   📦 Allocated {} ints", size);
    }

    // Destructor
    ~BufferRuleOfFive() {
        delete[] data;
        std::println("   🗑️  Freed memory");
    }

    // Copy Constructor
    BufferRuleOfFive(const BufferRuleOfFive& other) : size(other.size) {
        data = new int[size];
        std::copy(other.data, other.data + size, data);
        std::println("   📋 Copy constructed");
    }

    // Copy Assignment
    BufferRuleOfFive& operator=(const BufferRuleOfFive& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = new int[size];
            std::copy(other.data, other.data + size, data);
            std::println("   📋 Copy assigned");
        }
        return *this;
    }

    // Move Constructor
    BufferRuleOfFive(BufferRuleOfFive&& other) noexcept
        : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
        std::println("   🚚 Move constructed");
    }

    // Move Assignment
    BufferRuleOfFive& operator=(BufferRuleOfFive&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
            std::println("   🚚 Move assigned");
        }
        return *this;
    }
};
}

void example2_rule_of_five() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 2: RULE OF ZERO/THREE/FIVE             ║");
    std::println("╚══════════════════════════════════════════════════╝");

    {
        good_example2::BufferRuleOfFive buf1(5);
        good_example2::BufferRuleOfFive buf2 = buf1;  // Copy constructor
        good_example2::BufferRuleOfFive buf3(10);
        buf3 = buf1;  // Copy assignment
        good_example2::BufferRuleOfFive buf4 = std::move(buf1);  // Move constructor
    }

}

// ============================================
// EXAMPLE 3: CONST CORRECTNESS
// ============================================

namespace bad_example3 {
// ❌ BAD: No const correctness

class Person {
private:
    std::string name;
    int age;

public:
    Person(std::string n, int a) : name(n), age(a) {}

    // 🚨 Should be const but isn't!
    std::string getName() { return name; }
    int getAge() { return age; }

    // 🚨 Can modify when it shouldn't!
    void print() {
        std::println("{}, {}", name, age);
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
    std::string name;
    int age;

public:
    Person(const std::string& n, int a) : name(n), age(a) {}

    // ✅ Const member functions
    std::string getName() const { return name; }
    int getAge() const { return age; }

    void print() const {
        std::println("   {}, age {}", name, age);
    }

    // ✅ Non-const for modification
    void setName(const std::string& n) { name = n; }
    void setAge(int a) { age = a; }
};

// ✅ Const parameters when not modifying
void printPerson(const Person& p) {
    p.print();
}

// ✅ Const reference return for members
class Team {
private:
    std::vector<Person> members;

public:
    void addMember(const Person& p) {
        members.push_back(p);
    }

    // ✅ Const version
    const std::vector<Person>& getMembers() const {
        return members;
    }

    // ✅ Non-const version for modification
    std::vector<Person>& getMembers() {
        return members;
    }
};
}

void example3_const_correctness() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 3: CONST CORRECTNESS                   ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   ");

    std::println("   ");

    good_example3::Person john("John Doe", 30);
    std::println("");
    std::println("   Created person:");
    john.print();

    const good_example3::Person jane("Jane Smith", 28);
    std::println("");
    std::println("   Const person can still be read:");
    jane.print();
    // jane.setAge(29);  // ❌ Won't compile - const!

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
    std::string name;  // ✅ OK - string initializes itself
    int age;      // 🚨 Not initialized in constructor!
    bool active;  // 🚨 Not initialized!

public:
    User(const std::string& n) : name(n) {
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

    std::vector<int> numbers;  // ✅ Empty, but valid state
    std::string text;          // ✅ Empty string

    // All variables have defined values!
    if (flag) {
        count++;
    }
}

class User {
private:
    std::string name;
    int age;
    bool active;

public:
    // ✅ Initialize all members in constructor
    User(const std::string& n, int a = 0)
        : name(n), age(a), active(true) {
        std::println("   ✅ User created: {}, age {}", name, age);
    }

    // ✅ Or use default member initializers (C++11)
};

class Account {
private:
    std::string accountNumber;
    double balance = 0.0;      // ✅ Default initializer
    bool isActive = true;      // ✅ Default initializer
    int transactionCount = 0;  // ✅ Default initializer

public:
    Account(const std::string& num) : accountNumber(num) {
        std::println("   ✅ Account created with defaults");
    }
};
}

void example4_initialize_variables() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 4: INITIALIZE ALL VARIABLES            ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   ");

    std::println("   ");

    good_example4::User user("Alice", 25);
    good_example4::Account account("ACC-12345");

    std::println("   ");
    std::println("   ");
    std::println("   ");
}

// ============================================
// EXAMPLE 5: PREFER REFERENCES OVER POINTERS
// ============================================

namespace bad_example5 {
// ❌ BAD: Unnecessary pointer usage

struct User {
    std::string name;
    int age;
    User(std::string n, int a) : name(std::move(n)), age(a) {}
    void print() const { std::println("User: {} ({})", name, age); }
};

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
using good_example3::Person;
// ✅ GOOD: Use references when possible

void processUser(const Person& user) {  // ✅ Can't be null!
    user.print();  // No null check needed
}

// ✅ For optional values, use optional<T>
std::optional<Person> findUser(int id) {
    if (id < 0) {
        return std::nullopt;  // Clear: no value
    }
    return Person("John", 30);
}

class Team {
private:
    Person& leader;  // ✅ Must exist, no null

public:
    // Reference must be initialized in constructor
    Team(Person& l) : leader(l) {
        std::println("   ✅ Team created with leader: {}", l.getName());
    }

    Person& getLeader() {
        return leader;
    }
};

// ✅ When ownership is needed, use smart pointers
class Company {
private:
    std::unique_ptr<Person> ceo;  // ✅ Clear ownership

public:
    void setCEO(std::unique_ptr<Person> p) {
        ceo = std::move(p);
        std::println("   ✅ CEO set (ownership transferred)");
    }
};
}

void example5_references_vs_pointers() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 5: PREFER REFERENCES OVER POINTERS     ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   ");

    std::println("   ");
    std::println("   ");

    good_example3::Person alice("Alice", 30);
    std::println("");
    std::println("   Using reference (can't be null):");
    good_example5::processUser(alice);

    std::println("");
    std::println("   Creating team with reference:");
    good_example5::Team team(alice);

}

// ============================================
// EXAMPLE 6: USE SMART POINTERS
// ============================================

namespace bad_example6 {
// ❌ BAD: Raw pointers (manual memory management)

using bad_example5::User;

void createUser() {
    User* user = new User("John", 30);  // 🚨 Who deletes?

    // Do work...

    bool someCondition = false;  // pretend this depends on run-time input
    if (someCondition) {
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
using good_example3::Person;
// ✅ GOOD: Smart pointers (automatic memory management)

void createUser() {
    std::unique_ptr<Person> user = std::make_unique<Person>("John", 30);

    std::println("   ✅ User created with unique_ptr");

    // Do work...

    bool someCondition = false;  // pretend this depends on run-time input
    if (someCondition) {
        return;  // ✅ Automatically deleted!
    }

}  // ✅ user automatically deleted here

class UserManager {
private:
    std::vector<std::unique_ptr<Person>> users;  // ✅ Clear ownership

public:
    void addUser(std::unique_ptr<Person> user) {
        users.push_back(std::move(user));  // ✅ Takes ownership
        std::println("   ✅ Added user (ownership transferred)");
    }

    // ✅ Destructor automatically cleans up all users
};

// ✅ shared_ptr for shared ownership
class Team {
private:
    std::shared_ptr<Person> leader;  // ✅ Can be shared

public:
    void setLeader(std::shared_ptr<Person> l) {
        leader = l;  // ✅ Reference count increased
        std::println("   ✅ Leader set (shared ownership)");
    }

    std::shared_ptr<Person> getLeader() {
        return leader;  // ✅ Safe to share
    }
};

// ✅ weak_ptr to break cycles
class Employee {
private:
    std::shared_ptr<Team> team;
    std::weak_ptr<Person> manager;  // ✅ Doesn't increase ref count

public:
    void setManager(std::shared_ptr<Person> m) {
        manager = m;  // ✅ Weak reference
    }

    void contactManager() {
        if (auto m = manager.lock()) {  // ✅ Check if still alive
            std::println("   ✅ Manager is: {}", m->getName());
        } else {
            std::println("   ⚠️  Manager no longer available");
        }
    }
};
}

void example6_smart_pointers() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 6: USE SMART POINTERS                  ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   ");
    std::println("   ");

    std::println("   ");

    {
        std::unique_ptr<good_example3::Person> john =
            std::make_unique<good_example3::Person>("John", 30);
        std::println("   ✅ Created unique_ptr");
    }
    std::println("   ✅ Automatically deleted when scope ends");

    std::println("");
    std::println("   shared_ptr (shared ownership):");
    {
        std::shared_ptr<good_example3::Person> alice =
            std::make_shared<good_example3::Person>("Alice", 28);
        {
            std::shared_ptr<good_example3::Person> aliceRef = alice;
            std::println("   ✅ Reference count: 2");
        }
        std::println("   ✅ Reference count: 1 (inner scope ended)");
    }

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
    std::vector<int> array(100);  // ✅ Automatic management

    // Process data...

    for (int i = 0; i < 100; i++) {
        if (array[i] < 0) {
            return;  // ✅ No leak! vector cleans up
        }
    }

}  // ✅ vector automatically destroyed

class DataProcessor {
private:
    std::vector<int> buffer;  // ✅ Automatic management

public:
    DataProcessor(size_t size) : buffer(size) {
        std::println("   ✅ Buffer created (vector manages memory)");
    }

    // ✅ Compiler-generated destructor is perfect
    // ✅ Compiler-generated copy/move are perfect
    // ✅ Rule of Zero achieved!
};

// ✅ If you must allocate dynamically, use smart pointers
class CustomAllocator {
private:
    std::unique_ptr<int[]> buffer;  // ✅ Smart pointer for arrays
    size_t size;

public:
    CustomAllocator(size_t s)
        : buffer(std::make_unique<int[]>(s)), size(s) {
        std::println("   ✅ Buffer allocated with unique_ptr");
    }

    // ✅ Automatically cleaned up
    // ✅ Can't be copied (unique ownership)
};
}

void example7_avoid_raw_new_delete() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 7: AVOID RAW new/delete                ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   ");
    std::println("   ");

    std::println("   ");

    {
        good_example7::DataProcessor proc(1000);
    }
    std::println("   ✅ Memory automatically freed");

    std::println("");
    std::println("   Using smart pointer (when needed):");
    {
        good_example7::CustomAllocator alloc(500);
    }

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

void processVector(std::vector<int> v) {  // 🚨 Copies entire vector!
    for (int x : v) {
        std::print("{} ", x);
    }
    // Could be thousands of elements copied!
}

std::vector<int> getLargeVector() {
    std::vector<int> result(10000);
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

void processVector(const std::vector<int>& v) {  // ✅ No copy!
    std::print("   Processing {} elements: ", v.size());
    for (int x : v) {
        std::print("{} ", x);
    }
    std::println("");
}

// ✅ When modifying, pass by non-const reference
void modifyVector(std::vector<int>& v) {
    for (int& x : v) {
        x *= 2;
    }
}

// ✅ For output, return by value (RVO/move)
std::vector<int> getLargeVector() {
    std::vector<int> result(10);
    for (int i = 0; i < 10; i++) {
        result[i] = i;
    }
    return result;  // ✅ Move, not copy
}

// ✅ Small types can be passed by value
void processInt(int value) {  // ✅ OK - int is small
    std::println("   Value: {}", value);
}

void processDouble(double value) {  // ✅ OK - double is small
    std::println("   Value: {}", value);
}
}

void example8_pass_by_const_ref() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 8: PASS BY CONST REFERENCE             ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   ");

    std::println("   ");

    std::vector<int> numbers = {1, 2, 3, 4, 5};

    std::println("");
    std::println("   Passing by const reference (no copy):");
    good_example8::processVector(numbers);

    std::println("");
    std::println("   Modifying with non-const reference:");
    std::print("   Before: ");
    for (int x : numbers) std::print("{} ", x);
    std::println("");

    good_example8::modifyVector(numbers);
    std::print("   After:  ");
    for (int x : numbers) std::print("{} ", x);
    std::println("");

}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    std::println("╔═══════════════════════════════════════════════════════╗");
    std::println("║                                                       ║");
    std::println("║              C++ BEST PRACTICES                       ║");
    std::println("║                                                       ║");
    std::println("║  \"Follow proven patterns and idioms that make        ║");
    std::println("║   C++ code safer, more efficient, and maintainable.\" ║");
    std::println("║                                                       ║");
    std::println("╚═══════════════════════════════════════════════════════╝");

    example1_raii();
    example2_rule_of_five();
    example3_const_correctness();
    example4_initialize_variables();
    example5_references_vs_pointers();
    example6_smart_pointers();
    example7_avoid_raw_new_delete();
    example8_pass_by_const_ref();

    return 0;
}
