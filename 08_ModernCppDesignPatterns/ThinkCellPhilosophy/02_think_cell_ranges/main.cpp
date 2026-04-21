/*
 * think_cell_ranges.cpp
 *
 * Demonstrating think-cell's Range-Based Programming Philosophy
 *
 * think-cell has developed their own range library that goes beyond Boost.Range,
 * unifying internal and external iteration. This file shows examples inspired
 * by their approach.
 *
 * Core Principles:
 * - Functional composition over imperative loops
 * - Lazy evaluation for performance
 * - Composable, reusable operations
 * - Zero-cost abstractions
 * - Clear intent over implementation details
 *
 * Compile: g++ -std=c++20 -O3 -Wall -Wextra think_cell_ranges.cpp -o tc_ranges
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <map>
#include <functional>

// ============================================================================
// EXAMPLE 1: Data Processing Pipeline (think-cell Style)
// ============================================================================

namespace DataProcessing {

// ❌ Traditional imperative approach
std::vector<int> process_imperative(const std::vector<int>& data) {
    std::vector<int> result;
    for (const auto& value : data) {
        if (value > 0) {                    // Filter positive
            if (value % 2 == 0) {            // Filter even
                result.push_back(value * value);  // Square
            }
        }
    }
    return result;
}

// ✅ think-cell style: Composable pipeline
auto process_functional(const std::vector<int>& data) {
    return data
           | std::views::filter([](int x) { return x > 0; })      // Positive
           | std::views::filter([](int x) { return x % 2 == 0; }) // Even
           | std::views::transform([](int x) { return x * x; });  // Square
}

void demonstrate() {
    std::cout << "\n=== Example 1: Data Processing Pipeline ===\n";

    std::vector<int> data = {-5, 2, -3, 4, 6, 1, 8, -2, 10};

    std::cout << "\nInput: ";
    for (int x : data) std::cout << x << " ";

    std::cout << "\n\nImperative result: ";
    auto result1 = process_imperative(data);
    for (int x : result1) std::cout << x << " ";

    std::cout << "\n\nFunctional result:  ";
    for (int x : process_functional(data)) std::cout << x << " ";

    std::cout << "\n\n✅ Functional is more readable and composable!\n";
    std::cout << "✅ Same performance (zero-cost abstraction)\n";
}
}

// ============================================================================
// EXAMPLE 2: Chart Data Processing (Real-world think-cell Scenario)
// ============================================================================

namespace ChartProcessing {

struct DataPoint {
    std::string label;
    double value;
    bool is_valid;
};

// ❌ Traditional approach: Nested loops and temporary vectors
std::vector<std::string> get_valid_labels_imperative(
    const std::vector<DataPoint>& points) {

    std::vector<std::string> result;
    for (const auto& point : points) {
        if (point.is_valid && point.value > 0) {
            result.push_back(point.label);
        }
    }
    return result;
}

// ✅ think-cell style: Pipeline that expresses intent
auto get_valid_labels_functional(const std::vector<DataPoint>& points) {
    return points
           | std::views::filter([](const DataPoint& p) {
                 return p.is_valid && p.value > 0;
             })
           | std::views::transform([](const DataPoint& p) {
                 return p.label;
             });
}

void demonstrate() {
    std::cout << "\n=== Example 2: Chart Data Processing ===\n";

    std::vector<DataPoint> chart_data = {
        {"Q1", 100.0, true},
        {"Q2", -50.0, true},   // Negative value
        {"Q3", 75.0, false},   // Invalid
        {"Q4", 120.0, true},
        {"Q5", 0.0, true}      // Zero value
    };

    std::cout << "\nImperative approach:\n";
    auto labels1 = get_valid_labels_imperative(chart_data);
    for (const auto& label : labels1) {
        std::cout << "  " << label << "\n";
    }

    std::cout << "\nFunctional approach:\n";
    for (const auto& label : get_valid_labels_functional(chart_data)) {
        std::cout << "  " << label << "\n";
    }

    std::cout << "\n✅ Pipeline clearly shows: filter → transform\n";
    std::cout << "✅ Easy to add more processing steps\n";
}
}

// ============================================================================
// EXAMPLE 3: Range Composition (think-cell's Custom Range Library Style)
// ============================================================================

namespace RangeComposition {

// Reusable filter predicates
auto is_positive = [](int x) { return x > 0; };
auto is_even = [](int x) { return x % 2 == 0; };
auto is_large = [](int x) { return x > 50; };

// Reusable transformations
auto square = [](int x) { return x * x; };
auto double_it = [](int x) { return x * 2; };

// Build complex pipelines from simple parts
template<typename Range>
auto process_pipeline_1(Range&& r) {
    return std::forward<Range>(r)
           | std::views::filter(is_positive)
           | std::views::transform(square);
}

template<typename Range>
auto process_pipeline_2(Range&& r) {
    return std::forward<Range>(r)
           | std::views::filter(is_even)
           | std::views::filter(is_large)
           | std::views::transform(double_it);
}

void demonstrate() {
    std::cout << "\n=== Example 3: Range Composition ===\n";

    std::vector<int> data = {-5, 2, 3, 4, 8, 10, 15};

    std::cout << "\nOriginal: ";
    for (int x : data) std::cout << x << " ";

    std::cout << "\n\nPipeline 1 (positive → square): ";
    for (int x : process_pipeline_1(data)) {
        std::cout << x << " ";
    }

    std::cout << "\n\nPipeline 2 (even → large → double): ";
    for (int x : process_pipeline_2(data)) {
        std::cout << x << " ";
    }

    std::cout << "\n\n✅ Pipelines are composable and reusable!\n";
    std::cout << "✅ Named operations express intent clearly\n";
}
}

// ============================================================================
// EXAMPLE 4: Lazy Evaluation (Performance Benefit)
// ============================================================================

namespace LazyEvaluation {

// Expensive operation
int expensive_transform(int x) {
    // Simulate expensive computation
    std::cout << "  [Computing " << x << "]\n";
    return x * x;
}

void demonstrate() {
    std::cout << "\n=== Example 4: Lazy Evaluation ===\n";

    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    std::cout << "\n❌ Eager evaluation (processes ALL elements):\n";
    std::vector<int> eager_result;
    for (int x : data) {
        int transformed = expensive_transform(x);
        if (x > 5) {
            eager_result.push_back(transformed);
        }
    }
    std::cout << "Result: ";
    for (int x : eager_result) std::cout << x << " ";

    std::cout << "\n\n✅ Lazy evaluation (processes ONLY needed elements):\n";
    auto lazy_result = data
                       | std::views::filter([](int x) { return x > 5; })
                       | std::views::transform(expensive_transform);

    std::cout << "Result: ";
    for (int x : lazy_result) std::cout << x << " ";

    std::cout << "\n\n✅ Lazy evaluation = Better performance!\n";
    std::cout << "✅ Only computes what's actually used\n";
}
}

// ============================================================================
// EXAMPLE 5: Working with Maps (Key-Value Processing)
// ============================================================================

namespace MapProcessing {

void demonstrate() {
    std::cout << "\n=== Example 5: Map Processing ===\n";

    std::map<std::string, int> scores = {
        {"Alice", 95},
        {"Bob", 72},
        {"Charlie", 88},
        {"David", 60},
        {"Eve", 91}
    };

    // Extract high scores (functional style)
    std::cout << "\nHigh scorers (>= 85):\n";
    for (const auto& [name, score] : scores
                                         | std::views::filter([](const auto& p) {
                                               return p.second >= 85;
                                           })) {
        std::cout << "  " << name << ": " << score << "\n";
    }

    // Transform scores
    std::cout << "\nBoosted scores (+10 if < 80):\n";
    for (const auto& [name, score] : scores) {
        int boosted = score < 80 ? score + 10 : score;
        std::cout << "  " << name << ": " << boosted << "\n";
    }

    std::cout << "\n✅ Ranges work beautifully with maps!\n";
    std::cout << "✅ Structured bindings make it even cleaner\n";
}
}

// ============================================================================
// EXAMPLE 6: Index-Based Processing (When You Need It)
// ============================================================================

namespace IndexedProcessing {

void demonstrate() {
    std::cout << "\n=== Example 6: Indexed Processing ===\n";

    std::vector<std::string> items = {"Apple", "Banana", "Cherry", "Date"};

    // Use views::enumerate (C++23) or manual approach
    std::cout << "\nIndexed items:\n";
    size_t index = 0;
    for (const auto& item : items) {
        std::cout << "  [" << index++ << "] " << item << "\n";
    }

    // Filter by index
    std::cout << "\nEven-indexed items:\n";
    for (size_t i = 0; i < items.size(); i += 2) {
        std::cout << "  " << items[i] << "\n";
    }

    std::cout << "\n✅ Sometimes you need indices - that's OK!\n";
    std::cout << "✅ Use the right tool for the job\n";
}
}

// ============================================================================
// EXAMPLE 7: Range Algorithms (STL Algorithms with Ranges)
// ============================================================================

namespace RangeAlgorithms {

void demonstrate() {
    std::cout << "\n=== Example 7: Range Algorithms ===\n";

    std::vector<int> data = {5, 2, 8, 1, 9, 3, 7};

    std::cout << "\nOriginal: ";
    for (int x : data) std::cout << x << " ";

    // Sort (modifies container)
    std::ranges::sort(data);
    std::cout << "\nSorted: ";
    for (int x : data) std::cout << x << " ";

    // Find
    auto it = std::ranges::find(data, 8);
    if (it != data.end()) {
        std::cout << "\n\nFound 8 at position "
                  << std::distance(data.begin(), it);
    }

    // Count
    int count = std::ranges::count_if(data, [](int x) { return x > 5; });
    std::cout << "\nNumbers > 5: " << count;

    // All, any, none
    bool all_positive = std::ranges::all_of(data, [](int x) { return x > 0; });
    std::cout << "\nAll positive: " << std::boolalpha << all_positive;

    std::cout << "\n\n✅ Range algorithms = Cleaner than iterator pairs!\n";
    std::cout << "✅ Work directly on containers\n";
}
}

// ============================================================================
// EXAMPLE 8: Real-World: Text Processing Pipeline
// ============================================================================

namespace TextProcessing {

void demonstrate() {
    std::cout << "\n=== Example 8: Text Processing Pipeline ===\n";

    std::vector<std::string> lines = {
        "# This is a comment",
        "data1: 100",
        "",
        "data2: 200",
        "# Another comment",
        "data3: 150"
    };

    std::cout << "\nProcessing log file:\n";
    std::cout << "Valid data lines:\n";

    // Filter and process in one pipeline
    for (const auto& line : lines
                                | std::views::filter([](const std::string& s) {
                                      return !s.empty() && s[0] != '#';
                                  })) {
        std::cout << "  " << line << "\n";
    }

    std::cout << "\n✅ Clear data extraction pipeline\n";
    std::cout << "✅ Easy to extend with more filters/transforms\n";
}
}

// ============================================================================
// MAIN - Demonstrate All Examples
// ============================================================================

int main() {
    std::cout << "\n╔═══════════════════════════════════════════════════════╗\n";
    std::cout << "║         think-cell RANGE-BASED PROGRAMMING          ║\n";
    std::cout << "║                                                     ║\n";
    std::cout << "║  \"We prefer functional-style generic algorithms     ║\n";
    std::cout << "║   and ranges over imperative programming.\"          ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════╝\n";

    DataProcessing::demonstrate();
    ChartProcessing::demonstrate();
    RangeComposition::demonstrate();
    LazyEvaluation::demonstrate();
    MapProcessing::demonstrate();
    IndexedProcessing::demonstrate();
    RangeAlgorithms::demonstrate();
    TextProcessing::demonstrate();

    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "🎓 think-cell RANGE PHILOSOPHY:\n";
    std::cout << std::string(60, '=') << "\n";
    std::cout << "1. Functional composition over imperative loops\n";
    std::cout << "2. Lazy evaluation for performance\n";
    std::cout << "3. Composable, reusable operations\n";
    std::cout << "4. Clear intent over implementation\n";
    std::cout << "5. Zero-cost abstractions\n";
    std::cout << "\n💡 Key Benefits:\n";
    std::cout << "   ✅ More readable - See WHAT, not HOW\n";
    std::cout << "   ✅ More composable - Build complex from simple\n";
    std::cout << "   ✅ More reusable - Named operations\n";
    std::cout << "   ✅ Better performance - Lazy evaluation\n";
    std::cout << "   ✅ Fewer bugs - Less manual iteration\n";
    std::cout << "\n🚀 think-cell's Custom Range Library:\n";
    std::cout << "   - Goes beyond Boost.Range\n";
    std::cout << "   - Unifies internal and external iteration\n";
    std::cout << "   - Zero-cost abstractions\n";
    std::cout << "   - Production-proven in millions of installations\n";
    std::cout << "\n💭 Remember:\n";
    std::cout << "   \"Code should read like what it does,\n";
    std::cout << "    not how it does it.\"\n";
    std::cout << std::string(60, '=') << "\n\n";

    return 0;
}

/*
 * COMPILE AND RUN:
 * ===============
 * g++ -std=c++20 -O3 -Wall -Wextra think_cell_ranges.cpp -o tc_ranges
 * ./tc_ranges
 *
 * REQUIREMENTS:
 * ============
 * - C++20 compiler (for ranges)
 * - Optimization flag (-O3) to see zero-cost abstractions
 *
 * KEY CONCEPTS DEMONSTRATED:
 * =========================
 * 1. Imperative vs Functional comparison
 * 2. Data processing pipelines
 * 3. Range composition and reuse
 * 4. Lazy evaluation benefits
 * 5. Working with complex data structures
 * 6. Range algorithms
 * 7. Real-world text processing
 *
 * think-cell PRINCIPLES:
 * =====================
 * ✅ Prefer ranges over raw loops
 * ✅ Build pipelines from composable parts
 * ✅ Name operations to express intent
 * ✅ Use lazy evaluation for performance
 * ✅ Trust the compiler for optimization
 *
 * WHEN TO USE RANGES:
 * ==================
 * ✅ Data transformation pipelines
 * ✅ Filtering and mapping operations
 * ✅ Complex multi-step processing
 * ✅ When you need composability
 *
 * WHEN TO USE LOOPS:
 * =================
 * ✅ Simple iteration without transformation
 * ✅ When you need low-level control
 * ✅ Performance-critical tight loops (profile first!)
 * ✅ Very simple operations (for display, etc.)
 *
 * NEXT STEPS:
 * ===========
 * - Study think_cell_templates.cpp for template patterns
 * - Practice building your own range pipelines
 * - Explore Boost.Range for more advanced features
 * - Read C++23 ranges additions (views::enumerate, etc.)
 */
