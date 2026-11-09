# C++23: Formatted Output

Back to [main README](README.md)

| Feature | Header | Standard |
|---|---|---|
| `std::print` | `<print>` | C++23 (new) |
| `std::println` | `<print>` | C++23 (new) |
| `std::vprint_unicode`, `std::vprint_nonunicode` | `<print>` | C++23 (new) |
| `std::formatter` for ranges, `std::pair`, `std::tuple` | `<format>` | C++23 (extends C++20 `std::format`) |
| `std::range_formatter`, `std::format_kind` | `<format>` | C++23 (new) |
| `std::formattable` (concept) | `<format>` | C++23 (new) |

## Notes

- `std::print` and `std::println` use the `std::format` syntax. They write to `stdout` by default, or to a `FILE*` / `std::ostream` if you pass one as the first argument.
- `println` appends a newline.
- `std::format` itself is **C++20**. C++23 makes standard containers formattable directly.

## Example

```cpp
#include <map>
#include <print>
#include <string>
#include <vector>

int main() {
    std::vector<int> v{1, 2, 3};
    std::map<int, std::string> m{{1, "a"}, {2, "b"}};

    std::println("{}", v);   // [1, 2, 3]
    std::println("{}", m);   // {1: "a", 2: "b"}
    std::print(stderr, "error code {}\n", 42);
}
```
