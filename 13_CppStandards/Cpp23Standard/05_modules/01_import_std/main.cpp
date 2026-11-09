/*
 * import std; and import std.compat; (C++23)
 *
 * Instead of many #include lines, import the whole standard library as a
 * module. `import std` provides everything in namespace std;
 * `import std.compat` also provides the global C names (::printf, ::size_t).
 *
 * Needs CMake 3.30+ (4.x recommended), Ninja and GCC 15. See the Readme.
 */

import std;

int main() {
    // === EXAMPLE 1: use the standard library without includes ===
    std::println("=== Example 1: import std ===");
    std::vector<int> numbers = {5, 3, 8, 1};
    std::ranges::sort(numbers);
    std::println("sorted: {}", numbers);

    std::map<std::string, int> ages = {{"Ann", 31}, {"Ben", 27}};
    for (const auto& [name, age] : ages) {
        std::println("{} is {}", name, age);
    }

    // === EXAMPLE 2: ranges and expected come with it ===
    std::println("");
    std::println("=== Example 2: everything is available ===");
    std::expected<int, std::string> result = 42;
    std::println("expected holds: {}", *result);
    // NOTE: with GCC 15 the pipe operator `|` on views can fail inside a module
    // build ("use of operator| before deduction of auto"), so the adaptors are
    // called like functions here.
    auto evens = std::views::filter(std::views::iota(1, 10), [](int x) { return x % 2 == 0; });
    std::println("evens: {}", std::ranges::to<std::vector>(evens));
    return 0;
}
