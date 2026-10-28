/*
 * imperative_vs_functional.cpp
 *
 * Side-by-side comparison of Imperative vs Functional programming styles
 *
 * Difficulty: 🟢 Beginner
 *
 * Key Concepts:
 * - Imperative: Focus on HOW (step-by-step instructions)
 * - Functional: Focus on WHAT (declarative transformations)
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <vector>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <string>
#include <print>

// ============================================================================
// EXAMPLE 1: Filtering - Get even numbers
// ============================================================================

void example1_filtering() {
    std::println("");
    std::println("=== Example 1: Filtering Even Numbers ===");

    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // IMPERATIVE STYLE
    std::println("");
    std::print("Imperative (HOW): ");
    std::vector<int> evens_imperative;
    for (const auto& n : numbers) {
        if (n % 2 == 0) {                    // Manual condition checking
            evens_imperative.push_back(n);    // Manual insertion
        }
    }
    for (const auto& n : evens_imperative) {
        std::print("{} ", n);
    }

    // FUNCTIONAL STYLE (C++20 Ranges)
    std::println("");
    std::print("Functional (WHAT): ");
    auto evens_functional = numbers
                            | std::views::filter([](int n) { return n % 2 == 0; });

    for (const auto& n : evens_functional) {
        std::print("{} ", n);
    }

    std::println("");
}

// ============================================================================
// EXAMPLE 2: Transformation - Square all numbers
// ============================================================================

void example2_transformation() {
    std::println("");
    std::println("=== Example 2: Squaring Numbers ===");

    std::vector<int> numbers = {1, 2, 3, 4, 5};

    // IMPERATIVE STYLE
    std::println("");
    std::print("Imperative (HOW): ");
    std::vector<int> squared_imperative;
    for (const auto& n : numbers) {
        int result = n * n;                      // Manual calculation
        squared_imperative.push_back(result);     // Manual insertion
    }
    for (const auto& n : squared_imperative) {
        std::print("{} ", n);
    }

    // FUNCTIONAL STYLE (STL Algorithm)
    std::println("");
    std::print("Functional (STL): ");
    std::vector<int> squared_stl(numbers.size());
    std::transform(numbers.begin(), numbers.end(),
                   squared_stl.begin(),
                   [](int n) { return n * n; });
    for (const auto& n : squared_stl) {
        std::print("{} ", n);
    }

    // FUNCTIONAL STYLE (C++20 Ranges)
    std::println("");
    std::print("Functional (Ranges): ");
    auto squared_ranges = numbers
                          | std::views::transform([](int n) { return n * n; });

    for (const auto& n : squared_ranges) {
        std::print("{} ", n);
    }

    std::println("");
}

// ============================================================================
// EXAMPLE 3: Filter + Transform - Even numbers squared
// ============================================================================

void example3_filter_and_transform() {
    std::println("");
    std::println("=== Example 3: Filter Even + Square ===");

    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // IMPERATIVE STYLE
    std::println("");
    std::print("Imperative (HOW): ");
    std::vector<int> result_imperative;
    for (const auto& n : numbers) {
        if (n % 2 == 0) {                        // Step 1: Filter
            result_imperative.push_back(n * n);   // Step 2: Transform
        }
    }
    for (const auto& n : result_imperative) {
        std::print("{} ", n);
    }

    // FUNCTIONAL STYLE - Pipeline
    std::println("");
    std::print("Functional (Pipeline): ");
    auto result_functional = numbers
                             | std::views::filter([](int n) { return n % 2 == 0; })   // Step 1
                             | std::views::transform([](int n) { return n * n; });    // Step 2

    for (const auto& n : result_functional) {
        std::print("{} ", n);
    }

    std::println("");
}

// ============================================================================
// EXAMPLE 4: Reduction - Sum of squares
// ============================================================================

void example4_reduction() {
    std::println("");
    std::println("=== Example 4: Sum of Squares ===");

    std::vector<int> numbers = {1, 2, 3, 4, 5};

    // IMPERATIVE STYLE
    std::println("");
    std::print("Imperative (HOW): ");
    int sum_imperative = 0;
    for (const auto& n : numbers) {
        sum_imperative += (n * n);    // Manual accumulation
    }
    std::print("{}", sum_imperative);

    // FUNCTIONAL STYLE
    std::println("");
    std::print("Functional (WHAT): ");
    auto squared = numbers
                   | std::views::transform([](int n) { return n * n; });
    int sum_functional = std::accumulate(
        std::ranges::begin(squared),
        std::ranges::end(squared),
        0
        );
    std::print("{}", sum_functional);

    std::println("");
}

// ============================================================================
// EXAMPLE 5: Finding - First element > 50
// ============================================================================

void example5_finding() {
    std::println("");
    std::println("=== Example 5: Finding First Element > 50 ===");

    std::vector<int> numbers = {10, 25, 38, 42, 55, 68, 72};

    // IMPERATIVE STYLE
    std::println("");
    std::print("Imperative (HOW): ");
    int found_imperative = -1;
    for (const auto& n : numbers) {
        if (n > 50) {
            found_imperative = n;
            break;                    // Manual early exit
        }
    }
    std::print("{}", found_imperative);

    // FUNCTIONAL STYLE
    std::println("");
    std::print("Functional (WHAT): ");
    auto found = std::ranges::find_if(numbers, [](int n) { return n > 50; });
    if (found != numbers.end()) {
        std::print("{}", *found);
    }

    std::println("");
}

// ============================================================================
// EXAMPLE 6: Complex Pipeline - Real-world scenario
// ============================================================================

struct Person {
    std::string name;
    int age;
    double salary;
};

void example6_complex_pipeline() {
    std::println("");
    std::println("=== Example 6: Complex Data Processing ===");
    std::println("Task: Get names of people over 30 with salary > 50000");

    std::vector<Person> people = {
        {"Alice", 28, 45000},
        {"Bob", 35, 60000},
        {"Charlie", 42, 75000},
        {"Diana", 31, 55000},
        {"Eve", 29, 48000}
    };

    // IMPERATIVE STYLE
    std::println("");
    std::println("Imperative (HOW):");
    std::vector<std::string> result_imperative;
    for (const auto& person : people) {
        if (person.age > 30 && person.salary > 50000) {    // Multiple conditions
            result_imperative.push_back(person.name);       // Manual extraction
        }
    }
    for (const auto& name : result_imperative) {
        std::println("  {}", name);
    }

    // FUNCTIONAL STYLE
    std::println("");
    std::println("Functional (WHAT):");
    auto result_functional = people
                             | std::views::filter([](const Person& p) {
                                   return p.age > 30 && p.salary > 50000;
                               })
                             | std::views::transform([](const Person& p) {
                                   return p.name;
                               });

    for (const auto& name : result_functional) {
        std::println("  {}", name);
    }

}

// ============================================================================
// EXAMPLE 7: Sorting - Different approaches
// ============================================================================

void example7_sorting() {
    std::println("");
    std::println("=== Example 7: Sorting ===");

    std::vector<int> numbers = {5, 2, 8, 1, 9, 3, 7, 4, 6};

    // IMPERATIVE STYLE (Bubble Sort - for demonstration)
    std::println("");
    std::print("Imperative (Bubble Sort): ");
    std::vector<int> sorted_imperative = numbers;
    for (size_t i = 0; i < sorted_imperative.size(); ++i) {
        for (size_t j = 0; j < sorted_imperative.size() - i - 1; ++j) {
            if (sorted_imperative[j] > sorted_imperative[j + 1]) {
                std::swap(sorted_imperative[j], sorted_imperative[j + 1]);
            }
        }
    }
    for (const auto& n : sorted_imperative) {
        std::print("{} ", n);
    }

    // FUNCTIONAL STYLE
    std::println("");
    std::print("Functional (STL): ");
    std::vector<int> sorted_functional = numbers;
    std::ranges::sort(sorted_functional);
    for (const auto& n : sorted_functional) {
        std::print("{} ", n);
    }

    std::println("");
}

// ============================================================================
// EXAMPLE 8: State Management - Counter
// ============================================================================

void example8_state_management() {
    std::println("");
    std::println("=== Example 8: Counting Occurrences ===");

    std::vector<int> numbers = {1, 2, 2, 3, 3, 3, 4, 4, 4, 4};

    // IMPERATIVE STYLE - Mutable state
    std::println("");
    std::println("Imperative (Mutable State):");
    int count_imperative = 0;
    for (const auto& n : numbers) {
        if (n == 3) {
            count_imperative++;    // Mutable counter
        }
    }
    std::println("  Count of 3: {}", count_imperative);

    // FUNCTIONAL STYLE - No mutable state
    std::println("");
    std::println("Functional (No Mutable State):");
    auto count_functional = std::ranges::count(numbers, 3);
    std::println("  Count of 3: {}", count_functional);

}

// ============================================================================
// EXAMPLE 9: Lazy Evaluation - Performance benefit
// ============================================================================

void example9_lazy_evaluation() {
    std::println("");
    std::println("=== Example 9: Lazy Evaluation ===");
    std::println("Processing first 3 elements after filtering");

    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // IMPERATIVE STYLE - Processes everything
    std::println("");
    std::print("Imperative (Eager): ");
    std::vector<int> result_imperative;
    for (const auto& n : numbers) {
        if (n % 2 == 0) {
            result_imperative.push_back(n * n);
        }
    }
    // Then take first 3
    for (size_t i = 0; i < std::min(size_t(3), result_imperative.size()); ++i) {
        std::print("{} ", result_imperative[i]);
    }
    std::print("(processed all {} elements)", numbers.size());

    // FUNCTIONAL STYLE - Lazy evaluation
    std::println("");
    std::print("Functional (Lazy): ");
    auto result_functional = numbers
                             | std::views::filter([](int n) { return n % 2 == 0; })
                             | std::views::transform([](int n) { return n * n; })
                             | std::views::take(3);

    for (const auto& n : result_functional) {
        std::print("{} ", n);
    }
    std::print("(stopped after finding 3 matches)");

    std::println("");
}

// ============================================================================
// EXAMPLE 10: Composition - Building complex operations
// ============================================================================

void example10_composition() {
    std::println("");
    std::println("=== Example 10: Function Composition ===");

    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Define reusable predicates and transformations
    auto is_even = [](int n) { return n % 2 == 0; };
    auto square = [](int n) { return n * n; };
    auto is_large = [](int n) { return n > 20; };

    std::println("");
    std::print("Imperative (Nested conditions): ");
    std::vector<int> result_imperative;
    for (const auto& n : numbers) {
        if (n % 2 == 0) {                    // Check even
            int squared = n * n;              // Square
            if (squared > 20) {               // Check large
                result_imperative.push_back(squared);
            }
        }
    }
    for (const auto& n : result_imperative) {
        std::print("{} ", n);
    }

    std::println("");
    std::print("Functional (Composed pipeline): ");
    auto result_functional = numbers
                             | std::views::filter(is_even)        // Reusable predicate
                             | std::views::transform(square)      // Reusable transformation
                             | std::views::filter(is_large);      // Reusable predicate

    for (const auto& n : result_functional) {
        std::print("{} ", n);
    }

    std::println("");
}

// ============================================================================
// MAIN - Run all examples
// ============================================================================

int main() {
    std::println("╔═══════════════════════════════════════════════════════╗");
    std::println("║   IMPERATIVE vs FUNCTIONAL PROGRAMMING IN C++         ║");
    std::println("╚═══════════════════════════════════════════════════════╝");

    example1_filtering();
    example2_transformation();
    example3_filter_and_transform();
    example4_reduction();
    example5_finding();
    example6_complex_pipeline();
    example7_sorting();
    example8_state_management();
    example9_lazy_evaluation();
    example10_composition();

    return 0;
}
