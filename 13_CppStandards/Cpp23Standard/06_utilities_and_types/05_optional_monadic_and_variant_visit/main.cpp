/*
 * std::optional monadic operations (and_then, transform, or_else) and
 * std::visit on classes derived from std::variant (C++23)
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <map>
#include <optional>
#include <print>
#include <string>
#include <variant>

std::optional<int> findAge(const std::map<std::string, int>& people, const std::string& name) {
    auto it = people.find(name);
    if (it == people.end()) {
        return std::nullopt;
    }
    return it->second;
}

std::optional<std::string> categorise(int age) {
    if (age < 0) {
        return std::nullopt;
    }
    return age < 18 ? "minor" : "adult";
}

// A class derived from std::variant can be passed to std::visit directly.
struct Value : std::variant<int, double, std::string> {
    using variant::variant;
};

int main() {
    std::map<std::string, int> people = {{"Mia", 34}, {"Leo", 12}};

    // === EXAMPLE 1: and_then / transform / or_else on optional ===
    std::println("=== Example 1: optional monadic operations ===");
    for (std::string name : {"Mia", "Leo", "Zoe"}) {
        auto text = findAge(people, name)
                        .and_then(categorise)                                     // may return empty itself
                        .transform([](const std::string& s) { return s + "!"; })  // changes the value
                        .or_else([] { return std::optional<std::string>("unknown"); });  // only if empty
        std::println("{}: {}", name, *text);
    }

    // === EXAMPLE 2: visit with a class derived from variant ===
    std::println("");
    std::println("=== Example 2: visit on a derived variant ===");
    Value values[] = {Value(3), Value(2.5), Value(std::string("text"))};
    for (const Value& v : values) {
        std::visit(
            [](const auto& x) { std::println("holds {}", x); },
            v);
    }
    return 0;
}
