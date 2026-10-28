/*
 * ranges_introduction.cpp
 *
 * Introduction to C++20 Ranges Library
 *
 * Difficulty: 🟡 Intermediate
 *
 * Key Concepts:
 * - Range views and adaptors
 * - Lazy evaluation
 * - Pipe operator |
 * - Common range algorithms
 * - Custom ranges
 * - Range-based transformations
 *
 * Requirements: C++20 compiler (GCC 10+, Clang 13+, MSVC 2019+)
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <vector>
#include <ranges>
#include <algorithm>
#include <string>
#include <numeric>
#include <print>

// ============================================================================
// PART 1: Basic Range Views
// ============================================================================

namespace BasicViews {

void demonstrate() {
    std::println("");
    std::println("=== Basic Range Views ===");

    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // filter - select elements matching predicate
    std::println("");
    std::print("filter (even numbers): ");
    auto evens = numbers | std::views::filter([](int n) { return n % 2 == 0; });
    for (auto n : evens) std::print("{} ", n);

    // transform - apply function to each element
    std::println("");
    std::print("transform (square): ");
    auto squared = numbers | std::views::transform([](int n) { return n * n; });
    for (auto n : squared) std::print("{} ", n);

    // take - first N elements
    std::println("");
    std::print("take (first 5): ");
    auto first5 = numbers | std::views::take(5);
    for (auto n : first5) std::print("{} ", n);

    // drop - skip first N elements
    std::println("");
    std::print("drop (skip first 5): ");
    auto skip5 = numbers | std::views::drop(5);
    for (auto n : skip5) std::print("{} ", n);

    // reverse - elements in reverse order
    std::println("");
    std::print("reverse: ");
    auto reversed = numbers | std::views::reverse;
    for (auto n : reversed) std::print("{} ", n);

    std::println("");
}
}

// ============================================================================
// PART 2: Lazy Evaluation - The Power of Ranges
// ============================================================================

namespace LazyEvaluation {

void demonstrate() {
    std::println("");
    std::println("=== Lazy Evaluation ===");

    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    std::println("");
    std::println("Creating a view pipeline (no computation yet):");
    auto pipeline = numbers
                    | std::views::filter([](int n) {
                          std::println("  Filtering {}", n);
                          return n % 2 == 0;
                      })
                    | std::views::transform([](int n) {
                          std::println("  Transforming {}", n);
                          return n * n;
                      })
                    | std::views::take(3);

    std::println("");
    std::println("Pipeline created but not executed!");
    std::println("");
    std::println("Now iterating (computation happens):");

    for (auto n : pipeline) {
        std::println("Result: {}", n);
    }

}
}

// ============================================================================
// PART 3: Composing Multiple Views (Pipelines)
// ============================================================================

namespace PipelineComposition {

void demonstrate() {
    std::println("");
    std::println("=== Pipeline Composition ===");

    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

    // Complex pipeline: filter evens, square, take 5, reverse
    std::println("");
    std::println("Complex pipeline:");
    std::println("numbers | filter(even) | transform(square) | take(5) | reverse");
    std::print("Result: ");

    auto result = numbers
                  | std::views::filter([](int n) { return n % 2 == 0; })
                  | std::views::transform([](int n) { return n * n; })
                  | std::views::take(5)
                  | std::views::reverse;

    for (auto n : result) std::print("{} ", n);

    // Another example: filter > 5, double, skip first 2
    std::println("");
    std::println("");
    std::println("Another pipeline:");
    std::println("numbers | filter(>5) | transform(*2) | drop(2)");
    std::print("Result: ");

    auto result2 = numbers
                   | std::views::filter([](int n) { return n > 5; })
                   | std::views::transform([](int n) { return n * 2; })
                   | std::views::drop(2);

    for (auto n : result2) std::print("{} ", n);

    std::println("");
}
}

// ============================================================================
// PART 4: Common Range Algorithms
// ============================================================================

namespace RangeAlgorithms {

void demonstrate() {
    std::println("");
    std::println("=== Range Algorithms ===");

    std::vector<int> numbers = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};

    // std::ranges::sort
    auto sorted = numbers;
    std::ranges::sort(sorted);
    std::println("");
    std::print("Sorted: ");
    for (auto n : sorted) std::print("{} ", n);

    // std::ranges::find
    auto it = std::ranges::find(numbers, 5);
    if (it != numbers.end()) {
        std::println("");
        std::print("Found 5 at position: {}", std::distance(numbers.begin(), it));
    }

    // std::ranges::count
    auto count = std::ranges::count(numbers, 1);
    std::println("");
    std::print("Count of 1: {}", count);

    // std::ranges::count_if
    auto evens_count = std::ranges::count_if(numbers,
                                             [](int n) { return n % 2 == 0; });
    std::println("");
    std::print("Count of even numbers: {}", evens_count);

    // std::ranges::any_of, all_of, none_of
    bool hasEven = std::ranges::any_of(numbers,
                                       [](int n) { return n % 2 == 0; });
    std::println("");
    std::print("Has even numbers: {}", (hasEven ? "yes" : "no"));

    bool allPositive = std::ranges::all_of(numbers,
                                           [](int n) { return n > 0; });
    std::println("");
    std::print("All positive: {}", (allPositive ? "yes" : "no"));

    // std::ranges::max, min
    auto max = std::ranges::max(numbers);
    auto min = std::ranges::min(numbers);
    std::println("");
    std::print("Min: {}, Max: {}", min, max);

    std::println("");
}
}

// ============================================================================
// PART 5: Working with Strings
// ============================================================================

namespace StringRanges {

void demonstrate() {
    std::println("");
    std::println("=== Ranges with Strings ===");

    std::string text = "Hello, World! This is C++20 Ranges!";

    // Filter uppercase letters
    std::println("");
    std::print("Uppercase letters: ");
    auto uppercase = text
                     | std::views::filter([](char c) { return std::isupper(c); });
    for (char c : uppercase) std::print("{}", c);

    // Convert to uppercase
    std::println("");
    std::print("To uppercase: ");
    auto uppercased = text
                      | std::views::transform([](char c) { return std::toupper(c); });
    for (char c : uppercased) std::print("{}", c);

    // Take first 5 chars
    std::println("");
    std::print("First 5 chars: ");
    auto first5 = text | std::views::take(5);
    for (char c : first5) std::print("{}", c);

    // Split into words (using views::split)
    // Note: In C++20, split returns subranges that we iterate over directly
    std::println("");
    std::println("Words:");
    for (auto word : text | std::views::split(' ')) {
        std::print("  '");
        for (char c : word) {
            std::print("{}", c);
        }
        std::println("'");
    }

}
}

// ============================================================================
// PART 6: iota - Generate Sequences
// ============================================================================

namespace IotaView {

void demonstrate() {
    std::println("");
    std::println("=== iota - Generate Sequences ===");

    // Generate numbers 1 to 10
    std::println("");
    std::print("iota(1, 11): ");
    for (auto n : std::views::iota(1, 11)) {
        std::print("{} ", n);
    }

    // Infinite sequence (take first 10)
    std::println("");
    std::print("Infinite iota(1) | take(10): ");
    for (auto n : std::views::iota(1) | std::views::take(10)) {
        std::print("{} ", n);
    }

    // Generate and transform
    std::println("");
    std::print("iota(1, 6) squared: ");
    auto squares = std::views::iota(1, 6)
                   | std::views::transform([](int n) { return n * n; });
    for (auto n : squares) std::print("{} ", n);

    // Even numbers using iota
    std::println("");
    std::print("Even numbers 2-20: ");
    auto evens = std::views::iota(1, 11)
                 | std::views::transform([](int n) { return n * 2; });
    for (auto n : evens) std::print("{} ", n);

    std::println("");
}
}

// ============================================================================
// PART 7: join and flatten
// ============================================================================

namespace JoinAndFlatten {

void demonstrate() {
    std::println("");
    std::println("=== join - Flatten Nested Ranges ===");

    std::vector<std::vector<int>> nested = {
        {1, 2, 3},
        {4, 5},
        {6, 7, 8, 9}
    };

    std::println("");
    std::println("Nested vector: {{{{1,2,3}}, {{4,5}}, {{6,7,8,9}}}}");
    std::print("Flattened: ");

    auto flattened = nested | std::views::join;
    for (auto n : flattened) std::print("{} ", n);

    // Practical example: words to characters
    std::println("");
    std::println("");
    std::println("Words to characters:");
    std::vector<std::string> words = {"Hello", "World", "C++20"};

    std::print("Words: ");
    for (const auto& word : words) std::print("{} ", word);

    std::println("");
    std::print("All characters: ");
    auto chars = words | std::views::join;
    for (char c : chars) std::print("{}", c);

    std::println("");
}
}

// ============================================================================
// PART 8: elements - Access Tuple Elements
// ============================================================================

namespace ElementsView {

void demonstrate() {
    std::println("");
    std::println("=== elements - Access Tuple Elements ===");

    std::vector<std::pair<int, std::string>> data = {
        {1, "Alice"},
        {2, "Bob"},
        {3, "Charlie"},
        {4, "Diana"}
    };

    std::println("");
    std::println("Original pairs:");
    for (const auto& [id, name] : data) {
        std::println("  {}: {}", id, name);
    }

    // Extract first elements (keys)
    std::println("");
    std::print("IDs only (elements<0>): ");
    auto ids = data | std::views::elements<0>;
    for (auto id : ids) std::print("{} ", id);

    // Extract second elements (values)
    std::println("");
    std::print("Names only (elements<1>): ");
    auto names = data | std::views::elements<1>;
    for (const auto& name : names) std::print("{} ", name);

    // Alias: keys and values
    std::println("");
    std::println("");
    std::println("Using keys/values aliases:");
    std::print("Keys: ");
    for (auto key : data | std::views::keys) std::print("{} ", key);

    std::println("");
    std::print("Values: ");
    for (const auto& value : data | std::views::values) std::print("{} ", value);

    std::println("");
}
}

// ============================================================================
// PART 9: Real-World Example - Data Processing
// ============================================================================

namespace RealWorldExample {

struct Product {
    std::string name;
    double price;
    int quantity;
    std::string category;
};

void demonstrate() {
    std::println("");
    std::println("=== Real-World: Product Analysis ===");

    std::vector<Product> products = {
                                     {"Laptop", 1200.0, 5, "Electronics"},
                                     {"Mouse", 25.0, 50, "Electronics"},
                                     {"Keyboard", 75.0, 30, "Electronics"},
                                     {"Desk", 300.0, 10, "Furniture"},
                                     {"Chair", 150.0, 20, "Furniture"},
                                     {"Monitor", 400.0, 15, "Electronics"},
                                     };

    // Task 1: Total value of electronics
    std::println("");
    std::println("Task 1: Total value of Electronics");
    auto electronics_value = products
                             | std::views::filter([](const Product& p) {
                                   return p.category == "Electronics";
                               })
                             | std::views::transform([](const Product& p) {
                                   return p.price * p.quantity;
                               });

    double total = 0;
    for (auto value : electronics_value) {
        total += value;
    }
    std::println("Total: ${}", total);

    // Task 2: Expensive products (> $100)
    std::println("");
    std::println("Task 2: Expensive products (>$100):");
    auto expensive = products
                     | std::views::filter([](const Product& p) { return p.price > 100; })
                     | std::views::transform([](const Product& p) { return p.name; });

    for (const auto& name : expensive) {
        std::println("  - {}", name);
    }

    // Task 3: Low stock items (< 20)
    std::println("");
    std::println("Task 3: Low stock items (<20):");
    auto low_stock = products
                     | std::views::filter([](const Product& p) { return p.quantity < 20; })
                     | std::views::take(3);  // Top 3

    for (const auto& p : low_stock) {
        std::println("  - {} (qty: {})", p.name, p.quantity);
    }

    // Task 4: Price range
    std::println("");
    std::println("Task 4: Prices sorted:");
    auto prices = products
                  | std::views::transform([](const Product& p) { return p.price; });

    std::vector<double> price_vec(prices.begin(), prices.end());
    std::ranges::sort(price_vec);

    std::println("  Min: ${}, Max: ${}", price_vec.front(), price_vec.back());

}
}

// ============================================================================
// PART 10: Performance and Best Practices
// ============================================================================

namespace BestPractices {

void demonstrate() {
    std::println("");
    std::println("=== Best Practices ===");

    std::vector<int> large_data(1000);
    std::iota(large_data.begin(), large_data.end(), 1);

}
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
    std::println("╔═══════════════════════════════════════════════════════╗");
    std::println("║              C++20 RANGES INTRODUCTION              ║");
    std::println("╚═══════════════════════════════════════════════════════╝");

    BasicViews::demonstrate();
    LazyEvaluation::demonstrate();
    PipelineComposition::demonstrate();
    RangeAlgorithms::demonstrate();
    StringRanges::demonstrate();
    IotaView::demonstrate();
    JoinAndFlatten::demonstrate();
    ElementsView::demonstrate();
    RealWorldExample::demonstrate();
    BestPractices::demonstrate();

    std::println("");
    std::println("{}", std::string(60, '='));
    std::println("🎓 RANGES MASTERY CHECKLIST:");
    std::println("{}", std::string(60, '='));
    std::println("{}", std::string(60, '='));
    std::println("");

    return 0;
}

/*
 * COMPILE AND RUN:
 * ===============
 * g++ -std=c++20 -Wall -Wextra ranges_introduction.cpp -o ranges_intro
 * ./ranges_intro
 *
 * REQUIREMENTS:
 * ============
 * - C++20 compiler (GCC 10+, Clang 13+, MSVC 2019+)
 * - Standard library with ranges support
 *
 * KEY CONCEPTS:
 * ============
 * 1. Views are lazy and non-owning
 * 2. Pipe operator | enables composition
 * 3. Range algorithms work directly on ranges
 * 4. Lazy evaluation improves performance
 * 5. Composable and reusable
 *
 * COMMON VIEWS:
 * ============
 * - filter     - Select elements
 * - transform  - Apply function
 * - take       - First N elements
 * - drop       - Skip N elements
 * - reverse    - Reverse order
 * - split      - Split by delimiter
 * - join       - Flatten nested
 * - elements   - Extract from tuples
 * - iota       - Generate sequence
 * - keys       - Extract keys (pairs)
 * - values     - Extract values (pairs)
 *
 * C++20 QUIRKS:
 * ============
 * - split returns subranges, not strings (C++23 improves this)
 * - Some conversions require manual iteration
 * - C++23 adds ranges::to<> for easier conversions
 *
 * NEXT STEPS:
 * ===========
 * - Practice building your own pipelines
 * - Explore TemplateMeta/ for compile-time patterns
 * - Study ../../RangesAndGenericStyle/ for production examples
 * - Read C++20 ranges documentation
 * - Consider upgrading to C++23 for ranges::to<>
 */
