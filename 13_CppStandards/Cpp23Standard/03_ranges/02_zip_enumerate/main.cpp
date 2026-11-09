/*
 * views::zip, views::zip_transform, views::enumerate (C++23, <ranges>)
 *
 * zip           : walk several ranges in lockstep
 * zip_transform : zip, then combine every group with a function
 * enumerate     : pair every element with its index
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <print>
#include <ranges>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> cities = {"Oslo", "Lima", "Kyoto"};
    std::vector<int> visitors = {120, 340, 275};
    std::vector<double> prices = {9.5, 7.0, 12.25};

    // === EXAMPLE 1: zip ===
    std::println("=== Example 1: zip ===");
    for (auto [city, count, price] : std::views::zip(cities, visitors, prices)) {
        std::println("{:<6} {:>4} visitors, price {}", city, count, price);
    }

    // zip gives references: changing the structured binding changes the original
    for (auto&& [count, price] : std::views::zip(visitors, prices)) {
        count += 10;
        price *= 2;
    }
    std::println("after the change: {} {}", visitors, prices);

    // === EXAMPLE 2: zip_transform ===
    std::println("");
    std::println("=== Example 2: zip_transform ===");
    auto revenue = std::views::zip_transform([](int count, double price) { return count * price; },
                                             visitors, prices);
    for (double value : revenue) {
        std::println("revenue {}", value);
    }

    // === EXAMPLE 3: enumerate ===
    std::println("");
    std::println("=== Example 3: enumerate ===");
    for (auto [index, city] : std::views::enumerate(cities)) {
        std::println("{}: {}", index, city);
    }

    // zip stops at the shortest range
    std::vector<int> shortList = {1, 2};
    for (auto [city, number] : std::views::zip(cities, shortList)) {
        std::println("{} -> {}", city, number);
    }
    return 0;
}
