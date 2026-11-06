// ============================================================
// [[likely]] / [[unlikely]] - hint which branch is usual (C++20)
//
// Topics:
//   1. In an if / else
//   2. On switch case labels
//   3. In a loop (error path)
//
// The attributes do not change what the program computes, only how the
// compiler arranges the code. See Readme.md.
// Build: g++ -std=c++23 -Wall -Wextra main.cpp -o likely
// ============================================================

#include <print>
#include <vector>

// === EXAMPLE 1: IF / ELSE ===

int parseDigit(char c)
{
    if (c >= '0' && c <= '9') [[likely]] {
        return c - '0';
    } else [[unlikely]] {
        return -1;
    }
}

void example1()
{
    std::println("=== EXAMPLE 1: IF / ELSE ===");
    std::println("parseDigit('7') = {}", parseDigit('7'));
    std::println("parseDigit('x') = {}", parseDigit('x'));
}

// === EXAMPLE 2: SWITCH CASE LABELS ===

enum class Packet { Data, Ack, Reset };

int cost(Packet p)
{
    switch (p) {
    case Packet::Data:
        [[likely]] return 1;
    case Packet::Ack:
        return 2;
    case Packet::Reset:
        [[unlikely]] return 100;
    }
    return 0;
}

void example2()
{
    std::println("=== EXAMPLE 2: SWITCH ===");
    std::println("cost(Data)  = {}", cost(Packet::Data));
    std::println("cost(Ack)   = {}", cost(Packet::Ack));
    std::println("cost(Reset) = {}", cost(Packet::Reset));
}

// === EXAMPLE 3: ERROR PATH IN A LOOP ===

int sumValid(const std::vector<int>& values)
{
    int sum = 0;
    for (int v : values) {
        if (v < 0) [[unlikely]] {
            continue;              // rare: skip invalid entries
        }
        sum += v;
    }
    return sum;
}

void example3()
{
    std::println("=== EXAMPLE 3: ERROR PATH IN A LOOP ===");
    std::println("sumValid = {}", sumValid({1, 2, -3, 4, 5}));
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
