/*
 * Ranges library: views and pipelines (<ranges>, C++20)
 *
 * A view is a lazy, non-owning description of a sequence. Views are combined
 * with the pipe operator; nothing is computed until the result is iterated.
 *
 * Compile: g++ -std=c++20 -Wall -Wextra main.cpp -o example
 */

#include <iostream>
#include <map>
#include <ranges>
#include <string>
#include <vector>

template <std::ranges::range R>
void print(const char* label, R&& range) {
    std::cout << label << ":";
    for (const auto& x : range) {
        std::cout << " " << x;
    }
    std::cout << "\n";
}

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // === EXAMPLE 1: filter, transform, take, drop ===
    std::cout << "=== Example 1: filter, transform, take, drop ===\n";
    auto evenSquares = numbers
                       | std::views::filter([](int x) { return x % 2 == 0; })
                       | std::views::transform([](int x) { return x * x; });
    print("even squares", evenSquares);
    print("first three", numbers | std::views::take(3));
    print("without first seven", numbers | std::views::drop(7));
    print("while < 4", numbers | std::views::take_while([](int x) { return x < 4; }));
    print("reversed", numbers | std::views::reverse | std::views::take(3));

    // === EXAMPLE 2: iota and lazy evaluation ===
    std::cout << "\n=== Example 2: iota (an infinite range is fine) ===\n";
    auto firstSquares = std::views::iota(1) | std::views::transform([](int x) { return x * x; }) | std::views::take(5);
    print("squares", firstSquares);

    // === EXAMPLE 3: split, keys, values, elements ===
    std::cout << "\n=== Example 3: split, keys, values ===\n";
    std::string csv = "red,green,blue";
    for (auto part : csv | std::views::split(',')) {
        std::cout << "[" << std::string(part.begin(), part.end()) << "]";
    }
    std::cout << "\n";
    std::map<std::string, int> stock = {{"apple", 3}, {"pear", 5}};
    print("keys", stock | std::views::keys);
    print("values", stock | std::views::values);

    // === EXAMPLE 4: a view sees later changes ===
    std::cout << "\n=== Example 4: views are lazy ===\n";
    std::vector<int> v = {1, 2, 3};
    auto doubled = v | std::views::transform([](int x) { return x * 2; });
    v.push_back(4);   // the view is evaluated when it is iterated, so it includes 4
    print("doubled", doubled);
    return 0;
}
