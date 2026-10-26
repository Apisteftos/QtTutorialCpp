#include <cstddef>
#include <print>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

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
// === EXAMPLE 1: Basic enable_if ===
// ============================================

namespace BasicEnableIf {

// Only enable for integral types
template<typename T,
         typename = std::enable_if_t<std::is_integral_v<T>>>
void print_integral(T value) {
    std::println("  Integral: {}", value);
}

// Only enable for floating point types
template<typename T,
         typename = std::enable_if_t<std::is_floating_point_v<T>>>
void print_floating(T value) {
    std::println("  Floating: {}", value);
}

void demonstrate() {
    std::println("");
    std::println("=== Example 1: Basic enable_if ===");

    std::println("Integral types:");
    print_integral(42);
    print_integral('X');
    print_integral(true);
    // print_integral(3.14);  // ❌ Won't compile - float not integral

    std::println("");
    std::println("Floating types:");
    print_floating(3.14);
    print_floating(2.71f);
    // print_floating(42);  // ❌ Won't compile - int not floating
}
}

// ============================================
// === EXAMPLE 2: Return Type SFINAE ===
// ============================================

namespace ReturnTypeSFINAE {

// Return int for arithmetic types
template<typename T>
std::enable_if_t<std::is_arithmetic_v<T>, int>
process(T value) {
    std::print("  Arithmetic: {} → ", value);
    return static_cast<int>(value * 2);
}

// Return string for string types
template<typename T>
std::enable_if_t<std::is_same_v<T, std::string>, std::string>
process(const T& value) {
    std::print("  String: \"{}\" → ", value);
    return value + " processed";
}

void demonstrate() {
    std::println("");
    std::println("=== Example 2: Return Type SFINAE ===");

    std::println("Process arithmetic:");
    std::println("{}", process(21));
    std::println("{}", process(3.14));

    std::println("");
    std::println("Process string:");
    std::println("\"{}\"", process(std::string("hello")));
}
}

// ============================================
// === EXAMPLE 3: Detecting Member Functions ===
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
    std::println("  Size: {}", container.size());
}

// Fallback for types without size()
template<typename T>
std::enable_if_t<!has_size_v<T>, void>
print_size(const T&) {
    std::println("  No size() method available");
}

void demonstrate() {
    std::println("");
    std::println("=== Example 3: Detecting Member Functions ===");

    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::string str = "hello";
    int num = 42;

    std::println("vector:");
    print_size(vec);

    std::println("");
    std::println("string:");
    print_size(str);

    std::println("");
    std::println("int:");
    print_size(num);
}
}

// ============================================
// === EXAMPLE 4: Detecting Operators ===
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
    std::println("  Type doesn't support operator<");
    return false;
}

// Example type without operator<
struct NoCompare {
    int x;
};

void demonstrate() {
    std::println("");
    std::println("=== Example 4: Detecting Operators ===");

    std::println("Comparing ints:");
    std::println("  5 < 10 = {}", compare(5, 10));

    std::println("");
    std::println("Comparing strings:");
    std::println("  \"abc\" < \"xyz\" = {}", compare(std::string("abc"), std::string("xyz")));

    std::println("");
    std::println("Comparing NoCompare objects:");
    NoCompare a{5}, b{10};
    compare(a, b);
}
}

// ============================================
// === EXAMPLE 5: Multiple Constraints ===
// ============================================

namespace MultipleConstraints {

// Function that requires BOTH conditions
template<typename T>
std::enable_if_t<
    std::is_arithmetic_v<T> &&
        !std::is_same_v<T, bool>,
    void>
print_number(T value) {
    std::println("  Number: {} (arithmetic, not bool)", value);
}

// Detect containers
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
    std::print("  Container of numbers: [ ");
    for (const auto& elem : container) {
        std::print("{} ", elem);
    }
    std::println("]");
}

void demonstrate() {
    std::println("");
    std::println("=== Example 5: Multiple Constraints ===");

    std::println("Print numbers:");
    print_number(42);
    print_number(3.14);
    // print_number(true);  // ❌ Won't compile - bool excluded

    std::println("");
    std::println("Print numeric containers:");
    std::vector<int> nums = {1, 2, 3, 4, 5};
    print_container(nums);

    // std::vector<std::string> strs = {"a", "b"};
    // print_container(strs);  // ❌ Won't compile - not arithmetic
}
}

// ============================================
// === EXAMPLE 6: Function Overloading with SFINAE ===
// ============================================

namespace FunctionOverloading {

// Version 1: For pointers
template<typename T>
std::enable_if_t<std::is_pointer_v<T>, void>
print_value(T ptr) {
    if (ptr) {
        std::println("  Pointer to: {}", *ptr);
    } else {
        std::println("  Null pointer");
    }
}

// Version 2: For arrays
template<typename T, std::size_t N>
void print_value(T (&arr)[N]) {
    std::print("  Array of {} elements: [ ", N);
    for (std::size_t i = 0; i < N; ++i) {
        std::print("{} ", arr[i]);
    }
    std::println("]");
}

// Version 3: For regular values
template<typename T>
std::enable_if_t<!std::is_pointer_v<T>, void>
print_value(T value) {
    std::println("  Value: {}", value);
}

void demonstrate() {
    std::println("");
    std::println("=== Example 6: Function Overloading ===");

    int x = 42;
    int* ptr = &x;
    int* null = nullptr;
    int arr[] = {1, 2, 3, 4, 5};

    std::println("Direct value:");
    print_value(x);

    std::println("");
    std::println("Pointer:");
    print_value(ptr);

    std::println("");
    std::println("Null pointer:");
    print_value(null);

    std::println("");
    std::println("Array:");
    print_value(arr);
}
}

// ============================================
// === EXAMPLE 7: Tag Dispatching Alternative ===
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
    std::println("  Processing integral: {} → {}", value, value * 2);
}

template<typename T>
void process_impl(T value, floating_tag) {
    std::println("  Processing floating: {} → {}", value, value / 2.0);
}

template<typename T>
void process_impl(T, other_tag) {
    std::println("  Processing other type");
}

// Public interface - dispatch to correct implementation
template<typename T>
void process(T value) {
    process_impl(value, tag_t<T>{});
}

void demonstrate() {
    std::println("");
    std::println("=== Example 7: Tag Dispatching ===");

    std::println("Process different types:");
    process(42);
    process(3.14);
    process(std::string("hello"));
}
}

// ============================================
// === EXAMPLE 8: Modern Alternative - if constexpr ===
// ============================================

namespace ModernIfConstexpr {

// Single function with compile-time branching
template<typename T>
void print_info(const T& value) {
    if constexpr (std::is_integral_v<T>) {
        std::println("  Integral: {} (hex: {:x})", value, value);
    }
    else if constexpr (std::is_floating_point_v<T>) {
        std::println("  Floating: {} (double: {})", value, static_cast<double>(value));
    }
    else if constexpr (std::is_same_v<T, std::string>) {
        std::println("  String: \"{}\" (length: {})", value, value.length());
    }
    else {
        std::println("  Other type");
    }
}

void demonstrate() {
    std::println("");
    std::println("=== Example 8: Modern if constexpr (C++17) ===");

    std::println("Much cleaner than SFINAE:");
    print_info(42);
    print_info(3.14);
    print_info(std::string("hello"));
}
}

// ============================================
// === EXAMPLE 9: Real-world - Generic Serializer ===
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
        std::print("  Using custom serialize: ");
        obj.serialize();
    }

    // For arithmetic types
    template<typename T>
    std::enable_if_t<
        !has_serialize_v<T> && std::is_arithmetic_v<T>, void>
    serialize(T value) {
        std::println("  Arithmetic: {}", value);
    }

    // For strings
    template<typename T>
    std::enable_if_t<
        !has_serialize_v<T> && std::is_same_v<T, std::string>, void>
    serialize(const T& str) {
        std::println("  String: \"{}\"", str);
    }

    // For everything else (containers, unknown types)
    template<typename T>
    std::enable_if_t<
        !has_serialize_v<T> &&
            !std::is_arithmetic_v<T> &&
            !std::is_same_v<T, std::string>, void>
    serialize(const T&) {
        std::println("  Container or unknown type");
    }
};

// Example classes
struct Point {
    int x, y;
    void serialize() const {
        std::println("Point({}, {})", x, y);
    }
};

struct Widget {
    std::string name;
    int value;
    void serialize() const {
        std::println("Widget{{\"{}\", {}}}", name, value);
    }
};

struct NoSerialize {
    int data;
};

void demonstrate() {
    std::println("");
    std::println("=== Example 9: Generic Serializer ===");

    Serializer s;

    std::println("Serialize custom types:");
    Point p{10, 20};
    s.serialize(p);

    Widget w{"MyWidget", 42};
    s.serialize(w);

    std::println("");
    std::println("Serialize built-in types:");
    s.serialize(123);
    s.serialize(3.14);
    s.serialize(std::string("hello"));

    std::println("");
    std::println("Serialize type without serialize():");
    NoSerialize ns{99};
    s.serialize(ns);
}
}

// ============================================
// MAIN
// ============================================

int main() {
    std::println("");
    std::println("╔═══════════════════════════════════════════════════════╗");
    std::println("║            SFINAE IN C++                              ║");
    std::println("║                                                       ║");
    std::println("║  Substitution Failure Is Not An Error                ║");
    std::println("║  Compile-time function selection!                    ║");
    std::println("╚═══════════════════════════════════════════════════════╝");

    BasicEnableIf::demonstrate();
    ReturnTypeSFINAE::demonstrate();
    DetectMemberFunction::demonstrate();
    DetectOperators::demonstrate();
    MultipleConstraints::demonstrate();
    FunctionOverloading::demonstrate();
    TagDispatching::demonstrate();
    ModernIfConstexpr::demonstrate();
    GenericSerializer::demonstrate();

    return 0;
}
