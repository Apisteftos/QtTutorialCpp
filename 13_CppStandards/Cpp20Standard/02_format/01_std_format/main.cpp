/*
 * std::format, std::format_to, std::format_to_n, std::formatted_size,
 * custom formatter (<format>, C++20)
 *
 * Note: std::print / std::println need C++23. In C++20 the formatted string
 * is created with std::format and written with std::cout.
 *
 * Compile: g++ -std=c++20 -Wall -Wextra main.cpp -o example
 */

#include <format>
#include <iostream>
#include <iterator>
#include <string>

struct Point {
    int x;
    int y;
};

// A custom formatter: {} prints "(x, y)"
template <>
struct std::formatter<Point> {
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }
    auto format(const Point& p, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "({}, {})", p.x, p.y);
    }
};

int main() {
    // === EXAMPLE 1: placeholders and specs ===
    std::cout << "=== Example 1: format specs ===\n";
    std::cout << std::format("{} is {} years old\n", "Ann", 31);
    std::cout << std::format("{1} before {0}\n", "second", "first");        // by index
    std::cout << std::format("[{:>8}] [{:<8}] [{:^8}]\n", "right", "left", "mid");
    std::cout << std::format("[{:*^9}] [{:08.3f}] [{:+d}]\n", "x", 3.14159, 5);
    std::cout << std::format("hex {:#x}, binary {:#b}, octal {:o}\n", 255, 5, 8);
    std::cout << std::format("{:.2f}% done, {:e}\n", 99.5, 12345.678);
    std::cout << std::format("braces: {{}}, bool: {}\n", true);

    // === EXAMPLE 2: width from an argument ===
    std::cout << "\n=== Example 2: dynamic width ===\n";
    std::cout << std::format("[{:{}}]\n", 42, 6);
    std::cout << std::format("[{:{}.{}f}]\n", 3.14159, 10, 2);

    // === EXAMPLE 3: format_to, format_to_n, formatted_size ===
    std::cout << "\n=== Example 3: output iterators ===\n";
    std::string out;
    std::format_to(std::back_inserter(out), "{}-{}", 1, 2);
    std::cout << "format_to: " << out << "\n";
    char buffer[8];
    auto result = std::format_to_n(buffer, sizeof(buffer) - 1, "{}", "a long text");
    *result.out = '\0';
    std::cout << "format_to_n cut: " << buffer << " (would need " << result.size << " characters)\n";
    std::cout << "formatted_size: " << std::formatted_size("{:>10}", "abc") << "\n";

    // === EXAMPLE 4: custom type ===
    std::cout << "\n=== Example 4: custom formatter ===\n";
    std::cout << std::format("point: {}\n", Point{3, 4});
    return 0;
}
