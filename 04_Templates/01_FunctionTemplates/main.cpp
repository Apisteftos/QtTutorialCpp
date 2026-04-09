#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <type_traits>
using namespace std;

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
// EXAMPLE 1: THE PROBLEM (Without Templates)
// ============================================

// Need separate function for each type!
int maxInt(int a, int b) {
    cout << "  maxInt called\n";
    return (a > b) ? a : b;
}

double maxDouble(double a, double b) {
    cout << "  maxDouble called\n";
    return (a > b) ? a : b;
}

string maxString(const string& a, const string& b) {
    cout << "  maxString called\n";
    return (a > b) ? a : b;
}

void demonstrateProblem() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: WITHOUT TEMPLATES      ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Need separate function for each type ---\n";
    cout << "Max int: " << maxInt(10, 20) << "\n";
    cout << "Max double: " << maxDouble(3.14, 2.71) << "\n";
    cout << "Max string: " << maxString("apple", "banana") << "\n";

    cout << "\n❌ PROBLEMS:\n";
    cout << "   • Code duplication\n";
    cout << "   • Hard to maintain\n";
    cout << "   • More types = more functions\n";
    cout << "   • Scaling nightmare\n";
    cout << "   • Error-prone\n";
}

// ============================================
// EXAMPLE 2: BASIC FUNCTION TEMPLATE
// ============================================

// One template works for all types!
template<typename T>
T maximum(T a, T b) {
    cout << "  maximum<T> called\n";
    return (a > b) ? a : b;
}

void demonstrateBasicTemplate() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: BASIC TEMPLATE         ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- One template, many types! ---\n";
    cout << "Max int: " << maximum(10, 20) << "\n";
    cout << "Max double: " << maximum(3.14, 2.71) << "\n";
    cout << "Max string: " << maximum(string("apple"), string("banana")) << "\n";
    cout << "Max char: " << maximum('x', 'a') << "\n";

    cout << "\n✅ TEMPLATE SOLUTION:\n";
    cout << "   • One function for all types\n";
    cout << "   • No code duplication\n";
    cout << "   • Type safe\n";
    cout << "   • Easy to maintain\n";
    cout << "   • Compiler generates code\n";
}

// ============================================
// EXAMPLE 3: TYPE DEDUCTION
// ============================================

template<typename T>
void printType(T value) {
    cout << "  Value: " << value << "\n";
    cout << "  Type: " << typeid(T).name() << "\n";
}

void demonstrateTypeDeduction() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: TYPE DEDUCTION         ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Implicit type deduction ---\n";
    printType(42);          // T = int
    printType(3.14);        // T = double
    printType("Hello");     // T = const char*
    printType(string("World"));  // T = string

    cout << "\n--- Explicit type specification ---\n";
    printType<int>(42);
    printType<double>(3.14);
    printType<string>("Explicit");

    cout << "\n💡 TYPE DEDUCTION:\n";
    cout << "   • Compiler figures out type\n";
    cout << "   • Based on arguments\n";
    cout << "   • Can specify explicitly\n";
    cout << "   • Type must be consistent\n";
}

// ============================================
// EXAMPLE 4: MULTIPLE TEMPLATE PARAMETERS
// ============================================

template<typename T1, typename T2>
void printPair(T1 first, T2 second) {
    cout << "  Pair: (" << first << ", " << second << ")\n";
    cout << "  Types: " << typeid(T1).name() << ", "
         << typeid(T2).name() << "\n";
}

template<typename T1, typename T2, typename T3>
T1 mix(T1 a, T2 b, T3 c) {
    cout << "  Mixing: " << a << ", " << b << ", " << c << "\n";
    return a + static_cast<T1>(b) + static_cast<T1>(c);
}

void demonstrateMultipleParams() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: MULTIPLE PARAMETERS    ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Different types in one template ---\n";
    printPair(42, 3.14);
    printPair("Age", 25);
    printPair(true, "Success");

    cout << "\n--- Three different types ---\n";
    cout << "Result: " << mix(10, 20.5, 30) << "\n";
    cout << "Result: " << mix(1.5, 2, 3.7) << "\n";

    cout << "\n💡 MULTIPLE PARAMETERS:\n";
    cout << "   • Each can be different type\n";
    cout << "   • Flexible combinations\n";
    cout << "   • Type deduction works\n";
}

// ============================================
// EXAMPLE 5: TEMPLATE SPECIALIZATION
// ============================================

// Generic template
template<typename T>
T add(T a, T b) {
    cout << "  Generic add\n";
    return a + b;
}

// Specialization for const char*
string add(const char* a, const char* b) {
    cout << "  Specialized add for strings\n";
    return string(a) + string(b);
}

// Specialization for bool
template<>
bool add<bool>(bool a, bool b) {
    cout << "  Specialized add for bool (logical OR)\n";
    return a || b;
}

void demonstrateSpecialization() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: SPECIALIZATION         ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Generic template ---\n";
    cout << "Result: " << add(10, 20) << "\n";
    cout << "Result: " << add(3.14, 2.86) << "\n";

    cout << "\n--- Specialized versions ---\n";
    cout << "Result: " << add("Hello", " World") << "\n";
    cout << "Result: " << add(true, false) << "\n";

    cout << "\n💡 SPECIALIZATION:\n";
    cout << "   • Custom behavior for specific types\n";
    cout << "   • Overrides generic template\n";
    cout << "   • template<> syntax\n";
    cout << "   • Type-specific optimization\n";
}

// ============================================
// EXAMPLE 6: OVERLOADING WITH TEMPLATES
// ============================================

// Template version
template<typename T>
void display(T value) {
    cout << "  Template: " << value << "\n";
}

// Regular overloads
void display(int value) {
    cout << "  Regular int: " << value << "\n";
}

void display(double value) {
    cout << "  Regular double: " << value << "\n";
}

// Template with two parameters
template<typename T>
void display(T a, T b) {
    cout << "  Template pair: " << a << ", " << b << "\n";
}

void demonstrateOverloading() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: OVERLOADING            ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Regular functions preferred ---\n";
    display(42);        // Regular int
    display(3.14);      // Regular double

    cout << "\n--- Template used when no match ---\n";
    display("Hello");   // Template
    display('x');       // Template

    cout << "\n--- Template with multiple args ---\n";
    display(10, 20);
    display("Hi", "There");

    cout << "\n💡 OVERLOAD RESOLUTION:\n";
    cout << "   1. Exact match (non-template)\n";
    cout << "   2. Template with deduction\n";
    cout << "   3. Template with conversion\n";
    cout << "   Regular functions preferred!\n";
}

// ============================================
// EXAMPLE 7: NON-TYPE TEMPLATE PARAMETERS
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
    cout << "  Array[" << Size << "]: ";
    for (int i = 0; i < Size; ++i) {
        cout << arr[i] << " ";
    }
    cout << "\n";
}

void demonstrateNonType() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 7: NON-TYPE PARAMETERS    ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Array with compile-time size ---\n";
    Array<int, 5> arr1;
    arr1[0] = 10;
    arr1[1] = 20;
    cout << "  Array size: " << arr1.size() << "\n";
    cout << "  First element: " << arr1[0] << "\n";

    cout << "\n--- Non-type in function ---\n";
    cout << "  multiply<5>(): " << multiply<5>() << "\n";
    cout << "  multiply<10>(): " << multiply<10>() << "\n";

    cout << "\n--- Array size deduction ---\n";
    int nums[] = {1, 2, 3, 4, 5};
    printArray(nums);

    double values[] = {1.1, 2.2, 3.3};
    printArray(values);

    cout << "\n💡 NON-TYPE PARAMETERS:\n";
    cout << "   • Compile-time constants\n";
    cout << "   • Can be int, enum, pointer\n";
    cout << "   • Size, dimensions, etc.\n";
    cout << "   • Part of type signature\n";
}

// ============================================
// EXAMPLE 8: RETURN TYPE DEDUCTION
// ============================================

// C++11: Trailing return type
template<typename T1, typename T2>
auto addAuto(T1 a, T2 b) -> decltype(a + b) {
    cout << "  Adding with trailing return\n";
    return a + b;
}

// C++14: Full return type deduction
template<typename T1, typename T2>
auto multiply(T1 a, T2 b) {
    cout << "  Multiplying with auto return\n";
    return a * b;
}

void demonstrateReturnDeduction() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 8: RETURN TYPE DEDUCTION  ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Trailing return type ---\n";
    auto r1 = addAuto(10, 20);        // int + int = int
    auto r2 = addAuto(10, 2.5);       // int + double = double
    auto r3 = addAuto(1.5, 2.5);      // double + double = double
    cout << "  Results: " << r1 << ", " << r2 << ", " << r3 << "\n";

    cout << "\n--- Auto return (C++14) ---\n";
    auto r4 = multiply(5, 10);        // int
    auto r5 = multiply(2.5, 4);       // double
    cout << "  Results: " << r4 << ", " << r5 << "\n";

    cout << "\n💡 RETURN TYPE DEDUCTION:\n";
    cout << "   • decltype(expr) gives expression type\n";
    cout << "   • auto return (C++14)\n";
    cout << "   • Compiler figures it out\n";
    cout << "   • Works with mixed types\n";
}

// ============================================
// EXAMPLE 9: TEMPLATE CONSTRAINTS (Concepts Preview)
// ============================================

// Using SFINAE (before C++20 concepts)
template<typename T>
typename enable_if<is_arithmetic<T>::value, T>::type
safeAdd(T a, T b) {
    cout << "  Arithmetic type: safe add\n";
    return a + b;
}

// Check if type is numeric
template<typename T>
void checkType(T value) {
    if (is_integral<T>::value) {
        cout << "  Integral type: " << value << "\n";
    } else if (is_floating_point<T>::value) {
        cout << "  Floating point type: " << value << "\n";
    } else {
        cout << "  Other type\n";
    }
}

void demonstrateConstraints() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 9: CONSTRAINTS (SFINAE)   ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Type constraints with enable_if ---\n";
    cout << "Result: " << safeAdd(10, 20) << "\n";
    cout << "Result: " << safeAdd(3.14, 2.86) << "\n";
    // safeAdd("hello", "world");  // ❌ Error: not arithmetic

    cout << "\n--- Type checking ---\n";
    checkType(42);
    checkType(3.14);
    checkType("Hello");

    cout << "\n💡 CONSTRAINTS:\n";
    cout << "   • Restrict template usage\n";
    cout << "   • SFINAE (Substitution Failure...)\n";
    cout << "   • enable_if helper\n";
    cout << "   • C++20: Concepts (better!)\n";
}

// ============================================
// EXAMPLE 10: REAL-WORLD EXAMPLES
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
void printContainer(const Container& container, const string& name) {
    cout << "  " << name << ": ";
    for (const auto& item : container) {
        cout << item << " ";
    }
    cout << "\n";
}

// Generic find
template<typename Container, typename Value>
bool contains(const Container& container, const Value& value) {
    for (const auto& item : container) {
        if (item == value) return true;
    }
    return false;
}

// Generic accumulate
template<typename Iterator, typename T>
T accumulate(Iterator begin, Iterator end, T init) {
    T result = init;
    for (auto it = begin; it != end; ++it) {
        result += *it;
    }
    return result;
}

void demonstrateRealWorld() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 10: REAL-WORLD USAGE      ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Generic swap ---\n";
    int x = 10, y = 20;
    cout << "  Before: x=" << x << ", y=" << y << "\n";
    mySwap(x, y);
    cout << "  After: x=" << x << ", y=" << y << "\n";

    string s1 = "Hello", s2 = "World";
    mySwap(s1, s2);
    cout << "  Strings: " << s1 << ", " << s2 << "\n";

    cout << "\n--- Generic print ---\n";
    vector<int> numbers{1, 2, 3, 4, 5};
    vector<string> words{"apple", "banana", "cherry"};
    printContainer(numbers, "Numbers");
    printContainer(words, "Words");

    cout << "\n--- Generic find ---\n";
    cout << "  Contains 3? " << (contains(numbers, 3) ? "Yes" : "No") << "\n";
    cout << "  Contains 10? " << (contains(numbers, 10) ? "Yes" : "No") << "\n";

    cout << "\n--- Generic accumulate ---\n";
    int sum = accumulate(numbers.begin(), numbers.end(), 0);
    cout << "  Sum: " << sum << "\n";

    cout << "\n💡 REAL-WORLD TEMPLATES:\n";
    cout << "   • Work with any type\n";
    cout << "   • Generic algorithms\n";
    cout << "   • STL is built on templates\n";
    cout << "   • Code reuse at its best!\n";
}

// ============================================
// MAIN FUNCTION - RUN ALL EXAMPLES
// ============================================

int main() {
    cout << "\n";
    cout << "╔═══════════════════════════════════════════════════════════╗\n";
    cout << "║                                                           ║\n";
    cout << "║            FUNCTION TEMPLATES IN C++                     ║\n";
    cout << "║            Generic Programming Foundation                 ║\n";
    cout << "║                                                           ║\n";
    cout << "╚═══════════════════════════════════════════════════════════╝\n";

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
    cout << "WHAT ARE FUNCTION TEMPLATES?\n";
    cout << "========================================\n";
    cout << "Function templates:\n";
    cout << "  • Generic functions for any type\n";
    cout << "  • Blueprint for creating functions\n";
    cout << "  • Compiler generates code\n";
    cout << "  • Type-safe generics\n";
    cout << "  • Zero runtime overhead\n";
    cout << "\n";
    cout << "SYNTAX:\n";
    cout << "template<typename T>\n";
    cout << "T maximum(T a, T b) {\n";
    cout << "    return (a > b) ? a : b;\n";
    cout << "}\n";
    cout << "\n";
    cout << "USAGE:\n";
    cout << "int m1 = maximum(10, 20);      // T = int\n";
    cout << "double m2 = maximum(3.14, 2.71); // T = double\n";
    cout << "string m3 = maximum(\"a\", \"b\");  // T = string\n";

    cout << "\n========================================\n";
    cout << "WHY TEMPLATES?\n";
    cout << "========================================\n";
    cout << "WITHOUT TEMPLATES:\n";
    cout << "int maxInt(int a, int b) { return (a > b) ? a : b; }\n";
    cout << "double maxDouble(double a, double b) { return (a > b) ? a : b; }\n";
    cout << "string maxString(string a, string b) { return (a > b) ? a : b; }\n";
    cout << "// Need one for each type! 💥\n";
    cout << "\n";
    cout << "WITH TEMPLATES:\n";
    cout << "template<typename T>\n";
    cout << "T maximum(T a, T b) { return (a > b) ? a : b; }\n";
    cout << "// One function for all types! ✅\n";
    cout << "\n";
    cout << "BENEFITS:\n";
    cout << "  • No code duplication\n";
    cout << "  • Type safety maintained\n";
    cout << "  • Easy to maintain\n";
    cout << "  • Works with any type\n";
    cout << "  • Compiler generates optimal code\n";

    cout << "\n========================================\n";
    cout << "HOW TEMPLATES WORK\n";
    cout << "========================================\n";
    cout << "COMPILATION PROCESS:\n";
    cout << "\n";
    cout << "1. TEMPLATE DEFINITION:\n";
    cout << "   template<typename T>\n";
    cout << "   T add(T a, T b) { return a + b; }\n";
    cout << "   // This is just a blueprint!\n";
    cout << "\n";
    cout << "2. TEMPLATE USAGE:\n";
    cout << "   int r1 = add(5, 10);     // Use with int\n";
    cout << "   double r2 = add(3.14, 2.71); // Use with double\n";
    cout << "\n";
    cout << "3. INSTANTIATION:\n";
    cout << "   Compiler generates:\n";
    cout << "   int add(int a, int b) { return a + b; }\n";
    cout << "   double add(double a, double b) { return a + b; }\n";
    cout << "\n";
    cout << "4. RESULT:\n";
    cout << "   • One template → multiple functions\n";
    cout << "   • Generated at compile time\n";
    cout << "   • Each type gets own function\n";
    cout << "   • Optimized for each type\n";

    cout << "\n========================================\n";
    cout << "TYPE DEDUCTION\n";
    cout << "========================================\n";
    cout << "IMPLICIT DEDUCTION:\n";
    cout << "template<typename T>\n";
    cout << "T max(T a, T b) { return (a > b) ? a : b; }\n";
    cout << "\n";
    cout << "max(10, 20);        // T = int (deduced)\n";
    cout << "max(3.14, 2.71);    // T = double (deduced)\n";
    cout << "max('a', 'z');      // T = char (deduced)\n";
    cout << "\n";
    cout << "EXPLICIT SPECIFICATION:\n";
    cout << "max<int>(10, 20);       // T = int (explicit)\n";
    cout << "max<double>(3.14, 2.71); // T = double (explicit)\n";
    cout << "\n";
    cout << "WHEN NEEDED:\n";
    cout << "  • Ambiguous cases\n";
    cout << "  • Force specific type\n";
    cout << "  • Return type differs from parameter\n";

    cout << "\n========================================\n";
    cout << "MULTIPLE TEMPLATE PARAMETERS\n";
    cout << "========================================\n";
    cout << "template<typename T1, typename T2>\n";
    cout << "void printPair(T1 first, T2 second) {\n";
    cout << "    cout << first << \", \" << second;\n";
    cout << "}\n";
    cout << "\n";
    cout << "USAGE:\n";
    cout << "printPair(42, 3.14);        // T1=int, T2=double\n";
    cout << "printPair(\"Age\", 25);       // T1=const char*, T2=int\n";
    cout << "printPair(true, \"Success\"); // T1=bool, T2=const char*\n";
    cout << "\n";
    cout << "BENEFITS:\n";
    cout << "  • Different types per parameter\n";
    cout << "  • Flexible combinations\n";
    cout << "  • Type deduction still works\n";

    cout << "\n========================================\n";
    cout << "TEMPLATE SPECIALIZATION\n";
    cout << "========================================\n";
    cout << "GENERIC TEMPLATE:\n";
    cout << "template<typename T>\n";
    cout << "T add(T a, T b) {\n";
    cout << "    return a + b;\n";
    cout << "}\n";
    cout << "\n";
    cout << "SPECIALIZATION:\n";
    cout << "template<>\n";
    cout << "string add<string>(string a, string b) {\n";
    cout << "    return a + \" \" + b;  // Custom behavior!\n";
    cout << "}\n";
    cout << "\n";
    cout << "USAGE:\n";
    cout << "add(10, 20);           // Uses generic\n";
    cout << "add(\"Hello\", \"World\"); // Uses specialization\n";
    cout << "\n";
    cout << "WHY SPECIALIZE?\n";
    cout << "  • Custom behavior for specific types\n";
    cout << "  • Type-specific optimizations\n";
    cout << "  • Special handling needed\n";

    cout << "\n========================================\n";
    cout << "OVERLOAD RESOLUTION\n";
    cout << "========================================\n";
    cout << "PRIORITY ORDER:\n";
    cout << "1. Exact match (non-template function)\n";
    cout << "2. Template with exact match\n";
    cout << "3. Template with conversion\n";
    cout << "4. Regular function with conversion\n";
    cout << "\n";
    cout << "EXAMPLE:\n";
    cout << "void func(int x) { }           // Regular\n";
    cout << "template<typename T>\n";
    cout << "void func(T x) { }             // Template\n";
    cout << "\n";
    cout << "func(42);     // Calls regular (exact match)\n";
    cout << "func(3.14);   // Calls template (no int overload)\n";
    cout << "func(\"hi\");   // Calls template (no string overload)\n";

    cout << "\n========================================\n";
    cout << "NON-TYPE TEMPLATE PARAMETERS\n";
    cout << "========================================\n";
    cout << "template<typename T, int Size>\n";
    cout << "class Array {\n";
    cout << "    T data[Size];\n";
    cout << "};\n";
    cout << "\n";
    cout << "USAGE:\n";
    cout << "Array<int, 5> arr1;      // int array, size 5\n";
    cout << "Array<double, 10> arr2;  // double array, size 10\n";
    cout << "\n";
    cout << "ALLOWED TYPES:\n";
    cout << "  • Integral types (int, char, bool)\n";
    cout << "  • Enum types\n";
    cout << "  • Pointers\n";
    cout << "  • References\n";
    cout << "  • Must be compile-time constant!\n";
    cout << "\n";
    cout << "USE CASES:\n";
    cout << "  • Array sizes\n";
    cout << "  • Matrix dimensions\n";
    cout << "  • Buffer sizes\n";
    cout << "  • Optimization parameters\n";

    cout << "\n========================================\n";
    cout << "BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "✅ DO:\n";
    cout << "  • Use meaningful parameter names (T, U, V)\n";
    cout << "  • Prefer typename over class\n";
    cout << "  • Document requirements (T must support <)\n";
    cout << "  • Use const references for large types\n";
    cout << "  • Keep templates simple\n";
    cout << "  • Test with multiple types\n";
    cout << "  • Consider constraints (C++20 concepts)\n";
    cout << "  • Provide good error messages\n";
    cout << "\n";
    cout << "❌ DON'T:\n";
    cout << "  • Over-complicate templates\n";
    cout << "  • Forget to test with various types\n";
    cout << "  • Ignore compilation errors\n";
    cout << "  • Use templates unnecessarily\n";
    cout << "  • Forget const-correctness\n";
    cout << "  • Make templates too general\n";
    cout << "  • Ignore type requirements\n";
    cout << "  • Create circular dependencies\n";

    cout << "\n========================================\n";
    cout << "COMMON PATTERNS\n";
    cout << "========================================\n";
    cout << "PATTERN 1: Generic swap\n";
    cout << "template<typename T>\n";
    cout << "void swap(T& a, T& b) {\n";
    cout << "    T temp = a;\n";
    cout << "    a = b;\n";
    cout << "    b = temp;\n";
    cout << "}\n";
    cout << "\n";
    cout << "PATTERN 2: Generic min/max\n";
    cout << "template<typename T>\n";
    cout << "T min(T a, T b) { return (a < b) ? a : b; }\n";
    cout << "\n";
    cout << "PATTERN 3: Generic print\n";
    cout << "template<typename Container>\n";
    cout << "void print(const Container& c) {\n";
    cout << "    for (const auto& item : c)\n";
    cout << "        cout << item << \" \";\n";
    cout << "}\n";

    cout << "\n========================================\n";
    cout << "QUICK REFERENCE\n";
    cout << "========================================\n";
    cout << "// Basic template\n";
    cout << "template<typename T>\n";
    cout << "T func(T param) { return param; }\n";
    cout << "\n";
    cout << "// Multiple parameters\n";
    cout << "template<typename T1, typename T2>\n";
    cout << "void func(T1 a, T2 b) { }\n";
    cout << "\n";
    cout << "// Non-type parameter\n";
    cout << "template<typename T, int N>\n";
    cout << "void func() { }\n";
    cout << "\n";
    cout << "// Specialization\n";
    cout << "template<>\n";
    cout << "void func<int>(int x) { }\n";
    cout << "\n";
    cout << "// Return type deduction\n";
    cout << "template<typename T1, typename T2>\n";
    cout << "auto func(T1 a, T2 b) -> decltype(a+b) {\n";
    cout << "    return a + b;\n";
    cout << "}\n";

    cout << "\n========================================\n";
    cout << "KEY TAKEAWAYS\n";
    cout << "========================================\n";
    cout << "1. Templates enable generic programming\n";
    cout << "2. Write once, use with many types\n";
    cout << "3. Type safe and zero overhead\n";
    cout << "4. Compiler generates specific functions\n";
    cout << "5. Type deduction is automatic\n";
    cout << "6. Can specialize for specific types\n";
    cout << "7. Regular functions preferred in overload\n";
    cout << "8. Non-type parameters for constants\n";
    cout << "9. Foundation of STL\n";
    cout << "10. Modern C++ is template-heavy\n";

    cout << "\n========================================\n";
    cout << "GOLDEN RULES\n";
    cout << "========================================\n";
    cout << "⚠️  RULE 1: Template is a blueprint, not code\n";
    cout << "   Compiler generates actual functions!\n";
    cout << "   \n";
    cout << "   One template → many functions\n";
    cout << "\n";
    cout << "⚠️  RULE 2: Type requirements matter\n";
    cout << "   T must support operations used!\n";
    cout << "   \n";
    cout << "   max(a, b) needs operator<\n";
    cout << "\n";
    cout << "⚠️  RULE 3: Prefer const references\n";
    cout << "   For efficiency with large types!\n";
    cout << "   \n";
    cout << "   template<typename T>\n";
    cout << "   void func(const T& param)\n";
    cout << "\n";
    cout << "⚠️  RULE 4: Test with multiple types\n";
    cout << "   Ensure it works for all!\n";
    cout << "   \n";
    cout << "   Test int, double, string, custom types\n";
    cout << "\n";
    cout << "⚠️  RULE 5: Regular functions preferred\n";
    cout << "   In overload resolution!\n";
    cout << "   \n";
    cout << "   Exact match beats template\n";

    return 0;
}
