/*
 * std::unreachable (<utility>) and std::stacktrace (<stacktrace>), C++23
 *
 * unreachable() tells the compiler that a code path can never be executed.
 * stacktrace captures the chain of function calls at run time.
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 * (if the linker reports undefined stacktrace symbols, add -lstdc++exp)
 */

#include <print>
#include <stacktrace>
#include <string>
#include <utility>

enum class Direction { North, East, South, West };

std::string turnRight(Direction direction) {
    switch (direction) {
    case Direction::North:
        return "east";
    case Direction::East:
        return "south";
    case Direction::South:
        return "west";
    case Direction::West:
        return "north";
    }
    // Every enumerator is handled above. Reaching this line is a bug:
    // the behaviour is undefined, and the compiler may drop the check.
    std::unreachable();
}

std::size_t level3() {
    auto trace = std::stacktrace::current();
    return trace.size();
}

std::size_t level2() { return level3(); }
std::size_t level1() { return level2(); }

int main() {
    // === EXAMPLE 1: std::unreachable ===
    std::println("=== Example 1: unreachable ===");
    std::println("turn right from north: {}", turnRight(Direction::North));
    std::println("turn right from west:  {}", turnRight(Direction::West));
    // turnRight(static_cast<Direction>(42));   // undefined behaviour!

    // === EXAMPLE 2: std::stacktrace ===
    std::println("");
    std::println("=== Example 2: stacktrace ===");
    std::size_t frames = level1();
    std::println("captured at least 4 frames (level3, level2, level1, main): {}", frames >= 4);

    std::stacktrace empty;
    std::println("default constructed stacktrace is empty: {}", empty.empty());
    return 0;
}
