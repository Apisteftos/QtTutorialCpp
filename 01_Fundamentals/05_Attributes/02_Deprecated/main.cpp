// ============================================================
// [[deprecated]] - mark old API so users get a warning
//
// Topics:
//   1. A deprecated function with a replacement hint
//   2. A deprecated class, enum value and type alias
//   3. A deprecated variable and namespace
//   4. Calling deprecated code on purpose (silencing the warning)
//
// See Readme.md for the explanation and the compiler warnings.
// Build: g++ -std=c++23 -Wall -Wextra main.cpp -o deprecated
// ============================================================

#include <print>
#include <string>

// === EXAMPLE 1: A DEPRECATED FUNCTION ===

[[deprecated("use sumAll(a, b) instead")]]
int add(int a, int b)
{
    return a + b;
}

int sumAll(int a, int b)
{
    return a + b;
}

void example1()
{
    std::println("=== EXAMPLE 1: DEPRECATED FUNCTION ===");

    std::println("sumAll(2, 3) = {}", sumAll(2, 3));

    //! std::println("add(2, 3) = {}", add(2, 3));       // warning: 'add' is deprecated
}

// === EXAMPLE 2: CLASS, ENUM VALUE, TYPE ALIAS ===

class [[deprecated("use Logger instead")]] OldLogger {
public:
    void log(const std::string& text) const { std::println("[old] {}", text); }
};

class Logger {
public:
    void log(const std::string& text) const { std::println("[log] {}", text); }
};

enum class Color {
    Red,
    Green,
    Grey [[deprecated("use Gray")]] = 5,
    Gray = 5
};

using Text [[deprecated("use std::string")]] = std::string;

void example2()
{
    std::println("=== EXAMPLE 2: CLASS, ENUM VALUE, ALIAS ===");

    Logger logger;
    logger.log("new logger");

    Color c = Color::Gray;
    std::println("Gray = {}", static_cast<int>(c));

    //! OldLogger old;                                   // warning: 'OldLogger' is deprecated
    //! Color g = Color::Grey;                           // warning: 'Grey' is deprecated
    //! Text t = "hello";                                // warning: 'Text' is deprecated
}

// === EXAMPLE 3: VARIABLE AND NAMESPACE ===

[[deprecated("use kMaxUsers")]] constexpr int MAX_USERS = 100;
constexpr int kMaxUsers = 100;

namespace [[deprecated("moved to namespace util")]] legacy {
    inline int twice(int x) { return 2 * x; }
}

namespace util {
    inline int twice(int x) { return 2 * x; }
}

void example3()
{
    std::println("=== EXAMPLE 3: VARIABLE AND NAMESPACE ===");

    std::println("kMaxUsers = {}", kMaxUsers);
    std::println("util::twice(21) = {}", util::twice(21));

    //! std::println("MAX_USERS = {}", MAX_USERS);                // warning: deprecated variable
    //! std::println("legacy::twice(21) = {}", legacy::twice(21)); // warning: deprecated namespace
}

// === EXAMPLE 4: CALLING DEPRECATED CODE ON PURPOSE ===

// Old code that must keep working during a migration can silence the warning
// for a small region. GCC and Clang accept this pragma; MSVC uses
// #pragma warning(disable : 4996).
void example4()
{
    std::println("=== EXAMPLE 4: SILENCING THE WARNING ===");

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    int old = add(10, 20);
#pragma GCC diagnostic pop

    std::println("add(10, 20) = {} (still works, no warning)", old);
}

int main()
{
    example1();
    std::println("");
    example2();
    std::println("");
    example3();
    std::println("");
    example4();
    return 0;
}
