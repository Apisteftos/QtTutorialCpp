/*
 * std::print and std::println (C++23, <print>)
 *
 * print writes formatted text, println does the same and adds a line break.
 * Both use the std::format syntax: {} placeholders with optional format specs.
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o 01_printExample
 */

#include <cstdio>
#include <print>
#include <string>

int main() {
    int x = 1;
    double y = 3.6;
    char f = 'F';
    bool isTrue = true;
    std::string s = "FDFA";
    float floatVal = 1233564.456F;

    // === EXAMPLE 1: Basic values ===
    std::println("=== Example 1: Basic values ===");
    std::println("The value x is: {}", x);
    std::println("The value y is: {}", y);
    std::println("The value f is: {}", f);
    std::println("The boolean value is: {}", isTrue);
    std::println("The string value is: {}", s);

    // === EXAMPLE 2: Format specs for floating point ===
    std::println("");
    std::println("=== Example 2: Floating point specs ===");
    std::println("{:.3f}", floatVal);          // 3 decimals
    std::println("{:14.3f}", floatVal);        // width 14, numbers are right-aligned
    std::println("{:<14.3f}|", floatVal);      // left-aligned
    std::println("{:016.3f}", floatVal);       // zero-padded
    std::println("{:+.3f}", floatVal);         // always show the sign
    std::println("{0:.3f} {0:.1f}", floatVal); // same argument twice, by index

    // === EXAMPLE 3: print without line break, and other output targets ===
    std::println("");
    std::println("=== Example 3: print, stderr and FILE* ===");
    std::print("no line break here, ");
    std::print("so this continues the line");
    std::println("");
    std::println(stderr, "this line goes to stderr");
    std::println(stdout, "this line goes to stdout via a FILE*");
    return 0;
}
