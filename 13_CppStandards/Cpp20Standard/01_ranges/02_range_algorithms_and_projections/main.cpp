/*
 * Range algorithms with projections: std::ranges::sort, find, min_element,
 * count_if, copy, ... (<algorithm>, C++20), plus std::ranges::subrange and
 * std::ranges::dangling
 *
 * Compile: g++ -std=c++20 -Wall -Wextra main.cpp -o example
 */

#include <algorithm>
#include <iostream>
#include <iterator>
#include <ranges>
#include <string>
#include <type_traits>
#include <vector>

struct Person {
    std::string name;
    int age;
};

std::vector<Person> makePeople() { return {{"Zoe", 31}, {"Adam", 45}, {"Mia", 27}}; }

int main() {
    // === EXAMPLE 1: algorithms take the whole container ===
    std::cout << "=== Example 1: no begin()/end() pairs ===\n";
    std::vector<int> numbers = {5, 2, 9, 1, 7};
    std::ranges::sort(numbers);
    for (int x : numbers) {
        std::cout << x << " ";
    }
    std::cout << "\n";
    std::cout << "contains 9: " << (std::ranges::find(numbers, 9) != numbers.end()) << "\n";

    // === EXAMPLE 2: projections ===
    std::cout << "\n=== Example 2: projections ===\n";
    auto people = makePeople();
    std::ranges::sort(people, {}, &Person::age);        // sort by age: the projection selects the key
    for (const auto& p : people) {
        std::cout << p.name << " (" << p.age << ")  ";
    }
    std::cout << "\n";
    std::ranges::sort(people, std::ranges::greater{}, &Person::name);   // by name, descending
    for (const auto& p : people) {
        std::cout << p.name << "  ";
    }
    std::cout << "\n";
    auto oldest = std::ranges::max_element(people, {}, &Person::age);
    std::cout << "oldest: " << oldest->name << "\n";

    // === EXAMPLE 3: subrange and dangling ===
    std::cout << "\n=== Example 3: subrange, dangling ===\n";
    auto sub = std::ranges::subrange(numbers.begin() + 1, numbers.end() - 1);
    for (int x : sub) {
        std::cout << x << " ";
    }
    std::cout << "\n";
    // Calling find on a temporary container would return a dangling iterator,
    // so the library returns std::ranges::dangling instead of an iterator:
    auto bad = std::ranges::max_element(std::vector<int>{3, 1, 2});
    std::cout << "result of an algorithm on a temporary is dangling: "
              << std::is_same_v<decltype(bad), std::ranges::dangling> << "\n";

    // === EXAMPLE 4: results with structured information ===
    std::cout << "\n=== Example 4: ranges::copy returns in and out ===\n";
    std::vector<int> out;
    auto [in, outIt] = std::ranges::copy(numbers, std::back_inserter(out));
    std::cout << "copied " << out.size() << " elements, input fully consumed: " << (in == numbers.end()) << "\n";
    (void)outIt;
    return 0;
}
