# `[[noreturn]]`: this function never comes back

> **Intent:** Declare that a function never returns to its caller, because it always throws or ends the program.

**Since:** C++11

## Examples in `main.cpp`

| # | What | Result |
|---|---|---|
| 1 | `[[noreturn]] void fail(const std::string&)` that always throws, used at the end of `safeDivide` | no `return` needed after the call, no warning |
| 2 | The same helper without the attribute | `-Wreturn-type`: control reaches end of non-void function |
| 3 | A `[[noreturn]]` function that can return | warning, and undefined behavior if it does |
| 4 | Standard functions | `std::exit`, `std::abort`, `std::terminate`, `std::quick_exit` never return |

```cpp
[[noreturn]] void fail(const std::string& message)
{
    throw std::runtime_error(message);
}

int safeDivide(int a, int b)
{
    if (b != 0) {
        return a / b;
    }
    fail("division by zero");      // compiler knows: nothing after this line is reached
}
```

## What the compiler says

Compiling the commented-out versions (marked `//!`) without the attribute or with a wrong one (GCC 15.2.0, `-Wall -Wextra`):

```text
warning: control reaches end of non-void function [-Wreturn-type]
warning: 'noreturn' function does return
```

## Notes

- A `[[noreturn]]` function that **does** return is undefined behavior. The compiler may even remove the code after the call.
- Throwing is fine: the function does not return normally, the exception leaves it.
- The attribute also lets the optimizer drop the code after the call and treat the call path as rarely taken.
- In C++23, `std::unreachable()` (`<utility>`) marks a point in the code that must never be reached.

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`): GCC 14+ or a recent Clang/MSVC.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o noreturn
./noreturn
```

or with CMake:

```bash
cmake -S . -B build
cmake --build build
./build/noreturn
```

## Output (GCC 15.2.0)

```text
WARNING: ld.so: invalid GLIBC_TUNABLES value `default' for option `glibc.malloc.mmap_threshold': ignored.
WARNING: ld.so: invalid GLIBC_TUNABLES value `default' for option `glibc.malloc.trim_threshold': ignored.
=== EXAMPLE 1: HELPER THAT ALWAYS THROWS ===
safeDivide(10, 2) = 5
caught: division by zero

=== EXAMPLE 2: WITHOUT THE ATTRIBUTE ===
see Readme.md: the compiler cannot know that failPlain() never returns

=== EXAMPLE 3: A NORETURN FUNCTION THAT RETURNS ===
see Readme.md: returning from a [[noreturn]] function is undefined behavior

=== EXAMPLE 4: STANDARD NORETURN FUNCTIONS ===
std::exit, std::abort, std::terminate, std::quick_exit and
std::unreachable (C++23) are all [[noreturn]] or never return
```

## Tested

- Builds without warnings with GCC 15.2.0, `-std=c++23 -Wall -Wextra`; also with `-O2 -DNDEBUG`. Runs clean under AddressSanitizer and UBSan.
- The warnings shown above were produced by compiling a copy of `main.cpp` with the `//!` lines enabled.
- Not tested with Clang or MSVC.

## Reference

- Attribute `[[noreturn]]`, C++11. Standard noreturn functions in `<cstdlib>` and `<exception>`; `std::unreachable` in `<utility>` (C++23).
