/*
 * Three-way comparison: operator<=> and defaulted comparisons (<compare>, C++20)
 *
 * One defaulted <=> (and ==) gives all six comparison operators.
 *
 * Compile: g++ -std=c++20 -Wall -Wextra main.cpp -o example
 */

#include <cctype>
#include <cmath>
#include <compare>
#include <iostream>
#include <set>
#include <string>

struct Version {
    int major;
    int minor;
    int patch;
    auto operator<=>(const Version&) const = default;   // compares member by member, in order
};

struct CaseInsensitiveName {
    std::string value;
    bool operator==(const CaseInsensitiveName& other) const { return (*this <=> other) == 0; }
    std::weak_ordering operator<=>(const CaseInsensitiveName& other) const {
        auto lower = [](std::string s) {
            for (char& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
            return s;
        };
        auto a = lower(value);
        auto b = lower(other.value);
        return a < b ? std::weak_ordering::less : (a > b ? std::weak_ordering::greater : std::weak_ordering::equivalent);
    }
};

const char* text(std::strong_ordering o) {
    return o < 0 ? "less" : (o > 0 ? "greater" : "equal");
}

int main() {
    // === EXAMPLE 1: defaulted <=> ===
    std::cout << "=== Example 1: defaulted <=> ===\n";
    Version a{1, 2, 3};
    Version b{1, 3, 0};
    std::cout << std::boolalpha;
    std::cout << "a < b:  " << (a < b) << "\n";
    std::cout << "a == b: " << (a == b) << "\n";
    std::cout << "a >= b: " << (a >= b) << "\n";
    std::cout << "a <=> b is " << text(a <=> b) << "\n";

    // === EXAMPLE 2: usable as key ===
    std::cout << "\n=== Example 2: ordered container ===\n";
    std::set<Version> versions = {{2, 0, 0}, {1, 9, 9}, {1, 10, 0}};
    for (const auto& v : versions) {
        std::cout << v.major << "." << v.minor << "." << v.patch << " ";
    }
    std::cout << "\n";

    // === EXAMPLE 3: ordering categories ===
    std::cout << "\n=== Example 3: weak ordering ===\n";
    CaseInsensitiveName x{"Hello"};
    CaseInsensitiveName y{"hELLO"};
    std::cout << "\"Hello\" == \"hELLO\" (ignoring case): " << (x == y) << "\n";
    std::cout << "(x <=> y) is equivalent: " << ((x <=> y) == std::weak_ordering::equivalent) << "\n";
    double nan = std::nan("");
    auto r = 1.0 <=> nan;   // doubles give partial_ordering
    std::cout << "1.0 <=> NaN is unordered: " << (r == std::partial_ordering::unordered) << "\n";
    return 0;
}
