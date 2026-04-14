#include <iostream>
#include <string>
#include <print>

// ============================================================
// .h header  — C-compatible, no C++ specific features
// .hpp header — C++ only, templates, classes, header-only
// ============================================================

// .h — plain C-style utility (could be used from C too)
#include "MathUtils.h"

// .hpp — C++ template class (definition must be in header)
#include "Stack.hpp"

// .hpp — header-only class (no .cpp needed)
#include "Logger.hpp"

// ============================================================
// .h vs .hpp — what the COMPILER actually thinks
// ============================================================
// The file extension (.h or .hpp) means NOTHING to the compiler.
// #include "MathUtils.h"   and   #include "Stack.hpp"
// are processed identically — the preprocessor just pastes
// the file content at that line.
//
// The extension is a CONVENTION for humans:
//   .h   → "this could work in C too"
//   .hpp → "this is C++ only"
// ============================================================


int main()
{
    // -------------------------------------------------------
    std::cout << "=====================================================\n";
    std::cout << "Example 1: .h vs .hpp — the core difference\n";
    std::cout << "=====================================================\n";

    std::cout << ".h  (C-compatible header):\n";
    std::cout << "  - No templates, no classes, no C++ stdlib\n";
    std::cout << "  - Can be #included from C code\n";
    std::cout << "  - Functions declared here, defined in .cpp\n";
    std::cout << "  - Example: MathUtils.h\n\n";

    std::cout << ".hpp (C++ only header):\n";
    std::cout << "  - Templates, classes, std::vector, std::string etc.\n";
    std::cout << "  - Cannot be used from C\n";
    std::cout << "  - Often header-only (no separate .cpp)\n";
    std::cout << "  - Template definitions MUST be in header\n";
    std::cout << "  - Example: Stack.hpp, Logger.hpp\n\n";

    std::cout << "Both use #pragma once and are #included the same way.\n";
    std::cout << "The compiler treats them identically.\n";
    std::cout << "The extension is a convention — for human readers.\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: Using .h — MathUtils (C-style functions)\n";
    std::cout << "=====================================================\n";

    // Functions declared in MathUtils.h, defined in MathUtils.cpp
    std::println("math_add(10, 5)        = {}", math_add(10, 5));
    std::println("math_subtract(10, 5)   = {}", math_subtract(10, 5));
    std::println("math_multiply(6, 7)    = {}", math_multiply(6, 7));
    std::println("math_divide(10.0, 3.0) = {:.4f}", math_divide(10.0f, 3.0f));
    std::println("math_clamp(150, 0,100) = {}", math_clamp(150, 0, 100));
    std::println("MATH_PI                = {}", MATH_PI);

    std::cout << "\nNote: MathUtils.h uses C-compatible syntax only.\n";
    std::cout << "      It could be #included from a .c file.\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: Using .hpp — Stack<T> (template class)\n";
    std::cout << "=====================================================\n";

    // Stack<int> — compiler generates int version from template in Stack.hpp
    Stack<int> intStack;
    intStack.push(10);
    intStack.push(20);
    intStack.push(30);
    std::println("intStack: {}", intStack.toString());
    std::println("top: {} size: {}", intStack.top(), intStack.size());
    intStack.pop();
    std::println("after pop: {}", intStack.toString());

    // Stack<std::string> — compiler generates string version
    Stack<std::string> strStack;
    strStack.push("alpha");
    strStack.push("beta");
    strStack.push("gamma");
    std::println("\nstrStack top: {}", strStack.top());
    std::println("strStack size: {}", strStack.size());

    // Stack<double>
    Stack<double> dblStack(10);   // reserve 10 slots
    dblStack.push(3.14);
    dblStack.push(2.71);
    std::println("\ndblStack top: {}", dblStack.top());

    // Exception on empty stack
    Stack<int> empty;
    try {
        empty.top();
    } catch (const std::underflow_error& e) {
        std::println("caught: {}", e.what());
    }

    std::cout << "\nWhy Stack definition must be in .hpp (not .cpp):\n";
    std::cout << "  Template is a blueprint — no code generated until\n";
    std::cout << "  compiler sees Stack<int>, Stack<string> etc.\n";
    std::cout << "  If definition were in Stack.cpp, main.cpp cannot\n";
    std::cout << "  see it → linker error: undefined reference to Stack<int>\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: Using .hpp — Logger (header-only class)\n";
    std::cout << "=====================================================\n";

    // Logger is entirely defined in Logger.hpp — no Logger.cpp needed
    Logger appLog("APP");
    appLog.debug("application started");
    appLog.info("loading configuration");
    appLog.warning("config file not found — using defaults");
    appLog.error("connection refused");

    std::cout << '\n';
    Logger mcxLog("MCX");
    mcxLog.setLevel(LogLevel::WARNING);   // only warnings and above
    mcxLog.debug("this won't print — below threshold");
    mcxLog.info("this won't print — below threshold");
    mcxLog.warning("floor request timeout");
    mcxLog.error("session disconnected");

    std::cout << "\nLogger.hpp is header-only:\n";
    std::cout << "  Small utility class — all methods fit in the header\n";
    std::cout << "  No Logger.cpp needed — just #include \"Logger.hpp\"\n";
    std::cout << "  Common for small helpers, utilities, adapters\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: Mixing .h and .hpp in one project\n";
    std::cout << "=====================================================\n";

    std::cout << "It is completely normal to mix .h and .hpp:\n\n";

    std::cout << "  #include \"MathUtils.h\"   // C-style functions (.h)\n";
    std::cout << "  #include \"Stack.hpp\"     // template class (.hpp)\n";
    std::cout << "  #include \"Logger.hpp\"    // header-only class (.hpp)\n";
    std::cout << "  #include <iostream>      // system header (no extension)\n\n";

    // Mix them in the same code — no problem
    Logger mixLog("MIX");
    Stack<int> mixStack;

    for (int i : {5, 10, 15, 20}) {
        int clamped = math_clamp(i, 8, 18);   // from .h
        mixStack.push(clamped);               // from .hpp
        mixLog.debug("pushed " + std::to_string(clamped));  // from .hpp
    }

    std::println("\nfinal stack: {}", mixStack.toString());


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: When to use .h vs .hpp — decision guide\n";
    std::cout << "=====================================================\n";

    std::cout << "Use .h when:\n";
    std::cout << "  The header might be shared with C code\n";
    std::cout << "  No templates, no C++ classes, no std library\n";
    std::cout << "  Simple function declarations and #define constants\n";
    std::cout << "  Working with C APIs or embedded systems\n\n";

    std::cout << "Use .hpp when:\n";
    std::cout << "  Template classes or functions (definitions MUST be here)\n";
    std::cout << "  Header-only library (small class, all in one file)\n";
    std::cout << "  C++ only — uses std::vector, std::string, classes etc.\n";
    std::cout << "  3rd party C++ libraries (Eigen, nlohmann/json use .hpp)\n\n";

    std::cout << "Use either (.h or .hpp) for:\n";
    std::cout << "  C++ class declarations with separate .cpp\n";
    std::cout << "  It's a team/project style choice — be consistent\n\n";

    std::cout << "Qt and many C++ projects use .h for everything:\n";
    std::cout << "  QThread.h, QObject.h, QVector.h — all Qt headers are .h\n";
    std::cout << "  Qt uses .h even for C++ only classes\n\n";

    std::cout << "Standard library uses NO extension:\n";
    std::cout << "  <vector>, <string>, <algorithm> — no .h or .hpp\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: Template in .h — also valid\n";
    std::cout << "=====================================================\n";

    std::cout << "Some projects use .h for everything including templates.\n";
    std::cout << "The compiler doesn't care — extension is just convention.\n\n";

    std::cout << "  // MyTemplate.h (using .h for a template — valid!)\n";
    std::cout << "  #pragma once\n";
    std::cout << "  template<typename T>\n";
    std::cout << "  T max(T a, T b) { return a > b ? a : b; }\n\n";

    // Inline template in this file to demonstrate
    auto maxVal = [](auto a, auto b) { return a > b ? a : b; };
    std::println("max(3, 7) = {}", maxVal(3, 7));
    std::println("max(3.14, 2.71) = {}", maxVal(3.14, 2.71));

    std::cout << "\nConvention summary across popular projects:\n";
    std::cout << "  Qt          → .h  for everything\n";
    std::cout << "  Boost       → .hpp for C++ headers\n";
    std::cout << "  Eigen       → .hpp for template math library\n";
    std::cout << "  nlohmann    → .hpp for json header-only library\n";
    std::cout << "  OpenCV      → .hpp for C++ API\n";
    std::cout << "  Your project→ pick ONE convention and stick to it\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: CMakeLists.txt — only .cpp files listed\n";
    std::cout << "=====================================================\n";

    std::cout << "  add_executable(08_HppHeaders\n";
    std::cout << "      main.cpp\n";
    std::cout << "      MathUtils.cpp    # .h functions need a .cpp\n";
    std::cout << "      # Stack.hpp      — NOT listed! Header-only\n";
    std::cout << "      # Logger.hpp     — NOT listed! Header-only\n";
    std::cout << "  )\n\n";

    std::cout << "  .h + .cpp pair  → list the .cpp in add_executable\n";
    std::cout << "  .hpp header-only → do NOT list in add_executable\n";
    std::cout << "                     just #include it where needed\n";

    std::cout << "\n=== .h vs .hpp complete ===\n";
    return 0;
}
