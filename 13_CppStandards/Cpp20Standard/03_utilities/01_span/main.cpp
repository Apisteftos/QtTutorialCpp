/*
 * std::span (<span>, C++20)
 *
 * A non-owning view of a contiguous sequence (array, vector, C array).
 * One function signature accepts all of them.
 *
 * Compile: g++ -std=c++20 -Wall -Wextra main.cpp -o example
 */

#include <array>
#include <iostream>
#include <span>
#include <vector>

int sum(std::span<const int> values) {
    int total = 0;
    for (int v : values) {
        total += v;
    }
    return total;
}

void fill(std::span<int> values, int value) {
    for (int& v : values) {
        v = value;
    }
}

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 6};
    std::array<int, 3> arr = {10, 20, 30};
    int raw[4] = {7, 7, 7, 7};

    // === EXAMPLE 1: one function for every contiguous container ===
    std::cout << "=== Example 1: span as parameter ===\n";
    std::cout << "vector: " << sum(vec) << ", array: " << sum(arr) << ", C array: " << sum(raw) << "\n";

    // === EXAMPLE 2: sub-spans ===
    std::cout << "\n=== Example 2: first, last, subspan ===\n";
    std::span<int> all(vec);
    std::cout << "first(2): " << sum(all.first(2)) << ", last(2): " << sum(all.last(2))
              << ", subspan(1, 3): " << sum(all.subspan(1, 3)) << "\n";
    fill(all.subspan(2, 2), 0);    // writes through the span into the vector
    for (int v : vec) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    // === EXAMPLE 3: fixed extent and bytes ===
    std::cout << "\n=== Example 3: static extent, as_bytes ===\n";
    std::span<int, 3> fixed(arr);
    std::cout << "size known at compile time: " << fixed.extent << "\n";
    auto bytes = std::as_bytes(std::span<const int>(arr));
    std::cout << "bytes: " << bytes.size() << ", size_bytes(): " << fixed.size_bytes() << "\n";
    // Warning: a span does not own the elements. Do not use it after the vector
    // reallocates or is destroyed.
    return 0;
}
