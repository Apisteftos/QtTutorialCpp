/*
 * std::flat_map, std::flat_set (C++23, <flat_map>, <flat_set>)
 *
 * Sorted associative containers stored in contiguous memory (sorted vectors).
 * Fast to iterate and compact, slower to insert in the middle than std::map.
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <flat_map>
#include <flat_set>
#include <print>
#include <string>
#include <vector>

int main() {
    // === EXAMPLE 1: flat_map ===
    std::println("=== Example 1: flat_map ===");
    std::flat_map<std::string, int> stock;
    stock["pear"] = 4;
    stock["apple"] = 10;
    stock.emplace("cherry", 25);
    stock.insert({"banana", 7});

    for (const auto& [name, count] : stock) {
        std::println("{:<7} {}", name, count);
    }
    std::println("as a map: {}", stock);
    std::println("apple count: {}", stock.at("apple"));
    std::println("contains plum: {}", stock.contains("plum"));

    // === EXAMPLE 2: the data is stored in two sorted vectors ===
    std::println("");
    std::println("=== Example 2: keys and values are separate containers ===");
    const auto& keys = stock.keys();
    const auto& values = stock.values();
    std::println("keys:   {}", keys);
    std::println("values: {}", values);

    // === EXAMPLE 3: flat_set ===
    std::println("");
    std::println("=== Example 3: flat_set ===");
    std::flat_set<int> ids = {5, 1, 4, 1, 3};
    ids.insert(2);
    std::println("set: {}", ids);
    std::println("size {}, contains 4: {}", ids.size(), ids.contains(4));
    ids.erase(4);
    std::println("after erase(4): {}", ids);

    // === EXAMPLE 4: replacing the underlying container ===
    std::println("");
    std::println("=== Example 4: sorted_unique build ===");
    // when the data is already sorted and unique, skip the sorting
    std::vector<int> sortedData = {10, 20, 30};
    std::flat_set<int> fast(std::sorted_unique, sortedData);
    std::println("{}", fast);
    return 0;
}
