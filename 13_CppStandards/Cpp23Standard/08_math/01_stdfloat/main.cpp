/*
 * <stdfloat>: std::float16_t, std::bfloat16_t, std::float32_t, std::float64_t,
 * std::float128_t (C++23)
 *
 * Fixed-width floating point types. Each one is OPTIONAL: the compiler defines
 * a macro such as __STDCPP_FLOAT16_T__ when the type is available.
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <print>
#include <stdfloat>

int main() {
    // === EXAMPLE 1: which types exist here? ===
    std::println("=== Example 1: availability ===");
#ifdef __STDCPP_FLOAT16_T__
    std::println("float16_t:  yes");
#else
    std::println("float16_t:  no");
#endif
#ifdef __STDCPP_BFLOAT16_T__
    std::println("bfloat16_t: yes");
#else
    std::println("bfloat16_t: no");
#endif
#ifdef __STDCPP_FLOAT32_T__
    std::println("float32_t:  yes");
#else
    std::println("float32_t:  no");
#endif
#ifdef __STDCPP_FLOAT64_T__
    std::println("float64_t:  yes");
#else
    std::println("float64_t:  no");
#endif
#ifdef __STDCPP_FLOAT128_T__
    std::println("float128_t: yes");
#else
    std::println("float128_t: no");
#endif

    // === EXAMPLE 2: same value, different precision ===
    std::println("");
    std::println("=== Example 2: precision ===");
#if defined(__STDCPP_FLOAT32_T__) && defined(__STDCPP_FLOAT64_T__)
    std::float32_t f32 = 0.1f32;
    std::float64_t f64 = 0.1f64;
    std::println("float32_t: {:.20f}", f32);
    std::println("float64_t: {:.20f}", f64);
    std::println("sizes: {} and {} bytes", sizeof(f32), sizeof(f64));
#endif
#ifdef __STDCPP_FLOAT16_T__
    std::float16_t f16 = 0.1f16;
    std::println("float16_t: {:.10f}, size {} bytes", f16, sizeof(f16));
#endif
    return 0;
}
