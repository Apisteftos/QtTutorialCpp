// ============================================================
// [[no_unique_address]] - an empty member may take no space (C++20)
//
// Topics:
//   1. An empty member costs one byte plus padding
//   2. With the attribute the empty member shares an address
//   3. Real use: a stateless comparator or allocator inside a container
//   4. Two empty members of the same type must stay apart
//
// See Readme.md for the explanation.
// Build: g++ -std=c++23 -Wall -Wextra main.cpp -o no_unique_address
// ============================================================

#include <print>

struct Empty {};

// === EXAMPLE 1: WITHOUT THE ATTRIBUTE ===

struct Plain {
    int*  data;
    Empty tag;      // needs its own address: 1 byte + 7 bytes padding
};

// === EXAMPLE 2: WITH THE ATTRIBUTE ===

struct Compact {
    int*                          data;
    [[no_unique_address]] Empty   tag;    // may overlap with 'data'
};

void example12()
{
    std::println("=== EXAMPLES 1 AND 2: SIZES ===");
    std::println("sizeof(Empty)   = {}", sizeof(Empty));
    std::println("sizeof(Plain)   = {}", sizeof(Plain));
    std::println("sizeof(Compact) = {}", sizeof(Compact));
}

// === EXAMPLE 3: STATELESS POLICY IN A CONTAINER ===

struct Less {
    bool operator()(int a, int b) const { return a < b; }
};

template <typename Compare>
class SortedPair {
public:
    SortedPair(int a, int b) : first_(a), second_(b)
    {
        if (compare_(second_, first_)) {
            int tmp = first_;
            first_  = second_;
            second_ = tmp;
        }
    }

    int first() const  { return first_; }
    int second() const { return second_; }

private:
    int                             first_;
    int                             second_;
    [[no_unique_address]] Compare   compare_;   // stateless: costs nothing
};

void example3()
{
    std::println("=== EXAMPLE 3: STATELESS POLICY ===");

    SortedPair<Less> p(9, 4);
    std::println("sorted: {} {}", p.first(), p.second());
    std::println("sizeof(SortedPair<Less>) = {} (two ints = {})",
                 sizeof(SortedPair<Less>), 2 * sizeof(int));
}

// === EXAMPLE 4: TWO EMPTY MEMBERS OF THE SAME TYPE ===

// Two different objects of the same type must have different addresses,
// so the compiler cannot overlap these two.
struct SameType {
    [[no_unique_address]] Empty a;
    [[no_unique_address]] Empty b;
};

struct Tag1 {};
struct Tag2 {};

struct DifferentTypes {
    [[no_unique_address]] Tag1 a;
    [[no_unique_address]] Tag2 b;
};

void example4()
{
    std::println("=== EXAMPLE 4: SAME TYPE vs DIFFERENT TYPES ===");
    std::println("sizeof(SameType)       = {}", sizeof(SameType));
    std::println("sizeof(DifferentTypes) = {}", sizeof(DifferentTypes));
}

int main()
{
    example12();
    std::println("");
    example3();
    std::println("");
    example4();
    return 0;
}
