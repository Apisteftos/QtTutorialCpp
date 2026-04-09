#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <type_traits>
using namespace std;

// ============================================
// WHAT ARE VARIADIC TEMPLATES?
// ============================================

/*
PROBLEM:
    C functions like printf can take any number of arguments:
    printf("%d %s %f", 42, "hello", 3.14);

    BUT they're NOT type-safe! (runtime errors, undefined behavior)

SOLUTION - VARIADIC TEMPLATES:
    Templates that accept ANY NUMBER of arguments
    Type-safe, compile-time checked, zero overhead!

    template<typename... Args>
    void func(Args... args) {
        // Work with args...
    }

KEY CONCEPTS:
    1. Parameter Pack     - Args...      (type parameter pack)
    2. Pack Expansion     - args...      (expand the pack)
    3. sizeof... operator - sizeof...(Args)  (count pack elements)
    4. Fold Expressions   - (args + ...)     (C++17 operations)
    5. Recursion          - Base case + recursive case

SYNTAX:
    typename... Args   - Type parameter pack (can be 0 or more types)
    Args... args       - Function parameter pack
    sizeof...(Args)    - Number of types in pack
    sizeof...(args)    - Number of arguments in pack
    (args + ...)       - Fold expression (C++17)

GOLDEN RULE:
    Variadic templates = Type-safe printf with ZERO runtime cost!
*/

// ============================================
// EXAMPLE 1: The Problem - Unsafe va_args
// ============================================

namespace UnsafeVersion {

#include <cstdarg>

// Old C-style: NOT type-safe!
void print_unsafe(int count, ...) {
    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; ++i) {
        // ❌ Must manually specify type - easy to mess up!
        int value = va_arg(args, int);
        cout << value << " ";
    }

    va_end(args);
    cout << "\n";
}

void demonstrate() {
    cout << "\n=== Example 1: The Problem ===\n";
    cout << "Old C-style variadic functions:\n";

    cout << "  ";
    print_unsafe(3, 10, 20, 30);  // Works

    // ❌ These compile but cause UNDEFINED BEHAVIOR:
    // print_unsafe(2, 10, "hello");     // Wrong type!
    // print_unsafe(1, 10, 20, 30);      // Wrong count!

    cout << "\n❌ Problems:\n";
    cout << "  • Not type-safe\n";
    cout << "  • Must manually track count\n";
    cout << "  • Can't mix types safely\n";
    cout << "  • Undefined behavior if wrong\n";
    cout << "  • Runtime errors only\n";
}
}

// ============================================
// EXAMPLE 2: Basic Variadic Template
// ============================================

namespace BasicVariadic {

// Base case: no arguments
void print() {
    cout << "\n";
}

// Recursive case: peel off first argument
template<typename T, typename... Args>
void print(T first, Args... rest) {
    cout << first << " ";
    print(rest...);  // Recursively call with remaining arguments
}

void demonstrate() {
    cout << "\n=== Example 2: Basic Variadic Template ===\n";

    cout << "✅ Type-safe variadic function:\n";
    cout << "  ";
    print(1, 2, 3, 4, 5);

    cout << "  ";
    print("Hello", "World", "from", "C++");

    cout << "  ";
    print(42, "answer", 3.14, 'X', true);

    cout << "\n✅ Advantages:\n";
    cout << "  • Type-safe (compile-time checked)\n";
    cout << "  • Works with ANY types\n";
    cout << "  • No manual count needed\n";
    cout << "  • Zero runtime overhead\n";
}
}

// ============================================
// EXAMPLE 3: sizeof... Operator
// ============================================

namespace SizeofPack {

// Count number of arguments at compile-time
template<typename... Args>
void print_with_count(Args... args) {
    cout << "  Number of arguments: " << sizeof...(Args) << "\n";
    cout << "  Values: ";

    // Need base case for recursion
    int dummy[] = {(cout << args << " ", 0)...};
    (void)dummy;  // Suppress unused warning

    cout << "\n";
}

// Alternative: use fold expression (C++17)
template<typename... Args>
size_t count_args(Args...) {
    return sizeof...(Args);
}

void demonstrate() {
    cout << "\n=== Example 3: sizeof... Operator ===\n";

    cout << "Count arguments at compile-time:\n";
    print_with_count(1, 2, 3);
    print_with_count("a", "b", "c", "d", "e");
    print_with_count(42);

    cout << "\nDirect count:\n";
    cout << "  count_args(1,2,3,4,5) = " << count_args(1,2,3,4,5) << "\n";
    cout << "  count_args() = " << count_args() << "\n";
}
}

// ============================================
// EXAMPLE 4: Fold Expressions (C++17)
// ============================================

namespace FoldExpressions {

// Sum any number of arguments
template<typename... Args>
auto sum(Args... args) {
    return (args + ...);  // Unary right fold
}

// Product
template<typename... Args>
auto product(Args... args) {
    return (args * ...);
}

// Logical AND
template<typename... Args>
bool all_true(Args... args) {
    return (args && ...);
}

// Comma fold - execute all, return last
template<typename... Args>
void print_all(Args... args) {
    (cout << ... << args) << "\n";  // Binary left fold with <<
}

// Print with separator
template<typename... Args>
void print_separated(Args... args) {
    bool first = true;
    ((cout << (first ? "" : ", ") << args, first = false), ...);
    cout << "\n";
}

void demonstrate() {
    cout << "\n=== Example 4: Fold Expressions (C++17) ===\n";

    cout << "Sum:\n";
    cout << "  sum(1,2,3,4,5) = " << sum(1,2,3,4,5) << "\n";
    cout << "  sum(1.5, 2.5, 3.0) = " << sum(1.5, 2.5, 3.0) << "\n";

    cout << "\nProduct:\n";
    cout << "  product(2,3,4) = " << product(2,3,4) << "\n";

    cout << "\nLogical operations:\n";
    cout << "  all_true(true, true, true) = " << all_true(true, true, true) << "\n";
    cout << "  all_true(true, false, true) = " << all_true(true, false, true) << "\n";

    cout << "\nPrint all:\n";
    cout << "  ";
    print_all("Hello", " ", "World", "!");

    cout << "\nPrint with separator:\n";
    cout << "  ";
    print_separated(1, 2, 3, 4, 5);
    cout << "  ";
    print_separated("apple", "banana", "cherry");
}
}

// ============================================
// EXAMPLE 5: Variadic Class Templates
// ============================================

namespace VariadicClass {

// Tuple-like container
template<typename... Types>
class MyTuple {
private:
    static constexpr size_t size = sizeof...(Types);

public:
    MyTuple() {
        cout << "  MyTuple created with " << size << " types\n";
    }

    static size_t getSize() { return size; }
};

// Variadic inheritance
template<typename... Bases>
class Multi : public Bases... {
public:
    Multi() {
        cout << "  Multi<> inherits from " << sizeof...(Bases) << " bases\n";
    }
};

// Example base classes
struct A { void funcA() { cout << "    A::funcA()\n"; } };
struct B { void funcB() { cout << "    B::funcB()\n"; } };
struct C { void funcC() { cout << "    C::funcC()\n"; } };

void demonstrate() {
    cout << "\n=== Example 5: Variadic Class Templates ===\n";

    cout << "Variadic tuple:\n";
    MyTuple<int, double, string> t1;
    MyTuple<char> t2;
    MyTuple<> t3;

    cout << "\nVariadic inheritance:\n";
    Multi<A, B, C> m;
    m.funcA();
    m.funcB();
    m.funcC();

    cout << "  ✅ Object has all base class methods!\n";
}
}

// ============================================
// EXAMPLE 6: Perfect Forwarding with Variadic
// ============================================

namespace PerfectForwarding {

// Factory function that forwards all arguments
template<typename T, typename... Args>
T* create(Args&&... args) {
    cout << "  Creating object with " << sizeof...(Args) << " arguments\n";
    return new T(std::forward<Args>(args)...);
}

// Example class
class Widget {
    int x;
    double y;
    string name;

public:
    Widget(int x, double y, string name)
        : x(x), y(y), name(name) {
        cout << "  Widget(" << x << ", " << y << ", \"" << name << "\") constructed\n";
    }

    void display() const {
        cout << "  Widget: x=" << x << ", y=" << y << ", name=\"" << name << "\"\n";
    }
};

void demonstrate() {
    cout << "\n=== Example 6: Perfect Forwarding ===\n";

    cout << "Creating widgets:\n";
    auto w1 = create<Widget>(10, 3.14, "Widget1");
    w1->display();

    auto w2 = create<Widget>(20, 6.28, "Widget2");
    w2->display();

    delete w1;
    delete w2;

    cout << "\n✅ Arguments perfectly forwarded to constructor!\n";
}
}

// ============================================
// EXAMPLE 7: Type-safe printf
// ============================================

namespace TypeSafePrintf {

// Helper: print single value
template<typename T>
void print_value(const T& value) {
    cout << value;
}

// Base case: no more arguments
void printf_impl(const char* format) {
    while (*format) {
        if (*format == '%' && *(format + 1) != '%') {
            throw runtime_error("Too few arguments for format string");
        }
        cout << *format++;
    }
}

// Recursive case: process one argument
template<typename T, typename... Args>
void printf_impl(const char* format, T value, Args... args) {
    while (*format) {
        if (*format == '%' && *(format + 1) != '%') {
            print_value(value);
            printf_impl(format + 1, args...);
            return;
        }
        cout << *format++;
    }
    throw runtime_error("Too many arguments for format string");
}

// User-facing function
template<typename... Args>
void my_printf(const char* format, Args... args) {
    printf_impl(format, args...);
}

void demonstrate() {
    cout << "\n=== Example 7: Type-safe printf ===\n";

    cout << "Custom printf:\n";
    cout << "  ";
    my_printf("Hello, %!\n", "World");

    cout << "  ";
    my_printf("Number: %, String: %, Float: %\n", 42, "test", 3.14);

    cout << "  ";
    my_printf("Mix: % and % and %\n", 10, "hello", 2.5);

    cout << "\n✅ Type-safe at compile-time!\n";
    cout << "✅ Works with any printable type!\n";
}
}

// ============================================
// EXAMPLE 8: Variadic Minimum/Maximum
// ============================================

namespace MinMax {

// Base case: single value
template<typename T>
T minimum(T value) {
    return value;
}

// Base case: single argument
template<typename T>
T maximum(T value) {
    return value;
}


// Recursive case: compare first with rest
template<typename T, typename... Args>
T minimum(T first, Args... args) {
    T rest_min = minimum(args...);
    return (first < rest_min) ? first : rest_min;
}


// Recursive case: multiple arguments
template<typename T, typename... Args>
T maximum(T first, Args... args) {
    T rest_max = maximum(args...);
    return (first > rest_max) ? first : rest_max;
}


// Using fold expression (C++17)
template<typename... Args>
auto min_fold(Args... args) {
    return [](auto... values) {
        auto result = (values, ...);  // Get last value as initial
        ((result = (values < result ? values : result)), ...);
        return result;
    }(args...);
}

// Using fold expression (C++17)
template<typename... Args>
auto max_fold(Args... args) {
    return [](auto... values) {
        auto result = (values, ...);  // Get last value as initial
        ((result = (values > result ? values : result)), ...);
        return result;
    }(args...);
}

void demonstrate() {
    cout << "\n=== Example 8: Variadic Min/Max ===\n";

    cout << "Minimum:\n";
    cout << "  minimum(5,2,9,1,7) = " << minimum(5,2,9,1,7) << "\n";
    cout << "  minimum(3.14, 2.71, 1.41) = " << minimum(3.14, 2.71, 1.41) << "\n";

    cout << "\nMaximum:\n";
    cout << "  maximum(5,2,9,1,7) = " << maximum(5,2,9,1,7) << "\n";
    cout << "  maximum(10, 50, 30, 70, 20) = " << maximum(10, 50, 30, 70, 20) << "\n";

    cout << "\n✅ Works with any number of arguments!\n";
    cout << "✅ Works with any comparable type!\n";
}
}

// ============================================
// EXAMPLE 9: Variadic emplace
// ============================================

namespace Emplace {

// Simple vector-like container
template<typename T>
class Container {
private:
    vector<T> data;

public:
    // Variadic emplace_back - construct in-place
    template<typename... Args>
    void emplace_back(Args&&... args) {
        cout << "  Constructing element with " << sizeof...(Args) << " arguments\n";
        data.emplace_back(std::forward<Args>(args)...);
    }

    size_t size() const { return data.size(); }

    const T& operator[](size_t i) const { return data[i]; }
};

// Complex object
struct Point {
    int x, y;
    string label;

    Point(int x, int y, string label) : x(x), y(y), label(label) {
        cout << "    Point(" << x << ", " << y << ", \"" << label << "\") constructed\n";
    }

    void display() const {
        cout << "    Point: (" << x << ", " << y << ") \"" << label << "\"\n";
    }
};

void demonstrate() {
    cout << "\n=== Example 9: Variadic emplace ===\n";

    Container<Point> points;

    cout << "Adding points:\n";
    points.emplace_back(10, 20, "Origin");
    points.emplace_back(100, 200, "Target");
    points.emplace_back(50, 75, "Middle");

    cout << "\nStored points:\n";
    for (size_t i = 0; i < points.size(); ++i) {
        points[i].display();
    }

    cout << "\n✅ Objects constructed directly in container!\n";
    cout << "✅ No temporary objects created!\n";
    cout << "✅ Perfect forwarding to constructor!\n";
}
}

// ============================================
// EXAMPLE 10: Real-world - Logger
// ============================================

namespace Logger {

enum class LogLevel {
    DEBUG, INFO, WARNING, ERROR
};

// Convert log level to string
const char* levelToString(LogLevel level) {
    switch (level) {
    case LogLevel::DEBUG:   return "DEBUG";
    case LogLevel::INFO:    return "INFO";
    case LogLevel::WARNING: return "WARNING";
    case LogLevel::ERROR:   return "ERROR";
    default: return "UNKNOWN";
    }
}

// Variadic logger
template<typename... Args>
void log(LogLevel level, Args... args) {
    cout << "[" << levelToString(level) << "] ";
    (cout << ... << args) << "\n";
}

// Convenience functions
template<typename... Args>
void debug(Args... args) {
    log(LogLevel::DEBUG, args...);
}

template<typename... Args>
void info(Args... args) {
    log(LogLevel::INFO, args...);
}

template<typename... Args>
void warning(Args... args) {
    log(LogLevel::WARNING, args...);
}

template<typename... Args>
void error(Args... args) {
    log(LogLevel::ERROR, args...);
}

void demonstrate() {
    cout << "\n=== Example 10: Variadic Logger ===\n";

    debug("Starting application...");
    info("User logged in: ", "John", ", ID: ", 12345);
    warning("Low memory: ", 128, " MB remaining");
    error("Connection failed to ", "server.com", ":", 8080);

    cout << "\n✅ Type-safe logging!\n";
    cout << "✅ Any number of arguments!\n";
    cout << "✅ Mixed types supported!\n";
    cout << "✅ Zero overhead!\n";
}
}

// ============================================
// COMPREHENSIVE OUTPUT
// ============================================

void printComprehensiveInfo() {
    cout << "\n" << string(60, '=') << "\n";
    cout << "VARIADIC TEMPLATES - COMPLETE REFERENCE\n";
    cout << string(60, '=') << "\n";

    cout << "\n📚 WHAT ARE VARIADIC TEMPLATES?\n";
    cout << string(60, '-') << "\n";
    cout << "Templates that accept ANY NUMBER of type parameters:\n";
    cout << "  template<typename... Args>     // Parameter pack\n";
    cout << "  void func(Args... args) {      // Function parameters\n";
    cout << "      // sizeof...(Args)          // Count at compile-time\n";
    cout << "      // (args + ...)             // Fold expression\n";
    cout << "  }\n";

    cout << "\n📊 KEY SYNTAX\n";
    cout << string(60, '-') << "\n";
    cout << "typename... Args       // Type parameter pack declaration\n";
    cout << "Args... args           // Parameter pack expansion\n";
    cout << "sizeof...(Args)        // Number of types (compile-time)\n";
    cout << "sizeof...(args)        // Number of arguments\n";
    cout << "(args + ...)           // Fold expression (C++17)\n";
    cout << "func(args...)          // Expand pack in function call\n";
    cout << "std::forward<Args>(args)...  // Perfect forwarding\n";

    cout << "\n🔄 PROCESSING PATTERNS\n";
    cout << string(60, '-') << "\n";
    cout << "1. RECURSION (Classic):\n";
    cout << "   • Base case: void func() { }\n";
    cout << "   • Recursive: template<typename T, typename... Args>\n";
    cout << "                void func(T first, Args... rest)\n";
    cout << "\n";
    cout << "2. FOLD EXPRESSIONS (C++17):\n";
    cout << "   • Unary right fold:  (args op ...)\n";
    cout << "   • Unary left fold:   (... op args)\n";
    cout << "   • Binary right fold: (args op ... op init)\n";
    cout << "   • Binary left fold:  (init op ... op args)\n";
    cout << "\n";
    cout << "3. COMMA TRICK:\n";
    cout << "   int dummy[] = {(func(args), 0)...};\n";
    cout << "\n";
    cout << "4. INITIALIZER LIST:\n";
    cout << "   {func(args)...}\n";

    cout << "\n💡 COMMON USE CASES\n";
    cout << string(60, '-') << "\n";
    cout << "✅ Generic print/logging functions\n";
    cout << "✅ Factory functions (perfect forwarding)\n";
    cout << "✅ Tuple-like containers\n";
    cout << "✅ Type-safe printf alternatives\n";
    cout << "✅ Mathematical operations (sum, product, min, max)\n";
    cout << "✅ Container emplace operations\n";
    cout << "✅ Event handlers with any arguments\n";
    cout << "✅ Generic wrappers and adapters\n";

    cout << "\n⚠️  COMMON MISTAKES\n";
    cout << string(60, '-') << "\n";
    cout << "❌ Forgetting base case in recursion:\n";
    cout << "   template<typename... Args>\n";
    cout << "   void func(Args... args);  // ❌ Infinite recursion!\n";
    cout << "\n";
    cout << "❌ Wrong pack expansion:\n";
    cout << "   func(args)...   // ❌ Wrong!\n";
    cout << "   func(args...)   // ✅ Correct!\n";
    cout << "\n";
    cout << "❌ Not forwarding in perfect forwarding:\n";
    cout << "   template<typename... Args>\n";
    cout << "   void wrapper(Args&&... args) {\n";
    cout << "       func(args...);  // ❌ Loses rvalue-ness!\n";
    cout << "       func(std::forward<Args>(args)...);  // ✅ Correct!\n";
    cout << "   }\n";
    cout << "\n";
    cout << "❌ Trying to use sizeof(Args):\n";
    cout << "   sizeof(Args)     // ❌ Won't compile!\n";
    cout << "   sizeof...(Args)  // ✅ Correct!\n";

    cout << "\n🎯 BEST PRACTICES\n";
    cout << string(60, '-') << "\n";
    cout << "✅ Use fold expressions (C++17) instead of recursion\n";
    cout << "✅ Perfect forward when forwarding to other functions\n";
    cout << "✅ Use sizeof... to get pack size\n";
    cout << "✅ Combine with SFINAE for constraints\n";
    cout << "✅ Use concepts (C++20) for better error messages\n";
    cout << "✅ Keep variadic functions simple\n";
    cout << "✅ Document expected types/requirements\n";
    cout << "✅ Test with 0, 1, and many arguments\n";

    cout << "\n🔍 TYPE REQUIREMENTS\n";
    cout << string(60, '-') << "\n";
    cout << "Args can be:\n";
    cout << "  • Any types (mixed)\n";
    cout << "  • Same type (homogeneous)\n";
    cout << "  • Empty (zero arguments)\n";
    cout << "\n";
    cout << "Operations must be valid for all types:\n";
    cout << "  (args + ...)  // All types must support operator+\n";
    cout << "  print(args...) // All types must be printable\n";

    cout << "\n⚡ PERFORMANCE\n";
    cout << string(60, '-') << "\n";
    cout << "✅ Zero runtime overhead:\n";
    cout << "  • Everything resolved at compile-time\n";
    cout << "  • No type erasure\n";
    cout << "  • Inline expansion\n";
    cout << "  • No virtual calls\n";
    cout << "\n";
    cout << "✅ Perfect forwarding avoids copies:\n";
    cout << "  • Preserves value categories\n";
    cout << "  • Moves when possible\n";
    cout << "  • Direct construction (emplace)\n";

    cout << "\n📖 RELATED CONCEPTS\n";
    cout << string(60, '-') << "\n";
    cout << "• Parameter packs (typename... Args)\n";
    cout << "• Pack expansion (args...)\n";
    cout << "• Fold expressions (C++17)\n";
    cout << "• Perfect forwarding (std::forward)\n";
    cout << "• Universal references (T&&)\n";
    cout << "• SFINAE (enable_if)\n";
    cout << "• Concepts (C++20 constraints)\n";
    cout << "• std::tuple (variadic tuple type)\n";
    cout << "• std::apply (invoke with tuple)\n";

    cout << "\n🚀 C++ EVOLUTION\n";
    cout << string(60, '-') << "\n";
    cout << "C++11: Variadic templates introduced\n";
    cout << "C++14: Improved type deduction\n";
    cout << "C++17: Fold expressions (major improvement!)\n";
    cout << "C++20: Concepts for better constraints\n";
}

// ============================================
// MAIN
// ============================================

int main() {
    cout << "\n╔═══════════════════════════════════════════════════════╗\n";
    cout << "║         VARIADIC TEMPLATES IN C++                     ║\n";
    cout << "║                                                       ║\n";
    cout << "║  Type-safe variable arguments with zero overhead!    ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n";

    UnsafeVersion::demonstrate();
    BasicVariadic::demonstrate();
    SizeofPack::demonstrate();
    FoldExpressions::demonstrate();
    VariadicClass::demonstrate();
    PerfectForwarding::demonstrate();
    TypeSafePrintf::demonstrate();
    MinMax::demonstrate();
    Emplace::demonstrate();
    Logger::demonstrate();

    printComprehensiveInfo();

    cout << "\n" << string(60, '=') << "\n";
    cout << "🎓 KEY TAKEAWAYS\n";
    cout << string(60, '=') << "\n";
    cout << "1. Variadic templates = type-safe variable arguments\n";
    cout << "2. Zero runtime overhead (compile-time feature)\n";
    cout << "3. Use fold expressions (C++17) for simplicity\n";
    cout << "4. Perfect forward when passing to other functions\n";
    cout << "5. sizeof... operator counts arguments\n";
    cout << "6. Works with any types (mixed or homogeneous)\n";
    cout << "7. Foundation of many STL features (tuple, apply)\n";
    cout << "8. Replaces unsafe C-style va_args\n";
    cout << "9. Essential for modern C++ generic programming\n";
    cout << "10. Combine with SFINAE/concepts for constraints\n";
    cout << string(60, '=') << "\n\n";

    return 0;
}
