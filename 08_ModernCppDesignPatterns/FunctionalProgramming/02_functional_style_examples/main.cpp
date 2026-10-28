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
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <vector>
#include <functional>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <string>
#include <ranges>
#include <print>

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
    std::println("");
    std::println("=== Pure Functions ===");
    std::println("Pure add(5, 3): {} (always 8)", add(5, 3));
    std::println("Pure square(4): {} (always 16)", square(4));

    std::println("");
    std::println("Impure addAndCount(5, 3): {} (counter now: {})", addAndCount(5, 3), counter);
    std::println("Impure addAndCount(5, 3): {} (counter now: {})", addAndCount(5, 3), counter);
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
    std::println("");
    std::println("=== Immutability ===");

    std::vector<int> original = {1, 2, 3, 4, 5};
    std::print("Original: ");
    for (auto n : original) std::print("{} ", n);

    // Mutable approach
    auto mutable_copy = original;
    modifyInPlace(mutable_copy);
    std::println("");
    std::print("After mutable operation: ");
    for (auto n : mutable_copy) std::print("{} ", n);
    std::print(" (original changed if same object!)");

    // Immutable approach
    auto immutable_result = createModified(original);
    std::println("");
    std::println("");
    std::print("Original (unchanged): ");
    for (auto n : original) std::print("{} ", n);
    std::println("");
    std::print("Immutable result: ");
    for (auto n : immutable_result) std::print("{} ", n);

    std::println("");
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
    std::println("");
    std::println("=== Higher-Order Functions ===");

    std::vector<int> numbers = {1, 2, 3, 4, 5};

    // Function as parameter
    std::print("Apply function to each: ");
    applyToEach(numbers, [](int x) { std::print("{} ", x * 2); });

    // Function as return value
    auto multiplyBy3 = makeMultiplier(3);
    std::println("");
    std::println("");
    std::println("Multiply by 3: {}", multiplyBy3(5));

    auto multiplyBy10 = makeMultiplier(10);
    std::println("Multiply by 10: {}", multiplyBy10(5));

    // Function transformation
    auto addOne = [](int x) { return x + 1; };
    auto addTwo = applyTwice(addOne);
    std::println("");
    std::println("Apply twice (add 1): {} (5+1+1=7)", addTwo(5));

    // Map, Filter, Reduce
    std::println("");
    std::print("Map (square): ");
    auto squared = map(numbers, [](int x) { return x * x; });
    for (auto n : squared) std::print("{} ", n);

    std::println("");
    std::print("Filter (evens): ");
    auto evens = filter(numbers, [](int x) { return x % 2 == 0; });
    for (auto n : evens) std::print("{} ", n);

    std::println("");
    std::print("Reduce (sum): ");
    auto sum = reduce(numbers, 0, [](int a, int b) { return a + b; });
    std::println("{}", sum);

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
    std::println("");
    std::println("=== Function Composition ===");

    // Define simple functions
    auto multiplyBy2 = [](int x) { return x * 2; };
    auto add10 = [](int x) { return x + 10; };
    auto square = [](int x) { return x * x; };

    // Compose: multiply then add
    auto multiplyThenAdd = compose(add10, multiplyBy2);
    std::println("Compose (add10 ∘ multiply2): 5 → {} (5*2+10=20)", multiplyThenAdd(5));

    // Compose: add then multiply
    auto addThenMultiply = compose(multiplyBy2, add10);
    std::println("Compose (multiply2 ∘ add10): 5 → {} ((5+10)*2=30)", addThenMultiply(5));

    // Triple composition
    auto complex = compose(square, compose(add10, multiplyBy2));
    std::println("Triple compose (square ∘ add10 ∘ multiply2): 5 → {} ((5*2+10)²=400)", complex(5));

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
    std::println("");
    std::println("=== Currying ===");

    // Regular use
    std::println("Regular add(2, 3, 4): {}", add(2, 3, 4));

    // Curried use - all at once
    std::println("Curried add(2)(3)(4): {}", add_curried(2)(3)(4));

    // Partial application
    auto add2 = add_curried(2);
    auto add2and3 = add2(3);
    std::println("Partial application add2and3(4): {}", add2and3(4));

    // Creating specialized functions
    auto multiplyBy10 = multiply_curried(10);
    std::println("");
    std::println("Specialized multiplyBy10(5): {}", multiplyBy10(5));
    std::println("Specialized multiplyBy10(7): {}", multiplyBy10(7));

}
}

// ============================================================================
// CONCEPT 6: FIRST-CLASS FUNCTIONS
// ============================================================================
// Functions as values (can be stored, passed, returned)

namespace FirstClassFunctions {

void demonstrate() {
    std::println("");
    std::println("=== First-Class Functions ===");

    // Store functions in variables
    std::function<int(int)> square = [](int x) { return x * x; };
    std::function<int(int)> cube = [](int x) { return x * x * x; };

    std::println("Stored function square(5): {}", square(5));
    std::println("Stored function cube(3): {}", cube(3));

    // Store functions in containers
    std::vector<std::function<int(int)>> operations = {
        [](int x) { return x + 1; },
        [](int x) { return x * 2; },
        [](int x) { return x * x; }
    };

    std::println("");
    std::println("Apply operations to 5:");
    for (size_t i = 0; i < operations.size(); ++i) {
        std::println("  Operation {}: {}", i, operations[i](5));
    }

    // Choose function based on condition
    auto operation = (5 > 3) ? square : cube;
    std::println("");
    std::println("Conditional function: {}", operation(4));

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
    std::println("");
    std::println("=== Real-World: Event System ===");

    EventSystem events;

    // Register simple handlers
    events.on([](const std::string& e) {
        std::println("Handler 1: {}", e);
    });

    // Register filtered handler (composition)
    auto errorFilter = [](const std::string& e) {
        return e.find("ERROR") != std::string::npos;
    };
    auto errorHandler = [](const std::string& e) {
        std::println("🚨 Error Handler: {}", e);
    };
    events.on(EventSystem::createFilteredHandler(errorFilter, errorHandler));

    // Emit events
    std::println("");
    std::println("Emitting events:");
    events.emit("INFO: System started");
    events.emit("ERROR: Connection failed");
    events.emit("WARNING: Low memory");

}
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
    std::println("╔═══════════════════════════════════════════════════════╗");
    std::println("║       FUNCTIONAL PROGRAMMING CONCEPTS IN C++          ║");
    std::println("╚═══════════════════════════════════════════════════════╝");

    PureFunctions::demonstrate();
    Immutability::demonstrate();
    HigherOrderFunctions::demonstrate();
    FunctionComposition::demonstrate();
    Currying::demonstrate();
    FirstClassFunctions::demonstrate();
    RealWorldExample::demonstrate();

    return 0;
}
