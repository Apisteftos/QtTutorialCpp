#include <iostream>
#include <bitset>

// ============================================================
// C++ OPERATOR KEYWORDS (Alternative tokens + bitwise/logical)
// ============================================================
//
// These are keyword alternatives for operators in C++.
// They behave exactly like their symbolic versions.
//
// ------------------------------------------------------------
// LOGICAL OPERATORS (used in conditions)
// ------------------------------------------------------------
// and     -> &&   (logical AND)
// or      -> ||   (logical OR)
// not     -> !    (logical NOT)
// not_eq  -> !=   (not equal comparison)
//
// ------------------------------------------------------------
// BITWISE OPERATORS (work on bits)
// ------------------------------------------------------------
// bitand  -> &    (bitwise AND)
// bitor   -> |    (bitwise OR)
// xor     -> ^    (bitwise XOR)
// compl   -> ~    (bitwise complement / NOT)
//
// ------------------------------------------------------------
// ASSIGNMENT + BITWISE COMBINATIONS
// ------------------------------------------------------------
// and_eq  -> &=
// or_eq   -> |=
// xor_eq  -> ^=
//
// ============================================================

int main()
{
    int a = 12;   // 1100 in binary
    int b = 10;   // 1010 in binary

    std::cout << "a = " << a << "  b = " << b << "\n\n";

    // ========================================================
    // LOGICAL OPERATORS (and / or / not / not_eq)
    // ========================================================

    bool x = true;
    bool y = false;

    std::cout << "Logical Operators:\n";
    std::cout << "x and y  = " << (x and y) << "   (true AND false)\n";
    std::cout << "x or y   = " << (x or y)  << "   (true OR false)\n";
    std::cout << "not x    = " << (not x)   << "   (NOT true)\n";
    std::cout << "x not_eq y = " << (x not_eq y) << " (true != false)\n\n";

    // ========================================================
    // BITWISE OPERATORS (bitand / bitor / xor / compl)
    // ========================================================

    std::cout << "Bitwise Operators:\n";

    std::cout << "a bitand b = " << (a bitand b)
              << "   binary: " << std::bitset<4>(a bitand b) << "\n";

    std::cout << "a bitor b  = " << (a bitor b)
              << "   binary: " << std::bitset<4>(a bitor b) << "\n";

    std::cout << "a xor b    = " << (a xor b)
              << "   binary: " << std::bitset<4>(a xor b) << "\n";

    std::cout << "compl a    = " << (compl a)
              << "   binary (only low bits shown): "
              << std::bitset<8>(compl a) << "\n\n";

    // ========================================================
    // COMPOUND ASSIGNMENTS (and_eq / or_eq / xor_eq)
    // ========================================================

    int c = a;  // 12

    std::cout << "Compound Assignment Operators:\n";

    c and_eq b;  // c &= b
    std::cout << "c and_eq b -> " << c << "   (c &= b)\n";

    c = a;
    c or_eq b;   // c |= b
    std::cout << "c or_eq b  -> " << c << "   (c |= b)\n";

    c = a;
    c xor_eq b;  // c ^= b
    std::cout << "c xor_eq b -> " << c << "   (c ^= b)\n";

    return 0;
}
