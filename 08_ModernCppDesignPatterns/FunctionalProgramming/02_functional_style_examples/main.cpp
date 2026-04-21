/*
 * functional_style_examples.cpp
 *
 * Core Functional Programming Concepts in C++
 *
 * Difficulty: 🟢 Beginner
 *
 * Key Concepts:
 * - Pure Functions
 * - Immutability
 * - Higher-Order Functions
 * - Function Composition
 * - Currying
 * - First-Class Functions
 *
 * Compile: g++ -std=c++20 -Wall -Wextra functional_style_examples.cpp -o functional_style
 */

#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <string>
#include <ranges>

// ============================================================================
// CONCEPT 1: PURE FUNCTIONS
// ============================================================================
// A pure function:
// 1. Always returns the same output for the same input
// 2. Has no side effects (doesn't modify external state)

namespace PureFunctions {

// ✅ PURE - Always returns same result, no side effects
int add(int a, int b) {
    return a + b;
}

// ✅ PURE - No external state modification
int square(int x) {
    return x * x;
}

// ❌ IMPURE - Modifies external state (global variable)
int counter = 0;
int addAndCount(int a, int b) {
    counter++;  // Side effect!
    return a + b;
}

// ❌ IMPURE - Different results based on external state
int addToCounter(int x) {
    return x + counter;  // Depends on mutable state!
}

// ✅ PURE - Even with complex logic
bool isPrime(int n) {
    if (n <= 1) return false;
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
}

void demonstrate() {
    std::cout << "\n=== Pure Functions ===\n";
    std::cout << "Pure add(5, 3): " << add(5, 3) << " (always 8)\n";
    std::cout << "Pure square(4): " << square(4) << " (always 16)\n";

    std::cout << "\nImpure addAndCount(5, 3): " << addAndCount(5, 3)
              << " (counter now: " << counter << ")\n";
    std::cout << "Impure addAndCount(5, 3): " << addAndCount(5, 3)
              << " (counter now: " << counter << ")\n";
    std::cout << "❌ Different side effects each time!\n";

    std::cout << "\n✅ Benefits of pure functions:\n";
    std::cout << "   - Easy to test\n";
    std::cout << "   - Easy to reason about\n";
    std::cout << "   - Can be memoized (cached)\n";
    std::cout << "   - Thread-safe\n";
}
}

// ============================================================================
// CONCEPT 2: IMMUTABILITY
// ============================================================================
// Prefer const and avoid mutation

namespace Immutability {

// ❌ MUTABLE APPROACH
void modifyInPlace(std::vector<int>& vec) {
    for (auto& n : vec) {
        n *= 2;  // Mutates original!
    }
}

// ✅ IMMUTABLE APPROACH
std::vector<int> createModified(const std::vector<int>& vec) {
    std::vector<int> result;
    std::transform(vec.begin(), vec.end(),
                   std::back_inserter(result),
                   [](int n) { return n * 2; });
    return result;  // Returns new vector, original unchanged
}

// ✅ IMMUTABLE APPROACH (more functional)
auto createModifiedFunctional(const std::vector<int>& vec) {
    return vec | std::views::transform([](int n) { return n * 2; });
}

void demonstrate() {
    std::cout << "\n=== Immutability ===\n";

    std::vector<int> original = {1, 2, 3, 4, 5};
    std::cout << "Original: ";
    for (auto n : original) std::cout << n << " ";

    // Mutable approach
    auto mutable_copy = original;
    modifyInPlace(mutable_copy);
    std::cout << "\nAfter mutable operation: ";
    for (auto n : mutable_copy) std::cout << n << " ";
    std::cout << " (original changed if same object!)";

    // Immutable approach
    auto immutable_result = createModified(original);
    std::cout << "\n\nOriginal (unchanged): ";
    for (auto n : original) std::cout << n << " ";
    std::cout << "\nImmutable result: ";
    for (auto n : immutable_result) std::cout << n << " ";

    std::cout << "\n\n✅ Benefits of immutability:\n";
    std::cout << "   - No unexpected side effects\n";
    std::cout << "   - Thread-safe by default\n";
    std::cout << "   - Easier to reason about\n";
    std::cout << "   - Can safely share data\n";
}
}

// ============================================================================
// CONCEPT 3: HIGHER-ORDER FUNCTIONS
// ============================================================================
// Functions that take functions as parameters or return functions

namespace HigherOrderFunctions {

// Takes a function as parameter
void applyToEach(const std::vector<int>& vec,
                 std::function<void(int)> func) {
    for (const auto& item : vec) {
        func(item);
    }
}

// Returns a function
std::function<int(int)> makeMultiplier(int factor) {
    return [factor](int x) { return x * factor; };
}

// Takes function, returns modified function
auto applyTwice(std::function<int(int)> f) {
    return [f](int x) { return f(f(x)); };
}

// Generic higher-order function
template<typename T, typename Func>
std::vector<T> map(const std::vector<T>& vec, Func func) {
    std::vector<T> result;
    std::transform(vec.begin(), vec.end(),
                   std::back_inserter(result), func);
    return result;
}

template<typename T, typename Pred>
std::vector<T> filter(const std::vector<T>& vec, Pred predicate) {
    std::vector<T> result;
    std::copy_if(vec.begin(), vec.end(),
                 std::back_inserter(result), predicate);
    return result;
}

template<typename T, typename Func>
T reduce(const std::vector<T>& vec, T initial, Func func) {
    return std::accumulate(vec.begin(), vec.end(), initial, func);
}

void demonstrate() {
    std::cout << "\n=== Higher-Order Functions ===\n";

    std::vector<int> numbers = {1, 2, 3, 4, 5};

    // Function as parameter
    std::cout << "Apply function to each: ";
    applyToEach(numbers, [](int x) { std::cout << x * 2 << " "; });

    // Function as return value
    auto multiplyBy3 = makeMultiplier(3);
    std::cout << "\n\nMultiply by 3: " << multiplyBy3(5) << "\n";

    auto multiplyBy10 = makeMultiplier(10);
    std::cout << "Multiply by 10: " << multiplyBy10(5) << "\n";

    // Function transformation
    auto addOne = [](int x) { return x + 1; };
    auto addTwo = applyTwice(addOne);
    std::cout << "\nApply twice (add 1): " << addTwo(5) << " (5+1+1=7)\n";

    // Map, Filter, Reduce
    std::cout << "\nMap (square): ";
    auto squared = map(numbers, [](int x) { return x * x; });
    for (auto n : squared) std::cout << n << " ";

    std::cout << "\nFilter (evens): ";
    auto evens = filter(numbers, [](int x) { return x % 2 == 0; });
    for (auto n : evens) std::cout << n << " ";

    std::cout << "\nReduce (sum): ";
    auto sum = reduce(numbers, 0, [](int a, int b) { return a + b; });
    std::cout << sum << "\n";

    std::cout << "\n✅ Higher-order functions enable:\n";
    std::cout << "   - Abstraction over behavior\n";
    std::cout << "   - Code reuse\n";
    std::cout << "   - Flexible composition\n";
}
}

// ============================================================================
// CONCEPT 4: FUNCTION COMPOSITION
// ============================================================================
// Combining simple functions to create complex ones

namespace FunctionComposition {

// Simple compose for two functions
template<typename F, typename G>
auto compose(F f, G g) {
    return [f, g](auto x) { return f(g(x)); };
}

// Multiple composition using fold
template<typename... Funcs>
auto compose_many(Funcs... funcs) {
    return [funcs...](auto x) {
        return (funcs(x), ...);  // Fold expression
    };
}

void demonstrate() {
    std::cout << "\n=== Function Composition ===\n";

    // Define simple functions
    auto multiplyBy2 = [](int x) { return x * 2; };
    auto add10 = [](int x) { return x + 10; };
    auto square = [](int x) { return x * x; };

    // Compose: multiply then add
    auto multiplyThenAdd = compose(add10, multiplyBy2);
    std::cout << "Compose (add10 ∘ multiply2): 5 → "
              << multiplyThenAdd(5) << " (5*2+10=20)\n";

    // Compose: add then multiply
    auto addThenMultiply = compose(multiplyBy2, add10);
    std::cout << "Compose (multiply2 ∘ add10): 5 → "
              << addThenMultiply(5) << " ((5+10)*2=30)\n";

    // Triple composition
    auto complex = compose(square, compose(add10, multiplyBy2));
    std::cout << "Triple compose (square ∘ add10 ∘ multiply2): 5 → "
              << complex(5) << " ((5*2+10)²=400)\n";

    std::cout << "\n✅ Composition allows:\n";
    std::cout << "   - Building complex from simple\n";
    std::cout << "   - Reusable building blocks\n";
    std::cout << "   - Mathematical elegance\n";
}
}

// ============================================================================
// CONCEPT 5: CURRYING
// ============================================================================
// Transforming multi-argument function into sequence of single-argument functions

namespace Currying {

// Regular function
int add(int a, int b, int c) {
    return a + b + c;
}

// Curried version
auto add_curried(int a) {
    return [a](int b) {
        return [a, b](int c) {
            return a + b + c;
        };
    };
}

// Curried multiplication
auto multiply_curried(int a) {
    return [a](int b) {
        return a * b;
    };
}

void demonstrate() {
    std::cout << "\n=== Currying ===\n";

    // Regular use
    std::cout << "Regular add(2, 3, 4): " << add(2, 3, 4) << "\n";

    // Curried use - all at once
    std::cout << "Curried add(2)(3)(4): " << add_curried(2)(3)(4) << "\n";

    // Partial application
    auto add2 = add_curried(2);
    auto add2and3 = add2(3);
    std::cout << "Partial application add2and3(4): " << add2and3(4) << "\n";

    // Creating specialized functions
    auto multiplyBy10 = multiply_curried(10);
    std::cout << "\nSpecialized multiplyBy10(5): " << multiplyBy10(5) << "\n";
    std::cout << "Specialized multiplyBy10(7): " << multiplyBy10(7) << "\n";

    std::cout << "\n✅ Currying enables:\n";
    std::cout << "   - Partial application\n";
    std::cout << "   - Creating specialized functions\n";
    std::cout << "   - More flexible composition\n";
}
}

// ============================================================================
// CONCEPT 6: FIRST-CLASS FUNCTIONS
// ============================================================================
// Functions as values (can be stored, passed, returned)

namespace FirstClassFunctions {

void demonstrate() {
    std::cout << "\n=== First-Class Functions ===\n";

    // Store functions in variables
    std::function<int(int)> square = [](int x) { return x * x; };
    std::function<int(int)> cube = [](int x) { return x * x * x; };

    std::cout << "Stored function square(5): " << square(5) << "\n";
    std::cout << "Stored function cube(3): " << cube(3) << "\n";

    // Store functions in containers
    std::vector<std::function<int(int)>> operations = {
        [](int x) { return x + 1; },
        [](int x) { return x * 2; },
        [](int x) { return x * x; }
    };

    std::cout << "\nApply operations to 5:\n";
    for (size_t i = 0; i < operations.size(); ++i) {
        std::cout << "  Operation " << i << ": " << operations[i](5) << "\n";
    }

    // Choose function based on condition
    auto operation = (5 > 3) ? square : cube;
    std::cout << "\nConditional function: " << operation(4) << "\n";

    std::cout << "\n✅ First-class functions allow:\n";
    std::cout << "   - Storing functions in variables\n";
    std::cout << "   - Passing functions as arguments\n";
    std::cout << "   - Returning functions from functions\n";
    std::cout << "   - Dynamic function selection\n";
}
}

// ============================================================================
// REAL-WORLD EXAMPLE: Event System using Functional Concepts
// ============================================================================

namespace RealWorldExample {

class EventSystem {
private:
    std::vector<std::function<void(const std::string&)>> handlers;

public:
    // Register event handler (first-class function)
    void on(std::function<void(const std::string&)> handler) {
        handlers.push_back(handler);
    }

    // Trigger event
    void emit(const std::string& event) {
        for (const auto& handler : handlers) {
            handler(event);  // Higher-order function
        }
    }

    // Create filtered handler (composition)
    static auto createFilteredHandler(
        std::function<bool(const std::string&)> filter,
        std::function<void(const std::string&)> handler)
    {
        return [filter, handler](const std::string& event) {
            if (filter(event)) {
                handler(event);
            }
        };
    }
};

void demonstrate() {
    std::cout << "\n=== Real-World: Event System ===\n";

    EventSystem events;

    // Register simple handlers
    events.on([](const std::string& e) {
        std::cout << "Handler 1: " << e << "\n";
    });

    // Register filtered handler (composition)
    auto errorFilter = [](const std::string& e) {
        return e.find("ERROR") != std::string::npos;
    };
    auto errorHandler = [](const std::string& e) {
        std::cout << "🚨 Error Handler: " << e << "\n";
    };
    events.on(EventSystem::createFilteredHandler(errorFilter, errorHandler));

    // Emit events
    std::cout << "\nEmitting events:\n";
    events.emit("INFO: System started");
    events.emit("ERROR: Connection failed");
    events.emit("WARNING: Low memory");

    std::cout << "\n✅ Functional patterns in real system:\n";
    std::cout << "   - Handlers as first-class functions\n";
    std::cout << "   - Higher-order event processing\n";
    std::cout << "   - Composition for filtering\n";
}
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
    std::cout << "\n╔═══════════════════════════════════════════════════════╗\n";
    std::cout << "║       FUNCTIONAL PROGRAMMING CONCEPTS IN C++        ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════╝\n";

    PureFunctions::demonstrate();
    Immutability::demonstrate();
    HigherOrderFunctions::demonstrate();
    FunctionComposition::demonstrate();
    Currying::demonstrate();
    FirstClassFunctions::demonstrate();
    RealWorldExample::demonstrate();

    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "🎓 FUNCTIONAL PROGRAMMING PRINCIPLES:\n";
    std::cout << std::string(60, '=') << "\n";
    std::cout << "1. PURE FUNCTIONS - No side effects, deterministic\n";
    std::cout << "2. IMMUTABILITY - Prefer const, create new instead of modify\n";
    std::cout << "3. HIGHER-ORDER - Functions that work with functions\n";
    std::cout << "4. COMPOSITION - Build complex from simple\n";
    std::cout << "5. CURRYING - Partial application of functions\n";
    std::cout << "6. FIRST-CLASS - Functions as values\n";
    std::cout << "\n💡 think-cell uses these principles extensively!\n";
    std::cout << "   Their code is a masterclass in functional C++.\n";
    std::cout << std::string(60, '=') << "\n\n";

    return 0;
}

/*
 * COMPILE AND RUN:
 * ===============
 * g++ -std=c++20 -Wall -Wextra functional_style_examples.cpp -o functional_style
 * ./functional_style
 *
 * LEARNING OBJECTIVES:
 * ===================
 * 1. Understand what makes a function "pure"
 * 2. Learn the benefits of immutability
 * 3. Master higher-order functions (map, filter, reduce)
 * 4. Practice function composition
 * 5. Understand currying and partial application
 * 6. Use functions as first-class citizens
 *
 * NEXT STEPS:
 * ===========
 * After mastering these concepts, move to:
 * - lambda_composition.cpp (advanced lambda usage)
 * - ranges_introduction.cpp (C++20 ranges)
 * - ThinkCellPhilosophy/ (real-world patterns)
 */
