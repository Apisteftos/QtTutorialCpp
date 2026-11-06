// ============================================================
// [[assume(expression)]] - promise the optimizer something is true (C++23)
//
// Topics:
//   1. A value that is never negative
//   2. A loop count that is a multiple of 8
//   3. What happens if the promise is wrong (not run here)
//
// The expression is NOT evaluated at run time. If it would be false,
// the behavior is undefined. See Readme.md.
// Build: g++ -std=c++23 -Wall -Wextra main.cpp -o assume
// ============================================================

#include <print>

// === EXAMPLE 1: A VALUE THAT IS NEVER NEGATIVE ===

int remainderOf8(int x)
{
    [[assume(x >= 0)]];
    return x % 8;           // can be a single AND instruction for x >= 0
}

int remainderOf8NoHint(int x)
{
    return x % 8;           // must handle negative x too
}

void example1()
{
    std::println("=== EXAMPLE 1: NEVER NEGATIVE ===");
    std::println("remainderOf8(21)       = {}", remainderOf8(21));
    std::println("remainderOf8NoHint(21) = {}", remainderOf8NoHint(21));
}

// === EXAMPLE 2: LOOP COUNT IS A MULTIPLE OF 8 ===

int sumFirst(const int* data, int n)
{
    [[assume(n > 0)]];
    [[assume(n % 8 == 0)]];

    int sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += data[i];
    }
    return sum;
}

void example2()
{
    std::println("=== EXAMPLE 2: MULTIPLE OF 8 ===");

    int values[16];
    for (int i = 0; i < 16; ++i) {
        values[i] = i + 1;
    }
    std::println("sumFirst(values, 16) = {}", sumFirst(values, 16));
}

// === EXAMPLE 3: A WRONG PROMISE ===

void example3()
{
    std::println("=== EXAMPLE 3: A WRONG PROMISE ===");
    std::println("remainderOf8(-5) would be undefined behavior: the promise x >= 0 is false");
    std::println("(it is not called here)");
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
