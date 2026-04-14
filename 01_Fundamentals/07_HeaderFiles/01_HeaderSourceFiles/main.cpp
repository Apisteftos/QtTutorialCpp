#include <iostream>
#include <string>
#include <vector>
#include <print>

// ============================================================
// Our own headers — use quotes, not angle brackets
// ============================================================
#include "MathUtils.h"    // math utility declarations
#include "McxConfig.h"    // MCX config struct + constants

// ============================================================
// HEADER FILES in C++23
// ============================================================
// Topics covered:
//   1.  Why header files exist
//   2.  #pragma once vs include guards
//   3.  Declaration in .h vs definition in .cpp
//   4.  Using our own header: MathUtils.h / MathUtils.cpp
//   5.  Struct and class headers: McxConfig.h / McxConfig.cpp
//   6.  Constants and enums in headers
//   7.  Angle brackets <> vs quotes ""
//   8.  Forward declarations
//   9.  What belongs in a header vs .cpp
//  10.  CMakeLists.txt — registering multiple source files
// ============================================================


// ============================================================
// Example 8 — Forward declaration
// Declare a type without including its full header
// ============================================================
class Logger;   // forward declaration — we just say Logger exists
    // no need to #include "Logger.h" here

void doSomethingWithLogger(Logger* log);  // can use Logger* in declarations


int main()
{
    // -------------------------------------------------------
    std::cout << "=====================================================\n";
    std::cout << "Example 1: Why header files exist\n";
    std::cout << "=====================================================\n";

    std::cout << "Problem without headers:\n";
    std::cout << "  File A defines add()\n";
    std::cout << "  File B wants to call add()\n";
    std::cout << "  File B must know add's signature — but how?\n\n";
    std::cout << "Solution: header file\n";
    std::cout << "  MathUtils.h  — declares 'int add(int a, int b)'\n";
    std::cout << "  MathUtils.cpp— defines the actual implementation\n";
    std::cout << "  main.cpp     — #includes MathUtils.h to use add()\n\n";
    std::cout << "Benefits:\n";
    std::cout << "  Separation of interface from implementation\n";
    std::cout << "  Compile once, use everywhere\n";
    std::cout << "  Faster builds — only changed .cpp files recompile\n";
    std::cout << "  Clean public API — header IS the documentation\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: #pragma once vs include guards\n";
    std::cout << "=====================================================\n";

    std::cout << "Both prevent a header from being included twice:\n\n";
    std::cout << "  // Modern way — #pragma once (supported everywhere)\n";
    std::cout << "  #pragma once\n\n";
    std::cout << "  // Traditional way — macro include guard\n";
    std::cout << "  #ifndef MATHUTILS_H\n";
    std::cout << "  #define MATHUTILS_H\n";
    std::cout << "  // ... header content ...\n";
    std::cout << "  #endif // MATHUTILS_H\n\n";
    std::cout << "  Both files in this project use #pragma once\n";
    std::cout << "  Prefer #pragma once — simpler, less error-prone\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: Declaration (.h) vs Definition (.cpp)\n";
    std::cout << "=====================================================\n";

    std::cout << "MathUtils.h  (declaration — WHAT exists):\n";
    std::cout << "  int add(int a, int b);        // just the signature\n\n";
    std::cout << "MathUtils.cpp (definition — HOW it works):\n";
    std::cout << "  int add(int a, int b) {        // actual code\n";
    std::cout << "      return a + b;\n";
    std::cout << "  }\n\n";
    std::cout << "Rule: declarations in .h, definitions in .cpp\n";
    std::cout << "      (exceptions: templates, inline, constexpr)\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: Using MathUtils.h\n";
    std::cout << "=====================================================\n";

    // All these functions are declared in MathUtils.h
    // and implemented in MathUtils.cpp
    std::println("add(10, 5)       = {}", MathUtils::add(10, 5));
    std::println("subtract(10, 5)  = {}", MathUtils::subtract(10, 5));
    std::println("multiply(6, 7)   = {}", MathUtils::multiply(6, 7));
    std::println("divide(10.0, 3)  = {:.4f}", MathUtils::divide(10.0, 3.0));
    std::println("power(2, 10)     = {}", MathUtils::power(2.0, 10));
    std::println("squareRoot(144)  = {}", MathUtils::squareRoot(144.0));
    std::println("factorial(6)     = {}", MathUtils::factorial(6));
    std::println("isPrime(17)      = {}", MathUtils::isPrime(17));
    std::println("isPrime(15)      = {}", MathUtils::isPrime(15));
    std::println("clamp(150, 0,100)= {}", MathUtils::clamp(150, 0, 100));

    // Constants from the header
    std::println("PI    = {:.10f}", MathUtils::PI);
    std::println("EULER = {:.10f}", MathUtils::EULER);

    // Exception from implementation
    try {
        MathUtils::divide(5.0, 0.0);
    } catch (const std::invalid_argument& e) {
        std::println("caught: {}", e.what());
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: Struct and class headers — McxConfig.h\n";
    std::cout << "=====================================================\n";

    // Default constructor (defined in McxConfig.cpp)
    McxSessionConfig defaultCfg;
    std::println("default: {}", defaultCfg.toString());
    std::println("valid:   {}", defaultCfg.isValid());

    // Parameterized constructor
    McxSessionConfig session("192.168.1.100",
                             5060,
                             "GRP-ALPHA",
                             McxCodec::AMR_WB,
                             true);
    std::println("session: {}", session.toString());
    std::println("valid:   {}", session.isValid());

    // Invalid config
    McxSessionConfig bad("", 0, "");
    std::println("bad cfg valid: {}", bad.isValid());


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: Constants and enums in headers\n";
    std::cout << "=====================================================\n";

    // These constants are declared in McxConfig.h
    // Available in any file that includes McxConfig.h
    std::println("MCX_MAX_SESSIONS:  {}", MCX_MAX_SESSIONS);
    std::println("MCX_DEFAULT_PORT:  {}", MCX_DEFAULT_PORT);
    std::println("MCX_FLOOR_TIMEOUT: {}", MCX_FLOOR_TIMEOUT);

    // Enum from header
    McxCodec codec = McxCodec::OPUS;
    if (codec == McxCodec::OPUS)
        std::cout << "codec is OPUS\n";

    std::cout << "\nWhat belongs in headers:\n";
    std::cout << "  constexpr constants    — available everywhere\n";
    std::cout << "  enum / enum class      — shared type definitions\n";
    std::cout << "  struct / class         — layout known to all users\n";
    std::cout << "  function declarations  — signature only\n";
    std::cout << "  inline functions       — small, defined in header\n";
    std::cout << "  template definitions   — must be in header\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: Angle brackets <> vs quotes \"\"\n";
    std::cout << "=====================================================\n";

    std::cout << "  #include <iostream>     // system/library header\n";
    std::cout << "  #include <vector>       // system/library header\n";
    std::cout << "  #include <Qt/QThread>   // Qt framework header\n\n";
    std::cout << "  #include \"MathUtils.h\" // OUR header — same project\n";
    std::cout << "  #include \"McxConfig.h\" // OUR header — same project\n\n";
    std::cout << "Search order:\n";
    std::cout << "  <>  — searches compiler's system include paths\n";
    std::cout << "  \"\"  — searches current directory FIRST,\n";
    std::cout << "          then falls back to system include paths\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: Forward declarations\n";
    std::cout << "=====================================================\n";

    std::cout << "Forward declaration: tell compiler a type EXISTS\n";
    std::cout << "without including its full header:\n\n";
    std::cout << "  class Logger;                    // forward declare\n";
    std::cout << "  void fn(Logger* log);            // can use Logger* \n\n";
    std::cout << "  // NO #include \"Logger.h\" needed here!\n\n";
    std::cout << "  // But in the .cpp that CALLS fn:\n";
    std::cout << "  #include \"Logger.h\"             // full definition needed\n\n";
    std::cout << "Benefits:\n";
    std::cout << "  Breaks circular include dependencies\n";
    std::cout << "  Reduces compile time (less headers to parse)\n";
    std::cout << "  Reduces coupling between files\n\n";
    std::cout << "Limitation:\n";
    std::cout << "  Can only use Logger* or Logger& (pointer/reference)\n";
    std::cout << "  Cannot use Logger by value — size unknown\n";
    std::cout << "  Cannot call any Logger methods — definition unknown\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 9: What belongs in .h vs .cpp\n";
    std::cout << "=====================================================\n";

    std::cout << "In the HEADER (.h):\n";
    std::cout << "  class / struct definitions\n";
    std::cout << "  function DECLARATIONS (prototypes)\n";
    std::cout << "  constexpr constants\n";
    std::cout << "  enum / enum class\n";
    std::cout << "  inline function definitions\n";
    std::cout << "  template definitions (MUST be in header)\n";
    std::cout << "  #pragma once or include guard\n\n";

    std::cout << "In the CPP (.cpp):\n";
    std::cout << "  function DEFINITIONS (implementations)\n";
    std::cout << "  class method implementations\n";
    std::cout << "  static variable definitions\n";
    std::cout << "  #include of headers needed by implementation\n\n";

    std::cout << "NOT in the header:\n";
    std::cout << "  using namespace std;  ← pollutes all including files!\n";
    std::cout << "  global variable definitions\n";
    std::cout << "  function definitions (causes multiple definition error)\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 10: CMakeLists.txt — multiple source files\n";
    std::cout << "=====================================================\n";

    std::cout << "To compile multiple .cpp files in Qt Creator / CMake:\n\n";
    std::cout << "  add_executable(07_HeaderFiles\n";
    std::cout << "      main.cpp\n";
    std::cout << "      MathUtils.cpp      # add each .cpp here\n";
    std::cout << "      McxConfig.cpp      # header .h files NOT listed\n";
    std::cout << "  )\n\n";
    std::cout << "  target_link_libraries(07_HeaderFiles\n";
    std::cout << "      Qt${QT_VERSION_MAJOR}::Core\n";
    std::cout << "      stdc++exp\n";
    std::cout << "  )\n\n";
    std::cout << "Rules:\n";
    std::cout << "  List .cpp files in add_executable — they get compiled\n";
    std::cout << "  Header .h files are NOT listed — they get #included\n";
    std::cout << "  Each .cpp compiles independently into an object file\n";
    std::cout << "  Linker combines object files into final executable\n";

    std::cout << "\n=== HeaderFiles complete ===\n";
    return 0;
}
