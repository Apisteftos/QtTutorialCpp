/*
 * ranges::contains, contains_subrange, starts_with, ends_with, find_last,
 * find_last_if, find_last_if_not (<algorithm>), shift_left / shift_right
 * (C++23)
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <algorithm>
#include <print>
#include <ranges>
#include <string>
#include <version>
#include <vector>

int main() {
    std::vector<int> data = {3, 8, 15, 4, 8, 23, 42};
    std::println("data: {}", data);

    // === EXAMPLE 1: contains, contains_subrange, starts_with, ends_with ===
    std::println("");
    std::println("=== Example 1: contains and friends ===");
    std::println("contains 15:            {}", std::ranges::contains(data, 15));
    std::println("contains 99:            {}", std::ranges::contains(data, 99));

    std::vector<int> part = {15, 4, 8};
    std::println("contains_subrange:      {}", std::ranges::contains_subrange(data, part));
#ifdef __cpp_lib_ranges_starts_ends_with
    std::println("starts_with {{3, 8}}:    {}", std::ranges::starts_with(data, std::vector{3, 8}));
    std::println("ends_with {{23, 42}}:    {}", std::ranges::ends_with(data, std::vector{23, 42}));
#else
    // ranges::starts_with / ends_with arrive in libstdc++ 16 (GCC 15 does not have them yet)
    std::println("starts_with / ends_with: not available in this standard library");
#endif

    // === EXAMPLE 2: find_last family ===
    std::println("");
    std::println("=== Example 2: find_last, find_last_if, find_last_if_not ===");
    // These return a subrange that starts at the LAST match and goes to the end.
    auto lastEight = std::ranges::find_last(data, 8);
    std::println("find_last 8, found at index {}: {}", lastEight.begin() - data.begin(),
                 lastEight | std::ranges::to<std::vector>());

    auto lastEven = std::ranges::find_last_if(data, [](int x) { return x % 2 == 0; });
    std::println("last even number:        {}", *lastEven.begin());

    auto lastNotEight = std::ranges::find_last_if_not(data, [](int x) { return x >= 8; });
    std::println("last number below 8:     {}", *lastNotEight.begin());

    auto missing = std::ranges::find_last(data, 99);
    std::println("find_last 99 is empty:   {}", missing.empty());

    // === EXAMPLE 3: shift_left / shift_right ===
    std::println("");
    std::println("=== Example 3: shift_left and shift_right ===");
    std::vector<int> a = {1, 2, 3, 4, 5};
    std::shift_left(a.begin(), a.end(), 2);   // moves elements to the front, the tail is unspecified
    std::println("after shift_left(2):  first three = {}", std::vector(a.begin(), a.begin() + 3));

    std::vector<int> b = {1, 2, 3, 4, 5};
    std::shift_right(b.begin(), b.end(), 2);  // moves elements to the back, the head is unspecified
    std::println("after shift_right(2): last three  = {}", std::vector(b.begin() + 2, b.end()));
    return 0;
}
