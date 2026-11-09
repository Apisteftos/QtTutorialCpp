# C++23: Standard Library Modules

Back to [main README](README.md)

| Feature | Standard |
|---|---|
| `import std;` | C++23 (new) |
| `import std.compat;` | C++23 (new) |

## Notes

- `import std;` makes everything in namespace `std` available (including `std::println`, `std::expected`, ...), without including headers.
- `import std.compat;` additionally exports the C library names in the global namespace (`::printf`, `::size_t`, ...).
- The language feature "modules" itself is **C++20**. Only the standard library modules are new in C++23.
- Build system support is the hard part. With CMake it is enabled through `CXX_MODULE_STD` and needs a recent CMake plus a Ninja or Visual Studio generator. Check the CMake documentation for your version, because the support was experimental for a long time.

## Example

```cpp
import std;

int main() {
    std::println("Hello, modules");
}
```
