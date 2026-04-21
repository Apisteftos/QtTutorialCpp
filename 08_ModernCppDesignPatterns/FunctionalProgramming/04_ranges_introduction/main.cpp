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
 * Compile: g++ -std=c++20 -Wall -Wextra ranges_introduction.cpp -o ranges_intro
 */

#include <iostream>
#include <vector>
#include <ranges>
#include <algorithm>
#include <string>
#include <numeric>

// ============================================================================
// PART 1: Basic Range Views
// ============================================================================

namespace BasicViews {

void demonstrate() {
    std::cout << "\n=== Basic Range Views ===\n";

    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // filter - select elements matching predicate
    std::cout << "\nfilter (even numbers): ";
    auto evens = numbers | std::views::filter([](int n) { return n % 2 == 0; });
    for (auto n : evens) std::cout << n << " ";

    // transform - apply function to each element
    std::cout << "\ntransform (square): ";
    auto squared = numbers | std::views::transform([](int n) { return n * n; });
    for (auto n : squared) std::cout << n << " ";

    // take - first N elements
    std::cout << "\ntake (first 5): ";
    auto first5 = numbers | std::views::take(5);
    for (auto n : first5) std::cout << n << " ";

    // drop - skip first N elements
    std::cout << "\ndrop (skip first 5): ";
    auto skip5 = numbers | std::views::drop(5);
    for (auto n : skip5) std::cout << n << " ";

    // reverse - elements in reverse order
    std::cout << "\nreverse: ";
    auto reversed = numbers | std::views::reverse;
    for (auto n : reversed) std::cout << n << " ";

    std::cout << "\n\n✅ Views are:\n";
    std::cout << "   - Lazy (computed on-demand)\n";
    std::cout << "   - Composable (chain multiple views)\n";
    std::cout << "   - Non-owning (don't copy data)\n";
}
}

// ============================================================================
// PART 2: Lazy Evaluation - The Power of Ranges
// ============================================================================

namespace LazyEvaluation {

void demonstrate() {
    std::cout << "\n=== Lazy Evaluation ===\n";

    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    std::cout << "\nCreating a view pipeline (no computation yet):\n";
    auto pipeline = numbers
                    | std::views::filter([](int n) {
                          std::cout << "  Filtering " << n << "\n";
                          return n % 2 == 0;
                      })
                    | std::views::transform([](int n) {
                          std::cout << "  Transforming " << n << "\n";
                          return n * n;
                      })
                    | std::views::take(3);

    std::cout << "\nPipeline created but not executed!\n";
    std::cout << "\nNow iterating (computation happens):\n";

    for (auto n : pipeline) {
        std::cout << "Result: " << n << "\n";
    }

    std::cout << "\n✅ Notice:\n";
    std::cout << "   - Only filtered/transformed 6 elements (not all 10!)\n";
    std::cout << "   - Stopped after finding 3 matches\n";
    std::cout << "   - This is LAZY EVALUATION!\n";
}
}

// ============================================================================
// PART 3: Composing Multiple Views (Pipelines)
// ============================================================================

namespace PipelineComposition {

void demonstrate() {
    std::cout << "\n=== Pipeline Composition ===\n";

    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

    // Complex pipeline: filter evens, square, take 5, reverse
    std::cout << "\nComplex pipeline:\n";
    std::cout << "numbers | filter(even) | transform(square) | take(5) | reverse\n";
    std::cout << "Result: ";

    auto result = numbers
                  | std::views::filter([](int n) { return n % 2 == 0; })
                  | std::views::transform([](int n) { return n * n; })
                  | std::views::take(5)
                  | std::views::reverse;

    for (auto n : result) std::cout << n << " ";

    // Another example: filter > 5, double, skip first 2
    std::cout << "\n\nAnother pipeline:\n";
    std::cout << "numbers | filter(>5) | transform(*2) | drop(2)\n";
    std::cout << "Result: ";

    auto result2 = numbers
                   | std::views::filter([](int n) { return n > 5; })
                   | std::views::transform([](int n) { return n * 2; })
                   | std::views::drop(2);

    for (auto n : result2) std::cout << n << " ";

    std::cout << "\n\n✅ Pipelines are:\n";
    std::cout << "   - Readable (left-to-right data flow)\n";
    std::cout << "   - Efficient (lazy evaluation)\n";
    std::cout << "   - Composable (build complex from simple)\n";
}
}

// ============================================================================
// PART 4: Common Range Algorithms
// ============================================================================

namespace RangeAlgorithms {

void demonstrate() {
    std::cout << "\n=== Range Algorithms ===\n";

    std::vector<int> numbers = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};

    // std::ranges::sort
    auto sorted = numbers;
    std::ranges::sort(sorted);
    std::cout << "\nSorted: ";
    for (auto n : sorted) std::cout << n << " ";

    // std::ranges::find
    auto it = std::ranges::find(numbers, 5);
    if (it != numbers.end()) {
        std::cout << "\nFound 5 at position: "
                  << std::distance(numbers.begin(), it);
    }

    // std::ranges::count
    auto count = std::ranges::count(numbers, 1);
    std::cout << "\nCount of 1: " << count;

    // std::ranges::count_if
    auto evens_count = std::ranges::count_if(numbers,
                                             [](int n) { return n % 2 == 0; });
    std::cout << "\nCount of even numbers: " << evens_count;

    // std::ranges::any_of, all_of, none_of
    bool hasEven = std::ranges::any_of(numbers,
                                       [](int n) { return n % 2 == 0; });
    std::cout << "\nHas even numbers: " << (hasEven ? "yes" : "no");

    bool allPositive = std::ranges::all_of(numbers,
                                           [](int n) { return n > 0; });
    std::cout << "\nAll positive: " << (allPositive ? "yes" : "no");

    // std::ranges::max, min
    auto max = std::ranges::max(numbers);
    auto min = std::ranges::min(numbers);
    std::cout << "\nMin: " << min << ", Max: " << max;

    std::cout << "\n\n✅ Range algorithms:\n";
    std::cout << "   - Cleaner syntax than iterator-based\n";
    std::cout << "   - Work directly with ranges\n";
    std::cout << "   - More expressive\n";
}
}

// ============================================================================
// PART 5: Working with Strings
// ============================================================================

namespace StringRanges {

void demonstrate() {
    std::cout << "\n=== Ranges with Strings ===\n";

    std::string text = "Hello, World! This is C++20 Ranges!";

    // Filter uppercase letters
    std::cout << "\nUppercase letters: ";
    auto uppercase = text
                     | std::views::filter([](char c) { return std::isupper(c); });
    for (char c : uppercase) std::cout << c;

    // Convert to uppercase
    std::cout << "\nTo uppercase: ";
    auto uppercased = text
                      | std::views::transform([](char c) { return std::toupper(c); });
    for (char c : uppercased) std::cout << c;

    // Take first 5 chars
    std::cout << "\nFirst 5 chars: ";
    auto first5 = text | std::views::take(5);
    for (char c : first5) std::cout << c;

    // Split into words (using views::split)
    // Note: In C++20, split returns subranges that we iterate over directly
    std::cout << "\nWords:\n";
    for (auto word : text | std::views::split(' ')) {
        std::cout << "  '";
        for (char c : word) {
            std::cout << c;
        }
        std::cout << "'\n";
    }

    std::cout << "\n💡 C++20 note: split returns subranges, not strings.\n";
    std::cout << "   C++23 adds ranges::to<> for easier conversion.\n";

    std::cout << "\n✅ String ranges enable:\n";
    std::cout << "   - Character-level transformations\n";
    std::cout << "   - Easy filtering\n";
    std::cout << "   - String splitting\n";
}
}

// ============================================================================
// PART 6: iota - Generate Sequences
// ============================================================================

namespace IotaView {

void demonstrate() {
    std::cout << "\n=== iota - Generate Sequences ===\n";

    // Generate numbers 1 to 10
    std::cout << "\niota(1, 11): ";
    for (auto n : std::views::iota(1, 11)) {
        std::cout << n << " ";
    }

    // Infinite sequence (take first 10)
    std::cout << "\nInfinite iota(1) | take(10): ";
    for (auto n : std::views::iota(1) | std::views::take(10)) {
        std::cout << n << " ";
    }

    // Generate and transform
    std::cout << "\niota(1, 6) squared: ";
    auto squares = std::views::iota(1, 6)
                   | std::views::transform([](int n) { return n * n; });
    for (auto n : squares) std::cout << n << " ";

    // Even numbers using iota
    std::cout << "\nEven numbers 2-20: ";
    auto evens = std::views::iota(1, 11)
                 | std::views::transform([](int n) { return n * 2; });
    for (auto n : evens) std::cout << n << " ";

    std::cout << "\n\n✅ iota is useful for:\n";
    std::cout << "   - Generating sequences\n";
    std::cout << "   - Index generation\n";
    std::cout << "   - Infinite sequences (with take)\n";
}
}

// ============================================================================
// PART 7: join and flatten
// ============================================================================

namespace JoinAndFlatten {

void demonstrate() {
    std::cout << "\n=== join - Flatten Nested Ranges ===\n";

    std::vector<std::vector<int>> nested = {
        {1, 2, 3},
        {4, 5},
        {6, 7, 8, 9}
    };

    std::cout << "\nNested vector: {{1,2,3}, {4,5}, {6,7,8,9}}\n";
    std::cout << "Flattened: ";

    auto flattened = nested | std::views::join;
    for (auto n : flattened) std::cout << n << " ";

    // Practical example: words to characters
    std::cout << "\n\nWords to characters:\n";
    std::vector<std::string> words = {"Hello", "World", "C++20"};

    std::cout << "Words: ";
    for (const auto& word : words) std::cout << word << " ";

    std::cout << "\nAll characters: ";
    auto chars = words | std::views::join;
    for (char c : chars) std::cout << c;

    std::cout << "\n\n✅ join useful for:\n";
    std::cout << "   - Flattening nested containers\n";
    std::cout << "   - Processing multi-level data\n";
    std::cout << "   - Combining sequences\n";
}
}

// ============================================================================
// PART 8: elements - Access Tuple Elements
// ============================================================================

namespace ElementsView {

void demonstrate() {
    std::cout << "\n=== elements - Access Tuple Elements ===\n";

    std::vector<std::pair<int, std::string>> data = {
        {1, "Alice"},
        {2, "Bob"},
        {3, "Charlie"},
        {4, "Diana"}
    };

    std::cout << "\nOriginal pairs:\n";
    for (const auto& [id, name] : data) {
        std::cout << "  " << id << ": " << name << "\n";
    }

    // Extract first elements (keys)
    std::cout << "\nIDs only (elements<0>): ";
    auto ids = data | std::views::elements<0>;
    for (auto id : ids) std::cout << id << " ";

    // Extract second elements (values)
    std::cout << "\nNames only (elements<1>): ";
    auto names = data | std::views::elements<1>;
    for (const auto& name : names) std::cout << name << " ";

    // Alias: keys and values
    std::cout << "\n\nUsing keys/values aliases:\n";
    std::cout << "Keys: ";
    for (auto key : data | std::views::keys) std::cout << key << " ";

    std::cout << "\nValues: ";
    for (const auto& value : data | std::views::values) std::cout << value << " ";

    std::cout << "\n\n✅ elements useful for:\n";
    std::cout << "   - Extracting from pairs/tuples\n";
    std::cout << "   - Working with map-like structures\n";
    std::cout << "   - Projecting specific fields\n";
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
    std::cout << "\n=== Real-World: Product Analysis ===\n";

    std::vector<Product> products = {
                                     {"Laptop", 1200.0, 5, "Electronics"},
                                     {"Mouse", 25.0, 50, "Electronics"},
                                     {"Keyboard", 75.0, 30, "Electronics"},
                                     {"Desk", 300.0, 10, "Furniture"},
                                     {"Chair", 150.0, 20, "Furniture"},
                                     {"Monitor", 400.0, 15, "Electronics"},
                                     };

    // Task 1: Total value of electronics
    std::cout << "\nTask 1: Total value of Electronics\n";
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
    std::cout << "Total: $" << total << "\n";

    // Task 2: Expensive products (> $100)
    std::cout << "\nTask 2: Expensive products (>$100):\n";
    auto expensive = products
                     | std::views::filter([](const Product& p) { return p.price > 100; })
                     | std::views::transform([](const Product& p) { return p.name; });

    for (const auto& name : expensive) {
        std::cout << "  - " << name << "\n";
    }

    // Task 3: Low stock items (< 20)
    std::cout << "\nTask 3: Low stock items (<20):\n";
    auto low_stock = products
                     | std::views::filter([](const Product& p) { return p.quantity < 20; })
                     | std::views::take(3);  // Top 3

    for (const auto& p : low_stock) {
        std::cout << "  - " << p.name << " (qty: " << p.quantity << ")\n";
    }

    // Task 4: Price range
    std::cout << "\nTask 4: Prices sorted:\n";
    auto prices = products
                  | std::views::transform([](const Product& p) { return p.price; });

    std::vector<double> price_vec(prices.begin(), prices.end());
    std::ranges::sort(price_vec);

    std::cout << "  Min: $" << price_vec.front()
              << ", Max: $" << price_vec.back() << "\n";

    std::cout << "\n✅ Ranges enable clean data pipelines:\n";
    std::cout << "   - Readable business logic\n";
    std::cout << "   - Easy to modify and extend\n";
    std::cout << "   - Efficient (lazy evaluation)\n";
    std::cout << "   - Type-safe\n";
}
}

// ============================================================================
// PART 10: Performance and Best Practices
// ============================================================================

namespace BestPractices {

void demonstrate() {
    std::cout << "\n=== Best Practices ===\n";

    std::vector<int> large_data(1000);
    std::iota(large_data.begin(), large_data.end(), 1);

    std::cout << "\n✅ DO: Use ranges for clarity\n";
    std::cout << "auto result = data | filter(...) | transform(...);\n";

    std::cout << "\n✅ DO: Leverage lazy evaluation\n";
    std::cout << "auto result = data | view1 | view2 | take(10);\n";
    std::cout << "(Only processes what's needed!)\n";

    std::cout << "\n✅ DO: Compose reusable views\n";
    std::cout << "auto evens = numbers | filter(is_even);\n";
    std::cout << "auto squared_evens = evens | transform(square);\n";

    std::cout << "\n❌ DON'T: Over-complicate simple cases\n";
    std::cout << "For simple loops, traditional for may be clearer\n";

    std::cout << "\n❌ DON'T: Materialize unless needed\n";
    std::cout << "Views are lazy - don't convert to vector unnecessarily\n";

    std::cout << "\n💡 Performance tips:\n";
    std::cout << "   - Views don't copy data (zero overhead)\n";
    std::cout << "   - Lazy evaluation = better performance\n";
    std::cout << "   - Composable = reusable code\n";
    std::cout << "   - Type-safe = fewer bugs\n";

    std::cout << "\n💡 C++23 upgrade: ranges::to<> makes conversions easier\n";
    std::cout << "   auto vec = range | ranges::to<std::vector>(); // C++23\n";
}
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
    std::cout << "\n╔═══════════════════════════════════════════════════════╗\n";
    std::cout << "║              C++20 RANGES INTRODUCTION              ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════╝\n";

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

    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "🎓 RANGES MASTERY CHECKLIST:\n";
    std::cout << std::string(60, '=') << "\n";
    std::cout << "✅ Understand lazy evaluation\n";
    std::cout << "✅ Use pipe operator for composition\n";
    std::cout << "✅ Know common views (filter, transform, take, drop)\n";
    std::cout << "✅ Use range algorithms (sort, find, count, etc.)\n";
    std::cout << "✅ Work with iota for sequences\n";
    std::cout << "✅ Flatten with join\n";
    std::cout << "✅ Extract with elements/keys/values\n";
    std::cout << "✅ Build production pipelines\n";
    std::cout << "\n💡 think-cell Philosophy:\n";
    std::cout << "   'We prefer functional-style generic algorithms\n";
    std::cout << "    and ranges over imperative programming.'\n";
    std::cout << "\n🚀 Ranges are the FUTURE of C++ collections!\n";
    std::cout << std::string(60, '=') << "\n\n";

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
 * - Study ThinkCellPhilosophy/ for production examples
 * - Read C++20 ranges documentation
 * - Consider upgrading to C++23 for ranges::to<>
 */
