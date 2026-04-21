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
 * Compile: g++ -std=c++20 -Wall -Wextra imperative_vs_functional.cpp -o imperative_vs_functional
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <string>

// ============================================================================
// EXAMPLE 1: Filtering - Get even numbers
// ============================================================================

void example1_filtering() {
    std::cout << "\n=== Example 1: Filtering Even Numbers ===\n";

    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // IMPERATIVE STYLE
    std::cout << "\nImperative (HOW): ";
    std::vector<int> evens_imperative;
    for (const auto& n : numbers) {
        if (n % 2 == 0) {                    // Manual condition checking
            evens_imperative.push_back(n);    // Manual insertion
        }
    }
    for (const auto& n : evens_imperative) {
        std::cout << n << " ";
    }

    // FUNCTIONAL STYLE (C++20 Ranges)
    std::cout << "\nFunctional (WHAT): ";
    auto evens_functional = numbers
                            | std::views::filter([](int n) { return n % 2 == 0; });

    for (const auto& n : evens_functional) {
        std::cout << n << " ";
    }

    std::cout << "\n\n✅ Functional is more declarative - says WHAT not HOW\n";
}

// ============================================================================
// EXAMPLE 2: Transformation - Square all numbers
// ============================================================================

void example2_transformation() {
    std::cout << "\n=== Example 2: Squaring Numbers ===\n";

    std::vector<int> numbers = {1, 2, 3, 4, 5};

    // IMPERATIVE STYLE
    std::cout << "\nImperative (HOW): ";
    std::vector<int> squared_imperative;
    for (const auto& n : numbers) {
        int result = n * n;                      // Manual calculation
        squared_imperative.push_back(result);     // Manual insertion
    }
    for (const auto& n : squared_imperative) {
        std::cout << n << " ";
    }

    // FUNCTIONAL STYLE (STL Algorithm)
    std::cout << "\nFunctional (STL): ";
    std::vector<int> squared_stl(numbers.size());
    std::transform(numbers.begin(), numbers.end(),
                   squared_stl.begin(),
                   [](int n) { return n * n; });
    for (const auto& n : squared_stl) {
        std::cout << n << " ";
    }

    // FUNCTIONAL STYLE (C++20 Ranges)
    std::cout << "\nFunctional (Ranges): ";
    auto squared_ranges = numbers
                          | std::views::transform([](int n) { return n * n; });

    for (const auto& n : squared_ranges) {
        std::cout << n << " ";
    }

    std::cout << "\n\n✅ Functional expresses transformation clearly\n";
}

// ============================================================================
// EXAMPLE 3: Filter + Transform - Even numbers squared
// ============================================================================

void example3_filter_and_transform() {
    std::cout << "\n=== Example 3: Filter Even + Square ===\n";

    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // IMPERATIVE STYLE
    std::cout << "\nImperative (HOW): ";
    std::vector<int> result_imperative;
    for (const auto& n : numbers) {
        if (n % 2 == 0) {                        // Step 1: Filter
            result_imperative.push_back(n * n);   // Step 2: Transform
        }
    }
    for (const auto& n : result_imperative) {
        std::cout << n << " ";
    }

    // FUNCTIONAL STYLE - Pipeline
    std::cout << "\nFunctional (Pipeline): ";
    auto result_functional = numbers
                             | std::views::filter([](int n) { return n % 2 == 0; })   // Step 1
                             | std::views::transform([](int n) { return n * n; });    // Step 2

    for (const auto& n : result_functional) {
        std::cout << n << " ";
    }

    std::cout << "\n\n✅ Pipeline clearly shows data flow: filter → transform\n";
}

// ============================================================================
// EXAMPLE 4: Reduction - Sum of squares
// ============================================================================

void example4_reduction() {
    std::cout << "\n=== Example 4: Sum of Squares ===\n";

    std::vector<int> numbers = {1, 2, 3, 4, 5};

    // IMPERATIVE STYLE
    std::cout << "\nImperative (HOW): ";
    int sum_imperative = 0;
    for (const auto& n : numbers) {
        sum_imperative += (n * n);    // Manual accumulation
    }
    std::cout << sum_imperative;

    // FUNCTIONAL STYLE
    std::cout << "\nFunctional (WHAT): ";
    auto squared = numbers
                   | std::views::transform([](int n) { return n * n; });
    int sum_functional = std::accumulate(
        std::ranges::begin(squared),
        std::ranges::end(squared),
        0
        );
    std::cout << sum_functional;

    std::cout << "\n\n✅ Functional separates transformation from aggregation\n";
}

// ============================================================================
// EXAMPLE 5: Finding - First element > 50
// ============================================================================

void example5_finding() {
    std::cout << "\n=== Example 5: Finding First Element > 50 ===\n";

    std::vector<int> numbers = {10, 25, 38, 42, 55, 68, 72};

    // IMPERATIVE STYLE
    std::cout << "\nImperative (HOW): ";
    int found_imperative = -1;
    for (const auto& n : numbers) {
        if (n > 50) {
            found_imperative = n;
            break;                    // Manual early exit
        }
    }
    std::cout << found_imperative;

    // FUNCTIONAL STYLE
    std::cout << "\nFunctional (WHAT): ";
    auto found = std::ranges::find_if(numbers, [](int n) { return n > 50; });
    if (found != numbers.end()) {
        std::cout << *found;
    }

    std::cout << "\n\n✅ Functional is more expressive about intent\n";
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
    std::cout << "\n=== Example 6: Complex Data Processing ===\n";
    std::cout << "Task: Get names of people over 30 with salary > 50000\n";

    std::vector<Person> people = {
        {"Alice", 28, 45000},
        {"Bob", 35, 60000},
        {"Charlie", 42, 75000},
        {"Diana", 31, 55000},
        {"Eve", 29, 48000}
    };

    // IMPERATIVE STYLE
    std::cout << "\nImperative (HOW):\n";
    std::vector<std::string> result_imperative;
    for (const auto& person : people) {
        if (person.age > 30 && person.salary > 50000) {    // Multiple conditions
            result_imperative.push_back(person.name);       // Manual extraction
        }
    }
    for (const auto& name : result_imperative) {
        std::cout << "  " << name << "\n";
    }

    // FUNCTIONAL STYLE
    std::cout << "\nFunctional (WHAT):\n";
    auto result_functional = people
                             | std::views::filter([](const Person& p) {
                                   return p.age > 30 && p.salary > 50000;
                               })
                             | std::views::transform([](const Person& p) {
                                   return p.name;
                               });

    for (const auto& name : result_functional) {
        std::cout << "  " << name << "\n";
    }

    std::cout << "\n✅ Functional pipeline reads like English:\n";
    std::cout << "   'people' | filter(conditions) | transform(extract name)\n";
}

// ============================================================================
// EXAMPLE 7: Sorting - Different approaches
// ============================================================================

void example7_sorting() {
    std::cout << "\n=== Example 7: Sorting ===\n";

    std::vector<int> numbers = {5, 2, 8, 1, 9, 3, 7, 4, 6};

    // IMPERATIVE STYLE (Bubble Sort - for demonstration)
    std::cout << "\nImperative (Bubble Sort): ";
    std::vector<int> sorted_imperative = numbers;
    for (size_t i = 0; i < sorted_imperative.size(); ++i) {
        for (size_t j = 0; j < sorted_imperative.size() - i - 1; ++j) {
            if (sorted_imperative[j] > sorted_imperative[j + 1]) {
                std::swap(sorted_imperative[j], sorted_imperative[j + 1]);
            }
        }
    }
    for (const auto& n : sorted_imperative) {
        std::cout << n << " ";
    }

    // FUNCTIONAL STYLE
    std::cout << "\nFunctional (STL): ";
    std::vector<int> sorted_functional = numbers;
    std::ranges::sort(sorted_functional);
    for (const auto& n : sorted_functional) {
        std::cout << n << " ";
    }

    std::cout << "\n\n✅ Functional hides complexity, focuses on intent\n";
}

// ============================================================================
// EXAMPLE 8: State Management - Counter
// ============================================================================

void example8_state_management() {
    std::cout << "\n=== Example 8: Counting Occurrences ===\n";

    std::vector<int> numbers = {1, 2, 2, 3, 3, 3, 4, 4, 4, 4};

    // IMPERATIVE STYLE - Mutable state
    std::cout << "\nImperative (Mutable State):\n";
    int count_imperative = 0;
    for (const auto& n : numbers) {
        if (n == 3) {
            count_imperative++;    // Mutable counter
        }
    }
    std::cout << "  Count of 3: " << count_imperative << "\n";

    // FUNCTIONAL STYLE - No mutable state
    std::cout << "\nFunctional (No Mutable State):\n";
    auto count_functional = std::ranges::count(numbers, 3);
    std::cout << "  Count of 3: " << count_functional << "\n";

    std::cout << "\n✅ Functional avoids mutable state\n";
}

// ============================================================================
// EXAMPLE 9: Lazy Evaluation - Performance benefit
// ============================================================================

void example9_lazy_evaluation() {
    std::cout << "\n=== Example 9: Lazy Evaluation ===\n";
    std::cout << "Processing first 3 elements after filtering\n";

    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // IMPERATIVE STYLE - Processes everything
    std::cout << "\nImperative (Eager): ";
    std::vector<int> result_imperative;
    for (const auto& n : numbers) {
        if (n % 2 == 0) {
            result_imperative.push_back(n * n);
        }
    }
    // Then take first 3
    for (size_t i = 0; i < std::min(size_t(3), result_imperative.size()); ++i) {
        std::cout << result_imperative[i] << " ";
    }
    std::cout << "(processed all " << numbers.size() << " elements)";

    // FUNCTIONAL STYLE - Lazy evaluation
    std::cout << "\nFunctional (Lazy): ";
    auto result_functional = numbers
                             | std::views::filter([](int n) { return n % 2 == 0; })
                             | std::views::transform([](int n) { return n * n; })
                             | std::views::take(3);

    for (const auto& n : result_functional) {
        std::cout << n << " ";
    }
    std::cout << "(stopped after finding 3 matches)";

    std::cout << "\n\n✅ Ranges are lazy - compute only what's needed!\n";
}

// ============================================================================
// EXAMPLE 10: Composition - Building complex operations
// ============================================================================

void example10_composition() {
    std::cout << "\n=== Example 10: Function Composition ===\n";

    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Define reusable predicates and transformations
    auto is_even = [](int n) { return n % 2 == 0; };
    auto square = [](int n) { return n * n; };
    auto is_large = [](int n) { return n > 20; };

    std::cout << "\nImperative (Nested conditions): ";
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
        std::cout << n << " ";
    }

    std::cout << "\nFunctional (Composed pipeline): ";
    auto result_functional = numbers
                             | std::views::filter(is_even)        // Reusable predicate
                             | std::views::transform(square)      // Reusable transformation
                             | std::views::filter(is_large);      // Reusable predicate

    for (const auto& n : result_functional) {
        std::cout << n << " ";
    }

    std::cout << "\n\n✅ Functional allows reusable, composable operations\n";
}

// ============================================================================
// MAIN - Run all examples
// ============================================================================

int main() {
    std::cout << "\n╔═══════════════════════════════════════════════════════╗\n";
    std::cout << "║   IMPERATIVE vs FUNCTIONAL PROGRAMMING IN C++       ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════╝\n";

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

    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "KEY TAKEAWAYS:\n";
    std::cout << std::string(60, '=') << "\n";
    std::cout << "✅ Functional style is more DECLARATIVE (WHAT not HOW)\n";
    std::cout << "✅ Pipelines are COMPOSABLE and REUSABLE\n";
    std::cout << "✅ Ranges provide LAZY EVALUATION (better performance)\n";
    std::cout << "✅ No MUTABLE STATE (fewer bugs)\n";
    std::cout << "✅ Code is more READABLE and MAINTAINABLE\n";
    std::cout << "✅ Better for PARALLEL processing (no shared state)\n";
    std::cout << "\n💡 think-cell Philosophy:\n";
    std::cout << "   'Prefer functional-style generic algorithms and ranges\n";
    std::cout << "    over imperative programming.'\n";
    std::cout << std::string(60, '=') << "\n\n";

    return 0;
}

/*
 * COMPILE AND RUN:
 * ===============
 * g++ -std=c++20 -Wall -Wextra imperative_vs_functional.cpp -o imperative_vs_functional
 * ./imperative_vs_functional
 *
 * EXPECTED OUTPUT:
 * ===============
 * Clear side-by-side comparisons showing:
 * - How imperative focuses on steps (HOW)
 * - How functional focuses on transformations (WHAT)
 * - Benefits of functional style
 *
 * KEY LEARNING POINTS:
 * ===================
 * 1. Imperative = step-by-step instructions, mutable state
 * 2. Functional = declarative transformations, immutable data
 * 3. Ranges = lazy evaluation + composition
 * 4. Pipelines = readable data flow
 * 5. think-cell heavily uses this style!
 */
