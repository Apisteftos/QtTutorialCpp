/*
 * C++20 language features: designated initializers, template lambdas,
 * lambda pack capture, using enum, [[likely]] / [[unlikely]], range-based for
 * with initializer, consteval, constinit, <version> feature test macros
 *
 * Compile: g++ -std=c++20 -Wall -Wextra main.cpp -o example
 */

#include <iostream>
#include <string>
#include <vector>
#include <version>

struct Config {
    int width = 80;
    int height = 24;
    bool fullscreen = false;
};

enum class Color { Red, Green, Blue };

std::string name(Color c) {
    switch (c) {
        using enum Color;       // use the enumerators without the Color:: prefix
    case Red:
        return "red";
    case Green:
        return "green";
    case Blue:
        return "blue";
    }
    return "?";
}

consteval int square(int x) { return x * x; }          // must be evaluated at compile time
constinit int globalCounter = square(4);               // initialized at compile time, but mutable

template <typename... Args>
auto makeAdder(Args... args) {
    return [... captured = args] { return (captured + ...); };   // pack capture with initializer
}

int main() {
    // === EXAMPLE 1: designated initializers ===
    std::cout << "=== Example 1: designated initializers ===\n";
    Config c{.width = 120, .fullscreen = true};         // order must follow the declaration
    std::cout << c.width << "x" << c.height << " fullscreen: " << std::boolalpha << c.fullscreen << "\n";

    // === EXAMPLE 2: template lambdas ===
    std::cout << "\n=== Example 2: template lambda ===\n";
    auto firstOf = []<typename T>(const std::vector<T>& v) { return v.front(); };
    std::cout << firstOf(std::vector<int>{4, 5}) << " " << firstOf(std::vector<std::string>{"a", "b"}) << "\n";
    auto adder = makeAdder(1, 2, 3);
    std::cout << "pack capture sum: " << adder() << "\n";

    // === EXAMPLE 3: using enum, consteval, constinit ===
    std::cout << "\n=== Example 3: using enum, consteval, constinit ===\n";
    std::cout << name(Color::Green) << "\n";
    constexpr int sq = square(7);
    std::cout << "consteval square(7) = " << sq << ", constinit variable = " << globalCounter << "\n";
    ++globalCounter;                                    // allowed: constinit is not const
    std::cout << "after increment: " << globalCounter << "\n";

    // === EXAMPLE 4: attributes and init in range-for ===
    std::cout << "\n=== Example 4: [[likely]], for with initializer ===\n";
    for (std::vector<int> data = {1, 2, 3}; int x : data) {
        if (x % 2 == 0) [[unlikely]] {
            std::cout << x << " (even, rare) ";
        } else [[likely]] {
            std::cout << x << " ";
        }
    }
    std::cout << "\n";
#ifdef __cpp_lib_ranges
    std::cout << "__cpp_lib_ranges = " << __cpp_lib_ranges << "\n";
#endif
    return 0;
}
