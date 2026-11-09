/*
 * constexpr <cmath> / <cstdlib> functions (C++23)
 *
 * Basic functions such as floor, ceil, round, trunc, fabs, fmod, copysign,
 * fmin, fmax and abs can be evaluated at compile time.
 * Functions like sqrt, sin, exp, pow become constexpr only in C++26.
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <array>
#include <cmath>
#include <cstdlib>
#include <print>

int main() {
    // === EXAMPLE 1: rounding at compile time ===
    std::println("=== Example 1: rounding ===");
    constexpr double x = 7.65;
    static_assert(std::floor(x) == 7.0);
    static_assert(std::ceil(x) == 8.0);
    static_assert(std::round(x) == 8.0);
    static_assert(std::trunc(-x) == -7.0);
    std::println("floor {}, ceil {}, round {}, trunc(-x) {}", std::floor(x), std::ceil(x), std::round(x),
                 std::trunc(-x));

    // === EXAMPLE 2: sign and remainder ===
    std::println("");
    std::println("=== Example 2: fabs, fmod, copysign, fmin, fmax ===");
    static_assert(std::fabs(-2.5) == 2.5);
    static_assert(std::fmod(10.0, 3.0) == 1.0);
    static_assert(std::copysign(3.0, -1.0) == -3.0);
    static_assert(std::fmin(2.0, 5.0) == 2.0);
    static_assert(std::fmax(2.0, 5.0) == 5.0);
    std::println("fabs(-2.5)={} fmod(10,3)={} copysign(3,-1)={}", std::fabs(-2.5), std::fmod(10.0, 3.0),
                 std::copysign(3.0, -1.0));

    // === EXAMPLE 3: integer helpers ===
    std::println("");
    std::println("=== Example 3: abs for integers ===");
    static_assert(std::abs(-42) == 42);
    static_assert(std::abs(-42) == 42);
    static_assert(std::abs(-3.5) == 3.5);
    std::println("abs(-42) = {}, abs(-3.5) = {}", std::abs(-42), std::abs(-3.5));

    // === EXAMPLE 4: a compile-time table ===
    std::println("");
    std::println("=== Example 4: table built at compile time ===");
    constexpr auto table = [] {
        std::array<double, 5> t{};
        for (std::size_t i = 0; i < t.size(); ++i) {
            t[i] = std::floor(i * 1.5);
        }
        return t;
    }();
    std::println("{}", table);
    return 0;
}
