/*
 * constexpr std::vector, std::string and algorithms (C++20)
 *
 * Containers can be used inside constant expressions as long as the memory is
 * released before the evaluation ends (nothing may escape to run time).
 *
 * Compile: g++ -std=c++20 -Wall -Wextra main.cpp -o example
 */

#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

constexpr int sumOfSquares(int n) {
    std::vector<int> values(static_cast<std::size_t>(n));
    std::iota(values.begin(), values.end(), 1);
    for (int& v : values) {
        v *= v;
    }
    return std::accumulate(values.begin(), values.end(), 0);   // the vector is destroyed at the end
}

constexpr std::size_t countWords(std::string text) {
    std::size_t words = 0;
    bool inWord = false;
    for (char c : text) {
        if (c == ' ') {
            inWord = false;
        } else if (!inWord) {
            inWord = true;
            ++words;
        }
    }
    return words;
}

constexpr auto sortedPrimes() {
    std::array<int, 5> primes = {11, 3, 7, 2, 5};
    std::sort(primes.begin(), primes.end());       // std::sort is constexpr in C++20
    return primes;
}

int main() {
    // === EXAMPLE 1: vector in a constant expression ===
    std::cout << "=== Example 1: constexpr vector ===\n";
    constexpr int result = sumOfSquares(10);
    static_assert(result == 385);
    std::cout << "sum of squares 1..10 computed at compile time: " << result << "\n";

    // === EXAMPLE 2: string ===
    std::cout << "\n=== Example 2: constexpr string ===\n";
    constexpr std::size_t words = countWords("the quick brown fox");
    static_assert(words == 4);
    std::cout << "words: " << words << "\n";

    // === EXAMPLE 3: algorithms ===
    std::cout << "\n=== Example 3: constexpr sort ===\n";
    constexpr auto primes = sortedPrimes();
    static_assert(primes.front() == 2 && primes.back() == 11);
    for (int p : primes) {
        std::cout << p << " ";
    }
    std::cout << "\n";
    return 0;
}
