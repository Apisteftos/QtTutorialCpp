/*
 * std::to_underlying, std::forward_like (<utility>), explicit object
 * parameter ("deducing this") (C++23)
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <memory>
#include <print>
#include <string>
#include <utility>

enum class Level : unsigned char { Low = 1, Medium = 5, High = 10 };

struct Wrapper {
    std::string text = "payload";

    // Deducing this: `self` is the object itself, with its value category.
    // forward_like forwards the member with the same constness / rvalue-ness as `self`.
    template <typename Self>
    auto&& get(this Self&& self) {
        return std::forward_like<Self>(self.text);
    }
};

int main() {
    // === EXAMPLE 1: to_underlying ===
    std::println("=== Example 1: to_underlying ===");
    Level level = Level::High;
    auto number = std::to_underlying(level);   // no cast to the right type needed
    std::println("High is {} and the type is unsigned char: {}", static_cast<int>(number),
                 std::is_same_v<decltype(number), unsigned char>);

    // === EXAMPLE 2: forward_like ===
    std::println("");
    std::println("=== Example 2: forward_like with deducing this ===");
    Wrapper w;
    const Wrapper cw;
    static_assert(std::is_same_v<decltype(w.get()), std::string&>);
    static_assert(std::is_same_v<decltype(cw.get()), const std::string&>);
    static_assert(std::is_same_v<decltype(Wrapper{}.get()), std::string&&>);
    std::println("lvalue object -> lvalue member, const object -> const member, rvalue object -> rvalue member");

    std::string taken = Wrapper{}.get();   // moves the string out of the temporary
    std::println("moved out: {}", taken);

    // === EXAMPLE 3: deducing this for a recursive lambda ===
    std::println("");
    std::println("=== Example 3: recursive lambda ===");
    auto factorial = [](this auto&& self, int n) -> long long {
        return n <= 1 ? 1 : n * self(n - 1);
    };
    std::println("10! = {}", factorial(10));
    return 0;
}
