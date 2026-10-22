#include <print>
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
            std::println("  [Using cache]");
            return cachedResult;
        }

        std::println("  [Computing...]");
        // Simulate expensive calculation
        int result = value * value * value;

        // Want to cache, but this prevents const!
        cachedResult = result;
        cached = true;

        return result;
    }
};

void demonstrateProblem() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: PROBLEM WITHOUT        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    ExpensiveCalculatorBad calc(5);

    std::println("--- First call ---");
    std::println("Result: {}", calc.compute());

    std::println("\n--- Second call ---");
    std::println("Result: {}", calc.compute());

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
            std::println("  [Using cache]");
            return cachedResult;
        }

        std::println("  [Computing...]");
        int result = value * value * value;

        // ✅ Can modify mutable members in const function
        cachedResult = result;
        cached = true;

        return result;
    }

    void display() const {
        std::println("  Value: {}", value);
    }
};

void demonstrateSolution() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: SOLUTION WITH MUTABLE  ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    const ExpensiveCalculatorGood calc(5);  // const object!

    std::println("--- First call (const object) ---");
    std::println("Result: {}", calc.compute());

    std::println("\n--- Second call (cached) ---");
    std::println("Result: {}", calc.compute());

    std::println("\n--- Third call (still cached) ---");
    std::println("Result: {}", calc.compute());

    calc.display();
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
        std::println("  Title: {}", title);
        std::println("  Content: {}", content);
        std::println("  Access count: {}", accessCount);
    }
};

void demonstrateAccessCounter() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: ACCESS COUNTER         ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    const Document doc("Report", "Important data...");

    std::println("--- Accessing document ---");
    std::println("Title: {}", doc.getTitle());
    std::println("Content: {}", doc.getContent());
    std::println("Title again: {}", doc.getTitle());

    std::println("\n--- Statistics ---");
    doc.display();
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
            std::println("  [Initializing expensive resource...]");
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
        std::println("  HeavyResource created (data not loaded yet)");
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
        std::println("  Resource: {}", name);
        std::println("  Initialized: {}", (initialized ? "Yes" : "No"));
    }
};

void demonstrateLazyInit() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: LAZY INITIALIZATION    ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating resource (not initialized) ---");
    const HeavyResource resource("DataSet");
    resource.display();

    std::println("\n--- First access (triggers initialization) ---");
    int size = resource.getDataSize();
    std::println("Size: {}", size);
    resource.display();

    std::println("\n--- Second access (already initialized) ---");
    const vector<int>& data = resource.getData();
    std::println("Data count: {}", data.size());
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
        std::println("  Value: {}", value);
    }
};

void demonstrateMutex() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 5: MUTEX (THREAD SAFETY)  ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    const ThreadSafeCounter counter(0);

    std::println("--- Reading from const object ---");
    counter.display();
    std::println("Value: {}", counter.getValue());
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
        std::println("  Data: {}", data);
        std::println("  Access count: {}", accessCount);
        std::print("  Last accessed: {}", ctime(&lastAccessed));
    }
};

void demonstrateTimestamp() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 6: TIMESTAMP/LOGGING      ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    const AuditedData data("Sensitive Information");

    std::println("--- First access ---");
    std::println("Data: {}", data.getData());

    std::println("\n--- Second access ---");
    std::println("Data: {}", data.getData());

    std::println("\n--- Third access ---");
    std::println("Data: {}", data.getData());

    std::println("\n--- Audit trail ---");
    data.displayAudit();
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
            std::println("  [Using cached distance]");
            return cachedDistance;
        }

        std::println("  [Computing distance...]");
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
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 7: CACHE INVALIDATION     ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    ComplexCalculator calc(3.0, 4.0);

    std::println("--- First distance calculation ---");
    std::println("Distance: {}", calc.distance());

    std::println("\n--- Second call (cached) ---");
    std::println("Distance: {}", calc.distance());

    std::println("\n--- Modifying X (invalidates cache) ---");
    calc.setX(5.0);

    std::println("\n--- Third call (recomputes) ---");
    std::println("Distance: {}", calc.distance());

    std::println("\n--- Fourth call (cached again) ---");
    std::println("Distance: {}", calc.distance());
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
        std::println("  Data: {}", data);
        std::println("  process() calls: {}", processCallCount);
        std::println("  getLength() calls: {}", lengthCallCount);
    }
};

void demonstrateDebugStats() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 8: DEBUG/STATISTICS       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    const StringProcessor processor("hello world");

    std::println("--- Using processor ---");
    std::println("Processed: {}", processor.process());
    std::println("Length: {}", processor.getLength());
    std::println("Processed again: {}", processor.process());
    std::println("Length again: {}", processor.getLength());

    std::println("\n--- Statistics ---");
    processor.displayStats();
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
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 9: WHEN NOT TO USE        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    BankAccountGood account("Anna", 100.0);

    std::println("--- Balance is business state (not mutable) ---");
    account.deposit(50.0);   // non-const function changes the balance
    std::println("Balance: {}", account.getBalance());   // const function, only the counter changes
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
        std::println("  Image loaded: {}", filename);
    }

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    // Expensive calculation - cached
    double aspectRatio() const {
        if (aspectRatioCached) {
            std::println("  [Using cached aspect ratio]");
            return cachedAspectRatio;
        }

        std::println("  [Computing aspect ratio...]");
        cachedAspectRatio = static_cast<double>(width) / height;
        aspectRatioCached = true;
        return cachedAspectRatio;
    }

    // Another expensive calculation - cached
    int area() const {
        if (areaCached) {
            std::println("  [Using cached area]");
            return cachedArea;
        }

        std::println("  [Computing area...]");
        cachedArea = width * height;
        areaCached = true;
        return cachedArea;
    }

    void display() const {
        std::println("  Image: {}", filename);
        std::println("  Dimensions: {}x{}", width, height);
        std::println("  Aspect ratio: {}", aspectRatio());
        std::println("  Area: {}", area());
    }
};

void demonstrateCachedProperties() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 10: CACHED PROPERTIES     ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    const Image img("photo.jpg", 1920, 1080);

    std::println("\n--- First display ---");
    img.display();

    std::println("\n--- Second display (all cached) ---");
    img.display();

    std::println("\n--- Individual access (cached) ---");
    std::println("Aspect ratio: {}", img.aspectRatio());
    std::println("Area: {}", img.area());
}

// ============================================
// MAIN FUNCTION - RUN ALL EXAMPLES
// ============================================

int main() {
    std::println();
    std::println("╔═══════════════════════════════════════════════════════════╗");
    std::println("║                                                           ║");
    std::println("║              MUTABLE KEYWORD IN C++                       ║");
    std::println("║              Logical vs Physical Constness                ║");
    std::println("║                                                           ║");
    std::println("╚═══════════════════════════════════════════════════════════╝");

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

    return 0;
}
