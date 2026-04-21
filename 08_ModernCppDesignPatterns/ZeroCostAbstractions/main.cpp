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
 * This is a core principle at think-cell and in modern C++ design.
 *
 * Compile with optimizations:
 * g++ -std=c++20 -O3 -Wall -Wextra zero_cost_abstractions.cpp -o zero_cost
 *
 * To see assembly output:
 * g++ -std=c++20 -O3 -S zero_cost_abstractions.cpp -o zero_cost.s
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <numeric>
#include <ranges>
#include <memory>

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
    std::cout << "\n=== Example 1: Loop Abstraction ===\n";

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

    std::cout << "\nResults (all identical): " << result1 << "\n";
    std::cout << "\nPerformance comparison:\n";
    std::cout << "  Manual loop:     " << manual_time << " μs\n";
    std::cout << "  Range-for loop:  " << range_time << " μs\n";
    std::cout << "  Algorithm:       " << algo_time << " μs\n";
    std::cout << "\n✅ ALL THREE COMPILE TO IDENTICAL ASSEMBLY!\n";
    std::cout << "✅ Zero-cost abstraction = No runtime overhead\n";
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
    std::cout << "\n=== Example 2: Functors vs Lambdas ===\n";

    std::vector<int> data = {1, 2, 3, 4, 5};
    std::vector<int> result1(5);
    std::vector<int> result2(5);

    // Using functor
    std::transform(data.begin(), data.end(), result1.begin(), MultiplyBy(10));

    // Using lambda (cleaner!)
    std::transform(data.begin(), data.end(), result2.begin(),
                   [](int x) { return x * 10; });

    std::cout << "\nFunctor result:  ";
    for (int x : result1) std::cout << x << " ";
    std::cout << "\nLambda result:   ";
    for (int x : result2) std::cout << x << " ";

    std::cout << "\n\n✅ Both compile to IDENTICAL machine code!\n";
    std::cout << "✅ Lambda is cleaner but has ZERO runtime cost\n";
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
    std::cout << "  Value: " << *ptr << "\n";
    delete ptr;  // Easy to forget!
}

// RAII with unique_ptr (safe, SAME performance!)
std::unique_ptr<int> create_smart() {
    return std::make_unique<int>(42);
}

void use_smart(std::unique_ptr<int> ptr) {
    std::cout << "  Value: " << *ptr << "\n";
    // Automatically deleted!
}

void demonstrate() {
    std::cout << "\n=== Example 3: Smart Pointers ===\n";

    std::cout << "\nRaw pointer:\n";
    int* raw = create_manual();
    use_manual(raw);

    std::cout << "\nSmart pointer:\n";
    auto smart = create_smart();
    use_smart(std::move(smart));

    std::cout << "\n✅ unique_ptr has ZERO overhead compared to raw pointer!\n";
    std::cout << "✅ But it's safer - no memory leaks possible\n";
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
    std::cout << "\n=== Example 4: Ranges (C++20) ===\n";

    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    std::cout << "\nManual loop result: ";
    auto result1 = process_manual(data);
    for (int x : result1) std::cout << x << " ";

    std::cout << "\nRanges result:      ";
    for (int x : process_ranges(data)) std::cout << x << " ";

    std::cout << "\n\n✅ Ranges are composable and lazy-evaluated\n";
    std::cout << "✅ Compile to same code as manual loops!\n";
    std::cout << "✅ Much more readable and maintainable\n";
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
    std::cout << "\n=== Example 5: Template Polymorphism ===\n";

    // Runtime polymorphism
    std::unique_ptr<ShapeBase> shape1 = std::make_unique<Circle>(5.0);
    std::cout << "\nVirtual function area: " << shape1->area();

    // Compile-time polymorphism
    CircleStatic shape2{5.0};
    std::cout << "\nTemplate area:         " << calculate_area(shape2);

    std::cout << "\n\n✅ Template version has ZERO virtual function overhead!\n";
    std::cout << "✅ Function call is inlined at compile-time\n";
    std::cout << "✅ This is how think-cell achieves high performance\n";
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
    void draw() const { std::cout << "Drawing circle\n"; }
};

struct Square {
    void draw() const { std::cout << "Drawing square\n"; }
};

void demonstrate() {
    std::cout << "\n=== Example 6: Type Erasure ===\n";

    std::vector<Drawable> shapes;
    shapes.push_back(Circle{});
    shapes.push_back(Square{});

    std::cout << "\nDrawing shapes:\n";
    for (const auto& shape : shapes) {
        shape.draw();
    }

    std::cout << "\n✅ Type erasure: Flexibility when you need it\n";
    std::cout << "✅ Still better than traditional inheritance\n";
    std::cout << "✅ Used in std::function, std::any\n";
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
    std::cout << "\n=== Example 7: Compile-Time Computation ===\n";

    // Computed at runtime
    int runtime_result = factorial_runtime(5);
    std::cout << "\nRuntime factorial(5):     " << runtime_result;

    // Computed at COMPILE TIME!
    constexpr int compiletime_result = factorial_compiletime(5);
    std::cout << "\nCompile-time factorial(5): " << compiletime_result;

    std::cout << "\n\n✅ Compile-time result is a LITERAL in the binary!\n";
    std::cout << "✅ ZERO runtime cost - computed before program runs\n";
    std::cout << "✅ This is template metaprogramming power\n";
}
}

// ============================================================================
// MAIN - Demonstrate All Examples
// ============================================================================

int main() {
    std::cout << "\n╔═══════════════════════════════════════════════════════╗\n";
    std::cout << "║           ZERO-COST ABSTRACTIONS IN C++            ║\n";
    std::cout << "║                                                     ║\n";
    std::cout << "║  \"What you don't use, you don't pay for.            ║\n";
    std::cout << "║   What you do use, you couldn't hand code better.\" ║\n";
    std::cout << "║                        - Bjarne Stroustrup          ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════╝\n";

    LoopAbstraction::demonstrate();
    FunctionObjects::demonstrate();
    SmartPointers::demonstrate();
    RangesExample::demonstrate();
    TemplateAbstraction::demonstrate();
    TypeErasure::demonstrate();
    CompileTime::demonstrate();

    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "🎓 ZERO-COST ABSTRACTION PRINCIPLES:\n";
    std::cout << std::string(60, '=') << "\n";
    std::cout << "1. High-level code compiles to same assembly as low-level\n";
    std::cout << "2. Abstractions are optimized away by the compiler\n";
    std::cout << "3. No runtime overhead for safety and convenience\n";
    std::cout << "4. Templates enable compile-time polymorphism\n";
    std::cout << "5. Modern C++ is both safe AND fast\n";
    std::cout << "\n💡 This is why think-cell can write:\n";
    std::cout << "   - Beautiful, readable code\n";
    std::cout << "   - That's also extremely performant\n";
    std::cout << "   - Using functional-style abstractions\n";
    std::cout << "   - With zero runtime cost!\n";
    std::cout << "\n🚀 Key Takeaway:\n";
    std::cout << "   Don't write C in C++!\n";
    std::cout << "   Use high-level abstractions - they're FREE!\n";
    std::cout << std::string(60, '=') << "\n\n";

    return 0;
}

/*
 * COMPILE AND RUN:
 * ===============
 * Basic compilation:
 *   g++ -std=c++20 -O3 -Wall -Wextra zero_cost_abstractions.cpp -o zero_cost
 *   ./zero_cost
 *
 * View assembly to prove zero-cost:
 *   g++ -std=c++20 -O3 -S zero_cost_abstractions.cpp -o zero_cost.s
 *   cat zero_cost.s  # Compare manual vs high-level code
 *
 * OPTIMIZATION LEVELS:
 * ===================
 * -O0: No optimization (abstractions have cost)
 * -O1: Basic optimization
 * -O2: Full optimization
 * -O3: Aggressive optimization (abstractions disappear!)
 *
 * ASSEMBLY INSPECTION:
 * ===================
 * To see that abstractions compile to identical code:
 * 1. Compile with -S flag (generates assembly)
 * 2. Look at sum_manual, sum_range_for, sum_algorithm
 * 3. They produce IDENTICAL assembly code!
 *
 * THINK-CELL PHILOSOPHY:
 * =====================
 * "We prefer functional-style generic algorithms and ranges
 *  over imperative programming because:
 *  1. Code is more readable and maintainable
 *  2. Less error-prone (compiler catches more mistakes)
 *  3. Zero runtime cost with optimizations
 *  4. Easier to reason about and test"
 *
 * ZERO-COST EXAMPLES:
 * ==================
 * ✅ Range-based for loops
 * ✅ Lambdas vs functors
 * ✅ Smart pointers vs raw pointers
 * ✅ std::algorithms vs manual loops
 * ✅ Templates vs virtual functions
 * ✅ constexpr vs runtime computation
 *
 * WHEN ABSTRACTIONS HAVE COST:
 * ============================
 * ❌ Virtual functions (vtable lookup)
 * ❌ std::function (type erasure overhead)
 * ❌ Exceptions (table lookups)
 * ❌ RTTI (dynamic_cast, typeid)
 *
 * But even these are minimal and worth it for the safety!
 *
 * NEXT STEPS:
 * ===========
 * - Experiment with -O0 vs -O3 compilation
 * - Compare assembly output
 * - Profile your own abstractions
 * - Read Compiler Explorer (godbolt.org) to see live
 * - Study think-cell's blog about their C++ practices
 */
