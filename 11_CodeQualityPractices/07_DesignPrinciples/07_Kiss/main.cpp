#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <algorithm>
#include <functional>
using namespace std;

// ============================================
// KISS PRINCIPLE (Keep It Simple, Stupid)
// ============================================

/*
KISS PRINCIPLE:
    "Keep It Simple, Stupid"
    "Simplicity should be a key goal in design,
     and unnecessary complexity should be avoided."
    — Kelly Johnson (Lockheed Skunk Works)

WHAT IT MEANS:
    • Favor simple solutions over complex ones
    • Avoid unnecessary abstractions
    • Write code that's easy to understand
    • Don't over-engineer
    • Simplest solution that works is often the best

WHY IT MATTERS:
    ✅ Easier to understand and maintain
    ✅ Fewer bugs (less code = fewer bugs)
    ✅ Faster to implement
    ✅ Easier to test
    ✅ Easier for others to modify

KEY CONCEPTS:
    • Simplicity = Easy to understand, not necessarily short
    • Complexity = Hard to understand, predict, or maintain
    • Over-engineering = Adding unnecessary features/abstractions
    • YAGNI = You Aren't Gonna Need It
    • Essential complexity vs Accidental complexity

VIOLATION SIGNS:
    🚨 Code that's hard to explain
    🚨 Too many layers of abstraction
    🚨 Solving problems you don't have
    🚨 Clever tricks instead of clear code
    🚨 "Future-proofing" that isn't needed

TYPES OF COMPLEXITY:
    1. Essential complexity (inherent to problem)
    2. Accidental complexity (introduced by solution)

GOAL: Minimize accidental complexity!
*/

// ============================================
// EXAMPLE 1: SIMPLE CONDITIONALS
// ============================================

namespace bad_example1 {
// ❌ BAD: Overcomplicated condition checking

class UserValidator {
private:
    bool checkAge(int age) { return age >= 18; }
    bool checkEmail(const string& email) { return email.find("@") != string::npos; }
    bool checkName(const string& name) { return !name.empty() && name.length() >= 2; }

public:
    bool validate(int age, const string& email, const string& name) {
        // 🚨 Unnecessarily complex validation chain
        function<bool()> ageValidator = [this, age]() { return checkAge(age); };
        function<bool()> emailValidator = [this, &email]() { return checkEmail(email); };
        function<bool()> nameValidator = [this, &name]() { return checkName(name); };

        vector<function<bool()>> validators = {ageValidator, emailValidator, nameValidator};

        for (const auto& validator : validators) {
            if (!validator()) {
                return false;
            }
        }

        return true;
    }
};

// PROBLEMS:
// • Way too complex for simple validation
// • Lambdas and function objects unnecessary
// • Hard to debug
// • Unclear what's being validated
}

namespace good_example1 {
// ✅ GOOD: Simple, straightforward validation

bool isValidAge(int age) {
    return age >= 18;
}

bool isValidEmail(const string& email) {
    return email.find("@") != string::npos;
}

bool isValidName(const string& name) {
    return !name.empty() && name.length() >= 2;
}

bool validateUser(int age, const string& email, const string& name) {
    return isValidAge(age) &&
           isValidEmail(email) &&
           isValidName(name);
}
}

void example1_conditionals() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 1: SIMPLE CONDITIONALS                ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (Overcomplicated):\n";
    cout << "```cpp\n";
    cout << "function<bool()> ageValidator = [this, age]() { ... };\n";
    cout << "vector<function<bool()>> validators = {...};\n";
    cout << "for (const auto& validator : validators) { ... }\n";
    cout << "// 🚨 Too complex for simple validation!\n";
    cout << "```\n";

    cout << "\n🚨 PROBLEMS:\n";
    cout << "   • Unnecessary abstraction with lambdas\n";
    cout << "   • Hard to understand at a glance\n";
    cout << "   • Difficult to debug\n";
    cout << "   • Over-engineering simple validation\n";

    cout << "\n✅ GOOD CODE (Simple):\n";
    cout << "```cpp\n";
    cout << "bool validateUser(int age, string email, string name) {\n";
    cout << "    return isValidAge(age) &&\n";
    cout << "           isValidEmail(email) &&\n";
    cout << "           isValidName(name);\n";
    cout << "}\n";
    cout << "// ✅ Clear, simple, easy to understand!\n";
    cout << "```\n";

    cout << "\n✨ DEMONSTRATION:\n";

    cout << "\n   Valid user:\n";
    bool valid = good_example1::validateUser(25, "user@example.com", "John");
    cout << "   Result: " << (valid ? "✅ Valid" : "❌ Invalid") << endl;

    cout << "\n   Invalid user (too young):\n";
    valid = good_example1::validateUser(16, "user@example.com", "John");
    cout << "   Result: " << (valid ? "✅ Valid" : "❌ Invalid") << endl;

    cout << "\n✨ BENEFITS:\n";
    cout << "   • Immediately clear what's happening\n";
    cout << "   • Easy to modify or extend\n";
    cout << "   • Less code, fewer bugs\n";
    cout << "   • Anyone can understand it\n";
}

// ============================================
// EXAMPLE 2: OVER-ENGINEERED CALCULATOR
// ============================================

namespace bad_example2 {
// ❌ BAD: Over-engineered with unnecessary abstractions

class Operation {
public:
    virtual ~Operation() = default;
    virtual double execute(double a, double b) = 0;
    virtual string getName() const = 0;
};

class AddOperation : public Operation {
public:
    double execute(double a, double b) override { return a + b; }
    string getName() const override { return "add"; }
};

class SubtractOperation : public Operation {
public:
    double execute(double a, double b) override { return a - b; }
    string getName() const override { return "subtract"; }
};

class MultiplyOperation : public Operation {
public:
    double execute(double a, double b) override { return a * b; }
    string getName() const override { return "multiply"; }
};

class DivideOperation : public Operation {
public:
    double execute(double a, double b) override { return a / b; }
    string getName() const override { return "divide"; }
};

class OperationFactory {
private:
    map<string, shared_ptr<Operation>> operations;

public:
    OperationFactory() {
        operations["add"] = make_shared<AddOperation>();
        operations["subtract"] = make_shared<SubtractOperation>();
        operations["multiply"] = make_shared<MultiplyOperation>();
        operations["divide"] = make_shared<DivideOperation>();
    }

    shared_ptr<Operation> getOperation(const string& name) {
        return operations[name];
    }
};

class Calculator {
private:
    OperationFactory factory;

public:
    double calculate(const string& operation, double a, double b) {
        auto op = factory.getOperation(operation);
        return op->execute(a, b);
    }
};

// 🚨 PROBLEM: Way too complex for a simple calculator!
// 6 classes, inheritance, factory pattern, shared_ptr...
// All for basic arithmetic!
}

namespace good_example2 {
// ✅ GOOD: Simple and clear

double add(double a, double b) { return a + b; }
double subtract(double a, double b) { return a - b; }
double multiply(double a, double b) { return a * b; }
double divide(double a, double b) { return b != 0 ? a / b : 0; }

double calculate(const string& operation, double a, double b) {
    if (operation == "add") return add(a, b);
    if (operation == "subtract") return subtract(a, b);
    if (operation == "multiply") return multiply(a, b);
    if (operation == "divide") return divide(a, b);
    return 0;
}
}

void example2_calculator() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 2: OVER-ENGINEERED CALCULATOR         ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (Over-engineered):\n";
    cout << "   • 6 classes: Operation, AddOperation, SubtractOperation, etc.\n";
    cout << "   • Factory pattern\n";
    cout << "   • Inheritance hierarchy\n";
    cout << "   • shared_ptr management\n";
    cout << "   • 100+ lines of code\n";
    cout << "   🚨 All for simple addition/subtraction!\n";

    cout << "\n✅ GOOD CODE (Simple):\n";
    cout << "   • Simple functions\n";
    cout << "   • One function to dispatch\n";
    cout << "   • 15 lines of code\n";
    cout << "   • Clear and maintainable\n";

    cout << "\n✨ DEMONSTRATION:\n";

    cout << "\n   10 + 5 = " << good_example2::calculate("add", 10, 5) << endl;
    cout << "   10 - 5 = " << good_example2::calculate("subtract", 10, 5) << endl;
    cout << "   10 * 5 = " << good_example2::calculate("multiply", 10, 5) << endl;
    cout << "   10 / 5 = " << good_example2::calculate("divide", 10, 5) << endl;

    cout << "\n✨ BENEFITS:\n";
    cout << "   • 85% less code\n";
    cout << "   • Anyone can understand it\n";
    cout << "   • Easy to modify\n";
    cout << "   • No unnecessary abstractions\n";

    cout << "\n💡 LESSON:\n";
    cout << "   Don't use design patterns just because you can!\n";
    cout << "   Use them when the complexity is justified.\n";
}

// ============================================
// EXAMPLE 3: BOOLEAN SIMPLIFICATION
// ============================================

namespace bad_example3 {
// ❌ BAD: Complex boolean logic

bool canAccessResource(bool isLoggedIn, bool hasPermission,
                       bool isAdmin, bool isResourcePublic) {
    // 🚨 Overcomplicated condition
    if (isLoggedIn == true) {
        if (hasPermission == true) {
            return true;
        } else {
            if (isAdmin == true) {
                return true;
            } else {
                if (isResourcePublic == true) {
                    return true;
                } else {
                    return false;
                }
            }
        }
    } else {
        if (isResourcePublic == true) {
            return true;
        } else {
            return false;
        }
    }
}
}

namespace good_example3 {
// ✅ GOOD: Simplified boolean logic

bool canAccessResource(bool isLoggedIn, bool hasPermission,
                       bool isAdmin, bool isResourcePublic) {
    // Clear, simple logic
    if (isResourcePublic) return true;
    if (!isLoggedIn) return false;
    return hasPermission || isAdmin;
}
}

void example3_boolean() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 3: BOOLEAN SIMPLIFICATION             ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE:\n";
    cout << "   Nested if-else statements\n";
    cout << "   Comparing booleans to true/false\n";
    cout << "   Hard to follow logic flow\n";

    cout << "\n✅ GOOD CODE:\n";
    cout << "```cpp\n";
    cout << "if (isResourcePublic) return true;\n";
    cout << "if (!isLoggedIn) return false;\n";
    cout << "return hasPermission || isAdmin;\n";
    cout << "```\n";

    cout << "\n✨ DEMONSTRATION:\n";

    bool result;

    cout << "\n   Public resource, not logged in:\n";
    result = good_example3::canAccessResource(false, false, false, true);
    cout << "   Access: " << (result ? "✅ Granted" : "❌ Denied") << endl;

    cout << "\n   Private resource, logged in, has permission:\n";
    result = good_example3::canAccessResource(true, true, false, false);
    cout << "   Access: " << (result ? "✅ Granted" : "❌ Denied") << endl;

    cout << "\n   Private resource, logged in as admin:\n";
    result = good_example3::canAccessResource(true, false, true, false);
    cout << "   Access: " << (result ? "✅ Granted" : "❌ Denied") << endl;

    cout << "\n✨ BENEFITS:\n";
    cout << "   • 70% less code\n";
    cout << "   • Logic clear at a glance\n";
    cout << "   • Easy to understand and modify\n";
}

// ============================================
// EXAMPLE 4: UNNECESSARY ABSTRACTION LAYERS
// ============================================

namespace bad_example4 {
// ❌ BAD: Too many abstraction layers

class DataAccessInterface {
public:
    virtual ~DataAccessInterface() = default;
    virtual string getData() = 0;
};

class DataAccessImplementation : public DataAccessInterface {
public:
    string getData() override { return "data"; }
};

class DataAccessProxy : public DataAccessInterface {
private:
    shared_ptr<DataAccessInterface> implementation;
public:
    DataAccessProxy(shared_ptr<DataAccessInterface> impl) : implementation(impl) {}
    string getData() override { return implementation->getData(); }
};

class DataAccessDecorator : public DataAccessInterface {
private:
    shared_ptr<DataAccessInterface> wrapped;
public:
    DataAccessDecorator(shared_ptr<DataAccessInterface> w) : wrapped(w) {}
    string getData() override { return wrapped->getData(); }
};

class DataService {
private:
    shared_ptr<DataAccessInterface> dataAccess;
public:
    DataService(shared_ptr<DataAccessInterface> da) : dataAccess(da) {}
    string retrieveData() { return dataAccess->getData(); }
};

// 🚨 PROBLEM: 5 classes to return "data"!
}

namespace good_example4 {
// ✅ GOOD: Direct and simple

class DataService {
public:
    string getData() {
        return "data";
    }
};
}

void example4_abstraction() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 4: UNNECESSARY ABSTRACTION LAYERS      ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE:\n";
    cout << "   5 classes: Interface → Implementation → Proxy → Decorator → Service\n";
    cout << "   Multiple layers of indirection\n";
    cout << "   Shared pointers everywhere\n";
    cout << "   All to return a simple string!\n";

    cout << "\n✅ GOOD CODE:\n";
    cout << "```cpp\n";
    cout << "class DataService {\n";
    cout << "public:\n";
    cout << "    string getData() { return \"data\"; }\n";
    cout << "};\n";
    cout << "```\n";

    cout << "\n💡 RULE:\n";
    cout << "   Add abstraction layers ONLY when:\n";
    cout << "   • Multiple implementations needed NOW\n";
    cout << "   • Testing requires mocking\n";
    cout << "   • Requirement clearly documented\n";
    cout << "   \n";
    cout << "   Don't add layers \"just in case\"!\n";
}

// ============================================
// EXAMPLE 5: SIMPLE DATA STRUCTURES
// ============================================

namespace bad_example5 {
// ❌ BAD: Over-complicated data structure

template<typename T>
class Node {
public:
    T data;
    shared_ptr<Node<T>> next;
    weak_ptr<Node<T>> prev;

    Node(T d) : data(d) {}
};

template<typename T>
class List {
private:
    shared_ptr<Node<T>> head;
    shared_ptr<Node<T>> tail;
    int size;

public:
    List() : size(0) {}

    void add(T item) {
        auto node = make_shared<Node<T>>(item);
        if (!head) {
            head = tail = node;
        } else {
            tail->next = node;
            node->prev = tail;
            tail = node;
        }
        size++;
    }

    void print() {
        auto current = head;
        while (current) {
            cout << "   • " << current->data << endl;
            current = current->next;
        }
    }
};

// 🚨 PROBLEM: Custom linked list when vector would work fine!
}

namespace good_example5 {
// ✅ GOOD: Use standard library

template<typename T>
class List {
private:
    vector<T> items;

public:
    void add(T item) {
        items.push_back(item);
    }

    void print() {
        for (const auto& item : items) {
            cout << "   • " << item << endl;
        }
    }

    int size() const { return items.size(); }
};
}

void example5_data_structures() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 5: SIMPLE DATA STRUCTURES              ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE:\n";
    cout << "   Custom linked list with:\n";
    cout << "   • shared_ptr for next\n";
    cout << "   • weak_ptr for prev\n";
    cout << "   • Manual memory management\n";
    cout << "   • Complex add/remove logic\n";

    cout << "\n✅ GOOD CODE:\n";
    cout << "   Use vector<T> from standard library!\n";
    cout << "   • Well-tested\n";
    cout << "   • Optimized\n";
    cout << "   • Simple to use\n";

    cout << "\n✨ DEMONSTRATION:\n";

    good_example5::List<string> list;
    list.add("Apple");
    list.add("Banana");
    list.add("Cherry");

    cout << "\n   Items in list:\n";
    list.print();

    cout << "\n💡 RULE:\n";
    cout << "   Use standard library containers!\n";
    cout << "   Don't reinvent the wheel unless:\n";
    cout << "   • Specific performance requirement\n";
    cout << "   • Unique behavior needed\n";
    cout << "   • Standard library doesn't fit\n";
}

// ============================================
// EXAMPLE 6: CLEVER CODE VS CLEAR CODE
// ============================================

namespace bad_example6 {
// ❌ BAD: Clever but unclear

// Check if number is even (clever one-liner)
bool isEven(int n) {
    return !(n & 1);  // 🚨 Bitwise trick - clever but unclear
}

// Swap without temporary variable (clever)
void swap(int& a, int& b) {
    a ^= b ^= a ^= b;  // 🚨 XOR swap - clever but confusing
}

// Calculate factorial (clever recursion)
int factorial(int n) {
    return n > 1 ? n * factorial(n - 1) : 1;  // 🚨 One-liner, but harder to read
}
}

namespace good_example6 {
// ✅ GOOD: Clear and obvious

bool isEven(int n) {
    return n % 2 == 0;  // ✅ Clear: check if divisible by 2
}

void swap(int& a, int& b) {
    int temp = a;  // ✅ Clear: use temporary variable
    a = b;
    b = temp;
}

int factorial(int n) {
    int result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;  // ✅ Clear: iterative approach
}
}

void example6_clever_vs_clear() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 6: CLEVER CODE VS CLEAR CODE          ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (Clever):\n";
    cout << "```cpp\n";
    cout << "bool isEven(int n) { return !(n & 1); }\n";
    cout << "void swap(int& a, int& b) { a ^= b ^= a ^= b; }\n";
    cout << "// 🚨 Clever, but unclear!\n";
    cout << "```\n";

    cout << "\n✅ GOOD CODE (Clear):\n";
    cout << "```cpp\n";
    cout << "bool isEven(int n) { return n % 2 == 0; }\n";
    cout << "void swap(int& a, int& b) {\n";
    cout << "    int temp = a; a = b; b = temp;\n";
    cout << "}\n";
    cout << "// ✅ Clear and obvious!\n";
    cout << "```\n";

    cout << "\n✨ DEMONSTRATION:\n";

    cout << "\n   Is 4 even? " << (good_example6::isEven(4) ? "Yes" : "No") << endl;
    cout << "   Is 7 even? " << (good_example6::isEven(7) ? "Yes" : "No") << endl;

    int a = 10, b = 20;
    cout << "\n   Before swap: a = " << a << ", b = " << b << endl;
    good_example6::swap(a, b);
    cout << "   After swap:  a = " << a << ", b = " << b << endl;

    cout << "\n   Factorial of 5: " << good_example6::factorial(5) << endl;

    cout << "\n💡 RULE:\n";
    cout << "   Code is read much more than it's written!\n";
    cout << "   Optimize for readability, not cleverness.\n";
    cout << "   \n";
    cout << "   \"Any fool can write code that a computer can understand.\n";
    cout << "    Good programmers write code that humans can understand.\"\n";
    cout << "   — Martin Fowler\n";
}

// ============================================
// EXAMPLE 7: WHEN COMPLEXITY IS JUSTIFIED
// ============================================

namespace example7 {
// Sometimes complexity IS needed!

// ❌ TOO SIMPLE (doesn't handle edge cases)
class SimpleCacheBad {
private:
    map<string, string> cache;

public:
    string get(const string& key) {
        return cache[key];  // 🚨 No size limit, grows forever!
    }

    void put(const string& key, const string& value) {
        cache[key] = value;  // 🚨 Memory leak waiting to happen
    }
};

// ✅ APPROPRIATE COMPLEXITY (handles requirements)
class LRUCache {
private:
    int capacity;
    map<string, string> cache;
    vector<string> accessOrder;

public:
    LRUCache(int cap) : capacity(cap) {}

    string get(const string& key) {
        if (cache.find(key) == cache.end()) {
            return "";
        }

        // Update access order
        accessOrder.erase(
            remove(accessOrder.begin(), accessOrder.end(), key),
            accessOrder.end()
            );
        accessOrder.push_back(key);

        return cache[key];
    }

    void put(const string& key, const string& value) {
        // Remove if exists
        if (cache.find(key) != cache.end()) {
            accessOrder.erase(
                remove(accessOrder.begin(), accessOrder.end(), key),
                accessOrder.end()
                );
        }

        // Evict least recently used if at capacity
        if (cache.size() >= capacity && cache.find(key) == cache.end()) {
            string oldestKey = accessOrder.front();
            cache.erase(oldestKey);
            accessOrder.erase(accessOrder.begin());
        }

        cache[key] = value;
        accessOrder.push_back(key);
    }
};
}

void example7_justified_complexity() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 7: WHEN COMPLEXITY IS JUSTIFIED        ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n⚖️  BALANCE: Simple vs Complete\n";

    cout << "\n❌ TOO SIMPLE:\n";
    cout << "   Simple cache with no size limit\n";
    cout << "   • Easy to understand ✅\n";
    cout << "   • But grows forever 🚨\n";
    cout << "   • Memory leak 🚨\n";

    cout << "\n✅ APPROPRIATELY COMPLEX:\n";
    cout << "   LRU Cache with eviction\n";
    cout << "   • More complex ⚠️\n";
    cout << "   • But handles real requirements ✅\n";
    cout << "   • Prevents memory issues ✅\n";

    cout << "\n💡 WHEN COMPLEXITY IS JUSTIFIED:\n";
    cout << "   ✅ Solving real, documented requirements\n";
    cout << "   ✅ Handling necessary edge cases\n";
    cout << "   ✅ Performance critical code\n";
    cout << "   ✅ Security requirements\n";
    cout << "   ✅ Scalability needs\n";

    cout << "\n❌ WHEN COMPLEXITY IS NOT JUSTIFIED:\n";
    cout << "   🚨 \"Future-proofing\" for unknown needs\n";
    cout << "   🚨 Showing off programming skills\n";
    cout << "   🚨 Following patterns blindly\n";
    cout << "   🚨 Premature optimization\n";
    cout << "   🚨 \"Because it might be needed\"\n";

    cout << "\n✨ DEMONSTRATION:\n";

    example7::LRUCache cache(3);  // Max 3 items

    cout << "\n   Adding items to cache (capacity = 3):\n";
    cache.put("key1", "value1");
    cout << "   Added: key1" << endl;
    cache.put("key2", "value2");
    cout << "   Added: key2" << endl;
    cache.put("key3", "value3");
    cout << "   Added: key3" << endl;

    cout << "\n   Adding 4th item (will evict key1):\n";
    cache.put("key4", "value4");
    cout << "   Added: key4 (key1 evicted)" << endl;

    cout << "\n   Retrieving values:\n";
    cout << "   key1: " << (cache.get("key1").empty() ? "Not found" : cache.get("key1")) << endl;
    cout << "   key2: " << cache.get("key2") << endl;
}

// ============================================
// EXAMPLE 8: HOW TO KEEP IT SIMPLE
// ============================================

void example8_how_to_keep_simple() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 8: HOW TO KEEP IT SIMPLE              ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n✅ STRATEGIES FOR SIMPLICITY:\n";

    cout << "\n1. START SIMPLE, ADD COMPLEXITY ONLY WHEN NEEDED:\n";
    cout << "   • Begin with simplest solution\n";
    cout << "   • Add features when requirements appear\n";
    cout << "   • Don't anticipate future needs\n";

    cout << "\n2. AVOID PREMATURE ABSTRACTION:\n";
    cout << "   • Wait for patterns to emerge\n";
    cout << "   • Don't create abstractions \"just in case\"\n";
    cout << "   • YAGNI: You Aren't Gonna Need It\n";

    cout << "\n3. USE STANDARD LIBRARY:\n";
    cout << "   • Don't reinvent vector, map, string\n";
    cout << "   • Use proven, tested implementations\n";
    cout << "   • Focus on business logic\n";

    cout << "\n4. FAVOR CLARITY OVER CLEVERNESS:\n";
    cout << "   • Write code that's obvious\n";
    cout << "   • Avoid tricks and shortcuts\n";
    cout << "   • Optimize for reading, not writing\n";

    cout << "\n5. MINIMIZE DEPENDENCIES:\n";
    cout << "   • Fewer dependencies = simpler code\n";
    cout << "   • Each dependency adds complexity\n";
    cout << "   • Question each new dependency\n";

    cout << "\n6. AVOID DEEP NESTING:\n";
    cout << "   ❌ if (a) { if (b) { if (c) { ... }}}\n";
    cout << "   ✅ if (!a) return; if (!b) return; if (!c) return;\n";

    cout << "\n7. SMALL FUNCTIONS:\n";
    cout << "   • One function, one purpose\n";
    cout << "   • 5-15 lines ideal\n";
    cout << "   • Easy to understand and test\n";

    cout << "\n8. MEANINGFUL NAMES:\n";
    cout << "   ❌ int d; // elapsed time in days\n";
    cout << "   ✅ int elapsedTimeInDays;\n";

    cout << "\n🔍 SIMPLICITY CHECKLIST:\n";
    cout << "   □ Can I explain this in one sentence?\n";
    cout << "   □ Would a junior developer understand it?\n";
    cout << "   □ Is this the simplest solution that works?\n";
    cout << "   □ Am I solving a problem I actually have?\n";
    cout << "   □ Can I remove any abstraction layers?\n";
    cout << "   □ Am I using standard library where possible?\n";
    cout << "   □ Is my clever code actually clearer?\n";

    cout << "\n⚖️  THE SIMPLICITY BALANCE:\n";
    cout << "   Too Simple ←--- Sweet Spot ---→ Too Complex\n";
    cout << "   (Missing features)           (Over-engineered)\n";
    cout << "   \n";
    cout << "   Aim for: Simplest solution that meets requirements!\n";
}

// ============================================
// EXAMPLE 9: BENEFITS & SUMMARY
// ============================================

void example9_benefits() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 9: BENEFITS & SUMMARY                  ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n✅ BENEFITS OF KISS:\n";

    cout << "\n1. EASIER TO UNDERSTAND:\n";
    cout << "   • New team members onboard faster\n";
    cout << "   • Less time spent understanding code\n";
    cout << "   • Clearer intent and purpose\n";

    cout << "\n2. FEWER BUGS:\n";
    cout << "   • Less code = less surface for bugs\n";
    cout << "   • Simple logic easier to test\n";
    cout << "   • Edge cases more obvious\n";

    cout << "\n3. FASTER DEVELOPMENT:\n";
    cout << "   • Less time writing\n";
    cout << "   • Less time debugging\n";
    cout << "   • Quicker to modify\n";

    cout << "\n4. EASIER MAINTENANCE:\n";
    cout << "   • Quick to understand and fix\n";
    cout << "   • Changes less likely to break things\n";
    cout << "   • Lower cost of ownership\n";

    cout << "\n5. BETTER TESTABILITY:\n";
    cout << "   • Simple code easier to test\n";
    cout << "   • Fewer test cases needed\n";
    cout << "   • Tests easier to understand\n";

    cout << "\n⚠️  COST OF COMPLEXITY:\n";
    cout << "   • Takes longer to understand\n";
    cout << "   • More bugs hiding places\n";
    cout << "   • Harder to modify\n";
    cout << "   • Scares away contributors\n";
    cout << "   • Higher maintenance cost\n";

    cout << "\n💡 KEY INSIGHTS:\n";
    cout << "   \"Simplicity is the ultimate sophistication.\"\n";
    cout << "   — Leonardo da Vinci\n";
    cout << "   \n";
    cout << "   \"The art of programming is the art of organizing complexity.\"\n";
    cout << "   — Dijkstra\n";
    cout << "   \n";
    cout << "   \"Simple is better than complex.\n";
    cout << "    Complex is better than complicated.\"\n";
    cout << "   — Zen of Python\n";

    cout << "\n🎯 REMEMBER:\n";
    cout << "   • Start simple\n";
    cout << "   • Add complexity only when justified\n";
    cout << "   • Optimize for readability\n";
    cout << "   • Use standard library\n";
    cout << "   • Avoid premature optimization\n";
    cout << "   • YAGNI: You Aren't Gonna Need It\n";

    cout << "\n📊 SIMPLICITY METRICS:\n";
    cout << "   • Lines of code: Fewer is often better\n";
    cout << "   • Cyclomatic complexity: < 10 per function\n";
    cout << "   • Nesting depth: < 3 levels\n";
    cout << "   • Dependencies: Minimize\n";
    cout << "   • Abstraction layers: Only when needed\n";

    cout << "\n🔄 KISS & OTHER PRINCIPLES:\n";
    cout << "   KISS + DRY = Simple and non-repetitive\n";
    cout << "   KISS + YAGNI = Build what you need now\n";
    cout << "   KISS + SOLID = Well-designed and simple\n";

    cout << "\n⚠️  COMPLEXITY SMELLS:\n";
    cout << "   🚨 \"Let me explain how this works...\"\n";
    cout << "   🚨 Need to draw diagrams to explain\n";
    cout << "   🚨 Lots of abstraction layers\n";
    cout << "   🚨 Design patterns used everywhere\n";
    cout << "   🚨 Code review takes hours\n";
    cout << "   🚨 \"It's clever but...\"\n";
}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║         KISS PRINCIPLE (Keep It Simple, Stupid)       ║\n";
    cout << "║                                                       ║\n";
    cout << "║  \"Simplicity should be a key goal in design, and      ║\n";
    cout << "║   unnecessary complexity should be avoided.\"          ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n";

    example1_conditionals();
    example2_calculator();
    example3_boolean();
    example4_abstraction();
    example5_data_structures();
    example6_clever_vs_clear();
    example7_justified_complexity();
    example8_how_to_keep_simple();
    example9_benefits();

    cout << "\n\n";
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║            🎉 KISS PRINCIPLE COMPLETE! 🎉             ║\n";
    cout << "║                                                       ║\n";
    cout << "║  Key Takeaway: Keep it simple! Favor clarity over     ║\n";
    cout << "║  cleverness, avoid unnecessary complexity, and use    ║\n";
    cout << "║  the simplest solution that solves the problem.       ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n\n";

    return 0;
}
