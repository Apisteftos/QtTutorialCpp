#include <iostream>
#include <print>
#include <vector>
#include <expected>
#include <string>
#include <generator>
#include <ranges>
#include <utility>

// ============================================================
// C++23 KEYWORDS + FEATURES OVERVIEW
// ============================================================
//
// ACTUAL NEW / IMPORTANT LANGUAGE ADDITIONS:
// ------------------------------------------------------------
// if consteval        (C++23 feature)
// if constinit        (already C++20, reinforced usage)
// auto(x)             (deducing this - C++23)
// static_assert(msg)  (improved message support)
//
// COROUTINES LIBRARY ADDITION:
// ------------------------------------------------------------
// std::generator (official in C++23)
//
// NEW STANDARD LIBRARY FEATURES:
// ------------------------------------------------------------
// std::print / std::println
// std::expected (error handling alternative to exceptions)
// std::ranges improvements
// std::mdspan (multi-dimensional views, not shown here)
//
// ============================================================

// ============================================================
// 1. if consteval (C++23)
// ============================================================

consteval int compileTimeOnly(int x)
{
    return x * x;
}

constexpr int maybeRuntime(int x)
{
    if consteval
    {
        // executed only at compile time
        return x * 2;
    }
    else
    {
        // runtime path
        return x + 2;
    }
}

// ============================================================
// 2. DEDUCING THIS (auto this) - C++23
// ============================================================

struct Counter
{
    int value = 0;

    // "auto this" allows perfect forwarding style member functions
    void increment(this auto& self)
    {
        self.value++;
    }

    void print(this const auto& self)
    {
        std::cout << "Counter = " << self.value << "\n";
    }
};

// ============================================================
// 3. std::expected (modern error handling)
// ============================================================

std::expected<int, std::string> safeDivide(int a, int b)
{
    if (b == 0)
        return std::unexpected("division by zero");

    return a / b;
}

// ============================================================
// 4. std::print / std::println (C++23 IO)
// ============================================================

void printDemo()
{
    std::print("Hello {}\n", "C++23");
    std::println("Sum: {}", 10 + 20);
}

// ============================================================
// 5. std::generator (C++23 coroutines simplified)
// ============================================================

std::generator<int> rangeGenerator(int start, int end)
{
    for (int i = start; i <= end; i++)
        co_yield i;
}

// ============================================================
// 6. static_assert improvements (C++23 message improvements)
// ============================================================

static_assert(sizeof(int) >= 4, "int must be at least 4 bytes");

// ============================================================
// MAIN DEMO
// ============================================================

int main()
{
    // ---------------- compile-time / runtime split ----------------
    std::cout << "if consteval:\n";
    std::cout << maybeRuntime(5) << "\n\n";

    // ---------------- deducing this ----------------
    Counter c;
    c.increment();
    c.increment();
    c.print();
    std::cout << "\n";

    // ---------------- expected ----------------
    auto result = safeDivide(10, 2);

    if (result)
        std::cout << "expected result = " << *result << "\n";
    else
        std::cout << "error: " << result.error() << "\n";

    std::cout << "\n";

    // ---------------- print / println ----------------
    printDemo();
    std::cout << "\n";

    // ---------------- generator ----------------
    std::cout << "generator output: ";
    for (int v : rangeGenerator(1, 5))
        std::cout << v << " ";
    std::cout << "\n";

    return 0;
}
