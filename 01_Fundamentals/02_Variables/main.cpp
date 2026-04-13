#include <iostream>

// =====================================================
// GLOBAL VARIABLES — declared outside any function
// accessible from anywhere in the file
// =====================================================
int globalCounter = 0;
const int MAX_PLAYERS = 10;         // global constant — value never changes


// =====================================================
// CONSTEXPR — evaluated at compile time
// =====================================================
constexpr double PI        = 3.141592653589793;
constexpr int    BOARD_SIZE = 8;             // chess board
constexpr double circleArea(double r) {      // constexpr function — C++11
    return PI * r * r;
}


// =====================================================
// STATIC VARIABLE in a function
// keeps its value between calls
// =====================================================
void countCalls()
{
    static int callCount = 0;   // initialized only ONCE, survives function exit
    ++callCount;
    std::cout << "countCalls() has been called " << callCount << " time(s)\n";
}


int main()
{
    // =================================================
    // DECLARATION & INITIALIZATION
    // =================================================
    std::cout << "=====================================================\n";
    std::cout << "Example 1: Declaration & Initialization\n";
    std::cout << "=====================================================\n";

    // Declaration only (uninitialized — undefined value, avoid this)
    int uninitialzed;

    // Declaration + initialization
    int age         = 25;
    double salary   = 3500.50;
    char grade      = 'A';
    bool isEmployed = true;

    // Uniform initialization (C++11) — preferred modern style
    int    score{100};
    double temperature{36.6};
    char   symbol{'#'};

    // auto — compiler deduces the type
    auto autoInt    = 42;           // int
    auto autoDouble = 3.14;         // double
    auto autoString = std::string("Kostas");  // std::string

    // Multiple declarations (same type)
    int x = 1, y = 2, z = 3;

    std::cout << "age:         " << age         << '\n';
    std::cout << "salary:      " << salary       << '\n';
    std::cout << "grade:       " << grade        << '\n';
    std::cout << "isEmployed:  " << (isEmployed ? "true" : "false") << '\n';
    std::cout << "score:       " << score        << '\n';
    std::cout << "temperature: " << temperature  << '\n';
    std::cout << "symbol:      " << symbol       << '\n';
    std::cout << "autoInt:     " << autoInt      << '\n';
    std::cout << "autoDouble:  " << autoDouble   << '\n';
    std::cout << "autoString:  " << autoString   << '\n';
    std::cout << "x, y, z:     " << x << ", " << y << ", " << z << '\n';


    // =================================================
    // CONST & CONSTEXPR
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: const & constexpr\n";
    std::cout << "=====================================================\n";

    // const — runtime constant, cannot be modified after initialization
    const int maxScore    = 100;
    const double taxRate  = 0.19;     // 19% German VAT

    // constexpr — compile-time constant, resolved before program runs
    constexpr int    SECONDS_PER_HOUR = 3600;
    constexpr double GRAVITY          = 9.81;

    // constexpr function call — computed at compile time
    constexpr double area = circleArea(5.0);

    std::cout << "MAX_PLAYERS (global const):  " << MAX_PLAYERS       << '\n';
    std::cout << "maxScore (local const):      " << maxScore          << '\n';
    std::cout << "taxRate:                     " << taxRate           << '\n';
    std::cout << "SECONDS_PER_HOUR:            " << SECONDS_PER_HOUR  << '\n';
    std::cout << "GRAVITY:                     " << GRAVITY           << '\n';
    std::cout << "PI (global constexpr):       " << PI                << '\n';
    std::cout << "BOARD_SIZE:                  " << BOARD_SIZE        << '\n';
    std::cout << "circleArea(5.0):             " << area              << '\n';

    // maxScore = 200;  // ERROR — cannot assign to const variable


    // =================================================
    // GLOBAL vs LOCAL SCOPE
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: Global vs Local Scope\n";
    std::cout << "=====================================================\n";

    globalCounter = 5;    // modifying global variable

    int localVar = 100;   // local to main()

    std::cout << "globalCounter:    " << globalCounter << '\n';
    std::cout << "localVar (main):  " << localVar      << '\n';

    // Inner scope — localVar here shadows nothing, but shows block scope
    {
        int blockVar = 42;          // lives only inside these braces
        globalCounter += 1;         // still accessible — it's global
        std::cout << "blockVar (inner scope):    " << blockVar      << '\n';
        std::cout << "globalCounter (modified):  " << globalCounter << '\n';
    }
    // std::cout << blockVar;  // ERROR — blockVar no longer exists here

    // Local variable shadowing global (same name)
    int globalCounter = 999;    // this LOCAL globalCounter shadows the global one
    std::cout << "local globalCounter (shadows global): " << globalCounter << '\n';
    std::cout << "real global via ::globalCounter:      " << ::globalCounter << '\n';


    // =================================================
    // STATIC VARIABLES
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: Static Variables\n";
    std::cout << "=====================================================\n";

    countCalls();   // called 1 time
    countCalls();   // called 2 times
    countCalls();   // called 3 times

    // Static local — same concept inline
    for (int i = 0; i < 3; ++i) {
        static int staticLoopVar = 0;   // initialized only on first iteration
        ++staticLoopVar;
        int normalLoopVar = 0;          // re-initialized every iteration
        ++normalLoopVar;
        std::cout << "iteration " << i
                  << " | staticLoopVar = " << staticLoopVar
                  << " | normalLoopVar = " << normalLoopVar << '\n';
    }

    return 0;
}
