#pragma once   // include guard — prevents multiple inclusion

#include <string>

// ============================================================
// MathUtils.h — Math utility function declarations
// ============================================================
// This header declares WHAT functions exist and their signatures.
// The IMPLEMENTATION lives in MathUtils.cpp
// ============================================================

namespace MathUtils
{
    // Basic operations
    int    add       (int a, int b);
    int    subtract  (int a, int b);
    int    multiply  (int a, int b);
    double divide    (double a, double b);

    // Powers and roots
    double power     (double base, int exp);
    double squareRoot(double x);

    // Utility
    int    factorial (int n);
    bool   isPrime   (int n);
    int    clamp     (int value, int lo, int hi);

    // Constants
    constexpr double PI      = 3.14159265358979;
    constexpr double EULER   = 2.71828182845905;
}
