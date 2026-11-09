# C++23: Ranges

Back to [main README](README.md)

## Views and adaptors (`<ranges>`)

| Feature | Standard |
|---|---|
| `views::zip`, `views::zip_transform` | C++23 (new) |
| `views::enumerate` | C++23 (new) |
| `views::adjacent<N>`, `views::adjacent_transform<N>`, `views::pairwise` | C++23 (new) |
| `views::chunk`, `views::slide`, `views::chunk_by` | C++23 (new) |
| `views::stride` | C++23 (new) |
| `views::join_with` | C++23 (new) |
| `views::cartesian_product` | C++23 (new) |
| `views::repeat` | C++23 (new) |
| `views::as_rvalue`, `views::as_const` | C++23 (new) |
| `std::ranges::range_adaptor_closure` | C++23 (new) |
| `std::ranges::elements_of` | C++23 (new) |

## Conversion and generators

| Feature | Header | Standard |
|---|---|---|
| `std::ranges::to<Container>()` | `<ranges>` | C++23 (new) |
| `std::generator<T>` | `<generator>` | C++23 (new) |
| `std::const_iterator`, `std::basic_const_iterator`, `ranges::constant_range` | `<iterator>`, `<ranges>` | C++23 (new) |

## Algorithms

| Feature | Header | Standard |
|---|---|---|
| `ranges::contains`, `ranges::contains_subrange` | `<algorithm>` | C++23 (new) |
| `ranges::starts_with`, `ranges::ends_with` | `<algorithm>` | C++23 (new) |
| `ranges::find_last`, `find_last_if`, `find_last_if_not` | `<algorithm>` | C++23 (new) |
| `ranges::fold_left`, `fold_left_first`, `fold_right`, `fold_right_last` (+ `_with_iter`) | `<algorithm>` | C++23 (new) |
| `ranges::shift_left`, `ranges::shift_right` | `<algorithm>` | C++23 (new) |
| `ranges::iota` | `<numeric>` | C++23 (new) |

## Notes

- The core of ranges (`std::ranges`, `views::filter`, `views::transform`, `views::take`, ...) is **C++20**. C++23 fills the gaps.
- `views::concat` and `views::to_input` are **C++26**, not C++23.
- `std::generator` is a synchronous coroutine generator. It needs a coroutine-capable compiler setup.

## Example

```cpp
#include <print>
#include <ranges>
#include <vector>

int main() {
    std::vector<int> a{1, 2, 3};
    std::vector<char> b{'x', 'y', 'z'};

    for (auto [i, v] : a | std::views::enumerate) {
        std::println("{}: {}", i, v);
    }

    for (auto [x, y] : std::views::zip(a, b)) {
        std::println("{} {}", x, y);
    }

    auto evens = std::views::iota(1, 10)
               | std::views::filter([](int n) { return n % 2 == 0; })
               | std::ranges::to<std::vector>();
    std::println("{}", evens);   // [2, 4, 6, 8]
}
```
