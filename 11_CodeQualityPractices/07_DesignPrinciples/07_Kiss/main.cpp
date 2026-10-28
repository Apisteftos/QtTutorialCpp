#include <vector>
#include <string>
#include <memory>
#include <map>
#include <algorithm>
#include <functional>
#include <print>

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
    bool checkEmail(const std::string& email) { return email.find("@") != std::string::npos; }
    bool checkName(const std::string& name) { return !name.empty() && name.length() >= 2; }

public:
    bool validate(int age, const std::string& email, const std::string& name) {
        // 🚨 Unnecessarily complex validation chain
        std::function<bool()> ageValidator = [this, age]() { return checkAge(age); };
        std::function<bool()> emailValidator = [this, &email]() { return checkEmail(email); };
        std::function<bool()> nameValidator = [this, &name]() { return checkName(name); };

        std::vector<std::function<bool()>> validators = {ageValidator, emailValidator, nameValidator};

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

bool isValidEmail(const std::string& email) {
    return email.find("@") != std::string::npos;
}

bool isValidName(const std::string& name) {
    return !name.empty() && name.length() >= 2;
}

bool validateUser(int age, const std::string& email, const std::string& name) {
    return isValidAge(age) &&
           isValidEmail(email) &&
           isValidName(name);
}
}

void example1_conditionals() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 1: SIMPLE CONDITIONALS                ║");
    std::println("╚══════════════════════════════════════════════════╝");

    bool valid = good_example1::validateUser(25, "user@example.com", "John");
    std::println("   Result: {}", (valid ? "✅ Valid" : "❌ Invalid"));

    std::println("");
    std::println("   Invalid user (too young):");
    valid = good_example1::validateUser(16, "user@example.com", "John");
    std::println("   Result: {}", (valid ? "✅ Valid" : "❌ Invalid"));

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
    virtual std::string getName() const = 0;
};

class AddOperation : public Operation {
public:
    double execute(double a, double b) override { return a + b; }
    std::string getName() const override { return "add"; }
};

class SubtractOperation : public Operation {
public:
    double execute(double a, double b) override { return a - b; }
    std::string getName() const override { return "subtract"; }
};

class MultiplyOperation : public Operation {
public:
    double execute(double a, double b) override { return a * b; }
    std::string getName() const override { return "multiply"; }
};

class DivideOperation : public Operation {
public:
    double execute(double a, double b) override { return a / b; }
    std::string getName() const override { return "divide"; }
};

class OperationFactory {
private:
    std::map<std::string, std::shared_ptr<Operation>> operations;

public:
    OperationFactory() {
        operations["add"] = std::make_shared<AddOperation>();
        operations["subtract"] = std::make_shared<SubtractOperation>();
        operations["multiply"] = std::make_shared<MultiplyOperation>();
        operations["divide"] = std::make_shared<DivideOperation>();
    }

    std::shared_ptr<Operation> getOperation(const std::string& name) {
        return operations[name];
    }
};

class Calculator {
private:
    OperationFactory factory;

public:
    double calculate(const std::string& operation, double a, double b) {
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

double calculate(const std::string& operation, double a, double b) {
    if (operation == "add") return add(a, b);
    if (operation == "subtract") return subtract(a, b);
    if (operation == "multiply") return multiply(a, b);
    if (operation == "divide") return divide(a, b);
    return 0;
}
}

void example2_calculator() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 2: OVER-ENGINEERED CALCULATOR         ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   10 + 5 = {}", good_example2::calculate("add", 10, 5));
    std::println("   10 - 5 = {}", good_example2::calculate("subtract", 10, 5));
    std::println("   10 * 5 = {}", good_example2::calculate("multiply", 10, 5));
    std::println("   10 / 5 = {}", good_example2::calculate("divide", 10, 5));

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
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 3: BOOLEAN SIMPLIFICATION             ║");
    std::println("╚══════════════════════════════════════════════════╝");

    bool result;

    std::println("");
    std::println("   Public resource, not logged in:");
    result = good_example3::canAccessResource(false, false, false, true);
    std::println("   Access: {}", (result ? "✅ Granted" : "❌ Denied"));

    std::println("");
    std::println("   Private resource, logged in, has permission:");
    result = good_example3::canAccessResource(true, true, false, false);
    std::println("   Access: {}", (result ? "✅ Granted" : "❌ Denied"));

    std::println("");
    std::println("   Private resource, logged in as admin:");
    result = good_example3::canAccessResource(true, false, true, false);
    std::println("   Access: {}", (result ? "✅ Granted" : "❌ Denied"));

}

// ============================================
// EXAMPLE 4: UNNECESSARY ABSTRACTION LAYERS
// ============================================

namespace bad_example4 {
// ❌ BAD: Too many abstraction layers

class DataAccessInterface {
public:
    virtual ~DataAccessInterface() = default;
    virtual std::string getData() = 0;
};

class DataAccessImplementation : public DataAccessInterface {
public:
    std::string getData() override { return "data"; }
};

class DataAccessProxy : public DataAccessInterface {
private:
    std::shared_ptr<DataAccessInterface> implementation;
public:
    DataAccessProxy(std::shared_ptr<DataAccessInterface> impl) : implementation(impl) {}
    std::string getData() override { return implementation->getData(); }
};

class DataAccessDecorator : public DataAccessInterface {
private:
    std::shared_ptr<DataAccessInterface> wrapped;
public:
    DataAccessDecorator(std::shared_ptr<DataAccessInterface> w) : wrapped(w) {}
    std::string getData() override { return wrapped->getData(); }
};

class DataService {
private:
    std::shared_ptr<DataAccessInterface> dataAccess;
public:
    DataService(std::shared_ptr<DataAccessInterface> da) : dataAccess(da) {}
    std::string retrieveData() { return dataAccess->getData(); }
};

// 🚨 PROBLEM: 5 classes to return "data"!
}

namespace good_example4 {
// ✅ GOOD: Direct and simple

class DataService {
public:
    std::string getData() {
        return "data";
    }
};
}

void example4_abstraction() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 4: UNNECESSARY ABSTRACTION LAYERS      ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   ");
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
    std::shared_ptr<Node<T>> next;
    std::weak_ptr<Node<T>> prev;

    Node(T d) : data(d) {}
};

template<typename T>
class List {
private:
    std::shared_ptr<Node<T>> head;
    std::shared_ptr<Node<T>> tail;
    int size;

public:
    List() : size(0) {}

    void add(T item) {
        auto node = std::make_shared<Node<T>>(item);
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
            std::println("   • {}", current->data);
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
    std::vector<T> items;

public:
    void add(T item) {
        items.push_back(item);
    }

    void print() {
        for (const auto& item : items) {
            std::println("   • {}", item);
        }
    }

    int size() const { return items.size(); }
};
}

void example5_data_structures() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 5: SIMPLE DATA STRUCTURES              ║");
    std::println("╚══════════════════════════════════════════════════╝");

    good_example5::List<std::string> list;
    list.add("Apple");
    list.add("Banana");
    list.add("Cherry");

    std::println("");
    std::println("   Items in list:");
    list.print();

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
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 6: CLEVER CODE VS CLEAR CODE           ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   Is 4 even? {}", (good_example6::isEven(4) ? "Yes" : "No"));
    std::println("   Is 7 even? {}", (good_example6::isEven(7) ? "Yes" : "No"));

    int a = 10, b = 20;
    std::println("");
    std::println("   Before swap: a = {}, b = {}", a, b);
    good_example6::swap(a, b);
    std::println("   After swap:  a = {}, b = {}", a, b);

    std::println("");
    std::println("   Factorial of 5: {}", good_example6::factorial(5));

    std::println("   ");
}

// ============================================
// EXAMPLE 7: WHEN COMPLEXITY IS JUSTIFIED
// ============================================

namespace example7 {
// Sometimes complexity IS needed!

// ❌ TOO SIMPLE (doesn't handle edge cases)
class SimpleCacheBad {
private:
    std::map<std::string, std::string> cache;

public:
    std::string get(const std::string& key) {
        return cache[key];  // 🚨 No size limit, grows forever!
    }

    void put(const std::string& key, const std::string& value) {
        cache[key] = value;  // 🚨 Memory leak waiting to happen
    }
};

// ✅ APPROPRIATE COMPLEXITY (handles requirements)
class LRUCache {
private:
    int capacity;
    std::map<std::string, std::string> cache;
    std::vector<std::string> accessOrder;

public:
    LRUCache(int cap) : capacity(cap) {}

    std::string get(const std::string& key) {
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

    void put(const std::string& key, const std::string& value) {
        // Remove if exists
        if (cache.find(key) != cache.end()) {
            accessOrder.erase(
                remove(accessOrder.begin(), accessOrder.end(), key),
                accessOrder.end()
                );
        }

        // Evict least recently used if at capacity
        if (cache.size() >= capacity && cache.find(key) == cache.end()) {
            std::string oldestKey = accessOrder.front();
            cache.erase(oldestKey);
            accessOrder.erase(accessOrder.begin());
        }

        cache[key] = value;
        accessOrder.push_back(key);
    }
};
}

void example7_justified_complexity() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 7: WHEN COMPLEXITY IS JUSTIFIED        ║");
    std::println("╚══════════════════════════════════════════════════╝");

    example7::LRUCache cache(3);  // Max 3 items

    std::println("");
    std::println("   Adding items to cache (capacity = 3):");
    cache.put("key1", "value1");
    std::println("   Added: key1");
    cache.put("key2", "value2");
    std::println("   Added: key2");
    cache.put("key3", "value3");
    std::println("   Added: key3");

    std::println("");
    std::println("   Adding 4th item (will evict key1):");
    cache.put("key4", "value4");
    std::println("   Added: key4 (key1 evicted)");

    std::println("");
    std::println("   Retrieving values:");
    std::println("   key1: {}", (cache.get("key1").empty() ? "Not found" : cache.get("key1")));
    std::println("   key2: {}", cache.get("key2"));
}

// ============================================
// EXAMPLE 8: HOW TO KEEP IT SIMPLE
// ============================================

void example8_how_to_keep_simple() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 8: HOW TO KEEP IT SIMPLE               ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   ");
}

// ============================================
// EXAMPLE 9: BENEFITS & SUMMARY
// ============================================

void example9_benefits() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 9: BENEFITS & SUMMARY                  ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   ");
    std::println("   ");

}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    std::println("╔═══════════════════════════════════════════════════════╗");
    std::println("║                                                       ║");
    std::println("║         KISS PRINCIPLE (Keep It Simple, Stupid)       ║");
    std::println("║                                                       ║");
    std::println("║  \"Simplicity should be a key goal in design, and     ║");
    std::println("║   unnecessary complexity should be avoided.\"         ║");
    std::println("║                                                       ║");
    std::println("╚═══════════════════════════════════════════════════════╝");

    example1_conditionals();
    example2_calculator();
    example3_boolean();
    example4_abstraction();
    example5_data_structures();
    example6_clever_vs_clear();
    example7_justified_complexity();
    example8_how_to_keep_simple();
    example9_benefits();

    return 0;
}
