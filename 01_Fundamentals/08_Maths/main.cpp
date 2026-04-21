#include <iostream>
#include <cmath>
#include <numbers>
#include <numeric>
#include <algorithm>
#include <vector>
#include <complex>
#include <bit>
#include <print>

// ============================================================
// MATHS in C++23
// ============================================================
// Topics covered:
//   1.  std::numbers     — built-in math constants (C++20)
//   2.  <cmath> basics   — pow, sqrt, abs, ceil, floor, round
//   3.  Trigonometry     — sin, cos, tan, asin, acos, atan2
//   4.  Logarithms       — log, log2, log10, exp
//   5.  Rounding         — floor, ceil, round, trunc
//   6.  Min / Max / Clamp— std::min, max, clamp
//   7.  GCD / LCM        — std::gcd, std::lcm (C++17)
//   8.  std::complex     — complex numbers
//   9.  std::numeric     — accumulate, inner_product, iota
//  10.  Bit operations   — std::popcount, std::countl_zero (C++20)
// ============================================================


// ============================================================
// Helper — print a separator
// ============================================================
void section(const std::string& title) {
    std::cout << "\n=====================================================\n";
    std::cout << title << '\n';
    std::cout << "=====================================================\n";
}


int main()
{
    // -------------------------------------------------------
    section("Example 1: std::numbers — built-in constants (C++20)");

    // Full list of std::numbers constants:
    std::println("e           = {:.15f}", std::numbers::e);
    std::println("log2e       = {:.15f}", std::numbers::log2e);
    std::println("log10e      = {:.15f}", std::numbers::log10e);
    std::println("pi          = {:.15f}", std::numbers::pi);
    std::println("inv_pi      = {:.15f}", std::numbers::inv_pi);
    std::println("inv_sqrtpi  = {:.15f}", std::numbers::inv_sqrtpi);
    std::println("ln2         = {:.15f}", std::numbers::ln2);
    std::println("ln10        = {:.15f}", std::numbers::ln10);
    std::println("sqrt2       = {:.15f}", std::numbers::sqrt2);
    std::println("sqrt3       = {:.15f}", std::numbers::sqrt3);
    std::println("inv_sqrt3   = {:.15f}", std::numbers::inv_sqrt3);
    std::println("phi (golden)= {:.15f}", std::numbers::phi);
    std::println("egamma      = {:.15f}", std::numbers::egamma);

    // Type-specific precision
    std::println("\npi as float:       {:.7f}",  std::numbers::pi_v<float>);
    std::println("pi as double:      {:.15f}", std::numbers::pi_v<double>);
    std::println("pi as long double: {:.18Lf}", std::numbers::pi_v<long double>);

    // Practical use
    double r    = 5.0;
    double area = std::numbers::pi * r * r;
    double circ = 2.0 * std::numbers::pi * r;
    std::println("\nCircle r=5:  area={:.4f}  circumference={:.4f}", area, circ);


    // -------------------------------------------------------
    section("Example 2: <cmath> basics");

    std::println("pow(2, 10)       = {:.0f}", std::pow(2.0, 10.0));
    std::println("pow(3.0, 0.5)    = {:.6f}", std::pow(3.0, 0.5));
    std::println("sqrt(144)        = {:.1f}", std::sqrt(144.0));
    std::println("cbrt(27)         = {:.1f}", std::cbrt(27.0));
    std::println("hypot(3, 4)      = {:.1f}", std::hypot(3.0, 4.0));
    std::println("abs(-42)         = {}",     std::abs(-42));
    std::println("fabs(-3.14)      = {:.2f}", std::fabs(-3.14));
    std::println("fmod(10.5, 3.2)  = {:.4f}", std::fmod(10.5, 3.2));

    std::println("\nstd::isinf(1/0.0):   {}", std::isinf(1.0/0.0));
    std::println("std::isnan(0.0/0.0): {}", std::isnan(0.0/0.0));
    std::println("std::isfinite(3.14): {}", std::isfinite(3.14));


    // -------------------------------------------------------
    section("Example 3: Trigonometry");

    auto toRad = [](double deg) { return deg * std::numbers::pi / 180.0; };
    auto toDeg = [](double rad) { return rad * 180.0 / std::numbers::pi; };

    std::println("sin(30°) = {:.4f}   (expected 0.5)",    std::sin(toRad(30)));
    std::println("cos(60°) = {:.4f}   (expected 0.5)",    std::cos(toRad(60)));
    std::println("tan(45°) = {:.4f}   (expected 1.0)",    std::tan(toRad(45)));
    std::println("sin(90°) = {:.4f}   (expected 1.0)",    std::sin(toRad(90)));
    std::println("cos(0°)  = {:.4f}   (expected 1.0)",    std::cos(toRad(0)));

    std::println("\nasin(0.5) = {:.2f}° (expected 30°)", toDeg(std::asin(0.5)));
    std::println("acos(0.5) = {:.2f}° (expected 60°)", toDeg(std::acos(0.5)));
    std::println("atan(1.0) = {:.2f}° (expected 45°)", toDeg(std::atan(1.0)));

    std::println("\natan2(1, 1)   = {:.2f}° (expected 45°)",  toDeg(std::atan2(1.0, 1.0)));
    std::println("atan2(1, -1)  = {:.2f}° (expected 135°)", toDeg(std::atan2(1.0, -1.0)));
    std::println("atan2(-1, -1) = {:.2f}° (expected -135°)",toDeg(std::atan2(-1.0,-1.0)));

    std::println("\nsinh(1) = {:.6f}", std::sinh(1.0));
    std::println("cosh(1) = {:.6f}", std::cosh(1.0));
    std::println("tanh(1) = {:.6f}", std::tanh(1.0));

    double x1=0, y1=0, x2=3, y2=4;
    double dist  = std::hypot(x2-x1, y2-y1);
    double angle = toDeg(std::atan2(y2-y1, x2-x1));
    std::println("\nDistance (0,0)→(3,4) = {:.1f}", dist);
    std::println("Angle of vector (3,4) = {:.2f}°", angle);


    // -------------------------------------------------------
    section("Example 4: Logarithms and Exponentials");

    std::println("exp(1)         = {:.6f}  (e¹)",    std::exp(1.0));
    std::println("exp(2)         = {:.6f}  (e²)",    std::exp(2.0));
    std::println("exp2(10)       = {:.0f}  (2¹⁰)",  std::exp2(10.0));

    std::println("\nlog(e)         = {:.6f}  (natural log)", std::log(std::numbers::e));
    std::println("log(1)         = {:.6f}",                  std::log(1.0));
    std::println("log10(100)     = {:.6f}  (log base 10)",   std::log10(100.0));
    std::println("log10(1000)    = {:.6f}",                   std::log10(1000.0));
    std::println("log2(1024)     = {:.6f}  (log base 2)",    std::log2(1024.0));
    std::println("log2(65536)    = {:.6f}",                   std::log2(65536.0));

    std::println("\nlog1p(0.0001)  = {:.10f}  (log(1+x) accurate)", std::log1p(0.0001));
    std::println("expm1(0.0001)  = {:.10f}  (exp(x)-1 accurate)", std::expm1(0.0001));


    // -------------------------------------------------------
    section("Example 5: Rounding functions");

    double val = 3.7;
    std::println("value: {}", val);
    std::println("  floor({})  = {}  (round DOWN)", val, std::floor(val));
    std::println("  ceil({})   = {}  (round UP)",   val, std::ceil(val));
    std::println("  round({})  = {}  (nearest, .5 rounds away)", val, std::round(val));
    std::println("  trunc({})  = {}  (truncate toward zero)", val, std::trunc(val));

    double neg = -3.7;
    std::println("\nvalue: {}", neg);
    std::println("  floor({}) = {}  (round DOWN)", neg, std::floor(neg));
    std::println("  ceil({})  = {}  (round UP)",   neg, std::ceil(neg));
    std::println("  round({}) = {}  (nearest)", neg, std::round(neg));
    std::println("  trunc({}) = {}  (toward zero)", neg, std::trunc(neg));

    std::println("\nnearbyint(3.5) = {}", std::nearbyint(3.5));
    std::println("nearbyint(4.5) = {}", std::nearbyint(4.5));


    // -------------------------------------------------------
    section("Example 6: Min / Max / Clamp");

    std::println("min(3, 7)            = {}", std::min(3, 7));
    std::println("max(3, 7)            = {}", std::max(3, 7));
    std::println("clamp(150, 0, 100)   = {}", std::clamp(150, 0, 100));
    std::println("clamp(-5,  0, 100)   = {}", std::clamp(-5,  0, 100));
    std::println("clamp(50,  0, 100)   = {}", std::clamp(50,  0, 100));

    std::println("\nmin({{3,1,4,1,5,9}}) = {}", std::min({3,1,4,1,5,9}));
    std::println("max({{3,1,4,1,5,9}}) = {}", std::max({3,1,4,1,5,9}));

    auto [lo, hi] = std::minmax(42, 17);
    std::println("minmax(42,17) → min={} max={}", lo, hi);

    auto normalize = [](double v, double lo, double hi) {
        return (std::clamp(v, lo, hi) - lo) / (hi - lo);
    };
    std::println("\nnormalize(75,  0, 100) = {:.2f}", normalize(75,  0, 100));
    std::println("normalize(150, 0, 100) = {:.2f}", normalize(150, 0, 100));
    std::println("normalize(-10, 0, 100) = {:.2f}", normalize(-10, 0, 100));


    // -------------------------------------------------------
    section("Example 7: GCD and LCM (C++17 — <numeric>)");

    std::println("gcd(12, 8)    = {}", std::gcd(12, 8));
    std::println("gcd(100, 75)  = {}", std::gcd(100, 75));
    std::println("gcd(17, 13)   = {}  (coprime)", std::gcd(17, 13));

    std::println("\nlcm(4, 6)     = {}", std::lcm(4, 6));
    std::println("lcm(12, 18)   = {}", std::lcm(12, 18));
    std::println("lcm(7, 5)     = {}  (coprime)", std::lcm(7, 5));

    int rate1 = 44100, rate2 = 48000;
    std::println("\ngcd({}, {}) = {}", rate1, rate2, std::gcd(rate1, rate2));
    std::println("lcm({}, {}) = {}", rate1, rate2, std::lcm(rate1, rate2));


    // -------------------------------------------------------
    section("Example 8: std::complex — complex numbers");

    using namespace std::complex_literals;

    std::complex<double> z1 = {3.0, 4.0};
    std::complex<double> z2 = {1.0, -2.0};

    std::println("z1 = {} + {}i",  z1.real(), z1.imag());
    std::println("z2 = {} + {}i",  z2.real(), z2.imag());

    auto z3 = z1 + z2;
    auto z4 = z1 * z2;
    std::println("\nz1 + z2 = {} + {}i", z3.real(), z3.imag());
    std::println("z1 * z2 = {} + {}i", z4.real(), z4.imag());

    std::println("\n|z1| (magnitude) = {:.4f}", std::abs(z1));
    std::println("arg(z1) (angle)  = {:.4f} rad", std::arg(z1));
    std::println("conj(z1)         = {} + {}i", std::conj(z1).real(), std::conj(z1).imag());
    std::println("norm(z1)         = {:.1f}",   std::norm(z1));

    auto z5 = 2.0 + 3.0i;
    std::println("\n2.0 + 3.0i = {} + {}i", z5.real(), z5.imag());


    // -------------------------------------------------------
    section("Example 9: <numeric> — algorithms on numbers");

    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    int sum = std::accumulate(v.begin(), v.end(), 0);
    std::println("sum [1..10] = {}", sum);

    int product = std::accumulate(v.begin(), v.end(), 1, std::multiplies<int>());
    std::println("product [1..10] = {}", product);

    std::vector<int> a = {1, 2, 3};
    std::vector<int> b = {4, 5, 6};
    int dot = std::inner_product(a.begin(), a.end(), b.begin(), 0);
    std::println("\ndot([1,2,3], [4,5,6]) = {}  (1×4 + 2×5 + 3×6)", dot);

    std::vector<int> seq(10);
    std::iota(seq.begin(), seq.end(), 1);
    std::cout << "iota(1..10):   ";
    for (int x : seq) std::cout << x << ' ';
    std::cout << '\n';

    std::vector<int> running(10);
    std::partial_sum(seq.begin(), seq.end(), running.begin());
    std::cout << "partial_sum:   ";
    for (int x : running) std::cout << x << ' ';
    std::cout << '\n';

    std::vector<int> diffs(10);
    std::adjacent_difference(seq.begin(), seq.end(), diffs.begin());
    std::cout << "adjacent_diff: ";
    for (int x : diffs) std::cout << x << ' ';
    std::cout << '\n';

    int total = std::reduce(v.begin(), v.end(), 0);
    std::println("reduce sum: {}", total);


    // -------------------------------------------------------
    section("Example 10: Bit operations (C++20 — <bit>)");

    uint32_t n = 0b10110100;

    std::println("n = {} (binary: {:08b})", n, n);
    std::println("popcount(n)       = {}  (count of 1-bits)",    std::popcount(n));
    std::println("countl_zero(n)    = {}  (leading zeros)",      std::countl_zero(n));
    std::println("countr_zero(n)    = {}  (trailing zeros)",     std::countr_zero(n));
    std::println("has_single_bit(4) = {}  (is power of 2?)",     std::has_single_bit(4u));
    std::println("has_single_bit(6) = {}  (is power of 2?)",     std::has_single_bit(6u));
    std::println("bit_ceil(5)       = {}  (next power of 2 ≥ 5)",std::bit_ceil(5u));
    std::println("bit_floor(5)      = {}  (largest power of 2 ≤ 5)", std::bit_floor(5u));
    std::println("bit_width(100)    = {}  (bits needed)",        std::bit_width(100u));

    uint8_t byte = 0b10110001;
    std::println("\nbyte           = {:08b}", byte);
    std::println("rotl(byte, 2)  = {:08b}", std::rotl(byte, 2));
    std::println("rotr(byte, 2)  = {:08b}", std::rotr(byte, 2));

    std::cout << "\n=== Maths complete ===\n";
    return 0;
}
