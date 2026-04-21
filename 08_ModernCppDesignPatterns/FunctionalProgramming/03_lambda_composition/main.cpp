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
 * Compile: g++ -std=c++20 -Wall -Wextra lambda_composition.cpp -o lambda_composition
 */

#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <string>
#include <memory>

// ============================================================================
// PART 1: Lambda Basics and Capture Modes
// ============================================================================

namespace LambdaBasics {

void demonstrate() {
    std::cout << "\n=== Lambda Basics & Captures ===\n";

    int x = 10;
    int y = 20;

    // 1. No capture
    auto noCapture = []() { return 42; };
    std::cout << "No capture: " << noCapture() << "\n";

    // 2. Capture by value [=]
    auto captureByValue = [=]() { return x + y; };
    std::cout << "Capture by value [=]: " << captureByValue() << "\n";
    x = 100;  // Doesn't affect lambda
    std::cout << "After x=100: " << captureByValue() << " (still 30)\n";

    // 3. Capture by reference [&]
    x = 10;
    auto captureByRef = [&]() { return x + y; };
    std::cout << "Capture by reference [&]: " << captureByRef() << "\n";
    x = 100;  // Affects lambda
    std::cout << "After x=100: " << captureByRef() << " (now 120)\n";

    // 4. Capture specific variables
    x = 10;
    auto captureSpecific = [x, &y]() { return x + y; };
    std::cout << "Capture [x, &y]: " << captureSpecific() << "\n";

    // 5. Init capture (C++14)
    auto initCapture = [z = x + y]() { return z; };
    std::cout << "Init capture [z = x+y]: " << initCapture() << "\n";

    // 6. Capture this (in class methods)
    std::cout << "\n✅ Capture modes:\n";
    std::cout << "   []       - No capture\n";
    std::cout << "   [=]      - Capture all by value\n";
    std::cout << "   [&]      - Capture all by reference\n";
    std::cout << "   [x, &y]  - Capture x by value, y by ref\n";
    std::cout << "   [z = expr] - Init capture (C++14)\n";
}
}

// ============================================================================
// PART 2: Generic Lambdas (C++14)
// ============================================================================

namespace GenericLambdas {

void demonstrate() {
    std::cout << "\n=== Generic Lambdas (C++14) ===\n";

    // Generic lambda with auto
    auto print = [](auto value) {
        std::cout << "  Value: " << value << "\n";
    };

    std::cout << "Generic lambda works with any type:\n";
    print(42);
    print(3.14);
    print("Hello");
    print(std::string("World"));

    // Generic lambda with multiple auto parameters
    auto add = [](auto a, auto b) {
        return a + b;
    };

    std::cout << "\nGeneric add:\n";
    std::cout << "  int + int: " << add(5, 3) << "\n";
    std::cout << "  double + double: " << add(5.5, 3.2) << "\n";
    std::cout << "  string + string: " << add(std::string("Hello"), std::string(" World")) << "\n";

    // Generic lambda with type constraints (C++20 concepts)
    auto squareIfNumber = [](auto x) {
        if constexpr (std::is_arithmetic_v<decltype(x)>) {
            return x * x;
        } else {
            return x;
        }
    };

    std::cout << "\nConditional generic lambda:\n";
    std::cout << "  squareIfNumber(5): " << squareIfNumber(5) << "\n";
    std::cout << "  squareIfNumber(3.0): " << squareIfNumber(3.0) << "\n";

    std::cout << "\n✅ Generic lambdas enable:\n";
    std::cout << "   - Type-generic algorithms\n";
    std::cout << "   - Less template boilerplate\n";
    std::cout << "   - Compile-time polymorphism\n";
}
}

// ============================================================================
// PART 3: Mutable Lambdas
// ============================================================================

namespace MutableLambdas {

void demonstrate() {
    std::cout << "\n=== Mutable Lambdas ===\n";

    int x = 0;

    // Non-mutable lambda (default)
    auto nonMutable = [x]() {
        // x++;  // ❌ ERROR: Cannot modify captured variable
        return x + 1;
    };
    std::cout << "Non-mutable lambda: " << nonMutable() << "\n";

    // Mutable lambda - can modify captured values
    auto counter = [x]() mutable {
        return ++x;  // ✅ OK: Can modify because of 'mutable'
    };

    std::cout << "Mutable lambda calls:\n";
    std::cout << "  Call 1: " << counter() << "\n";
    std::cout << "  Call 2: " << counter() << "\n";
    std::cout << "  Call 3: " << counter() << "\n";
    std::cout << "Original x: " << x << " (unchanged)\n";

    // Practical example: Stateful filter
    auto takeFirst3 = [count = 0]() mutable {
        return ++count <= 3;
    };

    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7};
    std::cout << "\nTake first 3 using mutable lambda: ";
    for (const auto& n : numbers) {
        if (takeFirst3()) {
            std::cout << n << " ";
        }
    }
    std::cout << "\n";

    std::cout << "\n✅ Mutable lambdas:\n";
    std::cout << "   - Can modify captured values\n";
    std::cout << "   - Useful for stateful operations\n";
    std::cout << "   - Don't affect original variables\n";
}
}

// ============================================================================
// PART 4: Recursive Lambdas
// ============================================================================

namespace RecursiveLambdas {

void demonstrate() {
    std::cout << "\n=== Recursive Lambdas ===\n";

    // Using std::function for recursion
    std::function<int(int)> factorial = [&factorial](int n) {
        return (n <= 1) ? 1 : n * factorial(n - 1);
    };

    std::cout << "Factorial using recursive lambda:\n";
    for (int i = 1; i <= 6; ++i) {
        std::cout << "  " << i << "! = " << factorial(i) << "\n";
    }

    // Fibonacci
    std::function<int(int)> fibonacci = [&fibonacci](int n) {
        return (n <= 1) ? n : fibonacci(n - 1) + fibonacci(n - 2);
    };

    std::cout << "\nFibonacci using recursive lambda:\n";
    std::cout << "  ";
    for (int i = 0; i < 10; ++i) {
        std::cout << fibonacci(i) << " ";
    }
    std::cout << "\n";

    // Tree traversal (practical example)
    struct Node {
        int value;
        std::vector<std::shared_ptr<Node>> children;
    };

    std::function<void(const std::shared_ptr<Node>&, int)> printTree =
        [&printTree](const std::shared_ptr<Node>& node, int depth) {
            if (!node) return;
            std::cout << std::string(depth * 2, ' ') << node->value << "\n";
            for (const auto& child : node->children) {
                printTree(child, depth + 1);
            }
        };

    // Create simple tree
    auto root = std::make_shared<Node>(Node{1, {}});
    root->children.push_back(std::make_shared<Node>(Node{2, {}}));
    root->children.push_back(std::make_shared<Node>(Node{3, {}}));
    root->children[0]->children.push_back(std::make_shared<Node>(Node{4, {}}));

    std::cout << "\nTree traversal with recursive lambda:\n";
    printTree(root, 0);

    std::cout << "\n✅ Recursive lambdas:\n";
    std::cout << "   - Need std::function for self-reference\n";
    std::cout << "   - Useful for tree/graph algorithms\n";
    std::cout << "   - Can replace small recursive functions\n";
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
    std::cout << "\n=== Lambda Composition ===\n";

    // Define simple transformations
    auto add10 = [](int x) { return x + 10; };
    auto multiplyBy2 = [](int x) { return x * 2; };
    auto square = [](int x) { return x * x; };
    auto toString = [](int x) { return std::to_string(x); };

    // Compose functions
    auto add10ThenMultiply = compose(multiplyBy2, add10);
    auto multiplyThenAdd = compose(add10, multiplyBy2);

    std::cout << "Composition examples with input 5:\n";
    std::cout << "  add10 then multiply: " << add10ThenMultiply(5)
              << " ((5+10)*2 = 30)\n";
    std::cout << "  multiply then add10: " << multiplyThenAdd(5)
              << " ((5*2)+10 = 20)\n";

    // Triple composition
    auto complex = compose(square, compose(add10, multiplyBy2));
    std::cout << "  Triple: multiply→add10→square: " << complex(5)
              << " (((5*2)+10)² = 400)\n";

    // Using pipe operator
    std::cout << "\nUsing pipe operator:\n";
    auto result = 5
                  | multiplyBy2
                  | add10
                  | square;
    std::cout << "  5 | multiply2 | add10 | square: " << result << "\n";

    // Compose with different types
    auto numericPipeline = compose(toString, compose(square, add10));
    std::string strResult = numericPipeline(5);
    std::cout << "  Mixed types: " << strResult << " (string)\n";

    std::cout << "\n✅ Lambda composition:\n";
    std::cout << "   - Build complex from simple\n";
    std::cout << "   - Readable data flow\n";
    std::cout << "   - Reusable transformations\n";
}
}

// ============================================================================
// PART 6: Immediately Invoked Lambdas (IIFE)
// ============================================================================

namespace ImmediatelyInvokedLambdas {

void demonstrate() {
    std::cout << "\n=== Immediately Invoked Lambda Expressions ===\n";

    // Use case 1: Complex initialization
    const int complexValue = [](){
        int result = 0;
        for (int i = 1; i <= 10; ++i) {
            result += i * i;
        }
        return result;
    }();  // <-- Immediately invoked!

    std::cout << "Complex initialization: " << complexValue << "\n";

    // Use case 2: Conditional const initialization
    const std::string message = [](int hour) {
        if (hour < 12) return "Good morning";
        else if (hour < 18) return "Good afternoon";
        else return "Good evening";
    }(14);  // <-- Pass argument and invoke

    std::cout << "Conditional init: " << message << "\n";

    // Use case 3: RAII pattern
    std::cout << "\nRAII with IIFE:\n";
    [&]() {
        std::cout << "  Setup resources...\n";
        // Do work
        std::cout << "  Process data...\n";
        // Cleanup happens automatically
        std::cout << "  Cleanup resources...\n";
    }();

    // Use case 4: One-time initialization
    static bool initialized = []() {
        std::cout << "One-time initialization\n";
        return true;
    }();
    (void)initialized;  // Suppress unused warning

    std::cout << "\n✅ IIFE useful for:\n";
    std::cout << "   - Complex const initialization\n";
    std::cout << "   - Scoped temporary variables\n";
    std::cout << "   - RAII patterns\n";
    std::cout << "   - One-time setup code\n";
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
    std::cout << "\n=== Advanced Lambda Patterns ===\n";

    // Currying
    std::cout << "Curried add(2)(3)(4): " << curry_add(2)(3)(4) << "\n";
    auto add2 = curry_add(2);
    auto add2and3 = add2(3);
    std::cout << "Partial application: " << add2and3(4) << "\n";

    // Partial application
    auto multiply = [](int a, int b, int c) { return a * b * c; };
    auto multiplyBy2 = partial(multiply, 2);
    std::cout << "\nPartial application multiplyBy2(3, 4): "
              << multiplyBy2(3, 4) << "\n";

    // Lambda as strategy pattern
    std::cout << "\nStrategy pattern with lambdas:\n";
    auto calculate = [](int a, int b, auto operation) {
        return operation(a, b);
    };

    std::cout << "  10 + 5 = " << calculate(10, 5, std::plus<>()) << "\n";
    std::cout << "  10 - 5 = " << calculate(10, 5, std::minus<>()) << "\n";
    std::cout << "  10 * 5 = " << calculate(10, 5, std::multiplies<>()) << "\n";

    std::cout << "\n✅ Advanced patterns:\n";
    std::cout << "   - Currying for flexible application\n";
    std::cout << "   - Partial application for specialization\n";
    std::cout << "   - Strategy pattern with lambdas\n";
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
    std::cout << "\n=== Real-World: Transaction Pipeline ===\n";

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
    std::cout << "\nSum of approved large transactions: $" << sum << "\n";

    // Pipeline 2: Count electronics
    auto count = std::count_if(transactions.begin(), transactions.end(),
                               [isApproved, isElectronics](const Transaction& t) {
                                   return isApproved(t) && isElectronics(t);
                               }
                               );
    std::cout << "Approved electronics count: " << count << "\n";

    // Pipeline 3: Apply discount with lambda composition
    auto applyDiscount = [](double rate) {
        return [rate](double amount) {
            return amount * (1.0 - rate);
        };
    };

    auto discount10 = applyDiscount(0.10);
    auto discount20 = applyDiscount(0.20);

    std::cout << "\nApply discounts:\n";
    std::cout << "  $100 with 10% off: $" << discount10(100) << "\n";
    std::cout << "  $100 with 20% off: $" << discount20(100) << "\n";

    std::cout << "\n✅ Lambda patterns in production:\n";
    std::cout << "   - Reusable predicates\n";
    std::cout << "   - Composable transformations\n";
    std::cout << "   - Parameterized operations\n";
    std::cout << "   - Type-safe callbacks\n";
}
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
    std::cout << "\n╔═══════════════════════════════════════════════════════╗\n";
    std::cout << "║          ADVANCED LAMBDA COMPOSITION IN C++         ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════╝\n";

    LambdaBasics::demonstrate();
    GenericLambdas::demonstrate();
    MutableLambdas::demonstrate();
    RecursiveLambdas::demonstrate();
    LambdaComposition::demonstrate();
    ImmediatelyInvokedLambdas::demonstrate();
    AdvancedPatterns::demonstrate();
    RealWorldPipeline::demonstrate();

    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "🎓 LAMBDA MASTERY CHECKLIST:\n";
    std::cout << std::string(60, '=') << "\n";
    std::cout << "✅ Understand all capture modes [=], [&], [x], [&y]\n";
    std::cout << "✅ Use generic lambdas with auto parameters\n";
    std::cout << "✅ Know when to use mutable lambdas\n";
    std::cout << "✅ Implement recursive algorithms with lambdas\n";
    std::cout << "✅ Compose lambdas to build complex operations\n";
    std::cout << "✅ Use IIFE for complex initialization\n";
    std::cout << "✅ Apply currying and partial application\n";
    std::cout << "✅ Build production-ready pipelines\n";
    std::cout << "\n💡 think-cell heavily relies on lambda composition\n";
    std::cout << "   for building elegant, reusable algorithms!\n";
    std::cout << std::string(60, '=') << "\n\n";

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
 * - Study ThinkCellPhilosophy/ for real-world patterns
 */
