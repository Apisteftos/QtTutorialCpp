#include <print>
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
    std::println("[byValue]     inside: {} level={} score={}", player.name, player.level, player.score);
}

// Pass by REFERENCE — receives the original, can modify it
void levelUpByRef(Player& player)
{
    player.level++;
    player.score += 100;
    std::println("[byRef]       inside: {} level={} score={}", player.name, player.level, player.score);
}

// Pass by CONST REFERENCE — read-only access, no copy
void printPlayer(const Player& player)
{
    std::println("[constRef]    {} level={} score={}", player.name, player.level, player.score);
    // player.level++;   // ERROR — const reference, cannot modify
}

// Return by reference — returns reference to existing object
// Precondition: names is not empty (names[0] on an empty vector is undefined behavior)
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
    static inline int copyCount = 0;   // how often a LargeData has been copied

    LargeData() {
        for (int i = 0; i < 100; ++i)
            data[i] = "entry_" + std::to_string(i);
    }

    LargeData(const LargeData& other) {
        for (int i = 0; i < 100; ++i)
            data[i] = other.data[i];
        ++copyCount;
    }

    LargeData& operator=(const LargeData&) = default;
};

// WRONG — copies entire LargeData (expensive)
void processDataByValue(LargeData ld)
{
    std::println("[byValue]  first entry: {}", ld.data[0]);
}

// CORRECT — no copy, just a reference (cheap regardless of size)
void processDataByConstRef(const LargeData& ld)
{
    std::println("[constRef] first entry: {}", ld.data[0]);
}


int main()
{
    // =================================================
    // BASIC REFERENCE — alias for existing variable
    // =================================================
    std::println("=====================================================");
    std::println("Example 1: Basic Reference");
    std::println("=====================================================");

    int original = 42;
    int& ref     = original;   // ref is an alias — same memory location

    std::println("original = {}", original);
    std::println("ref      = {}", ref);
    std::println("Same address? {}", (&original == &ref ? "YES — same memory" : "NO"));

    // Modifying through reference modifies the original
    ref = 100;
    std::println("");
    std::println("After ref = 100:");
    std::println("original = {}", original);
    std::println("ref      = {}", ref);

    // Modifying original also visible through reference
    original = 999;
    std::println("");
    std::println("After original = 999:");
    std::println("ref      = {}", ref);


    // =================================================
    // REFERENCE RULES
    // =================================================
    std::println("");
    std::println("=====================================================");
    std::println("Example 2: Reference Rules");
    std::println("=====================================================");

    int a = 10, b = 20;
    int& refA = a;   // refA refers to a

    std::println("refA = {} (refers to a={})", refA, a);

    // References cannot be reseated — this ASSIGNS b's value to a, not repoints refA
    refA = b;
    std::println("");
    std::println("After refA = b:");
    std::println("a    = {} (changed to b's value)", a);
    std::println("b    = {}", b);
    std::println("refA = {} (still refers to a, not b)", refA);
    std::println("&refA == &a? {}", (&refA == &a ? "YES" : "NO"));
    std::println("&refA == &b? {}", (&refA == &b ? "YES" : "NO"));

    // Must be initialized at declaration
    // int& badRef;   // ERROR — reference must be initialized


    // =================================================
    // PASS BY VALUE vs PASS BY REFERENCE
    // =================================================
    std::println("");
    std::println("=====================================================");
    std::println("Example 3: Pass by Value vs Reference");
    std::println("=====================================================");

    Player kostas{"Kostas", 500, 5};
    printPlayer(kostas);

    std::println("");
    std::println("Calling levelUpByValue:");
    levelUpByValue(kostas);
    std::println("[main]        after:  {} level={} score={} (UNCHANGED)", kostas.name, kostas.level, kostas.score);

    std::println("");
    std::println("Calling levelUpByRef:");
    levelUpByRef(kostas);
    std::println("[main]        after:  {} level={} score={} (CHANGED)", kostas.name, kostas.level, kostas.score);


    // =================================================
    // CONST REFERENCE — read-only, no copy
    // =================================================
    std::println("");
    std::println("=====================================================");
    std::println("Example 4: const Reference — Performance");
    std::println("=====================================================");

    LargeData bigData;

    std::println("Calling processDataByValue (copies 100 strings):");
    processDataByValue(bigData);
    std::println("  LargeData copies so far: {}", LargeData::copyCount);

    std::println("Calling processDataByConstRef (zero copy):");
    processDataByConstRef(bigData);
    std::println("  LargeData copies so far: {}", LargeData::copyCount);


    // =================================================
    // REFERENCE TO STRUCT MEMBERS
    // =================================================
    std::println("");
    std::println("=====================================================");
    std::println("Example 5: Reference to Struct Member");
    std::println("=====================================================");

    Player andre{"Andre", 800, 8};

    // Reference to a member — avoids repeated struct access
    int& scoreRef = andre.score;
    std::println("scoreRef = {}", scoreRef);

    scoreRef += 500;   // modifies andre.score directly
    std::println("After scoreRef += 500: andre.score = {}", andre.score);


    // =================================================
    // SWAP WITH REFERENCES
    // =================================================
    std::println("");
    std::println("=====================================================");
    std::println("Example 6: Swap Using References");
    std::println("=====================================================");

    int x = 10, y = 20;
    std::println("Before swap: x={} y={}", x, y);
    swapValues(x, y);
    std::println("After swap:  x={} y={}", x, y);


    // =================================================
    // RETURN BY REFERENCE
    // =================================================
    std::println("");
    std::println("=====================================================");
    std::println("Example 7: Return by Reference");
    std::println("=====================================================");

    std::vector<std::string> names = {"Kostas", "Andre", "Maria"};
    std::println("Before: names[0] = {}", names[0]);

    // Get reference to first element and modify it
    std::string& first = getFirstName(names);
    first = "Konstantinos";   // modifies names[0] directly

    std::println("After:  names[0] = {}", names[0]);


    // =================================================
    // RANGE-BASED FOR WITH REFERENCES
    // =================================================
    std::println("");
    std::println("=====================================================");
    std::println("Example 8: Range-based for — ref vs copy");
    std::println("=====================================================");

    std::vector<int> scores = {10, 20, 30, 40, 50};

    // By VALUE — copies each element, original unchanged
    std::println("Loop by value (no change):");
    for ([[maybe_unused]] int s : scores)
        s *= 2;   // modifies copy only

    std::print("scores: ");
    for (int s : scores) std::print("{} ", s);
    std::println("");

    // By REFERENCE — modifies original
    std::println("");
    std::println("Loop by reference (doubles each):");
    for (int& s : scores)
        s *= 2;   // modifies original

    std::print("scores: ");
    for (int s : scores) std::print("{} ", s);
    std::println("");

    // By CONST REFERENCE — read only, no copy (best for complex types)
    std::println("");
    std::println("Loop by const ref (read only):");
    for (const int& s : scores)
        std::print("{} ", s);
    std::println("");


    // =================================================
    // RVALUE REFERENCE (C++11) — move semantics intro
    // =================================================
    std::println("");
    std::println("=====================================================");
    std::println("Example 9: lvalue vs rvalue Reference");
    std::println("=====================================================");

    int val = 5;

    int& lref  = val;      // lvalue reference — binds to named variable
    // int& bad = 5;        // ERROR — cannot bind lvalue ref to rvalue (literal)

    int&& rref = 10;       // rvalue reference — binds to temporary/literal
    int&& rref2 = val * 2; // rvalue reference — binds to expression result

    std::println("lref  = {}", lref);
    std::println("rref  = {}", rref);
    std::println("rref2 = {}", rref2);

    // const lvalue ref can bind to rvalue (special rule)
    const int& constRef = 42;   // OK — extends lifetime of temporary
    std::println("const int& bound to 42: {}", constRef);

    return 0;
}
