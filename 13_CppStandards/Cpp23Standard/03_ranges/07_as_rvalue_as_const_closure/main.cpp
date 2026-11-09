/*
 * views::as_rvalue, views::as_const, const iterators, range_adaptor_closure
 * (C++23, <ranges>, <iterator>)
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <algorithm>
#include <iterator>
#include <print>
#include <ranges>
#include <string>
#include <type_traits>
#include <vector>

// A user-defined range adaptor: deriving from range_adaptor_closure
// makes it usable with the pipe operator.
struct KeepEven : std::ranges::range_adaptor_closure<KeepEven> {
    template <std::ranges::viewable_range R>
    auto operator()(R&& range) const {
        return std::forward<R>(range) | std::views::filter([](int x) { return x % 2 == 0; });
    }
};
inline constexpr KeepEven keepEven;

int main() {
    // === EXAMPLE 1: as_rvalue ===
    std::println("=== Example 1: as_rvalue ===");
    std::vector<std::string> source = {"alpha", "beta", "gamma"};
    // every element is moved, not copied
    std::vector<std::string> target(source.size());
    std::ranges::copy(source | std::views::as_rvalue, target.begin());
    std::println("target: {}", target);
    std::println("source elements are empty after the move: {}", source[0].empty());

    // === EXAMPLE 2: as_const ===
    std::println("");
    std::println("=== Example 2: as_const ===");
    std::vector<int> numbers = {1, 2, 3};
    auto readOnly = numbers | std::views::as_const;
    using Ref = std::ranges::range_reference_t<decltype(readOnly)>;
    std::println("elements are read only: {}", std::is_const_v<std::remove_reference_t<Ref>>);
    // *readOnly.begin() = 5;   // error: assignment of read-only location

    // std::ranges::cbegin now gives a really constant iterator
    auto it = std::ranges::cbegin(numbers);
    static_assert(std::ranges::constant_range<const std::vector<int>>);
    static_assert(std::input_iterator<decltype(it)>);
    std::println("first element through a const iterator: {}", *it);

    // === EXAMPLE 3: range_adaptor_closure ===
    std::println("");
    std::println("=== Example 3: custom adaptor with range_adaptor_closure ===");
    std::vector<int> values = {1, 2, 3, 4, 5, 6, 7, 8};
    std::println("{}", values | keepEven | std::views::transform([](int x) { return x * 10; })
                           | std::ranges::to<std::vector>());
    std::println("called like a function: {}", keepEven(values) | std::ranges::to<std::vector>());
    return 0;
}
