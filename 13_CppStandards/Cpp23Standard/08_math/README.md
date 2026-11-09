# C++23: Math and Numerics

Back to [main README](README.md)

| Feature | Header | Standard |
|---|---|---|
| `std::float16_t`, `std::float32_t`, `std::float64_t`, `std::float128_t`, `std::bfloat16_t` | `<stdfloat>` | C++23 (new, optional) |
| constexpr for selected `<cmath>` / `<cstdlib>` functions | `<cmath>`, `<cstdlib>` | C++23 (extends C++11/17) |
| `ranges::iota` | `<numeric>` | C++23 (new) |
| `ranges::fold_left` and related folds | `<algorithm>` | C++23 (new) |
| `std::mdspan` (multidimensional data) | `<mdspan>` | C++23 (new) |

## Notes

- The `<stdfloat>` types are **optional**. Check the macros `__STDCPP_FLOAT16_T__`, `__STDCPP_FLOAT32_T__`, `__STDCPP_FLOAT64_T__`, `__STDCPP_FLOAT128_T__`, `__STDCPP_BFLOAT16_T__` before using them.
- Literal suffixes: `1.5f32`, `1.5f64`, `1.5bf16`, ...
- constexpr `<cmath>` only covers functions whose result does not depend on the rounding mode or `errno`, for example `std::abs`, `std::floor`, `std::ceil`, `std::fmod`, `std::copysign`, `std::fma`.
- Not C++23: `std::numbers` and `std::midpoint` / `std::lerp` (C++20), `std::linalg`, `std::simd`, `std::saturating_add` (C++26).

## Example

```cpp
#include <cmath>
#include <numeric>
#include <print>
#include <ranges>
#include <stdfloat>
#include <vector>
#include <algorithm>

int main() {
#ifdef __STDCPP_FLOAT32_T__
    std::float32_t x = 1.5f32;
    std::println("{}", x);
#endif

    constexpr double d = std::floor(2.7);   // constexpr cmath
    std::println("{}", d);                  // 2

    std::vector<int> v{1, 2, 3, 4};
    auto sum = std::ranges::fold_left(v, 0, std::plus<>{});
    std::println("{}", sum);                // 10
}
```
