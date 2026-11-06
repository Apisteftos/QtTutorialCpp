// ============================================================
// [[fallthrough]] - falling into the next case is intended
//
// Topics:
//   1. A cascading switch (permissions)
//   2. Grouped case labels (no attribute needed)
//   3. The same switch without the attribute (warning)
//
// See Readme.md for the explanation and the compiler warnings.
// Build: g++ -std=c++23 -Wall -Wextra main.cpp -o fallthrough
// ============================================================

#include <print>
#include <string>

// === EXAMPLE 1: A CASCADING SWITCH ===

enum class Level { Guest, User, Admin };

// Higher levels get everything the lower levels get.
std::string permissions(Level level)
{
    std::string result;
    switch (level) {
    case Level::Admin:
        result += "manage ";
        [[fallthrough]];
    case Level::User:
        result += "write ";
        [[fallthrough]];
    case Level::Guest:
        result += "read";
        break;
    }
    return result;
}

void example1()
{
    std::println("=== EXAMPLE 1: CASCADING SWITCH ===");
    std::println("Guest: {}", permissions(Level::Guest));
    std::println("User:  {}", permissions(Level::User));
    std::println("Admin: {}", permissions(Level::Admin));
}

// === EXAMPLE 2: GROUPED CASE LABELS ===

// Several labels in a row without a statement between them are not a
// fall-through. No attribute is needed.
bool isVowel(char c)
{
    switch (c) {
    case 'a':
    case 'e':
    case 'i':
    case 'o':
    case 'u':
        return true;
    default:
        return false;
    }
}

void example2()
{
    std::println("=== EXAMPLE 2: GROUPED LABELS ===");
    std::println("isVowel('e') = {}", isVowel('e'));
    std::println("isVowel('x') = {}", isVowel('x'));
}

// === EXAMPLE 3: WITHOUT THE ATTRIBUTE ===

//! std::string permissionsBad(Level level)
//! {
//!     std::string result;
//!     switch (level) {
//!     case Level::Admin:
//!         result += "manage ";          // warning: this statement may fall through
//!     case Level::User:
//!         result += "write ";           // warning: this statement may fall through
//!     case Level::Guest:
//!         result += "read";
//!         break;
//!     }
//!     return result;
//! }

void example3()
{
    std::println("=== EXAMPLE 3: WITHOUT THE ATTRIBUTE ===");
    std::println("see Readme.md: -Wimplicit-fallthrough warns, the code would still run the same");
}

int main()
{
    example1();
    std::println("");
    example2();
    std::println("");
    example3();
    return 0;
}
