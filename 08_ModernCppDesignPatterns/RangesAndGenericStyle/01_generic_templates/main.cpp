/*
 * 01_generic_templates
 *
 * Generic programming with templates: compile-time vs runtime polymorphism,
 * generic algorithms, type traits / SFINAE / concepts, constexpr computation,
 * type-safe units, specialization, variadic templates and perfect forwarding.
 *
 * Compile: g++ -std=c++23 -O3 -Wall -Wextra main.cpp -o example
 */

#include <type_traits>
#include <vector>
#include <string>
#include <concepts>
#include <memory>
#include <ranges>
#include <print>

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

// ✅ Compile-time polymorphism (templates)
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
    std::println("");
    std::println("=== Example 1: Polymorphism Comparison ===");

    // Runtime polymorphism
    std::unique_ptr<ShapeRuntime> shape1 =
        std::make_unique<CircleRuntime>(5.0);
    std::println("");
    std::print("❌ Runtime (virtual):  {}", shape1->area());

    // Compile-time polymorphism
    CircleStatic shape2{5.0};
    std::println("");
    std::print("✅ Compile-time (template): {}", calculate_area(shape2));

    std::println("");
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

// ✅ Generic algorithm (works with ANY container)
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
    std::println("");
    std::println("=== Example 2: Generic Algorithms ===");

    std::vector<int> int_data = {1, 2, 3, 4, 5};
    std::vector<double> double_data = {1.5, 2.5, 3.5};

    std::println("");
    std::println("❌ Type-specific functions:");
    std::println("   int sum: {}", sum_vector(int_data));
    std::println("   double sum: {}", sum_vector_double(double_data));

    std::println("");
    std::println("✅ Generic algorithm (works with both!):");
    std::println("   int sum: {}", sum_generic(int_data));
    std::println("   double sum: {}", sum_generic(double_data));

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
    std::println("  [Integral version]");
    return x * x;
}

// Only works with floating-point types
template<typename T>
typename std::enable_if<std::is_floating_point<T>::value, T>::type
square(T x) {
    std::println("  [Floating-point version]");
    return x * x;
}

// Modern C++20: Using concepts (cleaner!)
template<std::integral T>
T square_modern(T x) {
    std::println("  [Integral version - concepts]");
    return x * x;
}

template<std::floating_point T>
T square_modern(T x) {
    std::println("  [Floating-point version - concepts]");
    return x * x;
}

void demonstrate() {
    std::println("");
    std::println("=== Example 3: Type Traits & SFINAE ===");

    std::println("");
    std::println("Using SFINAE:");
    std::println("square(5) = {}", square(5));
    std::println("square(5.0) = {}", square(5.0));

    std::println("");
    std::println("Using concepts (C++20):");
    std::println("square(10) = {}", square_modern(10));
    std::println("square(3.14) = {}", square_modern(3.14));

    // This would NOT compile (caught at compile-time!):
    // square(std::string("hello"));  // ❌ Error!

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
    std::println("");
    std::println("=== Example 4: Compile-Time Computation ===");

    // Runtime
    int runtime_result = factorial_runtime(5);
    std::println("");
    std::print("❌ Runtime factorial(5): {}", runtime_result);
    std::println(" (computed when program runs)");

    // Compile-time with constexpr
    constexpr int compiletime_result = factorial_compiletime(5);
    std::println("");
    std::print("✅ Compile-time factorial(5): {}", compiletime_result);
    std::println(" (computed at compile-time!)");

    // Template metaprogramming
    std::println("");
    std::print("✅ Template metaprogramming factorial(5): {}", Factorial<5>::value);
    std::println(" (also compile-time!)");

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
    std::println("");
    std::println("=== Example 5: Type-Safe Units ===");

    constexpr Length distance{100.0};  // 100 meters
    constexpr Time duration{10.0};     // 10 seconds

    constexpr Velocity speed = distance / duration;  // Type-safe!

    std::println("");
    std::println("Distance: {} meters", distance.value);
    std::println("Time: {} seconds", duration.value);
    std::println("Speed: {} m/s", speed.value);

    // This would NOT compile (caught at compile-time!):
    // Length invalid = distance + duration;  // ❌ Can't add meters and seconds!
    // Velocity wrong = distance + speed;     // ❌ Wrong units!

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
        std::print("{}", value);
    }
};

// Specialized for bool (custom format)
template<>
struct Printer<bool> {
    static void print(bool value) {
        std::print("{}", (value ? "true" : "false"));
    }
};

// Specialized for vectors (custom format)
template<typename T>
struct Printer<std::vector<T>> {
    static void print(const std::vector<T>& vec) {
        std::print("[");
        for (size_t i = 0; i < vec.size(); ++i) {
            if (i > 0) std::print(", ");
            Printer<T>::print(vec[i]);
        }
        std::print("]");
    }
};

void demonstrate() {
    std::println("");
    std::println("=== Example 6: Template Specialization ===");

    std::println("");
    std::println("Printing different types:");

    std::print("  int: ");
    Printer<int>::print(42);

    std::println("");
    std::print("  bool: ");
    Printer<bool>::print(true);

    std::println("");
    std::print("  vector<int>: ");
    Printer<std::vector<int>>::print({1, 2, 3, 4, 5});

    std::println("");
}
}

// ============================================================================
// EXAMPLE 7: Variadic Templates (Arbitrary Number of Arguments)
// ============================================================================

namespace VariadicTemplates {

// Base case
void print() {
    std::println("");
}

// Recursive case
template<typename T, typename... Args>
void print(T first, Args... rest) {
    std::print("{}", first);
    if constexpr (sizeof...(rest) > 0) {
        std::print(", ");
        print(rest...);
    } else {
        std::println("");
    }
}

// Sum with variadic templates
template<typename... Args>
auto sum(Args... args) {
    return (... + args);  // Fold expression (C++17)
}

void demonstrate() {
    std::println("");
    std::println("=== Example 7: Variadic Templates ===");

    std::println("");
    std::println("Print with any number of arguments:");
    std::print("  ");
    print(1, 2, 3, 4, 5);

    std::print("  ");
    print("Hello", "World", "from", "C++");

    std::println("");
    std::println("Sum with variadic template:");
    std::println("  sum(1,2,3,4,5) = {}", sum(1, 2, 3, 4, 5));
    std::println("  sum(1.5,2.5,3.0) = {}", sum(1.5, 2.5, 3.0));

}
}

// ============================================================================
// EXAMPLE 8: Perfect Forwarding (Avoiding Copies)
// ============================================================================

namespace PerfectForwarding {

struct LargeObject {
    std::vector<int> data;
    LargeObject() : data(1000000, 42) {
        std::println("  [LargeObject constructed]");
    }
    LargeObject(const LargeObject&) {
        std::println("  [LargeObject COPIED - expensive!]");
    }
    LargeObject(LargeObject&&) {
        std::println("  [LargeObject MOVED - cheap!]");
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
    std::println("");
    std::println("=== Example 8: Perfect Forwarding ===");

    std::println("");
    std::println("❌ Passing by value:");
    LargeObject obj1;
    process_by_value(std::move(obj1));  // Still copies!

    std::println("");
    std::println("✅ Perfect forwarding:");
    LargeObject obj2;
    process_perfect(std::move(obj2));   // Moves efficiently!

}
}

// ============================================================================
// MAIN - Demonstrate All Examples
// ============================================================================

int main() {
    std::println("╔═══════════════════════════════════════════════════════╗");
    std::println("║ GENERIC TEMPLATES AND METAPROGRAMMING                 ║");
    std::println("╚═══════════════════════════════════════════════════════╝");

    Polymorphism::demonstrate();
    GenericAlgorithms::demonstrate();
    TypeTraits::demonstrate();
    CompileTime::demonstrate();
    TypeSafeUnits::demonstrate();
    TemplateSpecialization::demonstrate();
    VariadicTemplates::demonstrate();
    PerfectForwarding::demonstrate();

    std::println("");

    return 0;
}
