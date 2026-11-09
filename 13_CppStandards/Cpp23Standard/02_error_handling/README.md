# C++23: Error Handling and Diagnostics

Back to [main README](README.md)

| Feature | Header | Standard |
|---|---|---|
| `std::expected<T, E>` | `<expected>` | C++23 (new) |
| `std::unexpected`, `std::unexpect`, `std::bad_expected_access` | `<expected>` | C++23 (new) |
| `std::unreachable()` | `<utility>` | C++23 (new) |
| `std::stacktrace`, `std::stacktrace_entry`, `std::basic_stacktrace` | `<stacktrace>` | C++23 (new) |

## Notes

- `std::expected` holds either a value or an error and has monadic operations: `and_then`, `or_else`, `transform`, `transform_error`.
- `std::unreachable()` tells the optimizer a code path can never be reached. Reaching it is undefined behavior.
- `std::stacktrace` usually needs an extra link flag with GCC (for example `-lstdc++exp` on GCC 14, `-lstdc++_libbacktrace` on GCC 12/13).

## Example

```cpp
#include <charconv>
#include <expected>
#include <print>
#include <string>
#include <string_view>
#include <system_error>

std::expected<int, std::string> parse(std::string_view s) {
    int value{};
    auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), value);
    if (ec != std::errc{}) {
        return std::unexpected("not a number");
    }
    return value;
}

int main() {
    auto r = parse("42").transform([](int x) { return x * 2; });
    if (r) {
        std::println("{}", *r);        // 84
    } else {
        std::println("{}", r.error());
    }
}
```
