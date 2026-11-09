/*
 * std::reduce, std::transform_reduce, std::inclusive_scan,
 * std::exclusive_scan, std::transform_inclusive_scan (<numeric>, C++17)
 *
 * Compile: g++ -std=c++17 -Wall -Wextra main.cpp -o example
 */

#include <functional>
#include <iostream>
#include <numeric>
#include <vector>

void print(const char* label, const std::vector<int>& v) {
    std::cout << label << ":";
    for (int x : v) {
        std::cout << " " << x;
    }
    std::cout << "\n";
}

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};

    // === EXAMPLE 1: reduce and transform_reduce ===
    std::cout << "=== Example 1: reduce, transform_reduce ===\n";
    std::cout << "reduce:           " << std::reduce(v.begin(), v.end(), 0) << "\n";
    std::cout << "product:          " << std::reduce(v.begin(), v.end(), 1, std::multiplies<>{}) << "\n";
    // sum of squares: transform every element, then reduce
    std::cout << "sum of squares:   "
              << std::transform_reduce(v.begin(), v.end(), 0, std::plus<>{}, [](int x) { return x * x; }) << "\n";
    // dot product of two ranges
    std::vector<int> w = {5, 4, 3, 2, 1};
    std::cout << "dot product:      " << std::transform_reduce(v.begin(), v.end(), w.begin(), 0) << "\n";

    // === EXAMPLE 2: scans ===
    std::cout << "\n=== Example 2: inclusive_scan, exclusive_scan ===\n";
    std::vector<int> out(v.size());
    std::inclusive_scan(v.begin(), v.end(), out.begin());   // running sum, includes the current element
    print("inclusive_scan", out);
    std::exclusive_scan(v.begin(), v.end(), out.begin(), 0);   // running sum before the current element
    print("exclusive_scan", out);
    std::transform_inclusive_scan(v.begin(), v.end(), out.begin(), std::plus<>{}, [](int x) { return x * 10; });
    print("transform_inclusive_scan", out);

    // Note: unlike std::accumulate, reduce may add in any order, so the operation
    // should be associative (floating point sums can differ slightly).
    return 0;
}
