/*
 * lambda_composition.cpp
 *
 * Advanced Lambda Expressions and Composition in C++
 *
 * Difficulty: 🟡 Intermediate
 *
 * Key Concepts:
 * - Lambda syntax and captures
 * - Generic lambdas (C++14)
 * - Recursive lambdas
 * - Lambda composition
 * - Perfect forwarding in lambdas
 * - Mutable lambdas
 * - Immediately invoked lambdas (IIFE)
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <vector>
#include <functional>
#include <algorithm>
#include <string>
#include <memory>
#include <print>

// ============================================================================
// PART 1: Lambda Basics and Capture Modes
// ============================================================================

namespace LambdaBasics {

void demonstrate() {
    std::println("");
    std::println("=== Lambda Basics & Captures ===");

    int x = 10;
    int y = 20;

    // 1. No capture
    auto noCapture = []() { return 42; };
    std::println("No capture: {}", noCapture());

    // 2. Capture by value [=]
    auto captureByValue = [=]() { return x + y; };
    std::println("Capture by value [=]: {}", captureByValue());
    x = 100;  // Doesn't affect lambda
    std::println("After x=100: {} (still 30)", captureByValue());

    // 3. Capture by reference [&]
    x = 10;
    auto captureByRef = [&]() { return x + y; };
    std::println("Capture by reference [&]: {}", captureByRef());
    x = 100;  // Affects lambda
    std::println("After x=100: {} (now 120)", captureByRef());

    // 4. Capture specific variables
    x = 10;
    auto captureSpecific = [x, &y]() { return x + y; };
    std::println("Capture [x, &y]: {}", captureSpecific());

    // 5. Init capture (C++14)
    auto initCapture = [z = x + y]() { return z; };
    std::println("Init capture [z = x+y]: {}", initCapture());

    // 6. Capture this (in class methods)
}
}

// ============================================================================
// PART 2: Generic Lambdas (C++14)
// ============================================================================

namespace GenericLambdas {

void demonstrate() {
    std::println("");
    std::println("=== Generic Lambdas (C++14) ===");

    // Generic lambda with auto
    auto print = [](auto value) {
        std::println("  Value: {}", value);
    };

    std::println("Generic lambda works with any type:");
    print(42);
    print(3.14);
    print("Hello");
    print(std::string("World"));

    // Generic lambda with multiple auto parameters
    auto add = [](auto a, auto b) {
        return a + b;
    };

    std::println("");
    std::println("Generic add:");
    std::println("  int + int: {}", add(5, 3));
    std::println("  double + double: {}", add(5.5, 3.2));
    std::println("  string + string: {}", add(std::string("Hello"), std::string(" World")));

    // Generic lambda with type constraints (C++20 concepts)
    auto squareIfNumber = [](auto x) {
        if constexpr (std::is_arithmetic_v<decltype(x)>) {
            return x * x;
        } else {
            return x;
        }
    };

    std::println("");
    std::println("Conditional generic lambda:");
    std::println("  squareIfNumber(5): {}", squareIfNumber(5));
    std::println("  squareIfNumber(3.0): {}", squareIfNumber(3.0));

}
}

// ============================================================================
// PART 3: Mutable Lambdas
// ============================================================================

namespace MutableLambdas {

void demonstrate() {
    std::println("");
    std::println("=== Mutable Lambdas ===");

    int x = 0;

    // Non-mutable lambda (default)
    auto nonMutable = [x]() {
        // x++;  // ❌ ERROR: Cannot modify captured variable
        return x + 1;
    };
    std::println("Non-mutable lambda: {}", nonMutable());

    // Mutable lambda - can modify captured values
    auto counter = [x]() mutable {
        return ++x;  // ✅ OK: Can modify because of 'mutable'
    };

    std::println("Mutable lambda calls:");
    std::println("  Call 1: {}", counter());
    std::println("  Call 2: {}", counter());
    std::println("  Call 3: {}", counter());
    std::println("Original x: {} (unchanged)", x);

    // Practical example: Stateful filter
    auto takeFirst3 = [count = 0]() mutable {
        return ++count <= 3;
    };

    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7};
    std::println("");
    std::print("Take first 3 using mutable lambda: ");
    for (const auto& n : numbers) {
        if (takeFirst3()) {
            std::print("{} ", n);
        }
    }
    std::println("");

}
}

// ============================================================================
// PART 4: Recursive Lambdas
// ============================================================================

namespace RecursiveLambdas {

void demonstrate() {
    std::println("");
    std::println("=== Recursive Lambdas ===");

    // Using std::function for recursion
    std::function<int(int)> factorial = [&factorial](int n) {
        return (n <= 1) ? 1 : n * factorial(n - 1);
    };

    std::println("Factorial using recursive lambda:");
    for (int i = 1; i <= 6; ++i) {
        std::println("  {}! = {}", i, factorial(i));
    }

    // Fibonacci
    std::function<int(int)> fibonacci = [&fibonacci](int n) {
        return (n <= 1) ? n : fibonacci(n - 1) + fibonacci(n - 2);
    };

    std::println("");
    std::println("Fibonacci using recursive lambda:");
    std::print("  ");
    for (int i = 0; i < 10; ++i) {
        std::print("{} ", fibonacci(i));
    }
    std::println("");

    // Tree traversal (practical example)
    struct Node {
        int value;
        std::vector<std::shared_ptr<Node>> children;
    };

    std::function<void(const std::shared_ptr<Node>&, int)> printTree =
        [&printTree](const std::shared_ptr<Node>& node, int depth) {
            if (!node) return;
            std::println("{}{}", std::string(depth * 2, ' '), node->value);
            for (const auto& child : node->children) {
                printTree(child, depth + 1);
            }
        };

    // Create simple tree
    auto root = std::make_shared<Node>(Node{1, {}});
    root->children.push_back(std::make_shared<Node>(Node{2, {}}));
    root->children.push_back(std::make_shared<Node>(Node{3, {}}));
    root->children[0]->children.push_back(std::make_shared<Node>(Node{4, {}}));

    std::println("");
    std::println("Tree traversal with recursive lambda:");
    printTree(root, 0);

}
}

// ============================================================================
// PART 5: Lambda Composition
// ============================================================================

namespace LambdaComposition {

// Compose two functions: (f ∘ g)(x) = f(g(x))
template<typename F, typename G>
auto compose(F f, G g) {
    return [f, g](auto x) { return f(g(x)); };
}

// Pipe operator: x | f = f(x)
template<typename T, typename F>
auto operator|(T&& value, F func) {
    return func(std::forward<T>(value));
}

void demonstrate() {
    std::println("");
    std::println("=== Lambda Composition ===");

    // Define simple transformations
    auto add10 = [](int x) { return x + 10; };
    auto multiplyBy2 = [](int x) { return x * 2; };
    auto square = [](int x) { return x * x; };
    auto toString = [](int x) { return std::to_string(x); };

    // Compose functions
    auto add10ThenMultiply = compose(multiplyBy2, add10);
    auto multiplyThenAdd = compose(add10, multiplyBy2);

    std::println("Composition examples with input 5:");
    std::println("  add10 then multiply: {} ((5+10)*2 = 30)", add10ThenMultiply(5));
    std::println("  multiply then add10: {} ((5*2)+10 = 20)", multiplyThenAdd(5));

    // Triple composition
    auto complex = compose(square, compose(add10, multiplyBy2));
    std::println("  Triple: multiply→add10→square: {} (((5*2)+10)² = 400)", complex(5));

    // Using pipe operator
    std::println("");
    std::println("Using pipe operator:");
    auto result = 5
                  | multiplyBy2
                  | add10
                  | square;
    std::println("  5 | multiply2 | add10 | square: {}", result);

    // Compose with different types
    auto numericPipeline = compose(toString, compose(square, add10));
    std::string strResult = numericPipeline(5);
    std::println("  Mixed types: {} (string)", strResult);

}
}

// ============================================================================
// PART 6: Immediately Invoked Lambdas (IIFE)
// ============================================================================

namespace ImmediatelyInvokedLambdas {

void demonstrate() {
    std::println("");
    std::println("=== Immediately Invoked Lambda Expressions ===");

    // Use case 1: Complex initialization
    const int complexValue = [](){
        int result = 0;
        for (int i = 1; i <= 10; ++i) {
            result += i * i;
        }
        return result;
    }();  // <-- Immediately invoked!

    std::println("Complex initialization: {}", complexValue);

    // Use case 2: Conditional const initialization
    const std::string message = [](int hour) {
        if (hour < 12) return "Good morning";
        else if (hour < 18) return "Good afternoon";
        else return "Good evening";
    }(14);  // <-- Pass argument and invoke

    std::println("Conditional init: {}", message);

    // Use case 3: RAII pattern
    std::println("");
    std::println("RAII with IIFE:");
    [&]() {
    }();

    // Use case 4: One-time initialization
    static bool initialized = []() {
        std::println("One-time initialization");
        return true;
    }();
    (void)initialized;  // Suppress unused warning

}
}

// ============================================================================
// PART 7: Advanced Lambda Patterns
// ============================================================================

namespace AdvancedPatterns {

// Currying with lambdas
auto curry_add = [](int a) {
    return [a](int b) {
        return [a, b](int c) {
            return a + b + c;
        };
    };
};

// Partial application
template<typename Func, typename... Args>
auto partial(Func func, Args... args) {
    return [func, args...](auto... rest) {
        return func(args..., rest...);
    };
}

// Y Combinator for anonymous recursion
template<typename F>
auto Y(F f) {
    return [f](auto x) {
        return f([x](auto... args) {
            return x(x)(args...);
        });
    }([f](auto x) {
        return f([x](auto... args) {
            return x(x)(args...);
        });
    });
}

void demonstrate() {
    std::println("");
    std::println("=== Advanced Lambda Patterns ===");

    // Currying
    std::println("Curried add(2)(3)(4): {}", curry_add(2)(3)(4));
    auto add2 = curry_add(2);
    auto add2and3 = add2(3);
    std::println("Partial application: {}", add2and3(4));

    // Partial application
    auto multiply = [](int a, int b, int c) { return a * b * c; };
    auto multiplyBy2 = partial(multiply, 2);
    std::println("");
    std::println("Partial application multiplyBy2(3, 4): {}", multiplyBy2(3, 4));

    // Lambda as strategy pattern
    std::println("");
    std::println("Strategy pattern with lambdas:");
    auto calculate = [](int a, int b, auto operation) {
        return operation(a, b);
    };

    std::println("  10 + 5 = {}", calculate(10, 5, std::plus<>()));
    std::println("  10 - 5 = {}", calculate(10, 5, std::minus<>()));
    std::println("  10 * 5 = {}", calculate(10, 5, std::multiplies<>()));

}
}

// ============================================================================
// REAL-WORLD EXAMPLE: Data Processing Pipeline
// ============================================================================

namespace RealWorldPipeline {

struct Transaction {
    std::string id;
    double amount;
    std::string category;
    bool approved;
};

void demonstrate() {
    std::println("");
    std::println("=== Real-World: Transaction Pipeline ===");

    std::vector<Transaction> transactions = {
                                             {"T1", 150.0, "food", true},
                                             {"T2", 2500.0, "electronics", true},
                                             {"T3", 50.0, "food", false},
                                             {"T4", 1200.0, "electronics", true},
                                             {"T5", 75.0, "food", true},
                                             };

    // Define reusable filters and transformations
    auto isApproved = [](const Transaction& t) { return t.approved; };
    auto isLargeAmount = [](const Transaction& t) { return t.amount > 100; };
    auto isElectronics = [](const Transaction& t) {
        return t.category == "electronics";
    };
    auto extractAmount = [](const Transaction& t) { return t.amount; };

    // Pipeline 1: Sum of approved large transactions
    double sum = 0;
    std::for_each(transactions.begin(), transactions.end(),
                  [&sum, isApproved, isLargeAmount, extractAmount](const Transaction& t) {
                      if (isApproved(t) && isLargeAmount(t)) {
                          sum += extractAmount(t);
                      }
                  }
                  );
    std::println("");
    std::println("Sum of approved large transactions: ${}", sum);

    // Pipeline 2: Count electronics
    auto count = std::count_if(transactions.begin(), transactions.end(),
                               [isApproved, isElectronics](const Transaction& t) {
                                   return isApproved(t) && isElectronics(t);
                               }
                               );
    std::println("Approved electronics count: {}", count);

    // Pipeline 3: Apply discount with lambda composition
    auto applyDiscount = [](double rate) {
        return [rate](double amount) {
            return amount * (1.0 - rate);
        };
    };

    auto discount10 = applyDiscount(0.10);
    auto discount20 = applyDiscount(0.20);

    std::println("");
    std::println("Apply discounts:");
    std::println("  $100 with 10% off: ${}", discount10(100));
    std::println("  $100 with 20% off: ${}", discount20(100));

}
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
    std::println("╔═══════════════════════════════════════════════════════╗");
    std::println("║          ADVANCED LAMBDA COMPOSITION IN C++         ║");
    std::println("╚═══════════════════════════════════════════════════════╝");

    LambdaBasics::demonstrate();
    GenericLambdas::demonstrate();
    MutableLambdas::demonstrate();
    RecursiveLambdas::demonstrate();
    LambdaComposition::demonstrate();
    ImmediatelyInvokedLambdas::demonstrate();
    AdvancedPatterns::demonstrate();
    RealWorldPipeline::demonstrate();

    std::println("");
    std::println("{}", std::string(60, '='));
    std::println("🎓 LAMBDA MASTERY CHECKLIST:");
    std::println("{}", std::string(60, '='));
    std::println("{}", std::string(60, '='));
    std::println("");

    return 0;
}

/*
 * COMPILE AND RUN:
 * ===============
 * g++ -std=c++20 -Wall -Wextra lambda_composition.cpp -o lambda_composition
 * ./lambda_composition
 *
 * KEY TAKEAWAYS:
 * =============
 * 1. Lambdas are powerful tools for functional programming
 * 2. Capture modes determine how variables are accessed
 * 3. Generic lambdas work with any type (C++14)
 * 4. Mutable lambdas can maintain state
 * 5. Composition builds complex from simple
 * 6. IIFE useful for complex initialization
 * 7. Lambdas enable elegant, concise code
 *
 * NEXT STEPS:
 * ===========
 * - Practice building your own composable lambdas
 * - Move to ranges_introduction.cpp for C++20 ranges
 * - Study ../../RangesAndGenericStyle/ for real-world patterns
 */
