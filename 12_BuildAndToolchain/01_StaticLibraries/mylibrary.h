#pragma once

#include <string>
#include <vector>

// ============================================================
// MyLibrary — Static Library Header
// ============================================================
// This header is the PUBLIC API of the library.
// Anyone linking against libmylibrary.a includes this file.
// The implementation lives in mylibrary.cpp (compiled into .a)
// ============================================================

namespace MyLib {

    // ---- Math utilities ----
    int    add        (int a, int b);
    int    subtract   (int a, int b);
    int    multiply   (int a, int b);
    double divide     (double a, double b);
    int    clamp      (int value, int lo, int hi);
    double power      (double base, int exp);

    // ---- String utilities ----
    std::string toUpperCase (const std::string& s);
    std::string toLowerCase (const std::string& s);
    std::string trim        (const std::string& s);
    bool        startsWith  (const std::string& s, const std::string& prefix);
    bool        endsWith    (const std::string& s, const std::string& suffix);

    // ---- Collection utilities ----
    int         sumVector   (const std::vector<int>& v);
    double      avgVector   (const std::vector<int>& v);
    int         maxVector   (const std::vector<int>& v);
    int         minVector   (const std::vector<int>& v);

    // ---- Library info ----
    std::string version     ();
    void        printBanner ();

} // namespace MyLib
