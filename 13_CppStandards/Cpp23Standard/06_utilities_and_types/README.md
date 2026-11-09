# C++23: Utilities and Types

Back to [main README](README.md)

## Function objects and callables

| Feature | Header | Standard |
|---|---|---|
| `std::move_only_function` | `<functional>` | C++23 (new) |
| `std::bind_back` | `<functional>` | C++23 (new) |
| `std::invoke_r` | `<functional>` | C++23 (new) |

## Enums, bits and memory

| Feature | Header | Standard |
|---|---|---|
| `std::to_underlying` | `<utility>` | C++23 (new) |
| `std::is_scoped_enum` | `<type_traits>` | C++23 (new) |
| `std::byteswap` | `<bit>` | C++23 (new) |
| `std::out_ptr`, `std::inout_ptr` | `<memory>` | C++23 (new) |
| `std::start_lifetime_as`, `std::start_lifetime_as_array` | `<memory>` | C++23 (new) |

## Type traits and forwarding

| Feature | Header | Standard |
|---|---|---|
| `std::is_implicit_lifetime` | `<type_traits>` | C++23 (new) |
| `std::reference_constructs_from_temporary`, `std::reference_converts_from_temporary` | `<type_traits>` | C++23 (new) |
| `std::forward_like` | `<utility>` | C++23 (new) |
| `std::unreachable` | `<utility>` | C++23 (new) |

## Extended vocabulary types

| Feature | Header | Standard |
|---|---|---|
| `std::optional::and_then`, `or_else`, `transform` | `<optional>` | C++23 (extends C++17) |
| `std::visit` for types derived from `std::variant` | `<variant>` | C++23 (extends C++17) |
| `std::pair` / `std::tuple` with tuple-like types | `<utility>`, `<tuple>` | C++23 (extends C++11) |
| constexpr `std::unique_ptr`, `std::bitset` | `<memory>`, `<bitset>` | C++23 (extends earlier) |

## Notes

- `std::move_only_function` can hold callables that cannot be copied, such as lambdas capturing a `std::unique_ptr`.
- `std::copyable_function` and `std::function_ref` are **C++26**.
- `std::out_ptr` / `std::inout_ptr` help when a C API returns a pointer through an `T**` argument and you want a smart pointer to own it.
- `std::start_lifetime_as` is the well-defined replacement for many `reinterpret_cast` tricks when reading bytes from a buffer.

## Example

```cpp
#include <functional>
#include <memory>
#include <optional>
#include <print>
#include <utility>

enum class Color : unsigned char { Red = 1, Green = 2 };

int main() {
    // to_underlying
    auto n = std::to_underlying(Color::Green);
    std::println("{}", n);   // 2

    // move_only_function with a move-only capture
    std::move_only_function<int()> f = [p = std::make_unique<int>(7)] { return *p; };
    std::println("{}", f());   // 7

    // optional monadic operations
    std::optional<int> o = 5;
    auto r = o.transform([](int x) { return x * 2; })
              .and_then([](int x) -> std::optional<int> { return x > 5 ? std::optional{x} : std::nullopt; });
    std::println("{}", r.value_or(-1));   // 10
}
```
