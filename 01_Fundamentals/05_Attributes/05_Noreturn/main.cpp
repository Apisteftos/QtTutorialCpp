// ============================================================
// [[noreturn]] - this function never comes back
//
// Topics:
//   1. A helper that always throws
//   2. No "control reaches end" warning after the call
//   3. A noreturn function that does return (warning)
//   4. Standard noreturn functions
//
// See Readme.md for the explanation and the compiler warnings.
// Build: g++ -std=c++23 -Wall -Wextra main.cpp -o noreturn
// ============================================================

#include <print>
#include <stdexcept>
#include <string>

// === EXAMPLE 1: A HELPER THAT ALWAYS THROWS ===

[[noreturn]] void fail(const std::string& message)
{
    throw std::runtime_error(message);
}

int safeDivide(int a, int b)
{
    if (b != 0) {
        return a / b;
    }
    fail("division by zero");
    // No return statement here: the compiler knows fail() never returns,
    // so there is no "control reaches end of non-void function" warning.
}

void example1()
{
    std::println("=== EXAMPLE 1: HELPER THAT ALWAYS THROWS ===");

    std::println("safeDivide(10, 2) = {}", safeDivide(10, 2));

    try {
        std::println("safeDivide(1, 0) = {}", safeDivide(1, 0));
    } catch (const std::exception& e) {
        std::println("caught: {}", e.what());
    }
}

// === EXAMPLE 2: WITHOUT THE ATTRIBUTE ===

//! void failPlain(const std::string& message)
//! {
//!     throw std::runtime_error(message);
//! }
//!
//! int safeDivideBad(int a, int b)
//! {
//!     if (b != 0) {
//!         return a / b;
//!     }
//!     failPlain("division by zero");
//! }                                    // warning: control reaches end of non-void function

void example2()
{
    std::println("=== EXAMPLE 2: WITHOUT THE ATTRIBUTE ===");
    std::println("see Readme.md: the compiler cannot know that failPlain() never returns");
}

// === EXAMPLE 3: A NORETURN FUNCTION THAT RETURNS ===

//! [[noreturn]] void wrong(int x)
//! {
//!     if (x > 0) {
//!         throw std::runtime_error("positive");
//!     }
//! }                                    // warning: 'noreturn' function does return

void example3()
{
    std::println("=== EXAMPLE 3: A NORETURN FUNCTION THAT RETURNS ===");
    std::println("see Readme.md: returning from a [[noreturn]] function is undefined behavior");
}

// === EXAMPLE 4: STANDARD NORETURN FUNCTIONS ===

void example4()
{
    std::println("=== EXAMPLE 4: STANDARD NORETURN FUNCTIONS ===");
    std::println("std::exit, std::abort, std::terminate, std::quick_exit and");
    std::println("std::unreachable (C++23) are all [[noreturn]] or never return");
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
