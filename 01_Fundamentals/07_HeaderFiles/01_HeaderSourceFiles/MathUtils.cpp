#include "MathUtils.h"
#include <stdexcept>
#include <cmath>

// ============================================================
// MathUtils.cpp — Math utility function IMPLEMENTATIONS
// ============================================================
// Only this .cpp file needs to be compiled once.
// Any file that #includes "MathUtils.h" gets the declarations
// and links against this compiled object file.
// ============================================================

namespace MathUtils
{
    int add(int a, int b)
    {
        return a + b;
    }

    int subtract(int a, int b)
    {
        return a - b;
    }

    int multiply(int a, int b)
    {
        return a * b;
    }

    double divide(double a, double b)
    {
        if (b == 0.0)
            throw std::invalid_argument("divide: division by zero");
        return a / b;
    }

    double power(double base, int exp)
    {
        double result = 1.0;
        for (int i = 0; i < std::abs(exp); ++i)
            result *= base;
        return (exp >= 0) ? result : 1.0 / result;
    }

    double squareRoot(double x)
    {
        if (x < 0.0)
            throw std::invalid_argument("squareRoot: negative input");
        return std::sqrt(x);
    }

    int factorial(int n)
    {
        if (n < 0)  throw std::invalid_argument("factorial: negative input");
        if (n == 0) return 1;
        int result = 1;
        for (int i = 2; i <= n; ++i) result *= i;
        return result;
    }

    bool isPrime(int n)
    {
        if (n < 2) return false;
        for (int i = 2; i * i <= n; ++i)
            if (n % i == 0) return false;
        return true;
    }

    int clamp(int value, int lo, int hi)
    {
        if (value < lo) return lo;
        if (value > hi) return hi;
        return value;
    }
}
