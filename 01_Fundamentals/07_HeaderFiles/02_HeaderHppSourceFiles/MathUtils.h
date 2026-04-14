#pragma once

// ============================================================
// MathUtils.h — Plain C-style utility declarations
// ============================================================
// USE .h WHEN:
//   - The header could be shared with C code
//   - No C++ specific features (no templates, no classes)
//   - Simple function declarations and constants
// ============================================================

// Works in both C and C++
int   math_add     (int a, int b);
int   math_subtract(int a, int b);
int   math_multiply(int a, int b);
float math_divide  (float a, float b);
int   math_clamp   (int value, int lo, int hi);

// C-compatible constant
#define MATH_PI 3.14159265f
