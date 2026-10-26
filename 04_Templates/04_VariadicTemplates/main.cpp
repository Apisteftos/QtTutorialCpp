#include <cstdarg>
#include <cstddef>
#include <print>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

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
// === EXAMPLE 1: The Problem - Unsafe va_args ===
// ============================================

namespace UnsafeVersion {

// Old C-style: NOT type-safe!
void print_unsafe(int count, ...) {
    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; ++i) {
        // ❌ Must manually specify type - easy to mess up!
        int value = va_arg(args, int);
        std::print("{} ", value);
    }

    va_end(args);
    std::println("");
}

void demonstrate() {
    std::println("");
    std::println("=== Example 1: The Problem ===");
    std::println("Old C-style variadic functions:");

    std::print("  ");
    print_unsafe(3, 10, 20, 30);  // Works

    // ❌ These compile but cause UNDEFINED BEHAVIOR:
    // print_unsafe(2, 10, "hello");     // Wrong type!
    // print_unsafe(1, 10, 20, 30);      // Wrong count!
}
}

// ============================================
// === EXAMPLE 2: Basic Variadic Template ===
// ============================================

namespace BasicVariadic {

// Base case: no arguments
void printArgs() {
    std::println("");
}

// Recursive case: peel off first argument
template<typename T, typename... Args>
void printArgs(T first, Args... rest) {
    std::print("{} ", first);
    printArgs(rest...);  // Recursively call with remaining arguments
}

void demonstrate() {
    std::println("");
    std::println("=== Example 2: Basic Variadic Template ===");

    std::println("✅ Type-safe variadic function:");
    std::print("  ");
    printArgs(1, 2, 3, 4, 5);

    std::print("  ");
    printArgs("Hello", "World", "from", "C++");

    std::print("  ");
    printArgs(42, "answer", 3.14, 'X', true);
}
}

// ============================================
// === EXAMPLE 3: sizeof... Operator ===
// ============================================

namespace SizeofPack {

// Count number of arguments at compile-time
template<typename... Args>
void print_with_count(Args... args) {
    std::println("  Number of arguments: {}", sizeof...(Args));
    std::print("  Values: ");

    // Comma trick: the braced list is evaluated left to right.
    // The leading 0 keeps the array non-empty when the pack is empty.
    int dummy[] = {0, (std::print("{} ", args), 0)...};
    (void)dummy;  // Suppress unused warning

    std::println("");
}

// sizeof... gives the number of types in the pack
template<typename... Args>
std::size_t count_args(Args...) {
    return sizeof...(Args);
}

void demonstrate() {
    std::println("");
    std::println("=== Example 3: sizeof... Operator ===");

    std::println("Count arguments at compile-time:");
    print_with_count(1, 2, 3);
    print_with_count("a", "b", "c", "d", "e");
    print_with_count(42);
    print_with_count();

    std::println("");
    std::println("Direct count:");
    std::println("  count_args(1,2,3,4,5) = {}", count_args(1, 2, 3, 4, 5));
    std::println("  count_args() = {}", count_args());
}
}

// ============================================
// === EXAMPLE 4: Fold Expressions (C++17) ===
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

// Binary left fold with <<: write everything into one stream
template<typename... Args>
std::string concat(Args... args) {
    std::ostringstream oss;
    (oss << ... << args);
    return oss.str();
}

// Comma fold - execute for every argument, in order
template<typename... Args>
void print_all(Args... args) {
    (std::print("{}", args), ...);
    std::println("");
}

// Print with separator
template<typename... Args>
void print_separated(Args... args) {
    bool first = true;
    ((std::print("{}{}", first ? "" : ", ", args), first = false), ...);
    std::println("");
}

void demonstrate() {
    std::println("");
    std::println("=== Example 4: Fold Expressions (C++17) ===");

    std::println("Sum:");
    std::println("  sum(1,2,3,4,5) = {}", sum(1, 2, 3, 4, 5));
    std::println("  sum(1.5, 2.5, 3.0) = {}", sum(1.5, 2.5, 3.0));

    std::println("");
    std::println("Product:");
    std::println("  product(2,3,4) = {}", product(2, 3, 4));

    std::println("");
    std::println("Logical operations:");
    std::println("  all_true(true, true, true) = {}", all_true(true, true, true));
    std::println("  all_true(true, false, true) = {}", all_true(true, false, true));
    std::println("  all_true() = {}", all_true());  // empty && fold is true

    std::println("");
    std::println("Binary left fold with <<:");
    std::println("  concat(\"x=\", 42, \", y=\", 3.5) = \"{}\"", concat("x=", 42, ", y=", 3.5));

    std::println("");
    std::println("Print all:");
    std::print("  ");
    print_all("Hello", " ", "World", "!");

    std::println("");
    std::println("Print with separator:");
    std::print("  ");
    print_separated(1, 2, 3, 4, 5);
    std::print("  ");
    print_separated("apple", "banana", "cherry");
}
}

// ============================================
// === EXAMPLE 5: Variadic Class Templates ===
// ============================================

namespace VariadicClass {

// Tuple-like container
template<typename... Types>
class MyTuple {
private:
    static constexpr std::size_t size = sizeof...(Types);

public:
    MyTuple() {
        std::println("  MyTuple created with {} types", size);
    }

    static std::size_t getSize() { return size; }
};

// Variadic inheritance
template<typename... Bases>
class Multi : public Bases... {
public:
    Multi() {
        std::println("  Multi<> inherits from {} bases", sizeof...(Bases));
    }
};

// Example base classes
struct A { void funcA() { std::println("    A::funcA()"); } };
struct B { void funcB() { std::println("    B::funcB()"); } };
struct C { void funcC() { std::println("    C::funcC()"); } };

void demonstrate() {
    std::println("");
    std::println("=== Example 5: Variadic Class Templates ===");

    std::println("Variadic tuple:");
    MyTuple<int, double, std::string> t1;
    MyTuple<char> t2;
    MyTuple<> t3;

    std::println("");
    std::println("Variadic inheritance:");
    Multi<A, B, C> m;
    m.funcA();
    m.funcB();
    m.funcC();
}
}

// ============================================
// === EXAMPLE 6: Perfect Forwarding with Variadic ===
// ============================================

namespace PerfectForwarding {

// Factory function that forwards all arguments
template<typename T, typename... Args>
T* create(Args&&... args) {
    std::println("  Creating object with {} arguments", sizeof...(Args));
    return new T(std::forward<Args>(args)...);
}

// Example class
class Widget {
    int x;
    double y;
    std::string name;

public:
    Widget(int x, double y, std::string name)
        : x(x), y(y), name(name) {
        std::println("  Widget({}, {}, \"{}\") constructed", x, y, name);
    }

    void display() const {
        std::println("  Widget: x={}, y={}, name=\"{}\"", x, y, name);
    }
};

void demonstrate() {
    std::println("");
    std::println("=== Example 6: Perfect Forwarding ===");

    std::println("Creating widgets:");
    auto w1 = create<Widget>(10, 3.14, "Widget1");
    w1->display();

    auto w2 = create<Widget>(20, 6.28, "Widget2");
    w2->display();

    delete w1;
    delete w2;
}
}

// ============================================
// === EXAMPLE 7: Custom printf ===
// ============================================

namespace TypeSafePrintf {

// Helper: print single value
template<typename T>
void print_value(const T& value) {
    std::print("{}", value);
}

// Base case: no more arguments
void printf_impl(const char* format) {
    while (*format) {
        if (*format == '%' && *(format + 1) != '%') {
            throw std::runtime_error("Too few arguments for format string");
        }
        std::print("{}", *format++);
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
        std::print("{}", *format++);
    }
    throw std::runtime_error("Too many arguments for format string");
}

// User-facing function
template<typename... Args>
void my_printf(const char* format, Args... args) {
    printf_impl(format, args...);
}

void demonstrate() {
    std::println("");
    std::println("=== Example 7: Custom printf ===");

    std::println("Custom printf:");
    std::print("  ");
    my_printf("Hello, %!\n", "World");

    std::print("  ");
    my_printf("Number: %, String: %, Float: %\n", 42, "test", 3.14);

    std::print("  ");
    my_printf("Mix: % and % and %\n", 10, "hello", 2.5);

    // The types are checked at compile time (every argument must be
    // formattable), but the NUMBER of arguments is only checked at runtime:
    std::println("");
    std::println("Wrong argument count throws at runtime:");
    try {
        my_printf("Only one slot: %\n", 1, 2);
    } catch (const std::runtime_error& e) {
        std::println("  Caught: {}", e.what());
    }
    try {
        my_printf("Two slots: % and %\n", 1);
    } catch (const std::runtime_error& e) {
        std::println("  Caught: {}", e.what());
    }
}
}

// ============================================
// === EXAMPLE 8: Variadic Minimum/Maximum ===
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


// Using fold expression (C++17): start with the first value,
// then fold the rest into it
template<typename T, typename... Args>
T min_fold(T first, Args... args) {
    T result = first;
    ((result = (args < result ? args : result)), ...);
    return result;
}

// Using fold expression (C++17)
template<typename T, typename... Args>
T max_fold(T first, Args... args) {
    T result = first;
    ((result = (args > result ? args : result)), ...);
    return result;
}

void demonstrate() {
    std::println("");
    std::println("=== Example 8: Variadic Min/Max ===");

    std::println("Minimum:");
    std::println("  minimum(5,2,9,1,7) = {}", minimum(5, 2, 9, 1, 7));
    std::println("  minimum(3.14, 2.71, 1.41) = {}", minimum(3.14, 2.71, 1.41));

    std::println("");
    std::println("Maximum:");
    std::println("  maximum(5,2,9,1,7) = {}", maximum(5, 2, 9, 1, 7));
    std::println("  maximum(10, 50, 30, 70, 20) = {}", maximum(10, 50, 30, 70, 20));

    std::println("");
    std::println("Fold versions:");
    std::println("  min_fold(5,2,9,1,7) = {}", min_fold(5, 2, 9, 1, 7));
    std::println("  max_fold(5,2,9,1,7) = {}", max_fold(5, 2, 9, 1, 7));
}
}

// ============================================
// === EXAMPLE 9: Variadic emplace ===
// ============================================

namespace Emplace {

// Simple vector-like container
template<typename T>
class Container {
private:
    std::vector<T> data;

public:
    // Variadic emplace_back - construct in-place
    template<typename... Args>
    void emplace_back(Args&&... args) {
        std::println("  Constructing element with {} arguments", sizeof...(Args));
        data.emplace_back(std::forward<Args>(args)...);
    }

    std::size_t size() const { return data.size(); }

    const T& operator[](std::size_t i) const { return data[i]; }
};

// Complex object
struct Point {
    int x, y;
    std::string label;

    Point(int x, int y, std::string label) : x(x), y(y), label(label) {
        std::println("    Point({}, {}, \"{}\") constructed", x, y, label);
    }

    void display() const {
        std::println("    Point: ({}, {}) \"{}\"", x, y, label);
    }
};

void demonstrate() {
    std::println("");
    std::println("=== Example 9: Variadic emplace ===");

    Container<Point> points;

    std::println("Adding points:");
    points.emplace_back(10, 20, "Origin");
    points.emplace_back(100, 200, "Target");
    points.emplace_back(50, 75, "Middle");

    std::println("");
    std::println("Stored points:");
    for (std::size_t i = 0; i < points.size(); ++i) {
        points[i].display();
    }
}
}

// ============================================
// === EXAMPLE 10: Real-world - Logger ===
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
    std::print("[{}] ", levelToString(level));
    (std::print("{}", args), ...);
    std::println("");
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
    std::println("");
    std::println("=== Example 10: Variadic Logger ===");

    debug("Starting application...");
    info("User logged in: ", "John", ", ID: ", 12345);
    warning("Low memory: ", 128, " MB remaining");
    error("Connection failed to ", "server.com", ":", 8080);
}
}

// ============================================
// MAIN
// ============================================

int main() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════════╗");
    std::println("║         VARIADIC TEMPLATES IN C++                     ║");
    std::println("║                                                       ║");
    std::println("║  Type-safe variable arguments with zero overhead!    ║");
    std::println("╚═══════════════════════════════════════════════════════╝");

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

    return 0;
}
