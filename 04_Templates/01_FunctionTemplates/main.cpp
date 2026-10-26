#include <print>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <vector>

// ============================================
// WHAT ARE FUNCTION TEMPLATES?
// ============================================

/*
 * FUNCTION TEMPLATES:
 * - Generic functions that work with any type
 * - Write once, use with many types
 * - Type parameters specified with template<typename T>
 * - Compiler generates specific versions (instantiation)
 * - Foundation of generic programming
 *
 * SYNTAX:
 * - template<typename T>
 * - template<class T>  (same as typename)
 * - T is type parameter (placeholder)
 * - Actual type deduced or specified
 *
 * WHY TEMPLATES?
 * - Code reuse without duplication
 * - Type safety (compile-time checking)
 * - No runtime overhead
 * - STL is built on templates
 * - Generic algorithms
 *
 * HOW IT WORKS:
 * - Template is NOT code, it's a blueprint
 * - Compiler generates actual functions
 * - One template → many functions
 * - Instantiation on first use
 * - Dead code elimination
 *
 * TEMPLATE INSTANTIATION:
 * - Implicit: compiler deduces type
 * - Explicit: you specify type
 * - Each type creates new function
 * - Compile-time process
 *
 * BENEFITS:
 * - Type safe
 * - No code duplication
 * - Compile-time optimization
 * - Works with any type
 * - Zero runtime overhead
 */

// ============================================
// === EXAMPLE 1: THE PROBLEM (Without Templates) ===
// ============================================

// Need separate function for each type!
int maxInt(int a, int b) {
    std::println("  maxInt called");
    return (a > b) ? a : b;
}

double maxDouble(double a, double b) {
    std::println("  maxDouble called");
    return (a > b) ? a : b;
}

std::string maxString(const std::string& a, const std::string& b) {
    std::println("  maxString called");
    return (a > b) ? a : b;
}

void demonstrateProblem() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: WITHOUT TEMPLATES      ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Need separate function for each type ---");
    std::println("Max int: {}", maxInt(10, 20));
    std::println("Max double: {}", maxDouble(3.14, 2.71));
    std::println("Max string: {}", maxString("apple", "banana"));
}

// ============================================
// === EXAMPLE 2: BASIC FUNCTION TEMPLATE ===
// ============================================

// One template works for all types!
template<typename T>
T maximum(T a, T b) {
    std::println("  maximum<T> called");
    return (a > b) ? a : b;
}

void demonstrateBasicTemplate() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: BASIC TEMPLATE         ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- One template, many types! ---");
    std::println("Max int: {}", maximum(10, 20));
    std::println("Max double: {}", maximum(3.14, 2.71));
    std::println("Max string: {}", maximum(std::string("apple"), std::string("banana")));
    std::println("Max char: {}", maximum('x', 'a'));
}

// ============================================
// === EXAMPLE 3: TYPE DEDUCTION ===
// ============================================

template<typename T>
void printType(T value) {
    std::println("  Value: {}", value);
    std::println("  Type: {}", typeid(T).name());
}

void demonstrateTypeDeduction() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: TYPE DEDUCTION         ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Implicit type deduction ---");
    printType(42);          // T = int
    printType(3.14);        // T = double
    printType("Hello");     // T = const char*
    printType(std::string("World"));  // T = string

    std::println("");
    std::println("--- Explicit type specification ---");
    printType<int>(42);
    printType<double>(3.14);
    printType<std::string>("Explicit");
}

// ============================================
// === EXAMPLE 4: MULTIPLE TEMPLATE PARAMETERS ===
// ============================================

template<typename T1, typename T2>
void printPair(T1 first, T2 second) {
    std::println("  Pair: ({}, {})", first, second);
    std::println("  Types: {}, {}", typeid(T1).name(), typeid(T2).name());
}

template<typename T1, typename T2, typename T3>
T1 mix(T1 a, T2 b, T3 c) {
    std::println("  Mixing: {}, {}, {}", a, b, c);
    return a + static_cast<T1>(b) + static_cast<T1>(c);
}

void demonstrateMultipleParams() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: MULTIPLE PARAMETERS    ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Different types in one template ---");
    printPair(42, 3.14);
    printPair("Age", 25);
    printPair(true, "Success");

    std::println("");
    std::println("--- Three different types ---");
    std::println("Result: {}", mix(10, 20.5, 30));
    std::println("Result: {}", mix(1.5, 2, 3.7));
}

// ============================================
// === EXAMPLE 5: TEMPLATE SPECIALIZATION ===
// ============================================

// Generic template
template<typename T>
T add(T a, T b) {
    std::println("  Generic add");
    return a + b;
}

// NOT a specialization: a plain (non-template) OVERLOAD for const char*.
// Non-template functions win over the template when the match is equally good.
std::string add(const char* a, const char* b) {
    std::println("  Overload add for strings");
    return std::string(a) + std::string(b);
}

// Real specialization for bool
template<>
bool add<bool>(bool a, bool b) {
    std::println("  Specialized add for bool (logical OR)");
    return a || b;
}

void demonstrateSpecialization() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 5: SPECIALIZATION         ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Generic template ---");
    std::println("Result: {}", add(10, 20));
    std::println("Result: {}", add(3.14, 2.86));

    std::println("");
    std::println("--- Overload and specialization ---");
    std::println("Result: {}", add("Hello", " World"));
    std::println("Result: {}", add(true, false));
}

// ============================================
// === EXAMPLE 6: OVERLOADING WITH TEMPLATES ===
// ============================================

// Template version
template<typename T>
void display(T value) {
    std::println("  Template: {}", value);
}

// Regular overloads
void display(int value) {
    std::println("  Regular int: {}", value);
}

void display(double value) {
    std::println("  Regular double: {}", value);
}

// Template with two parameters
template<typename T>
void display(T a, T b) {
    std::println("  Template pair: {}, {}", a, b);
}

void demonstrateOverloading() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 6: OVERLOADING            ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Regular functions preferred ---");
    display(42);        // Regular int
    display(3.14);      // Regular double

    std::println("");
    std::println("--- Template used when no match ---");
    display("Hello");   // Template
    display('x');       // Template

    std::println("");
    std::println("--- Template with multiple args ---");
    display(10, 20);
    display("Hi", "There");
}

// ============================================
// === EXAMPLE 7: NON-TYPE TEMPLATE PARAMETERS ===
// ============================================

// Template with non-type parameter
template<typename T, int N>
class Array {
private:
    T data[N];

public:
    int size() const { return N; }

    T& operator[](int index) { return data[index]; }
    const T& operator[](int index) const { return data[index]; }
};

// Function with non-type parameter
template<int N>
int multiply() {
    return N * 2;
}

template<typename T, int Size>
void printArray(const T (&arr)[Size]) {
    std::print("  Array[{}]: ", Size);
    for (int i = 0; i < Size; ++i) {
        std::print("{} ", arr[i]);
    }
    std::println("");
}

void demonstrateNonType() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 7: NON-TYPE PARAMETERS    ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Array with compile-time size ---");
    Array<int, 5> arr1;
    arr1[0] = 10;
    arr1[1] = 20;
    std::println("  Array size: {}", arr1.size());
    std::println("  First element: {}", arr1[0]);

    std::println("");
    std::println("--- Non-type in function ---");
    std::println("  multiply<5>(): {}", multiply<5>());
    std::println("  multiply<10>(): {}", multiply<10>());

    std::println("");
    std::println("--- Array size deduction ---");
    int nums[] = {1, 2, 3, 4, 5};
    printArray(nums);

    double values[] = {1.1, 2.2, 3.3};
    printArray(values);
}

// ============================================
// === EXAMPLE 8: RETURN TYPE DEDUCTION ===
// ============================================

// C++11: Trailing return type
template<typename T1, typename T2>
auto addAuto(T1 a, T2 b) -> decltype(a + b) {
    std::println("  Adding with trailing return");
    return a + b;
}

// C++14: Full return type deduction
template<typename T1, typename T2>
auto multiply(T1 a, T2 b) {
    std::println("  Multiplying with auto return");
    return a * b;
}

void demonstrateReturnDeduction() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 8: RETURN TYPE DEDUCTION  ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Trailing return type ---");
    auto r1 = addAuto(10, 20);        // int + int = int
    auto r2 = addAuto(10, 2.5);       // int + double = double
    auto r3 = addAuto(1.5, 2.5);      // double + double = double
    std::println("  Results: {}, {}, {}", r1, r2, r3);

    std::println("");
    std::println("--- Auto return (C++14) ---");
    auto r4 = multiply(5, 10);        // int
    auto r5 = multiply(2.5, 4);       // double
    std::println("  Results: {}, {}", r4, r5);
}

// ============================================
// === EXAMPLE 9: TEMPLATE CONSTRAINTS (Concepts Preview) ===
// ============================================

// Using SFINAE (before C++20 concepts)
template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value, T>::type
safeAdd(T a, T b) {
    std::println("  Arithmetic type: safe add");
    return a + b;
}

// Check if type is numeric
template<typename T>
void checkType(T value) {
    if (std::is_integral<T>::value) {
        std::println("  Integral type: {}", value);
    } else if (std::is_floating_point<T>::value) {
        std::println("  Floating point type: {}", value);
    } else {
        std::println("  Other type");
    }
}

void demonstrateConstraints() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 9: CONSTRAINTS (SFINAE)   ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Type constraints with enable_if ---");
    std::println("Result: {}", safeAdd(10, 20));
    std::println("Result: {}", safeAdd(3.14, 2.86));
    // safeAdd("hello", "world");  // ❌ Error: not arithmetic

    std::println("");
    std::println("--- Type checking ---");
    checkType(42);
    checkType(3.14);
    checkType("Hello");
}

// ============================================
// === EXAMPLE 10: REAL-WORLD EXAMPLES ===
// ============================================

// Generic swap
template<typename T>
void mySwap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

// Generic print container
template<typename Container>
void printContainer(const Container& container, const std::string& name) {
    std::print("  {}: ", name);
    for (const auto& item : container) {
        std::print("{} ", item);
    }
    std::println("");
}

// Generic find
template<typename Container, typename Value>
bool contains(const Container& container, const Value& value) {
    for (const auto& item : container) {
        if (item == value) return true;
    }
    return false;
}

// Generic sum of a range
// (named sumRange, not accumulate: with `using namespace std` and <numeric>
//  included, a call to our own accumulate() would be ambiguous with std::accumulate)
template<typename Iterator, typename T>
T sumRange(Iterator begin, Iterator end, T init) {
    T result = init;
    for (auto it = begin; it != end; ++it) {
        result += *it;
    }
    return result;
}

void demonstrateRealWorld() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 10: REAL-WORLD USAGE      ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Generic swap ---");
    int x = 10, y = 20;
    std::println("  Before: x={}, y={}", x, y);
    mySwap(x, y);
    std::println("  After: x={}, y={}", x, y);

    std::string s1 = "Hello", s2 = "World";
    mySwap(s1, s2);
    std::println("  Strings: {}, {}", s1, s2);

    std::println("");
    std::println("--- Generic print ---");
    std::vector<int> numbers{1, 2, 3, 4, 5};
    std::vector<std::string> words{"apple", "banana", "cherry"};
    printContainer(numbers, "Numbers");
    printContainer(words, "Words");

    std::println("");
    std::println("--- Generic find ---");
    std::println("  Contains 3? {}", contains(numbers, 3) ? "Yes" : "No");
    std::println("  Contains 10? {}", contains(numbers, 10) ? "Yes" : "No");

    std::println("");
    std::println("--- Generic sum ---");
    int sum = sumRange(numbers.begin(), numbers.end(), 0);
    std::println("  Sum: {}", sum);
}

// ============================================
// MAIN FUNCTION - RUN ALL EXAMPLES
// ============================================

int main() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════════════╗");
    std::println("║                                                           ║");
    std::println("║            FUNCTION TEMPLATES IN C++                     ║");
    std::println("║            Generic Programming Foundation                 ║");
    std::println("║                                                           ║");
    std::println("╚═══════════════════════════════════════════════════════════╝");

    demonstrateProblem();
    demonstrateBasicTemplate();
    demonstrateTypeDeduction();
    demonstrateMultipleParams();
    demonstrateSpecialization();
    demonstrateOverloading();
    demonstrateNonType();
    demonstrateReturnDeduction();
    demonstrateConstraints();
    demonstrateRealWorld();

    return 0;
}
