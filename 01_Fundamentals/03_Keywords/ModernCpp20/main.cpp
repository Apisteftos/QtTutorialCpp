#include <iostream>
#include <concepts>
#include <ranges>
#include <vector>
#include <string>
#include <coroutine>
#include <optional>

// ============================================================
// C++20 KEYWORDS + MAJOR FEATURES (BIG OVERVIEW)
// ============================================================
//
// REAL NEW KEYWORDS IN C++20:
// ------------------------------------------------------------
// concept
// requires
// consteval   (also in C++20, formalized)
// constinit   (from C++20)
// co_await
// co_return
// co_yield
//
// BIG FEATURES (not always "keywords"):
// ------------------------------------------------------------
// concepts (type constraints)
// ranges
// coroutines
// modules (syntax concept, not shown in one file easily)
// designated initializers
// spaceship operator <=>
// ============================================================

// ============================================================
// 1. CONCEPTS (concept + requires)
// ============================================================

// A concept defines compile-time constraints for types
template <typename T>
concept Numeric = std::is_arithmetic_v<T>;

// requires can also define constraints manually
template <typename T>
    requires std::integral<T>
T add(T a, T b)
{
    return a + b;
}

// Using concept directly
template <Numeric T>
T multiply(T a, T b)
{
    return a * b;
}

// ============================================================
// 2. consteval (MUST run at compile time)
// ============================================================

consteval int square(int x)
{
    return x * x;
}

// ============================================================
// 3. constinit (guaranteed compile-time initialization)
// ============================================================

constinit int globalCounter = 100;

// ============================================================
// 4. COROUTINES (co_await / co_yield / co_return)
// ============================================================

struct SimpleGenerator
{
    struct promise_type
    {
        int current_value;

        SimpleGenerator get_return_object()
        {
            return SimpleGenerator{this};
        }

        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }

        std::suspend_always yield_value(int value)
        {
            current_value = value;
            return {};
        }

        void return_void() {}
        void unhandled_exception() {}
    };

    promise_type* p;

    SimpleGenerator(promise_type* p) : p(p) {}

    int value() { return p->current_value; }
};

// coroutine function
SimpleGenerator counter()
{
    co_yield 1;
    co_yield 2;
    co_yield 3;
}

// ============================================================
// 5. RANGES (C++20 modern iteration)
// ============================================================

void rangesExample()
{
    std::vector<int> v = {1,2,3,4,5,6,7,8,9,10};

    // ranges view + filter + transform
    auto evenSquares = v
                       | std::views::filter([](int x) { return x % 2 == 0; })
                       | std::views::transform([](int x) { return x * x; });

    std::cout << "Ranges result: ";
    for (int x : evenSquares)
        std::cout << x << " ";
    std::cout << "\n";
}

// ============================================================
// 6. SPACESHIP OPERATOR (C++20 <=>
// ============================================================

struct Point
{
    int x, y;

    auto operator<=>(const Point&) const = default;
};

// ============================================================
// MAIN DEMO
// ============================================================

int main()
{
    // ---------------- concepts ----------------
    std::cout << "Concepts:\n";
    std::cout << "add<int>(2,3) = " << add(2, 3) << "\n";
    std::cout << "multiply<float>(2.5, 4.0) = " << multiply(2.5, 4.0) << "\n\n";

    // ---------------- consteval ----------------
    constexpr int c = square(5);
    std::cout << "consteval square(5) = " << c << "\n\n";

    // ---------------- constinit ----------------
    std::cout << "constinit globalCounter = " << globalCounter << "\n\n";

    // ---------------- ranges ----------------
    rangesExample();
    std::cout << "\n";

    // ---------------- coroutines ----------------
    std::cout << "coroutines:\n";
    auto gen = counter();
    std::cout << gen.value() << "\n";
    std::cout << gen.value() << "\n";
    std::cout << gen.value() << "\n\n";

    // ---------------- spaceship operator ----------------
    Point p1{1,2}, p2{1,3};

    auto result = (p1 <=> p2);

    if (result < 0)
        std::cout << "p1 < p2\n";
    else if (result > 0)
        std::cout << "p1 > p2\n";
    else
        std::cout << "p1 == p2\n";

    return 0;
}
