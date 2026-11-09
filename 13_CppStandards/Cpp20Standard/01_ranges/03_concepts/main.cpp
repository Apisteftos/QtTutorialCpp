/*
 * Concepts and requires (<concepts>, C++20)
 *
 * A concept is a named compile-time condition on template arguments. It gives
 * short, readable error messages and allows overloading on properties of a type.
 *
 * Compile: g++ -std=c++20 -Wall -Wextra main.cpp -o example
 */

#include <concepts>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

// === standard concepts ===
template <std::integral T>
T half(T value) {
    return value / 2;
}

template <std::floating_point T>
T half(T value) {
    return value * T(0.5);
}

// === own concept with a requires expression ===
template <typename T>
concept Printable = requires(const T& value, std::ostream& os) {
    { os << value } -> std::same_as<std::ostream&>;
};

template <typename T>
concept HasSize = requires(const T& c) {
    { c.size() } -> std::convertible_to<std::size_t>;
};

template <Printable T>
void show(const T& value) {
    std::cout << "value: " << value << "\n";
}

// Abbreviated function template: `auto` with a concept
void describe(const HasSize auto& container) {
    std::cout << "size: " << container.size() << "\n";
}

// requires clause after the signature
template <typename T>
    requires std::is_arithmetic_v<T> && (sizeof(T) >= 4)
T doubleIt(T x) {
    return x * 2;
}

struct Plain {};   // not printable, no size()

int main() {
    // === EXAMPLE 1: overloading by concept ===
    std::cout << "=== Example 1: integral vs floating_point ===\n";
    std::cout << "half(9)   = " << half(9) << "\n";
    std::cout << "half(9.0) = " << half(9.0) << "\n";

    // === EXAMPLE 2: own concepts ===
    std::cout << "\n=== Example 2: own concepts ===\n";
    show(42);
    show(std::string("text"));
    // show(Plain{});   // error: constraints not satisfied (Printable<Plain>)
    describe(std::vector<int>{1, 2, 3});
    describe(std::string("four"));
    std::cout << "Printable<int>: " << Printable<int> << ", Printable<Plain>: " << Printable<Plain> << "\n";
    std::cout << "HasSize<std::vector<int>>: " << HasSize<std::vector<int>> << ", HasSize<int>: " << HasSize<int> << "\n";

    // === EXAMPLE 3: requires clause ===
    std::cout << "\n=== Example 3: requires clause ===\n";
    std::cout << "doubleIt(21) = " << doubleIt(21) << "\n";
    // doubleIt(short(1));   // error: sizeof(short) < 4
    static_assert(std::derived_from<std::runtime_error, std::exception>);
    static_assert(std::invocable<decltype([](int) {}), int>);
    std::cout << "static_asserts with std::derived_from and std::invocable passed\n";
    return 0;
}
