# C++23 Standard Library Cheat Sheet

Overview of the library features added or extended in **C++23** (ISO/IEC 14882:2023), with header and standard family.

## Legend

| Label | Meaning |
|---|---|
| **C++23 (new)** | Introduced in C++23 |
| **C++23 (extends C++20)** | Base feature exists since C++20, C++23 adds to it |
| **C++26** | Often confused with C++23, but NOT part of it |

## Category index

| Category | File |
|---|---|
| Formatted output | [README_formatted-output.md](README_formatted-output.md) |
| Error handling | [README_error-handling.md](README_error-handling.md) |
| Ranges | [README_ranges.md](README_ranges.md) |
| Containers and strings | [README_containers.md](README_containers.md) |
| Modules | [README_modules.md](README_modules.md) |
| Utilities and types | [README_utilities-and-types.md](README_utilities-and-types.md) |
| Concurrency | [README_concurrency.md](README_concurrency.md) |
| Math and numerics | [README_math.md](README_math.md) |

---

## Formatted output

| Feature | Header | Standard |
|---|---|---|
| `std::print` | `<print>` | C++23 (new) |
| `std::println` | `<print>` | C++23 (new) |
| `std::vprint_unicode`, `std::vprint_nonunicode` | `<print>` | C++23 (new) |
| `std::formatter` for ranges, `std::pair`, `std::tuple` | `<format>` | C++23 (extends C++20 `std::format`) |
| `std::range_formatter`, `std::format_kind` | `<format>` | C++23 (new) |
| `std::formattable` (concept) | `<format>` | C++23 (new) |

## Error handling

| Feature | Header | Standard |
|---|---|---|
| `std::expected<T, E>` | `<expected>` | C++23 (new) |
| `std::unexpected`, `std::unexpect`, `std::bad_expected_access` | `<expected>` | C++23 (new) |
| `std::unreachable()` | `<utility>` | C++23 (new) |
| `std::stacktrace`, `std::stacktrace_entry`, `std::basic_stacktrace` | `<stacktrace>` | C++23 (new) |

## Ranges

| Feature | Header | Standard |
|---|---|---|
| `std::ranges::to<Container>()` | `<ranges>` | C++23 (new) |
| `std::generator<T>` | `<generator>` | C++23 (new) |
| `views::zip`, `views::zip_transform` | `<ranges>` | C++23 (new) |
| `views::enumerate` | `<ranges>` | C++23 (new) |
| `views::adjacent`, `views::adjacent_transform`, `views::pairwise` | `<ranges>` | C++23 (new) |
| `views::chunk`, `views::slide`, `views::chunk_by` | `<ranges>` | C++23 (new) |
| `views::stride` | `<ranges>` | C++23 (new) |
| `views::join_with` | `<ranges>` | C++23 (new) |
| `views::cartesian_product` | `<ranges>` | C++23 (new) |
| `views::repeat` | `<ranges>` | C++23 (new) |
| `views::as_rvalue`, `views::as_const` | `<ranges>` | C++23 (new) |
| `std::ranges::range_adaptor_closure` | `<ranges>` | C++23 (new) |
| `std::ranges::elements_of` | `<ranges>` | C++23 (new) |
| `std::const_iterator`, `std::basic_const_iterator`, `ranges::constant_range` | `<iterator>`, `<ranges>` | C++23 (new) |
| `ranges::contains`, `ranges::contains_subrange` | `<algorithm>` | C++23 (new) |
| `ranges::starts_with`, `ranges::ends_with` | `<algorithm>` | C++23 (new) |
| `ranges::find_last`, `find_last_if`, `find_last_if_not` | `<algorithm>` | C++23 (new) |
| `ranges::fold_left`, `fold_left_first`, `fold_right`, `fold_right_last` (+ `_with_iter` variants) | `<algorithm>` | C++23 (new) |
| `ranges::shift_left`, `ranges::shift_right` | `<algorithm>` | C++23 (new) |
| `ranges::iota` | `<numeric>` | C++23 (new) |

## Containers and strings

| Feature | Header | Standard |
|---|---|---|
| `std::flat_map`, `std::flat_multimap` | `<flat_map>` | C++23 (new) |
| `std::flat_set`, `std::flat_multiset` | `<flat_set>` | C++23 (new) |
| `std::mdspan`, `std::extents`, `std::layout_*` | `<mdspan>` | C++23 (new) |
| `std::spanstream`, `ispanstream`, `ospanstream` | `<spanstream>` | C++23 (new) |
| `insert_range`, `append_range`, `assign_range`, `prepend_range`, `std::from_range` | container headers | C++23 (new) |
| Heterogeneous `erase` for associative containers | container headers | C++23 (new) |
| `std::string::contains`, `std::string_view::contains` | `<string>`, `<string_view>` | C++23 (new) |
| `std::string::resize_and_overwrite` | `<string>` | C++23 (new) |
| `basic_string(nullptr_t) = delete` | `<string>` | C++23 (new) |

## Modules

| Feature | Header | Standard |
|---|---|---|
| `import std;` | (module) | C++23 (new) |
| `import std.compat;` | (module) | C++23 (new) |

## Utilities and types

| Feature | Header | Standard |
|---|---|---|
| `std::move_only_function` | `<functional>` | C++23 (new) |
| `std::bind_back` | `<functional>` | C++23 (new) |
| `std::invoke_r` | `<functional>` | C++23 (new) |
| `std::to_underlying` | `<utility>` | C++23 (new) |
| `std::is_scoped_enum` | `<type_traits>` | C++23 (new) |
| `std::forward_like` | `<utility>` | C++23 (new) |
| `std::byteswap` | `<bit>` | C++23 (new) |
| `std::out_ptr`, `std::inout_ptr` | `<memory>` | C++23 (new) |
| `std::start_lifetime_as`, `start_lifetime_as_array` | `<memory>` | C++23 (new) |
| `std::is_implicit_lifetime` | `<type_traits>` | C++23 (new) |
| `std::reference_constructs_from_temporary`, `reference_converts_from_temporary` | `<type_traits>` | C++23 (new) |
| `std::optional::and_then`, `or_else`, `transform` | `<optional>` | C++23 (extends C++17) |
| `std::visit` for types derived from `std::variant` | `<variant>` | C++23 (extends C++17) |
| `std::pair` / `std::tuple` support for tuple-like types | `<utility>`, `<tuple>` | C++23 (extends C++11) |
| constexpr `std::unique_ptr`, `std::bitset` | `<memory>`, `<bitset>` | C++23 (extends earlier) |

## Concurrency

| Feature | Header | Standard |
|---|---|---|
| `<stdatomic.h>` (C-compatible atomics, `_Atomic(T)`) | `<stdatomic.h>` | C++23 (new) |

Concurrency is the smallest area in C++23. `std::jthread`, `std::stop_token`, `std::latch`, `std::barrier`, `std::semaphore` and `std::atomic_ref` are all **C++20**.

## Math and numerics

| Feature | Header | Standard |
|---|---|---|
| `std::float16_t`, `float32_t`, `float64_t`, `float128_t`, `bfloat16_t` | `<stdfloat>` | C++23 (new, optional) |
| constexpr for selected `<cmath>` / `<cstdlib>` functions | `<cmath>`, `<cstdlib>` | C++23 (extends C++11/17) |

---

## Often confused with C++23

| Feature | Real standard |
|---|---|
| `std::format` | C++20 |
| `std::span` | C++20 |
| `std::atomic_ref` | C++20 |
| `std::jthread`, `std::stop_token` | C++20 |
| `std::source_location` | C++20 |
| `std::copyable_function` | C++26 |
| `std::function_ref` | C++26 |
| `std::execution` (senders/receivers) | C++26 |
| `std::submdspan` | C++26 |
| `std::simd`, `std::linalg` | C++26 |
| `<stdbit.h>` helpers, `std::saturating_add` | C++26 |
| `std::hazard_pointer`, `std::rcu` | C++26 |

## Compiler support

Support differs a lot between GCC (libstdc++), Clang (libc++) and MSVC. `<print>` needs GCC 14+, for example. For anything else, check the
[cppreference compiler support table](https://en.cppreference.com/w/cpp/compiler_support/23) before relying on a feature.
