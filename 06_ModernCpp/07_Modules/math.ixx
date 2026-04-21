export module Math;

// No #include here — avoid stdlib in module interface on GCC 14
// Only use built-in types: int, double, float, bool

export namespace Math {

    constexpr double PI    = 3.14159265358979;
    constexpr double EULER = 2.71828182845905;

    int    add      (int a, int b)          { return a + b;  }
    int    subtract (int a, int b)          { return a - b;  }
    int    multiply (int a, int b)          { return a * b;  }
    double divide   (double a, double b)    { return a / b;  }
    int    square   (int x)                 { return x * x;  }

    int clamp(int v, int lo, int hi) {
        if (v < lo) return lo;
        if (v > hi) return hi;
        return v;
    }

    int versionMajor() { return 1; }
    int versionMinor() { return 0; }
}

// Module-private — NOT visible outside
namespace Math::Internal {
    bool isValidDivisor(double d) { return d != 0.0; }
}
