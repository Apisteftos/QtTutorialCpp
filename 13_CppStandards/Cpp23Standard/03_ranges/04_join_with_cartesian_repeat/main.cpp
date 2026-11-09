/*
 * views::join_with, views::cartesian_product, views::repeat (C++23, <ranges>)
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <print>
#include <ranges>
#include <string>
#include <vector>

int main() {
    // === EXAMPLE 1: join_with ===
    std::println("=== Example 1: join_with ===");
    std::vector<std::string> words = {"red", "green", "blue"};
    // join_with puts a separator between the elements (join has none)
    auto sentence = words | std::views::join_with(std::string(", ")) | std::ranges::to<std::string>();
    std::println("{}", sentence);

    std::vector<std::vector<int>> rows = {{1, 2}, {3}, {4, 5, 6}};
    auto flat = rows | std::views::join_with(0) | std::ranges::to<std::vector>();
    std::println("rows joined with 0: {}", flat);

    // === EXAMPLE 2: cartesian_product ===
    std::println("");
    std::println("=== Example 2: cartesian_product ===");
    std::vector<std::string> sizes = {"S", "M"};
    std::vector<std::string> colours = {"black", "white", "grey"};
    for (auto [size, colour] : std::views::cartesian_product(sizes, colours)) {
        std::print("{}-{} ", size, colour);
    }
    std::println("");

    // === EXAMPLE 3: repeat ===
    std::println("");
    std::println("=== Example 3: repeat ===");
    auto line = std::views::repeat('-', 20) | std::ranges::to<std::string>();  // bounded: 20 times
    std::println("{}", line);
    auto zeros = std::views::repeat(0) | std::views::take(5);                  // unbounded + take
    std::println("{}", zeros | std::ranges::to<std::vector>());
    return 0;
}
