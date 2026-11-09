/*
 * std::string_view (<string_view>), std::from_chars / std::to_chars (<charconv>)
 * (C++17)
 *
 * Compile: g++ -std=c++17 -Wall -Wextra main.cpp -o example
 */

#include <array>
#include <charconv>
#include <iostream>
#include <string>
#include <string_view>
#include <system_error>

// Accepts std::string, string literals and char arrays without copying
std::size_t countSpaces(std::string_view text) {
    std::size_t count = 0;
    for (char c : text) {
        if (c == ' ') {
            ++count;
        }
    }
    return count;
}

int main() {
    // === EXAMPLE 1: string_view ===
    std::cout << "=== Example 1: string_view ===\n";
    std::string owner = "the quick brown fox";
    std::string_view view = owner;
    std::cout << "spaces: " << countSpaces(view) << ", " << countSpaces("a b c") << "\n";
    std::cout << "first word: " << view.substr(0, view.find(' ')) << "\n";
    view.remove_prefix(4);
    view.remove_suffix(4);
    std::cout << "trimmed view: " << view << "\n";
    // Warning: a string_view does not own the characters; never keep it after
    // the string it points to is gone.

    // === EXAMPLE 2: from_chars ===
    std::cout << "\n=== Example 2: from_chars ===\n";
    for (std::string_view text : {"123", "12x", "", "99999999999"}) {
        int value = 0;
        auto [end, ec] = std::from_chars(text.data(), text.data() + text.size(), value);
        if (ec == std::errc()) {
            std::cout << "'" << text << "' -> " << value << (end == text.data() + text.size() ? "" : " (stopped early)") << "\n";
        } else if (ec == std::errc::result_out_of_range) {
            std::cout << "'" << text << "' -> out of range\n";
        } else {
            std::cout << "'" << text << "' -> not a number\n";
        }
    }

    // === EXAMPLE 3: to_chars ===
    std::cout << "\n=== Example 3: to_chars ===\n";
    std::array<char, 32> buffer{};
    auto result = std::to_chars(buffer.data(), buffer.data() + buffer.size(), 255, 16);   // base 16
    std::cout << "255 in hex: " << std::string_view(buffer.data(), result.ptr - buffer.data()) << "\n";
    result = std::to_chars(buffer.data(), buffer.data() + buffer.size(), 3.14159);
    std::cout << "double: " << std::string_view(buffer.data(), result.ptr - buffer.data()) << "\n";
    return 0;
}
