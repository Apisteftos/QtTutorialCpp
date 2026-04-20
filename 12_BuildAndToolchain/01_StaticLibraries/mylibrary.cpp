#include "mylibrary.h"
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <numeric>
#include <iostream>
#include <cmath>

// ============================================================
// MyLibrary — Static Library Implementation
// ============================================================
// This file is compiled into libmylibrary.a
// It is NOT directly available to the user — only the .h and .a
// ============================================================

namespace MyLib {

    // ---- Math utilities ----

    int add(int a, int b)       { return a + b; }
    int subtract(int a, int b)  { return a - b; }
    int multiply(int a, int b)  { return a * b; }

    double divide(double a, double b) {
        if (b == 0.0)
            throw std::invalid_argument("MyLib::divide — division by zero");
        return a / b;
    }

    int clamp(int value, int lo, int hi) {
        if (value < lo) return lo;
        if (value > hi) return hi;
        return value;
    }

    double power(double base, int exp) {
        double result = 1.0;
        for (int i = 0; i < std::abs(exp); ++i) result *= base;
        return (exp >= 0) ? result : 1.0 / result;
    }

    // ---- String utilities ----

    std::string toUpperCase(const std::string& s) {
        std::string result = s;
        std::transform(result.begin(), result.end(),
                       result.begin(), ::toupper);
        return result;
    }

    std::string toLowerCase(const std::string& s) {
        std::string result = s;
        std::transform(result.begin(), result.end(),
                       result.begin(), ::tolower);
        return result;
    }

    std::string trim(const std::string& s) {
        size_t start = s.find_first_not_of(" \t\n\r");
        size_t end   = s.find_last_not_of(" \t\n\r");
        if (start == std::string::npos) return "";
        return s.substr(start, end - start + 1);
    }

    bool startsWith(const std::string& s, const std::string& prefix) {
        return s.size() >= prefix.size() &&
               s.compare(0, prefix.size(), prefix) == 0;
    }

    bool endsWith(const std::string& s, const std::string& suffix) {
        return s.size() >= suffix.size() &&
               s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
    }

    // ---- Collection utilities ----

    int sumVector(const std::vector<int>& v) {
        return std::accumulate(v.begin(), v.end(), 0);
    }

    double avgVector(const std::vector<int>& v) {
        if (v.empty()) return 0.0;
        return static_cast<double>(sumVector(v)) / v.size();
    }

    int maxVector(const std::vector<int>& v) {
        if (v.empty()) throw std::invalid_argument("MyLib::maxVector — empty vector");
        return *std::max_element(v.begin(), v.end());
    }

    int minVector(const std::vector<int>& v) {
        if (v.empty()) throw std::invalid_argument("MyLib::minVector — empty vector");
        return *std::min_element(v.begin(), v.end());
    }

    // ---- Library info ----

    std::string version() { return "1.0.0"; }

    void printBanner() {
        std::cout << "╔══════════════════════════╗\n";
        std::cout << "║   MyLibrary v" << version() << "        ║\n";
        std::cout << "║   Static Library Demo     ║\n";
        std::cout << "╚══════════════════════════╝\n";
    }

} // namespace MyLib
