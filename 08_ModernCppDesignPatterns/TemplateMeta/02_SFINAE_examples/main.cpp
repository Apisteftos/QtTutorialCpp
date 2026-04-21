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
 * Compile: g++ -std=c++20 -Wall -Wextra SFINAE_examples.cpp -o sfinae
 */

#include <iostream>
#include <vector>
#include <type_traits>
#include <string>

// ============================================================================
// PART 1: Basic SFINAE - Function Overloading
// ============================================================================

namespace BasicSFINAE {

// Overload 1: For integral types
template<typename T>
std::enable_if_t<std::is_integral_v<T>, void>
process(T value) {
    std::cout << "Processing integer: " << value << " * 2 = " << value * 2 << "\n";
}

// Overload 2: For floating point types
template<typename T>
std::enable_if_t<std::is_floating_point_v<T>, void>
process(T value) {
    std::cout << "Processing float: " << value << " * 1.5 = " << value * 1.5 << "\n";
}

// Overload 3: For string types
template<typename T>
std::enable_if_t<std::is_same_v<T, std::string>, void>
process(const T& value) {
    std::cout << "Processing string: \"" << value << "\" (length: " << value.size() << ")\n";
}

void demonstrate() {
    std::cout << "\n=== Basic SFINAE: Function Overloading ===\n";
    std::cout << "\nDifferent implementations based on type:\n";

    process(42);              // Calls integer version
    process(3.14);            // Calls float version
    process(std::string("Hello"));  // Calls string version

    std::cout << "\n✅ SFINAE selects correct overload at compile-time!\n";
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
    std::cout << "\n=== Detecting Member Functions ===\n";

    std::cout << "\nChecking for size():\n";
    std::cout << "  std::vector has size(): " << has_size_v<std::vector<int>> << "\n";
    std::cout << "  int has size(): " << has_size_v<int> << "\n";
    std::cout << "  std::string has size(): " << has_size_v<std::string> << "\n";

    std::cout << "\nChecking for push_back():\n";
    std::cout << "  std::vector has push_back(): " << has_push_back_v<std::vector<int>> << "\n";
    std::cout << "  std::string has push_back(): " << has_push_back_v<std::string> << "\n";
    std::cout << "  int has push_back(): " << has_push_back_v<int> << "\n";

    std::cout << "\nChecking if iterable:\n";
    std::cout << "  std::vector is iterable: " << is_iterable_v<std::vector<int>> << "\n";
    std::cout << "  std::string is iterable: " << is_iterable_v<std::string> << "\n";
    std::cout << "  int is iterable: " << is_iterable_v<int> << "\n";

    std::cout << "\n✅ Can detect interface at compile-time!\n";
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
    std::cout << "  [ ";
    for (const auto& item : c) {
        std::cout << item << " ";
    }
    std::cout << "]\n";
}

void demonstrate() {
    std::cout << "\n=== Generic Container Operations ===\n";

    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::string str = "Hello";

    std::cout << "\nGet size:\n";
    std::cout << "  vector size: " << getSize(vec) << "\n";
    std::cout << "  string size: " << getSize(str) << "\n";

    std::cout << "\nPrint containers:\n";
    std::cout << "  vector: ";
    print(vec);

    std::cout << "\n✅ One function works with any container!\n";
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
    std::cout << "\n=== Return Type SFINAE ===\n";

    std::cout << "\nDifferent return types based on input:\n";
    std::cout << "  getValue(5): " << getValue(5) << " (int)\n";
    std::cout << "  getValue(3.14): " << getValue(3.14) << " (int from double)\n";
    std::cout << "  getValue(\"test\"): " << getValue(std::string("test")) << " (string)\n";
    std::cout << "  getValue(vector): " << getValue(std::vector<int>{1,2,3}) << " (size_t)\n";

    std::cout << "\n✅ Return type depends on input type!\n";
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
    std::cout << "  Processing integer: " << value << "\n";
}

// Implementation for floating point
template<typename T>
void process_impl(T value, floating_tag) {
    std::cout << "  Processing float: " << value << "\n";
}

// Implementation for other types
template<typename T>
void process_impl(const T& value, other_tag) {
    std::cout << "  Processing other type\n";
    (void)value;
}

// Public interface
template<typename T>
void process(const T& value) {
    process_impl(value, typename type_category<T>::type{});
}

void demonstrate() {
    std::cout << "\n=== Tag Dispatching ===\n";

    std::cout << "\nUsing tags for dispatch:\n";
    process(42);
    process(3.14);
    process(std::string("hello"));

    std::cout << "\n✅ Tag dispatching = cleaner than SFINAE!\n";
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
    std::cout << "\n=== Expression SFINAE ===\n";

    std::cout << "\nChecking if types are addable:\n";
    std::cout << "  int + int: " << is_addable_v<int, int> << "\n";
    std::cout << "  int + double: " << is_addable_v<int, double> << "\n";
    std::cout << "  string + string: " << is_addable_v<std::string, std::string> << "\n";
    std::cout << "  int + string: " << is_addable_v<int, std::string> << "\n";

    std::cout << "\nChecking if types are printable:\n";
    std::cout << "  int is printable: " << is_printable_v<int> << "\n";
    std::cout << "  string is printable: " << is_printable_v<std::string> << "\n";
    std::cout << "  vector is printable: " << is_printable_v<std::vector<int>> << "\n";

    std::cout << "\nSafe add:\n";
    std::cout << "  safeAdd(5, 3): " << safeAdd(5, 3) << "\n";
    std::cout << "  safeAdd(5, 3.14): " << safeAdd(5, 3.14) << "\n";

    // This would NOT compile:
    // safeAdd(5, std::string("hello"));  // ❌ Not addable!

    std::cout << "\n✅ Expression SFINAE validates operations!\n";
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
    std::cout << "\n=== Generic Serializer ===\n";

    int num = 42;
    double pi = 3.14159;
    std::string text = "Hello";
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::vector<std::string> words = {"one", "two", "three"};

    std::cout << "\nSerializing different types:\n";
    std::cout << "  int: " << serialize(num) << "\n";
    std::cout << "  double: " << serialize(pi) << "\n";
    std::cout << "  string: " << serialize(text) << "\n";
    std::cout << "  vector<int>: " << serialize(vec) << "\n";
    std::cout << "  vector<string>: " << serialize(words) << "\n";

    std::cout << "\n✅ One serialize function for all types!\n";
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
    std::cout << "\n=== SFINAE vs C++20 Concepts ===\n";

    std::cout << "\nBoth work the same:\n";
    std::cout << "  SFINAE: multiply(5, 3) = " << multiply_sfinae(5, 3) << "\n";
    std::cout << "  Concept: multiply(5, 3) = " << multiply_concept(5, 3) << "\n";
    std::cout << "  Integral: add(10, 20) = " << add_integers(10, 20) << "\n";

    std::cout << "\n✅ Concepts are cleaner but SFINAE still useful!\n";
    std::cout << "   SFINAE = C++11+, Concepts = C++20+\n";
}
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
    std::cout << "\n╔═══════════════════════════════════════════════════════╗\n";
    std::cout << "║     SFINAE: Substitution Failure Is Not An Error    ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════╝\n";

    BasicSFINAE::demonstrate();
    DetectMembers::demonstrate();
    GenericContainer::demonstrate();
    ReturnTypeSFINAE::demonstrate();
    TagDispatching::demonstrate();
    ExpressionSFINAE::demonstrate();
    GenericSerializer::demonstrate();
    SFINAEVsConcepts::demonstrate();

    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "🎓 SFINAE MASTERY:\n";
    std::cout << std::string(60, '=') << "\n";
    std::cout << "✅ SFINAE = Compile-time function selection\n";
    std::cout << "✅ Enable/disable functions based on type properties\n";
    std::cout << "✅ Detect member functions and operations\n";
    std::cout << "✅ Create truly generic code\n";
    std::cout << "✅ Better error messages than template errors\n";
    std::cout << "\n💡 Common SFINAE patterns:\n";
    std::cout << "   - std::enable_if for conditional compilation\n";
    std::cout << "   - std::void_t for member detection\n";
    std::cout << "   - Tag dispatching for cleaner code\n";
    std::cout << "   - Expression SFINAE for operation validation\n";
    std::cout << "\n💡 think-cell uses SFINAE for:\n";
    std::cout << "   - Generic algorithm selection\n";
    std::cout << "   - Interface detection\n";
    std::cout << "   - Type-safe APIs\n";
    std::cout << "   - Zero-cost abstractions\n";
    std::cout << "\n🚀 Modern C++20: Use concepts when possible!\n";
    std::cout << "   Concepts are clearer, but SFINAE still relevant\n";
    std::cout << std::string(60, '=') << "\n\n";

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
 * - Read ThinkCellPhilosophy/ for real usage
 */
