/*
 * pair / tuple construction from tuple-like objects, constexpr std::unique_ptr
 * and constexpr std::bitset (C++23)
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <array>
#include <bitset>
#include <memory>
#include <print>
#include <string>
#include <tuple>
#include <utility>

constexpr int sumWithUniquePtr() {
    auto p = std::make_unique<int>(40);   // allocation and release inside a constant expression
    *p += 2;
    return *p;
}

constexpr std::size_t countBits() {
    std::bitset<16> bits(0b1011'0110);
    bits.set(0);
    return bits.count();
}

int main() {
    // === EXAMPLE 1: pair and tuple from tuple-like types ===
    std::println("=== Example 1: tuple-like conversions ===");
    std::array<int, 2> array = {3, 4};
    std::pair<int, int> fromArray(array);            // std::array is tuple-like
    std::println("pair from array: {}", fromArray);

    std::tuple<int, std::string> tuple(7, "seven");
    std::pair<int, std::string> fromTuple(tuple);    // a 2-element tuple converts to a pair
    std::println("pair from tuple: {}", fromTuple);

    std::pair<int, int> point{1, 2};
    std::tuple<int, int> asTuple(point);
    std::println("tuple from pair: {}", asTuple);

    // === EXAMPLE 2: constexpr unique_ptr and bitset ===
    std::println("");
    std::println("=== Example 2: constexpr unique_ptr and bitset ===");
    constexpr int result = sumWithUniquePtr();       // evaluated by the compiler
    static_assert(result == 42);
    constexpr std::size_t bits = countBits();
    static_assert(bits == 6);
    std::println("compile time: unique_ptr gave {}, bitset count {}", result, bits);
    return 0;
}
