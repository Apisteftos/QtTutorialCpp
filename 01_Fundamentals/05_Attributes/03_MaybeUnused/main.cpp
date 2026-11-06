// ============================================================
// [[maybe_unused]] - an unused name is fine here
//
// Topics:
//   1. Unused function parameter
//   2. Variable used only in assert() (disappears with -DNDEBUG)
//   3. Structured binding used only in assert()
//   4. Unused function in a translation unit
//
// See Readme.md for the explanation and the compiler warnings.
// Build: g++ -std=c++23 -Wall -Wextra main.cpp -o maybe_unused
//        g++ -std=c++23 -Wall -Wextra -DNDEBUG main.cpp -o maybe_unused
// ============================================================

#include <cassert>
#include <print>
#include <utility>

// === EXAMPLE 1: UNUSED PARAMETER ===

// A callback interface fixes the signature. Not every implementation needs
// every parameter.
void onKey([[maybe_unused]] int keyCode, bool pressed)
{
    std::println("key pressed? {}", pressed);
}

//! void onKeyBad(int keyCode, bool pressed)    // warning: unused parameter 'keyCode'
//! {
//!     std::println("key pressed? {}", pressed);
//! }

void example1()
{
    std::println("=== EXAMPLE 1: UNUSED PARAMETER ===");
    onKey(65, true);
}

// === EXAMPLE 2: ONLY USED IN assert() ===

int compute()
{
    return 42;
}

void example2()
{
    std::println("=== EXAMPLE 2: ONLY USED IN assert() ===");

    // With -DNDEBUG the assert disappears and 'result' would be unused.
    [[maybe_unused]] int result = compute();
    assert(result == 42);

    std::println("compute() was called");

    //! int other = compute();                  // with -DNDEBUG: warning: unused variable
    //! assert(other == 42);
}

// === EXAMPLE 3: STRUCTURED BINDING USED ONLY IN assert() ===

std::pair<int, int> makeRange()
{
    return {1, 10};
}

void example3()
{
    std::println("=== EXAMPLE 3: STRUCTURED BINDING ===");

    // GCC only warns when ALL names of a structured binding are unused.
    // With -DNDEBUG both names are unused, so the attribute is needed.
    [[maybe_unused]] auto [low, high] = makeRange();
    assert(low < high);

    std::println("range created");

    //! auto [low2, high2] = makeRange();            // with -DNDEBUG: warning: structured binding set but not used
    //! assert(low2 < high2);
}

// === EXAMPLE 4: UNUSED FUNCTION ===

// A helper that is only used in some build configurations.
[[maybe_unused]] static void debugDump(int value)
{
    std::println("debug: {}", value);
}

//! static void debugDumpBad(int value)            // warning: defined but not used
//! {
//!     std::println("debug: {}", value);
//! }

void example4()
{
    std::println("=== EXAMPLE 4: UNUSED FUNCTION ===");
    std::println("debugDump is defined but never called, and that is fine");
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
