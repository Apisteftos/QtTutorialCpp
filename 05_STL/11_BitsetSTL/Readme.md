# BitsetSTL — std::bitset in C++23

`std::bitset<N>` is a fixed-size sequence of N bits with a rich manipulation interface.
Size is a compile-time constant. More readable and safer than manual bitmask operations.

**Bit order:** bit 0 is the **rightmost** (least significant) bit; in the string form the leftmost character is the
highest bit. `std::bitset<8> b("00001000")` has `b[3] == true` (tested).

---

## Header

```cpp
#include <bitset>
```

---

## Declaration

```cpp
std::bitset<8> b1;                    // all zeros
std::bitset<8> b2(0b10110100);        // from integer literal
std::bitset<8> b3(0xA5);              // from hex
std::bitset<8> b4("11001010");        // from string (MSB = leftmost char)
```

Details (all tested):

- a value wider than `N` is truncated: `std::bitset<8>(0xFFFF)` is `11111111`
- a string longer than `N` uses its **first** `N` characters: `std::bitset<4>("101011")` is `1010`
- a character other than `0`/`1` in the string throws `std::invalid_argument`

Size `N` must be a compile-time constant.

---

## Setting, clearing, flipping

```cpp
b.set(3)     // set bit 3 to 1 (throws std::out_of_range if 3 >= N)
b.set(3, false) // set bit 3 to the given value (same as reset(3))
b.set()      // set ALL bits to 1
b.reset(3)   // clear bit 3 to 0
b.reset()    // clear ALL bits to 0
b.flip(3)    // toggle bit 3
b.flip()     // toggle ALL bits
```

---

## Accessing bits

```cpp
b[3]         // no bounds check (UB if out of range); returns a proxy object on a non-const bitset
b.test(3)    // bounds checked — throws std::out_of_range

b.count()    // number of set bits (1s)
b.size()     // total number of bits (always N)
b.any()      // true if any bit is 1
b.all()      // true if all bits are 1
b.none()     // true if no bits are 1
```

The proxy returned by `b[i]` can be assigned to (`b[1] = true; b[2] = b[1];` works, tested), but it has no
`std::formatter`: `std::println("{}", b[1])` does not compile — write `static_cast<bool>(b[1])` or use `b.test(1)`.
The same is true for the whole bitset: **`std::println("{}", b)` does not compile** (no formatter for `std::bitset`, tested)
— print `b.to_string()`.

---

## Bitwise operators

```cpp
x & y    // AND
x | y    // OR
x ^ y    // XOR
~x       // NOT (flip all)
x << 2   // left shift  (towards the MSB; bits shifted out are lost)
x >> 2   // right shift (towards bit 0)

x &= y; x |= y; x ^= y;  // in-place versions
x == y; x != y;          // comparison — but NO <, <=, >, >=
```

Shifting by `N` or more is well-defined and gives all zeros (`bitset<8>(0xFF) << 8` → `00000000`, tested),
unlike shifting a built-in integer by its width. Because there is no `operator<`, a `bitset` cannot be a key of
`std::set` / `std::map` directly (compile error, tested); `std::hash<std::bitset<N>>` exists for `unordered_*`.

---

## Conversion

```cpp
b.to_ulong()    // to unsigned long — throws std::overflow_error if the value does not fit
b.to_ullong()   // to unsigned long long — same (tested with bitset<100> and bit 99 set)
b.to_string()   // to "10110100" string
b.to_string('O', 'I')  // custom chars for 0/1
```

`to_ulong()` returns a number, so print it in the base you mean: `std::println("{:o}", perms.to_ulong())` for octal,
`"0x{:x}"` for hex. (The old `main.cpp` printed the decimal value 492 under the label "octal value"; the correct octal
value of that permission set is 754.)

---

## Permission flags

```cpp
enum Perm { EXEC=0, WRITE=1, READ=2 };   // bit 0 = x, bit 1 = w, bit 2 = r (like chmod: r=4, w=2, x=1)
std::bitset<3> perms;

perms.set(READ);
perms.set(WRITE);

perms.test(READ)   // true
perms.test(EXEC)   // false
perms.to_ulong()   // 6 (binary 110) — rw-
```

---

## Feature flags

```cpp
enum Feature { DARK_MODE=0, NOTIFICATIONS=1, ANALYTICS=2 };
std::bitset<8> features;

features.set(DARK_MODE);
features.set(NOTIFICATIONS);

features.test(DARK_MODE)     // true
features.flip(DARK_MODE)     // toggle
features.reset(NOTIFICATIONS) // disable
```

---

## Sieve of Eratosthenes

```cpp
constexpr int N = 1000;
std::bitset<N+1> isPrime;
isPrime.set();
isPrime.reset(0); isPrime.reset(1);

for (int i = 2; i*i <= N; ++i)
    if (isPrime.test(i))
        for (int j = i*i; j <= N; j += i)
            isPrime.reset(j);

isPrime.count()  // number of primes up to N: 168 for N = 1000 (tested)
```

**Large sieves:** a `std::bitset` stores its bits inside the object, so a local `std::bitset<100'000'000>` (12.5 MB)
overflows the default 8 MB stack and **crashes** (segmentation fault, tested). Make it `static` / global (that worked:
5,761,455 primes below 10⁸), or use `std::vector<bool>` / `std::vector<std::uint64_t>`.

---

## bitset vs uint32_t bitmask

| | `uint32_t` | `std::bitset<N>` |
|--|-----------|-----------------|
| Max bits | 32 (`uint64_t`: 64) | Any N |
| `count()` set bits | `std::popcount` (C++20, `<bit>`) or `__builtin_popcount` | ✅ built-in |
| Bounds check | ❌ | ✅ with `test()` |
| String output | ❌ | ✅ `to_string()` |
| Arithmetic ops | ✅ | ❌ |
| C interop | ✅ | via `to_ulong()` |
| Runtime size | ✅ (bit width fixed by the type, which of the bits you use is up to you) | ❌ compile-time only |
| Printing | `std::println("0x{:x}", flags)` | `b.to_string()`, no formatter |
| Constexpr | ✅ | ✅ for construction, `test`, `count`, `[]` (tested; C++23 makes the modifying members constexpr too) |

---

## When to use bitset

✅ Fixed-size bit flags (permissions, features, status)
✅ Need `count()` of set bits (or use `std::popcount` on an integer)
✅ Sieve of Eratosthenes
✅ Debugging — easy string representation
✅ More than 64 bits needed

❌ Size not known at compile time → use `std::vector<bool>` or `uint64_t`
❌ Need arithmetic on the value → use `uint32_t`/`uint64_t`
❌ C interoperability → use plain integer bitmask

---

## bitset vs uint32_t bitmask — summary

`std::bitset` advantages: `count()`, `any`/`all`/`none` read well, string output for debugging, bounds checking with `test()`, any size.
`uint32_t` advantages: arithmetic operations, C interop, `std::formatter` support, no proxy objects.

---

## Notes moved from main.cpp / Corrections

- `main.cpp` now uses `std::println`/`std::print` and `std::` everywhere; `<iostream>` is gone, `<cstdint>` and `<stdexcept>` were added. The old file used `uint32_t` without `<cstdint>` and **did not compile on GCC 13** (`'uint32_t' was not declared`); it is now `std::uint32_t` with the include.
- `std::bitset` has no formatter, so all bitsets are printed with `.to_string()`; `c[0]` etc. are converted with `static_cast<bool>`. The advantages lists printed at the end of Example 10 moved into this README.
- **Bug fixed:** Example 6 printed `perms.to_ulong()` (decimal 492) under the label "octal value". It now prints `{:o}` → `754` (rwxr-xr--). Example 10 printed the `uint32_t` in hex and the bitset in decimal; both are hex now.
- The old status-flags example (protocol-specific flag names) is now a neutral "Connection Status Flags" example; the unused enumerator `FEATURE_COUNT` was removed.
- README: the permission-flag sketch said `to_ulong()` is `3 (binary 011)` for READ|WRITE, which only holds if READ=0, WRITE=1; it now uses chmod-like bit positions and says `6`. The table claimed `uint32_t` has "Max bits 64" — it has 32 (`uint64_t` has 64). `const int N` became `constexpr`.

## What was tested

GCC 13.3, `-std=c++23 -Wall -Wextra -Wshadow`, ASan + UBSan: all of `main.cpp` (the original with `-include cstdint` as baseline; output identical apart from `true`/`false`, the formatter workarounds, the corrected octal value, the neutral status example, the hex output and the removed advantages text); constructor truncation rules and the invalid-character exception; `to_ulong` overflow; shifts by ≥ N; `==` vs missing `<`; `std::set<bitset>` and `std::format("{}", bitset)` rejected; proxy assignment; `std::popcount` agrees with `count()`; constexpr reads; sieve results (168 primes ≤ 1000, 5,761,455 below 10⁸) and the stack overflow of a local 100M-bit bitset.

## Not tested

Performance (memory is N/8 bytes rounded up to a word, runtime speed not measured), behaviour on other standard libraries (exception messages differ), `std::bitset` with N > 64 beyond the cases above.
