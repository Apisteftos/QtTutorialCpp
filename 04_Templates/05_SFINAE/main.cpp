#include <iostream>
#include <string>
#include <vector>
#include <type_traits>
using namespace std;

// ============================================
// WHAT IS SFINAE?
// ============================================

/*
SFINAE = "Substitution Failure Is Not An Error"

THE PROBLEM:
    How do you write a template function that only works with certain types?

    template<typename T>
    void func(T value) {
        value.foo();  // ❌ What if T doesn't have foo()?
    }

    func(42);  // ❌ Compiler ERROR! int has no foo()

THE SOLUTION - SFINAE:
    When the compiler tries to substitute template parameters and fails,
    it doesn't immediately ERROR. Instead, it removes that overload from
    consideration and tries other candidates.

    "Substitution failure is NOT an error - just try the next option!"

KEY CONCEPTS:
    1. Template substitution can fail
    2. Failure removes overload, doesn't error
    3. Use std::enable_if to control overloads
    4. Use type traits to detect capabilities
    5. Modern C++: use if constexpr or concepts instead

SYNTAX:
    template<typename T,
             typename = std::enable_if_t<condition>>
    void func(T value) { // ... // }

    // Or return type SFINAE:
    template<typename T>
    std::enable_if_t<condition, ReturnType>
    func(T value) { // ... // }

GOLDEN RULE:
    SFINAE = Compile-time function overload selection based on type properties!

    Modern C++20: Use concepts instead - much cleaner!
*/

// ============================================
// EXAMPLE 1: The Problem Without SFINAE
// ============================================

namespace WithoutSFINAE {

// ❌ This doesn't work - compilation fails if T doesn't have size()
/*
    template<typename T>
    void print_size(const T& container) {
        cout << "Size: " << container.size() << "\n";
    }
    */

// We'd get errors like:
// print_size(42);  // ERROR: int has no member 'size'

void demonstrate() {
    cout << "\n=== Example 1: The Problem ===\n";
    cout << "Without SFINAE, we can't write generic functions\n";
    cout << "that only work with certain types.\n";
    cout << "\n";
    cout << "Problem: How to write print_size() that works with:\n";
    cout << "  ✅ vector<int>  (has size())\n";
    cout << "  ✅ string       (has size())\n";
    cout << "  ❌ int          (no size())\n";
    cout << "  ❌ double       (no size())\n";
    cout << "\n";
    cout << "Solution: SFINAE!\n";
}
}

// ============================================
// EXAMPLE 2: Basic enable_if
// ============================================

namespace BasicEnableIf {

// Only enable for integral types
template<typename T,
         typename = std::enable_if_t<std::is_integral_v<T>>>
void print_integral(T value) {
    cout << "  Integral: " << value << "\n";
}

// Only enable for floating point types
template<typename T,
         typename = std::enable_if_t<std::is_floating_point_v<T>>>
void print_floating(T value) {
    cout << "  Floating: " << value << "\n";
}

void demonstrate() {
    cout << "\n=== Example 2: Basic enable_if ===\n";

    cout << "Integral types:\n";
    print_integral(42);
    print_integral('X');
    print_integral(true);
    // print_integral(3.14);  // ❌ Won't compile - float not integral

    cout << "\nFloating types:\n";
    print_floating(3.14);
    print_floating(2.71f);
    // print_floating(42);  // ❌ Won't compile - int not floating

    cout << "\n✅ SFINAE selects correct overload at compile-time!\n";
}
}

// ============================================
// EXAMPLE 3: Return Type SFINAE
// ============================================

namespace ReturnTypeSFINAE {

// Return int for arithmetic types
template<typename T>
std::enable_if_t<std::is_arithmetic_v<T>, int>
process(T value) {
    cout << "  Arithmetic: " << value << " → ";
    return static_cast<int>(value * 2);
}

// Return string for string types
template<typename T>
std::enable_if_t<std::is_same_v<T, string>, string>
process(const T& value) {
    cout << "  String: \"" << value << "\" → ";
    return value + " processed";
}

void demonstrate() {
    cout << "\n=== Example 3: Return Type SFINAE ===\n";

    cout << "Process arithmetic:\n";
    cout << process(21) << "\n";
    cout << process(3.14) << "\n";

    cout << "\nProcess string:\n";
    cout << "\"" << process(string("hello")) << "\"\n";

    cout << "\n✅ Different return types based on input type!\n";
}
}

// ============================================
// EXAMPLE 4: Detecting Member Functions
// ============================================

namespace DetectMemberFunction {

// SFINAE to detect if T has size() member function
template<typename T, typename = void>
struct has_size : std::false_type {};

template<typename T>
struct has_size<T, std::void_t<decltype(std::declval<T>().size())>>
    : std::true_type {};

template<typename T>
inline constexpr bool has_size_v = has_size<T>::value;

// Use detection to print size if available
template<typename T>
std::enable_if_t<has_size_v<T>, void>
print_size(const T& container) {
    cout << "  Size: " << container.size() << "\n";
}

// Fallback for types without size()
template<typename T>
std::enable_if_t<!has_size_v<T>, void>
print_size(const T& value) {
    cout << "  No size() method available\n";
}

void demonstrate() {
    cout << "\n=== Example 4: Detecting Member Functions ===\n";

    vector<int> vec = {1, 2, 3, 4, 5};
    string str = "hello";
    int num = 42;

    cout << "vector:\n";
    print_size(vec);

    cout << "\nstring:\n";
    print_size(str);

    cout << "\nint:\n";
    print_size(num);

    cout << "\n✅ SFINAE detects capabilities at compile-time!\n";
}
}

// ============================================
// EXAMPLE 5: Detecting Operators
// ============================================

namespace DetectOperators {

// Detect if T supports operator<
template<typename T, typename = void>
struct has_less_than : std::false_type {};

template<typename T>
struct has_less_than<T, std::void_t<
                            decltype(std::declval<T>() < std::declval<T>())>>
    : std::true_type {};

template<typename T>
inline constexpr bool has_less_than_v = has_less_than<T>::value;

// Compare if operator< exists
template<typename T>
std::enable_if_t<has_less_than_v<T>, bool>
compare(const T& a, const T& b) {
    return a < b;
}

// Fallback - can't compare
template<typename T>
std::enable_if_t<!has_less_than_v<T>, bool>
compare(const T&, const T&) {
    cout << "  Type doesn't support operator<\n";
    return false;
}

// Example type without operator<
struct NoCompare {
    int x;
};

void demonstrate() {
    cout << "\n=== Example 5: Detecting Operators ===\n";

    cout << "Comparing ints:\n";
    cout << "  5 < 10 = " << compare(5, 10) << "\n";

    cout << "\nComparing strings:\n";
    cout << "  \"abc\" < \"xyz\" = " << compare(string("abc"), string("xyz")) << "\n";

    cout << "\nComparing NoCompare objects:\n";
    NoCompare a{5}, b{10};
    compare(a, b);

    cout << "\n✅ SFINAE handles missing operators gracefully!\n";
}
}

// ============================================
// EXAMPLE 6: Multiple Constraints
// ============================================

namespace MultipleConstraints {

// Function that requires BOTH conditions
template<typename T>
std::enable_if_t<
    std::is_arithmetic_v<T> &&
        !std::is_same_v<T, bool>,
    void>
print_number(T value) {
    cout << "  Number: " << value << " (arithmetic, not bool)\n";
}

// Another version for containers
template<typename T, typename = void>
struct is_container : std::false_type {};

template<typename T>
struct is_container<T, std::void_t<
                           typename T::value_type,
                           typename T::iterator,
                           decltype(std::declval<T>().begin()),
                           decltype(std::declval<T>().end())>>
    : std::true_type {};

template<typename T>
inline constexpr bool is_container_v = is_container<T>::value;

// Print container elements
template<typename T>
std::enable_if_t<
    is_container_v<T> &&
        std::is_arithmetic_v<typename T::value_type>,
    void>
print_container(const T& container) {
    cout << "  Container of numbers: [ ";
    for (const auto& elem : container) {
        cout << elem << " ";
    }
    cout << "]\n";
}

void demonstrate() {
    cout << "\n=== Example 6: Multiple Constraints ===\n";

    cout << "Print numbers:\n";
    print_number(42);
    print_number(3.14);
    // print_number(true);  // ❌ Won't compile - bool excluded

    cout << "\nPrint numeric containers:\n";
    vector<int> nums = {1, 2, 3, 4, 5};
    print_container(nums);

    // vector<string> strs = {"a", "b"};
    // print_container(strs);  // ❌ Won't compile - not arithmetic

    cout << "\n✅ Multiple conditions combined with &&!\n";
}
}

// ============================================
// EXAMPLE 7: Function Overloading with SFINAE
// ============================================

namespace FunctionOverloading {

// Version 1: For pointers
template<typename T>
std::enable_if_t<std::is_pointer_v<T>, void>
print_value(T ptr) {
    if (ptr) {
        cout << "  Pointer to: " << *ptr << "\n";
    } else {
        cout << "  Null pointer\n";
    }
}

// Version 2: For arrays
template<typename T, size_t N>
void print_value(T (&arr)[N]) {
    cout << "  Array of " << N << " elements: [ ";
    for (size_t i = 0; i < N; ++i) {
        cout << arr[i] << " ";
    }
    cout << "]\n";
}

// Version 3: For regular values
template<typename T>
std::enable_if_t<!std::is_pointer_v<T>, void>
print_value(T value) {
    cout << "  Value: " << value << "\n";
}

void demonstrate() {
    cout << "\n=== Example 7: Function Overloading ===\n";

    int x = 42;
    int* ptr = &x;
    int* null = nullptr;
    int arr[] = {1, 2, 3, 4, 5};

    cout << "Direct value:\n";
    print_value(x);

    cout << "\nPointer:\n";
    print_value(ptr);

    cout << "\nNull pointer:\n";
    print_value(null);

    cout << "\nArray:\n";
    print_value(arr);

    cout << "\n✅ SFINAE selects best overload!\n";
}
}

// ============================================
// EXAMPLE 8: Tag Dispatching Alternative
// ============================================

namespace TagDispatching {

// Tag types
struct integral_tag {};
struct floating_tag {};
struct other_tag {};

// Helper to select tag
template<typename T>
using tag_t = std::conditional_t<
    std::is_integral_v<T>, integral_tag,
    std::conditional_t<
        std::is_floating_point_v<T>, floating_tag,
        other_tag>>;

// Implementation functions
template<typename T>
void process_impl(T value, integral_tag) {
    cout << "  Processing integral: " << value << " → " << value * 2 << "\n";
}

template<typename T>
void process_impl(T value, floating_tag) {
    cout << "  Processing floating: " << value << " → " << value / 2.0 << "\n";
}

template<typename T>
void process_impl(T value, other_tag) {
    cout << "  Processing other type\n";
}

// Public interface - dispatch to correct implementation
template<typename T>
void process(T value) {
    process_impl(value, tag_t<T>{});
}

void demonstrate() {
    cout << "\n=== Example 8: Tag Dispatching ===\n";

    cout << "Process different types:\n";
    process(42);
    process(3.14);
    process(string("hello"));

    cout << "\n✅ Tag dispatching = cleaner alternative to SFINAE!\n";
}
}

// ============================================
// EXAMPLE 9: Modern Alternative - if constexpr
// ============================================

namespace ModernIfConstexpr {

// Single function with compile-time branching
template<typename T>
void print_info(const T& value) {
    if constexpr (std::is_integral_v<T>) {
        cout << "  Integral: " << value
             << " (binary: " << std::hex << value << std::dec << ")\n";
    }
    else if constexpr (std::is_floating_point_v<T>) {
        cout << "  Floating: " << value
             << " (double: " << static_cast<double>(value) << ")\n";
    }
    else if constexpr (std::is_same_v<T, string>) {
        cout << "  String: \"" << value
             << "\" (length: " << value.length() << ")\n";
    }
    else {
        cout << "  Other type\n";
    }
}

void demonstrate() {
    cout << "\n=== Example 9: Modern if constexpr (C++17) ===\n";

    cout << "Much cleaner than SFINAE:\n";
    print_info(42);
    print_info(3.14);
    print_info(string("hello"));

    cout << "\n✅ if constexpr = compile-time if statement!\n";
    cout << "✅ Easier to read and write than SFINAE!\n";
}
}

// ============================================
// EXAMPLE 10: Real-world - Generic Serializer
// ============================================

namespace GenericSerializer {

// Detect if type has serialize() method
template<typename T, typename = void>
struct has_serialize : std::false_type {};

template<typename T>
struct has_serialize<T, std::void_t<
                            decltype(std::declval<T>().serialize())>>
    : std::true_type {};

template<typename T>
inline constexpr bool has_serialize_v = has_serialize<T>::value;

// Serializer class
class Serializer {
public:
    // For types with serialize() method
    template<typename T>
    std::enable_if_t<has_serialize_v<T>, void>
    serialize(const T& obj) {
        cout << "  Using custom serialize: ";
        obj.serialize();
    }

    // For arithmetic types
    template<typename T>
    std::enable_if_t<
        !has_serialize_v<T> && std::is_arithmetic_v<T>, void>
    serialize(T value) {
        cout << "  Arithmetic: " << value << "\n";
    }

    // For strings
    template<typename T>
    std::enable_if_t<
        !has_serialize_v<T> && std::is_same_v<T, string>, void>
    serialize(const T& str) {
        cout << "  String: \"" << str << "\"\n";
    }

    // For containers
    template<typename T>
    std::enable_if_t<
        !has_serialize_v<T> &&
            !std::is_arithmetic_v<T> &&
            !std::is_same_v<T, string>, void>
    serialize(const T&) {
        cout << "  Container or unknown type\n";
    }
};

// Example classes
struct Point {
    int x, y;
    void serialize() const {
        cout << "Point(" << x << ", " << y << ")\n";
    }
};

struct Widget {
    string name;
    int value;
    void serialize() const {
        cout << "Widget{\"" << name << "\", " << value << "}\n";
    }
};

struct NoSerialize {
    int data;
};

void demonstrate() {
    cout << "\n=== Example 10: Generic Serializer ===\n";

    Serializer s;

    cout << "Serialize custom types:\n";
    Point p{10, 20};
    s.serialize(p);

    Widget w{"MyWidget", 42};
    s.serialize(w);

    cout << "\nSerialize built-in types:\n";
    s.serialize(123);
    s.serialize(3.14);
    s.serialize(string("hello"));

    cout << "\nSerialize type without serialize():\n";
    NoSerialize ns{99};
    s.serialize(ns);

    cout << "\n✅ SFINAE selects correct serialization method!\n";
    cout << "✅ Fallback for types without custom method!\n";
}
}

// ============================================
// COMPREHENSIVE OUTPUT
// ============================================

void printComprehensiveInfo() {
    cout << "\n" << string(60, '=') << "\n";
    cout << "SFINAE - COMPLETE REFERENCE\n";
    cout << string(60, '=') << "\n";

    cout << "\n📚 WHAT IS SFINAE?\n";
    cout << string(60, '-') << "\n";
    cout << "SFINAE = Substitution Failure Is Not An Error\n";
    cout << "\n";
    cout << "When the compiler substitutes template parameters:\n";
    cout << "  1. If substitution fails → Remove that overload\n";
    cout << "  2. Try next candidate\n";
    cout << "  3. Error only if NO candidates work\n";
    cout << "\n";
    cout << "Result: Compile-time function selection based on types!\n";

    cout << "\n📊 KEY SYNTAX\n";
    cout << string(60, '-') << "\n";
    cout << "Template parameter SFINAE:\n";
    cout << "  template<typename T,\n";
    cout << "           typename = std::enable_if_t<condition>>\n";
    cout << "  void func(T value) { }\n";
    cout << "\n";
    cout << "Return type SFINAE:\n";
    cout << "  template<typename T>\n";
    cout << "  std::enable_if_t<condition, ReturnType>\n";
    cout << "  func(T value) { }\n";
    cout << "\n";
    cout << "Modern alternatives:\n";
    cout << "  if constexpr (condition) { }    // C++17\n";
    cout << "  requires condition              // C++20 concepts\n";

    cout << "\n🔍 COMMON TYPE TRAITS\n";
    cout << string(60, '-') << "\n";
    cout << "std::is_integral_v<T>         // int, long, char, bool\n";
    cout << "std::is_floating_point_v<T>   // float, double\n";
    cout << "std::is_arithmetic_v<T>       // integral or floating\n";
    cout << "std::is_pointer_v<T>          // T*\n";
    cout << "std::is_same_v<T, U>          // Exact type match\n";
    cout << "std::is_base_of_v<Base, Der>  // Inheritance check\n";
    cout << "std::is_const_v<T>            // const qualified\n";
    cout << "std::is_reference_v<T>        // T& or T&&\n";

    cout << "\n🔧 CUSTOM TYPE TRAITS\n";
    cout << string(60, '-') << "\n";
    cout << "Detect member function:\n";
    cout << "  template<typename T, typename = void>\n";
    cout << "  struct has_foo : std::false_type {};\n";
    cout << "\n";
    cout << "  template<typename T>\n";
    cout << "  struct has_foo<T, std::void_t<\n";
    cout << "      decltype(std::declval<T>().foo())>>\n";
    cout << "      : std::true_type {};\n";

    cout << "\n💡 COMMON PATTERNS\n";
    cout << string(60, '-') << "\n";
    cout << "Pattern 1: Enable/Disable overloads\n";
    cout << "  template<typename T>\n";
    cout << "  enable_if_t<condition, void> func(T) { }\n";
    cout << "\n";
    cout << "Pattern 2: Different return types\n";
    cout << "  enable_if_t<cond1, int> func(T) { }\n";
    cout << "  enable_if_t<cond2, string> func(T) { }\n";
    cout << "\n";
    cout << "Pattern 3: Multiple constraints\n";
    cout << "  enable_if_t<cond1 && cond2 && !cond3, void>\n";
    cout << "\n";
    cout << "Pattern 4: Tag dispatching\n";
    cout << "  func_impl(value, tag_type{});  // Cleaner!\n";

    cout << "\n⚠️  COMMON MISTAKES\n";
    cout << string(60, '-') << "\n";
    cout << "❌ Forgetting typename:\n";
    cout << "   enable_if<...>::type  // ❌ Missing typename!\n";
    cout << "   typename enable_if<...>::type  // ✅\n";
    cout << "   enable_if_t<...>  // ✅ Better (C++14)\n";
    cout << "\n";
    cout << "❌ Ambiguous overloads:\n";
    cout << "   Both conditions true → ambiguous!\n";
    cout << "   Use mutually exclusive conditions\n";
    cout << "\n";
    cout << "❌ Overly complex conditions:\n";
    cout << "   Keep conditions simple and readable\n";
    cout << "   Consider if constexpr or concepts instead\n";

    cout << "\n🎯 BEST PRACTICES\n";
    cout << string(60, '-') << "\n";
    cout << "✅ Prefer modern alternatives:\n";
    cout << "   C++17: Use if constexpr\n";
    cout << "   C++20: Use concepts\n";
    cout << "   C++11/14: Use SFINAE when necessary\n";
    cout << "\n";
    cout << "✅ Use helper traits:\n";
    cout << "   Create has_foo_v instead of repeating logic\n";
    cout << "\n";
    cout << "✅ Document requirements:\n";
    cout << "   Comment what types are expected\n";
    cout << "\n";
    cout << "✅ Keep conditions simple:\n";
    cout << "   Complex conditions = hard to understand\n";
    cout << "\n";
    cout << "✅ Test edge cases:\n";
    cout << "   Test with various types\n";

    cout << "\n⚡ PERFORMANCE\n";
    cout << string(60, '-') << "\n";
    cout << "✅ Zero runtime overhead:\n";
    cout << "  • All resolved at compile-time\n";
    cout << "  • No runtime type checks\n";
    cout << "  • Overload resolution during compilation\n";
    cout << "  • Inline expansion possible\n";
    cout << "\n";
    cout << "⚠️  Compilation cost:\n";
    cout << "  • Complex SFINAE = slower compilation\n";
    cout << "  • Many overloads = longer compile times\n";
    cout << "  • Binary size may increase (templates)\n";

    cout << "\n🚀 C++ EVOLUTION\n";
    cout << string(60, '-') << "\n";
    cout << "C++11: SFINAE with enable_if\n";
    cout << "C++14: enable_if_t helper\n";
    cout << "C++17: if constexpr (simpler!)\n";
    cout << "C++20: Concepts (best option!)\n";
    cout << "\n";
    cout << "Recommendation: Use C++20 concepts when possible!\n";
}

// ============================================
// MAIN
// ============================================

int main() {
    cout << "\n╔═══════════════════════════════════════════════════════╗\n";
    cout << "║            SFINAE IN C++                              ║\n";
    cout << "║                                                       ║\n";
    cout << "║  Substitution Failure Is Not An Error                ║\n";
    cout << "║  Compile-time function selection!                    ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n";

    WithoutSFINAE::demonstrate();
    BasicEnableIf::demonstrate();
    ReturnTypeSFINAE::demonstrate();
    DetectMemberFunction::demonstrate();
    DetectOperators::demonstrate();
    MultipleConstraints::demonstrate();
    FunctionOverloading::demonstrate();
    TagDispatching::demonstrate();
    ModernIfConstexpr::demonstrate();
    GenericSerializer::demonstrate();

    printComprehensiveInfo();

    cout << "\n" << string(60, '=') << "\n";
    cout << "🎓 KEY TAKEAWAYS\n";
    cout << string(60, '=') << "\n";
    cout << "1. SFINAE = compile-time function selection\n";
    cout << "2. enable_if controls overload availability\n";
    cout << "3. Substitution failure removes overload, doesn't error\n";
    cout << "4. Use type traits to detect capabilities\n";
    cout << "5. Custom traits detect members/operators\n";
    cout << "6. Zero runtime overhead (all compile-time)\n";
    cout << "7. C++17: Prefer if constexpr (simpler)\n";
    cout << "8. C++20: Prefer concepts (clearest)\n";
    cout << "9. Tag dispatching = cleaner alternative\n";
    cout << "10. Essential for generic library code\n";
    cout << string(60, '=') << "\n\n";

    return 0;
}
