#include <cstddef>
#include <print>
#include <string>
#include <type_traits>

// Simple struct for pointer examples
struct Player {
    std::string name;
    int         score;
};

// Function that may return nullptr (simulates a search)
Player* findPlayer(Player* players, int count, const std::string& name)
{
    for (int i = 0; i < count; ++i) {
        if (players[i].name == name)
            return &players[i];
    }
    return nullptr;   // not found
}

// Overloads to demonstrate the difference between 0, NULL and nullptr
void process(int x)  { std::println("process(int): {}", x); }
void process(int* p) { std::println("process(int*): ptr = {}", static_cast<const void*>(p)); }

int main()
{
    // =================================================
    // WHAT IS nullptr
    // =================================================
    std::println("=====================================================");
    std::println("Example 1: nullptr — declaration");
    std::println("=====================================================");

    int* p1 = nullptr;          // modern C++ — preferred
    int* p2 = 0;                // old C style — avoid
    // int* p3 = NULL;          // C macro — avoid in C++

    std::println("p1 (nullptr): {}", static_cast<const void*>(p1));  // prints 0x0
    std::println("p2 (0):       {}", static_cast<const void*>(p2));  // prints 0x0

    std::println("p1 == nullptr: {}", p1 == nullptr);
    std::println("p2 == nullptr: {}", p2 == nullptr);


    // =================================================
    // WHY nullptr IS SAFER THAN NULL or 0
    // =================================================
    std::println("");
    std::println("=====================================================");
    std::println("Example 2: nullptr vs NULL vs 0");
    std::println("=====================================================");

    // 0 and NULL are integers — they can cause ambiguous overload resolution
    // nullptr is a distinct type (std::nullptr_t) — always unambiguous
    // (the overloads process(int) and process(int*) are defined above main)

    process(0);             // calls process(int) — 0 is an int
    process(nullptr);       // calls process(int*) — always the pointer version
    // process(NULL);       // ❌ Error with GCC: call of overloaded 'process(NULL)' is ambiguous

    std::println("nullptr type is std::nullptr_t — distinct from int");
    std::println("is_same_v<decltype(nullptr), std::nullptr_t>: {}", std::is_same_v<decltype(nullptr), std::nullptr_t>);


    // =================================================
    // NULLPTR CHECK — always before dereferencing
    // =================================================
    std::println("");
    std::println("=====================================================");
    std::println("Example 3: Always Check Before Dereferencing");
    std::println("=====================================================");

    int* safePtr = nullptr;

    // BAD — crashes if safePtr is nullptr
    // std::println("{}", *safePtr);   // undefined behavior (typically a crash)

    // GOOD — always check first
    if (safePtr != nullptr) {
        std::println("Value: {}", *safePtr);
    } else {
        std::println("safePtr is null — skipping dereference");
    }

    // Shorter form — pointer in boolean context
    if (safePtr) {
        std::println("Value: {}", *safePtr);
    } else {
        std::println("safePtr is null (bool check)");
    }

    // Now assign and check again
    int x = 42;
    safePtr = &x;
    if (safePtr) {
        std::println("safePtr now valid — value: {}", *safePtr);
    }


    // =================================================
    // NULLPTR IN FUNCTION RETURN (real-world pattern)
    // =================================================
    std::println("");
    std::println("=====================================================");
    std::println("Example 4: nullptr as 'not found' return value");
    std::println("=====================================================");

    Player players[3] = {
        {"Kostas", 1500},
        {"Andre",  800},
        {"Maria",  2200}
    };

    // Search for existing player
    Player* found = findPlayer(players, 3, "Kostas");
    if (found != nullptr) {
        std::println("Found: {} | score: {}", found->name, found->score);
    } else {
        std::println("Player not found");
    }

    // Search for non-existing player
    Player* notFound = findPlayer(players, 3, "Unknown");
    if (notFound != nullptr) {
        std::println("Found: {}", notFound->name);
    } else {
        std::println("Player 'Unknown' not found — got nullptr");
    }


    // =================================================
    // DANGLING POINTER — set to nullptr after delete
    // =================================================
    std::println("");
    std::println("=====================================================");
    std::println("Example 5: Dangling Pointer — Always nullptr After delete");
    std::println("=====================================================");

    int* pDynamic = new int(100);
    std::println("Before delete: *pDynamic = {}", *pDynamic);

    delete pDynamic;
    // pDynamic now points to freed memory — DANGLING POINTER
    // *pDynamic = 99;   // undefined behavior (typically a crash or silent corruption)

    pDynamic = nullptr;   // safe — now it's a null pointer, not dangling
    std::println("After delete + nullptr: pDynamic = {}", static_cast<const void*>(pDynamic));

    if (pDynamic) {
        std::println("Value: {}", *pDynamic);
    } else {
        std::println("pDynamic is null — safe, no dereference");
    }

    delete pDynamic;   // deleting a null pointer is allowed and does nothing
    std::println("delete on a null pointer: no effect, no crash");


    // =================================================
    // std::nullptr_t — the actual type of nullptr
    // =================================================
    std::println("");
    std::println("=====================================================");
    std::println("Example 6: std::nullptr_t");
    std::println("=====================================================");

    std::nullptr_t nullValue = nullptr;
    std::println("nullptr_t value == nullptr: {}", (nullValue == nullptr));

    // Can be assigned to any pointer type
    int*    pi  = nullValue;
    double* pd  = nullValue;
    char*   pc  = nullValue;

    std::println("int*    = {}", static_cast<const void*>(pi));
    std::println("double* = {}", static_cast<const void*>(pd));
    std::println("char*   = {}", static_cast<const void*>(pc));

    return 0;
}
