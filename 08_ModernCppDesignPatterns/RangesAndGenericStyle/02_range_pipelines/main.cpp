/*
 * 02_range_pipelines
 *
 * Range-based programming: composable views instead of hand-written loops,
 * lazy evaluation, reusable named operations, range algorithms.
 *
 * Compile: g++ -std=c++23 -O3 -Wall -Wextra main.cpp -o example
 */

#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <map>
#include <functional>
#include <print>

// ============================================================================
// EXAMPLE 1: Data Processing Pipeline
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

// ✅ Composable pipeline
auto process_functional(const std::vector<int>& data) {
    return data
           | std::views::filter([](int x) { return x > 0; })      // Positive
           | std::views::filter([](int x) { return x % 2 == 0; }) // Even
           | std::views::transform([](int x) { return x * x; });  // Square
}

void demonstrate() {
    std::println("");
    std::println("=== Example 1: Data Processing Pipeline ===");

    std::vector<int> data = {-5, 2, -3, 4, 6, 1, 8, -2, 10};

    std::println("");
    std::print("Input: ");
    for (int x : data) std::print("{} ", x);

    std::println("");
    std::println("");
    std::print("Imperative result: ");
    auto result1 = process_imperative(data);
    for (int x : result1) std::print("{} ", x);

    std::println("");
    std::println("");
    std::print("Functional result:  ");
    for (int x : process_functional(data)) std::print("{} ", x);

    std::println("");
}
}

// ============================================================================
// EXAMPLE 2: Report Data Processing
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

// ✅ Pipeline that expresses intent
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
    std::println("");
    std::println("=== Example 2: Report Data Processing ===");

    std::vector<DataPoint> chart_data = {
        {"Q1", 100.0, true},
        {"Q2", -50.0, true},   // Negative value
        {"Q3", 75.0, false},   // Invalid
        {"Q4", 120.0, true},
        {"Q5", 0.0, true}      // Zero value
    };

    std::println("");
    std::println("Imperative approach:");
    auto labels1 = get_valid_labels_imperative(chart_data);
    for (const auto& label : labels1) {
        std::println("  {}", label);
    }

    std::println("");
    std::println("Functional approach:");
    for (const auto& label : get_valid_labels_functional(chart_data)) {
        std::println("  {}", label);
    }

}
}

// ============================================================================
// EXAMPLE 3: Range Composition (Reusable Named Operations)
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
    std::println("");
    std::println("=== Example 3: Range Composition ===");

    std::vector<int> data = {-5, 2, 3, 4, 8, 10, 15};

    std::println("");
    std::print("Original: ");
    for (int x : data) std::print("{} ", x);

    std::println("");
    std::println("");
    std::print("Pipeline 1 (positive → square): ");
    for (int x : process_pipeline_1(data)) {
        std::print("{} ", x);
    }

    std::println("");
    std::println("");
    std::print("Pipeline 2 (even → large → double): ");
    for (int x : process_pipeline_2(data)) {
        std::print("{} ", x);
    }

    std::println("");
}
}

// ============================================================================
// EXAMPLE 4: Lazy Evaluation (Performance Benefit)
// ============================================================================

namespace LazyEvaluation {

// Expensive operation
int expensive_transform(int x) {
    // Simulate expensive computation
    std::println("  [Computing {}]", x);
    return x * x;
}

void demonstrate() {
    std::println("");
    std::println("=== Example 4: Lazy Evaluation ===");

    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    std::println("");
    std::println("❌ Eager evaluation (processes ALL elements):");
    std::vector<int> eager_result;
    for (int x : data) {
        int transformed = expensive_transform(x);
        if (x > 5) {
            eager_result.push_back(transformed);
        }
    }
    std::print("Result: ");
    for (int x : eager_result) std::print("{} ", x);

    std::println("");
    std::println("");
    std::println("✅ Lazy evaluation (processes ONLY needed elements):");
    auto lazy_result = data
                       | std::views::filter([](int x) { return x > 5; })
                       | std::views::transform(expensive_transform);

    std::print("Result: ");
    for (int x : lazy_result) std::print("{} ", x);

    std::println("");
}
}

// ============================================================================
// EXAMPLE 5: Working with Maps (Key-Value Processing)
// ============================================================================

namespace MapProcessing {

void demonstrate() {
    std::println("");
    std::println("=== Example 5: Map Processing ===");

    std::map<std::string, int> scores = {
        {"Alice", 95},
        {"Bob", 72},
        {"Charlie", 88},
        {"David", 60},
        {"Eve", 91}
    };

    // Extract high scores (functional style)
    std::println("");
    std::println("High scorers (>= 85):");
    for (const auto& [name, score] : scores
                                         | std::views::filter([](const auto& p) {
                                               return p.second >= 85;
                                           })) {
        std::println("  {}: {}", name, score);
    }

    // Transform scores
    std::println("");
    std::println("Boosted scores (+10 if < 80):");
    for (const auto& [name, score] : scores) {
        int boosted = score < 80 ? score + 10 : score;
        std::println("  {}: {}", name, boosted);
    }

}
}

// ============================================================================
// EXAMPLE 6: Index-Based Processing (When You Need It)
// ============================================================================

namespace IndexedProcessing {

void demonstrate() {
    std::println("");
    std::println("=== Example 6: Indexed Processing ===");

    std::vector<std::string> items = {"Apple", "Banana", "Cherry", "Date"};

    // Use views::enumerate (C++23) or manual approach
    std::println("");
    std::println("Indexed items:");
    size_t index = 0;
    for (const auto& item : items) {
        std::println("  [{}] {}", index++, item);
    }

    // Filter by index
    std::println("");
    std::println("Even-indexed items:");
    for (size_t i = 0; i < items.size(); i += 2) {
        std::println("  {}", items[i]);
    }

}
}

// ============================================================================
// EXAMPLE 7: Range Algorithms (STL Algorithms with Ranges)
// ============================================================================

namespace RangeAlgorithms {

void demonstrate() {
    std::println("");
    std::println("=== Example 7: Range Algorithms ===");

    std::vector<int> data = {5, 2, 8, 1, 9, 3, 7};

    std::println("");
    std::print("Original: ");
    for (int x : data) std::print("{} ", x);

    // Sort (modifies container)
    std::ranges::sort(data);
    std::println("");
    std::print("Sorted: ");
    for (int x : data) std::print("{} ", x);

    // Find
    auto it = std::ranges::find(data, 8);
    if (it != data.end()) {
        std::println("");
        std::println("");
        std::print("Found 8 at position {}", std::distance(data.begin(), it));
    }

    // Count
    int count = std::ranges::count_if(data, [](int x) { return x > 5; });
    std::println("");
    std::print("Numbers > 5: {}", count);

    // All, any, none
    bool all_positive = std::ranges::all_of(data, [](int x) { return x > 0; });
    std::println("");
    std::print("All positive: {}", all_positive);

    std::println("");
}
}

// ============================================================================
// EXAMPLE 8: Real-World: Text Processing Pipeline
// ============================================================================

namespace TextProcessing {

void demonstrate() {
    std::println("");
    std::println("=== Example 8: Text Processing Pipeline ===");

    std::vector<std::string> lines = {
        "# This is a comment",
        "data1: 100",
        "",
        "data2: 200",
        "# Another comment",
        "data3: 150"
    };

    std::println("");
    std::println("Processing log file:");
    std::println("Valid data lines:");

    // Filter and process in one pipeline
    for (const auto& line : lines
                                | std::views::filter([](const std::string& s) {
                                      return !s.empty() && s[0] != '#';
                                  })) {
        std::println("  {}", line);
    }

}
}

// ============================================================================
// MAIN - Demonstrate All Examples
// ============================================================================

int main() {
    std::println("╔═══════════════════════════════════════════════════════╗");
    std::println("║ RANGE-BASED PROGRAMMING                               ║");
    std::println("╚═══════════════════════════════════════════════════════╝");

    DataProcessing::demonstrate();
    ChartProcessing::demonstrate();
    RangeComposition::demonstrate();
    LazyEvaluation::demonstrate();
    MapProcessing::demonstrate();
    IndexedProcessing::demonstrate();
    RangeAlgorithms::demonstrate();
    TextProcessing::demonstrate();

    std::println("");

    return 0;
}
