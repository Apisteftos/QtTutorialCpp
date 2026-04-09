#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <ctime>
#include <cmath>
using namespace std;

// ============================================
// WHAT IS THE MUTABLE KEYWORD?
// ============================================

/*
 * MUTABLE KEYWORD:
 * - Allows modification of member variables in const functions
 * - Applied to member variables, not functions
 * - Breaks the "const" promise for specific members
 * - Used for implementation details, not business logic
 *
 * WHY MUTABLE?
 * - Caching mechanisms (lazy evaluation)
 * - Debug counters and statistics
 * - Mutable state in const objects
 * - Thread synchronization (mutex)
 * - Performance optimization
 *
 * SYNTAX:
 * - mutable Type memberName;
 * - Can be modified in const member functions
 * - Can be modified in const objects (indirectly)
 *
 * COMMON USES:
 * - Cache storage (mutable cache)
 * - Access counters (mutable int counter)
 * - Mutex locks (mutable mutex m)
 * - Lazy initialization (mutable bool initialized)
 * - Performance metrics (mutable statistics)
 *
 * BEST PRACTICES:
 * - Use only for implementation details
 * - Not for business logic state
 * - Document why member is mutable
 * - Keep mutable members private
 * - Use sparingly and carefully
 *
 * LOGICAL VS PHYSICAL CONSTNESS:
 * - Physical: Actual bits don't change
 * - Logical: Object appears unchanged to users
 * - Mutable enables logical constness
 * - Cache doesn't affect logical state
 */

// ============================================
// EXAMPLE 1: THE PROBLEM WITHOUT MUTABLE
// ============================================

class ExpensiveCalculatorBad {
private:
    int value;
    // Want to cache, but can't in const function!
    bool cached;
    int cachedResult;

public:
    ExpensiveCalculatorBad(int v)
        : value(v), cached(false), cachedResult(0) {}

    // ❌ Can't be const because it modifies cache
    int compute() {  // Should be const, but can't!
        if (cached) {
            cout << "  [Using cache]\n";
            return cachedResult;
        }

        cout << "  [Computing...]\n";
        // Simulate expensive calculation
        int result = value * value * value;

        // Want to cache, but this prevents const!
        cachedResult = result;
        cached = true;

        return result;
    }
};

void demonstrateProblem() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: PROBLEM WITHOUT        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    ExpensiveCalculatorBad calc(5);

    cout << "--- First call ---\n";
    cout << "Result: " << calc.compute() << "\n";

    cout << "\n--- Second call ---\n";
    cout << "Result: " << calc.compute() << "\n";

    cout << "\n⚠️  THE PROBLEM:\n";
    cout << "   • compute() can't be const\n";
    cout << "   • Can't use with const objects\n";
    cout << "   • Cache is implementation detail\n";
    cout << "   • Shouldn't affect const-ness!\n";

    // const ExpensiveCalculatorBad constCalc(10);
    // constCalc.compute();  // ❌ ERROR - not const!
}

// ============================================
// EXAMPLE 2: SOLUTION WITH MUTABLE
// ============================================

class ExpensiveCalculatorGood {
private:
    int value;
    mutable bool cached;        // mutable = can modify in const!
    mutable int cachedResult;

public:
    ExpensiveCalculatorGood(int v)
        : value(v), cached(false), cachedResult(0) {}

    // ✅ Can be const now!
    int compute() const {
        if (cached) {
            cout << "  [Using cache]\n";
            return cachedResult;
        }

        cout << "  [Computing...]\n";
        int result = value * value * value;

        // ✅ Can modify mutable members in const function
        cachedResult = result;
        cached = true;

        return result;
    }

    void display() const {
        cout << "  Value: " << value << "\n";
    }
};

void demonstrateSolution() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: SOLUTION WITH MUTABLE  ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    const ExpensiveCalculatorGood calc(5);  // const object!

    cout << "--- First call (const object) ---\n";
    cout << "Result: " << calc.compute() << "\n";

    cout << "\n--- Second call (cached) ---\n";
    cout << "Result: " << calc.compute() << "\n";

    cout << "\n--- Third call (still cached) ---\n";
    cout << "Result: " << calc.compute() << "\n";

    calc.display();

    cout << "\n✅ THE SOLUTION:\n";
    cout << "   • compute() is const\n";
    cout << "   • Works with const objects\n";
    cout << "   • Cache is mutable\n";
    cout << "   • Logical constness preserved!\n";
}

// ============================================
// EXAMPLE 3: ACCESS COUNTER
// ============================================

class Document {
private:
    string title;
    string content;
    mutable int accessCount;  // Track how many times accessed

public:
    Document(const string& t, const string& c)
        : title(t), content(c), accessCount(0) {}

    string getTitle() const {
        accessCount++;  // ✅ Can modify mutable member
        return title;
    }

    string getContent() const {
        accessCount++;  // ✅ Can modify mutable member
        return content;
    }

    int getAccessCount() const {
        return accessCount;
    }

    void display() const {
        cout << "  Title: " << title << "\n";
        cout << "  Content: " << content << "\n";
        cout << "  Access count: " << accessCount << "\n";
    }
};

void demonstrateAccessCounter() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: ACCESS COUNTER         ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    const Document doc("Report", "Important data...");

    cout << "--- Accessing document ---\n";
    cout << "Title: " << doc.getTitle() << "\n";
    cout << "Content: " << doc.getContent() << "\n";
    cout << "Title again: " << doc.getTitle() << "\n";

    cout << "\n--- Statistics ---\n";
    doc.display();

    cout << "\n💡 USE CASE:\n";
    cout << "   • Track access statistics\n";
    cout << "   • Doesn't change logical state\n";
    cout << "   • Implementation detail only\n";
    cout << "   • Useful for analytics/debugging\n";
}

// ============================================
// EXAMPLE 4: LAZY INITIALIZATION
// ============================================

class HeavyResource {
private:
    string name;
    mutable bool initialized;
    mutable vector<int> expensiveData;  // Loaded on demand

    void initialize() const {
        if (!initialized) {
            cout << "  [Initializing expensive resource...]\n";
            // Simulate expensive initialization
            for (int i = 0; i < 10; ++i) {
                expensiveData.push_back(i * i);
            }
            initialized = true;
        }
    }

public:
    HeavyResource(const string& n)
        : name(n), initialized(false) {
        cout << "  HeavyResource created (data not loaded yet)\n";
    }

    // Lazy loading - initialize only when needed
    const vector<int>& getData() const {
        initialize();  // ✅ Can call in const function
        return expensiveData;
    }

    int getDataSize() const {
        initialize();  // ✅ Initialize if needed
        return expensiveData.size();
    }

    void display() const {
        cout << "  Resource: " << name << "\n";
        cout << "  Initialized: " << (initialized ? "Yes" : "No") << "\n";
    }
};

void demonstrateLazyInit() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: LAZY INITIALIZATION    ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating resource (not initialized) ---\n";
    const HeavyResource resource("DataSet");
    resource.display();

    cout << "\n--- First access (triggers initialization) ---\n";
    int size = resource.getDataSize();
    cout << "Size: " << size << "\n";
    resource.display();

    cout << "\n--- Second access (already initialized) ---\n";
    const vector<int>& data = resource.getData();
    cout << "Data count: " << data.size() << "\n";

    cout << "\n💡 USE CASE:\n";
    cout << "   • Delay expensive operations\n";
    cout << "   • Initialize only when needed\n";
    cout << "   • Optimize performance\n";
    cout << "   • Transparent to users\n";
}

// ============================================
// EXAMPLE 5: MUTEX FOR THREAD SAFETY
// ============================================

class ThreadSafeCounter {
private:
    int value;
    mutable mutex mtx;  // Mutex must be mutable!

public:
    ThreadSafeCounter(int v) : value(v) {}

    // const function but needs to lock mutex
    int getValue() const {
        lock_guard<mutex> lock(mtx);  // ✅ Can lock mutable mutex
        return value;
    }

    void increment() {
        lock_guard<mutex> lock(mtx);
        value++;
    }

    void display() const {
        lock_guard<mutex> lock(mtx);  // ✅ Can lock in const function
        cout << "  Value: " << value << "\n";
    }
};

void demonstrateMutex() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: MUTEX (THREAD SAFETY)  ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    const ThreadSafeCounter counter(0);

    cout << "--- Reading from const object ---\n";
    counter.display();
    cout << "Value: " << counter.getValue() << "\n";

    cout << "\n💡 USE CASE:\n";
    cout << "   • Thread synchronization\n";
    cout << "   • Mutex must be mutable\n";
    cout << "   • Locking in const functions\n";
    cout << "   • Essential for thread safety\n";

    cout << "\n⚠️  CRITICAL:\n";
    cout << "   • Without mutable, can't lock in const functions\n";
    cout << "   • Mutex is implementation detail\n";
    cout << "   • Doesn't affect logical state\n";
}

// ============================================
// EXAMPLE 6: TIMESTAMP/LOGGING
// ============================================

class AuditedData {
private:
    string data;
    mutable time_t lastAccessed;
    mutable int accessCount;

    void logAccess() const {
        lastAccessed = time(nullptr);
        accessCount++;
    }

public:
    AuditedData(const string& d)
        : data(d), lastAccessed(0), accessCount(0) {}

    string getData() const {
        logAccess();  // ✅ Can call in const function
        return data;
    }

    void displayAudit() const {
        cout << "  Data: " << data << "\n";
        cout << "  Access count: " << accessCount << "\n";
        cout << "  Last accessed: " << ctime(&lastAccessed);
    }
};

void demonstrateTimestamp() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: TIMESTAMP/LOGGING      ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    const AuditedData data("Sensitive Information");

    cout << "--- First access ---\n";
    cout << "Data: " << data.getData() << "\n";

    cout << "\n--- Second access ---\n";
    cout << "Data: " << data.getData() << "\n";

    cout << "\n--- Third access ---\n";
    cout << "Data: " << data.getData() << "\n";

    cout << "\n--- Audit trail ---\n";
    data.displayAudit();

    cout << "\n💡 USE CASE:\n";
    cout << "   • Audit trails\n";
    cout << "   • Logging access\n";
    cout << "   • Timestamp tracking\n";
    cout << "   • Debug information\n";
}

// ============================================
// EXAMPLE 7: CACHING WITH INVALIDATION
// ============================================

class ComplexCalculator {
private:
    double x, y;
    mutable bool distanceCached;
    mutable double cachedDistance;

    void invalidateCache() {
        distanceCached = false;
    }

public:
    ComplexCalculator(double x, double y)
        : x(x), y(y), distanceCached(false), cachedDistance(0) {}

    double getX() const { return x; }
    double getY() const { return y; }

    // Expensive calculation with caching
    double distance() const {
        if (distanceCached) {
            cout << "  [Using cached distance]\n";
            return cachedDistance;
        }

        cout << "  [Computing distance...]\n";
        cachedDistance = sqrt(x*x + y*y);
        distanceCached = true;
        return cachedDistance;
    }

    // Modifying functions invalidate cache
    void setX(double newX) {
        x = newX;
        invalidateCache();
    }

    void setY(double newY) {
        y = newY;
        invalidateCache();
    }
};

void demonstrateCacheInvalidation() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 7: CACHE INVALIDATION     ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    ComplexCalculator calc(3.0, 4.0);

    cout << "--- First distance calculation ---\n";
    cout << "Distance: " << calc.distance() << "\n";

    cout << "\n--- Second call (cached) ---\n";
    cout << "Distance: " << calc.distance() << "\n";

    cout << "\n--- Modifying X (invalidates cache) ---\n";
    calc.setX(5.0);

    cout << "\n--- Third call (recomputes) ---\n";
    cout << "Distance: " << calc.distance() << "\n";

    cout << "\n--- Fourth call (cached again) ---\n";
    cout << "Distance: " << calc.distance() << "\n";

    cout << "\n💡 CACHE INVALIDATION:\n";
    cout << "   • Cache valid until state changes\n";
    cout << "   • Modifications invalidate cache\n";
    cout << "   • Next access recomputes\n";
    cout << "   • Maintains correctness\n";
}

// ============================================
// EXAMPLE 8: DEBUG/STATISTICS MODE
// ============================================

class StringProcessor {
private:
    string data;
    mutable int processCallCount;
    mutable int lengthCallCount;

public:
    StringProcessor(const string& d)
        : data(d), processCallCount(0), lengthCallCount(0) {}

    string process() const {
        processCallCount++;  // ✅ Track statistics
        string result = data;
        // Simulate processing
        for (char& c : result) {
            c = toupper(c);
        }
        return result;
    }

    int getLength() const {
        lengthCallCount++;  // ✅ Track statistics
        return data.length();
    }

    void displayStats() const {
        cout << "  Data: " << data << "\n";
        cout << "  process() calls: " << processCallCount << "\n";
        cout << "  getLength() calls: " << lengthCallCount << "\n";
    }
};

void demonstrateDebugStats() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 8: DEBUG/STATISTICS       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    const StringProcessor processor("hello world");

    cout << "--- Using processor ---\n";
    cout << "Processed: " << processor.process() << "\n";
    cout << "Length: " << processor.getLength() << "\n";
    cout << "Processed again: " << processor.process() << "\n";
    cout << "Length again: " << processor.getLength() << "\n";

    cout << "\n--- Statistics ---\n";
    processor.displayStats();

    cout << "\n💡 USE CASE:\n";
    cout << "   • Performance profiling\n";
    cout << "   • Usage statistics\n";
    cout << "   • Debug information\n";
    cout << "   • Hot path identification\n";
}

// ============================================
// EXAMPLE 9: WHEN NOT TO USE MUTABLE
// ============================================

// ❌ BAD: Using mutable for business logic
class BankAccountBad {
private:
    string owner;
    mutable double balance;  // ❌ BAD! Balance is business logic!

public:
    BankAccountBad(const string& o, double b) : owner(o), balance(b) {}

    double getBalance() const {
        return balance;
    }

    // ❌ BAD: Modifying business state in const function!
    void cheat() const {
        balance += 1000000;  // This is wrong!
    }
};

// ✅ GOOD: Don't use mutable for business logic
class BankAccountGood {
private:
    string owner;
    double balance;  // ✅ GOOD! Not mutable
    mutable int accessCount;  // ✅ OK for statistics

public:
    BankAccountGood(const string& o, double b)
        : owner(o), balance(b), accessCount(0) {}

    double getBalance() const {
        accessCount++;  // ✅ OK - just statistics
        return balance;
    }

    // ✅ GOOD: Non-const function for modification
    void deposit(double amount) {
        balance += amount;
    }
};

void demonstrateWhenNotToUse() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 9: WHEN NOT TO USE        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "❌ BAD USES OF MUTABLE:\n";
    cout << "   • Business logic state\n";
    cout << "   • Core object data\n";
    cout << "   • User-visible state\n";
    cout << "   • Making non-const functions \"work\"\n";
    cout << "   • Avoiding proper design\n";
    cout << "\n";
    cout << "✅ GOOD USES OF MUTABLE:\n";
    cout << "   • Caching computed values\n";
    cout << "   • Access counters\n";
    cout << "   • Debug statistics\n";
    cout << "   • Mutex for thread safety\n";
    cout << "   • Lazy initialization\n";
    cout << "   • Timestamp tracking\n";
    cout << "\n";
    cout << "GOLDEN RULE:\n";
    cout << "   If users care about it → NOT mutable!\n";
    cout << "   If it's implementation detail → mutable OK\n";
}

// ============================================
// EXAMPLE 10: REAL-WORLD - CACHED PROPERTIES
// ============================================

class Image {
private:
    string filename;
    int width, height;

    // Cached computed properties
    mutable bool aspectRatioCached;
    mutable double cachedAspectRatio;

    mutable bool areaCached;
    mutable int cachedArea;

public:
    Image(const string& f, int w, int h)
        : filename(f), width(w), height(h),
        aspectRatioCached(false), areaCached(false) {
        cout << "  Image loaded: " << filename << "\n";
    }

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    // Expensive calculation - cached
    double aspectRatio() const {
        if (aspectRatioCached) {
            cout << "  [Using cached aspect ratio]\n";
            return cachedAspectRatio;
        }

        cout << "  [Computing aspect ratio...]\n";
        cachedAspectRatio = static_cast<double>(width) / height;
        aspectRatioCached = true;
        return cachedAspectRatio;
    }

    // Another expensive calculation - cached
    int area() const {
        if (areaCached) {
            cout << "  [Using cached area]\n";
            return cachedArea;
        }

        cout << "  [Computing area...]\n";
        cachedArea = width * height;
        areaCached = true;
        return cachedArea;
    }

    void display() const {
        cout << "  Image: " << filename << "\n";
        cout << "  Dimensions: " << width << "x" << height << "\n";
        cout << "  Aspect ratio: " << aspectRatio() << "\n";
        cout << "  Area: " << area() << "\n";
    }
};

void demonstrateCachedProperties() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 10: CACHED PROPERTIES     ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    const Image img("photo.jpg", 1920, 1080);

    cout << "\n--- First display ---\n";
    img.display();

    cout << "\n--- Second display (all cached) ---\n";
    img.display();

    cout << "\n--- Individual access (cached) ---\n";
    cout << "Aspect ratio: " << img.aspectRatio() << "\n";
    cout << "Area: " << img.area() << "\n";

    cout << "\n💡 REAL-WORLD PATTERN:\n";
    cout << "   • Cache expensive computations\n";
    cout << "   • Transparent to users\n";
    cout << "   • Improves performance\n";
    cout << "   • Maintains const correctness\n";
}

// ============================================
// MAIN FUNCTION - RUN ALL EXAMPLES
// ============================================

int main() {
    cout << "\n";
    cout << "╔═══════════════════════════════════════════════════════════╗\n";
    cout << "║                                                           ║\n";
    cout << "║              MUTABLE KEYWORD IN C++                      ║\n";
    cout << "║              Logical vs Physical Constness                ║\n";
    cout << "║                                                           ║\n";
    cout << "╚═══════════════════════════════════════════════════════════╝\n";

    demonstrateProblem();
    demonstrateSolution();
    demonstrateAccessCounter();
    demonstrateLazyInit();
    demonstrateMutex();
    demonstrateTimestamp();
    demonstrateCacheInvalidation();
    demonstrateDebugStats();
    demonstrateWhenNotToUse();
    demonstrateCachedProperties();

    // ============================================
    // COMPREHENSIVE EXPLANATION
    // ============================================

    cout << "\n\n";
    cout << "╔═══════════════════════════════════════════════════════════╗\n";
    cout << "║                                                           ║\n";
    cout << "║              COMPREHENSIVE EXPLANATION                    ║\n";
    cout << "║                                                           ║\n";
    cout << "╚═══════════════════════════════════════════════════════════╝\n";

    cout << "\n========================================\n";
    cout << "WHAT IS MUTABLE?\n";
    cout << "========================================\n";
    cout << "The mutable keyword:\n";
    cout << "  • Allows modification in const functions\n";
    cout << "  • Applied to member variables\n";
    cout << "  • Breaks const promise for specific members\n";
    cout << "  • Used for implementation details\n";
    cout << "\n";
    cout << "SYNTAX:\n";
    cout << "class MyClass {\n";
    cout << "    int data;              // Regular member\n";
    cout << "    mutable int counter;   // Mutable member\n";
    cout << "    \n";
    cout << "public:\n";
    cout << "    void func() const {\n";
    cout << "        // data = 10;      // ❌ ERROR\n";
    cout << "        counter = 10;      // ✅ OK - mutable\n";
    cout << "    }\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "WHY MUTABLE EXISTS\n";
    cout << "========================================\n";
    cout << "THE PROBLEM:\n";
    cout << "class Cache {\n";
    cout << "    int value;\n";
    cout << "    bool cached;\n";
    cout << "    int cachedResult;\n";
    cout << "    \n";
    cout << "public:\n";
    cout << "    int compute() {  // ❌ Can't be const!\n";
    cout << "        if (!cached) {\n";
    cout << "            cachedResult = expensiveCalc(value);\n";
    cout << "            cached = true;\n";
    cout << "        }\n";
    cout << "        return cachedResult;\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "• compute() modifies cache state\n";
    cout << "• Can't be const function\n";
    cout << "• Can't use with const objects\n";
    cout << "• But caching is implementation detail!\n";
    cout << "\n";
    cout << "THE SOLUTION:\n";
    cout << "class Cache {\n";
    cout << "    int value;\n";
    cout << "    mutable bool cached;  // ✅ Mutable!\n";
    cout << "    mutable int cachedResult;\n";
    cout << "    \n";
    cout << "public:\n";
    cout << "    int compute() const {  // ✅ Can be const now!\n";
    cout << "        if (!cached) {\n";
    cout << "            cachedResult = expensiveCalc(value);\n";
    cout << "            cached = true;\n";
    cout << "        }\n";
    cout << "        return cachedResult;\n";
    cout << "    }\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "LOGICAL VS PHYSICAL CONSTNESS\n";
    cout << "========================================\n";
    cout << "PHYSICAL CONSTNESS:\n";
    cout << "  • No bits in object change\n";
    cout << "  • Strict interpretation\n";
    cout << "  • What compiler enforces\n";
    cout << "\n";
    cout << "LOGICAL CONSTNESS:\n";
    cout << "  • Object appears unchanged to users\n";
    cout << "  • Observable state unchanged\n";
    cout << "  • What users care about\n";
    cout << "\n";
    cout << "EXAMPLE:\n";
    cout << "class Point {\n";
    cout << "    int x, y;\n";
    cout << "    mutable int accessCount;\n";
    cout << "    \n";
    cout << "public:\n";
    cout << "    int getX() const {\n";
    cout << "        accessCount++;  // Physical change\n";
    cout << "        return x;       // Logical const - x unchanged\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "• accessCount changes (physical)\n";
    cout << "• But point coordinates unchanged (logical)\n";
    cout << "• Users don't care about counter\n";
    cout << "• Mutable enables logical constness\n";

    cout << "\n========================================\n";
    cout << "COMMON USE CASES\n";
    cout << "========================================\n";
    cout << "1. CACHING:\n";
    cout << "   mutable bool cached;\n";
    cout << "   mutable ResultType cachedResult;\n";
    cout << "   • Store expensive computations\n";
    cout << "   • Transparent optimization\n";
    cout << "\n";
    cout << "2. ACCESS COUNTERS:\n";
    cout << "   mutable int accessCount;\n";
    cout << "   • Track usage statistics\n";
    cout << "   • Performance metrics\n";
    cout << "\n";
    cout << "3. LAZY INITIALIZATION:\n";
    cout << "   mutable bool initialized;\n";
    cout << "   mutable ResourceType resource;\n";
    cout << "   • Delay expensive creation\n";
    cout << "   • Initialize on first use\n";
    cout << "\n";
    cout << "4. MUTEX/LOCKS:\n";
    cout << "   mutable std::mutex mtx;\n";
    cout << "   • Thread synchronization\n";
    cout << "   • Lock in const functions\n";
    cout << "\n";
    cout << "5. TIMESTAMPS:\n";
    cout << "   mutable time_t lastAccessed;\n";
    cout << "   • Audit trails\n";
    cout << "   • Logging\n";
    cout << "\n";
    cout << "6. DEBUG INFO:\n";
    cout << "   mutable int callCount;\n";
    cout << "   • Profiling\n";
    cout << "   • Debugging\n";

    cout << "\n========================================\n";
    cout << "WHEN TO USE MUTABLE\n";
    cout << "========================================\n";
    cout << "✅ USE MUTABLE FOR:\n";
    cout << "  • Implementation details\n";
    cout << "  • Hidden from users\n";
    cout << "  • Performance optimizations\n";
    cout << "  • Thread synchronization\n";
    cout << "  • Debug/statistics\n";
    cout << "  • Lazy evaluation\n";
    cout << "\n";
    cout << "❌ DON'T USE MUTABLE FOR:\n";
    cout << "  • Business logic state\n";
    cout << "  • User-visible data\n";
    cout << "  • Core object properties\n";
    cout << "  • Making non-const work\n";
    cout << "  • Avoiding proper design\n";
    cout << "\n";
    cout << "DECISION GUIDE:\n";
    cout << "Ask: \"Do users care if this changes?\"\n";
    cout << "  • Yes → DON'T use mutable\n";
    cout << "  • No → mutable OK\n";
    cout << "\n";
    cout << "Ask: \"Does this affect observable behavior?\"\n";
    cout << "  • Yes → DON'T use mutable\n";
    cout << "  • No → mutable OK\n";

    cout << "\n========================================\n";
    cout << "BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "✅ DO:\n";
    cout << "  • Use for caching/optimization\n";
    cout << "  • Use for statistics/debugging\n";
    cout << "  • Use for mutex/synchronization\n";
    cout << "  • Keep mutable members private\n";
    cout << "  • Document WHY member is mutable\n";
    cout << "  • Use sparingly\n";
    cout << "  • Ensure thread safety\n";
    cout << "  • Test with const objects\n";
    cout << "\n";
    cout << "❌ DON'T:\n";
    cout << "  • Use for business logic\n";
    cout << "  • Make everything mutable\n";
    cout << "  • Use to bypass const\n";
    cout << "  • Expose mutable members publicly\n";
    cout << "  • Use without justification\n";
    cout << "  • Forget thread safety\n";
    cout << "  • Use for core object state\n";
    cout << "  • Abuse the feature\n";

    cout << "\n========================================\n";
    cout << "COMMON PATTERNS\n";
    cout << "========================================\n";
    cout << "PATTERN 1: Simple Cache\n";
    cout << "class Data {\n";
    cout << "    int value;\n";
    cout << "    mutable bool computed;\n";
    cout << "    mutable int result;\n";
    cout << "    \n";
    cout << "public:\n";
    cout << "    int compute() const {\n";
    cout << "        if (!computed) {\n";
    cout << "            result = expensiveFunc(value);\n";
    cout << "            computed = true;\n";
    cout << "        }\n";
    cout << "        return result;\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "PATTERN 2: Access Counter\n";
    cout << "class Object {\n";
    cout << "    mutable int accessCount = 0;\n";
    cout << "    \n";
    cout << "public:\n";
    cout << "    void access() const {\n";
    cout << "        accessCount++;\n";
    cout << "        // Do work...\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "PATTERN 3: Thread-Safe Access\n";
    cout << "class SharedData {\n";
    cout << "    int data;\n";
    cout << "    mutable std::mutex mtx;\n";
    cout << "    \n";
    cout << "public:\n";
    cout << "    int getData() const {\n";
    cout << "        std::lock_guard<std::mutex> lock(mtx);\n";
    cout << "        return data;\n";
    cout << "    }\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "MUTABLE AND THREADS\n";
    cout << "========================================\n";
    cout << "CRITICAL FOR THREAD SAFETY:\n";
    cout << "\n";
    cout << "class ThreadSafe {\n";
    cout << "    int value;\n";
    cout << "    mutable std::mutex mtx;  // Must be mutable!\n";
    cout << "    \n";
    cout << "public:\n";
    cout << "    int getValue() const {\n";
    cout << "        std::lock_guard<std::mutex> lock(mtx);\n";
    cout << "        return value;\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "WHY MUTEX IS MUTABLE:\n";
    cout << "  • Need to lock in const functions\n";
    cout << "  • Locking modifies mutex state\n";
    cout << "  • But doesn't change logical state\n";
    cout << "  • Synchronization is implementation detail\n";
    cout << "\n";
    cout << "WITHOUT MUTABLE:\n";
    cout << "  • Can't lock in const functions\n";
    cout << "  • No thread safety for const operations\n";
    cout << "  • Major design problem!\n";

    cout << "\n========================================\n";
    cout << "QUICK REFERENCE\n";
    cout << "========================================\n";
    cout << "// Declaring mutable member\n";
    cout << "class MyClass {\n";
    cout << "    int value;                  // Regular\n";
    cout << "    mutable int counter;        // Mutable\n";
    cout << "    mutable bool cached;        // Mutable\n";
    cout << "    mutable std::mutex mtx;     // Mutable\n";
    cout << "    \n";
    cout << "public:\n";
    cout << "    void func() const {\n";
    cout << "        // value = 10;          // ❌ ERROR\n";
    cout << "        counter = 10;           // ✅ OK\n";
    cout << "        cached = true;          // ✅ OK\n";
    cout << "        mtx.lock();             // ✅ OK\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Using with const objects\n";
    cout << "const MyClass obj;\n";
    cout << "obj.func();  // Modifies mutable members\n";

    cout << "\n========================================\n";
    cout << "KEY TAKEAWAYS\n";
    cout << "========================================\n";
    cout << "1. Mutable allows modification in const functions\n";
    cout << "2. Use only for implementation details\n";
    cout << "3. Not for business logic or user-visible state\n";
    cout << "4. Common uses: cache, counters, mutex\n";
    cout << "5. Enables logical constness\n";
    cout << "6. Essential for thread-safe const functions\n";
    cout << "7. Keep mutable members private\n";
    cout << "8. Use sparingly and with justification\n";
    cout << "9. Document why member is mutable\n";
    cout << "10. Test thoroughly with const objects\n";

    cout << "\n========================================\n";
    cout << "GOLDEN RULES\n";
    cout << "========================================\n";
    cout << "⚠️  RULE 1: Mutable for implementation ONLY\n";
    cout << "   Not for business logic!\n";
    cout << "   \n";
    cout << "   mutable int cacheHits;    // ✅ OK\n";
    cout << "   mutable double balance;   // ❌ BAD\n";
    cout << "\n";
    cout << "⚠️  RULE 2: Users shouldn't care\n";
    cout << "   If users care, don't use mutable!\n";
    cout << "   \n";
    cout << "   Ask: \"Does this affect observable behavior?\"\n";
    cout << "\n";
    cout << "⚠️  RULE 3: Document the reason\n";
    cout << "   Always explain WHY it's mutable!\n";
    cout << "   \n";
    cout << "   mutable int cache; // Cache for performance\n";
    cout << "\n";
    cout << "⚠️  RULE 4: Keep it private\n";
    cout << "   Never expose mutable members!\n";
    cout << "   \n";
    cout << "private:  // ✅ Always private\n";
    cout << "    mutable int counter;\n";
    cout << "\n";
    cout << "⚠️  RULE 5: Use sparingly\n";
    cout << "   Not everything should be mutable!\n";
    cout << "   \n";
    cout << "   Only when truly needed for const correctness\n";

    return 0;
}
