#include "MathUtils.h"

// ============================================================
// MathUtils.cpp — C-style math implementations
// ============================================================

int math_add(int a, int b)          { return a + b;        }
int math_subtract(int a, int b)     { return a - b;        }
int math_multiply(int a, int b)     { return a * b;        }
float math_divide(float a, float b) { return b != 0 ? a/b : 0.0f; }
int math_clamp(int v, int lo, int hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}
