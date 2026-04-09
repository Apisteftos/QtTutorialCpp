#include <iostream>
#include <string>

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

int main()
{
    // =================================================
    // WHAT IS nullptr
    // =================================================
    std::cout << "=====================================================\n";
    std::cout << "Example 1: nullptr — declaration\n";
    std::cout << "=====================================================\n";

    int* p1 = nullptr;          // modern C++ — preferred
    int* p2 = 0;                // old C style — avoid
    // int* p3 = NULL;          // C macro — avoid in C++

    std::cout << "p1 (nullptr): " << p1 << '\n';   // prints 0
    std::cout << "p2 (0):       " << p2 << '\n';   // prints 0

    std::cout << "p1 == nullptr: " << (p1 == nullptr ? "true" : "false") << '\n';
    std::cout << "p2 == nullptr: " << (p2 == nullptr ? "true" : "false") << '\n';


    // =================================================
    // WHY nullptr IS SAFER THAN NULL or 0
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: nullptr vs NULL vs 0\n";
    std::cout << "=====================================================\n";

    // NULL and 0 are integers — they can cause ambiguous overload resolution
    // nullptr is a distinct type (std::nullptr_t) — always unambiguous

    // Overloaded functions to demonstrate
    auto process = [](int x)   { std::cout << "process(int): "     << x << '\n'; };
    auto processPtr = [](int* p) { std::cout << "process(int*): ptr = " << p << '\n'; };

    process(0);             // calls int version — clear
    processPtr(nullptr);    // calls int* version — clear
    // processPtr(0);       // would call int version — wrong! 0 is an int, not a pointer
    // processPtr(NULL);    // same ambiguity as 0 on most platforms

    std::cout << "nullptr type is std::nullptr_t — distinct from int\n";


    // =================================================
    // NULLPTR CHECK — always before dereferencing
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: Always Check Before Dereferencing\n";
    std::cout << "=====================================================\n";

    int* safePtr = nullptr;

    // BAD — crashes if safePtr is nullptr
    // std::cout << *safePtr;   // CRASH — undefined behavior

    // GOOD — always check first
    if (safePtr != nullptr) {
        std::cout << "Value: " << *safePtr << '\n';
    } else {
        std::cout << "safePtr is null — skipping dereference\n";
    }

    // Shorter form — pointer in boolean context
    if (safePtr) {
        std::cout << "Value: " << *safePtr << '\n';
    } else {
        std::cout << "safePtr is null (bool check)\n";
    }

    // Now assign and check again
    int x = 42;
    safePtr = &x;
    if (safePtr) {
        std::cout << "safePtr now valid — value: " << *safePtr << '\n';
    }


    // =================================================
    // NULLPTR IN FUNCTION RETURN (real-world pattern)
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: nullptr as 'not found' return value\n";
    std::cout << "=====================================================\n";

    Player players[3] = {
        {"Kostas", 1500},
        {"Andre",  800},
        {"Maria",  2200}
    };

    // Search for existing player
    Player* found = findPlayer(players, 3, "Kostas");
    if (found != nullptr) {
        std::cout << "Found: " << found->name
                  << " | score: " << found->score << '\n';
    } else {
        std::cout << "Player not found\n";
    }

    // Search for non-existing player
    Player* notFound = findPlayer(players, 3, "Unknown");
    if (notFound != nullptr) {
        std::cout << "Found: " << notFound->name << '\n';
    } else {
        std::cout << "Player 'Unknown' not found — got nullptr\n";
    }


    // =================================================
    // DANGLING POINTER — set to nullptr after delete
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: Dangling Pointer — Always nullptr After delete\n";
    std::cout << "=====================================================\n";

    int* pDynamic = new int(100);
    std::cout << "Before delete: *pDynamic = " << *pDynamic << '\n';

    delete pDynamic;
    // pDynamic now points to freed memory — DANGLING POINTER
    // *pDynamic = 99;   // CRASH / undefined behavior

    pDynamic = nullptr;   // safe — now it's a null pointer, not dangling
    std::cout << "After delete + nullptr: pDynamic = " << pDynamic << '\n';

    if (pDynamic) {
        std::cout << "Value: " << *pDynamic << '\n';
    } else {
        std::cout << "pDynamic is null — safe, no dereference\n";
    }


    // =================================================
    // std::nullptr_t — the actual type of nullptr
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: std::nullptr_t\n";
    std::cout << "=====================================================\n";

    std::nullptr_t nullValue = nullptr;
    std::cout << "nullptr_t value == nullptr: "
              << (nullValue == nullptr ? "true" : "false") << '\n';

    // Can be assigned to any pointer type
    int*    pi  = nullValue;
    double* pd  = nullValue;
    char*   pc  = nullValue;

    std::cout << "int*    = " << pi << '\n';
    std::cout << "double* = " << pd << '\n';
    std::cout << "char*   = " << (void*)pc << '\n';

    return 0;
}
