/*
 * std::sample, std::for_each_n (<algorithm>), searchers (<functional>)
 * (C++17)
 *
 * Compile: g++ -std=c++17 -Wall -Wextra main.cpp -o example
 */

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <string>
#include <vector>

int main() {
    // === EXAMPLE 1: sample ===
    std::cout << "=== Example 1: sample ===\n";
    std::vector<int> population(100);
    std::iota(population.begin(), population.end(), 1);
    std::vector<int> picked;
    std::mt19937 generator(12345);   // fixed seed: reproducible
    std::sample(population.begin(), population.end(), std::back_inserter(picked), 5, generator);
    std::cout << "picked " << picked.size() << " values, all within 1..100: "
              << std::all_of(picked.begin(), picked.end(), [](int x) { return x >= 1 && x <= 100; }) << "\n";
    std::cout << "order is kept (sorted): " << std::is_sorted(picked.begin(), picked.end()) << "\n";

    // === EXAMPLE 2: for_each_n ===
    std::cout << "\n=== Example 2: for_each_n ===\n";
    std::vector<int> v = {1, 2, 3, 4, 5};
    std::for_each_n(v.begin(), 3, [](int& x) { x *= 10; });   // only the first three
    for (int x : v) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    // === EXAMPLE 3: searchers ===
    std::cout << "\n=== Example 3: boyer_moore_searcher ===\n";
    std::string text = "the quick brown fox jumps over the lazy dog";
    std::string pattern = "lazy";
    auto it = std::search(text.begin(), text.end(), std::boyer_moore_searcher(pattern.begin(), pattern.end()));
    std::cout << "found at index " << (it - text.begin()) << "\n";
    auto it2 = std::search(text.begin(), text.end(), std::default_searcher(pattern.begin(), pattern.end()));
    std::cout << "default_searcher agrees: " << (it == it2) << "\n";
    return 0;
}
