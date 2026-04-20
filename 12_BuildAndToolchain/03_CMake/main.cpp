#include <iostream>
#include <string>
#include <vector>
#include <print>

// ============================================================
// CMake Tutorial — C++23
// ============================================================
// This file demonstrates what CMake builds — it references
// concepts explained in the README.
//
// Topics covered in the README:
//   1.  What CMake is and why we use it
//   2.  CMakeLists.txt structure
//   3.  Variables, options, conditions
//   4.  add_executable / add_library
//   5.  target_link_libraries
//   6.  find_package
//   7.  install() rules
//   8.  Out-of-source builds
//   9.  Build types (Debug, Release)
//  10.  Modern CMake (target-based)
// ============================================================

// Simulated project info injected by CMake via configure_file
// In a real project these come from CMakeLists.txt:
// configure_file(config.h.in config.h)
#define PROJECT_NAME    "03_CMake"
#define PROJECT_VERSION "1.0.0"
#define BUILD_TYPE      "Release"

void printBuildInfo()
{
    std::println("Project:   {}", PROJECT_NAME);
    std::println("Version:   {}", PROJECT_VERSION);
    std::println("BuildType: {}", BUILD_TYPE);
}

int add(int a, int b) { return a + b; }
int multiply(int a, int b) { return a * b; }

int main()
{
    std::cout << "=====================================================\n";
    std::cout << "CMake Tutorial — Build Info\n";
    std::cout << "=====================================================\n";
    printBuildInfo();

    std::cout << "\n=====================================================\n";
    std::cout << "Program output\n";
    std::cout << "=====================================================\n";
    std::println("add(6, 7)      = {}", add(6, 7));
    std::println("multiply(6, 7) = {}", multiply(6, 7));

    std::vector<std::string> cmakeFeatures = {
        "Out-of-source builds",
        "Cross-platform Makefiles/Ninja",
        "find_package for dependencies",
        "target_link_libraries",
        "install() rules",
        "Build types: Debug / Release / RelWithDebInfo",
        "Generator expressions",
        "CPack for packaging"
    };

    std::cout << "\nCMake key features:\n";
    for (const auto& f : cmakeFeatures)
        std::println("  - {}", f);

    std::cout << "\n=== CMake example complete ===\n";
    return 0;
}
