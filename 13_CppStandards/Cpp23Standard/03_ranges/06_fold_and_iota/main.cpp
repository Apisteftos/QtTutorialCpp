/*
 * ranges::fold_left, fold_left_first, fold_right, fold_right_last,
 * fold_left_with_iter, fold_left_first_with_iter, ranges::iota (C++23)
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <algorithm>
#include <functional>
#include <numeric>
#include <print>
#include <string>
#include <vector>

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5};

    // === EXAMPLE 1: fold_left / fold_right ===
    std::println("=== Example 1: fold_left and fold_right ===");
    int sum = std::ranges::fold_left(numbers, 0, std::plus<>{});
    std::println("sum:     {}", sum);

    // the order matters for operations that are not commutative
    std::string left = std::ranges::fold_left(numbers, std::string(), [](std::string acc, int x) {
        return "(" + acc + " " + std::to_string(x) + ")";
    });
    std::string right = std::ranges::fold_right(numbers, std::string(), [](int x, std::string acc) {
        return "(" + std::to_string(x) + " " + acc + ")";
    });
    std::println("left:    {}", left);
    std::println("right:   {}", right);

    // === EXAMPLE 2: the "first" variants (no start value) ===
    std::println("");
    std::println("=== Example 2: fold_left_first and fold_right_last ===");
    // the result is std::optional, because an empty range has no first element
    auto biggest = std::ranges::fold_left_first(numbers, [](int a, int b) { return a > b ? a : b; });
    std::println("maximum: {}", *biggest);

    std::vector<int> empty;
    auto none = std::ranges::fold_left_first(empty, std::plus<>{});
    std::println("empty range has a value: {}", none.has_value());

    auto lastResult = std::ranges::fold_right_last(numbers, std::minus<>{});
    std::println("fold_right_last with minus: {}", *lastResult);  // 1 - (2 - (3 - (4 - 5)))

    // === EXAMPLE 3: the with_iter variants ===
    std::println("");
    std::println("=== Example 3: fold_left_with_iter ===");
    // returns the end iterator and the value, useful with input ranges
    auto [end, total] = std::ranges::fold_left_with_iter(numbers, 100, std::plus<>{});
    std::println("total {} and iterator at end: {}", total, end == numbers.end());

    // === EXAMPLE 4: ranges::iota ===
    std::println("");
    std::println("=== Example 4: ranges::iota ===");
    std::vector<int> filled(6);
    auto result = std::ranges::iota(filled, 10);  // 10, 11, 12, ...
    std::println("filled: {}, next value would be {}", filled, result.value);
    return 0;
}
