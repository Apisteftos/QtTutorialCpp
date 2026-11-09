/*
 * C++17 language features that the library relies on:
 * structured bindings, if/switch with initializer, if constexpr, fold
 * expressions, class template argument deduction (CTAD), inline variables,
 * nested namespaces, [[nodiscard]] / [[maybe_unused]] / [[fallthrough]]
 *
 * Compile: g++ -std=c++17 -Wall -Wextra main.cpp -o example
 */

#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace Shapes::Flat {          // nested namespace definition
inline constexpr double pi = 3.14159;   // inline variable: one definition across files
}

[[nodiscard]] int computeSquare(int x) { return x * x; }

template <typename T>
std::string describe(const T& value) {
    if constexpr (std::is_integral_v<T>) {          // the other branch is not compiled
        return "integer " + std::to_string(value);
    } else if constexpr (std::is_floating_point_v<T>) {
        return "floating point " + std::to_string(value);
    } else {
        return "something else";
    }
}

template <typename... Args>
auto sumAll(Args... args) {
    return (args + ...);              // unary right fold
}

template <typename... Args>
void printAll(const Args&... args) {
    ((std::cout << args << " "), ...);   // fold over the comma operator
    std::cout << "\n";
}

std::pair<int, std::string> makePair() { return {7, "seven"}; }

int main() {
    // === EXAMPLE 1: structured bindings ===
    std::cout << "=== Example 1: structured bindings ===\n";
    auto [number, name] = makePair();
    std::cout << number << " " << name << "\n";
    std::map<std::string, int> ages = {{"Ann", 31}, {"Ben", 27}};
    for (const auto& [person, age] : ages) {
        std::cout << person << " is " << age << "\n";
    }

    // === EXAMPLE 2: if / switch with initializer ===
    std::cout << "\n=== Example 2: if with initializer ===\n";
    if (auto it = ages.find("Ben"); it != ages.end()) {
        std::cout << "found " << it->first << "\n";
    }
    switch (int code = computeSquare(3); code) {
    case 9:
        std::cout << "nine\n";
        [[fallthrough]];
    default:
        std::cout << "default also runs\n";
    }

    // === EXAMPLE 3: if constexpr and fold expressions ===
    std::cout << "\n=== Example 3: if constexpr, fold ===\n";
    std::cout << describe(5) << ", " << describe(2.5) << ", " << describe("text") << "\n";
    std::cout << "sumAll = " << sumAll(1, 2, 3, 4) << "\n";
    printAll(1, "two", 3.5);

    // === EXAMPLE 4: CTAD and inline variable ===
    std::cout << "\n=== Example 4: CTAD ===\n";
    std::pair p{1, 2.5};              // pair<int, double> deduced
    std::vector v{1, 2, 3};           // vector<int> deduced
    std::tuple t{1, 'x', std::string("s")};
    std::cout << p.first << " " << v.size() << " " << std::get<1>(t) << "\n";
    std::cout << "pi = " << Shapes::Flat::pi << "\n";
    [[maybe_unused]] int unused = 0;
    return 0;
}
