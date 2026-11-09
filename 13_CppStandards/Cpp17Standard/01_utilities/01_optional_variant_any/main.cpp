/*
 * std::optional, std::variant, std::any (C++17)
 *
 * optional : a value that may be missing
 * variant  : a type-safe union, holds exactly one of several types
 * any      : holds a value of any copyable type
 *
 * Compile: g++ -std=c++17 -Wall -Wextra main.cpp -o example
 */

#include <any>
#include <iostream>
#include <optional>
#include <string>
#include <variant>
#include <vector>

std::optional<int> parsePositive(const std::string& text) {
    try {
        int value = std::stoi(text);
        if (value > 0) {
            return value;
        }
    } catch (const std::exception&) {
    }
    return std::nullopt;
}

// A helper that lets std::visit take several lambdas
template <typename... Ts>
struct Overloaded : Ts... {
    using Ts::operator()...;
};
template <typename... Ts>
Overloaded(Ts...) -> Overloaded<Ts...>;

int main() {
    // === EXAMPLE 1: optional ===
    std::cout << "=== Example 1: optional ===\n";
    for (const std::string text : {"42", "-5", "abc"}) {
        auto value = parsePositive(text);
        if (value) {
            std::cout << text << " -> " << *value << "\n";
        } else {
            std::cout << text << " -> no value (value_or: " << value.value_or(0) << ")\n";
        }
    }

    // === EXAMPLE 2: variant and visit ===
    std::cout << "\n=== Example 2: variant and visit ===\n";
    std::vector<std::variant<int, double, std::string>> items = {7, 2.5, std::string("text")};
    for (const auto& item : items) {
        std::visit(Overloaded{
                       [](int i) { std::cout << "int " << i << "\n"; },
                       [](double d) { std::cout << "double " << d << "\n"; },
                       [](const std::string& s) { std::cout << "string " << s << "\n"; },
                   },
                   item);
    }
    std::variant<int, std::string> v = 10;
    std::cout << "holds int: " << std::holds_alternative<int>(v) << ", index " << v.index() << "\n";
    v = "now a string";
    if (auto* s = std::get_if<std::string>(&v)) {
        std::cout << "get_if string: " << *s << "\n";
    }

    // === EXAMPLE 3: any ===
    std::cout << "\n=== Example 3: any ===\n";
    std::any box = 42;
    std::cout << "int inside: " << std::any_cast<int>(box) << "\n";
    box = std::string("hello");
    std::cout << "string inside: " << std::any_cast<std::string>(box) << "\n";
    try {
        std::any_cast<int>(box);
    } catch (const std::bad_any_cast& e) {
        std::cout << "wrong type -> bad_any_cast\n";
    }
    std::cout << "has value: " << box.has_value() << "\n";
    return 0;
}
