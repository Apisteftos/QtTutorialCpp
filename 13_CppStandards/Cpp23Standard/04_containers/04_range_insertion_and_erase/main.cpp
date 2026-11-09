/*
 * from_range, insert_range, append_range, assign_range, prepend_range and
 * std::erase / std::erase_if (C++23)
 *
 * Container members that accept a whole range (not only an iterator pair).
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <print>
#include <ranges>
#include <set>
#include <string>
#include <vector>
#include <version>

int main() {
    // === EXAMPLE 1: constructing from a range ===
    std::println("=== Example 1: std::from_range ===");
    auto squares = std::views::iota(1, 6) | std::views::transform([](int x) { return x * x; });
    std::vector<int> v(std::from_range, squares);
    std::set<int> s(std::from_range, squares);
    std::println("vector: {}", v);
    std::println("set:    {}", s);

    // === EXAMPLE 2: insert_range, append_range, assign_range ===
    std::println("");
    std::println("=== Example 2: insert_range, append_range, assign_range ===");
    std::vector<int> numbers = {1, 2, 3};
    numbers.append_range(std::views::iota(10, 13));              // at the end
    numbers.insert_range(numbers.begin() + 1, std::vector{7, 8}); // at a position
    std::println("numbers: {}", numbers);
    numbers.assign_range(std::views::iota(0, 4));                // replace everything
    std::println("assigned:  {}", numbers);

    std::string text = "world";
    text.insert_range(text.begin(), std::string_view("hello "));
    std::println("string: {}", text);

    // === EXAMPLE 3: prepend_range ===
    std::println("");
    std::println("=== Example 3: prepend_range ===");
    std::list<int> list = {4, 5};
    list.prepend_range(std::vector{1, 2, 3});
    std::println("list: {}", list);

    std::deque<int> deque = {4, 5};
    deque.prepend_range(std::vector{1, 2, 3});
    std::println("deque: {}", deque);

    std::forward_list<int> forward = {4, 5};
    forward.prepend_range(std::vector{1, 2, 3});
    std::println("forward_list: {}", forward);

    // === EXAMPLE 4: erase and erase_if for associative containers ===
    std::println("");
    std::println("=== Example 4: erase_if ===");
    std::map<int, std::string> table = {{1, "one"}, {2, "two"}, {3, "three"}, {4, "four"}};
    auto removed = std::erase_if(table, [](const auto& item) { return item.first % 2 == 0; });
    std::println("removed {} items: {}", removed, table);

    // C++23: erase with a key of another type, without building a temporary
    std::set<std::string, std::less<>> names = {"anna", "bob", "carl"};
    std::string_view key = "bob";
#ifdef __cpp_lib_associative_heterogeneous_erasure
    names.erase(key);   // heterogeneous erase
#else
    names.erase(std::string(key));   // GCC 15: not available yet, a temporary string is needed
#endif
    std::println("names: {}", names);
    return 0;
}
