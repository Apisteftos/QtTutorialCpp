/*
 * std::move_only_function, std::bind_back, std::invoke_r (C++23)
 *
 * move_only_function (<functional>): like std::function, but accepts callables
 *                                    that can only be moved (e.g. owning a unique_ptr)
 * bind_back (<functional>)         : fix the LAST arguments of a callable
 * invoke_r (<functional>)          : call and convert the result to a chosen type
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <functional>
#include <memory>
#include <print>
#include <string>
#include <vector>

int subtract(int a, int b) { return a - b; }

int main() {
    // === EXAMPLE 1: move_only_function ===
    std::println("=== Example 1: move_only_function ===");
    auto resource = std::make_unique<int>(41);
    // std::function would not compile here: the lambda owns a unique_ptr
    std::move_only_function<int()> task = [res = std::move(resource)] { return *res + 1; };
    std::println("task() = {}", task());

    std::vector<std::move_only_function<void()>> queue;
    queue.push_back([] { std::println("first job"); });
    queue.push_back([p = std::make_unique<std::string>("second job")] { std::println("{}", *p); });
    for (auto& job : queue) {
        job();
    }

    // === EXAMPLE 2: bind_back ===
    std::println("");
    std::println("=== Example 2: bind_back ===");
    auto minusTen = std::bind_back(subtract, 10);   // x -> subtract(x, 10)
    std::println("minusTen(25) = {}", minusTen(25));

    auto repeatWord = std::bind_back(std::plus<>{}, std::string("!"));
    std::println("{}", repeatWord(std::string("hello")));

    // === EXAMPLE 3: invoke_r ===
    std::println("");
    std::println("=== Example 3: invoke_r ===");
    double asDouble = std::invoke_r<double>(subtract, 9, 4);   // int result converted to double
    std::println("invoke_r<double>: {}", asDouble);
    std::invoke_r<void>(subtract, 9, 4);                       // result thrown away
    std::println("invoke_r<void> ran without using the result");
    return 0;
}
