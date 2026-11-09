/*
 * Small additions: std::to_array, std::remove_cvref, map::contains,
 * std::erase / std::erase_if, string starts_with / ends_with,
 * std::make_shared for arrays, std::ranges-free helpers (C++20)
 *
 * Compile: g++ -std=c++20 -Wall -Wextra main.cpp -o example
 */

#include <array>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <type_traits>
#include <vector>

int main() {
    // === EXAMPLE 1: to_array, contains ===
    std::cout << "=== Example 1: to_array, contains ===\n";
    auto arr = std::to_array({1, 2, 3});               // size deduced: std::array<int, 3>
    auto chars = std::to_array("abc");                 // copies the characters, including '\0'
    std::cout << "array size " << arr.size() << ", char array size " << chars.size() << "\n";
    std::map<std::string, int> ages = {{"Ann", 31}};
    std::set<int> ids = {1, 2, 3};
    std::cout << std::boolalpha;
    std::cout << "map contains Ann: " << ages.contains("Ann") << ", set contains 5: " << ids.contains(5) << "\n";

    // === EXAMPLE 2: erase / erase_if ===
    std::cout << "\n=== Example 2: erase, erase_if ===\n";
    std::vector<int> v = {1, 2, 3, 2, 4, 2};
    auto removedTwos = std::erase(v, 2);               // erase-remove idiom in one call
    std::cout << "removed " << removedTwos << " twos, left " << v.size() << "\n";
    auto removedBig = std::erase_if(v, [](int x) { return x > 2; });
    std::cout << "removed " << removedBig << " values greater than 2, left " << v.size() << "\n";

    // === EXAMPLE 3: string prefix / suffix ===
    std::cout << "\n=== Example 3: starts_with, ends_with ===\n";
    std::string file = "report_2024.csv";
    std::cout << "starts with report: " << file.starts_with("report") << ", ends with .csv: " << file.ends_with(".csv")
              << ", starts with 'x': " << file.starts_with('x') << "\n";

    // === EXAMPLE 4: remove_cvref, make_shared for arrays ===
    std::cout << "\n=== Example 4: remove_cvref, make_shared<T[]> ===\n";
    static_assert(std::is_same_v<std::remove_cvref_t<const std::string&>, std::string>);
    std::cout << "remove_cvref_t<const std::string&> is std::string\n";
    auto numbers = std::make_shared<int[]>(4);         // value-initialized array of 4 ints
    numbers[2] = 7;
    std::cout << "shared array: " << numbers[0] << " " << numbers[2] << "\n";
    return 0;
}
