/*
 * std::ranges::to, std::generator, std::ranges::elements_of (C++23)
 *
 * ranges::to  (<ranges>)    : turn any range into a container
 * generator   (<generator>) : a coroutine that produces a lazy sequence
 * elements_of (<ranges>)    : yield all elements of another range from a generator
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <generator>
#include <list>
#include <print>
#include <ranges>
#include <set>
#include <string>
#include <vector>

// A generator is lazy: the next value is computed only when it is asked for.
std::generator<long long> fibonacci() {
    long long a = 0;
    long long b = 1;
    while (true) {
        co_yield a;
        auto next = a + b;
        a = b;
        b = next;
    }
}

struct Node {
    int value;
    std::vector<Node> children;
};

// elements_of passes through every element of the recursive call.
std::generator<int> walk(const Node& node) {
    co_yield node.value;
    for (const auto& child : node.children) {
        co_yield std::ranges::elements_of(walk(child));
    }
}

int main() {
    // === EXAMPLE 1: ranges::to ===
    std::println("=== Example 1: ranges::to ===");
    auto squares = std::views::iota(1, 6)
                   | std::views::transform([](int x) { return x * x; })
                   | std::ranges::to<std::vector>();  // element type is deduced
    std::println("vector: {}", squares);

    auto asSet = squares | std::ranges::to<std::set>();
    auto asList = squares | std::ranges::to<std::list<int>>();
    std::println("set:    {}", asSet);
    std::println("list:   {}", asList);

    std::string word = "generator";
    auto reversed = word | std::views::reverse | std::ranges::to<std::string>();
    std::println("reversed: {}", reversed);

    // === EXAMPLE 2: generator ===
    std::println("");
    std::println("=== Example 2: generator ===");
    auto firstTen = fibonacci() | std::views::take(10) | std::ranges::to<std::vector>();
    std::println("first ten Fibonacci numbers: {}", firstTen);

    // === EXAMPLE 3: elements_of with a recursive generator ===
    std::println("");
    std::println("=== Example 3: elements_of ===");
    Node tree{1, {Node{2, {Node{4, {}}}}, Node{3, {}}}};
    std::println("depth-first walk: {}", walk(tree) | std::ranges::to<std::vector>());
    return 0;
}
