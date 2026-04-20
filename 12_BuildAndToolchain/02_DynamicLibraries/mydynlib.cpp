#define MYDYNLIB_EXPORTS   // define before including header
#include "mydynlib.h"

#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <numeric>
#include <iostream>

// ============================================================
// MyDynLib — Dynamic Library Implementation
// ============================================================
// Compiled into libmydynlib.so (Linux) or mydynlib.dll (Windows)
// Loaded at runtime by the operating system
// ============================================================

namespace MyDynLib {

    int add(int a, int b)      { return a + b; }
    int multiply(int a, int b) { return a * b; }

    double divide(double a, double b) {
        if (b == 0.0)
            throw std::invalid_argument("MyDynLib::divide — division by zero");
        return a / b;
    }

    std::string toUpperCase(const std::string& s) {
        std::string result = s;
        std::transform(result.begin(), result.end(),
                       result.begin(), ::toupper);
        return result;
    }

    std::string trim(const std::string& s) {
        size_t start = s.find_first_not_of(" \t\n\r");
        size_t end   = s.find_last_not_of (" \t\n\r");
        if (start == std::string::npos) return "";
        return s.substr(start, end - start + 1);
    }

    double avgVector(const std::vector<int>& v) {
        if (v.empty()) return 0.0;
        return static_cast<double>(
            std::accumulate(v.begin(), v.end(), 0)) / v.size();
    }

    std::string version() { return "1.0.0"; }

    void printBanner() {
        std::cout << "╔══════════════════════════╗\n";
        std::cout << "║   MyDynLib v" << version() << "        ║\n";
        std::cout << "║   Dynamic Library Demo    ║\n";
        std::cout << "╚══════════════════════════╝\n";
    }

} // namespace MyDynLib
