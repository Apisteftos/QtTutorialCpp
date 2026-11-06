# `[[assume(expression)]]`: promise the optimizer something is true

> **Intent:** Tell the compiler that an expression is true at this point, so it can generate shorter or faster code. The expression is not evaluated.

**Since:** C++23

## Examples in `main.cpp`

| # | What | Promise |
|---|---|---|
| 1 | `remainderOf8(x)` with and without the hint | `x >= 0` |
| 2 | `sumFirst(data, n)` | `n > 0` and `n % 8 == 0` |
| 3 | A wrong promise | undefined behavior (not run) |

```cpp
int remainderOf8(int x)
{
    [[assume(x >= 0)]];
    return x % 8;
}
```

## What the hint changes

Assembly of the two functions at `-O2` (GCC 15.2.0, x86-64):

```text
remainderOf8(int)  with [[assume(x >= 0)]]      remainderOf8NoHint(int)  without
    movl  %edi, %eax                                 movl  %edi, %edx
    andl  $7, %eax                                   sarl  $31, %edx
    ret                                              shrl  $29, %edx
                                                     leal  (%rdi,%rdx), %eax
                                                     andl  $7, %eax
                                                     subl  %edx, %eax
                                                     ret
```

Without the promise the compiler must handle negative numbers (the sign of `-5 % 8` is negative in C++). With the promise one `and` is enough.

## Notes

- **The expression is not checked.** If it is false when the code runs, the behavior is undefined. This is not like `assert`, which stops the program.
- The expression is not evaluated, so it must have no side effects you depend on.
- Use it only for conditions you can prove. A wrong promise can make the optimizer produce code that crashes or silently returns nonsense.
- Check the promise with `assert` in a debug build and add `[[assume]]` only after measuring that it helps.
- Before C++23: `__builtin_assume` (Clang), `__assume` (MSVC), `if (!cond) __builtin_unreachable();` (GCC).
- GCC supports `[[assume]]` since version 13.

## Build and run

`main.cpp` uses `std::println` (C++23, `<print>`): GCC 14+ or a recent Clang/MSVC.

```bash
g++ -std=c++23 -Wall -Wextra main.cpp -o assume
./assume
```

or with CMake:

```bash
cmake -S . -B build
cmake --build build
./build/assume
```

## Output (GCC 15.2.0)

```text
WARNING: ld.so: invalid GLIBC_TUNABLES value `default' for option `glibc.malloc.mmap_threshold': ignored.
WARNING: ld.so: invalid GLIBC_TUNABLES value `default' for option `glibc.malloc.trim_threshold': ignored.
=== EXAMPLE 1: NEVER NEGATIVE ===
remainderOf8(21)       = 5
remainderOf8NoHint(21) = 5

=== EXAMPLE 2: MULTIPLE OF 8 ===
sumFirst(values, 16) = 136

=== EXAMPLE 3: A WRONG PROMISE ===
remainderOf8(-5) would be undefined behavior: the promise x >= 0 is false
(it is not called here)
```

## Tested

- Builds without warnings with GCC 15.2.0, `-std=c++23 -Wall -Wextra`; also with `-O2 -DNDEBUG`. Runs clean under AddressSanitizer and UBSan.
- The warnings shown above were produced by compiling a copy of `main.cpp` with the `//!` lines enabled.
- Not tested with Clang or MSVC.

## Reference

- Attribute `[[assume(expression)]]`, C++23. GCC 13 or newer.
