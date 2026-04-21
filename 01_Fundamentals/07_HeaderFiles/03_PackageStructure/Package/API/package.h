#pragma once

#include <iostream>
#include <string>

// ============================================================
// Package/API/package.h
// ============================================================
// This header lives in a subdirectory: Package/API/
// Included via:  #include "Package/API/package.h"
//
// Benefits of organizing headers into directories:
//   - Logical grouping of related classes
//   - Avoids name collisions between different packages
//   - Clear public API boundary (Package/API/ = public)
//   - Mirrors how real libraries structure their headers
// ============================================================

namespace Package {

    class PackageClass {
    public:
        int         first  = 12;
        std::string second = "Honorably";

        // Parameterized constructor
        PackageClass(int one, const std::string& two)
            : first(one), second(two) {}

        void display() const {
            std::cout << "PackageClass: " << first
                      << " | " << second << '\n';
        }

        ~PackageClass() {}
    };


    class PackageUtils {
    public:
        static int    add      (int a, int b)           { return a + b; }
        static bool   isEmpty  (const std::string& s)   { return s.empty(); }
        static std::string version()                     { return "1.0.0"; }
    };

} // namespace Package
