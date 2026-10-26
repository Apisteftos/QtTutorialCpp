# IomanipSTL — I/O Manipulation in C++23

`<iomanip>` provides stream manipulators for formatting output.
Most manipulators are **sticky** — they stay active until changed.
Exception: `setw` resets after each use.

> Examples 1–9 in `main.cpp` use `std::cout` on purpose: these manipulators work on
> stream objects and cannot be passed to `std::println`. Example 10 shows the
> `std::println` equivalents.

---

## Header

```cpp
#include <iomanip>
#include <iostream>
```

---

## setw — field width

```cpp
std::cout << std::setw(10) << "hello" << '\n';  // "     hello"
std::cout << std::setw(10) << 42      << '\n';  // "        42"
```

`setw` affects **only the next output** — not sticky.

---

## setfill — fill character

```cpp
std::cout << std::setfill('0') << std::setw(6) << 42;   // "000042"
std::cout << std::setfill('*') << std::setw(8) << "hi"; // "******hi"
std::cout << std::setfill(' ');  // reset to default
```

`setfill` is sticky — stays until changed.

---

## Alignment

```cpp
std::cout << std::right    << std::setw(10) << "text";  // "      text"
std::cout << std::left     << std::setw(10) << "text";  // "text      "
std::cout << std::internal << std::setw(10) << -42;     // "-       42"
```

---

## setprecision — floating point digits

```cpp
double pi = 3.14159265;
std::cout << std::setprecision(2) << pi;   // 3.1
std::cout << std::setprecision(6) << pi;   // 3.14159 (default)
```

---

## fixed, scientific, defaultfloat

```cpp
double v = 123456.789;

std::cout << std::defaultfloat << v;                         // 123457
std::cout << std::fixed        << std::setprecision(2) << v; // 123456.79
std::cout << std::scientific   << std::setprecision(2) << v; // 1.23e+05

std::cout << std::defaultfloat << std::setprecision(6);  // reset
```

All are **sticky**.

---

## boolalpha

```cpp
std::cout << std::boolalpha   << true << " " << false;  // "true false"
std::cout << std::noboolalpha << true << " " << false;  // "1 0"
```

---

## Numeric bases

```cpp
int n = 255;
std::cout << std::dec      << n;   // 255
std::cout << std::hex      << n;   // ff
std::cout << std::oct      << n;   // 377
std::cout << std::showbase << std::hex << n;  // 0xff
std::cout << std::uppercase << std::hex << n; // 0XFF  (showbase is still on!)
std::cout << std::showpos   << 42;            // 0X2A  (still hex, and showpos has no effect in hex)
```

All of these are sticky, so run in sequence the last two lines do **not** print `FF` / `+42`.
Reset first: `std::cout << std::dec << std::noshowbase << std::nouppercase;` — then
`std::uppercase << std::hex << 255` gives `FF` and `std::showpos << 42` gives `+42`.

---

## Formatted table pattern

```cpp
std::cout << std::left  << std::setw(16) << "Name"
          << std::right << std::setw(8)  << "Qty"
          << std::fixed << std::setprecision(2)
          << std::setw(10) << "Price" << '\n';

for (const auto& p : products) {
    std::cout << std::left  << std::setw(16) << p.name
              << std::right << std::setw(8)  << p.qty
              << std::setw(10) << p.price << '\n';
}
```

---

## Zero-padded numbers

```cpp
for (int i = 1; i <= 99; ++i)
    std::cout << std::setfill('0') << std::setw(3) << i << '\n';
// 001, 002, ..., 099
std::cout << std::setfill(' ');   // setfill is sticky — reset it afterwards
```

---

## Hex dump

```cpp
for (unsigned char b : buffer)
    std::cout << std::uppercase << std::hex
              << std::setfill('0') << std::setw(2)
              << (int)b << ' ';
std::cout << std::dec << std::nouppercase;
```

---

## C++23 std::println — modern alternative

```cpp
#include <print>

std::println("{:.2f}", 3.14159);       // 3.14
std::println("{:10.4f}", 3.14159);     // "    3.1416"
std::println("{:<10.2f}|", 3.14);      // "3.14      |"
std::println("{:010.2f}", 3.14);       // "0000003.14"
std::println("{:+.2f}", 3.14);         // "+3.14"
std::println("{:x}", 255);             // "ff"
std::println("{:08b}", 42);            // "00101010"
```

`std::println` is preferred for new code — cleaner, type-safe, and the format spec lives
in the call itself, so it never reads or changes the stream's sticky flags.

---

## Sticky vs non-sticky manipulators

| Manipulator | Sticky? |
|-------------|---------|
| `setw` | ❌ resets after each use |
| `setfill` | ✅ |
| `setprecision` | ✅ |
| `fixed/scientific` | ✅ |
| `left/right/internal` | ✅ |
| `boolalpha` | ✅ |
| `hex/dec/oct` | ✅ |
| `showbase/showpos` | ✅ |
| `uppercase` | ✅ |

---

## Notes moved from main.cpp

- `main.cpp` has no `using namespace std;` — every standard name is written with `std::`.
- Examples 1–9 intentionally stay on `std::cout`; only titles, blank lines and Example 10 use `std::println`.
- Behaviour of `main.cpp` is unchanged: output is byte-identical to the original file.
- The unused `<cmath>` include was dropped; `<string_view>` was added for the small `banner` helper.
- The hex-dump cast is now `static_cast<int>(b)` instead of `(int)b`. The cast matters: an
  `unsigned char` streamed directly prints as a character (`'A'`), not a number.

## Corrections to the original README

- The "Numeric bases" snippet was wrong when read as a sequence: manipulators are sticky, so
  the `uppercase` line prints `0XFF` and the `showpos` line prints `0X2A`, not `FF` / `+42`.
- "No sticky state issues" for `std::println` is true for the format itself, but it also means
  `println` **ignores** flags set on `std::cout` (see tested list).
- The zero-padding loop leaves `setfill('0')` active; the snippet now resets it.

## Extra gotchas (all tested)

- `setw` never truncates: `setw(3) << "toolong"` prints `toolong`.
- `setw` applies to the next formatted output only: `setw(5) << "a" << "b"` gives `    ab`.
  It is consumed even by an empty string: `setw(4) << "" << setw(4) << 1` gives `       1`.
- `showbase` with value 0 prints just `0` (no `0x`).
- `showpos` has no effect on hex output (`showpos << hex << 255` gives `ff`).
- `uppercase` also affects the prefix and exponent: `0XFF`, `1.234568E+05`.
- `internal` with `showbase`/hex puts padding between prefix and digits: `0x......ff` (fill `.`).
- Zero fill with default (right) alignment puts the zeros before the sign: `setfill('0') << setw(6) << -42`
  gives `000-42`; adding `std::internal` gives `-00042`.
- With `defaultfloat`, `setprecision(n)` counts significant digits; with `fixed`/`scientific` it counts
  digits after the point. `setprecision(2)` of `123456.789` gives `1.2e+05`; of `0.000123456` gives `0.00012`.
- `fixed` does not affect integers: `fixed << setprecision(2) << 3` prints `3`.
- `setiosflags(std::ios::hex)` alone does **not** switch to hex (the decimal bit is still set); use
  `std::hex` or `setiosflags` together with `resetiosflags(std::ios::basefield)`.
- `std::println("{}", 255)` prints `255` even while `std::cout` is in `std::hex` mode.
- `std::println("{}", 3.14159265)` prints the shortest round-trip text `3.14159265`, whereas `cout`
  with default precision prints `3.14159`.
- Other `<iomanip>` items: `std::setbase(16)` (prints `ff`), `std::quoted("a \"q\"")` (prints `"a \"q\""`),
  `std::put_time(&tm, "%Y-%m-%d %H:%M:%S")` (prints `2026-10-26 19:33:05` for a hand-filled `tm`).
- `println` has options `cout`+`iomanip` lacks: centre alignment `{:^10}`, fill+align in one spec `{:*^10}`,
  `{:#x}` → `0xff`, `{:#o}` → `0377`, binary `{:b}`.

## What was tested

GCC 13.3, `-std=c++23 -Wall -Wextra -Wshadow -fsanitize=address,undefined`, with a small test-only
`<print>` shim (GCC 13 has no `<print>`); build clean, no sanitizer reports. Original and new output
compared with `diff`, identical, also when piped. All README snippets and the list above were run.

## Not tested

- Real `<print>` from GCC 14+ / Clang with libc++ (only the shim was used).
- Locale-dependent output (`std::put_time`, `{:L}`, thousands separators) beyond the default "C" locale.
- MSVC / Clang behaviour.
