#include <iostream>
#include <bitset>   // for pretty binary printing

int main()
{
    // =================================================
    // ARITHMETIC OPERATORS
    // =================================================
    std::cout << "=====================================================\n";
    std::cout << "Example 1: Arithmetic Operators\n";
    std::cout << "=====================================================\n";

    int a = 20, b = 6;

    std::cout << "a = " << a << ", b = " << b << '\n';
    std::cout << "a + b  = " << (a + b)  << '\n';   // addition
    std::cout << "a - b  = " << (a - b)  << '\n';   // subtraction
    std::cout << "a * b  = " << (a * b)  << '\n';   // multiplication
    std::cout << "a / b  = " << (a / b)  << '\n';   // integer division — truncates
    std::cout << "a % b  = " << (a % b)  << '\n';   // modulo — remainder

    // Floating point division
    double fa = 20.0, fb = 6.0;
    std::cout << "20.0 / 6.0 = " << (fa / fb) << '\n';   // 3.3333...

    // Integer division pitfall
    std::cout << "7 / 2   = " << (7 / 2)   << '\n';  // 3 — truncated!
    std::cout << "7.0 / 2 = " << (7.0 / 2) << '\n';  // 3.5 — correct


    // =================================================
    // INCREMENT & DECREMENT
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: Increment & Decrement\n";
    std::cout << "=====================================================\n";

    int x = 5;

    // Prefix — increment first, then use value
    std::cout << "x = "   << x    << '\n';
    std::cout << "++x = " << ++x  << '\n';   // x becomes 6, prints 6
    std::cout << "x = "   << x    << '\n';   // x is still 6

    // Postfix — use value first, then increment
    std::cout << "x++ = " << x++  << '\n';   // prints 6, then x becomes 7
    std::cout << "x = "   << x    << '\n';   // x is now 7

    // Decrement
    std::cout << "--x = " << --x  << '\n';   // x becomes 6, prints 6
    std::cout << "x-- = " << x--  << '\n';   // prints 6, then x becomes 5
    std::cout << "x = "   << x    << '\n';   // x is now 5


    // =================================================
    // ASSIGNMENT OPERATORS
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: Assignment Operators\n";
    std::cout << "=====================================================\n";

    int n = 10;
    std::cout << "n = " << n << '\n';

    n += 5;   std::cout << "n += 5  → " << n << '\n';   // n = n + 5  = 15
    n -= 3;   std::cout << "n -= 3  → " << n << '\n';   // n = n - 3  = 12
    n *= 2;   std::cout << "n *= 2  → " << n << '\n';   // n = n * 2  = 24
    n /= 4;   std::cout << "n /= 4  → " << n << '\n';   // n = n / 4  = 6
    n %= 4;   std::cout << "n %= 4  → " << n << '\n';   // n = n % 4  = 2

    // Bitwise assignment
    n = 12;
    std::cout << "\nn = " << n << " (binary: " << std::bitset<8>(n) << ")\n";
    n &= 10;  std::cout << "n &= 10 → " << n << " (binary: " << std::bitset<8>(n) << ")\n";
    n = 12;
    n |= 3;   std::cout << "n |= 3  → " << n << " (binary: " << std::bitset<8>(n) << ")\n";
    n = 12;
    n ^= 10;  std::cout << "n ^= 10 → " << n << " (binary: " << std::bitset<8>(n) << ")\n";
    n = 12;
    n <<= 2;  std::cout << "n <<= 2 → " << n << " (binary: " << std::bitset<8>(n) << ")\n";
    n = 12;
    n >>= 2;  std::cout << "n >>= 2 → " << n << " (binary: " << std::bitset<8>(n) << ")\n";


    // =================================================
    // COMPARISON OPERATORS
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: Comparison Operators\n";
    std::cout << "=====================================================\n";

    int p = 10, q = 20;
    std::cout << "p = " << p << ", q = " << q << '\n';
    std::cout << "p == q : " << (p == q) << '\n';   // equal
    std::cout << "p != q : " << (p != q) << '\n';   // not equal
    std::cout << "p <  q : " << (p <  q) << '\n';   // less than
    std::cout << "p >  q : " << (p >  q) << '\n';   // greater than
    std::cout << "p <= q : " << (p <= q) << '\n';   // less or equal
    std::cout << "p >= q : " << (p >= q) << '\n';   // greater or equal

    // C++20 three-way comparison (spaceship operator)
    auto result = p <=> q;
    if (result < 0)       std::cout << "p <=> q : p is less than q\n";
    else if (result > 0)  std::cout << "p <=> q : p is greater than q\n";
    else                  std::cout << "p <=> q : p equals q\n";


    // =================================================
    // LOGICAL OPERATORS
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: Logical Operators\n";
    std::cout << "=====================================================\n";

    bool isAdult  = true;
    bool hasID    = false;
    bool isMember = true;

    std::cout << "isAdult  = " << isAdult  << '\n';
    std::cout << "hasID    = " << hasID    << '\n';
    std::cout << "isMember = " << isMember << '\n';

    // AND — both must be true
    std::cout << "\nisAdult && hasID    = " << (isAdult && hasID)    << '\n';
    std::cout << "isAdult && isMember = " << (isAdult && isMember) << '\n';

    // OR — at least one must be true
    std::cout << "\nisAdult || hasID    = " << (isAdult || hasID)    << '\n';
    std::cout << "hasID   || isMember = " << (hasID   || isMember) << '\n';

    // NOT — inverts the value
    std::cout << "\n!isAdult = " << !isAdult << '\n';
    std::cout << "!hasID   = " << !hasID   << '\n';

    // Practical example — entry check
    bool canEnter = isAdult && (hasID || isMember);
    std::cout << "\ncanEnter (isAdult && (hasID || isMember)) = "
              << (canEnter ? "YES" : "NO") << '\n';

    // Short-circuit evaluation
    int counter = 0;
    bool shortCircuit = false && (++counter > 0);   // counter never incremented
    std::cout << "\nShort-circuit AND: counter = " << counter << " (never evaluated)\n";
    shortCircuit = true || (++counter > 0);          // counter never incremented
    std::cout << "Short-circuit OR:  counter = " << counter << " (never evaluated)\n";


    // =================================================
    // BITWISE OPERATORS
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: Bitwise Operators\n";
    std::cout << "=====================================================\n";

    unsigned int u = 0b00001100;   // 12
    unsigned int v = 0b00001010;   // 10

    std::cout << "u = " << u << " (binary: " << std::bitset<8>(u) << ")\n";
    std::cout << "v = " << v << " (binary: " << std::bitset<8>(v) << ")\n";

    // AND — bit is 1 only if BOTH bits are 1
    std::cout << "\nu & v  = " << (u & v)
              << " (binary: " << std::bitset<8>(u & v) << ")\n";

    // OR — bit is 1 if EITHER bit is 1
    std::cout << "u | v  = " << (u | v)
              << " (binary: " << std::bitset<8>(u | v) << ")\n";

    // XOR — bit is 1 if bits are DIFFERENT
    std::cout << "u ^ v  = " << (u ^ v)
              << " (binary: " << std::bitset<8>(u ^ v) << ")\n";

    // NOT — flips all bits
    std::cout << "~u     = " << (uint8_t)(~u)
              << " (binary: " << std::bitset<8>((uint8_t)~u) << ")\n";

    // Left shift — multiply by 2 per shift
    std::cout << "\nu << 1 = " << (u << 1)
              << " (binary: " << std::bitset<8>(u << 1) << ") → u * 2\n";
    std::cout << "u << 2 = " << (u << 2)
              << " (binary: " << std::bitset<8>(u << 2) << ") → u * 4\n";

    // Right shift — divide by 2 per shift
    std::cout << "u >> 1 = " << (u >> 1)
              << " (binary: " << std::bitset<8>(u >> 1) << ") → u / 2\n";
    std::cout << "u >> 2 = " << (u >> 2)
              << " (binary: " << std::bitset<8>(u >> 2) << ") → u / 4\n";

    // Practical: bit flags (common in Qt and embedded/MCX work)
    std::cout << "\n--- Bit Flags Example ---\n";
    constexpr uint8_t FLAG_READ    = 0b00000001;   // bit 0
    constexpr uint8_t FLAG_WRITE   = 0b00000010;   // bit 1
    constexpr uint8_t FLAG_EXECUTE = 0b00000100;   // bit 2

    uint8_t permissions = 0;
    permissions |= FLAG_READ;                      // set READ
    permissions |= FLAG_WRITE;                     // set WRITE
    std::cout << "permissions: " << std::bitset<8>(permissions) << '\n';

    bool canRead    = permissions & FLAG_READ;
    bool canWrite   = permissions & FLAG_WRITE;
    bool canExecute = permissions & FLAG_EXECUTE;

    std::cout << "canRead:    " << (canRead    ? "YES" : "NO") << '\n';
    std::cout << "canWrite:   " << (canWrite   ? "YES" : "NO") << '\n';
    std::cout << "canExecute: " << (canExecute ? "YES" : "NO") << '\n';

    permissions &= ~FLAG_WRITE;                    // clear WRITE bit
    std::cout << "After clearing WRITE: " << std::bitset<8>(permissions) << '\n';

    return 0;
}
