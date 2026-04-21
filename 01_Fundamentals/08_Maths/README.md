# Maths — C++23

## Headers overview

```mermaid
flowchart LR
    subgraph Headers ["C++23 Math Headers"]
        cmath["&lt;cmath&gt;\npow, sqrt, sin, cos\nlog, exp, floor, ceil\nabs, fmod, hypot"]
        numbers["&lt;numbers&gt; C++20\nstd::numbers::pi\nstd::numbers::e\nstd::numbers::sqrt2"]
        numeric["&lt;numeric&gt;\naccumulate, reduce\ninner_product, iota\ngcd, lcm"]
        complex["&lt;complex&gt;\nstd::complex&lt;T&gt;\nabs, arg, conj, norm"]
        bit["&lt;bit&gt; C++20\npopcount, countl_zero\nbit_ceil, bit_floor\nrotl, rotr"]
        algorithm["&lt;algorithm&gt;\nstd::min, max\nstd::clamp\nstd::minmax"]
    end
```

---

## 1. std::numbers — complete list (C++20)

```cpp
#include <numbers>

std::numbers::e           // 2.71828182845905...  Euler's number
std::numbers::log2e       // log₂(e)
std::numbers::log10e      // log₁₀(e)
std::numbers::pi          // 3.14159265358979...
std::numbers::inv_pi      // 1/π
std::numbers::inv_sqrtpi  // 1/√π
std::numbers::ln2         // ln(2)
std::numbers::ln10        // ln(10)
std::numbers::sqrt2       // √2
std::numbers::sqrt3       // √3
std::numbers::inv_sqrt3   // 1/√3
std::numbers::phi         // 1.61803398874989... golden ratio
std::numbers::egamma      // Euler–Mascheroni constant

// Type-specific precision via _v<T>
std::numbers::pi_v<float>        // float
std::numbers::pi_v<double>       // double (default)
std::numbers::pi_v<long double>  // max precision
```

> **Note:** `inv_sqrt2` does NOT exist — use `inv_sqrt3` or `1.0/std::numbers::sqrt2`.

---

## 2. `<cmath>` — core math functions

```mermaid
flowchart TD
    subgraph Power ["Power & Roots"]
        P1["pow(x, y)   → xʸ"]
        P2["sqrt(x)     → √x"]
        P3["cbrt(x)     → ∛x"]
        P4["hypot(x, y) → √(x²+y²)"]
    end

    subgraph Log ["Logarithms & Exp"]
        L1["exp(x)   → eˣ"]
        L2["exp2(x)  → 2ˣ"]
        L3["log(x)   → ln(x)"]
        L4["log2(x)  → log₂(x)"]
        L5["log10(x) → log₁₀(x)"]
        L6["log1p(x) → log(1+x) accurate"]
        L7["expm1(x) → exp(x)-1 accurate"]
    end

    subgraph Trig ["Trigonometry"]
        T1["sin/cos/tan   → radians in"]
        T2["asin/acos/atan→ radians out"]
        T3["atan2(y, x)   → all quadrants"]
        T4["sinh/cosh/tanh→ hyperbolic"]
    end

    subgraph Round ["Rounding"]
        R1["floor(x) → round DOWN"]
        R2["ceil(x)  → round UP"]
        R3["round(x) → nearest (.5 away)"]
        R4["trunc(x) → toward zero"]
    end
```

---

## 3. Trigonometry — degrees vs radians

```mermaid
flowchart LR
    degrees["Degrees\n0° — 360°"] -- "× π/180" --> radians["Radians\n0 — 2π"]
    radians -- "× 180/π" --> degrees

    note["All C++ trig functions\nwork in RADIANS"]
    style note fill:#2a2a2a,color:#aaa
```

```cpp
auto toRad = [](double deg) { return deg * std::numbers::pi / 180.0; };
auto toDeg = [](double rad) { return rad * 180.0 / std::numbers::pi; };

std::sin(toRad(30))  // 0.5
std::cos(toRad(60))  // 0.5
std::tan(toRad(45))  // 1.0

// atan2 — correct angle for all four quadrants
std::atan2(y, x)     // returns angle in radians [-π, π]
```

---

## 4. Rounding comparison

| Function | 3.2 | 3.5 | 3.7 | -3.2 | -3.7 |
|----------|:---:|:---:|:---:|:----:|:----:|
| `floor` | 3 | 3 | 3 | -4 | -4 |
| `ceil` | 4 | 4 | 4 | -3 | -3 |
| `round` | 3 | 4 | 4 | -3 | -4 |
| `trunc` | 3 | 3 | 3 | -3 | -3 |

---

## 5. Min / Max / Clamp

```cpp
std::min(3, 7)              // 3
std::max(3, 7)              // 7
std::clamp(150, 0, 100)     // 100  — clamped to hi
std::clamp(-5,  0, 100)     //   0  — clamped to lo
std::clamp(50,  0, 100)     //  50  — within range

std::min({3, 1, 4, 1, 5})   // 1  — initializer list
std::max({3, 1, 4, 1, 5})   // 5

auto [lo, hi] = std::minmax(42, 17);   // lo=17, hi=42
```

---

## 6. GCD and LCM (C++17)

```mermaid
flowchart LR
    subgraph GCD ["std::gcd"]
        G1["gcd(12, 8)  = 4"]
        G2["gcd(100,75) = 25"]
        G3["gcd(17, 13) = 1  (coprime)"]
    end

    subgraph LCM ["std::lcm"]
        L1["lcm(4, 6)   = 12"]
        L2["lcm(12, 18) = 36"]
        L3["lcm(7, 5)   = 35  (coprime)"]
    end
```

---

## 7. std::complex

```cpp
#include <complex>
using namespace std::complex_literals;

std::complex<double> z1 = {3.0, 4.0};  // 3 + 4i
std::complex<double> z2 = 2.0 + 3.0i; // literal syntax

std::abs(z1)    // magnitude = √(3²+4²) = 5
std::arg(z1)    // angle in radians
std::conj(z1)   // conjugate: 3 - 4i
std::norm(z1)   // |z|² = 25
```

---

## 8. `<numeric>` algorithms

```mermaid
flowchart TD
    subgraph Numeric ["std::numeric algorithms"]
        A["accumulate  — sum / product of range"]
        B["reduce      — parallel-friendly C++17"]
        C["inner_product — dot product of two ranges"]
        D["iota        — fill with sequential values"]
        E["partial_sum — running total"]
        F["adjacent_difference — consecutive differences"]
    end
```

```cpp
std::accumulate(v.begin(), v.end(), 0)          // sum
std::accumulate(v.begin(), v.end(), 1,
    std::multiplies<int>())                      // product
std::reduce(v.begin(), v.end(), 0)              // parallel sum
std::inner_product(a.begin(),a.end(),b.begin(),0) // dot product
std::iota(seq.begin(), seq.end(), 1)            // {1,2,3,...}
```

---

## 9. Bit operations (C++20)

```mermaid
flowchart LR
    subgraph Bits ["&lt;bit&gt; C++20"]
        B1["popcount(n)    → count 1-bits"]
        B2["countl_zero(n) → leading zeros"]
        B3["countr_zero(n) → trailing zeros"]
        B4["has_single_bit → is power of 2?"]
        B5["bit_ceil(n)    → next power of 2 ≥ n"]
        B6["bit_floor(n)   → largest power of 2 ≤ n"]
        B7["bit_width(n)   → bits needed"]
        B8["rotl/rotr      → bitwise rotation"]
    end
```

---

## Quick reference table

| Need | Function | Header |
|------|----------|--------|
| π, e, √2, φ | `std::numbers::pi` etc. | `<numbers>` |
| Power xʸ | `std::pow(x, y)` | `<cmath>` |
| Square root | `std::sqrt(x)` | `<cmath>` |
| Cube root | `std::cbrt(x)` | `<cmath>` |
| Hypotenuse | `std::hypot(x, y)` | `<cmath>` |
| sin/cos/tan | `std::sin/cos/tan(rad)` | `<cmath>` |
| Angle from point | `std::atan2(y, x)` | `<cmath>` |
| Natural log | `std::log(x)` | `<cmath>` |
| Log base 2 | `std::log2(x)` | `<cmath>` |
| Round down | `std::floor(x)` | `<cmath>` |
| Round up | `std::ceil(x)` | `<cmath>` |
| Round nearest | `std::round(x)` | `<cmath>` |
| Min/Max | `std::min/max(a, b)` | `<algorithm>` |
| Clamp | `std::clamp(v, lo, hi)` | `<algorithm>` |
| GCD | `std::gcd(a, b)` | `<numeric>` |
| LCM | `std::lcm(a, b)` | `<numeric>` |
| Sum of range | `std::accumulate` | `<numeric>` |
| Dot product | `std::inner_product` | `<numeric>` |
| Fill sequential | `std::iota` | `<numeric>` |
| Complex numbers | `std::complex<T>` | `<complex>` |
| Count 1-bits | `std::popcount` | `<bit>` |
| Power of 2? | `std::has_single_bit` | `<bit>` |
