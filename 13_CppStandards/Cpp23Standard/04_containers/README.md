# C++23: Containers and Strings

Back to [main README](README.md)

## Containers and views

| Feature | Header | Standard |
|---|---|---|
| `std::flat_map`, `std::flat_multimap` | `<flat_map>` | C++23 (new) |
| `std::flat_set`, `std::flat_multiset` | `<flat_set>` | C++23 (new) |
| `std::mdspan`, `std::extents`, `std::dextents`, `std::layout_left/right/stride` | `<mdspan>` | C++23 (new) |
| `std::spanstream`, `std::ispanstream`, `std::ospanstream` | `<spanstream>` | C++23 (new) |

## Range-aware member functions

| Feature | Standard |
|---|---|
| `insert_range`, `append_range`, `assign_range`, `prepend_range` (`insert_range_after` for `forward_list`) | C++23 (new) |
| `std::from_range` constructor tag | C++23 (new) |
| Heterogeneous `erase` for associative containers | C++23 (new) |

## Strings

| Feature | Header | Standard |
|---|---|---|
| `std::string::contains`, `std::string_view::contains` | `<string>`, `<string_view>` | C++23 (new) |
| `std::string::resize_and_overwrite` | `<string>` | C++23 (new) |
| `basic_string(nullptr_t) = delete` | `<string>` | C++23 (new) |

## Notes

- `flat_map` / `flat_set` store their data in sorted contiguous containers (by default `std::vector`). They have fast iteration and a small memory footprint, but slow insertion in the middle.
- `std::mdspan` is a non-owning multidimensional view. It uses the C++23 multi-argument `operator[]`.
- `std::span` is **C++20**. Only `std::spanstream` is new in C++23.
- `std::submdspan` is **C++26**.

## Example

```cpp
#include <flat_map>
#include <mdspan>
#include <print>
#include <vector>

int main() {
    std::flat_map<int, int> fm{{1, 10}, {2, 20}};
    std::println("{}", fm.at(2));    // 20

    std::vector<int> data{1, 2, 3, 4, 5, 6};
    std::mdspan m(data.data(), 2, 3);   // 2 rows, 3 columns
    std::println("{}", m[1, 2]);        // 6
}
```
