/*
 * std::string::contains, resize_and_overwrite, deleted nullptr constructor
 * (C++23, <string>, <string_view>)
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <cstdio>
#include <print>
#include <string>
#include <string_view>

int main() {
    // === EXAMPLE 1: contains ===
    std::println("=== Example 1: contains ===");
    std::string path = "/usr/local/bin/tool";
    std::string_view view = path;
    std::println("string contains \"local\": {}", path.contains("local"));
    std::println("string contains 'z':     {}", path.contains('z'));
    std::println("view contains \"bin\":     {}", view.contains("bin"));

    // === EXAMPLE 2: resize_and_overwrite ===
    std::println("");
    std::println("=== Example 2: resize_and_overwrite ===");
    // Reserves space and lets a C function fill it directly, without first
    // writing zeros into the whole buffer. The lambda returns the final size.
    std::string text;
    text.resize_and_overwrite(64, [](char* buffer, std::size_t capacity) {
        int n = std::snprintf(buffer, capacity, "value=%d hex=%x", 255, 255);
        return static_cast<std::size_t>(n);
    });
    std::println("size {}: {}", text.size(), text);

    // === EXAMPLE 3: nullptr is no longer accepted ===
    std::println("");
    std::println("=== Example 3: deleted nullptr constructors ===");
    // std::string bad = nullptr;        // error in C++23: constructor is deleted
    // std::string_view bad2 = nullptr;  // error in C++23 too
    std::string safe;                    // use an empty string instead
    std::println("empty string: '{}', empty: {}", safe, safe.empty());
    return 0;
}
