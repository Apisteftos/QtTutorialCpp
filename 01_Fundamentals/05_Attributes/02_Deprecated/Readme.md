# `[[deprecated]]`: warn when old code is used

> **Intent:** Mark a function, type, variable or namespace as obsolete so that every use produces a compiler warning, ideally with a hint what to use instead.

**Since:** C++14 (on more entity kinds such as enumerators, namespaces and type aliases since C++17)

## Examples in `main.cpp`

| # | What | Declaration |
|---|---|---|
| 1 | Function | `[[deprecated("use sumAll(a, b) instead")]] int add(int, int);` |
| 2 | Class, enumerator, type alias | `class [[deprecated("...")]] OldLogger`, `Grey [[deprecated("use Gray")]] = 5`, `using Text [[deprecated("...")]] = std::string;` |
| 3 | Variable and namespace | `[[deprecated("...")]] constexpr int MAX_USERS`, `namespace [[deprecated("...")]] legacy` |
| 4 | Calling deprecated code on purpose | `#pragma GCC diagnostic ignored "-Wdeprecated-declarations"` around the call |

The message text is optional: `[[deprecated]]` alone also works, but the text is what tells the user what to do.

## What the compiler says

Compiling the calls that are commented out with `//!` in `main.cpp` (GCC 15.2.0, `-Wall -Wextra`):

```text
warning: 'int add(int, int)' is deprecated: use sumAll(a, b) instead [-Wdeprecated-declarations]
warning: 'OldLogger' is deprecated: use Logger instead [-Wdeprecated-declarations]
warning: 'Color::Grey' is deprecated: use Gray [-Wdeprecated-declarations]
warning: 'using Text = std::string' is deprecated: use std::string [-Wdeprecated-declarations]
warning: 'MAX_USERS' is deprecated: use kMaxUsers [-Wdeprecated-declarations]
warning: 'legacy' is deprecated: moved to namespace util [-Wdeprecated-declarations]
```

## How to use it in a real library

1. Add the replacement first, then mark the old one `[[deprecated("use X")]]`. Release a MINOR version (see the versioning guide).
2. Keep the old function working and let it call the new one.
3. Remove it in the next MAJOR version.

## Notes

- Only **uses** warn, not the declaration itself.
- `-Werror=deprecated-declarations` turns the warning into an error, useful in CI after the migration deadline.
- The warning text is the only message the user sees, so write it as an instruction: "use X instead".
- To silence a single use, wrap it with `#pragma GCC diagnostic push/ignored/pop` (GCC, Clang) or `#pragma warning(disable : 4996)` (MSVC).

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`): GCC 14+ or a recent Clang/MSVC.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o deprecated
./deprecated
```

or with CMake:

```bash
cmake -S . -B build
cmake --build build
./build/deprecated
```

## Output (GCC 15.2.0)

```text
WARNING: ld.so: invalid GLIBC_TUNABLES value `default' for option `glibc.malloc.mmap_threshold': ignored.
WARNING: ld.so: invalid GLIBC_TUNABLES value `default' for option `glibc.malloc.trim_threshold': ignored.
=== EXAMPLE 1: DEPRECATED FUNCTION ===
sumAll(2, 3) = 5

=== EXAMPLE 2: CLASS, ENUM VALUE, ALIAS ===
[log] new logger
Gray = 5

=== EXAMPLE 3: VARIABLE AND NAMESPACE ===
kMaxUsers = 100
util::twice(21) = 42

=== EXAMPLE 4: SILENCING THE WARNING ===
add(10, 20) = 30 (still works, no warning)
```

## Tested

- Builds without warnings with GCC 15.2.0, `-std=c++23 -Wall -Wextra`; also with `-O2 -DNDEBUG`. Runs clean under AddressSanitizer and UBSan.
- The warnings shown above were produced by compiling a copy of `main.cpp` with the `//!` lines enabled.
- Not tested with Clang or MSVC.

## Reference

- Attribute `[[deprecated]]` / `[[deprecated("reason")]]`, C++14.
