#include <iostream>
#include <string>
#include <cstdio>

// ============================================================
// MACROS — Old Style C/C++ (#define preprocessor)
// ============================================================
// Topics covered:
//   1.  Object-like macros         — constant replacement
//   2.  Function-like macros       — code replacement
//   3.  Conditional compilation    — #ifdef / #ifndef / #if
//   4.  Include guards             — #ifndef HEADER_H
//   5.  Stringification            — #x operator
//   6.  Token concatenation        — ## operator
//   7.  Variadic macros            — __VA_ARGS__
//   8.  Predefined macros          — __FILE__, __LINE__, __func__
//   9.  Multi-line macros          — backslash continuation
//  10.  Common pitfalls            — why macros are dangerous
// ============================================================


// ============================================================
// Example 1 — Object-like macros (constant replacement)
// The preprocessor does a TEXT substitution before compilation
// ============================================================
#define PI          3.14159265358979
#define MAX_BUFFER  1024
#define APP_NAME    "MCXApp"
#define APP_VERSION "2.3.1"
#define MCX_PORT    5060
#define NEWLINE     '\n'

// ============================================================
// Example 2 — Function-like macros (code replacement)
// WARNING: not real functions — just text substitution!
// ============================================================
#define SQUARE(x)         ((x) * (x))
#define MAX(a, b)         ((a) > (b) ? (a) : (b))
#define MIN(a, b)         ((a) < (b) ? (a) : (b))
#define ABS(x)            ((x) < 0 ? -(x) : (x))
#define CLAMP(v, lo, hi)  ((v) < (lo) ? (lo) : ((v) > (hi) ? (hi) : (v)))
#define TO_RAD(deg)       ((deg) * PI / 180.0)
#define SWAP(a, b)        { auto _t = (a); (a) = (b); (b) = _t; }

// ============================================================
// Example 3 — Conditional compilation
// ============================================================
#define DEBUG_MODE        // comment this out to disable debug output
#define PLATFORM_LINUX

// ============================================================
// Example 5 — Stringification: # turns argument into string
// ============================================================
#define STRINGIFY(x)      #x
#define TOSTRING(x)       STRINGIFY(x)   // double expansion trick

// ============================================================
// Example 6 — Token concatenation: ## joins two tokens
// ============================================================
#define CONCAT(a, b)      a##b
#define MAKE_VAR(prefix, num)  prefix##num

// ============================================================
// Example 7 — Variadic macros: __VA_ARGS__
// ============================================================
#define LOG(fmt, ...)     printf("[LOG]  " fmt "\n", ##__VA_ARGS__)
#define WARN(fmt, ...)    printf("[WARN] " fmt "\n", ##__VA_ARGS__)
#define ERR(fmt, ...)     fprintf(stderr, "[ERR]  " fmt "\n", ##__VA_ARGS__)

// ============================================================
// Example 9 — Multi-line macro with backslash
// ============================================================
#define PRINT_SESSION_INFO(id, host, port)  \
    do {                                     \
        printf("Session: %s\n", id);         \
        printf("Host:    %s\n", host);       \
        printf("Port:    %d\n", port);       \
    } while(0)

// The do { } while(0) trick makes it safe to use in if/else:
// if (x) PRINT_SESSION_INFO(...);   ← works correctly
// Without it: if (x) printf(...); printf(...);  ← second line always runs!


// ============================================================
// MAIN
// ============================================================
int main()
{
    // -------------------------------------------------------
    std::cout << "=====================================================\n";
    std::cout << "Example 1: Object-like macros\n";
    std::cout << "=====================================================\n";

    std::cout << "APP_NAME:    " << APP_NAME    << '\n';
    std::cout << "APP_VERSION: " << APP_VERSION << '\n';
    std::cout << "MCX_PORT:    " << MCX_PORT    << '\n';
    std::cout << "PI:          " << PI          << '\n';
    std::cout << "MAX_BUFFER:  " << MAX_BUFFER  << '\n';

    // What the preprocessor actually does — text substitution:
    // double area = PI * r * r;
    // becomes:
    // double area = 3.14159265358979 * r * r;
    double r    = 5.0;
    double area = PI * r * r;
    std::cout << "Circle area (r=5): " << area << '\n';

    std::cout << "\nProblem with object macros:\n";
    std::cout << "  No type — PI could be used as int, double, anything\n";
    std::cout << "  No scope — visible everywhere after #define\n";
    std::cout << "  No debugger support — replaced before compilation\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: Function-like macros\n";
    std::cout << "=====================================================\n";

    std::cout << "SQUARE(5)        = " << SQUARE(5)        << '\n';
    std::cout << "SQUARE(3+2)      = " << SQUARE(3+2)      << '\n';
    std::cout << "MAX(10, 20)      = " << MAX(10, 20)      << '\n';
    std::cout << "MIN(10, 20)      = " << MIN(10, 20)      << '\n';
    std::cout << "ABS(-42)         = " << ABS(-42)         << '\n';
    std::cout << "CLAMP(150, 0,100)= " << CLAMP(150, 0,100)<< '\n';
    std::cout << "TO_RAD(180)      = " << TO_RAD(180)      << '\n';

    // Classic pitfall — double evaluation!
    int x = 5;
    // SQUARE(x++)  expands to: ((x++) * (x++))
    // x is incremented TWICE — undefined behavior!
    std::cout << "\nPitfall — SQUARE(x++) where x=5:\n";
    std::cout << "  Expands to: ((x++) * (x++)) — x incremented twice!\n";
    std::cout << "  Result is undefined behavior\n";

    // Swap macro
    int a = 10, b = 20;
    std::cout << "\nBefore SWAP: a=" << a << " b=" << b << '\n';
    SWAP(a, b);
    std::cout << "After  SWAP: a=" << a << " b=" << b << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: Conditional compilation\n";
    std::cout << "=====================================================\n";

#ifdef DEBUG_MODE
    std::cout << "DEBUG_MODE is defined — debug output enabled\n";
    std::cout << "  [DBG] session state: CONNECTED\n";
    std::cout << "  [DBG] buffer size:   " << MAX_BUFFER << '\n';
#else
    std::cout << "DEBUG_MODE not defined — release build\n";
#endif

#ifdef PLATFORM_LINUX
    std::cout << "Platform: Linux\n";
#elif defined(PLATFORM_WINDOWS)
    std::cout << "Platform: Windows\n";
#else
    std::cout << "Platform: Unknown\n";
#endif

    // #if with numeric comparison
#if MCX_PORT == 5060
    std::cout << "MCX using standard SIP port 5060\n";
#elif MCX_PORT == 5061
    std::cout << "MCX using TLS port 5061\n";
#endif

    std::cout << "\nConditional compilation use cases:\n";
    std::cout << "  Platform-specific code (Linux vs Windows)\n";
    std::cout << "  Debug vs Release builds\n";
    std::cout << "  Feature flags\n";
    std::cout << "  Include guards\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: Include guards\n";
    std::cout << "=====================================================\n";

    std::cout << "Include guard pattern (in header files):\n\n";
    std::cout << "  #ifndef MYHEADER_H     ← if not defined yet\n";
    std::cout << "  #define MYHEADER_H     ← define it (mark as included)\n";
    std::cout << "  \n";
    std::cout << "  // header content here\n";
    std::cout << "  \n";
    std::cout << "  #endif                 ← end of guard\n\n";
    std::cout << "Modern alternative: #pragma once (same effect, simpler)\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: Stringification — # operator\n";
    std::cout << "=====================================================\n";

    // # turns the argument into a string literal
    std::cout << "STRINGIFY(hello)       = " << STRINGIFY(hello)       << '\n';
    std::cout << "STRINGIFY(42)          = " << STRINGIFY(42)          << '\n';
    std::cout << "STRINGIFY(PI)          = " << STRINGIFY(PI)          << '\n';  // "PI" not value!
    std::cout << "TOSTRING(PI)           = " << TOSTRING(PI)           << '\n';  // value via expansion

    // Practical use: printing variable names
    #define PRINT_VAR(var) printf("  %s = %d\n", #var, var)

    int sessionCount = 5;
    int maxSessions  = MAX_BUFFER;
    PRINT_VAR(sessionCount);
    PRINT_VAR(maxSessions);


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: Token concatenation — ## operator\n";
    std::cout << "=====================================================\n";

    // ## joins two tokens into one
    int CONCAT(my, Variable) = 42;    // creates int myVariable = 42
    std::cout << "CONCAT(my, Variable) creates 'myVariable' = " << myVariable << '\n';

    // Generate multiple variables
    int MAKE_VAR(sensor, 1) = 100;
    int MAKE_VAR(sensor, 2) = 200;
    int MAKE_VAR(sensor, 3) = 300;
    std::cout << "sensor1=" << sensor1 << " sensor2=" << sensor2 << " sensor3=" << sensor3 << '\n';

    // Practical: generate function names
    #define HANDLER(event)  void on_##event()
    // HANDLER(click)  expands to:  void on_click()
    std::cout << "HANDLER(click) expands to: void on_click()\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: Variadic macros — __VA_ARGS__\n";
    std::cout << "=====================================================\n";

    LOG("Application started");
    LOG("Session %s connected on port %d", "SES-001", MCX_PORT);
    WARN("Buffer usage: %d%%", 85);
    LOG("MCX group %s has %d members", "GRP-ALPHA", 3);


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: Predefined macros\n";
    std::cout << "=====================================================\n";

    std::cout << "__FILE__ = " << __FILE__ << '\n';
    std::cout << "__LINE__ = " << __LINE__ << '\n';
    std::cout << "__DATE__ = " << __DATE__ << '\n';
    std::cout << "__TIME__ = " << __TIME__ << '\n';

    // __func__ — current function name (C99/C++11)
    auto printFuncName = []() {
        std::cout << "__func__ = " << __func__ << '\n';
    };
    printFuncName();

    // Practical: assertion macro using predefined macros
    #define MY_ASSERT(cond) \
        do { \
            if (!(cond)) { \
                fprintf(stderr, "ASSERT failed: %s at %s:%d\n", \
                        #cond, __FILE__, __LINE__); \
            } \
        } while(0)

    int port = MCX_PORT;
    MY_ASSERT(port > 0);
    MY_ASSERT(port < 65536);
    std::cout << "Assertions passed\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 9: Multi-line macro\n";
    std::cout << "=====================================================\n";

    PRINT_SESSION_INFO("SES-001", "192.168.1.100", MCX_PORT);


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 10: Common macro pitfalls\n";
    std::cout << "=====================================================\n";

    std::cout << "Pitfall 1 — No parentheses:\n";
    std::cout << "  #define DOUBLE(x) x * 2\n";
    std::cout << "  DOUBLE(3+4) expands to 3+4*2 = 11, NOT 14!\n";
    std::cout << "  Fix: #define DOUBLE(x) ((x) * 2)\n\n";

    std::cout << "Pitfall 2 — Double evaluation:\n";
    std::cout << "  #define MAX(a,b) (a) > (b) ? (a) : (b)\n";
    std::cout << "  MAX(i++, j++) — i and j incremented twice!\n\n";

    std::cout << "Pitfall 3 — No type safety:\n";
    std::cout << "  #define PI 3.14  — int x = PI; no warning!\n\n";

    std::cout << "Pitfall 4 — No scope:\n";
    std::cout << "  #define MAX 100  — clashes with std::max, class MAX etc.\n\n";

    std::cout << "Pitfall 5 — Hard to debug:\n";
    std::cout << "  Macros replaced before compilation\n";
    std::cout << "  Debugger shows expanded code, not macro name\n\n";

    std::cout << "Modern C++ solution:\n";
    std::cout << "  constexpr — replaces object macros\n";
    std::cout << "  inline functions / templates — replaces function macros\n";
    std::cout << "  if constexpr — replaces conditional compilation\n";
    std::cout << "  #pragma once — replaces include guards\n";
    std::cout << "  std::source_location — replaces __FILE__ / __LINE__\n";

    std::cout << "\n=== Old-style Macros complete ===\n";
    return 0;
}
