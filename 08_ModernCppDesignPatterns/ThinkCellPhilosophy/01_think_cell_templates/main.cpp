/*
 * think_cell_templates.cpp
 *
 * Demonstrating think-cell's Template Metaprogramming Philosophy
 *
 * think-cell extensively uses advanced template metaprogramming to achieve:
 * - Zero-cost abstractions
 * - Compile-time type manipulation
 * - Generic, reusable code
 * - Type-safe interfaces
 *
 * Core Principles:
 * - Compute at compile-time when possible
 * - Use templates for type-safe generics
 * - Prefer compile-time polymorphism over runtime
 * - Don't be afraid of advanced techniques when they improve code
 *
 * Compile: g++ -std=c++20 -O3 -Wall -Wextra think_cell_templates.cpp -o tc_templates
 */

#include <iostream>
#include <type_traits>
#include <vector>
#include <string>
#include <concepts>
#include <memory>

// ============================================================================
// EXAMPLE 1: Compile-Time vs Runtime Polymorphism
// ============================================================================

namespace Polymorphism {

// ❌ Runtime polymorphism (virtual functions)
class ShapeRuntime {
public:
    virtual double area() const = 0;
    virtual ~ShapeRuntime() = default;
};

class CircleRuntime : public ShapeRuntime {
    double radius_;
public:
    CircleRuntime(double r) : radius_(r) {}
    double area() const override {
        return 3.14159 * radius_ * radius_;
    }
};

class SquareRuntime : public ShapeRuntime {
    double side_;
public:
    SquareRuntime(double s) : side_(s) {}
    double area() const override {
        return side_ * side_;
    }
};

// ✅ Compile-time polymorphism (templates) - think-cell style
template<typename Shape>
double calculate_area(const Shape& shape) {
    return shape.area();  // Resolved at compile-time!
}

struct CircleStatic {
    double radius;
    double area() const { return 3.14159 * radius * radius; }
};

struct SquareStatic {
    double side;
    double area() const { return side * side; }
};

void demonstrate() {
    std::cout << "\n=== Example 1: Polymorphism Comparison ===\n";

    // Runtime polymorphism
    std::unique_ptr<ShapeRuntime> shape1 =
        std::make_unique<CircleRuntime>(5.0);
    std::cout << "\n❌ Runtime (virtual):  " << shape1->area();

    // Compile-time polymorphism
    CircleStatic shape2{5.0};
    std::cout << "\n✅ Compile-time (template): " << calculate_area(shape2);

    std::cout << "\n\n✅ Template version:\n";
    std::cout << "   - Zero virtual function overhead\n";
    std::cout << "   - Function can be inlined\n";
    std::cout << "   - Errors caught at compile-time\n";
}
}

// ============================================================================
// EXAMPLE 2: Generic Algorithms (Type-Independent Code)
// ============================================================================

namespace GenericAlgorithms {

// ❌ Type-specific implementation
int sum_vector(const std::vector<int>& vec) {
    int sum = 0;
    for (int x : vec) sum += x;
    return sum;
}

double sum_vector_double(const std::vector<double>& vec) {
    double sum = 0.0;
    for (double x : vec) sum += x;
    return sum;
}
// ... need separate function for each type!

// ✅ think-cell style: Generic algorithm (works with ANY container)
template<typename Container>
auto sum_generic(const Container& container) {
    using ValueType = typename Container::value_type;
    ValueType sum{};
    for (const auto& value : container) {
        sum += value;
    }
    return sum;
}

// Even better: With concepts (C++20)
template<std::ranges::range Range>
auto sum_modern(const Range& range) {
    using ValueType = std::ranges::range_value_t<Range>;
    ValueType sum{};
    for (const auto& value : range) {
        sum += value;
    }
    return sum;
}

void demonstrate() {
    std::cout << "\n=== Example 2: Generic Algorithms ===\n";

    std::vector<int> int_data = {1, 2, 3, 4, 5};
    std::vector<double> double_data = {1.5, 2.5, 3.5};

    std::cout << "\n❌ Type-specific functions:\n";
    std::cout << "   int sum: " << sum_vector(int_data) << "\n";
    std::cout << "   double sum: " << sum_vector_double(double_data) << "\n";

    std::cout << "\n✅ Generic algorithm (works with both!):\n";
    std::cout << "   int sum: " << sum_generic(int_data) << "\n";
    std::cout << "   double sum: " << sum_generic(double_data) << "\n";

    std::cout << "\n✅ Benefits:\n";
    std::cout << "   - Write once, use everywhere\n";
    std::cout << "   - Type-safe\n";
    std::cout << "   - Zero runtime overhead\n";
}
}

// ============================================================================
// EXAMPLE 3: Type Traits and SFINAE (Conditional Compilation)
// ============================================================================

namespace TypeTraits {

// Only works with integral types
template<typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type
square(T x) {
    std::cout << "  [Integral version]\n";
    return x * x;
}

// Only works with floating-point types
template<typename T>
typename std::enable_if<std::is_floating_point<T>::value, T>::type
square(T x) {
    std::cout << "  [Floating-point version]\n";
    return x * x;
}

// Modern C++20: Using concepts (cleaner!)
template<std::integral T>
T square_modern(T x) {
    std::cout << "  [Integral version - concepts]\n";
    return x * x;
}

template<std::floating_point T>
T square_modern(T x) {
    std::cout << "  [Floating-point version - concepts]\n";
    return x * x;
}

void demonstrate() {
    std::cout << "\n=== Example 3: Type Traits & SFINAE ===\n";

    std::cout << "\nUsing SFINAE:\n";
    std::cout << "square(5) = " << square(5) << "\n";
    std::cout << "square(5.0) = " << square(5.0) << "\n";

    std::cout << "\nUsing concepts (C++20):\n";
    std::cout << "square(10) = " << square_modern(10) << "\n";
    std::cout << "square(3.14) = " << square_modern(3.14) << "\n";

    // This would NOT compile (caught at compile-time!):
    // square(std::string("hello"));  // ❌ Error!

    std::cout << "\n✅ Different implementations for different types\n";
    std::cout << "✅ Errors caught at compile-time\n";
    std::cout << "✅ Zero runtime overhead\n";
}
}

// ============================================================================
// EXAMPLE 4: Compile-Time Computation
// ============================================================================

namespace CompileTime {

// ❌ Runtime computation
int factorial_runtime(int n) {
    return (n <= 1) ? 1 : n * factorial_runtime(n - 1);
}

// ✅ Compile-time computation
constexpr int factorial_compiletime(int n) {
    return (n <= 1) ? 1 : n * factorial_compiletime(n - 1);
}

// Template metaprogramming (old style, still used)
template<int N>
struct Factorial {
    static constexpr int value = N * Factorial<N - 1>::value;
};

template<>
struct Factorial<0> {
    static constexpr int value = 1;
};

void demonstrate() {
    std::cout << "\n=== Example 4: Compile-Time Computation ===\n";

    // Runtime
    int runtime_result = factorial_runtime(5);
    std::cout << "\n❌ Runtime factorial(5): " << runtime_result;
    std::cout << " (computed when program runs)\n";

    // Compile-time with constexpr
    constexpr int compiletime_result = factorial_compiletime(5);
    std::cout << "\n✅ Compile-time factorial(5): " << compiletime_result;
    std::cout << " (computed at compile-time!)\n";

    // Template metaprogramming
    std::cout << "\n✅ Template metaprogramming factorial(5): "
              << Factorial<5>::value;
    std::cout << " (also compile-time!)\n";

    std::cout << "\n✅ Compile-time computation:\n";
    std::cout << "   - Zero runtime cost\n";
    std::cout << "   - Result is a literal in the binary\n";
    std::cout << "   - Perfect for constants and configurations\n";
}
}

// ============================================================================
// EXAMPLE 5: Type-Safe Units (Compile-Time Dimensional Analysis)
// ============================================================================

namespace TypeSafeUnits {

// Define units as types
template<typename Unit>
struct Quantity {
    double value;

    explicit constexpr Quantity(double v) : value(v) {}

    constexpr Quantity operator+(Quantity other) const {
        return Quantity{value + other.value};
    }

    constexpr Quantity operator*(double scalar) const {
        return Quantity{value * scalar};
    }
};

// Unit types
struct Meter {};
struct Second {};
struct MeterPerSecond {};

using Length = Quantity<Meter>;
using Time = Quantity<Second>;
using Velocity = Quantity<MeterPerSecond>;

// Type-safe operations
constexpr Velocity operator/(Length length, Time time) {
    return Velocity{length.value / time.value};
}

void demonstrate() {
    std::cout << "\n=== Example 5: Type-Safe Units ===\n";

    constexpr Length distance{100.0};  // 100 meters
    constexpr Time duration{10.0};     // 10 seconds

    constexpr Velocity speed = distance / duration;  // Type-safe!

    std::cout << "\nDistance: " << distance.value << " meters\n";
    std::cout << "Time: " << duration.value << " seconds\n";
    std::cout << "Speed: " << speed.value << " m/s\n";

    // This would NOT compile (caught at compile-time!):
    // Length invalid = distance + duration;  // ❌ Can't add meters and seconds!
    // Velocity wrong = distance + speed;     // ❌ Wrong units!

    std::cout << "\n✅ Compile-time unit checking!\n";
    std::cout << "✅ Impossible to mix incompatible units\n";
    std::cout << "✅ Zero runtime overhead\n";
}
}

// ============================================================================
// EXAMPLE 6: Template Specialization (Optimization for Specific Types)
// ============================================================================

namespace TemplateSpecialization {

// Generic implementation
template<typename T>
struct Printer {
    static void print(const T& value) {
        std::cout << value;
    }
};

// Specialized for bool (custom format)
template<>
struct Printer<bool> {
    static void print(bool value) {
        std::cout << (value ? "true" : "false");
    }
};

// Specialized for vectors (custom format)
template<typename T>
struct Printer<std::vector<T>> {
    static void print(const std::vector<T>& vec) {
        std::cout << "[";
        for (size_t i = 0; i < vec.size(); ++i) {
            if (i > 0) std::cout << ", ";
            Printer<T>::print(vec[i]);
        }
        std::cout << "]";
    }
};

void demonstrate() {
    std::cout << "\n=== Example 6: Template Specialization ===\n";

    std::cout << "\nPrinting different types:\n";

    std::cout << "  int: ";
    Printer<int>::print(42);

    std::cout << "\n  bool: ";
    Printer<bool>::print(true);

    std::cout << "\n  vector<int>: ";
    Printer<std::vector<int>>::print({1, 2, 3, 4, 5});

    std::cout << "\n\n✅ Specialized behavior for specific types\n";
    std::cout << "✅ All resolved at compile-time\n";
}
}

// ============================================================================
// EXAMPLE 7: Variadic Templates (Arbitrary Number of Arguments)
// ============================================================================

namespace VariadicTemplates {

// Base case
void print() {
    std::cout << "\n";
}

// Recursive case
template<typename T, typename... Args>
void print(T first, Args... rest) {
    std::cout << first;
    if constexpr (sizeof...(rest) > 0) {
        std::cout << ", ";
        print(rest...);
    } else {
        std::cout << "\n";
    }
}

// Sum with variadic templates
template<typename... Args>
auto sum(Args... args) {
    return (... + args);  // Fold expression (C++17)
}

void demonstrate() {
    std::cout << "\n=== Example 7: Variadic Templates ===\n";

    std::cout << "\nPrint with any number of arguments:\n";
    std::cout << "  ";
    print(1, 2, 3, 4, 5);

    std::cout << "  ";
    print("Hello", "World", "from", "think-cell");

    std::cout << "\nSum with variadic template:\n";
    std::cout << "  sum(1,2,3,4,5) = " << sum(1, 2, 3, 4, 5) << "\n";
    std::cout << "  sum(1.5,2.5,3.0) = " << sum(1.5, 2.5, 3.0) << "\n";

    std::cout << "\n✅ Handle any number of arguments\n";
    std::cout << "✅ Type-safe\n";
    std::cout << "✅ Efficient (inlined)\n";
}
}

// ============================================================================
// EXAMPLE 8: Perfect Forwarding (Avoiding Copies)
// ============================================================================

namespace PerfectForwarding {

struct LargeObject {
    std::vector<int> data;
    LargeObject() : data(1000000, 42) {
        std::cout << "  [LargeObject constructed]\n";
    }
    LargeObject(const LargeObject&) {
        std::cout << "  [LargeObject COPIED - expensive!]\n";
    }
    LargeObject(LargeObject&&) {
        std::cout << "  [LargeObject MOVED - cheap!]\n";
    }
};

// ❌ Takes by value - always copies
template<typename T>
void process_by_value(T obj) {
    // Work with obj
}

// ✅ Perfect forwarding - preserves value category
template<typename T>
void process_perfect(T&& obj) {
    // std::forward preserves lvalue/rvalue-ness
    auto copy = std::forward<T>(obj);
}

void demonstrate() {
    std::cout << "\n=== Example 8: Perfect Forwarding ===\n";

    std::cout << "\n❌ Passing by value:\n";
    LargeObject obj1;
    process_by_value(std::move(obj1));  // Still copies!

    std::cout << "\n✅ Perfect forwarding:\n";
    LargeObject obj2;
    process_perfect(std::move(obj2));   // Moves efficiently!

    std::cout << "\n✅ Perfect forwarding:\n";
    std::cout << "   - Avoids unnecessary copies\n";
    std::cout << "   - Preserves value categories\n";
    std::cout << "   - Essential for high-performance code\n";
}
}

// ============================================================================
// MAIN - Demonstrate All Examples
// ============================================================================

int main() {
    std::cout << "\n╔═══════════════════════════════════════════════════════╗\n";
    std::cout << "║      think-cell TEMPLATE METAPROGRAMMING            ║\n";
    std::cout << "║                                                     ║\n";
    std::cout << "║  \"We're not afraid of advanced template             ║\n";
    std::cout << "║   metaprogramming when it leads to cleaner,         ║\n";
    std::cout << "║   more readable code.\"                              ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════╝\n";

    Polymorphism::demonstrate();
    GenericAlgorithms::demonstrate();
    TypeTraits::demonstrate();
    CompileTime::demonstrate();
    TypeSafeUnits::demonstrate();
    TemplateSpecialization::demonstrate();
    VariadicTemplates::demonstrate();
    PerfectForwarding::demonstrate();

    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "🎓 think-cell TEMPLATE PHILOSOPHY:\n";
    std::cout << std::string(60, '=') << "\n";
    std::cout << "1. Prefer compile-time over runtime when possible\n";
    std::cout << "2. Use templates for generic, reusable code\n";
    std::cout << "3. Zero-cost abstractions through inlining\n";
    std::cout << "4. Type safety without runtime overhead\n";
    std::cout << "5. Advanced techniques when they improve quality\n";
    std::cout << "\n💡 Key Benefits:\n";
    std::cout << "   ✅ Write once, use everywhere\n";
    std::cout << "   ✅ Type-safe - errors at compile-time\n";
    std::cout << "   ✅ Zero runtime overhead\n";
    std::cout << "   ✅ Optimal performance through inlining\n";
    std::cout << "   ✅ Expressive and maintainable\n";
    std::cout << "\n🚀 When think-cell Uses Templates:\n";
    std::cout << "   - Generic algorithms that work with any type\n";
    std::cout << "   - Type-safe interfaces\n";
    std::cout << "   - Compile-time computation\n";
    std::cout << "   - Zero-cost abstractions\n";
    std::cout << "   - Type lists and metafunctions\n";
    std::cout << "\n💭 Remember:\n";
    std::cout << "   \"Don't be afraid of advanced techniques\n";
    std::cout << "    when they make code better.\"\n";
    std::cout << std::string(60, '=') << "\n\n";

    return 0;
}

/*
 * COMPILE AND RUN:
 * ===============
 * g++ -std=c++20 -O3 -Wall -Wextra think_cell_templates.cpp -o tc_templates
 * ./tc_templates
 *
 * REQUIREMENTS:
 * ============
 * - C++20 compiler (for concepts, if constexpr)
 * - Optimization (-O3) to see zero-cost abstractions
 *
 * KEY CONCEPTS DEMONSTRATED:
 * =========================
 * 1. Compile-time vs runtime polymorphism
 * 2. Generic algorithms
 * 3. Type traits and SFINAE
 * 4. Compile-time computation
 * 5. Type-safe units
 * 6. Template specialization
 * 7. Variadic templates
 * 8. Perfect forwarding
 *
 * think-cell PRINCIPLES:
 * =====================
 * ✅ Compute at compile-time when possible
 * ✅ Use templates for generic code
 * ✅ Type safety without runtime cost
 * ✅ Advanced techniques for quality
 * ✅ Zero-cost abstractions
 *
 * WHEN TO USE TEMPLATES:
 * =====================
 * ✅ Generic algorithms
 * ✅ Type-safe interfaces
 * ✅ Compile-time computation
 * ✅ Zero-cost polymorphism
 * ✅ Type manipulation
 *
 * WHEN NOT TO USE TEMPLATES:
 * =========================
 * ❌ When runtime polymorphism is needed
 * ❌ When they hurt readability significantly
 * ❌ For code that doesn't benefit from genericity
 * ❌ When simpler solutions exist
 *
 * NEXT STEPS:
 * ===========
 * - Study ../TemplateMeta/ for advanced patterns
 * - Combine with ranges for powerful abstractions
 * - Practice building generic libraries
 * - Read C++20 concepts documentation
 */
