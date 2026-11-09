/*
 * views::adjacent, adjacent_transform, pairwise, chunk, slide, chunk_by, stride
 * (C++23, <ranges>)
 *
 * Different ways to look at neighbouring elements of a range.
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <print>
#include <ranges>
#include <vector>

int main() {
    std::vector<int> temperatures = {12, 14, 13, 17, 20, 19, 22, 25};
    std::println("data: {}", temperatures);

    // === EXAMPLE 1: adjacent / pairwise ===
    std::println("");
    std::println("=== Example 1: adjacent, pairwise, adjacent_transform ===");
    // pairwise = adjacent<2>: every element together with its successor
    for (auto [a, b] : temperatures | std::views::pairwise) {
        std::print("({} -> {}) ", a, b);
    }
    std::println("");
    // adjacent<3>: groups of three neighbours, as a tuple of references
    for (auto [a, b, c] : temperatures | std::views::adjacent<3>) {
        std::print("[{} {} {}] ", a, b, c);
    }
    std::println("");
    // adjacent_transform: combine every group with a function (here: the change)
    auto changes = temperatures
                   | std::views::pairwise_transform([](int a, int b) { return b - a; })
                   | std::ranges::to<std::vector>();
    std::println("changes: {}", changes);

    // === EXAMPLE 2: chunk and slide ===
    std::println("");
    std::println("=== Example 2: chunk and slide ===");
    // chunk(3): separate groups, the last one may be shorter
    for (auto group : temperatures | std::views::chunk(3)) {
        std::println("chunk: {}", group | std::ranges::to<std::vector>());
    }
    // slide(3): overlapping windows of 3
    for (auto window : temperatures | std::views::slide(3)) {
        std::println("window: {}", window | std::ranges::to<std::vector>());
    }

    // === EXAMPLE 3: chunk_by ===
    std::println("");
    std::println("=== Example 3: chunk_by ===");
    // start a new group whenever the predicate is false for two neighbours
    for (auto run : temperatures | std::views::chunk_by([](int a, int b) { return a <= b; })) {
        std::println("rising run: {}", run | std::ranges::to<std::vector>());
    }

    // === EXAMPLE 4: stride ===
    std::println("");
    std::println("=== Example 4: stride ===");
    std::println("every 3rd element: {}", temperatures | std::views::stride(3) | std::ranges::to<std::vector>());
    return 0;
}
