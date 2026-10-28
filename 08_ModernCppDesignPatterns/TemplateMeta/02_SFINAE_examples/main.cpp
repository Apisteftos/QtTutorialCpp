/*
 * SFINAE_examples.cpp
 *
 * SFINAE: Substitution Failure Is Not An Error
 *
 * Difficulty: 🔴 Advanced
 *
 * Key Concepts:
 * - Function overload resolution
 * - enable_if for conditional compilation
 * - Detecting member functions
 * - Tag dispatching
 * - Expression SFINAE
 * - Concepts-like constraints (pre-C++20)
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <iostream>
#include <vector>
#include <type_traits>
#include <string>
#include <print>

// ============================================================================
// PART 1: Basic SFINAE - Function Overloading
// ============================================================================

namespace BasicSFINAE {

// Overload 1: For integral types
template<typename T>
std::enable_if_t<std::is_integral_v<T>, void>
process(T value) {
    std::println("Processing integer: {} * 2 = {}", value, value * 2);
}

// Overload 2: For floating point types
template<typename T>
std::enable_if_t<std::is_floating_point_v<T>, void>
process(T value) {
    std::println("Processing float: {} * 1.5 = {}", value, value * 1.5);
}

// Overload 3: For string types
template<typename T>
std::enable_if_t<std::is_same_v<T, std::string>, void>
process(const T& value) {
    std::println("Processing string: \"{}\" (length: {})", value, value.size());
}

void demonstrate() {
    std::println("");
    std::println("=== Basic SFINAE: Function Overloading ===");
    std::println("");
    std::println("Different implementations based on type:");

    process(42);              // Calls integer version
    process(3.14);            // Calls float version
    process(std::string("Hello"));  // Calls string version

    std::println("");
    std::println("✅ SFINAE selects correct overload at compile-time!");
}
}

// ============================================================================
// PART 2: Detecting Member Functions
// ============================================================================

namespace DetectMembers {

// Detect if type has size() member function
template<typename T, typename = void>
struct has_size : std::false_type {};

template<typename T>
struct has_size<T, std::void_t<decltype(std::declval<T>().size())>>
    : std::true_type {};

template<typename T>
inline constexpr bool has_size_v = has_size<T>::value;

// Detect if type has push_back() member function
template<typename T, typename = void>
struct has_push_back : std::false_type {};

template<typename T>
struct has_push_back<T, std::void_t<decltype(std::declval<T>().push_back(std::declval<typename T::value_type>()))>>
    : std::true_type {};

template<typename T>
inline constexpr bool has_push_back_v = has_push_back<T>::value;

// Detect if type has begin() and end()
template<typename T, typename = void>
struct is_iterable : std::false_type {};

template<typename T>
struct is_iterable<T, std::void_t<
                          decltype(std::declval<T>().begin()),
                          decltype(std::declval<T>().end())
                          >> : std::true_type {};

template<typename T>
inline constexpr bool is_iterable_v = is_iterable<T>::value;

void demonstrate() {
    std::println("");
    std::println("=== Detecting Member Functions ===");

    std::println("");
    std::println("Checking for size():");
    std::println("  std::vector has size(): {}", has_size_v<std::vector<int>>);
    std::println("  int has size(): {}", has_size_v<int>);
    std::println("  std::string has size(): {}", has_size_v<std::string>);

    std::println("");
    std::println("Checking for push_back():");
    std::println("  std::vector has push_back(): {}", has_push_back_v<std::vector<int>>);
    std::println("  std::string has push_back(): {}", has_push_back_v<std::string>);
    std::println("  int has push_back(): {}", has_push_back_v<int>);

    std::println("");
    std::println("Checking if iterable:");
    std::println("  std::vector is iterable: {}", is_iterable_v<std::vector<int>>);
    std::println("  std::string is iterable: {}", is_iterable_v<std::string>);
    std::println("  int is iterable: {}", is_iterable_v<int>);

    std::println("");
    std::println("✅ Can detect interface at compile-time!");
}
}

// ============================================================================
// PART 3: Using Detection for Generic Code
// ============================================================================

namespace GenericContainer {

// Get size if container has size(), otherwise use distance
template<typename Container>
auto getSize(const Container& c)
    -> std::enable_if_t<DetectMembers::has_size_v<Container>, size_t>
{
    return c.size();
}

template<typename Container>
auto getSize(const Container& c)
    -> std::enable_if_t<!DetectMembers::has_size_v<Container> &&
                            DetectMembers::is_iterable_v<Container>, size_t>
{
    return std::distance(c.begin(), c.end());
}

// Print container if iterable
template<typename Container>
std::enable_if_t<DetectMembers::is_iterable_v<Container>, void>
print(const Container& c) {
    std::print("  [ ");
    for (const auto& item : c) {
        std::print("{} ", item);
    }
    std::println("]");
}

void demonstrate() {
    std::println("");
    std::println("=== Generic Container Operations ===");

    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::string str = "Hello";

    std::println("");
    std::println("Get size:");
    std::println("  vector size: {}", getSize(vec));
    std::println("  string size: {}", getSize(str));

    std::println("");
    std::println("Print containers:");
    std::print("  vector: ");
    print(vec);

    std::println("");
    std::println("✅ One function works with any container!");
}
}

// ============================================================================
// PART 4: Return Type SFINAE
// ============================================================================

namespace ReturnTypeSFINAE {

// Returns int for arithmetic types
template<typename T>
auto getValue(T t) -> std::enable_if_t<std::is_arithmetic_v<T>, int> {
    return static_cast<int>(t * 2);
}

// Returns string for string types
template<typename T>
auto getValue(const T& t) -> std::enable_if_t<std::is_same_v<T, std::string>, std::string> {
    return t + " processed";
}

// Returns size for containers (but not strings)
template<typename T>
auto getValue(const T& t) -> std::enable_if_t<
                              DetectMembers::has_size_v<T> && !std::is_same_v<T, std::string>, size_t> {
    return t.size();
}

void demonstrate() {
    std::println("");
    std::println("=== Return Type SFINAE ===");

    std::println("");
    std::println("Different return types based on input:");
    std::println("  getValue(5): {} (int)", getValue(5));
    std::println("  getValue(3.14): {} (int from double)", getValue(3.14));
    std::println("  getValue(\"test\"): {} (string)", getValue(std::string("test")));
    std::println("  getValue(vector): {} (size_t)", getValue(std::vector<int>{1,2,3}));

    std::println("");
    std::println("✅ Return type depends on input type!");
}
}

// ============================================================================
// PART 5: Tag Dispatching
// ============================================================================

namespace TagDispatching {

// Tags
struct integral_tag {};
struct floating_tag {};
struct other_tag {};

// Get tag for type
template<typename T>
struct type_category {
    using type = std::conditional_t<
        std::is_integral_v<T>, integral_tag,
        std::conditional_t<
            std::is_floating_point_v<T>, floating_tag,
            other_tag
            >
        >;
};

// Implementation for integral
template<typename T>
void process_impl(T value, integral_tag) {
    std::println("  Processing integer: {}", value);
}

// Implementation for floating point
template<typename T>
void process_impl(T value, floating_tag) {
    std::println("  Processing float: {}", value);
}

// Implementation for other types
template<typename T>
void process_impl(const T& value, other_tag) {
    std::println("  Processing other type");
    (void)value;
}

// Public interface
template<typename T>
void process(const T& value) {
    process_impl(value, typename type_category<T>::type{});
}

void demonstrate() {
    std::println("");
    std::println("=== Tag Dispatching ===");

    std::println("");
    std::println("Using tags for dispatch:");
    process(42);
    process(3.14);
    process(std::string("hello"));

    std::println("");
    std::println("✅ Tag dispatching = cleaner than SFINAE!");
}
}

// ============================================================================
// PART 6: Expression SFINAE
// ============================================================================

namespace ExpressionSFINAE {

// Check if two types can be added
template<typename T, typename U, typename = void>
struct is_addable : std::false_type {};

template<typename T, typename U>
struct is_addable<T, U, std::void_t<decltype(std::declval<T>() + std::declval<U>())>>
    : std::true_type {};

template<typename T, typename U>
inline constexpr bool is_addable_v = is_addable<T, U>::value;

// Check if type can be printed
template<typename T, typename = void>
struct is_printable : std::false_type {};

template<typename T>
struct is_printable<T, std::void_t<decltype(std::declval<std::ostream&>() << std::declval<T>())>>
    : std::true_type {};

template<typename T>
inline constexpr bool is_printable_v = is_printable<T>::value;

// Safe add - only compiles if types are addable
template<typename T, typename U>
std::enable_if_t<is_addable_v<T, U>, decltype(std::declval<T>() + std::declval<U>())>
safeAdd(T a, U b) {
    return a + b;
}

void demonstrate() {
    std::println("");
    std::println("=== Expression SFINAE ===");

    std::println("");
    std::println("Checking if types are addable:");
    std::println("  int + int: {}", is_addable_v<int, int>);
    std::println("  int + double: {}", is_addable_v<int, double>);
    std::println("  string + string: {}", is_addable_v<std::string, std::string>);
    std::println("  int + string: {}", is_addable_v<int, std::string>);

    std::println("");
    std::println("Checking if types are printable:");
    std::println("  int is printable: {}", is_printable_v<int>);
    std::println("  string is printable: {}", is_printable_v<std::string>);
    std::println("  vector is printable: {}", is_printable_v<std::vector<int>>);

    std::println("");
    std::println("Safe add:");
    std::println("  safeAdd(5, 3): {}", safeAdd(5, 3));
    std::println("  safeAdd(5, 3.14): {}", safeAdd(5, 3.14));

    // This would NOT compile:
    // safeAdd(5, std::string("hello"));  // ❌ Not addable!

    std::println("");
    std::println("✅ Expression SFINAE validates operations!");
}
}

// ============================================================================
// PART 7: Practical Example - Generic Serializer
// ============================================================================

namespace GenericSerializer {

// Serialize arithmetic types
template<typename T>
std::enable_if_t<std::is_arithmetic_v<T>, std::string>
serialize(T value) {
    return std::to_string(value);
}

// Serialize strings
std::string serialize(const std::string& value) {
    return "\"" + value + "\"";
}

// Serialize containers with begin/end
template<typename T>
std::enable_if_t<DetectMembers::is_iterable_v<T> &&
                     !std::is_same_v<T, std::string>, std::string>
serialize(const T& container) {
    std::string result = "[";
    bool first = true;
    for (const auto& item : container) {
        if (!first) result += ", ";
        result += serialize(item);
        first = false;
    }
    result += "]";
    return result;
}

void demonstrate() {
    std::println("");
    std::println("=== Generic Serializer ===");

    int num = 42;
    double pi = 3.14159;
    std::string text = "Hello";
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::vector<std::string> words = {"one", "two", "three"};

    std::println("");
    std::println("Serializing different types:");
    std::println("  int: {}", serialize(num));
    std::println("  double: {}", serialize(pi));
    std::println("  string: {}", serialize(text));
    std::println("  vector<int>: {}", serialize(vec));
    std::println("  vector<string>: {}", serialize(words));

    std::println("");
    std::println("✅ One serialize function for all types!");
}
}

// ============================================================================
// PART 8: SFINAE vs C++20 Concepts
// ============================================================================

namespace SFINAEVsConcepts {

// SFINAE way (C++11/14/17)
template<typename T>
std::enable_if_t<std::is_arithmetic_v<T>, T>
multiply_sfinae(T a, T b) {
    return a * b;
}

// Concepts way (C++20)
template<typename T>
    requires std::is_arithmetic_v<T>
T multiply_concept(T a, T b) {
    return a * b;
}

// Or using concept directly
template<std::integral T>
T add_integers(T a, T b) {
    return a + b;
}

void demonstrate() {
    std::println("");
    std::println("=== SFINAE vs C++20 Concepts ===");

    std::println("");
    std::println("Both work the same:");
    std::println("  SFINAE: multiply(5, 3) = {}", multiply_sfinae(5, 3));
    std::println("  Concept: multiply(5, 3) = {}", multiply_concept(5, 3));
    std::println("  Integral: add(10, 20) = {}", add_integers(10, 20));

    std::println("");
    std::println("✅ Concepts are cleaner but SFINAE still useful!");
    std::println("   SFINAE = C++11+, Concepts = C++20+");
}
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
    std::println("╔═══════════════════════════════════════════════════════╗");
    std::println("║     SFINAE: Substitution Failure Is Not An Error    ║");
    std::println("╚═══════════════════════════════════════════════════════╝");

    BasicSFINAE::demonstrate();
    DetectMembers::demonstrate();
    GenericContainer::demonstrate();
    ReturnTypeSFINAE::demonstrate();
    TagDispatching::demonstrate();
    ExpressionSFINAE::demonstrate();
    GenericSerializer::demonstrate();
    SFINAEVsConcepts::demonstrate();

    std::println("");
    std::println("{}", std::string(60, '='));
    std::println("🎓 SFINAE MASTERY:");
    std::println("{}", std::string(60, '='));
    std::println("{}", std::string(60, '='));
    std::println("");

    return 0;
}

/*
 * COMPILE AND RUN:
 * ===============
 * g++ -std=c++20 -Wall -Wextra SFINAE_examples.cpp -o sfinae
 * ./sfinae
 *
 * KEY SFINAE TECHNIQUES:
 * =====================
 * 1. std::enable_if - Enable/disable functions
 * 2. std::void_t - Detect member functions
 * 3. decltype - Check expressions
 * 4. std::declval - Get type without construction
 * 5. Tag dispatching - Alternative to SFINAE
 *
 * COMMON PATTERNS:
 * ===============
 * // Detect member function:
 * template<typename T, typename = void>
 * struct has_func : std::false_type {};
 *
 * template<typename T>
 * struct has_func<T, std::void_t<decltype(std::declval<T>().func())>>
 *     : std::true_type {};
 *
 * // Enable if condition:
 * template<typename T>
 * std::enable_if_t<condition, ReturnType>
 * function(T arg) { ... }
 *
 * SFINAE VS CONCEPTS:
 * ==================
 * SFINAE (C++11+):
 * - More verbose
 * - Complex syntax
 * - Works everywhere
 *
 * Concepts (C++20+):
 * - Cleaner syntax
 * - Better error messages
 * - More readable
 *
 * WHEN TO USE SFINAE:
 * ==================
 * - Pre-C++20 codebases
 * - Complex type detection
 * - When concepts aren't enough
 * - Library development
 *
 * NEXT STEPS:
 * ===========
 * - Study type_list.cpp for advanced TMP
 * - Learn C++20 concepts (modern alternative)
 * - Practice with your own SFINAE patterns
 * - Read ../../RangesAndGenericStyle/ for real usage
 */
