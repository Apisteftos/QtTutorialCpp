#include <iostream>
#include <string>
#include <string_view>
#include <algorithm>
#include <source_location>
#include <print>
#include <numbers>
#include <type_traits>
#include <cassert>

// ============================================================
// MODERN C++23 — Replacements for #define Macros
// ============================================================
// Topics covered:
//   1.  constexpr       — replaces #define constants
//   2.  inline/template — replaces function-like macros
//   3.  if constexpr    — replaces #ifdef compile-time branching
//   4.  #pragma once    — replaces include guards
//   5.  std::source_location — replaces __FILE__ / __LINE__
//   6.  std::numbers    — replaces #define PI, E etc.
//   7.  [[nodiscard]]   — replaces warning macros
//   8.  Concepts        — replaces SFINAE macros
//   9.  consteval       — guaranteed compile-time evaluation
//  10.  Side-by-side comparison: old vs modern
// ============================================================


// ============================================================
// Example 1 — constexpr replaces #define constants
// ============================================================

// OLD WAY — no type, no scope, no debugger support
// #define PI          3.14159265358979
// #define MAX_BUFFER  1024
// #define APP_NAME    "MCXApp"

// MODERN WAY — typed, scoped, debuggable
constexpr double      PI          = 3.14159265358979;
constexpr int         MAX_BUFFER  = 1024;
constexpr std::string_view APP_NAME    = "MCXApp";
constexpr std::string_view APP_VERSION = "2.3.1";
constexpr uint16_t    MCX_PORT    = 5060;

// Can be grouped in a namespace — no name collision risk
namespace McxConfig {
constexpr int         MAX_SESSIONS   = 8;
constexpr int         FLOOR_TIMEOUT  = 30;
constexpr std::string_view DEFAULT_GROUP = "GRP-DEFAULT";
constexpr double      AUDIO_SAMPLE_RATE = 8000.0;
}


// ============================================================
// Example 2 — inline/template functions replace function macros
// ============================================================

// OLD WAY — dangerous, no type safety
// #define SQUARE(x)   ((x) * (x))
// #define MAX(a, b)   ((a) > (b) ? (a) : (b))
// #define CLAMP(v,lo,hi) ((v)<(lo)?(lo):((v)>(hi)?(hi):(v)))

// MODERN WAY — typed, safe, debuggable
template<typename T>
[[nodiscard]] constexpr T square(T x) noexcept { return x * x; }

template<typename T>
[[nodiscard]] constexpr T maxVal(T a, T b) noexcept { return a > b ? a : b; }

template<typename T>
[[nodiscard]] constexpr T minVal(T a, T b) noexcept { return a < b ? a : b; }

template<typename T>
[[nodiscard]] constexpr T clamp(T v, T lo, T hi) noexcept {
    return v < lo ? lo : (v > hi ? hi : v);
}

template<typename T>
[[nodiscard]] constexpr T toRadians(T degrees) noexcept {
    return degrees * static_cast<T>(PI) / static_cast<T>(180.0);
}

template<typename T>
constexpr void swapValues(T& a, T& b) noexcept {
    T temp = a; a = b; b = temp;
}

// No double-evaluation problem:
// square(x++)  →  calls square with x, x incremented ONCE after call


// ============================================================
// Example 3 — if constexpr replaces #ifdef
// ============================================================

// OLD WAY — preprocessor, not type-aware
// #ifdef DEBUG_MODE
//     std::cout << "debug info\n";
// #endif

// MODERN WAY — compile-time conditional, but inside functions
constexpr bool DEBUG_BUILD = true;   // set to false for release

template<bool Debug = DEBUG_BUILD>
void logDebug(std::string_view msg) {
    if constexpr (Debug) {
        std::println("[DBG] {}", msg);
    }
    // When Debug=false: compiler removes the body entirely
    // No runtime overhead — same as #ifdef
}

// Platform detection — still uses preprocessor but in a cleaner way
template<typename T>
constexpr std::string_view platformName() {
#if defined(__linux__)
    return "Linux";
#elif defined(_WIN32)
    return "Windows";
#elif defined(__APPLE__)
    return "macOS";
#else
    return "Unknown";
#endif
}


// ============================================================
// Example 5 — std::source_location replaces __FILE__ / __LINE__
// ============================================================

// OLD WAY
// #define LOG(msg) printf("[%s:%d] %s\n", __FILE__, __LINE__, msg)

// MODERN WAY — std::source_location (C++20)
void log(std::string_view message,
         const std::source_location& loc = std::source_location::current())
{
    std::println("[{}:{}] {}",
                 loc.file_name(),
                 loc.line(),
                 message);
}

void logWarning(std::string_view message,
                const std::source_location& loc = std::source_location::current())
{
    std::println("[WARN {}:{}] {}",
                 loc.file_name(),
                 loc.line(),
                 message);
}

// Custom assert using source_location
void myAssert(bool condition,
              std::string_view message = "assertion failed",
              const std::source_location& loc = std::source_location::current())
{
    if (!condition) {
        std::println("[ASSERT FAILED] {} at {}:{}  fn={}",
                     message, loc.file_name(), loc.line(), loc.function_name());
        std::terminate();
    }
}


// ============================================================
// Example 6 — std::numbers replaces #define PI, E etc.
// ============================================================

// OLD WAY
// #define PI    3.14159265358979
// #define E     2.71828182845905
// #define SQRT2 1.41421356237310

// MODERN WAY — std::numbers (C++20)
// std::numbers::pi, std::numbers::e, std::numbers::sqrt2
// All available as double and as template for any floating point type


// ============================================================
// Example 8 — Concepts replace SFINAE/type-check macros
// ============================================================

// OLD WAY — ugly SFINAE macro to restrict types
// #define ENABLE_IF_NUMERIC(T) \
//     typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr

// MODERN WAY — C++20 concepts
template<typename T>
concept Numeric = std::is_arithmetic_v<T>;

template<Numeric T>
T absoluteValue(T x) noexcept {
    return x < 0 ? -x : x;
}

template<Numeric T>
T power(T base, int exp) noexcept {
    T result = 1;
    for (int i = 0; i < exp; ++i) result *= base;
    return result;
}


// ============================================================
// Example 9 — consteval: guaranteed compile-time
// ============================================================

// constexpr CAN run at runtime
// consteval MUST run at compile time — compiler error if it can't

consteval int factorial(int n) {
    if (n <= 0) return 1;
    return n * factorial(n - 1);
}

consteval double circleArea(double radius) {
    return std::numbers::pi * radius * radius;
}

consteval int fibonacciCT(int n) {
    if (n <= 1) return n;
    return fibonacciCT(n-1) + fibonacciCT(n-2);
}


// ============================================================
// MAIN
// ============================================================
int main()
{
    std::cout << "=====================================================\n";
    std::cout << "Example 1: constexpr replaces #define constants\n";
    std::cout << "=====================================================\n";

    std::println("APP_NAME:    {}", APP_NAME);
    std::println("APP_VERSION: {}", APP_VERSION);
    std::println("MCX_PORT:    {}", MCX_PORT);
    std::println("PI:          {}", PI);
    std::println("MAX_BUFFER:  {}", MAX_BUFFER);

    std::println("McxConfig::MAX_SESSIONS:   {}", McxConfig::MAX_SESSIONS);
    std::println("McxConfig::FLOOR_TIMEOUT:  {}", McxConfig::FLOOR_TIMEOUT);
    std::println("McxConfig::DEFAULT_GROUP:  {}", McxConfig::DEFAULT_GROUP);

    // constexpr is typed — compiler catches this:
    // int bad = APP_NAME;   // ERROR — string_view not int
    // #define APP_NAME "MCXApp" would allow: int bad = APP_NAME (compiles but wrong)

    std::cout << "\nconstexpr advantages over #define:\n";
    std::cout << "  Type-safe — compiler enforces correct usage\n";
    std::cout << "  Scoped    — lives in namespace, no global pollution\n";
    std::cout << "  Debuggable— debugger knows the name and value\n";
    std::cout << "  Checked   — syntax/type errors caught at definition\n";


    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: Template functions replace function macros\n";
    std::cout << "=====================================================\n";

    std::println("square(5)           = {}", square(5));
    std::println("square(5.5)         = {}", square(5.5));
    std::println("square(3+2)         = {}", square(3+2));   // safe!

    std::println("maxVal(10, 20)      = {}", maxVal(10, 20));
    std::println("minVal(10, 20)      = {}", minVal(10, 20));
    std::println("clamp(150, 0, 100)  = {}", clamp(150, 0, 100));
    std::println("toRadians(180.0)    = {:.6f}", toRadians(180.0));

    int a = 10, b = 20;
    std::println("Before swap: a={} b={}", a, b);
    swapValues(a, b);
    std::println("After swap:  a={} b={}", a, b);

    // No double evaluation problem
    int x = 5;
    std::println("square(x++) where x=5: result={} x after={}", square(x), x+1);
    // x++ passed to square, then x incremented once — predictable!

    std::cout << "\nTemplate function advantages over function macros:\n";
    std::cout << "  Type-safe   — works with any type correctly\n";
    std::cout << "  No side effects — arguments evaluated once\n";
    std::cout << "  Debuggable  — can set breakpoints inside\n";
    std::cout << "  Overloadable— can specialize for types\n";


    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: if constexpr replaces #ifdef\n";
    std::cout << "=====================================================\n";

    logDebug("session SES-001 connected");
    logDebug("MCX group GRP-ALPHA has 3 members");

    std::println("Platform: {}", platformName<void>());

    std::cout << "\nif constexpr advantages over #ifdef:\n";
    std::cout << "  Still inside the function — better scoping\n";
    std::cout << "  Both branches type-checked (even unused one)\n";
    std::cout << "  Works with template parameters — very powerful\n";


    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: #pragma once replaces include guards\n";
    std::cout << "=====================================================\n";

    std::cout << "Old way (include guard):\n";
    std::cout << "  #ifndef MYHEADER_H\n";
    std::cout << "  #define MYHEADER_H\n";
    std::cout << "  // content\n";
    std::cout << "  #endif\n\n";
    std::cout << "Modern way:\n";
    std::cout << "  #pragma once    ← single line, no macro name needed\n";
    std::cout << "  // content\n\n";
    std::cout << "  Both prevent double-inclusion.\n";
    std::cout << "  #pragma once: simpler, no name collision risk.\n";
    std::cout << "  Supported by all major compilers (GCC, Clang, MSVC).\n";


    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: std::source_location replaces __FILE__/__LINE__\n";
    std::cout << "=====================================================\n";

    log("Application started");
    log("MCX session connecting to 192.168.1.100:5060");
    logWarning("Buffer usage above 80%");

    myAssert(MCX_PORT > 0,      "port must be positive");
    myAssert(MCX_PORT < 65536,  "port must be < 65536");
    std::cout << "All assertions passed\n";

    std::cout << "\nstd::source_location advantages over __FILE__/__LINE__:\n";
    std::cout << "  No macros needed — just a default parameter\n";
    std::cout << "  Also provides function_name() and column()\n";
    std::cout << "  Works with overloaded functions correctly\n";


    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: std::numbers — built-in math constants\n";
    std::cout << "=====================================================\n";

    std::println("std::numbers::pi     = {:.15f}", std::numbers::pi);
    std::println("std::numbers::e      = {:.15f}", std::numbers::e);
    std::println("std::numbers::sqrt2  = {:.15f}", std::numbers::sqrt2);
    std::println("std::numbers::phi    = {:.15f}", std::numbers::phi);
    std::println("std::numbers::ln2    = {:.15f}", std::numbers::ln2);

    // Typed for any precision
    std::println("pi as float  = {:.7f}", std::numbers::pi_v<float>);
    std::println("pi as double = {:.15f}", std::numbers::pi_v<double>);

    double area = std::numbers::pi * 5.0 * 5.0;
    std::println("Circle area (r=5):  {:.4f}", area);


    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: [[nodiscard]] — replaces warning macros\n";
    std::cout << "=====================================================\n";

    // [[nodiscard]] forces caller to use the return value
    // square(), maxVal(), minVal(), clamp() all have [[nodiscard]]

    std::cout << "[[nodiscard]] on square(), maxVal(), clamp():\n";
    std::cout << "  If you call square(5) without using result,\n";
    std::cout << "  compiler emits a warning — caught at compile time!\n";

    // This would warn:
    // square(5);    // warning: ignoring return value — [[nodiscard]]

    int result = square(7);   // OK — using the result
    std::println("square(7) used correctly: {}", result);


    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: Concepts replace SFINAE macros\n";
    std::cout << "=====================================================\n";

    std::println("absoluteValue(-42)   = {}", absoluteValue(-42));
    std::println("absoluteValue(-3.14) = {}", absoluteValue(-3.14));
    std::println("power(2, 10)         = {}", power(2, 10));
    std::println("power(3.0, 4)        = {}", power(3.0, 4));

    // absoluteValue("hello");   // ERROR — "hello" is not Numeric
    // Concept gives a clear error message instead of SFINAE gibberish

    std::cout << "\nConcepts advantages over SFINAE macros:\n";
    std::cout << "  Readable — 'concept Numeric' vs enable_if madness\n";
    std::cout << "  Clear errors — compiler says 'not Numeric'\n";
    std::cout << "  Composable — combine concepts with && and ||\n";


    std::cout << "\n=====================================================\n";
    std::cout << "Example 9: consteval — guaranteed compile-time\n";
    std::cout << "=====================================================\n";

    // All computed at COMPILE TIME — zero runtime cost
    constexpr int  fact6   = factorial(6);
    constexpr double area5 = circleArea(5.0);
    constexpr int  fib10   = fibonacciCT(10);

    std::println("factorial(6)     = {}  (compile-time)", fact6);
    std::println("circleArea(5.0)  = {:.4f}  (compile-time)", area5);
    std::println("fibonacci(10)    = {}  (compile-time)", fib10);

    // consteval vs constexpr:
    // constexpr factorial(6) — MAY be compile-time
    // consteval factorial(6) — MUST be compile-time (compiler error if not)

    std::cout << "\nconsteval advantages:\n";
    std::cout << "  Guarantees compile-time — no accidental runtime call\n";
    std::cout << "  Use for lookup tables, size calculations, constants\n";
    std::cout << "  Replaces complex template metaprogramming\n";


    std::cout << "\n=====================================================\n";
    std::cout << "Example 10: Side-by-side comparison\n";
    std::cout << "=====================================================\n";

    std::cout << "Old #define                    Modern C++23\n";
    std::cout << "─────────────────────────────────────────────────\n";
    std::cout << "#define PI 3.14159            constexpr double PI\n";
    std::cout << "#define MAX(a,b) ...          template<T> T max()\n";
    std::cout << "#define SQUARE(x) ((x)*(x))  constexpr T square()\n";
    std::cout << "#ifdef DEBUG                  if constexpr (Debug)\n";
    std::cout << "#ifndef HEADER_H              #pragma once\n";
    std::cout << "__FILE__ / __LINE__           std::source_location\n";
    std::cout << "#define PI 3.14159            std::numbers::pi\n";
    std::cout << "enable_if SFINAE macro        concept Numeric\n";
    std::cout << "constexpr (may be runtime)    consteval (must be CT)\n";
    std::cout << "\n";
    std::cout << "Rule: Only use #define when you have no alternative:\n";
    std::cout << "  - Include guards (or use #pragma once)\n";
    std::cout << "  - Platform detection (#ifdef __linux__)\n";
    std::cout << "  - Conditional feature compilation\n";
    std::cout << "  - Interop with C code\n";
    std::cout << "  Everything else → use modern C++\n";

    std::cout << "\n=== Modern C++23 Macros complete ===\n";
    return 0;
}
