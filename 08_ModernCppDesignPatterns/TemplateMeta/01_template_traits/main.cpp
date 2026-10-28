/*
 * template_traits.cpp
 *
 * Type Traits and Type Manipulation
 *
 * Difficulty: 🟡 Intermediate
 *
 * Key Concepts:
 * - Standard library type traits
 * - Custom type traits
 * - Type transformations
 * - Compile-time type queries
 * - Practical applications
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <type_traits>
#include <vector>
#include <string>
#include <cstdint>
#include <print>

// ============================================================================
// PART 1: Standard Library Type Traits
// ============================================================================

namespace StandardTraits {

template<typename T>
void analyzeType() {
    std::println("");
    std::println("Analyzing type: {}", typeid(T).name());
    std::println("  Is integral: {}", std::is_integral_v<T>);
    std::println("  Is floating point: {}", std::is_floating_point_v<T>);
    std::println("  Is pointer: {}", std::is_pointer_v<T>);
    std::println("  Is reference: {}", std::is_reference_v<T>);
    std::println("  Is const: {}", std::is_const_v<T>);
    std::println("  Is class: {}", std::is_class_v<T>);
    std::println("  Size: {} bytes", sizeof(T));
}

void demonstrate() {
    std::println("");
    std::println("=== Standard Library Type Traits ===");

    analyzeType<int>();
    analyzeType<double>();
    analyzeType<int*>();
    analyzeType<const int>();
    analyzeType<std::string>();

    // Type comparisons
    std::println("");
    std::println("Type comparisons:");
    std::println("  int == int: {}", std::is_same_v<int, int>);
    std::println("  int == long: {}", std::is_same_v<int, long>);
    std::println("  int == const int: {}", std::is_same_v<int, const int>);
}
}

// ============================================================================
// PART 2: Custom Type Traits
// ============================================================================

namespace CustomTraits {

// Check if type is a pointer
template<typename T>
struct is_pointer {
    static constexpr bool value = false;
};

template<typename T>
struct is_pointer<T*> {
    static constexpr bool value = true;
};

// Helper variable template (C++14)
template<typename T>
inline constexpr bool is_pointer_v = is_pointer<T>::value;

// Check if type is an array
template<typename T>
struct is_array {
    static constexpr bool value = false;
};

template<typename T, size_t N>
struct is_array<T[N]> {
    static constexpr bool value = true;
};

template<typename T>
inline constexpr bool is_array_v = is_array<T>::value;

// Check if type is std::vector
template<typename T>
struct is_vector {
    static constexpr bool value = false;
};

template<typename T>
struct is_vector<std::vector<T>> {
    static constexpr bool value = true;
};

template<typename T>
inline constexpr bool is_vector_v = is_vector<T>::value;

void demonstrate() {
    std::println("");
    std::println("=== Custom Type Traits ===");

    std::println("");
    std::println("Pointer check:");
    std::println("  is_pointer<int*>: {}", is_pointer_v<int*>);
    std::println("  is_pointer<int>: {}", is_pointer_v<int>);

    std::println("");
    std::println("Array check:");
    std::println("  is_array<int[5]>: {}", is_array_v<int[5]>);
    std::println("  is_array<int>: {}", is_array_v<int>);

    std::println("");
    std::println("Vector check:");
    std::println("  is_vector<std::vector<int>>: {}", is_vector_v<std::vector<int>>);
    std::println("  is_vector<int>: {}", is_vector_v<int>);
}
}

// ============================================================================
// PART 3: Type Transformations
// ============================================================================

namespace TypeTransformations {

void demonstrate() {
    std::println("");
    std::println("=== Type Transformations ===");

    // Remove const
    using T1 = const int;
    using T2 = std::remove_const_t<T1>;
    std::println("");
    std::println("Remove const:");
    std::println("  const int -> {}", (std::is_const_v<T2> ? "const" : "non-const"));

    // Remove reference
    using T3 = int&;
    using T4 = std::remove_reference_t<T3>;
    std::println("");
    std::println("Remove reference:");
    std::println("  int& -> {}", (std::is_reference_v<T4> ? "reference" : "non-reference"));

    // Remove pointer
    using T5 = int*;
    using T6 = std::remove_pointer_t<T5>;
    std::println("");
    std::println("Remove pointer:");
    std::println("  int* -> {}", (std::is_pointer_v<T6> ? "pointer" : "non-pointer"));

    // Add const
    using T7 = int;
    using T8 = std::add_const_t<T7>;
    std::println("");
    std::println("Add const:");
    std::println("  int -> {}", (std::is_const_v<T8> ? "const" : "non-const"));

    // Add pointer
    using T9 = int;
    using T10 = std::add_pointer_t<T9>;
    std::println("");
    std::println("Add pointer:");
    std::println("  int -> {}", (std::is_pointer_v<T10> ? "pointer" : "non-pointer"));
}
}

// ============================================================================
// PART 4: Conditional Types
// ============================================================================

namespace ConditionalTypes {

// Choose type based on condition
template<bool Condition, typename TrueType, typename FalseType>
struct conditional {
    using type = TrueType;
};

template<typename TrueType, typename FalseType>
struct conditional<false, TrueType, FalseType> {
    using type = FalseType;
};

template<bool Cond, typename T, typename F>
using conditional_t = typename conditional<Cond, T, F>::type;

// Practical example: Choose storage type based on size
template<size_t Size>
struct OptimalInt {
    using type = conditional_t<
        (Size <= 8),
        uint8_t,
        conditional_t<
            (Size <= 16),
            uint16_t,
            conditional_t<
                (Size <= 32),
                uint32_t,
                uint64_t
                >
            >
        >;
};

void demonstrate() {
    std::println("");
    std::println("=== Conditional Types ===");

    // Choose between int and double based on condition
    using TypeA = std::conditional_t<true, int, double>;
    using TypeB = std::conditional_t<false, int, double>;

    std::println("");
    std::println("Conditional type selection:");
    std::println("  conditional<true, int, double>: {}", (std::is_same_v<TypeA, int> ? "int" : "double"));
    std::println("  conditional<false, int, double>: {}", (std::is_same_v<TypeB, int> ? "int" : "double"));

    // Optimal integer type
    std::println("");
    std::println("Optimal integer types:");
    std::println("  For 5 bits: {} bytes", sizeof(OptimalInt<5>::type));
    std::println("  For 10 bits: {} bytes", sizeof(OptimalInt<10>::type));
    std::println("  For 20 bits: {} bytes", sizeof(OptimalInt<20>::type));
    std::println("  For 40 bits: {} bytes", sizeof(OptimalInt<40>::type));
}
}

// ============================================================================
// PART 5: Enable If Pattern
// ============================================================================

namespace EnableIfPattern {

// Only enable for integral types
template<typename T>
std::enable_if_t<std::is_integral_v<T>, void>
printDouble(T value) {
    std::println("  Integer doubled: {}", value * 2);
}

// Only enable for floating point types
template<typename T>
std::enable_if_t<std::is_floating_point_v<T>, void>
printDouble(T value) {
    std::println("  Float doubled: {}", value * 2.0);
}

// Generic print for arithmetic types
template<typename T>
std::enable_if_t<std::is_arithmetic_v<T>, void>
printType(T value) {
    std::println("  Arithmetic value: {}", value);
}

void demonstrate() {
    std::println("");
    std::println("=== Enable If Pattern ===");

    std::println("");
    std::println("Type-specific overloads:");
    printDouble(5);        // Calls integer version
    printDouble(3.14);     // Calls floating point version

    std::println("");
    std::println("Generic arithmetic:");
    printType(42);
    printType(3.14159);
}
}

// ============================================================================
// PART 6: Decay Type
// ============================================================================

namespace DecayType {

void demonstrate() {
    std::println("");
    std::println("=== Decay Type ===");

    // Decay removes references, const, and converts arrays/functions to pointers
    using T1 = int&;
    using T2 = std::decay_t<T1>;

    using T3 = const int;
    using T4 = std::decay_t<T3>;

    using T5 = int[10];
    using T6 = std::decay_t<T5>;

    std::println("");
    std::println("Decay transformations:");
    std::println("  int& -> {}", (std::is_reference_v<T2> ? "reference" : "value"));
    std::println("  const int -> {}", (std::is_const_v<T4> ? "const" : "non-const"));
    std::println("  int[10] -> {}", (std::is_pointer_v<T6> ? "pointer" : "array"));

    std::println("");
    std::println("✅ Decay useful for template parameters!");
}
}

// ============================================================================
// PART 7: Common Type
// ============================================================================

namespace CommonTypeExample {

void demonstrate() {
    std::println("");
    std::println("=== Common Type ===");

    // Find common type between int and double
    using CommonT = std::common_type_t<int, double, float>;

    std::println("");
    std::println("Common type of int, double, float:");
    std::println("  Size: {} bytes", sizeof(CommonT));
    std::println("  Is double: {}", std::is_same_v<CommonT, double>);

    // Practical use: generic min function
    auto min = [](auto a, auto b) -> std::common_type_t<decltype(a), decltype(b)> {
        return (a < b) ? a : b;
    };

    std::println("");
    std::println("Generic min function:");
    std::println("  min(5, 3.14): {}", min(5, 3.14));
    std::println("  min(2.5, 3): {}", min(2.5, 3));
}
}

// ============================================================================
// PART 8: Underlying Type (for enums)
// ============================================================================

namespace UnderlyingTypeExample {

enum class Color : uint8_t {
    Red, Green, Blue
};

enum class Size : uint32_t {
    Small, Medium, Large
};

void demonstrate() {
    std::println("");
    std::println("=== Underlying Type ===");

    using ColorType = std::underlying_type_t<Color>;
    using SizeType = std::underlying_type_t<Size>;

    std::println("");
    std::println("Enum underlying types:");
    std::println("  Color size: {} bytes", sizeof(ColorType));
    std::println("  Size size: {} bytes", sizeof(SizeType));

    // Convert enum to underlying type
    Color c = Color::Red;
    auto value = static_cast<ColorType>(c);
    std::println("  Color::Red value: {}", static_cast<int>(value));
}
}

// ============================================================================
// PART 9: Practical Application - Type-Safe ID
// ============================================================================

namespace TypeSafeID {

template<typename Tag>
class ID {
private:
    int value_;

public:
    explicit ID(int value) : value_(value) {}

    int get() const { return value_; }

    // Only allow comparison with same tag type
    bool operator==(const ID<Tag>& other) const {
        return value_ == other.value_;
    }
};

// Different ID types (can't mix them up!)
struct UserTag {};
struct ProductTag {};

using UserID = ID<UserTag>;
using ProductID = ID<ProductTag>;

void demonstrate() {
    std::println("");
    std::println("=== Type-Safe ID Pattern ===");

    UserID user(123);
    ProductID product(456);

    std::println("");
    std::println("Type-safe IDs:");
    std::println("  User ID: {}", user.get());
    std::println("  Product ID: {}", product.get());

    // This compiles
    UserID user2(123);
    bool same = (user == user2);
    std::println("  user == user2: {}", same);

    // This would NOT compile (different types):
    // bool wrong = (user == product);  // ❌ Compile error!

    std::println("");
    std::println("✅ Can't accidentally compare UserID with ProductID!");
}
}

// ============================================================================
// PART 10: Practical Application - Generic Serialization
// ============================================================================

namespace GenericSerialization {

template<typename T>
std::enable_if_t<std::is_arithmetic_v<T>, std::string>
serialize(T value) {
    return std::to_string(value);
}

template<typename T>
std::enable_if_t<std::is_same_v<T, std::string>, std::string>
serialize(const T& value) {
    return "\"" + value + "\"";
}

template<typename T>
std::enable_if_t<std::is_pointer_v<T> && std::is_arithmetic_v<std::remove_pointer_t<T>>, std::string>
serialize(T value) {
    return value ? serialize(*value) : "null";
}

void demonstrate() {
    std::println("");
    std::println("=== Generic Serialization ===");

    int num = 42;
    double pi = 3.14159;
    std::string text = "Hello";
    int* ptr = &num;
    int* null_ptr = nullptr;

    std::println("");
    std::println("Serializing different types:");
    std::println("  int: {}", serialize(num));
    std::println("  double: {}", serialize(pi));
    std::println("  string: {}", serialize(text));
    std::println("  pointer: {}", serialize(ptr));
    std::println("  nullptr: {}", serialize(null_ptr));

    std::println("");
    std::println("✅ Different serialization for each type!");
}
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
    std::println("╔═══════════════════════════════════════════════════════╗");
    std::println("║           TYPE TRAITS AND TYPE MANIPULATION         ║");
    std::println("╚═══════════════════════════════════════════════════════╝");

    StandardTraits::demonstrate();
    CustomTraits::demonstrate();
    TypeTransformations::demonstrate();
    ConditionalTypes::demonstrate();
    EnableIfPattern::demonstrate();
    DecayType::demonstrate();
    CommonTypeExample::demonstrate();
    UnderlyingTypeExample::demonstrate();
    TypeSafeID::demonstrate();
    GenericSerialization::demonstrate();

    std::println("");
    std::println("{}", std::string(60, '='));
    std::println("🎓 TYPE TRAITS MASTERY:");
    std::println("{}", std::string(60, '='));
    std::println("{}", std::string(60, '='));
    std::println("");

    return 0;
}

/*
 * COMPILE AND RUN:
 * ===============
 * g++ -std=c++20 -Wall -Wextra template_traits.cpp -o template_traits
 * ./template_traits
 *
 * KEY TYPE TRAITS:
 * ===============
 * - is_same<T, U>           - Check if types are same
 * - is_integral<T>          - Check if integral type
 * - is_floating_point<T>    - Check if floating point
 * - is_pointer<T>           - Check if pointer
 * - is_reference<T>         - Check if reference
 * - is_const<T>             - Check if const
 * - is_class<T>             - Check if class/struct
 *
 * TYPE TRANSFORMATIONS:
 * ====================
 * - remove_const<T>         - Remove const qualifier
 * - remove_reference<T>     - Remove reference
 * - remove_pointer<T>       - Remove pointer
 * - add_const<T>            - Add const qualifier
 * - add_pointer<T>          - Add pointer
 * - decay<T>                - Remove cv and ref, decay arrays
 * - common_type<T, U>       - Find common type
 *
 * PRACTICAL USES:
 * ==============
 * - Type-safe IDs
 * - Generic serialization
 * - Conditional compilation
 * - Template constraints (pre-C++20 concepts)
 * - Zero-cost abstractions
 *
 * NEXT STEPS:
 * ===========
 * - Learn SFINAE_examples.cpp for advanced patterns
 * - Study type_list.cpp for type collections
 * - Explore C++20 concepts (modern alternative)
 */
