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
 * Compile: g++ -std=c++20 -Wall -Wextra template_traits.cpp -o template_traits
 */

#include <iostream>
#include <type_traits>
#include <vector>
#include <string>
#include <cstdint>

// ============================================================================
// PART 1: Standard Library Type Traits
// ============================================================================

namespace StandardTraits {

template<typename T>
void analyzeType() {
    std::cout << "\nAnalyzing type: " << typeid(T).name() << "\n";
    std::cout << "  Is integral: " << std::is_integral_v<T> << "\n";
    std::cout << "  Is floating point: " << std::is_floating_point_v<T> << "\n";
    std::cout << "  Is pointer: " << std::is_pointer_v<T> << "\n";
    std::cout << "  Is reference: " << std::is_reference_v<T> << "\n";
    std::cout << "  Is const: " << std::is_const_v<T> << "\n";
    std::cout << "  Is class: " << std::is_class_v<T> << "\n";
    std::cout << "  Size: " << sizeof(T) << " bytes\n";
}

void demonstrate() {
    std::cout << "\n=== Standard Library Type Traits ===\n";

    analyzeType<int>();
    analyzeType<double>();
    analyzeType<int*>();
    analyzeType<const int>();
    analyzeType<std::string>();

    // Type comparisons
    std::cout << "\nType comparisons:\n";
    std::cout << "  int == int: " << std::is_same_v<int, int> << "\n";
    std::cout << "  int == long: " << std::is_same_v<int, long> << "\n";
    std::cout << "  int == const int: " << std::is_same_v<int, const int> << "\n";
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
    std::cout << "\n=== Custom Type Traits ===\n";

    std::cout << "\nPointer check:\n";
    std::cout << "  is_pointer<int*>: " << is_pointer_v<int*> << "\n";
    std::cout << "  is_pointer<int>: " << is_pointer_v<int> << "\n";

    std::cout << "\nArray check:\n";
    std::cout << "  is_array<int[5]>: " << is_array_v<int[5]> << "\n";
    std::cout << "  is_array<int>: " << is_array_v<int> << "\n";

    std::cout << "\nVector check:\n";
    std::cout << "  is_vector<std::vector<int>>: " << is_vector_v<std::vector<int>> << "\n";
    std::cout << "  is_vector<int>: " << is_vector_v<int> << "\n";
}
}

// ============================================================================
// PART 3: Type Transformations
// ============================================================================

namespace TypeTransformations {

void demonstrate() {
    std::cout << "\n=== Type Transformations ===\n";

    // Remove const
    using T1 = const int;
    using T2 = std::remove_const_t<T1>;
    std::cout << "\nRemove const:\n";
    std::cout << "  const int -> " << (std::is_const_v<T2> ? "const" : "non-const") << "\n";

    // Remove reference
    using T3 = int&;
    using T4 = std::remove_reference_t<T3>;
    std::cout << "\nRemove reference:\n";
    std::cout << "  int& -> " << (std::is_reference_v<T4> ? "reference" : "non-reference") << "\n";

    // Remove pointer
    using T5 = int*;
    using T6 = std::remove_pointer_t<T5>;
    std::cout << "\nRemove pointer:\n";
    std::cout << "  int* -> " << (std::is_pointer_v<T6> ? "pointer" : "non-pointer") << "\n";

    // Add const
    using T7 = int;
    using T8 = std::add_const_t<T7>;
    std::cout << "\nAdd const:\n";
    std::cout << "  int -> " << (std::is_const_v<T8> ? "const" : "non-const") << "\n";

    // Add pointer
    using T9 = int;
    using T10 = std::add_pointer_t<T9>;
    std::cout << "\nAdd pointer:\n";
    std::cout << "  int -> " << (std::is_pointer_v<T10> ? "pointer" : "non-pointer") << "\n";
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
    std::cout << "\n=== Conditional Types ===\n";

    // Choose between int and double based on condition
    using TypeA = std::conditional_t<true, int, double>;
    using TypeB = std::conditional_t<false, int, double>;

    std::cout << "\nConditional type selection:\n";
    std::cout << "  conditional<true, int, double>: "
              << (std::is_same_v<TypeA, int> ? "int" : "double") << "\n";
    std::cout << "  conditional<false, int, double>: "
              << (std::is_same_v<TypeB, int> ? "int" : "double") << "\n";

    // Optimal integer type
    std::cout << "\nOptimal integer types:\n";
    std::cout << "  For 5 bits: " << sizeof(OptimalInt<5>::type) << " bytes\n";
    std::cout << "  For 10 bits: " << sizeof(OptimalInt<10>::type) << " bytes\n";
    std::cout << "  For 20 bits: " << sizeof(OptimalInt<20>::type) << " bytes\n";
    std::cout << "  For 40 bits: " << sizeof(OptimalInt<40>::type) << " bytes\n";
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
    std::cout << "  Integer doubled: " << value * 2 << "\n";
}

// Only enable for floating point types
template<typename T>
std::enable_if_t<std::is_floating_point_v<T>, void>
printDouble(T value) {
    std::cout << "  Float doubled: " << value * 2.0 << "\n";
}

// Generic print for arithmetic types
template<typename T>
std::enable_if_t<std::is_arithmetic_v<T>, void>
printType(T value) {
    std::cout << "  Arithmetic value: " << value << "\n";
}

void demonstrate() {
    std::cout << "\n=== Enable If Pattern ===\n";

    std::cout << "\nType-specific overloads:\n";
    printDouble(5);        // Calls integer version
    printDouble(3.14);     // Calls floating point version

    std::cout << "\nGeneric arithmetic:\n";
    printType(42);
    printType(3.14159);
}
}

// ============================================================================
// PART 6: Decay Type
// ============================================================================

namespace DecayType {

void demonstrate() {
    std::cout << "\n=== Decay Type ===\n";

    // Decay removes references, const, and converts arrays/functions to pointers
    using T1 = int&;
    using T2 = std::decay_t<T1>;

    using T3 = const int;
    using T4 = std::decay_t<T3>;

    using T5 = int[10];
    using T6 = std::decay_t<T5>;

    std::cout << "\nDecay transformations:\n";
    std::cout << "  int& -> " << (std::is_reference_v<T2> ? "reference" : "value") << "\n";
    std::cout << "  const int -> " << (std::is_const_v<T4> ? "const" : "non-const") << "\n";
    std::cout << "  int[10] -> " << (std::is_pointer_v<T6> ? "pointer" : "array") << "\n";

    std::cout << "\n✅ Decay useful for template parameters!\n";
}
}

// ============================================================================
// PART 7: Common Type
// ============================================================================

namespace CommonTypeExample {

void demonstrate() {
    std::cout << "\n=== Common Type ===\n";

    // Find common type between int and double
    using CommonT = std::common_type_t<int, double, float>;

    std::cout << "\nCommon type of int, double, float:\n";
    std::cout << "  Size: " << sizeof(CommonT) << " bytes\n";
    std::cout << "  Is double: " << std::is_same_v<CommonT, double> << "\n";

    // Practical use: generic min function
    auto min = [](auto a, auto b) -> std::common_type_t<decltype(a), decltype(b)> {
        return (a < b) ? a : b;
    };

    std::cout << "\nGeneric min function:\n";
    std::cout << "  min(5, 3.14): " << min(5, 3.14) << "\n";
    std::cout << "  min(2.5, 3): " << min(2.5, 3) << "\n";
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
    std::cout << "\n=== Underlying Type ===\n";

    using ColorType = std::underlying_type_t<Color>;
    using SizeType = std::underlying_type_t<Size>;

    std::cout << "\nEnum underlying types:\n";
    std::cout << "  Color size: " << sizeof(ColorType) << " bytes\n";
    std::cout << "  Size size: " << sizeof(SizeType) << " bytes\n";

    // Convert enum to underlying type
    Color c = Color::Red;
    auto value = static_cast<ColorType>(c);
    std::cout << "  Color::Red value: " << static_cast<int>(value) << "\n";
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
    std::cout << "\n=== Type-Safe ID Pattern ===\n";

    UserID user(123);
    ProductID product(456);

    std::cout << "\nType-safe IDs:\n";
    std::cout << "  User ID: " << user.get() << "\n";
    std::cout << "  Product ID: " << product.get() << "\n";

    // This compiles
    UserID user2(123);
    bool same = (user == user2);
    std::cout << "  user == user2: " << same << "\n";

    // This would NOT compile (different types):
    // bool wrong = (user == product);  // ❌ Compile error!

    std::cout << "\n✅ Can't accidentally compare UserID with ProductID!\n";
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
    std::cout << "\n=== Generic Serialization ===\n";

    int num = 42;
    double pi = 3.14159;
    std::string text = "Hello";
    int* ptr = &num;
    int* null_ptr = nullptr;

    std::cout << "\nSerializing different types:\n";
    std::cout << "  int: " << serialize(num) << "\n";
    std::cout << "  double: " << serialize(pi) << "\n";
    std::cout << "  string: " << serialize(text) << "\n";
    std::cout << "  pointer: " << serialize(ptr) << "\n";
    std::cout << "  nullptr: " << serialize(null_ptr) << "\n";

    std::cout << "\n✅ Different serialization for each type!\n";
}
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
    std::cout << "\n╔═══════════════════════════════════════════════════════╗\n";
    std::cout << "║           TYPE TRAITS AND TYPE MANIPULATION         ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════╝\n";

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

    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "🎓 TYPE TRAITS MASTERY:\n";
    std::cout << std::string(60, '=') << "\n";
    std::cout << "✅ Use type traits to query type properties\n";
    std::cout << "✅ Transform types at compile-time\n";
    std::cout << "✅ Enable/disable code based on types\n";
    std::cout << "✅ Create type-safe abstractions\n";
    std::cout << "✅ Build generic, reusable code\n";
    std::cout << "\n💡 think-cell uses type traits for:\n";
    std::cout << "   - Type-safe interfaces\n";
    std::cout << "   - Generic algorithm selection\n";
    std::cout << "   - Compile-time optimizations\n";
    std::cout << "   - Zero-cost abstractions\n";
    std::cout << std::string(60, '=') << "\n\n";

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
