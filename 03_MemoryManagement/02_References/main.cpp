#include <iostream>
#include <string>
#include <vector>

// =============================================================
// Helper structs used across examples
// =============================================================
struct Player {
    std::string name;
    int         score;
    int         level;
};

// =============================================================
// Functions to demonstrate pass by value vs reference
// =============================================================

// Pass by VALUE — receives a COPY, original unchanged
void levelUpByValue(Player player)
{
    player.level++;
    player.score += 100;
    std::cout << "[byValue]     inside: " << player.name
              << " level=" << player.level << " score=" << player.score << '\n';
}

// Pass by REFERENCE — receives the original, can modify it
void levelUpByRef(Player& player)
{
    player.level++;
    player.score += 100;
    std::cout << "[byRef]       inside: " << player.name
              << " level=" << player.level << " score=" << player.score << '\n';
}

// Pass by CONST REFERENCE — read-only access, no copy
void printPlayer(const Player& player)
{
    std::cout << "[constRef]    " << player.name
              << " level=" << player.level
              << " score=" << player.score << '\n';
    // player.level++;   // ERROR — const reference, cannot modify
}

// Return by reference — returns reference to existing object
// WARNING: never return reference to local variable!
std::string& getFirstName(std::vector<std::string>& names)
{
    return names[0];   // reference to element inside the vector
}

// Swap using references — classic example
void swapValues(int& a, int& b)
{
    int temp = a;
    a = b;
    b = temp;
}

// Large struct — shows why const ref matters for performance
struct LargeData {
    std::string data[100];   // 100 strings — expensive to copy

    LargeData() {
        for (int i = 0; i < 100; ++i)
            data[i] = "entry_" + std::to_string(i);
    }
};

// WRONG — copies entire LargeData (expensive)
void processDataByValue(LargeData ld)
{
    std::cout << "[byValue]  first entry: " << ld.data[0] << '\n';
}

// CORRECT — no copy, just a reference (cheap regardless of size)
void processDataByConstRef(const LargeData& ld)
{
    std::cout << "[constRef] first entry: " << ld.data[0] << '\n';
}


int main()
{
    // =================================================
    // BASIC REFERENCE — alias for existing variable
    // =================================================
    std::cout << "=====================================================\n";
    std::cout << "Example 1: Basic Reference\n";
    std::cout << "=====================================================\n";

    int original = 42;
    int& ref     = original;   // ref is an alias — same memory location

    std::cout << "original = " << original << '\n';
    std::cout << "ref      = " << ref      << '\n';
    std::cout << "Same address? "
              << (&original == &ref ? "YES — same memory" : "NO") << '\n';

    // Modifying through reference modifies the original
    ref = 100;
    std::cout << "\nAfter ref = 100:\n";
    std::cout << "original = " << original << '\n';
    std::cout << "ref      = " << ref      << '\n';

    // Modifying original also visible through reference
    original = 999;
    std::cout << "\nAfter original = 999:\n";
    std::cout << "ref      = " << ref << '\n';


    // =================================================
    // REFERENCE RULES
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: Reference Rules\n";
    std::cout << "=====================================================\n";

    int a = 10, b = 20;
    int& refA = a;   // refA refers to a

    std::cout << "refA = " << refA << " (refers to a=" << a << ")\n";

    // References cannot be reseated — this ASSIGNS b's value to a, not repoints refA
    refA = b;
    std::cout << "\nAfter refA = b:\n";
    std::cout << "a    = " << a    << " (changed to b's value)\n";
    std::cout << "b    = " << b    << '\n';
    std::cout << "refA = " << refA << " (still refers to a, not b)\n";
    std::cout << "&refA == &a? " << (&refA == &a ? "YES" : "NO") << '\n';
    std::cout << "&refA == &b? " << (&refA == &b ? "YES" : "NO") << '\n';

    // Must be initialized at declaration
    // int& badRef;   // ERROR — reference must be initialized


    // =================================================
    // PASS BY VALUE vs PASS BY REFERENCE
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: Pass by Value vs Reference\n";
    std::cout << "=====================================================\n";

    Player kostas{"Kostas", 500, 5};
    printPlayer(kostas);

    std::cout << "\nCalling levelUpByValue:\n";
    levelUpByValue(kostas);
    std::cout << "[main]        after:  " << kostas.name
              << " level=" << kostas.level << " score=" << kostas.score
              << " (UNCHANGED)\n";

    std::cout << "\nCalling levelUpByRef:\n";
    levelUpByRef(kostas);
    std::cout << "[main]        after:  " << kostas.name
              << " level=" << kostas.level << " score=" << kostas.score
              << " (CHANGED)\n";


    // =================================================
    // CONST REFERENCE — read-only, no copy
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: const Reference — Performance\n";
    std::cout << "=====================================================\n";

    LargeData bigData;

    std::cout << "Calling processDataByValue (copies 100 strings):\n";
    processDataByValue(bigData);

    std::cout << "Calling processDataByConstRef (zero copy):\n";
    processDataByConstRef(bigData);


    // =================================================
    // REFERENCE TO STRUCT MEMBERS
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: Reference to Struct Member\n";
    std::cout << "=====================================================\n";

    Player andre{"Andre", 800, 8};

    // Reference to a member — avoids repeated struct access
    int& scoreRef = andre.score;
    std::cout << "scoreRef = " << scoreRef << '\n';

    scoreRef += 500;   // modifies andre.score directly
    std::cout << "After scoreRef += 500: andre.score = " << andre.score << '\n';


    // =================================================
    // SWAP WITH REFERENCES
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: Swap Using References\n";
    std::cout << "=====================================================\n";

    int x = 10, y = 20;
    std::cout << "Before swap: x=" << x << " y=" << y << '\n';
    swapValues(x, y);
    std::cout << "After swap:  x=" << x << " y=" << y << '\n';


    // =================================================
    // RETURN BY REFERENCE
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: Return by Reference\n";
    std::cout << "=====================================================\n";

    std::vector<std::string> names = {"Kostas", "Andre", "Maria"};
    std::cout << "Before: names[0] = " << names[0] << '\n';

    // Get reference to first element and modify it
    std::string& first = getFirstName(names);
    first = "Konstantinos";   // modifies names[0] directly

    std::cout << "After:  names[0] = " << names[0] << '\n';


    // =================================================
    // RANGE-BASED FOR WITH REFERENCES
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: Range-based for — ref vs copy\n";
    std::cout << "=====================================================\n";

    std::vector<int> scores = {10, 20, 30, 40, 50};

    // By VALUE — copies each element, original unchanged
    std::cout << "Loop by value (no change):\n";
    for (int s : scores)
        s *= 2;   // modifies copy only

    std::cout << "scores: ";
    for (int s : scores) std::cout << s << " ";
    std::cout << '\n';

    // By REFERENCE — modifies original
    std::cout << "\nLoop by reference (doubles each):\n";
    for (int& s : scores)
        s *= 2;   // modifies original

    std::cout << "scores: ";
    for (int s : scores) std::cout << s << " ";
    std::cout << '\n';

    // By CONST REFERENCE — read only, no copy (best for complex types)
    std::cout << "\nLoop by const ref (read only):\n";
    for (const int& s : scores)
        std::cout << s << " ";
    std::cout << '\n';


    // =================================================
    // RVALUE REFERENCE (C++11) — move semantics intro
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 9: lvalue vs rvalue Reference\n";
    std::cout << "=====================================================\n";

    int val = 5;

    int& lref  = val;      // lvalue reference — binds to named variable
    // int& bad = 5;        // ERROR — cannot bind lvalue ref to rvalue (literal)

    int&& rref = 10;       // rvalue reference — binds to temporary/literal
    int&& rref2 = val * 2; // rvalue reference — binds to expression result

    std::cout << "lref  = " << lref  << '\n';
    std::cout << "rref  = " << rref  << '\n';
    std::cout << "rref2 = " << rref2 << '\n';

    // const lvalue ref can bind to rvalue (special rule)
    const int& constRef = 42;   // OK — extends lifetime of temporary
    std::cout << "const int& bound to 42: " << constRef << '\n';

    return 0;
}
