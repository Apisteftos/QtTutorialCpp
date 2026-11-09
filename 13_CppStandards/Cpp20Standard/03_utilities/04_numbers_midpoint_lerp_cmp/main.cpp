/*
 * <numbers> constants, std::midpoint, std::lerp, std::ssize, integer
 * comparison functions std::cmp_less ..., std::in_range (C++20)
 *
 * Compile: g++ -std=c++20 -Wall -Wextra main.cpp -o example
 */

#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numbers>
#include <numeric>
#include <utility>
#include <vector>

int main() {
    std::cout << std::setprecision(8);

    // === EXAMPLE 1: mathematical constants ===
    std::cout << "=== Example 1: <numbers> ===\n";
    std::cout << "pi      = " << std::numbers::pi << "\n";
    std::cout << "e       = " << std::numbers::e << "\n";
    std::cout << "sqrt(2) = " << std::numbers::sqrt2 << "\n";
    std::cout << "ln(2)   = " << std::numbers::ln2 << "\n";
    std::cout << "pi (float) = " << std::numbers::pi_v<float> << "\n";

    // === EXAMPLE 2: midpoint and lerp ===
    std::cout << "\n=== Example 2: midpoint, lerp ===\n";
    int big = std::numeric_limits<int>::max();
    std::cout << "midpoint(max, max-2) = " << std::midpoint(big, big - 2) << "   (no overflow)\n";
    std::cout << "midpoint(1, 4) = " << std::midpoint(1, 4) << " (rounds toward the first)\n";
    std::cout << "lerp(10, 20, 0.25) = " << std::lerp(10.0, 20.0, 0.25) << "\n";
    std::cout << "lerp(10, 20, 1.5)  = " << std::lerp(10.0, 20.0, 1.5) << "  (extrapolates)\n";

    // === EXAMPLE 3: safe integer comparison ===
    std::cout << "\n=== Example 3: cmp_less, in_range ===\n";
    int negative = -1;
    unsigned int positive = 1;
    std::cout << std::boolalpha;
    std::cout << "-1 < 1u with the operator:     " << (negative < static_cast<int>(positive)) << " (after a cast)\n";
    std::cout << "plain comparison -1 < 1u:      " << (static_cast<unsigned>(negative) < positive) << " (wrong: -1 became a huge unsigned)\n";
    std::cout << "std::cmp_less(-1, 1u):         " << std::cmp_less(negative, positive) << "\n";
    std::cout << "in_range<std::uint8_t>(300):   " << std::in_range<std::uint8_t>(300) << "\n";
    std::cout << "in_range<std::uint8_t>(200):   " << std::in_range<std::uint8_t>(200) << "\n";

    // === EXAMPLE 4: ssize ===
    std::cout << "\n=== Example 4: ssize ===\n";
    std::vector<int> v = {1, 2, 3};
    for (auto i = std::ssize(v) - 1; i >= 0; --i) {   // signed: the loop ends correctly
        std::cout << v[static_cast<std::size_t>(i)] << " ";
    }
    std::cout << "\n";
    return 0;
}
