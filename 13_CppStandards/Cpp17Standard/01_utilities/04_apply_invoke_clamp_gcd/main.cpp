/*
 * std::apply, std::make_from_tuple, std::invoke, std::not_fn, std::clamp,
 * std::gcd / std::lcm, std::as_const, std::size / std::data / std::empty,
 * std::byte (C++17)
 *
 * Compile: g++ -std=c++17 -Wall -Wextra main.cpp -o example
 */

#include <algorithm>
#include <array>
#include <cstddef>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

struct Point {
    Point(int x, int y) : x(x), y(y) {}
    int x;
    int y;
    int sum(int extra) const { return x + y + extra; }
};

int add(int a, int b) { return a + b; }

int main() {
    // === EXAMPLE 1: apply and make_from_tuple ===
    std::cout << "=== Example 1: apply, make_from_tuple ===\n";
    std::tuple<int, int> args{3, 4};
    std::cout << "apply(add, {3,4}) = " << std::apply(add, args) << "\n";
    auto point = std::make_from_tuple<Point>(args);   // Point(3, 4)
    std::cout << "point = " << point.x << ", " << point.y << "\n";

    // === EXAMPLE 2: invoke and not_fn ===
    std::cout << "\n=== Example 2: invoke, not_fn ===\n";
    std::cout << "invoke(function):      " << std::invoke(add, 1, 2) << "\n";
    std::cout << "invoke(member function): " << std::invoke(&Point::sum, point, 10) << "\n";
    std::cout << "invoke(data member):     " << std::invoke(&Point::x, point) << "\n";
    auto isEven = [](int x) { return x % 2 == 0; };
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    std::cout << "odd numbers: " << std::count_if(numbers.begin(), numbers.end(), std::not_fn(isEven)) << "\n";

    // === EXAMPLE 3: clamp, gcd, lcm ===
    std::cout << "\n=== Example 3: clamp, gcd, lcm ===\n";
    std::cout << "clamp(15, 0, 10) = " << std::clamp(15, 0, 10) << "\n";
    std::cout << "clamp(-3, 0, 10) = " << std::clamp(-3, 0, 10) << "\n";
    std::cout << "gcd(12, 18) = " << std::gcd(12, 18) << ", lcm(4, 6) = " << std::lcm(4, 6) << "\n";

    // === EXAMPLE 4: size, data, empty, as_const, byte ===
    std::cout << "\n=== Example 4: size, data, empty, as_const, byte ===\n";
    int raw[5] = {1, 2, 3, 4, 5};
    std::cout << "size(array) = " << std::size(raw) << ", empty(vector) = " << std::empty(numbers) << "\n";
    std::cout << "first via data(): " << *std::data(numbers) << "\n";
    std::string text = "abc";
    const std::string& readOnly = std::as_const(text);
    std::cout << "as_const: " << readOnly << "\n";
    std::byte b{0x0F};
    b <<= 4;
    std::cout << "byte: " << std::to_integer<int>(b) << "\n";
    return 0;
}
