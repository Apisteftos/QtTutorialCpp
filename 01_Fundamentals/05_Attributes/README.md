# 05_Attributes: standard C++ attributes

Attributes are hints written as `[[name]]`. They tell the compiler something about the code: warn here, do not warn there, optimize this way. Compilers that do not know an attribute ignore it. Each folder has `main.cpp`, `Readme.md` and `CMakeLists.txt`, and the code compiles without warnings on GCC 15.2.0 (`-std=c++23 -Wall -Wextra`).

| Folder | Attribute | Since | Purpose | Changes behavior? |
|---|---|---|---|---|
| 01_Nodiscard | `[[nodiscard]]` | C++17 (message and constructors: C++20) | warn when a return value is ignored | no, warning only |
| 02_Deprecated | `[[deprecated("...")]]` | C++14 | warn when old API is used | no, warning only |
| 03_MaybeUnused | `[[maybe_unused]]` | C++17 | an unused name is fine | no, silences a warning |
| 04_Fallthrough | `[[fallthrough]]` | C++17 | falling into the next `case` is intended | no, silences a warning |
| 05_Noreturn | `[[noreturn]]` | C++11 | the function never returns | yes: optimization, and UB if it does return |
| 06_LikelyUnlikely | `[[likely]]`, `[[unlikely]]` | C++20 | hint which branch is usual | no, only code layout |
| 07_NoUniqueAddress | `[[no_unique_address]]` | C++20 | an empty member needs no storage | yes: changes `sizeof` |
| 08_Assume | `[[assume(expr)]]` | C++23 | promise that an expression is true | yes: UB if the promise is false |

## Groups

- **Help the reader and the warnings:** `nodiscard`, `deprecated`, `maybe_unused`, `fallthrough`. These make the compiler warn at the right place, or stay quiet where the code is intended.
- **Help the optimizer:** `noreturn`, `likely`/`unlikely`, `assume`. These can change the generated code. A wrong `noreturn` or `assume` is undefined behavior.
- **Help the layout:** `no_unique_address`.

## Not covered

- `[[carries_dependency]]` (C++11): for the memory model's `consume` ordering, removed in C++26.
- `[[indeterminate]]` (C++26): marks an uninitialized variable as intentional.
- Compiler-specific attributes such as `[[gnu::always_inline]]`, `[[gnu::noinline]]`, `__attribute__((...))`, `__declspec(...)`.

## Build

Every example builds on its own:

```bash
cd 01_Nodiscard
g++ -std=c++23 -Wall -Wextra main.cpp -o example && ./example
```

The `//!` lines in several `main.cpp` files are the calls that would warn. Remove the `//! ` prefix to see the warning yourself. (`01_Nodiscard` uses `//` with the text "would warn" for the same purpose.)
