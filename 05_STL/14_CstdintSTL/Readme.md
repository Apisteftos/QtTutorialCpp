# CstdintSTL — Fixed-Width Integer Types in C++23

`<cstdint>` gives integer types with a known size, so binary layouts, file formats and
hardware registers do not depend on what `int` or `long` happen to be on a platform.

---

## Header

```cpp
#include <cstdint>   // std::int8_t ... std::uintmax_t, plus the limit macros
#include <cstddef>   // std::size_t, std::ptrdiff_t
#include <limits>    // std::numeric_limits<T>
```

Write the types with `std::` (`std::uint32_t`). The limit macros (`INT32_MAX` ...) are
preprocessor macros and have no namespace.

---

## The type families

| Family | Meaning | Example |
|--------|---------|---------|
| Exact width | exactly N bits, **optional** (exists only if the platform has it) | `std::int8_t`, `std::uint32_t` |
| Minimum width | smallest type with at least N bits | `std::int_least16_t` |
| Fastest | a type with at least N bits that the implementation considers fastest | `std::int_fast32_t` |
| Pointer-sized | can hold a pointer value (optional) | `std::intptr_t`, `std::uintptr_t` |
| Maximum width | the widest integer type | `std::intmax_t`, `std::uintmax_t` |

On one x86-64 Linux GCC run: `int_least8/16/32/64_t` are 1/2/4/8 bytes, `int_fast8_t` is 1 byte
but `int_fast16_t`, `int_fast32_t`, `int_fast64_t` are all 8 bytes. Other platforms differ — that is
exactly why `int_least` / `int_fast` exist.

---

## Exact-width types: what is really guaranteed

- `std::int8_t` is the same type as `signed char`, `std::uint8_t` the same as `unsigned char`
  (true with GCC on Linux; the standard only requires "an integer type of that width").
- "sizeof is always 1/2/4/8" assumes 8-bit bytes (`CHAR_BIT == 8`), which is true on every common platform.
- Signed exact-width types are two's complement.

---

## Printing 8-bit values

```cpp
std::uint8_t u = 65;
std::cout << u;                         // prints the character 'A'
std::cout << +u;                        // 65  (unary + promotes to int)
std::cout << static_cast<unsigned>(u);  // 65
std::println("{}", u);                  // 65  — println formats it as a number
```

`std::println` / `std::format` treat only plain `char` as a character; `signed char` and
`unsigned char` (and so `int8_t` / `uint8_t`) print as numbers, so no cast is needed there.

---

## Overflow

- **Unsigned** arithmetic wraps modulo 2^N — well defined: `std::uint8_t w = 250; w += 10;` gives `4`;
  `--(std::uint32_t{0})` gives `4294967295`.
- **Small types are promoted to `int` first**: `std::int8_t i = 127; i++;` computes `127 + 1 = 128` in `int`
  and converts back. Since C++20 that conversion is modular, so the result is `-128` and it is **not**
  undefined behavior (the comment in the old `main.cpp` called it UB).
- **Real signed overflow** is undefined behavior: `int x = INT_MAX; x += 1;` — UBSan reports
  `signed integer overflow`.
- `static_cast<std::int8_t>(300)` keeps the low 8 bits and gives `44`.
- Mixing signed and unsigned is a classic trap: `std::int32_t{-1} < std::uint32_t{1}` is `false`
  (the `-1` is converted to a huge unsigned value). `-Wsign-compare` warns about it.

---

## Limit macros and their types

`INT8_MAX`, `UINT8_MAX`, `INT16_MAX` and `INT32_MIN` have type `int`; `UINT32_MAX` has type
`unsigned int`; `INT64_MAX` has type `long` (on this platform). So the old `(int)INT8_MAX` casts
in the demo were not needed.

## std::numeric_limits

Use `std::numeric_limits<std::int32_t>::max()` in templates — it works with any type, and
`::digits` gives the number of value bits (`31` for `int32_t`, `7` for `int8_t`, `8` for `uint8_t`).
With `println`, `float` and `double` print as the shortest text that round-trips, e.g.
`1.1920929e-07` for `float` epsilon.

---

## Practical: binary packet header

```cpp
struct PacketHeader {
    std::uint8_t  version;
    std::uint8_t  type;
    std::uint16_t length;
    std::uint32_t sequence;
    std::uint32_t deviceId;
    std::uint64_t timestamp;
} __attribute__((packed));          // GCC/Clang only

static_assert(sizeof(PacketHeader) == 20);
```

- Without `packed` the compiler inserts padding: the 20 bytes of fields become **24** on this platform.
- `__attribute__((packed))` is a GCC/Clang extension (MSVC uses `#pragma pack`). Packed members may be
  misaligned, so **do not bind a reference or pointer to them** — copy to a local first (the demo does
  this before calling `println`).
- Byte order is not handled by fixed-width types: a header written on a little-endian machine is
  not portable to a big-endian one without converting.
- Always protect layout assumptions with `static_assert(sizeof(...) == N)`.

## Bit-fields (register simulation)

```cpp
struct StatusRegister {
    std::uint8_t enabled  : 1;
    std::uint8_t ready    : 1;
    std::uint8_t error    : 1;
    std::uint8_t busy     : 1;
    std::uint8_t reserved : 4;
};
```

- `sizeof` is 1 byte with GCC here, but **bit-field layout is implementation-defined** — do not use it
  for data that must match a wire format or a hardware spec on other compilers.
- A bit-field cannot be bound to a reference, so `std::println("{}", reg.enabled)` does not compile
  (`cannot bind bit-field ... to 'unsigned char&'`). Convert first: `static_cast<int>(reg.enabled)`.

---

## Notes moved from main.cpp

- `main.cpp` uses `std::` everywhere (no `using namespace std;`) and `std::println` instead of `std::cout`.
- Printed remarks moved here: *"int_least types may be larger than the minimum on some platforms"* and
  *"int_fast types may be larger, for CPU register alignment"* (see the table above), and the
  field-size breakdown of the packet header (1 + 1 + 2 + 4 + 4 + 8 = 20 bytes, now checked by `static_assert`).
- The previous domain-specific frame example was replaced by a neutral binary packet header
  (`PacketHeader`) and a neutral register (`enabled / ready / error / busy`).
- Unused includes (`<iostream>`, `<climits>`) were removed. The pointer addresses printed in
  Example 6 change from run to run.

## Corrections

- "Wraps to -128 (undefined behavior)" was wrong: `int8_t` increment is a well-defined modular
  conversion since C++20; only overflow of `int` and wider is UB.
- "uint8_t prints as char — cast to int" applies to `std::cout`, not to `std::println`.
- Exact-width types are optional and their "always N bytes" assumes `CHAR_BIT == 8`.

## What was tested

GCC 13.3, `-std=c++23 -Wall -Wextra -Wshadow -fsanitize=address,undefined`, with a small test-only
`<print>` shim (GCC 13 has no `<print>`). Output compared with the original by `diff`: only the
intended differences (neutral Example 10 labels, two removed remark lines, changing pointer values).
Every claim in this README about sizes, printing, overflow, limit-macro types and packed sizes was
run on this machine.

## Not tested

- Platforms other than x86-64 Linux / GCC (sizes of `int_fast*`, bit-field layout, absence of exact-width types).
- Real `<print>` from GCC 14+ / libc++, MSVC.
