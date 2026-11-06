# `[[maybe_unused]]`: an unused name is fine here

> **Intent:** Tell the compiler that a variable, parameter, function or structured binding may stay unused, so `-Wunused-*` stays quiet.

**Since:** C++17

## Examples in `main.cpp`

| # | What | Why the name can be unused |
|---|---|---|
| 1 | Function parameter | the signature is fixed by a callback or interface, but this implementation does not need the parameter |
| 2 | Local variable | it is only used inside `assert()`, which disappears with `-DNDEBUG` |
| 3 | Structured binding | same, both names are only used in an `assert()` |
| 4 | Function | a helper that is only used in some build configurations |

```cpp
void onKey([[maybe_unused]] int keyCode, bool pressed);
[[maybe_unused]] int result = compute();
[[maybe_unused]] auto [low, high] = makeRange();
[[maybe_unused]] static void debugDump(int value);
```

## What the compiler says

Compiling the commented-out versions (marked `//!`) without the attribute (GCC 15.2.0, `-Wall -Wextra`; the `assert` cases need `-DNDEBUG`):

```text
warning: unused parameter 'keyCode' [-Wunused-parameter]
warning: unused variable 'other' [-Wunused-variable]                                     (-DNDEBUG)
warning: structured binding declaration set but not used [-Wunused-but-set-variable]    (-DNDEBUG)
warning: 'void debugDumpBad(int)' defined but not used [-Wunused-function]
```

## Alternatives

| Way | Notes |
|---|---|
| Omit the parameter name: `void onKey(int, bool)` | simplest for parameters, nothing to document |
| `(void)keyCode;` | works in every standard, but it is a statement in the body |
| `[[maybe_unused]]` | says it at the declaration, works for variables, functions, types, enumerators and members |

## Notes

- The attribute does not stop you from using the name. It only suppresses the warning.
- Do not put it everywhere. A real unused variable is often a bug, and the warning is useful.
- GCC warns about a structured binding only if all of its names are unused.
- With `-Wall -Wextra` the warnings in the good build are zero in both normal and `-DNDEBUG -O2` builds.

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`): GCC 14+ or a recent Clang/MSVC.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o maybe_unused
./maybe_unused
```

or with CMake:

```bash
cmake -S . -B build
cmake --build build
./build/maybe_unused
```

## Output (GCC 15.2.0)

```text
WARNING: ld.so: invalid GLIBC_TUNABLES value `default' for option `glibc.malloc.mmap_threshold': ignored.
WARNING: ld.so: invalid GLIBC_TUNABLES value `default' for option `glibc.malloc.trim_threshold': ignored.
=== EXAMPLE 1: UNUSED PARAMETER ===
key pressed? true

=== EXAMPLE 2: ONLY USED IN assert() ===
compute() was called

=== EXAMPLE 3: STRUCTURED BINDING ===
range created

=== EXAMPLE 4: UNUSED FUNCTION ===
debugDump is defined but never called, and that is fine
```

## Tested

- Builds without warnings with GCC 15.2.0, `-std=c++23 -Wall -Wextra`; also with `-O2 -DNDEBUG`. Runs clean under AddressSanitizer and UBSan.
- The warnings shown above were produced by compiling a copy of `main.cpp` with the `//!` lines enabled.
- Not tested with Clang or MSVC.

## Reference

- Attribute `[[maybe_unused]]`, C++17. `assert` is in `<cassert>`.
