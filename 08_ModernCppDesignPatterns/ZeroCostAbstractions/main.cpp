/*
 * zero_cost_abstractions.cpp
 *
 * Zero-Cost Abstractions in Modern C++
 *
 * Demonstrates how high-level abstractions in C++ have ZERO runtime cost
 * when compared to low-level code. The compiler optimizes them away!
 *
 * Philosophy: "What you don't use, you don't pay for. And what you do use,
 *             you couldn't hand code any better." - Bjarne Stroustrup
 *
 * This is a core principle of modern C++ design.
 *
 * Compile with optimizations:
 * g++ -std=c++20 -O3 -Wall -Wextra zero_cost_abstractions.cpp -o zero_cost
 *
 * To see assembly output:
 * g++ -std=c++20 -O3 -S zero_cost_abstractions.cpp -o zero_cost.s
 */

#include <vector>
#include <algorithm>
#include <chrono>
#include <numeric>
#include <ranges>
#include <memory>
#include <print>

// ============================================================================
// EXAMPLE 1: Loop Abstraction - Same Performance
// ============================================================================

namespace LoopAbstraction {

// Low-level: Manual loop (what most people think is "fast")
int sum_manual(const std::vector<int>& vec) {
    int sum = 0;
    for (size_t i = 0; i < vec.size(); ++i) {
        sum += vec[i];
    }
    return sum;
}

// High-level: Range-based for loop (cleaner, SAME performance)
int sum_range_for(const std::vector<int>& vec) {
    int sum = 0;
    for (int num : vec) {
        sum += num;
    }
    return sum;
}

// Highest-level: Algorithm (cleanest, SAME performance)
int sum_algorithm(const std::vector<int>& vec) {
    return std::accumulate(vec.begin(), vec.end(), 0);
}

void demonstrate() {
    std::println("");
    std::println("=== Example 1: Loop Abstraction ===");

    std::vector<int> data(1'000'000);
    std::iota(data.begin(), data.end(), 1);  // Fill with 1, 2, 3, ...

    // Time manual loop
    auto start = std::chrono::high_resolution_clock::now();
    volatile int result1 = sum_manual(data);
    auto end = std::chrono::high_resolution_clock::now();
    auto manual_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    // Time range-for loop
    start = std::chrono::high_resolution_clock::now();
    volatile int result2 = sum_range_for(data);
    end = std::chrono::high_resolution_clock::now();
    auto range_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    // Time algorithm
    start = std::chrono::high_resolution_clock::now();
    volatile int result3 = sum_algorithm(data);
    end = std::chrono::high_resolution_clock::now();
    auto algo_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::println("");
    std::println("Results (all identical): {}", static_cast<int>(result1));
    std::println("");
    std::println("Performance comparison:");
    std::println("  Manual loop:     {} μs", manual_time);
    std::println("  Range-for loop:  {} μs", range_time);
    std::println("  Algorithm:       {} μs", algo_time);
}
}

// ============================================================================
// EXAMPLE 2: Function Objects vs Lambdas - Same Performance
// ============================================================================

namespace FunctionObjects {

// Old-style: Functor (verbose)
struct MultiplyBy {
    int factor;
    MultiplyBy(int f) : factor(f) {}
    int operator()(int x) const { return x * factor; }
};

void demonstrate() {
    std::println("");
    std::println("=== Example 2: Functors vs Lambdas ===");

    std::vector<int> data = {1, 2, 3, 4, 5};
    std::vector<int> result1(5);
    std::vector<int> result2(5);

    // Using functor
    std::transform(data.begin(), data.end(), result1.begin(), MultiplyBy(10));

    // Using lambda (cleaner!)
    std::transform(data.begin(), data.end(), result2.begin(),
                   [](int x) { return x * 10; });

    std::println("");
    std::print("Functor result:  ");
    for (int x : result1) std::print("{} ", x);
    std::println("");
    std::print("Lambda result:   ");
    for (int x : result2) std::print("{} ", x);

    std::println("");
}
}

// ============================================================================
// EXAMPLE 3: Smart Pointers vs Raw Pointers - Same Performance
// ============================================================================

namespace SmartPointers {

// Manual memory management (error-prone, but "fast"?)
int* create_manual() {
    return new int(42);
}

void use_manual(int* ptr) {
    std::println("  Value: {}", *ptr);
    delete ptr;  // Easy to forget!
}

// RAII with unique_ptr (safe, SAME performance!)
std::unique_ptr<int> create_smart() {
    return std::make_unique<int>(42);
}

void use_smart(std::unique_ptr<int> ptr) {
    std::println("  Value: {}", *ptr);
    // Automatically deleted!
}

void demonstrate() {
    std::println("");
    std::println("=== Example 3: Smart Pointers ===");

    std::println("");
    std::println("Raw pointer:");
    int* raw = create_manual();
    use_manual(raw);

    std::println("");
    std::println("Smart pointer:");
    auto smart = create_smart();
    use_smart(std::move(smart));

}
}

// ============================================================================
// EXAMPLE 4: Ranges vs Manual Loops (C++20)
// ============================================================================

namespace RangesExample {

// Manual imperative style
std::vector<int> process_manual(const std::vector<int>& data) {
    std::vector<int> result;
    for (int x : data) {
        if (x % 2 == 0) {           // Filter even numbers
            result.push_back(x * x);  // Square them
        }
    }
    return result;
}

// Functional style with ranges (cleaner, SAME performance!)
auto process_ranges(const std::vector<int>& data) {
    return data
           | std::views::filter([](int x) { return x % 2 == 0; })
           | std::views::transform([](int x) { return x * x; });
}

void demonstrate() {
    std::println("");
    std::println("=== Example 4: Ranges (C++20) ===");

    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    std::println("");
    std::print("Manual loop result: ");
    auto result1 = process_manual(data);
    for (int x : result1) std::print("{} ", x);

    std::println("");
    std::print("Ranges result:      ");
    for (int x : process_ranges(data)) std::print("{} ", x);

    std::println("");
}
}

// ============================================================================
// EXAMPLE 5: Template Abstraction - Compile-Time Polymorphism
// ============================================================================

namespace TemplateAbstraction {

// Runtime polymorphism (virtual functions = overhead)
class ShapeBase {
public:
    virtual double area() const = 0;
    virtual ~ShapeBase() = default;
};

class Circle : public ShapeBase {
    double radius;
public:
    Circle(double r) : radius(r) {}
    double area() const override { return 3.14159 * radius * radius; }
};

// Compile-time polymorphism (templates = ZERO overhead!)
template<typename Shape>
double calculate_area(const Shape& shape) {
    return shape.area();  // No virtual call!
}

struct CircleStatic {
    double radius;
    double area() const { return 3.14159 * radius * radius; }
};

void demonstrate() {
    std::println("");
    std::println("=== Example 5: Template Polymorphism ===");

    // Runtime polymorphism
    std::unique_ptr<ShapeBase> shape1 = std::make_unique<Circle>(5.0);
    std::println("");
    std::print("Virtual function area: {}", shape1->area());

    // Compile-time polymorphism
    CircleStatic shape2{5.0};
    std::println("");
    std::print("Template area:         {}", calculate_area(shape2));

    std::println("");
}
}

// ============================================================================
// EXAMPLE 6: Type Erasure - Abstraction When Needed
// ============================================================================

namespace TypeErasure {

// Sometimes you NEED runtime polymorphism
// Type erasure gives you flexibility with minimal cost

class Drawable {
public:
    template<typename T>
    Drawable(T obj) : self_(std::make_unique<Model<T>>(std::move(obj))) {}

    void draw() const { self_->draw(); }

private:
    struct Concept {
        virtual ~Concept() = default;
        virtual void draw() const = 0;
    };

    template<typename T>
    struct Model : Concept {
        Model(T obj) : object_(std::move(obj)) {}
        void draw() const override { object_.draw(); }
        T object_;
    };

    std::unique_ptr<Concept> self_;
};

struct Circle {
    void draw() const { std::println("Drawing circle"); }
};

struct Square {
    void draw() const { std::println("Drawing square"); }
};

void demonstrate() {
    std::println("");
    std::println("=== Example 6: Type Erasure ===");

    std::vector<Drawable> shapes;
    shapes.push_back(Circle{});
    shapes.push_back(Square{});

    std::println("");
    std::println("Drawing shapes:");
    for (const auto& shape : shapes) {
        shape.draw();
    }

}
}

// ============================================================================
// EXAMPLE 7: Compile-Time Computation
// ============================================================================

namespace CompileTime {

// Runtime computation
int factorial_runtime(int n) {
    return (n <= 1) ? 1 : n * factorial_runtime(n - 1);
}

// Compile-time computation
constexpr int factorial_compiletime(int n) {
    return (n <= 1) ? 1 : n * factorial_compiletime(n - 1);
}

void demonstrate() {
    std::println("");
    std::println("=== Example 7: Compile-Time Computation ===");

    // Computed at runtime
    int runtime_result = factorial_runtime(5);
    std::println("");
    std::print("Runtime factorial(5):     {}", runtime_result);

    // Computed at COMPILE TIME!
    constexpr int compiletime_result = factorial_compiletime(5);
    std::println("");
    std::print("Compile-time factorial(5): {}", compiletime_result);

    std::println("");
}
}

// ============================================================================
// MAIN - Demonstrate All Examples
// ============================================================================

int main() {
    std::println("╔═══════════════════════════════════════════════════════╗");
    std::println("║           ZERO-COST ABSTRACTIONS IN C++               ║");
    std::println("║                                                       ║");
    std::println("║  \"What you don't use, you don't pay for.              ║");
    std::println("║   What you do use, you couldn't hand code better.\"    ║");
    std::println("║                        - Bjarne Stroustrup            ║");
    std::println("╚═══════════════════════════════════════════════════════╝");

    LoopAbstraction::demonstrate();
    FunctionObjects::demonstrate();
    SmartPointers::demonstrate();
    RangesExample::demonstrate();
    TemplateAbstraction::demonstrate();
    TypeErasure::demonstrate();
    CompileTime::demonstrate();

    std::println("");

    return 0;
}
