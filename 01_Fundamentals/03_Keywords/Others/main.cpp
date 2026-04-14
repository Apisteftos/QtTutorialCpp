#include <iostream>
#include <typeinfo>

// ============================================================
// C++ KEYWORDS DEMO: asm, operator, enum, union, inline, export
// (All in one big educational example with explanations)
// ============================================================
//
// NOTE:
// - asm is compiler-specific (mostly GCC/Clang style, not portable)
// - export (for templates) is mostly obsolete in modern C++
// ============================================================

// ============================================================
// 1. asm (Inline Assembly - compiler dependent)
// ============================================================
//
// Used to embed assembly instructions directly into C++ code.
// Mostly used in low-level systems programming.
//
// WARNING: Not portable, and often disabled in modern builds.

int add_with_asm(int a, int b)
{
    int result;

    asm (
        "addl %%ebx, %%eax;"   // eax = eax + ebx (x86 example)
        : "=a"(result)         // output
        : "a"(a), "b"(b)       // inputs
        );

    return result;
}

// ============================================================
// 2. enum (Enumeration types)
// ============================================================
//
// Used to define named constants.

enum Color
{
    RED,
    GREEN,
    BLUE
};

// Strongly scoped enum (better practice in modern C++)
enum class Direction
{
    North,
    South,
    East,
    West
};

// ============================================================
// 3. union
// ============================================================
//
// A union shares memory between all members.
// Only ONE member is valid at a time.

union Data
{
    int i;
    float f;
    char c;
};

// ============================================================
// 4. inline
// ============================================================
//
// Suggests compiler to replace function call with function body.
// Useful for small functions.

inline int multiply(int x, int y)
{
    return x * y;
}

// Inline variables (C++17+)
inline int globalInlineValue = 100;

// ============================================================
// 5. operator (Operator overloading)
// ============================================================
//
// Allows custom behavior for operators in classes.

class Vector2
{
public:
    int x, y;

    Vector2(int x, int y) : x(x), y(y) {}

    // Overload + operator
    Vector2 operator+(const Vector2& other)
    {
        return Vector2(x + other.x, y + other.y);
    }

    // Overload == operator
    bool operator==(const Vector2& other)
    {
        return x == other.x && y == other.y;
    }
};

// ============================================================
// 6. export (Template export - mostly obsolete)
// ============================================================
//
// Historically used to separate template implementation
// across compilation units.
//
// Most compilers IGNORE it today.

export template <typename T>
T legacyAdd(T a, T b)
{
    return a + b;
}

// ============================================================
// MAIN DEMONSTRATION
// ============================================================

int main()
{
    // ---------------- asm ----------------
    int a = 5, b = 7;
    std::cout << "asm add result (5 + 7) = " << add_with_asm(a, b) << "\n\n";

    // ---------------- enum ----------------
    Color c = GREEN;
    Direction d = Direction::East;

    std::cout << "enum Color GREEN = " << c << "\n";
    std::cout << "enum class Direction::East = " << static_cast<int>(d) << "\n\n";

    // ---------------- union ----------------
    Data data;
    data.i = 42;

    std::cout << "union as int = " << data.i << "\n";

    data.f = 3.14f; // overwrites memory
    std::cout << "union as float = " << data.f << "\n";
    std::cout << "warning: previous int value is now overwritten\n\n";

    // ---------------- inline ----------------
    std::cout << "inline multiply(3,4) = " << multiply(3,4) << "\n";
    std::cout << "inline variable = " << globalInlineValue << "\n\n";

    // ---------------- operator overloading ----------------
    Vector2 v1(2, 3);
    Vector2 v2(4, 5);

    Vector2 v3 = v1 + v2;

    std::cout << "Vector2 operator+ result: ("
              << v3.x << ", " << v3.y << ")\n";

    std::cout << "Vector2 operator== result: "
              << (v1 == v2) << "\n\n";

    // ---------------- export ----------------
    std::cout << "export template legacyAdd(2,3) = "
              << legacyAdd(2, 3) << "\n";

    return 0;
}
